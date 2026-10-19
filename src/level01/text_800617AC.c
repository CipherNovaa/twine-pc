#include "twine.h"

extern char D_800A7D74[];
void func_80052BEC(void);
void *func_800390F8(s32 arg0);
void func_80060094(void *arg0);
void func_80063F34(void *arg0, s32 arg1);
void func_80058B10(void *);
void func_800391B0(void *, s32);


extern u8 D_800DD7F4[];
extern s32 D_800AFF88;


void RotMatrixYXZ_gte(void *, void *);
void func_800634C0(u8 *obj);
typedef struct { s32 m[8]; } Mat32;


typedef struct { u32 active : 1; u32 life : 5; u32 pad6 : 6; u32 state : 4; u32 pad16 : 16; s32 pad4[4]; u32 start; } Spark;
void func_80065588(void *arg0);
s32 func_80066098(s16 *arg0, SVECTOR *arg1);




typedef struct { s32 active; s32 cursor; s32 count[12]; s32 vals[12]; } DevMenu;
extern u8 D_8009CD6C;


extern s32 D_800B8EFC[][5];
extern u8 D_8009D00E;
extern u8 D_8009CFBB;


typedef struct { u8 unk0; u8 active; u8 pad2[2]; s32 unk4; u8 pad8[0x10]; s32 time; u8 *sub; s32 unk20; u8 pad24[0x10]; } EffSlot;
void func_8004C498(s32, u8 *);


typedef struct { s32 x, y, z; s32 unkC, unk10, unk14, unk18; u32 flags; s32 unk20, unk24, unk28, unk2C, unk30; } Ent34;
extern Ent34 D_800DF200[];


typedef struct { s32 count; u32 *items[4]; } ScriptStack;
extern char D_800A8C08[];


short func_8008C5D8(s32 *in, s32 *out);


extern s32 D_8009D038;
extern s32 D_8009D084;
extern s32 D_8009D088;
typedef struct { u32 addr : 24; u32 len : 8; } PTag;


extern u8 D_800D7C24[][0x10];


extern u8 D_800D3288[];


extern s16 D_800AC4E0;
extern s16 D_800AC4E4;


extern s16 D_800D4480[];
extern u8 D_800DDC70[];
extern s32 D_800DDDC0;
extern s16 D_8009D8F0;
extern s16 D_800DE0D0;
void func_80049470(u8 *obj, s16 *out);
void func_8004973C(VECTOR v, s16 *out);


extern s32 D_800B8EFC[][5];
extern u32 D_800B0EFC[][0x1000];
extern u32 D_800D28F4[][0x40];


typedef struct { u32 count; u32 *table; u32 idx; s32 color; s32 timer; } ColCycle;
typedef struct { s16 flags; s16 pad2[3]; u16 du; u16 dv; s16 u; s16 v; u16 au; u16 av; } UvScroll;
typedef struct { s32 nCol; s32 nUv; ColCycle *col; s32 padC; UvScroll *uv; } FxHdr;
extern s32 D_8009EEDC;
extern s32 (*D_8009CE48)(s32);
extern s32 D_8009CAC0;


s32 func_800698C4(void);
void func_8003732C(u8 *, s32);
s16 func_80037768(u8 *obj, u8 *proj);
void func_80038C8C(u8 *obj);
void func_80062F38(u8 *a, u8 *b);
void func_800632AC(u8 *arg0, s32 arg1);


extern s32 D_8009CF50;
extern s32 D_800D31BC;
s32 func_80050A10(VECTOR *, VECTOR *, u8 *, void *);
void func_80049A88(void);


typedef struct { u32 flags; s32 w4; u32 w5; } SpawnInfo;
typedef struct { s32 x; s32 y; s32 z; SpawnInfo info; } SpawnEnt;
typedef struct { u8 pad0[5]; u8 type5; u8 pad6[2]; u8 unk8; u8 pad9[0x2B]; s32 unk34; u8 pad38[0x10]; s32 flags48; u8 pad4C[0x14]; SpawnInfo *src; u32 path : 7; u32 b64 : 22; u32 idx : 3; u32 f68 : 4; u32 b68 : 12; s16 scale; u8 pad6C[0x24]; u8 *parts; u8 pad94[0x10]; u8 stateA4; u8 padA5[0x27]; u8 unkCC; u8 unkCD; u8 padCE[0x56]; } SmartObj;
typedef struct { u8 pad0[3]; u8 state; s16 unk4; s16 unk6; s32 x; s32 y; s32 z; s16 unk14; s16 unk16; s16 unk18; s16 pad1A; SpawnInfo *src; } Rec20;
typedef struct { s16 x; s16 y; s16 z; s16 pad6; s32 *src; s32 unkC; u32 active : 1; u32 f1 : 2; u32 f3 : 3; u32 f6 : 4; u32 f10 : 1; u32 f11 : 3; } SpawnRec;
extern Rec20 D_800B0CB0[];
extern char D_800B0BC0[];
extern char D_800A7F5C[];
u8 *func_800333D4(void);
s32 func_8003C6B0(s32 arg0, s32 arg1, s32 arg2);
void func_8003FCE4(s32, SpawnInfo *, s32, s32, s32, s32);
void func_80053790(u8 *, s32);
void func_8002DD6C(u8 *obj);


extern u8 D_8009CA5C;
extern u16 D_8009CA6C;
extern s16 D_8009CAB0;
extern void (*D_8009CDD4)(void);
extern void (*D_8009CDD8)(void);
extern s16 D_800B0C6C;
extern s8 D_800B0E48;
M2C_UNK func_8002847C();
void func_8002BDE4(void);
M2C_UNK func_8002E798();
void func_8002F1BC(void);
void func_8002F1F4(void);
void func_8002FE04(void);
void func_80031F28(void);
void func_80031F54(s32 arg0);
void func_80032024(void);
M2C_UNK func_80032074(s16);
M2C_UNK func_800332A4();
void func_8003B4F4(void);
void func_8003FC60(void);
void func_800478C8(void);
void func_8004B324(void);
M2C_UNK func_8004BDA8();
M2C_UNK func_8004D048();
void func_8004F3D8(void);
void func_800530F4(void);
M2C_UNK func_80057C40();
void func_8005ED88(void);
void func_80075290(void);
M2C_UNK func_800761DC();


s16 func_80044AF4(void);
s32 func_8004A370(void);
extern char D_800A7800[];


extern s16 D_800D43CC;


extern s32 D_8009CF60;


extern s16 D_800B0AB8;
extern s16 D_800B0AC0;
extern s16 D_8009CD5E;
extern s16 D_8009CD60;
extern s32 D_8009CD58;
s32 func_8004DFF8(s32 *);
s32 func_80040AA8(s32 *);
void func_800503D8(s32, s32, s32, s32);


extern u8 D_800B0848;
extern u8 D_800DE004[];


void func_8007CE30(s32, u8);


typedef struct { u32 tag; u8 r0, g0, b0, code; s16 x0, y0; s16 w, h; } TILE;
typedef struct { s32 hdr; TILE t[12][2]; } TileGrid;
extern TILE D_800DD818[];
void func_80081B74(void *);


typedef struct { u8 pad0[4]; s32 voice : 16; u32 active : 1; u32 timer : 10; u32 rest : 5; } ChrSnd;


extern char D_800A8464[];
extern char D_800A8468[];
extern char D_800A8478[];
extern char D_800A8490[];
extern char D_800A849C[];
extern char D_800A84A8[];
extern char D_800A84B4[];
extern char D_800A84B8[];
extern s32 D_800D43A8;
extern s32 D_800D43C0;
extern s32 D_800D43D0;
extern s32 D_800D43E4;
extern s32 D_800D43F8;
extern s32 D_800D4400;
extern s32 D_800D4440;
extern s32 D_800D4450;
extern s32 D_800D4458;
extern s32 D_800D445C;
extern s32 D_800D4464;
void func_800425E4(void);


extern char D_800A8C48[];
s32 func_80057AB0(s32 arg0);
void func_80057AD8(void *arg0, s16 arg1, s16 arg2);


typedef struct { s16 unk0; s16 unk2; u32 unk4 : 16; u32 unk6 : 8; u32 unk7 : 2; u32 pad7 : 6; s32 unk8; s32 unkC; } SndCh;
extern SndCh D_800D31C0;
extern SndCh D_800D31D0;


typedef struct { char s[5]; } Str5;
extern Str5 D_800A770C;
extern char D_800D2F48[4][16];


extern u8 D_800B8FB4[][0x8CA0];
extern u8 *D_800FC640;


void func_8007F25C(s16 *, s32, s32, s32);


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
void func_8006CEA4(s32 arg0);
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
void func_8005BA30(void *obj);
void func_8005B7BC(void *obj, s32 arg1);


extern s32 D_8009D020;
extern s32 D_8009D024;
extern s32 D_8009D080;


extern char D_800A80A4[];
extern s32 D_800D40D8[];
void func_8003EA24(SVECTOR *arg0, s32 *arg1, SVECTOR *arg2);


typedef struct { u32 tag; u8 r0, g0, b0, code; s16 x0, y0; u8 u0, v0; u16 clut; s16 x1, y1; u8 u1, v1; u16 tpage; s16 x2, y2; u8 u2, v2; u16 pad1; s16 x3, y3; u8 u3, v3; u16 pad2; } POLY_FT4;
extern POLY_FT4 D_800D4984[2][32][2];
extern u8 D_8009D040[][8];
s16 GetClut(s32, s32);
void func_80044B74(void *arg0, s8 arg1, s8 arg2, s8 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);




extern s32 D_800E2438;
extern s32 D_800B0018[];
extern s32 D_800E2DBC;
void func_8006E054(void);
void func_8006E9E0(s32 arg0);


extern u8 D_1F8003E0[];


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
void *func_80063CAC(void *obj, s16 x, s16 y, s16 z, s16 arg4);
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


Ent34 *func_800644DC(s16 x, s16 y, s16 z, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, u8 arg8);


extern s32 D_800A9498;
void func_800658B4(void *arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4);
void func_800669A4(SVECTOR *arg0, s32 *arg1, void *arg2);
Ent34 *func_8006439C(s16 x, s16 y, s16 z, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, u8 arg8);


void func_80052CA8(s32 *out, u8 *obj);
Ent34 *func_80065058(s16 x, s16 y, s16 z, void *obj, s16 arg4);


typedef struct { u8 active; u8 type; s16 unk2; s32 unk4; s32 animIdx; s32 unkC; s32 startTime[8]; u8 *obj; u16 flags; u8 duration; u8 pad37[9]; u8 enable[8]; } AnimEnt;
typedef struct { AnimEnt e[1]; } AnimEntArr;
extern M2C_UNK D_800A887C;


extern s32 D_800A0370[];
void func_8005ADD4(void *, s32, s32 *);


void func_80031B58(u8 *arg0);


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
u8 *func_8003C1BC(SpawnEnt *p, s32 x, s32 y, s32 z);


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
void func_8006F168(void *arg0, s32 arg1);
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
Ent34 *func_8006417C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);


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
s32 func_80037B6C(u8 *obj);
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
void func_8004E07C(void);
void func_8003BC98(void *arg0, void *arg1, s32 arg2);
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


