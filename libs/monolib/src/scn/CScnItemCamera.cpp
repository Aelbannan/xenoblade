// Translation unit: monolib/src/scn/CScnItemCamera
// Scene item camera class implementation.
//
// Status: Only func_8049EA98 and func_8049F9A4 are FULL_MATCH.
//         Remaining functions are NOT_STARTED stubs.

#include <monolib/scn/CScnItemCamera.hpp>
#include <monolib/device/CDeviceGX.hpp>
#include <nw4r/math.h>
#include <nw4r/db/db_assert.h>
#include <math.h>
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

// Shared .sdata2 float constants referenced by this TU (retail pool names).
// Declared here at global scope (plain extern -> no name mangling).
extern const f32 lbl_eu_8066ABF0;
extern const f32 lbl_eu_8066AC00;
extern const f32 lbl_eu_8066AC38;
extern const f32 lbl_eu_8066AC10;
extern const f32 lbl_eu_8066A210;
extern const f32 lbl_eu_8066AC14;
extern const f32 lbl_eu_8066AC18;
extern const f32 lbl_eu_8066AC1C;
extern const f32 lbl_eu_8066AC04;     // viewport aspect-ratio scale (func_8049F8E4)
extern const f64 lbl_eu_8066AC30;     // signed int->float magic (2^52 + 2^31)
extern const char lbl_eu_80526324[];  // nw4r db Warning file name
// nw4r db Warning message (lbl_eu_80526300)
extern const char lbl_eu_80526300[];

// Static camera default-parameter table initialized by sinit_8049FC60 (.bss).
// Nine floats laid out as three CVec3s at 0x00, 0x0C and 0x18.
extern f32 lbl_eu_80658658[9];

// ============================================================
// FULL_MATCH functions
// ============================================================

// Trivial no-op constructor for CScnItemCameraSubObj arrays.
// Used by __construct_array when initializing mSubObjArray1
// and mSubObjArray2 at offsets 0x1F8 and 0x258.
// Equivalent to: CScnItemCameraSubObj::CScnItemCameraSubObj() {}
void func_8049EA98() {}

// Trivial no-op virtual function override at vtable+0x0C.
// Base class default is overridden with a no-op in this class.
// Equivalent to: void CScnItemCamera::vfunc_0C() {}
void func_8049F9A4() {}

// Camera fields the draft header does not lay out correctly. This
// overlay re-describes the retail object with the true offsets: the
// projection matrix occupies 0x154-0x1D4 (0x80 bytes), the viewport
// fields sit at 0x1D4-0x1E0, 0x1E4 is the cot-of-half-FOV value, and
// the sub-object arrays are at 0x1F8 / 0x258 (the header's draft
// layout is 0x44 short). func_8049F168 stores the transform
// position bits (0x54) as raw u32s.
struct CScnItemCameraLayout {
    u8 pad_0x00[0x4];
    void* mParent;              // 0x04 - parent/owner pointer
    u8 pad_0x08[0x54 - 0x08];
    u32 mPosX;                  // 0x54 - mTransform.mPos.x (raw bits)
    u32 mPosY;                  // 0x58 - mTransform.mPos.y
    u32 mPosZ;                  // 0x5C - mTransform.mPos.z
    u8 pad_0x60[0x138 - 0x60];
    u32 mCamParam0;             // 0x138 (raw bits)
    u32 mCamParam1;             // 0x13C
    u32 mCamParam2;             // 0x140
    f32 mDepthNear;             // 0x144
    f32 mDepthMid;              // 0x148
    f32 mDepthFarNear;          // 0x14C
    f32 mDepthFar;              // 0x150
    u8 pad_0x154[0x194 - 0x154];
    f32 mProj[16];              // 0x194 - projection matrix (MTX44)
    u16 mViewportX;             // 0x1D4
    u16 mViewportY;             // 0x1D6
    u16 mViewportWidth;         // 0x1D8
    u16 mViewportHeight;        // 0x1DA
    u8 pad_0x1DC[0x1E0 - 0x1DC];
    f32 mFovY;                  // 0x1E0
    f32 mUnk1E4;                // 0x1E4 - E / tan(half FOV)
    f32 mAspectRatio;           // 0x1E8
    f32 mNearZ;                 // 0x1EC
    f32 mFarZ;                  // 0x1F0
    f32 mUnk1F4;                // 0x1F4
    CScnItemCameraSubObj mSubObjArray1[6];  // 0x1F8
    CScnItemCameraSubObj mSubObjArray2[6];  // 0x258
};

