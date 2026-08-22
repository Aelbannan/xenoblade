#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>
#include <monolib/work/CProcess.hpp>

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/COption.hpp"

/*
 * Option (settings) menu screen process.
 *
 * Layout (constructor/destructor + Init):
 *   0x00: CProcess          -- task-system base (0x3C) + vtable PMF data
 *   0x54: u8                -- phase/state flag (written by func_8029BE7C)
 *   0x58: IScnRender        -- render-callback subobject (member, vptr)
 *   0x5C: CProcess*         -- parent process reference
 *   0x60: CBgTex            -- background layout widget
 *   0x80: CTitleAHelp       -- title/help bar
 *   0xB8: COption           -- core option menu logic (0x104 bytes)
 *  0x1BC: u8 mState         -- phase/state indicator (0..4)
 *  0x1BD: u8 mArg           -- constructor argument
 */
class CMenuOption : public CProcess {
public:
    CMenuOption(CProcess* parent, u32 arg);
    virtual ~CMenuOption();

    // CProcess overrides
    virtual void Init();
    virtual void Term();
    virtual void Move();

    // Render callback (dispatched through the IScnRender subobject vtable)
    void cbRenderBefore();

    u32 ptmf0[3];                 // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];                 // 0x48-0x53: null PMF callback slot group 2
    u8 mField54;                  // 0x54: phase flag
    u8 mField55;                  // 0x55
    u8 _pad56[2];                 // 0x56-0x57
    IScnRender mIScnRender;       // 0x58: render-callback subobject (vptr)
    CProcess* mParentRef;         // 0x5C: parent process reference
    CBgTex mBgTex;                // 0x60
    CTitleAHelp mTitleAHelp;      // 0x80
    COption mOption;              // 0xB8 (0x104 bytes)
    u8 mState;                    // 0x1BC
    u8 mArg;                      // 0x1BD
    CMenuOption();
};

// IScnRender vtable this-adjusting thunks (retail: subi r3, r3, 0x58; b ...).
void func_8029BECC(void* self);
void func_8029BED4(void* self);

// Option-menu helper entry points (retail-unmangled callee names).
extern "C" void func_8029BC28(CMenuOption* self);
extern "C" void func_8029BE7C(CMenuOption* self);
extern "C" void func_8029BBB0(CMenuOption* self);
extern "C" void func_8029BC78(CMenuOption* self);

// Move()/cbRenderBefore() callees.
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_8029C4F4(COption* self);
extern "C" void func_8029C5C8(COption* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" int func_8013BE50();
// Raw-storage nw4r DrawInfo build/destroy for cbRenderBefore (pre-mangled names).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
extern u32 lbl_eu_80663E28;
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool insertTop);

// Ready/idle-check and animation helpers for the embedded sub-widgets
// (retail unmangles these member helpers; int returns so callers compare
// with cmpwi directly, no byte mask - matching retail).
extern "C" int func_801C3E34(CBgTex* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" int func_8029C734(COption* self);
extern "C" void func_8029CB9C(COption* self);
extern "C" void func_80138078__FUl(u32 op);

// Ctor/Init re-init helpers (retail-unmangled callee names - US strips
// mangling for these __ct__/func_ helpers).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void __ct__COption(COption* self, u8 arg);
// __ct__UnkClass_8011C974 is declared by COption.hpp; func_80136190 by CFloorMap.hpp.
extern "C" void func_801C3C14(CBgTex* self);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" void func_8029C35C(COption* self);

// Option-menu input-handler callees (retail-unmangled func_ names).
extern "C" void func_8029CC9C(COption* self);
extern "C" void func_8029CDB0(COption* self, int arg);
extern "C" void func_8029C7A8(COption* self);
extern "C" void func_8029C8C4(COption* self);
extern "C" void func_8029C9E8(COption* self);
extern "C" void func_8029CABC(COption* self);
extern "C" void func_8029CF7C(COption* self);
extern "C" int func_8029D054(COption* self);
extern "C" int func_8029C798(COption* self);
extern "C" int func_8029C7A0(COption* self);
extern "C" void func_8029CC30(COption* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// isIdle/func_8029C790 are declared int (not u8) so callers compare with
// cmpwi directly (no byte mask), matching retail.
extern "C" int isIdle__11CTitleAHelpFv(CTitleAHelp* h);
extern "C" int func_8029C790(COption* self);
extern "C" void func_8029C66C(COption* self);
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
// CBgTex / CTitleAHelp helpers (retail unmangles these member helpers).
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);

// D2-form subobject/base destructor helpers (explicit delete flags), used by
// the free-function form of ~CMenuOption. All are declared extern "C" so the
// literal retail symbol names are emitted (a plain C++ declaration would make
// MWCC append a __F<params> suffix). __dt__800FED0C is the CProcess base dtor
// retail symbol (0x800FF7F4 - the game-side D2 wrapper, distinct from the
// library D1 __dt__8CProcessFv at 0x804474D0).
extern "C" void __dt__7COptionFv(COption* _this, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* _this, int flags);
extern "C" void __dt__6CBgTexFv(CBgTex* _this, int flags);
extern "C" void __dt__800FED0C(CProcess* _this, int flags);

// Shared singleton state flag (cleared by Term).
extern u32 lbl_eu_80664A38;

// Data imports (MWCC does not mangle global-scope data names).
extern u8 lbl_eu_8052BF70[];    // CMenuOption temporary vtable (pre-PMF copy)
extern u8 lbl_eu_805392C8[];    // CMenuOption final vtable; IScnRender vtable at +0x24
extern u32 __ptmf_null[3];      // null pointer-to-member-function constant
extern char lbl_eu_805103C4[];  // option title/help string table base

/*
 * CfPadData overlay for the option-menu input handler (retail func_8029BC78).
 * Retail reads the current-pad pressed flags at +0x04 and the turbo-press
 * flags at +0x104 (the latter lives past CPad, in CfPadData's own flags
 * region; cf. CMenuMapSelect's CfPadDataLocal).
 */
struct CMenuOptionPadData {
    u32 mPadHeldFlags;           // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;        // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0x104 - 0x08];       // 0x08-0x103
    u32 mTurboPressButtonFlags;  // 0x104 - CfPadData::mTurboPressButtonFlags
};
