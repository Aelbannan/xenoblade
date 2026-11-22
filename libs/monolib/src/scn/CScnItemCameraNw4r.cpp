// Translation unit: monolib/src/scn/CScnItemCameraNw4r
// CScnItemCameraNw4r - scene item camera with an nw4r frustum.
//
// Class layout: inherits CScnItemCamera (0x2B8 bytes), appends an nw4r
// FRUSTUM at 0x2B8. The class is defined in this TU because the shared
// header copy (include/monolib/scn) is handled elsewhere.
//
// Vtable: lbl_eu_8056DC90

#include <monolib/scn/CScnItemCamera.hpp>
#include <monolib/device/CDeviceVI.hpp>
#include <nw4r/math/math_geometry.h>

class CScnItemCameraNw4r : public CScnItemCamera {
public:
    // Ctor is the free function __ct__CScnItemCameraNw4r (see below).
    virtual ~CScnItemCameraNw4r();

    nw4r::math::FRUSTUM mFrustum; // 0x2B8
};

// Base-class camera refresh helpers (retail ScnCamRebuild / ScnCamViewReg).
extern "C" void ScnCamRebuild(CScnItemCameraNw4r* self);
extern "C" void ScnCamViewReg(CScnItemCameraNw4r* self);

// ===========================================================================
// Constructor - free-function form (retail __ct__CScnItemCameraNw4r). Calls
// the flat base ctor then installs the Nw4r vtable.
// ===========================================================================
extern "C" CScnItemCamera* __ct__CScnItemCamera(CScnItemCamera* obj, CScnItemCamera* parent,
                                               const char* name);
extern "C" u32 lbl_eu_8056DC90[];
extern "C" void* __ct__CScnItemCameraNw4r(CScnItemCamera* self, void* parent,
                                         const char* name) {
    // Return ths after the vtable store so MWCC keeps r3=self (or r31,r31)
    // and materialises the vtable address in r4 -- matching retail lis/or/addi/stw.
    CScnItemCamera* ths = self;
    __ct__CScnItemCamera(ths, (CScnItemCamera*)parent, name);
    *(u32**)ths = lbl_eu_8056DC90;
    return ths;
}

// ===========================================================================
// Destructor - base dtor, then optional delete.
// ===========================================================================
CScnItemCameraNw4r::~CScnItemCameraNw4r() {}

// ===========================================================================
// Scalar deleting destructor for CScnItemCamera (retail __dt__80482054).
// Only frees the memory if a deletion flag is set; otherwise it's a plain
// non-deleting dtor call target.
// ===========================================================================
extern "C" void* __dt__80482054(u8* self, int deleting) {
    if (self != 0 && deleting > 0)
        operator delete(self);
    return self;
}

// ===========================================================================
// ScnCamNw4r_RefreshFrustumA (vtable slot 9) - rebuild the nw4r camera frustum.
// ===========================================================================
void ScnCamNw4r_RefreshFrustumA(CScnItemCameraNw4r* self) {
    // Refresh camera internals in the base object first.
    ScnCamRebuild(self);

    f32 farZ = self->mFarZ;
    f32 nearZ = self->mNearZ;
    f32 aspect = self->mAspectRatio;
    f32 fovY = self->mFovY;

    self->mFrustum.Set(fovY, aspect * CDeviceVI::getWidthScale(), nearZ, farZ,
                       *self->mTransform.mLocalMatInv);
}

// ===========================================================================
// ScnCamNw4r_RefreshFrustumB (vtable slot 5) - rebuild the nw4r camera frustum.
// ===========================================================================
void ScnCamNw4r_RefreshFrustumB(CScnItemCameraNw4r* self) {
    // Refresh camera internals in the base object first.
    ScnCamViewReg(self);

    f32 farZ = self->mFarZ;
    f32 nearZ = self->mNearZ;
    f32 aspect = self->mAspectRatio;
    f32 fovY = self->mFovY;

    self->mFrustum.Set(fovY, aspect * CDeviceVI::getWidthScale(), nearZ, farZ,
                       *self->mTransform.mLocalMatInv);
}

// ===========================================================================
// Keep the remaining (unmatched) unit symbols as stubs.
// ===========================================================================
extern "C" bool ScnCamNw4r_TestSphereHit(CScnItemCameraNw4r* self,
                                        const nw4r::math::SPHERE* sphere) {
    return self->mFrustum.IntersectSphere(sphere);
}
extern "C" void* ScnCamNw4r_NullHook40(CScnItemCameraNw4r* self) { return 0; }
void ScnCamNw4r_EmptyHook48() {}
void* ScnCamNw4r_GetField10Ptr(u8* self) {
    CScnItemCamera* cam = static_cast<CScnItemCamera*>(static_cast<void*>(self));
    return cam->mName;
}