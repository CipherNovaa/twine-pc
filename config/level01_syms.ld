/* Symbols the C needs that splat does not generate: absolute addresses outside the EXE.
 * 0x1F800000 is the PS1 scratchpad (1 KB of fast on-chip RAM). The game accesses some of its words
 * through separate symbols (a fresh lui per access), so each needs its own name to match. */
D_1F8003D4 = 0x1F8003D4;
D_1F8003D8 = 0x1F8003D8;
D_1F8003DC = 0x1F8003DC;

/* Several distinct (probably zero-size) pools at the very start of BSS: the effect pool init
 * (func_80057C40) addresses each through its own symbol, so they need separate names. Aliases of
 * D_800AB540, so they follow it in both the plain and the shiftable link. */
D_800AB540_b = D_800AB540;
D_800AB540_c = D_800AB540;

/* Second word of D_8009CE20, which func_80030B04 addresses as its own symbol. */
D_8009CE24 = D_8009CE20 + 4;
