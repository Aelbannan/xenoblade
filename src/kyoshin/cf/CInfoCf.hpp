#pragma once

/**
 * cf::CInfoCf — Information/settings singleton.
 *
 * One global instance is created at boot; the constructor stores the
 * instance pointer in the global lbl_eu_80664250 so any code in this TU
 * can reach it without passing ``this`` around.  Used by the menu / info
 * stack to query and toggle UI state flags.
 *
 * Field layout (MWCC big-endian 32-bit):
 *   0x00  void* vtable     — set to externally-defined lbl_eu_805309B0
 *   0x04  u32   mFlags     — bitfield of state flags
 *   0x08  u32   mField08   — cached setting value (set by func_801667AC)
 *   0x0C  u32   mField0C   — runtime value 1 (set by func_801667AC)
 *   0x10  u32   mField10   — runtime value 2 (set by func_801667AC)
 */

#include <types.h>

namespace cf {

struct CInfoCf {
    void* vtable;   // 0x00 — externally-defined vtable (lbl_eu_805309B0)
    u32 mFlags;     // 0x04
    u32 mField08;   // 0x08
    u32 mField0C;   // 0x0C
    u32 mField10;   // 0x10
};

} // namespace cf

