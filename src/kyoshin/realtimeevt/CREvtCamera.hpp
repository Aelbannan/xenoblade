#pragma once

#include <types.h>

// CREvtCamera — realtime event camera class
// Fields are partial; only offsets accessed by FULL_MATCH functions are documented.

struct CREvtCamera {
    u8 unk00[0x1C]; // unknown / padding
    u32 mField1C;    // offset 0x1C — accessed by func_801809B0 (FULL_MATCH)
};
