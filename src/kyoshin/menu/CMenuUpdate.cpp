// High-level C++ reconstruction for kyoshin/menu/CMenuUpdate
#include <string.h>
#include <types.h>
#include "kyoshin/menu/CMenuUpdate.hpp"
#include "libs/monolib/src/scn/CScn_8049603C.hpp" // func_8049603C (single owner decl)
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/cf/CfGameManagerUnityHelpers.hpp" // code80135FDC_getByte_64059 (owner-API decl)
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "monolib/util/FixStr.hpp"
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTypes.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_resourceAccessor.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>

extern u32 lbl_eu_80525A10[];
extern u32 lbl_eu_80525A00[];
extern u32 lbl_eu_8052D1F0[];
extern u32 lbl_eu_8052D238[];
extern u32 lbl_eu_8052E7B0[];
extern u32 lbl_eu_805013C8[];
extern u32 lbl_eu_80664198;
extern u32 lbl_eu_806640E0;
extern f32 lbl_eu_806673A0;
extern u32 lbl_eu_806673A8[];
extern u32 lbl_eu_806673AC[];
extern u32 lbl_eu_806673B0[];
extern u32 lbl_eu_806673B4[];
extern f64 lbl_eu_806673B8;
extern f32 lbl_eu_806673C0;
extern f32 lbl_eu_806673C4;
extern u32 __ptmf_null[];

// Out-of-line accessor helpers (defined further down; retail calls them via bl)
extern "C" u32 getField10(u8* self);
extern "C" void setField67(u8* self, u8 val);
extern "C" void setField64(u8* self, u8 val);
extern "C" u8 getField64(u8* self);

// IWorkEvent
extern "C" __declspec(noinline) void __ct__IWorkEvent(void* self) {
    *(u32*)self = (u32)lbl_eu_80525A10;
}

// IScnRender
extern "C" __declspec(noinline) void __ct__IScnRender(void* self) {
    *(u32*)self = (u32)lbl_eu_80525A00;
}

// CMenuUpdate_8014274C ctor
// Returns self so callers can nest it as the src argument of func_80143F54
// (retail emits 'bl __ct__; mr r4,r3' shapes).
extern "C" __declspec(noinline) CMenuUpdate_8014274C* __ct__8014274C(CMenuUpdate_8014274C* self) {
    self->field_0 = 0; self->field_4 = 0; self->field_8 = 0; self->field_C = 0;
    return self;
}

// CTTask_IUIWindow ctor: base CProcess ctor imported; stores second-base
// vtable and two null pointers-to-member-function (each 12 bytes)
// kept out-of-line: retail emits this as a standalone callee of IUIWindow's ctor
// (retail label is the C-linkage name __ct__CTTask_IUIWindow)
extern "C" __declspec(noinline) void __ct__CTTask_IUIWindow(void* self) {
    __ct__8CProcessFv(self);
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052D238;
    u32* ptmf = __ptmf_null;
    *(u32*)((u8*)self + 0x40) = ptmf[1];
    *(u32*)((u8*)self + 0x3C) = ptmf[0];
    *(u32*)((u8*)self + 0x44) = ptmf[2];
    *(u32*)((u8*)self + 0x4C) = ptmf[1];
    *(u32*)((u8*)self + 0x48) = ptmf[0];
    *(u32*)((u8*)self + 0x50) = ptmf[2];
}

// IUIWindow ctor: retail keeps the CTTask init as a separate callee
extern "C" __declspec(noinline) void __ct__IUIWindow(void* self) {
    __ct__CTTask_IUIWindow(self);
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052D1F0;
    *(u32*)((u8*)self + 0x54) = 0;
    *(u32*)((u8*)self + 0x58) = 0;
    *(u32*)((u8*)self + 0x5C) = 0;
    *(s32*)((u8*)self + 0x60) = -1;
    *(u8*)((u8*)self + 0x64) = 0;
    *(u8*)((u8*)self + 0x65) = 0;
    *(u8*)((u8*)self + 0x66) = 0;
    *(u8*)((u8*)self + 0x67) = 1;
    *(u32*)((u8*)self + 0x68) = 0;
}

