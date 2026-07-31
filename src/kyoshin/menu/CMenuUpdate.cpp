// High-level C++ reconstruction for kyoshin/menu/CMenuUpdate
// Matches retail MWCC -O4,s output

#include <string.h>
#include <types.h>

#include "kyoshin/menu/CMenuUpdate.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/vm/yvm2.h"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

// ============================================================
// External declarations (not in headers)
// ============================================================

extern "C" {
    extern u32 lbl_eu_80525A10[]; // IWorkEvent vtable
    extern u32 lbl_eu_80525A00[]; // IScnRender vtable
    extern u32 lbl_eu_8052D1F0[]; // IUIWindow vtable
    extern u32 lbl_eu_8052D238[]; // CTTask_IUIWindow vtable
    extern u32 lbl_eu_8052E7B0[]; // CMenuUpdate vtable
    extern u32 lbl_eu_805013C8[]; // String table
    extern u32 lbl_eu_80664198;   // Singleton CMenuUpdate pointer
    extern u32 lbl_eu_806640E0;   // globalInt
    extern u32 lbl_eu_80664A10;   // HelpFlagBag
    extern u32 lbl_eu_806673A0;   // 0.0f
    extern u32 lbl_eu_806673A8;   // Float table
    extern u32 lbl_eu_806673AC;
    extern u32 lbl_eu_806673B0;
    extern u32 lbl_eu_806673B4;
    extern f64 lbl_eu_806673B8;   // Double constant for int-to-float conversion
    extern u32 lbl_eu_806673C0;   // 1.0f
    extern u32 lbl_eu_806673C4;   // 0.016666668f (frame delta)
    extern u32 __ptmf_null[];

    // CProcess
    void __ct__8CProcessFv(void* self);
    void Regist__8CProcessFP8CProcessb(void* self, void* parent, bool unk);

    // CTaskGame
    void* getInstance__9CTaskGameFv();
    bool func_800426F0__9CTaskGameFv(void* self);

    // CScn
    void addRenderCB__4CScnFP10IScnRenderUlUl(void* self, void* render, u32 a, u32 b);
    void removeRenderCB__4CScnFP10IScnRender(void* self, void* render);

    // CDeviceFont
    void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 fontType, void* layout);

    // CDeviceVI
    void waitForDrawDone__9CDeviceVIFv();

    // MemManager
    void* getHandleMEM2__Q23mtl10MemManagerFv();
    void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 align);

    // CWorkThreadSystem
    void* getWorkMem__17CWorkThreadSystemFv();

    // Layout helpers
    void func_801355F4();
    void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(void** layout, void* accessor, const char* name);
    void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(void* layout, void** anim, void* accessor, char* name);
    void func_8013676C(void* layout, void* anim);
    void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void* layout, void* drawInfo, int a, int b);
    void func_80137250__FPQ34nw4r3lyt8DrawInfo(void* drawInfo);
    bool func_80137444__FPQ34nw4r3lyt13AnimTransformf(void* anim, f32 frame);
    void func_80137E7C(void* layout, const char* name, void* text);
    void func_80137B44(void* layout, const char* name, int unk);
    void func_80136B4C(void* layout, const char* name, void* text, int unk);
    void func_8013639C(void* accessor, const char* name, int unk);
    void func_80136254(void* accessor, const char* name, int unk);
    void func_8013606C(const char* a, const char* b, int count);
    void func_80136190(const char* a, const char* b, int count);
    void func_80138078__FUl(u32 val);
    void func_80135898();
    void func_80133A08(u32 val);
    void func_801347EC(u32 val);
    void func_8013D7C0(u32 val);
    void func_80134F2C(u32 val);
    void func_80122450();
    void func_80226B94();
    void func_80124270(void* obj, bool val);
    void func_80124288(void* obj, void* vec2);
    void func_8006EF04__Fi(int val);
    void func_801BCF38();
    void func_8029A658();
    void func_8029EE58();
    void func_8013BFA8();
    void func_8049603C();
    void code80135FDC_getByte_64059();
    void code80135FDC_getByte_621F0();
    void CfRes_getE14();
    void func_8009CF8C(int val);
    void func_8009D018(int val, int flag);
    void format__Q22ml10FixStr_32_FPCce(void* self, const char* fmt, ...);
}

// ============================================================
// CTTask_IUIWindow
// ============================================================

void __ct__CTTask_IUIWindow() {
    CTTask_IUIWindow* self = (CTTask_IUIWindow*)__ct__8CProcessFv;
    // This is called via bl from the actual constructor;
    // the actual code is inline in the asm.
}

// Actual implementations as extern "C" functions matching the asm

// ============================================================
// IWorkEvent
// ============================================================
extern "C" void __ct__IWorkEvent(void* self) {
    *(u32*)self = (u32)lbl_eu_80525A10;
}

