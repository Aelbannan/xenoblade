// Translation unit: monolib/src/scn/CScnRootNw4r
// CScnRootNw4r virtuals (compiler-emitted __vt__12CScnRootNw4r with the
// IScnObjCallback secondary table and its weak @12@ thunks) and the nw4r
// MEMAllocator callbacks.

#include <harness_catalog.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include "libs/monolib/src/scn/CScnRootNw4r.hpp"  // CScnRootNw4r class + fields
#include "libs/monolib/src/scn/CScnCameraMan.hpp"  // refreshCurrentCamera
#include "monolib/util/MemManager.hpp"            // mtl::MemManager
#include "libs/monolib/src/scn/CScnBloom.hpp"
#include "libs/monolib/src/scn/CScnRoot.hpp"     // ctor param/region views, sdata2 constants
#include <stdlib.h>                                // labs

// --- CScnRootNw4r class ---

CScnRootNw4r::CScnRootNw4r(CScn* scn, void* param) : CScnRoot(scn) {
    mScnRoot = NULL;
    mBloom = NULL;
    mFrameDone = 0;
    mRedrawReq = 0;
    mFrame = 0;
    mPairCount = 0;
    mIdCount = 0;
    mAllocTail = 0;
    mGroup[0].mHandle = 0xFFFFFFFF;
    mGroup[1].mHandle = 0xFFFFFFFF;
    mGroup[2].mHandle = 0xFFFFFFFF;
    mGroup[3].mHandle = 0xFFFFFFFF;

    // Viewport overrides (default: 20.0f scale, 200 height, 160-wide groups).
    f32 scale = 20.0f;
    u32 vpHeight = 0xC8;
    u16 rect[10];
    for (u32 i = 0; i < 10; i++) {
        rect[i] = 0xA0;
    }

    if (param != NULL) {
        CScnRootNw4rParam* p = (CScnRootNw4rParam*)param;
        scale = p->field_0x8;
        vpHeight = p->field_0xC;
        for (u32 i = 0; i < 10; i++) {
            rect[i] = p->rect[i];
        }
    }

    // Size the scene-root buffers from the owning region's size.
    scale *= 0.01f;
    CScnRootMgrRegion* region = (CScnRootMgrRegion*)mpScn->unk88;
    s32 v = labs(region->field_0xC);

    s32 bufsz = ((s32)((f32)v * scale) & 0x1F)
                    ? (s32)((f32)v * scale) + 0x20 - ((s32)((f32)v * scale) & 0x1F)
                    : (s32)((f32)v * scale);
    s32 rem = v - bufsz;
    s32 rem2 = 0;
    if (rem >= 0x80000) {
        rem2 = (s32)((f32)rem * 0.08f);
        if (rem2 & 0xF) {
            rem2 = rem2 + 0x10 - (rem2 & 0xF);
        }
        rem -= rem2;
    }

    // Per-slot alloc regions: create from the region handle when the viewport
    // scale is positive, otherwise fall back to the MEM2 handle.
    if (scale > 0.0f) {
        mGroup[0].mHandle =
            mtl::MemManager::create(
                region->field_0x8,
                rem - 0x100, "NW4R Mem");
    } else {
        mGroup[0].mHandle = mtl::MemManager::getHandleMEM2();
    }
    mGroup[0].mFuncs.allocFunc = (MEMAllocatorAllocFunc)RootNw4r_AllocBuf;
    mGroup[0].mFuncs.freeFunc = (MEMAllocatorFreeFunc)RootNw4r_FreeBuf;
    mAlloc[0].funcs = &mGroup[0].mFuncs;
    mAlloc[0].heap = NULL;
    mAlloc[0].heapParam1 = (u32)this;
    mAlloc[0].heapParam2 = mGroup[0].mHandle;

    if (scale > 0.0f) {
        mGroup[1].mHandle =
            mtl::MemManager::create(
                ((CScnRootMgrRegion*)mpScn->unk88)->field_0x8,
                rem2 - 0x100, "NW4R Mem(P)");
    } else {
        mGroup[1].mHandle = mtl::MemManager::getHandleMEM2();
    }
    mGroup[1].mFuncs.allocFunc = (MEMAllocatorAllocFunc)RootNw4r_AllocBuf;
    mGroup[1].mFuncs.freeFunc = (MEMAllocatorFreeFunc)RootNw4r_FreeBuf;
    mAlloc[1].funcs = &mGroup[1].mFuncs;
    mAlloc[1].heap = NULL;
    mAlloc[1].heapParam1 = (u32)this;
    mAlloc[1].heapParam2 = mGroup[1].mHandle;

    if (scale > 0.0f) {
        mGroup[2].mHandle =
            mtl::MemManager::create(
                ((CScnRootMgrRegion*)mpScn->unk88)->field_0x8,
                bufsz - 0x100, "NW4R Mem(S)");
    } else {
        mGroup[2].mHandle = mtl::MemManager::getHandleMEM2();
    }
    mGroup[2].mFuncs.allocFunc = (MEMAllocatorAllocFunc)RootNw4r_AllocBuf;
    mGroup[2].mFuncs.freeFunc = (MEMAllocatorFreeFunc)RootNw4r_FreeBuf;
    mAlloc[2].funcs = &mGroup[2].mFuncs;
    mAlloc[2].heap = NULL;
    mAlloc[2].heapParam1 = (u32)this;
    mAlloc[2].heapParam2 = mGroup[2].mHandle;

    // Retail passes the just-created group[2] handle as the heap (r3 is
    // still holding it at this call in retail asm).
    if (scale > 0.0f) {
        mGroup[3].mHandle =
            mtl::MemManager::create(mGroup[2].mHandle, bufsz / 2, "NW4R Mem(A)");
    } else {
        mGroup[3].mHandle = mtl::MemManager::getHandleMEM2();
    }
    mGroup[3].mFuncs.allocFunc = (MEMAllocatorAllocFunc)RootNw4r_AllocBuf;
    mGroup[3].mFuncs.freeFunc = (MEMAllocatorFreeFunc)RootNw4r_FreeBuf;
    mAlloc[3].funcs = &mGroup[3].mFuncs;
    mAlloc[3].heap = NULL;
    mAlloc[3].heapParam1 = (u32)this;
    mAlloc[3].heapParam2 = mGroup[3].mHandle;

    // MEM1 / MEM2 allocators (no per-slot handle).
    mFuncs[0].allocFunc = (MEMAllocatorAllocFunc)RootNw4r_AllocBuf;
    mFuncs[0].freeFunc = (MEMAllocatorFreeFunc)RootNw4r_FreeBuf;
    mAlloc[4].funcs = &mFuncs[0];
    mAlloc[4].heap = NULL;
    mAlloc[4].heapParam1 = (u32)this;
    mAlloc[4].heapParam2 = mtl::MemManager::getHandleMEM1();

    mFuncs[1].allocFunc = (MEMAllocatorAllocFunc)RootNw4r_AllocBuf;
    mFuncs[1].freeFunc = (MEMAllocatorFreeFunc)RootNw4r_FreeBuf;
    mAlloc[5].funcs = &mFuncs[0];  // retail: both MEM allocators share mFuncs[0]
    mAlloc[5].heap = NULL;
    mAlloc[5].heapParam1 = (u32)this;
    mAlloc[5].heapParam2 = mtl::MemManager::getHandleMEM2();

    // Build the nw4r scene root from the second allocator slot.
    mScnRoot = nw4r::g3d::ScnRoot::Construct(&mAlloc[1], NULL, 10,
                                                vpHeight, 0x20, 4);

    // Bloom object (handled by the scene-root allocator).
    void* mem = mtl::MemManager::allocate(0x94, getGroupHandle());
    if (mem != NULL) {
        mem = CScnBloom_InitWithView((CScnBloom*)mem, (CView*)scn);
    }
    mBloom = (CScnBloom*)mem;

    // Ten scn groups, each sized from the viewport rect and attached to the
    // scene root; the object itself registers as the group callback target.
    for (u32 i = 0; i < 10; i++) {
        // Invalid-handle test retail emits as addis+cmplwi (handle + 0x10000
        // wraps to 0xFFFF exactly when handle == -1).
        MEMAllocator* alloc = ((u32)(mGroup[2].mHandle + 0x10000) == 0xFFFF)
                                  ? &mAlloc[0]
                                  : &mAlloc[2];
        nw4r::g3d::ScnGroup* group =
            nw4r::g3d::ScnGroup::Construct(alloc, NULL, (u32)rect[i]);
        mScnRoot->PushBack(group);
        ((CScnGroupTailLayout*)group)->field_0xD4 =
            static_cast<nw4r::g3d::IScnObjCallback*>(this);
        group->EnableScnObjCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_A);
        group->EnableScnObjCallbackExecOp(nw4r::g3d::ScnObj::EXECOP_CALC_WORLD);
    }

}

