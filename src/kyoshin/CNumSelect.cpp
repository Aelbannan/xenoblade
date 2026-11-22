#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CNumSelect.hpp"
#include <cstdio>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Layout/resource helpers used by this TU (code_80135FDC / CUICfManager).
extern "C" void writePanePos(nw4r::lyt::Pane*, float*);
// Retail symbol name is a placeholder guess (the call passes -1 in r3).
#include "kyoshin/cf/CfGameManager.hpp"

/* 'timg' texture record resolved through the shared arc accessor: +0x8 points
 * at a pointer to the {u16 width, u16 height} pair used to size the panes. */
struct CNumSelectTimgDims {
    u16 m00;
    u16 m02;
};

struct CNumSelectTimgRef {
    CNumSelectTimgDims* dims; // 0x0
};

struct CNumSelectTimg {
    u32 unk0;                // 0x0
    u32 unk4;                // 0x4
    CNumSelectTimgRef* unk8; // 0x8
};

#pragma push
#pragma optimize_for_size on
CNumSelect::CNumSelect(u8 region)
    : mFileHandle(NULL), mArcResAcc(NULL), mpLayout(NULL), mAnimIn(NULL),
      mAnimLoop(NULL), mAnimOn(NULL), mOpen(0), mLoaded(0), mIdle(1),
      mState(0), mRegion(region) {}

CNumSelect::~CNumSelect() {}
#pragma pop

void CNumSelect::LoadFiles() {
    mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(), "menu/jp/ShopSubWin.arc",
                                        this, 0, 0);
}

void CNumSelect::UpdateStates() {
    if (mOpen == 0) {
        return;
    }
    switch (mState) {
    case 1:
        UpdateOpen();
        break;
    case 2:
        advanceAnimTransform(mAnimLoop, 1.0f);
        break;
    case 3:
        UpdateClose();
        break;
    case 4:
        UpdateCountUp();
        break;
    case 5:
        UpdateCountDown();
        break;
    }
    mpLayout->Animate();
}

void CNumSelect::DrawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (mOpen == 0) {
        return;
    }
    drawLayout(mpLayout, drawInfo, 0, 1);
}

void CNumSelect::UnloadFiles() {
    closeFileHandle(&mFileHandle);
    mOpen = 0;
    if (mpLayout != NULL) {
        delete mpLayout;
        mpLayout = NULL;
    }
    releaseArcResourceAccessor(mArcResAcc);
    mArcResAcc = NULL;
    mMemRegion.deleteRegion();
}

int CNumSelect::IsLoaded() { return mLoaded; }

int CNumSelect::IsOpen() { return mOpen; }

int CNumSelect::IsIdle() { return mIdle; }

void CNumSelect::SetItemName(char* name) {
    LayoutSetTextBoxFmtValue(mpLayout, "txt_name", name, 0);
}

void CNumSelect::SetCount(u8 count) {
    setLayoutTextBoxNumber(mpLayout, "txt_value", count);
}

#pragma push
#pragma optimize_for_size on
void CNumSelect::SetTotal(int value) {
    char buf[0x1C];
    char* unit = BdatTouchStringCell("MNU_item", "name", 3);
    sprintf(buf, "%d%s", value, unit);
    LayoutSetTextBoxText(mpLayout, "txt_total", buf, 0);
}

void CNumSelect::Open() {
    u8 state = mState;
    if (state == 1 || state == 3) {
        return;
    }
    mState = 1;
    mIdle = 0;
    mOpen = 1;
    mpLayout->UnbindAllAnimation();
    mpLayout->BindAnimation(mAnimIn);
    mpLayout->SetAnimationEnable(mAnimIn, true);
    playUISound(0xd);
}
#pragma pop

void CNumSelect::Close() {
    u8 state = mState;
    if (state == 1 || state == 3) {
        return;
    }
    mState = 3;
    mIdle = 0;
    mpLayout->UnbindAllAnimation();
    mpLayout->BindAnimation(mAnimIn);
    mpLayout->SetAnimationEnable(mAnimIn, true);
    playUISound(0xe);
}

void CNumSelect::CountUp() {
    u8 state = mState;
    if (state == 1 || state == 3) {
        return;
    }
    mpLayout->UnbindAllAnimation();
    mpLayout->BindAnimation(mAnimOn);
    mpLayout->SetAnimationEnable(mAnimOn, true);
    mAnimOn->SetFrame(0.0f);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("pic_arrow_o01", true), 1);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("pic_arrow_u01", true), 0);
    mState = 4;
}

void CNumSelect::CountDown() {
    u8 state = mState;
    if (state == 1 || state == 3) {
        return;
    }
    mpLayout->UnbindAllAnimation();
    mpLayout->BindAnimation(mAnimOn);
    mpLayout->SetAnimationEnable(mAnimOn, true);
    mAnimOn->SetFrame(0.0f);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("pic_arrow_o01", true), 0);
    setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("pic_arrow_u01", true), 1);
    mState = 5;
}

#pragma push
#pragma optimize_for_size on
void CNumSelect::SetArrowVisible(bool visible) {
    if (mpLayout != NULL) {
        setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("pic_up", true), visible);
        setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("pic_down", true), visible);
    }
}
#pragma pop

