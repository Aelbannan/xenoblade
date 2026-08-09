#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

// Forward declarations
namespace nw4r { namespace lyt {
    class Layout;
    class AnimTransform;
    class DrawInfo;
    class ArcResourceAccessor;
}}

class CProcess;
class CScn;

// 16-byte parameter struct used for window entries
struct CMenuUpdate_8014274C {
    u32 field_0;
    u32 field_4;
    u32 field_8;
    u32 field_C;

    void __ct__8014274C() {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        field_C = 0;
    }

    void init_8014274C(u32 v0, u32 v1, u32 v2, u32 v3) {
        field_0 = v0;
        field_4 = v1;
        field_8 = v2;
        field_C = v3;
    }
};

class IUIWindow;

class CTTask_IUIWindow {
public:
    // vtable at +0x00, +0x10
    u8 _00[0x10];
    u32 _10; // vtable
    u8 _14[0x3C - 0x14];
    u32 _3C; // ptmf func
    u32 _40; // ptmf this
    u32 _44; // ptmf delta
    u32 _48; // ptmf func
    u32 _4C; // ptmf this
    u32 _50; // ptmf delta

    CTTask_IUIWindow();
};

class IUIWindow {
public:
    u8 _00[0x10];
    u32 _10; // vtable
    u8 _14[0x54 - 0x14];
    nw4r::lyt::Layout* mLayout; // 0x54
    void* mField58; // 0x58
    void* mField5C; // 0x5C
    s32 mField60; // 0x60, default -1
    u8 mField64; // 0x64, bool
    u8 mField65; // 0x65
    u8 mField66; // 0x66
    u8 mField67; // 0x67, bool, default 1
    u8 _68[0x6C - 0x68];

    IUIWindow();
};

class IWorkEvent {
public:
    u32 mVtable; // 0x00

    IWorkEvent();
};

class IScnRender {
public:
    u32 mVtable; // 0x00

    IScnRender();
};

class CMenuUpdate : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    // IUIWindow at +0x00 (size 0x6C)
    // IWorkEvent at +0x6C (size 0x04)
    // IScnRender at +0x70 (size 0x04)
    CScn* mScene; // 0x74
    UnkClass_8045F564 mMemRegion; // 0x78 (size 0x10)
    nw4r::lyt::AnimTransform* mAnim1; // 0x88
    nw4r::lyt::AnimTransform* mAnim2; // 0x8C
    nw4r::lyt::AnimTransform* mActiveAnim; // 0x90
    CMenuUpdate_8014274C mEntries[8]; // 0x94 - 0x113 (8 * 0x10 = 0x80)
    u32 mState; // 0x114
    u32 mSubState; // 0x118
    u32 mSubType; // 0x11C
    s32 mCounter; // 0x120
    u32 mMode; // 0x124, default 2
    f32 mFloat128; // 0x128
    f32 mFloat12C; // 0x12C

    CMenuUpdate(u32 r4, u32 r5, u32 r6, u32 r7, u32 r8);
    virtual ~CMenuUpdate();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // Accessors
    u32 getField10() { return *(u32*)((u8*)this + 0x10); }
    void setField67(u8 val) { mField67 = val; }
    u8 getField64() { return mField64; }
    void setField64(u8 val) { mField64 = val; }
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void __dt__10IScnRenderFv(void*, int);
extern "C" void __dt__10IWorkEventFv(void*, int);
extern "C" void __dt__9IUIWindowFv(void*, int);
extern "C" void __ct__8CProcessFv(void*);
extern "C" void __dl__FPv(void*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void*, int);
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void*);
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void* getInstance__9CTaskGameFv();
extern "C" bool func_800426F0__9CTaskGameFv(void*);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(void*, void*, u32, u32);
extern "C" void removeRenderCB__4CScnFP10IScnRender(void*, void*);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, void*);
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
extern "C" void* getWorkMem__17CWorkThreadSystemFv();
extern "C" void* func_801355F4();
extern "C" void Regist__8CProcessFP8CProcessb(void*, void*, bool);
extern "C" void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(void*, void*, const char*);
extern "C" void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(void*, void**, void*, char*);
extern "C" void func_8013676C(void*, void*);
extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
extern "C" void func_80137250__FPQ34nw4r3lyt8DrawInfo(void*);
extern "C" bool func_80137444__FPQ34nw4r3lyt13AnimTransformf(void*, f32);
extern "C" void func_80137E7C(void*, const char*, void*);
extern "C" void func_80137B44(void*, const char*, int);
extern "C" void func_80136B4C(void*, const char*, void*, int);
// func_8013639C: declared with char* return in code_80135FDC.hpp
extern "C" u16 func_80136254(void*, const char*, int);
extern "C" u16 func_8013606C(const char*, const char*, int);
// func_80136190: declared with char* return in code_80135FDC.hpp (caller-tuned)
extern "C" void func_80138078__FUl(u32);
extern "C" void func_80135898();
extern "C" void func_80133A08(u32);
extern "C" void func_801347EC(u32);
extern "C" void func_8013D7C0(u32);
extern "C" void func_80134F2C(u32);
extern "C" bool func_80122450();
extern "C" bool func_80226B94();
extern "C" void func_80124270(void*, bool);
extern "C" void func_80124288(void*, void*);
extern "C" bool func_8006EF04__Fi(int);
extern "C" bool func_801BCF38();
extern "C" bool func_8029A658();
extern "C" bool func_8029EE58();
extern "C" bool func_8013BFA8();
extern "C" void* func_8049603C();
extern "C" bool code80135FDC_getByte_64059();
extern "C" int func_8013BE50();
extern "C" void* CfRes_getE14();
extern "C" bool func_8009CF8C(int);
extern "C" void func_8009D018(int, int);
extern "C" void format__Q22ml10FixStr_32_FPCce(void*, const char*, ...);
extern "C" char* func_80138F78(u32);
extern "C" bool func_800829B8__Q22cf13CfGameManagerFv();
extern "C" bool func_8008585C__Q22cf13CfGameManagerFv();
extern "C" bool func_80085840__Q22cf13CfGameManagerFv();

// C-linkage helpers for thunks
extern "C" void* func_80142B4C(void* self, u32 r4, int r5, int r6, int r7, int r8);
extern "C" void func_80142C64(void* self);
extern "C" void* getGlobalA10();
extern "C" int getGlobal0E0();
extern "C" void func_80142C80();
extern "C" void func_80142CA0(void* self, void* r4, void* r5);
extern "C" void* noop_80142D5C(void* self);
extern "C" void func_80142D60(void* self);
extern "C" void func_80143ADC(void* self, u32 r4, u32 r5, u32 r6, u32 r7);
extern "C" int func_80143F78(void* self);
extern "C" void func_80144070(void* self);
extern "C" void func_801440A8(void* self);
extern "C" f32 func_801443E4();
extern "C" void func_80144410(void* self);