// FULL_MATCH: CMenuOption_renderBeforeAdj58, CMenuOption_dtorAdj58

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuOption.hpp"
#include "kyoshin/CMainMenu.hpp" // CMainMenu_GetInstancePtr

// NOTE: kyoshin/harness_catalog.hpp deliberately omitted - its CScn clashes
// with monolib/scn.hpp (pulled in by CTaskGame.hpp below).
#include "kyoshin/CTaskGame.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"


// Option-menu helper entry points and imports (retail unmangled names; moved
// here from the header, which now carries only the class).
extern "C" CMenuOption* createOptionMenu(CProcess* registParent, CProcess* parent, u32 arg);
extern "C" u32 hasOptionMenu();

// Option-menu helper entry points (retail-unmangled callee names).
extern "C" void advanceOptionPhase2(CMenuOption* self);
extern "C" void flagOptionPhase1(CMenuOption* self);
extern "C" void advanceOptionPhase1(CMenuOption* self);
extern "C" void handleOptionInput(CMenuOption* self);

// Move()/cbRenderBefore() callees.
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern u32 gmPauseFlags;

// Ready/idle-check and animation helpers for the embedded sub-widgets
// (retail unmangles these member helpers; int returns so callers compare
// with cmpwi directly, no byte mask - matching retail).
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);


// Ctor/Init re-init helpers (retail-unmangled callee names - US strips
// mangling for these __ct__/func_ helpers).
extern "C" void BgTex_Acquire_3C14(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.

// Option-menu input-handler callees (retail-unmangled func_ names).
// Declared by kyoshin/CMainMenu.hpp.
extern "C" void ArtsInfo_SetReadyFlag();

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// isIdle/COptionGetLiveFlag are declared int (not u8) so callers compare with
// cmpwi directly (no byte mask), matching retail.
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
// CBgTex / CTitleAHelp helpers (retail unmangles these member helpers).
extern "C" void BgTex_Release_3D9C(CBgTex* self);

// D2-form subobject/base destructor helpers (explicit delete flags), used by
// the free-function form of ~CMenuOption. All are declared extern "C" so the
// literal retail symbol names are emitted (a plain C++ declaration would make
// retail symbol (0x800FF7F4 - the game-side D2 wrapper, distinct from the
// library D1 __dt__8CProcessFv at 0x804474D0).

// Shared singleton state flag (cleared by Term).
extern u32 lbl_eu_80664A38[2];

// Data imports (MWCC does not mangle global-scope data names).
extern char lbl_eu_805103C4[];  // option title/help string table base

/*
 * CfPadData overlay for the option-menu input handler (retail handleOptionInput).
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

/* Constructor. The IUICf base builds the task header and the IScnRender
 * subobject sits at +0x58; the widgets are built by the initialiser list. */
CMenuOption::CMenuOption(CProcess* parent, u32 arg)
    : mParentRef(parent), mBgTex(0), mTitleAHelp(0, 0), mOption(0) {
    mState = 0;
    mArg = arg;
}

/* The compiler destroys mOption, mTitleAHelp, mBgTex and the bases. */
CMenuOption::~CMenuOption() {}


/* Re-initialise each embedded widget by assigning a fresh temporary, start
 * its load, then register the menu as a render callback on the parent scene. */
void CMenuOption::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    mBgTex = CBgTex(0);
    BgTex_Acquire_3C14(&mBgTex);

    char* name = BdatTouchStringCell(lbl_eu_805103C4, &lbl_eu_805103C4[9], 0x3b);
    mTitleAHelp = CTitleAHelp(name, 0x6c);
    mTitleAHelp.CTitleAHelp_load();

    mOption = COption(mArg);
    mOption.rebuildWidgets();

    // Register the widget as an IScnRender render callback on its parent scene.
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(this, 0xd, 0);
}

void CMenuOption::Term() {
    CDeviceVI::waitForDrawDone();

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    mOption.teardown();

    lbl_eu_80664A38[0] = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuOption::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() || (gmPauseFlags & 0x200000))
        return;

    switch (mState) {
    case 0: advanceOptionPhase1(this); break;
    case 1: advanceOptionPhase2(this); break;
    case 2: handleOptionInput(this); break;
    case 3: flagOptionPhase1(this); break;
    }

    BgTex_Tick_3D54(&mBgTex);
    mTitleAHelp.updateHelp();
    mOption.tickState();
}

void CMenuOption::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    mOption.draw(&drawInfo);
    mTitleAHelp.drawHelp(&drawInfo);
}

/* Singleton factory (retail unmangled symbol, called from CUICfManager). Returns
 * 0 if an instance is already live; otherwise constructs the screen in work
 * memory, stores it in the shared flag and registers it under `registParent`. */
