#pragma once

#include <types.h>

#include "monolib/core/CScheduleItem.hpp"

// Schedule item pool (sScheduleItemPool, 0x10 bytes in .bss).
struct CScheduleItemPool {
    CScheduleItem* base; // 0x0: item array (stride 0x58)
    s32 count;           // 0x4
    s32 freeCount;       // 0x8
    s32 lastHandle;      // 0xC
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" CScheduleItemPool sScheduleItemPool;
// (schedReleaseItemSlots is now CScheduleItem::ReleaseSlots; stale decl retired.)

// Effect-node pool allocator defined in this TU. The effect startup code in
// code_804C8718.cpp includes this header instead of redeclaring it locally.
extern "C" void* SchedNode_AllocPool(u32 size, s32 count);
