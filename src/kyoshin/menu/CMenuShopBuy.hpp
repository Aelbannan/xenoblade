#pragma once

#include "kyoshin/CItemBoxLine.hpp"

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Forward declarations: the owning scene is kept opaque (same scheme as
// CMenuBattleEnd).
class CScn;


/* 8-byte copy unit used by the shop-buy data blob. ShopBuy_CopyData copies the
 * blob's sub-struct members; MWCC turns each struct assignment into the
 * retail's `mtctr` + `lwzu/stwu` counted loop. */
struct ShopBuyDataItem {
    u32 lo;  // +0x0
    u32 hi;  // +0x4
};

struct ShopBuyData24 {
    ShopBuyDataItem v[3];  // 24 bytes
};

struct ShopBuyData144 {
    ShopBuyDataItem v[18];  // 144 bytes
};

struct ShopBuyData128 {
    ShopBuyDataItem v[16];  // 128 bytes
};

/* Shop buy screen data blob (0x159 bytes), copied field-by-field by
 * ShopBuy_CopyData (and filled from Init's stack scratch copies). */
struct ShopBuyData {
    ShopBuyData24 m00;      // 0x00..0x18  (3 x 8B copy loop)
    ShopBuyData144 m18;     // 0x18..0xA8  (18 x 8B copy loop)
    u32 mA8[6];             // 0xA8..0xC0  (6 unrolled words)
    ShopBuyData24 mC0;      // 0xC0..0xD8  (3 x 8B copy loop)
    u8 mD8;                 // 0xD8        (single byte)
    ShopBuyData128 mD9;     // 0xD9..0x159 (16 x 8B copy loop)
};

/*
 * Shop buy menu screen process.
 *
 * Layout (Term / dtor / phase handlers):
 *   0x00: IUICf           -- task base (CTTask<IUICf>) + mField54/mField55
 *   0x54: u8 mField54     -- closing-state flag (written by ShopBuy_MarkClosing)
 *   0x58: IScnRender      -- render-callback subobject (base, vptr)
 *   0x5C: CScn*           -- owning scene (removeRenderCB target in Term)
 *   0x60: u32 mField60    -- ctor arg 3
 *   0x64: CBgTex          -- background layout widget (0x20 bytes)
 *   0x84: CTitleAHelp     -- title/help bar (0x38 bytes)
 *   0xBC: CItemBoxLine    -- item box line (0x63C bytes)
 *  0x6F8: u8 mState       -- phase/state indicator
 *  0x6FC: f32 mFloat6FC   -- float init (lbl_eu_80667A20 pool constant)
 *  total: 0x700
 */
class CMenuShopBuy : public IUICf, public IScnRender {
public:
    CMenuShopBuy(CScn* scene, u32 arg);
    virtual ~CMenuShopBuy();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    static CMenuShopBuy* sInstance;

    CScn* mScene;               // 0x5C: owning scene
    u32 mField60;               // 0x60: ctor arg 3
    CBgTex mBgTex;              // 0x64
    CTitleAHelp mTitleAHelp;    // 0x84
    CItemBoxLine mItemBoxLine;  // 0xBC
    u8 mState;                  // 0x6F8
    f32 mFloat6FC;              // 0x6FC: float init
};

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// BgTex_IsLoaded_3E34/ItemBoxLine_IsReadyFlag/ItemBoxLine_GetSelectReady are
// declared int (not u8) so callers compare with cmpwi directly (no byte mask).
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void BgTex_Release_3D9C(CBgTex* self);

// ShopBuy_HandleInput pad/flag callees (retail unmangled names). ItemBoxLine_IsBusy is
// declared int (not u8) so the caller compares with cmpwi directly.
extern "C" int MenuTutorialIsCreated();
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void BgTex_Acquire_3C14(CBgTex* self);

// Subobject ctors/dtors (retail keeps the unmangled names).

// Subobject draw/update hooks called by Move / cbRenderBefore (retail names
// are unmangled for these helpers too).
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);

// DrawInfo setup helper (C++ mangled name).

// Phase handlers (same TU, extern "C" definitions in CMenuShopBuy.cpp) plus
// the factory (retail unmangled name). stops
// -inline auto from inlining the small phase handlers into Move (retail keeps
// real bls; see MWCC_CASES -inline auto same-TU helpers).
extern "C" void ShopBuy_OpenPhase(CMenuShopBuy* self);
extern "C" void ShopBuy_AdvancePhase(CMenuShopBuy* self);
extern "C" void ShopBuy_HandleInput(CMenuShopBuy* self);
extern "C" void ShopBuy_MarkClosing(CMenuShopBuy* self);
extern "C" CMenuShopBuy* ShopBuy_CreateMenu(CProcess* parent, CScn* scene, u32 arg);

// Other retail-named helpers.
// unsigned int (not u32 = unsigned long): must match CItemBoxLine.hpp's
// declaration so the two extern "C" redeclarations merge.
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
// CScn.hpp pulls broken monolib umbrella headers (kyoshin headers already
// define an opaque CScn), so the render-callback removal is called through
// its retail C name (same scheme as CMenuBattleEnd / CMenuCollepedia).

// Task-game gate helpers. Declared with C linkage because CTaskGame.hpp pulls
// broken monolib umbrella headers that redefine CScn (same scheme as
// CMenuUpdate). isFlag01Set takes the task-game instance in r3.

// Shared data imports (MWCC does not mangle global-scope data names).
extern u32 gmPauseFlags;       // render-gate mode bitfield (.sbss; bit 21 = busy)
