#pragma once

#include <types.h>
#include <revolution/MTX.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/work/CWorkThread.hpp"

// Camera item in the scene pool's camera list (pool kind 4 via CScnItemPool_lookupSubPool).
// Head layout mirrors CScnItemCamera: vtable at +0x00, mIndex (camera id) at +0x0C.
struct CScnCameraItem {
    virtual void vf0() = 0;   // vtable slot 0 (+0x04)
    virtual void vf1() = 0;   // vtable slot 1 (+0x0C)
    virtual void vf2() = 0;   // vtable slot 2 (+0x10)
    virtual void vf3() = 0;   // vtable slot 3 (+0x14) - invoked by refreshCurrentCamera()

    u32 mParent;   // +0x04
    u16 mType;     // +0x08
    s32 mIndex;    // +0x0C - camera id / index into CScnCameraMan::mViews
    u8 _10[0xBC];           // +0x10 .. +0xCC
    Mtx mMtx;               // +0xCC - view matrix
    u8 _FC[0x10];           // +0xFC .. +0x10C
    ml::CVec3 mPos;         // +0x10C - world position
};

// Node in the camera circular list: next link at +0x00, item at +0x08.
struct CScnCameraNode {
    CScnCameraNode* next;   // +0x00
    u32 _04;                // +0x04
    CScnCameraItem* item;   // +0x08
};

// List handle returned by the scene-item-pool accessor (CScnItemPool_lookupSubPool, kind 4);
// the circular-list sentinel lives at +0x04.
struct CScnCameraList {
    u32 _00;                  // +0x00
    CScnCameraNode* sentinel; // +0x04
};

// Parameter object for the camera manager: view id at +0x4C, pool at +0x60.
// The opaque field at +0x54 is handed to the view release/attach callbacks
// (enqueueContextMsg and the vtable+0xB8 slot) as a pointer.
struct CScnCameraParam {
    u8 _00[0x4C];         // +0x00
    u32 mViewId;          // +0x4C - current view id
    u8 _50[4];            // +0x50
    u32 field_0x54;       // +0x54 - opaque; its address is passed to view callbacks
    u8 _58[8];            // +0x58 .. +0x60
    u8* mPool;            // +0x60 - scene item pool
};

// Event payload object handed to WorkEvent1: indexed accessors resolve slots
// in an array at +0x1104 (defined with C linkage in work/CEvent1.cpp).
struct CEvent1 {
    u8 _00[0x1104];
    void* mPtrArray[1];   // +0x1104 - indexed value slots
};

// Retail symbols keep their Fv mangling because the defining TU declares them
// with C linkage.
extern "C" f32 getFloatParam__7CEvent1Fv(CEvent1* self, int index);
extern "C" void* getPtrParam__7CEvent1Fv(CEvent1* self, int index);

// Camera-item fields written by WorkEvent1 (overlay over CScnItemCamera;
// the read-only header stops before these offsets).
class CScnItemCamera;
struct CScnItemCameraCamTail {
    u8 _00[0x60];
    u32 field_0x60;       // 0x60 - rotation euler x (raw bits)
    u32 field_0x64;       // 0x64 - rotation euler y (raw bits)
    u32 field_0x68;       // 0x68 - rotation euler z (raw bits)
    u8 _6C[0x1E0 - 0x6C];
    f32 mFovY;            // 0x1E0
    f32 mUnk1E4;          // 0x1E4
    f32 mAspectRatio;     // 0x1E8
    f32 mNearZ;           // 0x1EC
    f32 mFarZ;            // 0x1F0
};

// Camera-item helpers defined in CScnItemCamera.cpp.
void ScnCamDepthUp(CScnItemCamera* self);
void ScnCamPlaceAt(CScnItemCamera* self, const ml::CVec3* v);
void ScnCamSetDist(CScnItemCamera* self, f32 dist);

// Work event payload for WorkEvent3 (event id 0x15 = camera select).
struct CScnCameraEvent {
    s32 mId;        // +0x00
    void* pData;    // +0x04
    u32 dataSize;   // +0x08
};

