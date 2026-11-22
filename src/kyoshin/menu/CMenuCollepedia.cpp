// Catalog TU for kyoshin/menu/CMenuCollepedia
// FULL_MATCH: ctor, dtor, Colle_ThunkRender/Colle_ThunkDtor (now compiler-emitted thunks)

#include "kyoshin/menu/CMenuCollepedia.hpp"
#include "kyoshin/CMainMenu.hpp" // CMainMenu_GetInstancePtr

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// name-guess: retail CollepediaCreateSingleton allocates a CMenuCollepedia and
// stores it here; Colle_HasInstance reads it and Term clears it.
CMenuCollepedia* spCollepediaInstance;

// This-unit phase handlers (retail-unmangled func_ names), referenced by
// Colle_HandleInput before their definitions below. extern "C" keeps the call
// relocs bare (retail keeps the unmangled names at these call sites).
// (memberized: Colle_NavigateBack -> CMenuCollepedia::Colle_NavigateBack; declaration moved into class CMenuCollepedia)
// (memberized: Colle_CloseMenu -> CMenuCollepedia::Colle_CloseMenu; declaration moved into class CMenuCollepedia)
extern "C" void Colle_BeginOpen(CMenuCollepedia* self);
// (memberized: Colle_OpenMenu -> CMenuCollepedia::Colle_OpenMenu; declaration moved into class CMenuCollepedia)
// (memberized: Colle_HandleInput -> CMenuCollepedia::Colle_HandleInput; declaration moved into class CMenuCollepedia)
extern "C" void Colle_MarkClosing(CMenuCollepedia* self);
extern "C" void clpDoFrameUpd(CCollepedia* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);

CMenuCollepedia::CMenuCollepedia(CScn* scene)
    : mScene(scene), mBgTex(0), mTitleAHelp(nullptr, 0), mState(0), mTimer(0.0f) {}

CMenuCollepedia::~CMenuCollepedia() {}

// Re-initialise the collepedia menu screen: rebuild each embedded widget
// (CBgTex / CTitleAHelp / CCollepedia) by constructing a stack temporary and
// copying its body into the embedded storage, then register this screen as a
// render callback on the owning scene (same pattern as CMenuSave::Init).
void CMenuCollepedia::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // --- Re-initialise the background CBgTex via a temporary ---
    mBgTex = CBgTex(0);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = BdatTouchStringCell("MNU_collect", "name", 0x9);

    mTitleAHelp = CTitleAHelp(name, 0x50);

    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CCollepedia via a temporary ---
    mCollepedia = CCollepedia();

    clpStartLoads(&mCollepedia);

    // Register this screen as a render callback on the owning scene.
    mScene->addRenderCB(this, 0xd, 0);
}

