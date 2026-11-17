// Decomp of kyoshin/menu/CMenuShopSell.
// Shop sell menu screen process (IUICf task + IScnRender MI layout).

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuShopSell.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CItemBoxGrid.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
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

/* Local shim for cf::CfPadData - only the flag words MenuShopSellHandlePhase2 reads
 * (CfPadData.hpp owns the full type but pulls CPad/monolib umbrella headers
 * into this TU). 0x00/0x04 are CPad held/pressed flags, 0x104 turbo flags. */
struct ShopSellPadData {
    u32 mHeldFlags;                   // +0x00 (CPad::mHeldButtonFlags)
    u32 mPressedFlags;                // +0x04 (CPad::mPressedButtonFlags)
    u8 _pad08[0x104 - 0x08];          // 0x08..0x103
    u32 mTurboFlags;                  // +0x104 (CfPadData::mTurboPressButtonFlags)
};

// Item-grid C-ABI ctor/dtor (the retail 4-arg ctor is not the C++ default ctor).
extern "C" void __ct__12CItemBoxGridFUcUsUlUc(CItemBoxGrid* self, u32 a, u32 b, u32 c, u32 d);
extern "C" void __dt__12CItemBoxGridFv(CItemBoxGrid* self, int flags);

// MenuShopSellHandlePhase2 pad/flag callees (retail unmangled names). IsItemBoxActive is
// declared int so the caller compares with cmpwi directly; GetPromptState is
// declared int so the (u8) cast at the tail emits the retail clrlwi.
extern "C" int MenuTutorialIsCreated();
extern "C" ShopSellPadData* getCfPadData__Q22cf13CfGameManagerFv();
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);

// Retail-unmangled callee names (US strips the member manglings for these
// func_ helpers; GetField61 keeps its bare name too). int returns keep the
// caller's `!= 0` as a plain cmpwi (no u8 mask), like retail.
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void teardown(CTitleAHelp* self);
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);

// SetWideLocAdjust keeps its mangled nw4r form, so it is declared outside
// extern "C" for MWCC to re-mangle it.

// Shop-sell render-gate mode bitfield (.sbss; bit 21 = busy).
extern u32 lbl_eu_80663E28;

/*
 * Constructor. The IUICf base and the IScnRender subobject are built by their
 * own ctors; the members with real C++ classes are initialised in the list, the
 * item-grid storage goes through its C-ABI ctor, and the state byte is set last.
 */
CMenuShopSell::CMenuShopSell(CScn* scene) : mScene(scene), mBgTex(0), mTitleAHelp(0, 0) {
    __ct__12CItemBoxGridFUcUsUlUc(itemBoxGrid(), 0, 0, 0, 1);
    mState = 0;
}

/*
 * Destructor. The item grid is destroyed through its C-ABI dtor; the title bar,
 * background, IUICf base and storage are destroyed by the compiler.
 */
CMenuShopSell::~CMenuShopSell() {
    __dt__12CItemBoxGridFv(itemBoxGrid(), -1);
}

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
    ((CItemBoxGrid*)(itemBoxGrid()))->UnloadItemBox();

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
        MenuShopSellHandlePhase0(this);
        break;
    case 1:
        MenuShopSellHandlePhase1(this);
        break;
    case 2:
        MenuShopSellHandlePhase2(this);
        break;
    case 3:
        MenuShopSellHandlePhase3(this);
        break;
    default:
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    ((CItemBoxGrid*)(itemBoxGrid()))->UpdateItemBox();
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
    ((CItemBoxGrid*)(itemBoxGrid()))->DrawItemBoxGrid(&drawInfo);
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
extern "C" __declspec(noinline) void MenuShopSellHandlePhase0(CMenuShopSell* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 && isInitialized(&self->mTitleAHelp) != 0 &&
        ((CItemBoxGrid*)(self->itemBoxGrid()))->IsItemBoxReady() != 0) {
        func_801C412C(&self->mTitleAHelp);
        ((CItemBoxGrid*)(self->itemBoxGrid()))->InitItemDisplay();
        self->mState = 1;
        playUISound(0x6d);
    }
}

