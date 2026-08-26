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

// Named-magic conversion helpers (retail references the shared pool labels
// instead of pooling local .sdata2 entries; this TU emits no .sdata2).
// (u16/s16 conversions now use plain builtin casts - see the ctor.)
extern const char lbl_eu_80524258[];  // FixStr<32> format string (func_8049F9A8)
extern const char lbl_eu_80526324[];  // nw4r db Warning file name
// nw4r db Warning message (lbl_eu_80526300)
extern const char lbl_eu_80526300[];

// Static camera default-parameter table initialized by sinit_8049FC60 (.bss).
// Defined in this TU (dissolved monolibdata2): 12B + 28B retail split.

// Camera default-parameter table row: one CVec3.
struct SCamDefaultRow {
    f32 x, y, z;
};

// Union view of the default table (row struct used by sinit).
union SCamDefaultTable {
    f32 v[10];
    SCamDefaultRow rows[3];
};

extern SCamDefaultTable lbl_eu_80658658;

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
extern "C" void func_8049F9A4() {}

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
// compiler vtable, so retail has no vtable store here  -  a
// member dtor of the virtual-declaring header would emit one.
// ============================================================
extern "C" CScnItemCamera* __dt__14CScnItemCameraFv(CScnItemCamera* obj, int deleting) {
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

// Retail vtable data (dissolved monolibdata2); DEFINED at the bottom of this
// TU as one 0x354-byte array. Referenced by the ctor's explicit vtable store.
extern "C" u32 lbl_eu_8056ECC0[0x354 / 4];

// Fake single-inheritance interface used to reach the retail vtable slot 9
// (offset 0x24) with real virtual dispatch. A manual function-pointer call
// colors the vptr r4 (MWCC_CASES: manual vtable cast vs real virtual
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

    f32 rad = lbl_eu_8066AC10 * cam->mFovY;
    f32 scaled = rad * lbl_eu_8066A210;
    f32 t = (f32)tan(lbl_eu_8066AC14 * (lbl_eu_8066AC18 * scaled));

    f32 inv = lbl_eu_8066AC1C / t;
    cam->mUnk1E4 = inv;

    f32 near = -cam->mNearZ / inv;
    self->mDepthNear = near;

    self->mDepthMid = -near;
    self->mDepthFarNear = near * cam->mAspectRatio;
    // Retail rounds -near through double (frsp) before the Far multiply.
    self->mDepthFar = (f32)(-(f64)near) * cam->mAspectRatio;
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

    // Position (0x54) += v first: the helper writes the sum into newPos
    // (paired stores), the CVec3 copy ctor makes a member-wise float
    // copy, and the final field stores are raw-bit word copies
    // (retail lwz/stw shape).
    ml::CVec3 tmp;
    ml::CVec3 newPos;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&newPos,
                        (nw4r::math::VEC3*)&self->mTransform.mPos,
                        (nw4r::math::VEC3*)v);
    ml::CVec3 tmp2(newPos);

    // Aim point (0x138) += v.
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->mCamParam0,
                        (nw4r::math::VEC3*)&self->mCamParam0,
                        (nw4r::math::VEC3*)v);

    cam->mPosX = *(u32*)&tmp2.x;
    cam->mPosY = *(u32*)&tmp2.y;
    cam->mPosZ = *(u32*)&tmp2.z;

    ((CScnItemCameraRefreshIf*)self)->refresh();
}

// ============================================================
// sinit_8049FC60 - static initializer (.ctors hook) for the
// camera default-parameter table at lbl_eu_80658658: nine
// floats arranged as three CVec3s (0x00, 0x0C, 0x18).
// ============================================================
void sinit_8049FC60() {
    // Union-typed object: raw-float stores for row heads (.x), struct-row
    // casts for .y/.z. Retail additionally materializes three addi row
    // bases (r6/r5/r4/r3); every high-level shape tried folds to one base
    // via MWCC CSE - see sinit_804C86C0 KNOWN RESIDUAL note in
    // code_804C8684.cpp (same codegen artifact).
    lbl_eu_80658658.v[0] = lbl_eu_8066ABF0;
    ((SCamDefaultRow*)&lbl_eu_80658658)->y = lbl_eu_8066AC00;
    ((SCamDefaultRow*)&lbl_eu_80658658)->z = lbl_eu_8066AC38;
    lbl_eu_80658658.v[3] = lbl_eu_8066ABF0;
    ((volatile SCamDefaultTable*)&lbl_eu_80658658)->rows[1].y = lbl_eu_8066ABF0;
    ((volatile SCamDefaultTable*)&lbl_eu_80658658)->rows[1].z = lbl_eu_8066ABF0;
    lbl_eu_80658658.v[6] = lbl_eu_8066ABF0;
    ((volatile SCamDefaultTable*)&lbl_eu_80658658)->rows[2].y = lbl_eu_8066ABF0;
    ((volatile SCamDefaultTable*)&lbl_eu_80658658)->rows[2].z = lbl_eu_8066AC1C;
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

    // Declared in retail stack-slot order (first -> highest offset):
    // lenVec@0x14, diff@0x08.
    ml::CVec3 lenVec;
    ml::CVec3 diff;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                        (const nw4r::math::VEC3*)&self->mCamParam0,
                        (const nw4r::math::VEC3*)v);

    cam->mPosX = *(const u32*)&v->x;
    cam->mPosY = *(const u32*)&v->y;
    cam->mPosZ = *(const u32*)&v->z;

    // Member-wise float copy: struct-assign lowers to word copies, retail
    // uses lfs/stfs.
    lenVec.x = diff.x;
    lenVec.y = diff.y;
    lenVec.z = diff.z;
    self->mUnk1F4 = PSVECMag((const Vec*)&lenVec);

    ((CScnItemCameraRefreshIf*)self)->refresh();
}

