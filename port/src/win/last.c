/* The Windows build: linked after the port's own objects, before the libraries (port/Makefile): where their data and
 * BSS end (platform_win.c plat_state_ranges) */
char plat_data_last[4] = {1};
char plat_bss_last[4];
