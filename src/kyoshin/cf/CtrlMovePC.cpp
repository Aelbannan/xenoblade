// Translation unit for kyoshin/cf/CtrlMovePC
//
// CCtrlMovePC - player-character movement controller. A state machine driven
// by a pointer-to-member-function (ptmf) at +0x108, dispatched each tick.
// Reconstructed from retail assembly (build/us/asm/kyoshin/cf/CtrlMovePC.s).
//
// All 20 functions use placeholder retail symbols (extern "C") so hexdiff /
// objdiff can pair them exactly with the retail object.

#include "kyoshin/cf/CtrlMovePC.hpp"
#include "kyoshin/cf/CtrlMovePC_intf.hpp"
#include <monolib/math/CVec3.hpp>

using cf::CCtrlMovePC;
using cf::CMoveWrapper;

// ---- external runtime / engine symbols (exact retail names) ----
extern "C" {

// Base class ctor (CCtrlMoveBase) and helpers.
void __ct__80088904(CCtrlMovePC* self, void* baseParam);
void func_80089684(CCtrlMovePC* self);
void func_8008962C(CCtrlMovePC* self);
void func_80089694(CCtrlMovePC* self, const Vec* v, f32 f);

// ptmf runtime.
extern int (CCtrlMovePC::*const __ptmf_null)();
long __ptmf_test(PTMF* ptmf);
void __ptmf_scall(...);

// Engine helpers.
void* func_8016FE34(void* obj);
void* func_80083298(void);                       // cf::CfGameManager accessor
void  func_8047CF20(void* unk, void* task);      // UnkClass_8047CD0C::func
void* func_8047CE7C(void);                       // UnkClass_8047CD0C::func
void  func_8047DE14(void* a, Vec* b, f32 c, f32 d);
void  func_8047DD4C(void* a, Vec* b, void* c, f32 d, f32 e, int f);
void* getInstance__Q22cf14CBattleManagerFv(void);
f32 SinFIdx__Q24nw4r4mathFf(f32);
f32 CosFIdx__Q24nw4r4mathFf(f32);

// Data labels (vtables / ptmf constants).
extern const char lbl_eu_80532D58[];  // secondary vtable
extern int (CCtrlMovePC::*const lbl_eu_80532B60)();
extern int (CCtrlMovePC::*const lbl_eu_80532B78)();
extern int (CCtrlMovePC::*const lbl_eu_80532B84)();
extern int (CCtrlMovePC::*const lbl_eu_80532C14)();
extern int (CCtrlMovePC::*const lbl_eu_80532C20)();
extern int (CCtrlMovePC::*const lbl_eu_80532C98)();
extern int (CCtrlMovePC::*const lbl_eu_80532D28)();
extern int (CCtrlMovePC::*const lbl_eu_80532D34)();
extern int (CCtrlMovePC::*const lbl_eu_80532D40)();
extern int (CCtrlMovePC::*const lbl_eu_80532D4C)();

extern const f32 lbl_eu_80667B60;  // 0.0f
extern const f32 lbl_eu_80667B68;
extern const f32 lbl_eu_80667B9C;
extern const f32 lbl_eu_80667C40;
}

// ============================================================================
// __ct__801993C4 - constructor
// ============================================================================
extern "C" CCtrlMovePC* __ct__801993C4(CCtrlMovePC* self, void* baseParam, void* obj) {
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
    return self;
}

// ============================================================================
// func_80199618 - release active task, reset base (0x60 bytes)
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
// func_8019EDAC - tick counter; on wrap set data flag + transition (0x5C)
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
// func_8019EE08 - flag-driven state transition + counter reset (0xB0)
// ============================================================================
extern "C" int func_8019EE08(CCtrlMovePC* self) {
    u32 f = self->mFlags4C;
    if ((f & 0x00200000u) == 0) {      // rlwinm. bit 10
        self->mStateFunc = lbl_eu_80532D34;
    } else {
        if (self->mDistFC >= lbl_eu_80667C40 && self->mArr124[17] <= 0) {
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

// ============================================================================
// func_8019CCDC - facing sin/cos update or ptmf transition (0xC4)
// ============================================================================
extern "C" int func_8019CCDC(CCtrlMovePC* self) {
    if (self->mFlags4C & 0x00040000u) {        // rlwinm. bit 13
        self->mVec90.y = lbl_eu_80667B60;      // 0x94 = 0.0
        CMoveWrapper* w = (CMoveWrapper*)self->mObject;
        f32 s = w->getAngle();                 // vtable 0x5b4
        self->mVec90.x = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * s);
        f32 c = ((CMoveWrapper*)self->mObject)->getAngle();
        self->mVec90.z = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * c);
        func_80089694(self, (const Vec*)&self->mVec90, lbl_eu_80667B68);
    } else {
        self->mStateFunc = lbl_eu_80532C98;
        *(f32*)((char*)self->mBaseData + 0x14) = lbl_eu_80667B60;
    }
    return 0;
}

// ============================================================================
// func_8019EEB8 - counter-gated facing update or transition (0xD8)
// ============================================================================
extern "C" int func_8019EEB8(CCtrlMovePC* self) {
    s16 old = self->mArr124[15];               // 0x142
    self->mArr124[15] = (s16)(old + 1);
    if (old < 30) {
        CMoveWrapper* w = (CMoveWrapper*)self->mObject;
        void* sub = *(void**)((char*)w + 0x3f60);
        u16 flag = *(u16*)((char*)sub + 0x530);
        if ((flag & 1u) == 0) {                // clrlwi. bit 31
            f32 a = w->getAngle();             // vtable 0x5b4
            self->mVec90.x = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * a);
            f32 b = ((CMoveWrapper*)self->mObject)->getAngle();
            self->mVec90.z = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * b);
            func_80089694(self, (const Vec*)&self->mVec90, lbl_eu_80667B68);
            return 0;
        }
    }
    self->mArr124[15] = 0;
    self->mStateFunc = lbl_eu_80532D4C;
    return 0;
}