// CMenuUpdate ctor
extern "C" void __ct__CMenuUpdate(void* self, u32 r4, u32 r5, u32 r6, u32 r7, u32 r8) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    __ct__IUIWindow(self);
    __ct__IWorkEvent((u8*)self + 0x6C);
    __ct__IScnRender((u8*)self + 0x70);

    u32* vtable = lbl_eu_8052E7B0;
    obj->mScene = (CScn*)r4;
    *(u32*)((u8*)obj + 0x10) = (u32)vtable;
    *(u32*)((u8*)obj + 0x6C) = (u32)(vtable + 9);
    *(u32*)((u8*)obj + 0x70) = (u32)(vtable + 43);

    __ct__17UnkClass_8045F564Fv(&obj->mMemRegion);
    obj->mAnim1 = NULL;
    obj->mAnim2 = NULL;
    obj->mActiveAnim = NULL;

    // 8 entry slots plus one more header-sized slot over the state block
    CMenuUpdate_8014274C* entryEnd = (CMenuUpdate_8014274C*)&obj->mState;
    CMenuUpdate_8014274C* entry = &obj->mEntries[0];
    do {
        __ct__8014274C(entry);
        entry++;
    } while (entry < entryEnd);
    __ct__8014274C((CMenuUpdate_8014274C*)&obj->mState);

    obj->mMode = 2;
    f32 zero = lbl_eu_806673A0;
    obj->mFloat128 = zero;
    obj->mFloat12C = zero;
    memset(obj->mEntries, 0, 0x80);
    memset(&obj->mEntries[8], 0, 0x10);

    func_80143ADC(obj, r5, r6, r7, r8);
}

// CMenuUpdate dtor
extern "C" void __dt__11CMenuUpdateFv(void* self, int flags) {
    if (self) {
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x78, -1);
        __dt__10IScnRenderFv((u8*)self + 0x70, 0);
        __dt__10IWorkEventFv((u8*)self + 0x6C, 0);
        __dt__9IUIWindowFv(self, 0);
        if (flags > 0) __dl__FPv(self);
    }
}

// Init
extern "C" void Init__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    u32 handle = (u32)getHandleMEM2__Q23mtl10MemManagerFv();
    obj->mMemRegion.createRegion(handle, 0x4000, (const char*)lbl_eu_805013C8, 0);

    Class_8045F858 stackObj(&obj->mMemRegion);

    void* accessor = func_801355F4();
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc((void*)&obj->mLayout, accessor, (const char*)((u32)lbl_eu_805013C8 + 0xC));

    accessor = func_801355F4();
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc((void*)obj->mLayout, (void**)&obj->mAnim1, accessor, (char*)((u32)lbl_eu_805013C8 + 0x25));

    accessor = func_801355F4();
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc((void*)obj->mLayout, (void**)&obj->mAnim2, accessor, (char*)((u32)lbl_eu_805013C8 + 0x41));

    // Retail passes &mAnim2 here - the address was just materialized for the
    // preceding func_80136F08 call, so MWCC CSEs away the recompute.
    u8* font = (u8*)func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, (nw4r::lyt::Layout*)&obj->mAnim2);
    void* fontVtab = *(void**)font;
    void* result2 = ((void* (*)(void*))((void**)fontVtab)[0x24 / 4])(font);
    func_8013676C((void*)getField10((u8*)obj), result2);

    obj->mActiveAnim = obj->mAnim2;
    void* layoutVtab = *(void**)obj->mLayout;
    ((void (*)(void*, void*, bool))((void**)layoutVtab)[0x2C / 4])(obj->mLayout, obj->mActiveAnim, 0);
    obj->mActiveAnim = obj->mAnim1;
    ((void (*)(void*, void*, bool))((void**)layoutVtab)[0x2C / 4])(obj->mLayout, obj->mActiveAnim, 1);
    ((void (*)(void*, bool))((void**)layoutVtab)[0x38 / 4])(obj->mLayout, 0);

    ::setField67((u8*)obj, 0);
    addRenderCB__4CScnFP10IScnRenderUlUl(obj->mScene, (u8*)self + 0x70, 7, 0);
    obj->mMemRegion.func_8045F810();
    // stackObj destructor called automatically
}

// Term
extern "C" void Term__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    waitForDrawDone__9CDeviceVIFv();
    u8* renderPtr = self ? (u8*)self + 0x70 : 0;
    removeRenderCB__4CScnFP10IScnRender(obj->mScene, renderPtr);
    if (obj->mLayout) {
        void* vtab = *(void**)obj->mLayout;
        ((void (*)(void*, bool))((void**)vtab)[0x08 / 4])(obj->mLayout, 1);
        obj->mLayout = 0;
    }
    obj->mMemRegion.func_8045F778();
    lbl_eu_80664198 = 0;
}

