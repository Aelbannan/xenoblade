// CSystemWindow - system window processes (CSystemWindow + CSysWinSelect).
//
// Retail TU: CSystemWindow's code, then CSysWinSelect's, then the compiler's
// @108@/@112@ adjustor thunks for both classes in that order; the vtables,
// RTTI records and type-name strings are emitted in reverse (CSysWinSelect
// first) in .data/.rodata/.sdata.

#define IWORK_EVENT_INLINE_DTOR
#define CTTASK_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CSystemWindow.hpp"
#include "kyoshin/CSysWinSelect.hpp"
#include "kyoshin/CSysWinBuff.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/gx/GXPixel.h>
#include <string.h>

// Retail .sbss singleton slot owned by this TU (split 0x80663FD8-0x80663FDC).
// Tentative definition -> .sbss, zero-initialized like retail.
CSystemWindow* lbl_eu_80663FD8;

CSystemWindow::CSystemWindow(CScn* scene, u32 opt, const char* str1, const char* str2)
    : mScene(scene), mSysWin(0), mState(0), field_B5(opt), mFlag2B6(0) {
    if (str1)
        strcpy(mStr1, str1);
    else
        mStr1[0] = 0;
    if (str2)
        strcpy(mStr2, str2);
    else
        mStr2[0] = 0;
}

// Complete-object destructor: the compiler destroys the embedded CSysWin and
// the CProcess base (through the inlined CTTask<IUIWindow>/IUIWindow dtors),
// then the conditional operator delete.
CSystemWindow::~CSystemWindow() {}

void CSystemWindow::Init() {
    u32 w = gmGameFlags;
    mFlag2B6 = (w >> 30) & 1;
    if (mFlag2B6 == 0)
        setPresentationFlag__Q22cf13CfGameManagerFv(true);

    IScnRender* render = this;
    mScene->addRenderCB(render, 0xd, 0);

    // Rebuild the embedded CSysWin storage: construct a stack temp, memberwise
    // copy every body field except the vtable word (+0) and the 0x29-0x2B pad
    // (the embedded vtable set by the ctor is retained), destroy the temp, then
    // dispatch the layout-build virtual at vtable+0x88.
    u8 temp[0x3A];
    __ct__7CSysWinFUc(temp, field_B5);
    CSysWinDataBuff* sw = reinterpret_cast<CSysWinDataBuff*>(&mSysWin);
    CSysWinDataBuff* tw = reinterpret_cast<CSysWinDataBuff*>(temp);
    sw->f_04 = tw->f_04;
    sw->f_08 = tw->f_08;
    sw->f_0c = tw->f_0c;
    sw->f_10 = tw->f_10;
    sw->f_14 = tw->f_14;
    sw->f_18 = tw->f_18;
    sw->f_1c = tw->f_1c;
    sw->f_20 = tw->f_20;
    sw->f_24 = tw->f_24;
    sw->f_28 = tw->f_28;
    sw->f_2c = tw->f_2c;
    sw->f_30 = tw->f_30;
    sw->f_34 = tw->f_34;
    sw->f_35 = tw->f_35;
    sw->f_36 = tw->f_36;
    sw->f_37 = tw->f_37;
    sw->f_38 = tw->f_38;
    sw->f_39 = tw->f_39;
    __dt__7CSysWinFv(temp, -1);

    // Dispatch the layout-build virtual at vtable+0x88 on the rebuilt CSysWin.
    reinterpret_cast<CSysWinView*>(&mSysWin)->v20();
}

void CSystemWindow::Term() {
    CDeviceVI::waitForDrawDone();
    mSysWin.termLayout();

    lbl_eu_80663FD8 = 0;

    IScnRender* render = this;
    mScene->removeRenderCB(render);

    if (mFlag2B6 == 0)
        setPresentationFlag__Q22cf13CfGameManagerFv(false);
}

void CSystemWindow::Move() {
    // Bail out if the task is busy or the global render flag is set.
    // (single OR so MWCC emits short-circuit branches: A -> bne exit,
    //  B -> beq continue / b exit)
    if (CTaskGame::getInstance()->isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    switch (mState) {
    case 0: {
        // Window just became ready: load strings and start animation.
        if (mSysWin.isReady()) {
            mState = 1;
            mSysWin.setKindContent(reinterpret_cast<char*>(&mStr1[0]), reinterpret_cast<char*>(&mStr2[0]));
            mSysWin.applyKind(1);
            mSysWin.openPhase1();
        }
        break;
    }
    case 1:
        // Wait for the system window to become active.
        if (mSysWin.isActive()) {
            mState = 2;
        }
        break;
    case 2: {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        int confirmBtn;
        // In co-op, the player-specific button (Classic A); otherwise Wiimote A.
        if (cf::CfGameManager::isClassicController(-1) != 0)
            confirmBtn = (pad->mPressedButtonFlags >> 21) & 1;
        else
            confirmBtn = (pad->mPressedButtonFlags >> 4) & 1;
        if (confirmBtn != 0) {
            mState = 3;
            mSysWin.advancePhase3();
            playUISound(3);
        }
        break;
    }
    case 3:
        // Closing transition finished - hand control back to the UI.
        if (mSysWin.isActive()) {
            mField67 = 0;
            mField64 = 1;
        }
        break;
    }

    mSysWin.dispatchPhase();
}


void CSystemWindow::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    mSysWin.drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo));
}

