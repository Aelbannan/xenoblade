#pragma once

#include <types.h>
#include <nw4r/math/math_types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/CSelShopWin.hpp"

class CScn;

/*
 * Select-shop menu screen object.
 *
 * Layout mirrors the other menu objects (CMenuQstCnt / CMenuUpdate): the
 * IUIWindow task region is stored inline (vtable at +0x10, pmtf nodes at
 * +0x3C), with the OC/render subobject vtable slots at +0x6C/+0x70 and the
 * owning scene at +0x74, followed by the embedded memory region, shop window
 * widget and CCur18 cursor.
 *
 *   0x00  link-list nodes
 *   0x10  vtable ptr
 *   0x3C  p-mtf null region (IUIWindow/Move)
 *   0x6C  IWorkEvent vtable slot
 *   0x70  IScnRender vtable slot (render-callback subobject)
 *   0x74  CScn* owning scene
 *   0x78  UnkClass_8045F564 memory region
 *   0x88  CSelShopWin shop window widget
 *   0xB0  CCur18 cursor (0x18 bytes raw storage)
 *   0xC8  u8 mState
 *   0xCC  u32 mSelIndex (selection index fed to func_8022C930)
 */
class CMenuSelectShop {
public:
    u8  mPad0[0x10];                       // 0x00 link-list nodes
    u32 mVtab;                             // 0x10 vtable ptr
    u8  mPad14[0x3C - 0x14];               // 0x14
    u32 mPtmfFunc0;                        // 0x3C
    u32 mPtmfThis0;                        // 0x40
    u32 mPtmfDelta0;                       // 0x44
    u32 mPtmfFunc1;                        // 0x48
    u32 mPtmfThis1;                        // 0x4C
    u32 mPtmfDelta1;                       // 0x50
    nw4r::lyt::Layout* mLayout;            // 0x54
    u32 mField58;                          // 0x58
    u32 mField5C;                          // 0x5C
    s32 mField60;                          // 0x60 (default -1)
    u8  mField64;                          // 0x64 (bool)
    u8  mField65;                          // 0x65
    u8  mField66;                          // 0x66
    u8  mField67;                          // 0x67 (default 1)
    u32 mField68;                          // 0x68
    u32 mOcc6C;                            // 0x6C IWorkEvent vtable slot
    u32 mOcc70;                            // 0x70 IScnRender vtable slot
    CScn* mScene;                          // 0x74
    UnkClass_8045F564 mMemRegion;          // 0x78
    CSelShopWin mSelShop;                  // 0x88
    u8  mCursor[0x18];                     // 0xB0 CCur18 cursor storage
    u8  mState;                            // 0xC8
    u8  mPadC9[3];                         // 0xC9-0xCB
    u32 mSelIndex;                         // 0xCC
    u32 mFieldD0;                          // 0xD0 (ctor arg 3, default 0)

    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
};

// Abstract views into the embedded CCur18 vtable (the cursor is stored as raw
// bytes at +0xB0; MWCC inserts an offset-to-top + RTTI prefix so vtable offset
// = (virtual index + 2) * 4). Mirrors the CCur18View in CSysWinSelect.hpp.
class CCursor18 {
public:
    virtual void vf2() = 0;  // index 0 -> +0x08
    virtual void vf3() = 0;  // index 1 -> +0x0C (Term update virtual, no args)
};
class CCur18View {
public:
    virtual void vf02() = 0;       // index 0 -> +0x08
    virtual void vf03(void*) = 0;  // index 1 -> +0x0C
    virtual void vf04(void*) = 0;  // index 2 -> +0x10 (cursor move, VEC3 arg)
};