// Move
extern "C" void Move__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    void* taskGame = getInstance__9CTaskGameFv();
    // Short-circuit OR gate: first test -> bne exit, res flag -> beq continue /
    // b exit (CMenuShopBuy::Move shape).
    if (func_800426F0__9CTaskGameFv(taskGame) || testResInfoFlag(0x200000))
        return;
    if (func_8013BE50() == 0) return;
    if (func_80143F78(self)) return;

    switch ((int)obj->mMode) {
    case 0: func_80144070(self); break;
    case 1: func_801440A8(self); break;
    case 2: func_80144410(self); break;
    }

    // Layout::Animate(0) - virtual slot 0x38
    obj->mLayout->Animate(0);
}

// cbRenderBefore
extern "C" void cbRenderBefore__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    void* taskGame = getInstance__9CTaskGameFv();
    // Short-circuit OR gate (CMenuShopBuy::cbRenderBefore shape).
    if (func_800426F0__9CTaskGameFv(taskGame) || testResInfoFlag(0x200000))
        return;
    if (func_8013BE50() == 0) return;
    if (func_80143F78(self)) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);

    nw4r::lyt::DrawInfo drawInfo;
    func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo);
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(obj->mLayout, &drawInfo, 0, 1);
    // No explicit dtor: the implicit member destruction emits the retail's
    // single __dt__ call (an explicit call would double it).
}

// Helper accessors
extern "C" __declspec(noinline) u32 getField10(u8* self) { return *(u32*)(self + 0x10); }
extern "C" __declspec(noinline) void setField67(u8* self, u8 val) { self[0x67] = val; }
extern "C" u32 func_80142954(u8* self) { return *(u32*)(self + 0x10); }
extern "C" void func_8014295C(u8* self, u8 val) { self[0x67] = val; }
extern "C" __declspec(noinline) void func_80142C98(u8* self, u8 val) { self[0x64] = val; }
extern "C" __declspec(noinline) void setField64(u8* self, u8 val) { self[0x64] = val; }
extern "C" u8 func_80143F4C(u8* self) { return self[0x64]; }
extern "C" __declspec(noinline) u8 getField64(u8* self) { return self[0x64]; }
extern "C" __declspec(noinline) void* getGlobalA10() { return (void*)lbl_eu_80664A10; }
extern "C" __declspec(noinline) int getGlobal0E0() { return lbl_eu_806640E0; }
extern "C" __declspec(noinline) void func_80142C64(void* self) { ((u8*)self)[0x15] = 1; }
extern "C" void func_80142C80() { if (lbl_eu_80664198) { ((void(*)(void*, u8))setField64)((void*)(u32)lbl_eu_80664198, 1); } }
extern "C" __declspec(noinline) void* noop_80142D5C(void* self) { return self; }
extern "C" void* setVec2(void* self, f32 a, f32 b) { *(f32*)self = a; *(f32*)((u8*)self + 4) = b; return self; }
extern "C" __declspec(noinline) void setFieldFloat10(void* self, f32 val) { *(f32*)((u8*)self + 0x10) = val; }
extern "C" void func_80143AC8(void* self, f32 a, f32 b) { setVec2(self, a, b); }
extern "C" void func_80143AD4(void* self, f32 val) { setFieldFloat10(self, val); }
// Returns self (see __ct__8014274C note).
extern "C" __declspec(noinline) CMenuUpdate_8014274C* init_8014274C(CMenuUpdate_8014274C* self, u32 v0, u32 v1, u32 v2, u32 v3) {
    self->field_0 = v0; self->field_4 = v1; self->field_8 = v2; self->field_C = v3;
    return self;
}
extern "C" void func_80143F38(CMenuUpdate_8014274C* self, u32 v0, u32 v1, u32 v2, u32 v3) {
    init_8014274C(self, v0, v1, v2, v3);
}
extern "C" __declspec(noinline) void func_80143F54(u32* dest, const u32* src) {
    const u32* s = src; u32* d = dest;
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
}

// func_80142B4C - factory
extern "C" void* func_80142B4C(void* self, u32 r4, int r5, int r6, int r7, int r8) {
    if (r5 == 2) { func_80142C64(getGlobalA10()); }
    if (r5 == 6) {
        int val = getGlobal0E0();
        u16 result = func_80136254((void*)val, (const char*)((u32)lbl_eu_805013C8 + 0x64), r6);
        if (!result) return 0;
        if (!func_8009CF8C(r6 + 0x3214)) func_8009D018(r6 + 0x3214, 1);
        else if (!func_8013BFA8()) return 0;
    }
    if (lbl_eu_80664198) {
        func_80143ADC((void*)(u32)lbl_eu_80664198, r5, r6, r7, r8);
        return 0;
    }
    void* workMem = getWorkMem__17CWorkThreadSystemFv();
    void* obj = allocate__Q23mtl10MemManagerFUlUl(0x130, (u32)workMem);
    if (obj) __ct__CMenuUpdate(obj, r4, r5, r6, r7, r8);
    lbl_eu_80664198 = (u32)obj;
    Regist__8CProcessFP8CProcessb(obj, self, 0);
    return (void*)(u32)lbl_eu_80664198;
}

