#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/IUIWindow.hpp"

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

/*
 * System window "save" process (singleton, created by SysWinSave_Create).
 *
 * Real MI class: IUIWindow at +0x00 (0x6C - brings the CTTask<IUIWindow>
 * ptmf hooks and the IUIWindow state), IWorkEvent at +0x6C and IScnRender at
 * +0x70 (the long-lived render callback subobject), plus an embedded
 * UnkClass_8045F564 region, a CSysWin and a CCur18 cursor:
 *   0x00  IUIWindow base (destructor chain ends at CProcess)
 *   0x6C  IWorkEvent base
 *   0x70  IScnRender base
 *   0x74  CScn* owning scene
 *   0x78  UnkClass_8045F564 embedded region (0x10 bytes)
 *   0x88  CSysWin storage (0x3C bytes)
 *   0xC4  CCur18 cursor storage (0x18 bytes)
 *   0xDC  flag byte
 *   0xDD  signed flag byte (drives cf load/release helper in Term)
 *
 * The ctor has no out-of-line copy (it is inlined into the SysWinSave_Create
 * singleton factory); the virtual dtor is defined in CSysWinSave.cpp.
 */
class CSysWinSave : public IUIWindow, public IWorkEvent, public IScnRender {
public:
    // Single-use ctor: retail has no out-of-line __ct__11CSysWinSave symbol - the
    // whole body (CProcess ctor, CTTask<IUIWindow> vptr + null ptmf hooks,
    // IUIWindow state, composite + sub-vptrs, the embedded region/CSysWin/CCur18)
    // is inlined into the SysWinSave_Create factory.
    CSysWinSave(CScn* scene)
        : mScene(scene), mMemRegion(), mSysWin(0), mCur18(NULL), mFlagDC(0), mFlagDD(0) {}
    virtual ~CSysWinSave();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    void HandleInput();

    CScn* mScene;                  // 0x74 owning scene
    UnkClass_8045F564 mMemRegion;  // 0x78 embedded scratch region
    CSysWin mSysWin;               // 0x88 CSysWin storage (0x3C)
    CCur18 mCur18;                 // 0xC4
    u8  mFlagDC;                   // 0xDC
    s8  mFlagDD;                   // 0xDD signed flag (cf load/release toggle)
};

// name-guess: SysWinSave_Create (singleton factory; modelled on the retail
// sibling SysWinLog_Create(CProcess*, u8*)). Returns 0 if already created.
CSysWinSave* SysWinSave_Create(CProcess* registerParent, CScn* scene);

#include "monolib/scn/CScn.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUICfManager.hpp"

// CSysWin vtable view exposing the layout-build virtual at +0x88 (slot 34)
// invoked by Init. Leading pure virtuals occupy inherited slots so the target
// lands at virtual index 32 (-> vtable +0x88 after the RTTI prefix).
class CSysWinView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void v0C() = 0;
    virtual void v0D() = 0;
    virtual void v0E() = 0;
    virtual void v0F() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v1A() = 0;
    virtual void v1B() = 0;
    virtual void v1C() = 0;
    virtual void v1D() = 0;
    virtual void v1E() = 0;
    virtual void v1F() = 0;
    virtual void v20() = 0;  // index 32 -> +0x88 - Init
};

// Minimal view of cf::CfPadData for SysWinSaveHandleInput (offsets match retail).
struct CSysPadData {
    u8  _00[0x4];
    u32 field_04;      // +0x4 CPad::mPressedButtonFlags
    u8  _08[0x104 - 8];
    u32 field_104;     // +0x104 mTurboPressButtonFlags (short-press state)
};

// Data body of the embedded CSysWin (0x3C bytes). The Init body copy skips the
// vtable (+0) only, copying every other field so MWCC emits the exact
// lwz/stw + lbz/stb sequence retail shows.
struct CSysWinData {
    void* vtable;  // 0x00 (retained from construction, not copied)
    u32   f_04;    // 0x04
    u32   f_08;    // 0x08
    u32   f_0c;    // 0x0c
    u32   f_10;    // 0x10
    u32   f_14;    // 0x14
    u32   f_18;    // 0x18
    u32   f_1c;    // 0x1c
    u32   f_20;    // 0x20
    u32   f_24;    // 0x24 (copied as a word)
    u8    f_28;    // 0x28 (copied as a byte)
    u8    f_29;    // 0x29
    u8    f_2a;    // 0x2a
    u8    f_2b;    // 0x2b
    u32   f_2c;    // 0x2c
    u32   f_30;    // 0x30
    u8    f_34;    // 0x34
    u8    f_35;    // 0x35
    u8    f_36;    // 0x36
    u8    f_37;    // 0x37
    u8    f_38;    // 0x38
    u8    f_39;    // 0x39
};

// C-ABI import used by SysWinSaveHandleInput.
extern "C" {
void* getCfPadData__Q22cf13CfGameManagerFv();
}

// DrawInfo helper (retail mangled name SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo).

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 gmPauseFlags;            // cbRenderBefore flag (.sbss)



