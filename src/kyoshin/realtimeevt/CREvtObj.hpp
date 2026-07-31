#pragma once

#include <types.h>

namespace cf {

// CREvtObj - base class for realtime event objects
// Vtable at 0x80532320 (size 0x28 = 10 entries)
// Total size: 0x14
struct CREvtObj {
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 ptmf[3];  // __ptmf (12 bytes: 3 x u32)
};

} // namespace cf