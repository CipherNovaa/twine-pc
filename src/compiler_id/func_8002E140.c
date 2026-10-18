extern unsigned char *g_Args;
extern int D_800B0B18[];
extern int D_8009CCBC[];
void func_8002E140(void) {
    int *dst;
    int *src;
    int i;
    int add;
    dst = D_800B0B18;
    add = (((signed char *)g_Args)[0x1BC] == 0) << 4;
    src = D_8009CCBC;
    i = 0;
    do {
        i++;
        *dst++ = *src++ + add;
    } while (i < 15);
}
