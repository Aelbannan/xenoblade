// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTutorialList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuTutorialList.hpp"
#include "kyoshin/CTutorial.hpp"       // playUISound (retail playUISound__FUl)
#include "kyoshin/code_80135FDC.hpp"   // BdatTouchStringCell string-format helper
#include "kyoshin/CTaskGame.hpp"       // CTaskGame::getInstance / isFlag01Set
#include "kyoshin/cf/CfGameManager.hpp" // cf::CfGameManager::getCurrentPad
#include "monolib/core/CPadManager.hpp" // CPad::mPressedButtonFlags
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include <types.h>
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/CTutorialImports.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Shared singleton / string pool (retail names; global-scope plain extern
// keeps the symbols unmangled).
extern char lbl_eu_80510B44[];         // title/help label string pool
extern const float lbl_eu_80668DCC;     // idle timer increment (.sdata2)
extern const float lbl_eu_80668DD0;     // idle timer cap (.sdata2)
// Game-state flags word (bit 21 = menu blocked; shared with CMenuTutorial).
extern u32 lbl_eu_80663E28;

CMenuTutorialList* CMenuTutorialList::sInstance;

// Retail-unmangled phase handlers (US keeps the bare func_ names; C linkage
// so Move's dispatch calls and the definitions bind the literal symbols).
extern "C" void CMenuTutorialList_PhaseOpenList(CMenuTutorialList* self);
extern "C" void CMenuTutorialList_PhaseAdvance(CMenuTutorialList* self);
extern "C" void CMenuTutorialList_PhaseConfirm(CMenuTutorialList* self);
extern "C" void CMenuTutorialList_PhaseClose(CMenuTutorialList* self);
extern "C" void CMenuTutorialList_PhaseOpenSeq(CMenuTutorialList* self);
extern "C" void CMenuTutorialList_PhaseTurnPage(CMenuTutorialList* self);
extern "C" void CMenuTutorialList_PhaseHandleInput(CMenuTutorialList* self);
extern "C" void CMenuTutorialList_PhaseIntroDone(CMenuTutorialList* self);

// Retail-unmangled callee names (US retail strips the mangling for these
// widget helpers) -- C linkage so calls bind the literal retail symbol.
// Return types are int (not u8) so callers compare with cmpwi directly.
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void applyPaneTevColorsAlt(CTitleAHelp* self);
extern "C" void applyPaneTevColors(CTitleAHelp* self);
extern "C" void setNameText(CTitleAHelp* self, char* name);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void teardown(CTitleAHelp* self);
extern "C" void beginClose(CTitleAHelp* self);
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
// CfGameManager controller-type query: retail loads r3=-1 into the symbol (the
// no-arg member wrapper drops it), so call through the C name directly.
// UI sound playback (retail unmangled C symbol; id + param).
extern "C" void CtrlRemote_SetSharedBit(u32 soundId, u32 arg);
// Save/skip system busy queries (retail unmangled).
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();

/* Constructor: IUICf base (flags 0x54/0x55) is built by its own ctor; the
 * widget sub-objects are constructed in declaration order, then the
 * timer/flag/choice fields are set. Kept out of line so the factory calls it. */
CMenuTutorialList::CMenuTutorialList(CScn* scene, u16 choice)
    : mScene(scene), mBgTex(0), mTitleAHelp((char*)0, 0), mTutorialList(0), mTutorial(0, 0),
      mTimer(0.0f), mFlag(0), mChoice(choice) {}

/* Destructor: members (CTutorial, the TutorialList storage, CTitleAHelp,
 * CBgTex) and the IUICf base are torn down by the compiler. */
CMenuTutorialList::~CMenuTutorialList() {}

void CMenuTutorialList::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);
    code80135FDC_postIncByte_64080();

    // Re-initialise the embedded widgets: assignment from a temporary (the
    // implicit operator= copies member by member, then the temporary is
    // destroyed).
    mBgTex = CBgTex(0);
    mBgTex.BgTex_Acquire_3C14();

    mTitleAHelp = CTitleAHelp(
        BdatTouchStringCell(lbl_eu_80510B44, lbl_eu_80510B44 + 0x9, 0x23), 0x73);
    mTitleAHelp.CTitleAHelp_load();

    mTutorialList = CTutorialList(mChoice);
    mTutorialList.TutorialList_LoadLayoutFiles();

    // Register this screen's IScnRender subobject as a render callback on the
    // owning scene.
    mScene->addRenderCB(this, 0xd, 0);
}

