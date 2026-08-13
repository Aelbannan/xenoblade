#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include <monolib/math/CVec3.hpp>

// MWCC pointer-to-member-function (12 bytes). Layout matches Runtime/ptmf.c:
//   this_delta @+0, v_offset @+4, f_data @+8.
// Retail stores the triple at +0x108/+0x10C/+0x110 and dispatches via
// __ptmf_scall (r12=&ptmf, r3=this) / tests via __ptmf_test.
struct PTMF {
    int this_delta;
    int v_offset;
    int f_data;
};

namespace cf {

// CCtrlMovePC - player-character movement controller (state machine).
//
// Derived from CCtrlMoveBase (base occupies 0x00..0x4B; secondary vtable at
// 0x48 is overwritten by the derived ctor). Own fields begin at 0x4C.
// mStateFunc (0x108) is a ptmf updated on transitions and dispatched per tick.
// Layout recovered from retail __ct__801993C4 and the state update functions.
class CCtrlMovePC {
public:
    // --- base class (CCtrlMoveBase) region 0x00..0x4B ---
    char mBase00[0x30];        // 0x00 primary vtable + base fields
    void* mBase30;             // 0x30 (NULL-init)
    void* mBaseData;           // 0x34 ctor param (data ptr)
    f32 mBase38;               // 0x38
    f32 mBase3C;               // 0x3C
    u16 mBase40;               // 0x40
    u16 mBase42;               // 0x42
    u8 mBase44;                // 0x44
    u8 mBase45;                // 0x45
    char mBase46[2];           // 0x46..0x47
    void* mBaseVtable2;        // 0x48 secondary vtable (overwritten here)

