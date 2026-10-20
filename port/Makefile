# Native PC build: `make -C port` (from the repo root: `make port`) -> build/port/twine.
# Every executable of the game (the boot stub, the movie player, the front end, the eleven levels) becomes a
# module: its decompiled C (src/<module>) and its recompiled MIPS code are compiled for 32-bit x86, linked on
# their own and made private but for the module's descriptor. The runtime (port/src) links them all into one
# program and switches between them like the console does (port/src/main.c). Each executable's PS1 image is
# loaded from the disc at run time.
# Needs the matching builds of the executables (build/<module>/<module>.elf, .exe: `tools/build_all.sh`).

ROOT := ..
PB := $(ROOT)/build/port
RT_OUT := $(PB)/rt
MODULES ?= boot movie shell level01 level02 level03 level04 level05 level06 level07 level08 level09 level10 \
           level11
# C mods built into the levels (port/mods.py): twin-stick controls with the PC's mouse look, and smooth motion
# between the game's 20 Hz ticks
PORT_MODS ?= TWINSTICK SMOOTH

CC := gcc
# gnu89 and -fpermissive: the decompiled C keeps the original's K&R-era idioms. -fwrapv and
# -fno-strict-aliasing: it relies on MIPS integer wraparound and type punning. -O0: the game busy-waits on
# variables its interrupt handlers change (which gcc 2.8 reloaded each time); an optimizing modern gcc
# hoists those loads out of the loops. The game logic is small next to what a PC runs anyway.
CFLAGS := -m32 -std=gnu89 -fpermissive -O0 -g -fno-omit-frame-pointer -fwrapv -fno-strict-aliasing -fno-pie \
          -fno-stack-protector -fcommon -w -DNATIVE_PORT -D_LANGUAGE_C \
          -I$(ROOT)/include -I$(ROOT)/src
RT_CFLAGS := -m32 -std=gnu11 -O2 -g -fno-pie -fno-stack-protector -Wall -Wno-unused-function -DNATIVE_PORT \
             -I$(ROOT)/include -I$(ROOT)/port/src
LDFLAGS := -m32 -no-pie