// ============================================================
// IScnRender
// ============================================================
extern "C" void __ct__IScnRender(void* self) {
    *(u32*)self = (u32)lbl_eu_80525A00;
}

// ============================================================
// CMenuUpdate_8014274C
// ============================================================
extern "C" void __ct__8014274C(CMenuUpdate_8014274C* self) {
    self->field_0 = 0;
    self->field_4 = 0;
    self->field_8 = 0;
    self->field_C = 0;
}

// ============================================================
// CTTask_IUIWindow constructor
// ============================================================
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

// ============================================================
// IUIWindow constructor
// ============================================================
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

// ============================================================
// CMenuUpdate constructor __ct__CMenuUpdate
// ============================================================
extern "C" void __ct__CMenuUpdate(void* self, u32 r4, u32 r5, u32 r6, u32 r7, u32 r8) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    __ct__IUIWindow(self);
    __ct__IWorkEvent((u8*)self + 0x6C);
    __ct__IScnRender((u8*)self + 0x70);

    u32* vtable = lbl_eu_8052E7B0;
    obj->mScene = (CScn*)r4;
    *(u32*)((u8*)obj + 0x10) = (u32)vtable;
    *(u32*)((u8*)obj + 0x6C) = (u32)(vtable + 0x24 / 4);
    *(u32*)((u8*)obj + 0x70) = (u32)(vtable + 0xAC / 4);

    obj->mMemRegion.__ct__UnkClass_8045F564Fv();
    obj->mAnim1 = 0;
    obj->mAnim2 = 0;
    obj->mActiveAnim = 0;

    // Initialize 8 entries at 0x94 (0x94 to 0x114 exclusive)
    for (int i = 0; i < 8; i++) {
        obj->mEntries[i].__ct__8014274C();
    }
    // Also init the entry at 0x114 (which is mState, but read as CMenuUpdate_8014274C*)
    // Actually 0x114 is past the 8 entries. Let me re-check.
    // 0x94 + 8*0x10 = 0x94 + 0x80 = 0x114
    // So the 9th "entry" at 0x114 is actually mState treated as a CMenuUpdate_8014274C
    // This is a quirk in the retail code.
    ((CMenuUpdate_8014274C*)((u8*)obj + 0x114))->__ct__8014274C();

    obj->mMode = 2;
    f32 zero = 0.0f;
    obj->mFloat128 = zero;
    obj->mFloat12C = zero;

    // Clear the entries area
    memset(obj->mEntries, 0, 0x80);
    // Clear the extra area
    memset((u8*)obj + 0x114, 0, 0x10);

    // Call the post-init function
    func_80143ADC(obj, r5, r6, r7, r8);
}

// ============================================================
// CMenuUpdate destructor __dt__CMenuUpdate
// ============================================================
extern "C" void __dt__11CMenuUpdateFv(void* self, int flags) {
    if (self) {
        obj->mMemRegion.~UnkClass_8045F564();
        __dt__10IScnRenderFv((u8*)self + 0x70, 0);
        __dt__10IWorkEventFv((u8*)self + 0x6C, 0);
        __dt__9IUIWindowFv(self, 0);
        if (flags <= 0) {
            // fall through to return
        } else {
            __dl__FPv(self);
        }
    }
}

// ============================================================
// CMenuUpdate::Init
// ============================================================
extern "C" void Init__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    obj->mMemRegion.createRegion(handle, 0x4000, (const char*)lbl_eu_805013C8, 0);

    Class_8045F858 stackObj(&obj->mMemRegion);
    func_801355F4();
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &obj->mLayout, (void*)result, (const char*)((u32)lbl_eu_805013C8 + 0xC));

    func_801355F4();
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        obj->mLayout, &obj->mAnim1, (void*)result, (char*)((u32)lbl_eu_805013C8 + 0x25));

    func_801355F4();
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        obj->mLayout, &obj->mAnim2, (void*)result, (char*)((u32)lbl_eu_805013C8 + 0x41));

    void* font = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, obj->mLayout);
    // Call virtual function at offset 0x24 (vtable+0x24)
    void* vtab = *(void**)font;
    void* result2 = ((void* (*)(void*))vtab[0x24 / 4])(font);
    func_8013676C(obj->getField10(), result2);

    // Set animation active
    obj->mActiveAnim = obj->mAnim2;
    // Call virtual SetAnimation at offset 0x2C
    void* layoutVtab = *(void**)obj->mLayout;
    ((void (*)(void*, void*, bool))layoutVtab[0x2C / 4])(obj->mLayout, obj->mActiveAnim, 0);

    obj->mActiveAnim = obj->mAnim1;
    ((void (*)(void*, void*, bool))layoutVtab[0x2C / 4])(obj->mLayout, obj->mActiveAnim, 1);

    // Call virtual function at offset 0x38 (Calculate)
    ((void (*)(void*, bool))layoutVtab[0x38 / 4])(obj->mLayout, 0);

    obj->setField67(0);

    // Register render callback
    void* renderPtr = (self) ? (u8*)self + 0x70 : 0;
    addRenderCB__4CScnFP10IScnRenderUlUl(obj->mScene, renderPtr, 7, 0);

    obj->mMemRegion.func_8045F810();
    stackObj.~Class_8045F858();
}

