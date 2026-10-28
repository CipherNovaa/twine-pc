/* The Windows build: linked before the port's own objects (port/Makefile), after the C runtime's start-up: where their
 * data and BSS begin, what a save state holds (platform_win.c plat_state_ranges; Linux: savestate.c's
 * [__data_start, _end)). */
char plat_data_first[4] = {1};
char plat_bss_first[4];
