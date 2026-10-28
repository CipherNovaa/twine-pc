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
# between the game's 20 Hz ticks, and a quick punch with the left hand
PORT_MODS ?= TWINSTICK SMOOTH QUICKPUNCH QUICKGRENADE WEAPONKEYS SAVESTATE DEBUGMENU ENDLESS TRACE PEOPLE

CC := gcc
# gnu89 and -fpermissive: the decompiled C keeps the original's K&R-era idioms. -fwrapv and
# -fno-strict-aliasing: it relies on MIPS integer wraparound and type punning. -O0: the game busy-waits on
# variables its interrupt handlers change (which gcc 2.8 reloaded each time); an optimizing modern gcc
# hoists those loads out of the loops. The game logic is small next to what a PC runs anyway.
CFLAGS := -m32 -std=gnu89 -fpermissive -O0 -g -fno-omit-frame-pointer -fwrapv -fno-strict-aliasing -fno-pie \
          -fno-stack-protector -fcommon -w -DNATIVE_PORT -D_LANGUAGE_C \
          -I$(ROOT)/include -I$(ROOT)/src
RT_CFLAGS := -m32 -std=gnu11 -O2 -g -fno-pie -fno-stack-protector -Wall -Wno-unused-function -DNATIVE_PORT \
             -I$(ROOT)/include -I$(ROOT)/port/src -include $(ROOT)/port/src/glibc_compat.h
# -static-libgcc: one library fewer for the systems it is copied to (make dist-linux)
LDFLAGS := -m32 -no-pie -static-libgcc
# what the Windows build changes (below): the partial link, objcopy, the runtime's headers for generated code, the
# module's names made local
LD_R := ld -m elf_i386 -r
OBJCOPY := objcopy
PORT_INC := -Isrc
EXE := $(PB)/twine
MODULE_LOCALIZE := $(OBJCOPY)

# The Windows build (make windows -> build/dist-win/twine): the same sources and objects, cross-compiled with MinGW-w64
# (i686-w64-mingw32) into build/port-win. Compiled without C symbols' leading underscore, so every object's names
# are the Linux build's (port/gen_native.py, recomp.py and mods.py read them with nm as they are); the final link
# bridges them to the Windows libraries' (port/tools/win_link.py). What Linux has and Windows lacks is in the
# platform layer: port/src/platform_win.c, with the POSIX headers it gives in port/src/win/ (before MinGW's on the
# include path). workbench/research/NOTES_windows.md has the design.
ifeq ($(PORT_OS),windows)
WIN_CC := i686-w64-mingw32-gcc
SDL3_VERSION := 3.4.18
SDL3_DIR := $(ROOT)/build/win-deps/SDL3-$(SDL3_VERSION)/i686-w64-mingw32
PB := $(ROOT)/build/port-win
RT_OUT := $(PB)/rt
CC := $(WIN_CC)
# (as on Linux: bit fields laid out as there (the game's structures over PS1 memory), structures returned through memory
# (recompiled code and calls by PS1 address pass the result's address), time_t of 32 bits)
WIN_FLAGS := -fno-leading-underscore -mno-ms-bitfields -fpcc-struct-return -D_USE_32BIT_TIME_T -isystem $(ROOT)/port/src/win
CFLAGS += $(WIN_FLAGS)
# (-iquote: port/src/io.h is not the C runtime's <io.h>)
RT_CFLAGS := $(subst -I$(ROOT)/port/src,-iquote $(ROOT)/port/src,$(RT_CFLAGS)) $(WIN_FLAGS) -I$(SDL3_DIR)/include
LD_R := i686-w64-mingw32-ld -r -d --oformat pe-i386 -T tools/win_partial.ld
OBJCOPY := i686-w64-mingw32-objcopy
PORT_INC := -iquote src $(WIN_FLAGS)
EXE := $(PB)/twine.exe
MODULE_LOCALIZE := sh tools/win_localize.sh $(OBJCOPY)
LDFLAGS := -m32 -static-libgcc -mwindows -Wl,--large-address-aware,--disable-dynamicbase,--no-insert-timestamp \
           -Wl,--image-base=0x10000
endif

