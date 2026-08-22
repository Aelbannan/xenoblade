#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CBaseCur.hpp"

#include <nw4r/lyt.h>
#include <revolution/gx/GXTypes.h>

// Forward decls (nw4r math VEC3 is only used as a pointer in the cursor view).
namespace nw4r { namespace math { struct VEC3; } }

/*
 * Title-screen UI widget. Layout (0x88 bytes):
 *   +0x00: IWorkEvent vtable pointer (lbl_eu_8053B2C8)
 *   +0x04: UnkClass_8045F564 scratch mem region (0x10)
 *   +0x14: CFileHandle* - async load handle
 *   +0x18: ArcResourceAccessor* - built from the loaded arc
 *   +0x1C: u8 - layout-ready flag
 *   +0x20: u32 (opaque; used as a pointer w/ vtable by func_802B775C)
 *   +0x24: u8 - phase/state
 *   +0x25: s8 - menu selection index (-1 = none)
 *   +0x2C: CTitleLogo (0x1C)
 *   +0x48: CTitleMenu (0x28)
 *   +0x70: CCur18 cursor (0x18, opaque)
 */
class CTitleLogo {
public:
    CTitleLogo();
    virtual ~CTitleLogo();

    /* 0x04 */ nw4r::lyt::ArcResourceAccessor* mAccessor;
    /* 0x08 */ nw4r::lyt::Layout* mLayout;
    /* 0x0C */ nw4r::lyt::AnimTransform* mAnimTrans0;
    /* 0x10 */ nw4r::lyt::AnimTransform* mAnimTrans1;
    /* 0x14 */ nw4r::lyt::AnimTransform* mAnimTrans2;
    /* 0x18 */ u8 field_0x18; // active flag
    /* 0x19 */ u8 field_0x19; // phase flag (init 1)
    /* 0x1A */ u8 field_0x1A; // state machine index (0..3)
};

class CTitleMenu {
public:
    CTitleMenu();
    virtual ~CTitleMenu();

    /* 0x04 */ nw4r::lyt::ArcResourceAccessor* mAccessor;
    /* 0x08 */ nw4r::lyt::Layout* mLayout;
    /* 0x0C */ nw4r::lyt::AnimTransform* mAnimTrans0;
    /* 0x10 */ nw4r::lyt::AnimTransform* mAnimTrans1;
    /* 0x14 */ nw4r::lyt::AnimTransform* mAnimTrans2;
    /* 0x18 */ nw4r::lyt::AnimTransform* mAnimTrans3;
    /* 0x1C */ nw4r::lyt::AnimTransform* mAnimTrans4;
    /* 0x20 */ nw4r::lyt::AnimTransform* mAnimTrans5;
    /* 0x24 */ u8 field_0x24; // active flag
    /* 0x25 */ u8 field_0x25; // phase flag (init 1)
    /* 0x26 */ u8 field_0x26; // state machine index (0..6)
};

// Opaque controller behind CTitle::field_0x20: only the virtual at vtable
// +0x0C (declared index 1) is invoked from this TU - it receives the menu
// selection index. All-pure so MWCC emits no vtable for the view itself.
class CTitleAction {
public:
    virtual void vf00() = 0;                 // index 0 -> +0x08
    virtual void vf01(int selection) = 0;    // index 1 -> +0x0C
};

class CTitle : public IWorkEvent {
public:
    CTitle();
    virtual ~CTitle();
    virtual bool OnFileEvent(CEventFile* pEventFile) override;

    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ CFileHandle* mFileHandle;
    /* 0x18 */ nw4r::lyt::ArcResourceAccessor* mAccessor;
    /* 0x1C */ u8 field_0x1C; // 1 once the layout is built
    /* 0x1D */ u8 field_0x1D[0x03]; // padding
    /* 0x20 */ CTitleAction* field_0x20;
    /* 0x24 */ u8 field_0x24; // phase/state
    /* 0x25 */ s8 field_0x25; // menu selection index (-1 = none)
    /* 0x26 */ u8 field_0x26[0x06]; // padding
    /* 0x2C */ CTitleLogo mLogo;
    /* 0x48 */ CTitleMenu mMenu;
    /* 0x70 */ u8 mCur[0x18]; // CCur18 cursor (opaque)
};

// Abstract view into the embedded CCur18 cursor vtable. MWCC inserts an
// offset-to-top + RTTI prefix (2 entries), so vtable offset = (index + 2) * 4;
// index 2 -> +0x10 is the "Move" virtual that repositions the cursor (takes
// a VEC3*). All-pure so MWCC emits no vtable for the view itself.
class CCur18View {
public:
    virtual void vf02() = 0;                        // index 0 -> +0x08
    virtual void vf03(const u8*) = 0;               // index 1 -> +0x0C
    virtual void vf04(const nw4r::math::VEC3*) = 0; // index 2 -> +0x10 - Move
};