// ============================================================
// func_8049F774 - replace the camera's embedded matrix
// (mTransform.mMtx1 at 0x6C) with `mtx` and set FLAG_0, then
// refresh via vtable slot 9.
// ============================================================
void func_8049F774(CScnItemCamera* self, const ml::CMat34* mtx) {
    self->mTransform.mMtx1 = *mtx;
    self->mTransform.mFlags |= ml::CAttrTransform::FLAG_0;

    // Interface-cast virtual dispatch (same shape as the ctor/F8E4 calls).
    ((CScnItemCameraRefreshIf*)self)->refresh();
}

// ============================================================
// func_8049F824 - store `f1` as the camera distance, transform the
// (0, 0, -f1) normal by the inverse local matrix, offset the result
// by the camera position and write it into the camera parameter
// vector (0x138) as raw bits.
// ============================================================
void func_8049F824(CScnItemCamera* self, f32 f1) {
    ((CScnItemCameraLayout*)self)->mUnk1F4 = f1;

    // Declared in retail stack-slot order (first -> highest offset):
    // normal@0x38, tmp@0x2C, v@0x20, out@0x14, sum@0x08. tmp's dummy init
    // is dead (reassigned before any read); the final copy is member-wise
    // so MWCC cannot copy-propagate tmp away.
    ml::CVec3 normal(lbl_eu_8066ABF0, lbl_eu_8066ABF0, -f1);
    ml::CVec3 tmp;
    ml::CVec3 v;
    ml::CVec3 out;
    ml::CVec3 sum;
    nw4r::math::VEC3TransformNormal(
        (nw4r::math::VEC3*)&out,
        (const nw4r::math::MTX34*)&self->mTransform.mLocalMatInv,
        (const nw4r::math::VEC3*)&normal);

    // Member-wise float copies: struct/plain assigns lower to word copies.
    v.x = out.x;
    v.y = out.y;
    v.z = out.z;
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&sum,
                        (const nw4r::math::VEC3*)&v,
                        (const nw4r::math::VEC3*)&self->mTransform.mPos);

    tmp.x = sum.x;
    tmp.y = sum.y;
    tmp.z = sum.z;

    ((CScnItemCameraLayout*)self)->mCamParam0 = *(u32*)&tmp.x;
    ((CScnItemCameraLayout*)self)->mCamParam1 = *(u32*)&tmp.y;
    ((CScnItemCameraLayout*)self)->mCamParam2 = *(u32*)&tmp.z;
}

// ============================================================
// func_8049F8E4 - update viewport + aspect ratio from the GX cache
// viewport, rebuild the projection matrix via CGXCache::func_8044BB20
// (fovY/near/far into 0x194), then register the camera on the
// parent's scene object (vtable slot 4).
// ============================================================
// Real-virtual-dispatch interface reaching vtable slot 4 (offset 0x10)
// of the parent scene object; a manual function-pointer call would color
// the camera argument differently than retail.
struct CScnItemSceneRegIf {
    virtual void v0();
    virtual void v1();
    virtual void registerCamera(CScnItemCamera* cam);
};

extern "C" void func_8049F8E4(CScnItemCamera* self) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)self;

    CGXCacheViewportRect* vp =
        func_8044BE10__8CGXCacheFv(CDeviceGX::getCacheInstance());
    // Read before the aspect math: keeps f1-f3 reserved for these call
    // arguments while the conversion temporaries color into f4+.
    f32 farZ = cam->mFarZ;
    f32 nearZ = cam->mNearZ;
    f32 fovY = cam->mFovY;

    s16 width;
    s16 height;
    height = vp->height;
    width = vp->width;

    cam->mViewportX = 0;
    cam->mViewportY = 0;
    cam->mViewportWidth = (u16)width;
    cam->mViewportHeight = (u16)height;

    cam->mAspectRatio = ((f32)width / (f32)height) * lbl_eu_8066AC04;

    func_8044BB20__8CGXCacheFv(CDeviceGX::getCacheInstance(), cam->mProj,
                               fovY, nearZ, farZ);

    CScnItemParentLayout* parent = (CScnItemParentLayout*)cam->mParent;
    ((CScnItemSceneRegIf*)parent->field_0x8C)->registerCamera(self);
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

    // Retail overwrites the camera parameters with v2 BEFORE deriving the
    // direction vector: diff = (new mCamParam) - v.
    cam->mCamParam0 = *(const u32*)&v2->x;
    cam->mCamParam1 = *(const u32*)&v2->y;
    cam->mCamParam2 = *(const u32*)&v2->z;
    cam->mPosX = *(const u32*)&v->x;
    cam->mPosY = *(const u32*)&v->y;
    cam->mPosZ = *(const u32*)&v->z;
    self->mTransform.mRot.z = f1;

    ml::CVec3 sub;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&sub,
                        (const nw4r::math::VEC3*)&self->mCamParam0,
                        (const nw4r::math::VEC3*)v);

    // Retail keeps two intermediate copies of the difference vector
    // (type-conversion temporaries) before the magnitude call.
    ml::CVec3 diff = sub;
    ml::CVec3 dir = diff;

    self->mUnk1F4 = PSVECMag((const Vec*)&dir);

    // Yaw around Y from the x/z components (atan2(y=dx, x=-dz)).
    self->mTransform.mRot.y =
        -(lbl_eu_8066AC14 * nw4r::math::Atan2FIdx(dir.x, -dir.z));

    // Pitch from the y component against the horizontal length. The
    // horizontal length is only well-defined when dx^2+dz^2 > 0.
    f32 len2d = dir.x * dir.x + dir.z * dir.z;
    if (len2d < lbl_eu_8066ABF0) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    f32 lenXZ = len2d <= lbl_eu_8066ABF0
                    ? lbl_eu_8066ABF0
                    : len2d * nw4r::math::FrSqrt(len2d);
    self->mTransform.mRot.x =
        lbl_eu_8066AC14 * nw4r::math::Atan2FIdx(dir.y, lenXZ);

    ((CScnItemCameraRefreshIf*)self)->refresh();
}