    // --- CCtrlMovePC own fields ---
    u32 mFlags4C;              // 0x4C state/condition flags
    u32 mFlags50;              // 0x50 secondary flags
    ml::CVec3 mVec54;          // 0x54 goal position
    ml::CVec3 mPos;            // 0x60 current position
    ml::CVec3 mVec6C;          // 0x6C previous position
    ml::CVec3 mVec78;          // 0x78 approach direction
    ml::CVec3 mVec84;          // 0x84 scratch delta
    ml::CVec3 mVec90;          // 0x90 sin/cos facing
    ml::CVec3 mVec9C;          // 0x9C
    ml::CVec3 mVecA8;          // 0xA8
    ml::CVec3 mVecB4;          // 0xB4
    ml::CVec3 mVecC0;          // 0xC0
    ml::CVec3 mVecCC;          // 0xCC
    ml::CVec3 mVecD8;          // 0xD8
    ml::CVec3 mVecE4;          // 0xE4 (recovered from func_8019CDA0)
    char mPadF0[0xC];          // 0xF0..0xFB
    f32 mDistFC;               // 0xFC
    f32 mFloat100;             // 0x100
    char mPad104[0x4];         // 0x104..0x107
    int (CCtrlMovePC::*mStateFunc)();  // 0x108 state dispatch ptmf (12 bytes)
    void* mPlayer;             // 0x114 player wrapper (CfObject @ +0x3e9c)
    void* mObject;             // 0x118 controlled object wrapper
    void* mTask;               // 0x11C active move task
    s16 mShort120;             // 0x120
    s16 mShort122;             // 0x122
    s16 mArr124[18];           // 0x124..0x146
};

// Move state sub-object (pointed by CfObjWrap::mSub): horizontal velocity
// components at 0x3F0/0x3F8, condition flags at 0x4EC, anim state at 0x530.
struct CfMoveSub {
    u8 pad_00[0xC];
    u32 mField0C;             // 0x0C
    u8 pad_10[0x3E0];         // 0x10..0x3EF
    f32 mField3F0;            // 0x3F0
    f32 mField3F4;            // 0x3F4
    f32 mField3F8;            // 0x3F8
    u8 pad_3FC[0xF0];         // 0x3FC..0x4EC
    u32 mField4EC;            // 0x4EC
    u8 pad_4F0[0x40];         // 0x4F0..0x530
    u16 mField530;            // 0x530
};

// Move-data block referenced by CCtrlMovePC::mBaseData.
struct CfMoveData {
    u8 pad_00[0x4];
    u32 mField04;             // 0x04
    u8 pad_08[0xC];
    f32 mField14;             // 0x14
};

// Wrapper for the object referenced by mObject/mPlayer: the move-state
// sub-object at +0x3F60 and the animation id u16 at +0x455A.
struct CfObjWrap {
    u8 pad_00[0x4];           // 0x00..0x03
    void* mField04;           // +0x04 sub-object (getCtrl-style vtable)
    u8 pad_08[0x3F20];        // 0x08..0x3F27
    u16 mField3F28;           // +0x3F28 character id (u16)
    u8 pad_3F2A[0x36];        // 0x3F2A..0x3F5F
    CfMoveSub* mSub;          // +0x3F60
    u8 pad_3F64[0x5F6];       // 0x3F64..0x455A
    u16 mField455A;           // +0x455A
};

// Object returned by getUnk80664658 (flag word at +0x214).
struct CfGlobalSettings {
    u8 field_0x00[0x214];
    u32 field_214;            // 0x214 flag bits
};

// State sub-object returned by CMoveObj::get68 (f32 at +0x14, s16 at +0x1AC).
struct CfObjState {
    u8 pad_00[0x14];
    f32 mField14;             // 0x14
    u8 pad_18[0x1AC - 0x18];
    s16 mField1AC;            // 0x1AC
};

// Object referenced by CCtrlMovePC::mBase30 (u16 at +0x22).
struct CfBase30Obj {
    u8 pad_00[0x22];
    u16 mField22;             // 0x22
};

// Battle manager singleton (s16 at +0x20C8).
struct CfBattleState {
    u8 pad_00[0x20C8];
    s16 mField20C8;           // 0x20C8
};

// Party-slot list returned by func_8009ECB0: two character ids.
struct CfPartyList {
    u8 pad_00[0x8];
    u32 mField08;             // 0x08 first slot character id
    u32 mField0C;             // 0x0C second slot character id
};

// Embedded move-object interface (object at CfObjWrap +0x3E9C). MWCC lays out
// the vtable at declared idx P -> offset 4*P; the class is never instantiated.
class CMovePosIntf {
public:
    virtual void vf00();  virtual void vf01();  virtual void vf02();
    virtual void vf03();  virtual void vf04();  virtual void vf05();
    virtual void vf06();  virtual void vf07();  virtual void vf08();
    virtual void vf09();  virtual void vf10();  virtual void vf11();
    virtual void vf12();  virtual void vf13();  virtual void vf14();
    virtual void vf15();  virtual void vf16();  virtual void vf17();
    virtual void vf18();  virtual void vf19();  virtual void vf20();
    virtual void vf21();  virtual void vf22();  virtual void vf23();
    virtual void vf24();  virtual void vf25();  virtual void vf26();
    virtual void vf27();  virtual void vf28();  virtual void vf29();
    virtual void vf30();  virtual void vf31();  virtual void vf32();
    virtual void vf33();  virtual void vf34();  virtual void vf35();
    virtual void vf36();  virtual void vf37();  virtual void vf38();
    virtual void vf39();  virtual void vf40();  virtual void vf41();
    virtual void* getHandle();      // idx 42 -> vtable 0xA8
    virtual ml::CVec3* getPosition();   // idx 43 -> vtable 0xAC
    virtual void vf44();  virtual void vf45();  virtual void vf46();
    virtual void vf47();  virtual void vf48();  virtual void vf49();
    virtual void setAngle(f32 a);   // idx 50 -> vtable 0xC8
    virtual void vf51();  virtual void vf52();  virtual void vf53();
    virtual void vf54();  virtual void vf55();  virtual void vf56();
    virtual void vf57();  virtual void vf58();  virtual void vf59();
    virtual void vf60();  virtual void vf61();  virtual void vf62();
    virtual void vf63();  virtual void vf64();  virtual void vf65();
    virtual void vf66();  virtual void vf67();  virtual void vf68();
    virtual void vf69();  virtual void vf70();  virtual void vf71();
    virtual void vf72();  virtual void vf73();  virtual void vf74();
    virtual void vf75();  virtual void vf76();  virtual void vf77();
    virtual void vf78();  virtual void vf79();  virtual void vf80();
    virtual void vf81();  virtual void vf82();  virtual void vf83();
    virtual void vf84();  virtual void vf85();  virtual void vf86();
    virtual void vf87();  virtual void vf88();  virtual void vf89();
    virtual void setState(f32 a);   // idx 90 -> vtable 0x168
};

// Player-embedded move object (CfObject at wrapper+0x3E9C) used by
// func_801999C0: vf03(int) at vtable 0xC, getPosition at 0xAC, get68 at 0x110.
class CMoveObj {
public:
    virtual void vf00();  virtual void vf01();  virtual void vf02();
    virtual void* vf03(int a);   // idx 3 -> vtable 0xC
    virtual void vf04();  virtual void vf05();  virtual void vf06();
    virtual void vf07();  virtual void vf08();  virtual void vf09();
    virtual void vf10();  virtual void vf11();  virtual void vf12();
    virtual void vf13();  virtual void vf14();  virtual void vf15();
    virtual void vf16();  virtual void vf17();  virtual void vf18();
    virtual void vf19();  virtual void vf20();  virtual void vf21();
    virtual void vf22();  virtual void vf23();  virtual void vf24();
    virtual void vf25();  virtual void vf26();  virtual void vf27();
    virtual void vf28();  virtual void vf29();  virtual void vf30();
    virtual void vf31();  virtual void vf32();  virtual void vf33();
    virtual void vf34();  virtual void vf35();  virtual void vf36();
    virtual void vf37();  virtual void vf38();  virtual void vf39();
    virtual void vf40();  virtual void vf41();  virtual void vf42();
    virtual ml::CVec3* getPosition();   // idx 43 -> vtable 0xAC
    virtual void vf44();  virtual void vf45();  virtual void vf46();
    virtual void vf47();  virtual void vf48();  virtual void vf49();
    virtual void vf50();  virtual void vf51();  virtual void vf52();
    virtual void vf53();  virtual void vf54();  virtual void vf55();
    virtual void vf56();  virtual void vf57();  virtual void vf58();
    virtual void vf59();  virtual void vf60();  virtual void vf61();
    virtual void vf62();  virtual void vf63();  virtual void vf64();
    virtual void vf65();  virtual void vf66();  virtual void vf67();
    virtual void* get68();           // idx 68 -> vtable 0x110
    virtual void vf69();  virtual void vf70();  virtual void vf71();
    virtual void vf72();  virtual void vf73();  virtual void vf74();
    virtual void vf75();  virtual void vf76();  virtual void vf77();
    virtual void vf78();  virtual void vf79();  virtual void vf80();
    virtual void vf81();  virtual void vf82();  virtual void vf83();
    virtual void vf84();  virtual void vf85();  virtual void vf86();
    virtual void vf87();  virtual void vf88();  virtual void vf89();
    virtual void vf90();  virtual void vf91();
};

} // namespace cf

