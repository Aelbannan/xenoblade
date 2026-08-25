// Translation unit for kyoshin/cf/CCtrlMoveBase
// This file implements the CtrlMoveBase class - base for move-control objects.
// Two virtual stubs are FULL_MATCH (100% instruction match, semantically equivalent).
//
// The base move controller works against a "move data" block (mpDataPtr): its
// object at +0x28 exposes the entity position through vtable slot 0xAC. The
// per-object runtime instance at +0x30 is lazily created through the game
// manager's sub-object at +0x2F2C.

// CfGameManager.hpp declares func_800D9354(CBattleManagerView*) while
// CBattleManagerApi.hpp (pulled in by that same header) declares
// func_800D9354(cf::CBattleManager*); MWCC rejects the mismatched extern "C"
// pair (10197). This TU never calls it, so rename the declaration out of the
// way for the duration of the includes (same shield as CBattleManager.cpp).
#define func_800D9354 cfgGameMgr9354Unused
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#undef func_800D9354
#include "monolib/math/Random.hpp"
#include <nw4r/math.h>

using cf::CCtrlMoveBase;

// Position source: data->mPosObj->getPosition() (vtable slot 0xAC).
static inline ml::CVec3* getPos(CCtrlMoveBase* self) {
    cf::CCtrlMoveData* data = (cf::CCtrlMoveData*)self->mpDataPtr;
    return data->mPosObj->getPosition();
}

// Normalize a vector; a zero-length vector is replaced by ml::CVec3::zero.
// `zero` is the caller's pre-loaded copy of lbl_eu_806665A0 used for the
// component checks; the length test re-reads the global.
static inline void normalizeOrZeroZ(ml::CVec3* v, f32 zero) {
    if (v->x == zero && v->y == zero && v->z == zero) {
        return;
    }
    if (v->x * v->x + v->y * v->y + v->z * v->z == lbl_eu_806665A0) {
        *v = ml::CVec3::zero;
    } else {
        PSVECNormalize(*v, *v);
    }
}

static inline void normalizeOrZero(ml::CVec3* v) {
    normalizeOrZeroZ(v, lbl_eu_806665A0);
}

// Move the horizontal velocity toward a target direction with the +0x3C
// lerp factor, then export it back to the direction vector (func_800898D4
// shape: y is left untouched).
static inline void lerpVel(CCtrlMoveBase* self, ml::CVec3* dir) {
    self->mVelocity.x = self->mFloatParam2 * (dir->x - self->mVelocity.x) + self->mVelocity.x;
    self->mVelocity.z = self->mFloatParam2 * (dir->z - self->mVelocity.z) + self->mVelocity.z;
    dir->x = self->mVelocity.x;
    dir->y = self->mVelocity.y;
    dir->z = self->mVelocity.z;
}

// Constructor: stores the interface vtable label at +0x48 (class is built
// without a compiler-generated vtable), zeroes state, copies ml::CVec3::zero
// into position/velocity, and sets the default move parameters.
CCtrlMoveBase::CCtrlMoveBase(void* dataPtr) {
    mSecondaryVtable = (cf::CCtrlMoveBaseIface*)lbl_eu_80527808;
    // Interleaving matters: retail loads param1 before the mpSomePtr store
    // and param2 after it.
    f32 param2;
    f32 param1;
    param1 = lbl_eu_80666598;
    mpSomePtr = 0;
    param2 = lbl_eu_8066659C;
    mpDataPtr = dataPtr;
    mFlagsU16_1 = 0;
    mPosition = ml::CVec3::zero;
    mVelocity = ml::CVec3::zero;
    mFlagsU16_2 = 0;
    mByte1 = 0;
    mByte2 = 0;
    mFloatParam1 = param1;
    mFloatParam2 = param2;
}