CScnItemCamera* __ct__CScnItemCamera(CScnItemCamera* obj, CScnItemCamera* parent,
                                    const char* name) {
    CScnItemCameraLayout* cam = (CScnItemCameraLayout*)obj;

    cam->mParent = parent;
    obj->mType = 4;
    // Retail vtable data lives in another TU; install the label explicitly.
    *(CScnItemCameraVtbl**)obj = (CScnItemCameraVtbl*)&lbl_eu_8056ECC0;
    obj->mIndex = -1;
    obj->mNameLen = strlen(name);
    strcpy(obj->mName, name);

    obj->mTransform.mFlags = 0;
    obj->mCamParam0 = lbl_eu_80658658.v[3];
    obj->mCamParam1 = lbl_eu_80658658.v[4];
    obj->mCamParam2 = lbl_eu_80658658.v[5];
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

    // Plain builtin casts: MWCC's u16->f32 conversion emits the retail
    // stw/stw/lfd/lfd/fsubs 2^52 idiom directly (manual union helpers add an
    // extra rounding insn - see MWCC_CASES CActorParam UnkVirtualFunc11).
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
extern "C" void func_8049EBF0(CScnItemCamera* self) {
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
    // Retail routes the sum through a second buffer with word bit-copies
    // into mRot (sp+68..76 intermediate).
    nw4r::math::VEC3 newRot = rot;
    self->mTransform.mRot.x = *(u32*)&newRot.x;
    self->mTransform.mRot.y = *(u32*)&newRot.y;
    self->mTransform.mRot.z = *(u32*)&newRot.z;

    // All six trig values computed first (retail keeps them live across
    // the last CosFIdx call -> 9 callee-saved FPRs).
    f32 sinZ = nw4r::math::SinFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.z);
    f32 cosZ = nw4r::math::CosFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.z);
    f32 sinX = nw4r::math::SinFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.x);
    f32 cosX = nw4r::math::CosFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.x);
    f32 sinY = nw4r::math::SinFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.y);
    f32 cosY = nw4r::math::CosFIdx(lbl_eu_8066AC18 * self->mTransform.mRot.y);

    // M_A @retail sp+224..252: {cz,-sz,1 | sz,cz,1 | 1,1,AC1C}.
    f32 mz[3][3] = {
        { cosZ, -sinZ, lbl_eu_8066ABF0 },
        { sinZ, cosZ, lbl_eu_8066ABF0 },
        { lbl_eu_8066ABF0, lbl_eu_8066ABF0, lbl_eu_8066AC1C },
    };

    // Row-major X rotation matrix.
    f32 mx[3][3] = {
        { lbl_eu_8066ABF0, lbl_eu_8066ABF0, lbl_eu_8066ABF0 },
        { lbl_eu_8066ABF0, cosX, -sinX },
        { lbl_eu_8066ABF0, sinX, cosX },
    };

    // Compose: Z then X (full 3x3 multiplies).
    f32 m1[3][3];
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            m1[r][c] = mz[r][0] * mx[0][c] + mz[r][1] * mx[1][c] +
                       mz[r][2] * mx[2][c];
        }
    }

    // Y rotation matrix (trig computed above).
    f32 my[3][3] = {
        { cosY, lbl_eu_8066ABF0, sinY },
        { lbl_eu_8066ABF0, lbl_eu_8066AC1C, lbl_eu_8066ABF0 },
        { -sinY, lbl_eu_8066ABF0, cosY },
    };

    nw4r::math::MTX34 m;
    m._00 = m1[0][0] * my[0][0] + m1[0][1] * my[1][0] + m1[0][2] * my[2][0];
    m._01 = m1[0][0] * my[0][1] + m1[0][1] * my[1][1] + m1[0][2] * my[2][1];
    m._02 = m1[0][0] * my[0][2] + m1[0][1] * my[1][2] + m1[0][2] * my[2][2];
    m._10 = m1[1][0] * my[0][0] + m1[1][1] * my[1][0] + m1[1][2] * my[2][0];
    m._11 = m1[1][0] * my[0][1] + m1[1][1] * my[1][1] + m1[1][2] * my[2][1];
    m._12 = m1[1][0] * my[0][2] + m1[1][1] * my[1][2] + m1[1][2] * my[2][2];
    m._20 = m1[2][0] * my[0][0] + m1[2][1] * my[1][0] + m1[2][2] * my[2][0];
    m._21 = m1[2][0] * my[0][1] + m1[2][1] * my[1][1] + m1[2][2] * my[2][1];
    m._22 = m1[2][0] * my[0][2] + m1[2][1] * my[1][2] + m1[2][2] * my[2][2];

    // Rotate the base aim offset by the matrix scaled by the distance.
    f32 dist = cam->mUnk1F4;
    nw4r::math::VEC3 dir(-(m._02 * dist), -(m._12 * dist),
                         -(m._22 * dist));

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
    lay->mPosX = *(const u32*)&lbl_eu_80658658.v[0];
    lay->mPosY = *(const u32*)&lbl_eu_80658658.v[1];
    lay->mPosZ = *(const u32*)&lbl_eu_80658658.v[2];
    lay->mCamParam0 = *(const u32*)&lbl_eu_80658658.v[3];
    lay->mCamParam1 = *(const u32*)&lbl_eu_80658658.v[4];
    lay->mCamParam2 = *(const u32*)&lbl_eu_80658658.v[5];
    cam->mTransform.mRot.z = lbl_eu_8066ABF0;

    // diff = aim point - table origin; distance + yaw/pitch derive from it.
    nw4r::math::VEC3 diff;
    nw4r::math::VEC3Sub((nw4r::math::VEC3*)&diff,
                        (const nw4r::math::VEC3*)&cam->mCamParam0,
                        (const nw4r::math::VEC3*)&lbl_eu_80658658.v[0]);
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

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// referenced externs (functions / data labels)
extern "C" void func_80482040();
extern "C" void func_80482048();
extern "C" void func_8048204C();
extern "C" u32 lbl_eu_80523E70;
extern "C" u32 lbl_eu_80523E84;
extern "C" u32 lbl_eu_80523E98;
extern "C" u32 lbl_eu_80523EA8;
extern "C" u32 lbl_eu_80523EB8;
extern "C" u32 lbl_eu_80523ED8;
extern "C" u32 lbl_eu_80523EF4;
extern "C" u32 lbl_eu_80523F18;
extern "C" u32 lbl_eu_80523F28;
extern "C" u32 lbl_eu_80523F3C;
extern "C" u32 lbl_eu_80523F58;
extern "C" u32 lbl_eu_80523F68;
extern "C" u32 lbl_eu_80523FA0;
extern "C" u32 lbl_eu_80523FAC;
extern "C" u32 lbl_eu_80523FC0;
extern "C" u32 lbl_eu_80523FD0;
extern "C" u32 lbl_eu_80524010;
extern "C" u32 lbl_eu_80524030;
extern "C" u32 lbl_eu_80524050;
extern "C" u32 lbl_eu_80524064;
extern "C" u32 lbl_eu_80524078;
extern "C" u32 lbl_eu_805240A0;
extern "C" u32 lbl_eu_805240AC;
extern "C" u32 lbl_eu_805240B8;
extern "C" u32 lbl_eu_805240C4;
extern "C" u32 lbl_eu_805240D8;
extern "C" u32 lbl_eu_805240F8;
extern "C" u32 lbl_eu_80524108;
extern "C" u32 lbl_eu_8052411C;
extern "C" u32 lbl_eu_80524128;
extern "C" u32 lbl_eu_80524138;
extern "C" u32 lbl_eu_80524148;
extern "C" u32 lbl_eu_80524190;
extern "C" u32 lbl_eu_805241E0;
extern "C" u32 lbl_eu_805241F8;
extern "C" u32 lbl_eu_80524218;
extern "C" u32 lbl_eu_80524228;
extern "C" u32 lbl_eu_80524238;
extern "C" u32 lbl_eu_80524248;
extern "C" const u32 lbl_eu_80524268[112];  // .rodata string table (defined below)
extern "C" u32 lbl_eu_80524274;
extern "C" u32 lbl_eu_80524280;
extern "C" u32 lbl_eu_8052428C;
extern "C" u32 lbl_eu_80524298;
extern "C" u32 lbl_eu_805242A8;
extern "C" u32 lbl_eu_805242B4;
extern "C" u32 lbl_eu_805242C0;
extern "C" u32 lbl_eu_805242CC;
extern "C" u32 lbl_eu_805242D8;
extern "C" u32 lbl_eu_805242E4;
extern "C" u32 lbl_eu_805242F0;
extern "C" u32 lbl_eu_805242FC;
extern "C" u32 lbl_eu_80524308;
extern "C" u32 lbl_eu_80524314;
extern "C" u32 lbl_eu_80524320;
extern "C" u32 lbl_eu_8052432C;
extern "C" u32 lbl_eu_80524338;
extern "C" u32 lbl_eu_80524344;
extern "C" u32 lbl_eu_80524350;
extern "C" u32 lbl_eu_8052435C;
extern "C" u32 lbl_eu_80524368;
extern "C" u32 lbl_eu_80524374;
extern "C" u32 lbl_eu_80524380;
extern "C" u32 lbl_eu_8052438C;
extern "C" u32 lbl_eu_80524398;
extern "C" u32 lbl_eu_805243A8;
extern "C" u32 lbl_eu_805243B4;
extern "C" u32 lbl_eu_805243C0;
extern "C" u32 lbl_eu_805243CC;
extern "C" u32 lbl_eu_805243D8;
extern "C" u32 lbl_eu_805243E4;
extern "C" u32 lbl_eu_805243F0;
extern "C" u32 lbl_eu_805243FC;
extern "C" u32 lbl_eu_8056DCB8;
extern "C" u32 lbl_eu_8056DCCC;
extern "C" u32 lbl_eu_8056DCF8;
extern "C" u32 lbl_eu_8056DD28;
extern "C" u32 lbl_eu_8056DD58;
extern "C" u32 lbl_eu_8056E52C;
extern "C" u32 lbl_eu_8056E54C;
extern "C" u32 lbl_eu_8056E67C;
extern "C" u32 lbl_eu_8056E6B0;
extern "C" u32 lbl_eu_8056E6DC;
extern "C" u32 lbl_eu_8056E7C4;
extern "C" u32 lbl_eu_8056E7E8;
extern "C" u32 lbl_eu_8056E88C;
extern "C" u32 lbl_eu_8056E8AC;
extern "C" u32 lbl_eu_8056E9FC;
extern "C" u32 lbl_eu_8056EA2C;
extern "C" u32 lbl_eu_8056EAC8;
extern "C" u32 lbl_eu_8056EB40;
extern "C" u32 lbl_eu_8056EB94;
extern "C" u32 lbl_eu_8056EBB8;
extern "C" u32 lbl_eu_8056ECB0;
extern "C" u32 lbl_eu_806638B0;
extern "C" u32 lbl_eu_8066A8D0;
extern "C" u32 lbl_eu_8066A8D8;
extern "C" u32 lbl_eu_8066A930;
extern "C" u32 lbl_eu_8066A938;
extern "C" u32 lbl_eu_8066A9D8;
extern "C" u32 lbl_eu_8066A9E0;
extern "C" u32 lbl_eu_8066AB70;
extern "C" u32 lbl_eu_8066AC40;
extern "C" u32 lbl_eu_8066AC48;
extern "C" u32 lbl_eu_8066AC50;
extern "C" u32 lbl_eu_8066AC58;
extern "C" u32 lbl_eu_8066AC60;
extern "C" u32 lbl_eu_8066AC68;
extern "C" u32 lbl_eu_8066AC70;
extern "C" u32 lbl_eu_8066AC78;
extern "C" u32 lbl_eu_8066AC7C;
extern "C" u32 lbl_eu_8066AC80;
extern "C" u32 lbl_eu_8066AC88;
extern "C" u32 lbl_eu_8066AC90;
extern "C" u32 lbl_eu_8066AC98;
extern "C" u32 lbl_eu_8066ACA0;
extern "C" u32 lbl_eu_8066ACA8;
extern "C" u32 lbl_eu_8066ACB0;
extern "C" u32 lbl_eu_8066ACB8;
extern "C" u32 lbl_eu_8066ACC0;
extern "C" u32 lbl_eu_8066ACC8;
extern "C" u32 lbl_eu_8066ACD0;
extern "C" u32 lbl_eu_8066ACD8;
extern "C" u32 lbl_eu_8066ACE0;
extern "C" u32 lbl_eu_8066ACE8;
extern "C" u32 lbl_eu_8066ACF0;
extern "C" u32 lbl_eu_8066ACF8;
extern "C" u32 lbl_eu_8066AD00;
extern "C" u32 lbl_eu_8066AD04;
extern "C" u32 lbl_eu_8066AD0C;
extern "C" u32 lbl_eu_8066AD10;
extern "C" u32 lbl_eu_8066AD14;
extern "C" u32 lbl_eu_8066AD18;
extern "C" u32 lbl_eu_8066AD1C;
extern "C" u32 lbl_eu_8066AD20;
extern "C" u32 lbl_eu_8066AD24;
extern "C" u32 lbl_eu_8066AD28;
extern "C" u32 lbl_eu_8066AD30;
extern "C" u32 lbl_eu_8066AD38;
extern "C" u32 lbl_eu_8066AD3C;
extern "C" u32 lbl_eu_8066AD40;
extern "C" u32 lbl_eu_8066AD44;
extern "C" u32 lbl_eu_8066AD48;
extern "C" u32 lbl_eu_8066AD4C;
extern "C" u32 lbl_eu_8066AD50;
extern "C" u32 lbl_eu_8066AD58;
extern "C" u32 lbl_eu_8066AD60;
extern "C" u32 lbl_eu_8066AD64;
extern "C" u32 lbl_eu_8066AD68;
extern "C" u32 lbl_eu_8066AD70;
extern "C" u32 lbl_eu_8066AD78;
extern "C" u32 lbl_eu_8066AD7C;
extern "C" u32 lbl_eu_8066AD84;
extern "C" u32 lbl_eu_8066AD8C;
extern "C" u32 lbl_eu_8066AD94;
extern "C" u32 lbl_eu_8066AD9C;
extern "C" u32 lbl_eu_8066ADA4;
extern "C" u32 lbl_eu_8066ADAC;
extern "C" u32 lbl_eu_8066ADB4;
extern "C" u32 lbl_eu_8066ADBC;
extern "C" u32 lbl_eu_8066ADC4;
extern "C" u32 lbl_eu_8066ADCC;
extern "C" u32 lbl_eu_8066ADD0;
extern "C" u32 lbl_eu_8066ADD4;
extern "C" u32 lbl_eu_8066ADDC;
extern "C" u32 lbl_eu_8066ADE0;
extern "C" u32 lbl_eu_8066ADE4;
extern "C" u32 lbl_eu_8066ADEC;
extern "C" u32 lbl_eu_8066ADF4;
extern "C" u32 lbl_eu_8066ADF8;
extern "C" u32 lbl_eu_8066AE00;
extern "C" u32 lbl_eu_8066AE08;
extern "C" u32 lbl_eu_8066AE10;
extern "C" u32 lbl_eu_8066AE18;
extern "C" u32 lbl_eu_8066AE1C;

