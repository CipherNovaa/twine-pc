extern int D_8009C188[];
short func_80026C38(int mask) {
    short i;
    for (i = 0; i < 24; i++) {
        if (mask & D_8009C188[i]) {
            return i;
        }
    }
    return -1;
}
