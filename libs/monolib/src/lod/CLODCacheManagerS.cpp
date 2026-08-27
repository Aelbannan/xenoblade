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
 * distance-table lookup functions (func_80463118 / func_8046323C /
 * func_8046339C / func_80463590).
 */
struct CLODCacheManagerS {
    u16 field_0x0;   // 0x00 near distance
    u16 field_0x2;   // 0x02 far distance
    u16 field_0x4;   // 0x04 record id (dispatch-table index)
    u16 field_0x6;   // 0x06 entry count
    u32 field_0x8;   // 0x08 table index

    s32 func_80463590();
    f32 func_80463118();
    f32 func_8046323C();
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
};

/**
 * LOD distance-key element (index table lbl_eu_80665748, stride 4).
 * near is a signed distance (0x00), far an unsigned cutoff (0x02).
 */
struct LODCacheIndex {
    s16 field_0x0;   // 0x00 near distance
    u16 far;         // 0x02 far cutoff
};

/**
 * LOD linear distance-table entry (lbl_eu_8066573C, stride 8).
 * near is a float distance (0x00); far an unsigned cutoff (0x04).
 */
struct LODDistEntry {
    f32 near;   // 0x00
    u16 far;    // 0x04
};

/**
 * LOD level-table entry (lbl_eu_80665740, stride 0x10) used by the cubic
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
 * LOD short-table entry (lbl_eu_80665744, stride 8) backing the 2D
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
LOD::CLODCacheManagerS* lbl_eu_80665738;  // cache records, stride 0xC
LOD::LODDistEntry* lbl_eu_8066573C;   // distance table, stride 8
LOD::LODLevelEntry* lbl_eu_80665740;   // level table, stride 0x10
LOD::LODShortEntry* lbl_eu_80665744;   // short table, stride 8
LOD::LODCacheIndex* lbl_eu_80665748;   // distance-key index table, stride 4
u32* lbl_eu_8066574C;    // index -> pair-table offset
void* lbl_eu_80665750;   // pair table, stride 2 (u16 entries)
f32 lbl_eu_80665754;     // LOD threshold distance
u16 lbl_eu_80665758;     // per-entry distance key threshold (8B retail span)
u16 lbl_eu_8066575A;     // pad within the 8B span
u32 lbl_eu_8066575C;     // pad within the 8B span
LOD::LODRecordFn lbl_eu_80665760;  // active record lookup fn (8B retail span)
u32 lbl_eu_80665764;     // pad within the 8B span
__declspec(align(8)) u32 lbl_eu_80665768;  // retail tail global (aligns .sbss 8)
extern void (*lbl_eu_8056D700[])(LOD::UnkClass_8046368C*);  // .data dispatch

// sdata2 constants used by the interpolation helpers.
// Named .sdata2 conversion magic: defining the 2^52 doubles lets MWCC's
// constant pool reuse the retail symbols for the builtin (f32)u16/(f32)s32
// casts instead of emitting TU-local @N labels (CfResReloadImpl /
// CSuddenCommu idiom, same as CMiniMap.cpp's 806670A8).
// Declared extern only: a TU-local const definition would let MWCC const-fold
// reads into anonymous @N pool labels instead of retail-symbol relocs
// (CMiniMap.cpp 806670A8 note).
extern const f32 lbl_eu_8066A5C0;  // 1.0f
extern const f32 lbl_eu_8066A5C4;  // default LOD value
extern const double lbl_eu_8066A5C8;  // 2^52 (u16->f32 magic, blob-owned)
extern const double lbl_eu_8066A5D0;  // 2^52+2^31 (s32->f32 magic, blob-owned)
extern const f32 lbl_eu_8066A5D8;
extern const f32 lbl_eu_8066A5DC;
extern const f32 lbl_eu_8066A5E0;
extern const f32 lbl_eu_8066A5E4;

// Builtin (f32)u16/(f32)s32 casts pool TU-local magic doubles; the retail
// object references the blob pool entries (lbl_eu_8066A5C8/A5D0) instead.
// Union helpers keep this TU's .sdata2 empty (retail shape).
inline f32 u16ToF_a5c8(u16 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = v;             // low word first: matches retail store order
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066A5C8);
}
inline f32 s32ToF_a5d0(s32 v) {
    union {
        double d;
        u32 w[2];
    } c;
    c.w[1] = (u32)v ^ 0x80000000u;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066A5D0);
}

using namespace LOD;

// Forward declarations of the two record-lookup functions whose addresses are
// stored into lbl_eu_80665760 (defined later in this TU).
extern "C" f32 func_8046323C__Q23LOD17CLODCacheManagerSFv(CLODCacheManagerS*);
extern "C" f32 func_80463118__Q23LOD17CLODCacheManagerSFv(CLODCacheManagerS*);

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
    lbl_eu_80665738 = (CLODCacheManagerS*)(static_cast<char*>(self) + cache->p00);
    lbl_eu_8066573C = (LOD::LODDistEntry*)(static_cast<char*>(self) + cache->p08);
    lbl_eu_8066574C = (u32*)(static_cast<char*>(self) + cache->p10);
    lbl_eu_80665750 = static_cast<char*>(self) + cache->p18;
    lbl_eu_80665740 = (LOD::LODLevelEntry*)(static_cast<char*>(self) + cache->p20);
    lbl_eu_80665748 = reinterpret_cast<LOD::LODCacheIndex*>(static_cast<char*>(self) + cache->p30);
    lbl_eu_80665744 = (LOD::LODShortEntry*)(static_cast<char*>(self) + cache->p28);
}

// ===========================================================================
// us-8046765c  selectCubicLookup  (select the record-lookup function)
// ===========================================================================
extern "C" void selectCubicLookup__Q23LOD17UnkClass_8046368CFv()
{
    lbl_eu_80665760 = func_8046323C__Q23LOD17CLODCacheManagerSFv;
}

// ===========================================================================
// us-8046766c  selectLinearLookup  (select the other record-lookup function)
// ===========================================================================
extern "C" void selectLinearLookup__Q23LOD17UnkClass_8046368CFv()
{
    lbl_eu_80665760 = func_80463118__Q23LOD17CLODCacheManagerSFv;
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
    u16* pair = (u16*)lbl_eu_80665750;       // pair table (u16 stride)
    u32 entry = lbl_eu_8066574C[index];      // offset into the pair table
    u16* cursor = &pair[entry];
    u16 n = pair[entry];                     // record count for this pair

    PSMTXIdentity(*(Mtx*)(void*)self);   // identity into self+0

    for (s32 i = 0; i < n; i++) {
        u16 kind = cursor[1];                // kind tag of this cache record
        CLODCacheManagerS* rec =
            (CLODCacheManagerS*)((u8*)lbl_eu_80665738 + (u32)kind * 0xC);
        lbl_eu_80665760(rec);                // distance lookup (dead return)
        u16 id = rec->field_0x4;             // dispatch-table index
        lbl_eu_8056D700[id](self);           // .data dispatch table
        cursor++;
    }
}

// ===========================================================================
// us-80467560  func_80463590  (distance-bin lookup: which index cuts?)
//
// Returns nonzero when the index element preceding the first bin whose far
// cutoff exceeds the per-entry key lies inside the given near/far sweep.
// Byte-identical FULL_MATCH target (fixed u16->f32 float-conv codegen).
// ===========================================================================
s32 CLODCacheManagerS::func_80463590()
{
    // Sweep up to the near distance: report the first element's near state.
    if (lbl_eu_80665754 <= (f32)(u32)field_0x0) {
        return lbl_eu_80665748[field_0x8].field_0x0 != 0;
    }

    // Sweep past the far distance: report the last element's near state.
    if (lbl_eu_80665754 >= (f32)(u32)field_0x2) {
        return lbl_eu_80665748[field_0x8 + field_0x6 - 1].field_0x0 != 0;
    }

    // Inside the sweep: walk forward until a bin's far cutoff exceeds the key.
    u16 key = lbl_eu_80665758;
    s32 cnt = (s32)field_0x6;
    LODCacheIndex* e = &lbl_eu_80665748[field_0x8 + 1];
    for (s32 j = 1; j < cnt; j++) {
        if (key < e->far) {
            return (e - 1)->field_0x0 != 0;
        }
        e++;
    }
    return 0;
}

// ===========================================================================
// us-804670e8  func_80463118  (linear distance lookup)
//
// Return the base level distance when it precedes the sweep, the last bin
// distance when it follows, otherwise linearly interpolate the two bins
// that straddle the current LOD threshold key.
// ===========================================================================
f32 CLODCacheManagerS::func_80463118()
{
    // Plain builtin u16/s32 -> f32 casts: MWCC emits the 0x43300000 stack
    // scratch / xoris / fsubs-of-2^52-magic sequence per conversion, reusing
    // two scratch slots across the whole function (retail shape).
    if (lbl_eu_80665754 <= (f32)field_0x0) {
        return lbl_eu_8066573C[field_0x8].near;
    }

    if (lbl_eu_80665754 >= (f32)field_0x2) {
        LODDistEntry* end = &lbl_eu_8066573C[field_0x8 + field_0x6];
        return end[-1].near;
    }

    // Walk the straddling bins; counter runs over entries after the first.
    s32 base = field_0x8;
    s32 cnt = field_0x6;
    LODDistEntry* e = &lbl_eu_8066573C[base + 1];
    u16 key = lbl_eu_80665758;
    if (cnt > 1) {
        s32 n = cnt - 1;
        do {
            u16 A = e->far;
            if (key < A) {
                u16 B = e[-1].far;
                s32 d = (s32)A - (s32)B;
                f32 bFar = (f32)B;
                f32 denom = (f32)d;
                f32 t = (lbl_eu_80665754 - bFar) / denom;
                return e[-1].near * (lbl_eu_8066A5C0 - t) + e->near * t;
            }
            e++;
        } while (--n != 0);
    }
    return lbl_eu_8066A5C4;
}

/**
 * Integer->double bit-conversion scratch slot (MWCC 0x4330-prefix trick).
 * w[0] is the high word (always 0x43300000), w[1] the payload; subtracting a
 * shared 2^52 magic double yields the numeric value.
 */
