// Translation unit for kyoshin/cf/CtrlMovePC
//
// CCtrlMovePC - player-character movement controller. A state machine driven
// by a pointer-to-member-function (ptmf) at +0x108, dispatched each tick.
// Reconstructed from retail assembly (build/us/asm/kyoshin/cf/CtrlMovePC.s).
//
// All 20 functions use placeholder retail symbols (extern "C") so hexdiff /
// objdiff can pair them exactly with the retail object.

#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CPartsChange.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"
#include "monolib/coli/code_804B2FF0.hpp"
#include "kyoshin/cf/CBattleManager.hpp"

#include "kyoshin/cf/CtrlMovePC.hpp"
#include "kyoshin/cf/CtrlActApi.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"
#include <nw4r/math/math_types.h>
#include <monolib/math/CVec3.hpp>
namespace nw4r { namespace math { f32 FrSqrt(f32 x); } }

using cf::CCtrlMovePC;

typedef int (CCtrlMovePC::*MoveFn)();
struct MoveFnTable {
    MoveFn mEntries[36];
};

// ---- external runtime / engine symbols (exact retail names) ----
extern "C" {

// CCtrlMoveBase helpers.
void moveBaseUnlink(CCtrlMovePC* self);

// ptmf runtime.
extern int (CCtrlMovePC::*const __ptmf_null)();
long __ptmf_test(PTMF* ptmf);
void __ptmf_scall(...);

// Engine helpers (allocFreeNode / dispatchScnCommand / updateWalker come
// from kyoshin/cf/CtrlMoveBase.hpp).
void* MapFx_ResolveOwner(void* obj);
void  releaseNode__17UnkClass_8047CD0CFv(void* unk, void* task); // UnkClass_8047CD0C::func
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

extern int (CCtrlMovePC::*const lbl_eu_80532CC8)();
extern int (CCtrlMovePC::*const lbl_eu_80532CD4)();

// Kept label spellings in fully matched functions: turning these frozen
// constant-pool references into literals gives anonymous reloc targets there.
extern const f32 lbl_eu_80667B60;    // 0.0f
extern const f32 lbl_eu_80667B68;    // 1.0f
extern const f32 lbl_eu_80667B9C;    // 40.7436637878418f
extern const f32 lbl_eu_80667C40;    // 36.0f
}

// ============================================================================
// CCtrlMovePC::CCtrlMovePC - constructor
// ============================================================================
CCtrlMovePC::CCtrlMovePC(void* owner, void* obj) : CCtrlMoveBase(owner) {
    setVtable(lbl_eu_80532D58);

    mStateFunc = __ptmf_null;
    maskMoveBaseFlags((void*)this);

    mStateFunc = lbl_eu_80532B60;
    moveBaseUnlink(this);

    mFlags4C = 0x80000000u;
    mFlags50 = 0;

    mPos   = ml::CVec3::zero;
    mVec6C = ml::CVec3::zero;
    mVecA8 = ml::CVec3::zero;
    mVecD8 = ml::CVec3::zero;
    mVecB4 = ml::CVec3::zero;
    mVec90 = ml::CVec3::zero;

    mFloat100 = 0.0f;

    mArr124[17] = 0;
    mArr124[16] = 0;
    mArr124[15] = 0;
    mArr124[14] = 0;
    mArr124[13] = 0;
    mArr124[12] = 0;
    mArr124[11] = 0;
    mArr124[10] = 0;
    mArr124[9]  = 0;
    mArr124[8]  = 0;
    mArr124[7]  = 0;
    mArr124[6]  = 0;
    mArr124[5]  = 0;
    mArr124[4]  = 0;
    mArr124[3]  = 0;
    mArr124[2]  = 0;
    mArr124[1]  = 0;
    mArr124[0]  = 0;

    mObject = MapFx_ResolveOwner(obj);
    mShort120 = 1;
    mShort122 = 1;
    mTask = 0;
}


// movePcReleaseTaskReset - release active task, reset base (0x60 bytes)
// ============================================================================
extern "C" void movePcReleaseTaskReset(CCtrlMovePC* self) {
    if (self->mTask != 0) {
        void* gm = cf::CfGameManager::getGameSubManager();
        if (gm != 0) {
            void* p = &((cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2f2c))->field_0x00;
            if (p != 0) {
                releaseNode__17UnkClass_8047CD0CFv(p, self->mTask);
            }
        }
        self->mTask = 0;
    }
    moveBaseUnlink(self);
}

// ============================================================================
// movePcTickCounterWrap - tick counter; on wrap set data flag + transition (0x5C)
// ============================================================================
extern "C" int movePcTickCounterWrap(CCtrlMovePC* self) {
    s16 v = self->mArr124[1];          // 0x126
    self->mArr124[1] = (s16)(v + 1);
    if (v >= 8) {
        self->mArr124[1] = 0;
        u32* d = (u32*)self->mpDataPtr;
        d[1] |= 1u;                    // *(data+4) |= 1
        self->mStateFunc = lbl_eu_80532D28;
    }
    self->mpDataPtr->field_0x14 = 0.0f;
    return 0;
}

// ============================================================================
// movePcFlagTransitionReset - flag-driven state transition + counter reset (0xB0)
// ============================================================================
extern "C" int movePcFlagTransitionReset(CCtrlMovePC* self) {
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
    self->mpDataPtr->field_0x14 = lbl_eu_80667B60;
    return 0;
}

// ============================================================================
// movePcFacingSinCosUpdate - facing sin/cos update or ptmf transition (0xC4)
// ============================================================================
extern "C" int movePcFacingSinCosUpdate(CCtrlMovePC* self) {
    if (self->mFlags4C & 0x00040000u) {        // rlwinm. bit 13
        self->mVec90.y = lbl_eu_80667B60;      // 0x94 = 0.0
        cf::CfObjectActor* w = (cf::CfObjectActor*)self->mObject;
        f32 s = w->CfObject_getMoveHeadAngle();                 // vtable 0x5b4
        self->mVec90.x = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * s);
        f32 c = ((cf::CfObjectActor*)self->mObject)->CfObject_getMoveHeadAngle();
        self->mVec90.z = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * c);
        setMoveFacing((cf::CCtrlMoveBase*)self, (const ml::CVec3*)&self->mVec90, lbl_eu_80667B68);
    } else {
        self->mStateFunc = lbl_eu_80532C98;
        self->mpDataPtr->field_0x14 = lbl_eu_80667B60;
    }
    return 0;
}

// ============================================================================
// movePcCounterFacingUpdate
// ============================================================================
extern "C" int movePcCounterFacingUpdate(CCtrlMovePC* self) {
    s16 old = self->mArr124[15];               // 0x142
    self->mArr124[15] = (s16)(old + 1);
    if (old < 30) {
        cf::CfObjectActor* w = (cf::CfObjectActor*)self->mObject;
        void* sub = ((cf::CfObjWrap*)w)->mSub;
        u16 flag = ((cf::CfMoveSub*)sub)->mField530;
        if ((flag & 1u) == 0) {                // clrlwi. bit 31
            f32 a = w->CfObject_getMoveHeadAngle();             // vtable 0x5b4
            self->mVec90.x = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * a);
            f32 b = ((cf::CfObjectActor*)self->mObject)->CfObject_getMoveHeadAngle();
            self->mVec90.z = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * b);
            setMoveFacing((cf::CCtrlMoveBase*)self, (const ml::CVec3*)&self->mVec90, lbl_eu_80667B68);
            return 0;
        }
    }
    self->mArr124[15] = 0;
    self->mStateFunc = lbl_eu_80532D4C;
    return 0;
}

// Address anchor for the inline CfObjectMove at wrapper +0x3E9C
// (CfObjWrap models the same object; this exposes the move subobject).
struct CfWrapMoveAnchor {
    u8 _pad00[0x3e9c];
    u8 moveObj;
};