// ============================================================
// Destructor for the 0x10-byte sub-objects stored in the two
// member arrays (retail __dt__8049EA9C, a fragment-anchored
// symbol). Standard MWCC deleting-dtor shape: free the object
// only when the deletion flag is positive, return the object.
// extern "C" keeps the raw fragment name so that the dtor's
// __destroy_arr address argument relocates to __dt__8049EA9C.
// ============================================================
extern "C" CScnItemCameraSubObj* __dt__8049EA9C(CScnItemCameraSubObj* obj,
                                                int deleting) {
    if (obj != nullptr && deleting > 0) {
        operator delete(obj);
    }
    return obj;
}

// ============================================================
// CScnItemCamera::~CScnItemCamera() (retail __dt__14CScnItemCameraFv)
// Destroys the two 6-element sub-object arrays in reverse
// declaration order (mSubObjArray2 at 0x258 first, then
// mSubObjArray1 at 0x1F8) via the MWCC runtime helper
// __destroy_arr, then frees the object when the deletion flag
// is positive. Written as a fragment function (not a member
// dtor): the class's +0x00 entry is a manual data table, not a
// compiler vtable, so retail has no vtable store here — a
// member dtor of the virtual-declaring header would emit one.
// ============================================================
CScnItemCamera* __dt__14CScnItemCameraFv(CScnItemCamera* obj, int deleting) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)obj;
    if (obj != nullptr) {
        __destroy_arr(&cam->mSubObjArray2[0],
                      reinterpret_cast<ConstructorDestructor*>(&__dt__8049EA9C),
                      0x10, 6);
        __destroy_arr(&cam->mSubObjArray1[0],
                      reinterpret_cast<ConstructorDestructor*>(&__dt__8049EA9C),
                      0x10, 6);
        if (deleting > 0) {
            operator delete(obj);
        }
    }
    return obj;
}

// vtable layout for CScnItemCamera (lbl_eu_8056ECC0, 10 entries).
// Only slot 9 (offset 0x24) is used from this TU.
struct CScnItemCameraVtbl {
    void (*slot0)(CScnItemCamera*);
    void (*slot1)(CScnItemCamera*);
    void (*slot2)(CScnItemCamera*);
    void (*slot3)(CScnItemCamera*);
    void (*slot4)(CScnItemCamera*);
    void (*slot5)(CScnItemCamera*);
    void (*slot6)(CScnItemCamera*);
    void (*slot7)(CScnItemCamera*);
    void (*slot8)(CScnItemCamera*);
    void (*slot9)(CScnItemCamera*);
};

// Viewport rect returned by CGXCache::func_8044BE10 (four s16 fields).
struct CGXCacheViewportRect {
    s16 x;       // +0x0
    s16 y;       // +0x2
    s16 width;   // +0x4
    s16 height;  // +0x6
};

// CGXCache.hpp only declares no-arg void stubs for these two members, but the
// retail functions take real arguments (the mangled `Fv` suffix is a stub
// artifact). The header is shared and read-only here, so C-linkage
// declarations reproduce the exact retail reloc names.
extern "C" CGXCacheViewportRect* func_8044BE10__8CGXCacheFv(CGXCache* cache);
extern "C" void func_8044BB20__8CGXCacheFv(CGXCache* cache, f32* projOut,
                                           f32 fovY, f32 nearZ, f32 farZ);

// Parent scene item (self->mParent, +0x04); +0x8C holds the scene object the
// camera registers itself on (its vtable slot 4 takes the camera).
struct CScnItemParentLayout {
    u8 pad_0x00[0x8C];
    void* field_0x8C;            // +0x8C
};

struct CScnItemSceneVtbl {
    void (*slot0)(void*);
    void (*slot1)(void*);
    void (*slot2)(void*);
    void (*slot3)(void*);
    void (*slot4)(void* self, CScnItemCamera* cam);
};