// Creates the singleton CSystemWindow on the work heap and registers it as a
// CProcess under `parent`. Returns 0 if the singleton already exists.
extern "C" CSystemWindow* SysWinCreateSingleton(CProcess* parent, CScn* scene, u32 opt,
                             const char* str1, const char* str2) {
    if (lbl_eu_80663FD8 != 0) return 0;

    u32 workMem = CWorkThreadSystem::getWorkMem();
    CSystemWindow* obj =
        new (mtl::MemManager::allocate(0x2b8, workMem)) CSystemWindow(scene, opt, str1, str2);
    lbl_eu_80663FD8 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return lbl_eu_80663FD8;
}

/* Returns the singleton CSystemWindow instance (lbl_eu_80663FD8 in .sbss). */
CSystemWindow* SysWinGetSingleton() {
    return lbl_eu_80663FD8;
}

// __vt__13CSysWinSelect, its RTTI base list/record and the "CSysWinSelect"
// type-name string are compiler-emitted.
CSysWinSelect* lbl_eu_80663FDC = nullptr;

// CSysWinSelect::~CSysWinSelect (us-80282058-style D1/D0 dtor): the compiler
// destroys mCur18@0xB4 and the embedded CSysWin@0x78, then the CProcess base
// (through the inlined CTTask<IUIWindow>/IUIWindow dtors) and the conditional
// operator delete.
CSysWinSelect::~CSysWinSelect() {}

// CSysWinSelect::CSysWinSelect (us-8012565c)
// CProcess ctor, inlined CTTask<IUIWindow>/IUIWindow state + vptrs, the embedded
// CSysWin / CCur18, the three scalar fields, then the three optional strings.
CSysWinSelect::CSysWinSelect(void* a2, void* a3, void* a4, void* a5)
    : mScene((CScn*)a2), mSysWin(0), mCur18(NULL), mState(0), mCursorSel(1), field_24E(0) {
    if (a3 != 0) {
        strcpy(mStrA, (char*)a3);
    } else {
        mStrA[0] = 0;
    }
    if (a4 != 0) {
        strcpy(mStrB, (char*)a4);
    } else {
        mStrB[0] = 0;
    }
    if (a5 != 0) {
        strcpy(mStrC, (char*)a5);
    } else {
        mStrC[0] = 0;
    }
}

// SysWinSelectCreate (us-80125b4c)
// Allocate the CSysWinSelect singleton (0x250 bytes) from work memory, run its
// constructor, store it in the .sbss global, and register it as a CProcess
// child of `parent`. Returns 0 when the singleton already exists.
extern "C" CSysWinSelect* SysWinSelectCreate(CProcess* parent, void* a2, void* a3, void* a4,
                                        void* a5) {
    if (lbl_eu_80663FDC != 0)
        return 0;

    CSysWinSelect* obj = new (mtl::MemManager::allocate(0x250, CWorkThreadSystem::getWorkMem()))
        CSysWinSelect(a2, a3, a4, a5);
    lbl_eu_80663FDC = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return lbl_eu_80663FDC;
}

// SysWinSelectHandleInput (us-80125bd8)
// The state==2 input handling for the select dialog. Reads the current cf pad
// data; the confirm button flag is read from a different bit in co-op vs
// single-player, and the D-pad up/down bits select the cursor row (wrapping).
// The pad fields are read inside each mode branch - retail emits the loads in
// both arms, so keep them out of shared locals or MWCC hoists them above the
// branch and the function grows an extra saved register.
extern "C" void SysWinSelectHandleInput(CSysWinSelect* self) {
    CSysPadData* pad = (CSysPadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int confirm, up, down;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Co-op: D-pad rows read from the turbo-press flags, confirm from bit 21.
        up = (pad->field_104 & 0x8004) != 0;
        down = (pad->field_104 & 0x10008) != 0;
        confirm = (pad->field_04 >> 21) & 1;
    } else {
        // Single-player: confirm is bit 4.
        up = (pad->field_104 & 0x8004) != 0;
        down = (pad->field_104 & 0x10008) != 0;
        confirm = (pad->field_04 >> 4) & 1;
    }

    if (confirm != 0) {
        // Confirm: close the window and record the chosen row index.
        self->mState = 3;
        self->mSysWin.advancePhase3();
        self->mField60 = (s8)self->mCursorSel + 1;
        self->mCur18.SetVisible(0);
        playUISound(3);
    } else if (up != 0) {
        // Up: move the cursor up one row, wrapping below 0 to 1.
        self->mCursorSel = (u8)(self->mCursorSel - 1);
        if ((s8)self->mCursorSel < 0) self->mCursorSel = 1;
        nw4r::math::VEC3 tmp;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&tmp), &self->mSysWin, self->mCursorSel);
        self->mCur18.setRootPaneTranslate(&tmp);
        playUISound(1);
    } else if (down != 0) {
        // Down: move the cursor down one row, wrapping over 1 to 0.
        self->mCursorSel = (u8)(self->mCursorSel + 1);
        if ((s8)self->mCursorSel > 1) self->mCursorSel = 0;
        nw4r::math::VEC3 tmp;
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(&tmp), &self->mSysWin, self->mCursorSel);
        self->mCur18.setRootPaneTranslate(&tmp);
        playUISound(1);
    }
}