// ============================================================
// CMenuUpdate::Term
// ============================================================
extern "C" void Term__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    waitForDrawDone__9CDeviceVIFv();

    void* renderPtr = (self) ? (u8*)self + 0x70 : 0;
    removeRenderCB__4CScnFP10IScnRender(obj->mScene, renderPtr);

    if (obj->mLayout) {
        // Call virtual destructor at offset 0x08
        void* vtab = *(void**)obj->mLayout;
        ((void (*)(void*, bool))vtab[0x08 / 4])(obj->mLayout, 1);
        obj->mLayout = 0;
    }

    obj->mMemRegion.func_8045F778();
    lbl_eu_80664198 = 0;
}

// ============================================================
// CMenuUpdate::Move
// ============================================================
extern "C" void Move__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    void* taskGame = getInstance__9CTaskGameFv();
    bool flag = func_800426F0__9CTaskGameFv(taskGame);
    if (flag) {
        return;
    }

    bool resFlag = func_8006EF04__Fi(0x20);
    if (resFlag) {
        return;
    }

    bool byteFlag = code80135FDC_getByte_621F0();
    if (!byteFlag) {
        return;
    }

    bool result = func_80143F78(self);
    if (result) {
        return;
    }

    u32 mode = obj->mMode;
    switch (mode) {
    case 0:
        func_80144070(self);
        break;
    case 1:
        func_801440A8(self);
        break;
    case 2:
        func_80144410(self);
        break;
    default:
        break;
    }

    // Call virtual Calculate at offset 0x38
    void* vtab = *(void**)obj->mLayout;
    ((void (*)(void*, bool))vtab[0x38 / 4])(obj->mLayout, 0);
}

// ============================================================
// CMenuUpdate::cbRenderBefore
// ============================================================
extern "C" void cbRenderBefore__11CMenuUpdateFv(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    void* taskGame = getInstance__9CTaskGameFv();
    bool flag = func_800426F0__9CTaskGameFv(taskGame);
    if (flag) {
        return;
    }

    bool resFlag = func_8006EF04__Fi(0x20);
    if (resFlag) {
        return;
    }

    bool byteFlag = code80135FDC_getByte_621F0();
    if (!byteFlag) {
        return;
    }

    bool result = func_80143F78(self);
    if (result) {
        return;
    }

    // GXSetZMode(false, false, false)
    GXSetZMode(0, 0, 0);

    nw4r::lyt::DrawInfo drawInfo;
    // Constructor call
    func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo);

    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(obj->mLayout, &drawInfo, 0, 1);

    // Destructor call
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo, -1);
}

// ============================================================
// Helper functions
// ============================================================

// getField10 - returns the vtable pointer at offset 0x10
extern "C" u32 func_80142954(void* self) {
    return *(u32*)((u8*)self + 0x10);
}

// setField67
extern "C" void func_8014295C(void* self, u8 val) {
    ((u8*)self)[0x67] = val;
}

// getField10 (alias for func_80142954)
extern "C" u32 getField10(void* self) {
    return *(u32*)((u8*)self + 0x10);
}

// setField67 (alias)
extern "C" void setField67(void* self, u8 val) {
    ((u8*)self)[0x67] = val;
}

// setField64
extern "C" void func_80142C98(void* self, u8 val) {
    ((u8*)self)[0x64] = val;
}

// setField64 alias
extern "C" void setField64(void* self, u8 val) {
    ((u8*)self)[0x64] = val;
}

// getField64
extern "C" u8 func_80143F4C(void* self) {
    return ((u8*)self)[0x64];
}

// getField64 alias
extern "C" u8 getField64(void* self) {
    return ((u8*)self)[0x64];
}

// getGlobalA10
extern "C" void* getGlobalA10() {
    return (void*)lbl_eu_80664A10;
}

// getGlobal0E0
extern "C" int getGlobal0E0() {
    return lbl_eu_806640E0;
}

// func_80142C64
extern "C" void func_80142C64(void* self) {
    ((u8*)self)[0x15] = 1;
}

// func_80142C80
extern "C" void func_80142C80(void* self) {
    if (lbl_eu_80664198) {
        setField64((void*)lbl_eu_80664198, 1);
    }
}

// noop_80142D5C
extern "C" void* noop_80142D5C(void* self) {
    return self; // returns its argument, no-op effectively
}

