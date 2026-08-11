// Translation unit: monolib/src/scn/CScnItemCamera
// Scene item camera class implementation.
//
// Status: Only func_8049EA98 and func_8049F9A4 are FULL_MATCH.
//         Remaining functions are NOT_STARTED stubs.

#include <monolib/scn/CScnItemCamera.hpp>
#include <monolib/device/CDeviceGX.hpp>
#include <monolib/device/CDeviceVI.hpp>
#include <monolib/util/FixStr.hpp>
#include <nw4r/math.h>
#include <nw4r/db/db_assert.h>
#include <math.h>
#include <string.h>
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

// Shared .sdata2 float constants referenced by this TU (retail pool names).
// Declared here at global scope (plain extern -> no name mangling).
extern const f32 lbl_eu_8066ABF0;
extern const f32 lbl_eu_8066ABF4;     // 1.0f
extern const f32 lbl_eu_8066ABF8;     // 0.5f
 extern const f32 lbl_eu_8066ABFC;    // 500.0f
extern const f32 lbl_eu_8066AC00;
extern const f32 lbl_eu_8066AC38;
extern const f32 lbl_eu_8066AC10;
extern const f32 lbl_eu_8066A210;
extern const f32 lbl_eu_8066AC14;
extern const f32 lbl_eu_8066AC18;
extern const f32 lbl_eu_8066AC1C;
extern const f32 lbl_eu_8066AC20;
extern const f32 lbl_eu_8066AC24;
extern const f32 lbl_eu_8066AC28;
extern const f32 lbl_eu_8066AC04;     // viewport aspect-ratio scale (func_8049F8E4)
extern const f64 lbl_eu_8066AC08;     // 2^52 (u16 -> float conversion magic)
extern const f64 lbl_eu_8066AC30;     // signed int->float magic (2^52 + 2^31)
extern const char lbl_eu_80524258[];  // FixStr<32> format string (func_8049F9A8)
extern const char lbl_eu_80526324[];  // nw4r db Warning file name
// nw4r db Warning message (lbl_eu_80526300)
extern const char lbl_eu_80526300[];

// Static camera default-parameter table initialized by sinit_8049FC60 (.bss).
// Nine floats laid out as three CVec3s at 0x00, 0x0C and 0x18. The ctor reads
// the second CVec3 (lbl_eu_80658664 = lbl_eu_80658658 + 0x0C) into mCamParam.
extern f32 lbl_eu_80658658[9];
extern f32 lbl_eu_80658664[3];

// ============================================================
// FULL_MATCH functions
// ============================================================

// Trivial no-op constructor for CScnItemCameraSubObj arrays.
// Used by __construct_array when initializing mSubObjArray1
// and mSubObjArray2 at offsets 0x1F8 and 0x258.
// Equivalent to: CScnItemCameraSubObj::CScnItemCameraSubObj() {}
// extern "C": the retail reloc name is the unmangled short form
// func_8049EA98 (a C++ definition would mangle to func_8049EA98__Fv
// and drift at every __construct_array ctor-arg site).
extern "C" void func_8049EA98() {}

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
    u8 pad_0x60[0x10C - 0x60];
    f32 field_0x10C[3];         // 0x10C - base aim offset (func_8049F204)
    u8 pad_0x118[0x138 - 0x118];
    u32 mCamParam0;             // 0x138 (raw bits)
    u32 mCamParam1;             // 0x13C
    u32 mCamParam2;             // 0x140
    f32 mDepthNear;             // 0x144
    f32 mDepthMid;              // 0x148
    f32 mDepthFarNear;          // 0x14C
    f32 mDepthFar;              // 0x150
    f32 mMtx154[4][4];          // 0x154 - depth-split projection matrix
    f32 mProj[16];              // 0x194 - projection matrix (MTX44)
    u16 mViewportX;             // 0x1D4
    u16 mViewportY;             // 0x1D6
    u16 mViewportWidth;         // 0x1D8
    u16 mViewportHeight;        // 0x1DA
    u8 mUnk1DC;                 // 0x1DC
    u8 pad_0x1DD[3];
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

// Retail vtable data (owned by another TU); referenced by the ctor's
// explicit vtable store.
extern CScnItemCameraVtbl lbl_eu_8056ECC0;

