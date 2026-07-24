#pragma once

#include <types.h>

/* Quest log list widget. Owned by CMenuQuestLog.
   Sorting: mSortEnabled (0x179) gates sort/filter logic,
   mSortDescending (0x17A) flips the sort direction arrow. */
struct CQstLogList {
    u8 pad_0000[0x179];     // padding to field at offset 0x179
    u8 mSortEnabled;        // 0x179 - enables sorting/filtering (init=1)
    u8 mSortDescending;     // 0x17A - sort direction, 0=asc, 1=desc (init=0)
};
