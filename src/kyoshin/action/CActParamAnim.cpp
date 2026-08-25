// Auto-scaffolded catalog TU for kyoshin/action/CActParamAnim
// Replace stubs with high-level C/C++ during decomp.

// CActParamData.hpp declares func_80055B88 with its real (self, key, tag)
// signature, but retail's func_8004B6BC call site passes only the child
// pointer (the original retail TU used a stale prototype). Hide the typed
// declaration in this TU so the byte-exact bare 1-arg bl (locked FULL_MATCH
// us-8004bd94) still compiles.
#define func_80055B88 func_80055B88_typed_hidden
// CActParamData.hpp also declares func_80053960 with an ActParamData388*
// parameter; retail's func_80053960 lives in this TU and takes the anim
// object (CActParamAnim*). Hide that typed declaration so the extern "C"
// definition below does not collide with it as an illegal overload.
#define func_80053960 func_80053960_typed_hidden

#include "kyoshin/action/CActParamAnim.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)
#undef func_80055B88
#undef func_80053960

// Legacy 1-arg prototype kept for the func_8004B6BC call site (see above).
extern "C" bool func_80055B88(void* data);

#include "kyoshin/action/CActParamData.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <math.h>
#include <monolib/math/CQuat.hpp>
#include <monolib/math/CVec3.hpp>
#include <monolib/math/Random.hpp>
#include <nw4r/math.h>
#include <revolution/mtx/quat.h>
#include <revolution/mtx/vec.h>

CActParamAnim::CActParamAnim() : mField0C(0) {}

extern "C" CActParamAnim* __dt__8004B070(CActParamAnim* self, s32 deleteFlag) {
    if (self != nullptr && deleteFlag > 0) {
        __dl__FPv(self);
    }
    return self;
}

extern "C" __declspec(noinline) void func_8004B0B0(void*) {}

extern "C" __declspec(noinline) void func_8004B0B4(void*) {}

CActParamAnim::~CActParamAnim() {}

void CActParamAnim::func_8004B114() {}

void* CActParamAnim::getChild() { return &reinterpret_cast<CActParamAnimFull*>(this)->mChildData; }

float CActParamAnim::getBlendWeight() {
    return *(float*)((char*)this + 0x43c);
}

struct CActParamAnimObjectView {
    u8 _pad_000[0x3A0];
    void* object;
    u32 objectState;
    f32 field_3A8;
    f32 field_3AC;
    f32 field_3B0;
};

struct CActParamAnimFlagView {
    u8 _pad_000[0x0C];
    u32 flags;
};

bool func_8004B354(CActParamAnim* self, const CActParamAnimData3* value) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimData3* destination;
    if (view->object3A0 == nullptr) goto ret_false;
    if ((view->field0C & 0x100) != 0) goto ret_false;
    // Commit the new position to the cached state and the attached sub-object.
    view->field3A8 = value->x;
    view->field3AC = value->y;
    view->field3B0 = value->z;
    destination = reinterpret_cast<CActParamAnimData3*>(func_8048315C(view->object3A0));
    destination->x = value->x;
    destination->y = value->y;
    destination->z = value->z;
    return true;
ret_false:
    return false;
}

int CActParamAnim::checkFlag(int mask) {
    return (*(int*)((char*)this) & mask) != 0 ? 1 : 0;
}

void CActParamAnim_copyTranslation(void* dst, const void* src) {
    *(int*)((char*)dst + 0) = *(int*)((char*)src + 0);
    *(int*)((char*)dst + 4) = *(int*)((char*)src + 4);
    *(int*)((char*)dst + 8) = *(int*)((char*)src + 8);
}

extern "C" __declspec(noinline) void func_8004B3F0(f32* dst, const f32* src) {
    u32 a = *(u32*)((char*)src + 0);
    *(u32*)((char*)dst + 0) = a;
    u32 b = *(u32*)((char*)src + 4);
    *(u32*)((char*)dst + 4) = b;
    u32 c = *(u32*)((char*)src + 8);
    *(u32*)((char*)dst + 8) = c;
}

bool func_8004B40C(CActParamAnim* self, const CActParamAnimData3* value) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    view->field488 = *reinterpret_cast<const f32*>(&value->y);
    u32 flags = view->field0C | 0x10;
    view->field0C = flags;
    bool result;
    CActParamAnimData3* destination;
    if (view->object3A0 == nullptr) goto ret_zero;
    if ((flags & 0x100) != 0) goto ret_zero;
    // Commit the new position to the cached state and the attached sub-object.
    view->field3A8 = value->x;
    view->field3AC = value->y;
    view->field3B0 = value->z;
    destination = reinterpret_cast<CActParamAnimData3*>(func_8048315C(view->object3A0));
    destination->x = value->x;
    destination->y = value->y;
    destination->z = value->z;
    result = true;
    goto ret_end;
ret_zero:
    result = false;
ret_end:
    return result;
}

extern "C" bool func_8004B4A4(CActParamAnim* self, f32 value) {
    CActParamAnimOwnerIf* owner =
        *reinterpret_cast<CActParamAnimOwnerIf**>(reinterpret_cast<u8*>(self) + 8);
    if (owner != NULL && owner->v3() == 0) {
        return true;
    }
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if ((view->field0C & 0x20) == 0) {
        view->field440 = value;
    }
    return true;
}

void* CActParamAnim::getOwner() {
    return *(void**)((char*)this + 8);
}

bool CActParamAnim_isEnabled() { return true; }

bool func_8004B52C(CActParamAnim* self, f32 value) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    if (owner == 0 || owner->v3() != 0) {
        if ((view->field0C & 0x20) == 0) {
            view->field440 = value;
        }
    }
    if ((view->field0C & 0x20) == 0) {
        u8* obj = view->object3A0;
        view->field444 = value;
        if (obj != 0) {
            f32 tmp[3];
            tmp[1] = (f32)(f64)value;
            tmp[0] = lbl_eu_80665EA0;
            tmp[2] = lbl_eu_80665EA0;
            CActParamAnimObjVec* dst =
                reinterpret_cast<CActParamAnimObjVec*>(func_8048315C(obj));
            dst->x = *reinterpret_cast<u32*>(&tmp[0]);
            dst->y = *reinterpret_cast<u32*>(&tmp[1]);
            dst->z = *reinterpret_cast<u32*>(&tmp[2]);
        }
    }
    return true;
}

void CActParamAnim_copyRotation(void* dst, const void* src) {
    *(int*)((char*)dst + 0xC) = *(int*)((char*)src + 0);
    *(int*)((char*)dst + 0x10) = *(int*)((char*)src + 4);
    *(int*)((char*)dst + 0x14) = *(int*)((char*)src + 8);
}

void vec3_set(float *data, float a, float b, float c) { data[0] = a; data[1] = b; data[2] = c; }

extern "C" f32 func_8004B61C(CActParamAnim* self) {
    return *(f32*)((char*)self + 0x444);
}

extern "C" void func_8004B624(CActParamAnim* self, void* object, void* state,
                              u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    view->object3A0 = reinterpret_cast<u8*>(object);
    view->state3A4 = reinterpret_cast<u32>(state);
    view->field0C |= 8;
    func_80053B24(reinterpret_cast<CActParamData*>(reinterpret_cast<u8*>(self) + 0x10), self, self, state,
                  reinterpret_cast<void*>(param));
    func_8004B9D4(self, 1, 0, -1, 0);
}

void bits_clear(uint32_t* ptr, uint32_t mask) {
    *ptr &= ~mask;
}

void CActParamAnim::updateSubAnim() {
    if (*(int*)((char*)this + 0x3A4) != 0) func_80055AC4((char*)this + 16);
}

extern "C" void func_8004B6BC(CActParamAnim* self, void* object) {
    CActParamAnimObjectView* view = reinterpret_cast<CActParamAnimObjectView*>(self);
    if (view->objectState != 0 && func_80055B88(reinterpret_cast<u8*>(self) + 0x10)) {
        if (view->objectState == reinterpret_cast<u32>(object)) {
            view->objectState = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + 0x1E0);
        }
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + 0x374) = 0;
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + 0x37C) = 0xFFFFFFFF;
    }
}

void CActParamAnim::setOwner(int val) { *(int*)((char*)this + 8) = val; }

extern "C" __declspec(noinline) void func_8004B738(float* destination, const float* source) {
    *reinterpret_cast<nw4r::math::VEC3*>(destination) +=
        *reinterpret_cast<const nw4r::math::VEC3*>(source);
}

extern "C" __declspec(noinline) void func_8004B75C(float* destination, const float* source, float scale) {
    nw4r::math::VEC3 scaled =
        *reinterpret_cast<const nw4r::math::VEC3*>(source) * scale;
    destination[0] = scaled.x;
    destination[1] = scaled.y;
    destination[2] = scaled.z;
}