// Fake single-inheritance interface used to reach the retail vtable slot 9
// (offset 0x24) with real virtual dispatch. A manual function-pointer call
// colors the vptr r4 (MWCC_REFERENCE: manual vtable cast vs real virtual
// dispatch); a virtual call emits the retail lwz r12,0 / lwz r12,0x24 / bctrl
// shape. With -RTTI on MWCC reserves slots 0-1 for typeinfo, so the 8th
// declared virtual lands at slot 9 (0x24).
struct CScnItemCameraRefreshIf {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void refresh();
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

// Scene-item pool helpers (retail reloc names are unmangled short forms, so
// C-linkage declarations reproduce them; the retail map resolves the symbols).
struct CScnCameraItemHost;
extern "C" u32 func_8048C5B8(u8* self, s32 kind);
extern "C" u8* func_8048C480(u8* self);
extern "C" u32 func_8048C630(u8* self, u8* other, u32 flag);
// Runtime throw helper (NMWException.h is not included: it drags in
// __ppc_eabi_linker.h which conflicts with __ppc_eabi_init.h's _stack_addr).
// noreturn: MWCC elides the __end__catch epilogue of a catch handler that
// ends in a terminal call (retail catch-all handlers end with `bl __throw`).
extern "C" __declspec(noreturn) void __throw(char* throwtype, void* location,
                                             void* dtor);
// CScnItemCameraNw4r's constructor (defined in the Nw4r TU; short-form name).
extern "C" void __ct__CScnItemCameraNw4r(CScnItemCamera* self,
                                         CScnCameraItemHost* parent,
                                         const char* name);

// Parent scene item (self->mParent, +0x04); +0x8C holds the scene object the
// camera registers itself on (its vtable slot 4 takes the camera).
struct CScnItemParentLayout {
    u8 pad_0x00[0x8C];
    void* field_0x8C;            // +0x8C
};

// Host object for func_8049F9A8: a scene-item pool at +0x60.
struct CScnCameraItemHost {
    u8 pad_0x00[0x60];
    u8* mPool;                   // 0x60 - scene-item pool
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
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                        (const nw4r::math::VEC3*)&self->mCamParam0,
                        (const nw4r::math::VEC3*)v);

    cam->mPosX = *(const u32*)&v->x;
    cam->mPosY = *(const u32*)&v->y;
    cam->mPosZ = *(const u32*)&v->z;

    ml::CVec3 lenVec = diff;
    self->mUnk1F4 = PSVECMag((const Vec*)&lenVec);

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

CScnItemCamera* __ct__CScnItemCamera(CScnItemCamera* obj, CScnItemCamera* parent,
                                    const char* name) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)obj;

    cam->mParent = parent;
    obj->mType = 4;
    // Retail vtable data lives in another TU; install the label explicitly.
    *(CScnItemCameraVtbl**)obj = &lbl_eu_8056ECC0;
    obj->mIndex = -1;
    obj->mNameLen = strlen(name);
    strcpy(obj->mName, name);

    obj->mTransform.mFlags = 0;
    obj->mCamParam0 = lbl_eu_80658664[0];
    obj->mCamParam1 = lbl_eu_80658664[1];
    obj->mCamParam2 = lbl_eu_80658664[2];
    obj->mDepthNear = lbl_eu_8066ABF0;
    obj->mDepthMid = lbl_eu_8066ABF0;
    obj->mDepthFarNear = lbl_eu_8066ABF0;
    obj->mDepthFar = lbl_eu_8066ABF0;
    cam->mUnk1DC = 0;
    cam->mFovY = lbl_eu_8066ABF4;
    cam->mNearZ = lbl_eu_8066ABF8;
    cam->mFarZ = lbl_eu_8066ABFC;
    cam->mUnk1F4 = lbl_eu_8066AC00;

    __construct_array(&cam->mSubObjArray1[0],
                      (ConstructorDestructor)&func_8049EA98,
                      (ConstructorDestructor)&__dt__8049EA9C, 0x10, 6);
    __construct_array(&cam->mSubObjArray2[0],
                      (ConstructorDestructor)&func_8049EA98,
                      (ConstructorDestructor)&__dt__8049EA9C, 0x10, 6);

    f32 width = (f32)CDeviceVI::getRenderModeObj()->fbWidth;
    f32 height = (f32)CDeviceVI::getRenderModeObj()->efbHeight;
    cam->mAspectRatio = lbl_eu_8066AC04 * (height / width);

    obj->mTransform.clear();
    obj->mTransform.mFlags |= ml::CAttrTransform::FLAG_USE_ZXY;

    s16 heightS = (s16)CDeviceVI::getRenderModeObj()->efbHeight;
    s16 widthS = (s16)CDeviceVI::getRenderModeObj()->fbWidth;
    cam->mViewportWidth = (u16)widthS;
    cam->mViewportX = 0;
    cam->mViewportY = 0;
    cam->mViewportHeight = (u16)heightS;

    // vtable slot 9: rebuild the camera (real virtual dispatch -> r12).
    ((CScnItemCameraRefreshIf*)obj)->refresh();

    f32 farZ = cam->mFarZ;
    f32 nearZ = cam->mNearZ;
    f32 fovY = cam->mFovY;
    func_8044BB20__8CGXCacheFv(CDeviceGX::getCacheInstance(), cam->mProj,
                               fovY, nearZ, farZ);
    return obj;
}

