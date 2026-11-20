#pragma once

#include <types.h>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CLoad.hpp"
#include "kyoshin/makecrystal/CMCCrystalBox.hpp"
#include "kyoshin/makecrystal/CModelDispMakeCrystal.hpp"

// is raw bytes), so a forward declaration keeps CModelDispMakeCrystal.hpp's
// member-type include graph (CMCGetItemBox.hpp et al.) out of this TU.
class CModelDispMakeCrystal;
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/CWorkThread.hpp"
#include "monolib/core/CProc.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

#include "monolib/scn/CScn.hpp"

#include "kyoshin/CTaskGame.hpp"

// Main crystal-crafting window. Fixed sub-object layout: a title-help banner
// (CTitleAHelp, 0x38 bytes) at +0x18, the crystal model-display object at
// +0x50, the crystal box list (CMCCrystalBox, 0x1508 bytes) at +0x2E28, the
// loading overlay (CLoad, 0x30 bytes) at +0x4330, then two state bytes.
//
// Deliberately NON-polymorphic (plain struct, CSystemWindow idiom): retail
// stores two raw vtable addresses (primary at +0x00 = lbl_eu_80535B70, the
// IScnRender render-callback sub-vtable at +0x04 = lbl_eu_80535B70 + 0x88)
// and drives all subobject construction by hand, so the ctor is emitted as a
// C-ABI global `__ct__CMakeCrystalWin` instead of a member ctor.
class CMakeCrystalWin {
public:
    /* 0x00 */ u32 mVtbl;               // primary vtable (lbl_eu_80535B70)
    /* 0x04 */ u32 mScnRenderVtbl;      // IScnRender sub-vtable (lbl_eu_80535B70 + 0x88)
    /* 0x08 */ u32 field_08;            // ctor arg 2
    /* 0x0C */ u32 field_0C;            // ctor arg 3
    /* 0x10 */ CScn* mScene;            // owning scene (0 until assigned)
    /* 0x14 */ CWorkThread* mThread;    // child work thread (0 until assigned)
    /* 0x18 */ CTitleAHelp mTitleAHelp;             // sizeof 0x38
    /* 0x50 */ u8 mModelDispArea[0x2E28 - 0x50];    // CModelDispMakeCrystal
    /* 0x2E28 */ CMCCrystalBox mCrystalBox;         // sizeof 0x1508
    /* 0x4330 */ CLoad mLoad;                       // sizeof 0x30
    /* 0x4360 */ u8 field_4360;
    /* 0x4361 */ u8 field_4361;

    void cbRenderBefore();
    int mkCrystalIsBusyState();
};

// Imports used by this unit's window functions. The retail US reloc names for
// these callees are unmangled free functions (per config/us/symbols.txt), so
// they are declared extern "C" - C++ linkage would re-mangle them.
extern "C" {
void MakeCrystal_UnlinkFileSlot(CModelDispMakeCrystal* self, u8 a, u8 b);


void unregisterEffectScene(CScn* scene);
void __ct__11CTitleAHelpFPcUc(CTitleAHelp* self, char* name, u8 arg);
void __ct__21CModelDispMakeCrystalFP4CScn(CModelDispMakeCrystal* self, CScn* scene);
// CMakeCrystalWin is still a hand-built object: its ctor/dtor construct the
// embedded CMCCrystalBox through the real member symbols.
CMCCrystalBox* __ct__13CMCCrystalBoxFUc(CMCCrystalBox* self, u8 parentType);
extern "C" void __ct__CLoad(CLoad* self, u8 arg);
void __dt__21CModelDispMakeCrystalFv(CModelDispMakeCrystal* self, int flags);
void __dt__5CLoadFv(CLoad* self, int flags);
void __dt__13CMCCrystalBoxFv(CMCCrystalBox* self, int flags);
void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
}

// C-linkage imports. The retail DOL symbols are unmangled (per the retail
// relocs), so C++-linkage declarations would re-mangle the reloc names.
extern "C" void MakeCrystal_StartCharge(CModelDispMakeCrystal* self);

// State-copy helpers defined in CMenuMakeCrystal.cpp. Retail uses the plain
// C names at both call sites (config/us/symbols.txt).
extern "C" void MakeCrystalCopyDispArea(u8* dst, const u8* src);
extern "C" void MakeCrystalCopyBox(CMCCrystalBox* self, const CMCCrystalBox* src);

// DrawInfo setup helper (retail mangled name SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo).

// Global data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_80535B70[];   // CMakeCrystalWin vtable data (IScnRender sub-vtable at +0x88)
extern u32 gmPauseFlags;      // render-gate mode bitfield (.sbss; bit 21 = busy)

// Free functions defined in this TU (real C++ linkage, retail-mangled names).
u32 mkCrystalGetFlag4361(void* self);
u32 mkCrystalGetWord10(void* self);
