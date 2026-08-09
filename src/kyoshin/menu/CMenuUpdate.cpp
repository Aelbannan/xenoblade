// High-level C++ reconstruction for kyoshin/menu/CMenuUpdate
#include <string.h>
#include <types.h>
#include "kyoshin/menu/CMenuUpdate.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTypes.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/lyt/lyt_resourceAccessor.h>

extern u32 lbl_eu_80525A10[];
extern u32 lbl_eu_80525A00[];
extern u32 lbl_eu_8052D1F0[];
extern u32 lbl_eu_8052D238[];
extern u32 lbl_eu_8052E7B0[];
extern u32 lbl_eu_805013C8[];
extern u32 lbl_eu_80664198;
extern u32 lbl_eu_806640E0;
extern u32 lbl_eu_80664A10;
extern f32 lbl_eu_806673A0;
extern u32 lbl_eu_806673A8[];
extern u32 lbl_eu_806673AC[];
extern u32 lbl_eu_806673B0[];
extern u32 lbl_eu_806673B4[];
extern f64 lbl_eu_806673B8;
extern f32 lbl_eu_806673C0;
extern f32 lbl_eu_806673C4;
extern u32 __ptmf_null[];

// IWorkEvent
extern "C" void __ct__IWorkEvent(void* self) {
    *(u32*)self = (u32)lbl_eu_80525A10;
}

// IScnRender
extern "C" void __ct__IScnRender(void* self) {
    *(u32*)self = (u32)lbl_eu_80525A00;
}

// CMenuUpdate_8014274C ctor
extern "C" void __ct__8014274C(CMenuUpdate_8014274C* self) {
    self->field_0 = 0; self->field_4 = 0; self->field_8 = 0; self->field_C = 0;
}

// CTTask_IUIWindow
extern "C" void __ct__CTTask_IUIWindow(void* self) {
    __ct__8CProcessFv(self);
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052D238;
    u32* ptmf = __ptmf_null;
    *(u32*)((u8*)self + 0x3C) = ptmf[0];
    *(u32*)((u8*)self + 0x40) = ptmf[1];
    *(u32*)((u8*)self + 0x44) = ptmf[2];
    *(u32*)((u8*)self + 0x48) = ptmf[0];
    *(u32*)((u8*)self + 0x4C) = ptmf[1];
    *(u32*)((u8*)self + 0x50) = ptmf[2];
}

// IUIWindow
extern "C" void __ct__IUIWindow(void* self) {
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
    obj->mAnim1 = 0; obj->mAnim2 = 0; obj->mActiveAnim = 0;

    for (int i = 0; i < 9; i++) __ct__8014274C(&obj->mEntries[i]);

    obj->mMode = 2;
    obj->mFloat128 = 0.0f; obj->mFloat12C = 0.0f;

    memset(obj->mEntries, 0, 0x80);
    memset((u8*)obj + 0x114, 0, 0x10);

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

    u8* font = (u8*)func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, obj->mLayout);
    void* fontVtab = *(void**)font;
    void* result2 = ((void* (*)(void*))((void**)fontVtab)[0x24 / 4])(font);
    func_8013676C((void*)*(u32*)((u8*)obj + 0x10), result2);

    obj->mActiveAnim = obj->mAnim2;
    void* layoutVtab = *(void**)obj->mLayout;
    ((void (*)(void*, void*, bool))((void**)layoutVtab)[0x2C / 4])(obj->mLayout, obj->mActiveAnim, 0);
    obj->mActiveAnim = obj->mAnim1;
    ((void (*)(void*, void*, bool))((void**)layoutVtab)[0x2C / 4])(obj->mLayout, obj->mActiveAnim, 1);
    ((void (*)(void*, bool))((void**)layoutVtab)[0x38 / 4])(obj->mLayout, 0);

    obj->setField67(0);
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
    if (func_800426F0__9CTaskGameFv(taskGame)) return;
    if (func_8006EF04__Fi(0x20)) return;
    if (!func_8013BE50()) return;
    if (func_80143F78(self)) return;

    switch (obj->mMode) {
    case 0: func_80144070(self); break;
    case 1: func_801440A8(self); break;
    case 2: func_80144410(self); break;
    }

    void* vtab = *(void**)obj->mLayout;
    ((void (*)(void*, bool))((void**)vtab)[0x38 / 4])(obj->mLayout, 0);
}

// cbRenderBefore
extern "C" void cbRenderBefore__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    void* taskGame = getInstance__9CTaskGameFv();
    if (func_800426F0__9CTaskGameFv(taskGame)) return;
    if (func_8006EF04__Fi(0x20)) return;
    if (!func_8013BE50()) return;
    if (func_80143F78(self)) return;

    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);

    nw4r::lyt::DrawInfo drawInfo;
    func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo);
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(obj->mLayout, &drawInfo, 0, 1);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo, -1);
}