// ============================================================================
// func_80088974 - main per-frame move update (0x734 bytes)
// ============================================================================
int func_80088974(CCtrlMoveBase* self, ml::CVec3* out, const ml::CVec3* src,
                  int flagA, int flagB) {
    self->mFlagsU16_1 &= ~0x2;
    if (self->mpSomePtr == 0) {
        UnkClass_80083298* gm = cf::CfGameManager::func_80083298();
        if (gm != 0) {
            cf::CCtrlMoveMgr2F2C* p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2f2c);
            if (p != 0) {
                if (p->field_0x00 == 0) {
                    ml::CVec3* pos = getPos(self);
                    *out = *src - *pos;
                    normalizeOrZero(out);
                    self->mVelocity = *out;
                    return 1;
                }
                self->mpSomePtr = func_8047CE7C__17UnkClass_8047CD0CFv();
                self->mVelocity = ml::CVec3::zero;
            }
        }
        if (self->mpSomePtr == 0) {
            *out = ml::CVec3::zero;
            return 0;
        }
    }

    int result = 0;
    if (self->mpSomePtr != 0) {
        if (self->mFlagsU16_2 != 0) {
            self->mFlagsU16_2--;
            *out = self->mVelocity;
            return 1;
        }

        ml::CVec3* pos = getPos(self);
        int r = func_8047D2AC__17UnkClass_8047D2ACFv(
            self->mpSomePtr, out, pos, lbl_eu_806665A0, self->mFloatParam1, src);
        if (r == 0) {
            result = 1;
            if (flagA != 0) {
                self->mVelocity.x = self->mFloatParam2 * (out->x - self->mVelocity.x) + self->mVelocity.x;
                self->mVelocity.z = self->mFloatParam2 * (out->z - self->mVelocity.z) + self->mVelocity.z;
                if (flagB != 0) {
                    ml::CVec3 dir;
                    dir.x = self->mVelocity.x;
                    dir.y = self->mVelocity.y;
                    dir.z = self->mVelocity.z;
                    normalizeOrZero(&dir);
                    ml::CVec3* pz = getPos(self);
                    f32 vz = dir.z + pz->z;
                    ml::CVec3* py = getPos(self);
                    f32 vy = lbl_eu_806665A4 + py->y;
                    ml::CVec3* px = getPos(self);
                    f32 vx = dir.x + px->x;
                    ml::CVec3 v50;
                    v50.y = vy;
                    v50.z = vz;
                    v50.x = vx;
                    if (func_804BE398(&v50, 0x4a05, 0, 0, lbl_eu_806665A8, lbl_eu_806665A0) == 0) {
                        self->mFlagsU16_1 |= 0x2;
                        self->mVelocity = *out;
                        return 1;
                    }
                    ml::CVec3 v44;
                    func_804BE4E0(&v44, 0);
                    if (v44.y <= lbl_eu_8066AF20) {
                        ml::CVec3 v38;
                        func_804BE4B4(&v38, 0);
                        ml::CVec3* p2 = getPos(self);
                        if (v38.y < p2->y) {
                            self->mFlagsU16_1 |= 0x2;
                            self->mVelocity = *out;
                            return 1;
                        }
                    }
                    *out = dir;
                } else {
                    *out = self->mVelocity;
                    normalizeOrZero(out);
                }
            } else {
                self->mVelocity = *out;
            }
        } else if (r == 1) {
            self->mVelocity = *out;
            self->mFlagsU16_2 = 30;
            result = 1;
        } else {
            ml::CVec3* pos2 = getPos(self);
            *out = *src - *pos2;
            normalizeOrZero(out);
            if (flagA != 0) {
                lerpVel(self, out);
                normalizeOrZero(out);
            } else {
                self->mVelocity = *out;
            }
        }
    } else {
        *out = ml::CVec3::zero;
    }
    return result;
}

