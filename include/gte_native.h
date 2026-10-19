#ifndef GTE_NATIVE_H
#define GTE_NATIVE_H

/* Native port: the gte_* macros of gte.h (and the ones single source files define) as calls into the
 * software GTE in port/src/gte.c. Each macro moves the same registers as its MIPS version (mtc2/mfc2 =
 * data registers, ctc2/cfc2 = control registers, lwc2/swc2 = data registers from/to memory); commands go
 * through GTE_cmd with the original cop2 instruction word. Writes and reads apply the hardware's
 * truncation and sign/zero extension, so code that relies on them behaves the same. */

u32 GTE_mfc2(int reg);
void GTE_mtc2(int reg, u32 value);
u32 GTE_cfc2(int reg);
void GTE_ctc2(int reg, u32 value);
void GTE_cmd(u32 op);

#define GTE_W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define GTE_H(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define GTE_HU(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define GTE_LWC2(reg, p, o) GTE_mtc2(reg, GTE_W(p, o))
#define GTE_SWC2(reg, p, o) (GTE_W(p, o) = GTE_mfc2(reg))

/* commands */
#define gte_rtps() GTE_cmd(0x4A180001)
#define gte_rtpt() GTE_cmd(0x4A280030)
#define gte_rtv0() GTE_cmd(0x4A486012)
#define gte_rtir() GTE_cmd(0x4A49E012)
#define gte_rtv0tr() GTE_cmd(0x4A080012)
#define gte_mvmva_rt_ir() GTE_cmd(0x4A41E012)
#define gte_mvmva_v0() GTE_cmd(0x4A480012)
#define gte_nclip() GTE_cmd(0x4B400006)
#define gte_avsz3() GTE_cmd(0x4B58002D)
#define gte_gpf12() GTE_cmd(0x4B98003D)
#define gte_sqr0() GTE_cmd(0x4AA00428)
#define gte_op0() GTE_cmd(0x4B70000C)
#define gte_op1() GTE_cmd(0x4B78000C)

/* control registers */
#define gte_SetGeomScreen(h) GTE_ctc2(26, (h))
#define gte_SetGeomOffset(r0, r1) do { GTE_ctc2(24, (u32)(r0) << 16); GTE_ctc2(25, (u32)(r1) << 16); } while (0)
#define gte_ReadGeomScreen(r0) (*(s32 *)(r0) = GTE_cfc2(26))
#define gte_ReadGeomOffset(r0, r1) do { *(s32 *)(r0) = (s32)GTE_cfc2(24) >> 16; *(s32 *)(r1) = (s32)GTE_cfc2(25) >> 16; } while (0)
#define gte_SetRotMatrix(r0) do { GTE_ctc2(0, GTE_W(r0, 0)); GTE_ctc2(1, GTE_W(r0, 4)); GTE_ctc2(2, GTE_W(r0, 8)); \
    GTE_ctc2(3, GTE_W(r0, 12)); GTE_ctc2(4, GTE_W(r0, 16)); } while (0)
#define gte_SetTransMatrix(r0) do { GTE_ctc2(5, GTE_W(r0, 20)); GTE_ctc2(6, GTE_W(r0, 24)); GTE_ctc2(7, GTE_W(r0, 28)); } while (0)
#define gte_SetRotMatrix_la(r0) gte_SetRotMatrix(r0)
#define gte_SetTransMatrix_la(r0) gte_SetTransMatrix(r0)
#define gte_ldR11R12R13R21(r0) do { GTE_ctc2(0, GTE_W(r0, 0)); GTE_ctc2(1, GTE_W(r0, 4)); } while (0)
#define gte_ldR31R32R33(r0) do { GTE_ctc2(3, GTE_W(r0, 0)); GTE_ctc2(4, GTE_W(r0, 4)); } while (0)
#define gte_ldDiag(r0) do { GTE_ctc2(0, GTE_W(r0, 0)); GTE_ctc2(2, GTE_W(r0, 4)); GTE_ctc2(4, GTE_W(r0, 8)); } while (0)
#define gte_ldopv1(r0) do { GTE_ctc2(0, (s32)GTE_H(r0, 0)); GTE_ctc2(2, (s32)GTE_H(r0, 2)); GTE_ctc2(4, (s32)GTE_H(r0, 4)); } while (0)
#define gte_cfFLAG(r) ((r) = GTE_cfc2(31))
#define gte_stflg(r0) (*(s32 *)(r0) = GTE_cfc2(31))