// --- CScnRootNw4r helpers ---

// Returns the scene-group alloc handle, resolving it lazily through getBaseHandle when
// the stored handle is still invalid (-1). The (+ 0x10000) == 0xFFFF test is
// the invalid-handle check retail emits as addis/cmplwi.
u32 CScnRootNw4r::getGroupHandle() {
    u32 handle = mGroup[2].mHandle;
    u32 result;
    // Negated test + cheap-arm-first reproduces retail's beq-forward/b-over
    // layout (MWCC_PATTERNS 7f).
    if ((u32)(handle + 0x10000) != 0xFFFF) {
        result = handle;
    } else {
        result = getBaseHandle();
    }
    return result;
}

// Fixed-offset words/blocks reached through an untyped object base
// (RootNw4r_Ptr44C/45C/SelPtr46C/47C/Ptr49C). Only the
// starts are significant; spans are explicit pads.
struct CScnRootNw4rOffView {
    u8 pad_0x00[0x14];
    u32 word_0x14;                 // +0x14 (RootNw4r_GetField14)
    u8 pad_0x18[0x44C - 0x18];
    u8 block_0x44C[0x10];          // +0x44C
    u8 block_0x45C[0x10];          // +0x45C
    u8 block_0x46C[0x10];          // +0x46C
    u8 block_0x47C[0x20];          // +0x47C
    u8 block_0x49C[4];             // +0x49C word group start
    u8 pad_0x4A0[0x4C4 - 0x4A0];
    u32 word_0x4C4;                // +0x4C4 (RootNw4r_SelPtr46C gate)
    u8 pad_0x4C8[0x4D0 - 0x4C8];
    u32 word_0x4D0;                // +0x4D0 (RootNw4r_SelPtr47C gate)
};

