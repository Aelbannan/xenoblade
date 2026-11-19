// Skip-timer widget: shown during cutscene skips.

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuSkipTimer.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "monolib/core/CPadManager.hpp"
#include "kyoshin/CSkipTimerImports.hpp"
#include "kyoshin/menu/CMenuSkipTimerImports.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"


CMenuSkipTimer::CMenuSkipTimer(CProcess* parent)
    : mParentRef(parent), mBgTex(0), mTitleAHelp(0, 0), mFlag3(0) {}

CMenuSkipTimer::~CMenuSkipTimer() {}

void CMenuSkipTimer::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    mBgTex = CBgTex(0);
    BgTex_Acquire_3C14(&mBgTex);

    mTitleAHelp = CTitleAHelp(BdatTouchStringCell(lbl_eu_80510540, lbl_eu_80510540 + 9, 0x35), 0x75);
    mTitleAHelp.CTitleAHelp_load();

    mTimerData = CSkipTimer();
    CSkipTimer_BeginLoad(&mTimerData);

    // Register the widget's IScnRender subobject as a render callback on its
    // parent scene (the base conversion null-checks `this`).
    static_cast<CScn*>(mParentRef)->addRenderCB(this, 0xd, 0);
}

void CMenuSkipTimer::Term() {
    CDeviceVI::waitForDrawDone();

    static_cast<CScn*>(mParentRef)->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    CSkipTimerTeardown(&mTimerData);

    sInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

/* Per-frame update of the skip-timer widget.
 * 1. Bail when CTaskGame is paused or the realtime event bit is busy.
 * 2. On the skip button (config-dependent bit), open the skip-timer panel panel
 *    if it is not already engaged, then play the open cue (sound 0x6).
 * 3. Drive the 5-state FSM (mFlag3) and refresh the bg/help bar/layout each
 *    frame. */
void CMenuSkipTimer::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() || (gmPauseFlags & 0x200000)) {
        return;
    }

    CPad* pad = cf::CfGameManager::getCurrentPad();
    // Keep the extracted skip-button bit as a plain u32 (0/1): retail compares
    // the extrwi result with cmpwi directly (no bool u8 re-mask).
    u32 pressed;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        pressed = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        pressed = (pad->mPressedButtonFlags >> 10) & 1;
    }

    if (pressed) {
        if (CSkipTimerIsEngaged(&mTimerData) == 0) {
            if (CMainMenu_GetInstancePtr() != 0) {
                ArtsInfo_SetReadyFlag();
                playUISound(6);
            }
            mFlag3 = 4;
            mField54 = 1;
        }
    }

    switch (mFlag3) {
    case 0: SkipTimer_Phase0Init(this); break;
    case 1: SkipTimer_Phase1Mark2(this); break;
    case 2: SkipTimer_Phase2PollPad(this); break;
    case 3: SkipTimer_Phase3Handle(this); break;
    }

    BgTex_Tick_3D54(&mBgTex);
    mTitleAHelp.updateHelp();
    CSkipTimerTick(&mTimerData);
}

void CMenuSkipTimer::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    CSkipTimerDraw(&mTimerData, &drawInfo);
    mTitleAHelp.drawHelp(&drawInfo);
}


// ---------- Factory: lazily allocate + construct + register the widget ----------

CMenuSkipTimer* CMenuSkipTimer::sInstance;

CMenuSkipTimer* CMenuSkipTimer::create(CProcess* self, CProcess* parent) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuSkipTimer* menu = new (CWorkThreadSystem::getWorkMem()) CMenuSkipTimer(parent);
    sInstance = menu;
    menu->Regist(self, false);
    return sInstance;
}


// ---------- Idle + advance helpers ----------

// Retail-unmangled C-linkage callees in other TUs. They are emitted by retail
// under their bare func_ names (no Itanium mangling), so they are declared
// extern "C" here (same pattern as CMenuTutorial.cpp). Return types are int so
// the `!= 0` tests compile to cmpwi rather than an rlwinm byte-mask.
extern "C" {
int BgTex_IsLoaded_3E34(CBgTex* self);
void CSkipTimerFlagButton(CSkipTimer* self);
}

// When the bg texture, title/help bar, and skip-timer panel are all ready,
// engage the skip timer (start anim, flag phase) and play the open sound.
extern "C" void SkipTimer_Phase0Init(CMenuSkipTimer* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 &&
        self->mTitleAHelp.isInitialized() != 0 &&
        CSkipTimerIsReady(&self->mTimerData) != 0) {
        self->mTitleAHelp.func_801C412C();
        CSkipTimerFlagButton(&self->mTimerData);
        self->mFlag3 = 1;
        playUISound(0x6d);
    }
}

extern "C" unsigned long SkipTimer_IsActiveFlag(void) { return CMenuSkipTimer::sInstance != 0; }

// When the title/help bar is idle and the timer widget is mid-anim, mark the
// widget as having reached phase 2 (mFlag3 at 0x150).
extern "C" void SkipTimer_Phase1Mark2(CMenuSkipTimer* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 &&
        CSkipTimerGetSkipButton(&self->mTimerData) != 0) {
        self->mFlag3 = 2;
    }
}

/* Advance the skip FSM (retail SkipTimer_Phase2PollPad): while the syswin panel is
 * ready, poll the config-dependent stick/pad bits plus the turbo-button
 * masks and dispatch to the switch-key advance/retreat helpers; if the timer
 * is then active, leave the skip state machine and raise phase 3. */
extern "C" void SkipTimer_Phase2PollPad(CMenuSkipTimer* self) {
    if (CSkipTimerIsReady(&self->mTimerData) == 0) return;

    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();
    bool upM, dnM;
    u32 upBit, dnBit;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 held = pad->mPad.mPressedButtonFlags;
        u32 turbo = pad->mTurboPressButtonFlags;
        upM = (turbo & 0x4002) != 0;
        dnM = (turbo & 0x2001) != 0;
        upBit = (held >> 21) & 1;
        dnBit = (held >> 22) & 1;
    } else {
        u32 held = pad->mPad.mPressedButtonFlags;
        u32 turbo = pad->mTurboPressButtonFlags;
        upM = (turbo & 0x4002) != 0;
        dnM = (turbo & 0x2001) != 0;
        upBit = (held >> 4) & 1;
        dnBit = (held >> 5) & 1;
    }

    if (upBit) {
        CSkipTimerConfirmSkip(&self->mTimerData);
    } else if (dnBit) {
        CSkipTimerEngageSkip(&self->mTimerData);
    } else if (upM) {
        CSkipTimerNextKey(&self->mTimerData);
    } else if (dnM) {
        CSkipTimerPrevKey(&self->mTimerData);
    }

    if (isSkipActive(&self->mTimerData) != 0) {
        self->mTitleAHelp.beginClose();
        CSkipTimerLeaveSkip(&self->mTimerData);
        self->mFlag3 = 3;
    }
}

// Same idle+advance check, but advances to phase 1 (writes mField54 at 0x54).
extern "C" void SkipTimer_Phase3Handle(CMenuSkipTimer* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 &&
        CSkipTimerGetSkipButton(&self->mTimerData) != 0) {
        self->mField54 = 1;
    }
}
