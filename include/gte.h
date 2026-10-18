#ifndef GTE_H
#define GTE_H

/* GTE (geometry coprocessor) access as inline asm, mirroring PsyQ's inline_c.h so the compiler
   schedules around it exactly like the original. Add macros as functions need them; verify each. */

#define gte_SetGeomScreen(h) __asm__ volatile("ctc2 %0, $26" : : "r"(h))
#define gte_SetGeomOffset(r0, r1) __asm__ volatile("sll $12, %0, 16; sll $13, %1, 16; ctc2 $12, $24; ctc2 $13, $25" : : "r"(r0), "r"(r1) : "$12", "$13")
#define gte_ReadGeomScreen(r0) __asm__ volatile("cfc2 $12, $26; nop; sw $12, 0(%0)" : : "r"(r0) : "$12", "memory")

/* PsyQ libgte vector type. */
typedef struct SVECTOR {
    s16 vx, vy, vz, pad;
} SVECTOR;

#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" : : "r"(r0))
#define gte_ldsv(r0) __asm__ volatile("lhu $12, 0(%0); lhu $13, 2(%0); lhu $14, 4(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(r0) : "$12", "$13", "$14")
#define gte_gpf12() __asm__ volatile("nop; nop; .word 0x4B98003D")
#define gte_stsv(r0) __asm__ volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 2(%0); sh $14, 4(%0)" : : "r"(r0) : "$12", "$13", "$14", "memory")

/* Game-specific: first rotation-matrix row from three shorts, MVMVA rt*ir (no shift), MAC1 read. */
#define gte_ldR11R12R13R21(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1" : : "r"(r0) : "$12", "$13")
#define gte_ldR31R32R33(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $3; ctc2 $13, $4" : : "r"(r0) : "$12", "$13")
#define gte_mvmva_rt_ir() __asm__ volatile("nop; nop; .word 0x4A41E012")
#define gte_mfMAC1(r) __asm__ volatile("mfc2 %0, $25; nop" : "=r"(r))
#define gte_mfMAC3(r) __asm__ volatile("mfc2 %0, $27; nop" : "=r"(r))

#endif
