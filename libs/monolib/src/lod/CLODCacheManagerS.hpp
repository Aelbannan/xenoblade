#pragma once

#include <types.h>

namespace LOD {

/**
 * LOD cache record.  Retail stride is 0xC bytes.  Shared with the
 * distance-table lookup functions (func_80463118 / func_8046323C /
 * func_8046339C / func_80463590).
 */
class CLODCacheManagerS {
public:
    u16 field_0x0;   // 0x00 near distance
    u16 field_0x2;   // 0x02 far distance
    u16 field_0x4;   // 0x04 record id (dispatch-table index)
    u16 field_0x6;   // 0x06 entry count
    u32 field_0x8;   // 0x08 table index

    void func_804630C0();
    s32 func_80463590();
};

/**
 * LOD distance-key element (index table lbl_eu_80665748, stride 4).
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

// Live LOD record dispatch function pointer (set by func_8046368C/69C).
typedef f32 (*LODRecordFn)(CLODCacheManagerS* rec);

} // namespace LOD

// ---------------------------------------------------------------------------
// Shared sbss / sdata2 data (retail symbol names).
// ---------------------------------------------------------------------------

// Cache record / pair-table bases (set up by func_804630C0).
extern LOD::CLODCacheManagerS* lbl_eu_80665738;  // cache records, stride 0xC
extern void* lbl_eu_8066573C;   // distance table, stride 8
extern u32* lbl_eu_8066574C;    // index -> pair-table offset
extern void* lbl_eu_80665750;   // pair table, stride 2 (u16 entries)
extern void* lbl_eu_80665740;   // level table, stride 0x10
extern void* lbl_eu_80665744;   // short table, stride 8
extern LOD::LODCacheIndex* lbl_eu_80665748;   // distance-key index table, stride 4

extern LOD::LODRecordFn lbl_eu_80665760;  // active record lookup fn
extern f32 lbl_eu_80665754;     // LOD threshold distance
extern u16 lbl_eu_80665758;     // per-entry distance key threshold
extern void (*lbl_eu_8056D700[])(LOD::UnkClass_8046368C*);  // .data dispatch

// sdata2 constants used by the interpolation helpers.
extern const f32 lbl_eu_8066A5C0;  // 1.0f
extern const f32 lbl_eu_8066A5C4;  // default LOD value
extern const f32 lbl_eu_8066A5D8;
extern const f32 lbl_eu_8066A5DC;
extern const f32 lbl_eu_8066A5E0;
extern const f32 lbl_eu_8066A5E4;