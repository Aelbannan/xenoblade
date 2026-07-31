#pragma once

#include <types.h>

namespace cf {

// CREvtMem - realtime event memory manager (singleton)
// Vtable at 0x80530A30
// Size: 0x2C
// Singleton pointer at lbl_eu_80664260
struct CREvtMem {
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u32 field_08;  // start1
    /* 0x0C */ u32 field_0C;  // size1
    /* 0x10 */ void* ptr1;    // MEM2 allocation 1
    /* 0x14 */ void* ptr2;    // MEM2 allocation 2
    /* 0x18 */ u32 arenaStart;
    /* 0x1C */ u32 arenaEnd;
    /* 0x20 */ u32 arenaSize;
    /* 0x24 */ u32 currentPos;
    /* 0x28 */ f32 someFloat;
};

} // namespace cf