// ============================================================
// func_8049EB60 - rebuild the camera depth-range parameters from
// the FOV / aspect / near plane:
//   tan(halfFov) = tan(A * fovY * B * C * D) with B = pi/180
//   (A, C, D are shared .sdata2 pool constants, product 0.5).
// ============================================================
void func_8049EB60(CScnItemCamera* self) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    f32 t = (f32)tan(lbl_eu_8066AC10 * cam->mFovY * lbl_eu_8066A210 *
                     lbl_eu_8066AC18 * lbl_eu_8066AC14);

    f32 inv = lbl_eu_8066AC1C / t;
    cam->mUnk1E4 = inv;

    f32 near = -cam->mNearZ / inv;
    self->mDepthNear = near;

    self->mDepthMid = -near;
    self->mDepthFarNear = near * cam->mAspectRatio;
    self->mDepthFar = -near * cam->mAspectRatio;
}

// ============================================================
// func_8049F168 - add a world-space offset vector to the camera
// parameter vector (0x138) and to the transform position (0x54),
// then refresh through vtable slot 9. The paired-single add comes
// from the nw4r VEC3Add helper; the position is written back as
// raw u32 bits.
// ============================================================
void func_8049F168(CScnItemCamera* self, ml::CVec3* v) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->mCamParam0,
                        (nw4r::math::VEC3*)&self->mCamParam0,
                        (nw4r::math::VEC3*)v);

    ml::CVec3 newPos;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&newPos,
                        (nw4r::math::VEC3*)&self->mTransform.mPos,
                        (nw4r::math::VEC3*)v);

    cam->mPosX = *(u32*)&newPos.x;
    cam->mPosY = *(u32*)&newPos.y;
    cam->mPosZ = *(u32*)&newPos.z;

    CScnItemCameraVtbl* vtbl = *(CScnItemCameraVtbl**)self;
    vtbl->slot9(self);
}

// ============================================================
// sinit_8049FC60 - static initializer (.ctors hook) for the
// camera default-parameter table at lbl_eu_80658658: nine
// floats arranged as three CVec3s (0x00, 0x0C, 0x18).
// ============================================================
void sinit_8049FC60() {
    f32* v0 = lbl_eu_80658658;
    f32* v1 = v0 + 3;
    f32* v2 = v0 + 6;

    v0[0] = lbl_eu_8066ABF0;
    v0[1] = lbl_eu_8066AC00;
    v0[2] = lbl_eu_8066AC38;
    v1[0] = lbl_eu_8066ABF0;
    v1[1] = lbl_eu_8066ABF0;
    v1[2] = lbl_eu_8066ABF0;
    v2[0] = lbl_eu_8066ABF0;
    v2[1] = lbl_eu_8066ABF0;
    v2[2] = lbl_eu_8066AC1C;
}

// ============================================================
// NOT_STARTED stubs (to be decompiled)
// ============================================================

// ============================================================
// func_8049F6D4 - move the camera position to `v` (raw bit copy into
// mTransform.mPos) and point the camera parameter vector (0x138) at
// (mCamParam - v); store the distance and refresh via vtable slot 9.
// ============================================================
void func_8049F6D4(CScnItemCamera* self, const ml::CVec3* v) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    ml::CVec3 diff;
    ml::CVec3::sub(diff, *(const ml::CVec3*)&self->mCamParam0, *v);

    cam->mPosX = *(const u32*)&v->x;
    cam->mPosY = *(const u32*)&v->y;
    cam->mPosZ = *(const u32*)&v->z;
    self->mUnk1F4 = PSVECMag((const Vec*)&diff);

    CScnItemCameraVtbl* vtbl = *(CScnItemCameraVtbl**)self;
    vtbl->slot9(self);
}

// ============================================================
// func_8049F774 - replace the camera's embedded matrix
// (mTransform.mMtx1 at 0x6C) with `mtx` and set FLAG_0, then
// refresh via vtable slot 9.
// ============================================================
void func_8049F774(CScnItemCamera* self, const ml::CMat34* mtx) {
    self->mTransform.mMtx1 = *mtx;
    self->mTransform.mFlags |= ml::CAttrTransform::FLAG_0;

    CScnItemCameraVtbl* vtbl = *(CScnItemCameraVtbl**)self;
    vtbl->slot9(self);
}