void func_80058DFC(void *arg0);


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
void func_80032724(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
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
typedef struct { s16 m[3][3]; s32 t[3]; } MATRIX;
typedef struct { s32 xy; u16 z; u16 pad; } TerrVert;
typedef struct { s32 sxy; s32 flags; s32 col; } TerrVCache;
typedef struct { u8 pad[0xC]; s32 col; s32 pad10; } TerrCol;
typedef struct { TerrVert *verts; TerrVCache *cache; s32 pad8; TerrCol *cols; u8 pad10[0x2E]; u16 idx[3]; u8 pad44[0x14]; s32 sxy[3]; s32 flags[3]; s32 col[3]; } TerrTri;
typedef struct { u32 active : 1; u32 life : 5; u32 f6 : 3; u32 f9 : 3; u32 state : 4; u32 pad16 : 16; s32 unk4; } SparkHdr;
typedef struct { u32 pad0 : 11; u32 f11 : 1; u32 f12 : 1; u32 pad13 : 19; } WFlags;
typedef struct { s16 sin; s16 cos; } SinCos;
typedef struct PE { u8 active; u8 flags; u8 pad2[0xE]; s32 cnt; s32 limit; u8 *ptr; u8 b1C; u8 b1D; u8 pad1E[2]; void (*f20)(struct PE *); void (*f24)(struct PE *); u8 pad28[0xC]; } PE;
typedef struct { u8 r, g, b; } C3;
typedef struct { SparkHdr h; s16 v[6]; s32 time; MATRIX m; s32 pos[3]; } SparkEnt;
typedef struct { s32 a; u16 b; } V4050;
typedef struct { s32 a; s32 b; } V4058;
typedef struct { s32 w[4]; } Q4;
typedef struct { s32 w[16]; } Q16;
typedef struct { Q4 q[6]; s32 w; } Fr6E;
typedef struct { u8 *n; s32 st; } Stk6E;
typedef struct { s16 x, y, z; } V3s;
typedef struct { u8 pad0[0x52]; s16 unk52; s16 pad54; s16 speed; u8 pad58[0x38]; u8 *sub; } PlObj;
typedef struct { u16 x, y, z; } H3;
typedef struct { u8 b[4]; } B4;
typedef struct { u8 b[8]; } B8;
typedef struct { u8 b[12]; } B12;
typedef struct { s32 w[10]; } W40;
typedef struct { u8 b[20]; } B20;
typedef struct { u32 tag; u8 r0, g0, b0, code; s16 x0, y0; u8 u0, v0; u16 clut; u8 r1, g1, b1, p1; s16 x1, y1; u8 u1, v1; u16 tpage; u8 r2, g2, b2, p2; s16 x2, y2; u8 u2, v2; u16 pad2; u8 r3, g3, b3, p3; s16 x3, y3; u8 u3, v3; u16 pad3; } POLY_GT4;
typedef struct { s32 pad0; s32 rows; s32 cols; s32 w; s32 h; s32 *tiles; } SkyGrid;
typedef struct { u8 p0[0x90]; u8 *sub; u8 p1[0x88]; u16 f11C; s16 f11E; u16 f120; s16 f122; } Obj6A;
typedef struct { SVECTOR p[3]; u32 sxy[3]; u32 flg[3]; u32 col[3]; u16 uv[3]; } SubTri;
typedef struct { SVECTOR p[6]; u32 sxy[6]; u32 flg[6]; u32 col[6]; u16 uv[6]; } SubTri6;
typedef struct { u32 tag; u32 rgb0; u32 xy0; u32 uv0; u32 rgb1; u32 xy1; u32 uv1; u32 rgb2; u32 xy2; u32 uv2; } PolyGT3;
typedef struct { u8 pad[0x5C]; s16 x; s16 y; s16 z; s16 w; s32 a; s32 b; u8 pad2[8]; u8 *src; } FxObj;
#define gte_ldv0_mt(r0, r1) __asm__ volatile("mtc2 %0, $0; mtc2 %1, $1" : : "r"(r0), "r"(r1))
#define gte_mfsxy2(r) __asm__ volatile("mfc2 %0, $14" : "=r"(r))
#define gte_mfsz3(r) __asm__ volatile("mfc2 %0, $19; nop" : "=r"(r))
void *memset(void *, int, unsigned int);
#define PTF(k) { a = ((s32 *)fr)[k]; b = ((s32 *)fr)[k + 1]; c = ((s32 *)fr)[k + 2]; if (a < 0) v[0] = lo.x; else v[0] = hi.x; if (b < 0) v[1] = lo.y; else v[1] = hi.y; if (c < 0) v[2] = lo.z; else v[2] = hi.z; }
void func_800679AC(void);
void func_80032424(void *arg0);
void func_8006AA20(u8 **arg0);
extern u8 D_8009D2C8;
void func_80066B14(void);
#define gte_ldv0_mt(r0, r1) __asm__ volatile("mtc2 %0, $0; mtc2 %1, $1" : : "r"(r0), "r"(r1))
#define gte_mfsxy2(r) __asm__ volatile("mfc2 %0, $14" : "=r"(r))
#define gte_mfsz3(r) __asm__ volatile("mfc2 %0, $19; nop" : "=r"(r))
#define VERT(OI, OP, OF, OC, AN) do { \
idx = *(u16 *)(arg0 + OI); \
v = (u32 *)(*(u8 **)(arg0 + 4) + idx * 0xC); \
f = v[1]; \
if (f < 0) { \
*(s32 *)(arg0 + OF) = f; \
col = (f << 8) >> 24; \
if (col >= 0) col = v[2]; \
*(s32 *)(arg0 + OP) = v[0]; \
if (f & 0x40000000) { \
col = v[2]; \
AN = 1; \
} \
*(s32 *)(arg0 + OC) = col; \
} else { \
p = *(u8 **)arg0 + idx * 8; \
gte_ldv0_mt(*(u32 *)p, *(u16 *)(p + 4)); \
gte_rtps(); \
col = (f << 8) >> 24; \
if (col >= 0) { \
col = *(s32 *)(*(u8 **)(arg0 + 0xC) + col * 0x14 + 0xC); \
v[2] = col; \
} \
d = func_8006D3C0((SVECTOR *)(*(u8 **)arg0 + idx * 8), (SVECTOR *)(arg0 + 0x30)); \
lim = *(s32 *)(arg0 + 0x38); \
if (lim >= d) { \
if (col < 0) col = (*(s32 **)(arg0 + 8))[*(u16 *)(arg0 + OI)]; \
t = 0x10000 - (d << 16) / lim; \
r = (col & 0xFF) + ((t * arg0[0x2C]) >> 16); \
g = ((col >> 8) & 0xFF) + ((t * arg0[0x2D]) >> 16); \
b = ((col >> 16) & 0xFF) + ((t * arg0[0x2E]) >> 16); \
if (r >= 0x100) r = 0xFF; \
if (g >= 0x100) g = 0xFF; \
if (b >= 0x100) b = 0xFF; \
col = g << 8; \
if (D_8009CABD == 0) col = r | (g << 8) | (b << 16); \
v[2] = col; \
f |= 0x40000000; \
AN = 1; \
} \
*(s32 *)(arg0 + OC) = col; \
gte_mfsxy2(sxy); \
gte_mfsz3(sz); \
f |= sz >> 2; \
sy = sxy >> 16; \
v[0] = sxy; \
*(s32 *)(arg0 + OP) = sxy; \
sx = (s16)sxy; \
gte_cfFLAG(flag); \
if (flag & 0x46000) f |= 0x20000000; \
if (!(sy < 0x300 && sx < 0x300 && sy >= -0x100 && sx >= -0x100)) f |= 0x10000000; \
if (sx < 0) f |= 0x08000000; \
if (!(sx < 0x21C)) f |= 0x04000000; \
if (sy < 0) f |= 0x02000000; \
if (!(sy < 0xF0)) f |= 0x01000000; \
f |= 0x80000000; \
v[1] = f; \
*(s32 *)(arg0 + OF) = f; \
} \
} while (0)
void func_8006A41C(u8 *obj);
extern u16 D_8009E4D4;
extern u16 D_8009E4D8;
extern u16 D_8009E4DC;
extern u16 D_8009E4E0;
extern s32 D_8009E494;
extern char D_800A94A4[];
extern char D_800A94CC[];
#define E(g) (tbl + (g) * 0x70)
void func_80067F18(void);
#define gte_ldsxy3c(r0) __asm__ volatile("lwc2 $12, 0(%0); lwc2 $13, 4(%0); lwc2 $14, 8(%0)" : : "r"(r0))
#define gte_nclip() __asm__ volatile("nop; nop; .word 0x4B400006")
#define gte_mfMAC0(r) __asm__ volatile("mfc2 %0, $24; nop" : "=r"(r))
extern s32 D_800E2DF4;
void func_8006DD20(s32 *a, s32 *b, s32 *c, s32 *out);
#define NORM(i, o, w) { s32 ax, az, ay, k, sh; u32 m; ax = abs((i)->vx); az = abs((i)->vz); ay = abs((i)->vy); m = 0x40000000; k = 30; do { if ((ax & m) || (ay & m) || (az & m)) break; k--; m >>= 1; } while (k >= 0xE); sh = k - 13; if (sh > 0) { (o)->vx = (i)->vx >> sh; (o)->vz = (i)->vz >> sh; (o)->vy = (i)->vy >> sh; } else { (o)->vx = (i)->vx; (o)->vy = (i)->vy; (o)->vz = (i)->vz; } func_8008C5D8((s32 *)(o), (s32 *)(w)); }
extern s32 D_800A93C0;
extern s32 D_800A93D4;
extern s32 D_800A93E8;
extern s32 D_800A93F8;
extern s32 D_800A9410;
extern s32 D_800A9428;
extern s32 D_800A943C;
void func_80061FB4(void);
#define PT(k) { a = f[k]; b = f[k + 1]; c = f[k + 2]; if (a < 0) v[0] = lo.x; else v[0] = hi.x; if (b < 0) v[1] = lo.y; else v[1] = hi.y; if (c < 0) v[2] = lo.z; else v[2] = hi.z; }
u8 *func_80058B6C(u8 type);
void func_80058D5C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_800743E8(s32, s32 *, s32, s32, SVECTOR *, s32, s32);
void *func_8006421C(void *obj, s16 *pos, s16 arg2, s16 arg3, s16 arg4);
void func_800699B8(void *arg0);
void func_8006A700(void *a);
void func_8006A75C(u8 *arg0);
void func_8006A7C4(u8 *arg0);
void func_8006A828(void *arg0);
void func_8006A850(void *arg0);
void func_8006A87C(void *arg0);
void func_8006A8A8(void *arg0);
#define RAT(a, b) ((s32 (*)())ratan2)(a, b)
extern s32 D_8009E498;
extern s32 D_8009E49C;
extern s32 D_8009E4D0;
extern s32 D_8009E9BC;
#define FA65(o) (*(s16 *)((u8 *)arg0 + (o)))
void func_8004B334(s32 arg0);
void func_8004A8A8(s8 arg0, s8 arg1, s8 arg2, s16 arg3);
void func_80069CD4(s32 *arg0);
void func_80051AE0(s32 *arg0);
void func_800334F4(s32 *arg0);
extern u8 D_800A9794[];
#define M(T, o) (*(T *)(D_800E2DA8 + (o)))
#define A(T, o) (*(T *)((u8 *)arg0 + (o)))
extern s32 D_8009E9B4;
extern s32 D_8009E9B8;
extern s32 D_800D3068;
extern C3 D_800AFF64;
extern H3 D_800AFF68;
void func_8006C8F4(u8 *arg0, u8 *arg1, s32 arg2);
void func_8006B2C4(u8 *ctx, SubTri *t, s32 depth, s32 unused);
s32 func_8006CB88(u8 *t);
void func_8006D470(u8 *arg0);
#define SP(o) (*(s32 *)((u8 *)spad + (o)))
#define SPH(o) (*(u16 *)((u8 *)spad + (o)))
void Pad_StickToSpeed(s32 axis, s32 maxSpeed, s32 *out);
extern s32 D_8009E4B8;
extern s32 D_8009E4BC;
extern s32 D_8009E4C4;
void func_80069D60(u8 *obj);
void func_8002E228(void);
void func_80030798(void *);
void func_80054118(void);
void func_80068AF0(void);
void func_8005370C(void);
void func_80053A7C(void);
void func_80068A74(void);
void func_80069330(void);
void func_80069470(void);
void func_80069478(void);
void func_80069618(void);
void func_8006AF88(void);
void func_8006B16C(void);
void func_8006B1B4(void);
extern s32 D_8009CF14[];
extern s32 D_8009D0FC;
extern s32 D_8009D8F4;
extern s32 D_8009D8F8;
extern s32 D_8009D8FC;
extern M2C_UNK D_8009E040;
extern M2C_UNK D_8009E084;
extern M2C_UNK D_8009E0C8;
extern M2C_UNK D_8009E908;
extern char D_800A956C[];
extern M2C_UNK D_800A9580;
extern M2C_UNK D_800A95A8;
extern s16 D_800AFF94;
extern s16 D_800AFF98;
extern s16 D_800AFFB0;
extern s16 D_800AFFB4;
extern s16 D_800B0AC8;
extern s16 D_800B0AE4;
extern s32 D_800D30BC;
extern u8 D_800D86DC;
extern s32 D_800D86E4;
extern M2C_UNK D_800E02BC;
extern M2C_UNK D_800E0784;
extern M2C_UNK D_800E1A84;
extern M2C_UNK D_800E1EE4;
extern M2C_UNK D_800E23FC;
void func_800600EC(s32 *arg0);
void func_800691F4(u32 arg0);
s32 func_80069920(void);
void func_80069928(s32 arg0);
extern Fr6E D_800E2DE4;
extern u8 *D_800E2448[];
s32 func_8006E620(void *fr, V3s lo, V3s hi);
extern V4050 D_800D4050;
extern V4058 D_800D4058;
extern s32 D_800A9488;
SparkHdr *func_80065510(void);
s32 func_800662A8(s16 *arg0, SVECTOR *arg1);
s32 func_80065A20(void *arg0, SVECTOR *arg1, s32 arg2);
void func_80053DE4(s16 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8003246C(u16 *a, s32 b, s32 c, C3 *d);
AnimSlot *func_800378FC(u8 *obj, u8 *anim, s16 arg2, s32 arg3);
extern M2C_UNK D_800A946C;
void func_80063D40(u8 *e);
extern s32 D_8009E47C;
extern u8 D_800A9668[];
void func_8002E1B0(void);
s32 func_8002D4E4(u8 *);
extern u8 *D_800E23D0;
u8 *func_8006A0D8(void);
void func_8006A174(Obj6A *arg0);
void func_80026C14(s16 arg0);
extern s32 D_8009DB44;
extern u16 D_8009E4A0;
extern s32 D_8009E4B0;
extern s32 D_8009E4AC;
s32 func_800697C8(void *a);
u8 *func_80033420(s32 *, s32, s32, s32);
void func_8006484C(void *arg0);
extern s32 D_8009DB40;
extern s32 D_8009DB48;
extern s32 D_8009DB4C;
void func_80061AB0(s32 a, s32 b);
void func_800335B4(void);
extern u8 D_800DF684[];
void func_800655B8(void);
void *func_80058A58(s32 type);
void func_800664B8(void *arg0);
void func_80066704(void *arg0);
void func_8006692C(void *arg0);
void func_80066968(void *arg0);
void func_80065100(void *arg0);
void func_800652F0(void *arg0);
void func_80064A10(u8 *arg0);
void func_80064C28(u8 *arg0);
void func_80062FAC(s32 arg0, s16 arg1, u8 *arg2, u8 *arg3);
Ent34 *func_800637B0(s16 x, s16 y, s16 z);
s32 *func_8006412C(void);
void func_8006461C(void *arg0, s8 arg1, s32 arg2, s16 arg3);
void func_80063EF8(void *arg0);
void func_80064E40(u8 *arg0);
void func_80064660(void *arg0);
#define gte_ldVXYZ0(r0, r1) __asm__ volatile("mtc2 %0, $0; mtc2 %1, $1" : : "r"(r0), "r"(r1))
#define gte_stSXY2_SZOTZ(r0, r1) __asm__ volatile("mfc2 %0, $14; mfc2 %1, $19; nop; sra %1, %1, 2" : : "r"(r0), "r"(r1))
extern u8 D_800E0904[];
extern s32 D_8009E4E4;
extern s32 D_8009E4E8;
extern s32 D_8009E4EC;
void RotMatrixYXZ(void *, void *);
extern CmdHandler D_8009D7A8[];


void Cheat_DoAction(s32 action);


extern s32 D_8009CD4C;
extern s32 D_800B0C74;
extern s32 D_8009CD54;
extern s32 D_8009D3A4;
extern s32 D_800B0A94;
extern s16 D_8009CDA2;




extern M2C_UNK D_800D45C4;


void func_800636B8(void);
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
s32 func_80063B20(void);
extern s32 D_800B8F24;
extern GsOT D_800B8F88[];


M2C_UNK func_8007F2EC(M2C_UNK *, M2C_UNK *);        /* extern */
extern M2C_UNK D_800B0C7C;
extern M2C_UNK D_800DE8E8;


M2C_UNK func_80029684();                            /* extern */
void func_800456D0(void);
M2C_UNK func_80047A04();                            /* extern */
M2C_UNK func_80047DBC();                            /* extern */
void func_800481B0(void);
M2C_UNK func_80048E60();                            /* extern */
M2C_UNK func_80049220(u8 *);                        /* extern */
void func_800493C0(void);
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
void func_8002F564(void *arg0, s32 arg1, s32 arg2, s16 arg3, u16 arg4);
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
void func_80030F20(void);
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
s32 func_800302AC(void);
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
void func_800617AC(PlObj *obj);
void func_80061B70(u8 *obj);
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
void func_80063858(void);
void func_80063F84(void);
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
void func_8004218C(void);
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
s32 func_80069684(u8 *arg0);
extern u16 D_800D31B4;
extern M2C_UNK D_800D9234;
M2C_UNK func_8005C894();                            /* extern */
s32 func_8002C168(M2C_UNK *, M2C_UNK);              /* extern */
void func_8005B508(void);
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
void func_8006A2B8();
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

#ifdef NON_MATCHING
void func_800617AC(PlObj *obj) {
    s16 pos[4];
    s16 hit[4];
    s32 speed;
    s32 cur;
    s32 limit;
    s32 ang;
    s32 h;
    s32 pitch;
    s32 *cs;
    Args *args;
    s32 bsp;
    s32 l, r;

    cur = obj->speed;
    speed = cur;
    if (*(u8 *)&D_800B0E54) {
        Pad_StickToSpeed(0, 0x50, &speed);
    } else {
        l = D_800DDCC4;
        r = D_800DDDB8;
        if (l != 0 || r != 0) {
        if (D_800DDC60 != 0 || D_800DDC54 != 0) {
            if (l != 0) {
                speed = -0x50;
            } else {
                speed = 0x50;
            }
        } else {
            if (l != 0) {
                if (cur < -9) {
                    speed = cur - 10;
                } else {
                    speed = -10;
                }
            } else {
                if (cur < 10) {
                    speed = 10;
                } else {
                    speed = cur + 10;
                }
            }
            limit = 0x50;
            if (D_8009DB3C != 0 && obj->unk52 != 0) {
                limit = 0x50 - (abs(obj->unk52) >> 4);
            }
            if (limit < speed) {
                speed = limit;
            } else if (speed < -limit) {
                speed = -limit;
            }
        }
        } else {
            speed = 0;
        }
    }
    if (!*(u8 *)&D_800B0E54 && g_FrameRate < g_FrameCap) {
        speed = speed * g_FrameCap / g_FrameRate;
    }
    ang = (*(s16 *)((*(u8 **)((u8 *)g_Player + 0x90)) + 0x62) + speed) & 0xFFF;
    obj->speed = speed;
    *(s16 *)((*(u8 **)((u8 *)g_Player + 0x90)) + 0x62) = ang;
    *(s16 *)((u8 *)g_Player + 0xA0) = ang >> 6;
    cs = &D_800A0370[ang];
    gPsi = ang;
    pos[0] = g_Args->playerX + ((*(s16 *)cs * 125) >> 10);
    pos[1] = g_Args->playerY;
    pos[2] = g_Args->playerZ + (((*(volatile s32 *)cs >> 16) * 125) >> 10);
    h = ((s32 (*)(s32, s16 *, s16 *, s32))func_800705A8)(D_800E2F54, pos, hit, 0);
    if (h == -0x7FFF || h == pos[1] || (hit[1] < -0x7EF4 && *(s16 *)((u8 *)g_Player + 0x11E) < -0x7EF4)) {
        pitch = 0;
    } else {
        pitch = ((s32 (*)(s32, s32))ratan2)(pos[1] - h, 500);
    }
    *(s16 *)(obj->sub + 0x60) = pitch;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_800617AC);
#endif

void func_80061AB0(s32 a, s32 b) {
    s32 d;

    if (a != 0 && D_8009DB3C != 0 && D_8009DB44 != 0) {
        s32 t = -(a >> 4); KEEP(t); b += t;
    }
    if (b < 0) {
        if (b < -2) {
            b += 2;
        } else {
            b = 0;
        }
    } else if (b > 0) {
        d = 2;
        if (D_8009DB44 != 0 && D_8009DB40 != 0) {
            d = 7;
        }
        if (b > d) {
            b -= d;
        } else {
            b = 0;
        }
    }
    if (b > 300) {
        b = 300;
    } else if (b < -300) {
        b = -300;
    }
    *(s16 *)((u8 *)g_Player + 0x9C) = b;
    *(s16 *)((u8 *)g_Player + 0x52) = b;
}

void func_80061B70(u8 *obj) {
    s32 pitch;
    s32 fwd;
    s32 side;

    pitch = *(s16 *)(*(u8 **)(obj + 0x90) + 0x60);
    fwd = 0;
    if (D_800DDD90 != 0 && D_800DDD8C != 0) {
    } else if (D_800DDD90 != 0 || D_800DDD8C != 0) {
        if (D_8009DB3C != 0) {
            if (*(s16 *)(obj + 0x52) < 0x81) {
                if (D_8009DB48 != 0) {
                    if (--D_8009DB48 == 0) {
                        D_8009DB4C = 0;
                    }
                } else {
                    fwd = D_800DDD90 != 0 ? 0xE1 : -0xE1;
                    if (++D_8009DB4C == 5) {
                        D_8009DB48 = 20;
                    }
                }
            }
        } else {
            fwd = D_800DDD90 != 0 ? 0xE1 : -0xE1;
        }
    } else {
        D_8009DB48 = 0;
        D_8009DB4C = 0;
    }
    if (D_8009DB3C != 0) {
        side = *(s16 *)(obj + 0x52);
        if (D_800DDC44 != 0) {
            side += 6;
        }
        if (D_800DDCC0 != 0) {
            side -= 4;
            D_8009DB40 = 1;
        } else {
            D_8009DB40 = 0;
        }
        func_80061AB0(pitch, side);
        *(s16 *)((u8 *)g_Player + 0x98) = fwd;
        return;
    }
    side = D_800DDC44 != 0 ? 0xE1 : (D_800DDCC0 != 0 ? -0xE1 : 0);
    if (fwd != 0 && side != 0) {
        fwd = D_800DDD90 != 0 ? 0xA0 : -0xA0;
        side = D_800DDC44 != 0 ? 0xA0 : -0xA0;
    }
    if (*(s32 *)(*(u8 **)((u8 *)g_Player + 0x94) + 0x24) & 0x400) {
        fwd >>= 1;
        side >>= 1;
    }
    if (pitch != 0) {
        SinCos *t = (SinCos *)D_800A0370;
        side = (side * t[abs(pitch)].cos) >> 12;
    }
    *(s16 *)((u8 *)g_Player + 0x98) = fwd;
    *(s16 *)((u8 *)g_Player + 0x9C) = side;
}

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

void func_80061EC4(void) {
    u8 *p = g_Player;
    Args *a = g_Args;
    u8 *m = M2C_FIELD(p, u8 **, 0x90);
    s32 *pos = (s32 *)(m + 0x28);
    a->playerX += M2C_FIELD(p, s16 *, 0x98);
    a->playerY = pos[1];
    a->playerZ += M2C_FIELD(p, s16 *, 0x9C);
    D_8009CFA8 = -a->playerY;
    pos[0] = a->playerX;
    pos[2] = a->playerZ;
    M2C_FIELD(m, s32 *, 0x10) = 2;
}

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

#ifdef NON_MATCHING
void func_80061FB4(void) {
    u8 *base;
    u8 *p;

    base = (u8 *)D_8009D8B8;
    p = base + 0x28;
    if (base + 0xC8 < p) {
        func_8006B230(0, (s32)&D_800A93C0);
    }
    M2C_FIELD(p, s16 *, 0x0) = 0x3C;
    *(u8 *)(p + 0xA) = 0x3F;
    M2C_FIELD(p, u8 *, 0xB) = 0x3F;
    M2C_FIELD(p, s16 *, 0xE) = 0x1C0;
    M2C_FIELD(p, u8 *, 0x8) = 0x0;
    *(s16 *)(p + 0x2) = 0x3C;
    *(u8 *)(p + 0x6) = 0x60;
    *(u8 *)(p + 0x9) = 0x80;
    *(u8 *)(p + 0x7) = 0x60;
    *(s16 *)(p + 0xC) = 0x260;
    *(u8 *)(p + 0x5) = 0x60;
    M2C_FIELD(p, s16 *, 0x12) = 0x180;
    *(s16 *)(p + 0x10) = 0x300;
    *(u8 *)(p + 0x4) = 0x27;
    base = (u8 *)D_8009D8B8;
    p = base + 0x3C;
    if (base + 0xC8 < p) {
        func_8006B230(0, (s32)&D_800A93D4);
    }
    M2C_FIELD(p, s16 *, 0x0) = 0xC8;
    M2C_FIELD(p, u8 *, 0xA) = 0x3F;
    M2C_FIELD(p, u8 *, 0xB) = 0x3F;
    *(s16 *)(p + 0x2) = 0xC8;
    *(u8 *)(p + 0x5) = 0x80;
    M2C_FIELD(p, s16 *, 0xE) = 0x1E1;
    M2C_FIELD(p, u8 *, 0x6) = 0x80;
    *(u8 *)(p + 0x7) = 0x80;
    *(s16 *)(p + 0xC) = 0x250;
    M2C_FIELD(p, u8 *, 0x8) = 0x0;
    M2C_FIELD(p, s16 *, 0x10) = 0x240;
    M2C_FIELD(p, u8 *, 0x9) = 0x80;
    M2C_FIELD(p, s16 *, 0x12) = 0x180;
    *(u8 *)(p + 0x4) = 0x35;
    base = (u8 *)D_8009D8B8;
    p = base + 0x50;
    if (base + 0xC8 < p) {
        func_8006B230(0, (s32)&D_800A93E8);
    }
    M2C_FIELD(p, s16 *, 0x0) = 0x64;
    M2C_FIELD(p, s16 *, 0x2) = 0x64;
    *(u8 *)(p + 0xA) = 0x1F;
    M2C_FIELD(p, s16 *, 0xC) = 0x240;
    M2C_FIELD(p, s16 *, 0xE) = 0x1C1;
    M2C_FIELD(p, u8 *, 0xB) = 0x1F;
    *(u8 *)(p + 0x5) = 0x80;
    M2C_FIELD(p, u8 *, 0x6) = 0x80;
    *(u8 *)(p + 0x7) = 0x80;
    *(u8 *)(p + 0x8) = 0xC0;
    M2C_FIELD(p, s16 *, 0x10) = 0x230;
    M2C_FIELD(p, u8 *, 0x9) = 0x80;
    *(u8 *)(p + 0x4) = 0x80;
    M2C_FIELD(p, s16 *, 0x12) = 0x180;
    base = (u8 *)D_8009D8B8;
    p = base + 0x64;
    if (base + 0xC8 < p) {
        func_8006B230(0, (s32)&D_800A93F8);
    }
    M2C_FIELD(p, s16 *, 0x0) = 0x64;
    M2C_FIELD(p, s16 *, 0xC) = 0x270;
    M2C_FIELD(p, s16 *, 0x10) = 0x220;
    *(u8 *)(p + 0xA) = 0x1F;
    *(u8 *)(p + 0xB) = 0x1F;
    *(u8 *)(p + 0x5) = 0x80;
    M2C_FIELD(p, u8 *, 0x6) = 0x80;
    M2C_FIELD(p, s16 *, 0x2) = 0x64;
    M2C_FIELD(p, s16 *, 0xE) = 0x1C1;
    M2C_FIELD(p, u8 *, 0x7) = 0x80;
    M2C_FIELD(p, s16 *, 0x12) = 0x1A0;
    *(u8 *)(p + 0x8) = 0x80;
    *(u8 *)(p + 0x9) = 0xA0;
    M2C_FIELD(p, u8 *, 0x4) = 0x80;
    base = (u8 *)D_8009D8B8;
    p = base + 0x78;
    if (base + 0xC8 < p) {
        func_8006B230(0, (s32)&D_800A9410);
    }
    M2C_FIELD(p, s16 *, 0x0) = 0x64;
    M2C_FIELD(p, s16 *, 0x2) = 0x64;
    M2C_FIELD(p, s16 *, 0xE) = 0x1C0;
    M2C_FIELD(p, u8 *, 0xB) = 0x1E;
    M2C_FIELD(p, u8 *, 0xA) = 0x1F;
    M2C_FIELD(p, u8 *, 0x9) = 0xE0;
    M2C_FIELD(p, u8 *, 0x5) = 0x80;
    *(u8 *)(p + 0x6) = 0x80;
    *(s16 *)(p + 0xC) = 0x280;
    M2C_FIELD(p, u8 *, 0x7) = 0x80;
    M2C_FIELD(p, u8 *, 0x8) = 0x80;
    *(s16 *)(p + 0x10) = 0x320;
    M2C_FIELD(p, s16 *, 0x12) = 0x1E0;
    *(u8 *)(p + 0x4) = 0x81;
    base = (u8 *)D_8009D8B8;
    p = base + 0x8C;
    if (base + 0xC8 < p) {
        func_8006B230(0, (s32)&D_800A9428);
    }
    M2C_FIELD(p, s16 *, 0x0) = 0x64;
    M2C_FIELD(p, s16 *, 0x2) = 0x64;
    M2C_FIELD(p, u8 *, 0x4) = 0x80;
    base = (u8 *)D_8009D8B8;
    p = base + 0xA0;
    if (base + 0xC8 < p) {
        func_8006B230(0, (s32)&D_800A943C);
    }
    M2C_FIELD(p, s16 *, 0x0) = 0x55;
    *(s16 *)(p + 0x2) = 0x55;
    M2C_FIELD(p, u8 *, 0xB) = 0x3F;
    M2C_FIELD(p, s16 *, 0x10) = 0x370;
    M2C_FIELD(p, u8 *, 0x5) = 0x80;
    M2C_FIELD(p, u8 *, 0x6) = 0x80;
    M2C_FIELD(p, u8 *, 0x7) = 0x80;
    M2C_FIELD(p, u8 *, 0x8) = 0xC0;
    M2C_FIELD(p, u8 *, 0x9) = 0x80;
    M2C_FIELD(p, s16 *, 0xC) = 0x260;
    M2C_FIELD(p, s16 *, 0xE) = 0x1E1;
    M2C_FIELD(p, s16 *, 0x12) = 0x180;
    *(u8 *)(p + 0xA) = 0x3E;
    *(u8 *)(p + 0x4) = 0x97;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_80061FB4);
#endif

void func_80062300(void) {
    GsGetLs(D_800D3238, D_800DF10C);
    gte_SetRotMatrix(D_800DF10C);
    gte_SetTransMatrix(D_800DF10C);
    func_80062394();
    func_80062784();
    func_80062B68();
}

void func_80062394(void) {
    SVECTOR v;
    s32 h;
    s32 otz;
    s32 flag;
    s16 dw;
    s16 dh;
    u8 *p;
    u8 *end;
    s32 w;
    s32 ht;
    s32 r;
    s32 rx;
    s32 t;
    s32 a;
    s32 b;

    dw = g_Args->displayW;
    dh = g_Args->displayH;
    gte_ReadGeomScreen(&h);
    end = D_800DB884 + 30 * 0xA0;
    for (p = end - 30 * 0xA0; p < end; p += 0xA0) {
        if (p[0x9B] == 0) {
            continue;
        }
        v = *(SVECTOR *)(p + 0x54);
        gte_ldv0(&v);
        gte_rtps();
        w = *(s16 *)(p + 0x78) * h;
        ht = *(s16 *)(p + 0x7A) * h;
        gte_stsxy(p + 0x84);
        gte_stszotz(&otz);
        *(s16 *)(p + 0x82) = otz;
        gte_stflg(&flag);
        if (flag < 0 && (flag & 0x26000)) {
            *(s16 *)(p + 0x82) = 0;
            continue;
        }
        if (otz == 0) {
            s16 cx;
            s16 cy;
            cx = *(s16 *)(p + 0x84);
            cy = *(s16 *)(p + 0x86);
            *(s16 *)(p + 0x90) = cx;
            *(s16 *)(p + 0x8C) = cx;
            *(s16 *)(p + 0x88) = cx;
            *(s16 *)(p + 0x92) = cy;
            *(s16 *)(p + 0x8E) = cy;
            *(s16 *)(p + 0x8A) = cy;
            continue;
        }
        w /= otz;
        ht /= otz;
        if (*(s16 *)(p + 0x9C) != 0) {
            s16 cx;
            s16 cy;
            w >>= 1;
            ht >>= 1;
            r = SquareRoot0(w * w + ht * ht);
            rx = r * dw / dh;
            t = *(u16 *)(p + 0x9C) & 0xFFF;
            b = r * (*(volatile s32 *)&D_800A0370[t] >> 16);
            a = rx * (s16)D_800A0370[t];
            cy = *(s16 *)(p + 0x86);
            t = (t + 0x400) & 0xFFF;
            cx = *(s16 *)(p + 0x84);
            *(s16 *)(p + 0x8A) = cy + (b >> 12);
            *(s16 *)(p + 0x88) = cx + (a >> 12);
            {
                s32 a2 = rx * (s16)D_800A0370[t];
                s32 b2 = r * (*(volatile s32 *)&D_800A0370[t] >> 16);
                t = (t + 0x400) & 0xFFF;
                *(s16 *)(p + 0x90) = cx + (a2 >> 12);
                *(s16 *)(p + 0x92) = cy + (b2 >> 12);
            }
            {
                s32 a3 = rx * (s16)D_800A0370[t];
                s32 b3 = r * (*(volatile s32 *)&D_800A0370[t] >> 16);
                t = (t + 0x400) & 0xFFF;
                *(s16 *)(p + 0x8C) = cx + (a3 >> 12);
                *(s16 *)(p + 0x8E) = cy + (b3 >> 12);
            }
            {
                s32 a4 = rx * (s16)D_800A0370[t];
                s32 b4 = r * (*(volatile s32 *)&D_800A0370[t] >> 16);
                *(s16 *)(p + 0x84) = cx + (a4 >> 12);
                *(s16 *)(p + 0x86) = cy + (b4 >> 12);
            }
        } else {
            *(s16 *)(p + 0x84) -= w >> 1;
            *(s16 *)(p + 0x86) -= ht >> 1;
            *(s16 *)(p + 0x8C) = *(s16 *)(p + 0x84);
            *(s16 *)(p + 0x88) = *(s16 *)(p + 0x90) = *(s16 *)(p + 0x84) + w;
            *(s16 *)(p + 0x8A) = *(s16 *)(p + 0x86);
            *(s16 *)(p + 0x8E) = *(s16 *)(p + 0x92) = *(s16 *)(p + 0x86) + ht;
        }
    }
}

void func_80062784(void) {
    s32 m[8];
    Mat32 save;
    SVECTOR a;
    SVECTOR b;
    SVECTOR pos;
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    s32 otz;
    s32 flag;
    u8 *p;
    u8 *end;
    u8 *obj;
    u8 f;
    s16 hw;
    s16 hh;

    save = *(Mat32 *)D_800DF10C;
    end = D_800DA5C4 + 30 * 0xA0;
    for (p = end - 30 * 0xA0; p < end; p += 0xA0) {
        f = p[0x9B];
        if (f == 0) {
            continue;
        }
        if (f & 4) {
            continue;
        }
        if (!(f & 8)) {
            obj = *(u8 **)(p + 0x74);
            if (obj == NULL || obj[4] == 0) {
                p[0x9B] = 0;
                continue;
            }
            if (*(s32 *)(*(u8 **)(obj + 0x90) + *(s16 *)(p + 0x62) * 0x8C + 0x88) & 1) {
                func_800634C0(p);
            }
        }
        pos.vx = *(s16 *)(p + 0x54);
        pos.vy = *(s16 *)(p + 0x56);
        pos.vz = *(s16 *)(p + 0x58);
        RotMatrixYXZ_gte(p + 0x6C, m);
        gte_SetRotMatrix(m);
        hw = *(s16 *)(p + 0x78) >> 1;
        hh = *(s16 *)(p + 0x7A) >> 1;
        a.vx = -hw;
        a.vy = -hh;
        a.vz = 0;
        gte_ldv0(&a);
        gte_rtv0();
        b.vx = hw;
        b.vy = -hh;
        b.vz = 0;
        gte_stsv(&a);
        gte_ldv0(&b);
        gte_rtv0();
        v0.vx = a.vx + pos.vx;
        v0.vy = a.vy + pos.vy;
        v0.vz = a.vz + pos.vz;
        a.vx = -hw;
        a.vy = hh;
        a.vz = 0;
        gte_stsv(&b);
        gte_ldv0(&a);
        gte_rtv0();
        v1.vx = b.vx + pos.vx;
        v1.vy = b.vy + pos.vy;
        v1.vz = b.vz + pos.vz;
        gte_stsv(&a);
        v2.vx = a.vx + pos.vx;
        v2.vy = a.vy + pos.vy;
        v2.vz = a.vz + pos.vz;
        gte_SetRotMatrix(&save);
        gte_ldv3(&v0, &v1, &v2);
        gte_rtpt();
        gte_stsxy3(p + 0x84, p + 0x88, p + 0x8C);
        gte_avsz3();
        gte_stotz(&otz);
        *(s16 *)(p + 0x82) = otz;
        gte_stflg(&flag);
        if (flag < 0 && (flag & 0x26000)) {
            *(s16 *)(p + 0x82) = 0;
            continue;
        }
        *(s16 *)(p + 0x90) = *(s16 *)(p + 0x8C) + (*(s16 *)(p + 0x88) - *(s16 *)(p + 0x84));
        *(s16 *)(p + 0x92) = *(s16 *)(p + 0x8E) + (*(s16 *)(p + 0x8A) - *(s16 *)(p + 0x86));
    }
}

void func_80062B68(void) {
    Mat32 save;
    SVECTOR ang;
    s32 m[8];
    s32 otz;
    s32 flag;
    s32 k;
    s32 paused;
    s32 now;
    u8 *p;
    u8 *end;
    u8 f;
    s32 age;
    s32 x;
    s32 z;
    s16 dy;
    s16 oy;
    s32 y;

    k = D_800AFF88;
    save = *(Mat32 *)D_800DF10C;
    paused = D_8009CA9C;
    now = D_8009CDA8;
    k = (k < 3) ? 3 : ((k > 6) ? 6 : k);
    end = D_800DD7F4;
    for (p = end - 30 * 0x6C; p < end; p += 0x6C) {
        f = p[0x58];
        if (f == 0) {
            continue;
        }
        if (f & 4) {
            continue;
        }
        if (paused == 0) {
            age = now - *(s32 *)(p + 0x54);
            if (age >= 10) {
                p[0x58] = 0;
                continue;
            }
            if (k != 3) {
                x = *(s16 *)(p + 0x3E) * k;
                z = *(s16 *)(p + 0x42) * k;
                D_1F8003D4 = x;
                *(s16 *)(p + 0x2C) += (x >> 2) + (x >> 3);
                D_1F8003DC = z;
                *(s16 *)(p + 0x30) += (z >> 2) + (z >> 3);
            } else {
                *(s16 *)(p + 0x2C) += *(s16 *)(p + 0x3E);
                *(s16 *)(p + 0x30) += *(s16 *)(p + 0x42);
            }
            oy = *(s16 *)(p + 0x2E);
            dy = *(s16 *)(p + 0x44) + *(s16 *)(p + 0x46) * age + (age * 12) * age - oy;
            if (k != 3) {
                y = dy * k;
                D_1F8003D8 = y;
                dy = (y >> 2) + (y >> 3);
            }
            *(s16 *)(p + 0x2E) = oy + dy;
            ang.vx = ((s32 (*)(s32, s32))ratan2)(oy - *(s16 *)(p + 0x2E), *(s16 *)(p + 0x62));
            ang.vy = *(s16 *)(p + 0x60);
            ang.vz = 0;
            RotMatrixYXZ_gte(&ang, m);
            gte_SetRotMatrix(m);
            ang.vx = 0;
            ang.vy = 0;
            ang.vz = *(s16 *)(p + 0x68);
            gte_ldv0(&ang);
            gte_rtv0();
            gte_stsv(p + 0x34);
            *(s16 *)(p + 0x34) = *(s16 *)(p + 0x2C) - *(s16 *)(p + 0x34);
            *(s16 *)(p + 0x36) = *(s16 *)(p + 0x2E) - *(s16 *)(p + 0x36);
            *(s16 *)(p + 0x38) = *(s16 *)(p + 0x30) - *(s16 *)(p + 0x38);
            gte_SetRotMatrix(&save);
        }
        gte_ldv0(p + 0x2C);
        gte_rtps();
        gte_stsxy(p + 0x4C);
        gte_stszotz(&otz);
        *(s16 *)(p + 0x4A) = otz;
        gte_stflg(&flag);
        if (flag < 0 && (flag & 0x26000)) {
            *(s16 *)(p + 0x4A) = 0;
            continue;
        }
        gte_ldv0(p + 0x34);
        gte_rtps();
        gte_stsxy(p + 0x50);
    }
}

void func_80062F38(u8 *a, u8 *b) {
    s32 i;
    s32 n = 15;
    s16 sz = 10;
    for (i = 0; i < n; i++) {
        func_80062FAC(40, sz, a, b);
    }
}

#ifdef NON_MATCHING
void func_80062FAC(s32 arg0, s16 arg1, u8 *arg2, u8 *arg3) {
    s32 r;
    s32 a;
    s16 *p;
    s16 mat[16];
    SVECTOR rot;
    u8 *o;
    r = rand();
    o = func_80058B6C(0x50);
    if (o != NULL) {
        o[0x59] = arg2[0];
        o[0x5A] = arg2[1];
        o[0x5B] = arg2[2];
        o[0x5C] = arg3[0];
        o[0x5D] = arg3[1];
        o[0x5E] = arg3[2];
        *(u16 *)(o + 0x44) = *(u16 *)0x1F8003D8;
        a = r % 15;
        *(u16 *)(o + 0x48) = a * 57 + 0x80;
        arg0 = arg0 + (r % 3) * (arg0 >> 1);
        *(s16 *)(o + 0x68) = arg1;
        *(s16 *)(o + 0x60) = a * 250;
        *(s16 *)(o + 0x46) = -((arg0 * ((s16 *)D_800A0370)[(*(u16 *)(o + 0x48) & 0xFFF) * 2]) >> 12);
        func_80058D5C(o, *(s16 *)(o + 0x60), *(s16 *)(o + 0x48), arg0);
        *(s16 *)(o + 0x62) = SquareRoot0(*(s16 *)(o + 0x3E) * *(s16 *)(o + 0x3E) + *(s16 *)(o + 0x42) * *(s16 *)(o + 0x42));
        rot.vx = ratan2(-*(s16 *)(o + 0x40), *(s16 *)(o + 0x62));
        rot.vz = 0;
        rot.vy = *(u16 *)(o + 0x60);
        RotMatrixYXZ_gte(&rot, mat);
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = *(u16 *)(o + 0x68);
        gte_SetRotMatrix(mat);
        gte_ldv0(&rot);
        gte_rtv0();
        gte_stsv(o + 0x34);
        *(u16 *)(o + 0x34) = *(u16 *)(o + 0x2C) - *(u16 *)(o + 0x34);
        *(u16 *)(o + 0x36) = *(u16 *)(o + 0x2E) - *(u16 *)(o + 0x36);
        *(u16 *)(o + 0x38) = *(u16 *)(o + 0x30) - *(u16 *)(o + 0x38);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_80062FAC);
#endif

void func_80063230(void) {
    u8 c1[8];
    u8 c2[8];
    s32 i;
    s32 n = 15;
    s16 sz = 10;
    c1[0] = 0x80;
    c1[1] = 0x80;
    c1[2] = 0x80;
    c2[0] = 0xFF;
    c2[1] = 0xFF;
    c2[2] = 0xFF;
    for (i = 0; i < n; i++) {
        func_80062FAC(40, sz, c1, c2);
    }
}

void func_800632AC(u8 *arg0, s32 arg1) {
    FxObj *o;
    u8 t = arg1;
    s16 w;
    o = func_80058694(t);
    if (o != NULL || (D_800DA5C0 != 0 && (o = func_80058A58(t)) != NULL)) {
        *(s32 *)&o->x = D_800D4050.a;
        o->src = arg0;
        o->z = D_800D4050.b;
        w = *(u16 *)(arg0 + 0x66) & 0x1F;
        o->x += (o->x * 0x41) >> 12;
        o->y += (o->y * 0x41) >> 12;
        o->w = w;
        o->a = *(s32 *)&D_800D4058;
        o->b = D_800D4058.b;
        o->z += (o->z * 0x41) >> 12;
        *((s16 *)&o->b + 1) = -1;
        func_800634C0((u8 *)o);
    }
}

void func_800633B0(u8 type, VECTOR v) {
    SVECTOR ang;
    u8 *e;
    e = func_80058694(type);
    if (e == NULL) {
        if (D_800DA5C0 == 0) {
            return;
        }
        e = func_80058A58(type);
        if (e == NULL) {
            return;
        }
    }
    M2C_FIELD(e, s32 *, 0x74) = 0;
    e[0x9B] |= 8;
    M2C_FIELD(e, s16 *, 0x64) = v.vx;
    M2C_FIELD(e, s16 *, 0x66) = v.vy;
    M2C_FIELD(e, s16 *, 0x68) = v.vz;
    ang.vx = ((s32 (*)(s32, s32))ratan2)(-M2C_FIELD(e, s16 *, 0x66), SquareRoot0(M2C_FIELD(e, s16 *, 0x68) * M2C_FIELD(e, s16 *, 0x68) + M2C_FIELD(e, s16 *, 0x64) * M2C_FIELD(e, s16 *, 0x64)));
    ang.vy = ratan2(M2C_FIELD(e, s16 *, 0x64), M2C_FIELD(e, s16 *, 0x68));
    M2C_FIELD(e, s16 *, 0x6C) = (ang.vx + 0x800) & 0xFFF;
    M2C_FIELD(e, s16 *, 0x6E) = ang.vy & 0xFFF;
}

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

void func_800636B8(void) {
    s16 i;
    Ent34 *e;
    u8 *p;
    u8 *b;
    s32 *q;
    for (i = 0, e = D_800DF200; i < 16; i++, e++) {
        e->flags &= ~1;
    }
    for (i = 0, b = D_800DF684; i < 60; i++, b += 0x34) {
        *b = 0;
    }
    for (i = 0, q = D_800DF540; i < 5; i++, q += 6) {
        *q &= ~1;
    }
    for (i = 0, p = D_800DF5B8; i < 3; i++, p += 0x44) {
        *(s32 *)p &= ~1;
    }
    func_800655B8();
}

Ent34 *func_800637B0(s16 x, s16 y, s16 z) {
    s16 i;
    Ent34 *e;

    i = 0;
    e = D_800DF200;
    for (; i < 16; i++, e++) {
        if (!(e->flags & 1)) {
            e->flags = 0;
            e->unkC = 0;
            e->unk10 = 0;
            e->unk14 = 0;
            e->unk18 = 0;
            e->unk20 = 0;
            e->unk24 = 0;
            e->unk28 = 0;
            e->unk2C = 0;
            e->flags |= 1;
            e->x = x;
            e->y = y;
            e->z = z;
            e->unk30 = 0;
            return e;
        }
    }
    return NULL;
}

void func_80063858(void) {
    s16 i;
    Ent34 *e;
    s32 range;
    s32 px;
    s32 pz;
    s32 dx;
    s32 dz;
    u16 period;
    s32 *src;

    if (D_8009CA9C == 0 && D_8009CDA0 == 0) {
        i = 0;
        e = D_800DF200;
        px = g_Args->playerX;
        range = D_800E2DE0;
        pz = g_Args->playerZ;
        for (; i < 16; i++, e++) {
            if (e->flags & 1) {
                dz = 0;
                dx = 0;
                if (e->unk2C != 0 && (e->flags & 0xFE) != 4) {
                    src = (s32 *)(*(u8 **)(e->unk2C + 0x90) + 0xD4);
                    e->x = src[0];
                    e->y = src[1];
                    e->z = src[2];
                    if ((e->flags & 0xFE) == 6) {
                        e->x += *(s16 *)(e->unk30 + 0x10);
                        e->y += *(s16 *)(e->unk30 + 0x12);
                        e->z += *(s16 *)(e->unk30 + 0x14);
                    }
                } else if (e->unk10 != -1) {
                    dx = e->x - px;
                    if (dx < 0) dx = -dx;
                    if (dx > range) goto kill;
                    dz = e->z - pz;
                    if (dz < 0) dz = -dz;
                    if (dz > range) goto kill;
                }
                if (e->unk20 != 0) {
                    period = *(u16 *)&e->unk14;
                    if ((e->flags & 0xFE) == 4 && (dx > 7000 || dz > 7000)) {
                        period <<= 1;
                    }
                    if (e->unkC == 0 || e->unkC - ((s16 *)&e->unk14)[1] >= (s16)period) {
                        ((s16 *)&e->unk14)[1] = e->unkC;
                        ((u16 *)&e->unk18)[1]++;
                        ((void (*)(Ent34 *))e->unk20)(e);
                    }
                }
                e->unkC++;
                if (e->unk10 != -1 && e->unk10 < e->unkC) {
kill:
                    func_80063ADC(e);
                }
            }
        }
    }
}

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

s32 func_80063B20(void) {
    s32 r;
    s16 i;
    PE *e;
    r = D_8009CA9C;
    if (r == 0) {
        r = D_8009CDA0;
        i = 0;
        if (r == 0) {
            e = D_800DF684;
            do {
                if (e->active != 0 && !(e->flags & 2)) {
                    e->cnt = e->cnt + 1;
                    if ((e->limit != -1 && e->limit < e->cnt) || (e->ptr != 0 && e->ptr[4] == 0)) {
                        func_80063ED4(e);
                    } else if (e->f24 == 0 || (e->f24(e), e->active != 0)) {
                        if (e->f20 != 0) {
                            e->f20(e);
                        }
                        if (e->flags & 4) {
                            if (e->cnt >= 0xB) {
                                func_80063D40(e);
                            }
                        } else if (e->b1D != 0 && e->b1C < e->b1D) {
                            func_80063D40(e);
                        }
                    }
                }
                i++;
                e++;
                r = i < 0x3C;
            } while (r);
        }
    }
    return r;
}

void *func_80063CAC(void *obj, s16 x, s16 y, s16 z, s16 arg4) {
    s16 i;
    u8 *p;
    for (i = 0, p = D_800DF684; i < 60; i++, p += 0x34) {
        if (*p == 0) {
            *(s32 *)p = 0;
            M2C_FIELD(p, s32 *, 0x4) = 0;
            M2C_FIELD(p, s32 *, 0x8) = 0;
            M2C_FIELD(p, s32 *, 0xC) = 0;
            M2C_FIELD(p, s32 *, 0x10) = 0;
            M2C_FIELD(p, s32 *, 0x14) = 0;
            M2C_FIELD(p, s32 *, 0x18) = 0;
            M2C_FIELD(p, s32 *, 0x1C) = 0;
            M2C_FIELD(p, s16 *, 0x2) = arg4;
            *p = 1;
            M2C_FIELD(p, s32 *, 0x20) = M2C_FIELD(obj, s32 *, 0x24);
            M2C_FIELD(p, s32 *, 0x24) = M2C_FIELD(obj, s32 *, 0x28);
            M2C_FIELD(p, s32 *, 0x30) = 0;
            return p;
        }
    }
    return NULL;
}

void func_80063D40(u8 *e) {
    u8 *o;
    u8 *k;
    u8 t, c, s;
    s32 d;
    o = M2C_FIELD(e, u8 **, 0xC);
    k = e + (e[0x1C] * 4 + 0x28);
    t = k[0];
    c = o[0x98];
    d = t - c;
    s = k[3];
    if (d > 0) {
        if (((volatile u8 *)o)[0x98] + s > ((volatile u8 *)k)[0]) {
            o[0x98] = t;
        } else {
            o[0x98] = c + s;
        }
    } else if (d < 0) {
        if (((volatile u8 *)o)[0x98] - s < ((volatile u8 *)k)[0]) {
            o[0x98] = t;
        } else {
            o[0x98] = c - s;
        }
    }
    t = k[1];
    c = o[0x99];
    d = t - c;
    s = k[3];
    if (d > 0) {
        if (((volatile u8 *)o)[0x99] + s > ((volatile u8 *)k)[1]) {
            o[0x99] = t;
        } else {
            o[0x99] = c + s;
        }
    } else if (d < 0) {
        if (((volatile u8 *)o)[0x99] - s < ((volatile u8 *)k)[1]) {
            o[0x99] = t;
        } else {
            o[0x99] = c - s;
        }
    }
    t = k[2];
    c = o[0x9a];
    d = t - c;
    s = k[3];
    if (d > 0) {
        if (((volatile u8 *)o)[0x9a] + s > ((volatile u8 *)k)[2]) {
            o[0x9a] = t;
        } else {
            o[0x9a] = c + s;
        }
    } else if (d < 0) {
        if (((volatile u8 *)o)[0x9a] - s < ((volatile u8 *)k)[2]) {
            o[0x9a] = t;
        } else {
            o[0x9a] = c - s;
        }
    }
    if (o[0x98] == k[0] && o[0x99] == k[1] && o[0x9A] == k[2]) {
        e[0x1C]++;
    }
}

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

void func_80063F84(void) {
    SVECTOR v;
    Spark *e;
    s16 i;
    u32 dt;
    u32 t;

    for (i = 0; i < 3; i++) {
        e = (Spark *)D_800DF1F4[i];
        if (e->active) {
            dt = D_8009CDA8 - e->start;
            t = e->life * 5;
            if (t < dt) {
                func_80065588(e);
            } else if ((s32)t >> 1 < dt && e->state == 0) {
                e->state = 1;
                v.vx = rand() % 20 - 10;
                v.vy = rand() % 20 - 10;
                v.vz = rand() % 20 - 10;
                ((s32 (*)(void *, SVECTOR *, s32))func_80066098)(D_800DF1F4[i], &v, ((Spark *)D_800DF1F4[i])->life);
            }
        }
    }
}

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

Ent34 *func_8006417C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    Ent34 *e;
    e = func_800637B0(arg0, arg1, arg2);
    if (e == NULL) {
        return NULL;
    }
    {
        *(s32 *)((u8*)e + 0x10) = arg3;
        *(void **)((u8*)e + 0x20) = func_8006484C;
        *(void **)((u8*)e + 0x24) = func_80064C28;
        *(void **)((u8*)e + 0x28) = func_80063EF8;
        *(s16 *)((u8*)e + 0x14) = 3;
        *(s16 *)((u8*)e + 0x18) = 1;
        *(s16 *)((u8 *)e + 0x1E) = arg4;
        ((u8*)e)[0x1D] = 2;
    }
    return e;
}

void *func_8006421C(void *obj, s16 *pos, s16 arg2, s16 arg3, s16 arg4) {
    Ent34 *e;
    s32 *sub;
    s32 *t;

    if (obj == NULL) {
        return NULL;
    }
    t = (s32 *)(M2C_FIELD(obj, u8 **, 0x90) + 0x28);
    e = func_800637B0(t[0] + pos[0], t[1] + pos[1], t[2] + pos[2]);
    if (e == NULL) {
        return NULL;
    }
    sub = func_8006412C();
    e->unk30 = (s32)sub;
    if (sub == NULL) {
        func_80063ADC(e);
        return NULL;
    }
    func_8006461C(sub, 0x78, 0x14, arg4);
    M2C_FIELD(sub, s16 *, 0x10) = pos[0];
    M2C_FIELD(sub, s16 *, 0x12) = pos[1];
    M2C_FIELD(sub, s16 *, 0x14) = pos[2];
    e->unk10 = arg2;
    e->unk20 = (s32)func_80064660;
    e->unk24 = (s32)func_80064E40;
    e->unk28 = (s32)func_80063EF8;
    M2C_FIELD(e, s16 *, 0x14) = 2;
    M2C_FIELD(e, s16 *, 0x18) = 1;
    M2C_FIELD(e, s16 *, 0x1E) = arg3;
    M2C_FIELD(e, u8 *, 0x1D) = 3;
    e->unk2C = (s32)obj;
    e->flags = (e->flags & ~0xFE) | 6;
    return e;
}

Ent34 *func_8006439C(s16 x, s16 y, s16 z, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, u8 arg8) {
    Ent34 *e;
    s32 *sub;

    e = func_800637B0(x, y, z);
    if (e == NULL) {
        return NULL;
    }
    sub = func_8006412C();
    e->unk30 = (s32)sub;
    if (sub == NULL) {
        func_80063ADC(e);
        return NULL;
    }
    ((void (*)(void *, u8, s32, s16))func_8006461C)(sub, 0x8C, (u8)arg7, arg6);
    e->unk10 = arg3;
    e->unk20 = (s32)func_80064660;
    switch (arg8) {
    case 1:
        e->unk24 = (s32)func_80064C28;
        break;
    case 2:
        e->unk24 = (s32)func_80064E40;
        break;
    default:
        e->unk24 = (s32)func_80064A10;
        break;
    }
    e->unk28 = (s32)func_80063EF8;
    M2C_FIELD(e, s16 *, 0x14) = arg4;
    M2C_FIELD(e, s16 *, 0x18) = arg5;
    M2C_FIELD(e, s16 *, 0x1E) = 0;
    M2C_FIELD(e, u8 *, 0x1D) = 3;
    return e;
}

Ent34 *func_800644DC(s16 x, s16 y, s16 z, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, u8 arg8) {
    Ent34 *e;
    s32 *sub;

    e = func_800637B0(x, y, z);
    if (e == NULL) {
        return NULL;
    }
    sub = func_8006412C();
    e->unk30 = (s32)sub;
    if (sub == NULL) {
        func_80063ADC(e);
        return NULL;
    }
    ((void (*)(void *, u8, s32, s16))func_8006461C)(sub, 0x8C, (u8)arg7, arg6);
    e->unk10 = arg3;
    e->unk20 = (s32)func_80064660;
    switch (arg8) {
    case 1:
        e->unk24 = (s32)func_80064C28;
        break;
    case 2:
        e->unk24 = (s32)func_80064E40;
        break;
    default:
        e->unk24 = (s32)func_80064A10;
        break;
    }
    e->unk28 = (s32)func_80063EF8;
    M2C_FIELD(e, s16 *, 0x14) = arg4;
    M2C_FIELD(e, s16 *, 0x18) = arg5;
    M2C_FIELD(e, s16 *, 0x1E) = 0;
    M2C_FIELD(e, u8 *, 0x1D) = 2;
    return e;
}

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

void func_80064660(void *arg0) {
    s16 i;
    u8 *t;
    void *p;
    void *sub;

    t = M2C_FIELD(arg0, u8 **, 0x30);
    for (i = 0; i < M2C_FIELD(arg0, s16 *, 0x18); i++) {
        D_1F8003D4 = M2C_FIELD(arg0, s32 *, 0);
        D_1F8003D8 = M2C_FIELD(arg0, s32 *, 4);
        D_1F8003DC = M2C_FIELD(arg0, s32 *, 8);
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
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), u8 *, 0x98) = t[4];
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), u8 *, 0x99) = t[5];
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), u8 *, 0x9A) = t[6];
        M2C_FIELD(p, s8 *, 0x28) = 0;
        M2C_FIELD(p, s8 *, 0x29) = 0;
        M2C_FIELD(p, s8 *, 0x2A) = 0;
        M2C_FIELD(p, u8 *, 0x2B) = t[0] >> 1;
        M2C_FIELD(p, s8 *, 0x1C) = 0;
        M2C_FIELD(p, u8 *, 0x1D) = (*(u32 *)t >> 8) & 0xF;
        M2C_FIELD(p, s16 *, 4) = 0;
        M2C_FIELD(p, s16 *, 6) = 0;
        M2C_FIELD(p, s16 *, 8) = 0;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) = *(s16 *)(t + 0xC);
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) = *(s16 *)(t + 0xE);
        if (M2C_FIELD(arg0, s16 *, 0x1E) != 0) {
            M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) = (M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
            M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) = (M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
        }
        M2C_FIELD(p, s8 *, 1) = 4;
    }
}

