// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTitle
// Replace stubs with high-level C/C++ during decomp.

#define IWORK_EVENT_INLINE_DTOR
#include <types.h>

#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <revolution/gx/GXPixel.h>
#include <nw4r/lyt.h>
#include "kyoshin/menu/CMenuTitle.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// CfPadData access shim - only holds the flags this handler reads (see CfPadData.hpp).
struct CfPadDataLocal {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xF0];              // 0x08-0xF7 - rest of CPad
    u32 mHeldButtonFlags;       // 0xF8
    u32 mPrevHeldButtonFlags;   // 0xFC
    u32 mPressedButtonFlags;    // 0x100
    u32 mTurboPressButtonFlags; // 0x104
    u8 mTimers[0x20];           // 0x108
};

// CTTask<CMenuTitle>::~CTTask (us-802b8638) - empty body; MWCC emits the null
// check, CProcess base dtor call and conditional operator delete.
template<>
CTTask<CMenuTitle>::~CTTask() {}

// CMenuTitle::~CMenuTitle (us-802b8690) - the CTitle member and the CProcess
// base are destroyed implicitly.
CMenuTitle::~CMenuTitle() {}

// CMenuTitle::Init (us-802b8700) - attach the render callback, then start
// loading the title layouts.
void CMenuTitle::Init() {
    IScnRender* render = this;
    mScene->addRenderCB(render, 0xd, 1);
    CTitle_startLoad(&mTitle);
}

// CMenuTitle::Term (us-802b8750) - wait for draw completion, detach the render
// callback, release the title and clear the singleton.
void CMenuTitle::Term() {
    CDeviceVI::waitForDrawDone();
    IScnRender* render = this;
    mScene->removeRenderCB(render);
    CTitle_teardown(&mTitle);
    sInstance = 0;
}

// CMenuTitle::Move (us-802b87a4) - dispatch the state handler table, then
// advance the title.
void CMenuTitle::Move() {
    (this->*sStateFuncs[mState])();
    CTitle_update(&mTitle);
}

// CMenuTitle::cbRenderBefore (us-802b87f0) - when the input-enable byte is
// set, disable Z testing and draw the title with a layout DrawInfo.

void CMenuTitle::cbRenderBefore() {
    if (mInputEnable != 0) {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        CTitle_draw(&mTitle, &drawInfo);
    }
}

// Constructor (retail: the single allocating function, i.e. create()). Defined
// inline here, used only by create().
inline CMenuTitle::CMenuTitle(CScn* scene, CTitleAction* action) : mScene(scene) {
    mState = 0;
    mInputEnable = 1;
    mInputLock = 1;
    mProgress = 10.0f;
    mTitle.field_0x20 = action;
}

CMenuTitle* CMenuTitle::sInstance;

// Allocates the title menu on the work heap and registers it under `parent`.
// Returns null when a title menu already exists.
CMenuTitle* CMenuTitle::create(CProcess* parent, CScn* scene, CTitleAction* action) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuTitle* menu = new (CWorkThreadSystem::getWorkMem()) CMenuTitle(scene, action);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

// CfGameManager one-arg controller-type query, kept as the retail mangled
// C symbol (extern "C" stops C++ `__Fi` param mangling). The inline header
// wrapper drops the -1 argument, so it cannot be used here.
void CMenuTitle::stateSetReady() { mState = 1; }

void CMenuTitle::stateOnLoadDone() {
    // Opening menu: query the +0x60 sub-menu, flag this menu when it opens.
    if (CTitle_isLoadDone(&mTitle)) {
        mState = 0x2;
    }
}

void CMenuTitle::stateShowSub() {
    mState = 3;
    CTitle_showMenu(&mTitle);
}

void CMenuTitle::stateOnAnimDone4() {
    // Non-zero sub-menu query -> open the sub-menu transition state.
    if (CTitle_isAnimDone(&mTitle)) {
        mState = 0x4;
    }
}

void CMenuTitle::statePollPress() {
    CfPadDataLocal* pad =
        (CfPadDataLocal*)cf::CfGameManager::getCfPadData();

    // The activation mask differs between controller types: classic controllers
    // add the right-stick trigger bits (0x1fe0060f) to the common button bits.
    u32 mask = (cf::CfGameManager::isClassicController(-1) != 0) ? 0x1fe0060fU : 0x1effU;
    if (pad->mPadPressedFlags & mask) {
        mState = 5;
        CTitle_beginMenuOutro(&mTitle);
        mProgress = 10.0f;
    }
}

