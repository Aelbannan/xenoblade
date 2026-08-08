#pragma once

#include <types.h>

#include "monolib/core/CScheduleItem.hpp"

// Schedule item pool (lbl_eu_80661718, 0x10 bytes in .bss).
struct CScheduleItemPool {
    CScheduleItem* base; // 0x0: item array (stride 0x58)
    s32 count;           // 0x4
    s32 freeCount;       // 0x8
    s32 lastHandle;      // 0xC
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" CScheduleItemPool lbl_eu_80661718;
extern "C" void func_804E3E2C(CScheduleItem* item);
