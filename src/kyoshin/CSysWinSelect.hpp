#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/IUIWindow.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Minimal view of cf::CfPadData for SysWinSelectHandleInput (offsets match retail).
struct CSysPadData {
    u8  _00[0x4];
    u32 field_04;       // +0x4 CPad::mPressedButtonFlags
    u8  _08[0x104 - 8];
    u32 field_104;      // +0x104 mTurboPressButtonFlags
};

/*
 * System window "select" process (singleton, created by SysWinSelectCreate).
 *
 * Real MI class: IUIWindow at +0x00 (0x6C), IWorkEvent at +0x6C and
 * IScnRender at +0x70 (long-lived render callback subobject), with an embedded
 * CSysWin and a CCur18 cursor:
 *   0x00  IUIWindow base (destructor chain ends at CProcess)
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base
 *   0x74  CScn* owning scene
 *   0x78  CSysWin storage (0x3C bytes)
 *   0xB4  CCur18 cursor storage (0x18 bytes)
 *
 * CSysWinSelect's ctor/dtor are emitted as global functions carrying the
 * retail symbol names (__ct__CSysWinSelect / __dt__13CSysWinSelectFv), so the
 * member dtor declared here is satisfied at link time by that global
 * definition (CMapSel idiom).
 */
class CSysWinSelect : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    CSysWinSelect(void* a2, void* a3, void* a4, void* a5);
    virtual ~CSysWinSelect();
    // Bodies not recovered yet; the vtable slots still have to carry the
    // class's own symbols, so keep them as empty definitions.
    virtual void Init() {}
    virtual void Term() {}
    virtual void Move() {}
    virtual void cbRenderBefore(CScn*) {}

    CScn* mScene;      // 0x74 owning scene
    CSysWin mSysWin;   // 0x78 CSysWin storage (0x3C)
    CCur18 mCur18;     // 0xB4 cursor
    u8  mState;        // 0xCC selection state (0..3)
    u8  mCursorSel;    // 0xCD cursor selection index (1 = default)
    char mStrA[0x80];  // 0xCE first string (a3)
    char mStrB[0x80];  // 0x14E second string (a4)
    char mStrC[0x80];  // 0x1CE third string (a5)
    u8  field_24E;     // 0x24E
};

#include "monolib/scn/CScn.hpp"

#include "kyoshin/CTaskGame.hpp"

// CSysWinView (vtable view used by Init) comes from CSysWinBuff.hpp in the
// shared CSystemWindow.cpp TU.

// Ctor/dtor and C-ABI helper imports. These retail symbols are C-linkage
// (unmangled), so they are declared `extern "C"`: C++ linkage would make MWCC
// append the __F<argtypes> suffix and publish a different reloc name.
extern "C" {
void SysWinSelectHandleInput(CSysWinSelect* self);  // C-linkage: retail symbol is unmangled
void* getCfPadData__Q22cf13CfGameManagerFv();
void __ct__7CSysWinFUc(void* syswin, int arg);
void __ct__8CProcessFv(CProcess* self);
void __dt__7CSysWinFv(void* syswin, int flags);
void __dt__8CProcessFv(CProcess* self, int flags);
}

// C++-linkage helper - retail emits the mangled form
// SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo (same convention as CSystemWindow.hpp).

// Global data imports (MWCC does not mangle global-scope data names).
extern CSysWinSelect* lbl_eu_80663FDC;  // singleton instance pointer (.sbss)
extern u32 gmPauseFlags;             // cbRenderBefore flag (.sbss)

