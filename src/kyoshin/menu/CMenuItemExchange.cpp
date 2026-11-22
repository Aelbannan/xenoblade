// Auto-scaffolded catalog TU for kyoshin/menu/CMenuItemExchange
// High-level C/C++ reconstruction from retail PPC.

#include "kyoshin/cf/CfGameManager.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuItemExchange.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/util/MemManager.hpp"  // canonical mtl::MemManager::allocate

#include <revolution/GX.h>

#include "kyoshin/CBgTex.hpp"
#include <monolib/util/FixStr.hpp>
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

extern "C" void cbRenderBefore__17CMenuItemExchangeFv(void*);
extern "C" void __dt__17CMenuItemExchangeFv(void*, int);

// name-guess: ItemEx_Create/HasInstance treat this as the screen singleton at 0x80664428.
CMenuItemExchange* sItemExInstance = NULL;
// name-guess: item-box line tab mode latch at 0x8066442C, read by ItemEx_GetFlagByte.
u8 sItemExTabMode = 0;

/**
 * Offset from the OC thunk interface (CMenuItemExchangeOC*) to the
 * containing CMenuItemExchange object. The OC system passes a pointer
 * to a sub-object embedded at this offset; thunks subtract this value
 * to recover the real this pointer before delegating.
 */
static const u32 CMENU_ITEM_EXCHANGE_OC_OFFSET = 0x6c;

/**
 * Class constructor. The base initialisers run IUIWindow's inline constructor
 * (temp CTTask<IUIWindow> vtable, null Move/Draw ptmf hooks, window fields) and
 * the IScnRender interface; MWCC then installs the composite vtable pair and
 * the four embedded widgets are constructed in the body.
 */
CMenuItemExchange::CMenuItemExchange(CScn* scene, u32 arg)
    : mScene(scene), mField74(arg), mBgTex(0), mTitleAHelp(NULL, 0), mItemBoxLine(0, 0),
      mItemBoxGrid(0, 0, 0, 1) {
    const f32 initTimer = lbl_eu_80667E78;
    mFloat511C = initTimer;
    mFloat5120 = initTimer;
    field_5118 = 0;
    sItemExTabMode = 0;
}

// Destructor: members are destroyed in reverse order (grid, line, title,
// bg), then the IUIWindow base dtor runs with flags 0; stmw/lmw frame via
// optimize_for_size.
#pragma push
#pragma optimize_for_size on
CMenuItemExchange::~CMenuItemExchange() {}
#pragma pop

// ---------------------------------------------------------------------------
// CMenuItemExchange::Init (us-801bf844)
// Re-initialise each embedded widget by assignment from a temporary (the
// compiler-generated operator= copies are emitted out of line right after
// Init), then register the OC render callback on the owning scene.
// ---------------------------------------------------------------------------
void CMenuItemExchange::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    mBgTex = CBgTex(0);
    BgTex_Acquire_3C14(&mBgTex);

    mTitleAHelp = CTitleAHelp(BdatTouchStringCell(lbl_eu_80505324, lbl_eu_80505324 + 9, 0x1a), 0);
    mTitleAHelp.CTitleAHelp_load();

    // Item-box line: register the tab entries (retail call order
    // 2,4,5,6,7,8,0xb,3,0xa) and finish the line setup.
    mItemBoxLine = CItemBoxLine((u16)mField74, 2);
    mItemBoxLine.ResetTabBytes();
    mItemBoxLine.PushTabByte(2);
    mItemBoxLine.PushTabByte(4);
    mItemBoxLine.PushTabByte(5);
    mItemBoxLine.PushTabByte(6);
    mItemBoxLine.PushTabByte(7);
    mItemBoxLine.PushTabByte(8);
    mItemBoxLine.PushTabByte(0xb);
    mItemBoxLine.PushTabByte(3);
    mItemBoxLine.PushTabByte(0xa);
    mItemBoxLine.LoadFiles();

    // Item-box grid: register the two tab lists.
    mItemBoxGrid = CItemBoxGrid(2, (u16)mField74, 0, 0);
    mItemBoxGrid.ClearListSlots();
    mItemBoxGrid.PushToList(0xb);
    mItemBoxGrid.PushToList(0xa);

    // Register the OC render callback (adjusted-this idiom: null this stays
    // null, otherwise the +0x6c sub-object).
    IScnRender* render = this;
    mScene->addRenderCB(render, 0xd, 0);
}

extern "C" void waitForDrawDone__9CDeviceVIFv();
extern "C" void BgTex_Release_3D9C(void*);

// Detach the render callback, tear down the embedded widgets, clear the
// global active flag, and reset the CfGameManager state.
void CMenuItemExchange::Term() {
    waitForDrawDone__9CDeviceVIFv();
    IScnRender* render = this;
    mScene->removeRenderCB(render);
    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    mItemBoxLine.UnloadFiles();
    mItemBoxGrid.UnloadItemBox();
    sItemExInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

// Phase handlers used by Move()'s state dispatch (retail jump table
// jumptable_eu_80533B98, 9 entries, states 0..8) are members defined below.
// Phase2Idle is still an empty stub of the undecompiled 0x3A4-byte retail
// body; noinline keeps the stub from being folded into Move.

/*
 * Per-frame update: skip while the task system is busy or the exchange screen
 * mode flag (bit 10) is set; otherwise run the current phase handler
 * (state dispatch through the retail jump table) and tick every widget.
 */
void CMenuItemExchange::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() == 0) {
        if ((gmPauseFlags & 0x200000) == 0) {
            goto body;
        }
    }
    goto end;
