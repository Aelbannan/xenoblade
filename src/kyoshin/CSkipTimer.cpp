#define IWORK_EVENT_INLINE_DTOR
// kyoshin/CSkipTimer - high-level C++ reconstruction.
// Functions are defined in retail .text order.

#include "kyoshin/CSkipTimer.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfTFile.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceFile.hpp"

#include <cstdio>
#include "kyoshin/CSkipTimerImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// ============================================================================
// CSkipTimer2
// ============================================================================

CSkipTimer2::CSkipTimer2(void* parent)
    : mParent(parent),
      mField18(nullptr),
      mAnimTransform(nullptr),
      mField20(0),
      mField21(0),
      mField22(0),
      mField23(1),
      mField24(0) {}

CSkipTimer2::~CSkipTimer2() {
    // mMemRegion is destroyed by its own destructor.
}

extern "C" void CSkipTimer2RebuildLayout(CSkipTimer2* self) {
    self->mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x10000,
                                  lbl_eu_80510568, 0);
    Class_8045F858 regionHost(&self->mMemRegion);
    buildLayout(reinterpret_cast<nw4r::lyt::Layout**>(&self->mField18),
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mParent),
                  &lbl_eu_80510568[0xc]);
    bindLayoutAnimTransform(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18),
                  &self->mAnimTransform,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mParent),
                  &lbl_eu_80510568[0x25]);
    CSkipTimer2RebindAnim(self);
    setLayoutTextBoxFont(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18),
                  &lbl_eu_80510568[0x41], (u32)CUICfManager_getPackedFontD8());
    nw4r::lyt::Pane* pane = reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                                ->GetRootPane()
                                ->FindPaneByName(&lbl_eu_80510568[0x41], true);
    setPaneVisible(pane, 0);
    CSkipTimer2MarkActive(self);
    self->mMemRegion.validateHeap();
}

extern "C" void CSkipTimer2DriveState(CSkipTimer2* self) {
    if (self->mField20 == 0) return;
    switch (self->mField21) {
    case 1: CSkipTimer2OnFwdDone(self); break;
    case 3: CSkipTimer2OnRewindDone(self); break;
    case 4: func_8029F6EC(self); break;
    case 5: CSkipTimer2OnAdvanceEnd(self); break;
    }
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->Animate(0);
}

extern "C" void CSkipTimer2Reset(CSkipTimer2* self) {
    self->mField20 = 0;
    if (self->mField18 != 0) {
        if (self->mField18 != 0) {
            self->mField18->vf2(1);
        }
        self->mField18 = 0;
    }
    self->mMemRegion.deleteRegion();
}

extern "C" void CSkipTimer2Start(CSkipTimer2* self, u8 arg) {
    if (self->mField21 != 0) return;
    self->mField21 = 1;
    self->mField24 = arg;
    self->mField23 = 0;
    nw4r::lyt::Layout* layout = reinterpret_cast<nw4r::lyt::Layout*>(self->mField18);
    nw4r::lyt::Pane* pane = layout->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true);
    setPaneVisible(pane, 1);
    CSkipTimer2SelectSlotPane(self, self->mField24);
}

extern "C" void CSkipTimer2HoldRelease(CSkipTimer2* self, u8 arg) {
    if (self->mField21 != 2) return;
    if (arg == self->mField24) return;
    self->mField21 = 4;
    self->mField23 = 0;
    self->mField24 = arg;
}

extern "C" void CSkipTimerStampSlotText(CSkipTimer2* self, int arg1, int arg2) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_80510568[0x4a], arg1, arg2);
    LayoutSetTextBoxText(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18),
                         &lbl_eu_80510568[0x41], buf, 0);
}

extern "C" void CSkipTimer2OnFwdDone(CSkipTimer2* self) {
    if (advanceAnimTransform(self->mAnimTransform, 1.0f) != 0) {
        self->mField21 = 2;
        self->mField23 = 1;
    }
    nw4r::lyt::Pane* pan52 =
        reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
            ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x52], true);
    reinterpret_cast<PaneAlphaB8*>(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                                       ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true))
        ->field_0xB8 =
        reinterpret_cast<PaneAlphaB8*>(pan52)->field_0xB8;
}

