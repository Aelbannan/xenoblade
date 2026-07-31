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

// C-linkage helpers for thunks
extern "C" void func_80142B4C(void* self, u32 r4, int r5, int r6, int r7, int r8);
extern "C" void func_80142C64(void* self);
extern "C" void* getGlobalA10();
extern "C" int getGlobal0E0();
extern "C" void func_80142C80(void* self);
extern "C" void func_80142CA0(void* self, void* r4, void* r5);
extern "C" void noop_80142D5C(void* self);
extern "C" void func_80142D60(void* self);
extern "C" void func_80143ADC(void* self, u32 r4, u32 r5, u32 r6, u32 r7);
extern "C" void func_80143F78(void* self);
extern "C" void func_80144070(void* self);
extern "C" void func_801440A8(void* self);
extern "C" void func_801443E4();
extern "C" void func_80144410(void* self);