// func_80142CA0 - format a texture name into a FixStr<32> buffer and bind it
// into the layout. The FixStr default-init is the out-of-line CfGameManager
// helper (retail shape).
extern "C" void func_80142CA0(void* self, void* name, void* fmtArg) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    if (!name) return;
    if (!fmtArg) return;

    u8 buf[0x24];
    // FixStr<32> default-init resolves to the retail CfGameManager-scope helper
    reinterpret_cast<cf::CfGameManager*>(buf)->func_8007D794();
    reinterpret_cast<ml::FixStr<32>*>(buf)->format(
        (const char*)((u32)lbl_eu_805013C8 + 0x69), fmtArg);
    void* str = noop_80142D5C(buf);

    nw4r::lyt::ResourceAccessor* accessor =
        (nw4r::lyt::ResourceAccessor*)func_801355F4();
    void* text = accessor->GetResource(0x74696D67, (const char*)str, NULL);
    if (text) func_80137E7C(obj->mLayout, (const char*)name, text);
}

// func_80142D60 - (re)bind the window layout's textures, animations and pane
// visibility for the current state, then start the active animation.
// Retail reloads mLayout from the object before every use rather than caching
// it; mirror that so register allocation matches.
#define layout (obj->mLayout)
extern "C" __declspec(noinline) void func_80142D60(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    const char* pool = (const char*)lbl_eu_805013C8;

    layout->UnbindAllAnimation();

    // Shared icon texture: message id picked by controller type.
    char* handle = func_80138F78(func_8013606C(
        pool + 0x82, cf::CfGameManager::func_80086F9C(-1) ? pool + 0x70 : pool + 0x79, 0x2a));
    void* tex = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                    ->GetResource(0x74696D67 /* 'timg' */, handle, NULL);
    if (tex != NULL) {
        func_80137E7C(layout, pool + 0x90, tex);
        func_80137E7C(layout, pool + 0x9d, tex);
        func_80137E7C(layout, pool + 0xaa, tex);

        // Texture object carries pixel dimensions through a two-level chain.
        u16 h = ((CMenuUpdateTexObj*)tex)->mChain->mDims->mH;
        u16 w = ((CMenuUpdateTexObj*)tex)->mChain->mDims->mW;

        nw4r::lyt::Pane* pane =
            ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x90, true);
        if (pane != NULL) {
            float v[2];
            setVec2(v, (f32)h, (f32)w);
            func_80124288(pane, v);
        }
        pane = ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x9d, true);
        if (pane != NULL) {
            float v[2];
            setVec2(v, (f32)h, (f32)w);
            func_80124288(pane, v);
        }
        pane = ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0xaa, true);
        if (pane != NULL) {
            float v[2];
            setVec2(v, (f32)h, (f32)w);
            func_80124288(pane, v);
        }
    }

    // Bind the three state animations and hide all four state panes.
    char* animName = func_80136190(pool + 0x82, pool + 0x64, 0x2a);
    func_80136B4C(layout, pool + 0xb7, animName, 0);
    func_80136B4C(layout, pool + 0xc5, animName, 0);
    func_80136B4C(layout, pool + 0xd3, animName, 0);

    nw4r::lyt::Pane* paneA =
        ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0xe1, true);
    nw4r::lyt::Pane* paneB =
        ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0xec, true);
    nw4r::lyt::Pane* paneC =
        ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0xf6, true);
    nw4r::lyt::Pane* paneD =
        ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0xff, true);
    func_80124270(paneA, 0);
    func_80124270(paneB, 0);
    func_80124270(paneC, 0);
    func_80124270(paneD, 0);

    switch (obj->mState) {
    case 1:
        func_80124270(paneB, 1);
        obj->mActiveAnim = obj->mAnim1;
        break;

    case 2: {
        func_80124270(paneC, 1);
        void* res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                        ->GetResource(0x74696D67, pool + 0x107, NULL);
        if (res != NULL) {
            func_80137E7C(layout, pool + 0x120, res);
        }
        obj->mActiveAnim = obj->mAnim1;
        break;
    }

    case 4: {
        char* s = func_80136190(pool + 0x140, pool + 0x14d, obj->mSubState + 0x6f);
        func_80142CA0(obj, (void*)(pool + 0x156), s);
        func_80142CA0(obj, (void*)(pool + 0x161), s);

        // Sub-type specific texture over the first text slot.
        void* res = NULL;
        switch (obj->mSubType) {
        case 1:
            res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                      ->GetResource(0x74696D67, pool + 0x16c, NULL);
            break;
        case 2:
            res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                      ->GetResource(0x74696D67, pool + 0x187, NULL);
            break;
        case 3:
            res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                      ->GetResource(0x74696D67, pool + 0x1a2, NULL);
            break;
        case 4:
            res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                      ->GetResource(0x74696D67, pool + 0x1bd, NULL);
            break;
        case 5:
            res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                      ->GetResource(0x74696D67, pool + 0x1d8, NULL);
            break;
        }
        if (res != NULL) {
            func_80137E7C(layout, pool + 0x156, res);
        }

        // Counter sign selects the countdown vs count-up gauge group; the
        // magnitude picks the number-strip texture bound to the gauge pane.
        if (obj->mCounter < 0) {
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x1f3, true),
                1);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x1fe, true),
                0);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x209, true),
                0);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x213, true),
                1);
            const char* name;
            if (obj->mCounter <= -200) {
                name = pool + 0x21c;
            } else if (obj->mCounter <= -100) {
                name = pool + 0x230;
            } else {
                name = pool + 0x244;
            }
            res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                      ->GetResource(0x74696D67, name, NULL);
            if (res != NULL) {
                func_80137E7C(layout, pool + 0x213, res);
            }
        } else {
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x1f3, true),
                0);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x1fe, true),
                1);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x258, true),
                0);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x262, true),
                1);
            const char* name;
            if (obj->mCounter >= 500) {
                name = pool + 0x26b;
            } else if (obj->mCounter >= 300) {
                name = pool + 0x27f;
            } else if (obj->mCounter >= 200) {
                name = pool + 0x21c;
            } else if (obj->mCounter >= 100) {
                name = pool + 0x230;
            } else {
                name = pool + 0x244;
            }
            res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                      ->GetResource(0x74696D67, name, NULL);
            if (res != NULL) {
                func_80137E7C(layout, pool + 0x262, res);
            }
        }
        func_80124270(paneA, 1);
        obj->mActiveAnim = obj->mAnim2;
        break;
    }

    case 5: {
        // Wide-vs-narrow number strip chosen by whether 32 digits fit.
        u16 len = func_8013606C(pool + 0x293, pool + 0x2a4, 5);
        u32 wideFlag = 0;
        if ((u32)func_8009CF8C(0x20) >= (u32)len) {
            wideFlag = 1;
        }

        u32 pairA[2];
        u32 pairB[2];
        pairB[0] = lbl_eu_806673A8[wideFlag];
        pairB[1] = lbl_eu_806673AC[wideFlag];
        pairA[0] = lbl_eu_806673B0[wideFlag];
        pairA[1] = lbl_eu_806673B4[wideFlag];

        char* s1 = func_80136190(
            pool + 0x140, pool + 0x14d,
            func_8013606C(pool + 0x2ab, (const char*)(u32)obj->mSubType, pairB[wideFlag]));
        char* s2 = func_80136190(
            pool + 0x140, pool + 0x14d,
            func_8013606C(pool + 0x2ab, (const char*)(u32)obj->mSubType, pairA[wideFlag]));
        func_80142CA0(obj, (void*)(pool + 0x156), s1);
        func_80142CA0(obj, (void*)(pool + 0x161), s2);
        func_80137B44(layout, pool + 0x156, -1);

        if (obj->mCounter < 0) {
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x1f3, true),
                1);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x1fe, true),
                0);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x209, true),
                1);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x213, true),
                0);
            const char* name;
            if (obj->mCounter <= -50) {
                name = pool + 0x2ba;
            } else if (obj->mCounter <= -16) {
                name = pool + 0x2d0;
            } else {
                name = pool + 0x2e6;
            }
            void* res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                            ->GetResource(0x74696D67, name, NULL);
            if (res != NULL) {
                func_80137E7C(layout, pool + 0x209, res);
            }
        } else {
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x1f3, true),
                0);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x1fe, true),
                1);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x258, true),
                1);
            func_80124270(
                ((nw4r::lyt::Pane*)getField10((u8*)layout))->FindPaneByName(pool + 0x262, true),
                0);
            const char* name;
            if (obj->mCounter >= 300) {
                name = pool + 0x2fc;
            } else if (obj->mCounter >= 50) {
                name = pool + 0x2ba;
            } else if (obj->mCounter >= 21) {
                name = pool + 0x312;
            } else if (obj->mCounter >= 16) {
                name = pool + 0x2d0;
            } else {
                name = pool + 0x2e6;
            }
            void* res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                            ->GetResource(0x74696D67, name, NULL);
            if (res != NULL) {
                func_80137E7C(layout, pool + 0x258, res);
            }
        }
        func_80124270(paneA, 1);
        obj->mActiveAnim = obj->mAnim2;
        break;
    }

    case 6:
        func_80136B4C(layout, pool + 0x328,
                      func_8013639C((const void*)getGlobal0E0(), pool + 0x330, obj->mSubState),
                      0);
        func_80124270(paneD, 1);
        obj->mActiveAnim = obj->mAnim1;
        break;

    case 3: {
        func_80124270(paneC, 1);
        void* res = ((nw4r::lyt::ArcResourceAccessor*)func_801355F4())
                        ->GetResource(0x74696D67, pool + 0x335, NULL);
        if (res != NULL) {
            func_80137E7C(layout, pool + 0x120, res);
        }
        obj->mActiveAnim = obj->mAnim1;
        break;
    }
    }

    // Start the active animation from its initial frame.
    layout->BindAnimation(obj->mActiveAnim);
    layout->SetAnimationEnable(obj->mActiveAnim, true);
    setFieldFloat10(obj->mActiveAnim, lbl_eu_806673A0);
    layout->Animate(0);
