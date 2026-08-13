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
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"
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
void  func_8047DE14__17UnkClass_8047D2ACFv(void* a, Vec* b, f32 c, f32 d);
void  func_8047DD4C__17UnkClass_8047D2ACFv(void* a, Vec* b, void* c, f32 d, f32 e, int f);
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
extern const f32 lbl_eu_80667B70;
extern const f32 lbl_eu_80667B90;
extern const f32 lbl_eu_80667BA8;
extern const f32 lbl_eu_80667BC0;
extern const f32 lbl_eu_80667BC4;
extern const f32 lbl_eu_80667BD0;
extern const f32 lbl_eu_80667BDC;
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
extern const f32 lbl_eu_80667BF0;
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

    u8 battleState = *(u8*)((char*)getInstance__Q22cf14CBattleManagerFv() + 0x1aa);
    u32 inBattle = 0;
    if (battleState < 1 || battleState > 0x18) {
        /* not in battle */
    } else {
        inBattle = 1;
    }
    if (inBattle != 0) {
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

extern "C" void func_8019A9C4(cf::CCtrlMovePC* self) {}
extern "C" int func_8019B4F0(cf::CCtrlMovePC* self) { return 0; }
extern "C" int func_8019C0D4(cf::CCtrlMovePC* self) { return 0; }
extern "C" void func_8019C304(cf::CCtrlMovePC* self) {}
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
        zeroArr(self);
        self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x30);
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
    ml::CVec3 d = self->mPos - *op;
    self->mVec84 = d;
    self->mDistFC = PSVECMag((const Vec*)&self->mVec84);
    f32 mag = (f32)(double)self->mDistFC;
    if (mag != lbl_eu_80667B60) {
        ml::CVec3 v = d * (lbl_eu_80667B68 / mag);
        self->mVec78 = v;
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
    if (func_80174C98(obj, &x, 4) != 0) {
        self->mFlags4C |= 0x40000u;
    }
    cf::CfObjWrap* pObj = (cf::CfObjWrap*)self->mPlayer;
    void* sub2 = pObj->mField04;
    void* r2 = ((cf::CMoveEmbedded*)sub2)->getCtrl();
    x = *(u32*)r2;
    if (func_80174C98(pObj, &x, 4) != 0) {
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

    ml::CVec3* op2 = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
    ml::CVec3 d2 = self->mVecA8 - *op2;
    if (d2.magnitude() <= lbl_eu_80667B68) {   // 1.0
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

    ml::CVec3 d3 = self->mVecB4 - self->mPos;
    if (d3.magnitude() <= lbl_eu_80667B68) {   // 1.0
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
                    self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x3c);
                    self->mArr124[7] = 0;
                }
            } else if (self->mFlags4C & 0x100u || self->mDistFC >= lbl_eu_80667B80) {
                if (self->mArr124[17] <= 0) {
                    self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                    self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x48);
                    self->mArr124[7] = 0;
                }
            } else if (self->mFlags50 & 0x100u) {
                if (self->mDistFC >= lbl_eu_80667B84) {   // 10
                    if (self->mArr124[17] <= 0) {
                        self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                        self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x54);
                        self->mArr124[7] = 0;
                    }
                } else {
                    ml::CVec3 d4 = self->mPos - self->mVec6C;
                    if (d4.magnitude() < lbl_eu_80667B88) {   // 16
                        if (self->mArr124[17] <= 0) {
                            self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                            self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x60);
                            self->mArr124[7] = 0;
                        }
                    } else if ((self->mFlags4C & 0x40000u) && self->mDistFC < lbl_eu_80667B8C) {  // 40
                        if (self->mArr124[17] <= 0) {
                            self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                            self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x6c);
                            self->mArr124[7] = 0;
                        }
                    } else if (self->mDistFC >= ((self->mFlags4C & 0x80000u) ? lbl_eu_80667B90 : lbl_eu_80667B94)) {
                        s16 v = self->mArr124[9];
                        self->mArr124[9] = (s16)(v + 1);
                        if (v + 1 >= 1200) {
                            self->mArr124[9] = 0;
                            if (self->mArr124[17] <= 0) {
                                self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                                self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x78);
                                self->mArr124[7] = 0;
                            }
                        } else {
                            ml::CVec3* op4 = ((cf::CMoveObj*)((char*)self->mObject + 0x3e9c))->getPosition();
                            ml::CVec3 d5 = self->mVecD8 - *op4;
                            if (d5.magnitude() <= lbl_eu_80667B98) {   // 225
                                s16 v2 = self->mArr124[10];
                                self->mArr124[10] = (s16)(v2 + 1);
                                if (v2 + 1 >= 300) {
                                    self->mArr124[10] = 0;
                                    if (self->mArr124[17] <= 0) {
                                        self->mFlags4C = (self->mFlags4C & ~0x100u) | 0x8u;
                                        self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x84);
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
        own->mField0C &= ~0x1000u;
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
        if (func_80174C98(obj2, &y, 8) != 0) {
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

    ml::CVec3 goal;
    if (self->mFlags4C & 0x1000u) {
        goal = self->mVecCC;
    } else {
        goal = self->mPos;
    }

    ml::CVec3* pos = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
    ml::CVec3 delta = *pos - goal;

    ml::CVec3* pos2 = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
    ml::CVec3 delta2 = *pos2 - self->mPos;

    f32 dist = delta2.magnitude();

    // Pick the party member (1 or 2, never ourselves) whose position is at
    // most as far from our current spot as we are; ties prefer member 2.
    cf::CfObjectMove* best = 0;
    for (int i = 1; i < 3; i++) {
        cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
        if (p == 0) continue;
        if (self->mShort122 == i) continue;
        ml::CVec3* pp = ((cf::CMovePosIntf*)p)->getPosition();
        ml::CVec3 d = *pp - self->mPos;
        f32 pd = d.magnitude();
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
        ml::CVec3 d = *op - *bp;
        f32 bd = d.magnitude();
        if (dist > bd) {
            dist = bd;
            delta2 = d;
        }
    }

    f32 f30 = lbl_eu_80667B68;    // 1.0
    f32 f29 = lbl_eu_80667C14;    // 0.333
    int act = 0;
    if (self->mFlags50 & 0x4u) {
        if (self->mFlags50 & 0x20u) {
            if (dist >= lbl_eu_80667C04) {      // 9.0: already close enough
                if (!(self->mFlags4C & 0x1000u) && best != 0) {
                    delta = delta2;
                }
                act = 1;
            } else {
                self->mFlags50 &= ~0x24u;
            }
        } else if (dist <= lbl_eu_80667C04) {
            // Overshot: aim the opposite way from the goal.
            delta = delta2 * lbl_eu_80667BD0;   // * -1.0
            f30 = lbl_eu_80667C18;              // 1.2
            f29 = lbl_eu_80667B7C;              // 0.6
            act = 1;
        } else {
            self->mFlags50 &= ~0x4u;
        }
    } else if (dist >= lbl_eu_80667C1C) {       // 10.889999
        if (!(self->mFlags4C & 0x1000u) && best != 0) {
            delta = delta2;
        }
        self->mFlags50 |= 0x24u;
        act = 1;
    } else if (!(self->mFlags4C & 0x100000u)) {
        if (dist <= lbl_eu_80667C20) {          // 5.29
            self->mFlags50 |= 0x4u;
            delta = delta2 * lbl_eu_80667BD0;
            f30 = lbl_eu_80667C18;
            f29 = lbl_eu_80667B7C;
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
        CMoveWrapper* w = (CMoveWrapper*)self->mObject;
        f32 ang = w->getAngle();
        f32 cosA = CosFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * ang);
        f32 ang2 = ((CMoveWrapper*)self->mObject)->getAngle();
        f32 sinA = SinFIdx__Q24nw4r4mathFf(lbl_eu_80667B9C * ang2);

        ml::CVec3 facing(sinA, lbl_eu_80667B60, cosA);
        ml::CVec3 up(lbl_eu_80667B60, lbl_eu_80667B68, lbl_eu_80667B60);
        ml::CVec3 right;
        PSVECCrossProduct(facing, up, right);
        right.normalizeSub();

        ml::CVec3 steer = delta;
        f32 dist2 = delta.x * delta.x + delta.z * delta.z;
        if (lbl_eu_80667B74 >= dist2) {          // 0.25: stand still
            steer.z = lbl_eu_80667B60;
            steer.x = lbl_eu_80667B60;
            self->mFlags50 &= ~0x80u;
        } else if (self->mFlags50 & 0x80u) {
            steer = self->mVecE4;
            s16 c = self->mArr124[8];
            self->mArr124[8] = (s16)(c - 1);
            if ((s16)(c - 1) <= 0) {
                self->mFlags50 &= ~0x80u;
                self->mArr124[8] = 0;
            }
        } else {
            f32 f28;
            if (self->mFlags50 & 0x10u) {
                f28 = lbl_eu_80667B68;
            } else {
                f28 = f29;
                f29 = lbl_eu_80667B68;
            }
            if (dist2 < lbl_eu_80667B60) {
                Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
            }
            f32 inv;
            if (dist2 > lbl_eu_80667B60) {
                inv = dist2 * FrSqrt__Q24nw4r4mathFf(dist2);
            } else {
                inv = lbl_eu_80667B60;
            }
            if (inv * f28 < f29 * ml::math::abs(delta.y)) {
                // Slope case: climb straight toward the goal.
                int r27 = 1;
                steer.z = lbl_eu_80667B60;
                steer.x = lbl_eu_80667B60;
                ml::CVec3 v(lbl_eu_80667B60, steer.y, lbl_eu_80667B60);
                v.normalizeSub();
                ml::CVec3 scaled = v * lbl_eu_80667C08;   // 0.3
                ml::CVec3* pos3 = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                ml::CVec3 dv = *pos3 - scaled;
                ml::CVec3 diff = dv - facing;
                ml::CVec3 sum = dv + facing;
                if (func_804BE348(&diff, &sum, 0, 0x2000, 0) == 0) {
                    if (delta.x != lbl_eu_80667B60 || delta.z != lbl_eu_80667B60) {
                        r27 = 0;
                        steer = ml::CVec3(delta.x, lbl_eu_80667B60, delta.z);
                    }
                }
                self->mFlags50 &= ~0x10u;
                if (r27 == 0) {
                    self->mVecE4 = steer;
                    self->mFlags50 |= 0x80u;
                    self->mArr124[8] = 60;
                }
            } else {
                // Flat case: run around the goal keeping its slope.
                int r27 = 1;
                steer.y = lbl_eu_80667B60;
                ml::CVec3 v(steer.x, lbl_eu_80667B60, steer.z);
                v.normalizeSub();
                ml::CVec3 scaled = v * lbl_eu_80667C08;   // 0.3
                ml::CVec3* pos3 = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                ml::CVec3 dv = *pos3 - scaled;
                ml::CVec3 diff = dv - facing;
                ml::CVec3 sum = dv + facing;
                if (func_804BE348(&diff, &sum, 0, 0x2000, 0) == 0) {
                    if (delta.y != lbl_eu_80667B60) {
                        r27 = 0;
                        steer = ml::CVec3(lbl_eu_80667B60, delta.y, lbl_eu_80667B60);
                    }
                }
                self->mFlags50 |= 0x10u;
                if (r27 == 0) {
                    self->mVecE4 = steer;
                    self->mFlags50 |= 0x80u;
                    self->mArr124[8] = 60;
                }
            }
        }

        // --- commit the steer direction ---
        steer.normalizeSub();
        self->mVec90.y = lbl_eu_80667B60;
        self->mVec90.x = facing.x * (-steer.y);
        self->mVec90.z = facing.z * (-steer.y);
        f32 dist3 = steer.x * steer.x + steer.z * steer.z;
        if (dist3 < lbl_eu_80667B60) {
            Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
        }
        f32 inv2;
        if (dist3 > lbl_eu_80667B60) {
            inv2 = dist3 * FrSqrt__Q24nw4r4mathFf(dist3);
        } else {
            inv2 = lbl_eu_80667B60;
        }
        f32 dot = steer.x * right.x + steer.z * right.z;
        if (dot < lbl_eu_80667B60) {
            inv2 = -inv2;
        }
        self->mVec90.x = right.x * inv2 + self->mVec90.x;
        self->mVec90.z = right.z * inv2 + self->mVec90.z;
        func_80089694(self, (const Vec*)&self->mVec90, f30);
        return 0;
    }

    self->mArr124[15] = 0;
    self->mFlags4C = (self->mFlags4C | 0x01000000u) & 0x78000000u;
    self->mArr124[2] = 0;
    self->mArr124[1] = 0;
    self->mArr124[3] = 0;
    ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;
    return 0;
}
extern "C" void func_8019D9E0(cf::CCtrlMovePC* self) {}
// ============================================================================
// func_8019DD54 - reposition state update: pick a spot beside the party
// leader (or in front), steer the candidate toward it, and commit the move
// (0x9BC bytes).
// ============================================================================
extern "C" void func_8019DD54(cf::CCtrlMovePC* self) {
    self->mFlags4C |= 0x8;
    ((cf::CfMoveData*)self->mBaseData)->mField14 = lbl_eu_80667B60;

    cf::CfMoveSub* sub = ((cf::CfObjWrap*)self->mObject)->mSub;
    if (sub != 0) {
        u16 fl = sub->mField530;
        if ((fl & 0xC) != 0 || (fl & 0x30) != 0) {
            return;
        }
    }

    int r31 = 0;
    f32 f30 = 0.0f;
    CfPartyInfoOut goal;   // result block at 0x128 (goal x/y/z + flags)
    if (self->mFlags50 & 1) {
        u32 f4C = self->mFlags4C;
        ml::CVec3 pos = self->mPos;
        f32 f29, f31;
        if (!(f4C & 0x4000)) {
            self->mFlags4C |= 0x20000;
            if (self->mShort120 == 1) {
                f29 = lbl_eu_8066A204;
            } else {
                f29 = -lbl_eu_8066A204;
            }
            f31 = lbl_eu_80667C28;
        } else {
            f29 = (f32)ml::math::mtRand(0x168) * lbl_eu_8066A210;
            f32 rr = (f32)ml::math::mtRand(0xa);
            f31 = lbl_eu_80667BDC * (lbl_eu_80667B6C * rr) + lbl_eu_80667C2C;
        }

        // facing rotation: up-vector -> world, then scale and offset
        f32 a1 = lbl_eu_80667B9C * self->mFloat100;
        f32 s1 = SinFIdx__Q24nw4r4mathFf(a1);
        f32 c1 = CosFIdx__Q24nw4r4mathFf(a1);
        Mtx mY = {
            {c1, lbl_eu_80667B60, s1, lbl_eu_80667B60},
            {lbl_eu_80667B60, lbl_eu_80667B68, lbl_eu_80667B60, lbl_eu_80667B60},
            {-s1, lbl_eu_80667B60, c1, lbl_eu_80667B60},
        };
        f32 a2 = lbl_eu_80667B9C * f29;
        f32 s2 = SinFIdx__Q24nw4r4mathFf(a2);
        f32 c2 = CosFIdx__Q24nw4r4mathFf(a2);
        Mtx mZ = {
            {c2, -s2, lbl_eu_80667B60, lbl_eu_80667B60},
            {s2, c2, lbl_eu_80667B60, lbl_eu_80667B60},
            {lbl_eu_80667B60, lbl_eu_80667B60, lbl_eu_80667B68, lbl_eu_80667B60},
        };
        PSMTXConcat(mZ, mY, mY);

        ml::CVec3 v(lbl_eu_80667B60, lbl_eu_80667BD0, lbl_eu_80667B60);
        nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&v,
            (const nw4r::math::MTX34*)mY, (const nw4r::math::VEC3*)&v);
        v = v * f31;
        ml::CVec3 target = pos + v;

        f32 a3 = lbl_eu_80667B9C * self->mFloat100;
        f32 c3 = CosFIdx__Q24nw4r4mathFf(a3);
        f32 s3 = SinFIdx__Q24nw4r4mathFf(a3);
        ml::CVec3 facing(s3, lbl_eu_80667B60, c3);
        ml::CVec3 diff = target - facing;
        ml::CVec3 sum = target + facing;
        if (func_804BE348(&diff, &sum, 0, 0x2000, 0) == 0) {
            return;
        }

        int r29 = 1;
        for (int r28 = 1; r28 < 3; r28++) {
            void* p = getPlayer__Q22cf13CfGameManagerFi(r28);
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
                r29 = 0;
            }
        }
        if (r29 == 0) {
            return;
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
        u32 f4C = self->mFlags4C;
        if (!(f4C & 0x4000)) {
            self->mFlags4C |= 0x20000;
            ml::CVec3 v0 = self->mPos;
            CfPartyInfoIn p1;
            func_80198710(&p1, &v0, 6, self->mFloat100, lbl_eu_80667B60,
                          lbl_eu_80667B70, self->mShort120);
            self->mArr124[9] = 0;
            p1.mField2E = 1;
            if (func_8019876C(&p1, &goal) == 0) {
                return;
            }
            if (goal.mField9C != 0) {
                f30 = goal.mField94;
                r31 = 1;
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
                ml::CVec3 v2 = self->mPos;
                v2.y += lbl_eu_80667B70;
                ml::CVec3 v3 = self->mPos;
                v3.y += lbl_eu_80667BBC;
                if (func_804B526C(lbl_eu_80665958,
                    (char*)self->mPlayer + 0x44a8, &v2, &v3, 2, 1, 0) == 0) {
                    return;
                }
                ml::CVec3* ppos = (ml::CVec3*)lbl_eu_80665958;
                goal.y = ppos->y + lbl_eu_80667BC0;
            } else if (goal.mField6C != 0) {
                f30 = goal.mField64;
                r31 = 1;
            }
        }
    }

    ml::CVec3 vA = self->mPos;
    vA.y += lbl_eu_80667BC4;
    ml::CVec3 vB(goal.x, goal.y, goal.z);
    vB.y += lbl_eu_80667BC4;
    if (func_804BE348(&vA, &vB, 0x44a05, 0, 0) != 0) {
        return;
    }
    self->mStateFunc = lbl_eu_80532CD4;

    cf::CfObjWrap* obj = (cf::CfObjWrap*)self->mObject;
    obj->mField455A = 0x64;
    if (r31 != 0) {
        if (goal.y - f30 >= lbl_eu_80667C28) {
            goal.y -= lbl_eu_80667C28;
        } else {
            goal.y = f30;
        }
    }
    self->mArr124[9] = 0;
    self->mArr124[10] = 0;
    self->mArr124[11] = 0;
    self->mArr124[1] = 0;
    self->mArr124[0] = 0;
    self->mArr124[17] = 30;
    self->mVecA8.x = goal.x;
    self->mVecA8.y = goal.y;
    self->mVecA8.z = goal.z;
    self->mVecD8.x = goal.x;
    self->mVecD8.y = goal.y;
    self->mVecD8.z = goal.z;

    cf::CMovePosIntf* emb = (cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c);
    emb->getHandle();
    emb->setAngle(self->mFloat100);
    emb->setState(lbl_eu_80667B68);
    func_800BC3B0((char*)self->mObject + 0x3e9c, lbl_eu_80667B90);

    cf::CfMoveSub* sub2 = ((cf::CfObjWrap*)self->mObject)->mSub;
    if (sub2 != 0) {
        func_8004B7C0(sub2, &ml::CVec3::zero);
        ml::CVec3 z = ml::CVec3::zero;
        sub2->mField3F0 = z.x;
        sub2->mField3F4 = z.y;
        sub2->mField3F8 = z.z;
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

    if (self->mFlags50 & 0x00010000u) {
        self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x180);
        flag = 0;
    } else {
        u32 f = self->mFlags4C;
        if (f & 0x00001000u) {
            cf::CfObjWrap* obj = (cf::CfObjWrap*)self->mObject;
            cf::CfObjWrap* player = (cf::CfObjWrap*)self->mPlayer;
            if ((f & 0x00000800u) && !(obj->mSub->mField4EC & 0x100u)
                && (player->mSub->mField4EC & 0x100u)) {
                if (self->mDistFC > lbl_eu_80667B90) {
                    self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x18c);
                    flag = 0;
                }
                goto tail;
            }
            if (self->mDistFC > lbl_eu_80667B68) {
                self->mStateFunc = *(const MoveFn*)((const char*)tbl + 0x198);
                flag = 0;
            }
        } else if (f & 0x00000400u) {
            f32 lim;
            if (f & 0x00000200u) {
                if (self->mDistFC >= lbl_eu_80667B70) {
                    cf::CfMoveSub* sub = ((cf::CfObjWrap*)self->mObject)->mSub;
                    if (sub->mField3F8 * sub->mField3F8 + sub->mField3F0 * sub->mField3F0 >= lbl_eu_80667C04) {
                        ml::CVec3 v;
                        ml::CVec3* vp = &v;
                        v.x = sub->mField3F0;
                        v.y = lbl_eu_80667B60;
                        v.z = sub->mField3F8;
                        if (!(v.x == lbl_eu_80667B60 && v.y == lbl_eu_80667B60 && v.z == lbl_eu_80667B60)) {
                            if (v.x * v.x + v.y * v.y + v.z * v.z == lbl_eu_80667B60) {
                                v = ml::CVec3::zero;
                            } else {
                                PSVECNormalize((const Vec*)vp, (Vec*)vp);
                            }
                            ml::CVec3* pos = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
                            ml::CVec3 goal = *pos + *vp;
                            goal.y += lbl_eu_80667B68;
                            if (func_804BE398(&goal, 0x44a05, 0, 0, lbl_eu_80667C34, lbl_eu_8066AF20) != 0) {
                                if (func_804BE5A4(0x400, 0) != 0) {
                                    v = v * lbl_eu_80667BD0;
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
                f32 limit = lbl_eu_80667BA8 * (f32)self->mShort122 + lbl_eu_80667BDC;
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

tail:
    if (flag == 0) {
        self->mArr124[13] = 0;
        self->mFlags4C &= ~0x4u;
    } else if (self->mDistFC >= lbl_eu_80667B70) {
        self->mArr124[13] = 0;
        self->mFlags4C &= ~0x4u;
    } else if (self->mFlags4C & 0x20000000u) {
        ml::CVec3* pos = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        ml::CVec3 delta = *pos - self->mVec78;
        ml::CVec3* dp = &delta;
        func_800896F4(self, &self->mVec90, dp);

        ml::CVec3 scaled = self->mVec90 * lbl_eu_80667BC4;
        ml::CVec3* pos2 = ((cf::CMovePosIntf*)((char*)self->mObject + 0x3e9c))->getPosition();
        delta = *pos2 + scaled;
        delta.y += lbl_eu_80667B70;

        int r27 = 0;
        if (func_804BE398(dp, 0x4a05, 0, 0, lbl_eu_80667BE4, lbl_eu_8066AF20) != 0) {
            if (func_804BE5A4(2, 0) == 0) {
                r27 = 1;
            } else if ((((cf::CfObjWrap*)self->mObject)->mSub->mField4EC & 0x100u) != 0) {
                r27 = 1;
            }
        }
        if (r27 != 0) {
            func_80089694(self, (const Vec*)&self->mVec90, lbl_eu_80667BC0);
            self->mArr124[13] = 0;
            self->mFlags4C &= 0xE0u;
            flag = 0;
        } else {
            s16 c = self->mArr124[13];
            self->mArr124[13] = (s16)(c + 1);
            if (c < 60) {
                self->mFlags4C |= 0x40u;
            } else {
                self->mArr124[13] = 60;
                self->mFlags4C &= 0xE0u;
            }
        }
    } else if (self->mDistFC < lbl_eu_80667C10) {
        self->mFlags4C |= 0x20000000u;
        self->mArr124[13] = 0;
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
extern "C" void func_8019EF90(cf::CCtrlMovePC* self) {}
extern "C" void func_8019F1E0(cf::CCtrlMovePC* self) {}