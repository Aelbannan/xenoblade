// Translation unit for kyoshin/cf/CtrlMovePC
//
// CCtrlMovePC - player-character movement controller. A state machine driven
// by a pointer-to-member-function (ptmf) at +0x108, dispatched each tick.
// Reconstructed from retail assembly (build/us/asm/kyoshin/cf/CtrlMovePC.s).
//
// All 20 functions use placeholder retail symbols (extern "C") so hexdiff /
// objdiff can pair them exactly with the retail object.

#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/CtrlMovePC.hpp"
#include "kyoshin/cf/CtrlMovePC_intf.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"
#include <nw4r/math/math_types.h>
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
void  func_8047CF20__17UnkClass_8047CD0CFv(void* unk, void* task); // UnkClass_8047CD0C::func
void* func_8047CE7C__17UnkClass_8047CD0CFv(void);                       // UnkClass_8047CD0C::func
int   func_8047DE14__17UnkClass_8047D2ACFv(void* a, Vec* b, f32 c, f32 d);
int   walkPathCheck__17UnkClass_8047D2ACFv(void* a, Vec* b, void* c, f32 d, f32 e, int f);
// getInstance__Q22cf14CBattleManagerFv comes from CfGameManager.hpp
// (extern "C" CBattleManagerView* form) - do not redeclare here.
f32 SinFIdx__Q24nw4r4mathFf(f32);
f32 CosFIdx__Q24nw4r4mathFf(f32);
f32 Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);

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
extern const f32 lbl_eu_80667BB4;
extern const f32 lbl_eu_80667BB8;
extern const f32 lbl_eu_80667B9C;
extern const f32 lbl_eu_80667C40;
extern const f32 lbl_eu_80667B70;
extern const f32 lbl_eu_80667B90;
extern const f32 lbl_eu_80667BA8;
extern const f32 lbl_eu_80667BC0;
extern const f32 lbl_eu_80667BC4;
extern const f32 lbl_eu_80667BD0;
extern const f32 lbl_eu_80667BDC;
extern const f32 lbl_eu_80667BD8;
extern const f32 lbl_eu_80667BE4;
extern const f32 lbl_eu_80667C04;
extern const f32 lbl_eu_80667C10;
extern const f32 lbl_eu_80667C34;
extern const f32 lbl_eu_80667C38;
extern const f32 lbl_eu_80667C3C;
extern const f32 lbl_eu_8066AF20;
extern const f32 lbl_eu_8066A204;
extern const f32 lbl_eu_8066A210;
extern const f32 lbl_eu_80667B6C;
extern const f32 lbl_eu_80667C28;
extern const f32 lbl_eu_80667C2C;
extern const f32 lbl_eu_80667C30;
extern const f32 lbl_eu_80667BBC;
extern const f32 lbl_eu_80667BC8;
extern const f32 lbl_eu_80667BB0;
extern const f32 lbl_eu_80667BF0;
extern const f32 lbl_eu_80667BF4;
extern const f32 lbl_eu_80667BF8;
extern const f32 lbl_eu_80667BFC;
extern const f32 lbl_eu_80667C00;
extern const f32 lbl_eu_80667C0C;
extern int (CCtrlMovePC::*const lbl_eu_80532CC8)();
extern int (CCtrlMovePC::*const lbl_eu_80532CD4)();
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
        void* gm = cf::CfGameManager::func_80083298();
        if (gm != 0) {
            void* p = (char*)gm + 0x2f2c;
            if (p != 0) {
                func_8047CF20__17UnkClass_8047CD0CFv(p, self->mTask);
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
    f32 v = emb->getF35();
    if (lbl_eu_80667B60 == v) {
        return;
    }

    // Ternary materialization: MWCC emits li 0 / cmplwi / blt / cmplwi /
    // bgt / li 1 for the nested ternary form (avoids the range-fold).
    int battleState = *(u8*)((char*)getInstance__Q22cf14CBattleManagerFv() + 0x1aa);
    bool inBattle = (u32)battleState >= 1 && (u32)battleState <= 0x18;
    if (inBattle) {
        return;
    }

    if (func_801999C0(self) == 0) {
        return;
    }
    if (!self->mStateFunc) {
        return;
    }
    while ((self->*self->mStateFunc)()) {
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
    void* gm = cf::CfGameManager::func_80083298();
    if (gm != 0) {
        void* p = (char*)gm + 0x2f2c;
        if (p != 0) {
            func_8047CF20__17UnkClass_8047CD0CFv(p, s->mTask);
        }
    }
    s->mTask = 0;
}

// ============================================================================
// func_80199678 - conditional full reset + task release (0x198)
// ============================================================================
extern "C" void func_80199678(void* selfv, int flag) {
    CCtrlMovePC* self = (CCtrlMovePC*)selfv;
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

// ============================================================================
// func_8019A9C4 - main per-tick movement controller (0xB14 bytes)
//
// One-shot entry reset (flag bit 0x100): release the move task, clear the
// counters, reset the follow anchor and re-arm the condition masks. Then the
// proximity checks against the party leader (idle transition on contact),
// the per-tick position/facing refresh, the four condition queries feeding
// the move-sub flag word, the battle-mode bail-out, the follow-anchor book-
// keeping and finally the party-spot planning + move-task commit.
// ============================================================================
extern "C" void func_8019A9C4(cf::CCtrlMovePC* self) {
    cf::CfObjectMove* p0 = cf::CfGameManager::getPlayer(0);
    self->mPlayer = (p0 != 0) ? (char*)p0 - 0x3e9c : 0;
    if ((self->mFlags4C & 0x100u) != 0) {
    // --- one-shot reset ---
    func_80089990(self);
    if (self->mTask != 0) {
        void* gm = cf::CfGameManager::func_80083298();
        if (gm != 0) {
            void* p = (char*)gm + 0x2f2c;
            if (p != 0) {
                func_8047CF20__17UnkClass_8047CD0CFv(p, self->mTask);
            }
        }
        self->mTask = 0;
    }
    self->mBase38 = lbl_eu_80667BA8;
    self->mArr124[0] = 0;
    self->mArr124[1] = 0;
    self->mArr124[2] = 0;
    self->mArr124[3] = 0;
    self->mFloat100 = lbl_eu_80667B60;
    self->mArr124[9] = 0;
    self->mArr124[10] = 0;
    self->mArr124[13] = 0;
    self->mFlags50 = 0;
    if (self->mObject != 0) {
        ((cf::CfObjWrap*)self->mObject)->mField455A = 100;
    }
    self->mBase3C = lbl_eu_80667B74;
    self->mFlags4C &= 0xE1634F1Fu;
    if (self->mPlayer != 0) {
    // --- proximity gate + contact handling ---
    // Retail control flow: every outcome here converges on the main per-tick
    // section; passing the checks merely sets the contact flag bit 0x80.
    {
        ml::CVec3* pl = ((cf::CMovePosIntf*)((char*)self->mPlayer + 0x3e9c))->getPosition();
        ml::CVec3* ob = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        ml::CVec3 d = *pl - *ob;
        if (!(d.x * d.x + d.y * d.y + d.z * d.z <= lbl_eu_80667BAC
              || ml::math::abs(d.y) <= lbl_eu_80667BB0)) {
            // Busy-state query twice: resolve an entity handle and run a
            // second proximity pass against it.
            if (((cf::CMoveEmbedded*)((char*)self->mPlayer + 0x3e9c))->get19() == 0) {
                self->mFlags4C |= 0x80u;
            } else {
                int id = (int)((cf::CMoveEmbedded*)((char*)self->mPlayer + 0x3e9c))->get19();
                void* ent = func_8016FE34(func_800B708C__Fi(id));
                if (ent == 0) {
                    self->mFlags4C |= 0x80u;
                } else {
                    ml::CVec3* pl2 = ((cf::CMovePosIntf*)((char*)self->mPlayer + 0x3e9c))->getPosition();
                    ml::CVec3* eb = ((cf::CMovePosIntf*)((char*)ent + 0x3e9c))->getPosition();
                    ml::CVec3 e = *pl2 - *eb;
                    if (e.x * e.x + e.y * e.y + e.z * e.z <= lbl_eu_80667BAC
                        || ml::math::abs(e.y) <= lbl_eu_80667BB0) {
                        self->mFlags4C |= 0x80u;
                    }
                }
            }
        }
    }
    }

    // === main per-tick section ===
    if (self->mPlayer == 0) {
        return;
    }
    cf::CfObjWrap* objW = (cf::CfObjWrap*)self->mObject;
    if (objW == 0 || objW->mSub == 0) {
        return;
    }

    self->mVec6C = self->mPos;
    self->mPos = *((cf::CMovePosIntf*)((char*)self->mPlayer + 0x3e9c))->getPosition();
    self->mFloat100 = ((cf::CMoveWrapper*)self->mPlayer)->getAngle();   // vtable 0x5b4
    if (self->mFlags4C & 0x80000000u) {
        self->mFlags4C &= 0x7FFFFFFFu;
        self->mVec6C = self->mPos;
    }

    // --- four condition queries against the object's ctrl state ---
    int condA = 1;   // r27
    int condB = 0;   // r30
    int condC = 1;   // r31
    {
        u32 q;
        q = *(u32*)((cf::CMoveEmbedded*)objW->mField04)->getCtrl();
        int hit10a = func_80174C98(objW, (int*)&q, 0xa);
        if (hit10a == 0) {
            q = *(u32*)((cf::CMoveEmbedded*)objW->mField04)->getCtrl();
            if (func_80174C98(objW, (int*)&q, 0x9) != 0) {
                condC = 0;
            }
        }
        q = *(u32*)((cf::CMoveEmbedded*)objW->mField04)->getCtrl();
        if (func_80174C98(objW, (int*)&q, 0x1a) != 0) {
            condA = 0;
        } else {
            q = *(u32*)((cf::CMoveEmbedded*)objW->mField04)->getCtrl();
            if (func_80174C98(objW, (int*)&q, 0x19) == 0 && condC != 0) {
                condB = 1;
            }
        }
    }

    // --- mirror the condition results into the move-sub flag word ---
    cf::CfMoveSub* sub = objW->mSub;
    if (condA != 0) {
        sub->mField4EC |= 0x20u;
    } else {
        sub->mField4EC &= ~0x20u;
    }
    sub = ((cf::CfObjWrap*)self->mObject)->mSub;
    if (condB != 0) {
        sub->mField4EC |= 0x200u;
    } else {
        sub->mField4EC &= ~0x200u;
    }
    sub = ((cf::CfObjWrap*)self->mObject)->mSub;
    if (condC == 0) {
        sub->mField4EC |= 0x800u;
    } else {
        sub->mField4EC &= ~0x800u;
    }

    // --- battle-mode bail-out: freeze the follow anchor ---
    cf::CfGameManager::getInstance();
    bool inBattle = func_8006EF04(0x40000000) != 0;
    if (!inBattle) {
        u8 phase = *(u8*)((char*)getInstance__Q22cf14CBattleManagerFv() + 0x1aa);
        if (!((u32)phase >= 1 && (u32)phase <= 0x18)) {
            s16 bf = ((cf::CfBattleState*)getInstance__Q22cf14CBattleManagerFv())->mField20C8;
            inBattle = (bf != 0);
        }
    }
    if (inBattle) {
        self->mVecA8 = *((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        self->mArr124[8] = 0;
        return;
    }

    // --- follow-anchor bookkeeping ---
    {
        ml::CVec3* op = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        ml::CVec3 delta = self->mVecA8 - *op;
        void* st = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->get68();
        bool anchorOk = (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z <= lbl_eu_80667B68)
                        && (((cf::CfObjState*)st)->mField14 >= lbl_eu_80667B6C)
                        && (condC == 0);
        op = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        if (!anchorOk) {
            self->mVecA8 = *op;
            self->mArr124[8] = 0;
        } else {
            ml::CVec3 d2v = self->mPos - *op;
            if (d2v.x * d2v.x + d2v.y * d2v.y + d2v.z * d2v.z < lbl_eu_80667BB4) {
                s16 c = self->mArr124[8];
                self->mArr124[8] = (s16)(c + 1);
            } else {
                self->mVecA8 = *op;
                self->mArr124[8] = 0;
            }
        }
    }

    cf::CfGlobalSettings* gs = getUnk80664658();
    if ((gs->field_214 & 0x8000u) && !(self->mFlags4C & 0x20u)) {
        ml::CVec3* pos = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        if (func_801F4ED8(gs, pos) != 0) {
            self->mFlags4C |= 0x20u;
        } else {
            self->mFlags4C |= 0x80u;
        }
    }

    // --- arrival gate: give up once the counter expires or we drift away ---
    {
        ml::CVec3* op = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        ml::CVec3 dv = self->mPos - *op;
        s16 cnt = self->mArr124[8];
        if (!(cnt < 0xD2 && dv.x * dv.x + dv.y * dv.y + dv.z * dv.z <= lbl_eu_80667BB8)) {
            if ((self->mFlags4C & 0x180u) == 0) {
                return;
            }
        }
    }

    // --- commit step: gate on the wrapper float, plan a party spot ---
    {
        f32 gate = ((cf::CMoveWrapper*)self->mObject)->getF74();   // vtable 0x128
        if (gate <= lbl_eu_80667B60) {
            return;
        }
    }
    self->mArr124[8] = 0;
    ml::CVec3 tmp = self->mPos;                  // stack +0xa8 source spot
    u32 rnd = (u32)ml::math::mtRand() & 0xffffu;
    CfPartyInfoIn info;                          // stack +0xe8 planner block
    func_80198710(&info, &tmp, 1, self->mFloat100,
                  lbl_eu_80667B70, lbl_eu_80667BA8, rnd);
    ml::CVec3 out;                               // stack +0xc0 candidate spot
    int planned = func_8019876C(&info, &out);
    if (planned == 0) {
        // Planner failed: fall back to the global coli-probe result.
        ml::CVec3 nearPt = self->mPos
            + ml::CVec3(lbl_eu_80667B60, lbl_eu_80667B70, lbl_eu_80667B60);
        ml::CVec3 farPt = self->mPos
            + ml::CVec3(lbl_eu_80667B60, lbl_eu_80667BBC, lbl_eu_80667B60);
        (void)nearPt;
        (void)farPt;
        if (((int (*)(void*, void*))func_804B526C)(lbl_eu_80665958,
                (char*)self->mPlayer + 0x44a8) == 0) {
            return;
        }
        ml::CVec3* gp = (ml::CVec3*)((char*)lbl_eu_80665958 + 0x60);
        self->mVecB4 = *gp;
        self->mVecC0.y = lbl_eu_80667BC0 + gp->y;
    }

    // --- move-task allocation and commit toward the candidate spot ---
    gs = getUnk80664658();
    if ((gs->field_214 & 0x8000u) && func_801F4ED8(gs, &out) != 0) {
        int okT;
        if (self->mTask == 0) {
            okT = 0;
            void* gm = cf::CfGameManager::func_80083298();
            if (gm != 0) {
                UnkClass_8047CD0C* mgr = (UnkClass_8047CD0C*)((char*)gm + 0x2f2c);
                if (*(void**)mgr != 0) {
                    self->mTask = mgr->func_8047CE7C();
                    okT = (self->mTask != 0);
                }
            }
        } else {
            okT = 1;
        }
        if (okT != 0) {
            if (func_8047DE14__17UnkClass_8047D2ACFv(self->mTask, (Vec*)&self->mPos,
                    lbl_eu_80667B60, lbl_eu_80667B60) != 0) {
                if (walkPathCheck__17UnkClass_8047D2ACFv(self->mTask, (Vec*)&self->mPos,
                        &out, lbl_eu_80667B60, lbl_eu_80667B60, 1) == 0) {
                    return;
                }
            }
        }
    }

    // --- final commit: raycast lifted positions, reset embedded move state ---
    {
        ml::CVec3 up(lbl_eu_80667B60, lbl_eu_80667BC4, lbl_eu_80667B60);   // (0, BC4, 0)
        ml::CVec3 diff = out + up;                   // stack +0x18 -> +0x54
        ml::CVec3 sum = self->mPos + up;             // stack +0x24 -> +0x6c
        if (func_804BE348(&sum, &diff, 0x40004a05, 0, 0) != 0) {
            return;
        }
    }
    self->mFlags4C &= ~0x100u;
    cf::CMovePosIntf* emb = (cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c);
    emb->getHandle();                                // vtable 0xa8
    emb->setState(lbl_eu_80667B68);                  // vtable 0x168 setState(1.0)
    func_800BC3B0((char*)self->mObject + 0x3e9c, lbl_eu_80667B90);
    cf::CfMoveSub* sub2 = ((cf::CfObjWrap*)self->mObject)->mSub;
    if (sub2 != 0) {
        sub2->mField3F0 = ml::CVec3::zero.x;
        sub2->mField3F4 = ml::CVec3::zero.y;
        sub2->mField3F8 = ml::CVec3::zero.z;
    }
    func_80089990(self);
    if (self->mTask != 0) {
        void* gm = cf::CfGameManager::func_80083298();
        if (gm != 0) {
            void* p = (char*)gm + 0x2f2c;
            if (p != 0) {
                func_8047CF20__17UnkClass_8047CD0CFv(p, self->mTask);
            }
        }
        self->mTask = 0;
    }
    }
}
// ============================================================================
// func_8019B4F0 - walk-state update (0x2A0 bytes)
//
// Two entry modes keyed on flag 0x02000000:
//  - clear: steering update. If the stored facing is degenerate, reset the
//    tick counter; otherwise probe a ray from the object forward and either
//    rotate the stored facing (counter-gated) or steer to a spot beside the
//    object (func_80089E88).
//  - set: timed nudge of mVecC0 toward a fixed step with arrival gates,
//    then a two-iteration candidate-spot search around the object (probe
//    point + facing offset) committing through func_80089E88.
// Common tail drives the move task contact handling (wall slide) or the
// steering helper func_80088974.
// ============================================================================
extern "C" int func_8019B4F0(cf::CCtrlMovePC* self) {
    typedef int (*Probe2Fn)(void*, void*);
    ml::CVec3* goalPtr = &self->mVec54;   // r24 alias: vec fed to the tail helpers
    cf::CfObjWrap* plW = (cf::CfObjWrap*)self->mPlayer;

    if ((self->mFlags4C & 0x02000000u) == 0) {
        // ---- steering path ----
        u16 anim = plW->mField455A;
        plW->mField455A = anim - 3;
        if (self->mVec90.x == lbl_eu_80667B60 && self->mVec90.z == lbl_eu_80667B60) {
            self->mArr124[3] = 0;
        } else {
            ml::CVec3 tgt;
            tgt.x = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()->x;
            tgt.y = lbl_eu_80667BC8
                  + ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()->y;
            tgt.z = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()->z;
            ml::CVec3 dir = self->mVec90 * lbl_eu_80667B70;
            ml::CVec3 sum = tgt + dir;
            if (func_804B526C(lbl_eu_80665958, (char*)self->mObject + 0x44a8,
                              &tgt, &sum, 0, 0, lbl_eu_80571810) == 0) {
                self->mArr124[3] = 0;
            } else {
                s16 cnt = self->mArr124[3];
                self->mArr124[3] = cnt + 1;
                if (cnt >= 15) {
                    self->mArr124[3] = 0;
                    f32 ang = Atan2FIdx__Q24nw4r4mathFff(self->mVec90.x, self->mVec90.z);
                    self->mFloat100 = lbl_eu_80667BD8 * ang;
                    f32 a = lbl_eu_80667BD8 * ang + lbl_eu_8066A1F8;
                    f32 c = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * a);
                    f32 s = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * a);
                    ml::CVec3 facing(s, lbl_eu_80667B60, c);
                    ml::CVec3 off = facing * lbl_eu_80667BDC;
                    ml::CVec3 spot = *((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()
                                     + off;
                    self->mVecC0 = spot;
                    if (func_80089E88(self, &spot, 1) != 0) {
                        self->mFlags4C = (self->mFlags4C | 0x60000000u) & ~0x10000u;
                        goalPtr = &self->mVecC0;
                    }
                }
            }
        }
        plW->mField455A = anim;
    } else {
        // ---- nudge path ----
        goalPtr = &self->mVecC0;
        if (self->mFlags4C & 0x14000000u) {
            s16 cnt = self->mArr124[3] + 1;
            self->mArr124[3] = cnt;
            if (cnt >= 30 && (cnt & 0xF) == 15) {
                // Periodic nudge: slide the follow anchor one step forward.
                ml::CVec3 step(lbl_eu_80667B60, lbl_eu_80667BC8, lbl_eu_80667B60);
                ml::CVec3 moved = self->mVecC0 + step;
                ml::CVec3 movedCopy = moved;
                (void)movedCopy;
                if (((Probe2Fn)func_804B54D4)(lbl_eu_80665958,
                        (char*)self->mObject + 0x44a8)) {
                    self->mArr124[3] = 120;
                }
            }
        }
        if (self->mArr124[3] >= 120) {
            self->mArr124[3] = 0;
            self->mFlags4C &= 0xE1FFFFFFu;
        } else {
            ml::CVec3 delta = *((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()
                              - self->mVecC0;
            u32 flags = self->mFlags4C;
            if (flags & 0x04000000u) {
                if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z
                    <= lbl_eu_80667BCC) {
                    self->mArr124[3] = 0;
                    self->mFlags4C = (flags | 0x80000000u) & ~0x04000000u;
                }
            } else {
                if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z
                    <= lbl_eu_80667B68) {
                    self->mArr124[3] = 0;
                    self->mFlags4C &= 0xE1FFFFFFu;
                }
            }
        }

        if (self->mFlags4C & 0x08000000u) {
            u16 anim = plW->mField455A;
            plW->mField455A = anim - 3;
            const f32 f28 = lbl_eu_80667BC8;
            const f32 f29 = lbl_eu_8066A204;
            const f32 f30 = f28 * f29;
            const f64 bias = (f64)lbl_eu_80667BE8;
            const f32 sideScale = lbl_eu_80667BD4;
            int notCommitted = 1;   // r23
            for (int i = 0; i < 2; i++) {
                f32 ampBase = (i & 1) ? lbl_eu_80667B68 : lbl_eu_80667BD0;
                s16 cnt = self->mArr124[3];
                f32 t = f30 * (f32)((f64)(cnt & 1) - bias) + f29;
                f32 angle = ampBase * t + self->mFloat100;
                f32 cosv = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * angle);
                f32 sinv = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * angle);
                ml::CVec3 dir;
                dir.x = sinv;
                dir.y = lbl_eu_80667B60;
                dir.z = cosv;
                ml::CVec3 probe;
                probe.x = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()->x;
                probe.y = f28
                        + ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()->y;
                probe.z = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()->z;
                ml::CVec3 side = dir * sideScale;
                ml::CVec3 dest = probe + side;
                if (func_804B526C(lbl_eu_80665958, (char*)self->mObject + 0x44a8,
                                  &probe, &dest, 0, 0, lbl_eu_80571810) != 0) {
                    continue;
                }
                if (func_80089E88(self, &dest, 1) == 0) {
                    continue;
                }
                // Spot committed: arm the walk-request flags and stop scanning.
                self->mArr124[3] = 0;
                notCommitted = 0;
                self->mFlags4C = ((self->mFlags4C & ~0x08000000u) | 0x10000000u);
                self->mVecC0 = dest;
                break;
            }
            plW->mField455A = anim;
            if (notCommitted != 0) {
                s16 c = self->mArr124[3];
                self->mArr124[3] = c + 1;
                if (c + 1 >= 2) {
                    self->mArr124[3] = -60;
                    self->mFlags4C &= 0xE1FFFFFFu;
                }
                ml::CVec3 delta = *((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()
                                  - self->mVecC0;
                if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z
                    <= lbl_eu_80667B74) {
                    goalPtr = &self->mVec54;
                }
            }
        }
        self->mFlags4C &= ~0x10000u;
    }

    // ---- common tail: move-task contact / wall-slide handling ----
    if (self->mBase30 != 0) {
        ml::CVec3* tp = ((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)
                            ->getPosition();
        if (walkPathCheck__17UnkClass_8047D2ACFv(self->mBase30, (Vec*)tp, (Vec*)goalPtr,
                lbl_eu_80667B60, lbl_eu_80667B60, 1) != 0) {
            // Contact: slide along the surface normal toward the goal.
            self->mFlags4C &= ~0x01000000u;
            ml::CVec3 delta = *goalPtr
                - *((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)
                      ->getPosition();
            ml::CVec3 dirTmp = delta;
            ml::CVec3 n = dirTmp;
            if (n.x != lbl_eu_80667B60 || n.y != lbl_eu_80667B60 || n.z != lbl_eu_80667B60) {
                if (n.x * n.x + n.y * n.y + n.z * n.z != lbl_eu_80667B60) {
                    PSVECNormalize((const Vec*)&n, (Vec*)&n);
                } else {
                    n = ml::CVec3::zero;
                }
            }
            ml::CVec3 out(self->mBase3C * (n.x - self->mField18) + self->mField18,
                          lbl_eu_80667B60,
                          self->mBase3C * (n.z - self->mField20) + self->mField20);
            if (out.x != lbl_eu_80667B60 || out.y != lbl_eu_80667B60
                || out.z != lbl_eu_80667B60) {
                if (out.x * out.x + out.y * out.y + out.z * out.z != lbl_eu_80667B60) {
                    PSVECNormalize((const Vec*)&out, (Vec*)&out);
                } else {
                    out = ml::CVec3::zero;
                }
            }
            if (self->mBase3C > lbl_eu_80667BE0) {
                if (!(self->mFlags50 & 0x400u)) {
                    self->mFlags50 |= 0x400u;
                    ml::CVec3 probe;
                    probe.x = lbl_eu_80667BC8 * out.x
                            + ((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)
                                  ->getPosition()->x;
                    probe.y = lbl_eu_80667B70
                            + ((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)
                                  ->getPosition()->y;
                    probe.z = lbl_eu_80667BC8 * out.z
                            + ((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)
                                  ->getPosition()->z;
                    if (func_804BE398(&probe, 0x4a05, 0, 0, lbl_eu_80667BE4,
                                      lbl_eu_80667B60) == 0) {
                        self->mFlags50 |= 0x800u;
                    } else if (func_804BE5A4(0x20000, 0) != 0) {
                        self->mFlags50 |= 0x1000u;
                    }
                }
                if (self->mFlags50 & 0x80000u) {
                    // Wall pressed: re-lerp with the tighter factor and latch bit 1.
                    out.x = lbl_eu_80667B7C * (n.x - self->mField18) + self->mField18;
                    out.y = lbl_eu_80667B60;
                    out.z = lbl_eu_80667B7C * (n.z - self->mField20) + self->mField20;
                    self->mFlags50 |= 2u;
                    if (out.x != lbl_eu_80667B60 || out.y != lbl_eu_80667B60
                        || out.z != lbl_eu_80667B60) {
                        if (out.x * out.x + out.y * out.y + out.z * out.z
                            != lbl_eu_80667B60) {
                            PSVECNormalize((const Vec*)&out, (Vec*)&out);
                        } else {
                            out = ml::CVec3::zero;
                        }
                    }
                }
            }
            self->mField18 = out.x;
            self->mField1C = out.y;
            self->mField20 = out.z;
            self->mVec90 = out;
            return 1;
        }
    }

    if (self->mFlags4C & 0x01000000u) {
        self->mFlags4C &= ~0x01000000u;
        if (func_80088974((cf::CfObjectMove*)self, &self->mVec90, goalPtr, 0, 0) != 0) {
            return 1;
        }
    } else {
        if (func_80088974((cf::CfObjectMove*)self, &self->mVec90, goalPtr, 1, 1) != 0) {
            return 1;
        }
    }
    self->mFlags50 |= 4u;
    return 0;
}
// ============================================================================
// func_8019C0D4 - approach-state entry (0x1D8 bytes)
//
// If the pair-move flag is set, switch to the paired-walk state and run the
// reposition step. Otherwise pick a goal position: reuse the stored goal when
// slope-locked, else query the party-info planner for one. When the planner
// succeeds and the task is missing, allocate a move task through the game
// manager's node pool and start it toward the goal. Finally advance to the
// walk-update state.
// ============================================================================
extern "C" int func_8019C0D4(cf::CCtrlMovePC* self) {
    u32 f50 = self->mFlags50;
    self->mArr124[7] = 0;
    self->mFlags4C &= ~0x10000u;
    if (f50 & 2u) {
        // Paired movement: delegate to the reposition state handler.
        self->mStateFunc = lbl_eu_80532C14;
        self->mArr124[15] = 0;
        func_8019CDA0(self);
        return 0;
    }

    if (self->mFlags4C & 0x1000u) {
        // Slope/height locked: keep the previous target.
        self->mVec54 = self->mVecCC;
    } else {
        ml::CVec3 pos = self->mPos;
        CfPartyInfoIn info;
        func_80198710(&info, &pos, 6, self->mFloat100, lbl_eu_80667B70,
                      lbl_eu_80667B60, self->mShort120);
        if (func_8019876C(&info, &self->mVec54) == 0) {
            // Planner failed: head straight for our own position.
            self->mFlags4C |= 1u;
            self->mVec54 = self->mPos;
        } else if (!(self->mFlags4C & 0x10000u)) {
            int ok;
            if (self->mTask == 0) {
                ok = 0;
                void* gm = cf::CfGameManager::func_80083298();
                if (gm != 0) {
                    UnkClass_8047CD0C* mgr = (UnkClass_8047CD0C*)((char*)gm + 0x2f2c);
                    if (*(void**)mgr != 0) {
                        self->mTask = mgr->func_8047CE7C();
                        ok = (self->mTask != 0);
                    }
                }
            } else {
                ok = 1;
            }
            if (ok && walkPathCheck__17UnkClass_8047D2ACFv(self->mTask,
                    (Vec*)&self->mPos, &self->mVec54,
                    lbl_eu_80667B60, lbl_eu_80667B60, 1) == 0) {
                self->mFlags4C |= 1u;
                self->mVec54 = self->mPos;
            }
        }
    }

    if (func_8019B4F0(self) == 0 && !(self->mFlags4C & 0x1000u)) {
        self->mFlags4C |= 1u;
        self->mVec54 = self->mPos;
    }
    self->mFlags4C |= 0x10u;
    self->mStateFunc = lbl_eu_80532C20;
    func_8019C304(self);
    self->mFlags4C &= ~0x18u;
    return 0;
}
// ============================================================================
// func_8019C304 - approach-state update (0x9D8 bytes)
//
// Counts ticks in the state; on timeout transitions. Depending on the mode
// flags it either scans the party for a member blocking the path (bail out
// toward func_8019D9E0) or runs the walk-speed selection, obstacle probe and
// steering (func_80089694) logic below.
// ============================================================================
extern "C" int func_8019C304(cf::CCtrlMovePC* self) {
    typedef int (cf::CCtrlMovePC::*MoveFn)();

    s16 cnt = self->mArr124[7];              // 0x132 tick counter
    self->mArr124[7] = (s16)(cnt + 1);
    const MoveFn* tbl = &lbl_eu_80532B60;
    if (cnt > 12) {
        self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0xcc);
    }

    u32 f4C = self->mFlags4C;
    f32 speed;
    if (f4C & 0x00001000u) {
        if (self->mFlags50 & 1u) {
            // Party-blocked scan: bail if any other member is closer than us
            // to the goal or stands within BF8 of our current spot.
            speed = lbl_eu_80667BF0;
            int bail = 0;
            if (self->mDistFC <= lbl_eu_80667BF4) {
                bail = 1;
            } else {
                for (int i = 1; i < 3; i++) {
                    cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
                    if (p == 0) continue;
                    if (self->mShort122 == i) continue;
                    ml::CVec3* pp = ((cf::CMovePosIntf*)p)->getPosition();
                    ml::CVec3 d = *pp - self->mPos;
                    if (!(d.x * d.x + d.y * d.y + d.z * d.z <= self->mDistFC * self->mDistFC)) break;
                    ml::CVec3* op = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                    ml::CVec3* pp2 = ((cf::CMovePosIntf*)p)->getPosition();
                    ml::CVec3 d2 = *pp2 - *op;
                    if (!(d2.x * d2.x + d2.y * d2.y + d2.z * d2.z <= lbl_eu_80667BF8)) break;
                    bail = 1;
                    break;
                }
            }
            if (bail != 0) {
                self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0xd8);
                func_8019D9E0(self);
                return 0;
            }
        } else {
            if (f4C & 0x00000800u) {
                self->mBase3C = lbl_eu_80667B74;
                speed = lbl_eu_80667BC8;
            } else {
                speed = lbl_eu_80667B6C;
            }
            if (self->mDistFC >= lbl_eu_80667BC4) {
                self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0xe4);
            }
        }
    } else if (f4C & 0x00000400u) {
        speed = lbl_eu_80667B68;
        f32 lim;
        if (f4C & 0x00000200u) {
            lim = lbl_eu_80667BFC;
        } else {
            lim = lbl_eu_80667C00;
        }
        if (self->mDistFC >= lim) {
            self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0xf0);
        }
    } else if (f4C & 0x00010000u) {
        speed = lbl_eu_80667BA8 * (f32)self->mShort122;
        if (self->mDistFC >= speed) {
            self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0xfc);
        }
    } else {
        speed = lbl_eu_80667B68;
        if (self->mDistFC >= lbl_eu_80667B70) {
            self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x108);
        }
    }

    // --- goal distance bookkeeping ---
    ml::CVec3* pos = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
    ml::CVec3 delta = *pos - self->mVec54;
    f32 dist2;
    if (delta.y * delta.y <= lbl_eu_80667C04 || (self->mFlags50 & 1u)) {
        dist2 = delta.x * delta.x + delta.z * delta.z;      // horizontal only
    } else {
        dist2 = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
    }

    f32 spd;
    if (dist2 < speed * speed) {
        // --- pick the walk speed for this tick ---
        if (!(f4C & 0x00001000u)) {
            u32 f4Cb = self->mFlags4C;
            if (dist2 < (lbl_eu_80667BA8 + speed) * (lbl_eu_80667BA8 + speed)) {
                if (!(f4Cb & 0x4u)) {
                    if (dist2 > (lbl_eu_80667BDC + speed) * (lbl_eu_80667BDC + speed)) {
                        self->mFlags4C |= 0x2u;
                    }
                    self->mArr124[5] = 0;
                    spd = lbl_eu_80667BC0;
                } else if (f4Cb & 0x00100000u) {
                    f32 th = lbl_eu_80667B70 + speed;
                    if (dist2 < th * th) {
                        if (dist2 < lbl_eu_80667B60) {
                            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                        }
                        f32 inv = (dist2 > lbl_eu_80667B60) ? dist2 * FrSqrt__Q24nw4r4mathFf(dist2) : lbl_eu_80667B60;
                        spd = lbl_eu_80667C08 * (inv / th) + lbl_eu_80667B6C;
                    }
                }
            } else {
                self->mFlags4C |= 0x4u;
                if (dist2 > lbl_eu_80667C0C) {
                    if (dist2 < lbl_eu_80667B60) {
                        Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                    }
                    f32 inv = (dist2 <= lbl_eu_80667B60) ? dist2 * FrSqrt__Q24nw4r4mathFf(dist2) : lbl_eu_80667B60;
                    spd = lbl_eu_80667B6C * (inv - lbl_eu_80667BB0) + lbl_eu_80667B68;
                    self->mBase3C = lbl_eu_80667B74;
                    if (spd > lbl_eu_80667C10) {
                        spd = lbl_eu_80667C10;
                    }
                }
            }
        }

        // Close-range arrival snap: drop the run/walk latch bits.
        if ((self->mFlags4C & 1u) && dist2 < (lbl_eu_80667B68 + speed) * (lbl_eu_80667B68 + speed)) {
            self->mFlags4C &= ~0x7u;
        }

        // --- ramp down when arriving / decelerate near the goal ---
        if (self->mFlags50 & 0x80000u) {
            if (spd != lbl_eu_80667B60) {
                u32 g = self->mFlags4C;
                s16 t = self->mArr124[4];
                self->mArr124[4] = (s16)(t + 1);
                if (g & 0x00001000u) {
                    if (t + 1 >= 15) {
                        self->mArr124[4] = 30;
                    } else {
                        spd = lbl_eu_80667B60;
                    }
                } else {
                    if (t + 1 >= 30) {
                        self->mArr124[4] = 30;
                    } else {
                        spd = lbl_eu_80667B60;
                    }
                }
            } else {
                self->mArr124[4] = 0;
            }
        }

        if (spd != lbl_eu_80667B60) {
            if (!(self->mFlags4C & 0x10u)) {
                func_8019B4F0(self);
            }
            u32 f50 = self->mFlags50;
            if (f50 & 0x20000u) {
                spd = lbl_eu_80667B6C;
            }
            if (!(f50 & 0x400u)) {
                // Obstacle probe ahead: steer around or mark blocked.
                self->mFlags50 |= 0x400u;
                ml::CVec3* p1 = ((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)->getPosition();
                f32 pz = p1->z;
                ml::CVec3* p2 = ((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)->getPosition();
                f32 tz = lbl_eu_80667BC8 * self->mVec90.z + pz;
                ml::CVec3* p3 = ((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)->getPosition();
                f32 ty = lbl_eu_80667B70 + p3->y;
                ml::CVec3* p4 = ((cf::CMovePosIntf*)((cf::CfMoveData*)self->mBaseData)->field_28)->getPosition();
                ml::CVec3 probe(lbl_eu_80667BC8 * self->mVec90.x + p4->x, ty, tz);
                if (func_804BE398(&probe, 0x4a05, 0, 0, lbl_eu_80667BE4, lbl_eu_80667B60) == 0) {
                    self->mFlags50 |= 0x800u;
                } else if (func_804BE5A4(0x20000, 0) != 0) {
                    self->mFlags50 |= 0x1000u;
                }
            }

            // Wall-slide hold: freeze while pressed against geometry.
            u32 f50b = self->mFlags50;
            if (f50b & 0x40000u) {
                if (self->mArr124[11] < 0x32) {
                    if (f50b & 0x800u) {
                        spd = lbl_eu_80667B60;
                        s16 c = self->mArr124[8];
                        self->mArr124[8] = (s16)(c + 1);
                        if (c + 1 >= 0x5a) {
                            self->mArr124[8] = 0;
                            if (self->mArr124[17] <= 0) {
                                u32 g = self->mFlags4C;
                                self->mFlags4C = (g & ~0x380u) | 0x8u;
                                self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x114);
                                self->mArr124[7] = 0;
                            }
                        }
                    } else {
                        self->mArr124[8] = 0;
                    }
                }
            } else {
                self->mArr124[8] = 0;
            }

            u32 f50e = self->mFlags50;
            if ((f50e & 0x1000u) && !(f50e & 0x8000u)
                && !(((cf::CfObjWrap*)self->mObject)->mSub->mField4EC & 0x100u)
                && (((cf::CfObjWrap*)self->mPlayer)->mSub->mField4EC & 0x100u)) {
                ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
                return 0;
            }

            int moved = 0;
            if (spd < lbl_eu_80667BC8) {
                if (self->mArr124[0] >= 0xa) {
                    u32 g = self->mFlags4C;
                    if ((g & 0x2000u) || (f50e & 0x100u)) {
                        self->mArr124[1] = 0;
                    } else {
                        s16 c = self->mArr124[1];
                        self->mArr124[1] = (s16)(c + 1);
                        if (c >= 0xa) {
                            self->mArr124[1] = 0;
                            self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x120);
                            moved = 1;
                        }
                    }
                }
            } else {
                self->mArr124[1] = 0;
            }
            if (moved != 0) {
                spd = lbl_eu_80667B60;
            }

            moved = 0;
            if (self->mArr124[0] >= 0x78) {
                s16 c = self->mArr124[2];
                self->mArr124[2] = (s16)(c + 1);
                if (c >= 0x1e) {
                    self->mArr124[2] = 0;
                    self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x12c);
                    moved = 1;
                }
            }
            if (moved != 0) {
                spd = lbl_eu_80667B60;
            }

            func_80089694(self, (const Vec*)&self->mVec90, spd);
            return 0;
        }
    } else {
        // Overspeed: just face the movement direction.
        if (self->mFlags50 & 1u) {
            f32 ang = func_8019EF90(self);
            self->mVec90.x = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * ang);
            self->mVec90.z = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * ang);
            func_80089694(self, (const Vec*)&self->mVec90, lbl_eu_80667BC0);
            return 0;
        }
        self->mArr124[4] = 0;
        self->mFlags4C |= 1u;
    }

    // Arrived: reset counters and idle the move task.
    u32 g = self->mFlags4C;
    self->mArr124[2] = 0;
    self->mFlags4C = (g | 0x01000000u) & 0xE1FFFFFFu;
    self->mArr124[1] = 0;
    self->mArr124[3] = 0;
    ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
    return 0;
}
// ============================================================================
// func_801999C0 - top-level movement update for the controlled character (0x9BC)
//
// One-shot init (state fn, party-index detection), then per-tick: update the
// goal position / facing from the player object, drive the move-state machine
// transitions (mode gates + timers) and finally decide the camera-facing
// side. Returns 1 when the actor is ready to move this tick.
// ============================================================================
extern "C" int func_801999C0(cf::CCtrlMovePC* self) {
    typedef int (cf::CCtrlMovePC::*MoveFn)();
    const MoveFn* tbl = &lbl_eu_80532B60;   // ptmf constant table

    if ((self->mFlags4C & 0x800000u) == 0) {
        self->mFlags4C |= 0x00800000u;
        func_80089990(self);
        if (self->mTask != 0) {
            void* gm = cf::CfGameManager::func_80083298();
            if (gm != 0) {
                void* p = (char*)gm + 0x2f2c;
                if (p != 0) {
                    func_8047CF20__17UnkClass_8047CD0CFv(p, self->mTask);
                }
            }
            self->mTask = 0;
        }
        ml::CVec3* pos = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
        self->mVecD8 = *pos;
        self->mBase38 = lbl_eu_80667B64;      // 32.0
        ((cf::CfObjWrap*)self->mObject)->mField455A = 100;
        func_800D59FC(self->mBaseData);
        // Retail interleaves the ptmf-table address calc between the array
        // zeroing stores, so the assignment sits lexically after [17].
        self->mArr124[17] = 0;
        const MoveFn* sel = (const MoveFn*)((const char*)tbl + 0x30);
        self->mStateFunc = *sel;
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
    }

    // Detect which party slot holds this character (r28 = mine, r27 = other).
    cf::CfPartyList* party = (cf::CfPartyList*)func_8009ECB0();
    u16 charId = ((cf::CfObjWrap*)self->mObject)->mField3F28;
    int mine = 0;
    int other = 0;
    if (party->mField08 != 0) {
        if (charId == party->mField08) {
            mine = 1;
            self->mShort122 = 1;
        }
        if (charId != party->mField08) {
            other = 1;
        }
        if (party->mField0C != 0) {
            if (charId == party->mField0C) {
                mine = 2;
                self->mShort122 = 2;
            }
            if (charId != party->mField0C) {
                other = 2;
            }
        }
    }

    cf::CfObjectMove* p0 = cf::CfGameManager::getPlayer(0);
    if (p0 == 0) {
        self->mPlayer = 0;
    } else {
        self->mPlayer = (char*)p0 - 0x3e9c;
    }
    if (self->mPlayer == 0) {
        ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
        return 0;
    }
    if (mine == 0) {
        ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
        return 0;
    }
    if (((cf::CfObjWrap*)self->mObject)->mSub == 0) {
        ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
        return 0;
    }
    if (((cf::CfObjWrap*)self->mPlayer)->mSub == 0) {
        ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
        return 0;
    }

    self->mVec6C = self->mPos;
    ml::CVec3* pp = ((cf::CMoveObj*)((char*)self->mPlayer + 0x3e9c))->getPosition();
    self->mPos = *pp;
    self->mFloat100 = ((cf::CMoveWrapper*)self->mPlayer)->getAngle();
    if (self->mFlags4C & 0x80000000u) {
        self->mFlags4C &= 0x7FFFFFFFu;
        self->mVec6C = self->mPos;
    }

    ml::CVec3* op = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
    self->mVec84.x = self->mPos.x - op->x;
    self->mVec84.y = self->mPos.y - op->y;
    self->mVec84.z = self->mPos.z - op->z;
    self->mDistFC = PSVECMag((const Vec*)&self->mVec84);
    f32 mag = (f32)(double)self->mDistFC;
    if (mag != lbl_eu_80667B60) {
        f32 k = lbl_eu_80667B68 / mag;
        ml::CVec3 t;
        t.x = self->mVec84.x * k;
        t.y = self->mVec84.y * k;
        t.z = self->mVec84.z * k;
        self->mVec78 = t;
    } else {
        self->mVec78.x = lbl_eu_80667B60;
        self->mVec78.y = lbl_eu_80667B60;
        self->mVec78.z = lbl_eu_80667B68;
    }

    self->mFlags50 &= 0xFFF862FDu;
    self->mFlags4C &= 0xFFE3E15Fu;
    self->mBase3C = lbl_eu_80667B6C;       // 0.1

    cf::CfMoveSub* own = ((cf::CfObjWrap*)self->mObject)->mSub;
    cf::CfMoveSub* plr = ((cf::CfObjWrap*)self->mPlayer)->mSub;
    own->mField4EC &= ~0xA20u;

    u16 fl = plr->mField530;
    if ((fl & 0xC) != 0 || (fl & 0x30) != 0) {
        if ((self->mFlags50 & 0x8u) == 0) {
            fl = plr->mField530;
            if ((fl & 0x4) != 0 || (fl & 0x8) != 0) {
                self->mVec9C = self->mPos;
            } else if ((fl & 0x10) != 0) {
                self->mVec9C = self->mPos;
                self->mVec9C.y += lbl_eu_80667B68;   // 1.0
            } else if ((fl & 0x20) != 0) {
                self->mVec9C = self->mPos;
                self->mVec9C.y -= lbl_eu_80667B68;
            }
            self->mFlags50 |= 0x48u;
        }
    }

    fl = plr->mField530;
    if (fl & 1) {
        if (own->mField530 & 1) {
            self->mFlags50 |= 0x2u;
            if ((fl & 0xC) == 0 && (fl & 0x30) == 0) {
                self->mFlags50 &= ~0x48u;
            }
        } else {
            if ((self->mFlags50 & 0x8u) == 0) {
                self->mVec9C = self->mPos;
                ml::CVec3* p = ((cf::CMoveObj*)((char*)self->mPlayer + 0x3e9c))->getPosition();
                self->mVec9C.y = p->y;
                if (own->mField0C & 0x2u) {
                    self->mFlags50 |= 0x8u;
                }
            }
            self->mFlags4C |= 0x1000u;
            self->mFlags50 &= ~0x80u;
            self->mVecCC = self->mVec9C;
        }
        self->mFlags50 |= 0x101u;
    } else if (own->mField530 & 1) {
        if ((self->mFlags50 & 0x8u) == 0) {
            self->mVec9C = self->mPos;
            if (plr->mField0C & 0x2u) {
                self->mFlags50 |= 0x8u;
                self->mVec9C.y -= lbl_eu_80667B70;   // 2.0
            }
        } else if ((self->mFlags50 & 0x40u) == 0 && (plr->mField0C & 0x2u)) {
            self->mFlags50 |= 0x40u;
            self->mVec9C.y = self->mPos.y - lbl_eu_80667B70;
        }
        self->mFlags4C |= 0x1000u;
        self->mFlags50 |= 0x102u;
        self->mVecCC = self->mVec9C;
        self->mFlags50 &= ~0x1u;
    } else {
        self->mFlags50 &= ~0xC8u;
        self->mFlags50 &= ~0x1u;
    }

    if ((self->mFlags4C & 0x1000u) == 0) {
        cf::CfGlobalSettings* gs = getUnk80664658();
        if (gs->field_214 & 0x8000u) {
            ml::CVec3* pos = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
            if (func_801F4ED8(getUnk80664658(), pos) == 0) {
                self->mFlags4C |= 0x1000u;
                self->mVecCC = self->mPos;
            }
        }
    }

    if ((self->mFlags4C & 0x1000u) == 0) {
        if (plr->mField4EC & 0x1000000u) {
            if (own->mField4EC & 0x1000000u) {
                self->mFlags4C &= 0xFFFF5FFFu;
            } else {
                if ((self->mFlags4C & 0x8000u) == 0) {
                    self->mVecCC = self->mVec6C;
                }
                self->mFlags4C |= 0x9000u;
            }
        } else if (own->mField4EC & 0x1000000u) {
            if ((self->mFlags4C & 0x8000u) == 0) {
                self->mVecCC = self->mPos;
            }
            self->mFlags4C |= 0x9000u;
        } else {
            self->mFlags4C &= 0xFFFF5FFFu;
        }
        if (self->mFlags4C & 0x1000u) {
            ml::CVec3* pos = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
            f32 dx = self->mVecCC.x - pos->x;
            ml::CVec3* pos2 = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
            f32 dz = self->mVecCC.z - pos2->z;
            if (dx * dx + dz * dz <= lbl_eu_80667B74) {   // 0.25
                self->mFlags4C |= 0x2000u;
                self->mVecCC = self->mPos;
            }
        }
    }

    if (own->mField0C & 0x200000u) {
        self->mFlags4C |= 0x600u;
    } else if (own->mField3F8 * own->mField3F8 + own->mField3F0 * own->mField3F0 >= lbl_eu_80667B78) {
        self->mFlags4C |= 0x400u;
    }

    void* st = ((cf::CMoveObj*)((char*)self->mPlayer + 0x3e9c))->get68();
    if (st != 0) {
        f32 h = ((cf::CfObjState*)st)->mField14;
        if (h >= lbl_eu_80667B7C) {       // 0.6
            self->mFlags4C |= 0x80000u;
        }
        if (h < lbl_eu_80667B6C) {        // 0.1
            self->mFlags4C |= 0x100000u;
        }
    }

    func_8019F1E0(self);

    cf::CfObjWrap* obj = (cf::CfObjWrap*)self->mObject;
    u32 x;
    void* sub = obj->mField04;
    void* r = ((cf::CMoveEmbedded*)sub)->getCtrl();
    x = *(u32*)r;
    if (func_80174C98(obj, (int*)&x, 4) != 0) {
        self->mFlags4C |= 0x40000u;
    }
    cf::CfObjWrap* pObj = (cf::CfObjWrap*)self->mPlayer;
    void* sub2 = pObj->mField04;
    void* r2 = ((cf::CMoveEmbedded*)sub2)->getCtrl();
    u32 x2;
    void* subP = pObj->mField04;
    void* rP = ((cf::CMoveEmbedded*)subP)->getCtrl();
    x2 = *(u32*)rP;
    if (func_80174C98(pObj, (int*)&x2, 4) != 0) {
        s16 v = self->mArr124[11];
        self->mArr124[11] = (s16)(v + 1);
        if (v + 1 > 60) {
            self->mArr124[11] = 60;
        }
    } else {
        self->mArr124[11] = 0;
    }

    s16 t = self->mArr124[17];
    self->mArr124[17] = (s16)(t - 1);
    if (t - 1 < 0) {
        self->mArr124[17] = 0;
    }

    ml::CVec3 dd;   // shared delta temp (retail reuses one stack slot)
    ml::CVec3* op2 = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
    dd.x = self->mVecA8.x - op2->x;
    dd.y = self->mVecA8.y - op2->y;
    dd.z = self->mVecA8.z - op2->z;
    if (dd.magnitude() <= lbl_eu_80667B68) {   // 1.0
        s16 v = self->mArr124[0];
        self->mArr124[0] = (s16)(v + 1);
        if (v + 1 >= 1000) {
            self->mArr124[0] = 1000;
        }
    } else {
        ml::CVec3* op3 = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
        self->mVecA8 = *op3;
        self->mArr124[0] = 0;
        self->mArr124[1] = 0;
        self->mArr124[2] = 0;
    }

    dd.x = self->mVecB4.x - self->mPos.x;
    dd.y = self->mVecB4.y - self->mPos.y;
    dd.z = self->mVecB4.z - self->mPos.z;
    if (dd.magnitude() <= lbl_eu_80667B68) {   // 1.0
        self->mFlags4C |= 0x20000000u;
    } else {
        self->mFlags4C &= 0xDFFFFFFFu;
        self->mVecB4 = self->mPos;
        self->mArr124[1] = 0;
        self->mArr124[2] = 0;
    }

    if ((self->mFlags4C & 0x8u) == 0) {
        if (own->mField530 & 1) {
            if (((cf::CMoveObj*)((char*)self->mPlayer + 0x3e9c))->vf03(0x100) != 0) {
                if (self->mArr124[17] <= 0) {
                    self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                    const MoveFn* sel = (const MoveFn*)((const char*)tbl + 0x3c);
                    self->mStateFunc = *sel;
                    self->mArr124[7] = 0;
                }
            } else if (self->mFlags4C & 0x100u || self->mDistFC >= lbl_eu_80667B80) {
                if (self->mArr124[17] <= 0) {
                    self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                    const MoveFn* sel = (const MoveFn*)((const char*)tbl + 0x48);
                    self->mStateFunc = *sel;
                    self->mArr124[7] = 0;
                }
            } else if ((self->mFlags50 & 0x100u) && self->mDistFC < lbl_eu_80667B84) {
                if (self->mArr124[17] <= 0) {
                    self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                    const MoveFn* sel = (const MoveFn*)((const char*)tbl + 0x54);
                    self->mStateFunc = *sel;
                    self->mArr124[7] = 0;
                }
            } else {
                    ml::CVec3 d4;
                    d4.x = self->mPos.x - self->mVec6C.x;
                    d4.y = self->mPos.y - self->mVec6C.y;
                    d4.z = self->mPos.z - self->mVec6C.z;
                    if (d4.magnitude() < lbl_eu_80667B88) {   // 16
                        if (self->mArr124[17] <= 0) {
                            self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                            const MoveFn* sel = (const MoveFn*)((const char*)tbl + 0x60);
                            self->mStateFunc = *sel;
                            self->mArr124[7] = 0;
                        }
                    } else if ((self->mFlags4C & 0x40000u) && self->mDistFC < lbl_eu_80667B8C) {  // 40
                        if (self->mArr124[17] <= 0) {
                            self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                            const MoveFn* sel = (const MoveFn*)((const char*)tbl + 0x6c);
                            self->mStateFunc = *sel;
                            self->mArr124[7] = 0;
                        }
                    } else if (self->mDistFC >= ((self->mFlags4C & 0x80000u) ? lbl_eu_80667B90 : lbl_eu_80667B94)) {
                        s16 v = self->mArr124[9];
                        self->mArr124[9] = (s16)(v + 1);
                        if (v + 1 >= 1200) {
                            self->mArr124[9] = 0;
                            if (self->mArr124[17] <= 0) {
                                self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                                const MoveFn* sel = (const MoveFn*)((const char*)tbl + 0x78);
                                self->mStateFunc = *sel;
                                self->mArr124[7] = 0;
                            }
                        } else {
                            ml::CVec3* op4 = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
                            ml::CVec3 dD8;
                            dD8.x = self->mVecD8.x - op4->x;
                            dD8.y = self->mVecD8.y - op4->y;
                            dD8.z = self->mVecD8.z - op4->z;
                            if (dD8.magnitude() <= lbl_eu_80667B98) {   // 225
                                s16 v2 = self->mArr124[10];
                                self->mArr124[10] = (s16)(v2 + 1);
                                if (v2 + 1 >= 300) {
                                    self->mArr124[10] = 0;
                                    if (self->mArr124[17] <= 0) {
                                        self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                                        const MoveFn* sel = (const MoveFn*)((const char*)tbl + 0x84);
                                        self->mStateFunc = *sel;
                                        self->mArr124[7] = 0;
                                    }
                                }
                            } else {
                                ml::CVec3* op5 = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
                                self->mVecD8 = *op5;
                                self->mArr124[10] = 0;
                            }
                        }
                    } else {
                        self->mArr124[9] = 0;
                        self->mArr124[10] = 0;
                    }
            }
        }
    }

    if (other == 0 || other <= mine) {
        f32 ang = self->mFloat100 + lbl_eu_8066A200;   // + pi/2
        f32 s = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * ang);
        f32 c = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * ang);
        f32 dir = self->mVec78.x * s + self->mVec78.z * c;
        if (self->mShort120 == 1) {
            if (dir > lbl_eu_80667BA0) {
                self->mShort120 = 2;
            } else {
                self->mShort120 = 1;
            }
        } else {
            if (dir < lbl_eu_80667BA4) {
                self->mShort120 = 1;
            } else {
                self->mShort120 = 2;
            }
        }
        self->mFlags50 &= ~0x80000u;
    } else {
        cf::CfObjectMove* p = cf::CfGameManager::getPlayer(other);
        cf::CfObjState* st2 = (cf::CfObjState*)((cf::CMoveObj*)p)->get68();
        if (st2 != 0) {
            if (st2->mField1AC == 2) {
                self->mShort120 = 1;
            } else {
                self->mShort120 = 2;
            }
        }
        self->mFlags50 |= 0x80000u;
    }

    if (self->mBase30 != 0) {
        own->mField0C &= ~0x3000u;
        ((cf::CfBase30Obj*)self->mBase30)->mField22 = 6;
    }

    if (self->mFlags4C & 0x8u) {
        cf::CfGlobalSettings* gs = getUnk80664658();
        if (self->mShort122 == 1) {
            gs->field_214 |= 0x10000u;
        } else {
            gs->field_214 |= 0x20000u;
        }
    }

    if ((lbl_eu_80663E28 & 0x4000000u) == 0) {
        if (((cf::CfBattleState*)getInstance__Q22cf14CBattleManagerFv())->mField20C8 != 0) {
            ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
            return 0;
        }
        if (func_8006EF04(0x40000000) != 0) {
            ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
            return 0;
        }
        cf::CfObjWrap* obj2 = (cf::CfObjWrap*)self->mObject;
        u32 y;
        void* sub3 = obj2->mField04;
        void* r3 = ((cf::CMoveEmbedded*)sub3)->getCtrl();
        y = *(u32*)r3;
        if (func_80174C98(obj2, (int*)&y, 8) != 0) {
            ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
            return 0;
        }
        f32 g = ((cf::CMoveWrapper*)self->mObject)->getF74();
        if (g > lbl_eu_80667B60) {
            return 1;
        }
    }

    ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
    return 0;
}

