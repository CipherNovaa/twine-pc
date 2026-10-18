typedef struct { char pad[0x38]; int flags; char pad2[0xB8 - 0x3C]; } Thing;
extern Thing D_800B00A4[];
void func_8002E294(void) {
    int i;
    Thing *p = D_800B00A4;
    for (i = 0; i < 6; i++, p++) {
        if (i != 5) p->flags |= 0x1800;
    }
}