RT_SRC := $(wildcard $(ROOT)/port/src/*.c)
RT_OBJ := $(patsubst $(ROOT)/port/src/%.c,$(RT_OUT)/%.o,$(RT_SRC))
GEN := gen_native.py psyq_names.txt native_names.txt
MOD_SRC := $(wildcard $(ROOT)/src/mods/*.c $(ROOT)/port/mods/*.c)

all: $(PB)/twine

$(RT_OUT)/%.o: $(ROOT)/port/src/%.c $(wildcard $(ROOT)/port/src/*.h) $(wildcard $(ROOT)/include/*.h)
	@mkdir -p $(dir $@)
	$(CC) $(RT_CFLAGS) -c $< -o $@

# The title card draws per pixel in floating point: SSE2 (vectorized) instead of the x87 unit 32-bit code
# defaults to.
$(RT_OUT)/intro_card.o: RT_CFLAGS += -O3 -msse2 -mfpmath=sse -fno-math-errno
# The rasterizer: every pixel of every frame
$(RT_OUT)/gpu.o: RT_CFLAGS += -O3

# One module: $(1) = its name
define MODULE
$(1)_OUT := $(PB)/$(1)
$(1)_SRC := $(wildcard $(ROOT)/src/$(1)/*.c)
$(1)_OBJ := $$(patsubst $(ROOT)/src/$(1)/%.c,$(PB)/$(1)/game/%.o,$$($(1)_SRC))
$(1)_DEPS := $(ROOT)/build/$(1)/$(1).elf $(GEN) $(wildcard $(ROOT)/config/$(1)_syms.ld)
$(1)_MODFLAGS := $(if $(filter level%,$(1)),$(foreach m,$(PORT_MODS),-DMOD_$(m)))

# Single-file GTE macros are MIPS asm; the native ones come from include/gte_native.h.
$(PB)/$(1)/game/%.c: $(ROOT)/src/$(1)/%.c
	@mkdir -p $$(dir $$@)
	sed -e '/^#define gte_[A-Za-z0-9_]*(.*__asm__/d' $$< > $$@

$(PB)/$(1)/game/%.o: $(PB)/$(1)/game/%.c $(wildcard $(ROOT)/include/*.h) Makefile $(PB)/mods.txt
	$(CC) $(CFLAGS) $$($(1)_MODFLAGS) -c $$< -o $$@

# the mods' objects, and copies of the game objects whose functions they replace
$(PB)/$(1)/mods/objs.txt: $$($(1)_OBJ) $(PB)/mods.txt mods.py mod_names.txt $(ROOT)/tools/build_cmod.py $(MOD_SRC)
	python3 mods.py $(1) $(PB)/$(1) "$(PORT_MODS)" "$(CC) $(CFLAGS) $$($(1)_MODFLAGS)" $$($(1)_OBJ)

# MIPS code that has no C yet (PsyQ libraries, hand-written GTE code, the stub) is statically recompiled
$(PB)/$(1)/recomp.c: $$($(1)_OBJ) $(RT_OBJ) $$($(1)_DEPS) recomp.py $(PB)/$(1)/mods/objs.txt
	@mkdir -p $$(dir $$@)
	python3 recomp.py $(1) $$@ --replaced $(PB)/$(1)/mods/recompiled.txt $$($(1)_OBJ) $(RT_OBJ)

$(PB)/$(1)/recomp.o: $(PB)/$(1)/recomp.c src/recomp.h
	$(CC) -m32 -std=gnu11 -fno-builtin -fpermissive -fno-pie -fno-stack-protector -O1 -g -w -DNATIVE_PORT -I$(ROOT)/include -Isrc \
	    -c $$< -o $$@

$(PB)/$(1)/gen.stamp: $(PB)/$(1)/mods/objs.txt $(RT_OBJ) $(PB)/$(1)/recomp.o $$($(1)_DEPS)
	python3 gen_native.py $(1) $(PB)/$(1) $$$$(cat $(PB)/$(1)/mods/objs.txt) $(PB)/$(1)/recomp.o -- $(RT_OBJ)
	@touch $$@

$(PB)/$(1)/stubs.o $(PB)/$(1)/funcs.o: $(PB)/$(1)/%.o: $(PB)/$(1)/gen.stamp src/module.h
	$(CC) -m32 -std=gnu11 -fno-pie -O1 -w -DNATIVE_PORT -I$(ROOT)/include -Isrc -c $(PB)/$(1)/$$*.c -o $$@

# The module is linked on its own (data symbols at their PS1 addresses, names bound to its functions) and all
# but its descriptor is made local.
$(PB)/$(1)/module.o: $(PB)/$(1)/mods/objs.txt $(PB)/$(1)/recomp.o $(PB)/$(1)/stubs.o $(PB)/$(1)/funcs.o
	ld -m elf_i386 -r -T $(PB)/$(1)/syms.ld -T $(PB)/$(1)/mods/mods.ld $$$$(cat $(PB)/$(1)/mods/objs.txt) \
	    $(PB)/$(1)/recomp.o $(PB)/$(1)/stubs.o $(PB)/$(1)/funcs.o -o $(PB)/$(1)/module.r.o
	objcopy --keep-global-symbol=port_module_$(1) $(PB)/$(1)/module.r.o $$@
endef
$(foreach m,$(MODULES),$(eval $(call MODULE,$(m))))

# rewritten only when the list of mods changes
$(PB)/mods.txt: FORCE
	@mkdir -p $(dir $@)
	@echo "$(PORT_MODS)" > $@.new
	@cmp -s $@.new $@ && rm $@.new || mv $@.new $@

# rewritten only when the list of modules changes
$(PB)/modules.c: FORCE
	@mkdir -p $(dir $@)
	@( echo '/* Generated by port/Makefile: the modules of this build. */'; echo '#include "module.h"'; \
	   for m in $(MODULES); do echo "extern const PortModule port_module_$$m;"; done; \
	   echo 'const PortModule *const port_modules[] = {'; \
	   for m in $(MODULES); do echo "    &port_module_$$m,"; done; \
	   echo '};'; echo 'const int port_module_count = sizeof(port_modules) / sizeof(port_modules[0]);' ) > $@.new
	@cmp -s $@.new $@ && rm $@.new || mv $@.new $@

$(PB)/modules.o: $(PB)/modules.c src/module.h
	$(CC) $(RT_CFLAGS) -c $< -o $@

$(PB)/twine: $(RT_OBJ) $(PB)/modules.o $(foreach m,$(MODULES),$(PB)/$(m)/module.o)
	$(CC) $(LDFLAGS) -o $@ $^ -lSDL3 -lm

# Host tool: the MDEC model on its own, decoding movie frames from the disc image (port/tools/mdectest.c)
mdectest: $(PB)/mdectest
$(PB)/mdectest: tools/mdectest.c src/mdec.c src/mdec.h
	@mkdir -p $(dir $@)
	$(CC) -m32 -std=gnu11 -O2 -g -Wall -DNATIVE_PORT -I$(ROOT)/include -Isrc tools/mdectest.c src/mdec.c -o $@

clean:
	rm -rf $(PB)

.PHONY: all clean mdectest FORCE
.PRECIOUS: $(PB)/%.c
