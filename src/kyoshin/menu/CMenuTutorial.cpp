#define ISCN_RENDER_NO_DTOR // see CMenuTutorial.hpp
// Decomp of kyoshin/menu/CMenuTutorial.
// Tutorial menu screen process (CProcess + IScnRender MI).

#include "kyoshin/menu/CMenuTutorial.hpp"
#include "kyoshin/CTaskGame.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
// Forward decl instead of including kyoshin/code_80135FDC.hpp: that header's
// extern "C" BdatGetU8Direct (u8) overload-clashes with CTutorial.hpp's
// (u32) (same clash CMenuPlayAward.cpp works around); this TU only needs
// BdatTouchStringCell.
extern "C" char* BdatTouchStringCell(const void*, const void*, int);
#include "monolib/core/CPadManager.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>

/*
 * Retail constructor symbol (unmangled global in US). Written as a free
 * function so the factory (MenuTutorialCreate) emits a real bl to the bare retail
 * symbol, and returns `this` in r3 like a real constructor (retail relies on
 * it). Mirrors the CMenuCollepedia ctor pattern: base ctor, temp vtable
 * store, null PMF data copy, then the composite vtable + the IScnRender
 * sub-vtable at +0x58, then each embedded widget's ctor and the final state
 * bytes + input-flag snapshot.
 */
extern "C" __declspec(noinline) CMenuTutorial* __ct__CMenuTutorial(
    CMenuTutorial* self, CProcess* parent, u32 arg2) {
    __ct__8CProcessFv((CProcess*)self);

    // Walk the null PMF words in retail order. Element temps are declared
    // (older vregs) so MWCC colors them against the long-lived pair;
    // assignments below stay in retail load order.
    u32 composite = (u32)lbl_eu_805391C0;
    u32 e2;
    u32 v0;
    const u32* pmf;
    u32 w0;
    u32 subVtable;
    const u32 zero = 0;
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    pmf = __ptmf_null;
    w0 = *(pmf++);
    subVtable = composite + 0x24;

    // Null PMF slots in retail store order (0x40, 0x3C, 0x44, 0x4C, 0x48,
    // 0x50), with the tutorial subobject address formed mid-sequence. The
    // pointer is walked twice over the same three words.
    self->ptmf0[1] = *(pmf++);
    self->ptmf0[0] = w0;
    e2 = *(pmf++);
    self->ptmf0[2] = e2;
    pmf -= 3;
    v0 = *(pmf++);
    e2 = *(pmf++);
    self->ptmf1[1] = e2;
    self->ptmf1[0] = v0;
    e2 = *(pmf);
    self->ptmf1[2] = e2;

    self->field_54 = zero;
    self->field_55 = zero;

    // Composite vtable + IScnRender sub-vtable stored after the PMF slots.
    *(u32*)((u8*)self + 0x10) = composite;
    *(u32*)&self->mIScnRender = subVtable;
    self->mParentRef = parent;

    __ct__CTutorial(&self->mTutorial, 0, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);

    self->mIsInitialised = zero;
    self->mType = arg2;
    self->mSomething = zero;
    self->mSavedInputFlags = zero;
    // Bit 1 of the Init flag word decides whether the open event is fired;
    // extracted once and stored, then tested.
    u32 openBit = (lbl_eu_80663E24 >> 30) & 1;
    self->mSomething = openBit;
    if (openBit == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
    }

    code80135FDC_postIncByte_64080();
    self->mSavedInputFlags = cf::CfGameManager::getEnabledInputFlags();

    // Re-enable the input-flag set the player had before the tutorial:
    // classic controller (0x00600000) or core pad (0x30).
    u32 padFlags =
        isClassicController__Q22cf13CfGameManagerFv(-1) ? 0x00600000 : 0x30;
    cf::CfGameManager::enablePadFlags((u32)-1, false);
    cf::CfGameManager::enablePadFlags(padFlags, true);
    return self;
}

