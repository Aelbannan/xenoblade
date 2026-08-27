#pragma once

#include <types.h>
#include <nw4r/g3d/g3d_scnroot.h>
#include <nw4r/g3d/g3d_init.h>
#include <nw4r/g3d/g3d_anmscn.h>
#include <nw4r/db/db_assert.h>
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/scn/ICulling.hpp"
#include "monolib/core/code_804E36DC.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__12CScnRootNw4rFv(void* self, int deleteFlag);
extern "C" void __dt__8CScnRootFv(void* self, int deleteFlag);
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
 *    slots; slot vf9 (retail func_804871A8) sits at vtable offset 0x2C and
 *    is the callee of func_80490040. The virtuals below are declared but
 *    never defined here (no key function -> no vtable emitted), purely so
 *    that call site compiles to the retail lwz/lwz/mtctr/bctrl sequence.
 *  - The real object derives from CScnRoot (+0x00) and
 *    nw4r::g3d::IScnObjCallback (+0x0C); the flat field layout below
 *    reproduces the same byte offsets.
 */

// Buffer object referenced by CScnRootNw4r::field_0x8 (partial layout).
struct CScnRootNw4rBuffer {
    u8 pad_0x0[0x448];                // +0x00
    u8 field_0x448;                   // +0x448 (allocation-mode flag)
};

// Manager classes owned by the scene object at CScnRootNw4r::field_0x4
// (defined in their own TUs under libs/monolib/src/scn/).
class CScnCameraMan;
class CScnLightMan;
class CScnFogMan;
class CScnEnvLgtCtrl;
struct CScnItemPool;

// Manager-slot layout of the owning scene object (retail CScn) - only the
// slots this TU passes to the per-manager update/draw helpers.
struct CScnMgrLayout {
    u8 pad_0x0[0x60];                 // +0x00
    CScnItemPool* mItemPool;          // +0x60 (arg of func_8048CD0C)
    CScnLightMan* mLightMan;          // +0x64 (arg of func_8048D1B0)
    CScnCameraMan* mCamWork;          // +0x68 (arg of func_8049B024)
    u8 pad_0x6C[0x78 - 0x6C];         // +0x6C
    CScnFogMan* mFogMan;              // +0x78 (arg of func_8049DE70)
    CScnEnvLgtCtrl* mEnvLgtCtrl;      // +0x7C (arg of func_804C2654 / 22F0 / 19B8)
    u8 pad_0x80[0xB4 - 0x80];         // +0x80
    ICulling* mCulling;               // +0xB4 (slot +0xC dispatch)
    u8 pad_0xB8[0x3E8 - 0xB8];        // +0xB8
    u8 field_0x3E8;                   // +0x3E8 (scene-ready flag)
    u8 pad_0x3E9[0x3EC - 0x3E9];      // +0x3E9
};

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
// Slot +0xC is dispatched by func_8048FED8 each frame.
struct CScnPairCb {
    virtual ~CScnPairCb() {}          // +0x8
    virtual void vf1(u32 value) = 0;  // +0xC
};

// Unmangled monolib callees (retail C-linkage names).
extern "C" void func_80496984(u32 value);
extern "C" void func_804C2654(CScnEnvLgtCtrl* ctrl);
extern "C" void func_804C22F0(CScnEnvLgtCtrl* ctrl);
extern "C" void func_804C19B8(CScnEnvLgtCtrl* ctrl);
extern "C" void func_8049B024(CScnCameraMan* cam);
extern "C" void func_8048D1B0(CScnLightMan* man);
extern "C" void func_8049DE70(CScnFogMan* man);
extern "C" void func_8048CD0C(CScnItemPool* pool);
extern "C" void func_8048FAA8(CScnRootNw4r* self, int flag);
extern "C" void resetGXStateA__8CGXCacheFv(CGXCache* cache);
extern "C" void updateViewRoot__9CViewRootFv();

// Panic strings for the ScnGroup bounds check (same file/format as CScnMem).
extern const char lbl_eu_8056E720[];
extern const char lbl_eu_8056E6F8[];

// Bloom post-effect owned at CScnRootNw4r::field_0x14 (defined in
// libs/monolib/src/scn/CScnBloom.cpp).
class CScnBloom;
extern "C" void func_8049A128(CScnBloom* bloom);
extern "C" void func_8049A318(CScnBloom* bloom);
extern "C" void func_8049AA74(CScnBloom* bloom);

// Scene-item model pool item drawn through the kind-1 draw list.
class CScnItemModel;
extern "C" int func_80484734(CScnItemModel* item, int param);

// Kind-1 sub-pool list returned by the scene-item-pool accessor.
extern "C" void* func_8048C698(void* pool, int kind);
extern "C" int func_8048D264(void* scene);

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

class CScnRootNw4r {
public:
    virtual ~CScnRootNw4r();          // vf0 (+0x08)
    virtual void vf1(u32 value);      // vf1 (+0x0C) func_8048F5C8
    virtual void vf2();               // vf2 (+0x10) func_8048F4D0
    virtual void vf3();               // vf3 (+0x14) func_8048F8E8
    virtual void vf4();               // vf4 (+0x18) func_8048F994
    virtual void vf5();               // vf5 (+0x1C) func_8048FC68
    virtual void vf6(u32 a, u32 b);   // vf6 (+0x20) func_8048FF90
    virtual void vf7(u32 a, u32 b);   // vf7 (+0x24) func_8048FFBC
    virtual void vf8();               // vf8 (+0x28) func_8048F2F0
    virtual u32 vf9();                // vf9 (+0x2C) func_804871A8
    virtual u32 vf10();               // vf10 (+0x30) func_80490040
    virtual u32 vf11();               // vf11 (+0x34) func_80490038

    // Fields (offsets verified against retail accessors).
    CScnMgrLayout* field_0x4;         // +0x04 (owning scene object, retail CScn)
    CScnRootNw4rBuffer* field_0x8;    // +0x08 (buffer, flag at +0x448)
    u32 field_0xC;                    // +0x0C (MemManager ALLOC_HANDLE)

    nw4r::g3d::ScnRoot* mScnRoot;     // +0x10 (nw4r scene root)
    CScnBloom* field_0x14;            // +0x14 (bloom post-effect)
    u8 field_0x18;                    // +0x18 (frame-drawn flag)
    u8 field_0x19;                    // +0x19 (draw-in-progress flag)
    u8 field_0x1A;                    // +0x1A (redraw-request latch)
    u32 field_0x1C;                   // +0x1C (frame counter / pair id)

    u32 mArrA[4];                     // +0x20 (pair table part A)
    s32 mArrB[4];                     // +0x30 (pair table part B)
    u32 mPairCount;                   // +0x40

    u32 mIds[256];                    // +0x44 (id table)
    u32 mIdCount;                     // +0x444

    u8 pad_0x448[0x4AC - 0x448];      // +0x448
    u32 field_0x4AC;                  // +0x4AC (alloc handle, init -1)
    u8 pad_0x4B0[0x4B8 - 0x4B0];      // +0x4B0
    u32 field_0x4B8;                  // +0x4B8 (alloc handle, init -1)
    u8 pad_0x4BC[0x4C4 - 0x4BC];      // +0x4BC
    u32 field_0x4C4;                  // +0x4C4 (alloc handle, init -1)
    u8 pad_0x4C8[0x4D0 - 0x4C8];      // +0x4C8
    u32 field_0x4D0;                  // +0x4D0 (alloc handle, init -1)
};
