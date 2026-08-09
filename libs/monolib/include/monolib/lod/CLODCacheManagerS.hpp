#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Pointee types proven by the co-located TU libs/monolib/src/lod/CLODCacheManagerS.cpp.
extern "C" u8*   lbl_eu_80665738;  // LOD cache records, stride 0xC
// LOD linear distance-table entry (stride 8) -- u32 element pairs
extern "C" u32*  lbl_eu_8066573C;
extern "C" u32*  lbl_eu_8066574C;  // index -> pair-table offset
// pair table, stride 2 (u16 entries)
extern "C" u16*  lbl_eu_80665750;
// LOD level-table entry (stride 0x10) used by the cubic lookup -- byte blob
extern "C" u8*   lbl_eu_80665740;
// LOD short-table entry (stride 8) backing the 2D lookup -- byte blob
extern "C" u8*   lbl_eu_80665744;
extern "C" u32*  lbl_eu_80665748;  // distance-key index table, stride 4