// ============================================================================
// func_8019CDA0 - state update: reposition / steer toward the goal (0x63C)
//
// Picks the goal from mVecCC/mPos, computes the delta to the object position,
// scans the party for the closest member (to bail toward), then applies the
// distance-gated state transitions and finally steers the facing direction
// toward the movement vector (func_80089694).
// ============================================================================
extern "C" int func_8019CDA0(cf::CCtrlMovePC* self) {
    if ((self->mFlags50 & 0x2u) == 0) {
        self->mStateFunc = lbl_eu_80532CA4;
        self->mArr124[15] = 0;
        return 1;
    }

    // Goal selection: freeze on the stored goal while slope-locked.
    ml::CVec3 goalSel;
    if (self->mFlags4C & 0x1000u) {
        goalSel = self->mVecCC;
    } else {
        goalSel = self->mPos;
    }

    // Two separate position queries: goal delta and self delta.
    ml::CVec3 dGoal;
    {
        ml::CVec3* p = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        dGoal = *p - goalSel;
    }
    ml::CVec3 dSelf;
    {
        ml::CVec3* p = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        dSelf = *p - self->mPos;
    }
    f32 dist = dSelf.x * dSelf.x + dSelf.y * dSelf.y + dSelf.z * dSelf.z;

    // First non-self party member; take it when it is at least as close as we
    // are (ties go to slot 2).
    cf::CfObjectMove* best = 0;
    for (int i = 1; i < 3; i++) {
        cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
        if (p == 0) continue;
        if (self->mShort122 == i) continue;
        ml::CVec3* pp = ((cf::CMovePosIntf*)p)->getPosition();
        ml::CVec3 d = *pp - self->mPos;
        f32 pd = d.x * d.x + d.y * d.y + d.z * d.z;
        if (dist > pd) {
            best = p;
        } else if (dist == pd && self->mShort122 == 2) {
            best = p;
        }
        break;
    }

    if (best != 0) {
        ml::CVec3* bp = ((cf::CMovePosIntf*)best)->getPosition();
        ml::CVec3* op = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        ml::CVec3 db = *op - *bp;
        f32 bd = db.x * db.x + db.y * db.y + db.z * db.z;
        if (dist > bd) {
            dist = bd;
            dSelf = db;
        }
    }

    f32 spd = lbl_eu_80667B68;      // move speed fed to the steer helper
    f32 slopeK = lbl_eu_80667C14;   // side-slip scale
    int act = 0;
    u32 f50 = self->mFlags50;
    if (f50 & 0x4u) {
        if (f50 & 0x20u) {
            if (dist >= lbl_eu_80667C04) {
                if (!(self->mFlags4C & 0x1000u) && best != 0) {
                    dGoal = dSelf;
                }
                act = 1;
            } else {
                self->mFlags50 = f50 & ~0x24u;
            }
        } else if (dist <= lbl_eu_80667C04) {
            // Overshot: aim the opposite way from the goal.
            dGoal = dSelf * lbl_eu_80667BD0;
            spd = lbl_eu_80667C18;
            slopeK = lbl_eu_80667B7C;
            act = 1;
        } else {
            self->mFlags50 = f50 & ~0x4u;
        }
    } else if (dist >= lbl_eu_80667C1C) {
        if (!(self->mFlags4C & 0x1000u) && best != 0) {
            dGoal = dSelf;
        }
        self->mFlags50 = f50 | 0x24u;
        act = 1;
    } else if (!(self->mFlags4C & 0x100000u)) {
        if (dist <= lbl_eu_80667C20) {
            self->mFlags50 = f50 | 0x4u;
            dGoal = dSelf * lbl_eu_80667BD0;
            spd = lbl_eu_80667C18;
            slopeK = lbl_eu_80667B7C;
            act = 1;
        }
    }

    if (act != 0) {
        if ((self->mFlags50 & 1u) == 0) {
            if (self->mArr124[0] >= 30) {
                s16 v = self->mArr124[15];
                self->mArr124[15] = (s16)(v + 1);
                if (v >= 60) {
                    self->mArr124[15] = 0;
                    if (self->mArr124[17] <= 0) {
                        self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                        self->mStateFunc = lbl_eu_80532CB0;
                        self->mArr124[7] = 0;
                    }
                    return 1;
                }
            }
        }

        // --- steer: rotate the facing toward the approach vector ---
        f32 cosA = CosFIdx__Q24nw4r4mathFf(
            lbl_eu_80667B9C * ((CMoveWrapper*)self->mObject)->getAngle());
        f32 sinA = SinFIdx__Q24nw4r4mathFf(
            lbl_eu_80667B9C * ((CMoveWrapper*)self->mObject)->getAngle());

        ml::CVec3 facing(sinA, lbl_eu_80667B60, cosA);
        ml::CVec3 up(lbl_eu_80667B60, lbl_eu_80667B68, lbl_eu_80667B60);
        ml::CVec3 right;
        PSVECCrossProduct((const Vec*)&facing, (const Vec*)&up, (Vec*)&right);
        // Right vector is flattened onto the XZ plane before normalizing.
        right.y = lbl_eu_80667B60;
        {
            f32 rl = right.x * right.x + right.y * right.y + right.z * right.z;
            if (rl == lbl_eu_80667B60) {
                right = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)&right, (Vec*)&right);
            }
        }

        ml::CVec3 work = dGoal;
        dist = dGoal.x * dGoal.x + dGoal.z * dGoal.z;   // horizontal length^2
        if (lbl_eu_80667B74 >= dist) {                  // 0.25: stand still
            work.x = lbl_eu_80667B60;
            work.z = lbl_eu_80667B60;
            self->mFlags50 &= ~0x80u;
        } else if (self->mFlags50 & 0x80u) {
            // Wall-slide hold: reuse the cached direction until the counter
            // runs out.
            work = self->mVecE4;
            s16 c = self->mArr124[8];
            self->mArr124[8] = (s16)(c - 1);
            if ((s16)(c - 1) <= 0) {
                self->mFlags50 &= ~0x80u;
                self->mArr124[8] = 0;
            }
        } else {
            f32 k;
            if (self->mFlags50 & 0x10u) {
                k = lbl_eu_80667B68;
            } else {
                k = slopeK;
                slopeK = lbl_eu_80667B68;
            }
            if (dist < lbl_eu_80667B60) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 inv;
            if (dist > lbl_eu_80667B60) {
                inv = dist * FrSqrt__Q24nw4r4mathFf(dist);
            } else {
                inv = lbl_eu_80667B60;
            }
            if (inv * k < slopeK * ml::math::abs(dGoal.y)) {
                // Slope case: climb straight up the surface toward the goal.
                int hit = 1;
                ml::CVec3 v(lbl_eu_80667B60, work.y, lbl_eu_80667B60);
                {
                    f32 vl = v.x * v.x + v.y * v.y + v.z * v.z;
                    if (vl == lbl_eu_80667B60) {
                        v = ml::CVec3::zero;
                    } else {
                        PSVECNormalize((const Vec*)&v, (Vec*)&v);
                    }
                }
                ml::CVec3 scaled = v * lbl_eu_80667C08;   // 0.3
                ml::CVec3* p3 = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                ml::CVec3 a = *p3 - scaled;
                ml::CVec3 diff = a - facing;
                ml::CVec3 sum = a + facing;
                if (func_804BE348(&diff, &sum, 0, 0x2000, 0) == 0) {
                    if (dGoal.x != lbl_eu_80667B60 || dGoal.z != lbl_eu_80667B60) {
                        hit = 0;
                        work = ml::CVec3(dGoal.x, lbl_eu_80667B60, dGoal.z);
                    }
                }
                self->mFlags50 &= ~0x10u;
                if (hit == 0) {
                    self->mVecE4 = work;
                    self->mFlags50 |= 0x80u;
                    self->mArr124[8] = 60;
                }
            } else {
                // Flat case: run around the goal keeping its height offset.
                int hit = 1;
                ml::CVec3 v(work.x, lbl_eu_80667B60, work.z);
                {
                    f32 vl = v.x * v.x + v.y * v.y + v.z * v.z;
                    if (vl == lbl_eu_80667B60) {
                        v = ml::CVec3::zero;
                    } else {
                        PSVECNormalize((const Vec*)&v, (Vec*)&v);
                    }
                }
                ml::CVec3 scaled = v * lbl_eu_80667C08;   // 0.3
                ml::CVec3* p3 = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                ml::CVec3 a = *p3 - scaled;
                ml::CVec3 diff = a - facing;
                ml::CVec3 sum = a + facing;
                if (func_804BE348(&diff, &sum, 0, 0x2000, 0) == 0) {
                    if (dGoal.y != lbl_eu_80667B60) {
                        hit = 0;
                        work = ml::CVec3(lbl_eu_80667B60, dGoal.y, lbl_eu_80667B60);
                    }
                }
                self->mFlags50 |= 0x10u;
                if (hit == 0) {
                    self->mVecE4 = work;
                    self->mFlags50 |= 0x80u;
                    self->mArr124[8] = 60;
                }
            }
        }

        // --- commit the steer direction ---
        {
            f32 wl = work.x * work.x + work.y * work.y + work.z * work.z;
            if (wl == lbl_eu_80667B60) {
                work = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)&work, (Vec*)&work);
            }
        }
        self->mVec90.y = lbl_eu_80667B60;
        self->mVec90.x = facing.x * (-work.y);
        self->mVec90.z = facing.z * (-work.y);
        dist = work.x * work.x + work.z * work.z;
        if (dist < lbl_eu_80667B60) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 inv2;
        if (dist > lbl_eu_80667B60) {
            inv2 = dist * FrSqrt__Q24nw4r4mathFf(dist);
        } else {
            inv2 = lbl_eu_80667B60;
        }
        f32 dot = work.x * right.x + work.z * right.z;
        if (dot >= lbl_eu_80667B60) {
            inv2 = -inv2;
        }
        self->mVec90.x = right.x * inv2 + self->mVec90.x;
        self->mVec90.z = right.z * inv2 + self->mVec90.z;
        func_80089694(self, (const Vec*)&self->mVec90, spd);
        return 0;
    }

    self->mArr124[15] = 0;
    self->mFlags4C = (self->mFlags4C | 0x01000000u) & 0xE1FFFFFFu;
    self->mArr124[2] = 0;
    self->mArr124[1] = 0;
    self->mArr124[3] = 0;
    ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
    return 0;
}
// ============================================================================
// func_8019D9E0 - state update: scan the other party members for one inside
// our personal space (within mDistFC of mPos), aim the goal at it, and run
// the distance-gated flag transitions / steering helper.
// ============================================================================
extern "C" int func_8019D9E0(cf::CCtrlMovePC* self) {
    if ((self->mFlags4C & 0x1000u) && !(self->mFlags50 & 0x2u)) {
        ml::CVec3 goal = self->mVec78;
        f32 bestLen = self->mDistFC;

        for (int i = 1; i < 3; i++) {
            cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
            if (p != NULL && self->mShort122 != i) {
                ml::CVec3* pp = ((cf::CMovePosIntf*)p)->getPosition();
                ml::CVec3 d;
                ml::CVec3* pd = &d;
                nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(pd),
                                    reinterpret_cast<nw4r::math::VEC3*>(pp),
                                    reinterpret_cast<nw4r::math::VEC3*>(&self->mPos));
                f32 d2 = pd->x * pd->x + pd->y * pd->y + pd->z * pd->z;
                if (d2 > self->mDistFC * self->mDistFC) break;

                // Distance between the member and our own embedded object; warn if
                // it slipped below the threshold.
                ml::CVec3* op = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                ml::CVec3* pp2 = ((cf::CMovePosIntf*)p)->getPosition();
                nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(pd),
                                    reinterpret_cast<nw4r::math::VEC3*>(pp2),
                                    reinterpret_cast<nw4r::math::VEC3*>(op));
                f32 e2 = pd->x * pd->x + pd->y * pd->y + pd->z * pd->z;
                if (e2 < self->mDistFC * self->mDistFC) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }

                bestLen = lbl_eu_80667B60;
                if (e2 > lbl_eu_80667B60) {
                    bestLen = e2 * FrSqrt__Q24nw4r4mathFf(e2);
                }
                if (bestLen != lbl_eu_80667B60) {
                    ml::CVec3 tmp;
                    nw4r::math::VEC3Scale(reinterpret_cast<nw4r::math::VEC3*>(&tmp),
                                          reinterpret_cast<nw4r::math::VEC3*>(pd),
                                          lbl_eu_80667B68 / bestLen);
                    ml::CVec3 tmp2(tmp);
                    goal = tmp2;
                }
                break;
            }
        }

        if (bestLen < lbl_eu_80667BA8) {
            if (bestLen >= lbl_eu_80667BDC) {
                self->mFlags4C &= ~0x20000000u;
            } else if (self->mFlags4C & 0x20000000u) {
                // Steer toward the chosen target via the move helper.
                ml::CVec3* op2 = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                ml::CVec3 delta;
                nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(&delta),
                                    reinterpret_cast<nw4r::math::VEC3*>(op2),
                                    reinterpret_cast<nw4r::math::VEC3*>(&goal));
                ml::CVec3 out(delta);
                if (func_80089E88(self, &out, 1) != 0) {
                    func_800896F4(self, &self->mVec90, &out);
                    func_80089694(self, (const Vec*)&self->mVec90, lbl_eu_80667BC0);
                    return 0;
                }
            } else if (bestLen < lbl_eu_80667C24) {
                self->mFlags4C |= 0x20000000u;
            }
        }

        self->mArr124[2] = 0;
        self->mFlags4C = (self->mFlags4C | 0x100u) & ~0x0F000000u;
        self->mArr124[1] = 0;
        self->mArr124[3] = 0;
        ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
        return 0;
    }

    self->mFlags4C &= ~0x20000000u;
    self->mStateFunc = lbl_eu_80532CBC;
    return 0;
}
// ============================================================================
// func_8019DD54 - reposition state update: pick a spot beside the party
// leader (or in front), steer the candidate toward it, and commit the move
// (0x9BC bytes). Returns 0 on every path.
// ============================================================================
extern "C" int func_8019DD54(cf::CCtrlMovePC* self) {
    self->mFlags4C |= 0x8;
    ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;

    // Bail out while the move-state reports an active condition/anim lock.
    cf::CfMoveSub* sub = ((cf::CfObjWrap*)self->mObject)->mSub;
    if (sub != 0) {
        u16 fl = sub->mField530;
        if ((fl & 0xC) != 0 || (fl & 0x30) != 0) {
            return 0;
        }
    }

    int flag = 0;          // height-clamp enable (set only in follow branch)
    f32 height;            // candidate ground height from party info
    CfPartyInfoOut goal;   // result block at stack +0x128 (goal x/y/z + flags)

    if (self->mFlags50 & 1) {
        // Follow mode: aim at a spot offset around the leader.
        u32 f4C = self->mFlags4C;
        ml::CVec3 target = self->mPos;
        f32 angle, radius;
        if (!(f4C & 0x4000)) {
            self->mFlags4C |= 0x20000;
            if (self->mShort120 == 1) {
                angle = lbl_eu_8066A204;
            } else {
                angle = -lbl_eu_8066A204;
            }
            radius = lbl_eu_80667C28;
        } else {
            angle = (f32)(u32)ml::math::mtRand(0x168) * lbl_eu_8066A210;
            radius = lbl_eu_80667BDC * (lbl_eu_80667B6C * (f32)(u32)ml::math::mtRand(0xa))
                     + lbl_eu_80667C2C;
        }

        // Rotate the local up vector into world space by facing yaw, then by
        // the approach angle, to get the lateral offset direction.
        f32 s1 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * self->mFloat100);
        f32 c1 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * self->mFloat100);
        Mtx mY = {
            {c1, lbl_eu_80667B60, s1, lbl_eu_80667B60},
            {lbl_eu_80667B60, lbl_eu_80667B68, lbl_eu_80667B60, lbl_eu_80667B60},
            {-s1, lbl_eu_80667B60, c1, lbl_eu_80667B60},
        };
        ml::CVec3 v(lbl_eu_80667B60, lbl_eu_80667BD0, lbl_eu_80667B60);
        f32 s2 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * angle);
        f32 c2 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * angle);
        Mtx mZ = {
            {c2, -s2, lbl_eu_80667B60, lbl_eu_80667B60},
            {s2, c2, lbl_eu_80667B60, lbl_eu_80667B60},
            {lbl_eu_80667B60, lbl_eu_80667B60, lbl_eu_80667B68, lbl_eu_80667B60},
        };
        PSMTXConcat(mZ, mY, mY);
        nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v,
            (const nw4r::math::MTX34*)mY, (const nw4r::math::VEC3*)&v);
        v = v * radius;
        target = target + v;

        f32 c3 = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * self->mFloat100);
        f32 s3 = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * self->mFloat100);
        ml::CVec3 facing(s3, lbl_eu_80667B60, c3);
        ml::CVec3 diff = target - facing;
        ml::CVec3 sum = target + facing;
        if (func_804BE348(&diff, &sum, 0, 0x2000, 0) == 0) {
            return 0;
        }

        // Reject spots overlapping another party member's personal space.
        int clear = 1;
        for (int i = 1; i < 3; i++) {
            void* p = getPlayer__Q22cf13CfGameManagerFi(i);
            if (p != 0) {
                p = (char*)p - 0x3e9c;
            }
            if (p == 0 || (void*)self->mObject == p) {
                continue;
            }
            ml::CVec3* pp = ((cf::CMovePosIntf*)((char*)p + 0x3e9c))->getPosition();
            v = *pp - target;
            if (v.x * v.x + v.z * v.z < lbl_eu_80667C30
                && ml::math::abs(v.y) < lbl_eu_80667B70) {
                clear = 0;
            }
        }
        if (clear == 0) {
            return 0;
        }

        func_804BE4E0(&v, 0);
        if (v.x * v.x + v.y * v.y + v.z * v.z == lbl_eu_80667B60) {
            v = ml::CVec3::zero;
        } else {
            PSVECNormalize((const Vec*)&v, (Vec*)&v);
        }
        func_804BE4B4(&goal, 0);
        goal.x += v.x * lbl_eu_80667BF0;
        goal.y += v.y * lbl_eu_80667BF0;
        goal.z += v.z * lbl_eu_80667BF0;
        self->mArr124[9] = 0;
        self->mStateFunc = lbl_eu_80532CC8;
        self->mArr124[15] = 0;
    } else {
        // Normal mode: query the party-spot builder for a candidate.
        u32 f4C = self->mFlags4C;
        if (!(f4C & 0x4000)) {
            self->mFlags4C |= 0x20000;
            ml::CVec3 v0 = self->mPos;
            CfPartyInfoIn p1;
            func_80198710(&p1, &v0, 6, self->mFloat100, lbl_eu_80667B70,
                          lbl_eu_80667B60, self->mShort120);
            self->mArr124[9] = 0;
            p1.mField2E = 1;
            if (func_8019876C(&p1, &goal) == 0) {
                return 0;
            }
            if (p1.mField2C != 0) {
                height = p1.mField24;
                flag = 1;
            }
        } else {
            ml::CVec3 v0 = self->mPos;
            u32 rnd = ml::math::mtRand();
            CfPartyInfoIn p2;
            func_80198710(&p2, &v0, 1, self->mFloat100, lbl_eu_80667B70,
                          lbl_eu_80667BA8, rnd & 0xffff);
            s16 cnt = self->mArr124[9];
            self->mArr124[9] = (s16)(cnt + 1);
            if (cnt > 3) {
                self->mArr124[9] = 5;
            } else {
                p2.mField2E = 1;
            }
            if (func_8019876C(&p2, &goal) == 0) {
                // No candidate: probe straight down/up for walkable floor.
                ml::CVec3 vHi = self->mPos;
                vHi.y += lbl_eu_80667BBC;
                ml::CVec3 vLo = self->mPos;
                vLo.y += lbl_eu_80667B70;
                if (func_804B526C(lbl_eu_80665958,
                    (char*)self->mPlayer + 0x44a8, &vHi, &vLo, 2, 1, 0) == 0) {
                    return 0;
                }
                ml::CVec3 tmp = *(ml::CVec3*)lbl_eu_80665958;
                goal.y = tmp.y + lbl_eu_80667BC0;
            } else if (p2.mField2C != 0) {
                height = p2.mField24;
                flag = 1;
            }
        }
        self->mStateFunc = lbl_eu_80532CD4;
    }

    // Common tail: raycast current pos -> candidate pos (both lifted by BC4),
    // then commit the reposition state.
    ml::CVec3 vA = self->mPos;
    vA.y += lbl_eu_80667BC4;
    ml::CVec3 vB(goal.x, goal.y, goal.z);
    vB.y += lbl_eu_80667BC4;
    if (func_804BE348(&vA, &vB, 0x44a05, 0, 0) != 0) {
        return 0;
    }

    cf::CfObjWrap* obj = (cf::CfObjWrap*)self->mObject;
    obj->mField455A = 0x64;
    if (flag != 0) {
        if (goal.y - height >= lbl_eu_80667C28) {
            goal.y -= lbl_eu_80667C28;
        } else {
            goal.y = height;
        }
    }
    self->mArr124[9] = 0;
    self->mArr124[10] = 0;
    self->mArr124[11] = 0;
    self->mArr124[1] = 0;
    self->mArr124[0] = 0;
    self->mArr124[17] = 30;
    self->mVecA8 = *(ml::CVec3*)&goal;
    self->mVecD8 = *(ml::CVec3*)&goal;

    // Reset the embedded move object: query handle, apply facing angle and
    // state, then clear the move-sub velocity.
    cf::CMovePosIntf* emb = (cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c);
    emb->getHandle();
    emb->setAngle(self->mFloat100);
    emb->setState(lbl_eu_80667B68);
    func_800BC3B0((char*)self->mObject + 0x3e9c, lbl_eu_80667B90);

    if (obj->mSub != 0) {
        cf::CfMoveSub* sub2 = obj->mSub;
        func_8004B7C0(sub2, &ml::CVec3::zero);
        sub2->mField3F0 = ml::CVec3::zero.x;
        sub2->mField3F4 = ml::CVec3::zero.y;
        sub2->mField3F8 = ml::CVec3::zero.z;
    }
    func_80089990(self);
    if (self->mTask != 0) {
        void* gm = cf::CfGameManager::func_80083298();
        if (gm != 0) {
            void* p = (char*)gm + 0x2f2c;
            if (p != 0) {
                func_8047CF20__17UnkClass_8047CD0CFv(p, self->mTask);
            }
        }
        self->mTask = 0;
    }
    self->mFlags4C &= 0xFFFDFEB7u;
    return 0;
}
// ============================================================================
// func_8019E710 - per-tick movement state update (0x69C bytes)
//
// Chooses a new state-fn ptmf from mFlags4C/mFlags50 and the current approach
// distance; when no transition happened (flag stays 1) it runs the steering
// logic: turn toward the target, collision-check the goal, and lerp the
// velocity toward the facing direction (func_800898D4).
// ============================================================================
extern "C" void func_8019E710(cf::CCtrlMovePC* self) {
    typedef int (cf::CCtrlMovePC::*MoveFn)();
    const MoveFn* tbl = &lbl_eu_80532B60;   // ptmf constant table
    int flag = 1;
    int blocked = 0;

    if (self->mFlags50 & 0x00010000u) {
        self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x180);
        flag = 0;
    } else {
        u32 f = self->mFlags4C;
        if (f & 0x00001000u) {
            if ((f & 0x00000800u)
                && ((((cf::CfObjWrap*)self->mObject)->mSub->mField4EC & 0x100u) == 0)
                && ((((cf::CfObjWrap*)self->mPlayer)->mSub->mField4EC & 0x100u) != 0)) {
                if (self->mDistFC > lbl_eu_80667B90) {
                    self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x18c);
                    flag = 0;
                }
            } else if (self->mDistFC > lbl_eu_80667B68) {
                self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x198);
                flag = 0;
            }
        } else if (f & 0x00000400u) {
            f32 lim;
            if (f & 0x00000200u) {
                // Slow-walk steering: only when close enough and slow enough.
                if (self->mDistFC < lbl_eu_80667B70) {
                    cf::CfMoveSub* sub = ((cf::CfObjWrap*)self->mObject)->mSub;
                    if (sub->mField3F0 * sub->mField3F0 + sub->mField3F8 * sub->mField3F8 < lbl_eu_80667C04) {
                        ml::CVec3 v;
                        ml::CVec3* vp = &v;
                        v.x = sub->mField3F0;
                        v.y = lbl_eu_80667B60;
                        v.z = sub->mField3F8;
                        if (!(v.x == lbl_eu_80667B60 && v.y == lbl_eu_80667B60 && v.z == lbl_eu_80667B60)) {
                            if (v.y * v.y + v.x * v.x + v.z * v.z == lbl_eu_80667B60) {
                                v = ml::CVec3::zero;
                            } else {
                                PSVECNormalize((const Vec*)vp, (Vec*)vp);
                            }
                            ml::CVec3* pos = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                            ml::CVec3 tmp = *pos + *vp;
                            ml::CVec3 probe(tmp.x, tmp.y + lbl_eu_80667B68, tmp.z);
                            if (func_804BE398(&probe, 0x44a05, 0, 0, lbl_eu_80667C34, lbl_eu_8066AF20) != 0) {
                                if (func_804BE5A4(0x400, 0) != 0) {
                                    ml::CVec3 scaled = *vp * lbl_eu_80667BD0;
                                    *vp = scaled;
                                    func_800898D4(self, vp);
                                    self->mVec90 = *vp;
                                    func_80089694(self, (const Vec*)&self->mVec90, lbl_eu_80667B68);
                                    flag = 0;
                                }
                            }
                        }
                    }
                }
                lim = lbl_eu_80667C38;
            } else {
                lim = lbl_eu_80667C3C;
            }
            if (self->mDistFC > lim) {
                self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x1a4);
                flag = 0;
            }
        } else {
            if (f & 0x00010000u) {
                // Limit arithmetic runs in double precision in retail.
                f64 limit = (f64)lbl_eu_80667BA8 * self->mShort122 + (f64)lbl_eu_80667BDC;
                if (self->mDistFC > limit) {
                    self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x1b0);
                    flag = 0;
                }
            } else if (self->mDistFC > lbl_eu_80667BA8) {
                self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x1bc);
                flag = 0;
            }
        }
    }

    // Tail: when no transition ran, steer toward the approach direction.
    // rlwinm masks here clear exactly bits 0x20000000 / 0x40.
    if (flag != 0) {
        if (self->mDistFC >= lbl_eu_80667B70) {
            self->mArr124[13] = 0;
            self->mFlags4C &= ~0x20000000u;
        } else if (!(self->mFlags4C & 0x20000000u)) {
            if (self->mDistFC < lbl_eu_80667C10) {
                u32 g = self->mFlags4C | 0x20000000u;
                self->mArr124[13] = 0;
                self->mFlags4C = g;
            }
        } else {
            ml::CVec3* pos = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
            ml::CVec3 delta = *pos - self->mVec78;
            ml::CVec3 deltaArg = delta;
            func_800896F4(self, &self->mVec90, &deltaArg);

            ml::CVec3 scaled = self->mVec90 * lbl_eu_80667BC4;
            ml::CVec3 scaledCopy = scaled;
            ml::CVec3* pos2 = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
            ml::CVec3 sum = *pos2 + scaledCopy;
            ml::CVec3 probePos(sum.x, sum.y + lbl_eu_80667BE4, sum.z);
            ml::CVec3 probeArg = probePos;

            blocked = 0;
            if (func_804BE398(&probeArg, 0x4a05, 0, 0, lbl_eu_80667BE4, lbl_eu_8066AF20) != 0) {
                if (func_804BE5A4(0x20000, 0) == 0) {
                    blocked = 1;
                } else if ((((cf::CfObjWrap*)self->mObject)->mSub->mField4EC & 0x100u) == 0) {
                    blocked = 1;
                }
            }
            if (blocked != 0) {
                func_80089694(self, (const Vec*)&self->mVec90, lbl_eu_80667BC0);
                self->mArr124[13] = 0;
                self->mFlags4C &= ~0x40u;
                flag = 0;
            } else {
                s16 c = self->mArr124[13];
                self->mArr124[13] = (s16)(c + 1);
                if (c < 60) {
                    self->mFlags4C |= 0x40u;
                } else {
                    self->mArr124[13] = 60;
                    self->mFlags4C &= ~0x40u;
                }
            }
        }
    } else {
        self->mArr124[13] = 0;
        self->mFlags4C &= ~0x20000000u;
    }

    cf::CfObjWrap* obj = (cf::CfObjWrap*)self->mObject;
    int free = 1;
    if (obj->mSub != 0) {
        free = ((obj->mSub->mField4EC & 2u) == 0);
    }
    if ((self->mFlags4C & 0x40u) && free != 0) {
        obj->mField455A = 0x65;
    } else {
        obj->mField455A = 0x64;
    }
    self->mArr124[4] = 0;
    if (flag != 0) {
        u32 g = self->mFlags4C;
        self->mArr124[2] = 0;
        g |= 0x01000000u;
        self->mArr124[1] = 0;
        g &= 0xE1FFFFFFu;
        self->mArr124[3] = 0;
        self->mFlags4C = g;
        ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
    }
}
// ============================================================================
// func_8019EF90 - ground-probe angle search around the goal (0x310)
//
// From a point just above the stored goal position, cast short rays outward
// in four directions (starting at the object's facing, stepping pi/2 per
// retry) at two heights (dropping the start point each pass). On the first
// hit, return the surface normal converted to degrees (negated components
// on the first pass). If no ray hits anything, return the facing angle.
// ============================================================================
extern "C" f32 func_8019EF90(cf::CCtrlMovePC* self) {
    ml::CVec3 goal(self->mVec54.x, self->mVec54.y + lbl_eu_80667B68, self->mVec54.z);
    ml::CVec3 dir;
    ml::CVec3 n;
    ml::CVec3 start;
    ml::CVec3 end;
    ml::CVec3 diff;
    ml::CVec3 sum;
    for (int i = 0; i < 2; i++) {
        f32 ang = ((CMoveWrapper*)self->mObject)->getAngle();   // vtable 0x5b4
        for (int j = 0; j < 4; j++) {
            dir.z = lbl_eu_80667B70 * CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * ang);
            dir.y = lbl_eu_80667B60;
            dir.x = lbl_eu_80667B70 * SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * ang);
            diff = goal - dir;
            sum = goal + dir;
            start = diff;
            end = sum;
            if (func_804BE348(&start, &end, 0, 0x2000, 0) != 0) {
                func_804BE4E0(&n, 0);
                if (i == 0) {
                    return lbl_eu_80667BD8 * Atan2FIdx__Q24nw4r4mathFff(-n.x, -n.z);
                }
                return lbl_eu_80667BD8 * Atan2FIdx__Q24nw4r4mathFff(n.x, n.z);
            }
            ang = ang + lbl_eu_8066A200;
        }
        goal.y = goal.y - lbl_eu_80667BDC;
    }
    return ((CMoveWrapper*)self->mObject)->getAngle();
}
// ============================================================================
// func_8019F1E0 - pair-follow mode update (0x2A8 bytes)
//
// Runs each tick from func_801999C0. When follow mode (mFlags50 bit 0x4000)
// is active it either cancels it (too far / spot occupied but arrived) or
// re-targets the stored candidate spot. Otherwise it maintains the follow
// request bits from distance thresholds. Finally, if a reposition is pending,
// it walks an angle/ring probe grid around the character (index mArr124[16],
// low 3 bits = direction step, high bits = ring radius), raycasts each
// candidate, and on a valid free ground spot commits it via func_80089E88.
// ============================================================================
extern "C" void func_8019F1E0(cf::CCtrlMovePC* self) {
    u32 f50 = self->mFlags50;

    if (f50 & 0x4000u) {
        if (!(self->mDistFC >= lbl_eu_80667B90)) {
            cf::CfObjWrap* ownW = (cf::CfObjWrap*)self->mObject;
            if (((ownW->mSub->mField4EC & 0x100u) != 0) ||
                ((((cf::CfObjWrap*)self->mPlayer)->mSub->mField4EC & 0x100u) != 0)) {
                // Spot occupied: confirm we actually reached the goal area.
                int arrived = 1;
                if ((ownW->mSub->mField4EC & 0x100u) != 0) {
                    const ml::CVec3& d = *((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition()
                                         - self->mVecF0;
                    f32 d2 = d.x * d.x + d.y * d.y + d.z * d.z;
                    if (!(d2 > lbl_eu_80667C44)) {
                        arrived = 0;
                        self->mFlags50 = (self->mFlags50 & 0x7FFu) | 0x8000u;
                    }
                }
                self->mFlags4C |= 0x1000u;
                self->mVecCC = self->mVecF0;
                if (arrived != 0) {
                    self->mFlags4C |= 0x800u;
                    return;
                }
                // fall through to the common tail below
            } else {
                // Nobody blocking but we are far: cancel follow mode.
                self->mArr124[16] = 0;
                self->mFlags50 = (self->mFlags50 & ~0x4200u) | 0xA000u;
                return;
            }
        } else {
            // Too far from the partner: cancel follow mode.
            self->mArr124[16] = 0;
            self->mFlags50 = (self->mFlags50 & ~0x4200u) | 0xA000u;
            return;
        }
    } else if (f50 & 0x2000u) {
        u32 t = f50 | 0x8000u;
        self->mFlags50 = t;
        if (self->mDistFC < lbl_eu_80667BD4) {
            self->mFlags50 = t & 0x1FFFu;
        }
    } else {
        if (!(self->mDistFC >= lbl_eu_80667BB0)) {
            self->mFlags50 = f50 | 0xA000u;
        }
    }

    // --- common tail: decide whether to run the spot-search this tick ---
    if ((((cf::CfObjWrap*)self->mObject)->mSub->mField4EC & 0x100u) == 0 ||
        (self->mFlags50 & 0x2000u)) {
        self->mArr124[16] = 0;
        self->mFlags50 &= 0x7FFu;
        return;
    }
    if (self->mFlags50 & 0x400u) {
        const ml::CVec3& d = self->mPos - self->mVecF0;
        f32 d2 = d.x * d.x + d.y * d.y + d.z * d.z;
        if (!(d2 > lbl_eu_80667BAC)) {
            self->mArr124[16] = 0;
            if (self->mFlags4C & 0x1000u) {
                return;
            }
            self->mFlags4C |= 0x1800u;
            self->mVecCC = self->mVecF0;
            return;
        }
        self->mFlags50 &= 0x7FFu;
    }

    // --- spot search: probe rings around the character until a free,
    //     reachable ground spot appears (or the grid index overflows) ---
    // Hoisted constants match the retail FP-register allocation order.
    ml::CVec3 out;                 // ground query result
    ml::CVec3 probe;               // candidate spot
    const f32 heightStep = lbl_eu_80667BD4;
    const f64 angBias = lbl_eu_80667BE8;
    const f32 ampScale = lbl_eu_8066A204;
    const f32 baseRadius = lbl_eu_80667B68;
    const f32 degScale = lbl_eu_80667B9C;
    const f32 maxReach = lbl_eu_80667C50;
    for (int i = 0; i < 4; i++) {
        s16 h = self->mArr124[16];
        if (h >= 0xF0) {
            self->mArr124[16] = 0;
            continue;
        }
        ml::CVec3* pos = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        probe.x = pos->x;
        probe.z = pos->z;
        probe.y = pos->y + heightStep;
        // direction index (low bits) and ring index (high bits), both biased
        // by the same double constant before scaling
        f32 amp = ((f32)((f64)(h & 7) - angBias)) * ampScale;
        f32 rad = baseRadius + (f32)((f64)(h >> 4) - angBias);
        f32 s = SinFIdx__Q24nw4r4mathFf(degScale * amp);
        probe.x += rad * s;
        f32 c = CosFIdx__Q24nw4r4mathFf(degScale * amp);
        probe.z += rad * c;

        if (func_804BE398(&probe, 0x4a05, 0, 0, lbl_eu_80667C48, lbl_eu_80667C4C) == 0) {
            self->mArr124[16] = h + 1;
            continue;
        }
        if (func_804BE5A4(0x20000, 0) != 0) {
            self->mArr124[16] = h + 1;
            continue;
        }
        func_804BE4B4(&out, 0);
        const ml::CVec3& d = self->mPos - out;
        f32 d2 = d.x * d.x + d.y * d.y + d.z * d.z;
        if (d2 > maxReach * maxReach) {
            self->mArr124[16] = h + 1;
            continue;
        }
        if (func_80089E88(self, &out, 0) == 0) {
            self->mArr124[16] = h + 1;
            continue;
        }
        // committed: remember the spot and raise the move-request flags
        self->mVecF0 = out;
        self->mFlags50 |= 0x4200u;
        self->mArr124[16] = 0;
        return;
    }
}