// ============================================================================
// func_8019956C - top-level tick: query object, gate on battle state, run the
// state-machine dispatch loop (0xAC).
// ============================================================================
extern "C" void func_8019956C(CCtrlMovePC* self) {
    cf::CMoveEmbedded* emb = (cf::CMoveEmbedded*)((char*)self->mObject + 0x3e9c);
    f32 v = emb->getF35();                       // vtable 0x8c
    if (v == lbl_eu_80667B60) {                  // == 0.0
        return;
    }
    u8 b = *(u8*)((char*)getInstance__Q22cf14CBattleManagerFv() + 0x1aa);
    if (b >= 1 && b <= 0x18) {
        return;
    }
    if (func_801999C0(self) == 0) {
        return;
    }
    if (!self->mStateFunc) {                     // __ptmf_test
        return;
    }
    while ((self->*self->mStateFunc)()) {        // __ptmf_scall loop
    }
}

// Zero the 18-short array at 0x124..0x146 (retail emits 18 explicit sth).
static inline void zeroArr(CCtrlMovePC* s) {
    s->mArr124[17]=0; s->mArr124[16]=0; s->mArr124[15]=0; s->mArr124[14]=0;
    s->mArr124[13]=0; s->mArr124[12]=0; s->mArr124[11]=0; s->mArr124[10]=0;
    s->mArr124[9]=0;  s->mArr124[8]=0;  s->mArr124[7]=0;  s->mArr124[6]=0;
    s->mArr124[5]=0;  s->mArr124[4]=0;  s->mArr124[3]=0;  s->mArr124[2]=0;
    s->mArr124[1]=0;  s->mArr124[0]=0;
}

// Release mTask through the game manager (shared by reset paths).
static inline void releaseTask(CCtrlMovePC* s) {
    void* gm = func_80083298();
    if (gm != 0) {
        void* p = (char*)gm + 0x2f2c;
        if (p != 0) {
            func_8047CF20(p, s->mTask);
        }
    }
    s->mTask = 0;
}

// ============================================================================
// func_80199678 - conditional full reset + task release (0x198)
// ============================================================================
extern "C" void func_80199678(CCtrlMovePC* self, int flag) {
    if (flag != 0) {
        func_80089684(self);
        self->mStateFunc = lbl_eu_80532B78;
        func_8008962C(self);
        self->mFlags4C = 0x80000000u;
        self->mFlags50 = 0;
        void* task = self->mTask;
        self->mPos   = ml::CVec3::zero;
        self->mVec6C = ml::CVec3::zero;
        self->mVecA8 = ml::CVec3::zero;
        self->mVecD8 = ml::CVec3::zero;
        self->mVecB4 = ml::CVec3::zero;
        self->mVec90 = ml::CVec3::zero;
        self->mFloat100 = lbl_eu_80667B60;
        zeroArr(self);
        if (task != 0) {
            releaseTask(self);
        }
        self->mFlags4C = 0x80000100u;
    } else {
        self->mFlags4C |= 0x100u;
    }
}

// ============================================================================
// func_80199810 - reset (gated on flag bit) then set position (0x1B0)
// ============================================================================
extern "C" void func_80199810(CCtrlMovePC* self, const Vec* pos) {
    if (self->mFlags4C & 0x00800000u) {        // rlwinm. bit 8
        func_80089684(self);
        self->mStateFunc = lbl_eu_80532B84;
        func_8008962C(self);
        self->mFlags4C = 0x80000000u;
        self->mFlags50 = 0;
        void* task = self->mTask;
        self->mPos   = ml::CVec3::zero;
        self->mVec6C = ml::CVec3::zero;
        self->mVecA8 = ml::CVec3::zero;
        self->mVecD8 = ml::CVec3::zero;
        self->mVecB4 = ml::CVec3::zero;
        self->mVec90 = ml::CVec3::zero;
        self->mFloat100 = lbl_eu_80667B60;
        zeroArr(self);
        if (task != 0) {
            releaseTask(self);
        }
    }
    self->mVec6C = *(const ml::CVec3*)pos;
    self->mPos   = *(const ml::CVec3*)pos;
}
