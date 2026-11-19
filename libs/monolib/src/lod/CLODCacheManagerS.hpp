#pragma once

#include <types.h>

namespace LOD {

/**
 * LOD cache record.  Retail stride is 0xC bytes.  Shared with the
 * distance-table lookup functions (lerpDistEntry / catmullRomBin /
 * fetchSampleXY / findCutBinIdx).
 */
class CLODCacheManagerS {
public:
    u16 field_0x0;   // 0x00 near distance
    u16 field_0x2;   // 0x02 far distance
    u16 field_0x4;   // 0x04 record id (dispatch-table index)
    u16 field_0x6;   // 0x06 entry count
    u32 field_0x8;   // 0x08 table index

    void initLodTables();
    s32 findCutBinIdx();
    f32 lerpDistEntry();
    f32 catmullRomBin();
};

/**
 * LOD linear distance-table entry (sLodDistTable, stride 8).
 * near is a float distance (0x00); far is an unsigned cutoff (0x04).
 */
struct LODDistEntry {
    f32 near;   // 0x00
    u16 far;    // 0x04
};

/**
 * LOD level-table entry (sLodLevelTable, stride 0x10) used by the cubic
 * catmull-rom interpolator.  Four float sample slots (0x00/0x04/0x08) and an
 * unsigned far cutoff (0x0C).
 */
struct LODLevelEntry {
    f32 f00;    // 0x00
    f32 f04;    // 0x04
    f32 f08;    // 0x08
    u16 far;    // 0x0C
};

/**
 * LOD short-table entry (sLodShortTable, stride 8) backing the 2D
 * interpolator.  val is a signed sample, outX/outY are output words, and
 * far is an unsigned cutoff.
 */
struct LODShortEntry {
    s16 val;    // 0x00
    s16 outX;   // 0x02
    s16 outY;   // 0x04
    u16 far;    // 0x06
};

/**
 * LOD distance-key element (index table sLodCacheIndexTable, stride 4).
 * near is a signed distance (0x00), far an unsigned cutoff (0x02).
 */
struct LODCacheIndex {
    s16 field_0x0;   // 0x00 near distance
    u16 field_0x2;   // 0x02 far distance
};

/**
 * LOD terrain state manager.  Only the fields touched by the recovered
 * functions are declared; remainder is opaque.
 */
class UnkClass_8046368C {
public:
    u8  pad_00[0xC];
    f32 field_0xC;   // 0x0C
    u8  pad_10[0x1C - 0x10];
    f32 field_0x1C;  // 0x1C
};

// Live LOD record dispatch function pointer (set by selectCubicLookup/69C).
typedef f32 (*LODRecordFn)(CLODCacheManagerS* rec);

} // namespace LOD

// ---------------------------------------------------------------------------
// Shared sbss / sdata2 data (retail symbol names).
// ---------------------------------------------------------------------------

// Cache record / pair-table bases are declared once in the public owner
// header (monolib/lod/CLODCacheManagerS.hpp).


extern f32 sLodThresholdDist;     // LOD threshold distance
extern u16 sLodEntryKeyThreshold;     // per-entry distance key threshold
extern void (*sLodRecordDispatchTable[])(LOD::UnkClass_8046368C*);  // .data dispatch