void CMenuTutorialList::Term() {
    CDeviceVI::waitForDrawDone();

    mScene->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    mTutorialList.TutorialList_ReleaseResources();
    mTutorial.Tutorial_TeardownTutorial();

    sInstance = nullptr;

    DecMenuCounter64080();
    if (code80135FDC_getByte_64080() == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(0);
    }
}

void CMenuTutorialList::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto exit;
    }
    // Branch-over-branch guard (same shape as cbRenderBefore): exit label
    // before body label emits retail's `beq body; b exit`.
    if (!(lbl_eu_80663E28 & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    // Close request: the menu's close button (classic: bit 23, wii: bit 10)
    // plays the close sound and flags the closing state at 0x54.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 close;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        close = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        close = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (close != 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
        }
        playUISound(6);
        mFlag = 4;
        mField54 = 1;
    }

    // Phase state machine (jumptable dispatch on the phase byte).
    switch (mFlag) {
    case 0:
        CMenuTutorialList_PhaseOpenList(this);
        break;
    case 1:
        CMenuTutorialList_PhaseAdvance(this);
        break;
    case 2:
        CMenuTutorialList_PhaseConfirm(this);
        break;
    case 3:
        CMenuTutorialList_PhaseClose(this);
        break;
    case 4:
        CMenuTutorialList_PhaseOpenSeq(this);
        break;
    case 5:
        CMenuTutorialList_PhaseTurnPage(this);
        break;
    case 6:
        CMenuTutorialList_PhaseHandleInput(this);
        break;
    case 7:
        CMenuTutorialList_PhaseIntroDone(this);
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    updateHelp(&mTitleAHelp);
    mTutorialList.TutorialList_UpdatePerFrame();
    mTutorial.Tutorial_Move();
}

