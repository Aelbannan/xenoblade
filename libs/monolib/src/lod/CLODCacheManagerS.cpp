// LOD cache-record distance manager (monolib/src/lod/CLODCacheManagerS).
//
// Private downstream fork decompilation.  Retail symbols are the (shortened)
// "...Fv" names; real ABI arguments are passed explicitly (Metrowerks passes
// extra registers even on Fv names -- see docs/MWCC_CASES.md "Fv ABI
// note", and the sibling TU UnkClass_8046368C.cpp uses the same convention).

#include <harness_catalog.h>
#include <revolution/MTX.h>

namespace LOD {

/**
 * LOD cache record.  Retail stride is 0xC bytes.  Shared with the
 * distance-table lookup functions (lerpDistEntry / catmullRomBin /
 * fetchSampleXY / findCutBinIdx).
 */
struct CLODCacheManagerS {
    u16 field_0x0;   // 0x00 near distance
    u16 field_0x2;   // 0x02 far distance
    u16 field_0x4;   // 0x04 record id (dispatch-table index)
    u16 field_0x6;   // 0x06 entry count
    u32 field_0x8;   // 0x08 table index

    s32 findCutBinIdx();
    f32 lerpDistEntry();
    f32 catmullRomBin();
};

/**
 * LOD terrain state manager.  Only the fields touched by the recovered
 * functions are declared; remainder is opaque.
 */
struct UnkClass_8046368C {
    u8  pad_00[0xC];
    f32 field_0xC;   // 0x0C
    u8  pad_10[0x1C - 0x10];
    f32 field_0x1C;  // 0x1C

