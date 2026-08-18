// LOD cache-record distance manager (monolib/src/lod/CLODCacheManagerS).
//
// Private downstream fork decompilation.  Retail symbols are the (shortened)
// "...Fv" names; real ABI arguments are passed explicitly (Metrowerks passes
// extra registers even on Fv names -- see docs/MWCC_REFERENCE.md "Fv ABI
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
    u16 field_0x2;   // 0x02 far distance
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

// Live LOD record dispatch function pointer (set by func_8046368C/69C).
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
DECOMP_FORCEACTIVE(CLODCacheManagerS_cpp, lbl_eu_8066575A);
DECOMP_FORCEACTIVE(CLODCacheManagerS_cpp, lbl_eu_8066575C);
DECOMP_FORCEACTIVE(CLODCacheManagerS_cpp, lbl_eu_80665764);
DECOMP_FORCEACTIVE(CLODCacheManagerS_cpp, lbl_eu_80665768);
extern void (*lbl_eu_8056D700[])(LOD::UnkClass_8046368C*);  // .data dispatch

// sdata2 constants used by the interpolation helpers.
// Named .sdata2 conversion magic: defining the 2^52 doubles lets MWCC's
// constant pool reuse the retail symbols for the builtin (f32)u16/(f32)s32
// casts instead of emitting TU-local @N labels (CfResReloadImpl /
// CSuddenCommu idiom, same as CMiniMap.cpp's 806670A8).
extern const f32 lbl_eu_8066A5C0;  // 1.0f
extern const f32 lbl_eu_8066A5C4;  // default LOD value
extern const double lbl_eu_8066A5C8 = 0x4330000000000000ll;  // 2^52 (u16->f32)
extern const double lbl_eu_8066A5D0 = 0x4330000080000000ll;  // 2^52+2^31 (s32->f32)
extern const f32 lbl_eu_8066A5D8;
extern const f32 lbl_eu_8066A5DC;
extern const f32 lbl_eu_8066A5E0;
extern const f32 lbl_eu_8066A5E4;

using namespace LOD;

// Forward declarations of the two record-lookup functions whose addresses are
// stored into lbl_eu_80665760 (defined later in this TU).
extern "C" f32 func_8046323C__Q23LOD17CLODCacheManagerSFv(CLODCacheManagerS*);
extern "C" f32 func_80463118__Q23LOD17CLODCacheManagerSFv(CLODCacheManagerS*);

// ===========================================================================
// us-80467504  func_804630C0  (bind LOD tables into the sbss bases)
// ===========================================================================
extern "C" void func_804630C0__Q23LOD17CLODCacheManagerSFv(void* self)
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
// us-8046765c  func_8046368C  (select the record-lookup function)
// ===========================================================================
extern "C" void func_8046368C__Q23LOD17UnkClass_8046368CFv()
{
    lbl_eu_80665760 = func_8046323C__Q23LOD17CLODCacheManagerSFv;
}

// ===========================================================================
// us-8046766c  func_8046369C  (select the other record-lookup function)
// ===========================================================================
extern "C" void func_8046369C__Q23LOD17UnkClass_8046368CFv()
{
    lbl_eu_80665760 = func_80463118__Q23LOD17CLODCacheManagerSFv;
}

// ===========================================================================
// us-8046771c  func_8046374C  (accumulate into field 0xC)
// ===========================================================================
extern "C" void func_8046374C__Q23LOD17UnkClass_8046368CFv(UnkClass_8046368C* self, f32 f1)
{
    self->field_0xC += f1;
}

// ===========================================================================
// us-8046772c  func_8046375C  (accumulate into field 0x1C)
// ===========================================================================
extern "C" void func_8046375C__Q23LOD17UnkClass_8046368CFv(UnkClass_8046368C* self, f32 f1)
{
    self->field_0x1C += f1;
}

