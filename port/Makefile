# Native PC build of one level: `make -C port LEVEL=level01` (from the repo root: `make port`).
# The game's C (src/LEVEL) and the runtime (port/src) are compiled for 32-bit x86; the level's PS1 data
# image is loaded at its own addresses at run time (see port/src/main.c).

LEVEL ?= level01
ROOT := ..
OUT := $(ROOT)/build/port/$(LEVEL)
RT_OUT := $(ROOT)/build/port/rt

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

GAME_SRC := $(wildcard $(ROOT)/src/$(LEVEL)/*.c)
GAME_OBJ := $(patsubst $(ROOT)/src/$(LEVEL)/%.c,$(OUT)/game/%.o,$(GAME_SRC))
RT_SRC := $(wildcard $(ROOT)/port/src/*.c)
RT_OBJ := $(patsubst $(ROOT)/port/src/%.c,$(RT_OUT)/%.o,$(RT_SRC))
ELF := $(ROOT)/build/$(LEVEL)/$(LEVEL).elf

all: $(OUT)/twine

# Single-file GTE macros are MIPS asm; the native ones come from include/gte_native.h.
$(OUT)/game/%.c: $(ROOT)/src/$(LEVEL)/%.c
	@mkdir -p $(dir $@)
	sed -e '/^#define gte_[A-Za-z0-9_]*(.*__asm__/d' $< > $@

$(OUT)/game/%.o: $(OUT)/game/%.c $(wildcard $(ROOT)/include/*.h) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(RT_OUT)/%.o: $(ROOT)/port/src/%.c $(wildcard $(ROOT)/port/src/*.h) $(wildcard $(ROOT)/include/*.h)
	@mkdir -p $(dir $@)
	$(CC) $(RT_CFLAGS) -c $< -o $@

# MIPS code that has no C yet (PsyQ libraries, hand-written GTE code) is statically recompiled
LEVEL_DEPS := $(ELF) gen_native.py psyq_names.txt $(wildcard $(ROOT)/config/$(LEVEL)_syms.ld)

$(OUT)/recomp.c: $(GAME_OBJ) $(RT_OBJ) $(LEVEL_DEPS) recomp.py
	python3 recomp.py $(LEVEL) $@ $(GAME_OBJ) $(RT_OBJ)

$(OUT)/recomp.o: $(OUT)/recomp.c src/recomp.h
	$(CC) -m32 -std=gnu11 -fno-builtin -fpermissive -fno-pie -O1 -g -w -DNATIVE_PORT -I$(ROOT)/include -Isrc -c $< -o $@

$(OUT)/gen.stamp: $(GAME_OBJ) $(RT_OBJ) $(OUT)/recomp.o $(LEVEL_DEPS)
	python3 gen_native.py $(LEVEL) $(OUT) $(GAME_OBJ) -- $(RT_OBJ) $(OUT)/recomp.o
	@touch $@

$(OUT)/stubs.o $(OUT)/funcs.o: $(OUT)/%.o: $(OUT)/gen.stamp
	$(CC) -m32 -std=gnu11 -fno-pie -O1 -w -c $(OUT)/$*.c -o $@

$(OUT)/twine: $(GAME_OBJ) $(RT_OBJ) $(OUT)/recomp.o $(OUT)/stubs.o $(OUT)/funcs.o $(OUT)/gen.stamp
	$(CC) $(LDFLAGS) -o $@ $(GAME_OBJ) $(RT_OBJ) $(OUT)/recomp.o $(OUT)/stubs.o $(OUT)/funcs.o $(OUT)/syms.ld -lSDL3 -lm

clean:
	rm -rf $(OUT) $(RT_OUT)

.PHONY: all clean
.PRECIOUS: $(OUT)/game/%.c