// [.data] 0x8056ECC0-0x8056F014 (0x354B)
extern "C" u32 lbl_eu_8056ECC0[213] = {
    (u32)&lbl_eu_806638B0, 0x00000000, (u32)&__dt__14CScnItemCameraFv, (u32)&func_8049F9A4, (u32)&func_80482048, (u32)&func_8049F8E4, (u32)&func_8048204C, (u32)&func_80482040,
    0x00000000, (u32)&func_8049EBF0, (u32)&lbl_eu_8066AC40, 0x00000000, (u32)&lbl_eu_8066AC48, 0x00000001, (u32)&lbl_eu_8066AC50, 0x00000001,
    (u32)&lbl_eu_8066AC58, 0x00000001, (u32)&lbl_eu_8066AC60, 0x00000001, (u32)&lbl_eu_8066AC68, 0x00000001, (u32)&lbl_eu_8066AC70, 0x00010001,
    (u32)&lbl_eu_80524268, 0x00020001, (u32)&lbl_eu_80524274, 0x00010001, (u32)&lbl_eu_80524280, 0x00020001, (u32)&lbl_eu_8052428C, 0x00010001,
    (u32)&lbl_eu_80524298, 0x00020001, (u32)&lbl_eu_805242A8, 0x00010001, (u32)&lbl_eu_805242B4, 0x00020001, (u32)&lbl_eu_8066AC78, 0x00010001,
    (u32)&lbl_eu_8066AC7C, 0x0001FFFF, (u32)&lbl_eu_8066AC80, 0x00010001, (u32)&lbl_eu_8066AC88, 0x0001FFFF, (u32)&lbl_eu_805242C0, 0x0001FFFF,
    (u32)&lbl_eu_8066AC90, 0x00000001, (u32)&lbl_eu_8066AC98, 0x00000001, (u32)&lbl_eu_8066ACA0, 0x00000001, (u32)&lbl_eu_8066ACA8, 0x00000001,
    (u32)&lbl_eu_8066ACB0, 0x0000FFFF, (u32)&lbl_eu_8066ACB8, 0x0000FFFF, (u32)&lbl_eu_8066ACC0, 0x0000FFFF, (u32)&lbl_eu_8066ACC8, 0x0000FFFF,
    (u32)&lbl_eu_805242CC, 0x00000001, (u32)&lbl_eu_805242D8, 0x00000001, (u32)&lbl_eu_805242E4, 0x00000001, (u32)&lbl_eu_805242F0, 0x00000001,
    (u32)&lbl_eu_805242FC, 0x0000FFFF, (u32)&lbl_eu_80524308, 0x0000FFFF, (u32)&lbl_eu_80524314, 0x0000FFFF, (u32)&lbl_eu_80524320, 0x0000FFFF,
    (u32)&lbl_eu_8052432C, 0x00010001, (u32)&lbl_eu_80524338, 0x00020001, (u32)&lbl_eu_80524344, 0x0001FFFF, (u32)&lbl_eu_80524350, 0x0002FFFF,
    (u32)&lbl_eu_8066ACD0, 0x0000FFFF, (u32)&lbl_eu_8066ACD8, 0x0000FFFD, (u32)&lbl_eu_8066ACE0, 0x00000001, (u32)&lbl_eu_8066ACE8, 0x00000001,
    (u32)&lbl_eu_8052435C, 0x00000001, (u32)&lbl_eu_8066ACF0, 0x00010001, (u32)&lbl_eu_80524368, 0x00020001, (u32)&lbl_eu_80524374, 0x00010001,
    (u32)&lbl_eu_80524380, 0x00020001, (u32)&lbl_eu_8052438C, 0x00010001, (u32)&lbl_eu_80524398, 0x00020001, (u32)&lbl_eu_8066ACF8, 0x00000000,
    (u32)&lbl_eu_8066AD00, 0x0000FFFF, (u32)&lbl_eu_8066AD04, 0x0000FFFF, (u32)&lbl_eu_8066AD0C, 0x0000FFFF, (u32)&lbl_eu_8066AD10, 0x0000FFFF,
    (u32)&lbl_eu_8066AD14, 0x0000FFFF, (u32)&lbl_eu_8066AD18, 0x0000FFFF, (u32)&lbl_eu_8066AD1C, 0x0000FFFF, (u32)&lbl_eu_8066AD20, 0x0000FFFF,
    (u32)&lbl_eu_8066AD24, 0x0000FFFF, (u32)&lbl_eu_8066AD28, 0x0000FFFF, (u32)&lbl_eu_8066AD30, 0x0000FFFF, (u32)&lbl_eu_8066AD38, 0x0000FFFF,
    (u32)&lbl_eu_8066AD3C, 0x0000FFFF, (u32)&lbl_eu_8066AD40, 0x0000FFFF, (u32)&lbl_eu_8066AD44, 0x0000FFFF, (u32)&lbl_eu_8066AD48, 0x0000FFFF,
    (u32)&lbl_eu_8066AD4C, 0x0000FFFF, (u32)&lbl_eu_8066AD50, 0x0000FFFF, (u32)&lbl_eu_8066AD58, 0x0000FFFF, (u32)&lbl_eu_8066AD60, 0x00020000,
    (u32)&lbl_eu_8066AD64, 0x0002FFFF, (u32)&lbl_eu_8066AD68, 0x00010000, (u32)&lbl_eu_8066AD70, 0x00020000, (u32)&lbl_eu_805243A8, 0x00000000,
    (u32)&lbl_eu_8066AD78, 0x00000000, (u32)&lbl_eu_8066AD7C, 0x00000000, (u32)&lbl_eu_8066AD84, 0x00010000, (u32)&lbl_eu_805243B4, 0x00020000,
    (u32)&lbl_eu_805243C0, 0x00010000, (u32)&lbl_eu_805243CC, 0x00020000, (u32)&lbl_eu_8066AD8C, 0x00010000, (u32)&lbl_eu_805243D8, 0x00020000,
    (u32)&lbl_eu_8066AD94, 0x00010000, (u32)&lbl_eu_805243E4, 0x00020000, (u32)&lbl_eu_8066AD9C, 0x0001FFFF, (u32)&lbl_eu_805243F0, 0x0002FFFF,
    (u32)&lbl_eu_8066ADA4, 0x00010000, (u32)&lbl_eu_8066ADAC, 0x00020000, (u32)&lbl_eu_8066ADB4, 0x00000000, (u32)&lbl_eu_8066ADBC, 0x00000000,
    (u32)&lbl_eu_8066ADC4, 0x00010000, (u32)&lbl_eu_8066ADCC, 0x00000000, (u32)&lbl_eu_8066ADD0, 0x00000000, (u32)&lbl_eu_8066ADD4, 0x00000000,
    (u32)&lbl_eu_8066ADDC, 0x00000000, (u32)&lbl_eu_8066ADE0, (u32)&lbl_eu_8066ADE4, (u32)&lbl_eu_8066ADEC, (u32)&lbl_eu_8066ADF4, (u32)&lbl_eu_8066ADF8, (u32)&lbl_eu_8066AE00,
    (u32)&lbl_eu_8066AE08, (u32)&lbl_eu_805243FC, (u32)&lbl_eu_8066AE10, (u32)&lbl_eu_8066AE18, (u32)&lbl_eu_8066AE1C,
};

