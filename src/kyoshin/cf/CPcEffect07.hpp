#pragma once

#include <types.h>

// PcEffect07 entries indexed by i*0x18 stride:
//   short at +0x14 is active flag for entry i
//   u8   at +0x1C is slot ID for entry i
// Three entries total (indices 0, 1, 2).
// Offset 0x58 = maxEffects count.

struct PcEffectData {
    u8 _00[0x14];
    s16 entry0_active;     // 0x14
    u8 _16[6];             // 0x16-0x1B
    u8 entry0_id;          // 0x1C
    u8 _1D[0x0F];          // 0x1D-0x2B
    s16 entry1_active;     // 0x2C
    u8 _2E[6];             // 0x2E-0x33
    u8 entry1_id;          // 0x34
    u8 _35[0x0F];          // 0x35-0x43
    s16 entry2_active;     // 0x44
    u8 _46[6];             // 0x46-0x4B
    u8 entry2_id;          // 0x4C
    u8 _4D[0x0B];          // 0x4D-0x57
    u8 maxEffects;          // 0x58
};