union F64ConvScratch {
    double d;
    u32 w[2];
};

// ===========================================================================
// us-8046720c  func_8046323C  (cubic catmull-rom distance lookup)
//
// Same sweep as func_80463118, but the straddling pair is interpolated with a
// catmull-rom cubic over four consecutive level samples.  Basis constants
// lbl_eu_8066A5D8 / _DC / _E0.
//
// Retail keeps TWO persistent 0x4330-prefix conversion scratch doubles on the
// stack across the whole function: slot A converts field_0x0 (u16) here and
// the signed span (A-B) in the loop; slot B converts field_0x2 (u16) and the
// unsigned previous-bin cutoff B.  Modelling them as two union locals makes
// MWCC allocate the identical frame (sp-0x20) and reuse the high words.
// ===========================================================================
f32 CLODCacheManagerS::func_8046323C()
{
    F64ConvScratch cvtB;   // sp+0x10 (MWCC allocates locals in reverse order)
    F64ConvScratch cvtA;   // sp+0x08

    // high word first (retail stores 0x4330 before the payload word)
    cvtA.w[0] = 0x43300000u;
    cvtA.w[1] = field_0x0;
    if (lbl_eu_80665754 <= (f32)(cvtA.d - lbl_eu_8066A5C8)) {
        return lbl_eu_80665740[field_0x8].f00;
    }

    cvtB.w[0] = 0x43300000u;
    cvtB.w[1] = field_0x2;
    if (lbl_eu_80665754 >= (f32)(cvtB.d - lbl_eu_8066A5C8)) {
        return lbl_eu_80665740[field_0x8 + field_0x6 - 1].f00;
    }

    u16 key = lbl_eu_80665758;
    LODLevelEntry* e = &lbl_eu_80665740[field_0x8 + 1];
    s32 n = field_0x6 - 1;
    if (n > 0) {
        do {
            u16 cutA = e->far;
            if (key < cutA) {
            u16 cutB = e[-1].far;
            cvtB.w[1] = cutB;
            s32 span = (s32)cutA - (s32)cutB;
            cvtA.w[1] = (u32)span ^ 0x80000000u;
            f32 t = (lbl_eu_80665754 - (f32)(cvtB.d - lbl_eu_8066A5C8)) /
                    (f32)(cvtA.d - lbl_eu_8066A5D0);
            // Catmull-Rom cubic over four consecutive level samples.
            // DC*t2 is shared between the v and c3 terms (retail CSE).
            f32 t2 = t * t;
            f32 t3 = t * t2;
            f32 dcT2 = lbl_eu_8066A5DC * t2;
            f32 u = t + (t3 - lbl_eu_8066A5D8 * t2);
            f32 v = lbl_eu_8066A5C0 + (lbl_eu_8066A5D8 * t3 - dcT2);
            f32 c3 = lbl_eu_8066A5E0 * t3 + dcT2;
            f32 w = t3 - t2;
            return e[-1].f04 * u +
                   e[-1].f00 * v +
                   e->f00 * c3 +
                   e->f08 * w;
            }
            e++;
        } while (--n != 0);
    }
    return lbl_eu_8066A5C4;
}

