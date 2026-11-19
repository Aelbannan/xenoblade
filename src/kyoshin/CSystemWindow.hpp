#pragma once

#include "kyoshin/CSystemWindowApi.hpp"
#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/IUIWindow.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

/*
 * System window process (singleton, created by SysWinCreateSingleton).
 *
 * Real MI class, same base arrangement as its TU-mate CSysWinSelect:
 *   0x00  IUIWindow base (destructor chain ends at CProcess)
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base (render callback subobject)
 *   0x74  CScn* owning scene
 *   0x78  CSysWin (0x3C)
 *   0xB4  state machine byte, 0xB5 CSysWin kind used by Init
 *   0xB6/0x1B6 two title strings, 0x2B6 presentation flag
 *
 * Retail CSystemWindow.cpp also defines CSysWinSelect (one TU: the
 * @108@/@112@ CSystemWindow adjustor thunks sit after CSysWinSelect's code,
 * and __vt__13CSystemWindow follows __vt__13CSysWinSelect in .data).
 */
class CSystemWindow : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    CSystemWindow(CScn* scene, u32 opt, const char* str1, const char* str2);
    virtual ~CSystemWindow();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    CScn* mScene;            // 0x74 owning scene
    CSysWin mSysWin;         // 0x78
    u8    mState;            // 0xB4 (Move state machine)
    u8    field_B5;          // 0xB5 CSysWin kind
    char  mStr1[0x100];      // 0xB6
    char  mStr2[0x100];      // 0x1B6
    u8    mFlag2B6;          // 0x2B6
};

#include "monolib/scn/CScn.hpp"

#include "kyoshin/CTaskGame.hpp"

// C-ABI function imports / definitions: these retail symbols are unmangled
// (C-linkage), so they must be declared `extern "C"` to stop MWCC appending a
// __F<argtypes> suffix to the emitted reloc names.
// SetWideLocAdjust is the exception: retail emits the mangled nw4r form
// (SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo), so it is declared as a normal C++
// global function to let MWCC mangle it - NOT inside the extern "C" block.
extern "C" {
void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);
void __ct__7CSysWinFUc(void* syswin, int arg);
void __dt__7CSysWinFv(void* syswin, int flags);

// CSysWin / system-window helper imports (C-linkage retail symbols).
// Verbatim-mangled retail import: a C++ member declaration on cf::CfGameManager
// would change call-site codegen (see MWCC_CASES); keep C linkage.
}

// C++-linkage helpers - MWCC mangles these to the retail symbols
// SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo / playUISound__FUl.

// Singleton factory (args forwarded to the CSystemWindow ctor).
extern "C" CSystemWindow* SysWinCreateSingleton(CProcess* parent, CScn* scene, u32 opt,
                             const char* str1, const char* str2);

// Global data imports (MWCC does not mangle global-scope data names).
extern CSystemWindow* lbl_eu_80663FD8;   // singleton instance pointer (.sbss)
extern u32 lbl_eu_80663E28;              // cbRenderBefore flag (.sbss)
