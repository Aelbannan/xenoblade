// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTutorialList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuTutorialList.hpp"
#include "kyoshin/CTutorial.hpp"       // func_80138078 (retail func_80138078__FUl)
#include "kyoshin/code_80135FDC.hpp"   // func_80136190 string-format helper
#include "kyoshin/CTaskGame.hpp"       // CTaskGame::getInstance / func_800426F0
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
 * factory (func_802AC494) emits a real bl to it, and returns `this` in r3
 * like a real constructor (retail relies on it). */
extern "C" __declspec(noinline) CMenuTutorialList* __ct__CMenuTutorialList(
    CMenuTutorialList* self, CProcess* parent, u32 arg2) {
    __ct__8CProcessFv((CProcess*)self);

    // vtable fixups: temp (CProcess) vtable first, then the null PMF
    // callback slots, then the composite vtable + the IScnRender sub-vtable
    // at +0x58 (same scheme as CMenuCollepedia's ctor).
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    self->ptmf0[0] = __ptmf_null[0];
    self->ptmf0[1] = __ptmf_null[1];
    self->ptmf0[2] = __ptmf_null[2];
    self->ptmf1[0] = __ptmf_null[0];
    self->ptmf1[1] = __ptmf_null[1];
    self->ptmf1[2] = __ptmf_null[2];
    self->mField54 = 0;
    self->mField55 = 0;
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

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_802AD1F4(reinterpret_cast<CTutorialList*>(mTutorialList));
    func_8029ABD8(reinterpret_cast<CTutorial*>(mTutorial));

    lbl_eu_80664BE8 = 0;

    func_8013B980();
    if (code80135FDC_getByte_64080() == 0) {
        func_8008294C__Q22cf13CfGameManagerFv(0);
    }
}

void CMenuTutorialList::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
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
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        close = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        close = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (close != 0) {
        if (func_800FEDF8() != 0) {
            func_800FF914();
        }
        func_80138078(6);
        mFlag = 4;
        mField54 = 1;
    }

    // Phase state machine (jumptable dispatch on the phase byte).
    switch (mFlag) {
    case 0:
        func_802AC520(this);
        break;
    case 1:
        func_802AC598(this);
        break;
    case 2:
        func_802AC5E8(this);
        break;
    case 3:
        func_802AC8A4(this);
        break;
    case 4:
        func_802AC8F4(this);
        break;
    case 5:
        func_802AC970(this);
        break;
    case 6:
        func_802ACA38(this);
        break;
    case 7:
        func_802ACB50(this);
        break;
    }

    func_801C3D54(&mBgTex);
    func_801C3FF0(&mTitleAHelp);
    func_802AD0E0(reinterpret_cast<CTutorialList*>(mTutorialList));
    func_8029AB28(reinterpret_cast<CTutorial*>(mTutorial));
}

