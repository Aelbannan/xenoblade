#pragma once

#include <types.h>
#include <nw4r/math/math_types.h>
#include "kyoshin/CBaseCur.hpp"

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/IUIWindow.hpp"
#include "kyoshin/CSelShopWin.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CUICfManager.hpp"

class CScn;

/*
 * Select-shop menu screen object.
 *
 * Real MI class, from the retail composite vtable __vt__15CMenuSelectShop
 * (0x80532550): IUIWindow at +0x00 (0x6C), IWorkEvent at +0x6C and
 * IScnRender at +0x70; the +0x24 / +0xAC sub-vtables and the base list are
 * the compiler's own output.
 *
 *   0x00  IUIWindow base (CTTask<IUIWindow> Move/Draw ptmf hooks at +0x3C/+0x48)
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base (render-callback subobject)
 *   0x74  CScn* owning scene
 *   0x78  UnkClass_8045F564 memory region
 *   0x88  CSelShopWin shop window widget
 *   0xB0  CCur18 cursor (0x18 bytes)
 *   0xC8  u8 mState
 *   0xCC  u32 mSelIndex (selection index fed to getShopWinPanePos)
 *   0xD0  u32 mFieldD0 (ctor arg 3, default 0)
 * retail sizeof = 0xD4 (what ShopSel_CreateSingleton allocates).
 */
class CMenuSelectShop : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    CMenuSelectShop(void* scene, void* a3);
    virtual ~CMenuSelectShop();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    CScn* mScene;                   // 0x74
    UnkClass_8045F564 mMemRegion;   // 0x78
    CSelShopWin mSelShop;           // 0x88
    CCur18 mCursor;                 // 0xB0
    u8  mState;                     // 0xC8
    u8  mPadC9[3];                  // 0xC9-0xCB
    u32 mSelIndex;                  // 0xCC
    u32 mFieldD0;                   // 0xD0 (ctor arg 3, default 0)
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
// These mirror the declarations used by the sibling menu TUs (CMenuQstCnt,
// CMenuUpdate, CSysWinSelect).
extern "C" void __dt__11CSelShopWinFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dt__9IUIWindowFv(void*, int);
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
extern "C" void getShopWinPanePos(nw4r::math::VEC3*, CSelShopWin*, int);

// Shop-window widget hooks. Retail keeps these unmangled (C linkage); they take
// the CSelShopWin explicitly. The is-loaded/animation-active queries return an
// int so the caller's `!= 0` emits a plain cmpi (no u8 zero-extend).

// Select-shop singleton pointer (.sbss) cleared by Term.
extern u32 lbl_eu_806642E8;

// Menu/gate and cursor helper imports (retail C-ABI symbols; keep un-mangled).
extern "C" int ShopBuy_HasInstance();
extern "C" int MenuShopSellIsCreated();

// Task-game gate helpers. Declared with C linkage because CTaskGame.hpp
// defines a minimal local CLibHbm view that clashes with the full CLibHbm
// pulled in by CSelShopWin.hpp -> monolib/lib.hpp (same scheme as
// CMenuUpdate / CMenuShopBuy). isFlag01Set takes the task-game instance.
extern "C" void __ct__8CProcessFv(void*);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, IScnRender*, u32, u32);

// State-machine handlers (retail C-ABI helpers taking the menu object).
extern "C" void ShopSel_State0Show(CMenuSelectShop*);
extern "C" void ShopSel_State1Cursor(CMenuSelectShop*);
extern "C" void ShopSel_State2Input(CMenuSelectShop*);
extern "C" void ShopSel_State3Done(CMenuSelectShop*);

// Shop-window close animation hook + select-shop selection actions (retail
// unmangled C-ABI symbols; defined in CSelShopWin/CUICfManager TUs).

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 gmPauseFlags;             // cbRenderBefore gate flag (.sbss)
extern char lbl_eu_80503AE0[];          // memory-region name string (.rodata)