// C-linkage imports used by this TU (retail names, defined in CtrlMoveBase).
extern "C" {
int  func_804BE398(void* vec, int a, int b, int c, f32 d, f32 e);
void func_800896F4(cf::CCtrlMovePC* self, ml::CVec3* a, const ml::CVec3* b);
void func_800898D4(cf::CCtrlMovePC* self, ml::CVec3* v);
int  func_804BE5A4(int a, int b);

// Additional engine helpers (retail C names).
void* func_8009ECB0();
u32  func_80174C98(void* actor, u32* outVal, u32 flags);
cf::CfGlobalSettings* getUnk80664658();
int  func_801F4ED8(void* a, void* b);
void func_800D59FC(void* a);
void func_80089990(cf::CCtrlMovePC* self);
void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
f32  FrSqrt__Q24nw4r4mathFf(f32 x);
}

// C-linkage imports for the move-state helpers (retail names).
extern "C" {
int  func_80198710(void* out, void* src, int a, f32 b, f32 c, f32 d, int e);
int  func_8019876C(void* a, void* b);
int  func_804BE348(void* a, void* b, int c, int d, int e);
void func_804BE4B4(void* out, int a);
void func_804BE4E0(void* out, int a);
int  func_804B526C(void* a, void* b, void* c, void* d, int e, int f, int g);
void func_8004B7C0(void* a, void* b);
void func_800BC3B0(void* a, f32 b);
void func_80089990(cf::CCtrlMovePC* self);
void* getPlayer__Q22cf13CfGameManagerFi(int idx);
}

extern void* lbl_eu_80665958;