// C-ABI scene-item-pool accessor (defined with C linkage in CScnItemPool.cpp):
// returns the pool slot (list) for the given item kind (4 = cameras). The
// definition returns void*; call sites apply the CScnCameraList view.
extern "C" void* CScnItemPool_lookupSubPool(void* pool, int kind);

// Scene camera manager: keeps a camera-id -> view-id table and resolves the
// current camera through the scene pool's camera item list.
class CScnCameraMan : public IWorkEvent {
public:
    CScnCameraMan(CScnCameraParam* param);
    virtual ~CScnCameraMan();
    virtual bool WorkEvent1(UNKTYPE* r4, const char* r5);
    virtual bool WorkEvent3(UNKTYPE* r4);

    // View registration / lookup helpers (out of line; retail keeps one copy
    // of each in this TU).
    void swapViewParam(CView* view, u32 idx);
    bool syncCameraToView();
    void refreshCurrentCamera();
    CWorkThread* getViewThread(s32 id);
    CScnCameraItem* findItem(s32 id);
    CScnCameraItem* getItem(s32 id);
    CScnCameraItem* findViewItem(CScnCameraParam* param);
    CScnCameraItem* getViewItem(CScnCameraParam* param);

    // +0x00: vtable (compiler-managed)
    CScnCameraParam* mParam;   // +0x04
    u32 mViews[10];            // +0x08 - camera id -> view id table
    u32 mCount;                // +0x30
    s32 mCamId;                // +0x34
};

class CScnItemCamera;
// Scene-item pool host whose +0x60 field is the pool (see CScnItemCamera.cpp).
struct CScnCameraItemHost;
// Camera-manager factory: creates camera `idx` in the pool (defined in
// CScnItemCamera.cpp).
CScnItemCamera* ScnCamFactory(CScnCameraItemHost* self, int idx);

// Retail CScriptCode line parser. The retail symbol is the Fv (no-arg) mangled
// name but the function takes (self, pData, dataSize) - CScriptCode.cpp defines
// it with C linkage and the exact name, so the call site must too.
extern "C" void parseLines__11CScriptCodeFv(void* self, u8* pData, u32 dataSize);

// Tail mirror of CGXCache: the read-only CGXCache.hpp header stops before the
// scissor-rect fields, so the s16 scissor deltas used by the camera aspect
// computation are declared here (cacheInstance + 0x4BC / + 0x4BE).
struct CGXCacheTail {
    u8 _00[0x4BC];
    s16 mScissorDeltaX;  // +0x4BC
    s16 mScissorDeltaY;  // +0x4BE
};

// Overlay of the CScnItemCamera fields used by the screen-projection helper
// CamMan_ProjectToScreen: view matrix at +0xCC, a 13-float coefficient block at +0x150
// (three plane rows plus one trailing float), s16 screen offsets at +0x1D8,
// and the aspect/near-plane tail.
struct CScnItemCameraProject {
    u8 _00[0xCC];
    Mtx mViewMtx;      // +0xCC - camera-space transform
    u8 _FC[0x54];
    f32 mProj[13];     // +0x150 .. +0x180
    u8 _184[0x54];
    s16 mOffsetX;      // +0x1D8
    s16 mOffsetY;      // +0x1DA
    u8 _1DC[0xC];
    f32 mAspect;       // +0x1E8
    f32 mNearZ;        // +0x1EC
};

// Volatile view of the CScnItemCamera frustum tail: volatile reads pin the
// depth loads at their program position (top of CamMan_BuildPerspective) so MWCC parks
// them in f31/f30 across the width-scale call, like retail's prologue. The
// load encoding (lfs off(rN)) is identical to a plain read.
struct CScnItemCameraFrustumVt {
    u8 _00[0x1E0];
    volatile f32 mFovY;   // +0x1E0
    f32 mUnk1E4;          // +0x1E4
    f32 mAspectRatio;     // +0x1E8
    volatile f32 mNearZ;  // +0x1EC
    volatile f32 mFarZ;   // +0x1F0
};
