// Translation unit for kyoshin/cf/CCtrlMoveBase
// This file implements the CtrlMoveBase class - base for move-control objects.
// Two virtual stubs are FULL_MATCH (100% instruction match, semantically equivalent).
//
// The base move controller works against a "move data" block (mpDataPtr): its
// object at +0x28 exposes the entity position through vtable slot 0xAC. The
// per-object runtime instance at +0x30 is lazily created through the game
// manager's sub-object at +0x2F2C.

// func_800D9354 has a single shared decl on kyoshin/cf/CBattleManagerApi.hpp;
// CfGameManager.hpp carries no local copy.
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"
#include <nw4r/math.h>

using cf::CCtrlMoveBase;

// Position source: data->mPosObj is a CfObject (vtable slot 0xAC is
// CfObject_UnkVirtualFunc23). Real owning class is CfObject, not a TU-local
// pad with _vNNN dummies. Call the real virtual directly.
static inline ml::CVec3* getPos(CCtrlMoveBase* self) {
    cf::CCtrlMoveData* data = (cf::CCtrlMoveData*)self->mpDataPtr;
    return data->mPosObj->CfObject_UnkVirtualFunc23();
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

// Constructor: stores the secondary vtable label at +0x48 (class is
// __declspec(novtable) so no compiler vtable is emitted). The secondary
// base CCtrlMoveSecondary's vptr is at +0x48, so we write the retail label
// there directly (same as CToken pattern).
CCtrlMoveBase::CCtrlMoveBase(void* dataPtr) {
    *(void**)((u8*)this + 0x48) = (void*)lbl_eu_80527808;
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
    // Declared up front so MWCC assigns the lazy-init locals the low
    // callee-saved slots (retail reuses them for src/flagB after init).
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;

    self->mFlagsU16_1 &= ~0x2;

    // Lazy-create the per-object move instance through the manager's
    // +0x2F2C sub-object.
    if (self->mpSomePtr == 0) {
        gm = cf::CfGameManager::getGameSubManager();
        if (gm != 0) {
            p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2f2c);
            if (p != 0) {
                if (p->field_0x00 == 0) {
                    // Manager not ready: steer straight toward src.
                    *out = *src - *getPos(self);
                    if (out->x != 0.0f || out->y != 0.0f || out->z != 0.0f) {
                        if (lbl_eu_806665A0 == out->x * out->x + out->y * out->y
                                + out->z * out->z) {
                            *out = ml::CVec3::zero;
                        } else {
                            PSVECNormalize(*out, *out);
                        }
                    }
                    self->mVelocity = *out;
                    return 1;
                }
                self->mpSomePtr = allocFreeNode__17UnkClass_8047CD0CFv();
                self->mVelocity = ml::CVec3::zero;
            }
        }
        if (self->mpSomePtr == 0) {
            *out = ml::CVec3::zero;
            return 0;
        }
    }

    int result = 0;
    // Redundant re-test is in the retail binary (kept verbatim).
    if (self->mpSomePtr != 0) {
        if (self->mFlagsU16_2 != 0) {
            self->mFlagsU16_2--;
            *out = self->mVelocity;
            return 1;
        }

        int r = func_8047D2AC__17UnkClass_8047D2ACFv(
            self->mpSomePtr, out, getPos(self),
            lbl_eu_806665A0, self->mFloatParam1, src);
        if (r == 0) {
            result = 1;
            if (flagA != 0) {
                self->mVelocity.x = self->mFloatParam2 * (out->x - self->mVelocity.x) + self->mVelocity.x;
                self->mVelocity.z = self->mFloatParam2 * (out->z - self->mVelocity.z) + self->mVelocity.z;
                if (flagB != 0) {
                    // Walk direction: export the lerped velocity, then run the
                    // ground/water probes ahead of the entity.
                    ml::CVec3 dir;
                    dir.x = self->mVelocity.x;
                    dir.y = self->mVelocity.y;
                    dir.z = self->mVelocity.z;
                    if (dir.x != 0.0f || dir.y != 0.0f || dir.z != 0.0f) {
                        if (lbl_eu_806665A0 == dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) {
                            dir = ml::CVec3::zero;
                        } else {
                            PSVECNormalize(dir, dir);
                        }
                    }
                    f32 gz = dir.z + getPos(self)->z;
                    f32 gy = lbl_eu_806665A4 + getPos(self)->y;
                    f32 gx = dir.x + getPos(self)->x;
                    ml::CVec3 goal;
                    goal.y = gy;
                    goal.z = gz;
                    goal.x = gx;
                    if (func_804BE398(&goal, 0x4a05, 0, 0, lbl_eu_806665A8, lbl_eu_806665A0) == 0) {
                        self->mFlagsU16_1 |= 0x2;
                        self->mVelocity = *out;
                        return 1;
                    }
                    ml::CVec3 probeHi;
                    func_804BE4E0(&probeHi, 0);
                    if (probeHi.y > lbl_eu_8066AF20) {
                        *out = dir;
                    } else {
                        ml::CVec3 probeLo;
                        func_804BE4B4(&probeLo, 0);
                        if (probeLo.y >= getPos(self)->y) {
                            *out = dir;
                        } else {
                            self->mFlagsU16_1 |= 0x2;
                            self->mVelocity = *out;
                            return 1;
                        }
                    }
                }
            } else {
                *out = self->mVelocity;
                if (out->x != 0.0f || out->y != 0.0f || out->z != 0.0f) {
                    if (lbl_eu_806665A0 == out->x * out->x + out->y * out->y + out->z * out->z) {
                        *out = ml::CVec3::zero;
                    } else {
                        PSVECNormalize(*out, *out);
                    }
                }
            }
        } else if (r == 1) {
            // Pathing cached a result: replay it for 30 frames (+0x42).
            self->mVelocity = *out;
            self->mFlagsU16_2 = 30;
            result = 1;
        } else {
            // Fallback: steer straight toward the source point.
            *out = *src - *getPos(self);
            if (out->x != 0.0f || out->y != 0.0f || out->z != 0.0f) {
                if (lbl_eu_806665A0 == out->x * out->x + out->y * out->y + out->z * out->z) {
                    *out = ml::CVec3::zero;
                } else {
                    PSVECNormalize(*out, *out);
                }
            }
            if (flagA != 0) {
                // flagB is intentionally unused on this path (retail never
                // tests r7 once r > 1).
                self->mVelocity.x = self->mFloatParam2 * (out->x - self->mVelocity.x) + self->mVelocity.x;
                self->mVelocity.z = self->mFloatParam2 * (out->z - self->mVelocity.z) + self->mVelocity.z;
                *out = self->mVelocity;
                if (out->x != 0.0f || out->y != 0.0f || out->z != 0.0f) {
                    if (lbl_eu_806665A0 == out->x * out->x + out->y * out->y + out->z * out->z) {
                        *out = ml::CVec3::zero;
                    } else {
                        PSVECNormalize(*out, *out);
                    }
                }
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
    // Lazy-init +0x30, written as an explicit branch lattice so MWCC keeps
    // one shared "ok = 1" site and two separate "ok = 0" blocks (retail
    // layout). Both the lazy-init failure AND the failed pathing call route
    // through the same ok variable into one shared test.
    int ok;
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;

    if (self->mpSomePtr != 0)
        goto okTrue;
    gm = cf::CfGameManager::getGameSubManager();
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
        void* inst = allocFreeNode__17UnkClass_8047CD0CFv();
        self->mpSomePtr = inst;
        if (inst != 0)
            goto okTrue;
    }
    ok = 0;
    goto haveOk;
okTrue:
    ok = 1;
haveOk:
    if (ok == 0)
        goto fail;
    {
        ml::CVec3* pos = getPos(self);
        if (walkPathCheck__17UnkClass_8047D2ACFv(self->mpSomePtr, pos, src,
                                                 lbl_eu_806665A0,
                                                 lbl_eu_806665A0, 1) == 0)
            goto fail;
    }
    *outFlag = 1;
    {
        ml::CVec3* pos = getPos(self);
        *out = *src - *pos;
    }
    out->y = lbl_eu_806665A0;
    if (out->x == lbl_eu_806665A0 && out->z == lbl_eu_806665A0)
        goto degen;
    // Normalize-or-snap WITHOUT the all-components-zero pre-guard (retail
    // only tests the squared length here).
    if (out->x * out->x + out->y * out->y + out->z * out->z
            == *(volatile f32*)&lbl_eu_806665A0) {
        *out = ml::CVec3::zero;
    } else {
        PSVECNormalize(*out, *out);
    }
    if (flag == 0) {
        self->mVelocity = *out;
        goto ret1;
    }
    // Lerp the horizontal velocity toward the normalized direction; when the
    // lerped horizontal velocity is exactly zero, leave *out as-is.
    self->mVelocity.y = lbl_eu_806665A0;
    self->mVelocity.x = self->mFloatParam2 * (out->x - self->mVelocity.x) + self->mVelocity.x;
    self->mVelocity.z = self->mFloatParam2 * (out->z - self->mVelocity.z) + self->mVelocity.z;
    if (self->mVelocity.x == lbl_eu_806665A0
            && self->mVelocity.z == lbl_eu_806665A0)
        goto ret1;
    if (self->mVelocity.x * self->mVelocity.x + self->mVelocity.y * self->mVelocity.y
            + self->mVelocity.z * self->mVelocity.z
            == *(volatile f32*)&lbl_eu_806665A0) {
        self->mVelocity = ml::CVec3::zero;
    } else {
        PSVECNormalize(self->mVelocity, self->mVelocity);
    }
    *out = self->mVelocity;
    goto ret1;
degen:
    // Degenerate horizontal direction: face +Z.
    out->x = lbl_eu_806665A0;
    out->y = lbl_eu_806665A0;
    out->z = lbl_eu_806665A4;
    self->mVelocity = *out;
ret1:
    return 1;
fail:
    *outFlag = 0;
    return func_80088974(self, out, src, flag, 0);
}

// ============================================================================
// func_80089398 - horizontal direction toward a source point (0x210)
// ============================================================================
void func_80089398(CCtrlMoveBase* self, ml::CVec3* dst, const ml::CVec3* src,
                   int flag) {
    ml::CVec3* pos = ((cf::CCtrlMoveData*)self->mpDataPtr)->mPosObj->CfObject_UnkVirtualFunc23();
    ml::CVec3 diff = *src - *pos;
    *dst = diff;
    if (dst->x != 0.0f || dst->y != 0.0f || dst->z != 0.0f) {
        if (dst->x * dst->x + dst->y * dst->y + dst->z * dst->z == 0.0f) {
            *dst = ml::CVec3::zero;
        } else {
            PSVECNormalize(*dst, *dst);
        }
    }
    if (flag != 0) {
        self->mVelocity.x = self->mFloatParam2 * (dst->x - self->mVelocity.x) + self->mVelocity.x;
        self->mVelocity.z = self->mFloatParam2 * (dst->z - self->mVelocity.z) + self->mVelocity.z;
        *dst = self->mVelocity;
        if (dst->x != 0.0f || dst->y != 0.0f || dst->z != 0.0f) {
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
        self->unk08();
    }
}

// (lazy-init logic is duplicated inline in func_80089F68 / func_8008A01C)

// FULL_MATCH: no-op virtual stub (CCtrlMoveBase::func_80089628)
void func_80089628() {}

namespace cf {
void CCtrlMoveBase::unk08() {}
}

// Dispatch the pending +0x30 slot through the manager's +0x2F2C sub-object,
// then clear it.
extern "C" void* getGameSubManager__Q22cf13CfGameManagerFv(void);
extern "C" void releaseNode__17UnkClass_8047CD0CFv(void*, void*);
extern "C" void func_8008962C(void* self) {
    if (*(u32*)((u8*)self + 0x30) != 0) {
        void* gm = getGameSubManager__Q22cf13CfGameManagerFv();
        if (gm && (u8*)gm + 0x2F2C) {
            releaseNode__17UnkClass_8047CD0CFv((u8*)gm + 0x2F2C, *(void**)((u8*)self + 0x30));
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
// Uses the same idiom that leaves func_800898D4 reg-swap-only: 0.0f literals
// for the component guards (MWCC lowers them to the sda21 A0 load) and a
// volatile read of lbl_eu_806665A0 for the length test.
void func_800896F4(CCtrlMoveBase* self, ml::CVec3* dst, const ml::CVec3* src) {
    ml::CVec3* pos = getPos(self);
    *dst = *src - *pos;
    if (dst->x != 0.0f || dst->y != 0.0f || dst->z != 0.0f) {
        if (*(volatile f32*)&lbl_eu_806665A0 == dst->x * dst->x + dst->y * dst->y + dst->z * dst->z) {
            *dst = ml::CVec3::zero;
        } else {
            PSVECNormalize(*dst, *dst);
        }
    }
    self->mVelocity.x += self->mFloatParam2 * (dst->x - self->mVelocity.x);
    self->mVelocity.z += self->mFloatParam2 * (dst->z - self->mVelocity.z);
    *dst = self->mVelocity;
    if (dst->x != 0.0f || dst->y != 0.0f || dst->z != 0.0f) {
        if (*(volatile f32*)&lbl_eu_806665A0 == dst->x * dst->x + dst->y * dst->y + dst->z * dst->z) {
            *dst = ml::CVec3::zero;
        } else {
            PSVECNormalize(*dst, *dst);
        }
    }
}

// Lerp the horizontal velocity toward dir, copy the velocity back into dir
// (struct copy -> retail's integer bit-move stores), then normalize.
// Zero-constant checks reference lbl_eu_806665A0 directly (retail reloads it
// through sda21 at each site rather than caching it in a register).
// Tail shape: zero-length guard + normalize-or-snap.
void func_800898D4(CCtrlMoveBase* self, ml::CVec3* dir) {
    self->mVelocity.x += self->mFloatParam2 * (dir->x - self->mVelocity.x);
    self->mVelocity.z += self->mFloatParam2 * (dir->z - self->mVelocity.z);
    *dir = self->mVelocity;
    // Negated form: same branch lattice as retail (bne/bne/beqlr); testing
    // whether the != tree avoids the pool-temp operand normalization.
    if (!(dir->x != 0.0f || dir->y != 0.0f || dir->z != 0.0f)) {
        return;
    }
    if (dir->x * dir->x + dir->y * dir->y + dir->z * dir->z
            == *(volatile f32*)&lbl_eu_806665A0) {
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
    // Lazy-init +0x30, written as an explicit branch lattice so MWCC keeps
    // one shared "ok = 1" site and two separate "ok = 0" sites (retail
    // layout); both hard failures and the failed pathing call share the
    // single "return 0" block.
    int ok;
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;

    if (self->mpSomePtr != 0)
        goto okTrue;
    gm = cf::CfGameManager::getGameSubManager();
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
        void* inst = allocFreeNode__17UnkClass_8047CD0CFv();
        self->mpSomePtr = inst;
        if (inst != 0)
            goto okTrue;
    }
    ok = 0;
    goto haveOk;
okTrue:
    ok = 1;
haveOk:
    if (ok == 0)
        goto ret0;

    // Short cache: once flag bit 2 is set (path cached), replay field_0x24
    // for up to 30 frames (counter at +0x44 bounds the window).
    if ((self->mFlagsU16_1 & 0x4) != 0) {
        u8 count = self->mByte1;
        self->mByte1 = count + 1;
        if (count <= 0x1e) {
            *out = self->field_0x24;
            return 1;
        }
    }
    {
        ml::CVec3* pos = getPos(self);
        if (func_8047DC8C__17UnkClass_8047D2ACFv(self->mpSomePtr,
                                                 &self->field_0x24, pos, val,
                                                 lbl_eu_806665A0,
                                                 lbl_eu_806665A0,
                                                 (int)arg) == 0)
            goto ret0;
        self->mByte1 = 0;
        *out = self->field_0x24;
        self->mFlagsU16_1 |= 0x4;
        return 1;
    }
ret0:
    return 0;
}

// ============================================================================
// func_80089B24 - wobbly move update: timer re-roll + angle jitter (0x364)
// ============================================================================
int func_80089B24(CCtrlMoveBase* self, ml::CVec3* out) {
    // Declared up front so MWCC assigns it the low stack slot (retail
    // layout: up@0x14, goal@0x20, tmp@0x2c).
    ml::CVec3 up;
    // u8 local + signed cast at the test: MWCC computes the decrement on the
    // raw byte (subi) and sign-extends only the compared value.
    u8 timer = self->mByte2;
    self->mByte2 = timer - 1;
    if ((s8)timer <= 0) {
        int r = ml::math::mtRand(0x20);
        self->mByte2 = (u8)(r + 0x10);
        if ((r & 1) != 0) {
            self->mFlagsU16_1 &= ~0x8;
        } else {
            self->mFlagsU16_1 |= 0x8;
        }
    }

    // Two-step product-then-add per branch blocks MWCC's fp-contract
    // fusion, keeping retail's separate fmuls + fadds/fsubs.
    f32 angle = lbl_eu_806665AC * nw4r::math::Atan2FIdx(self->mVelocity.x, self->mVelocity.z);
    f32 wobble;
    if ((self->mFlagsU16_1 & 0x8) != 0) {
        f32 off = lbl_eu_806665B0 * lbl_eu_8066A210;
        wobble = angle + off;
    } else {
        f32 off = lbl_eu_806665B0 * lbl_eu_8066A210;
        wobble = angle - off;
    }
    ml::CVec3 tmp;
    tmp.x = nw4r::math::SinFIdx(lbl_eu_806665B4 * wobble);
    tmp.z = nw4r::math::CosFIdx(lbl_eu_806665B4 * wobble);

    self->mVelocity.x = lbl_eu_8066659C * (tmp.x - self->mVelocity.x) + self->mVelocity.x;
    self->mVelocity.y = lbl_eu_806665A0;
    self->mVelocity.z = lbl_eu_8066659C * (tmp.z - self->mVelocity.z) + self->mVelocity.z;
    // Whole-struct copy: retail moves the three words with lwz/stw pairs.
    tmp = self->mVelocity;
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

    // Lazy-init +0x30 as a goto lattice so `ok` is set only in terminal
    // blocks and consumed immediately (retail keeps it in r0, never in a
    // callee-saved register).
    int ok;
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;

    if (self->mpSomePtr != 0)
        goto okTrue;
    gm = cf::CfGameManager::getGameSubManager();
    if (gm == 0)
        goto okTrue;
    p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2f2c);
    if (p == 0)
        goto okTrue;
    if (p->field_0x00 == 0) {
        ok = 0;
        goto haveOk;
    }
    {
        void* inst = allocFreeNode__17UnkClass_8047CD0CFv();
        self->mpSomePtr = inst;
        if (inst != 0)
            goto okTrue;
    }
    ok = 0;
    goto haveOk;
okTrue:
    ok = 1;
haveOk:
    int ok2;
    if (ok != 0) {
        ml::CVec3* pos2 = getPos(self);
        ok2 = (walkPathCheck__17UnkClass_8047D2ACFv(self->mpSomePtr, pos2,
                                                    &goal, lbl_eu_806665A0,
                                                    lbl_eu_806665A0, 1) != 0);
    } else {
        ok2 = 0;
    }
    if (ok2 != 0) {
        *out = tmp;
        return 1;
    }

    up.x = lbl_eu_806665A0;
    up.y = lbl_eu_806665A4;
    up.z = lbl_eu_806665A0;
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
    // Lazy-init +0x30, written as an explicit branch lattice so MWCC keeps
    // one shared "ok = 1" site and two separate "ok = 0" blocks (retail
    // layout). Only the two hard failures clear ok.
    int ok;
    UnkClass_80083298* gm;
    cf::CCtrlMoveMgr2F2C* p;

    if (self->mpSomePtr != 0)
        goto okTrue;
    gm = cf::CfGameManager::getGameSubManager();
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
        void* inst = allocFreeNode__17UnkClass_8047CD0CFv();
        self->mpSomePtr = inst;
        if (inst != 0)
            goto okTrue;
    }
    ok = 0;
    goto haveOk;
okTrue:
    ok = 1;
haveOk:
    // Both failure paths share one "return 0" block (retail routes the
    // ok-failure branch AND the failed-call branch into the same li r3,0
    // block); success falls through li r3,1 straight to the epilogue.
    if (ok == 0)
        goto ret0;
    {
        ml::CVec3* pos = getPos(self);
        if (walkPathCheck__17UnkClass_8047D2ACFv(self->mpSomePtr, pos, src,
                                                  lbl_eu_806665A0,
                                                  lbl_eu_806665A0, flag) != 0)
            return 1;
    }
ret0:
    return 0;
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
    gm = cf::CfGameManager::getGameSubManager();
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
        void* inst = allocFreeNode__17UnkClass_8047CD0CFv();
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
        return dispatchScnCommand__17UnkClass_8047D2ACFv(self->mpSomePtr, pos,
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
    gm = cf::CfGameManager::getGameSubManager();
    if (gm == 0)
        goto okTrue;
    p = (cf::CCtrlMoveMgr2F2C*)((char*)gm + 0x2F2C);
    if (p == 0)
        goto okTrue;
    if (p->field_0x00 == 0) {
        ok = 0;
        goto haveOk;
    }
    inst = allocFreeNode__17UnkClass_8047CD0CFv();
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

#include <decomp.h>
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

    0x63, 0x66, 0x3A, 0x3A, 0x43, 0x74, 0x72, 0x6C, 0x45, 0x6E, 0x65, 0x6D,
    0x79, 0x00, 0x00, 0x00, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x74, 0x72, 0x6C,
    0x41, 0x63, 0x74, 0x00, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x74, 0x72, 0x6C,
    0x42, 0x61, 0x73, 0x65, 0x00, 0x00, 0x00, 0x00, 0x43, 0x41, 0x63, 0x74,
    0x49, 0x6E, 0x70, 0x75, 0x74, 0x00, 0x00, 0x00, 0x63, 0x66, 0x3A, 0x3A,
    0x43, 0x74, 0x72, 0x6C, 0x4D, 0x6F, 0x76, 0x65, 0x42, 0x61, 0x73, 0x65,
    0x00, 0x00, 0x00, 0x00, 0x63, 0x66, 0x3A, 0x3A, 0x43, 0x74, 0x72, 0x6C,
    0x45, 0x6E, 0x65, 0x41, 0x69, 0x43, 0x61, 0x6C, 0x6C, 0x00, 0x00, 0x00,
    0x43, 0x43, 0x6F, 0x6C, 0x69, 0x50, 0x72, 0x6F, 0x63, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x44, 0xC8, 0x00, 0x00, 0x46, 0x1C, 0x40, 0x00,
    0x45, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

    0x3D, 0xCC, 0xCC, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0xC0, 0x00, 0x00, 0x00, 0x3C, 0xC9, 0x0F, 0xDB, 0x42, 0x20, 0x00, 0x00,
    0x42, 0x22, 0xF9, 0x83, 0xC0, 0x80, 0x00, 0x00, 0x3F, 0x35, 0x04, 0x81
};
