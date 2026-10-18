#include "twine.h"

extern s32 D_800D3FA8;
extern s32 D_800D3FAC;
extern s32 D_800D3FB0;
extern s32 D_800D3FD4;
extern s32 D_800D3FD8;
extern s32 D_800D3FE0;
extern s32 D_800D3FE4;
extern s32 D_800D3FEC;
extern s32 D_800D3FF0;
extern s32 D_800D4068;
extern s32 D_800D406C;
extern s32 D_800D4070;
extern s32 D_800D4074;
extern s32 D_800D40F8;
extern s32 D_800D40FC;
extern s32 D_800D4100;
extern s32 D_800D4104;
extern s32 D_800D4108;
extern s32 D_800D410C;
extern s32 D_800D4110;
extern s32 D_800D4114;


typedef struct { char text[0x23]; u8 unk23; u16 unk24; u8 flag; u8 pad27; s16 x; s16 y; } Msg;
void func_80028858(char *text, s16 *x, s16 *w);
void func_800299CC(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_800477C0(void *arg0, void *arg1);
extern s32 D_8009EED0;
extern u16 D_800AEDD4;


typedef struct { u32 tag; u8 r0, g0, b0, code; s16 x0, y0; u8 r1, g1, b1, pad1; s16 x1, y1; u8 r2, g2, b2, pad2; s16 x2, y2; u8 r3, g3, b3, pad3; s16 x3, y3; } POLY_G4;
typedef struct { u32 tag; u8 r0, g0, b0, code; s16 x0, y0; s16 x1, y1; s16 x2, y2; s16 x3, y3; } POLY_F4;
void func_80088C44(void *);
void func_80044B3C(void *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_80044BBC(void *arg0, s8 arg1, s8 arg2, s8 arg3);
extern POLY_G4 D_800D79D4[];
extern POLY_F4 D_800D7554[];


extern s32 D_8009E41C;
extern s32 D_800E23DC[];
extern s32 D_800E23EC[];


extern s16 D_800B0066;


void func_80050180(s32, s32);
void func_8004FD3C(s32, s32);
s32 func_8004F348(void);


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtv0() __asm__ volatile("nop; nop; .word 0x4A486012")
#define gte_stlvnl(r0) __asm__ volatile("swc2 $25, 0(%0); swc2 $26, 4(%0); swc2 $27, 8(%0)" : : "r"(r0) : "memory")
void func_8005C10C(u8 *, u8 *, s16 *, s16 *);
void RotMatrixY(s32, void *);


typedef struct { u8 pad0[0x1CC]; u32 flags; u8 pad1D0[0x78]; } AISlot;
extern AISlot D_800D9278[];


extern u8 *D_800DF1F4[];
extern u8 D_800DF5B8[];


extern s32 D_800DF540[];


extern s32 D_800D43F4;


extern u16 D_800D43AC;
extern s32 D_800D43F0;
void func_8007A040(s32);


extern char D_800A77E4[];


void func_80039520(u8 *obj);
void func_800753A4(void *arg0);


typedef struct { u32 length; u32 *org; u32 offset; u32 point; u32 *tag; } GsOT;
extern s32 D_800D2BE8;
extern s32 D_800B8EFC[][5];
extern GsOT D_800D2AFC[];
extern u8 D_800B8FB4[][0x8CA0];
extern s32 D_800D3234;
extern s32 D_800D3064;
extern s8 D_8009C1EE;
extern s32 D_8009C1F0;
extern s32 D_8009CDE8;
extern s32 D_8009CDC0;
extern u8 D_8009CD6E;
extern s16 D_800B07B4;
extern s16 D_800B07A4;
extern s16 D_800B0DB0;
extern s16 D_800B07C0;
void func_80087704(void *);
void GsClearOt(s32, s32, void *);
void GsSortOt(void *, void *);
void GsSortClear(s32, s32, s32, void *);
void GsSwapDispBuff(void);
s32 VSync(s32);
void func_8004B32C(void);
void func_8006F784(void);
void func_8006F6AC(void);
void func_8006CEA4(s32);
void func_80030354(void);
void func_8005AEF8(void);
void func_800781B4(void);
void func_80060410(void);
void func_8007545C(s32 first, s32 last);
void func_80077D50(s32, s32);
void func_80077FC4(s32, s32);
void func_800406A0(void);
void func_8004B58C(s32);
void func_800463C8(void);
void func_8004514C(void);
void func_80028DBC(s32, s32);
void func_80028C08(s32, s32);
void DevMenu_Draw(void);
void DebugScreen_DrawPage(s32);
void func_80030234(void);


extern char D_800A89A4[];
void func_800705A8(s32, SVECTOR *, s32, u8 *);


u32 func_8006D3C0(SVECTOR *a, SVECTOR *b);


extern char D_800A74E0[];
void *malloc3(s32);


extern s32 D_800E2E48;


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_stsxy(r0) __asm__ volatile("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile("mfc2 $12, $19; nop; sra $12, $12, 2; sw $12, 0(%0)" : : "r"(r0) : "$12", "memory")
#define gte_ReadGeomOffset(r0, r1) __asm__ volatile("cfc2 $12, $24; cfc2 $13, $25; sra $12, $12, 16; sra $13, $13, 16; sw $12, 0(%0); sw $13, 0(%1)" : : "r"(r0), "r"(r1) : "$12", "$13", "memory")
extern u8 D_800DD83C[];
extern s8 D_8009D8D4;
extern SVECTOR D_800DD808;
extern SVECTOR D_800DD810;
extern s32 D_800DD7F8;
extern s16 D_800DD7FC;
extern s32 D_800DD800;
extern s16 D_800DD804;


extern s8 D_800A7F1C;
extern s8 D_800A7F2C;
extern struct { s16 v[4]; } D_800B077C;
extern u32 D_800D2BF0[];
void *func_80030610(s32);
void func_8004C0C0(s32, void *);
void GsLinkObject4(s32, void *, s32);


typedef struct { u8 pad[4]; u8 flags; u8 pad5[15]; } FxTmpl;
extern s32 D_800AB540;
extern u8 D_800DB884[];
extern u8 D_800DA5C4[];
extern u8 D_800DA5C0;
extern char D_800A8CB0[];
extern char D_800A8CC4[];
extern char D_800A8CEC[];
extern char D_800A8CF8[];
void func_8005885C(u8 *, u8);


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtv0() __asm__ volatile("nop; nop; .word 0x4A486012")


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_stsxy(r0) __asm__ volatile("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
typedef struct VECTOR { s32 vx, vy, vz, pad; } VECTOR;
extern s32 D_800DDDA8;


extern char D_800A88A4[];
extern char D_800A88D0[];
extern s16 D_8009D914;
extern s16 D_8009D444;
extern s32 D_800E23E8;
extern s32 D_800E02B4;
extern s32 D_800E23D4;
s32 func_80075FA0(void);
void func_8004E9AC(void);
void func_8004EC70(void);
void func_8004A29C(void);
void func_8004A81C(void);
void func_8004A85C(s32 arg0);


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtps() __asm__ volatile("nop; nop; .word 0x4A180001")
#define gte_stsxy(r0) __asm__ volatile("swc2 $14, 0(%0)" : : "r"(r0) : "memory")
extern s32 D_800DDDDC[];


extern List D_800D86E8[][8];
extern char D_800A880C[];
extern char D_800A8820[];
extern char D_800A8858[];


extern u8 D_800DE0C8[];
extern u8 *D_800DDEF0[];
s32 func_8005B584(s32, u8, s16);
void func_8007D260(s32);


extern u8 D_800DE018[];
void func_8005BA30(void *);
void func_8005B7BC(void *obj, s32 arg1);


extern s32 D_8009D020;
extern s32 D_8009D024;
extern s32 D_8009D080;


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtv0() __asm__ volatile("nop; nop; .word 0x4A486012")
extern char D_800A80A4[];
extern s32 D_800D40D8[];
void func_8003EA24(SVECTOR *arg0, s32 *arg1, SVECTOR *arg2);


typedef struct { u32 tag; u8 r0, g0, b0, code; s16 x0, y0; u8 u0, v0; u16 clut; s16 x1, y1; u8 u1, v1; u16 tpage; s16 x2, y2; u8 u2, v2; u16 pad1; s16 x3, y3; u8 u3, v3; u16 pad2; } POLY_FT4;
extern POLY_FT4 D_800D4984[2][32][2];
extern u8 D_8009D040[][8];
s16 GetClut(s32, s32);
void func_80044B74(void *arg0, s8 arg1, s8 arg2, s8 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtv0() __asm__ volatile("nop; nop; .word 0x4A486012")


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
extern s32 D_800E2438;
extern s32 D_800B0018[];
extern s32 D_800E2DBC;
void func_8006E054(void);
void func_8006E9E0(s32);


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv0(r0) __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(r0))
#define gte_rtv0() __asm__ volatile("nop; nop; .word 0x4A486012")
#define gte_stlvnl(r0) __asm__ volatile("swc2 $25, 0(%0); swc2 $26, 4(%0); swc2 $27, 8(%0)" : : "r"(r0) : "memory")
extern u8 D_1F8003E0[];


#define gte_SetRotMatrix(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_SetTransMatrix(r0) __asm__ volatile("lw $12, 20(%0); lw $13, 24(%0); ctc2 $12, $5; lw $14, 28(%0); ctc2 $13, $6; ctc2 $14, $7" : : "r"(r0) : "$12", "$13", "$14")
extern s32 D_800DF10C[];
void func_80062394(void);
void func_80062784(void);
void func_80062B68(void);


extern char D_800A7DC0[];
extern char D_800A7DE8[];


extern s16 *D_8009D5AC;
extern s32 **D_8009D5A8;
extern s16 D_800D9232;
s16 func_80053684(s32 x, s32 z, u8 *obj, s32 dist);
s16 func_80053004(u8 *obj, s32 x, s32 z);


typedef struct { s32 unk0; s32 x; s32 y; s32 z; u32 flags; u16 start; u16 end; s16 frames; s16 pad1A; u8 *anim; s32 time; } AnimSlot;
extern AnimSlot D_800D30D8[];
extern char D_800A7D64[];


typedef struct { s16 x, y, w, h; } RECT16;
extern char D_800A79F0[];
void GsGetTimInfo(u32 *, GsIMAGE *);
void LoadClut(u32 *, s32, s32);
void LoadClut2(u32 *, s32, s32);


extern u8 D_800E2DA8[];


extern s32 D_800D86E0;
typedef struct { u8 pad[6]; s16 x[6]; s16 y[6]; s16 z[6]; u8 pad2[0x54]; } HierRow;
typedef struct { u8 pad[0x28]; s32 x; s32 y; s32 z; u8 pad2[0x58]; } Part;


extern s32 D_800B0C38[];
extern s16 D_800B0094;
void func_8007A4E4(void *, void *);
void func_80050960(void);
void func_8004E89C(void);
void func_8004E78C(void);


extern s32 D_8009CA78;
extern s32 D_8009CA7C;
extern s32 D_8009CA80;
extern s32 D_8009CA84;
extern s32 D_8009CAA8;
extern s32 D_8009CACC;
extern s32 D_8009CAD0;
extern u8 D_8009CFEC;
extern s32 D_800B00A0;
extern s32 D_800B0AA0;
extern s32 D_800B0B84;
extern s32 D_800B0B88;
extern s32 D_800B0B8C;
void func_8002D604(void);
void func_8004900C(void);
void func_8004D88C(void);
s32 func_8004B084(void);
void func_8004B04C(void);
s32 func_8004B06C(void);
void SsUtAllKeyOff(s32);


extern s32 (*D_8009E480[])(void);
extern s32 D_8009E490;
extern char D_800A8A2C[];


extern u8 D_8009CDA0;
extern s16 D_800B0C9C;
extern s16 D_8009CD5C;
extern u8 D_8009CD62;
extern s32 D_800B0B10;
extern s16 D_800D9200[];
extern s32 D_800B0C64;
extern s32 D_800B0C68;
extern s32 D_800B0C60;
extern u16 D_800B0AEC;
extern s16 D_800B0D94;
extern s16 D_8009CFCC;
extern s32 D_800B0B14;
void func_8004E304(void);
void func_8004E328(void);
void func_800448F4(void);


M2C_UNK func_80065614(void *, volatile int, M2C_UNK);
M2C_UNK func_80065614(void *, M2C_UNK, M2C_UNK);


extern s32 D_800A944C;
void *func_80058694(u8 type);
void *func_80063CAC(void *, s16, s16, s16, s32);
void func_80063ED4(void *arg0);


extern s32 D_8009D368;
extern ListSet D_800D8C7C[];
extern u8 D_800D8E8C[][12];
extern u8 D_800D8E74[][12];
extern s32 D_800D8EA8[][8];
void func_8004D844(ListSet *set, s32 i, u32 j);
void func_8004D770(ListSet *set, s32 i, s32 j);
s32 func_8004D72C(ListSet *set, s32 which);
void func_8004CE38(s32 arg0, M2C_UNK arg1);


void func_800644DC(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);


extern s32 D_800A9498;
void func_800658B4(void *arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4);
void func_800669A4(SVECTOR *arg0, s32 *arg1, void *arg2);
void func_8006439C(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);


void func_80052CA8(s32 *out, u8 *obj);
void func_80065058(s16, s16, s16, void *, s32);


typedef struct { u8 active; u8 type; s16 unk2; s32 unk4; s32 animIdx; s32 unkC; s32 startTime[8]; u8 *obj; u16 flags; u8 duration; u8 pad37[9]; u8 enable[8]; } AnimEnt;
typedef struct { AnimEnt e[1]; } AnimEntArr;
extern M2C_UNK D_800A887C;


extern s32 D_800A0370[];
void func_8005ADD4(void *, s32, s32 *);


M2C_UNK func_80031B58(s32);                         /* extern */


extern s32 D_8009CAB4;
extern s32 D_8009CAB8;
void func_8003801C(void);
void func_80069AB4(void);
void func_8006A3E0(void);


extern M2C_UNK D_800A87EC;
void gteMIMefunc(SVECTOR *otp, SVECTOR *dfp, s32 n, s32 p);


extern s16 D_8009CF68;
extern s32 *D_8009CF6C;
extern s32 **D_8009CF70;
extern s32 D_800E2DE0;
void func_8003C1BC(s32 *, s32, s32, s32);


extern s32 D_800B8EFC[][5];
extern u8 D_800D2FF4[];
extern u8 D_800D2FC4[];
extern s32 g_DevMenu[];
extern char **D_8009CEB0[];
extern char *D_8009CEE0[];
extern char D_800A7A24[];
extern char D_800A7A30[];


typedef u8 Rec48[0x48];
extern s32 D_800D8BE8[];


extern s32 D_800E2F54;
extern s32 D_800E2E44;
extern void (*D_8009CE44)(s32);
typedef struct { s32 x, y, z, w; } Vec4i;
extern Vec4i D_800D2FB0;
void func_8004B05C(void);
void func_80053DAC(void);
void func_8006F168(s32, s32);
void func_8006FA54(s32);
void func_8006F9A4(s32 arg0);
void func_80030FA8(s16 *);
void func_8003C6E4();
void func_8003BFB8(void);
void func_80058D00(void);
void func_8005FF60(void);


extern u16 D_800AFFAC;
extern u8 *D_800B0AB4;
extern s16 D_800B0C84;


extern s32 D_800DDDCC[];
s32 SquareRoot0(s32 a);


extern s32 D_800D2D30[];
void func_8004BC70(s32, s32 *, s32);


extern s32 D_800E2F2C;
s32 func_800746D0(s32, s32 *, s32 *, s16 *, s16 *, s32 *);


extern u8 D_800D5ED8[][0xC];
void func_80081C54(void *);
void func_8004B028(s8 arg0, s8 arg1, s8 arg2);


extern s32 D_1F8003E8[];
extern s32 D_800A7D44;
s32 func_80035BBC(u8 *obj);


extern s32 D_800A7CB8;
void func_80076B9C(void *arg0);
void func_8006417C(s32, s32, s32, s32, s32);


extern s32 D_8009CE28;
extern s32 D_8009E9D8;
extern char D_800A77B4[];
extern char D_800A77CC[];
extern s16 D_800D2F94[];
void func_800310B4(s32 base);


extern s16 D_800B07B8;
extern s16 D_800B0DA8;


extern M2C_UNK D_800D9210;
extern s16 D_800D9214;


extern s32 D_8009CD3C[];
extern s32 D_800A8FAC;
void Camera_ResetZoom(void);
void func_8004F134(s16 arg0, s8 arg1);
void func_80069318(void);
void func_8006B1DC(s32 arg0);
void func_800509E4(void *);
void func_8005E020();


void func_8006ECBC(void);


extern char D_800A85D4[];
int strlen(char *s);


extern s32 D_1F8003D4;
extern s32 D_1F8003D8;
extern s32 D_1F8003DC;
extern M2C_UNK D_800D30C0;
extern s32 D_8009CF30;
M2C_UNK func_80052B2C(M2C_UNK *, s32);
s32 func_80037B6C(s32);
void func_8004B270(s32 arg0, s32 arg1, s32 arg2);
void func_8005E228(void *arg0);
M2C_UNK func_8004009C(s32);


extern s32 D_800DDE30;
extern u8 D_800D7C24[][0x10];
void func_800498E4(s16 *out);


extern s16 D_8009CFB8;
extern s32 D_800D439C;
extern s32 D_800D43A0;
extern s32 D_800D43C4;
extern s32 D_800D4404;
extern s32 D_800D440C;
extern s32 D_800D4414;
extern s32 D_800D4460;
extern char D_800A8558[];
extern char D_800A8564[];
extern char D_800A8574[];
extern char D_800A857C[];
void func_80043610(void);


extern s32 D_800A8894;
extern u8 D_800B0064[];
extern s32 D_800B0808[];
extern s32 D_800D3238[];
extern s32 D_800B0C10[];
void func_8004DA60();
void func_8004DBCC();
void func_8004E07C();
void func_8003BC98(void *, void *, s32);
void GsSetRefView2(void *);
void GsGetLs(void *, void *);


extern char D_800A8484[];
extern char D_800A84D0[];
extern char D_800A84D8[];
extern char D_800A84E4[];
extern char D_800A84F0[];
extern char D_800A84FC[];
extern char D_800A850C[];
extern char D_800A851C[];
extern char D_800A8528[];
extern char D_800A8534[];
extern char D_800A8544[];
void func_800411BC(s32 *x, s32 *y, s16 step);
void func_800433A8(void);


extern s16 D_8009EEC4;


extern int D_800AC54C;
extern s8 D_800D446C[];
extern char D_800A84BC[];
extern char D_800A84C0[];
extern char D_800A84C4[];


extern s32 D_800D2E70;
extern u8 D_8009CDEC;
u32 func_80026800(void);
s32 func_800855BC(u32);


extern s16 D_800B0798;
extern s32 D_800D4474[];
void func_8006981C(s32 arg0);


extern u32 D_800B8F68[];
extern s32 D_800D2AF8;
void func_8007F34C(u32 *, s32);
void func_8007F4C0(u32 *);
void AddPrim(void *, void *);


extern s32 D_800D4410;
extern char D_800A842C[];
void func_80041E78(void);


void func_800419CC(void *arg0);
void func_80041CF0(void);


extern char *D_800D4444[];
extern char D_800A8420[];
void func_80041BA0(void);


extern s32 D_800B8EFC[][5];
extern u8 D_800D308C[];


s32 Cheat_CheckCodes(void);
void func_800414AC(void);
void RestartPrompt_Open(void);
void func_80042A94(void);
void func_80042E34(void);
void func_800433F4(void);


void func_80058DFC(void *);


extern struct { char *s[8]; } D_800A8370;
extern struct { s32 v[8]; } D_800A8390;


extern s16 D_8009D108;
extern s16 D_8009D10A;
extern s16 D_8009D124;
extern s32 D_8009D130[];
extern s16 D_800D86CC[];


extern u8 D_800D5EF0[][0x28];
extern u8 D_800D7980[][0x28];
void func_80044BAC(void *arg0, s8 arg1, s8 arg2, s8 arg3);
void func_80081C84(void *);
void func_80081CE4(void *, s32);


extern s32 D_800DE0C4;
extern s32 D_800DE0D4;


void func_80057BFC(void *arg0, s16 arg1);


extern s32 D_8009CD30;
extern s16 D_800AFF5C;
extern s16 D_800AFF60;
extern u8 D_800B0ACC;
extern u8 D_800B0AD0;
extern u8 D_800B0AD4;
extern u8 D_800B0ADC;
extern s16 D_800B0AE8;
extern s16 D_800B0AFC;
extern s16 D_800B0B54;
extern s16 D_800B0C58;
extern s16 D_800B0C94;
extern s16 D_800B0C98;
extern u8 D_800B0CA0;
extern u8 D_800B0CA4;
extern u8 D_800B0CA8;
extern s16 D_800B0CAC;
extern s16 D_800B0DD8;
extern s32 D_800B8F28[];
extern u8 D_800B8FB0[];
extern u8 D_800D2AF4[];


extern s32 D_800A962C;
s32 func_8004CB2C(void *, Weapon *);
void func_8004C558(Weapon *, s32);
void *func_8004C5C8(s32, s32);


extern s16 D_8009CF80;


M2C_UNK *func_8004C768(void *arg0);
s32 func_8004C824(void);
void *func_8004C82C(void *);
void *func_8004C944(void *);
void *func_800693DC(void *arg0);
void func_800395E8(void *arg0);


extern s32 D_800D4454;
extern char D_800A8444[];
extern char D_800A844C[];
extern char D_800A8458[];
extern char D_800A8460[];
void func_80042020(void);
void func_800289C0(s32, char *);


extern s16 D_800B0794;
extern u8 D_8009CDD0;
extern s16 D_800AEDA0;
extern s16 D_800AEDA4;
extern s32 D_8009CAC4;
extern char D_800A83B0[];
extern char D_800A83BC[];
extern char D_800A83D4[];
extern char D_800A83E0[];
extern char D_800A83E8[];
extern char D_800A83F0[];
extern char D_800A8400[];
void func_800417C4(void);
void func_80028B58(s32, s32, s32);


extern s32 D_8009CFA8;
extern s16 D_800D91FC;


extern M2C_UNK D_800F10A8;


M2C_UNK func_8007980C(void *, Weapon *);            /* extern */


extern u16 D_8009E4A8;


extern u16 D_8009E4A4;


extern s32 D_8009CFB0;
extern u16 D_800B0C5C;


extern u8 D_8009CDE4, D_8009CDE5, D_8009CDE6;
extern s32 D_800B8F24, D_800D2BE8;
void GsDefDispBuff(u16 x0, u16 y0, u16 x1, u16 y1);
void GsInit3D(void);
void InitGeom(void);
s32 GsGetActiveBuff(void);
void func_8008A904(s32);
void func_800876E4(s32);
void func_8008A404(s32, s32);
void func_8002F624(void);


s32 SquareRoot12(s32);                              /* extern */
s16 ratan2(s16, s32);                               /* extern */


extern u8 D_800D2E84;


void GsInitGraph(u16 x, u16 y, u16 intmode, u16 dith, u16 varmmode);
void func_80032648(u32 *tim);
M2C_UNK func_80032724(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
extern u8 D_8009CDE4;
extern u8 D_8009CDE5;
extern u8 D_8009CDE6;
extern s8 D_800A7330;
extern M2C_UNK D_800D2BC4;
extern s32 D_800D30D4;
extern s32 D_800D31B8;
s32 func_800794D4();                                /* extern */
void PauseMenu_Open(void);
extern u8 D_8009CA90;
extern s32 D_800B0B9C;
extern M2C_UNK D_800AC284;
M2C_UNK func_8002B69C();                            /* extern */
void func_80041A38(void);
extern s32 D_8009CFB4;
s32 func_80082F24(M2C_UNK, M2C_UNK);
M2C_UNK FntLoad(M2C_UNK, M2C_UNK);                  /* extern */
s32 FntOpen(M2C_UNK, M2C_UNK, M2C_UNK, M2C_UNK, s32, s32); /* extern */
M2C_UNK SetDumpFnt(s32, Args *);                    /* extern */
M2C_UNK func_8005099C();                            /* extern */
extern M2C_UNK D_800D90C0;
extern Rec48 D_800D8FF8[];


s32 func_80054098(s32);


extern char D_800A7344[];
void func_80032880(s32 arg0, GsIMAGE *arg1);
void func_800324D0(s32, s16, s16);


extern char D_800A7778[];
extern char D_800A7784[];
void func_80030DA0(u8 *data, s32 len, s32 mode);
void func_80030B04(u8 *);
void func_80061700(void);
void *memchr(void *, int, int);


void func_8007D184(s32, s32, s32, s32);
extern u8 D_8009CABD;
extern u8 D_8009CABE;


extern s16 D_8009CF54;
s32 func_800349F8(void *);


extern char D_800A828C[], D_800A82B4[], D_800A82DC[], D_800A8304[], D_800A8308[], D_800A8310[];
extern char D_800A8318[], D_800A8320[], D_800A832C[], D_800A8340[];


typedef s32 (*CmdHandler)(u8 *, void *, void *);
extern CmdHandler D_8009D7A8[];


void Cheat_DoAction(s32 action);


extern s32 D_8009CD4C;
extern s32 D_800B0C74;
extern s32 D_8009CD54;
extern s32 D_8009D3A4;
extern s32 D_800B0A94;
extern s16 D_8009CDA2;




extern M2C_UNK D_800D45C4;


M2C_UNK func_800636B8();                            /* extern */
s32 rand();                                         /* extern */
extern M2C_UNK *D_8009D8B8;
extern M2C_UNK D_800DA4F8;
extern M2C_UNK D_800DF12C;


extern s8 D_800A7540;
extern M2C_UNK D_800A7564;
extern s8 D_800A7570;
extern s8 D_800A75AC;
extern M2C_UNK D_800A75C0;
extern M2C_UNK D_800FDB2C;


extern s16 D_800AFF80;
extern s16 D_800AFFC0;
extern s16 D_800AFFC4;
extern s16 D_800B008C;
extern s16 D_800B0AA4;
extern s32 D_800B0AA8;
extern s16 D_800B0C30;


extern u16 D_8009E4C0;


unsigned long func_80027248(M2C_UNK, s32, M2C_UNK);


extern u8 D_8009D5E4;


M2C_UNK func_80057E80(s32);                         /* extern */
void func_80062300(void);
M2C_UNK func_80063B20();                            /* extern */
extern s32 D_800B8F24;
extern GsOT D_800B8F88[];


M2C_UNK func_8007F2EC(M2C_UNK *, M2C_UNK *);        /* extern */
extern M2C_UNK D_800B0C7C;
extern M2C_UNK D_800DE8E8;


M2C_UNK func_80029684();                            /* extern */
M2C_UNK func_800456D0();                            /* extern */
M2C_UNK func_80047A04();                            /* extern */
M2C_UNK func_80047DBC();                            /* extern */
void func_800481B0(void);
M2C_UNK func_80048E60();                            /* extern */
M2C_UNK func_80049220(u8 *);                        /* extern */
M2C_UNK func_800493C0();                            /* extern */
M2C_UNK func_80049D60();                            /* extern */
M2C_UNK func_8004A8E4();                            /* extern */
void func_8004AF0C(void);
void func_8004B0B0(void);
extern s8 D_800D74F8;
extern s16 D_800D7928;
extern s16 D_800D792C;
extern s16 D_800D7AB0;
extern s16 D_800D7AB4;
extern s16 D_800D7BE8;
extern s16 D_800D7BEC;
extern s16 D_800D7BF0;
extern s8 D_800D91F8;
extern s8 D_800D9218;


extern s32 D_8009D260;
extern s32 D_8009D27C;
extern s32 D_8009D280;
extern s32 D_8009D29C;
extern s32 D_8009D2A0;
extern s32 D_8009D2A8;
extern s32 D_8009D2BC;
extern s32 D_8009D2CC;
extern s32 D_8009D2D0;
extern s32 D_8009D2D4;
extern s32 D_8009D2F0;
extern s32 D_8009D2F4;
extern s32 D_8009D2F8;
extern s32 D_8009D300;
extern s32 D_8009D310;
extern s32 D_8009D314;
extern s32 D_8009D31C;
extern s32 D_8009D320;
extern s32 D_8009D324;
extern s32 D_8009D328;
extern s32 D_8009D32C;
extern s32 D_8009D330;
extern s32 D_8009D334;
extern s32 D_8009D338;
extern s32 D_8009D33C;
extern s32 D_8009D340;
extern s32 D_8009D344;
extern s32 D_8009D34C;
extern s32 D_8009D350;
extern s32 D_8009D354;
extern s32 D_8009D358;


M2C_UNK SsEnd();                                    /* extern */
M2C_UNK SsVabClose(s16);                            /* extern */
M2C_UNK func_8002B0A0();                            /* extern */
M2C_UNK func_80082F64(M2C_UNK);                     /* extern */
M2C_UNK func_80083388(M2C_UNK);                     /* extern */
M2C_UNK func_800841C4(M2C_UNK);                     /* extern */
M2C_UNK func_80085B9C();                            /* extern */
void func_800268CC(void);
M2C_UNK func_80082E04(M2C_UNK);
M2C_UNK func_80082F78(M2C_UNK);
s16 SsVabOpenHead(s32, M2C_UNK);                    /* extern */
s16 SsVabTransBody(s32);                            /* extern */
M2C_UNK SsVabTransCompleted(M2C_UNK);               /* extern */
extern s8 D_800A763C;
extern s8 D_800A7650;
extern s8 D_800A7664;
extern s8 D_800A7678;
extern M2C_UNK D_800A76A0;
extern s8 D_800A76AC;
extern s8 D_800A7948;
extern s8 D_800A795C;
extern s8 D_800A796C;
extern s8 D_800A7980;
extern s8 D_800A7994;
extern s8 D_800A79B8;
extern s32 D_8009CE3C;
extern s8 D_800A7788;
extern s32 D_800A77A0;
extern s16 D_800AFFA4;
extern s16 D_800B0BA0;
M2C_UNK func_80081458(s16);
M2C_UNK func_8002F564(void *, s32, u32, M2C_UNK, s32); /* extern */
M2C_UNK func_80038984(void *, s32, Weapon *);       /* extern */
extern u32 D_8009D910;
extern s32 D_8009DB38;
M2C_UNK func_80048B4C(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
extern s16 D_8009D104;
extern s16 D_8009D106;
extern s16 D_8009D126;
extern M2C_UNK D_800A87A0;
extern M2C_UNK D_800A87B0;
extern M2C_UNK D_800A87C0;
M2C_UNK func_80030994(u8 *);                        /* extern */
void func_80030D08(u8 arg0, u8 *buf, u8 *name);
M2C_UNK func_80030F20();                            /* extern */
extern s8 D_800A9604;
extern s8 D_800A9614;


M2C_UNK func_80060798();                            /* extern */


void func_8003331C(s32 arg0);
s32 func_8005D58C(s32 arg0);
s32 func_8005D5C4(M2C_UNK *, void *);               /* extern */
extern u8 D_8009E4F0;
extern u8 D_8009E6FC;
extern u8 D_800A9644;
extern M2C_UNK *D_800DDEE0;
extern u8 **D_800DDEE4;


M2C_UNK PadInitDirect(M2C_UNK *, void *);           /* extern */
M2C_UNK func_8008657C();                            /* extern */
M2C_UNK func_80086928(M2C_UNK);                     /* extern */
extern u8 D_800B0DE4;
extern s32 D_800B0E40;
extern s32 g_PadHeld;
extern s32 g_PadPressed;
extern s32 g_PadPrev;
extern s8 D_800B0E54;
extern s32 D_800B0E58;
extern u8 g_PadAxes;
extern u8 g_PadAxesPrev;
void func_8003237C(s32 arg0, s32 arg1);
extern s32 D_8009CE34;
extern M2C_UNK (*D_8009CE40)(s16);
extern s16 D_8009E9C4;
extern u8 D_8009E9DC;
void func_8004782C(void *arg0);
extern u8 D_800D4958;
extern u8 D_800D5DA4;
extern u8 D_800D7CB4;
s32 GetTPage(M2C_UNK, s32, M2C_UNK, M2C_UNK);       /* extern */
M2C_UNK SetDrawMode(M2C_UNK *, M2C_UNK, M2C_UNK, s32, s32); /* extern */
extern u8 D_800D8138;
extern u8 g_CamPosX;


extern u32 D_8009CE38;
s32 func_80045344(POLY_FT4 *p, s32 u, s32 v, s32 x, s32 y, s32 uw, s32 w, s32 h, s32 tpx, s32 tpy, s32 clx, s32 cly);
extern M2C_UNK D_800D5E80;
extern M2C_UNK D_800D8624;
extern M2C_UNK D_800D8674;
extern M2C_UNK D_800D7500;
extern M2C_UNK D_800D7888;
extern M2C_UNK D_800D78D8;
extern M2C_UNK D_800D7B08;
extern M2C_UNK D_800D7CB8;
extern M2C_UNK D_8009D5B4;
extern M2C_UNK D_800A89CC;
extern M2C_UNK D_800A89DC;
extern s32 D_800D9248;


M2C_UNK func_8005D630(s32 *);                       /* extern */
void func_8006B230(s32 arg0, M2C_UNK arg1);
extern M2C_UNK D_800A8FE4;
extern u8 D_800B05B0;


extern s16 D_800DA270;


void func_80047DB4(void);


M2C_UNK func_80076184(s32, M2C_UNK);                /* extern */
extern s16 D_8009CF48;


extern M2C_UNK D_800C1C54;
extern M2C_UNK D_800CA8F4;


M2C_UNK func_800393E4();                            /* extern */
void func_8005414C(void *arg0);
void func_80063ADC(void *arg0);
extern s8 D_8009CABF;
extern s32 D_8009CD64;
extern u8 D_8009D002;
extern s32 D_8009D004;
extern s32 D_8009D26C;
extern u8 D_8009D8D5;
extern s32 D_8009D8D8;


void func_800698CC(void);
extern s32 D_800E23D8;
extern s32 D_800E23F8;


extern M2C_UNK D_800DDF08;
extern M2C_UNK D_800DDFEC;


M2C_UNK func_80026C90(M2C_UNK);                     /* extern */
void func_8002F53C(void);
void func_80041C28(void);
void func_80069850(s32 arg0);
extern s32 D_8009CAC4;
extern u16 D_8009CFC0;
extern s32 D_8009CFC8;


void func_800393AC(void *arg0);
M2C_UNK func_80060614(s32);                         /* extern */


M2C_UNK func_80032768(s32);                         /* extern */
extern M2C_UNK D_800A76C0;
extern s32 D_800D2F44;


M2C_UNK func_8005C864();                            /* extern */


s32 func_800575FC(u8 *cmd, void *arg1, void *arg2);
extern M2C_UNK D_800A8C78;


s32 func_800303B4(u8 mode);
extern M2C_UNK D_800A970C;


extern M2C_UNK D_800A81E0;


s32 exit(M2C_UNK);                                  /* extern */
s32 func_800303B4(u8 mode);
extern M2C_UNK D_800A9710;


M2C_UNK func_80026B00(u8);                          /* extern */
M2C_UNK Menu_ResetState();                            /* extern */
M2C_UNK func_80061708(M2C_UNK);                     /* extern */
M2C_UNK func_8007F0D8(M2C_UNK);                     /* extern */
extern s32 D_8009CA9C;
extern u8 D_800AC38C;
extern s8 D_800AFFA8;
extern s8 D_800AFFCC;
extern s32 D_800AFFD0;
extern s8 D_800B0070;
extern s8 D_800B07C4;
extern s8 D_800B0AE0;
M2C_UNK func_800286EC(M2C_UNK);                     /* extern */
M2C_UNK func_8002872C(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
M2C_UNK func_80042B28();                            /* extern */
extern s32 D_8009CA8C;
extern M2C_UNK D_800A83C8;
extern M2C_UNK D_800A8410;
extern M2C_UNK D_800A8418;
extern s16 D_800B0794;
void func_80042D1C(void);
void func_80042E6C(void);
void func_80043078(void);


s32 func_800536F0(u32 arg0);
extern s16 D_800B07B0;
extern s16 D_800B0DA0;


s32 func_800324C0(void);
M2C_UNK func_80033090();                            /* extern */
M2C_UNK func_800352B8();                            /* extern */
M2C_UNK func_80035F04();                            /* extern */
void func_80037A24(void);
void func_8004BB00(void);
M2C_UNK func_80052DB8();                            /* extern */
M2C_UNK func_8005EF7C();                            /* extern */
M2C_UNK func_8005F3B0();                            /* extern */
M2C_UNK func_8005F788();                            /* extern */
M2C_UNK func_8002D1D4();                            /* extern */
M2C_UNK func_80031D94();                            /* extern */
M2C_UNK func_80074E38();                            /* extern */
M2C_UNK func_80079588();                            /* extern */
M2C_UNK CdInit();                                   /* extern */
M2C_UNK GsInitVcount();                             /* extern */
M2C_UNK SsSetTickMode(M2C_UNK);                     /* extern */
M2C_UNK SsUtReverbOff();                            /* extern */
M2C_UNK func_8002680C();                            /* extern */
void Pad_Init(void);
void Gfx_Init(void);
void func_8004E794(void);
M2C_UNK func_8007EECC(M2C_UNK);                     /* extern */
M2C_UNK func_80085B5C();                            /* extern */
M2C_UNK func_80085BBC(M2C_UNK);                     /* extern */
extern M2C_UNK D_800F1274;
extern M2C_UNK GsTMDdivF3L;
extern M2C_UNK GsTMDdivF3NL;
extern M2C_UNK GsTMDdivG3L;
extern M2C_UNK GsTMDdivG3NL;
extern M2C_UNK GsTMDdivNF3;
extern M2C_UNK GsTMDdivNG3;
extern M2C_UNK GsTMDdivTF3L;
extern M2C_UNK GsTMDdivTF3LFG;
extern M2C_UNK GsTMDdivTF3NL;
extern M2C_UNK GsTMDdivTG3L;
extern M2C_UNK GsTMDdivTG3NL;
extern M2C_UNK GsTMDdivTNF3;
extern M2C_UNK GsTMDdivTNG3;
extern M2C_UNK GsTMDfastF3GL;
extern M2C_UNK GsTMDfastF3GLFG;
extern M2C_UNK GsTMDfastF3GNL;
extern M2C_UNK GsTMDfastF3L;
extern M2C_UNK GsTMDfastF3LFG;
extern M2C_UNK GsTMDfastF3NL;
extern M2C_UNK GsTMDfastG3GL;
extern M2C_UNK GsTMDfastG3GLFG;
extern M2C_UNK GsTMDfastG3GNL;
extern M2C_UNK GsTMDfastG3L;
extern M2C_UNK GsTMDfastG3LFG;
extern M2C_UNK GsTMDfastG3NL;
extern M2C_UNK GsTMDfastNF3;
extern M2C_UNK GsTMDfastNG3;
extern M2C_UNK GsTMDfastTF3L;
extern M2C_UNK GsTMDfastTF3LFG;
extern M2C_UNK GsTMDfastTF3NL;
extern M2C_UNK GsTMDfastTG3L;
extern M2C_UNK GsTMDfastTG3LFG;
extern M2C_UNK GsTMDfastTG3NL;
extern M2C_UNK GsTMDfastTNF3;
extern M2C_UNK GsTMDfastTNG3;
extern M2C_UNK func_80089784;
extern M2C_UNK func_80089B44;
extern M2C_UNK func_8008B074;
s32 func_800302AC();                                /* extern */
extern M2C_UNK D_800A7460;
extern s32 D_800AFFB8;
extern s32 D_800AFFBC;
M2C_UNK func_80029868(s32, M2C_UNK, M2C_UNK, M2C_UNK, s32, s32); /* extern */
M2C_UNK func_8002B064(s32);                         /* extern */
extern void *D_800D4468;
extern s32 D_800DDC44;
extern s32 D_800DDC48;
extern s32 D_800DDC4C;
extern s32 D_800DDC50;
extern s32 D_800DDC54;
extern s32 D_800DDC58;
extern s32 D_800DDC5C;
extern s32 D_800DDC60;
extern s32 D_800DDC64;
extern s32 D_800DDC68;
extern s32 D_800DDCC0;
extern s32 D_800DDCC4;
extern s32 D_800DDCC8;
extern s32 D_800DDD8C;
extern s32 D_800DDD90;
extern s32 D_800DDD94;
extern s32 D_800DDD98;
extern s32 D_800DDDA0;
extern s32 D_800DDDA4;
extern s32 D_800DDDAC;
extern s32 D_800DDDB0;
extern s32 D_800DDDB8;
extern s32 D_800DDE04;
extern s32 D_800DDE08;
extern s32 D_800DDEE8;
extern s32 D_800DDEEC;
extern s32 D_800DE0D8;
extern s16 D_800DE0DC;
extern s16 D_800DE0E0;
void func_8005B96C(void *arg0, s32 arg1, void *arg2);
void func_8007D1E8(s32, s32);
extern s32 D_800D2F8C;
void func_8004DD9C(void);
M2C_UNK func_8005D694();                            /* extern */
M2C_UNK func_800617AC(void *);                      /* extern */
M2C_UNK func_80061B70(void *);                      /* extern */
M2C_UNK func_8007A17C(M2C_UNK);                     /* extern */
s32 func_8007A1B0();                                /* extern */
s32 func_8007A1FC();                                /* extern */
extern s32 D_8009CD50;
extern s16 D_800B0DD4;
extern s16 D_800B0DDC;
extern s32 D_800DDDB4;
extern s32 D_8009D2B4;
extern s32 D_8009D2B8;
extern s32 D_8009D2D8;
extern s32 D_8009D2DC;
extern s32 D_8009D2E0;
extern s32 D_8009D2E4;
extern s32 D_8009D2E8;
extern s32 D_8009D2EC;
extern s32 D_8009D318;
s32 func_8003C88C(s32 arg0, s32 arg1);
extern s32 D_8009D304;
extern s32 D_8009D348;
s32 func_80026D08(M2C_UNK, M2C_UNK);                /* extern */
s32 func_80026F30(M2C_UNK, M2C_UNK);                /* extern */
M2C_UNK func_800272A4(s16, M2C_UNK, M2C_UNK);       /* extern */
M2C_UNK func_80027394(M2C_UNK, s16, M2C_UNK, M2C_UNK); /* extern */
extern s32 D_8009E410;
extern s32 D_8009E414;
extern s32 D_8009E418;


extern s8 D_8009CD73;
extern void *D_8009CD98;
void func_80069B74(void);
M2C_UNK func_800604B0(s16, s16);                    /* extern */
M2C_UNK func_80060588(s32);                         /* extern */
extern s32 D_800AFFC8;
extern s16 D_800B07A8;
extern s16 D_800B0DA4;
extern s16 D_800B0DC0;
M2C_UNK func_80063858();                            /* extern */
M2C_UNK func_80063F84();                            /* extern */
M2C_UNK func_8002BC18();                            /* extern */
extern s32 D_8009D35C;
extern s32 D_800B0DD0;
M2C_UNK func_8008659C();                            /* extern */
extern M2C_UNK D_800B0E34;
void func_8006EEA8(void);
void func_8006F034(void);
M2C_UNK func_8007A17C(M2C_UNK);                     /* extern */
void func_80059C14(u8 *arg0);
M2C_UNK func_80059E4C(M2C_UNK *);                   /* extern */
extern s32 D_8009CF7C;
extern M2C_UNK D_800DDA40;
extern M2C_UNK D_800A81D0;
M2C_UNK Menu_ResetState();                            /* extern */
void func_8004783C(void);
extern s32 D_8009CFC4;
extern s32 D_8009CDC8;
s32 func_80026F30(M2C_UNK, M2C_UNK);
M2C_UNK func_8004218C();                            /* extern */
s16 func_80043B14(M2C_UNK, s16);                    /* extern */
extern s16 D_8009CFBE;
M2C_UNK func_800474AC();                            /* extern */
M2C_UNK func_80028810(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_800A85E4;
extern s32 D_8009CDA8;
extern M2C_UNK D_8009D008;
extern s16 D_8009D00C;
extern s32 D_800D79D0;
extern s8 D_800D8150;
M2C_UNK func_8004CE74();                            /* extern */
void func_8004CF00(u8 *arg0, u8 *arg1);
M2C_UNK func_80069684();                            /* extern */
extern u16 D_800D31B4;
extern M2C_UNK D_800D9234;
M2C_UNK func_8005C894();                            /* extern */
s32 func_8002C168(M2C_UNK *, M2C_UNK);              /* extern */
M2C_UNK func_8005B508();                            /* extern */
void func_80068EB4(void);
extern M2C_UNK D_800A95BC;
extern M2C_UNK D_800A95CC;
void func_80068F80(void);
extern M2C_UNK D_800A95E4;
void func_800690B8(void);
extern M2C_UNK D_800A95F4;
extern M2C_UNK D_8009E440;
extern M2C_UNK D_8009E420;
extern M2C_UNK D_8009E428;
extern M2C_UNK D_8009E430;
extern M2C_UNK D_8009E438;
extern M2C_UNK *D_800AB5B8;
extern M2C_UNK *D_800AB5BC;
extern M2C_UNK *D_800AB5CC;
extern M2C_UNK *D_800AB5D0;
extern M2C_UNK *D_800AB5D4;
extern M2C_UNK *D_800AB5DC;
s32 func_80053B40(s32 arg0);
M2C_UNK func_80054414(M2C_UNK, M2C_UNK, M2C_UNK);   /* extern */
s32 func_80039A9C(M2C_UNK, u8, M2C_UNK);            /* extern */
M2C_UNK func_8006A2B8();                            /* extern */
extern int D_8009D2FC;
extern M2C_UNK D_800A96D4;
extern s32 D_8009D270;
extern s32 D_8009D2AC;


s32 func_800305B4(s32 arg0);
extern s16 D_800B0868;
extern s16 D_800B0C8C;
extern s32 D_800DF0E8;
extern s32 D_800DF0EC;
extern s8 D_8009CDE7;
extern s32 D_800D2BEC;
extern s32 D_800D2E74;
extern s32 D_800D2E78;
extern s32 D_800D2E7C;
extern s32 D_800D2E80;
extern s32 D_800D2FC0;
extern s32 D_800D2F90;
extern s32 D_800D2FA4;
extern s32 D_800D2FA8;
extern s32 D_800D2FAC;
extern s32 D_8009CF10;
extern s16 D_8009CAA4;
extern s32 D_800D5F40;
extern u16 D_800AC4D8;
extern u16 D_800AC4DC;
extern u16 D_800D86C4;
extern u16 D_800D86C8;
extern s16 D_8009D014;
extern s16 D_8009D018;
extern s16 D_8009D01A;
extern s32 D_8009CFFC;
extern s16 D_8009D000;
extern s32 D_8009D010;
extern s32 D_8009CFF0;
extern s16 D_800D9208;
extern s8 D_800D920C;
extern s32 D_8009D3A8;
extern u32 D_800D91E4;
extern s32 D_800D9268;
s32 func_8005AD6C();                                /* extern */
extern s32 D_800F11F0;
extern s32 D_8009D268;
extern s32 D_8009D308;
extern s32 D_8009DB3C;
extern s32 D_8009D30C;
extern s32 D_800E02B8;
extern s32 D_8009D274;
extern s32 D_8009D2B0;



extern int g_CheatCodeProgress[];
extern int g_CheatCodes[];

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", start);

void func_8002C83C(void) {
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002C844);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", main);

void func_8002D0F0(void) {
    GsIMAGE sp18;

    if (g_Args->missionComplete == 0) {
        func_80032724(0, 0, 0x280, 0x1E0);
        g_Args->displayW = 0x280;
        g_Args->displayH = 0xF0;
        GsInitGraph(g_Args->displayW, 0xF0, D_8009CDE4, D_8009CDE5, (s32) D_8009CDE6);
        sprintf(g_Args->exePath, &D_800A7330);
        func_8002C168((s32 *) g_Args->exePath, D_800D2BEC);
        func_80032648(D_800D2BEC);
        func_80032880(D_800D2BEC, &sp18);
        func_800324D0(D_800D2BEC, sp18.px, (s16) ((u16) sp18.py + 0xF0));
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002D1D4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002D4E4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002D604);

void func_8002D708(void) {
    s32 t;

    if (*(u8 *)g_Args >> 7) {
        return;
    }
    if (g_Args->unk115 == 0) {
        if (g_Args->missionComplete == 0) {
            D_8009CA90 = 0;
            return;
        }
    }
    if (D_8009CAA8 == 0) {
        func_8002D604();
        D_8009CACC = D_800B00A0 * (t = g_FrameRate * 60) / 60;
        D_8009CAD0 = D_800B0AA0 * t / 60;
    }
    if (D_8009CA78 != 0 && (g_PadHeld & 0x800)) {
        D_8009CACC--;
        D_8009CAD0--;
        D_8009CAA8++;
        func_8004D88C();
    }
    D_8009CAA8++;
    func_8004900C();
    D_800B0B8C = 0;
    D_800B0B88 = 0;
    D_800B0B84 = 0;
    D_8009CA84 = 0;
    D_8009CA80 = 0;
    D_8009CA7C = 0;
    M2C_FIELD(g_Player, s16 *, 0x52) = 0;
    M2C_FIELD(g_Player, s16 *, 0x9C) = 0;
    M2C_FIELD(g_Player, s16 *, 0x9A) = 0;
    M2C_FIELD(g_Player, s16 *, 0x98) = 0;
    if (D_8009CAA8 >= D_8009CACC && (func_8004B084() << 0x10) == 0) {
        Menu_ResetState();
        func_8004B04C();
    }
    if ((func_8004B06C() << 0x10) != 0) {
        D_800B8FB0[2] = 0;
        D_800B8FB0[1] = 0;
        D_800B8FB0[0] = 0;
        if (g_Args->unk115 != 0) {
            D_8009CA9C = 1;
            g_Args->flags &= ~0x80;
            SsUtAllKeyOff(0);
            D_8009CFEC = 1;
            func_8002B0A0();
            D_8009CAA4 = 1;
            RestartPrompt_Open();
            return;
        }
        g_Args->flags |= 0x80;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", Cheat_DoAction);

s32 Cheat_CheckCodes(void) {
    s32 triggered;
    s32 i;
    int *progress;
    volatile int *codes;
    u32 *p;
    u32 mask;
    u8 *args;
    s32 hit;

    triggered = 0;
#ifdef MOD_DEVCHEATS
    if (1) { /* retail skips the developer cheat codes when running from the CD archive */
#else
    if (((s8 *)g_Args)[0x1BC] == 0) {
#endif
        i = 0;
        progress = g_CheatCodeProgress;
        codes = g_CheatCodes;
        do {
            p = (u32 *)*progress;
            mask = *p;
            if (g_PadPressed & mask) {
                p++;
                if (*p == -1) {
                    D_8009CA9C = 0;
                    hit = 1;
                } else {
                    *progress = (int)p;
                    hit = 0;
                }
            } else {
                if (g_PadPressed != 0) {
                    args = g_Args;
                    *progress = *codes;
                    if (((s8 *)args)[0x1BC] == 0) {
                        *progress += 0x10;
                    }
                }
                hit = 0;
            }
            if (hit) {
                Cheat_DoAction(i);
                triggered = 1;
            }
            progress++;
            i++;
            codes++;
        } while (i < 15);
    }
    return triggered;
}

void func_8002DD6C(u8 *obj) {
    s32 i;
    Part *part;

    if (obj[5] == 6) {
        return;
    }
    if (*(u16 *)(obj + 0x28) != 0) {
        *(s16 *)(obj + 0x1C) = (*(s16 *)(*(u8 **)obj + 0x10) * *(s16 *)(obj + 0x6A)) >> 12;
        *(s16 *)(obj + 0x1E) = (*(s16 *)(*(u8 **)obj + 0x12) * *(s16 *)(obj + 0x6A)) >> 12;
        *(s16 *)(obj + 0x20) = (*(s16 *)(*(u8 **)obj + 0x14) * *(s16 *)(obj + 0x6A)) >> 12;
        *(s16 *)(obj + 0x22) = (*(s16 *)(*(u8 **)obj + 0x16) * *(s16 *)(obj + 0x6A)) >> 12;
        *(s16 *)(obj + 0x24) = (*(s16 *)(*(u8 **)obj + 0x18) * *(s16 *)(obj + 0x6A)) >> 12;
        *(s16 *)(obj + 0x26) = (*(s16 *)(*(u8 **)obj + 0x1A) * *(s16 *)(obj + 0x6A)) >> 12;
        *(u16 *)(obj + 0x28) = (*(u16 *)(*(u8 **)obj + 0x1C) * *(s16 *)(obj + 0x6A)) >> 12;
        *(u16 *)(obj + 0x2A) = (*(u16 *)(*(u8 **)obj + 0x1E) * *(s16 *)(obj + 0x6A)) >> 12;
        *(s16 *)(obj + 0x3C) = (*(s16 *)(*(u8 **)obj + 0x30) * *(s16 *)(obj + 0x6A)) >> 12;
        *(s16 *)(obj + 0x3E) = (*(s16 *)(*(u8 **)obj + 0x32) * *(s16 *)(obj + 0x6A)) >> 12;
        *(s16 *)(obj + 0x40) = (*(s16 *)(*(u8 **)obj + 0x34) * *(s16 *)(obj + 0x6A)) >> 12;
    }
    part = (Part *)*(u8 **)(obj + 0x90);
    part++;
    for (i = 1; i < obj[0xB]; i++, part++) {
        part->x = (((HierRow *)D_800D86E0)[(*(u8 **)obj)[0x5F]].x[i] * *(s16 *)(obj + 0x6A)) >> 12;
        part->y = (((HierRow *)D_800D86E0)[(*(u8 **)obj)[0x5F]].y[i] * *(s16 *)(obj + 0x6A)) >> 12;
        part->z = (((HierRow *)D_800D86E0)[(*(u8 **)obj)[0x5F]].z[i] * *(s16 *)(obj + 0x6A)) >> 12;
    }
}

void ApplyBonusCheats(void) {
    s32 i;
    Weapon *p;
    s32 j;
    Weapon *w;
    s32 max;
    s32 clip;

    if (((g_Args->flags2 >> 15) & 1) && g_Args->level != 11) {
        p = g_Weapons;
        for (i = 0; i < 6; i++, p++) {
            if (i != 5) {
                p->flags |= 0x1800;
            }
        }
        j = 0;
        w = g_Weapons;
        do {
            max = w->maxAmmo;
            clip = w->clipSize;
            j++;
            w->ammo = max;
            w->clip = clip;
            w->reserve = max - clip;
            w++;
        } while (j < 6);
    }
    if (M2C_FIELD(g_Args, u16 *, 6) & 1) {
        g_Invincible = 1;
        g_GodMode = 1;
        M2C_FIELD(g_Player, s16 *, 0xE) = 0x7FFF;
    }
}

void func_8002E0E8(void) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(g_Player, void **, 0x8C);
    D_8009CD73 = 0;
    if (temp_a0 != NULL) {
        M2C_FIELD(temp_a0, s32 *, 0x7C) = (s32) (M2C_FIELD(temp_a0, s32 *, 0x7C) | 0x80000000);
    }
    if (D_8009CD98 != NULL) {
        M2C_FIELD(D_8009CD98, s32 *, 0x7C) = (s32) (M2C_FIELD(D_8009CD98, s32 *, 0x7C) | 0x80000000);
    }
}

void Cheat_ResetCodes(void) {
    int *dst;
    int *src;
    int i;
    int add;

    dst = g_CheatCodeProgress;
    add = (g_Args->cdArchive == 0) << 4;
    src = g_CheatCodes;
    i = 0;
    do {
        i++;
        *dst++ = *src++ + add;
    } while (i < 15);
}

void func_8002E190(void) {
}

int Args_GetFlag115(void) {
    return g_Args->unk115 != 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002E1B0);

void func_8002E228(void) {
  int new_var;
  D_800B0AA8 = ((((((D_800B0868 + (new_var = D_800B0C8C + 1)) + D_800AFFC0) + D_800B0AA4) + D_800B008C) + D_800B0C30) + D_800AFF80) + D_800AFFC4;
}

void Cheat_AllWeapons(void) {
    int i;
    Weapon *p = g_Weapons;

    for (i = 0; i < 6; i++, p++) {
        if (i != 5) {
            p->flags |= 0x1800;
        }
    }
}

void Cheat_RefillAmmo(void) {
    Weapon *w;
    s32 i;
    s32 max;
    s32 clip;

    i = 0;
    w = g_Weapons;
    do {
        max = w->maxAmmo;
        clip = w->clipSize;
        i++;
        w->ammo = max;
        w->clip = clip;
        w->reserve = max - clip;
        w++;
    } while (i < 6);
}

void func_8002E318(void) {
    GsIMAGE image;

    sprintf(g_Args->exePath, D_800A7344, g_Args->level);
    func_8002C168(g_Args->exePath, D_800D2BEC);
    func_80032648(D_800D2BEC);
    func_80032880(D_800D2BEC, &image);
    func_800324D0(D_800D2BEC, image.px, image.py + 0xF0);
}

void func_8002E3A0(void) {
    func_8005F788();
    func_8005EF7C();
    func_8005F3B0();
    if (func_800324C0() != 0) {
        func_800352B8();
    }
    func_80052DB8();
    func_80037A24();
    func_80033090();
    func_8004BB00();
    func_80035F04();
}

void func_8002E410(void) {
    func_80069B74();
}

void func_8002E430(void) {
    func_800604B0(D_800B0DC0, D_800B07A8);
    func_80060588(D_800AFFC8 + (D_800B0DA4 * 0x124));
}

void func_8002E48C(void) {
    func_80063F84();
    func_80063858();
}

void func_8002E4B4(void) {
    func_8002BC18();
}

void func_8002E4D4(void) {
}

void func_8002E4DC(void) {
    D_1F8003D4 = 0;
    D_1F8003D8 = 0;
    D_1F8003DC = 0x1F4;
    if (func_80039A9C(0, *(u8 *)&D_8009D26C, 8) != 0) {
        D_8009CABF = 1;
    }
}

void func_8002E534(s32 arg0) {
    switch (arg0) {                                 /* irregular */
    case 1:
        func_80079588();
        return;
    case 2:
        func_8002D1D4();
        return;
    case 3:
        func_80031D94();
        return;
    case 4:
        func_80074E38();
        return;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002E5C4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002E798);

void func_8002F0A4(void) {
    func_80085BBC(0);
    func_8007EECC(1);
    CdInit();
    GsInitVcount();
    func_80085B5C();
    SsSetTickMode(1);
    SsUtReverbOff();
    Pad_Init();
    Gfx_Init();
    func_8004E794();
    func_8002680C();
}

void func_8002F114(void) {
    func_8002F53C();
    func_80026C90(-1);
    func_8002B0A0();
    M2C_FIELD(g_Args, s32 *, 0) = (s32) (M2C_FIELD(g_Args, s32 *, 0) & ~8);
    func_800268CC();
    SsVabClose(M2C_FIELD(g_Args, s16 *, 0x12C));
    SsEnd();
    func_80085B9C();
    func_8007EECC(3);
    func_80083388(0);
    func_80082E04(0);
    func_80082F78(0);
    func_80082F64(0);
    func_800841C4(0);
    func_8002F51C();
}

void func_8002F1BC(void) {
    D_800B0DD0 = 0x20000;
    M2C_FIELD(((D_8009D35C * 0x70) + M2C_FIELD(g_Args, s32 *, 0x228)), s32 *, 0x20) = 0x20000;
}

void func_8002F1F4(void) {
    D_800DF0E8 = func_800305B4(D_800B0C8C * 4);
    D_800DF0EC = func_800305B4(D_800B0868 * 4);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", Pad_Update);

void Pad_Init(void) {
    D_800B0E54 = 0;
    D_800B0E58 = 6;
    g_PadHeld = 0;
    g_PadPrev = 0;
    g_PadPressed = 0;
    D_800B0E40 = 0;
    M2C_FIELD(&g_PadAxes, s8 *, 0) = 0x7F;
    M2C_FIELD(&g_PadAxes, s8 *, 1) = 0x7F;
    M2C_FIELD(&g_PadAxes, s8 *, 2) = 0x7F;
    M2C_FIELD(&g_PadAxes, s8 *, 3) = 0x7F;
    M2C_FIELD(&g_PadAxesPrev, s8 *, 0) = 0x7F;
    M2C_FIELD(&g_PadAxesPrev, s8 *, 1) = 0x7F;
    M2C_FIELD(&g_PadAxesPrev, s8 *, 2) = 0x7F;
    M2C_FIELD(&g_PadAxesPrev, s8 *, 3) = 0x7F;
    PadInitDirect(&D_800B0DE4, &D_800B0DE4 + 0x22);
    func_8008657C();
    func_8002F53C();
    func_80086928(1);
}

void func_8002F51C(void) {
    func_8008659C();
}

void func_8002F53C(void) {
    M2C_FIELD(&D_800B0E34, s8 *, 0) = 0;
    M2C_FIELD(&D_800B0E34, s8 *, 1) = 0;
    M2C_FIELD(&D_800B0E34, s8 *, 2) = 0;
    M2C_FIELD(&D_800B0E34, s16 *, 4) = 0;
    M2C_FIELD(&D_800B0E34, s16 *, 6) = 0;
    M2C_FIELD(&D_800B0E34, s16 *, 8) = 0;
    M2C_FIELD(&D_800B0E34, s16 *, 0xA) = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002F564);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002F624);

void Gfx_Init(void) {
    g_Projection = 300;
    g_Args->screenW = 512;
    g_Args->screenH = 240;
    g_Args->displayW = g_Args->screenW;
    g_Args->displayH = g_Args->screenH;
    GsInitGraph(g_Args->displayW, g_Args->displayH, D_8009CDE4, D_8009CDE5, D_8009CDE6);
    GsDefDispBuff(0, 0, 0, g_Args->displayH);
    GsInit3D();
    InitGeom();
    func_8008A904(100);
    func_800876E4(g_Projection);
    gte_SetGeomOffset((s16)g_Args->displayW >> 1, (s16)g_Args->displayH >> 1);
    func_8008A404(0, 0);
    func_8002F624();
    D_800B8F24 = GsGetActiveBuff();
    D_800D2BE8 = D_800B8F24 ^ 1;
    g_FrameRate = 60;
}

void func_8002F7CC(void) {
    s32 skip;
    u8 *m;

    skip = (s16)func_8004B06C();
    if (g_Args->cdArchive == 0) {
        __asm__ volatile("break 1024");
    }
    D_800B8F24 = 1 - D_800B8F24;
    D_800D3234 = (s32)D_800B8F88[D_800B8F24].org;
    ((void (*)(s32))SetDumpFnt)(D_800B8F24 == 1 ? M2C_FIELD(g_Args, s32 *, 0x1C4) : M2C_FIELD(g_Args, s32 *, 0x1C0));
    func_80087704(D_800B8FB4[D_800B8F24]);
    GsClearOt(0, 0, &D_800B8F88[D_800B8F24]);
    GsClearOt(0, 0, D_800B8EFC[D_800B8F24]);
    GsClearOt(0, 0, &D_800D2AFC[D_800B8F24]);
    if (skip == 0) {
        func_8004B32C();
        if (D_800E2E44 != 0 && M2C_FIELD(D_800E2E44, s32 *, 0x14) != 0) {
            func_8006F784();
        }
        if (D_800D3064 == 1) {
            func_8006F6AC();
            m = D_800E2DA8;
            func_8006CEA4(((s32 *)(m + 0x30))[D_800B8F24]);
        }
        func_80030354();
        func_800303B4(0);
        func_8005AEF8();
        func_800303B4(1);
        func_800781B4();
        func_80060410();
        func_8007545C(D_800B0DA4, D_800B07B4);
        if (D_8009CD6E != 0) {
            func_80077D50(D_800B0DC0, D_800B07A4);
        }
        func_80077FC4(D_800B0DB0, D_800B07C0);
        if (D_8009CDC0 != 0) {
            func_800406A0();
        }
        func_8004B58C(D_8009CF30);
        GsSortOt(D_800B8EFC[D_800B8F24], &D_800B8F88[D_800B8F24]);
        GsSortOt(&D_800D2AFC[D_800B8F24], &D_800B8F88[D_800B8F24]);
    }
    if (D_8009CDE8 != 0) {
        func_800463C8();
    }
    if (D_8009C1F0 > 0) {
        D_8009C1F0--;
    }
    func_8004514C();
    if (D_800AC54C > 0) {
        if (*(u8 *)&D_8009C1EE != 0) {
            func_80028DBC((s32)D_800B8F88[D_800B8F24].org, D_800B8F24);
        } else {
            func_80028C08(skip ? (s32)D_800B8F88[D_800B8F24].org : D_800B8EFC[D_800B8F24][1], D_800B8F24);
        }
    }
    if (g_DevMenu[0] != 0) {
        DevMenu_Draw();
    } else if (g_DevMenu[14] != 0) {
        DebugScreen_DrawPage(g_DevMenu[14]);
    }
    func_8007F0D8(0);
    D_800D2BE8 = D_800B8F24;
    func_80030234();
    g_Args->pad118[0x13] = 1;
    VSync(60 / g_FrameCap);
    GsSwapDispBuff();
    if (skip) {
        GsSortClear(0, 0, 0, &D_800B8F88[D_800D2BE8]);
    } else {
        GsSortClear(D_800B8FB0[0], D_800B8FB0[1], D_800B8FB0[2], &D_800B8F88[D_800D2BE8]);
    }
    func_8007F4C0(D_800B8F88[D_800D2BE8].tag);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", DebugScreen_DrawPage);

void func_8002FE04(void) {
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0) = &GsTMDfastF3L;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 4) = &GsTMDfastF3LFG;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 8) = &GsTMDfastF3NL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0xC) = &func_80089784;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x10) = &GsTMDdivF3L;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x14) = &GsTMDdivF3NL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x18) = &GsTMDfastNF3;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x1C) = &GsTMDdivNF3;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x20) = &GsTMDfastG3L;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x24) = &GsTMDfastG3LFG;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x28) = &GsTMDfastG3NL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x2C) = &func_80089B44;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x30) = &GsTMDdivG3L;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x34) = &GsTMDdivG3NL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x38) = &GsTMDfastNG3;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x3C) = &GsTMDdivNG3;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x40) = &GsTMDfastTF3L;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x44) = &GsTMDfastTF3LFG;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x48) = &GsTMDfastTF3NL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x4C) = &GsTMDdivTF3L;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x50) = &GsTMDdivTF3LFG;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x54) = &GsTMDdivTF3NL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x58) = &GsTMDfastTNF3;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x5C) = &GsTMDdivTNF3;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x60) = &GsTMDfastTG3L;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x64) = &GsTMDfastTG3LFG;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x68) = &GsTMDfastTG3NL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x6C) = &func_8008B074;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x70) = &GsTMDdivTG3L;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x74) = &GsTMDdivTG3NL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x78) = &GsTMDfastTNG3;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x7C) = &GsTMDdivTNG3;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x100) = &GsTMDfastF3GL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x104) = &GsTMDfastF3GLFG;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x108) = &GsTMDfastF3GNL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x10C) = &GsTMDfastG3GL;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x110) = &GsTMDfastG3GLFG;
    M2C_FIELD(&D_800F1274, M2C_UNK **, 0x114) = &GsTMDfastG3GNL;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002FFD8);

void func_80030118(void) {
    u32 r;
    u8 *p;

    if (func_8007F0D8(1) == 0) {
        func_8007F34C(D_800B8F68, 8);
        D_800D2AF8 = 0;
        if (M2C_FIELD(&D_800D2BC4, s16 *, 0x10) < M2C_FIELD(&D_800D2BC4, s16 *, 8) + 0x170) {
            r = (u32)g_Args->unk2DC % 13;
            if (r == 3 || r == 5 || r == 8 || r == 10 || r == 0) {
                M2C_FIELD(&D_800D2BC4, s16 *, 0x10)++;
                M2C_FIELD(&D_800D2BC4, s16 *, 0x20)++;
            }
        }
        p = (u8 *)&D_800D2BC4;
        AddPrim(&D_800B8F68[D_800D2AF8], p);
        D_800D2AF8++;
        func_8007F4C0(D_800B8F68);
    }
}

void func_80030234(void) {
    s32 temp_v0;

    temp_v0 = func_800302AC();
    D_800AFFB8 = temp_v0;
    if (temp_v0 < D_800AFFBC) {
        D_800AFFBC = temp_v0;
    }
    if ((M2C_FIELD(g_Args, s8 *, 0x1BC) == 0) && ((u32) (temp_v0 - 1) > 0x8C9FU)) {
        sprintf(g_Args->exePath, &D_800A7460, temp_v0);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800302AC);

M2C_UNK *func_80030304(s32 arg0) {
    if (arg0 == 0) {
        return &D_800C1C54;
    }
    return &D_800CA8F4;
}

void func_80030324(void) {
    D_8009CDE7 = 1;
}

void func_80030334(void) {
    s16 temp_v0;

    temp_v0 = M2C_FIELD(&D_800D2BC4, u16 *, 8) + 0x170;
    M2C_FIELD(&D_800D2BC4, s16 *, 0x20) = temp_v0;
    M2C_FIELD(&D_800D2BC4, s16 *, 0x10) = temp_v0;
}

void func_80030354(void) {
  s8 *new_var2;
  if (D_8009CAA4 == 0)
  {
    func_80062300();
    func_80063B20();
  }
  new_var2 = &D_800B8F88;
  func_80057E80(*((s32 *) (new_var2 = ((s8 *) ((10 * (D_800B8F24 * 2)) + new_var2)) + 4)));
}

s32 func_800303B4(u8 mode) {
    u32 sp;
    u32 offset;
    u32 *ram;
    u32 *scr;
    u32 a, b, c, d;

    sp = func_80026800();
    if (mode == 0) {
        D_8009CDEC = mode;
        if (sp - 0x1F800000 >= 0x400) {
            return 0;
        }
        offset = 0x1F8003D0 - sp;
        ram = (u32 *)D_800D2E74;
        scr = (u32 *)0x1F8003D0;
        do {
            a = *scr--;
            b = *scr--;
            c = *scr--;
            d = *scr--;
            *ram-- = a;
            *ram-- = b;
            *ram-- = c;
            *ram-- = d;
        } while ((u32)(scr - 4) > 0x1F800000);
        D_800D2E70 = func_800855BC(D_800D2E74 - offset);
    } else if (mode == 1) {
        D_8009CDEC = mode;
        if (sp - 0x1F800000 < 0x400) {
            return 0;
        }
        offset = D_800D2E74 - sp;
        ram = (u32 *)D_800D2E74;
        scr = (u32 *)0x1F8003D0;
        do {
            a = *ram--;
            b = *ram--;
            c = *ram--;
            d = *ram--;
            *scr-- = a;
            *scr-- = b;
            *scr-- = c;
            *scr-- = d;
        } while ((u32)(scr - 4) > 0x1F800000);
        D_800D2E70 = func_800855BC(0x1F8003D0 - offset);
    }
    return 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003052C);

s32 func_800305B4(s32 arg0) {
    void *p;

    p = malloc3(arg0);
    if (p == NULL) {
        printf(D_800A74E0, arg0);
        exit(-1);
    }
    arg0 = ((s32 *)p)[-1] * 8;
    M2C_FIELD(g_Args, s32 *, 0x1F4) += arg0;
    return (s32)p;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030610);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030670);

void func_800306A8(void) {
    D_800D2E78 = 0x800FF16C;
    D_800D2E80 = 0x2800;
    D_800D2E7C = 0x80101F6C;
    D_800D2BEC = 0x80101F6C;
    D_800D2E74 = 0x80101F5C;
}

void func_800306F0(void) {
  u32 new_var;
  new_var = (u32) (&D_800FDB2C);
  if (((u32) D_800D2E78) < new_var)
  {
    printf(&D_800A7540, D_800D2E78, &D_800FDB2C);
    func_8006B230(0, (s32) (&D_800A7564));
  }
  if (((u32) D_800D2E74) > ((u32) D_800D2E7C))
  {
    printf(&D_800A7570, D_800D2E7C, D_800D2E74);
    printf(&D_800A75AC, D_800D2E74 - D_800D2E7C);
    func_8006B230(0, (s32) (&D_800A75C0));
  }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030798);

void func_80030868(void) {
    s16 temp_v0;

    sprintf((s8 *) (g_Args->exePath), &D_800A763C, M2C_FIELD(g_Args, s16 *, 0x188));
    func_8002C168((s32 *) (g_Args->exePath), D_800D2E78);
    sprintf((s8 *) (g_Args->exePath), &D_800A7650, M2C_FIELD(g_Args, s16 *, 0x188));
    func_8002C168((s32 *) (g_Args->exePath), D_800D2E7C);
    temp_v0 = SsVabOpenHead(D_800D2E78, -1);
    M2C_FIELD(g_Args, s16 *, 0x12C) = temp_v0;
    if (temp_v0 < 0) {
        printf(&D_800A7664, temp_v0);
        printf(&D_800A7678, &D_800A76A0, 0xB0);
        exit(1);
    }
    if (SsVabTransBody(D_800D2E7C) != M2C_FIELD(g_Args, s16 *, 0x12C)) {
        printf(&D_800A76AC);
        printf(&D_800A7678, &D_800A76A0, 0xB9);
        exit(1);
    }
    SsVabTransCompleted(1);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030994);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030B04);

void func_80030D08(u8 arg0, u8 *buf, u8 *name) {
    s32 len;

    func_8006B230(name != NULL, D_800A7778);
    sprintf(g_Args->exePath, D_800A7784, name);
    len = func_8002C168(g_Args->exePath, buf);
    buf[len] = 0xFF;
    func_80030DA0(buf, len + 1, arg0 & 0xFF);
}

void func_80030DA0(u8 *data, s32 len, s32 mode) {
    u8 buf[0x78];
    u8 *line;
    u8 *end;
    u8 *src;
    u8 *dst;
    u8 *eol;

    end = data + len;
    eol = line = data;
    while (eol != NULL && eol <= end) {
        eol = memchr(line, '\n', len);
        if (eol == NULL) {
            eol = memchr(line, 0xFF, len + 1);
            if (eol == NULL) {
                break;
            }
        }
        dst = buf;
        for (src = line; src != eol && src < line + 0x50;) {
            *dst++ = *src++;
        }
        *dst = 0;
        if ((u8)mode == 5) {
            if (buf[0] != ';') {
                func_80030B04(buf);
            }
        } else {
            if (buf[0] == '.') {
                ((void (*)(u8 *))func_80061700)(buf);
            }
        }
        line = eol + 1;
    }
}

void func_80030EB0(void) {
    s16 temp_a2;

    temp_a2 = M2C_FIELD(g_Args, s16 *, 0x188);
    sprintf(g_Args->exePath, &D_800A76C0, temp_a2, temp_a2);
    func_8002C168(g_Args->exePath, D_800D2BEC);
    func_80032768(D_800D2BEC);
    D_800D2F44 = 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030F20);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030FA8);

void func_800310B4(s32 base) {
    s16 *start;
    s32 t;
    s32 i;
    s32 k;
    s32 *pos;
    s32 *pos2;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 ex;
    s32 ey;
    s32 ez;
    u8 *obj;
    u8 *elem;
    u8 *obj2;
    s32 j;
    s32 *p;

    if (M2C_FIELD(base, s32 *, 0x14) != 0) {
        M2C_FIELD(base, s32 *, 0x14) = base + M2C_FIELD(base, s32 *, 0x14);
    }
    if (M2C_FIELD(base, s32 *, 0) != 0) {
        M2C_FIELD(base, s32 *, 0) = base + M2C_FIELD(base, s32 *, 0);
        func_8006F168(M2C_FIELD(base, s32 *, 0), M2C_FIELD(base, s32 *, 0x14));
    }
    if (M2C_FIELD(base, s32 *, 4) != 0) {
        M2C_FIELD(base, s32 *, 4) = base + M2C_FIELD(base, s32 *, 4);
        t = M2C_FIELD(base, s32 *, 4);
        M2C_FIELD(t, s32 *, 8) = t + M2C_FIELD(t, s32 *, 8);
        M2C_FIELD(t, s32 *, 0xC) = t + M2C_FIELD(t, s32 *, 0xC);
        M2C_FIELD(t, s32 *, 0x10) = t + M2C_FIELD(t, s32 *, 0x10);
        elem = M2C_FIELD(t, u8 **, 8);
        for (k = 0; k < M2C_FIELD(t, s32 *, 0); k++) {
            p = (s32 *)elem + 1;
            p[k * 5] = t + p[k * 5];
        }
    }
    if (M2C_FIELD(base, s32 *, 8) != 0) {
        M2C_FIELD(base, s32 *, 8) = base + M2C_FIELD(base, s32 *, 8);
        func_8006FA54(M2C_FIELD(base, s32 *, 8));
    }
    D_800E2F54 = M2C_FIELD(base, s32 *, 8);
    if (M2C_FIELD(base, s32 *, 0xC) != 0) {
        M2C_FIELD(base, s32 *, 0xC) = base + M2C_FIELD(base, s32 *, 0xC);
        func_8006FA54(M2C_FIELD(base, s32 *, 0xC));
        D_800E2F2C = M2C_FIELD(base, s32 *, 0xC);
    } else {
        D_800E2F2C = D_800E2F54;
    }
    if (M2C_FIELD(base, s32 *, 0x10) != 0) {
        M2C_FIELD(base, s32 *, 0x10) = base + M2C_FIELD(base, s32 *, 0x10);
    }
    D_800E2E44 = M2C_FIELD(base, s32 *, 0x10);
    func_8006F9A4(M2C_FIELD(base, s32 *, 0x10));
    if (D_8009CE28 != 0) {
        start = D_800D2F94;
        D_800D2F90 = 0;
        D_8009CE28 = 0;
    } else if (M2C_FIELD(&D_8009E9C4, s16 **, 0x10) != NULL) {
        start = M2C_FIELD(&D_8009E9C4, s16 **, 0x10);
        D_800D2F90 = 1;
        func_8004B05C();
    } else {
        start = &D_8009E9C4;
        D_800D2F90 = 0;
    }
    func_80030FA8(start);
    func_800636B8();
    func_80058D00();
    func_8005FF60();
    pos = (s32 *)(D_800D2FC0 + 0x18);
    if (D_800AFFC8 != 0) {
        dx = pos[0] - D_800D2FB0.x;
        dy = pos[1] - D_800D2FB0.y;
        dz = pos[2] - D_800D2FB0.z;
        obj = (u8 *)D_800AFFC8 + 0x124;
        for (i = 1; i < D_800B0AA8; i++, obj += 0x124) {
            if (M2C_FIELD(obj, u8 *, 4) != 0) {
                M2C_FIELD(M2C_FIELD(obj, u8 **, 0x90), s32 *, 0x28) += dx;
                M2C_FIELD(M2C_FIELD(obj, u8 **, 0x90), s32 *, 0x2C) += dy;
                M2C_FIELD(M2C_FIELD(obj, u8 **, 0x90), s32 *, 0x30) += dz;
            }
        }
    }
    pos2 = (s32 *)(D_800D2FC0 + 0x18);
    if (D_8009CF7C != 0) {
        ex = pos2[0] - D_800D2FB0.x;
        ey = pos2[1] - D_800D2FB0.y;
        ez = pos2[2] - D_800D2FB0.z;
        obj2 = (u8 *)D_8009CF7C;
        for (j = 1; j < D_8009CF80; j++, obj2 += 0x44) {
            if (M2C_FIELD(obj2, u8 *, 0) != 0) {
                M2C_FIELD(obj2, s32 *, 0x10) += ex;
                M2C_FIELD(obj2, s32 *, 0x14) += ey;
                M2C_FIELD(obj2, s32 *, 0x18) += ez;
            }
        }
    }
    D_800D2FB0 = *(Vec4i *)(D_800D2FC0 + 0x18);
    D_800D2F8C = *start;
    func_8003C6E4();
    func_8003BFB8();
    if (D_8009CABD != 0) {
        func_8006ECBC();
    } else if (D_8009CABE != 0) {
        func_8006EEA8();
    }
    func_80053DAC();
    if (D_8009CE44 != NULL) {
        D_8009CE44(D_800D2F8C);
    }
}

void func_800314A0(void) {
    s32 level;
    s32 buf;
    s32 id;
    u32 p;

    id = D_800D2F8C;
    D_800D2F94[0] = D_800D2F8C;
    D_800D2F94[1] = gPsi;
    D_800D2F94[2] = *(s16 *)((u8 *)g_Args + 0x208);
    D_8009CE28 = 1;
    D_800D2F94[3] = *(s16 *)((u8 *)g_Args + 0x20C);
    D_800D2F90 = 4;
    D_800D2F94[4] = *(s16 *)((u8 *)g_Args + 0x210);
    D_8009E9D8 = 0;
    buf = D_800D2FC0;
    level = *(s16 *)((u8 *)g_Args + 0x188);
    sprintf(g_Args->exePath, D_800A77B4, level, level, id);
    func_8002C168((s32 *)g_Args->exePath, buf);
    func_80032768(buf);
    sprintf(g_Args->exePath, D_800A77CC, level, level, id);
    func_8002C168((s32 *)g_Args->exePath, buf);
    D_800D2F90 = 5;
    func_800310B4(buf);
    p = D_8009CE38;
    sprintf(g_Args->exePath, &D_800A7788, g_Args->level, g_Args->level, D_800D2F8C);
    if (func_8002C168((s32 *)g_Args->exePath, p) > 0x1800) {
        func_8006B1DC((s32)g_Args->exePath);
        func_8006B1DC((s32)&D_800A77A0);
        exit(-1);
    }
    M2C_FIELD(p, s32 *, 0xC) = p + M2C_FIELD(p, s32 *, 0xC);
    M2C_FIELD(p, s32 *, 0x10) = p + M2C_FIELD(p, s32 *, 0x10);
    M2C_FIELD(p, s32 *, 0x14) = p + M2C_FIELD(p, s32 *, 0x14);
    if (M2C_FIELD(p, s32 *, 0) != 0) {
        D_8009CE3C = 1;
    } else {
        D_8009CE3C = 0;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80031678);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80031B58);

void DebugPage_Loadpoint(void) {
    s16 *var_s0;

    g_DebugTextX = 0x29;
    g_DebugTextY = 5;
    if (D_8009CE34 >= 0) {
        var_s0 = (D_8009CE34 * 0x10) + &D_8009E9DC;
        FntPrint(&D_800A7948, M2C_FIELD(var_s0, s16 *, 0xA));
    } else {
        var_s0 = &D_8009E9C4;
        FntPrint(&D_800A795C);
    }
    FntPrint(&D_800A796C, M2C_FIELD(var_s0, s16 *, 0));
    FntPrint(&D_800A7980, (s32) (M2C_FIELD(var_s0, u16 *, 2) << 0x10) >> 0x18);
    FntPrint(&D_800A7994, M2C_FIELD(var_s0, s16 *, 4), M2C_FIELD(var_s0, s16 *, 6), M2C_FIELD(var_s0, s16 *, 8));
    FntPrint(&D_800A79B8, M2C_FIELD(var_s0, s32 *, 0xC));
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80031D94);

s32 func_80031F18(void) {
    return D_800D2FC0;
}

void func_80031F28(void) {
    D_8009CE38 = func_800305B4(0x1800);
}

void func_80031F54(s32 arg0) {
    s16 temp_a2;
    u32 temp_s0;

    temp_a2 = M2C_FIELD(g_Args, s16 *, 0x188);
    temp_s0 = D_8009CE38;
    sprintf((s8 *) (g_Args->exePath), &D_800A7788, temp_a2, temp_a2, arg0);
    if (func_8002C168((s32 *) (g_Args->exePath), (s32) temp_s0) >= 0x1801) {
        func_8006B1DC((s32 *) (g_Args->exePath));
        func_8006B1DC(&D_800A77A0);
        exit(-1);
    }
    M2C_FIELD(temp_s0, s32 *, 0xC) = (s32) (temp_s0 + M2C_FIELD(temp_s0, s32 *, 0xC));
    M2C_FIELD(temp_s0, s32 *, 0x10) = (s32) (temp_s0 + M2C_FIELD(temp_s0, s32 *, 0x10));
    M2C_FIELD(temp_s0, s32 *, 0x14) = (s32) (temp_s0 + M2C_FIELD(temp_s0, s32 *, 0x14));
    if (M2C_FIELD(temp_s0, s32 *, 0) != 0) {
        D_8009CE3C = 1;
        return;
    }
    D_8009CE3C = 0;
}

void func_80032024(void) {
    D_800D2FC0 = func_800305B4(0x87DB0);
    D_800D2F90 = 0;
    D_800D2FAC = 0x87DB0;
    D_800D2FA4 = 0;
    D_800D2FA8 = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80032074);

void func_80032150(void) {
    s16 *var_v0;
    s32 temp_v0;

    temp_v0 = D_8009CE34 + 1;
    D_8009CE34 = temp_v0;
    if (temp_v0 >= 3) {
        D_8009CE34 = -1;
    }
    if (D_8009CE40 != NULL) {
        if (D_8009CE34 >= 0) {
            var_v0 = (D_8009CE34 * 0x10) + &D_8009E9DC;
        } else {
            var_v0 = &D_8009E9C4;
        }
        D_8009CE40(*var_v0);
    }
    func_8003237C(D_8009CE34, 0);
}

void func_800321F0(s32 arg0) {
    s32 *hdr;
    u8 *e;
    s32 i;
    u8 c[4];
    s32 *hdr2;
    s32 i2;

    if (arg0 != 0) {
        hdr = *(s32 **)(D_800D2FC0 + 4);
        if (hdr != NULL && (e = (u8 *)hdr[2]) != NULL) {
            for (i = 0; i < hdr[0]; i++, e += 0x14) {
                *(s32 *)c = (*(s32 **)(e + 4))[*(s32 *)(e + 8)];
                c[1] = ((c[0] + c[1] + c[2]) >> 3) + 0x30;
                c[0] = 0;
                c[2] = 0;
                c[3] = 0;
                *(s32 *)(e + 0xC) = *(s32 *)c;
            }
        }
        func_8006ECBC();
    } else {
        hdr2 = *(s32 **)(D_800D2FC0 + 4);
        if (hdr2 != NULL && (e = (u8 *)hdr2[2]) != NULL) {
            for (i2 = 0; i2 < hdr2[0]; i2++, e += 0x14) {
                *(s32 *)(e + 0xC) = (*(s32 **)(e + 4))[*(s32 *)(e + 8)] & 0xFFFFFF;
            }
        }
        func_8006F034();
    }
}

void func_80032348(s32 arg0) {
    if (arg0 != 0) {
        func_8006EEA8();
        return;
    }
    func_8006F034();
}

void func_8003237C(s32 arg0, s32 arg1) {
    func_8002F53C();
    func_8004783C();
    Menu_ResetState();
    if (arg0 >= 3) {
        sprintf(g_Args->exePath, D_800A77E4, arg0);
        func_8006B1DC((s32)g_Args->exePath);
        return;
    }
    M2C_FIELD(&D_8009E9C4, u8 **, 0x10) = (arg0 * 0x10) + &D_8009E9DC;
    M2C_FIELD(&D_8009E9C4, s32 *, 0x14) = arg1;
    func_8004B04C();
    D_800D2F90 = 2;
}

void func_80032424(void *arg0) {
  typeof(*((s32 *) 0)) new_var2;
  struct 
  {
    char pad[(0 > 0) ? (0) : (0)];
    typeof(*((s32 *) 0)) f;
  } *new_var;
  new_var2 = ((struct 
  {
    char pad[(4 > 0) ? (4) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f;
  new_var = (struct 
  {
    char pad[(0 > 0) ? (0) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((0 < 0) ? (0) : (0)));
  if ((new_var->f == g_Player) && (D_800D2F90 == 0))
  {
    func_80031B58(new_var2);
  }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003246C);

s32 func_800324C0(void) {
    return D_800D2F90 == 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800324D0);

void func_80032648(u32 *tim) {
    GsIMAGE img;
    RECT16 rect;

    if (*tim != 0x10) {
        printf(D_800A79F0, tim);
        exit(0);
    }
    GsGetTimInfo(tim + 1, &img);
    rect.x = img.px;
    rect.y = img.py;
    rect.w = img.pw;
    rect.h = img.ph;
    func_8007F2EC((M2C_UNK *)&rect, (M2C_UNK *)img.pixel);
    if ((img.pmode & 3) == 1) {
        func_8007F0D8(0);
        LoadClut(img.clut, img.cx, img.cy);
    } else if ((img.pmode & 3) == 0) {
        func_8007F0D8(0);
        LoadClut2(img.clut, img.cx, img.cy);
    }
    func_8007F0D8(0);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80032724);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80032768);

void func_80032880(s32 arg0, GsIMAGE *arg1) {
    u32 *p;
    if (arg1 != NULL && arg0 != 0) {
        p = (u32 *)arg0;
        if (*p != 0x10) {
            printf(D_800A79F0, arg0, arg1);
            exit(0);
        }
        p++;
        GsGetTimInfo(p, arg1);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", DevMenu_HandleInput);

void DevMenu_Draw(void) {
    s16 *p;
    u32 *p2;
    u32 *ot;
    s32 i;
    s32 buf;
    s32 *m;
    s32 *vals;
    char ***names;
    char **labels;
    char *name;

    vals = &g_DevMenu[14];
    buf = D_800B8F24;
    labels = D_8009CEE0;
    m = g_DevMenu;
    names = D_8009CEB0;
    p = (s16 *)(D_800D2FF4 + buf * 0x18);
    ot = (u32 *)D_800B8EFC[buf][1];
    p[4] = 0x14;
    p[5] = m[1] * 8 + 0x18;
    p[6] = 0x112;
    p[7] = m[1] * 8 + 0x18;
    p[8] = 0x14;
    p[9] = m[1] * 8 + 0x20;
    p[10] = 0x112;
    p[11] = m[1] * 8 + 0x20;
    *(u32 *)p = (*(u32 *)p & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((u32)p & 0xFFFFFF);
    p2 = (u32 *)(D_800D2FC4 + buf * 0x18);
    *p2 = (*p2 & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((u32)p2 & 0xFFFFFF);
    i = 0;
    do {
        if (*vals >= 0) {
            name = (*names)[*vals];
        } else {
            name = D_800A7A24;
        }
        FntPrint(D_800A7A30, *labels, name);
        i++;
        labels++;
        names++;
        vals++;
    } while (i < 12);
    FntFlush(-1);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", DevMenu_Init);

void func_80032E4C(s32 arg0, s32 arg1) {
    s16 *p;
    u32 *ot;
    s32 x;
    s32 y;

    x = arg1 * 8 + 0x24;
    y = arg0 * 8 + 0x18;
    p = (s16 *)(D_800D308C + D_800B8F24 * 0x18);
    ot = (u32 *)D_800B8EFC[D_800B8F24][1];
    p[4] = 0x14;
    p[5] = 0x18;
    p[6] = x;
    p[7] = 0x18;
    p[8] = 0x14;
    p[9] = y;
    p[10] = x;
    p[11] = y;
    *(u32 *)p = (*(u32 *)p & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((u32)p & 0xFFFFFF);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80032EFC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80033090);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800332A4);

void func_8003331C(s32 arg0) {
    if ((D_8009CF10 < arg0) || (arg0 < 0)) {
        D_8009CF10 = arg0;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80033348);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800333D4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80033420);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800334F4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800335B4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80033F38);

s32 func_80034998(void *arg0) {
    if (D_8009CF54 == 0) {
        if (g_CutScene != 0) {
            return 0;
        }
        if (func_800349F8(arg0) == 0) {
            return func_800349F8(g_Player) == 0;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800349F8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80034A48);

s32 func_80035030(void *arg0, void *arg1) {
    s32 a;
    s32 b;
    s32 c;

    a = *(s16 *)(*(u8 **)((u8 *)arg0 + 0x90) + 0x62) * 360 / 4096;
    b = *(s16 *)(*(u8 **)((u8 *)arg1 + 0x90) + 0x62) * 360 / 4096;
    if (a < 0) {
        a += 360;
    }
    c = b - (a + 180);
    if (c < 0) c = -c;
    c += 23;
    return (c / 45) % 8;
}

void func_800350F0(u8 *obj) {
    u8 *sub;
    u32 elapsed;
    u32 dur;
    s32 type;

    dur = *(u32 *)(obj + 0x58);
    sub = *(u8 **)(obj + 0x90);
    elapsed = 0;
    if (dur != 0) {
        elapsed = D_8009CDA8 - *(s32 *)(obj + 0x2C);
        if (dur < elapsed) {
            if ((*(u8 **)obj)[5] != 0) {
                func_80039A9C(0, (*(u8 **)obj)[5], 2);
            }
            if ((*(u8 **)obj)[0x5D] != 0) {
                func_80027248((*(u8 **)obj)[0x5D], (s32)(sub + 0x14), 0);
            }
            (*(u8 **)(obj + 0x10C))[0x66] = 1;
            return;
        }
    }
    if ((*(s32 *)(obj + 0x64) & 0x800000) && (D_8009CDA8 & 1)) {
        func_8006417C(*(s16 *)(sub + 0x28), *(s16 *)(sub + 0x2C), *(s16 *)(sub + 0x30), 2, 1500);
    }
    if (obj[6] == D_8009D268 && (*(s32 *)(obj + 0x68) & 0x2000) && elapsed == 1) {
        *(s16 *)(obj + 0x9C) = elapsed;
        *(s16 *)(obj + 0x98) = elapsed;
        *(s16 *)(obj + 0x9A) = elapsed;
    }
    type = obj[7] & 0x3F;
    if (type == 0x25) {
        *(s16 *)(obj + 0x9A) += 0x20;
    } else if (type == 4) {
    } else if (type == 0x2E || type == 0x2C) {
        if (*(s32 *)(obj + 0x88) != 0) {
            *(s16 *)(obj + 0x9A) += 0x10;
        }
        func_80076B9C(obj);
    } else if (type == 0x2D) {
    } else if (type == 0x28) {
        func_8006B1DC((s32)&D_800A7CB8);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800352B8);

s32 func_80035964(void *arg0) {
    s32 *pos;
    u32 dx;
    u32 dz;

    if (M2C_FIELD(arg0, s32 *, 0x48) & 0x8100) {
        return 0;
    }
    pos = (s32 *)(M2C_FIELD(arg0, u8 **, 0x90) + 0x28);
    dx = pos[0] - g_Args->playerX;
    if ((s32)dx < 0) {
        dx = g_Args->playerX - pos[0];
    }
    dz = pos[2] - g_Args->playerZ;
    if ((s32)dz < 0) {
        dz = g_Args->playerZ - pos[2];
    }
    if (dx < D_800E2DE0 && dz < D_800E2DE0) {
        return 0;
    }
    return 1;
}

s32 func_800359EC(u8 *obj) {
    s32 id;
    u8 *rec;
    s32 type;

    id = func_80035BBC(obj);
    if (id != 0) {
        rec = *(u8 **)(obj + 0x10C);
        type = obj[7] & 0x3F;
        if (type != 0x25 && type != 0x28 && type != 4 && type != 0x2E && type != 0x2D && type != 0x2C) {
            rec[0x66] = 1;
        }
        if (rec[0x64] < 5) {
            ((struct { u16 e[5][10]; } *)rec)->e[rec[0x64]][0] = *(u16 *)&D_1F8003D4;
            ((struct { u16 e[5][10]; } *)rec)->e[rec[0x64]][1] = *(u16 *)&D_1F8003D8;
            ((struct { u16 e[5][10]; } *)rec)->e[rec[0x64]][2] = *(u16 *)&D_1F8003DC;
            ((struct { u16 e[5][10]; } *)rec)->e[rec[0x64]][3] = *(u16 *)&D_1F8003E8[0];
            ((struct { u16 e[5][10]; } *)rec)->e[rec[0x64]][4] = *(u16 *)&D_1F8003E8[1];
            ((struct { u16 e[5][10]; } *)rec)->e[rec[0x64]][5] = *(u16 *)&D_1F8003E8[2];
            ((struct { s16 e[5][10]; } *)rec)->e[rec[0x64]][9] = id;
            ((struct { s32 e[5][5]; } *)rec)->e[rec[0x64]][3] = 0;
            ((struct { s16 e[5][10]; } *)rec)->e[rec[0x64]][8] = *(u16 *)(obj + 0x122) & 0xF00;
            rec[0x64]++;
            return 1;
        }
        func_8006B1DC((s32)&D_800A7D44);
    }
    return 0;
}

s32 func_80035BBC(u8 *obj) {
    s32 from[4];
    s32 to[4];
    s16 normal[4];
    s16 hit[4];
    s32 flags;
    u8 *sub;
    s32 *pos;

    sub = *(u8 **)(obj + 0x90);
    pos = (s32 *)(sub + 0x28);
    from[0] = pos[0];
    from[1] = pos[1];
    from[2] = pos[2];
    to[0] = from[0] + *(s16 *)(obj + 0x98);
    to[1] = from[1] + *(s16 *)(obj + 0x9A);
    to[2] = from[2] + *(s16 *)(obj + 0x9C);
    if (*(s16 *)(*(u8 **)(obj + 0x10C) + 0x6E) != 0) {
        from[0] -= *(s16 *)(*(u8 **)(obj + 0x10C) + 0x68);
        from[1] -= *(s16 *)(*(u8 **)(obj + 0x10C) + 0x6A);
        from[2] -= *(s16 *)(*(u8 **)(obj + 0x10C) + 0x6C);
        to[0] += *(s16 *)(obj + 0x3C);
        to[1] += *(s16 *)(obj + 0x3E);
        to[2] += *(s16 *)(obj + 0x40);
    }
    if (func_800746D0(D_800E2F2C, from, to, normal, hit, &flags) != 0) {
        D_1F8003D4 = hit[0];
        D_1F8003D8 = hit[1];
        D_1F8003DC = hit[2];
        D_1F8003E8[0] = normal[0];
        D_1F8003E8[1] = normal[1];
        D_1F8003E8[2] = normal[2];
        M2C_FIELD(obj, s16 *, 0x11C) = normal[0];
        M2C_FIELD(obj, s16 *, 0x11E) = normal[1];
        M2C_FIELD(obj, s16 *, 0x120) = normal[2];
        M2C_FIELD(obj, s16 *, 0x122) = flags;

        if (!(*(s32 *)(obj + 0x4C) & 0x2000000)) {
            *(s16 *)(obj + 0x98) = hit[0] - pos[0];
            *(s16 *)(obj + 0x9A) = hit[1] - pos[1];
            *(s16 *)(obj + 0x9C) = hit[2] - pos[2];
        }
        if (*(void **)(obj + 0x88) == g_Player && (flags & 0x8000)) {
            return 1;
        }
        return 2;
    }
    return 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80035DE8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80035F04);

void func_800361BC(void) {
    s32 obj;
    u8 *sub;

    obj = D_800AFFC8;
    sub = *(u8 **)(obj + 0x90);
    func_80052B2C(&D_800D30C0, obj);
    if (D_8009CF54 == 0 && g_Invincible == 0 && g_GodMode == 0 && g_CutScene == 0) {
        if (func_80037B6C(obj) != 0) {
            D_1F8003D8 = *(s32 *)(sub + 0x2C);
            D_1F8003D4 = 0;
            D_1F8003DC = 0;
            func_8004B270(10000, 0, 0);
            *(s32 *)(obj + 0x64) = (*(s32 *)(obj + 0x64) & 0xFFE0FFFF) | 0x70000;
            func_8005E228(obj);
            if (*(u8 *)&g_Args->flags2 >> 7) {
                func_8002F564((void *)obj, 0xFF, 4, 0, 0x14);
            }
        }
    }
    D_8009CF30 = -1;
    func_8004009C(obj);
    if (g_CutScene != 0 && D_800AFFA4 != 0) {
        D_800AFFA4 = 0;
        func_80081458(D_800B0BA0);
        D_800B0BA0 = -1;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80036328);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800365B4);

void func_80036670(u8 *obj, u8 *tgt) {
    s32 v[4];
    s32 d[4];
    s32 len;
    s32 dist;

    v[0] = *(s16 *)(obj + 0x98);
    v[1] = *(s16 *)(obj + 0x9A);
    v[2] = *(s16 *)(obj + 0x9C);
    len = SquareRoot0(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    d[0] = *(s32 *)(*(u8 **)(tgt + 0x90) + 0x28) - *(s32 *)(*(u8 **)(obj + 0x90) + 0x28);
    d[1] = *(s32 *)(*(u8 **)(tgt + 0x90) + 0x2C) - (*(s16 *)(tgt + 0x22) - *(s16 *)(tgt + 0x20)) - *(s32 *)(*(u8 **)(obj + 0x90) + 0x2C);
    if (D_8009CD4C == 0) {
        d[1] += 200;
    }
    d[2] = *(s32 *)(*(u8 **)(tgt + 0x90) + 0x30) - *(s32 *)(*(u8 **)(obj + 0x90) + 0x30);
    dist = SquareRoot0(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    D_800DDDCC[0] = *(s32 *)(*(u8 **)(obj + 0x90) + 0x28) + dist * *(s16 *)(obj + 0x98) / len;
    D_800DDDCC[1] = *(s32 *)(*(u8 **)(obj + 0x90) + 0x2C) + dist * *(s16 *)(obj + 0x9A) / len;
    D_800DDDCC[2] = *(s32 *)(*(u8 **)(obj + 0x90) + 0x30) + dist * *(s16 *)(obj + 0x9C) / len;
}

void func_800368A8(void) {
    s32 size;
    s32 i;
    s32 j;
    s8 *w;
    s32 *p;

    size = sizeof(Weapon);
    for (i = 0; i < 6; i++) {
        w = (s8 *)&g_Weapons[i];
        p = (s32 *)w;
        for (j = 0; j < size / 4; j++) {
            *p++ = 0;
        }
        *(s16 *)(w + 0x60) = 0x3C;
        for (j = 0; j < 0x35; j++) {
            w[0x74 + j] = -1;
        }
        *(s16 *)(w + 0xAE) = -1;
        *(s16 *)(w + 0xB0) = -1;
        w[0x5A] = -1;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80036938);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80036E10);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003732C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800373FC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80037768);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800377F4);

AnimSlot *func_800378FC(u8 *obj, u8 *anim, s16 arg2, s32 arg3) {
    s16 i;
    AnimSlot *s;

    s = D_800D30D8;
    for (i = 0; i < 6; i++, s++) {
        if (!(s->flags & 1)) {
            s->flags |= 1;
            s->anim = anim;
            s->unk0 = arg3;
            s->x = *(s32 *)(obj + 0x14);
            s->y = *(s32 *)(obj + 0x18);
            s->z = *(s32 *)(obj + 0x1C);
            s->start = ((u16 *)s->anim)[0] + (arg2 >> 2);
            s->end = ((u16 *)s->anim)[1] + (arg2 >> 2);
            s->time = D_8009CDA8;
            if (*(u32 *)(anim + 8) == 0) {
                func_8006B230(0, (s32)D_800A7D64);
            }
            s->frames = (u32)(s->end - s->start) / *(u32 *)(anim + 8);
            return s;
        }
    }
    return NULL;
}

void func_80037A24(void) {
    s32 i;
    AnimSlot *s;

    s = D_800D30D8;
    for (i = 0; i < 6; i++, s++) {
        if (s->flags & 1) {
            if (*(u32 *)(s->anim + 8) != 0 && *(u32 *)(s->anim + 8) < (u32)(D_8009CDA8 - s->time)) {
                s->flags &= ~1;
            } else if (s->start < s->end) {
                s->start += s->frames;
            }
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80037ACC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80037B6C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80037EB0);

void func_8003801C(void) {
    Weapon *temp_v0;
    s8 temp_a0;

    if (D_800AFFA4 != 0) {
        if (D_800B0BA0 == -1) {
            temp_v0 = &g_Weapons[D_800B0DD4];
            temp_a0 = (s8) temp_v0->pad3C[0x1E];
            if ((temp_a0 >= 0) && (((u32) temp_v0->flags >> 1) & 1)) {
                D_800B0BA0 = func_80026F30((s32) temp_a0, 0);
            }
        }
    } else if (D_800B0BA0 >= 0) {
        func_80081458(D_800B0BA0);
        D_800B0BA0 = -1;
    }
}

void func_800380DC(void *arg0, s32 arg1) {
    s32 temp_s0;
    s32 var_s1;

    var_s1 = arg1;
    if (D_8009CD4C != 0) {
        var_s1 = var_s1 >> 1;
    }
    if (var_s1 != 0) {
        temp_s0 = -(var_s1 >> 1);
        D_800D30D4 = temp_s0 + (rand() % var_s1);
        D_800D31B8 = temp_s0 + (rand() % var_s1);
    }
    M2C_FIELD(arg0, u16 *, 0x60) = (u16) (M2C_FIELD(arg0, u16 *, 0x60) + (u16) D_800D30D4);
    M2C_FIELD(arg0, u16 *, 0x62) = (u16) (M2C_FIELD(arg0, u16 *, 0x62) + (u16) D_800D31B8);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800381CC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80038404);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80038448);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003872C);

void func_800388E8(void) {
    s32 temp_a0;
    Weapon *weapon;

    weapon = &g_Weapons[D_800B0DD4];
    func_8007A17C(0x26);
    temp_a0 = ((u32) weapon->flags >> 0xF) & 0xF;
    D_8009CF48 = 0;
    if (temp_a0 > 0) {
        func_80076184(temp_a0, 0);
    }
}

void func_80038954(void) {
    if (D_800F11F0 != 0x2F) {
        func_8007A17C(0x2F);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80038984);

void func_80038AA4(void) {
    Weapon *temp_s0;
    s32 temp_v0;
    s32 var_a0;
    s8 temp_a0;
    u32 temp_a2;
    void *temp_s1;
    void *temp_s2;

    temp_s2 = g_Player;
    temp_s0 = &g_Weapons[D_800B0DD4];
    temp_s1 = D_8009CD98;
    temp_v0 = func_80039A9C((s32) temp_s2, func_80069C00() & 0xFF, 1);
    if (temp_v0 != 0) {
        M2C_FIELD(temp_v0, s32 *, 0x7C) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x7C) | 0x80000000);
        func_80038984(temp_s1, temp_v0, temp_s0);
        if (((u8) M2C_FIELD(g_Args, u8 *, 4) >> 7) != 0) {
            temp_a2 = M2C_FIELD(temp_s0, u32 *, 0x5C);
            func_8002F564(temp_s2, (temp_a2 >> 0x10) & 0x3FF, temp_a2 >> 0x1A, 0, 0);
        }
        temp_a0 = (s8) temp_s0->pad3C[0x1E];
        if (temp_a0 >= 0) {
            if (((u32) temp_s0->flags >> 1) & 1) {
                D_800AFFA4 = 1;
                D_8009DB38 = D_8009CDA8;
            } else {
                func_80026F30((s32) temp_a0, 0);
            }
        }
        var_a0 = 8;
        if ((D_800DDEE8 != 0) && ((u32) D_8009D910 >= 3U)) {
            var_a0 = 9;
        }
        func_8007A17C(var_a0);
    }
}

s32 func_80038C04(void) {
  Weapon *new_var;
  new_var = &g_Weapons[D_800B0DD4];
  return (((u32) (*new_var).flags) >> 0x14) & 1;
}

void func_80038C44(void *arg0) {
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0);
    if (M2C_FIELD(temp_v1, s16 *, 0x52) == 0) {
        M2C_FIELD(temp_v1, s16 *, 0x52) = 0x400;
    }
    M2C_FIELD(arg0, s16 *, 0x98) = 0;
    M2C_FIELD(arg0, s16 *, 0x9A) = 0;
    M2C_FIELD(arg0, s16 *, 0x9C) = 0;
    M2C_FIELD(arg0, s16 *, 0x9E) = -0x2D;
    M2C_FIELD(arg0, s16 *, 0xA0) = 0;
    M2C_FIELD(arg0, s16 *, 0xA2) = 0;
    M2C_FIELD(arg0, u8 *, 0xA4) = (u8) (M2C_FIELD(arg0, u8 *, 0xA4) | 4);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80038C8C);

void *func_800390F8(s32 arg0) {
    s32 temp_a1;
    u8 temp_s1;
    void *temp_v0;

    temp_a1 = arg0 & 0xFF;
    temp_s1 = M2C_FIELD(((temp_a1 * 0x70) + M2C_FIELD(g_Args, s32 *, 0x228)), u8 *, 1);
    temp_v0 = func_80039A9C(0, temp_a1, temp_s1);
    if (temp_v0 == NULL) {
        return NULL;
    }
    if (M2C_FIELD(temp_v0, u8 *, 5) == 4) {
        M2C_FIELD(M2C_FIELD(temp_v0, void **, 0), s8 *, 4) = 0;
        if (temp_s1 == 1) {
            M2C_FIELD(temp_v0, s16 *, 0x1C) = -0xC8;
            M2C_FIELD(temp_v0, s16 *, 0x20) = -0xC8;
            M2C_FIELD(temp_v0, s16 *, 0x24) = -0xC8;
            M2C_FIELD(temp_v0, s16 *, 0x1E) = 0xC8;
            M2C_FIELD(temp_v0, s16 *, 0x22) = 0xC8;
            M2C_FIELD(temp_v0, s16 *, 0x26) = 0xC8;
        }
        func_800393AC(temp_v0);
    }
    return temp_v0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800391B0);

void func_800393AC(void *arg0) {
    if ((arg0 != NULL) && (M2C_FIELD(M2C_FIELD(arg0, void **, 0), u8 *, 0x2D) != 1)) {
        M2C_FIELD(arg0, s32 *, 0x64) = (s32) (M2C_FIELD(arg0, s32 *, 0x64) | 0x200000);
    }
}

s32 func_800393E4(arg0)
void *arg0; {
    s16 temp_a0_3;
    s32 temp_a0;
    void *temp_a0_2;
    void *temp_v0;
    void *temp_v1;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x114);
    if (temp_a0 != 0) {
        func_800753A4(temp_a0);
        M2C_FIELD(arg0, s32 *, 0x114) = 0;
    }
    temp_v0 = M2C_FIELD(arg0, void **, 0x10C);
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, s8 *, 0x65) = 0;
        M2C_FIELD(arg0, void **, 0x10C) = NULL;
    }
    if (M2C_FIELD(arg0, u8 *, 5) == 6) {
        if (M2C_FIELD(arg0, s32 *, 0xC0) != 0) {
            temp_v1 = M2C_FIELD(arg0, void **, 0x94);
            M2C_FIELD(arg0, s32 *, 0xC0) = 0;
            if (temp_v1 != NULL) {
                *(M2C_FIELD(temp_v1, u8 *, 0xA) + D_800DE0C8) = 0;
                goto block_8;
            }
        } else {
block_8:
            temp_a0_2 = M2C_FIELD(arg0, void **, 0x94);
            if (temp_a0_2 != NULL) {
                M2C_FIELD(temp_a0_2, s32 *, 0) = (s32) (M2C_FIELD(temp_a0_2, s32 *, 0) & ~1);
                temp_a0_3 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x94), s16 *, 4);
                if (temp_a0_3 >= 0) {
                    func_80081458(temp_a0_3);
                }
                if (M2C_FIELD(arg0, s32 *, 0xC0) != 0) {
                    *(M2C_FIELD(M2C_FIELD(arg0, void **, 0x94), u8 *, 0xA) + D_800DE0C8) = 0;
                    M2C_FIELD(arg0, s32 *, 0xC0) = 0;
                }
                M2C_FIELD(arg0, void **, 0x94) = NULL;
            }
        }
    }
    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0), u8 *, 3) != 0) {
        func_80039520(arg0);
    }
    M2C_FIELD(arg0, s16 *, 0x11C) = 0;
    M2C_FIELD(arg0, s16 *, 0x11E) = 0;
    M2C_FIELD(arg0, s16 *, 0x120) = 0;
    M2C_FIELD(arg0, s16 *, 0x122) = 0;
    return 0;
}

void func_80039520(u8 *obj) {
    u32 i;
    AnimEnt *r;
    ListSet *set;

    if (obj[0xA9] >= D_8009D368) {
        printf(D_800A7DC0, D_800A7DE8, 0x14B);
        exit(1);
    }
    r = &((AnimEnt *)D_800D8FF8)[obj[0xA9]];
    if (obj == r->obj) {
        r->active = 0;
        r->obj = NULL;
        set = &D_800D8C7C[r->animIdx];
        for (i = 0; i < *(u32 *)set; i++) {
            r->enable[i] = 0;
        }
    }
}

void func_800395E8(void *arg0) {
    s32 temp_a0_3;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_4;
    void *temp_v1;

    if ((arg0 != NULL) && (M2C_FIELD(arg0, u8 *, 4) != 0)) {
        func_800393E4();
        if (D_8009D8D5 != 0) {
            if (arg0 == D_8009D8D8) {
                D_8009D8D8 = 0;
            }
        }
        if (M2C_FIELD(arg0, u8 *, 6) == D_8009D26C) {
            D_8009CABF = 0;
        }
        if (arg0 == D_8009CD64) {
            D_8009CD64 = 0;
        }
        if ((D_8009D002 == 1) && (arg0 == D_8009D004)) {
            D_8009D002 = 0;
        }
        temp_a0 = M2C_FIELD(arg0, void **, 0x8C);
        M2C_FIELD(arg0, u8 *, 4) = 0U;
        if ((temp_a0 != NULL) && ((M2C_FIELD(temp_a0, s32 *, 0x68) & 0x380) == 0x80)) {
            func_800395E8(temp_a0);
        }
        temp_v1 = M2C_FIELD(arg0, void **, 0x88);
        if (temp_v1 != NULL) {
            if (M2C_FIELD(temp_v1, s32 *, 0x8C) == arg0) {
                M2C_FIELD(temp_v1, s32 *, 0x8C) = 0;
            }
            if (M2C_FIELD(M2C_FIELD(arg0, void **, 0x88), s32 *, 0xB8) == arg0) {
                M2C_FIELD(M2C_FIELD(arg0, void **, 0x88), s32 *, 0xB8) = 0;
            }
        }
        temp_a0_2 = M2C_FIELD(arg0, void **, 0xB8);
        if (temp_a0_2 != NULL) {
            func_800395E8(temp_a0_2);
        }
        temp_a0_3 = M2C_FIELD(arg0, s32 *, 0x100);
        if (temp_a0_3 != 0) {
            func_80063ADC(temp_a0_3);
            if (M2C_FIELD(arg0, s32 *, 0x100) != 0) {
                M2C_FIELD(arg0, s32 *, 0x100) = 0;
            }
        }
        temp_a0_4 = M2C_FIELD(arg0, void **, 0x104);
        if (temp_a0_4 != NULL) {
            if (M2C_FIELD(M2C_FIELD(temp_a0_4, void **, 0x1C), s32 *, 8) == 0) {
                M2C_FIELD(temp_a0_4, s32 *, 0x10) = (s32) (M2C_FIELD(temp_a0_4, s32 *, 0x10) & ~1);
            }
            M2C_FIELD(arg0, void **, 0x104) = NULL;
        }
        if (M2C_FIELD(arg0, s32 *, 0x118) != 0) {
            func_8005414C(arg0);
        }
    }
}

void func_800397A8(void) {
  int new_var2;
  u32 temp_s2;
  u32 var_s1;
  void *var_s0;
  s16 new_var;
  var_s1 = 0;
  new_var = D_800B0DA8;
  new_var2 = D_800AFFC8 + (D_800B0DA8 * 0x124);
  temp_s2 = (D_800B07B8 - new_var) + 1;
  var_s0 = new_var2;
  if (temp_s2 != 0)
  {
    do
    {
      func_800395E8(var_s0);
 do { var_s1 += 1; var_s0 += 0x124; } while (0);
    }
    while (var_s1 < temp_s2);
  }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003982C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80039A34);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80039A9C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003A51C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003A68C);

AnimEnt *func_8003A71C(u8 **obj) {
    s32 i;
    u32 j;
    AnimEnt *r;
    AnimEnt *found;
    ListSet *set;

    if ((*obj)[3] == 0) {
        return NULL;
    }
    found = NULL;
    for (i = 0; i < D_8009D368; i++) {
        r = &((AnimEnt *)D_800D8FF8)[i];
        if (r->active == 0 && r->type == (*obj)[3]) {
            found = r;
            break;
        }
    }
    if (found != NULL) {
        found->active = 1;
        found->obj = (u8 *)obj;
        ((u8 *)obj)[0xA9] = i;
        set = &D_800D8C7C[((AnimEnt *)D_800D8FF8)[i].animIdx];
        if (*(u32 *)set >= 9) {
            printf(D_800A7DC0, D_800A7DE8, 0x5B6);
            exit(1);
        }
        for (j = 0; j < *(u32 *)set; j++) {
            found->startTime[j] = D_8009CDA8;
            found->enable[j] = 1;
            if (found->flags & 0x20) {
                set->pad8[j] = 0;
            } else {
                set->pad8[j] = 1;
            }
        }
    }
    return found;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003A898);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003AA14);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003AC1C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003B4F4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003B53C);

u8 func_8003B750(s32 *arg0) {
    s32 x, y, d, v;
    y = arg0[6];
    y *= y;
    x = arg0[5] * arg0[5] + arg0[7] * arg0[7];
    d = x + y;
    if (d < 0) return 0;
    v = 127 - (u16)SquareRoot0(d) * 127 / 64000;
    if ((s16)v >= 0) {
        if ((s16)v > 127) {
            v = 127;
        }
    } else {
        v = 0;
    }
    return v;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003B808);

void func_8003B940(u8 *obj, s16 x, s16 y, s16 z, s16 model, u8 count) {
  u8 *p;
  u8 *row;
  s32 i;
  unsigned long new_var;
  if (model != 0)
  {
    func_8004C0C0(model, obj);
  }
  if (count == 0)
  {
    obj[0xB] = *((u8 *) (&D_800D2BF0[model]));
  }
  else
  {
    obj[0xB] = count;
  }
  *((void **) (obj + 0x90)) = func_80030610(obj[0xB] * 0x8C);
  if ((*((void **) (obj + 0x90))) == ((void *) 0))
  {
    printf(&D_800A7F1C, *((s32 *) (((u8 *) g_Args) + 0x1F4)));
    printf(&D_800A7F2C, model, obj[0xB] * 0x8C);
    exit(0);
  }
  *((s32 *) (((u8 *) g_Args) + 0x1F8)) += obj[0xB];
  p = *((u8 **) (obj + 0x90));
  for (i = 0; i < obj[0xB]; p += 0x8C, i++)
  {
    new_var = i;
    count += 0;
    if ((i != 0) && (model == 0))
    {
      break;
    }
    else
      if (new_var <= 0)
    {
      func_8007A4E4((void *) 0, p + 0x10);
      *((s32 *) (p + 0x28)) = x;
      *((s32 *) (p + 0x2C)) = y;
      *((s32 *) (p + 0x30)) = z;
    }
    else
      if (obj[5] != 6)
    {
      func_8007A4E4(((*((u8 **) (obj + 0x90))) + ((((u8 *) D_800D86E0) + ((*((u8 **) obj))[0x5F] * 0x7E))[i] * 0x8C)) - 0x7C, p + 0x10);
      *((s32 *) (p + 0x28)) = ((struct 
      {
        u8 par[6];
        s16 x[6];
        s16 y[6];
        s16 z[6];
        u8 pad[0x54];
      } *) D_800D86E0)[(*((u8 **) obj))[0x5F]].x[i];
      *((s32 *) (p + 0x2C)) = ((struct 
      {
        u8 par[6];
        s16 x[6];
        s16 y[6];
        s16 z[6];
        u8 pad[0x54];
      } *) D_800D86E0)[(*((u8 **) obj))[0x5F]].y[i];
      *((s32 *) (p + 0x30)) = ((struct 
      {
        u8 par[6];
        s16 x[6];
        s16 y[6];
        s16 z[6];
        u8 pad[0x54];
      } *) D_800D86E0)[(*((u8 **) obj))[0x5F]].z[i];
    }
    *((typeof(D_800B077C) *) (p + 0x60)) = D_800B077C;
  }

  if (model != 0)
  {
    p = *((u8 **) (obj + 0x90));
    for (i = 0; i < obj[0xB]; i++, p += 0x8C)
    {
      GsLinkObject4(D_800D2D30[model], p, i);
      *((u8 **) (p + 4)) = p + 0x10;
      *((s32 *) p) = 0;
      *((s32 *) p) = *((s32 *) (obj + 0x7C));
    }

  }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003BC98);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003BE68);

void func_8003BFB8(void) {
    s32 range;
    s32 px;
    s32 pz;
    s32 ox;
    s32 oy;
    s32 oz;
    s32 i;
    s32 j;
    s32 *p;
    u32 f;
    s32 t;
    s32 x;
    s32 z;
    s32 ok;
    s32 fl;

    range = D_800E2DE0;
    px = g_Args->playerX;
    pz = g_Args->playerZ;
    ox = D_800D2FB0.x;
    oy = D_800D2FB0.y;
    oz = D_800D2FB0.z;
    for (i = 0; i < D_8009CF68; i++) {
        p = D_8009CF70[i];
        for (j = 0; j < D_8009CF6C[i]; j++, p += 6) {
            f = p[3];
            if (!((f >> 8) & 1) && ((f >> 10) & 0xF) && (t = f & 0xFF) != 0) {
                ok = 1;
                D_1F8003D4 = x = p[0] + ox;
                D_1F8003D8 = p[1] + oy;
                fl = *(s32 *)((u8 *)g_Args->unk228 + t * 0x70 + 0x3C);
                D_1F8003DC = z = p[2] + oz;
                if (!(fl & 0x100)) {
                    ok = x < px + range && x >= px - range && z < pz + range && z >= pz - range;
                }
                if (ok) {
                    func_8003C1BC(p, D_1F8003D4, D_1F8003D8, D_1F8003DC);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003C1BC);

s32 func_8003C6B0(s32 arg0, s32 arg1, s32 arg2) {
  s32 var_v0;
  var_v0 = func_80039A9C(arg0, arg1 & 0xFF, arg2 & 0xFF);
  if (var_v0 == 0)
  {
    var_v0 = 0;
  }
  if (var_v0)
  {
    return var_v0;
  }
  else
  {
    return var_v0;
  }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003C6E4);

s32 func_8003C88C(s32 arg0, s32 arg1) {
    M2C_FIELD(((arg0 << 5) + &D_800D2E84), s16 *, 0x1C) = (s16) arg1;
    return (s32) &D_800D2E84;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003C8A4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003C9C4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003CA4C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003CABC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003CC8C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003D76C);

s32 func_8003DE58(VECTOR *p1, VECTOR *p2, VECTOR *p3, VECTOR *p4) {
    D_800D40FC = p1->vx;
    D_800D410C = p1->vy;
    D_800D4100 = p2->vx;
    D_800D4110 = p2->vy;
    D_800D3FEC = D_800D4100 - D_800D40FC;
    if (D_800D3FEC < 0) {
        D_800D4070 = D_800D4100;
        D_800D4074 = D_800D40FC;
    } else {
        D_800D4074 = D_800D4100;
        D_800D4070 = D_800D40FC;
    }
    D_800D4104 = p3->vx;
    D_800D4114 = p3->vy;
    D_800D4108 = p4->vx;
    D_800D40F8 = p4->vy;
    D_800D3FD4 = D_800D4104 - D_800D4108;
    D_800D3FD8 = D_800D4114 - D_800D40F8;
    if (D_800D3FD4 > 0) {
        if (D_800D4074 < D_800D4108 || D_800D4104 < D_800D4070) {
            return 0;
        }
    } else {
        if (D_800D4074 < D_800D4104 || D_800D4108 < D_800D4070) {
            return 0;
        }
    }
    D_800D3FF0 = D_800D4110 - D_800D410C;
    if (D_800D3FF0 < 0) {
        D_800D4068 = D_800D4110;
        D_800D406C = D_800D410C;
    } else {
        D_800D406C = D_800D4110;
        D_800D4068 = D_800D410C;
    }
    if (D_800D3FD8 > 0) {
        if (D_800D406C < D_800D40F8 || D_800D4114 < D_800D4068) {
            return 0;
        }
    } else {
        if (D_800D406C < D_800D4114 || D_800D40F8 < D_800D4068) {
            return 0;
        }
    }
    D_800D3FE0 = D_800D40FC - D_800D4104;
    D_800D3FE4 = D_800D410C - D_800D4114;
    D_800D3FA8 = D_800D3FD8 * D_800D3FE0 - D_800D3FD4 * D_800D3FE4;
    D_800D3FB0 = D_800D3FF0 * D_800D3FD4 - D_800D3FEC * D_800D3FD8;
    if (D_800D3FB0 > 0) {
        if (D_800D3FA8 < 0 || D_800D3FA8 > D_800D3FB0) {
            return 0;
        }
    } else {
        if (D_800D3FA8 > 0 || D_800D3FA8 < D_800D3FB0) {
            return 0;
        }
    }
    D_800D3FAC = D_800D3FEC * D_800D3FE4 - D_800D3FF0 * D_800D3FE0;
    if (D_800D3FB0 > 0) {
        if (D_800D3FAC < 0 || D_800D3FAC > D_800D3FB0) {
            return 0;
        }
    } else {
        if (D_800D3FAC > 0 || D_800D3FAC < D_800D3FB0) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003E158);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003E494);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003E5A0);

void func_8003EA24(SVECTOR *arg0, s32 *arg1, SVECTOR *arg2) {
    SVECTOR v;
    gte_SetRotMatrix(arg1);
    gte_ldv0(arg0);
    gte_rtv0();
    gte_stsv(&v);
    arg2->vx = ((s32 (*)(s32, s32))ratan2)(-v.vy, SquareRoot0(v.vz * v.vz + v.vx * v.vx));
    arg2->vy = ((s32 (*)(s32, s32))ratan2)(v.vx, v.vz);
    arg2->vz = ((s32 (*)(s32, s32))ratan2)(-v.vy, v.vx);
}

void func_8003EB00(u8 *arg0) {
    SVECTOR v;
    u8 *dst;
    u8 *e;
    dst = M2C_FIELD(arg0, u8 **, 0x1C);
    if (M2C_FIELD(arg0, u8 **, 0x20) == NULL) {
        func_8006B230(0, (s32)D_800A80A4);
    }
    e = M2C_FIELD(M2C_FIELD(arg0, u8 **, 0x20), u8 **, 0x90) + M2C_FIELD(arg0, s16 *, 0x2A) * 0x8C;
    GsGetLs(e + 0x10, D_800D40D8);
    gte_SetRotMatrix(e + 0x34);
    gte_ldv0(arg0 + 0x24);
    gte_rtv0();
    gte_stsv(&v);
    M2C_FIELD(dst, s32 *, 0x28) = v.vx + M2C_FIELD(e, s32 *, 0x48);
    M2C_FIELD(dst, s32 *, 0x2C) = v.vy + M2C_FIELD(e, s32 *, 0x4C);
    M2C_FIELD(dst, s32 *, 0x30) = v.vz + M2C_FIELD(e, s32 *, 0x50);
    func_8003EA24((SVECTOR *)(arg0 + 0x2C), (s32 *)(e + 0x34), &v);
    M2C_FIELD(dst, s16 *, 0x60) = (v.vx + 0x800) & 0xFFF;
    M2C_FIELD(dst, s16 *, 0x62) = v.vy & 0xFFF;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003EC44);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003F768);

void func_8003FC60(void) {
    s16 i;
    u8 *p;

    if (D_8009CF7C != 0) {
        i = 0;
        p = (u8 *)D_8009CF7C;
        for (; i < D_8009CF80; i++, p += 0x44) {
            *p = 0;
            *(s32 *)(p + 0x1C) = 0;
            *(s32 *)(p + 0x28) = 0;
            *(s32 *)(p + 0x2C) = 0;
            p[3] = 0;
        }
        func_80058DFC(&D_800DDA40);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003FCE4);

void func_8003FF9C(void) {
    s16 i;
    u8 *p;
    s16 dx;
    s16 dz;
    s32 *q;
    s32 minX;
    s32 minZ;
    s32 maxX;
    s32 maxZ;

    i = 0;
    if (D_8009CF7C != 0) {
        p = (u8 *)D_8009CF7C;
        maxZ = 0x7D00;
        minZ = -0x7D00;
        minX = minZ;
        maxX = maxZ;
        for (; i < D_8009CF80; i++, p += 0x44) {
            if (*p != 0) {
                dx = *(s16 *)(p + 0x10) - g_Args->playerX;
                dz = *(s16 *)(p + 0x18) - g_Args->playerZ;
                if (dx < minX || dz < minZ || dx > maxX || dz > maxZ) {
                    *p = 0;
                    q = *(s32 **)(p + 0x1C);
                    if (q != NULL) {
                        *q &= ~0x100;
                    }
                    *(s32 *)(p + 0x28) = 0;
                    *(s32 *)(p + 0x2C) = 0;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004009C);

void func_800406A0(void) {
    if (D_8009CF7C != 0) {
        func_80059C14(&D_800DDA40);
        func_80059E4C(&D_800DDA40);
    }
}

void func_800406E0(s32 *arg0) {
    u8 *p;
    s32 first;
    s32 i;

    if (D_8009CF7C != 0) {
        p = (u8 *)D_8009CF7C;
        first = -1;
        for (i = 0; i < D_8009CF80; i++, p += 0x44) {
            if (*p != 0) {
                if (first == -1) {
                    first = i;
                }
                if (*arg0 < i) {
                    *arg0 = i;
                    return;
                }
            }
        }
        if (first != -1) {
            *arg0 = first;
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80040778);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80040978);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80040AA8);

void DebugPage_Profile(void) {
    g_DebugTextX = 0x11;
    g_DebugTextY = 1;
    FntPrint(&D_800A81D0);
}

void DebugPage_ArtProfile(void) {
    g_DebugTextX = 0x11;
    g_DebugTextY = 1;
    FntPrint(&D_800A81D0);
}

void DebugPage_Framerate(void) {
    g_DebugTextX = 0x12;
    g_DebugTextY = 1;
    FntPrint(&D_800A81E0, g_FrameRate, g_FrameCap);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", DebugPage_ObjectCount);

void DebugPage_Camera(void) {
    long h;

    g_DebugTextX = 0x2F;
    g_DebugTextY = 0x11;
    gte_ReadGeomScreen(&h);
    FntPrint(D_800A828C, g_Camera.targetX, g_Camera.posX, g_Args->playerX);
    FntPrint(D_800A82B4, g_Camera.targetY, g_Camera.posY, g_Args->playerY);
    FntPrint(D_800A82DC, g_Camera.targetZ, g_Camera.posZ, g_Args->playerZ);
    FntPrint(D_800A8304);
    FntPrint(D_800A8308, g_Camera.rotX);
    FntPrint(D_800A8310, g_Camera.rotY);
    FntPrint(D_800A8318, g_Camera.rotZ);
    FntPrint(D_800A8320, gPsi);
    FntPrint(D_800A8304);
    FntPrint(D_800A832C, (u16)h);
    FntPrint(D_800A8340, g_CutScene);
}

void func_8004105C(void) {
    if ((func_8004A370() == 0) && ((func_8004B06C() << 0x10) == 0) && ((func_8004B084() << 0x10) == 0) && ((D_800B0E58 == 0) || ((g_PadHeld & 0x800) && !(g_PadPrev & 0x800))) && !(g_PadHeld & 0x100)) {
        if (D_8009CA9C != 0) {
            D_8009CA9C = 0;
            return;
        }
        if ((D_800B0E58 == 0) && (D_8009CD4C != 0)) {
            Camera_ResetZoom();
        }
        if ((D_8009CAA4 == 0) && (g_CutScene == 0) && (func_800794D4() != 0) && (func_80053BD0() != 0)) {
            func_8004783C();
            Menu_ResetState();
            D_8009CAA4 = 1;
            D_8009CFC8 = 0;
            func_80026C90(-1);
            func_80069850(0);
            func_80026F30(1, 0);
            func_8007A17C(0x1D);
        }
    }
}

void func_800411BC(s32 *x, s32 *y, s16 step) {
    __typeof__(D_800A8370) names = D_800A8370;
    __typeof__(D_800A8390) buttons = D_800A8390;
    s16 i;
    s16 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (g_Args->buttonMap[i] == buttons.v[j]) {
                func_8002872C((M2C_UNK *)names.s[j], *x, *y);
                *y += step;
            }
        }
    }
}

void func_8004133C(void) {
    D_8009CAA4 = 1;
    func_8004783C();
    Menu_ResetState();
    D_8009CFC4 = 2;
    func_8007A17C(0x1D);
}

void func_80041380(void) {
    D_8009CFC8 = 1;
    D_800B0794 = D_8009CFC0 - 1;
    func_80041C28();
    func_8002F53C();
    D_8009CA9C = 1;
    func_80026C90(-1);
    func_80069850(0);
    func_80026F30(1, 0);
    D_8009CAC4 = 1;
}

void func_800413F4(void) {
    D_8009CAA4 = 1;
    PauseMenu_Open();
    if (D_800B0B9C == 0x63) {
        D_8009CAA4 = 3;
    }
    if (D_8009CA90 != 0) {
        D_8009CAA4 = 0;
        return;
    }
    func_8002F53C();
    if (M2C_FIELD(&D_800AC284, u8 *, 0x108) != 0) {
        if (M2C_FIELD(&D_800AC284, s32 *, 0x104) != 1) {
            if (M2C_FIELD(&D_800AC284, s32 *, 0x104) == 0) {
                M2C_FIELD(&D_800AC284, u8 *, 0x108) = 0U;
            }
            goto block_9;
        }
    } else {
block_9:
        D_8009CA9C = 1;
        func_80026F30(1, 0);
    }
}

void func_800414AC(void) {
    if (D_800AFFD0 == 0) {
        func_80026B00(M2C_FIELD(g_Args, u8 *, 0x142));
    }
    func_8007F0D8(0);
    D_800B0AE0 = 0;
    D_800B07C4 = 0;
    D_800AFFA8 = 0;
    D_800B0070 = 0;
    D_800AFFCC = 0;
    Menu_ResetState();
    if (((u32) M2C_FIELD(g_Args, u32 *, 0) >> 6) & 1) {
        D_800AC38C = 0;
        M2C_FIELD(g_Args, s8 *, 0x115) = 1;
    }
    D_8009CA9C = 0;
    g_MenuUpdateFunc.update = NULL;
    if (g_CutScene == 7) {
        func_80061708(0);
    }
}

void func_80041578(void) {
    if (g_MenuUpdateFunc.update != NULL) {
        g_MenuUpdateFunc.update();
        D_8009CDC8 = M2C_FIELD(g_Args, s32 *, 0x2DC);
    }
}

void PauseMenu_Open(void) {
    s32 y;

    g_MenuUpdateFunc.update = func_800417C4;
    Cheat_ResetCodes();
    Menu_ResetState();
    D_8009CFBE = 0;
    D_800B0794 = 0;
    D_8009CFBE = 1;
    g_MenuUpdateFunc.cursor = 0;
    func_80028810(D_800A83B0, 0x37);
    g_MenuUpdateFunc.inventoryItem = D_8009CFBE++;
    func_80028810(D_800A83BC, 0x49);
    g_MenuUpdateFunc.objectivesItem = D_8009CFBE++;
    func_80028810(&D_800A83C8, 0x5B);
    g_MenuUpdateFunc.messagesItem = D_8009CFBE++;
    func_80028810(D_800A83D4, 0x6D);
    g_MenuUpdateFunc.optionsItem = D_8009CFBE++;
    func_80028810(D_800A83E0, 0x7F);
    g_MenuUpdateFunc.soundItem = D_8009CFBE++;
    func_80028810(D_800A83E8, 0x91);
    y = 0xA3;
    if (D_8009CDD0 != 0) {
        g_MenuUpdateFunc.restartItem = D_8009CFBE++;
        func_80028810(D_800A83F0, 0xA3);
        y = 0xB5;
    } else {
        g_MenuUpdateFunc.restartItem = -1;
    }
    g_MenuUpdateFunc.abortItem = D_8009CFBE++;
    func_80028810(D_800A8400, y);
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    func_8002872C(&D_800A8418, 0x19A, 0xD7);
    func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
    D_800B0AE0 = 1;
    D_8009CAC4 = 1;
}

void func_800417C4(void) {
    s32 sel;
    Args *args;

    sel = func_80043B14(0, D_8009CFBE);
    if (g_PadHeld != 0 && ((g_PadHeld & 0x800) && !(g_PadPrev & 0x800) || Cheat_CheckCodes() != 0)) {
        func_80026F30(1, 0);
        func_800414AC();
        return;
    }
    if (sel == 3) {
        if (D_800B0794 == g_MenuUpdateFunc.cursor) {
            func_80026F30(1, 0);
            func_800414AC();
        } else if (D_800B0794 == g_MenuUpdateFunc.objectivesItem) {
            func_80026F30(1, 0);
            func_80042A94();
        } else if (D_800B0794 == g_MenuUpdateFunc.inventoryItem) {
            func_80026F30(1, 0);
            func_80042E34();
        } else if (D_800B0794 == g_MenuUpdateFunc.messagesItem) {
            func_80026F30(1, 0);
            func_80041A38();
        } else if (D_800B0794 == g_MenuUpdateFunc.soundItem) {
            func_80026F30(1, 0);
            func_800433F4();
        } else if (D_800B0794 == g_MenuUpdateFunc.optionsItem) {
            func_80026F30(1, 0);
            func_8004218C();
        } else if (D_800B0794 == g_MenuUpdateFunc.restartItem) {
            func_80026F30(1, 0);
            RestartPrompt_Open();
        } else if (D_800B0794 == g_MenuUpdateFunc.abortItem) {
            func_80026F30(1, 0);
            args = g_Args;
            args->unk115 = 1;
            args->flags |= 0x80;
            RestartPrompt_Open();
        }
    } else if (sel == 1) {
        func_80026F30(1, 0);
        func_800414AC();
    }
}

void func_800419CC(void *arg0) {
    s32 temp_a0;
    void *var_s0;

    var_s0 = arg0;
    if (var_s0 == NULL) {
        var_s0 = D_800D4468;
    }
    temp_a0 = M2C_FIELD(var_s0, s32 *, 8);
    if (temp_a0 >= 0) {
        func_8002B064(temp_a0);
    }
    func_80029868(M2C_FIELD(var_s0, s32 *, 4), 0x17, 0x41, 0x1D1, 0x90, 1);
}

void func_80041A38(void) {
    s32 y;
    s32 i;
    s32 x;

    Menu_ResetState();
    y = 0x49;
    i = 0;
    D_8009CFBE = 0;
    D_800B0794 = 0;
    x = 0x4B;
    for (; i < *(s32 *)&D_8009CFC0; i++) {
        D_8009CFBE++;
        func_8002872C((M2C_UNK *)D_800D4444[i * 3], x, y);
        y += 0x12;
    }
    if (D_8009CFBE == 0) {
        func_80028810((M2C_UNK *)D_800A8420, y);
    }
    func_8002872C((M2C_UNK *)D_800A83D4, 0x32, 0x37);
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    if (D_8009CFBE != 0) {
        func_8002872C(&D_800A8418, 0x19A, 0xD7);
    }
    g_MenuUpdateFunc.update = func_80041BA0;
    if (D_8009CFBE > 0) {
        D_800B0794 = D_8009CFBE - 1;
        func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80041BA0);

void func_80041C28(void) {
    char **e;
    char **base;

    D_8009CFB4 = 1;
    base = D_800D4444;
    e = &base[D_800B0794 * 3];
    if ((s32)e[2] == M2C_FIELD(D_800D4468, s32 *, 8)) {
        D_8009CFC4 = 0;
    }
    func_800419CC(e);
    func_8002872C((M2C_UNK *)base[D_800B0794 * 3], 0x19, 0x33);
    D_8009CFBE = 0;
    D_800B0794 = 0;
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    g_MenuUpdateFunc.update = func_80041CF0;
}

void func_80041CF0(void) {
    if (func_80043B14(0, D_8009CFBE) == 1) {
        func_80026F30(1, 0);
        func_80082F24(0, 0);
        func_8002B0A0();
        func_8004783C();
        Menu_ResetState();
        D_8009CFB4 = 0;
        if (D_8009CFC8 != 0) {
            func_800414AC();
        } else {
            g_MenuUpdateFunc.update = (void (*)(void))func_80041A38;
        }
    }
    if (((u32) g_Args->flags >> 3) & 1) {
        func_8002B69C();
    }
}

void func_80041DBC(void) {
    Args *args;

    Menu_ResetState();
    D_8009CFBE = 0;
    D_800B0794 = 0;
    func_8002872C((M2C_UNK *)D_800A842C, 0x6F, 0x52);
    func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
    g_MenuUpdateFunc.update = func_80041E78;
    args = g_Args;
    D_800D4410 = 0;
    args->flags |= 0x40;
    if (args->unk115 == 0) {
        func_8002872C(&D_800A8410, 0x19, 0xD7);
        func_8002872C(&D_800A8418, 0x19A, 0xD7);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80041E78);

void RestartPrompt_Open(void) {
    Menu_ResetState();
    D_8009CFBE = 0;
    D_800B0794 = 0;
    D_8009CFBE = 1;
    do { } while (0);
    if (*(u8 *)g_Args >> 7) {
        func_8002872C(D_800A8444, 0x6F, 0x52);
    } else {
        func_8002872C(D_800A844C, 0x6F, 0x52);
    }
    func_8002872C(D_800A8458, 0x169, 0x52);
    D_800D4454 = 0;
    func_800289C0(D_8009CFBE, D_800A8460);
    func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
    func_80028B58(D_8009CFBE, D_800AEDA0, D_800AEDA4);
    g_MenuUpdateFunc.update = func_80042020;
    if (g_Args->unk115 == 0 || (*(u8 *)g_Args >> 7)) {
        func_8002872C(&D_800A8410, 0x19, 0xD7);
        func_8002872C(&D_800A8418, 0x19A, 0xD7);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80042020);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004218C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800425E4);

void func_80042A94(void) {
    Menu_ResetState();
    func_800286EC(0);
    D_8009CFBE = 0;
    D_800B0794 = 0;
    func_80042B28();
    func_8002872C(&D_800A83C8, 0x32, 0x37);
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    if (D_8009CA8C != 0) {
        func_8002872C(&D_800A8418, 0x19A, 0xD7);
    }
    g_MenuUpdateFunc.update = (void (*)(void))&func_80042D1C;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80042B28);

void func_80042D1C(void) {
    s32 sel;

    sel = func_80043B14(0, D_8009CFBE);
    if (sel == 1) {
        func_80026F30(1, 0);
        func_800286EC(0);
        PauseMenu_Open();
    } else if (D_8009CA8C != 0) {
        if (sel == 2) {
            func_80028B58(D_800B0798 + D_8009CFBE, (s16)D_800AC4D8, (s16)D_800AC4DC);
            func_80028B58(D_800B0794 + D_8009CFBE, D_800AEDA0, D_800AEDA4);
        } else if (sel == 3) {
            func_80026F30(2, 0);
            func_8007F0D8(0);
            func_8006981C(D_800D4474[D_800B0794]);
            Menu_ResetState();
            func_80042B28();
        }
    }
}

void func_80042E34(void) {
    Menu_ResetState();
    D_800B0794 = 0;
    func_80042E6C();
    g_MenuUpdateFunc.update = (void (*)(void))&func_80043078;
}

void func_80042E6C(void) {
    s32 i;
    s32 count;
    s32 x;
    s32 y;
    s32 step;

    step = 0xC;
    i = 0;
    x = 0x64;
    y = 0x3A;
    count = 0;
    for (; i < 6; i++) {
        if (((u32)g_Weapons[i].flags >> 11) & 1) {
            D_800D446C[count++] = i;
            func_8002872C((M2C_UNK *)&g_Weapons[i], x, y);
            y += step;
            func_80028B58(D_800AC54C - 1, (s16)D_800AC4D8, (s16)D_800AC4DC);
        }
    }
    x = 0x50;
    y = 0x3A;
    for (i = 0; i < count; i++) {
        if (((u32)g_Weapons[D_800D446C[i]].flags >> 12) & 1) {
            func_8002872C((M2C_UNK *)D_800A84BC, x, y);
        } else {
            func_8002872C((M2C_UNK *)D_800A84C0, x, y);
        }
        y += step;
        func_80028B58(D_800AC54C - 1, (s16)D_800AC4D8, (s16)D_800AC4DC);
    }
    D_8009CFBE = count;
    func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
    func_80028B58(D_800B0794 + count, D_800AEDA0, D_800AEDA4);
    func_8002872C((M2C_UNK *)D_800A83BC, 0x32, 0x2E);
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    func_8002872C(&D_800A8418, 0x19A, 0xD7);
    func_8002872C((M2C_UNK *)D_800A84C4, 0xC8, 0xC3);
}

void func_80043078(void) {
    s32 sel;

    sel = func_80043B14(0, D_8009CFBE);
    if (sel == 1) {
        func_80026F30(1, 0);
        PauseMenu_Open();
    } else if (sel == 2) {
        func_80028B58(D_800B0798 + D_8009CFBE, (s16)D_800AC4D8, (s16)D_800AC4DC);
        func_80028B58(D_800B0794 + D_8009CFBE, D_800AEDA0, D_800AEDA4);
    } else if (sel == 3) {
        func_80026F30(1, 0);
        D_8009EEC4 = D_800D446C[D_800B0794];
        func_800414AC();
    } else if (g_PadPressed & 0x80) {
        func_80026F30(2, 0);
        func_8007F0D8(0);
        g_Weapons[D_800D446C[D_800B0794]].flags = (g_Weapons[D_800D446C[D_800B0794]].flags & ~0x1000) | (((((u32)g_Weapons[D_800D446C[D_800B0794]].flags >> 12) & 1) ^ 1) << 12);
        Menu_ResetState();
        func_80042E6C();
    }
}

void func_80043200(void) {
    s32 x;
    s32 y;

    Menu_ResetState();
    D_8009CFBE = 0;
    D_800B0794 = 0;
    x = 0x32;
    y = 0x4B;
    func_800411BC(&x, &y, 0xE);
    func_8002872C((M2C_UNK *)D_800A84D0, x, y);
    y = 0x4B;
    x += 0x96;
    func_8002872C((M2C_UNK *)D_800A84D8, x, y);
    y += 0xE;
    func_8002872C((M2C_UNK *)D_800A84E4, x, y);
    y += 0xE;
    func_8002872C((M2C_UNK *)D_800A84F0, x, y);
    y += 0xE;
    func_8002872C((M2C_UNK *)D_800A84FC, x, y);
    y += 0xE;
    func_8002872C((M2C_UNK *)D_800A850C, x, y);
    y += 0xE;
    func_8002872C((M2C_UNK *)D_800A851C, x, y);
    y += 0xE;
    func_8002872C((M2C_UNK *)D_800A8528, x, y);
    y += 0xE;
    func_8002872C((M2C_UNK *)D_800A8534, x, y);
    y += 0xE;
    func_8002872C((M2C_UNK *)D_800A8544, x, y);
    y += 0x14;
    func_8002872C((M2C_UNK *)D_800A8484, 0x32, 0x37);
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    g_MenuUpdateFunc.update = func_800433A8;
}

void func_800433A8(void) {
    if (func_80043B14(0, D_8009CFBE) == 1) {
        func_80026F30(1, 0);
        func_8004218C();
    }
}

void func_800433F4(void) {
    char buf[24];
    s32 y;

    D_8009CFB8 = D_8009CFB8 < 0 ? 0 : (D_8009CFB8 > 0x68 ? 0x68 : D_8009CFB8);
    y = 0x50;
    D_800D4460 = M2C_FIELD(g_Args, u8 **, 0x130)[D_8009CFB8 * 8 + 2];
    Menu_ResetState();
    D_8009CFBE = 0;
    D_800B0794 = 0;
    D_8009CFBE = 1;
    D_800D43C4 = 0;
    func_8002872C((M2C_UNK *)D_800A8558, 0x64, y);
    D_800D439C = y;
    y = 0x69;
    D_800D4404 = D_8009CFBE++;
    func_8002872C((M2C_UNK *)D_800A8564, 0x64, y);
    D_800D43A0 = y;
    D_800D440C = D_800AC54C;
    func_8002872C((M2C_UNK *)D_800A8574, 0x15E, D_800D439C);
    sprintf(buf, D_800A857C, M2C_FIELD(g_Args, s16 *, 0x140));
    func_800289C0((s16)D_800D440C, buf);
    D_800D4414 = D_800AC54C;
    func_8002872C((M2C_UNK *)D_800A8574, 0x15E, D_800D43A0);
    sprintf(buf, D_800A857C, M2C_FIELD(g_Args, s16 *, 0x13E));
    func_800289C0((s16)D_800D4414, buf);
    func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
    func_80028B58(D_800D440C, D_800AEDA0, D_800AEDA4);
    func_8002872C((M2C_UNK *)D_800A83E8, 0x32, 0x37);
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    g_MenuUpdateFunc.update = func_80043610;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80043610);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80043B14);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80043EE0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", Pad_StickToSpeed);

void func_80044898(void) {
    D_8009CD54 = 1;
    D_8009CFB0 = 0;
    *(u8 *)&D_8009D3A4 = 0;
    M2C_FIELD(g_Player, s32 *, 0x5C) = 0;
    if (D_8009DB3C == 0 || M2C_FIELD(g_Player, s16 *, 0x52) == 0) {
        gPsi = D_800B0C5C;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800448F4);

void func_800449BC(void) {
    Weapon *w;
    s32 t;

    w = &g_Weapons[D_800B0DD4];
    t = D_800B0DD4 == 3;
    if (M2C_FIELD(w, s16 *, 0x54) >= M2C_FIELD(w, s16 *, 0x56) && !t) {
        if (M2C_FIELD(w, s16 *, 0x68) > D_800D43F0) {
            M2C_FIELD(w, s16 *, 0x68) -= D_800D43AC;
        }
        func_8007A040(D_800B0DD4);
        D_8009CFCC = 0;
    }
}

void func_80044A60(void) {
    Weapon *w;
    s32 t;

    w = &g_Weapons[D_800B0DD4];
    t = D_800B0DD4 == 3;
    if (M2C_FIELD(w, s16 *, 0x54) >= M2C_FIELD(w, s16 *, 0x56) && !t) {
        if (M2C_FIELD(w, s16 *, 0x68) <= D_800D43F4) {
            M2C_FIELD(w, s16 *, 0x68) = D_800D43F4;
        }
        func_8007A040(D_800B0DD4);
    }
}

s16 func_80044AF4(void) {
    return D_8009CAA4;
}

void func_80044B04(void *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    M2C_FIELD(arg0, s16 *, 8) = arg1;
    M2C_FIELD(arg0, s16 *, 0x10) = arg3;
    M2C_FIELD(arg0, s16 *, 0xA) = arg2;
    M2C_FIELD(arg0, s16 *, 0x18) = (s16) arg5;
    M2C_FIELD(arg0, s16 *, 0x20) = (s16) arg7;
    M2C_FIELD(arg0, s16 *, 0x12) = (s16) arg4;
    M2C_FIELD(arg0, s16 *, 0x1A) = (s16) arg6;
    M2C_FIELD(arg0, s16 *, 0x22) = (s16) arg8;
}

void func_80044B3C(void *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    M2C_FIELD(arg0, s16 *, 8) = arg1;
    M2C_FIELD(arg0, s16 *, 0xC) = arg3;
    M2C_FIELD(arg0, s16 *, 0xA) = arg2;
    M2C_FIELD(arg0, s16 *, 0x10) = (s16) arg5;
    M2C_FIELD(arg0, s16 *, 0x14) = (s16) arg7;
    M2C_FIELD(arg0, s16 *, 0xE) = (s16) arg4;
    M2C_FIELD(arg0, s16 *, 0x12) = (s16) arg6;
    M2C_FIELD(arg0, s16 *, 0x16) = (s16) arg8;
}

void func_80044B74(void *arg0, s8 arg1, s8 arg2, s8 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    M2C_FIELD(arg0, s8 *, 0xC) = arg1;
    M2C_FIELD(arg0, s8 *, 0x14) = arg3;
    M2C_FIELD(arg0, s8 *, 0xD) = arg2;
    M2C_FIELD(arg0, s8 *, 0x1C) = (s8) arg5;
    M2C_FIELD(arg0, s8 *, 0x24) = (s8) arg7;
    M2C_FIELD(arg0, s8 *, 0x15) = (s8) arg4;
    M2C_FIELD(arg0, s8 *, 0x1D) = (s8) arg6;
    M2C_FIELD(arg0, s8 *, 0x25) = (s8) arg8;
}

void func_80044BAC(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    M2C_FIELD(arg0, s8 *, 4) = arg1;
    M2C_FIELD(arg0, s8 *, 5) = arg2;
    M2C_FIELD(arg0, s8 *, 6) = arg3;
}

void func_80044BBC(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    M2C_FIELD(arg0, s8 *, 4) = arg1;
    M2C_FIELD(arg0, s8 *, 5) = arg2;
    M2C_FIELD(arg0, s8 *, 6) = arg3;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80044BCC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80045084);

void func_80045134(void) {
    D_800D5F40 |= 0x80000000;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004514C);

s32 func_80045344(POLY_FT4 *p, s32 u, s32 v, s32 x, s32 y, s32 uw, s32 w, s32 h, s32 tpx, s32 tpy, s32 clx, s32 cly) {
    s32 tp;

    ((u8 *)p)[3] = 9;
    p->code = 0x2C;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->u0 = u;
    p->v0 = v;
    p->u1 = u + (uw - 1);
    p->v1 = v;
    p->u2 = u;
    p->v2 = v + h;
    p->u3 = u + (uw - 1);
    p->v3 = v + h;
    p->x0 = x;
    p->y0 = y;
    p->x1 = x + w;
    p->y1 = y;
    p->x2 = x;
    p->y2 = y + h;
    p->x3 = x + w;
    p->y3 = y + h;
    p->tpage = tp = ((tpy & 0x100) >> 4) | ((tpx & 0x3FF) >> 6) | ((tpy & 0x200) << 2);
    p->clut = (cly << 6) | ((clx >> 4) & 0x3F);
    p->code |= 2;
    return tp;
}

void func_80045420(void) {
    M2C_UNK *var_s2;
    M2C_UNK *var_s3;
    M2C_UNK *var_s4;
    s32 var_s1;

    var_s4 = &D_800D5E80;
    var_s3 = &D_800D8674;
    var_s2 = &D_800D8624;
    var_s1 = 0;
    do {
        func_80045344(var_s3, 0xE0, 0xD0, 0x164, 0x19, 0x18, 0x18, 0x18, 0x338, 0x1D0, 0x250, 0x1C5);
        var_s3 += 0xA;
        func_80045344(var_s4, 0xF8, 0xE0, 0x17C, 0x19, 4, 0x28, 0x18, 0x33E, 0x1E0, 0x230, 0x1C5);
        var_s4 += 0xA;
        func_80045344(var_s2, 0x80, 0x80, 0x1A4, 0x19, 0x34, 0x34, 0x18, 0x260, 0x180, 0x260, 0x1C5);
        var_s1 += 1;
        var_s2 += 0xA;
    } while (var_s1 < 2);
}

void func_8004557C(void) {
    M2C_UNK *var_s1;
    M2C_UNK *var_s2;
    M2C_UNK *var_s3;
    s32 var_s0;

    var_s3 = &D_800D7500;
    var_s2 = &D_800D78D8;
    var_s1 = &D_800D7888;
    var_s0 = 0;
    do {
        func_80045344(var_s2, 0xE0, 0xC0, 0, 0, 0x18, 0, 0x10, 0x338, 0x1C0, 0x290, 0x1C5);
        var_s2 += 0xA;
        func_80045344(var_s3, 0xF8, 0xCE, 0, 0, 4, 0, 0x10, 0x33E, 0x1CE, 0x270, 0x1C5);
        var_s3 += 0xA;
        func_80045344(var_s1, 0xE0, 0xE8, 0, 0, 0x18, 0, 0x10, 0x338, 0x1E8, 0x280, 0x1C5);
        var_s0 += 1;
        var_s1 += 0xA;
    } while (var_s0 < 2);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800456D0);

void func_800458E8(void) {
    M2C_UNK *var_s1;
    s32 var_s0;

    var_s1 = &D_800D7B08;
    var_s0 = 0;
    do {
        func_80045344(var_s1, 0, 0xAC, 0x180, 0x2D, 0x60, 0x60, 0x14, 0x2C0, 0x1AC, 0x210, 0x1FF);
        var_s0 += 1;
        var_s1 += 0xA;
    } while (var_s0 < 2);
}

void func_80045988(void) {
    M2C_UNK *var_s1;
    s32 var_s0;

    var_s1 = &D_800D7CB8;
    var_s0 = 0;
    do {
        func_80045344(var_s1, 0x80, 0x98, 0x1E, 0xBF, 0x30, 0x30, 0x18, 0x260, 0x198, 0x2B0, 0x1C5);
        var_s0 += 1;
        var_s1 += 0xA;
    } while (var_s0 < 2);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80045A28);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004626C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800463C8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80047318);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800474AC);

void func_800475BC(void) {
    s16 x;
    s16 w;
    s16 i;
    s32 len;

    if (D_800AC38C == 0) {
        if (D_800D7CB4 != 0 && D_8009EED0 == 0) {
            if (((Msg *)&D_800D5DA4)->flag != 0) {
                len = strlen(((Msg *)&D_800D5DA4)->text);
                w = len;
                w = len * D_800AEDD4 + w * 17 + 18;
                func_800299CC((s32)&D_800D5DA4, 0x1E, 0xAC, w, 0xF, 0, ((Msg *)&D_800D5DA4)->x, ((Msg *)&D_800D5DA4)->y);
            } else {
                func_80028858(((Msg *)&D_800D5DA4)->text, &x, &w);
                x -= 9;
                w += 18;
                func_80029868((s32)&D_800D5DA4, x, 0xAC, w, 0xF, 0);
            }
            func_800477C0(&D_800D4958, &D_800D5DA4);
            for (i = 1; i < D_800D7CB4; i++) {
                func_800477C0(&((Msg *)&D_800D5DA4)[i - 1], &((Msg *)&D_800D5DA4)[i]);
            }
            D_800D7CB4--;
            func_8004782C(&((Msg *)&D_800D5DA4)[D_800D7CB4]);
        } else {
            func_8004782C(&D_800D4958);
        }
    }
}

void func_800477C0(void *arg0, void *arg1) {
    strcpy(arg0, arg1);
    M2C_FIELD(arg0, u8 *, 0x23) = (u8) M2C_FIELD(arg1, u8 *, 0x23);
    M2C_FIELD(arg0, u16 *, 0x24) = (u16) M2C_FIELD(arg1, u16 *, 0x24);
    M2C_FIELD(arg0, u8 *, 0x26) = (u8) M2C_FIELD(arg1, u8 *, 0x26);
    M2C_FIELD(arg0, u16 *, 0x28) = (u16) M2C_FIELD(arg1, u16 *, 0x28);
    M2C_FIELD(arg0, u16 *, 0x2A) = (u16) M2C_FIELD(arg1, u16 *, 0x2A);
}

void func_8004782C(void *arg0) {
    M2C_FIELD(arg0, s8 *, 0) = 0;
    M2C_FIELD(arg0, s8 *, 0x23) = 0;
    M2C_FIELD(arg0, s8 *, 0x26) = 0;
}

void func_8004783C(void) {
    s16 temp_v0;
    s16 var_s0;

    D_800AC38C = 0;
    D_800D7CB4 = 0;
    func_8004782C(&D_800D4958);
    var_s0 = 0;
    do {
        func_8004782C((var_s0 * 0x2C) + &D_800D5DA4);
        temp_v0 = var_s0 + 1;
        var_s0 = temp_v0;
    } while (temp_v0 < 5);
}

void func_800478C8(void) {
  s8 *new_var;
  func_80045988();
  func_800458E8();
  func_8004557C();
  D_800D7928 = 0x87;
  D_800D792C = 0x1B;
  D_800D7BEC = 0x230;
  new_var = (s8 *) g_Args;
  D_800D7BE8 = 0x230;
  D_800D7BF0 = 0x1E0;
  D_800D7AB0 = (*((u16 *) (new_var + 0x18C))) - 0x6A;
  D_800D7AB4 = (*((u16 *) (new_var + 0x18E))) - 0x24;
  func_80049220(g_Args);
  func_800493C0();
  func_80047DBC();
  func_80045134();
  func_80045420();
  func_800456D0();
  func_800481B0();
  func_80048E60();
  D_800D74F8 = 0;
  D_800D8150 = 0;
  D_800D9208 = -2;
  D_800D920C = 0;
  D_800D91F8 = 0;
  D_800D9218 = 0;
  func_80029684();
  func_80049D60();
  func_80047D74();
  func_80047A04();
  func_8004A8E4();
  func_8004AF0C();
  func_8004B0B0();
  func_8004B188();
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80047A04);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80047B50);

void func_80047D74(void) {
    s16 i;

    for (i = 0; i < 2; i++) {
    }
    func_80047DB4();
}

void func_80047DB4(void) {
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80047DBC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80047ECC);

void func_800481B0(void) {
    s16 k;
    s16 i;
    s16 j;
    POLY_FT4 *p;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 32; i++) {
            for (j = 0; j < 2; j++) {
                p = &D_800D4984[k][i][j];
                func_80081C84(p);
                func_80081CE4(p, 1);
                if (D_8009D040[i >> 2][3] != 0) {
                    D_800D4984[k][i][j].tpage = GetTPage(0, 3, 0x2B0, 0x180);
                    D_800D4984[k][i][j].clut = GetClut(0x2C0, 0x1F4);
                    ((void (*)(void *, s32, s32, s32, s32, s32, s32, s32, s32))func_80044B74)(p, 0xC0, 0x80, 0xFE, 0x80, 0xC0, 0xBF, 0xFE, 0xBF);
                } else {
                    D_800D4984[k][i][j].tpage = GetTPage(0, 3, 0x2A0, 0x180);
                    D_800D4984[k][i][j].clut = GetClut(0x2D0, 0x1F4);
                    ((void (*)(void *, s32, s32, s32, s32, s32, s32, s32, s32))func_80044B74)(p, 0x80, 0x80, 0xBF, 0x80, 0x80, 0xBF, 0xBF, 0xBF);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800483D8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004841C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80048794);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80048B4C);

s32 func_80048DE4(char *name) {
    if ((u32)strlen(name) < 0x23) {
        return 0;
    }
    if (g_Args->cdArchive == 0) {
        sprintf(g_Args->exePath, D_800A85D4, name);
        func_8006B230(0, (s32)g_Args->exePath);
    }
    return 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80048E60);

void func_80048FE8(void) {
    D_800D86C4 = D_800AC4D8;
    D_800D86C8 = D_800AC4DC;
}

void func_8004900C(void) {
    s32 level;

    if (*(s32 *)((u8 *)g_Player + 0x34) <= 0) {
        *((u8 *)g_Player + 0xA4) = 2;
    } else if (*((u8 *)g_Player + 0xA4) & 2) {
        *((u8 *)g_Player + 0xA4) = 0;
    }
    if (D_8009CA90 != 0) {
        level = 8;
    } else {
        level = 8 - (*(s32 *)((u8 *)g_Player + 0x34) >> 14);
    }
    if (D_8009D080 != level && D_8009CA90 == 0) {
        if (D_8009D080 < level) {
            if (rand() & 1) {
                func_80026F30((D_8009CDA8 & 3) + 15, 0);
            } else {
                func_80026F30(0x65, 0);
            }
        } else if (g_GodMode == 0) {
            func_80026F30(8, 0);
        }
    }
    D_8009D080 = level;
    if ((u32)D_8009CDA8 % 5 == 0) {
        if ((u32)D_8009CDA8 % 10 == 0 && level >= 6 && D_8009D020 <= 0) {
            D_800D7BE8 = D_800D7BEC + 0x80;
        } else {
            D_800D7BE8 = D_800D7BEC + level * 16;
        }
    }
    D_8009D020 = D_8009CFF0 >> 14;
    if (D_8009D020 < D_8009D024 && D_8009CA90 == 0) {
        if (rand() & 1) {
            func_80026F30((D_8009CDA8 & 3) + 15, 0);
        } else {
            func_80026F30(0x65, 0);
        }
    }
    D_8009D024 = D_8009D020;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80049220);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800493C0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80049470);

void func_8004973C(VECTOR v, s16 *out) {
    gte_SetRotMatrix(M2C_FIELD(D_800DDDA8, u8 **, 0x90) + 0x14);
    D_1F8003E8[0] = v.vx;
    D_1F8003E8[1] = v.vy;
    D_1F8003E8[2] = v.vz;
    D_1F8003E8[0] = (D_1F8003E8[0] < -0x7FFF) ? -0x7FFF : ((D_1F8003E8[0] > 0x7FFF) ? 0x7FFF : D_1F8003E8[0]);
    D_1F8003E8[1] = (D_1F8003E8[1] < -0x7FFF) ? -0x7FFF : ((D_1F8003E8[1] > 0x7FFF) ? 0x7FFF : D_1F8003E8[1]);
    D_1F8003E8[2] = (D_1F8003E8[2] < -0x7FFF) ? -0x7FFF : ((D_1F8003E8[2] > 0x7FFF) ? 0x7FFF : D_1F8003E8[2]);
    ((SVECTOR *)D_1F8003E0)->vx = D_1F8003E8[0];
    ((SVECTOR *)D_1F8003E0)->vy = D_1F8003E8[1];
    ((SVECTOR *)D_1F8003E0)->vz = D_1F8003E8[2];
    gte_SetRotMatrix(D_800B0808);
    gte_SetTransMatrix(D_800B0808);
    gte_ldv0(D_1F8003E0);
    gte_rtps();
    gte_stsxy(out);
}

void func_800498E4(s16 *out) {
    gte_SetRotMatrix(M2C_FIELD(D_800DDE30, u8 **, 0x90) + 0x14);
    D_1F8003E8[0] = D_800DDDDC[0];
    D_1F8003E8[1] = D_800DDDDC[1];
    D_1F8003E8[2] = D_800DDDDC[2];
    D_1F8003E8[0] = (D_1F8003E8[0] < -0x7FFF) ? -0x7FFF : ((D_1F8003E8[0] > 0x7FFF) ? 0x7FFF : D_1F8003E8[0]);
    D_1F8003E8[1] = (D_1F8003E8[1] < -0x7FFF) ? -0x7FFF : ((D_1F8003E8[1] > 0x7FFF) ? 0x7FFF : D_1F8003E8[1]);
    D_1F8003E8[2] = (D_1F8003E8[2] < -0x7FFF) ? -0x7FFF : ((D_1F8003E8[2] > 0x7FFF) ? 0x7FFF : D_1F8003E8[2]);
    ((SVECTOR *)D_1F8003E0)->vx = D_1F8003E8[0];
    ((SVECTOR *)D_1F8003E0)->vy = D_1F8003E8[1];
    ((SVECTOR *)D_1F8003E0)->vz = D_1F8003E8[2];
    gte_SetRotMatrix(D_800B0808);
    gte_SetTransMatrix(D_800B0808);
    gte_ldv0(D_1F8003E0);
    gte_rtps();
    gte_stsxy(out);
}

void func_80049A88(void) {
    s16 pos[2];
    s32 x;
    s32 y;
    s32 i;
    u8 r;
    u8 g;
    u8 b;
    u8 *p;

    if (D_800DDE30 != 0) {
        func_800498E4(pos);
        r = 0xFF;
        g = 0;
        b = 0;
        x = pos[0];
        y = pos[1];
    } else {
        r = 0;
        g = 0xFF;
        b = 0;
        x = (s16)g_Args->displayW >> 1;
        y = (s16)g_Args->displayH >> 1;
    }
    p = D_800D7C24[0];
    for (i = 0; i < 2; i++) {
        p[3] = 3;
        p[7] = 0x40;
        p[4] = r;
        p[5] = g;
        p[6] = b;
        M2C_FIELD(p, s16 *, 8) = x;
        M2C_FIELD(p, s16 *, 0xA) = y - 2;
        M2C_FIELD(p, s16 *, 0xC) = x;
        M2C_FIELD(p, s16 *, 0xE) = y + 2;
        p += 0x10;
        p[3] = 3;
        p[7] = 0x40;
        p[4] = r;
        p[5] = g;
        p[6] = b;
        M2C_FIELD(p, s16 *, 8) = x - 3;
        M2C_FIELD(p, s16 *, 0xA) = y;
        M2C_FIELD(p, s16 *, 0xC) = x + 3;
        M2C_FIELD(p, s16 *, 0xE) = y;
        p += 0x10;
    }
}

void func_80049B7C(void) {
}

void func_80049B84(s32 idx, s32 yoff) {
    POLY_G4 *g;
    POLY_F4 *f;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s16 top;
    s16 right;
    s16 bottom;

    g = &D_800D79D4[idx];
    func_80088C44(g);
    g->r0 = 0;
    g->g0 = 0x50;
    g->b0 = 0;
    g->r1 = 0x32;
    g->g1 = 0x75;
    g->b1 = 0x48;
    g->r2 = 0x32;
    g->g2 = 0x75;
    g->b2 = 0x48;
    g->r3 = 0;
    g->g3 = 0x50;
    g->b3 = 0;
    g->x0 = 0;
    g->y0 = yoff;
    g->x1 = g_Args->displayW;
    g->y1 = yoff;
    g->x2 = 0;
    g->y2 = g_Args->displayH + yoff;
    g->x3 = g_Args->displayW;
    g->y3 = g_Args->displayH + yoff;
    f = &D_800D7554[idx];
    func_80081C54(f);
    func_80044BBC(f, 0x10, 0x10, 0x10);
    func_80081CE4(f, 1);
    SetDrawMode((M2C_UNK *)(&D_800D8138 + idx * 12), 0, 0, GetTPage(0, 0, 0, 0) & 0xFFFF, 0);
    x = 0xA3;
    y = 0x57;
    w = 0xAA;
    h = D_8009CFBE * 8 + 0x1E;
    if (D_800B0B9C != 0) {
        x = 10;
        y = 10;
        w = g_DebugTextX;
        h = g_DebugTextY;
    }
    top = y + yoff;
    right = x + w;
    bottom = y + h + yoff;
    func_80044B3C(f, x, top, right, top, x, bottom, right, bottom);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80049D60);

void func_8004A294(void) {
}

void func_8004A29C(void) {
    if (D_8009D018 == 0) {
        D_8009D014 = 0x40;
        D_8009D018 = 1;
        Menu_ResetState();
        D_800AC38C = 0;
    }
}

void func_8004A2E0(void) {
    D_8009D018 = 5;
    D_8009D01A = 0x20;
    D_8009D014 = 0;
}

void func_8004A304(void) {
    if (D_8009D018 == 5) {
        D_8009D014 = 0;
        D_8009D018 = 4;
        D_8009D01A = 0x20;
    }
}

void func_8004A338(void) {
    D_8009D018 = 0;
    D_8009D014 = 0x40;
    D_8009D01A = 0;
    func_800474AC();
}

s32 func_8004A370(void) {
    return D_8009D018 != 0;
}

s32 func_8004A380(void) {
    return D_8009D018 == 5;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004A398);

void func_8004A81C(void) {
    Menu_ResetState();
    func_80028810(&D_800A85E4, (s32) (M2C_FIELD(g_Args, u16 *, 0x18E) << 0x10) >> 0x11);
}

void func_8004A85C(s32 arg0) {
    Menu_ResetState();
    if (arg0 != 0) {
        func_80028810(arg0, ((s32) (M2C_FIELD(g_Args, u16 *, 0x18E) << 0x10) >> 0x11) - 0x14);
    }
}

void func_8004A8A8(s8 arg0, s8 arg1, s8 arg2, s16 arg3) {
    M2C_FIELD(&D_8009D008, s8 *, 0) = arg0;
    M2C_FIELD(&D_8009D008, s8 *, 1) = arg1;
    M2C_FIELD(&D_8009D008, s8 *, 2) = arg2;
    D_8009D00C = arg3;
    D_800D8150 = 1;
    D_800D79D0 = D_8009CDA8;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004A8E4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004A9E4);

void func_8004AF0C(void) {
    s16 i;
    s16 (*p)[12];
    u8 *dm;

    for (i = 0; i < 2; i++) {
        p = (s16 (*)[12])&D_800D45C4;
        func_80081C54(p[i]);
        func_80081CE4(p[i], 1);
        p[i][4] = 0;
        p[i][5] = 0;
        p[i][6] = g_Args->displayW;
        p[i][7] = 0;
        p[i][8] = 0;
        p[i][9] = g_Args->displayH;
        p[i][10] = g_Args->displayW;
        p[i][11] = g_Args->displayH;
        dm = D_800D5ED8[i];
        SetDrawMode((M2C_UNK *)dm, 0, 0, GetTPage(0, 2, 0, 0) & 0xFFFF, 0);
    }
    ((void (*)(s32, s32, s32))func_8004B028)(*(u8 *)&D_8009D000, *(u8 *)&D_8009D000, *(u8 *)&D_8009D000);
}

void func_8004B028(s8 arg0, s8 arg1, s8 arg2) {
  M2C_UNK *new_var;
  new_var = &D_800D45C4;
  *((s8 *) (((s8 *) new_var) + 4)) = arg0;
  *((s8 *) (((s8 *) new_var) + 0x1C)) = arg0;
  *((s8 *) (((s8 *) new_var) + 5)) = arg1;
  *((s8 *) (((s8 *) new_var) + 0x1D)) = arg1;
  *((s8 *) (((s8 *) new_var) + 6)) = arg2;
  *((s8 *) (((s8 *) new_var) + 0x1E)) = arg2;
}

void func_8004B04C(void) {
    D_8009CFFC = 1;
}

void func_8004B05C(void) {
    D_8009CFFC = -1;
}

s32 func_8004B06C(void) {
    return D_8009D000 == 0xFF;
}

s32 func_8004B084(void) {
    if ((D_8009CFFC == 0) || (D_8009CFFC == 2)) {
        return 0;
    }
    return 1;
}

void func_8004B0B0(void) {
    s16 i;
    void *p;

    for (i = 0; i < 2; i++) {
        p = D_800D5EF0[i];
        func_80081C84(p);
        ((void (*)(void *, s32, s32, s32))func_80044BAC)(p, 0x80, 0x80, 0x80);
        func_80081CE4(p, 0);
        p = D_800D7980[i];
        func_80081C84(p);
        ((void (*)(void *, s32, s32, s32))func_80044BAC)(p, 0x80, 0x80, 0x80);
        func_80081CE4(p, 0);
    }
}

void func_8004B188(void) {
    D_8009D010 = 0;
}

void func_8004B194(void) {
    s32 var_a0;

    FntLoad(0x340, 0x100);
    M2C_FIELD(g_Args, s32 *, 0x1C0) = FntOpen(0x20, 0x18, 0x1F4, 0xE6, 0, 0x200);
    var_a0 = FntOpen(0x20, 0x18, 0x1F4, 0xE6, 0, 0x200);
    M2C_FIELD(g_Args, s32 *, 0x1C4) = var_a0;
    if (D_800B8F24 != 1) {
        var_a0 = M2C_FIELD(g_Args, s32 *, 0x1C0);
    }
    SetDumpFnt(var_a0, g_Args);
}

void func_8004B234(s32 arg0) {
    s32 temp_v1;

    temp_v1 = D_8009CFF0 + arg0;
    D_8009CFF0 = temp_v1;
    if (temp_v1 > 0x20000) {
        D_8009CFF0 = 0x20000;
        return;
    }
    if (temp_v1 < 0) {
        D_8009CFF0 = 0;
    }
}

void func_8004B270(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_a0;
    s32 var_a3;
    s32 var_v1;

    var_a0 = arg0;
    var_a3 = M2C_FIELD(g_Player, s32 *, 0x34);
    if (var_a0 <= 0) {
        var_a3 -= var_a0;
        if (var_a3 > 0x20000) {
            var_a3 = 0x20000;
        }
    } else if ((g_Invincible == 0) || (arg2 != 0)) {
        if ((arg1 == 0) && (D_8009CFF0 > 0) && !(M2C_FIELD(M2C_FIELD(g_Player, void **, 0x94), s32 *, 0x24) & 0x20)) {
            var_v1 = D_8009CFF0;
            if (D_8009CFF0 >= var_a0) {
                var_v1 = var_a0;
            }
            var_a0 -= var_v1;
            D_8009CFF0 -= var_v1;
        }
        var_a3 -= var_a0;
        if (var_a3 < 0) {
            var_a3 = 0;
        }
    }
    M2C_FIELD(g_Player, s32 *, 0x34) = var_a3;
}

void func_8004B324(void) {
}

void func_8004B32C(void) {
}

void func_8004B334(s32 arg0) {
    SetDrawMode(&D_800D8138, 0, 0, GetTPage(0, arg0, 0, 0) & 0xFFFF, 0);
    SetDrawMode(&D_800D8138 + 0xC, 0, 0, GetTPage(0, arg0, 0, 0) & 0xFFFF, 0);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004B3BC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004B58C);

void func_8004B9FC(u8 *obj) {
    s32 *flags;
    s32 *pc;
    s32 op;

    flags = *(s32 **)(obj + 0x90);
    if (*(u32 *)(obj + 0x2C) > (u32)D_8009CDA8) {
        *flags |= 0x80000000;
        return;
    }
    *flags &= 0x7FFFFFFF;
loop:
    pc = *(s32 **)(obj + 0x78);
    op = *pc;
    if ((op & 0xFFFF0000) == 0xFFFF0000) {
        if (op == -0xFF6) {
            *(s32 **)(obj + 0x78) = pc + 1;
            return;
        }
        if (op == -0x1000) {
            *(s32 **)(obj + 0x78) = pc + 1;
            op = pc[1];
            *(s32 *)(obj + 0x78) = op;
            goto loop;
        }
        if (op == -0xFFB) {
            *(s32 **)(obj + 0x78) = 0;
            func_800395E8(obj);
            return;
        }
    }
    op = **(s32 **)(obj + 0x78);
    func_8004BC70(D_800D2D30[*(u16 *)op], flags, 0);
    *(s32 **)(obj + 0x78) += 1;
}

void func_8004BB00(void) {
    u8 *e;
    u16 i;
    s32 *flags;
    s32 *pc;
    s32 op;

    e = D_800B0AB4;
    for (i = 0; i < D_800B0C84; i++, e += 0x34) {
        if (e[1] == 0) {
            continue;
        }
        flags = *(s32 **)(e + 0x1C);
        if (*(u32 *)(e + 0x18) > (u32)D_8009CDA8) {
            *flags |= 0x80000000;
            continue;
        }
        *flags &= 0x7FFFFFFF;
loop:
        pc = *(s32 **)(e + 0x10);
        if (pc == 0) {
            continue;
        }
        op = *pc;
        if ((op & 0xFFFF0000) == 0xFFFF0000) {
            if (op == -0xFF6) {
                *(s32 **)(e + 0x10) = pc + 1;
                continue;
            }
            if (op == -0xFFB) {
                e[1] = 0;
                ((s32 **)e)[4] = 0;
                D_800AFFAC--;
                continue;
            }
            if (op == -0x1000) {
                *(s32 **)(e + 0x10) = pc + 1;
                op = pc[1];
                *(s32 *)(e + 0x10) = op;
                goto loop;
            }
        }
        op = **(s32 **)(e + 0x10);
        func_8004BC70(D_800D2D30[*(u16 *)op], flags, 0);
        *(s32 **)(e + 0x10) += 1;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004BC70);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004BDA8);

void *func_8004BE84(void *arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_t2;
    u32 temp_t1;
    u32 var_a2;
    u32 var_t0;
    u32 var_t3;
    void *temp_a0;
    void *temp_a1;

    var_t2 = 0x70000000;
    var_t0 = D_800AFFC8 + (D_800B0DC0 * 0x124);
    temp_t1 = D_800AFFC8 + (D_800B07A8 * 0x124);
    var_t3 = 0;
    if (temp_t1 >= var_t0) {
        var_a2 = var_t0;
        do {
            if ((M2C_FIELD(var_a2, u8 *, 4) != 0) && (M2C_FIELD(var_a2, u8 *, 6) == arg1)) {
                temp_a0 = M2C_FIELD(arg0, void **, 0x90);
                temp_a1 = M2C_FIELD(var_a2, void **, 0x90);
                temp_v0 = M2C_FIELD(temp_a0, s32 *, 0x28) - M2C_FIELD(temp_a1, s32 *, 0x28);
                temp_v0_2 = M2C_FIELD(temp_a0, s32 *, 0x30) - M2C_FIELD(temp_a1, s32 *, 0x30);
                temp_v1 = (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
                if (temp_v1 < var_t2) {
                    var_t2 = temp_v1;
                    var_t3 = var_a2;
                }
            }
            var_t0 += 0x124;
            var_a2 += 0x124;
        } while (temp_t1 >= var_t0);
    }
    return (void *) var_t3;
}

void *func_8004BF70(void *arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_s3;
    u32 temp_s2;
    u32 var_s0;
    u32 var_s1;
    u32 var_s4;
    void *temp_a0;
    void *temp_a1;

    var_s4 = 0;
    var_s1 = D_800AFFC8 + (D_800B0DA0 * 0x124);
    temp_s2 = D_800AFFC8 + (D_800B07B0 * 0x124);
    var_s3 = 0x70000000;
    if (temp_s2 >= var_s1) {
        var_s0 = var_s1;
        do {
            if ((M2C_FIELD(var_s0, u8 *, 4) != 0) && (var_s1 != arg0) && ((func_800536F0(var_s1) << 0x10) == 0) && (M2C_FIELD(var_s0, u8 *, 6) == arg1)) {
                temp_a0 = M2C_FIELD(arg0, void **, 0x90);
                temp_a1 = M2C_FIELD(var_s0, void **, 0x90);
                temp_v0 = M2C_FIELD(temp_a0, s32 *, 0x28) - M2C_FIELD(temp_a1, s32 *, 0x28);
                temp_v0_2 = M2C_FIELD(temp_a0, s32 *, 0x30) - M2C_FIELD(temp_a1, s32 *, 0x30);
                temp_v1 = (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
                if (temp_v1 < var_s3) {
                    var_s3 = temp_v1;
                    var_s4 = var_s0;
                }
            }
            var_s1 += 0x124;
            var_s0 += 0x124;
        } while (temp_s2 >= var_s1);
    }
    return (void *) var_s4;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C0C0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C368);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C498);

s32 func_8004C50C(void *arg0, s32 arg1) {
    s16 temp_v0;
    s16 temp_v1;
    s32 temp_v1_2;
    s32 var_a1;

    var_a1 = arg1;
    temp_v1 = M2C_FIELD(arg0, s16 *, 0x5C);
    if (temp_v1 != 0) {
        temp_v1_2 = temp_v1 - M2C_FIELD(arg0, s16 *, 0x4A);
        if (temp_v1_2 < var_a1) {
            var_a1 = temp_v1_2;
        }
    }
    temp_v0 = (u16) M2C_FIELD(arg0, s16 *, 0x4A) + var_a1;
    M2C_FIELD(arg0, s16 *, 0x4A) = temp_v0;
    M2C_FIELD(arg0, s16 *, 0x44) = (s16) (M2C_FIELD(arg0, u16 *, 0x48) + temp_v0);
    return var_a1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C558);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C5C8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C698);

M2C_UNK *func_8004C768(void *arg0) {
    M2C_UNK *var_s0;
    s32 var_a0;
    u8 temp_v1;

    var_s0 = NULL;
    temp_v1 = M2C_FIELD(arg0, u8 *, 6);
    var_a0 = 0;
    if (temp_v1 == D_8009D104) {
        var_s0 = &D_800A87A0;
        var_a0 = 0x8000;
    } else if (temp_v1 == D_8009D106) {
        var_s0 = &D_800A87B0;
        var_a0 = 0x20000;
    }
    if (var_s0 != NULL) {
        if (D_8009CFF0 <= 0x1FFFF) {
            func_8004B234(var_a0);
            func_80026F30((s32) D_8009D126, 0);
            return var_s0;
        }
        func_80048B4C(&D_800A87C0, 0x80, 0);
        var_s0 = NULL;
        goto block_8;
    }
block_8:
    return var_s0;
}

s32 func_8004C824(void) {
    return 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C82C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C944);

s32 func_8004CA5C(void *arg0, void *arg1) {
    void *var_s0;

    var_s0 = NULL;
    if (M2C_FIELD(arg1, s32 *, 0x34) > 0) {
        if ((var_s0 = func_8004C768(arg0)) != NULL) {
        } else if ((var_s0 = ((void *(*)(void *))func_8004C824)(arg0)) != NULL) {
        } else if ((var_s0 = func_8004C82C(arg0)) != NULL) {
        } else if ((var_s0 = func_8004C944(arg0)) != NULL) {
        } else if ((var_s0 = func_800693DC(arg0)) == NULL) {
            goto end;
        }
        func_80048B4C(var_s0, 0x80, 0);
        if (M2C_FIELD(arg0, s32 **, 0x60) != NULL) {
            *M2C_FIELD(arg0, s32 **, 0x60) = 0;
            M2C_FIELD(arg0, s32 **, 0x60) = NULL;
        }
        func_800395E8(arg0);
    }
end:
    return var_s0 != NULL;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004CB2C);

void func_8004CBF4(s32 *arg0, u8 *arg1, s32 arg2) {
    s32 i;
    s32 total;
    s32 n;
    s32 idx;

    arg0 += 2;
    n = *arg1 = *arg0++;
    i = 0;
    total = 0;
    for (; i < n; i++) {
        (*(s32 **)(arg1 + 8))[i * 2] = *arg0++;
        (*(s32 **)(arg1 + 8))[i * 2 + 1] = *arg0;
        total += (*(s32 **)(arg1 + 8))[i * 2 + 1];
        arg0 += 6;
    }
    idx = D_800D8FF8[arg2][1];
    if (D_800D8BE8[idx] == 0) {
        D_800D8BE8[idx] = M2C_FIELD(g_Args, s32 *, 0x22C);
        *(s32 *)(arg1 + 4) = M2C_FIELD(g_Args, s32 *, 0x22C);
        M2C_FIELD(g_Args, s32 *, 0x22C) = *(s32 *)(arg1 + 4) + total * 8 + 8;
        return;
    }
    *(s32 *)(arg1 + 4) = D_800D8BE8[idx];
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004CCD8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004CD14);

void func_8004CDA0(s32 *arg0, u8 *arg1) {
    s32 i;
    s32 n;

    n = *arg1 = *arg0++;
    for (i = 0; i < n; i++) {
        *(u8 *)(*(u8 **)(arg1 + 4) + i * 16 + 0xC) = *arg0++;
        *(s32 *)(*(u8 **)(arg1 + 4) + i * 16 + 4) = *arg0++;
        *(s32 *)(*(u8 **)(arg1 + 4) + i * 16 + 8) = *arg0++;
        *(s32 **)(*(u8 **)(arg1 + 4) + i * 16) = arg0;
        arg0 += *(s32 *)(*(u8 **)(arg1 + 4) + i * 16 + 8) * 2;
    }
}

void func_8004CE38(s32 arg0, M2C_UNK arg1) {
    func_8004CE74();
    func_8004CF00(arg0, arg1);
}

s32 func_8004CE74(arg0)
u8 *arg0; {
    s32 i;
    s32 j;
    s32 n;
    s32 count;
    SVECTOR *src;
    SVECTOR *dst;

    n = 0;
    count = arg0[0];
    src = *(SVECTOR **)(arg0 + 4);
    for (i = 0; i < count; i++) {
        src += n;
        n = (*(s32 **)(arg0 + 8))[i * 2 + 1];
        dst = (*(SVECTOR ***)(arg0 + 8))[i * 2];
        for (j = 0; j < n; j++) {
            dst[j] = src[j];
        }
    }
}

void func_8004CF00(u8 *arg0, u8 *arg1) {
    s16 prev;
    s32 slot;
    s32 i;
    s32 n;
    s16 cur;
    SVECTOR *dfp;
    SVECTOR *otp;
    s32 w;

    prev = -1;
    slot = 0;
    n = arg1[0];
    for (i = 0; i < n; i++) {
        cur = *(*(u8 **)(arg1 + 4) + i * 16 + 0xC);
        dfp = *(SVECTOR **)(*(u8 **)(arg1 + 4) + i * 16);
        otp = (*(SVECTOR ***)(arg0 + 8))[*(*(u8 **)(arg1 + 4) + i * 16 + 0xC) * 2] + *(s32 *)(*(u8 **)(arg1 + 4) + i * 16 + 4);
        if (arg0[0] == 1) {
            slot = i;
        } else if (prev > cur || prev == cur) {
            slot++;
        }
        if (slot >= 8) {
            func_8006B230(0, (s32)&D_800A87EC);
        }
        w = (*(s32 **)(arg1 + 8))[slot];
        if (w != 0) {
            gteMIMefunc(otp, dfp, *(s32 *)(*(u8 **)(arg1 + 4) + i * 16 + 8), w);
        }
        prev = cur;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004D048);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004D214);

void func_8004D2D0(void) {
    AnimEnt *r;
    s32 i;
    u32 t;
    ListSet *set;
    u32 j;
    u8 *obj;
    u8 *o;
    s32 idx;

    r = (AnimEnt *)D_800D8FF8;
    for (i = 0; i < D_8009D368; i++, r++) {
        if (r->active == 0) {
            continue;
        }
        obj = r->obj;
        if (obj == NULL) {
            continue;
        }
        idx = r->animIdx;
        if (idx <= 0) {
            continue;
        }
        t = D_8009CDA8 - r->startTime[0];
        set = &D_800D8C7C[idx];
        if (r->type == 0x20 && t >= r->duration) {
            continue;
        }
        if (r->unk2 == D_8009D26C && obj[7] == 0x23 && t >= r->duration) {
            *(s32 *)(obj + 0x64) |= 0x1000000;
            continue;
        }
        for (j = 0; j < *(u32 *)set; j++) {
            t = D_8009CDA8 - r->startTime[j];
            if (r->obj[7] == 0x23) {
                func_8004D844(set, i, j);
            } else {
                func_8004D770(set, i, j);
            }
            if (r->enable[j] == 0) {
                D_800D8EA8[i][j] = 0;
            } else if (set->lists[j].count != 0) {
                D_800D8EA8[i][j] = func_8004D72C(set, j);
            }
        }
        func_8004CE38((s32)D_800D8E8C[i], (s32)D_800D8E74[i]);
        if (t >= r->duration) {
            if (r->flags & 8) {
                func_800395E8(r->obj);
            }
        } else if (r->flags & 0x40) {
            o = r->obj;
            if (*(u8 **)(o + 0x80) != NULL && *(u8 **)(o + 0x104) != NULL) {
                *(s32 *)(*(u8 **)(o + 0x80) + 4) = *(u16 *)(*(u8 **)(o + 0x104) + 0x14);
            }
        }
    }
}

void func_8004D5A8(s32 *arg0) {
    s32 i;
    u32 j;
    u32 max;
    u32 n;
    ListSet *set;
    s32 *data;

    i = 0;
    data = arg0;
    if (g_Args->unk21C > 20) {
        func_8006B230(0, (s32)D_800A880C);
    }
    for (; i < g_Args->unk21C; i++) {
        set = &D_800D8C7C[i + 1];
        if (*data++ != 0x5454) {
            return;
        }
        set->lists = D_800D86E8[i];
        *(u32 *)set = n = *data++;
        if (n >= 9) {
            printf(D_800A8820, i, n);
            exit(-1);
        }
        max = 0;
        for (j = 0; j < *(u32 *)set; j++) {
            set->pad8[j] = 1;
            set->lists[j].count = *data++;
            if (max < set->lists[j].count) {
                max = set->lists[j].count;
            }
            set->lists[j].items = data;
            data += set->lists[j].count;
        }
        M2C_FIELD(set, u32 *, 0x10) = max;
        if (max == 0) {
            printf(D_800A8858, i);
            exit(-1);
        }
    }
}

s32 func_8004D72C(ListSet *set, s32 which) {
    if (set->index >= set->lists[which].count) {
        return 0;
    }
    return set->lists[which].items[set->index];
}

void func_8004D770(ListSet *set, s32 i, s32 j) {
    u32 t;

    t = D_8009CDA8 - ((AnimEntArr *)D_800D8FF8)->e[i].startTime[j];
    if (set->pad8[j] == 1) {
        if (set->lists[j].count == 0) {
            func_8006B230(0, (s32)&D_800A887C);
        }
        t %= set->lists[j].count;
    }
    set->index = t;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004D844);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004D88C);

void func_8004D978(void) {
    switch (D_800B0A94) {
    case 14:
        func_8004DA60();
        break;
    case 7:
        func_8004E07C();
        break;
    default:
        func_8006B230(0, (s32)&D_800A8894);
        break;
    }
    func_8004DBCC();
    func_8003BC98(D_800B0064, D_800B0064 - 0x50, 0);
    GsSetRefView2(&D_800B0C10);
    GsGetLs(D_800D3238, D_800B0808);
    D_8009CD30 = (M2C_FIELD(D_800B0064, s16 *, 2) + ((s32 (*)(s32, s32))ratan2)(M2C_FIELD(&D_800B0C10, s32 *, 0xC) - M2C_FIELD(&D_800B0C10, s32 *, 0), M2C_FIELD(&D_800B0C10, s32 *, 0x14) - M2C_FIELD(&D_800B0C10, s32 *, 8))) & 0xFFF;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004DA60);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004DBCC);

void func_8004DD9C(void) {
    Weapon *w;
    s32 l;
    s32 r;
    s16 pitch;
    u8 *p;

    if (D_8009CAA4 != 0) {
        return;
    }
    if (D_8009CDA0 != 0) {
        return;
    }
    D_800B0C9C = 0;
    D_8009CD5C = 0;
    w = &g_Weapons[D_800B0DD4];
    D_8009CD50 = 1;
    if (D_8009CD62 == 0) {
        D_800B0B10 = *(s16 *)((u8 *)w + 0x56);
    }
    D_8009CD4C = 1;
    D_800D9200[0] = g_Camera.posX;
    D_800D9200[1] = g_Camera.posY;
    D_800D9200[2] = g_Camera.posZ;
    p = *(u8 **)((s32)g_Player + 0x90);
    D_800B0A94 = 7;
    pitch = ((s16 *)p)[0x30];
    D_800B0C64 = pitch - 0x17;
    if (D_800B0C64 > 0x800) {
        D_800B0C64 = pitch - 0x1017;
    }
    D_800B0C68 = D_800B0C64;
    *(s32 *)&D_800B0C5C = gPsi;
    D_800B0C60 = gPsi;
    D_800B0AEC = D_800B0C5C;
    l = g_PadHeld & g_Args->buttonMap[6];
    r = g_PadHeld & g_Args->buttonMap[7];
    if (l || r) {
        D_800B0D94 = D_800B0C5C;
        if (l) {
            D_800B0AEC = (D_800B0C5C - 0x400) & 0xFFF;
        }
        if (r) {
            D_800B0AEC = (D_800B0AEC + 0x400) & 0xFFF;
        }
    } else {
        D_800B0D94 = -1;
    }
    D_800B0C74 = 0;
    D_8009CD54 = 0;
    ((void (*)(s32))func_8004E304)(*(s16 *)&D_800B0C5C);
    func_8004E328();
    g_Camera.rotZ = 0;
    g_Camera.rotX = D_800B0C64;
    g_Camera.rotY = D_800B0C5C;
    D_800B0B14 = *(s16 *)((u8 *)w + 0x54);
    if (D_8009CFCC > 0x10) {
        func_800448F4();
    }
}

void Camera_ResetZoom(void) {
    gte_SetGeomScreen(g_Projection);
    D_800B0C74 = 0;
    D_8009CD54 = 0;
    D_8009D3A4 = 0;
    if (D_8009CD4C != 0) {
        D_800B0A94 = 14;
        D_8009CD4C = 0;
    }
    D_8009CDA2 = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004DFF8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004E07C);

void func_8004E304(void) {
    g_Camera.targetX = g_Args->playerX;
    g_Camera.targetZ = g_Args->playerZ;
}

void func_8004E328(void) {
  int new_var;
  s32 temp_v1;
  ;
  g_Camera.targetY = (-D_8009CFA8) - D_800D91FC;
  new_var = (0x24 < 0) ? (0x24) : (0);
  if (((struct 
  {
    char pad[(0x24 > 0) ? (0x24) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) ((struct 
  {
    char pad[(0x94 > 0) ? (0x94) : (0)];
    typeof(*((void **) 0)) f;
  } *) (((char *) g_Player) + ((0x94 < 0) ? (0x94) : (0))))->f) + new_var))->f & 0x400)
  {
    g_Camera.targetY = ((-D_8009CFA8) - D_800D91FC) + 0x258;
  }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004E37C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004E5D8);

void func_8004E78C(void) {
}

void func_8004E794(void) {
    func_8007A4E4(0, D_800D3238);
    D_800D3238[6] = 0;
    D_800D3238[7] = 0;
    D_800D3238[8] = 0;
    D_800D91FC = 0x578;
    func_8007A4E4(0, (u8 *)&g_Camera + 0x30);
    ((s16 *)&g_Camera)[0] = 10;
    ((s16 *)&g_Camera)[1] = 0x40;
    g_Camera.targetX = 0;
    g_Camera.targetY = -D_800D91FC;
    g_Camera.targetZ = 0;
    g_Camera.rotX = 0;
    g_Camera.rotY = 0;
    g_Camera.rotZ = 0;
    func_80050960();
    func_8004E89C();
    D_800B0C10[7] = (s32)((u8 *)&g_Camera + 0x30);
    D_800B0C10[0] = 0;
    D_800B0C10[1] = 0;
    D_800B0C10[2] = 0;
    D_800B0C10[3] = 0;
    D_800B0C10[4] = 0;
    D_800B0C10[5] = 0x1000;
    GsSetRefView2(D_800B0C10);
    D_800B0C38[7] = 0;
    D_800B0C38[0] = 0;
    D_800B0C38[1] = 0;
    D_800B0C38[2] = 0;
    D_800B0C38[3] = 0;
    D_800B0C38[4] = 0;
    D_800B0C38[5] = 0;
    D_800B0C38[6] = 0;
    D_800B0094 = 0x200;
    func_8004E78C();
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004E89C);

void func_8004E96C(void) {
  M2C_UNK *new_var2;
  s16 var_v0;
  char *new_var3;
  char *new_var;
  new_var2 = &D_800D9210;
  new_var3 = ((char *) new_var2) + ((0 < 0) ? (0) : (0));
  new_var = (char *) (&D_800D9210);
  ((struct 
  {
    char pad[(0 > 0) ? (0) : (0)];
    typeof(*((s16 *) 0)) f;
  } *) new_var3)->f = 0x3C;
  if (D_8009CDA8 & 1)
  {
    var_v0 = 0x1E;
  }
  else
    if (var_v0)
  {
    var_v0 = -0x1E;
  }
  else
  {
    var_v0 = -0x1E;
  }
  ((struct 
  {
    char pad[(2 > 0) ? (2) : (0)];
    typeof(*((s16 *) 0)) f;
  } *) (new_var + ((2 < 0) ? (2) : (0))))->f = var_v0;
  D_800D9214 = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004E9AC);

void func_8004EA68(void *arg0) {
    s32 temp_v1;
    s32 temp_v1_2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a1;
    void *temp_v0;

    temp_a0 = M2C_FIELD(arg0, void **, 0x90);
    M2C_FIELD(&g_CamPosX, s32 *, 0) = (s32) M2C_FIELD(&g_CamPosX, s32 *, -0x44);
    temp_a1 = &g_CamPosX - 0x44;
    M2C_FIELD(temp_a0, s32 *, 0x28) = (s32) M2C_FIELD(&g_CamPosX, s32 *, -0x44);
    temp_v1 = M2C_FIELD(temp_a1, s32 *, 4);
    temp_a0_2 = temp_a0 + 0x28;
    M2C_FIELD(&g_CamPosX, s32 *, 4) = temp_v1;
    M2C_FIELD(temp_a0_2, s32 *, 4) = temp_v1;
    temp_v1_2 = M2C_FIELD(temp_a1, s32 *, 8);
    M2C_FIELD(&g_CamPosX, s32 *, 8) = temp_v1_2;
    temp_v0 = &g_CamPosX - 0x48;
    M2C_FIELD(temp_a0_2, s32 *, 8) = temp_v1_2;
    M2C_FIELD(temp_v0, s32 *, 0x20) = 0;
    M2C_FIELD(temp_v0, s32 *, 0x1C) = 0;
    M2C_FIELD(temp_v0, s32 *, 0x18) = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004EAB8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004EB20);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004EC70);

void func_8004EF44(void) {
    if (g_Args->cdArchive != 0) {
        while (func_80075FA0() != 0) {
            func_8006B1DC((s32)D_800A88A4);
        }
    }
    if (*(s32 *)((u8 *)g_Player + 0x34) <= 0) {
        func_8004E9AC();
    }
    switch (g_CutScene) {
    case 1:
        func_8004EC70();
        break;
    case 2:
        if (D_8009D914 == 0) {
            func_8002B064(D_800E23E8);
            func_8004A29C();
            func_8004A81C();
            D_8009D444 = 20;
        }
        D_8009D914++;
        if (D_8009D444 < D_8009D914) {
            D_8009CA90 = 1;
        }
        break;
    case 3:
        if (D_8009D914 == 0) {
            func_8004A29C();
            func_8004A85C(D_8009D010);
            func_80028810((M2C_UNK *)D_800A88D0, (s16)g_Args->displayH >> 1);
            D_8009D444 = 20;
        } else if (D_8009D914 == 10) {
            if (*(s32 *)((u8 *)g_Player + 0x34) <= 0) {
                func_8002B064(D_800E02B4);
            } else {
                func_8002B064(D_800E23D4);
            }
        }
        D_8009D914++;
        if (D_8009D444 < D_8009D914) {
            D_8009CA90 = 1;
        }
        break;
    }
}

void func_8004F134(s16 arg0, s8 arg1) {
    D_800D9208 = arg0;
    D_800D920C = arg1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004F148);

s32 func_8004F348(void) {
    if (abs(g_Camera.targetX - g_Camera.posX) > 200 || abs(g_Camera.targetY - g_Camera.posY) > 200 || abs(g_Camera.targetZ - g_Camera.posZ) > 200) {
        return 1;
    }
    return 0;
}

void func_8004F3D8(void) {
    D_8009D3A8 = 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004F3E8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004F880);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004FD3C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80050180);

s32 func_8005038C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80050180(arg0, arg1);
    func_8004FD3C(arg2, arg3);
    return (s16)func_8004F348() != 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800503D8);

void func_80050960(void) {
    M2C_FIELD(&g_Camera, s32 *, 0x20) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x1C) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x18) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x2C) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x28) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x24) = 0;
    func_8005099C();
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005099C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800509E4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80050A10);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80050A70);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80051728);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80051AE0);

u8 func_800523E4(u8 *obj, u8 n) {
    SVECTOR a;
    SVECTOR b;
    u8 i;
    u8 best_i;
    u8 count;
    u32 best;
    u32 d;
    s32 ang;
    u8 *p;

    best_i = 0;
    best = -1;
    if (obj != NULL && n != 0) {
        a.vx = *(s32 *)(*(u8 **)(obj + 0x90) + 0x28);
        a.vy = *(s32 *)(*(u8 **)(obj + 0x90) + 0x2C);
        a.vz = *(s32 *)(*(u8 **)(obj + 0x90) + 0x30);
        b.vy = a.vy;
        count = D_8009D5AC[n - 1];
        for (i = 0; i < count; i++) {
            b.vx = D_8009D5A8[n - 1][i * 2] + D_800D2FB0.x;
            b.vz = D_8009D5A8[n - 1][i * 2 + 1] + D_800D2FB0.z;
            d = func_8006D3C0(&a, &b);
            if (d < best) {
                best = d;
                best_i = i;
            }
        }
        b.vx = D_8009D5A8[n - 1][best_i * 2] + D_800D2FB0.x;
        b.vz = D_8009D5A8[n - 1][best_i * 2 + 1] + D_800D2FB0.z;
        ang = ((s32 (*)(s32, s32))ratan2)(b.vx - a.vx, b.vz - a.vz);
        ang = (ang - *(s16 *)(*(u8 **)(obj + 0x90) + 0x62)) & 0xFFF;
        if (ang > 0x800) {
            ang -= 0x1000;
        }
        p = *(u8 **)(obj + 0x118);
        if (*(s32 *)(p + 0x10) & 0x4000) {
            if (ang < 0) {
                *(s32 *)(p + 0x10) |= 2;
            } else {
                *(s32 *)(p + 0x10) &= ~2;
            }
        }
        return best_i;
    }
    return 0xFF;
}

void func_80052630(u8 *obj, s32 dir) {
    u8 idx;
    u8 path;
    s32 x;
    s32 z;
    u8 *p;
    s16 r;

    idx = obj[0x6C];
    path = (obj[0x64] & 0x7F) - 1;
    x = D_8009D5A8[path][idx * 2] + D_800D2FB0.x;
    z = D_8009D5A8[path][idx * 2 + 1] + D_800D2FB0.z;
    if (func_80053684(x, z, obj, 400)) {
        p = *(u8 **)(obj + 0x118);
        if (p != NULL) {
            *(s32 *)(p + 0x10) |= 8;
        }
        if (dir == 1) {
            if (idx >= D_8009D5AC[path] - 1) {
                idx = 0;
                p = *(u8 **)(obj + 0x118);
                if (p != NULL) {
                    *(s32 *)(p + 0x10) |= 0x10;
                }
            } else {
                idx++;
            }
        } else {
            if (idx == 0) {
                p = *(u8 **)(obj + 0x118);
                if (p != NULL) {
                    *(s32 *)(p + 0x10) |= 0x10;
                }
                idx = D_8009D5AC[path] - 1;
            } else {
                idx--;
            }
        }
        *(u32 *)(obj + 0x6C) = (*(u32 *)(obj + 0x6C) & ~0xFF) | idx;
    }
    r = func_80053004(obj, x, z);
    D_800D9232 = r;
    if (r > 0xB6) {
        D_800D9232 = 0xB6;
    } else if (r < -0xB6) {
        D_800D9232 = -0xB6;
    }
}

void func_80052808(u8 *obj, s16 x, s16 y, s16 z) {
    SVECTOR pos;
    s32 out1;
    s32 out2;
    s32 out3;
    u8 *ai;
    u8 *model;
    u8 *nav;
    s32 *st;
    s32 *mpos;
    SVECTOR *node;
    s32 target;
    s16 self;
    u8 *p;
    s16 r;
    s32 mx;
    s32 my;
    s32 mz;
    s32 nx;
    s32 ny;
    s32 nz;

    ai = *(u8 **)(obj + 0x118);
    st = (s32 *)(ai + 0x23C);
    model = *(u8 **)(obj + 0x90);
    mpos = (s32 *)(model + 0x28);
    node = NULL;
    pos.vx = x;
    pos.vy = y;
    pos.vz = z;
    nav = (u8 *)D_8009CE38;
    func_800705A8(D_800E2F54, &pos, 0, (u8 *)&out1);
    target = *(u8 *)&out1 - 1;
    if (((s32 *)ai)[0x8F] != target) {
        st[1] = 0;
    }
    ((s32 *)ai)[0x8F] = target;
    mx = mpos[0];
    my = mpos[1];
    mz = mpos[2];
    pos.vx = mx;
    pos.vy = my;
    pos.vz = mz;
    func_800705A8(D_800E2F54, &pos, 0, (u8 *)&out2);
    self = *(u8 *)&out2 - 1;
    do {
    if (st[1] == 0) {
        ((s16 *)st)[4] = ((u16 *)(*(u8 **)(nav + 0x10) + self * *(s32 *)(nav + 4) * 4))[target * 2];
        ((s16 *)st)[5] = ((u16 *)(*(u8 **)(nav + 0x10) + self * *(s32 *)(nav + 4) * 4))[target * 2 + 1];
        if (((s16 *)st)[4] < 0 || ((s16 *)st)[5] < 0) {
            p = *(u8 **)(obj + 0x118);
            if (p != NULL) {
                ((s32 *)p)[4] |= 0x10;
                if (g_Args->cdArchive == 0 && self != target) {
                    sprintf(g_Args->exePath, D_800A89A4, self, target);
                    func_8006B1DC((s32)g_Args->exePath);
                }
            }
            break;
        }
        st[1] = 1;
    }
    if (st[1] == 1) {
        node = &(*(SVECTOR **)(nav + 0xC))[((s16 *)st)[4]];
        if (func_80053684(node->vx, node->vz, obj, 2000)) {
            p = *(u8 **)(obj + 0x118);
            if (p != NULL) {
                ((s32 *)p)[4] |= 8;
            }
            st[1] = 2;
        }
    }
    if (st[1] == 2) {
        node = &(*(SVECTOR **)(nav + 0xC))[((s16 *)st)[5]];
        if (func_80053684(node->vx, node->vz, obj, 2000)) {
            p = *(u8 **)(obj + 0x118);
            if (p != NULL) {
                ((s32 *)p)[4] |= 8;
            }
            nx = node->vx;
            ny = node->vy;
            nz = node->vz;
            pos.vx = nx;
            pos.vy = ny;
            pos.vz = nz;
            func_800705A8(D_800E2F54, &pos, 0, (u8 *)&out3);
            if (self == *(u8 *)&out3 - 1) {
                st[1] = 0;
            }
        }
    }
    } while (0);
    if (node != NULL) {
        r = func_80053004(obj, node->vx, node->vz);
        D_800D9232 = r;
        if (r > 0xB6) {
            D_800D9232 = 0xB6;
        } else if (r < -0xB6) {
            D_800D9232 = -0xB6;
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80052B2C);

void func_80052BEC(void) {
    func_80069684();
}

void func_80052C0C(u8 *obj) {
    s32 pos[3];
    s32 period;

    if (obj[0xA6] != 0) {
        period = 40;
        if ((D_8009CDA8 - *(s32 *)(obj + 0x2C)) % period == 0) {
            func_80052CA8(pos, obj);
            func_80065058(pos[0], pos[1], pos[2], obj, 10);
        }
    }
}

void func_80052CA8(s32 *out, u8 *obj) {
    s32 tmp[4];
    s32 dist;
    s32 dy;
    u8 *model;
    s32 *pos;
    s32 i;
    s32 dx, dz;

    model = *(u8 **)(obj + 0x90);
    if (obj[6] == D_8009D308 && (*(s32 *)(*(u8 **)(obj + 0x94) + 0x24) & 0x400)) {
        dy = -500;
        dist = 400;
    } else {
        func_8005ADD4(*(void **)(obj + 0xC0), 0xE, tmp);
        dist = 150;
        dy = tmp[1] - (*(s16 *)(*(u8 **)(*(u8 **)(obj + 0xC0) + 0x54) + 0x64C) >> 1);
    }
    i = *(u16 *)(model + 0x62) & 0xFFF;
    dx = (dist * *(s16 *)&D_800A0370[i]) >> 12;
    dz = (dist * (*(volatile s32 *)&D_800A0370[i] >> 16)) >> 12;
    pos = (s32 *)(model + 0x28);
    out[0] = pos[0] + dx;
    out[1] = pos[1] + dy;
    out[2] = pos[2] + dz;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80052DB8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053004);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053080);

void func_800530F4(void) {
    M2C_FIELD(&D_800D9234, u16 *, 0) = (u16) D_800D31B4;
    M2C_FIELD(&D_800D9234, u16 *, 2) = (u16) D_800D31B4;
    M2C_FIELD(&D_800D9234, u16 *, 4) = (u16) D_800D31B4;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053114);

void func_80053650(s32 *arg0) {
    s32 temp_v1;
    s32 var_a1;

    temp_v1 = *arg0 + 1;
    *arg0 = temp_v1;
    if (temp_v1 >= 0) {
        var_a1 = 0xA;
        if (temp_v1 < 0xB) {
            var_a1 = temp_v1;
        }
    } else {
        var_a1 = 0;
    }
    *arg0 = var_a1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053684);

s32 func_800536F0(u32 arg0) {
  void *new_var;
  int new_var2;
  new_var = *((void **) (((s8 *) arg0) + 0x94));
  new_var2 = (*((s32 *) (((s8 *) new_var) + 0x24))) & 0x20;
  return new_var2 != 0;
}

void func_8005370C(void) {
    M2C_UNK *var_a0;
    s32 *var_v1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a1;
    s32 var_a2;

    temp_v0 = func_800305B4(0x4000);
    func_8006B230(func_8002C168(&D_800A89CC, temp_v0) < 0x4001, &D_800A89DC);
    var_a2 = 0;
    var_a1 = 0;
    var_a0 = &D_8009D5B4;
    var_v1 = &D_800D9248;
    do {
        *var_v1 = temp_v0 + var_a2;
        temp_v0_2 = M2C_FIELD(var_a0, s32 *, 4);
        var_a0 += 0x2;
        var_a1 += 1;
        var_a2 += temp_v0_2;
        var_v1 += 0x1;
    } while (var_a1 < 6);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053790);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053804);

u32 func_8005397C(VECTOR *a, VECTOR *b) {
    u32 dx;
    u32 dy;
    u32 dz;

    dx = b->vx - a->vx;
    if ((s32)dx < 0) {
        dx = a->vx - b->vx;
    }
    dy = b->vy - a->vy;
    if ((s32)dy < 0) {
        dy = a->vy - b->vy;
    }
    dz = b->vz - a->vz;
    if ((s32)dz < 0) {
        dz = a->vz - b->vz;
    }
    if (dx < dy) {
        dx ^= dy;
        dy ^= dx;
        dx ^= dy;
    }
    if (dx < dz) {
        dx ^= dz;
        dz ^= dx;
        dx ^= dz;
    }
    if (dy < dz) {
        dy ^= dz;
        dz ^= dy;
        dy ^= dz;
    }
    dz += dy;
    dy >>= 3;
    dz >>= 2;
    return dx + dy + dz;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053A2C);

s32 func_80053A58(void *arg0) {
    s32 var_v0;

    var_v0 = -1;
    if (arg0 != NULL) {
        var_v0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x60), u8 *, 7) & 7;
    }
    return var_v0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053A7C);

s32 func_80053B40(s32 arg0) {
    return ((s32) (((u32) D_800D91E4 >> 4) & 0xFFFF) >> ((arg0 & 0xFF) - 1)) & 1;
}

void func_80053B64(void) {
    if ((M2C_FIELD(&D_800D90C0, u8 *, 0x127) & 1) && (func_8004A380() != 0) && (g_PadPressed & 0x800)) {
        M2C_FIELD(&D_800D90C0, s32 *, 0x124) = (s32) (M2C_FIELD(&D_800D90C0, s32 *, 0x124) | 0x04000000);
    }
}

s32 func_80053BD0(void) {
    return D_800D9268 == 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053BE0);

void func_80053DAC(void) {
    s16 *var_v1;
    s16 temp_v0;
    s16 var_a0;

    var_v1 = &D_800DA270;
    var_a0 = 0;
    do {
        *var_v1 = 0x1C;
        temp_v0 = var_a0 + 1;
        var_a0 = temp_v0;
        var_v1 += 0x18 / 2;
    } while (temp_v0 < 0x1B);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053DE4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053ED8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054098);

AISlot *func_800540CC(void) {
    AISlot *found;
    u32 i;

    found = NULL;
    for (i = 0; i < 7; i++) {
        if (!((D_800D9278[i].flags >> 1) & 1)) {
            found = &D_800D9278[i];
            found->flags |= 2;
            break;
        }
    }
    return found;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054118);

void func_8005414C(void *arg0) {
    void *temp_a1;

    temp_a1 = M2C_FIELD(arg0, void **, 0x118);
    M2C_FIELD(temp_a1, s32 *, 0x1CC) = (s32) (M2C_FIELD(temp_a1, s32 *, 0x1CC) & ~2);
    M2C_FIELD(arg0, void **, 0x118) = NULL;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005416C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054268);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005430C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800543BC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054414);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054604);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005469C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800547C4);

s32 func_8005483C(u32 *cmd, ScriptObj *obj) {
    s32 n;
    s32 x, y, z;
    u16 flags;
    s32 f;
    s32 fx, fy, fz;
    s32 *tgt;
    s32 *pos;
    s32 *cam;

    n = 1;
    x = 0;
    y = 0;
    f = ((u16 *)cmd)[1];
    z = 0;
    flags = f;
    fx = f & 3;
    if (fx) {
        x = cmd[1];
        n = 2;
    }
    fy = f & 0xC;
    if (fy) {
        y = cmd[n++];
    }
    fz = f & 0x30;
    if (fz) {
        z = cmd[n++];
    }
    if (obj != NULL) {
        if (fx == 1) {
            x = (u8)obj->vars[x];
        }
        if (fy == 4) {
            y = (u8)obj->vars[y];
        }
        if (fz == 0x10) {
            z = (u8)obj->vars[z];
        }
        pos = &g_Camera.posX;
        tgt = &g_Camera.targetX;
        if (fx == 0) {
            tgt[0] = pos[0];
        } else {
            tgt[0] = x;
        }
        if (!(flags & 0xC)) {
            tgt[1] = pos[1];
        } else {
            tgt[1] = y;
        }
        if (!(flags & 0x30)) {
            tgt[2] = pos[2];
        } else {
            tgt[2] = z;
        }
        cam = (s32 *)&g_Camera;
        cam[8] = 0;
        cam[7] = 0;
        cam[6] = 0;
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054964);

s32 func_80054AC8(u32 *cmd, ScriptObj *obj) {
    s32 n;
    s32 x, y, z;
    u16 flags;
    s32 f;
    s32 fx, fy, fz;
    s16 *tgt;
    s16 *pos;
    s32 *cam;

    n = 1;
    x = 0;
    y = 0;
    f = ((u16 *)cmd)[1];
    z = 0;
    flags = f;
    fx = f & 3;
    if (fx) {
        x = cmd[1];
        n = 2;
    }
    fy = f & 0xC;
    if (fy) {
        y = cmd[n++];
    }
    fz = f & 0x30;
    if (fz) {
        z = cmd[n++];
    }
    if (obj != NULL) {
        if (fx == 1) {
            x = (u8)obj->vars[x];
        }
        if (fy == 4) {
            y = (u8)obj->vars[y];
        }
        if (fz == 0x10) {
            z = (u8)obj->vars[z];
        }
        pos = (s16 *)((u8 *)&g_Camera + 0x80);
        tgt = &g_Camera.rotX;
        if (fx == 0) {
            tgt[0] = pos[0];
        } else {
            tgt[0] = x & 0xFFF;
        }
        if (!(flags & 0xC)) {
            tgt[1] = pos[1];
        } else {
            tgt[1] = y & 0xFFF;
        }
        if (!(flags & 0x30)) {
            tgt[2] = pos[2];
        } else {
            tgt[2] = z & 0xFFF;
        }
        cam = (s32 *)&g_Camera;
        cam[11] = 0;
        cam[10] = 0;
        cam[9] = 0;
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054BF8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054DA4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054E38);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054F34);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80054FDC);

s32 func_80055050(void) {
    return 0;
}

s32 func_80055058(void) {
    return 0;
}

s32 func_80055060(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 result;
    u32 w;
    u16 idx;
    u8 flags;
    u32 f;
    u32 dst;
    s32 ret;

    result = 0;
    ret = 1;
    w = *cmd;
    idx = ((u8 *)cmd)[2];
    dst = w >> 24;
    f = w >> 8;
    flags = f;
    if (obj != NULL) {
        if ((f & 3) == ret) {
            idx = obj->vars[idx];
        }
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    if (idx < D_8009E490) {
        result = D_8009E480[idx]();
    } else {
        obj->wait = 1;
        printf(D_800A8A2C, chr[0xA7], chr[6], idx, obj->pc - *(u32 **)((u8 *)obj + 0xA0));
    }
    if (obj != NULL && (flags & 0xC)) {
        obj->vars[dst] = result;
    }
    return ret;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005516C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005519C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055204);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005529C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800552FC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005535C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800553C0);

s32 func_80055494(void *arg0, void *arg1) {
  typeof(*((s32 *) 0)) new_var;
  if (1)
  {
    new_var = ((struct 
    {
      char pad[(4 > 0) ? (4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f;
    if (arg1 != ((void *) 0))
    {
      ((struct 
      {
        char pad[(0x1C4 > 0) ? (0x1C4) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) arg1) + ((0x1C4 < 0) ? (0x1C4) : (0))))->f = 0;
      ((struct 
      {
        char pad[(0xA4 > 0) ? (0xA4) : (0)];
        typeof(*((void **) 0)) f;
      } *) (((char *) arg1) + ((0xA4 < 0) ? (0xA4) : (0))))->f = (void *) (arg0 + 8);
      ((struct 
      {
        char pad[(0x10 > 0) ? (0x10) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) arg1) + ((0x10 < 0) ? (0x10) : (0))))->f = (s32) (((struct 
      {
        char pad[(0x10 > 0) ? (0x10) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) arg1) + ((0x10 < 0) ? (0x10) : (0))))->f | new_var);
    }
  }
  return 2;
}

s32 func_800554BC(void *arg0, void *arg1) {
  typeof(*((s32 *) 0)) new_var;
  new_var = ((struct 
  {
    char pad[(4 > 0) ? (4) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f;
  if (arg1 != ((void *) 0))
  {
    ((struct 
    {
      char pad[(0xA4 > 0) ? (0xA4) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg1) + ((0xA4 < 0) ? (0xA4) : (0))))->f = (void *) (arg0 + 8);
    ((struct 
    {
      char pad[(0x1C4 > 0) ? (0x1C4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1C4 < 0) ? (0x1C4) : (0))))->f = 0;
    ((struct 
    {
      char pad[(0x10 > 0) ? (0x10) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x10 < 0) ? (0x10) : (0))))->f = (s32) (((struct 
    {
      char pad[(0x10 > 0) ? (0x10) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x10 < 0) ? (0x10) : (0))))->f & (~new_var));
  }
  return 2;
}

s32 func_800554E8(void *arg0, void *arg1) {
  s32 new_var;
  typeof(*((s32 *) 0)) new_var2;
  new_var2 = ((struct 
  {
    char pad[(4 > 0) ? (4) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f;
  if (arg1 != ((void *) 0))
  {
    ((struct 
    {
      char pad[(0x1C4 > 0) ? (0x1C4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1C4 < 0) ? (0x1C4) : (0))))->f = 0;
    new_var = (s32) (((struct 
    {
      char pad[(0x18 > 0) ? (0x18) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x18 < 0) ? (0x18) : (0))))->f | new_var2);
    ((struct 
    {
      char pad[(0xA4 > 0) ? (0xA4) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg1) + ((0xA4 < 0) ? (0xA4) : (0))))->f = (void *) (arg0 + 8);
    ((struct 
    {
      char pad[(0x18 > 0) ? (0x18) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x18 < 0) ? (0x18) : (0))))->f = new_var;
  }
  return 2;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055510);

s32 func_80055544(void *arg0, void *arg1) {
  typeof(*((s32 *) 0)) new_var;
  new_var = ((struct 
  {
    char pad[(4 > 0) ? (4) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f;
  if (arg1 != ((void *) 0))
  {
    ((struct 
    {
      char pad[(0x1C4 > 0) ? (0x1C4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1C4 < 0) ? (0x1C4) : (0))))->f = 0;
    ((struct 
    {
      char pad[(0xA4 > 0) ? (0xA4) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg1) + ((0xA4 < 0) ? (0xA4) : (0))))->f = (void *) (arg0 + 8);
    ((struct 
    {
      char pad[(0 > 0) ? (0) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0 < 0) ? (0) : (0))))->f = (s32) (((struct 
    {
      char pad[(0 > 0) ? (0) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0 < 0) ? (0) : (0))))->f | new_var);
  }
  return 2;
}

s32 func_8005556C(void *arg0, void *arg1) {
  int new_var;
  ((struct 
  {
    char pad[(4 > 0) ? (4) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f++;
  ((struct 
  {
    char pad[(4 > 0) ? (4) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f--;
  if (arg1 != ((void *) 0))
  {
    new_var = ~((struct 
    {
      char pad[(4 > 0) ? (4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f;
    ((struct 
    {
      char pad[(0xA4 > 0) ? (0xA4) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg1) + ((0xA4 < 0) ? (0xA4) : (0))))->f = (void *) (arg0 + 8);
    ((struct 
    {
      char pad[(0x1C4 > 0) ? (0x1C4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1C4 < 0) ? (0x1C4) : (0))))->f = 0;
    ((struct 
    {
      char pad[(0 > 0) ? (0) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0 < 0) ? (0) : (0))))->f = (s32) (((struct 
    {
      char pad[(0 > 0) ? (0) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0 < 0) ? (0) : (0))))->f & new_var);
  }
  return 2;
}

s32 func_80055598(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 n;
    s32 x, y, z;
    s32 f;
    s32 fx, fy, fz;

    n = 1;
    x = 0;
    y = 0;
    f = ((u16 *)cmd)[1];
    z = 0;
    fx = f & 3;
    if (fx) {
        x = cmd[n++];
    }
    fy = f & 0xC;
    if (fy) {
        y = cmd[n++];
    }
    fz = f & 0x30;
    if (fz) {
        z = cmd[n++];
    }
    if (obj != NULL) {
        if (fx == 1) {
            x = (u8)obj->vars[x];
        }
        if (fy == 4) {
            y = (u8)obj->vars[y];
        }
        if (fz == 0x10) {
            z = (u8)obj->vars[z];
        }
        if (fy) {
            *(s16 *)(*(u8 **)(chr + 0x94) + 0x34) = y & 0xFFF;
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

s32 func_80055620(u32 *cmd, ScriptObj *obj) {
    s32 n;
    s32 x, y, z;
    s32 f;
    s32 fx, fy, fz;

    n = 1;
    x = 0;
    y = 0;
    f = ((u16 *)cmd)[1];
    z = 0;
    fx = f & 3;
    if (fx) {
        x = cmd[n++];
    }
    fy = f & 0xC;
    if (fy) {
        y = cmd[n++];
    }
    fz = f & 0x30;
    if (fz) {
        z = cmd[n++];
    }
    if (obj != NULL) {
        if (fx == 1) {
            x = (u8)obj->vars[x];
        }
        if (fy == 4) {
            y = (u8)obj->vars[y];
        }
        if (fz == 0x10) {
            z = (u8)obj->vars[z];
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

s32 func_80055670(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 n;
    s32 x, y, z;
    s32 f;
    s32 fx, fy, fz;

    n = 1;
    x = 0;
    y = 0;
    f = ((u16 *)cmd)[1];
    z = 0;
    fx = f & 3;
    if (fx) {
        x = cmd[n++];
    }
    fy = f & 0xC;
    if (fy) {
        y = cmd[n++];
    }
    fz = f & 0x30;
    if (fz) {
        z = cmd[n++];
    }
    if (obj != NULL) {
        if (fx == 1) {
            x = (u8)obj->vars[x];
        }
        if (fy == 4) {
            y = (u8)obj->vars[y];
        }
        if (fz == 0x10) {
            z = (u8)obj->vars[z];
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    if (chr != NULL && *(u8 **)(chr + 0x90) != NULL) {
        if (x) {
            *(s32 *)(*(u8 **)(chr + 0x90) + 0x28) = x;
        }
        if (y) {
            *(s32 *)(*(u8 **)(chr + 0x90) + 0x2C) = y;
        }
        if (z) {
            *(s32 *)(*(u8 **)(chr + 0x90) + 0x30) = z;
        }
    }
    return n;
}

s32 func_80055778(u32 *cmd, ScriptObj *obj) {
    s32 n;
    s32 x, y, z;
    s32 f;
    s32 fx, fy, fz;

    n = 1;
    x = 0;
    y = 0;
    f = ((u16 *)cmd)[1];
    z = 0;
    fx = f & 3;
    if (fx) {
        x = cmd[n++];
    }
    fy = f & 0xC;
    if (fy) {
        y = cmd[n++];
    }
    fz = f & 0x30;
    if (fz) {
        z = cmd[n++];
    }
    if (obj != NULL) {
        if (fx == 1) {
            x = (u8)obj->vars[x];
        }
        if (fy == 4) {
            y = (u8)obj->vars[y];
        }
        if (fz == 0x10) {
            z = (u8)obj->vars[z];
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800557C8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800557F4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055814);

s32 func_80055870(void) {
    return 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055878);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055BA8);

s32 func_80055E0C(s32 arg0, void *arg1) {
    if (arg1 != NULL) {
        M2C_FIELD(arg1, s32 *, 0x1C4) = 0;
        M2C_FIELD(arg1, s32 *, 0xA4) = (s32) (arg0 + 4);
    }
    return 1;
}

s32 func_80055E24(void *arg0, void *arg1) {
  void *new_var;
  s32 temp_s2;
  temp_s2 = ((struct 
  {
    char pad[(2 > 0) ? (2) : (0)];
    typeof(*((u16 *) 0)) f;
  } *) (((char *) arg0) + ((2 < 0) ? (2) : (0))))->f + 1;
  new_var = arg0 + 4;
  if (arg1 != ((void *) 0))
  {
    func_80048B4C(new_var, 0x80, 0);
    ((struct 
    {
      char pad[(0x1C4 > 0) ? (0x1C4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1C4 < 0) ? (0x1C4) : (0))))->f = 0;
    ((struct 
    {
      char pad[(0xA4 > 0) ? (0xA4) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg1) + ((0xA4 < 0) ? (0xA4) : (0))))->f = (void *) (arg0 + (temp_s2 * 4));
  }
  return temp_s2;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055E88);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055EF0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055F58);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056054);

s32 func_80056110(u32 *cmd, ScriptObj *obj) {
    u32 idx;

    idx = *cmd >> 8;
    if (obj != NULL) {
        obj->vars[idx & 0xFF] = abs(obj->vars[idx & 0xFF]);
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056154);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056358);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800563BC);

s32 func_8005642C(s32 arg0, void *arg1) {
    if (arg1 != NULL) {
        M2C_FIELD(arg1, s32 *, 0x1C4) = 0;
        M2C_FIELD(arg1, s32 *, 0xA4) = (s32) (arg0 + 4);
    }
    return 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056444);

s32 func_800564AC(s32 arg0, void *arg1, void *arg2) {
  if (arg1 != ((void *) 0))
  {
    ((struct 
    {
      char pad[(0x1E8 > 0) ? (0x1E8) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1E8 < 0) ? (0x1E8) : (0))))->f = (s32) ((struct 
    {
      char pad[(0x28 > 0) ? (0x28) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) ((struct 
    {
      char pad[(0x90 > 0) ? (0x90) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg2) + ((0x90 < 0) ? (0x90) : (0))))->f) + ((0x28 < 0) ? (0x28) : (0))))->f;
    ((struct 
    {
      char pad[(0x1EC > 0) ? (0x1EC) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1EC < 0) ? (0x1EC) : (0))))->f = (s32) ((struct 
    {
      char pad[(0x2C > 0) ? (0x2C) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) ((struct 
    {
      char pad[(0x90 > 0) ? (0x90) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg2) + ((0x90 < 0) ? (0x90) : (0))))->f) + ((0x2C < 0) ? (0x2C) : (0))))->f;
    ((struct 
    {
      char pad[(0x1F0 > 0) ? (0x1F0) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1F0 < 0) ? (0x1F0) : (0))))->f = (s32) ((struct 
    {
      char pad[(0x30 > 0) ? (0x30) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) ((struct 
    {
      char pad[(0x90 > 0) ? (0x90) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg2) + ((0x90 < 0) ? (0x90) : (0))))->f) + ((0x30 < 0) ? (0x30) : (0))))->f;
    ((struct 
    {
      char pad[(0x1C4 > 0) ? (0x1C4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1C4 < 0) ? (0x1C4) : (0))))->f = 0;
    ((struct 
    {
      char pad[(0xA4 > 0) ? (0xA4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0xA4 < 0) ? (0xA4) : (0))))->f = (s32) (arg0 + 4);
  }
  return 1;
}

s32 func_80056500(s32 arg0, void *arg1) {
    if (arg1 != NULL) {
        M2C_FIELD(arg1, s32 *, 0x1C4) = 0;
        M2C_FIELD(arg1, s32 *, 0xA4) = (s32) (arg0 + 4);
        M2C_FIELD(arg1, s32 *, 0x1D8) = (s32) M2C_FIELD(arg1, s32 *, 0x1E8);
        M2C_FIELD(arg1, s32 *, 0x1DC) = (s32) M2C_FIELD(arg1, s32 *, 0x1EC);
        M2C_FIELD(arg1, s32 *, 0x1E0) = (s32) M2C_FIELD(arg1, s32 *, 0x1F0);
    }
    return 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056538);

s32 func_80056570(u32 *cmd, ScriptObj *obj, s32 arg2) {
    u32 idx;

    idx = *cmd >> 8;
    if (obj != NULL) {
        obj->vars[idx & 0xFF] = func_80054098(arg2);
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800565DC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056614);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800566F8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005683C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056894);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056D1C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056F90);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80057074);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800570F8);

s32 func_80057194(void *arg0, void *arg1) {
  s32 new_var;
  new_var = (s32) (((struct 
  {
    char pad[(2 > 0) ? (2) : (0)];
    typeof(*((u16 *) 0)) f;
  } *) (((char *) arg0) + ((2 < 0) ? (2) : (0))))->f << 0x10);
  if (arg1 != ((void *) 0))
  {
    ((struct 
    {
      char pad[(0x1C4 > 0) ? (0x1C4) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) arg1) + ((0x1C4 < 0) ? (0x1C4) : (0))))->f = 0;
    ((struct 
    {
      char pad[(0xA4 > 0) ? (0xA4) : (0)];
      typeof(*((void **) 0)) f;
    } *) (((char *) arg1) + ((0xA4 < 0) ? (0xA4) : (0))))->f = (void *) (arg0 + (new_var >> 0xE));
  }
  return 1;
}

s32 func_800571B8(void *arg0, void *arg1, void *arg2) {
    s32 *p;
    s32 temp_s2;
    u32 n;
    u32 i;
    u16 j;
    void *temp_a2;
    void *temp_s1;
    void *temp_v0;

    temp_s2 = M2C_FIELD(arg0, s32 *, 4);
    temp_s1 = M2C_FIELD(arg2, void **, 0x94);
    if (arg1 != NULL) {
        if (temp_s2 == M2C_FIELD(M2C_FIELD(arg1, void **, 0xA0), s32 *, 4)) {
            M2C_FIELD(arg1, s32 *, 0x1CC) |= 1;
            temp_v0 = arg1 + 0xA8;
            if (temp_v0 != NULL) {
                M2C_FIELD(temp_v0, s32 *, 4) = 0;
                M2C_FIELD(arg1, u32 *, 0xA8) = 0;
            }
        } else {
            temp_a2 = arg1 + 0xA8;
            M2C_FIELD(arg1, s32 *, 0x1CC) &= ~1;
            if (temp_a2 != NULL) {
                p = arg1 + 0xB0;
                n = M2C_FIELD(arg1, u32 *, 0xA8);
                for (i = 0; i < n; i++) {
                    *p = 0;
                    p += 2;
                }
                M2C_FIELD(temp_a2, u32 *, 4) = n;
            }
        }
        M2C_FIELD(arg1, s32 *, 0x10) &= ~M2C_FIELD(arg1, s32 *, 0x18);
        for (j = 0; j < 0x20; j++) {
            if ((M2C_FIELD(arg1, u32 *, 0x1C) >> j) & 1) {
                M2C_FIELD((u8 *)arg1 + (j << 2), s32 *, 0x20) = 0;
            }
        }
        if ((arg1 + 0x1B0) != NULL) {
            M2C_FIELD(arg1, s32 *, 0x1B0) = 0;
        }
        func_80057BFC(temp_s1, 2);
        M2C_FIELD(temp_s1, s8 *, 3) = 0;
        M2C_FIELD(arg1, s32 *, 0x1C4) = 0;
        M2C_FIELD(arg1, s32 **, 0xA4) = M2C_FIELD(arg1, s32 **, 0xA0) + temp_s2;
    }
    return 2;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800572F4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800573A4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80057410);

s32 func_8005743C(u32 *cmd, ScriptObj *obj) {
    u32 v;
    u32 idx;
    s32 ret;

    v = *cmd;
    ret = 1;
    idx = v >> 16;
    v >>= 8;
    if (obj != NULL) {
        if ((v & 3) == ret) {
            idx = (u16)obj->vars[idx];
        }
        obj->wait = idx;
        obj->pc = cmd + 1;
    }
    return ret;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005747C);

s32 func_800575FC(u8 *cmd, void *arg1, void *arg2) {
    u8 op;

    op = *cmd;
    if (op >= 0x44) {
        op = 0x43;
    }
    return D_8009D7A8[op](cmd, arg1, arg2);
}

s32 func_80057648(void *arg0, void *arg1) {
    s32 temp_s2;
    s32 var_v1;

    temp_s2 = M2C_FIELD(g_Args, s32 *, 0x2DC);
    if (arg0 != NULL) {
        if (M2C_FIELD(arg0, s32 *, 0x1C4) == 0) {
            do {
                if (((u32) (M2C_FIELD(g_Args, s32 *, 0x2DC) - temp_s2) >= 3U) && (M2C_FIELD(g_Args, s8 *, 0x1BC) == 0)) {
                    printf(&D_800A8C78, M2C_FIELD(arg1, u8 *, 0xA7), M2C_FIELD(arg1, u8 *, 6), (s32) (M2C_FIELD(arg0, s32 *, 0xA4) - M2C_FIELD(arg0, s32 *, 0xA0)) >> 2);
                }
                func_800575FC(M2C_FIELD(arg0, s32 *, 0xA4), arg0, arg1);
            } while (M2C_FIELD(arg0, s32 *, 0x1C4) == 0);
        }
        var_v1 = 1;
        M2C_FIELD(arg0, s32 *, 0x1C4) = (s32) (M2C_FIELD(arg0, s32 *, 0x1C4) - 1);
    } else {
        var_v1 = 0;
    }
    return var_v1;
}

void func_8005772C(void *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0);
    temp_v1 = M2C_FIELD(arg0, s32 *, 8);
    M2C_FIELD(arg0, s32 *, 0) = 0;
    M2C_FIELD(arg0, s32 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 4) = temp_v0;
    M2C_FIELD(arg0, s32 *, 0xC) = temp_v1;
    M2C_FIELD(arg0, s32 *, 0x14) = (s32) M2C_FIELD(arg0, s32 *, 0x10);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80057750);

s32 func_80057A24(void *arg0, s32 arg1) {
    s32 *var_a2;
    u32 var_v1;
    void *temp_v0;

    if (arg0 != NULL) {
        temp_v0 = arg0 + 0xA8;
        if (arg1 != 0) {
            if (temp_v0 != NULL) {
                M2C_FIELD(temp_v0, s32 *, 4) = 0;
                M2C_FIELD(arg0, s32 *, 0xA8) = 0;
            }
            if ((arg0 + 0x1B0) != NULL) {
                M2C_FIELD(arg0, s32 *, 0x1B0) = 0;
            }
            var_a2 = arg0 + 0x20;
            var_v1 = 0;
            do {
                *var_a2 = 0;
                var_v1 += 1;
                var_a2 += 0x1;
            } while (var_v1 < 0x20U);
            M2C_FIELD(arg0, s32 *, 0) = 0;
            M2C_FIELD(arg0, s32 *, 4) = 0;
            M2C_FIELD(arg0, s32 *, 8) = 0;
            M2C_FIELD(arg0, s32 *, 0xC) = 0;
            M2C_FIELD(arg0, s32 *, 0x10) = 0;
            M2C_FIELD(arg0, s32 *, 0x14) = 0;
            M2C_FIELD(arg0, s32 *, 0x18) = 0;
            M2C_FIELD(arg0, s32 *, 0x1C) = 0;
            M2C_FIELD(arg0, s32 *, 0xA0) = arg1;
            M2C_FIELD(arg0, s32 *, 0xA4) = arg1;
            M2C_FIELD(arg0, s32 *, 0x1C4) = 0;
            M2C_FIELD(arg0, s16 *, 0x1F8) = 0;
            return 1;
        }
    }
    return 0;
}

s32 func_80057AB0(s32 arg0) {
    return *D_800DDEE4[(s16)arg0] - 1;
}

void func_80057AD8(void *arg0, s16 arg1, s16 arg2) {
    M2C_FIELD(arg0, s16 *, 0x12) = arg1;
    M2C_FIELD(arg0, s16 *, 0x16) = arg2;
    func_8005C894();
    M2C_FIELD(arg0, s32 *, 0x28) = 0;
    M2C_FIELD(arg0, s32 *, 0x2C) = 0;
    M2C_FIELD(arg0, s32 *, 0x30) = 0;
    M2C_FIELD(arg0, u16 *, 0x18) = (u16) M2C_FIELD(arg0, u16 *, 0x1E);
    M2C_FIELD(arg0, s32 *, 0x24) = (s32) (M2C_FIELD(arg0, s32 *, 0x24) & 0xFFFBFFFF);
}

void func_80057B2C(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    M2C_FIELD(arg0, s16 *, 0x10) = arg1;
    M2C_FIELD(arg0, s16 *, 0x14) = arg2;
    M2C_FIELD(arg0, s16 *, 0x12) = arg3;
    M2C_FIELD(arg0, s16 *, 0x16) = arg4;
    func_8005C864(arg0);
    func_8005C894(arg0);
    M2C_FIELD(arg0, s32 *, 0x28) = 0;
    M2C_FIELD(arg0, s32 *, 0x2C) = 0;
    M2C_FIELD(arg0, s32 *, 0x30) = 0;
    M2C_FIELD(arg0, u16 *, 0x18) = (u16) M2C_FIELD(arg0, u16 *, 0x1E);
    M2C_FIELD(arg0, s32 *, 0x24) = (s32) (M2C_FIELD(arg0, s32 *, 0x24) & 0xFFFBFFFF);
}

void func_80057B94(void *arg0, s16 arg1, s16 arg2, s16 arg3) {
    M2C_FIELD(arg0, s16 *, 0x10) = arg1;
    M2C_FIELD(arg0, s16 *, 0x14) = 0;
    M2C_FIELD(arg0, s16 *, 0x12) = arg2;
    M2C_FIELD(arg0, s16 *, 0x16) = 0;
    func_8005C864();
    func_8005C894(arg0);
    M2C_FIELD(arg0, s16 *, 0x18) = arg3;
    M2C_FIELD(arg0, s32 *, 0x28) = 0;
    M2C_FIELD(arg0, s32 *, 0x2C) = 0;
    M2C_FIELD(arg0, s32 *, 0x30) = 0;
    M2C_FIELD(arg0, s32 *, 0x24) = (s32) (M2C_FIELD(arg0, s32 *, 0x24) | 0x40000);
}

void func_80057BFC(void *arg0, s16 arg1) {
    if (arg1 > 0) {
        M2C_FIELD(arg0, u32 *, 0x20) = (u32) arg1;
    } else {
        M2C_FIELD(arg0, u32 *, 0x20) = 2U;
    }
    M2C_FIELD(arg0, s16 *, 0x18) = 0;
    M2C_FIELD(arg0, s16 *, 0x1E) = (s16) (0x1000U / (u32) M2C_FIELD(arg0, u32 *, 0x20));
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80057C40);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80057E80);

void *func_80058694(u8 type) {
    s16 i;
    u8 *p;
    s16 count;
    FxTmpl *t;

    p = NULL;
    D_800DA5C0 = 0;
    if (type >= 10) {
        printf(D_800A8CB0, type, 10);
        printf(D_800A8CC4, D_800A8CEC, 0x20C);
        exit(1);
    }
    t = &((FxTmpl *)D_8009D8B8)[type];
    switch (t->flags & 0xF0) {
    case 0x10:
        count = 0;
        p = (u8 *)&D_800AB540;
        break;
    case 0x20:
    case 0x30:
    case 0x90:
    case 0xA0:
        count = 30;
        p = D_800DB884;
        break;
    case 0x80:
        count = 30;
        p = D_800DA5C4;
        break;
    default:
        count = 0;
        sprintf(g_Args->exePath, D_800A8CF8, type);
        func_8006B230(0, (s32)g_Args->exePath);
        break;
    }
    for (i = 0; i < count; i++, p += 0xA0) {
        if (p[0x9B] == 0) {
            goto found;
        }
    }
    if (i == count) {
        D_800DA5C0 = 1;
    }
    return NULL;
found:
    func_8005885C(p, type);
    return p;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005885C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80058A58);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80058B10);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80058B6C);

void func_80058D00(void) {
    u8 *p;
    u8 *end;

    end = D_800DB884 + 30 * 0xA0;
    for (p = end - 30 * 0xA0; p < end; p += 0xA0) {
        p[0x9B] = 0;
    }
    end = D_800DA5C4 + 30 * 0xA0;
    for (p = end - 30 * 0xA0; p < end; p += 0xA0) {
        p[0x9B] = 0;
    }
}

void func_80058D5C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 t;
    s32 sa;
    s32 ca;
    s32 cb;
    s32 x;
    s32 z;

    t = D_800A0370[arg1 & 0xFFF];
    ca = t >> 16;
    sa = (s16)t;
    t = D_800A0370[arg2 & 0xFFF];
    cb = t >> 16;
    z = (ca * cb) >> 12;
    x = (((sa * cb) >> 12) * arg3) >> 12;
    *(s16 *)(arg0 + 0x40) = (-(s16)t * arg3) >> 12;
    *(s16 *)(arg0 + 0x3E) = x;
    *(s16 *)(arg0 + 0x42) = (z * arg3) >> 12;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80058DFC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80058F28);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80059634);

void func_80059C14(u8 *arg0) {
    s32 m[8];
    long ofx;
    long ofy;
    long z;
    s16 i;
    if (arg0 == NULL) {
        arg0 = D_800DD83C;
    }
    gte_ReadGeomOffset(&ofx, &ofy);
    gte_SetGeomOffset((s16)g_Args->displayW / 2, ofy);
    GsGetLs(D_800D3238, m);
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    for (i = 0; i < 8; i++) {
        gte_ldv0(&((struct { u8 pad[0x184]; SVECTOR v[8]; SVECTOR s[8]; } *)arg0)->v[i]);
        gte_rtps();
        gte_stsxy(&((struct { u8 pad[0x184]; SVECTOR v[8]; SVECTOR s[8]; } *)arg0)->s[i]);
        gte_stszotz(&z);
        ((struct { u8 pad[0x184]; SVECTOR v[8]; SVECTOR s[8]; } *)arg0)->s[i].vz = z;
    }
    if (D_8009D8D4 != -1) {
        D_800DD808.vx += *(s16 *)&D_8009CA7C;
        D_800DD808.vz += *(s16 *)&D_8009CA84;
        D_800DD810.vx += *(s16 *)&D_8009CA7C;
        D_800DD810.vz += *(s16 *)&D_8009CA84;
        gte_ldv0(&D_800DD808);
        gte_rtps();
        gte_stsxy(&D_800DD7F8);
        gte_stszotz(&z);
        D_800DD7FC = z;
        gte_ldv0(&D_800DD810);
        gte_rtps();
        gte_stsxy(&D_800DD800);
        gte_stszotz(&z);
        D_800DD804 = z;
    }
    gte_SetGeomOffset(ofx, ofy);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80059E4C);

s32 func_8005AD6C(arg0)
s32 arg0; {
    return ((s32 (*)(s32, s32))ratan2)(arg0, SquareRoot12(0x1000 - ((s32) (arg0 * arg0) >> 0xC)));
}

s32 func_8005ADB0(void) {
    return 0x400 - func_8005AD6C();
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005ADD4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005AEF8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005B508);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005B584);

s32 func_8005B5AC(u8 *obj, s32 *p) {
    u16 i;
    u8 *sub;
    s32 *end;

    i = 0;
    end = &(&D_800DDFEC)[5];
    for (; p <= end; p++, i++) {
        if (D_800DE0C8[i] == 0 && func_8005B584(obj == g_Player, obj[6], i) != 0) {
            func_8007D260((&D_800DDFEC)[i]);
            sub = *(u8 **)(obj + 0x94);
            sub[0xA] = i;
            *(s32 *)(sub + 0x24) &= ~0x20000;
            D_800DE0C8[i] = 1;
            D_800DDEF0[i] = obj;
            return *p;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005B6CC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005B76C);

void func_8005B7BC(void *obj, s32 arg1) {
    void *t;
    s32 r;
    s32 g;
    s32 b;

    t = M2C_FIELD(obj, void **, 0x94);
    if (M2C_FIELD(t, s32 *, 0x24) & 0x20000) {
        if (D_8009CABD != 0) {
            r = 0xFF;
            g = 0;
            b = 0;
        } else if (D_8009CABE != 0 || (M2C_FIELD(obj, s32 *, 0x48) & 4)) {
            r = 0;
            g = 0;
            b = 0;
        } else {
            b = M2C_FIELD(t, s16 *, 8) * 4;
            r = b;
            g = b;
        }
        func_8007D184(arg1, r, g, b);
        func_8007D1E8(arg1, 1);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005B86C);

void func_8005B96C(void *arg0, s32 arg1, void *arg2) {
    s32 *p;
    s32 *q;
    s32 idx;

    idx = ((u8 *)arg2)[0xA];
    p = (s32 *)(*(u8 **)((u8 *)arg0 + 0xC0) + 0x24);
    p[0] = (s32)D_800DDEE4[*(s16 *)((u8 *)arg2 + 0x10)];
    p[1] = (s32)D_800DDEE4[*(s16 *)((u8 *)arg2 + 0x12)];
    p[2] = *(s16 *)((u8 *)arg2 + 0x14);
    p[3] = *(s16 *)((u8 *)arg2 + 0x16);
    p[8] = *(s16 *)((u8 *)arg2 + 0x18);
    q = (s32 *)(D_800DE018 + idx * 32);
    *q++ = *(s32 *)(arg1 + 0x28);
    q[0] = *(s32 *)(arg1 + 0x2C);
    q[1] = *(s32 *)(arg1 + 0x30);
    func_8005BA30(arg0);
    func_8005B7BC(arg0, *(s32 *)((u8 *)arg0 + 0xC0));
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005BA30);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005BAE8);

void func_8005BC80(u8 *obj, u8 *anim) {
    SVECTOR v;
    s16 *p1;
    s16 *p;
    u8 *e;
    s16 x1, y1, z1, x2, y2, z2;
    u8 *m;
    e = D_800DDEE4[M2C_FIELD(anim, s16 *, 0x10)];
    p = (s16 *)(e + anim[0x14] * 0x4C);
    p += 6;
    x1 = *p;
    p++;
    y1 = *p;
    p++;
    z1 = -*p;
    p1 = p + 2;
    e = D_800DDEE4[M2C_FIELD(anim, s16 *, 0x12)];
    p = (s16 *)(e + anim[0x16] * 0x4C);
    p += 6;
    x2 = *p;
    p++;
    y2 = *p;
    p++;
    z2 = -*p;
    p += 2;
    func_8005C10C(obj, M2C_FIELD(obj, u8 **, 0x8C), p1, p);
    RotMatrixY(M2C_FIELD(M2C_FIELD(obj, u8 **, 0x90), s16 *, 0x62), M2C_FIELD(M2C_FIELD(obj, u8 **, 0x8C), u8 **, 0x90) + 0x14);
    v.vx = x1 + (((x2 - x1) * M2C_FIELD(anim, s16 *, 0x18)) >> 12);
    v.vy = z1 + (((z2 - z1) * M2C_FIELD(anim, s16 *, 0x18)) >> 12);
    v.vz = y1 + (((y2 - y1) * M2C_FIELD(anim, s16 *, 0x18)) >> 12);
    v.pad = 0;
    m = M2C_FIELD(obj, u8 **, 0x90);
    gte_SetRotMatrix(m + 0x14);
    gte_ldv0(&v);
    gte_rtv0();
    gte_stlvnl(M2C_FIELD(M2C_FIELD(obj, u8 **, 0x8C), u8 **, 0x90) + 0x28);
    M2C_FIELD(M2C_FIELD(M2C_FIELD(obj, u8 **, 0x8C), u8 **, 0x90), s32 *, 0x28) += M2C_FIELD(m, s32 *, 0x28);
    M2C_FIELD(M2C_FIELD(M2C_FIELD(obj, u8 **, 0x8C), u8 **, 0x90), s32 *, 0x2C) += M2C_FIELD(m, s32 *, 0x2C);
    M2C_FIELD(M2C_FIELD(M2C_FIELD(obj, u8 **, 0x8C), u8 **, 0x90), s32 *, 0x30) += M2C_FIELD(m, s32 *, 0x30);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005BEF0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005C10C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005C214);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005C33C);

M2C_UNK func_8005C864(arg0)
    void *arg0;
{
    M2C_FIELD(arg0, s16 *, 0x1A) = *D_800DDEE4[M2C_FIELD(arg0, s16 *, 0x10)] - 1;
}

M2C_UNK func_8005C894(arg0)
    void *arg0;
{
    M2C_FIELD(arg0, s16 *, 0x1C) = *D_800DDEE4[M2C_FIELD(arg0, s16 *, 0x12)] - 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005C8C4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005CA20);

void func_8005CAF8(s16 arg0, s32 arg1) {
  s32 var_a0;
  s32 *new_var;
  if (arg0 == 1)
  {
    var_a0 = 0x18;
  }
  else
  {
    var_a0 = 0x1C;
  }
  func_80027248(*(new_var = &var_a0), arg1 + 0x14, 0);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005CB38);

void func_8005CED0(void *arg0) {
  Weapon (*new_var)[6];
  Weapon *temp_v0;
  void *temp_a0;
  new_var = &g_Weapons;
  temp_v0 = &g_Weapons[D_800B0DD4];
  temp_a0 = ((struct 
  {
    char pad[(0x94 > 0) ? (0x94) : (0)];
    typeof(*((void **) 0)) f;
  } *) (((char *) arg0) + ((0x94 < 0) ? (0x94) : (0))))->f;
  if (temp_v0->clipSize != temp_v0->clip)
  {
    ((struct 
    {
      char pad[(0x24 > 0) ? (0x24) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) temp_a0) + ((0x24 < 0) ? (0x24) : (0))))->f = (s32) (((struct 
    {
      char pad[(0x24 > 0) ? (0x24) : (0)];
      typeof(*((s32 *) 0)) f;
    } *) (((char *) temp_a0) + ((0x24 < 0) ? (0x24) : (0))))->f & (~0x800));
    func_8007980C(temp_a0, *new_var);
    D_800AFFA4 = 0;
  }
}

void func_8005CF40(void) {
    Weapon *temp_a1;
    s32 temp_a0;

    temp_a1 = &g_Weapons[D_800B0DD4];
    temp_a0 = temp_a1->pad3C[0x14] - M2C_FIELD(temp_a1, s16 *, 0x48);
    if (M2C_FIELD(temp_a1, s16 *, 0x4A) >= temp_a0) {
        M2C_FIELD(temp_a1, s16 *, 0x48) = (s16) temp_a1->pad3C[0x14];
        M2C_FIELD(temp_a1, s16 *, 0x4A) = (s16) ((u16) M2C_FIELD(temp_a1, s16 *, 0x4A) - temp_a0);
    } else {
        M2C_FIELD(temp_a1, s16 *, 0x48) = (s16) ((u16) M2C_FIELD(temp_a1, s16 *, 0x48) + (u16) M2C_FIELD(temp_a1, s16 *, 0x4A));
        M2C_FIELD(temp_a1, s16 *, 0x4A) = 0;
    }
    M2C_FIELD(temp_a1, s16 *, 0x44) = (s16) ((u16) M2C_FIELD(temp_a1, s16 *, 0x48) + (u16) M2C_FIELD(temp_a1, s16 *, 0x4A));
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005CFC8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005D2B0);

s32 func_8005D58C(s32 arg0) {
  s32 temp_v0;
  *((s16 *) (((s8 *) arg0) + 0x1E)) = 0x800;
  *((s16 *) (((s8 *) arg0) + 4)) = -1;
  temp_v0 = D_8009D5E4;
  ;
  *((s32 *) (((s8 *) arg0) + 0)) = (s32) (((*((s32 *) (((s8 *) arg0) + 0))) & (~0xF0)) | ((temp_v0 & 0xF) * 0x10));
  return (temp_v0 & 0xF) * 0x10;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005D5C4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005D630);

void func_8005D654(void) {
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = M2C_FIELD(g_Player, void **, 0x94);
    M2C_FIELD(temp_v1, s32 *, 0x24) = (s32) (M2C_FIELD(temp_v1, s32 *, 0x24) ^ 0x400);
    temp_v1_2 = M2C_FIELD(g_Player, void **, 0x94);
    M2C_FIELD(temp_v1_2, s32 *, 0x24) = (s32) (M2C_FIELD(temp_v1_2, s32 *, 0x24) ^ 0x200);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005D694);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005D760);

void func_8005DA90(void) {
    D_800DDC44 = 0;
    D_800DDCC0 = 0;
    D_800DDCC4 = 0;
    D_800DDDB8 = 0;
    D_800DE0DC = 0;
    D_800DE0E0 = 0;
    D_800DDC60 = 0;
    D_800DDC54 = 0;
    D_800DDC4C = 0;
    D_800DDC68 = 0;
    D_800DDC5C = 0;
    D_800DDC50 = 0;
    D_800DDC48 = 0;
    D_800DDC64 = 0;
    D_800DDDAC = 0;
    D_800DDDB0 = 0;
    D_800DE0D8 = 0;
    D_800DDC58 = 0;
    D_800DDEEC = 0;
    D_800DDE08 = 0;
    D_800DDD94 = 0;
    D_800DDD98 = 0;
    D_800DDDA4 = 0;
    D_800DDD90 = 0;
    D_800DDD8C = 0;
    D_800DDEE8 = 0;
    D_800DDE04 = 0;
    D_800DDCC8 = 0;
    D_800DDDA0 = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005DB7C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005DC40);

void func_8005DF40(void *arg0) {
    s32 temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0x8C);
    if (temp_s0 != 0) {
        func_80060614(temp_s0);
        func_800393AC(temp_s0);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005DF7C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005E020);

void func_8005E228(void *arg0) {
    s32 lim;
    void *obj;
    s32 flags;
    s32 a;
    s32 b;

    if (M2C_FIELD(arg0, s32 *, 0x118) != 0) {
        func_8005E020();
        return;
    }
    if (arg0 != g_Player) {
        func_8006B1DC(&D_800A8FAC);
        return;
    }
    if (g_GodMode != 0 || D_8009CF54 != 0) {
        return;
    }
    obj = M2C_FIELD(arg0, void **, 0x94);
    flags = M2C_FIELD(obj, s32 *, 0x24);
    if (flags & 0x20) {
        return;
    }
    if (flags & 0x100000) {
        return;
    }
    if (M2C_FIELD(arg0, s32 *, 0x34) <= 0) {
        D_8009CD3C[0] = g_Args->playerX;
        D_8009CD3C[1] = g_Args->playerY;
        D_8009CD3C[2] = g_Args->playerZ;
        Camera_ResetZoom();
        D_800AFFA4 = 0;
        func_80069318();
        func_800509E4(g_Player);
        if (M2C_FIELD(g_Args, u8 *, 4) >> 7) {
            func_8002F564(arg0, 0xC8, 2, 0, 0);
        }
    } else {
        if (M2C_FIELD(g_Args, u8 *, 4) >> 7) {
            func_8002F564(arg0, 0xC8, 2, 0, 0);
        }
        if (D_8009CD4C != 0) {
            b = 0xA;
            if (M2C_FIELD(g_Args, u8 *, 0x12A) == 1) {
                b = 0x28;
            }
            a = M2C_FIELD(obj, u8 *, 0xB);
            lim = b;
            b = 2;
            if (lim + 0xA < a) {
                M2C_FIELD(obj, u8 *, 0xB) = 0;
                a = 8;
            } else {
                if (lim < a) {
                    M2C_FIELD(obj, u8 *, 0xB) = 0;
                    a = 8;
                } else {
                    a = D_800D9208;
                }
                b = 4;
            }
            func_8004F134(a, b);
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005E424);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005EADC);

void func_8005EB6C(s32 arg0, s32 arg1) {
    s32 d;
    s32 end;
    s32 mask;
    s32 n;

    d = (arg1 - arg0) & 0xFFF;
    if (d >= 0x800) {
        return;
    }
    end = (arg0 + d) & 0xFFF;
    if (arg0 < 0x800) {
        n = arg0 >> 6;
        mask = -1 << n;
        n = (end >> 6) + 1;
        if (n < 0x20) {
            mask &= ~(-1 << n);
        }
        D_800DE0D4 |= mask;
    } else if (end < 0x800) {
        n = (end >> 6) + 1;
        if (n < 0x20) {
            mask = ~(-1 << n);
        } else {
            mask = -1;
        }
        D_800DE0D4 |= mask;
    }
    if (0xFFF - end < 0x800) {
        n = (0xFFF - end) >> 6;
        mask = -1 << n;
        n = ((0xFFF - arg0) >> 6) + 1;
        if (n < 0x20) {
            mask &= ~(-1 << n);
        }
        D_800DE0C4 |= mask;
    } else if (0xFFF - arg0 < 0x800) {
        n = ((0xFFF - arg0) >> 6) + 1;
        if (n < 0x20) {
            mask = ~(-1 << n);
        } else {
            mask = -1;
        }
        D_800DE0C4 |= mask;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005EC84);

void func_8005ECB4(void *arg0, void *arg1) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x64);
    if (!(temp_v1 & 0x80)) {
        M2C_FIELD(arg0, s32 *, 0x64) = (s32) (temp_v1 | 0x80);
        M2C_FIELD(arg1, s16 *, 8) = 0x51;
        M2C_FIELD(arg0, u8 *, 0xA4) = (u8) (M2C_FIELD(arg0, u8 *, 0xA4) | 2);
        M2C_FIELD(arg0, s32 *, 0x4C) = (s32) (M2C_FIELD(arg0, s32 *, 0x4C) | 0x02000000);
    } else if (M2C_FIELD(arg1, s16 *, 8) == 0x1E) {
        func_8007D1E8(M2C_FIELD(arg0, s32 *, 0xC0), 3);
        M2C_FIELD(arg1, s16 *, 8) = (s16) ((u16) M2C_FIELD(arg1, s16 *, 8) - 1);
        M2C_FIELD(arg1, s32 *, 0x24) = (s32) (M2C_FIELD(arg1, s32 *, 0x24) | 0x20000);
    } else {
        M2C_FIELD(arg1, s16 *, 8) = (s16) ((u16) M2C_FIELD(arg1, s16 *, 8) - 1);
    }
    if (M2C_FIELD(arg0, s32 *, 0xC0) != 0) {
        func_8005B96C(arg0, M2C_FIELD(arg0, s32 *, 0x90), arg1);
    }
}

void func_8005ED88(void) {
    s32 *var_s0;
    s32 var_s1;

    func_8006B230(1, &D_800A8FE4);
    var_s0 = &D_800B05B0;
    var_s1 = 0;
    do {
        func_8005D630(var_s0);
        var_s1 += 1;
        *var_s0 &= ~1;
        var_s0 += 0x38 / 4;
    } while (var_s1 < 8);
}

void func_8005EDF8(void *arg0) {
    if ((D_800D2F8C == 2) && (arg0 != g_Player)) {
        if ((M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), s32 *, 0x2C) - M2C_FIELD(M2C_FIELD(arg0, void **, 0x90), s32 *, 0x2C)) >= 0x24B9) {
            M2C_FIELD(arg0, s32 *, 0x7C) = (s32) (M2C_FIELD(arg0, s32 *, 0x7C) | 0x80000000);
            return;
        }
        M2C_FIELD(arg0, s32 *, 0x7C) = (s32) (M2C_FIELD(arg0, s32 *, 0x7C) & 0x7FFFFFFF);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005EE6C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005EF7C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005F3B0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005F788);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005F93C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005FAA8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005FC4C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005FF60);

void func_80060094(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x4C) & 8) {
        func_80038C44(arg0);
        return;
    }
    if (M2C_FIELD(arg0, s32 *, 0x48) & 0x800) {
        func_80060798();
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800600EC);

void func_80060410(void) {
  M2C_UNK *new_var;
  M2C_UNK *new_var3;
  M2C_UNK *new_var2;
  if (new_var || new_var2)
  {
    new_var3 = &D_800B0C7C;
    new_var = &D_800DE8E8;
    new_var2 = new_var;
    func_8007F2EC(new_var3, new_var2);
  }
  else
  {
    new_var3 = &D_800B0C7C;
    new_var = &D_800DE8E8;
    new_var2 = new_var;
    func_8007F2EC(new_var3, new_var2);
  }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006043C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800604B0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80060588);

void func_8006060C(void) {
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80060614);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80060798);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800608E4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80060AC0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80060B5C);

void func_80060E0C(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x34) <= 0) {
        M2C_FIELD(arg0, s16 *, 0x56) = 0;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80060E28);

void func_80061018(void *arg0, s32 arg1) {
    s32 *temp_a0;
    s32 *temp_v0;

    temp_a0 = M2C_FIELD(arg0, s32 **, 0x118);
    *temp_a0 |= 0x80000;
    if (arg1 == g_Player) {
        temp_v0 = M2C_FIELD(arg0, s32 **, 0x118);
        *temp_v0 |= 0x200000;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80061068);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800615C8);

void func_80061700(void) {
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80061708);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800617AC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80061AB0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80061B70);

void func_80061DC0(void) {
    if ((g_CutScene == 0) && (D_8009CD50 == 0)) {
        func_8005D694();
        if (D_800F11F0 == 0x21) {
            M2C_FIELD(g_Player, s16 *, 0x98) = 0;
            M2C_FIELD(g_Player, s16 *, 0x9C) = 0;
            return;
        }
        if ((D_800DDDB4 != 0) && (D_800B0DDC == D_800B0DD4) && (func_8007A1B0() != 0)) {
            if (D_800B0DD4 == 3) {
                if (func_8007A1FC() == 0) {
                    func_8007A17C(0x21);
                }
            } else {
                func_8004DD9C();
            }
        } else {
            func_800617AC(g_Player);
            func_80061B70(g_Player);
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80061EC4);

void func_80061F38(void) {
  s16 new_var;
  s16 temp_v0;
  s16 var_s0;
  var_s0 = 0;
  D_8009D8B8 = &D_800DA4F8;
  new_var = var_s0;
  var_s0 = 0xE;
  var_s0 = ((s32) ((new_var << 6) << 10)) >> var_s0;
  do
  {
    *(var_s0 + (&D_800DF12C)) = rand();
    temp_v0 = var_s0 + 1;
    var_s0 = temp_v0;
  }
  while (temp_v0 < 0x32);
  func_800636B8();
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80061FB4);

void func_80062300(void) {
    GsGetLs(D_800D3238, D_800DF10C);
    gte_SetRotMatrix(D_800DF10C);
    gte_SetTransMatrix(D_800DF10C);
    func_80062394();
    func_80062784();
    func_80062B68();
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80062394);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80062784);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80062B68);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80062F38);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80062FAC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80063230);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800632AC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800633B0);

void func_800634C0(u8 *obj) {
    SVECTOR v;
    u8 *model;
    u8 *e;
    u8 *p;
    s32 i;
    if (obj[0x9B] & 8) {
        return;
    }
    model = M2C_FIELD(obj, u8 **, 0x74);
    if (model == NULL || model[4] == 0) {
        obj[0x9B] = 0;
        return;
    }
    e = M2C_FIELD(model, u8 **, 0x90) + M2C_FIELD(obj, s16 *, 0x62) * 0x8C;
    if (!(M2C_FIELD(e, s32 *, 0x88) & 1) && M2C_FIELD(obj, s16 *, 0x6A) != -1) {
        return;
    }
    for (i = 0; i < M2C_FIELD(obj, u8 **, 0x74)[0xB]; i++) {
        p = M2C_FIELD(M2C_FIELD(obj, u8 **, 0x74), u8 **, 0x90) + i * 0x8C;
        M2C_FIELD(p, s32 *, 0x10) = 0;
        GsGetLs(p + 0x10, D_800D40D8);
    }
    gte_SetRotMatrix(e + 0x34);
    ((SVECTOR *)D_1F8003E0)->vx = M2C_FIELD(obj, s16 *, 0x5C);
    ((SVECTOR *)D_1F8003E0)->vy = M2C_FIELD(obj, s16 *, 0x5E);
    ((SVECTOR *)D_1F8003E0)->vz = M2C_FIELD(obj, s16 *, 0x60);
    gte_ldv0(D_1F8003E0);
    gte_rtv0();
    gte_stsv(D_1F8003E0);
    M2C_FIELD(obj, s16 *, 0x54) = ((SVECTOR *)D_1F8003E0)->vx + M2C_FIELD(e, s32 *, 0x48);
    M2C_FIELD(obj, s16 *, 0x56) = ((SVECTOR *)D_1F8003E0)->vy + M2C_FIELD(e, s32 *, 0x4C);
    M2C_FIELD(obj, s16 *, 0x58) = ((SVECTOR *)D_1F8003E0)->vz + M2C_FIELD(e, s32 *, 0x50);
    func_8003EA24((SVECTOR *)(obj + 0x64), (s32 *)(e + 0x34), &v);
    M2C_FIELD(obj, s16 *, 0x6C) = (v.vx + 0x800) & 0xFFF;
    M2C_FIELD(obj, s16 *, 0x6E) = v.vy & 0xFFF;
    M2C_FIELD(obj, s16 *, 0x6A) = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800636B8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800637B0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80063858);

void func_80063ADC(void *arg0) {
    s32 *temp_a0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x2C);
    M2C_FIELD(arg0, s32 *, 0x1C) = (s32) (M2C_FIELD(arg0, s32 *, 0x1C) & ~1);
    if (temp_v1 != NULL) {
        M2C_FIELD(temp_v1, s32 *, 0x100) = 0;
    }
    temp_a0 = M2C_FIELD(arg0, s32 **, 0x30);
    if (temp_a0 != NULL) {
        *temp_a0 &= ~1;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80063B20);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80063CAC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80063D40);

void func_80063ED4(void *arg0) {
    void *temp_v0;

    if (arg0 != NULL) {
        temp_v0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(arg0, s8 *, 0) = 0;
        if (temp_v0 != NULL) {
            M2C_FIELD(temp_v0, s8 *, 0x9B) = 0;
        }
    }
}

void func_80063EF8(void *arg0) {
    if (!(M2C_FIELD(M2C_FIELD(arg0, void **, 0xC), s32 *, 0x98) & 0xFFFFFF)) {
        func_80063ED4(arg0);
    }
}

void func_80063F34(void *arg0, s32 arg1) {
  s32 temp_v0;
  unsigned char var_a2;
  u32 var_a1;
  temp_v0 = ((u32) (arg1 & 0x70)) >> 1;
  var_a1 = temp_v0 >> 3;
  if (var_a1 == 0)
  {
    var_a1 = 1;
  }
  temp_v0 = (var_a2 = (var_a2 = arg1 & 0xF));
  if (((u32) (temp_v0 - 1)) >= 6U)
  {
    var_a2 = 1;
  }
  func_80065614(arg0, (s32) var_a1, var_a2);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80063F84);

s32 *func_8006412C(void) {
    s16 i;
    s32 *p;

    i = 0;
    for (p = D_800DF540; i < 5; i++) {
        if (!(*p & 1)) {
            *p |= 1;
            return p;
        }
        p += 6;
    }
    return NULL;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006417C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006421C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006439C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800644DC);

void func_8006461C(void *arg0, s8 arg1, s32 arg2, s16 arg3) {
  s32 temp_v0;
  ((struct 
  {
    char pad[(4 > 0) ? (4) : (0)];
    typeof(*((s8 *) 0)) f;
  } *) (((char *) arg0) + ((4 < 0) ? (4) : (0))))->f = arg1;
  ((struct 
  {
    char pad[(5 > 0) ? (5) : (0)];
    typeof(*((s8 *) 0)) f;
  } *) (((char *) arg0) + ((5 < 0) ? (5) : (0))))->f = arg1;
  ((struct 
  {
    char pad[(6 > 0) ? (6) : (0)];
    typeof(*((s8 *) 0)) f;
  } *) (((char *) arg0) + ((6 < 0) ? (6) : (0))))->f = arg1;
  ((struct 
  {
    char pad[(0xC > 0) ? (0xC) : (0)];
    typeof(*((s16 *) 0)) f;
  } *) (((char *) arg0) + ((0xC < 0) ? (0xC) : (0))))->f = arg3;
  temp_v0 = (((struct 
  {
    char pad[(0 > 0) ? (0) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((0 < 0) ? (0) : (0))))->f & (~0xFE)) | ((arg2 & 0x7F) * 2);
  ((struct 
  {
    char pad[(0 > 0) ? (0) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((0 < 0) ? (0) : (0))))->f = temp_v0;
  ((struct 
  {
    char pad[(0xE > 0) ? (0xE) : (0)];
    typeof(*((s16 *) 0)) f;
  } *) (((char *) arg0) + ((0xE < 0) ? (0xE) : (0))))->f = arg3;
  ((struct 
  {
    char pad[(0 > 0) ? (0) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((0 < 0) ? (0) : (0))))->f = (s32) ((temp_v0 & (~0xF00)) | 0x100);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80064660);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006484C);

void func_80064A10(u8 *arg0) {
    u8 *obj;
    s32 d;
    u32 t;
    s32 amp;
    s32 *p;
    s32 x;
    s32 z;

    obj = *(u8 **)(arg0 + 0xC);
    d = 7;
    if (g_FrameRate != 20) {
        d = 140 / g_FrameRate;
    }
    if (*(s32 *)(obj + 0x94) & 1) {
        *(u16 *)(obj + 0x9C) += d;
    } else {
        *(u16 *)(obj + 0x9C) -= d;
    }
    d = -4;
    if (g_FrameRate != 20) {
        d = -80 / g_FrameRate;
    }
    *(u16 *)(obj + 0x56) += d;
    d = 4;
    if (g_FrameRate != 20) {
        d = 80 / g_FrameRate;
    }
    *(u16 *)(obj + 0x78) += d;
    *(u16 *)(obj + 0x7A) += d;
    d = 120;
    if (g_FrameRate != 20) {
        d = 2400 / g_FrameRate;
    }
    t = D_8009CDA8 - *(s32 *)(obj + 0x94);
    amp = ((s16)((t >> 4) + 25) * -(D_800A0370[(t * d) & 0xFFF] >> 16)) >> 12;
    if (*(s32 *)(obj + 0x94) & 1) {
        amp = -amp;
    }
    p = &D_800A0370[(D_800B0066 + 0x400) & 0xFFF];
    x = amp * *(s16 *)p;
    z = amp * (*(volatile s32 *)p >> 16);
    *(u16 *)(obj + 0x54) += x >> 12;
    *(u16 *)(obj + 0x58) += z >> 12;
}

void func_80064C28(u8 *arg0) {
    u8 *obj;
    s32 d;
    u32 t;
    s32 amp;
    s32 *p;
    s32 x;
    s32 z;

    obj = *(u8 **)(arg0 + 0xC);
    d = 8;
    if (g_FrameRate != 20) {
        d = 160 / g_FrameRate;
    }
    if (*(s32 *)(obj + 0x94) & 1) {
        *(u16 *)(obj + 0x9C) += d;
    } else {
        *(u16 *)(obj + 0x9C) -= d;
    }
    d = -10;
    if (g_FrameRate != 20) {
        d = -200 / g_FrameRate;
    }
    *(u16 *)(obj + 0x56) += d;
    d = 4;
    if (g_FrameRate != 20) {
        d = 80 / g_FrameRate;
    }
    *(u16 *)(obj + 0x78) += d;
    *(u16 *)(obj + 0x7A) += d;
    d = 80;
    if (g_FrameRate != 20) {
        d = 1600 / g_FrameRate;
    }
    t = D_8009CDA8 - *(s32 *)(obj + 0x94);
    amp = ((s16)((t >> 4) + 8) * -(D_800A0370[(t * d) & 0xFFF] >> 16)) >> 12;
    if (*(s32 *)(obj + 0x94) & 1) {
        amp = -amp;
    }
    p = &D_800A0370[(D_800B0066 + 0x400) & 0xFFF];
    x = amp * *(s16 *)p;
    z = amp * (*(volatile s32 *)p >> 16);
    *(u16 *)(obj + 0x54) += x >> 12;
    *(u16 *)(obj + 0x58) += z >> 12;
}

void func_80064E40(u8 *arg0) {
    u8 *obj;
    s32 d;
    u32 t;
    s32 amp;
    s32 *p;
    s32 x;
    s32 z;

    obj = *(u8 **)(arg0 + 0xC);
    d = 8;
    if (g_FrameRate != 20) {
        d = 160 / g_FrameRate;
    }
    if (*(s32 *)(obj + 0x94) & 1) {
        *(u16 *)(obj + 0x9C) += d;
    } else {
        *(u16 *)(obj + 0x9C) -= d;
    }
    d = -50;
    if (g_FrameRate != 20) {
        d = -1000 / g_FrameRate;
    }
    *(u16 *)(obj + 0x56) += d;
    d = 4;
    if (g_FrameRate != 20) {
        d = 80 / g_FrameRate;
    }
    *(u16 *)(obj + 0x78) += d;
    *(u16 *)(obj + 0x7A) += d;
    d = 80;
    if (g_FrameRate != 20) {
        d = 1600 / g_FrameRate;
    }
    t = D_8009CDA8 - *(s32 *)(obj + 0x94);
    amp = ((s16)((t >> 4) + 8) * -(D_800A0370[(t * d) & 0xFFF] >> 16)) >> 12;
    if (*(s32 *)(obj + 0x94) & 1) {
        amp = -amp;
    }
    p = &D_800A0370[(D_800B0066 + 0x400) & 0xFFF];
    x = amp * *(s16 *)p;
    z = amp * (*(volatile s32 *)p >> 16);
    *(u16 *)(obj + 0x54) += x >> 12;
    *(u16 *)(obj + 0x58) += z >> 12;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80065058);

void func_80065100(void *arg0) {
    s32 pos[3];
    s16 i;
    u8 *obj;
    void *p;
    void *sub;

    for (i = 0; i < M2C_FIELD(arg0, s16 *, 0x18); i++) {
        obj = M2C_FIELD(arg0, u8 **, 0x2C);
        if (obj == NULL) {
            func_8006B230(0, (s32) &D_800A944C);
        }
        func_80052CA8(pos, obj);
        D_1F8003D4 = pos[0];
        D_1F8003D8 = pos[1];
        D_1F8003DC = pos[2];
        p = func_80063CAC(arg0, *(s16 *)&D_1F8003D4, *(s16 *)&D_1F8003D8, *(s16 *)&D_1F8003DC, M2C_FIELD(arg0, s16 *, 0x1A));
        if (p == NULL) {
            return;
        }
        sub = func_80058694(M2C_FIELD(arg0, u8 *, 0x1D));
        M2C_FIELD(p, void **, 0xC) = sub;
        if (sub == NULL) {
            func_80063ED4(p);
            return;
        }
        M2C_FIELD(sub, u8 *, 0x9B) |= 4;
        M2C_FIELD(p, s32 *, 0x14) = -1;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s8 *, 0x98) = 100;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s8 *, 0x99) = 100;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s8 *, 0x9A) = 100;
        M2C_FIELD(p, s8 *, 0x2B) = 6;
        M2C_FIELD(p, s8 *, 0x1D) = 1;
        M2C_FIELD(p, s8 *, 0x28) = 0;
        M2C_FIELD(p, s8 *, 0x29) = 0;
        M2C_FIELD(p, s8 *, 0x2A) = 0;
        M2C_FIELD(p, s8 *, 0x1C) = 0;
        M2C_FIELD(p, s16 *, 4) = 0;
        M2C_FIELD(p, s16 *, 6) = 0;
        M2C_FIELD(p, s16 *, 8) = 0;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) = 45;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) = 45;
        if (M2C_FIELD(arg0, s16 *, 0x1E) != 0) {
            M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) = (M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
            M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) = (M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
        }
        M2C_FIELD(p, s32 *, 0x18) = M2C_FIELD(arg0, s32 *, 0x2C);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800652F0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80065510);

void func_80065588(void *arg0) {
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 4);
    M2C_FIELD(arg0, s32 *, 0) = (s32) (M2C_FIELD(arg0, s32 *, 0) & ~1);
    if (temp_v1 != NULL) {
        M2C_FIELD(temp_v1, s32 *, 0x10) = (s32) (M2C_FIELD(temp_v1, s32 *, 0x10) & ~1);
    }
}

void func_800655B8(void) {
    s16 i;

    for (i = 0; i < 3; i++) {
        D_800DF1F4[i] = &D_800DF5B8[i * 0x44];
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80065614);

void func_800658B4(void *arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 temp_a1;

    temp_a1 = arg1 >> 0x10;
    *arg2 = (s32) (temp_a1 * ((s32) (M2C_FIELD(arg0, s16 *, 0xA) - M2C_FIELD(arg0, s16 *, 8)) >> 1)) >> 0xB;
    *arg3 = (s32) (temp_a1 * ((s32) (M2C_FIELD(arg0, s16 *, 0xE) - M2C_FIELD(arg0, s16 *, 0xC)) >> 1)) >> 0xB;
    *arg4 = (s32) (temp_a1 * ((s32) (M2C_FIELD(arg0, s16 *, 0x12) - M2C_FIELD(arg0, s16 *, 0x10)) >> 1)) >> 0xB;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80065928);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80065A20);

s32 func_80066098(s16 *arg0, SVECTOR *arg1) {
    s32 out[4];
    SVECTOR pos;
    s32 x;
    s32 y;
    s32 z;
    s16 n;
    s32 i;

    if (arg0 == NULL) {
        func_8006B230(0, (s32) &D_800A9498);
    }
    n = 1 + (arg0[9] - arg0[8]) / 2000 + (arg0[7] - arg0[6]) / 2000 + (arg0[5] - arg0[4]) / 2000;
    if (n > 10) {
        n = 10;
    }
    for (i = 0; i < n; i++) {
        func_800658B4(arg0, (i << 12) / n, &x, &y, &z);
        pos.vx = arg1->vx + (x + ((arg0[5] + arg0[4]) >> 1));
        pos.vy = arg1->vy + (y + ((arg0[7] + arg0[6]) >> 1));
        pos.vz = arg1->vz + (z + ((arg0[9] + arg0[8]) >> 1));
        func_800669A4(&pos, out, arg0);
        func_8006439C(out[0], out[1], out[2], 7, 7, 1, 0xC8, 0x10, 0);
    }
    return 1;
}

s32 func_800662A8(s16 *arg0, SVECTOR *arg1) {
    s32 out[4];
    SVECTOR pos;
    s32 x;
    s32 y;
    s32 z;
    s16 n;
    s32 i;

    if (arg0 == NULL) {
        func_8006B230(0, (s32) &D_800A9498);
    }
    n = 1 + (arg0[9] - arg0[8]) / 2000 + (arg0[7] - arg0[6]) / 2000 + (arg0[5] - arg0[4]) / 2000;
    if (n > 10) {
        n = 10;
    }
    for (i = 0; i < n; i++) {
        func_800658B4(arg0, (i << 12) / n, &x, &y, &z);
        pos.vx = arg1->vx + (x + ((arg0[5] + arg0[4]) >> 1));
        pos.vy = arg1->vy + (y + ((arg0[7] + arg0[6]) >> 1));
        pos.vz = arg1->vz + (z + ((arg0[9] + arg0[8]) >> 1));
        func_800669A4(&pos, out, arg0);
        func_800644DC(out[0], out[1], out[2], 7, 7, 1, 0x64, 4, 0);
    }
    return 1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800664B8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80066704);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006692C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80066968);

void func_800669A4(SVECTOR *arg0, s32 *arg1, void *arg2) {
    gte_SetRotMatrix((u8 *)arg2 + 0x18);
    ((SVECTOR *)D_1F8003E0)->vx = arg0->vx;
    ((SVECTOR *)D_1F8003E0)->vy = arg0->vy;
    ((SVECTOR *)D_1F8003E0)->vz = arg0->vz;
    gte_ldv0(D_1F8003E0);
    gte_rtv0();
    gte_stlvnl(D_1F8003E8);
    arg1[0] = D_1F8003E8[0] + M2C_FIELD(arg2, s32 *, 0x38);
    arg1[1] = D_1F8003E8[1] + M2C_FIELD(arg2, s32 *, 0x3C);
    arg1[2] = D_1F8003E8[2] + M2C_FIELD(arg2, s32 *, 0x40);
}

void func_80066A60(void *arg0, s16 *arg1, s16 *arg2, s16 *arg3) {
    *arg1 = 0x640;
    *arg2 = 0x7D0;
    *arg3 = 0x640;
    if (((M2C_FIELD(arg0, s16 *, 0x12) - M2C_FIELD(arg0, s16 *, 0x10)) >= 0x1F41) || ((M2C_FIELD(arg0, s16 *, 0xA) - M2C_FIELD(arg0, s16 *, 8)) >= 0x1F41)) {
        *arg1 = (s16) ((s32) (*arg1 * 3) / 2);
        *arg2 = (s16) ((s32) (*arg2 * 3) / 2);
        *arg3 = (s16) ((s32) (*arg3 * 3) / 2);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80066B14);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800679AC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80067F18);

void func_800689F0(void) {
    M2C_FIELD(&D_800D8FF8, s8 *, 1) = 0x1B;
    M2C_FIELD(&D_800D8FF8, s16 *, 0x34) = 8;
    M2C_FIELD(&D_800D8FF8, s8 *, 0x36) = 0x3C;
    M2C_FIELD(&D_800D8FF8, s8 *, 0x49) = 0x20;
    M2C_FIELD(&D_800D8FF8, s8 *, 0x7E) = 0x5A;
    M2C_FIELD(&D_800D8FF8, s16 *, 0x7C) = 0x28;
    M2C_FIELD(&D_800D8FF8, u16 *, 2) = (u16) D_8009D26C;
    M2C_FIELD(&D_800D8FF8, s32 *, 4) = (s32) D_8009D274;
    M2C_FIELD(&D_800D8FF8, s32 *, 8) = (s32) D_8009D270;
    M2C_FIELD(&D_800D8FF8, u16 *, 0x4A) = (u16) D_8009D2A8;
    M2C_FIELD(&D_800D8FF8, s32 *, 0x4C) = (s32) D_8009D2B0;
    M2C_FIELD(&D_800D8FF8, s32 *, 0x50) = (s32) D_8009D2AC;
}

void func_80068A74(void) {
    D_8009D2B4 = 1;
    D_8009D2B8 = 2;
    D_8009D2D8 = 3;
    D_8009D2DC = 4;
    D_8009D2E0 = 5;
    D_8009D2E4 = 6;
    D_8009D2E8 = 7;
    D_8009D2EC = 8;
    D_8009D318 = 9;
    M2C_FIELD(g_Args, s32 *, 0x220) = 9;
}

void func_80068AF0(void) {
    func_8003C88C(0, D_8009D304);
    func_8003C88C(1, D_8009D304);
    func_8003C88C(2, D_8009D304);
    func_8003C88C(3, D_8009D304);
    func_8003C88C(4, D_8009D30C);
    func_8003C88C(5, D_8009D348);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80068B60);

void func_80068EB4(void) {
    M2C_FIELD(&D_800DDF08, s32 *, 0) = 0x8011292C;
    M2C_FIELD(&D_800DDF08, s32 *, 4) = 0x8011A52C;
    M2C_FIELD(&D_800DDF08, s32 *, 8) = 0x8012212C;
    M2C_FIELD(&D_800DDF08, s32 *, 0xC) = 0x80129D2C;
    M2C_FIELD(&D_800DDF08, s32 *, 0x10) = 0x8013192C;
    M2C_FIELD(&D_800DDF08, s32 *, 0x14) = 0x801394DC;
    M2C_FIELD(&D_800DDFEC, s32 *, 0) = 0x8011292C;
    M2C_FIELD(&D_800DDFEC, s32 *, 4) = 0x8011A52C;
    M2C_FIELD(&D_800DDFEC, s32 *, 8) = 0x8012212C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0xC) = 0x80129D2C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0x10) = 0x8013192C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0x14) = 0x801394DC;
}

void func_80068F28(void) {
    if (func_8002C168(&D_800A95BC, 0x8011292C) > 0x2F000) {
        printf(&D_800A95CC);
    }
    func_80068EB4();
    func_8005B508();
}

void func_80068F80(void) {
    M2C_FIELD(&D_800DDF08, s32 *, 0) = 0x8011292C;
    M2C_FIELD(&D_800DDF08, s32 *, 4) = 0x8011A52C;
    M2C_FIELD(&D_800DDF08, s32 *, 8) = 0x8012212C;
    M2C_FIELD(&D_800DDF08, s32 *, 0xC) = 0x80129D2C;
    M2C_FIELD(&D_800DDF08, s32 *, 0x10) = 0x8013192C;
    M2C_FIELD(&D_800DDF08, s32 *, 0x14) = 0x801394DC;
    M2C_FIELD(&D_800DDFEC, s32 *, 0) = 0x8011292C;
    M2C_FIELD(&D_800DDFEC, s32 *, 4) = 0x8011A52C;
    M2C_FIELD(&D_800DDFEC, s32 *, 8) = 0x8012212C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0xC) = 0x80129D2C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0x10) = 0x8013192C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0x14) = 0x801394DC;
}

void func_80068FF4(void) {
    if (func_8002C168(&D_800A95E4, 0x8011292C) > 0x2F000) {
        printf(&D_800A95CC);
    }
    func_80068F80();
    func_8005B508();
}

void func_8006904C(void) {
    func_8003C88C(0, D_8009D304);
    func_8003C88C(1, D_8009D304);
    func_8003C88C(2, D_8009D304);
    func_8003C88C(3, D_8009D304);
    func_8003C88C(4, D_8009D30C);
    func_8003C88C(5, D_8009D30C);
}

void func_800690B8(void) {
    M2C_FIELD(&D_800DDF08, s32 *, 0) = 0x8011292C;
    M2C_FIELD(&D_800DDF08, s32 *, 4) = 0x8011A52C;
    M2C_FIELD(&D_800DDF08, s32 *, 8) = 0x8012212C;
    M2C_FIELD(&D_800DDF08, s32 *, 0xC) = 0x80129D2C;
    M2C_FIELD(&D_800DDF08, s32 *, 0x10) = 0x8013192C;
    M2C_FIELD(&D_800DDF08, s32 *, 0x14) = 0x801394DC;
    M2C_FIELD(&D_800DDFEC, s32 *, 0) = 0x8011292C;
    M2C_FIELD(&D_800DDFEC, s32 *, 4) = 0x8011A52C;
    M2C_FIELD(&D_800DDFEC, s32 *, 8) = 0x8012212C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0xC) = 0x80129D2C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0x10) = 0x8013192C;
    M2C_FIELD(&D_800DDFEC, s32 *, 0x14) = 0x801394DC;
}

void func_8006912C(void) {
    if (func_8002C168(&D_800A95F4, 0x8011292C) > 0x2F000) {
        printf(&D_800A95CC);
    }
    func_800690B8();
    func_8005B508();
}

void func_80069184(void) {
    func_8003C88C(0, D_8009D304);
    func_8003C88C(1, D_8009D304);
    func_8003C88C(2, D_8009D304);
    func_8003C88C(3, D_8009D304);
    func_8003C88C(4, D_8009D30C);
    func_8003C88C(5, D_8009D308);
}

void func_800691F4(u32 arg0) {
    s16 temp_a2;
    s32 temp_s1;

    temp_s1 = func_80031F18();
    func_800303B4(0);
    func_80030F20();
    sprintf((s8 *) (g_Args->exePath), &D_800A9604, M2C_FIELD(g_Args, s16 *, 0x188), arg0);
    func_80030D08(5, temp_s1, g_Args->exePath);
    temp_a2 = M2C_FIELD(g_Args, s16 *, 0x188);
    sprintf((s8 *) (g_Args->exePath), &D_800A9614, temp_a2, temp_a2, arg0);
    func_80030994(g_Args->exePath);
    switch (arg0) {                                 /* irregular */
    case 1:
        func_80068F28();
        func_80068AF0();
        break;
    case 2:
        func_80068FF4();
        func_8006904C();
        break;
    case 3:
        func_8006912C();
        func_80069184();
        break;
    }
    func_800303B4(1);
}

void func_80069318(void) {
    D_800F11F0 = 3;
}

s32 func_80069328(void) {
    return 1;
}

void func_80069330(void) {
    D_8009D124 = 8;
    D_8009D126 = 0xA;
    D_800D86CC[0] = 0;
    D_800D86CC[3] = 0;
    D_800D86CC[4] = 0;
    D_800D86CC[5] = 0;
    D_800D86CC[6] = 0;
    D_800D86CC[7] = 0;
    D_8009D104 = D_8009D2A0;
    D_8009D106 = D_8009D29C;
    D_8009D108 = D_8009D2D0;
    D_8009D10A = D_8009D2D4;
    D_8009D130[0x74 / 4] = D_8009D2F0;
    D_8009D130[0x64 / 4] = D_8009D2F4;
    D_800D86CC[1] = D_8009D2F0;
    D_800D86CC[2] = D_8009D2F4;
}

void *func_800693DC(void *arg0) {
    void *ret;
    Weapon *w;
    s32 t;

    ret = NULL;
    if (M2C_FIELD(arg0, u8 *, 6) == D_8009D2F8) {
        w = &g_Weapons[2];
        t = func_8004CB2C(arg0, w);
        if (!(((u32)w->flags >> 11) & 1)) {
            w->flags |= 0x1800;
            func_8004C558(w, t);
            ret = &D_800A962C;
        } else {
            ret = func_8004C5C8(8, t);
        }
    }
    if (ret != NULL) {
        func_80026F30(8, 0);
    }
    return ret;
}

void func_80069470(void) {
}

void func_80069478(void) {
    D_800B0DD8 = 0;
    D_800B0DD4 = 0;
    D_800B0C58 = 0xB4;
    D_800B0C94 = 0x80;
    D_800B0C98 = 0x1770;
    D_800B0ADC = 0xC;
    D_800B0B54 = 0x578;
    D_800AFF5C = 0x834;
    D_800AFF60 = 0xA8C;
    D_800D2AF4[0] = 0;
    D_800D2AF4[1] = 0;
    D_800D2AF4[2] = 0;
    D_800B8FB0[0] = 0;
    D_800B8FB0[1] = 0;
    D_800B8FB0[2] = 0;
    D_800B8F28[0] = 1;
    D_800B8F28[1] = 1;
    D_800B8F28[2] = 1;
    D_800B0ACC = 0x82;
    D_800B0AD4 = 0x82;
    D_800B0AD0 = 0x82;
    D_800B0CA8 = 0x64;
    D_800B0CA0 = 0x64;
    D_800B0CA4 = 0x82;
    D_800B0AE8 = 0xDAC;
    D_800B0CAC = 0x848;
    D_8009CFA8 = 0;
    D_800B0AFC = 0;
    D_8009CD30 = 0;
    g_Args->playerX = 0;
    g_Args->playerY = 0;
    g_Args->playerZ = 0;
}

void func_8006959C(void) {
    s32 var_v0;

    var_v0 = func_8005D5C4(&D_800B05B0, &D_800B05B0 + 0x188);
    M2C_FIELD(g_Player, s32 *, 0x94) = var_v0;
    if (var_v0 == 0) {
        printf(&D_800A9644);
        var_v0 = exit(-1);
    }
    func_8005D58C(var_v0);
    D_800DDEE4 = &D_8009E4F0;
    D_800DDEE0 = &D_8009E6FC;
    func_8003331C(-1);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80069618);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80069684);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800697C8);

void func_8006981C(s32 arg0) {
  void *temp_v0;
  M2C_UNK *new_var;
  new_var = &D_800F10A8;
  temp_v0 = new_var;
  temp_v0 = (arg0 * 0x6C) + temp_v0;
  ((struct 
  {
    char pad[(0x68 > 0) ? (0x68) : (0)];
    typeof(*((u8 *) 0)) f;
  } *) (((char *) temp_v0) + ((0x68 < 0) ? (0x68) : (0))))->f = (u8) (((struct 
  {
    char pad[(0x68 > 0) ? (0x68) : (0)];
    typeof(*((u8 *) 0)) f;
  } *) (((char *) temp_v0) + ((0x68 < 0) ? (0x68) : (0))))->f ^ 1);
}

void func_80069850(s32 arg0) {
    s32 i;

    D_8009E414 = -1;
    D_8009E418 = -1;
    D_8009E41C = -1;
    if (arg0 == 0) {
        D_8009E410 = -1;
    }
    for (i = 0; i < 3; i++) {
        D_800E23DC[i] = 0x5B;
        D_800E23EC[i] = -1;
    }
}

M2C_UNK *func_800698B8(void) {
    return &D_8009E440;
}

s32 func_800698C4(void) {
    return 0x400;
}

void func_800698CC(void) {
    D_800AB5DC = &D_8009E420;
    D_800AB5B8 = &D_8009E430;
    D_800AB5D0 = &D_8009E428;
    D_800AB5CC = &D_8009E428;
    D_800AB5D4 = &D_8009E438;
    D_800AB5BC = &D_8009E420;
}

s32 func_80069920(void) {
    return 1;
}

void func_80069928(s32 arg0) {
    func_800698CC();
    if (arg0 == 1) {
        if (func_80053B40(1) == 0) {
            func_80054414(1, 0, 0);
        }
    }
    D_800E23F8 = 1;
    D_800E23D8 = 0;
    if (arg0 == 1) {
        func_8006A2B8();
    }
    func_80026C90(D_8009E410);
    func_80069850(1);
    D_800E02B8 = arg0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800699B8);

void func_80069AB4(void) {
    if (D_8009E414 < 0) {
        D_8009E414 = func_80026F30(0x5F, 0);
    } else {
        func_80027394(0, (s16) D_8009E414, 0x5F, 0);
    }
    if (D_8009E418 < 0) {
        D_8009E418 = func_80026F30(0x60, 0);
    } else {
        func_80027394(0, (s16) D_8009E418, 0x60, 0);
    }
    if (D_8009E410 < 0) {
        D_8009E410 = func_80026D08(0, 0);
        return;
    }
    func_800272A4((s16) D_8009E410, 0, 0);
}

void func_80069B74(void) {
    if ((func_80053B40(2) != 0) && (g_CutScene != 1)) {
        if (D_800E23D8 == 0) {
            if (D_800E23F8 != 0) {
                if (--D_800E23F8 == 0) {
                    func_8006A3E0();
                }
            }
        }
    }
    D_8009CAB4 = 0;
    D_8009CAB8 = 0xC4;
    func_8003801C();
    func_80069AB4();
}

s32 func_80069C00(void) {
    return D_8009D268;
}

void func_80069C10(void *arg0, void *arg1, s32 *arg2, s32 *arg3) {
  M2C_UNK var_a0;
  u8 temp_v1;
  void *temp_a0;
  temp_v1 = *((u8 *) (((s8 *) arg1) + 6));
  if (temp_v1 == D_8009D2F4)
  {
    var_a0 = 0x31;
    goto block_8;
  }
  if (temp_v1 == D_8009D2F0)
  {
    *arg2 = 1;
    goto block_6;
  }
  if (temp_v1 == D_8009D2F8)
  {
    *arg2 = 1;
    *((s8 *) (((s8 *) (*((void **) ((temp_a0 = (s8 *) arg0) + 0x94)))) + 2)) = 0x2C;
    block_6:
    temp_a0 = *((void **) (((s8 *) arg0) + 0x94));

    *((s32 *) (((s8 *) temp_a0) + 4)) = (s32) ((*((s32 *) (((s8 *) temp_a0) + 4))) | 0x10000);
    return;
  }
  var_a0 = 0x64;
  if (temp_v1 != D_8009D314)
  {
    block_8:
    func_80027248(var_a0, (*((s32 *) (((s8 *) arg1) + 0x90))) + 0x14, 0);

  }
  *arg3 = 1;
}

void func_80069CD4(s32 *arg0) {
    if ((*arg0 == g_Player) && (func_80053B40(2) == 0)) {
        func_80054414(2, 1, 0);
    }
}

s32 func_80069D20(void) {
    return D_8009D308;
}

s32 func_80069D30(void) {
    D_8009DB3C = 0;
    return 0;
}

s32 func_80069D40(void) {
    return D_8009D30C;
}

s32 func_80069D50(void) {
    return D_800E02B8;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80069D60);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006A0D8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006A174);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006A2B8);

void func_8006A3E0(void) {
    if (func_80039A9C(0, D_8009D2FC, 3) != 0) {
        func_8006A2B8();
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006A41C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006A700);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006A75C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006A7C4);

void func_8006A828(void *arg0) {
  ((struct 
  {
    char pad[(0xA0 > 0) ? (0xA0) : (0)];
    typeof(*((s16 *) 0)) f;
  } *) (((char *) arg0) + ((0xA0 < 0) ? (0xA0) : (0))))->f = 0;
  ((struct 
  {
    char pad[(0x50 > 0) ? (0x50) : (0)];
    typeof(*((u16 *) 0)) f;
  } *) (((char *) arg0) + ((0x50 < 0) ? (0x50) : (0))))->f = (u16) D_8009E4A4;
  ((struct 
  {
    char pad[(0x52 > 0) ? (0x52) : (0)];
    typeof(*((u16 *) 0)) f;
  } *) (((char *) arg0) + ((0x52 < 0) ? (0x52) : (0))))->f = (u16) D_8009E4C0;
  ((struct 
  {
    char pad[(0xA5 > 0) ? (0xA5) : (0)];
    typeof(*((s8 *) 0)) f;
  } *) (((char *) arg0) + ((0xA5 < 0) ? (0xA5) : (0))))->f = 5;
}

void func_8006A850(void *arg0) {
  ((struct 
  {
    char pad[(0xA0 > 0) ? (0xA0) : (0)];
    typeof(*((s16 *) 0)) f;
  } *) (((char *) arg0) + ((0xA0 < 0) ? (0xA0) : (0))))->f = 0;
  ((struct 
  {
    char pad[(0x6C > 0) ? (0x6C) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((0x6C < 0) ? (0x6C) : (0))))->f = 0;
  ((struct 
  {
    char pad[(0x50 > 0) ? (0x50) : (0)];
    typeof(*((u16 *) 0)) f;
  } *) (((char *) arg0) + ((0x50 < 0) ? (0x50) : (0))))->f = (u16) D_8009E4A8;
  ((struct 
  {
    char pad[(0x52 > 0) ? (0x52) : (0)];
    typeof(*((u16 *) 0)) f;
  } *) (((char *) arg0) + ((0x52 < 0) ? (0x52) : (0))))->f = (u16) D_8009E4C0;
  ((struct 
  {
    char pad[(0xA5 > 0) ? (0xA5) : (0)];
    typeof(*((s8 *) 0)) f;
  } *) (((char *) arg0) + ((0xA5 < 0) ? (0xA5) : (0))))->f = 6;
}

void func_8006A87C(void *arg0) {
  ((struct 
  {
    char pad[(0xA0 > 0) ? (0xA0) : (0)];
    typeof(*((s16 *) 0)) f;
  } *) (((char *) arg0) + ((0xA0 < 0) ? (0xA0) : (0))))->f = 0;
  ((struct 
  {
    char pad[(0x6C > 0) ? (0x6C) : (0)];
    typeof(*((s32 *) 0)) f;
  } *) (((char *) arg0) + ((0x6C < 0) ? (0x6C) : (0))))->f = 0;
  ((struct 
  {
    char pad[(0x50 > 0) ? (0x50) : (0)];
    typeof(*((u16 *) 0)) f;
  } *) (((char *) arg0) + ((0x50 < 0) ? (0x50) : (0))))->f = (u16) D_8009E4A8;
  ((struct 
  {
    char pad[(0x52 > 0) ? (0x52) : (0)];
    typeof(*((u16 *) 0)) f;
  } *) (((char *) arg0) + ((0x52 < 0) ? (0x52) : (0))))->f = (u16) D_8009E4C0;
  ((struct 
  {
    char pad[(0xA5 > 0) ? (0xA5) : (0)];
    typeof(*((s8 *) 0)) f;
  } *) (((char *) arg0) + ((0xA5 < 0) ? (0xA5) : (0))))->f = 7;
}

void func_8006A8A8(void *arg0) {
  u16 new_var;
  D_8009E4C0 = D_8009E4C0;
  *((s32 *) (((s8 *) arg0) + 0x8C)) = 0;
  new_var = D_8009E4C0;
  *((s16 *) (((s8 *) arg0) + 0x50)) = 0;
  *((s16 *) (((s8 *) arg0) + 0xA0)) = 0;
  *((s32 *) (((s8 *) arg0) + 0x6C)) = 0;
  *((s8 *) (((s8 *) arg0) + 0xA5)) = 8;
  *((u16 *) (((s8 *) arg0) + 0x52)) = (u16) new_var;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006A8D0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006AA20);

void func_8006AF88(void) {
  D_8009D27C = 1;
  D_8009D2D0 = 2;
  D_8009D2D4 = 3;
  D_8009D328 = 4;
  D_8009D32C = 5;
  D_8009D330 = 6;
  D_8009D2F8 = 7;
  D_8009D2A8 = 8;
  D_8009D260 = 9;
  D_8009D348 = 0xA;
  D_8009D268 = 0xB;
  D_8009D280 = 0xC;
  D_8009D310 = 0xD;
  D_8009D344 = 0xE;
  D_8009D358 = 0xF;
  D_8009D334 = 0x10;
  D_8009D338 = 0x11;
  D_8009D308 = 0x12;
  D_8009D26C = 0x13;
  D_8009D314 = 0x14;
  D_8009D34C = 0x15;
  D_8009D2F4 = 0x16;
  D_8009D300 = 0x17;
  D_8009D2CC = 0x18;
  D_8009D2BC = 0x19;
  D_8009D29C = 0x1A;
  D_8009D320 = 0x1B;
  D_8009D2A0 = 0x1C;
  D_8009D340 = 0x1D;
  D_8009D350 = 0x1E;
  D_8009D2FC = 0x1F;
  D_8009D30C = 0x20;
  D_8009D324 = 0x21;
  D_8009D33C = 0x22;
  D_8009D304 = 0x23;
  D_8009D354 = 0x24;
  D_8009D31C = 0x25;
  D_8009D2F0 = 0x26;
  D_8009D35C = 0x27;
  *((s32 *) (((s8 *) g_Args) + 0x214)) = 0x27;
}

void func_8006B16C(void) {
    func_8002C168(&D_800A96D4, 0x80101F6C);
}

void func_8006B198(void) {
    D_8009D274 = 1;
    D_8009D2B0 = 2;
}

void func_8006B1B4(void) {
    D_8009D270 = 1;
    D_8009D2AC = 2;
    M2C_FIELD(g_Args, s32 *, 0x21C) = 2;
}

void func_8006B1DC(s32 arg0) {
    s32 temp_s1;

    temp_s1 = func_800303B4(0);
    printf(&D_800A970C, arg0);
    if (temp_s1 != 0) {
        func_800303B4(1);
    }
}

void func_8006B230(s32 arg0, M2C_UNK arg1) {
    if (arg0 == 0) {
        func_800303B4(0);
        printf(&D_800A9710, arg1);
        exit(-1);
    }
}

s16 func_8006B274(int arg0) {
  s16 *new_var;
  return ratan2(arg0, SquareRoot12(0x1000 - ((*(new_var = &arg0)) * (((s32) (arg0 << 0x10)) >> 0x1C))));
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006B2C4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006C494);

void func_8006C8F4(u8 *arg0, u8 *arg1, s32 arg2) {
    SVECTOR tw;
    u8 *cur;
    u8 *rec;
    u32 *p0;
    u32 *p1;
    u32 *prim;
    u32 *ot;
    s32 u0, u1, u2;
    s32 a, b, c;
    s32 d;

    rec = *(u8 **)(arg1 + 0x24) + arg2 * 0x14;
    prim = *(u32 **)(arg1 + 0x20);
    if (*(s16 *)rec < 0) {
        return;
    }
    cur = *(u8 **)(arg1 + 0x10);
    p0 = (u32 *)cur;
    cur += 0x18;
    p1 = p0 + 3;
    if (*(u8 **)(arg1 + 0x14) < cur) {
        return;
    }
    *(u8 **)(arg1 + 0x10) = cur;
    *(s32 *)&tw.vx = ((s32 *)rec)[0];
    *(s32 *)&tw.vz = ((s32 *)rec)[1];
    ((u8 *)p0)[3] = 2;
    p0[1] = 0xE2000000 | (((tw.vy & 0xFF) >> 3) << 15) | (((tw.vx & 0xFF) >> 3) << 10) | ((((-tw.pad) & 0xFF) >> 3) << 5) | (((-tw.vz) & 0xFF) >> 3);
    p0[2] = 0;
    *(s32 *)&tw.vx = 0;
    *(s32 *)&tw.vz = 0;
    ((u8 *)p1)[3] = 2;
    p1[1] = 0xE2000000 | (((tw.vy & 0xFF) >> 3) << 15) | (((tw.vx & 0xFF) >> 3) << 10) | ((((-tw.pad) & 0xFF) >> 3) << 5) | (((-tw.vz) & 0xFF) >> 3);
    p1[2] = 0;
    p0[3] = (p0[3] & 0xFF000000) | (*prim & 0xFFFFFF);
    *prim = (*prim & 0xFF000000) | ((u32)p1 & 0xFFFFFF);
    ot = *(u32 **)(arg1 + 0x1C);
    *p0 = (*p0 & 0xFF000000) | *ot;
    *ot = (u32)p0 & 0xFFFFFF;
    u0 = *(u16 *)(arg0 + 0xC);
    u1 = *(u16 *)(arg0 + 0xE);
    u2 = *(u16 *)(arg0 + 0x10);
    a = u0 & 0xFF;
    b = u1 & 0xFF;
    c = u2 & 0xFF;
    d = *(s16 *)(rec + 0xC);
    a += d;
    b += d;
    c += d;
    if ((a | b | c) & 0x100) {
        a -= 0x80;
        b -= 0x80;
        c -= 0x80;
        if ((a | b | c) < 0) {
            a = 0;
            b = 0;
            c = 0;
        }
    }
    u0 = (u0 & 0xFF00) | a;
    u1 = (u1 & 0xFF00) | b;
    u2 = (u2 & 0xFF00) | c;
    a = u0 >> 8;
    b = u1 >> 8;
    c = u2 >> 8;
    d = *(s16 *)(rec + 0xE);
    a += d;
    b += d;
    c += d;
    if ((a | b | c) & 0x100) {
        a -= 0x80;
        b -= 0x80;
        c -= 0x80;
        if ((a | b | c) < 0) {
            a = 0;
            b = 0;
            c = 0;
        }
    }
    a <<= 8;
    b <<= 8;
    c <<= 8;
    u0 = (u0 & 0xFF) | a;
    u1 = (u1 & 0xFF) | b;
    u2 = (u2 & 0xFF) | c;
    *(s16 *)((u8 *)prim + 0xC) = u0;
    *(s16 *)((u8 *)prim + 0x18) = u1;
    *(s16 *)((u8 *)prim + 0x24) = u2;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006CB88);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006CEA4);

u32 func_8006D3C0(SVECTOR *a, SVECTOR *b) {
    u32 dx;
    u32 dy;
    u32 dz;

    dx = b->vx - a->vx;
    if ((s32)dx < 0) {
        dx = a->vx - b->vx;
    }
    dy = b->vy - a->vy;
    if ((s32)dy < 0) {
        dy = a->vy - b->vy;
    }
    dz = b->vz - a->vz;
    if ((s32)dz < 0) {
        dz = a->vz - b->vz;
    }
    if (dx < dy) {
        dx ^= dy;
        dy ^= dx;
        dx ^= dy;
    }
    if (dx < dz) {
        dx ^= dz;
        dz ^= dx;
        dx ^= dz;
    }
    if (dy < dz) {
        dy ^= dz;
        dz ^= dy;
        dy ^= dz;
    }
    dz += dy;
    dy >>= 3;
    dz >>= 2;
    return dx + dy + dz;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006D470);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006DD20);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006E054);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006E620);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006E9E0);

void func_8006ECBC(void) {
    u8 *m;
    u8 *buf0;
    u8 *buf1;
    s32 n;
    u32 *colors;
    u16 *f;
    s32 i;
    u8 *p;
    s32 j;
    u8 c[4];
    s32 flag;
    s32 v;
    u32 c0, c1, c2;

    flag = D_8009CABE;
    m = D_800E2DA8;
    buf0 = *(u8 **)(m + 0x30);
    buf1 = *(u8 **)(m + 0x34);
    n = *(s32 *)(m + 4);
    colors = *(u32 **)(m + 0x1C);
    func_8007F0D8(0);
    f = *(u16 **)(m + 0x20);
    for (i = 0; i < n; ) {
        j = f[0];
        *(u32 *)c = colors[f[1]];
        if (c[3] == 0) {
            if (flag) {
                v = 12;
            } else {
                v = (c[0] + c[1] + c[2]) >> 3;
            }
            c[0] = 0;
            c[1] = v + 0x30;
            c[2] = 0;
        }
        c0 = *(u32 *)c;
        *(u32 *)c = colors[f[2]];
        if (c[3] == 0) {
            if (flag) {
                v = 12;
            } else {
                v = (c[0] + c[1] + c[2]) >> 3;
            }
            c[0] = 0;
            c[1] = v + 0x30;
            c[2] = 0;
        }
        c1 = *(u32 *)c;
        *(u32 *)c = colors[f[3]];
        if (c[3] == 0) {
            if (flag) {
                v = 12;
            } else {
                v = (c[0] + c[1] + c[2]) >> 3;
            }
            c[0] = 0;
            c[1] = v + 0x30;
            c[2] = 0;
        }
        c2 = *(u32 *)c;
        p = buf0 + j * 40;
        c0 |= p[7] << 24;
        *(u32 *)(p + 4) = c0;
        *(u32 *)(p + 0x10) = c1;
        *(u32 *)(p + 0x1C) = c2;
        p = buf1 + j * 40;
        *(u32 *)(p + 4) = c0;
        *(u32 *)(p + 0x10) = c1;
        *(u32 *)(p + 0x1C) = c2;
        f[4] &= 0x9FFF;
        i++;
        f += 10;
    }
}

void func_8006EEA8(void) {
    u8 *m;
    u8 *buf0;
    u8 *buf1;
    s32 n;
    u32 *colors;
    u16 *f;
    s32 i;
    u8 *p;
    s32 j;
    u8 c[4];
    u32 c0, c1, c2;

    if (D_8009CABD != 0) {
        func_8006ECBC();
        return;
    }
    m = D_800E2DA8;
    buf0 = *(u8 **)(m + 0x30);
    buf1 = *(u8 **)(m + 0x34);
    n = *(s32 *)(m + 4);
    colors = *(u32 **)(m + 0x1C);
    func_8007F0D8(0);
    f = *(u16 **)(m + 0x20);
    for (i = 0; i < n; ) {
        j = f[0];
        *(u32 *)c = colors[f[1]];
        if (c[3] == 0) {
            c[0] = 0x18;
            c[1] = 0x18;
            c[2] = 0x18;
        }
        c0 = *(u32 *)c;
        *(u32 *)c = colors[f[2]];
        if (c[3] == 0) {
            c[0] = 0x18;
            c[2] = 0x18;
            c[1] = 0x18;
        }
        c1 = *(u32 *)c;
        *(u32 *)c = colors[f[3]];
        if (c[3] == 0) {
            c[0] = 0x18;
            c[2] = 0x18;
            c[1] = 0x18;
        }
        c2 = *(u32 *)c;
        p = buf0 + j * 40;
        c0 |= p[7] << 24;
        *(u32 *)(p + 4) = c0;
        *(u32 *)(p + 0x10) = c1;
        *(u32 *)(p + 0x1C) = c2;
        p = buf1 + j * 40;
        *(u32 *)(p + 4) = c0;
        *(u32 *)(p + 0x10) = c1;
        *(u32 *)(p + 0x1C) = c2;
        f[4] &= 0x9FFF;
        i++;
        f += 10;
    }
}

void func_8006F034(void) {
    u8 *m;
    u8 *buf0;
    u8 *buf1;
    s32 n;
    u32 *colors;
    u16 *f;
    s32 i;
    u8 *p;
    s32 j;
    u32 c0, c1, c2;

    if (D_8009CABD != 0) {
        func_8006ECBC();
        return;
    }
    if (D_8009CABE != 0) {
        func_8006EEA8();
        return;
    }
    m = D_800E2DA8;
    buf0 = *(u8 **)(m + 0x30);
    buf1 = *(u8 **)(m + 0x34);
    n = *(s32 *)(m + 4);
    colors = *(u32 **)(m + 0x1C);
    func_8007F0D8(0);
    f = *(u16 **)(m + 0x20);
    for (i = 0; i < n; ) {
        i++;
        j = f[0];
        p = buf0 + j * 40;
        c0 = colors[f[1]];
        c1 = colors[f[2]];
        c2 = colors[f[3]];
        c0 |= p[7] << 24;
        *(u32 *)(p + 4) = c0;
        *(u32 *)(p + 0x10) = c1;
        *(u32 *)(p + 0x1C) = c2;
        p = buf1 + j * 40;
        *(u32 *)(p + 4) = c0;
        *(u32 *)(p + 0x10) = c1;
        *(u32 *)(p + 0x1C) = c2;
        f[4] &= 0x9FFF;
        f += 10;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006F168);

void func_8006F6AC(void) {
    D_800E2438 = 0;
    gte_SetRotMatrix(D_800B0018);
    gte_SetTransMatrix(D_800B0018);
    func_8006E054();
    gte_SetRotMatrix(D_800B0808);
    gte_SetTransMatrix(D_800B0808);
    func_8006E9E0(D_800E2DBC);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8006F784);

void func_8006F9A4(s32 arg0) {
    struct { u8 c[4]; } *col;
    s32 *p;
    s32 n;

    if (arg0 != 0) {
        col = (void *)D_800B8FB0;
        *col = *(__typeof__(col))arg0;
        if (M2C_FIELD(arg0, s32 *, 0x14) != 0) {
            M2C_FIELD(arg0, s32 *, 0x14) = arg0 + M2C_FIELD(arg0, s32 *, 0x14);
            p = M2C_FIELD(arg0, s32 **, 0x14);
            n = M2C_FIELD(arg0, s32 *, 4) * M2C_FIELD(arg0, s32 *, 8);
            while (--n != -1) {
                p[1] = arg0 + p[1];
                p += 2;
            }
        }
        D_800E2E48 = M2C_FIELD(arg0, s32 *, 8) * M2C_FIELD(arg0, s32 *, 0xC);
        return;
    }
    D_800B8FB0[0] = 0;
    D_800B8FB0[1] = 0;
    D_800B8FB0[2] = 0;
}
