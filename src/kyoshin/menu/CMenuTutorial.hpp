#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTutorial.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Local IScnRender declaration with NO destructor and no inline bodies. The
// real monolib IScnRender (libs/monolib/include/monolib/scn/IScnRender.hpp)
// defines `virtual ~IScnRender(){}` inline; odr-using that inline dtor from
// this TU makes MWCC emit a standalone __dt__10IScnRenderFv strong copy
// (0x40) here and blows the split budget (retail keeps that copy only in
// CTaskGame.o, and its own dtor treats the subobject as trivial -- the
// generated ~CMenuTutorial destroys only mTitleAHelp/mTutorial then chains
// to the base destructor). Omitting the dtor gives IScnRender an implicitly
// trivial one while keeping the vtable layout identical (slot order:
// dtor slot, handleEmptyRender).
class IScnRender {
public:
    virtual void handleEmptyRender();
};

/*
 * Tutorial menu screen process (CProcess + IScnRender MI).
 *
 * Layout (constructor / destructor / extab):
 *   0x00: CProcess          -- task-system base
 *   0x3C: vtable PMF data   -- compiler-generated virtual dispatch entries
 *   0x54: u8 field_54       -- writable state byte
 *   0x58: IScnRender        -- render-callback subobject (member, vptr)
 *   0x5C: CProcess*         -- parent process reference
 *   0x60: CTutorial         -- tutorial widget member (0x54 bytes -> 0xB4)
 *   0xB4: CTitleAHelp       -- title/help bar member (0x38 bytes -> 0xEC)
 *   0xEC: u8[3]             -- mIsInitialised, mType, mSomething
 *   0xF0: u32               -- saved input flags
 */
class CMenuTutorial : public CProcess {
public:
    virtual ~CMenuTutorial();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void Draw();
    void cbRenderBefore();

    u32 ptmf0[3];             // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];             // 0x48-0x53: null PMF callback slot group 2
    u8 field_54;              // 0x54: writable state byte
    u8 field_55;              // 0x55
    u8 _pad56[2];             // 0x56-0x57
    // 0x58: render-callback subobject (vptr). Typed member against the
    // body-less local IScnRender decl above (see note at top of this header).
    IScnRender mIScnRender;   // 0x58
    CProcess* mParentRef;     // 0x5C: owning process (cast to CScn* at call sites)
    CTutorial mTutorial;      // 0x60-0xB3: tutorial widget
    CTitleAHelp mTitleAHelp;  // 0xB4-0xEB: title/help bar
    u8 mIsInitialised;        // 0xEC: initialisation flag
    u8 mType;                 // 0xED: tutorial type
    u8 mSomething;            // 0xEE
    u8 _padEF;                // 0xEF
    u32 mSavedInputFlags;     // 0xF0: input flags snapshot at construction
    CMenuTutorial();
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
// Widget ctor/dtor / load helper and DrawInfo ct/dt are retail-unmangled (or
// pre-mangled C names); declared extern "C" so calls bind to the literal
// symbol instead of an Itanium re-derivation.
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CTutorial(CTutorial* self, u8 type, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __dt__9CTutorialFv(CTutorial* self, int dealloc);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int dealloc);
extern "C" void __ct__UnkClass_8011C974(void* dst, void* src);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);

// CTitleAHelp widget callees (retail-unmangled func_ names).
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C41C0(CTitleAHelp* self, char* str);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void func_801C4760(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C414C(CTitleAHelp* self);
// isIdle declared int (not u8) so callers compare with cmpwi directly.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);

// CTutorial widget callees (retail-unmangled func_ names).
extern "C" void func_8029ABB8(CTutorial* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_8029AB28(CTutorial* self);
extern "C" void func_8029ABD8(CTutorial* self);
extern "C" void func_8029AA34(CTutorial* self);
extern "C" void func_8029ACC4(CTutorial* self);
extern "C" u8 func_8029AE5C(CTutorial* self);
extern "C" int func_8029ACAC(CTutorial* self);
extern "C" int func_8029ACB4(CTutorial* self);
extern "C" int func_8029ACBC(CTutorial* self);
extern "C" void func_8029AD88(CTutorial* self);
extern "C" void func_8029ACEC(CTutorial* self);
extern "C" void func_8029ADF8(CTutorial* self);

// CF / ui helpers (retail-unmangled names).
extern "C" int func_8013BE50();
extern "C" u8 func_8013B980();
extern "C" void func_80135550();
extern "C" u8 code80135FDC_getByte_64080();
extern "C" void code80135FDC_postIncByte_64080();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u32 enable);
// CfGameManager controller-type query: retail loads r3=-1 into the symbol
// (the no-arg member wrapper drops it), so call through the C name directly.
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);

// DrawInfo layout setup (retail MANGLED name
// func_80137250__FPQ34nw4r3lyt8DrawInfo; declared as plain C++ so MWCC
// re-derives the same mangling).
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// This-unit phase handlers (retail-unmangled func_ names). extern "C" keeps
// the call relocs bare (retail keeps the unmangled names at these call sites).
extern "C" void func_8029A668(CMenuTutorial* self);
extern "C" void func_8029A764(CMenuTutorial* self);
extern "C" void func_8029A7B4(CMenuTutorial* self);
extern "C" void func_8029A8D4(CMenuTutorial* self);

// IScnRender vtable this-adjusting thunks (retail: subi r3, r3, 0x58; b ...).
void func_8029A924(IScnRender* sub);
void func_8029A92C(IScnRender* sub);

// Global (unmangled retail) accessors for the CMenuTutorial object.
CMenuTutorial* func_8029A5DC(CProcess* self, CProcess* parent, u32 arg2);

// CTaskGame statics (retail-mangled names). Imported flat because including
// CTaskGame.hpp would pull monolib/scn.hpp and clash with the body-less
// local IScnRender declaration above.
extern "C" void getInstance__9CTaskGameFv();
extern "C" int isFlag01Set__9CTaskGameFv();

// vtable / PMF / shared data (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];   // CProcess primary vtable (pre-PMF copy)
extern char lbl_eu_805391C0[];   // CMenuTutorial composite vtable
extern u32 lbl_eu_80663E28;      // render-gate mode bitfield (bit 21 = busy)
extern u32 lbl_eu_80664A28;      // singleton instance pointer (.sbss)
extern char lbl_eu_80510260[];   // message-name string pool (Init title lookup)