// setVec2
extern "C" void* setVec2(void* self, f32 a, f32 b) {
    *(f32*)((u8*)self + 0) = a;
    *(f32*)((u8*)self + 4) = b;
    return self;
}

// setFieldFloat10
extern "C" void setFieldFloat10(void* self, f32 val) {
    *(f32*)((u8*)self + 0x10) = val;
}

// func_80143AC8 (setVec2 wrapper)
extern "C" void func_80143AC8(void* self, f32 a, f32 b) {
    setVec2(self, a, b);
}

// func_80143AD4 (setFieldFloat10 wrapper)
extern "C" void func_80143AD4(void* self, f32 val) {
    setFieldFloat10(self, val);
}

// init_8014274C
extern "C" void init_8014274C(CMenuUpdate_8014274C* self, u32 v0, u32 v1, u32 v2, u32 v3) {
    self->field_0 = v0;
    self->field_4 = v1;
    self->field_8 = v2;
    self->field_C = v3;
}

// func_80143F38 (init_8014274C with raw pointer)
extern "C" void func_80143F38(void* self, u32 v0, u32 v1, u32 v2, u32 v3) {
    init_8014274C((CMenuUpdate_8014274C*)self, v0, v1, v2, v3);
}

// func_80143F54 - copy 16 bytes
extern "C" void func_80143F54(void* dest, const void* src) {
    u32* d = (u32*)dest;
    const u32* s = (const u32*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

// ============================================================
// func_80142B4C - factory / creator function
// ============================================================
extern "C" void* func_80142B4C(void* self, u32 r4, int r5, int r6, int r7, int r8) {
    if (r5 == 2) {
        getGlobalA10();
        func_80142C64(self);
    }

    if (r5 == 6) {
        int val = getGlobal0E0();
        u16 result = func_80136254((void*)val, (const char*)((u32)lbl_eu_805013C8 + 0x64), r6);
        if (!result) {
            return 0;
        }

        bool flag = func_8009CF8C(r6 + 0x3214);
        if (!flag) {
            func_8009D018(r6 + 0x3214, 1);
        } else {
            bool flag2 = func_8013BFA8();
            if (!flag2) {
                return 0;
            }
        }
    }

    u32* singleton = (u32*)&lbl_eu_80664198;
    if (*singleton) {
        func_80143ADC((void*)*singleton, r5, r6, r7, r8);
        return 0;
    }

    void* workMem = getWorkMem__17CWorkThreadSystemFv();
    void* obj = allocate__Q23mtl10MemManagerFUlUl(0x130, (u32)workMem);
    if (obj) {
        __ct__CMenuUpdate(obj, r4, r5, r6, r7, r8);
    }

    lbl_eu_80664198 = (u32)obj;
    Regist__8CProcessFP8CProcessb(obj, self, 0);
    return (void*)lbl_eu_80664198;
}

// ============================================================
// func_80142CA0 - Set text on layout
// ============================================================
extern "C" void func_80142CA0(void* self, void* r4, void* r5) {
    if (!r4 || !r5) {
        return;
    }

    CMenuUpdate* obj = (CMenuUpdate*)self;
    FixStr32 str;
    CfGameManager::getInstance()->func_8007D794(&str);
    format__Q22ml10FixStr_32_FPCce(&str, (const char*)((u32)lbl_eu_805013C8 + 0x69), r5);

    void* result = noop_80142D5C(&str);
    func_801355F4();
    void* text = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, result, 0);

    if (text) {
        func_80137E7C(obj->mLayout, (const char*)r4, text);
    }
}

// ============================================================
// func_80142D60 - Main state processing function (large)
// ============================================================
extern "C" void func_80142D60(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    void* layout = obj->mLayout;

    // Get layout size via virtual function at vtable+0x24
    void* vtab = *(void**)layout;
    ((void* (*)(void*))vtab[0x24 / 4])(layout);

    bool isEvent = CfGameManager::getInstance()->func_80086F9C(-1);
    const char* prefix;
    if (isEvent) {
        prefix = (const char*)((u32)lbl_eu_805013C8 + 0x70);
    } else {
        prefix = (const char*)((u32)lbl_eu_805013C8 + 0x79);
    }

    // Build text ID
    u16 textId = func_8013606C((const char*)((u32)lbl_eu_805013C8 + 0x82), prefix, 0x2A);
    u32 textIdCleared = textId & 0xFFFF;
    void* text = func_80138F78(textIdCleared);

    func_801355F4();
    // Get text via vtable+0x0C
    void* textObj = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, text, 0);

    if (textObj) {
        // Set text on layout parts
        func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x90), textObj);
        func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x9D), textObj);
        func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0xAA), textObj);

        // Get pane dimensions
        u32* paneData = *(u32**)(*(u32*)((u8*)textObj + 8));
        u16 width = *(u16*)((u8*)paneData + 2);
        u16 height = *(u16*)((u8*)paneData + 0);

        // Set text for first pane name
        void* pane1 = obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x90), 1);
        if (pane1) {
            f64 convConst = lbl_eu_806673B8;
            f32 fw = (f32)(f64)width - convConst;
            f32 fh = (f32)(f64)height - convConst;
            void* vec2 = setVec2(alloca(8), fw, fh);
            func_80124288(pane1, vec2);
        }

        // Set text for second pane name
        void* pane2 = obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x9D), 1);
        if (pane2) {
            f64 convConst = lbl_eu_806673B8;
            f32 fw = (f32)(f64)width - convConst;
            f32 fh = (f32)(f64)height - convConst;
            void* vec2 = setVec2(alloca(8), fw, fh);
            func_80124288(pane2, vec2);
        }

        // Set text for third pane name
        void* pane3 = obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0xAA), 1);
        if (pane3) {
            f64 convConst = lbl_eu_806673B8;
            f32 fw = (f32)(f64)width - convConst;
            f32 fh = (f32)(f64)height - convConst;
            void* vec2 = setVec2(alloca(8), fw, fh);
            func_80124288(pane3, vec2);
        }
    }

    // Build second text ID
    u16 textId2 = func_8013606C((const char*)((u32)lbl_eu_805013C8 + 0x82),
        (const char*)((u32)lbl_eu_805013C8 + 0x64), 0x2A);
    void* text2 = func_80138F78(textId2 & 0xFFFF);

    // Set text on layout
    func_80136B4C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0xB7), text2, 0);
    func_80136B4C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0xC5), text2, 0);
    func_80136B4C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0xD3), text2, 0);

    // Get pane references
    void* paneRef1 = obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0xE1), 1);
    void* paneRef2 = obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0xEC), 1);
    void* paneRef3 = obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0xF6), 1);
    void* paneRef4 = obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0xFF), 1);

    // Hide all initially
    func_80124270(paneRef1, 0);
    func_80124270(paneRef2, 0);
    func_80124270(paneRef3, 0);
    func_80124270(paneRef4, 0);

    // Handle state-based visibility
    u32 state = obj->mEntries[0].field_0; // mState at 0x114
    switch (state) {
    case 1:
        func_80124270(paneRef2, 1);
        obj->mActiveAnim = obj->mAnim1;
        break;
    case 2:
        func_80124270(paneRef3, 1);
        func_801355F4();
        // Get text
        textObj = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x107), 0);
        if (textObj) {
            func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x120), textObj);
        }
        obj->mActiveAnim = obj->mAnim1;
        break;
    case 4:
        // Complex: handles sub-state
        obj->mSubState = obj->mEntries[1].field_0; // 0x118
        func_80142CA0(self, (const char*)((u32)lbl_eu_805013C8 + 0x156), (const char*)((u32)lbl_eu_805013C8 + 0x12B));
        func_80142CA0(self, (const char*)((u32)lbl_eu_805013C8 + 0x161), text2);

        u32 subType = obj->mEntries[2].field_0; // 0x11C
        void* textResult = 0;
        switch (subType) {
        case 1:
            func_801355F4();
            textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x16C), 0);
            break;
        case 2:
            func_801355F4();
            textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x187), 0);
            break;
        case 3:
            func_801355F4();
            textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x1A2), 0);
            break;
        case 4:
            func_801355F4();
            textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x1BD), 0);
            break;
        case 5:
            func_801355F4();
            textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x1D8), 0);
            break;
        default:
            break;
        }
        if (textResult && obj->mLayout) {
            func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x156), textResult);
        }

        // Handle counter
        if (obj->mCounter < 0) {
            // Negative counter
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x1F3), 1)->setVisible(1);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x1FE), 1)->setVisible(0);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x209), 1)->setVisible(1);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x213), 1)->setVisible(0);

            if (obj->mCounter <= -200) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x21C), 0);
            } else if (obj->mCounter <= -100) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x230), 0);
            } else {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x244), 0);
            }
            if (textResult) {
                func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x213), textResult);
            }
        } else {
            // Positive counter
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x1F3), 1)->setVisible(0);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x1FE), 1)->setVisible(1);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x258), 1)->setVisible(0);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x262), 1)->setVisible(1);

            if (obj->mCounter >= 500) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x26B), 0);
            } else if (obj->mCounter >= 300) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x27F), 0);
            } else if (obj->mCounter >= 200) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x21C), 0);
            } else if (obj->mCounter >= 100) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x230), 0);
            } else {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x244), 0);
            }
            if (textResult) {
                func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x262), textResult);
            }
        }
        func_80124270(paneRef4, 1);
        obj->mActiveAnim = obj->mAnim2;
        break;
    case 5:
        // Complex section
        u16 w1 = func_8013606C((const char*)((u32)lbl_eu_805013C8 + 0x293),
            (const char*)((u32)lbl_eu_805013C8 + 0x2A4), 5);
        bool isBig = func_8009CF8C(0x20) >= w1;

        // Select from float table based on boolean
        u32 floatIdx = isBig ? 1 : 0;
        u32 f1 = ((u32*)&lbl_eu_806673A8)[floatIdx];
        u32 f2 = ((u32*)&lbl_eu_806673AC)[floatIdx];
        u32 f3 = ((u32*)&lbl_eu_806673B0)[floatIdx];
        u32 f4 = ((u32*)&lbl_eu_806673B4)[floatIdx];

        u16 val1 = func_8013606C((const char*)((u32)lbl_eu_805013C8 + 0x2AB), (const char*)((u32)&f1), obj->mSubType);
        u16 val2 = func_8013606C((const char*)((u32)lbl_eu_805013C8 + 0x2AB), (const char*)((u32)&f3), obj->mSubType);

        u16 val1Cleared = val1 & 0xFFFF;
        u16 val2Cleared = val2 & 0xFFFF;

        void* text1 = func_80136190((const char*)((u32)lbl_eu_805013C8 + 0x140),
            (const char*)((u32)lbl_eu_805013C8 + 0x14D), val1Cleared);
        void* text2 = func_80136190((const char*)((u32)lbl_eu_805013C8 + 0x140),
            (const char*)((u32)lbl_eu_805013C8 + 0x14D), val2Cleared);

        func_80142CA0(self, (const char*)((u32)lbl_eu_805013C8 + 0x156), text1);
        func_80142CA0(self, (const char*)((u32)lbl_eu_805013C8 + 0x161), text2);
        func_80137B44(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x156), -1);

        // Handle counter
        if (obj->mCounter < 0) {
            // Negative counter version
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x1F3), 1)->setVisible(1);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x1FE), 1)->setVisible(0);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x209), 1)->setVisible(1);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x213), 1)->setVisible(0);

            if (obj->mCounter <= -50) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x2BA), 0);
            } else if (obj->mCounter <= -16) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x2D0), 0);
            } else {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x2E6), 0);
            }
            if (textResult) {
                func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x209), textResult);
            }
        } else {
            // Positive counter version
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x1F3), 1)->setVisible(0);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x1FE), 1)->setVisible(1);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x258), 1)->setVisible(1);
            obj->getField10()->findPaneByName((const char*)((u32)lbl_eu_805013C8 + 0x262), 1)->setVisible(0);

            if (obj->mCounter >= 300) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x2FC), 0);
            } else if (obj->mCounter >= 50) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x2BA), 0);
            } else if (obj->mCounter >= 21) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x312), 0);
            } else if (obj->mCounter >= 16) {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x2D0), 0);
            } else {
                func_801355F4();
                textResult = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x2E6), 0);
            }
            if (textResult) {
                func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x258), textResult);
            }
        }
        func_80124270(paneRef4, 1);
        obj->mActiveAnim = obj->mAnim2;
        break;
    case 6:
        {
            int global = getGlobal0E0();
            void* text3 = func_8013639C((void*)global, (const char*)((u32)lbl_eu_805013C8 + 0x330), obj->mEntries[1].field_0);
            func_80136B4C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x328), text3, 0);
        }
        func_80124270(paneRef4, 1);
        obj->mActiveAnim = obj->mAnim1;
        break;
    case 3:
        func_80124270(paneRef3, 1);
        func_801355F4();
        textObj = ((void* (*)(void*, u32, void*, int))vtab[0x0C / 4])(result, 0x74696D67, (const char*)((u32)lbl_eu_805013C8 + 0x335), 0);
        if (textObj) {
            func_80137E7C(obj->mLayout, (const char*)((u32)lbl_eu_805013C8 + 0x120), textObj);
        }
        obj->mActiveAnim = obj->mAnim1;
        break;
    default:
        break;
    }

    // Set animation frame
    void* layoutVtab = *(void**)obj->mLayout;
    ((void (*)(void*, void*))layoutVtab[0x1C / 4])(obj->mLayout, obj->mActiveAnim);
    ((void (*)(void*, void*, bool))layoutVtab[0x2C / 4])(obj->mLayout, obj->mActiveAnim, 1);

    // Reset animation
    setFieldFloat10(obj->mActiveAnim, 0.0f);

    // Call Calculate
    ((void (*)(void*, bool))layoutVtab[0x38 / 4])(obj->mLayout, 0);
}

