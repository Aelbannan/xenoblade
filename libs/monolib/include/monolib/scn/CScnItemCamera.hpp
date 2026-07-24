#pragma once

#include <types.h>
#include <monolib/math.hpp>

/**
 * Subobject stored in arrays at mSubObjArray1 and mSubObjArray2.
 * Element size: 0x10 (16 bytes).
 * Has a non-trivial destructor (__dt__8049EA9C) but trivial constructor.
 * The trivial constructor is the no-op function func_8049EA98 (just blr).
 */
struct CScnItemCameraSubObj {
    // TODO: Decompile subobject layout (0x10 bytes)
    u8 data[0x10];
};

/**
 * CScnItemCamera - Scene item camera (size: 0x2B8)
 *
 * Camera item within the scene graph. Manages camera parameters,
 * viewport settings, and two arrays of subobjects.
 *
 * Vtable: lbl_eu_8056ECC0 (0x28 bytes, 10 entries)
 * Base class destructor: __dt__80482054
 */
class CScnItemCamera {
public:
    CScnItemCamera();
    virtual ~CScnItemCamera();

    /**
     * Virtual function at vtable+0x0C.
     * No-op in this class (func_8049F9A4 - just blr).
     * Overridden by base class default.
     */
    virtual void vfunc_0C();

    // vtable at 0x00 (lbl_eu_8056ECC0)
    u32 mParent;                                 // 0x04 - parent/owner pointer (type unknown)
    u16 mType;                                   // 0x08 = 4
    s32 mIndex;                                  // 0x0C = -1
    char mName[0x40];                            // 0x10 - string name
    u32 mNameLen;                                // 0x50 - strlen of mName

    ml::CAttrTransform mTransform;               // 0x54 - embedded transform (0xE0 bytes)
    // mTransform.mFlags at 0x134: init 0, then ORed with FLAG_USE_ZXY (0x8)

    // Camera depth/range parameters (0x138-0x154)
    f32 mCamParam0;                              // 0x138
    f32 mCamParam1;                              // 0x13C
    f32 mCamParam2;                              // 0x140
    f32 mDepthNear;                              // 0x144
    f32 mDepthMid;                               // 0x148
    f32 mDepthFarNear;                           // 0x14C
    f32 mDepthFar;                               // 0x150

    // Projection matrix written by CGXCache::func_8044BB20 (0x154-0x1D4)
    // 0x40 bytes = 4x4 f32 matrix
    f32 mProjMtx[4][4];                          // 0x154

    // Viewport dimensions
    u16 mViewportX;                              // 0x1D4 = 0
    u16 mViewportY;                              // 0x1D6 = 0
    u16 mViewportWidth;                          // 0x1D8 - from getRenderModeObj
    u16 mViewportHeight;                         // 0x1DA - from getRenderModeObj
    u8 mUnk1DC;                                  // 0x1DC = 0

    u8 pad_1DD[3];                               // alignment

    // Additional camera parameters
    f32 mFovY;                                   // 0x1E0 - vertical FOV
    f32 mAspectRatio;                            // 0x1E8 - computed aspect ratio
    f32 mNearZ;                                  // 0x1EC
    f32 mFarZ;                                   // 0x1F0
    f32 mUnk1F4;                                 // 0x1F4

    // Subobject arrays - each is 6 elements of 0x10 bytes
    // Constructed with no-op constructor (func_8049EA98) and
    // __dt__8049EA9C destructor
    CScnItemCameraSubObj mSubObjArray1[6];       // 0x1F8 (6 * 0x10 = 0x60 bytes)
    CScnItemCameraSubObj mSubObjArray2[6];       // 0x258 (6 * 0x10 = 0x60 bytes)
    // end: 0x2B8
};