void vec3_copy(float *dst, const float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

float CActParamAnim::getAnimSpeed() const {
    return *(const float*)((const char*)this + 0x390);
}

extern "C" f32 func_8004B7B8(CActParamAnim* self) {
    return *(f32*)((char*)self + 0x390);
}

void CActParamAnim::copyVec3To3C0(const float* src) {
    *(int*)((char*)this + 0x3C0) = *(int*)((char*)src + 0);
    *(int*)((char*)this + 0x3C4) = *(int*)((char*)src + 4);
    *(int*)((char*)this + 0x3C8) = *(int*)((char*)src + 8);
}

struct CActParamAnimMotionView {
    u8 _pad_000[0x390];
    f32 animSpeed;
    u8 _pad_394[0x30];
    f32 vec3Y;
};

extern "C" void func_8004B7DC(CActParamAnim* self, f32 amount) {
    CActParamAnimMotionView* motion =
        reinterpret_cast<CActParamAnimMotionView*>(self);
    f32 animSpeed = motion->animSpeed;
    f32 frameSeconds = CDeviceVI::getSecPerFrame();
    f32 frameScale = animSpeed * frameSeconds;
    motion->vec3Y = amount * frameScale + motion->vec3Y;
}

void CActParamAnim::setVec3Y(float val) {
    *(float*)((char*)this + 0x3c4) = val;
}

int CActParamAnim::checkAnimFlag() {
    return (*(unsigned int *)((char *)this + 12) >> 1) & 1;
}

extern "C" u32 func_8004B848(const CActParamAnim* self) {
    return (*(const u32*)((const u8*)self + 0xC) >> 1) & 1;
}

float CActParamAnim::func_8004B854() {
    extern float func_80484F18(void*);
    void* obj = *(void**)((char*)this + 0x3A0);
    if (obj) return func_80484F18(obj);
    return lbl_eu_80665EA0;
}

float CActParamAnim::func_8004B86C() {
    extern float func_804850A4(void*);
    void* obj = *(void**)((char*)this + 0x3A0);
    if (obj != 0) return func_804850A4(obj);
    return lbl_eu_80665EA0;
}

void CActParamAnim::callObjUpdate() {
    extern void func_80484F80(void*);
    void* obj = *(void**)((char*)this + 0x3A0);
    if (obj != 0) func_80484F80(obj);
}

float CActParamAnim::func_8004B898() {
    extern float func_80484EB0(void*);
    void* obj = *(void**)((char*)this + 0x3A0);
    if (obj != 0) return func_80484EB0(obj);
    return lbl_eu_80665EA0;
}

extern "C" void func_8004B8B0(CActParamAnim* self, u32 param1, u32 param2, f32 value) {
    if (param1 != 0) {
        *(f32*)((u8*)self + 0x480) = value;
        u32 flags = *(u32*)((u8*)self + 0xC);
        flags |= 0x10000;
        *(u32*)((u8*)self + 0xC) = flags;
        if (param2 != 0) {
            flags |= 0x20000;
            *(u32*)((u8*)self + 0xC) = flags;
        } else {
            flags &= ~0x20000;
            *(u32*)((u8*)self + 0xC) = flags;
        }
    } else {
        u32 flags = *(u32*)((u8*)self + 0xC);
        flags &= ~0x30000;
        *(u32*)((u8*)self + 0xC) = flags;
    }
}

int func_8004B8F8(CActParamAnim* self, int param) {
    // First read feeds the null check; the second read through self exists
    // because func_8049798C clobbers r5, forcing a reload (retail keeps no
    // spill home for obj). Byte-invariant residual: retail interleaves the
    // first load with the prologue spills; every source shape tried emits
    // them grouped (scheduler tie-break wall, MWCC_CASES near-miss trio).
    u8* obj = *(u8**)((char*)self + 0x3A0);
    if (!obj) return 0;
    if (param == 0) {
        return func_80484F18(obj) <= lbl_eu_80665E9C;
    }
    if (func_8049798C(obj + 0xC) != 0) {
        u8* obj2 = *(u8**)((char*)self + 0x3A0);
        return func_80485174(obj2) <= lbl_eu_80665E9C;
    }
    return 0;
}

extern int func_80485464(void*, void*);
extern int func_80485244(void*);
int CActParamAnim::callObjFunc(void* param) {
    void* val = *(void**)((char*)this + 0x3A0);
    if (!val) return 0;
    if (!param) {
        return func_80485244(val);
    }
    return func_80485464(val, param);
}

void* CActParamAnim::getNextChainObj() {
    void* v = *(void**)((char*)this + 0x8);
    if (v) {
        return *(void**)((char*)v + 0x18);
    }
    return *(void**)((char*)this + 0x4b4);
}

void func_8004B9D4(){}

extern "C" f32 func_8004BC28(f32 value) {
    // Wrap an angle into [-pi, pi) by adding/subtracting 2*pi.
    while (lbl_eu_8066A1F8 <= value) value -= lbl_eu_8066A1FC;
    while (value < -lbl_eu_8066A1F8) value += lbl_eu_8066A1FC;
    return value;
}

void CActParamAnim::setBlendFlag(int param) {
    if (param != 0) {
        int tmp = *(int*)((char*)this + 0xc);
        *(unsigned char*)((char*)this + 0x4da) = 30;
        *(int*)((char*)this + 0xc) = tmp | 0x4000000;
    } else {
        *(int*)((char*)this + 0xc) &= ~0x4000000;
    }
}

// Open-item packet (plateau, best 28 mismatch / 24 structural / 4 reg-swap,
// size PASS, reloc drift none):
// - Residual is confined to the small-path call-arg materialization region.
//   Retail copies param to r5 and computes &local BEFORE the model fallback
//   branches, then computes data as `addi r3,r3,0x10` last (self parked in
//   r31). Our build parks data in r5 early and defers all arg setup until
//   after the branches, needing two extra `or` copies.
// - Ruled out: early-return guard form (worse, branch polarity), removing
//   the data temp (308B, load order shifts), inline self+0x10 arg (308B),
//   early `&local` pointer var (changes prologue, 304B), ternary fallback
//   (canonicalizes identically to if), flag logic inlined into the 5th call
//   argument via comma/&& expression (324B), no-data-temp + ternary (308B),
//   inlined self->getChild() accessor as arg1 (308B, same as plain field).
//   Single-var model chain adopted; data temp required for size parity.
// - Next experiments: none found within high-level C++; likely MWCC scheduler
//   tie-break on hoisting side-effect-free arg computations above branches.
// - Confirmed on func_8005194C too: reordering model/clear/b/data statements,
//   forcing a post-clear byte reload, and hoisting param into a dedicated
//   local (animId) all compile to the identical banked body (MWCC CSEs the
//   reload and canonicalizes the param copy); any full reorder regresses size.
// - Also ruled out: inlining BOTH arg1 (mChildData10) and arg4 (field4BD) at
//   the call site to force late data materialization (332B, 58 structural —
//   worse; MWCC drops the r31 spill and reshuffles the whole small path).
// - Confirmed on func_8004D194 too: rewriting the 0x40000/0x12 guard from
//   !(&&!=0) to the equivalent disjunction and reordering the small-path
//   statements (model load before data, flags snapshot split) compile to the
//   identical banked body (53 mismatch / 42 structural unchanged) - MWCC
//   canonicalizes guard polarity and keeps its own arg-materialization order
//   (param copied via or r5,r31 early, data = addi r3,r30,0x10 mid-sequence,
//   model chain in r8). Same scheduler tie-break wall family.
void func_8004BC94(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if (param != 0) {
        if (param < 0x68) {
            // The 0x40000 guard flag skips the anim start while the requested
            // anim is 0x12.
            if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                if (view->field4BD == 0) {
                    view->field4BD = 0;
                }
                u8* data = view->mChildData10;
                u32 model = view->field2FC;
                u8 b = view->field4BD;
                view->field0C &= ~0x200;
                u32 flag = 0;
                if (model == 0) model = view->field27C;
                if (model == 1) {
                    model = view->field270;
                    if ((model & 0x10) == 0 && (model & 0x8) == 0) flag = 1;
                }
                u32 local;
                u32 ret = func_80054170(data, &local, param, b, flag);
                func_8004BDCC(self, local, ret, view->field2A4, 0);
                view->field0C &= ~0x200;
            }
        } else {
            u32 sel = view->field278;
            u32 flag = 0;
            if (sel == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 local;
            u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
            func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
    }
}

// func_8004BDCC: start (or switch) the anim `param` on the attached
// sub-object. sel selects the heading/frame source pair, resId overrides the
// sub-object's resource file when non-zero, useDefaultScale forces the blend
// frame count to the lbl_eu_80665EA0 default.
void func_8004BDCC(CActParamAnim* self, u32 param, u32 sel, u32 resId,
                   u32 useDefaultScale) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if ((s32)param < 0) return;
    // Retail reuses the resource-id argument itself when falling back to the
    // sub-object's cached state word.
    if (resId == 0) resId = view->state3A4;
    if (resId == 0) return;
    if ((s32)param >= (s32)GetResAnmChrNumEntries__Q34nw4r3g3d7ResFileCFv(
                          reinterpret_cast<u8*>(resId) + 0xC))
        return;

    f32 prevBlend = reinterpret_cast<CActParamAnimVt0C*>(view)->f01();
    f32 blendTime = reinterpret_cast<CActParamAnimVt14*>(view)->f03();

    if ((view->field0C & 1) != 0) {
        // Rewind the previous anim's playback offset before switching.
        f32 v49c = view->field49C;
        if ((view->field270 & 0x1000) != 0)
            func_80484E5C(view->object3A0, -v49c);
        else
            func_80484E5C(view->object3A0, v49c);
        view->field0C &= ~1u;
    }

    view->field37C = -1;
    f32 blendFrames =
        func_80053DE8(reinterpret_cast<CActParamData*>(view->mChildData10), sel);
    if (useDefaultScale != 0) blendFrames = lbl_eu_80665EA0;

    f32 startAngle = (sel != 0) ? view->field334 : view->field2B4;
    if (startAngle != lbl_eu_80665EA0) {
        // Blend the stored heading toward the target, wrapped into (-pi, pi].
        // Each wrap branch duplicates the owner-gate store + sub-object mirror
        // tail (retail layout).
        f32 angle = startAngle + view->field444;
        blendFrames = lbl_eu_80665EA0;
        const f32 pi = lbl_eu_8066A1F8;
        const f32 twoPi = lbl_eu_8066A1FC;
        if (angle > pi) {
            angle -= twoPi;
            int ownerGate = 0;
            if (view->owner08 != NULL) ownerGate = view->owner08->v3();
            if (ownerGate == 0 && (view->field0C & 0x20) == 0)
                view->field440 = angle;
            if ((view->field0C & 0x20) == 0) {
                view->field444 = angle;
                u8* obj = view->object3A0;
                if (obj != NULL) {
                    // Mirror the blended heading onto the sub-object position.
                    CActParamAnimData3 pos;
                    pos.x = reinterpret_cast<const u32&>(lbl_eu_80665EA0);
                    pos.y = reinterpret_cast<const u32&>(angle);
                    pos.z = reinterpret_cast<const u32&>(lbl_eu_80665EA0);
                    *reinterpret_cast<CActParamAnimData3*>(func_8048315C(obj)) = pos;
                }
            }
        } else if (angle < -pi) {
            angle += twoPi;
            int ownerGate = 0;
            if (view->owner08 != NULL) ownerGate = view->owner08->v3();
            if (ownerGate == 0 && (view->field0C & 0x20) == 0)
                view->field440 = angle;
            if ((view->field0C & 0x20) == 0) {
                view->field444 = angle;
                u8* obj = view->object3A0;
                if (obj != NULL) {
                    CActParamAnimData3 pos;
                    pos.x = reinterpret_cast<const u32&>(lbl_eu_80665EA0);
                    pos.y = reinterpret_cast<const u32&>(angle);
                    pos.z = reinterpret_cast<const u32&>(lbl_eu_80665EA0);
                    *reinterpret_cast<CActParamAnimData3*>(func_8048315C(obj)) = pos;
                }
            }
        } else {
            int ownerGate = 0;
            if (view->owner08 != NULL) ownerGate = view->owner08->v3();
            if (ownerGate == 0 && (view->field0C & 0x20) == 0)
                view->field440 = angle;
            if ((view->field0C & 0x20) == 0) {
                view->field444 = angle;
                u8* obj = view->object3A0;
                if (obj != NULL) {
                    CActParamAnimData3 pos;
                    pos.x = reinterpret_cast<const u32&>(lbl_eu_80665EA0);
                    pos.y = reinterpret_cast<const u32&>(angle);
                    pos.z = reinterpret_cast<const u32&>(lbl_eu_80665EA0);
                    *reinterpret_cast<CActParamAnimData3*>(func_8048315C(obj)) = pos;
                }
            }
        }
    }

    reinterpret_cast<CActParamAnimVt18F0*>(view)->dispatchF0();

    if (sel != 0) {
        s32 frameS = static_cast<s32>(blendFrames);
        func_80484164(view->object3A0, resId, param, static_cast<u16>(frameS),
                      view->field36C);
    } else {
        if (func_80055EA0(view->mChildData10) == NULL) view->field4BD = 1;
        if (view->field4BD != 0 || (view->field270 & 0x800) == 0) {
            if (func_80055EBC(view->mChildData10) != 0) {
                // Reset the child data blocks before the fresh anim start.
                func_80054D34(view->mChildData10);
                func_80055DF0(view->mChildData10);
            }
        }
        s32 frameS = static_cast<s32>(blendFrames);
        func_804839D4(view->object3A0, resId, param,
                      static_cast<u16>(frameS),
                      (view->field270 >> 6) & 1,
                      ((view->field4BD != 0) || ((view->field270 & 0x800) == 0)) ? 1 : 0,
                      (sel != 0) ? view->field36C : view->field2EC);
    }

    view->field370 = param;
    f32 speed27 = lbl_eu_80665E9C;
    if ((view->field270 & 0x4) != 0) {
        if ((view->field0C & 0x10000) == 0)
            speed27 = reinterpret_cast<CActParamAnimSubObjView*>(view->object3A0)->field304;
        f32 sp390 = view->field390;
        f32 scale24 = view->field24;
        f32 spf = getSecPerFrame__9CDeviceVIFv();
        f32 f3 = sp390 * spf;
        f32 f1 = lbl_eu_80665EC8;
        f32 f2 = (lbl_eu_80665EC4 * scale24) / f1;
        f2 *= f3;
        speed27 = (view->field448 / f2) / speed27;
    }
    f32 speedVal = (speed27 * view->field388) * view->field38C;
    if ((view->field270 & 0x1000) != 0) speedVal = -speedVal;
    func_80484E5C(view->object3A0, speedVal);

    if (((view->field274 & 0x10) != 0 || (view->field274 & 0x8) != 0) &&
        (view->field270 & 0x10) == 0 && (view->field270 & 0x8) == 0 &&
        blendTime > lbl_eu_80665EA0) {
        f32 t = reinterpret_cast<CActParamAnimVt14*>(view)->f03();
        func_80484F80(view->object3A0, prevBlend * t / blendTime);
    }

    if (view->field370 == (s32)param) {
        f32 spdBase = lbl_eu_80665E9C;
        if ((view->field0C & 0x10000) == 0)
            spdBase = reinterpret_cast<CActParamAnimSubObjView*>(view->object3A0)->field304;
        f32 spd2 = lbl_eu_80665E9C;
        if ((view->field270 & 0x4) != 0) {
            f32 sp390 = view->field390;
            f32 scale24 = view->field24;
            f32 spf = getSecPerFrame__9CDeviceVIFv();
            f32 f3 = sp390 * spf;
            f32 f1 = lbl_eu_80665EC8;
            f32 f2 = (lbl_eu_80665EC4 * scale24) / f1;
            f2 *= f3;
            spd2 = (view->field448 / f2) / spdBase;
        }
        if ((view->field270 & 0x1000) != 0) spd2 = -spd2;
        func_80484E5C(view->object3A0, (spd2 * view->field388) * view->field38C);
    }

    if ((view->field270 & 0x80) != 0) {
        // Wrap the anim ratio difference into [-pi, pi] and derive the
        // normalized progress from the object's own scale virtual.
        f32 diff = view->field440 - view->field444;
        while (!(lbl_eu_8066A1F8 > diff)) diff -= lbl_eu_8066A1FC;
        while (diff < -lbl_eu_8066A1F8) diff += lbl_eu_8066A1FC;
        f32 absDiff = __fabs(diff);
        f32 len = reinterpret_cast<CActParamAnimVt14*>(view)->f03();
        view->field4A0 = absDiff / len;
        view->field4D0 = reinterpret_cast<CActParamAnimVt14*>(view)->f03();
    } else if (lbl_eu_80665EA0 == view->field4D0) {
        view->field4A0 = view->field4A4;
    }

    if (((view->field274 & 0x100000) != 0) && ((view->field270 & 0x200000) == 0))
        reinterpret_cast<CActParamAnimVt18F0*>(view)->dispatch18();

    if ((view->field0C & 0x2000000) != 0) {
        if (view->field26C == NULL) {
            if (view->object3A0 != NULL)
                reinterpret_cast<CActParamAnimObjVt6468*>(view->object3A0)->dispatch64(
                    view->field4E4);
            view->field0C &= ~0x40u;
        } else {
            view->field0C |= 0x2000000;
            if (view->object3A0 != NULL) {
                view->field4E4 =
                    reinterpret_cast<CActParamAnimObjVt6468*>(view->object3A0)->dispatch68();
                reinterpret_cast<CActParamAnimObjVt6468*>(view->object3A0)->dispatch64(
                    view->field26C[8]);
            }
        }
    } else if (view->field26C != NULL) {
        view->field0C |= 0x2000000;
        if (view->object3A0 != NULL) {
            view->field4E4 =
                reinterpret_cast<CActParamAnimObjVt6468*>(view->object3A0)->dispatch68();
            reinterpret_cast<CActParamAnimObjVt6468*>(view->object3A0)->dispatch64(
                view->field26C[8]);
        }
    }

    if (view->field2C >= 0) view->field374 = view->field2C;

    if (((view->field274 & 0x20) != 0 || (view->field270 & 0x20) != 0) &&
        (view->field0C & 0x4000000) == 0) {
        view->field45C = view->field44C;
        view->field0C &= ~0x1000u;
        view->field464 = reinterpret_cast<const f32&>(view->field3AC);
    }

    func_80054980(reinterpret_cast<ActParamWalkHost*>(view->mChildData10));
    view->field4BD = 0;
    view->field4D6 = 0;
    view->field4BE = 0;
    view->field4BF = 0;
    view->field4C0 = 0;
    if (sel == 0) {
        if (func_804978D0(view->object3A0 + 0xC) == 0)
            func_800554DC(view->mChildData10, 0);
    } else {
        if (func_80497914(view->object3A0 + 0xC) == 0)
            func_800554DC(view->mChildData10, 1);
    }
    view->field4C4 = 0;
}

void CActParamAnim::func_8004C5E8() {}

void* CActParamAnim::getModelObj() {
    void* result = *(void**)((char*)this + 0x2fc);
    if (result) return result;
    return *(void**)((char*)this + 0x27c);
}

// Position-delta sources on the model objects returned by func_80496264:
// previous position at +0x10C, current at +0x138.
struct CActParamAnimDeltaSrcA {
    u8 _pad[0x10C];
    f32 x;
    f32 y;
    f32 z;
};
struct CActParamAnimDeltaSrcB {
    u8 _pad[0x138];
    f32 x;
    f32 y;
    f32 z;
};

// Per-frame movement-direction update. Zeroes the anim offset triple
// (+0x3CC/+0x3D0/+0x3D4), blends the heading (+0x440) from either the owner's
// stored value, the sub-object's frame-to-frame position delta, or field444,
// then writes the resulting sin/cos-scaled offsets back into the triple.
// Plateau packet for func_8004C608 (best 344 mismatch / 297 structural /
// 47 reg-swap, decomp 1296B vs retail 1400B, size PASS):
// - Semantics fully reconstructed (heading blend + sin/cos offset triple).
// - Known residual walls (same families as func_8004D2F8/D4AC packets):
//   the "finished" gate appears twice and retail emits fcmpo with the const
//   as frA plus mfcr/srwi materialization that no source form reproduces;
//   retail keeps resX/Y/Z as memory locals copied word-wise at the tail
//   (VEC3 local adopted, closest so far); fabs must be (f32)__fabs((f64)x)
//   intrinsics, never fabsf calls.
// - Tried: direct if-return vs guard-int flag (guard-int WORSE, 302/345),
//   single owner variable with explicit reloads, early noOwner bool so all
//   six zero-stores precede the early-return branch.
// - Next experiments: reorder the two finished-gate copies to share one
//   code shape; try word-copy (u32) assignment of res into field3CC/3D0/3D4.
void func_8004C608(CActParamAnim* self) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    view->field3CC = ml::CVec3::zero.x;
    nw4r::math::VEC3 res;
    res.x = ml::CVec3::zero.x;
    CActParamAnimOwnerIf* owner = view->owner08;
    bool noOwner = owner == NULL;
    view->field3D0 = ml::CVec3::zero.y;
    res.y = ml::CVec3::zero.y;
    res.z = ml::CVec3::zero.z;
    view->field3D4 = ml::CVec3::zero.z;
    if (noOwner) return;
    owner->v1();
    // The 0x274 flag suppresses the whole update unless the 0x270 override
    // bit is also set.
    if ((view->field274 & 0x100000) != 0 && (view->field270 & 0x200000) == 0) return;

    // Heading offset saved before the dispatch below.
    f32 saved = owner->field0C;
    if (owner->v2() != 0) {
        // "Anim finished" gate: skip when the scaled remaining time is spent.
        owner = view->owner08;
        bool finished;
        if (owner == NULL || view->field394 == lbl_eu_80665EA0) {
            finished = true;
        } else {
            finished = (owner->field14 * view->field430) < lbl_eu_80665ECC;
        }
        if (!finished) {
            // Derive the heading from the sub-object's frame-to-frame move.
            u8* obj = view->object3A0;
            void* objA = func_80496264(*(void**)(obj + 4), -1);
            void* objB = func_80496264(*(void**)(obj + 4), -1);
            nw4r::math::VEC3 diff;
            diff.x = reinterpret_cast<CActParamAnimDeltaSrcB*>(objB)->x -
                     reinterpret_cast<CActParamAnimDeltaSrcA*>(objA)->x;
            diff.y = reinterpret_cast<CActParamAnimDeltaSrcB*>(objB)->y -
                     reinterpret_cast<CActParamAnimDeltaSrcA*>(objA)->y;
            diff.z = reinterpret_cast<CActParamAnimDeltaSrcB*>(objB)->z -
                     reinterpret_cast<CActParamAnimDeltaSrcA*>(objA)->z;
            bool xySmall = (f32)__fabs((f64)diff.x) <= lbl_eu_8066A208 &&
                           (f32)__fabs((f64)diff.y) <= lbl_eu_8066A208;
            if (!(xySmall && (f32)__fabs((f64)diff.z) <= lbl_eu_8066A208)) {
                // Horizontal direction only: y is forced to zero and the
                // delta's y feeds dir.z.
                nw4r::math::VEC3 dir;
                dir.x = diff.x;
                dir.z = diff.y;
                dir.y = lbl_eu_80665EA0;
                if (dir.z * dir.z + dir.x * dir.x + dir.y * dir.y == lbl_eu_80665EA0) {
                    dir = *reinterpret_cast<const nw4r::math::VEC3*>(&ml::CVec3::zero);
                } else {
                    PSVECNormalize(dir, dir);
                }
                view->field440 =
                    lbl_eu_80665ED0 * nw4r::math::Atan2FIdx(dir.x, dir.z) - saved;
            }
        }
    } else {
        // Same finished gate; when not finished the owner's saved heading is
        // adopted directly, otherwise fall back to field444 under the flag.
        owner = view->owner08;
        bool finished;
        if (owner == NULL || view->field394 == lbl_eu_80665EA0) {
            finished = true;
        } else {
            finished = (owner->field14 * view->field430) < lbl_eu_80665ECC;
        }
        if (!finished) {
            view->field440 = saved;
        } else {
            if ((view->field0C & 0x400) != 0) {
                view->field440 = view->field444;
            }
        }
    }

    // Clear the one-shot heading-hold flag.
    if ((view->field0C & 0x400) != 0) {
        view->field0C &= ~0x400;
    }
    // Wrap the blended heading into [-pi, pi].
    if (view->field440 > lbl_eu_8066A1F8) view->field440 -= lbl_eu_8066A1FC;
    if (view->field440 < -lbl_eu_8066A1F8) view->field440 += lbl_eu_8066A1FC;
    // Distance from the stored target angle, wrapped to [-pi, pi].
    f32 d = view->field440 - view->field444;
    while (lbl_eu_8066A1F8 <= d) d -= lbl_eu_8066A1FC;
    while (d < -lbl_eu_8066A1F8) d += lbl_eu_8066A1FC;
    f32 m = (f32)__fabs((f64)(d * lbl_eu_8066A20C));
    if (m > lbl_eu_80665ED4) m -= lbl_eu_80665ED4;
    if (m <= lbl_eu_80665E9C) {
        // Close enough to the target: count the guard down.
        s16 g = view->field4D4;
        if (g > 0) view->field4D4 = g - 1;
    } else {
        view->field4D4 = 3;
    }
    owner = view->owner08;
    f32 speed = lbl_eu_80665EA0;
    f32 prod = owner->field14 * view->field430;
    if (prod < lbl_eu_80665ECC) speed = owner->field14 * view->field394;
    f32 sx = lbl_eu_80665ED8 * view->field440;
    f32 spf1 = CDeviceVI::getSecPerFrame();
    f32 sinv = nw4r::math::SinFIdx(sx);
    res.x = speed * sinv * (lbl_eu_80665EC4 * view->field43C / lbl_eu_80665EC8) *
            (view->field390 * spf1);
    f32 cosv = nw4r::math::CosFIdx(sx);
    f32 spf2 = CDeviceVI::getSecPerFrame();
    res.z = speed * cosv * (lbl_eu_80665EC4 * view->field43C / lbl_eu_80665EC8) *
            (view->field390 * spf2);

    if ((view->field270 & 0x100) == 0) return;
    if ((view->field0C & 0x2) != 0) {
        view->field3CC = res.x;
        view->field3D0 = res.y;
        view->field3D4 = res.z;
        return;
    }
    if ((view->field270 & 0x20) == 0) {
        view->field3CC = res.x;
        view->field3D0 = res.y;
        view->field3D4 = res.z;
        return;
    }
    // Curved-turn branch: offsets swing around the turn radius.
    f32 amp2 = speed * (lbl_eu_80665EC4 * view->field43C / lbl_eu_80665EC8);
    view->field3CC = view->field398 * (amp2 * nw4r::math::SinFIdx(lbl_eu_80665ED8 * view->field440));
    view->field3D4 = view->field398 * (amp2 * nw4r::math::CosFIdx(lbl_eu_80665ED8 * view->field440));
    f32 sc = view->field390 * CDeviceVI::getSecPerFrame();
    view->field3CC *= sc;
    view->field3D4 *= sc;
    view->field440 = view->field444;
}

extern "C" __declspec(noinline) void func_8004CB80(f32* out, const f32* a, const f32* b){
    nw4r::math::VEC3 result =
        *reinterpret_cast<const nw4r::math::VEC3*>(a) -
        *reinterpret_cast<const nw4r::math::VEC3*>(b);
    out[0] = result.x;
    out[1] = result.y;
    out[2] = result.z;
}

// extern "C": retail symbol is the unmangled func_8004CBC8.
extern "C" __declspec(noinline) ml::CVec3* func_8004CBC8(ml::CVec3* vec) {
    // Normalize; degenerate (zero-length) vectors are replaced by the zero
    // vector instead (matches the retail lenSq == 0 branch).
    if (vec->x * vec->x + vec->y * vec->y + vec->z * vec->z == lbl_eu_80665EA0) {
        *vec = ml::CVec3::zero;
    } else {
        PSVECNormalize(*vec, *vec);
    }
    return vec;
}

float Atan2FIdx__Q24nw4r4mathFff(float, float);
float CActParamAnim_atan2Scaled(float y, float x) {
    extern float lbl_eu_80665ED0;
    return lbl_eu_80665ED0 * Atan2FIdx__Q24nw4r4mathFff(y, x);
}