// ============================================================
// func_8049EBF0 - rebuild the camera's depth-split projection and the six
// frustum plane normals: recompute the depth-range parameters (same math as
// func_8049EB60), refresh the embedded transform, build the depth-split
// projection matrix at 0x154, then transform each of the six local frustum
// planes by mLocalMat and store the world-space planes into mSubObjArray2.
// ============================================================
void func_8049EBF0(CScnItemCamera* self) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    // Rebuild the depth-range parameters (same computation as func_8049EB60).
    f32 tanHalf = (f32)tan(lbl_eu_8066AC10 * cam->mFovY * lbl_eu_8066A210 *
                           lbl_eu_8066AC18 * lbl_eu_8066AC14);
    f32 invTan = lbl_eu_8066AC1C / tanHalf;
    cam->mUnk1E4 = invTan;
    f32 near = -cam->mNearZ / invTan;
    cam->mDepthNear = near;
    cam->mDepthMid = -near;
    cam->mDepthFarNear = near * cam->mAspectRatio;
    cam->mDepthFar = -near * cam->mAspectRatio;

    self->mTransform.update();

    f32 nearZ = cam->mNearZ;
    f32 farZ = cam->mFarZ;
    f32 nearD = cam->mDepthNear;
    f32 midD = cam->mDepthMid;
    f32 farNearD = cam->mDepthFarNear;
    f32 farD = cam->mDepthFar;

    // Depth-split projection matrix at 0x154.
    f32* mtx = &cam->mMtx154[0][0];
    mtx[0x0] = lbl_eu_8066AC20 * nearZ / (midD - nearD);
    mtx[0x1] = lbl_eu_8066ABF0;
    mtx[0x2] = (midD + nearD) / (midD - nearD);
    mtx[0x3] = lbl_eu_8066ABF0;
    mtx[0x4] = lbl_eu_8066ABF0;
    mtx[0x5] = lbl_eu_8066AC20 * nearZ / (farD - farNearD);
    mtx[0x6] = (farD + farNearD) / (farD - farNearD);
    mtx[0x7] = lbl_eu_8066ABF0;
    mtx[0x8] = lbl_eu_8066ABF0;
    mtx[0x9] = lbl_eu_8066ABF0;
    mtx[0xa] = -(farZ + nearZ) / (farZ - nearZ);
    mtx[0xb] = lbl_eu_8066AC24 * nearZ * farZ / (farZ - nearZ);
    mtx[0xc] = lbl_eu_8066ABF0;
    mtx[0xd] = lbl_eu_8066ABF0;
    mtx[0xe] = lbl_eu_8066AC28;
    mtx[0xf] = lbl_eu_8066ABF0;
    cam->mUnk1DC = 1;

    // Frustum half-extents: cot(half-fov) and the aspect-scaled half-width.
    f32 cotHalf = lbl_eu_8066AC1C /
                  (f32)tan(lbl_eu_8066AC10 * cam->mFovY * lbl_eu_8066A210 *
                           lbl_eu_8066AC18 * lbl_eu_8066AC14);
    f32 wide = cam->mAspectRatio * CDeviceVI::getWidthScale();

    f32 h1 = cotHalf * cotHalf + lbl_eu_8066AC1C;
    if (!(h1 >= lbl_eu_8066ABF0)) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 invH1;
    if (h1 > lbl_eu_8066ABF0) {
        invH1 = h1 * nw4r::math::FrSqrt(h1);
    } else {
        invH1 = lbl_eu_8066ABF0;
    }
    f32 e1 = lbl_eu_8066AC1C / invH1;

    f32 h2 = cotHalf * cotHalf + wide * wide;
    if (!(h2 >= lbl_eu_8066ABF0)) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 invH2;
    if (h2 > lbl_eu_8066ABF0) {
        invH2 = h2 * nw4r::math::FrSqrt(h2);
    } else {
        invH2 = lbl_eu_8066ABF0;
    }
    f32 e2 = lbl_eu_8066AC1C / invH2;

    // Local frustum planes (4 floats each) in mSubObjArray1.
    f32* planes = (f32*)&cam->mSubObjArray1[0];
    planes[0x0] = lbl_eu_8066ABF0;   // [0].x
    planes[0x1] = lbl_eu_8066ABF0;   // [0].y
    planes[0x2] = lbl_eu_8066AC1C;   // [0].z
    planes[0x3] = farZ;              // [0].w
    planes[0x4] = lbl_eu_8066AC28;   // [1].x
    planes[0x5] = -nearZ;            // [1].y
    planes[0x6] = lbl_eu_8066ABF0;   // [1].z
    planes[0x7] = -cotHalf * e1;     // [1].w
    planes[0x8] = -e1;               // [2].x
    planes[0x9] = lbl_eu_8066ABF0;   // [2].y
    planes[0xa] = lbl_eu_8066ABF0;   // [2].z
    planes[0xb] = cotHalf * e1;      // [2].w
    planes[0xc] = -e1;               // [3].x
    planes[0xd] = lbl_eu_8066ABF0;   // [3].y
    planes[0xe] = cotHalf * e2;      // [3].z
    planes[0xf] = lbl_eu_8066ABF0;   // [3].w
    planes[0x10] = -wide * e2;       // [4].x
    planes[0x11] = lbl_eu_8066ABF0;  // [4].y
    planes[0x12] = -cotHalf * e2;    // [4].z
    planes[0x13] = lbl_eu_8066ABF0;  // [4].w
    planes[0x14] = -wide * e2;       // [5].x
    planes[0x15] = lbl_eu_8066ABF0;  // [5].y

    // Transform the 6 planes by the local matrix into world space.
    for (int i = 0; i < 6; i++) {
        nw4r::math::VEC3 out1;
        nw4r::math::VEC3TransformNormal(
            (nw4r::math::VEC3*)&out1,
            (const nw4r::math::MTX34*)&self->mTransform.mLocalMat,
            (const nw4r::math::VEC3*)&cam->mSubObjArray1[i]);

        f32 w = ((f32*)&cam->mSubObjArray1[i])[3];
        nw4r::math::VEC3 axis(lbl_eu_8066ABF0, lbl_eu_8066ABF0, fabsf(w));
        nw4r::math::VEC3 out2;
        PSMTXMultVec(self->mTransform.mLocalMat.mtx, (const Vec*)&axis,
                     (Vec*)&out2);

        *((nw4r::math::VEC3*)&cam->mSubObjArray2[i]) = out1;
        ((f32*)&cam->mSubObjArray2[i])[3] =
            -(nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&out1,
                                  (const nw4r::math::VEC3*)&out2));
    }
}