void func_8006484C(void *arg0) {
    s16 i;
    void *p;
    void *sub;

    for (i = 0; i < M2C_FIELD(arg0, s16 *, 0x18); i++) {
        D_1F8003D4 = M2C_FIELD(arg0, s32 *, 0);
        D_1F8003D8 = M2C_FIELD(arg0, s32 *, 4);
        D_1F8003DC = M2C_FIELD(arg0, s32 *, 8);
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
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s8 *, 0x98) = 120;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s8 *, 0x99) = 120;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s8 *, 0x9A) = 120;
        M2C_FIELD(p, s8 *, 0x2B) = 10;
        M2C_FIELD(p, s8 *, 0x1D) = 1;
        M2C_FIELD(p, s8 *, 0x28) = 0;
        M2C_FIELD(p, s8 *, 0x29) = 0;
        M2C_FIELD(p, s8 *, 0x2A) = 0;
        M2C_FIELD(p, s8 *, 0x1C) = 0;
        M2C_FIELD(p, s16 *, 4) = 0;
        M2C_FIELD(p, s16 *, 6) = 0;
        M2C_FIELD(p, s16 *, 8) = 0;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) = 150;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) = 150;
        if (M2C_FIELD(arg0, s16 *, 0x1E) != 0) {
            M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) = (M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
            M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) = (M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
        }
    }
}

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

Ent34 *func_80065058(s16 x, s16 y, s16 z, void *obj, s16 arg4) {
    Ent34 *e;

    e = func_800637B0(x, y, z);
    if (e == NULL) {
        return NULL;
    }
    e->unk10 = arg4;
    e->unk20 = (s32)func_80065100;
    e->unk24 = (s32)func_800652F0;
    e->unk28 = (s32)func_80063EF8;
    M2C_FIELD(e, s16 *, 0x14) = 1;
    M2C_FIELD(e, s16 *, 0x18) = 2;
    M2C_FIELD(e, s16 *, 0x1E) = 2000;
    M2C_FIELD(e, u8 *, 0x1D) = 2;
    e->unk2C = (s32)obj;
    M2C_FIELD(obj, Ent34 **, 0x100) = e;
    return e;
}

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

void func_800652F0(void *arg0) {
    void *s0;
    void *a3;
    s32 n;
    s32 *t;
    s32 *base;
    s32 lo;
    s32 hi;
    s0 = M2C_FIELD(arg0, void **, 0x18);
    if (s0 == NULL) {
        func_8006B230(0, (s32)&D_800A946C);
    }
    a3 = M2C_FIELD(arg0, void **, 0xC);
    n = 0xC;
    if (g_FrameRate != 0x14) {
        n = 0xF0 / g_FrameRate;
    }
    base = D_800A0370;
    t = &base[M2C_FIELD(M2C_FIELD(s0, void **, 0x90), u16 *, 0x62) & 0xFFF];
    lo = (n * *(s16 *)t) >> 12;
    hi = (n * (*(volatile s32 *)t >> 16)) >> 12;
    M2C_FIELD(a3, u16 *, 0x54) += lo;
    M2C_FIELD(a3, u16 *, 0x58) += hi;
    n = -8;
    if ((u32)(D_8009CDA8 - M2C_FIELD(a3, s32 *, 0x94)) >= 7U) {
        n = -0xC;
    }
    if (g_FrameRate != 0x14) {
        n = (n * 0x14) / g_FrameRate;
    }
    M2C_FIELD(a3, u16 *, 0x56) += n;
    n = 0x32;
    if (g_FrameRate != 0x14) {
        n = 0x3E8 / g_FrameRate;
    }
    if (M2C_FIELD(a3, s32 *, 0x94) & 1) {
        M2C_FIELD(a3, u16 *, 0x9C) = M2C_FIELD(a3, u16 *, 0x9C) + n;
    } else {
        M2C_FIELD(a3, u16 *, 0x9C) = M2C_FIELD(a3, u16 *, 0x9C) - n;
    }
    n = 2;
    if (g_FrameRate != 0x14) {
        n = 0x28 / g_FrameRate;
    }
    M2C_FIELD(a3, u16 *, 0x78) += n;
    M2C_FIELD(a3, u16 *, 0x7A) += n;
}

SparkHdr *func_80065510(void) {
    s16 i;
    SparkHdr *p;

    p = (SparkHdr *)D_800DF5B8;
    for (i = 0; i < 3; i++) {
        if (!p->active) {
            p->active = 1;
            p->life = 0;
            p->f6 = 0;
            p->f9 = 0;
            p->state = 0;
            p->unk4 = 0;
            return p;
        }
        p = (SparkHdr *)((u8 *)p + 0x44);
    }
    return NULL;
}

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