void CMenuTutorialList::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto exit;
    }
    // Branch-over-branch guard: the exit label before the body label keeps
    // MWCC from folding the bit test to a single `bne` -- it emits retail's
    // `beq body; b exit` (see CMenuPause::Move; docs/MWCC_CASES.md).
    if (!(lbl_eu_80663E28 & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    if (IsMenuState621F0() == 0) {
        goto exit;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    mTutorialList.TutorialList_DrawVisible(&drawInfo);
    mTutorial.Tutorial_DrawLayoutGated(&drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

/* Factory: lazily allocate + construct the single tutorial-list menu instance
 * and register it as a child of `parent`. Returns the stored instance (or 0 if
 * it already exists). */
CMenuTutorialList* CMenuTutorialList::create(CProcess* parent, CScn* scene, u32 choice) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuTutorialList* menu = new (CWorkThreadSystem::getWorkMem()) CMenuTutorialList(scene, (u16)choice);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

extern "C" unsigned long CMenuTutorialList_IsActive(void) { return CMenuTutorialList::sInstance != nullptr; }

// Phase 0 -> 1 (open): once the background, title bar and list data are all
// ready, run the list open sequence (title bar + list) and play the open
// sound, then advance the phase byte.
void CMenuTutorialList_PhaseOpenList(CMenuTutorialList* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0) {
        if (isInitialized(&self->mTitleAHelp) != 0) {
            if (self->mTutorialList.TutorialList_IsListVisible() != 0) {
                func_801C412C(&self->mTitleAHelp);
                self->mTutorialList.TutorialList_OpenListInit();
                self->mFlag = 1;
                playUISound(0x6d);
            }
        }
    }
}

// Phase 1 -> 2 (advance): once the title bar is idle and the list data is
// ready, move to the next phase.
void CMenuTutorialList_PhaseAdvance(CMenuTutorialList* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (self->mTutorialList.TutorialList_IsInitialized() != 0) {
            self->mFlag = 2;
        }
    }
}

// Phase 2 (list browsing): ramp the idle timer, read the pad (classic or Wii
// bit layout), then dispatch the first pressed action: open the highlighted
// tutorial, back out / close, page and cursor movement, or the sort-menu
// toggle (followed by the help-bar refresh).
void CMenuTutorialList_PhaseConfirm(CMenuTutorialList* self) {
    self->mTimer += lbl_eu_80668DCC;
    if (self->mTimer > lbl_eu_80668DD0) {
        self->mTimer = lbl_eu_80668DD0;
    }

    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    bool left, right, up, down;
    u32 sortToggle, select, cancel;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPad.mPressedButtonFlags;
        left = (turbo & 0x8004) != 0;
        right = ((turbo & 0x10000) | (turbo & 0x8)) != 0;
        up = (turbo & 0x2001) != 0;
        down = (turbo & 0x4002) != 0;
        select = (pressed >> 21) & 1;
        cancel = (pressed >> 22) & 1;
        sortToggle = (pressed >> 24) & 1;
    } else {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPad.mPressedButtonFlags;
        left = (turbo & 0x8004) != 0;
        right = ((turbo & 0x10000) | (turbo & 0x8)) != 0;
        up = (turbo & 0x2001) != 0;
        down = (turbo & 0x4002) != 0;
        select = (pressed >> 4) & 1;
        cancel = (pressed >> 5) & 1;
        sortToggle = (pressed >> 11) & 1;
    }

    if (select) {
        if (self->mTutorialList.TutorialList_IsSortMenuActive() != 0) {
            self->mTutorialList.TutorialList_RebuildAfterLoad();
        } else {
            self->mFlag = 4;
            self->mTutorial = CTutorial(self->mTutorialList.TutorialList_GetSelectedEntryId(), 0);
            self->mTutorial.Tutorial_LoadTutorialFiles();
        }
    } else if (cancel) {
        if (self->mTutorialList.TutorialList_IsSortMenuActive() != 0) {
            self->mTutorialList.TutorialList_AdvanceSelection(0);
        } else {
            beginClose(&self->mTitleAHelp);
            self->mTutorialList.TutorialList_BeginClose();
            self->mFlag = 3;
        }
    } else if (left) {
        self->mTutorialList.TutorialList_PageUpIdle();
    } else if (right) {
        self->mTutorialList.TutorialList_PageUpWrap();
    } else if (up) {
        self->mTutorialList.TutorialList_PageDownStep();
    } else if (down) {
        self->mTutorialList.TutorialList_PageDownBlock();
    } else if (sortToggle) {
        self->mTutorialList.TutorialList_ConfirmOpenMenu();
    }

    // Refresh the help bar for the current menu state.
    int state = self->mTutorialList.TutorialList_GetMenuStateCode();
    func_801C41E8(&self->mTitleAHelp, state);
}

// Close: same idle+ready guard as CMenuTutorialList_PhaseAdvance, but marks the closing state
// byte at 0x54 instead of advancing the phase byte.
void CMenuTutorialList_PhaseClose(CMenuTutorialList* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (self->mTutorialList.TutorialList_IsInitialized() != 0) {
            self->mField54 = 1;
        }
    }
}

