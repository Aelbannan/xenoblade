// Auto-scaffolded catalog TU for monolib/src/scn/CScnRootNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/scn/CScnTimeApi.hpp"
#include "monolib/scn/CScnRootNw4r.hpp"           // extern "C" __dt__12CScnRootNw4rFv
#include "libs/monolib/src/scn/CScnRootNw4r.hpp"  // CScnRootNw4r class + fields
#include "monolib/util/MemManager.hpp"            // mtl::MemManager
#include "libs/monolib/src/scn/CScnBloom.hpp"

extern "C" u32 lbl_eu_8056E768[23];  // CScnRootNw4r vtable group (.data, below)

void* func_8048FDDC(u8* self) { return (void*)((u8*)self + 0x44c); }

// us-804933ac: complete destructor. Restores both retail vptrs by hand
// (primary + IScnObjCallback secondary at +0xC), drains every remaining
// child of the nw4r scene root (ScnGroup-derived children are Destroy()ed),
// tears down the owned sub-resource, releases the four alloc handles that
// are not the shared MEM2 handle, runs the CScnRoot base teardown and lets
// the compiler's deleting tail free `this` when the delete flag is set.
CScnRootNw4r::~CScnRootNw4r() {
    // novtable: primary vptr + second-base vptr (vtbl + 0x38).
    *(u32**)this = (u32*)lbl_eu_8056E768;
    *(u32**)((char*)this + 0xC) = (u32*)(lbl_eu_8056E768 + 14);

    while (mScnRoot->Size() != 0) {
        nw4r::g3d::ScnObj* obj = mScnRoot->PopBack();
        // Keep only ScnGroup-derived objs (inlined DynamicCast against the
        // pooled ScnGroup type-name data).
        bool isGroup =
            obj != NULL &&
            obj->IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj(lbl_eu_8051D788));
        nw4r::g3d::ScnGroup* group =
            isGroup ? static_cast<nw4r::g3d::ScnGroup*>(obj) : NULL;
        group->Destroy();
    }
    mScnRoot->Destroy();

    CScnBloom* bloom = field_0x14;
    mScnRoot = NULL;
    if (bloom != NULL) {
        __dt__8049A0D0(bloom, 1);
        field_0x14 = NULL;
    }

    if (field_0x4AC != mtl::MemManager::getHandleMEM2()) {
        mtl::MemManager::erase(field_0x4AC);
    }
    field_0x4AC = -1;

    if (field_0x4B8 != mtl::MemManager::getHandleMEM2()) {
        mtl::MemManager::erase(field_0x4B8);
    }
    field_0x4B8 = -1;

    if (field_0x4D0 != mtl::MemManager::getHandleMEM2()) {
        mtl::MemManager::erase(field_0x4D0);
    }
    field_0x4D0 = -1;

    if (field_0x4C4 != mtl::MemManager::getHandleMEM2()) {
        mtl::MemManager::erase(field_0x4C4);
    }
    field_0x4C4 = -1;

    __dt__8CScnRootFv(this, 0);
}


extern "C" void* func_8048FDE4(u8* self) { return (void*)((u8*)self + 0x45c); }


extern "C" void* func_8048FDEC(u8* self) {
    if (*(u32*)((u8*)self + 0x4C4) == 0xFFFFFFFF) return (u8*)self + 0x44C;
    return (u8*)self + 0x46C;
}

extern "C" void* func_8048FE0C(u8* self) {
    if (*(u32*)((u8*)self + 0x4D0) == 0xFFFFFFFF) return (u8*)self + 0x44C;
    return (u8*)self + 0x47C;
}

extern "C" void* func_8048FE2C(u8* self) { return (void*)((u8*)self + 0x49c); }

// us-80493ea8: allocate `size` bytes from the scene-root allocator.
// Falls back to the MEM2 region when the current region cannot hold size*2.
void* func_8048FE34(CScnRootNw4r* self, u32 size) {
    CScnRootNw4rBuffer* buffer = self->field_0x8;
    u32 handle = self->field_0xC;
    u8 flag = buffer->field_0x448;

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
void func_8048FEC4(CScnRootNw4r* self, u8* p) {
    if (p != 0) {
        mtl::MemManager::deallocate(p);
    }
}

// us-80493f4c: advance the frame counter and dispatch to every registered
// (callback, id) pair whose id matches the current frame counter.
extern "C" void func_8048FED8(CScnRootNw4r* self, u32 r4,
                              nw4r::g3d::G3dObj* obj) {
    if (obj != NULL) {
        obj->IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj(lbl_eu_8051D788));
    }

    for (u32 i = 0; i < self->mPairCount; i++) {
        if (self->mArrB[i] == self->field_0x1C) {
            ((CScnPairCb*)self->mArrA[i])->vf1((u32)self->field_0x4);
        }
    }
    self->field_0x1C = self->field_0x1C + 1;
}