M2C_UNK func_80065614(obj, life, kind)
void *obj; u8 life; u8 kind; {
    SparkEnt *p;
    s32 *t;
    s32 n;
    SVECTOR sv;
    C3 c;

    if (obj == NULL) {
        func_8006B230(0, (s32)&D_800A9488);
    }
    p = (SparkEnt *)func_80065510();
    if (p == NULL) {
        return 0;
    }
    t = p->m.t;
    p->v[0] = *(s16 *)((u8 *)obj + 0x1C) + (*(s16 *)((u8 *)obj + 0x1C) >> 1);
    p->v[2] = *(s16 *)((u8 *)obj + 0x20) + (*(s16 *)((u8 *)obj + 0x20) >> 1);
    p->v[4] = *(s16 *)((u8 *)obj + 0x24) + (*(s16 *)((u8 *)obj + 0x24) >> 1);
    p->v[1] = *(s16 *)((u8 *)obj + 0x1E) + (*(s16 *)((u8 *)obj + 0x1E) >> 1);
    p->v[3] = *(s16 *)((u8 *)obj + 0x22) + (*(s16 *)((u8 *)obj + 0x22) >> 1);
    p->v[5] = *(s16 *)((u8 *)obj + 0x26) + (*(s16 *)((u8 *)obj + 0x26) >> 1);
    p->m = *(MATRIX *)(*(u8 **)((u8 *)obj + 0x90) + 0x14);
    p->pos[0] = p->m.t[0];
    p->pos[1] = t[1];
    p->pos[2] = t[2];
    c.r = 0xFF;
    c.g = 0x50;
    c.b = 0xA;
    func_8003246C((u16 *)t, 0x2EE0, life, &c);
    func_80053DE4(0x10, 0, p->m.t[0], t[1], t[2]);
    p->h.life = life;
    p->h.f6 = kind;
    p->h.f9 = 2;
    p->time = D_8009CDA8;
    n = life * 5;
    sv.vx = 0;
    sv.vy = 0;
    sv.vz = 0;
    if (kind == 4) {
        if ((s16)((s32 (*)(void *, SVECTOR *, s32))func_80066098)(p, &sv, n) != 0) {
            goto done;
        }
        func_80065588(p);
        return 0;
    }
    if (kind == 2) {
        if ((s16)((s32 (*)(void *, SVECTOR *, s32))func_800662A8)(p, &sv, n) != 0) {
            goto done;
        }
        func_80065588(p);
        return 0;
    }
    if ((s16)func_80065A20(p, &sv, n) != 0) {
        goto ok;
    }
fail:
    func_80065588(p);
    return 0;
ok:
    p->h.unk4 = (s32)func_800378FC((u8 *)&p->m, (u8 *)&D_800D31C0, *(s16 *)((u8 *)obj + 0x2A), (s32)obj);
done:
    return 1;
}

void func_800658B4(void *arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 temp_a1;

    temp_a1 = arg1 >> 0x10;
    *arg2 = (s32) (temp_a1 * ((s32) (M2C_FIELD(arg0, s16 *, 0xA) - M2C_FIELD(arg0, s16 *, 8)) >> 1)) >> 0xB;
    *arg3 = (s32) (temp_a1 * ((s32) (M2C_FIELD(arg0, s16 *, 0xE) - M2C_FIELD(arg0, s16 *, 0xC)) >> 1)) >> 0xB;
    *arg4 = (s32) (temp_a1 * ((s32) (M2C_FIELD(arg0, s16 *, 0x12) - M2C_FIELD(arg0, s16 *, 0x10)) >> 1)) >> 0xB;
}

s32 func_80065928(s32 *pos, s16 arg1, s32 type) {
    Ent34 *e;

    e = func_800637B0(pos[0], pos[1], pos[2]);
    if (e == NULL) {
        return 0;
    }
    e->unk20 = (s32)func_800664B8;
    if (type == 5) {
        M2C_FIELD(e, s16 *, 0x14) = 1;
        M2C_FIELD(e, s16 *, 0x18) = 1;
        e->unk24 = (s32)func_8006692C;
    } else if (type == 6) {
        M2C_FIELD(e, s16 *, 0x14) = 1;
        M2C_FIELD(e, s16 *, 0x18) = 1;
        e->unk24 = (s32)func_80066968;
    } else {
        M2C_FIELD(e, s16 *, 0x14) = 3;
        M2C_FIELD(e, s16 *, 0x18) = 1;
        e->unk24 = (s32)func_80066704;
    }
    e->unk28 = (s32)func_80063EF8;
    e->unk10 = arg1;
    M2C_FIELD(e, u8 *, 0x1D) = 8;
    M2C_FIELD(e, s16 *, 0x1E) = 7500;
    e->flags = (e->flags & ~0xFE) | 8;
    return 1;
}

#ifdef NON_MATCHING
s32 func_80065A20(void *arg0, SVECTOR *arg1, s32 arg2) {
    s32 sp18[4];
    SVECTOR p;
    s16 c0, c1, c2;
    s32 sp38, sp3C, sp40;
    s16 stepX, stepY, stepZ;
    s32 n, i; s16 nn, nx, nz, k, j, m, cnt;
    s16 half;
    s16 len;

    len = arg2;
    if (arg0 == 0) {
        func_8006B230(0, (s32)&D_800A9498);
    }
    if (((*(u32 *)arg0 >> 6) & 7) == 5) {
        n = ((FA65(18) - FA65(16)) >> 11) + (((FA65(14) - FA65(12)) >> 11) + 1) + ((FA65(10) - FA65(8)) >> 11);
        if (n >= 0xB) n = 10;
        nn = n;
        for (i = 0; i < nn; i++) {
            func_800658B4(arg0, (i << 12) / nn, &sp38, &sp3C, &sp40);
            p.vx = sp38 + ((FA65(10) + FA65(8)) >> 1);
            p.vy = sp3C + ((FA65(14) + FA65(12)) >> 1);
            p.vz = sp40 + ((FA65(18) + FA65(16)) >> 1);
            func_800669A4(&p, sp18, arg0);
            if (func_80065928(sp18, (s16)(len / 2), (*(u32 *)arg0 >> 6) & 7) == 0) return i;
        }
    } else if (((*(u32 *)arg0 >> 6) & 7) == 6) {
        n = (s16)(((FA65(18) - FA65(16)) / 2000) + (((FA65(14) - FA65(12)) / 2000) + 1) + ((FA65(10) - FA65(8)) / 2000));
        if (n >= 0xB) n = 10;
        nn = n;
        for (i = 0; i < nn; i++) {
            func_800658B4(arg0, (i << 12) / nn, &sp38, &sp3C, &sp40);
            p.vx = sp38 + ((FA65(10) + FA65(8)) >> 1);
            p.vy = sp3C + ((FA65(14) + FA65(12)) >> 1);
            p.vz = sp40 + ((FA65(18) + FA65(16)) >> 1);
            func_800669A4(&p, sp18, arg0);
            if (func_80065928(sp18, (s16)(len / 2), (*(u32 *)arg0 >> 6) & 7) == 0) return i;
        }
    } else {
        func_80066A60(arg0, &stepX, &stepY, &stepZ);
        nx = (FA65(10) - FA65(8)) / stepX;
        nz = (FA65(18) - FA65(16)) / stepZ;
        if ((s16)nx == 0) nx = 1;
        if ((s16)nz == 0) nz = 1;
        cnt = 0;
        p.vz = arg1->vz + (FA65(16) + (stepZ >> 1));
        for (k = 0; k < nz; k++) {
            p.vy = arg1->vy + (FA65(12) / 3);
            for (j = 0; j < 1; j++) {
                p.vx = arg1->vx + (FA65(8) + (stepX >> 1));
                for (m = 0; m < nx; m++) {
                    func_800669A4(&p, sp18, arg0);
                    if (func_80065928(sp18, len, (*(u32 *)arg0 >> 6) & 7) == 0) return cnt;
                    cnt++;
                    p.vx += stepX;
                }
                p.vy += stepY;
            }
            p.vz += stepZ;
        }
    }
    if (D_8009CABD != 0) {
        c0 = 0;
        c1 = 0xC8;
        c2 = 0;
    } else {
        c0 = 0xD2;
        c1 = 0x8C;
        c2 = 0x46;
    }
    func_8004B334(1);
    func_8004A8A8((s8)c0, (s8)c1, (s8)c2, 3);
    return 1;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_80065A20);
#endif

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

void func_800664B8(void *arg0) {
    s16 i;
    s32 r;
    void *p;
    void *sub;

    r = rand();
    for (i = 0; i < M2C_FIELD(arg0, s16 *, 0x18); i++) {
        D_1F8003D4 = M2C_FIELD(arg0, s32 *, 0);
        D_1F8003D8 = M2C_FIELD(arg0, s32 *, 4);
        D_1F8003DC = M2C_FIELD(arg0, s32 *, 8);
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
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s8 *, 0x98) = r % 20 - 56;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), u8 *, 0x99) = 200;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), u8 *, 0x9A) = 200;
        M2C_FIELD(p, s8 *, 0x28) = 50;
        M2C_FIELD(p, s8 *, 0x29) = 50;
        M2C_FIELD(p, s8 *, 0x2A) = 50;
        M2C_FIELD(p, s8 *, 0x2B) = 12;
        M2C_FIELD(p, s8 *, 0x2F) = 25;
        M2C_FIELD(p, s8 *, 0x1D) = 2;
        M2C_FIELD(p, s8 *, 0x2C) = 0;
        M2C_FIELD(p, s8 *, 0x2D) = 0;
        M2C_FIELD(p, s8 *, 0x2E) = 0;
        M2C_FIELD(p, s8 *, 0x1C) = 0;
        M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x9C) = r & 0xFFF;
        if (M2C_FIELD(arg0, s16 *, 0x1E) != 0) {
            M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) = (M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x78) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
            M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) = (M2C_FIELD(M2C_FIELD(p, void **, 0xC), s16 *, 0x7A) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
            M2C_FIELD(p, s16 *, 4) = (M2C_FIELD(p, s16 *, 4) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
            M2C_FIELD(p, s16 *, 6) = (M2C_FIELD(p, s16 *, 6) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
            M2C_FIELD(p, s16 *, 8) = (M2C_FIELD(p, s16 *, 8) * M2C_FIELD(arg0, s16 *, 0x1E)) >> 12;
        }
    }
}

void func_80066704(void *arg0) {
    u8 *obj;
    s16 v[3];
    s32 d;

    obj = *(u8 **)((u8 *)arg0 + 0xC);
    if (g_FrameRate != 20) {
        v[0] = *(s16 *)((u8 *)arg0 + 4);
        v[1] = *(s16 *)((u8 *)arg0 + 6);
        v[2] = *(s16 *)((u8 *)arg0 + 8);
        v[0] = v[0] * 20 / g_FrameRate;
        v[1] = v[1] * 20 / g_FrameRate;
        v[2] = v[2] * 20 / g_FrameRate;
        *(u16 *)(obj + 0x54) += v[0];
        *(u16 *)(obj + 0x56) += v[1];
        *(u16 *)(obj + 0x58) += v[2];
    } else {
        *(u16 *)(obj + 0x54) += *(u16 *)((u8 *)arg0 + 4);
        *(u16 *)(obj + 0x56) += *(u16 *)((u8 *)arg0 + 6);
        *(u16 *)(obj + 0x58) += *(u16 *)((u8 *)arg0 + 8);
    }
    d = 15;
    if (g_FrameRate != 20) {
        d = 300 / g_FrameRate;
    }
    *(u16 *)(obj + 0x9C) += d;
    d = 32;
    if (g_FrameRate != 20) {
        d = 640 / g_FrameRate;
    }
    *(u16 *)(obj + 0x78) += d;
    *(u16 *)(obj + 0x7A) += d;
}

void func_8006692C(void *arg0) {
    u8 *p = *(u8 **)((u8 *)arg0 + 0xC);
    *(u16 *)(p + 0x6A) += 0xC8;
    *(u16 *)(p + 0x56) -= 0xA;
    func_80066704(arg0);
}

void func_80066968(void *arg0) {
    u8 *p = *(u8 **)((u8 *)arg0 + 0xC);
    *(u16 *)(p + 0x6A) += 0xC8;
    *(u16 *)(p + 0x56) -= 0xA;
    func_80066704(arg0);
}

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