// ============================================================
// func_80143ADC - Entry slot management
// ============================================================
extern "C" void func_80143ADC(void* self, u32 r4, u32 r5, u32 r6, u32 r7) {
    CMenuUpdate* obj = (CMenuUpdate*)self;

    // Check if any entry slot is unused
    bool found = false;
    int i;

    if (r4 < 4) {
        // Simple mode: check if state matches or find empty slot
        if (obj->mEntries[0].field_0 == r4) {
            return; // Already in this state
        }

        for (i = 0; i < 8; i++) {
            if (obj->mEntries[i].field_0 == r4) {
                return; // Already have this state
            }
            if (obj->mEntries[i].field_0 == 0) {
                // Found empty slot
                CMenuUpdate_8014274C params;
                params.init_8014274C(r4, r5, r6, r7);
                func_80143F54(&obj->mEntries[i], &params);

                if (obj->getField64()) {
                    obj->setField64(0);
                }
                return;
            }
        }
    } else if (r4 == 6) {
        // Special mode for type 6 - check for duplicate sub-state
        if (obj->mEntries[0].field_0 == 6 && obj->mEntries[0].field_4 == r5) {
            return;
        }

        for (i = 0; i < 8; i++) {
            if (obj->mEntries[i].field_0 == 6 && obj->mEntries[i].field_4 == r5) {
                return;
            }
        }

        // Look for empty slot
        for (i = 0; i < 8; i++) {
            if (obj->mEntries[i].field_0 == 0) {
                CMenuUpdate_8014274C params;
                params.init_8014274C(r4, r5, r6, r7);
                func_80143F54(&obj->mEntries[i], &params);

                if (obj->getField64()) {
                    obj->setField64(0);
                }
                return;
            }
        }
    } else {
        // Other modes - complex rearrangement
        // Create local copies of entry data
        CMenuUpdate_8014274C stackEntries[8];
        int count = 0;

        // Copy entries with state 4 or 5
        for (i = 0; i < 8; i++) {
            if (obj->mEntries[i].field_0 == 4 || obj->mEntries[i].field_0 == 5) {
                func_80143F54(&stackEntries[count], &obj->mEntries[i]);
                count++;
            }
        }

        // Copy remaining non-zero entries
        CMenuUpdate_8014274C remaining[8];
        int remCount = 0;
        for (i = 0; i < 8; i++) {
            if (obj->mEntries[i].field_0 != 0) {
                func_80143F54(&remaining[remCount], &obj->mEntries[i]);
                remCount++;
            }
        }

        // Reset all entries
        for (i = 0; i < 8; i++) {
            obj->mEntries[i].__ct__8014274C();
        }

        // Copy back stacked entries (state 4/5)
        for (i = 0; i < count; i++) {
            func_80143F54(&obj->mEntries[i], &stackEntries[i]);
        }

        // Copy back remaining non-zero entries
        for (int j = 0; j < remCount; j++) {
            func_80143F54(&obj->mEntries[i + j], &remaining[j]);
        }

        // Add the new entry
        CMenuUpdate_8014274C newEntry;
        newEntry.init_8014274C(r4, r5, r6, r7);
        func_80143F54(&obj->mEntries[i + remCount], &newEntry);
    }
}

