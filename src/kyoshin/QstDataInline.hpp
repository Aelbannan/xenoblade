#pragma once

#include "kyoshin/QstData.hpp"

// QstInfo's assignment is defined apart from the class in QstData.hpp:
// retail inlines it in the TUs that include this header
// (CMenuQstCnt, CMenuQuestLog) and emits it weak right after its first user
// QstData::Refresh, but CQstLogList's ctor calls it out of line.
inline QstInfo& QstInfo::operator=(const QstInfo& rhs) {
    mId = rhs.mId;
    mKind = rhs.mKind;
    mCategory = rhs.mCategory;
    mFlag = rhs.mFlag;
    mShow = rhs.mShow;
    mState = rhs.mState;
    mOrder = rhs.mOrder;
    return *this;
}
