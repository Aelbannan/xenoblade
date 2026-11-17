#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/COption.hpp"
#include "kyoshin/CSysWinImports.hpp"

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