extern "C" void CSkipTimer2OnRewindDone(CSkipTimer2* self) {
    if (AnimRewindFrame(self->mAnimTransform, 1.0f) != 0) {
        self->mField21 = 0;
        self->mField23 = 1;
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true),
                      0);
    }
    nw4r::lyt::Pane* pan52 =
        reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
            ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x52], true);
    reinterpret_cast<PaneAlphaB8*>(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                                       ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x41], true))
        ->field_0xB8 =
        reinterpret_cast<PaneAlphaB8*>(pan52)->field_0xB8;
}

extern "C" void func_8029F6EC(CSkipTimer2* self) {
    if (AnimRewindFrame(self->mAnimTransform, 1.0f) != 0) {
        self->mField21 = 5;
        CSkipTimer2SelectSlotPane(self, self->mField24);
    }
}

extern "C" void CSkipTimer2OnAdvanceEnd(CSkipTimer2* self) {
    if (advanceAnimTransform(self->mAnimTransform, 1.0f) != 0) {
        self->mField21 = 2;
        self->mField23 = 1;
    }
}

extern "C" void CSkipTimer2MarkActive(CSkipTimer2* self) {
    if (self->mParent != nullptr) {
        self->mField22 = 1;
        self->mField20 = 1;
    }
}

extern "C" void CSkipTimer2RebindAnim(CSkipTimer2* self) {
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->UnbindAllAnimation();
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->BindAnimation(self->mAnimTransform);
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->SetAnimationEnable(self->mAnimTransform, true);
    reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)->Animate(0);
}

extern "C" void CSkipTimer2SelectSlotPane(CSkipTimer2* self, u8 arg) {
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x5a], true), 0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x63], true), 0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x6c], true), 0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x75], true), 0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x7e], true), 0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x87], true), 0);
    setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                      ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x90], true), 0);
    switch (arg) {
    case 1:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x5a], true), 1);
        break;
    case 2:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x63], true), 1);
        break;
    case 3:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x6c], true), 1);
        break;
    case 4:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x75], true), 1);
        break;
    case 5:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x7e], true), 1);
        break;
    case 6:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x87], true), 1);
        break;
    case 7:
        setPaneVisible(reinterpret_cast<nw4r::lyt::Layout*>(self->mField18)
                          ->GetRootPane()->FindPaneByName(&lbl_eu_80510568[0x90], true), 1);
        break;
    }
}

// ============================================================================
// CSkipTimer
// ============================================================================

CSkipTimer::CSkipTimer()
    : mFileHandle(nullptr),
      mLayout(nullptr),
      mLayout2(nullptr),
      mAnimTransform20(nullptr),
      mAnimTransform24(nullptr),
      mField28(0),
      mField29(0),
      mField2A(0),
      mField2B(1),
      mField2C(0),
      mField2E(0),
      mActive(0),
      mSysWin(0),
      mSkipTimer2(nullptr) {}

CSkipTimer::~CSkipTimer() {}

extern "C" void CSkipTimer_BeginLoad(CSkipTimer* self) {
    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), &lbl_eu_80510568[0x99],
        reinterpret_cast<IWorkEvent*>(self), 0, 0);
    self->mSysWin = CSysWin(0);
    self->mSysWin.loadSystemArc();
    self->mField2A = 0;
}

extern "C" void CSkipTimerTick(CSkipTimer* self) {
    if (self->mField28 == 0) return;
    switch (self->mField29) {
    case 1:
        self->OnFwdGate();
        break;
    case 2:
        self->ReengageInput();
        break;
    case 3: {
        int action = CSkipTimerMapKeyToAction(self);
        CSkipTimer2HoldRelease(&self->mSkipTimer2, static_cast<u8>(action));
        break;
    }
    case 4:
        self->EndSkip();
        break;
    case 5:
        self->OnReverseDone();
        break;
    case 6:
        self->ActivateSkipUI();
        break;
    case 7:
        self->LockSkipButton();
        break;
    }
    self->mLayout2->Animate(0);
    CSkipTimer2DriveState(&self->mSkipTimer2);
    reinterpret_cast<CSysWin*>(&self->mSysWin)->dispatchPhase();
}

extern "C" void CSkipTimerDraw(CSkipTimer* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField28 == 0) return;
    drawLayout(self->mLayout2, drawInfo, 0, 1);
    if (self->mSkipTimer2.mField20 != 0) {
        drawLayout(reinterpret_cast<nw4r::lyt::Layout*>(self->mSkipTimer2.mField18),
                   drawInfo, 0, 1);
    }
    reinterpret_cast<CSysWin*>(&self->mSysWin)->drawLayout(
        reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
}