extern "C" f32 func_8004CC68(f32 angle) {
    extern f32 SinFIdx__Q24nw4r4mathFf(f32);
    return SinFIdx__Q24nw4r4mathFf(lbl_eu_80665ED8 * angle);
}

extern "C" f32 func_8004CC74(f32 angle) {
    extern f32 CosFIdx__Q24nw4r4mathFf(f32);
    return CosFIdx__Q24nw4r4mathFf(lbl_eu_80665ED8 * angle);
}

int CActParamAnim::checkRenderFlag() { return ((*(unsigned int*)((unsigned char*)this + 0x260) >> 5) & 1); }

void func_8004CC8C(CActParamAnim* self) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if ((view->field270 & 0x8) != 0) {
        f32 v = view->field450 + view->field390;
        view->field44C = lbl_eu_80665EA0;
        view->field450 = v;
        view->field454 = v;
    } else {
        f32 v = view->field44C + view->field390;
        view->field450 = lbl_eu_80665EA0;
        view->field44C = v;
        if (!(v < lbl_eu_80665EE0)) {
            view->field454 = lbl_eu_80665EA0;
        }
    }
    CActParamAnimOwnerIf* owner = view->owner08;
    int t;
    if (owner == 0) {
        t = 1;
    } else if (view->field394 == lbl_eu_80665EA0) {
        t = 1;
    } else {
        t = (owner->field14 * view->field430) < lbl_eu_80665ECC;
    }
    if (t != 0) {
        view->field458 = view->field390 + view->field458;
    } else {
        view->field458 = lbl_eu_80665EA0;
    }
    u8* obj = view->object3A0;
    if (obj != 0) {
        if ((view->field270 & 0x4) != 0) {
            if (view->field390 == lbl_eu_80665EA0) {
                func_80484E5C(obj, lbl_eu_80665EA0);
            } else {
                f32 divisor;
                if ((view->field0C & 0x10000) != 0) {
                    divisor = lbl_eu_80665E9C;
                } else {
                    divisor = reinterpret_cast<CActParamAnimSubObjView*>(obj)->field304;
                }
                f32 animSpeed = view->field390;
                f32 field24 = view->field24;
                f32 field388 = view->field388;
                f32 field38C = view->field38C;
                f32 frameSec = CDeviceVI::getSecPerFrame();
                f32 frameScale = animSpeed * frameSec;
                f32 scaled = lbl_eu_80665EC4 * field24;
                f32 denom = (scaled / lbl_eu_80665EC8) * frameScale;
                f32 value = view->field448 / denom;
                value = field388 * value;
                value = field38C * value;
                value = value / divisor;
                if ((view->field270 & 0x1000) != 0) {
                    value = -value;
                }
                func_80484E5C(obj, value);
            }
        } else {
            if ((view->field270 & 0x1000) != 0) {
                func_80484E5C(obj, -(view->field388 * view->field38C));
            } else {
                func_80484E5C(obj, view->field388 * view->field38C);
            }
        }
    }
}

void CActParamAnim::setAnimCounter(int value) {
    *(int*)((char*)this + 0x374) = value;
}

void func_8004CF00(CActParamAnim* self) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if ((view->field0C & 0x800) != 0) return;
    if (view->object3A0 != 0) {
        // The sub-object's current position is read as ints and re-floated
        // (retail: fctiwz + lbl_eu_80665EE8 magic) before the state sync call.
        f32 v1 = (f32)(s32)func_80485174(view->object3A0);
        f32 v2 = (f32)(s32)func_80484F18(view->object3A0);
        func_80053F7C(reinterpret_cast<CActParamData*>(view->mChildData10), v2, v1);
        f32 rate = view->field388;
        f32 t = func_80496288(*(void**)(view->object3A0 + 4));
        view->field390 = t * rate;
        view->field4B0 = 0;
        if (func_80496288(*(void**)(view->object3A0 + 4)) > lbl_eu_80665EA0 && view->field4D8 < 10) {
            view->field4D8++;
        }
    }
    func_8004C608(self);
    func_80055EE4(view->mChildData10);
    func_80054A3C(view->mChildData10);
    func_80055F08(view->mChildData10);
    func_8004CC8C(self);
    reinterpret_cast<CActParamAnimVtE4*>(self)->dispatchE4();
    if ((view->field0C & 0x80000) != 0 && (view->field270 & 0x20) == 0) {
        // Retail reloads the flag word for the clear (lwz/stw pair) — a volatile
        // access keeps MWCC from reusing the value loaded by the test.
        *(volatile u32*)&view->field0C &= ~0x80000;
    }
    view->field4D6++;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
// Compares the sub-object's progress float against an int threshold widened
// to double (retail xoris/0x4330 magic conversion).
int func_8004D074__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if (param != 0) {
        int result = 1;
        if (func_80485464(view->object3A0, param) == 0) {
            s32 v;
            if (param != 0)
                v = view->field33C;
            else
                v = view->field2BC;
            float progress = func_80485174(view->object3A0);
            // Int threshold widened via the 0x4330 magic cast (retail
            // xoris/lis/fsubs template); the pool cookie it synthesizes maps
            // to lbl_eu_80665EE8 through retail_reloc_map.json.
            if (!(progress >= (f32)v)) result = 0;
        }
        return result;
    }
    int result = 1;
    if (func_80485244(view->object3A0) == 0) {
        s32 v;
        if (param != 0)
            v = view->field33C;
        else
            v = view->field2BC;
        float progress = func_80484F18(view->object3A0);
        if (!(progress >= (f32)v)) result = 0;
    }
    return result;
}

