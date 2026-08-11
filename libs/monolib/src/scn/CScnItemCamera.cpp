// Translation unit: monolib/src/scn/CScnItemCamera
// Scene item camera class implementation.
//
// Status: Only func_8049EA98 and func_8049F9A4 are FULL_MATCH.
//         Remaining functions are NOT_STARTED stubs.

#include <monolib/scn/CScnItemCamera.hpp>
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
    if (obj != nullptr) {
        __destroy_arr(&obj->mSubObjArray2[0],
                      reinterpret_cast<ConstructorDestructor*>(&__dt__8049EA9C),
                      0x10, 6);
        __destroy_arr(&obj->mSubObjArray1[0],
                      reinterpret_cast<ConstructorDestructor*>(&__dt__8049EA9C),
                      0x10, 6);
        if (deleting > 0) {
            operator delete(obj);
        }
    }
    return obj;
}

// Camera fields the draft header does not lay out correctly:
// the transform position at 0x54 is stored as raw u32 bits by
// func_8049F168, and 0x1E4 (cot of the half FOV) is missing.
// This overlay re-describes the retail object; the class members
// above 0x138 already match the header, so only the deltas are
// spelled out here.
struct CScnItemCameraLayout {
    u8 pad_0x00[0x54];
    u32 mPosX;                  // 0x54 - mTransform.mPos.x (raw bits)
    u32 mPosY;                  // 0x58 - mTransform.mPos.y
    u32 mPosZ;                  // 0x5C - mTransform.mPos.z
    u8 pad_0x60[0x1E0 - 0x60];
    f32 mFovY;                  // 0x1E0
    f32 mUnk1E4;                // 0x1E4 - E / tan(half FOV)
    f32 mAspectRatio;           // 0x1E8
    f32 mNearZ;                 // 0x1EC
    f32 mFarZ;                  // 0x1F0
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

    f32 negNear = -near;
    self->mDepthMid = negNear;
    self->mDepthFarNear = near * cam->mAspectRatio;
    self->mDepthFar = negNear * cam->mAspectRatio;
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

void __ct__CScnItemCamera(){}

void func_8049EBF0(){}

void func_8049EFF8(){}

void func_8049F204(){}

void func_8049F6D4(){}

void func_8049F774(){}

void func_8049F824(){}

void func_8049F8E4(){}

void func_8049F9A8(){}
