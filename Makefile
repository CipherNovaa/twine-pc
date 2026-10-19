LEVEL ?= level01
BUILD := build/$(LEVEL)
# C mods: `make cmod CMODS="TWINSTICK ..."` builds every level with the mods into build/<level>_cmod/
# (tools/build_cmod.py): src/mods/<name>.c, plus -DMOD_<NAME> for the levels' own C files. The results are
# shifted and no longer match, which the shiftable build allows.
CMODS ?=
ifneq ($(CMODS),)
BUILD := build/$(LEVEL)_cmod
endif

CROSS   := mips-elf-
AS      := $(CROSS)as
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
SPLAT   := .venv/bin/splat
PYTHON  := .venv/bin/python

GCC_DIR := tools/old-gcc/2.8.1-psx
CPP     := $(GCC_DIR)/cpp
CC1     := $(GCC_DIR)/cc1
MASPSX  := $(PYTHON) tools/maspsx/maspsx.py --aspsx-version=2.56 --expand-div

CPPFLAGS := -P -undef -D__GNUC__=2 -D_LANGUAGE_C -Iinclude -Isrc $(foreach m,$(CMODS),-DMOD_$(m))
CFLAGS   := -quiet -G0
OPTFLAGS := -O2
ASFLAGS  := -EL -march=r3000 -mtune=r3000 -no-pad-sections -G0 -Iinclude
LDFLAGS  := -EL --no-check-sections \
	-T linkers/$(LEVEL).ld \
	-T linkers/$(LEVEL)_undefined_syms_auto.txt \
	-T linkers/$(LEVEL)_undefined_funcs_auto.txt \
	$(addprefix -T ,$(wildcard config/$(LEVEL)_syms.ld))

S_FILES := $(shell find asm/$(LEVEL) -name '*.s' -not -path '*/nonmatchings/*' 2>/dev/null)
C_FILES := $(shell find src/$(LEVEL) -name '*.c' 2>/dev/null)
O_FILES := $(patsubst %.s,$(BUILD)/%.o,$(S_FILES)) $(patsubst %.c,$(BUILD)/%.o,$(C_FILES))
TARGET  := $(BUILD)/$(LEVEL).exe

# Regions the original developers built without optimization.
O0_FILES := text_8002680C text_8002A61C text_8006FA54
$(foreach f,$(O0_FILES),$(eval $(BUILD)/src/$(LEVEL)/$(f).o: OPTFLAGS := -O0))
O1_FILES := text_8005EDF8
$(foreach f,$(O1_FILES),$(eval $(BUILD)/src/$(LEVEL)/$(f).o: OPTFLAGS := -O1))

all: check

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

.SECONDEXPANSION:
$(BUILD)/%.o: %.c $$(wildcard asm/$(LEVEL)/nonmatchings/$$(notdir $$*)/*.s) $$(wildcard include/*.h)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< > $(BUILD)/$*.i
	$(CC1) $(CFLAGS) $(OPTFLAGS) -o $(BUILD)/$*.cc1.s $(BUILD)/$*.i
	$(PYTHON) tools/jtbl_fix.py $(LEVEL) < $(BUILD)/$*.cc1.s > $(BUILD)/$*.jt.s
	$(MASPSX) < $(BUILD)/$*.jt.s > $(BUILD)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(BUILD)/$*.s

$(BUILD)/$(LEVEL).elf: $(O_FILES) linkers/$(LEVEL).ld
	$(LD) $(LDFLAGS) -Map $(BUILD)/$(LEVEL).map -o $@

$(TARGET): $(BUILD)/$(LEVEL).elf
	$(OBJCOPY) -O binary $< $@
	$(PYTHON) tools/fix_exe.py $@

check: $(TARGET)
	@sha1sum -c config/$(LEVEL).sha1

# Levels whose split output is made shiftable (code/data may grow or shrink; see tools/make_shiftable.py).
SHIFTABLE := level01 level02 level03 level04 level05 level06 level07 level08 level09 level10 level11

split:
	$(SPLAT) split config/$(LEVEL).yaml
ifneq ($(filter $(LEVEL),$(SHIFTABLE)),)
	$(MAKE) check
	$(PYTHON) tools/make_shiftable.py $(LEVEL)
	$(MAKE) check
endif

# Full disc image with our built executable(s) swapped in.
DISC := build/disc/twine
disc: check
	@mkdir -p build/disc
	$(PYTHON) tools/make_disc.py build/disc/disc.xml --replace LEVEL01.EXE=build/level01/level01.exe
	tools/mkpsxiso/build/mkpsxiso -y -q -o $(DISC).bin -c $(DISC).cue build/disc/disc.xml
	@sha256sum $(DISC).bin

clean:
	rm -rf $(BUILD)

# Modded disc: matching build + mods (LEVEL01 patches are carried to every level by context match).
#   make mod   -> test disc with every mods/*.patch (boots straight into Level 1)
#   make play  -> normal playable disc with only the developer cheat codes unlocked
MODS ?= $(wildcard mods/*.patch)
MOD_NAME ?= twine_mod
mod: check
	@mkdir -p build/disc
	$(PYTHON) tools/make_disc.py build/disc/$(MOD_NAME).xml --replace \
		$$($(PYTHON) tools/apply_mods.py --all-levels --out-dir build/mod/$(MOD_NAME) $(MODS))
	tools/mkpsxiso/build/mkpsxiso -y -q -o build/disc/$(MOD_NAME).bin -c build/disc/$(MOD_NAME).cue build/disc/$(MOD_NAME).xml
	@echo "disc: build/disc/$(MOD_NAME).cue  (mods: $(MODS))"

play:
	$(MAKE) mod MODS=mods/debug_cheats.patch MOD_NAME=twine_play

# Objects for a C-mod build (used by tools/build_cmod.py).
cmod-objs: $(O_FILES)

# Playable disc with C mods in every level (normal front-end boot): make cmod CMODS=TWINSTICK
LEVELS := level01 level02 level03 level04 level05 level06 level07 level08 level09 level10 level11
cmod:
ifeq ($(CMODS),)
	$(error set CMODS, e.g. make cmod CMODS=TWINSTICK)
endif
	@for l in $(LEVELS); do $(PYTHON) tools/build_cmod.py $$l "$(CMODS)" || exit 1; done
	@mkdir -p build/disc
	$(PYTHON) tools/make_disc.py build/disc/twine_cmod.xml --replace \
		$(foreach l,$(LEVELS),$(shell echo $(l) | tr a-z A-Z).EXE=build/$(l)_cmod/$(l).exe)
	tools/mkpsxiso/build/mkpsxiso -y -q -o build/disc/twine_cmod.bin -c build/disc/twine_cmod.cue build/disc/twine_cmod.xml
	@echo "disc: build/disc/twine_cmod.cue  (C mods: $(CMODS))"

.PHONY: all check split clean disc mod play cmod cmod-objs