// [.rodata] 0x80524268-0x80524428 (0x1c0B) raw words
extern "C" const u32 lbl_eu_80524268[112] = {
    0x434F4E53, 0x545F495F, 0x57000000, 0x504F4F4C, 0x5F494E54, 0x00000000, 0x504F4F4C, 0x5F494E54,
    0x5F570000, 0x504F4F4C, 0x5F464958, 0x45440000, 0x504F4F4C, 0x5F464958, 0x45445F57, 0x00000000,
    0x504F4F4C, 0x5F535452, 0x00000000, 0x504F4F4C, 0x5F535452, 0x5F570000, 0x53545F41, 0x52475F4F,
    0x4D495400, 0x4C445F41, 0x52475F30, 0x00000000, 0x4C445F41, 0x52475F31, 0x00000000, 0x4C445F41,
    0x52475F32, 0x00000000, 0x4C445F41, 0x52475F33, 0x00000000, 0x53545F41, 0x52475F30, 0x00000000,
    0x53545F41, 0x52475F31, 0x00000000, 0x53545F41, 0x52475F32, 0x00000000, 0x53545F41, 0x52475F33,
    0x00000000, 0x4C445F53, 0x54415449, 0x43000000, 0x4C445F53, 0x54415449, 0x435F5700, 0x53545F53,
    0x54415449, 0x43000000, 0x53545F53, 0x54415449, 0x435F5700, 0x4C445F46, 0x414C5345, 0x00000000,
    0x4C445F46, 0x554E435F, 0x57000000, 0x4C445F50, 0x4C554749, 0x4E000000, 0x4C445F50, 0x4C554749,
    0x4E5F5700, 0x4C445F46, 0x554E435F, 0x46415200, 0x4C445F46, 0x554E435F, 0x4641525F, 0x57000000,
    0x43414C4C, 0x5F494E44, 0x00000000, 0x504C5547, 0x494E5F57, 0x00000000, 0x43414C4C, 0x5F464152,
    0x00000000, 0x43414C4C, 0x5F464152, 0x5F570000, 0x4745545F, 0x4F435F57, 0x00000000, 0x47455454,
    0x45525F57, 0x00000000, 0x53455454, 0x45525F57, 0x00000000, 0x66756E63, 0x74696F6E, 0x00000000,
    0x43436F6C, 0x694F626A, 0x43616C6C, 0x00000000, 0x43436F6C, 0x69526573, 0x43616C6C, 0x00000000,
};