// Phase transition: once the title bar is idle and the item grid is ready,
// advance the state byte at 0x4AC4 to 2 (the CMenuGCItem::Move case-1 check,
// kept as a separate helper here).
extern "C" __declspec(noinline) void MenuShopSellHandlePhase1(CMenuShopSell* self) {
    // isIdle called via the int-returning extern declaration (CMenuCollepedia
    // pattern) so the caller compares with cmpi (retail), not a byte mask.
    if (self->mTitleAHelp.isIdle() != 0 &&
        ((CItemBoxGrid*)(self->itemBoxGrid()))->GetField61() != 0) {
        self->mState = 2;
    }
}

// Phase 2 (interaction): poll the pad and drive the item-box grid. The two
// branches differ only in the button-bit layout (pointer-input controller vs
// plain pad); each handler ends by jumping to the shared focus refresh.
extern "C" __declspec(noinline) void MenuShopSellHandlePhase2(CMenuShopSell* self) {
    if (MenuTutorialIsCreated() != 0) return;

    ShopSellPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
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
            ((CItemBoxGrid*)(self->itemBoxGrid()))->func_801CCAF0();
        } else if (p400000) {
            if (((CItemBoxGrid*)(self->itemBoxGrid()))->IsItemBoxActive() != 0) {
                ((CItemBoxGrid*)(self->itemBoxGrid()))->HandleCancelBtn(0);
            } else {
                beginClose(&self->mTitleAHelp);
                ((CItemBoxGrid*)(self->itemBoxGrid()))->AdvanceBoxState();
                self->mState = 3;
            }
        } else if (t8004) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->GridMoveCursorLeft();
        } else if (t10008) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->GridMoveCursorRight();
        } else if (t2001) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->GridMoveCursorDown();
        } else if (t4002) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->GridMoveCursorUp();
        } else if (p1000000) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->OpenSortMenu();
        } else if (p200) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->TickItemBoxGrid();
        } else if (p400) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->SelectCatRow();
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
            ((CItemBoxGrid*)(self->itemBoxGrid()))->func_801CCAF0();
        } else if (p20) {
            if (((CItemBoxGrid*)(self->itemBoxGrid()))->IsItemBoxActive() != 0) {
                ((CItemBoxGrid*)(self->itemBoxGrid()))->HandleCancelBtn(0);
            } else {
                beginClose(&self->mTitleAHelp);
                ((CItemBoxGrid*)(self->itemBoxGrid()))->AdvanceBoxState();
                self->mState = 3;
            }
        } else if (t8004) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->GridMoveCursorLeft();
        } else if (t10008) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->GridMoveCursorRight();
        } else if (t2001) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->GridMoveCursorDown();
        } else if (t4002) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->GridMoveCursorUp();
        } else if (p800) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->OpenSortMenu();
        } else if (p200) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->TickItemBoxGrid();
        } else if (p40) {
            ((CItemBoxGrid*)(self->itemBoxGrid()))->SelectCatRow();
        }
    }

    func_801C41E8(&self->mTitleAHelp, (u8)((CItemBoxGrid*)(self->itemBoxGrid()))->GetPromptState());
}

// Phase transition: once the title bar is idle and the item grid is ready,
// set the closing-state flag at 0x54 (the CMenuGCItem::Move case-3 check,
// kept as a separate helper here).
extern "C" __declspec(noinline) void MenuShopSellHandlePhase3(CMenuShopSell* self) {
    // isIdle called via the int-returning extern declaration (CMenuCollepedia
    // pattern) so the caller compares with cmpi (retail), not a byte mask.
    if (self->mTitleAHelp.isIdle() != 0 &&
        ((CItemBoxGrid*)(self->itemBoxGrid()))->GetField61() != 0) {
        self->mField54 = 1;
    }
}
