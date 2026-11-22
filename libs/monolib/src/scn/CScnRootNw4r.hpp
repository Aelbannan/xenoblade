#pragma once

#include <types.h>
#include <nw4r/g3d/g3d_scnroot.h>
#include <nw4r/g3d/g3d_init.h>
#include <nw4r/g3d/g3d_anmscn.h>
#include <nw4r/db/db_assert.h>
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/scn/ICulling.hpp"
#include "monolib/core/code_804E36DC.hpp"
#include "monolib/scn/CScnRoot.hpp"
#include "monolib/scn/CScn.hpp"
#include <revolution/MEM.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__8049A0D0(void* self, int flag);

class CScnRootNw4r;  // fwd (imports below reference it)

// Opaque resource owned at CScnRootNw4r::field_0x14 (torn down by the
// retail deleting-dtor helper __dt__8049A0D0).
struct CScnRootNw4rRes;

/*
 * CScnRootNw4r - monolib nw4r scene-root wrapper.
 *
 * Layout recovered from retail:
 *  - primary vtable (lbl_eu_8056E768) = RTTI + offset-to-top + 12 virtual
 *    slots; slot vf9 (retail scnImN4GetWord4AC) sits at vtable offset 0x2C and
 *    is the callee of RootNw4r_GetAllocHandle. The virtuals below are declared but
 *    never defined here (no key function -> no vtable emitted), purely so
 *    that call site compiles to the retail lwz/lwz/mtctr/bctrl sequence.
 *  - The real object derives from CScnRoot (+0x00) and
 *    nw4r::g3d::IScnObjCallback (+0x0C); the flat field layout below
 *    reproduces the same byte offsets.
 */

// Manager classes owned by the scene (CScnRoot::mpScn)
// (defined in their own TUs under libs/monolib/src/scn/).
class CScnCameraMan;
class CScnLightMan;
class CScnFogMan;
struct CScnEnvLgtCtrlResList;
class CScnItemPool;


// Camera blob passed to the CScnRootNw4r camera callbacks; the owning class
// is not yet identified (fields recovered from the retail accessors).
struct CScnCamLayout {
    u8 pad_0x0[0xC];                  // +0x00
    u32 field_0xC;                    // +0x0C camera id
    u8 pad_0x10[0xCC - 0x10];         // +0x10
    nw4r::math::MTX34 mCamMtx;        // +0xCC (SetCameraMtxDirectly)
    u8 pad_0xFC[0x194 - 0xFC];        // +0xFC
    nw4r::math::MTX44 mProjMtx;       // +0x194 (SetProjectionMtxDirectly)
    u8 pad_0x1D4[0x1470 - 0x1D4];     // +0x1D4
    nw4r::g3d::ScnObj* field_0x1470;  // +0x1470
    u8 pad_0x1474[0x147C - 0x1474];   // +0x1474
    nw4r::g3d::ScnObj* field_0x147C;  // +0x147C
    u8 pad_0x1480[0x16C0 - 0x1480];   // +0x1480
    s16 field_0x16C0;                 // +0x16C0 camera index
};

// Viewport rect produced by CGXCache::getViewRectGX (four s16 fields).
struct CGXCacheViewportRect {
    s16 x;       // +0x0
    s16 y;       // +0x2
    s16 width;   // +0x4
    s16 height;  // +0x6
};

// Scissor cache area inside CGXCache (fields not yet named in CGXCache.hpp).
struct CGXCacheScissorArea {
    u8 pad_0x0[0x4A0];                // +0x00
    s16 field_0x4A0;                  // +0x4A0
    s16 field_0x4A2;                  // +0x4A2
    s16 field_0x4A4;                  // +0x4A4
    s16 field_0x4A6;                  // +0x4A6
};

// Pre-mangled retail member symbol (CGXCache::getViewRectGX) - returns the
// current viewport rect. The shared CGXCache.hpp still declares it as a void
// stub, so this TU declares the real shape under the retail mangled name.
CGXCacheViewportRect* getViewRectGX__8CGXCacheFv(CGXCache* cache);

// Callback object stored in the (callback, id) pair table (mArrA).
// Slot +0xC is dispatched by RootNw4r_DispatchFrame each frame.
struct CScnPairCb {
    virtual ~CScnPairCb() {}          // +0x8
    virtual void vf1(u32 value) = 0;  // +0xC
};

// Non-virtual scene-root helpers (retail C-linkage names; also called from
// CScnItemModelNw4r.cpp).
extern "C" s32 RootNw4r_AttachCamOne(CScnRootNw4r* self, CScnCamLayout* cam);
extern "C" s32 RootNw4r_DetachCamChild(CScnRootNw4r* self, CScnCamLayout* cam);
extern "C" void RootNw4r_DrawDispatch(CScnRootNw4r* self, int flag);

// Unmangled monolib callees (retail C-linkage names).
extern "C" void Scn_SetCurrentScene(u32 value);
extern "C" void scnLgtBindCamera(CScnEnvLgtCtrlResList* ctrl);
extern "C" void scnLgtUpdateSceneLights(CScnEnvLgtCtrlResList* ctrl);
extern "C" void scnLgtEnterMode20(CScnEnvLgtCtrlResList* ctrl);
// (CamMan_RefreshCurrentCamera is the CScnCameraMan member refreshCurrentCamera)
extern "C" void LightManInvokeLightV3(CScnLightMan* man);
extern "C" void FogManIsFogEnabled(CScnFogMan* man);