/* data registers: loads */
#define gte_ldv0(r0) do { GTE_LWC2(0, r0, 0); GTE_LWC2(1, r0, 4); } while (0)
#define gte_ldv3(r0, r1, r2) do { GTE_LWC2(0, r0, 0); GTE_LWC2(1, r0, 4); GTE_LWC2(2, r1, 0); GTE_LWC2(3, r1, 4); \
    GTE_LWC2(4, r2, 0); GTE_LWC2(5, r2, 4); } while (0)
#define gte_ldv3c_la(r0) do { GTE_LWC2(0, r0, 0); GTE_LWC2(1, r0, 4); GTE_LWC2(2, r0, 8); GTE_LWC2(3, r0, 12); \
    GTE_LWC2(4, r0, 16); GTE_LWC2(5, r0, 20); } while (0)
#define gte_ldv0_mt(r0, r1) do { GTE_mtc2(0, (r0)); GTE_mtc2(1, (r1)); } while (0)
#define gte_ldVXYZ0(r0, r1) do { GTE_mtc2(0, (r0)); GTE_mtc2(1, (r1)); } while (0)
#define gte_ldlv0(r0) do { GTE_mtc2(0, GTE_HU(r0, 0) | ((u32)GTE_HU(r0, 4) << 16)); GTE_LWC2(1, r0, 8); } while (0)
#define gte_lddp(r0) GTE_mtc2(8, (r0))
#define gte_ldsv(r0) do { GTE_mtc2(9, GTE_HU(r0, 0)); GTE_mtc2(10, GTE_HU(r0, 2)); GTE_mtc2(11, GTE_HU(r0, 4)); } while (0)
#define gte_ldclmv(r0) do { GTE_mtc2(9, GTE_HU(r0, 0)); GTE_mtc2(10, GTE_HU(r0, 6)); GTE_mtc2(11, GTE_HU(r0, 12)); } while (0)
#define gte_ldopv2(r0) do { GTE_mtc2(9, (s32)GTE_H(r0, 0)); GTE_mtc2(10, (s32)GTE_H(r0, 2)); GTE_mtc2(11, (s32)GTE_H(r0, 4)); } while (0)
#define gte_ldlvl(r0) do { GTE_LWC2(9, r0, 0); GTE_LWC2(10, r0, 4); GTE_LWC2(11, r0, 8); } while (0)
#define gte_ldlvl_zxy(r0) do { GTE_LWC2(11, r0, 8); GTE_LWC2(9, r0, 0); GTE_LWC2(10, r0, 4); } while (0)
#define gte_ldsxy3(r0, r1, r2) do { GTE_LWC2(12, r0, 0); GTE_LWC2(13, r1, 0); GTE_LWC2(14, r2, 0); } while (0)
#define gte_ldsxy3c(r0) do { GTE_LWC2(12, r0, 0); GTE_LWC2(13, r0, 4); GTE_LWC2(14, r0, 8); } while (0)

/* data registers: reads and stores */
#define gte_mfMAC0(r) ((r) = GTE_mfc2(24))
#define gte_mfMAC1(r) ((r) = GTE_mfc2(25))
#define gte_mfMAC3(r) ((r) = GTE_mfc2(27))
#define gte_mfsxy2(r) ((r) = GTE_mfc2(14))
#define gte_mfsz3(r) ((r) = GTE_mfc2(19))
#define gte_stSXY2_SZOTZ(r0, r1) do { (r0) = GTE_mfc2(14); (r1) = (s32)GTE_mfc2(19) >> 2; } while (0)
#define gte_stsxy(r0) GTE_SWC2(14, r0, 0)
#define gte_stsxy3(r0, r1, r2) do { GTE_SWC2(12, r0, 0); GTE_SWC2(13, r1, 0); GTE_SWC2(14, r2, 0); } while (0)
#define gte_stsz(r0) GTE_SWC2(19, r0, 0)
#define gte_stsz3(r0, r1, r2) do { GTE_SWC2(17, r0, 0); GTE_SWC2(18, r1, 0); GTE_SWC2(19, r2, 0); } while (0)
#define gte_stotz(r0) GTE_SWC2(7, r0, 0)
#define gte_stszotz(r0) (*(s32 *)(r0) = (s32)GTE_mfc2(19) >> 2)
#define gte_stlvnl(r0) do { GTE_SWC2(25, r0, 0); GTE_SWC2(26, r0, 4); GTE_SWC2(27, r0, 8); } while (0)
#define gte_stsv(r0) do { GTE_H(r0, 0) = GTE_mfc2(9); GTE_H(r0, 2) = GTE_mfc2(10); GTE_H(r0, 4) = GTE_mfc2(11); } while (0)
#define gte_stclmv(r0) do { GTE_H(r0, 0) = GTE_mfc2(9); GTE_H(r0, 6) = GTE_mfc2(10); GTE_H(r0, 12) = GTE_mfc2(11); } while (0)

#endif