extern "C" void func_8048FF90(CScnRootNw4r* self, u32 a, u32 b) {
    self->mArrA[self->mPairCount] = a;
    self->mArrB[self->mPairCount++] = b;
}

// us-80494030: remove the (a, b) pair entry, shifting later entries down.
extern "C" void func_8048FFBC(CScnRootNw4r* self, u32 a, s32 b) {
    u32 count = self->mPairCount;
    for (u32 i = 0; i < count; i++) {
        u32 cur = self->mArrA[i];
        if (cur == a && b == (s32)self->mArrB[i]) {
            for (u32 j = i; j < self->mPairCount - 1; j++) {
                self->mArrA[j] = self->mArrA[j + 1];
                self->mArrB[j] = self->mArrB[j + 1];
            }
            self->mPairCount = self->mPairCount - 1;
            return;
        }
    }
}

extern "C" u32 func_80490038(u8* self) { return *(u32*)((u8*)self + 0x14); }

// us-804940b4: return the +0x4B8 alloc handle, creating it lazily through
// the virtual at vtable+0x2C when it is still invalid (-1).
extern "C" u32 func_80490040(CScnRootNw4r* self) {
    u32 handle = self->field_0x4B8;
    // Invalid-handle test: (handle + 0x10000) == 0xFFFF holds only when both
    // halves of the packed alloc handle are 0xFFFF (i.e. all ones); this is
    // the addis/cmplwi lowering seen in retail.
    // OPEN ITEM: retail emits the gate as [cmp; beq body; b epi] (body
    // out-of-line); every reachable source shape collapses it to the single
    // inverted [bne epi]. See MWCC_CASES 10378 / sjrbf_PutChunk.
    if ((u32)(handle + 0x10000) == 0xFFFF) {
        handle = self->vf9();
    }
    return handle;
}

extern "C" void func_80490088(u8* self) { ((void(*)(void*))func_8048FED8)((char*)self - 0xc); }

extern "C" void func_80490090(u8* self) { ((void(*)(void*))__dt__12CScnRootNw4rFv)((char*)self - 0xc); }

extern "C" u32 func_80490098__Fv(void) {
    extern u32 lbl_eu_806639A8;
    return lbl_eu_806639A8;
}

extern "C" void func_804900A0__FUl(u32 param) {
    extern u32 lbl_eu_806658FC;
    if (param) {
        lbl_eu_806658FC += 1;
    } else {
        lbl_eu_806658FC -= 1;
    }
}

// us-8049363c: remove the entry equal to `value`, shifting later entries down.
extern "C" void func_8048F5C8(CScnRootNw4r* self, u32 value) {
    u32 count = self->mIdCount;
    for (u32 i = 0; i < count; i++) {
        u32 id = self->mIds[i];
        if (id == value) {
            for (u32 j = i; j < self->mIdCount - 1; j++) {
                self->mIds[j] = self->mIds[j + 1];
            }
            self->mIdCount = self->mIdCount - 1;
            return;
        }
    }
}

// Retail s16->f32 conversion magic (.sdata2 0x8066A9F8, 0x4330000080000000);
// referenced by name so the viewport conversions below emit no local .sdata2
// pool (retail CScnRootNw4r.o has an empty .sdata2).
extern double lbl_eu_8066A9F8;

