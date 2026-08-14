// Auto-scaffolded catalog TU for kyoshin/action/CActParamAnim
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/action/CActParamAnim.hpp"
#include "kyoshin/action/CActParamData.hpp"
#include "monolib/device/CDeviceVI.hpp"
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

void CActParamAnim_initSub1() {}

void CActParamAnim_initSub2() {}

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

float CActParamAnim::getScale() {
    return *(float*)((char*)this + 0x444);
}

extern "C" void func_8004B624(CActParamAnim* self, void* object, void* state,
                              u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    view->object3A0 = reinterpret_cast<u8*>(object);
    view->state3A4 = reinterpret_cast<u32>(state);
    view->field0C |= 8;
    func_80053B24(reinterpret_cast<u8*>(self) + 0x10, self, self, state,
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

extern "C" void func_8004B738(float* destination, const float* source) {
    *reinterpret_cast<nw4r::math::VEC3*>(destination) +=
        *reinterpret_cast<const nw4r::math::VEC3*>(source);
}

extern "C" void func_8004B75C(float* destination, const float* source, float scale) {
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
    const CActParamAnimStateView* view = reinterpret_cast<const CActParamAnimStateView*>(self);
    u8* obj = view->object3A0;
    if (obj == 0) return 0;
    if (param == 0) {
        return func_80484F18(obj) <= lbl_eu_80665E9C;
    }
    if (func_8049798C(obj + 0xC) != 0) {
        return func_80485174(view->object3A0) <= lbl_eu_80665E9C;
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

void func_8004BC94(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if (param == 0) return;
    if (param < 0x68) {
        if ((view->field0C & 0x40000) != 0 && param == 0x12) return;
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

void func_8004BDCC(){}

void CActParamAnim::func_8004C5E8() {}

void* CActParamAnim::getModelObj() {
    void* result = *(void**)((char*)this + 0x2fc);
    if (result) return result;
    return *(void**)((char*)this + 0x27c);
}

void func_8004C608(){}

extern "C" void func_8004CB80(f32* out, const f32* a, const f32* b){
    nw4r::math::VEC3 result =
        *reinterpret_cast<const nw4r::math::VEC3*>(a) -
        *reinterpret_cast<const nw4r::math::VEC3*>(b);
    out[0] = result.x;
    out[1] = result.y;
    out[2] = result.z;
}

ml::CVec3* func_8004CBC8(ml::CVec3* vec) {
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

void func_8004CC8C(){}

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
        func_80053F7C(view->mChildData10, v2, v1);
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

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004D074__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if (param != 0) {
        int result = 1;
        if (func_80485464(view->object3A0, param) == 0) {
            s32 v = (param != 0) ? (s32)view->field33C : (s32)view->field2BC;
            if (func_80485174(view->object3A0) < (f32)v) result = 0;
        }
        return result;
    }
    int result = 1;
    if (func_80485244(view->object3A0) == 0) {
        s32 v = (param != 0) ? (s32)view->field33C : (s32)view->field2BC;
        if (func_80484F18(view->object3A0) < (f32)v) result = 0;
    }
    return result;
}

// Retail symbol is Fv but the body reads r4/r5 (two extra args: anim id +
// dispatch arg) — forced-name free function, same scheme as
// func_80053164__13CActParamAnimFv.
int func_8004D194__13CActParamAnimFv(CActParamAnim* self, u32 param, u32 arg2) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
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
            if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                if (view->field4BD == 0) {
                    view->field4BD = 1;
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
            u32 ret = func_80054614(view->mChildData10, &localBig, param, 1, 0);
            func_8004BDCC(self, localBig, ret, view->field2A4, (view->field0C >> 9) & 1);
        }
        return 1;
    }
    return 0;
}

void CActParamAnim::func_8004D2F8() {}

void CActParamAnim::func_8004D4AC() {}

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
// free function, same scheme as func_80053164__13CActParamAnimFv.
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

void CActParamAnim::func_8004DAE0() {}

void CActParamAnim::func_8004DDD0() {}

void CActParamAnim::func_8004DF9C() {}

void CActParamAnim::func_8004E168() {}

void CActParamAnim::func_8004E334() {}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
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

void CActParamAnim::func_8004E828() {}

void CActParamAnim::func_8004E9EC() {}

f32 func_8004EC78(f32 value) {
    // nw4r FSqrt semantics: warn on negative input, then x<=0 ? 0 : x*FrSqrt(x).
    if (!(value >= lbl_eu_80665EA0)) {
        nw4r::db::Warning((const char*)lbl_eu_80526324, 0x273,
                          (const char*)lbl_eu_80526300);
    }
    return (value <= lbl_eu_80665EA0) ? lbl_eu_80665EA0
                                      : value * nw4r::math::FrSqrt(value);
}

void CActParamAnim::func_8004ECF4() const {}

// Retail symbol is Fv but the body reads r4/r5 (two extra args) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_8004F1E4__13CActParamAnimFv(CActParamAnim* self, u32 param, s32 val) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localBig;
    u32 localSmall;
    if (view->field374 == val) {
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

void CActParamAnim::func_8004FAB4() {}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
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

void CActParamAnim::func_8004FFBC() {}

extern "C" f32 func_800504BC(const f32* a, const f32* b){
    return nw4r::math::VEC3Dot(
        reinterpret_cast<const nw4r::math::VEC3*>(a),
        reinterpret_cast<const nw4r::math::VEC3*>(b));
}

void func_800504DC(){}

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

void CActParamAnim::func_80050890() {}

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

void CActParamAnim::func_80050DB0() {}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_80050F5C__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    // Wrap the anim angle delta into [-pi, pi) (same wrap as func_8004BC28),
    // then gate: fail when the scaled wrapped angle exceeds the upper bound.
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
                if (!((view->field0C & 0x40000) != 0 && param == 0x12)) {
                    if (view->field4BD == 0) {
                        view->field4BD = 0;
                    }
                    u32 model = view->field2FC;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u8 b = view->field4BD;
                    if (model == 0) model = view->field27C;
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

void CActParamAnim::func_800510FC() {}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_800512A8__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    u32 localSmall;
    u32 localBig;
    // Wrap the anim angle delta into [-pi, pi) (same wrap as func_8004BC28),
    // then gate: fail when the scaled wrapped angle falls below the lower bound.
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
                    u32 model = view->field2FC;
                    u32 flag = 0;
                    view->field0C &= ~0x200;
                    u8 b = view->field4BD;
                    if (model == 0) model = view->field27C;
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

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_80051448__13CActParamAnimFv(CActParamAnim* self, u32 param) {
    CActParamAnimStateView* view = reinterpret_cast<CActParamAnimStateView*>(self);
    if (param == 0) return 1;
    if (param < 0x68) {
        if ((view->field0C & 0x40000) != 0 && param == 0x12) return 1;
        if (view->field4BD == 0) {
            view->field4BD = 0;
        }
        u32 model = view->field2FC;
        if (model == 0) model = view->field27C;
        u32 flag = 0;
        if (model == 1) {
            u32 m = view->field270;
            if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
        }
        u32 local;
        view->field0C &= ~0x200;
        u32 ret = func_80054170(view->mChildData10, &local, param, view->field4BD, flag);
        func_8004BDCC(self, local, ret, view->field2A4, 0);
        view->field0C &= ~0x200;
    } else {
        u32 flag = 0;
        if (view->field278 == 1) {
            u32 m = view->field270;
            if ((m & 0x10) == 0 && (m & 0x8) == 0) flag = 1;
        }
        u32 local;
        u32 ret = func_80054614(view->mChildData10, &local, param, flag, 0);
        func_8004BDCC(self, local, ret, view->field2A4, (view->field0C >> 9) & 1);
    }
    return 1;
}

// Retail symbol is Fv but the body reads r4 (one extra param) — forced-name
// free function, same scheme as func_80053164__13CActParamAnimFv.
int func_80051584__13CActParamAnimFv(CActParamAnim* self, u32 param) {
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
                view->field0C &= ~0x200;
                u8* data = view->mChildData10;
                u8 b = view->field4BD;
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

extern "C" int func_80051CD4(CActParamAnim* self) {
    void* owner = *(void**)((u8*)self + 8);
    if (owner == NULL) return 0;
    f32 animSpeed = *(f32*)((u8*)self + 0x394);
    if (animSpeed == lbl_eu_80665EA0) return 0;
    f32 val = *(f32*)((u8*)owner + 0x14) * *(f32*)((u8*)self + 0x430);
    extern f32 lbl_eu_80665ECC;
    if (val <= lbl_eu_80665ECC) return 1;
    return 0;
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

void func_80052584(){}

void func_800526C0(Quaternion* out, const Vec* axis, f32 angle) {
    // Rotation quaternion from an axis + angle: (axis * sin(half), cos(half)).
    f32 t = lbl_eu_80665F00 * angle;
    f32 s = nw4r::math::SinFIdx(lbl_eu_80665ED8 * t);
    out->x = axis->x * s;
    out->y = axis->y * s;
    out->z = axis->z * s;
    out->w = nw4r::math::CosFIdx(lbl_eu_80665ED8 * t);
}

extern "C" Quaternion* func_8005274C(Quaternion* self, const Quaternion* param) {
    PSQUATMultiply(self, param, self);
    return self;
}

extern "C" void func_80052780(void* self, void* src){
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

extern "C" void* func_800527B0(void* self, const void* a, const void* b) {
    extern void PSVECCrossProduct(const void*, const void*, void*);
    PSVECCrossProduct(self, b, (void*)a);
    return (void*)a;
}

extern "C" void func_800527E8(CActParamAnim* self) {
    extern f32 getSecPerFrame__9CDeviceVIFv(void);
    extern f32 PSVECMag(const f32*);
    f32 animSpeed = *(f32*)((u8*)self + 0x390);
    f32 field380 = *(f32*)((u8*)self + 0x380);
    f32 frameSec = getSecPerFrame__9CDeviceVIFv();
    f32 field384 = *(f32*)((u8*)self + 0x384);
    f32 field3C8 = *(f32*)((u8*)self + 0x3C8);
    f32 field3C0 = *(f32*)((u8*)self + 0x3C0);
    f32 temp = field384 * field380 * animSpeed * frameSec;
    f32 vec[3];
    vec[0] = field3C0;
    vec[1] = 0.0f;
    vec[2] = field3C8;
    PSVECMag(vec);
}

void CActParamAnim::mulVec3Y(float param_2) {
    *(float*)((char*)this + 0x3c4) *= param_2;
}

void CActParamAnim::func_80052934() {}

// Retail symbol is Fv but the body reads r4/r5: two extra args forwarded to
// func_804BD94C's (a, b) slots (forced-name global, same scheme as
// CfObject_UnkVirtualFunc19__Q22cf13CfObjectModelFv).
void func_80053164__13CActParamAnimFv(CActParamAnim* self, void* a, void* b) {
    func_804BD94C(a, b, *(u32*)((u8*)self + 0x4A8), 0, 0, 0,
                  lbl_eu_80665EB8, lbl_eu_80665F6C, lbl_eu_80665F3C,
                  lbl_eu_80665F70, lbl_eu_80665EA0);
}

void func_80053198(){}

void func_80053490(){}

extern "C" float CActParamAnim_getAnimConstant() { return lbl_eu_80665ECC; }

int CActParamAnim::getChainInt() {
    int val = *(int*)((char*)this + 1220);
    if (val) return val;
    void* ptr = *(void**)((char*)this + 8);
    if (ptr) return *(int*)((char*)ptr + 24);
    return *(int*)((char*)this + 1204);
}

extern u8 lbl_eu_80663D4C;
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