#ifdef NON_MATCHING
void func_80066B14(void) {
    u8 *fp, *fp_2, *fp_3, *fp_4, *fp_5, *fp_6, *fp_7, *fp_8, *fp_9, *fp_10, *fp_11, *fp_12, *fp_13, *fp_14, *fp_15, *fp_16, *fp_17, *fp_18, *fp_19, *fp_20, *fp_21, *fp_22, *fp_23, *fp_24, *fp_25, *fp_26, *fp_27, *fp_28, *fp_29;

    fp = D_800E0904 + D_8009D27C * 0x70;
    *(s16 *)(fp + 0x10) = -0xA6;
    *(s16 *)(fp + 0x14) = -0xA6;
    *(s8 *)(fp + 1) = 2;
    *(s16 *)(fp + 0x12) = 0xA6;
    *(s16 *)(fp + 0x16) = 0xA6;
    *(s16 *)(fp + 0x1C) = 0x1DA;
    *(u8 *)(fp + 0) = (u8) D_8009D27C;
    *(s16 *)(fp + 0x64) = 0x20;
    *(s32 *)(fp + 0x40) = 0x42000400;
    *(s32 *)(fp + 0x24) = 1;
    *(s32 *)(fp + 0x54) = 0x40;
    *(s16 *)(fp + 0x1A) = 0x4A;
    *(s16 *)(fp + 0x1E) = 0xB5;
    fp_2 = D_800E0904 + D_8009D280 * 0x70;
    *(s16 *)(fp_2 + 0x64) = 0x14;
    *(s8 *)(fp_2 + 2) = 0xB;
    *(s32 *)(fp_2 + 0x24) = 0x14;
    *(s32 *)(fp_2 + 0x40) = 0x42000400;
    *(s8 *)(fp_2 + 1) = 2;
    *(u8 *)(fp_2 + 0) = (u8) D_8009D280;
    *(s16 *)(fp_2 + 0x10) = -6;
    *(s16 *)(fp_2 + 0x14) = -5;
    *(s16 *)(fp_2 + 0x18) = -0x1F;
    *(s16 *)(fp_2 + 0x12) = 6;
    *(s16 *)(fp_2 + 0x16) = 6;
    *(s16 *)(fp_2 + 0x1A) = 0x20;
    *(s16 *)(fp_2 + 0x1C) = 0x41;
    *(s16 *)(fp_2 + 0x1E) = 0x20;
    fp_3 = D_800E0904 + D_8009D268 * 0x70;
    *(u8 *)(fp_3 + 0) = (u8) D_8009D268;
    *(s16 *)(fp_3 + 0x10) = -0x3A;
    *(s16 *)(fp_3 + 0x14) = -0x44;
    *(s16 *)(fp_3 + 0x18) = -0x20B;
    *(s16 *)(fp_3 + 0x12) = 0x3B;
    *(s16 *)(fp_3 + 0x1A) = 0x9D;
    *(s16 *)(fp_3 + 0x1C) = 0x2BA;
    *(s8 *)(fp_3 + 1) = 1;
    *(s8 *)(fp_3 + 0x2C) = 1;
    *(s8 *)(fp_3 + 0x5C) = 0x2B;
    *(s16 *)(fp_3 + 0x64) = 0xC;
    *(s16 *)(fp_3 + 0x16) = 0x22;
    *(s32 *)(fp_3 + 0x20) = 0x3E8;
    *(s32 *)(fp_3 + 0x24) = 0xC8;
    fp_4 = D_800E0904 + D_8009D26C * 0x70;
    *(s16 *)(fp_4 + 0x64) = 0x30;
    *(s8 *)(fp_4 + 3) = 0x1B;
    *(s16 *)(fp_4 + 0x28) = 0x7FFF;
    *(s16 *)(fp_4 + 0x10) = -0x1A9;
    *(s16 *)(fp_4 + 0x14) = -0x140;
    *(s8 *)(fp_4 + 1) = 2;
    *(s32 *)(fp_4 + 0x40) = 0x02000400;
    *(u8 *)(fp_4 + 0) = (u8) D_8009D26C;
    *(s16 *)(fp_4 + 0x12) = 0x1AC;
    *(s16 *)(fp_4 + 0x16) = 0x140;
    *(s16 *)(fp_4 + 0x1E) = 0x1AC;
    *(s16 *)(fp_4 + 0x1C) = 0x42A;
    fp_5 = D_800E0904 + D_8009D2A8 * 0x70;
    *(s16 *)(fp_5 + 0x64) = 0x80;
    *(s8 *)(fp_5 + 3) = 0x20;
    *(s8 *)(fp_5 + 1) = 2;
    *(u8 *)(fp_5 + 0) = (u8) D_8009D2A8;
    *(s32 *)(fp_5 + 0x54) = 0x40;
    *(s16 *)(fp_5 + 0x10) = -0x143;
    *(s16 *)(fp_5 + 0x14) = -0x10F;
    *(s16 *)(fp_5 + 0x12) = 0x13B;
    *(s16 *)(fp_5 + 0x16) = -0xBB;
    *(s16 *)(fp_5 + 0x1C) = 0x2B5;
    *(s16 *)(fp_5 + 0x1E) = 0x143;
    fp_6 = D_800E0904 + D_8009D29C * 0x70;
    *(s16 *)(fp_6 + 0x64) = 0x3E;
    *(s16 *)(fp_6 + 0x10) = -0x171;
    *(s16 *)(fp_6 + 0x14) = -0x323;
    *(s16 *)(fp_6 + 0x18) = -0xB3;
    *(s16 *)(fp_6 + 0x12) = 0x171;
    *(s16 *)(fp_6 + 0x16) = -3;
    *(s16 *)(fp_6 + 0x1A) = 0xB3;
    *(s8 *)(fp_6 + 1) = 4;
    *(s16 *)(fp_6 + 0x1C) = 0x47C;
    *(u8 *)(fp_6 + 0) = (u8) D_8009D29C;
    *(s32 *)(fp_6 + 0x3C) = 0x20;
    *(s16 *)(fp_6 + 0x1E) = 0x19A;
    fp_7 = D_800E0904 + D_8009D260 * 0x70;
    *(u8 *)(fp_7 + 0) = (u8) D_8009D260;
    *(s16 *)(fp_7 + 0x64) = 1;
    *(s16 *)(fp_7 + 0x10) = -0xC;
    *(s8 *)(fp_7 + 1) = 6;
    *(s32 *)(fp_7 + 0x40) = 0x60408200;
    *(s16 *)(fp_7 + 0x12) = 0xC;
    *(s16 *)(fp_7 + 0x16) = 8;
    *(s16 *)(fp_7 + 0x1C) = 0x22;
    *(s16 *)(fp_7 + 0x1E) = 0x12B;
    *(s32 *)(fp_7 + 0x20) = 0x3E8;
    *(void **)(fp_7 + 0x38) = (void *)func_80051AE0;
    *(s16 *)(fp_7 + 0x14) = -0x10;
    fp_8 = D_800E0904 + D_8009D2CC * 0x70;
    *(s8 *)(fp_8 + 1) = 6;
    *(u8 *)(fp_8 + 0) = (u8) D_8009D2CC;
    *(u8 *)(fp_8 + 7) = (u8) D_8009D2D4;
    *(s16 *)(fp_8 + 0x10) = -0xC;
    *(u8 *)(fp_8 + 0x2C) = (u8) D_8009D2C8;
    *(s16 *)(fp_8 + 0x64) = 1;
    *(void **)(fp_8 + 0x38) = (void *)func_80051AE0;
    *(s16 *)(fp_8 + 0x14) = -0x10;
    *(s16 *)(fp_8 + 0x12) = 0xC;
    *(s16 *)(fp_8 + 0x16) = 8;
    *(s16 *)(fp_8 + 0x1C) = 0x22;
    *(s16 *)(fp_8 + 0x1E) = 0xC;
    *(s16 *)(fp_8 + 0x4C) = 0x3A98;
    *(s32 *)(fp_8 + 0x20) = 0x2EE0;
    *(s16 *)(fp_8 + 0x4E) = 0x11;
    *(s16 *)(fp_8 + 0x2A) = 0x3E8;
    *(s32 *)(fp_8 + 0x3C) = 0x14800420;
    *(s32 *)(fp_8 + 0x40) = 0x40000200;
    fp_9 = D_800E0904 + D_8009D2BC * 0x70;
    *(u8 *)(fp_9 + 0) = (u8) D_8009D2BC;
    *(s8 *)(fp_9 + 1) = 6;
    *(s16 *)(fp_9 + 0x10) = -0xC;
    *(s16 *)(fp_9 + 0x64) = 1;
    *(void **)(fp_9 + 0x38) = (void *)func_80051AE0;
    *(s16 *)(fp_9 + 0x14) = -0x10;
    *(s16 *)(fp_9 + 0x12) = 0xC;
    *(u8 *)(fp_9 + 7) = (u8) D_8009D2D0;
    *(s16 *)(fp_9 + 0x16) = 8;
    *(u8 *)(fp_9 + 0x2C) = (u8) D_8009D2C8;
    *(s16 *)(fp_9 + 0x4E) = 0x11;
    *(s16 *)(fp_9 + 0x2A) = 0x3E8;
    *(s32 *)(fp_9 + 0x40) = 0x40000200;
    *(s32 *)(fp_9 + 0x20) = 0x2EE0;
    *(s16 *)(fp_9 + 0x4C) = 0x3A98;
    *(s16 *)(fp_9 + 0x1C) = 0x22;
    *(s16 *)(fp_9 + 0x1E) = 0xC;
    *(s32 *)(fp_9 + 0x3C) = 0x14800420;
    fp_10 = D_800E0904 + D_8009D2D0 * 0x70;
    *(s16 *)(fp_10 + 0x64) = 0xE;
    *(s16 *)(fp_10 + 0x14) = -0xF5;
    *(u8 *)(fp_10 + 0) = (u8) D_8009D2D0;
    *(s8 *)(fp_10 + 1) = 4;
    *(s16 *)(fp_10 + 0x18) = -0x21;
    *(s16 *)(fp_10 + 0x16) = -0xD;
    *(s16 *)(fp_10 + 0x1C) = 0x101;
    *(s16 *)(fp_10 + 0x10) = -0x17;
    *(s16 *)(fp_10 + 0x12) = 0x17;
    *(s16 *)(fp_10 + 0x1A) = 0x21;
    *(s16 *)(fp_10 + 0x1E) = 0x28;
    fp_11 = D_800E0904 + D_8009D2D4 * 0x70;
    *(s16 *)(fp_11 + 0x64) = 0x1E;
    *(s16 *)(fp_11 + 0x14) = -0x134;
    *(s16 *)(fp_11 + 0x18) = -0x57;
    *(s16 *)(fp_11 + 0x16) = -8;
    *(s16 *)(fp_11 + 0x12) = 0x17;
    *(s16 *)(fp_11 + 0x10) = -0x17;
    *(s8 *)(fp_11 + 1) = 4;
    *(u8 *)(fp_11 + 0) = (u8) D_8009D2D4;
    *(s16 *)(fp_11 + 0x1A) = 0x57;
    *(s16 *)(fp_11 + 0x1C) = 0x165;
    *(s16 *)(fp_11 + 0x1E) = 0x5A;
    fp_12 = D_800E0904 + D_8009D2F0 * 0x70;
    *(s16 *)(fp_12 + 0x10) = -0x3F;
    *(s16 *)(fp_12 + 0x1C) = 0x167;
    *(s16 *)(fp_12 + 0x12) = 0xDC;
    *(s16 *)(fp_12 + 0x30) = 0xDC;
    *(s16 *)(fp_12 + 0x64) = 0x60;
    *(s8 *)(fp_12 + 1) = 4;
    *(s16 *)(fp_12 + 0x1A) = 0x60;
    *(s16 *)(fp_12 + 0x34) = 0x60;
    *(u8 *)(fp_12 + 0) = (u8) D_8009D2F0;
    *(s16 *)(fp_12 + 0x16) = 0x21;
    *(s16 *)(fp_12 + 0x32) = 0x21;
    *(u8 *)(fp_12 + 0x2C) = (u8) D_8009D268;
    *(s16 *)(fp_12 + 0x14) = 3;
    *(s16 *)(fp_12 + 0x18) = -0x7B;
    *(s16 *)(fp_12 + 0x1E) = 0xFC;
    fp_13 = D_800E0904 + D_8009D2F4 * 0x70;
    *(u8 *)(fp_13 + 0) = (u8) D_8009D2F4;
    *(s16 *)(fp_13 + 0x64) = 0x64;
    *(s8 *)(fp_13 + 1) = 4;
    *(s16 *)(fp_13 + 0x10) = -0xDD;
    *(s16 *)(fp_13 + 0x18) = -0x47;
    *(s16 *)(fp_13 + 0x1C) = 0x341;
    *(s16 *)(fp_13 + 0x14) = 4;
    *(s16 *)(fp_13 + 0x12) = 0x248;
    *(s16 *)(fp_13 + 0x16) = 0x25;
    *(s16 *)(fp_13 + 0x1A) = 0x8A;
    *(s16 *)(fp_13 + 0x1E) = 0x258;
    *(u8 *)(fp_13 + 0x2C) = (u8) D_8009D268;
    *(s16 *)(fp_13 + 0x30) = 0x248;
    *(s16 *)(fp_13 + 0x32) = 0x25;
    *(s16 *)(fp_13 + 0x34) = 0x8A;
    fp_14 = D_800E0904 + D_8009D2F8 * 0x70;
    *(s16 *)(fp_14 + 0x64) = 0x63;
    *(s16 *)(fp_14 + 0x10) = -0xFC;
    *(s16 *)(fp_14 + 0x14) = -0x14;
    *(s16 *)(fp_14 + 0x12) = 0x24D;
    *(s16 *)(fp_14 + 0x16) = 0x29;
    *(s16 *)(fp_14 + 0x1A) = 0x72;
    *(s16 *)(fp_14 + 0x1C) = 0x36B;
    *(s16 *)(fp_14 + 0x18) = -0x7B;
    *(s8 *)(fp_14 + 1) = 4;
    *(u8 *)(fp_14 + 0) = (u8) D_8009D2F8;
    *(u8 *)(fp_14 + 0x2C) = (u8) D_8009D268;
    *(s16 *)(fp_14 + 0x1E) = 0x259;
    fp_15 = D_800E0904 + D_8009D2FC * 0x70;
    *(u8 *)(fp_15 + 0) = (u8) D_8009D2FC;
    *(s16 *)(fp_15 + 0x64) = 0xEE;
    *(s32 *)(fp_15 + 0x3C) = 0x120;
    *(s16 *)(fp_15 + 0x28) = 0x400;
    *(s8 *)(fp_15 + 1) = 3;
    *(s8 *)(fp_15 + 0x2D) = 1;
    *(s32 *)(fp_15 + 0x20) = 0x2710;
    *(s32 *)(fp_15 + 0x40) = 0x82000080;
    *(s8 *)(fp_15 + 0x5E) = 0x37;
    *(s8 *)(fp_15 + 0x2F) = 0x23;
    *(void **)(fp_15 + 0x38) = (void *)func_8006AA20;
    *(u8 *)(fp_15 + 0x2E) = (u8) D_8009D268;
    *(s8 *)(fp_15 + 0x5D) = 0x4B;
    *(s16 *)(fp_15 + 0x10) = -0x49D;
    *(s16 *)(fp_15 + 0x14) = 0x1C58;
    *(s16 *)(fp_15 + 0x18) = -0x3F3;
    *(s16 *)(fp_15 + 0x12) = 0x4A4;
    *(s16 *)(fp_15 + 0x16) = 0x1FDC;
    *(s16 *)(fp_15 + 0x1A) = 0x98E;
    *(s16 *)(fp_15 + 0x1C) = 0x3A54;
    *(s16 *)(fp_15 + 0x1E) = 0xD48;
    *(s8 *)((u8 *)D_800D86E0 + 0) = 0;
    *(s8 *)((D_800E0904 + D_8009D2FC * 0x70) + 0x5F) = 0;
    *(s8 *)((u8 *)D_800D86E0 + 1) = 1;
    fp_16 = D_800E0904 + D_8009D300 * 0x70;
    *(s16 *)((u8 *)D_800D86E0 + 8) = 0;
    *(s16 *)((u8 *)D_800D86E0 + 0x14) = 0x1C75;
    *(s16 *)((u8 *)D_800D86E0 + 0x20) = 0;
    *(u8 *)(fp_16 + 0) = (u8) D_8009D300;
    *(s8 *)(fp_16 + 1) = 6;
    *(s16 *)(fp_16 + 0x64) = 1;
    *(s32 *)(fp_16 + 0x3C) = 0x14800420;
    *(s16 *)(fp_16 + 0x10) = -0xB;
    *(s16 *)(fp_16 + 0x14) = -0xF;
    *(s16 *)(fp_16 + 0x12) = 0xB;
    *(s16 *)(fp_16 + 0x16) = 7;
    *(s16 *)(fp_16 + 0x1C) = 0x1F;
    *(s16 *)(fp_16 + 0x1E) = 0xB;
    fp_17 = D_800E0904 + D_8009D304 * 0x70;
    *(u8 *)(fp_17 + 0) = (u8) D_8009D304;
    *(s16 *)(fp_17 + 0x10) = -0xC;
    *(s32 *)(fp_17 + 0x3C) = 0x14800420;
    *(s8 *)(fp_17 + 1) = 6;
    *(u8 *)(fp_17 + 0x2C) = (u8) D_8009D2F0;
    *(u8 *)(fp_17 + 0x2E) = (u8) D_8009D268;
    *(s16 *)(fp_17 + 0x64) = 1;
    *(s16 *)(fp_17 + 0x14) = -0x10;
    *(s16 *)(fp_17 + 0x12) = 0xC;
    *(s16 *)(fp_17 + 0x16) = 8;
    *(s16 *)(fp_17 + 0x1C) = 0x22;
    *(s16 *)(fp_17 + 0x1E) = 0xC;
    *(s32 *)(fp_17 + 0x20) = 0x7D0;
    *(void **)(fp_17 + 0x38) = (void *)func_80051AE0;
    fp_18 = D_800E0904 + D_8009D308 * 0x70;
    *(s8 *)(fp_18 + 1) = 6;
    *(u8 *)(fp_18 + 0) = (u8) D_8009D308;
    *(s8 *)(fp_18 + 8) = 1;
    *(s32 *)(fp_18 + 0x3C) = 0x20;
    *(s16 *)(fp_18 + 0x10) = -0xC;
    *(s32 *)(fp_18 + 0x20) = 0x7D0;
    *(s16 *)(fp_18 + 0x64) = 1;
    *(void **)(fp_18 + 0x38) = (void *)func_80051AE0;
    *(s16 *)(fp_18 + 0x14) = -0x10;
    *(s16 *)(fp_18 + 0x12) = 0xC;
    *(s16 *)(fp_18 + 0x16) = 8;
    *(s16 *)(fp_18 + 0x1C) = 0x22;
    *(s16 *)(fp_18 + 0x1E) = 0xC;
    fp_19 = D_800E0904 + D_8009D30C * 0x70;
    *(u8 *)(fp_19 + 0) = (u8) D_8009D30C;
    *(s16 *)(fp_19 + 0x64) = 1;
    *(s8 *)(fp_19 + 1) = 6;
    *(s8 *)(fp_19 + 0x5C) = 5;
    *(s8 *)(fp_19 + 0x5D) = 0x15;
    *(s8 *)(fp_19 + 0x66) = 0x14;
    *(s16 *)(fp_19 + 0x10) = -0xC;
    *(s32 *)(fp_19 + 0x3C) = 0x14800020;
    *(s16 *)(fp_19 + 0x16) = 8;
    *(void **)(fp_19 + 0x38) = (void *)func_80051AE0;
    *(s8 *)(fp_19 + 0x5E) = 0xF;
    *(s16 *)(fp_19 + 0x14) = -0x10;
    *(s16 *)(fp_19 + 0x12) = 0xC;
    *(s16 *)(fp_19 + 0x1C) = 0x22;
    *(s16 *)(fp_19 + 0x1E) = 0xC;
    *(u8 *)(fp_19 + 0x2C) = (u8) D_8009D2F8;
    *(u8 *)(fp_19 + 0x2E) = (u8) D_8009D268;
    *(s32 *)(fp_19 + 0x20) = 0xBB8;
    fp_20 = D_800E0904 + D_8009D310 * 0x70;
    *(u8 *)(fp_20 + 0) = (u8) D_8009D310;
    *(void **)(fp_20 + 0x38) = (void *)func_80032424;
    *(s16 *)(fp_20 + 0x10) = -0x2CE;
    *(s16 *)(fp_20 + 0x14) = -0xFD2;
    *(s16 *)(fp_20 + 0x18) = -0x13B8;
    *(s16 *)(fp_20 + 0x12) = 0x2CE;
    *(s16 *)(fp_20 + 0x1A) = 0x13B8;
    *(s16 *)(fp_20 + 0x1C) = 0x2ADC;
    *(s8 *)(fp_20 + 2) = 0x22;
    *(s8 *)(fp_20 + 1) = 0xB;
    *(s16 *)(fp_20 + 0x64) = 0xC;
    *(s16 *)(fp_20 + 0x1E) = 0x13EC;
    *(s32 *)(fp_20 + 0x40) = 0x80000000;
    fp_21 = D_800E0904 + D_8009D314 * 0x70;
    *(u8 *)(fp_21 + 0) = (u8) D_8009D314;
    *(s16 *)(fp_21 + 0x64) = 0x4A;
    *(s8 *)(fp_21 + 1) = 4;
    *(s8 *)(fp_21 + 2) = 0x2E;
    *(s16 *)(fp_21 + 0x10) = -0x35;
    *(s16 *)(fp_21 + 0x18) = -0x35;
    *(s16 *)(fp_21 + 0x14) = -0x82;
    *(s16 *)(fp_21 + 0x16) = 0xD;
    *(s16 *)(fp_21 + 0x12) = 0x35;
    *(s16 *)(fp_21 + 0x1A) = 0x35;
    *(s16 *)(fp_21 + 0x1C) = 0xCF;
    *(u8 *)(fp_21 + 0x2C) = (u8) D_8009D314;
    *(s16 *)(fp_21 + 0x1E) = 0x4B;
    fp_22 = D_800E0904 + D_8009D31C * 0x70;
    *(s16 *)(fp_22 + 0x64) = 1;
    *(s8 *)(fp_22 + 1) = 2;
    *(u8 *)(fp_22 + 0) = (u8) D_8009D31C;
    *(s16 *)(fp_22 + 0x10) = -0xB;
    *(s16 *)(fp_22 + 0x12) = 0xB;
    *(s16 *)(fp_22 + 0x1E) = 0xB;
    *(s16 *)(fp_22 + 0x14) = -0xF;
    *(s16 *)(fp_22 + 0x16) = 7;
    *(s16 *)(fp_22 + 0x1C) = 0x1F;
    fp_23 = D_800E0904 + D_8009D320 * 0x70;
    *(s8 *)(fp_23 + 1) = 2;
    *(s16 *)(fp_23 + 0x10) = -0xB;
    *(s16 *)(fp_23 + 0x14) = -0xF;
    *(s16 *)(fp_23 + 0x16) = 7;
    *(s16 *)(fp_23 + 0x12) = 0xB;
    *(u8 *)(fp_23 + 0) = (u8) D_8009D320;
    *(s16 *)(fp_23 + 0x1C) = 0x1F;
    *(s16 *)(fp_23 + 0x64) = 1;
    *(s16 *)(fp_23 + 0x1E) = 0xB;
    fp_24 = D_800E0904 + D_8009D324 * 0x70;
    *(s16 *)(fp_24 + 0x64) = 0x54;
    *(s16 *)(fp_24 + 0x10) = -0x361;
    *(s16 *)(fp_24 + 0x14) = -0x51E;
    *(s16 *)(fp_24 + 0x12) = 0x361;
    *(s16 *)(fp_24 + 0x1A) = 0x23B;
    *(s8 *)(fp_24 + 1) = 3;
    *(s16 *)(fp_24 + 0x1C) = 0x687;
    *(s8 *)(fp_24 + 0x5E) = 0x25;
    *(s8 *)(fp_24 + 0x66) = 0x26;
    *(u8 *)(fp_24 + 0) = (u8) D_8009D324;
    *(s32 *)(fp_24 + 0x3C) = 0x20;
    *(s16 *)(fp_24 + 0x18) = -0x23B;
    *(s16 *)(fp_24 + 0x1E) = 0x40D;
    *(s16 *)(fp_24 + 0x28) = 0x7FFF;
    *(s32 *)(fp_24 + 0x40) = 0x80000000;
    fp_25 = D_800E0904 + D_8009D328 * 0x70;
    *(s16 *)(fp_25 + 0x64) = 0x44;
    *(s16 *)(fp_25 + 0x10) = -0x9D9;
    *(s16 *)(fp_25 + 0x14) = -0x562;
    *(s16 *)(fp_25 + 0x18) = -0x1FE;
    *(s16 *)(fp_25 + 0x12) = 0x9D9;
    *(s16 *)(fp_25 + 0x1A) = 0x1FE;
    *(s16 *)(fp_25 + 0x1C) = 0xB66;
    *(s32 *)(fp_25 + 0x40) = 0x80000000;
    *(s8 *)(fp_25 + 1) = 3;
    *(s32 *)(fp_25 + 0x3C) = 0x20;
    *(s16 *)(fp_25 + 0x1E) = 0xA0C;
    *(s16 *)(fp_25 + 0x28) = 0x7FFF;
    *(u8 *)(fp_25 + 0) = (u8) D_8009D328;
    *(s32 *)(fp_25 + 0x54) = 0x200;
    *(s8 *)(fp_25 + 0x5E) = 0x25;
    *(s8 *)(fp_25 + 0x66) = 0x26;
    fp_26 = D_800E0904 + D_8009D32C * 0x70;
    *(s16 *)(fp_26 + 0x64) = 0x1E;
    *(s16 *)(fp_26 + 0x10) = -0x1A1;
    *(s16 *)(fp_26 + 0x14) = -0x4A4;
    *(s16 *)(fp_26 + 0x18) = -0x169;
    *(s16 *)(fp_26 + 0x12) = 0x1A1;
    *(s16 *)(fp_26 + 0x1A) = 0x169;
    *(s16 *)(fp_26 + 0x1C) = 0x655;
    *(s8 *)(fp_26 + 1) = 3;
    *(s16 *)(fp_26 + 0x1E) = 0x227;
    *(u8 *)(fp_26 + 0) = (u8) D_8009D32C;
    *(s32 *)(fp_26 + 0x3C) = 0x20;
    *(s8 *)(fp_26 + 0x2F) = 0x11;
    *(s8 *)(fp_26 + 0x5D) = 0x49;
    *(s8 *)(fp_26 + 0x5E) = 0x21;
    *(s8 *)(fp_26 + 0x66) = 0x22;
    *(u8 *)(fp_26 + 4) = (u8) D_8009D330;
    *(s32 *)(fp_26 + 0x20) = 0x3E8;
    fp_27 = D_800E0904 + D_8009D330 * 0x70;
    *(s16 *)(fp_27 + 0x10) = -0x20D;
    *(s16 *)(fp_27 + 0x14) = -0x41D;
    *(s16 *)(fp_27 + 0x18) = -0x1F8;
    *(s16 *)(fp_27 + 0x12) = 0x20D;
    *(s16 *)(fp_27 + 0x64) = 0x26;
    *(s8 *)(fp_27 + 1) = 3;
    *(s32 *)(fp_27 + 0x3C) = 0x20;
    *(s16 *)(fp_27 + 0x28) = 0x7FFF;
    *(u8 *)(fp_27 + 0) = (u8) D_8009D330;
    *(s16 *)(fp_27 + 0x1A) = 0x195;
    *(s16 *)(fp_27 + 0x1C) = 0x6CF;
    *(s16 *)(fp_27 + 0x1E) = 0x2D8;
    *(s32 *)(fp_27 + 0x40) = 0x02000400;
    fp_28 = D_800E0904 + D_8009D334 * 0x70;
    *(s16 *)(fp_28 + 0x64) = 0xE;
    *(s16 *)(fp_28 + 0x1C) = 0x697;
    *(s8 *)(fp_28 + 1) = 3;
    *(s32 *)(fp_28 + 0x3C) = 0x20;
    *(s16 *)(fp_28 + 0x10) = -0x1D9;
    *(s16 *)(fp_28 + 0x14) = -0x40E;
    *(s16 *)(fp_28 + 0x18) = -0x1D3;
    *(s16 *)(fp_28 + 0x12) = 0x1D3;
    *(s16 *)(fp_28 + 0x1A) = 0x1D9;
    *(s16 *)(fp_28 + 0x1E) = 0x29D;
    *(u8 *)(fp_28 + 0) = (u8) D_8009D334;
    *(s16 *)((u8 *)D_800D86E0 + 0xA8) = -0x1D9;
    *(s8 *)((D_800E0904 + D_8009D334 * 0x70) + 0x5F) = 1;
    *(s16 *)((u8 *)D_800D86E0 + 0xC0) = -0x40E;
    *(s16 *)((u8 *)D_800D86E0 + 0xD8) = -0x1D3;
    *(s16 *)((u8 *)D_800D86E0 + 0xB4) = 0x1D3;
    *(s16 *)((u8 *)D_800D86E0 + 0xCC) = 0;
    *(s16 *)((u8 *)D_800D86E0 + 0xE4) = 0x1D9;
    *(s32 *)(fp_28 + 0x20) = 0x1771;
    *(s8 *)(fp_28 + 0x2F) = 0x14;
    *(u8 *)(fp_28 + 4) = (u8) D_8009D338;
    *(s32 *)(fp_28 + 0x54) = 0x200;
    *(s32 *)(fp_28 + 0x40) = 0x80000100;
    *(s8 *)(fp_28 + 0x5D) = 0x4A;
    *(s8 *)(fp_28 + 0x5E) = 0x25;
    *(s8 *)(fp_28 + 0x66) = 0x26;
    fp_29 = D_800E0904 + D_8009D338 * 0x70;
    *(s16 *)(fp_29 + 0x64) = 0x12;
    *(s16 *)(fp_29 + 0x10) = -0x239;
    *(s16 *)(fp_29 + 0x18) = -0x239;
    *(s16 *)(fp_29 + 0x12) = 0x239;
    *(s16 *)(fp_29 + 0x1A) = 0x239;
    *(s16 *)(fp_29 + 0x1C) = 0x6AB;
    *(s8 *)(fp_29 + 1) = 3;
    *(s32 *)(fp_29 + 0x3C) = 0x20;
    *(s16 *)(fp_29 + 0x28) = 0x7FFF;
    *(s16 *)(fp_29 + 0x14) = -0x23B;
    *(s16 *)(fp_29 + 0x1E) = 0x324;
    *(s32 *)(fp_29 + 0x40) = 0x02000400;
    *(u8 *)(fp_29 + 0) = (u8) D_8009D338;
    func_800679AC();
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_80066B14);
#endif

#ifdef NON_MATCHING
void func_800679AC(void) {
    u8 *fp;
    fp = D_800E0904 + D_8009D33C * 0x70;
    *(s16 *)(fp + 0x64) = 0x14;
    *(s16 *)(fp + 0x10) = -0x1F4;
    *(s16 *)(fp + 0x1A) = 0x1F4;
    *(s8 *)(fp + 1) = 3;
    *(s32 *)(fp + 0x3C) = 0x20;
    *(s16 *)(fp + 0x28) = 0x7FFF;
    *(s16 *)(fp + 0x14) = -0x58B;
    *(s16 *)(fp + 0x12) = 0x519;
    *(s16 *)(fp + 0x1C) = 0x788;
    *(s16 *)(fp + 0x1E) = 0x519;
    *(u8 *)(fp + 0) = D_8009D33C;
    *(s16 *)((u8 *)D_800D86E0 + 0x126) = 0;
    *(s8 *)((D_800E0904 + D_8009D33C * 0x70) + 0x5F) = 2;
    *(s16 *)((u8 *)D_800D86E0 + 0x132) = 0x519;
    *(s16 *)((u8 *)D_800D86E0 + 0x13E) = -0x58B;
    *(s16 *)((u8 *)D_800D86E0 + 0x156) = 0;
    *(s16 *)((u8 *)D_800D86E0 + 0x14A) = 0;
    *(s16 *)((u8 *)D_800D86E0 + 0x162) = 0;
    *(s8 *)(fp + 0x5E) = 0x21;
    *(s8 *)(fp + 0x66) = 0x22;
    *(s32 *)(fp + 0x54) = 0x200;
    *(s32 *)(fp + 0x40) = 0x80000100;
    fp = D_800E0904 + D_8009D340 * 0x70;
    *(s32 *)(fp + 0x3C) = 0x20;
    *(s16 *)(fp + 0x64) = 0xE;
    *(s8 *)(fp + 1) = 3;
    *(u8 *)(fp + 0) = D_8009D340;
    *(s16 *)(fp + 0x28) = 0x7FFF;
    *(s16 *)(fp + 0x1C) = 0x836;
    *(s16 *)(fp + 0x10) = -0x2BB;
    *(s16 *)(fp + 0x14) = -0x407;
    *(s16 *)(fp + 0x18) = -0x251;
    *(s16 *)(fp + 0x12) = 0x2B2;
    *(s16 *)(fp + 0x1A) = 0x259;
    *(s16 *)(fp + 0x1E) = 0x39A;
    *(s16 *)((u8 *)D_800D86E0 + 0x1A4) = -0x2BB;
    *(s8 *)((D_800E0904 + D_8009D340 * 0x70) + 0x5F) = 3;
    *(s16 *)((u8 *)D_800D86E0 + 0x1BC) = -0x407;
    *(s16 *)((u8 *)D_800D86E0 + 0x1D4) = -0x251;
    *(s16 *)((u8 *)D_800D86E0 + 0x1B0) = 0x2B2;
    *(s16 *)((u8 *)D_800D86E0 + 0x1C8) = 0;
    *(s16 *)((u8 *)D_800D86E0 + 0x1E0) = 0x259;
    *(s32 *)(fp + 0x54) = 0x200;
    *(s8 *)(fp + 0x5E) = 0x21;
    *(s8 *)(fp + 0x66) = 0x22;
    *(s32 *)(fp + 0x40) = 0x80000100;
    fp = D_800E0904 + D_8009D344 * 0x70;
    *(u8 *)(fp + 0) = D_8009D344;
    *(s16 *)(fp + 0x64) = 0xC;
    *(s8 *)(fp + 1) = 0xB;
    *(s16 *)(fp + 0x10) = -0x167;
    *(s16 *)(fp + 0x14) = -0x7E9;
    *(s8 *)(fp + 2) = 0x22;
    *(void * *)(fp + 0x38) = (void *)func_80069CD4;
    *(s16 *)(fp + 0x12) = 0x167;
    *(s16 *)(fp + 0x1A) = 0x9DC;
    *(s16 *)(fp + 0x1C) = 0x156E;
    *(s16 *)(fp + 0x18) = -0x9DC;
    *(s16 *)(fp + 0x1E) = 0x9F6;
    fp = D_800E0904 + D_8009D348 * 0x70;
    *(s16 *)(fp + 0x64) = 1;
    *(s32 *)(fp + 0x3C) = 0x14800020;
    *(s8 *)(fp + 1) = 6;
    *(s16 *)(fp + 0x14) = -0x10;
    *(s16 *)(fp + 0x16) = 8;
    *(s32 *)(fp + 0x20) = 0x3E8;
    *(u8 *)(fp + 0) = D_8009D348;
    *(s16 *)(fp + 0x10) = -0xC;
    *(s16 *)(fp + 0x12) = 0xC;
    *(s16 *)(fp + 0x1C) = 0x22;
    *(s16 *)(fp + 0x1E) = 0xC;
    *(void * *)(fp + 0x38) = (void *)func_80051AE0;
    fp = D_800E0904 + D_8009D2A0 * 0x70;
    *(s16 *)(fp + 0x64) = 0x3A;
    *(s16 *)(fp + 0x10) = -0x139;
    *(s16 *)(fp + 0x14) = -0x24B;
    *(s16 *)(fp + 0x18) = -0x95;
    *(s16 *)(fp + 0x12) = 0x139;
    *(s8 *)(fp + 1) = 4;
    *(u8 *)(fp + 0) = D_8009D2A0;
    *(s16 *)(fp + 0x16) = -8;
    *(s16 *)(fp + 0x1A) = 0x95;
    *(s16 *)(fp + 0x1C) = 0x38C;
    *(s16 *)(fp + 0x1E) = 0x15B;
    fp = D_800E0904 + D_8009D34C * 0x70;
    *(s8 *)(fp + 1) = 4;
    *(u8 *)(fp + 0) = D_8009D34C;
    *(s16 *)(fp + 0x64) = 0x53;
    *(u8 *)(fp + 0x2C) = D_8009D268;
    *(s16 *)(fp + 0x14) = 2;
    *(s16 *)(fp + 0x18) = -0x32;
    *(s16 *)(fp + 0x12) = 0xA6;
    *(s16 *)(fp + 0x16) = 0x19;
    *(s16 *)(fp + 0x1A) = 0x4F;
    *(s16 *)(fp + 0x1C) = 0xDD;
    *(s16 *)(fp + 0x10) = -0xC;
    *(s16 *)(fp + 0x1E) = 0xB7;
    fp = D_800E0904 + D_8009D350 * 0x70;
    *(s16 *)(fp + 0x64) = 0xA1;
    *(s32 *)(fp + 0x3C) = 0x24;
    *(s16 *)(fp + 0x10) = -0x54D;
    *(s16 *)(fp + 0x14) = -0x222;
    *(s16 *)(fp + 0x18) = -0x478;
    *(s16 *)(fp + 0x12) = 0x50B;
    *(s16 *)(fp + 0x16) = 0x3F4;
    *(s16 *)(fp + 0x1A) = 0xA71;
    *(s8 *)(fp + 1) = 3;
    *(s16 *)(fp + 0x1C) = 0x1324;
    *(u8 *)(fp + 0) = D_8009D350;
    *(s16 *)(fp + 0x28) = 0x7FFF;
    *(s16 *)(fp + 0x1E) = 0xBB6;
    fp = D_800E0904 + D_8009D354 * 0x70;
    *(s16 *)(fp + 0x64) = 0xB2;
    *(s8 *)(fp + 1) = 3;
    *(s32 *)(fp + 0x3C) = 0x420;
    *(s16 *)(fp + 0x10) = -0x49D;
    *(s16 *)(fp + 0x14) = -0x498;
    *(s16 *)(fp + 0x18) = -0x3F3;
    *(s16 *)(fp + 0x12) = 0x4A4;
    *(u8 *)(fp + 0) = D_8009D354;
    *(s16 *)(fp + 0x16) = 0x367;
    *(s16 *)(fp + 0x1C) = 0x1237;
    *(s16 *)(fp + 0x1A) = 0x98E;
    *(s16 *)(fp + 0x1E) = 0xA9F;
    *(s16 *)((u8 *)D_800D86E0 + 0x222) = -0x49D;
    *(s8 *)((D_800E0904 + D_8009D354 * 0x70) + 0x5F) = 4;
    *(s16 *)((u8 *)D_800D86E0 + 0x23A) = -0x498;
    *(s16 *)((u8 *)D_800D86E0 + 0x252) = -0x3F3;
    *(s16 *)((u8 *)D_800D86E0 + 0x22E) = 0x4A4;
    *(s16 *)((u8 *)D_800D86E0 + 0x246) = 0x367;
    *(s16 *)((u8 *)D_800D86E0 + 0x25E) = 0x98E;
    *(s8 *)(fp + 0x5D) = 0x4B;
    *(s8 *)(fp + 0x5E) = 0x1E;
    *(s8 *)(fp + 0x2F) = 0x11;
    *(s32 *)(fp + 0x40) = 0x80000100;
    *(u8 *)(fp + 4) = D_8009D350;
    *(s32 *)(fp + 0x20) = 0x2710;
    fp = D_800E0904 + D_8009D358 * 0x70;
    *(u8 *)(fp + 0) = D_8009D358;
    *(s16 *)(fp + 0x64) = 0xC;
    *(s8 *)(fp + 1) = 0xB;
    *(s16 *)(fp + 0x10) = -0x167;
    *(s16 *)(fp + 0x14) = -0x7E9;
    *(s8 *)(fp + 2) = 0x22;
    *(void * *)(fp + 0x38) = (void *)func_800334F4;
    *(s16 *)(fp + 0x18) = -0x9DC;
    *(s16 *)(fp + 0x12) = 0x167;
    *(s16 *)(fp + 0x1A) = 0x9DC;
    *(s16 *)(fp + 0x1C) = 0x156E;
    *(s16 *)(fp + 0x1E) = 0x9F6;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_800679AC);