// us-80493544: apply the camera matrices plus the GX viewport/scissor from
// the GX cache, then select the camera on the nw4r scene root.
extern "C" void func_8048F4D0(CScnRootNw4r* self, CScnCamLayout* cam) {
    // Two s16->f32 conversion temporaries through the shared signed magic
    // double: seed the 0x43300000 exponent word once, fill mantissa words,
    // subtract the magic (retail reuses each temp for two conversions).
    union {
        double d;
        u32 w[2];
    } cy;
    union {
        double d;
        u32 w[2];
    } cx;
    cx.w[0] = 0x43300000u;
    cy.w[0] = 0x43300000u;

    nw4r::g3d::Camera camera = self->mScnRoot->GetCamera((int)cam->field_0xC);
    camera.SetProjectionMtxDirectly(&cam->mProjMtx);
    camera.SetCameraMtxDirectly(cam->mCamMtx);

    CGXCacheViewportRect* viewport =
        func_8044BE10__8CGXCacheFv(CDeviceGX::getCacheInstance());
    cx.w[1] = (u32)((s32)viewport->x ^ 0x80000000);
    f32 vx = (f32)(cx.d - lbl_eu_8066A9F8);
    cy.w[1] = (u32)((s32)viewport->y ^ 0x80000000);
    f32 vy = (f32)(cy.d - lbl_eu_8066A9F8);
    cx.w[1] = (u32)((s32)viewport->width ^ 0x80000000);
    f32 vw = (f32)(cx.d - lbl_eu_8066A9F8);
    cy.w[1] = (u32)((s32)viewport->height ^ 0x80000000);
    f32 vh = (f32)(cy.d - lbl_eu_8066A9F8);
    camera.SetViewport(vx, vy, vw, vh);


    CGXCacheScissorArea* scissor =
        (CGXCacheScissorArea*)CDeviceGX::getCacheInstance();
    camera.SetScissor((u32)scissor->field_0x4A0, (u32)scissor->field_0x4A2,
                      (u32)scissor->field_0x4A4, (u32)scissor->field_0x4A6);

    self->mScnRoot->SetCurrentCamera((int)cam->field_0xC);
}

// us-804936a4: attach one queued camera layout to its ScnGroup. When the
// scene flag (+0x3E8) is still clear the layout is re-queued for the next
// pass and 1 is returned; otherwise the camera's registered child object is
// inserted into the group at the child-array tail, bumping the attach
// counter. Returns whether an insertion happened.
s32 func_8048F630(CScnRootNw4r* self, CScnCamLayout* cam) {
    u8 ready = self->field_0x4->field_0x3E8;
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
        obj->IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj(lbl_eu_8051D788));
    nw4r::g3d::ScnGroup* group =
        isGroup ? static_cast<nw4r::g3d::ScnGroup*>(obj) : NULL;

    if (cam->field_0x1470 != NULL) {
        if (group->Insert(group->Size(), cam->field_0x1470)) {
            self->field_0x8 = (CScnRootNw4rBuffer*)((u8*)self->field_0x8 + 1);
            return 1;
        }
    } else {
        if (group->Insert(group->Size(), cam->field_0x147C)) {
            self->field_0x8 = (CScnRootNw4rBuffer*)((u8*)self->field_0x8 + 1);
            return 1;
        }
    }
    return 0;
}

// us-8049381c: remove a child from the camera's ScnGroup. The child must be
// ScnGroup-derived; which of the two candidate pointers is removed depends
// on cam->field_0x1470 being set. Returns 1 when a removal happened.
s32 func_8048F7A8(CScnRootNw4r* self, CScnCamLayout* cam) {
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
        obj->IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj(lbl_eu_8051D788));
    nw4r::g3d::ScnGroup* group =
        isGroup ? static_cast<nw4r::g3d::ScnGroup*>(obj) : NULL;

    if (cam->field_0x1470 != NULL) {
        if (group->Remove(cam->field_0x1470)) {
            self->field_0x8 = (CScnRootNw4rBuffer*)((u8*)self->field_0x8 - 1);
            return 1;
        }
    } else {
        if (group->Remove(cam->field_0x147C)) {
            self->field_0x8 = (CScnRootNw4rBuffer*)((u8*)self->field_0x8 - 1);
            return 1;
        }
    }
    return 0;
}

// us-8049395c: update the nw4r scene root (animation/world/material), then
// flag the frame as drawn.
extern "C" void func_8048F8E8(CScnRootNw4r* self) {
    if (self->mScnRoot == NULL) {
        return;
    }

    self->field_0x1C = 0;
    func_80496984((u32)self->field_0x4);

    nw4r::g3d::AnmScn* anm = self->mScnRoot->mpAnmScn;
    if (anm != NULL) {
        anm->SetUpdateRate(func_80496288(self->field_0x4));
    }

    func_804C2654(self->field_0x4->mEnvLgtCtrl);
    self->mScnRoot->UpdateFrame();
    self->mScnRoot->CalcWorld();
    self->mScnRoot->CalcMaterial();
    func_80496984(0);
    self->field_0x18 = 1;
}

