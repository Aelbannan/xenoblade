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
    CScnItemCameraNw4r();
    virtual ~CScnItemCameraNw4r();

    // Virtual override for base vtable slot 5 (retail func_80481F9C).
    virtual void buildFrustumNw4r();
    // Virtual override for base vtable slot 9 (retail func_80481F00).
    virtual void updateFrustumNw4r();

private:
    // Base-class helpers (retail func_8049EBF0 / func_8049F8E4).
    void baseUpdateA();
    void baseUpdateB();

public:
    nw4r::math::FRUSTUM mFrustum; // 0x2B8
};

// ===========================================================================
// Constructor - calls the base ctor, then installs the derived vtable.
// ===========================================================================
CScnItemCameraNw4r::CScnItemCameraNw4r() : CScnItemCamera() {}

// ===========================================================================
// Destructor - base dtor, then optional delete.
// ===========================================================================
CScnItemCameraNw4r::~CScnItemCameraNw4r() {}

// ===========================================================================
// func_80481F00 (vtable slot 9) - rebuild the nw4r camera frustum.
// ===========================================================================
void CScnItemCameraNw4r::updateFrustumNw4r() {
    // Refresh camera internals in the base object first.
    baseUpdateA();

    f32 farZ = mFarZ;
    f32 nearZ = mNearZ;
    f32 aspect = mAspectRatio;
    f32 fovY = mFovY;

    aspect *= CDeviceVI::getWidthScale();

    mFrustum.Set(fovY, aspect, nearZ, farZ, *mTransform.mLocalMatInv);
}

// ===========================================================================
// func_80481F9C (vtable slot 5) - rebuild the nw4r camera frustum.
// ===========================================================================
void CScnItemCameraNw4r::buildFrustumNw4r() {
    // Refresh camera internals in the base object first.
    baseUpdateB();

    f32 farZ = mFarZ;
    f32 nearZ = mNearZ;
    f32 aspect = mAspectRatio;
    f32 fovY = mFovY;

    aspect *= CDeviceVI::getWidthScale();

    mFrustum.Set(fovY, aspect, nearZ, farZ, *mTransform.mLocalMatInv);
}

// ===========================================================================
// Keep the remaining (unmatched) unit symbols as stubs.
// ===========================================================================
void func_80482038(void* self) {}
void func_80482040(void* self) {}
void func_80482048() {}
void* func_8048204C(void* self) { return (void*)((u8*)self + 0x10); }
void __dt__80482054() {}