#endif

#ifdef NON_MATCHING
void func_80067F18(void) {
    u8 *buf;
    u8 *tbl = (u8 *)&D_800E1A84;
    *E(D_8009D2B4) = D_8009D2B4;
    *(s16 *)(E(D_8009D2B4) + 0x6C) = 7;
    buf = (u8 *)func_800305B4(7);
    *(u8 **)(E(D_8009D2B4) + 0x68) = buf;
    if (buf == NULL) {
        printf(D_800A94A4, D_800A94CC, 43);
        exit(1);
    }
    buf[0] = 9;
    (*(u8 **)(E(D_8009D2B4) + 0x68))[1] = 8;
    (*(u8 **)(E(D_8009D2B4) + 0x68))[2] = 8;
    (*(u8 **)(E(D_8009D2B4) + 0x68))[3] = 31;
    (*(u8 **)(E(D_8009D2B4) + 0x68))[4] = 28;
    (*(u8 **)(E(D_8009D2B4) + 0x68))[5] = 46;
    (*(u8 **)(E(D_8009D2B4) + 0x68))[6] = 30;
    *(u8 *)(E(D_8009D2B4) + 0x2C) = D_8009D268;
    *(s16 *)(E(D_8009D2B4) + 0x32) = -0x6E;
    *(s16 *)(E(D_8009D2B4) + 0x30) = 0;
    *(s16 *)(E(D_8009D2B4) + 0x34) = 0x2BC;
    *E(D_8009D2B8) = D_8009D2B8;
    *(s16 *)(E(D_8009D2B8) + 0x6C) = 7;
    buf = (u8 *)func_800305B4(7);
    *(u8 **)(E(D_8009D2B8) + 0x68) = buf;
    if (buf == NULL) {
        printf(D_800A94A4, D_800A94CC, 61);
        exit(1);
    }
    buf[0] = 9;
    (*(u8 **)(E(D_8009D2B8) + 0x68))[1] = 10;
    (*(u8 **)(E(D_8009D2B8) + 0x68))[2] = 2;
    (*(u8 **)(E(D_8009D2B8) + 0x68))[3] = 10;
    (*(u8 **)(E(D_8009D2B8) + 0x68))[4] = 35;
    (*(u8 **)(E(D_8009D2B8) + 0x68))[5] = 43;
    (*(u8 **)(E(D_8009D2B8) + 0x68))[6] = 54;
    *(u8 *)(E(D_8009D2B8) + 0x2C) = D_8009D268;
    *(s16 *)(E(D_8009D2B8) + 0x32) = -0x82;
    *(s16 *)(E(D_8009D2B8) + 0x30) = 0;
    *(s16 *)(E(D_8009D2B8) + 0x34) = 0x258;
    *E(D_8009D2D8) = D_8009D2D8;
    *(s16 *)(E(D_8009D2D8) + 0x6C) = 6;
    buf = (u8 *)func_800305B4(6);
    *(u8 **)(E(D_8009D2D8) + 0x68) = buf;
    if (buf == NULL) {
        printf(D_800A94A4, D_800A94CC, 79);
        exit(1);
    }
    buf[0] = 16;
    (*(u8 **)(E(D_8009D2D8) + 0x68))[1] = 11;
    (*(u8 **)(E(D_8009D2D8) + 0x68))[2] = 2;
    (*(u8 **)(E(D_8009D2D8) + 0x68))[3] = 36;
    (*(u8 **)(E(D_8009D2D8) + 0x68))[4] = 38;
    (*(u8 **)(E(D_8009D2D8) + 0x68))[5] = 70;
    *E(D_8009D2DC) = D_8009D2DC;
    *(s16 *)(E(D_8009D2DC) + 0x6C) = 5;
    buf = (u8 *)func_800305B4(5);
    *(u8 **)(E(D_8009D2DC) + 0x68) = buf;
    if (buf == NULL) {
        printf(D_800A94A4, D_800A94CC, 92);
        exit(1);
    }
    buf[0] = 8;
    (*(u8 **)(E(D_8009D2DC) + 0x68))[1] = 7;
    (*(u8 **)(E(D_8009D2DC) + 0x68))[2] = 18;
    (*(u8 **)(E(D_8009D2DC) + 0x68))[3] = 35;
    (*(u8 **)(E(D_8009D2DC) + 0x68))[4] = 24;
    *(u8 *)(E(D_8009D2DC) + 0x2C) = D_8009D314;
    *E(D_8009D2E0) = D_8009D2E0;
    *(s16 *)(E(D_8009D2E0) + 0x6C) = 7;
    buf = (u8 *)func_800305B4(7);
    *(u8 **)(E(D_8009D2E0) + 0x68) = buf;
    if (buf == NULL) {
        printf(D_800A94A4, D_800A94CC, 105);
        exit(1);
    }
    buf[0] = 8;
    (*(u8 **)(E(D_8009D2E0) + 0x68))[1] = 8;
    (*(u8 **)(E(D_8009D2E0) + 0x68))[2] = 8;
    (*(u8 **)(E(D_8009D2E0) + 0x68))[3] = 12;
    (*(u8 **)(E(D_8009D2E0) + 0x68))[4] = 28;
    (*(u8 **)(E(D_8009D2E0) + 0x68))[5] = 32;
    (*(u8 **)(E(D_8009D2E0) + 0x68))[6] = 49;
    *E(D_8009D2E8) = D_8009D2E8;
    *(s16 *)(E(D_8009D2E8) + 0x6C) = 3;
    buf = (u8 *)func_800305B4(3);
    *(u8 **)(E(D_8009D2E8) + 0x68) = buf;
    if (buf == NULL) {
        printf(D_800A94A4, D_800A94CC, 119);
        exit(1);
    }
    buf[0] = 12;
    (*(u8 **)(E(D_8009D2E8) + 0x68))[1] = 14;
    (*(u8 **)(E(D_8009D2E8) + 0x68))[2] = 8;
    *E(D_8009D2EC) = D_8009D2EC;
    *(s16 *)(E(D_8009D2EC) + 0x6C) = 8;
    buf = (u8 *)func_800305B4(8);
    *(u8 **)(E(D_8009D2EC) + 0x68) = buf;
    if (buf == NULL) {
        printf(D_800A94A4, D_800A94CC, 129);
        exit(1);
    }
    buf[0] = 12;
    (*(u8 **)(E(D_8009D2EC) + 0x68))[1] = 9;
    (*(u8 **)(E(D_8009D2EC) + 0x68))[2] = 12;
    (*(u8 **)(E(D_8009D2EC) + 0x68))[3] = 42;
    (*(u8 **)(E(D_8009D2EC) + 0x68))[4] = 1;
    (*(u8 **)(E(D_8009D2EC) + 0x68))[5] = 9;
    (*(u8 **)(E(D_8009D2EC) + 0x68))[6] = 1;
    (*(u8 **)(E(D_8009D2EC) + 0x68))[7] = 35;
    *E(D_8009D318) = D_8009D318;
    *(s16 *)(E(D_8009D318) + 0x6C) = 4;
    buf = (u8 *)func_800305B4(4);
    *(u8 **)(E(D_8009D318) + 0x68) = buf;
    if (buf == NULL) {
        printf(D_800A94A4, D_800A94CC, 144);
        exit(1);
    }
    buf[0] = 2;
    (*(u8 **)(E(D_8009D318) + 0x68))[1] = 2;
    (*(u8 **)(E(D_8009D318) + 0x68))[2] = 23;
    (*(u8 **)(E(D_8009D318) + 0x68))[3] = 2;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_80067F18);
#endif

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

#ifdef NON_MATCHING
void func_80068B60(void) {
    s32 i;
    func_80054118();
    func_8005370C();
    func_80053A7C();
    D_8009D8F4 = 0x80;
    D_8009D8F8 = 0x80;
    D_8009D8FC = 0x80;
    D_8009CF7C = (s32)&D_800E02BC;
    D_8009CF80 = 0x12;
    func_80069478();
    D_800B0C84 = 0xA;
    D_8009CF68 = 0x11;
    D_8009CF6C = (s32 *)&D_8009E084;
    D_8009CF70 = (SpawnEnt **)&D_8009E040;
    D_800B0868 = 6;
    D_800AFF94 = 0xF;
    D_800AFF98 = 3;
    D_800B0C8C = 0x12;
    D_800AFFC0 = 0x14;
    D_800AFFB0 = 5;
    D_800AFFB4 = 0;
    D_800B0AA4 = 5;
    D_800B0AC8 = 0x12;
    D_800B0AE4 = 3;
    D_800B008C = 0x15;
    D_800B0C30 = 0;
    D_800AFF80 = 2;
    D_800AFFC4 = 1;
    func_8002E228();
    printf(D_800A956C, D_800B0AA8);
    D_800D86E4 = 0x28;
    D_8009D0FC = (s32)&D_800E1A84;
    D_800D86E0 = (s32)&D_800E1EE4;
    D_800D86DC = 0xA;
    g_Args->unk228 = (s32)D_800E0904;
    func_8006B16C();
    func_8006AF88();
    func_80068A74();
    func_80069470();
    func_8006B1B4();
    func_8006B230(g_Args->unk220 < 0xA, (M2C_UNK)&D_800A9580);
    func_800600EC((s32 *)0x80141930);
    D_800B0BA0 = -1;
    D_800AFFA4 = 0;
    D_800D30BC = 0;
    *(s32 *)((u8 *)g_Args + 0x130) = (s32)&D_8009E0C8;
    D_8009D35C = D_8009D260;
    func_80030798(&D_8009E908);
    func_80069618();
    func_80069330();
    D_800D31B4 = D_8009D27C;
    func_80068AF0();
    func_8006B230(1, (M2C_UNK)&D_800A95A8);
    D_8009CF14[0] = 3;
    D_8009CF14[1] = (s32)&D_800E23FC;
    D_8009CF14[2] = 0x10;
    D_8009CF14[3] = (s32)&D_800E0784;
    D_8009CF14[5] = D_8009D30C;
    D_8009CF14[4] = D_8009D304;
    D_800E02B8 = D_8009E9C4;
    func_800691F4(D_8009E9C4);
    D_8009CE40 = (M2C_UNK (*)(s16))func_80069920;
    D_8009CE48 = (s32 (*)(s32))func_800691F4;
    D_8009CE44 = func_80069928;
    D_800E23E8 = 0x31;
    D_800E23D4 = 0x32;
    D_800E02B4 = 0x33;
    D_8009DB3C = 1;
    for (i = 0; i < 3; i++) {
        D_800E23DC[i] = 0x5B;
        D_800E23EC[i] = -1;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_80068B60);
#endif

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

void func_80069618(void) {
    func_800335B4();
    g_Weapons[0].flags |= 0x1800;
    ((WFlags *)&g_Weapons[2].flags)->f12 = 0;
    ((WFlags *)&g_Weapons[2].flags)->f11 = 0;
    ((WFlags *)&g_Weapons[3].flags)->f12 = 0;
    ((WFlags *)&g_Weapons[3].flags)->f11 = 0;
    ((WFlags *)&g_Weapons[1].flags)->f12 = 0;
    ((WFlags *)&g_Weapons[1].flags)->f11 = 0;
}

s32 func_80069684(u8 *arg0) {
    s32 *p;
    s32 idx;
    s32 *base;
    s32 a;
    s32 n;
    if (arg0[6] == D_8009D30C) {
        if (*(u8 **)(arg0 + 0x88) != 0) {
            *(s32 *)(*(u8 **)(arg0 + 0x88) + 0x8C) = 0;
            func_800395E8(arg0);
        }
    }
    if (arg0[6] == D_8009D2FC) {
        base = D_800E23EC;
        idx = *(s16 *)(arg0 + 0x122);
        p = &base[idx];
        a = *p;
        if (a >= 0) {
            ((void (*)(s32))func_80026C14)(a);
            *p = -1;
            D_800E23DC[idx] = 0x5B;
        }
        if (D_800E02B8 == 3) {
            D_800E23D8 = 0;
            n = D_8009E47C + 1;
            D_8009E47C = n;
            if (n == D_800E02B8) {
                func_8002E1B0();
                D_800E23F8 = 0;
            } else {
                D_800E23F8 = (rand() & 0xFF) + 0x28;
            }
        }
    }
    if (arg0[6] == D_8009D308) {
        func_8002D4E4(D_800A9668);
        return 1;
    }
    return 0;
}

s32 func_800697C8(void *a) {
    return func_80027248((D_800E02B8 == 1 && func_8004A370() != 0) ? 0x42 : 0x40, M2C_FIELD(a, s32 *, 0x90) + 0x14, 0);
}

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

void func_800699B8(void *arg0) {
    s32 idx;
    s32 a;
    s32 b;
    idx = M2C_FIELD(arg0, s16 *, 0x122);
    a = D_800E23DC[idx];
    b = D_800E23EC[idx];
    if ((M2C_FIELD(M2C_FIELD(arg0, void **, 0), s32 *, 0x20) >> 1) >= M2C_FIELD(arg0, s32 *, 0x34) && a == 0x5B) {
        if (b >= 0) {
            ((void (*)(s32))func_80026C14)(b);
            b = -1;
        }
        a = 0x5C;
    }
    if (b < 0) {
        b = func_80027248((u8)a, M2C_FIELD(arg0, s32 *, 0x90) + 0x14, 0);
    } else {
        func_80027394(M2C_FIELD(arg0, s32 *, 0x90) + 0x14, b, a, 0);
    }
    D_800E23DC[idx] = a;
    D_800E23EC[idx] = b;
}

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

void func_80069D60(u8 *obj) {
  MATRIX m1;
  MATRIX m2;
  VECTOR v;
  VECTOR out;
  u8 *parent;
  u8 *a;
  u8 *b;
  s16 *ang;
  u8 *tbl;
  s32 *t;
  MATRIX *pm1;
  VECTOR *po;
  VECTOR *pv;
  if (b || D_8009E4E8)
  {
    parent = *((u8 **) (obj + 0x8C));
  }
  else
  {
    parent = *((u8 **) (obj + 0x8C));
  }
  if (parent == ((void *) 0))
  {
    return;
  }
  a = *((u8 **) (obj + 0x90));
  ang = (s16 *) (a + 0x60);
  b = *((u8 **) (parent + 0x90));
  tbl = ((u8 *) D_800D86E0) + (D_800E0904[(D_8009D2FC * 0x70) + 0x5F] * 126);
  RotMatrixYXZ(ang, &m1);
  t = (s32 *) (a + 0x28);
  m1.t[0] = t[0];
  m1.t[1] = t[1];
  m1.t[2] = t[2];
  *((s16 *) (b + 0x60)) = ((*((s16 *) (a + 0x60))) + (*((s16 *) (a + 0xEC)))) & 0xFFF;
  *((s16 *) (b + 0x62)) = ang[1];
  *((s16 *) (b + 0x64)) = ang[2];
  *((s32 *) (b + 0x10)) = 2;
  RotMatrixYXZ(a + 0xEC, &m2);
  po = &out;
  m2.t[0] = *((s16 *) (tbl + 0x8));
  m2.t[1] = *((s16 *) (tbl + 0x14));
  m2.t[2] = *((s16 *) (tbl + 0x20));
  pm1 = &m1;
  asm volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(pm1) : "$12", "$13", "$14");
  asm volatile("lhu $13, 4(%0); lhu $12, 0(%0); sll $13, $13, 16; or $12, $12, $13; mtc2 $12, $0; lwc2 $1, 8(%0)" : : "r"(m2.t) : "$12", "$13");
  asm volatile("nop; nop; .word 0x4A486012");
  asm volatile("swc2 $25, 0(%0); swc2 $26, 4(%0); swc2 $27, 8(%0)" : : "r"(po) : "memory");
  asm volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(&m2) : "$12", "$13", "$14");
  asm volatile("nop; nop; .word 0x4A49E012");
  asm volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(&m2) : "$12", "$13", "$14", "memory");
  asm volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(&m2.m[0][1]) : "$12", "$13", "$14");
  asm volatile("nop; nop; .word 0x4A49E012");
  asm volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(&m2.m[0][1]) : "$12", "$13", "$14", "memory");
  asm volatile("lhu $12, 0(%0); lhu $13, 6(%0); lhu $14, 12(%0); mtc2 $12, $9; mtc2 $13, $10; mtc2 $14, $11" : : "r"(&m2.m[0][2]) : "$12", "$13", "$14");
  asm volatile("nop; nop; .word 0x4A49E012");
  asm volatile("mfc2 $12, $9; mfc2 $13, $10; mfc2 $14, $11; sh $12, 0(%0); sh $13, 6(%0); sh $14, 12(%0)" : : "r"(&m2.m[0][2]) : "$12", "$13", "$14", "memory");
  m2.t[0] = out.vx + pm1->t[0];
  m2.t[1] = out.vy + pm1->t[1];
  m2.t[2] = out.vz + pm1->t[2];
  pv = &v;
  v.vx = D_8009E4E4;
  v.vy = D_8009E4E8;
  v.vz = D_8009E4EC;
  asm volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; ctc2 $13, $1; lw $12, 8(%0); lw $13, 12(%0); lw $14, 16(%0); ctc2 $12, $2; ctc2 $13, $3; ctc2 $14, $4" : : "r"(&m2) : "$12", "$13", "$14");
  asm volatile("lhu $13, 4(%0); lhu $12, 0(%0); sll $13, $13, 16; or $12, $12, $13; mtc2 $12, $0; lwc2 $1, 8(%0)" : : "r"(pv) : "$12", "$13");
  asm volatile("nop; nop; .word 0x4A486012");
  asm volatile("swc2 $25, 0(%0); swc2 $26, 4(%0); swc2 $27, 8(%0)" : : "r"(pv) : "memory");
  t = (s32 *) (b + 0x28);
  t[0] = m2.t[0] + v.vx;
  t[1] = m2.t[1] + v.vy;
  t[2] = m2.t[2] + v.vz;
  if ((abs(t[0]) > 0x7FFF) || (abs(t[2]) > 0x7FFF))
  {
    *((u32 *) (parent + 0x7C)) |= 0x80000000;
  }
  else
  {
    *((u32 *) (parent + 0x7C)) &= 0x7FFFFFFF;
  }
}

u8 *func_8006A0D8(void) {
    s32 sp[2];
    u8 *e;
    u8 *p;
    s32 m;
    sp[0] = 0;
    sp[1] = 0;
    e = func_80033420(sp, D_8009D30C, 0x01100000, 0);
    if (e != NULL) {
        *(s32 *)(e + 0x34) = 0x1F4;
        *(s32 *)(e + 0x68) = (*(s32 *)(e + 0x68) & ~0x380) | 0x80;
        m = 0x200000;
        *(s32 *)(e + 0x48) |= m;
        p = *(u8 **)(e + 0x118);
        if (p != NULL) {
            *(s32 *)(p + 0x10) |= 4;
            *(s32 *)*(u8 **)(e + 0x118) |= m;
        }
    }
    return e;
}

#ifdef NON_MATCHING
void func_8006A174(Obj6A *arg0) {
    s32 a2;
    s32 *a0;
    s32 w, lo;
    s32 *t0;
    s32 junk;
    t0 = (s32 *)(arg0->sub + 0x28);
    if (D_800E02B8 == 1) {
        a2 = (0x800 - (arg0->f122 << 10)) & 0xFFF;
        arg0->f11C = D_8009E4D4;
        arg0->f120 = D_8009E4D8;
    } else {
        arg0->f11C = D_8009E4DC;
        arg0->f120 = D_8009E4E0;
        if (D_8009CD4C != 0) {
            a2 = *(s32 *)&D_800B0C5C;
            a2 = (a2 + 0x800) & 0xFFF;
        } else {
            a2 = gPsi;
            a2 = (a2 + 0x800) & 0xFFF;
        }
    }
    a0 = &D_800A0370[a2];
    lo = *(s16 *)a0;
    w = *(volatile s32 *)a0;
    t0[0] = (lo * 0x6D60) >> 12;
    t0[2] = (s32)((w >> 16) * 0x6D60) >> 12;
    t0[1] = D_8009E494;
    *(s16 *)(arg0->sub + 0x62) = (a2 + 0x400) & 0xFFF;
    *(s32 *)(arg0->sub + 0x10) = 2;
    ((void (*)(void *, s32, s32, void *))func_8006A41C)(arg0, junk, a2, arg0);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006A174);
#endif

void func_8006A2B8() {
    s32 i;
    s32 k;
    s32 f;
    u8 *e;
    u8 *o;
    D_800E23D8 = 0;
    e = (u8 *)D_800AFFC8 + D_800B0DC0 * 0x124;
    i = D_800B0DC0;
    if (i <= D_800B07A8) {
        do {
            if (e[4] != 0 && e[6] == D_8009D2FC) {
                k = D_800E23D8;
                *(s16 *)(e + 0x122) = k;
                D_800E23D8 = k + 1;
                o = func_8006A0D8();
                if (o != 0) {
                    f = *(s32 *)(o + 0x48);
                    D_800E23D0 = o;
                    f |= 0x100;
                    *(s32 *)(o + 0x48) = f;
                    *(u8 **)(e + 0x8C) = o;
                    *(u8 **)(o + 0x88) = e;
                }
                func_8006A174(e);
                if (o != 0) {
                    func_8005B96C(o, *(s32 *)(o + 0x90), *(void **)(o + 0x94));
                }
            }
            i++;
            e += 0x124;
        } while (i <= D_800B07A8);
    }
}

void func_8006A3E0(void) {
    if (func_80039A9C(0, D_8009D2FC, 3) != 0) {
        func_8006A2B8();
    }
}

void func_8006A41C(u8 *obj) {
    u8 *a;
    s16 *ang;
    s32 *pos;
    s32 acc;
    s32 v;
    s32 t;
    s32 d;
    s32 lim;
    s32 y;
    s32 idx;
    s32 s;
    s32 fwd;
    s32 side;
    s16 *r;

    acc = *(u16 *)(obj + 0x54);
    a = *(u8 **)(obj + 0x90);
    v = *(s16 *)(obj + 0x52) + acc;
    ang = (s16 *)(a + 0x60);
    pos = (s32 *)(a + 0x28);
    if (v < 0) {
        v = 0;
    } else if (v > D_8009E4C4) {
        v = D_8009E4C4;
    }
    *(s16 *)(obj + 0x52) = v;
    v = (s16)v >> 1;
    if (acc > 0) {
        v = *(u16 *)(obj + 0x54) << 6;
    } else if (*(s16 *)(obj + 0x50) > 0) {
        v = *(s16 *)(obj + 0x50);
    }
    d = v - ang[0];
    acc = D_8009E4B8;
    v = -acc;
    if (d > acc) {
        d = acc;
    } else if (d < v) {
        d = v;
    }
    ang[0] = (ang[0] + d) & 0xFFF;
    if (ang[0] > 0x800) {
        ang[0] -= 0x1000;
    }
    r = (s16 *)(a + 0xEC);
    t = *(s16 *)(obj + 0x50);
    if (t > 0) {
        t = 0;
    } else {
        t -= ang[0];
    }
    d = t - *r;
    lim = D_8009E4B8;
    if (d > lim) {
        d = lim;
    } else if (d < -lim) {
        d = -lim;
    }
    y = (*r + d) & 0xFFF;
    idx = (y + ang[0]) & 0xFFF;
    if (idx != 0) {
        s = D_800A0370[idx];
        fwd = (*(s16 *)(obj + 0x52) * (s >> 16)) >> 12;
        side = -(*(s16 *)(obj + 0x52) * (s16)s) >> 12;
    } else {
        fwd = *(s16 *)(obj + 0x52);
        side = 0;
    }
    if (y > 0x800) {
        y -= 0x1000;
    }
    *r = y;
    ang[1] = (ang[1] + *(u16 *)(obj + 0xA0)) & 0xFFF;
    if (ang[1] > 0x800) {
        ang[1] -= 0x1000;
    }
    {
    s32 dx = fwd * (s16)D_800A0370[ang[1] & 0xFFF];
    fwd = fwd * (*(volatile s32 *)&D_800A0370[ang[1] & 0xFFF] >> 16);
    pos[1] += side;
    pos[0] += dx >> 12;
    pos[2] += fwd >> 12;
    }
    t = *(s16 *)(obj + 0xA0) << 3;
    if (*(s16 *)(obj + 0x52) != 0) {
        lim = D_8009E4B8;
    } else {
        lim = D_8009E4BC;
        t = *(s16 *)(obj + 0xA0) << 5;
    }
    d = t - ang[2];
    if (d > lim) {
        d = lim;
    } else if (d < -lim) {
        d = -lim;
    }
    ang[2] = (ang[2] + d) & 0xFFF;
    if (ang[2] > 0x800) {
        ang[2] -= 0x1000;
    }
    *(s32 *)(*(u8 **)(obj + 0x90) + 0x10) = 2;
    *(s32 *)(a + 0x9C) = 2;
    func_80069D60(obj);
}