// us-80493a08: per-frame draw pass - camera-manager cull, per-manager
// updates, view calc + gather, then the two callback phases.
extern "C" void func_8048F994(CScnRootNw4r* self) {
    if (self->mScnRoot == NULL) {
        return;
    }
    if (self->field_0x18 == 0) {
        return;
    }

    func_8049B024(self->field_0x4->mCamWork);

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

    ICulling* culling = self->field_0x4->mCulling;
    if (culling != NULL) {
        culling->ICulling_UnkVirtualFunc1(
            (ml::CFrustum*)func_80496264(self->field_0x4, -1));
    }

    func_804C22F0(self->field_0x4->mEnvLgtCtrl);
    func_8048D1B0(self->field_0x4->mLightMan);
    func_8049DE70(self->field_0x4->mFogMan);
    func_8048CD0C(self->field_0x4->mItemPool);

    self->mScnRoot->CalcView();
    self->mScnRoot->GatherDrawScnObj();

    // Called through a cast so MWCC's IPA cannot elide the calls to the
    // empty same-TU placeholder (MWCC_PATTERNS 7h).
    ((void (*)(CScnRootNw4r*, u32))func_8048FAA8)(self, 0);
    ((void (*)(CScnRootNw4r*, u32))func_8048FAA8)(self, 1);

    func_804C19B8(self->field_0x4->mEnvLgtCtrl);
}

// us-80493b1c: two-phase draw dispatch.
//  phase 0: reset g3d, run the opaque+transparent passes, refresh the GX
//           cache/view, and - when the scene reports new work - dispatch the
//           per-model draw hook of every item in the pool's kind-1 list.
//  phase 1: gated on the bloom object's enable bit; latches redraw requests
//           from every list item, re-runs the full draw when one is pending,
//           otherwise runs the half-res bloom path, then clears all flags.
extern "C" void func_8048FAA8(CScnRootNw4r* self, int flag) {
    if (flag == 0) {
        self->field_0x19 = 0;
        nw4r::g3d::G3dReset();
        self->mScnRoot->DrawOpa();
        self->mScnRoot->DrawXlu();

        CScnDrawList* list =
            (CScnDrawList*)func_8048C698(self->field_0x4->mItemPool, 1);
        func_8044BE38__8CGXCacheFv(CDeviceGX::getCacheInstance());
        func_80442DA8__9CViewRootFv();

        if (func_8048D264(self->field_0x4) != 0) {
            for (CScnDrawNode* node = list->mStartNodePtr->mNext;
                 node != list->mStartNodePtr; node = node->mNext) {
                if (node->mItem->hook != NULL) {
                    node->mItem->hook->unk_10();
                }
            }
        }
    } else if (flag == 1) {
        if (!(self->field_0x14->mFlags & 1)) {
            return;
        }
        self->field_0x19 = 1;

        CScnDrawList* list =
            (CScnDrawList*)func_8048C698(self->field_0x4->mItemPool, 1);
        // Collect redraw requests from every item in the draw list.
        int pending = self->field_0x1A;
        self->field_0x1A = 0;
        CScnDrawNode* node;
        for (node = list->mStartNodePtr->mNext;
             node != list->mStartNodePtr; node = node->mNext) {
            pending = func_80484734((CScnItemModel*)node->mItem, 1) | pending;
            pending = (pending != 0);
        }

        if (pending != 0) {
            // Full redraw: re-render the bloom source and both passes.
            func_8049A128(self->field_0x14);
            nw4r::g3d::G3dReset();
            self->mScnRoot->DrawOpa();
            self->mScnRoot->DrawXlu();
            func_8049A318(self->field_0x14);
        } else if (self->field_0x14->mFlags & 4) {
            func_8049AA74(self->field_0x14);
        }

        // Clear the per-item request flags.
        for (CScnDrawNode* node = list->mStartNodePtr->mNext;
             node != list->mStartNodePtr; node = node->mNext) {
            func_80484734((CScnItemModel*)node->mItem, 0);
        }
        self->field_0x19 = 0;
    }
}