// movePcTopTickDispatch - top-level tick: query object, gate on battle state, run the
// state-machine dispatch loop (0xAC).
// ============================================================================
extern "C" void movePcTopTickDispatch(CCtrlMovePC* self) {
    cf::CfObject* emb = (cf::CfObject*)&((CfWrapMoveAnchor*)self->mObject)->moveObj;
    f32 v = emb->CfObject_getMoveSpeedRate();
    if (0.0f == v) {
        return;
    }

    // Ternary materialization: MWCC emits li 0 / cmplwi / blt / cmplwi /
    // bgt / li 1 for the nested ternary form (avoids the range-fold).
    int battleState = ((cf::CBattleManager*)cf::CBattleManager::getInstance())->mChain.unk0[2];
    bool inBattle = (u32)battleState >= 1 && (u32)battleState <= 0x18;
    if (inBattle) {
        return;
    }

    if (movePcUpdateTop(self) == 0) {
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
    void* gm = cf::CfGameManager::getGameSubManager();
    if (gm != 0) {
        void* p = (char*)gm + 0x2f2c;
        if (p != 0) {
            releaseNode__17UnkClass_8047CD0CFv(p, s->mTask);
        }
    }
    s->mTask = 0;
}

// ============================================================================
// movePcCondFullReset - conditional full reset + task release (0x198)
// ============================================================================
extern "C" void movePcCondFullReset(void* selfv, int flag) {
    CCtrlMovePC* self = (CCtrlMovePC*)selfv;
    if (flag != 0) {
        maskMoveBaseFlags((void*)self);
        self->mStateFunc = lbl_eu_80532B78;
        moveBaseUnlink(self);
        self->mFlags4C = 0x80000000u;
        self->mFlags50 = 0;
        void* task = self->mTask;
        self->mPos   = ml::CVec3::zero;
        self->mVec6C = ml::CVec3::zero;
        self->mVecA8 = ml::CVec3::zero;
        self->mVecD8 = ml::CVec3::zero;
        self->mVecB4 = ml::CVec3::zero;
        self->mVec90 = ml::CVec3::zero;
        self->mFloat100 = 0.0f;
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
// movePcResetAndSetPos - reset (gated on flag bit) then set position (0x1B0)
// ============================================================================
extern "C" void movePcResetAndSetPos(CCtrlMovePC* self, const Vec* pos) {
    if (self->mFlags4C & 0x00800000u) {        // rlwinm. bit 8
        maskMoveBaseFlags((void*)self);
        self->mStateFunc = lbl_eu_80532B84;
        moveBaseUnlink(self);
        self->mFlags4C = 0x80000000u;
        self->mFlags50 = 0;
        void* task = self->mTask;
        self->mPos   = ml::CVec3::zero;
        self->mVec6C = ml::CVec3::zero;
        self->mVecA8 = ml::CVec3::zero;
        self->mVecD8 = ml::CVec3::zero;
        self->mVecB4 = ml::CVec3::zero;
        self->mVec90 = ml::CVec3::zero;
        self->mFloat100 = 0.0f;
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
    maskMoveChildFlags((maskMoveChildFlags_obj*)self);
    if (self->mTask != 0) {
        void* gm = cf::CfGameManager::getGameSubManager();
        if (gm != 0) {
            void* p = (char*)gm + 0x2f2c;
            if (p != 0) {
                releaseNode__17UnkClass_8047CD0CFv(p, self->mTask);
            }
        }
        self->mTask = 0;
    }
    self->mFloatParam1 = 4.0f;
    self->mArr124[0] = 0;
    self->mArr124[1] = 0;
    self->mArr124[2] = 0;
    self->mArr124[3] = 0;
    self->mFloat100 = 0.0f;
    self->mArr124[9] = 0;
    self->mArr124[10] = 0;
    self->mArr124[13] = 0;
    self->mFlags50 = 0;
    if (self->mObject != 0) {
        ((cf::CfObjWrap*)self->mObject)->mField455A = 100;
    }
    self->mFloatParam2 = 0.25f;
    self->mFlags4C &= 0xE1634F1Fu;
    if (self->mPlayer != 0) {
    // --- proximity gate + contact handling ---
    // Retail control flow: every outcome here converges on the main per-tick
    // section; passing the checks merely sets the contact flag bit 0x80.
    {
        ml::CVec3* pl = ((cf::CfObject*)((char*)self->mPlayer + 0x3e9c))->CfObject_getPosVector();
        ml::CVec3* ob = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        ml::CVec3 d = *pl - *ob;
        if (!(d.x * d.x + d.y * d.y + d.z * d.z <= 400.0f
              || ml::math::abs(d.y) <= 8.0f)) {
            // Busy-state query twice: resolve an entity handle and run a
            // second proximity pass against it.
            if (((cf::CObjectParam*)((char*)self->mPlayer + 0x3e9c))->CObjectParam_getSelfObjectId() == 0) {
                self->mFlags4C |= 0x80u;
            } else {
                int id = (int)((cf::CObjectParam*)((char*)self->mPlayer + 0x3e9c))->CObjectParam_getSelfObjectId();
                void* ent = MapFx_ResolveOwner(findObjectById__Fi(id));
                if (ent == 0) {
                    self->mFlags4C |= 0x80u;
                } else {
                    ml::CVec3* pl2 = ((cf::CfObject*)((char*)self->mPlayer + 0x3e9c))->CfObject_getPosVector();
                    ml::CVec3* eb = ((cf::CfObject*)((char*)ent + 0x3e9c))->CfObject_getPosVector();
                    ml::CVec3 e = *pl2 - *eb;
                    if (e.x * e.x + e.y * e.y + e.z * e.z <= 400.0f
                        || ml::math::abs(e.y) <= 8.0f) {
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
    self->mPos = *((cf::CfObject*)((char*)self->mPlayer + 0x3e9c))->CfObject_getPosVector();
    self->mFloat100 = ((cf::CfObjectActor*)self->mPlayer)->CfObject_getMoveHeadAngle();   // vtable 0x5b4
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
        q = *(u32*)((cf::CObjectState*)objW->mField04)->getStateData();
        int hit10a = testActorStatus(objW, (int*)&q, 0xa);
        if (hit10a == 0) {
            q = *(u32*)((cf::CObjectState*)objW->mField04)->getStateData();
            if (testActorStatus(objW, (int*)&q, 0x9) != 0) {
                condC = 0;
            }
        }
        q = *(u32*)((cf::CObjectState*)objW->mField04)->getStateData();
        if (testActorStatus(objW, (int*)&q, 0x1a) != 0) {
            condA = 0;
        } else {
            q = *(u32*)((cf::CObjectState*)objW->mField04)->getStateData();
            if (testActorStatus(objW, (int*)&q, 0x19) == 0 && condC != 0) {
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
    bool inBattle = isGlobalCamFlagSet(0x40000000) != 0;
    if (!inBattle) {
        u8 phase = *(u8*)((char*)cf::CBattleManager::getInstance() + 0x1aa);
        if (!((u32)phase >= 1 && (u32)phase <= 0x18)) {
            u16 bf = ((cf::CBattleManager*)cf::CBattleManager::getInstance())->unk20C8.unk0;
            inBattle = (bf != 0);
        }
    }
    if (inBattle) {
        self->mVecA8 = *((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        self->mArr124[8] = 0;
        return;
    }

    // --- follow-anchor bookkeeping ---
    {
        ml::CVec3* op = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        ml::CVec3 delta = self->mVecA8 - *op;
        void* st = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getCurrentTarget();
        bool anchorOk = (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z <= 1.0f)
                        && (((cf::CfObjState*)st)->mField14 >= 0.10000000149011612f)
                        && (condC == 0);
        op = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        if (!anchorOk) {
            self->mVecA8 = *op;
            self->mArr124[8] = 0;
        } else {
            ml::CVec3 d2v = self->mPos - *op;
            if (d2v.x * d2v.x + d2v.y * d2v.y + d2v.z * d2v.z < 2500.0f) {
                s16 c = self->mArr124[8];
                self->mArr124[8] = (s16)(c + 1);
            } else {
                self->mVecA8 = *op;
                self->mArr124[8] = 0;
            }
        }
    }

    CGimmickGlobal* gs = getUnk80664658();
    if ((gs->mFlags & 0x8000u) && !(self->mFlags4C & 0x20u)) {
        ml::CVec3* pos = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        if (GimProbeLockAccept(gs, pos) != 0) {
            self->mFlags4C |= 0x20u;
        } else {
            self->mFlags4C |= 0x80u;
        }
    }

    // --- arrival gate: give up once the counter expires or we drift away ---
    {
        ml::CVec3* op = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        ml::CVec3 dv = self->mPos - *op;
        s16 cnt = self->mArr124[8];
        if (!(cnt < 0xD2 && dv.x * dv.x + dv.y * dv.y + dv.z * dv.z <= 10000.0f)) {
            if ((self->mFlags4C & 0x180u) == 0) {
                return;
            }
        }
    }

    // --- commit step: gate on the wrapper float, plan a party spot ---
    {
        f32 gate = ((cf::CActorParam*)self->mObject)->CActorParam_getHp();   // vtable 0x128
        if (gate <= 0.0f) {
            return;
        }
    }
    self->mArr124[8] = 0;
    ml::CVec3 tmp = self->mPos;                  // stack +0xa8 source spot
    u32 rnd = (u32)ml::math::mtRand() & 0xffffu;
    cf::CfPartyInfo info;                        // stack +0xe8 planner block
    InitRecord(&info, &tmp, 1, self->mFloat100,
                  2.0f, 4.0f, rnd);
    ml::CVec3 out;                               // stack +0xc0 candidate spot
    int planned = CPartsChange_ProcessPartyInfo(&info, &out);
    if (planned == 0) {
        // Planner failed: fall back to the global coli-probe result.
        ml::CVec3 nearPt = self->mPos
            + ml::CVec3(0.0f, 2.0f, 0.0f);
        ml::CVec3 farPt = self->mPos
            + ml::CVec3(0.0f, -2.0f, 0.0f);
        (void)nearPt;
        (void)farPt;
        if (((int (*)(void*, void*))ColiWalkSegment)(sEneColiProbe,
                (char*)self->mPlayer + 0x44a8) == 0) {
            return;
        }
        self->mVecB4.x = sEneColiProbe->field_0x60;
        self->mVecB4.y = sEneColiProbe->field_0x64;
        self->mVecB4.z = sEneColiProbe->field_0x68;
        self->mVecC0.y = 0.20000000298023224f + sEneColiProbe->field_0x64;
    }

    // --- move-task allocation and commit toward the candidate spot ---
    gs = getUnk80664658();
    if ((gs->mFlags & 0x8000u) && GimProbeLockAccept(gs, &out) != 0) {
        int okT;
        if (self->mTask == 0) {
            okT = 0;
            void* gm = cf::CfGameManager::getGameSubManager();
            if (gm != 0) {
                UnkClass_8047CD0C* mgr = (UnkClass_8047CD0C*)((char*)gm + 0x2f2c);
                if (*(void**)mgr != 0) {
                    self->mTask = mgr->allocFreeNode();
                    okT = (self->mTask != 0);
                }
            }
        } else {
            okT = 1;
        }
        if (okT != 0) {
            if (dispatchScnCommand__17UnkClass_8047D2ACFv(self->mTask, (Vec*)&self->mPos,
                    0.0f, 0.0f) != 0) {
                if (updateWalker__17UnkClass_8047D2ACFv(self->mTask, (Vec*)&self->mPos,
                        &out, 0.0f, 0.0f, 1) == 0) {
                    return;
                }
            }
        }
    }

    // --- final commit: raycast lifted positions, reset embedded move state ---
    {
        ml::CVec3 up(0.0f, 0.800000011920929f, 0.0f);   // (0, BC4, 0)
        ml::CVec3 diff = out + up;                   // stack +0x18 -> +0x54
        ml::CVec3 sum = self->mPos + up;             // stack +0x24 -> +0x6c
        if (ScnRes_SegQueryForward_E348(&sum, &diff, 0x40004a05, 0, 0) != 0) {
            return;
        }
    }
    self->mFlags4C &= ~0x100u;
    cf::CfObject* emb = (cf::CfObject*)((char*)self->mObject + 0x3e9c);
    emb->CfObject_syncMoveTarget(&out);                  // vtable 0xa8
    emb->CfObject_syncModelRate(1.0f);        // vtable 0x168
    CfObjectMove_setMoveSpeedGated((char*)self->mObject + 0x3e9c, 20.0f);
    cf::CfMoveSub* sub2 = ((cf::CfObjWrap*)self->mObject)->mSub;
    if (sub2 != 0) {
        sub2->mField3F0 = ml::CVec3::zero.x;
        sub2->mField3F4 = ml::CVec3::zero.y;
        sub2->mField3F8 = ml::CVec3::zero.z;
    }
    maskMoveChildFlags((maskMoveChildFlags_obj*)self);
    if (self->mTask != 0) {
        void* gm = cf::CfGameManager::getGameSubManager();
        if (gm != 0) {
            void* p = (char*)gm + 0x2f2c;
            if (p != 0) {
                releaseNode__17UnkClass_8047CD0CFv(p, self->mTask);
            }
        }
        self->mTask = 0;
    }
    }
}
// ============================================================================
// movePcUpdateWalk - walk-state update (0x2A0 bytes)
//
// Two entry modes keyed on flag 0x02000000:
//  - clear: steering update. If the stored facing is degenerate, reset the
//    tick counter; otherwise probe a ray from the object forward and either
//    rotate the stored facing (counter-gated) or steer to a spot beside the
//    object (moveBaseRoute).
//  - set: timed nudge of mVecC0 toward a fixed step with arrival gates,
//    then a two-iteration candidate-spot search around the object (probe
//    point + facing offset) committing through moveBaseRoute.
// Common tail drives the move task contact handling (wall slide) or the
// steering helper moveBaseUpdate.
// ============================================================================
extern "C" int movePcUpdateWalk(cf::CCtrlMovePC* self) {
    typedef int (*Probe2Fn)(void*, void*);
    ml::CVec3* goalPtr = &self->mVec54;   // r24 alias: vec fed to the tail helpers
    cf::CfObjWrap* plW = (cf::CfObjWrap*)self->mPlayer;

    if ((self->mFlags4C & 0x02000000u) == 0) {
        // ---- steering path ----
        u16 anim = plW->mField455A;
        plW->mField455A = anim - 3;
        if (self->mVec90.x == 0.0f && self->mVec90.z == 0.0f) {
            self->mArr124[3] = 0;
        } else {
            ml::CVec3 tgt;
            tgt.x = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()->x;
            tgt.y = 0.5f
                  + ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()->y;
            tgt.z = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()->z;
            ml::CVec3 dir = self->mVec90 * 2.0f;
            ml::CVec3 sum = tgt + dir;
            if (ColiWalkSegment(sEneColiProbe, (char*)self->mObject + 0x44a8,
                              &tgt, &sum, 0, 0, sEneColiProbeBuffer) == 0) {
                self->mArr124[3] = 0;
            } else {
                s16 cnt = self->mArr124[3];
                self->mArr124[3] = cnt + 1;
                if (cnt >= 15) {
                    self->mArr124[3] = 0;
                    f32 ang = Atan2FIdx__Q24nw4r4mathFff(self->mVec90.x, self->mVec90.z);
                    self->mFloat100 = 0.02454369328916073f * ang;
                    f32 a = 0.02454369328916073f * ang + ml::pi;
                    f32 c = CosFIdx__Q24nw4r4mathFf(40.7436637878418f * a);
                    f32 s = SinFIdx__Q24nw4r4mathFf(40.7436637878418f * a);
                    ml::CVec3 facing(s, 0.0f, c);
                    ml::CVec3 off = facing * 3.0f;
                    ml::CVec3 spot = *((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()
                                     + off;
                    self->mVecC0 = spot;
                    if (moveBaseRoute(self, &spot, 1) != 0) {
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
                ml::CVec3 step(0.0f, 0.5f, 0.0f);
                ml::CVec3 moved = self->mVecC0 + step;
                ml::CVec3 movedCopy = moved;
                (void)movedCopy;
                if (((Probe2Fn)Coli_SweepSegNodes)(sEneColiProbe,  // DEBT(callcast): C symbol; retail call view differs from the canonical typed declaration
                        (char*)self->mObject + 0x44a8)) {
                    self->mArr124[3] = 120;
                }
            }
        }
        if (self->mArr124[3] >= 120) {
            self->mArr124[3] = 0;
            self->mFlags4C &= 0xE1FFFFFFu;
        } else {
            ml::CVec3 delta = *((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()
                              - self->mVecC0;
            u32 flags = self->mFlags4C;
            if (flags & 0x04000000u) {
                if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z
                    <= 2.25f) {
                    self->mArr124[3] = 0;
                    self->mFlags4C = (flags | 0x80000000u) & ~0x04000000u;
                }
            } else {
                if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z
                    <= 1.0f) {
                    self->mArr124[3] = 0;
                    self->mFlags4C &= 0xE1FFFFFFu;
                }
            }
        }

        if (self->mFlags4C & 0x08000000u) {
            u16 anim = plW->mField455A;
            plW->mField455A = anim - 3;
            const f32 f28 = 0.5f;
            const f32 f29 = ml::quarterpi;
            const f32 f30 = f28 * f29;
            const f32 sideScale = 5.0f;
            int notCommitted = 1;   // r23
            for (int i = 0; i < 2; i++) {
                f32 ampBase = (i & 1) ? 1.0f : -1.0f;
                s16 cnt = self->mArr124[3];
                f32 t = f30 * (f32)(cnt & 1) + f29;
                f32 angle = ampBase * t + self->mFloat100;
                f32 cosv = CosFIdx__Q24nw4r4mathFf(40.7436637878418f * angle);
                f32 sinv = SinFIdx__Q24nw4r4mathFf(40.7436637878418f * angle);
                ml::CVec3 dir;
                dir.x = sinv;
                dir.y = 0.0f;
                dir.z = cosv;
                ml::CVec3 probe;
                probe.x = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()->x;
                probe.y = f28
                        + ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()->y;
                probe.z = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()->z;
                ml::CVec3 side = dir * sideScale;
                ml::CVec3 dest = probe + side;
                if (ColiWalkSegment(sEneColiProbe, (char*)self->mObject + 0x44a8,
                                  &probe, &dest, 0, 0, sEneColiProbeBuffer) != 0) {
                    continue;
                }
                if (moveBaseRoute(self, &dest, 1) == 0) {
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
                ml::CVec3 delta = *((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()
                                  - self->mVecC0;
                if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z
                    <= 0.25f) {
                    goalPtr = &self->mVec54;
                }
            }
        }
        self->mFlags4C &= ~0x10000u;
    }

    // ---- common tail: move-task contact / wall-slide handling ----
    if (self->mpSomePtr != 0) {
        ml::CVec3* tp = self->mpDataPtr->mPosObj->CfObject_getPosVector();
        if (updateWalker__17UnkClass_8047D2ACFv(self->mpSomePtr, (Vec*)tp, (Vec*)goalPtr,
                0.0f, 0.0f, 1) != 0) {
            // Contact: slide along the surface normal toward the goal.
            self->mFlags4C &= ~0x01000000u;
            ml::CVec3 delta = *goalPtr
                - *self->mpDataPtr->mPosObj->CfObject_getPosVector();
            ml::CVec3 dirTmp = delta;
            ml::CVec3 n = dirTmp;
            if (n.x != 0.0f || n.y != 0.0f || n.z != 0.0f) {
                if (n.x * n.x + n.y * n.y + n.z * n.z != 0.0f) {
                    PSVECNormalize((const Vec*)&n, (Vec*)&n);
                } else {
                    n = ml::CVec3::zero;
                }
            }
            ml::CVec3 out(self->mFloatParam2 * (n.x - self->mVelocity.x) + self->mVelocity.x,
                          0.0f,
                          self->mFloatParam2 * (n.z - self->mVelocity.z) + self->mVelocity.z);
            if (out.x != 0.0f || out.y != 0.0f
                || out.z != 0.0f) {
                if (out.x * out.x + out.y * out.y + out.z * out.z != 0.0f) {
                    PSVECNormalize((const Vec*)&out, (Vec*)&out);
                } else {
                    out = ml::CVec3::zero;
                }
            }
            if (self->mFloatParam2 > 0.15000000596046448f) {
                if (!(self->mFlags50 & 0x400u)) {
                    self->mFlags50 |= 0x400u;
                    ml::CVec3 probe;
                    probe.x = 0.5f * out.x
                            + self->mpDataPtr->mPosObj->CfObject_getPosVector()->x;
                    probe.y = 2.0f
                            + self->mpDataPtr->mPosObj->CfObject_getPosVector()->y;
                    probe.z = 0.5f * out.z
                            + self->mpDataPtr->mPosObj->CfObject_getPosVector()->z;
                    if (ScnRes_VertRayForward_E398(&probe, 0x4a05, 0, 0, -4.0f,
                                      0.0f) == 0) {
                        self->mFlags50 |= 0x800u;
                    } else if (ScnRes_EntryFlagThunk_E5A4(0x20000, 0) != 0) {
                        self->mFlags50 |= 0x1000u;
                    }
                }
                if (self->mFlags50 & 0x80000u) {
                    // Wall pressed: re-lerp with the tighter factor and latch bit 1.
                    out.x = 0.6000000238418579f * (n.x - self->mVelocity.x) + self->mVelocity.x;
                    out.y = 0.0f;
                    out.z = 0.6000000238418579f * (n.z - self->mVelocity.z) + self->mVelocity.z;
                    self->mFlags50 |= 2u;
                    if (out.x != 0.0f || out.y != 0.0f
                        || out.z != 0.0f) {
                        if (out.x * out.x + out.y * out.y + out.z * out.z
                            != 0.0f) {
                            PSVECNormalize((const Vec*)&out, (Vec*)&out);
                        } else {
                            out = ml::CVec3::zero;
                        }
                    }
                }
            }
            self->mVelocity.x = out.x;
            self->mVelocity.y = out.y;
            self->mVelocity.z = out.z;
            self->mVec90 = out;
            return 1;
        }
    }

    if (self->mFlags4C & 0x01000000u) {
        self->mFlags4C &= ~0x01000000u;
        if (moveBaseUpdate((cf::CfObjectMove*)self, &self->mVec90, goalPtr, 0, 0) != 0) {
            return 1;
        }
    } else {
        if (moveBaseUpdate((cf::CfObjectMove*)self, &self->mVec90, goalPtr, 1, 1) != 0) {
            return 1;
        }
    }
    self->mFlags50 |= 4u;
    return 0;
}
// ============================================================================
// movePcEnterApproach - approach-state entry (0x1D8 bytes)
//
// If the pair-move flag is set, switch to the paired-walk state and run the
// reposition step. Otherwise pick a goal position: reuse the stored goal when
// slope-locked, else query the party-info planner for one. When the planner
// succeeds and the task is missing, allocate a move task through the game
// manager's node pool and start it toward the goal. Finally advance to the
// walk-update state.
// ============================================================================
extern "C" int movePcEnterApproach(cf::CCtrlMovePC* self) {
    u32 f50 = self->mFlags50;
    self->mArr124[7] = 0;
    self->mFlags4C &= ~0x10000u;
    if (f50 & 2u) {
        // Paired movement: delegate to the reposition state handler.
        self->mStateFunc = lbl_eu_80532C14;
        self->mArr124[15] = 0;
        movePcSteerToGoal(self);
        return 0;
    }

    if (self->mFlags4C & 0x1000u) {
        // Slope/height locked: keep the previous target.
        self->mVec54 = self->mVecCC;
    } else {
        ml::CVec3 pos = self->mPos;
        cf::CfPartyInfo info;
        InitRecord(&info, &pos, 6, self->mFloat100, 2.0f,
                      0.0f, self->mShort120);
        if (CPartsChange_ProcessPartyInfo(&info, &self->mVec54) == 0) {
            // Planner failed: head straight for our own position.
            self->mFlags4C |= 1u;
            self->mVec54 = self->mPos;
        } else if (!(self->mFlags4C & 0x10000u)) {
            int ok;
            if (self->mTask == 0) {
                ok = 0;
                void* gm = cf::CfGameManager::getGameSubManager();
                if (gm != 0) {
                    UnkClass_8047CD0C* mgr = (UnkClass_8047CD0C*)((char*)gm + 0x2f2c);
                    if (*(void**)mgr != 0) {
                        self->mTask = mgr->allocFreeNode();
                        ok = (self->mTask != 0);
                    }
                }
            } else {
                ok = 1;
            }
            if (ok && updateWalker__17UnkClass_8047D2ACFv(self->mTask,
                    (Vec*)&self->mPos, &self->mVec54,
                    0.0f, 0.0f, 1) == 0) {
                self->mFlags4C |= 1u;
                self->mVec54 = self->mPos;
            }
        }
    }

    if (movePcUpdateWalk(self) == 0 && !(self->mFlags4C & 0x1000u)) {
        self->mFlags4C |= 1u;
        self->mVec54 = self->mPos;
    }
    self->mFlags4C |= 0x10u;
    self->mStateFunc = lbl_eu_80532C20;
    movePcUpdateApproach(self);
    self->mFlags4C &= ~0x18u;
    return 0;
}
// ============================================================================
// movePcUpdateApproach - approach-state update (0x9D8 bytes)
//
// Counts ticks in the state; on timeout transitions. Depending on the mode
// flags it either scans the party for a member blocking the path (bail out
// toward movePcUpdateFollow) or runs the walk-speed selection, obstacle probe and
// steering (setMoveFacing) logic below.
// ============================================================================
extern "C" int movePcUpdateApproach(cf::CCtrlMovePC* self) {
    typedef int (cf::CCtrlMovePC::*MoveFn)();

    s16 cnt = self->mArr124[7];              // 0x132 tick counter
    self->mArr124[7] = (s16)(cnt + 1);
    const MoveFnTable* tbl = (const MoveFnTable*)&lbl_eu_80532B60;
    if (cnt > 12) {
        self->mStateFunc = tbl->mEntries[17];
    }

    u32 f4C = self->mFlags4C;
    f32 speed;
    if (f4C & 0x00001000u) {
        if (self->mFlags50 & 1u) {
            // Party-blocked scan: bail if any other member is closer than us
            // to the goal or stands within BF8 of our current spot.
            speed = 0.4000000059604645f;
            int bail = 0;
            if (self->mDistFC <= 3.5f) {
                bail = 1;
            } else {
                for (int i = 1; i < 3; i++) {
                    cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
                    if (p == 0) continue;
                    if (self->mShort122 == i) continue;
                    ml::CVec3* pp = ((cf::CfObject*)p)->CfObject_getPosVector();
                    ml::CVec3 d = *pp - self->mPos;
                    if (!(d.x * d.x + d.y * d.y + d.z * d.z <= self->mDistFC * self->mDistFC)) break;
                    ml::CVec3* op = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
                    ml::CVec3* pp2 = ((cf::CfObject*)p)->CfObject_getPosVector();
                    ml::CVec3 d2 = *pp2 - *op;
                    if (!(d2.x * d2.x + d2.y * d2.y + d2.z * d2.z <= 12.25f)) break;
                    bail = 1;
                    break;
                }
            }
            if (bail != 0) {
                self->mStateFunc = tbl->mEntries[18];
                movePcUpdateFollow(self);
                return 0;
            }
        } else {
            if (f4C & 0x00000800u) {
                self->mFloatParam2 = 0.25f;
                speed = 0.5f;
            } else {
                speed = 0.10000000149011612f;
            }
            if (self->mDistFC >= 0.800000011920929f) {
                self->mStateFunc = tbl->mEntries[19];
            }
        }
    } else if (f4C & 0x00000400u) {
        speed = 1.0f;
        f32 lim;
        if (f4C & 0x00000200u) {
            lim = 7.200000286102295f;
        } else {
            lim = 6.0f;
        }
        if (self->mDistFC >= lim) {
            self->mStateFunc = tbl->mEntries[20];
        }
    } else if (f4C & 0x00010000u) {
        speed = 4.0f * (f32)self->mShort122;
        if (self->mDistFC >= speed) {
            self->mStateFunc = tbl->mEntries[21];
        }
    } else {
        speed = 1.0f;
        if (self->mDistFC >= 2.0f) {
            self->mStateFunc = tbl->mEntries[22];
        }
    }

    // --- goal distance bookkeeping ---
    ml::CVec3* pos = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
    ml::CVec3 delta = *pos - self->mVec54;
    f32 dist2;
    if (delta.y * delta.y <= 9.0f || (self->mFlags50 & 1u)) {
        dist2 = delta.x * delta.x + delta.z * delta.z;      // horizontal only
    } else {
        dist2 = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
    }

    f32 spd;
    if (dist2 < speed * speed) {
        // --- pick the walk speed for this tick ---
        if (!(f4C & 0x00001000u)) {
            u32 f4Cb = self->mFlags4C;
            if (dist2 < (4.0f + speed) * (4.0f + speed)) {
                if (!(f4Cb & 0x4u)) {
                    if (dist2 > (3.0f + speed) * (3.0f + speed)) {
                        self->mFlags4C |= 0x2u;
                    }
                    self->mArr124[5] = 0;
                    spd = 0.20000000298023224f;
                } else if (f4Cb & 0x00100000u) {
                    f32 th = 2.0f + speed;
                    if (dist2 < th * th) {
                        if (dist2 < 0.0f) {
                            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                        }
                        f32 inv = (dist2 > 0.0f) ? dist2 * nw4r::math::FrSqrt(dist2) : 0.0f;
                        spd = 0.30000001192092896f * (inv / th) + 0.10000000149011612f;
                    }
                }
            } else {
                self->mFlags4C |= 0x4u;
                if (dist2 > 64.0f) {
                    if (dist2 < 0.0f) {
                        Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                    }
                    f32 inv = (dist2 <= 0.0f) ? dist2 * nw4r::math::FrSqrt(dist2) : 0.0f;
                    spd = 0.10000000149011612f * (inv - 8.0f) + 1.0f;
                    self->mFloatParam2 = 0.25f;
                    if (spd > 1.5f) {
                        spd = 1.5f;
                    }
                }
            }
        }

        // Close-range arrival snap: drop the run/walk latch bits.
        if ((self->mFlags4C & 1u) && dist2 < (1.0f + speed) * (1.0f + speed)) {
            self->mFlags4C &= ~0x7u;
        }

        // --- ramp down when arriving / decelerate near the goal ---
        if (self->mFlags50 & 0x80000u) {
            if (spd != 0.0f) {
                u32 g = self->mFlags4C;
                s16 t = self->mArr124[4];
                self->mArr124[4] = (s16)(t + 1);
                if (g & 0x00001000u) {
                    if (t + 1 >= 15) {
                        self->mArr124[4] = 30;
                    } else {
                        spd = 0.0f;
                    }
                } else {
                    if (t + 1 >= 30) {
                        self->mArr124[4] = 30;
                    } else {
                        spd = 0.0f;
                    }
                }
            } else {
                self->mArr124[4] = 0;
            }
        }

        if (spd != 0.0f) {
            if (!(self->mFlags4C & 0x10u)) {
                movePcUpdateWalk(self);
            }
            u32 f50 = self->mFlags50;
            if (f50 & 0x20000u) {
                spd = 0.10000000149011612f;
            }
            if (!(f50 & 0x400u)) {
                // Obstacle probe ahead: steer around or mark blocked.
                self->mFlags50 |= 0x400u;
                ml::CVec3* p1 = self->mpDataPtr->mPosObj->CfObject_getPosVector();
                f32 pz = p1->z;
                ml::CVec3* p2 = self->mpDataPtr->mPosObj->CfObject_getPosVector();
                f32 tz = 0.5f * self->mVec90.z + pz;
                ml::CVec3* p3 = self->mpDataPtr->mPosObj->CfObject_getPosVector();
                f32 ty = 2.0f + p3->y;
                ml::CVec3* p4 = self->mpDataPtr->mPosObj->CfObject_getPosVector();
                ml::CVec3 probe(0.5f * self->mVec90.x + p4->x, ty, tz);
                if (ScnRes_VertRayForward_E398(&probe, 0x4a05, 0, 0, -4.0f, 0.0f) == 0) {
                    self->mFlags50 |= 0x800u;
                } else if (ScnRes_EntryFlagThunk_E5A4(0x20000, 0) != 0) {
                    self->mFlags50 |= 0x1000u;
                }
            }

            // Wall-slide hold: freeze while pressed against geometry.
            u32 f50b = self->mFlags50;
            if (f50b & 0x40000u) {
                if (self->mArr124[11] < 0x32) {
                    if (f50b & 0x800u) {
                        spd = 0.0f;
                        s16 c = self->mArr124[8];
                        self->mArr124[8] = (s16)(c + 1);
                        if (c + 1 >= 0x5a) {
                            self->mArr124[8] = 0;
                            if (self->mArr124[17] <= 0) {
                                u32 g = self->mFlags4C;
                                self->mFlags4C = (g & ~0x380u) | 0x8u;
                                self->mStateFunc = tbl->mEntries[23];
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
                self->mpDataPtr->field_0x14 = 0.0f;
                return 0;
            }

            int moved = 0;
            if (spd < 0.5f) {
                if (self->mArr124[0] >= 0xa) {
                    u32 g = self->mFlags4C;
                    if ((g & 0x2000u) || (f50e & 0x100u)) {
                        self->mArr124[1] = 0;
                    } else {
                        s16 c = self->mArr124[1];
                        self->mArr124[1] = (s16)(c + 1);
                        if (c >= 0xa) {
                            self->mArr124[1] = 0;
                            self->mStateFunc = tbl->mEntries[24];
                            moved = 1;
                        }
                    }
                }
            } else {
                self->mArr124[1] = 0;
            }
            if (moved != 0) {
                spd = 0.0f;
            }

            moved = 0;
            if (self->mArr124[0] >= 0x78) {
                s16 c = self->mArr124[2];
                self->mArr124[2] = (s16)(c + 1);
                if (c >= 0x1e) {
                    self->mArr124[2] = 0;
                    self->mStateFunc = tbl->mEntries[25];
                    moved = 1;
                }
            }
            if (moved != 0) {
                spd = 0.0f;
            }

            setMoveFacing((cf::CCtrlMoveBase*)self, (const ml::CVec3*)&self->mVec90, spd);
            return 0;
        }
    } else {
        // Overspeed: just face the movement direction.
        if (self->mFlags50 & 1u) {
            f32 ang = movePcProbeGoalAngle(self);
            self->mVec90.x = SinFIdx__Q24nw4r4mathFf(40.7436637878418f * ang);
            self->mVec90.z = CosFIdx__Q24nw4r4mathFf(40.7436637878418f * ang);
            setMoveFacing((cf::CCtrlMoveBase*)self, (const ml::CVec3*)&self->mVec90, 0.20000000298023224f);
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
    self->mpDataPtr->field_0x14 = 0.0f;
    return 0;
}
// ============================================================================
// movePcUpdateTop - top-level movement update for the controlled character (0x9BC)
//
// One-shot init (state fn, party-index detection), then per-tick: update the
// goal position / facing from the player object, drive the move-state machine
// transitions (mode gates + timers) and finally decide the camera-facing
// side. Returns 1 when the actor is ready to move this tick.
// ============================================================================
extern "C" int movePcUpdateTop(cf::CCtrlMovePC* self) {
    typedef int (cf::CCtrlMovePC::*MoveFn)();
    const MoveFnTable* tbl = (const MoveFnTable*)&lbl_eu_80532B60;   // ptmf constant table

    if ((self->mFlags4C & 0x800000u) == 0) {
        self->mFlags4C |= 0x00800000u;
        maskMoveChildFlags((maskMoveChildFlags_obj*)self);
        if (self->mTask != 0) {
            void* gm = cf::CfGameManager::getGameSubManager();
            if (gm != 0) {
                void* p = (char*)gm + 0x2f2c;
                if (p != 0) {
                    releaseNode__17UnkClass_8047CD0CFv(p, self->mTask);
                }
            }
            self->mTask = 0;
        }
        ml::CVec3* pos = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        self->mVecD8 = *pos;
        self->mFloatParam1 = 32.0f;      // 32.0
        ((cf::CfObjWrap*)self->mObject)->mField455A = 100;
        ctrlActUpdateData70(self->mpDataPtr);
        // Retail interleaves the ptmf-table address calc between the array
        // zeroing stores, so the assignment sits lexically after [17].
        self->mArr124[17] = 0;
        const MoveFn* sel = &tbl->mEntries[4];
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
    cf::CfPartyList* party = (cf::CfPartyList*)CtrlObjectParam_GetSlotTableBase();
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
        self->mpDataPtr->field_0x14 = 0.0f;
        return 0;
    }
    if (mine == 0) {
        self->mpDataPtr->field_0x14 = 0.0f;
        return 0;
    }
    if (((cf::CfObjWrap*)self->mObject)->mSub == 0) {
        self->mpDataPtr->field_0x14 = 0.0f;
        return 0;
    }
    if (((cf::CfObjWrap*)self->mPlayer)->mSub == 0) {
        self->mpDataPtr->field_0x14 = 0.0f;
        return 0;
    }

    self->mVec6C = self->mPos;
    ml::CVec3* pp = ((cf::CfObject*)((char*)self->mPlayer + 0x3e9c))->CfObject_getPosVector();
    self->mPos = *pp;
    self->mFloat100 = ((cf::CfObjectActor*)self->mPlayer)->CfObject_getMoveHeadAngle();
    if (self->mFlags4C & 0x80000000u) {
        self->mFlags4C &= 0x7FFFFFFFu;
        self->mVec6C = self->mPos;
    }

    ml::CVec3* op = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
    self->mVec84.x = self->mPos.x - op->x;
    self->mVec84.y = self->mPos.y - op->y;
    self->mVec84.z = self->mPos.z - op->z;
    self->mDistFC = PSVECMag((const Vec*)&self->mVec84);
    f32 mag = (f32)(double)self->mDistFC;
    if (mag != 0.0f) {
        f32 k = 1.0f / mag;
        ml::CVec3 t;
        t.x = self->mVec84.x * k;
        t.y = self->mVec84.y * k;
        t.z = self->mVec84.z * k;
        self->mVec78 = t;
    } else {
        self->mVec78.x = 0.0f;
        self->mVec78.y = 0.0f;
        self->mVec78.z = 1.0f;
    }

    self->mFlags50 &= 0xFFF862FDu;
    self->mFlags4C &= 0xFFE3E15Fu;
    self->mFloatParam2 = 0.10000000149011612f;       // 0.1

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
                self->mVec9C.y += 1.0f;   // 1.0
            } else if ((fl & 0x20) != 0) {
                self->mVec9C = self->mPos;
                self->mVec9C.y -= 1.0f;
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
                ml::CVec3* p = ((cf::CfObject*)((char*)self->mPlayer + 0x3e9c))->CfObject_getPosVector();
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
                self->mVec9C.y -= 2.0f;   // 2.0
            }
        } else if ((self->mFlags50 & 0x40u) == 0 && (plr->mField0C & 0x2u)) {
            self->mFlags50 |= 0x40u;
            self->mVec9C.y = self->mPos.y - 2.0f;
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
        CGimmickGlobal* gs = getUnk80664658();
        if (gs->mFlags & 0x8000u) {
            ml::CVec3* pos = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
            if (GimProbeLockAccept(getUnk80664658(), pos) == 0) {
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
            ml::CVec3* pos = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
            f32 dx = self->mVecCC.x - pos->x;
            ml::CVec3* pos2 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
            f32 dz = self->mVecCC.z - pos2->z;
            if (dx * dx + dz * dz <= 0.25f) {   // 0.25
                self->mFlags4C |= 0x2000u;
                self->mVecCC = self->mPos;
            }
        }
    }

    if (own->mField0C & 0x200000u) {
        self->mFlags4C |= 0x600u;
    } else if (own->mField3F8 * own->mField3F8 + own->mField3F0 * own->mField3F0 >= 25.0f) {
        self->mFlags4C |= 0x400u;
    }

    void* st = ((cf::CfObject*)((char*)self->mPlayer + 0x3e9c))->CfObject_getCurrentTarget();
    if (st != 0) {
        f32 h = ((cf::CfObjState*)st)->mField14;
        if (h >= 0.6000000238418579f) {       // 0.6
            self->mFlags4C |= 0x80000u;
        }
        if (h < 0.10000000149011612f) {        // 0.1
            self->mFlags4C |= 0x100000u;
        }
    }

    movePcUpdatePairFollow(self);

    cf::CfObjWrap* obj = (cf::CfObjWrap*)self->mObject;
    u32 x;
    void* sub = obj->mField04;
    void* r = ((cf::CObjectState*)sub)->getStateData();
    x = *(u32*)r;
    if (testActorStatus(obj, (int*)&x, 4) != 0) {
        self->mFlags4C |= 0x40000u;
    }
    cf::CfObjWrap* pObj = (cf::CfObjWrap*)self->mPlayer;
    void* sub2 = pObj->mField04;
    void* r2 = ((cf::CObjectState*)sub2)->getStateData();
    u32 x2;
    void* subP = pObj->mField04;
    void* rP = ((cf::CObjectState*)subP)->getStateData();
    x2 = *(u32*)rP;
    if (testActorStatus(pObj, (int*)&x2, 4) != 0) {
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
    ml::CVec3* op2 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
    dd.x = self->mVecA8.x - op2->x;
    dd.y = self->mVecA8.y - op2->y;
    dd.z = self->mVecA8.z - op2->z;
    if (dd.magnitude() <= 1.0f) {   // 1.0
        s16 v = self->mArr124[0];
        self->mArr124[0] = (s16)(v + 1);
        if (v + 1 >= 1000) {
            self->mArr124[0] = 1000;
        }
    } else {
        ml::CVec3* op3 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        self->mVecA8 = *op3;
        self->mArr124[0] = 0;
        self->mArr124[1] = 0;
        self->mArr124[2] = 0;
    }

    dd.x = self->mVecB4.x - self->mPos.x;
    dd.y = self->mVecB4.y - self->mPos.y;
    dd.z = self->mVecB4.z - self->mPos.z;
    if (dd.magnitude() <= 1.0f) {   // 1.0
        self->mFlags4C |= 0x20000000u;
    } else {
        self->mFlags4C &= 0xDFFFFFFFu;
        self->mVecB4 = self->mPos;
        self->mArr124[1] = 0;
        self->mArr124[2] = 0;
    }

    if ((self->mFlags4C & 0x8u) == 0) {
        if (own->mField530 & 1) {
            if (((cf::CObjectState*)((char*)self->mPlayer + 0x3e9c))->checkStateFlags(0x100) != 0) {
                if (self->mArr124[17] <= 0) {
                    self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                    const MoveFn* sel = &tbl->mEntries[5];
                    self->mStateFunc = *sel;
                    self->mArr124[7] = 0;
                }
            } else if (self->mFlags4C & 0x100u || self->mDistFC >= 100.0f) {
                if (self->mArr124[17] <= 0) {
                    self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                    const MoveFn* sel = &tbl->mEntries[6];
                    self->mStateFunc = *sel;
                    self->mArr124[7] = 0;
                }
            } else if ((self->mFlags50 & 0x100u) && self->mDistFC < 10.0f) {
                if (self->mArr124[17] <= 0) {
                    self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                    const MoveFn* sel = &tbl->mEntries[7];
                    self->mStateFunc = *sel;
                    self->mArr124[7] = 0;
                }
            } else {
                    ml::CVec3 d4;
                    d4.x = self->mPos.x - self->mVec6C.x;
                    d4.y = self->mPos.y - self->mVec6C.y;
                    d4.z = self->mPos.z - self->mVec6C.z;
                    if (d4.magnitude() < 16.0f) {   // 16
                        if (self->mArr124[17] <= 0) {
                            self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                            const MoveFn* sel = &tbl->mEntries[8];
                            self->mStateFunc = *sel;
                            self->mArr124[7] = 0;
                        }
                    } else if ((self->mFlags4C & 0x40000u) && self->mDistFC < 40.0f) {  // 40
                        if (self->mArr124[17] <= 0) {
                            self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                            const MoveFn* sel = &tbl->mEntries[9];
                            self->mStateFunc = *sel;
                            self->mArr124[7] = 0;
                        }
                    } else if (self->mDistFC >= ((self->mFlags4C & 0x80000u) ? 20.0f : 15.0f)) {
                        s16 v = self->mArr124[9];
                        self->mArr124[9] = (s16)(v + 1);
                        if (v + 1 >= 1200) {
                            self->mArr124[9] = 0;
                            if (self->mArr124[17] <= 0) {
                                self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                                const MoveFn* sel = &tbl->mEntries[10];
                                self->mStateFunc = *sel;
                                self->mArr124[7] = 0;
                            }
                        } else {
                            ml::CVec3* op4 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
                            ml::CVec3 dD8;
                            dD8.x = self->mVecD8.x - op4->x;
                            dD8.y = self->mVecD8.y - op4->y;
                            dD8.z = self->mVecD8.z - op4->z;
                            if (dD8.magnitude() <= 225.0f) {   // 225
                                s16 v2 = self->mArr124[10];
                                self->mArr124[10] = (s16)(v2 + 1);
                                if (v2 + 1 >= 300) {
                                    self->mArr124[10] = 0;
                                    if (self->mArr124[17] <= 0) {
                                        self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                                        const MoveFn* sel = &tbl->mEntries[11];
                                        self->mStateFunc = *sel;
                                        self->mArr124[7] = 0;
                                    }
                                }
                            } else {
                                ml::CVec3* op5 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
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
        f32 ang = self->mFloat100 + ml::halfpi;   // + pi/2
        f32 s = SinFIdx__Q24nw4r4mathFf(40.7436637878418f * ang);
        f32 c = CosFIdx__Q24nw4r4mathFf(40.7436637878418f * ang);
        f32 dir = self->mVec78.x * s + self->mVec78.z * c;
        if (self->mShort120 == 1) {
            if (dir > 0.699999988079071f) {
                self->mShort120 = 2;
            } else {
                self->mShort120 = 1;
            }
        } else {
            if (dir < -0.699999988079071f) {
                self->mShort120 = 1;
            } else {
                self->mShort120 = 2;
            }
        }
        self->mFlags50 &= ~0x80000u;
    } else {
        cf::CfObjectMove* p = cf::CfGameManager::getPlayer(other);
        cf::CfObjState* st2 = (cf::CfObjState*)((cf::CfObject*)p)->CfObject_getCurrentTarget();
        if (st2 != 0) {
            if (st2->mField1AC == 2) {
                self->mShort120 = 1;
            } else {
                self->mShort120 = 2;
            }
        }
        self->mFlags50 |= 0x80000u;
    }

    if (self->mpSomePtr != 0) {
        own->mField0C &= ~0x3000u;
        ((cf::CfBase30Obj*)self->mpSomePtr)->mField22 = 6;
    }

    if (self->mFlags4C & 0x8u) {
        CGimmickGlobal* gs = getUnk80664658();
        if (self->mShort122 == 1) {
            gs->mFlags |= 0x10000u;
        } else {
            gs->mFlags |= 0x20000u;
        }
    }

    if ((gmPauseFlags & 0x4000000u) == 0) {
        if (((cf::CBattleManager*)cf::CBattleManager::getInstance())->unk20C8.unk0 != 0) {
            self->mpDataPtr->field_0x14 = 0.0f;
            return 0;
        }
        if (isGlobalCamFlagSet(0x40000000) != 0) {
            self->mpDataPtr->field_0x14 = 0.0f;
            return 0;
        }
        cf::CfObjWrap* obj2 = (cf::CfObjWrap*)self->mObject;
        u32 y;
        void* sub3 = obj2->mField04;
        void* stateData = ((cf::CObjectState*)sub3)->getStateData();
        y = *(u32*)stateData;
        if (testActorStatus(obj2, (int*)&y, 8) != 0) {
            self->mpDataPtr->field_0x14 = 0.0f;
            return 0;
        }
        f32 g = ((cf::CActorParam*)self->mObject)->CActorParam_getHp();
        if (g > 0.0f) {
            return 1;
        }
    }

    self->mpDataPtr->field_0x14 = 0.0f;
    return 0;
}

// ============================================================================
// movePcSteerToGoal - state update: reposition / steer toward the goal (0x63C)
//
// Picks the goal from mVecCC/mPos, computes the delta to the object position,
// scans the party for the closest member (to bail toward), then applies the
// distance-gated state transitions and finally steers the facing direction
// toward the movement vector (setMoveFacing).
// ============================================================================
extern "C" int movePcSteerToGoal(cf::CCtrlMovePC* self) {
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
        ml::CVec3* p = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        dGoal = *p - goalSel;
    }
    ml::CVec3 dSelf;
    {
        ml::CVec3* p = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
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
        ml::CVec3* pp = ((cf::CfObject*)p)->CfObject_getPosVector();
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
        ml::CVec3* bp = ((cf::CfObject*)best)->CfObject_getPosVector();
        ml::CVec3* op = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        ml::CVec3 db = *op - *bp;
        f32 bd = db.x * db.x + db.y * db.y + db.z * db.z;
        if (dist > bd) {
            dist = bd;
            dSelf = db;
        }
    }

    f32 spd = 1.0f;      // move speed fed to the steer helper
    f32 slopeK = 0.3330000042915344f;   // side-slip scale
    int act = 0;
    u32 f50 = self->mFlags50;
    if (f50 & 0x4u) {
        if (f50 & 0x20u) {
            if (dist >= 9.0f) {
                if (!(self->mFlags4C & 0x1000u) && best != 0) {
                    dGoal = dSelf;
                }
                act = 1;
            } else {
                self->mFlags50 = f50 & ~0x24u;
            }
        } else if (dist <= 9.0f) {
            // Overshot: aim the opposite way from the goal.
            dGoal = dSelf * -1.0f;
            spd = 1.2000000476837158f;
            slopeK = 0.6000000238418579f;
            act = 1;
        } else {
            self->mFlags50 = f50 & ~0x4u;
        }
    } else if (dist >= 10.889999389648438f) {
        if (!(self->mFlags4C & 0x1000u) && best != 0) {
            dGoal = dSelf;
        }
        self->mFlags50 = f50 | 0x24u;
        act = 1;
    } else if (!(self->mFlags4C & 0x100000u)) {
        if (dist <= 5.289999961853027f) {
            self->mFlags50 = f50 | 0x4u;
            dGoal = dSelf * -1.0f;
            spd = 1.2000000476837158f;
            slopeK = 0.6000000238418579f;
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
            40.7436637878418f * ((cf::CfObjectActor*)self->mObject)->CfObject_getMoveHeadAngle());
        f32 sinA = SinFIdx__Q24nw4r4mathFf(
            40.7436637878418f * ((cf::CfObjectActor*)self->mObject)->CfObject_getMoveHeadAngle());

        ml::CVec3 facing(sinA, 0.0f, cosA);
        ml::CVec3 up(0.0f, 1.0f, 0.0f);
        ml::CVec3 right;
        PSVECCrossProduct((const Vec*)&facing, (const Vec*)&up, (Vec*)&right);
        // Right vector is flattened onto the XZ plane before normalizing.
        right.y = 0.0f;
        {
            f32 rl = right.x * right.x + right.y * right.y + right.z * right.z;
            if (rl == 0.0f) {
                right = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)&right, (Vec*)&right);
            }
        }

        ml::CVec3 work = dGoal;
        dist = dGoal.x * dGoal.x + dGoal.z * dGoal.z;   // horizontal length^2
        if (0.25f >= dist) {                  // 0.25: stand still
            work.x = 0.0f;
            work.z = 0.0f;
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
                k = 1.0f;
            } else {
                k = slopeK;
                slopeK = 1.0f;
            }
            if (dist < 0.0f) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 inv;
            if (dist > 0.0f) {
                inv = dist * nw4r::math::FrSqrt(dist);
            } else {
                inv = 0.0f;
            }
            if (inv * k < slopeK * ml::math::abs(dGoal.y)) {
                // Slope case: climb straight up the surface toward the goal.
                int hit = 1;
                ml::CVec3 v(0.0f, work.y, 0.0f);
                {
                    f32 vl = v.x * v.x + v.y * v.y + v.z * v.z;
                    if (vl == 0.0f) {
                        v = ml::CVec3::zero;
                    } else {
                        PSVECNormalize((const Vec*)&v, (Vec*)&v);
                    }
                }
                ml::CVec3 scaled = v * 0.30000001192092896f;   // 0.3
                ml::CVec3* p3 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
                ml::CVec3 a = *p3 - scaled;
                ml::CVec3 diff = a - facing;
                ml::CVec3 sum = a + facing;
                if (ScnRes_SegQueryForward_E348(&diff, &sum, 0, 0x2000, 0) == 0) {
                    if (dGoal.x != 0.0f || dGoal.z != 0.0f) {
                        hit = 0;
                        work = ml::CVec3(dGoal.x, 0.0f, dGoal.z);
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
                ml::CVec3 v(work.x, 0.0f, work.z);
                {
                    f32 vl = v.x * v.x + v.y * v.y + v.z * v.z;
                    if (vl == 0.0f) {
                        v = ml::CVec3::zero;
                    } else {
                        PSVECNormalize((const Vec*)&v, (Vec*)&v);
                    }
                }
                ml::CVec3 scaled = v * 0.30000001192092896f;   // 0.3
                ml::CVec3* p3 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
                ml::CVec3 a = *p3 - scaled;
                ml::CVec3 diff = a - facing;
                ml::CVec3 sum = a + facing;
                if (ScnRes_SegQueryForward_E348(&diff, &sum, 0, 0x2000, 0) == 0) {
                    if (dGoal.y != 0.0f) {
                        hit = 0;
                        work = ml::CVec3(0.0f, dGoal.y, 0.0f);
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
            if (wl == 0.0f) {
                work = ml::CVec3::zero;
            } else {
                PSVECNormalize((const Vec*)&work, (Vec*)&work);
            }
        }
        self->mVec90.y = 0.0f;
        self->mVec90.x = facing.x * (-work.y);
        self->mVec90.z = facing.z * (-work.y);
        dist = work.x * work.x + work.z * work.z;
        if (dist < 0.0f) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 inv2;
        if (dist > 0.0f) {
            inv2 = dist * nw4r::math::FrSqrt(dist);
        } else {
            inv2 = 0.0f;
        }
        f32 dot = work.x * right.x + work.z * right.z;
        if (dot >= 0.0f) {
            inv2 = -inv2;
        }
        self->mVec90.x = right.x * inv2 + self->mVec90.x;
        self->mVec90.z = right.z * inv2 + self->mVec90.z;
        setMoveFacing((cf::CCtrlMoveBase*)self, (const ml::CVec3*)&self->mVec90, spd);
        return 0;
    }

    self->mArr124[15] = 0;
    self->mFlags4C = (self->mFlags4C | 0x01000000u) & 0xE1FFFFFFu;
    self->mArr124[2] = 0;
    self->mArr124[1] = 0;
    self->mArr124[3] = 0;
    self->mpDataPtr->field_0x14 = 0.0f;
    return 0;
}
// ============================================================================
// movePcUpdateFollow - state update: scan the other party members for one inside
// our personal space (within mDistFC of mPos), aim the goal at it, and run
// the distance-gated flag transitions / steering helper.
// ============================================================================
extern "C" int movePcUpdateFollow(cf::CCtrlMovePC* self) {
    if ((self->mFlags4C & 0x1000u) && !(self->mFlags50 & 0x2u)) {
        ml::CVec3 goal = self->mVec78;
        f32 bestLen = self->mDistFC;

        for (int i = 1; i < 3; i++) {
            cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
            if (p != NULL && self->mShort122 != i) {
                ml::CVec3* pp = ((cf::CfObject*)p)->CfObject_getPosVector();
                ml::CVec3 d;
                ml::CVec3* pd = &d;
                nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(pd),
                                    reinterpret_cast<nw4r::math::VEC3*>(pp),
                                    reinterpret_cast<nw4r::math::VEC3*>(&self->mPos));
                f32 d2 = pd->x * pd->x + pd->y * pd->y + pd->z * pd->z;
                if (d2 > self->mDistFC * self->mDistFC) break;

                // Distance between the member and our own embedded object; warn if
                // it slipped below the threshold.
                ml::CVec3* op = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
                ml::CVec3* pp2 = ((cf::CfObject*)p)->CfObject_getPosVector();
                nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(pd),
                                    reinterpret_cast<nw4r::math::VEC3*>(pp2),
                                    reinterpret_cast<nw4r::math::VEC3*>(op));
                f32 e2 = pd->x * pd->x + pd->y * pd->y + pd->z * pd->z;
                if (e2 < self->mDistFC * self->mDistFC) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }

                bestLen = 0.0f;
                if (e2 > 0.0f) {
                    bestLen = e2 * nw4r::math::FrSqrt(e2);
                }
                if (bestLen != 0.0f) {
                    ml::CVec3 tmp;
                    nw4r::math::VEC3Scale(reinterpret_cast<nw4r::math::VEC3*>(&tmp),
                                          reinterpret_cast<nw4r::math::VEC3*>(pd),
                                          1.0f / bestLen);
                    ml::CVec3 tmp2(tmp);
                    goal = tmp2;
                }
                break;
            }
        }

        if (bestLen < 4.0f) {
            if (bestLen >= 3.0f) {
                self->mFlags4C &= ~0x20000000u;
            } else if (self->mFlags4C & 0x20000000u) {
                // Steer toward the chosen target via the move helper.
                ml::CVec3* op2 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
                ml::CVec3 delta;
                nw4r::math::VEC3Sub(reinterpret_cast<nw4r::math::VEC3*>(&delta),
                                    reinterpret_cast<nw4r::math::VEC3*>(op2),
                                    reinterpret_cast<nw4r::math::VEC3*>(&goal));
                ml::CVec3 out(delta);
                if (moveBaseRoute(self, &out, 1) != 0) {
                    moveBaseChase(self, &self->mVec90, &out);
                    setMoveFacing((cf::CCtrlMoveBase*)self, (const ml::CVec3*)&self->mVec90, 0.20000000298023224f);
                    return 0;
                }
            } else if (bestLen < 2.200000047683716f) {
                self->mFlags4C |= 0x20000000u;
            }
        }

        self->mArr124[2] = 0;
        self->mFlags4C = (self->mFlags4C | 0x100u) & ~0x0F000000u;
        self->mArr124[1] = 0;
        self->mArr124[3] = 0;
        self->mpDataPtr->field_0x14 = 0.0f;
        return 0;
    }

    self->mFlags4C &= ~0x20000000u;
    self->mStateFunc = lbl_eu_80532CBC;
    return 0;
}
// ============================================================================
// movePcUpdateReposition - reposition state update: pick a spot beside the party
// leader (or in front), steer the candidate toward it, and commit the move
// (0x9BC bytes). Returns 0 on every path.
// ============================================================================
extern "C" int movePcUpdateReposition(cf::CCtrlMovePC* self) {
    self->mFlags4C |= 0x8;
    self->mpDataPtr->field_0x14 = 0.0f;

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
                angle = ml::quarterpi;
            } else {
                angle = -ml::quarterpi;
            }
            radius = 1.7999999523162842f;
        } else {
            angle = (f32)(u32)ml::math::mtRand(0x168) * ml::deg2rad;
            radius = 3.0f * (0.10000000149011612f * (f32)(u32)ml::math::mtRand(0xa))
                     + 2.4000000953674316f;
        }

        // Rotate the local up vector into world space by facing yaw, then by
        // the approach angle, to get the lateral offset direction.
        f32 s1 = SinFIdx__Q24nw4r4mathFf(40.7436637878418f * self->mFloat100);
        f32 c1 = CosFIdx__Q24nw4r4mathFf(40.7436637878418f * self->mFloat100);
        Mtx mY = {
            {c1, 0.0f, s1, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {-s1, 0.0f, c1, 0.0f},
        };
        ml::CVec3 v(0.0f, -1.0f, 0.0f);
        f32 s2 = SinFIdx__Q24nw4r4mathFf(40.7436637878418f * angle);
        f32 c2 = CosFIdx__Q24nw4r4mathFf(40.7436637878418f * angle);
        Mtx mZ = {
            {c2, -s2, 0.0f, 0.0f},
            {s2, c2, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
        };
        PSMTXConcat(mZ, mY, mY);
        nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v,
            (const nw4r::math::MTX34*)mY, (const nw4r::math::VEC3*)&v);
        v = v * radius;
        target = target + v;

        f32 c3 = CosFIdx__Q24nw4r4mathFf(40.7436637878418f * self->mFloat100);
        f32 s3 = SinFIdx__Q24nw4r4mathFf(40.7436637878418f * self->mFloat100);
        ml::CVec3 facing(s3, 0.0f, c3);
        ml::CVec3 diff = target - facing;
        ml::CVec3 sum = target + facing;
        if (ScnRes_SegQueryForward_E348(&diff, &sum, 0, 0x2000, 0) == 0) {
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
            ml::CVec3* pp = ((cf::CfObject*)((char*)p + 0x3e9c))->CfObject_getPosVector();
            v = *pp - target;
            if (v.x * v.x + v.z * v.z < 0.4899999797344208f
                && ml::math::abs(v.y) < 2.0f) {
                clear = 0;
            }
        }
        if (clear == 0) {
            return 0;
        }

        ScnRes_CopyEntryMid(&v, 0);
        if (v.x * v.x + v.y * v.y + v.z * v.z == 0.0f) {
            v = ml::CVec3::zero;
        } else {
            PSVECNormalize((const Vec*)&v, (Vec*)&v);
        }
        ScnRes_CopyEntryHead(&goal, 0);
        goal.x += v.x * 0.4000000059604645f;
        goal.y += v.y * 0.4000000059604645f;
        goal.z += v.z * 0.4000000059604645f;
        self->mArr124[9] = 0;
        self->mStateFunc = lbl_eu_80532CC8;
        self->mArr124[15] = 0;
    } else {
        // Normal mode: query the party-spot builder for a candidate.
        u32 f4C = self->mFlags4C;
        if (!(f4C & 0x4000)) {
            self->mFlags4C |= 0x20000;
            ml::CVec3 v0 = self->mPos;
            cf::CfPartyInfo p1;
            InitRecord(&p1, &v0, 6, self->mFloat100, 2.0f,
                          0.0f, self->mShort120);
            self->mArr124[9] = 0;
            p1.field_2E = 1;
            if (CPartsChange_ProcessPartyInfo(&p1, &goal) == 0) {
                return 0;
            }
            if (p1.field_2C != 0) {
                height = p1.field_24;
                flag = 1;
            }
        } else {
            ml::CVec3 v0 = self->mPos;
            u32 rnd = ml::math::mtRand();
            cf::CfPartyInfo p2;
            InitRecord(&p2, &v0, 1, self->mFloat100, 2.0f,
                          4.0f, rnd & 0xffff);
            s16 cnt = self->mArr124[9];
            self->mArr124[9] = (s16)(cnt + 1);
            if (cnt > 3) {
                self->mArr124[9] = 5;
            } else {
                p2.field_2E = 1;
            }
            if (CPartsChange_ProcessPartyInfo(&p2, &goal) == 0) {
                // No candidate: probe straight down/up for walkable floor.
                ml::CVec3 vHi = self->mPos;
                vHi.y += -2.0f;
                ml::CVec3 vLo = self->mPos;
                vLo.y += 2.0f;
                if (ColiWalkSegment(sEneColiProbe,
                    (char*)self->mPlayer + 0x44a8, &vHi, &vLo, 2, 1, 0) == 0) {
                    return 0;
                }
                ml::CVec3 tmp = *(ml::CVec3*)sEneColiProbe;
                goal.y = tmp.y + 0.20000000298023224f;
            } else if (p2.field_2C != 0) {
                height = p2.field_24;
                flag = 1;
            }
        }
        self->mStateFunc = lbl_eu_80532CD4;
    }

    // Common tail: raycast current pos -> candidate pos (both lifted by BC4),
    // then commit the reposition state.
    ml::CVec3 vA = self->mPos;
    vA.y += 0.800000011920929f;
    ml::CVec3 vB(goal.x, goal.y, goal.z);
    vB.y += 0.800000011920929f;
    if (ScnRes_SegQueryForward_E348(&vA, &vB, 0x44a05, 0, 0) != 0) {
        return 0;
    }

    cf::CfObjWrap* obj = (cf::CfObjWrap*)self->mObject;
    obj->mField455A = 0x64;
    if (flag != 0) {
        if (goal.y - height >= 1.7999999523162842f) {
            goal.y -= 1.7999999523162842f;
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
    cf::CfObject* emb = (cf::CfObject*)((char*)self->mObject + 0x3e9c);
    emb->CfObject_syncMoveTarget((const ml::CVec3*)&goal);
    emb->CfObject_setMoveYaw(self->mFloat100);
    emb->CfObject_syncModelRate(1.0f);
    CfObjectMove_setMoveSpeedGated((char*)self->mObject + 0x3e9c, 20.0f);

    if (obj->mSub != 0) {
        cf::CfMoveSub* sub2 = obj->mSub;
        setAnimPosVec(sub2, &ml::CVec3::zero);
        sub2->mField3F0 = ml::CVec3::zero.x;
        sub2->mField3F4 = ml::CVec3::zero.y;
        sub2->mField3F8 = ml::CVec3::zero.z;
    }
    maskMoveChildFlags((maskMoveChildFlags_obj*)self);
    if (self->mTask != 0) {
        void* gm = cf::CfGameManager::getGameSubManager();
        if (gm != 0) {
            void* p = (char*)gm + 0x2f2c;
            if (p != 0) {
                releaseNode__17UnkClass_8047CD0CFv(p, self->mTask);
            }
        }
        self->mTask = 0;
    }
    self->mFlags4C &= 0xFFFDFEB7u;
    return 0;
}
// ============================================================================
// movePcTickState - per-tick movement state update (0x69C bytes)
//
// Chooses a new state-fn ptmf from mFlags4C/mFlags50 and the current approach
// distance; when no transition happened (flag stays 1) it runs the steering
// logic: turn toward the target, collision-check the goal, and lerp the
// velocity toward the facing direction (moveBaseLerp).
// ============================================================================
extern "C" void movePcTickState(cf::CCtrlMovePC* self) {
    typedef int (cf::CCtrlMovePC::*MoveFn)();
    const MoveFnTable* tbl = (const MoveFnTable*)&lbl_eu_80532B60;   // ptmf constant table
    int flag = 1;
    int blocked = 0;

    if (self->mFlags50 & 0x00010000u) {
        self->mStateFunc = tbl->mEntries[32];
        flag = 0;
    } else {
        u32 f = self->mFlags4C;
        if (f & 0x00001000u) {
            if ((f & 0x00000800u)
                && ((((cf::CfObjWrap*)self->mObject)->mSub->mField4EC & 0x100u) == 0)
                && ((((cf::CfObjWrap*)self->mPlayer)->mSub->mField4EC & 0x100u) != 0)) {
                if (self->mDistFC > 20.0f) {
                    self->mStateFunc = tbl->mEntries[33];
                    flag = 0;
                }
            } else if (self->mDistFC > 1.0f) {
                self->mStateFunc = tbl->mEntries[34];
                flag = 0;
            }
        } else if (f & 0x00000400u) {
            f32 lim;
            if (f & 0x00000200u) {
                // Slow-walk steering: only when close enough and slow enough.
                if (self->mDistFC < 2.0f) {
                    cf::CfMoveSub* sub = ((cf::CfObjWrap*)self->mObject)->mSub;
                    if (sub->mField3F0 * sub->mField3F0 + sub->mField3F8 * sub->mField3F8 < 9.0f) {
                        ml::CVec3 v;
                        ml::CVec3* vp = &v;
                        v.x = sub->mField3F0;
                        v.y = 0.0f;
                        v.z = sub->mField3F8;
                        if (!(v.x == 0.0f && v.y == 0.0f && v.z == 0.0f)) {
                            if (v.y * v.y + v.x * v.x + v.z * v.z == 0.0f) {
                                v = ml::CVec3::zero;
                            } else {
                                PSVECNormalize((const Vec*)vp, (Vec*)vp);
                            }
                            ml::CVec3* pos = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
                            ml::CVec3 tmp = *pos + *vp;
                            ml::CVec3 probe(tmp.x, tmp.y + 1.0f, tmp.z);
                            if (ScnRes_VertRayForward_E398(&probe, 0x44a05, 0, 0, -3.0f, 0.707099974155426f) != 0) {
                                if (ScnRes_EntryFlagThunk_E5A4(0x400, 0) != 0) {
                                    ml::CVec3 scaled = *vp * -1.0f;
                                    *vp = scaled;
                                    moveBaseLerp(self, vp);
                                    self->mVec90 = *vp;
                                    setMoveFacing((cf::CCtrlMoveBase*)self, (const ml::CVec3*)&self->mVec90, 1.0f);
                                    flag = 0;
                                }
                            }
                        }
                    }
                }
                lim = 12.0f;
            } else {
                lim = 11.0f;
            }
            if (self->mDistFC > lim) {
                self->mStateFunc = tbl->mEntries[35];
                flag = 0;
            }
        } else {
            if (f & 0x00010000u) {
                // Limit arithmetic runs in double precision in retail.
                f64 limit = (f64)4.0f * self->mShort122 + (f64)3.0f;
                if (self->mDistFC > limit) {
                    self->mStateFunc = tbl->mEntries[36];
                    flag = 0;
                }
            } else if (self->mDistFC > 4.0f) {
                self->mStateFunc = tbl->mEntries[37];
                flag = 0;
            }
        }
    }

    // Tail: when no transition ran, steer toward the approach direction.
    // rlwinm masks here clear exactly bits 0x20000000 / 0x40.
    if (flag != 0) {
        if (self->mDistFC >= 2.0f) {
            self->mArr124[13] = 0;
            self->mFlags4C &= ~0x20000000u;
        } else if (!(self->mFlags4C & 0x20000000u)) {
            if (self->mDistFC < 1.5f) {
                u32 g = self->mFlags4C | 0x20000000u;
                self->mArr124[13] = 0;
                self->mFlags4C = g;
            }
        } else {
            ml::CVec3* pos = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
            ml::CVec3 delta = *pos - self->mVec78;
            ml::CVec3 deltaArg = delta;
            moveBaseChase(self, &self->mVec90, &deltaArg);

            ml::CVec3 scaled = self->mVec90 * 0.800000011920929f;
            ml::CVec3 scaledCopy = scaled;
            ml::CVec3* pos2 = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
            ml::CVec3 sum = *pos2 + scaledCopy;
            ml::CVec3 probePos(sum.x, sum.y + -4.0f, sum.z);
            ml::CVec3 probeArg = probePos;

            blocked = 0;
            if (ScnRes_VertRayForward_E398(&probeArg, 0x4a05, 0, 0, -4.0f, 0.707099974155426f) != 0) {
                if (ScnRes_EntryFlagThunk_E5A4(0x20000, 0) == 0) {
                    blocked = 1;
                } else if ((((cf::CfObjWrap*)self->mObject)->mSub->mField4EC & 0x100u) == 0) {
                    blocked = 1;
                }
            }
            if (blocked != 0) {
                setMoveFacing((cf::CCtrlMoveBase*)self, (const ml::CVec3*)&self->mVec90, 0.20000000298023224f);
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
        self->mpDataPtr->field_0x14 = 0.0f;
    }
}
// ============================================================================
// movePcProbeGoalAngle - ground-probe angle search around the goal (0x310)
//
// From a point just above the stored goal position, cast short rays outward
// in four directions (starting at the object's facing, stepping pi/2 per
// retry) at two heights (dropping the start point each pass). On the first
// hit, return the surface normal converted to degrees (negated components
// on the first pass). If no ray hits anything, return the facing angle.
// ============================================================================
extern "C" f32 movePcProbeGoalAngle(cf::CCtrlMovePC* self) {
    ml::CVec3 goal(self->mVec54.x, self->mVec54.y + 1.0f, self->mVec54.z);
    ml::CVec3 dir;
    ml::CVec3 n;
    ml::CVec3 start;
    ml::CVec3 end;
    ml::CVec3 diff;
    ml::CVec3 sum;
    for (int i = 0; i < 2; i++) {
        f32 ang = ((cf::CfObjectActor*)self->mObject)->CfObject_getMoveHeadAngle();   // vtable 0x5b4
        for (int j = 0; j < 4; j++) {
            dir.z = 2.0f * CosFIdx__Q24nw4r4mathFf(40.7436637878418f * ang);
            dir.y = 0.0f;
            dir.x = 2.0f * SinFIdx__Q24nw4r4mathFf(40.7436637878418f * ang);
            diff = goal - dir;
            sum = goal + dir;
            start = diff;
            end = sum;
            if (ScnRes_SegQueryForward_E348(&start, &end, 0, 0x2000, 0) != 0) {
                ScnRes_CopyEntryMid(&n, 0);
                if (i == 0) {
                    return 0.02454369328916073f * Atan2FIdx__Q24nw4r4mathFff(-n.x, -n.z);
                }
                return 0.02454369328916073f * Atan2FIdx__Q24nw4r4mathFff(n.x, n.z);
            }
            ang = ang + ml::halfpi;
        }
        goal.y = goal.y - 3.0f;
    }
    return ((cf::CfObjectActor*)self->mObject)->CfObject_getMoveHeadAngle();
}
// ============================================================================
// movePcUpdatePairFollow - pair-follow mode update (0x2A8 bytes)
//
// Runs each tick from movePcUpdateTop. When follow mode (mFlags50 bit 0x4000)
// is active it either cancels it (too far / spot occupied but arrived) or
// re-targets the stored candidate spot. Otherwise it maintains the follow
// request bits from distance thresholds. Finally, if a reposition is pending,
// it walks an angle/ring probe grid around the character (index mArr124[16],
// low 3 bits = direction step, high bits = ring radius), raycasts each
// candidate, and on a valid free ground spot commits it via moveBaseRoute.
// ============================================================================
extern "C" void movePcUpdatePairFollow(cf::CCtrlMovePC* self) {
    u32 f50 = self->mFlags50;

    if (f50 & 0x4000u) {
        if (!(self->mDistFC >= 20.0f)) {
            cf::CfObjWrap* ownW = (cf::CfObjWrap*)self->mObject;
            if (((ownW->mSub->mField4EC & 0x100u) != 0) ||
                ((((cf::CfObjWrap*)self->mPlayer)->mSub->mField4EC & 0x100u) != 0)) {
                // Spot occupied: confirm we actually reached the goal area.
                int arrived = 1;
                if ((ownW->mSub->mField4EC & 0x100u) != 0) {
                    const ml::CVec3& d = *((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector()
                                         - self->mVecF0;
                    f32 d2 = d.x * d.x + d.y * d.y + d.z * d.z;
                    if (!(d2 > 0.36000001430511475f)) {
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
        if (self->mDistFC < 5.0f) {
            self->mFlags50 = t & 0x1FFFu;
        }
    } else {
        if (!(self->mDistFC >= 8.0f)) {
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
        if (!(d2 > 400.0f)) {
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
    const f32 heightStep = 5.0f;
    const f32 ampScale = ml::quarterpi;
    const f32 baseRadius = 1.0f;
    const f32 degScale = 40.7436637878418f;
    const f32 maxReach = 19.0f;
    for (int i = 0; i < 4; i++) {
        s16 h = self->mArr124[16];
        if (h >= 0xF0) {
            self->mArr124[16] = 0;
            continue;
        }
        ml::CVec3* pos = ((cf::CfObject*)((char*)self->mObject + 0x3e9c))->CfObject_getPosVector();
        probe.x = pos->x;
        probe.z = pos->z;
        probe.y = pos->y + heightStep;
        // direction index (low bits) and ring index (high bits), both
        // converted to f32 before scaling
        f32 amp = (f32)(h & 7) * ampScale;
        f32 rad = baseRadius + (f32)(h >> 4);
        f32 s = SinFIdx__Q24nw4r4mathFf(degScale * amp);
        probe.x += rad * s;
        f32 c = CosFIdx__Q24nw4r4mathFf(degScale * amp);
        probe.z += rad * c;

        if (ScnRes_VertRayForward_E398(&probe, 0x4a05, 0, 0, -10.0f, 0.707099974155426f) == 0) {
            self->mArr124[16] = h + 1;
            continue;
        }
        if (ScnRes_EntryFlagThunk_E5A4(0x20000, 0) != 0) {
            self->mArr124[16] = h + 1;
            continue;
        }
        ScnRes_CopyEntryHead(&out, 0);
        const ml::CVec3& d = self->mPos - out;
        f32 d2 = d.x * d.x + d.y * d.y + d.z * d.z;
        if (d2 > maxReach * maxReach) {
            self->mArr124[16] = h + 1;
            continue;
        }
        if (moveBaseRoute(self, &out, 0) == 0) {
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