extern "C" void CSkipTimerTeardown(CSkipTimer* self) {
    closeFileHandle(&self->mFileHandle);
    nw4r::lyt::Layout* layout = self->mLayout2;
    self->mField28 = 0;
    if (layout != 0) {
        delete layout;
        self->mLayout2 = 0;
    }
    CSkipTimer2Reset(&self->mSkipTimer2);
    releaseArcResourceAccessor(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mLayout));
    self->mLayout = 0;
    reinterpret_cast<CSysWin*>(&self->mSysWin)->termLayout();
    self->mMemRegion.deleteRegion();
}

extern "C" int CSkipTimerIsReady(CSkipTimer* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWin)->isReady() != 0) {
        return self->mField2A;
    }
    return 0;
}

extern "C" int CSkipTimerGetSkipButton(CSkipTimer* self) {
    CSkipTimer2* sub = &self->mSkipTimer2;
    if (sub->mField23 != 0) {
        return self->mField2B;
    }
    return 0;
}

extern "C" int isSkipActive(CSkipTimer* self) {
    return self->mActive;
}

extern "C" void CSkipTimerNextKey(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWin)->isOpened() != 0) return;
    s16 t = self->mField2C + 1;
    self->mField2C = t;
    if (t > 0x17) self->mField2C = 0;
    CSkipTimer_UpdateSlotFrame(self);
    playUISound(0x22);
}

extern "C" void CSkipTimerPrevKey(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWin)->isOpened() != 0) return;
    s16 t = self->mField2C - 1;
    self->mField2C = t;
    if (t < 0) self->mField2C = 0x17;
    CSkipTimer_UpdateSlotFrame(self);
    playUISound(0x22);
}

extern "C" void CSkipTimerFlagButton(CSkipTimer* self) {
    if (self->mField29 != 0) return;
    self->mField29 = 1;
    self->mField2B = 0;
}

extern "C" void CSkipTimerLeaveSkip(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    CSkipTimer2* sub = &self->mSkipTimer2;
    self->mField29 = 4;
    self->mField2B = 0;
    sub->mField21 = 3;
    sub->mField23 = 0;
    playUISound(6);
}

extern "C" void CSkipTimerConfirmSkip(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWin)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&self->mSysWin)->isActive() == 0) return;
        self->mField29 = 7;
        self->mField2B = 0;
        reinterpret_cast<CSysWin*>(&self->mSysWin)->advancePhase3();
        func_80086B5C__Q22cf13CfGameManagerFv((u32)(u16)self->mField2C,
                                              (u32)(u16)self->mField2E, 1);
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
        }
        playUISound(3);
    } else {
        self->mField29 = 6;
        self->mField2B = 0;
        char* msg = BdatTouchStringCell(&lbl_eu_80510568[0xaf], &lbl_eu_80510568[0xb8], 0x3a);
        reinterpret_cast<CSysWin*>(&self->mSysWin)->setKindContent(reinterpret_cast<char*>(msg), 0);
        reinterpret_cast<CSysWin*>(&self->mSysWin)->applyKind(1);
        reinterpret_cast<CSysWin*>(&self->mSysWin)->openPhase1();
        playUISound(3);
    }
}

extern "C" void CSkipTimerEngageSkip(CSkipTimer* self) {
    if (self->mField29 != 3) return;
    if (reinterpret_cast<CSysWin*>(&self->mSysWin)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&self->mSysWin)->isActive() == 0) return;
        self->mField29 = 7;
        self->mField2B = 0;
        reinterpret_cast<CSysWin*>(&self->mSysWin)->advancePhase3();
        func_80086B5C__Q22cf13CfGameManagerFv((u32)(u16)self->mField2C,
                                              (u32)(u16)self->mField2E, 1);
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
        }
        playUISound(3);
    } else {
        self->mActive = 1;
        playUISound(6);
    }
}

extern "C" int CSkipTimerIsEngaged(CSkipTimer* self) {
    if (reinterpret_cast<CSysWin*>(&self->mSysWin)->isOpened() != 0) {
        return 1;
    }
    return self->mActive;
}

void CSkipTimer::OnFwdGate() {
    if (advanceAnimTransform(this->mAnimTransform20, 1.0f) != 0) {
        this->mField29 = 2;
        CSkipTimerBindAnim24(this);
        int action = CSkipTimerMapKeyToAction(this);
        CSkipTimer2Start(&this->mSkipTimer2, static_cast<u8>(action));
    }
}

void CSkipTimer::ReengageInput() {
    CSkipTimer2* sub = &this->mSkipTimer2;
    if (sub->mField23 == 0) return;
    this->mField29 = 3;
    this->mField2B = 1;
}