    static void selectCubicLookup();
    static void selectLinearLookup();
};

/**
 * LOD distance-key element (index table sLodCacheIndexTable, stride 4).
 * near is a signed distance (0x00), far an unsigned cutoff (0x02).
 */
struct LODCacheIndex {
    s16 field_0x0;   // 0x00 near distance
    u16 far;         // 0x02 far cutoff
};

/**
 * LOD linear distance-table entry (sLodDistTable, stride 8).
 * near is a float distance (0x00); far an unsigned cutoff (0x04).
 */
struct LODDistEntry {
    f32 near;   // 0x00
    u16 far;    // 0x04
};

/**
 * LOD level-table entry (sLodLevelTable, stride 0x10) used by the cubic
 * catmull-rom interpolator.  Four float sample slots plus an unsigned far
 * cutoff.
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

// Live LOD record dispatch function pointer (set by selectCubicLookup/69C).
typedef f32 (*LODRecordFn)(CLODCacheManagerS* rec);

} // namespace LOD

// ---------------------------------------------------------------------------
// Shared sbss / sdata2 data (retail symbol names).
// ---------------------------------------------------------------------------

// [.sbss] 0x80665738-0x8066576C (52B) - definitions (address order mirrors the
// retail symbol layout: 7x4B pointers, f32, u16+pad (8B span), fnptr+pad (8B
// span), tail global).
LOD::CLODCacheManagerS* sLodCacheRecords;  // cache records, stride 0xC
LOD::LODDistEntry* sLodDistTable;   // distance table, stride 8
LOD::LODLevelEntry* sLodLevelTable;   // level table, stride 0x10
LOD::LODShortEntry* sLodShortTable;   // short table, stride 8
LOD::LODCacheIndex* sLodCacheIndexTable;   // distance-key index table, stride 4
u32* sLodPairOffsets;    // index -> pair-table offset
void* sLodPairTable;   // pair table, stride 2 (u16 entries)
f32 sLodThresholdDist;     // LOD threshold distance
u16 sLodEntryKeyThreshold;     // per-entry distance key threshold (8B retail span)
LOD::LODRecordFn sLodRecordLookupFn __attribute__((aligned(8)));  // active record lookup fn (8B retail span)
__declspec(align(8)) u32 sLodCacheTailFlag;  // retail tail global (aligns .sbss 8)
extern void (*sLodRecordDispatchTable[])(LOD::UnkClass_8046368C*);  // .data dispatch

using namespace LOD;

// Forward declarations of the two record-lookup functions whose addresses are
// stored into sLodRecordLookupFn (defined later in this TU).
extern "C" f32 catmullRomBin__Q23LOD17CLODCacheManagerSFv(CLODCacheManagerS*);
extern "C" f32 lerpDistEntry__Q23LOD17CLODCacheManagerSFv(CLODCacheManagerS*);

// ===========================================================================
// us-80467504  initLodTables  (bind LOD tables into the sbss bases)
// ===========================================================================
extern "C" void initLodTables__Q23LOD17CLODCacheManagerSFv(void* self)
{
    struct Layout {
        unsigned int p00;
        unsigned int unused04;
        unsigned int p08;
        unsigned int unused0C;
        unsigned int p10;
        unsigned int unused14;
        unsigned int p18;
        unsigned int unused1C;
        unsigned int p20;
        unsigned int unused24;
        unsigned int p28;
        unsigned int unused2C;
        unsigned int p30;
    };

    Layout* cache = static_cast<Layout*>(self);
    sLodCacheRecords = (CLODCacheManagerS*)(static_cast<char*>(self) + cache->p00);
    sLodDistTable = (LOD::LODDistEntry*)(static_cast<char*>(self) + cache->p08);
    sLodPairOffsets = (u32*)(static_cast<char*>(self) + cache->p10);
    sLodPairTable = static_cast<char*>(self) + cache->p18;
    sLodLevelTable = (LOD::LODLevelEntry*)(static_cast<char*>(self) + cache->p20);
    sLodCacheIndexTable = reinterpret_cast<LOD::LODCacheIndex*>(static_cast<char*>(self) + cache->p30);
    sLodShortTable = (LOD::LODShortEntry*)(static_cast<char*>(self) + cache->p28);
}

// ===========================================================================
// us-8046765c  selectCubicLookup  (select the record-lookup function)
// ===========================================================================
void UnkClass_8046368C::selectCubicLookup()
{
    sLodRecordLookupFn = catmullRomBin__Q23LOD17CLODCacheManagerSFv;
}

// ===========================================================================
// us-8046766c  selectLinearLookup  (select the other record-lookup function)
// ===========================================================================
void UnkClass_8046368C::selectLinearLookup()
{
    sLodRecordLookupFn = lerpDistEntry__Q23LOD17CLODCacheManagerSFv;
}

// ===========================================================================
// us-8046771c  getCacheEntry  (accumulate into field 0xC)
// ===========================================================================
extern "C" void getCacheEntry__Q23LOD17UnkClass_8046368CFv(UnkClass_8046368C* self, f32 f1)
{
    self->field_0xC += f1;
}

// ===========================================================================
// us-8046772c  getCacheCount  (accumulate into field 0x1C)
// ===========================================================================
extern "C" void getCacheCount__Q23LOD17UnkClass_8046368CFv(UnkClass_8046368C* self, f32 f1)
{
    self->field_0x1C += f1;
}

// ===========================================================================
// us-8046767c  dispatchLodPair  (dispatch one record-pair via bound lookup fn)
// ===========================================================================
extern "C" void dispatchLodPair__Q23LOD17UnkClass_8046368CFv(UnkClass_8046368C* self, s32 index)
{
    u16* pair = (u16*)sLodPairTable;       // pair table (u16 stride)
    u32 entry = sLodPairOffsets[index];      // offset into the pair table
    u16* cursor = &pair[entry];
    u16 n = pair[entry];                     // record count for this pair

    PSMTXIdentity(*(Mtx*)(void*)self);   // identity into self+0

    for (s32 i = 0; i < n; i++) {
        u16 kind = cursor[1];                // kind tag of this cache record
        CLODCacheManagerS* rec =
            (CLODCacheManagerS*)((u8*)sLodCacheRecords + (u32)kind * 0xC);
        sLodRecordLookupFn(rec);                // distance lookup (dead return)
        u16 id = rec->field_0x4;             // dispatch-table index
        sLodRecordDispatchTable[id](self);           // .data dispatch table
        cursor++;
    }
}

// ===========================================================================
// us-804670e8  lerpDistEntry  (linear distance lookup)
//
// Return the base level distance when it precedes the sweep, the last bin
// distance when it follows, otherwise linearly interpolate the two bins
// that straddle the current LOD threshold key.
// ===========================================================================
f32 CLODCacheManagerS::lerpDistEntry()
{
    // Retail model: builtin casts (MWCC 0x4330 idiom, lone fsubs), two
    // scratch slots, CTR pointer loop (for-shape); minimal float temps so
    // MWCC's scratch-FPR birth order matches retail (magic f1, 1.0 f2).
    if (sLodThresholdDist <= (f32)field_0x0) {
        return sLodDistTable[field_0x8].near;
    }
    if (sLodThresholdDist >= (f32)field_0x2) {
        LODDistEntry* en = &sLodDistTable[field_0x8 + field_0x6];
        return en[-1].near;
    }
    u16 key = sLodEntryKeyThreshold;
    s32 cnt = (s32)field_0x6;
    LODDistEntry* e = &sLodDistTable[field_0x8 + 1];
    for (s32 j = 1; j < cnt; j++) {
        u16 A = e->far;
        if (key < A) {
            u16 B = e[-1].far;
            s32 span = (s32)A - (s32)B;
            f32 unit = 1.0f;
            f32 spanF = (f32)span;
            f32 t = (sLodThresholdDist - (f32)B) / spanF;
            f32 leftP = e[-1].near * (unit - t);
            return leftP + e->near * t;
        }
        e++;
    }
    return 0.0f;
}
// (line-neutral pads: keep lerpDistEntry region at 36 lines)
// (pad 2)
// (pad 3)
// (pad 4)
// (pad 5)
// (pad 6: restores the 36-line region so downstream line numbers hold)


// us-8046720c  catmullRomBin  (cubic catmull-rom distance lookup)
//
// Same sweep as lerpDistEntry, but the straddling pair is interpolated with a
// catmull-rom cubic over four consecutive level samples.  The integer fields
// are plain f32 casts, so MWCC emits the 0x4330-prefix conversion sequence
// directly in the order the values are first used.

f32 CLODCacheManagerS::catmullRomBin()
{
    if (sLodThresholdDist <= (f32)field_0x0) {
        return sLodLevelTable[field_0x8].f00;
    }

    if (sLodThresholdDist >= (f32)field_0x2) {
        return sLodLevelTable[field_0x8 + field_0x6 - 1].f00;
    }

    u16 key = sLodEntryKeyThreshold;
    LODLevelEntry* e = &sLodLevelTable[field_0x8 + 1];
    s32 n = field_0x6 - 1;
    if (n > 0) {
        do {
            u16 cutA = e->far;
            if (key < cutA) {
            u16 cutB = e[-1].far;
            s32 span = (s32)cutA - (s32)cutB;
            f32 t = (sLodThresholdDist - (f32)cutB) / (f32)span;
            // Catmull-Rom cubic over four consecutive level samples.
            // DC*t2 is shared between the v and c3 terms (retail CSE).
            f32 t2 = t * t;
            f32 t3 = t * t2;
            f32 twoT3 = 2.0f * t3;
            f32 dcT2 = 3.0f * t2;
            f32 u = t + (t3 - 2.0f * t2);
            f32 v = 1.0f + (twoT3 - dcT2);
            f32 c3 = -2.0f * t3 + dcT2;
            f32 w = t3 - t2;
            return e[-1].f04 * u +
                   e[-1].f00 * v +
                   e->f00 * c3 +
                   e->f08 * w;
            }
            e++;
        } while (--n != 0);
    }
    return 0.0f;
}

// ===========================================================================
// us-8046736c  fetchSampleXY  (2D distance -> integer sample lookup)
//
// Looks up a cache record by the pair table, then over each straddling bin
// rounds `offset + (1-t)*prev + t*cur` to an integer and emits the bin's
// 2D word pair (outX/outY) when it equals the current sample, else the
// previous bin's pair.
//
// Plain builtin u16/s32 -> f32 casts throughout: MWCC emits the 0x43300000
// stack-scratch / fsubs-of-2^52-magic sequence per conversion, reusing two
// scratch slots (sp+0x08 signed, sp+0x10 unsigned) across the whole function
// (same shape as lerpDistEntry).
// ===========================================================================
extern "C" void fetchSampleXY__Q23LOD17CLODCacheManagerSFv(s32* outA, s32* outB,
                                                           u32 index)
{
    u16 kind = ((u16*)sLodPairTable)[sLodPairOffsets[index] + 1];
    CLODCacheManagerS* rec = sLodCacheRecords + kind;

    if (sLodThresholdDist <= (f32)rec->field_0x0) {
        *outA = sLodShortTable[rec->field_0x8].outX;
        *outB = sLodShortTable[rec->field_0x8].outY;
        return;
    }

    if (sLodThresholdDist >= (f32)rec->field_0x2) {
        *outA = sLodShortTable[rec->field_0x8 + rec->field_0x6 - 1].outX;
        *outB = sLodShortTable[rec->field_0x8 + rec->field_0x6 - 1].outY;
        return;
    }

    LODShortEntry* e = &sLodShortTable[rec->field_0x8 + 1];
    u16 key = sLodEntryKeyThreshold;
    s32 n = rec->field_0x6 - 1;
    if (n > 0) {
        do {
            u16 A = e->far;
            if (key < A) {
                u16 B = (e - 1)->far;
                s32 span = (s32)A - (s32)B;
                f32 t = (sLodThresholdDist - (f32)B) / (f32)span;
                // Rounded lerp of the straddling sample pair plus offset.
                s16 curVal = e->val;
                f32 v = 0.5f +
                        (e - 1)->val * (1.0f - t) + e->val * t;
                if ((s32)v == curVal) {
                    *outA = e->outX;
                    *outB = e->outY;
                } else {
                    *outA = (e - 1)->outX;
                    *outB = (e - 1)->outY;
                }
                return;
            }
            e++;
        } while (--n != 0);
    }
    *outA = 0;
    *outB = 0;
}
// ===========================================================================
// us-80467560  findCutBinIdx  (distance-bin lookup: which index cuts?)
//
// Returns nonzero when the index element preceding the first bin whose far
// cutoff exceeds the per-entry key lies inside the given near/far sweep.
// Byte-identical FULL_MATCH target (fixed u16->f32 float-conv codegen).
// ===========================================================================
s32 CLODCacheManagerS::findCutBinIdx()
{
    // Sweep up to the near distance: report the first element's near state.
    if (sLodThresholdDist <= (f32)(u32)field_0x0) {
        return sLodCacheIndexTable[field_0x8].field_0x0 != 0;
    }

    // Sweep past the far distance: report the last element's near state.
    if (sLodThresholdDist >= (f32)(u32)field_0x2) {
        return sLodCacheIndexTable[field_0x8 + field_0x6 - 1].field_0x0 != 0;
    }

    // Inside the sweep: walk forward until a bin's far cutoff exceeds the key.
    u16 key = sLodEntryKeyThreshold;
    s32 cnt = (s32)field_0x6;
    LODCacheIndex* e = &sLodCacheIndexTable[field_0x8 + 1];
    for (s32 j = 1; j < cnt; j++) {
        if (key < e->far) {
            return (e - 1)->field_0x0 != 0;
        }
        e++;
    }
    return 0;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056D700-0x8056D71C (28B): LOD dispatch table. 8046374C/8046375C
// are this TU's; the rest live in the sibling LOD TUs.
extern "C" void setLodRotation__Q23LOD17UnkClass_8046368CFv();
extern "C" void drawLodRecordExt__Q23LOD17UnkClass_804645CCFv();
extern "C" void dispatchLodRecord__Q23LOD17UnkClass_80468434Fv();
extern "C" void drawLodRecord__Q23LOD17UnkClass_804645CCFv();
typedef void (*LODDispFn)(LOD::UnkClass_8046368C*);
void (*sLodRecordDispatchTable[])(LOD::UnkClass_8046368C*) = {
    (LODDispFn)&getCacheEntry__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)&getCacheCount__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)&setLodRotation__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)0,
    (LODDispFn)&drawLodRecordExt__Q23LOD17UnkClass_804645CCFv,
    (LODDispFn)&dispatchLodRecord__Q23LOD17UnkClass_80468434Fv,
    (LODDispFn)&drawLodRecord__Q23LOD17UnkClass_804645CCFv,
};