void CMenuTutorialList::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto exit;
    }
    // Branch-over-branch guard: the exit label before the body label keeps
    // MWCC from folding the bit test to a single `bne` -- it emits retail's
    // `beq body; b exit` (see CMenuPause::Move; docs/MWCC_REFERENCE.md).
    if (!(lbl_eu_80663E28 & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    if (func_8013BE50() == 0) {
        goto exit;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    func_801C3D7C(&mBgTex, &drawInfo);
    func_802AD188(reinterpret_cast<CTutorialList*>(mTutorialList), &drawInfo);
    func_8029ABB8(reinterpret_cast<CTutorial*>(mTutorial), &drawInfo);
    func_801C4080(&mTitleAHelp, &drawInfo);
}

/* Factory: lazily allocate + construct the single tutorial-list menu instance
 * and register it as a child of `self`. Returns the stored instance (or 0 if
 * it already exists). */
extern "C" CMenuTutorialList* func_802AC494(CProcess* self, CProcess* parent, u32 arg2) {
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
void func_802AC520(CMenuTutorialList* self) {
    if (func_801C3E34(&self->mBgTex) != 0) {
        if (func_801C4114(&self->mTitleAHelp) != 0) {
            if (func_802AD2A4((CTutorialList*)self->mTutorialList) != 0) {
                func_801C412C(&self->mTitleAHelp);
                func_802AD308((CTutorialList*)self->mTutorialList);
                self->mFlag = 1;
                func_80138078(0x6d);
            }
        }
    }
}

// Phase 1 -> 2 (advance): once the title bar is idle and the list data is
// ready, move to the next phase.
void func_802AC598(CMenuTutorialList* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (func_802AD300((CTutorialList*)self->mTutorialList) != 0) {
            self->mFlag = 2;
        }
    }
}

void func_802AC5E8(CMenuTutorialList* self) {}

// Close: same idle+ready guard as func_802AC598, but marks the closing state
// byte at 0x54 instead of advancing the phase byte.
void func_802AC8A4(CMenuTutorialList* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (func_802AD300((CTutorialList*)self->mTutorialList) != 0) {
            self->mField54 = 1;
        }
    }
}

// Phase handler (retail func_802AC8F4): once the tutorial data is loaded,
// mark phase 5, play the title/help intro, label the current page via the
// list's page counter, then run the tutorial open sequence.
void func_802AC8F4(CMenuTutorialList* self) {
    if (func_8029ACAC(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
        self->mFlag = 5;
        func_801C4760(&self->mTitleAHelp);
        func_801C41C0(&self->mTitleAHelp,
            func_80136190(lbl_eu_80510B44 + 0xe, lbl_eu_80510B44 + 0x17,
                func_802AD838(reinterpret_cast<CTutorialList*>(self->mTutorialList))));
        func_8029ACC4(reinterpret_cast<CTutorial*>(self->mTutorial));
    }
}

// Phase 5: once the tutorial data is ready, mark phase 6 and repaint the
// title/help labels for the current tutorial phase, then play the page-flip
// sound and advance the list page counter.
void func_802AC970(CMenuTutorialList* self) {
    if (func_8029ACB4(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
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
        func_8009D018(0x33bf + func_802AD838(reinterpret_cast<CTutorialList*>(self->mTutorialList)), 1);
        func_802AD854(reinterpret_cast<CTutorialList*>(self->mTutorialList));
    }
}

// Phase 6: react to confirm/cancel input on the tutorial widget, then repaint
// the title/help labels according to the widget's current phase.
void func_802ACA38(CMenuTutorialList* self) {
    if (func_8029ACAC(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 first;
        u32 second;
        if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
            first = (pad->mPressedButtonFlags >> 21) & 1;
            second = (pad->mPressedButtonFlags >> 22) & 1;
        } else {
            first = (pad->mPressedButtonFlags >> 4) & 1;
            second = (pad->mPressedButtonFlags >> 5) & 1;
        }
        if (first) {
            func_8029AD88(reinterpret_cast<CTutorial*>(self->mTutorial));
            if (func_8029ACBC(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
                func_8029ACEC(reinterpret_cast<CTutorial*>(self->mTutorial));
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

// Phase handler (retail func_802ACB50): once the tutorial list has finished
// its intro, mark phase 2, repaint the title/help labels, start the tutorial
// and set the help-bar display mode.
void func_802ACB50(CMenuTutorialList* self) {
    if (func_8029ACB4(reinterpret_cast<CTutorial*>(self->mTutorial)) != 0) {
        self->mFlag = 2;
        func_801C4744(&self->mTitleAHelp);
        func_801C41C0(&self->mTitleAHelp,
            func_80136190(lbl_eu_80510B44, lbl_eu_80510B44 + 0x9, 0x23));
        func_8029ABD8(reinterpret_cast<CTutorial*>(self->mTutorial));
        func_801C41E8(&self->mTitleAHelp, 0x73);
    }
}

// --- IScnRender vtable this-adjusting thunks ---
// These are IScnRender vtable trampolines.  When CMenuTutorialList registers
// its IScnRender subobject (member at offset +0x58) as a render callback, the
// scene dispatches with an IScnRender* pointing at the subobject. Each thunk
// adjusts 'this' back by -0x58 so the real CMenuTutorialList implementation
// receives the correct pointer.

void func_802ACBCC(IScnRender* sub) {
    cbRenderBefore__17CMenuTutorialListFv((CMenuTutorialList*)((char*)sub - 0x58));
}

void func_802ACBD4(IScnRender* sub) {
    // Cast to a one-arg pointer so the call emits only the this-adjustment
    // (subi r3, r3, 0x58; b __dt__...) and leaves r4 as caller leftover.
    ((void(*)(void*))__dt__17CMenuTutorialListFv)((CMenuTutorialList*)((char*)sub - 0x58));
}

extern "C" unsigned long func_802AC510(void) { return lbl_eu_80664BE8 != 0; }