// Additional sdata2 float constants used by the state functions (values from
// retail .sdata2; keep as named labels so the relocs pair with retail).
extern const f32 lbl_eu_80667B64;   // 32.0f
extern const f32 lbl_eu_80667B6C;   // 0.1f
extern const f32 lbl_eu_80667B70;   // 2.0f
extern const f32 lbl_eu_80667B74;   // 0.25f
extern const f32 lbl_eu_80667B78;   // 25.0f
extern const f32 lbl_eu_80667B7C;   // 0.6f
extern const f32 lbl_eu_80667B80;   // 100.0f
extern const f32 lbl_eu_80667B84;   // 10.0f
extern const f32 lbl_eu_80667B88;   // 16.0f
extern const f32 lbl_eu_80667B8C;   // 40.0f
extern const f32 lbl_eu_80667B90;   // 20.0f
extern const f32 lbl_eu_80667B94;   // 15.0f
extern const f32 lbl_eu_80667B98;   // 225.0f
extern const f32 lbl_eu_80667B9C;   // 40.743664f
extern const f32 lbl_eu_80667BA0;   // 0.7f
extern const f32 lbl_eu_80667BA4;   // -0.7f
extern const f32 lbl_eu_80667BD0;   // -1.0f
extern const f32 lbl_eu_80667C04;   // 9.0f
extern const f32 lbl_eu_80667C08;   // 0.3f
extern const f32 lbl_eu_80667C14;   // 0.333f
extern const f32 lbl_eu_80667C18;   // 1.2f
extern const f32 lbl_eu_80667C1C;   // 10.889999f
extern const f32 lbl_eu_80667C20;   // 5.29f
extern const f32 lbl_eu_8066A200;   // 1.5707964f (pi/2)

extern const u32 lbl_eu_80663E28;    // .sbss global flag word

// ptmf constants for state transitions (12-byte ptr-to-member triples).
extern int (cf::CCtrlMovePC::*const lbl_eu_80532CA4)();
extern int (cf::CCtrlMovePC::*const lbl_eu_80532CB0)();

// nw4r assert strings.
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// C++-linkage global (mangles to func_8006EF04__Fi).
int func_8006EF04(int r3);

// Result block filled by func_8019876C (CfPartyInfo at stack +0x128): the
// goal position is the leading x/y/z triple (y doubles as mGoalY).
struct CfPartyInfoOut {
    f32 x;               // 0x00
    f32 y;               // 0x04 (clamped goal height)
    f32 z;               // 0x08
    u8 pad_0C[0x58];     // 0x0C..0x64
    f32 mField64;        // 0x64
    u8 pad_68[0x4];      // 0x68..0x6C
    u8 mField6C;         // 0x6C
    u8 mField6E;         // 0x6E
    u8 pad_70[0x24];     // 0x70..0x94
    f32 mField94;        // 0x94
    u8 pad_98[0x4];      // 0x98..0x9C
    u8 mField9C;         // 0x9C
    u8 mField9E;         // 0x9E
};

// Input block for func_80198710 (party-info builder).
struct CfPartyInfoIn {
    u8 pad_00[0x2E];
    u8 mField2E;         // 0x2E
};

// The 20 retail functions (placeholder symbols). Implemented in CtrlMovePC.cpp.
extern "C" {
cf::CCtrlMovePC* __ct__801993C4(cf::CCtrlMovePC* self, void* baseParam, void* obj);
void func_8019956C(cf::CCtrlMovePC* self);
void func_80199618(cf::CCtrlMovePC* self);
void func_80199678(cf::CCtrlMovePC* self, int flag);
void func_80199810(cf::CCtrlMovePC* self, const Vec* pos);
int  func_801999C0(cf::CCtrlMovePC* self);
void func_8019A9C4(cf::CCtrlMovePC* self);
int  func_8019B4F0(cf::CCtrlMovePC* self);
int  func_8019C0D4(cf::CCtrlMovePC* self);
void func_8019C304(cf::CCtrlMovePC* self);
int  func_8019CCDC(cf::CCtrlMovePC* self);
int  func_8019CDA0(cf::CCtrlMovePC* self);
void func_8019D9E0(cf::CCtrlMovePC* self);
void func_8019DD54(cf::CCtrlMovePC* self);
void func_8019E710(cf::CCtrlMovePC* self);
int  func_8019EDAC(cf::CCtrlMovePC* self);
int  func_8019EE08(cf::CCtrlMovePC* self);
int  func_8019EEB8(cf::CCtrlMovePC* self);
void func_8019EF90(cf::CCtrlMovePC* self);
void func_8019F1E0(cf::CCtrlMovePC* self);
}
