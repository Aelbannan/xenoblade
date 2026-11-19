#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/COption.hpp"
#include "kyoshin/code_80296898.hpp"

// Flat retail imports for this TU. The two animation helpers have different
// linkage in the retail binary: advanceAnimTransform keeps its C++ mangling
// while AnimRewindFrame is a flat symbol.
int advanceAnimTransform(nw4r::lyt::AnimTransform*, f32);
extern "C" u32 AnimRewindFrame(nw4r::lyt::AnimTransform*, f32);
extern void playUISound(u32);

extern const f32 lbl_eu_80668C10;          // shared animation frame target
extern u8 lbl_eu_8053948C[0x30];           // option-cell maximum table
extern Class_80296898 lbl_eu_80577308;     // game configuration singleton

// Semantic names for the flat retail objects.
inline f32 optionAnimFrame() { return lbl_eu_80668C10; }
inline u8 optionCellMax(s32 index) { return lbl_eu_8053948C[index]; }
inline Class_80296898& optionConfig() { return lbl_eu_80577308; }

__declspec(noinline) CCur19::CCur19(nw4r::lyt::ArcResourceAccessor* accessor) : CBaseCur(accessor) {}

#pragma push
#pragma optimize_for_size on
CCur19::~CCur19() {}
#pragma pop

#pragma push
#pragma optimize_for_size on
COption::COption(u8 kind)
    : mFileHandle(nullptr), mArcAccessor(nullptr), mLayout(nullptr),
      mAnimTransA(nullptr), mAnimTransB(nullptr), field_0x28(0), field_0x29(0),
      field_0x2A(0), mLiveFlag(1), mRowTop(0), mRow(0), field_0x2E(0),
      field_0x2F(0), mConfirmGate(0), field_0x31(0), mSecondConfirm(0),
      field_0x33(0), mKind(kind), mScrollBar(0), mSysWin(0), field_0xFC(0),
      field_0x100(0) {}
#pragma pop

#pragma push
#pragma optimize_for_size on
COption::~COption() {}
#pragma pop

int COption::getLiveFlag() { return mLiveFlag; }
int COption::getConfirmGate() { return mConfirmGate; }
int COption::getSecondConfirm() { return mSecondConfirm; }

int COption::isWindowReady() {
    if (!mScrollBar.isVisible()) return 0;
    if (!mSysWin.isReady()) return 0;
    return field_0x2A;
}

void COption::confirmSelection() {
    if (field_0x29 == 3) {
        field_0x29 = 4;
        mLiveFlag = 0;
        mScrollBar.requestScrollOut();
        mCur78.SetVisible(0);
        mCur90.SetVisible(0);
        playUISound(6);
    }
}

int COption::getNavCode() {
    if (mSysWin.isOpened()) return 0;
    if (field_0x2E != 0) return 0x6D;
    return isConfigDirty() != 0 ? 0x6E : 0x6C;
}

u8 COption::readCurrentConfigCell() {
    return readConfigCell((u8)((u32)mRowTop + (u32)mRow));
}

u8 COption::getCellMax() { return optionCellMax(mRowTop + mRow); }

void COption::advanceState1To2() {
    if (advanceAnimTransform(mAnimTransA, optionAnimFrame())) {
        field_0x29 = 2;
        setupSubAnim24();
    }
}

void COption::advanceState2To3() {
    if (advanceAnimTransform(mAnimTransB, optionAnimFrame())) {
        field_0x29 = 3;
        mLiveFlag = 1;
        mCur78.SetVisible(1);
        mCur90.SetVisible(1);
        refreshCursorPanes();
    }
}

void COption::advanceState4To5() {
    if (AnimRewindFrame(mAnimTransB, optionAnimFrame())) {
        field_0x29 = 5;
        setupSubAnim20();
    }
}

void COption::advanceState5To0() {
    if (AnimRewindFrame(mAnimTransA, optionAnimFrame())) {
        field_0x29 = 0;
        mLiveFlag = 1;
    }
}

void COption::advanceState6To3() {
    if (mSysWin.isActive()) {
        field_0x29 = 3;
        mLiveFlag = 1;
        mCur78.SetVisible(0);
        mCurE4.SetVisible(1);
        refreshCursorPanes();
    }
}

void COption::advanceState8To9() {
    if (mSysWin.isActive()) {
        field_0x29 = 9;
        mLiveFlag = 1;
        mCur78.SetVisible(0);
        mCurE4.SetVisible(1);
        refreshCursorPanes();
    }
}

void COption::advanceState10To3() {
    if (mSysWin.isActive()) {
        field_0x29 = 3;
        mLiveFlag = 1;
        if ((s8)field_0x31 == 0) {
            optionConfig().init();
            refreshRows();
        }
    }
}

void COption::setupSubAnim20() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTransA);
    mLayout->SetAnimationEnable(mAnimTransA, true);
    mLayout->Animate(0);
}

void COption::setupSubAnim24() {
    mLayout->UnbindAllAnimation();
    mLayout->BindAnimation(mAnimTransB);
    mLayout->SetAnimationEnable(mAnimTransB, true);
    mLayout->Animate(0);
}

extern "C" void sinit_8029E7D8() { initInstance(&optionConfig()); }
