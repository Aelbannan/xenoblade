#pragma once

#include <types.h>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CCollepedia.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"

// Forward declaration for the owning scene (CScn.hpp pulls broken monolib
// umbrella headers; keep the pointer opaque, same scheme as CMenuBattleEnd).
class CScn;

/*
 * Collepedia (encyclopedia) menu screen.
 *
 * Layout:
 *   0x00: IUICf           -- common in-game menu task base (0x58 bytes)
 *   0x58: IScnRender      -- render-callback subobject (secondary base, vptr)
 *   0x5C: CScn*           -- owning scene (removeRenderCB target in Term)
 *   0x60: CBgTex          -- background layout widget
 *   0x80: CTitleAHelp     -- title/help bar
 *   0xB8: CCollepedia     -- core collepedia data/logic (0x28FC bytes)
 *  0x29B4: u8 mState      -- phase/state indicator
 *  0x29B8: f32 mTimer     -- animation/open timer
 *  0x29BC: total size     -- sizeof(CMenuCollepedia) = 0x29BC
 */
class CMenuCollepedia : public IUICf, public IScnRender {
public:
    CMenuCollepedia(CScn* scene);
    virtual ~CMenuCollepedia();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    void Colle_NavigateBack();
    void Colle_CloseMenu();
    void Colle_OpenMenu();
    void Colle_HandleInput();

    CScn* mScene;                 // 0x5C: owning scene
    CBgTex mBgTex;                // 0x60
    CTitleAHelp mTitleAHelp;      // 0x80
    CCollepedia mCollepedia;      // 0xB8 (0x28FC bytes -> 0x29B4)
    u8 mState;                    // 0x29B4
    f32 mTimer;                   // 0x29B8
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim).
extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u32 enable);
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void teardown(CTitleAHelp* self);
extern "C" void beginClose(CTitleAHelp* self);
// isIdle declared int (not u8) so callers compare with cmpwi directly (no byte mask).

// CCollepedia callees (retail-unmangled func_ names).
extern "C" int clpGetInputOk(CCollepedia* self);
extern "C" void clpFreeAllRes(CCollepedia* self);
extern "C" void clpReqCloseVw(CCollepedia* self);
extern "C" void clpOnCancel(CCollepedia* self);
extern "C" int clpIsOverlay(CCollepedia* self);
extern "C" u32 clpIsBusy(CCollepedia* self);

// Widget ctor/dtor and Init helper calls (US retail strips these names).
extern "C" void __ct__CCollepedia(CCollepedia* self);
extern "C" void __dt__11CCollepediaFv(CCollepedia* self, int dealloc);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dst, void* src);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" int BgTex_Acquire_3C14(CBgTex* self);
extern "C" void clpStartLoads(CCollepedia* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);

// Colle_HandleInput callees (retail-unmangled func_ names).
extern "C" int clpCanClaim(CCollepedia* self);
extern "C" void clpPrevCatPg(CCollepedia* self);
extern "C" void clpNextCatPg(CCollepedia* self);
extern "C" void clpOnConfirm(CCollepedia* self);
extern "C" void clpCursorLeft(CCollepedia* self);
extern "C" void clpCursorRght(CCollepedia* self);
extern "C" void clpCursorUp(CCollepedia* self);
extern "C" void clpCursorDown(CCollepedia* self);
// CfGameManager controller-type query: retail loads r3=-1 into the symbol
// (the no-arg member wrapper drops it), so call through the C name directly.

// Flat pad-data view exposing only the flag words Colle_HandleInput reads:
// CPad::mPressedButtonFlags at +0x4 and CfPadData::mTurboPressButtonFlags at
// +0x104 (see kyoshin/cf/CfPadData.hpp for the full layout).
struct MenuCollepediaPadData {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xFC];              // 0x08-0x103 - rest of the pad data
    u32 mTurboPressButtonFlags; // 0x104
};

// One 8-byte element of the CCollepedia sub-array copied by Init's loop
// (0x500 entries at CCollepedia+0xE8, stride 8).
struct CollepediaSubEntry {
    u32 field_0;
    u32 field_4;
};

// vtable / PMF / shared data (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];
extern char lbl_eu_805372A0[];   // CMenuCollepedia composite vtable
extern char lbl_eu_8050C688[];   // message-name string pool (Init title lookup)

// Shared singleton (retail name; global-scope plain extern keeps unmangled symbol).
extern u32 lbl_eu_806647D0;