// us-804933ac: destructor. Drains every remaining child of the nw4r scene
// root (ScnGroup-derived children are Destroy()ed), tears down the bloom
// object and releases the four alloc handles that are not the shared MEM2
// handle; the CScnRoot base dtor and deleting tail are compiler-emitted.
CScnRootNw4r::~CScnRootNw4r() {
    while (mScnRoot->Size() != 0) {
        nw4r::g3d::ScnObj* obj = mScnRoot->PopBack();
        // Keep only ScnGroup-derived objs (inlined DynamicCast against the
        // pooled ScnGroup type-name data).
        bool isGroup =
            obj != NULL &&
            obj->IsDerivedFrom(nw4r::g3d::ScnGroup::GetTypeObjStatic());
        nw4r::g3d::ScnGroup* group =
            isGroup ? static_cast<nw4r::g3d::ScnGroup*>(obj) : NULL;
        group->Destroy();
    }
    mScnRoot->Destroy();

    CScnBloom* bloom = mBloom;
    mScnRoot = NULL;
    if (bloom != NULL) {
        __dt__8049A0D0(bloom, 1);
        mBloom = NULL;
    }

    if (mGroup[0].mHandle != mtl::MemManager::getHandleMEM2()) {
        mtl::MemManager::erase(mGroup[0].mHandle);
    }
    mGroup[0].mHandle = -1;

    if (mGroup[1].mHandle != mtl::MemManager::getHandleMEM2()) {
        mtl::MemManager::erase(mGroup[1].mHandle);
    }
    mGroup[1].mHandle = -1;

    if (mGroup[3].mHandle != mtl::MemManager::getHandleMEM2()) {
        mtl::MemManager::erase(mGroup[3].mHandle);
    }
    mGroup[3].mHandle = -1;

    if (mGroup[2].mHandle != mtl::MemManager::getHandleMEM2()) {
        mtl::MemManager::erase(mGroup[2].mHandle);
    }
    mGroup[2].mHandle = -1;

}

