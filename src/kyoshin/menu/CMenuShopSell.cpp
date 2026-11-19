// Decomp of kyoshin/menu/CMenuShopSell.
// Shop sell menu screen process (IUICf task + IScnRender MI layout).

#include "kyoshin/cf/CfGameManager.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuShopSell.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// isIdle returns raw int so callers use cmpwi (CMenuCollepedia pattern).

/* 8-byte copy unit used by the sell-screen data blob. MenuShopSellCopyData copies
 * the 256-item array as one struct assignment, which MWCC lowers to the
 * retail `mtctr` + `lwzu/stwu` counted loop (same shape as CMenuShopBuy's
 * ShopBuyDataItem blob copies). */
struct ShopSellDataItem {
    u32 lo;  // +0x0
    u32 hi;  // +0x4
};

/* 0x800-byte array wrapper (whole-struct assignment emits the counted copy
 * loop; C arrays are not assignable directly). */
struct ShopSellDataItems {
    ShopSellDataItem v[256];  // 0x800 bytes
};

/* 0x806-byte sell data blob: the item array lives at +0x4 (offset +0x0 is
 * skipped by MenuShopSellCopyData) with a trailing u16 at +0x804. */
struct ShopSellData {
    u32 mCount;                   // +0x0 (not copied)
    ShopSellDataItems mItems;     // +0x4..+0x803
    u16 mField804;                // +0x804
};

/* Local shim for cf::CfPadData - only the flag words HandlePhase2 reads
 * (CfPadData.hpp owns the full type but pulls CPad/monolib umbrella headers
 * into this TU). 0x00/0x04 are CPad held/pressed flags, 0x104 turbo flags. */
struct ShopSellPadData {
    u32 mHeldFlags;                   // +0x00 (CPad::mHeldButtonFlags)
    u32 mPressedFlags;                // +0x04 (CPad::mPressedButtonFlags)
    u8 _pad08[0x104 - 0x08];          // 0x08..0x103
    u32 mTurboFlags;                  // +0x104 (CfPadData::mTurboPressButtonFlags)
};

// HandlePhase2 pad/flag callees (retail unmangled names). IsItemBoxActive is
// declared int so the caller compares with cmpwi directly; GetPromptState is
// declared int so the (u8) cast at the tail emits the retail clrlwi.
extern "C" int MenuTutorialIsCreated();
extern "C" ShopSellPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void func_801C412C(CTitleAHelp* self);

// Retail-unmangled callee names (US strips the member manglings for these
// func_ helpers; GetField61 keeps its bare name too). int returns keep the
// caller's `!= 0` as a plain cmpwi (no u8 mask), like retail.
extern "C" void teardown(CTitleAHelp* self);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);

extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);

// SetWideLocAdjust keeps its mangled nw4r form, so it is declared outside
// extern "C" for MWCC to re-mangle it.

// Shop-sell render-gate mode bitfield (.sbss; bit 21 = busy).
extern u32 lbl_eu_80663E28;

/* Constructor: bases and members are built in layout order, then the state
 * byte is cleared. */
CMenuShopSell::CMenuShopSell(CScn* scene)
    : mScene(scene), mBgTex(0), mTitleAHelp(0, 0), mItemBoxGrid(0, 0, 0, 1) {
    mState = 0;
}

/* Destructor: members and bases are destroyed by the compiler. */
CMenuShopSell::~CMenuShopSell() {}

void CMenuShopSell::Init() {}

// Retail emits the weak implicit CBaseCur::operator= (__as__8CBaseCurFRC8CBaseCur)
// here, right after its first use in Init. Init is not decompiled yet (empty
// stub), so this TU does not emit it until Init's cursor assignments exist.