void func_8006A700(void *a) {
    u16 v = D_8009E4C0;
    s32 q = ((s32)(v << 16) >> 5) / D_8009E4AC;
    M2C_FIELD(a, s16 *, 0x50) = 0;
    M2C_FIELD(a, s8 *, 0xA5) = 2;
    M2C_FIELD(a, u16 *, 0x52) = v;
    M2C_FIELD(a, s16 *, 0xA0) = q;
}

void func_8006A75C(u8 *arg0) {
    u16 u;
    u16 t = D_8009E4C0;
    s32 d = -((s32)(t << 16) >> 5);
    d = d / D_8009E4B0;
    u = D_8009E4A0;
    arg0[0xA5] = 3;
    *(u16 *)(arg0 + 0x52) = t;
    *(u16 *)(arg0 + 0x50) = u;
    *(s16 *)(arg0 + 0xA0) = d;
}

void func_8006A7C4(u8 *arg0) {
    u16 u;
    u16 t = D_8009E4C0;
    s32 d = (s32)(t << 16) >> 5;
    d = d / D_8009E4B0;
    u = D_8009E4A0;
    arg0[0xA5] = 4;
    *(u16 *)(arg0 + 0x52) = t;
    *(u16 *)(arg0 + 0x50) = u;
    *(s16 *)(arg0 + 0xA0) = d;
}

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

s32 func_8006A8D0(void *arg0) {
    s32 ang;
    s32 r;
    if (M2C_FIELD(arg0, s32 *, 0x6C) != 0) {
        M2C_FIELD(arg0, s32 *, 0x6C)--;
        return 0;
    }
    ang = M2C_FIELD(M2C_FIELD(arg0, void **, 0x90), s16 *, 0x62);
    r = rand();
    if (M2C_FIELD(arg0, u8 *, 0xA5) == 2) {
        if (D_800E02B8 == 1) {
            if ((ang >= (r & 0xFF) + 0x4B0 && ang < 0x641) || (ang >= (r & 0x3FF) - 0x44C && ang < 0x321)) {
                return 1;
            }
        } else {
            if (ang >= (r & 0x7FF) - 0x800 && ang < 0x12D) {
                return 1;
            }
        }
    } else {
        if (D_800E02B8 == 1) {
            if ((ang >= -0x3E8 && ang <= -0x64 - (r & 0x1FF)) || ang >= 0x6A4 || ang <= -0x384 - (r & 0x3FF)) {
                return 1;
            }
        } else {
            if (ang >= 0x244 || ang <= -0x6A4 - (r & 0x3FF)) {
                return 1;
            }
        }
    }
    return 0;
}

#ifdef NON_MATCHING
void func_8006AA20(u8 **arg0) {
    SVECTOR a;
    SVECTOR b;
    s32 v[4];
    s16 ang;
    s32 d;
    s32 t;
    s32 turn;
    s32 lim;
    u8 *obj;
    u8 *pos;
    u8 *rot;
    u8 *base;

    obj = *arg0;
    base = M2C_FIELD(obj, u8 **, 0x90);
    pos = base + 0x28;
    rot = base + 0x60;
    if (M2C_FIELD(obj, void **, 0x8C) == NULL) {
        func_8006A8A8(obj);
    }
    switch (M2C_FIELD(obj, u8 *, 0xA5)) {
    case 0:
        func_8006A700(obj);
        M2C_FIELD(obj, s32 *, 0x6C) = 0x78;
        break;
    case 1:
    case 2:
        if (func_8006A8D0(obj) != 0) {
            if (M2C_FIELD(obj, u8 *, 0xA5) == 1) {
                func_8006A75C(obj);
            } else {
                func_8006A7C4(obj);
            }
        }
        break;
    case 3:
    case 4:
        {
        u8 *tgt;
        d = (M2C_FIELD(rot, s16 *, 2) - RAT(M2C_FIELD(obj, s16 *, 0x11C) - M2C_FIELD(pos, s32 *, 0), M2C_FIELD(obj, s16 *, 0x120) - M2C_FIELD(pos, s32 *, 8))) & 0xFFF;
        tgt = M2C_FIELD(obj, u8 **, 0x8C);
        if (tgt != NULL) {
            s32 *flags;
            flags = M2C_FIELD(tgt, s32 **, 0x118);
            if (flags != NULL) {
                *flags |= 0x20;
            }
        }
        if ((d < D_8009E4D0) || ((0x1000 - D_8009E4D0) < d) || (D_8009E49C < M2C_FIELD(pos, s32 *, 4))) {
            func_8006A828(obj);
        }
        }
        break;
    case 5:
        {
        u8 *tgt = M2C_FIELD(obj, u8 **, 0x8C);
        if (tgt != NULL) {
            s32 *flags;
            flags = M2C_FIELD(tgt, s32 **, 0x118);
            if (flags != NULL) {
                *flags |= 0x20;
            }
        }
        if ((u32) (((M2C_FIELD(rot, s16 *, 2) - RAT(M2C_FIELD(obj, s16 *, 0x11C) - M2C_FIELD(pos, s32 *, 0), M2C_FIELD(obj, s16 *, 0x120) - M2C_FIELD(pos, s32 *, 8))) & 0xFFF) - 0x401) < 0x7FFU) {
            if (D_8009CDA8 & 1) {
                func_8006A87C(obj);
            } else {
                func_8006A850(obj);
            }
        } else if (D_8009E49C < M2C_FIELD(pos, s32 *, 4)) {
            M2C_FIELD(obj, s16 *, 0x50) = 0;
        }
        }
        break;
    case 6:
    case 7:
        if ((M2C_FIELD(obj, s16 *, 0x50) == 0) && (M2C_FIELD(obj, s32 *, 0x6C) != 0)) {
            M2C_FIELD(obj, s32 *, 0x6C) = (rand() & 0x3F) + 0x28;
            if (M2C_FIELD(obj, u8 *, 0xA5) == 7) {
                M2C_FIELD(obj, u8 *, 0xA5) = 2;
            } else {
                M2C_FIELD(obj, u8 *, 0xA5) = 1;
            }
        } else {
            if ((M2C_FIELD(pos, s32 *, 4) < D_8009E498) && (M2C_FIELD(obj, s16 *, 0x50) == D_8009E4A8)) {
                M2C_FIELD(obj, s16 *, 0x50) = 0;
            }
            if (M2C_FIELD(obj, s32 *, 0x6C) == 0) {
                ang = RAT((s16) -M2C_FIELD(pos, s32 *, 0), -M2C_FIELD(pos, s32 *, 8));
                if (ang >= 0x801) {
                    ang -= 0x1000;
                }
                t = ang - 0x400;
                if (M2C_FIELD(obj, u8 *, 0xA5) == 6) {
                    t = ang + 0x400;
                }
                turn = ((t & 0xFFF) - M2C_FIELD(rot, s16 *, 2)) & 0xFFF;
                if (turn >= 0x801) {
                    turn -= 0x1000;
                }
                a.vx = 0;
                a.vy = 0;
                a.vz = 0;
                b.pad = 0;
                b.vx = M2C_FIELD(pos, s32 *, 0);
                b.vz = M2C_FIELD(pos, s32 *, 8);
                if (func_8006D3C0(&a, &b) >= 0x6D60) {
                    if (turn != 0) {
                        lim = D_8009E4BC;
                        M2C_FIELD(obj, u16 *, 0x52) = 0;
                        if ((lim < turn) || (lim = -lim, (turn < lim))) {
                            turn = lim;
                        }
                        M2C_FIELD(obj, s16 *, 0xA0) = turn;
                    } else {
                        M2C_FIELD(obj, s32 *, 0x6C) = -1;
                        M2C_FIELD(obj, u16 *, 0x52) = D_8009E4C0;
                        if (M2C_FIELD(obj, u8 *, 0xA5) == 6) {
                            t = -((s16) M2C_FIELD(obj, u16 *, 0x52) << 0xB);
                        } else {
                            t = (s16) M2C_FIELD(obj, u16 *, 0x52) << 0xB;
                        }
                        M2C_FIELD(obj, s16 *, 0xA0) = t / D_8009E4AC;
                    }
                }
            }
        }
        break;
    case 8:
        t = D_800A0370[(u16) M2C_FIELD(rot, s16 *, 2) & 0xFFF];
        { s32 *q = (s32 *)(base + 0xD4);
        v[0] = q[0];
        v[1] = q[1];
        a.vx = t;
        a.vy = 0;
        a.vz = t >> 0x10;
        v[2] = q[2]; }
        d = func_800743E8(D_800E2F2C, v, M2C_FIELD(obj, s16 *, 0x22) - M2C_FIELD(obj, s16 *, 0x20), 0x64, &a, 0, 1);
        a.vx = 0;
        a.vy = 0;
        a.vz = 0;
        b.pad = 0;
        b.vx = M2C_FIELD(pos, s32 *, 0);
        b.vz = M2C_FIELD(pos, s32 *, 8);
        t = func_8006D3C0(&a, &b) < 0x6D61;
        if ((d != 0) || (t == 0)) {
            M2C_FIELD(obj, s8 *, 0xA4) = 2;
            func_80038C8C(obj);
            return;
        }
        break;
    default:
        break;
    }
    func_8006A41C(obj);
    if (((M2C_FIELD(M2C_FIELD(obj, u8 **, 0), s32 *, 0x20) >> 1) >= M2C_FIELD(obj, s32 *, 0x34)) && (M2C_FIELD(obj, void **, 0x100) == NULL)) {
        *(s16 *)0x1F8003E0 = 0;
        *(s16 *)0x1F800002 = 0;
        *(s16 *)0x1F800004 = 0;
        M2C_FIELD(obj, void **, 0x100) = func_8006421C(obj, (s16 *)0x1F800000, -1, 0, 0x258);
    }
    func_800699B8(obj);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006AA20);
#endif

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