// ============================================================================
// func_800890A8 - move-toward-target with instance setup + fallback (0x2F0)
// ============================================================================
int func_800890A8(CCtrlMoveBase* self, ml::CVec3* out, u8* outFlag,
                  const ml::CVec3* src, int flag) {
    int ok;
    UnkClass_80083298* gm;
    if (self->mpSomePtr == 0
            && (gm = cf::CfGameManager::func_80083298()) != 0
            && (char*)gm + 0x2f2c != 0) {
        if (((cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2f2c))->field_0x00 == 0) {
            ok = 0;
        } else {
            void* inst = func_8047CE7C__17UnkClass_8047CD0CFv();
            self->mpSomePtr = inst;
            ok = (inst != 0);
        }
    } else {
        ok = 1;
    }
    if (ok != 0) {
        ml::CVec3* pos = getPos(self);
        if (func_8047DD4C__17UnkClass_8047D2ACFv(self->mpSomePtr, pos, src,
                                                 lbl_eu_806665A0,
                                                 lbl_eu_806665A0, 1) != 0) {
            *outFlag = 1;
            ml::CVec3* pos2 = getPos(self);
            *out = *src - *pos2;
            out->y = lbl_eu_806665A0;
            if (out->x == lbl_eu_806665A0 && out->z == lbl_eu_806665A0) {
                out->x = lbl_eu_806665A0;
                out->y = lbl_eu_806665A0;
                out->z = lbl_eu_806665A4;
                self->mVelocity = *out;
            } else {
                normalizeOrZero(out);
            }
            if (flag != 0) {
                self->mVelocity.y = lbl_eu_806665A0;
                self->mVelocity.x = self->mFloatParam2 * (out->x - self->mVelocity.x) + self->mVelocity.x;
                self->mVelocity.z = self->mFloatParam2 * (out->z - self->mVelocity.z) + self->mVelocity.z;
                // When the lerped horizontal velocity is exactly zero the
                // retail leaves *out as the normalized direction.
                if (!(self->mVelocity.x == lbl_eu_806665A0
                      && self->mVelocity.z == lbl_eu_806665A0)) {
                    normalizeOrZero(&self->mVelocity);
                    *out = self->mVelocity;
                }
            } else {
                self->mVelocity = *out;
            }
            return 1;
        }
    }
    *outFlag = 0;
    return func_80088974(self, out, src, flag, 0);
}

// ============================================================================
// func_80089398 - horizontal direction toward a source point (0x210)
// ============================================================================
void func_80089398(CCtrlMoveBase* self, ml::CVec3* dst, const ml::CVec3* src,
                   int flag) {
    cf::CCtrlMoveData* data = (cf::CCtrlMoveData*)self->mpDataPtr;
    ml::CVec3* pos = data->mPosObj->getPosition();
    *dst = *src - *pos;
    if (!(dst->x == 0.0f && dst->y == 0.0f && dst->z == 0.0f)) {
        if (dst->x * dst->x + dst->y * dst->y + dst->z * dst->z == 0.0f) {
            *dst = ml::CVec3::zero;
        } else {
            PSVECNormalize(*dst, *dst);
        }
    }
    if (flag != 0) {
        self->mVelocity.x = self->mFloatParam2 * (dst->x - self->mVelocity.x) + self->mVelocity.x;
        self->mVelocity.z = self->mFloatParam2 * (dst->z - self->mVelocity.z) + self->mVelocity.z;
        dst->x = self->mVelocity.x;
        dst->y = self->mVelocity.y;
        dst->z = self->mVelocity.z;
        if (!(dst->x == 0.0f && dst->y == 0.0f && dst->z == 0.0f)) {
            if (dst->x * dst->x + dst->y * dst->y + dst->z * dst->z == 0.0f) {
                *dst = ml::CVec3::zero;
            } else {
                PSVECNormalize(*dst, *dst);
            }
        }
    } else {
        self->mVelocity = *dst;
    }
}

// Destination for the lazy position sync.
// The wrapper spans offset 0x48, so MWCC cannot prove the copy stores are
// disjoint from the secondary-vtable slot and keeps the iface load below
// them (retail scheduling).
struct SyncBlob {
    ml::CVec3 head;
    u8 tail[0x48];
};

static inline ml::CVec3* getSyncDst(CCtrlMoveBase* self) {
    return &((SyncBlob*)self)->head;
}

// Lazily sync velocity from the entity position (once, guarded by flag bit 0),
// then notify through the +0x48 interface.
// (volatile member keeps the dispatch load below the copy stores)
void func_800895A8(CCtrlMoveBase* self) {
    u16 flags = self->mFlagsU16_1;
    if ((flags & 1) == 0) {
        self->mFlagsU16_1 = flags | 1;
        self->field_0x00 = *getPos(self);
        ((cf::CCtrlMoveBaseIface*)self->mSecondaryVtable)->unk08(self);
    }
}

// (lazy-init logic is duplicated inline in func_80089F68 / func_8008A01C)

// FULL_MATCH: no-op virtual stub (CCtrlMoveBase::func_80089628)
void func_80089628() {}