// Copies the 0x806-byte sell data blob: the 256 x 8-byte item array at +0x4
// (the mItems struct assignment lowers to the retail mtctr/lwzu/stwu counted
// loop) followed by the trailing u16 at +0x804.
// Open item: struct-copy loop (mItems 0x800 + mField804). Retail saves the
// src pointer first (or r6,r4,r4 before or r7,r3,r3); MWCC invariantly saves
// dst first across ~30 source shapes, 14 MWCC versions, -O4,s/-O4,p/-O3,
// -ipa/-func_align variants (same ABI-boundary class as reslistCount;
// witness rejects: r6/r7 and r3/r4 both swap, no consistent bijection).
// The loop body and tail are otherwise byte-identical.
void MenuShopSellCopyData(void* dst, void* src) {
    ShopSellData* d = static_cast<ShopSellData*>(dst);
    const ShopSellData* s = static_cast<const ShopSellData*>(src);
    d->mItems = s->mItems;
    d->mField804 = s->mField804;
}

/* Tear the shop-sell screen down: wait for the draw callback to finish,
 * remove the render callback from the owning scene, release each embedded
 * widget and clear the singleton instance flag. */
void CMenuShopSell::Term() {
    CDeviceVI::waitForDrawDone();

    mScene->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    mItemBoxGrid.UnloadItemBox();

    sInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

/* Per-frame update of the shop-sell screen. Gate on the task/busy flags, then
 * drive the 4-state FSM and refresh the bg / item grid / title bar each frame. */
void CMenuShopSell::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        HandlePhase0();
        break;
    case 1:
        HandlePhase1();
        break;
    case 2:
        HandlePhase2();
        break;
    case 3:
        HandlePhase3();
        break;
    default:
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    mItemBoxGrid.UpdateItemBox();
    updateHelp(&mTitleAHelp);
}

/* Render the shop-sell screen through a stack DrawInfo: gate on the task/busy
 * flags and the scene-active check, then draw bg, item grid and title/help
 * bar in that order. */