// ============================================================
// func_8049F824 - store `f1` as the camera distance, transform the
// (0, 0, -f1) normal by the inverse local matrix, offset the result
// by the camera position and write it into the camera parameter
// vector (0x138) as raw bits.
// ============================================================
void func_8049F824(CScnItemCamera* self, f32 f1) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    self->mUnk1F4 = f1;

    ml::CVec3 normal(lbl_eu_8066ABF0, lbl_eu_8066ABF0, -f1);
    ml::CVec3 out;
    nw4r::math::VEC3TransformNormal(
        (nw4r::math::VEC3*)&out,
        (const nw4r::math::MTX34*)&self->mTransform.mLocalMatInv,
        (const nw4r::math::VEC3*)&normal);

    ml::CVec3 v = out;
    ml::CVec3 sum;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&sum,
                        (const nw4r::math::VEC3*)&v,
                        (const nw4r::math::VEC3*)&self->mTransform.mPos);

    cam->mCamParam0 = *(u32*)&sum.x;
    cam->mCamParam1 = *(u32*)&sum.y;
    cam->mCamParam2 = *(u32*)&sum.z;
}

// ============================================================
// func_8049F8E4 - update viewport + aspect ratio from the GX cache
// viewport, rebuild the projection matrix via CGXCache::func_8044BB20
// (fovY/near/far into 0x194), then register the camera on the
// parent's scene object (vtable slot 4).
// ============================================================
void func_8049F8E4(CScnItemCamera* self) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    CGXCacheViewportRect* vp =
        func_8044BE10__8CGXCacheFv(CDeviceGX::getCacheInstance());
    s16 width = vp->width;
    s16 height = vp->height;

    self->mViewportX = 0;
    self->mViewportY = 0;
    self->mViewportWidth = (u16)width;
    self->mViewportHeight = (u16)height;
    self->mAspectRatio =
        lbl_eu_8066AC04 * ((f32)width / (f32)height);

    func_8044BB20__8CGXCacheFv(CDeviceGX::getCacheInstance(), cam->mProj,
                               self->mFovY, self->mNearZ, self->mFarZ);

    CScnItemParentLayout* parent = (CScnItemParentLayout*)cam->mParent;
    void* scene = parent->field_0x8C;
    CScnItemSceneVtbl* vtbl = *(CScnItemSceneVtbl**)scene;
    vtbl->slot4(scene, self);
}

// ============================================================
// func_8049EFF8 - point the camera at a target: copy the target
// position into the camera parameters (0x138) and the look-at
// origin into mTransform.mPos, then derive yaw/pitch from the
// (mCamParam - v) vector and refresh via vtable slot 9.
// ============================================================
void func_8049EFF8(CScnItemCamera* self, const ml::CVec3* v,
                   const ml::CVec3* v2, f32 f1) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    ml::CVec3 diff = *(const ml::CVec3*)&self->mCamParam0 - *v;

    cam->mCamParam0 = *(const u32*)&v2->x;
    cam->mCamParam1 = *(const u32*)&v2->y;
    cam->mCamParam2 = *(const u32*)&v2->z;
    cam->mPosX = *(const u32*)&v->x;
    cam->mPosY = *(const u32*)&v->y;
    cam->mPosZ = *(const u32*)&v->z;
    self->mTransform.mRot.z = f1;

    self->mUnk1F4 = PSVECMag((const Vec*)&diff);

    // Yaw around Y from the x/z components (atan2(y=dx, x=-dz)).
    self->mTransform.mRot.y =
        -(lbl_eu_8066AC14 * nw4r::math::Atan2FIdx(diff.x, -diff.z));

    // Pitch from the y component against the horizontal length. The
    // horizontal length is only well-defined when dx^2+dz^2 > 0.
    f32 len2d = diff.x * diff.x + diff.z * diff.z;
    if (len2d < lbl_eu_8066ABF0) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 lenXZ = len2d <= lbl_eu_8066ABF0
                    ? lbl_eu_8066ABF0
                    : len2d * nw4r::math::FrSqrt(len2d);
    self->mTransform.mRot.x =
        lbl_eu_8066AC14 * nw4r::math::Atan2FIdx(diff.y, lenXZ);

    CScnItemCameraVtbl* vtbl = *(CScnItemCameraVtbl**)self;
    vtbl->slot9(self);
}

void __ct__CScnItemCamera(){}

void func_8049EBF0(){}

void func_8049F204(){}

void func_8049F9A8(){}
