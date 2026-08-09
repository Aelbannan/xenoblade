#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

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
    u8  mCur18[0x18];             // 0xC4 CCur18 cursor storage
    u8  mFlagDC;                  // 0xDC
    s8  mFlagDD;                  // 0xDD signed flag (cf load/release toggle)
};

// Render-callback and scene helper used by Term/cbRenderBefore (retail CScn
// member; defined in monolib/src/scn/CScn).
class CScn {
public:
    void removeRenderCB(IScnRender* cb);
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
};

// Minimal CTaskGame decl (retail global-namespace class, symbols
// getInstance__9CTaskGameFv / func_800426F0__9CTaskGameFv).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// Abstract view into the embedded CCur18 vtable (slot 3 at +0x0C is the
// cursor update virtual invoked by Term). Matches CSysWinSelect::CCursor18.
class CCursor18 {
public:
    virtual void vf2(int) = 0;  // slot 2 (0x08)
    virtual void vf3() = 0;     // slot 3 (0x0C)
};

// C-ABI function imports / definitions: these retail symbols are unmangled so
// they must be declared `extern "C"` to stop MWCC suffixing the reloc name.
// (func_80137250 is the exception: retail emits the mangled nw4r form, so it
// is declared below as a normal C++ function to let MWCC mangle it.)
extern "C" {
void func_8045F778__17UnkClass_8045F564Fv(UnkClass_8045F564* region);
void func_8022B7F4(void* syswin);
void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);
void func_801D20B0(void* cursor, nw4r::lyt::DrawInfo* drawInfo);
int func_8013BE50();
void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
void __ct__8CProcessFv(CProcess* self);
void __ct__CSysWin(void* syswin, int arg);
void __ct__CCur18(void* cursor, void* accessor);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void __dt__7CSysWinFv(void* syswin, int flags);
void __dt__6CCur18Fv(void* cursor, int flags);
void __dt__17UnkClass_8045F564Fv(void* self, int flags);
void __dt__8CProcessFv(CProcess* self, int flags);
void* __dt__11CSysWinSaveFv(CSysWinSave* self, int flags);
}

// DrawInfo helper (retail mangled name func_80137250__FPQ34nw4r3lyt8DrawInfo).
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Global data imports (MWCC does not mangle global-scope data names).
extern CSysWinSave* lbl_eu_80664A08;   // singleton instance pointer (.sbss)
extern u32 lbl_eu_80663E28;            // cbRenderBefore flag (.sbss)

extern u32 __ptmf_null[3];             // null pointer-to-member-function constant
// IUIWindow base vtable (written at +0x10 first, then overwritten by the
// CSysWinSave composite vtable lbl_eu_805389A0).
extern char lbl_eu_8052D238[];
extern char lbl_eu_805389A0[];

// Float literals used by func_80294844 (shared .rodata in the split).
extern const f32 lbl_eu_80668BA0;      // value written to storage pre-guard
extern const f32 lbl_eu_80668BA4;      // compare threshold / subtract term
extern const f32 lbl_eu_80668BA8;      // multiplier