// ============================================================
// func_8049F204 - apply a rotation delta to the camera and rebuild its aim
// point: add `delta` to the embedded rotation, build the combined Z-then-X
// rotation matrix from the FIdx-scaled angles, rotate the base aim offset
// (0x10C) by the matrix scaled by the camera distance, and store the result
// as the new camera parameter vector, then refresh via vtable slot 9.
// ============================================================
void func_8049F204(CScnItemCamera* self, const ml::CVec3* delta) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    // Add the rotation delta to the embedded rotation.
    nw4r::math::VEC3 rot;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&rot,
                        (const nw4r::math::VEC3*)&self->mTransform.mRot,
                        (const nw4r::math::VEC3*)delta);
    self->mTransform.mRot.x = rot.x;
    self->mTransform.mRot.y = rot.y;
    self->mTransform.mRot.z = rot.z;

    // Rotation matrices about Z and X (angle scale = lbl_eu_8066AC18).
    f32 sinZ = nw4r::math::SinFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.z);
    f32 cosZ = nw4r::math::CosFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.z);
    f32 sinX = nw4r::math::SinFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.x);
    f32 cosX = nw4r::math::CosFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.x);

    // Combined Z-then-X rotation matrix.
    f32 m[3][3] = {
        { cosZ, -sinZ * cosX, sinZ * sinX },
        { sinZ, cosZ * cosX, -cosZ * sinX },
        { 0, sinX, cosX },
    };

    // Rotate the base aim offset by the matrix scaled by the distance.
    f32 dist = cam->mUnk1F4;
    nw4r::math::VEC3 dir(-(m[0][2] * dist), -(m[1][2] * dist),
                        -(m[2][2] * dist));

    nw4r::math::VEC3 aim;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&aim,
                        (const nw4r::math::VEC3*)&cam->field_0x10C,
                        (const nw4r::math::VEC3*)&dir);

    cam->mCamParam0 = *(const u32*)&aim.x;
    cam->mCamParam1 = *(const u32*)&aim.y;
    cam->mCamParam2 = *(const u32*)&aim.z;

    ((CScnItemCameraRefreshIf*)self)->refresh();
}