// us-80493544: apply the camera matrices plus the GX viewport/scissor from
// the GX cache, then select the camera on the nw4r scene root.
void CScnRootNw4r::ApplyCamera(CScnCamLayout* cam) {
    nw4r::g3d::Camera camera = mScnRoot->GetCamera((int)cam->field_0xC);
    camera.SetProjectionMtxDirectly(&cam->mProjMtx);
    camera.SetCameraMtxDirectly(cam->mCamMtx);

    CGXCacheViewportRect* viewport =
        getViewRectGX__8CGXCacheFv(CDeviceGX::getCacheInstance());
    camera.SetViewport(viewport->x, viewport->y, viewport->width, viewport->height);


    CGXCacheScissorArea* scissor =
        (CGXCacheScissorArea*)CDeviceGX::getCacheInstance();
    camera.SetScissor((u32)scissor->field_0x4A0, (u32)scissor->field_0x4A2,
                      (u32)scissor->field_0x4A4, (u32)scissor->field_0x4A6);

    mScnRoot->SetCurrentCamera((int)cam->field_0xC);
}

// us-8049363c: remove the entry equal to `value`, shifting later entries down.
void CScnRootNw4r::RemoveId(u32 value) {
    u32 count = mIdCount;
    for (u32 i = 0; i < count; i++) {
        u32 id = mIds[i];
        if (id == value) {
            for (u32 j = i; j < mIdCount - 1; j++) {
                mIds[j] = mIds[j + 1];
            }
            mIdCount = mIdCount - 1;
            return;
        }
    }
}

// us-804936a4: attach one queued camera layout to its ScnGroup. When the
// scene flag (+0x3E8) is still clear the layout is re-queued for the next
// pass and 1 is returned; otherwise the camera's registered child object is
// inserted into the group at the child-array tail, bumping the attach
// counter. Returns whether an insertion happened.
extern "C" s32 RootNw4r_AttachCamOne(CScnRootNw4r* self, CScnCamLayout* cam) {
    u8 ready = self->mpScn->unk_3E8;
    if (ready == 0) {
        // Not ready yet: push back onto the queue and retry next frame.
        u32 count = self->mIdCount;
        u32 next = count + 1;
        self->mIds[count] = (u32)cam;
        self->mIdCount = next;
        return 1;
    }

    s16 idx = cam->field_0x16C0;
    nw4r::g3d::ScnRoot* scnRoot = self->mScnRoot;
    if ((u32)idx >= scnRoot->Size()) {
        nw4r::db::Panic(lbl_eu_8056E720, 0x22b, lbl_eu_8056E6F8);
    }

    nw4r::g3d::ScnObj* obj = (*scnRoot)[(u32)idx];
    // Keep only ScnGroup-derived objs (inlined DynamicCast against the
    // pooled ScnGroup type-name data).
    bool isGroup =
        obj != NULL &&
        obj->IsDerivedFrom(nw4r::g3d::ScnGroup::GetTypeObjStatic());
    nw4r::g3d::ScnGroup* group =
        isGroup ? static_cast<nw4r::g3d::ScnGroup*>(obj) : NULL;

    if (cam->field_0x1470 != NULL) {
        if (group->Insert(group->Size(), cam->field_0x1470)) {
            self->mAttachCount++;
            return 1;
        }
    } else {
        if (group->Insert(group->Size(), cam->field_0x147C)) {
            self->mAttachCount++;
            return 1;
        }
    }
    return 0;
}

// us-8049381c: remove a child from the camera's ScnGroup. The child must be
// ScnGroup-derived; which of the two candidate pointers is removed depends
// on cam->field_0x1470 being set. Returns 1 when a removal happened.
extern "C" s32 RootNw4r_DetachCamChild(CScnRootNw4r* self, CScnCamLayout* cam) {
    s16 idx = cam->field_0x16C0;
    nw4r::g3d::ScnRoot* scnRoot = self->mScnRoot;
    if ((u32)idx >= scnRoot->Size()) {
        nw4r::db::Panic(lbl_eu_8056E720, 0x22b, lbl_eu_8056E6F8);
    }

    nw4r::g3d::ScnObj* obj = (*scnRoot)[(u32)idx];
    // Keep only ScnGroup-derived objs (inlined DynamicCast against the
    // pooled ScnGroup type-name data).
    bool isGroup =
        obj != NULL &&
        obj->IsDerivedFrom(nw4r::g3d::ScnGroup::GetTypeObjStatic());
    nw4r::g3d::ScnGroup* group =
        isGroup ? static_cast<nw4r::g3d::ScnGroup*>(obj) : NULL;

    if (cam->field_0x1470 != NULL) {
        if (group->Remove(cam->field_0x1470)) {
            self->mAttachCount--;
            return 1;
        }
    } else {
        if (group->Remove(cam->field_0x147C)) {
            self->mAttachCount--;
            return 1;
        }
    }
    return 0;
}