#undef layout
}

// func_80143ADC
// func_80143ADC - insert/select a window entry.
// r4 = entry type, r5..r7 = payload words. Selects an existing matching entry
// or appends into the first free slot; the general path preserves leading
// entries, clears the table, and re-appends around the new entry.
extern "C" __declspec(noinline) void func_80143ADC(void* self, u32 r4, u32 r5, u32 r6, u32 r7) {
    CMenuUpdate* obj = (CMenuUpdate*)self;

    // Free-slot scan over entries[0..6] only (entry 7 is never tested).
    int found = 0;
    if (obj->mEntries[0].field_0 == 0) found = 1;
    else if (obj->mEntries[1].field_0 == 0) found = 1;
    else if (obj->mEntries[2].field_0 == 0) found = 1;
    else if (obj->mEntries[3].field_0 == 0) found = 1;
    else if (obj->mEntries[4].field_0 == 0) found = 1;
    else if (obj->mEntries[5].field_0 == 0) found = 1;
    else if (obj->mEntries[6].field_0 == 0) found = 1;
    if (!found) return;

    // Insert helper shared by the append paths: fill slot, then clear the
    // one-shot "new window" flag if it was still set.
#define INSERT_ENTRY(slot)                                                    \
    do {                                                                      \
        CMenuUpdate_8014274C e;                                               \
        func_80143F54((u32*)(slot),                                           \
                      (u32*)init_8014274C(&e, r4, r5, r6, r7));               \
        if (!getField64((u8*)self)) return;                                   \
        setField64((u8*)self, 0);                                             \
        return;                                                               \
    } while (0)

    if ((int)r4 < 4) {
        if (obj->mState == r4) return;
        for (int i = 0; i < 8; i++) {
            if (obj->mEntries[i].field_0 == r4) return;
            if (obj->mEntries[i].field_0 == 0) INSERT_ENTRY(&obj->mEntries[i].field_0);
        }
        return;
    }

    if (r4 == 6) {
        if (obj->mSubState == r5) return;
        for (int i = 0; i < 8; i++)
            if (obj->mEntries[i].field_0 == 6 && obj->mEntries[i].field_4 == r5) return;
        for (int i = 0; i < 8; i++)
            if (obj->mEntries[i].field_0 == 0) INSERT_ENTRY(&obj->mEntries[i].field_0);
        return;
    }
#undef INSERT_ENTRY

    // General insert: preserve existing entries, clear the table, then
    // rebuild as [preserved][new entry][remaining non-empty].
    CMenuUpdate_8014274C keepA[10]; // sp+0xc8..
    for (int k = 0; k < 10; k++) __ct__8014274C(&keepA[k]);
    int nA = 0;
    for (int i = 0; i < 8; i++) {
        if (obj->mEntries[i].field_0 == 6) {
            func_80143F54((u32*)&keepA[nA].field_0, (u32*)&obj->mEntries[i].field_0);
            nA++;
        }
    }

    CMenuUpdate_8014274C keepB[11]; // sp+0x18..
    for (int k = 0; k < 11; k++) __ct__8014274C(&keepB[k]);
    int nB = 0;
    for (int i = nA; i < 8 && obj->mEntries[i].field_0 != 0; i++) {
        func_80143F54((u32*)&keepB[nB].field_0, (u32*)&obj->mEntries[i].field_0);
        nB++;
    }

    for (int k = 0; k < 8; k++) {
        CMenuUpdate_8014274C clr;
        clr.__ct__8014274C();
        func_80143F54((u32*)&obj->mEntries[k].field_0, (u32*)&clr.field_0);
    }

    for (int j = 0; j < nA; j++)
        func_80143F54((u32*)&obj->mEntries[j].field_0, (u32*)&keepA[j].field_0);

    CMenuUpdate_8014274C neu;
    func_80143F54((u32*)&obj->mEntries[nA].field_0,
                  (u32*)init_8014274C(&neu, r4, r5, r6, r7));
    nA++;

    for (int j = 0; j < nB; j++)
        func_80143F54((u32*)&obj->mEntries[nA + j].field_0, (u32*)&keepB[j].field_0);
}