// Dispatch the pending +0x30 slot through the manager's +0x2F2C sub-object,
// then clear it.
extern "C" void* func_80083298__Q22cf13CfGameManagerFv(void);
extern "C" void func_8047CF20__17UnkClass_8047CD0CFv(void*, void*);
extern "C" void func_8008962C(void* self) {
    if (*(u32*)((u8*)self + 0x30) != 0) {
        void* gm = func_80083298__Q22cf13CfGameManagerFv();
        if (gm && (u8*)gm + 0x2F2C) {
            func_8047CF20__17UnkClass_8047CD0CFv((u8*)gm + 0x2F2C, *(void**)((u8*)self + 0x30));
        }
        *(u32*)((u8*)self + 0x30) = 0;
    }
}

void func_80089684(void* self) {
    *(unsigned short*)((char*)self + 0x40) &= 1;
}

// Cache facing angle (atan2 of x/z scaled) and the passed-in scalar into the
// move data block. mpDataPtr is re-read per store, matching the two retail
// loads at +0x34.
void func_80089694(CCtrlMoveBase* self, const ml::CVec3* vec, f32 speed) {
    ((cf::CCtrlMoveData*)self->mpDataPtr)->field_0x0C =
        nw4r::math::Atan2FIdx(vec->x, vec->z) * lbl_eu_806665AC;
    ((cf::CCtrlMoveData*)self->mpDataPtr)->field_0x14 = speed;
}

// Move toward src: direction = normalized(src - entityPos), then lerp the
// horizontal velocity toward it and re-export/re-normalize.
void func_800896F4(CCtrlMoveBase* self, ml::CVec3* dst, const ml::CVec3* src) {
    ml::CVec3* pos = getPos(self);
    *dst = *src - *pos;
    normalizeOrZero(dst);
    self->mVelocity.x = self->mFloatParam2 * (dst->x - self->mVelocity.x) + self->mVelocity.x;
    self->mVelocity.z = self->mFloatParam2 * (dst->z - self->mVelocity.z) + self->mVelocity.z;
    *dst = self->mVelocity;
    normalizeOrZero(dst);
}

// Lerp the horizontal velocity toward dir, copy the velocity back into dir
// (struct copy -> retail's integer bit-move stores), then normalize.
// Zero-constant checks reference lbl_eu_806665A0 directly (retail reloads it
// through sda21 at each site rather than caching it in a register).
void func_800898D4(CCtrlMoveBase* self, ml::CVec3* dir) {
    self->mVelocity.x += self->mFloatParam2 * (dir->x - self->mVelocity.x);
    self->mVelocity.z += self->mFloatParam2 * (dir->z - self->mVelocity.z);
    *dir = self->mVelocity;
// (volatile reads: retail reloads the copied fields from memory for the
// comparisons instead of forwarding the registers held by the struct copy)
    ml::CVec3* chk = (ml::CVec3*)dir;
    if (*(volatile f32*)&chk->x == 0.0f && *(volatile f32*)&chk->y == 0.0f
            && *(volatile f32*)&chk->z == 0.0f) {
        return;
    }
    if (lbl_eu_806665A0 == dir->x * dir->x + dir->y * dir->y + dir->z * dir->z) {
        *dir = ml::CVec3::zero;
    } else {
        PSVECNormalize(*dir, *dir);
    }
}

struct func_80089990_child { char pad[0x10]; unsigned short flags; };
struct func_80089990_obj { char pad[0x30]; func_80089990_child* child; };
void func_80089990(func_80089990_obj* self) {
    func_80089990_child* p = self->child;
    if (p != 0) {
        p->flags &= 0xff83;
    }
}

void func_800899AC(void* obj, float val) {
    void* it = *(void**)((char*)obj + 0x30);
    if (it) {
        *(float*)((char*)it + 0x4) = val;
    }
}