// us-8049395c: update the nw4r scene root (animation/world/material), then
// flag the frame as drawn.
void CScnRootNw4r::UpdateFrame() {
    if (mScnRoot == NULL) {
        return;
    }

    mFrame = 0;
    Scn_SetCurrentScene((u32)mpScn);

    nw4r::g3d::AnmScn* anm = mScnRoot->mpAnmScn;
    if (anm != NULL) {
        anm->SetUpdateRate(Scn_GetFrameDelta(mpScn));
    }

    scnLgtBindCamera(mpScn->mLgtCtrl);
    mScnRoot->UpdateFrame();
    mScnRoot->CalcWorld();
    mScnRoot->CalcMaterial();
    Scn_SetCurrentScene(0);
    mFrameDone = 1;
}

// us-80493a08: per-frame draw pass - camera-manager cull, per-manager
// updates, view calc + gather, then the two callback phases.
void CScnRootNw4r::DrawFrame() {
    if (mScnRoot == NULL) {
        return;
    }
    if (mFrameDone == 0) {
        return;
    }

    ((CScnCameraMan*)mpScn->mCamWork)->refreshCurrentCamera();

    // Scissor validity gate: computed into a flag local, matching retail's
    // li/set-branch sequence.
    s32 skip = 0;
    CGXCacheScissorArea* cache =
        (CGXCacheScissorArea*)CDeviceGX::getCacheInstance();
    if (cache->field_0x4A4 <= 0 || cache->field_0x4A6 <= 0) {
        skip = 1;
    }
    if (skip != 0) {
        return;
    }

    ICulling* culling = mpScn->unkB4;
    if (culling != NULL) {
        culling->update(
            (ml::CFrustum*)Scn_FindCamItem(mpScn, -1));
    }

    scnLgtUpdateSceneLights(mpScn->mLgtCtrl);
    LightManInvokeLightV3((CScnLightMan*)mpScn->unk64);
    FogManIsFogEnabled((CScnFogMan*)mpScn->unk78);
    CScnItemPool_forEachCallVfunc14(mpScn->mItemPool);

    mScnRoot->CalcView();
    mScnRoot->GatherDrawScnObj();

    // Called through a cast so MWCC's IPA cannot elide the calls to the
    // empty same-TU placeholder (MWCC_PATTERNS 7h).
    ((void (*)(CScnRootNw4r*, u32))RootNw4r_DrawDispatch)(this, 0);
    ((void (*)(CScnRootNw4r*, u32))RootNw4r_DrawDispatch)(this, 1);

    scnLgtEnterMode20(mpScn->mLgtCtrl);
}

