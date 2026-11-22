#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/menu/CMenuArtsSet
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/CLibLayout.hpp"

#include "kyoshin/menu/CMenuArtsSet.hpp"
#include "kyoshin/CMainMenu.hpp" // CMainMenu_GetInstancePtr
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "monolib/util/MemManager.hpp"        // mtl::MemManager::allocate
#include "monolib/work/CProcess.hpp"          // CProcess::Regist
#include "monolib/device/CDeviceVI.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp" // CWorkThreadSystem::getWorkMem
#include <cstddef>
#include <nw4r/lyt.h>
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Unit-private imports (shared with code_80231950.cpp): see CMenuArtsSetImports.hpp.

// Pad state import: cf::CfGameManager::getCfPadData() comes from the
// harness_catalog include chain; CfPadData.hpp supplies the struct layout.
#include "kyoshin/cf/CfPadData.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"

// Struct view of the null pointer-to-member-function constant. Reading
// through named members keeps MWCC from emitting duplicate @l references.
struct PtmfNullWordsArts {
    u32 w0;
    u32 w1;
    u32 w2;
};


// C++-linkage draw helper (mangles to the retail symbol).

CMenuArtsSet::~CMenuArtsSet() {}

void CMenuArtsSet::Term() {
    CDeviceVI::waitForDrawDone();
    static_cast<CScn*>(mScene)->removeRenderCB(this);
    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    CMenuArtsSet_teardown(&mList);
    sInstance = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuArtsSet::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() || (gmPauseFlags & 0x200000)) {
        return;
    }
    switch (mState) {
    case 0:
        if (BgTex_IsLoaded_3E34(&mBgTex) != 0 && mTitleAHelp.isInitialized() != 0 &&
            CArtsList_isReady(&mList) != 0) {
            mTitleAHelp.func_801C412C();
            CMenuArtsSet_beginOpen(&mList);
            mState = 1;
            playUISound(0x6d);
        }
        break;
    case 1:
        if (mTitleAHelp.func_801C4124() != 0 && mList.CMenuArtsSet_getReadyFlag() != 0) {
            mState = 2;
        }
        break;
    case 2:
        artsMenuInput(this);
        break;
    case 3:
        if (mTitleAHelp.func_801C4124() != 0 && mList.CMenuArtsSet_getReadyFlag() != 0) {
            mField54 = 1;
        }
        break;
    }
    BgTex_Tick_3D54(&mBgTex);
    mTitleAHelp.updateHelp();
    artsStepState(&mList);
}

void CMenuArtsSet::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    CMenuArtsSet_draw(&mList, &drawInfo);
    mTitleAHelp.drawHelp(&drawInfo);
}

CMenuArtsSet* CMenuArtsSet::sInstance;