#ifdef NON_MATCHING
void func_8006B2C4(u8 *ctx, SubTri *t, s32 depth, s32 unused) {
    SubTri c;
    SubTri6 v;
    u8 fl[3];
    s32 nd;
    s32 j;
    s32 k;
    s32 ia;
    s32 ib;
    s32 ic;
    s32 cls;
    s32 mac;
    u32 f0;
    u32 f1;
    u32 f2;
    u32 c0;
    u32 c1;
    u32 c2;
    u32 uvw;
    u32 gteflg;
    u32 sxy;
    s32 sz;
    s32 sx;
    s32 sy;
    s32 x;
    s32 y;
    s32 z;
    u32 fm;
    PolyGT3 *prim;
    u32 *ot;

    memset(fl, 0, 3);
    nd = depth - 1;
    if (depth <= 0) return;

    /* vertices 0-2 are the triangle's own, 3-5 are the edge midpoints (01, 12, 02) */
    for (k = 0; k < 3; k++) {
        v.p[k] = t->p[k];
        v.sxy[k] = t->sxy[k];
        v.flg[k] = t->flg[k];
        v.col[k] = t->col[k];
        v.uv[k] = t->uv[k];
    }
    for (k = 0; k < 3; k++) {
        ia = (k == 2) ? 0 : k;
        ib = (k == 0) ? 1 : 2;
        x = (t->p[ia].vx + t->p[ib].vx) >> 1;
        y = (t->p[ia].vy + t->p[ib].vy) >> 1;
        z = (t->p[ia].vz + t->p[ib].vz) >> 1;
        ((u32 *)&v.p[3 + k])[0] = (x & 0xFFFF) | (y << 16);
        ((u32 *)&v.p[3 + k])[1] = z & 0xFFFF;
        gte_ldv0_mt(((u32 *)&v.p[3 + k])[0], z);
        gte_rtps();
        c0 = t->col[ia];
        c1 = t->col[ib];
        v.col[3 + k] = ((c0 & 0xFF) + (c1 & 0xFF)) >> 1 | ((((c0 >> 8) & 0xFF) + ((c1 >> 8) & 0xFF)) >> 1) << 8 | (((c0 >> 16) + (c1 >> 16)) >> 1) << 16;
        c0 = t->uv[ia];
        c1 = t->uv[ib];
        v.uv[3 + k] = ((c0 & 0xFF) + (c1 & 0xFF)) >> 1 | ((((c0 & 0xFF00) + (c1 & 0xFF00)) >> 1) & 0xFF00);
        gte_mfsxy2(sxy);
        gte_mfsz3(sz);
        sz >>= 2;
        sy = (s32)sxy >> 16;
        sx = (s16)sxy;
        gte_cfFLAG(gteflg);
        v.sxy[3 + k] = sxy;
        fm = sz;
        if (gteflg & 0x46000) fm |= 0x20000000;
        if (sy >= 0x300 || sx >= 0x300 || sy < -0x100 || sx < -0x100) fm |= 0x10000000;
        if (sx < 0) fm |= 0x08000000;
        if (sx >= 0x21C) fm |= 0x04000000;
        if (sy < 0) fm |= 0x02000000;
        if (sy >= 0xF0) fm |= 0x01000000;
        v.flg[3 + k] = fm;
    }

    /* jobs 0-2: corner triangles, 3: centre triangle, 4-6: edge gap fillers */
    for (j = 0; j < 7; j++) {
        switch (j) {
        case 0: ia = 0; ib = 3; ic = 5; break;
        case 1: ia = 1; ib = 4; ic = 3; break;
        case 2: ia = 2; ib = 5; ic = 4; break;
        case 3: ia = 3; ib = 4; ic = 5; break;
        case 4: ia = 0; ib = 1; ic = 3; break;
        case 5: ia = 1; ib = 2; ic = 4; break;
        default: ia = 2; ib = 0; ic = 5; break;
        }
        if (j == 4 && (*(u32 *)(ctx + 0x50) & 0x2000000)) return;
        c.p[0] = v.p[ia];
        c.p[1] = v.p[ib];
        c.p[2] = v.p[ic];
        c.sxy[0] = v.sxy[ia];
        c.sxy[1] = v.sxy[ib];
        c.sxy[2] = v.sxy[ic];
        c.flg[0] = v.flg[ia];
        c.flg[1] = v.flg[ib];
        c.flg[2] = v.flg[ic];
        c.col[0] = v.col[ia];
        c.col[1] = v.col[ib];
        c.col[2] = v.col[ic];
        c.uv[0] = v.uv[ia];
        c.uv[1] = v.uv[ib];
        c.uv[2] = v.uv[ic];
        if (j < 4) {
            f0 = c.flg[0];
            f1 = c.flg[1];
            f2 = c.flg[2];
            if ((f0 & f1 & f2) & 0x3F000000) {
                cls = 0;
            } else if ((f0 | f1 | f2) & 0x30000000) {
                cls = 2;
            } else {
                gte_ldsxy3c(c.sxy);
                gte_nclip();
                gte_mfMAC0(mac);
                cls = mac > 0;
            }
            if (cls == 0) continue;
            if (cls != 1 && nd == 0) continue;
            if (cls == 2) {
                ((void (*)(u8 *, SubTri *, s32))func_8006B2C4)(ctx, &c, nd);
                continue;
            }
        } else {
            if (fl[j - 4] == 0) continue;
            if ((v.flg[ia] | v.flg[ib]) & 0x20000000) continue;
        }
        /* emit a POLY_GT3 */
        prim = *(PolyGT3 **)(ctx + 0x10);
        if (*(u32 *)(ctx + 0x14) >= (u32)(prim + 1)) {
            *(PolyGT3 **)(ctx + 0x10) = prim + 1;
            prim->tag = 0x09000000;
            prim->xy0 = c.sxy[0];
            prim->xy1 = c.sxy[1];
            prim->xy2 = c.sxy[2];
            c0 = c.col[0];
            c1 = c.col[1];
            c2 = c.col[2];
            if (D_800D3068 == 2) {
                c0 |= 0xFF;
                c1 |= 0xFF;
                c2 |= 0xFF;
            }
            prim->rgb2 = c2;
            prim->rgb1 = c1;
            prim->rgb0 = c0 | *(u32 *)(ctx + 0x50);
            uvw = *(u32 *)&c.uv[0];
            prim->uv0 = (uvw & 0xFFFF) | (*(u32 *)(ctx + 0x7C) & 0xFFFF0000);
            prim->uv1 = (uvw >> 16) | (*(u32 *)(ctx + 0x7C) << 16);
            prim->uv2 = c.uv[2];
            ot = *(u32 **)(ctx + 0x1C);
            prim->tag = (prim->tag & 0xFF000000) | *ot;
            *ot = (u32)prim & 0xFFFFFF;
            if (*(s16 *)(ctx + 0x46) >= 0) {
                *(PolyGT3 **)(ctx + 0x20) = prim;
                ((void (*)(u8 *, u8 *))func_8006C8F4)((u8 *)c.col, ctx);
            }
        }
        if (j == 0) { fl[2] = 1; fl[0] = 1; }
        else if (j == 1) { fl[1] = 1; fl[0] = 1; }
        else if (j == 2) { fl[2] = 1; fl[1] = 1; }
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006B2C4);
#endif

void func_8006C494(TerrTri *t) {
    TerrVert *v;

    {
        TerrVCache *e;
        s32 f;
        s32 c;
        s32 sxy;
        s32 otz;
        s32 y;
        s32 flg;
        s32 xy;
        s32 z;

        e = &t->cache[t->idx[0]];
        f = e->flags;
        if (f < 0) {
            s32 s;
            s32 col;

            s = e->sxy;
            t->flags[0] = f;
            col = (f << 8) >> 24;
            if (col >= 0) {
                col = e->col;
            }
            t->sxy[0] = s;
            t->col[0] = col;
        } else {
            v = &t->verts[t->idx[0]];
            xy = v->xy;
            z = v->z;
            gte_ldVXYZ0(xy, z);
            gte_rtps();
            c = (f << 8) >> 24;
            if (c >= 0) {
                c = t->cols[c].col;
                e->col = c;
            }
            t->col[0] = c;
            gte_stSXY2_SZOTZ(sxy, otz);
            f |= otz;
            e->sxy = sxy;
            t->sxy[0] = sxy;
            y = sxy >> 16;
            sxy = (sxy << 16) >> 16;
            gte_cfFLAG(flg);
            if (flg & 0x46000) {
                f |= 0x20000000;
            }
            if (y > 0x200 || sxy >= 0x300 || y < -0x100 || sxy < -0x100) {
                f |= 0x10000000;
            }
            if (sxy < 0) {
                f |= 0x8000000;
            }
            if (sxy >= 0x21C) {
                f |= 0x4000000;
            }
            if (y < 0) {
                f |= 0x2000000;
            }
            if (y >= 0xF0) {
                f |= 0x1000000;
            }
            f |= 0x80000000;
            e->flags = f;
            t->flags[0] = f;
        }
    }
    {
        TerrVCache *e;
        s32 f;
        s32 c;
        s32 sxy;
        s32 otz;
        s32 y;
        s32 flg;
        s32 xy;
        s32 z;

        e = &t->cache[t->idx[1]];
        f = e->flags;
        if (f < 0) {
            s32 s;
            s32 col;

            s = e->sxy;
            t->flags[1] = f;
            col = (f << 8) >> 24;
            if (col >= 0) {
                col = e->col;
            }
            t->sxy[1] = s;
            t->col[1] = col;
        } else {
            v = &t->verts[t->idx[1]];
            xy = v->xy;
            z = v->z;
            gte_ldVXYZ0(xy, z);
            gte_rtps();
            c = (f << 8) >> 24;
            if (c >= 0) {
                c = t->cols[c].col;
                e->col = c;
            }
            t->col[1] = c;
            gte_stSXY2_SZOTZ(sxy, otz);
            f |= otz;
            e->sxy = sxy;
            t->sxy[1] = sxy;
            y = sxy >> 16;
            sxy = (sxy << 16) >> 16;
            gte_cfFLAG(flg);
            if (flg & 0x46000) {
                f |= 0x20000000;
            }
            if (y > 0x200 || sxy >= 0x300 || y < -0x100 || sxy < -0x100) {
                f |= 0x10000000;
            }
            if (sxy < 0) {
                f |= 0x8000000;
            }
            if (sxy >= 0x21C) {
                f |= 0x4000000;
            }
            if (y < 0) {
                f |= 0x2000000;
            }
            if (y >= 0xF0) {
                f |= 0x1000000;
            }
            f |= 0x80000000;
            e->flags = f;
            t->flags[1] = f;
        }
    }
    {
        TerrVCache *e;
        s32 f;
        s32 c;
        s32 sxy;
        s32 otz;
        s32 y;
        s32 flg;
        s32 xy;
        s32 z;

        e = &t->cache[t->idx[2]];
        f = e->flags;
        if (f < 0) {
            s32 s;
            s32 col;

            s = e->sxy;
            t->flags[2] = f;
            col = (f << 8) >> 24;
            if (col >= 0) {
                col = e->col;
            }
            t->sxy[2] = s;
            t->col[2] = col;
        } else {
            v = &t->verts[t->idx[2]];
            xy = v->xy;
            z = v->z;
            gte_ldVXYZ0(xy, z);
            gte_rtps();
            c = (f << 8) >> 24;
            if (c >= 0) {
                c = t->cols[c].col;
                e->col = c;
            }
            t->col[2] = c;
            gte_stSXY2_SZOTZ(sxy, otz);
            f |= otz;
            e->sxy = sxy;
            t->sxy[2] = sxy;
            y = sxy >> 16;
            sxy = (sxy << 16) >> 16;
            gte_cfFLAG(flg);
            if (flg & 0x46000) {
                f |= 0x20000000;
            }
            if (y > 0x200 || sxy >= 0x300 || y < -0x100 || sxy < -0x100) {
                f |= 0x10000000;
            }
            if (sxy < 0) {
                f |= 0x8000000;
            }
            if (sxy >= 0x21C) {
                f |= 0x4000000;
            }
            if (y < 0) {
                f |= 0x2000000;
            }
            if (y >= 0xF0) {
                f |= 0x1000000;
            }
            f |= 0x80000000;
            e->flags = f;
            t->flags[2] = f;
        }
    }
}

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

#ifdef NON_MATCHING
s32 func_8006CB88(u8 *t) {
    u8 c0[4];
    u8 c1[4];
    u8 c2[4];
    u8 c3[4];
    u8 c4[4];
    u8 c5[4];
    s32 f0;
    s32 f1;
    s32 f2;
    s32 all;
    s32 any;
    s32 clip;
    s32 any40;
    s32 z0;
    s32 z1;
    s32 z2;
    s32 mn;
    s32 mx;
    s32 z;
    s32 idx;
    u32 semi;
    u16 flags;
    s32 bit;
    s32 grey;
    s32 dim;
    u8 *c;
    s32 opz;

    f0 = *(s32 *)(t + 0x64);
    f1 = *(s32 *)(t + 0x68);
    f2 = *(s32 *)(t + 0x6C);
    all = f0 & f1 & f2 & 0x3F000000;
    any = f0 | f1 | f2;
    clip = any & 0x30000000;
    any40 = any & 0x40000000;
    z0 = (s16)f0;
    z1 = (s16)f1;
    z2 = (s16)f2;
    if (all) {
        goto ret0;
    }
    if (!clip) {
        gte_ldsxy3c(t + 0x58);
        gte_nclip();
        gte_mfMAC0(opz);
        if (opz <= 0) {
            goto ret0;
        }
    }
    if (z0 < z1) {
        mn = (z0 < z2) ? z0 : z2;
        mx = (z1 < z2) ? z2 : z1;
    } else {
        mn = (z1 < z2) ? z1 : z2;
        mx = (z0 < z2) ? z2 : z0;
    }
    z = (mx * 3 + mn) >> 2;
    *(s32 *)(t + 0x48) = z;
    idx = (u32)z >> 2;
    semi = *(u16 *)(t + 0x44) >> 15;
    if (idx > 0xFFF) {
        idx = 0xFFF;
    }
    if (idx < 0) {
        idx = 0;
    }
    *(u32 *)(t + 0x50) = (semi << 25) | 0x34000000;
    *(u32 **)(t + 0x1C) = *(u32 **)(t + 0x18) + idx;
    flags = *(u16 *)(t + 0x44);
    bit = 0x2000 << D_800B8F24;
    if (flags & bit) {
        dim = D_8009CABD;
        grey = D_8009CABE;
        if (*(s32 *)(t + 0x70) < 0) {
            c = *(u8 **)(t + 0x8) + *(u16 *)(t + 0x3E) * 4;
            if (dim) {
                c0[1] = ((c[0] + c[1] + c[2]) >> 3) + 0x30;
                c0[0] = 0;
                c0[2] = 0;
                c0[3] = c[3];
                c = c0;
            } else if (grey) {
                c1[0] = 0x18;
                c1[1] = 0x18;
                c1[2] = 0x18;
                c1[3] = c[3];
                c = c1;
            }
            *(u32 *)(*(u8 **)(t + 0x20) + 4) = *(u32 *)c | *(u32 *)(t + 0x50);
        }
        if (*(s32 *)(t + 0x74) < 0) {
            c = *(u8 **)(t + 0x8) + *(u16 *)(t + 0x40) * 4;
            if (dim) {
                c2[1] = ((c[0] + c[1] + c[2]) >> 3) + 0x30;
                c2[0] = 0;
                c2[2] = 0;
                c = c2;
            } else if (grey) {
                c3[0] = 0x18;
                c3[1] = 0x18;
                c3[2] = 0x18;
                c = c3;
            }
            *(u32 *)(*(u8 **)(t + 0x20) + 0x10) = *(u32 *)c;
        }
        if (*(s32 *)(t + 0x78) < 0) {
            c = *(u8 **)(t + 0x8) + *(u16 *)(t + 0x42) * 4;
            if (dim) {
                c4[1] = ((c[0] + c[1] + c[2]) >> 3) + 0x30;
                c4[0] = 0;
                c4[2] = 0;
                c = c4;
            } else if (grey) {
                c5[0] = 0x18;
                c5[1] = 0x18;
                c5[2] = 0x18;
                c = c5;
            }
            *(u32 *)(*(u8 **)(t + 0x20) + 0x1C) = *(u32 *)c;
        }
        if (!any40) {
            flags &= ~bit;
        }
    }
    *(u16 *)(t + 0x44) = flags;
    if (clip || (*(u32 *)(t + 0x48) < *(u32 *)(t + 0x4C) && !semi)) {
        return 2;
    }
    return 1;
ret0:
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006CB88);
#endif

#ifdef NON_MATCHING
void func_8006CEA4(s32 arg0) {
    s16 pos[3];
    s32 buf[17];
    s32 res;
    s32 *p;
    u8 *it;
    u8 *obj;
    u8 *poly;
    u8 *vt;
    u32 i;
    u32 s6;
    u32 nObj;
    s32 n;
    s32 r;
    s32 v0;
    s32 v1;
    s32 a3;
    u32 w;
    u16 s5;
    u32 cnt;
    u16 hw;
    s16 sh;
    u8 *spad = (u8 *)0x1F800000;
    u8 *m = D_800E2DA8;

    SP(0x00) = *(s32 *)(m + 0x18);
    SP(0x04) = *(s32 *)(m + 0x24);
    SP(0x08) = *(s32 *)(m + 0x1C);
    SP(0x0C) = *(s32 *)(*(s32 *)(D_800D2FC0 + 4) + 8);
    SP(0x24) = *(s32 *)(*(s32 *)(D_800D2FC0 + 4) + 0x10);
    SP(0x10) = *(s32 *)(m + D_800B8F24 * 4 + 0x28);
    SP(0x14) = *(s32 *)(m + D_800B8F24 * 4 + 0x28) + *(s32 *)(m + 8) * 0x28;
    SP(0x38) = D_8009CAC0;
    SP(0x18) = (s32)D_800B8F88[D_800B8F24].org;
    *(B4 *)(spad + 0x2C) = *(B4 *)&D_800AFF64;
    *(B8 *)(spad + 0x30) = *(B8 *)&D_800AFF68;
    nObj = D_800E2438;
    if (D_800D3068 == 0) {
        SP(0x4C) = 0;
        SP(0x54) = 0;
    } else {
        SP(0x4C) = D_8009E9B8;
        SP(0x54) = D_8009E9B4;
    }
    if (*(s32 *)(m + 0xC) != 0) {
        pos[1] = *(u16 *)(&g_CamPosX + 4);
        pos[0] = *(u16 *)&g_CamPosX;
        pos[2] = *(u16 *)(&g_CamPosX + 8);
        func_800705A8(D_800E2F54, pos, 0, (s32)&res);
        SP(0x28) = *(s32 *)(m + 0x10) + (((res & 0x3F) - 1) << *(s32 *)(m + 0xC));
    } else {
        SP(0x28) = 0;
    }
    i = 0;
    cnt = *(u16 *)D_800E2DA8;
    if (cnt != 0) {
        p = (s32 *)(SP(0x04) + 4);
        do {
            i += 1;
            *p &= 0xFF0000;
            p = (s32 *)((u8 *)p + 0xC);
        } while (i < cnt);
    }
    s6 = 0;
    if (nObj != 0) {
        do {
            obj = D_800E2448[s6];
            n = *(s32 *)(obj + 0x1C);
            it = *(u8 **)(obj + 0x18);
            if (n != 0) {
                do {
                    *(B12 *)(spad + 0x3C) = *(B12 *)it;
                    s5 = SPH(0x44);
                    SP(0x20) = arg0 + *(u16 *)it * 0x28;
                    if (SP(0x28) == 0 || ((u8 *)SP(0x28))[s5 & 0x3F] != 0) {
                        if (SP(0x38) != 0) {
                            func_8006D470(spad);
                        } else {
                            func_8006C494(spad);
                        }
                        r = func_8006CB88(spad);
                        if (r != 0) {
                            if (r == 2) {
                                w = SP(0x40);
                                poly = (u8 *)SP(0x20);
                                vt = (u8 *)SP(0x00) + SPH(0x3E) * 8;
                                SP(0x7C) = *(u16 *)(poly + 0x1A) | (*(u16 *)(poly + 0xE) << 16);
                                buf[16] = *(u16 *)(it + 0x10);
                                buf[15] = *(s32 *)(it + 0xC);
                                buf[1] = *(u16 *)(vt + 4);
                                buf[0] = *(s32 *)vt;
                                vt = (u8 *)SP(0x00) + (w & 0xFFFF) * 8;
                                buf[2] = *(s32 *)vt;
                                buf[3] = *(u16 *)(vt + 4);
                                vt = (u8 *)SP(0x00) + ((s32)w >> 16) * 8;
                                buf[6] = SP(0x58);
                                buf[9] = SP(0x64);
                                buf[7] = SP(0x5C);
                                buf[10] = SP(0x68);
                                buf[4] = *(s32 *)vt;
                                buf[5] = *(u16 *)(vt + 4);
                                buf[8] = SP(0x60);
                                v0 = SP(0x70);
                                v1 = SP(0x74);
                                a3 = SP(0x78);
                                buf[11] = SP(0x6C);
                                if (v0 < 0) {
                                    v0 = *(s32 *)(poly + 4);
                                }
                                if (v1 < 0) {
                                    v1 = *(s32 *)(poly + 0x10);
                                }
                                if (a3 < 0) {
                                    a3 = *(s32 *)(poly + 0x1C);
                                }
                                a3 &= 0xFFFFFF;
                                buf[12] = v0 & 0xFFFFFF;
                                buf[13] = v1 & 0xFFFFFF;
                                buf[14] = a3;
                                func_8006B2C4(spad, buf, SP(0x54), a3);
                            } else {
                                poly = (u8 *)SP(0x20);
                                *(s32 *)(poly + 8) = SP(0x58);
                                *(s32 *)(poly + 0x14) = SP(0x5C);
                                *(s32 *)(poly + 0x20) = SP(0x60);
                                v1 = SP(0x70);
                                if (v1 >= 0) {
                                    *(s32 *)(poly + 4) = v1 | SP(0x50);
                                }
                                v1 = SP(0x74);
                                if (v1 >= 0) {
                                    *(s32 *)(poly + 0x10) = v1;
                                }
                                v1 = SP(0x78);
                                if (v1 >= 0) {
                                    *(s32 *)(poly + 0x1C) = v1;
                                }
                                p = (s32 *)SP(0x1C);
                                *(s32 *)poly = (*(s32 *)poly & 0xFF000000) | *p;
                                *p = (s32)poly & 0xFFFFFF;
                                sh = SPH(0x46);
                                if (sh >= 0) {
                                    func_8006C8F4(it, spad, sh);
                                }
                            }
                        }
                        hw = SPH(0x44);
                        if (hw != s5) {
                            *(u16 *)(it + 8) = hw;
                        }
                    }
                    it += 0x14;
                } while (--n != 0);
            }
            s6 += 1;
        } while (s6 < nObj);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006CEA4);
#endif

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

#ifdef NON_MATCHING
void func_8006D470(u8 *arg0) {
    s32 a0, a1, a2;
    s32 idx;
    u32 *v;
    s32 f;
    s32 col;
    s32 d;
    s32 lim;
    s32 t;
    s32 r, g, b;
    s32 sxy, sz, flag;
    s32 sx, sy;
    u8 *p;

    a0 = 0;
    a1 = 0;
    a2 = 0;
    VERT(0x3E, 0x58, 0x64, 0x70, a0);
    VERT(0x40, 0x5C, 0x68, 0x74, a1);
    VERT(0x42, 0x60, 0x6C, 0x78, a2);
    if ((a0 | a1 | a2) != 0) *(u16 *)(arg0 + 0x44) |= 0x2000 << D_800B8F24;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006D470);
#endif

void func_8006DD20(s32 *a, s32 *b, s32 *c, s32 *out) {
  s32 d[4];
  s32 n1[4];
  s32 n2[4];
  s32 t[4];
  s32 r[4];
  s32 *p;
  u32 mask;
  s32 bit;
  s32 shift;
  s32 *p2;
  u32 mask2;
  s32 bit2;
  s32 shift2;
  p = d;
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  for (mask = 0x40000000, bit = 30; bit > 13; bit--, mask >>= 1)
  {
    if (((abs(p[0]) & mask) || (abs(p[1]) & mask)) || (abs(p[2]) & mask))
    {
      break;
    }
  }

  shift = bit - 13;
  if (shift > 0)
  {
    t[0] = p[0] >> shift;
    t[1] = p[1] >> shift;
    t[2] = p[2] >> shift;
  }
  else
  {
    t[0] = p[0];
    t[1] = p[1];
    t[2] = p[2];
  }
  func_8008C5D8(t, r);
  n1[0] = r[0] << 4;
  n1[1] = r[1] << 4;
  n1[2] = r[2] << 4;
  p2 = d;
  d[0] = c[0] - a[0];
  d[1] = c[1] - a[1];
  d[2] = c[2] - a[2];
  for (mask2 = 0x40000000, bit2 = 30; bit2 > 13; bit2--, mask2 >>= 1)
  {
    if (((abs(p2[0]) & mask2) || (abs(p2[1]) & mask2)) || (abs(p2[2]) & mask2))
    {
      break;
    }
  }

  shift2 = bit2 - 13;
  if (shift2 > 0)
  {
    t[0] = p2[0] >> shift2;
    t[1] = p2[1] >> shift2;
    t[2] = p2[2] >> shift2;
  }
  else
  {
    t[0] = p2[0];
    t[1] = p2[1];
    t[2] = p2[2];
  }
  func_8008C5D8(t, r);
  n2[0] = r[0] << 4;
  n2[1] = r[1] << 4;
  n2[2] = r[2] << 4;
  out[0] = ((n1[1] >> 8) * (n2[2] >> 8)) - ((n1[2] >> 8) * (n2[1] >> 8));
  out[1] = ((n1[2] >> 8) * (n2[0] >> 8)) - ((n1[0] >> 8) * (n2[2] >> 8));
  out[2] = ((n1[0] >> 8) * (n2[1] >> 8)) - ((n1[1] >> 8) * (n2[0] >> 8));
}

#ifdef NON_MATCHING
void func_8006E054(void) {
    VECTOR f;
    VECTOR r;
    VECTOR u;
    VECTOR apex;
    SVECTOR fwd;
    VECTOR t1;
    VECTOR t2;
    VECTOR p1;
    VECTOR p2;
    VECTOR p3;
    VECTOR p4;
    s32 h;
    s32 *pl;
    s32 fx;
    fwd.vx = 0;
    fwd.vy = 0;
    fwd.vz = 0x1000;
    fwd.pad = 0;
    apex.vx = g_Camera.posX << 16;
    apex.vy = g_Camera.posY << 16;
    apex.vz = g_Camera.posZ << 16;
    gte_ldv0(&fwd);
    gte_rtv0();
    gte_stlvnl(&t1);
    NORM(&t1, &t2, &p1);
    f.vx = p1.vx << 4;
    f.vy = p1.vy << 4;
    f.vz = p1.vz << 4;
    {
        s32 nx;
        s32 z = t1.vz;
        nx = -t1.vx;
        t1.vy = nx;
        t1.vx = z;
        t1.vz = nx;
        t1.vy = 0;
    }
    NORM(&t1, &t2, &p1);
    {
        s32 rz = p1.vz << 4;
        s32 rx = p1.vx << 4;
        s32 a, b, c, d;
        a = (rz >> 8) * (f.vy >> 8);
        b = (rz >> 8) * (f.vx >> 8);
        c = (rx >> 8) * (f.vz >> 8);
        d = (rx >> 8) * (f.vy >> 8);
        r.vz = rz;
        r.vx = rx;
        t1.vx = -a;
        r.vy = p1.vy << 4;
        t1.vy = b - c;
        t1.vz = d;
    }
    NORM(&t1, &t2, &p1);
    u.vx = p1.vx << 4;
    u.vy = p1.vy << 4;
    u.vz = p1.vz << 4;
    {
        s32 k = (s16)g_Args->displayW >> 1;
        r.vx = r.vx * k;
        r.vz = r.vz * k;
        k = (s16)g_Args->displayW * 3 / 4 >> 1;
        u.vx = u.vx * k;
        u.vy = u.vy * k;
        u.vz = u.vz * k;
    }
    gte_ReadGeomScreen(&h);
    {
        s32 cx = apex.vx + h * f.vx;
        s32 cy = apex.vy + h * f.vy;
        s32 cz = apex.vz + h * f.vz;
        t2.vx = cx;
        p1.vx = cx + u.vx - r.vx;
        t2.vy = cy;
        p1.vy = cy + u.vy - r.vy;
        p1.vz = cz + u.vz - r.vz;
        p2.vx = cx + u.vx + r.vx;
        p2.vy = cy + u.vy + r.vy;
        t2.vz = cz;
        p2.vz = cz + u.vz + r.vz;
        p3.vx = cx - u.vx - r.vx;
        p3.vy = cy - u.vy - r.vy;
        p3.vz = cz - u.vz - r.vz;
        p4.vx = cx - u.vx + r.vx;
        p4.vy = cy - u.vy + r.vy;
        p4.vz = cz - u.vz + r.vz;
    }
    pl = &D_800E2DF4;
    func_8006DD20((s32 *)&apex, (s32 *)&p1, (s32 *)&p3, pl);
    func_8006DD20((s32 *)&apex, (s32 *)&p4, (s32 *)&p2, pl + 4);
    func_8006DD20((s32 *)&apex, (s32 *)&p2, (s32 *)&p1, pl + 8);
    func_8006DD20((s32 *)&apex, (s32 *)&p3, (s32 *)&p4, pl + 12);
    fx = f.vx;
    pl[-4] = fx;
    pl -= 4;
    pl[0x14] = -fx;
    pl[1] = f.vy;
    pl[2] = f.vz;
    pl[0x15] = -f.vy;
    pl[0x16] = -f.vz;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006E054);
#endif

s32 func_8006E620(void *fr, V3s lo, V3s hi) {
    u16 v[4];
    s16 r[2];
    u32 z;
    u32 flag;
    s32 a, b, c;
    PTF(0);
    gte_ldv0(v);
    gte_rtps();
    PTF(4);
    gte_cfFLAG(flag);
    if (flag & 0x20000) goto ret0;
    gte_ldv0(v);
    gte_rtps();
    PTF(8);
    gte_stsxy(r);
    if (r[0] < 0) goto ret0;
    gte_ldv0(v);
    gte_rtps();
    PTF(12);
    gte_stsxy(r);
    if (r[0] >= 0x21D) goto ret0;
    gte_ldv0(v);
    gte_rtps();
    PTF(16);
    gte_stsxy(r);
    if (r[1] < 0) goto ret0;
    gte_ldv0(v);
    gte_rtps();
    PTF(20);
    gte_stsxy(r);
    if (r[1] >= 0xF1) goto ret0;
    gte_ldv0(v);
    gte_rtps();
    gte_stsz(&z);
    if (z > ((s32 *)fr)[0x18]) goto ret0;
    return 1;
ret0:
    return 0;
}

void func_8006E9E0(s32 arg0) {
    Stk6E stk[32];
    Fr6E fr;
    u32 s3;
    s32 s2;
    u8 *s1;
    s32 st;
    u8 *v;
    u8 **list;
    u8 *p;
    list = D_800E2448;
    s3 = 0;
    s2 = 0;
    fr.q[1] = D_800E2DE4.q[1];
    fr.q[2] = D_800E2DE4.q[2];
    fr.q[3] = D_800E2DE4.q[3];
    fr.q[4] = D_800E2DE4.q[4];
    fr.q[0] = D_800E2DE4.q[0];
    fr.q[5] = D_800E2DE4.q[5];
    stk[0].n = (u8 *)arg0;
    stk[0].st = 3;
    fr.w = D_800E2DE0;
    do {
        st = stk[s2].st;
        s1 = stk[s2].n;
        switch (st) {
        case 3:
            if (*(s32 *)(s1 + 0x1C) != 0) {
                if (s3 >= 0x258U) {
                    s3 = 0x258;
                    goto done;
                }
                list[s3] = s1;
                s3 += 1;
                s2 -= 1;
                break;
            }
        case 0:
            p = *(u8 **)(s1 + 0xC);
            if (p != NULL && func_8006E620(&fr, ((V3s *)p)[0], ((V3s *)p)[1]) != 0) {
                stk[s2].st = 1;
                s2 += 1;
                v = *(u8 **)(s1 + 0xC);
                stk[s2].st = 3;
                stk[s2].n = v;
                break;
            }
        case 1:
            p = *(u8 **)(s1 + 0x10);
            if (p != NULL && func_8006E620(&fr, ((V3s *)p)[0], ((V3s *)p)[1]) != 0) {
                stk[s2].st = 2;
                s2 += 1;
                v = *(u8 **)(s1 + 0x10);
                stk[s2].st = 3;
                stk[s2].n = v;
                break;
            }
        case 2:
            p = *(u8 **)(s1 + 0x14);
            if (p != NULL && func_8006E620(&fr, ((V3s *)p)[0], ((V3s *)p)[1]) != 0) {
                v = *(u8 **)(s1 + 0x14);
                stk[s2].st = 3;
                stk[s2].n = v;
                break;
            }
            s2 -= 1;
        }
    } while (s2 >= 0);
done:
    D_800E2438 = s3;
}

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

#ifdef NON_MATCHING
void func_8006F168(void *arg0, s32 arg1) {
    u16 sp[10];
    s32 count;
    u32 n;
    u32 i;
    s32 j;
    s32 v;
    s32 k;
    u8 *node;
    u8 *s0;
    u8 *it;
    u8 *q;
    u8 *a3;
    u8 *a0;
    u8 *vt;
    count = A(s32, 8);
    n = A(u32, 0x10);
    M(u8 *, 0x14) = (u8 *)arg0 + A(s32, 0xC);
    M(u16, 0) = A(u16, 0x18);
    M(u8 *, 0x18) = (u8 *)arg0 + A(s32, 0x1C);
    M(u8 *, 0x1C) = (u8 *)arg0 + A(s32, 0x20);
    v = A(s32, 4);
    M(u32, 4) = n;
    s0 = (u8 *)arg0 + A(s32, 0x14);
    M(u8 *, 0x20) = s0;
    M(s32, 0x24) = arg1;
    M(s32, 0x28) = arg1 + M(u16, 0) * 0xC;
    M(s32, 0x2C) = M(s32, 0x28) + v * 0x28;
    M(s32, 0x30) = M(s32, 0x2C) + v * 0x28;
    M(s32, 8) = v;
    M(s32, 0x34) = M(s32, 0x30) + n * 0x28;
    M(s32, 0xC) = A(s32, 0x24);
    D_800E2DE0 = A(s32, 0);
    if (A(s32, 0x24) != 0) {
        M(u8 *, 0x10) = (u8 *)arg0 + A(s32, 0x28);
    } else {
        M(u8 *, 0x10) = 0;
    }
    if ((u32)D_800D2FAC < (u32)(M(s32, 0x34) + n * 0x28 - D_800D2FC0)) {
        func_8006B230(0, (s32)D_800A9794);
    }
    for (j = 0; j < count; j++) {
        node = M(u8 *, 0x14) + (j << 5);
        for (k = 0xC; k <= 0x14; k += 4) {
            v = *(s32 *)(node + k);
            if (v >= 0) {
                *(s32 *)(node + k) = (s32)(M(u8 *, 0x14) + (v << 5));
            } else {
                *(s32 *)(node + k) = 0;
            }
        }
        *(s32 *)(node + 0x18) = (s32)s0 + *(s32 *)(node + 0x18);
    }
    it = s0;
    for (i = 0; i < n; i++) {
        *(B20 *)sp = *(B20 *)it;
        q = M(u8 *, 0x30) + sp[1] * 0x28;
        *(u16 *)(q + 0xC) = sp[5];
        *(u16 *)(q + 0x18) = sp[7];
        *(u16 *)(q + 0x24) = sp[9];
        *(u16 *)(q + 0x1A) = sp[8];
        *(u16 *)(q + 0xE) = sp[6];
        *(s16 *)(it + 0xA) = -1;
        a3 = M(u8 *, 0x34) + sp[1] * 0x28;
        for (k = 0; k < 3; k++) {
            a0 = M(u8 *, 0x1C) + sp[2 + k] * 4;
            vt = M(u8 *, 0x24) + sp[2 + k] * 0xC;
            *(s32 *)(vt + 4) = 0xFF0000;
            *(s32 *)(q + 4 + k * 0xC) = *(s32 *)a0;
            if (a0[3] != 0) {
                if (a0[0] < 0x64) {
                    *(s16 *)(vt + 6) = a0[0];
                }
                if (a0[1] < 0x64) {
                    *(s16 *)(it + 0xA) = a0[1];
                }
            }
        }
        q[3] = 9;
        q[7] = 0x34;
        if (sp[0] & 0x8000) {
            q[7] = 0x36;
        }
        *(s16 *)(it + 8) = sp[0] & 0x803F;
        *(W40 *)a3 = *(W40 *)q;
        *(u16 *)it = sp[1];
        *(u16 *)(it + 2) = sp[2];
        *(u16 *)(it + 4) = sp[3];
        *(u16 *)(it + 6) = sp[4];
        *(u16 *)(it + 0xC) = sp[5];
        *(u16 *)(it + 0xE) = sp[7];
        *(u16 *)(it + 0x10) = sp[9];
        it += 0x14;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006F168);
#endif

void func_8006F6AC(void) {
    D_800E2438 = 0;
    gte_SetRotMatrix(D_800B0018);
    gte_SetTransMatrix(D_800B0018);
    func_8006E054();
    gte_SetRotMatrix(D_800B0808);
    gte_SetTransMatrix(D_800B0808);
    func_8006E9E0(D_800E2DBC);
}

#ifdef NON_MATCHING
void func_8006F784(void) {
    s32 db;
    s32 *tile;
    s32 rows;
    s32 cols;
    s32 x0;
    s32 x;
    s32 x2;
    s32 y;
    s32 y2;
    s32 r;
    s32 c;
    s32 a;
    u32 *ot;
    u8 *prim;

    tile = ((SkyGrid *)D_800E2E44)->tiles;
    rows = ((SkyGrid *)D_800E2E44)->rows;
    cols = ((SkyGrid *)D_800E2E44)->cols;
    x0 = 0x100 - (((M2C_FIELD(&g_Camera, u16 *, 0x82) & 0xFFF) * D_800E2E48) >> 12);
    x = x0 + D_800E2E48;
    db = D_800B8F24;
    ot = D_800B8F88[db].tag;
    if (x < 0x200) {
        x0 = x;
    }
    a = M2C_FIELD(&g_Camera, u16 *, 0x80) & 0xFFF;
    if (a > 0x800) {
        a -= 0x1000;
    }
    y = (a * D_8009E9BC) >> 12;
    for (r = 0; r < rows; r++) {
        y2 = y + ((SkyGrid *)D_800E2E44)->h;
        if (y2 < 0) {
            tile += cols * 2;
        } else {
            if (y >= 0xF0) {
                break;
            }
            x = x0;
            for (c = 0; c < cols; c++) {
                x2 = x + ((SkyGrid *)D_800E2E44)->w;
                if (x2 >= 0) {
                    if (x >= 0x200) {
                        x -= D_800E2E48;
                        x2 = x + ((SkyGrid *)D_800E2E44)->w;
                        if (x2 < 0) {
                            goto next;
                        }
                    }
                    if (tile[0] == 0) {
                        prim = (u8 *)tile[1];
                        if (db != 0) {
                            prim += 0x34;
                        }
                        ((POLY_GT4 *)prim)->x0 = x;
                        ((POLY_GT4 *)prim)->y0 = y;
                        ((POLY_GT4 *)prim)->x1 = x2;
                        ((POLY_GT4 *)prim)->y1 = y;
                        ((POLY_GT4 *)prim)->x2 = x;
                        ((POLY_GT4 *)prim)->y2 = y2;
                        ((POLY_GT4 *)prim)->x3 = x2;
                        ((POLY_GT4 *)prim)->y3 = y2;
                    } else {
                        prim = (u8 *)tile[1];
                        if (db != 0) {
                            prim += 0x24;
                        }
                        ((POLY_G4 *)prim)->x0 = x;
                        ((POLY_G4 *)prim)->y0 = y;
                        ((POLY_G4 *)prim)->x1 = x2;
                        ((POLY_G4 *)prim)->y1 = y;
                        ((POLY_G4 *)prim)->x2 = x;
                        ((POLY_G4 *)prim)->y2 = y2;
                        ((POLY_G4 *)prim)->x3 = x2;
                        ((POLY_G4 *)prim)->y3 = y2;
                    }
                    ((PTag *)prim)->addr = ((PTag *)ot)->addr;
                    ((PTag *)ot)->addr = (u32)prim;
                }
            next:
                x = x2;
                tile += 2;
            }
        }
        y = y2;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_800617AC", func_8006F784);
#endif

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