// State-dispatch tables (.data): 12-byte pointer-to-member-function records
// indexed by the state fields. MWCC lowers (self->*table[idx])() to
// mulli/lis/addi + `bl __ptmf_scall`.
typedef void (CTitleLogo::*CTitleLogoStateFn)();
typedef void (CTitleMenu::*CTitleMenuStateFn)();
// CTitle-level state table, indexed by field_0x24.
typedef void (CTitle::*CTitleStateFn)();
extern CTitleStateFn lbl_eu_8053B274[];
extern CTitleLogoStateFn lbl_eu_8053B1F0[];
extern CTitleMenuStateFn lbl_eu_8053B220[];

// C-linkage imports / retail short-name decls (retail stripped manglings in
// this TU; keep linkage/signatures verbatim). noinline: retail calls each sub
// ctor via bl, so the call must survive.
extern "C" __declspec(noinline) CTitleLogo* __ct__CTitleLogo(CTitleLogo* self);
extern "C" __declspec(noinline) CTitleMenu* __ct__CTitleMenu(CTitleMenu* self);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
extern "C" CBaseCur* __ct__CCur18(CBaseCur* self, nw4r::lyt::ArcResourceAccessor* arcResAcc);
extern "C" void __dt__6CCur18Fv(void* self, int flags);
extern "C" void func_801D216C(void* pCur, u8 val);
extern "C" void func_801D202C(CBaseCur* pCur);
extern "C" void func_802B7948(void* a, unsigned int b, unsigned char v);
extern "C" int func_8023FEDC(void (*pCallback)(void*, unsigned int, unsigned char));
// Menu-cursor SFX cue (mangles to func_80138078__FUl).
void func_80138078(u32);
// Cursor-position builder (retail calls it via bl, so keep it out of line).
extern "C" void __declspec(noinline) func_802B6CBC(nw4r::math::VEC3* dest, CTitleMenu* menu, u8 val);

// Shared string pool (split1 .rodata) - "/menu/Title.arc" etc.
extern char lbl_eu_80513628[];
// Secondary mode bitfield (.sbss) - bit 30 gates the menu color pass.
extern u32 lbl_eu_80663E28;
// RGB color builder (defined in CTitleAHelp.cpp; returns the dest pointer so
// callers can thread it into func_80139A18). C++ linkage matches the retail
// TU's declaration, so calls bind to the same mangled symbol.
GXColorS10* func_801C4B60(GXColorS10* color, s16 r, s16 g, s16 b, s16 a);
// .sdata2 constants used by the logo/menu animation helpers (US/EU names).
extern const float lbl_eu_80668FE0;
extern double lbl_eu_80668FE8; // 0x4330000000000000 (2^52) - u32->f32 conversion magic
extern float lbl_eu_80668FF0;
// vtables (.data): CTitle / CTitleLogo / CTitleMenu. Declared as arrays so
// MWCC uses full 32-bit (lis/addi) addressing like the retail.
extern void* lbl_eu_8053B2C8[];
extern void* lbl_eu_8053B368[];
extern void* lbl_eu_8053B35C[];
// Current CTitle instance (.sbss) - file-event callback target.
extern void* lbl_eu_80664C38;

namespace cf {

class CfObjectMove;

// +0x04 battle sub-object of a func_800BFC68 result (CfObjectPc): its vtable
// +0x30 virtual (declared index 10 with the 2-entry RTTI prefix) returns a
// word holder whose first word is the actor id queried by func_80174C98.
class CHelpBattleSub4 {
public:
    virtual void vf00() = 0;  // index 0
    virtual void vf04() = 0;  // index 1
    virtual void vf08() = 0;  // index 2
    virtual void vf0C() = 0;  // index 3
    virtual void vf10() = 0;  // index 4
    virtual void vf14() = 0;  // index 5
    virtual void vf18() = 0;  // index 6
    virtual void vf1C() = 0;  // index 7
    virtual void vf20() = 0;  // index 8
    virtual void vf24() = 0;  // index 9
    virtual u32* vf30() = 0;  // index 10 -> vtable offset 0x30
};

// Minimal view of a func_800BFC68 result (CfObjectPc): only +0x04 used.
class CHelpBattleObj {
public:
    u8 pad00[0x4];                  // 0x00
    CHelpBattleSub4* field_0x4;     // 0x04
};

// Title "to attack" help hook: caches whether the A / Z buttons are held.
class CHelp_ToAttack {
public:
    int func_802B7A90();

    u8 pad00[0x0C];  // 0x00-0x0B
    s8 field_0xC;    // 0x0C - cached A-button state
    s8 field_0xD;    // 0x0D - cached Z-button state
};

} // namespace cf

// Convert a CfObjectMove to its containing battle object (declared in
// CfObjectObj.cpp); C++ linkage mangles to the retail name
// func_800BFC68__FPQ22cf12CfObjectMove.
cf::CHelpBattleObj* func_800BFC68(cf::CfObjectMove* objMove);