extern "C" void resetGXStateA__8CGXCacheFv(CGXCache* cache);
extern "C" void updateViewRoot__9CViewRootFv();

// Panic strings for the ScnGroup bounds check (same file/format as CScnMem).
extern const char lbl_eu_8056E720[];
extern const char lbl_eu_8056E6F8[];

// Bloom post-effect owned at CScnRootNw4r::field_0x14 (defined in
// libs/monolib/src/scn/CScnBloom.cpp).
class CScnBloom;
extern "C" void CScnBloom_RenderFullRes(CScnBloom* bloom);
extern "C" void CScnBloom_RenderCached(CScnBloom* bloom);
extern "C" void CScnBloom_RenderHalfRes(CScnBloom* bloom);

// Scene-item model pool item drawn through the kind-1 draw list.
class CScnItemModel;
extern "C" int simSyncBuf824Flags(CScnItemModel* item, int param);

// Kind-1 sub-pool list returned by the scene-item-pool accessor.
extern "C" void* CScnItemPool_lookupSubPool(void* pool, int kind);
extern "C" int MaruShadowSetupTexturePipe(void* scene);

// Per-model draw hook object stored at CScnItemModel+0x7EC. Dispatched at
// vtable offset 0x10 during the opaque-draw pass.
struct CScnModelDrawHook {
    virtual ~CScnModelDrawHook() {}  // +0x08
    virtual void unk_0C();           // +0x0C
    virtual void unk_10();           // +0x10 dispatched
};

// Tail view of CScnItemModel exposing the +0x7EC hook pointer (the shared
// header types that field as u32).
struct CScnItemModelHooks {
    u8 pad_0x0[0x7EC];        // 0x00..0x7EC
    CScnModelDrawHook* hook;  // 0x7EC
};

// _reslist_node/_reslist_base view of the pool's kind-1 draw list
// (links at +0x0/+0x4, item pointer at +0x8; sentinel pointer at +0x4).
struct CScnDrawNode {
    CScnDrawNode* mNext;          // 0x00
    CScnDrawNode* mPrev;          // 0x04
    CScnItemModelHooks* mItem;    // 0x08
};

struct CScnDrawList {
    u8 pad_0x0[4];                // 0x00 (vtable)
    CScnDrawNode* mStartNodePtr;  // 0x04 sentinel node
};

// CScnRootNw4r: the nw4r scene root (size 0x4EC). Bases: CScnRoot (+0x00)
// and nw4r::g3d::IScnObjCallback (+0x0C, the ScnGroup callback target).
// Its CALC_WORLD override gets the weak @12@ thunk in the secondary table and
// a real slot appended at the end of __vt__12CScnRootNw4r.
class CScnRootNw4r : public CScnRoot, nw4r::g3d::IScnObjCallback {
public:
    struct GroupAlloc {
        u32 mHandle;                  // +0x0
        MEMAllocatorFuncs mFuncs;     // +0x4
    };

    CScnRootNw4r(CScn* scn, void* param);
    virtual ~CScnRootNw4r();
    virtual void RemoveId(u32 id);
    virtual void ApplyCamera(CScnCamLayout* cam);
    virtual void UpdateFrame();
    virtual void DrawFrame();
    virtual void AttachQueued();
    virtual void AddPair(u32 cb, u32 id);
    virtual void RemovePair(u32 cb, s32 id);
    virtual u32 getGroupHandle();
    virtual u32 getBaseHandle() { return mGroup[0].mHandle; }
    virtual u32 getAllocHandle();
    virtual CScnBloom* getBloom();
    virtual void ExecCallback_CALC_WORLD(nw4r::g3d::ScnObj::Timing timing,
                                         nw4r::g3d::ScnObj* obj, u32 param,
                                         void* info);

    nw4r::g3d::ScnRoot* mScnRoot;     // +0x10 (nw4r scene root)
    CScnBloom* mBloom;                // +0x14 (bloom post-effect)
    u8 mFrameDone;                    // +0x18 (frame-updated flag)
    u8 mInBloomPass;                  // +0x19 (draw-in-progress flag)
    u8 mRedrawReq;                    // +0x1A (redraw-request latch)
    u32 mFrame;                       // +0x1C (frame counter / pair id)
    u32 mArrA[4];                     // +0x20 (pair table: callbacks)
    s32 mArrB[4];                     // +0x30 (pair table: ids)
    u32 mPairCount;                   // +0x40
    u32 mIds[256];                    // +0x44 (queued camera layouts)
    u32 mIdCount;                     // +0x444
    u8 mAllocTail;                    // +0x448 (allocate from the region tail)
    MEMAllocator mAlloc[6];           // +0x44C
    GroupAlloc mGroup[4];             // +0x4AC
    MEMAllocatorFuncs mFuncs[2];      // +0x4DC
};