void CMenuTitle::stateAdvanceProgress() {
    // Sub-menu still closed: poll input and open it on the activation mask.
    if (CTitle_isAnimDone(&mTitle) == 0) {
        CfPadDataLocal* pad =
            (CfPadDataLocal*)cf::CfGameManager::getCfPadData();
        u32 mask = (cf::CfGameManager::isClassicController(-1) != 0)
                       ? 0x1fe0060fU
                       : 0x1effU;
        if (pad->mPadPressedFlags & mask) {
            CTitle_stepLogoMenu(&mTitle);
        }
        mProgress = 0.0f;
    } else {
        // Sub-menu open: advance the progress float toward its threshold.
        mProgress += 1.0f;
        if (mProgress < 10.0f) {
            return;
        }
        mProgress = 10.0f;
        mState = 6;
    }
}

void CMenuTitle::stateHandleInput() {
    // Only accept input while enabled (field_e9) and not in the transient
    // field_ea state (e.g. during an animation/transition). The || guard
    // makes MWCC emit the retail branch-over-branch on the last disjunct
    // (beq exit / beq body / b exit).
    if (mInputEnable == 0 || mInputLock != 0) {
        return;
    }

    {
        CfPadDataLocal* pad =
            (CfPadDataLocal*)cf::CfGameManager::getCfPadData();

        // Direction/stick trigger bits differ between controller types; retail
        // re-runs the whole extraction block per branch (both flag words reloaded).
        // 0x10008 = bit16|bit3 exceeds the andi immediate, so MWCC masks it via
        // rlwinm+rlwimi; !!booleanization lowers to the neg/or/shift sequence.
        u32 turbo;
        u32 pressed;
        u32 trigger1;
        u32 trigger2;
        u32 cancelButton;
        u32 dirButton;
        s32 controllerType = cf::CfGameManager::isClassicController(-1);
        if (controllerType != 0) {
            // Classic controller: trigger bits 21, 22 from right.
            turbo = pad->mTurboPressButtonFlags;
            pressed = pad->mPadPressedFlags;
            trigger1 = (pressed >> 21) & 1;
            trigger2 = (pressed >> 22) & 1;
            dirButton = !!(turbo & 0x8004);
            cancelButton = !!(turbo & 0x10008);
        } else {
            // Wiimote/Nunchuk: trigger bits 4, 5 from right.
            turbo = pad->mTurboPressButtonFlags;
            pressed = pad->mPadPressedFlags;
            trigger1 = (pressed >> 4) & 1;
            trigger2 = (pressed >> 5) & 1;
            dirButton = !!(turbo & 0x8004);
            cancelButton = !!(turbo & 0x10008);
        }

        // Each branch handles one input; every path falls through to the exit.
        if (trigger1 != 0) {
            if (CTitle_confirmSelection(&mTitle) != 0) {
                mState = 8;
            }
        } else if (trigger2 != 0) {
            mState = 7;
            CTitle_beginLogoOutro(&mTitle);
        } else if (dirButton != 0) {
            CTitle_moveCursorUp(&mTitle);
        } else if (cancelButton != 0) {
            CTitle_moveCursorDown(&mTitle);
        }
    }
}

void CMenuTitle::stateOnAnimDone2() {
    // Non-zero sub-menu query -> re-open the sub-menu transition state.
    if (CTitle_isAnimDone(&mTitle)) {
        mState = 0x2;
    }
}

void CMenuTitle::stateOnAnimDone9() {
    if (CTitle_isAnimDone(&mTitle)) {
        mState = 0x9;
    }
}

void CMenuTitle::stateNoop() {}

const CMenuTitle::StateFunc CMenuTitle::sStateFuncs[10] = {
    &CMenuTitle::stateSetReady,
    &CMenuTitle::stateOnLoadDone,
    &CMenuTitle::stateShowSub,
    &CMenuTitle::stateOnAnimDone4,
    &CMenuTitle::statePollPress,
    &CMenuTitle::stateAdvanceProgress,
    &CMenuTitle::stateHandleInput,
    &CMenuTitle::stateOnAnimDone2,
    &CMenuTitle::stateOnAnimDone9,
    &CMenuTitle::stateNoop,
};

// --- CTTask<CMenuTitle> out-of-line specializations ---
// MWCC lowers the PTMF dispatch to the retail __ptmf_test / __ptmf_scall calls
// (r3=this, r12=&ptmf); static_cast mirrors CTTask.hpp's inline body.
template<>
void CTTask<CMenuTitle>::Move() {
    if (mMoveFunc) {
        (static_cast<CMenuTitle*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CMenuTitle>::Draw() {
    if (mDrawFunc) {
        (static_cast<CMenuTitle*>(this)->*mDrawFunc)();
    }
}
