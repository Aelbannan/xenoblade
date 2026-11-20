// Decomp of kyoshin/menu/CMenuTutorial.
// Tutorial menu screen process (IUICf + IScnRender MI).

#include "kyoshin/menu/CMenuTutorial.hpp"
#include "kyoshin/CTaskGame.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmGameFlags)
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
// Forward decl instead of including kyoshin/code_80135FDC.hpp: that header's
// extern "C" BdatGetU8Direct (u8) overload-clashes with CTutorial.hpp's
// (u32) (same clash CMenuPlayAward.cpp works around); this TU only needs
// BdatTouchStringCell.
#include "monolib/core/CPadManager.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"

// ---- Declarations private to this unit ----

extern u32 gmPauseFlags; // render-gate mode bitfield (bit 21 = busy)

extern "C" {


    void CUICfManager_setTimeout30();
    // setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
    // CfGameManager controller-type query: retail loads r3=-1 into the symbol
    // (the no-arg member wrapper drops it), so call through the C name directly.

    // This-unit phase handlers (retail-unmangled func_ names). extern "C" keeps
    // the call relocs bare (retail keeps the unmangled names at these call sites).
    void MenuTutorialHandlePhase0(CMenuTutorial* self);
    void MenuTutorialHandlePhase1(CMenuTutorial* self);
    void MenuTutorialHandlePhase2(CMenuTutorial* self);
    void MenuTutorialHandlePhase3(CMenuTutorial* self);
}

// DrawInfo layout setup (retail MANGLED name
// SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo; declared as plain C++ so MWCC
// re-derives the same mangling).

// Retail-unmangled string pool entries are now literals: "MNU_main"/"name"
// (Init title lookup), "MNU_ttrl"/"title" (Phase0 name lookup).

// Singleton instance (retail lbl_eu_80664A28, .sbss).
CMenuTutorial* CMenuTutorial::sInstance;

// ---- Constructor / destructor ----

// Retail constructor (unmangled global in US, `__ct__CMenuTutorial`). The IUICf
// base (CTTask PTMF slots, mField54/mField55) and the IScnRender sub-vtable are
// set up by the base-class ctors; members follow in declaration order (parent
// ref, CTutorial, CTitleAHelp), then the body stores the state bytes and the
// input-flag snapshot.
CMenuTutorial::CMenuTutorial(CProcess* parent, u32 arg2)
    : mParentRef(parent), mTutorial(0, 0), mTitleAHelp(nullptr, 0) {
    mIsInitialised = 0;
    mType = arg2;
    mSomething = 0;
    mSavedInputFlags = 0;
    // Bit 1 of the Init flag word decides whether the open event is fired;
    // extracted once and stored, then tested.
    u32 openBit = (gmGameFlags >> 30) & 1;
    mSomething = openBit;
    if (openBit == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
    }

    code80135FDC_postIncByte_64080();
    mSavedInputFlags = cf::CfGameManager::getEnabledInputFlags();

    // Re-enable the input-flag set the player had before the tutorial:
    // classic controller (0x00600000) or core pad (0x30).
    u32 padFlags =
        cf::CfGameManager::isClassicController(-1) ? 0x00600000 : 0x30;
    cf::CfGameManager::enablePadFlags((u32)-1, false);
    cf::CfGameManager::enablePadFlags(padFlags, true);
}

// Retail destructor (`__dt__13CMenuTutorialFv`). Members (CTitleAHelp,
// CTutorial) and the IUICf / IScnRender bases are destroyed by the compiler.
CMenuTutorial::~CMenuTutorial() {}

// ---- Member functions ----

void CMenuTutorial::Init() {
    char* name = BdatTouchStringCell("MNU_main", "name", 0x23);

    // Re-initialise the embedded widgets: assignment from a temporary (the
    // implicit operator= copies member by member, then the temporary is
    // destroyed).
    mTitleAHelp = CTitleAHelp(name, 0x6f);

    mTitleAHelp.CTitleAHelp_load();

    mTutorial = CTutorial(mType, 1);

    mTutorial.Tutorial_LoadTutorialFiles();

    // Register this screen's IScnRender subobject as a render callback on the
    // owning scene.
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(this, 0x11, 0);
}

void CMenuTutorial::Term() {
    CDeviceVI::waitForDrawDone();

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    mTitleAHelp.teardown();
    mTutorial.Tutorial_TeardownTutorial();

    sInstance = nullptr;
    cf::CfGameManager::enablePadFlags((u32)-1, false);
    cf::CfGameManager::enablePadFlags(mSavedInputFlags, true);

    DecMenuCounter64080();
    if (mSomething == 0) {
        if (code80135FDC_getByte_64080() == 0) {
            setPresentationFlag__Q22cf13CfGameManagerFv(0);
        }
        CUICfManager_setTimeout30();
    }
}

