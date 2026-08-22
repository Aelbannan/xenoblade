#pragma once

#include <types.h>

#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CLoad.hpp"
#include "kyoshin/makecrystal/CMCCrystalBox.hpp"
#include "kyoshin/makecrystal/CModelDispMakeCrystal.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/CWorkThread.hpp"
#include "monolib/core/CProc.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

class CScn;

// Minimal CScn view (same guarded declaration as CQuestWindow.hpp /
// CTaskGameEff.hpp). Needed here for removeRenderCB in func_8021299C.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// Byte flags written on the owning scene during window teardown
// (func_8021299C). The kyoshin minimal CScn (guarded decl in the task
// headers) has no fields, so access goes through this layout view.
struct CScnFlagView {
    u8 _00[0x39];
    u8 field_39;           // +0x39
    u8 _3A[0x3E4 - 0x3A];
    u8 field_3E4;          // +0x3E4
};

// Minimal CTaskGame view: the static gate helpers used by cbRenderBefore and
// the +0x6C sub-process slot read by func_8021299C. The real CTaskGame.hpp
// pulls broken monolib umbrella headers (same scheme as CSystemWindow.hpp).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
    u8 _00[0x6C];
    CProc* unk6C;          // +0x6C: task sub-process (pssDetachView target)
};

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
};

// Imports used by this unit's window functions. The retail US reloc names for
// these callees are unmangled free functions (per config/us/symbols.txt), so
// they are declared extern "C" - C++ linkage would re-mangle them.
extern "C" {
int getCrystalStateA(CModelDispMakeCrystal* self);
int getCrystalStateB(CModelDispMakeCrystal* self);
int getCrystalStateC(CModelDispMakeCrystal* self);
int func_80221CD0(CModelDispMakeCrystal* self);
void setCrystalCursor(CModelDispMakeCrystal* self, u32 val);
void func_80221B90(CModelDispMakeCrystal* self, u8 a, u8 b);
void func_8021C928(CModelDispMakeCrystal* self, nw4r::lyt::DrawInfo* drawInfo);
void func_8021C8B0(CModelDispMakeCrystal* self);
void func_8021C6E4(CModelDispMakeCrystal* self);
void func_80210E9C(u8* dst, const u8* src);
int func_8029A658();
int func_802146F8(CMCCrystalBox* self);
int func_80214634(CMCCrystalBox* self);
void func_80214700(CMCCrystalBox* self);
void func_802144F4(CMCCrystalBox* self);
void func_80214408(CMCCrystalBox* self, nw4r::lyt::DrawInfo* drawInfo);
int func_8021625C(CMCCrystalBox* self);
u8 func_8021624C(CMCCrystalBox* self);
u8 func_80216254(CMCCrystalBox* self);
void func_80211CEC(CMCCrystalBox* self, const CMCCrystalBox* src);
void func_80213FE4(CMCCrystalBox* self);
int func_802AE6AC(CLoad* self);
void func_802AE5F0(CLoad* self, nw4r::lyt::DrawInfo* drawInfo);
void func_802AE62C(CLoad* self);
void func_802AE6C4(CLoad* self);
int func_801C4114(CTitleAHelp* self);
void func_801C412C(CTitleAHelp* self);
void func_801C414C(CTitleAHelp* self);
void func_801C40A0(CTitleAHelp* self);
void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
void func_80138078__FUl(u32 op);
int func_8013BE50();
void func_800453EC(CScn* scene);
void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
void __ct__CModelDispMakeCrystal(CModelDispMakeCrystal* self, CScn* scene);
void __ct__CMCCrystalBox(CMCCrystalBox* self, u8 parentType);
void __ct__CLoad(CLoad* self, u8 arg);
void __dt__21CModelDispMakeCrystalFv(CModelDispMakeCrystal* self, int flags);
void __dt__5CLoadFv(CLoad* self, int flags);
void __dt__13CMCCrystalBoxFv(CMCCrystalBox* self, int flags);
void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
void __ct__Q34nw4r3lyt8DrawInfoFv(void* drawInfo);
void __dt__Q34nw4r3lyt8DrawInfoFv(void* drawInfo, int flags);
}

// C-linkage imports. The retail DOL symbols are unmangled (per the retail
// relocs), so C++-linkage declarations would re-mangle the reloc names.
extern "C" int func_802146C0(CMCCrystalBox* self);
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" int func_8021CA3C(CModelDispMakeCrystal* self);
extern "C" void func_8021CB20(CModelDispMakeCrystal* self);
extern "C" void func_802AE758(CLoad* self);

// DrawInfo setup helper (retail mangled name func_80137250__FPQ34nw4r3lyt8DrawInfo).
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Global data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_80535B70[];   // CMakeCrystalWin vtable data (IScnRender sub-vtable at +0x88)
extern u32 lbl_eu_80663E28;      // render-gate mode bitfield (.sbss; bit 21 = busy)
