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

// CCtrlMovePC — player-character movement controller (state machine).
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
    char mPadE4[0x18];         // 0xE4..0xFB
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

} // namespace cf

// The 20 retail functions (placeholder symbols). Implemented in CtrlMovePC.cpp.
extern "C" {
void __ct__801993C4(cf::CCtrlMovePC* self, void* baseParam, void* obj);
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
void func_8019CDA0(cf::CCtrlMovePC* self);
void func_8019D9E0(cf::CCtrlMovePC* self);
void func_8019DD54(cf::CCtrlMovePC* self);
void func_8019E710(cf::CCtrlMovePC* self);
int  func_8019EDAC(cf::CCtrlMovePC* self);
int  func_8019EE08(cf::CCtrlMovePC* self);
int  func_8019EEB8(cf::CCtrlMovePC* self);
void func_8019EF90(cf::CCtrlMovePC* self);
void func_8019F1E0(cf::CCtrlMovePC* self);
}