// ============================================================
// func_8049F9A8 - camera-manager factory: allocate a CScnItemCameraNw4r
// from the scene-item pool, name it from arg2, seed its transform position
// and aim point from the static default tables, aim it at the table origin,
// then rebuild its frustum parameters. Returns the new camera (or null when
// the pool is full).
// ============================================================
CScnItemCamera* func_8049F9A8(CScnCameraItemHost* self, int arg2) {
    if (func_8048C5B8(self->mPool, 4) == 0) {
        return 0;
    }

    // Build the camera name from the index before allocating.
    ml::FixStr<32> str(true);
    str.format(lbl_eu_80524258, arg2);

    u8* alloc = func_8048C480(self->mPool);
    CScnItemCamera* cam = (CScnItemCamera*)alloc;
    if (alloc != 0) {
        // Construct the Nw4r camera. The try/catch is the retail exception
        // frame: it drives the frame-pointer prologue and the CATCHBLOCK
        // extab around the constructor call; the handler rethrows via the
        // runtime __throw(0, 0, 0) (retail `li r3,0; li r4,0; li r5,0; bl
        // __throw`).
        try {
            __ct__CScnItemCameraNw4r(cam, self, (const char*)&str);
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    CScnItemCameraLayout* lay = (CScnItemCameraLayout*)cam;
    cam->mIndex = arg2;

    // Seed the transform position (0x54) and the aim point (0x138) from the
    // static default tables (raw bit copies).
    lay->mPosX = *(const u32*)&lbl_eu_80658658[0];
    lay->mPosY = *(const u32*)&lbl_eu_80658658[1];
    lay->mPosZ = *(const u32*)&lbl_eu_80658658[2];
    lay->mCamParam0 = *(const u32*)&lbl_eu_80658664[0];
    lay->mCamParam1 = *(const u32*)&lbl_eu_80658664[1];
    lay->mCamParam2 = *(const u32*)&lbl_eu_80658664[2];
    cam->mTransform.mRot.z = lbl_eu_8066ABF0;

    // diff = aim point - table origin; distance + yaw/pitch derive from it.
    nw4r::math::VEC3 diff;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                        (const nw4r::math::VEC3*)&cam->mCamParam0,
                        (const nw4r::math::VEC3*)&lbl_eu_80658658[0]);
    nw4r::math::VEC3 dir = diff;

    lay->mUnk1F4 = PSVECMag((const Vec*)&dir);

    cam->mTransform.mRot.y =
        -(lbl_eu_8066AC14 * nw4r::math::Atan2FIdx(dir.x, -dir.z));

    f32 len2d = dir.x * dir.x + dir.z * dir.z;
    if (len2d < lbl_eu_8066ABF0) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 lenXZ;
    if (len2d <= lbl_eu_8066ABF0) {
        lenXZ = lbl_eu_8066ABF0;
    } else {
        lenXZ = len2d * nw4r::math::FrSqrt(len2d);
    }
    cam->mTransform.mRot.x =
        lbl_eu_8066AC14 * nw4r::math::Atan2FIdx(dir.y, lenXZ);

    // vtable slot 9: rebuild the camera.
    ((CScnItemCameraRefreshIf*)cam)->refresh();

    // Rebuild the frustum parameters (fov 1.0, near/far from the camera).
    lay->mFovY = lbl_eu_8066ABF4;
    f32 t = (f32)tan(lbl_eu_8066AC10 * lbl_eu_8066ABF4 * lbl_eu_8066A210 *
                     lbl_eu_8066AC18 * lbl_eu_8066AC14);
    f32 inv = lbl_eu_8066AC1C / t;
    lay->mUnk1E4 = inv;
    f32 near = -lay->mNearZ / inv;
    lay->mDepthNear = near;
    lay->mDepthMid = -near;
    lay->mDepthFarNear = near * lay->mAspectRatio;
    lay->mDepthFar = -near * lay->mAspectRatio;

    func_8048C630(self->mPool, alloc, 0);
    return cam;
}