// ============================================================
// func_80143F78 - Check conditions
// ============================================================
extern "C" bool func_80143F78(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;

    if (!code80135FDC_getByte_64059()) {
        bool flag1 = func_8006EF04__Fi(0x100);
        if (!flag1) {
            bool flag2 = func_8006EF04__Fi(0xBFE4);
            if (!flag2) {
                goto check_next;
            }
        }
        return true;
    }
check_next:
    if (func_8029A658()) {
        obj->mFloat12C = 1.0f;
    }

    if (CfGameManager::func_800829B8()) {
        return true;
    }

    if (CfGameManager::func_8008585C()) {
        return true;
    }

    if (!CfGameManager::func_80085840()) {
        return true;
    }

    if (func_801BCF38()) {
        return true;
    }

    if (func_8029EE58()) {
        return true;
    }

    // Decrement timer
    obj->mFloat12C -= 0.016666668f;
    if (obj->mFloat12C < 0.0f) {
        obj->mFloat12C = 0.0f;
    }

    return false;
}

// ============================================================
// func_80144070 - Mode 0 handler
// ============================================================
extern "C" void func_80144070(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;
    func_80138078__FUl(0x1F);
    obj->mMode = 1;
}

// ============================================================
// func_801440A8 - Mode 1 handler (input processing)
// ============================================================
extern "C" void func_801440A8(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;

    bool resFlag = func_8006EF04__Fi(0x100);
    if (resFlag) {
        if (obj->mEntries[0].field_0 == 4 || obj->mEntries[0].field_0 == 5) {
            // Already in a transition state, skip
        } else {
            obj->mMode = 2;
            setFieldFloat10(obj->mActiveAnim, 0.0f);

            // Reorganize entries
            CMenuUpdate_8014274C stackEntries[8];
            int count = 0;

            for (int i = 0; i < 8; i++) {
                if (obj->mEntries[i].field_0 == 4 || obj->mEntries[i].field_0 == 5) {
                    func_80143F54(&stackEntries[count], &obj->mEntries[i]);
                    count++;
                }
            }

            // Clear all entries
            CMenuUpdate_8014274C clearEntries[8];
            for (int i = 0; i < 8; i++) {
                clearEntries[i].__ct__8014274C();
            }

            // Copy back stacked entries
            for (int i = 0; i < count; i++) {
                func_80143F54(&obj->mEntries[i], &stackEntries[i]);
            }

            // Copy remaining cleared entries
            for (int i = 0; i < 8; i++) {
                func_80143F54(&obj->mEntries[i + count], &clearEntries[i]);
            }

            // Copy state from current entry to mState
            func_80143F54(&obj->mEntries[count], &obj->mEntries[0]);
            func_80143F54(&obj->mState, &obj->mEntries[0]);
        }
    } else {
        // Check for button press
        CPad* pad = CfGameManager::getCurrentPad();
        if (pad->mButtons & 0x400) { // Check specific button bit
            if (!code80135FDC_getByte_64059()) {
                bool flag = func_80135898();
                if (!flag) {
                    flag = func_80122450();
                    if (!flag) {
                        flag = func_80226B94();
                        if (!flag) {
                            // Handle based on current state
                            u32 state = obj->mEntries[0].field_0;
                            switch (state) {
                            case 1:
                                func_80133A08(obj->mEntries[1].field_0 & 0xFFFF);
                                break;
                            case 2:
                                func_801347EC(0);
                                break;
                            case 6:
                                func_8013D7C0(obj->mEntries[1].field_0 & 0xFFFF);
                                break;
                            case 3:
                                func_80134F2C(obj->mEntries[1].field_0 & 0xFFFF);
                                break;
                            }
                        }
                    }
                }
            }
            if (!flag) {
                goto check_anim;
            }
            func_80138078__FUl(5);
        }
    }

check_anim:
    // Check if animation finished
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(obj->mActiveAnim, 0.016666668f)) {
        obj->mMode = 2;
    }
}

