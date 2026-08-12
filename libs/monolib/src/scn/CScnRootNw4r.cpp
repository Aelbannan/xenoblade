// Auto-scaffolded catalog TU for monolib/src/scn/CScnRootNw4r
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/scn/CScnRootNw4r.hpp"           // extern "C" __dt__12CScnRootNw4rFv
#include "libs/monolib/src/scn/CScnRootNw4r.hpp"  // CScnRootNw4r class + fields
#include "monolib/util/MemManager.hpp"            // mtl::MemManager

void* func_8048FDDC(u8* self) { return (void*)((u8*)self + 0x44c); }


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
    u32 idx = self->mPairCount;
    self->mArrA[idx] = a;
    u32 idx2 = self->mPairCount;
    self->mArrB[idx2] = b;
    self->mPairCount = idx2 + 1;
}

// us-80494030: remove the (a, b) pair entry, shifting later entries down.
void func_8048FFBC(CScnRootNw4r* self, u32 a, s32 b) {
    u32 count = self->mPairCount;
    for (u32 i = 0; i < count; i++) {
        if (self->mArrA[i] == a && b == self->mArrB[i]) {
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
u32 func_80490040(CScnRootNw4r* self) {
    u32 result = self->field_0x4B8;
    if (result == 0xFFFFFFFF) {
        result = self->vf9();
    }
    return result;
}

extern "C" void func_80490088(u8* self) { ((void(*)(void*))func_8048FED8)((char*)self - 0xc); }

void func_80490090(u8* self) { ((void(*)(void*))__dt__12CScnRootNw4rFv)((char*)self - 0xc); }

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
void func_8048F5C8(CScnRootNw4r* self, u32 value) {
    u32 count = self->mIdCount;
    for (u32 i = 0; i < count; i++) {
        if (self->mIds[i] == value) {
            for (u32 j = i; j < self->mIdCount - 1; j++) {
                self->mIds[j] = self->mIds[j + 1];
            }
            self->mIdCount = self->mIdCount - 1;
            return;
        }
    }
}

// us-80493544: apply the camera matrices plus the GX viewport/scissor from
// the GX cache, then select the camera on the nw4r scene root.
extern "C" void func_8048F4D0(CScnRootNw4r* self, CScnCamLayout* cam) {
    nw4r::g3d::Camera camera = self->mScnRoot->GetCamera((int)cam->field_0xC);
    camera.SetProjectionMtxDirectly(&cam->mProjMtx);
    camera.SetCameraMtxDirectly(cam->mCamMtx);

    CGXCacheViewportRect* viewport =
        func_8044BE10__8CGXCacheFv(CDeviceGX::getCacheInstance());
    camera.SetViewport((f32)viewport->x, (f32)viewport->y, (f32)viewport->width,
                       (f32)viewport->height);

    CGXCacheScissorArea* scissor =
        (CGXCacheScissorArea*)CDeviceGX::getCacheInstance();
    camera.SetScissor((u32)scissor->field_0x4A0, (u32)scissor->field_0x4A2,
                      (u32)scissor->field_0x4A4, (u32)scissor->field_0x4A6);

    self->mScnRoot->SetCurrentCamera((int)cam->field_0xC);
}

extern "C" void func_8048F630() {}

// us-8049381c: remove a child from the camera's ScnGroup. The child must be
// ScnGroup-derived; which of the two candidate pointers is removed depends
// on cam->field_0x1470 being set. Returns 1 when a removal happened.
extern "C" s32 func_8048F7A8(CScnRootNw4r* self, CScnCamLayout* cam) {
    nw4r::g3d::ScnRoot* scnRoot = self->mScnRoot;
    s16 idx = cam->field_0x16C0;
    if ((u32)idx >= scnRoot->Size()) {
        nw4r::db::Panic(lbl_eu_8056E720, 0x22b, (const char*)lbl_eu_8056E6F8);
    }

    nw4r::g3d::ScnObj* obj = (*scnRoot)[(u32)idx];
    s32 isGroup = 0;
    if (obj != NULL &&
        obj->IsDerivedFrom(nw4r::g3d::G3dObj::TypeObj(lbl_eu_8051D788))) {
        isGroup = 1;
    }
    obj = isGroup ? obj : NULL;

    nw4r::g3d::ScnGroup* group = (nw4r::g3d::ScnGroup*)obj;
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

    CGXCacheScissorArea* cache =
        (CGXCacheScissorArea*)CDeviceGX::getCacheInstance();
    if (cache->field_0x4A4 <= 0 || cache->field_0x4A6 <= 0) {
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

    func_8048FAA8(self, 0);
    func_8048FAA8(self, 1);

    func_804C19B8(self->field_0x4->mEnvLgtCtrl);
}

extern "C" void func_8048FAA8(CScnRootNw4r* self, u32 flag) {}

extern "C" void func_8048FC68() {}