void CMenuTutorial::Move() {
    CTaskGame::getInstance();
    // Gate: skip the whole move when the task is busy or the global mode bit
    // (0x200000) is set. The &&-chain + goto body + return shape reproduces
    // retail's branch-over-branch `beq body; b exit` (MWCC_CASES §8960).
    if (CTaskGame::isFlag01Set() == 0 &&
        (gmPauseFlags & (1u << 21)) == 0) {
        goto body;
    }
    return;
body:
    switch (mIsInitialised) {
    case 0:
        MenuTutorialHandlePhase0(this);
        break;
    case 1:
        MenuTutorialHandlePhase1(this);
        break;
    case 2:
        MenuTutorialHandlePhase2(this);
        break;
    case 3:
        MenuTutorialHandlePhase3(this);
        break;
    }
    mTitleAHelp.updateHelp();
    mTutorial.Tutorial_Move();
    return;
}

// NOTE: Draw() is intentionally NOT defined here. It is inherited from
// CTTask<IUICf>, and the class vtable is emitted without a local override.

void CMenuTutorial::cbRenderBefore() {
    CTaskGame::getInstance();
    // Gate: skip the render when the task is busy or the global mode bit
    // (0x200000) is set. Same branch-over-branch shape as Move().
    if (CTaskGame::isFlag01Set() == 0 &&
        (gmPauseFlags & (1u << 21)) == 0) {
        goto body;
    }
    return;
body:
    if (IsMenuState621F0() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    mTutorial.Tutorial_DrawLayoutGated(&drawInfo);
    mTitleAHelp.drawHelp(&drawInfo);
    return;
}

// ---- Factory / singleton accessors ----

/*
 * Factory: lazily allocate + construct the single tutorial menu instance and
 * register it as a child of `self`. Returns the stored instance (or nullptr if
 * it already exists).
 */
CMenuTutorial* CMenuTutorial::create(CProcess* self, CProcess* parent, u32 arg2) {
    if (sInstance != nullptr) {
        return nullptr;
    }
    CMenuTutorial* menu = new (CWorkThreadSystem::getWorkMem()) CMenuTutorial(parent, arg2);
    sInstance = menu;
    menu->Regist(self, false);
    return sInstance;
}

extern "C" unsigned long MenuTutorialIsCreated(void) { return CMenuTutorial::sInstance != nullptr; }

// ---- Phase handlers (retail-unmangled, this-unit) ----

/*
 * Phase 0 handler: run the opening tutorial intro sequence. Advances the
 * tutorial widget phase and schedules the tutorial menu's next stage.
 */
void MenuTutorialHandlePhase0(CMenuTutorial* self) {
    if (self->mTitleAHelp.isInitialized() != 0 &&
        self->mTutorial.Tutorial_GetField46Mark() != 0) {
        self->mTitleAHelp.applyPaneTevColorsAlt();
        self->mTitleAHelp.setNameText(BdatTouchStringCell("MNU_ttrl", "title", self->mType));

        u8 phase = self->mTutorial.Tutorial_GetPageState();
        switch (phase) {
        case 0:
            self->mTitleAHelp.func_801C41E8(0x6f);
            break;
        case 1:
            self->mTitleAHelp.func_801C41E8(0x70);
            break;
        case 2:
            self->mTitleAHelp.func_801C41E8(0x71);
            break;
        case 3:
            self->mTitleAHelp.func_801C41E8(0x72);
            break;
        }
        self->mTitleAHelp.func_801C412C();
        self->mTutorial.Tutorial_StartIfIdle();
        self->mIsInitialised = 1;
        playUISound(0x6d);
    }
}

/*
 * When the title/help bar is idle and the tutorial widget has finished its
 * current animation, mark the tutorial as having reached phase 2.
 */
void MenuTutorialHandlePhase1(CMenuTutorial* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 &&
        self->mTutorial.Tutorial_GetField47Mark() != 0) {
        self->mIsInitialised = 2;
    }
}

/*
 * Phase 2 (case 2) handler: react to confirm/cancel input on the tutorial
 * widget, then repaint the title/help bar labels according to the widget's
 * current phase.
 */
void MenuTutorialHandlePhase2(CMenuTutorial* self) {
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
                self->mTitleAHelp.beginClose();
                self->mTutorial.Tutorial_AdvanceState3To4();
                self->mIsInitialised = 3;
            }
        } else if (second) {
            self->mTutorial.Tutorial_PrevPage();
        }

        u8 phase = self->mTutorial.Tutorial_GetPageState();
        switch (phase) {
        default:
            break;
        case 0:
            self->mTitleAHelp.func_801C41E8(0x6f);
            break;
        case 1:
            self->mTitleAHelp.func_801C41E8(0x70);
            break;
        case 2:
            self->mTitleAHelp.func_801C41E8(0x71);
            break;
        case 3:
            self->mTitleAHelp.func_801C41E8(0x72);
            break;
        }
    }
}

/*
 * Same idle+advance check as MenuTutorialHandlePhase1, but advances the tutorial to
 * phase 1 (writes the state byte at offset 0x54).
 */
void MenuTutorialHandlePhase3(CMenuTutorial* self) {
    if (self->mTitleAHelp.func_801C4124() != 0 &&
        self->mTutorial.Tutorial_GetField47Mark() != 0) {
        self->mField54 = 1;
    }
}