// [.sdata] 0x806638A8-0x80663A90 (0x1E8 = 488B): 8-byte u32 pairs
extern "C" u32 lbl_sd_00[2] = { (u32)&lbl_eu_80523E70, (u32)&lbl_eu_8056DCB8 };
extern "C" u32 lbl_sd_02[2] = { (u32)&lbl_eu_80523E84, (u32)&lbl_eu_8056DCCC };
extern "C" u32 lbl_sd_04[2] = { (u32)&lbl_eu_80523E98, (u32)&lbl_eu_8056DCF8 };
extern "C" u32 lbl_sd_06[2] = { (u32)&lbl_eu_80523EA8, (u32)&lbl_eu_8056DD28 };
extern "C" u32 lbl_sd_08[2] = { (u32)&lbl_eu_80523EB8, (u32)&lbl_eu_8056DD58 };
extern "C" u32 lbl_sd_10[2] = { (u32)&lbl_eu_8066A8D0, (u32)&lbl_eu_8066A8D8 };
extern "C" u32 lbl_sd_12[2] = { (u32)&lbl_eu_80523ED8, 0x00000000 };
extern "C" u32 lbl_sd_14[2] = { (u32)&lbl_eu_80523EF4, 0x00000000 };
extern "C" u32 lbl_sd_16[2] = { 0x5265734D, 0x61740000 };
extern "C" u32 lbl_sd_18[2] = { 0x72656600, 0x52657354 };
extern "C" u32 lbl_sd_20[2] = { 0x65760000, 0x72656600 };
extern "C" u32 lbl_sd_22[2] = { 0x5265734D, 0x646C0000 };
extern "C" u32 lbl_sd_24[2] = { 0x72656600, 0x72656600 };
extern "C" u32 lbl_sd_26[2] = { 0x5265734E, 0x6F646500 };
extern "C" u32 lbl_sd_28[2] = { 0x72656600, 0x72656600 };
extern "C" u32 lbl_sd_30[2] = { 0x72656600, 0x72656600 };
extern "C" u32 lbl_sd_32[2] = { 0x52657344, 0x69630000 };
extern "C" u32 lbl_sd_34[2] = { 0x72656600, 0x00000000 };
extern "C" u32 lbl_sd_36[2] = { (u32)&lbl_eu_80523F18, (u32)&lbl_eu_8056E52C };
extern "C" u32 lbl_sd_38[2] = { (u32)&lbl_eu_80523F28, (u32)&lbl_eu_8056E54C };
extern "C" u32 lbl_sd_40[2] = { (u32)&lbl_eu_80523F3C, 0x00000000 };
extern "C" u32 lbl_sd_42[2] = { (u32)&lbl_eu_80523F58, 0x00000000 };
extern "C" u32 lbl_sd_44[2] = { (u32)&lbl_eu_80523F68, (u32)&lbl_eu_8066A930 };
extern "C" u32 lbl_sd_46[2] = { (u32)&lbl_eu_8066A938, 0x3D03126F };
extern "C" u32 lbl_sd_48[2] = { 0x72656600, 0x72656600 };
extern "C" u32 lbl_sd_50[2] = { 0x72656600, 0x00000000 };
extern "C" u32 lbl_sd_52[2] = { (u32)&lbl_eu_8066A9D8, 0x00000000 };
extern "C" u32 lbl_sd_54[2] = { (u32)&lbl_eu_80523FA0, (u32)&lbl_eu_8056E67C };
extern "C" u32 lbl_sd_56[2] = { (u32)&lbl_eu_8066A9E0, (u32)&lbl_eu_8056E6B0 };
extern "C" u32 lbl_sd_58[2] = { (u32)&lbl_eu_80523FAC, (u32)&lbl_eu_8056E6DC };
extern "C" u32 lbl_sd_60[2] = { (u32)&lbl_eu_80523FC0, 0x00000000 };
extern "C" u32 lbl_sd_62[2] = { (u32)&lbl_eu_80523FD0, (u32)&lbl_eu_8056E7C4 };
extern "C" u32 lbl_sd_64[2] = { 0xFFFFFFFF, 0xFFFFFFFF };
extern "C" u32 lbl_sd_66[2] = { (u32)&lbl_eu_80524010, (u32)&lbl_eu_8056E7E8 };
extern "C" u32 lbl_sd_68[2] = { 0x72656600, 0x72656600 };
extern "C" u32 lbl_sd_70[2] = { (u32)&lbl_eu_80524030, 0x00000000 };
extern "C" u32 lbl_sd_72[2] = { (u32)&lbl_eu_80524050, (u32)&lbl_eu_8056E88C };
extern "C" u32 lbl_sd_74[2] = { (u32)&lbl_eu_80524064, 0x00000000 };
extern "C" u32 lbl_sd_76[2] = { (u32)&lbl_eu_80524078, (u32)&lbl_eu_8056E8AC };
extern "C" u32 lbl_sd_78[2] = { 0x72656600, 0x72656600 };
extern "C" u32 lbl_sd_80[2] = { (u32)&lbl_eu_805240A0, (u32)&lbl_eu_8056E9FC };
extern "C" u32 lbl_sd_82[2] = { (u32)&lbl_eu_805240AC, 0x00000000 };
extern "C" u32 lbl_sd_84[2] = { (u32)&lbl_eu_805240B8, (u32)&lbl_eu_8056EA2C };
extern "C" u32 lbl_sd_86[2] = { (u32)&lbl_eu_805240C4, 0x00000000 };
extern "C" u32 lbl_sd_88[2] = { (u32)&lbl_eu_805240D8, 0xFFFFFFFF };
extern "C" u32 lbl_sd_90[2] = { (u32)&lbl_eu_805240F8, (u32)&lbl_eu_8056EAC8 };
extern "C" u32 lbl_sd_92[2] = { (u32)&lbl_eu_8066AB70, 0x00000000 };
extern "C" u32 lbl_sd_94[2] = { (u32)&lbl_eu_80524108, (u32)&lbl_eu_8056EB40 };
extern "C" u32 lbl_sd_96[2] = { (u32)&lbl_eu_8052411C, 0x00000000 };
extern "C" u32 lbl_sd_98[2] = { 0x72656600, 0x72656600 };
extern "C" u32 lbl_sd_100[2] = { (u32)&lbl_eu_80524128, 0x00000000 };
extern "C" u32 lbl_sd_102[2] = { (u32)&lbl_eu_80524138, 0x00000000 };
extern "C" u32 lbl_sd_104[2] = { (u32)&lbl_eu_80524148, (u32)&lbl_eu_8056EB94 };
extern "C" u32 lbl_sd_106[2] = { (u32)&lbl_eu_80524190, 0x00000000 };
extern "C" u32 lbl_sd_108[2] = { (u32)&lbl_eu_805241E0, (u32)&lbl_eu_8056EBB8 };
extern "C" u32 lbl_sd_110[2] = { (u32)&lbl_eu_805241F8, 0x00000000 };
extern "C" u32 lbl_sd_112[2] = { (u32)&lbl_eu_80524218, 0x00000000 };
extern "C" u32 lbl_sd_114[2] = { (u32)&lbl_eu_80524228, 0x00000000 };
extern "C" u32 lbl_sd_116[2] = { (u32)&lbl_eu_80524238, 0x00000000 };
extern "C" u32 lbl_sd_118[2] = { (u32)&lbl_eu_80524248, (u32)&lbl_eu_8056ECB0 };
extern "C" u32 lbl_sd_120[2] = { 0x72656600, 0x00000000 };


// [.bss] 0x80658658-0x80658680 (0x28 = 40B): camera default-parameter table.
SCamDefaultTable lbl_eu_80658658;