// us-80493b1c: two-phase draw dispatch.
//  phase 0: reset g3d, run the opaque+transparent passes, refresh the GX
//           cache/view, and - when the scene reports new work - dispatch the
//           per-model draw hook of every item in the pool's kind-1 list.
//  phase 1: gated on the bloom object's enable bit; latches redraw requests
//           from every list item, re-runs the full draw when one is pending,
//           otherwise runs the half-res bloom path, then clears all flags.
extern "C" void RootNw4r_DrawDispatch(CScnRootNw4r* self, int flag) {
    if (flag == 0) {
        self->mInBloomPass = 0;
        nw4r::g3d::G3dReset();
        self->mScnRoot->DrawOpa();
        self->mScnRoot->DrawXlu();

        CScnDrawList* list =
            (CScnDrawList*)CScnItemPool_lookupSubPool(self->mpScn->mItemPool, 1);
        resetGXStateA__8CGXCacheFv(CDeviceGX::getCacheInstance());
        updateViewRoot__9CViewRootFv();

        if (MaruShadowSetupTexturePipe(self->mpScn) != 0) {
            for (CScnDrawNode* node = list->mStartNodePtr->mNext;
                 node != list->mStartNodePtr; node = node->mNext) {
                if (node->mItem->hook != NULL) {
                    node->mItem->hook->unk_10();
                }
            }
        }
    } else if (flag == 1) {
        if (!(self->mBloom->mFlags & 1)) {
            return;
        }
        self->mInBloomPass = 1;

        CScnDrawList* list =
            (CScnDrawList*)CScnItemPool_lookupSubPool(self->mpScn->mItemPool, 1);
        // Collect redraw requests from every item in the draw list.
        int pending = self->mRedrawReq;
        self->mRedrawReq = 0;
        CScnDrawNode* node;
        for (node = list->mStartNodePtr->mNext;
             node != list->mStartNodePtr; node = node->mNext) {
            pending = simSyncBuf824Flags((CScnItemModel*)node->mItem, 1) | pending;
            pending = (pending != 0);
        }

        if (pending != 0) {
            // Full redraw: re-render the bloom source and both passes.
            CScnBloom_RenderFullRes(self->mBloom);
            nw4r::g3d::G3dReset();
            self->mScnRoot->DrawOpa();
            self->mScnRoot->DrawXlu();
            CScnBloom_RenderCached(self->mBloom);
        } else if (self->mBloom->mFlags & 4) {
            CScnBloom_RenderHalfRes(self->mBloom);
        }

        // Clear the per-item request flags.
        for (CScnDrawNode* node = list->mStartNodePtr->mNext;
             node != list->mStartNodePtr; node = node->mNext) {
            simSyncBuf824Flags((CScnItemModel*)node->mItem, 0);
        }
        self->mInBloomPass = 0;
    }
}

// us-80493cdc (vtable slot vf5): attach every queued camera layout to its
// ScnGroup.
// Each queue entry whose scene flag (+0x3E8) is still clear is re-queued for
// the next pass; otherwise the camera's registered child object is inserted
// into the group at the child-array tail, bumping the attach counter.
void CScnRootNw4r::AttachQueued() {
    // Signed loop compare (retail emits cmpw/blt against the queue count).
    for (u32 i = 0; i < mIdCount; i++) {
        CScnCamLayout* cam = (CScnCamLayout*)mIds[i];

        if (mpScn->unk_3E8 == 0) {
            // Not ready yet: push back onto the queue and retry next frame.
            u32 n = mIdCount;
            mIds[n] = (u32)cam;
            mIdCount = n + 1;
            continue;
        }

        s16 idx = cam->field_0x16C0;
        if ((u32)idx >= mScnRoot->Size()) {
            nw4r::db::Panic(lbl_eu_8056E720, 0x22b, lbl_eu_8056E6F8);
        }

        nw4r::g3d::ScnObj* obj = (*(mScnRoot))[(u32)idx];
        // Keep only ScnGroup-derived objs (inlined DynamicCast against the
        // pooled ScnGroup type-name data).
        bool isGroup =
            obj != NULL &&
            obj->IsDerivedFrom(nw4r::g3d::ScnGroup::GetTypeObjStatic());
        nw4r::g3d::ScnGroup* group =
            isGroup ? static_cast<nw4r::g3d::ScnGroup*>(obj) : NULL;

        if (cam->field_0x1470 != NULL) {
            if (group->Insert(group->Size(), cam->field_0x1470)) {
                mAttachCount++;
            }
        } else {
            if (group->Insert(group->Size(), cam->field_0x147C)) {
                mAttachCount++;
            }
        }
    }
    mIdCount = 0;
}

extern "C" void* RootNw4r_Ptr44C(u8* self) { return ((CScnRootNw4rOffView*)self)->block_0x44C; }

extern "C" void* RootNw4r_Ptr45C(u8* self) { return ((CScnRootNw4rOffView*)self)->block_0x45C; }

extern "C" void* RootNw4r_SelPtr46C(u8* self) {
    if (((CScnRootNw4rOffView*)self)->word_0x4C4 == 0xFFFFFFFF) return ((CScnRootNw4rOffView*)self)->block_0x44C;
    return ((CScnRootNw4rOffView*)self)->block_0x46C;
}