void CSkipTimer::EndSkip() {
    CSkipTimer2* sub = &this->mSkipTimer2;
    if (sub->mField23 == 0) return;
    this->mField29 = 5;
    CSkipTimerBindAnim20(this);
}

void CSkipTimer::OnReverseDone() {
    if (AnimRewindFrame(this->mAnimTransform20, 1.0f) != 0) {
        this->mField29 = 0;
        this->mField2B = 1;
    }
}

void CSkipTimer::ActivateSkipUI() {
    if (reinterpret_cast<CSysWin*>(&this->mSysWin)->isActive() == 0) return;
    this->mField29 = 3;
    this->mField2B = 1;
}

void CSkipTimer::LockSkipButton() {
    if (reinterpret_cast<CSysWin*>(&this->mSysWin)->isActive() == 0) return;
    this->mField2B = 1;
    this->mActive = 1;
}

extern "C" void CSkipTimerActivateTimer(CSkipTimer* self) {
    if (self->mLayout == 0) return;
    CSkipTimerBindAnim24(self);
    self->mField2C = 0;
    self->mField2E = 0;
    CfT_PlayClockSnapshot(reinterpret_cast<u16*>(&self->mField2C), reinterpret_cast<u16*>(&self->mField2E));
    self->mField2E = 0;
    CSkipTimer_UpdateSlotFrame(self);
    CSkipTimerBindAnim20(self);
    self->mField2A = 1;
    self->mField28 = 1;
}

extern "C" void CSkipTimer_UpdateSlotFrame(CSkipTimer* self) {
    float cur;
    int total = (int)self->mField2C * 0x3c + (int)(s16)self->mField2E;
    cur = lbl_eu_80662C78 * (float)total + lbl_eu_80664A50;
    float max = (float)(u16)self->mAnimTransform24->GetFrameSize();
    if (cur > max) {
        cur -= max;
    }
    self->mAnimTransform24->SetFrame(cur);
    self->mLayout2->Animate(0);
    CSkipTimerStampSlotText(&self->mSkipTimer2, self->mField2C, (s16)self->mField2E);
}

extern "C" int CSkipTimerMapKeyToAction(CSkipTimer* self) {
    s16 key = self->mField2C;
    if (key < 5) return 7;
    if (key == 5) return 1;
    if (key < 10) return 2;
    if (key < 17) return 3;
    if (key == 17) return 4;
    if (key == 18) return 5;
    return 6;
}

extern "C" void CSkipTimerBindAnim20(CSkipTimer* self) {
    self->mLayout2->UnbindAllAnimation();
    self->mLayout2->BindAnimation(self->mAnimTransform20);
    self->mLayout2->SetAnimationEnable(self->mAnimTransform20, true);
    self->mLayout2->Animate(0);
}

extern "C" void CSkipTimerBindAnim24(CSkipTimer* self) {
    self->mLayout2->UnbindAllAnimation();
    self->mLayout2->BindAnimation(self->mAnimTransform24);
    self->mLayout2->SetAnimationEnable(self->mAnimTransform24, true);
    self->mLayout2->Animate(0);
}

bool CSkipTimer::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
        mMemRegion.createRegion(mem2, 0x10000, &lbl_eu_80510568[0xbd], 0);
        Class_8045F858 memHost(&mMemRegion);
        void* fileData = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);
        mLayout = reinterpret_cast<nw4r::lyt::Layout*>(CLibLayout::createArcResourceAccessor());
        reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout)
            ->Attach(fileData, &lbl_eu_80510568[0xc8]);
        buildLayout(&mLayout2, reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                    &lbl_eu_80510568[0xcc]);
        bindLayoutAnimTransform(mLayout2, &mAnimTransform20,
                    reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                    &lbl_eu_80510568[0xe5]);
        bindLayoutAnimTransform(mLayout2, &mAnimTransform24,
                    reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(mLayout),
                    &lbl_eu_80510568[0x101]);
        CSkipTimerBindAnim20(this);

        mSkipTimer2 = CSkipTimer2(mLayout);
        CSkipTimer2RebuildLayout(&mSkipTimer2);
        CSkipTimerActivateTimer(this);
        mFileHandle = 0;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}

extern "C" void sinit_802A07D8() {
    lbl_eu_80664A50 = lbl_eu_80668C48 * (lbl_eu_80668C4C * lbl_eu_80662C78);
}
