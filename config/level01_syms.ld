/* Symbols the C needs that splat does not generate: absolute addresses outside the EXE.
 * 0x1F800000 is the PS1 scratchpad (1 KB of fast on-chip RAM). The game accesses some of its words
 * through separate symbols (a fresh lui per access), so each needs its own name to match. */
D_1F8003D4 = 0x1F8003D4;
D_1F8003D8 = 0x1F8003D8;
D_1F8003DC = 0x1F8003DC;