// ===========================================================================
// us-8046736c  func_8046339C  (2D distance -> integer sample lookup)
//
// Looks up a cache record by the pair table, then over each straddling bin
// rounds `offset + (1-t)*prev + t*cur` to an integer and emits the bin's
// 2D word pair (outX/outY) when it equals the current sample, else the
// previous bin's pair.
//
// Plain builtin u16/s32 -> f32 casts throughout: MWCC emits the 0x43300000
// stack-scratch / fsubs-of-2^52-magic sequence per conversion, reusing two
// scratch slots (sp+0x08 signed, sp+0x10 unsigned) across the whole function
// (same shape as func_80463118).
// ===========================================================================
extern "C" void func_8046339C__Q23LOD17CLODCacheManagerSFv(s32* outA, s32* outB,
                                                           u32 index)
{
    u16 kind = ((u16*)lbl_eu_80665750)[lbl_eu_8066574C[index] + 1];
    CLODCacheManagerS* rec = lbl_eu_80665738 + kind;

    if (lbl_eu_80665754 <= (f32)rec->field_0x0) {
        *outA = lbl_eu_80665744[rec->field_0x8].outX;
        *outB = lbl_eu_80665744[rec->field_0x8].outY;
        return;
    }

    if (lbl_eu_80665754 >= (f32)rec->field_0x2) {
        *outA = lbl_eu_80665744[rec->field_0x8 + rec->field_0x6 - 1].outX;
        *outB = lbl_eu_80665744[rec->field_0x8 + rec->field_0x6 - 1].outY;
        return;
    }

    LODShortEntry* e = &lbl_eu_80665744[rec->field_0x8 + 1];
    u16 key = lbl_eu_80665758;
    s32 n = rec->field_0x6 - 1;
    if (n > 0) {
        do {
            u16 A = e->far;
            if (key < A) {
                u16 B = (e - 1)->far;
                s32 span = (s32)A - (s32)B;
                f32 t = (lbl_eu_80665754 - (f32)B) / (f32)span;
                // Rounded lerp of the straddling sample pair plus offset.
                s16 curVal = e->val;
                f32 v = lbl_eu_8066A5E4 +
                        (e - 1)->val * (lbl_eu_8066A5C0 - t) + e->val * t;
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
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056D700-0x8056D71C (28B): LOD dispatch table. 8046374C/8046375C
// are this TU's; the rest live in the sibling LOD TUs.
extern "C" void setLodRotation__Q23LOD17UnkClass_8046368CFv();
extern "C" void func_804645CC__Q23LOD17UnkClass_804645CCFv();
extern "C" void func_80468434__Q23LOD17UnkClass_80468434Fv();
extern "C" void func_80464B84__Q23LOD17UnkClass_804645CCFv();
typedef void (*LODDispFn)(LOD::UnkClass_8046368C*);
void (*lbl_eu_8056D700[])(LOD::UnkClass_8046368C*) = {
    (LODDispFn)&getCacheEntry__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)&getCacheCount__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)&setLodRotation__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)0,
    (LODDispFn)&func_804645CC__Q23LOD17UnkClass_804645CCFv,
    (LODDispFn)&func_80468434__Q23LOD17UnkClass_80468434Fv,
    (LODDispFn)&func_80464B84__Q23LOD17UnkClass_804645CCFv,
};