// Phase handler (retail CMenuTutorialList_PhaseOpenSeq): once the tutorial data is loaded,
// mark phase 5, play the title/help intro, label the current page via the
// list's page counter, then run the tutorial open sequence.
void CMenuTutorialList_PhaseOpenSeq(CMenuTutorialList* self) {
    if (self->mTutorial.Tutorial_GetField46Mark() != 0) {
        self->mFlag = 5;
        applyPaneTevColorsAlt(&self->mTitleAHelp);
        setNameText(&self->mTitleAHelp,
            BdatTouchStringCell(lbl_eu_80510B44 + 0xe, lbl_eu_80510B44 + 0x17,
                self->mTutorialList.TutorialList_GetSelectedEntryId()));
        self->mTutorial.Tutorial_StartIfIdle();
    }
}

// Phase 5: once the tutorial data is ready, mark phase 6 and repaint the
// title/help labels for the current tutorial phase, then play the page-flip
// sound and advance the list page counter.
void CMenuTutorialList_PhaseTurnPage(CMenuTutorialList* self) {
    if (self->mTutorial.Tutorial_GetField47Mark() != 0) {
        self->mFlag = 6;
        u8 phase = self->mTutorial.Tutorial_GetPageState();
        switch (phase) {
        default:
            break;
        case 0:
            func_801C41E8(&self->mTitleAHelp, 0x6f);
            break;
        case 1:
            func_801C41E8(&self->mTitleAHelp, 0x70);
            break;
        case 2:
            func_801C41E8(&self->mTitleAHelp, 0x71);
            break;
        case 3:
            func_801C41E8(&self->mTitleAHelp, 0x72);
            break;
        }
        CtrlRemote_SetSharedBit(0x33bf + self->mTutorialList.TutorialList_GetSelectedEntryId(), 1);
        self->mTutorialList.TutorialList_RefreshRowTexts();
    }
}

// Phase 6: react to confirm/cancel input on the tutorial widget, then repaint
// the title/help labels according to the widget's current phase.
void CMenuTutorialList_PhaseHandleInput(CMenuTutorialList* self) {
    if (self->mTutorial.Tutorial_GetField46Mark() != 0) {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 first;
        u32 second;
        if (cf::CfGameManager::isClassicController(-1) != 0) {
            first = (pad->mPressedButtonFlags >> 21) & 1;
            second = (pad->mPressedButtonFlags >> 22) & 1;
        } else {
            first = (pad->mPressedButtonFlags >> 4) & 1;
            second = (pad->mPressedButtonFlags >> 5) & 1;
        }
        if (first) {
            self->mTutorial.Tutorial_NextPage();
            if (self->mTutorial.Tutorial_GetField52Mark() != 0) {
                self->mTutorial.Tutorial_AdvanceState3To4();
                self->mFlag = 7;
            }
        } else if (second) {
            self->mTutorial.Tutorial_PrevPage();
        }

        u8 phase = self->mTutorial.Tutorial_GetPageState();
        switch (phase) {
        default:
            break;
        case 0:
            func_801C41E8(&self->mTitleAHelp, 0x6f);
            break;
        case 1:
            func_801C41E8(&self->mTitleAHelp, 0x70);
            break;
        case 2:
            func_801C41E8(&self->mTitleAHelp, 0x71);
            break;
        case 3:
            func_801C41E8(&self->mTitleAHelp, 0x72);
            break;
        }
    }
}

// Phase handler (retail CMenuTutorialList_PhaseIntroDone): once the tutorial list has finished
// its intro, mark phase 2, repaint the title/help labels, start the tutorial
// and set the help-bar display mode.
void CMenuTutorialList_PhaseIntroDone(CMenuTutorialList* self) {
    if (self->mTutorial.Tutorial_GetField47Mark() != 0) {
        self->mFlag = 2;
        applyPaneTevColors(&self->mTitleAHelp);
        setNameText(&self->mTitleAHelp,
            BdatTouchStringCell(lbl_eu_80510B44, lbl_eu_80510B44 + 0x9, 0x23));
        self->mTutorial.Tutorial_TeardownTutorial();
        func_801C41E8(&self->mTitleAHelp, 0x73);
    }
}
