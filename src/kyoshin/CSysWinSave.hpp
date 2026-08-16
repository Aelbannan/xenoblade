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

// Full CCur18 vtable view invoked by Init/Move so MWCC emits a genuine
// `lwz r12, slot(r12)` virtual dispatch (virtual index 2 -> vtable +0x10).
// MWCC inserts an offset-to-top + RTTI prefix (2 entries), so vtable offset =
// (virtual index + 2) * 4. Matches CSysWinSelect::CCur18View.
class CCur18View {
public:
    virtual void vf02() = 0;       // index 0 -> +0x08
    virtual void vf03(void*) = 0;  // index 1 -> +0x0C (unused here)
    virtual void vf04(void*) = 0;  // index 2 -> +0x10 - Move, func_80294638
};

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

// Minimal view of cf::CfPadData for func_80294638 (offsets match retail).
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

// Data body of the embedded CCur18 cursor (0x18 bytes). Init copies the
// non-vtable fields (0x14/0x15 are bytes; 0x16/0x17 are not copied).
struct CCur18Data {
    void* vtable;  // 0x00 (retained, not copied)
    u32   f_04;    // 0x04
    u32   f_08;    // 0x08
    u32   f_0c;    // 0x0c
    u32   f_10;    // 0x10
    u8    f_14;    // 0x14
    u8    f_15;    // 0x15
    u8    f_16;    // 0x16
    u8    f_17;    // 0x17
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
// (func_80137250 is the exception: retail emits the mangled nw4r form, so it
// is declared below as a normal C++ function to let MWCC mangle it.)
extern "C" {
void func_8045F778__17UnkClass_8045F564Fv(UnkClass_8045F564* region);
void func_8022B7F4(void* syswin);
void func_8022B7C8(void* syswin, nw4r::lyt::DrawInfo* drawInfo);
void func_801D20B0(void*, void*);
int func_8013BE50();
void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self);
void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);
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

// Additional C-ABI imports used by Init/Move/func_80294638 (retail symbols
// are unmangled / non-namespace, so C linkage is required).
void* func_801355F4();
void* getCfPadData__Q22cf13CfGameManagerFv();
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
void func_80138078__FUl(u32 op);
u32 CSysWin_isReady(void* syswin);
int CSysWin_isActive(void* syswin);
void func_8022B9B4(void* syswin, void* str1, void* str2);
void func_8022BF6C(void* syswin, void* a, void* b);
void func_8022BFC8(void* syswin, int kind);
void func_8022B8B8(void* syswin);
void func_8022B8E4(void* syswin);
void func_8022B748(void* syswin);
void func_8022C1B4(void* out, void* syswin, u8 sel);
void func_801D216C(void* cur, int val);
void func_801D202C(void* cur);
char* func_80136190(const void* a, const void* b, int id);
void func_80134B50(int a, int b);
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

// Shared string table: createRegion tag in Init, message table base +0xc/+0x17
// in Move (read-only .rodata). MWCC does not mangle global-scope data names.
extern char lbl_eu_8050FD6C[];

// Float literals used by func_80294844 (shared .rodata in the split).
extern const f32 lbl_eu_80668BA0;      // value written to storage pre-guard
extern const f32 lbl_eu_80668BA4;      // compare threshold / subtract term
extern const f32 lbl_eu_80668BA8;      // multiplier