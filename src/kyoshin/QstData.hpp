#pragma once

#include <types.h>

/*
 * Quest-info table shared by CMenuQstCnt (which defines the out-of-line
 * QstData members) and CQstLogList (which embeds a QstData at +0x182).
 *
 * Retail evidence (CMenuQstCnt .text, first-use order):
 *   QstData::QstData()  0x80228DE4  __construct_array(mList, QstInfo ctor/dtor, 8, 0x400)
 *   QstInfo::QstInfo()  0x80228E44  weak, zeroes every field
 *   QstInfo::~QstInfo() 0x80228E68  weak, empty (only delete)
 *   QstData::~QstData() 0x80228EA8  weak, __destroy_arr(mList)
 *   QstInfo::operator=  0x8022905C  weak, implicit. Emitted out of line
 *                                   after its first user QstData::Refresh
 *                                   because -O4,s does not inline functions
 *                                   with several call sites; CMenuQuestLog
 *                                   (one site) inlines it.
 *   QstData::~QstData() is implicit (CQstLogList inlines it); no live
 *                       CMenuQstCnt function references it, so this TU does
 *                       not emit the retail copy at 0x80228EA8 (its retail
 *                       position suggests a dead-stripped user between the
 *                       QstData ctor and Refresh).
 *   QstInfo(QstInfo&)      0x80229498  weak, after QstData::Sort; non-const
 *                                         source -> loads are not hoisted
 *                                         above the stores (may alias)
 *   QstInfo(const QstInfo&)  0x802294D4  weak, const source -> scheduled
 */
class QstInfo {
public:
    QstInfo() {
        mId = 0;
        mKind = 0;
        mCategory = 0;
        mFlag = 0;
        mShow = 0;
        mState = 0;
        mOrder = 0;
    }
    QstInfo(u16 id, u8 kind, u8 category, u8 flag, u8 show, u8 state, u8 order) {
        mId = id;
        mKind = kind;
        mCategory = category;
        mFlag = flag;
        mShow = show;
        mState = state;
        mOrder = order;
    }
    QstInfo(QstInfo& rhs) {
        mId = rhs.mId;
        mKind = rhs.mKind;
        mCategory = rhs.mCategory;
        mFlag = rhs.mFlag;
        mShow = rhs.mShow;
        mState = rhs.mState;
        mOrder = rhs.mOrder;
    }
    QstInfo(const QstInfo& rhs) {
        mId = rhs.mId;
        mKind = rhs.mKind;
        mCategory = rhs.mCategory;
        mFlag = rhs.mFlag;
        mShow = rhs.mShow;
        mState = rhs.mState;
        mOrder = rhs.mOrder;
    }
    ~QstInfo() {}

    u16 mId;        // 0x0 quest id
    u8  mKind;      // 0x2 kind class (1 / 2 / 3)
    u8  mCategory;  // 0x3 category (1..0x1A)
    u8  mFlag;      // 0x4 raw progress flag
    u8  mShow;      // 0x5 show flag
    u8  mState;     // 0x6 BDAT state byte
    u8  mOrder;     // 0x7 category sort order
};

class QstData {
public:
    QstData();

    void Refresh();
    void Sort(u32 kind, u32 filter);
    u16 GetCount();
    QstInfo* GetInfo(u16 idx);
    u16 FindIndex(u16 id);

    QstInfo mList[0x400];   // 0x0000
    u16 mCount;             // 0x2000
    s16 mFilterCount;       // 0x2002 (-1 = no filter)
    u8  mSortKind;          // 0x2004
    u8  mSortFilter;        // 0x2005
};