// Tear down the collepedia menu: detach the render callback, release the
// background/title widgets and the collepedia core, clear the singleton, and
// hand control back to the game manager.
void CMenuCollepedia::Term() {
    waitForDrawDone__9CDeviceVIFv();

    mScene->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    clpFreeAllRes(&mCollepedia);

    spCollepediaInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuCollepedia::Move() {
    switch (mState) {
    case 0:
        Colle_BeginOpen(this);
        break;
    case 1:
        this->Colle_OpenMenu();
        break;
    case 2:
        this->Colle_HandleInput();
        break;
    case 3:
        Colle_MarkClosing(this);
        break;
    default:
        break;
    }
    BgTex_Tick_3D54(&mBgTex);
    mTitleAHelp.updateHelp();
    clpDoFrameUpd(&mCollepedia);
}

void CMenuCollepedia::cbRenderBefore(CScn*) {}

void CollepediaCreateSingleton(){}

void stub_us_80254f10() {}

void Colle_BeginOpen(){}

// Advance the collepedia menu: once the title bar is idle and the collepedia
// core is ready, open the menu (state 2).
void CMenuCollepedia::Colle_OpenMenu() {
    if (this->mTitleAHelp.func_801C4124() != 0) {
        if (clpGetInputOk(&this->mCollepedia) != 0) {
            this->mState = 2;
        }
    }
}

// Advance the collepedia menu input (retail Colle_HandleInput): once the menu is
// open, tick the open timer (clamped to 10s), then read the pad and dispatch
// to the collepedia core / menu handlers. Classic-controller (co-op) and
// Wiimote pad layouts map the same actions to different button bits.
void CMenuCollepedia::Colle_HandleInput() {
    if (GetItemMulti_IsActiveFlag() != 0) {
        return;
    }
    this->mTimer += 1.0f;
    if (this->mTimer > 10.0f) {
        this->mTimer = 10.0f;
    }

    MenuCollepediaPadData* pad =
        (MenuCollepediaPadData*)cf::CfGameManager::getCfPadData();
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Classic-controller (co-op) layout.
        if (clpCanClaim(&this->mCollepedia) != 0) {
            this->mTitleAHelp.func_801C41E8(0x50);
        } else {
            this->mTitleAHelp.func_801C41E8(0x51);
        }
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((turbo >> 6) & 1) {
            clpPrevCatPg(&this->mCollepedia);
            return;
        }
        if ((turbo >> 5) & 1) {
            clpNextCatPg(&this->mCollepedia);
            return;
        }
        if ((pressed >> 10) & 1) {
            clpOnConfirm(&this->mCollepedia);
            return;
        }
        if ((pressed >> 9) & 1) {
            this->Colle_NavigateBack();
            return;
        }
        if (turbo & 0x8004) {
            clpCursorLeft(&this->mCollepedia);
            return;
        }
        if (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) {
            clpCursorRght(&this->mCollepedia);
            return;
        }
        if (turbo & 0x2001) {
            clpCursorUp(&this->mCollepedia);
            return;
        }
        if (turbo & 0x4002) {
            clpCursorDown(&this->mCollepedia);
            return;
        }
        if ((pressed >> 8) & 1) {
            this->Colle_CloseMenu();
            return;
        }
    } else {
        // Single-player Wiimote-style layout.
        if (clpCanClaim(&this->mCollepedia) != 0) {
            this->mTitleAHelp.func_801C41E8(0x50);
        } else {
            this->mTitleAHelp.func_801C41E8(0x51);
        }
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        // Holding the confirm button (bit 19) with the core idle plays the
        // open sound once and restarts the timer, then feeds the pad through.
        if (((pad->mPadHeldFlags >> 19) & 1) &&
            clpIsBusy(&this->mCollepedia) == 0) {
            this->mTitleAHelp.func_801C41E8(0x52);
            if (this->mTimer > 1.0f) {
                playUISound(2);
            }
            this->mTimer = 0.0f;
            if (turbo & 0x2001) {
                clpPrevCatPg(&this->mCollepedia);
                return;
            }
            if (turbo & 0x4002) {
                clpNextCatPg(&this->mCollepedia);
                return;
            }
            return;
        }
        if ((pressed >> 27) & 1) {
            clpOnConfirm(&this->mCollepedia);
            return;
        }
        if ((pressed >> 26) & 1) {
            this->Colle_NavigateBack();
            return;
        }
        if (turbo & 0x8004) {
            clpCursorLeft(&this->mCollepedia);
            return;
        }
        if (((turbo >> 15) & 1) | ((turbo >> 28) & 1)) {
            clpCursorRght(&this->mCollepedia);
            return;
        }
        if (turbo & 0x2001) {
            clpCursorUp(&this->mCollepedia);
            return;
        }
        if (turbo & 0x4002) {
            clpCursorDown(&this->mCollepedia);
            return;
        }
        if ((pressed >> 21) & 1) {
            this->Colle_CloseMenu();
        }
    }
}

// Same idle+advance check as Colle_OpenMenu, but marks the menu as closing
// (mField54 = 1) instead of opening.
void Colle_MarkClosing(CMenuCollepedia* self) {
    if (self->mTitleAHelp.func_801C4124() != 0) {
        if (clpGetInputOk(&self->mCollepedia) != 0) {
            self->mField54 = 1;
        }
    }
}

// Close the collepedia menu: when the core is not blocking, run the close
// sequence (save prompt + close sound) and set state 4.
void CMenuCollepedia::Colle_CloseMenu() {
    if (clpIsBusy(&this->mCollepedia) == 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
            playUISound(6);
        }
        this->mState = 4;
        this->mField54 = 1;
    }
}

// Navigate the collepedia menu: forward input to the core, or close the
// detail view and return to the list (state 3).
void CMenuCollepedia::Colle_NavigateBack() {
    if (clpIsOverlay(&this->mCollepedia) != 0) {
        clpOnCancel(&this->mCollepedia);
    } else {
        if (clpIsBusy(&this->mCollepedia) == 0) {
            this->mTitleAHelp.beginClose();
            clpReqCloseVw(&this->mCollepedia);
            this->mState = 3;
        }
    }
}

extern "C" int Colle_HasInstance(void) { return spCollepediaInstance != 0; }
