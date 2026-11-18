#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

/*
 * System window "save" process (singleton, created by __ct__802944D8).
 *
 * A CProcess subclass whose layout mirrors CSystemWindow / CSysWinSelect's
 * Move/UI region, plus an embedded UnkClass_8045F564 region, a CSysWin and a
 * CCur18 cursor:
 *   0x00  CProcess primary base (destroyed via ~CProcess)
 *   0x3C  IUIWindow/Move region (48 bytes)
 *   0x6C  IWorkEvent vtable slot
 *   0x70  IScnRender vtable slot (long-lived render callback subobject)
 *   0x74  CScn* owning scene
 *   0x78  UnkClass_8045F564 embedded region (0x10 bytes)
 *   0x88  CSysWin storage (0x3C bytes)
 *   0xC4  CCur18 cursor storage (0x18 bytes)
 *   0xDC  flag byte
 *   0xDD  signed flag byte (drives cf load/release helper in Term)
 *
 * CSysWinSave's ctor/dtor are emitted as global functions carrying the retail
 * symbol names (__ct__802944D8 / __dt__11CSysWinSaveFv), so the member dtor
 * declared here is satisfied at link time by that global definition (CMapSel
 * / CSysWinSelect idiom).
 */
class CSysWinSave : public CProcess {
public:
    virtual ~CSysWinSave();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x3C IUIWindow/Move region (48 bytes, same as CSystemWindow/CSysWinSelect).
    u32 ptmf0[3];                 // 0x3C null pointer-to-member-function
    u32 ptmf1[3];                 // 0x48 null pointer-to-member-function
    u32 field_54;                 // 0x54
    u32 field_58;                 // 0x58
    u32 field_5C;                 // 0x5C
    s32 field_60;                 // 0x60 (-1)
    u8  field_64;                 // 0x64
    u8  field_65;                 // 0x65
    u8  field_66;                 // 0x66
    u8  field_67;                 // 0x67 (1)
    u32 field_68;                 // 0x68
    u32 mWorkEvent;               // 0x6C IWorkEvent vtable slot
    u32 mScnRender;               // 0x70 IScnRender vtable slot
    CScn* mScene;                 // 0x74 owning scene
    UnkClass_8045F564 mMemRegion; // 0x78 embedded scratch region
    u8  mSysWin[0x3C];            // 0x88 CSysWin storage
    CCur18 mCur18;  // 0xC4
    u8  mFlagDC;                  // 0xDC
    s8  mFlagDD;                  // 0xDD signed flag (cf load/release toggle)
    void HandleInput();
};

#include "monolib/scn/CScn.hpp"

#include "kyoshin/CTaskGame.hpp"

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

// Byte-range shim over the CProcess vtable slot (+0x10) so __ct__802944D8 can
// write it without pointer arithmetic (same shape as CCol6Invite's ctor shim).
// The rest of the 0x00-0x78 region is written through the named CSysWinSave
// members (ptmf0/ptmf1/field_*).
struct CSysWinSaveVtShim {
    u8 _00[0x10];
    void* vtable;  // 0x10 - CProcess vtable, overwritten twice by the ctor
};

// C-ABI function imports / definitions: these retail symbols are unmangled so
// they must be declared `extern "C"` to stop MWCC suffixing the reloc name.
// (SetWideLocAdjust is the exception: retail emits the mangled nw4r form, so it
// is declared below as a normal C++ function to let MWCC mangle it.)
extern "C" {
void deleteRegion__17UnkClass_8045F564Fv(UnkClass_8045F564* region);
void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);
void __ct__8CProcessFv(CProcess* self);
void __ct__7CSysWinFUc(void* syswin, int arg);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void __dt__7CSysWinFv(void* syswin, int flags);
void __dt__17UnkClass_8045F564Fv(void* self, int flags);
void __dt__8CProcessFv(CProcess* self, int flags);
void* __dt__11CSysWinSaveFv(CSysWinSave* self, int flags);

// Additional C-ABI imports used by Init/Move/SysWinSaveHandleInput (retail symbols
// are unmangled / non-namespace, so C linkage is required).
void* getCfPadData__Q22cf13CfGameManagerFv();
int isClassicController__Q22cf13CfGameManagerFv(int arg);
void CUICfManager_queueSaveMenu(int a, int b);
}

// DrawInfo helper (retail mangled name SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo).

// Global data imports (MWCC does not mangle global-scope data names).
extern CSysWinSave* lbl_eu_80664A08;   // singleton instance pointer (.sbss)
extern u32 lbl_eu_80663E28;            // cbRenderBefore flag (.sbss)

extern u32 __ptmf_null[3];             // null pointer-to-member-function constant
// IUIWindow base vtable (written at +0x10 first, then overwritten by the
// CSysWinSave composite vtable lbl_eu_805389A0).
extern char lbl_eu_8052D238[];
// (389A0 struct-def lives in CSysWinSave.cpp; no hpp decl to avoid type coupling.)

// Shared string table: createRegion tag in Init, message table base +0xc/+0x17
// in Move (read-only .rodata). MWCC does not mangle global-scope data names.
extern char lbl_eu_8050FD6C[];

// Float literals used by setChainGauge (.sdata2). Mutable (const would give
// internal linkage and clash with the definitions in CSysWinSave.cpp).
// (BA0+BA4 share one nonzero-anchored pair; single-float zeros exile to .sbss2.)
extern f32 lbl_eu_80668BA04[2];
extern f64 lbl_eu_80668BA8;      // multiplier