RT_SRC := $(wildcard $(ROOT)/port/src/*.c)
RT_OBJ := $(patsubst $(ROOT)/port/src/%.c,$(RT_OUT)/%.o,$(RT_SRC))
GEN := gen_native.py psyq_names.txt native_names.txt
MOD_SRC := $(wildcard $(ROOT)/src/mods/*.c $(ROOT)/port/mods/*.c $(ROOT)/port/fixes/*.c)

all: $(EXE)

# What a save state leaves as it is (port/src/savestate.c): these objects' data and BSS go in the nosave
# sections, with PORT_NOSAVE variables: the window, input and on-screen text, the audio device, the memory
# cards, the hardware renderer, the films, the recompiled code's stack, the smooth motion's timing and the
# runtime's place (main.c), the console's boot animation, save states themselves.
NOSAVE := video audio memcard gpu_hw hdmovie recomp_rt interp main libpad mdec intro intro_card bios_boot \
          bios_cdrom savestate setup

$(RT_OUT)/%.o: $(ROOT)/port/src/%.c $(wildcard $(ROOT)/port/src/*.h) $(wildcard $(ROOT)/include/*.h) Makefile
	@mkdir -p $(dir $@)
	$(CC) $(RT_CFLAGS) -c $< -o $@
	$(if $(filter $*,$(NOSAVE)),$(OBJCOPY) --rename-section .data=nosave_data --rename-section .bss=nosave_bss $@)

# The title card draws per pixel in floating point: SSE2 (vectorized) instead of the x87 unit 32-bit code
# defaults to.
$(RT_OUT)/intro_card.o: RT_CFLAGS += -O3 -msse2 -mfpmath=sse -fno-math-errno
# The rasterizer: every pixel of every frame
$(RT_OUT)/gpu.o: RT_CFLAGS += -O3

# The hardware renderer's shaders (port/shaders): GLSL compiled to SPIR-V with glslc, built into the program as
# C arrays (shaders.h: hw_vert_spv, hw_frag_spv, ...)
SHADER_SRC := $(wildcard $(ROOT)/port/shaders/*.vert $(ROOT)/port/shaders/*.frag)
SHADER_OUT := $(PB)/shaders
SHADER_SPV := $(patsubst $(ROOT)/port/shaders/%,$(SHADER_OUT)/%.spv,$(SHADER_SRC))

$(SHADER_OUT)/%.spv: $(ROOT)/port/shaders/%
	@mkdir -p $(dir $@)
	glslc -O --target-env=vulkan1.0 $< -o $@

$(SHADER_OUT)/shaders.h: $(SHADER_SPV)
	( cd $(SHADER_OUT) && for f in $(notdir $(SHADER_SPV)); do xxd -i -n $$(echo $$f | tr . _) $$f; done ) | \
	    sed 's/^unsigned/static const unsigned/' > $@.new && mv $@.new $@

$(RT_OUT)/gpu_hw.o: $(SHADER_OUT)/shaders.h
$(RT_OUT)/gpu_hw.o: RT_CFLAGS += -I$(SHADER_OUT)

# What the module generators take from the runtime: the names it defines (gen_native.py's symbol types),
# rewritten only when they change, so a change inside a runtime function only relinks the program.
$(RT_OUT)/defined.txt: $(RT_OBJ)
	@nm $(RT_OBJ) | awk 'NF == 3 && $$2 ~ /^[TWVDdBbRrC]$$/ {print $$2, $$3}' | sort -u > $@.new
	@cmp -s $@.new $@ && rm $@.new || mv $@.new $@

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
$(PB)/$(1)/recomp.c: $$($(1)_OBJ) $(RT_OUT)/defined.txt $$($(1)_DEPS) recomp.py $(PB)/$(1)/mods/objs.txt
	@mkdir -p $$(dir $$@)
	python3 recomp.py $(1) $$@ --replaced $(PB)/$(1)/mods/recompiled.txt $$($(1)_OBJ) $(RT_OBJ)

$(PB)/$(1)/recomp.o: $(PB)/$(1)/recomp.c src/recomp.h
	$(CC) -m32 -std=gnu11 -fno-builtin -fpermissive -fno-pie -fno-stack-protector -O1 -g -w -DNATIVE_PORT -I$(ROOT)/include $(PORT_INC) \
	    -c $$< -o $$@

$(PB)/$(1)/gen.stamp: $(PB)/$(1)/mods/objs.txt $(RT_OUT)/defined.txt $(PB)/$(1)/recomp.o $$($(1)_DEPS)
	python3 gen_native.py $(1) $(PB)/$(1) $$$$(cat $(PB)/$(1)/mods/objs.txt) $(PB)/$(1)/recomp.o -- $(RT_OBJ)
	@touch $$@

$(PB)/$(1)/stubs.o $(PB)/$(1)/funcs.o: $(PB)/$(1)/%.o: $(PB)/$(1)/gen.stamp src/module.h
	$(CC) -m32 -std=gnu11 -fno-pie -O1 -w -DNATIVE_PORT -I$(ROOT)/include $(PORT_INC) -c $(PB)/$(1)/$$*.c -o $$@

# The module is linked on its own (data symbols at their PS1 addresses, names bound to its functions) and all
# but its descriptor is made local.
$(PB)/$(1)/module.o: $(PB)/$(1)/mods/objs.txt $(PB)/$(1)/recomp.o $(PB)/$(1)/stubs.o $(PB)/$(1)/funcs.o
	$(LD_R) -T $(PB)/$(1)/syms.ld -T $(PB)/$(1)/mods/mods.ld $$$$(cat $(PB)/$(1)/mods/objs.txt) \
	    $(PB)/$(1)/recomp.o $(PB)/$(1)/stubs.o $(PB)/$(1)/funcs.o -o $(PB)/$(1)/module.r.o
	$(MODULE_LOCALIZE) --keep-global-symbol=port_module_$(1) $(PB)/$(1)/module.r.o $$@
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
	$(CC) $(LDFLAGS) -Wl,-rpath,'$$ORIGIN/lib' -o $@ $^ -lSDL3 -lm

ifeq ($(PORT_OS),windows)
# Windows: SDL3's MinGW build (zlib licence), fetched once into build/win-deps (port/tools/fetch_sdl3_mingw.sh)
$(SDL3_DIR)/lib/libSDL3.dll.a:
	sh tools/fetch_sdl3_mingw.sh $(SDL3_VERSION) $(ROOT)/build/win-deps

# (Windows' own structures as Windows lays them out)
$(RT_OUT)/platform_win.o: RT_CFLAGS += -mms-bitfields -freg-struct-return

$(RT_OBJ) $(PB)/modules.o: $(SDL3_DIR)/lib/libSDL3.dll.a $(wildcard $(ROOT)/port/src/win/*.h $(ROOT)/port/src/win/sys/*.h)

# what lies before and after the port's own objects (their data and BSS: a save state's memory, platform_win.c)
$(RT_OUT)/win_%.o: $(ROOT)/port/src/win/%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(RT_CFLAGS) -c $< -o $@

WIN_OBJ := $(RT_OUT)/win_first.o $(RT_OBJ) $(PB)/modules.o $(foreach m,$(MODULES),$(PB)/$(m)/module.o) $(RT_OUT)/win_last.o

# the linker script: MinGW's, with PS1 memory's low addresses kept for the image (KUSEG, platform_win.c) and the
# objects' names bridged to the libraries' (port/tools/win_link.py)
$(PB)/twine.ld: $(WIN_OBJ) tools/win_link.py
	python3 tools/win_link.py "$(CC)" $@ $(WIN_OBJ)

$(PB)/twine.exe: $(WIN_OBJ) $(PB)/twine.ld
	$(CC) $(LDFLAGS) -Wl,-T,$(PB)/twine.ld -o $@ $(WIN_OBJ) -L$(SDL3_DIR)/lib -lSDL3 -lwinmm -lm

# A folder to copy to a Windows PC as it is (build/dist-win/twine): the game and SDL3.dll. The player's own disc
# image, BIOS and film clips go next to twine.exe (disc/, bios/, hdmovies/), as in the Linux build's folder.
WIN_DIST := $(ROOT)/build/dist-win/twine
windist: $(PB)/twine.exe
	rm -rf $(WIN_DIST) && mkdir -p $(WIN_DIST)
	i686-w64-mingw32-strip -o $(WIN_DIST)/twine.exe $(PB)/twine.exe
	cp $(SDL3_DIR)/bin/SDL3.dll $(WIN_DIST)/
	cp $(SDL3_DIR)/share/licenses/SDL3/LICENSE.txt $(WIN_DIST)/LICENSE-SDL3.txt
	@echo "$(WIN_DIST): $$(du -sh $(WIN_DIST) | cut -f1)"

# The system report of the USB test kit (port/usbkit)
$(PB)/winsysinfo.exe: tools/winsysinfo.c $(SDL3_DIR)/lib/libSDL3.dll.a
	$(CC) -O2 -Wall -I$(SDL3_DIR)/include tools/winsysinfo.c -o $@ -L$(SDL3_DIR)/lib -lSDL3 -lopengl32 -lgdi32

# The USB test kit (port/usbkit/README.txt) in DEST: the game's folder as windist makes it, "portable" (settings,
# saves, cards, logs all kept there), TEST.bat and kit/. Never a disc image: the player copies their own into disc/.
usbkit-win: windist $(PB)/winsysinfo.exe
	@test -n "$(DEST)" || { echo "make -C port usbkit DEST=<folder on the stick>"; exit 1; }
	mkdir -p "$(DEST)/kit" "$(DEST)/disc" "$(DEST)/bios" "$(DEST)/hdmovies" "$(DEST)/results"
	cp $(WIN_DIST)/* "$(DEST)/"
	cp usbkit/TEST.bat usbkit/README.txt "$(DEST)/"
	cp usbkit/test.ps1 usbkit/runs.txt $(PB)/winsysinfo.exe $(SDL3_DIR)/bin/SDL3.dll "$(DEST)/kit/"
	printf 'This file keeps the game portable: its settings, saves, cards and logs stay in the twine folder here.\r\n' > "$(DEST)/portable"
	@echo "$(DEST): the test kit; put your own disc image into $(DEST)/disc"
endif

# The Windows build and its folder (from Linux: MinGW-w64's i686-w64-mingw32 tools)
windows:
	$(MAKE) PORT_OS=windows windist

# The USB test kit for a Windows PC (port/usbkit): make -C port usbkit DEST=/run/media/.../STICK/twine
usbkit:
	$(MAKE) PORT_OS=windows usbkit-win DEST="$(abspath $(DEST))"

# Host tool: the MDEC model on its own, decoding movie frames from the disc image (port/tools/mdectest.c)
mdectest: $(PB)/mdectest
$(PB)/mdectest: tools/mdectest.c src/mdec.c src/mdec.h
	@mkdir -p $(dir $@)
	$(CC) -m32 -std=gnu11 -O2 -g -Wall -DNATIVE_PORT -I$(ROOT)/include -Isrc tools/mdectest.c src/mdec.c -o $@

# Host tool: the hardware renderer against the software GPU on random GPU commands (port/tools/hwtest.c)
hwtest: $(PB)/hwtest
$(PB)/hwtest: tools/hwtest.c src/gpu.c src/gpu_hw.c src/gpu.h src/gpu_hw.h $(SHADER_OUT)/shaders.h
	@mkdir -p $(dir $@)
	$(CC) -m32 -std=gnu11 -O2 -g -Wall -DNATIVE_PORT -I$(ROOT)/include -Isrc -I$(SHADER_OUT) tools/hwtest.c src/gpu.c \
	    src/gpu_hw.c -o $@ -lSDL3 -lm

# A folder to copy to another PC as it is (build/dist/twine; build/ is never committed): the game, the SDL3 it
# was built with (lib/, found before the system's), the film clips (hdmovies/, from HD_MOVIES, by default the
# user's own; see tools/hdmovies.py), and the user's own disc image and BIOS (disc/, bios/, from WORKBENCH),
# which the game uses when twine.ini doesn't name others.
HD_MOVIES ?= $(HOME)/.local/share/twine/movies
WORKBENCH ?= $(ROOT)/workbench
DIST := $(ROOT)/build/dist/twine
dist: $(PB)/twine
	rm -rf $(DIST) && mkdir -p $(DIST)/lib $(DIST)/hdmovies $(DIST)/disc $(DIST)/bios
	cp -l $(WORKBENCH)/disc/* $(DIST)/disc/ 2>/dev/null || cp $(WORKBENCH)/disc/* $(DIST)/disc/
	cp $(WORKBENCH)/bios/* $(DIST)/bios/
	cp $(PB)/twine $(DIST)/
	cp -L $$($(CC) -m32 -print-file-name=libSDL3.so.0) $(DIST)/lib/
	-cp -l $(HD_MOVIES)/*.mpg $(DIST)/hdmovies/ 2>/dev/null || cp $(HD_MOVIES)/*.mpg $(DIST)/hdmovies/
	@echo "$(DIST): $$(ls $(DIST)/hdmovies | wc -l) HD clips, $$(du -sh $(DIST) | cut -f1)"

# The Linux download (build/dist/twine-linux and twine-linux-x86.tar.gz): this build, its SDL3 and a 32-bit runtime for
# systems without one, built in containers, with a launcher; no game data. See tools/dist/dist_linux.sh.
dist-linux: $(PB)/twine
	tools/dist/dist_linux.sh $(abspath $(PB)/twine) $(abspath $(ROOT)/build/dist)

clean:
	rm -rf $(PB)

.PHONY: all dist dist-linux clean mdectest hwtest windows windist usbkit usbkit-win FORCE
.PRECIOUS: $(PB)/%.c
