#ifndef GTE_H
#define GTE_H

/* GTE (geometry coprocessor) access as inline asm, mirroring PsyQ's inline_c.h so the compiler
   schedules around it exactly like the original. Add macros as functions need them; verify each. */

/* PsyQ libgte vector type. */
typedef struct SVECTOR {
    s16 vx, vy, vz, pad;
} SVECTOR;

typedef struct VECTOR {
    s32 vx, vy, vz, pad;
} VECTOR;

#ifdef NATIVE_PORT
/* The native port runs the same macros on a software GTE (port/src/gte.c). */
#include "gte_native.h"
#else

#define gte_SetGeomScreen(h) __asm__ volatile("ctc2 %0, $26" : : "r"(h))
#define gte_SetGeomOffset(r0, r1) __asm__ volatile("sll $12, %0, 16; sll $13, %1, 16; ctc2 $12, $24; ctc2 $13, $25" : : "r"(r0), "r"(r1) : "$12", "$13")
#define gte_ReadGeomScreen(r0) __asm__ volatile("cfc2 $12, $26; nop; sw $12, 0(%0)" : : "r"(r0) : "$12", "memory")

#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" : : "r"(r0))
#define gte_ldsv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 2(%0); lhu $14, 4(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_gpf12() __asm__ volatile("nop; nop; .word 0x4B98003D")
#define gte_stsv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 2(%0); sh $14, 4(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")

/* PsyQ inline_c.h: vector load, perspective/rotation transforms, result stores, matrices. */
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_rtv0() __asm__ volatile("nop; nop; .word 0x4A486012")
#define gte_stsxy(r0) __asm__ volatile("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
#define gte_stlvnl(r0) __asm__ volatile("swc2 $25, 0(%0); swc2 $26, 4(%0); swc2 $27, 8(%0)" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile("mfc2 $12, $19; nop; sra $12, $12, 2; sw $12, 0(%0)" : : "r"(r0) : "$12", "memory")
#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ReadGeomOffset(r0, r1) __asm__ volatile("cfc2 $12, $24; cfc2 $13, $25; sra $12, $12, 16; sra $13, $13, 16; sw $12, 0(%0); sw $13, 0(%1)" : : "r"(r0), "r"(r1) : "$12", "$13", "memory")

#define gte_avsz3() __asm__ volatile("nop; nop; .word 0x4B58002D")
#define gte_cfFLAG(r) __asm__ volatile("cfc2 %0, $31; nop" : "=r"(r))
#define gte_ldclmv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldlv0(r0) __asm__ volatile("lhu $13, 4(%0); lhu $12, 0(%0); sll $13, $13, 16; or $12, $12, $13; mtc2 $12, $0; lwc2 $1, 8(%0)" : : "r"(r0) : "$12", "$13")
#define gte_ldlvl(r0) __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0)" : : "r"(r0))
#define gte_ldsxy3c(r0) __asm__ volatile("lwc2 $12, 0(%0); lwc2 $13, 4(%0); lwc2 $14, 8(%0)" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0); lwc2 $2, 0(%1); lwc2 $3, 4(%1); lwc2 $4, 0(%2); lwc2 $5, 4(%2)" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_mfMAC0(r) __asm__ volatile("mfc2 %0, $24; nop" : "=r"(r))
#define gte_nclip() __asm__ volatile("nop; nop; .word 0x4B400006")
#define gte_rtir() __asm__ volatile("nop; nop; .word 0x4A49E012")
#define gte_rtpt() __asm__ volatile("nop; nop; .word 0x4A280030")
#define gte_sqr0() __asm__ volatile("nop; nop; .word 0x4AA00428")
#define gte_stclmv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")
#define gte_stflg(r0) __asm__ volatile("cfc2 $12, $31; nop; sw $12, 0(%0)" : : "r"(r0) : "$12", "memory")
#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" : : "r"(r0) : "memory")
#define gte_stsxy3(r0, r1, r2) __asm__ volatile("swc2 $12, 0(%0); swc2 $13, 0(%1); swc2 $14, 0(%2)" : : "r"(r0), "r"(r1), "r"(r2) : "memory")
#define gte_stsz(r0) __asm__ volatile("swc2 $19, 0(%0)" : : "r"(r0) : "memory")

/* Game-specific: first rotation-matrix row from three shorts, MVMVA rt*ir (no shift), MAC1 read. */
#define gte_ldR11R12R13R21(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1" : : "r"(r0) : "$12", "$13")
#define gte_ldR31R32R33(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $3; ctc2 $13, $4" : : "r"(r0) : "$12", "$13")
#define gte_mvmva_rt_ir() __asm__ volatile("nop; nop; .word 0x4A41E012")
#define gte_mfMAC1(r) __asm__ volatile("mfc2 %0, $25; nop" : "=r"(r))
#define gte_mfMAC3(r) __asm__ volatile("mfc2 %0, $27; nop" : "=r"(r))

#endif /* NATIVE_PORT */

#endif