// func_80143F78 - pause/transition guard; decrements a fade timer in 0x12C
extern "C" int func_80143F78(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    if (!code80135FDC_getByte_64059() && !func_8006EF04__Fi(0x1000000) &&
        func_8006EF04__Fi((int)0xBFE40000)) {
        return 1;
    }
    if (func_8029A658()) obj->mFloat12C = lbl_eu_806673C0;
    if (func_800829B8__Q22cf13CfGameManagerFv()) return 1;
    if (func_8008585C__Q22cf13CfGameManagerFv()) return 1;
    if (!func_80085840__Q22cf13CfGameManagerFv()) return 1;
    if (func_801BCF38()) return 1;
    if (func_8029EE58()) return 1;
    f32 zero = lbl_eu_806673A0;
    obj->mFloat12C -= lbl_eu_806673C4;
    if (obj->mFloat12C < zero) obj->mFloat12C = zero;
    return 0;
}

// func_80144070
extern "C" __declspec(noinline) void func_80144070(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    func_80138078__FUl(0x1F);
    obj->mMode = 1;
}

// func_801440A8
extern "C" __declspec(noinline) void func_801440A8(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;

    if (func_8006EF04__Fi(0x1000000) && obj->mState != 4 && obj->mState != 5) {
        // Active-window maintenance: compact the table around the active
        // entries (types 4/5) and re-file the saved header block after them.
        obj->mMode = 2;
        setFieldFloat10(obj->mActiveAnim, lbl_eu_806673A0);

        CMenuUpdate_8014274C act[8]; // sp+0xa8
        __ct__8014274C(&act[0]);
        __ct__8014274C(&act[1]);
        __ct__8014274C(&act[2]);
        __ct__8014274C(&act[3]);
        __ct__8014274C(&act[4]);
        __ct__8014274C(&act[5]);
        __ct__8014274C(&act[6]);
        __ct__8014274C(&act[7]);
        u8 iA, nAct;
        nAct = 0;
        iA = 0;
        for (; iA < 8; iA++) {
            s32 f = obj->mEntries[iA].field_0;
            if (f != 4 && f != 5) break;
            func_80143F54((u32*)&act[nAct++], (u32*)&obj->mEntries[iA].field_0);
        }

        CMenuUpdate_8014274C rest[8]; // sp+0x28
        __ct__8014274C(&rest[0]);
        __ct__8014274C(&rest[1]);
        __ct__8014274C(&rest[2]);
        __ct__8014274C(&rest[3]);
        __ct__8014274C(&rest[4]);
        __ct__8014274C(&rest[5]);
        __ct__8014274C(&rest[6]);
        __ct__8014274C(&rest[7]);
        u8 nRest, iB;
        iB = nAct;
        nRest = 0;
        while (iB < 8) {
            if (obj->mEntries[iB].field_0 == 0) break;
            func_80143F54((u32*)&rest[nRest++], (u32*)&obj->mEntries[iB].field_0);
            iB++;
        }

        u8 k;
        for (k = 0; k < 8; k++) {
            CMenuUpdate_8014274C clr;
            func_80143F54((u32*)&obj->mEntries[k].field_0,
                          (u32*)__ct__8014274C(&clr));
        }

        u8 j, w;
        w = 0;
        for (j = 0; j < nAct; j++) {
            func_80143F54((u32*)&obj->mEntries[w++].field_0, (u32*)&act[j].field_0);
        }
        // Re-file the saved header block right after the active entries.
        func_80143F54((u32*)&obj->mEntries[w++].field_0, (u32*)&obj->mState);
        for (j = 0; j < nRest; j++) {
            func_80143F54((u32*)&obj->mEntries[w++].field_0, (u32*)&rest[j].field_0);
        }

        CMenuUpdate_8014274C clrState;
        func_80143F54((u32*)&obj->mState, (u32*)__ct__8014274C(&clrState));
    } else {
        // Confirm-button handling on the state-machine side.
        CMenuUpdatePadData* pad = (CMenuUpdatePadData*)cf::CfGameManager::getCurrentPad();
        if (pad->mPressedButtonFlags & 0x200) {
            if (code80135FDC_getByte_64059() || func_80135898() || func_80122450() ||
                func_80226B94()) {
                func_80138078__FUl(5);
            } else {
                CfRes_getE14();
                f32 v = func_801443E4();
                // Gate on the fade timers before dispatching the state action.
                if (v < lbl_eu_806673C4)
                    if (obj->mFloat12C <= lbl_eu_806673A0) {
                    switch (obj->mState) {
                    case 1: func_80133A08(obj->mSubState & 0xFFFF); break;
                    case 2: func_801347EC(0); break;
                    case 6: func_8013D7C0(obj->mSubState & 0xFFFF); break;
                    case 3: func_80134F2C(obj->mSubState & 0xFFFF); break;
                    }
                }
            }
        }
        if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(obj->mActiveAnim, lbl_eu_806673C4))
            obj->mMode = 2;
    }
}