end:
    return;
body:
    switch (field_5118) {
    case 0: Phase0Load(); break;
    case 1: GateFrom1(); break;
    case 2: Phase2Idle(); break;
    case 3: GateFrom3(); break;
    case 4: Phase4Fade(); break;
    case 5: GateFrom5(); break;
    case 6: Phase6Input(); break;
    case 7: Phase7Entry(); break;
    case 8: GateFrom8(); break;
    }
    BgTex_Tick_3D54(&mBgTex);
    mItemBoxLine.UpdateStates();
    mItemBoxGrid.UpdateItemBox();
    mTitleAHelp.updateHelp();
}

/*
 * Render the exchange screen through a stack DrawInfo. The empty
 * if/goto/end/body chain reproduces the retail gate shape: task-busy check
 * (`bne end`), then the global mode flag tested with branch-over-branch
 * (`beq body`; fallthrough = end).
 */
void CMenuItemExchange::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() == 0 &&
        (gmPauseFlags & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (IsMenuState621F0() != 0) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        BgTex_Draw_3D7C(&mBgTex, &drawInfo);
        mItemBoxLine.DrawLayout(&drawInfo);
        mItemBoxGrid.DrawItemBoxGrid(&drawInfo);
        mTitleAHelp.drawHelp(&drawInfo);
    }
}

/* Singleton factory (retail unmangled symbol): allocate the 0x5124-byte
 * screen from the work-thread heap, build it, register it under the parent
 * process and store/return the instance (0 when one already exists). */
extern "C" CMenuItemExchange* ItemEx_Create(CProcess* registParent, CScn* scene,
                                            u32 arg) {
    if (sItemExInstance != NULL) {
        return NULL;
    }
    CMenuItemExchange* obj = new (mtl::MemManager::allocate(
        0x5124, reinterpret_cast<u32>(getWorkMem__17CWorkThreadSystemFv())))
        CMenuItemExchange(scene, arg);
    sItemExInstance = obj;
    Regist__8CProcessFP8CProcessb(sItemExInstance, registParent, false);
    return sItemExInstance;
}

// (sItemExInstance != 0)
extern "C" bool ItemEx_HasInstance() { return sItemExInstance != NULL; }

u8 ItemEx_GetFlagByte() { return sItemExTabMode; }

/* Load-finish hook: once the background layout and item-box line are ready,
 * hide the title/help bar prompt, finish the line load, ready the grid and
 * advance to phase 1 (field_5118), then kick op 0x6d. */
void CMenuItemExchange::Phase0Load() {
    if (BgTex_IsLoaded_3E34(&mBgTex) != 0 &&
        mTitleAHelp.isInitialized() != 0 &&
        mItemBoxLine.GetSelectReady() != 0) {
        mTitleAHelp.func_801C412C();
        mItemBoxLine.OpenLine();
        mItemBoxGrid.LoadItemBoxFiles();
        field_5118 = 1;
        playUISound(0x6d);
    }
}

/* Exchange-menu phase gate: once the title/help bar is idle and the item-box
 * line is ready, advance the menu to phase 2 (field_5118). */
void CMenuItemExchange::GateFrom1() {
    if (mTitleAHelp.func_801C4124() != 0 &&
        mItemBoxLine.IsReadyFlag() != 0) {
        field_5118 = 2;
    }
}

__declspec(noinline) void CMenuItemExchange::Phase2Idle() {}

/* Exchange-menu phase gate: idle title/help bar plus ready item-box grid
 * advances the menu to phase 1 (field_5118). */
void CMenuItemExchange::GateFrom3() {
    if (mTitleAHelp.func_801C4124() != 0 &&
        mItemBoxLine.IsReadyFlag() != 0 &&
        mItemBoxGrid.GetField61() != 0) {
        mField64 = 1;
    }
}

/* Phase 4: once help bar/line/grid are ready, run the fade-in timer; when it
 * leaves the initial value, apply the grid selection to help bar + line and
 * advance to phase 5, kicking menu-op 0x6d. */
void CMenuItemExchange::Phase4Fade() {
    if (mTitleAHelp.func_801C4124() != 0 &&
        mItemBoxLine.IsReadyFlag() != 0 &&
        mItemBoxGrid.IsItemBoxReady() != 0) {
        mFloat5120 = mFloat5120 + lbl_eu_80667E7C;
        if (!(mFloat5120 <= lbl_eu_80667E7C)) {
            mFloat5120 = lbl_eu_80667E78;
            mTitleAHelp.func_801C41E8(mItemBoxGrid.GetPromptState());
            mItemBoxGrid.SetInfoMsgId(mItemBoxLine.TouchTabEntryItem());
            mTitleAHelp.reopenFromClose();
            mItemBoxGrid.InitItemDisplay();
            field_5118 = 5;
            playUISound(0x6d);
        }
    }
}

