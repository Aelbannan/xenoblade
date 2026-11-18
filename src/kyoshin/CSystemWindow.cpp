// CSystemWindow - system window process (singleton).
//
// Matching session for unit kyoshin/CSystemWindow.

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CSystemWindow.hpp"
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

// Retail declares the constructor as a C-ABI global `__ct__CSystemWindow` (the
// split symbol carries no class-length mangling), so it is kept as a plain
// global function rather than a member ctor.
//
// Runs CProcess's ctor then fills the IUIWindow base region by hand (base ctor
// is out-of-line in retail, its effects are emitted inline here): the vtable at
// +0x10 is written twice (temp IUIWindow vtable, then the composite vtable),
// the two __ptmf_null callback slots are copied, then the scalar fields and the
// embedded CSysWin instance are constructed and the two text strings copied.
extern "C" CSystemWindow* __ct__CSystemWindow(CSystemWindow* self, CScn* scene,
                                              u32 opt, const char* str1,
                                              const char* str2) {
    __ct__8CProcessFv((CProcess*)self);

    self->mVptr = (u32)lbl_eu_8052D238;

    // Composite vtable base and null-ptmf source. Post-increment derefs of a
    // local pointer fold the first access into lwzu; reusing one const src
    // pointer across both callback slots matches retail's reload schedule
    // (same shape as the matched __ct__CTalkWindow).
    char* vt = lbl_eu_8052D378;
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 workEvt = (u32)(vt + 0x24);
    u32 w1 = *src++;
    self->ptmf0[1] = w1;
    u32 scnRender = (u32)(vt + 0xac);
    self->ptmf0[0] = w0;
    u32 w2 = *src++;
    self->ptmf0[2] = w2;

    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    self->ptmf1[1] = w0;
    self->ptmf1[0] = w1;
    w2 = *src++;
    self->ptmf1[2] = w2;

    self->field_54 = 0;
    self->field_58 = 0;
    self->field_5C = 0;
    self->field_60 = -1;
    self->field_64 = 0;
    self->field_65 = 0;
    self->field_66 = 0;
    self->field_67 = 1;
    self->field_68 = 0;

    self->mVptr = (u32)vt;
    self->mWorkEvent = workEvt;
    self->mScnRender = scnRender;
    self->mScene = scene;

    __ct__7CSysWinFUc(&self->mSysWin[0], 0);

    // Default window state + optional flags, then copy the two title strings
    // (empty-string cases zero the first byte to match strcpy(null-copy).
    self->mState = 0;
    self->field_B5 = opt;
    self->mFlag2B6 = 0;
    if (str1)
        strcpy(&self->mStr1[0], str1);
    else
        self->mStr1[0] = 0;
    if (str2)
        strcpy(&self->mStr2[0], str2);
    else
        self->mStr2[0] = 0;

    return self;
}

CSystemWindow::~CSystemWindow() {
    // Destroy the embedded CSysWin storage (C-linkage dtor; -1 = delete scalar).
    __dt__7CSysWinFv(&mSysWin[0], -1);
    // Destroy the CProcess base at +0. The nested double null-check is an
    // MWCC artifact of D2-inlined-into-D1 (same shape as ~CSysWinSave).
    if (this != 0) {
        if (this != 0) {
            __dt__8CProcessFv((CProcess*)this, 0);
        }
    }
}

void CSystemWindow::Init() {
    u32 w = lbl_eu_80663E24;
    mFlag2B6 = (w >> 30) & 1;
    if (mFlag2B6 == 0)
        setPresentationFlag__Q22cf13CfGameManagerFv(true);

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xd, 0);

    // Rebuild the embedded CSysWin storage: construct a stack temp, memberwise
    // copy every body field except the vtable word (+0) and the 0x29-0x2B pad
    // (the embedded vtable set by the ctor is retained), destroy the temp, then
    // dispatch the layout-build virtual at vtable+0x88.
    u8 temp[0x3A];
    __ct__7CSysWinFUc(temp, field_B5);
    CSysWinDataBuff* sw = reinterpret_cast<CSysWinDataBuff*>(&mSysWin[0]);
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
    reinterpret_cast<CSysWinView*>(&mSysWin[0])->v20();
}

void CSystemWindow::Term() {
    CDeviceVI::waitForDrawDone();
    reinterpret_cast<CSysWin*>(&mSysWin[0])->termLayout();

    lbl_eu_80663FD8 = 0;

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);

    if (mFlag2B6 == 0)
        setPresentationFlag__Q22cf13CfGameManagerFv(false);
}

void CSystemWindow::Move() {
    // Bail out if the task is busy or the global render flag is set.
    // (single OR so MWCC emits short-circuit branches: A -> bne exit,
    //  B -> beq continue / b exit)
    if (CTaskGame::getInstance()->isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    switch (mState) {
    case 0: {
        // Window just became ready: load strings and start animation.
        if (reinterpret_cast<CSysWin*>(&mSysWin[0])->isReady()) {
            mState = 1;
            reinterpret_cast<CSysWin*>(&mSysWin[0])->setKindContent(reinterpret_cast<char*>(&mStr1[0]), reinterpret_cast<char*>(&mStr2[0]));
            reinterpret_cast<CSysWin*>(&mSysWin[0])->applyKind(1);
            reinterpret_cast<CSysWin*>(&mSysWin[0])->openPhase1();
        }
        break;
    }
    case 1:
        // Wait for the system window to become active.
        if (reinterpret_cast<CSysWin*>(&mSysWin[0])->isActive()) {
            mState = 2;
        }
        break;
    case 2: {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        int confirmBtn;
        // In co-op, the player-specific button (Classic A); otherwise Wiimote A.
        if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0)
            confirmBtn = (pad->mPressedButtonFlags >> 21) & 1;
        else
            confirmBtn = (pad->mPressedButtonFlags >> 4) & 1;
        if (confirmBtn != 0) {
            mState = 3;
            reinterpret_cast<CSysWin*>(&mSysWin[0])->advancePhase3();
            playUISound(3);
        }
        break;
    }
    case 3:
        // Closing transition finished - hand control back to the UI.
        if (reinterpret_cast<CSysWin*>(&mSysWin[0])->isActive()) {
            field_67 = 0;
            field_64 = 1;
        }
        break;
    }

    reinterpret_cast<CSysWin*>(&mSysWin[0])->dispatchPhase();
}


void CSystemWindow::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    reinterpret_cast<CSysWin*>(&mSysWin[0])->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(&drawInfo));
}

// Creates the singleton CSystemWindow on the work heap and registers it as a
// CProcess under `parent`. Returns 0 if the singleton already exists.
extern "C" CSystemWindow* SysWinCreateSingleton(CProcess* parent, CScn* scene, u32 opt,
                             const char* str1, const char* str2) {
    if (lbl_eu_80663FD8 != 0) return 0;

    u32 workMem = CWorkThreadSystem::getWorkMem();
    CSystemWindow* obj =
        (CSystemWindow*)mtl::MemManager::allocate(0x2b8, workMem);
    if (obj != 0) {
        obj = __ct__CSystemWindow(obj, scene, opt, str1, str2);
    }
    lbl_eu_80663FD8 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, 0);
    return lbl_eu_80663FD8;
}

/* Returns the singleton CSystemWindow instance (lbl_eu_80663FD8 in .sbss). */
CSystemWindow* SysWinGetSingleton() {
    return lbl_eu_80663FD8;
}