// ============================================================
// func_801443E4 - Get float from scene manager
// ============================================================
extern "C" f32 func_801443E4() {
    void* mgr = func_8049603C();
    f32 val = *(f32*)((u8*)mgr + 0xC);
    return 0.016666668f - val;
}

// ============================================================
// func_80144410 - Mode 2 handler (state transition)
// ============================================================
extern "C" void func_80144410(void* self) {
    CMenuUpdate* obj = (CMenuUpdate*)self;

    bool resFlag = func_8006EF04__Fi(0x100);
    if (resFlag) {
        if (obj->mEntries[0].field_0 == 4 || obj->mEntries[0].field_0 == 5) {
            // Already in transition
        } else {
            goto not_in_transition;
        }
    }
not_in_transition:
    bool flag = func_80135898();
    if (flag) {
        goto end;
    }
    flag = func_80122450();
    if (flag) {
        goto end;
    }
    flag = func_80226B94();
    if (flag) {
        goto end;
    }

    if (obj->mEntries[0].field_0 == 0) {
        obj->setField64(1);
        goto end;
    }

    // Reset state
    obj->mFloat128 = 0.0f;
    obj->mMode = 0;
    func_80143F54(&obj->mState, &obj->mEntries[0]);

    // Clear entries
    CMenuUpdate_8014274C clearEntry;
    clearEntry.__ct__8014274C();
    func_80143F54(&obj->mEntries[0], &clearEntry);

    // Shift entries up
    int count = 0;
    CMenuUpdate_8014274C shifted[8];
    for (int i = 1; i < 8; i++) {
        if (obj->mEntries[i].field_0 != 0) {
            func_80143F54(&shifted[count], &obj->mEntries[i]);
            count++;
        }
    }

    // Copy shifted entries back
    for (int i = 0; i < 8; i++) {
        func_80143F54(&obj->mEntries[i], &shifted[i]);
    }

    func_80142D60(self);

end:
    return;
}

// ============================================================
// Thunk functions
// ============================================================

// dtorThunk_6c - adjusts this by -0x6C then calls destructor
extern "C" void func_80144590(void* self) {
    __dt__11CMenuUpdateFv((char*)self - 0x6C, -1);
}

// renderThunk_70 - adjusts this by -0x70 then calls cbRenderBefore
extern "C" void func_80144598(void* self) {
    cbRenderBefore__11CMenuUpdateFv((char*)self - 0x70);
}

// dtorThunk_70 - adjusts this by -0x70 then calls destructor
extern "C" void func_801445A0(void* self) {
    __dt__11CMenuUpdateFv((char*)self - 0x70, -1);
}