// Data tail of the embedded CCur18 cursor (+0x4..+0x15). Init copies it from a
// stack-constructed temporary so MWCC emits 4x lwz/stw + 2x lbz/stb.
struct CCur18Data {
    u32 m04;  // +0x4
    u32 m08;  // +0x8
    u32 m0C;  // +0xC
    u32 m10;  // +0x10
    u8  m14;  // +0x14
    u8  m15;  // +0x15
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
// These mirror the declarations used by the sibling menu TUs (CMenuQstCnt,
// CMenuUpdate, CSysWinSelect).
extern "C" CMenuSelectShop* __ct__CMenuSelectShop(CMenuSelectShop* self,
                                                  void* scene, void* a3);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__11CSelShopWinFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dt__9IUIWindowFv(void*, int);
extern "C" void __dl__FPv(void*);
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void removeRenderCB__4CScnFP10IScnRender(void*, void*);
extern "C" void func_801D216C(void*, int);
extern "C" void func_8022C930(nw4r::math::VEC3*, CSelShopWin*, int);

// Shop-window widget hooks. Retail keeps these unmangled (C linkage); they take
// the CSelShopWin explicitly. The is-loaded/animation-active queries return an
// int so the caller's `!= 0` emits a plain cmpi (no u8 zero-extend).
extern "C" void func_8022C85C(CSelShopWin*);
extern "C" int func_8022C8D0(CSelShopWin*);
extern "C" int func_8022C8D8(CSelShopWin*);
extern "C" void func_8022C8E0(CSelShopWin*);

// Select-shop singleton pointer (.sbss) cleared by Term.
extern u32 lbl_eu_806642E8;

// Menu/gate and cursor helper imports (retail C-ABI symbols; keep un-mangled).
extern "C" int func_8013BE50();
extern "C" int func_8018C180();
extern "C" int func_8018B398();

// Task-game gate helpers. Declared with C linkage because CTaskGame.hpp
// defines a minimal local CLibHbm view that clashes with the full CLibHbm
// pulled in by CSelShopWin.hpp -> monolib/lib.hpp (same scheme as
// CMenuUpdate / CMenuShopBuy). isFlag01Set takes the task-game instance.
extern "C" void* getInstance__9CTaskGameFv();
extern "C" bool isFlag01Set__9CTaskGameFv(void* taskGame);
extern "C" void func_8022C7C0(CSelShopWin*);
extern "C" void func_8022C770(CSelShopWin*);
extern "C" void func_8022C830(CSelShopWin*, nw4r::lyt::DrawInfo*);
extern "C" void func_801D202C(void*);
extern "C" void func_801D20B0(void*, nw4r::lyt::DrawInfo*);
extern "C" void func_80137250__FPQ34nw4r3lyt8DrawInfo(void*);
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(void*);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(void*, int);
extern "C" void func_80138078__FUl(u32);
extern "C" void* func_801355F4();
extern "C" void __ct__8CProcessFv(void*);
extern "C" void __ct__CCur18(void*, void*);
extern "C" void __ct__17UnkClass_8045F564Fv(void*);
extern "C" void __ct__CSelShopWin(void*);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, void*, u32, u32);
extern "C" void validateHeap__17UnkClass_8045F564Fv(void*);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, UnkClass_8045F564*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);

// State-machine handlers (retail C-ABI helpers taking the menu object).
extern "C" void func_8018A200(CMenuSelectShop*);
extern "C" void func_8018A248(CMenuSelectShop*);
extern "C" void func_8018A2C0(CMenuSelectShop*);
extern "C" void func_8018A4A0(CMenuSelectShop*);

// Shop-window close animation hook + select-shop selection actions (retail
// unmangled C-ABI symbols; defined in CSelShopWin/CUICfManager TUs).
extern "C" void func_8022C908(CSelShopWin*);
extern "C" void func_80134024(u32);
extern "C" void func_80134100();

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 lbl_eu_80663E28;             // cbRenderBefore gate flag (.sbss)
extern u32 __ptmf_null[];               // null pointer-to-member-function constant
extern char lbl_eu_8052D238[];          // IUIWindow temp base vtable
extern char lbl_eu_80532550[];          // composite vtable
extern char lbl_eu_80503AE0[];          // memory-region name string (.rodata)