// ===========================================================================
// us-8046767c  func_804636AC  (dispatch one record-pair via bound lookup fn)
// ===========================================================================
extern "C" void func_804636AC__Q23LOD17UnkClass_8046368CFv(UnkClass_8046368C* self, s32 index)
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
    f32 lim = lbl_eu_80665754;

    // Sweep up to the near distance: report the first element's near state.
    if (lim <= (f32)field_0x0) {
        return lbl_eu_80665748[field_0x8].field_0x0 != 0 ? 1 : 0;
    }

    // Sweep past the far distance: report the last element's near state.
    if (lim >= (f32)field_0x2) {
        return lbl_eu_80665748[field_0x8 + field_0x6 - 1].field_0x0 != 0 ? 1 : 0;
    }

    // Inside the sweep: walk forward until a bin's far cutoff exceeds the key.
    u16 key = lbl_eu_80665758;
    s32 cnt = field_0x6;
    LODCacheIndex* e = &lbl_eu_80665748[field_0x8 + 1];
    for (s32 j = 1; j < cnt; j++) {
        if (key < e->field_0x2) {
            return (e - 1)->field_0x0 != 0 ? 1 : 0;
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
    f32 nearF = (f32)field_0x0;
    f32 lim = lbl_eu_80665754;

    if (lim <= nearF) {
        return lbl_eu_8066573C[field_0x8].near;
    }

    f32 farF = (f32)field_0x2;
    if (lim >= farF) {
        return lbl_eu_8066573C[field_0x8 + field_0x6 - 1].near;
    }

    u16 key = lbl_eu_80665758;
    s32 cnt = (s32)field_0x6;
    s32 base = (s32)field_0x8;
    for (s32 i = base + 1; i < base + cnt; i++) {
        u16 A = lbl_eu_8066573C[i].far;
        if (key < A) {
            u16 B = lbl_eu_8066573C[i - 1].far;
            f32 t = (lim - (f32)B) / (f32)(A - B);
            return lbl_eu_8066573C[i - 1].near * (1.0f - t) +
                   lbl_eu_8066573C[i].near * t;
        }
    }
    return lbl_eu_8066A5C4;
}

// ===========================================================================
// us-8046720c  func_8046323C  (cubic catmull-rom distance lookup)
//
// Same sweep as func_80463118, but the straddling pair is interpolated with a
// catmull-rom cubic over four consecutive level samples.  Basis constants
// lbl_eu_8066A5D8 / _DC / _E0.
// ===========================================================================
f32 CLODCacheManagerS::func_8046323C()
{
    f32 lim = lbl_eu_80665754;

    if (lim <= (f32)field_0x0) {
        return lbl_eu_80665740[field_0x8].f00;
    }

    if (lim >= (f32)field_0x2) {
        return lbl_eu_80665740[field_0x8 + field_0x6 - 1].f00;
    }

    u16 key = lbl_eu_80665758;
    s32 cnt = (s32)field_0x6;
    s32 base = (s32)field_0x8;
    for (s32 k = base + 1; k < base + cnt; k++) {
        u16 A = lbl_eu_80665740[k].far;
        if (key < A) {
            u16 B = lbl_eu_80665740[k - 1].far;
            f32 t  = (lim - (f32)B) / (f32)(A - B);
            f32 t2 = t * t;
            f32 t3 = t * t2;
            f32 u = t + (t3 - lbl_eu_8066A5D8 * t2);
            f32 v = lbl_eu_8066A5C0 + (lbl_eu_8066A5D8 * t3 - lbl_eu_8066A5DC * t2);
            f32 c3 = lbl_eu_8066A5E0 * t3 + lbl_eu_8066A5DC * t2;
            f32 w = t3 - t2;
            return lbl_eu_80665740[k - 1].f04 * u +
                   lbl_eu_80665740[k - 1].f00 * v +
                   lbl_eu_80665740[k].f00 * c3 +
                   lbl_eu_80665740[k].f08 * w;
        }
    }
    return lbl_eu_8066A5C4;
}

// ===========================================================================
// us-8046736c  func_8046339C  (2D distance -> integer sample lookup)
//
// Looks up a cache record by the pair table, then over each straddling bin
// rounds `offset + (1-t)*prev + t*cur` to an integer and emits the bin's
// 2D word pair (outX/outY) when it equals the current sample, else the
// previous bin's pair.  Returns Fv (args via r3/r4/r5: outputs, index).
// ===========================================================================
extern "C" void func_8046339C__Q23LOD17CLODCacheManagerSFv(s32* outA, s32* outB,
                                                           u32 index)
{
    f32 lim = lbl_eu_80665754;
    u32 entry = lbl_eu_8066574C[index];       // pair-table offset
    u32 recIdx = ((u16*)lbl_eu_80665750)[entry + 1];
    CLODCacheManagerS* rec = lbl_eu_80665738 + recIdx;

    if (lim <= (f32)rec->field_0x0) {
        *outA = lbl_eu_80665744[rec->field_0x8].outX;
        *outB = lbl_eu_80665744[rec->field_0x8].outY;
        return;
    }

    if (lim >= (f32)rec->field_0x2) {
        *outA = lbl_eu_80665744[rec->field_0x8 + rec->field_0x6 - 1].outX;
        *outB = lbl_eu_80665744[rec->field_0x8 + rec->field_0x6 - 1].outY;
        return;
    }

    s32 base = (s32)rec->field_0x8;
    s32 cnt = (s32)rec->field_0x6;
    LODShortEntry* e = &lbl_eu_80665744[base + 1];
    u16 key = lbl_eu_80665758;
    for (s32 j = 1; j < cnt; j++) {
        u16 A = e->far;
        if (key < A) {
            u16 B = (e - 1)->far;
            f32 t = (lim - (f32)B) / (f32)(A - B);
            f32 lerp = (f32)(e - 1)->val * (1.0f - t) + (f32)e->val * t;
            s32 ri = (s32)(lbl_eu_8066A5E4 + lerp);
            if (ri == e->val) {
                *outA = e->outX;
                *outB = e->outY;
            } else {
                *outA = (e - 1)->outX;
                *outB = (e - 1)->outY;
            }
            return;
        }
        e++;
    }
    *outA = 0;
    *outB = 0;
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056D700-0x8056D71C (28B): LOD dispatch table. 8046374C/8046375C
// are this TU's; the rest live in the sibling LOD TUs.
extern "C" void func_8046376C__Q23LOD17UnkClass_8046368CFv();
extern "C" void func_804645CC__Q23LOD17UnkClass_804645CCFv();
extern "C" void func_80468434__Q23LOD17UnkClass_80468434Fv();
extern "C" void func_80464B84__Q23LOD17UnkClass_804645CCFv();
typedef void (*LODDispFn)(LOD::UnkClass_8046368C*);
void (*lbl_eu_8056D700[])(LOD::UnkClass_8046368C*) = {
    (LODDispFn)&func_8046374C__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)&func_8046375C__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)&func_8046376C__Q23LOD17UnkClass_8046368CFv,
    (LODDispFn)0,
    (LODDispFn)&func_804645CC__Q23LOD17UnkClass_804645CCFv,
    (LODDispFn)&func_80468434__Q23LOD17UnkClass_80468434Fv,
    (LODDispFn)&func_80464B84__Q23LOD17UnkClass_804645CCFv,
};
DECOMP_FORCEACTIVE(CLODCacheManagerS_cpp, lbl_eu_8056D700);