// Free-function dtor (retail D2): destroy widgets, then game-side CProcess
// wrapper __dt__800FED0C -- not library __dt__8CProcessFv.
extern "C" void __dt__800FED0C(CProcess* self, int flags);
extern "C" CMenuTutorial* __dt__13CMenuTutorialFv(CMenuTutorial* self, int flags) {
    if (self != 0) {
        __dt__11CTitleAHelpFv(&self->mTitleAHelp, -1);
        __dt__9CTutorialFv(&self->mTutorial, -1);
        __dt__800FED0C((CProcess*)self, 0);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

void CMenuTutorial::Init() {
    char* name = BdatTouchStringCell(lbl_eu_80510260, lbl_eu_80510260 + 9, 0x23);

    // Re-initialise the embedded CTitleAHelp via a temporary: copy the unk4
    // sub-object with its copy-ctor, then copy the remaining scalar fields.
    u8 tempTitle[0x38];
    __ct__CTitleAHelp((CTitleAHelp*)tempTitle, name, 0x6f);

    __ct__UnkClass_8011C974(&mTitleAHelp.unk4, &reinterpret_cast<CTitleAHelp*>(tempTitle)->unk4);
    mTitleAHelp.mFileHandle = reinterpret_cast<CTitleAHelp*>(tempTitle)->mFileHandle;
    mTitleAHelp.mArcResourceAccessor =
        reinterpret_cast<CTitleAHelp*>(tempTitle)->mArcResourceAccessor;
    mTitleAHelp.mLayout = reinterpret_cast<CTitleAHelp*>(tempTitle)->mLayout;
    mTitleAHelp.mAnimTrans20 = reinterpret_cast<CTitleAHelp*>(tempTitle)->mAnimTrans20;
    mTitleAHelp.mAnimTrans24 = reinterpret_cast<CTitleAHelp*>(tempTitle)->mAnimTrans24;
    mTitleAHelp.unk28 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk28;
    mTitleAHelp.unk2c = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk2c;
    mTitleAHelp.mName = reinterpret_cast<CTitleAHelp*>(tempTitle)->mName;
    mTitleAHelp.unk34 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk34;
    mTitleAHelp.unk35 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk35;
    mTitleAHelp.unk36 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk36;
    mTitleAHelp.unk37 = reinterpret_cast<CTitleAHelp*>(tempTitle)->unk37;
    __dt__11CTitleAHelpFv((CTitleAHelp*)tempTitle, -1);

    CTitleAHelp_load(&mTitleAHelp);

    // Re-initialise the embedded CTutorial via a second temporary.
    u8 tempTutorial[0x54];
    __ct__CTutorial((CTutorial*)tempTutorial, this->mType, 1);

    __ct__UnkClass_8011C974(&mTutorial.mRegion0, &reinterpret_cast<CTutorial*>(tempTutorial)->mRegion0);
    __ct__UnkClass_8011C974(&mTutorial.mRegion1, &reinterpret_cast<CTutorial*>(tempTutorial)->mRegion1);

    mTutorial.mFileHandle0 = reinterpret_cast<CTutorial*>(tempTutorial)->mFileHandle0;
    mTutorial.mFileHandle1 = reinterpret_cast<CTutorial*>(tempTutorial)->mFileHandle1;
    mTutorial.mFileHandle2 = reinterpret_cast<CTutorial*>(tempTutorial)->mFileHandle2;
    mTutorial.mAccessor0 =
        reinterpret_cast<CTutorial*>(tempTutorial)->mAccessor0;
    mTutorial.mAccessor1 =
        reinterpret_cast<CTutorial*>(tempTutorial)->mAccessor1;
    mTutorial.mpLayout = reinterpret_cast<CTutorial*>(tempTutorial)->mpLayout;
    mTutorial.mpAnimTrans0 =
        reinterpret_cast<CTutorial*>(tempTutorial)->mpAnimTrans0;
    mTutorial.mpAnimTrans1 =
        reinterpret_cast<CTutorial*>(tempTutorial)->mpAnimTrans1;
    mTutorial.field_44 = reinterpret_cast<CTutorial*>(tempTutorial)->field_44;
    mTutorial.field_45 = reinterpret_cast<CTutorial*>(tempTutorial)->field_45;
    mTutorial.field_46 = reinterpret_cast<CTutorial*>(tempTutorial)->field_46;
    mTutorial.field_47 = reinterpret_cast<CTutorial*>(tempTutorial)->field_47;
    mTutorial.field_48 = reinterpret_cast<CTutorial*>(tempTutorial)->field_48;
    mTutorial.field_4C = reinterpret_cast<CTutorial*>(tempTutorial)->field_4C;
    mTutorial.field_50 = reinterpret_cast<CTutorial*>(tempTutorial)->field_50;
    mTutorial.field_51 = reinterpret_cast<CTutorial*>(tempTutorial)->field_51;
    mTutorial.field_52 = reinterpret_cast<CTutorial*>(tempTutorial)->field_52;
    mTutorial.field_53 = reinterpret_cast<CTutorial*>(tempTutorial)->field_53;
    __dt__9CTutorialFv((CTutorial*)tempTutorial, -1);

    Tutorial_LoadTutorialFiles(&mTutorial);

    // Register this screen's IScnRender subobject as a render callback on the
    // owning scene (retail null-checks `this`).
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        cb = reinterpret_cast<IScnRender*>(&mIScnRender);
    }
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(cb, 0x11, 0);
}

void CMenuTutorial::Term() {
    CDeviceVI::waitForDrawDone();

    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>(&mIScnRender);
    }
    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(renderCB);

    teardown(&mTitleAHelp);
    Tutorial_TeardownTutorial(&mTutorial);

    lbl_eu_80664A28 = 0;
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
        (lbl_eu_80663E28 & (1u << 21)) == 0) {
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
    updateHelp(&mTitleAHelp);
    Tutorial_Move(&mTutorial);
    return;
}

// NOTE: Draw() is intentionally NOT defined here. The class vtable is
// imported (lbl_eu_805391C0), so nothing odr-uses the override and retail's
// split for this TU contains no local Draw__13CMenuTutorialFv copy.