extern "C" void* RootNw4r_SelPtr47C(u8* self) {
    if (((CScnRootNw4rOffView*)self)->word_0x4D0 == 0xFFFFFFFF) return ((CScnRootNw4rOffView*)self)->block_0x44C;
    return ((CScnRootNw4rOffView*)self)->block_0x47C;
}

extern "C" void* RootNw4r_Ptr49C(u8* self) { return ((CScnRootNw4rOffView*)self)->block_0x49C; }

// us-80493ea8: allocate `size` bytes from the scene-root allocator.
// Falls back to the MEM2 region when the current region cannot hold size*2.
extern "C" void* RootNw4r_AllocBuf(MEMAllocator* alloc, u32 size) {
    CScnRootNw4r* root = (CScnRootNw4r*)alloc->heapParam1;
    u32 handle = alloc->heapParam2;
    u8 flag = root->mAllocTail;

    if (mtl::MemManager::getMaxAllocSize(handle) < size * 2) {
        handle = mtl::MemManager::getHandleMEM2();
        flag = 1;
    }

    if (flag != 0) {
        return mtl::MemManager::allocate_tail(handle, size, 0x20);
    }
    return mtl::MemManager::allocate_head(handle, size, 0x20);
}

// us-80493f38: deallocate a buffer unless it is null.
extern "C" void RootNw4r_FreeBuf(MEMAllocator* alloc, void* p) {
    if (p != 0) {
        mtl::MemManager::deallocate(p);
    }
}

// us-80493f4c (IScnObjCallback CALC_WORLD override): advance the frame counter and dispatch to every registered
// (callback, id) pair whose id matches the current frame counter.
void CScnRootNw4r::ExecCallback_CALC_WORLD(nw4r::g3d::ScnObj::Timing timing,
                                           nw4r::g3d::ScnObj* obj, u32 param,
                                           void* info) {
    if (obj != NULL) {
        obj->IsDerivedFrom(nw4r::g3d::ScnGroup::GetTypeObjStatic());
    }

    for (u32 i = 0; i < mPairCount; i++) {
        if (mArrB[i] == mFrame) {
            ((CScnPairCb*)mArrA[i])->vf1((u32)mpScn);
        }
    }
    mFrame = mFrame + 1;
}

void CScnRootNw4r::AddPair(u32 a, u32 b) {
    mArrA[mPairCount] = a;
    mArrB[mPairCount++] = b;
}

// us-80494030: remove the (a, b) pair entry, shifting later entries down.
void CScnRootNw4r::RemovePair(u32 a, s32 b) {
    u32 count = mPairCount;
    for (u32 i = 0; i < count; i++) {
        u32 cur = mArrA[i];
        if (cur == a && b == (s32)mArrB[i]) {
            for (u32 j = i; j < mPairCount - 1; j++) {
                mArrA[j] = mArrA[j + 1];
                mArrB[j] = mArrB[j + 1];
            }
            mPairCount = mPairCount - 1;
            return;
        }
    }
}

CScnBloom* CScnRootNw4r::getBloom() { return mBloom; }

// us-804940b4: return the +0x4B8 alloc handle, creating it lazily through
// getBaseHandle (vtable+0x2C) when it is still invalid (-1).
u32 CScnRootNw4r::getAllocHandle() {
    u32 handle = mGroup[1].mHandle;
    // Invalid-handle test: (handle + 0x10000) == 0xFFFF holds only when both
    // halves of the packed alloc handle are 0xFFFF (i.e. all ones); this is
    // the addis/cmplwi lowering seen in retail.
    // OPEN ITEM: retail emits the gate as [cmp; beq body; b epi] (body
    // out-of-line); every reachable source shape collapses it to the single
    // inverted [bne epi]. See MWCC_CASES 10378 / sjrbf_PutChunk.
    if ((u32)(handle + 0x10000) == 0xFFFF) {
        handle = getBaseHandle();
    }
    return handle;
}

extern "C" u32 getScnCounter__Fv(void) {
    extern u32 lbl_eu_806639A8;
    return lbl_eu_806639A8;
}

extern "C" void updateScnCounter__FUl(u32 param) {
    extern u32 lbl_eu_806658FC;
    if (param) {
        lbl_eu_806658FC += 1;
    } else {
        lbl_eu_806658FC -= 1;
    }
}
