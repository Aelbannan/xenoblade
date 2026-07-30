// Translation unit for kyoshin/cf/CtrlMovePC
//
// CCtrlMovePC — player-character movement controller. A state machine driven
// by a pointer-to-member-function (ptmf) at +0x108, dispatched each tick.
// Reconstructed from retail assembly (build/us/asm/kyoshin/cf/CtrlMovePC.s).
//
// All 20 functions use placeholder retail symbols (extern "C") so hexdiff /
// objdiff can pair them exactly with the retail object.

#include "kyoshin/cf/CtrlMovePC.hpp"
#include <monolib/math/CVec3.hpp>

using cf::CCtrlMovePC;

// ---- external runtime / engine symbols (exact retail names) ----
extern "C" {

// Base class ctor (CCtrlMoveBase) and helpers.
void __ct__80088904(CCtrlMovePC* self, void* baseParam);
void func_80089684(CCtrlMovePC* self);
void func_8008962C(CCtrlMovePC* self);
void func_80089694(CCtrlMovePC* self, const Vec* v, f32 f);

// ptmf runtime.
extern const PTMF __ptmf_null;
long __ptmf_test(PTMF* ptmf);
void __ptmf_scall(...);

// Engine helpers.
void* func_8016FE34(void* obj);
void* func_80083298(void);                       // cf::CfGameManager accessor
void  func_8047CF20(void* unk, void* task);      // UnkClass_8047CD0C::func
void* func_8047CE7C(void);                       // UnkClass_8047CD0C::func
void  func_8047DE14(void* a, Vec* b, f32 c, f32 d);
void  func_8047DD4C(void* a, Vec* b, void* c, f32 d, f32 e, int f);

// Data labels (vtables / ptmf constants).
extern const char lbl_eu_80532D58[];  // secondary vtable
extern const PTMF lbl_eu_80532B60;
extern const PTMF lbl_eu_80532B78;
extern const PTMF lbl_eu_80532B84;
extern const PTMF lbl_eu_80532C14;
extern const PTMF lbl_eu_80532C20;
extern const PTMF lbl_eu_80532C98;
extern const PTMF lbl_eu_80532D28;
extern const PTMF lbl_eu_80532D34;
extern const PTMF lbl_eu_80532D40;
extern const PTMF lbl_eu_80532D4C;

extern const f32 lbl_eu_80667B60;  // 0.0f
}

// ============================================================================
// __ct__801993C4 — constructor
// ============================================================================
extern "C" void __ct__801993C4(CCtrlMovePC* self, void* baseParam, void* obj) {
    __ct__80088904(self, baseParam);

    self->mBaseVtable2 = (void*)lbl_eu_80532D58;

    self->mStateFunc = __ptmf_null;
    func_80089684(self);

    self->mStateFunc = lbl_eu_80532B60;
    func_8008962C(self);

    self->mFlags4C = 0x80000000u;
    self->mFlags50 = 0;

    self->mPos   = ml::CVec3::zero;
    self->mVec6C = ml::CVec3::zero;
    self->mVecA8 = ml::CVec3::zero;
    self->mVecD8 = ml::CVec3::zero;
    self->mVecB4 = ml::CVec3::zero;
    self->mVec90 = ml::CVec3::zero;

    self->mFloat100 = lbl_eu_80667B60;

    self->mArr124[17] = 0;
    self->mArr124[16] = 0;
    self->mArr124[15] = 0;
    self->mArr124[14] = 0;
    self->mArr124[13] = 0;
    self->mArr124[12] = 0;
    self->mArr124[11] = 0;
    self->mArr124[10] = 0;
    self->mArr124[9]  = 0;
    self->mArr124[8]  = 0;
    self->mArr124[7]  = 0;
    self->mArr124[6]  = 0;
    self->mArr124[5]  = 0;
    self->mArr124[4]  = 0;
    self->mArr124[3]  = 0;
    self->mArr124[2]  = 0;
    self->mArr124[1]  = 0;
    self->mArr124[0]  = 0;

    self->mObject = func_8016FE34(obj);
    self->mShort120 = 1;
    self->mShort122 = 1;
    self->mTask = 0;
}

// ============================================================================
// func_80199618 — release active task, reset base (0x60 bytes)
// ============================================================================
extern "C" void func_80199618(CCtrlMovePC* self) {
    if (self->mTask != 0) {
        void* gm = func_80083298();
        if (gm != 0) {
            void* p = (char*)gm + 0x2f2c;
            if (p != 0) {
                func_8047CF20(p, self->mTask);
            }
        }
        self->mTask = 0;
    }
    func_8008962C(self);
}

// ============================================================================
// func_8019EDAC — tick counter; on wrap set data flag + transition (0x5C)
// ============================================================================
extern "C" int func_8019EDAC(CCtrlMovePC* self) {
    s16 v = self->mArr124[1];          // 0x126
    self->mArr124[1] = (s16)(v + 1);
    if (v >= 8) {
        self->mArr124[1] = 0;
        u32* d = (u32*)self->mBaseData;
        d[1] |= 1u;                    // *(data+4) |= 1
        self->mStateFunc = lbl_eu_80532D28;
    }
    *(f32*)((char*)self->mBaseData + 0x14) = lbl_eu_80667B60;
    return 0;
}

// ============================================================================
// func_8019EE08 — flag-driven state transition + counter reset (0xB0)
// ============================================================================
extern "C" int func_8019EE08(CCtrlMovePC* self) {
    u32 f = self->mFlags4C;
    if ((f & 0x00200000u) == 0) {      // rlwinm. bit 10
        self->mStateFunc = lbl_eu_80532D34;
    } else {
        extern const f32 lbl_eu_80667C40;
        if (!(self->mDistFC >= lbl_eu_80667C40) && !(self->mArr124[17] > 0)) {
            self->mFlags4C = (f & 0xFFFFFEFFu) | 0x8u;
            self->mStateFunc = lbl_eu_80532D40;
            self->mArr124[7] = 0;      // 0x132
        }
    }
    u32 g = self->mFlags4C;
    self->mArr124[2] = 0;              // 0x128
    g |= 0x01000000u;                  // oris 0x100
    self->mArr124[1] = 0;              // 0x126
    g &= 0xE1FFFFFFu;                  // rlwinm 0,7,2
    self->mArr124[3] = 0;              // 0x12a
    self->mFlags4C = g;
    *(f32*)((char*)self->mBaseData + 0x14) = lbl_eu_80667B60;
    return 0;
}