CMenuOption* createOptionMenu(CProcess* registParent, CProcess* parent, u32 arg) {
    if (lbl_eu_80664A38[0] != 0) {
        return NULL;
    }
    CMenuOption* obj = new (CWorkThreadSystem::getWorkMem()) CMenuOption(parent, arg);
    lbl_eu_80664A38[0] = (u32)obj;
    obj->Regist(registParent, false);
    return (CMenuOption*)lbl_eu_80664A38[0];
}

/* Advance the option menu to phase 1 once the background texture, title bar
 * and option panel are all ready, then start the panel intro animations and
 * play the confirm sound effect (writes the state byte at 0x1BC). */
void advanceOptionPhase1(CMenuOption* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 && self->mTitleAHelp.isInitialized() != 0 &&
        self->mOption.isWindowReady() != 0) {
        self->mTitleAHelp.func_801C412C();
        self->mOption.beginScrollSetup();
        self->mState = 1;
        playUISound(0x6d);
    }
}

/* Advance the option menu to phase 2 once the title bar is idle and the
 * option panel has finished its intro (writes the state byte at 0x1BC). */
void advanceOptionPhase2(CMenuOption* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 && self->mOption.getLiveFlag() != 0) {
        self->mState = 2;
    }
}

/* Option-menu input handler (retail handleOptionInput). Reads the current pad
 * state and dispatches to the embedded COption's directional/confirm handlers
 * depending on which control the player pressed this frame, then repaints the
 * title/help bar with the option's current index and advances the phase state
 * machine (mState / mField54). */
void handleOptionInput(CMenuOption* self) {
    if (self->mOption.isWindowReady() == 0) {
        return;
    }

    CMenuOptionPadData* pad = (CMenuOptionPadData*)cf::CfGameManager::getCfPadData();

    // Classic controllers shift the directional/confirm trigger bits of the
    // pressed flags; Wiimote/Nunchuk use different positions. The four
    // directional buttons are read from the turbo-press flags.
    bool up;
    bool down;
    bool pageUp;
    bool pageDown;
    int pageRight;
    int left;
    int right;
    int pageLeft;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        left = (pressed >> 21) & 1;
        right = (pressed >> 22) & 1;
        pageLeft = (pressed >> 28) & 1;
        pageRight = (pressed >> 23) & 1;
    } else {
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        up = (turbo & 0x8004) != 0;
        down = (turbo & 0x10008) != 0;
        pageUp = (turbo & 0x2001) != 0;
        pageDown = (turbo & 0x4002) != 0;
        left = (pressed >> 4) & 1;
        right = (pressed >> 5) & 1;
        pageLeft = (pressed >> 7) & 1;
        pageRight = (pressed >> 10) & 1;
    }

    if (left) {
        self->mOption.handleLeft();
    } else if (right) {
        self->mOption.handleRightAccept(0);
    } else if (up) {
        self->mOption.cursorUp();
    } else if (down) {
        self->mOption.cursorDown();
    } else if (pageUp) {
        self->mOption.pageUp();
    } else if (pageDown) {
        self->mOption.pageDown();
    } else if (pageLeft) {
        self->mOption.openConfigDialog();
    } else if (pageRight) {
        self->mOption.handleRightAccept(1);
    }

    int navCode = self->mOption.getNavCode();
    self->mTitleAHelp.func_801C41E8((u8)navCode);

    if (self->mOption.getConfirmGate() != 0) {
        if (self->mOption.getSecondConfirm() != 0) {
            if (CMainMenu_GetInstancePtr() != 0) {
                ArtsInfo_SetReadyFlag();
            }
            self->mState = 4;
            self->mField54 = 1;
        } else {
            self->mTitleAHelp.beginClose();
            self->mOption.confirmSelection();
            self->mState = 3;
        }
    }
}

/* Same idle+advance check as advanceOptionPhase2, but advances the option menu to
 * phase 1 (writes the state byte at offset 0x54). */
void flagOptionPhase1(CMenuOption* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 && self->mOption.getLiveFlag() != 0) {
        self->mField54 = 1;
    }
}

unsigned long hasOptionMenu(void) { return lbl_eu_80664A38[0] != 0; }

// Retail tables owned by this unit.
// .rodata (0x20): the "CMenuOption" RTTI name head and the title pool
// (lbl_eu_805103C4, used above at +0x144/+0x14D/+0x150). The tutorial string
// pool that used to be defined here belongs to kyoshin/CTutorial (see the
// splits: CTutorial owns .rodata 0x80510280-0x805103B8).
// .sbss (0x8): singleton pointer + emitted zero word (retail section is 0x8;
// no code reads the second word).
extern __declspec(section ".rodata") char lbl_eu_805103C4[0x14] =
    "MNU_main\0name\0\0\0\0\0\0";
u32 lbl_eu_80664A38[2] = { 0, 0 };