void CMenuShopSell::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    mItemBoxGrid.DrawItemBoxGrid(&drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

// Singleton storage (.sbss).
CMenuShopSell* CMenuShopSell::sInstance;

/* Shop-sell singleton factory: if no instance exists yet, construct the
 * CMenuShopSell through the work-thread placement new, publish it to the
 * singleton and register it as a CProcess child of `parent`. Returns the
 * singleton (or 0 when one already exists). */
CMenuShopSell* CMenuShopSell::create(CProcess* parent, CScn* scene) {
    if (sInstance != 0) {
        return 0;
    }

    CMenuShopSell* menu = new (CWorkThreadSystem::getWorkMem()) CMenuShopSell(scene);
    sInstance = menu;
    menu->Regist(parent, 0);
    return sInstance;
}

// (CMenuShopSell::sInstance != 0)
extern "C" bool MenuShopSellIsCreated() { return CMenuShopSell::sInstance != 0; }

/* Phase 0 -> 1 (open): once the background, title bar and item grid are all
 * ready, run the open sequence (title bar + item grid) and play the open
 * sound, then advance the phase byte at 0x4AC4. */
void CMenuShopSell::HandlePhase0() {
    if (BgTex_IsLoaded_3E34(&mBgTex) != 0 && mTitleAHelp.isInitialized() != 0 &&
        mItemBoxGrid.IsItemBoxReady() != 0) {
        func_801C412C(&mTitleAHelp);
        mItemBoxGrid.InitItemDisplay();
        mState = 1;
        playUISound(0x6d);
    }
}

// Phase transition: once the title bar is idle and the item grid is ready,
// advance the state byte at 0x4AC4 to 2 (the CMenuGCItem::Move case-1 check,
// kept as a separate helper here).
void CMenuShopSell::HandlePhase1() {
    // isIdle called via the int-returning extern declaration (CMenuCollepedia
    // pattern) so the caller compares with cmpi (retail), not a byte mask.
    if (mTitleAHelp.isIdle() != 0 &&
        mItemBoxGrid.GetField61() != 0) {
        mState = 2;
    }
}

// Phase 2 (interaction): poll the pad and drive the item-box grid. The two
// branches differ only in the button-bit layout (pointer-input controller vs
// plain pad); each handler ends by jumping to the shared focus refresh.
void CMenuShopSell::HandlePhase2() {
    if (MenuTutorialIsCreated() != 0) return;

    ShopSellPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Pointer input enabled: materialise the 0/1 flag bools first (retail
        // andi/rlwinm+rlwimi then subic/subfe normalisation, pressed bits via
        // extrwi), then dispatch through cmpwi/beq in this order.
        u32 turbo = pad->mTurboFlags;
        u32 pressed = pad->mPressedFlags;
        u32 t8004 = (turbo & 0x8004) != 0;
        u32 t10008 = ((turbo & 0x10000) != 0) || ((turbo & 0x8) != 0);
        u32 t2001 = (turbo & 0x2001) != 0;
        u32 t4002 = (turbo & 0x4002) != 0;
        u32 p200000 = (pressed & 0x200000) != 0;
        u32 p400000 = (pressed & 0x400000) != 0;
        u32 p1000000 = (pressed & 0x1000000) != 0;
        u32 p200 = (pressed & 0x200) != 0;
        u32 p400 = (pressed & 0x400) != 0;

        if (p200000) {
            mItemBoxGrid.func_801CCAF0();
        } else if (p400000) {
            if (mItemBoxGrid.IsItemBoxActive() != 0) {
                mItemBoxGrid.HandleCancelBtn(0);
            } else {
                beginClose(&mTitleAHelp);
                mItemBoxGrid.AdvanceBoxState();
                mState = 3;
            }
        } else if (t8004) {
            mItemBoxGrid.GridMoveCursorLeft();
        } else if (t10008) {
            mItemBoxGrid.GridMoveCursorRight();
        } else if (t2001) {
            mItemBoxGrid.GridMoveCursorDown();
        } else if (t4002) {
            mItemBoxGrid.GridMoveCursorUp();
        } else if (p1000000) {
            mItemBoxGrid.OpenSortMenu();
        } else if (p200) {
            mItemBoxGrid.TickItemBoxGrid();
        } else if (p400) {
            mItemBoxGrid.SelectCatRow();
        }
    } else {
        // Pointer input disabled: plain pad bits drive the flow.
        u32 turbo = pad->mTurboFlags;
        u32 pressed = pad->mPressedFlags;
        u32 t8004 = (turbo & 0x8004) != 0;
        u32 t10008 = ((turbo & 0x10000) != 0) || ((turbo & 0x8) != 0);
        u32 t2001 = (turbo & 0x2001) != 0;
        u32 t4002 = (turbo & 0x4002) != 0;
        u32 p10 = (pressed & 0x10) != 0;
        u32 p20 = (pressed & 0x20) != 0;
        u32 p800 = (pressed & 0x800) != 0;
        u32 p200 = (pressed & 0x200) != 0;
        u32 p40 = (pressed & 0x40) != 0;

        if (p10) {
            mItemBoxGrid.func_801CCAF0();
        } else if (p20) {
            if (mItemBoxGrid.IsItemBoxActive() != 0) {
                mItemBoxGrid.HandleCancelBtn(0);
            } else {
                beginClose(&mTitleAHelp);
                mItemBoxGrid.AdvanceBoxState();
                mState = 3;
            }
        } else if (t8004) {
            mItemBoxGrid.GridMoveCursorLeft();
        } else if (t10008) {
            mItemBoxGrid.GridMoveCursorRight();
        } else if (t2001) {
            mItemBoxGrid.GridMoveCursorDown();
        } else if (t4002) {
            mItemBoxGrid.GridMoveCursorUp();
        } else if (p800) {
            mItemBoxGrid.OpenSortMenu();
        } else if (p200) {
            mItemBoxGrid.TickItemBoxGrid();
        } else if (p40) {
            mItemBoxGrid.SelectCatRow();
        }
    }

    func_801C41E8(&mTitleAHelp, (u8)mItemBoxGrid.GetPromptState());
}

// Phase transition: once the title bar is idle and the item grid is ready,
// set the closing-state flag at 0x54 (the CMenuGCItem::Move case-3 check,
// kept as a separate helper here).
void CMenuShopSell::HandlePhase3() {
    // isIdle called via the int-returning extern declaration (CMenuCollepedia
    // pattern) so the caller compares with cmpi (retail), not a byte mask.
    if (mTitleAHelp.isIdle() != 0 &&
        mItemBoxGrid.GetField61() != 0) {
        mField54 = 1;
    }
}
