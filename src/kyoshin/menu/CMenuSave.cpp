// Decomp of kyoshin/menu/CMenuSave.
// Menu save/load screen process (IUICf task + IScnRender MI).

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuSave.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

#include <revolution/GX.h>
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"


// Flat view of cf::CfPadData exposing only the flag words handleSaveMenuInput reads:
// CPad::mPressedButtonFlags at +0x4 and CfPadData::mTurboPressButtonFlags at
// +0x104 (see kyoshin/cf/CfPadData.hpp for the full layout).
struct MenuSavePadData {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xFC];              // 0x08-0x103 - rest of the pad data
    u32 mTurboPressButtonFlags; // 0x104
};

// Message table base used by Init's CTitleAHelp name lookup (+0xb string).
extern char lbl_eu_8050F7B0[];
// Render-gate mode bitfield (.sbss; bit 21 = realtime event busy).
extern u32 lbl_eu_80663E28;

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// isIdle/BgTex_IsLoaded_3E34/... are declared int (not u8) so callers compare with
// cmpwi directly (no byte mask), matching retail.
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void teardown(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" void NandTeardown();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);

// Additional C-ABI imports used by the Move/cbRenderBefore/Init handlers
// (retail symbols are unmangled / non-namespace, so C linkage is required).
extern "C" void openSaveMenu(CMenuSave* self);
extern "C" void advanceSaveMenuPhase(CMenuSave* self);
extern "C" void handleSaveMenuInput(CMenuSave* self);
extern "C" void flagSaveMenuReady(CMenuSave* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Acquire_3C14(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();
extern "C" void NandAllocBufs();

// DrawInfo helper - retail emits the mangled nw4r form
// (SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo), so it is declared as a normal C++
// global function to let MWCC mangle it - NOT inside the extern "C" block.

/*
 * Constructor. The widgets are built by the initialiser list; the trailing
 * stores and the presentation flag follow.
 */
CMenuSave::CMenuSave(CProcess* parent, u32 arg208, u32 arg209, u32 arg20A)
    : mParentRef(parent), mBgTex(0), mTitleAHelp(0, 0), mSaveLoad(0, 0) {
    mField208 = arg208;
    mField209 = arg209;
    mField20A = arg20A;
    mState = 0;
    if ((u8)arg20A != 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
    }
}

/*
 * Destructor. The compiler destroys mSaveLoad, mTitleAHelp, mBgTex and the
 * IUICf base in reverse order.
 */
CMenuSave::~CMenuSave() {}

/*
 * Init - (re)initialise the save screen. Resets the CfGameManager init flag
 * and the media when requested, then rebuilds each embedded widget (CBgTex /
 * CTitleAHelp / CSaveLoad) by assignment from a temporary, then registers the render callback on the
 * parent scene (see CMenuKizunaTalkList::Init for the same pattern).
 */
void CMenuSave::Init() {
    if (mField20A != 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
    }
    if (mField208 == 0) {
        NandAllocBufs();
    }

    mBgTex = CBgTex(0);
    BgTex_Acquire_3C14(&mBgTex);

    char* name = BdatTouchStringCell(lbl_eu_8050F7B0, lbl_eu_8050F7B0 + 0xb,
                                mField208 != 0 ? 0x49 : 0x48);
    mTitleAHelp = CTitleAHelp(name, mField208 != 0 ? 0x6b : 0x69);
    mTitleAHelp.CTitleAHelp_load();

    mSaveLoad = CSaveLoad(mField208, mField209);
    mSaveLoad.loadSaveData();

    // Register this screen as a render callback on the parent scene.
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(this, 0xd, 0);
}

void CMenuSave::Term() {
    CDeviceVI::waitForDrawDone();

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    mSaveLoad.reset();

    sInstance = nullptr;

    if (mField208 == 0) {
        NandTeardown();
    }
    if (mField20A != 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(0);
    }
}

// Per-frame update: gate on the task/busy flags, drive the 4-state FSM
// (phase handlers openSaveMenu/E4E0/E530/E768), then refresh the background,
// title/help bar and save-load panel each frame.
void CMenuSave::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        openSaveMenu(this);
        break;
    case 1:
        advanceSaveMenuPhase(this);
        break;
    case 2:
        handleSaveMenuInput(this);
        break;
    case 3:
        flagSaveMenuReady(this);
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    updateHelp(&mTitleAHelp);
    mSaveLoad.update();
}

// Render the save screen through a stack DrawInfo: gate on the task/busy
// flags and the scene-active check, then draw background, save-load panel and
// title/help bar in that order.
void CMenuSave::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    mSaveLoad.draw(&drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

CMenuSave* CMenuSave::sInstance;

/*
 * Factory: lazily allocate + construct the single save-menu instance and
 * register it as a child of `parent`. Returns the stored instance (or NULL if it
 * already exists). The last three args are forwarded to the constructor, with
 * `parentRef` becoming mParentRef.
 */
CMenuSave* CMenuSave::create(CProcess* parent, CProcess* parentRef, u32 arg208,
                             u32 arg209, u32 arg20A) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuSave* menu = new (CWorkThreadSystem::getWorkMem())
        CMenuSave(parentRef, arg208, arg209, arg20A);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

void stub_us_802908bc() {}

extern "C" unsigned long isSaveMenuActive(void) { return CMenuSave::sInstance != nullptr; }

/* Once the background, title bar and save-load panel are all ready, pull the
 * save-slot data, update the title bar, and play the open sound. */
extern "C" void openSaveMenu(CMenuSave* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 &&
        isInitialized(&self->mTitleAHelp) != 0 &&
        self->mSaveLoad.isReady() != 0) {
        u8 slot = self->mSaveLoad.getHelpKind();
        func_801C41E8(&self->mTitleAHelp, slot);
        func_801C412C(&self->mTitleAHelp);
        self->mSaveLoad.open();
        self->mState = 1;
        playUISound(0x6d);
    }
}

/* Advance the save menu to phase 2 once the title bar is idle and the
 * save-load panel has finished its intro (writes the state byte at 0x20B). */
extern "C" void advanceSaveMenuPhase(CMenuSave* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        self->mSaveLoad.getIdle() != 0) {
        self->mState = 2;
    }
}

/*
 * handleSaveMenuInput - save/load panel input handling (state 2). Reads the current
 * cf pad data; depending on the co-op/single-player controller layout,
 * different pressed-button bits map to the same actions. The button tests use
 * the (inverted) == 0 form for the turbo-flag masks, matching retail's
 * subic/subfe bool idiom.
 */
extern "C" void handleSaveMenuInput(CMenuSave* self) {
    if (self->mSaveLoad.isReady() == 0) {
        return;
    }

    if (self->mSaveLoad.getDone() != 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
        }
        self->mState = 4;
        self->mField54 = 1;
    }

    MenuSavePadData* pad = (MenuSavePadData*)cf::CfGameManager::getCfPadData();
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Co-op layout: Classic-controller style bits.
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((pressed >> 21) & 1) {
            self->mSaveLoad.confirm();
        } else if ((pressed >> 22) & 1) {
            if (self->mSaveLoad.isBusy() == 0) {
                beginClose(&self->mTitleAHelp);
                self->mSaveLoad.close();
                self->mState = 3;
            } else {
                self->mSaveLoad.cancel();
            }
        } else if ((pressed >> 28) & 1) {
            self->mSaveLoad.openDel();
        } else if ((turbo & 0x8004) == 0) {
            self->mSaveLoad.curUp();
        } else if ((((turbo >> 16) & 1) | (turbo & 8)) == 0) {
            self->mSaveLoad.curDown();
        } else if ((turbo & 0x2001) == 0) {
            self->mSaveLoad.pageUp();
        } else if ((turbo & 0x4002) == 0) {
            self->mSaveLoad.pageDown();
        } else if ((pressed >> 23) & 1) {
            if (self->mField209 == 0 &&
                self->mSaveLoad.isBusy() == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                }
                playUISound(6);
                self->mState = 4;
                self->mField54 = 1;
            }
        }
    } else {
        // Single-player layout: Wiimote-style bits.
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((pressed >> 4) & 1) {
            self->mSaveLoad.confirm();
        } else if ((pressed >> 5) & 1) {
            if (self->mSaveLoad.isBusy() == 0) {
                beginClose(&self->mTitleAHelp);
                self->mSaveLoad.close();
                self->mState = 3;
            } else {
                self->mSaveLoad.cancel();
            }
        } else if ((pressed >> 7) & 1) {
            self->mSaveLoad.openDel();
        } else if ((turbo & 0x8004) == 0) {
            self->mSaveLoad.curUp();
        } else if ((((turbo >> 16) & 1) | (turbo & 8)) == 0) {
            self->mSaveLoad.curDown();
        } else if ((turbo & 0x2001) == 0) {
            self->mSaveLoad.pageUp();
        } else if ((turbo & 0x4002) == 0) {
            self->mSaveLoad.pageDown();
        } else if ((pressed >> 10) & 1) {
            if (self->mField209 == 0 &&
                self->mSaveLoad.isBusy() == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                }
                playUISound(6);
                self->mState = 4;
                self->mField54 = 1;
            }
        }
    }

    func_801C41E8(&self->mTitleAHelp, self->mSaveLoad.getHelpKind());
}

/* Same idle+advance check as advanceSaveMenuPhase, but advances the phase flag at
 * 0x54 instead of the state byte. */
extern "C" void flagSaveMenuReady(CMenuSave* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        self->mSaveLoad.getIdle() != 0) {
        self->mField54 = 1;
    }
}