// Helper accessors
extern "C" u32 func_80142954(u8* self) { return *(u32*)(self + 0x10); }
extern "C" void func_8014295C(u8* self, u8 val) { self[0x67] = val; }
extern "C" u32 getField10(u8* self) { return *(u32*)(self + 0x10); }
extern "C" void setField67(u8* self, u8 val) { self[0x67] = val; }
extern "C" void func_80142C98(u8* self, u8 val) { self[0x64] = val; }
extern "C" void setField64(u8* self, u8 val) { self[0x64] = val; }
extern "C" u8 func_80143F4C(u8* self) { return self[0x64]; }
extern "C" u8 getField64(u8* self) { return self[0x64]; }
extern "C" void* getGlobalA10() { return (void*)lbl_eu_80664A10; }
extern "C" int getGlobal0E0() { return lbl_eu_806640E0; }
extern "C" void func_80142C64(void* self) { ((u8*)self)[0x15] = 1; }
extern "C" void func_80142C80() { if (lbl_eu_80664198) { ((void(*)(void*, u8))setField64)((void*)(u32)lbl_eu_80664198, 1); } }
extern "C" void* noop_80142D5C(void* self) { return self; }
extern "C" void* setVec2(void* self, f32 a, f32 b) { *(f32*)self = a; *(f32*)((u8*)self + 4) = b; return self; }
extern "C" void setFieldFloat10(void* self, f32 val) { *(f32*)((u8*)self + 0x10) = val; }
extern "C" void func_80143AC8(void* self, f32 a, f32 b) { setVec2(self, a, b); }
extern "C" void func_80143AD4(void* self, f32 val) { setFieldFloat10(self, val); }
extern "C" void init_8014274C(CMenuUpdate_8014274C* self, u32 v0, u32 v1, u32 v2, u32 v3) {
    self->field_0 = v0; self->field_4 = v1; self->field_8 = v2; self->field_C = v3;
}
extern "C" void func_80143F38(CMenuUpdate_8014274C* self, u32 v0, u32 v1, u32 v2, u32 v3) {
    init_8014274C(self, v0, v1, v2, v3);
}
extern "C" void func_80143F54(u32* dest, const u32* src) {
    const u32* s = src; u32* d = dest;
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
}

// func_80142B4C - factory
extern "C" void* func_80142B4C(void* self, u32 r4, int r5, int r6, int r7, int r8) {
    if (r5 == 2) { getGlobalA10(); func_80142C64(self); }
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

// func_80142CA0
extern "C" void func_80142CA0(void* self, void* r4, void* r5) {
    if (!r4 || !r5) return;
    CMenuUpdate* obj = (CMenuUpdate*)self;
    char buf[32];
    format__Q22ml10FixStr_32_FPCce(&buf, (const char*)((u32)lbl_eu_805013C8 + 0x69), r5);
    noop_80142D5C(&buf);
    void* accessor = func_801355F4();
    void* vtab = *(void**)accessor;
    void* text = ((void* (*)(void*, u32, void*, int))((void**)vtab)[0x0C / 4])(accessor, 0x74696D67, &buf, 0);
    if (text) func_80137E7C(obj->mLayout, (const char*)r4, text);
}

// func_80142D60
extern "C" void func_80142D60(void* self) {}

// func_80143ADC
extern "C" void func_80143ADC(void* self, u32 r4, u32 r5, u32 r6, u32 r7) {}

// func_80143F78
extern "C" int func_80143F78(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    if (!code80135FDC_getByte_64059()) {
        if (!func_8006EF04__Fi(0x100) && !func_8006EF04__Fi(0xBFE4)) {}
        else return 1;
    }
    if (func_8029A658()) obj->mFloat12C = 1.0f;
    if (func_800829B8__Q22cf13CfGameManagerFv()) return 1;
    if (func_8008585C__Q22cf13CfGameManagerFv()) return 1;
    if (!func_80085840__Q22cf13CfGameManagerFv()) return 1;
    if (func_801BCF38()) return 1;
    if (func_8029EE58()) return 1;
    obj->mFloat12C -= 0.016666668f;
    if (obj->mFloat12C < 0.0f) obj->mFloat12C = 0.0f;
    return 0;
}

// func_80144070
extern "C" void func_80144070(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    func_80138078__FUl(0x1F);
    obj->mMode = 1;
}

// func_801440A8
extern "C" void func_801440A8(void* self) {}

// func_801443E4
extern "C" f32 func_801443E4() {
    void* mgr = func_8049603C();
    return 0.016666668f - *(f32*)((u8*)mgr + 0xC);
}

// func_80144410
extern "C" void func_80144410(void* self) {}

// Thunks
extern "C" void dtorThunk_6c(void* self) { ((void(*)(void*))__dt__11CMenuUpdateFv)((char*)self - 0x6C); }
extern "C" void renderThunk_70(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuUpdateFv)((char*)self - 0x70); }
extern "C" void dtorThunk_70(void* self) { ((void(*)(void*))__dt__11CMenuUpdateFv)((char*)self - 0x70); }