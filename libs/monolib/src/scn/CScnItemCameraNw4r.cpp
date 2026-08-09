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

    nw4r::math::FRUSTUM mFrustum; // 0x2B8
};

// Base-class camera refresh helpers (retail func_8049EBF0 / func_8049F8E4).
extern "C" void func_8049EBF0(CScnItemCameraNw4r* self);
extern "C" void func_8049F8E4(CScnItemCameraNw4r* self);

//
// The base class header (monolib/scn/CScnItemCamera.hpp) is a draft whose
// member types do not actually lay out to the retail offsets, so matching the
// retail frustum builders would read the wrong fields. This overlay gives the
// object the true retail layout: mLocalMatInv at 0xCC (inside CAttrTransform
// at 0x54), the near/far view params at 0x1E0-0x1F0, and the nw4r FRUSTUM
// appended at 0x2B8. CScnItemCameraNw4r inherits CScnItemCamera so `this`
// points at the same base, and this struct simply re-describes that memory.
//
struct CScnItemCameraNw4rLayout {
    u8 pad_0xCC[0xCC];
    ml::CMat34 mLocalMatInv;    // 0xCC
    u8 pad_0xFC[0xE4];          // 0xFC -> 0x1E0
    f32 mFovY;                  // 0x1E0
    u32 unk_0x1E4;              // 0x1E4
    f32 mAspect;                // 0x1E8
    f32 mNearZ;                 // 0x1EC
    f32 mFarZ;                  // 0x1F0
    u8 pad_0x1F4[0xC4];         // 0x1F4 -> 0x2B8
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
// func_80481F00 (vtable slot 9) - rebuild the nw4r camera frustum.
// ===========================================================================
void func_80481F00(CScnItemCameraNw4r* self) {
    CScnItemCameraNw4rLayout* cam = (CScnItemCameraNw4rLayout*)self;

    // Refresh camera internals in the base object first.
    func_8049EBF0(self);

    f32 farZ = cam->mFarZ;
    f32 nearZ = cam->mNearZ;
    f32 aspect = cam->mAspect;
    f32 fovY = cam->mFovY;

    cam->mFrustum.Set(fovY, aspect * CDeviceVI::getWidthScale(), nearZ, farZ,
                      *cam->mLocalMatInv);
}

// ===========================================================================
// func_80481F9C (vtable slot 5) - rebuild the nw4r camera frustum.
// ===========================================================================
void func_80481F9C(CScnItemCameraNw4r* self) {
    CScnItemCameraNw4rLayout* cam = (CScnItemCameraNw4rLayout*)self;

    // Refresh camera internals in the base object first.
    func_8049F8E4(self);

    f32 farZ = cam->mFarZ;
    f32 nearZ = cam->mNearZ;
    f32 aspect = cam->mAspect;
    f32 fovY = cam->mFovY;

    cam->mFrustum.Set(fovY, aspect * CDeviceVI::getWidthScale(), nearZ, farZ,
                      *cam->mLocalMatInv);
}

// ===========================================================================
// Keep the remaining (unmatched) unit symbols as stubs.
// ===========================================================================
void func_80482038(u8* self) {}
extern "C" void* func_80482040(u8* self) { return 0; }
void func_80482048() {}
void* func_8048204C(u8* self) { return (void*)((u8*)self + 0x10); }