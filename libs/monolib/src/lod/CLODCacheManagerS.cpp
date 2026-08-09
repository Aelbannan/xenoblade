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

// Live LOD record dispatch function pointer (set by func_8046368C/69C).
typedef f32 (*LODRecordFn)(CLODCacheManagerS* rec);

} // namespace LOD

// ---------------------------------------------------------------------------
// Shared sbss / sdata2 data (retail symbol names).
// ---------------------------------------------------------------------------

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
    lbl_eu_8066573C = static_cast<char*>(self) + cache->p08;
    lbl_eu_8066574C = (u32*)(static_cast<char*>(self) + cache->p10);
    lbl_eu_80665750 = static_cast<char*>(self) + cache->p18;
    lbl_eu_80665740 = static_cast<char*>(self) + cache->p20;
    lbl_eu_80665748 = reinterpret_cast<LOD::LODCacheIndex*>(static_cast<char*>(self) + cache->p30);
    lbl_eu_80665744 = static_cast<char*>(self) + cache->p28;
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
    u16 n = pair[entry];                     // record count for this pair
    u16* cursor = &pair[entry];

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
    f32 dist = lbl_eu_80665754;

    // Sweep up to the near distance: first element whose near is nonzero.
    if (dist <= (f32)field_0x0) {
        return lbl_eu_80665748[field_0x8].field_0x0 != 0 ? 1 : 0;
    }

    // Sweep past the far distance: last element of the range.
    if (dist >= (f32)field_0x2) {
        return lbl_eu_80665748[field_0x8 + field_0x6 - 1].field_0x0 != 0 ? 1 : 0;
    }

    // Inside the sweep: walk forward until a bin's far cutoff exceeds the
    // key threshold; report whether the preceding element's near is nonzero.
    u16 key = lbl_eu_80665758;
    s32 cnt = field_0x6;
    if (cnt > 1) {
        LODCacheIndex* e = &lbl_eu_80665748[field_0x8 + 1];
        for (s32 j = 1; j < cnt; j++) {
            if (key < e->field_0x2) {
                return (e - 1)->field_0x0 != 0 ? 1 : 0;
            }
            e++;
        }
    }
    return 0;
}