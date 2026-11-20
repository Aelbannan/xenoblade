// kyoshin/menu/code_80228DE4 - the quest-info table (QstData / QstInfo members).
//
// MWCC emits a TU's generated code - the static initialiser and the MI
// adjusting thunks - at the END of its .text, so retail's CMenuQstCnt .text
// stops right after [@108@__dt__ / @112@cbRenderBefore / @112@__dt__] at
// 0x80228DE4. The QstData out-of-line members that follow (0x80228DE4 ..
// 0x80229584) are this TU; QstInfo's ctors and operator= are weak
// out-of-line copies that come with them.
//
// The unit is data-free: QstData::Refresh reads the shared string pool at
// lbl_eu_80509AB4 (owned by the next unit) and the quest tables at
// lbl_eu_80573D18 / lbl_eu_806640A8, all external. CMenuQstCnt keeps
// .rodata 0x80509A00..0x80509AA8, .data 0x80536128..0x80536228,
// .sdata/.sdata2/.sbss, and the next unit starts at 0x80509AA8.

#include <types.h>

#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/menu/CMenuQstCnt.hpp"
#include "kyoshin/menu/CMenuTutorial.hpp"

QstData::QstData() {
    mCount = 0;
    mFilterCount = 0;
    mSortKind = 0xff;
    mSortFilter = 0xff;
}

// Rebuild the 0x400-entry quest-info table from the game's quest catalog.
// Categories 1..0x1A (ranges from GetU16PairEntry) each contribute their quests;
// entries whose kind is 0 / 0xC8 are skipped.
void QstData::Refresh() {
    mCount = 0;
    u16 cat;
    for (cat = 1; cat < 0x1B; cat++) {
        u32 start = GetU16PairEntry(cat);
        if ((start & 0xffff) == 0) continue;
        u16 end = GetU16PairEntry(cat + 1);
        u16 i = (u16)start;
        for (; i < end; i++) {
            u8 kind = (u8)CtrlRemote_TouchBitByArg(i + 0x220);
            if (kind == 0 || kind == 0xC8) continue;
            int kindClass;
            if (kind >= 0xFE) {
                kindClass = 3;
            } else if (kind == 1) {
                kindClass = 1;
            } else {
                kindClass = 2;
            }
            u32 v = SearchThresholdIdx(i);
            u32 tbl = lbl_eu_80573D18[v];
            u32 res = BdatGetU8Direct((const void*)tbl, &lbl_eu_80509AB4[0], i);
            if ((res & 0xff) == 2) continue;
            u32 showFlag = (res & 0xff) == 1;
            if (kindClass == 3) showFlag = 0;
            u8 x = (u8)BdatGetU8Direct((const void*)lbl_eu_806640A8, &lbl_eu_80509AB4[0xa], cat);
            mList[mCount++] = QstInfo(i, kindClass, cat, kind, showFlag, res, x);
        }
    }
}

void QstData::Sort(u32 kind, u32 filter) {
    // Select/re-select the sort column. kind==4 uses the quest-level filter
    // (mSortFilter); otherwise kind is the column (mSortKind). If the selection
    // didn't change, there is nothing to re-sort.
    if (kind == 4) {
        if (mSortFilter == filter) return;
        mSortKind = kind;
        mSortFilter = filter;
    } else {
        if (mSortKind == kind) return;
        mSortKind = kind;
        mSortFilter = 0;
    }
    mFilterCount = -1;   // "unsorted" marker until the count is recomputed

    // Pass 1: bubble sort quest entries by quest id ascending.
    for (u16 i = 0; i < mCount - 1; i++) {
        bool changed = true;
        for (u16 j = 0; j < mCount - 1 - i; j++) {
            QstInfo* pa = &mList[j];
            QstInfo* pb = &mList[j + 1];
            if (pa->mId > pb->mId) {
                const QstInfo tmp(*pa);
                *pa = QstInfo(*pb);
                *pb = QstInfo(tmp);
                changed = false;
            }
        }
        if (changed) break;
    }

    // Pass 2: bubble sort by category order ascending.
    for (u16 i = 0; i < mCount - 1; i++) {
        bool changed = true;
        for (u16 j = 0; j < mCount - 1 - i; j++) {
            QstInfo* pa = &mList[j];
            QstInfo* pb = &mList[j + 1];
            if (pa->mOrder > pb->mOrder) {
                const QstInfo tmp(*pa);
                *pa = QstInfo(*pb);
                *pb = QstInfo(tmp);
                changed = false;
            }
        }
        if (changed) break;
    }

    // Pass 3: bubble sort by show flag descending.
    for (u16 i = 0; i < mCount - 1; i++) {
        bool changed = true;
        for (u16 j = 0; j < mCount - 1 - i; j++) {
            QstInfo* pa = &mList[j];
            QstInfo* pb = &mList[j + 1];
            if (pa->mShow < pb->mShow) {
                const QstInfo tmp(*pa);
                *pa = QstInfo(*pb);
                *pb = QstInfo(tmp);
                changed = false;
            }
        }
        if (changed) break;
    }

    // When a column filter is active, bubble the matching entries to the front
    // (sort by match flag ascending, so matches land first), then count them.
    if (kind != 0) {
        for (u16 i = 0; i < mCount - 1; i++) {
            bool changed = true;
            for (u16 j = 0; j < mCount - 1 - i; j++) {
                QstInfo* pa = &mList[j];
                QstInfo* pb = &mList[j + 1];
                u8 m1 = (kind == 4) ? (pa->mCategory == filter)
                                    : (pa->mKind == kind);
                u8 m2 = (kind == 4) ? (pb->mCategory == filter)
                                    : (pb->mKind == kind);
                if (m1 < m2) {
                    const QstInfo tmp(*pa);
                    *pa = QstInfo(*pb);
                    *pb = QstInfo(tmp);
                    changed = false;
                }
            }
            if (changed) break;
        }

        mFilterCount = 0;
        for (u16 k = 0; k < mCount; k++) {
            u8 m = (kind == 4) ? (mList[k].mCategory == filter)
                               : (mList[k].mKind == kind);
            if (m == 0) break;
            mFilterCount++;
        }
    }
}

u16 QstData::GetCount() {
    short r = mFilterCount;
    // mFilterCount (signed) is negative -> use the u16 count instead. Keep the
    // temp as short so the single u16 conversion/clrlwi happens at the join.
    if (r < 0)
        r = (short)mCount;
    return (unsigned short)r;
}

QstInfo* QstData::GetInfo(u16 idx) {
    if (idx >= 0x400) return 0;
    return &mList[idx];
}

// Returns the index of the entry with the given quest id, or 0.
u16 QstData::FindIndex(u16 id) {
    for (u16 i = 0; i < mCount; i++) {
        if (mList[i].mId == id) return i;
    }
    return 0;
}