// func_801443E4
extern "C" __declspec(noinline) f32 func_801443E4() {
    Unk_8049603C* mgr = func_8049603C();
    f32 val = mgr->field_C;
    return lbl_eu_806673C4 - val;
}

// func_80144410
// func_80144410 - idle-state handler (mode 2): closes the front window and
// compacts the remaining non-empty entries toward the front.
extern "C" __declspec(noinline) void func_80144410(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    if (func_8006EF04__Fi(0x1000000) && obj->mEntries[0].field_0 != 4 &&
        obj->mEntries[0].field_0 != 5)
        return;
    if (func_80135898() || func_80122450() || func_80226B94())
        return;

    if (obj->mEntries[0].field_0 == 0) {
        setField64((u8*)self, 1);
        return;
    }

    obj->mFloat128 = lbl_eu_806673A0;
    obj->mMode = 0;
    // Stash the closing entry in the header block, then blank slot 0.
    func_80143F54((u32*)&obj->mState, (u32*)&obj->mEntries[0].field_0);
    {
        CMenuUpdate_8014274C clr;
        func_80143F54((u32*)&obj->mEntries[0].field_0, (u32*)__ct__8014274C(&clr));
    }
    func_80142D60(self);

    // Compact entries[1..7] down over the freed front slots.
    CMenuUpdate_8014274C tmp[8]; // sp+0x18
    __ct__8014274C(&tmp[0]);
    __ct__8014274C(&tmp[1]);
    __ct__8014274C(&tmp[2]);
    __ct__8014274C(&tmp[3]);
    __ct__8014274C(&tmp[4]);
    __ct__8014274C(&tmp[5]);
    __ct__8014274C(&tmp[6]);
    __ct__8014274C(&tmp[7]);
    u8 i, n;
    n = 0;
    i = 1;
    for (; i < 8; i++) {
        if (obj->mEntries[i].field_0 == 0) break;
        func_80143F54((u32*)&tmp[n++], (u32*)&obj->mEntries[i].field_0);
    }
    s32* dst = &obj->mEntries[0].field_0;
    s32* src = &tmp[0].field_0;
    for (int k = 0; k < 8; k++) {
        func_80143F54((u32*)dst, (u32*)src);
        src += 4;
        dst += 4;
    }
}

// Thunks
extern "C" void dtorThunk_6c(void* self) { ((void(*)(void*))__dt__11CMenuUpdateFv)((char*)self - 0x6C); }
extern "C" void renderThunk_70(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuUpdateFv)((char*)self - 0x70); }
extern "C" void dtorThunk_70(void* self) { ((void(*)(void*))__dt__11CMenuUpdateFv)((char*)self - 0x70); }