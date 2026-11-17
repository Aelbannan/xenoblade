#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include <monolib/math/CVec3.hpp>
#include "kyoshin/code_801F3BE0.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"

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
// Layout recovered from the retail ctor and the state update functions.
class __declspec(novtable) CCtrlMovePC : public CCtrlMoveBase {
public:
    CCtrlMovePC(void* owner, void* obj);

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
    ml::CVec3 mVecE4;          // 0xE4 (recovered from movePcSteerToGoal)
    ml::CVec3 mVecF0;          // 0xF0 follow-mode candidate spot
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

// Move-data block referenced by CCtrlMovePC::mpDataPtr.
struct CfMoveData {
    u8 pad_00[0x4];
    u32 mField04;             // 0x04
    u8 pad_08[0xC];
    f32 mField14;             // 0x14
    u8 pad_18[0x10];          // 0x18..0x27
    void* field_28;           // 0x28 embedded move-object (vtable 0xAC = getPosition)
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

 
// State sub-object returned by CfObject::UVF48 (void* at vtable 0x110 on the
// embedded move object; f32 at +0x14, s16 at +0x1AC).
struct CfObjState {
    u8 pad_00[0x14];
    f32 mField14;             // 0x14
    u8 pad_18[0x1AC - 0x18];
    s16 mField1AC;            // 0x1AC
};

// Object referenced by CCtrlMovePC::mpSomePtr (u16 at +0x22).
struct CfBase30Obj {
    u8 pad_00[0x22];
    u16 mField22;             // 0x22
};

// Battle manager singleton (s16 at +0x20C8).
struct CfBattleState {
    u8 pad_00[0x20C8];
    s16 mField20C8;           // 0x20C8
};

// Party-slot list returned by CtrlObjectParam_GetSlotTableBase: two character ids.
struct CfPartyList {
    u8 pad_00[0x8];
    u32 mField08;             // 0x08 first slot character id
    u32 mField0C;             // 0x0C second slot character id
};

// Embedded move-object slots (object at CfObjWrap +0x3E9C, a CfObjectMove) are
// called as real virtuals; retail slot map (verified against the CfObjectPc
// vtable group at lbl_eu_80529DA0):
//   +0x8C CfObject::UVF15 (float)      +0xAC CfObject::UVF23 (CVec3*)
//   +0xA8 CfObject::UVF22 (const vec*) +0xC8 CfObject::UVF30 (float)
//   +0x110 CfObject::UVF48 (void*)     +0x168 CfObject::UVF70 (float)
//   +0x0C CObjectState::UVF2 (int)     +0x4C CObjectParam::UVF5 (BOOL)
// Full-actor slots: +0x5B4 CfObjectActor::UVF6, +0x128 CActorParam::UVF37.

} // namespace cf

// C-linkage imports used by this TU (retail names, defined in CtrlMoveBase).
extern "C" {
int  ScnRes_VertRayForward_E398(void* vec, int a, int b, int c, f32 d, f32 e);
void PSVECNormalize(const Vec* a, Vec* b);
int  moveBaseUpdate(void* self, ml::CVec3* out, ml::CVec3* pos, int a, int b);
void moveBaseChase(cf::CCtrlMovePC* self, ml::CVec3* a, const ml::CVec3* b);
void moveBaseLerp(cf::CCtrlMovePC* self, ml::CVec3* v);
int  ScnRes_EntryFlagThunk_E5A4(int a, int b);

// Additional engine helpers (retail C names).
void* CtrlObjectParam_GetSlotTableBase();
void ctrlActUpdateData70(void* a);
void maskMoveChildFlags(cf::CCtrlMovePC* self);
int  moveBaseRoute(cf::CCtrlMovePC* self, ml::CVec3* out, int flag);
void Warning__Q24nw4r2dbFPCciPCce(const char* file, int line, const char* fmt, ...);
}

// C-linkage imports for the move-state helpers (retail names).
extern "C" {
int  InitRecord(void* out, void* src, int a, f32 b, f32 c, f32 d, int e);
int  CPartsChange_ProcessPartyInfo(void* a, void* b);
int  ScnRes_SegQueryForward_E348(void* a, void* b, int c, int d, int e);
void ScnRes_CopyEntryHead(void* out, int a);
void ScnRes_CopyEntryMid(void* out, int a);
int  ColiWalkSegment(void* a, void* b, void* c, void* d, int e, int f, void* g);
int  Coli_SweepSegNodes(void* a, void* b, void* c, int d, int e);
void* findObjectById__Fi(int id);
void* setAnimPosVec(void* out, const ml::CVec3* src); // matches CfGameManager.hpp decl
void CfObjectMove_setMoveSpeedGated(void* a, f32 b);
void maskMoveChildFlags(cf::CCtrlMovePC* self);
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
extern const f32 lbl_eu_80667C24;
extern const f32 lbl_eu_80667BAC;   // goal-proximity squared threshold
extern const f32 lbl_eu_80667BD4;   // probe height offset
extern const f32 lbl_eu_80667BCC;   // arrival radius squared (nudge path)
extern const f32 lbl_eu_80667BE0;   // slope gate
extern const f64 lbl_eu_80667BE8;   // probe angle bias (double)
extern const f32 lbl_eu_80667C44;   // arrival radius squared
extern const f32 lbl_eu_80667C48;   // raycast param
extern const f32 lbl_eu_80667C4C;   // raycast param
extern const f32 lbl_eu_80667C50;   // max follow distance

// lbl_eu_80663E28 is declared by kyoshin/cf/object/CfObjectMove.hpp (canonical
// non-const form); do not redeclare here.

// Wall-steer helper shared with CtrlMoveEne (retail name).
extern u8 lbl_eu_80571810[0x38];

// ptmf constants for state transitions (12-byte ptr-to-member triples).
extern int (cf::CCtrlMovePC::*const lbl_eu_80532CA4)();
extern int (cf::CCtrlMovePC::*const lbl_eu_80532CB0)();
extern int (cf::CCtrlMovePC::*const lbl_eu_80532CBC)();

// nw4r assert strings.
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// C++-linkage global (mangles to isGlobalCamFlagSet__Fi).
int isGlobalCamFlagSet(int r3);

// Node-pool manager shared with the scene path classes
// (libs/monolib/src/scn/UnkClass_8047CD0C.cpp). Only the task-allocating
// accessor is needed here; the definition lives in its own TU.
class UnkClass_8047CD0C {
public:
    void* allocFreeNode();
};

// Result block filled by CPartsChange_ProcessPartyInfo (CfPartyInfo at stack +0x128): the
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

// Input block for InitRecord (party-info builder).
struct CfPartyInfoIn {
    u8 pad_00[0x24];
    f32 mField24;        // 0x24 candidate height
    u8 pad_28[2];
    u8 mField2C;         // 0x2C valid-height flag
    u8 mField2D;
    u8 mField2E;         // 0x2E mode byte
};

// The 20 retail functions (placeholder symbols). Implemented in CtrlMovePC.cpp.
extern "C" {
void movePcTopTickDispatch(cf::CCtrlMovePC* self);
void movePcReleaseTaskReset(cf::CCtrlMovePC* self);
void movePcCondFullReset(void* ctrl, int flag); // matches CfGameManager.hpp / CfGimmickObject.hpp
void movePcResetAndSetPos(cf::CCtrlMovePC* self, const Vec* pos);
int  movePcUpdateTop(cf::CCtrlMovePC* self);
void func_8019A9C4(cf::CCtrlMovePC* self);
int  movePcUpdateWalk(cf::CCtrlMovePC* self);
int  movePcEnterApproach(cf::CCtrlMovePC* self);
int  movePcUpdateApproach(cf::CCtrlMovePC* self);
int  movePcFacingSinCosUpdate(cf::CCtrlMovePC* self);
int  movePcSteerToGoal(cf::CCtrlMovePC* self);
int  movePcUpdateFollow(cf::CCtrlMovePC* self);
int  movePcUpdateReposition(cf::CCtrlMovePC* self);
void movePcTickState(cf::CCtrlMovePC* self);
int  movePcTickCounterWrap(cf::CCtrlMovePC* self);
int  movePcFlagTransitionReset(cf::CCtrlMovePC* self);
int  movePcCounterFacingUpdate(cf::CCtrlMovePC* self);
f32 movePcProbeGoalAngle(cf::CCtrlMovePC* self);
void movePcUpdatePairFollow(cf::CCtrlMovePC* self);
}