// Singleton factory: returns NULL when the instance already exists. The
// singleton store + Regist run even when allocation failed (retail shape).
CMenuArtsSet* CMenuArtsSet::create(CProcess* parent, CScn* scene) {
    if (sInstance != 0) {
        return 0;
    }
    CMenuArtsSet* menu = new (CWorkThreadSystem::getWorkMem()) CMenuArtsSet(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

extern "C" unsigned long CMenuArtsSet_isCreated() {
    return CMenuArtsSet::sInstance != 0;
}

// Per-frame input dispatch for the arts menu (retail artsMenuInput). After
// ramping the idle timer, two mirror pad chains run: classic controller
// (isClassicController(-1) != 0) and Wii remote/nunchuk. Each tests turbo/pressed/
// held button-flag combinations and dispatches to the cursor/window handlers;
// the shared tail syncs the title help bar from the list state.
extern "C" void artsMenuInput(CMenuArtsSet* self) {
    if (MenuTutorialIsCreated() != 0) {
        return;
    }

    // Idle timer ramp, clamped at lbl_eu_80668640.
    self->mTimer += 1.0f;
    if (self->mTimer > 10.0f) {
        self->mTimer = 10.0f;
    }

    cf::CfPadData* pad = cf::CfGameManager::getCfPadData();

    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // -- classic-controller chain --
        int special = 0;
        if (CMenuArtsSet_isBusy(&self->mList) == 0 &&
            code80135FDC_getByte_64077() > 1) {
            special = 1;
        }
        self->mList.CMenuArtsSet_setArmedFlag(0);
        // turbo stays live across the two turbo tests (retail keeps it in
        // r3); pressed is loaded lazily at its first test.
        u32 turbo = pad->mTurboPressButtonFlags;
        if ((turbo & 0x02000000) != 0 && special != 0) {
            if (self->mList.CMenuArtsSet_isSlotWindowEmpty() != 0) {
                CMenuArtsSet_armSysWin(&self->mList);
            } else {
                CMenuArtsSet_selectPrevChar(&self->mList);
            }
            goto tail;
        }
        if ((turbo & 0x04000000) != 0 && special != 0) {
            if (self->mList.CMenuArtsSet_isSlotWindowEmpty() != 0) {
                CMenuArtsSet_armSysWin(&self->mList);
            } else {
                artsStepAhead(&self->mList);
            }
            goto tail;
        }
        u32 pressed = pad->mPad.mPressedButtonFlags;
        if ((pressed & 0x00200000) != 0) {
            artsDriveMenu(&self->mList);
            goto tail;
        }
        if ((pressed & 0x00400000) != 0) {
            if (CMenuArtsSet_isBusy(&self->mList) != 0) {
                CMenuArtsSet_advanceIdle(&self->mList);
            } else if (self->mList.CMenuArtsSet_isSlotWindowEmpty() != 0) {
                CMenuArtsSet_armSysWin(&self->mList);
            } else {
                self->mTitleAHelp.beginClose();
                CMenuArtsSet_beginClose(&self->mList);
                self->mState = 3;
            }
            goto tail;
        }
        if ((pressed & 0x00008004) != 0) {
            CMenuArtsSet_cursorUp(&self->mList);
            goto tail;
        }
        if ((pressed & 0x00010008) != 0) {
            CMenuArtsSet_cursorDown(&self->mList);
            goto tail;
        }
        if ((pressed & 0x00002001) != 0) {
            CMenuArtsSet_slotCursorUp(&self->mList);
            goto tail;
        }
        if ((pressed & 0x00004002) != 0) {
            CMenuArtsSet_slotCursorDown(&self->mList);
            goto tail;
        }
        if ((pressed & 0x01000000) != 0) {
            if (self->mList.CMenuArtsSet_isArtsInfoActive() == 0) {
                CMenuArtsSet_confirmSelect(&self->mList);
            }
            goto tail;
        }
        if ((pressed & 0x10000000) != 0) {
            if (self->mList.CMenuArtsSet_isArtsInfoActive() == 0) {
                CMenuArtsSet_resetSubList(&self->mList);
            }
            goto tail;
        }
        if ((pressed & 0x00800000) != 0 &&
            self->mList.CMenuArtsSet_isArtsInfoActive() == 0) {
            // Confirm: re-arm the window or close the menu with the exit sound.
            if (self->mList.CMenuArtsSet_isSlotWindowEmpty() != 0) {
                CMenuArtsSet_armSysWin(&self->mList);
            } else if (CMenuArtsSet_isWindowBusy(&self->mList) == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                    playUISound(6);
                }
                self->mState = 4;
                self->mField54 = 1;
            }
        }
        goto tail;
    } else {
        // -- Wii remote / nunchuk chain --
        self->mList.CMenuArtsSet_setArmedFlag(0);
        if ((pad->mPad.mHeldButtonFlags & 0x00001000) != 0 &&
            CMenuArtsSet_isBusy(&self->mList) == 0 &&
            code80135FDC_getByte_64077() > 1) {
            // First C-down press: play a cue when the timer had ramped, then
            // reset it and re-arm the list for pointer-style input.
            if (self->mTimer > 1.0f) {
                playUISound(2);
            }
            self->mTimer = 0.0f;
            self->mList.CMenuArtsSet_setArmedFlag(1);
            u32 turbo = pad->mTurboPressButtonFlags;
            if ((turbo & 0x00002001) != 0) {
                if (self->mList.CMenuArtsSet_isSlotWindowEmpty() != 0) {
                    CMenuArtsSet_armSysWin(&self->mList);
                } else {
                    CMenuArtsSet_selectPrevChar(&self->mList);
                }
                goto tail;
            }
            if ((turbo & 0x00004002) != 0) {
                if (self->mList.CMenuArtsSet_isSlotWindowEmpty() != 0) {
                    CMenuArtsSet_armSysWin(&self->mList);
                } else {
                    artsStepAhead(&self->mList);
                }
            }
            goto tail;
        }
        u32 pressed = pad->mPad.mPressedButtonFlags;
        if ((pressed & 0x00000010) != 0) {
            artsDriveMenu(&self->mList);
            goto tail;
        }
        if ((pressed & 0x00000020) != 0) {
            if (CMenuArtsSet_isBusy(&self->mList) != 0) {
                CMenuArtsSet_advanceIdle(&self->mList);
            } else if (self->mList.CMenuArtsSet_isSlotWindowEmpty() != 0) {
                CMenuArtsSet_armSysWin(&self->mList);
            } else {
                self->mTitleAHelp.beginClose();
                CMenuArtsSet_beginClose(&self->mList);
                self->mState = 3;
            }
            goto tail;
        }
        u32 turbo = pad->mTurboPressButtonFlags;
        if ((turbo & 0x00008004) != 0) {
            CMenuArtsSet_cursorUp(&self->mList);
            goto tail;
        }
        if ((turbo & 0x00010008) != 0) {
            CMenuArtsSet_cursorDown(&self->mList);
            goto tail;
        }
        if ((turbo & 0x00002001) != 0) {
            CMenuArtsSet_slotCursorUp(&self->mList);
            goto tail;
        }
        if ((turbo & 0x00004002) != 0) {
            CMenuArtsSet_slotCursorDown(&self->mList);
            goto tail;
        }
        if ((pressed & 0x00000800) != 0) {
            if (self->mList.CMenuArtsSet_isArtsInfoActive() == 0) {
                CMenuArtsSet_confirmSelect(&self->mList);
            }
            goto tail;
        }
        if ((pressed & 0x00000080) != 0) {
            if (self->mList.CMenuArtsSet_isArtsInfoActive() == 0) {
                CMenuArtsSet_resetSubList(&self->mList);
            }
            goto tail;
        }
        if ((pressed & 0x00000400) != 0 &&
            self->mList.CMenuArtsSet_isArtsInfoActive() == 0) {
            if (self->mList.CMenuArtsSet_isSlotWindowEmpty() != 0) {
                CMenuArtsSet_armSysWin(&self->mList);
            } else if (CMenuArtsSet_isWindowBusy(&self->mList) == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                    playUISound(6);
                }
                self->mState = 4;
                self->mField54 = 1;
            }
        }
    }
tail:
    u32 helpMode = CMenuArtsSet_getHelpMode(&self->mList);
    self->mTitleAHelp.func_801C41E8((u8)helpMode);
}