// Retail symbol is Fv but the body reads r4/r5 (two extra args: anim id +
// dispatch arg) — forced-name free function, same scheme as
// func_80053164__13CActParamAnimFv.
int func_8004D194__13CActParamAnimFv(CActParamAnim* self, u32 param, u32 arg2) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localBig;
    u32 localSmall;
    // Dispatch through the vtable slot at +0x80 (retail: lwz r12, 0x80(r12));
    // proceed unless BOTH the virtual call and the child reset return 0.
    if (reinterpret_cast<CActParamAnimVt80*>(self)->dispatch80(arg2) != 0 ||
        func_80054A24(view->mChildData10, 0) != 0) {
        if (view->field30 >= 0) {
            view->field374 = view->field30;
        }
        if (param < 0x68) {
            // The 0x40000 guard flag skips the anim start while the requested
            // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
            if ((view->field0C & 0x40000) == 0 || param != 0x12) {
                if (view->field4BD == 0) {
                    view->field4BD = 1;
                }
                u32 model = view->field2FC;
                u8* data = view->mChildData10;
                u8 b = view->field4BD;
                view->field0C &= ~0x200;
                u32 flag = 0;
                if (model == 0) model = view->field27C;
                if (model == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054170(data, &localSmall, param, b, flag);
                func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                view->field0C &= ~0x200;
            }
        } else {
            u32 ret = func_80054614(view->mChildData10, &localBig, param, 1, 0);
            func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
        return 1;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function. Gate: proceed only when the anim is NOT finished, i.e. owner
// is null, or anim speed (+0x394) is zero, or the scaled remaining time
// (+0x430 * owner +0x14) is still below lbl_eu_80665ECC.
int func_8004D2F8__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    u32 localSmall;
    u32 localBig;

    // Bit 23 of field270 must be set before anything else happens.
    if ((view->field270 & 0x100) == 0) return 0;

    int ok;
    if (owner != 0) {
        if (lbl_eu_80665EA0 == view->field394) {
            ok = 1;
        } else {
            ok = (owner->field14 * view->field430) < lbl_eu_80665ECC;
        }
    } else {
        ok = 1;
    }
    // Inverted vs func_8004D4AC: bail when the "anim finished" condition holds.
    if (ok) return 0;
    // Bit 30 of the owner flags word also aborts.
    if ((owner->field04 & 0x2) != 0) return 0;

    if (param == 0) return 1;
    if (param < 0x68) {
        // The 0x40000 guard flag skips the anim start while the requested
        // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
        if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
            if (view->field4BD == 0) {
                view->field4BD = 0;
            }
            u32 model = view->field2FC;
            u8 b = view->field4BD;
            view->field0C &= ~0x200;
            u32 flag = 0;
            if (model == 0) model = view->field27C;
            if (model == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
            func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
            view->field0C &= ~0x200;
        }
        return 1;
    }
    u32 flag = 0;
    if (view->field278 == 1) {
        u32 m = view->field270;
        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
    }
    u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
    func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
    return 1;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function. Gate: proceed only when the anim IS finished (owner non-null,
// speed non-zero, and scaled remaining time below lbl_eu_80665ECC).
// Open-item packet (best 85 mismatch / 75 structural / 10 reg-swap, decomp
// 432B vs retail 420B, no name/type reloc drift - only presence drift from
// the shifted call sites):
// - Residual 1 (gate): retail emits fcmpo cr0,f0,f1 (const as frA) for the
//   `field394 == lbl_eu_80665EA0` test with the mem load still scheduled
//   first (lfs f1,0x394(r3)); every source form tried emits fcmpo cr0,f1,f0.
//   Tried: natural/reversed operand order x positive/negative branch
//   structure (if/else both polarities), nested ternary chain,
//   short-circuit || assigned to a bool (that one changed size/hoisting and
//   was worse), data-temp hoist in the small path (+16B, worse).
// - Residual 2 (body): the same arg-materialization scheduler wall as
//   func_8004BC94/D194/D950/E500 - retail copies param to r5 early,
//   materializes child-data last (`addi r3,r3,0x10`) with member reloads off
//   r31; our build keeps the view pointer in r3 and parks child-data in r5.
// - Next experiments: none found within high-level C++; likely the shared
//   MWCC float-compare canonicalization + scheduler tie-break walls.
int func_8004D4AC__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    u32 localSmall;
    u32 localBig;

    // "Anim finished" gate: pass when owner is null, speed is zero, or the
    // scaled remaining time is below the threshold.
    int ok;
    if (owner == 0) {
        ok = 1;
    } else {
        if (view->field394 == lbl_eu_80665EA0) {
            ok = 1;
        } else {
            ok = (owner->field14 * view->field430) < lbl_eu_80665ECC;
        }
    }
    if (!ok) return 0;
    // Retail reloads the owner pointer here (second lwz from +0x8).
    if ((view->owner08->field04 & 0x2) != 0) return 0;

    if (param == 0) return 1;
    if (param < 0x68) {
        // The 0x40000 guard flag skips the anim start while the requested
        // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
        if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
            if (view->field4BD == 0) {
                view->field4BD = 0;
            }
            u32 model = view->field2FC;
            u8 b = view->field4BD;
            view->field0C &= ~0x200;
            u32 flag = 0;
            if (model == 0) model = view->field27C;
            if (model == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
            func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
            view->field0C &= ~0x200;
        }
        return 1;
    }
    u32 flag = 0;
    if (view->field278 == 1) {
        u32 m = view->field270;
        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
    }
    u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
    func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
    return 1;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004D650__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    u32 localSmall;
    u32 localBig;
    int ok;
    if (owner != 0) {
        ok = owner->field04 & 1;
    } else {
        ok = 0;
    }
    if (ok) {
        // Proceed unless the anim is busy: either the 0x80000 anim flag is
        // clear (bit 5 of field270) with the 0x2 active flag set, or the anim
        // counter is in the "2" state (retail: bne to the field374 test on bit
        // 5, bne to the body on the 0x2 flag, beq to the body on counter == 2).
        if (((view->field270 & 0x20) == 0 && (view->field0C & 0x2) != 0) ||
            view->field374 == 2) {
            if (param != 0) {
                if (param < 0x68) {
                    // The 0x40000 guard flag skips the anim start while the
                    // requested anim is 0x12 (retail: beq body; cmplwi 0x12;
                    // beq ret1).
                    if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                        if (view->field4BD == 0) {
                            view->field4BD = 0;
                        }
                        u32 model = view->field2FC;
                        u8 b = view->field4BD;
                        view->field0C &= ~0x200;
                        u32 flag = 0;
                        model = (model != 0) ? model : view->field27C;
                        if (model == 1) {
                            u32 m = view->field270;
                            if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                        }
                        u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
                        func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                        view->field0C &= ~0x200;
                    }
                } else {
                    u32 flag = 0;
                    if (view->field278 == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                    func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
                }
            }
            view->field0C &= ~0x80000;
            return 1;
        }
        return 0;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004D7EC__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    u32 localSmall;
    u32 localBig;
    if (owner == 0) return 0;
    if ((owner->field04 & 8) != 0) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u8* data = view->mChildData10;
                    u8 b = view->field4BD;
                    view->field0C &= ~0x200;
                    u32 flag = 0;
                    u32 model = view->field2FC;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(data, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
// Plateau packet (best 30 mismatch / 24 structural / 6 reg-swap, size PASS,
// no reloc drift): semantics fully reconstructed; residual confined to the
// call-arg materialization region of the small path. Same scheduler tie-break
// wall as func_80051448/func_8004BC94: retail copies param to r5 (`or r5,r4,r4`)
// and computes &local early, then materializes child-data LAST (`addi r3,0x10`)
// with member reloads off r31; every build keeps the view pointer live in r3,
// parks child-data in r5 early, and emits a tail `or r3,r5,r5`.
// Ruled out here: inline field4BD at the call site (35/28), explicit
// `u32* out = &localSmall` pointer var (hoists the param copy into the
// prologue region, 80/75 - much worse), dead param alias `animId = param`
// (coalesces, identical body), removing the shadowed inner localSmall decl
// (32/24), model-first without data temp (67/60, size 368), declare-first /
// assign-last data temp (67/60 - breaks the r8 model alloc; first ASSIGNMENT,
// not just declaration order, is load-bearing), ternary model fallback placed
// after the b load (35/28 - shifts the flags load after lbz r6).
// Root cause per reg_mapping: retail consumes r3 as child-data late
// (`addi r3,r3,0x10`), forcing post-setup member reloads onto r31; our build
// parks child-data in r5 so the view pointer stays live in r3.
// Load-bearing shape: data-temp first assignment (fixes r8 model alloc),
// shadowed inner localSmall decl, b temp loaded before the flags clear.
int func_8004D950__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    CActParamAnimOwnerIf* owner = view->owner08;
    if (owner == 0) return 0;
    if ((owner->field04 & 2) != 0) {
        view->field4C8 = owner->field24;
        if (param != 0) {
        if (param < 0x68) {
            // The 0x40000 guard flag skips the anim start while the requested
            // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
            if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                if (view->field4BD == 0) {
                    view->field4BD = 0;
                }
                u8* data = view->mChildData10;
                u32 model = view->field2FC;
                u32 localSmall;
                u32 b = view->field4BD;
                view->field0C &= ~0x200;
                u32 flag = 0;
                if (model == 0) model = view->field27C;
                if (model == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054170(data, &localSmall, param, b, flag);
                func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                view->field0C &= ~0x200;
            }
        } else {
            u32 flag = 0;
            if (view->field278 == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
            func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
        view->field4C8 = -1;
        return 1;
    }
    return 0;
}
}

void* CActParamAnim::getEffObj() {
    return *(void**)((char*)this + 0x4c0);
}

void CActParamAnim::clearEffObj() {
    CActParamAnimFull* full = reinterpret_cast<CActParamAnimFull*>(this);
    full->mEffByte0 = 0;
    full->mEffByte1 = 0;
    *(int *)((char *)this + 0x4c0) = 0;
}


extern "C" void* func_8004B344(CActParamAnim* self) {
    return reinterpret_cast<u8*>(self) + 16;
}

extern "C" f32 func_8004B34C(CActParamAnim* self) {
    return *(f32*)(reinterpret_cast<u8*>(self) + 0x43C);
}

extern "C" void* func_8004B51C(CActParamAnim* self) {
    return *(void**)(reinterpret_cast<u8*>(self) + 8);
}

extern "C" u32 func_8004B524() {
    return 1;
}

extern "C" void func_8004B730(CActParamAnim* self, void* val) {
    *(void**)(reinterpret_cast<u8*>(self) + 8) = val;
}

extern "C" void func_8004B840(CActParamAnim* self, f32 val) {
    *(f32*)(reinterpret_cast<u8*>(self) + 0x3C4) = val;
}

extern "C" void func_8004B5F0(void* dstObj, const f32* srcVec) {
    u8* base = reinterpret_cast<u8*>(dstObj);
    *(u32*)(base + 12) = *(const u32*)&srcVec[0];
    *(u32*)(base + 16) = *(const u32*)&srcVec[1];
    *(u32*)(base + 20) = *(const u32*)&srcVec[2];
}

extern "C" void func_8004B60C(void* out, f32 a, f32 b, f32 c) {
    *(f32*)(reinterpret_cast<u8*>(out) + 0) = a;
    *(f32*)(reinterpret_cast<u8*>(out) + 4) = b;
    *(f32*)(reinterpret_cast<u8*>(out) + 8) = c;
}

extern "C" void func_8004B694(u32* flags, u32 mask) {
    *flags &= ~mask;
}

extern "C" void func_8004B79C(f32* out, const f32* src) {
    out[0] = src[0];
    out[1] = src[1];
    out[2] = src[2];
}

extern "C" void func_8004B7C0(CActParamAnim* self, const u32* srcVec) {
    u8* base = reinterpret_cast<u8*>(self);
    *(u32*)(base + 0x3C0) = srcVec[0];
    *(u32*)(base + 0x3C4) = srcVec[1];
    *(u32*)(base + 0x3C8) = srcVec[2];
}

extern "C" int func_8004B3D8(u32* flags, u32 mask) {
    u32 tmp = *flags & mask;
    return ((-(u32)tmp | tmp) >> 31) & 1;
}

extern "C" void* func_8004B9B8(CActParamAnim* self) {
    void* ptr = *(void**)(reinterpret_cast<u8*>(self) + 8);
    if (ptr != NULL) {
        return *(void**)(reinterpret_cast<u8*>(ptr) + 24);
    }
    return *(void**)(reinterpret_cast<u8*>(self) + 0x4B4);
}

void CActParamAnim::func_8004DAE0() {}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004DDD0__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    int ok;
    if (owner == 0) {
        ok = 0;
    } else {
        int t;
        if (owner == 0) {
            t = 1;
        } else if (lbl_eu_80665EA0 == view->field394) {
            t = 1;
        } else {
            t = (owner->field14 * view->field430) < lbl_eu_80665ECC;
        }
        ok = 0;
        if (t == 0 && (owner->field04 & 0x2000) != 0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        if ((view->owner08->field04 & 2) == 0) {
            if (param == 0) return 1;
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 local;
                    u32 model = view->field2FC;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u8 b = view->field4BD;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(view->mChildData10, &local, param, b, flag);
                    func_8004BDCC(self, local, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                u32 local;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
                func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
            return 1;
        }
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv. Mirror of
// func_8004DDD0 with the owner flag mask 0x800.
int func_8004DF9C__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    int ok;
    if (owner == 0) {
        ok = 0;
    } else {
        int t;
        if (view->field394 == lbl_eu_80665EA0) {
            t = 1;
        } else {
            t = (owner->field14 * view->field430) < lbl_eu_80665ECC;
        }
        ok = 0;
        if (t == 0 && (owner->field04 & 0x800) != 0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        owner = view->owner08;
        if ((owner->field04 & 2) == 0) {
            if (param == 0) return 1;
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 local;
                    u32 model = view->field2FC;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u8 b = view->field4BD;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(view->mChildData10, &local, param, b, flag);
                    func_8004BDCC(self, local, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                u32 local;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
                func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
            return 1;
        }
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv. Mirror of
// func_8004DDD0 with the owner flag mask 0x400.
int func_8004E168__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    int ok;
    if (owner == 0) {
        ok = 0;
    } else {
        int t;
        if (view->field394 == lbl_eu_80665EA0) {
            t = 1;
        } else {
            t = (owner->field14 * view->field430) < lbl_eu_80665ECC;
        }
        ok = 0;
        if (t == 0 && (owner->field04 & 0x400) != 0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        owner = view->owner08;
        if ((owner->field04 & 2) == 0) {
            if (param == 0) return 1;
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 local;
                    u32 model = view->field2FC;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u8 b = view->field4BD;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(view->mChildData10, &local, param, b, flag);
                    func_8004BDCC(self, local, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                u32 local;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
                func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
            return 1;
        }
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv. Mirror of
// func_8004DDD0 with the owner flag mask 0x1000.
int func_8004E334__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    int ok;
    if (owner == 0) {
        ok = 0;
    } else {
        int t;
        if (view->field394 == lbl_eu_80665EA0) {
            t = 1;
        } else {
            t = (owner->field14 * view->field430) < lbl_eu_80665ECC;
        }
        ok = 0;
        if (t == 0 && (owner->field04 & 0x1000) != 0) {
            ok = 1;
        }
    }
    if (ok != 0) {
        owner = view->owner08;
        if ((owner->field04 & 2) == 0) {
            if (param == 0) return 1;
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 local;
                    u32 model = view->field2FC;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u8 b = view->field4BD;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(view->mChildData10, &local, param, b, flag);
                    func_8004BDCC(self, local, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                u32 local;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
                func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
            return 1;
        }
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
// Plateau packet (best 35 mismatch / 28 structural / 7 reg-swap, size PASS
// 404B=404B, no reloc drift): residual is the small-path arg-
// materialization scheduler wall from the func_8004FCE0 packet - retail
// parks the param copy (or r5,r4) and model temp in r5..r8 ahead of the
// flags clear-store; every build instead keeps self in r3 through the model
// chain, reuses r5 for field2FC/27C/270, and defers li r7/addi r3 until
// after the branches. Newly ruled out here: if-form model select without
// ternary (65/58), hoisted u8* child local (62/51, +4 size), explicit
// p = param copy local (coalesces at every tried position - function top,
// branch top, after model load - identical output), ternary moved before
// the field4BD load / flags clear (identical 35/28).
int func_8004E500__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    u32 localSmall;
    u32 localBig;
    int ok;
    if (owner == 0) {
        ok = 0;
    } else {
        f32 animSpeed = view->field394;
        if (lbl_eu_80665EA0 == animSpeed) {
            ok = 0;
        } else {
            // Gate: fail when the scaled owner value exceeds the anim upper bound.
            ok = owner->field14 * view->field430 > view->field434;
        }
    }
    if (!ok) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    u32 p = param;
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 model = view->field2FC;
                    model = (model != 0) ? model : view->field27C;
                    u8 b = view->field4BD;
                    view->field0C &= ~0x200;
                    u32 flag = 0;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) {
                            flag = 1;
                        }
                    }
                    u32 ret = func_80054170(view->mChildData10, &localSmall, p, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004E694__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    u32 localSmall;
    u32 localBig;
    int ok;
    if (owner == 0) {
        ok = 0;
    } else {
        f32 animSpeed = view->field394;
        if (lbl_eu_80665EA0 == animSpeed) {
            ok = 0;
        } else {
            ok = owner->field14 * view->field430 > view->field434;
        }
    }
    if (ok) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 model = view->field2FC;
                    u8 b = view->field4BD;
                    view->field0C &= ~0x200;
                    u32 flag = 0;
                    model = (model != 0) ? model : view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function. Gate chain: blend accumulator (+0x44C) below the threshold,
// +0x454 above lbl_eu_80665EF0, owner present, speed non-zero, and scaled
// remaining time above +0x434.
int func_8004E828__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner = view->owner08;
    u32 localSmall;
    u32 localBig;

    if (!(view->field44C < lbl_eu_80665EE0)) return 0;
    if (!(view->field454 > lbl_eu_80665EF0)) return 0;
    if (owner == 0) return 0;
    if (view->field394 == lbl_eu_80665EA0) return 0;
    if (!((owner->field14 * view->field430) > view->field434)) return 0;

    if (param == 0) return 1;
    if (param < 0x68) {
        // The 0x40000 guard flag skips the anim start while the requested
        // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
        if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
            if (view->field4BD == 0) {
                view->field4BD = 0;
            }
            u32 model = view->field2FC;
            u8 b = view->field4BD;
            view->field0C &= ~0x200;
            u32 flag = 0;
            if (model == 0) model = view->field27C;
            if (model == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
            func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
            view->field0C &= ~0x200;
        }
        return 1;
    }
    u32 flag = 0;
    if (view->field278 == 1) {
        u32 m = view->field270;
        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
    }
    u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
    func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
    return 1;
}

void func_8004E9EC__13CActParamAnimFv(CActParamAnim* self) {
    // TODO: reconstruct body (retail 0x26c bytes)
}

// noinline: retail calls this from func_80051CD4; inlined copies would
// duplicate the FSqrt warning block into the caller.
// extern "C": retail symbol is the unmangled func_8004EC78.
extern "C" __declspec(noinline) f32 func_8004EC78(f32 value) {
    // nw4r FSqrt semantics: warn on negative input, then x<=0 ? 0 : x*FrSqrt(x).
    if (!(value >= lbl_eu_80665EA0)) {
        nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                          (const char*)lbl_eu_80526300);
    }
    return (value <= lbl_eu_80665EA0) ? lbl_eu_80665EA0
                                      : value * nw4r::math::FrSqrt(value);
}

void CActParamAnim::func_8004ECF4() const {}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv. Per-frame
// anim update: gates on the active/playing flags, starts the requested anim
// (param) through the data dispatcher, then advances the anim axis state and
// writes the per-frame delta to +0x48C.
int func_8004ECF4__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimInitView* view = reinterpret_cast<CActParamAnimInitView*>(self);
    CActParamAnimStateView* sview = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 flags = sview->field0C;
    if ((flags & 0x40) != 0) return 0;
    if ((flags & 0x2) != 0) return 0;
    if (sview->field4D8 <= 2) return 0;

    // Movement gate: horizontal anim delta above threshold, or the stopped
    // fallback (direction below gate, clamped Y, and last value outside band).
    f32 v1 = view->field3AC - view->field47C;
    int gate = v1 >= lbl_eu_80665EF8;
    f32 v2 = view->field488 - view->field3AC;
    int stop = 0;
    int cond = 0;
    if (view->field3DC < lbl_eu_80663D48 && view->field3C4 <= lbl_eu_80665EFC) {
        cond = 1;
    }
    if (cond != 0) {
        cond = 0;
        if (v2 <= lbl_eu_80665EA0 || v2 >= lbl_eu_80665F00) {
            cond = 1;
        }
        if (cond != 0) {
            stop = 1;
        }
    }
    if (gate == 0 && stop == 0) return 0;

    u32 localSmall;
    u32 localBig;
    if (param != 0) {
        if (param < 0x68) {
            // The 0x40000 guard flag skips the anim start while the requested
            // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
            if (!((sview->field0C & 0x40000) != 0 && param == 0x12)) {
                if (sview->field4BD == 0) {
                    sview->field4BD = 0;
                }
                sview->field0C &= ~0x200;
                u32 model = sview->field2FC;
                if (model == 0) model = sview->field27C;
                u32 flag = 0;
                if (model == 1 && (sview->field270 & 0x10) == 0) {
                    flag = 1;
                }
                u32 flag2 = 0;
                if (flag != 0 && (sview->field270 & 0x8) == 0) {
                    flag2 = 1;
                }
                u8 b = sview->field4BD;
                u32 ret = func_80054170(sview->mChildData10, &localSmall,
                                        param, b, flag2);
                func_8004BDCC(self, localSmall, ret, sview->field2A4, 0);
                sview->field0C &= ~0x200;
            }
        } else {
            u32 flag = 0;
            if (sview->field278 == 1 && (sview->field270 & 0x10) == 0) {
                flag = 1;
            }
            u32 flag2 = 0;
            if (flag != 0 && (sview->field270 & 0x8) == 0) {
                flag2 = 1;
            }
            u32 ret = func_80054614(sview->mChildData10, &localBig, param,
                                    flag2, 0);
            bool b = (sview->field0C & 0x200) != 0;
            func_8004BDCC(self, localBig, ret, sview->field2A4, b);
        }
    }

    // Advance the anim axis: rotate the (0,0,z) vector by the Y-rotation
    // matrix, scale by speed*dt, fold into +0x3C0..+0x3C8 and the stored
    // offset +0x3F0..+0x3F8, then clamp the resulting length.
    ml::CVec3 v = ml::CVec3::zero;
    f32 f31 = lbl_eu_80665F04;
    if (view->field3C4 < lbl_eu_80665F08) {
        f31 = view->field3C4;
    }
    if (view->field45C < lbl_eu_80665EF0) {
        v.z = (lbl_eu_80665EF0 - view->field45C) *
              (lbl_eu_80665EC4 * view->field43C / lbl_eu_80665EC8) /
              lbl_eu_80665EF0;
    }
    f32 ang = view->field444;
    f32 s = nw4r::math::SinFIdx(lbl_eu_80665ED8 * ang);
    f32 c = nw4r::math::CosFIdx(lbl_eu_80665ED8 * ang);
    f32 m[3][3] = {
        {c, lbl_eu_80665EA0, s},
        {lbl_eu_80665EA0, lbl_eu_80665E9C, lbl_eu_80665EA0},
        {-s, lbl_eu_80665EA0, c},
    };
    ml::CVec3 r;
    r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
    f32 scale = view->field390 * CDeviceVI::getSecPerFrame();
    ml::CVec3 d = r * scale;
    view->field3C0 += d.x;
    view->field3C4 += d.y;
    view->field3C8 += d.z;
    view->field3C4 = f31;
    view->field3C0 += view->field3F0;
    view->field3C4 += view->field3F4;
    view->field3C8 += view->field3F8;
    f32 lenSq = view->field3C0 * view->field3C0 + view->field3C8 * view->field3C8;
    if (!(lenSq >= lbl_eu_80665EA0)) {
        nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                          (const char*)lbl_eu_80526300);
    }
    f32 len = lbl_eu_80665EA0;
    if (lenSq > lbl_eu_80665EA0) {
        len = lenSq * nw4r::math::FrSqrt(lenSq);
    }
    f32 limit = lbl_eu_80665EDC *
                (lbl_eu_80665EC4 * view->field43C / lbl_eu_80665EC8);
    if (len < limit) {
        len = limit;
    }
    view->field48C = len * (view->field390 * CDeviceVI::getSecPerFrame());
    view->field498 = (len <= lbl_eu_80665F0C *
                              (lbl_eu_80665EC4 * view->field43C /
                               lbl_eu_80665EC8));
    view->field0C |= 0x80000;
    *reinterpret_cast<ml::CVec3*>(&view->field3F0) = ml::CVec3::zero;
    return 1;
}

// Retail symbol is Fv but the body reads r4/r5 (two extra args) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004F1E4__13CActParamAnimFv(CActParamAnim* self, u32 param, s32 val) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localBig;
    u32 localSmall;
    if (view->field374 != val) {
        return 0;
    }
    if (param == 0) {
        return 1;
    }
    if (param < 0x68) {
        // Skip the anim start while the 0x40000 guard flag is set and the
        // requested anim is 0x12 (same guard as the sibling helpers).
        if ((view->field0C & 0x40000) != 0 && param == 0x12) {
            return 1;
        }
        if (view->field4BD == 0) {
            view->field4BD = 0;
        }
        u32 model = view->field2FC;
        view->field0C &= ~0x200;
        u8 b = view->field4BD;
        u32 flag = 0;
        if (model == 0) {
            model = view->field27C;
        }
        if (model == 1) {
            u32 m = view->field270;
            if ((m & 0x10) == 0 && (m & 0x8) == 0) {
                flag = 1;
            }
        }
        u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
        func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
        view->field0C &= ~0x200;
    } else {
        u32 flag = 0;
        if (view->field278 == 1) {
            u32 m = view->field270;
            if ((m & 0x10) == 0 && (m & 0x8) == 0) {
                flag = 1;
            }
        }
        u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
        func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
    }
    return 1;
}

// Retail symbol is Fv but the body reads r4/r5 (two extra args) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004F334__13CActParamAnimFv(CActParamAnim* self, u32 param, s32 val) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localBig;
    u32 localSmall;
    if (view->field378 == val) {
        if (param != 0) {
            if (param < 0x68) {
                // Skip the anim start while the 0x40000 guard flag is set and the
                // requested anim is 0x12 (same guard as the sibling helpers).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u8* data = view->mChildData10;
                    u8 b = view->field4BD;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    // Single variable for the whole model chain so MWCC keeps
                    // it in one register like retail.
                    u32 model = view->field2FC;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        model = view->field270;
                        if ((model & 0x10) == 0 && (model & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(data, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 sel = view->field278;
                u32 flag = 0;
                if (sel == 1) {
                    sel = view->field270;
                    if ((sel & 0x10) == 0 && (sel & 0x8) == 0) flag = 1;
                }
                u32 ret =
                    func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4/r5 (two extra args) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004F484__13CActParamAnimFv(CActParamAnim* self, u32 param, s32 val) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    s32 f37C = view->field37C;
    if (f37C < 0 && view->field4C8 < 0) return 0;
    if (f37C == val || view->field4C8 == val) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the requested
                // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 model = view->field2FC;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    model = (model != 0) ? model : view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(view->mChildData10, &localSmall, param, view->field4BD, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

void CActParamAnim::func_8004F5FC() {}

void CActParamAnim::func_8004F884() {}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
void func_8004FAB4__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    view->field384 = lbl_eu_80665E9C;
    if (param != 0) {
        if (param < 0x68) {
            // The 0x40000 guard flag skips the anim start while the requested
            // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
            if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                if (view->field4BD == 0) {
                    view->field4BD = 0;
                }
                u32 local;
                u32 model = view->field2FC;
                u32 flag = 0;
                view->field0C &= ~0x200;
                u8 b = view->field4BD;
                if (model == 0) model = view->field27C;
                if (model == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054170(view->mChildData10, &local, param, b, flag);
                func_8004BDCC(self, local, ret, view->field2A4, 0);
                view->field0C &= ~0x200;
            }
        } else {
            u32 flag = 0;
            u32 local;
            if (view->field278 == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
            func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
    }
    // Y-rotation of the anim axis: build the rotation matrix, transform the
    // (0, field474, field470) vector, and store the result as words.
    ml::CVec3 v = ml::CVec3::zero;
    v.y = view->field474;
    v.z = view->field470;
    f32 ang = view->field478;
    f32 s = nw4r::math::SinFIdx(lbl_eu_80665ED8 * ang);
    f32 c = nw4r::math::CosFIdx(lbl_eu_80665ED8 * ang);
    f32 m[3][3] = {
        {c, lbl_eu_80665EA0, s},
        {lbl_eu_80665EA0, lbl_eu_80665E9C, lbl_eu_80665EA0},
        {-s, lbl_eu_80665EA0, c},
    };
    ml::CVec3 r;
    r.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    r.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    r.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
    CActParamAnimPosView* pos = reinterpret_cast<CActParamAnimPosView*>(self);
    pos->x = *reinterpret_cast<u32*>(&r.x);
    pos->y = *reinterpret_cast<u32*>(&r.y);
    pos->z = *reinterpret_cast<u32*>(&r.z);
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
// Plateau packet (best 51 mismatch / 29 structural / 22 reg-swap, size PASS
// 376=376, no reloc drift): residual is the same small-path arg-
// materialization scheduler wall as func_8004BC94/51448/51584 — retail parks
// self in r31/param in r30 and interleaves the arg setup (or r5,r30 / addi
// r3,r31,0x10 / lbz r6 / addi r4 / li r7) around the model==1 branch; every
// build swaps the r30/r31 assignment and defers data/&local until after the
// branches. Newly ruled out here: s32 param (flips cmplwi 0x68 to cmpi,
// 76/59), if-form model chain without ternary (372B, -4 size), varargs
// (.../va_list — full FP save area + __va_arg reloc, 118/106), dead animId
// copy-local (coalesces, identical), moving localSmall/localBig into their
// branches (identical), volatile flag clears (canonicalized, identical),
// reference view alias instead of pointer (identical), nested-call form
// (func_80054170 inlined into the BDCC arg list, canonicalized, identical).
int func_8004FCE0__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    // Both gates share the out-of-line return-0 block (retail: beq/ble to
    // .L_80050474).
    if ((view->field0C & 2) != 0) {
        if (reinterpret_cast<CActParamAnimVt0C*>(view)->f01() > lbl_eu_80665EF0) {
            if (param != 0) {
                if (param < 0x68) {
                    // The 0x40000 guard flag skips the anim start while the requested
                    // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
                    if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                        if (view->field4BD == 0) {
                            view->field4BD = 0;
                        }
                        u32 model = view->field2FC;
                        u32 flag = 0;
                        view->field0C &= ~0x200;
                        model = (model != 0) ? model : view->field27C;
                        if (model == 1) {
                            u32 m = view->field270;
                            if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                        }
                        u32 localSmall;
                        u32 ret = func_80054170(view->mChildData10, &localSmall, param, view->field4BD, flag);
                        func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                        view->field0C &= ~0x200;
                    }
                } else {
                    u32 flag = 0;
                    if (view->field278 == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 localBig;
                    u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                    func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
                }
            }
            return 1;
        }
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004FE58__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    view->field384 = lbl_eu_80665EF0;
    if (param != 0) {
        if (param < 0x68) {
            // The 0x40000 guard flag skips the anim start while the requested
            // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
            if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                if (view->field4BD == 0) {
                    view->field4BD = 0;
                }
                u32 model = view->field2FC;
                u32 flag = 0;
                view->field0C &= ~0x200;
                model = (model != 0) ? model : view->field27C;
                if (model == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054170(view->mChildData10, &localSmall, param, view->field4BD, flag);
                func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                view->field0C &= ~0x200;
            }
        } else {
            u32 flag = 0;
            if (view->field278 == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
            func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
    }
    if ((view->field0C & 0x4000000) == 0) {
        reinterpret_cast<CActParamAnimVtEC*>(self)->dispatchEC();
    }
    return 1;
}

// Retail symbol is Fv (no params) — forced-name free function, same scheme as
// func_80053164__13CActParamAnimFv. Per-frame anim-axis update: builds the
// Y-rotation matrix from the anim angle, optionally aligns the anim quaternion
// (+0x414) through func_80053490 and pulls the running value toward +0x460,
// then computes the frame delta (+0x48C) from the rotated axis and the speed.
void func_8004FFBC__13CActParamAnimFv(CActParamAnim* self) {
    CActParamAnimInitView* view = reinterpret_cast<CActParamAnimInitView*>(self);
    CActParamAnimStateView* sview = reinterpret_cast<CActParamAnimStateView*>(self);

    f32 f29 = view->field440;
    f32 f31 = view->field460;
    ml::CVec3 v0 = ml::CVec3::zero;
    f32 s = nw4r::math::SinFIdx(lbl_eu_80665ED8 * f29);
    f32 c = nw4r::math::CosFIdx(lbl_eu_80665ED8 * f29);
    f32 m[3][3] = {
        {c, lbl_eu_80665EA0, s},
        {lbl_eu_80665EA0, lbl_eu_80665E9C, lbl_eu_80665EA0},
        {-s, lbl_eu_80665EA0, c},
    };

    if ((sview->field0C & 0x400000) != 0) {
        if (func_80053490(self,
                          reinterpret_cast<ml::CVec3*>(&view->field3D8)) != 0) {
            CActParamAnimOwnerIf* owner = sview->owner08;
            int r0;
            if (owner == 0) {
                r0 = 0;
            } else if (view->field394 == lbl_eu_80665EA0) {
                r0 = 0;
            } else {
                r0 = owner->field14 * view->field430 > view->field434;
            }
            if (r0 != 0) {
                ml::CVec3 r;
                r.x = m[0][0] * ml::CVec3::unitZ.x + m[0][1] * ml::CVec3::unitZ.y +
                      m[0][2] * ml::CVec3::unitZ.z;
                r.y = m[1][0] * ml::CVec3::unitZ.x + m[1][1] * ml::CVec3::unitZ.y +
                      m[1][2] * ml::CVec3::unitZ.z;
                r.z = m[2][0] * ml::CVec3::unitZ.x + m[2][1] * ml::CVec3::unitZ.y +
                      m[2][2] * ml::CVec3::unitZ.z;
                f32 dot = ml::CVec3::dot(
                    *reinterpret_cast<ml::CVec3*>(&view->field424), r);
                if (dot < lbl_eu_80665EA0) {
                    f32 t = lbl_eu_80665E9C -
                            (lbl_eu_80665F10 - view->field4CC) /
                                lbl_eu_80665F14;
                    if (t < lbl_eu_80665EA0) {
                        t = lbl_eu_80665EA0;
                    } else if (t > lbl_eu_80665E9C) {
                        t = lbl_eu_80665E9C;
                    }
                    f31 = view->field460 +
                          lbl_eu_80665F18 * (view->field460 * dot * t);
                }
            }
        }
    }

    if ((sview->field0C & 0x8) != 0) {
        f32 len = f31 * (lbl_eu_80665EB4 * view->field380);
        if (!(len >= lbl_eu_80665EA0)) {
            nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                              (const char*)lbl_eu_80526300);
        }
        if (len > lbl_eu_80665EA0) {
            v0.y = len * nw4r::math::FrSqrt(len);
        } else {
            v0.y = lbl_eu_80665EA0;
        }
    } else {
        f31 = view->field380 * reinterpret_cast<CActParamAnimVt14*>(self)->f03();
        v0.y = f31 * CDeviceVI::getSecPerFrame() * lbl_eu_80665F00;
    }

    if (view->field45C < lbl_eu_80665EF0) {
        if ((sview->field0C & 0x2) != 0) {
            CActParamAnimOwnerIf* owner = sview->owner08;
            int r0;
            if (owner == 0) {
                r0 = 0;
            } else if (view->field394 == lbl_eu_80665EA0) {
                r0 = 0;
            } else {
                r0 = owner->field14 * view->field430 > view->field434;
            }
            if (r0 != 0) {
                v0.z = lbl_eu_80665EC4 * view->field43C / lbl_eu_80665EC8;
            } else {
                v0.z = (lbl_eu_80665EF0 - view->field45C) *
                       (lbl_eu_80665EC4 * view->field43C / lbl_eu_80665EC8) /
                       lbl_eu_80665EF0;
            }
            v0.z *= view->field394;
        }
    }

    ml::CVec3 r;
    r.x = m[0][0] * v0.x + m[0][1] * v0.y + m[0][2] * v0.z;
    r.y = m[1][0] * v0.x + m[1][1] * v0.y + m[1][2] * v0.z;
    r.z = m[2][0] * v0.x + m[2][1] * v0.y + m[2][2] * v0.z;
    *reinterpret_cast<ml::CVec3*>(&view->field3C0) = r;
    view->field3C0 += view->field3F0;
    view->field3C4 += view->field3F4;
    view->field3C8 += view->field3F8;
    *reinterpret_cast<ml::CVec3*>(&view->field3F0) = ml::CVec3::zero;
    f32 lenSq = view->field3C0 * view->field3C0 +
                view->field3C8 * view->field3C8;
    if (!(lenSq >= lbl_eu_80665EA0)) {
        nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                          (const char*)lbl_eu_80526300);
    }
    f32 len = lbl_eu_80665EA0;
    if (lenSq > lbl_eu_80665EA0) {
        len = lenSq * nw4r::math::FrSqrt(lenSq);
    }
    f32 limit = lbl_eu_80665EDC *
                (lbl_eu_80665EC4 * view->field43C / lbl_eu_80665EC8);
    if (len < limit) {
        len = limit;
    }
    view->field48C = len * (view->field390 * CDeviceVI::getSecPerFrame());
    view->field498 = (len <= lbl_eu_80665F0C *
                              (lbl_eu_80665EC4 * view->field43C /
                               lbl_eu_80665EC8));
}

extern "C" __declspec(noinline) f32 func_800504BC(const f32* a, const f32* b){
    return nw4r::math::VEC3Dot(
        reinterpret_cast<const nw4r::math::VEC3*>(a),
        reinterpret_cast<const nw4r::math::VEC3*>(b));
}

void func_800504DC(CActParamAnim* self) {}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_80050744__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localBig;
    u32 localSmall;
    if ((view->field0C & 2) != 0) {
        if (param != 0) {
            if (param < 0x68) {
                // Skip the anim start while the 0x40000 guard flag is set and the
                // requested anim is 0x12 (same guard as the sibling helpers).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u8* data = view->mChildData10;
                    u8 b = view->field4BD;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u32 model = view->field2FC;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(data, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// Anim-start request dispatcher variant (forced-name free function): compares
// val against our own eff timer (+0x4C0) when armed, else against the owner
// object's +0x18 timer (or fallback timer +0x4B4 when no owner). On a match
// latches val into +0x4C4 (only on the +0x4C0 half), then runs the same
// small (<0x68, func_80054170) / big (func_80054614) split as func_8004F5FC.
// Each half shares one pass/fail exit pair placed between halves (as retail
// emits them), hence the gotos.
#define ACT_PARAM_ANIM_START_SMALL(outLocal, passLabel)                         \
    do {                                                                        \
        if ((view->field0C & 0x40000) != 0 && param == 0x12) {                  \
            goto passLabel;                                                     \
        }                                                                       \
        if (view->field4BD == 0) {                                              \
            view->field4BD = 0;                                                 \
        }                                                                       \
        flag = 0;                                                               \
        model = view->field2FC;                                                 \
        b = view->field4BD;                                                     \
        view->field0C &= ~0x200u;                                               \
        model = (model != 0) ? model : view->field27C;                          \
        if (model == 1) {                                                       \
            u32 m = view->field270;                                             \
            if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;                    \
        }                                                                       \
        u32 startRet =                                                          \
            func_80054170(view->mChildData10, &outLocal, param, b, flag);        \
        func_8004BDCC(self, outLocal, startRet, view->field2A4, 0);              \
        view->field0C &= ~0x200u;                                               \
    } while (0)

#define ACT_PARAM_ANIM_START_BIG(outLocal)                                      \
    do {                                                                        \
        flag = 0;                                                               \
        if (view->field278 == 1) {                                              \
            u32 m = view->field270;                                             \
            if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;                    \
        }                                                                       \
        u32 ret = func_80054614(view->mChildData10, &outLocal, param, flag, 0); \
        func_8004BDCC(self, outLocal, ret, view->field2A4,                      \
                      (view->field0C >> 9) & 1);                                \
    } while (0)

int func_80050890__13CActParamAnimFv(CActParamAnim* self, u32 param, s32 val) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    // Shared scratch across the three halves.
    u32 model;
    u8 b;
    u32 flag;
    if (view->field4C0 != 0) {
        if (static_cast<s32>(view->field4C0) != val) {
            goto failSelf;
        }
        view->field4C4 = val;
        if (param == 0) {
            goto passSelf;
        }
        if (param < 0x68) {
            // The 0x40000 guard flag skips the anim start while the requested
            // anim is 0x12 (same guard as the sibling helpers).
            u32 localSmall;
            ACT_PARAM_ANIM_START_SMALL(localSmall, passSelf);
        } else {
            u32 localBig;
            ACT_PARAM_ANIM_START_BIG(localBig);
        }
        goto passSelf;
passSelf:
    return 1;
failSelf:
    return 0;
    }
    if (view->owner08 == 0) {
        // No owner attached: match against the eff-timer fallback.
        if (static_cast<s32>(view->field4B4) != val) {
            goto failFallback;
        }
        if (param == 0) {
            goto passFallback;
        }
        if (param < 0x68) {
            u32 localSmall;
            ACT_PARAM_ANIM_START_SMALL(localSmall, passFallback);
        } else {
            u32 localBig;
            ACT_PARAM_ANIM_START_BIG(localBig);
        }
        goto passFallback;
passFallback:
    return 1;
failFallback:
    return 0;
    } else {
        // Owner object attached: match against its +0x18 timer.
        // (Operand order reversed vs the other halves - retail emits cmpw r5, r0.)
        if (val != static_cast<s32>(view->owner08->field18)) {
            goto failOwner;
        }
        if (param == 0) {
            goto passOwner;
        }
        if (param < 0x68) {
            u32 localSmall;
            ACT_PARAM_ANIM_START_SMALL(localSmall, passOwner);
        } else {
            u32 localBig;
            ACT_PARAM_ANIM_START_BIG(localBig);
        }
        goto passOwner;
    }
passOwner:
    return 1;
failOwner:
    return 0;
#undef ACT_PARAM_ANIM_START_SMALL
#undef ACT_PARAM_ANIM_START_BIG
}

// Retail symbol is Fv but the body reads r4/r5 (anim id + rng arg) —
// forced-name free function, same scheme as func_80053164__13CActParamAnimFv.
int func_80050C50__13CActParamAnimFv(CActParamAnim* self, u32 param, int rngArg) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    if (ml::math::mtRand(rngArg) == 0) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u8* data = view->mChildData10;
                    u8 b = view->field4BD;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u32 model = view->field2FC;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(data, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function. Gate: wrap the anim angle delta into [-pi, pi), scale it,
// and proceed only when lbl_eu_80665F50 < scaled <= lbl_eu_80665F4C.
int func_80050DB0__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;

    f32 value = view->field440 - view->field444;
    while (lbl_eu_8066A1F8 <= value) value -= lbl_eu_8066A1FC;
    while (value < -lbl_eu_8066A1F8) value += lbl_eu_8066A1FC;
    f32 scaled = value * lbl_eu_8066A20C;
    if (!(scaled <= lbl_eu_80665F4C)) return 0;
    if (!(scaled > lbl_eu_80665F50)) return 0;

    if (param == 0) return 1;
    if (param < 0x68) {
        // The 0x40000 guard flag skips the anim start while the requested
        // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
        if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
            if (view->field4BD == 0) {
                view->field4BD = 0;
            }
            u32 model = view->field2FC;
            u8 b = view->field4BD;
            view->field0C &= ~0x200;
            u32 flag = 0;
            if (model == 0) model = view->field27C;
            if (model == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
            func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
            view->field0C &= ~0x200;
        }
        return 1;
    }
    u32 flag = 0;
    if (view->field278 == 1) {
        u32 m = view->field270;
        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
    }
    u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
    func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
    return 1;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function.
// Plateau packet (best 41 mismatch / 31 structural / 10 reg-swap, size PASS
// 416B=416B, reloc drift: one A1FC presence pair):
// - Residual clusters: (1) retail hoists the loop1 2*pi lfs to +0x08 before
//   the spill stores; our build places it at +0x1c after the field reads
//   (register assignment identical otherwise). (2) small-path head arg
//   materialization: retail emits `or r5,r4` (param copy) + `addi r4,sp,8`
//   (&local) early and holds model in r8; our build keeps param in r4,
//   computes &local late, uses r5 for the model chain, and `li r7,0` sits
//   before cmpi instead of after stw. Same TU-wide scheduler wall as
//   func_8004BC94/func_8004D950/func_8004E500.
// - Found this session: writing the model fallback as a TERNARY placed AFTER
//   the flags-clear store (`model = (model != 0) ? model : view->field27C;`)
//   makes MWCC emit retail's non-inverted beq/b pair around the field27C
//   load and restores exact size parity (was -4B with if-statement form).
// - Ruled out: hoisted `f32 twoPi` local (CSEs back to one load, identical
//   output); calling func_8004BC28 directly (inlines to identical body);
//   dropping the v444 temp (flips field440/444 load order, worse); clearing
//   flags via CActParamAnimFlagView cast aliasing barrier (sees through it,
//   identical); explicit `u32* out = &localSmall` pointer var (hoists param
//   copy to instruction 3, reshuffles whole prefix, 89/82 - much worse);
//   ternary placed BEFORE the b-load/clear (keeps fallback load before the
//   store, 41/31 same count but different layout); hoisting BOTH pi and 2pi
//   into locals (98/80, loses the fneg/reload loop2 shape, -8B); moving the
//   small-path `flag = 0` init after the clear store (identical output);
//   disjunction guard form `(field0C & 0x40000) == 0 || param != 0x12`
//   instead of the negated conjunction (identical output).
// - reg_mapping hints for the next pass: one li site colored r6 in retail vs
//   r7 here (big-path flag alloc), model chain in retail r8 vs our r5, and
//   the field27C/270 fallback loads use base r31 in retail vs r3 here
//   (because retail's addi r3,r3,0x10 destroys self earlier).
// - Next: find a lever for the early A1FC lfs hoist and the early param/
//   &local materialization (possibly tied to how `param` reaches the call).
int func_80050F5C__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    // Wrap the anim angle delta into [-pi, pi) (same wrap as func_8004BC28),
    // then gate: fail when the scaled wrapped angle exceeds the upper bound.
    // Residual known diff: retail hoists the loop1 lbl_eu_8066A1FC load to
    // +0x08 (before the spills); every source shape tried keeps it after the
    // field reads.
    f32 v444 = view->field444;
    f32 value = view->field440 - v444;
    while (lbl_eu_8066A1F8 <= value) value -= lbl_eu_8066A1FC;
    while (value < -lbl_eu_8066A1F8) value += lbl_eu_8066A1FC;
    if (value * lbl_eu_8066A20C <= lbl_eu_80665F50) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if ((view->field0C & 0x40000) == 0 || param != 0x12) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 model = view->field2FC;
                    u8 b = view->field4BD;
                    view->field0C &= ~0x200;
                    u32 flag = 0;
                    // Ternary form (not if-statement) makes MWCC emit the
                    // non-inverted beq/b branch pair around the field27C
                    // fallback load, matching retail and restoring size parity.
                    model = (model != 0) ? model : view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function. Gate: wrap the anim angle delta into [-pi, pi), scale it,
// and proceed only when lbl_eu_80665EC0 <= scaled < lbl_eu_80665F54.
int func_800510FC__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;

    f32 value = view->field440 - view->field444;
    while (lbl_eu_8066A1F8 <= value) value -= lbl_eu_8066A1FC;
    while (value < -lbl_eu_8066A1F8) value += lbl_eu_8066A1FC;
    f32 scaled = value * lbl_eu_8066A20C;
    if (!(scaled >= lbl_eu_80665EC0)) return 0;
    if (!(scaled < lbl_eu_80665F54)) return 0;

    if (param == 0) return 1;
    if (param < 0x68) {
        // The 0x40000 guard flag skips the anim start while the requested
        // anim is 0x12 (retail: beq body; cmplwi 0x12; beq ret1).
        if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
            if (view->field4BD == 0) {
                view->field4BD = 0;
            }
            u32 model = view->field2FC;
            u8 b = view->field4BD;
            view->field0C &= ~0x200;
            u32 flag = 0;
            if (model == 0) model = view->field27C;
            if (model == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054170(view->mChildData10, &localSmall, param, b, flag);
            func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
            view->field0C &= ~0x200;
        }
        return 1;
    }
    u32 flag = 0;
    if (view->field278 == 1) {
        u32 m = view->field270;
        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
    }
    u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
    func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
    return 1;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_800512A8__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    // Wrap the anim angle delta into [-pi, pi) (same wrap as func_8004BC28;
    // for-loop form canonicalizes identically), then gate: fail when the
    // scaled wrapped angle falls below the lower bound.
    f32 v444 = view->field444;
    f32 value = view->field440 - v444;
    while (lbl_eu_8066A1F8 <= value) value -= lbl_eu_8066A1FC;
    while (value < -lbl_eu_8066A1F8) value += lbl_eu_8066A1FC;
    if (value * lbl_eu_8066A20C >= lbl_eu_80665F54) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u8* data;
                    u32 model = view->field2FC;
                    u8 b = view->field4BD;
                    view->field0C &= ~0x200;
                    data = view->mChildData10;
                    u32 flag = 0;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(data, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// func_800512A8 plateau packet (best 36 mismatch / 27 structural / 9 reg-swap,
// size PASS 416B=416B, reloc drift 2 sites on the prologue twoPi load):
// Newly ruled out here: named twoPi local hoisted to fn top (97/82 - merging
// both loop constants into one f1 live range breaks float alloc), for-loop
// wrap form (canonicalizes identically to while), declare-first /
// late-assigned child-data temp (drops the temp, regresses to 71/61),
// b-load-before-clear reorder (neutral). Load-bearing shape: child-data temp
// FIRST-assigned before model/b loads (fixes r8 model alloc, 71 -> 36),
// matching the func_8004D950 finding. Also ruled out: explicit
// `u32* out = &localSmall` pointer local (hoists param copy into prologue,
// 87/80 - same regression as D950 packet); moving the data assignment between
// the flags clear and flag init (canonicalizes identically - assignment
// position inside the guard block is not a lever); removing the v444 temp
// (37/27, flips the 440/444 load order). Next experiments: none found within
// high-level C++; scheduler tie-break wall family (func_8004BC94 packet).
// Semantics fully reconstructed; residual is confined to the
// call-arg materialization region. Retail hoists all four arg setups of the
// func_80054170 call (`or r5,r4,r4` param copy, `addi r4,sp+8` &out,
// `li r7,0` flag init, `addi r3,0x10` child-data) ABOVE the conditional
// field27C fallback / field270 bit tests, which also forces member reloads
// onto r31. Every MWCC build defers arg setup to the call site, keeping the
// view pointer live in r3 (lwz ra=r3) plus two tail `or` copies.
// Ruled out here: per-branch vs function-scope out-param local (function scope
// is required - it fixes the sp+8 slot), named child-data temp in 3 positions
// (first-assigned temp is load-bearing for r8 model allocation; inline or
// late-assigned regresses to 62/57), single `cleared` temp splitting the flag
// load/store, early model==0 fallback before the clear, dead alias copies of
// param (coalesced, no effect). Same wall as func_8004BC94's packet and
// MWCC_CASES rfc_send_test: MWCC scheduler tie-break on hoisting side-effect-
// free arg computations above branches. Byte-identical sibling
// func_80051584 carries the identical residual.
// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_80051448__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 local;
    if (param != 0) {
        if (param < 0x68) {
            // Skip the anim start while the 0x40000 guard flag is set and the
            // requested anim is 0x12 (same guard as the sibling helpers).
            if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                if (view->field4BD == 0) {
                    view->field4BD = 0;
                }
                u8* data = view->mChildData10;
                u32 model = view->field2FC;
                u8 b = view->field4BD;
                view->field0C &= ~0x200;
                u32 flag = 0;
                if (model == 0) model = view->field27C;
                if (model == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054170(data, &local, param, b, flag);
                func_8004BDCC(self, local, ret, view->field2A4, 0);
                view->field0C &= ~0x200;
            }
        } else {
            u32 sel = view->field278;
            u32 flag = 0;
            if (sel == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
            func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
    }
    return 1;
}

// Retail symbol is Fv but the body reads r4 (one extra param) - forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_80051584__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if (param != 0) {
        if (param < 0x68) {
            // Skip the anim start while the 0x40000 guard flag is set and the
            // requested anim is 0x12 (same guard as the sibling helpers).
            if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                if (view->field4BD == 0) {
                    view->field4BD = 0;
                }
                u8* data = view->mChildData10;
                u32 model = view->field2FC;
                u8 b = view->field4BD;
                view->field0C &= ~0x200;
                u32 flag = 0;
                if (model == 0) model = view->field27C;
                if (model == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 local;
                u32 ret = func_80054170(data, &local, param, b, flag);
                func_8004BDCC(self, local, ret, view->field2A4, 0);
                view->field0C &= ~0x200;
            }
        } else {
            u32 sel = view->field278;
            u32 flag = 0;
            if (sel == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 local;
            u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
            func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
    }
    return 1;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_800516C0__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localBig;
    u32 localSmall;
    if (param != 0) {
        if (param < 0x68) {
            // Skip the anim start while the 0x40000 guard flag is set and the
            // requested anim is 0x12 (same guard as the sibling helpers).
            if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                if (view->field4BD == 0) {
                    view->field4BD = 0;
                }
                u8* data = view->mChildData10;
                u32 model = view->field2FC;
                u8 b = view->field4BD;
                view->field0C &= ~0x200;
                u32 flag = 0;
                if (model == 0) model = view->field27C;
                if (model == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054170(data, &localSmall, param, b, flag);
                func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                view->field0C &= ~0x200;
            }
        } else {
            u32 sel = view->field278;
            u32 flag = 0;
            if (sel == 1) {
                u32 m = view->field270;
                if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
            }
            u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
            func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
    }
    return 1;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_800517FC__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    if (view->field4D4 != 0) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u8* data = view->mChildData10;
                    u8 b = view->field4BD;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u32 model = view->field2FC;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(data, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv. Mirror of
// func_800517FC with the 0x4D4 guard inverted.
int func_8005194C__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    if (view->field4D4 == 0) {
        if (param != 0) {
            if (param < 0x68) {
                // The 0x40000 guard flag skips the anim start while the
                // requested anim is 0x12 (retail: beq body; cmplwi 0x12; beq
                // ret1).
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u8* data = view->mChildData10;
                    u8 b = view->field4BD;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u32 model = view->field2FC;
                    if (model == 0) model = view->field27C;
                    if (model == 1) {
                        u32 m = view->field270;
                        if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                    }
                    u32 ret = func_80054170(data, &localSmall, param, b, flag);
                    func_8004BDCC(self, localSmall, ret, view->field2A4, 0);
                    view->field0C &= ~0x200;
                }
            } else {
                u32 flag = 0;
                if (view->field278 == 1) {
                    u32 m = view->field270;
                    if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
                }
                u32 ret = func_80054614(view->mChildData10, &localBig, param, flag, 0);
                func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
            }
        }
        return 1;
    }
    return 0;
}

void CActParamAnim::resetVec3Y() { *(float*)((u8*)this + 964) = lbl_eu_80665EA0; }

void CActParamAnim::setActiveFlag(s32 param) {
    if (param != 0) {
        *(u32*)((u8*)this + 0xC) |= 2;
    } else {
        *(u32*)((u8*)this + 0xC) &= ~2;
    }
}

extern "C" int func_80051AD0(CActParamAnim* self) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimOwnerIf* owner =
        *reinterpret_cast<CActParamAnimOwnerIf**>(reinterpret_cast<u8*>(self) + 8);
    if (owner == NULL) return 0;
    f32 animSpeed = view->field394;
    if (lbl_eu_80665EA0 == animSpeed) return 0;
    f32 val = owner->field14 * view->field430;
    int result = 0;
    if (val <= view->field434 && val >= lbl_eu_80665ECC) {
        result = 1;
    }
    return result;
}

extern "C" int func_80051B38(CActParamAnim* self) {
    void* owner = *(void**)((u8*)self + 8);
    if (owner == NULL) return 0;
    f32 animSpeed = *(f32*)((u8*)self + 0x394);
    if (lbl_eu_80665EA0 == animSpeed) return 0;
    f32 val = *(f32*)((u8*)owner + 0x14) * *(f32*)((u8*)self + 0x430);
    // 1 while val exceeds the upper bound (mirror of func_80051AD0's first condition).
    return val > *(f32*)((u8*)self + 0x434) ? 1 : 0;
}

void CActParamAnim::startAnimA() {
    *(unsigned int *)((char *)this + 0x4a8) = 0x44a05;
    *(unsigned int *)((char *)this + 0xc) |= 0x800000;
}

void CActParamAnim::startAnimA2() {
    extern float lbl_eu_80665F18;
    u32 v = *(u32*)((u8*)this + 12);
    float f = lbl_eu_80665F18;
    *(volatile u32*)((u8*)this + 0x4A8) = 0x44A05;
    *(volatile float*)((u8*)this + 0x484) = f;
    *(u32*)((u8*)this + 12) = v | 0x00800000;
}

void CActParamAnim::startAnimB() {
    extern float lbl_eu_80665F18;
    float tmp = lbl_eu_80665F18;
    *(volatile unsigned int*)((char*)this + 0x4A8) = 0x44A09;
    *(volatile float*)((char*)this + 0x484) = tmp;
}

void CActParamAnim::startAnimC() {
    extern float lbl_eu_80665F18;
    float tmp = lbl_eu_80665F18;
    *(volatile unsigned int*)((char*)this + 0x4A8) = 0x44A11;
    *(volatile float*)((char*)this + 0x484) = tmp;
}

extern "C" int func_80051BF4(CActParamAnim* self) {
    void* owner = *(void**)((u8*)self + 8);
    if (owner == NULL) return 1;
    f32 animSpeed = *(f32*)((u8*)self + 0x394);
    if (lbl_eu_80665EA0 == animSpeed) return 1;
    f32 val = *(f32*)((u8*)owner + 0x14) * *(f32*)((u8*)self + 0x430);
    // 1 while val is below the lower bound (mirror of func_80051AD0's second condition).
    return val < lbl_eu_80665ECC ? 1 : 0;
}

extern "C" void func_80051C40(CActParamAnim* self) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    // Copy 0x3A8..0x3B0 to 0x3B4..0x3BC, then flag juggling. The flag word is
    // shared state (retail re-loads it after each store), so access it through
    // a volatile pointer to keep MWCC from caching it in a register.
    volatile u32* flagp = reinterpret_cast<volatile u32*>(reinterpret_cast<u8*>(self) + 0xC);
    u32 flags = *flagp;
    view->field3B4 = view->field3A8;
    view->field3B8 = view->field3AC;
    view->field3BC = view->field3B0;
    if ((flags & 2) != 0) {
        *flagp |= 4;
    } else {
        *flagp &= ~4;
    }
    if ((*flagp & 0x40) != 0) {
        return;
    }
    if (view->field3C4 > lbl_eu_80665EA0) {
        *flagp &= ~2;
    }
}

void CActParamAnim::stopAnim() {
    u32 val = *(u32*)((u8*)this + 12);
    u32 tmp = val & ~0x10u;
    *(u32*)((u8*)this + 12) = tmp;
    if ((val & 2u) == 0) return;
    tmp &= ~0x80u;
    *(u8*)((u8*)this + 1176) = 0;
    *(u32*)((u8*)this + 12) = tmp;
}

// Per-frame ground-move update: drives the anim angle (+0x440/0x444) toward
// its target, then rotates the move direction (+0x408) toward the desired
// heading (turn / strafe / counter-turn modes) and commits the resulting
// rotation quaternion to the attached sub-object.
// Open-item packet (best 689 mismatch / 645 structural / 44 reg-swap,
// decomp 2796B vs retail 2156B, frame 0x1B0 vs retail 0x1A0):
// - All same-TU helper inlining is now eliminated via __declspec(noinline)
//   on func_8004CBC8/CC68/CC74/EC78/504BC/B3F0/CB80/B75C/B738/B0B0/B0B4/
//   52584/526C0/5274C/52780/527B0; those definitions also needed extern "C"
//   (unmangled retail symbols). Do NOT revert these - reverting regresses
//   size by ~1000B and reintroduces mangled reloc names.
// - Retail call structure reproduced: BC28 wraps on all three clamp
//   else-branches, dead func_8004B61C tail calls, wrapper trig (func_8004CC74/
//   CC68), signed s16 counter (lha/subi/sth).
// - Ruled out: removing the flagp local (neutral, MWCC CSEs &s->field0C);
//   direct nw4r::math trig calls (wrong relocs); macro with setCounter param
//   (retail sets field4DC=0xF only in the turn branch).
// - Remaining residual: ~640B excess concentrated in the three COMMIT_QUAT
//   expansions and two n-normalization blocks (MWCC emits larger shapes than
//   retail's tail-merged ones) plus prologue coloring (retail: stw r31/r30
//   spills then `or r30,r3,r3`; ours parks self in r31 first).
// - Next experiments: (1) shrink COMMIT locals (unitYCopy may cost the extra
//   0x10 frame slot); (2) hoist the shared n-normalization into a noinline
//   helper to force one copy; (3) reduce live locals across the early section
//   so self colors into r30 post-spill like retail.
// Commit helper: normalize-or-replace the move direction (+0x408), build the
// shortest-arc quaternion toward it, pre-multiply by the angle rotation, and
// push the result to the attached sub-object.
    #define COMMIT_QUAT()                                                        \
    do {                                                                          \
        if (iv->field408 == lbl_eu_80665EA0 && iv->field40C == lbl_eu_80665EA0 && \
            iv->field410 == lbl_eu_80665EA0) {                                     \
            func_8004B3F0(&iv->field408,                                           \
                          reinterpret_cast<const f32*>(&ml::CVec3::unitY));        \
        } else {                                                                  \
            func_8004CBC8(reinterpret_cast<ml::CVec3*>(&iv->field408));            \
        }                                                                         \
        nw4r::math::VEC3 unitYCopy;                                               \
        func_8004B79C(&unitYCopy.x,                                               \
                      reinterpret_cast<const f32*>(&ml::CVec3::unitY));            \
        Quaternion qa;                                                            \
        Quaternion qb;                                                            \
        func_8004B0B4(&qa);                                                       \
        func_8004B0B4(&qb);                                                       \
        func_80052584(&qa, reinterpret_cast<const Vec*>(&ml::CVec3::unitY),       \
                      reinterpret_cast<const Vec*>(&iv->field408));                \
        func_800526C0(&qb, reinterpret_cast<const Vec*>(&ml::CVec3::unitY),       \
                      f31);                                                        \
        func_8005274C(&qa, &qb);                                                  \
        if (s->object3A0 != 0) {                                                  \
            func_80052780(func_8048315C(s->object3A0), &qa);                       \
        }                                                                         \
    } while (0)

extern "C" void func_80051CD4(CActParamAnim* self) {
    CActParamAnimStateView* s = reinterpret_cast<CActParamAnimStateView*>(self);
    CActParamAnimInitView* iv = reinterpret_cast<CActParamAnimInitView*>(self);

    func_8004B344(self);
    if (func_8004CC80() != 0) return;
    func_8004B344(self);

    f32 f31 = lbl_eu_80665EA0;
    if (func_80052540(self) == 0) {
        // dt-scaled angular step limit.
        f32 f30 = func_8004B7B8(self);
        f31 = func_8005254C(self) * f30;
        void* obj = func_8004B51C(self);
        if (obj != 0) {
            // Chase the sub-object target (func_80052554) when the object's
            // +0x14 virtual returns 0 and the 0x100 flag is clear; otherwise
            // clamp the anim angle toward the stored heading (+0x440).
            bool chase = false;
            if (reinterpret_cast<CActParamAnimObjVt14*>(obj)->f14() == 0 &&
                func_8004B3D8((u32*)&s->field0C, 0x100) == 0) {
                chase = true;
            }
            if (chase) {
                if (func_80051BF4(self) == 0) {
                    f32 cur = func_8004B61C(self);
                    f32 d = func_8004BC28(func_80052554(func_8004B51C(self)) - cur);
                    if (d > f31) {
                        s->field444 = func_8004BC28(func_8004B61C(self) + f31);
                    } else if (d < -f31) {
                        s->field444 = func_8004BC28(func_8004B61C(self) - f31);
                    } else {
                        s->field444 = func_8004BC28(func_80052554(func_8004B51C(self)));
                        func_8004B694((u32*)&s->field0C, 0x100);
                    }
                    s->field440 = func_8004B61C(self);
                }
            } else {
                f32 d = func_8004BC28(iv->field440 - func_8004B61C(self));
                if (d > f31) {
                    s->field444 = func_8004BC28(func_8004B61C(self) + f31);
                } else if (d < -f31) {
                    s->field444 = func_8004BC28(func_8004B61C(self) - f31);
                } else {
                    s->field444 = func_8004BC28(iv->field440);
                    func_8004B694((u32*)&s->field0C, 0x100);
                }
                func_8004B61C(self);
            }
        } else {
            f32 d = func_8004BC28(iv->field440 - func_8004B61C(self));
            if (d > f31) {
                s->field444 = func_8004BC28(func_8004B61C(self) + f31);
            } else if (d < -f31) {
                s->field444 = func_8004BC28(func_8004B61C(self) - f31);
            } else {
                s->field444 = func_8004BC28(iv->field440);
                func_8004B694((u32*)&s->field0C, 0x100);
            }
            func_8004B61C(self);
        }
    }

    // Current anim angle.
    f31 = func_8004B61C(self);

    bool turnMode = func_8004B3D8((u32*)&s->field0C, 0x2000) != 0;
    if (!turnMode && func_8005255C(self) != 0) {
        turnMode = true; // child probe true falls through to the turn section
    }
    if (turnMode) {
        // Turn mode: converge the move direction onto the desired heading.
        if (func_8004B848(self) != 0) {
            nw4r::math::VEC3 n;
            func_8004B79C(&n.x, &iv->field3D8);
            if (iv->field3DC < lbl_eu_8066AF20) {
                f32 root = func_8004EC78(iv->field3E0 * iv->field3E0 +
                                         iv->field3D8 * iv->field3D8);
                if (root != lbl_eu_80665EA0) {
                    f32 k = lbl_eu_80665E9C / root;
                    f32 t = func_8004EC78(lbl_eu_80665E9C -
                                          lbl_eu_8066AF20 * lbl_eu_8066AF20);
                    n.x = iv->field3D8 * k * t;
                    n.y = lbl_eu_8066AF20;
                    n.z = iv->field3E0 * k * t;
                }
            }
            if (func_80052568(reinterpret_cast<u8*>(self) + 0x10) != 0) {
                func_8004B3F0(&iv->field408, &n.x);
            } else {
                nw4r::math::VEC3 d;
                func_8004CB80(&d.x, &n.x, &iv->field408);
                nw4r::math::VEC3 d2;
                func_8004B75C(&d2.x, &d.x, lbl_eu_80665ECC);
                func_8004B738(&iv->field408, &d2.x);
            }
        } else {
            nw4r::math::VEC3 fwd;
            func_8004B60C(&fwd, lbl_eu_80665EA0, lbl_eu_80665E9C,
                          lbl_eu_80665EA0);
            nw4r::math::VEC3 d;
            func_8004CB80(&d.x, &fwd.x, &iv->field408);
            nw4r::math::VEC3 d2;
            func_8004B75C(&d2.x, &d.x, lbl_eu_80665ECC);
            func_8004B738(&iv->field408, &d2.x);
        }
        COMMIT_QUAT();
        s->field4DC = 0xF;
    } else if (func_8004B3D8((u32*)&s->field0C, 0x4000) != 0) {
        // Strafe/dash mode: orbit the heading around the radial direction.
        if (func_8004B848(self) != 0) {
            nw4r::math::VEC3 n;
            func_8004B79C(&n.x, &iv->field3D8);
            if (iv->field3DC < lbl_eu_8066AF20) {
                f32 root = func_8004EC78(iv->field3E0 * iv->field3E0 +
                                         iv->field3D8 * iv->field3D8);
                if (root != lbl_eu_80665EA0) {
                    f32 k = lbl_eu_80665E9C / root;
                    n.y = lbl_eu_80665EA0;
                    f32 t = func_8004EC78(lbl_eu_80665E9C -
                                          lbl_eu_8066AF20 * lbl_eu_8066AF20);
                    n.x = iv->field3D8 * k * t;
                    n.z = iv->field3E0 * k * t;
                }
            }
            f32 mag = n.y;
            if (mag == lbl_eu_80665EA0) {
                mag = lbl_eu_80665E9C;
            }
            nw4r::math::VEC3 radial;
            func_8004B60C(&radial.x, func_8004CC74(f31), lbl_eu_80665EA0,
                          func_8004CC68(f31));
            // Remove the component along n so the radial stays on the plane.
            radial.y -= func_800504BC(&radial.x, &n.x) / mag;
            func_8004CBC8(reinterpret_cast<ml::CVec3*>(&radial));
            f32 ang2 = lbl_eu_8066A200 + func_8004CC40(radial.x, radial.z);
            nw4r::math::VEC3 dir;
            func_8004B60C(&dir.x, func_8004CC74(ang2), lbl_eu_80665EA0,
                          func_8004CC68(ang2));
            nw4r::math::VEC3 axis;
            func_8004B0B0(&axis);
            func_800527B0(&radial, &axis, &dir);
            nw4r::math::VEC3 d;
            func_8004CB80(&d.x, &axis.x, &iv->field408);
            nw4r::math::VEC3 d2;
            func_8004B75C(&d2.x, &d.x, lbl_eu_80665ECC);
            func_8004B738(&iv->field408, &d2.x);
        } else {
            nw4r::math::VEC3 fwd;
            func_8004B60C(&fwd.x, lbl_eu_80665EA0, lbl_eu_80665E9C,
                          lbl_eu_80665EA0);
            nw4r::math::VEC3 d;
            func_8004CB80(&d.x, &fwd.x, &iv->field408);
            nw4r::math::VEC3 d2;
            func_8004B75C(&d2.x, &d.x, lbl_eu_80665ECC);
            func_8004B738(&iv->field408, &d2.x);
        }
        COMMIT_QUAT();
    } else {
        s16* counter = reinterpret_cast<s16*>(&s->field4DC);
        if (*counter > 0) {
            // Counter-turn mode: slow drift toward unit Y while the counter
            // runs down (signed halfword, retail lha/subi/sth).
            nw4r::math::VEC3 d;
            func_8004CB80(&d.x, reinterpret_cast<const f32*>(&ml::CVec3::unitY),
                          &iv->field408);
            nw4r::math::VEC3 d2;
            func_8004B75C(&d2.x, &d.x, lbl_eu_80665F58);
            func_8004B738(&iv->field408, &d2.x);
            COMMIT_QUAT();
            *counter = *counter - 1;
        } else if (s->object3A0 != 0) {
            func_8004B344(self);
            if (func_80052540(self) == 0) {
                nw4r::math::VEC3 v;
                func_8004B60C(&v.x, lbl_eu_80665EA0, f31, lbl_eu_80665EA0);
                func_8004B5F0(func_8048315C(s->object3A0), &v.x);
            }
        }
    }

    // Tail: decay the snap timer and swap in the stored snap value.
    func_8004B344(self);
    if (func_80052540(self) == 0 && s->field4D0 > lbl_eu_80665EA0) {
        f32 dec = func_80484EB0(s->object3A0);
        f32 rem = s->field4D0 - dec;
        s->field4D0 = rem;
        if (rem <= lbl_eu_80665EA0) {
            s->field4A0 = iv->field4A4;
            s->field4D0 = lbl_eu_80665EA0;
        }
    }

    #undef COMMIT_QUAT
}

u32 CActParamAnim::checkFlag13() { return (*(u32*)((u8*)this + 608) >> 13) & 0x1u; }

float CActParamAnim::getParamFloat() { return *(float*)((char*)this + 0x4A0); }

float CActParamAnim::getField10() { return *(float *)((char *)this + 0x10); }

u32 CActParamAnim::checkFlag16() {
    return (*(u32*)((u8*)this + 0x260) >> 0x10) & 1;
}

u32 CActParamAnim::testAndClearFlag18() {
    u32 result = (*(u32*)((u8*)this + 0x260) >> 18) & 1u;
    *(volatile u32*)((u8*)this + 0x260) &= ~(1u << 18);
    return result;
}

// Shortest-arc rotation quaternion taking vector a to vector b. The dot is
// computed with the nw4r paired-single helper; vectors within the antiparallel
// threshold fall back to a fixed 180-degree Y rotation.
// noinline + extern "C": retail calls this (unmangled func_80052584) from
// func_80051CD4; without it MWCC absorbs the body (Warning/FrSqrt block).
extern "C" __declspec(noinline) void func_80052584(Quaternion* out, const Vec* a, const Vec* b) {
    f32 dot = nw4r::math::VEC3Dot(reinterpret_cast<const nw4r::math::VEC3*>(a),
                                  reinterpret_cast<const nw4r::math::VEC3*>(b));
    if (dot < lbl_eu_80665F5C) {
        out->x = lbl_eu_80665EA0;
        out->y = lbl_eu_80665E9C;
        out->z = lbl_eu_80665EA0;
        out->w = lbl_eu_80665EA0;
        return;
    }
    Vec cross;
    PSVECCrossProduct(a, b, &cross);
    // nw4r FSqrt wrapper inlined: warn on negative input, then
    // x <= 0 ? 0 : x * FrSqrt(x). The expression is re-derived around each
    // call (retail recomputes 2*(1+dot) instead of CSE-ing it).
    if (!(lbl_eu_80665EB4 * (lbl_eu_80665E9C + dot) >= lbl_eu_80665EA0)) {
        nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                          (const char*)lbl_eu_80526300);
    }
    f32 root =
        (lbl_eu_80665EB4 * (lbl_eu_80665E9C + dot) <= lbl_eu_80665EA0)
            ? lbl_eu_80665EA0
            : lbl_eu_80665EB4 * (lbl_eu_80665E9C + dot) *
                  nw4r::math::FrSqrt(lbl_eu_80665EB4 * (lbl_eu_80665E9C + dot));
    f32 scale = lbl_eu_80665E9C / root;
    out->x = cross.x * scale;
    out->y = cross.y * scale;
    out->z = cross.z * scale;
    out->w = lbl_eu_80665F00 * root;
}

extern "C" __declspec(noinline) void func_800526C0(Quaternion* out, const Vec* axis, f32 angle) {
    // Rotation quaternion from an axis + angle: (axis * sin(half), cos(half)).
    f32 t = lbl_eu_80665F00 * angle;
    f32 s = nw4r::math::SinFIdx(lbl_eu_80665ED8 * t);
    out->x = axis->x * s;
    out->y = axis->y * s;
    out->z = axis->z * s;
    out->w = nw4r::math::CosFIdx(lbl_eu_80665ED8 * t);
}

extern "C" __declspec(noinline) Quaternion* func_8005274C(Quaternion* self, const Quaternion* param) {
    PSQUATMultiply(self, param, self);
    return self;
}

extern "C" __declspec(noinline) void func_80052780(void* self, void* src){
    int f = *(int*)((char*)self + 224);
    volatile int* vs = (volatile int*)src;
    int a = vs[0];
    int b = vs[1];
    int c = vs[2];
    int d = vs[3];
    *(int*)((char*)self + 168) = a;
    *(int*)((char*)self + 172) = b;
    *(int*)((char*)self + 176) = c;
    *(int*)((char*)self + 180) = d;
    *(int*)((char*)self + 224) = f | 4;
}

extern "C" __declspec(noinline) void* func_800527B0(void* self, const void* a, const void* b) {
    extern void PSVECCrossProduct(const void*, const void*, void*);
    PSVECCrossProduct(self, b, (void*)a);
    return (void*)a;
}

// Clamp the anim direction (+0x3C0 triple, y forced to 0) to the per-frame
// limit speed*secPerFrame*scale: when it exceeds the limit it is renormalized,
// negated and added back; when it is below, the whole triple is reset to 0.
void func_800527E8(CActParamAnim* self) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    f32 animSpeed = view->field390;
    f32 field380 = view->field380;
    f32 frameScale = animSpeed * CDeviceVI::getSecPerFrame();
    f32 x = view->field3C0;
    f32 z = view->field3C8;
    f32 field384 = view->field384;
    Vec v;
    f32 limit = frameScale * (field384 * field380);
    v.x = x;
    v.y = lbl_eu_80665EA0;
    v.z = z;
    if (PSVECMag(&v) > limit) {
        if (v.x * v.x + v.y * v.y + v.z * v.z == lbl_eu_80665EA0) {
            *reinterpret_cast<ml::CVec3*>(&v) = ml::CVec3::zero;
        } else {
            PSVECNormalize(&v, &v);
        }
        Vec* pos = reinterpret_cast<Vec*>(&view->field3C0);
        // Retail negates/scales v in place, then adds it back onto the position.
        nw4r::math::VEC3Scale(reinterpret_cast<nw4r::math::VEC3*>(&v),
                              reinterpret_cast<nw4r::math::VEC3*>(&v), -limit);
        nw4r::math::VEC3Add(reinterpret_cast<nw4r::math::VEC3*>(pos),
                            reinterpret_cast<nw4r::math::VEC3*>(pos),
                            reinterpret_cast<nw4r::math::VEC3*>(&v));
    } else {
        if (PSVECMag(&v) < limit) {
            view->field3C0 = lbl_eu_80665EA0;
            view->field3C8 = lbl_eu_80665EA0;
        }
    }
}


extern "C" void func_80051B84(CActParamAnim* self) {
    u32 flags = *(u32*)(reinterpret_cast<u8*>(self) + 12);
    *(u32*)(reinterpret_cast<u8*>(self) + 0x4A8) = 0x00044A05;
    *(u32*)(reinterpret_cast<u8*>(self) + 0x0C) = flags | 0x00800000;
}

extern "C" int func_80052540(CActParamAnim* self) {
    return (*(u32*)(reinterpret_cast<u8*>(self) + 0x260) >> 13) & 1;
}

extern "C" f32 func_8005254C(CActParamAnim* self) {
    return *(f32*)(reinterpret_cast<u8*>(self) + 0x4A0);
}

extern "C" f32 func_80052554(void* obj) {
    return *(f32*)(reinterpret_cast<u8*>(obj) + 16);
}


extern "C" int func_80052568(void* data) {
    u8* base = reinterpret_cast<u8*>(data);
    int result = (*(u32*)(base + 0x260) >> 18) & 1;
    *(u32*)(base + 0x260) &= ~0x40000;
    return result;
}

extern "C" void func_80052924(CActParamAnim* self, f32 param) {
    *(f32*)(reinterpret_cast<u8*>(self) + 0x3C4) *= param;
}

extern "C" int func_8005255C(CActParamAnim* self) {
    return (*(u32*)(reinterpret_cast<u8*>(self) + 0x260) >> 16) & 1;
}

void CActParamAnim::mulVec3Y(float param_2) {
    *(float*)((char*)this + 0x3c4) *= param_2;
}

// Per-frame anim-axis update: shifts the position history, applies the
// speed-scaled direction clamp/decay, advances the move delta (+0x3F0 offset
// applied twice depending on the 0x100000 flag), then either commits the new
// position to the attached sub-object or reports arrival through the +0xE8
// virtual.
// Open-item packet (us-80052f6c, best ~531 mismatch / ~484 structural / 47
// reg-swap, decomp 2148B vs retail 2096B, size PASS):
// - Semantics fully reconstructed (history shift, flag juggling, snap/decay
//   clamps, blend countdown, ground probe, FrSqrt len + cap, overshoot
//   scaling, dispatchE8/BCC10 branches, commit, tail cleanup).
// - Residual 1: decomp frame is 192B vs retail 176B - one extra saved GPR
//   (r30). Routed every call through the single state-view local (`s`) and it
//   did not drop; the extra live value is likely a VEC3 local MWCC keeps in
//   r30 that retail keeps on the stack.
// - Residual 2: history-shift load order - retail loads px(3A8) first then
//   py/pz before any store; our build hoists flags first and defers the px
//   load. Explicit temps did not reproduce retail's scheduling.
// - Tail semantics CORRECTED vs banked draft: retail clears 0x10 always,
//   then (when bit1 set) clears 0x80 and zeroes byte 0x498 - NOT ~0x180.
//   Also corrected: field3C4 stored BEFORE flags |= 2 in the dispatchE8-hit
//   branch (order swapped in the BCC10-false branch).
// - Next experiments: identify the r30 value from a full decomp disasm;
//   try splitting the advance-block VEC3 locals into component floats to
//   push them to stack slots.
void CActParamAnim::func_80052934() {
    CActParamAnimStateView* s = reinterpret_cast<CActParamAnimStateView*>(this);

    // Shift the position history: 3A8/3AC/3B0 -> 3B4/3B8/3BC.
    s->field3B4 = s->field3A8;
    s->field3B8 = s->field3AC;
    s->field3BC = s->field3B0;

    // Flag juggling on the shared flag word.
    if ((s->field0C & 0x2) != 0) {
        s->field0C |= 0x4;
    } else {
        s->field0C &= ~0x4;
    }
    if ((s->field0C & 0x40) == 0 && s->field3C4 > lbl_eu_80665EA0) {
        s->field0C &= ~0x2;
    }

    u32 flags = s->field0C;
    if ((flags & 0x100) == 0) {
        nw4r::math::VEC3 cur;
        cur.x = s->field3A8;
        cur.y = s->field3AC;
        cur.z = s->field3B0;

        if ((flags & 0x10000) != 0) {
            // Snap mode: clamp the direction triple to the per-frame limit.
            func_800504DC(reinterpret_cast<CActParamAnim*>(s));
            f32 speed = s->field390;
            f32 base = s->field380;
            f32 sec = CDeviceVI::getSecPerFrame();
            f32 limit = s->field384 * base * (speed * sec);
            nw4r::math::VEC3 v;
            v.x = s->field3C0;
            v.y = lbl_eu_80665EA0;
            v.z = s->field3C8;
            if (PSVECMag(reinterpret_cast<const Vec*>(&v)) > limit) {
                if (v.x * v.x + v.y * v.y + v.z * v.z == lbl_eu_80665EA0) {
                    *reinterpret_cast<ml::CVec3*>(&v) = ml::CVec3::zero;
                } else {
                    PSVECNormalize(reinterpret_cast<const Vec*>(&v),
                                   reinterpret_cast<Vec*>(&v));
                }
                s->field3C0 += -limit * v.x;
                s->field3C4 += -limit * v.y;
                s->field3C8 += -limit * v.z;
            } else {
                if (PSVECMag(reinterpret_cast<const Vec*>(&v)) < limit) {
                    s->field3C0 = lbl_eu_80665EA0;
                    s->field3C8 = lbl_eu_80665EA0;
                }
            }
        } else {
            if ((flags & 0x40) != 0) {
                s->field3C4 = lbl_eu_80665EA0;
            } else {
                f32 speed = s->field390;
                f32 n = -s->field380;
                f32 sec = CDeviceVI::getSecPerFrame();
                s->field3C4 += n * (speed * sec);
            }
            if ((s->field0C & 0x2) != 0) {
                // Same per-frame clamp as above, active while bit30 is set.
                f32 speed = s->field390;
                f32 base = s->field380;
                f32 sec = CDeviceVI::getSecPerFrame();
                f32 limit = s->field384 * base * (speed * sec);
                nw4r::math::VEC3 v;
                v.x = s->field3C0;
                v.y = lbl_eu_80665EA0;
                v.z = s->field3C8;
                if (PSVECMag(reinterpret_cast<const Vec*>(&v)) > limit) {
                    if (v.x * v.x + v.y * v.y + v.z * v.z == lbl_eu_80665EA0) {
                        *reinterpret_cast<ml::CVec3*>(&v) = ml::CVec3::zero;
                    } else {
                        PSVECNormalize(reinterpret_cast<const Vec*>(&v),
                                       reinterpret_cast<Vec*>(&v));
                    }
                    s->field3C0 += -limit * v.x;
                    s->field3C4 += -limit * v.y;
                    s->field3C8 += -limit * v.z;
                } else {
                    if (PSVECMag(reinterpret_cast<const Vec*>(&v)) < limit) {
                        s->field3C0 = lbl_eu_80665EA0;
                        s->field3C8 = lbl_eu_80665EA0;
                    }
                }
            }
            // Clamp the anim height into [F64, F60].
            if (s->field3C4 > lbl_eu_80665F60) {
                s->field3C4 = lbl_eu_80665F60;
            } else {
                if (s->field3C4 < lbl_eu_80665F64) {
                    s->field3C4 = lbl_eu_80665F64;
                }
            }
        }

        // Common advance: frame delta plus the stored anim-direction offset.
        f32 dt = s->field390 * CDeviceVI::getSecPerFrame();
        nw4r::math::VEC3 delta;
        delta.x = s->field3C0 * dt;
        delta.y = s->field3C4 * dt;
        delta.z = s->field3C8 * dt;
        nw4r::math::VEC3 p = delta;
        p.x += s->field3CC;
        p.y += s->field3D0;
        p.z += s->field3D4;

        int r4 = 0;
        if ((s->field0C & 0x04000000) != 0) {
            // Blend countdown: after it hits zero, clear the flag and skip.
            u8 c = s->field4DA;
            u8 n2 = c - 1;
            s->field4DA = n2;
            if ((s32)(s8)n2 <= 0) {
                s->field0C &= ~0x04000000;
            } else {
                r4 = 1;
            }
        }
        if (r4 != 0 && (s->field0C & 0x1000) != 0) {
            // Ground-alignment probe: kill the horizontal components when the
            // normalized delta faces the ground normal (3FC/400/404).
            if (p.x != lbl_eu_80665EA0 || p.z != lbl_eu_80665EA0) {
                if (p.x * p.x + p.y * p.y + p.z * p.z == lbl_eu_80665EA0) {
                    *reinterpret_cast<ml::CVec3*>(&p) = ml::CVec3::zero;
                } else {
                    PSVECNormalize(reinterpret_cast<const Vec*>(&p),
                                   reinterpret_cast<Vec*>(&p));
                }
                f32 d = p.x * s->field3FC + p.y * s->field400 + p.z * s->field404;
                if (d >= lbl_eu_80665EA0) {
                    p.z = lbl_eu_80665EA0;
                    p.x = lbl_eu_80665EA0;
                }
            }
        }
        if ((s->field0C & 0x00100000) != 0) {
            nw4r::math::VEC3 step;
            step.x = s->field3F0 * dt;
            step.y = s->field3F4 * dt;
            step.z = s->field3F8 * dt;
            nw4r::math::VEC3 step2 = step;
            p += step2;
            f32 lenSq = p.x * p.x + p.z * p.z;
            if (!(lenSq >= lbl_eu_80665EA0)) {
                nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                                  (const char*)lbl_eu_80526300);
            }
            f32 len = lenSq;
            if (lenSq > lbl_eu_80665EA0) {
                len = lenSq * nw4r::math::FrSqrt(lenSq);
            }
            s->field448 = len;
            f32 cap = lbl_eu_80665F68 * dt;
            if ((f32)len > cap) {
                s->field448 = cap;
            }
        } else {
            f32 lenSq = p.x * p.x + p.z * p.z;
            if (!(lenSq >= lbl_eu_80665EA0)) {
                nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                                  (const char*)lbl_eu_80526300);
            }
            f32 len = lenSq;
            if (lenSq > lbl_eu_80665EA0) {
                len = lenSq * nw4r::math::FrSqrt(lenSq);
            }
            s->field448 = len;
            if ((s->field0C & 0x80) == 0) {
                if ((s->field270 & 0x20) != 0 &&
                    reinterpret_cast<CActParamAnimInitView*>(s)->field498 != 0) {
                    // Scale the step down so it never overshoots the last
                    // committed anim value.
                    f32 rl = (f32)len;
                    f32 last =
                        reinterpret_cast<CActParamAnimInitView*>(s)->field48C;
                    if (rl > last) {
                        f32 k = last / rl;
                        p.x *= k;
                        p.z *= k;
                        s->field448 = last;
                    }
                }
            }
            nw4r::math::VEC3 step;
            step.x = s->field3F0 * dt;
            step.y = s->field3F4 * dt;
            step.z = s->field3F8 * dt;
            nw4r::math::VEC3 step2 = step;
            p += step2;
        }

        u8* obj = s->object3A0;
        if (obj != 0) {
            const f32* ov = reinterpret_cast<const f32*>(obj + 0x7CC);
            p.x += ov[0];
            p.y += ov[1];
            p.z += ov[2];
        }
        if ((s->field0C & 0x00200000) != 0) {
            // Roll-back mode: subtract the direction offset again.
            p.x -= s->field3CC;
            p.y -= s->field3D0;
            p.z -= s->field3D4;
        }
        func_804BC9EC();
        if (func_804BCC10() != 0) {
            if ((s->field0C & 0x00400000) != 0) {
                func_80053198(reinterpret_cast<CActParamAnim*>(s),
                              reinterpret_cast<ml::CVec3*>(&p));
            }
            if (reinterpret_cast<CActParamAnimVtE8*>(s)->dispatchE8(
                    reinterpret_cast<const ml::CVec3*>(&cur)) != 0) {
                s->field3C4 = lbl_eu_80665EA0;
                s->field0C |= 0x2;
            } else {
                if ((s->field0C & 0x40) == 0) {
                    s->field0C &= ~0x2;
                }
                s->field39C = s->field3C4;
            }
        } else {
            s->field0C |= 0x2;
            s->field3C4 = lbl_eu_80665EA0;
            cur.x += p.x;
            cur.z += p.z;
        }

        // Commit the updated position to the cached state and sub-object.
        u8* obj2 = s->object3A0;
        if (obj2 != 0 && (s->field0C & 0x100) == 0) {
            s->field3A8 = cur.x;
            s->field3AC = cur.y;
            s->field3B0 = cur.z;
            u32* dst = reinterpret_cast<u32*>(func_8048315C(obj2));
            u32* srcw = reinterpret_cast<u32*>(&cur);
            dst[0] = srcw[0];
            dst[1] = srcw[1];
            dst[2] = srcw[2];
        }
    }

    if ((s->field0C & 0x20) == 0) {
        func_80051CD4(reinterpret_cast<CActParamAnim*>(s));
    }
    // Final flag cleanup: always drop 0x10; when the arrival bit (0x2) is
    // set, also drop 0x80 and reset the last-anim-value byte.
    u32 fl = s->field0C & ~0x10u;
    s->field0C = fl;
    if ((fl & 0x2) != 0) {
        reinterpret_cast<CActParamAnimInitView*>(s)->field498 = 0;
        s->field0C = fl & ~0x80u;
    }
}

// Retail symbol is Fv but the body reads r4/r5: two extra args forwarded to
// func_804BD94C's (a, b) slots (forced-name global, same scheme as
// CfObject_UnkVirtualFunc19__Q22cf13CfObjectModelFv).
void func_80053164__13CActParamAnimFv(CActParamAnim* self, void* a, void* b) {
    func_804BD94C(a, b, *(u32*)((u8*)self + 0x4A8), 0, 0, 0,
                  lbl_eu_80665EB8, lbl_eu_80665F6C, lbl_eu_80665F3C,
                  lbl_eu_80665F70, lbl_eu_80665EA0);
}

void func_80053198(CActParamAnim* self, const ml::CVec3* v) {}

// func_80053490: align the anim quaternion at +0x414 to the Y axis.
// Open-item packet (banked rank 4, best 296 mismatch / 232 structural / 64
// reg-swap, decomp 1220B vs retail 1224B, no size fail):
// - Residual is pervasive instruction-order drift from the prologue onward:
//   retail evaluates cmpwi r4,0 FIRST (before LR/spill stores), parks param
//   in r29 then dir=r30=param and self=r28 last, and recomputes &unitY into
//   a scratch reg for each of the three inline paired-single dot products;
//   our build groups the spills first and keeps different temps live.
// - Ruled out (this session): hoisting `dir = dirParam` before the null
//   check with view second (303/260, +4B); inline component-form dot
//   expressions `x*ux+y*uy+z*uz` with a shared `const CVec3& uy` reference
//   (adds a 5th live pointer -> _savegpr_27/_restgpr_27 helpers appear,
//   frame 176->192, 303/260) - the reference local must NOT be used; inline
//   component-form dots WITHOUT a ref (3x direct ml::CVec3::unitY member
//   access, 301/266, +24B - MWCC does not fold the expanded form back into
//   psq_l/ps_mul/ps_madd/ps_sum0 like ml::CVec3::dot does, so dot() is the
//   correct spelling for all three dot sites).
// - Next experiments: statement reorder of
//   the FSqrt-inlined quat writes (retail emits 420 before 41C in both late
//   branches), and matching the probe-call float-arg liveness (f2=EA0 held
//   across the sum computation).
// When dirParam is null the direction is recovered from the sub-object state
// (+0x3A8) through the probe helpers and the current quaternion is slerped
// toward the result; when non-null the caller-provided direction is used and
// the quaternion is written directly. Returns 0 when the direction is (nearly)
// parallel to Y, in which case +0x424 is left as the cross-product axis.
extern "C" int func_80053490(CActParamAnim* self, const ml::CVec3* dirParam) {    CActParamAnimInitView* view = reinterpret_cast<CActParamAnimInitView*>(self);
    const ml::CVec3* dir;
    if (dirParam == 0) {
        ml::CVec3 off;
        off.x = lbl_eu_80665EA0;
        off.y = lbl_eu_80665F18;
        off.z = lbl_eu_80665EA0;
        ml::CVec3 sum = *reinterpret_cast<ml::CVec3*>(&view->field3A8) + off;
        ml::CVec3 v = sum;
        if (func_804BE398(&v, view->field4A8, 0, 0, lbl_eu_80665F40,
                          lbl_eu_80665EA0) != 0) {
            ml::CVec3 a;
            ml::CVec3 b;
            func_804BE4B4(&a, 0);
            func_804BE4E0(&b, 0);
            dir = &b;
        } else {
            dir = &ml::CVec3::unitY;
        }
    } else {
        view->field414 = lbl_eu_80665EA0;
        view->field418 = lbl_eu_80665EA0;
        view->field41C = lbl_eu_80665EA0;
        view->field420 = lbl_eu_80665E9C;
        dir = dirParam;
    }

    f32 dot = ml::CVec3::dot(*dir, ml::CVec3::unitY);
    if (dot < lbl_eu_80665F34) {
        dot = lbl_eu_80665F34;
    } else if (dot > lbl_eu_80665E9C) {
        dot = lbl_eu_80665E9C;
    }
    int ok = 0;
    if (dot <= lbl_eu_80665E9C && dot >= lbl_eu_80665F34) {
        ok = 1;
    }
    if (ok == 0) {
        nw4r::db::Warning((const char*)lbl_eu_805262F0, 0xef,
                          (const char*)lbl_eu_805262C8);
    }
    f32 ang = ml::math::abs((f32)acos(dot) * lbl_eu_8066A20C);
    if (ang > lbl_eu_80665F14) return 0;
    if (dirParam != 0 && ang < lbl_eu_80665F00) return 0;

    view->field4CC = ang;
    ml::CVec3 tmp;
    PSVECCrossProduct(ml::CVec3::unitY, *dir, tmp);
    PSVECCrossProduct(tmp, ml::CVec3::unitY,
                      *reinterpret_cast<ml::CVec3*>(&view->field424));
    if (view->field424 * view->field424 + view->field428 * view->field428 +
            view->field42C * view->field42C ==
        lbl_eu_80665EA0) {
        *reinterpret_cast<ml::CVec3*>(&view->field424) = ml::CVec3::zero;
    } else {
        PSVECNormalize(*reinterpret_cast<ml::CVec3*>(&view->field424),
                       *reinterpret_cast<ml::CVec3*>(&view->field424));
    }

    if (dirParam != 0) {
        f32 d = ml::CVec3::dot(*dir, ml::CVec3::unitY);
        if (d < lbl_eu_80665F5C) {
            view->field414 = lbl_eu_80665EA0;
            view->field418 = lbl_eu_80665E9C;
            view->field41C = lbl_eu_80665EA0;
            view->field420 = lbl_eu_80665EA0;
        } else {
            ml::CVec3 c;
            PSVECCrossProduct(ml::CVec3::unitY, *dir, c);
            // nw4r FSqrt wrapper inlined: warn on negative input, then
            // x <= 0 ? 0 : x * FrSqrt(x). The expression is re-derived after
            // the warning/FrSqrt calls (retail recomputes EB4*(1+d) 3 times).
            if (!(lbl_eu_80665EB4 * (lbl_eu_80665E9C + d) >= lbl_eu_80665EA0)) {
                nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                                  (const char*)lbl_eu_80526300);
            }
            f32 len = (lbl_eu_80665EB4 * (lbl_eu_80665E9C + d) <=
                       lbl_eu_80665EA0)
                          ? lbl_eu_80665EA0
                          : lbl_eu_80665EB4 * (lbl_eu_80665E9C + d) *
                                nw4r::math::FrSqrt(lbl_eu_80665EB4 *
                                                    (lbl_eu_80665E9C + d));
            f32 inv = lbl_eu_80665E9C / len;
            view->field414 = c.x * inv;
            view->field418 = c.y * inv;
            view->field41C = c.z * inv;
            view->field420 = lbl_eu_80665F00 * len;
        }
    } else {
        ml::CQuat q;
        f32 d = ml::CVec3::dot(*dir, ml::CVec3::unitY);
        if (d < lbl_eu_80665F5C) {
            q.set(lbl_eu_80665EA0, lbl_eu_80665E9C, lbl_eu_80665EA0,
                  lbl_eu_80665EA0);
        } else {
            ml::CVec3 c;
            PSVECCrossProduct(ml::CVec3::unitY, *dir, c);
            // nw4r FSqrt wrapper inlined (see the dirParam != 0 branch).
            if (!(lbl_eu_80665EB4 * (lbl_eu_80665E9C + d) >= lbl_eu_80665EA0)) {
                nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                                  (const char*)lbl_eu_80526300);
            }
            f32 len = (lbl_eu_80665EB4 * (lbl_eu_80665E9C + d) <=
                       lbl_eu_80665EA0)
                          ? lbl_eu_80665EA0
                          : lbl_eu_80665EB4 * (lbl_eu_80665E9C + d) *
                                nw4r::math::FrSqrt(lbl_eu_80665EB4 *
                                                    (lbl_eu_80665E9C + d));
            f32 inv = lbl_eu_80665E9C / len;
            q.x = c.x * inv;
            q.y = c.y * inv;
            q.z = c.z * inv;
            q.w = lbl_eu_80665F00 * len;
        }
        ml::CQuat out;
        ml::CQuat::slerp(out,
                         *reinterpret_cast<ml::CQuat*>(&view->field414), q,
                         lbl_eu_80665F18);
        *reinterpret_cast<ml::CQuat*>(&view->field414) = out;
    }
    return 1;
}

extern "C" float CActParamAnim_getAnimConstant() { return lbl_eu_80665ECC; }

int CActParamAnim::getChainInt() {
    int val = *(int*)((char*)this + 1220);
    if (val) return val;
    void* ptr = *(void**)((char*)this + 8);
    if (ptr) return *(int*)((char*)ptr + 24);
    return *(int*)((char*)this + 1204);
}

extern u8 lbl_eu_80663D4C;

void func_80051A9C(CActParamAnim* self) {
    *(f32*)(reinterpret_cast<u8*>(self) + 0x3C4) = lbl_eu_80665EA0;
}



// Null-check guards: early return on null, otherwise dispatch into view pipeline
extern "C" void func_8004BC64(CActParamAnim* self, s32 param) {
    u8* B = reinterpret_cast<u8*>(self);
    if (param != 0) {
        u32 flags = *(u32*)(B + 12);
        *(u8*)(B + 0x4DA) = 30;
        *(u32*)(B + 12) = flags | 0x04000000u;
        return;
    }
    *(u32*)(B + 12) &= ~0x04000000u;
}

extern "C" void* func_8004C5EC(void* self) {
    void* p = *(void**)(reinterpret_cast<u8*>(self) + 0x2FC);
    if (p != 0)
        return p;
    return *(void**)(reinterpret_cast<u8*>(self) + 0x27C);
}

extern "C" f32 func_8004CC40(f32 a, f32 b) {
    return lbl_eu_80665ED0 * nw4r::math::Atan2FIdx(a, b);
}

extern "C" void func_8004CEF8(void* self, u32 value) {
    *(u32*)(reinterpret_cast<u8*>(self) + 0x374) = value;
}

extern "C" void func_8004DACC(void* self) {
    u8* B = reinterpret_cast<u8*>(self);
    *(u8*)(B + 0x4BE) = 0;
    *(u8*)(B + 0x4BF) = 0;
    *(u32*)(B + 0x4C0) = 0;
}

extern "C" void func_80051CAC(CActParamAnim* self) {
    u32 flags = *(u32*)(reinterpret_cast<u8*>(self) + 12);
    u32 masked = flags & ~0x10u;
    *(u32*)(reinterpret_cast<u8*>(self) + 12) = masked;
    if ((flags & 2) == 0)
        return;
    masked &= ~0x80u;
    *(u8*)(reinterpret_cast<u8*>(self) + 0x498) = 0;
    *(u32*)(reinterpret_cast<u8*>(self) + 12) = masked;
}

extern "C" void func_8004B884(CActParamAnim* self, f32 value) {
    void* ptr = *(void**)(reinterpret_cast<u8*>(self) + 928);
    if (ptr == 0)
        return;
    func_80484F80(ptr, value);
}

extern "C" int func_8004B990(void* obj, u32 sel) {
    if (*(void**)((u8*)obj + 928) == 0)
        return 0;
    if (sel != 0)
        return func_80485464(*(u8**)((u8*)obj + 928), sel);
    return func_80485244(*(u8**)((u8*)obj + 928));
}

extern "C" void func_80051AA8(CActParamAnim* self, u32 param) {
    if (param != 0) {
        *(u32*)(reinterpret_cast<u8*>(self) + 12) |= 2;
        return;
    }
    *(u32*)(reinterpret_cast<u8*>(self) + 12) &= ~2u;
}


extern "C" f32 func_80053958() {
    return lbl_eu_80665ECC;
}

extern "C" u32 func_80053960(ActParamData388* obj) {
    u8* base = reinterpret_cast<u8*>(obj);
    if (*(u32*)(base + 0x4C4) != 0)
        return *(u32*)(base + 0x4C4);
    void* ptr = *(void**)(base + 8);
    if (ptr != 0)
        return *(u32*)(reinterpret_cast<u8*>(ptr) + 24);
    return *(u32*)(base + 0x4B4);
}

extern "C" void func_eu_80053FC8() { lbl_eu_80663D4C = 0; }

extern "C" u8 func_eu_80053FD4() { return lbl_eu_80663D4C; }

extern u8 lbl_eu_80663D4C;
extern "C" void func_eu_80053FDC() { lbl_eu_80663D4C = 1; }

bool CActParamAnim::func_80053990() { return false; }

bool CActParamAnim::func_80053998() { return false; }

bool CActParamAnim::func_800539A0() { return false; }

bool CActParamAnim::func_800539A8() { return false; }

bool CActParamAnim::func_800539B0() { return false; }

bool CActParamAnim::func_800539B8() { return false; }

bool CActParamAnim::func_800539C0() { return false; }

bool CActParamAnim::func_800539C8() { return false; }

bool CActParamAnim::func_800539D0() { return false; }

bool CActParamAnim::func_800539D8() { return false; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_800539E0() {
    extern float lbl_eu_8066AF20;
    extern float lbl_eu_80663D48;
    lbl_eu_80663D48 = lbl_eu_8066AF20;
}

extern "C" void* func_8004DAC4(CActParamAnim* self) {
    return *(void**)((char*)self + 0x4c0);
}

extern "C" void func_8004B6A4(CActParamAnim* self) {
    if (*(int*)((char*)self + 0x3A4) != 0) {
        func_80055AC4((char*)self + 0x10);
    }
}

extern "C" void func_80051BA0(CActParamAnim* self) {
    u32 v = *(u32*)((u8*)self + 0xC);
    float f = lbl_eu_80665F18;
    *(volatile u32*)((u8*)self + 0x4A8) = 0x44A05;
    *(volatile float*)((u8*)self + 0x484) = f;
    *(u32*)((u8*)self + 0xC) = v | 0x00800000;
}

extern "C" void func_80051BC4(CActParamAnim* self) {
    float f = lbl_eu_80665F18;
    *(volatile u32*)((u8*)self + 0x4A8) = 0x44A09;
    *(volatile float*)((u8*)self + 0x484) = f;
}

extern "C" void func_80051BDC(CActParamAnim* self) {
    float f = lbl_eu_80665F18;
    *(volatile u32*)((u8*)self + 0x4A8) = 0x44A11;
    *(volatile float*)((u8*)self + 0x484) = f;
}