void CMenuTutorial::cbRenderBefore() {
    CTaskGame::getInstance();
    // Gate: skip the render when the task is busy or the global mode bit
    // (0x200000) is set. Same branch-over-branch shape as Move().
    if (CTaskGame::isFlag01Set() == 0 &&
        (lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto body;
    }
    return;
body:
    if (IsMenuState621F0() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI ct/dt calls to
    // match the retail direct calls (a C++ local would virtual-
    // dispatch its scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)drawInfo);
    SetWideLocAdjust((nw4r::lyt::DrawInfo*)drawInfo);
    Tutorial_DrawLayoutGated(&mTutorial, (nw4r::lyt::DrawInfo*)drawInfo);
    drawHelp(&mTitleAHelp, (nw4r::lyt::DrawInfo*)drawInfo);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)drawInfo, -1);
    return;
}

/*
 * When the title/help bar is idle and the tutorial widget has finished its
 * current animation, mark the tutorial as having reached phase 2.
 */
void MenuTutorialHandlePhase1(CMenuTutorial* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        Tutorial_GetField47Mark(&self->mTutorial) != 0) {
        self->mIsInitialised = 2;
    }
}

/*
 * Factory: lazily allocate + construct the single tutorial menu instance and
 * register it as a child of `self`. Returns the stored instance (or 0 if it
 * already exists).
 */
CMenuTutorial* MenuTutorialCreate(CProcess* self, CProcess* parent, u32 arg2) {
    if (lbl_eu_80664A28 != 0) {
        return 0;
    }
    CMenuTutorial* obj = (CMenuTutorial*)mtl::MemManager::allocate(
        0xf4, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuTutorial(obj, parent, arg2);
    }
    lbl_eu_80664A28 = (unsigned long)obj;
    obj->Regist(self, 0x0);
    return (CMenuTutorial*)lbl_eu_80664A28;
}

/*
 * Phase 0 handler: run the opening tutorial intro sequence. Advances the
 * tutorial widget phase and schedules the tutorial menu's next stage.
 */
void MenuTutorialHandlePhase0(CMenuTutorial* self) {
    if (isInitialized(&self->mTitleAHelp) != 0 &&
        Tutorial_GetField46Mark(&self->mTutorial) != 0) {
        applyPaneTevColorsAlt(&self->mTitleAHelp);
        setNameText(&self->mTitleAHelp,
            BdatTouchStringCell(lbl_eu_80510260 + 0xe, lbl_eu_80510260 + 0x17,
                self->mType));

        u8 phase = Tutorial_GetPageState(&self->mTutorial);
        switch (phase) {
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
        func_801C412C(&self->mTitleAHelp);
        Tutorial_StartIfIdle(&self->mTutorial);
        self->mIsInitialised = 1;
        playUISound(0x6d);
    }
}

/*
 * Phase 2 (case 2) handler: react to confirm/cancel input on the tutorial
 * widget, then repaint the title/help bar labels according to the widget's
 * current phase.
 */
void MenuTutorialHandlePhase2(CMenuTutorial* self) {
    if (Tutorial_GetField46Mark(&self->mTutorial) != 0) {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 first;
        u32 second;
        if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
            first = (pad->mPressedButtonFlags >> 21) & 1;
            second = (pad->mPressedButtonFlags >> 22) & 1;
        } else {
            first = (pad->mPressedButtonFlags >> 4) & 1;
            second = (pad->mPressedButtonFlags >> 5) & 1;
        }
        if (first) {
            Tutorial_NextPage(&self->mTutorial);
            if (Tutorial_GetField52Mark(&self->mTutorial) != 0) {
                beginClose(&self->mTitleAHelp);
                Tutorial_AdvanceState3To4(&self->mTutorial);
                self->mIsInitialised = 3;
            }
        } else if (second) {
            Tutorial_PrevPage(&self->mTutorial);
        }

        u8 phase = Tutorial_GetPageState(&self->mTutorial);
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

/*
 * Same idle+advance check as MenuTutorialHandlePhase1, but advances the tutorial to
 * phase 1 (writes the state byte at offset 0x54).
 */
void MenuTutorialHandlePhase3(CMenuTutorial* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        Tutorial_GetField47Mark(&self->mTutorial) != 0) {
        self->field_54 = 1;
    }
}

/**
 * IScnRender vtable this-adjusting thunk: subi r3, r3, 0x58; b cbRenderBefore.
 */
void MenuTutorialRenderThunk58(IScnRender* sub) {
    ((CMenuTutorial*)((char*)sub - 0x58))->cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk: subi r3, r3, 0x58; b __dt__.
 * Tail-calls the destructor, forwarding the caller delete flag in r4.
 */
void MenuTutorialDtorThunk58(IScnRender* sub, int flag) {
    __dt__13CMenuTutorialFv((CMenuTutorial*)((char*)sub - 0x58), flag);
}

unsigned long MenuTutorialIsCreated(void) { return lbl_eu_80664A28 != 0; }