/* Exchange-menu phase gate: idle title/help bar plus ready item-box grid
 * advances the menu to phase 6 (field_5118). */
void CMenuItemExchange::GateFrom5() {
    if (mTitleAHelp.func_801C4124() != 0 &&
        mItemBoxGrid.GetField61() != 0) {
        field_5118 = 6;
    }
}

/* Phase 6 (main exchange input): decode turbo/pressed pad flags (the pressed
 * bit positions depend on the game-manager mode), dispatch one cursor/grid
 * action, then run the help-bar sync and the two phase transitions. */
void CMenuItemExchange::Phase6Input() {
    CMIExPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    int turboCancel = (pad->mTurboFlags & 0x8004) != 0;
    int pageTurn = (pad->mTurboFlags & 0x10010000) != 0;
    int cancel = (pad->mTurboFlags & 0x2001) != 0;
    int decide = (pad->mTurboFlags & 0x4002) != 0;
    int up, down, leftTrigger, yHeld;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        up = (pad->mPressedFlags & 0x00400000) != 0;
        down = (pad->mPressedFlags & 0x00800000) != 0;
        leftTrigger = (pad->mPressedFlags & 0x01000000) != 0;
        yHeld = (pad->mPressedFlags & 0x00000400) != 0;
    } else {
        up = (pad->mPressedFlags & 0x00000010) != 0;
        down = (pad->mPressedFlags & 0x00000020) != 0;
        leftTrigger = (pad->mPressedFlags & 0x00000800) != 0;
        yHeld = (pad->mPressedFlags & 0x00000040) != 0;
    }

    if (up != 0) {
        mItemBoxGrid.func_801CCAF0();
    } else if (down != 0) {
        if (mItemBoxGrid.IsItemBoxActive() != 0) {
            mItemBoxGrid.HandleCancelBtn(0);
        } else if (mItemBoxGrid.GetExchFlag52C() == 0 &&
                   mItemBoxGrid.IsSubWinActive() == 0) {
            mTitleAHelp.markReplayClose();
            mItemBoxGrid.AdvanceBoxState();
            field_5118 = 7;
        }
    } else if (turboCancel != 0) {
        mItemBoxGrid.GridMoveCursorLeft();
    } else if (pageTurn != 0) {
        mItemBoxGrid.GridMoveCursorRight();
    } else if (cancel != 0) {
        mItemBoxGrid.GridMoveCursorDown();
    } else if (decide != 0) {
        mItemBoxGrid.GridMoveCursorUp();
    } else if (leftTrigger != 0) {
        mItemBoxGrid.OpenSortMenu();
    } else if (yHeld != 0) {
        mItemBoxGrid.SelectCatRow();
    }

    // Help bar follows the grid's current mode.
    mTitleAHelp.func_801C41E8(mItemBoxGrid.GetPromptState());
    if (mItemBoxGrid.GetExchFlag52C() != 0) {
        sItemExTabMode = mItemBoxGrid.GetField52D();
        mTitleAHelp.beginClose();
        mItemBoxGrid.AdvanceBoxState();
        field_5118 = 3;
    }
    if (mItemBoxGrid.GetIdleFlag542() != 0) {
        mTitleAHelp.markReplayClose();
        mItemBoxGrid.AdvanceBoxState();
        field_5118 = 7;
    }
}

/* Phase-8 entry: once the title/help bar is idle and the item-box grid is
 * ready, reset the help bar to mode 0, stop its animation, finish the line
 * load and advance the menu to phase 8 (field_5118). */
void CMenuItemExchange::Phase7Entry() {
    if (mTitleAHelp.func_801C4124() != 0 &&
        mItemBoxGrid.GetField61() != 0) {
        mTitleAHelp.func_801C41E8(0);
        mTitleAHelp.reopenFromClose();
        mItemBoxLine.OpenLine();
        field_5118 = 8;
    }
}

/* Exchange-menu phase gate: twin of GateFrom1 (idle help bar + ready
 * item-box line) advancing the menu to phase 2. */
void CMenuItemExchange::GateFrom8() {
    if (mTitleAHelp.func_801C4124() != 0 &&
        mItemBoxLine.IsReadyFlag() != 0) {
        field_5118 = 2;
    }
}

/**
 * OC thunk for cbRenderBefore. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to cbRenderBefore.
 */
extern "C" void ItemEx_ThunkRender(CMenuItemExchangeOC* param) {
    ((void(*)(void*))cbRenderBefore__17CMenuItemExchangeFv)((char*)param - 0x6c);
}

/**
 * OC thunk for the destructor. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to the real destructor.
 */
extern "C" void ItemEx_ThunkDtor(CMenuItemExchangeOC* param) {
    ((void(*)(void*))__dt__17CMenuItemExchangeFv)((char*)param - 0x6c);
}
