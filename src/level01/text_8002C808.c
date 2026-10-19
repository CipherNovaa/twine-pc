#include "twine.h"

extern s16 D_8009D3FC;
void func_80026C14(s16 arg0);
void func_8005CA20(void *obj);


s16 func_80053080(u8 *obj, s32 dist, s32 y);
u32 func_8005397C(VECTOR *a, VECTOR *b);
void func_8004EAB8(u8 *obj);


extern SVECTOR D_800B0B68;
extern SVECTOR D_800B0B70;
extern SVECTOR D_800D91E8;
extern SVECTOR D_800D91F0;


extern s32 D_8009D5F8;
void func_8003B940(u8 *obj, s16 x, s16 y, s16 z, s16 model, u8 count);


s32 func_800743E8(s32, s32 *, s32, s32, SVECTOR *, s32, s32);


extern s16 D_8009D10C;
char *func_8004C698(s32 arg0, s32 arg1);


extern s8 D_800D4384;
extern s8 D_800D4385;
extern s8 D_800D4386;
extern s8 D_800D4387;
extern s8 D_800D4388;
extern s8 D_800D4389;
extern s8 D_800D438A;
extern s8 D_800D438B;
extern s8 D_800D438C;
extern s8 D_800D438D;
extern s8 D_800D438E;
extern s8 D_800D438F;


extern char D_800A86CC[];


void func_8003CA4C(void);


extern u8 D_800D7DF8;
extern s16 D_800D45F4;
extern s32 D_8009CF4C;
extern u8 D_800D7A1C[];
void func_80048794(s16 arg0, s16 arg1);


extern char D_800A7D74[];
void func_80052BEC(void);
void *func_800390F8(s32 arg0);
void func_80060094(void *arg0);
void func_80063F34(void *arg0, s32 arg1);
void func_80058B10(s32 owner);
void func_800391B0(u8 *arg0, s32 arg1);


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
void func_8004C498(s32 idx, u8 *e);


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
void func_8003732C(u8 *arg0, u8 *arg1);
s16 func_80037768(u8 *obj, u8 *proj);
void func_80038C8C(u8 *obj);
void func_80062F38(u8 *, u8 *);
void func_800632AC(u8 *, s32);


extern s32 D_8009CF50;
extern s32 D_800D31BC;
s32 func_80050A10(VECTOR *arg0, VECTOR *arg1, u8 *arg2, void *arg3);
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
void *func_8003FCE4(s32, SpawnInfo *, s32, s32, s32, s32);
void func_80053790(u8 *arg0, s32 arg1);
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
s32 func_8002E798(void);
void func_8002F1BC(void);
void func_8002F1F4(void);
void func_8002FE04(void);
void func_80031F28(void);
void func_80031F54(s32 arg0);
void func_80032024(void);
void func_80032074(s32 arg0);
void func_800332A4(void);
void func_8003B4F4(void);
void func_8003FC60(void);
void func_800478C8(void);
void func_8004B324(void);
void func_8004BDA8(void);
void func_8004D048(void);
void func_8004F3D8(void);
void func_800530F4(void);
void func_80057C40(void);
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
s32 func_8004DFF8(s32 *pos);
s32 func_80040AA8(s32 *pos);
s32 func_800503D8(s16 arg0, s16 arg1, s16 arg2, s16 arg3);


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


void func_80050180(s16 *arg0, s16 *arg1);
void func_8004FD3C(s16 *A0, s16 *A1);
s32 func_8004F348(void);


void func_8005C10C(u8 *obj, u8 *model, s16 *f1, s16 *f2);
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
void func_8004B58C(s32 arg0);
void func_800463C8(void);
void func_8004514C(void);
void func_80028DBC(s32, s32);
void func_80028C08(s32, s32);
void DevMenu_Draw(void);
void DebugScreen_DrawPage(s32 arg0);
void func_80030234(void);


extern char D_800A89A4[];
s32 func_800705A8(s32, s16 *, s32, s32);


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
void *func_80030610(s32 arg0);
void func_8004C0C0(s32 arg0, void *arg1);
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
void func_8005885C(u8 *p, u8 type);




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
s32 func_8005B584(s32 a0, u16 v, s16 i);
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
void func_8006E9E0(s32);


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
void *func_80063CAC(void *, s16, s16, s16, s32);
void func_80063ED4(void *arg0);


extern s32 D_8009D368;
extern ListSet D_800D8C7C[];
extern u8 D_800D8E8C[][12];
extern u8 D_800D8E74[][12];
extern s32 D_800D8EA8[][8];
void func_8004D844(void *arg0, s32 i, s32 j);
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
void func_8005ADD4(void *arg0, s32 n, s32 *out);


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
extern SpawnEnt **D_8009CF70;
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
void func_8006F168(s32, s32);
void func_8006FA54(s32);
void func_8006F9A4(s32 arg0);
void func_80030FA8(s16 *arg0);
void func_8003C6E4(void);
void func_8003BFB8(void);
void func_80058D00(void);
void func_8005FF60(void);


extern u16 D_800AFFAC;
extern u8 *D_800B0AB4;
extern s16 D_800B0C84;


extern s32 D_800DDDCC[];
s32 SquareRoot0(s32 a);


extern s32 D_800D2D30[];
void func_8004BC70(s32 objs, s32 *model, s32 idx);


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
void func_800509E4(void *x);
void func_8005E020();


void func_8006ECBC(void);


extern char D_800A85D4[];
int strlen(char *s);


extern s32 D_1F8003D4;
extern s32 D_1F8003D8;
extern s32 D_1F8003DC;
extern M2C_UNK D_800D30C0;
extern s32 D_8009CF30;
void func_80052B2C(s32 *out, u8 *obj);
s32 func_80037B6C(u8 *obj);
void func_8004B270(s32 arg0, s32 arg1, s32 arg2);
void func_8005E228(void *arg0);
void func_8004009C(u8 *arg0);


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
void func_8004DA60(void);
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
s32 func_8004CB2C(void *obj, Weapon *w);
s32 func_8004C558(u8 *arg0, s32 arg1);
void *func_8004C5C8(s32 arg0, s32 arg1);


extern s16 D_8009CF80;


M2C_UNK *func_8004C768(void *arg0);
s32 func_8004C824(void);
void *func_8004C82C(void *arg0);
void *func_8004C944(void *obj);
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
void func_8005099C(void);
extern M2C_UNK D_800D90C0;
extern Rec48 D_800D8FF8[];


s32 func_80054098(s32 a);


extern char D_800A7344[];
void func_80032880(s32 arg0, GsIMAGE *arg1);
void func_800324D0(s32 timArg, s16 x, s16 y);


extern char D_800A7778[];
extern char D_800A7784[];
void func_80030DA0(u8 *data, s32 len, s32 mode);
void func_80030B04(char *arg0);
void func_80061700(void);
void *memchr(void *, int, int);


void func_8007D184(s32, s32, s32, s32);
extern u8 D_8009CABD;
extern u8 D_8009CABE;


extern s16 D_8009CF54;
s32 func_800349F8(void *a);


extern char D_800A828C[], D_800A82B4[], D_800A82DC[], D_800A8304[], D_800A8308[], D_800A8310[];
extern char D_800A8318[], D_800A8320[], D_800A832C[], D_800A8340[];


typedef s32 (*CmdHandler)(u8 *, void *, void *);
typedef struct { u8 pad0[0x124]; u32 mission : 4; u32 completed : 16; u32 unk20 : 4; u32 unk24 : 1; u32 unk25 : 1; u32 unk26 : 1; u32 pad27 : 5; } MissionChr;
typedef struct { s16 type; s16 pad2; VECTOR pos; u32 time; } Pickup;
typedef struct { s32 range; s32 dist; s32 flag; } PickRec;
typedef struct { u8 unk0; u8 type; u8 pad2[0x6E]; } ObjDef;
typedef struct { ObjDef r[1]; } ObjDefArr;
typedef struct { s32 a, b, c; } V3;
typedef struct { u8 r, g, b; } C3;
typedef struct { u16 x, y, z; } H3;
typedef struct { u8 p[0x20]; struct { u16 a, b; } e[1]; } HalfTbl20;
typedef struct { s16 x, y, z, p0; s32 p1; u8 *obj; s16 sx, sy; } EntA;
typedef struct { EntA arr[5]; u8 cnt; } EntList;
typedef struct { s16 id; s16 b; s32 c; s32 d; s32 e; s32 f; u32 expire; } Slot;
typedef struct { u8 pad0[4]; u8 active; u8 pad5[0x8B]; u8 *obj; s32 pad94; s16 d[6]; } EntHH;
typedef struct { u8 h[4]; u8 r0, g0, b0, p0; s16 x0, y0; u8 r1, g1, b1, p1; s16 x1, y1; u8 r2, g2, b2, p2; s16 x2, y2; u8 r3, g3, b3, p3; s16 x3, y3; } Quad;
typedef struct { u8 t[3]; u8 len; u8 r, g, b, code; s16 x0, y0, x1, y1, x2, y2, x3, y3; } PF4;
typedef struct { u32 f0; u8 p4[0xC]; u16 a10; u16 a12; u16 a14; u16 a16; u16 a18; u16 a1A; u16 a1C; u16 a1E; u8 p20[4]; u32 f24; } AnimTick;
typedef struct { u8 a; u8 p[0x22]; u8 b; } T23;
typedef struct { s32 w0; s16 h4, h6, h8, hA, hC; u8 b0E, b0F; u16 h10, h12; u8 pad[0x10]; } E24;
typedef struct { u8 b[8]; } PB;
typedef struct { u8 p0; u8 b1; u8 b2; u8 b3; u8 b4; u8 b5; s16 w6; s16 w8; s16 wA; } RumbleSt;
typedef struct { u8 b[32]; } Rec32;
typedef struct { char *key; s32 type; } CmdEnt;
typedef struct { char c[10]; } Buf10 __attribute__((aligned(4)));
typedef struct { u8 b[8]; } R8;
typedef struct { s32 a[3]; s32 x; s32 y; } E14;
typedef struct { s32 a[3]; s32 x; s32 b[2]; } E18;
typedef struct { s32 n; E14 *p; s32 m; E18 *q; } Tbl;
typedef struct { s32 pad[2]; u32 *e; s32 u1; s32 u2; } F14;
typedef struct { u8 pad[0x20]; s32 t[56]; s32 x[2]; } Q;
typedef struct { u8 f; u8 pad[3]; s32 *b; s32 *a; } E1;
typedef struct { u8 f; u8 pad[3]; s32 x; s32 *c; } E2;
typedef struct { s32 a, b; s32 *obj; s32 z; s32 flags; } LEnt;
typedef struct { void * f0; s8 f4; u8 f5; u8 f6; u8 f7; u8 f8; s8 f9; u8 fA; u8 padB[1]; u8 fC; u8 fD; u16 fE; s32 f10; s16 f14; u8 pad16[2]; s32 f18; s32 f1C; s32 f20; s32 f24; s32 f28; s32 f2C; s32 f30; s32 f34; u16 f38; u8 f3A; u8 pad3B[1]; u16 f3C; u16 f3E; u16 f40; u8 pad42[2]; s32 f44; s32 f48; s32 f4C; u16 f50; u16 f52; s16 f54; u16 f56; s32 f58; s32 f5C; s32 f60; s16 f64; s8 f66; s8 f67; s16 f68; s16 f6A; s32 f6C; s32 f70; s32 f74; s32 f78; s32 f7C; s32 f80; s32 f84; s32 f88; s32 f8C; u8 pad90[32]; s16 fB0; u8 padB2[26]; s8 fCC; s8 fCD; s8 fCE; u8 padCF[1]; s32 fD0; s32 fD4; s32 fD8; u8 padDC[4]; s32 fE0; s32 fE4; s32 fE8; u8 padEC[4]; s32 fF0; s32 fF4; s32 fF8; u8 padFC[4]; s32 f100; s32 f104; s32 f108; s32 f10C; u8 pad110[4]; s32 f114; } DstT;
typedef struct { u8 f0; u8 f1; u8 f2; u8 pad3[4]; u8 f7; u8 f8; u8 f9; u8 fA; u8 padB[5]; s32 f10; s32 f14; s32 f18; s32 f1C; s32 f20; s32 f24; u16 f28; u16 f2A; u8 f2C; u8 pad2D[3]; u16 f30; u16 f32; u16 f34; u8 pad36[2]; s32 f38; s32 f3C; s32 f40; u16 f44; u8 f46; u8 pad47[5]; u16 f4C; u16 f4E; u8 pad50[4]; s32 f54; s32 f58; u8 pad5C[20]; } SrcT;
typedef struct { s32 a, b, c, d; } Q4;
typedef struct { s16 m[3][3]; s32 t[3]; } MATRIX;
typedef struct { u8 active; u8 kind; u8 pad2; u8 flag3; u16 v[6]; s32 x; s32 y; s32 z; SpawnInfo *src; s16 h20; s16 h22; s16 h24; s16 pad26; s32 w28; s32 pad2C; u16 h30; u16 h32; s32 x2; s32 y2; s32 z2; s32 pad40; } Rec44;
typedef struct { u8 pad8; } X;
typedef struct { u8 p0[0xE]; u16 h0E; u8 p10[0x24]; s32 w34; u8 p38[0x20]; s32 w58; u8 p5C[8]; u32 w64; u32 w68; u8 p6C[0x24]; u8 *s90; u8 p94[0x18]; s16 hAC; } ObjHH;
typedef struct { u8 p0[0x38]; u16 h38; u8 p3A; u8 b3B; u8 p3C[0x70]; s16 hAC; u8 pAE[4]; u16 hB2; s32 wB4; } ArgHH;
typedef struct { s32 pad; s16 m[3][3]; } QM;
typedef struct { s16 x, y, z, pad; s32 *p8; s32 wC; u32 w10; } SObj;
typedef struct { u8 pad[0x28]; s32 x, y, z; } SubPos;
typedef struct { u8 pad0[0xA]; u8 unkA; u8 padB[0x3D]; s32 flags48; u8 pad4C[0x3C]; void *owner; u8 pad8C[4]; SubPos *sub; } AlarmObj;
typedef struct { u32 lo : 8; u32 analog : 1; u32 hi : 23; } Flags2;
typedef s32 MainFn(s32 arg0, Args *arg1);
typedef struct { u8 pad[4]; u8 active; u8 type; u8 pad2[0x11E]; } ObjEnt;
typedef struct { u8 pad0[4]; u8 active; u8 pad5; u8 id; u8 pad7[0x59]; s32 busy; u8 pad64[0x24]; s32 owner; u8 pad8C[0x98]; } Ent124;
typedef struct { s32 unk0; char name[0x64]; u8 unk68; u8 unk69; u8 pad[2]; } SaveSlot;
typedef struct { s32 m[8]; } GsMat32;
typedef struct { s16 v[4]; } V4S;
typedef struct { u8 b[8]; } PairC368;
typedef struct { void * p0; u8 b4; u8 pad5[1]; u8 b6; u8 pad7[2]; u8 b9; u8 padA[6]; s32 w10; s16 h14; u8 pad16[2]; s32 w18; u8 pad1C[16]; s32 w2C; s32 w30; u8 pad34[6]; u8 b3A; u8 pad3B[1]; PairC368 c3C; u8 pad44[24]; s32 w5C; s32 w60; s16 h64; u8 b66; u8 b67; s16 h68; s16 h6A; s32 w6C; s32 w70; s32 w74; u8 pad78[8]; s32 w80; s32 w84; s32 w88; s32 w8C; u8 pad90[32]; s16 hB0; u8 padB2[26]; u8 bCC; u8 bCD; u8 bCE; u8 padCF[1]; s32 wD0; s32 wD4; s32 wD8; u8 padDC[4]; s32 wE0; s32 wE4; s32 wE8; u8 padEC[4]; s32 wF0; s32 wF4; s32 wF8; u8 padFC[4]; s32 w100; s32 w104; s32 w108; s32 w10C; u8 pad110[4]; s32 w114; } ObjC368;
typedef struct { u8 on; u8 type; u8 p2[2]; u16 a[6]; s32 x; s32 y; s32 z; u8 p1C[6]; s16 ang; u8 p24[4]; void (*cb)(s32 *); u8 p2C[4]; s16 a30; u16 a32; u8 p34[16]; } Trig;
typedef struct { u8 b[8]; } B8;
extern s16 D_800DD838;
extern u8 D_800A7F80[];
extern u8 D_800A7FA0[];
extern u8 D_800A7FC8[];
extern u8 D_800D4264[];
extern u8 D_800D430C[];
s32 func_8003CABC(u8 *a, u8 *b);
R8 func_8003D76C(VECTOR *arg1, VECTOR *arg2, u8 *arg3, s32 arg4);
R8 func_8003E158(s32 arg1, s32 arg2, SVECTOR *P, SVECTOR *Q);
s32 func_8003EC44(s32 *arg0, s32 *arg1, s16 *arg2, s16 *arg3);
void func_80058F28(u8 *arg0);
extern u8 *D_800AB5D8;
extern s32 D_800A7C70;
extern s32 D_800A7C88;
extern s32 D_800A7C9C;
extern u8 D_800A7C60[];
void PushMatrix(void);
void PopMatrix(void);
void func_80076F24(u8 *obj, s32 idx);
void func_8007632C(u8 *, s16);
s32 func_80034998(void *arg0);
void func_80034A48(u8 *arg0, u8 *arg1, s16 arg2, s16 arg3);
void func_80036670(u8 *obj, u8 *tgt);
void func_800373FC(u8 *proj, u8 *obj);
void func_800633B0(u8 type, VECTOR v);
void func_8006060C(void);
void func_80063230(void);
s32 func_8003AA14(s32 *a, s32 *b);
extern s16 D_8009D8D6;
#define CORNER(k, a, b, c) in[0] = a; in[1] = b; in[2] = c; gte_SetRotMatrix(m); gte_ldv0(in); gte_rtv0(); gte_stsv(out); *(s16 *)(D_800DD83C + 0x184 + 8 * k) = bs[0] + out[0]; *(s16 *)(D_800DD83C + 0x186 + 8 * k) = bs[1] + out[1]; *(s16 *)(D_800DD83C + 0x188 + 8 * k) = bs[2] + out[2];
extern s32 D_8009CA98;
extern s32 D_800AFF48;
extern s32 D_800B0B58;
extern char D_800A7300[];
extern char D_800A7310[];
extern char D_800A7320[];
extern u8 D_800A7324[];
extern char D_800B086C[];
void func_800787FC(void);
void func_800788E0(void);
void func_80079E10(void);
void func_80036328(void);
int Args_GetFlag115(void);
void func_80041578(void);
void func_8004105C(void);
s32 func_80053BD0(void);
void func_8004133C(void);
void DevMenu_HandleInput(void);
void func_80061EC4(void);
void func_80053BE0(void);
void func_8004D978(void);
void func_80031678(void);
void func_8002E3A0(void);
void func_800361BC(void);
void func_8002E410(void);
void func_8004841C(void);
void func_8004D2D0(void);
void func_80036938(void);
void func_8002E4D4(void);
void func_8003FF9C(void);
void func_8002E430(void);
void func_8002F7CC(void);
void func_80035DE8(void);
void func_8002E48C(void);
void func_8002E4B4(void);
void func_8002D708(void);
void func_8002E4DC(void);
void func_8002E534(s32 arg0);
void func_80047B50(s32 buf);
s32 Pad_Update(void);
extern u8 D_800DDCCC[];
extern u8 *D_800DDDEC[];
extern s32 D_800DDE10[];
extern s16 *D_8009D918;
extern s32 *D_8009D91C;
extern u16 *D_8009D920;
extern s32 D_8009D924;
extern s32 D_8009D928;
void ApplyMatrix(void *, void *, void *);
void func_8004A398(void);
void func_8005C33C(void);
void func_80033F38(void);
void func_800AACFC(void *, void *, u8, u32 *);
#define gte_rtv0tr() __asm__ volatile("nop; nop; .word 0x4A080012")
extern u8 D_8009CF79;
extern s8 D_8009CF82;
extern s8 D_8009D8DC;
extern u8 D_800D4128[];
extern s8 D_800D424C;
extern s8 D_800D4260;
extern u8 D_800A8144[];
extern s32 D_800D4390;
extern s32 D_800D4080;
void TransposeMatrix(void *, void *);
s32 func_8003C9C4(u8 *a, u8 *b);
s32 func_8003F768(VECTOR *p, VECTOR *q, SVECTOR *min, SVECTOR *max);
extern char D_800A80B0[];
extern char D_800A80BC[];
extern char D_800A80C8[];
extern s32 D_8009D5F0;
extern s32 D_8009CD90;
extern u8 D_800A89BC[];
EffSlot *func_8003A898(s32 arg0, s32 arg1);
void func_80053650(s32 *arg0);
void func_80069C10(void *, void *, s32 *, s32 *);
s32 func_80076A0C(u8 *);
void func_8003BE68(s16 *a, s16 *b);
void func_80069C10(void *arg0, void *arg1, s32 *arg2, s32 *arg3);
s32 func_80076A0C(u8 *arg0);
extern u16 D_8009D3F8;
extern s32 D_8009D3F4;
extern s32 D_8009D3F0;
extern VECTOR D_8009D3BC;
void ApplyMatrixLV(void *, void *, void *);
extern s32 D_8009D0FC;
extern u8 D_800B0E8C[];
extern u8 D_800B0EA0[];
void SetDefDispEnv(void *, s32, s32, s32, s32);
void SetDefDrawEnv(void *, s32, s32, s32, s32);
void func_8007F530(void *);
void func_8007F5F0(void *);
extern M2C_UNK D_800A7A0C;
void func_80038448(void);
void func_800388E8(void);
void func_80038954(void);
extern M2C_UNK D_800A7C14;
extern u8 D_800A7C20;
extern M2C_UNK D_800A7C24;
extern M2C_UNK D_800A7C30;
extern M2C_UNK D_800A7C3C;
extern M2C_UNK D_800A7C50;
extern M2C_UNK D_800A7C58;
void func_800335B4(void);
extern s32 D_8009CA94;
extern char D_800A8F94[];
void func_8005D654(void);
s32 *func_80054268(u32 *list, u32 *cmd);
s32 *func_8005430C(u32 *list, u32 *cmd);
s32 *func_800543BC(u32 *list);
extern u8 D_800B0E62;
extern u8 D_800B0E63;
u8 func_8003B750(s32 *arg0);
void func_8003B808(u8 *arg0);
extern char D_800A7D90[];
u8 *func_80033420(s16 *arg0, s32 arg1, s32 arg2);
extern char D_800A75D0[];
void GsMapModelingData(u8 *);
extern s16 D_800B07BC;
extern s16 D_800B0DAC;
extern u16 D_800AFF94;
extern u16 D_800AFF98;
extern u16 D_800AFFB0;
extern u16 D_800AFFB4;
extern u16 D_800B0AC8;
extern u16 D_800B0AE4;
extern char D_800A73BC[];
extern char D_800A73D8[];
extern char D_800A7418[];
extern char D_800A7424[];
extern char D_800A7EFC[];
void func_8004C368(s32 idx, ObjC368 *o);
s32 func_800752E8(u8);
extern char D_800A773C[];
extern char D_800A76E4[];
extern char D_800A76F8[];
extern char D_800A7714[];
extern char D_800A7728[];
s32 func_80031F18(void);
s32 func_8007D2B4(RECT16 *, RECT16 *);
s32 func_8007DA88(s32, s32);
extern u8 D_800A8D34[];
extern GsMat32 D_800FC6A0;
extern u8 D_8009D8DD;
#define OUT(k) { u8 *q = p + (k) * 8; *(u16 *)(q + 0x184) = pos.vx + r.vx; *(u16 *)(q + 0x186) = pos.vy + r.vy; *(u16 *)(q + 0x188) = pos.vz + r.vz; }
#define XFORM() { gte_SetRotMatrix(&mat); gte_ldv0(&vv); gte_rtv0(); gte_stsv(&r); }
u8 *func_80050A70(s32 *arg0, s32 *arg1, u8 *arg2, s32 arg3);
extern u8 D_800A85F8[];
extern s32 D_800D4250[];
#define gte_ldDiag(r0) __asm__ volatile("lw $12, 0(%0); lw $13, 4(%0); ctc2 $12, $0; lw $14, 8(%0); ctc2 $13, $2; ctc2 $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldlvl_zxy(r0) __asm__ volatile("lwc2 $11, 8(%0); lwc2 $9, 0(%0); lwc2 $10, 4(%0)" : : "r"(r0))
#define gte_op0() __asm__ volatile("nop; nop; .word 0x4B70000C")
void func_80044898(void);
void func_800449BC(void);
void func_80044A60(void);
void func_80061AB0(s32 a, s32 b);
void Pad_StickToSpeed(s32 axis, s32 maxSpeed, s32 *out);
s32 func_8004F3E8(void);
extern POLY_FT4 D_800D7588[];
#define HUD_DIGIT(ent, yy) { u8 *e_ = (ent); s16 w_ = e_[2]; s16 y_ = (yy); M2C_FIELD(p, s16 *, 8) = x; M2C_FIELD(p, s16 *, 0xA) = y_; M2C_FIELD(p, s16 *, 0x12) = y_; M2C_FIELD(p, s16 *, 0x18) = x; M2C_FIELD(p, s16 *, 0x1A) = y_ + 8; M2C_FIELD(p, s16 *, 0x22) = y_ + 8; M2C_FIELD(p, s16 *, 0x10) = x + w_; M2C_FIELD(p, s16 *, 0x20) = x + w_; M2C_FIELD(p, u8 *, 0xC) = e_[0]; M2C_FIELD(p, u8 *, 0xD) = e_[1]; M2C_FIELD(p, s8 *, 0x14) = e_[0] + w_; M2C_FIELD(p, u8 *, 0x15) = e_[1]; M2C_FIELD(p, u8 *, 0x1C) = e_[0]; M2C_FIELD(p, s8 *, 0x1D) = e_[1] + 8; M2C_FIELD(p, s8 *, 0x24) = e_[0] + w_; M2C_FIELD(p, s8 *, 0x25) = e_[1] + 8; x += w_ + *(s16 *) &D_800AEDD4; ((PTag *) p)->addr = ((PTag *) ((u8 *) arg0 + 4))->addr; ((PTag *) ((u8 *) arg0 + 4))->addr = (u32) p; p += 0x28; }
extern u8 D_800AB4A0[];
extern u8 D_800DB7E4[];
extern u8 D_800DCAA4[];
extern s16 D_8009D8BE;
extern s32 D_8009D8C4;
extern s32 D_8009D8C8;
extern s32 D_8009D8CC;
void func_8004E37C(void);
void func_8004E5D8(void);
void func_8004EF44(void);
s32 func_8005038C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
AnimEnt *func_8003A71C(u8 **obj);
s32 func_8005B5AC(u8 *obj, s32 *p);
extern s8 D_800A7E34;
extern s8 D_800A7E50;
extern s32 D_800A7E74;
extern s8 D_800A7E90;
extern s32 D_800A7EC4;
extern u16 D_800AFF4C;
extern s16 D_800AFF70;
extern u16 D_800AFF84;
extern s16 D_800B07AC;
extern u16 D_800B0804;
extern s16 D_800B0BB4;
extern s16 D_800B0BB8;
extern s16 D_800B0BBC;
extern s16 D_800B0DB8;
extern s16 D_800B0DBC;
extern s16 D_800B0DC4;
extern s16 D_800B0DC8;
extern s16 D_800B0DCC;
u8 *func_8003A68C(void);
s32 func_80039A34(u8 *arg0);
s32 func_80051728(u8 *obj, u8 *tgt);
s32 func_80053A58(void *arg0);
void func_80052630(u8 *obj, s32 dir);
void func_80052808(u8 *obj, s16 x, s16 y, s16 z);
void func_80053804(u8 *arg0);
void func_8005772C(void *arg0);
extern s8 D_800A8944;
extern u8 D_800D91E7;
void func_80053114(u8 *obj);
#define LOADV(ix, w) { SVECTOR *p = (SVECTOR *) (verts + (ix) * 8); u16 x = p->vx; u16 y; u16 z; sv->vx = x; y = p->vy; sv->vy = y; z = p->vz; sv->vz = z; w[0] = (s16) x; w[1] = (s16) y; w[2] = (s16) z; if (scale != 0) { w[0] = ((s16) x * scale) >> 12; w[1] = ((s16) y * scale) >> 12; w[2] = ((s16) z * scale) >> 12; } }
extern char D_800A7FF8[];
extern char D_800A8000[];
s32 func_80040778(s32 *arg0, s32 *arg1, s32 *arg2, s32 *arg3);
void func_80044B04(void *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
extern POLY_FT4 D_800D7C64[];
extern char D_800A7DF4[];
extern char D_800A7E14[];
extern s16 D_800AFF58;
extern s16 D_800B05AC;
extern s16 D_800B0B78;
extern s16 D_800B0B80;
void func_8005EADC(s32 *arg0);
void func_8005EB6C(s32 arg0, s32 arg1);
#define W(p, T, o) (*(T *)((u8 *)(p) + (o)))
extern s32 D_8009CF8C;
extern s32 D_8009CF90;
extern s32 D_8009CF94;
extern s32 D_8009CF98;
extern s32 D_8009CF9C;
extern char D_800A81F8[];
extern char D_800A8210[];
extern char D_800A8224[];
extern char D_800A8238[];
extern char D_800A824C[];
extern char D_800A8260[];
extern char D_800A8274[];
extern char D_800A8280[];
void func_800350F0(u8 *obj);
s32 func_80035964(void *arg0);
s32 func_800359EC(u8 *obj);
extern u16 D_800D4050[];
extern u16 D_800D4058[];
extern u8 D_800A7CF0[];
extern u8 D_800A7D08[];
extern u8 D_800A7D20[];
s32 func_8003CC8C(u8 *arg0, u8 *arg1);
#define W(p, T, o) (*(T *)((u8 *)(p) + (o)))
extern s32 D_8009CF38;
extern s32 D_8009CF3C;
extern s32 D_8009CF40;
#define W(p, T, o) (*(T *)((u8 *)(p) + (o)))
void func_8004F148(s16 arg0, s16 arg1);
extern s32 D_8009C180;
extern s16 D_8009C184;
void func_8002C83C(void);
void func_800306A8(void);
void func_800306F0(void);
void func_80084134(void);
void func_8002F0A4(void);
void func_8002C6BC(s32 arg0);
void CdBin_SetLevel(s32 arg0, s32 arg1);
void func_8002FFD8(void);
void func_80030324(void);
void func_800368A8(void);
void func_8004B194(void);
void DevMenu_Init(void);
void func_80030EB0(void);
void func_80030868(void);
void func_8003052C(void);
void func_80037ACC(void);
void func_8002E5C4(void);
void func_80030334(void);
void ApplyBonusCheats(void);
void func_8002C844(void);
void func_8002E318(void);
void func_8002D0F0(void);
void func_8002F114(void);
void func_800841F8(void);
MainFn main_ __asm("main");
extern u8 D_800D7DFC[];
extern u8 D_800D7E9C[];
extern u8 D_800D7EEC[];
extern u8 D_800D7FDC[];
void func_8004A294(void);
#define INIT(p, st) { (p)[3] = 9; (p)[7] = 0x2C; (p)[4] = 0x80; (p)[5] = 0x80; (p)[6] = 0x80; func_80081CE4(p, st); }
#define S16(p, o, v) *(s16 *)((p) + (o)) = (v)
#define QUAD(p, x0, y0, x1, y1) S16(p, 8, x0); S16(p, 0xA, y0); S16(p, 0x10, x1); S16(p, 0x12, y0); S16(p, 0x18, x0); S16(p, 0x1A, y1); S16(p, 0x20, x1); S16(p, 0x22, y1)
#define QUADY(p, x0, y0, x1, y1) S16(p, 8, x0); S16(p, 0x10, x1); S16(p, 0xA, y0); S16(p, 0x18, x0); S16(p, 0x12, y0); S16(p, 0x20, x1); S16(p, 0x1A, y1); S16(p, 0x22, y1)
extern s32 D_8009CFE4;
extern s32 D_8009CFE8;
#define AF(T, o) (*(T *)((u8 *)g_Args + (o)))
extern u8 D_800AE9C0[];
#define DIGIT(p, d) { g = &D_800AE9C0[(d) * 8]; (p)[0xC] = g[0]; (p)[0xD] = g[1]; (p)[0x14] = g[0] + g[2]; (p)[0x15] = g[1]; (p)[0x1C] = g[0]; (p)[0x1D] = g[1] + 8; (p)[0x24] = g[0] + g[2]; (p)[0x25] = g[1] + 8; *(u16 *)((p) + 0x12) = *(u16 *)((p) + 0xA); *(u16 *)((p) + 0x18) = *(u16 *)((p) + 8); *(s16 *)((p) + 0x1A) = *(u16 *)((p) + 0xA) + 8; *(s16 *)((p) + 0x10) = *(u16 *)((p) + 8) + g[2]; *(s16 *)((p) + 0x22) = *(u16 *)((p) + 0xA) + 8; *(s16 *)((p) + 0x20) = *(u16 *)((p) + 8) + g[2]; }
#define SP0 (*(s32 *)0x1F8003E8)
#define SP1 (*(s32 *)0x1F800004)
#define SP2 (*(s32 *)0x1F800008)
#define CW(o) (*(s32 *)((u8 *)&g_Camera + (o)))
#define CH(o) (*(u16 *)((u8 *)&g_Camera + (o)))
extern s32 D_8009CD00;
extern u8 D_800B0E5F;
extern u8 D_800B0E5E;
extern s32 D_800DDC6C;
extern s32 D_800DDDBC;
extern u8 D_8009D900;
extern s32 D_8009D90C;
void func_8005DA90(void);
extern u32 D_8009D904;
void func_80043200(void);
extern POLY_FT4 D_800D46D8[][8];
extern POLY_FT4 D_800D7930[];
extern POLY_FT4 D_800D7AB8[];
extern s16 D_8009D028[];
#define C(o) (*(u16 *)((u8 *)&g_Camera + (o)))
#define V(o) (*(s32 *)((u8 *)&g_Camera + (o)))
#define K0 (*(s16 *)&g_Camera)
#define K1 (*(s16 *)((u8 *)&g_Camera + 2))
s32 func_80069C00(void);
void func_8003872C(void *arg0, void *arg1, void *arg2);
void func_80038404(void *a0, void *a1);
#define C(o) (*(u16 *)((u8 *)&g_Camera + (o)))
#define V(o) (*(s32 *)((u8 *)&g_Camera + (o)))
#define Q(k) (P[2 * (k)])
extern s32 D_800D911C;
extern s32 D_8009D36C;
extern s32 D_8009E908[];
extern s32 D_8009E9A8[];
extern char D_800A87F8[];
extern char D_800A8808[];
extern s32 D_800D8C90;
s32 PCcreat(char *, s32);
void func_8004CCD8(u8 *a, u8 *b);
void func_8004D214(void);
void func_80083F14(s32);
void func_80084E0C(s32, char *, s32);
void func_8006B198(void);
void func_800689F0(void);
s32 func_800365B4(s32 a);
void func_80040978(s32 *v);
extern char D_800A8884[];
extern u16 D_800AC4E8;
extern u8 D_800D7D08[];
void InitHeap3(s32, s32, s32);
extern char D_800A74D4[];
void func_8005DF40(void *arg0);
void func_8005DC40(AlarmObj *obj);
void func_8005DB7C(u8 *arg0, u8 *arg1);
extern u8 D_8009D600[];
extern u8 D_8009D610[];
extern u8 D_8009D620[];
extern u8 *D_8009D5FC;
u8 func_800523E4(u8 *obj, u8 n);
extern s8 D_800A811C;
void func_8003EB00(u8 *arg0);
extern s8 D_800A8764;
extern s32 D_800A85C4;
s32 func_80048DE4(char *name);
extern s32 D_800D8EE8[][32];
extern s32 D_800D8FE8[][2];
extern s32 D_800A8794;
s32 func_8004C50C(void *arg0, s32 arg1);
s32 func_8005EC84(void *a, u32 *b);
extern struct { u8 pad[0x1C]; u16 v; u8 pad2[2]; } D_800D2E84_r[] asm("D_800D2E84");
void func_80033348(void);
void func_8004CD14(u8 *arg0, SVECTOR *dst);
void func_8004CDA0(s32 *arg0, u8 *arg1);
extern s32 *D_800D8EA4;
extern s32 (*D_8009D7A4)(s32);
void free3(void *);
void func_80030670(s32 *p);
void func_8005CAF8(s16 arg0, s32 arg1);
void *memset(void *, int, int);
s32 func_8003E494(void);
R8 func_8003E5A0(VECTOR *o, VECTOR *e, VECTOR *p0, VECTOR *p1, VECTOR *p2);
s32 func_8002D4E4(s32 arg0);
void Cheat_AllWeapons(void);
void Cheat_RefillAmmo(void);
void func_800314A0(void);
void func_80032150(void);
void func_800321F0(s32 arg0);
void func_80032348(s32 arg0);
void func_800406E0(s32 *arg0);
void func_8005B86C(void);
void func_80059634(u8 *arg0);
extern s32 D_8009CDC4;
extern char D_800A7370[];
extern s32 D_800B0A98;
extern CmdEnt D_8009CDF0[6];
extern s32 D_8009CE20;
extern s32 D_8009CE24;
extern Buf10 D_800A7750;
extern char D_800A775C[];
extern char D_800D2E90[];
char *strstr(char *, char *);
char *strtok(char *, char *);
int strcmp(char *, char *);
long strtol(char *, char **, int);
char *strcpy(char *dst, char *src);
extern s32 D_800D74FC;
extern char D_800A7474[];
extern char D_800A747C[];
extern char D_800A7498[];
extern char *g_DebugPageNames[];
void func_80032E4C(s32 arg0, s32 arg1);
void DebugPage_Framerate(void);
void DebugPage_FPWeapon(void);
void DebugPage_Camera(void);
void DebugPage_Loadpoint(void);
void DebugPage_ObjectCount(void);
void DebugPage_Profile(void);
void DebugPage_ArtProfile(void);
void DebugPage_ContourMask(void);
extern s16 D_800B0DB4;
s32 func_8003AC1C(u8 *o, s32 arg1);
s32 func_8003C8A4(u8 *a, u8 *b);
s32 func_8004CA5C(void *arg0, void *arg1);
s32 func_80053A2C(void *o);
s32 func_8005F93C(void *);
s32 func_800608E4(u8 *);
void func_8005EDF8(void *);
void func_8005EDF8(void *arg0);
s32 func_800608E4(u8 *obj);
extern s32 D_8009CF2C;
extern s32 D_8009CF74;
extern s32 D_8009D8E4;
u8 *func_80032EFC(SObj *arg0, s32 arg1);
extern M2C_UNK g_CamTargetX;
void func_8005DF7C(u8 *obj);
#define PRAW (*(PB *)&D_800B0DE4)
#define PAX (*(PB *)&g_PadAxes)
#define PAXP (*(PB *)&g_PadAxesPrev)
#define RUM (*(RumbleSt *)&D_800B0E34)
extern u8 D_800B0E36;
extern u8 D_8009CDDC[];
s32 func_800865BC(s32);
s32 func_80086688(s32, s32, s32);
s32 func_80086854(s32, u8 *);
u8 func_8008688C(s32, u8, s32);
void func_800868D4(s32, u8 *, s32);
extern s32 D_800D86E4;
extern u8 D_800D86DC;
void func_80066B14();
void func_80067F18();
extern u16 D_8009D374[];
extern u16 D_8009D384[];
extern u16 D_8009D394[];
extern u8 D_800D4484[];
extern u8 D_800D4564[];
void func_800380DC(void *arg0, s32 arg1);
void func_8003B53C(u8 *obj, s32 yaw, s32 pitch, s32 speed);
extern u16 D_800B0068;
extern u8 D_800AE940[];
extern E24 D_800D8154[];
extern s16 D_800D5ED0;
extern s16 D_800D7550;
extern s8 D_800D861C;
extern s8 D_800D8620;
void func_80048FE8(void);
void func_80041DBC(void);
s16 func_8006B274(int arg0);
void func_8005CB38(s32 arg0);
void func_8005C8C4(s32 arg0, AnimTick *e);
extern POLY_G4 D_800D7A20[2][2];
s32 func_8003DE58(VECTOR *p1, VECTOR *p2, VECTOR *p3, VECTOR *p4);
extern u8 D_800D77B8[];
void func_80047ECC(s32 arg0, s32 arg1);
void func_80049B7C(void);
extern s32 D_800DF0F8;
void func_80060AC0(u8 *obj);
s32 func_800377F4(u8 *arg0, u8 *arg1);
extern s32 D_8009CAAC;
extern u8 D_800A735C[];
void func_8002E0E8(void);
extern char D_800A89F0[];
extern s32 D_8009D3B0;
void func_8004E96C(void);
s32 func_8004F880(void);
#define gte_mvmva_v0() __asm__ volatile("nop; nop; .word 0x4A480012")
extern char D_800A8D44[];
extern char D_800A8D58[];
extern u32 D_800F3B48;
extern u8 D_800F2FBC[];
extern s32 D_800F1DCC;
extern u8 D_800F3848[];
extern M2C_UNK D_800DDF24;
extern u8 D_8009D8E8;
void func_8007EC78(void *, void *, u8);
void func_80057B94(void *arg0, s16 arg1, s16 arg2, s16 arg3);
extern u16 D_800AFF8C;
AISlot *func_800540CC(void);
extern s32 D_8009D3AC;
s32 func_8003982C(u8 *obj);
extern char D_800A7510[];
void *calloc3(s32 size, s32 n);
extern C3 D_800AFF64;
extern H3 D_800AFF68;
extern s32 D_8009CF14[];
void func_800381CC(void *objArg, s32 arg1);
extern s32 D_8009CF34;
extern u8 D_8009D09C[];
extern s32 D_800A86C0[];
void func_8004B3BC(V3 *arg0, void *arg1, s32 arg2, s32 arg3);
void func_80057B2C(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
extern s32 D_800B0084;
extern s32 D_800B0D50;
void func_80036E10(u8 *a, u8 *b);
s32 func_8005ADB0(void);
extern s32 D_8009CD88;
extern s32 *D_8009CD8C;
M2C_UNK *func_800698B8(void);
void func_8005BAE8(u8 *obj, u8 *anim);
extern u8 D_800DDE34[];
void func_8005CFC8(s16 *q, s16 *rx, s16 *ry, s16 *rz);
void func_8005BEF0(u8 *obj, u8 *anim);
void func_8005B76C(u8 *obj);
void func_8005C214(s16 *q, QM *m);
void func_8005D2B0(s16 *a, s16 *b, s16 t, s16 *out);
extern s32 D_8009D8F4;
extern s32 D_8009D8F8;
extern s32 D_8009D8FC;
void func_8005B6CC(u8 *obj);
extern char D_800A8D14[];
void func_80053DE4(s16 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4);
void *func_8004BE84(void *arg0, s32 arg1);
void *func_8004BF70(void *arg0, s32 arg1);
void func_8004EB20(void);
extern char D_800A8A74[];
void func_8002AFF8(s32 idx);
extern u8 D_800DCB4C[];
extern u8 D_800AB540_b[];
extern u8 D_800AB540_c[];
void func_80061F38(void);
void func_80061FB4(void);
void func_8008C5A4(void *);
extern char D_800A8C28[];
void func_8004EA68(void *arg0);
extern char D_800A8BAC[];
extern char D_800A8B80[];
extern char D_800A8AA8[];
void func_8005469C(u8 arg0);
void func_8004A304(void);
void func_8004A338(void);
void func_8002C664(void);
extern PickRec D_8009D660[];
extern s32 D_800A8A08;
void func_800397A8(void);
void func_8004A2E0(void);
Pickup *func_80053ED8(u8 *obj);
void func_80057750(u32 *arg0, void *arg1);
s32 func_80057648(void *arg0, void *arg1);
s32 func_8005416C(u32 *a, u32 *b);
extern s32 g_CamPosY;
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


s32 func_80057E80(s32 arg0);
void func_80062300(void);
M2C_UNK func_80063B20();                            /* extern */
extern s32 D_800B8F24;
extern GsOT D_800B8F88[];


M2C_UNK func_8007F2EC(M2C_UNK *, M2C_UNK *);        /* extern */
extern M2C_UNK D_800B0C7C;
extern M2C_UNK D_800DE8E8;


M2C_UNK func_80029684();                            /* extern */
void func_800456D0(void);
void func_80047A04(void);
M2C_UNK func_80047DBC();
void func_800481B0(void);
void func_80048E60(void);
void func_80049220(u8 *arg0);
void func_800493C0(void);
s32 func_80049D60(void);
void func_8004A8E4(void);
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
void func_80038984(u8 *arg0, ObjHH *arg1, ArgHH *arg2);
extern u32 D_8009D910;
extern s32 D_8009DB38;
M2C_UNK func_80048B4C(M2C_UNK *, M2C_UNK, M2C_UNK); /* extern */
extern s16 D_8009D104;
extern s16 D_8009D106;
extern s16 D_8009D126;
extern M2C_UNK D_800A87A0;
extern M2C_UNK D_800A87B0;
extern M2C_UNK D_800A87C0;
void func_80030994(u8 *arg0);
void func_80030D08(u8 arg0, u8 *buf, u8 *name);
void func_80030F20(void);
extern s8 D_800A9604;
extern s8 D_800A9614;


M2C_UNK func_80060798();                            /* extern */


void func_8003331C(s32 arg0);
s32 func_8005D58C(s32 arg0);
s32 func_8005D5C4(M2C_UNK *arg0, void *arg1);
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
extern T23 D_800D4958;
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


void func_8005D630(s32 *p);
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


M2C_UNK func_80032768(s32 arg0);
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
void func_80042B28(void);
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
void func_80033090(void);
s32 func_800352B8(void);
void func_80035F04(void);
void func_80037A24(void);
void func_8004BB00(void);
void func_80052DB8(void);
M2C_UNK func_8005EF7C();                            /* extern */
M2C_UNK func_8005F3B0();                            /* extern */
M2C_UNK func_8005F788();                            /* extern */
void func_8002D1D4(void);
void func_80031D94(void);
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
void func_8005D694(void);
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
s32 func_80059E4C(s32 *arg0);
extern s32 D_8009CF7C;
extern M2C_UNK D_800DDA40;
extern M2C_UNK D_800A81D0;
M2C_UNK Menu_ResetState();                            /* extern */
void func_8004783C(void);
extern s32 D_8009CFC4;
extern s32 D_8009CDC8;
s32 func_80026F30(M2C_UNK, M2C_UNK);
void func_8004218C(void);
s16 func_80043B14(s16 arg0, s16 arg1);
extern s16 D_8009CFBE;
void func_800474AC(void);
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
s32 func_80054414(u8 level, u8 arg1, u8 arg2);
s32 func_80039A9C(s32 arg0, u8 arg1, s32 arg2);
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

#ifdef NON_MATCHING
void func_8002C844(void) {
    s32 t;
    g_FrameCap = 20;
    D_800B8F24 = GsGetActiveBuff() ^ 1;
    func_800787FC();
    func_80026B00(g_Args->unk142);
    D_8009CDE7 = 0;
    func_80032724(0, 0, (s16)g_Args->displayW, (s16)g_Args->displayH * 2);
    func_8002E4DC();
    printf(D_800A7300, *(s32 *)((u8 *)g_Args + 0x1F0) - *(s32 *)((u8 *)g_Args + 0x1F4));
    if (g_Args->cdArchive == 0) {
        sprintf(D_800B086C, D_800A7310, g_Args->level);
        D_800AFF48 = PCcreat(D_800B086C, 0);
        if (D_800AFF48 != -1) {
            t = *(s32 *)((u8 *)g_Args + 0x1F0) - *(s32 *)((u8 *)g_Args + 0x1F4);
            D_800B0B58 = t;
            sprintf(D_800B086C, D_800A7320, t);
            func_80084E0C(D_800AFF48, D_800B086C, strlen(D_800B086C));
            func_80083F14(D_800AFF48);
        }
    }
    g_Args->flags2 &= ~8;
    for (;;) {
        if (D_8009CABF == 0) {
            g_PadPrev = g_PadHeld;
            g_PadHeld = Pad_Update();
            if (D_8009CAC4 != 0) {
                if (g_PadHeld != 0) {
                    g_PadPrev = 0;
                    g_PadHeld = 0;
                } else {
                    D_8009CAC4 = 0;
                }
            }
            t = g_PadHeld ^ g_PadPrev;
            g_PadPressed = t;
            D_800B0E40 = t & g_PadPrev;
            g_PadPressed = t & g_PadHeld;
        }
        if (D_8009CAA4 != 0) {
            func_80041578();
            GsGetLs(D_800D3238, D_800B0808);
            func_800788E0();
            func_8004841C();
            func_8002E430();
            func_8002F7CC();
            if (D_8009D018 != 0 && D_8009D01A != 0) func_80047B50(D_800B8F24);
            if (D_800F11F0 == 0x1F) func_8007F0D8(0);
            continue;
        }
        if ((g_Args->flags >> 6) & 1) return;
        if (g_DevMenu[0] == 0 && (u32)D_8009CDA8 >= 0x51 && ((s32 *)g_Player)[0x34 / 4] > 0) func_8004105C();
        if ((g_PadHeld & 0x900) == 0x900) {
            if (g_CutScene == 1 && !((D_800D91E4 >> 25) & 1)) {
                *(u16 *)((u8 *)g_Args + 0x144) <<= 2;
                *(u16 *)((u8 *)g_Args + 0x13A) <<= 1;
            }
            g_Args->unk115 = 1;
            g_Args->flags |= 0x80;
        }
        if ((((u8 *)g_Player)[0xA4] & 2) && (Args_GetFlag115() << 16) == 0) func_8002D4E4((s32)D_800A7324);
        if (D_8009CA90 != 0) func_8002D708();
        if (*(u8 *)g_Args >> 7) return;
        if (D_8009CA90 == 0 && (g_CutScene == 0 || g_CutScene == 0xF) && (g_PadPressed & 0x100) && D_8009CFC4 == 1 && (func_8004B06C() << 16) == 0 && (func_8004B084() << 16) == 0 && g_Args->unk115 == 0 && func_80053BD0() != 0) func_8004133C();
        if (g_DevMenu[0] != 0) {
            DevMenu_HandleInput();
            if (g_DevMenu[0x3C / 4] == 2) GsGetLs(D_800D3238, D_800B0808);
        } else if (g_DevMenu[0x3C / 4] != 0) {
            func_8002E534(g_DevMenu[0x3C / 4]);
            func_80061EC4();
        } else {
            func_80079E10();
        }
        func_80053BE0();
        if (g_DevMenu[0x3C / 4] != 2) func_8004D978();
        func_8004900C();
        gte_SetRotMatrix(D_800B0808);
        gte_SetTransMatrix(D_800B0808);
        if (g_DevMenu[0] == 0) {
            if (D_8009CD98 != 0) func_800788E0();
            if (D_8009CAA4 != 0) continue;
            func_80031678();
            if (D_8009CA98 != 0) func_8003BFB8();
            func_8002E3A0();
            func_800361BC();
            func_8002E410();
            func_80036328();
            func_8004841C();
            func_8004D2D0();
            func_80036938();
            func_8002E4D4();
            func_8003FF9C();
        }
        func_8002E430();
        if (D_800B0A98 != 0) {
            D_800AFF68.x = ((u16 *)&g_Camera)[0x68 / 2];
            D_800AFF68.y = ((u16 *)&g_Camera)[0x6C / 2];
            D_800AFF68.z = ((u16 *)&g_Camera)[0x70 / 2];
        }
        func_8002F7CC();
        func_80035DE8();
        func_8002E48C();
        D_8009CDA8 += 1;
        func_8002E4B4();
        if ((g_Args->flags >> 3) & 1) func_8002B69C();
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002C844);
#endif

s32 main_(s32 arg0, Args *arg1) {
    s32 temp_v0;

    func_8002C83C();
    temp_v0 = func_80026800();
    g_Args = arg1;
    ((u8 *)arg1)[0x12B] = 0;
    func_800306A8();
    func_800855BC(0x1F8003D0);
    func_800306F0();
    func_80084134();
    func_8002F0A4();
    g_Args->flags2 |= 0x40;
    func_8002C6BC(g_Args->unk18A);
    CdBin_SetLevel(g_Args->level, 0);
    func_8002FFD8();
    func_80030324();
    func_800368A8();
    func_8004B194();
    DevMenu_Init();
    func_80030EB0();
    func_800303B4(0);
    func_80030868();
    func_800303B4(1);
    func_8003052C();
    func_80037ACC();
    func_8002E5C4();
    D_8009C184 = 1;
    if (*(u8 *)&D_8009CDE7 != 0) {
        func_80030334();
    }
    ApplyBonusCheats();
    func_8002C844();
    M2C_FIELD(g_Args, s32 *, 0x2D4) = M2C_FIELD(g_Player, s32 *, 0x34);
    M2C_FIELD(g_Args, s32 *, 0x2D8) = D_8009C180;
    ((Flags2 *)&g_Args->flags2)->analog = (u8)D_800B0E54;
    SsUtAllKeyOff(0);
    if (((u32) g_Args->flags >> 6) & 1) {
        func_8002E318();
    } else {
        func_8002D0F0();
    }
    func_8002F114();
    func_800841F8();
    func_800855BC(temp_v0);
    return 0;
}

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

void func_8002D1D4(void) {
    s32 rs;
    s32 ms;
    s32 pad;
    s32 sc;
    s32 *p;
    Camera *cam;
    s32 *tbl;

    rs = 0x14;
    ms = 0x3C;
    if (g_PadHeld & 0x10) {
        rs = 0x50;
        ms = 0xF0;
    }
    if (g_PadHeld & 0x80) {
        g_Camera.rotX -= rs;
    }
    if (g_PadHeld & 0x20) {
        g_Camera.rotX += rs;
    }
    if (g_PadHeld & 8) {
        g_Camera.targetY -= ms;
    }
    if (g_PadHeld & 4) {
        g_Camera.targetY += ms;
    }
    if (g_PadHeld & 0x8000) {
        g_Camera.rotY -= rs;
    }
    if (g_PadHeld & 0x2000) {
        g_Camera.rotY += rs;
    }
    tbl = D_800A0370;
    cam = &g_Camera;
    sc = tbl[cam->rotY & 0xFFF];
    pad = g_PadHeld;
    if (pad & 0x1000) {
        cam->targetX += (ms * (s16)sc) >> 12;
        cam->targetZ += (ms * (sc >> 16)) >> 12;
    }
    if (pad & 0x4000) {
        cam->targetX -= (ms * (s16)sc) >> 12;
        cam->targetZ -= (ms * (sc >> 16)) >> 12;
    }
    if (pad & 1) {
        {
        s32 dx = ms * (s16)tbl[(cam->rotY - 0x400) & 0xFFF];
        s32 dz;
        sc = *(volatile s32 *)&tbl[(cam->rotY - 0x400) & 0xFFF];
        dz = ms * (sc >> 16);
        cam->targetX += dx >> 12;
        cam->targetZ += dz >> 12;
        }
    }
    if (g_PadHeld & 2) {
        {
        s32 dx = ms * (s16)tbl[(cam->rotY + 0x400) & 0xFFF];
        s32 dz;
        sc = *(volatile s32 *)&tbl[(cam->rotY + 0x400) & 0xFFF];
        dz = ms * (sc >> 16);
        cam->targetX += dx >> 12;
        cam->targetZ += dz >> 12;
        }
    }
    if ((g_PadHeld & 0xF) == 0xF) {
        cam->targetX = 0;
        cam->targetY = -800;
        cam->targetZ = 0;
        cam->rotX = 0;
        cam->rotY = 0;
        cam->rotZ = 0;
    }
    func_80050960();
    func_8003BC98((u8 *)cam + 0x80, (u8 *)cam + 0x30, 0);
    GsSetRefView2(D_800B0C10);
    GsGetLs(D_800D3238, D_800B0808);
}

s32 func_8002D4E4(s32 arg0) {
    s32 i;
    u8 *p;
    s32 n;
    s32 st;
    u8 *pl;
    if (*(u16 *)((u8 *)g_Args + 0x114) == 0) {
        st = D_800B0DA0;
        p = (u8 *)D_800AFFC8 + st * 0x124;
        i = st;
        n = D_800B07B0;
        if (i <= n) {
            pl = g_Player;
            do {
                if (p[4] != 0 && *(s32 *)(pl + 0x34) <= 0) {
                    **(u32 **)(p + 0x118) |= 0x4000;
                }
                i++;
                p += 0x124;
            } while (i <= n);
        }
        D_8009D3AC = 1;
        D_8009CF54 = 1;
        D_8009D914 = 0;
        func_80061708(3);
        Camera_ResetZoom();
        D_800AFFA4 = 0;
        D_8009D010 = arg0;
        g_Args->unk115 = 1;
        return 1;
    }
    return 0;
}

void func_8002D604(void) {
    D_8009CAAC = g_Args->unk2DC;
    func_80026C90(-1);
    if (g_Args->missionComplete != 0) goto block_5;
    if (g_Args->unk115 == 0) goto other;
    if (M2C_FIELD(g_Player, s32 *, 0x34) > 0) goto block_5;
    D_800B0AA0 = 7;
    D_800B00A0 = 9;
    func_8002E0E8();
    func_8004783C();
    D_1F8003D4 = 0;
    D_1F8003D8 = 0x32;
    D_1F8003DC = 0x1F4;
    D_8009CA78 = func_80039A9C(0, D_8009D2A8, 8);
    return;
block_5:
    D_800B0AA0 = 4;
    D_800B00A0 = 9;
    return;
other:
    func_8006B230(0, (s32)D_800A735C);
}

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

void Cheat_DoAction(s32 action) {
    s16 hp;
    g_Args->flags2 |= 8;
    switch (action) {
    case 0:
        *g_DevMenu = 1;
        func_80026F30(1U, 0);
        break;
    case 1:
        func_8004B270(-D_800B0DD0, 0, 0);
        break;
    case 2:
        func_8004B270(*(s32 *)((u8 *)g_Player + 0x34) - 1, 0, 0);
        break;
    case 3:
        g_Invincible = g_Invincible == 0;
        g_GodMode = g_GodMode == 0;
        D_8009CD6C = D_8009CD6C == 0;
    default:
        break;
    case 4:
        if (*(u16 *)&g_Args->missionComplete == 0) {
            D_8009D3AC = 1;
            D_8009CF54 = 1;
            D_8009D914 = 0;
            func_80061708(2);
            Camera_ResetZoom();
            D_800AFFA4 = 0;
            g_Args->missionComplete = 1;
        }
        break;
    case 5:
        func_8002D4E4((s32)D_800A7370);
        break;
    case 6:
        D_800B0A98 = D_800B0A98 == 0;
        if (D_800B0A98 != 0) {
            D_800AFF64.r = 0xFF;
            D_800AFF64.g = 0xFF;
            D_800AFF64.b = 0;
            D_8009CAC0 = 0xFA0;
        } else {
            D_8009CAC0 = 0;
        }
        break;
    case 13:
        {
            s32 t = D_8009CABD == 0;
            if (t != D_8009CABD) {
                D_8009CABD = t;
                func_800321F0(t);
                func_8005B86C();
            }
        }
        break;
    case 14:
        {
            s32 t = D_8009CABE == 0;
            if (t != D_8009CABE) {
                D_8009CABE = t;
                func_80032348(t);
                func_8005B86C();
            }
        }
        break;
    case 7:
        Cheat_RefillAmmo();
        break;
    case 8:
        Cheat_AllWeapons();
        break;
    case 9:
        D_8009CDC0 = D_8009CDC0 == 0;
        D_8009CDC4 = -1;
    case 10:
        if (D_8009CDC0 != 0) {
            func_800406E0(&D_8009CDC4);
            if (D_8009CDC4 >= 0) {
                func_80059634(D_8009CF7C + D_8009CDC4 * 0x44);
            }
        }
        break;
    case 11:
        func_80032150();
        break;
    case 12:
        func_800314A0();
        break;
    }
    if (g_GodMode) {
        *(s16 *)((u8 *)g_Player + 0xE) = 0x7FFF;
    } else {
        *(s16 *)((u8 *)g_Player + 0xE) = *(u16 *)((u8 *)g_Player + 0xE) & 0x8000;
    }
}

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

s32 func_8002E1B0(void) {
    if (*(u16 *)((u8 *)g_Args + 0x114) == 0) {
        D_8009D3AC = 1;
        D_8009CF54 = 1;
        D_8009D914 = 0;
        func_80061708(2);
        Camera_ResetZoom();
        D_800AFFA4 = 0;
        g_Args->missionComplete = 1;
        return 1;
    }
    return 0;
}

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

void func_8002E5C4(void) {
    u16 v;
    Args *a;
    u32 f;

    func_80032024();
    func_80031F28();
    D_800B0E48 = 0;
    D_800B0DD8 = 0;
    D_800B0DD4 = 0;
    D_8009CAB0 = 0;
    *(u8 *)&D_8009CD4C = 0;
    a = g_Args;
    g_Player = (void *)D_800AFFC8;
    v = M2C_FIELD(a, u16 *, 0x142);
    M2C_FIELD(a, s16 *, 0x2C4) = 0;
    M2C_FIELD(a, s16 *, 0x2C6) = 0;
    *(u16 *)&D_8009CA5C = v;
    D_8009CA6C = v;
    D_800B0E54 = (a->flags2 >> 8) & 1;
    func_8005ED88();
    func_8003B4F4();
    D_800B0C6C = 2;
    func_8002847C();
    D_8009CDD4();
    func_8004BDA8();
    func_800530F4();
    func_8002F1BC();
    func_8002E798();
    func_8003FC60();
    func_800332A4();
    func_800478C8();
    func_8004D048();
    func_80057C40();
    g_PadHeld = 0;
    D_8009CDD8();
    M2C_FIELD(g_Args, u8 *, 0x12B) = 0;
    D_800B0AE0 = 0;
    D_800B07C4 = 0;
    D_800AFFA8 = 0;
    D_800B0070 = 0;
    D_800AFFCC = 0;
    f = g_Args->flags;
    f &= ~0x80;
    f &= ~0x40;
    g_Args->flags = f;
    func_8002FE04();
    M2C_FIELD(g_Args, s32 *, 0x2CC) = 0;
    M2C_FIELD(g_Args, s32 *, 0x2C8) = 0;
    g_Args->missionComplete = 0;
    g_Args->unk115 = 0;
    func_80075290();
    func_8004F3D8();
    func_80032074(D_8009E9C4);
    func_80031F54(D_8009E9C4);
    func_800761DC();
    func_8004B324();
    func_8002BDE4();
    func_8002F1F4();
}

#ifdef NON_MATCHING
s32 func_8002E798(void) {
    s32 n;
    s32 sz = 0x124;
    s32 i;
    u8 *obj;
    u8 *ent;
    u8 *lnk;
    s16 a, b, c, d, e, f, g;
    u8 *list;

    D_800B077C.v[0] = 0;
    D_800B077C.v[1] = 0;
    D_800B077C.v[2] = 0;
    D_800AFFC8 = 0;
    func_8006B230(1, (s32)D_800A73D8);
    n = 0;
    D_800AFFC8 = (s32)func_80030610(D_800B0AA8 * sz);
    D_800B0C84 = 0x14;
    D_800B0AB4 = func_80030610(0x410);
    if (D_800B0C84 > 0) {
        ent = D_800B0AB4 + 4;
        do {
            obj = func_80030610(0x8C);
            func_8006B230(obj != NULL, (s32)D_800A73BC);
            lnk = obj + 0x10;
            *(void **)(ent + 0x18) = obj;
            func_8007A4E4(NULL, lnk);
            *(s32 *)(obj + 0x28) = 0;
            *(s32 *)(obj + 0x2C) = 0;
            *(s32 *)(obj + 0x30) = 0;
            *(V4S *)(obj + 0x60) = *(V4S *)&D_800B077C;
            n++;
            GsLinkObject4(D_800D2D30[1], obj, 0);
            *(void **)(obj + 4) = lnk;
            *(s32 *)obj = *(s32 *)ent;
            ent += 0x34;
        } while (n < D_800B0C84);
    }
    D_800B0DA0 = 1;
    a = (u16)D_800B0868 + 1;
    b = (a + (u16)D_800B0C8C) - 1;
    c = a + D_800AFF94;
    d = (c + D_800AFF98) - 1;
    D_800B0DC0 = a;
    D_800B0DB8 = a;
    D_800B0BB4 = c - 1;
    D_800B0DC4 = c;
    D_800B07B0 = (u16)D_800B0868;
    D_800B07A8 = b;
    D_800B0BB8 = d;
    func_8006B230(d == b, (s32)D_800A7418);
    a = (u16)D_800B07A8 + 1;
    e = (a + (u16)D_800B0AA4) - 1;
    c = a + D_800AFFB0;
    f = (c + D_800AFFB4) - 1;
    D_800B0DAC = a;
    D_800B0DBC = a;
    D_800B07BC = e;
    D_800AFF70 = c - 1;
    D_800B0DCC = c;
    D_800B0BBC = f;
    func_8006B230(f == e, (s32)D_800A7424);
    a = (u16)D_800B07BC + 1;
    D_800B0DA8 = a;
    list = (u8 *)D_800AFFC8;
    b = a + (u16)D_800AFFC0;
    D_800B07B8 = b - 1;
    D_800B0DB4 = b;
    c = b + (u16)D_800B008C;
    D_800B07A4 = c - 1;
    D_800B05AC = b;
    d = b + D_800B0AC8;
    D_800B0B80 = d - 1;
    D_800B0B78 = d;
    D_800AFF58 = (d + D_800B0AE4) - 1;
    g = D_800AFF70 + 1;
    D_800B0DCC = g;
    D_800B0BBC = (g + D_800AFFB4) - 1;
    D_800B0DC8 = c;
    e = c + (u16)D_800B0C30;
    D_800B07AC = e - 1;
    D_800B0DB0 = e;
    f = e + (u16)D_800AFF80;
    D_800B07C0 = f - 1;
    D_800B0DA4 = f;
    D_800B07B4 = (f + (u16)D_800AFFC4) - 1;
    g_Player = list;
    func_8003B940(list, g_Args->playerX, g_Args->playerY, g_Args->playerZ, (s16)D_8009D35C, 0);
    *(s32 *)*(u8 **)(list + 0x90) |= 0x80000000;
    *(s8 *)(list + 0xA8) = 0x63;
    *(s8 *)(list + 0xA) = 1;
    *(s8 *)(list + 4) = 1;
    *(u16 *)(*(u8 **)(list + 0x90) + 0x62) = gPsi;
    *(s16 *)(list + 0x20) = -0x578;
    *(s16 *)(list + 0x2A) = 0x12C;
    *(s16 *)(list + 0x22) = 0;
    i = D_800B0DA0;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B07B0; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 1);
        *(s8 *)(obj + 4) = 0;
    }
    D_800AFF84 = 1;
    i = D_800B0DB8;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B0BB4; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 1);
        *(s8 *)(obj + 4) = 0;
    }
    D_800B0804 = 4;
    i = D_800B0DC4;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B0BB8; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 4);
        *(s8 *)(obj + 4) = 0;
    }
    i = D_800B0DA8;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B07B8; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 1);
        *(s8 *)(obj + 4) = 0;
    }
    i = D_800B0DBC;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= (s16)D_800AFF70; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 1);
        *(s8 *)(obj + 4) = 0;
    }
    if ((s16)D_800AFFB4 != 0) {
        i = D_800B0DCC;
        for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B0BBC; i++, obj += 0x124) {
            func_8003B940(obj, 0, 0, 0, 0, 6);
            *(s8 *)(obj + 4) = 0;
        }
    }
    i = D_800B0DB4;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B07A4; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 1);
        *(s8 *)(obj + 4) = 0;
    }
    i = D_800B0DC8;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B07AC; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 1);
        *(s8 *)(obj + 4) = 0;
    }
    i = D_800B0DB0;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B07C0; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 2);
        *(s8 *)(obj + 4) = 0;
    }
    i = D_800B0DA4;
    for (obj = (u8 *)D_800AFFC8 + i * 0x124; i <= D_800B07B4; i++, obj += 0x124) {
        func_8003B940(obj, 0, 0, 0, 0, 1);
        *(s8 *)(obj + 4) = 0;
    }
    return i > D_800B07B4;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002E798);
#endif

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

s32 Pad_Update(void) {
    s32 s;
    D_800B0E58 = func_800865BC(0);
    if (*(u8 *)&D_800B0E48 == 0) {
        D_800B0E48 = func_8008688C(0, D_800B0E54, 2);
    }
    if (PRAW.b[1] == 0x73) {
        D_800B0E54 = 1;
        PAXP.b[0] = PAX.b[0];
        PAXP.b[1] = PAX.b[1];
        PAXP.b[2] = PAX.b[2];
        PAXP.b[3] = PAX.b[3];
        PAX.b[0] = PRAW.b[4];
        PAX.b[1] = PRAW.b[5];
        PAX.b[2] = PRAW.b[6];
        PAX.b[3] = PRAW.b[7];
    } else {
        D_800B0E54 = 0;
    }
    s = (PRAW.b[2] << 8) | PRAW.b[3];
    s = ~s & 0xFFFF;
    if (func_80086688(0, 2, 0) != 0) {
        if (RUM.w8 != 0) {
            RUM.b1 = 0;
            RUM.w8 -= 1;
        } else if (RUM.w6 != 0) {
            RUM.w6 -= 1;
            RUM.b1 = RUM.b4;
        } else {
            RUM.b1 = 0;
        }
        {
        s32 t = func_800865BC(0);
        if (t == 6) {
            D_800B0E36 = 0;
        }
        if (RUM.b2 == 0) {
            func_800868D4(0, (u8 *)&RUM, 2);
            if (t == 2) {
                RUM.b2 = 1;
            }
            if (t == 6) {
                if (func_80086854(0, D_8009CDDC) != 0) {
                    RUM.b2 = 1;
                }
            }
        }
        }
        if (RUM.w8 == 0 && RUM.w6 != 0 && RUM.wA != 0) {
            *(u16 *)&RUM.b4 -= RUM.wA;
        }
    }
    if (D_800B0E58 == 0) {
        s = 0;
    }
    return s;
}

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

void func_8002F564(void *arg0, s32 arg1, s32 arg2, s16 arg3, u16 arg4) {
    s16 *p;
    s32 v;

    p = (s16 *)&D_800B0E34;
    if (*(u8 *)((u8 *)g_Args + 4) >> 7) {
        if (func_800324C0() != 0) {
            if ((s16)arg1 >= 0) {
                v = arg1;
                if ((s16)arg1 >= 0x100) {
                    v = 0xFF;
                }
            } else {
                v = 0;
            }
            if ((s16)arg2 < 2) {
                arg2 = 2;
            }
            p[2] = v;
            p[3] = arg2;
            p[4] = arg3;
            p[5] = arg4;
        }
    }
}

void func_8002F624(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        D_800B8F88[i].length = 12;
        D_800B8F88[i].org = D_800B0EFC[i];
        D_800B8EFC[i][0] = 6;
        D_800B8EFC[i][1] = (s32)D_800D28F4[i];
        D_800D2AFC[i].length = 12;
        D_800D2AFC[i].org = (u32 *)((u8 *)&D_800CA8F4 + i * 0x4000);
    }
}

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

void DebugScreen_DrawPage(s32 arg0) {
    func_80032E4C(g_DebugTextY + 1, g_DebugTextX);
    FntPrint(D_800A7474, g_DebugPageNames[arg0]);
    switch (arg0) {
    case 1: DebugPage_Framerate(); break;
    case 2: DebugPage_FPWeapon(); break;
    case 3: DebugPage_Camera(); break;
    case 5: DebugPage_Loadpoint(); break;
    case 6: DebugPage_ObjectCount(); break;
    case 7: DebugPage_Profile(); break;
    case 8: DebugPage_ArtProfile(); break;
    case 4:
        g_DebugTextX = 0x23;
        g_DebugTextY = 2;
        FntPrint(D_800A747C, g_Args->playerX, g_Args->playerY, g_Args->playerZ);
        FntPrint(D_800A7498, *((u8 *)g_Player + 0x122));
        break;
    case 9: DebugPage_ContourMask(); break;
    }
    FntFlush(-1);
}

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

#ifdef NON_MATCHING
void func_8002FFD8(void) {
    SetDefDrawEnv(D_800B0EA0, 0, 0, *(s16 *)((u8 *)g_Args + 0x190), *(s16 *)((u8 *)g_Args + 0x192));
    D_800B0EA0[0x18] = 0;
    D_800B0EA0[0x16] = 1;
    func_8007F530(D_800B0EA0);
    SetDefDispEnv(D_800B0E8C, 0, 0, *(s16 *)((u8 *)g_Args + 0x190), *(s16 *)((u8 *)g_Args + 0x192));
    func_8007F5F0(D_800B0E8C);
    func_8007F0D8(0);
    func_80088C44(&D_800D2BC4);
    M2C_FIELD(&D_800D2BC4, u8 *, 4) = 0x38;
    M2C_FIELD(&D_800D2BC4, u8 *, 20) = 0x38;
    M2C_FIELD(&D_800D2BC4, u8 *, 5) = 0x30;
    M2C_FIELD(&D_800D2BC4, u8 *, 21) = 0x30;
    M2C_FIELD(&D_800D2BC4, u8 *, 6) = 0xA0;
    M2C_FIELD(&D_800D2BC4, u8 *, 22) = 0xA0;
    M2C_FIELD(&D_800D2BC4, u8 *, 12) = 0xF0;
    M2C_FIELD(&D_800D2BC4, u8 *, 13) = 0xCB;
    M2C_FIELD(&D_800D2BC4, u8 *, 14) = 0x1E;
    M2C_FIELD(&D_800D2BC4, u8 *, 28) = 0xF0;
    M2C_FIELD(&D_800D2BC4, u8 *, 29) = 0xCB;
    M2C_FIELD(&D_800D2BC4, u8 *, 30) = 0x1E;
    ((void (*)(void *, s32, s32))func_80081CE4)(&D_800D2BC4, 0, *(u8 *)((u8 *)&D_800D2BC4 + 0x1C));
    M2C_FIELD(&D_800D2BC4, s16 *, 8) = 0x4A;
    M2C_FIELD(&D_800D2BC4, s16 *, 10) = 0xCC;
    M2C_FIELD(&D_800D2BC4, s16 *, 18) = 0xCC;
    M2C_FIELD(&D_800D2BC4, s16 *, 24) = 0x4A;
    M2C_FIELD(&D_800D2BC4, s16 *, 26) = 0xCF;
    M2C_FIELD(&D_800D2BC4, s16 *, 16) = *(u16 *)((u8 *)g_Args + 0x2E0) + 0x4A;
    M2C_FIELD(&D_800D2BC4, s16 *, 34) = 0xCF;
    M2C_FIELD(&D_800D2BC4, s16 *, 32) = *(u16 *)((u8 *)g_Args + 0x2E0) + 0x4A;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8002FFD8);
#endif

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

s32 func_800302AC(void) {
    s32 size;
    if (D_800B8F24 == 0) {
        size = -0x8CA0;
        return D_800B8FB4[0] - (D_800FC640 + size);
    } else {
        size = -0x11940;
        return D_800B8FB4[0] - (D_800FC640 + size);
    }
}

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

#ifdef NON_MATCHING
void func_8003052C(void) {
    s32 t;
    s32 a = 0x801687D0;
    s32 b = 0x97830;
    *(s32 *)((u8 *)g_Args + 0x1EC) = a;
    *(s32 *)((u8 *)g_Args + 0x1F0) = b;
    InitHeap3(a, 0x97830, b);
    t = *(s32 *)((u8 *)g_Args + 0x1F0);
    *(s32 *)((u8 *)g_Args + 0x1F4) = 0;
    if (t & 7) {
        sprintf(g_Args->exePath, D_800A74D4, t);
        func_8006B230(0, g_Args->exePath);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003052C);
#endif

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

void *func_80030610(s32 arg0) {
    void *p;

    p = calloc3(arg0, 1);
    if (p == NULL) {
        printf(D_800A7510, arg0);
        exit(-1);
    }
    arg0 = ((s32 *)p)[-1] * 8;
    M2C_FIELD(g_Args, s32 *, 0x1F4) += arg0;
    return p;
}

void func_80030670(s32 *p) {
    *(s32 *)((u8 *)g_Args + 0x1F4) -= p[-1] << 3;
    free3(p);
}

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

#ifdef NON_MATCHING
void func_80030798(u8 **arg0) {
    u8 *o;
    u32 v;
    s32 i;
    func_8006B230(*(s32 *)((u8 *)g_Args + 0x214) < 0x51, (M2C_UNK)D_800A75D0);
    i = 0;
    if (*(s32 *)((u8 *)g_Args + 0x214) + 1 > 0) {
        do {
            o = arg0[i];
            GsMapModelingData(o + 4);
            v = *(u32 *)(o + 8);
            o += 0xC;
            D_800D2D30[i] = (s32)o;
            D_800D2BF0[i] = v;
            i++;
        } while (i < *(s32 *)((u8 *)g_Args + 0x214) + 1);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030798);
#endif

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

#ifdef NON_MATCHING
void func_80030994(u8 *arg0) {
    RECT16 r1;
    RECT16 r2;
    s32 buf;
    RECT16 *a;
    RECT16 *b;
    buf = func_80031F18();
    a = &r1;
    b = &r2;
    a->x = 0x200;
    a->h = 0x80;
    a->w = 0x140;
    a->y = 0x100;
    b->x = 0x200;
    b->y = 0x1CD;
    b->w = 0x100;
    b->h = 6;
    if (func_8007D2B4(a, b) != 0) {
        printf(&D_800A7678, &D_800A76A0, 0x15A);
        exit(1);
    }
    func_8006B230(1, (s32)D_800A773C);
    func_8002C168((M2C_UNK *)arg0, buf);
    func_8006B230(func_8007DA88(buf, 0) == 0, (s32)D_800A76E4);
    buf += 0x5258;
    func_8006B230(func_8007DA88(buf, 1) == 0, (s32)D_800A76F8);
    buf += 0x5258;
    if (strcmp(D_800D2F48[2], (char *)&D_800A770C) != 0) {
        func_8006B230(func_8007DA88(buf, 2) == 0, (s32)D_800A7714);
        buf += 0x5258;
    }
    if (strcmp(D_800D2F48[3], (char *)&D_800A770C) != 0) {
        func_8006B230(func_8007DA88(buf, 3) == 0, (s32)D_800A7728);
    }
    func_800303B4(1);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80030994);
#endif

void func_80030B04(char *arg0) {
    Buf10 sp10;
    char *sp20;
    char *p;
    char *tok;
    char *dst;
    s32 off;
    s16 i;
    sp10 = D_800A7750;
    for (i = 0; i < 6; i++) {
        p = strstr(arg0, D_8009CDF0[i].key);
        if (p) {
            strcmp(strtok(p, (char *)&sp10), D_800A775C);
            tok = strtok(0, (char *)&sp10);
            switch (D_8009CDF0[i].type) {
            case 2:
                strcpy(&D_800D2E90[D_8009CE20 << 5], tok);
                break;
            case 1:
                D_8009CE20 = strtol(tok, &sp20, 10) - 1;
                ((Rec32 *)&D_800D2E84)[D_8009CE20].b[0] = 1;
                ((Rec32 *)&D_800D2E84)[D_8009CE20].b[2] = D_8009CE20;
                ((Rec32 *)&D_800D2E84)[D_8009CE20].b[3] = D_8009CE20;
                ((Rec32 *)&D_800D2E84)[D_8009CE20].b[5] = 0;
                break;
            case 3:
                strcpy(&D_800D2F48[0][D_8009CE24 * 16], tok);
                break;
            case 5:
                D_8009CE24 = strtol(tok, &sp20, 10) - 1;
                break;
            case 4:
                ((Rec32 *)&D_800D2E84)[D_8009CE20].b[1] = strtol(tok, &sp20, 10) - 1;
                break;
            }
            return;
        }
    }
}

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

void func_80030F20(void) {
    *(Str5 *)D_800D2F48[0] = D_800A770C;
    *(Str5 *)D_800D2F48[1] = D_800A770C;
    *(Str5 *)D_800D2F48[2] = D_800A770C;
    *(Str5 *)D_800D2F48[3] = D_800A770C;
}

void func_80030FA8(s16 *arg0) {
    s32 a1;
    s32 s0;
    s32 s1;
    s32 s2;
    a1 = arg0[3];
    s0 = arg0[1];
    s1 = arg0[2];
    s2 = arg0[4];
    if (g_Player != NULL) {
        M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), s16 *, 0x62) = s0;
        M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), s32 *, 0x28) = s1;
        M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), s32 *, 0x2C) = a1;
        M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), s32 *, 0x30) = s2;
        func_80060AC0(g_Player);
        a1 = M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), s32 *, 0x2C);
    }
    gPsi = s0;
    D_800DF0F8 = s0 << 6;
    *(s32 *)&D_800B0C5C = s0;
    D_800B0AEC = s0;
    g_Camera.rotY = s0;
    g_Args->playerY = a1;
    a1 -= 0x578;
    g_Args->playerX = s1;
    g_Args->playerZ = s2;
    g_Camera.targetX = s1;
    g_Camera.targetY = a1;
    g_Camera.targetZ = s2;
    D_800D9200[0] = s1;
    D_800D9200[1] = a1;
    D_800D9200[2] = s2;
    func_80050960();
    func_8004F3D8();
}

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

void func_80031678(void) {
    FxHdr *hdr;
    ColCycle *e;
    UvScroll *f;
    s32 i;
    s32 buf;
    s32 level;
    s32 id;
    u32 p;
    u32 v;
    u8 c[4];

    hdr = *(FxHdr **)(D_800D2FC0 + 4);
    if (D_800D2F90 == 2 && (func_8004B084() << 16) == 0 && func_80075FA0() == 0) {
        D_800D2F90 = 4;
        func_80026C90(D_8009E410);
        func_80069850(1);
        D_800B0BA0 = -1;
        D_8009EEDC = -1;
        if (M2C_FIELD(M2C_FIELD(&D_8009E9C4, s32 **, 0x10), s32 *, 0xC) >= 0) {
            func_80026F30(M2C_FIELD(M2C_FIELD(&D_8009E9C4, s32 **, 0x10), s32 *, 0xC), 0);
        }
        if (D_8009CE48 != NULL) {
            D_8009CE48(*M2C_FIELD(&D_8009E9C4, s16 **, 0x10));
        }
        id = *M2C_FIELD(&D_8009E9C4, s16 **, 0x10);
        level = g_Args->level;
        buf = D_800D2FC0;
        if (!((M2C_FIELD(&D_8009E9C4, u32 *, 0x14) >> 23) & 1)) {
            sprintf(g_Args->exePath, D_800A77B4, level, level, id);
            func_8002C168(g_Args->exePath, buf);
            func_80032768(buf);
        }
        sprintf(g_Args->exePath, D_800A77CC, level, level, id);
        func_8002C168(g_Args->exePath, buf);
        D_800D2F90 = 5;
        func_800310B4(buf);
        p = D_8009CE38;
        sprintf(g_Args->exePath, &D_800A7788, g_Args->level, g_Args->level, *M2C_FIELD(&D_8009E9C4, s16 **, 0x10));
        if (func_8002C168(g_Args->exePath, p) > 0x1800) {
            func_8006B1DC((s32 *)g_Args->exePath);
            func_8006B1DC(&D_800A77A0);
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
        return;
    }
    if (D_800D2F90 == 1 && (func_8004B084() << 16) == 0) {
        D_800D2F90 = 0;
    }
    if (hdr != NULL) {
        e = hdr->col;
        if (e != NULL) {
            for (i = 0; i < hdr->nCol; i++, e++) {
                if (e->timer == 0) {
                    if (++e->idx >= e->count) {
                        e->idx = 0;
                    }
                    v = e->table[e->idx];
                    if (D_8009CABD != 0) {
                        *(u32 *)c = v;
                        c[1] = ((c[0] + c[1] + c[2]) >> 3) + 0x30;
                        c[0] = 0;
                        c[2] = 0;
                        v = *(u32 *)c;
                    } else if (D_8009CABE != 0) {
                        v = 0;
                    }
                    e->color = v & 0xFFFFFF;
                    e->timer = v >> 24;
                }
                if (e->timer > 0) {
                    e->timer--;
                }
            }
        }
        f = hdr->uv;
        if (f != NULL) {
            for (i = 0; i < hdr->nUv; i++, f++) {
                if (!(f->flags & 0x8000)) {
                    f->au += f->du;
                    f->av += f->dv;
                    f->u = f->au >> 9;
                    f->v = f->av >> 9;
                }
            }
        }
    }
    if (D_800D2FA4 != 0) {
        if (--D_800D2FA4 == 0) {
            D_800D2FA4 = -1;
        }
        if (D_800D2FA4 < 0) {
            if ((D_8009CAC0 -= 1000) < 0) {
                D_8009CAC0 = 0;
            }
            if (D_8009CAC0 <= 0) {
                D_800D2FA4 = 0;
            }
        }
    }
    if (D_800D2FA8 != 0) {
        if (--D_800D2FA8 == 0) {
            D_800D2FA8 = -1;
        }
        if (D_800D2FA8 < 0) {
            if ((D_8009CAC0 -= 1000) < 0) {
                D_8009CAC0 = 0;
            }
            if (D_8009CAC0 <= 0) {
                D_800D2FA8 = 0;
            }
        }
    }
}

void func_80031B58(u8 *arg0) {
    u32 *cmd;
    u32 id;
    s32 *p;
    s32 q;
    s32 *st;
    s32 i;
    u8 *e;

    cmd = *(u32 **)(arg0 + 0x1C);
    p = (s32 *)(cmd + 1);
    id = cmd[0] >> 27;
    if (((s32 (*)(void))func_80044AF4)() != 0) {
        return;
    }
    if (func_8004A370() != 0) {
        return;
    }
    e = &D_8009E9DC;
    i = 0;
    for (; i < 3; i++, e += 0x10) {
        if (*(s16 *)(e + 0xA) == id) {
            if (D_8009CE40 != NULL && D_8009CE40(*(s16 *)e) == 0) {
                return;
            }
            q = *p;
            func_8002F53C();
            func_8004783C();
            Menu_ResetState();
            if (i >= 3) {
                sprintf(g_Args->exePath, D_800A77E4, i);
                func_8006B1DC((M2C_UNK *)g_Args->exePath);
            } else {
                st = (s32 *)&D_8009E9C4;
                st[4] = (s32)((i * 0x10) + &D_8009E9DC);
                st[5] = q;
                func_8004B04C();
                D_800D2F90 = 2;
            }
            D_8009CE34 = i;
            return;
        }
    }
    sprintf(g_Args->exePath, D_800A7800, id);
    func_8006B1DC((M2C_UNK *)g_Args->exePath);
}

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

void func_80031D94(void) {
    s32 t;
    if (g_PadPressed & 4) {
        D_8009CE34 = D_8009CE34 + 1;
        if (D_8009CE34 >= 3) {
            D_8009CE34 = -1;
        }
    }
    if (g_PadPressed & 1) {
        D_8009CE34 = D_8009CE34 - 1;
        if (D_8009CE34 < -1) {
            D_8009CE34 = 2;
        }
    }
    if (g_PadPressed & 0x100) {
        if (D_8009CE40 != 0) {
            s16 *e;
            if (D_8009CE34 >= 0) {
                e = (s16 *)(&D_8009E9DC + D_8009CE34 * 0x10);
            } else {
                e = &D_8009E9C4;
            }
            D_8009CE40(*e);
        }
        t = D_8009CE34;
        func_8002F53C();
        func_8004783C();
        Menu_ResetState();
        if (t >= 3) {
            sprintf((char *)g_Args->exePath, D_800A77E4, t);
            func_8006B1DC((M2C_UNK *)g_Args->exePath);
            return;
        }
        M2C_FIELD(&D_8009E9C4, void **, 0x10) = (void *)((t * 0x10) + &D_8009E9DC);
        M2C_FIELD(&D_8009E9C4, s32 *, 0x14) = 0;
        func_8004B04C();
        D_800D2F90 = 2;
    }
}

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

void func_80032074(s32 arg0) {
    s32 lvl;
    s32 base;
    lvl = g_Args->level;
    base = D_800D2FC0;
    if (!(((u32)D_8009E9D8 >> 23) & 1)) {
        sprintf(g_Args->exePath, D_800A77B4, lvl, lvl, arg0);
        func_8002C168(g_Args->exePath, base);
        func_80032768(base);
    }
    sprintf(g_Args->exePath, D_800A77CC, lvl, lvl, arg0);
    func_8002C168(g_Args->exePath, base);
    D_800D2F90 = 5;
    func_800310B4(base);
}

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

void func_8003246C(u16 *a, s32 b, s32 c, C3 *d) {
    D_800AFF64.r = d->r; D_800AFF64.g = d->g; D_800AFF64.b = d->b; D_8009CAC0 = b;
    D_800AFF68.x = a[0];
    D_800AFF68.y = a[2];
    D_800D2FA4 = c;
    D_800AFF68.z = a[4];
}

s32 func_800324C0(void) {
    return D_800D2F90 == 0;
}

void func_800324D0(s32 timArg, s16 x, s16 y) {
    u32 *tim = (u32 *)timArg;
    GsIMAGE img;
    s16 rect[4];
    u32 *p;
    s32 hasClut;
    u32 mode;
    u32 n;

    if (*tim != 0x10) {
        printf(&D_800A79F0, tim);
        exit(0);
    }
    tim++;
    p = tim;
    mode = *p & 7;
    hasClut = 0;
    if (mode < 2) {
        hasClut = 1;
    } else if (mode >= 4) {
        func_8006B230(0, (s32)&D_800A7A0C);
    }
    tim++;
    n = *tim >> 2;
    if (hasClut) {
        tim += n + 1;
    } else {
        tim++;
    }
    ((s16 *)tim)[0] = x;
    ((s16 *)tim)[1] = y;
    GsGetTimInfo(p, &img);
    rect[0] = img.px;
    rect[1] = img.py;
    rect[2] = img.pw;
    rect[3] = img.ph;
    func_8007F2EC((M2C_UNK *)rect, (M2C_UNK *)img.pixel);
    if ((img.pmode & 3) == 1) {
        func_8007F0D8(0);
        LoadClut(img.clut, img.cx, img.cy);
    } else if ((img.pmode & 3) == 0) {
        func_8007F0D8(0);
        LoadClut2(img.clut, img.cx, img.cy);
    }
    func_8007F0D8(0);
}

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

void func_80032724(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 r[4];

    r[0] = arg0;
    r[1] = arg1;
    r[2] = arg2;
    r[3] = arg3;
    func_8007F25C(r, 0, 0, 0);
    func_8007F0D8(0);
}

M2C_UNK func_80032768(s32 arg0) {
    s16 rect[4];
    GsIMAGE img;
    u32 *p;
    register s32 n REG("v1");
    s32 w;
    s32 h;

    p = (u32 *)arg0;
    func_8007F0D8(0);
loop:
    if (*p++ == 0x10) {
        GsGetTimInfo(p, &img);
        n = img.pw * img.ph / 2;
        n = n * 4 + 0x10;
        p = (u32 *)((u8 *)p + n);
        rect[0] = img.px;
        rect[1] = img.py;
        rect[2] = img.pw;
        rect[3] = img.ph;
        func_8007F2EC((M2C_UNK *)rect, (M2C_UNK *)img.pixel);
        func_8007F0D8(0);
        if ((img.pmode >> 3) & 1) {
            rect[0] = img.cx;
            rect[1] = img.cy;
            rect[2] = img.cw;
            rect[3] = img.ch;
            func_8007F2EC((M2C_UNK *)rect, (M2C_UNK *)img.clut);
            func_8007F0D8(0);
            n = img.cw * img.ch / 2;
            p += n + 3;
        }
        goto loop;
    }
}

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

void DevMenu_HandleInput(void) {
    s32 cur;
    s32 pressed;
    s32 v;

    pressed = (g_PadHeld ^ g_PadPrev) & g_PadHeld;
    cur = ((DevMenu *)g_DevMenu)->cursor;
    if (pressed & 0x800) {
        ((DevMenu *)g_DevMenu)->active = 0;
        func_80026F30(4, 0);
        D_8009CAC4 = 1;
        return;
    }
    ((s32 *)g_DevMenu)[21] = g_Invincible != 0;
    ((s32 *)g_DevMenu)[22] = D_8009CD6C == 0;
    ((s32 *)g_DevMenu)[23] = D_8009CA8C != 0;
    ((s32 *)g_DevMenu)[24] = D_8009CDE8 != 0;
    ((s32 *)g_DevMenu)[25] = D_8009CD6E != 0;
    if (pressed & 0x1000) {
        do {
            if (cur == 0) {
                cur = 11;
            } else {
                cur--;
            }
        } while (((DevMenu *)g_DevMenu)->vals[cur] < 0);
        func_80026F30(3, 0);
    }
    if (pressed & 0x4000) {
        do {
            if (cur < 11) {
                cur++;
            } else {
                cur = 0;
            }
        } while (((DevMenu *)g_DevMenu)->vals[cur] < 0);
        func_80026F30(3, 0);
    }
    ((DevMenu *)g_DevMenu)->cursor = cur;
    if (pressed & 0x8000) {
        v = ((DevMenu *)g_DevMenu)->vals[cur];
        if (v != 0) {
            v--;
        } else {
            v = ((DevMenu *)g_DevMenu)->count[cur] - 1;
        }
        ((DevMenu *)g_DevMenu)->vals[cur] = v;
        func_80026F30(3, 0);
    }
    if (pressed & 0x2000) {
        v = ((DevMenu *)g_DevMenu)->vals[cur];
        if (v < ((DevMenu *)g_DevMenu)->count[cur] - 1) {
            v++;
        } else {
            v = 0;
        }
        ((DevMenu *)g_DevMenu)->vals[cur] = v;
        func_80026F30(3, 0);
    }
    g_GodMode = g_Invincible = ((DevMenu *)g_DevMenu)->vals[7] == 1;
    D_8009CA8C = ((DevMenu *)g_DevMenu)->vals[9] == 1;
    D_8009CD6C = ((DevMenu *)g_DevMenu)->vals[8] == 0;
    D_8009CDE8 = ((DevMenu *)g_DevMenu)->vals[10] == 1;
    D_8009CD6E = ((DevMenu *)g_DevMenu)->vals[11] == 1;
}

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

void DevMenu_Init(void) {
    s32 i;
    PF4 *a, *b, *c;
    i = 0;
    c = (PF4 *)D_800D308C;
    b = (PF4 *)D_800D2FF4;
    a = (PF4 *)D_800D2FC4;
    g_DevMenu[0] = 0;
    g_DevMenu[2] = 10;
    g_DevMenu[3] = 5;
    g_DevMenu[4] = 2;
    g_DevMenu[5] = 3;
    g_DevMenu[6] = 4;
    g_DevMenu[7] = 2;
    g_DevMenu[8] = 2;
    g_DevMenu[9] = 2;
    g_DevMenu[10] = 2;
    g_DevMenu[11] = 2;
    g_DevMenu[12] = 2;
    g_DevMenu[13] = 2;
    g_DevMenu[1] = 0;
    g_DevMenu[14] = 0;
    g_DevMenu[15] = 0;
    g_DevMenu[16] = 1;
    g_DevMenu[17] = 1;
    g_DevMenu[18] = -1;
    g_DevMenu[19] = -1;
    g_DevMenu[20] = -1;
    g_DevMenu[21] = 0;
    g_DevMenu[22] = 0;
    g_DevMenu[23] = 0;
    g_DevMenu[24] = 1;
    g_DevMenu[25] = 1;
    do {
        i++;
        a->len = 5; a->code = 0x28; a->r = 0; a->g = 0; a->b = 0x64;
        a->x0 = 0x10; a->y0 = 0x14; a->x1 = 0x116; a->y1 = 0x14;
        a->x2 = 0x10; a->y2 = 0x7C; a->x3 = 0x116; a->y3 = 0x7C;
        b->len = 5; b->code = 0x28; b->r = 0; b->g = 0x80; b->b = 0x80;
        c->len = 5; c->code = 0x28; c->r = 0; c->g = 0; c->b = 0x64;
        a++; b++; c++;
    } while (i < 2);
}

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

#ifdef NON_MATCHING
u8 *func_80032EFC(SObj *arg0, s32 arg1) {
    s32 n;
    s32 cnt;
    SpawnEnt *base;
    s32 t1;
    u8 *ret;
    u8 *r2;
    SpawnEnt *e;
    SpawnEnt *ent;
    s32 *q;
    u32 w;
    s32 f;
    n = 0;
    ret = 0;
    arg1 &= ~0x100;
    t1 = arg0->p8[1] & ~7;
    cnt = D_8009CF14[2];
    base = (SpawnEnt *)D_8009CF14[3];
    if (cnt > 0) {
        e = base;
        while (1) {
            ent = e;
            if (e->info.flags == 0) break;
            n++;
            e++;
            if (n >= cnt) { ent = 0; break; }
        }
    } else {
        ent = 0;
    }
    if (ent != 0) {
        ent->x = arg0->x - D_800D2FB0.x;
        ent->y = arg0->y - D_800D2FB0.y;
        ent->info.flags = arg1;
        ent->info.w4 = t1;
        ent->info.w5 = t1;
        ent->z = arg0->z - D_800D2FB0.z;
        ret = func_8003C1BC(ent, arg0->x, arg0->y, arg0->z);
    }
    r2 = ret;
    if (r2 != 0) {
        q = *(s32 **)(r2 + 0x118);
        if (q != 0) {
            q[4] |= 4;
            D_8009CF2C = 0x28;
            arg0->wC = ((arg0->w10 >> 3) & 7) * 0x14;
            w = arg0->w10;
            if (!((w >> 10) & 1)) {
                f = (w >> 6) & 0xF;
                if (f == 0 || (arg0->w10 = (w & ~0x3C0) | (((f - 1) & 0xF) << 6), ((arg0->w10 >> 6) & 0xF) == 0)) {
                    *arg0->p8 = 0;
                    arg0->w10 = 0;
                }
            }
        }
    }
    return r2;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80032EFC);
#endif

void func_80033090(void) {
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 a1;
    s32 s5;
    u32 f;
    u8 *a0;
    if (D_8009CF74 != 0) {
        return;
    }
    if (D_8009D8E4 != 0) {
        return;
    }
    if (D_8009CF10 == 0) {
        return;
    }
    if (D_8009CF10 > 0) {
        D_8009CF10 -= 1;
    }
    if (D_8009CF2C > 0) {
        D_8009CF2C -= 1;
    }
    a1 = 0;
    s2 = 0;
    a0 = (u8 *)D_800AFFC8 + D_800B0DA0 * 0x124;
    if (D_800B0868 > 0) {
        do {
            if (a0[4] != 0) {
                a1 += 1;
            }
            s2 += 1;
            a0 += 0x124;
        } while (s2 < D_800B0868);
    }
    if (a1 < 5) {
        s5 = M2C_FIELD(&D_8009CF14, s32 *, 4);
        s2 = 0;
        if (M2C_FIELD(&D_8009CF14, s32 *, 0) > 0) {
            do {
                f = M2C_FIELD(s5, u32 *, 0x10);
                if ((f & 0x4001) == 0x4001) {
                    if (M2C_FIELD(s5, s32 *, 0xC) != 0) {
                        M2C_FIELD(s5, s32 *, 0xC) -= 1;
                    } else if (D_8009CF2C == 0) {
                        s0 = 0;
                        if (((f >> 1) & 3) == 0) {
                            s1 = M2C_FIELD(&D_8009CF14, s32 *, 0x10);
                            s3 = s1 << 16;
                        } else {
                            s1 = M2C_FIELD(&D_8009CF14, s32 *, 0x14);
                            s3 = s1 << 16;
                        }
                        do {
                            if (D_800DE0C8[s0] == 0 && ((s32 (*)(s32, s32, s16))func_8005B584)(0, s3 >> 16, s0) != 0) {
                                func_80032EFC(s5, s1);
                                s2 += 1;
                                goto next;
                            }
                            s0 += 1;
                        } while (s0 < 6);
                    }
                }
                s2 += 1;
next:
                s5 += 0x14;
            } while (s2 < M2C_FIELD(&D_8009CF14, s32 *, 0));
        }
    }
}

void func_800332A4(void) {
    s32 i;
    E14 *p = ((Tbl *)D_8009CF14)->p;
    E18 *q;
    for (i = 0; i < ((Tbl *)D_8009CF14)->n;) {
        i++;
        p->y = 0;
        p->x = 0;
        p++;
    }
    q = ((Tbl *)D_8009CF14)->q;
    for (i = 0; i < ((Tbl *)D_8009CF14)->m;) {
        i++;
        q->x = 0;
        q++;
    }
}

void func_8003331C(s32 arg0) {
    if ((D_8009CF10 < arg0) || (arg0 < 0)) {
        D_8009CF10 = arg0;
    }
}

void func_80033348(void) {
    s32 i;
    F14 *p = ((Tbl *)D_8009CF14)->p;
    u8 *q;
    for (i = 0; i < ((Tbl *)D_8009CF14)->n;) {
        p->u2 = 0;
        p->u1 = 0;
        *p->e &= ~0x100;
        i++;
        p++;
    }
    q = ((Tbl *)D_8009CF14)->q;
    for (i = 0; i < ((Tbl *)D_8009CF14)->m;) {
        i++;
        *(s32 *)(q + 0xC) = 0;
        q += 0x18;
    }
}

u8 *func_800333D4(void) {
    s32 i;
    u8 *p = (u8 *)D_8009CF14[1];
    for (i = 0; i < D_8009CF14[0]; i++) {
        if (!(*(s32 *)(p + 0x10) & 1)) return p;
        p += 0x14;
    }
    return 0;
}

#ifdef NON_MATCHING
u8 *func_80033420(s16 *arg0, s32 arg1, s32 arg2) {
    s32 i = 0;
    u8 *r = 0;
    s32 *e = 0;
    s32 *p;
    s32 n = D_8009CF14[2];
    if (n > 0) {
        p = (s32 *)D_8009CF14[3];
        do {
            if (p[3] == 0) {
                e = p;
                break;
            }
            i++;
            p += 6;
        } while (i < n);
    }
    if (e != 0) {
        e[0] = arg0[0] - D_800D2FB0.x;
        e[1] = arg0[1] - D_800D2FB0.y;
        e[2] = arg0[2] - D_800D2FB0.z;
        e[3] = arg1;
        e[4] = arg2;
        e[5] = arg2;
        r = func_8003C1BC(e, arg0[0], arg0[1], arg0[2]);
    }
    return r;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80033420);
#endif

void func_800334F4(s32 *arg0) {
  int new_var2;
  int new_var;
  u32 t;
  s32 h;
  s32 i;
  LEnt *e;
  s32 k;
  t = (*(*((u32 **) (((u8 *) arg0[1]) + 0x1C)))) >> 27;
  h = ((s32) t) >> 1;
  if (arg0[0] == ((s32) g_Player))
  {
 ;
    new_var2 = t ^ 1;
    i = 0;
    e = (LEnt *) D_8009CF14[1];
    if (D_8009CF14[0] > 0)
    {
      do
      {
        if ((e->flags & 1) && ((k = e->obj[1], (h < 0) || ((k & 7) == h))))
        {
          new_var = (new_var2 & 1) << 14;
          e->flags = (e->flags & (~0x4000)) | ((unsigned int) new_var);
          e->z = 0;
        }
        i++;
        e++;
      }
      while (i < D_8009CF14[0]);
    }
  }
}

#ifdef NON_MATCHING
void func_800335B4(void) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 temp_a0;
    s32 temp_a0_2;

    sp0 = 0x28000;
    sp4 = 0x03FFFFFF;
    sp8 = 0xFC00FFFF;
    g_Weapons->pad5E[0x21] = 4;
    M2C_FIELD(g_Weapons, s16 *, 0xB2) = 0x2EE0;
    g_Weapons->pad5E[0x17] = 5;
    g_Weapons->pad5E[0x18] = 6;
    g_Weapons->pad5E[0x20] = 3;
    g_Weapons->pad5E[0x19] = 1;
    g_Weapons->pad5E[0x1A] = -1U;
    g_Weapons->pad5E[0x1D] = 0;
    g_Weapons->pad5E[0x1E] = 2;
    g_Weapons->pad5E[0x1F] = 2;
    g_Weapons->pad5E[0x37] = -1U;
    g_Weapons->pad5E[0x38] = -1U;
    g_Weapons->pad5E[0x3A] = -1U;
    M2C_FIELD(g_Weapons, s32 *, 0xB4) = 0x14;
    g_Weapons->pad51[0xA] = D_8009D2B4;
    g_Weapons->flags = (g_Weapons->flags & 0xFFF87FFF) | sp0 | 0x400000 | 0x800000;
    M2C_FIELD(g_Weapons, s32 *, 0) = (s32) M2C_FIELD(&D_800A7C14, s32 *, 0);
    M2C_FIELD(g_Weapons, s32 *, 4) = (s32) M2C_FIELD(&D_800A7C14, s32 *, 4);
    M2C_FIELD(g_Weapons, s32 *, 8) = (s32) M2C_FIELD(&D_800A7C14, s32 *, 8);
    g_Weapons->pad0[0x1C] = D_800A7C20;
    M2C_FIELD(g_Weapons, s16 *, 0xB0) = 0x16;
    g_Weapons->ammo = 0x50;
    g_Weapons->clipSize = 0x10;
    g_Weapons->clip = 0x10;
    M2C_FIELD(g_Weapons, s16 *, 0xAE) = 0xA;
    M2C_FIELD(g_Weapons, s8 *, 0x46) = 6;
    M2C_FIELD(g_Weapons, s8 *, 0x47) = 5;
    M2C_FIELD(g_Weapons, void (**)(), 0x3C) = func_80038448;
    M2C_FIELD(g_Weapons, void (**)(), 0x40) = func_80069318;
    g_Weapons->reserve = 0x40;
    g_Weapons->pad4C[2] = 1;
    g_Weapons->pad4C[3] = 1;
    g_Weapons->maxAmmo = 0x60;
    M2C_FIELD(g_Weapons, s16 *, 0xAC) = 0x3E8;
    g_Weapons->pad51[2] = 0x20;
    g_Weapons->pad51[7] = 0x18;
    M2C_FIELD(g_Weapons, s16 *, 0x60) = 0x3C;
    M2C_FIELD(g_Weapons, s16 *, 0x62) = 0x2D;
    g_Weapons->pad51[9] = 0x2B;
    g_Weapons[3].pad5E[0x17] = 7;
    M2C_FIELD(g_Weapons, s16 *, 0x64) = 0x11D;
    g_Weapons[3].pad5E[0x18] = 8;
    M2C_FIELD(g_Weapons, s16 *, 0xAA) = 0x294;
    g_Weapons->pad51[0] = 3;
    g_Weapons->pad51[1] = 2;
    M2C_FIELD(g_Weapons, s16 *, 0x66) = 0x136;
    M2C_FIELD(g_Weapons, s16 *, 0x68) = -5;
    M2C_FIELD(g_Weapons, s16 *, 0x6C) = 0xA;
    M2C_FIELD(g_Weapons, s16 *, 0x6E) = -0xA;
    g_Weapons[3].pad5E[0x20] = 3;
    g_Weapons[3].pad5E[0x21] = 3;
    g_Weapons[3].pad5E[0x19] = 1;
    g_Weapons[3].pad5E[0x1A] = 1;
    g_Weapons[3].pad5E[0x37] = 4;
    g_Weapons[3].pad5E[0x1D] = 0;
    g_Weapons[3].pad5E[0x1E] = 2;
    g_Weapons[3].pad5E[0x1F] = 2;
    g_Weapons[3].pad5E[0x38] = 5;
    g_Weapons[3].pad5E[0x3A] = 6;
    M2C_FIELD(g_Weapons, u32 *, 0x5C) = (s32) ((((M2C_FIELD(g_Weapons, u32 *, 0x5C) & sp8) | 0x8C0000) & sp4) | 0x08000000);
    g_Weapons->flags = ((((g_Weapons->flags | 4) & ~0x600) | 0x1800) & ~0x1F8) | 0x2008 | 0x80000 | 0x200000;
    g_Weapons[3].pad51[0xA] = D_8009D2EC;
    g_Weapons[3].flags = (g_Weapons[3].flags & 0xFFF87FFF) | sp0 | 0x800000;
    M2C_FIELD(g_Weapons, s16 *, 0x2DA) = 0x2EE0;
    M2C_FIELD(g_Weapons, s32 *, 0x2DC) = 0x14;
    M2C_FIELD(g_Weapons, s32 *, 0x228) = (s32) M2C_FIELD(&D_800A7C24, s32 *, 0);
    M2C_FIELD(g_Weapons, s32 *, 0x22C) = (s32) M2C_FIELD(&D_800A7C24, s32 *, 4);
    M2C_FIELD(g_Weapons, s32 *, 0x230) = (s32) M2C_FIELD(&D_800A7C24, s32 *, 8);
    M2C_FIELD(g_Weapons, s16 *, 0x2D6) = 7;
    M2C_FIELD(g_Weapons, s16 *, 0x2D4) = 0x1770;
    M2C_FIELD(g_Weapons, s16 *, 0x2D8) = 0x14;
    M2C_FIELD(g_Weapons, s8 *, 0x26E) = 6;
    M2C_FIELD(g_Weapons, s8 *, 0x26F) = 5;
    M2C_FIELD(g_Weapons, void (**)(), 0x264) = func_80038448;
    M2C_FIELD(g_Weapons, void (**)(), 0x268) = func_80069318;
    g_Weapons[3].ammo = 0x14;
    g_Weapons[3].clipSize = 0xA;
    g_Weapons[3].clip = 0xA;
    g_Weapons[3].reserve = 0xA;
    g_Weapons[3].pad4C[2] = 1;
    g_Weapons[3].pad4C[3] = 1;
    g_Weapons[3].pad51[1] = 2;
    M2C_FIELD(g_Weapons, s16 *, 0x27C) = 0x2328;
    M2C_FIELD(g_Weapons, s16 *, 0x27E) = 0x15E;
    M2C_FIELD(g_Weapons, s16 *, 0x28C) = 0x127;
    M2C_FIELD(g_Weapons, s16 *, 0x290) = 0x7D;
    M2C_FIELD(g_Weapons, s16 *, 0x288) = 0x3C;
    g_Weapons[3].pad51[2] = 0x20;
    g_Weapons[3].pad51[7] = 0x18;
    g_Weapons[3].maxAmmo = 0x1E;
    M2C_FIELD(g_Weapons, s16 *, 0x28A) = 0xC8;
    M2C_FIELD(g_Weapons, s16 *, 0x28E) = 0x144;
    M2C_FIELD(g_Weapons, s16 *, 0x294) = -0xA;
    M2C_FIELD(g_Weapons, u32 *, 0x284) = (s32) ((((M2C_FIELD(g_Weapons, u32 *, 0x284) & 0xFC00FFFF) | 0x02580000) & 0x03FFFFFF) | 0x10000000);
    g_Weapons[3].pad0[0x1C] = D_800A7C20;
    M2C_FIELD(g_Weapons, s16 *, 0x2D2) = 0x3AC;
    g_Weapons[3].pad51[9] = 0x31;
    g_Weapons[1].pad5E[0x20] = 4;
    g_Weapons[1].pad5E[0x17] = 5;
    g_Weapons[1].pad5E[0x18] = 6;
    g_Weapons[1].pad5E[0x21] = 4;
    g_Weapons[1].pad5E[0x19] = 1;
    g_Weapons[1].pad5E[0x1A] = -1U;
    g_Weapons[1].pad5E[0x1D] = 0;
    g_Weapons[1].pad5E[0x1E] = 3;
    g_Weapons[1].pad5E[0x1F] = 2;
    g_Weapons[1].pad5E[0x37] = -1U;
    g_Weapons[1].pad5E[0x38] = -1U;
    g_Weapons[1].pad5E[0x3A] = -1U;
    M2C_FIELD(g_Weapons, s16 *, 0x16A) = 0x2EE0;
    M2C_FIELD(g_Weapons, s32 *, 0x16C) = 0x14;
    g_Weapons[3].flags = (g_Weapons[3].flags & ~0x600) | 0x2200 | 0x80000 | 0x100000 | 0x200000;
    g_Weapons[1].pad51[0xA] = D_8009D2E0;
    g_Weapons[1].flags = (g_Weapons[1].flags & 0xFFF87FFF) | 0x38000 | 0x400000 | 0x800000;
    M2C_FIELD(g_Weapons, s32 *, 0xB8) = (s32) M2C_FIELD(&D_800A7C30, s32 *, 0);
    M2C_FIELD(g_Weapons, s32 *, 0xBC) = (s32) M2C_FIELD(&D_800A7C30, s32 *, 4);
    M2C_FIELD(g_Weapons, s16 *, 0xC0) = (s16) M2C_FIELD(&D_800A7C30, s16 *, 8);
    M2C_FIELD(g_Weapons, s16 *, 0x166) = 8;
    M2C_FIELD(g_Weapons, s16 *, 0x168) = 0x12;
    M2C_FIELD(g_Weapons, s8 *, 0xFE) = 6;
    M2C_FIELD(g_Weapons, s8 *, 0xFF) = 5;
    M2C_FIELD(g_Weapons, void (**)(), 0xF4) = func_80038448;
    M2C_FIELD(g_Weapons, void (**)(), 0xF8) = func_80069318;
    g_Weapons[1].ammo = 0x1E;
    g_Weapons[1].clipSize = 0x1E;
    g_Weapons[1].pad0[0x1C] = D_800A7C20;
    g_Weapons[1].clip = 0x1E;
    M2C_FIELD(g_Weapons, s16 *, 0x164) = 0x384;
    g_Weapons[1].maxAmmo = 0x96;
    M2C_FIELD(g_Weapons, s16 *, 0x11C) = 0x1E5;
    g_Weapons[1].pad51[2] = 0x20;
    g_Weapons[1].pad51[7] = 0x18;
    M2C_FIELD(g_Weapons, s16 *, 0x11E) = 0x168;
    M2C_FIELD(g_Weapons, s16 *, 0x10C) = 0xC8;
    g_Weapons[1].reserve = 0;
    g_Weapons[1].pad4C[2] = 1;
    g_Weapons[1].pad4C[3] = 1;
    g_Weapons[1].pad51[0] = 3;
    g_Weapons[1].pad51[1] = 2;
    M2C_FIELD(g_Weapons, s16 *, 0x10E) = 0x64;
    M2C_FIELD(g_Weapons, s16 *, 0x118) = 0x3C;
    M2C_FIELD(g_Weapons, s16 *, 0x11A) = 5;
    M2C_FIELD(g_Weapons, s16 *, 0x120) = -5;
    M2C_FIELD(g_Weapons, s16 *, 0x124) = 0xA;
    M2C_FIELD(g_Weapons, s16 *, 0x126) = -0xA;
    M2C_FIELD(g_Weapons, s16 *, 0x162) = 0x294;
    g_Weapons[1].pad51[9] = 0x30;
    g_Weapons[2].pad5E[0x17] = 4;
    g_Weapons[2].pad5E[0x18] = 5;
    g_Weapons[2].pad5E[0x20] = 3;
    g_Weapons[2].pad5E[0x21] = 3;
    g_Weapons[2].pad5E[0x19] = 1;
    g_Weapons[2].pad5E[0x1A] = -1U;
    g_Weapons[2].pad5E[0x1D] = 0;
    g_Weapons[2].pad5E[0x1E] = 2;
    g_Weapons[2].pad5E[0x1F] = 2;
    M2C_FIELD(g_Weapons, u32 *, 0x114) = (s32) ((((M2C_FIELD(g_Weapons, u32 *, 0x114) & 0xFC00FFFF) | 0xB40000) & 0x03FFFFFF) | 0x08000000);
    g_Weapons[1].flags = ((((g_Weapons[1].flags | 6) & ~0x600) | 0x200) & ~0x1F8) | 0x2028 | 0x80000 | 0x100000 | 0x200000;
    g_Weapons[2].pad5E[0x37] = -1U;
    M2C_FIELD(g_Weapons, s16 *, 0x222) = 0x2EE0;
    g_Weapons[2].pad5E[0x38] = -1U;
    g_Weapons[2].pad5E[0x3A] = -1U;
    M2C_FIELD(g_Weapons, s32 *, 0x224) = 0x14;
    g_Weapons[2].pad51[0xA] = D_8009D2DC;
    g_Weapons[2].flags = (g_Weapons[2].flags & 0xFFF87FFF) | sp0 | 0x400000 | 0x800000;
    M2C_FIELD(g_Weapons, s32 *, 0x170) = (s32) M2C_FIELD(&D_800A7C3C, s32 *, 0);
    M2C_FIELD(g_Weapons, s32 *, 0x174) = (s32) M2C_FIELD(&D_800A7C3C, s32 *, 4);
    M2C_FIELD(g_Weapons, s32 *, 0x178) = (s32) M2C_FIELD(&D_800A7C3C, s32 *, 8);
    M2C_FIELD(g_Weapons, s32 *, 0x17C) = (s32) M2C_FIELD(&D_800A7C3C, s32 *, 0xC);
    g_Weapons[2].pad0[0x10] = (u8) M2C_FIELD(&D_800A7C3C, s8 *, 0x10);
    M2C_FIELD(g_Weapons, s16 *, 0x220) = 0xF;
    M2C_FIELD(g_Weapons, s16 *, 0x21E) = -1;
    g_Weapons[2].pad0[0x1C] = D_800A7C20;
    M2C_FIELD(g_Weapons, s8 *, 0x1B6) = -1;
    M2C_FIELD(g_Weapons, void (**)(), 0x1AC) = func_800388E8;
    g_Weapons[2].reserve = 9;
    g_Weapons[2].maxAmmo = 0xA;
    M2C_FIELD(g_Weapons, s16 *, 0x21C) = 0x1388;
    g_Weapons[2].pad51[7] = 0x18;
    M2C_FIELD(g_Weapons, s8 *, 0x1B7) = 3;
    M2C_FIELD(g_Weapons, void (**)(), 0x1B0) = func_80069318;
    g_Weapons[2].ammo = 0xA;
    g_Weapons[2].clipSize = 1;
    g_Weapons[2].clip = 1;
    g_Weapons[2].pad4C[2] = 1;
    g_Weapons[2].pad4C[3] = 1;
    g_Weapons[2].pad51[0] = 3;
    g_Weapons[2].pad51[1] = 2;
    g_Weapons[2].pad51[2] = 0x20;
    M2C_FIELD(g_Weapons, s16 *, 0x1D0) = 0x3C;
    M2C_FIELD(g_Weapons, u32 *, 0x1CC) = (s32) ((((M2C_FIELD(g_Weapons, u32 *, 0x1CC) & 0xFC00FFFF) | 0x012C0000) & 0x03FFFFFF) | 0x08000000);
    M2C_FIELD(g_Weapons, s16 *, 0x1D2) = 0x12C;
    M2C_FIELD(g_Weapons, s16 *, 0x1D4) = 0x11D;
    M2C_FIELD(g_Weapons, s16 *, 0x1D6) = 0x136;
    M2C_FIELD(g_Weapons, s16 *, 0x1D8) = -5;
    M2C_FIELD(g_Weapons, s16 *, 0x1DC) = 0xA;
    M2C_FIELD(g_Weapons, s16 *, 0x1DE) = -0xA;
    M2C_FIELD(g_Weapons, s16 *, 0x21A) = 0x294;
    g_Weapons[5].pad5E[0x17] = 2;
    g_Weapons[5].pad5E[0x18] = -1U;
    g_Weapons[5].pad5E[0x20] = -1U;
    g_Weapons[5].pad5E[0x21] = -1U;
    g_Weapons[5].pad5E[0x19] = 1;
    g_Weapons[5].pad5E[0x1A] = -1U;
    g_Weapons[5].pad5E[0x1D] = 0;
    g_Weapons[5].pad5E[0x1E] = -1U;
    g_Weapons[5].pad5E[0x1F] = -1U;
    g_Weapons[5].pad5E[0x37] = -1U;
    g_Weapons[5].pad5E[0x38] = -1U;
    g_Weapons[5].pad5E[0x3A] = -1U;
    M2C_FIELD(g_Weapons, s16 *, 0x44A) = 0x2EE0;
    M2C_FIELD(g_Weapons, s32 *, 0x44C) = 0x14;
    g_Weapons[2].flags = (((g_Weapons[2].flags & ~0x600) | 0x1A00) & ~0x1F8) | 0x21C0 | 0x80000 | 0x100000;
    g_Weapons[5].pad51[0xA] = D_8009D2E8;
    M2C_FIELD(g_Weapons, s32 *, 0x398) = (s32) M2C_FIELD(&D_800A7C50, s32 *, 0);
    M2C_FIELD(g_Weapons, s32 *, 0x39C) = (s32) M2C_FIELD(&D_800A7C50, s32 *, 4);
    M2C_FIELD(g_Weapons, s16 *, 0x446) = -1;
    M2C_FIELD(g_Weapons, s16 *, 0x448) = -1;
    M2C_FIELD(g_Weapons, s8 *, 0x3DE) = -1;
    M2C_FIELD(g_Weapons, s8 *, 0x3DF) = -1;
    M2C_FIELD(g_Weapons, void (**)(), 0x3D4) = func_80038448;
    g_Weapons[5].pad0[0x1C] = D_800A7C20;
    M2C_FIELD(g_Weapons, void (**)(), 0x3D8) = func_80069318;
    g_Weapons[5].reserve = 0x1E;
    g_Weapons[5].ammo = 0x32;
    g_Weapons[5].clipSize = 0x14;
    g_Weapons[5].clip = 0x14;
    g_Weapons[5].pad4C[2] = 1;
    g_Weapons[5].pad4C[3] = 1;
    g_Weapons[5].pad51[0] = 3;
    g_Weapons[5].pad51[1] = 2;
    g_Weapons[5].pad51[2] = 0x20;
    g_Weapons[5].pad51[7] = 0x18;
    g_Weapons[5].maxAmmo = 0x64;
    M2C_FIELD(g_Weapons, s16 *, 0x3FC) = -0xF64;
    M2C_FIELD(g_Weapons, s16 *, 0x3FE) = 0x134C;
    M2C_FIELD(g_Weapons, s16 *, 0x400) = 0x64F;
    M2C_FIELD(g_Weapons, s16 *, 0x442) = 0x960;
    g_Weapons[4].pad5E[0x17] = 3;
    g_Weapons[4].pad5E[0x18] = 4;
    g_Weapons[4].pad5E[0x20] = -1U;
    g_Weapons[4].pad5E[0x21] = -1U;
    g_Weapons[4].pad5E[0x19] = 1;
    g_Weapons[4].pad5E[0x1A] = -1U;
    g_Weapons[4].pad5E[0x1D] = 0;
    g_Weapons[4].pad5E[0x1E] = 2;
    g_Weapons[4].pad5E[0x1F] = -1U;
    g_Weapons[4].pad5E[0x37] = -1U;
    g_Weapons[4].pad5E[0x38] = -1U;
    g_Weapons[4].pad5E[0x3A] = -1U;
    M2C_FIELD(g_Weapons, s16 *, 0x3F8) = 0x3C;
    M2C_FIELD(g_Weapons, s16 *, 0x3FA) = 0xA;
    temp_a0 = g_Weapons[5].flags & 0xF9FFFFFF;
    g_Weapons[5].flags = temp_a0;
    g_Weapons[5].flags = (((temp_a0 & ~0x600) | 0x200) & ~0x1F8) | 0x1C0;
    g_Weapons[4].pad51[0xA] = D_8009D318;
    M2C_FIELD(g_Weapons, s32 *, 0x394) = 4;
    M2C_FIELD(g_Weapons, s16 *, 0x392) = 0x3E8;
    M2C_FIELD(g_Weapons, s32 *, 0x2E0) = (s32) M2C_FIELD(&D_800A7C58, s32 *, 0);
    M2C_FIELD(g_Weapons, s32 *, 0x2E4) = (s32) M2C_FIELD(&D_800A7C58, s32 *, 4);
    M2C_FIELD(g_Weapons, void (**)(), 0x31C) = func_80038954;
    M2C_FIELD(g_Weapons, s16 *, 0x38E) = -1;
    M2C_FIELD(g_Weapons, s16 *, 0x390) = -1;
    M2C_FIELD(g_Weapons, s8 *, 0x326) = -1;
    M2C_FIELD(g_Weapons, s8 *, 0x327) = -1;
    M2C_FIELD(g_Weapons, void (**)(), 0x320) = func_80069318;
    g_Weapons[4].ammo = 0x32;
    g_Weapons[4].clipSize = 0x14;
    M2C_FIELD(g_Weapons, s16 *, 0x38C) = 0x7D0;
    M2C_FIELD(g_Weapons, s16 *, 0x344) = 0x16D;
    M2C_FIELD(g_Weapons, s16 *, 0x346) = 0x14A;
    M2C_FIELD(g_Weapons, s16 *, 0x348) = 0x311;
    M2C_FIELD(g_Weapons, s16 *, 0x34E) = -0x1E;
    g_Weapons[4].pad51[2] = 0x20;
    g_Weapons[4].pad51[7] = 0x18;
    g_Weapons[4].pad4C[2] = 1;
    g_Weapons[4].pad4C[3] = 1;
    g_Weapons[4].pad51[1] = 2;
    g_Weapons[4].maxAmmo = 0x64;
    M2C_FIELD(g_Weapons, s16 *, 0x340) = 0x3C;
    M2C_FIELD(g_Weapons, s16 *, 0x342) = 0xA;
    M2C_FIELD(g_Weapons, s16 *, 0x38A) = 0x474;
    temp_a0_2 = g_Weapons[4].flags & 0xF9FFFFFF;
    g_Weapons[4].pad0[0x1C] = D_800A7C20;
    g_Weapons[4].clip = (s16) g_Weapons[4].clipSize;
    g_Weapons[4].reserve = 0x32 - g_Weapons[4].clipSize;
    g_Weapons[4].flags = temp_a0_2;
    g_Weapons[4].flags = ((temp_a0_2 | 0x1E00) & ~0x1F8) | 0x1C0 | 0x08000000;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800335B4);
#endif

#ifdef NON_MATCHING
void func_80033F38(void) {
    AnimSlot *func_800378FC(u8 *obj, u8 *anim, s16 arg2, s32 arg3);
    u8 *fp;
    u8 *end;
    u8 *s3;
    u8 *s4;
    u8 *s1;
    u8 *s7;
    u8 *obj;
    u8 *tbl;
    s32 best;
    s32 i;
    s32 kind;
    s16 sp50;
    s16 sp28[3];
    u16 sp30[3];
    u8 c0[3];
    u8 c1[3];
    s32 v;

    PushMatrix();
    fp = (u8 *)D_800AFFC8 + D_800B0DA8 * 0x124;
    end = (u8 *)D_800AFFC8 + D_800B07B8 * 0x124;
    if (end >= fp) {
        s3 = fp + 4;
        for (; end >= fp; fp += 0x124, s3 += 0x124) {
            if (s3[0] == 0) continue;
            s4 = *(u8 **)(s3 + 0x108);
            best = -1;
            if (s4[0x64] == 0) {
                v = s4[0x66];
            } else {
                i = 0;
                sp50 = -1;
                kind = s3[3] & 0x3F;
                s7 = *(u8 **)(*(u8 **)(s3 + 0x84) + 0x90) + 0x28;
                s1 = s4;
                if (s4[0x64] != 0) {
                    do {
                        *(u16 *)(D_1F8003E0 + 0) = *(u16 *)(s1 + 0);
                        *(u16 *)(D_1F8003E0 + 2) = *(u16 *)(s1 + 2);
                        *(u16 *)(D_1F8003E0 + 4) = *(u16 *)(s1 + 4);
                        obj = *(u8 **)(s1 + 0xC);
                        if (obj != 0 && obj[5] != 6 && obj[4] != 0) {
                            v = *(s32 *)(obj + 0x4C);
                            obj = *(u8 **)(obj + 0x90) + *(s16 *)(s1 + 0x12) * 0x8C;
                            if (v & 0x100) {
                                if (*(void **)(s3 + 0x84) == g_Player) {
                                    GsGetLs(*(void **)(obj + 4), obj + 0x68);
                                }
                            }
                            gte_SetRotMatrix(obj + 0x34);
                            gte_ldv0(D_1F8003E0);
                            gte_rtv0();
                            gte_stsv(D_1F8003E0);
                            *(u16 *)(D_1F8003E0 + 0) += *(u16 *)(obj + 0x48);
                            *(u16 *)(D_1F8003E0 + 2) += *(u16 *)(obj + 0x4C);
                            *(u16 *)(D_1F8003E0 + 4) += *(u16 *)(obj + 0x50);
                        }
                        sp30[0] = *(u16 *)(s7 + 0);
                        sp30[1] = *(u16 *)(s3 + 0xA8);
                        sp30[2] = *(u16 *)(s7 + 8);
                        v = func_8006D3C0((SVECTOR *)D_1F8003E0, (SVECTOR *)sp30);
                        if (best == -1 || v < best) {
                            best = v;
                            sp50 = i;
                            sp28[0] = *(u16 *)(D_1F8003E0 + 0);
                            sp28[1] = *(u16 *)(D_1F8003E0 + 2);
                            sp28[2] = *(u16 *)(D_1F8003E0 + 4);
                        }
                        i++;
                        s1 += 0x14;
                    } while (i < s4[0x64]);
                }
                if (sp50 == -1) {
                    func_8006B230(0, &D_800A7C60);
                }
                if (kind == 40) {
                    func_8006B1DC((void *)&D_800A7C70);
                    continue;
                }
                if (kind == 4) {
                    func_8006B1DC((void *)&D_800A7C88);
                    continue;
                }
                if (kind == 46) {
                    func_80027248(0x32, (s32)(*(u8 **)(s3 + 0x8C) + 0x14), 0);
                    func_8007632C(fp, sp50);
                    continue;
                }
                if (kind == 44) {
                    func_80076F24(fp, sp50);
                    continue;
                }
                if (kind == 45) {
                    func_8006B1DC((void *)&D_800A7C9C);
                    continue;
                }
                s1 = s4 + sp50 * 0x14;
                obj = *(u8 **)(s1 + 0xC);
                if (obj != 0 && obj[4] != 0) {
                    if (obj[5] == 6) {
                        if (func_80034998(obj) != 0) {
                            func_80034A48(obj, fp, *(s16 *)(s1 + 0x10), *(s16 *)(s1 + 0x12));
                            if (obj != g_Player && D_800DDDA8 != 0 && D_8009CD4C != 0) {
                                func_80036670(fp, (u8 *)D_800DDDA8);
                            }
                        }
                        goto tail;
                    }
                    if (obj[5] == 1) {
                        *(s32 *)(obj + 0x34) = 0;
                    }
                    *(s32 *)(*(u8 **)(s3 + 0x8C) + 0x28) = sp28[0];
                    *(s32 *)(*(u8 **)(s3 + 0x8C) + 0x2C) = sp28[1];
                    *(s32 *)(*(u8 **)(s3 + 0x8C) + 0x30) = sp28[2];
                    D_800D4050[0] = *(u16 *)(s1 + 0);
                    D_800D4050[1] = *(u16 *)(s1 + 2);
                    D_800D4050[2] = *(u16 *)(s1 + 4);
                    D_800D4058[0] = *(u16 *)(s1 + 6);
                    D_800D4058[1] = *(u16 *)(s1 + 8);
                    D_800D4058[2] = *(u16 *)(s1 + 0xA);
                    if (!(*(s32 *)(s3 + 0x64) & 0x6000)) {
                        func_800373FC(fp, obj);
                    }
                } else {
                    v = *(s16 *)(s1 + 0);
                    *(s32 *)(*(u8 **)(s3 + 0x8C) + 0x28) = v;
                    D_1F8003D4 = v;
                    v = *(s16 *)(s1 + 2);
                    *(s32 *)(*(u8 **)(s3 + 0x8C) + 0x2C) = v;
                    D_1F8003D8 = v;
                    v = *(s16 *)(s1 + 4);
                    *(s32 *)(*(u8 **)(s3 + 0x8C) + 0x30) = v;
                    D_1F8003DC = v;
                    if ((*(u8 **)fp)[5] != 0) {
                        obj = (u8 *)func_80039A9C(0, (*(u8 **)fp)[5], 2);
                        if (obj != 0 && (*(u16 *)(s3 + 0xA) & 0x200)) {
                            *(AnimSlot **)(obj + 0x104) = func_800378FC(*(u8 **)(obj + 0x90) + 0x14, (u8 *)&D_800D31C0, *(s16 *)(obj + 0x2A), *(s32 *)(s3 + 0x84));
                        }
                    }
                    switch (*(s16 *)(s1 + 0x10)) {
                    case 0x200:
                        tbl = D_800AB5D4;
                        c0[0] = 0xFE; c0[1] = 0xA3; c0[2] = 0x22;
                        c1[0] = 0xFF; c1[1] = 0xF2; c1[2] = 0x8B;
                        break;
                    case 0x600:
                        tbl = D_800AB5DC;
                        c0[0] = 0x8A; c0[1] = 0x72; c0[2] = 0x58;
                        c1[0] = 0xFF; c1[1] = 0xEF; c1[2] = 0x65;
                        break;
                    case 0x500:
                        tbl = D_800AB5B8;
                        c0[0] = 0x5A; c0[1] = 0x1E; c0[2] = 9;
                        c1[0] = 0xDD; c1[1] = 0xC3; c1[2] = 0x59;
                        break;
                    case 0x100:
                        tbl = D_800AB5D8;
                        c0[0] = 0x86; c0[1] = 0xC6; c0[2] = 0xC6;
                        c1[0] = 0xEC; c1[1] = 0xFD; c1[2] = 0xD0;
                        break;
                    case 0x400:
                        tbl = D_800AB5D0;
                        c0[0] = 0x69; c0[1] = 0xCC; c0[2] = 0xD7;
                        c1[0] = 0xFF; c1[1] = 0xF8; c1[2] = 0xC1;
                        break;
                    case 0x300:
                        tbl = D_800AB5CC;
                        c0[0] = 0x58; c0[1] = 0x8A; c0[2] = 0x7F;
                        c1[0] = 0xD0; c1[1] = 0xFD; c1[2] = 0xD5;
                        break;
                    default:
                        tbl = D_800AB5BC;
                        c0[0] = 0xFC; c0[1] = 0x7E; c0[2] = 1;
                        c1[0] = 0xFC; c1[1] = 0xE2; c1[2] = 1;
                        break;
                    }
                    if (*(s32 *)(s3 + 0x64) & 0x2000) {
                        func_80026F30(0x63, 0);
                    } else if (tbl != 0 && (rand() & 1)) {
                        v = rand() & 1;
                        if (rand() & 1) {
                            v = -v;
                        }
                        *(s16 *)((u8 *)g_Args + 0x134) = v;
                        func_80027248(tbl[(rand() & 1) * 4], (s32)(*(u8 **)(s3 + 0x8C) + 0x14), 4);
                    }
                    if (*(s16 *)(s1 + 0x12) == 1 && *(void **)(s3 + 0x84) == g_Player && !(*(u16 *)(s3 + 0xA) & 0x100)) {
                        u8 ty;
                        D_1F8003E8[0] = *(s16 *)(s1 + 6);
                        D_1F8003E8[1] = *(s16 *)(s1 + 8);
                        D_1F8003E8[2] = *(s16 *)(s1 + 0xA);
                        if (*(s16 *)(s1 + 0x10) == 0x100) {
                            ty = 5;
                        } else if (*(s16 *)(s1 + 0x10) == 0x200) {
                            ty = 6;
                        } else {
                            ty = 4;
                        }
                        func_800633B0(ty, *(VECTOR *)D_1F8003E8);
                    }
                    if (s3[3] == 0x25) {
                        func_8006060C();
                        D_1F8003D8 -= 0xC8;
                        func_8006439C(D_1F8003D4, D_1F8003D8, D_1F8003DC, 6, 3, 1, 0x64, 8, 0);
                        if ((*(u8 **)fp)[0x5D] != 0) {
                            func_80027248((*(u8 **)fp)[0x5D], (s32)(*(u8 **)(s3 + 0x8C) + 0x14), 0);
                        }
                    } else if (*(s16 *)(s1 + 0x12) != 3 && !(*(s32 *)(s3 + 0x64) & 0x6000)) {
                        func_80063230();
                    }
                }
                if (!(*(u16 *)(s3 + 0xA) & 0x100)) {
                    func_80053DE4(3, 0, D_1F8003D4, D_1F8003D8, D_1F8003DC);
                }
tail:
                if (*(s32 *)(s3 + 0x60) & 0x800000) {
                    func_80027248(0x40, (s32)(*(u8 **)(s3 + 0x8C) + 0x14), 0);
                    func_80065614(fp, 1, 5);
                }
                v = s3[0];
            }
            if (v != 0) {
                func_800395E8(fp);
            }
        }
    }
    PopMatrix();
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80033F38);
#endif

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

s32 func_800349F8(void *a) {
    void *t;
    u32 f;
    if (M2C_FIELD(a, s32 *, 0x34) > 0) goto b;
ret1:
    return 1;
b:
    t = M2C_FIELD(a, void **, 0x94);
    if (t != 0 && (M2C_FIELD(t, s32 *, 0x24) & 0x20)) goto ret1;
    f = M2C_FIELD(a, u8 *, 0xA4) & 2;
    return f != 0;
}

#ifdef NON_MATCHING
void func_80034A48(u8 *arg0, u8 *arg1, s16 arg2, s16 arg3) {
    s16 sel;
    s32 isPlayerSrc, noArmor, dmg, a1, ang, d, t, code, cnt;
    u8 *w;
    s32 s6;

    sel = arg2;
    s6 = 0;
    if (W(arg1, u16, 0xE) & 0x200) {
        W(arg1, s32, 0x104) = (s32)func_800378FC(W(arg1, u8 *, 0x90) + 0x14, D_800D31C0, W(arg1, s16, 0x2A), W(arg1, s32, 0x88));
    }
    if (W(W(arg1, u8 *, 0), u8, 5) != 0) {
        u8 *pos = W(arg1, u8 *, 0x90) + 0x14;
        *(s32 *)0x1F8003D4 = W(pos, s32, 0x14);
        *(s32 *)0x1F8003D8 = W(pos, s32, 0x18);
        *(s32 *)0x1F8003DC = W(pos, s32, 0x1C);
        func_80039A9C(0, W(W(arg1, u8 *, 0), u8, 5), 2);
    }
    code = W(W(arg1, u8 *, 0), u8, 0x5D);
    if (code != 0) {
        func_80027248(code, (s32)(W(arg1, u8 *, 0x90) + 0x14), 0);
    }
    if (W(arg0, u16, 0xE) == 0x7FFF) return;
    isPlayerSrc = W(arg1, s32, 0x88) == (s32)g_Player;
    if ((void *)arg0 == g_Player && isPlayerSrc == 0) {
        t = func_80035030(arg1, arg0);
        D_8009CF30 = t;
        if (t == D_8009CF3C) {
            D_8009CF38 = D_8009CF38 + 1;
            if (D_8009CF38 >= 4) {
                d = t + 4;
                D_8009CF3C = -1;
                D_8009CF38 = 0;
                a1 = d;
                if (d < 0) a1 = t + 0xB;
                D_8009CF40 = ((d - ((a1 >> 3) * 8)) << 16) + 4;
            }
        } else {
            D_8009CF38 = 0;
        }
        W(arg0, s32, 0x64) = (W(arg0, s32, 0x64) & 0xFFE0FFFF) | ((sel & 0x1F) << 16);
        a1 = W(arg1, s32, 0x34);
        D_8009CF3C = D_8009CF30;
        if (((u8 *)g_Args)[0x12A] == 1) a1 = a1 * 2; else a1 = a1 * 3;
        func_8004B270(a1, 0, 0);
        func_8005E228(arg0);
        return;
    }
    noArmor = (W(arg1, u32, 0x68) >> 13) & 1;
    if (W(arg1, u8, 0x3A) == 8) {
        sel = 8;
        W(arg0, u8, 0xA4) |= 0x10;
    } else if (noArmor != 0) {
        func_80026F30(0x63, 0);
    } else {
        func_80027248(0x1D, (s32)(W(arg0, u8 *, 0x90) + 0x14), 0);
    }
    if (W(arg0, s32, 0x34) <= 0) return;
    if (isPlayerSrc != 0) {
        D_8009D8F0 = 4;
        D_800DE0D0 = arg3;
        if (D_8009CD4C != 0 || ((g_Args->flags2 >> 4) & 1)) D_800DDDA8 = arg0; else D_800DDDA8 = 0;
    } else {
        s6 = 1;
        if (!(W(W(arg0, u8 *, 0x118), s32, 0x10) & 0x200)) return;
    }
    if (noArmor != 0) {
        if (sel == 0xE) sel = 8;
    }
    W(arg0, s32, 0x64) = (W(arg0, s32, 0x64) & 0xFFE0FFFF) | ((sel & 0x1F) << 16);
    a1 = W(arg1, s32, 0x34);
    ang = (W(W(arg1, u8 *, 0x90), u16, 0x62) - W(W(arg0, u8 *, 0x90), u16, 0x62)) & 0xFFF;
    if (ang >= 0x801) ang |= 0xF000;
    W(arg0, s8, 0x3B) = 0;
    if ((u32)((ang + 0x5FF) & 0xFFFF) < 0xBFF) W(arg0, s8, 0x3B) = 1;
    if (sel == 0xE) {
        if (W(arg0, u8, 0xA) & 2) {
            w = W(arg0, u8 *, 0);
            cnt = W(w, u8, 0x47);
            if (cnt <= 0) cnt = 1;
            a1 = W(w, s32, 0x20) / cnt;
        } else {
            a1 = W(W(arg0, u8 *, 0), s32, 0x20);
        }
        if (s6 != 0) a1 = W(W(arg0, u8 *, 0), s32, 0x20) >> 2;
    } else {
        if (sel == 7 && (W(arg0, s32, 0x48) & 0x02000000)) {
            a1 = a1 >> 3;
        } else if (((sel == 8 || sel == 0xB) && (W(arg0, s32, 0x48) & 0x02000000)) || sel == 3 || sel == 6) {
            a1 = a1 >> 2;
        } else if (sel == 2 || sel == 5 || sel == 9 || sel == 0xC || sel == 0xA || sel == 0xD) {
            a1 = a1 >> 1;
        }
    }
    if (isPlayerSrc != 0 && ((g_Args->flags2 >> 13) & 1)) a1 *= 4;
    if (W(arg1, u32, 0x68) & 0x4000) a1 = W(arg1, s32, 0x34);
    W(arg0, s32, 0x34) = W(arg0, s32, 0x34) - a1;
    if (isPlayerSrc != 0) {
        W(g_Args, s32, 0x2CC) = W(g_Args, s32, 0x2CC) + 1;
        if (W(arg0, s32, 0x34) <= 0) {
            if (sel == 0xE) W(g_Args, u16, 0x2C6) = W(g_Args, u16, 0x2C6) + 1;
            W(g_Args, u16, 0x2C4) = W(g_Args, u16, 0x2C4) + 1;
            D_800DDDC0 = 1;
            if (W(arg1, u32, 0x68) & 0x2000) W(arg0, u8, 0xA4) |= 0x20;
        } else {
            D_800DDDC0 = 0;
        }
    }
    func_8005E020(arg0);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80034A48);
#endif

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

#ifdef NON_MATCHING
s32 func_800352B8(void) {
    u8 *o, *oEnd, *s, *sEnd, *sStart, *pos, *w, *row, *pl;
    s32 x;

    D_800F3B48 = 0;
    if (D_8009CA9C != 0) return D_8009CA9C;
    if (D_800B0B9C == 0x63) return D_800B0DA8;
    o = (u8 *)D_800AFFC8 + D_800B0DA8 * 0x124;
    oEnd = (u8 *)D_800AFFC8 + D_800B07B8 * 0x124;
    sStart = (u8 *)D_800AFFC8 + D_800B0DC0 * 0x124;
    sEnd = (u8 *)D_800AFFC8 + D_800B07A8 * 0x124;
    if ((u32)oEnd < (u32)o) return 0;
    do {
        if (W(o, u8, 4) != 0) {
            pos = W(o, u8 *, 0x90);
            if ((u32)(W(pos, s32, 0x28) + 0x7FBC) <= 0xFF78U && W(pos, s32, 0x30) < 0x7FBD && W(pos, s32, 0x30) >= -0x7FBC && W(pos, s32, 0x2C) < 0x7FBD && W(pos, s32, 0x2C) >= -0x7FBC) {
                func_800350F0(o);
                if (!(W(o, s32, 0x4C) & 0x02000000)) {
                    func_800359EC(o);
                    if (W(o, s32, 0x88) != 0) {
                        if (D_800F3B48 < 0x20) {
                            x = W(pos, s32, 0x28);
                            row = D_800F2FBC + D_800F3B48 * 0x44;
                            D_800F3B48 += 1;
                            W(row, s32, 0) = x;
                            W(row, s32, 4) = W(pos, s32, 0x2C);
                            W(row, s32, 8) = W(pos, s32, 0x30);
                            W(row, s32, 0x10) = W(pos, s32, 0x28) + W(o, s16, 0x98);
                            W(row, s32, 0x14) = W(pos, s32, 0x2C) + W(o, s16, 0x9A);
                            W(row, s32, 0x18) = W(pos, s32, 0x30) + W(o, s16, 0x9C);
                            if (W(W(o, u8 *, 0x10C), s16, 0x6E) != 0) {
                                W(row, s32, 0) = x - W(W(o, u8 *, 0x10C), s16, 0x68);
                                W(row, s32, 4) = W(row, s32, 4) - W(W(o, u8 *, 0x10C), s16, 0x6A);
                                W(row, s32, 8) = W(row, s32, 8) - W(W(o, u8 *, 0x10C), s16, 0x6C);
                                W(row, s32, 0x10) = W(row, s32, 0x10) + W(o, s16, 0x3C);
                                W(row, s32, 0x14) = W(row, s32, 0x14) + W(o, s16, 0x3E);
                                W(row, s32, 0x18) = W(row, s32, 0x18) + W(o, s16, 0x40);
                            }
                            W(row, u32, 0x3C) = (u32)o;
                            W(row, s32, 0x38) = W(o, s32, 0x34);
                            W(row, s16, 0x40) = W(W(o, u8 *, 0x90), u16, 0x62) & 0xFFF;
                        } else {
                            func_8006B1DC(D_800A7CF0);
                        }
                    }
                    pl = (u8 *)g_Player;
                    w = W(o, u8 *, 0x10C);
                    if (!(W(pl, u8, 0xA4) & 2) && W(o, s32, 0x88) != (s32)pl && func_8003CC8C(pl, o) != 0) {
                        if (W(w, u8, 0x64) < 5U) {
                            W(w + W(w, u8, 0x64) * 0x14, u8 *, 0xC) = pl;
                            W(w + W(w, u8, 0x64) * 0x14, s16, 0x12) = W(pl, u16, 0x66) & 0x1F;
                            W(w + W(w, u8, 0x64) * 0x14, u16, 0) = W(&g_Camera, u16, 0x48);
                            W(w + W(w, u8, 0x64) * 0x14, u16, 2) = W(&g_Camera, u16, 0x4C);
                            W(w + W(w, u8, 0x64) * 0x14, u16, 4) = W(&g_Camera, u16, 0x50);
                            W(w + W(w, u8, 0x64) * 0x14, u16, 6) = D_800D4058[0];
                            W(w + W(w, u8, 0x64) * 0x14, u16, 8) = D_800D4058[1];
                            W(w + W(w, u8, 0x64) * 0x14, u16, 0xA) = D_800D4058[2];
                            W(w, u8, 0x64) = W(w, u8, 0x64) + 1;
                        } else {
                            func_8006B1DC(D_800A7D08);
                        }
                    }
                    for (s = sStart; (u32)s <= (u32)sEnd; s += 0x124) {
                        if (W(s, u8, 4) != 0 && (W(s, s32, 0x4C) & 0x400) == 0 && W(o, s32, 0x88) != (s32)s && (!(W(s, s32, 0x4C) & 0x80) || W(o, s32, 0x88) == (s32)g_Player) && func_8003CC8C(s, o) != 0) {
                            if (W(w, u8, 0x64) < 5U) {
                                W(w + W(w, u8, 0x64) * 0x14, u32, 0xC) = (u32)s;
                                W(w + W(w, u8, 0x64) * 0x14, s16, 0x12) = W(s, u16, 0x66) & 0x1F;
                                W(w + W(w, u8, 0x64) * 0x14, u16, 0) = D_800D4050[0];
                                W(w + W(w, u8, 0x64) * 0x14, u16, 2) = D_800D4050[1];
                                W(w + W(w, u8, 0x64) * 0x14, u16, 4) = D_800D4050[2];
                                W(w + W(w, u8, 0x64) * 0x14, u16, 6) = D_800D4058[0];
                                W(w + W(w, u8, 0x64) * 0x14, u16, 8) = D_800D4058[1];
                                W(w + W(w, u8, 0x64) * 0x14, u16, 0xA) = D_800D4058[2];
                                W(w, u8, 0x64) = W(w, u8, 0x64) + 1;
                            } else {
                                func_8006B1DC(D_800A7D20);
                            }
                        }
                    }
                }
                func_8003AC1C(o, D_800B8F24);
                W(W(o, u8 *, 0x10C), s16, 0x6E) = 0;
                if (func_80035964(o) != 0) func_800395E8(o);
            } else {
                func_800395E8(o);
            }
        }
        o += 0x124;
    } while ((u32)oEnd >= (u32)o);
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800352B8);
#endif

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

void func_80035DE8(void) {
    EntHH *e;
    EntHH *end;
    u8 *a1;
    s32 *a0;
    if (D_8009CA9C == 0 && D_800B0B9C != 0x63) {
        e = (EntHH *)((u8 *)D_800AFFC8 + D_800B0DA8 * 0x124);
        end = (EntHH *)((u8 *)D_800AFFC8 + D_800B07B8 * 0x124);
        if (!((u32)end < (u32)e)) {
            do {
                if (e->active != 0) {
                    a1 = e->obj;
                    a0 = (s32 *)(a1 + 0x28);
                    *(s32 *)(a1 + 0x28) += e->d[0];
                    a0[1] += e->d[1];
                    a0[2] += e->d[2];
                    *(u16 *)(a1 + 0x60) += e->d[3];
                    *(u16 *)(a1 + 0x62) += e->d[4];
                    *(u16 *)(a1 + 0x64) += e->d[5];
                    *(s32 *)(a1 + 0x10) = 2;
                }
                e = (EntHH *)((u8 *)e + 0x124);
            } while (!((u32)end < (u32)e));
        }
    }
}

void func_80035F04(void) {
    u8 *obj;
    u8 *end;
    u8 *e;
    MATRIX *m;
    u8 *cam;
    u16 *a;
    s32 t;
    s16 sx;
    s16 cx;
    s16 sy;
    s16 cy;
    s32 sz;
    s32 cz;
    s32 t1;
    s32 t2;

    end = D_800B0AB4 + D_800B0C84 * 0x34;
    for (obj = D_800B0AB4; obj < end; obj += 0x34) {
        if (obj[1] == 0) {
            continue;
        }
        if (*(s16 *)(obj + 0xE) != 0 && (u32)D_8009CDA8 >= *(u32 *)(obj + 0x18) + *(s16 *)(obj + 0xE)) {
            obj[1] = 0;
            continue;
        }
        e = *(u8 **)(obj + 0x1C);
        if (*(s32 *)(obj + 0x20) != 0) {
            func_8003EB00(obj);
        }
        if (*(s32 *)(obj + 0x10) != 0) {
            cam = (u8 *)&g_Camera;
            *(u16 *)(e + 0x60) = *(u16 *)(cam + 0x80);
            *(u16 *)(e + 0x62) = *(u16 *)(cam + 0x82);
        }
        a = (u16 *)(e + 0x60);
        t = D_800A0370[a[0] & 0xFFF];
        cx = t >> 16;
        sx = t;
        t = D_800A0370[a[1] & 0xFFF];
        sy = t;
        cy = t >> 16;
        m = (MATRIX *)(e + 0x14);
        t = D_800A0370[a[2] & 0xFFF];
        m->m[2][2] = (cx * cy) >> 12;
        sz = t;
        cz = t >> 16;
        m->m[1][2] = -sx;
        m->m[0][2] = (cx * sy) >> 12;
        t1 = ((s16)sz * sx) >> 12;
        m->m[2][0] = (t1 * cy - cz * sy) >> 12;
        m->m[0][0] = (t1 * sy + cz * cy) >> 12;
        m->m[1][0] = ((s16)sz * cx) >> 12;
        t2 = (cz * sx) >> 12;
        m->m[2][1] = (t2 * cy + (s16)sz * sy) >> 12;
        m->m[0][1] = (t2 * sy - (s16)sz * cy) >> 12;
        m->m[1][1] = (cz * cx) >> 12;
        *(s32 *)(e + 0x10) = 0;
    }
}

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

void func_80036328(void) {
    u8 *obj;
    u8 *end;
    s32 j;
    s32 n;
    u8 *e;
    s32 a;
    s32 b;
    s16 sx;
    s16 cx;
    s16 sy;
    s16 cy;
    s16 sz;
    s16 cz;
    s32 t1;
    s32 t2;
    s16 *p;

    end = (u8 *)D_800AFFC8 + D_800B0AA8 * 0x124;
    for (obj = (u8 *)D_800AFFC8; obj < end; obj += 0x124) {
        if (obj[4] != 0) {
            e = *(u8 **)(obj + 0x90);
            n = obj[0xB];
            for (j = 0; j < n; j++, e += 0x8C) {
                if (*(s32 *)(e + 0x10) == 2) {
                    a = D_800A0370[*(u16 *)(e + 0x60) & 0xFFF];
                    cx = a >> 16;
                    sx = a;
                    a = D_800A0370[*(u16 *)(e + 0x62) & 0xFFF];
                    sy = a;
                    cy = a >> 16;
                    a = D_800A0370[*(u16 *)(e + 0x64) & 0xFFF];
                    cz = a >> 16;
                    sz = a;
                    t1 = (sz * sx) >> 12;
                    t2 = (cz * sx) >> 12;
                    *(s32 *)(e + 0x88) |= 1;
                    *(s16 *)(e + 0x24) = (cx * cy) >> 12;
                    *(s16 *)(e + 0x18) = (cx * sy) >> 12;
                    *(s16 *)(e + 0x20) = (t1 * cy - cz * sy) >> 12;
                    *(s16 *)(e + 0x1E) = -sx;
                    *(s32 *)(e + 0x10) = 0;
                    *(s16 *)(e + 0x14) = (t1 * sy + cz * cy) >> 12;
                    *(s16 *)(e + 0x1A) = (sz * cx) >> 12;
                    *(s16 *)(e + 0x22) = (t2 * cy + sz * sy) >> 12;
                    *(s16 *)(e + 0x16) = (t2 * sy - sz * cy) >> 12;
                    *(s16 *)(e + 0x1C) = (cz * cx) >> 12;
                } else if (*(s32 *)(e + 0x58) == 0) {
                    *(s32 *)(e + 0x10) = 0;
                }
            }
        }
    }
}

s32 func_800365B4(s32 a) {
  s16 idx;
  s16 step;
  s16 n;
  s32 *tbl;
  s32 neg = 0;
  if (((s16) a) >= (-0x1000))
  {
    if (((s16) a) > 0x1000)
    {
      a = 0x1000;
    }
  }
  else
  {
    a = -0x1000;
  }
  if (((s16) a) < 0)
  {
    a = -a;
    neg = 1;
  }
  idx = 0;
  step = 0x200;
  n = 0;
  tbl = D_800A0370;
  do
  {
    if (((s16 *) (&tbl[idx + step]))[1] >= ((s16) a))
    {
      idx += step;
    }
    step >>= 1;
  }
  while ((++n) < 10);
  if ((u8) neg)
  {
    idx = 0x800 - idx;
  }
  return idx & 0xFFF;
}

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

void func_80036938(void) {
    VECTOR tgt;
    VECTOR cam;
    VECTOR off;
    s16 i;
    s16 end;
    s16 yaw;
    s16 pitch;
    s32 maxYaw;
    s32 maxPitch;
    u8 *parts;
    u8 *obj;
    s32 dx;
    s32 dz;
    s32 yawd;
    s32 pitchd;
    s32 d;

    D_800DDE30 = 0;
    parts = M2C_FIELD(g_Player, u8 **, 0x90);
    M2C_FIELD(g_Player, s32 *, 0x5C) = 0;
    D_800D31BC = -1;
    if (M2C_FIELD(M2C_FIELD(g_Player, u8 **, 0x94), s32 *, 0x24) & 0x200) {
        maxYaw = 0xFA;
        maxPitch = 0x400;
    } else {
        maxYaw = 0x72;
        maxPitch = 0xFA;
    }
    if (D_800DDD90 != 0 || D_800DDD8C != 0 || D_800DDC44 != 0 || D_800DDCC0 != 0) {
        maxYaw = 0x5B;
    }
    if (D_8009CD4C == 0 && ((g_Args->flags2 >> 4) & 1) && D_800B0DD4 != 3 && D_800B0DD4 != 2) {
        pitch = *(s16 *)(parts + 0x60);
        yaw = *(s16 *)(parts + 0x62);
        cam.vx = g_Camera.posX;
        cam.vy = g_Camera.posY;
        cam.vz = g_Camera.posZ;
        end = D_800B07B0;
        obj = (u8 *)D_800AFFC8 + D_800B0DA0 * 0x124;
        i = D_800B0DA0;
        for (; i <= end; i++, obj += 0x124) {
            if (obj[4] == 0) {
                continue;
            }
            if (obj[0xA] & 1) {
                continue;
            }
            if (obj[0xA6] == 0) {
                continue;
            }
            if (*(s32 *)(obj + 0x34) <= 0) {
                continue;
            }
            if (obj[0xA4] & 2) {
                continue;
            }
            func_8005ADD4(*(void **)(obj + 0xC0), 7, (s32 *)&off);
            tgt.vx = *(s32 *)(*(u8 **)(obj + 0x90) + 0x28) + off.vx;
            tgt.vy = *(s32 *)(*(u8 **)(obj + 0x90) + 0x2C) + off.vy;
            tgt.vz = *(s32 *)(*(u8 **)(obj + 0x90) + 0x30) + off.vz;
            D_1F8003E8[0] = tgt.vx - cam.vx;
            D_1F8003E8[1] = tgt.vy - cam.vy;
            D_1F8003E8[2] = tgt.vz - cam.vz;
            D_8009CF50 = abs(D_1F8003E8[0]) + abs(D_1F8003E8[2]);
            if (D_8009CF50 > 39999) {
                continue;
            }
            D_8009CF50 = SquareRoot0(D_1F8003E8[0] * D_1F8003E8[0] + D_1F8003E8[2] * D_1F8003E8[2]);
            if (D_8009CF50 > 10000) {
                continue;
            }
            yawd = (((s32 (*)(s32, s32))ratan2)(D_1F8003E8[0], D_1F8003E8[2]) - yaw) & 0xFFF;
            if (yawd > 0x800) {
                yawd -= 0x1000;
            }
            pitchd = (((s32 (*)(s32, s32))ratan2)(-D_1F8003E8[1], D_8009CF50) - pitch) & 0xFFF;
            if (pitchd > 0x800) {
                pitchd -= 0x1000;
            }
            if (abs(pitchd) > maxPitch) {
                continue;
            }
            if (abs(yawd) > maxYaw) {
                continue;
            }
            d = D_8009CF50 >> 4;
            if (D_800D31BC >= 0 && d >= D_800D31BC) {
                continue;
            }
            func_8005ADD4(*(void **)(obj + 0xC0), 7, (s32 *)&off);
            tgt.vx = *(s32 *)(*(u8 **)(obj + 0x90) + 0x28) + off.vx;
            tgt.vy = *(s32 *)(*(u8 **)(obj + 0x90) + 0x2C) + off.vy;
            tgt.vz = *(s32 *)(*(u8 **)(obj + 0x90) + 0x30) + off.vz;
            if (func_80050A10(&cam, &tgt, obj, g_Player)) {
                D_800DDE30 = (s32)obj;
                M2C_FIELD(g_Player, u8 **, 0x5C) = obj;
                D_800D31BC = d;
                D_800DDDDC[0] = tgt.vx;
                D_800DDDDC[1] = tgt.vy;
                D_800DDDDC[2] = tgt.vz;
                M2C_FIELD(g_Player, s16 *, 0xC4) = yawd;
                M2C_FIELD(g_Player, s16 *, 0xC6) = pitchd;
                M2C_FIELD(g_Player, s32 *, 0x110) = D_8009CF50;
            }
        }
    }
    if (M2C_FIELD(g_Player, s32 *, 0x5C) == 0) {
        M2C_FIELD(g_Player, s16 *, 0xC4) = 0;
        M2C_FIELD(g_Player, s16 *, 0xC6) = 0;
        M2C_FIELD(g_Player, s32 *, 0x110) = 0;
    }
    func_80049A88();
}

void func_80036E10(u8 *a, u8 *b) {
    VECTOR sq;
    VECTOR pa;
    VECTOR pb;
    s32 *p;
    s32 *q;
    s32 yaw;
    u8 *m;
    u8 *bm;
    s32 ax;
    s32 az;
    s32 d;
    s16 yawd;
    s32 t;
    s32 pitch;

    bm = *(u8 **)(b + 0x90);
    yaw = *(u16 *)(bm + 0x62);
    p = (s32 *)(*(u8 **)(a + 0x90) + 0x28);
    pa.vx = p[0];
    pa.vy = p[1];
    pa.vz = p[2];
    q = (s32 *)(bm + 0x28);
    pb.vx = q[0];
    pb.vy = q[1];
    pb.vz = q[2];
    if (a[5] == 6) {
        pa.vy += *(s16 *)(*(u8 **)(a + 0x94) + 0xE);
        if (a == g_Player) {
            Camera *cam = &g_Camera;
            pa.vy = cam->posY;
            if (D_8009CDA8 & 1) {
                pa.vx = *(s32 *)(*(u8 **)(a + 0x90) + 0x28);
                pa.vz = *(s32 *)(*(u8 **)(a + 0x90) + 0x30);
            } else {
                pa.vx = cam->posX;
                pa.vz = cam->posZ;
            }
        }
    } else {
        pa.vy += (*(s16 *)(a + 0x22) + *(s16 *)(a + 0x20)) >> 1;
    }
    if (b[5] == 6) {
        pb.vy += *(s16 *)(*(u8 **)(b + 0x94) + 0xE);
    }
    if (b[0xA6] == 0 || b[5] == 6 || b[5] == 1) {
        D_1F8003E8[0] = pa.vx - pb.vx;
        D_1F8003E8[1] = pa.vy - pb.vy;
        D_1F8003E8[2] = pa.vz - pb.vz;
    } else if (b[0xD] != 0) {
        *(SVECTOR *)D_1F8003E0 = *(SVECTOR *)(b + 0x3C);
        m = *(u8 **)(b + 0x90) + b[0xD] * 0x8C;
        gte_SetRotMatrix(m + 0x34);
        gte_ldv0(D_1F8003E0);
        gte_rtv0();
        p = (s32 *)(m + 0x48);
        gte_stlvnl(D_1F8003E8);
        D_1F8003E8[0] = pa.vx - p[0] - D_1F8003E8[0];
        D_1F8003E8[1] = pa.vy - p[1] - D_1F8003E8[1];
        D_1F8003E8[2] = pa.vz - p[2] - D_1F8003E8[2];
    } else {
        *(SVECTOR *)D_1F8003E0 = *(SVECTOR *)(b + 0x3C);
        m = *(u8 **)(b + 0x90);
        gte_SetRotMatrix(m + 0x14);
        gte_ldv0(D_1F8003E0);
        gte_rtv0();
        gte_stlvnl(D_1F8003E8);
        D_1F8003E8[0] = pa.vx - pb.vx - D_1F8003E8[0];
        D_1F8003E8[1] = pa.vy - pb.vy - D_1F8003E8[1];
        D_1F8003E8[2] = pa.vz - pb.vz - D_1F8003E8[2];
    }
    D_8009CF4C = d = (ax = abs(D_1F8003E8[0])) + abs(D_1F8003E8[1]) + (az = abs(D_1F8003E8[2]));
    if (d < 60000) {
        gte_ldlvl(D_1F8003E8);
        gte_sqr0();
        gte_stlvnl(&sq);
        D_8009CF4C = SquareRoot0(sq.vx + sq.vy + sq.vz);
        D_8009CF50 = SquareRoot0(sq.vx + sq.vz);
    } else {
        D_8009CF50 = ax + az;
    }
    D_800B0084 = ((s32 (*)(s32, s32))ratan2)(D_1F8003E8[0], D_1F8003E8[2]);
    D_800B0D50 = pitch = ((s32 (*)(s32, s32))ratan2)(-D_1F8003E8[1], D_8009CF50);
    t = (*(u16 *)&D_800B0084 - yaw) & 0xFFF;
    yawd = t;
    if (b[5] == 6) {
        yawd = (t - *(u16 *)(*(u8 **)(b + 0x94) + 0xC)) & 0xFFF;
    }
    if (yawd > 0x800) {
        yawd -= 0x1000;
    }
    D_800B0D50 &= 0xFFF;
    if (D_800B0D50 > 0x800) {
        D_800B0D50 -= 0x1000;
    }
    M2C_FIELD(b, s16 *, 0xC4) = yawd;
    M2C_FIELD(b, s16 *, 0xC6) = *(u16 *)&D_800B0D50;
    M2C_FIELD(b, s32 *, 0x110) = D_8009CF50;
}

void func_8003732C(u8 *arg0, u8 *arg1) {
    u8 id;
    s32 r;
    id = arg1[0x5E];
    if (id != 0) {
        if (arg1[0x66] != 0 && (rand() & 1)) {
            id = arg1[0x66];
        }
        g_Args->pad12E[0xA] = 0x7F;
        if (rand() & 1) {
            r = rand() & 1;
            if (rand() & 1) {
                r = -r;
            }
            *(s16 *)((u8 *)g_Args + 0x134) = r;
            if (id != 0x27) {
                func_80027248(id, *(s32 *)(arg0 + 0x90) + 0x14, 6);
            }
        }
    }
}

void func_800373FC(u8 *proj, u8 *obj) {
    AnimSlot *func_800378FC(u8 *obj, u8 *anim, s16 arg2, s32 arg3);
    s32 kind;
    s32 *v;
    u8 c1[8];
    u8 c2[8];

    if (proj != NULL) {
        v = (s32 *)(M2C_FIELD(proj, u8 **, 0x90) + 0x28);
        D_1F8003D4 = v[0];
        D_1F8003D8 = v[1];
        D_1F8003DC = v[2];
        if (M2C_FIELD(proj, void **, 0x88) == g_Player) {
            kind = M2C_FIELD(obj, s32 *, 0x4C) & 0x900;
            if (kind == 0x100 && !(M2C_FIELD(proj, u16 *, 0xE) & 0x100)) {
                switch (((s32 (*)(u8))func_800698C4)(M2C_FIELD(obj, u8 *, 6))) {
                case 0x100:
                    func_800632AC(obj, 5);
                    break;
                case 0x200:
                    func_800632AC(obj, 6);
                    break;
                default:
                    func_800632AC(obj, 4);
                    break;
                }
            }
        }
        if (M2C_FIELD(M2C_FIELD(proj, u8 **, 0), u8 *, 0x5D) != 0) {
            func_80027248(M2C_FIELD(M2C_FIELD(proj, u8 **, 0), u8 *, 0x5D), (s32)(M2C_FIELD(proj, u8 **, 0x90) + 0x14), 0);
        }
        if (g_Invincible != 0 && obj == g_Player) {
            goto end;
        }
        if (func_80037768(obj, proj) != 0) {
            M2C_FIELD(obj, s32 *, 0x34) -= M2C_FIELD(proj, s32 *, 0x34);
            if (M2C_FIELD(obj, u8 *, 6) == D_8009D334 || M2C_FIELD(obj, u8 *, 6) == D_8009D2FC) {
                M2C_FIELD(g_Args, s32 *, 0x2CC)++;
            }
            if (obj == g_Player && (M2C_FIELD(g_Args, u8 *, 4) >> 7)) {
                func_8002F564(obj, 0xC8, 2, 0, 0);
            }
        }
    }
    if (M2C_FIELD(obj, u8 *, 5) == 1) {
        M2C_FIELD(obj, s32 *, 0x34) = 0;
    }
    if (M2C_FIELD(obj, s32 *, 0x34) <= 0 && !(M2C_FIELD(obj, u8 *, 0xA4) & 2) && M2C_FIELD(obj, u16 *, 0xE) != 0x7FFF) {
        M2C_FIELD(obj, u8 *, 0xA4) = 2;
        if (M2C_FIELD(obj, u16 *, 0xE) & 0x200) {
            if (proj != NULL) {
                M2C_FIELD(obj, AnimSlot **, 0x104) = func_800378FC(M2C_FIELD(obj, u8 **, 0x90) + 0x14, (u8 *)&D_800D31C0, M2C_FIELD(obj, s16 *, 0x2A), M2C_FIELD(proj, s32 *, 0x88));
            } else {
                M2C_FIELD(obj, AnimSlot **, 0x104) = func_800378FC(M2C_FIELD(obj, u8 **, 0x90) + 0x14, (u8 *)&D_800D31C0, M2C_FIELD(obj, s16 *, 0x2A), 0);
            }
        }
        func_80038C8C(obj);
    } else {
        if (proj == NULL) {
            return;
        }
        if (!(M2C_FIELD(proj, s32 *, 0x68) & 0x2000)) {
            c1[0] = 0xFC;
            c1[1] = 0x7E;
            c1[2] = 1;
            c2[0] = 0xFC;
            c2[1] = 0xE2;
            c2[2] = 1;
            func_80062F38(c1, c2);
        }
        if ((u32)(D_8009CDA8 - M2C_FIELD(obj, s32 *, 0x30)) > 10) {
            if (!(M2C_FIELD(proj, u16 *, 0xE) & 0x200) && !(M2C_FIELD(proj, s32 *, 0x68) & 0x2000)) {
                func_8003732C(obj, M2C_FIELD(obj, s32 *, 0));
            }
            M2C_FIELD(obj, s32 *, 0x30) = D_8009CDA8;
        }
        if (M2C_FIELD(proj, u16 *, 0xE) & 0x200) {
            M2C_FIELD(proj, AnimSlot **, 0x104) = func_800378FC(M2C_FIELD(proj, u8 **, 0x90) + 0x14, (u8 *)&D_800D31C0, M2C_FIELD(proj, s16 *, 0x2A), M2C_FIELD(proj, s32 *, 0x88));
        }
    }
    if (proj != NULL) {
end:
        func_800395E8(proj);
    }
}

s16 func_80037768(u8 *obj, u8 *proj) {
    u16 f;

    f = M2C_FIELD(obj, u16 *, 0xE);
    if (f == 0x7FFF) {
fail:
        return 0;
    }
    if (f == 0) {
        return 1;
    }
    if ((f & 0x400) && M2C_FIELD(proj, void **, 0x88) != g_Player) {
        goto fail;
    }
    if ((*(volatile u16 *)(obj + 0xE) & 0x800) && M2C_FIELD(proj, void **, 0x88) == g_Player) {
        goto fail;
    }
    return (M2C_FIELD(proj, u8 *, 0x3A) & M2C_FIELD(obj, u16 *, 0xE)) == 0;
}

s32 func_800377F4(u8 *arg0, u8 *arg1) {
    if (M2C_FIELD(arg0, u16 *, 0xE) != 0x7FFF) {
        if (M2C_FIELD(arg0, s32 *, 0x34) <= 0) return 0;
        if (arg0[0xA4] & 2) return 0;
        if (M2C_FIELD(arg0, s32 *, 0x4C) & 0x400) return 0;
        if ((arg0[7] & 0x3F) == 0xE) return 0;
        if (func_80037B6C(arg0) != 0) {
            D_1F8003D4 = M2C_FIELD(arg1, s32 *, 0x14);
            D_1F8003D8 = M2C_FIELD(arg1, s32 *, 0x18);
            D_1F8003DC = M2C_FIELD(arg1, s32 *, 0x1C);
            if (arg0[5] == 6) {
                if (arg0[0x3B] != 2) {
                    arg0[0x3B] = 5;
                }
                func_8005E020(arg0);
            } else {
                func_800373FC(0, arg0);
            }
            if (arg0[4] == 0) return 1;
        }
    }
    return 0;
}

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

void func_80037ACC(void) {
    s32 i;
    AnimSlot *s;

    s = D_800D30D8;
    for (i = 0; i < 6; i++) {
        s->flags &= ~1;
        s++;
    }
    D_800D31C0.unk0 = 0;
    D_800D31C0.unk2 = 0x9C4;
    D_800D31C0.unk4 = 0;
    D_800D31C0.unk6 = 0;
    D_800D31C0.unk8 = 2;
    D_800D31C0.unkC = 0x88B8;
    D_800D31D0.unk0 = 0;
    D_800D31D0.unk2 = 0x9C4;
    D_800D31D0.unk4 = 0;
    D_800D31D0.unk6 = 0;
    D_800D31D0.unk8 = 2;
    D_800D31D0.unkC = 0x88B8;
    D_800D31C0.unk7 = 0;
    D_800D31D0.unk7 = 0;
}

s32 func_80037B6C(u8 *obj) {
    AnimSlot *s;
    s32 *pos;
    u8 hit;

    if (D_8009CF54 != 0) {
        goto fail;
    }
    if (g_CutScene != 0) {
        goto fail;
    }
    if (*(u16 *)(obj + 0xE) == 0x7FFF) {
        goto fail;
    }
    if (*(u16 *)(obj + 0xE) & 0x200) {
        goto fail;
    }
    if (*(s32 *)(obj + 0x34) <= 0) {
        goto fail;
    }
    if (obj[0xA4] & 2) {
        goto fail;
    }
    if (func_800349F8(g_Player) != 0) {
        goto fail;
    }
    if (obj[5] == 6 && func_800349F8(obj) != 0) {
    fail:
        return 0;
    }
    pos = (s32 *)(*(u8 **)(obj + 0x90) + 0x28);
    hit = 0;
    for (s = D_800D30D8; s < &D_800D30D8[6]; s++) {
        if (!(s->flags & 1)) {
            continue;
        }
        if (s->unk0 != 0) {
            if (*(void **)(s->unk0 + 0x88) != g_Player) {
                if (!(*(u16 *)(obj + 0xE) & 0x400)) goto next;
                goto fail;
            }
            if (*(u16 *)(obj + 0xE) & 0x800) goto fail;
        }
    next:
        if (s->start + *(u16 *)(obj + 0x2A) < ((pos[0] - s->x) < 0 ? -(pos[0] - s->x) : (pos[0] - s->x))) {
            continue;
        }
        if (s->start + *(u16 *)(obj + 0x28) < ((pos[1] - s->y) < 0 ? -(pos[1] - s->y) : (pos[1] - s->y))) {
            continue;
        }
        if (s->start + *(u16 *)(obj + 0x2A) < ((pos[2] - s->z) < 0 ? -(pos[2] - s->z) : (pos[2] - s->z))) {
            continue;
        }
        if ((u32)(s->time - 3) >= *(u32 *)(obj + 0x2C)) {
            hit = 1;
            break;
        }
    }
    if (!hit) {
        goto fail;
    }
    if (obj == g_Player) {
        return D_8009CF54 == 0;
    }
    if (obj[0xA] & 2) {
        M2C_FIELD(obj, s32 *, 0x34) -= 1000;
        if (D_8009CF60 == 0) {
            D_8009CF60 = D_8009CDA8;
            func_8005E020(obj);
        }
        if ((u32)(D_8009CDA8 - D_8009CF60) > 30) {
            D_8009CF60 = 0;
        }
        return *(s32 *)(obj + 0x34) <= 0;
    }
    if (obj[5] == 6) {
        *(s32 *)(obj + 0x34) = 0;
    } else {
        *(s32 *)(obj + 0x34) -= 0x88B8;
    }
    if (*(s32 *)(obj + 0x34) <= 0 && obj[5] == 6 && obj != g_Player) {
        obj[0xA4] |= 4;
        *(s16 *)(*(u8 **)(obj + 0x90) + 0x62) = ((s32 (*)(s32, s32))ratan2)(s->x - pos[0], s->z - pos[2]) & 0xFFF;
    }
    return 1;
}

void func_80037EB0(void *arg0) {
    ChrSnd *s;

    s = M2C_FIELD(arg0, ChrSnd **, 0x94);
    if (g_CutScene) {
        if (s->voice >= 0) {
            func_80081458(s->voice);
            s->voice = -1;
            s->timer = 0;
            s->active = 0;
        }
        return;
    }
    if (s->voice == -1 && s->active) {
        s->voice = func_80027248(0x30, M2C_FIELD(arg0, s32 *, 0x90) + 0x14, 0);
    } else if (!(M2C_FIELD(s, u32 *, 4) & 0x10000)) {
        if (s->voice >= 0) {
            func_80081458(s->voice);
            s->voice = -1;
            s->timer = 0;
        }
    }
    if (M2C_FIELD(s, u32 *, 4) & 0x10000) {
        if (s->voice >= 0) {
            func_80027394(M2C_FIELD(arg0, s32 *, 0x90) + 0x14, s->voice, 0x30, 0);
        }
        s->timer++;
        if ((u32)s->timer >= 9) {
            s->active = 0;
        }
    }
}

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

#ifdef NON_MATCHING
void func_800381CC(void *objArg, s32 arg1) {
    u8 *obj = (u8 *)objArg;
    u8 *m;
    u8 *p;
    s32 speed;
    s32 *t;
    *(SVECTOR *)D_1F8003E0 = M2C_FIELD(obj, SVECTOR *, 0x3C);
    ((SVECTOR *)D_1F8003E0)->vz = ((SVECTOR *)D_1F8003E0)->vz >> 1;
    m = M2C_FIELD(obj, u8 **, 0x90);
    gte_SetRotMatrix(m + 0x14);
    gte_ldv0(D_1F8003E0);
    gte_rtv0();
    t = (s32 *)(m + 0x28);
    gte_stlvnl(D_1F8003E8);
    D_1F8003D4 = t[0] + D_1F8003E8[0];
    D_1F8003D8 = t[1] + D_1F8003E8[1];
    D_1F8003DC = t[2] + D_1F8003E8[2];
    p = (u8 *)func_80039A9C(0, arg1, 2);
    if (p != NULL) {
        speed = 70;
        M2C_FIELD(p, s16 *, 0xB0) = (rand() & 0xF) * 34 + 0x5B;
        *(s32 *)(p + 0x58) = 100;
        M2C_FIELD(p, s16 *, 0xAC) = D_1F8003D8;
        M2C_FIELD(p, s16 *, 0xAE) = -((*(s16 *)&D_800A0370[M2C_FIELD(p, u16 *, 0xB0) & 0xFFF] * speed) >> 12);
        func_8003B53C(p, (M2C_FIELD(M2C_FIELD(obj, u8 **, 0x90), u16 *, 0x62) - 0x38E) & 0xFFF, M2C_FIELD(p, s16 *, 0xB0), speed);
        M2C_FIELD(p, s16 *, 0x9E) = (rand() & 3) * 91 + 0x22;
        M2C_FIELD(p, s16 *, 0xA0) = (rand() & 3) * 91 + 0x22;
        M2C_FIELD(p, s16 *, 0xA2) = (rand() & 3) * 91 + 0x22;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800381CC);
#endif

void func_80038404(void *a0, void *a1) {
    ((SVECTOR *)D_1F8003E0)->vx = -60;
    ((SVECTOR *)D_1F8003E0)->vy = -60;
    ((SVECTOR *)D_1F8003E0)->vz = 200;
    func_800381CC(a1, D_8009D280);
}

void func_80038448(void) {
    void *s2;
    void *player;
    Weapon *w;
    u8 id;
    s32 obj;
    s32 n;
    s32 snd;
    s32 k;

    s2 = D_8009CD98;
    player = g_Player;
    w = &g_Weapons[D_800B0DD4];
    id = M2C_FIELD(s2, u8 *, 0x3A);
    if (id == 0) {
        id = func_80069C00();
    }
    obj = func_80039A9C(player, id, 1);
    if (obj != 0) {
        func_8003872C(s2, (void *)obj, w);
        if (!(M2C_FIELD(obj, s32 *, 0x68) & 0x4000)) {
            M2C_FIELD(g_Args, s32 *, 0x2C8)++;
            if (!((g_Args->flags2 >> 14) & 1)) {
                if (((u32)w->flags >> 13) & 1) {
                    w->clip--;
                    w->ammo = w->clip + w->reserve;
                } else {
                    w->ammo--;
                }
            }
        }
        if (*(u8 *)&g_Args->flags2 >> 7) {
            u32 t = M2C_FIELD(w, u32 *, 0x5C);
            func_8002F564(player, (t >> 16) & 0x3FF, t >> 26, 0, 0);
        }
        if (((u32)w->flags >> 20) & 1) {
            D_800D9208 = M2C_FIELD(w, u32 *, 0x5C) >> 26;
            n = (u16)((M2C_FIELD(w, u16 *, 0x5E) & 0x3FF) / 100);
            if (n <= 0) {
                D_800D920C = 1;
            } else if (n >= 9) {
                D_800D920C = 8;
            } else {
                D_800D920C = n;
            }
        } else {
            D_800D920C = 0;
            D_800D9208 = 0;
        }
        if ((s8)w->pad51[9] >= 0) {
            if (((u32)w->flags >> 1) & 1) {
                D_800AFFA4 = 1;
                D_8009DB38 = D_8009CDA8;
            } else {
                func_80026F30((s8)w->pad51[9], 0);
            }
        }
        if (D_800B0DD4 == 3 && D_8009CD4C != 0) {
            snd = 0x23;
        } else {
            snd = 8;
            if (D_800DDEE8 != 0 && D_8009D910 >= 3) {
                snd = 9;
            }
        }
        func_8007A17C(snd);
        k = ((u32)w->flags >> 15) & 0xF;
        if (k > 0) {
            func_80076184(k, 0);
        }
        if (((u32)w->flags >> 21) & 1) {
            func_80038404((void *)(s32)D_800B0DD4, g_Player);
        }
    }
}

void func_8003872C(void *arg0, void *arg1, void *arg2) {
    s32 temp_a0;
    u32 temp_a2;
    void *temp_s0;

    M2C_FIELD(arg0, u16 *, 0x38) = (u16) M2C_FIELD(arg2, u16 *, 0xB2);
    temp_s0 = M2C_FIELD(arg1, void **, 0x90);
    M2C_FIELD(arg1, s32 *, 0x58) = (s32) M2C_FIELD(arg2, s32 *, 0xB4);
    M2C_FIELD(arg1, s32 *, 0x34) = (s32) M2C_FIELD(arg2, s16 *, 0xAC);
    M2C_FIELD(arg1, s32 *, 0x64) = (s32) ((M2C_FIELD(arg1, s32 *, 0x64) & 0xFF7FFFFF) | ((M2C_FIELD(arg2, u8 *, 0x3B) & 1) << 0x17));
    M2C_FIELD(temp_s0, s32 *, 0x28) = (s32) (g_Camera.posX + D_800B0C10[0]);
    M2C_FIELD(temp_s0, s32 *, 0x2C) = (s32) (g_Camera.posY + D_800B0C10[1]);
    M2C_FIELD(temp_s0, s32 *, 0x30) = (s32) (g_Camera.posZ + D_800B0C10[2]);
    temp_a0 = M2C_FIELD(g_Player, s32 *, 0x5C);
    if ((temp_a0 != 0) && !(M2C_FIELD(M2C_FIELD(g_Player, void **, 0x94), s32 *, 0x24) & 0x800)) {
        func_80036E10((u8 *)temp_a0, arg1);
        M2C_FIELD(temp_s0, u16 *, 0x60) = (u16) D_800B0D50;
        M2C_FIELD(temp_s0, u16 *, 0x62) = (u16) D_800B0084;
    } else {
        M2C_FIELD(temp_s0, u16 *, 0x60) = (u16) M2C_FIELD(&g_Camera, u16 *, 0x80);
        M2C_FIELD(temp_s0, u16 *, 0x62) = (u16) M2C_FIELD(&g_Camera, u16 *, 0x82);
    }
    M2C_FIELD(temp_s0, u16 *, 0x64) = (u16) D_800B0068;
    M2C_FIELD(arg1, u16 *, 0xAC) = (u16) M2C_FIELD(temp_s0, s32 *, 0x2C);
    func_8003BC98((u8 *)temp_s0 + 0x60, (u8 *)temp_s0 + 0x10, (s32)temp_s0);
    temp_a2 = M2C_FIELD(arg2, u32 *, 0x38);
    if (((temp_a2 >> 2) & 1) && (D_8009CF48 < 0x78)) {
        ((void (*)(void *, s32, u32))func_800380DC)(temp_s0, ((temp_a2 >> 3) & 0x3F) * 0xB, temp_a2);
    }
    ((void (*)(u8 *, s32, s32, s16))func_8003B53C)(arg1, (s16) M2C_FIELD(temp_s0, u16 *, 0x62), (s16) M2C_FIELD(temp_s0, u16 *, 0x60), M2C_FIELD(arg0, s16 *, 0x38));
    D_8009CF48 = 0;
}

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

#ifdef NON_MATCHING
void func_80038984(u8 *arg0, ObjHH *arg1, ArgHH *arg2) {
    u8 *s0;
    s32 t;
    s0 = arg1->s90;
    t = arg1->w64 & 0xFF7FFFFF;
    *(u16 *)(arg0 + 0x38) = arg2->hB2;
    arg1->w58 = arg2->wB4;
    arg1->w34 = arg2->hAC;
    arg1->w64 = t | ((arg2->b3B & 1) << 23);
    arg1->w68 |= 0x2000;
    arg1->h0E |= 0x100;
    *(s32 *)(s0 + 0x28) = g_Camera.posX + D_800B0C10[0];
    *(s32 *)(s0 + 0x30) = g_Camera.posZ + D_800B0C10[2];
    *(s32 *)(s0 + 0x2C) = g_Camera.posY + D_800B0C10[1];
    *(u16 *)(s0 + 0x62) = *(u16 *)((u8 *)&g_Camera + 0x82);
    *(u16 *)(s0 + 0x64) = *(u16 *)((u8 *)&g_Camera + 0x84);
    *(u16 *)(s0 + 0x60) = *(u16 *)((u8 *)&g_Camera + 0x80);
    arg1->hAC = *(u16 *)(s0 + 0x2C);
    func_8003BC98(s0 + 0x60, s0 + 0x10, (s32)s0);
    func_8003B53C((u8 *)arg1, *(s16 *)(s0 + 0x62), *(s16 *)(s0 + 0x60), *(s16 *)(arg0 + 0x38));
    D_8009CF48 = 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80038984);
#endif

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

void func_80038C8C(u8 *obj) {
    s32 *pos;
    u8 *r;
    u8 *def;
    u8 drop1;
    u8 drop2;
    u8 score;
    u8 snd;
    u8 b;
    ListSet *set;
    u32 j;

    if (*(void **)(obj + 0x100) != NULL) {
        func_80063ADC(*(void **)(obj + 0x100));
        *(void **)(obj + 0x100) = NULL;
    }
    ((void (*)(void *))func_80052BEC)(obj);
    if (obj[5] == 6) {
        return;
    }
    if (obj[8] != 0) {
        pos = (s32 *)(*(u8 **)(obj + 0x90) + 0x28);
        D_1F8003D4 = pos[0];
        D_1F8003DC = pos[2];
        D_1F8003D8 = pos[1] - 100;
        if (D_1F8003D8 < -0x36B0) {
            D_1F8003D8 = -0x36B0;
        }
        r = func_800390F8(obj[8]);
        if (r != NULL && *(s32 **)(obj + 0x60) != NULL) {
            (*(s32 **)(obj + 0x60))[0] = 0;
            (*(s32 **)(obj + 0x60))[1] = 0;
            (*(s32 **)(obj + 0x60))[2] = 0;
            (*(s32 **)(obj + 0x60))[0] |= 0x100 | obj[8];
            *(s32 **)(r + 0x60) = *(s32 **)(obj + 0x60);
            (*(s32 **)(obj + 0x60))[-3] = g_Args->playerX + *(s32 *)(*(u8 **)(r + 0x90) + 0x28);
            (*(s32 **)(obj + 0x60))[-2] = g_Args->playerY + *(s32 *)(*(u8 **)(r + 0x90) + 0x2C);
            (*(s32 **)(obj + 0x60))[-1] = g_Args->playerZ + *(s32 *)(*(u8 **)(r + 0x90) + 0x30);
            *(s32 **)(obj + 0x60) = NULL;
        }
    }
    func_80060094(obj);
    def = *(u8 **)obj;
    drop1 = def[5];
    drop2 = def[6];
    score = def[4];
    if (def[0x5D] != 0) {
        *((u8 *)g_Args + 0x138) = 0x7F;
        func_80027248((*(u8 **)obj)[0x5D], (s32)(*(u8 **)(obj + 0x90) + 0x14), 2);
    }
    if (def[0x2F] != 0) {
        func_80063F34(obj, def[0x2F]);
    }
    if (drop1 != 0) {
        s32 h = *(s16 *)(obj + 0x22);
        pos = (s32 *)(*(u8 **)(obj + 0x90) + 0x28);
        D_1F8003D4 = pos[0];
        D_1F8003D8 = pos[1] + h;
        D_1F8003DC = pos[2];
        func_80039A9C(0, drop1, 2);
    }
    if (drop2 != 0) {
        s32 h = *(s16 *)(obj + 0x22);
        pos = (s32 *)(*(u8 **)(obj + 0x90) + 0x28);
        D_1F8003D4 = pos[0];
        D_1F8003D8 = pos[1] + h;
        D_1F8003DC = pos[2];
        r = (u8 *)func_80039A9C(0, drop2, 2);
        if (r != NULL) {
            r[0xA4] = 0;
            if (((AnimEnt *)D_800D8FF8)[r[0xA9]].flags & 0x200) {
                *(s16 *)(*(u8 **)(r + 0x90) + 0x62) = *(s16 *)(*(u8 **)(obj + 0x90) + 0x62);
                *(s32 *)(*(u8 **)(r + 0x90) + 0x10) = 2;
            }
            if (((AnimEnt *)D_800D8FF8)[r[0xA9]].flags & 0x100) {
                *(s16 *)(r + 0x6A) = *(s16 *)(obj + 0x28) + 0x800;
            }
            if (((AnimEnt *)D_800D8FF8)[r[0xA9]].flags & 1) {
                *(s32 *)(*(u8 **)(r + 0x90) + 0x2C) -= *(s16 *)(obj + 0x22);
            }
        }
    }
    if ((*(u8 **)obj)[3] != 0) {
        set = &D_800D8C7C[((AnimEnt *)D_800D8FF8)[obj[0xA9]].animIdx];
        for (j = 0; j < *(u32 *)set; j++) {
            ((AnimEntArr *)D_800D8FF8)->e[obj[0xA9]].startTime[j] = D_8009CDA8;
        }
    }
    if ((*(s32 *)(obj + 0x4C) & 0x900) == 0x100) {
        func_80058B10(obj);
    }
    if (score == 0) {
        if (*(s32 **)(obj + 0x60) != NULL) {
            **(s32 **)(obj + 0x60) = 0;
            *(s32 **)(obj + 0x60) = NULL;
        }
        func_800395E8(obj);
        return;
    }
    func_8006B230(*(s32 *)((u8 *)g_Args + 0x214) >= score, (M2C_UNK)D_800A7D74);
    func_800391B0(obj, score);
}

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

#ifdef NON_MATCHING
void func_800391B0(u8 *arg0, s32 arg1) {
    u8 *obj;
    u8 *p;
    u8 *part;
    part = *(u8 **)(arg0 + 0x90);
    D_1F8003D4 = *(s32 *)(part + 0x28);
    p = part;
    D_1F8003DC = *(s32 *)(p + 0x30);
    D_1F8003D8 = *(s32 *)(p + 0x2C);
    obj = (u8 *)func_80039A9C(0, arg1, ((u8 *)g_Args->unk228 + (u8)arg1 * 0x70)[1]);
    if (obj != NULL) {
        if (*(s32 **)(arg0 + 0x60) != NULL) {
            **(s32 **)(arg0 + 0x60) &= ~0xFF;
            **(s32 **)(arg0 + 0x60) |= 0x100 | (*(u8 **)arg0)[4];
            *(s32 **)(obj + 0x60) = *(s32 **)(arg0 + 0x60);
            if ((*(s32 **)(obj + 0x60))[1] < 0) {
                *(s32 *)(obj + 0x48) &= ~2;
            }
        }
        obj[0xA4] = 0;
        *(SVECTOR *)(*(u8 **)(obj + 0x90) + 0x60) = *(SVECTOR *)(*(u8 **)(arg0 + 0x90) + 0x60);
        p = *(u8 **)(obj + 0x90);
        func_8003BC98(p + 0x60, p + 0x10, (s32)p);
        if (*(s16 *)(arg0 + 0x6A) != 0) {
            *(u16 *)(obj + 0x6A) = *(u16 *)(arg0 + 0x6A);
            func_8002DD6C(obj);
        }
        func_80060094(obj);
        if (*(u8 **)(arg0 + 0x88) != NULL) {
            if (g_Args->cdArchive == 0) {
                sprintf(g_Args->exePath, D_800A7D90, arg1 & 0xFF);
                func_8006B1DC((s32)g_Args->exePath);
            }
            *(u8 **)(*(u8 **)(arg0 + 0x88) + 0x8C) = obj;
            *(u8 **)(obj + 0x88) = *(u8 **)(arg0 + 0x88);
            *(u8 **)(obj + 0x8C) = *(u8 **)(arg0 + 0x8C);
            if (*(u8 **)(arg0 + 0x8C) != NULL) {
                *(u8 **)(*(u8 **)(arg0 + 0x8C) + 0x88) = obj;
            }
            *(u8 **)(arg0 + 0x88) = NULL;
            *(u8 **)(arg0 + 0x8C) = NULL;
        }
    }
    func_800395E8(arg0);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800391B0);
#endif

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

#ifdef NON_MATCHING
s32 func_8003982C(u8 *obj) {
    s32 first;
    s32 last;
    s32 i;
    u8 id;
    u8 t;
    Ent124 *p;

    id = obj[0x3A];
    if (((u8 *)g_Args->unk228)[id * 0x70 + 0x2D] == 1) {
        first = D_800B0B78;
        last = D_800AFF58;
    } else {
        first = D_800B05AC;
        last = D_800B0B80;
    }
    i = first;
    p = (Ent124 *)D_800AFFC8 + i;
    for (; i <= last; i++, p++) {
        if (p->owner == 0 && p->active != 0) {
            t = p->id;
            if (t == id) {
                p->active = 0;
                if (g_Args->cdArchive == 0) {
                    sprintf(D_800B0BC0, D_800A7DF4, t, i);
                    func_8006B1DC((s32)D_800B0BC0);
                }
                return func_80039A9C((s32)obj, t, 4);
            }
        }
    }
    i = first;
    p = (Ent124 *)D_800AFFC8 + i;
    for (; i <= last; i++, p++) {
        if (p->owner == 0 && p->active != 0 && p->busy == 0) {
            p->active = 0;
            if (g_Args->cdArchive == 0) {
                sprintf(D_800B0BC0, D_800A7E14, p->id, i);
                func_8006B1DC((s32)D_800B0BC0);
            }
            return func_80039A9C((s32)obj, id, 4);
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003982C);
#endif

s32 func_80039A34(u8 *arg0) {
    s32 p;

    p = func_80039A9C(arg0, arg0[0x3A], 4);
    *(s32 *)(arg0 + 0x8C) = p;
    if (p == 0) {
        p = func_8003982C(arg0);
        *(s32 *)(arg0 + 0x8C) = p;
        if (p == 0) {
            return 0;
        }
    }
    *(s32 *)(p + 0x68) = (*(s32 *)(p + 0x68) & ~0x380) | 0x80;
    return 1;
}

#ifdef NON_MATCHING
s32 func_80039A9C(s32 arg0, u8 arg1, s32 arg2) {
    u16 count;
    u8 persistent;
    u8 type;
    u8 kind;
    AnimEnt *fp;
    s32 lo;
    s32 hi;
    u16 limit;
    s32 i;
    s32 t;
    s32 idx;
    s32 *msg;
    u8 *slot;
    u8 *objs;
    u8 *sub;
    u8 *rec;
    u8 *parent;
    u8 *ent;
    AnimSlot *func_800378FC();

    type = arg1;
    kind = arg2;
    persistent = 0;
    if (!type) {
        if (g_Args->cdArchive == 0) {
            sprintf(D_800B0BC0, &D_800A7E34, 0, kind, D_8009CDA8);
            func_8006B1DC((s32 *) D_800B0BC0);
        }
        return 0;
    }
    switch (kind) {
    case 1:
        lo = D_800B0DA8;
        hi = D_800B07B8;
        limit = 1;
        break;
    case 2:
        if (M2C_FIELD(((type * 0x70) + g_Args->unk228), u8 *, 0x2D) == 1) {
            lo = D_800B0DCC;
            hi = D_800B0BBC;
            limit = D_800AFF4C;
        } else {
            lo = D_800B0DBC;
            hi = D_800AFF70;
            limit = 1;
        }
        break;
    case 3:
        if (M2C_FIELD(((type * 0x70) + g_Args->unk228), u8 *, 0x2D) == 1) {
            lo = D_800B0DC4;
            hi = D_800B0BB8;
            limit = D_800B0804;
        } else {
            lo = D_800B0DB8;
            hi = D_800B0BB4;
            limit = D_800AFF84;
        }
        break;
    case 4:
    case 9:
        limit = 1;
        if (M2C_FIELD(((type * 0x70) + g_Args->unk228), u8 *, 0x2D) == 1) {
            lo = D_800B0B78;
            hi = D_800AFF58;
        } else {
            lo = D_800B05AC;
            hi = D_800B0B80;
        }
        if (kind == 9) {
            persistent = 1;
        }
        break;
    case 5:
        lo = D_800B0DC8;
        hi = D_800B07AC;
        limit = 1;
        break;
    case 6:
        lo = D_800B0DA0;
        hi = D_800B07B0;
        limit = 1;
        break;
    case 7:
        limit = 1;
        kind = 1;
        lo = D_800B0DA8;
        hi = D_800B07B8;
        persistent = 1;
        break;
    case 8:
        lo = D_800B0DB0;
        hi = D_800B07C0;
        limit = 1;
        break;
    default:
        return 0;
    }
    count = D_800D2BF0[type];
    if ((s16) limit < (s32) D_800D2BF0[type]) {
        return 0;
    }
    /* find a free slot in [lo, hi] */
    slot = (u8 *) D_800AFFC8 + (lo * 0x124);
    if (hi >= lo) {
        while (1) {
            lo++;
            if (M2C_FIELD(slot, u8 *, 4) == 0) {
                break;
            }
            slot += 0x124;
            if (hi < lo) {
                goto no_slot;
            }
        }
    } else {
no_slot:
        if (kind == 4) {
            if (M2C_FIELD(((type * 0x70) + g_Args->unk228), u8 *, 0x2D) == 1) {
                if (g_Args->cdArchive == 0) {
                    sprintf(D_800B0BC0, &D_800A7E50, type, limit);
                    func_8006B230(0, (s32) D_800B0BC0);
                    return 0;
                }
            }
        }
        return 0;
    }
    func_8004C0C0(type, slot);
    M2C_FIELD(slot, s32 *, 0x94) = 0;
    if (kind == 6) {
        M2C_FIELD(slot, s16 *, 0x28) = 0x4EE;
        M2C_FIELD(slot, s16 *, 0x2A) = 0xD4;
        M2C_FIELD(slot, s32 *, 0xC0) = 0;
        t = func_8005D5C4((M2C_UNK *) &D_800B05B0, &D_800B05B0 + 0x188);
        if (t == 0) {
            return func_800393E4(slot);
        }
        func_8005D58C(t);
        M2C_FIELD(slot, s32 *, 0x94) = t;
        t = func_8005B5AC(slot, &D_800DDFEC);
        if (t == 0) {
            return func_800393E4(slot);
        }
        M2C_FIELD(slot, s32 *, 0xC0) = t;
        func_8005B6CC(slot);
        M2C_FIELD(slot, s32 *, 0x118) = 0;
        if (M2C_FIELD(slot, s32 *, 0x34) == 0) {
            M2C_FIELD(slot, s32 *, 0x34) = 1;
        }
    } else if (kind == 1) {
        t = func_8003A68C();
        M2C_FIELD(slot, s32 *, 0x10C) = t;
        if (t == 0) {
            func_8006B1DC(&D_800A7E74);
            return func_800393E4(slot);
        }
    }
    fp = NULL;
    if (M2C_FIELD(M2C_FIELD(slot, u8 **, 0), u8 *, 3) != 0) {
        M2C_FIELD(slot, s8 *, 0xA9) = 0;
        fp = func_8003A71C((u8 **) slot);
        if (fp == NULL) {
            return func_800393E4(slot);
        }
        type = M2C_FIELD(fp, u8 *, 2);
    }
    M2C_FIELD(slot, s8 *, 0xA4) = 0;
    M2C_FIELD(slot, s8 *, 5) = kind;
    M2C_FIELD(slot, u8 *, 0xB) = count;
    if (kind == 6) {
        objs = M2C_FIELD(slot, u8 **, 0x90);
        func_8007A4E4(NULL, objs + 0x10);
        M2C_FIELD(objs, s32 *, 0x28) = *(s32 *) 0x1F8003D4;
        M2C_FIELD(objs, s32 *, 0x2C) = *(s32 *) 0x1F8003D8;
        M2C_FIELD(objs, s32 *, 0x30) = *(s32 *) 0x1F8003DC;
        func_80060AC0(slot);
        *(SVECTOR *) (objs + 0x60) = *(SVECTOR *) &D_800B077C;
        M2C_FIELD(objs, s16 *, 0x66) = 0;
        M2C_FIELD(objs, s32 *, 0x54) = 0;
    } else {
        ent = M2C_FIELD(slot, u8 **, 0);
        if (M2C_FIELD(ent, u8 *, 0x5F) >= D_800D86DC) {
            sprintf(g_Args->exePath, &D_800A7E90, M2C_FIELD(slot, u8 *, 6), M2C_FIELD(ent, u8 *, 0x5F), (s32) D_800D86DC);
            func_8006B230(0, (s32) g_Args->exePath);
        }
        objs = M2C_FIELD(slot, u8 **, 0x90);
        func_8007A4E4(NULL, objs + 0x10);
        ent = M2C_FIELD(slot, u8 **, 0);
        if ((M2C_FIELD(ent, u8 *, 0x5F) != 0) || (M2C_FIELD(slot, s32 *, 0x48) & 0x40)) {
            M2C_FIELD(objs, s32 *, 0x28) = *(s32 *) 0x1F8003D4 + M2C_FIELD(((M2C_FIELD(ent, u8 *, 0x5F) * 0x7E) + (u8 *) D_800D86E0), s16 *, 6);
            M2C_FIELD(objs, s32 *, 0x2C) = *(s32 *) 0x1F8003D8 + M2C_FIELD(((M2C_FIELD(M2C_FIELD(slot, u8 **, 0), u8 *, 0x5F) * 0x7E) + (u8 *) D_800D86E0), s16 *, 0x12);
            t = *(s32 *) 0x1F8003DC + M2C_FIELD(((M2C_FIELD(M2C_FIELD(slot, u8 **, 0), u8 *, 0x5F) * 0x7E) + (u8 *) D_800D86E0), s16 *, 0x1E);
        } else {
            M2C_FIELD(objs, s32 *, 0x28) = *(s32 *) 0x1F8003D4;
            M2C_FIELD(objs, s32 *, 0x2C) = *(s32 *) 0x1F8003D8;
            t = *(s32 *) 0x1F8003DC;
        }
        M2C_FIELD(objs, s32 *, 0x30) = t;
        *(SVECTOR *) (objs + 0x60) = *(SVECTOR *) &D_800B077C;
        M2C_FIELD(objs, s16 *, 0x66) = 0;
        M2C_FIELD(objs, s32 *, 0x54) = 0;
        sub = objs;
        for (i = 1; i < (s16) count; i++) {
            sub += 0x8C;
            rec = (u8 *) D_800D86E0 + M2C_FIELD(M2C_FIELD(slot, u8 **, 0), u8 *, 0x5F) * 0x7E;
            func_8007A4E4((M2C_FIELD(slot, u8 **, 0x90) + (rec[i] * 0x8C)) - 0x7C, sub + 0x10);
            rec = (u8 *) D_800D86E0 + M2C_FIELD(M2C_FIELD(slot, u8 **, 0), u8 *, 0x5F) * 0x7E;
            M2C_FIELD(sub, s32 *, 0x28) = M2C_FIELD(rec + i * 2, s16 *, 6);
            M2C_FIELD(sub, s32 *, 0x2C) = M2C_FIELD(rec + i * 2, s16 *, 0x12);
            M2C_FIELD(sub, s32 *, 0x30) = M2C_FIELD(rec + i * 2, s16 *, 0x1E);
            *(SVECTOR *) (sub + 0x60) = *(SVECTOR *) &D_800B077C;
            M2C_FIELD(sub, s16 *, 0x66) = 0;
            M2C_FIELD(sub, s32 *, 0x54) = 0;
        }
    }
    sub = M2C_FIELD(slot, u8 **, 0x90);
    for (i = 0; i < (s16) count; i++) {
        GsLinkObject4(D_800D2D30[type], sub, i);
        M2C_FIELD(sub, u8 **, 4) = sub + 0x10;
        M2C_FIELD(sub, s32 *, 0) = 0;
        M2C_FIELD(sub, s32 *, 0) = M2C_FIELD(slot, s32 *, 0x7C);
        GsGetLs(M2C_FIELD(sub, u8 **, 4), &D_800B0848);
        M2C_FIELD(sub, s32 *, 0x10) = 2;
        M2C_FIELD(sub, s32 *, 0x88) &= 0xFFFF0000;
        sub += 0x8C;
    }
    M2C_FIELD(slot, s32 *, 0x30) = 0;
    M2C_FIELD(slot, s32 *, 0x18) = 0xFFF;
    M2C_FIELD(slot, s8 *, 0xA8) = 0x63;
    M2C_FIELD(slot, s32 *, 0x2C) = D_8009CDA8;
    M2C_FIELD(slot, s32 *, 0x8C) = 0;
    M2C_FIELD(slot, s32 *, 0x88) = arg0;
    if (M2C_FIELD(slot, u8 *, 7) == 0x17) {
        M2C_FIELD(slot, s16 *, 0x6A) = 0x1000;
        func_8002DD6C(slot);
    }
    idx = M2C_FIELD((type * 0x70) + g_Args->unk228, u8 *, 0x5C);
    if ((persistent == 0) && (idx != 0) && (arg0 != (s32) g_Player)) {
        func_80027248(idx, (s32) (M2C_FIELD(slot, u8 **, 0x90) + 0x14), 0);
    }
    M2C_FIELD(slot, s32 *, 0xB8) = 0;
    idx = M2C_FIELD((type * 0x70) + g_Args->unk228, u8 *, 0xB);
    if (idx != 0) {
        M2C_FIELD(slot, s32 *, 0xB8) = func_80039A9C((s32) slot, idx, 2);
    }
    if (kind == 6) {
        func_8005BA30(slot);
        if ((M2C_FIELD(slot, u8 *, 0x3A) != 0) && (func_80039A34(slot) == 0)) {
            if ((g_Args->cdArchive == 0) && (D_8009CDA8 != ((D_8009CDA8 / 90) * 0x5A))) {
                func_8006B1DC(&D_800A7EC4);
            }
            return func_800393E4(slot);
        }
        M2C_FIELD(slot, s16 *, 0x20) = -0x578;
        M2C_FIELD(slot, s16 *, 0x22) = 0;
    }
    M2C_FIELD(slot, u8 *, 4) = 1;
    if ((fp != NULL) && (fp->flags & 0x80)) {
        M2C_FIELD(slot, AnimSlot **, 0x104) = func_800378FC(M2C_FIELD(slot, u8 **, 0x90) + 0x14, (u8 *) &D_800D31C0, M2C_FIELD(slot, s16 *, 0x2A), M2C_FIELD(slot, s32 *, 0x88));
    }
    return (s32) slot;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80039A9C);
#endif

#ifdef NON_MATCHING
u8 *func_8003A51C(arg0, type, sub)
s32 arg0;
u8 type;
u8 sub;
{
    s32 i;
    u8 *e;
    u8 *p;
    if (type == 0) {
        if (g_Args->cdArchive == 0) {
            sprintf(D_800B0BC0, D_800A7EFC, 0, sub, D_8009CDA8);
            func_8006B1DC((s32)D_800B0BC0);
        }
        return 0;
    }
    if (sub != 10) {
        return 0;
    }
    i = D_800B0DA4;
    e = (u8 *)D_800AFFC8 + i * 0x124;
    for (; i <= D_800B07B4; i++, e += 0x124) {
        if (e[4] == 0) {
            goto found;
        }
    }
    return 0;
found:
    func_8004C368(type, e);
    e[5] = sub;
    *(s32 *)(e + 0x94) = 0;
    *(s32 *)(e + 0x114) = func_800752E8(e[6]);
    if (*(s32 *)(e + 0x114) != 0) {
        p = e;
        p[0xB] = 1;
        p[0xA4] = 0;
        p[5] = sub;
        *(s32 *)(p + 0x30) = 0;
        *(s32 *)(p + 0x88) = arg0;
        *(s32 *)(p + 0x8C) = 0;
        p[4] = 1;
        *(s32 *)(p + 0x2C) = D_8009CDA8;
    } else {
        p = (u8 *)func_800393E4(e);
    }
    return p;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003A51C);
#endif

#ifdef NON_MATCHING
u8 *func_8003A68C(void) {
    u8 *e = D_800D3288;
    s32 *q = (s32 *)(D_800D3288 + 0x68);
    s32 end = (s32)(e + 0xD88);
    s32 k = 0;
    s32 j, off;
    do {
        if (e[0x65] == 0) {
            e[0x65] = 1;
            e[0x64] = 0;
            e[0x66] = 0;
            off = k;
            for (j = 0; j < 5; j++) {
                u8 *s = D_800D3288 + off;
                *(s32 *)(s + 0xC) = 0;
                *(s16 *)(s + 0x10) = 0;
                *(s16 *)(s + 0x12) = 0;
                off += 0x14;
            }
            *q = 0;
            *(s32 *)(D_800D3288 + 0x68 + k + 4) = 0;
            return e;
        }
        e += 0x70;
        q = (s32 *)((u8 *)q + 0x70);
        k += 0x70;
    } while ((s32)q < end);
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003A68C);
#endif

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

EffSlot *func_8003A898(s32 arg0, s32 arg1) {
    EffSlot *e;
    s16 i;
    u8 *p;
    s32 idx;
    s32 x;
    s32 y;
    s32 z;

    e = (EffSlot *)D_800B0AB4;
    for (i = 0; i < D_800B0C84; i++, e++) {
        if (e->active == 0) {
            goto found;
        }
    }
    return NULL;
found:
    idx = arg0 & 0xFF;
    func_8004C498(idx, (u8 *)e);
    p = e->sub;
    func_8007A4E4(0, p + 0x10);
    x = D_1F8003D4;
    y = D_1F8003D8;
    z = D_1F8003DC;
    *(s32 *)(p + 0x28) = x;
    *(s32 *)(p + 0x2C) = y;
    *(s32 *)(p + 0x30) = z;
    *(SVECTOR *)(p + 0x60) = *(SVECTOR *)&D_800B077C;
    GsLinkObject4(D_800D2D30[idx], p, 0);
    *(u8 **)(p + 4) = p + 0x10;
    *(s32 *)p = e->unk4;
    e->active = 1;
    e->unk20 = arg1;
    e->time = D_8009CDA8;
    if (((u8 *)g_Args->unk228 + idx * 0x70)[0x5C] != 0) {
        func_80027248(((u8 *)g_Args->unk228 + idx * 0x70)[0x5C], (s32)(p + 0x14), 0);
    }
    return e;
}

s32 func_8003AA14(s32 *a, s32 *b) {
    s32 top;
    s32 left;
    s32 right;
    s32 bottom;
    s32 *p;
    s32 *q;
    s32 dx;
    s32 dy;
    s32 t;

    top = 0;
    left = top;
    right = 0x200;
    bottom = 0xF0;
    if (a[1] < b[1]) {
        p = b;
        q = a;
    } else {
        p = a;
        q = b;
    }
    dx = p[0] - q[0];
    dy = p[1] - q[1];
    if (p[1] > bottom && q[1] < bottom) {
        t = dx * (bottom - p[1]);
        if (dy * (left - p[0]) < t && t < dy * (right - p[0])) {
            return 1;
        }
    }
    if (q[1] < top && p[1] > top) {
        t = dx * (top - q[1]);
        if (dy * (left - q[0]) < t && t < dy * (right - q[0])) {
            return 1;
        }
    }
    if (a[0] < b[0]) {
        p = b;
        q = a;
    } else {
        p = a;
        q = b;
    }
    dx = p[0] - q[0];
    dy = p[1] - q[1];
    if (p[0] > right && q[0] < right) {
        t = dy * (right - p[0]);
        if (dx * (left - p[0]) < t && t < dx * (bottom - p[1])) {
            return 1;
        }
    }
    if (q[0] < left && p[0] > left) {
        t = dy * (left - q[0]);
        if (dx * (top - q[1]) < t && t < dx * (bottom - q[1])) {
            return 1;
        }
    }
    return 0;
}

#ifdef NON_MATCHING
s32 func_8003AC1C(u8 *o, s32 arg1) {
    s16 box[8][4];
    s16 prj[8][4];
    s32 ea[2];
    s32 eb[2];
    s32 z;
    s32 flg;
    s32 t1;
    s32 i;
    s32 v;
    u8 *p;
    u8 *par;
    s32 isSix;
    u16 dy;
    s32 ia;
    s32 ib;

    t1 = 0;
    if (D_800B0B9C == 0x63) {
        o[0xA6] = 1;
        return 1;
    }
    if (D_8009CAA4 != 0) {
        o[0xA6] = 1;
        return 1;
    }
    if (o[5] != 1 && *(u8 **)(o + 0x88) != 0) {
        *(B8 *)(o + 0x10) = *(B8 *)(*(u8 **)(o + 0x88) + 0x10);
        *(s32 *)(o + 0x18) = *(s32 *)(*(u8 **)(o + 0x88) + 0x18);
        par = *(u8 **)(o + 0x88);
        if (*(u8 **)(par + 0x8C) == o) {
            v = par[0xA6];
            o[0xA6] = v;
            return v;
        }
    }
    *(s16 *)(D_1F8003E0 + 0) = (*(s16 *)(o + 0x1E) + *(s16 *)(o + 0x1C)) >> 1;
    isSix = o[5] == 6;
    if (isSix) {
        v = *(s16 *)(o + 0x20) - *(s16 *)(o + 0x22);
    } else {
        v = *(s16 *)(o + 0x22) + *(s16 *)(o + 0x20);
    }
    *(s16 *)(D_1F8003E0 + 2) = v >> 1;
    v = (*(s16 *)(o + 0x26) + *(s16 *)(o + 0x24)) >> 1;
    *(s16 *)(D_1F8003E0 + 4) = v;
    gte_SetRotMatrix(*(u8 **)(o + 0x90) + 0x14);
    if (*(s32 *)(o + 0x4C) < 0) {
        gte_ldv0(D_1F8003E0);
        gte_rtv0();
        gte_stlvnl(D_1F8003E8);
    } else {
        D_1F8003E8[2] = v;
        D_1F8003E8[0] = *(s16 *)(D_1F8003E0 + 0);
        D_1F8003E8[1] = *(s16 *)(D_1F8003E0 + 2);
    }
    p = *(u8 **)(o + 0x90);
    D_1F8003E8[0] += *(s32 *)(p + 0x28);
    D_1F8003E8[1] += *(s32 *)(p + 0x2C);
    if (*(u8 **)(o + 0x94) != 0) {
        D_1F8003E8[1] += *(s16 *)(*(u8 **)(o + 0x94) + 0xE);
    }
    D_1F8003E8[2] += *(s32 *)(p + 0x30);
    v = D_1F8003E8[0];
    if (v >= -0x8000) { s32 c = 0x7FFF; if (v <= 0x7FFF) c = v; v = c; } else v = -0x8000;
    D_1F8003E8[0] = v;
    v = D_1F8003E8[1];
    if (v >= -0x8000) { s32 c = 0x7FFF; if (v <= 0x7FFF) c = v; v = c; } else v = -0x8000;
    D_1F8003E8[1] = v;
    v = D_1F8003E8[2];
    if (v >= -0x8000) { s32 c = 0x7FFF; if (v <= 0x7FFF) c = v; v = c; } else v = -0x8000;
    D_1F8003E8[2] = v;
    *(s16 *)(D_1F8003E0 + 0) = D_1F8003E8[0];
    *(s16 *)(D_1F8003E0 + 2) = D_1F8003E8[1];
    *(s16 *)(D_1F8003E0 + 4) = D_1F8003E8[2];
    gte_SetRotMatrix(D_800B0808);
    gte_SetTransMatrix(D_800B0808);
    gte_ldv0(D_1F8003E0);
    gte_rtps();
    gte_stsxy(o + 0x10);
    gte_stszotz(&z);
    *(s32 *)(o + 0x18) = z;
    *(s16 *)(o + 0x14) = z;
    if (z <= 0) {
        o[0xA6] = 0;
        if (*(u16 *)(o + 0x2A) < 0x1F4) return 1;
    } else {
        v = 0;
        if ((u16)(*(u16 *)(o + 0x10) + 0x14) < 0x229) {
            s16 y = *(s16 *)(o + 0x12);
            if (y >= -0xA) v = y < 0xFB;
        }
        o[0xA6] = v;
    }
    if (!(*(s32 *)(o + 0x48) & 0x20)) return 0;
    if (o[0xA6] != 0 && t1 == 0) return 0;
    box[0][0] = *(s16 *)(o + 0x1C); box[0][1] = *(s16 *)(o + 0x20); box[0][2] = *(s16 *)(o + 0x24);
    box[1][0] = *(s16 *)(o + 0x1E); box[1][1] = *(s16 *)(o + 0x20); box[1][2] = *(s16 *)(o + 0x24);
    box[2][0] = *(s16 *)(o + 0x1E); box[2][1] = *(s16 *)(o + 0x20); box[2][2] = *(s16 *)(o + 0x26);
    box[3][0] = *(s16 *)(o + 0x1C); box[3][1] = *(s16 *)(o + 0x20); box[3][2] = *(s16 *)(o + 0x26);
    box[4][0] = *(s16 *)(o + 0x1C); box[4][1] = *(s16 *)(o + 0x22); box[4][2] = *(s16 *)(o + 0x24);
    box[5][0] = *(s16 *)(o + 0x1E); box[5][1] = *(s16 *)(o + 0x22); box[5][2] = *(s16 *)(o + 0x24);
    box[6][0] = *(s16 *)(o + 0x1E); box[6][1] = *(s16 *)(o + 0x22); box[6][2] = *(s16 *)(o + 0x26);
    box[7][0] = *(s16 *)(o + 0x1C); box[7][1] = *(s16 *)(o + 0x22); box[7][2] = *(s16 *)(o + 0x26);
    gte_SetRotMatrix(*(u8 **)(o + 0x90) + 0x14);
    dy = *(u16 *)(p + 0x2C);
    if (isSix) dy -= *(u16 *)(o + 0x22);
    for (i = 0; i < 8; i++) {
        gte_ldv0(box[i]);
        gte_rtv0();
        gte_stsv(box[i]);
        box[i][0] += *(s32 *)(p + 0x28);
        box[i][1] += dy;
        box[i][2] += *(s32 *)(p + 0x30);
    }
    gte_SetRotMatrix(D_800B0808);
    gte_SetTransMatrix(D_800B0808);
    o[0xA6] = 0;
    for (i = 0; i < 8; i = (s16)(i + 1)) {
        gte_ldv0(box[i]);
        gte_rtps();
        gte_stsxy(prj[i]);
        gte_stszotz(&z);
        gte_stflg(&flg);
        prj[i][2] = z;
        if (*(u8 **)(o + 0xB4) != 0) {
            *(B8 *)(*(u8 **)(o + 0xB4) + i * 8) = *(B8 *)prj[i];
        }
        if (o[0xA6] == 0) {
            if (flg != 0) {
                o[0xA6] = 1;
            } else if (prj[i][2] > 0 && prj[i][0] >= 0 && prj[i][0] <= 0x200 && prj[i][1] >= 0 && prj[i][1] <= 0xF0) {
                o[0xA6] = 1;
            }
        }
    }
    if (o[0xA6] != 0) return 0;
    for (i = 0; i < 4; i = (s16)(i + 1)) {
        switch (i) {
        case 0: ia = 3; ib = 5; break;
        case 1: ia = 4; ib = 2; break;
        case 2: ia = 7; ib = 1; break;
        default: ia = 0; ib = 6; break;
        }
        ea[0] = prj[ia][0];
        ea[1] = prj[ia][1];
        eb[0] = prj[ib][0];
        eb[1] = prj[ib][1];
        if (ea[0] > 0x200 && eb[0] > 0x200) continue;
        if (ea[0] < 0 && eb[0] < 0) continue;
        if (ea[1] < 0 && eb[1] < 0) continue;
        if (ea[1] > 0xF0 && eb[1] > 0xF0) continue;
        if (func_8003AA14(ea, eb) != 0) {
            o[0xA6] = 1;
            return 1;
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003AC1C);
#endif

void func_8003B4F4(void) {
    s16 i;

    for (i = 0; i < 30; i++) {
        M2C_FIELD(D_800D3288 + i * 0x70, u8 *, 0x65) = 0;
    }
}

#ifdef NON_MATCHING
void func_8003B53C(u8 *obj, s32 yaw, s32 pitch, s32 speed) {
    s32 t;
    s32 c1;
    s32 sn1;
    s32 c2;
    s32 x;
    s32 y;
    s32 z;
    s32 v;
    s32 d;
    u8 *s0;
    u8 *o;
    u8 *q;
    s32 dx;
    s32 dz;

    t = D_800A0370[yaw & 0xFFF];
    c1 = t >> 16;
    sn1 = (s16)t;
    t = D_800A0370[pitch & 0xFFF];
    c2 = t >> 16;
    x = (sn1 * c2) >> 12;
    y = -(s16)t;
    z = (c1 * c2) >> 12;
    M2C_FIELD(obj, s16 *, 0x98) = (x * speed) >> 12;
    M2C_FIELD(obj, s16 *, 0x9A) = (y * speed) >> 12;
    M2C_FIELD(obj, s16 *, 0x9C) = (z * speed) >> 12;
    if (M2C_FIELD(obj, u8 *, 5) == 1) {
        v = M2C_FIELD(obj, s16 *, 0x26);
        s0 = M2C_FIELD(obj, u8 **, 0x10C);
        M2C_FIELD(obj, s16 *, 0x52) = speed;
        M2C_FIELD(obj, s16 *, 0x52) = speed + M2C_FIELD(obj, u16 *, 0x26);
        M2C_FIELD(obj, s16 *, 0x3C) = (x * v) >> 12;
        M2C_FIELD(obj, s16 *, 0x3E) = (y * v) >> 12;
        M2C_FIELD(obj, s16 *, 0x40) = (z * v) >> 12;
        if (M2C_FIELD(s0, s16 *, 0x6E) == 0) {
            o = M2C_FIELD(obj, u8 **, 0x88);
            if (o != NULL && M2C_FIELD(o, u8 *, 5) == 6) {
                if (o == (u8 *)g_Player) {
                    o = &g_CamPosX;
                } else {
                    o = M2C_FIELD(o, u8 **, 0x90) + 0x28;
                }
                q = M2C_FIELD(obj, u8 **, 0x90);
                dx = M2C_FIELD(q, s32 *, 0x28) - M2C_FIELD(o, s32 *, 0);
                dz = M2C_FIELD(q, s32 *, 0x30) - M2C_FIELD(o, s32 *, 8);
                d = SquareRoot0(dx * dx + dz * dz);
                M2C_FIELD(s0, s16 *, 0x6E) = 1;
                M2C_FIELD(s0, s16 *, 0x68) = (x * d) >> 12;
                M2C_FIELD(s0, s16 *, 0x6A) = (y * d) >> 12;
                M2C_FIELD(s0, s16 *, 0x6C) = (z * d) >> 12;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003B53C);
#endif

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

#ifdef NON_MATCHING
void func_8003B808(u8 *arg0) {
    s32 v[8];
    u32 vol;
    s32 half;
    s32 r;
    s32 l;
    s32 a;
    s32 b;

    v[5] = *(s32 *)(arg0 + 0x14) - g_Camera.posX;
    v[6] = *(s32 *)(arg0 + 0x18) - g_Camera.posY;
    v[7] = *(s32 *)(arg0 + 0x1C) - g_Camera.posZ;
    vol = func_8003B750(v);
    if (vol == 0) {
        g_Args->pad12E[9] = 0;
        g_Args->pad12E[8] = 0;
        return;
    }
    half = vol >> 1;
    r = ((half * *(s16 *)&D_800A0370[(((s32 (*)(s32, s32))ratan2)(v[5], v[7]) - *(u16 *)&D_8009CD30) & 0xFFF]) >> 12) + half;
    l = vol;
    l -= r;
    l *= 2;
    r *= 2;
    if (l >= 0) {
        a = 0x7F;
        if (l < 0x80) {
            a = l;
        }
    } else {
        a = 0;
    }
    l = a;
    if (r >= 0) {
        a = 0x7F;
        if (r < 0x80) {
            a = r;
        }
    } else {
        a = 0;
    }
    g_Args->pad12E[9] = l;
    g_Args->pad12E[8] = a;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003B808);
#endif

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

void func_8003BC98(void *arg0, void *arg1, s32 arg2) {
    s32 t;
    s32 i;
    s32 sx;
    s32 cx;
    s32 sy;
    s32 cy;
    s32 sz;
    s32 cz;
    s32 ssx;
    s32 csx;
    s16 *m;

    m = arg1;
    i = ((u16 *)arg0)[0] & 0xFFF;
    t = D_800A0370[i];
    cx = t >> 16;
    sx = (s16)t;
    i = ((u16 *)arg0)[1] & 0xFFF;
    t = D_800A0370[i];
    cy = t >> 16;
    sy = (s16)t;
    i = ((u16 *)arg0)[2] & 0xFFF;
    t = D_800A0370[i];
    cz = t >> 16;
    sz = (s16)t;
    ssx = (sz * sx) >> 12;
    csx = (cz * sx) >> 12;
    *(s32 *)m = 0;
    m[7] = -sx;
    m[10] = (cx * cy) >> 12;
    m[4] = (cx * sy) >> 12;
    m[8] = (ssx * cy - cz * sy) >> 12;
    m[2] = (ssx * sy + cz * cy) >> 12;
    m[5] = (sz * cx) >> 12;
    m[9] = (csx * cy + sz * sy) >> 12;
    m[3] = (csx * sy - sz * cy) >> 12;
    m[6] = (cz * cx) >> 12;
    if (arg2 != 0) {
        *(s32 *)(arg2 + 0x88) |= 1;
    }
}

void func_8003BE68(s16 *a, s16 *b) {
  s16 t;
  s16 v;
  u8 *new_var;
  s32 s;
  s16 r;
  s16 new_var2;
  new_var2 = a[5];
  t = new_var2;
  if (t < (-0x1000))
  {
    v = -0x1000;
  }
  else
    if (t > 0x1000)
  {
    v = 0x1000;
  }
  else
  {
    v = new_var2;
  }
  a[5] = v;
  r = func_8006B274((s16) (-((s32) v)));
  b[0] = r;
  new_var = (u8 *) D_800A0370;
  s = *((s16 *) ((new_var + ((r & 0xFFF) * 4)) + 2));
  if (abs(s) < 2)
  {
    b[2] = 0;
    b[1] = ((s32 (*)(s32, s32)) ratan2)(-a[6], a[0]);
    return;
  }
  b[1] = ((s32 (*)(s32, s32)) ratan2)(a[2], a[8]);
  b[2] = ((s32 (*)(s32, s32)) ratan2)((a[3] << 12) / s, (a[4] << 12) / s);
}

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

u8 *func_8003C1BC(SpawnEnt *p, s32 x, s32 y, s32 z) {
    u32 flags;
    SpawnInfo *pf;
    s32 type;
    s32 w4;
    u32 w5;
    u8 *def;
    Rec20 *e;
    SpawnRec *r;
    SmartObj *obj;
    s16 i;
    u32 t1;
    u32 t2;
    u8 kind;
    s32 scale;
    s32 rot;
    s16 b8;
    u32 v;
    u32 n;

    flags = p->info.flags;
    pf = &p->info;
    if (!((flags >> 8) & 1)) {
    type = flags & 0xFF;
    if (type == 0) {
        return NULL;
    }
    w4 = p->info.w4;
    w5 = p->info.w5;
    D_1F8003D4 = x;
    D_1F8003D8 = y;
    D_1F8003DC = z;
    def = (u8 *)(type * 0x70 + g_Args->unk228);
    if (def[2] == 0x22) {
        t2 = p->info.flags;
        func_8003FCE4(type, pf, (w4 << 5) & 0xF00, (t2 >> 14) & 0xF, (t2 >> 10) & 0xF00, (t2 >> 22) & 0x1F);
        p->info.flags |= 0x100;
        return NULL;
    }
    if (type == D_8009D31C) {
        r = (SpawnRec *)func_800333D4();
        if (r == NULL) {
            return NULL;
        }
        r->x = x;
        r->z = z;
        r->src = pf;
        r->active = 1;
        r->unkC = 0;
        r->y = D_1F8003D8;
        r->f1 = (w5 >> 12) & 1;
        r->f3 = (w5 >> 13) & 7;
        n = (w5 >> 16) & 0xF;
        r->f6 = n;
        r->f10 = (n == 0);
        r->f11 = (w4 >> 7) & 7;
        p->info.flags |= 0x100;
    } else if (type == D_8009D320) {
        t1 = p->info.flags;
        for (i = 0; i < 5; i++) {
            e = &D_800B0CB0[i];
            if (e->state == 0) {
                e->state = 2;
                e->unk4 = 1000;
                e->unk6 = 0x1800;
                D_800B0CA8 = t1 >> 8;
                e->x = D_1F8003D4;
                e->z = D_1F8003DC;
                e->y = D_1F8003D8;
                e->unk14 = e->unk16 = e->unk18 = 0;
                D_800B0CA0 = t1 >> 16;
                D_800B0CA4 = t1 >> 24;
                pf->flags |= 0x100;
                e->src = pf;
                return NULL;
            }
        }
        return NULL;
    } else {
    kind = def[1];
    obj = (SmartObj *)func_8003C6B0(0, type, kind);
    scale = w4 & 7;
    if (obj != NULL) {
        goto have;
    }
    if (g_Args->cdArchive == 0 && kind != 6 && (u32)D_8009CDA8 % 90 == 0) {
        sprintf(D_800B0BC0, D_800A7F5C, type);
        func_8006B1DC((s32)D_800B0BC0);
    }
    }
    }
    return NULL;
have:
    p->info.flags |= 0x100;
    obj->src = pf;
    if (w4 < 0) {
        obj->flags48 &= ~2;
    }
    if ((flags >> 9) & 1) {
        obj->stateA4 = 2;
    }
    obj->path = ((u32)w4 >> 7) & 0x1F;
    obj->f68 = ((u32)w4 >> 24) & 7;
    rot = ((u32)w4 >> 3) & 0xF;
    if (kind == 4) {
        obj->unk34 = (flags >> 14) & 0x7F;
    }
    b8 = ((u32)w4 >> 12) & 0xFF;
    if (obj->type5 == 6) {
        obj->idx = obj - ((SmartObj *)D_800AFFC8 + D_800B0DA0);
        v = (u32)w4 >> 20;
        v &= 7;
        func_80053790((u8 *)obj, v);
    }
    if (obj->type5 == 3) {
        obj->unkCC = (flags >> 23) & 7;
        obj->unkCD = (flags >> 26) & 7;
    }
    if (scale != 0) {
        obj->scale = scale << 9;
        func_8002DD6C((u8 *)obj);
    }
    if (rot != 0) {
        *(s16 *)(obj->parts + 0x62) = rot << 8;
    }
    if (b8 != 0) {
        obj->unk8 = b8;
    }
    return (u8 *)obj;
}

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

void func_8003C6E4(void) {
  s16 sv[3];
  s32 mask;
  SpawnEnt **pp;
  int new_var;
  s32 *cp;
  s32 fx;
  s32 fy;
  s32 fz;
  s32 k;
  s32 j;
  SpawnEnt *e;
  s32 t;
  pp = D_8009CF70;
  cp = D_8009CF6C;
  fx = D_800D2FB0.x;
  fy = D_800D2FB0.y;
  fz = D_800D2FB0.z;
  k = 0;
  new_var = 0;
  if (D_8009CF68 > new_var)
  {
    mask = ~0x3C00;
    do
    {
      e = *pp;
      j = 0;
      if ((*cp) > 0)
      {
        do
        {
          s32 nf;
          t = e->x + fx;
          if (abs(t) < 0x7FFF)
          {
            sv[0] = t;
            t = e->y + fy;
            if (abs(t) < 0x7FFF)
            {
              sv[1] = t;
              t = e->z + fz;
              if (abs(t) < 0x7FFF)
              {
                sv[2] = t;
                if (func_800705A8(D_800E2F54, sv, 0, 0) != (-0x7FFF))
                {
                  e->info.flags = (e->info.flags & mask) | 0x400;
                  goto st2;
                }
              }
            }
          }
          nf = e->info.flags & mask;
          st:
          e->info.flags = nf;

          st2:
          j++;

          e++;
        }
        while (j < (*cp));
      }
      k++;
      pp++;
      cp++;
    }
    while (k < D_8009CF68);
  }
}

s32 func_8003C88C(s32 arg0, s32 arg1) {
    M2C_FIELD(((arg0 << 5) + &D_800D2E84), s16 *, 0x1C) = (s16) arg1;
    return (s32) &D_800D2E84;
}

s32 func_8003C8A4(u8 *a, u8 *b) {
    u8 *pb;
    s32 *pos;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 by;
    s32 top;

    pb = *(u8 **)(b + 0x90);
    pos = (s32 *)(*(u8 **)(a + 0x90) + 0x28);
    dx = *(s32 *)(pb + 0x28) - pos[0];
    dy = *(s32 *)(pb + 0x2C) - pos[1];
    dz = *(s32 *)(pb + 0x30) - pos[2];
    by = *(s32 *)(pb + 0x2C);
    if (b == g_Player) {
        top = g_CamPosY;
    } else {
        top = by - (*(s16 *)(b + 0x22) - *(s16 *)(b + 0x20));
    }
    if (by < pos[1] - (*(s16 *)(a + 0x22) - *(s16 *)(a + 0x20)) || pos[1] < top) {
        return 0;
    }
    if (dy > 0x200 && abs(dx) < *(u16 *)(a + 0x28) + 500 && abs(dz) < *(u16 *)(a + 0x28) + 500) {
        return 1;
    }
    if (abs(dx) < *(u16 *)(a + 0x2A) + 400 && abs(dz) < *(u16 *)(a + 0x2A) + 400) {
        return 1;
    }
    return 0;
}

s32 func_8003C9C4(u8 *a, u8 *b) {
    VECTOR *pa;
    VECTOR *pb;
    s32 dx;
    s32 dy;
    s32 dz;

    if (*(s32 *)(a + 0x4C) & 0x02000000) {
        return 0;
    }
    pa = (VECTOR *)(*(u8 **)(a + 0x90) + 0x28);
    pb = (VECTOR *)(*(u8 **)(b + 0x90) + 0x28);
    dx = pa->vx - pb->vx;
    if (dx < 0) dx = -dx;
    dy = pa->vy - pb->vy;
    if (dy < 0) dy = -dy;
    dz = pa->vz - pb->vz;
    if (dz < 0) dz = -dz;
    if (dx < dy) {
        dx = dy;
    }
    if (dx < dz) {
        dx = dz;
    }
    return 0;
}

void func_8003CA4C(void) {
    s32 s;

    if (rand() & 1) {
        s = rand() & 1;
        if ((rand() & 1) && s != 0) {
            s = -s;
        }
        M2C_FIELD(g_Args, s16 *, 0x134) = s;
        g_Args->pad12E[0xA] = 0x7F;
        func_80026F30(0x27, 6);
    }
}

s32 func_8003CABC(u8 *a, u8 *b) {
    u8 *pa;
    u8 *pb;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 i;
    s32 sq;

    pa = *(u8 **)(a + 0x90);
    pb = *(u8 **)(b + 0x90);
    dx = *(s32 *)(pa + 0x28) - *(s32 *)(pb + 0x28);
    dz = *(s32 *)(pa + 0x30) - *(s32 *)(pb + 0x30);
    dist = SquareRoot0(dx * dx + dz * dz);
    i = (*(s16 *)(*(u8 **)(b + 0x90) + 0x62) - ((s32 (*)(s32, s32))ratan2)(dx, dz)) & 0xFFF;
    dx = (dist * (s16)D_800A0370[i]) >> 12;
    dz = (dist * (D_800A0370[i] >> 16)) >> 12;
    if (dz < 0) {
        if (*(u16 *)(a + 0x2A) < dist) {
            if (a == g_Player) {
                if (dist < *(u16 *)(a + 0x2A) + 500) {
                    func_8003CA4C();
                }
            }
            return 0;
        }
        return 1;
    }
    if (dz <= *(s16 *)(b + 0x52)) {
        if (dx < 0) {
            dx = -dx;
        }
        if (*(u16 *)(a + 0x2A) < dx) {
            if (a == g_Player) {
                if (dx < *(u16 *)(a + 0x2A) + 500) {
                    func_8003CA4C();
                }
            }
            return 0;
        }
        return 1;
    }
    dist = dz - *(s16 *)(b + 0x52);
    sq = dx * dx + dist * dist;
    if (*(u16 *)(a + 0x2A) * *(u16 *)(a + 0x2A) < sq) {
        if (a == g_Player) {
            if (sq < (*(u16 *)(a + 0x2A) + 500) * (*(u16 *)(a + 0x2A) + 500)) {
                func_8003CA4C();
            }
        }
        return 0;
    }
    return 1;
}

#ifdef NON_MATCHING
s32 func_8003CC8C(u8 *arg0, u8 *arg1) {
    extern R8 D_800D4050_r asm("D_800D4050");
    s32 a[4];
    s32 b[4];
    u16 mn[4];
    u16 mx[4];
    u16 p0[4];
    u16 p1[4];
    u16 q[4];
    s32 mat[8];
    s32 *tri;
    u8 *rec;
    u8 *s1;
    s32 flags;
    s32 inRange;
    s32 n;
    s32 best;
    s32 v;
    u16 cnt;
    s8 i;
    u8 near;
    u8 isTgt;
    u8 dflag;
    u8 s2;
    s16 hi;
    u16 s3, s4, s5, s6, s7, fp;

    if ((g_GodMode != 0 || D_8009CF54 != 0) && arg0 == g_Player) return 0;
    if (func_8003CABC(arg0, arg1) == 0) return 0;
    tri = (s32 *)(*(u8 **)(arg1 + 0x90) + 0x28);
    if (arg0[5] == 6) {
        if (arg0 != g_Player) {
            func_8006B230(0, D_800A7F80);
        }
    }
    flags = *(s32 *)(arg0 + 0x4C);
    isTgt = 0;
    inRange = (flags & 0x300100) != 0;
    if (flags & 0x100) {
        isTgt = *(void **)(arg1 + 0x88) == g_Player;
    }
    if (inRange) {
        near = 1;
        if (isTgt != 0 || arg0[0xB] == 1) {
            dflag = 0;
        } else {
            dflag = 1;
        }
        hi = arg0[0xB] - 1;
    } else {
        near = 0;
        dflag = 0;
        hi = 0;
    }
    b[0] = tri[0];
    b[1] = tri[1];
    b[2] = tri[2];
    i = dflag;
    n = 0;
    if (hi >= (u8)i) {
        rec = D_800D4264;
        do {
            s1 = *(u8 **)(arg0 + 0x90) + i * 0x8C;
            if (n == 6) {
                func_8006B230(0, D_800A7FA0);
            }
            rec[0x18] = i;
            n++;
            *(s32 *)(rec + 8) = b[0] - *(s32 *)(s1 + 0x48);
            i++;
            *(s32 *)(rec + 0xC) = b[1] - *(s32 *)(s1 + 0x4C);
            *(s32 *)(rec + 0x10) = b[2] - *(s32 *)(s1 + 0x50);
            rec += 0x1C;
        } while (hi >= i);
    }
    cnt = 0;
    i = 0;
    if (n > 0) {
        do {
            rec = D_800D4264 + i * 0x1C;
            a[0] = *(s32 *)(rec + 8);
            if ((u32)(a[0] + 0x8000) <= 0x10000) {
                a[0] += *(s16 *)(arg1 + 0x98);
                if ((u32)(a[0] + 0x8000) <= 0x10000) {
                    a[1] = *(s32 *)(rec + 0xC);
                    if ((u32)(a[1] + 0x8000) <= 0x10000) {
                        a[1] += *(s16 *)(arg1 + 0x9A);
                        if ((u32)(a[1] + 0x8000) <= 0x10000) {
                            a[2] = *(s32 *)(rec + 0x10);
                            if ((u32)(a[2] + 0x8000) <= 0x10000) {
                                a[2] += *(s16 *)(arg1 + 0x9C);
                                if ((u32)(a[2] + 0x8000) <= 0x10000) {
                                    s2 = rec[0x18];
                                    s1 = *(u8 **)(arg0 + 0x90) + s2 * 0x8C;
                                    if (*(s32 *)s1 >= 0 || arg0 == g_Player) {
                                        if (s2 >= 6) {
                                            func_8006B230(0, D_800A7FC8);
                                        }
                                        if (near != 0) {
                                            u8 *t = (u8 *)D_800D86E0 + (*(u8 **)arg0)[0x5F] * 0x7E + s2 * 2;
                                            s3 = *(u16 *)(t + 0x2A);
                                            s6 = *(u16 *)(t + 0x36);
                                            s4 = *(u16 *)(t + 0x42);
                                            s7 = *(u16 *)(t + 0x4E);
                                            s5 = *(u16 *)(t + 0x5A);
                                            fp = *(u16 *)(t + 0x66);
                                        } else {
                                            s3 = *(u16 *)(arg0 + 0x1C);
                                            s6 = *(u16 *)(arg0 + 0x1E);
                                            s4 = *(u16 *)(arg0 + 0x20);
                                            s7 = *(u16 *)(arg0 + 0x22);
                                            s5 = *(u16 *)(arg0 + 0x24);
                                            fp = *(u16 *)(arg0 + 0x26);
                                        }
                                        v = *(s16 *)(arg0 + 0x6A);
                                        if (v != 0) {
                                            s3 = ((s16)s3 * v) >> 12;
                                            s6 = ((s16)s6 * v) >> 12;
                                            s4 = ((s16)s4 * v) >> 12;
                                            s7 = ((s16)s7 * v) >> 12;
                                            s5 = ((s16)s5 * v) >> 12;
                                            fp = ((s16)fp * v) >> 12;
                                        }
                                        TransposeMatrix(s1 + 0x34, mat);
                                        gte_SetRotMatrix(mat);
                                        rec = D_800D4264 + i * 0x1C;
                                        p0[0] = *(u16 *)(rec + 8);
                                        p0[1] = *(u16 *)(rec + 0xC);
                                        p0[2] = *(u16 *)(rec + 0x10);
                                        p1[0] = p0[0] + *(s16 *)(arg1 + 0x98);
                                        p1[1] = p0[1] + *(s16 *)(arg1 + 0x9A);
                                        p1[2] = p0[2] + *(s16 *)(arg1 + 0x9C);
                                        p0[0] -= *(u16 *)(*(u8 **)(arg1 + 0x10C) + 0x68);
                                        p0[1] -= *(u16 *)(*(u8 **)(arg1 + 0x10C) + 0x6A);
                                        p0[2] -= *(u16 *)(*(u8 **)(arg1 + 0x10C) + 0x6C);
                                        p1[0] += *(u16 *)(arg1 + 0x3C);
                                        p1[1] += *(u16 *)(arg1 + 0x3E);
                                        p1[2] += *(u16 *)(arg1 + 0x40);
                                        gte_ldv0(p0);
                                        gte_rtv0();
                                        gte_stlvnl(a);
                                        gte_ldv0(p1);
                                        gte_rtv0();
                                        gte_stlvnl(b);
                                        mn[0] = s3;
                                        mn[1] = s4;
                                        mn[2] = s5;
                                        mx[0] = s6;
                                        mx[1] = s7;
                                        mx[2] = fp;
                                        if (func_8003EC44(a, b, mn, mx) != 0) {
                                            if (isTgt != 0 && !(*(u16 *)(arg1 + 0xE) & 0x100)) {
                                                D_800D4050_r = func_8003D76C((VECTOR *)a, (VECTOR *)b, arg0, s2);
                                            } else {
                                                D_800D4050_r = func_8003E158((s32)a, (s32)b, (SVECTOR *)mn, (SVECTOR *)mx);
                                            }
                                            if (*(s16 *)(D_800D4050 + 3) != 1) {
                                                rec = D_800D430C + (s16)cnt * 0x14;
                                                *(s16 *)(rec + 0x12) = s2;
                                                *(u16 *)(rec + 0) = D_800D4050[0];
                                                *(u16 *)(rec + 2) = D_800D4050[1];
                                                *(u16 *)(rec + 4) = D_800D4050[2];
                                                *(u16 *)(rec + 6) = D_800D4058[0];
                                                *(u16 *)(rec + 8) = D_800D4058[1];
                                                cnt++;
                                                *(u16 *)(rec + 0xA) = D_800D4058[2];
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            i++;
        } while (i < n);
    }
    if ((s16)cnt > 0) {
        s8 sel = cnt - 1;
        if ((s16)cnt >= 2) {
            best = -1;
            sel = -1;
            i = 0;
            if ((s16)cnt > 0) {
                do {
                    rec = D_800D430C + i * 0x14;
                    *(u16 *)(D_1F8003E0 + 0) = *(u16 *)(rec + 0);
                    *(u16 *)(D_1F8003E0 + 2) = *(u16 *)(rec + 2);
                    *(u16 *)(D_1F8003E0 + 4) = *(u16 *)(rec + 4);
                    s1 = *(u8 **)(arg0 + 0x90) + *(s16 *)(rec + 0x12) * 0x8C;
                    gte_SetRotMatrix(s1 + 0x34);
                    gte_ldv0(D_1F8003E0);
                    gte_rtv0();
                    gte_stsv(D_1F8003E0);
                    *(u16 *)(D_1F8003E0 + 0) += *(u16 *)(s1 + 0x48);
                    *(u16 *)(D_1F8003E0 + 2) += *(u16 *)(s1 + 0x4C);
                    *(u16 *)(D_1F8003E0 + 4) += *(u16 *)(s1 + 0x50);
                    q[0] = tri[0];
                    q[1] = tri[1];
                    q[2] = tri[2];
                    v = func_8006D3C0((SVECTOR *)D_1F8003E0, (SVECTOR *)q);
                    if (best == -1 || v < best) {
                        best = v;
                        sel = i;
                    }
                    i++;
                } while (i < (s16)cnt);
            }
        }
        rec = D_800D430C + sel * 0x14;
        *(s32 *)(arg0 + 0x64) = (*(s32 *)(arg0 + 0x64) & 0xFFE0FFFF) | ((rec[0x12] & 0x1F) << 16);
        D_800D4050[0] = *(u16 *)(rec + 0);
        D_800D4050[1] = *(u16 *)(rec + 2);
        D_800D4050[2] = *(u16 *)(rec + 4);
        D_800D4058[0] = *(u16 *)(rec + 6);
        D_800D4058[1] = *(u16 *)(rec + 8);
        D_800D4058[2] = *(u16 *)(rec + 0xA);
        if (D_8009D8D5 != 0 && (arg1 == g_Player || *(void **)(arg1 + 0x88) == g_Player)) {
            D_8009D8D8 = (s32)arg0;
            func_80058F28(arg0);
            D_800DD838 = arg0[6];
            D_8009D8D6 = *(u16 *)(arg0 + 0x66) & 0x1F;
        }
        return 1;
    }
    if (arg0 == g_Player) {
        func_8003CA4C();
    }
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003CC8C);
#endif

#ifdef NON_MATCHING
R8 func_8003D76C(VECTOR *arg1, VECTOR *arg2, u8 *arg3, s32 arg4) {
    s32 vv[4][4];       /* triangle vertices 0..2, then the hit point */
    R8 resr;            /* "no hit" result */
    R8 hitr;
    SVECTOR *res = (SVECTOR *) &resr;
    SVECTOR *hit = (SVECTOR *) &hitr;
    SVECTOR minv;
    SVECTOR maxv;
    u16 i0;
    u16 i1;
    u16 i2;
    u8 *rec;
    u8 *face;
    u8 *verts;
    u8 *obj;
    u32 count;
    u32 n;
    u32 cmd;
    s32 type;
    s32 quad;
    s32 stride;
    s32 scale;
    s32 dot0;
    s32 dot1;
    s32 dot2;
    s16 dx;
    s16 dy;
    s16 dz;
    SVECTOR *sv;

    memset(&resr, 0, 8);
    i0 = 0;
    i1 = 0;
    i2 = 0;
    stride = 0;
    obj = M2C_FIELD(M2C_FIELD(arg3, u8 **, 0x90), u8 **, 8);
    if (!(obj[-8] & 1)) {
        func_8006B230(0, (s32) D_800A7FF8);
    }
    rec = obj + (s16) arg4 * 0x1C;
    verts = M2C_FIELD(rec, u8 **, 0);
    count = M2C_FIELD(rec, u32 *, 0x14);
    face = M2C_FIELD(rec, u8 **, 0x10);
    if (count != 0) {
        sv = (SVECTOR *) D_1F8003E0;
        scale = (s16) M2C_FIELD(arg3, u16 *, 0x6A);
        n = 0;
        do {
            cmd = M2C_FIELD(face, u32 *, 0);
            type = cmd >> 24;
            quad = (cmd >> 18) & 1;
            switch (type) {
            case 0x35:
                i0 = M2C_FIELD(face, u16 *, 0x1C);
                i1 = M2C_FIELD(face, u16 *, 0x1E);
                i2 = M2C_FIELD(face, u16 *, 0x20);
                stride = 9;
                break;
            case 0x25:
                i0 = M2C_FIELD(face, u16 *, 0x14);
                i1 = M2C_FIELD(face, u16 *, 0x16);
                i2 = M2C_FIELD(face, u16 *, 0x18);
                stride = 7;
                break;
            case 0x20:
                if (quad) {
                    stride = 6;
                    i0 = M2C_FIELD(face, u16 *, 0xA);
                    i1 = M2C_FIELD(face, u16 *, 0xC);
                    i2 = M2C_FIELD(face, u16 *, 0xE);
                } else {
                    stride = 4;
                    i0 = M2C_FIELD(face, u16 *, 0x12);
                    i1 = M2C_FIELD(face, u16 *, 0x14);
                    i2 = M2C_FIELD(face, u16 *, 0x16);
                }
                break;
            case 0x24:
            case 0x26:
                stride = 6;
                i0 = M2C_FIELD(face, u16 *, 0x12);
                i1 = M2C_FIELD(face, u16 *, 0x14);
                i2 = M2C_FIELD(face, u16 *, 0x16);
                break;
            case 0x30:
                if (!quad) {
                    i0 = M2C_FIELD(face, u16 *, 0xA);
                    i1 = M2C_FIELD(face, u16 *, 0xE);
                    i2 = M2C_FIELD(face, u16 *, 0x12);
                    stride = 5;
                    break;
                }
                /* fall through */
            case 0x34:
            case 0x36:
                i0 = M2C_FIELD(face, u16 *, 0x12);
                i1 = M2C_FIELD(face, u16 *, 0x16);
                i2 = M2C_FIELD(face, u16 *, 0x1A);
                stride = 7;
                break;
            case 0x21:
                i0 = M2C_FIELD(face, u16 *, 8);
                i1 = M2C_FIELD(face, u16 *, 0xA);
                i2 = M2C_FIELD(face, u16 *, 0xC);
                stride = 4;
                break;
            case 0x31:
                i0 = M2C_FIELD(face, u16 *, 0x10);
                i1 = M2C_FIELD(face, u16 *, 0x12);
                i2 = M2C_FIELD(face, u16 *, 0x14);
                stride = 6;
                break;
            default:
                sprintf(g_Args->exePath, D_800A8000, type, arg3[6]);
                func_8006B230(0, (s32) g_Args->exePath);
                break;
            }
            LOADV(i0, vv[0]);
            LOADV(i1, vv[1]);
            LOADV(i2, vv[2]);
            hitr = func_8003E5A0((s32) arg1, (s32) arg2, (VECTOR *) vv[0], (VECTOR *) vv[2], (VECTOR *) vv[1]);
            if (hit->pad != 1) {
                dx = arg2->vx - arg1->vx;
                dot0 = dx * D_800D4058[0];
                sv->vx = dx;
                dy = arg2->vy - arg1->vy;
                dot1 = dy * D_800D4058[1];
                sv->vy = dy;
                dz = arg2->vz - arg1->vz;
                dot2 = dz * D_800D4058[2];
                sv->vz = dz;
                *(s32 *) 0x1F8003E8 = dot0;
                *(s32 *) 0x1F800004 = dot1;
                *(s32 *) 0x1F800008 = dot2;
                if ((dot0 + dot1 + dot2) <= 0) {
                    vv[3][0] = hit->vx;
                    vv[3][1] = hit->vy;
                    vv[3][2] = hit->vz;
                    if ((s32) (u16) arg2->vx < (s32) (u16) arg1->vx) {
                        maxv.vx = (u16) arg1->vx;
                        minv.vx = (u16) arg2->vx;
                    } else {
                        minv.vx = (u16) arg1->vx;
                        maxv.vx = (u16) arg2->vx;
                    }
                    if ((s32) (u16) arg2->vy < (s32) (u16) arg1->vy) {
                        maxv.vy = (u16) arg1->vy;
                        minv.vy = (u16) arg2->vy;
                    } else {
                        minv.vy = (u16) arg1->vy;
                        maxv.vy = (u16) arg2->vy;
                    }
                    if ((s32) (u16) arg2->vz < (s32) (u16) arg1->vz) {
                        maxv.vz = (u16) arg1->vz;
                        minv.vz = (u16) arg2->vz;
                    } else {
                        minv.vz = (u16) arg1->vz;
                        maxv.vz = (u16) arg2->vz;
                    }
                    if ((vv[3][0] >= minv.vx) && (vv[3][0] <= maxv.vx) &&
                        (vv[3][1] >= minv.vy) && (vv[3][1] <= maxv.vy) &&
                        (vv[3][2] >= minv.vz) && (vv[3][2] <= maxv.vz) &&
                        (func_80040778(vv[0], vv[2], vv[1], vv[3]) != 0)) {
                        hit->pad = 0;
                        return hitr;
                    }
                }
            }
            face += stride * 4;
            n++;
        } while (n < count);
    }
    res->pad = 1;
    return resr;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003D76C);
#endif

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

R8 func_8003E158(s32 arg1, s32 arg2, SVECTOR *P, SVECTOR *Q) {
    VECTOR v[3];
    R8 r;
    R8 z;
    s8 t;
    s8 u;
    memset(&z, 0, 8);
    t = func_8003E494();
    D_8009D8D4 = t;
    u = t + 1;
    switch (u) {
    case 2:
        v[0].vx = P->vx;
        v[0].vy = Q->vy;
        v[0].vz = P->vz;
        v[1].vx = Q->vx;
        v[1].vy = Q->vy;
        v[1].vz = P->vz;
        v[2].vx = P->vx;
        v[2].vy = P->vy;
        v[2].vz = P->vz;
        break;
    case 1:
        v[0].vx = Q->vx;
        v[0].vy = Q->vy;
        v[0].vz = Q->vz;
        v[1].vx = P->vx;
        v[1].vy = Q->vy;
        v[1].vz = Q->vz;
        v[2].vx = Q->vx;
        v[2].vy = P->vy;
        v[2].vz = Q->vz;
        break;
    case 3:
        v[0].vx = Q->vx;
        v[0].vy = Q->vy;
        v[0].vz = P->vz;
        v[1].vx = Q->vx;
        v[1].vy = Q->vy;
        v[1].vz = Q->vz;
        v[2].vx = Q->vx;
        v[2].vy = P->vy;
        v[2].vz = P->vz;
        break;
    case 4:
        v[0].vx = P->vx;
        v[0].vy = Q->vy;
        v[0].vz = Q->vz;
        v[1].vx = P->vx;
        v[1].vy = Q->vy;
        v[1].vz = P->vz;
        v[2].vx = P->vx;
        v[2].vy = P->vy;
        v[2].vz = Q->vz;
        break;
    case 5:
        v[0].vx = P->vx;
        v[0].vy = P->vy;
        v[0].vz = P->vz;
        v[1].vx = Q->vx;
        v[1].vy = P->vy;
        v[1].vz = P->vz;
        v[2].vx = P->vx;
        v[2].vy = P->vy;
        v[2].vz = Q->vz;
        break;
    case 6:
        v[0].vx = P->vx;
        v[0].vy = Q->vy;
        v[0].vz = Q->vz;
        v[1].vx = Q->vx;
        v[1].vy = Q->vy;
        v[1].vz = Q->vz;
        v[2].vx = P->vx;
        v[2].vy = Q->vy;
        v[2].vz = P->vz;
        break;
    case 0:
        return z;
    }
    r = func_8003E5A0(arg1, arg2, &v[0], &v[1], &v[2]);
    return r;
}

s32 func_8003E494(void) {
    if (D_800D438A == 1 && D_800D438F == D_800D438A) {
        return 1;
    }
    if (D_800D4388 == 1 && D_800D438D == D_800D4388) {
        return 0;
    }
    if (D_800D4385 == 1 && D_800D4389 == D_800D4385) {
        return 2;
    }
    if (D_800D4387 == 1 && D_800D438B == D_800D4387) {
        return 3;
    }
    if (D_800D4384 == 1 && D_800D438C == D_800D4384) {
        return 4;
    }
    if (D_800D4386 == 1 && D_800D438E == D_800D4386) {
        return 5;
    }
    return -1;
}

#ifdef NON_MATCHING
R8 func_8003E5A0(VECTOR *o, VECTOR *e, VECTOR *p0, VECTOR *p1, VECTOR *p2) {
    VECTOR d1;
    VECTOR d2;
    SVECTOR res;
    VECTOR n;
    SVECTOR zero;
    s32 lim;
    s32 dx, dy, dz, a2, a3;
    memset(&zero, 0, 8);
    d1.vx = p1->vx - p0->vx;
    d1.vy = p1->vy - p0->vy;
    d1.vz = p1->vz - p0->vz;
    d2.vx = p2->vx - p0->vx;
    d2.vy = p2->vy - p0->vy;
    d2.vz = p2->vz - p0->vz;
    gte_ldDiag(&d1);
    gte_ldlvl_zxy(&d2);
    gte_op0();
    gte_stlvnl(&n);
    D_800D4250[0] = n.vx;
    D_800D4250[1] = n.vy;
    D_800D4250[2] = n.vz;
    lim = 0xC350;
    while (abs(n.vx * o->vx) > lim || abs(n.vy * o->vy) > lim || abs(n.vz * o->vz) > lim) {
        n.vx >>= 1;
        n.vy >>= 1;
        n.vz >>= 1;
    }
    n.pad = -(n.vx * p0->vx + n.vy * p0->vy + n.vz * p0->vz);
    dx = e->vx - o->vx;
    dy = e->vy - o->vy;
    dz = e->vz - o->vz;
    a2 = -(n.vx * o->vx + n.vy * o->vy + n.vz * o->vz + n.pad);
    a3 = n.vx * dx + n.vy * dy + n.vz * dz;
    if (a3 == 0) {
        zero.pad = 1;
        return *(R8 *)&zero;
    }
    res.vx = o->vx + dx * a2 / a3;
    res.vy = o->vy + dy * a2 / a3;
    D_800D4058[0] = n.vx;
    D_800D4058[2] = n.vz;
    D_800D4058[1] = n.vy;
    res.pad = 0;
    res.vz = o->vz + dz * a2 / a3;
    return *(R8 *)&res;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003E5A0);
#endif

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

#ifdef NON_MATCHING
s32 func_8003EC44(s32 *arg0, s32 *arg1, s16 *arg2, s16 *arg3) {
    VECTOR v10;
    VECTOR v20;
    VECTOR v30;
    VECTOR v40;
    VECTOR v50;
    VECTOR v60;
    VECTOR v70;
    VECTOR v80;
    VECTOR v90;
    u8 fA0;
    u8 fA8;
    u8 sx0, sz0, sx1, sy0;
    u8 hit;

    M2C_FIELD(&D_800D4388, s8 *, 0) = 0;
    M2C_FIELD(&D_800D4388, s8 *, 1) = 0;
    M2C_FIELD(&D_800D4388, s8 *, 2) = 0;
    M2C_FIELD(&D_800D4388, s8 *, 3) = 0;
    M2C_FIELD(&D_800D4384, s8 *, 0) = 0;
    M2C_FIELD(&D_800D4384, s8 *, 1) = 0;
    M2C_FIELD(&D_800D4384, s8 *, 2) = 0;
    M2C_FIELD(&D_800D4384, s8 *, 3) = 0;
    M2C_FIELD(&D_800D438C, s8 *, 0) = 0;
    M2C_FIELD(&D_800D438C, s8 *, 1) = 0;
    M2C_FIELD(&D_800D438C, s8 *, 2) = 0;
    M2C_FIELD(&D_800D438C, s8 *, 3) = 0;
    /* in-range flags: *0 = start point (arg0), *1 = end point (arg1), per axis against box arg2..arg3 */
    sx0 = 0;
    if (arg0[0] >= arg2[0]) sx0 = arg3[0] >= arg0[0];
    sz0 = 0;
    if (arg0[2] >= arg2[2]) sz0 = arg3[2] >= arg0[2];
    sx1 = 0;
    if (arg1[0] >= arg2[0]) sx1 = arg3[0] >= arg1[0];
    fA8 = 0;
    if (arg1[2] >= arg2[2]) fA8 = arg3[2] >= arg1[2];
    sy0 = 0;
    if (arg0[1] >= arg2[1]) sy0 = arg3[1] >= arg0[1];
    fA0 = 0;
    if (arg1[1] >= arg2[1]) fA0 = arg3[1] >= arg1[1];
    v80.vx = (arg3[0] - arg2[0]) * 2;
    v80.vy = (arg3[1] - arg2[1]) * 2;
    v80.vz = (arg3[2] - arg2[2]) * 2;
    v90.vx = arg1[0] - arg0[0];
    v90.vy = arg1[1] - arg0[1];
    v90.vz = arg1[2] - arg0[2];
    /* pass 1: x/z plane (box edges as segments) */
    v10.vx = arg0[0];
    v10.vy = arg0[2];
    v20.vx = arg1[0];
    v20.vy = arg1[2];
    v30.vx = arg2[0];
    v30.vy = arg3[2];
    v40.vx = arg3[0];
    v40.vy = arg3[2];
    v50.vx = arg2[0];
    v50.vy = arg2[2];
    v60.vx = arg3[0];
    v60.vy = arg2[2];
    if (sx0) {
    hit = 0;
    if (sz0) {
        if (v90.vx > 0) {
            v70.vx = -v80.vx;
        } else if (v90.vx < 0) {
            v70.vx = v80.vx;
        } else {
            v70.vx = 0;
            if (v90.vz > 0) {
                v70.vz = -v80.vz;
            } else if (v90.vz < 0) {
                v70.vz = v80.vz;
            } else {
                v70.vz = 0;
            }
        }
        if (v70.vx != 0) {
            if (v90.vx == 0) {
                func_8006B230(0, (s32)D_800A80B0);
            }
            v70.vz = v70.vx * v90.vz / v90.vx;
        }
        v10.vx += v70.vx;
        v10.vy += v70.vz;
    }
    } else {
        hit = 0;
    }
    if (func_8003DE58(&v10, &v20, &v30, &v40) != 0) {
        hit = 1;
        if (v20.vy < v10.vy) {
            D_800D4388 = 1;
        } else {
            D_800D4388 = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v60, &v40) != 0) {
        hit = 1;
        if (v20.vx < v10.vx) {
            D_800D4389 = 1;
        } else {
            D_800D4389 = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v60, &v50) != 0) {
        hit = 1;
        if (v10.vy < v20.vy) {
            D_800D438A = 1;
        } else {
            D_800D438A = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v30, &v50) != 0) {
        if (v10.vx < v20.vx) {
            D_800D438B = 1;
        } else {
            D_800D438B = -1;
        }
        hit = 1;
    }
    if (hit == 0 && (sx1 == 0 || fA8 == 0)) return 0;
    /* pass 2: x/y plane */
    v10.vx = arg0[0];
    v10.vy = arg0[1];
    v20.vx = arg1[0];
    v20.vy = arg1[1];
    v30.vx = arg2[0];
    v30.vy = arg2[1];
    v40.vx = arg3[0];
    v40.vy = arg2[1];
    v50.vx = arg2[0];
    v50.vy = arg3[1];
    v60.vx = arg3[0];
    v60.vy = arg3[1];
    if (sx0) {
    hit = 0;
    if (sy0) {
        if (v90.vx > 0) {
            v70.vx = -v80.vx;
        } else if (v90.vx < 0) {
            v70.vx = v80.vx;
        } else {
            v70.vx = 0;
            if (v90.vy > 0) {
                v70.vy = -v80.vy;
            } else if (v90.vy < 0) {
                v70.vy = v80.vy;
            } else {
                v70.vy = 0;
            }
        }
        if (v70.vx != 0) {
            if (v90.vx == 0) {
                func_8006B230(0, (s32)D_800A80BC);
            }
            v70.vy = v70.vx * v90.vy / v90.vx;
        }
        v10.vx += v70.vx;
        v10.vy += v70.vy;
    }
    } else {
        hit = 0;
    }
    if (func_8003DE58(&v10, &v20, &v30, &v40) != 0) {
        hit = 1;
        if (v10.vy < v20.vy) {
            D_800D4384 = 1;
        } else {
            D_800D4384 = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v60, &v40) != 0) {
        hit = 1;
        if (v20.vx < v10.vx) {
            D_800D4385 = 1;
        } else {
            D_800D4385 = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v60, &v50) != 0) {
        hit = 1;
        if (v20.vy < v10.vy) {
            D_800D4386 = 1;
        } else {
            D_800D4386 = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v30, &v50) != 0) {
        if (v10.vx < v20.vx) {
            D_800D4387 = 1;
        } else {
            D_800D4387 = -1;
        }
        hit = 1;
    }
    if (hit == 0 && (sx1 == 0 || fA0 == 0)) return 0;
    /* pass 3: y/z plane */
    v10.vx = arg0[1];
    v10.vy = arg0[2];
    v20.vx = arg1[1];
    v20.vy = arg1[2];
    v30.vx = arg2[1];
    v30.vy = arg2[2];
    v40.vx = arg2[1];
    v40.vy = arg3[2];
    v50.vx = arg3[1];
    v50.vy = arg2[2];
    v60.vx = arg3[1];
    v60.vy = arg3[2];
    if (sy0) {
    hit = 0;
    if (sz0) {
        if (v90.vy > 0) {
            v70.vy = -v80.vy;
        } else if (v90.vy < 0) {
            v70.vy = v80.vy;
        } else {
            v70.vy = 0;
            if (v90.vz > 0) {
                v70.vz = -v80.vz;
            } else if (v90.vz < 0) {
                v70.vz = v80.vz;
            } else {
                v70.vz = 0;
            }
        }
        if (v70.vy != 0) {
            if (v90.vy == 0) {
                func_8006B230(0, (s32)D_800A80C8);
            }
            v70.vz = v70.vy * v90.vz / v90.vy;
        }
        v10.vx += v70.vy;
        v10.vy += v70.vz;
    }
    } else {
        hit = 0;
    }
    if (func_8003DE58(&v10, &v20, &v30, &v40) != 0) {
        hit = 1;
        if (v10.vx < v20.vx) {
            D_800D438C = 1;
        } else {
            D_800D438C = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v60, &v40) != 0) {
        hit = 1;
        if (v20.vy < v10.vy) {
            D_800D438D = 1;
        } else {
            D_800D438D = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v60, &v50) != 0) {
        hit = 1;
        if (v20.vx < v10.vx) {
            D_800D438E = 1;
        } else {
            D_800D438E = -1;
        }
    }
    if (func_8003DE58(&v10, &v20, &v30, &v50) != 0) {
        if (v10.vy < v20.vy) {
            D_800D438F = 1;
        } else {
            D_800D438F = -1;
        }
        hit = 1;
    }
    if (hit) return 1;
    return fA0 && fA8;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8003EC44);
#endif

s32 func_8003F768(VECTOR *p, VECTOR *q, SVECTOR *min, SVECTOR *max) {
    VECTOR a;
    VECTOR b;
    VECTOR c;
    VECTOR d;
    VECTOR e;
    VECTOR f;
    u8 pInX;
    u8 pInZ;
    u8 qInX;
    u8 qInZ;
    u8 pInY;
    u8 qInY;

    pInX = qInX = qInZ = 0;
    pInX = p->vx >= min->vx && p->vx <= max->vx;
    pInZ = p->vz >= min->vz && p->vz <= max->vz;
    if (!pInX || !pInZ) {
        qInX = q->vx >= min->vx && q->vx <= max->vx;
        qInZ = q->vz >= min->vz && q->vz <= max->vz;
        if (!qInX || !qInZ) {
            a.vx = p->vx;
            a.vy = p->vz;
            b.vx = q->vx;
            b.vy = q->vz;
            c.vx = min->vx;
            c.vy = max->vz;
            d.vx = max->vx;
            d.vy = max->vz;
            e.vx = min->vx;
            e.vy = min->vz;
            f.vx = max->vx;
            f.vy = min->vz;
            if (!func_8003DE58(&a, &b, &c, &d) && !func_8003DE58(&a, &b, &f, &d) &&
                !func_8003DE58(&a, &b, &f, &e) && !func_8003DE58(&a, &b, &c, &e)) {
                return 0;
            }
        }
    }
    pInY = p->vy >= min->vy && p->vy <= max->vy;
    qInY = q->vy >= min->vy && q->vy <= max->vy;
    if (!pInX || !pInY) {
        if (!qInX || !qInY) {
            a.vx = p->vx;
            a.vy = p->vy;
            b.vx = q->vx;
            b.vy = q->vy;
            c.vx = min->vx;
            c.vy = min->vy;
            d.vx = max->vx;
            d.vy = min->vy;
            e.vx = min->vx;
            e.vy = max->vy;
            f.vx = max->vx;
            f.vy = max->vy;
            if (!func_8003DE58(&a, &b, &c, &d) && !func_8003DE58(&a, &b, &f, &d) &&
                !func_8003DE58(&a, &b, &f, &e) && !func_8003DE58(&a, &b, &c, &e)) {
                return 0;
            }
        }
    }
    if ((pInY && pInZ) || (qInY && qInZ)) {
        return 1;
    }
    a.vx = p->vy;
    a.vy = p->vz;
    b.vx = q->vy;
    b.vy = q->vz;
    c.vx = min->vy;
    c.vy = min->vz;
    d.vx = min->vy;
    d.vy = max->vz;
    e.vx = max->vy;
    e.vy = min->vz;
    f.vx = max->vy;
    f.vy = max->vz;
    if (func_8003DE58(&a, &b, &c, &d)) return 1;
    if (func_8003DE58(&a, &b, &f, &d)) return 1;
    if (func_8003DE58(&a, &b, &f, &e)) return 1;
    return func_8003DE58(&a, &b, &c, &e) != 0;
}

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

void *func_8003FCE4(model, src, a2, kind, a4, a5)
s32 model; SpawnInfo *src; s16 a2; u8 kind; u16 a4; s32 a5; {
    Rec44 *e;
    s16 i;
    u8 *p;
    s32 t;

    if (D_8009CF7C == 0) {
        return NULL;
    }
    i = 0;
    e = (Rec44 *)D_8009CF7C;
    for (; i < D_8009CF80; i++, e++) {
        if (e->active == 0) {
            goto found;
        }
    }
    if (g_Args->cdArchive == 0) {
        sprintf(g_Args->exePath, (char *)&D_800A811C, model);
        func_8006B1DC((s32)g_Args->exePath);
    }
    return NULL;
found:
    e->active = 1;
    e->kind = kind;
    if (M2C_FIELD(model * 0x70 + g_Args->unk228, s32 *, 0x3C) & 2) {
        e->flag3 = 1;
    } else {
        e->flag3 = 0;
    }
    e->v[0] = M2C_FIELD(model * 0x70 + g_Args->unk228, u16 *, 0x10);
    e->v[1] = M2C_FIELD(model * 0x70 + g_Args->unk228, u16 *, 0x12);
    e->v[2] = M2C_FIELD(model * 0x70 + g_Args->unk228, u16 *, 0x14);
    e->v[3] = M2C_FIELD(model * 0x70 + g_Args->unk228, u16 *, 0x16);
    e->v[4] = M2C_FIELD(model * 0x70 + g_Args->unk228, u16 *, 0x18);
    e->v[5] = M2C_FIELD(model * 0x70 + g_Args->unk228, u16 *, 0x1A);
    e->x = D_1F8003D4;
    e->y = D_1F8003D8;
    e->z = D_1F8003DC;
    if (src != NULL) {
        src->flags |= 0x100;
        e->src = src;
        if (src->w4 < 0) {
            e->flag3 = 0;
        }
    }
    e->h20 = 0;
    e->h22 = a2;
    e->h24 = 0;
    e->h30 = a4;
    e->w28 = M2C_FIELD(model * 0x70 + g_Args->unk228, s32 *, 0x38);
    e->h32 = M2C_FIELD(model * 0x70 + g_Args->unk228, u16 *, 0x1C);
    if (e->kind == 3) {
        t = D_800D86E0;
        p = (u8 *)(M2C_FIELD(model * 0x70 + g_Args->unk228, u8 *, 0x5F) * 0x7E + t);
        e->x2 = e->x + *(s16 *)(p + 8);
        e->y2 = e->y + *(s16 *)(p + 0x14);
        e->z2 = e->z + *(s16 *)(p + 0x20);
    }
    return e;
}

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

#ifdef NON_MATCHING
void func_8004009C(u8 *arg0) {
    s16 d[3];
    u16 mn[3];
    u16 mx[3];
    VECTOR v0;
    VECTOR v1;
    GsMat32 m;
    s32 cbarg[2];
    Trig *t;
    u8 *pl;
    u8 *o;
    s32 isp;
    s32 ang;

    if (D_8009CF7C == 0 || D_8009CF79 != 0) return;
    isp = arg0 == (u8 *)g_Player;
    if (isp) {
        D_800D424C = 0;
        D_800D4390 = 0;
    }
    t = D_8009CF7C;
    pl = *(u8 **)(arg0 + 0x90);
    for (; t < D_8009CF7C + D_8009CF80; t++) {
        if (t->on == 0) continue;
        if (t->type == 0) {
            o = D_800D4128;
            if (isp) {
                *(u16 *)(o + 0x1C) = t->a[0];
                *(u16 *)(o + 0x1E) = t->a[1];
                *(u16 *)(o + 0x20) = t->a[2];
                *(u16 *)(o + 0x22) = t->a[3];
                *(u16 *)(o + 0x24) = t->a[4];
                *(u16 *)(o + 0x26) = t->a[5];
                *(u16 *)(o + 0x28) = t->a32;
                *(s32 *)(*(u8 **)(o + 0x90) + 0x28) = t->x;
                *(s32 *)(*(u8 **)(o + 0x90) + 0x2C) = t->y;
                *(s32 *)(*(u8 **)(o + 0x90) + 0x30) = t->z;
                func_8003BC98((u8 *)t + 0x20, *(u8 **)(o + 0x90) + 0x10, (s32)*(u8 **)(o + 0x90));
                if (func_8003C9C4(o, arg0) != 0) {
                    D_800D4260 = 0;
                    D_8009CF82 = 1;
                } else {
                    D_800D4260 = 1;
                }
            }
        } else if ((u8)(t->type - 1) < 2) {
            s32 dx = *(s32 *)(pl + 0x28) - t->x;
            s32 dz = *(s32 *)(pl + 0x30) - t->z;
            if (abs(dx) < 0x1F41 && abs(dz) < 0x1F41) {
                m = D_800FC6A0;
                RotMatrixY(t->ang, &m);
                TransposeMatrix(&m, &m);
                gte_SetRotMatrix(&m);
                mn[0] = t->a[0];
                mn[1] = t->a[2];
                mn[2] = t->a[4];
                mx[0] = t->a[1];
                mx[1] = t->a[3];
                mx[2] = t->a[5];
                d[0] = dx;
                d[2] = dz;
                d[1] = *(u16 *)(pl + 0x2C) - (u16)t->y;
                if (arg0[5] == 6) d[1] += *(s16 *)(arg0 + 0x20) >> 1;
                gte_ldv0(d);
                gte_rtv0();
                d[0] = dx + *(u16 *)(arg0 + 0x98);
                gte_stlvnl(&v0);
                d[1] += *(u16 *)(arg0 + 0x9A);
                d[2] += *(u16 *)(arg0 + 0x9C);
                gte_ldv0(d);
                gte_rtv0();
                gte_stlvnl(&v1);
                if (func_8003F768(&v0, &v1, (SVECTOR *)mn, (SVECTOR *)mx) != 0) {
                    if (t - D_8009CF7C == D_8009CDC4) D_8009D8DC = 1;
                    if (!isp || t->type == 2) {
                        ang = 0;
                    } else {
                        if (D_8009CD4C != 0) ang = D_800B0C5C;
                        else ang = *(u16 *)(*(u8 **)(arg0 + 0x90) + 0x62);
                        ang = (s16)ang;
                        ang = (t->a30 - ang) & 0xFFF;
                        if (ang > 0x800) ang -= 0x1000;
                        ang = abs(ang);
                    }
                    if (ang < 0x2CE) {
                        if (t - D_8009CF7C == D_8009CDC4) D_8009D8DC = 2;
                        if (isp) D_800D4390 = (s32)t;
                        if (t->cb != 0) {
                            cbarg[1] = (s32)t;
                            cbarg[0] = (s32)arg0;
                            t->cb(cbarg);
                        }
                    }
                } else {
                    if (t - D_8009CF7C == D_8009CDC4) D_8009D8DC = 0;
                }
            } else {
                if (t - D_8009CF7C == D_8009CDC4) D_8009D8DC = 0;
            }
        } else {
            func_8006B230(0, D_800A8144);
        }
    }
    if (g_CutScene != 0 && D_800D4390 == 0 && D_800D4080 != 0 && *(void **)((u8 *)D_800D4080 + 0x28)) {
        cbarg[1] = D_800D4080;
        cbarg[0] = (s32)g_Player;
        (*(void (**)(s32 *))((u8 *)D_800D4080 + 0x28))(cbarg);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004009C);
#endif

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

#ifdef NON_MATCHING
s32 func_80040778(s32 *arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    s32 a[4];
    s32 b[4];
    s32 c[4];
    s32 d1;
    s32 d2;
    s32 d3;

    a[0] = arg0[0] - arg3[0];
    a[1] = arg0[1] - arg3[1];
    a[2] = arg0[2] - arg3[2];
    func_80040978(a);
    b[0] = arg1[0] - arg3[0];
    b[1] = arg1[1] - arg3[1];
    b[2] = arg1[2] - arg3[2];
    func_80040978(b);
    c[0] = arg2[0] - arg3[0];
    c[1] = arg2[1] - arg3[1];
    c[2] = arg2[2] - arg3[2];
    func_80040978(c);
    d1 = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) >> 12;
    d2 = (b[0] * c[0] + b[1] * c[1] + b[2] * c[2]) >> 12;
    d3 = (c[0] * a[0] + c[1] * a[1] + c[2] * a[2]) >> 12;
    return func_800365B4(d1) + func_800365B4(d2) + func_800365B4(d3) >= 0xF3C;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80040778);
#endif

void func_80040978(s32 *v) {
    s32 len;
    len = SquareRoot0(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] <<= 12;
    v[1] <<= 12;
    v[2] <<= 12;
    if (len != 0) {
        v[0] = v[0] / len;
        v[1] = v[1] / len;
        v[2] = v[2] / len;
    }
}

s32 func_80040AA8(s32 *pos) {
    s32 i;
    u8 *e;
    s32 dx, dy, dz, r;
    s32 *t;
    e = (u8 *)D_800AFFC8 + D_800B0DC0 * 0x124;
    for (i = D_800B0DC0; i <= D_800B07A8; i++, e += 0x124) {
        if (e[4] != 0 && !(*(s32 *)(e + 0x4C) & 0x02000000)) {
            t = *(s32 **)(e + 0x90);
            dx = abs(t[0x28 / 4] - pos[0]);
            if (dx < 0x1389) {
                dy = abs(t[0x2C / 4] - pos[1]);
                if (dy < 0x1389) {
                    dz = abs(t[0x30 / 4] - pos[2]);
                    if (dz < 0x1389) {
                        dx *= dx;
                        dy *= dy;
                        dz *= dz;
                        dx += dy + dz;
                        dz = *(u16 *)(e + 0x2A) + 0x1F4;
                        if (dx < dz * dz) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

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

void DebugPage_ObjectCount(void) {
    s16 total;
    s16 i;
    s16 n;
    ObjEnt *e;
    u8 *p;
    u8 t;

    g_DebugTextX = 0x16;
    g_DebugTextY = 0xB;
    FntPrint(D_800A81F8, g_FrameRate, g_FrameCap);
    total = 0;
    D_8009CF9C = 0;
    D_8009CF98 = 0;
    D_8009CF94 = 0;
    D_8009CF90 = 0;
    D_8009CF8C = 0;
    e = (ObjEnt *)D_800AFFC8 + 1;
    for (i = 0; i < D_800B0AA8; i++) {
        if (e->active != 0) {
            t = e->type;
            total++;
            if (t == 6) {
                D_8009CF8C++;
            } else if (t == 3) {
                D_8009CF90++;
            } else if (t == 2) {
                D_8009CF94++;
            } else if (t == 1) {
                D_8009CF98++;
            } else if (t == 4) {
                D_8009CF9C++;
            }
        }
        e++;
    }
    FntPrint(D_800A8210, D_8009CF8C, 6);
    FntPrint(D_800A8224, D_8009CF90, D_800B0C8C);
    FntPrint(D_800A8238, D_8009CF94, D_800B0AA4);
    FntPrint(D_800A824C, D_8009CF98, D_800AFFC0);
    FntPrint(D_800A8260, D_8009CF9C, D_800B008C);
    FntPrint(D_800A8274, total);
    n = 0;
    p = D_800B0AB4;
    for (i = 0; i < D_800B0C84; i++) {
        if (p[1] != 0) {
            n++;
        }
        p += 0x34;
    }
    FntPrint(D_800A8280, n);
}

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

void func_80041BA0(void) {
    s32 sel;

    sel = func_80043B14(0, D_8009CFBE);
    if (sel == 3 && *(s32 *)&D_8009CFC0 > 0) {
        func_80026F30(1, 0);
        g_MenuUpdateFunc.update = func_80041C28;
    } else if (sel == 1) {
        func_80026F30(1, 0);
        PauseMenu_Open();
    }
}

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

void func_80041E78(void) {
    if ((u32)D_800D4410++ >= 10) {
        D_8009CA90 = 1;
        D_8009CAA4 = 0;
        g_Args->flags |= 0x40;
        func_800414AC();
    }
}

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

void func_80042020(void) {
    s16 r;
    r = func_80043B14(D_8009CFBE, D_8009CFBE);
    if (r == 1) {
        func_80026F30(1, 0);
        goto open;
    }
    if (r == 3) {
        func_80026F30(1, 0);
        if (D_800D4454 != 0) {
            if ((*(u8 *)g_Args >> 7) != 0) {
                D_8009CAA4 = 0;
                func_800414AC();
                return;
            }
            func_80041DBC();
            return;
        }
        if (D_8009CA90 != 0) {
            D_8009CAA4 = 0;
            g_Args->flags |= 0x80;
            func_800414AC();
            return;
        }
open:
        {
        Args *a = g_Args;
        a->unk115 = 0;
        a->flags &= ~0x80;
        }
        PauseMenu_Open();
        return;
    }
    if ((u32)(r - 4) < 4U) {
        func_80026F30(2, 0);
        D_800D4454 ^= 1;
        if (D_800D4454 != 0) {
            func_800289C0(D_8009CFBE, D_800A8464);
        } else {
            func_800289C0(D_8009CFBE, D_800A8460);
        }
    }
}

void func_8004218C(void) {
    s32 x;

    Menu_ResetState();
    D_8009CFBE = 0;
    D_800B0794 = 0;
    D_8009CFBE = 1;
    D_800D4458 = 0;
    func_8002872C((M2C_UNK *)D_800A8468, 0x64, 0x53);
    D_800D43E4 = D_8009CFBE++;
    func_8002872C((M2C_UNK *)D_800A8478, 0x64, 0x65);
    D_800D43F8 = D_8009CFBE++;
    func_8002872C((M2C_UNK *)D_800A8484, 0x64, 0x77);
    D_800D445C = D_8009CFBE++;
    func_8002872C((M2C_UNK *)D_800A8490, 0x64, 0x89);
    D_800D43D0 = D_8009CFBE++;
    func_8002872C((M2C_UNK *)D_800A849C, 0x64, 0x9B);
    D_800D4400 = D_8009CFBE++;
    func_8002872C((M2C_UNK *)D_800A84A8, 0x64, 0xAD);
    x = 0x64;
    D_800D4440 = D_800AC54C;
    func_8002872C((M2C_UNK *)D_800A8458, x + 0xFA, 0x53);
    if (g_Args->reverseSniper == 0) {
        func_800289C0((s16)D_800D4440, D_800A8464);
    } else {
        func_800289C0((s16)D_800D4440, D_800A8460);
    }
    D_800D43C0 = D_800AC54C;
    func_8002872C((M2C_UNK *)D_800A8458, x + 0xFA, 0x65);
    if (*(u8 *)&g_Args->flags2 >> 7) {
        func_800289C0((s16)D_800D43C0, D_800A84B4);
    } else {
        func_800289C0((s16)D_800D43C0, D_800A84B8);
    }
    D_800D4450 = D_800AC54C;
    func_8002872C((M2C_UNK *)D_800A8458, x + 0xFA, 0x89);
    if (g_Args->flags2 & 1) {
        func_800289C0((s16)D_800D4450, D_800A84B4);
    } else {
        func_800289C0((s16)D_800D4450, D_800A84B8);
    }
    D_800D43A8 = D_800AC54C;
    func_8002872C((M2C_UNK *)D_800A8458, x + 0xFA, 0x9B);
    if ((g_Args->flags2 >> 4) & 1) {
        func_800289C0((s16)D_800D43A8, D_800A84B4);
    } else {
        func_800289C0((s16)D_800D43A8, D_800A84B8);
    }
    D_800D4464 = D_800AC54C;
    func_8002872C((M2C_UNK *)D_800A8458, x + 0xFA, 0xAD);
    if ((g_Args->flags2 >> 5) & 1) {
        func_800289C0((s16)D_800D4464, D_800A84B4);
    } else {
        func_800289C0((s16)D_800D4464, D_800A84B8);
    }
    func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
    if (D_800B0794 == D_800D4458) {
        func_80028B58(D_800D4440, D_800AEDA0, D_800AEDA4);
    } else if (D_800B0794 == D_800D43E4) {
        func_80028B58(D_800D43C0, D_800AEDA0, D_800AEDA4);
    } else if (D_800B0794 == D_800D445C) {
        func_80028B58(D_800D4450, D_800AEDA0, D_800AEDA4);
    } else if (D_800B0794 == D_800D43D0) {
        func_80028B58(D_800D43A8, D_800AEDA0, D_800AEDA4);
    } else if (D_800B0794 == D_800D4400) {
        func_80028B58(D_800D4464, D_800AEDA0, D_800AEDA4);
    }
    func_8002872C((M2C_UNK *)D_800A83E0, 0x32, 0x37);
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    func_8002872C(&D_800A8418, 0x19A, 0xD7);
    g_MenuUpdateFunc.update = func_800425E4;
}

#ifdef NON_MATCHING
void func_800425E4(void) {
    s16 r;
    r = func_80043B14(0, D_8009CFBE);
    if (r == 1) {
        func_80026F30(1, 0);
        PauseMenu_Open();
        return;
    }
    if (r == 3) {
        if (D_800B0794 == D_800D43F8) {
            func_80026F30(1, 0);
            func_80043200();
        }
        return;
    }
    if (r == 2) {
        if (D_800B0794 == D_800D4458) {
            func_80028B58(D_800D4440, D_800AEDA0, D_800AEDA4);
        } else {
            func_80028B58(D_800D4440, (s16)D_800AC4D8, (s16)D_800AC4DC);
        }
        if (D_800B0794 == D_800D43E4) {
            func_80028B58(D_800D43C0, D_800AEDA0, D_800AEDA4);
        } else {
            func_80028B58(D_800D43C0, (s16)D_800AC4D8, (s16)D_800AC4DC);
        }
        if (D_800B0794 == D_800D445C) {
            func_80028B58(D_800D4450, D_800AEDA0, D_800AEDA4);
        } else {
            func_80028B58(D_800D4450, (s16)D_800AC4D8, (s16)D_800AC4DC);
        }
        if (D_800B0794 == D_800D43D0) {
            func_80028B58(D_800D43A8, D_800AEDA0, D_800AEDA4);
        } else {
            func_80028B58(D_800D43A8, (s16)D_800AC4D8, (s16)D_800AC4DC);
        }
        if (D_800B0794 == D_800D4400) {
            func_80028B58(D_800D4464, D_800AEDA0, D_800AEDA4);
        } else {
            func_80028B58(D_800D4464, (s16)D_800AC4D8, (s16)D_800AC4DC);
        }
        return;
    }
    if ((u32)(r - 4) < 2) {
        if (D_800B0794 != D_800D43F8) {
            func_80026F30(2, 0);
        }
        if (D_800B0794 == D_800D4458) {
            if (((u8 *)g_Args)[0x117] == 0) {
                ((u8 *)g_Args)[0x117] = 1;
                func_800289C0(D_800D4440, D_800A8460);
            } else {
                ((u8 *)g_Args)[0x117] = 0;
                func_800289C0(D_800D4440, D_800A8464);
            }
        } else if (D_800B0794 == D_800D43E4) {
            g_Args->flags2 ^= 0x80;
            if (g_Args->flags2 & 0x80) {
                func_800289C0(D_800D43C0, D_800A84B4);
                func_8002F564(g_Player, 0xFF, 5, 0, 0);
            } else {
                func_800289C0(D_800D43C0, D_800A84B8);
            }
        } else if (D_800B0794 == D_800D445C) {
            g_Args->flags2 ^= 1;
            if (g_Args->flags2 & 1) {
                func_800289C0(D_800D4450, D_800A84B4);
            } else {
                func_800289C0(D_800D4450, D_800A84B8);
            }
        } else if (D_800B0794 == D_800D43D0) {
            g_Args->flags2 ^= 0x10;
            if (g_Args->flags2 & 0x10) {
                func_800289C0(D_800D43A8, D_800A84B4);
            } else {
                func_800289C0(D_800D43A8, D_800A84B8);
            }
        } else if (D_800B0794 == D_800D4400) {
            g_Args->flags2 ^= 0x20;
            if (g_Args->flags2 & 0x20) {
                func_800289C0(D_800D4464, D_800A84B4);
            } else {
                func_800289C0(D_800D4464, D_800A84B8);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800425E4);
#endif

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

void func_80042B28(void) {
    s32 i;
    s32 n;
    s32 x;
    s32 y;
    i = 0;
    x = 100;
    y = 0x4B;
    n = 0;
    for (; i < 2; i++) {
        if (((SaveSlot *)&D_800F10A8)[i].unk69 != 0) {
            D_800D4474[n] = i;
            func_8002872C((M2C_UNK *)((SaveSlot *)&D_800F10A8)[i].name, x, y);
            if (D_8009CA8C != 0) {
                func_80028B58(D_800AC54C - 1, (s16)D_800AC4D8, (s16)D_800AC4DC);
            }
            y += 0x12;
            n++;
        }
    }
    x = 0x50;
    y = 0x4B;
    for (i = 0; i < n; i++) {
        if (((SaveSlot *)&D_800F10A8)[i].unk69 != 0) {
            if (((SaveSlot *)&D_800F10A8)[D_800D4474[i]].unk68 != 0) {
                func_8002872C((M2C_UNK *)D_800A84BC, x, y);
            } else {
                func_8002872C((M2C_UNK *)D_800A84C0, x, y);
            }
            if (D_8009CA8C != 0) {
                func_80028B58(D_800AC54C - 1, (s16)D_800AC4D8, (s16)D_800AC4DC);
            }
            y += 0x12;
        }
    }
    if (D_8009CA8C != 0) {
        D_8009CFBE = n;
        func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
        func_80028B58(D_800B0794 + n, D_800AEDA0, D_800AEDA4);
    }
}

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

#ifdef NON_MATCHING
void func_80043610(void) {
    char buf[16];
    s32 r;
    s16 v;
    s32 dir;
    if (D_8009CFE8 >= 0) {
        D_8009CFE8 += 1;
    }
    r = (s16)func_80043B14(0, D_8009CFBE);
    if (D_800B0794 == D_800D4404) {
        if (D_8009CFE4 < 0) {
            D_8009CFE4 = func_80026D08(0, 0);
        } else {
            func_800272A4((s16)D_8009CFE4, 0, 0);
        }
    }
    if (r == 2) {
        if (D_8009CFE4 >= 0) {
            if (D_800B0794 != D_800D4404) {
                func_80026C14((s16)D_8009CFE4);
                D_8009CFE4 = -1;
            }
        }
        if (D_800B0794 == D_800D43C4) {
            func_80028B58(D_800D440C, D_800AEDA0, D_800AEDA4);
        } else {
            func_80028B58(D_800D440C, D_800AC4D8, D_800AC4DC);
        }
        if (D_800B0794 == D_800D4404) {
            func_80028B58(D_800D4414, D_800AEDA0, D_800AEDA4);
            return;
        }
        func_80028B58(D_800D4414, D_800AC4D8, D_800AC4DC);
        return;
    }
    if (r == 1) {
        D_8009CFE8 = 0;
        func_80026C90(-1);
        D_8009CFE4 = -1;
        func_80026F30(1, 0);
        PauseMenu_Open();
        return;
    }
    if ((u32)(r - 4) < 2U) {
        D_8009CFE8 = 0;
        dir = (r == 4) ? 1 : -1;
    } else if (g_PadPressed == 0 && (g_PadHeld & 0xA000) && (D_8009CFE8 == (D_8009CFE8 / 5) * 5 || D_8009CFE8 == -1)) {
        D_8009CFE8 = -1;
        dir = (g_PadHeld & 0x2000) ? 1 : -1;
    } else {
        return;
    }
    if (D_800B0794 == D_800D43C4) {
        AF(s16, 0x140) = AF(u16, 0x140) + dir;
        v = AF(u16, 0x140);
        if (v < 0) {
            v = 0;
        } else if (v >= 0x65) {
            v = 0x64;
        }
        AF(u16, 0x140) = v;
        sprintf(buf, D_800A857C, AF(s16, 0x140));
        func_800289C0((s16)D_800D440C, buf);
        AF(s16, 0x13A) = AF(s16, 0x140) * 0x7F / 100;
        func_80026F30(2, 0);
        return;
    }
    if (D_800B0794 == D_800D4404) {
        AF(s16, 0x13E) = AF(u16, 0x13E) + dir;
        v = AF(u16, 0x13E);
        if (v < 0) {
            v = 0;
        } else if (v >= 0x65) {
            v = 0x64;
        }
        AF(u16, 0x13E) = v;
        sprintf(buf, D_800A857C, AF(s16, 0x13E));
        func_800289C0((s16)D_800D4414, buf);
        func_80026F30(2, 0);
        AF(s16, 0x144) = AF(s16, 0x13E) * 0x7F / 100;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80043610);
#endif

#ifdef NON_MATCHING
s16 func_80043B14(s16 arg0, s16 arg1) {
    s16 up;
    s16 down;
    s16 left;
    s16 right;

    up = 0;
    if ((*(u8 *)&D_800B0E54 != 0 && D_800B0E5F < 0x4E && D_800B0E63 >= 0x4E) || ((g_PadHeld & 0x1000) && !(g_PadPrev & 0x1000))) {
        up = 1;
    }
    down = 0;
    if ((*(u8 *)&D_800B0E54 != 0 && D_800B0E5F >= 0xB3 && D_800B0E63 < 0xB3) || ((g_PadHeld & 0x4000) && !(g_PadPrev & 0x4000))) {
        down = 1;
    }
    left = 0;
    if ((*(u8 *)&D_800B0E54 != 0 && D_800B0E5E < 0x4E && D_800B0E62 >= 0x4E) || ((g_PadHeld & 0x8000) && !(g_PadPrev & 0x8000))) {
        left = 1;
    }
    right = 0;
    if ((*(u8 *)&D_800B0E54 != 0 && D_800B0E5E >= 0xB3 && D_800B0E62 < 0xB3) || ((g_PadHeld & 0x2000) && !(g_PadPrev & 0x2000))) {
        right = 1;
    }
    D_800B0798 = D_800B0794;
    if (D_8009CA90 == 0) {
        if (up && arg1 >= 2) {
            func_80026F30(1, 0);
            func_80028B58(D_800B0794, (s16)D_800AC4D8, (s16)D_800AC4DC);
            D_800B0794--;
            if (D_800B0794 < arg0) {
                D_800B0794 = arg0 + (arg1 - 1);
            }
            func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
            return 2;
        }
        if (down && arg1 >= 2) {
            func_80026F30(1, 0);
            func_80028B58(D_800B0794, (s16)D_800AC4D8, (s16)D_800AC4DC);
            D_800B0794++;
            if ((arg0 - 1) + D_8009CFBE < D_800B0794) {
                D_800B0794 = arg0;
            }
            func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
            return 2;
        }
        if ((g_PadHeld & 0x10) && !(g_PadPrev & 0x10)) {
            return 1;
        }
    }
    if (left) {
        return 5;
    }
    if (right) {
        return 4;
    }
    if (up) {
        return 6;
    }
    if (down) {
        return 7;
    }
    if ((g_PadHeld & 0x40) && !(g_PadPrev & 0x40)) {
        return 3;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80043B14);
#endif

#ifdef NON_MATCHING
void func_80043EE0(void) {
    s32 sp10;
    Weapon *wp;
    u16 prevYaw;
    u16 prevPitch;
    s16 pitchDir;
    s32 moved;
    s32 held;
    s32 yawStep;
    s32 pitchStep;
    s32 t;
    s32 a0;
    s16 v;
    s16 cnt;
    s32 btnA;
    s32 btnB;

    prevYaw = D_800B0C5C;
    wp = &g_Weapons[D_800B0DD4];
    prevPitch = D_800B0C64;
    if (g_Args->reverseSniper == 0) {
        pitchDir = -(s16) wp->pad4C[2];
    } else {
        pitchDir = (s16) wp->pad4C[2];
    }
    moved = 0;
    M2C_FIELD(g_Player, s16 *, 0x98) = 0;
    M2C_FIELD(g_Player, s16 *, 0xA0) = 0;
    M2C_FIELD(g_Player, s16 *, 0x56) = 0;
    if (D_8009DB3C != 0) {
        func_80061AB0((s32) M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), s16 *, 0x60), (s32) M2C_FIELD(g_Player, s16 *, 0x52));
    } else {
        M2C_FIELD(g_Player, s16 *, 0x9C) = 0;
    }
    if (D_8009CD54 != 0) {
        /* camera glides back out of aim mode */
        if (g_PadHeld & g_Args->buttonMap[5]) {
            Camera_ResetZoom();
            func_8004DD9C();
            return;
        }
        D_800B0B10 = 0xFA0;
        g_Camera.targetX = M2C_FIELD(D_800D9200, s16 *, 0);
        g_Camera.targetY = M2C_FIELD(D_800D9200, s16 *, 2);
        g_Camera.targetZ = M2C_FIELD(D_800D9200, s16 *, 4);
        g_Camera.rotX = M2C_FIELD(&g_Camera, u16 *, 0x80);
        g_Camera.rotY = M2C_FIELD(&g_Camera, u16 *, 0x82);
        g_Camera.rotZ = M2C_FIELD(&g_Camera, u16 *, 0x84);
        func_8004F3E8();
        if (D_800B0C74 <= 0) {
            Camera_ResetZoom();
        } else {
            a0 = D_800B0C74 - D_800B0B10;
            D_800B0C74 = a0;
            if (a0 >= 0) {
                t = D_800B0B14;
                if (t >= a0) {
                    t = a0;
                }
            } else {
                t = 0;
            }
            D_800B0C74 = t;
            func_80044A60();
        }
        if ((D_8009DB3C == 0) || (M2C_FIELD(g_Player, s16 *, 0x52) == 0)) {
            M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), u16 *, 0x62) = D_800B0C5C;
        }
        return;
    }
    func_8005D694();
    if (!(g_PadHeld & g_Args->buttonMap[5])) {
        func_80044898();
        return;
    }
    /* zoom in towards D_800B0B14 */
    if (D_800B0C74 < D_800B0B14) {
        D_800B0C74 = D_800B0C74 + D_800B0B10;
    } else if (D_800B0B14 < D_800B0C74) {
        D_800B0C74 = D_800B0C74 - D_800B0B10;
    }
    if (D_800B0C74 >= 0) {
        t = D_800B0B14;
        if (t >= D_800B0C74) {
            t = D_800B0C74;
        }
    } else {
        t = 0;
    }
    D_800B0C74 = t;
    func_800449BC();
    if (*(u8 *) &D_800B0E54 != 0) {
        /* analog stick aiming */
        Pad_StickToSpeed(0, wp->pad51[2], &sp10);
        sp10 = sp10 * 2;
        held = g_PadHeld & 0x200;
        if (held != 0) {
            t = sp10 / 4;
            if (abs(t) > 0) {
                sp10 = t;
            }
        }
        D_800B0C5C = D_800B0C5C + sp10;
        Pad_StickToSpeed(1, wp->pad51[7], &sp10);
        sp10 = sp10 * pitchDir * 2;
        if (held != 0) {
            t = sp10 / 4;
            if (abs(t) > 0) {
                sp10 = t;
            }
        }
        t = D_800B0C64 + sp10;
        D_800B0C64 = t;
        if (t >= -0x280) {
            a0 = 0x320;
            if (t < 0x321) {
                a0 = t;
            }
        } else {
            a0 = -0x280;
        }
        D_800B0C64 = a0;
        if (((s16) prevYaw != (s32) D_800B0C5C) || ((s16) prevPitch != D_800B0C64)) {
            moved = 1;
        }
    } else {
        /* digital pad aiming with acceleration */
        yawStep = 0;
        if (g_PadHeld & 0xA000) {
            moved = 1;
            a0 = wp->pad51[1];
            cnt = D_800B0C9C + 1;
            D_800B0C9C = cnt;
            v = cnt + (a0 + (cnt >> 1));
            if (v >= a0) {
                if ((s32) wp->pad51[2] < v) {
                    v = wp->pad51[2];
                }
            } else {
                v = a0;
            }
            if (g_PadHeld & 0x8000) {
                v = -v;
            }
            yawStep = v * wp->pad4C[3] * 2;
            if (g_FrameRate < g_FrameCap) {
                yawStep = (yawStep * g_FrameCap) / g_FrameRate;
            }
            D_800B0C5C = (D_800B0C5C + yawStep) & 0xFFF;
        }
        if (g_PadHeld & 0x5000) {
            moved = 1;
            a0 = wp->pad51[1];
            cnt = D_800B0C9C + 1;
            D_800B0C9C = cnt;
            v = cnt + (a0 + (cnt >> 1));
            t = (s16) (pitchDir * a0);
            if (v >= a0) {
                t = (s16) (pitchDir * v);
                if ((s32) wp->pad51[7] < v) {
                    t = (s16) (pitchDir * wp->pad51[7]);
                }
            }
            v = t;
            if (g_PadHeld & 0x4000) {
                v = -t;
            }
            pitchStep = v * wp->pad4C[2] * 2;
            if (g_FrameRate < g_FrameCap) {
                pitchStep = (pitchStep * g_FrameCap) / g_FrameRate;
            }
            if (pitchStep != 0) {
                if (yawStep != 0) {
                    pitchStep -= pitchStep >> 2;
                    D_800B0C5C = (D_800B0C5C - (yawStep >> 2)) & 0xFFF;
                }
                t = D_800B0C64 + pitchStep;
                D_800B0C64 = t;
                a0 = -0x280;
                if (t >= -0x280) {
                    a0 = 0x320;
                    if (t < 0x321) {
                        a0 = t;
                    }
                }
                D_800B0C64 = a0;
            }
        }
    }
    /* limit the yaw swing around the locked direction */
    if (D_800B0D94 >= 0) {
        if (D_800B0D94 >= 0x801) {
            v = 0x1000 - (u16) D_800B0D94;
        } else {
            v = -(s16) (u16) D_800B0D94;
        }
        t = (D_800B0C5C + v) & 0xFFF;
        if ((u32) (t - 0xEAB) >= 0x156U) {
            if ((u32) t >= 0x156U) {
                a0 = 0x155;
                if (t >= 0x801) {
                    a0 = -0x155;
                }
                D_800B0C5C = a0 - v;
            }
        }
    }
    btnA = g_PadHeld & g_Args->buttonMap[6];
    btnB = g_PadHeld & g_Args->buttonMap[7];
    if ((btnA != 0) && (btnB != 0)) {
        D_800B0AEC = D_800B0C5C;
    } else if (btnA != 0) {
        if (!(g_PadPrev & g_Args->buttonMap[6])) {
            D_800B0D94 = D_800B0C5C;
        }
        D_800B0AEC = ((u16) D_800B0D94 - 0x400) & 0xFFF;
    } else if (btnB != 0) {
        if (!(g_PadPrev & g_Args->buttonMap[7])) {
            D_800B0D94 = D_800B0C5C;
        }
        D_800B0AEC = ((u16) D_800B0D94 + 0x400) & 0xFFF;
    } else {
        D_800B0D94 = -1;
        D_800B0AEC = D_800B0C5C;
    }
    if (moved == 0) {
        D_800B0C9C = 0;
        D_8009CD5C = D_8009CD5C + 1;
        return;
    }
    D_8009CD5C = 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80043EE0);
#endif

void Pad_StickToSpeed(s32 axis, s32 maxSpeed, s32 *out) {
    s32 deadzone;
    s32 sign;
    s16 raw;
    s16 mag;
    s16 speed;

    deadzone = 3;
    if (axis == 0) {
        raw = (&g_PadAxes)[2] - 0x7F;
    } else {
        raw = -(&g_PadAxes)[3] + 0x7F;
    }
    mag = raw;
    sign = mag;
    if (sign < 0) {
        mag = -mag;
    }
    mag = mag - deadzone;
    if (mag < 0) {
        mag = 0;
    }
    speed = (s16)(mag * mag) / (s16)((s16)((0x80 - deadzone) * (0x80 - deadzone)) / maxSpeed);
    if (sign < 0) {
        speed = -speed;
    }
    if (g_FrameRate < g_FrameCap) {
        speed = speed * g_FrameCap / g_FrameRate;
    }
    *out = speed;
}

void func_80044898(void) {
    D_8009CD54 = 1;
    D_8009CFB0 = 0;
    *(u8 *)&D_8009D3A4 = 0;
    M2C_FIELD(g_Player, s32 *, 0x5C) = 0;
    if (D_8009DB3C == 0 || M2C_FIELD(g_Player, s16 *, 0x52) == 0) {
        gPsi = D_800B0C5C;
    }
}

void func_800448F4(void) {
    Weapon *w;
    s16 a;
    s16 b;

    w = &g_Weapons[D_800B0DD4];
    a = *(u16 *)((u8 *)w + 0x54);
    b = *(u16 *)((u8 *)w + 0x56);
    D_800D43F4 = *(s16 *)((u8 *)w + 0x68);
    D_800D43CC = a / 10;
    D_800D43AC = b / 10;
    if ((s16)D_800D43CC < (s16)D_800D43AC) {
        D_800D43AC = 0xB4;
    }
    D_800D43CC = 0xB4;
    D_8009CFCC = 0;
    D_800D43F0 = *(s16 *)((u8 *)w + 0x68) - 0xB4;
}

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

#ifdef NON_MATCHING
void func_80044BCC(s32 arg0) {
    u8 *d = (u8 *)&D_800D5F40;
    s32 secs = arg0 / 60;
    s32 mins;
    s32 n = *(s32 *)d;
    s32 flag;
    u8 *g;
    u8 *p;
    if (*(s16 *)(d + 0x10) < 0 || secs != *(s16 *)(d + 0x10)) {
        flag = (*(s16 *)(d + 0x10) & 0x8000) ^ 0x8000;
        if (*(s16 *)(d + 0x12) != 0) {
            if (flag != 0) {
                func_80026F30(*(s16 *)(d + 0x12), 0);
            }
        }
        p = *(u8 **)(d + 0xC);
        *(s16 *)(d + 0x10) = secs | flag;
        if (D_800B8F24 != 0) {
            p += n * 0x28;
        }
        if (n >= 4) {
            if (n >= 5) {
                DIGIT(p, secs / 60 / 10);
                p += 0x28;
            }
            DIGIT(p, secs / 60 % 10);
            p += 0x28;
            secs = secs % 60;
        }
        if (n >= 3) {
            p += 0x28;
        }
        DIGIT(p, secs / 10);
        p += 0x28;
        DIGIT(p, secs % 10);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80044BCC);
#endif

void func_80045084(void) {
    s32 n;
    u32 *prim;
    u32 *ot;
    s32 i;

    n = D_800D5F40;
    if (n > 0) {
        ot = D_800B8F88[D_800B8F24].org + 1;
        prim = ((u32 **)&D_800D5F40)[3];
        if (D_800B8F24 != 0) {
            prim += n * 10;
        }
        for (i = 0; i < n; i++, prim += 10) {
            ((PTag *)prim)->addr = ((PTag *)ot)->addr;
            ((PTag *)ot)->addr = (u32)prim;
        }
    }
}

void func_80045134(void) {
    D_800D5F40 |= 0x80000000;
}

void func_8004514C(void) {
  s16 temp_v0;
  M2C_UNK *new_var2;
  s16 temp_v0_2;
  s32 *temp_a0;
  s32 *temp_a0_2;
  s32 *temp_a1;
  s32 *temp_a1_2;
  s32 temp_v1;
  s32 temp_v1_2;
  void *temp_t0;
  M2C_UNK *new_var;
  s32 idx;
  if (D_8009CFFC != 0)
  {
    temp_t0 = (void *) D_800B8EFC[D_800B8F24][1];
    idx = D_800B8F24;
    if (D_8009CFEC != 0)
    {
      new_var = &D_800D45C4;
      temp_a0 = (s32 *) ((s16 (*)[12]) new_var)[idx];
      *temp_a0 = ((*temp_a0) & 0xFF000000) | (((struct 
      {
        char pad[(4 > 0) ? (4) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((4 < 0) ? (4) : (0))))->f & 0xFFFFFF);
      temp_a1 = (s32 *) D_800D5ED8[idx];
      temp_v1 = (((struct 
      {
        char pad[(4 > 0) ? (4) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((4 < 0) ? (4) : (0))))->f & 0xFF000000) | (((s32) temp_a0) & 0xFFFFFF);
      ((struct 
      {
        char pad[(4 > 0) ? (4) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((4 < 0) ? (4) : (0))))->f = temp_v1;
      *temp_a1 = ((*temp_a1) & 0xFF000000) | (temp_v1 & 0xFFFFFF);
      ((struct 
      {
        char pad[(4 > 0) ? (4) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((4 < 0) ? (4) : (0))))->f = (s32) ((((struct 
      {
        char pad[(4 > 0) ? (4) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((4 < 0) ? (4) : (0))))->f & 0xFF000000) | (((s32) temp_a1) & 0xFFFFFF));
    }
    else
    {
      new_var2 = &D_800D45C4;
      temp_a0_2 = (s32 *) ((s16 (*)[12]) new_var2)[idx];
      *temp_a0_2 = ((*temp_a0_2) & 0xFF000000) | (((struct 
      {
        char pad[(0 > 0) ? (0) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((0 < 0) ? (0) : (0))))->f & 0xFFFFFF);
      temp_a1_2 = (s32 *) D_800D5ED8[idx];
      temp_v1_2 = (((struct 
      {
        char pad[(0 > 0) ? (0) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((0 < 0) ? (0) : (0))))->f & 0xFF000000) | (((s32) temp_a0_2) & 0xFFFFFF);
      ((struct 
      {
        char pad[(0 > 0) ? (0) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((0 < 0) ? (0) : (0))))->f = temp_v1_2;
      *temp_a1_2 = ((*temp_a1_2) & 0xFF000000) | (temp_v1_2 & 0xFFFFFF);
      ((struct 
      {
        char pad[(0 > 0) ? (0) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((0 < 0) ? (0) : (0))))->f = (s32) ((((struct 
      {
        char pad[(0 > 0) ? (0) : (0)];
        typeof(*((s32 *) 0)) f;
      } *) (((char *) temp_t0) + ((0 < 0) ? (0) : (0))))->f & 0xFF000000) | (((s32) temp_a1_2) & 0xFFFFFF));
    }
    if (D_8009CFFC == 1)
    {
      temp_v0 = ((u16) D_8009D000) + 0x14;
      D_8009D000 = temp_v0;
      if (temp_v0 >= 0x100)
      {
        D_8009D000 = 0xFF;
        D_8009CFFC = 2;
      }
    }
    else
      if (D_8009CFFC == (-1))
    {
      temp_v0_2 = ((u16) D_8009D000) - 0x14;
      D_8009D000 = temp_v0_2;
      if (temp_v0_2 < 0)
      {
        D_8009D000 = 0;
        D_8009CFFC = 2;
      }
    }
    ((void (*)(s32, s32, s32)) func_8004B028)(*((u8 *) (&D_8009D000)), *((u8 *) (&D_8009D000)), *((u8 *) (&D_8009D000)));
  }
}

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

void func_800456D0(void) {
    extern POLY_FT4 D_800D7588[];
    extern POLY_FT4 D_800D4688[];
    extern POLY_FT4 D_800D77E8[];
    extern POLY_FT4 D_800D7838[];
    s32 i;
    s32 tp;
    s32 cl;
    s32 j;
    POLY_FT4 *p;
    POLY_FT4 *q;
    POLY_FT4 *a;
    POLY_FT4 *b;
    POLY_FT4 *c;

    p = D_800D7588;
    b = D_800D4688;
    a = D_800D7838;
    c = D_800D77E8;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 7; j++) {
            tp = ((D_800AC4E4 & 0x100) >> 4) | ((D_800AC4E0 & 0x3FF) >> 6) | ((D_800AC4E4 & 0x200) << 2);
            cl = (*(s16 *)&D_800AC4DC << 6) | ((D_800AC4D8 >> 4) & 0x3F);
            M2C_FIELD(p, u8 *, 3) = 9;
            p->code = 0x2C;
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->tpage = tp;
            p->clut = cl;
            p++;
        }
        func_80045344(a, 0xE0, 0xC0, 0x191, 0xC7, 0x18, 0x18, 0x10, 0x338, 0x1C0, 0x290, 0x1C5);
        a++;
        func_80045344(b, 0xF8, 0xCE, 0x1A9, 0xC7, 4, 0x21, 0x10, 0x33E, 0x1CE, 0x270, 0x1C5);
        b++;
        func_80045344(c, 0xE0, 0xE8, 0x1CA, 0xC7, 0x18, 0x18, 0x10, 0x338, 0x1E8, 0x280, 0x1C5);
        c++;
    }
}

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

#ifdef NON_MATCHING
void func_80045A28(void *arg0) {
    Weapon *wp;
    u8 *p;
    u8 *digits;
    s16 x;
    s16 y;
    s16 cur;
    s16 res;
    s32 shown;
    s32 d;

    p = (u8 *) D_800D7588 + D_800B8F24 * 0x118;
    shown = 0;
    wp = &g_Weapons[D_800B0DDC];
    x = (s16) g_Args->displayW - 0x6B;
    y = (s16) g_Args->displayH - 0x25;
    if (((u32) wp->flags >> 0xD) & 1) {
        cur = wp->clip;
        res = wp->reserve;
    } else {
        cur = wp->ammo;
        res = -1;
    }
    d = cur / 100;
    if (d > 0) {
        HUD_DIGIT(D_800AE9C0 + d * 8, y);
        shown = 1;
    }
    d = (cur % 100) / 10;
    if ((d > 0) || (shown != 0)) {
        HUD_DIGIT(D_800AE9C0 + d * 8, y);
    }
    d = (cur % 100) % 10;
    HUD_DIGIT(D_800AE9C0 + d * 8, y);
    if (((u32) wp->flags >> 0xD) & 1) {
        HUD_DIGIT(D_800AE9C0 - 8, y);
        y = y + 1;
        digits = D_800AE9C0 + 0x278;
        shown = 0;
        d = res / 100;
        if (d > 0) {
            HUD_DIGIT(digits + d * 8, y);
            shown = 1;
        }
        d = (res % 100) / 10;
        if ((d > 0) || (shown != 0)) {
            HUD_DIGIT(digits + d * 8, y);
        }
        d = (res % 100) % 10;
        HUD_DIGIT(digits + d * 8, y);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80045A28);
#endif

void func_8004626C(u32 *ot) {
    if (D_8009CFC4 == 1) {
        if (D_8009D038 < 200) {
            D_8009D038++;
            if (D_8009D084 == 0) {
                D_8009D084 = 15;
                D_8009D088 ^= 1;
                if (D_8009D088 == 1) {
                    func_80026F30(12, 0);
                }
            } else {
                D_8009D084--;
            }
            if (D_8009D088 == 0) {
                ((POLY_FT4 *)&D_800D7CB8)[D_800B8F24].clut = 0x716B;
            } else {
                ((POLY_FT4 *)&D_800D7CB8)[D_800B8F24].clut = 0x716C;
            }
            ((PTag *)&((POLY_FT4 *)&D_800D7CB8)[D_800B8F24])->addr = ((PTag *)(ot + 1))->addr;
            ((PTag *)(ot + 1))->addr = (u32)&((POLY_FT4 *)&D_800D7CB8)[D_800B8F24];
        }
    } else {
        D_8009D084 = 15;
        D_8009D088 = 0;
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800463C8);

void func_80047318(void) {
    s16 t;
    if (D_8009D018 != 0) {
        func_800474AC();
    }
    t = (u16)D_8009D018 + 1;
    switch (t) {
    case 2:
        D_8009D014 -= 8;
        if (D_8009D014 <= 0) {
            D_8009D018 = 3;
            D_8009D014 = 0;
        }
        break;
    case 4:
        D_8009D01A += 4;
        if (D_8009D01A > 0x20) {
            D_8009D018 = 5;
            D_8009D01A = 0x20;
            D_800D74FC = D_8009CDA8;
        }
        break;
    case 5:
        D_8009D01A -= 4;
        if (D_8009D01A <= 0) {
            D_8009D018 = -1;
            D_8009D014 = 0;
            D_8009D01A = 0;
        }
        break;
    case 6:
    default:
        break;
    case 0:
        D_8009D014 += 8;
        if (D_8009D014 > 0x40) {
            D_8009D018 = 2;
            D_8009D014 = 0x40;
        }
        break;
    case 3:
        D_8009D014 += 8;
        if (D_8009D014 > 0x40) {
            D_8009D018 = 0;
            D_8009D014 = 0x40;
        }
        break;
    }
}

#ifdef NON_MATCHING
void func_800474AC(void) {
    extern POLY_FT4 D_800D7AB8[];
    extern POLY_FT4 D_800D7930[];
    extern POLY_FT4 D_800D46D8[][8];
    s16 i;
    s16 j;
    s16 h;
    POLY_FT4 *p;

    h = 6;
    for (i = 0; i < 2; i++) {
        D_800D7AB8[i].y0 = D_800D7AB8[i].y1 = D_8009D014 - 0x27;
        D_800D7AB8[i].y2 = D_800D7AB8[i].y3 = D_8009D014 - 0x1B;
        D_800D7930[i].y0 = D_800D7930[i].y1 = D_8009D014 - 0x27;
        D_800D7930[i].y2 = D_800D7930[i].y3 = D_8009D014 - 0xB;
        p = D_800D46D8[i];
        for (j = 0; j < D_8009D020; j++) {
            p->y0 = p->y1 = D_8009D014 - 0x21;
            p->y2 = p->y3 = D_8009D014 - (0x21 - h);
            p++;
        }
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800474AC);
#endif

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

void func_80047A04(void) {
    s16 i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            Quad *q = (Quad *)&D_800D7A20[i][j];
            func_80088C44(q);
            q->r0 = 0; q->g0 = 0; q->b0 = 0;
            q->r1 = 0; q->g1 = 0; q->b1 = 0;
            q->r2 = 0; q->g2 = 0; q->b2 = 0;
            q->r3 = 0; q->g3 = 0; q->b3 = 0;
            if (i == 0) {
                q->x0 = 0; q->y0 = 0; q->x1 = 0x200; q->y1 = 0;
                q->x2 = 0; q->y2 = 0; q->x3 = 0x200; q->y3 = 0;
            } else {
                q->x0 = 0; q->y0 = 0xF0; q->x1 = 0x200; q->y1 = 0xF0;
                q->x2 = 0; q->y2 = 0xF0; q->x3 = 0x200; q->y3 = 0xF0;
            }
        }
    }
}

void func_80047B50(s32 buf) {
    extern POLY_G4 D_800D7A20[2][2];
    u32 *ot;
    POLY_G4 *p;
    s32 b;

    ot = (u32 *)D_800B8EFC[D_800B8F24][1];
    if (D_8009D00E != 0) {
        D_8009D01A = g_Args->displayH;
    }
    b = 0xF0;
    D_800D7A20[0][buf].y0 = 0;
    D_800D7A20[0][buf].y1 = 0;
    D_800D7A20[0][buf].y2 = D_8009D01A;
    D_800D7A20[0][buf].y3 = D_8009D01A;
    D_800D7A20[1][buf].y0 = b - D_8009D01A;
    D_800D7A20[1][buf].y1 = b - D_8009D01A;
    D_800D7A20[1][buf].y2 = b;
    D_800D7A20[1][buf].y3 = b;
    if (D_8009CFBB != 0) {
        p = &D_800D7A20[0][buf];
        p->r0 = 0x2D; p->g0 = 0x38; p->b0 = 0x41;
        p->r1 = 5; p->g1 = 7; p->b1 = 0xB;
        p->r2 = 5; p->g2 = 7; p->b2 = 0xB;
        p->r3 = 0x1A; p->g3 = 0x23; p->b3 = 0x2A;
        p = &D_800D7A20[1][buf];
        p->r0 = 0x2D; p->g0 = 0x38; p->b0 = 0x41;
        p->r1 = 5; p->g1 = 7; p->b1 = 0xB;
        p->r2 = 5; p->g2 = 7; p->b2 = 0xB;
        p->r3 = 0x1A; p->g3 = 0x23; p->b3 = 0x2A;
    } else {
        p = &D_800D7A20[0][buf];
        p->r0 = 0; p->g0 = 0; p->b0 = 0;
        p->r1 = 0; p->g1 = 0; p->b1 = 0;
        p->r2 = 0; p->g2 = 0; p->b2 = 0;
        p->r3 = 0; p->g3 = 0; p->b3 = 0;
        p = &D_800D7A20[1][buf];
        p->r0 = 0; p->g0 = 0; p->b0 = 0;
        p->r1 = 0; p->g1 = 0; p->b1 = 0;
        p->r2 = 0; p->g2 = 0; p->b2 = 0;
        p->r3 = 0; p->g3 = 0; p->b3 = 0;
    }
    ((PTag *)&D_800D7A20[0][buf])->addr = ((PTag *)ot)->addr;
    ((PTag *)ot)->addr = (u32)&D_800D7A20[0][buf];
    ((PTag *)&D_800D7A20[1][buf])->addr = ((PTag *)ot)->addr;
    ((PTag *)ot)->addr = (u32)&D_800D7A20[1][buf];
}

void func_80047D74(void) {
    s16 i;

    for (i = 0; i < 2; i++) {
    }
    func_80047DB4();
}

void func_80047DB4(void) {
}

M2C_UNK func_80047DBC() {
    s16 i;
    u8 *p;
    s16 v;
    rand();
    for (i = 0; i < 2; i++) {
        D_800D45F4 = 0;
        p = D_800D77B8 + i * 0x18;
        func_80081C54(p);
        ((void (*)(u8 *, s32, s32, s32))func_80044BBC)(p, 0xC0, 0x10, 0x10);
        func_80081CE4(p, 1);
        func_80044B3C(p, 0, D_800D45F4, 0x200, D_800D45F4, 0, (s16)((u16)D_800D45F4 + 0xF0), 0x200, (s16)((u16)D_800D45F4 + 0xF0));
        ((void (*)(s32, s32))func_80049B7C)(i, D_800D45F4);
        func_80049B84(i, D_800D45F4);
        func_80047ECC(i, D_800D45F4);
    }
}

void func_80047ECC(s32 arg0, s32 arg1) {
    POLY_FT4 *p;
    s16 i;
    s16 x;

    p = D_800D46D8[arg0];
    i = 0;
    do {
        func_80081C84(p);
        ((void (*)(void *, s32, s32, s32))func_80044BAC)(p, 0x80, 0x80, 0x80);
        func_80081CE4(p, 0);
        p->tpage = GetTPage(0, 0, 0x22A, 0x1A2);
        p->u0 = 0xA8; p->v0 = 0xA2;
        p->u1 = 0xB4; p->v1 = 0xA2;
        p->u2 = 0xA8; p->v2 = 0xA8;
        p->u3 = 0xB4; p->v3 = 0xA8;
        x = D_8009D028[i];
        p->x0 = x + 0x1D; p->y0 = arg1 + 0x1F;
        p->x1 = x + 0x29; p->y1 = arg1 + 0x1F;
        p->x2 = x + 0x1D; p->y2 = arg1 + 0x25;
        p->x3 = x + 0x29; p->y3 = arg1 + 0x25;
        p->clut = GetClut(0x2A0, 0x1C5);
        i++;
        p++;
    } while (i < 8);
    p = &D_800D7930[arg0];
    func_80081C84(p);
    ((void (*)(void *, s32, s32, s32))func_80044BAC)(p, 0x80, 0x80, 0x80);
    func_80081CE4(p, 1);
    p->tpage = GetTPage(0, 0, 0x2C0, 0x180);
    p->u0 = 0; p->v0 = 0x80;
    p->u1 = 0xA7; p->v1 = 0x80;
    p->u2 = 0; p->v2 = 0x9C;
    p->u3 = 0xA7; p->v3 = 0x9C;
    p->x0 = 0x1E; p->y0 = 0x19;
    p->x1 = 0xC6; p->y1 = 0x19;
    p->x2 = 0x1E; p->y2 = 0x35;
    p->x3 = 0xC6; p->y3 = 0x35;
    p->clut = GetClut(0x230, 0x1F5);
    p = &D_800D7AB8[arg0];
    func_80081C84(p);
    ((void (*)(void *, s32, s32, s32))func_80044BAC)(p, 0x80, 0x80, 0x80);
    func_80081CE4(p, 0);
    p->tpage = GetTPage(0, 0, 0x280, 0x180);
    p->u0 = 0; p->v0 = 0x80;
    p->u1 = 0x53; p->v1 = 0x80;
    p->u2 = 0; p->v2 = 0x8C;
    p->u3 = 0x53; p->v3 = 0x8C;
    p->x0 = 0x1E; p->y0 = 0x1A;
    p->x1 = 0x72; p->y1 = 0x1A;
    p->x2 = 0x1E; p->y2 = 0x26;
    p->x3 = 0x72; p->y3 = 0x26;
    p->clut = GetClut(D_800D7BE8, D_800D7BF0);
}

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

void func_800483D8(void) {
    s32 i;
    Rec20 *e;

    e = D_800B0CB0;
    for (i = 0; i < 5; i++, e++) {
        e->state = 0;
        e->src->flags &= ~0x100;
    }
}

void func_8004841C(void) {
    VECTOR sq;
    s32 otz;
    s16 i;
    Rec20 *e;
    s16 sx;
    s16 sy;
    s32 d;
    s32 r;
    s16 b;
    s32 b3;

    D_800D7DF8 = 0;
    D_800D45F4 = 0;
    GsGetLs(D_800D3238, D_800B0808);
    for (i = 0; i < 5; i++) {
        if (D_800D7DF8 >= 2) {
            break;
        }
        e = &D_800B0CB0[i];
        if (e->state == 0) {
            continue;
        }
        D_1F8003D4 = e->x;
        D_1F8003D8 = e->y;
        D_1F8003DC = e->z;
        D_1F8003E8[0] = D_1F8003D4;
        D_1F8003E8[1] = D_1F8003D8;
        D_1F8003E8[2] = D_1F8003DC;
        if (func_80050A10((VECTOR *)&D_800D30C0, (VECTOR *)D_1F8003E8, g_Player, g_Player) == 0) {
            continue;
        }
        ((SVECTOR *)D_1F8003E0)->vx = D_1F8003D4;
        ((SVECTOR *)D_1F8003E0)->vy = D_1F8003D8;
        ((SVECTOR *)D_1F8003E0)->vz = D_1F8003DC;
        gte_SetRotMatrix(D_800B0808);
        gte_SetTransMatrix(D_800B0808);
        gte_ldv0(D_1F8003E0);
        gte_rtps();
        gte_stsxy(&D_800B0CB0[i].unk14);
        gte_stszotz(&otz);
        e->unk18 = otz;
        if (e->unk18 == 0) {
            continue;
        }
        sx = e->unk14 - 0x100;
        sy = e->unk16 - 0x78;
        D_1F8003E8[0] = D_1F8003D4 - g_Camera.posX;
        D_1F8003E8[1] = D_1F8003D8 - g_Camera.posY;
        D_1F8003E8[2] = D_1F8003DC - g_Camera.posZ;
        gte_ldlvl(D_1F8003E8);
        gte_sqr0();
        gte_stlvnl(&sq);
        r = (u16)e->unk4;
        d = sq.vx + sq.vy + sq.vz;
        D_8009CF4C = d;
        b = 0x80 - d / ((r * r) >> 1);
        b = (b < 0) ? 0 : ((b > 0x80) ? 0x80 : b);
        if (b == 0) {
            continue;
        }
        D_800D7A1C[0] = (D_800B0CA8 * (b3 = b * 3)) >> 9;
        D_800D7A1C[1] = (D_800B0CA0 * b3) >> 9;
        D_800D7A1C[2] = (D_800B0CA4 * b3) >> 9;
        func_80048794(sx, sy);
    }
}

#ifdef NON_MATCHING
void func_80048794(s16 arg0, s16 arg1) {
    s32 i;
    u8 (*e)[8] = D_8009D040;
    s16 t1, t2, t3, a1, a3, t0;
    s32 v1, a2;
    u8 w0, w1, w2, c0, c1;
    POLY_FT4 *P;
    D_800D45F4 = 0;
    for (i = 0; i < 8; i++, e++) {
        P = &D_800D4984[D_800D7DF8][4 * i][1 - D_800B8F24];
        v1 = (*e)[2] - 0x80;
        c0 = (*e)[0];
        c1 = (*e)[1];
        t1 = ((v1 * arg0) >> 6) + 0x100;
        t3 = t1 - c0;
        t2 = t1 + c0;
        Q(0).x0 = t3; Q(0).x1 = t1; Q(0).x2 = t3; Q(0).x3 = t1;
        a2 = ((v1 * arg1) >> 6) + 0x78;
        a1 = D_800D45F4 + (a2 - c1);
        a3 = a2 + D_800D45F4;
        Q(0).y0 = a1; Q(0).y1 = a1; Q(0).y2 = a3; Q(0).y3 = a3;
        Q(1).x0 = t2; Q(1).y0 = a1; Q(1).x1 = t1; Q(1).y1 = a1;
        Q(1).x2 = t2; Q(1).y2 = a3; Q(1).x3 = t1; Q(1).y3 = a3;
        t0 = D_800D45F4 + (a2 + c1);
        Q(2).x0 = t3; Q(2).y0 = t0; Q(2).x1 = t1; Q(2).y1 = t0;
        Q(2).x2 = t3; Q(2).y2 = a3; Q(2).x3 = t1; Q(2).y3 = a3;
        Q(3).x0 = t2; Q(3).y0 = t0; Q(3).x1 = t1; Q(3).y1 = t0;
        Q(3).x2 = t2; Q(3).y2 = a3; Q(3).x3 = t1; Q(3).y3 = a3;
        if (D_8009CABD != 0) {
            w0 = 0; w1 = (*e)[5]; w2 = 0;
        } else {
            w0 = 0;
            if (D_8009CABE != 0) {
                w1 = 0; w2 = 0;
            } else {
                w0 = (*e)[4]; w1 = (*e)[5]; w2 = (*e)[6];
            }
        }
        P = &D_800D4984[D_800D7DF8][4 * i][1 - D_800B8F24];
        Q(0).r0 = w0; Q(0).g0 = w1; Q(0).b0 = w2;
        Q(1).r0 = w0; Q(1).g0 = w1; Q(1).b0 = w2;
        Q(2).r0 = w0; Q(2).g0 = w1; Q(2).b0 = w2;
        Q(3).r0 = w0; Q(3).g0 = w1; Q(3).b0 = w2;
    }
    D_800D7DF8 += 1;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80048794);
#endif

M2C_UNK func_80048B4C(msg, prio, flag)
M2C_UNK *msg; u8 prio; u8 flag; {
    s16 i;
    s16 j;
    Msg *e;
    Msg *f;
    u32 p;
    u8 t;
    s32 tm;
    s16 x;
    s16 y;

    tm = 0xAC;
    if (D_8009CAA4 == 0 && g_CutScene != 2 && g_CutScene != 3) {
        if (!prio && g_Args->cdArchive == 0) {
            func_8006B1DC((s32)&D_800A85C4);
        }
        if ((s16)func_80048DE4((char *)msg) != 0) {
            return 0;
        }
        if (prio == D_800D4958.b && strcmp((char *)&D_800D4958, (char *)msg) == 0) {
        dup:
            return 3;
        }
        goto loop;
    over:
        D_800D7CB4 = 4;
        return 2;
    loop:
        i = 0;
        p = prio;
        for (; i < 5; i++) {
            e = &((Msg *)&D_800D5DA4)[i];
            if (p == e->unk23) {
                if (strcmp(e->text, (char *)msg) == 0) goto dup;
                if (p == 0xFF) {
                    strcpy(e->text, (char *)msg);
                    x = D_800D86C4;
                    y = D_800D86C8;
                    e->unk24 = tm;
                    e->flag = flag;
                    e->x = x;
                    e->y = y;
                    return 1;
                }
            } else if (e->unk23 < p) {
                for (j = D_800D7CB4; i < j; j--) {
                    func_800477C0(&((Msg *)&D_800D5DA4)[j], &((Msg *)&D_800D5DA4)[j - 1]);
                }
                f = &((Msg *)&D_800D5DA4)[i];
                strcpy(f->text, (char *)msg);
                f->flag = flag;
                f->unk23 = prio;
                f->unk24 = tm;
                f->x = D_800D86C4;
                f->y = D_800D86C8;
                t = ++D_800D7CB4;
                if (t < 5) return 1;
                goto over;
            }
        }
    }
    return 0;
}

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

void func_80048E60(void) {
  s16 i;
  u8 *p;
  E24 *e;
  func_80048FE8();
  i = 0;
  do
  {
    e = &D_800D8154[i];
    e->w0 = 0x50000000;
    e->hC = GetTPage(0, 0, D_800AC4E0, D_800AC4E4);
    e->h8 = 0x11;
    e->hA = 8;
    e->h4 = (i * 0x12) + 0x1E;
    e->h10 = D_800D86C4;
    e->h6 = 0xAC;
    e->h12 = D_800D86C8;
    e->b0E = D_800AE940[0];
    i++;
    e->b0F = D_800AE940[1];
  }
  while (i < 0x22);
  i = 0;
  do
  {
    p = (&D_800D5DA4) + (i * 0x2C);
    i++;
    p[0] = 0;
    p[0x23] = 0;
  }
  while (i < 5);
  D_800D4958.a = 0;
  D_800D4958.b = 0;
  D_800D861C = 0;
  D_800D5ED0 = 0;
  D_800D8620 = 0;
  D_800D7CB4 = 0;
  D_800D7550 = 0xAC;
}

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

#ifdef NON_MATCHING
void func_80049220(u8 *arg0) {
    POLY_FT4 *p;
    s16 i;
    s32 x;
    s16 y;
    s32 w;
    s32 h;
    s16 cx;

    w = 0x40;
    h = 0x20;
    cx = 0x200;
    x = ((s16)g_Args->displayW >> 1) - 0x20;
    y = ((s16)g_Args->displayH >> 1) - 0x10;
    for (i = 0; i < 2; i++) {
        p = &D_800D7C64[i];
        func_80081C84(p);
        ((void (*)(void *, s32, s32, s32))func_80044BAC)(p, 0x80, 0x80, 0x80);
        func_80081CE4(p, 1);
        p->clut = GetClut(cx, 0x1FF);
        p->tpage = GetTPage(0, 0, 0x220, 0x180);
        ((void (*)(void *, s16, s16, s16, s16, s16, s16, s16, s16))func_80044B04)(p, x, y, x + w, y, x, y + h, x + w, y + h);
        ((void (*)(void *, s32, s32, s32, s32, s32, s32, s32, s32))func_80044B74)(p, 0x80, 0x80, 0x80 + w, 0x80, 0x80, 0x80 + h, 0x80 + w, 0x80 + h);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80049220);
#endif

void func_800493C0(void) {
    s32 i;
    u8 *p;
    s32 x;
    s32 y;

    y = (s16)g_Args->displayH >> 1;
    x = (s16)g_Args->displayW >> 1;
    p = D_800D7C24[0];
    for (i = 0; i < 2; i++) {
        p[3] = 3;
        p[7] = 0x40;
        p[4] = 0;
        p[5] = 0xFF;
        p[6] = 0;
        M2C_FIELD(p, s16 *, 8) = x;
        M2C_FIELD(p, s16 *, 0xA) = y - 2;
        M2C_FIELD(p, s16 *, 0xC) = x;
        M2C_FIELD(p, s16 *, 0xE) = y + 2;
        p += 0x10;
        p[3] = 3;
        p[7] = 0x40;
        p[4] = 0;
        p[5] = 0xFF;
        p[6] = 0;
        M2C_FIELD(p, s16 *, 8) = x - 3;
        M2C_FIELD(p, s16 *, 0xA) = y;
        M2C_FIELD(p, s16 *, 0xC) = x + 3;
        M2C_FIELD(p, s16 *, 0xE) = y;
        p += 0x10;
    }
}

void func_80049470(u8 *obj, s16 *out) {
    u8 *m;
    s32 type;
    s32 s;
    s32 *t;

    m = *(u8 **)(obj + 0x90);
    type = obj[5];
    ((SVECTOR *)D_1F8003E0)->vx = (*(s16 *)(obj + 0x1E) + *(s16 *)(obj + 0x1C)) >> 1;
    if (*(s32 *)(*(u8 **)(obj + 0x94) + 0x24) & 0x400) {
        ((SVECTOR *)D_1F8003E0)->vy = (*(s16 *)(obj + 0x22) + *(s16 *)(obj + 0x20)) >> 1;
    } else {
        s = *(s16 *)(obj + 0x22) + *(s16 *)(obj + 0x20);
        ((SVECTOR *)D_1F8003E0)->vy = (s >> 1) + (s >> 2) + (s >> 3);
    }
    ((SVECTOR *)D_1F8003E0)->vz = (*(s16 *)(obj + 0x26) + *(s16 *)(obj + 0x24)) >> 1;
    if (type == 6) {
        ((SVECTOR *)D_1F8003E0)->vy -= *(s16 *)(obj + 0x22);
    }
    gte_SetRotMatrix(m + 0x14);
    if (*(s32 *)(obj + 0x4C) < 0) {
        gte_ldv0(D_1F8003E0);
        gte_rtv0();
        gte_stlvnl(D_1F8003E8);
    } else {
        D_1F8003E8[0] = ((SVECTOR *)D_1F8003E0)->vx;
        D_1F8003E8[1] = ((SVECTOR *)D_1F8003E0)->vy;
        D_1F8003E8[2] = ((SVECTOR *)D_1F8003E0)->vz;
    }
    t = (s32 *)(*(u8 **)(obj + 0x90) + 0x28);
    D_1F8003E8[0] += t[0];
    D_1F8003E8[1] += t[1];
    D_1F8003E8[2] += t[2];
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

#ifdef NON_MATCHING
s32 func_80049D60(void) {
    s16 i;
    s32 s;
    u8 *p;
    u8 *b2 = D_800D7DFC + 0x50;
    D_800D7DFC[0x32C] = 0;
    i = 0;
    do {
        s = i * 0x28;
        p = s + b2;
        INIT(p, 1);
        QUAD(p, g_Args->displayW - 0x72, g_Args->displayH - 0x3B, g_Args->displayW - 0x22, g_Args->displayH - 0x1D);
        p = s + D_800D7E9C;
        INIT(p, 1);
        QUADY(p, 0x22, g_Args->displayH - 0x3B, 0x72, g_Args->displayH - 0x1D);
        p = s + D_800D7EEC;
        INIT(p, 0);
        QUAD(p, g_Args->displayW - 0x72, g_Args->displayH - 0x59, g_Args->displayW - 0x22, g_Args->displayH - 0x3B);
        p = s + D_800D7DFC;
        INIT(p, 0);
        S16(p, 0xE, GetClut(0x2F0, 0x1E4));
        {
            s32 tp = GetTPage(0, 0, 0x360, 0x1D0);
            p[0xD] = 0xD0;
            p[0x15] = 0xD0;
            p[0xC] = 0x80;
            p[0x14] = 0xEC;
            p[0x1C] = 0x80;
            p[0x1D] = 0xFF;
            p[0x24] = 0xEC;
            p[0x25] = 0xFF;
            S16(p, 0x16, tp);
        }
        QUAD(p, g_Args->displayW - 0x8A, g_Args->displayH - 0x48, g_Args->displayW - 0x1E, g_Args->displayH - 0x19);
        p = s + (D_800D7DFC + 0x140);
        INIT(p, 0);
        QUAD(p, g_Args->displayW - 0x6F, g_Args->displayH - 0x46, g_Args->displayW - 0x43, g_Args->displayH - 0x3E);
        p = s + (D_800D7DFC + 0x190);
        INIT(p, 0);
        QUADY(p, 0x2D, g_Args->displayH - 0x46, 0x59, g_Args->displayH - 0x3E);
        p = s + D_800D7FDC;
        INIT(p, 0);
        i += 1;
    } while (i < 2);
    func_8004A294();
    func_8004A294();
    func_8004A294();
    func_8004A294();
    func_8004A294();
    func_8004A294();
    *(s32 *)(D_800D7DFC + 0x338) = 1;
    *(s32 *)(D_800D7DFC + 0x334) = 6;
    return 6;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80049D60);
#endif

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

void func_8004A398(void) {
    s16 pos[2];
    POLY_FT4 *p;
    s32 w;
    s32 h;
    s32 d;
    s16 z;
    u32 *ot;

    w = 0;
    h = 0;
    if (D_800DDDA8 != 0) {
        if (D_8009CD4C == 0) {
            func_80049470((u8 *)D_800DDDA8, pos);
        } else {
            func_8004973C(*(VECTOR *)D_800DDDCC, pos);
        }
        D_800D4480[0] = pos[0];
        D_800D4480[1] = pos[1];
    } else {
        D_800D4480[0] = 0x100;
        D_800D4480[1] = 0x78;
    }
    p = (POLY_FT4 *)D_800DDC70 + D_800B8F24;
    func_80081C84(p);
    func_80081CE4(p, 1);
    if (D_8009CABD != 0) {
        p->r0 = 0;
        p->g0 = 200;
        p->b0 = 0;
    } else if (D_8009CABE != 0) {
        p->r0 = 0;
        p->g0 = 0;
        p->b0 = 0;
    } else if (D_800DDDC0 != 0) {
        p->r0 = 0xE0;
        p->g0 = 0x30;
        p->b0 = 0x10;
    } else {
        p->r0 = 200;
        p->g0 = 200;
        p->b0 = 200;
    }
    switch (D_8009D8F0) {
    case 4:
        p->tpage = GetTPage(0, 1, 0x23C, 0x1B0);
        p->clut = GetClut(0x210, 0x1C1);
        w = 7;
        h = 8;
        p->u0 = 0xF0;
        p->v0 = 0xB0;
        p->u1 = 0xFF;
        p->v1 = 0xB0;
        p->u2 = 0xF0;
        p->v2 = 0xC0;
        p->u3 = 0xFF;
        p->v3 = 0xC0;
        break;
    case 3:
        p->tpage = GetTPage(0, 1, 0x298, 0x1A0);
        p->clut = GetClut(0x220, 0x1C1);
        w = 0x10;
        h = 0x10;
        p->u0 = 0x60;
        p->v0 = 0xA0;
        p->u1 = 0x80;
        p->v1 = 0xA0;
        p->u2 = 0x60;
        p->v2 = 0xC0;
        p->u3 = 0x80;
        p->v3 = 0xC0;
        break;
    case 2:
        p->tpage = GetTPage(0, 1, 0x280, 0x190);
        p->clut = GetClut(0x230, 0x1C1);
        w = 0x18;
        h = 0x18;
        p->u0 = 0;
        p->v0 = 0x90;
        p->u1 = 0x30;
        p->v1 = 0x90;
        p->u2 = 0;
        p->v2 = 0xC0;
        p->u3 = 0x30;
        p->v3 = 0xC0;
        break;
    case 1:
        p->tpage = GetTPage(0, 1, 0x320, 0x180);
        p->clut = GetClut(0x200, 0x1C1);
        w = 0x20;
        h = 0x20;
        p->u0 = 0x80;
        p->v0 = 0x80;
        p->u1 = 0xC0;
        p->v1 = 0x80;
        p->u2 = 0x80;
        p->v2 = 0xC0;
        p->u3 = 0xC0;
        p->v3 = 0xC0;
        break;
    }
    h /= 2;
    D_800DE0D0 = (D_800DE0D0 < 0) ? 0 : (D_800DE0D0 > 0x1D4C) ? 0x1D4C : D_800DE0D0;
    d = 10000 - D_800DE0D0;
    w = w * d / 10000;
    w *= 2;
    p->x0 = D_800D4480[0] - w;
    p->x1 = D_800D4480[0] + w;
    p->x2 = D_800D4480[0] - w;
    p->x3 = D_800D4480[0] + w;
    h = h * d / 10000;
    h *= 2;
    p->y0 = D_800D4480[1] - h;
    p->y1 = D_800D4480[1] - h;
    p->y2 = D_800D4480[1] + h;
    p->y3 = D_800D4480[1] + h;
    ot = D_800B8F88[D_800B8F24].org;
    p->tag = (p->tag & 0xFF000000) | (*ot & 0xFFFFFF);
    *D_800B8F88[D_800B8F24].org = (*D_800B8F88[D_800B8F24].org & 0xFF000000) | ((u32)p & 0xFFFFFF);
}

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

#ifdef NON_MATCHING
void func_8004A8E4(void) {
    s16 a;
    s16 b;
    s16 c;
    s32 t;
    u8 *p;
    D_8009D002 = 0;
    a = 0;
    do {
        b = 0;
        do {
            p = (u8 *)(a * 0x50) + (b * 0x28 + (s32)D_800D7D08);
            func_80081C84(p);
            c = b + 1;
            b = c;
            do { } while (0);
            p[4] = 0x80;
            p[5] = 0x80;
            p[6] = 0x80;
            t = c < 2;
            *(u16 *)(p + 0xE) = (D_800AC4DC << 6) | ((D_800AC4D8 >> 4) & 0x3F);
            *(u16 *)(p + 0x16) = D_800AC4E8;
        } while (t);
        c = a + 1;
        a = c;
    } while (c < 3);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004A8E4);
#endif

#ifdef NON_MATCHING
void func_8004A9E4(u8 *arg0, u8 *arg1) {
    s32 flg;
    u16 sxy[2];
    s32 n;
    s32 h;
    s16 s3;
    s32 s1;
    u8 *s0;
    s32 a1, a2, t1, t5, t8;
    u16 a3, t3;
    s16 i;
    u8 *tbl;
    u16 t;
    u8 *base;
    s3 = 0x10;
    base = D_800AE940;
    s1 = base[0x82];
    s0 = 0;
    if (arg0 != 0 && arg0[4] != 0 && *(s32 *)(arg0 + 0x34) > 0) {
        if (*(s32 *)(*(u8 **)arg0 + 0x20) <= 0) {
            func_8006B230(0, D_800A85F8);
        }
        ((SVECTOR *)D_1F8003E0)->vx = *(u16 *)(*(u8 **)(arg0 + 0x90) + 0x28);
        t = *(u16 *)(*(u8 **)(arg0 + 0x90) + 0x2C);
        ((SVECTOR *)D_1F8003E0)->vy = t;
        ((SVECTOR *)D_1F8003E0)->vy = t - 0x384;
        ((SVECTOR *)D_1F8003E0)->vz = *(u16 *)(*(u8 **)(arg0 + 0x90) + 0x30);
        GsGetLs(D_800D3238, &D_800B0848);
        gte_SetRotMatrix(&D_800B0848);
        gte_SetTransMatrix(&D_800B0848);
        gte_ldv0(D_1F8003E0);
        gte_rtps();
        gte_stflg(&flg);
        if (!(flg & ~0x1000)) {
            a1 = *(s32 *)(arg0 + 0x34) * 100 / *(s32 *)(*(u8 **)arg0 + 0x20);
            if (a1 == 0) a1 = 1;
            t1 = a1 / 100;
            a1 = a1 % 100;
            t5 = a1 / 10;
            t8 = a1 % 10;
            gte_stsxy(&sxy);
            gte_stszotz(&n);
            if (n > 0) {
                a3 = sxy[0];
                t3 = sxy[1];
                gte_ReadGeomScreen(&h);
                i = 0;
                do {
                    if (i == 1) {
                        a3 = s1 + (a3 + 2);
                        s3 = t5 + 0x10;
                        s0 = D_800D7D08 + 0x50 + D_800B8F24 * 0x28;
                    } else if (i < 2) {
                        if (i == 0) {
                            s3 = t1 + 0x10;
                            s0 = D_800D7D08 + D_800B8F24 * 0x28;
                        }
                    } else if (i == 2) {
                        a3 = s1 + (a3 + 2);
                        s3 = t8 + 0x10;
                        s0 = D_800D7D08 + 0xA0 + D_800B8F24 * 0x28;
                    }
                    tbl = base + s3 * 8;
                    s1 = tbl[2];
                    s0[0xC] = tbl[0];
                    s0[0xD] = tbl[1];
                    s0[0x14] = tbl[0] + s1;
                    s0[0x15] = tbl[1];
                    s0[0x1C] = tbl[0];
                    s0[0x1D] = tbl[1] + 8;
                    s0[0x24] = tbl[0] + s1;
                    s0[0x25] = tbl[1] + 8;
                    a2 = (h << 5) / n;
                    s1 = (s1 * h * 4) / n;
                    if ((s16)s1 < 0xB) s1 = 0xB;
                    if ((s16)a2 < 5) a2 = 5;
                    if (i != 0 || t1 != 0) {
                        if (i == 1 && t1 == 0 && t5 == 0) {
                        } else {
                            *(u16 *)(s0 + 8) = a3;
                            *(u16 *)(s0 + 0xA) = t3;
                            *(s16 *)(s0 + 0x10) = a3 + s1;
                            *(u16 *)(s0 + 0x12) = t3;
                            *(u16 *)(s0 + 0x18) = a3;
                            *(s16 *)(s0 + 0x20) = a3 + s1;
                            *(s16 *)(s0 + 0x1A) = t3 + a2;
                            *(s16 *)(s0 + 0x22) = t3 + a2;
                            ((PTag *)s0)->addr = ((PTag *)(arg1 + (n >> 2) * 4 + 0x28))->addr;
                            ((PTag *)(arg1 + (n >> 2) * 4 + 0x28))->addr = (u32)s0;
                        }
                    }
                    i++;
                } while (i < 3);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004A9E4);
#endif

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

void func_8004B3BC(V3 *arg0, void *arg1, s32 arg2, s32 arg3) {
    u8 *p;
    PTag *ot;
    PTag *dm;

    ot = (PTag *)D_800B8F88[D_800B8F24].org;
    p = (u8 *)(arg2 * 0x38) + (D_800B8F24 * 0x1C + (s32)D_800D4484);
    p[3] = 6;
    p[7] = 0x30;
    p[4] = 0;
    p[5] = 1;
    p[6] = 2;
    p[0xC] = 0;
    p[0xD] = 1;
    p[0xE] = 2;
    p[0x14] = M2C_FIELD(arg0, u8 *, 0);
    p[0x15] = M2C_FIELD(arg0, u8 *, 4);
    p[0x16] = M2C_FIELD(arg0, u8 *, 8);
    func_80081CE4(p, 1);
    dm = (PTag *)((u8 *)(arg2 * 0x18) + (D_800B8F24 * 0xC + (s32)D_800D4564));
    *(u16 *)(p + 8) = M2C_FIELD(arg1, u16 *, 0);
    *(u16 *)(p + 0xA) = M2C_FIELD(arg1, u16 *, 4);
    *(u16 *)(p + 0x10) = M2C_FIELD(arg1, u16 *, 8);
    *(u16 *)(p + 0x12) = M2C_FIELD(arg1, u16 *, 0xC);
    *(u16 *)(p + 0x18) = M2C_FIELD(arg1, u16 *, 0x10);
    *(u16 *)(p + 0x1A) = M2C_FIELD(arg1, u16 *, 0x14);
    SetDrawMode((M2C_UNK *)dm, 0, 0, GetTPage(0, 1, 0, 0) & 0xFFFF, 0);
    ((PTag *)p)->addr = (ot + 1)->addr;
    (ot + 1)->addr = (u32)p;
    dm->addr = (ot + 1)->addr;
    (ot + 1)->addr = (u32)dm;
}

void func_8004B58C(s32 arg0) {
    V3 v;
    s32 d;
    s32 k;
    s32 s1;
    s32 best;
    s32 best2;
    s32 i1;
    s32 i2;
    s32 n;
    s32 m = 0xA;

    d = arg0;
    k = 0;
    v = *(V3 *)D_800A86C0;
    best = 0;
    best2 = 0;
    i1 = -1;
    i2 = -1;
    s1 = 0;
    if (d == -1) {
        for (; k < 8; k++) {
            n = (D_8009CF34 >> (k * 4)) & m;
            if (d < n) {
                d = k;
            }
        }
    } else {
        D_8009CF34 |= m << (d * 4);
    }
    if (D_8009CF34 != 0) {
        if (d & 1) {
            n = d / 2;
            func_8004B3BC(&v, D_8009D09C + n * 0x18, s1++, n);
        } else {
            n = (d + 1) % 8;
 n /= 2;
            func_8004B3BC(&v, D_8009D09C + n * 0x18, s1++, n);
            n = (d + 7) % 8;
 n /= 2;
            func_8004B3BC(&v, D_8009D09C + n * 0x18, s1++, n);
        }
        for (k = 0; k < 8; k++) {
            n = (D_8009CF34 >> (k * 4)) & m;
            if (best < n && k != d) {
                best2 = best;
                i2 = i1;
                best = n;
                i1 = k;
            } else if (best2 < n && k != d) {
                best2 = n;
                i2 = k;
            }
        }
        if (i1 != -1) {
            v.a = 0xC8 - (0xF - best) * 13;
            if (i1 & 1) {
                n = i1 / 2;
                func_8004B3BC(&v, D_8009D09C + n * 0x18, s1++, n);
            } else {
                n = (d + 1) % 8;
 n /= 2;
                func_8004B3BC(&v, D_8009D09C + n * 0x18, s1++, n);
                n = (i1 + 7) % 8;
 n /= 2;
                func_8004B3BC(&v, D_8009D09C + n * 0x18, s1++, n);
            }
        }
        if (i2 != -1) {
            v.a = 0xC8 - (0xF - best2) * 13;
            if (i2 & 1) {
                n = i2 / 2;
            } else {
                n = (d + 1) % 8;
 n /= 2;
                if (s1 < 4) {
                    func_8004B3BC(&v, D_8009D09C + n * 0x18, s1++, n);
                }
                n = (i2 + 7) % 8;
 n /= 2;
            }
            if (s1 < 4) {
                func_8004B3BC(&v, D_8009D09C + n * 0x18, s1, n);
            }
        }
        i1 = D_8009CF34;
        D_8009CF34 = 0;
        for (n = 7; n >= 0; n--) {
            k = (i1 >> (n * 4)) & m;
            if (k > 0) {
                k--;
                D_8009CF34 += k << (n * 4);
            }
        }
    }
}

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

void func_8004BC70(s32 objs, s32 *model, s32 idx) {
    s32 mode;
    u8 prev;
    u32 i;
    u32 n;
    s32 count;
    u8 *p;
    s16 *run;
    u32 *obj;

    mode = 0;
    i = 0;
    count = 0;
    obj = (u32 *)objs + idx * 7;
    *(u32 **)(model + 2) = obj;
    p = (u8 *)obj[4];
    n = obj[5];
    run = (s16 *)p;
    for (; i < n; i++, count++) {
        prev = mode;
        mode = p[3];
        if (prev != 0 && mode != prev) {
            *run = count;
            count = 0;
            run = (s16 *)p;
        }
        switch (mode & 0xFD) {
        case 0x24:
            p += 0x18;
            break;
        case 0x25:
        case 0x34:
            p += 0x1C;
            break;
        case 0x35:
            p += 0x24;
            break;
        default:
            sprintf(g_Args->exePath, D_800A86CC, mode);
            func_8006B1DC((M2C_UNK *)g_Args->exePath);
            break;
        }
    }
    *run = count;
}

void func_8004BDA8(void) {
  s32 i;
  s32 j;
  s32 new_var;
  s32 n = 0x1C;
  u32 k;
  s32 *p;
  u8 *q;
  i = 0;
  if (D_800D86E4 > 0)
  {
    do
    {
      p = (s32 *) (g_Args->unk228 + (i * 0x70));
      new_var = n;
      for (j = 0; j < new_var; j++)
      {
        *(p++) = 0;
      }

      i++;
    }
    while (i < D_800D86E4);
  }
  i = 0;
  if (D_800D86DC != 0)
  {
    do
    {
      k = 0;
      q = (u8 *) (D_800D86E0 + (i * 0x7E));
      do
      {
        *q = 0;
 do { k++; } while (0);
        q++;
      }
      while (k < 0x7E);
      i++;
    }
    while (i < D_800D86DC);
  }
  func_80066B14();
  func_80067F18();
}

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

void func_8004C0C0(s32 arg0, void *arg1) {
    DstT *d = arg1;
    s32 var_v1;
    SrcT *s;
    s32 *var_a0;
    s32 a;
    s32 b;
    s32 c;
    s32 e;

    if (arg0 >= D_800D86E4) {
        sprintf(g_Args->exePath, (char *)&D_800A8764, arg0);
        func_8006B230(0, (s32)g_Args->exePath);
    }
    s = (SrcT *)(g_Args->unk228 + arg0 * 0x70);
    d->f0 = s;
    d->f5 = s->f1;
    d->f6 = s->f0;
    d->f7 = s->f2;
    d->fA = s->f8;
    d->f8 = s->f7;
    a = s->f10;
    b = s->f14;
    c = s->f18;
    e = s->f1C;
    d->f1C = a;
    d->f20 = b;
    d->f24 = c;
    d->f28 = e;
    d->f52 = s->f44;
    d->f54 = s->f46;
    d->f50 = s->f4C;
    d->f56 = s->f4E;
    d->f34 = s->f20;
    d->f58 = s->f24;
    d->fC = s->f9;
    d->f38 = s->f2A;
    d->f3A = s->f2C;
    d->fD = s->fA;
    d->f3C = s->f30;
    d->f3E = s->f32;
    d->f40 = s->f34;
    d->fE = s->f28;
    d->f7C = s->f54;
    d->f78 = s->f58;
    d->f44 = s->f38;
    var_a0 = (s32 *)((u8 *)d + 0x98);
    d->f48 = s->f3C;
    d->f4C = s->f40;
    var_v1 = 5;
    d->f4 = 0;
    d->f18 = 0;
    d->f10 = 0;
    d->f14 = 0;
    d->f2C = 0;
    d->f30 = 0;
    d->f6A = 0;
    d->f60 = 0;
    d->f64 = 0;
    d->f66 = 0;
    d->f67 = 0;
    d->f68 = 0;
    d->f6C = 0;
    d->f70 = 0;
    d->f74 = 0;
    d->f80 = 0;
    d->f84 = 0;
    d->f88 = 0;
    d->f8C = 0;
    d->f5C = 0;
    do {
        *var_a0 = 0;
        var_v1 -= 1;
        var_a0 += 1;
    } while (var_v1 >= 0);
    d->fB0 = 0;
    d->fCC = 0;
    d->fCD = 0;
    d->fCE = 0;
    a = D_800B8F28[0];
    b = D_800B8F28[1];
    c = D_800B8F28[2];
    e = D_800B8F28[4];
    d->fD0 = a;
    d->fD4 = b;
    d->fD8 = c;
    d->fE0 = e;
    a = D_800B8F28[5];
    b = D_800B8F28[6];
    c = D_800B8F28[8];
    e = D_800B8F28[9];
    d->fF8 = 0x3E8;
    d->f108 = 0;
    d->f10C = 0;
    d->f100 = 0;
    d->f104 = 0;
    d->f9 = 0;
    d->f114 = 0;
    d->fE4 = a;
    d->fE8 = b;
    d->fF0 = c;
    d->fF4 = e;
}

#ifdef NON_MATCHING
void func_8004C368(s32 idx, ObjC368 *o) {
    s32 *q;
    s32 n;
    u8 *r;
    s32 *g;
    s32 t0, t1, t2, t3;
    r = (u8 *)D_8009D0FC + idx * 0x70;
    q = (s32 *)((u8 *)o + 0x98);
    o->b6 = idx;
    o->p0 = r;
    n = 5;
    o->b3A = r[0x2C];
    o->c3C = *(PairC368 *)(r + 0x30);
    o->b4 = 0;
    o->w18 = 0;
    o->w10 = 0;
    o->h14 = 0x1B57;
    o->w2C = 0;
    o->w30 = 0;
    o->h6A = 0;
    o->w60 = 0;
    o->h64 = 0;
    o->b66 = 0;
    o->b67 = 0;
    o->h68 = 0;
    o->w6C = 0;
    o->w70 = 0;
    o->w74 = 0;
    o->w80 = 0;
    o->w84 = 0;
    o->w88 = 0;
    o->w8C = 0;
    o->w5C = 0;
    do {
        *q = 0;
        n -= 1;
        q += 1;
    } while (n >= 0);
    o->hB0 = 0;
    o->bCC = 0;
    o->bCD = 0;
    o->bCE = 0;
    g = D_800B8F28;
    t0 = D_800B8F28[0];
    t1 = g[1];
    t2 = g[2];
    t3 = g[4];
    o->wD0 = t0;
    o->wD4 = t1;
    o->wD8 = t2;
    o->wE0 = t3;
    o->wF8 = 0x3E8;
    o->w108 = 0;
    o->w10C = 0;
    o->w100 = 0;
    o->w104 = 0;
    o->b9 = 0;
    o->w114 = 0;
    t0 = g[5];
    t1 = g[6];
    t2 = g[8];
    t3 = g[9];
    o->wE4 = t0;
    o->wE8 = t1;
    o->wF0 = t2;
    o->wF4 = t3;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004C368);
#endif

void func_8004C498(s32 idx, u8 *e) {
    u8 *w;

    w = (u8 *)g_Args->unk228 + idx * 0x70;
    e[0] = w[0];
    e[3] = w[2];
    *(s32 *)(e + 4) = *(s32 *)(w + 0x54);
    *(s32 *)(e + 0x10) = *(s32 *)(w + 0x58);
    *(s32 *)(e + 0x14) = *(s32 *)(w + 0x38);
    *(u16 *)(e + 0xE) = *(u16 *)(w + 0x24);
    e[1] = 0;
    *(s16 *)(e + 8) = 0;
    *(s16 *)(e + 0xA) = 0;
    *(s16 *)(e + 0xC) = 0;
}

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

s32 func_8004C558(u8 *arg0, s32 arg1) {
    s32 m;
    s16 t;

    m = arg0[0x50];
    *(u16 *)(arg0 + 0x48) += arg1;
    t = *(u16 *)(arg0 + 0x48);
    if (m < t) {
        *(u16 *)(arg0 + 0x48) = arg0[0x50];
        arg1 = func_8004C50C(arg0, t - m);
    }
    *(s16 *)(arg0 + 0x44) = *(u16 *)(arg0 + 0x48) + *(u16 *)(arg0 + 0x4A);
    return arg1;
}

void *func_8004C5C8(s32 arg0, s32 arg1) {
    s32 *e = &D_8009D130[arg0 * 4];
    Weapon *w = &g_Weapons[e[0]];
    char *ret;
    s32 n;
    register s32 x REG("a2") = arg1;
    ret = 0;
    if (*(s16 *)&w->maxAmmo != 0 && *(s16 *)&w->maxAmmo <= w->reserve) {
        func_80048B4C((M2C_UNK *)&D_800A8794, 0x80, 0);
        return 0;
    }
    n = ((s32 (*)(Weapon *, s32, s32))func_8004C558)(w, x, x);
    if (e[3] != 0) {
        sprintf(g_Args->exePath, (char *)e[3], n);
        ret = g_Args->exePath;
    }
    return ret;
}

char *func_8004C698(s32 arg0, s32 arg1) {
    s32 *e = &D_8009D130[arg0 * 4];
    Weapon *w = &g_Weapons[e[0]];
    char *ret;
    s32 n;
    register s32 x REG("a2") = arg1;
    ret = 0;
    if (*(s16 *)&w->maxAmmo != 0 && *(s16 *)&w->maxAmmo <= w->reserve) {
        func_80048B4C((M2C_UNK *)&D_800A8794, 0x80, 0);
        return 0;
    }
    n = func_8004C50C(w, x);
    if (e[3] != 0) {
        sprintf(g_Args->exePath, (char *)e[3], n);
        ret = g_Args->exePath;
    }
    return ret;
}

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

void *func_8004C82C(void *arg0) {
    void *r;
    s32 *p;
    s32 i;
    s32 id;
    Weapon *w;
    s32 a1;
    u8 t;
    r = 0;
    p = D_8009D130;
    i = 0;
    t = *((u8 *)arg0 + 6);
    do {
        if (p[1] == t) {
            id = p[0];
            if (id >= 0) {
                w = &g_Weapons[id];
                a1 = func_8004CB2C(arg0, w);
                if (!(((u32)w->flags >> 11) & 1)) {
                    w->flags |= 0x1800;
                    func_8004C558(w, a1);
                    r = (void *)p[2];
                } else {
                    r = func_8004C5C8(i, a1);
                }
                if (r != 0) {
                    func_80026F30(D_8009D124, 0);
                }
            }
        }
        i++;
        p += 4;
    } while (i < 0x13);
    return r;
}

void *func_8004C944(void *obj) {
    void *ret;
    s32 type;
    s32 slot;

    ret = NULL;
    if (*((u8 *)obj + 6) == D_8009D108 || *((u8 *)obj + 6) == D_8009D10A) {
        type = *(s32 *)((u8 *)obj + 0x34);
        slot = D_8009D130[type * 4];
        if (slot >= 0) {
            ret = func_8004C698(type, func_8004CB2C(obj, &g_Weapons[slot]));
        }
    } else if (*((u8 *)obj + 6) == D_8009D10C) {
        slot = D_8009D130[0];
        if (slot >= 0) {
            ret = func_8004C5C8(0, func_8004CB2C(obj, &g_Weapons[slot]));
        }
    }
    if (ret != NULL) {
        func_80026F30(D_8009D124, 0);
    }
    return ret;
}

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

s32 func_8004CB2C(void *obj, Weapon *w) {
    s32 n;
    s32 r;

    if (w->clipSize < 2) {
        n = 1;
    } else {
        if (*(s32 *)((u8 *)obj + 0x60) != 0) {
            n = w->clipSize;
        } else {
            r = rand() % (w->clipSize >> 1) + 1;
            n = (w->clipSize >> 2) + r;
        }
        if (!(w->clipSize & 1) && (n & 1)) {
            n++;
        }
    }
    return n;
}

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

void func_8004CCD8(u8 *a, u8 *b) {
    func_8004CD14(a, *(SVECTOR **)(a + 4));
    func_8004CDA0(D_800D8EA4, b);
}

void func_8004CD14(u8 *arg0, SVECTOR *dst) {
    s32 i;
    s32 j;
    s32 n;
    s32 count;
    SVECTOR *src;

    n = 0;
    count = arg0[0];
    *(SVECTOR **)(arg0 + 4) = dst;
    for (i = 0; i < count; i++) {
        dst += n;
        n = (*(s32 **)(arg0 + 8))[i * 2 + 1];
        src = (*(SVECTOR ***)(arg0 + 8))[i * 2];
        for (j = 0; j < n; j++) {
            dst[j] = src[j];
        }
    }
}

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

#ifdef NON_MATCHING
void func_8004D048(void) {
    char sp10[16];
    s32 i;
    s32 off;
    Rec48 *e;
    u8 *s0;
    s32 fd;
    s32 t;

    func_8004D214();
    D_8009D368 = 2;
    D_8009D36C = 0x80167210;
    M2C_FIELD(g_Args, s32 *, 0x22C) = 0x80167218;
    func_8006B198();
    i = 0;
    func_800689F0();
    if (D_8009D368 > 0) {
        off = 0;
        e = D_800D8FF8;
        do {
            s0 = D_800D8E8C[0] + off;
            t = D_8009E908[M2C_FIELD(D_800D8FF8[i], s16 *, 2)];
            D_800D8C90 = t;
            D_800D8EA4 = D_8009E9A8[M2C_FIELD(D_800D8FF8[i], s32 *, 4)];
            func_8004CBF4((s32 *)t, s0, i);
            i++;
            func_8004CCD8(s0, D_800D8E74[0] + off);
            off += 12;
        } while (i < D_8009D368);
    }
    func_8004D5A8((s32 *)0x80165CF8);
    if (g_Args->cdArchive == 0) {
        sprintf(sp10, D_800A87F8, g_Args->level);
        fd = PCcreat(sp10, 0);
        if (fd != -1) {
            sprintf(sp10, D_800A8808, M2C_FIELD(g_Args, s32 *, 0x22C) - D_8009D36C);
            func_80084E0C(fd, sp10, strlen(sp10));
            func_80083F14(fd);
        }
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004D048);
#endif

void func_8004D214(void) {
    s32 i;
    u32 j;
    s32 k;
    u8 *p;
    for (i = 0; i < 2; i++) {
        p = D_800D8FF8[i];
        j = 0;
        do {
            *p = 0;
            j++;
            p++;
        } while (j < 0x48);
        ((E2 *)D_800D8E8C)[i].f = 1;
        ((E2 *)D_800D8E8C)[i].x = 0;
        ((E2 *)D_800D8E8C)[i].c = D_800D8FE8[i];
        ((E1 *)D_800D8E74)[i].f = 1;
        ((E1 *)D_800D8E74)[i].b = D_800D8EE8[i];
        ((E1 *)D_800D8E74)[i].a = D_800D8EA8[i];
    }
    for (i = 0x29; i >= 0; i--) {
        D_800D8BE8[i] = 0;
    }
}

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

void func_8004D844(void *arg0, s32 i, s32 j) {
    u8 *b = D_800D8FF8[0];
    s32 o = i * 0x48;
    s32 t = D_8009CDA8 - M2C_FIELD(b + (j * 4 + o), s32 *, 0x10);
    t++;
    M2C_FIELD(arg0, s32 *, 0x14) = b[o + 0x36] - t;
}

#ifdef NON_MATCHING
void func_8004D88C(void) {
    u8 *e = (u8 *)D_800D8FF8;
    s32 i = 0;
    register s32 n REG("a2") = D_8009D368;
    register s32 found REG("a1") = i;
    register s32 want REG("a3");
    u32 j;
    s32 *cnt;
    s32 t;
    if (n > 0) {
        want = D_8009CA78;
loop:
        if (*e == 0 || (t = *(s32 *)(e + 0x30)) == 0 || t != want) {
            i++;
            e += 0x48;
            if (i < n) goto loop;
        } else {
            goto hit;
        }
    }
test:
    if (found == 0) {
        func_8006B1DC((M2C_UNK *)D_800A8884);
        return;
    }
    goto main;
hit:
    found = 1;
    goto test;
main:
    cnt = (s32 *)((u8 *)D_800D8C7C + *(s32 *)(e + 8) * 0x18);
    j = 0;
    if (*cnt != 0) {
        do {
            j++;
            *(s32 *)(e + 0x10) -= 1;
            e += 4;
        } while (j < *(u32 *)cnt);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004D88C);
#endif

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

void func_8004DA60(void) {
    Camera *c;
    s32 y;
    if (g_CutScene != 0) {
        func_8004EF44();
        return;
    }
    c = &g_Camera;
    c->targetX = g_Args->playerX;
    y = -D_8009CFA8 - D_800D91FC;
    c->targetZ = g_Args->playerZ;
    c->targetY = y;
    D_8009CD50 = 0;
    D_8009CD54 = 0;
    if (M2C_FIELD(M2C_FIELD(g_Player, void **, 0x94), s32 *, 0x24) & 0x400) {
        if (M2C_FIELD(g_Player, s16 *, 0x98) == 0 && M2C_FIELD(g_Player, s16 *, 0x9C) == 0) {
            c->targetY = y + 0x258;
        } else {
            c->targetY = y + 0x190;
        }
    }
    g_Camera.rotX = M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), u16 *, 0x60) - 0x17;
    g_Camera.rotY = gPsi;
    g_Camera.rotZ = M2C_FIELD(M2C_FIELD(g_Player, void **, 0x90), u16 *, 0x64);

    func_8004E37C();
    func_8004E5D8();
    if (D_8009D3A8 != 0) {
        func_8005099C();
        D_8009D3A8 = 0;
    } else {
        func_8005038C((s32)&D_800B0AB8, (s32)&D_800B0AC0, (s32)&D_800D91E8, (s32)&D_800D91F0);
    }
    if (D_8009CFCC < 0x3E9) {
        D_8009CFCC = D_8009CFCC + 1;
    } else {
        D_8009CFCC = 0x11;
    }

}

void func_8004DBCC() {
    s16 o;

    if (D_800D9208 > 0) {
        o = (u32)D_8009CDA8 % 7;
        g_Camera.posX += (s16)D_8009D374[o] * *(u8 *)&D_800D920C;
        g_Camera.posZ += (s16)D_8009D394[o] * *(u8 *)&D_800D920C;
        if (*(u8 *)&D_800D9218 == 0) {
            g_Camera.posY += (s16)D_8009D384[o] * *(u8 *)&D_800D920C;
        }
        D_800D9208 = (u16)D_800D9208 - 1;
        return;
    }
    if (*(u8 *)&D_800D91F8 != 0) {
        o = (u32)D_8009CDA8 % 7;
        g_Camera.posX += (s32)(D_8009D374[o] << 16) >> 19;
        g_Camera.posZ += (s32)(D_8009D394[o] << 16) >> 19;
        if (*(u8 *)&D_800D9218 == 0) {
            g_Camera.posY += (s32)(D_8009D384[o] << 16) >> 19;
        }
    } else if (D_800D9208 >= -3) {
        D_800D9208 = (u16)D_800D9208 - 1;
    }
}

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

s32 func_8004DFF8(s32 *pos) {
    SVECTOR v;
    u8 *m = *(u8 **)((u8 *)g_Player + 0x90);
    s32 *mp = (s32 *)(m + 0x28);
    s32 y = mp[1];
    v.vx = pos[0] - mp[0];
    v.vy = 0;
    v.vz = pos[2] - mp[2];
    y -= g_Camera.posY;
    return func_800743E8(D_800E2F54, pos, y, 500, &v, 0, 1);
}

void func_8004E07C(void) {
    s32 pos1[4];
    s32 pos2[4];
    s32 *zp;
    s32 zoom;
    s32 m;
    s32 sc;
    s32 s;
    s32 c;
    s32 *p;

    zp = &D_800B0C74;
    zoom = *zp;
    if (zoom >= 0) {
        m = (zoom > D_800B0B14) ? D_800B0B14 : zoom;
    } else {
        m = 0;
    }
    *zp = m;
    g_Camera.rotX = D_800B0C64;
    g_Camera.rotY = D_800B0C5C;
    if (D_8009CD5C >= 9) {
        *(s16 *)((u8 *)&g_Camera + 0x80) = D_800B0C64;
        *(s16 *)((u8 *)&g_Camera + 0x82) = D_800B0C5C;
    }
    ((void (*)(s32))func_8004E304)(*(s16 *)&D_800B0C5C);
    D_8009D3A4 = 0;
    if (*(s16 *)&D_800B0AEC != *(s32 *)&D_800B0C5C) {
        sc = D_800A0370[D_800B0AEC & 0xFFF];
        c = sc >> 16;
        s = (s16)sc;
        pos1[0] = g_Camera.targetX;
        pos1[1] = g_Args->playerY;
        pos1[2] = g_Camera.targetZ;
        pos1[0] += (s * 125) >> 10;
        pos1[2] += (c * 125) >> 10;
        if (func_8004DFF8(pos1) == 0 && func_80040AA8(pos1) == 0) {
            pos2[0] = g_Camera.targetX;
            pos2[1] = g_Args->playerY;
            pos2[2] = g_Camera.targetZ;
            pos2[0] += (s * 25) >> 7;
            pos2[2] += (c * 25) >> 7;
            if (func_8004DFF8(pos2) != 0 || func_80040AA8(pos2) != 0) {
                p = pos1;
            } else {
                p = pos2;
            }
            g_Camera.targetX = p[0];
            D_8009D3A4 = 1;
            g_Camera.targetZ = p[2];
        }
    }
    func_8004E328();
    if (D_8009D3A8 != 0) {
        D_8009D3A8 = 0;
        func_8005099C();
    } else {
        func_800503D8(D_800B0AB8, D_800B0AC0, D_8009CD5E, D_8009CD60);
    }
    gte_SetGeomScreen(g_Projection + (D_800B0C74 >> D_8009CD58));
    D_8009CFCC = 0;
}

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

void func_8004E37C(void) {
    s32 step;
    s32 t;

    step = 300;
    if (*(s32 *)(*(u8 **)((u8 *)g_Player + 0x94) + 0x24) & 0x400) {
        step = 150;
    }
    if (*(s16 *)((u8 *)g_Player + 0x98) != 0 || *(s16 *)((u8 *)g_Player + 0x9C) != 0) {
        if (D_8009E41C < 0 && D_8009CAA4 == 0 && D_8009DB3C != 0) {
            D_8009E41C = func_80026F30(0x50, 0);
        }
        if (D_800DDC44 != 0 || D_800DDCC0 != 0 || D_800DDD8C != 0 || D_800DDD90 != 0) {
            if (D_8009DB3C != 0) {
                if (g_PadPressed != 0) {
                    func_80026F30((rand() & 1) | 0x4E, 0);
                }
            } else {
                t = step + D_8009D3FC;
                if (t > 0x1000 || (t >= 0x800 && D_8009D3FC < 0x800)) {
                    if (*(s32 *)(*(u8 **)((u8 *)g_Player + 0x94) + 0x24) & 0x400) {
                        g_Args->pad12E[0xA] = 0x20;
                    } else {
                        g_Args->pad12E[0xA] = 0x7F;
                    }
                    func_8005CA20(g_Player);
                }
                D_8009D3FC = (D_8009D3FC + step) & 0xFFF;
            }
        } else if ((D_800DDCC4 != 0 || D_800DDDB8 != 0) && g_PadPressed != 0) {
            func_80026F30((rand() & 1) + 0x53, 0);
        }
    } else {
        if (D_8009E41C >= 0) {
            ((void (*)(s32))func_80026C14)(D_8009E41C);
            D_8009E41C = -1;
        }
        D_8009D3FC = 0xC00;
    }
}

#ifdef NON_MATCHING
void func_8004E5D8(void) {
    GsMat32 m;
    s32 p;
    s32 sc;
    s32 y;
    s32 x;

    if ((D_800DDC44 != 0 || (*(s32 *)(*(u8 **)((u8 *)g_Player + 0x94) + 0x24) & 0x400)) && (*(s16 *)((u8 *)g_Player + 0x98) != 0 || *(s16 *)((u8 *)g_Player + 0x9C) != 0)) {
        p = (((u16 *)&D_8009D3FC)[1] + D_8009D3F8) & 0xFFF;
        sc = D_800A0370[p];
        ((u16 *)&D_8009D3FC)[1] = p;
        y = (D_8009D3F4 * (s16)sc) >> 12;
        x = (D_8009D3F0 * (sc >> 16)) >> 12;
        if (y > 0) {
            y = -y;
        }
        D_8009D3BC.vx = x;
        D_8009D3BC.vy = y;
        D_8009D3BC.vz = 0;
        m = D_800FC6A0;
        RotMatrixY(g_Camera.rotY, &m);
        ApplyMatrixLV(&m, &D_8009D3BC, D_1F8003E8);
        g_Camera.targetX += D_1F8003E8[0];
        g_Camera.targetY += D_1F8003E8[1];
        g_Camera.targetZ += D_1F8003E8[2];
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004E5D8);
#endif

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

void func_8004E89C(void) {
    (&D_800B0AB8)[0] = 0x19;
    (&D_800B0AC0)[0] = 0x2D;
    (&D_800B0AB8)[1] = 0x19;
    (&D_800B0AC0)[1] = 0x27;
    (&D_800B0AB8)[2] = 0x19;
    (&D_800B0AC0)[2] = 0x2D;
    D_800B0B68.vx = 0x19;
    D_800B0B70.vx = 0x40;
    D_800B0B68.vy = 0x19;
    D_800B0B70.vy = 0x2D;
    D_800B0B68.vz = 0x19;
    D_800B0B70.vz = 0x40;
    D_800D91E8 = D_800B0B68;
    D_800D91F0 = D_800B0B70;
    D_800D91E8.vy = 0x27;
    D_800D91F0.vy = 0x3A;
}

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

void func_8004E9AC(void) {
    if (D_8009D3B0 == 0) {
        func_8004E96C();
    }
    if (g_Camera.targetY < g_Args->playerY - 0x12C) {
        g_Camera.targetY += 0x40;
    }
    if (D_8009D3B0 < 10) {
        g_Camera.rotX += ((u16 *)&D_800D9210)[0];
        g_Camera.rotY += ((u16 *)&D_800D9210)[1];
        g_Camera.rotZ += ((u16 *)&D_800D9210)[2];
    }
    func_8004F880();
    D_8009D3B0++;
}

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

void func_8004EAB8(u8 *obj) {
    u8 *m = *(u8 **)(obj + 0x90);
    *(SVECTOR *)D_800B0064 = *(SVECTOR *)(D_800B0064 - 0x70);
    *(SVECTOR *)(m + 0x60) = *(SVECTOR *)D_800B0064;
    M2C_FIELD(D_800B0064 - 0x80, s32 *, 0x2C) = 0;
    M2C_FIELD(D_800B0064 - 0x80, s32 *, 0x28) = 0;
    M2C_FIELD(D_800B0064 - 0x80, s32 *, 0x24) = 0;
}

void func_8004EB20(void) {
    VECTOR a;
    VECTOR b;
    u8 *obj = (u8 *)&D_800D90C0;
    u8 *m = *(u8 **)(obj + 0x90);
    u8 *tm = *(u8 **)(*(u8 **)(obj + 0x5C) + 0x90);
    VECTOR *cp = (VECTOR *)&g_Camera.posX;
    VECTOR *mp = (VECTOR *)(m + 0x28);
    SVECTOR *cr = (SVECTOR *)((u8 *)&g_Camera + 0x80);
    s16 *mr = (s16 *)(m + 0x60);
    VECTOR *tp = (VECTOR *)(tm + 0x28);

    mp->vx = a.vx = cp->vx;
    mp->vy = a.vy = cp->vy;
    mp->vz = a.vz = cp->vz;
    mr[0] = cr->vx;
    mr[1] = cr->vy;
    mr[2] = cr->vz;
    b.vx = tp->vx;
    b.vy = tp->vy;
    b.vz = tp->vz;
    *(s32 *)(obj + 0x110) = func_8005397C(&a, &b);
    b.vy += (*(s16 *)(*(u8 **)(obj + 0x5C) + 0x20) + *(s16 *)(*(u8 **)(obj + 0x5C) + 0x22)) >> 1;
    *(s16 *)(obj + 0xC4) = func_80053004(obj, b.vx, b.vz);
    *(s16 *)(obj + 0xC6) = func_80053080(obj, *(s32 *)(obj + 0x110), b.vy);
    g_Camera.rotX = (mr[0] + *(s16 *)(obj + 0xC6)) & 0xFFF;
    g_Camera.rotY = (mr[1] + *(s16 *)(obj + 0xC4)) & 0xFFF;
    g_Camera.rotZ = mr[2];
    func_8004EAB8(obj);
}

void func_8004EC70(void) {
    u8 *obj;
    s32 *pos;
    s32 *tgt;
    s16 *rot2;
    s32 *spd;
    s16 *rot;
    s32 dx;
    s32 dy;
    s32 dz;
    u32 ax;
    u32 ay;
    u32 az;

    obj = (u8 *)&D_800D90C0;
    pos = (s32 *)&g_CamPosX;
    tgt = (s32 *)(&g_CamPosX - 0x44);
    rot2 = (s16 *)(&g_CamPosX + 0x38);
    spd = (s32 *)(&g_CamPosX - 0x48);
    rot = (s16 *)(&g_CamPosX - 0x38);
    if (spd[6] != 0 || spd[7] != 0 || spd[8] != 0) {
        dx = tgt[0] - pos[0];
        if (dx < 0) dx = pos[0] - tgt[0];
        dy = tgt[1] - pos[1];
        if (dy < 0) dy = pos[1] - tgt[1];
        dz = tgt[2] - pos[2];
        if (dz < 0) dz = pos[2] - tgt[2];
        if ((u32)dx < abs(M2C_FIELD(&g_Camera, s32 *, 0x18)) || (u32)dy < abs(M2C_FIELD(&g_Camera, s32 *, 0x1C)) || (u32)dz < abs(M2C_FIELD(&g_Camera, s32 *, 0x20))) {
            func_8004EA68(obj);
        } else {
            pos[0] += M2C_FIELD(&g_Camera, s32 *, 0x18);
            pos[1] += M2C_FIELD(&g_Camera, s32 *, 0x1C);
            pos[2] += M2C_FIELD(&g_Camera, s32 *, 0x20);
        }
    }
    if (D_800D911C != 0) {
        func_8004EB20();
        return;
    }
    if (M2C_FIELD(&g_Camera, s32 *, 0x24) != 0 || M2C_FIELD(&g_Camera, s32 *, 0x28) != 0 || M2C_FIELD(&g_Camera, s32 *, 0x2C) != 0) {
        ax = (rot[0] - rot2[0]) & 0xFFF;
        if (ax > 0x800) ax = 0x1000 - ax;
        ay = (rot[1] - rot2[1]) & 0xFFF;
        if (ay > 0x800) ay = 0x1000 - ay;
        az = (rot[2] - rot2[2]) & 0xFFF;
        if (az > 0x800) az = 0x1000 - az;
        if (ax < abs(M2C_FIELD(&g_Camera, s32 *, 0x24)) || ay < abs(M2C_FIELD(&g_Camera, s32 *, 0x28)) || az < abs(M2C_FIELD(&g_Camera, s32 *, 0x2C))) {
            func_8004EAB8(obj);
        } else {
            ((u16 *)rot2)[0] += M2C_FIELD(&g_Camera, u16 *, 0x24);
            ((u16 *)rot2)[1] += M2C_FIELD(&g_Camera, u16 *, 0x28);
            ((u16 *)rot2)[2] += M2C_FIELD(&g_Camera, u16 *, 0x2C);
        }
    }
}

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

void func_8004F148(s16 arg0, s16 arg1) {
    s32 a;
    s32 b;
    s32 c;
    s32 x;
    s32 y;
    s32 z;
    a = M2C_FIELD(&g_Camera, s32 *, 0x18);
    a += ((g_Camera.targetX - g_Camera.posX) * arg0) - ((a >> 6) * arg1);
    x = (a * 10) / g_FrameRate;
    b = M2C_FIELD(&g_Camera, s32 *, 0x1C);
    b += ((g_Camera.targetY - g_Camera.posY) * arg0) - ((b >> 6) * arg1);
    y = (b * 10) / g_FrameRate;
    c = M2C_FIELD(&g_Camera, s32 *, 0x20);
    c += ((g_Camera.targetZ - g_Camera.posZ) * arg0) - ((c >> 6) * arg1);
    z = (c * 10) / g_FrameRate;
    M2C_FIELD(&g_Camera, s32 *, 0x18) = a;
    M2C_FIELD(&g_Camera, s32 *, 0x1C) = b;
    M2C_FIELD(&g_Camera, s32 *, 0x20) = c;
    D_1F8003E8[0] = x;
    D_1F8003E8[1] = y;
    D_1F8003E8[2] = z;
    if (abs(x) < 0xC0) {
        D_1F8003E8[0] = 0;
    }
    if (abs(y) < 0xC0) {
        D_1F8003E8[1] = 0;
    }
    if (abs(z) < 0xC0) {
        D_1F8003E8[2] = 0;
    }
    g_Camera.posX += D_1F8003E8[0] >> 6;
    g_Camera.posY += D_1F8003E8[1] >> 6;
    g_Camera.posZ += D_1F8003E8[2] >> 6;
}

s32 func_8004F348(void) {
    if (abs(g_Camera.targetX - g_Camera.posX) > 200 || abs(g_Camera.targetY - g_Camera.posY) > 200 || abs(g_Camera.targetZ - g_Camera.posZ) > 200) {
        return 1;
    }
    return 0;
}

void func_8004F3D8(void) {
    D_8009D3A8 = 1;
}

#ifdef NON_MATCHING
s32 func_8004F3E8(void) {
    s16 d;
    s32 v, a, b, c, qa, qb, qc;
    a = V(0x18) + ((g_Camera.targetX - g_Camera.posX) * K0 - (V(0x18) >> 6) * K1);
    qa = a * 0xA / g_FrameRate;
    b = V(0x1C) + ((g_Camera.targetY - g_Camera.posY) * K0 - (V(0x1C) >> 6) * K1);
    qb = b * 0xA / g_FrameRate;
    c = V(0x20) + ((g_Camera.targetZ - g_Camera.posZ) * K0 - (V(0x20) >> 6) * K1);
    qc = c * 0xA / g_FrameRate;
    V(0x18) = a;
    V(0x1C) = b;
    V(0x20) = c;
    D_1F8003E8[0] = qa;
    D_1F8003E8[1] = qb;
    D_1F8003E8[2] = qc;
    if (abs(qa) < 0xC0) {
        D_1F8003E8[0] = 0;
    }
    if (abs(qb) < 0xC0) {
        D_1F8003E8[1] = 0;
    }
    if (abs(qc) < 0xC0) {
        D_1F8003E8[2] = 0;
    }
    d = ((u16)g_Camera.rotX - C(0x80)) & 0xFFF;
    g_Camera.posX += D_1F8003E8[0] >> 6;
    g_Camera.posY += D_1F8003E8[1] >> 6;
    g_Camera.posZ += D_1F8003E8[2] >> 6;
    if (d >= 0x801) {
        d = d | 0xF000;
    }
    v = V(0x24) + (d * K0 - (V(0x24) >> 6) * K1);
    V(0x24) = v;
    if (v != 0) {
        s32 st = (v * 0xA / g_FrameRate) >> 6;
        if (abs(st) < abs(d)) {
            C(0x80) = C(0x80) + st;
        } else {
            C(0x80) = (u16)g_Camera.rotX;
        }
    }
    d = ((u16)g_Camera.rotY - C(0x82)) & 0xFFF;
    if (d >= 0x801) {
        d = d | 0xF000;
    }
    v = V(0x28) + (d * K0 - (V(0x28) >> 6) * K1);
    V(0x28) = v;
    if (v != 0) {
        s32 st = (v * 0xA / g_FrameRate) >> 6;
        if (abs(st) < abs(d)) {
            C(0x82) = C(0x82) + st;
        } else {
            C(0x82) = (u16)g_Camera.rotY;
        }
    }
    d = ((u16)g_Camera.rotZ - C(0x84)) & 0xFFF;
    if (d >= 0x801) {
        d = d | 0xF000;
    }
    v = V(0x2c) + (d * K0 - (V(0x2c) >> 6) * K1);
    V(0x2c) = v;
    if (v != 0) {
        s32 st = (v * 0xA / g_FrameRate) >> 6;
        if (abs(st) < abs(d)) {
            C(0x84) = C(0x84) + st;
        } else {
            C(0x84) = (u16)g_Camera.rotZ;
        }
    }
    return (s16)func_8004F348() != 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004F3E8);
#endif

#ifdef NON_MATCHING
s32 func_8004F880(void) {
    s32 a, b, c, v;
    s16 d;
    u16 t1, t0;
    a = CW(0x18);
    a = a + ((CW(4) - CW(0x48)) * (&D_800B0AB8)[0] - (a >> 6) * (&D_800B0AC0)[0]);
    b = CW(0x1C);
    b = b + ((CW(8) - CW(0x4C)) * (&D_800B0AB8)[1] - (b >> 6) * (&D_800B0AC0)[1]);
    c = CW(0x20);
    c = c + ((CW(0xC) - CW(0x50)) * (&D_800B0AB8)[2] - (c >> 6) * (&D_800B0AC0)[2]);
    CW(0x18) = a;
    CW(0x1C) = b;
    CW(0x20) = c;
    SP0 = a * 10 / g_FrameRate;
    SP1 = b * 10 / g_FrameRate;
    SP2 = c * 10 / g_FrameRate;
    if (abs(SP0) < 0xC0) SP0 = 0;
    if (abs(SP1) < 0xC0) SP1 = 0;
    if (abs(SP2) < 0xC0) SP2 = 0;
    SP0 >>= 6;
    SP1 >>= 6;
    SP2 >>= 6;
    CW(0x4C) += SP1;
    CW(0x48) += SP0;
    CW(0x50) += SP2;
    t1 = CH(0x10);
    t0 = CH(0x80);
    d = (t1 - t0) & 0xFFF;
    if (d >= 0x801) d = d | 0xF000;
    v = CW(0x24);
    v = v + (d * D_800B0B68.vx - (v >> 6) * D_800B0B70.vx);
    CW(0x24) = v;
    if (v != 0) {
        v = (v * 10 / g_FrameRate) >> 6;
        if (abs(v) < abs(d)) CH(0x80) = t0 + v;
        else CH(0x80) = t1;
    }
    d = (g_Camera.rotY - CH(0x82)) & 0xFFF;
    if (d >= 0x801) d = d | 0xF000;
    v = CW(0x28);
    v = v + (d * D_800B0B68.vy - (v >> 6) * D_800B0B70.vy);
    CW(0x28) = v;
    if (v != 0) {
        v = (v * 10 / g_FrameRate) >> 6;
        if (abs(v) < abs(d)) CH(0x82) = CH(0x82) + v;
        else CH(0x82) = g_Camera.rotY;
    }
    d = (g_Camera.rotZ - CH(0x84)) & 0xFFF;
    if (d >= 0x801) d = d | 0xF000;
    v = CW(0x2C);
    v = v + (d - (v >> 6)) * D_800B0B68.vz;
    CW(0x2C) = v;
    if (v != 0) {
        v = (v * 10 / g_FrameRate) >> 6;
        if (abs(v) < abs(d)) CH(0x84) = CH(0x84) + v;
        else CH(0x84) = g_Camera.rotZ;
    }
    return (s16)func_8004F348() != 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004F880);
#endif

#ifdef NON_MATCHING
void func_8004FD3C(s16 *A0, s16 *A1) {
    s16 d;
    s32 v;
    d = ((u16)g_Camera.rotX - C(0x80)) & 0xFFF;
    if (d >= 0x801) {
        d = d | 0xF000;
    }
    v = V(0x24) + (d * A0[0] - (V(0x24) >> 6) * A1[0]);
    V(0x24) = v;
    if (v != 0) {
        s32 st = (v * 0xA / g_FrameRate) >> 6;
        if (abs(st) < abs(d)) {
            C(0x80) = C(0x80) + st;
        } else {
            C(0x80) = (u16)g_Camera.rotX;
        }
    }
    d = ((u16)g_Camera.rotY - C(0x82)) & 0xFFF;
    if (d >= 0x801) {
        d = d | 0xF000;
    }
    v = V(0x28) + (d * A0[1] - (V(0x28) >> 6) * A1[1]);
    V(0x28) = v;
    if (v != 0) {
        s32 st = (v * 0xA / g_FrameRate) >> 6;
        if (abs(st) < abs(d)) {
            C(0x82) = C(0x82) + st;
        } else {
            C(0x82) = (u16)g_Camera.rotY;
        }
    }
    d = ((u16)g_Camera.rotZ - C(0x84)) & 0xFFF;
    if (d >= 0x801) {
        d = d | 0xF000;
    }
    v = V(0x2c) + (d * A0[2] - (V(0x2c) >> 6) * A1[2]);
    V(0x2c) = v;
    if (v != 0) {
        s32 st = (v * 0xA / g_FrameRate) >> 6;
        if (abs(st) < abs(d)) {
            C(0x84) = C(0x84) + st;
        } else {
            C(0x84) = (u16)g_Camera.rotZ;
        }
    }
    v = V(0x24);
    if (abs(v) >= 0x3A) {
        if (g_FrameRate < 0xA) {
            v = v * 0xA / g_FrameRate;
        }
        C(0x80) = C(0x80) + (v >> 6);
    } else {
        C(0x80) = (u16)g_Camera.rotX;
    }
    v = V(0x28);
    if (abs(v) >= 0x3A) {
        if (g_FrameRate < 0xA) {
            v = v * 0xA / g_FrameRate;
        }
        C(0x82) = C(0x82) + (v >> 6);
    } else {
        C(0x82) = (u16)g_Camera.rotY;
    }
    v = V(0x2c);
    if (abs(v) >= 0x3A) {
        if (g_FrameRate < 0xA) {
            v = v * 0xA / g_FrameRate;
        }
        C(0x84) = C(0x84) + (v >> 6);
    } else {
        C(0x84) = (u16)g_Camera.rotZ;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8004FD3C);
#endif

void func_80050180(s16 *arg0, s16 *arg1) {
    s32 a;
    s32 b;
    s32 c;
    s32 x;
    s32 y;
    s32 z;
    s32 *q = &g_CamTargetX;
    s32 *v;
    v = q - 1;
    a = v[6];
    a += ((q[0] - q[0x11]) * arg0[0]) - ((a >> 6) * arg1[0]);
    x = (a * 10) / g_FrameRate;
    v[6] = a;
    b = v[7];
    b += ((q[1] - q[0x12]) * arg0[1]) - ((b >> 6) * arg1[1]);
    y = (b * 10) / g_FrameRate;
    v[7] = b;
    c = v[8];
    c += ((q[2] - q[0x13]) * arg0[2]) - ((c >> 6) * arg1[2]);
    z = (c * 10) / g_FrameRate;
    v[8] = c;
    D_1F8003E8[0] = x;
    D_1F8003E8[1] = y;
    D_1F8003E8[2] = z;
    if (abs(x) < 0xC0) {
        D_1F8003E8[0] = 0;
    }
    if (abs(y) < 0xC0) {
        D_1F8003E8[1] = 0;
    }
    if (abs(z) < 0xC0) {
        D_1F8003E8[2] = 0;
    }
    D_1F8003E8[0] >>= 6;
    D_1F8003E8[1] >>= 6;
    D_1F8003E8[2] >>= 6;
    q[0x11] += D_1F8003E8[0];
    q[0x12] += D_1F8003E8[1];
    q[0x13] += D_1F8003E8[2];
}

s32 func_8005038C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80050180(arg0, arg1);
    func_8004FD3C(arg2, arg3);
    return (s16)func_8004F348() != 0;
}

#ifdef NON_MATCHING
s32 func_800503D8(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 dx, dy, dz;
    s32 v, step;
    
    func_8004F148(arg0, arg1);
    dx = (g_Camera.rotX - M2C_FIELD(&g_Camera, u16 *, 0x80)) & 0xFFF;
    if (dx >= 0x801) dx |= 0xF000;
    v = M2C_FIELD(&g_Camera, s32 *, 0x24) + (dx * arg2 - (M2C_FIELD(&g_Camera, s32 *, 0x24) >> 6) * arg3);
    M2C_FIELD(&g_Camera, s32 *, 0x24) = v;
    if (v != 0) {
        step = (v * 10 / g_FrameRate) >> 6;
        if (abs(step) < abs(dx)) M2C_FIELD(&g_Camera, u16 *, 0x80) = M2C_FIELD(&g_Camera, u16 *, 0x80) + step; else M2C_FIELD(&g_Camera, u16 *, 0x80) = g_Camera.rotX;
    }
    dy = (g_Camera.rotY - M2C_FIELD(&g_Camera, u16 *, 0x82)) & 0xFFF;
    if (dy >= 0x801) dy |= 0xF000;
    v = M2C_FIELD(&g_Camera, s32 *, 0x28) + (dy * arg2 - (M2C_FIELD(&g_Camera, s32 *, 0x28) >> 6) * arg3);
    M2C_FIELD(&g_Camera, s32 *, 0x28) = v;
    if (v != 0) {
        step = (v * 10 / g_FrameRate) >> 6;
        if (abs(step) < abs(dy)) M2C_FIELD(&g_Camera, u16 *, 0x82) = M2C_FIELD(&g_Camera, u16 *, 0x82) + step; else M2C_FIELD(&g_Camera, u16 *, 0x82) = g_Camera.rotY;
    }
    dz = (g_Camera.rotZ - M2C_FIELD(&g_Camera, u16 *, 0x84)) & 0xFFF;
    if (dz >= 0x801) dz |= 0xF000;
    v = M2C_FIELD(&g_Camera, s32 *, 0x2C) + (dz * arg2 - (M2C_FIELD(&g_Camera, s32 *, 0x2C) >> 6) * arg3);
    M2C_FIELD(&g_Camera, s32 *, 0x2C) = v;
    if (v != 0) {
        step = (v * 10 / g_FrameRate) >> 6;
        if (abs(step) < abs(dz)) M2C_FIELD(&g_Camera, u16 *, 0x84) = M2C_FIELD(&g_Camera, u16 *, 0x84) + step; else M2C_FIELD(&g_Camera, u16 *, 0x84) = g_Camera.rotZ;
    }
    v = M2C_FIELD(&g_Camera, s32 *, 0x24);
    if (abs(v) >= 0x3A) {
        if (g_FrameRate < 10) v = v * 10 / g_FrameRate;
        M2C_FIELD(&g_Camera, u16 *, 0x80) = M2C_FIELD(&g_Camera, u16 *, 0x80) + (v >> 6);
    } else M2C_FIELD(&g_Camera, u16 *, 0x80) = g_Camera.rotX;
    v = M2C_FIELD(&g_Camera, s32 *, 0x28);
    if (abs(v) >= 0x3A) {
        if (g_FrameRate < 10) v = v * 10 / g_FrameRate;
        M2C_FIELD(&g_Camera, u16 *, 0x82) = M2C_FIELD(&g_Camera, u16 *, 0x82) + (v >> 6);
    } else M2C_FIELD(&g_Camera, u16 *, 0x82) = g_Camera.rotY;
    v = M2C_FIELD(&g_Camera, s32 *, 0x2C);
    if (abs(v) >= 0x3A) {
        if (g_FrameRate < 10) v = v * 10 / g_FrameRate;
        M2C_FIELD(&g_Camera, u16 *, 0x84) = M2C_FIELD(&g_Camera, u16 *, 0x84) + (v >> 6);
    } else M2C_FIELD(&g_Camera, u16 *, 0x84) = g_Camera.rotZ;
    if (abs(g_Camera.targetX - g_Camera.posX) >= 0xC9) return 1;
    if (abs(g_Camera.targetY - g_Camera.posY) >= 0xC9) return 1;
    if (abs(g_Camera.targetZ - g_Camera.posZ) >= 0xC9) return 1;
    if (abs(dx) >= 0x33) return 1;
    if (abs(dy) >= 0x33) return 1;
    if (abs(dz) >= 0x33) return 1;
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800503D8);
#endif

void func_80050960(void) {
    M2C_FIELD(&g_Camera, s32 *, 0x20) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x1C) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x18) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x2C) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x28) = 0;
    M2C_FIELD(&g_Camera, s32 *, 0x24) = 0;
    func_8005099C();
}

void func_8005099C(void) {
    Camera *cam = &g_Camera;
    cam->posX = cam->targetX;
    cam->posY = cam->targetY;
    cam->posZ = cam->targetZ;
    *(SVECTOR *)((u8 *)cam + 0x80) = *(SVECTOR *)&cam->rotX;
}

void func_800509E4(void *x) {
    volatile s32 a[3];
    volatile s16 b[3];
    a[0] = 0; a[1] = -0x5DC; a[2] = -0x32;
    b[0] = 0; b[1] = -0x5DC; b[2] = -0x32;
}

s32 func_80050A10(VECTOR *arg0, VECTOR *arg1, u8 *arg2, void *arg3) {
    s16 a[4];
    s16 b[4];

    if (func_80050A70((s32 *)arg0, (s32 *)arg1, arg2, (s32)arg3) == 0) {
        return func_800746D0(D_800E2F2C, (s32 *)arg1, (s32 *)arg0, a, b, 0) == 0;
    }
    return 0;
}

#ifdef NON_MATCHING
u8 *func_80050A70(s32 *arg0, s32 *arg1, u8 *arg2, s32 arg3) {
    VECTOR v[5];
    s32 dx, dz, len, ang, t, cs, sn;
    s32 pass;
    u8 *obj, *lo, *hi, *base, *P;
    s32 rx, rz, cx, cz, py, R;
    s32 h0, h1, h2, h3, h4, h5, scale;
    s32 c2, s2;
    s32 *tbl;
    s32 i, e, a;
    s32 neg, pos, far, in;
    s32 ylo, yhi, dy;
    s32 w0, w1, x0, x1, d, zc, hy;
    VECTOR *A, *B;

    if (abs(arg0[0] - arg1[0]) + abs(arg0[1] - arg1[1]) + abs(arg0[2] - arg1[2]) < 0x1F4) {
        return 0;
    }
    dx = arg1[0] - arg0[0];
    dz = arg1[2] - arg0[2];
    ang = ratan2((s16)dx, dz) & 0xFFF;
    t = D_800A0370[ang];
    cs = t >> 16;
    sn = (s16)t;
    /* length of the ray projected on the rotated z axis */
    len = (dz * cs + dx * sn) >> 12;
    if (len < 0) {
        len = -len;
    }
    base = (u8 *)D_800AFFC8;
    lo = base;
    hi = base;
    for (pass = 0; pass < 2; pass++) {
        if (pass == 1) {
            lo = base + D_800B0DC0 * 0x124;
            hi = base + D_800B07A8 * 0x124;
        }
        for (obj = lo; (u32)obj <= (u32)hi; obj += 0x124) {
            if (obj[4] == 0 || !(*(s32 *)(obj + 0x48) & 0x400) || obj == arg2 || (s32)obj == arg3) {
                continue;
            }
            P = *(u8 **)(obj + 0x90);
            rx = *(s32 *)(P + 0x28) - arg0[0];
            rz = *(s32 *)(P + 0x30) - arg0[2];
            t = D_800A0370[ang];
            cs = t >> 16;
            sn = (s16)t;
            cx = (rx * cs - rz * sn) >> 12;
            cz = (rz * cs + rx * sn) >> 12;
            py = *(s32 *)(P + 0x2C);
            R = *(u16 *)(obj + 0x2A);
            if (!(cx >= -R && R >= cx && cz >= -R && R + len >= cz)) {
                continue;
            }
            h0 = *(s16 *)(obj + 0x1C);
            h1 = *(s16 *)(obj + 0x1E);
            h2 = *(s16 *)(obj + 0x20);
            h3 = *(s16 *)(obj + 0x22);
            h4 = *(s16 *)(obj + 0x24);
            h5 = *(s16 *)(obj + 0x26);
            tbl = &D_800A0370[(ang - *(s16 *)(P + 0x62)) & 0xFFF];
            s2 = *(s16 *)tbl;
            scale = *(s16 *)(obj + 0x6A);
            c2 = *((s16 *)tbl + 1);
            if (scale != 0) {
                h0 = (h0 * scale) >> 12;
                h1 = (h1 * scale) >> 12;
                h2 = (h2 * scale) >> 12;
                h3 = (h3 * scale) >> 12;
                h4 = (h4 * scale) >> 12;
                h5 = (h5 * scale) >> 12;
            }
            /* v[0] = box centre, v[1..4] = footprint corners, all relative to the ray (x across, z along) */
            v[0].vx = cx;
            v[0].vy = py;
            v[0].vz = cz;
            v[1].vx = cx + ((h0 * c2 - h4 * s2) >> 12);
            v[1].vz = cz + ((h4 * c2 + h0 * s2) >> 12);
            v[1].vy = h2;
            v[2].vx = cx + ((h1 * c2 - h4 * s2) >> 12);
            v[2].vz = cz + ((h4 * c2 + h1 * s2) >> 12);
            v[2].vy = h3;
            v[3].vx = cx + ((h1 * c2 - h5 * s2) >> 12);
            v[3].vz = cz + ((h5 * c2 + h1 * s2) >> 12);
            v[4].vx = cx + ((h0 * c2 - h5 * s2) >> 12);
            v[4].vz = cz + ((h5 * c2 + h0 * s2) >> 12);
            /* corners must lie on both sides of the ray (x = 0) */
            neg = 0;
            pos = 0;
            for (i = 1; i < 5; i++) {
                if (v[i].vx < 0) {
                    neg = 1;
                } else {
                    pos = 1;
                }
            }
            if (neg == 0 || pos == 0) {
                continue;
            }
            /* ... and not all of them before / beyond the ray ends (0 .. len) */
            neg = 0;
            far = 0;
            in = 0;
            for (i = 1; i < 5; i++) {
                if (v[i].vz < 0) {
                    neg = 1;
                } else if (len < v[i].vz) {
                    far = 1;
                } else {
                    in++;
                }
            }
            if (neg + in + far < 2) {
                continue;
            }
            dy = arg1[1] - arg0[1];
            ylo = py + v[1].vy;
            yhi = py + v[2].vy;
            for (e = 1; e < 5; e++) {
                A = &v[e];
                B = &v[(e & 3) + 1];
                /* edge A-B has to cross the ray line x = 0 */
                neg = 0;
                pos = 0;
                if (A->vx < 0) neg = 1; else pos = 1;
                if (B->vx < 0) neg = 1; else pos = 1;
                if (neg == 0 || pos == 0) {
                    continue;
                }
                neg = 0;
                far = 0;
                in = 0;
                if (A->vz < 0) neg = 1; else if (len < A->vz) far = 1; else in++;
                if (B->vz < 0) neg = 1; else if (len < B->vz) far = 1; else in++;
                if (neg + in + far < 2) {
                    continue;
                }
                if (in < 2) {
                    /* the edge sticks out of the ray's z range: check the crossing lies inside it */
                    if (neg != 0) {
                        if (A->vz < 0) {
                            w0 = abs(A->vz);
                            w1 = abs(B->vz);
                            x0 = abs(A->vx);
                            x1 = abs(B->vx);
                        } else {
                            w1 = abs(A->vz);
                            w0 = abs(B->vz);
                            x1 = abs(A->vx);
                            x0 = abs(B->vx);
                        }
                    } else {
                        if (len < A->vz) {
                            w0 = abs(A->vz - len);
                            w1 = abs(B->vz - len);
                            x0 = abs(A->vx);
                            x1 = abs(B->vx);
                        } else {
                            w1 = abs(A->vz - len);
                            w0 = abs(B->vz - len);
                            x1 = abs(A->vx);
                            x0 = abs(B->vx);
                        }
                    }
                    if (x0 * w1 < x1 * w0) {
                        continue;
                    }
                    if ((x0 + x1) * (w0 + len) < x0 * (w0 + w1)) {
                        continue;
                    }
                }
                /* vertical test: ray end points against the box's y range */
                neg = 0;
                far = 0;
                in = 0;
                if (arg0[1] < ylo) neg = 1; else if (yhi < arg0[1]) far = 1; else in = 1;
                if (arg1[1] < ylo) neg = 1; else if (yhi < arg1[1]) far = 1; else in++;
                if (neg + in + far < 2) {
                    continue;
                }
                if (in >= 2) {
                    return obj;
                }
                d = A->vx - B->vx;
                if (d == 0) {
                    continue;
                }
                zc = A->vx * (B->vz - A->vz) / d + A->vz;
                if (zc < 0 || len < zc) {
                    continue;
                }
                hy = zc * dy / len + arg0[1];
                if (hy < ylo || yhi < hy) {
                    continue;
                }
                return obj;
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80050A70);
#endif

s32 func_80051728(u8 *obj, u8 *tgt) {
    VECTOR a;
    VECTOR b;
    u8 *sub;
    s32 range;
    s32 near;
    s32 fov;
    s32 far;
    s32 flags;
    u32 dx;
    u32 dy;
    u32 dz;
    s32 d;
    VECTOR *mp;
    VECTOR *pa;
    u32 v;

    sub = *(u8 **)(obj + 0x118);
    range = *(s32 *)(sub + 0x4C);
    near = *(s32 *)(sub + 0x48);
    fov = *(s32 *)(sub + 0x50);
    far = *(s32 *)(sub + 0x84);
    flags = 0;
    if (tgt == g_Player && ((*(s32 *)(*(u8 **)(tgt + 0x94) + 0x24) & 0x200) || ((g_Args->flags2 >> 10) & 1)) && !(*(s32 *)(sub + 0x10) & 4)) {
        range >>= 1;
        fov >>= 1;
    }
    near -= (*(u32 *)(obj + 0x64) >> 29) << 7;
    mp = (VECTOR *)(*(u8 **)(obj + 0x90) + 0x28);
    a.vx = mp->vx;
    a.vy = mp->vy;
    a.vz = mp->vz;
    if (tgt != NULL) {
        mp = (VECTOR *)(*(u8 **)(tgt + 0x90) + 0x28);
        b.vx = mp->vx;
        b.vy = mp->vy;
        b.vz = mp->vz;
    } else {
        b.vx = *(s32 *)(sub + 0x1D8);
        b.vy = *(s32 *)(sub + 0x1DC);
        b.vz = *(s32 *)(sub + 0x1E0);
    }
    pa = &a;
    dx = b.vx - pa->vx;
    if ((s32)dx < 0) {
        dx = pa->vx - b.vx;
    }
    dy = b.vy - pa->vy;
    if ((s32)dy < 0) {
        dy = pa->vy - b.vy;
    }
    dz = b.vz - pa->vz;
    if ((s32)dz < 0) {
        dz = pa->vz - b.vz;
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
    *(s32 *)(obj + 0x110) = dx + dy + dz;
    *(s16 *)(obj + 0xC4) = func_80053004(obj, b.vx, b.vz);
    *(s16 *)(obj + 0xC6) = func_80053080(obj, *(s32 *)(obj + 0x110), b.vy);
    if (*(s32 *)(obj + 0x110) < range && abs(*(s16 *)(obj + 0xC4)) < fov && abs(*(s16 *)(obj + 0xC6)) < fov) {
        func_80052B2C((s32 *)pa, obj);
        if (tgt != NULL) {
            func_80052B2C((s32 *)&b, tgt);
        }
        if (func_80050A10(&b, pa, obj, tgt)) {
            if (((*(u32 *)(obj + 0x64) >> 25) & 7) < 7) {
                *(u32 *)(obj + 0x64) = (*(u32 *)(obj + 0x64) & 0xF1FFFFFF) | ((((*(u32 *)(obj + 0x64) >> 25) & 7) + 1) & 7) << 25;
            }
            if (((*(u32 *)(obj + 0x64) >> 25) & 7) >= 3) {
                flags |= 0x40;
                *(u32 *)(obj + 0x64) = (*(u32 *)(obj + 0x64) & 0xF1FFFFFF) | 0x06000000;
            }
            if (abs(*(s16 *)(obj + 0xC4)) < 0x71) {
                flags |= 0x100;
            }
        } else {
            v = *(u32 *)(obj + 0x64);
            if (v & 0x0E000000) {
                *(u32 *)(obj + 0x64) = (v & 0xF1FFFFFF) | (((((v >> 25) & 7) - 1) & 7) << 25);
            }
        }
    }
    d = *(s32 *)(obj + 0x110);
    if (d < near) {
        flags |= 0x10;
    }
    if (d < 200) {
        flags |= 0x8000;
    }
    if (d < far) {
        flags |= 0x20000000;
    }
    return flags;
}

#ifdef NON_MATCHING
void func_80051AE0(u8 **arg0) {
    s16 pos[3];
    u8 hit;
    s16 yaw;
    s16 ang;
    s32 v;
    s32 st;
    s32 set;
    s32 rflags;
    u8 *obj;
    u8 *w;
    u8 *tgt;
    u8 *vp;
    Pickup *pk;

    obj = *arg0;
    w = M2C_FIELD(obj, u8 **, 0x118);
    yaw = 0;
    if (w == NULL) {
        return;
    }
    if ((g_PadPressed & g_Args->buttonMap[3]) && (func_8004A370() == 0)) {
        M2C_FIELD(w, s32 *, 0) |= 0x01000000;
    }
    if (D_800D91E7 & 1) {
        M2C_FIELD(w, s32 *, 0x10) |= 0x800;
    } else {
        M2C_FIELD(w, s32 *, 0x10) &= ~0x800;
    }
    if ((((u32) D_800D91E4 >> 0x14) & 0xF) == 1) {
        M2C_FIELD(w, s32 *, 0x10) |= 0x1000;
    } else {
        M2C_FIELD(w, s32 *, 0x10) &= ~0x1000;
    }
    if ((((u32) D_800D91E4 >> 0x14) & 0xF) == 2) {
        M2C_FIELD(w, s32 *, 0x10) |= 0x2000;
    } else {
        M2C_FIELD(w, s32 *, 0x10) &= ~0x2000;
    }
    if (((u32) D_800D91E4 >> 0x1A) & 1) {
        M2C_FIELD(w, s32 *, 0) |= 0x10000000;
    } else {
        M2C_FIELD(w, s32 *, 0) &= 0xEFFFFFFF;
    }
    if (((u8) D_8009CABF != 0) || (g_CutScene != 0)) {
        M2C_FIELD(w, s32 *, 0) |= 0x2000;
    }
    tgt = M2C_FIELD(obj, u8 **, 0x5C);
    if ((tgt != NULL) && (M2C_FIELD(tgt, s32 *, 0x34) <= 0)) {
        M2C_FIELD(w, s32 *, 0) |= 0x4000;
    }
    v = M2C_FIELD(w, s32 *, 0x1C8);
    if (v != 0) {
        M2C_FIELD(w, s32 *, 0x1C8) = v - 1;
        if (v - 1 == 0) {
            M2C_FIELD(w, s32 *, 0) |= 0x1000;
        }
    }
    tgt = M2C_FIELD(obj, u8 **, 0x5C);
    set = 0;
    if (tgt != NULL) {
        vp = M2C_FIELD(tgt, u8 **, 0x90) + 0x28;
        pos[0] = M2C_FIELD(vp, s32 *, 0);
        pos[1] = M2C_FIELD(vp, s32 *, 4);
        pos[2] = M2C_FIELD(vp, s32 *, 8);
        func_800705A8(D_800E2F54, pos, 0, (s32) &hit);
        if (M2C_FIELD(w, s32 *, 0x23C) != (hit - 1)) {
            set = 1;
        }
    }
    if (set) {
        M2C_FIELD(w, s32 *, 0x10) |= 0x8000;
    } else {
        M2C_FIELD(w, s32 *, 0x10) &= 0xFFFF7FFF;
    }
    if (D_8009CE3C == 1) {
        M2C_FIELD(w, s32 *, 0x10) |= 0x20;
    }
    if ((u32) (M2C_FIELD(w, u32 *, 0x54) - 4) < 3U) {
        M2C_FIELD(M2C_FIELD(obj, u8 **, 0x118), s32 *, 0) |= func_80051728(obj, NULL);
        yaw = M2C_FIELD(obj, s16 *, 0xC4);
    }
    tgt = M2C_FIELD(obj, u8 **, 0x5C);
    if (tgt == NULL) {
        M2C_FIELD(obj, void **, 0x5C) = (void *) g_Player;
        rflags = func_80051728(obj, (u8 *) g_Player);
        M2C_FIELD(obj, void **, 0x5C) = NULL;
    } else {
        rflags = func_80051728(obj, tgt);
    }
    if (rflags & 0x20000000) {
        M2C_FIELD(M2C_FIELD(obj, u8 **, 0x118), s32 *, 0) |= 0x20000000;
    }
    if (rflags & 0x40) {
        M2C_FIELD(M2C_FIELD(obj, u8 **, 0x118), s32 *, 0) |= 0x80;
        func_80053804(obj);
        if (((u32) g_Weapons[D_800B0DD4].flags >> 0x13) & 1) {
            M2C_FIELD(M2C_FIELD(obj, u8 **, 0x118), s32 *, 0) |= 0x400;
        }
        if (rflags & 0x100) {
            M2C_FIELD(M2C_FIELD(obj, u8 **, 0x118), s32 *, 0) |= 0x200;
        }
    } else if (!(M2C_FIELD(M2C_FIELD(obj, u8 **, 0x118), s32 *, 0x10) & 0x10000)) {
        if (M2C_FIELD(w, s32 *, 0x3C) == 4) {
            M2C_FIELD(w, s32 *, 0x3C) = 3;
        }
    }
    if (rflags & 0x10) {
        M2C_FIELD(M2C_FIELD(obj, u8 **, 0x118), s32 *, 0) |= 0x20;
    }
    M2C_FIELD(w, s32 *, 0x30) = M2C_FIELD(obj, s32 *, 0x110);
    M2C_FIELD(w, s32 *, 0x2C) = M2C_FIELD(obj, s16 *, 0xC4);
    M2C_FIELD(w, s32 *, 0x64) = M2C_FIELD(obj, s16 *, 0xC6);
    pk = func_80053ED8(obj);
    M2C_FIELD(w, Pickup **, 0x1D4) = pk;
    if (pk != NULL) {
        M2C_FIELD(w, s32 *, 0) |= 0x80000000;
        M2C_FIELD(w, s32 *, 0x58) = M2C_FIELD(w, Pickup **, 0x1D4)->type;
    }
    func_80057750(w, obj);
    func_80057648(w, obj);
    switch (M2C_FIELD(w, s32 *, 0x3C)) {
    case 0:
        if (M2C_FIELD(obj, u8 *, 6) == D_8009D30C) {
            if (M2C_FIELD(w, s32 *, 0x40) == 1) {
                tgt = M2C_FIELD(obj, u8 **, 0x8C);
                if (tgt != NULL) {
                    if (M2C_FIELD(tgt, u8 *, 6) != D_8009D314) {
                        func_800395E8(tgt);
                        M2C_FIELD(obj, void **, 0x8C) = NULL;
                    }
                }
            }
        } else {
            func_8005DF40(obj);
        }
        M2C_FIELD(w, s32 *, 0x3C) = 1;
        break;
    case 2:
        tgt = M2C_FIELD(obj, u8 **, 0x8C);
        M2C_FIELD(obj, u8 *, 0x3A) = D_8009D34C;
        if (tgt != NULL) {
            func_800395E8(tgt);
            M2C_FIELD(obj, void **, 0x8C) = NULL;
        }
        if (M2C_FIELD(obj, u8 *, 6) == D_8009D30C) {
            if (M2C_FIELD(w, s32 *, 0x40) == 3) {
                M2C_FIELD(obj, u8 *, 0x3A) = D_8009D314;
            } else {
                M2C_FIELD(obj, u8 *, 0x3A) = D_8009D2F8;
                M2C_FIELD(w, s32 *, 0x40) = 1;
            }
        }
        func_80039A34(obj);
        M2C_FIELD(w, s32 *, 0x3C) = 3;
        break;
    case 4:
        if ((M2C_FIELD(obj, u8 *, 6) == D_8009D30C) && (M2C_FIELD(obj, void **, 0x8C) == NULL)) {
            M2C_FIELD(w, s32 *, 0x3C) = 2;
        } else {
            func_80053114(obj);
        }
        break;
    }
    switch (M2C_FIELD(w, u32 *, 0x54)) {
    case 0:
        if (D_8009CE3C == 1) {
            M2C_FIELD(w, s32 *, 0x240) = 0;
        } else {
            M2C_FIELD(w, s16 *, 0x1F8) = 0;
        }
        break;
    case 1:
        if (D_8009CE3C == 1) {
            M2C_FIELD(w, s32 *, 0x240) = 0;
        } else {
            M2C_FIELD(w, s16 *, 0x1F8) = 0;
        }
        if (M2C_FIELD(w, s32 *, 0x10) & 1) {
            ang = M2C_FIELD(obj, s16 *, 0xC4);
            if (ang >= 0xB7) {
                M2C_FIELD(obj, s16 *, 0xC4) = 0xB6;
            } else if (ang < -0xB6) {
                M2C_FIELD(obj, s16 *, 0xC4) = -0xB6;
            }
        }
        M2C_FIELD(obj, u16 *, 0xA0) = M2C_FIELD(obj, s16 *, 0xC4);
        break;
    case 2:
        if (D_8009CE3C == 1) {
            M2C_FIELD(w, s32 *, 0x240) = 0;
        } else {
            M2C_FIELD(w, s16 *, 0x1F8) = 0;
        }
        if (!(M2C_FIELD(obj, s32 *, 0x64) & 0x7F)) {
            if (g_Args->cdArchive == 0) {
                sprintf(D_800B0BC0, &D_800A8944, M2C_FIELD(obj, u8 *, 6), M2C_FIELD(obj, u8 *, 0xA7), func_80053A58(obj));
                func_8006B1DC((s32 *) D_800B0BC0);
            }
        } else {
            func_80052630(obj, (M2C_FIELD(w, s32 *, 0x10) & 2) ? 1 : 0);
        }
        M2C_FIELD(obj, u16 *, 0xA0) = D_800D9232;
        break;
    case 3:
        tgt = M2C_FIELD(obj, u8 **, 0x5C);
        if (tgt != NULL) {
            vp = M2C_FIELD(tgt, u8 **, 0x90) + 0x28;
            if (D_8009CE3C == 1) {
                func_80052808(obj, (s16) M2C_FIELD(vp, s32 *, 0), (s16) M2C_FIELD(vp, s32 *, 4), (s16) M2C_FIELD(vp, s32 *, 8));
            }
            M2C_FIELD(obj, u16 *, 0xA0) = D_800D9232;
        }
        break;
    case 4:
        vp = w + 0x1D8;
        if (D_8009CE3C == 1) {
            func_80052808(obj, (s16) M2C_FIELD(vp, s32 *, 0), (s16) M2C_FIELD(vp, s32 *, 4), (s16) M2C_FIELD(vp, s32 *, 8));
        }
        M2C_FIELD(obj, u16 *, 0xA0) = D_800D9232;
        break;
    case 5:
        if (D_8009CE3C == 1) {
            M2C_FIELD(w, s32 *, 0x240) = 0;
        } else {
            M2C_FIELD(w, s16 *, 0x1F8) = 0;
        }
        if (M2C_FIELD(w, s32 *, 0x10) & 1) {
            if (yaw >= 0xB7) {
                yaw = 0xB6;
            } else if (yaw < -0xB6) {
                yaw = -0xB6;
            }
        }
        M2C_FIELD(obj, u16 *, 0xA0) = yaw;
        break;
    case 6:
        if (D_8009CE3C == 1) {
            M2C_FIELD(w, s32 *, 0x240) = 0;
        } else {
            M2C_FIELD(w, s16 *, 0x1F8) = 0;
        }
        ang = (M2C_FIELD(M2C_FIELD(obj, u8 **, 0x94), u16 *, 0x34) - M2C_FIELD(M2C_FIELD(obj, u8 **, 0x90), u16 *, 0x62)) & 0xFFF;
        if (ang >= 0x801) {
            ang |= 0xF000;
        }
        M2C_FIELD(obj, u16 *, 0xA0) = ang;
        if (M2C_FIELD(w, s32 *, 0x10) & 1) {
            if (ang >= 0xB7) {
                M2C_FIELD(obj, u16 *, 0xA0) = 0xB6;
            } else if (ang < -0xB6) {
                M2C_FIELD(obj, u16 *, 0xA0) = -0xB6;
            }
        }
        break;
    }
    func_8005772C(w);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80051AE0);
#endif

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

void func_80052B2C(s32 *out, u8 *obj) {
    s32 *p;
    u8 *m;
    if (obj == g_Player) {
        p = &g_Camera.posX;
        out[0] = *p++;
        out[1] = *p++;
        out[2] = *p;
        return;
    }
    m = *(u8 **)(obj + 0x90);
    out[0] = *(s32 *)(m + 0x28);
    out[1] = *(s32 *)(m + 0x2C);
    out[2] = *(s32 *)(m + 0x30);
    if (*(s32 *)(*(u8 **)(obj + 0x94) + 0x24) & 0x400) {
        out[1] += *(s16 *)(obj + 0x20) / 2;
    } else {
        out[1] += *(s16 *)(obj + 0x20);
    }
}

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

void func_80052DB8(void) {
    u8 *o;
    u8 *end;
    u8 *s1;
    s32 v1;
    s32 t;
    o = (u8 *)(D_800AFFC8 + D_800B0DB4 * 0x124);
    end = (u8 *)(D_800AFFC8 + D_800B07A4 * 0x124);
    for (; o <= end; o += 0x124) {
        if (o[4] == 0) {
            continue;
        }
        if (func_8005F93C(o) != 0) {
            continue;
        }
        func_8003AC1C(o, D_800B8F24);
        if (func_80053A2C(o) != 0) {
            GsGetLs(M2C_FIELD(o, u8 **, 0x90) + 0x10, M2C_FIELD(o, u8 **, 0x90) + 0x68);
            func_8005EDF8(o);
            continue;
        }
        if (func_8003C8A4(o, g_Player) != 0 && func_8004CA5C(o, g_Player) != 0) {
            continue;
        }
        s1 = M2C_FIELD(o, u8 **, 0x90);
        if (o[7] == 0x26) {
            if (func_800608E4(o) == 0) {
                M2C_FIELD(s1, s32 *, 0x10) = 2;
                M2C_FIELD(s1, s32 *, 0x2C) -= 0xC8;
                o[7] = 0;
            }
        } else {
            M2C_FIELD(o, s32 *, 0x64) &= 0xFFBFFFFF;
        }
        t = (o[6] * 0x70) + g_Args->unk228;
        if (M2C_FIELD(t, s16 *, 0x50) != 0) {
            M2C_FIELD(s1, s16 *, 0x60) = -M2C_FIELD(t, s16 *, 0x50);
            M2C_FIELD(s1, s32 *, 0x10) = 2;
        }
        if (M2C_FIELD(o, s32 *, 0x64) & 0x200000) {
            v1 = 0x384 - (D_8009CDA8 - M2C_FIELD(o, s32 *, 0x2C));
            if (v1 <= 0) {
                func_800395E8(o);
                continue;
            }
            if (v1 < 0x64) {
                if ((v1 % 16) >= 9) {
                    M2C_FIELD(o, s32 *, 0x7C) &= 0x7FFFFFFF;
                } else {
                    M2C_FIELD(o, s32 *, 0x7C) |= 0x80000000;
                }
            }
        }
        func_8005EDF8(o);
    }
}

s16 func_80053004(u8 *obj, s32 x, s32 z) {
    u8 *t;
    s32 a;

    t = *(u8 **)(obj + 0x90);
    D_1F8003E8[0] = x - *(s32 *)(t + 0x28);
    D_1F8003E8[2] = z - *(s32 *)(t + 0x30);
    a = (((s32 (*)(s32, s32))ratan2)(D_1F8003E8[0], D_1F8003E8[2]) - *(s16 *)(*(u8 **)(obj + 0x90) + 0x62)) & 0xFFF;
    if (a >= 0x801) {
        a -= 0x1000;
    }
    return a;
}

s16 func_80053080(u8 *obj, s32 dist, s32 y) {
    u8 *t;
    s32 d;
    s32 a;

    t = *(u8 **)(obj + 0x90);
    D_1F8003E8[0] = dist;
    d = y - *(s32 *)(t + 0x2C);
    D_1F8003E8[1] = d;
    a = (((s32 (*)(s32))ratan2)(-d) - *(s16 *)(*(u8 **)(obj + 0x90) + 0x60)) & 0xFFF;
    if (a >= 0x801) {
        a -= 0x1000;
    }
    return a;
}

void func_800530F4(void) {
    M2C_FIELD(&D_800D9234, u16 *, 0) = (u16) D_800D31B4;
    M2C_FIELD(&D_800D9234, u16 *, 2) = (u16) D_800D31B4;
    M2C_FIELD(&D_800D9234, u16 *, 4) = (u16) D_800D31B4;
}

#ifdef NON_MATCHING
void func_80053114(u8 *obj) {
    u8 *tgt;
    u8 *sub;
    u8 *s3;
    u8 *s2;
    u8 *eff;
    s32 ok;
    s32 a0;
    u32 sp18;
    s32 sp1C;
    s32 a2;
    s32 t;
    s32 half;
    s32 fr;
    s32 rv;
    s32 gt;
    s32 oa;
    u32 okv;
    u16 tv;
    u16 px;
    u16 py;
    s32 m;
    u32 v;
    s32 *pos;
    s32 idx;

    tgt = *(u8 **)(obj + 0x8C);
    if (tgt != 0) {
        a0 = *(s16 *)(obj + 0xC4);
        if (*(u32 *)(*(u8 **)(obj + 0x94) + 0x24) & 0x40000) {
            ok = (u32)(a0 + 0x200) < 0x401;
            goto test;
        }
        if (obj[6] == D_8009D30C && *(s32 *)(obj + 0x88) != 0) {
            goto body;
        }
        ok = (u32)(a0 + 0xFA) < 0x1F5U;
test:
        if (ok) {
body:
            sp18 = (*(*(u8 **)(obj + 0x94))) >> 4;
            if (g_Args->pad118[0x12] == 2) {
                func_80053650((s32 *)&sp18);
            }
            sub = *(u8 **)(tgt + 0x90);
            GsGetLs(sub + 0x10, sub + 0x68);
            *(SVECTOR *)D_1F8003E0 = *(SVECTOR *)(tgt + 0x3C);
            gte_SetRotMatrix(sub + 0x34);
            gte_ldv0(D_1F8003E0);
            gte_rtv0();
            gte_stlvnl(D_1F8003E8);
            pos = (s32 *)(sub + 0x48);
            D_1F8003D4 = pos[0] + D_1F8003E8[0];
            sp1C = 0;
            D_1F8003D8 = pos[1] + D_1F8003E8[1];
            D_1F8003DC = pos[2] + D_1F8003E8[2];
            func_80069C10(obj, tgt, &sp1C, &D_8009D5F0);
            if (tgt[6] == D_8009D314) {
                D_8009D5F0 = 1;
                func_80076A0C(tgt);
                return;
            }
            if (sp1C != 0 || D_8009D5F0 != 0) {
                s3 = (u8 *)func_80039A9C((s32)obj, tgt[0x3A], 7);
            } else {
                s3 = (u8 *)func_80039A9C((s32)obj, tgt[0x3A], *(u8 *)(g_Args->unk228 + tgt[0x3A] * 0x70 + 1));
            }
            D_8009D5F0 = 0;
            if (s3 != 0) {
                gt = D_8009CDA8;
                s2 = *(u8 **)(s3 + 0x90);
                D_8009DB38 = gt;
                if (*(u32 *)(obj + 0x48) & 0x4000000) {
                    if (*(u8 **)(obj + 0x5C) != 0) {
                        func_80036E10(*(u8 **)(obj + 0x5C), s3);
                        if (obj == (u8 *)D_8009CD90 || (obj[0xA] & 2) || sp18 == 0xA) {
                            px = *(u16 *)&D_800B0084;
                            py = *(u16 *)&D_800B0D50;
                            *(u16 *)(s2 + 0x62) = px;
                            *(u16 *)(s2 + 0x60) = py;
                        } else {
                            m = (g_Args->pad118[0x12] == 1) ? 0x17 : 0x11;
                            t = (0xA - sp18) * m;
                            half = t >> 1;
                            *(u16 *)(s2 + 0x62) = *(u16 *)&D_800B0084 + (half - rand() % t);
                            *(u16 *)(s2 + 0x60) = *(u16 *)&D_800B0D50 + (half - rand() % t);
                        }
                    } else {
                        func_8003BE68(sub + 0x34, s2 + 0x60);
                    }
                } else {
                    func_8003BE68(sub + 0x34, s2 + 0x60);
                }
                func_8003B53C(s3, *(s16 *)(s2 + 0x62), *(s16 *)(s2 + 0x60), 0xBB8);
                *(u8 **)(s3 + 0x5C) = *(u8 **)(obj + 0x5C);
                *(u32 *)(s3 + 0x7C) |= 0x80000000;
                if (*(u32 *)(obj + 0x48) & 0x800000) {
                    if (*(s16 *)(*(u8 **)obj + 0x36) >= 3) {
                        func_8006B230(0, (s32)D_800A89BC);
                    }
                    tv = *(u16 *)((u8 *)&D_800D9234 + *(s16 *)(*(u8 **)obj + 0x36) * 2);
                    if (tv != 0) {
                        eff = (u8 *)func_8003A898(tv & 0xFF, 0);
                        if (eff != 0) {
                            *(u16 *)(*(u8 **)(eff + 0x1C) + 0x62) = *(u16 *)(*(u8 **)(obj + 0x90) + 0x62);
                            rv = (rand() & 7) * 0x44;
                            fr = g_FrameRate;
                            *(s16 *)(*(u8 **)(eff + 0x1C) + 0x64) = rv;
                            if (fr >= 0x14) {
                                ((SVECTOR *)D_1F8003E0)->vy = -0x3C;
                                ((SVECTOR *)D_1F8003E0)->vz = 0xC8;
                                ((SVECTOR *)D_1F8003E0)->vx = 0;
                                func_800381CC(tgt, D_8009D280);
                            }
                        }
                    }
                }
                a0 = *(s32 *)(tgt + 0x34);
                if (a0 > 0) {
                    *(s32 *)(s3 + 0x34) = a0;
                }
                if ((g_Args->flags2 >> 12) & 1) {
                    *(s32 *)(s3 + 0x34) = *(s32 *)(s3 + 0x34) * 2;
                }
                func_80053DE4(0, 0, D_1F8003D4, D_1F8003D8, D_1F8003DC);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80053114);
#endif

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

s16 func_80053684(s32 x, s32 z, u8 *obj, s32 dist) {
    s32 *p = (s32 *)(*(u8 **)(obj + 0x90) + 0x28);
    if (((x - p[0]) < 0 ? p[0] - x : x - p[0]) + ((z - p[2]) < 0 ? p[2] - z : z - p[2]) < dist) {
        return 1;
    }
    return 0;
}

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

void func_80053790(u8 *arg0, s32 arg1) {
    s32 i;
    void *p;

    i = arg1;
    if (i < 0) {
        i = 5;
    }
    if (i < 6) {
        p = func_800540CC();
        *(void **)(arg0 + 0x118) = p;
        if (p != NULL) {
            func_80057A24(p, (&D_800D9248)[i]);
            arg0[0xA7] = i;
        }
    }
}

void func_80053804(u8 *arg0) {
  s16 x;
  s16 y;
  int new_var2;
  s32 w;
  s32 a;
  s16 new_var;
  s32 b;
  s32 dx;
  s32 dy;
  if (((func_800536F0((u32) arg0) << 16) == 0) && ((((u32) g_Weapons[D_800B0DD4].flags) >> 19) & 1))
  {
    new_var2 = 0x10;
    x = *((s16 *) (arg0 + new_var2));
    if (x >= 0)
    {
      y = *((s16 *) (arg0 + 0x12));
      new_var = y;
      if (new_var >= 0)
      {
        w = *((s16 *) (arg0 + 0x14));
        if (w > 0)
        {
 do { a = (((*((s16 *) (arg0 + 0x22))) - (*((s16 *) (arg0 + 0x20)))) * 0xF0) / w; w = (((*((s16 *) (arg0 + 0x1E))) - (*((s16 *) (arg0 + 0x1C)))) << 9) / w; if (a < 0) { a += 3; } dx = x - 0x100; dy = y - 0x78; a >>= 2; } while (0);
          if (dx < 0)
          {
            dx = -dx;
          }
          if (dy < 0)
          {
            dy = -dy;
          }
          if ((w >= dx) && (a >= dy))
          {
            *((s32 *) (*((s32 *) (arg0 + 0x118)))) |= 0x08000000;
          }
        }
      }
    }
  }
}

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

s32 func_80053A2C(void *o) {
    u8 *p = *(u8 **)((u8 *)o + 0x88);
    if (p != 0) {
        u32 c = p[5];
        s32 n = 6;
        if (c == n) return 1;
    }
    return 0;
}

s32 func_80053A58(void *arg0) {
    s32 var_v0;

    var_v0 = -1;
    if (arg0 != NULL) {
        var_v0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x60), u8 *, 7) & 7;
    }
    return var_v0;
}

void func_80053A7C(void) {
    u8 *obj = (u8 *)&D_800D90C0;
    s32 v;
    D_800D9268 = 0;
    *(s32 *)(obj + 0x124) &= ~0xF;
    *(s32 *)(obj + 0x124) &= 0xFFF0000F;
    *(s32 *)(obj + 0x124) &= 0xFF0FFFFF;
    *(s32 *)(obj + 0x124) &= 0xFDFFFFFF;
    *(s32 *)(obj + 0x124) &= 0xFBFFFFFF;
    v = func_8005D5C4((M2C_UNK *)&D_800B05B0, &D_800B05B0 + 0x188);
    *(s32 *)(obj + 0x94) = v;
    func_8005D58C(v);
    func_8003B940(obj, 0, 0, 0, 0, 1);
    func_80053790(obj, -1);
    D_8009D5F8 = 1;
}

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

void func_80053BE0(void) {
    u8 *obj;
    u8 *sub;
    s32 *cp;
    SVECTOR *cr;
    s32 *mp;
    s16 *mr;

    if (D_8009D5F8 != 0) {
        obj = (u8 *)&D_800D90C0;
        cp = &g_Camera.posX;
        cr = (SVECTOR *)(cp + 0xE);
        sub = *(u8 **)(obj + 0x118);
        mp = (s32 *)(*(u8 **)(obj + 0x90) + 0x28);
        mr = (s16 *)(*(u8 **)(obj + 0x90) + 0x60);
        if (obj[0x127] & 1) {
            *(s32 *)(sub + 0x10) |= 0x800;
        } else {
            *(s32 *)(sub + 0x10) &= ~0x800;
        }
        if (((D_800D91E4 >> 20) & 0xF) == 1) {
            *(s32 *)(sub + 0x10) |= 0x1000;
        } else {
            *(s32 *)(sub + 0x10) &= ~0x1000;
        }
        if (((D_800D91E4 >> 20) & 0xF) == 2) {
            *(s32 *)(sub + 0x10) |= 0x2000;
        } else {
            *(s32 *)(sub + 0x10) &= ~0x2000;
        }
        if ((D_800D91E4 >> 26) & 1) {
            *(s32 *)sub |= 0x10000000;
        } else {
            *(s32 *)sub &= ~0x10000000;
        }
        mp[0] = cp[0];
        mp[1] = cp[1];
        mp[2] = cp[2];
        mr[0] = cr->vx;
        mr[1] = cr->vy;
        mr[2] = cr->vz;
        if ((*(s16 **)(sub + 0x1D4) = func_80053ED8(obj)) != 0) {
            *(s32 *)sub |= 0x80000000;
            *(s32 *)(sub + 0x58) = **(s16 **)(sub + 0x1D4);
        }
        func_80057750(sub, obj);
        func_80057648(sub, obj);
        if (D_800D9268 > 0) {
            D_800D9268--;
        }
    }
}

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

void func_80053DE4(s16 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Slot *s = (Slot *)&D_800DA270;
    s32 i = 0;
    s16 t;
    s32 ok;
    do {
        t = s->id;
        ok = 0;
        if (D_8009D660[t].flag != 0) {
            ok = D_8009CDA8 >= s->expire;
        }
        if (t == 0x1C || ok) {
            s->id = arg0;
            s->b = arg1;
            s->c = arg2;
            s->d = arg3;
            s->e = arg4;
            s->expire = D_8009CDA8 + D_8009D660[s->id].flag;
            break;
        } else {
            i++;
            s++;
        }
    } while (i < 0x1B);
    if (i >= 0x1B) {
        printf(D_800A89F0);
    }
}

Pickup *func_80053ED8(u8 *obj) {
    VECTOR pos;
    s32 range;
    s32 fov;
    u32 mask;
    Pickup *e;
    Pickup *best;
    PickRec *rec;
    s16 i;
    s32 type;
    s32 t;
    u32 d;
    s16 a;
    u8 *sub;
    VECTOR *mp;

    mp = (VECTOR *)(*(u8 **)(obj + 0x90) + 0x28);
    pos.vx = mp->vx;
    pos.vy = mp->vy;
    pos.vz = mp->vz;
    best = NULL;
    e = (Pickup *)&D_800DA270;
    sub = *(u8 **)(obj + 0x118);
    range = *(s32 *)(sub + 0x4C);
    fov = *(s32 *)(sub + 0x50);
    mask = *(u32 *)(sub + 0x1D0);
    for (i = 0; i < 27; i++, e++) {
        type = e->type;
        rec = &D_8009D660[type];
        t = 0;
        if (rec->flag != 0) {
            t = (u32)D_8009CDA8 >= e->time;
        }
        if (t != 0) {
            continue;
        }
        if (type == 0x1C) {
            continue;
        }
        if (!(mask & (1 << type))) {
            continue;
        }
        if (best != NULL && type >= best->type) {
            continue;
        }
        d = func_8005397C(&pos, &e->pos);
        a = func_80053004(obj, e->pos.vx, e->pos.vz);
        if ((d < rec->dist + range && a < fov) || d < rec->range + 4000) {
            best = e;
        }
    }
    return best;
}

s32 func_80054098(s32 a) {
    if (D_8009D7A4 != 0) return D_8009D7A4(a);
    return 0;
}

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

void func_80054118(void) {
    u32 i;
    for (i = 0; i < 7; i++) {
        D_800D9278[i].flags &= ~2;
    }
}

void func_8005414C(void *arg0) {
    void *temp_a1;

    temp_a1 = M2C_FIELD(arg0, void **, 0x118);
    M2C_FIELD(temp_a1, s32 *, 0x1CC) = (s32) (M2C_FIELD(temp_a1, s32 *, 0x1CC) & ~2);
    M2C_FIELD(arg0, void **, 0x118) = NULL;
}

s32 func_8005416C(u32 *a, u32 *b) {
    u16 f;
    u16 flags;

    f = ((u16 *)a)[1];
    flags = f;
    if ((f & 0xFFF) != (((u16 *)b)[1] & 0xFFF)) {
        goto fail;
    }
    if (f & 1) {
        a++;
        b++;
        if (*a != *b) {
            goto fail;
        }
    }
    if (f & 2) {
        a++;
        b++;
        if (*a != *b) {
            goto fail;
        }
    }
    if (f & 4) {
        a++;
        b++;
        if (*a != *b) {
            goto fail;
        }
    }
    if (f & 8) {
        a++;
        b++;
        if (*a != *b) {
            goto fail;
        }
    }
    if (f & 0x10) {
        a++;
        b++;
        if (*a != *b) {
            goto fail;
        }
    }
    if (flags & 0x20) {
        a++;
        b++;
        if (*a != *b) {
            goto fail;
        }
    }
    return 1;
fail:
    return 0;
}

s32 *func_80054268(u32 *list, u32 *cmd) {
    s32 *p;
    u32 i;
    u32 n;
    s32 *found;

    p = (s32 *)(list + 2);
    found = NULL;
    n = list[0];
    for (i = 0; i < n; i++, p += 2) {
        if (p[0] == 0 && func_8005416C((u32 *)p[1], cmd) == 1) {
            found = p;
            break;
        }
    }
    return found;
}

s32 *func_8005430C(u32 *list, u32 *cmd) {
    s32 *p;
    u32 i;
    u32 n;
    s32 *found;

    n = list[1];
    p = (s32 *)(list + 2) + list[0] * 2;
    found = NULL;
    for (i = list[0]; i < n; i++, p += 2) {
        if (p[0] == 0 && func_8005416C((u32 *)p[1], cmd) == 1) {
            found = p;
            break;
        }
    }
    return found;
}

s32 *func_800543BC(u32 *list) {
    s32 *p;
    u32 i;
    u32 n;
    s32 *found;

    p = (s32 *)(list + 2) + list[0] * 2;
    i = list[0];
    n = list[1];
    found = NULL;
    for (; i < n; i++, p += 2) {
        if (p[0] == 1) {
            found = p;
            break;
        }
    }
    return found;
}

s32 func_80054414(u8 level, u8 arg1, u8 arg2) {
    s32 i;

    if (*(u16 *)&g_Args->missionComplete != 0) {
        return 0;
    }
    i = 1;
    if (g_Args->cdArchive != 0) {
        while (func_80075FA0() != 0) {
            if (i % 20 == 0) {
                func_8006B1DC((s32)&D_800A8A08);
            }
            i++;
        }
    }
    ((MissionChr *)&D_800D90C0)->unk26 = 0;
    ((MissionChr *)&D_800D90C0)->unk24 = 1;
    ((MissionChr *)&D_800D90C0)->mission = level;
    ((MissionChr *)&D_800D90C0)->unk20 = 0;
    ((MissionChr *)&D_800D90C0)->completed |= 1 << (level - 1);
    D_8009CF54 = 1;
    func_800397A8();
    Menu_ResetState();
    Camera_ResetZoom();
    func_80061708(1);
    if (arg1 == 1) {
        func_8004A29C();
    } else {
        func_8004A2E0();
    }
    if (arg2) {
        ((MissionChr *)&D_800D90C0)->unk25 = 0;
        *(s16 *)((u8 *)g_Args + 0x144) >>= 2;
        *(s16 *)((u8 *)g_Args + 0x13A) >>= 1;
    } else {
        ((MissionChr *)&D_800D90C0)->unk25 = 1;
    }
    return 1;
}

s32 func_80054604(u32 *cmd, ScriptObj *obj) {
    u32 v;
    u32 a;
    u32 b;
    u32 f;
    s32 ret;

    v = *cmd;
    ret = 1;
    a = v >> 16;
    b = v >> 24;
    f = v >> 8;
    if (obj != NULL) {
        if ((f & 3) == ret) {
            a = (u8)obj->vars[a & 0xFF];
        }
        if ((f & 0xC) == 4) {
            b = (u8)obj->vars[b];
        }
        func_80054414(a, b, 1);
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return ret;
}

void func_8005469C(u8 arg0) {
    u8 *part;
    VECTOR *pos;
    s32 *tgt;

    part = *(u8 **)((u8 *)g_Player + 0x90);
    pos = (VECTOR *)(part + 0x28);
    tgt = &g_Camera.targetX;
    tgt[0] = pos->vx;
    tgt[1] = pos->vy;
    tgt[2] = pos->vz;
    *(SVECTOR *)(tgt + 3) = *(SVECTOR *)(part + 0x60);
    ((MissionChr *)&D_800D90C0)->unk24 = 0;
    func_80061708(0);
    if (arg0 == 1) {
        func_8004A304();
    } else {
        func_8004A338();
    }
    D_8009CF54 = 0;
    D_800D9268 = 0x23;
    func_8002C664();
    if (!((MissionChr *)&D_800D90C0)->unk25) {
        *(s16 *)((u8 *)g_Args + 0x144) <<= 2;
        *(s16 *)((u8 *)g_Args + 0x13A) <<= 1;
    } else {
        ((MissionChr *)&D_800D90C0)->unk25 = 0;
    }
}

s32 func_800547C4(u32 *cmd, ScriptObj *obj) {
    u32 v;
    u32 a;
    u32 f;
    s32 ret;

    v = *cmd;
    ret = 1;
    a = v >> 16;
    f = v >> 8;
    if (obj != NULL) {
        if ((f & 3) == ret) {
            a = (u8)obj->vars[a & 0xFF];
        }
        func_8005469C(a);
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return ret;
}

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

s32 func_80054964(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 n;
    u32 v;
    u32 f;
    u16 frames;
    s32 *tgt;
    s32 *pos;
    s32 *cam;

    v = *cmd;
    n = 1;
    frames = v >> 16;
    f = v >> 8;
    if (obj != NULL) {
        if ((f & 3) == n) {
            frames = obj->vars[frames];
        }
        if (frames >= 2) {
            tgt = &g_Camera.targetX;
            pos = &g_Camera.posX;
            cam = (s32 *)&g_Camera;
            cam[6] = (s32)(tgt[0] - pos[0]) / frames;
            cam[7] = (s32)(tgt[1] - pos[1]) / frames;
            cam[8] = (s32)(tgt[2] - pos[2]) / frames;
        } else {
            func_8004EA68(chr);
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

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

s32 func_80054BF8(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 n;
    u32 v;
    u32 f;
    u16 frames;
    s16 *tgt;
    s16 *pos;
    s32 *cam;
    s32 dx, dy, dz;

    v = *cmd;
    n = 1;
    frames = v >> 16;
    f = v >> 8;
    if (obj != NULL) {
        if ((f & 3) == n) {
            frames = obj->vars[frames];
        }
        if (frames >= 2) {
            tgt = &g_Camera.rotX;
            pos = (s16 *)((u8 *)&g_Camera + 0x80);
            dx = (tgt[0] - pos[0]) & 0xFFF;
            if (dx > 0x800) {
                dx -= 0x1000;
            }
            dy = (tgt[1] - pos[1]) & 0xFFF;
            if (dy > 0x800) {
                dy -= 0x1000;
            }
            dz = (tgt[2] - pos[2]) & 0xFFF;
            if (dz > 0x800) {
                dz -= 0x1000;
            }
            cam = (s32 *)&g_Camera;
            cam[9] = dx / frames;
            cam[10] = dy / frames;
            cam[11] = dz / frames;
        } else {
            func_8004EAB8(chr);
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

s32 func_80054DA4(u32 *cmd, ScriptObj *obj) {
    u16 temp_a0;
    u8 *temp_v1;
    void *temp_a1;
    void *temp_a1_2;
    if (obj != NULL) {
        s32 *c = (s32 *)&g_CamPosX;
        temp_a1 = M2C_FIELD(g_Player, void **, 0x90);
        g_Args->playerX = c[0];
        g_Args->playerY = c[1];
        g_Args->playerZ = c[2];
        temp_v1 = &g_CamPosX + 0x38;
        temp_a0 = M2C_FIELD(&g_CamPosX, u16 *, 0x38);
        M2C_FIELD(temp_a1, u16 *, 0x60) = temp_a0;
        D_800AFF8C = temp_a0;
        temp_a0 = M2C_FIELD(temp_v1, u16 *, 2);
        temp_a1_2 = temp_a1 + 0x60;
        M2C_FIELD(temp_a1_2, u16 *, 2) = temp_a0;
        gPsi = (s16) temp_a0;
        D_800B0AFC = temp_a0;
        M2C_FIELD(temp_a1_2, u16 *, 4) = (u16) M2C_FIELD(temp_v1, u16 *, 4);
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return 1;
}

s32 func_80054E38(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 n;
    u32 v;
    u32 f;
    u16 id;

    v = *cmd;
    n = 1;
    id = v >> 16;
    f = v >> 8;
    if (obj != NULL) {
        if ((f & 3) == n) {
            id = obj->vars[id];
        }
        if (id == 0) {
            *(void **)(chr + 0x5C) = NULL;
        } else {
            if (((ObjDefArr *)g_Args->unk228)->r[id].type == 6) {
                *(void **)(chr + 0x5C) = func_8004BF70(&D_800D90C0, id);
            } else {
                *(void **)(chr + 0x5C) = func_8004BE84(&D_800D90C0, id);
            }
            if (*(void **)(chr + 0x5C) != NULL) {
                func_8004EB20();
            }
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

s32 func_80054F34(u32 *cmd, ScriptObj *obj) {
    s32 n;
    u32 v;
    u32 f;
    u16 id;

    v = *cmd;
    n = 1;
    id = v >> 16;
    f = v >> 8;
    if (obj != NULL) {
        if ((f & 3) == n) {
            id = obj->vars[id];
        }
        if (id == 0xB1) {
            func_80082F24(0, 0);
            func_8002B0A0();
        } else {
            func_8002B064(id);
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

s32 func_80054FDC(u32 *cmd, ScriptObj *obj) {
    u32 v;
    u32 idx;
    u32 f;
    s32 ret;

    v = *cmd;
    ret = 1;
    idx = v >> 16;
    f = v >> 8;
    if (obj != NULL) {
        if ((f & 3) == ret) {
            idx = (u16)obj->vars[idx];
        }
        func_8002AFF8(idx);
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return ret;
}

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

s32 func_8005516C(u32 *cmd, ScriptObj *obj, void *arg) {
    if (obj != NULL) {
        obj->wait = 1;
        *(s32 *)((u8 *)arg + 0x34) = 0;
        func_8005E228(arg);
    }
    return 1;
}

s32 func_8005519C(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 ret;

    ret = 1;
    if (obj != NULL) {
        obj->wait = ret;
        if (g_Args->cdArchive == 0) {
            printf(D_800A8A74, chr[0xA7], chr[6], obj->pc - *(u32 **)((u8 *)obj + 0xA0));
        }
    }
    return ret;
}

s32 func_80055204(u32 *cmd, ScriptObj *obj) {
    s32 n;
    u32 w;
    s32 x;
    u32 dst;
    s32 one;
    s32 four;
    u32 f;

    one = 1;
    four = 4;
    n = 2;
    w = *cmd;
    x = cmd[1];
    dst = w >> 8;
    f = w >> 16;
    if (obj != NULL) {
        if ((f & 0xC) == four) {
            x = obj->vars[x];
        }
        if ((dst & 0xFF) != 7 || x != 3 || obj->vars[7] != one) {
            obj->vars[dst & 0xFF] = x;
        }
        if ((dst & 0xFF) == 0xD) {
            *(s16 *)((u8 *)obj + 0x1F8) = 0;
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

s32 func_8005529C(u32 *cmd, ScriptObj *obj) {
    u32 w;
    s32 x;
    u32 dst;
    s32 four;
    u32 f;

    w = *cmd;
    x = cmd[1];
    dst = w >> 8;
    f = w >> 16;
    four = 4;
    if (obj != NULL) {
        if ((f & 0xC) == four) {
            x = obj->vars[x];
        }
        obj->vars[dst & 0xFF] += x;
        obj->wait = 0;
        obj->pc = cmd + 2;
    }
    return 2;
}

s32 func_800552FC(u32 *cmd, ScriptObj *obj) {
    u32 w;
    s32 x;
    u32 dst;
    s32 four;
    u32 f;

    w = *cmd;
    x = cmd[1];
    dst = w >> 8;
    f = w >> 16;
    four = 4;
    if (obj != NULL) {
        if ((f & 0xC) == four) {
            x = obj->vars[x];
        }
        obj->vars[dst & 0xFF] -= x;
        obj->wait = 0;
        obj->pc = cmd + 2;
    }
    return 2;
}

s32 func_8005535C(u32 *cmd, ScriptObj *obj) {
    u32 w;
    s32 x;
    u32 dst;
    s32 four;
    u32 f;

    w = *cmd;
    x = cmd[1];
    dst = w >> 8;
    f = w >> 16;
    four = 4;
    if (obj != NULL) {
        if ((f & 0xC) == four) {
            x = obj->vars[x];
        }
        obj->vars[dst & 0xFF] *= x;
        obj->wait = 0;
        obj->pc = cmd + 2;
    }
    return 2;
}

s32 func_800553C0(u32 *cmd, ScriptObj *obj) {
    s32 n;
    u32 w;
    s32 x;
    u32 dst;
    s32 four;
    u32 f;

    four = 4;
    n = 2;
    w = *cmd;
    x = cmd[1];
    dst = w >> 8;
    f = w >> 16;
    if (obj != NULL) {
        if ((f & 0xC) == four) {
            x = obj->vars[x];
        }
        if (x != 0) {
            obj->vars[dst & 0xFF] /= x;
        } else {
            printf(D_800A8AA8);
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

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

s32 func_80055510(u32 *cmd, ScriptObj *obj) {
    u32 b = cmd[0] >> 8;
    if (obj != NULL) {
        obj->pc = cmd + 1;
        *(u32 *)((u8 *)obj + 0x1C) |= 1 << b;
        obj->wait = 0;
    }
    return 1;
}

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

s32 func_800557C8(u32 *cmd, ScriptObj *obj, void *arg) {
    if (obj != NULL) {
        obj->wait = 1;
        func_800395E8(arg);
    }
    return 1;
}

#ifdef NON_MATCHING
s32 func_800557F4(u32 *cmd, ScriptObj *obj) {
    s32 n = 1;
    u16 f = ((volatile u16 *)cmd)[1];
    if (obj != NULL) {
        obj->wait = 0;
        obj->pc = cmd + n;
        return n;
    }
    return n;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800557F4);
#endif

s32 func_80055814(u32 *a, HalfTbl20 *b) {
    u32 w = *a;
    s32 r = 1;
    u32 x = w >> 16;
    u32 y = w >> 8;
    if (b != 0) {
        M2C_FIELD(b, s32 *, 0) &= ~0x1000;
        M2C_FIELD(b, s32 *, 4) &= ~0x1000;
        if ((y & 3) == r) x = b->e[x].a;
        M2C_FIELD(b, u32 *, 0x1C8) = x;
        M2C_FIELD(b, u32 **, 0xA4) = a + 1;
    }
    return r;
}

s32 func_80055870(void) {
    return 0;
}

#ifdef NON_MATCHING
s32 func_80055878(u32 *cmd, ScriptObj *obj, u8 *src) {
    u32 w;
    u32 idx;
    s32 one;
    w = *cmd;
    one = 1;
    idx = w >> 16;
    if (obj != NULL) {
        switch ((w >> 8) & 0xFF) {
        case 0: obj->vars[idx & 0xFF] = *(u32 *)(src + 0x34); break;
        case 1: obj->vars[idx & 0xFF] = *(u32 *)(*(u8 **)src + 0x20); break;
        case 2: obj->vars[idx & 0xFF] = ((s32)(*(u32 *)(src + 0x34) << 0x12) / (s32)(*(u32 *)(*(u8 **)src + 0x20) << 0xC)) << 6; break;
        case 3:
            func_80036E10(src, (u8 *)g_Player);
            obj->vars[idx & 0xFF] = *(s16 *)((u8 *)g_Player + 0xC4);
            break;
        case 5: obj->vars[idx & 0xFF] = (*(u8 **)(src + 0x60))[3] & 7; break;
        case 6: obj->vars[idx & 0xFF] = (*(u32 *)*(u8 **)(src + 0x60) >> 0xE) & 7; break;
        case 7: obj->vars[idx & 0xFF] = *(s32 *)(((*(u32 *)*(u8 **)(src + 0x60) >> 0x12) & 0xC) + D_8009D600); break;
        case 8: obj->vars[idx & 0xFF] = *(s32 *)(((*(u32 *)*(u8 **)(src + 0x60) >> 0x14) & 0xC) + D_8009D610); break;
        case 9: obj->vars[idx & 0xFF] = *(s32 *)(((*(u32 *)(*(u8 **)(src + 0x60) + 4) >> 0x19) & 0x3C) + D_8009D620); break;
        case 10:
            if (D_8009D5FC != NULL) {
                obj->vars[idx & 0xFF] = *(u16 *)(D_8009D5FC + 0x64) & 0x7F;
            } else {
                obj->vars[idx & 0xFF] = 0;
            }
            break;
        case 11: obj->vars[idx & 0xFF] = (*(u32 *)(*(u8 **)(src + 0x60) + 4) >> 7) & 0x1F; break;
        case 12:
            if (*(s32 *)(src + 0x64) & 0x7F) {
                obj->vars[idx & 0xFF] = D_8009D5AC[(*(u16 *)(src + 0x64) & 0x7F) - 1];
            } else {
                obj->vars[idx & 0xFF] = 0;
            }
            break;
        case 13: obj->vars[idx & 0xFF] = (*(u8 **)(src + 0x60))[7] & 7; break;
        case 14: obj->vars[idx & 0xFF] = D_800D91E4 & 0xF; break;
        case 17: obj->vars[idx & 0xFF] = (D_800D91E4 >> 4) & 0xFFFF; break;
        case 18: obj->vars[idx & 0xFF] = D_8009CABE; break;
        case 23: obj->vars[idx & 0xFF] = **(u8 **)(src + 0x94) >> 4; break;
        }
        obj->wait = 0;
        obj->pc = cmd + one;
    }
    return one;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055878);
#endif

#ifdef NON_MATCHING
s32 func_80055BA8(u32 *cmd, ScriptObj *obj, u8 *tgt) {
    u32 w;
    u32 op;
    u32 fl;
    s32 v;
    s32 a1;
    s32 two;
    s32 four;
    four = 4;
    two = 2;
    w = cmd[0];
    v = cmd[1];
    op = w >> 8;
    fl = w >> 16;
    if (obj != NULL) {
        if ((fl & 0xC) == four) {
            v = *(s32 *)((u8 *)obj + (v << two) + 0x20);
        }
        switch (op & 0xFF) {
        case 0:
            *(s32 *)(tgt + 0x34) = v;
            break;
        case 11:
            {
                u8 r;
                *(s32 *)(tgt + 0x6C) &= ~0xFF;
                r = func_800523E4(tgt, v & 0xFF);
                *(s32 *)(tgt + 0x64) = (v & 0x7F) | (*(s32 *)(tgt + 0x64) & ~0x7F);
                *(s32 *)(tgt + 0x6C) |= r;
            }
            break;
        case 15:
            *(s32 *)((u8 *)obj + 0x10) &= ~0x1000;
            *(s32 *)((u8 *)obj + 0x10) &= ~0x2000;
            {
                s32 *q = (s32 *)&D_800D90C0;
                q[0x49] = (q[0x49] & 0xFF0FFFFF) | ((v & 0xF) << 0x14);
            }
            break;
        case 16:
            if (v != 0) {
                *(s32 *)((u8 *)D_8009CD98 + 0x7C) &= 0x7FFFFFFF;
            } else {
                *(s32 *)((u8 *)D_8009CD98 + 0x7C) |= 0x80000000;
            }
            break;
        case 13:
            {
                u8 *p = *(u8 **)(tgt + 0x60);
                *(s32 *)(p + 4) = (*(s32 *)(p + 4) & 0xF8FFFFFF) | ((v & 7) << 0x18);
            }
            break;
        case 21:
            if (v != 0) {
                u8 *p = *(u8 **)(tgt + 0x94);
                *(s32 *)(p + 0x24) |= 0x400;
            } else {
                u8 *p = *(u8 **)(tgt + 0x94);
                *(s32 *)(p + 0x24) &= ~0x400;
            }
            break;
        case 22:
            if (v != 0) {
                tgt[0xA4] |= 8;
            } else {
                tgt[0xA4] &= 0xF7;
            }
            break;
        case 23:
            if (v > 0) {
                a1 = 10;
                if (v < 11) a1 = v;
            } else {
                a1 = 1;
            }
            {
                u8 *p = *(u8 **)(tgt + 0x94);
                *(s32 *)p = (*(s32 *)p & ~0xF0) | ((a1 & 0xF) * 0x10);
            }
            break;
        }
        *(s32 *)((u8 *)obj + 0x1C4) = 0;
        *(u32 **)((u8 *)obj + 0xA4) = cmd + two;
    }
    return two;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80055BA8);
#endif

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

s32 func_80055E88(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 ret;

    ret = 1;
    if (obj != NULL) {
        obj->wait = ret;
        if (g_Args->cdArchive == 0) {
            printf(D_800A8B80, chr[0xA7], chr[6], obj->pc - *(u32 **)((u8 *)obj + 0xA0));
        }
    }
    return ret;
}

s32 func_80055EF0(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 ret;

    ret = 1;
    if (obj != NULL) {
        obj->wait = ret;
        if (g_Args->cdArchive == 0) {
            printf(D_800A8BAC, chr[0xA7], chr[6], obj->pc - *(u32 **)((u8 *)obj + 0xA0));
        }
    }
    return ret;
}

s32 func_80055F58(u32 *cmd, ScriptObj *obj, u8 *chr) {
  u32 w;
  u32 new_var;
  u8 *anim;
  u8 id1;
  s8 start;
  u8 id2;
  s8 end;
  anim = *((u8 **) (chr + 0x94));
  w = cmd[1];
  new_var = w;
  id1 = w;
  start = w >> 8;
  id2 = new_var >> 16;
  end = w >> 24;
  if (obj != ((void *) 0))
  {
    if (start == (-1))
    {
      start = func_80057AB0(id1);
    }
    if (end == (-1))
    {
      end = func_80057AB0((short) id2);
    }
    func_80057B2C(anim, id1, start, id2, end);
    obj->wait = *((s16 *) (anim + 0x20));
    obj->pc = cmd + 2;
  }
  return 2;
}

s32 func_80056054(u32 *cmd, ScriptObj *obj, u8 *chr) {
    void *anim = *(void **)(chr + 0x94);
    u32 v = cmd[0];
    s32 c = cmd[1];
    u32 a = v >> 16;
    u32 b = v >> 24;
    u32 f = v >> 8;
    s32 n = 1;
    s32 k = 4;
    if (obj != NULL) {
        if ((f & 3) == n) {
            a = (u8)obj->vars[a & 0xFF];
        }
        if ((f & 0xC) == k) {
            b = (u8)obj->vars[b];
        }
        if ((f & 0x30) == 0x10) {
            c = obj->vars[c];
        }
        ((void (*)())func_80057B94)(anim, a & 0xFF, b, c);
        obj->wait = n;
        obj->pc = cmd + 2;
    }
    return 2;
}

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

#ifdef NON_MATCHING
s32 func_80056154(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 from[4];
    s32 to[4];
    s16 normal[4];
    s16 hit[4];
    s32 flags;
    s32 n;
    s32 ang;
    u32 w;
    s32 dist;
    u32 dst;
    u32 f;
    s32 result;
    s32 i;
    s32 dx, dz;
    s32 four;
    u8 *hitobj;

    four = 4;
    n = 3;
    ang = cmd[1];
    w = cmd[0];
    dist = cmd[2];
    dst = w >> 8;
    f = w >> 16;
    if (obj != NULL) {
        if ((f & 0xC) == four) {
            ang = obj->vars[ang];
        }
        if ((f & 0x30) == 0x10) {
            dist = obj->vars[dist];
        }
        from[0] = *(s32 *)(*(u8 **)(chr + 0x90) + 0x28);
        from[1] = *(s32 *)(*(u8 **)(chr + 0x90) + 0x2C) - 200;
        from[2] = *(s32 *)(*(u8 **)(chr + 0x90) + 0x30);
        to[1] = from[1];
        i = (*(u16 *)(*(u8 **)(chr + 0x90) + 0x62) + ang) & 0xFFF;
        result = 0;
        dx = (dist * *(s16 *)&D_800A0370[i]) >> 12;
        dz = (dist * (*(volatile s32 *)&D_800A0370[i] >> 16)) >> 12;
        to[0] = from[0] + dx;
        to[2] = from[2] + dz;
        if (func_800746D0(D_800E2F2C, from, to, normal, hit, &flags) != 0) {
            result = 1;
            to[0] = hit[0];
            to[1] = hit[1];
            to[2] = hit[2];
        }
        hitobj = func_80050A70(to, from, chr, 0);
        D_8009D5FC = hitobj;
        if (hitobj != NULL) {
            result = 1;
            if (*(s32 *)(hitobj + 0x64) & 0x7F) {
                result = 2;
            }
        }
        obj->vars[dst & 0xFF] = result;
        if (result == 2) {
            *(s32 *)obj |= 0x800;
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80056154);
#endif

s32 func_80056358(u32 *cmd, ScriptObj *obj) {
    s32 n;
    u32 idx;

    idx = *cmd >> 8;
    n = 1;
    if (obj != NULL) {
        if ((idx & 0xFF) == 0x1C) {
            *(u32 *)obj &= 0x7FFFFFFF;
            *(s32 *)((u8 *)obj + 0x1D0) = 0;
        } else {
            *(s32 *)((u8 *)obj + 0x1D0) |= n << (idx & 0xFF);
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

s32 func_800563BC(u32 *cmd, ScriptObj *obj, u8 *chr) {
    u32 idx;
    s32 *pos;

    idx = *cmd >> 8;
    if (obj != NULL) {
        pos = (s32 *)(*(u8 **)(chr + 0x90) + 0x28);
        func_80053DE4(idx & 0xFF, 0, pos[0], pos[1], pos[2]);
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return 1;
}

s32 func_8005642C(s32 arg0, void *arg1) {
    if (arg1 != NULL) {
        M2C_FIELD(arg1, s32 *, 0x1C4) = 0;
        M2C_FIELD(arg1, s32 *, 0xA4) = (s32) (arg0 + 4);
    }
    return 1;
}

s32 func_80056444(u32 *cmd, ScriptObj *obj, u8 *chr) {
    /* op 0x2B: target position (+0x1D8) = position of the record at +0x1D4, model heading = its +2; 1 word */
    s32 n;

    n = 1;
    if (obj != NULL) {
        if (*(u8 **)((u8 *)obj + 0x1D4) != NULL) {
            *(s32 *)((u8 *)obj + 0x1D8) = *(s32 *)(*(u8 **)((u8 *)obj + 0x1D4) + 4);
            *(s32 *)((u8 *)obj + 0x1DC) = *(s32 *)(*(u8 **)((u8 *)obj + 0x1D4) + 8);
            *(s32 *)((u8 *)obj + 0x1E0) = *(s32 *)(*(u8 **)((u8 *)obj + 0x1D4) + 0xC);
            if (chr != NULL) {
                *(u16 *)(*(u8 **)(chr + 0x94) + 0x34) = *(u16 *)(*(u8 **)((u8 *)obj + 0x1D4) + 2);
            }
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

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

s32 func_80056538(u32 *arg0, Q *arg1) {
    u32 w = *arg0;
    u32 i;
    if (arg1 != NULL) {
        i = (w >> 8) & 0xFF;
        KEEP(i);
        arg1->t[i] = (s32) g_Player;
        M2C_FIELD(arg1, s32 *, 0x1C4) = 0;
        M2C_FIELD(arg1, void **, 0xA4) = (void *) (arg0 + 1);
    }
    return 1;
}

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

s32 func_800565DC(u32 *arg0, Q *arg1, void *arg2) {
    u32 w = *arg0;
    u32 i;
    if (arg1 != NULL) {
        i = (w >> 8) & 0xFF;
        KEEP(i);
        M2C_FIELD(arg2, s32 *, 0x5C) = arg1->t[i];
        M2C_FIELD(arg1, s32 *, 0x1C4) = 0;
        M2C_FIELD(arg1, void **, 0xA4) = (void *) (arg0 + 1);
    }
    return 1;
}

s32 func_80056614(u32 *cmd, ScriptObj *obj) {
    /* op 0x08: vars[byte 1] = a + rand() % b (a, b: immediate or variable, modes bits 2-3 / 4-5); 3 words */
    u32 w;
    u32 dst;
    u32 f;
    s32 a;
    s32 b;
    s32 k;

    k = 4;
    a = cmd[1];
    w = cmd[0];
    b = cmd[2];
    dst = w >> 8;
    f = w >> 16;
    if (obj != NULL) {
        if ((f & 0xC) == k) {
            a = obj->vars[a];
        }
        if ((f & 0x30) == 0x10) {
            b = obj->vars[b];
        }
        obj->vars[dst & 0xFF] = a + rand() % b;
        obj->wait = 0;
        obj->pc = cmd + 3;
    }
    return 3;
}

s32 func_800566F8(u32 *cmd, ScriptObj *obj) {
    s32 t;
    u32 w;
    s32 a;
    s32 b;
    u32 op;
    u32 flags;
    s32 one;
    s32 four;
    s32 n;

    t = 0;
    one = 1;
    four = 4;
    n = 3;
    a = cmd[1];
    w = cmd[0];
    b = cmd[2];
    op = w >> 8;
    flags = w >> 24;
    if (obj != NULL) {
        if ((flags & n) == one) {
            a = obj->vars[a];
        }
        if ((flags & 0xC) == four) {
            b = obj->vars[b];
        }
        switch (op & 0xFF) {
        case 0: if (a == b) t = 1; break;
        case 1: if (a != b) t = 1; break;
        case 2: if (a > b) t = 1; break;
        case 3: if (a >= b) t = 1; break;
        case 4: if (a < b) t = 1; break;
        case 5: if (a <= b) t = 1; break;
        case 6: if ((a & b) == b) t = 1; break;
        case 7: if ((a & b) == 0) t = 1; break;
        }
        obj->wait = 0;
        if (t == 1) {
            obj->pc = cmd + n + 1;
        } else {
            obj->pc = cmd + n;
        }
    }
    return n;
}

s32 func_8005683C(u32 *cmd, ScriptObj *obj) {
    /* op 0x22: set the first word of every entry in the list at +0xA8 to 1 (list[1] = count); 1 word */
    s32 n;
    s32 *p;
    u32 *list;
    u32 cnt;
    u32 i;

    n = 1;
    if (obj != NULL) {
        list = (u32 *)((u8 *)obj + 0xA8);
        p = (s32 *)((u8 *)obj + 0xB0);
        if (list != NULL) {
            cnt = *(u32 *)((u8 *)obj + 0xA8);
            for (i = 0; i < cnt; i++) {
                *p = 1;
                p += 2;
            }
            list[1] = cnt;
        }
        obj->wait = 0;
        obj->pc = cmd + n;
    }
    return n;
}

s32 func_80056894(u32 *cmd, ScriptObj *obj, void *arg2) {
    register u32 f REG("v1");
    s32 n;
    u32 *sub;
    s32 subop;
    s32 one;
    s32 found;
    s32 t;
    u32 *p;
    u32 m;
    s32 res;
    u16 f3;

    f = ((u16 *)cmd)[1];
    n = ((f & 1) + ((f >> 1) & 1) + ((f >> 2) & 1) + ((f >> 3) & 1) + ((f >> 4) & 1) + ((f >> 5) & 1)) + 1;
    sub = cmd + n;
    subop = *(u8 *)sub;
    if (obj != NULL) {
        t = *(s32 *)((u8 *)obj + 0x1CC) & 1;
        one = 1;
        found = 0;
        if (t == one) {
            u32 *pc;
            u32 *list;
            s32 *item;
            s32 ok;
            s32 cnt;

            pc = obj->pc;
            list = (u32 *)((u8 *)obj + 0xA8);
            {
                u32 fl = ((u16 *)pc)[1];
                cnt = (fl & 1) + ((fl >> 1) & 1) + ((fl >> 2) & 1) + ((fl >> 3) & 1) + ((fl >> 4) & 1) + ((fl >> 5) & 1);
            }
            found = *(u8 *)(pc + cnt + 1) == 0x1E;
            item = func_80054268(list, pc);
            if (item != NULL) {
                ok = 1;
            } else if (*(u32 *)((u8 *)obj + 0xA8) < 0x20) {
                item = (s32 *)(list + 2) + *(u32 *)((u8 *)obj + 0xA8) * 2;
                ok = 1;
                *(u32 *)((u8 *)obj + 0xA8) += 1;
                list[1] = *(u32 *)((u8 *)obj + 0xA8);
            } else {
                ok = 0;
            }
            if (ok == 1) {
                item[1] = (s32)pc;
                item[0] = found;
            }
        } else {
            u32 *pc;
            u32 *list;
            s32 *item;
            s32 once;
            s32 cnt;

            pc = obj->pc;
            list = (u32 *)((u8 *)obj + 0xA8);
            {
                u32 fl = ((u16 *)pc)[1];
                cnt = (fl & 1) + ((fl >> 1) & 1) + ((fl >> 2) & 1) + ((fl >> 3) & 1) + ((fl >> 4) & 1) + ((fl >> 5) & 1);
            }
            once = *(u8 *)(pc + cnt + 1) == 0x1E;
            item = func_80054268(list, pc);
            if (item != NULL) {
                item[0] = one;
            }
            item = func_8005430C(list, pc);
            if (item != NULL) {
                found = 1;
            } else if (!once) {
                item = func_800543BC(list);
                if (item != NULL) {
                    found = 1;
                } else if (list[1] < 0x20) {
                    item = (s32 *)(list + 2) + list[1] * 2;
                    list[1]++;
                    found = 1;
                }
            }
            if (found == 1) {
                item[1] = (s32)pc;
                item[0] = once;
            }
        }
        if (subop != 0x1E) {
            f3 = ((u16 *)cmd)[1];
            p = cmd;
            if (f3 & 1) {
                p++;
                m = *p;
                if ((((u32 *)obj)[0] & m) != m) goto fail;
                if ((f3 & 0x40) && (m & ((u32 *)obj)[1]) == m) goto fail;
            }
            if (f3 & 2) {
                p++;
                m = *p;
                if (((u32 *)obj)[0] & m) goto fail;
                if ((f3 & 0x80) && !(((u32 *)obj)[1] & m)) goto fail;
            }
            if (f3 & 4) {
                p++;
                m = *p;
                if ((m & ((u32 *)obj)[2]) != m) goto fail;
                if ((f3 & 0x100) && (m & ((u32 *)obj)[3]) == m) goto fail;
            }
            if (f3 & 8) {
                p++;
                m = *p;
                if (m & ((u32 *)obj)[2]) goto fail;
                if ((f3 & 0x100) && !(m & ((u32 *)obj)[3])) goto fail;
            }
            if (f3 & 0x10) {
                p++;
                m = *p;
                if ((m & ((u32 *)obj)[4]) != m) goto fail;
                if ((f3 & 0x400) && (m & ((u32 *)obj)[5]) == m) goto fail;
            }
            if (f3 & 0x20) {
                p++;
                m = *p;
                if (m & ((u32 *)obj)[4]) goto fail;
                if ((f3 & 0x400) && !(m & ((u32 *)obj)[5])) goto fail;
            }
            res = 1;
            goto done;
        fail:
            res = 0;
        done:
            if (res == 1) {
                n += func_800575FC((u8 *)sub, obj, arg2);
                goto end;
            }
        }
        obj->wait = 0;
        n += func_800575FC((u8 *)sub, NULL, arg2);
        obj->pc = cmd + n;
    } else {
        n += func_800575FC((u8 *)sub, obj, arg2);
    }
end:
    return n;
}

s32 func_80056D1C(u32 *cmd, u32 *obj, void *arg2) {
    register u32 w REG("v1");
    u16 f;
    s32 n;
    u32 *p;
    u32 m;
    s32 ok;
    u8 *next;
    u8 op;
    u32 *a;

    w = ((u16 *)cmd)[1];
    n = ((w & 1) + ((w >> 1) & 1) + ((w >> 2) & 1) + ((w >> 3) & 1) + ((w >> 4) & 1) + ((w >> 5) & 1)) + 1;
    next = (u8 *)(cmd + n);
    op = *next;
    if (obj != NULL) {
        if (op != 0x1E) {
            f = ((volatile u16 *)cmd)[1];
            p = cmd;
            if (f & 1) {
                m = *++p;
                if ((obj[0] & m) != m) goto fail;
                if (f & 0x40) {
                    if ((m & obj[1]) == m) goto fail;
                }
            }
            if (f & 2) {
                m = *++p;
                if (obj[0] & m) goto fail;
                if (f & 0x80) {
                    if (!(obj[1] & m)) goto fail;
                }
            }
            if (f & 4) {
                m = *++p;
                if ((m & obj[2]) != m) goto fail;
                if (f & 0x100) {
                    if ((m & obj[3]) == m) goto fail;
                }
            }
            if (f & 8) {
                m = *++p;
                if (m & obj[2]) goto fail;
                if (f & 0x100) {
                    if (!(m & obj[3])) goto fail;
                }
            }
            if (f & 0x10) {
                m = *++p;
                if ((m & obj[4]) != m) goto fail;
                if (f & 0x400) {
                    if ((m & obj[5]) == m) goto fail;
                }
            }
            if (f & 0x20) {
                m = *++p;
                if (m & obj[4]) goto fail;
                if (f & 0x400) {
                    if (!(m & obj[5])) goto fail;
                }
            }
            ok = 1;
            goto done;
        fail:
            ok = 0;
        done:
            if (ok == 1) {
                a = obj;
                goto call;
            }
        }
        obj[0x1C4 / 4] = 0;
        n += func_800575FC(next, NULL, arg2);
        ((u32 **)obj)[0xA4 / 4] = cmd + n;
    } else {
        a = NULL;
    call:
        n += func_800575FC(next, a, arg2);
    }
    return n;
}

s32 func_80056F90(u32 *cmd, ScriptObj *obj) {
    u32 w;
    u32 n;
    s32 len;
    u8 v;
    ScriptStack *s;
    u32 *ret;
    s32 ok;
    s32 c;

    w = *cmd;
    v = w >> 8;
    n = (w >> 16) & 0xFF;
    len = n + 1;
    if (obj != NULL) {
        v = obj->vars[v];
        if (v >= n) {
            obj->wait = 0;
            obj->pc = cmd + (n + 1);
        } else {
            ret = (u32 *)(n * 4 + (s32)cmd);
            ret++;
            s = (ScriptStack *)((u8 *)obj + 0x1B0);
            c = s->count;
            if (c < 4) {
                ok = 1;
                s->items[c] = ret;
                s->count = c + 1;
            } else {
                ok = 0;
            }
            if (ok == 1) {
                obj->wait = 0;
                obj->pc = cmd + (v + 1);
            } else {
                printf(D_800A8C08);
                obj->wait = 1;
            }
        }
    }
    return len;
}

s32 func_80057074(u32 *cmd, ScriptObj *obj) {
    /* op 0x03: return from gosub (pop pc from the call stack at +0x1B0; error + wait 1 if empty) */
    ScriptStack *s;
    s32 ok;
    s32 c;

    if (obj != NULL) {
        s = (ScriptStack *)((u8 *)obj + 0x1B0);
        c = s->count;
        if (c > 0) {
            c--;
            s->count = c;
            obj->pc = s->items[c];
            ok = 1;
        } else {
            ok = 0;
        }
        if (ok == 1) {
            obj->wait = 0;
        } else {
            printf(D_800A8C28);
            obj->wait = 1;
        }
    }
    return 1;
}

s32 func_800570F8(u32 *cmd, ScriptObj *obj) {
    s32 ok;
    u32 *next;
    s32 n;
    s16 off = ((u16 *)cmd)[1];
    s32 base = (s32)obj + 0x1B0;
    s32 *stk = (s32 *)base;
    if (obj != NULL) {
        n = stk[0];
        next = cmd + 1;
        if (n < 4) {
            ok = 1;
            *(u32 **)(base + (n << 2) + 4) = next;
            stk[0] = n + 1;
        } else {
            ok = 0;
        }
        if (ok == 1) {
            obj->wait = 0;
            obj->pc = (u32 *)((u8 *)cmd + off * 4);
        } else {
            printf(D_800A8C08, ok, next, stk);
            obj->wait = 1;
        }
    }
    return 1;
}

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

#ifdef NON_MATCHING
s32 func_800572F4(u32 *cmd, ScriptObj *obj, u8 *chr) {
    s32 ret;
    u32 v;
    s32 flag;
    u32 hi;

    ret = 1;
    v = *cmd;
    flag = 0;
    hi = v >> 16;
    v >>= 8;
    if (hi & 0xC) {
        ret = 2;
        flag = 2;
        g_Args->pad12E[0xA] = (u32)(((s32)cmd[1] >> 6) * 5) >> 2;
    }
    if (obj != NULL) {
        if ((hi & 3) == 1) {
            v = (u8)obj->vars[v & 0xFF];
        }
        obj->wait = 0;
        obj->pc = cmd + ret;
        func_80027248(v & 0xFF, *(s32 *)(chr + 0x90) + 0x14, flag);
    }
    return ret;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_800572F4);
#endif

s32 func_800573A4(u32 *cmd, ScriptObj *obj, u8 *chr) {
    /* op 0x19: func_80057BFC(model anim, byte 1) - set animation rate (0 or less -> 2); 1 word */
    s8 mode;
    void *anim;

    anim = *(void **)(chr + 0x94);
    mode = *cmd >> 8;
    if (obj != NULL) {
        if (mode < 0) {
            mode = 2;
        }
        func_80057BFC(anim, mode);
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return 1;
}

s32 func_80057410(u32 *cmd, ScriptObj *obj, void *arg) {
    u32 w = cmd[0];
    u32 t = w >> 8;
    if (obj != NULL) {
        (*(u8 **)((u8 *)arg + 0x94))[3] = t;
        obj->wait = 0;
        obj->pc = cmd + 1;
    }
    return 1;
}

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

s32 func_8005747C(u32 *cmd, ScriptObj *obj, u8 *chr) {
    u32 v;
    u8 *anim;
    u32 id;
    u32 endArg;
    s8 startArg;
    s32 start;
    s32 end;
    s32 tmp;
    s16 n;

    v = *cmd;
    anim = *(u8 **)(chr + 0x94);
    id = v >> 8;
    endArg = v >> 24;
    startArg = v >> 16;
    if (obj != NULL) {
        tmp = startArg;
        if (tmp == -1) {
            start = func_80057AB0(id & 0xFF);
        } else {
            start = tmp;
        }
        tmp = (s8)endArg;
        if (tmp == -1) {
            end = func_80057AB0(id & 0xFF);
        } else {
            end = tmp;
        }
        n = end - start + 1;
        if (n <= 0 && g_Args->cdArchive == 0) {
            printf(D_800A8C48, chr[0xA7], chr[6], obj->pc - (u32 *)obj->vars[0x20]);
        }
        obj->wait = n * *(s32 *)(anim + 0x20);
        obj->pc = cmd + 1;
        if ((s8)startArg == (s8)endArg) {
            *(s32 *)anim |= 2;
        } else {
            *(s32 *)anim &= ~2;
        }
        func_80057AD8(anim, id & 0xFF, start);
    }
    return 1;
}

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

#ifdef NON_MATCHING
void func_80057750(u32 *arg0, void *arg1) {
    u8 *tab;
    u32 cnt;
    u32 i;
    u32 *p;
    u32 m;
    u16 f;
    s32 ok;
    u32 w;
    u8 *cmd;
    u32 *q;
    s32 s0;
    s32 s1;
    tab = (u8 *)arg0 + 0xB0;
    cnt = arg0[0xAC / 4];
    i = 0;
    if (cnt != 0) {
        do {
            if (*(s32 *)tab == 0) {
                p = *(u32 **)(tab + 4);
                f = ((u16 *)p)[1];
                if (f & 1) {
                    p += 1;
                    m = *p;
                    if ((arg0[0] & m) != m) { ok = 0; goto done; }
                    if (f & 0x40) {
                        if ((m & arg0[1]) == m) { goto fail; }
                    }
                }
                if (f & 2) {
                    p += 1;
                    m = *p;
                    if (arg0[0] & m) { ok = 0; goto done; }
                    if (f & 0x80) {
                        if (!(arg0[1] & m)) { ok = 0; goto done; }
                    }
                }
                if (f & 4) {
                    p += 1;
                    m = *p;
                    if ((m & arg0[2]) != m) { ok = 0; goto done; }
                    if (f & 0x100) {
                        if ((m & arg0[3]) == m) { ok = 0; goto done; }
                    }
                }
                if (f & 8) {
                    p += 1;
                    m = *p;
                    if (m & arg0[2]) { ok = 0; goto done; }
                    if (f & 0x100) {
                        if (!(m & arg0[3])) { ok = 0; goto done; }
                    }
                }
                if (f & 0x10) {
                    p += 1;
                    m = *p;
                    if ((m & arg0[4]) != m) { ok = 0; goto done; }
                    if (f & 0x400) {
                        if ((m & arg0[5]) == m) { ok = 0; goto done; }
                    }
                }
                if (f & 0x20) {
                    m = p[1];
                    if (m & arg0[4]) {
                        ok = 0;
                    } else if (!(f & 0x400)) {
                        ok = 1;
                    } else if (m & arg0[5]) {
                        ok = 1;
                    } else {
                    fail:
                        ok = 0;
                    }
                } else {
                    ok = 1;
                }
            done:
                if (ok != 0) {
                    q = *(u32 **)(tab + 4);
                    w = ((u16 *)q)[1];
                    cmd = (u8 *)q + (((w & 1) + ((w >> 1) & 1) + ((w >> 2) & 1) + ((w >> 3) & 1) + ((w >> 4) & 1) + ((w >> 5) & 1)) * 4 + 4);
                    if (*cmd < 4U) {
                        func_800575FC(cmd, arg0, arg1);
                        return;
                    }
                    s0 = arg0[0xA4 / 4];
                    s1 = arg0[0x1C4 / 4];
                    func_800575FC(cmd, arg0, arg1);
                    arg0[0xA4 / 4] = s0;
                    arg0[0x1C4 / 4] = s1;
                }
            }
            i++;
            tab += 8;
        } while (i < cnt);
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80057750);
#endif

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

void func_80057C40(void) {
    s16 type;
    u8 done;
    u8 *p;
    u8 *q;
    s16 count;
    s16 i;

    done = 0;
    p = NULL;
    q = NULL;
    func_80061F38();
    func_80061FB4();
    type = 0;
    do {
        switch (type++) {
        case 0:
            count = 0;
            p = (u8 *)&D_800AB540;
            break;
        case 1:
            count = 30;
            p = D_800DB884;
            break;
        case 2:
            count = 30;
            p = D_800DA5C4;
            break;
        default:
            count = 0;
            done = 1;
            break;
        }
        for (i = 0; i < count; i++, p += 0xA0) {
            p[0x9B] = 0;
            func_80081C84(p);
            func_80081C84(p + 0x28);
        }
    } while (!done);
    type = 0;
    done = 0;
    do {
        switch (type++) {
        case 0:
            count = 0;
            q = (u8 *)&D_800AB540;
            break;
        case 1:
            count = 0;
            q = D_800AB540_b;
            break;
        case 2:
            count = 30;
            q = D_800DCB4C;
            break;
        case 3:
            count = 0;
            q = D_800AB540_c;
            break;
        default:
            count = 0;
            done = 1;
            break;
        }
        for (i = 0; i < count; i++, q += 0x6C) {
            q[0x58] = 0;
            func_8008C5A4(q);
            func_8008C5A4(q + 0x14);
            q[0x59] = 0x80;
            q[0x5A] = 0x80;
            q[0x5B] = 0x80;
            q[0x5C] = 0x80;
            q[0x5D] = 0x80;
            q[0x5E] = 0x80;
            func_80081CE4(q, 0);
            func_80081CE4(q + 0x14, 0);
        }
    } while (!done);
}

#ifdef NON_MATCHING
s32 func_80057E80(s32 arg0) {
    s16 v[4][4];        /* screen-space (x, y) of up to four corners, 8 bytes apart */
    u8 flag30;
    u8 flag31;
    u8 nActive;
    s32 db;
    s32 off1;
    s32 off2;
    s16 dispW;
    s32 nTris;
    s32 nQuadsA;
    s32 nQuadsB;
    s32 nQuadsC;
    s32 sec;
    s32 n;
    s32 k;
    s32 inside;
    s32 oi;
    u8 *base;
    u8 *rec;
    u8 *fl;
    u8 *prim;
    u32 *ot;
    u16 depth;
    u8 *ent;

    nTris = 0;
    nQuadsA = 0;
    nQuadsB = 0;
    nQuadsC = 0;
    flag30 = D_8009CABD;
    flag31 = D_8009CABE;
    nActive = 0;
    db = D_800B8F24;
    off1 = db * 0x28;
    dispW = g_Args->displayW;
    sec = 0;
    base = D_800AB4A0;
    do {
        sec++;
        n = 0;
        if (sec - 1 == 1) {
            n = 0x1E;
            base = D_800DCAA4;
        } else if (sec - 1 == 2) {
            n = 0x1E;
            base = D_800DB7E4;
        } else if (sec - 1 != 0) {
            break;
        }
        for (k = 0; k < n; k++) {
            rec = base - k * 0xA0;
            fl = rec + 0x9B;
            if (*fl == 0) {
                continue;
            }
            depth = M2C_FIELD(rec, u16 *, 0x82);
            *(s32 *) &v[0][0] = M2C_FIELD(rec, s32 *, 0x84);
            *(s32 *) &v[1][0] = M2C_FIELD(rec, s32 *, 0x88);
            *(s32 *) &v[2][0] = M2C_FIELD(rec, s32 *, 0x8C);
            *(s32 *) &v[3][0] = M2C_FIELD(rec, s32 *, 0x90);
            nActive++;
            prim = rec + off1;
            if ((s16) depth <= 0) {
                continue;
            }
            /* x extent of the first edge against 0..0x200 */
            if (v[0][0] < v[1][0]) {
                inside = (v[1][0] >= 0) && (v[0][0] < 0x201);
            } else {
                inside = (v[0][0] >= 0) && (v[1][0] < 0x201);
            }
            if (!inside) {
                continue;
            }
            /* y extent of the 0-2 diagonal against 0..0xF0 */
            if (v[0][1] < v[2][1]) {
                inside = (v[0][1] <= 0xF0) && (v[2][1] >= 0);
            } else {
                inside = (v[2][1] <= 0xF0) && (v[0][1] >= 0);
            }
            if (!inside) {
                continue;
            }
            oi = (s16) depth >> 2;
            if (!(((*fl & 0xF0) != 0x80)
                  || (((v[0][0] - v[2][0]) * (v[3][1] - v[2][1])) >= ((v[3][0] - v[2][0]) * (v[0][1] - v[2][1])))
                  || !(((v[3][0] - v[1][0]) * (v[0][1] - v[1][1])) < ((v[0][0] - v[1][0]) * (v[3][1] - v[1][1]))))) {
                continue;
            }
            ent = M2C_FIELD(fl, u8 **, -0x27);
            if ((ent != NULL) && (ent[0xA6] == 0)) {
                continue;
            }
            M2C_FIELD(prim, s16 *, 8) = v[0][0];
            M2C_FIELD(prim, s16 *, 0xA) = v[0][1];
            M2C_FIELD(prim, s16 *, 0x10) = v[1][0];
            M2C_FIELD(prim, s16 *, 0x12) = v[1][1];
            M2C_FIELD(prim, s16 *, 0x18) = v[2][0];
            M2C_FIELD(prim, s16 *, 0x1A) = v[2][1];
            M2C_FIELD(prim, s16 *, 0x20) = v[3][0];
            M2C_FIELD(prim, s16 *, 0x22) = v[3][1];
            if (flag30 != 0) {
                prim[4] = 0;
                prim[6] = 0;
                prim[5] = fl[-2];
            } else if (flag31 != 0) {
                prim[4] = 0;
                prim[5] = 0;
                prim[6] = 0;
            } else {
                prim[4] = fl[-3];
                prim[5] = fl[-2];
                prim[6] = fl[-1];
            }
            if (oi & 0x8000) {
                oi = 0;
            }
            ot = (u32 *) (arg0 + (s16) oi * 4);
            ((PTag *) prim)->addr = ((PTag *) ot)->addr;
            ((PTag *) ot)->addr = (u32) prim;
            if (*fl & 4) {
                nQuadsC++;
            } else {
                nQuadsB++;
            }
        }
    } while (1);
    off2 = db * 0x14;
    sec = 0;
    do {
        sec++;
        n = 0;
        base = (u8 *) &D_800AB540;
        if (sec - 1 == 2) {
            n = 0x1E;
            base = D_800DCB4C;
        } else if (sec - 1 > 3) {
            break;
        }
        for (k = 0; k < n; k++) {
            rec = base + k * 0x6C;
            fl = rec + 0x58;
            if (*fl == 0) {
                continue;
            }
            nQuadsA++;
            depth = M2C_FIELD(rec, u16 *, 0x4A);
            *(s32 *) &v[0][0] = M2C_FIELD(rec, s32 *, 0x4C);
            *(s32 *) &v[1][0] = M2C_FIELD(rec, s32 *, 0x50);
            prim = base + off2 + k * 0x6C;
            if ((s16) depth <= 0) {
                continue;
            }
            if ((v[0][0] < 0) || (v[1][0] < 0) || (dispW < v[0][0]) || (dispW < v[1][0])
                || (v[0][1] < 0) || (v[1][1] < 0) || (v[0][1] > 0xF0) || (v[1][1] > 0xF0)) {
                continue;
            }
            if ((v[1][0] == v[0][0]) && (v[1][1] == v[0][1])) {
                v[1][0] = v[0][0] + 1;
                v[1][1] = v[0][1];
            }
            M2C_FIELD(prim, u16 *, 8) = v[0][0];
            M2C_FIELD(prim, u16 *, 0xA) = v[0][1];
            M2C_FIELD(prim, u16 *, 0x10) = v[1][0];
            M2C_FIELD(prim, u16 *, 0x12) = v[1][1];
            if (flag30 != 0) {
                prim[4] = 0;
                prim[5] = fl[2];
                prim[6] = 0;
                prim[0xC] = 0;
                prim[0xD] = fl[5];
                prim[0xE] = 0;
            } else if (flag31 != 0) {
                prim[4] = 0;
                prim[5] = 0;
                prim[6] = 0;
                prim[0xC] = 0;
                prim[0xD] = 0;
                prim[0xE] = 0;
            } else {
                prim[4] = fl[1];
                prim[5] = fl[2];
                prim[6] = fl[3];
                prim[0xC] = fl[4];
                prim[0xD] = fl[5];
                prim[0xE] = fl[6];
            }
            if ((*fl & 0xF0) == 0x60) {
                ot = (u32 *) (arg0 + 0x3E80);
                ((PTag *) prim)->addr = ((PTag *) ot)->addr;
                ((PTag *) ot)->addr = (u32) prim;
            } else if ((*fl & 0xF0) == 0x70) {
                if (*fl & 8) {
                    ot = (u32 *) arg0;
                    ((PTag *) prim)->addr = ((PTag *) ot)->addr;
                    ((PTag *) ot)->addr = (u32) prim;
                }
            } else {
                ot = (u32 *) (arg0 + ((s16) depth >> 2) * 4);
                ((PTag *) prim)->addr = ((PTag *) ot)->addr;
                ((PTag *) ot)->addr = (u32) prim;
            }
            nTris++;
        }
    } while (1);
    D_8009D8C4 = nActive;
    D_8009D8C8 = nQuadsA & 0xFF;
    D_8009D8BE = nTris & 0xFF;
    D_8009D8CC = nQuadsB & 0xFF;
    *(s32 *) D_800DD7F4 = nQuadsC & 0xFF;
    return nQuadsC & 0xFF;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80057E80);
#endif

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

void func_8005885C(u8 *p, u8 type) {
    u8 *t;
    s16 i;
    u8 *prim;
    u8 f;

    M2C_FIELD(p, u8 **, 0x50) = (u8 *)&((FxTmpl *)D_8009D8B8)[type];
    t = M2C_FIELD(p, u8 **, 0x50);
    M2C_FIELD(p, u8 *, 0x9B) = M2C_FIELD(t, u8 *, 4) & 0xF0;
    M2C_FIELD(p, s16 *, 0x5A) = 0;
    M2C_FIELD(p, s32 *, 0x94) = D_8009CDA8;
    M2C_FIELD(p, u16 *, 0x54) = *(u16 *)&D_1F8003D4;
    M2C_FIELD(p, u16 *, 0x56) = *(u16 *)&D_1F8003D8;
    M2C_FIELD(p, u16 *, 0x58) = *(u16 *)&D_1F8003DC;
    M2C_FIELD(p, u16 *, 0x78) = M2C_FIELD(t, u16 *, 0);
    M2C_FIELD(p, u16 *, 0x7A) = M2C_FIELD(t, u16 *, 2);
    M2C_FIELD(p, s16 *, 0x9C) = 0;
    M2C_FIELD(p, u8 *, 0x98) = M2C_FIELD(t, u8 *, 5);
    M2C_FIELD(p, u8 *, 0x99) = M2C_FIELD(t, u8 *, 5);
    M2C_FIELD(p, u8 *, 0x9A) = M2C_FIELD(t, u8 *, 5);
    M2C_FIELD(p, s32 *, 0x74) = 0;
    for (i = 0; i < 2; i++) {
        prim = p + i * 0x28;
        f = M2C_FIELD(t, u8 *, 4);
        func_80081CE4(prim, f & 1);
        M2C_FIELD(prim, u16 *, 0x16) = GetTPage(0, (f & 6) >> 1, M2C_FIELD(t, s16 *, 0x10), M2C_FIELD(t, s16 *, 0x12));
        M2C_FIELD(prim, u8 *, 0xC) = M2C_FIELD(t, u8 *, 8);
        M2C_FIELD(prim, u8 *, 0xD) = M2C_FIELD(t, u8 *, 9);
        M2C_FIELD(prim, u8 *, 0x14) = M2C_FIELD(t, u8 *, 8) + M2C_FIELD(t, u8 *, 0xA);
        M2C_FIELD(prim, u8 *, 0x15) = M2C_FIELD(t, u8 *, 9);
        M2C_FIELD(prim, u8 *, 0x1C) = M2C_FIELD(t, u8 *, 8);
        M2C_FIELD(prim, u8 *, 0x1D) = M2C_FIELD(t, u8 *, 9) + M2C_FIELD(t, u8 *, 0xB);
        M2C_FIELD(prim, u8 *, 0x24) = M2C_FIELD(t, u8 *, 8) + M2C_FIELD(t, u8 *, 0xA);
        M2C_FIELD(prim, u8 *, 0x25) = M2C_FIELD(t, u8 *, 9) + M2C_FIELD(t, u8 *, 0xB);
        M2C_FIELD(prim, u16 *, 0xE) = (M2C_FIELD(t, u16 *, 0xE) << 6) | ((M2C_FIELD(t, u16 *, 0xC) >> 4) & 0x3F);
        M2C_FIELD(prim, u8 *, 4) = M2C_FIELD(t, u8 *, 5);
        M2C_FIELD(prim, u8 *, 5) = M2C_FIELD(t, u8 *, 6);
        M2C_FIELD(prim, u8 *, 6) = M2C_FIELD(t, u8 *, 7);
    }
}

void *func_80058A58(s32 type) {
    s16 i;
    u8 *p;
    u8 *best;
    u32 min;
    u8 *pool;

    best = NULL;
    pool = D_800DA5C4;
    i = 0;
    min = D_8009CDA8;
    for (; i < 30; i++) {
        p = (u8 *)(i * 0xA0 + (u32)pool);
        if ((p[0x9B] & 0xF0) == 0x80 && *(u32 *)(p + 0x94) < min) {
            min = *(u32 *)(p + 0x94);
            best = p;
        }
    }
    if (best == NULL) {
        return NULL;
    }
    func_8005885C(best, type);
    return best;
}

void func_80058B10(s32 owner) {
    s16 i;
    u8 *p;

    p = D_800DA5C4;
    for (i = 0; i < 30; i++, p += 0xA0) {
        if ((p[0x9B] & 0xF0) == 0x80 && *(s32 *)(p + 0x74) == owner) {
            p[0x9B] = 0;
        }
    }
}

u8 *func_80058B6C(u8 type) {
    s16 i;
    u8 *p;
    s16 count;

    p = NULL;
    switch (type) {
    case 0x40:
        count = 0;
        p = (u8 *)&D_800AB540;
        break;
    case 0x50:
        count = 30;
        p = D_800DCB4C;
        break;
    case 0x70:
        count = 0;
        p = D_800AB540_c;
        break;
    case 0x60:
        count = 0;
        p = D_800AB540_b;
        break;
    default:
        count = 0;
        sprintf(g_Args->exePath, D_800A8D14, type);
        func_8006B230(0, (s32)g_Args->exePath);
        break;
    }
    for (i = 0; i < count; i++, p += 0x6C) {
        if (p[0x58] == 0) {
            goto found;
        }
    }
    return NULL;
found:
    M2C_FIELD(p, u8 *, 0x58) = type;
    M2C_FIELD(p, s16 *, 0x32) = 0;
    M2C_FIELD(p, u16 *, 0x2C) = *(u16 *)&D_1F8003D4;
    M2C_FIELD(p, u16 *, 0x2E) = *(u16 *)&D_1F8003D8;
    M2C_FIELD(p, u16 *, 0x30) = *(u16 *)&D_1F8003DC;
    M2C_FIELD(p, s32 *, 0x54) = D_8009CDA8;
    if ((M2C_FIELD(p, u8 *, 0x58) & 0xF0) == 0x40) {
        M2C_FIELD(p, s16 *, 0x3C) = 200;
        M2C_FIELD(p, u8 *, 0x59) = 0x80;
        M2C_FIELD(p, u8 *, 0x5A) = 0x80;
        M2C_FIELD(p, u8 *, 0x5B) = 0x80;
        M2C_FIELD(p, u8 *, 0x5C) = 0xFF;
        M2C_FIELD(p, u8 *, 0x5D) = 0xFF;
        M2C_FIELD(p, u8 *, 0x5E) = 0xFF;
    }
    return p;
}

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

void func_80058DFC(void *arg0) {
    TileGrid *g;
    s16 j;
    s16 i;
    TILE *p;

    g = arg0;
    if (g == NULL) {
        g = (TileGrid *)D_800DD83C;
    }
    for (j = 0; j < 12; j++) {
        for (i = 0; i < 2; i++) {
            p = &g->t[j][i];
            func_80081B74(p);
            p->r0 = 0x10;
            p->g0 = 0xF0;
            p->b0 = 0x10;
            func_80081CE4(p, 0);
        }
    }
    for (i = 0; i < 2; i++) {
        func_80081B74(&D_800DD818[i]);
        D_800DD818[i].r0 = 0;
        D_800DD818[i].g0 = 0;
        D_800DD818[i].b0 = 0xFF;
        func_80081CE4(&D_800DD818[i], 0);
    }
}

#ifdef NON_MATCHING
void func_80058F28(u8 *arg0) {
    u16 bs[4];
    u16 in[4];
    u16 out[4];
    u16 x0, x1, y0, y1, z0, z1;
    u8 *ent;
    u8 *a3;
    u8 *m;
    u8 *q;
    s16 sc;
    u32 p0, p1, p2, p3, p4, p5;

    a3 = *(u8 **)(arg0 + 0x90);
    ent = a3 + D_8009D8D6 * 0x8C;
    if (arg0[0xB] >= 2) a3 += 0x8C;
    *(s32 *)D_800DD83C = (s16)*(u16 *)(arg0 + 0x14) >> 2;
    bs[0] = *(u16 *)(ent + 0x48) + *(u16 *)&D_8009CA7C;
    bs[1] = *(u16 *)(ent + 0x4C) + *(u16 *)&D_8009CA80;
    bs[2] = *(u16 *)(ent + 0x50) + *(u16 *)&D_8009CA84;
    if (arg0[5] == 6) {
        x0 = *(u16 *)(arg0 + 0x1C);
        x1 = *(u16 *)(arg0 + 0x1E);
        y0 = *(u16 *)(arg0 + 0x20);
        y1 = *(u16 *)(arg0 + 0x22);
        z0 = *(u16 *)(arg0 + 0x24);
        z1 = *(u16 *)(arg0 + 0x26);
        if (D_8009D8D6 == 0) {
            bs[0] += *(u16 *)(a3 + 0x28);
            bs[1] += *(u16 *)(a3 + 0x2C);
            bs[2] += *(u16 *)(a3 + 0x30);
        }
    } else if (D_8009D8D6 == 0) {
        x0 = *(u16 *)(arg0 + 0x1C);
        x1 = *(u16 *)(arg0 + 0x1E);
        y0 = *(u16 *)(arg0 + 0x20);
        y1 = *(u16 *)(arg0 + 0x22);
        z0 = *(u16 *)(arg0 + 0x24);
        z1 = *(u16 *)(arg0 + 0x26);
    } else {
        q = (u8 *)D_800D86E0 + (*(u8 **)arg0)[0x5F] * 0x7E + D_8009D8D6 * 2;
        x0 = *(u16 *)(q + 0x2A);
        x1 = *(u16 *)(q + 0x36);
        y0 = *(u16 *)(q + 0x42);
        y1 = *(u16 *)(q + 0x4E);
        z0 = *(u16 *)(q + 0x5A);
        z1 = *(u16 *)(q + 0x66);
    }
    if (D_8009D8D6 != 0) {
        sc = *(s16 *)(arg0 + 0x6A);
        if (sc != 0) {
            p0 = (s16)x0 * sc;
            p1 = (s16)x1 * sc;
            p2 = (s16)y0 * sc;
            p3 = (s16)y1 * sc;
            p4 = (s16)z0 * sc;
            p5 = (s16)z1 * sc;
            x0 = p0 >> 12;
            x1 = p1 >> 12;
            y0 = p2 >> 12;
            y1 = p3 >> 12;
            z0 = p4 >> 12;
            z1 = p5 >> 12;
        }
    }
    m = ent + 0x34;
    CORNER(0, x0, y0, z0)
    CORNER(1, x1, y0, z0)
    CORNER(2, x1, y0, z1)
    CORNER(3, x0, y0, z1)
    CORNER(4, x0, y1, z0)
    CORNER(5, x1, y1, z0)
    CORNER(6, x1, y1, z1)
    CORNER(7, x0, y1, z1)
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80058F28);
#endif

void func_80059634(u8 *arg0) {
    SVECTOR pos;
    SVECTOR vv;
    SVECTOR r;
    MATRIX mat;
    u8 *p;
    u16 x0, x1, y0, y1, z0, z1;
    s32 one;
    func_8006B230(arg0 != 0, D_800A8D34);
    mat = *(MATRIX *)&D_800FC6A0;
    RotMatrixY(*(s16 *)(arg0 + 0x22), &mat);
    *(s32 *)&D_800DDA40 = 10;
    pos.vx = *(u16 *)(arg0 + 0x10);
    pos.vy = *(u16 *)(arg0 + 0x14);
    pos.vz = *(u16 *)(arg0 + 0x18);
    x0 = *(u16 *)(arg0 + 4);
    x1 = *(u16 *)(arg0 + 6);
    y0 = *(u16 *)(arg0 + 8);
    y1 = *(u16 *)(arg0 + 0xA);
    z0 = *(u16 *)(arg0 + 0xC);
    z1 = *(u16 *)(arg0 + 0xE);
    p = &D_800DDA40;
    vv.vx = x0;
    vv.vy = y0;
    vv.vz = z0;
    XFORM();
    vv.vx = x1; vv.vy = y0; vv.vz = z0;
    OUT(0);
    XFORM();
    one = 1;
    vv.vx = x1; vv.vy = y0; vv.vz = z1;
    OUT(1);
    XFORM();
    vv.vx = x0; vv.vy = y0; vv.vz = z1;
    OUT(2);
    XFORM();
    vv.vx = x0; vv.vy = y1; vv.vz = z0;
    OUT(3);
    XFORM();
    vv.vx = x1; vv.vy = y1; vv.vz = z0;
    OUT(4);
    XFORM();
    vv.vx = x1; vv.vy = y1; vv.vz = z1;
    OUT(5);
    XFORM();
    vv.vx = x0; vv.vy = y1; vv.vz = z1;
    OUT(6);
    XFORM();
    OUT(7);
    if (arg0[1] == 0 || arg0[3] == one) {
        D_8009D8DD = one;
    } else {
        D_8009D8DD = 0;
    }
}

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

#ifdef NON_MATCHING
s32 func_80059E4C(s32 *arg0) {
    u8 *obj;
    u8 *prim;
    s16 *pa;
    s16 *pb;
    u32 *ot;
    u32 w;
    u32 ret;
    s32 buf;
    s32 k;
    s32 a, b, s1, s2;

    obj = (u8 *)arg0;
    if (obj == NULL) {
        obj = D_800DD83C;
    }
    buf = (1 - *(u8 *)&D_800B8F24) & 0xFF;
    for (k = 0; k < 12; k++) {
        /* wireframe cube: edge k joins corners a and b; highlighted when the selection is s1 or s2 */
        switch (k) {
        case 0: a = 0; b = 1; s1 = 1; s2 = 4; break;
        case 1: a = 1; b = 2; s1 = 2; s2 = 4; break;
        case 2: a = 2; b = 3; s1 = 0; s2 = 4; break;
        case 3: a = 3; b = 0; s1 = 3; s2 = 4; break;
        case 4: a = 4; b = 5; s1 = 1; s2 = 5; break;
        case 5: a = 5; b = 6; s1 = 2; s2 = 5; break;
        case 6: a = 6; b = 7; s1 = 0; s2 = 5; break;
        case 7: a = 7; b = 4; s1 = 3; s2 = 5; break;
        case 8: a = 0; b = 4; s1 = 1; s2 = 3; break;
        case 9: a = 1; b = 5; s1 = 1; s2 = 2; break;
        case 10: a = 2; b = 6; s1 = 0; s2 = 2; break;
        default: a = 3; b = 7; s1 = 0; s2 = 3; break;
        }
        prim = obj + 4 + buf * 0x10 + k * 0x20;
        pa = (s16 *)(obj + 0x1C4 + a * 8);
        pb = (s16 *)(obj + 0x1C4 + b * 8);
        *(u16 *)(prim + 8) = pa[0];
        *(u16 *)(prim + 0xA) = pa[1];
        *(u16 *)(prim + 0xC) = pb[0];
        *(u16 *)(prim + 0xE) = pb[1];
        if (D_8009D8D4 == s1 || D_8009D8D4 == s2 || D_8009D8DC == 1) {
            prim[4] = 0xFF;
            prim[5] = 0;
            prim[6] = 0;
        } else if (D_8009D8DC == 2) {
            prim[4] = 0;
            prim[5] = 0xFF;
            prim[6] = 0xFF;
        } else if (D_8009D8DD != 0) {
            prim[4] = 0;
            prim[5] = 0;
            prim[6] = 0xFF;
        } else {
            prim[4] = 0x10;
            prim[5] = 0xF0;
            prim[6] = 0x10;
        }
        ot = (u32 *)D_800D3234 + ((((pa[2] + pb[2]) >> 3) - 0xA) & 0xFFF);
        *(u32 *)prim = (*(u32 *)prim & 0xFF000000) | (*ot & 0xFFFFFF);
        w = (*ot & 0xFF000000) | ((u32)prim & 0xFFFFFF);
        *ot = w;
        ret = w;
    }
    return ret;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_80059E4C);
#endif

s32 func_8005AD6C(arg0)
s32 arg0; {
    return ((s32 (*)(s32, s32))ratan2)(arg0, SquareRoot12(0x1000 - ((s32) (arg0 * arg0) >> 0xC)));
}

s32 func_8005ADB0(void) {
    return 0x400 - func_8005AD6C();
}

#ifdef NON_MATCHING
void func_8005ADD4(void *arg0, s32 n, s32 *out) {
    register s16 *t REG("t1");
    s16 *u;
    register s32 j REG("a3");
    s32 k;
    t = *(s16 **)((u8 *)arg0 + 0x18);
    out[0] = t[0];
    out[1] = -t[2];
    out[2] = t[1];
    if (n != 0) {
        if (n < 7) {
            j = 4;
            if (n < j) {
                j = 1;
            }
        } else {
            out[0] += t[0x2A];
            out[1] -= t[0x2C];
            out[2] += t[0x2B];
            j = 8;
            if (n >= 11) {
                j = 14;
                if (n < j) {
                    j = 11;
                }
            }
        }
        t = t + j * 6;
        if (n >= j) {
            u = t + 1;
            do {
                k = j;
                j++;
                out[0] += *t;
                t += 6;
                out[1] -= u[1];
                out[2] += u[0];
                u += 6;
            } while (n >= k);
        }
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005ADD4);
#endif

#ifdef NON_MATCHING
void func_8005AEF8(void) {
    s32 i;
    s32 n;
    s32 k;
    s32 a;
    u8 *e;
    u8 *o;
    u8 *src;
    s32 *s;
    s32 *d;

    if (D_8009D8E4 == -1) return;
    a = D_8009D924 + D_8009D928;
    D_8009D924 = a;
    if (a == 0x1000 || a == 0) D_8009D928 = -D_8009D928;
    TransposeMatrix(D_800B0018, D_8009D91C);
    D_8009D918[0] = -*(u16 *)((u8 *)D_800B0018 + 0x14);
    D_8009D918[1] = -*(u16 *)((u8 *)D_800B0018 + 0x18);
    D_8009D918[2] = -*(u16 *)((u8 *)D_800B0018 + 0x1C);
    ApplyMatrix(D_8009D91C, D_8009D918, (u8 *)D_8009D91C + 0x14);
    gte_SetRotMatrix(D_8009D91C);
    gte_SetTransMatrix(D_8009D91C);
    s = D_8009D91C + 8;
    d = D_800DDE10 + 8;
    for (i = 8; i != 0; i--) *d-- = *s--;
    D_8009D8E8 = 0;
    for (i = 0; i < 6; i++) {
        u8 f = D_800DE0C8[i];
        if (f == 1 && !(D_8009D8E4 & (f << i)) && *(s32 *)(D_800DDEF0[i] + 0x7C) >= 0) {
            ((s32 *)&D_800DDF08)[D_8009D8E8] = ((s32 *)&D_800DDFEC)[i];
            *(GsMat32 *)(D_800DDCCC + (D_8009D8E8 << 5)) = *(GsMat32 *)(D_800DE004 + i * 0x20);
            n = D_8009D8E8;
            D_8009D8E8 += 1;
            D_800DDDEC[n] = D_800DDEF0[i];
        }
    }
    e = D_800DDCCC;
    for (i = 0; i < D_8009D8E8; i++) {
        o = (u8 *)&D_800DDF24 + i * 0x20;
        D_8009D918[0] = *(u16 *)(e + 0x14);
        D_8009D918[1] = *(u16 *)(e + 0x18);
        D_8009D918[2] = *(u16 *)(e + 0x1C);
        gte_ldv0(D_8009D918);
        gte_rtv0tr();
        gte_stlvnl(o + 0x14);
        gte_ldclmv(e);
        gte_mvmva_rt_ir();
        gte_stclmv(D_8009D920);
        gte_ldclmv(e + 2);
        gte_mvmva_rt_ir();
        gte_stclmv(D_8009D920 + 1);
        gte_ldclmv(e + 4);
        gte_mvmva_rt_ir();
        gte_stclmv(D_8009D920 + 2);
        ((u16 *)o)[0] = D_8009D920[0];
        ((u16 *)o)[1] = D_8009D920[2];
        ((u16 *)o)[2] = -D_8009D920[1];
        ((u16 *)o)[3] = D_8009D920[3];
        ((u16 *)o)[4] = D_8009D920[5];
        ((u16 *)o)[5] = -D_8009D920[4];
        ((u16 *)o)[6] = D_8009D920[6];
        ((u16 *)o)[7] = D_8009D920[8];
        ((u16 *)o)[8] = -D_8009D920[7];
        e += 0x20;
    }
    func_8005C33C();
    func_80033F38();
    n = D_8009D8E8;
    k = 6553;
    for (i = 0; i < n; i++) {
        s32 w, x, y, z;
        o = (u8 *)&D_800DDF24 + i * 0x20;
        w = (*(s32 *)(o + 0x18) << 12) / k;
        x = (*(s16 *)(o + 6) << 12) / k;
        y = (*(s16 *)(o + 8) << 12) / k;
        z = (*(s16 *)(o + 10) << 12) / k;
        *(s32 *)(o + 0x18) = w;
        *(s16 *)(o + 6) = x;
        *(s16 *)(o + 8) = y;
        *(s16 *)(o + 10) = z;
    }
    func_800AACFC(&D_800DDF08, &D_800DDF24, D_8009D8E8, D_800B8F88[D_800B8F24].org);
    if (D_8009D8F0 > 0) {
        func_8004A398();
        D_8009D8F0--;
        if (D_8009D8F0 <= 0) D_800DDDA8 = 0;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005AEF8);
#endif

void func_8005B508(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        func_8007CE30((&D_800DDFEC)[i], M2C_FIELD(&D_800D2E84 + i * 0x20, u8 *, 1));
        D_800DE0C8[i] = 0;
    }
}

s32 func_8005B584(s32 a0, u16 v, s16 i) {
    return D_800D2E84_r[i].v == v;
}

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

void func_8005B6CC(u8 *obj) {
    s32 m;

    *(s32 *)(obj + 0x48) &= ~0x14;
    m = *(s32 *)(obj + 0xC0);
    if (D_8009CABD != 0) {
        func_8007D184(m, 0xFF, 0, 0);
    } else if (D_8009CABE != 0) {
        func_8007D184(m, 0x1E, 0x1E, 0x64);
    } else {
        func_8007D184(m, D_8009D8F4, D_8009D8F8, D_8009D8FC);
    }
    func_8007D260(m);
}

void func_8005B76C(u8 *obj) {
    M2C_FIELD(obj, s32 *, 0x48) |= 4;
    if (D_8009CABD != 0) {
        func_8007D184(M2C_FIELD(obj, s32 *, 0xC0), 0x7F, 0, 0);
    } else {
        func_8007D184(M2C_FIELD(obj, s32 *, 0xC0), 0, 0, 0);
    }
}

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

void func_8005B86C(void) {
    u8 *o;
    s32 i;
    s32 a;
    i = D_800B0DA0;
    o = (u8 *)D_800AFFC8 + i * 0x124;
    if (D_800B07B0 >= i) {
        do {
            if (o[4] != 0 && (a = M2C_FIELD(o, s32 *, 0xC0), a != 0)) {
                if (M2C_FIELD(M2C_FIELD(o, void **, 0x94), s32 *, 0x24) & 0x20000) {
                    func_8005B7BC(o, a);
                } else if (M2C_FIELD(o, s32 *, 0x48) & 4) {
                    func_8005B76C(o);
                } else {
                    func_8005B6CC(o);
                }
            }
            i++;
            o += 0x124;
        } while (D_800B07B0 >= i);
    }
}

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

void func_8005BA30(void *obj) {
    u8 *c;
    s32 *dst;

    c = *(u8 **)((u8 *)obj + 0x90);
    dst = (s32 *)&D_800DE004[(*(u8 **)((u8 *)obj + 0x94))[0xA] << 5];
    *(s32 *)(c + 0x10) = 0;
    GsGetLs(c + 0x10, &D_800B0848);
    *(s32 *)(c + 0x10) = 2;
    *dst++ = *(s32 *)(c + 0x34);
    *dst++ = *(s32 *)(c + 0x38);
    *dst++ = *(s32 *)(c + 0x3C);
    *dst++ = *(s32 *)(c + 0x40);
    *dst++ = *(s32 *)(c + 0x44);
    *dst++ = *(s32 *)(c + 0x48);
    *dst++ = *(s32 *)(c + 0x4C);
    *dst++ = *(s32 *)(c + 0x50);
}

void func_8005BAE8(u8 *obj, u8 *anim) {
    s16 *p;
    s16 x1, y1, z1, x2, y2, z2;
    s32 dx, dz;
    s16 f;

    p = ((s16 **)D_800DDEE0)[M2C_FIELD(anim, s16 *, 0x10)];
    p += anim[0x14] * 3;
    x1 = *p;
    p++;
    y1 = *p;
    p++;
    z1 = *p;
    p = ((s16 **)D_800DDEE0)[M2C_FIELD(anim, s16 *, 0x12)];
    p += anim[0x16] * 3;
    x2 = *p;
    p++;
    y2 = *p;
    p++;
    z2 = *p;
    if (M2C_FIELD(anim, s32 *, 0) & 2) {
        M2C_FIELD(obj, s16 *, 0x98) = 0;
        M2C_FIELD(obj, s16 *, 0x9C) = 0;
    } else if (anim[3] == 0) {
        if (M2C_FIELD(anim, s16 *, 0x18) != 0) {
            f = M2C_FIELD(anim, s16 *, 0x1E);
            dx = (x2 * f) >> 12;
            M2C_FIELD(obj, s16 *, 0x98) = dx;
            dz = (z2 * f) >> 12;
            M2C_FIELD(obj, s16 *, 0x9C) = dz;
            M2C_FIELD(anim, s32 *, 0x28) += dx;
            M2C_FIELD(anim, s32 *, 0x30) += dz;
        } else {
            M2C_FIELD(obj, s16 *, 0x98) = x1 - M2C_FIELD(anim, s32 *, 0x28);
            M2C_FIELD(obj, s16 *, 0x9C) = z1 - M2C_FIELD(anim, s32 *, 0x30);
            M2C_FIELD(anim, s32 *, 0x28) = 0;
            M2C_FIELD(anim, s32 *, 0x30) = 0;
        }
    }
    M2C_FIELD(M2C_FIELD(obj, u8 **, 0x94), s16 *, 0xE) = y1 + (((y2 - y1) * M2C_FIELD(anim, s16 *, 0x18)) >> 12);
    if (obj[0xA6] != 0) {
        D_800DE0C8[anim[0xA]] = 1;
    } else {
        D_800DE0C8[anim[0xA]] = 2;
    }
}

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

void func_8005BEF0(u8 *obj, u8 *anim) {
    s16 q1[4];
    s16 q2[4];
    s16 rx, ry, rz;
    u8 *p1;
    u8 *p2;
    u8 *sub;
    u8 *e;
    u8 *m = D_800DDE34;
    if ((M2C_FIELD(anim, s32 *, 0x24) & 0x40000) || (sub = M2C_FIELD(obj, u8 **, 0x118), M2C_FIELD(sub, s32 *, 0x3C) == 4) || M2C_FIELD(sub, s32 *, 0x54) == 1) {
        M2C_FIELD(anim, s16 *, 0xC) = 0;
        return;
    }
    e = D_800DDEE4[M2C_FIELD(anim, s16 *, 0x10)];
    p2 = e + M2C_FIELD(anim, s16 *, 0x14) * 0x4C;
    p1 = p2 + 0x14;
    e = D_800DDEE4[M2C_FIELD(anim, s16 *, 0x12)];
    p2 = e + M2C_FIELD(anim, s16 *, 0x16) * 0x4C;
    p2 += 0x14;
    q1[0] = (s8)p1[0x38] << 5;
    q1[2] = (s8)p1[0x39] << 5;
    q1[1] = (s8)p1[0x3A] << 5;
    q1[3] = -(s8)p1[0x3B] << 5;
    q2[0] = (s8)p2[0x38] << 5;
    q2[2] = (s8)p2[0x39] << 5;
    q2[1] = (s8)p2[0x3A] << 5;
    q2[3] = -(s8)p2[0x3B] << 5;
    q1[0] = q1[0] + (((q2[0] - q1[0]) * M2C_FIELD(anim, s16 *, 0x18)) >> 12);
    q1[1] = q1[1] + (((q2[1] - q1[1]) * M2C_FIELD(anim, s16 *, 0x18)) >> 12);
    q1[2] = q1[2] + (((q2[2] - q1[2]) * M2C_FIELD(anim, s16 *, 0x18)) >> 12);
    q1[3] = q1[3] + (((q2[3] - q1[3]) * M2C_FIELD(anim, s16 *, 0x18)) >> 12);
    func_8005C214(q1, m + 0x10);
    func_8005CFC8(q1, &rx, &ry, &rz);
    M2C_FIELD(anim, s16 *, 0xC) = rx;
}

void func_8005C10C(u8 *obj, u8 *model, s16 *f1, s16 *f2) {
    s16 q1[4];
    s16 q2[4];
    s16 q[4];
    q1[0] = (s8)((u8 *)f1)[0x28] << 5;
    q1[2] = (s8)((u8 *)f1)[0x29] << 5;
    q1[1] = (s8)((u8 *)f1)[0x2A] << 5;
    q1[3] = -((s8)((u8 *)f1)[0x2B] << 5);
    q2[0] = (s8)((u8 *)f2)[0x28] << 5;
    q2[2] = (s8)((u8 *)f2)[0x29] << 5;
    q2[1] = (s8)((u8 *)f2)[0x2A] << 5;
    q2[3] = -((s8)((u8 *)f2)[0x2B] << 5);
    if (f1 == f2) {
        func_8005C214(q1, M2C_FIELD(model, u8 **, 0x90) + 0x10);
    } else {
        func_8005D2B0(q1, q2, M2C_FIELD(M2C_FIELD(obj, u8 **, 0x94), s16 *, 0x18), q);
        func_8005C214(q, M2C_FIELD(model, u8 **, 0x90) + 0x10);
    }
}

#ifdef NON_MATCHING
void func_8005C214(s16 *q, QM *m) {
    s32 xx, xy, xz, yy, yz, zz, wx, wy, wz;
    s32 x = q[0];
    s32 y = q[1];
    s32 z = q[2];
    s32 w;
    xx = x * x;
    xy = x * y;
    xz = x * z;
    yy = y * y;
    yz = y * z;
    zz = z * z;
    do { } while (0);
    w = q[3];
    wz = w * z;
    wy = w * y;
    wx = w * x;
    m->m[0][0] = 0x1000 - (zz >> 11) - (yy >> 11);
    m->m[0][1] = (wz >> 11) - (xy >> 11);
    m->m[0][2] = (xz >> 11) + (wy >> 11);
    m->m[1][0] = -(xy >> 11) - (wz >> 11);
    m->m[1][1] = 0x1000 - (zz >> 11) - (xx >> 11);
    m->m[1][2] = (wx >> 11) - (yz >> 11);
    m->m[2][0] = (xz >> 11) - (wy >> 11);
    m->m[2][1] = -(yz >> 11) - (wx >> 11);
    m->m[2][2] = 0x1000 - (yy >> 11) - (xx >> 11);
    m->pad = 0;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005C214);
#endif

void func_8005C33C(void) {
    SVECTOR v;
    VECTOR d;
    SVECTOR *sv;
    VECTOR *dv;
    u8 slots[48];
    s32 off[3];
    u8 *p;
    s32 n;
    s32 i;
    s32 j;
    u8 *q;
    u8 *q2;
    u8 *obj;
    u8 *objEnd;
    u8 *o;
    u8 *e;
    u8 *ent;
    u8 *s;
    s16 *z;
    u8 *sl;
    u8 *sl2;
    s32 lim;
    EntList *el;
    s32 ix;
    u8 *tb;
    u8 *e2;
    u32 id;
    u32 cnt;
    s32 cnt2;
    s32 t;

    cnt = D_800F3B48;
    if (cnt != 0) {
        p = D_800F2FBC;
        sv = &v;
        dv = &d;
        v.pad = 0;
        d.pad = 0;
        {
            for (n = cnt - 1; n != -1; n--) {
                if ((u32)(D_800F2FBC + 0x880) < (u32)p) {
                    printf(D_800A8D44);
                    return;
                }
                sv->vx = *(u16 *)p;
                sv->vy = *(u16 *)(p + 0x4);
                sv->vz = *(u16 *)(p + 0x8);
                gte_ldv0(sv);
                gte_mvmva_v0();
                sv->vx = *(u16 *)(p + 0x10);
                gte_stlvnl(p);
                sv->vy = *(u16 *)(p + 0x14);
                sv->vz = *(u16 *)(p + 0x18);
                gte_ldv0(sv);
                gte_mvmva_v0();
                gte_stlvnl(p + 0x10);
                dv->vx = *(s32 *)(p + 0x10) - *(s32 *)p;
                dv->vy = *(s32 *)(p + 0x14) - *(s32 *)(p + 0x4);
                dv->vz = *(s32 *)(p + 0x18) - *(s32 *)(p + 0x8);
                dv->vx = dv->vx < -0x6882 ? -0x6882 : (dv->vx > 0x6882 ? 0x6882 : dv->vx);
                dv->vy = dv->vy < -0x6882 ? -0x6882 : (dv->vy > 0x6882 ? 0x6882 : dv->vy);
                dv->vz = dv->vz < -0x6882 ? -0x6882 : (dv->vz > 0x6882 ? 0x6882 : dv->vz);
                t = ((s32 (*)(VECTOR *, u8 *))func_8008C5D8)(dv, p + 0x20);
                *(s32 *)(p + 0x30) = t;
                *(s32 *)(p + 0x34) = SquareRoot0(t);
                p += 0x44;
            }
        }
        func_8007EC78(&D_800DDF08, &D_800DDF24, D_8009D8E8);
        z = (s16 *)(slots + 0x28);
        for (j = 5; j >= 0; j--) {
            *z = 0;
            z -= 4;
        }
        cnt2 = D_800F1DCC;
        if (cnt2 != 0) {
            q = D_800F3848;
            for (i = cnt2 - 1; i != -1; i--) {
                sl = slots + *(s32 *)q * 8;
                if (*(s16 *)sl == 0) {
                    *(s16 *)sl = *(u16 *)(q + 4);
                    sl2 = slots + *(s32 *)q * 8;
                    *(u8 **)(sl2 + 4) = q;
                }
                q += 0x18;
            }
        }
        j = 0;
        objEnd = (u8 *)D_800AFFC8 + D_800B0DA0 * 0x124;
        obj = (u8 *)D_800AFFC8 + D_800B07B0 * 0x124;
        do {
            if (*(s16 *)(slots + j * 8) != 0) {
                e = 0;
                for (o = objEnd; obj >= o; o += 0x124) {
                    if (*(u32 *)(o + 0xC0) == ((u32 *)&D_800DDF08)[j]) {
                        e = o;
                        break;
                    }
                }
                if (e != 0 && !(*(s32 *)(e + 0x4C) & 0x400)) {
                    lim = 5;
                    if (*(u8 *)(e + 0xA) & 2) {
                        lim = 0x14;
                    }
                    if ((u32)(D_8009CDA8 - *(s32 *)(e + 0x30)) >= (u32)lim) {
                        s = slots + j * 8;
                        ix = *(s32 *)(*(u8 **)(s + 4) + 8);
                        tb = D_800F2FBC;
                        e2 = tb + ix * 0x44;
                        q2 = *(u8 **)(e2 + 0x3C);
                        if (*(u8 **)(q2 + 0x88) != e) {
                            ent = *(u8 **)(q2 + 0x10C);
                            el = (EntList *)ent;
                            if (el->cnt < 5) {
                                u8 *src = *(u8 **)(e + 0x90);
                                func_8005ADD4(*(void **)(e + 0xC0), *(s16 *)s, off);
                                el->arr[el->cnt].obj = e;
                                el->arr[el->cnt].x = off[0] + *(u16 *)(src + 0x28);
                                src += 0x28;
                                el->arr[el->cnt].y = off[1] + *(u16 *)(src + 4);
                                el->arr[el->cnt].z = off[2] + *(u16 *)(src + 8);
                                el->arr[el->cnt].sx = *(s16 *)s;
                                el->arr[el->cnt].sy = *(u16 *)(*(u8 **)(s + 4) + 0xC);
                                el->cnt = el->cnt + 1;
                            } else {
                                printf(D_800A8D58, el->cnt);
                            }
                        }
                    }
                }
            }
            j++;
        } while (j < 6);
    }
}

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

void func_8005C8C4(s32 arg0, AnimTick *e) {
    u16 t;
    s32 x;
    s32 v;
    s32 h;
    if (!(e->f24 & 0x40000)) {
        t = e->a18 + e->a1E;
        e->a18 = t;
        x = (s16)t;
        if (x >= 0x1000) {
            v = x;
            if (v < 0) v = v + 0xFFF;
            e->a18 = x - ((v >> 12) << 12);
            if ((s16)e->a12 != (s16)e->a10 || (e->f0 & 2)) {
                e->a14 = e->a16;
            } else {
                e->a14 = e->a14 + 1;
            }
            e->a10 = e->a12;
            func_8005C864(e);
            h = (s16)e->a14 % ((s16)e->a1A + 1);
            e->a14 = h;
            if (e->f0 & 2) {
                e->a16 = h;
            } else {
                e->a16 = h + 1;
            }
            if ((s16)e->a16 > (s16)e->a1C) {
                e->a16 = 0;
            }
            if ((s16)e->a18 == 0) {
                func_8005CB38(arg0);
            }
        }
    }
}

void func_8005CA20(void *obj) {
    if ((D_8009CD8C = ((s32 *(*)(s32))func_800698B8)(M2C_FIELD(obj, s16 *, 0x122))) != NULL) {
        s32 r = rand() % 3 + 1;
        D_8009CD88 += r;
        D_8009CD88 = D_8009CD88 % 4;
        if (obj != g_Player) {
            g_Args->pad12E[0xA] = 0x7F;
        }
        func_80027248((u8)D_8009CD8C[D_8009CD88], M2C_FIELD(obj, s32 *, 0x90) + 0x14, 2);
    }
}

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

void func_8005CB38(s32 arg0) {
    u8 *p;
    s32 x;
    u16 code;
    s16 t;
    s16 c1;
    s16 sw1;
    p = *(u8 **)(arg0 + 0x94);
    code = *(u16 *)(p + 0x12);
    x = *(s32 *)(arg0 + 0x90);
    sw1 = code - 0x25;
    switch (sw1) {
    case 0x0: t = *(s16 *)(p + 0x16); if (t == 6 || t == 0xC || t == 0xE) func_8005CA20((void *)arg0); break;
    case 0xF: t = *(s16 *)(p + 0x16); if (t == 7 || t == 0xC || t == 0xF) func_8005CA20((void *)arg0); break;
    case 0x50: t = *(s16 *)(p + 0x16); if (t == 5 || t == 9 || t == 0xC || t == 0xE || t == 0x11) func_8005CA20((void *)arg0); break;
    case 0x47: t = *(s16 *)(p + 0x16); if (t == 8 || t == 0xD || t == 0x10 || t == 0x13) func_8005CA20((void *)arg0); break;
    case 0x34: t = *(s16 *)(p + 0x16); if (t == 2 || t == 5) func_8005CA20((void *)arg0); break;
    case 0x54: t = *(s16 *)(p + 0x16); if (t == 5 || t == 0xA) func_8005CA20((void *)arg0); break;
    case 0x1A: t = *(s16 *)(p + 0x16); if (t == 4 || t == 0xB) func_8005CA20((void *)arg0); break;
    case 0x42: t = *(s16 *)(p + 0x16); if (t == 3 || t == 5) func_8005CA20((void *)arg0); break;
    case 0x53: t = *(s16 *)(p + 0x16); if (t == 4 || t == 0xB) func_8005CA20((void *)arg0); break;
    case 0x2A: t = *(s16 *)(p + 0x16); if (t == 4 || t == 0xB) func_8005CA20((void *)arg0);
    case 0x3D: t = *(s16 *)(p + 0x16); if (t == 4 || t == 9) func_8005CA20((void *)arg0); break;
    case 0x35: t = *(s16 *)(p + 0x16); if (t == 6 || t == 0xD || t == 0x14 || t == 0x1B) func_8005CA20((void *)arg0); break;
    case 0x6: t = *(s16 *)(p + 0x16); if (t == 8 || t == 0xC || t == 0xF || t == 0x12) func_8005CA20((void *)arg0); break;
    case 0x4B: t = *(s16 *)(p + 0x16); if (t == 9 || t == 0xD || t == 0xF || t == 0x11) func_8005CA20((void *)arg0); break;
    case 0x48: t = *(s16 *)(p + 0x16); if (t == 8 || t == 4) func_8005CA20((void *)arg0); break;
    case 0x5: t = *(s16 *)(p + 0x16); if (t == 2 || t == 5 || t == 8 || t == 0xA) func_8005CA20((void *)arg0); break;
    case 0x43: t = *(s16 *)(p + 0x16); if (t == 3 || t == 8 || t == 0xD || t == 0x14) func_8005CA20((void *)arg0); break;
    }
    c1 = code - 1;
    switch (c1) {
    case 34: t = *(s16 *)(p + 0x16); if (t == 9) func_8005CAF8(1, x); else if (t == 0x10) func_8005CAF8(2, x); break;
    case 48: t = *(s16 *)(p + 0x16); if (t == 0xB || t == 0x13) func_8005CAF8(1, x); break;
    case 2: if (*(s16 *)(p + 0x16) == 0xA) func_8005CAF8(1, x); break;
    case 0: if (*(s16 *)(p + 0x16) == 8) func_8005CAF8(1, x); break;
    case 49: t = *(s16 *)(p + 0x16); if (t == 5 || t == 0xB) func_8005CAF8(1, x); break;
    case 47: if (*(s16 *)(p + 0x16) == 5) func_8005CAF8(1, x); break;
    case 23: if (*(s16 *)(p + 0x16) == 9) func_8005CAF8(2, x); break;
    }
}

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

#ifdef NON_MATCHING
void func_8005CFC8(s16 *q, s16 *rx, s16 *ry, s16 *rz) {
    s32 m;
    s32 n;
    s32 s;
    s32 len;
    s32 sq;
    s16 a;
    s16 b;
    s16 c;
    s16 d;

    a = (q[3] * q[3]) >> 12;
    b = (q[1] * q[1]) >> 12;
    c = (q[1] * q[2]) >> 12;
    d = (q[0] * q[3]) >> 12;
    m = 0x1000 - b * 2 - a * 2;
    n = (c + d) * 2;
    m *= m;
    sq = m + n * n;
    s = (q[0] * q[1] - q[2] * q[3]) * 2;
    len = SquareRoot0(sq) << 12;
    *ry = ((s32 (*)(s32, s32))ratan2)(s, len);
    if (len > 0x4000) {
        *rx = ((s32 (*)(s32, s32))ratan2)((q[1] * q[3] + q[0] * q[2]) * 2, 0x1000000 - q[2] * q[2] * 2 - q[1] * q[1] * 2);
        *rz = ((s32 (*)(s32, s32))ratan2)((q[1] * q[2] + q[0] * q[3]) * 2, 0x1000000 - q[1] * q[1] * 2 - q[3] * q[3] * 2);
    } else {
        *rx = ((s32 (*)(s32, s32))ratan2)((q[0] * q[3] - q[1] * q[2]) * 2, 0x1000000 - q[2] * q[2] * 2 - q[3] * q[3] * 2);
        *rz = 0;
    }
    *ry &= 0xFFF;
    *rx &= 0xFFF;
    *rz &= 0xFFF;
    if (*ry > 0x400 && *ry < 0xC00) {
        *ry = (0x800 - *ry) & 0xFFF;
        *rx = (*rx + 0x800) & 0xFFF;
        *rz = (*rz + 0x800) & 0xFFF;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005CFC8);
#endif

void func_8005D2B0(s16 *a, s16 *b, s16 t, s16 *out) {
    s16 tmp[4];
    s32 dot;
    s16 ang;
    s16 s;
    s16 k0, k1;
    dot = (s16)((a[0] * b[0]) >> 12) + (s16)((a[1] * b[1]) >> 12) + (s16)((a[2] * b[2]) >> 12) + (s16)((a[3] * b[3]) >> 12);
    if (dot < 0) {
        tmp[1] = -b[1];
        tmp[2] = -b[2];
        tmp[3] = -b[3];
        dot = -dot;
        tmp[0] = -b[0];
    } else {
        tmp[1] = b[1];
        tmp[2] = b[2];
        tmp[3] = b[3];
        tmp[0] = b[0];
    }
    if (0x1000 - dot > 16) {
        ang = ((s32 (*)(s32))func_8005ADB0)(dot);
        k0 = *(s16 *)&D_800A0370[((0x1000 - t) * ang >> 12) & 0xFFF];
        s = *(s16 *)&D_800A0370[ang & 0xFFF];
        k0 = (k0 << 12) / s;
        k1 = (*(s16 *)&D_800A0370[(t * ang >> 12) & 0xFFF] << 12) / s;
    } else {
        k0 = 0x1000 - t;
        k1 = t;
    }
    out[0] = (a[0] * k0 >> 12) + (tmp[0] * k1 >> 12);
    out[1] = (a[1] * k0 >> 12) + (tmp[1] * k1 >> 12);
    out[2] = (a[2] * k0 >> 12) + (tmp[2] * k1 >> 12);
    out[3] = (a[3] * k0 >> 12) + (tmp[3] * k1 >> 12);
}

s32 func_8005D58C(s32 arg0) {
  s32 temp_v0;
  *((s16 *) (((s8 *) arg0) + 0x1E)) = 0x800;
  *((s16 *) (((s8 *) arg0) + 4)) = -1;
  temp_v0 = D_8009D5E4;
  ;
  *((s32 *) (((s8 *) arg0) + 0)) = (s32) (((*((s32 *) (((s8 *) arg0) + 0))) & (~0xF0)) | ((temp_v0 & 0xF) * 0x10));
  return (temp_v0 & 0xF) * 0x10;
}

s32 func_8005D5C4(M2C_UNK *arg0, void *arg1) {
    s32 *p;

    p = (s32 *)arg0;
    if ((u32)arg1 >= (u32)p) {
        do {
            if (!(*p & 1)) {
                goto found;
            }
            p += 0xE;
        } while ((u32)arg1 >= (u32)p);
    }
    return 0;
found:
    func_8005D630(p);
    *p |= 1;
    return (s32)p;
}

void func_8005D630(s32 *p) {
    s32 i;
    s32 n;

    n = 14;
    for (i = 0; i < n; i++) {
        *p++ = 0;
    }
}

void func_8005D654(void) {
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = M2C_FIELD(g_Player, void **, 0x94);
    M2C_FIELD(temp_v1, s32 *, 0x24) = (s32) (M2C_FIELD(temp_v1, s32 *, 0x24) ^ 0x400);
    temp_v1_2 = M2C_FIELD(g_Player, void **, 0x94);
    M2C_FIELD(temp_v1_2, s32 *, 0x24) = (s32) (M2C_FIELD(temp_v1_2, s32 *, 0x24) ^ 0x200);
}

#ifdef NON_MATCHING
void func_8005D694(void) {
    s32 toggle;
    s32 crouched;
    u16 button;

    button = g_Args->buttonMap[4];
    toggle = (g_Args->flags2 >> 5) & 1;
    crouched = *(s32 *)(*(u8 **)((u8 *)g_Player + 0x94) + 0x24) & 0x400;
    if ((g_PadPressed & button) || (toggle && crouched && !(g_PadHeld & button))) {
        if (D_8009CA94 != 0 && crouched) {
            if (!toggle) {
                func_80026F30(0xE, 0);
                func_80048B4C((M2C_UNK *)D_800A8F94, 0x80, 0);
            }
        } else {
            func_8005D654();
        }
    }
}
#else
void func_8005D694(void) {
  s32 toggle;
  int new_var;
  s32 crouched;
  int button;
  new_var = 5;
  button = g_Args->buttonMap[4];
  toggle = g_Args->flags2 >> new_var;
  toggle = toggle & 1;
  crouched = (*((s32 *) ((*((u8 **) (((u8 *) g_Player) + 0x94))) + 0x24))) & 0x400;
  if ((g_PadPressed & button) || ((toggle && crouched) && (!(g_PadHeld & g_Args->buttonMap[4]))))
  {
    if ((D_8009CA94 != 0) && crouched)
    {
      if (!toggle)
      {
        func_80026F30(0xE, 0);
        func_80048B4C((M2C_UNK *) D_800A8F94, 0x80, 0);
      }
    }
    else
    {
      func_8005D654();
    }
  }
}
#endif

void func_8005D760(void) {
    u8 *msg;
    s32 held;
    s32 prev;
    s16 v;
    u16 b5;
    s32 b1;
    s32 b2;
    u16 b3;
    u16 b0;
    s32 fire;
    Args *args;

    if (g_CutScene != 0 || D_8009CD00 != 0 || (M2C_FIELD(&D_800AC284, u8 *, 0x108) != 0 && M2C_FIELD(&D_800AC284, s32 *, 0x104) == 1)) {
        func_8005DA90();
        return;
    }
    held = g_PadHeld;
    prev = g_PadPrev;
    if (*(u8 *)&D_800B0E54 != 0) {
        D_800DE0DC = 0;
        D_800DE0E0 = 0;
        D_800DDDB8 = 0;
        D_800DDCC4 = 0;
        D_800DDCC0 = 0;
        v = D_800B0E5F - 0x7F;
        D_800DDC44 = 0;
        if (v > 50) {
            D_800DDCC0 = 1;
            D_800DE0E0 = v;
        } else if (v < -50) {
            D_800DDC44 = 1;
            D_800DE0E0 = v;
        }
        v = D_800B0E5E - 0x7F;
        if (v > 50) {
            D_800DDDB8 = 1;
            D_800DE0DC = v;
        } else if (v < -50) {
            D_800DDCC4 = 1;
            D_800DE0DC = v;
        }
    } else {
        D_800DDC44 = held & 0x1000;
        D_800DDCC0 = held & 0x4000;
        D_800DDCC4 = held & 0x8000;
        D_800DDDB8 = held & 0x2000;
        D_800DE0DC = 0;
        D_800DE0E0 = 0;
    }
    D_800DDC60 = held & 1;
    D_800DDC54 = held & 2;
    D_800DDC4C = prev & 1;
    D_800DDC68 = prev & 2;
    D_800DDC5C = held & 4;
    D_800DDC50 = held & 8;
    D_800DDC48 = prev & 4;
    D_800DDC64 = prev & 8;
    D_800DDDAC = prev & 0x1000;
    D_800DDDB0 = prev & 0x4000;
    D_800DE0D8 = held & 0x80;
    D_800DDC58 = prev & 0x80;
    args = g_Args;
    D_800DDC6C = D_800DDD94;
    b5 = args->buttonMap[5];
    b1 = args->buttonMap[1];
    D_800DDDB4 = held & b5;
    D_800DDDBC = prev & b5;
    D_800DDE08 = held & b1;
    D_800DDD94 = held & args->buttonMap[4];
    b2 = args->buttonMap[2];
    D_800DDDA4 = held & b2;
    D_800DDD90 = held & args->buttonMap[7];
    D_800DDD8C = held & args->buttonMap[6];
    b3 = args->buttonMap[3];
    D_800DDCC8 = prev & b3;
    D_800DDD98 = held & b3;
    b0 = args->buttonMap[0];
    D_800DDEEC = fire = held & b0;
    D_800DDEE8 = prev & b0;
    if (D_8009D900 != 0) {
        D_800DDEEC = 0;
        D_800DDEE8 = 0;
    }
    if (fire != 0) {
        D_8009D90C = 0;
        D_8009D910++;
    } else {
        D_8009D910 = 0;
        D_8009D90C++;
    }
    D_800DDDA0 = prev & b2;
    D_800DDE04 = prev & b1;
}

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

#ifdef NON_MATCHING
void func_8005DB7C(u8 *arg0, u8 *arg1) {
    s32 *v = *(s32 **)(arg0 + 0x60);
    s32 *q;
    if (v != 0) {
        *v = 0;
        if (arg1 != 0) {
            u8 *o = *(u8 **)(arg1 + 0x90);
            D_1F8003D4 = *(s32 *)(o + 0x28);
            D_1F8003D8 = *(s32 *)(o + 0x2C);
            D_1F8003DC = *(s32 *)(o + 0x30);
            q = *(s32 **)(arg0 + 0x60);
            *(s32 **)(arg1 + 0x60) = q;
            *q = arg1[6] | 0x100;
            (*(s32 **)(arg1 + 0x60))[-3] = D_1F8003D4 - D_800D2FB0.x;
            (*(s32 **)(arg1 + 0x60))[-2] = D_1F8003D8 - D_800D2FB0.y;
            (*(s32 **)(arg1 + 0x60))[-1] = D_1F8003DC - D_800D2FB0.z;
            (*(s32 **)(arg1 + 0x60))[1] = 0;
            (*(s32 **)(arg1 + 0x60))[2] = 0;
        }
        *(s32 **)(arg0 + 0x60) = 0;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005DB7C);
#endif

void func_8005DC40(AlarmObj *obj) {
    s32 spawned;
    s32 kind;
    s32 skill;
    s32 recent;
    u32 dt;
    s32 r;
    s32 chance;
    s32 go;
    u8 id;

    spawned = 0;
    kind = 1;
    if (obj->unkA != kind) {
        skill = M2C_FIELD(g_Args, u8 *, 0x12A);
        dt = D_8009CDA8 - D_8009D904;
        recent = dt < skill * 200;
        r = rand() % 100;
        if (skill == 1) {
            chance = r < 30;
        } else {
            chance = r < 20;
        }
        kind = 0;
        if ((obj->flags48 & 0x2000000) && (rand() & 3)) {
            go = 1;
            kind = 1;
        } else if (obj->flags48 & 0x400000) {
            go = 1;
            kind = 2;
        } else {
            if (recent) {
                return;
            }
            go = 0;
            if (*(s32 *)((u8 *)g_Player + 0x34) <= 0x7FFF) {
                go = D_8009CFF0 <= 0x7FFF;
            }
            go &= chance;
        }
        D_1F8003D4 = obj->sub->x;
        D_1F8003D8 = obj->sub->y - 900;
        D_1F8003DC = obj->sub->z;
        if (go) {
            if (kind == 1) {
                spawned = func_80039A9C((s32)obj, D_8009D104, 4);
            } else if (kind == 2) {
                spawned = func_80039A9C((s32)obj, D_8009D106, 4);
            } else if (skill == 1 && *(s32 *)((u8 *)g_Player + 0x34) < 0x200 && D_8009CFF0 <= 0x7FFF && (rand() & 1)) {
                spawned = func_80039A9C((s32)obj, D_8009D106, 4);
            } else {
                spawned = func_80039A9C((s32)obj, D_8009D104, 4);
            }
        } else if (chance) {
            s32 *tbl = (s32 *)&D_800DF12C;
            if (tbl[(u32)D_8009CDA8 % 50] & 1) {
                if (D_8009D108 == 0) {
                    goto none;
                }
                spawned = func_80039A9C((s32)obj, D_8009D108, 4);
            } else {
                if (D_8009D10A == 0) {
none:
                    spawned = 0;
                    goto done;
                }
                spawned = func_80039A9C((s32)obj, D_8009D10A, 4);
            }
        }
done:
        if (spawned) {
            ((AlarmObj *)spawned)->owner = obj;
            D_8009D904 = D_8009CDA8;
            func_80060614(spawned);
            func_800393AC((void *)spawned);
        }
    }
}

void func_8005DF40(void *arg0) {
    s32 temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0x8C);
    if (temp_s0 != 0) {
        func_80060614(temp_s0);
        func_800393AC(temp_s0);
    }
}

#ifdef NON_MATCHING
void func_8005DF7C(u8 *obj) {
    s32 s = 0;
    s32 a, b;
    if (obj[8] != 0) {
        D_1F8003D4 = *(s32 *)(*(u8 **)(obj + 0x90) + 0x28);
        a = *(s32 *)(*(u8 **)(obj + 0x90) + 0x2C) - 0x32;
        b = *(s32 *)(*(u8 **)(obj + 0x90) + 0x30);
        D_1F8003D8 = a;
        D_1F8003DC = b;
        s = func_80039A9C(0, obj[8], 4);
        func_800393AC((void *)s);
    }
    func_8005DF40(obj);
    func_8005DC40(obj);
    func_8005DB7C(obj, (void *)s);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005DF7C);
#endif

void func_8005E020(arg0) {
    void *s1;
    s32 *p;
    s32 *p2;
    s32 *p3;
    s32 *p4;
    s32 *p5;
    s32 *p6;
    s32 fl;
    s32 q;
    q = M2C_FIELD(arg0, u16 *, 0x66) & 0x1F;
    s1 = M2C_FIELD(arg0, void **, 0x94);
    fl = M2C_FIELD(s1, s32 *, 0x24);
    if (M2C_FIELD(arg0, u8 *, 0xA4) & 0x10) {
        p = M2C_FIELD(arg0, s32 **, 0x118);
        if (p != NULL) {
            *p |= 0x20000;
        }
        M2C_FIELD(arg0, u8 *, 0xA4) &= 0xEF;
        return;
    }
    if (!(fl & 0x20))
    if (!(fl & 0x100000)) {
        p2 = M2C_FIELD(arg0, s32 **, 0x118);
        if (p2 != NULL) {
            p2[9] = q;
        }
        M2C_FIELD(arg0, s32 *, 0x30) = D_8009CDA8;
        if (M2C_FIELD(arg0, s32 *, 0x34) <= 0) {
            u8 f;
            ((void (*)(void *))func_80052BEC)(arg0);
            if (!(M2C_FIELD(arg0, u8 *, 0xA4) & 8)) {
                func_8005DF7C(arg0);
            }
            f = M2C_FIELD(arg0, u8 *, 0xA4);
            if (f & 4) {
                p3 = M2C_FIELD(arg0, s32 **, 0x118);
                M2C_FIELD(arg0, u8 *, 0xA4) = f & 0xFB;
                if (p3 != NULL) {
                    *p3 |= 0xD;
                }
            } else if (f & 8) {
                p4 = M2C_FIELD(arg0, s32 **, 0x118);
                M2C_FIELD(arg0, u8 *, 0xA4) = f & 0xF7;
                if (p4 != NULL) {
                    *p4 |= 5;
                }
            } else if (M2C_FIELD(arg0, u8 *, 0x3B) == 1) {
                p5 = M2C_FIELD(arg0, s32 **, 0x118);
                if (p5 != NULL) {
                    *p5 |= 7;
                }
            } else {
                p6 = M2C_FIELD(arg0, s32 **, 0x118);
                if (p6 != NULL) {
                    *p6 |= 5;
                }
            }
            M2C_FIELD(s1, s32 *, 0x24) = (M2C_FIELD(s1, s32 *, 0x24) | 0x20) & 0xFFFBFFFF;
        } else {
            if (M2C_FIELD(arg0, u8 *, 0x3B) == 1) {
                p = M2C_FIELD(arg0, s32 **, 0x118);
                if (p != NULL) {
                    *p |= 3;
                }
            } else {
                p = M2C_FIELD(arg0, s32 **, 0x118);
                if (p != NULL) {
                    *p |= 1;
                }
            }
            M2C_FIELD(s1, s32 *, 0x24) = M2C_FIELD(s1, s32 *, 0x24) & 0xFFFBFFFF;
        }
    }
}

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

#ifdef NON_MATCHING
s32 func_8005E424(u8 *a, u8 *b, s32 flags) {
    s32 pts[20];
    s32 bv[3], dv[3];
    s32 tx, tz, cx, cz, t, ang, v, ang2;
    s32 sinB, cosB, sinA, cosA, q;
    u16 r;
    u8 *ap, *bp;

    if (b == a) return 0;
    r = 0x1F4;
    if (b != (u8 *)g_Player) r = W(b, u16, 0x2A);
    if (W(W(b, u8 *, 0x94), s32, 0x24) & 0x20) {
        if (W(a, u8, 5) != 6) r += 0x258;
    }
    if (flags & 1) r *= 2;
    bp = W(b, u8 *, 0x90);
    bv[0] = W(bp, s32, 0x28) + W(b, s16, 0x98);
    bv[1] = W(bp, s32, 0x2C);
    bv[2] = W(bp, s32, 0x30) + W(b, s16, 0x9C);
    if (W(b, u8, 5) == 6) bv[1] = bv[1] - W(b, s16, 0x22);
    ap = W(a, u8 *, 0x90);
    pts[0] = W(ap, s32, 0x28);
    pts[1] = W(ap, s32, 0x2C);
    dv[1] = bv[1] - pts[1];
    pts[2] = W(ap, s32, 0x30);
    if (W(a, s16, 0x22) < dv[1] + W(b, s16, 0x20)) return 0;
    if (dv[1] + W(b, s16, 0x22) < W(a, s16, 0x20)) return 0;
    t = D_800A0370[W(W(a, u8 *, 0x90), u16, 0x62) & 0xFFF];
    cosA = t >> 16;
    sinA = (s16)t;
    if (W(a, u8, 5) == 3) {
        s32 hi = W(a, s16, 0x1E);
        s32 hz = W(a, s16, 0x26);
        cx = hi - ((hi - W(a, s16, 0x1C)) / 2);
        cz = hz - ((hz - W(a, s16, 0x24)) / 2);
        pts[0] = (cx * cosA + cz * sinA) >> 12;
        pts[0] = pts[0] + W(ap, s32, 0x28);
        pts[2] = (cz * cosA - cx * sinA) >> 12;
        pts[2] = pts[2] + W(ap, s32, 0x30);
    }
    if (W(a, u8, 5) == 6) pts[1] = W(ap, s32, 0x2C) - W(a, s16, 0x22);
    tx = bv[0] - pts[0];
    tz = bv[2] - pts[2];
    if (abs(tx) >= 0x3A99) return 0;
    if (abs(tz) >= 0x3A99) return 0;
    q = r + W(a, u16, 0x2A);
    if (q * q < tx * tx + tz * tz) return 0;
    if (W(a, u8, 5) == 6) {
        if (flags & 4) return 0x80;
        if (flags & 1) {
            s16 av = W(b, s16, 0xC4);
            if (av < 0) av = -av;
            if (av < 0x401) ang = W(W(b, u8 *, 0x90), u16, 0x62) & 0xFFF;
            else ang = ((s16)W(W(b, u8 *, 0x90), u16, 0x62) + 0x800) & 0xFFF;
            v = (ratan2(-tx, -tz) & 0xFFF) - ang;
            func_8005EB6C((v - 0x111) & 0xFFF, (v + 0x111) & 0xFFF);
        }
        return 0;
    }
    tx = bv[0] - W(ap, s32, 0x28);
    tz = bv[2] - W(ap, s32, 0x30);
    dv[0] = (tx * cosA - tz * sinA) >> 12;
    dv[2] = (tz * cosA + tx * sinA) >> 12;
    if (W(a, s16, 0x1E) < dv[0] - r) return 0;
    if (dv[0] + r < W(a, s16, 0x1C)) return 0;
    if (W(a, s16, 0x26) < dv[2] - r) return 0;
    if (dv[2] + r < W(a, s16, 0x24)) return 0;
    if (flags & 1) {
        s16 av = W(b, s16, 0xC4);
        s32 ntx = -tx, ntz = -tz;
        if (av < 0) av = -av;
        if (av < 0x401) ang = W(W(b, u8 *, 0x90), u16, 0x62) & 0xFFF;
        else ang = ((s16)W(W(b, u8 *, 0x90), u16, 0x62) + 0x800) & 0xFFF;
        cosB = W(&D_800A0370[ang], s16, 2);
        sinB = W(&D_800A0370[ang], s16, 0);
        pts[0] = (ntx * cosB - ntz * sinB) >> 12;
        pts[2] = (ntz * cosB + ntx * sinB) >> 12;
        ang2 = (ang - (W(W(a, u8 *, 0x90), u16, 0x62) & 0xFFF)) & 0xFFF;
        sinA = W(&D_800A0370[ang2], s16, 0);
        cosA = W(&D_800A0370[ang2], s16, 2);
        pts[4] = pts[0] + ((W(a, s16, 0x1C) * cosA - W(a, s16, 0x24) * sinA) >> 12);
        pts[6] = pts[2] + ((W(a, s16, 0x24) * cosA + W(a, s16, 0x1C) * sinA) >> 12);
        pts[8] = pts[0] + ((W(a, s16, 0x1E) * cosA - W(a, s16, 0x24) * sinA) >> 12);
        pts[10] = pts[2] + ((W(a, s16, 0x24) * cosA + W(a, s16, 0x1E) * sinA) >> 12);
        pts[12] = pts[0] + ((W(a, s16, 0x1E) * cosA - W(a, s16, 0x26) * sinA) >> 12);
        pts[14] = pts[2] + ((W(a, s16, 0x26) * cosA + W(a, s16, 0x1E) * sinA) >> 12);
        pts[16] = pts[0] + ((W(a, s16, 0x1C) * cosA - W(a, s16, 0x26) * sinA) >> 12);
        pts[18] = pts[2] + ((W(a, s16, 0x26) * cosA + W(a, s16, 0x1C) * sinA) >> 12);
        func_8005EADC(pts);
    }
    return (flags << 5) & 0x80;
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_8002C808", func_8005E424);
#endif

void func_8005EADC(s32 *arg0) {
    s32 i;
    for (i = 0; i < 5; i++) {
        arg0[i * 4 + 1] = ((s32 (*)(s32, s32))ratan2)(arg0[i * 4], arg0[i * 4 + 2]) & 0xFFF;
    }
    for (i = 1; i < 5; i++) {
        func_8005EB6C(arg0[i * 4 + 1], arg0[(i & 3) * 4 + 5]);
    }
}

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

s32 func_8005EC84(void *a, u32 *b) {
    if (*(s32 *)((u8 *)a + 0x34) <= 0) {
        if ((*b & 2) != 0) return 1;
    }
    return 0;
}

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
