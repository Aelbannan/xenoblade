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

/* Retail constructor symbol (unmangled global in US). Kept out-of-line so the
 * factory (CMenuTutorialList_Create) emits a real bl to it, and returns `this` in r3
 * like a real constructor (retail relies on it). */
extern "C" __declspec(noinline) CMenuTutorialList* __ct__CMenuTutorialList(
    CMenuTutorialList* self, CProcess* parent, u32 arg2) {
    __ct__8CProcessFv(self);

    // Temp (CProcess) primary vtable.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;

    // Null PMF callback slot groups.
    self->ptmf0[0] = __ptmf_null[0];
    self->ptmf0[1] = __ptmf_null[1];
    self->ptmf0[2] = __ptmf_null[2];
    self->ptmf1[0] = __ptmf_null[0];
    self->ptmf1[1] = __ptmf_null[1];
    self->ptmf1[2] = __ptmf_null[2];

    self->mField54 = 0;
    self->mField55 = 0;

    // Composite vtable + the IScnRender sub-vtable at +0x58.
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8053A238;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_8053A238 + 0x24;
    self->mScene = reinterpret_cast<CScn*>(parent);

    // Embedded widgets in construction order.
    __ct__CBgTex(&self->mBgTex, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CTutorialList(reinterpret_cast<CTutorialList*>(self->mTutorialList), 0);
    __ct__CTutorial(reinterpret_cast<CTutorial*>(self->mTutorial), 0, 0);

    self->mTimer = lbl_eu_80668DC8;
    self->mFlag = 0;
    self->mChoice = (u16)arg2;
    return self;
}

/* Destructor (retail D2 form, symbol keeps its C++ mangling in US).
 *
 * Written as a free function with an explicit flags parameter (same scheme as
 * CMenuSave's dtor): a real member destructor would make MWCC auto-emit a
 * vtable store plus the library D1 __dt__8CProcessFv for the base call,
 * neither of which matches retail. Subobjects are destroyed in reverse
 * construction order with the -1 subobject flag, then the game-side CProcess
 * dtor wrapper __dt__800FED0C with flag 0, then conditional operator delete
 * when flags > 0.
 */
extern "C" CMenuTutorialList* __dt__17CMenuTutorialListFv(CMenuTutorialList* _this, int flags) {
    if (_this != 0) {
        __dt__9CTutorialFv(reinterpret_cast<CTutorial*>(_this->mTutorial), -1);
        __dt__13CTutorialListFv(reinterpret_cast<CTutorialList*>(_this->mTutorialList), -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__6CBgTexFv(&_this->mBgTex, -1);
        __dt__800FED0C(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void CMenuTutorialList::Init() {}

void CMenuTutorialList::Term() {
    CDeviceVI::waitForDrawDone();

    // The IScnRender render callback lives at +0x58; the `if (this)` is the
    // MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58 (same as the
    // sibling CMenuTutorial/CMenuCollepedia Term bodies).
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = reinterpret_cast<IScnRender*>(&mIScnRender);
    }
    mScene->removeRenderCB(renderCB);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    TutorialList_ReleaseResources(reinterpret_cast<CTutorialList*>(mTutorialList));
    Tutorial_TeardownTutorial(reinterpret_cast<CTutorial*>(mTutorial));

    lbl_eu_80664BE8 = 0;

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
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
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
        func_802AC5E8(this);
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
    TutorialList_UpdatePerFrame(reinterpret_cast<CTutorialList*>(mTutorialList));
    func_8029AB28(reinterpret_cast<CTutorial*>(mTutorial));
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
    func_80137250(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    TutorialList_DrawVisible(reinterpret_cast<CTutorialList*>(mTutorialList), &drawInfo);
    Tutorial_DrawLayoutGated(reinterpret_cast<CTutorial*>(mTutorial), &drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

/* Factory: lazily allocate + construct the single tutorial-list menu instance
 * and register it as a child of `self`. Returns the stored instance (or 0 if
 * it already exists). */
extern "C" CMenuTutorialList* CMenuTutorialList_Create(CProcess* self, CProcess* parent, u32 arg2) {
    if (lbl_eu_80664BE8 != 0) {
        return 0;
    }
    CMenuTutorialList* obj = (CMenuTutorialList*)mtl::MemManager::allocate(
        sizeof(CMenuTutorialList), CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        obj = __ct__CMenuTutorialList(obj, parent, arg2);
    }
    lbl_eu_80664BE8 = (unsigned long)obj;
    obj->Regist(self, 0x0);
    return (CMenuTutorialList*)lbl_eu_80664BE8;
}

// Phase 0 -> 1 (open): once the background, title bar and list data are all
// ready, run the list open sequence (title bar + list) and play the open
// sound, then advance the phase byte.
void CMenuTutorialList_PhaseOpenList(CMenuTutorialList* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0) {
        if (isInitialized(&self->mTitleAHelp) != 0) {
            if (TutorialList_IsListVisible((CTutorialList*)self->mTutorialList) != 0) {
                func_801C412C(&self->mTitleAHelp);
                TutorialList_OpenListInit((CTutorialList*)self->mTutorialList);
                self->mFlag = 1;
                playUISound(0x6d);
            }
        }
    }
}

// Phase 1 -> 2 (advance): once the title bar is idle and the list data is
// ready, move to the next phase.
void CMenuTutorialList_PhaseAdvance(CMenuTutorialList* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (TutorialList_IsInitialized((CTutorialList*)self->mTutorialList) != 0) {
            self->mFlag = 2;
        }
    }
}

void func_802AC5E8(CMenuTutorialList* self) {}

// Close: same idle+ready guard as CMenuTutorialList_PhaseAdvance, but marks the closing state
// byte at 0x54 instead of advancing the phase byte.
void CMenuTutorialList_PhaseClose(CMenuTutorialList* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (TutorialList_IsInitialized((CTutorialList*)self->mTutorialList) != 0) {
            self->mField54 = 1;
        }
    }
}

// Phase handler (retail CMenuTutorialList_PhaseOpenSeq): once the tutorial data is loaded,
// mark phase 5, play the title/help intro, label the current page via the
// list's page counter, then run the tutorial open sequence.
void CMenuTutorialList_PhaseOpenSeq(CMenuTutorialList* self) {
    if (Tutorial_GetField46Mark(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
        self->mFlag = 5;
        applyPaneTevColorsAlt(&self->mTitleAHelp);
        setNameText(&self->mTitleAHelp,
            BdatTouchStringCell(lbl_eu_80510B44 + 0xe, lbl_eu_80510B44 + 0x17,
                TutorialList_GetSelectedEntryId(reinterpret_cast<CTutorialList*>(self->mTutorialList))));
        Tutorial_StartIfIdle(reinterpret_cast<CTutorial*>(self->mTutorial));
    }
}

// Phase 5: once the tutorial data is ready, mark phase 6 and repaint the
// title/help labels for the current tutorial phase, then play the page-flip
// sound and advance the list page counter.
void CMenuTutorialList_PhaseTurnPage(CMenuTutorialList* self) {
    if (Tutorial_GetField47Mark(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
        self->mFlag = 6;
        u8 phase = func_8029AE5C(reinterpret_cast<CTutorial*>(self->mTutorial));
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
        CtrlRemote_SetSharedBit(0x33bf + TutorialList_GetSelectedEntryId(reinterpret_cast<CTutorialList*>(self->mTutorialList)), 1);
        TutorialList_RefreshRowTexts(reinterpret_cast<CTutorialList*>(self->mTutorialList));
    }
}

// Phase 6: react to confirm/cancel input on the tutorial widget, then repaint
// the title/help labels according to the widget's current phase.
void CMenuTutorialList_PhaseHandleInput(CMenuTutorialList* self) {
    if (Tutorial_GetField46Mark(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
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
            func_8029AD88(reinterpret_cast<CTutorial*>(self->mTutorial));
            if (Tutorial_GetField52Mark(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
                Tutorial_AdvanceState3To4(reinterpret_cast<CTutorial*>(self->mTutorial));
                self->mFlag = 7;
            }
        } else if (second) {
            func_8029ADF8(reinterpret_cast<CTutorial*>(self->mTutorial));
        }

        u8 phase = func_8029AE5C(reinterpret_cast<CTutorial*>(self->mTutorial));
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
    if (Tutorial_GetField47Mark(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
        self->mFlag = 2;
        applyPaneTevColors(&self->mTitleAHelp);
        setNameText(&self->mTitleAHelp,
            BdatTouchStringCell(lbl_eu_80510B44, lbl_eu_80510B44 + 0x9, 0x23));
        Tutorial_TeardownTutorial(reinterpret_cast<CTutorial*>(self->mTutorial));
        func_801C41E8(&self->mTitleAHelp, 0x73);
    }
}

// --- IScnRender vtable this-adjusting thunks ---
// These are IScnRender vtable trampolines.  When CMenuTutorialList registers
// its IScnRender subobject (member at offset +0x58) as a render callback, the
// scene dispatches with an IScnRender* pointing at the subobject. Each thunk
// adjusts 'this' back by -0x58 so the real CMenuTutorialList implementation
// receives the correct pointer.

void CMenuTutorialList_RenderThunk58(IScnRender* sub) {
    cbRenderBefore__17CMenuTutorialListFv((CMenuTutorialList*)((char*)sub - 0x58));
}

void CMenuTutorialList_DtorThunk58(IScnRender* sub) {
    // Cast to a one-arg pointer so the call emits only the this-adjustment
    // (subi r3, r3, 0x58; b __dt__...) and leaves r4 as caller leftover.
    ((void(*)(void*))__dt__17CMenuTutorialListFv)((CMenuTutorialList*)((char*)sub - 0x58));
}

extern "C" unsigned long CMenuTutorialList_IsActive(void) { return lbl_eu_80664BE8 != 0; }