void CNumSelect::UpdateOpen() {
    if (advanceAnimTransform(mAnimIn, 1.0f)) {
        mState = 2;
        mIdle = 1;
        mpLayout->UnbindAllAnimation();
        mpLayout->BindAnimation(mAnimLoop);
        mpLayout->SetAnimationEnable(mAnimLoop, true);
    }
}

void CNumSelect::UpdateClose() {
    if (AnimRewindFrame(mAnimIn, 1.0f)) {
        mState = 0;
        mIdle = 1;
        mOpen = 0;
    }
}

void CNumSelect::UpdateCountUp() {
    if (advanceAnimTransform(mAnimOn, 1.0f) == 0) {
        return;
    }
    mAnimOn->SetFrame(0.0f);
    mpLayout->Animate();
    mpLayout->UnbindAllAnimation();
    mpLayout->BindAnimation(mAnimLoop);
    mpLayout->SetAnimationEnable(mAnimLoop, true);
    mAnimLoop->SetFrame(0.0f);
    mState = 2;
    mIdle = 1;
}

void CNumSelect::UpdateCountDown() {
    if (advanceAnimTransform(mAnimOn, 1.0f) == 0) {
        return;
    }
    mAnimOn->SetFrame(0.0f);
    mpLayout->Animate();
    mpLayout->UnbindAllAnimation();
    mpLayout->BindAnimation(mAnimLoop);
    mpLayout->SetAnimationEnable(mAnimLoop, true);
    mAnimLoop->SetFrame(0.0f);
    mState = 2;
    mIdle = 1;
}

#pragma push
#pragma optimize_for_size on
bool CNumSelect::OnFileEvent(CEventFile* evt) {
    if (mFileHandle == evt->mFileHandle) {
        mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x8000, "CNumSelect", 0);
        Class_8045F858 host(&mMemRegion);
        void* data = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);

        mArcResAcc = CLibLayout::createArcResourceAccessor();
        mArcResAcc->Attach(data, "arc");
        buildLayout(&mpLayout, mArcResAcc, "mf70_cf60_syswin05.brlyt");
        bindLayoutAnimTransform(mpLayout, &mAnimIn, mArcResAcc, "mf70_cf60_syswin05_in_value.brlan");
        bindLayoutAnimTransform(mpLayout, &mAnimLoop, mArcResAcc, "mf70_cf60_syswin05_roop.brlan");
        bindLayoutAnimTransform(mpLayout, &mAnimOn, mArcResAcc, "mf70_cf60_syswin05_on.brlan");

        nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)((IDeviceFontInfo*)CDeviceFont::getFontInfo(1, mpLayout))->getFont());
        setLayoutTextBoxFont(mpLayout, "txt_value", (u32)CUICfManager_getPackedFont9C());
        setLayoutTextBoxFont(mpLayout, "txt_total", (u32)CUICfManager_getPackedFont9C());

        mpLayout->UnbindAllAnimation();
        setPaneVisible(mpLayout->GetRootPane()->FindPaneByName("nul_shslct", true), 0);

        // The region flag selects the buy-caption row (11 vs 12).
        char* table = "MNU_shop";
        char* column = "name";
        u32 row = 11;
        if (mRegion == 1) {
            row = 12;
        }
        LayoutSetTextBoxFmtValue(mpLayout, "txt_buy01", BdatTouchStringCell(table, column, row), 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_buy02", BdatTouchStringCell("MNU_shop", "name", 13), 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_buy03", BdatTouchStringCell("MNU_shop", "name", 14), 0);

        char* closeText = BdatTouchStringCell("MNU_kyeassign", "help", 0x2b);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_close00", closeText, 0);
        LayoutSetTextBoxFmtValue(mpLayout, "txt_close01", closeText, 0);

        const char* key = (cf::CfGameManager::isClassicController(-1) != 0) ? "fileID_2" : "fileID_1";
        u16 fileId = BdatGetU16ByTableKey("MNU_kyeassign", key, 0x2b);
        char* tplName = MakeTplNameSysFile(fileId);
        CNumSelectTimg* timg =
            (CNumSelectTimg*)CUICfManager_getArcResourceAccessor()->GetResource(0x74696d67, tplName, 0);
        if (timg != NULL) {
            PaneSetTexPaletteByName(mpLayout, "pic_btn00", (TPLPalette*)timg);
            PaneSetTexPaletteByName(mpLayout, "pic_btn01", (TPLPalette*)timg);
            u16 h = timg->unk8->dims->m02;
            u16 w = timg->unk8->dims->m00;

            nw4r::lyt::Pane* paneA = mpLayout->GetRootPane()->FindPaneByName("pic_btn00", true);
            if (paneA != NULL) {
                f32 sz[2];
                sz[0] = static_cast<f32>(h);
                sz[1] = static_cast<f32>(w);
                writePanePos(paneA, sz);
            }
            nw4r::lyt::Pane* paneB = mpLayout->GetRootPane()->FindPaneByName("pic_btn01", true);
            if (paneB != NULL) {
                f32 sz[2];
                sz[0] = static_cast<f32>(h);
                sz[1] = static_cast<f32>(w);
                writePanePos(paneB, sz);
            }
        }

        if (mpLayout != NULL) {
            mLoaded = 1;
        }
        mFileHandle = NULL;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}
#pragma pop