// us-80493cdc (vtable slot vf5): attach every queued camera layout to its
// ScnGroup.
// Each queue entry whose scene flag (+0x3E8) is still clear is re-queued for
// the next pass; otherwise the camera's registered child object is inserted
// into the group at the child-array tail, bumping the attach counter.
void func_8048FC68(CScnRootNw4r* self) {
    // Signed loop compare (retail emits cmpw/blt against the queue count).
    for (u32 i = 0; i < self->mIdCount; i++) {
        CScnCamLayout* cam = (CScnCamLayout*)self->mIds[i];

        if (((u8*)self->field_0x4)[0x3E8] == 0) {
            // Not ready yet: push back onto the queue and retry next frame.
            u32 n = self->mIdCount;
            self->mIds[n] = (u32)cam;
            self->mIdCount = n + 1;
            continue;
        }

        s16 idx = cam->field_0x16C0;
        if ((u32)idx >= self->mScnRoot->Size()) {
            nw4r::db::Panic(lbl_eu_8056E720, 0x22b, lbl_eu_8056E6F8);
        }

        nw4r::g3d::ScnObj* obj = (*(self->mScnRoot))[(u32)idx];
        // Keep only ScnGroup-derived objs (inlined DynamicCast against the
        // pooled ScnGroup type-name data).
        bool isGroup =
            obj != NULL &&
            obj->IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj(lbl_eu_8051D788));
        nw4r::g3d::ScnGroup* group =
            isGroup ? static_cast<nw4r::g3d::ScnGroup*>(obj) : NULL;

        if (cam->field_0x1470 != NULL) {
            if (group->Insert(group->Size(), cam->field_0x1470)) {
                self->field_0x8 =
                    (CScnRootNw4rBuffer*)((u8*)self->field_0x8 + 1);
            }
        } else {
            if (group->Insert(group->Size(), cam->field_0x147C)) {
                self->field_0x8 =
                    (CScnRootNw4rBuffer*)((u8*)self->field_0x8 + 1);
            }
        }
    }
    self->mIdCount = 0;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056E768-0x8056E7D0 (104B): CScnRootNw4r vtable group.
// lbl_eu_806639A0/80663998 (typeinfo pairs) live in foreign TUs; the
// ExecCallback_* names are splitter-shortened nw4r template symbols.
extern "C" u32 lbl_eu_806639A0;
extern "C" u32 lbl_eu_80663998;
extern "C" void func_8048F2F0();
extern "C" void func_804871A8();
extern "C" void ExecCallback_CALC_MAT__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv();
extern "C" void ExecCallback_CALC_VIEW__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv();
extern "C" void ExecCallback_DRAW_OPA__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv();
extern "C" void ExecCallback_DRAW_XLU__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv();
extern "C" u32 lbl_eu_8056E768[23] = {
    (u32)&lbl_eu_806639A0, 0x00000000,
    (u32)&__dt__12CScnRootNw4rFv,
    (u32)&func_8048F5C8,
    (u32)&func_8048F4D0,
    (u32)&func_8048F8E8,
    (u32)&func_8048F994,
    (u32)&func_8048FC68,
    (u32)&func_8048FF90,
    (u32)&func_8048FFBC,
    (u32)&func_8048F2F0,
    (u32)&func_804871A8,
    (u32)&func_80490040,
    (u32)&func_80490038,
    (u32)&lbl_eu_806639A0,
    0xFFFFFFF4,
    (u32)&func_80490090,
    (u32)&func_80490088,
    (u32)&ExecCallback_CALC_MAT__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv,
    (u32)&ExecCallback_CALC_VIEW__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv,
    (u32)&ExecCallback_DRAW_OPA__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv,
    (u32)&ExecCallback_DRAW_XLU__Q34nw4r3g3d15IScnObjCallbackFQ44nw4r3g3d6ScnObj6TimingPQ34nw4r3g3d6ScnObjUlPv,
    (u32)&func_8048FED8,
};
extern "C" u32 lbl_eu_8056E7C4[3] = { (u32)&lbl_eu_80663998, 0x00000000, 0x00000000 };

// [.rodata] 0x80523FD0-0x80524010 (64B): RTTI name + pooled heap names.
extern "C" __declspec(align(8)) const char lbl_eu_80523FD0[0xD] = { 0x43,0x53,0x63,0x6E,0x52,0x6F,0x6F,0x74,0x4E,0x77,0x34,0x72,0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80523FE0[0x30] = { 0x4E,0x57,0x34,0x52,0x20,0x4D,0x65,0x6D,0x00,0x4E,0x57,0x34,0x52,0x20,0x4D,0x65,0x6D,0x28,0x50,0x29,0x00,0x4E,0x57,0x34,0x52,0x20,0x4D,0x65,0x6D,0x28,0x53,0x29,0x00,0x4E,0x57,0x34,0x52,0x20,0x4D,0x65,0x6D,0x28,0x41,0x29,0x00,0x00,0x00,0x00 };