// Move request with a short cache: for up to 30 frames after a successful
// pathing update, replay the cached direction from field_0x24 instead of
// querying the path instance again.
int func_800899C0(CCtrlMoveBase* self, ml::CVec3* out, void* arg, f32 val) {
    // Lazy-init +0x30 (single "proceed" assignment site mirrors the retail
    // flag diamond).
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;
    int ok;
    if (self->mpSomePtr != 0 || (gm = cf::CfGameManager::func_80083298()) == 0
            || (p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2F2C)) == 0)
        ok = 1;
    else if (p->field_0x00 == 0)
        ok = 0;
    else {
        void* inst = func_8047CE7C__17UnkClass_8047CD0CFv();
        self->mpSomePtr = inst;
        ok = (inst != 0);
    }
    if (ok == 0) {
        return 0;
    }

    if ((self->mFlagsU16_1 & 0x4) == 0) {
        u8 count = self->mByte1;
        self->mByte1 = count + 1;
        if (count <= 0x1e) {
            *out = self->field_0x24;
            return 1;
        }
    }

    ml::CVec3* pos = getPos(self);
    if (func_8047DC8C__17UnkClass_8047D2ACFv(self->mpSomePtr, &self->field_0x24,
                                             pos, val, lbl_eu_806665A0,
                                             lbl_eu_806665A0, (int)arg) != 0) {
        *out = self->field_0x24;
        self->mByte1 = 0;
        self->mFlagsU16_1 |= 0x4;
        return 1;
    }
    return 0;
}

// ============================================================================
// func_80089B24 - wobbly move update: timer re-roll + angle jitter (0x364)
// ============================================================================
int func_80089B24(CCtrlMoveBase* self, ml::CVec3* out) {
    s8 timer = (s8)self->mByte2;
    self->mByte2 = (u8)(timer - 1);
    if (timer <= 0) {
        int r = ml::math::mtRand(0x20);
        self->mByte2 = (u8)(r + 0x10);
        if ((r & 1) != 0) {
            self->mFlagsU16_1 &= ~0x8;
        } else {
            self->mFlagsU16_1 |= 0x8;
        }
    }

    f32 angle = lbl_eu_806665AC * nw4r::math::Atan2FIdx(self->mVelocity.x, self->mVelocity.z);
    f32 wobble;
    if ((self->mFlagsU16_1 & 0x8) != 0) {
        wobble = angle + lbl_eu_806665B0 * lbl_eu_8066A210;
    } else {
        wobble = angle - lbl_eu_806665B0 * lbl_eu_8066A210;
    }
    ml::CVec3 tmp;
    tmp.x = nw4r::math::SinFIdx(lbl_eu_806665B4 * wobble);
    tmp.z = nw4r::math::CosFIdx(lbl_eu_806665B4 * wobble);

    self->mVelocity.x = lbl_eu_8066659C * (tmp.x - self->mVelocity.x) + self->mVelocity.x;
    self->mVelocity.y = lbl_eu_806665A0;
    self->mVelocity.z = lbl_eu_8066659C * (tmp.z - self->mVelocity.z) + self->mVelocity.z;
    tmp.x = self->mVelocity.x;
    tmp.y = self->mVelocity.y;
    tmp.z = self->mVelocity.z;
    if (!(tmp.x == lbl_eu_806665A0 && tmp.y == lbl_eu_806665A0 && tmp.z == lbl_eu_806665A0)) {
        if (tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z == lbl_eu_806665A0) {
            tmp = ml::CVec3::zero;
        } else {
            PSVECNormalize(tmp, tmp);
        }
    } else {
        tmp.x = lbl_eu_806665A0;
        tmp.y = lbl_eu_806665A0;
        tmp.z = lbl_eu_806665A4;
    }

    ml::CVec3* pos = getPos(self);
    ml::CVec3 goal = *pos + tmp;

    int ok = 1;
    if (self->mpSomePtr == 0) {
        UnkClass_80083298* gm = cf::CfGameManager::func_80083298();
        if (gm != 0) {
            cf::CCtrlMoveMgr2F2C* p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2f2c);
            if (p != 0) {
                if (p->field_0x00 != 0) {
                    void* inst = func_8047CE7C__17UnkClass_8047CD0CFv();
                    self->mpSomePtr = inst;
                    ok = (inst != 0);
                } else {
                    ok = 0;
                }
            }
        }
    }
    if (ok != 0) {
        ml::CVec3* pos2 = getPos(self);
        ok = (func_8047DD4C__17UnkClass_8047D2ACFv(self->mpSomePtr, pos2, &goal,
                                                   lbl_eu_806665A0,
                                                   lbl_eu_806665A0, 1) != 0);
    }
    if (ok != 0) {
        *out = tmp;
        return 1;
    }

    ml::CVec3 up(lbl_eu_806665A0, lbl_eu_806665A4, lbl_eu_806665A0);
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&goal, (nw4r::math::VEC3*)&goal,
                        (nw4r::math::VEC3*)&up);
    if (func_804BE398(&goal, 0x4a05, 0, 0, lbl_eu_806665B8, lbl_eu_806665BC) != 0) {
        *out = tmp;
        return 1;
    }
    return 0;
}

// Path-follow request through the lazily created instance; passes both extra
// arguments straight through to the pathing helper.
int func_80089E88(CCtrlMoveBase* self, const ml::CVec3* src, int flag) {
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;
    int ok;
    if (self->mpSomePtr != 0 || (gm = cf::CfGameManager::func_80083298()) == 0
            || (p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2F2C)) == 0)
        ok = 1;
    else if (p->field_0x00 == 0)
        ok = 0;
    else {
        void* inst = func_8047CE7C__17UnkClass_8047CD0CFv();
        self->mpSomePtr = inst;
        ok = (inst != 0);
    }
    if (ok == 0) {
        return 0;
    }
    ml::CVec3* pos = getPos(self);
    return func_8047DD4C__17UnkClass_8047D2ACFv(self->mpSomePtr, pos, src,
                                                lbl_eu_806665A0,
                                                lbl_eu_806665A0, flag) != 0;
}

int func_80089F68(CCtrlMoveBase* self) {
    // Lazy-init +0x30, written as an explicit branch lattice so MWCC keeps
    // one shared "ok = 1" site and two separate "ok = 0" blocks (retail
    // layout). Only the two hard failures clear ok.
    int ok;
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;

    if (self->mpSomePtr != 0)
        goto okTrue;
    gm = cf::CfGameManager::func_80083298();
    if (gm == 0)
        goto okTrue;
    p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2F2C);
    if (p == 0)
        goto okTrue;
    if (p->field_0x00 == 0) {
        ok = 0;
        goto haveOk;
    }
    {
        void* inst = func_8047CE7C__17UnkClass_8047CD0CFv();
        self->mpSomePtr = inst;
        if (inst != 0)
            goto okTrue;
    }
    ok = 0;
    goto haveOk;
okTrue:
    ok = 1;
haveOk:
    if (ok != 0) {
        ml::CVec3* pos = getPos(self);
        return func_8047DE14__17UnkClass_8047D2ACFv(self->mpSomePtr, pos,
                                                     lbl_eu_806665A0, lbl_eu_806665A0);
    }
    return 0;
}

int func_8008A01C(CCtrlMoveBase* self, ml::CVec3* out) {
    // Lazy-init +0x30, written as an explicit branch lattice so MWCC keeps
    // one shared "ok = 1" site and two separate "ok = 0" blocks (retail
    // layout). Only the two hard failures clear ok.
    int ok;
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;
    void* inst;

    if (self->mpSomePtr != 0)
        goto okTrue;
    gm = cf::CfGameManager::func_80083298();
    if (gm == 0)
        goto okTrue;
    p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2F2C);
    if (p == 0)
        goto okTrue;
    if (p->field_0x00 == 0) {
        ok = 0;
        goto haveOk;
    }
    inst = func_8047CE7C__17UnkClass_8047CD0CFv();
    self->mpSomePtr = inst;
    if (inst != 0)
        goto okTrue;
    ok = 0;
    goto haveOk;
okTrue:
    ok = 1;
haveOk:
    if (ok != 0) {
        return func_8047DE3C__17UnkClass_8047D2ACFv(self->mpSomePtr, out,
                                                     lbl_eu_806665A0, lbl_eu_806665A0);
    }
    return 0;
}

// FULL_MATCH: no-op virtual stub (CCtrlMoveBase::func_8008A0C4)
extern "C" void func_8008A0C4() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8008A0C8() {
    extern void __ct__CColiProc(void*);
    extern char lbl_eu_80571810[];
    extern const void lbl_eu_80527814;

    __ct__CColiProc(lbl_eu_80571810);
    *(void**)lbl_eu_80571810 = (void*)&lbl_eu_80527814;
}
