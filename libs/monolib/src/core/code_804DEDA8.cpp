// Auto-scaffolded catalog TU for monolib/src/core/code_804DEDA8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <string.h>

#include "monolib/core/CScheduleItem.hpp"
#include "monolib/core/code_804DEDA8.hpp"
#include "monolib/effect/CERand.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CVec4.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/GX.h>

// Plain 3-float vector POD. Nesting the floats in a nameless struct makes
// MWCC use lwz/stw for struct copies (same trick as ml::CVec3).
struct AnimVec3 {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
};

// Plain 4-float vector POD (same lwz/stw copy trick as AnimVec3).
struct AnimVec4 {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
};

// View of the schedule item used by the anim/entry helpers (func_804E04D4,
// func_804E18CC): f32 at 0x00, byte frame fields at 0x05/0x06, f32s at
// 0x0C/0x10. Overlaps CScheduleItem (same object, different class view).
struct CSchedAnimItem {
    f32 mField00;   // 0x00
    union {         // 0x04: flag byte (MWCC allocates bitfields MSB-first)
        u8 field_0x04; // raw view: bit 0x20/0x40 = non-identity scale, bit 0x80
        struct {
            u8 mFlagBit07 : 1;  // bit 7
            u8 mScaleFlag : 2;  // bits 5-6 (0x60): non-identity scale flag
            u8 mFlagBits04 : 5; // bits 4-0
        };
    };
    u8 mField05;    // 0x05: entry-run start (written by func_804E0188)
    u8 mField06;    // 0x06
    u8 mField07;    // 0x07: entry-run tail count (written by func_804E0188)
    u8 _pad08[4];   // 0x08..0x0B
    union {         // 0x0C..0x30: three anim vectors (also read component-wise
                    // as the flat mFieldNN floats below)
        struct {
            AnimVec3 mVecScale; // 0x0C
            AnimVec3 mVecPos;   // 0x18
            AnimVec3 mVecColor; // 0x24
        };
        struct { // handle view: shared-component pointers (bit-punned)
            f32* mRef0; // 0x0C
            f32* mRef1; // 0x10
            f32* mRef2; // 0x14
        };
        struct { // flat component-wise float view (sequential, 0x0C..0x2C)
            f32 mField0C; // 0x0C
            f32 mField10; // 0x10
            f32 mField14; // 0x14
            f32 mField18; // 0x18
            f32 mField1C; // 0x1C
            f32 mField20; // 0x20
            f32 mField24; // 0x24
            f32 mField28; // 0x28
            f32 mField2C; // 0x2C
        };
        struct { // s16 pair view of the first vector word (slot handles)
            s16 mSlot0C;
            s16 mSlot0E;
        };
        AnimVec4 mQuad0C; // 0x0C: 16-byte quad view spanning +0x0C..+0x1B
        struct { // byte/extended-float view used by func_804E24A8
            u8 mByte0C;       // 0x0C: low byte of the first scale float
            u8 _pad0D[0xF];   // 0x0D..0x1B
            u8 mByte1C;       // 0x1C..0x1F: per-channel color bytes
            u8 mByte1D;
            u8 mByte1E;
            u8 mByte1F;
            u8 _pad20[4];
            u8 mByte24;       // 0x24
            u8 _pad25[3];
            f32 mExt28;       // 0x28 position/color working floats
            f32 mExt2C;
            f32 mExt30;
            f32 mExt34;
            f32 mExt38;
            f32 mExt3C;
        };
    };
};

// View of ml::CVec3::zero through a plain POD so copies emit lwz/stw words.
#define schedZero (*(AnimVec3*)&ml::CVec3::zero)

// View of ml::CVec4::zero through a plain POD so copies emit lwz/stw words.
#define schedZero4 (*(AnimVec4*)&ml::CVec4::zero)

// View of ml::CVec3::unit through a plain POD so copies emit lwz/stw words.
#define schedUnit (*(AnimVec3*)&ml::CVec3::unit)

// 0x14-stride entry element: type tag followed by a 16-byte vector.
struct CEntryWide4Elem {
    u16 mType;      // 0x00 (0x4000 = terminator)
    u8 _pad02[2];
    AnimVec4 mVec;  // 0x04
};

// 0x10-stride entry element: type tag followed by a vector.
struct CEntryVecElem {
    u16 mType;      // 0x00 (0x4000 = terminator)
    u8 _pad02[2];
    AnimVec3 mVec;  // 0x04
};

// 0x28-stride entry element: type tag followed by three vectors.
struct CEntryWideElem {
    u16 mType;      // 0x00 (0x4000 = terminator)
    u8 _pad02[2];
    AnimVec3 mV0;   // 0x04 -> item + 0x0C
    AnimVec3 mV1;   // 0x10 -> item + 0x18
    AnimVec3 mV2;   // 0x1C -> item + 0x24
};

// 8-byte element of the entry array returned by func_804E0248 (f32 at +4).
struct CEntryFloatElem {
    u8 mPad[4];   // 0x00
    f32 mF32;     // 0x04
};

// 8-byte entry element carrying two u16 frame values (read by func_804E0CF0
// and the channel lerp in func_804E0B94).
struct CEntryU16PairElem {
    u8 mPad[4];  // 0x00
    u16 mA;      // 0x04
    u16 mB;      // 0x06
};

// Schedule-resource manager object (static instance at 0x804DF068).
struct CSchedResMgr {
    u8 mField00;    // 0x00
    u8 _pad01;      // 0x01
    u8 mField02;    // 0x02
    u8 mField03;    // 0x03
    void* mField04; // 0x04: MemManager allocation
    void* mField08; // 0x08: heap array (delete[])
    void* mField0C; // 0x0C: heap array (delete[])
    void* mField10; // 0x10
};

// MemManager-glob view (lbl_eu_8065FC18) used by the schedule resource
// manager: flag halfword + allocation handle. Padded past the sdata size
// threshold so MWCC uses the retail lis/@ha + offset addressing (the real
// object is a larger singleton, not sdata).
struct CSchedMemGlob {
    u16 mFlags;    // 0x00: bit 10 = MEM1 allocator active
    u16 mPad02;    // 0x02
    u32 mHandle;   // 0x04
    u8 mPad08[8];  // 0x08..0x10
};
extern CSchedMemGlob lbl_eu_8065FC18;

// .sdata2 float/double pool shared with the schedule anim helpers:
// 1.0f, the u32->double conversion constant (2^52), the s32->double
// conversion constant (2^52 + 2^31), and the frame-rate clamp upper bound.
// const routes these into the readonly sdata2 pool so MWCC hoists the lfs
// above the frame stores (docs/MWCC_CASES.md lfs-hoist scheduling fix).
extern const f32 lbl_eu_8066B290;
extern f64 lbl_eu_8066B298;
extern f64 lbl_eu_8066B2A0;
extern const f64 lbl_eu_8066B288; // u8->float conversion magic (2^52)
extern const f32 lbl_eu_8066B2AC;
extern const f32 lbl_eu_8066B2B0;
extern f32 lbl_eu_8066B2A8;
extern const f32 lbl_eu_8066B2B4;
extern const f32 lbl_eu_8066B2B8;
extern const f32 lbl_eu_8066B2BC;
extern const f32 lbl_eu_8066B2C0;
extern const f32 lbl_eu_8066B2C4;
extern const f32 lbl_eu_8066B2C8;
extern const f32 lbl_eu_8066B2D4;
extern const f32 lbl_eu_8066B2D8;
extern const f32 lbl_eu_8066B2CC;
extern const f32 lbl_eu_8066B2DC;

// s32 -> f32 through the shared signed magic double lbl_eu_8066B2A0 (retail
// xoris + 0x43300000 stack-double shape). Writing it explicitly keeps MWCC
// from pooling its own 2^52 constant for a bare `(f32)(s32)` cast.
// Statement order matters: the value word first, then 0x43300000.
static inline f32 s32ToF32_B2A0(s32 v) {
    union {
        f64 d;
        u32 w[2];
    } c;
    c.w[1] = (u32)v ^ 0x80000000u;
    c.w[0] = 0x43300000u;
    return (f32)(c.d - lbl_eu_8066B2A0);
}

// Import: random unit-ish vector generator (monolib/src/effect/CERand.cpp,
// retail symbol randVec__6CERandFPQ22ml5CVec3).
extern "C" void randVec__6CERandFPQ22ml5CVec3(ml::CVec3* v);

extern void* lbl_eu_80665A44; // .sbss pointer slot (4 bytes)

// Zero one 0x18-byte schedule-resource entry. never_inline so callers (e.g.
// func_804DF344) keep the retail direct `bl` instead of an inlined loop body.
// extern "C" keeps the call-site reloc name matching retail (func_804DEDA8).
// Definition sits below func_804DF344 so MWCC's same-TU IPA cannot inline it
// into the allocation loop at -O4,p.
extern "C" void func_804DEDA8(void* r3);

// Destructor of the static schedule-resource manager (object at 0x804DF068):
// release the two heap arrays and the MemManager block, then zero the object.
// extern "C" keeps the symbol/reloc name matching retail (__dt__804DF068).
// Definition sits below func_804DF4BC so MWCC's same-TU IPA cannot prove it
// leaves the pool globals untouched (retail reloads them per iteration).
extern "C" void __dt__804DF068(CSchedResMgr* obj);

// Lookup-table object shared by func_804DF118 / func_804DF2C4 / func_804DF2F0
// (func_804DF2A8 reads the same object's res-table pointer at +0x10).
struct CResLookupEntry {
    void* mValue; // 0x00
    void* mKey;   // 0x04
};

struct CResLookup {
    u8 _pad00[2];              // 0x00
    u8 mCount;                 // 0x02: number of entries
    u8 _pad03;                 // 0x03
    u32 _pad04;                // 0x04
    CResLookupEntry* mEntries; // 0x08: {value,key} entries, 8-byte stride
    u32* mTable;               // 0x0C: index -> u32 table (func_804DF2C4)
};

// Indexed key accessor: index must be < 0x100 and within the entry count.
// never_inline so func_804DF5F8 keeps the retail direct `bl`; extern "C"
// keeps the call-site reloc name matching retail.
extern "C" DECOMP_DONT_INLINE void* func_804DF118(CResLookup* self, u32 index) {
    if (index >= 0x100) {
        return NULL;
    }
    if (self->mCount <= index) {
        return NULL;
    }
    return self->mEntries[index].mKey;
}

// One 0x18-byte schedule-resource entry. func_804DF5F8 matches the key at
// 0x04; the lookup-table fields (count at 0x02, entry array at 0x08) are read
// through the CResLookup view by func_804DF118. Each element is zeroed by
// func_804DEDA8 at allocation time (func_804DF344).
struct CResEntry {
    u8 mField00;                // 0x00
    u8 mField01;                // 0x01
    u8 mCount;                  // 0x02 (CResLookup view)
    u8 mField03;                // 0x03
    u32 mKey;                   // 0x04 resource key
    CResLookupEntry* mEntries;  // 0x08 (CResLookup view)
    u8 mRest[0x0C];             // 0x0C..0x18
};

extern u8 lbl_eu_806616B8[0x60]; // .bss buffer cleared by func_804DF150

// Zero a 0x60-byte global buffer (tail call to memset).
void func_804DF150() {
    memset(lbl_eu_806616B8, 0, 0x60);
}

// Texture-loader manager: resource object pointer at +0x10 (see code_804DEDA8.hpp).
struct TexLoaderView {
    u8 _00[0x10]; // 0x00
    u8* mResObj;  // 0x10
};

struct TexResObjView {
    u8 _00[8];        // 0x00
    u32 mOffsetTable; // 0x08: byte offset of the TexOfsEntry array
};

struct TexOfsEntryView {
    u32 mOffset; // byte offset of the TexInfo within the res object
    u32 mPad;    // entries are 8 bytes apart
};

// One texture record inside the resource object.
struct TexInfoView {
    u16 mHeight;  // 0x00
    u16 mWidth;   // 0x02
    u32 mFmt;     // 0x04: GXTexFmt
    u32 mImgOfs;  // 0x08: image data byte offset (from the res object)
    u8 _0C[8];    // 0x0C
    u32 mMinFilt; // 0x14: GXTexFilter (min)
    u32 mMagFilt; // 0x18: GXTexFilter (mag)
    f32 mLodBias; // 0x1C
    u8 mEdgeLod;  // 0x20
    u8 mMinLod;   // 0x21
    u8 mMaxLod;   // 0x22
};

// 12-byte last-loaded-texture cache entry (array at lbl_eu_806616B8).
struct TexCacheEntryView {
    void* mMgr; // 0x00
    s32 mKey;   // 0x04
    s32 mWrap;  // 0x08
};

// Load texture `index` of `mgr` into tex map slot `texMap` with wrap mode
// `wrapMode`, unless that exact combination is already the cached last load
// (the cache is indexed by tex map slot).
// 0x4330 magic-double builder for u8 -> float conversion (retail shape).
union LodConv {
    struct {
        u32 mHi;
        u32 mLo;
    };
    f64 mD;
};

void func_804DF164(TexLoaderView* mgr, s32 index, GXTexMapID texMap,
                   GXTexWrapMode wrapMode) {
    TexCacheEntryView* entry =
        (TexCacheEntryView*)(lbl_eu_806616B8 + texMap * sizeof(TexCacheEntryView));
    if (!(entry->mMgr == mgr && index == entry->mKey && wrapMode == entry->mWrap)) {
        TexResObjView* resObj = (TexResObjView*)mgr->mResObj;
        TexOfsEntryView* offsets =
            (TexOfsEntryView*)((u8*)resObj + resObj->mOffsetTable);
        TexInfoView* info = (TexInfoView*)((u8*)resObj + offsets[index].mOffset);
        bool mipmapped = info->mMinLod != info->mMaxLod;
        GXTexObj obj;
        GXInitTexObj(&obj, (u8*)resObj + info->mImgOfs, info->mWidth,
                     info->mHeight, (GXTexFmt)info->mFmt, wrapMode, wrapMode,
                     mipmapped);
        if (mipmapped) {
            LodConv conv;
            conv.mHi = 0x43300000;
            conv.mLo = info->mMinLod;
            f32 minLod = (f32)(conv.mD - lbl_eu_8066B288);
            conv.mLo = info->mMaxLod;
            f32 maxLod = (f32)(conv.mD - lbl_eu_8066B288);
            GXInitTexObjLOD(&obj, (GXTexFilter)info->mMinFilt,
                            (GXTexFilter)info->mMagFilt, minLod, maxLod,
                            info->mLodBias, false, info->mEdgeLod, GX_ANISO_1);
        }
        GXLoadTexObj(&obj, texMap);
        entry->mMgr = mgr;
        entry->mKey = index;
        entry->mWrap = wrapMode;
    }
}

struct ResTableIndex {
    u32 offset;
    u32 size;
};

struct ResTable {
    u8 _00[8];
    u32 indexOffset;
};

void* func_804DF2A8(u8* self, int index) {
    ResTable* base = *(ResTable**)((u8*)self + 0x10);
    ResTableIndex* entries = (ResTableIndex*)((u8*)base + base->indexOffset);
    return (u8*)base + entries[index].offset;
}

// Import from monolib/src/scn/CScn.cpp (retail symbol func_80495EA4, C linkage).
extern "C" int func_80495EA4(int a, int b, int c);

// Look up mTable[index]; a non-zero entry is forwarded to func_80495EA4 along
// with the caller's argument. The -1 third argument is dead at the callee
// (func_80495EA4 hard-codes 8 into r5) but kept for byte-identical codegen.
int func_804DF2C4(CResLookup* self, int a, int index) {
    u32 entry = self->mTable[index];
    if (entry != 0) {
        return func_80495EA4(a, entry, -1);
    }
    return 0;
}

// Search the entry table for a matching key and return its value.
void* func_804DF2F0(CResLookup* self, void* key) {
    if (key != NULL) {
        int count = self->mCount;
        int off;             // byte offset of the current entry
        int i = 0;
        off = 0;
        for (; i < count; i++) {
            CResLookupEntry* entry = (CResLookupEntry*)((u8*)self->mEntries + off);
            if (entry->mKey == key) {
                return *(void**)((u8*)self->mEntries + i * 8);
            }
            off += 8;
        }
    }
    return NULL;
}

// Schedule-resource tables (.sbss): element base, element count and a live
// counter. Entries are 0x18 bytes (CResEntry).
CResEntry* lbl_eu_80665A20;
s32 lbl_eu_80665A24;
s32 lbl_eu_80665A28[2];  // retail object is 8 bytes; only word 0 is written

// Allocate/initialize the schedule-resource table: round the byte size up
// to 32, record base/count, then zero-initialize each 0x18-byte entry.
void* func_804DF344(u32 size, u32 count) {
    u32 off;
    s32 i;
    u32 aligned = ((size + 0x1f) & ~0x1f);
    u32 total = count * 0x18;
    void* end = (u8*)aligned + total;
    lbl_eu_80665A24 = count;
    lbl_eu_80665A20 = (CResEntry*)aligned;
    lbl_eu_80665A28[0] = 0;
    off = 0;
    for (i = 0; i < (s32)count; i++) {
        func_804DEDA8((u8*)lbl_eu_80665A20 + off);
        off += 0x18;
    }
    return end;
}

#pragma push
#pragma auto_inline off
extern "C" void func_804DEDA8(void* r3) {
    char* base = (char*)r3;
    base[0] = 0;
    base[2] = 0;
    base[3] = 0;
    *(int*)(base + 4) = 0;
    *(int*)(base + 8) = 0;
    *(int*)(base + 0x10) = 0;
    *(int*)(base + 0xc) = 0;
}
#pragma pop

// Element destructor placeholder: retail callers test a nonzero
// "destroyed" result. extern "C" keeps the symbol/reloc name matching
// retail (__dt__804DEDCC); definition sits below func_804DF3D0 so MWCC's
// same-TU IPA cannot inline it into the free-slot scan.
// Resource-chain manager (retail symbol __dt__804DEDCC): allocates the
// entry's node/index arrays from the resource descriptor's count bytes,
// walks the chunked resource chain at entry+0x20 building free lists,
// then conditionally tears down. Returns 0 only on full teardown.
extern "C" DECOMP_DONT_INLINE int __dt__804DEDCC(CResEntry* entry, void* res,
                                                  u32 flagBit);

// Find the schedule-resource entry whose key matches `key`; if no live
// entry matches and the pool still has capacity, claim the first free
// (key == 0) entry instead. Returns 1 when an entry was claimed/found.
s32 func_804DF3D0(u32 key) {
    CResEntry* p = lbl_eu_80665A20;
    s32 n = lbl_eu_80665A24;
    u8 found = 0;
    if (n > 0) {
        CResEntry* q = p;
        s32 c = n;
        do {
            if (q->mKey != 0 && key == q->mKey) {
                found = 1;
                break;
            }
            q++;
        } while (--c);
    }
    if (found != 0) {
        return 1;
    }
    if (n <= lbl_eu_80665A28[0]) {
        return 0;
    }
    // bool-normalize the MEM1-active flag via the neg/or/srwi idiom
    s32 flagBit = (lbl_eu_8065FC18.mFlags >> 12) & 1;
    flagBit = (u32)(-flagBit | flagBit) >> 31;
    if (n > 0) {
        CResEntry* q = p;
        s32 c = n;
        do {
            if (q->mKey == 0) {
                if (__dt__804DEDCC(q, (void*)key, (u32)flagBit) != 0) {
                    lbl_eu_80665A28[0]++;
                    return 1;
                }
                return 0;
            }
            q++;
        } while (--c);
    }
    return 0;
}

extern "C" void __dla__FPv(void*);
#pragma push
#pragma auto_inline off
// Working view of the entry header used while walking the chain.
struct DedccHdr {
    u8 b0;    // +0x00
    u8 b1;    // +0x01 (bit7 = flagBit)
    u8 b2;    // +0x02 node-array count
    u8 b3;    // +0x03 index-array count
    u32 w4;   // +0x04 resource ptr
    void* p8; // +0x08 node array
    void* pc; // +0x0C index array
    void* p10; // +0x10
};
extern "C" int __dt__804DEDCC(CResEntry* entry, void* res, u32 flagBit) {
    DedccHdr* e = (DedccHdr*)entry;
    if (res == NULL) {
        return 0;
    }
    if (*(u8*)((u8*)res + 6) != 0) {
        u32 max = mtl::MemManager::getMaxAllocSize(
            *(u32*)((u8*)&lbl_eu_8065FC18 + 4));
        u32 size = *(u8*)((u8*)res + 6) << 3;
        void* arr;
        if (size > max) {
            arr = NULL;
        } else {
            arr = mtl::MemManager::allocate_array(size,
                                                  *(u32*)((u8*)&lbl_eu_8065FC18 + 4));
        }
        e->p8 = arr;
    }
    if (*(u8*)((u8*)res + 7) != 0) {
        u32 max = mtl::MemManager::getMaxAllocSize(
            *(u32*)((u8*)&lbl_eu_8065FC18 + 4));
        u32 size = *(u8*)((u8*)res + 7) << 2;
        void* arr;
        if (size > max) {
            arr = NULL;
        } else {
            arr = mtl::MemManager::allocate_array(size,
                                                  *(u32*)((u8*)&lbl_eu_8065FC18 + 4));
        }
        e->pc = arr;
    }
    u8* chunk = (u8*)e + 0x20;
    for (;;) {
        u16 type = *(u16*)(chunk + 4);
        if (type == 0) {
            break;
        }
        if (type == 3) {
            if (e->p8 != NULL) {
                u32 idx = e->b2;
                *(void**)((u8*)e->p8 + idx * 8) = chunk + 8;
                *(void**)((u8*)e->p8 + idx * 8 + 4) = chunk + 0x20;
                e->b2++;
            }
            chunk += *(u32*)chunk;
        } else if (type >= 5) {
            continue;
        } else if (type == 1) {
            e->p10 = chunk + 0x20;
            chunk += *(u32*)chunk;
        } else {
            if (e->pc != NULL) {
                u32 idx = e->b3;
                *(void**)((u8*)e->pc + idx * 4) = chunk + 0x20;
                e->b3++;
            }
            u8* next = chunk + *(u32*)chunk;
            if (*(u16*)(next + 4) == 5) {
                chunk = next;
            }
            chunk += *(u32*)chunk;
        }
    }
    u8 done = (e->b2 != 0);
    e->w4 = (u32)res;
    e->b0 = 0;
    e->b1 |= (u8)((flagBit << 7) & 0x80);
    if (!done && e->p10 == NULL && e->b3 == 0) {
        if (e->p8 != NULL) {
            __dla__FPv(e->p8);
            e->p8 = NULL;
        }
        if (e->pc != NULL) {
            __dla__FPv(e->pc);
            e->pc = NULL;
        }
        u32 memActive =
            (*(volatile CSchedMemGlob*)&lbl_eu_8065FC18).mFlags;
        memActive = (memActive >> 10) & 1;
        if (memActive != 0) {
            if (e->w4 != 0) {
                mtl::MemManager::deallocate((void*)e->w4);
            }
        }
        e->w4 = 0;
        e->b0 = 0;
        e->b2 = 0;
        e->b3 = 0;
        return 0;
    }
    return 1;
}
#pragma pop

// Release schedule-resource entries (retail func_804DF4BC):
// - key != 0: scan the entry array (counted do-while over the base pointer)
//   for a live entry whose key matches and whose 0x01 bit-6 flag equals
//   mFlags bit 11; on a match run the manager dtor over it, drop the live
//   counter when positive, return 1.
// - key == 0: sweep every live entry whose bit-6 flag matches, running the
//   dtor + counter drop on each, then return 1.
// Returns 0 only when the keyed scan found nothing.
extern "C" s32 func_804DF4BC(u32 key) {
    // bool-normalize via the neg/or/srwi idiom (MWCC_CASES: (u32)(-x|x)>>31,
    // neg operand FIRST, (u32) cast -> logical srwi).
    s32 flagBit = (lbl_eu_8065FC18.mFlags >> 12) & 1;
    flagBit = (u32)(-flagBit | flagBit) >> 31;
    if (key != 0) {
        CResEntry* p = lbl_eu_80665A20;
        for (s32 j = 0; j < lbl_eu_80665A24; j++, p++) {
            if (p->mKey != 0 && key == p->mKey &&
                (u32)(-((p->mField01 >> 7) & 1) |
                      ((p->mField01 >> 7) & 1)) >>
                        31 ==
                    flagBit) {
                __dt__804DF068((CSchedResMgr*)p);
                if (lbl_eu_80665A28[0] > 0) {
                    lbl_eu_80665A28[0]--;
                }
                return 1;
            }
        }
        return 0;
    }
    {
        CResEntry* base = lbl_eu_80665A20;
        for (s32 i = 0; i < lbl_eu_80665A24; i++) {
            CResEntry* e = &base[i];
            if (e->mKey != 0 &&
                (u32)(-((e->mField01 >> 7) & 1) |
                      ((e->mField01 >> 7) & 1)) >>
                        31 ==
                    flagBit) {
                __dt__804DF068((CSchedResMgr*)e);
                if (lbl_eu_80665A28[0] > 0) {
                    lbl_eu_80665A28[0]--;
                }
            }
        }
    }
    return 1;
}

// Definition moved below func_804DF4BC (see prototype above).
#pragma push
#pragma auto_inline off
extern "C" void __dt__804DF068(CSchedResMgr* obj) {
    if (obj->mField08 != 0) {
        delete[] (u8*)obj->mField08;
        obj->mField08 = 0;
    }
    if (obj->mField0C != 0) {
        delete[] (u8*)obj->mField0C;
        obj->mField0C = 0;
    }
    if ((lbl_eu_8065FC18.mFlags >> 10) & 1) {
        if (obj->mField04 != 0) {
            mtl::MemManager::deallocate(obj->mField04);
            obj->mField04 = 0;
        }
    } else {
        obj->mField04 = 0;
    }
    obj->mField00 = 0;
    obj->mField02 = 0;
    obj->mField03 = 0;
    obj->mField04 = 0;
    obj->mField08 = 0;
    obj->mField10 = 0;
    obj->mField0C = 0;
}
#pragma pop

// Look up a schedule-resource entry by key; resolve its value through the
// entry's lookup table (func_804DF118) and report the entry/value pointers.
// Returns 1 on a match, 0 otherwise.
s32 func_804DF5F8(CResEntry** outEntry, u32* outValue, u32 key, u32 index) {
    s32 i = 0;
    while (i < lbl_eu_80665A24) {
        CResEntry* entry = &lbl_eu_80665A20[i];
        if (entry->mKey != 0 && key == entry->mKey) {
            u32 v = (u32)func_804DF118((CResLookup*)entry, index);
            *outValue = v;
            if (v != 0) {
                *outEntry = entry;
                return 1;
            }
        }
        i++;
    }
    return 0;
}

// Schedule-resource pool state (.sbss): live counter, the 0x14-byte element
// array, the 0xC-byte node array and the element count.
//
// 0x14-stride schedule element: per-key record holding a linked list of
// 0xC-stride nodes (func_804DF808 builds/traverses these lists).
struct CSchedNodeC;
struct CSchedElem14 {
    u8 _00[4];          // 0x00
    CSchedNodeC* mList; // 0x04: head of this entry's node list
    f32 mPrio;          // 0x08
    u32 mVal;           // 0x0C: key (0 = free slot)
    s16 mCount;         // 0x10
    u8 _12[2];
};

// 0xC-stride list node: next pointer plus the two values sorted on.
struct CSchedNodeC {
    CSchedNodeC* mNext; // 0x00
    void* mKey;         // 0x04: owner object pointer (sorted ascending)
    void* mObj;         // 0x08
};

// Effect object carrying the priority byte at +0x58 used by func_804DF808's
// sorted insertion.
struct CSchedFxObjView {
    u8 _00[0x58];
    u8 mKind58; // 0x58
};

void* lbl_eu_80665A30;
CSchedElem14* lbl_eu_80665A34;
CSchedNodeC* lbl_eu_80665A38;
u32 lbl_eu_80665A3C;
u32 lbl_eu_80665A40;

// (Re)allocate the schedule-resource element arrays: 0x14-byte and 0xC-byte
// tables sized to `count`, each allocated from the MemManager handle when it
// fits the region's max alloc size (else NULL). Records the count in
// lbl_eu_80665A3C and clears the live counter first.
extern "C" void func_804DF690(u32 count) {
    lbl_eu_80665A30 = 0;
    u32 s1 = count * 0x14;
    if (s1 <= mtl::MemManager::getMaxAllocSize(lbl_eu_8065FC18.mHandle)) {
        lbl_eu_80665A34 =
            (CSchedElem14*)mtl::MemManager::allocate_array(s1, lbl_eu_8065FC18.mHandle);
    } else {
        lbl_eu_80665A34 = NULL;
    }
    u32 s2 = count * 0xc;
    if (s2 <= mtl::MemManager::getMaxAllocSize(lbl_eu_8065FC18.mHandle)) {
        lbl_eu_80665A38 =
            (CSchedNodeC*)mtl::MemManager::allocate_array(s2, lbl_eu_8065FC18.mHandle);
    } else {
        lbl_eu_80665A38 = NULL;
    }
    lbl_eu_80665A3C = count;
}

void __dla__FPv(void*);

// Release the schedule-resource arrays (element dtor).
void __dt__804DF744() {
    CSchedElem14* p1 = lbl_eu_80665A34;
    lbl_eu_80665A30 = 0;
    if (p1) {
        __dla__FPv(p1);
        lbl_eu_80665A34 = NULL;
    }
    CSchedNodeC* p2 = lbl_eu_80665A38;
    if (p2) {
        __dla__FPv(p2);
        lbl_eu_80665A38 = NULL;
    }
    lbl_eu_80665A3C = 0;
}

// Effect object pool at lbl_eu_80661728, looked up by handle via func_804DFEAC.
// CEffectObj in CScheduleItem.hpp is a partial view of the 0x22C-byte elements.
struct CEffectObject {
    u8 mData[0x22C];
};
struct CEffectPool {
    CEffectObject* mBase;  // 0x00: effect object array
    s32 mCount;            // 0x04: number of entries
    s32 mActiveCount;      // 0x08: currently active objects
    s32 mLastHandle;       // 0x0C: most recently released handle
};
extern CEffectPool lbl_eu_80661728;

// Effect-node pool at lbl_eu_80661738 (node stride 0x338), looked up via
// func_804E0114(index). Cross-TU callers treat nodes as EffectNode*.
struct CEffectNode {
    s16 mMark;        // 0x00: allocation mark; < 0 marks the slot free
    u8 mData[0x336];  // 0x02..0x338
};
struct CEffectNodePool {
    CEffectNode* mBase;    // 0x00: node array
    s32 mCount;            // 0x04: number of entries
    s32 mField08;          // 0x08
    s32 mLastHandle;       // 0x0C: most recently released handle
};
extern CEffectNodePool lbl_eu_80661738;

// Effect-manager release (monolib/src/effect/code_804C8718.cpp).
extern "C" void func_804C8790(CEffectObj* obj);

// Effect-object constructor (monolib/src/effect/code_804C8718.cpp).
extern "C" s32 func_804C8830(CEffectObj* self, const u8* base, const u8* data,
                             u8* arg4, s32 arg5, u8 arg6);

// Re-initialize the schedule resource tables: clear both element arrays and
// reset the pool counters.
void func_804DF7A4() {
    lbl_eu_80665A30 = 0;
    memset(lbl_eu_80665A34, 0, (u32)lbl_eu_80665A3C * 0x14);
    memset(lbl_eu_80665A38, 0, (u32)lbl_eu_80665A3C * 0xC);
    lbl_eu_80665A40 = 0;
}

void func_804DF7FC() { lbl_eu_80665A30 = 0; }

// Claim or refresh a schedule-resource element for `key`:
// - scan the 0x14-byte element array for an entry whose key matches; if none
//   matches, claim the first free (key == 0) entry instead.
// - the claimed/matched entry gets a fresh node (from the 0xC-byte node pool
//   at the live-counter slot) pushed onto the appropriate sorted linked list
//   and the live counter is bumped.
// `obj` carries a priority byte at +0x58: entries with kind 0x40 are ordered
// by the float priority, all others by the signed kind byte.
void func_804DF808(u32 key, CSchedFxObjView* obj, void* owner, f32 prio) {
    s32 count = lbl_eu_80665A3C;
    s32 i;
    bool claimed = false;
    for (i = 0; i < count; i++) {
        u32 v = lbl_eu_80665A34[i].mVal;
        if (v == key) {
            break;
        }
        if (v == 0) {
            claimed = true;
            break;
        }
    }
    if (i >= count) {
        return;
    }
    CSchedElem14* elem = &lbl_eu_80665A34[i];
    s32 n = lbl_eu_80665A40;
    CSchedNodeC* node = &lbl_eu_80665A38[n];
    if (!claimed) {
        // Existing entry: insert the new node into the entry's own list,
        // ordered by owner pointer.
        CSchedNodeC* prev = NULL;
        CSchedNodeC* cur = elem->mList;
        while (cur != NULL) {
            if ((s32)obj < (s32)cur->mKey) {
                break;
            }
            prev = cur;
            cur = cur->mNext;
        }
        node->mNext = NULL;
        node->mKey = obj;
        node->mObj = owner;
        if (prev != NULL) {
            node->mNext = prev->mNext;
            prev->mNext = node;
        } else {
            node->mNext = elem->mList;
            elem->mList = node;
        }
        if (((*(volatile u16*)((u8*)owner + 0x1C)) >> 4) & 1) {
            elem->mCount++;
        }
    } else {
        // Free entry: bind it to this key and insert the node into the
        // global priority list rooted at lbl_eu_80665A30.
        elem->mList = node;
        elem->mPrio = prio;
        elem->mVal = (u32)obj;
        elem->mCount = ((*(volatile u16*)((u8*)owner + 0x1C)) >> 4) & 1;
        node->mNext = NULL;
        node->mKey = (void*)(u32)i;
        node->mObj = owner;
        CSchedNodeC* prev = NULL;
        CSchedNodeC* cur = (CSchedNodeC*)lbl_eu_80665A30;
        while (cur != NULL) {
            CSchedFxObjView* curo = (CSchedFxObjView*)cur->mObj;
            if (obj->mKind58 == 0x40 && curo->mKind58 == 0x40) {
                if (prio > *(f32*)&cur->mObj) {
                    break;
                }
            } else {
                if ((s8)obj->mKind58 < (s8)curo->mKind58) {
                    break;
                }
            }
            prev = cur;
            cur = cur->mNext;
        }
        if (prev != NULL) {
            node->mNext = prev->mNext;
            prev->mNext = node;
        } else {
            node->mNext = (CSchedNodeC*)lbl_eu_80665A30;
            lbl_eu_80665A30 = node;
        }
    }
    lbl_eu_80665A40 = n + 1;
}

// Import from monolib/src/core/code_804E36DC.cpp (retail symbol func_804E3D88):
// initialize one pool item (all slots closed).
extern "C" void func_804E3D88(CScheduleItem* item);

// Allocate/initialize the schedule-item pool: round the byte size up to 32,
// record base/count, then run the item initializer over each 0x58-byte item.
void* func_804DFA08(u32 size, s32 count) {
    s32 i;
    u32 aligned = (size + 0x1f) & ~0x1f;
    lbl_eu_80661718.base = (CScheduleItem*)aligned;
    lbl_eu_80661718.count = count;
    lbl_eu_80661718.freeCount = 0;
    lbl_eu_80661718.lastHandle = 0;
    i = 0;
    s32 off = 0;
    while (i < count) {
        func_804E3D88((CScheduleItem*)((u8*)lbl_eu_80661718.base + off));
        off += 0x58;
        i++;
    }
    return (u8*)aligned + count * 0x58;
}

// Entry-blob resolver for allocated schedule items (retail symbol
// func_804E3EB4, monolib/src/core/code_804E36DC.cpp).
extern "C" s32 func_804E3EB4(CScheduleItem* item, u32 a, u32 b, u32 c);

// Allocate a free schedule item from the pool ring for (a, b, c): starting
// past the most recently released handle, claim the first slot whose
// mPoolIndex is negative, re-initialize it, stamp its pool index and
// advance the handle cursor. Returns its index, or -1 when the pool is
// full / fully scanned / entry resolution fails.
s32 func_804DFA84(u32 a, u32 b, u32 c) {
    CScheduleItem* node = NULL;
    if (lbl_eu_80661718.freeCount < lbl_eu_80661718.count) {
        s32 last = lbl_eu_80661718.lastHandle;
        s32 idx = last;
        do {
            node = &lbl_eu_80661718.base[idx];
            if (node->mPoolIndex >= 0) {
                idx = (idx + 1) % lbl_eu_80661718.count;
            } else {
                func_804E3D88(node);
                node->mPoolIndex = (s16)idx;
                lbl_eu_80661718.freeCount++;
                lbl_eu_80661718.lastHandle =
                    (idx + 1) % lbl_eu_80661718.count;
                break;
            }
        } while (idx != last);
    }
    if (node == NULL) {
        return -1;
    }
    if (func_804E3EB4(node, a, b, c) != 0) {
        return node->mPoolIndex;
    }
    return -1;
}

// Release a schedule item by handle (idempotent for invalid handles).
// NOTE: retail keeps a duplicated `handle < 0` test (two blt from one cmpwi);
// the `!(handle < 0)` conjunct preserves it (same shape as func_804DFE20).
extern "C" void func_804DFB88(s16 handle) {
    if (handle < 0) {
        return;
    }
    if (!(handle < 0) && lbl_eu_80661718.count > handle) {
        lbl_eu_80661718.lastHandle = handle;
        func_804E3E2C(&lbl_eu_80661718.base[handle]);
        if (lbl_eu_80661718.freeCount > 0) {
            lbl_eu_80661718.freeCount--;
        }
    }
}

// Look up a schedule item by handle; returns NULL for invalid handles.
extern "C" CScheduleItem* func_804DFBF4(s16 handle) {
    if (handle < 0) {
        return NULL;
    }
    bool inRange = handle >= 0 && handle < lbl_eu_80661718.count;
    if (inRange) {
        return &lbl_eu_80661718.base[handle];
    }
    return NULL;
}

// Import from monolib/src/effect/code_804C8718.cpp (retail symbol
// func_804C8718): effect-manager initializer, run per pool element.
extern "C" void func_804C8718(CEffectObj* obj);

// Allocate/initialize the effect-object pool: round the byte size up to 32,
// record base/count, then run the effect-manager init over each 0x22C-byte
// object.
void* func_804DFC48(u32 size, s32 count) {
    s32 i;
    u32 aligned = (size + 0x1f) & ~0x1f;
    lbl_eu_80661728.mBase = (CEffectObject*)aligned;
    lbl_eu_80661728.mCount = count;
    lbl_eu_80661728.mActiveCount = 0;
    lbl_eu_80661728.mLastHandle = 0;
    i = 0;
    s32 off = 0;
    while (i < count) {
        func_804C8718((CEffectObj*)((u8*)lbl_eu_80661728.mBase + off));
        off += 0x22c;
        i++;
    }
    return (u8*)aligned + count * 0x22c;
}

// Claim a free effect object from the pool ring for the given init args:
// starting past the most recently released handle, claim the first slot whose
// pool index (s16 at +0x1E) is negative, re-initialize it, stamp its index and
// advance the handle cursor. Runs the effect constructor on the claimed
// object; on constructor failure the object is released again and the handle
// cursor rewound. Returns the object's pool index, or -1 when the pool is
// full or the constructor failed.
s32 func_804DFCC4(const u8* base, const u8* data, u8* arg3, s32 arg4,
                  u8 arg5) {
    CEffectObj* node = NULL;
    if (lbl_eu_80661728.mActiveCount < lbl_eu_80661728.mCount) {
        s32 last = lbl_eu_80661728.mLastHandle;
        s32 idx = last;
        do {
            node = (CEffectObj*)((u8*)lbl_eu_80661728.mBase + idx * 0x22c);
            if (*(s16*)((u8*)node + 0x1e) >= 0) {
                idx = (idx + 1) % lbl_eu_80661728.mCount;
            } else {
                func_804C8718(node);
                *(s16*)((u8*)node + 0x1e) = (s16)idx;
                lbl_eu_80661728.mActiveCount++;
                lbl_eu_80661728.mLastHandle =
                    (idx + 1) % lbl_eu_80661728.mCount;
                break;
            }
        } while (idx != last);
    }
    if (node == NULL) {
        return -1;
    }
    if (func_804C8830(node, base, data, arg3, arg4, arg5) != 0) {
        return *(s16*)((u8*)node + 0x1e);
    }
    // Constructor failed: rewind the handle cursor and release the object.
    s16 h = *(s16*)((u8*)node + 0x1e);
    if (h >= 0 && h < lbl_eu_80661728.mCount) {
        lbl_eu_80661728.mLastHandle = h;
        func_804C8790((CEffectObj*)((u8*)lbl_eu_80661728.mBase + h * 0x22c));
        if (lbl_eu_80661728.mActiveCount > 0) {
            lbl_eu_80661728.mActiveCount--;
        }
    }
    return -1;
}

// Release an effect object handle: remember it, run the effect-manager
// release, and drop the active-object count.
void func_804DFE20(s16 handle) {
    if (handle < 0) {
        return;
    }
    if (!(handle < 0) && lbl_eu_80661728.mCount > handle) {
        lbl_eu_80661728.mLastHandle = handle;
        func_804C8790((CEffectObj*)&lbl_eu_80661728.mBase[handle]);
        if (lbl_eu_80661728.mActiveCount > 0) {
            lbl_eu_80661728.mActiveCount--;
        }
    }
}

u32 func_804DFE8C() { return lbl_eu_80661728.mActiveCount; }

u32 func_804DFE9C() { return lbl_eu_80661728.mCount; }

// Look up an effect object by handle; NULL for out-of-range handles.
CEffectObj* func_804DFEAC(s16 handle) {
    if (handle < 0) {
        return NULL;
    }
    bool inRange = handle >= 0 && handle < lbl_eu_80661728.mCount;
    if (inRange) {
        return (CEffectObj*)&lbl_eu_80661728.mBase[handle];
    }
    return NULL;
}

// Effect-node pool element initializer (monolib/src/effect/code_804CC2B8.cpp,
// retail symbol func_804CC2B8). extern "C" keeps the call-site reloc name
// matching retail.
extern "C" void func_804CC2B8(CEffectNode* node);

// Allocate the effect-node pool (stride 0x338) plus a follow-on 0x34-per-item
// table: round the byte size up twice (32-byte granularity), record the pool
// base/count, run the node initializer over each element, then zero the
// follow-on table (returning its end pointer).
extern "C" void* func_804DFF00(u32 size, u32 count) {
    u32 a = (size + 0x1f) & ~0x1f;
    lbl_eu_80661738.mCount = count;
    u32 base = (a + 0x1f) & ~0x1f;
    lbl_eu_80661738.mBase = (CEffectNode*)base;
    lbl_eu_80661738.mField08 = 0;
    lbl_eu_80661738.mLastHandle = 0;
    // Retail keeps both a per-element byte cursor and the loop index.
    u32 off = 0;
    s32 i;
    for (i = 0; i < (s32)count; i++) {
        func_804CC2B8((CEffectNode*)((u8*)lbl_eu_80661738.mBase + off));
        off += 0x338;
    }
    u32 tail = (base + count * 0x338 + 0x1f) & ~0x1f;
    lbl_eu_80665A44 = (void*)tail;
    memset(lbl_eu_80665A44, 0, count * 0x34);
    return (void*)((u8*)lbl_eu_80665A44 + count * 0x34);
}

// Effect-node activator (monolib/src/effect/code_804CC2B8.cpp,
// retail symbol func_804CC3A4).
extern "C" void func_804CC3A4(CEffectNode* node, CScheduleItem* item);

// Allocate a free effect node from the pool ring: starting past the most
// recently released handle, claim the first slot whose mark is negative
// (free), re-initialize it, stamp it with its index and advance the handle
// cursor. Returns its mark, or -1 when the pool is full / fully scanned.
s32 func_804DFFA8(CScheduleItem* item) {
    CEffectNode* node = NULL;
    if (lbl_eu_80661738.mField08 < lbl_eu_80661738.mCount) {
        s32 last = lbl_eu_80661738.mLastHandle;
        s32 idx = last;
        do {
            node = &lbl_eu_80661738.mBase[idx];
            if (node->mMark >= 0) {
                idx = (idx + 1) % lbl_eu_80661738.mCount;
            } else {
                func_804CC2B8(node);
                node->mMark = (s16)idx;
                lbl_eu_80661738.mField08++;
                lbl_eu_80661738.mLastHandle =
                    (idx + 1) % lbl_eu_80661738.mCount;
                break;
            }
        } while (idx != last);
    }
    if (node == NULL) {
        return -1;
    }
    func_804CC3A4(node, item);
    return node->mMark;
}

// Import: effect-node pool element destructor (monolib/src/effect/
// code_804CC2B8.cpp, retail symbol __dt__804CC2E4). The node class is not
// decompiled yet; the element is passed as its opaque 0x338-byte view.
extern "C" void __dt__804CC2E4(CEffectNode* node);

// Release an effect node by handle (idempotent for invalid handles).
// NOTE: retail has `cmpwi r3,0; blt; blt` (two branches from one compare)
// before the pool-count test; MWCC merges the duplicated `handle < 0`
// disjuncts to `handle <= -1` under the unit's -O4,p, so this shape keeps
// the byte count but the branch test remains `cmpwi r3,-1; ble` (open item).
void func_804E0098(s16 handle) {
    if (handle < 0) {
        return;
    }
    if (!(handle < 0) && lbl_eu_80661738.mCount > handle) {
        lbl_eu_80661738.mLastHandle = handle;
        __dt__804CC2E4((CEffectNode*)&lbl_eu_80661738.mBase[handle]);
        if (lbl_eu_80661738.mField08 > 0) {
            lbl_eu_80661738.mField08--;
        }
    }
}

u32 func_804E0104() { return lbl_eu_80661738.mCount; }

// Look up an effect node by index; NULL for out-of-range indices.
CEffectNode* func_804E0114(s32 index) {
    if (index < 0) {
        return NULL;
    }
    bool inRange = index >= 0 && index < lbl_eu_80661738.mCount;
    if (inRange) {
        return &lbl_eu_80661738.mBase[index];
    }
    return NULL;
}

extern void* lbl_eu_80665A44; // .sbss pointer slot (4 bytes)

// Indexed handle lookup: returns the sbss pointer-slot value advanced by
// arg*0x34 (retail folds the constant slot address into an sda21 load).
void* func_804E0168(s32 arg) {
    if (arg < 0) {
        return NULL;
    }
    s32 off = arg * 0x34;
    return (void*)(*(u32*)&lbl_eu_80665A44 + off);
}

// Pick a random entry from a run of equal u16 values in a schedule entry
// blob. `base` holds u16 items `stride` bytes apart; the scan starts at
// `start` and continues while items equal the first (0x4000 is the
// terminator that ends the scan). When the run is longer than one item a
// random item is chosen: *out1 = start + rand, *out2 = run len - rand, and
// that item's pointer is returned; a single-item run stores start / 1.
extern "C" u8* func_804E0188(u8* base, u32 stride, s32 start, u8* out1, u8* out2) {
    s32 i;
    u16 first = *(u16*)(base + stride * start);
    u8* result = base + stride * start;
    if (first != 0x4000) {
        s32 count = 1;
        i = start + 1;
        u8* p = base + stride * i;
        u16 v;
        while ((v = *(u16*)p) != 0x4000 && v == first) {
            i++;
            count++;
            p = base + stride * i;
        }
        if (count > 1) {
            s32 r = ml::math::mtRand(count);
            *out1 = (u8)(start + r);
            result = base + stride * (start + r);
            *out2 = (u8)(count - r);
        } else {
            *out1 = (u8)start;
            *out2 = 1;
        }
    }
    return result;
}

// Entry-blob element accessed by func_804E0248 (s16 fields at +4/+6).
struct CEntryElem {
    u16 mField00;    // 0x00: element type (0x4000 = terminator)
    s16 mField02;    // 0x02
    s16 mField04;    // 0x04
    s16 mField06;    // 0x06
    u8 mRest[0x0E];  // 0x08
};

// Entry-blob interpreter (retail symbol func_804E0248): resolves the element
// for an anim item by walking the blob with func_804E0188. Guards on the
// item flag byte (bit 7 armed, bit 3 clear), honors each candidate's u16
// limit against mField00, applies the per-object channel mask at obj+0x59,
// quantizes mField00 modulo the chosen element's limit, and advances
// mField00 by the caller rate. Returns the resolved element pointer.
extern "C" CEntryElem* func_804E0248(CScheduleItem* itemArg, CEntryElem* entries,
                                     s32 index, u8* arg4, f32 f1) {
    u8* obj = arg4;
    CSchedAnimItem* item = (CSchedAnimItem*)itemArg;
    u8* base = (u8*)entries;
    u32 stride = (u32)index;
    if (!((item->field_0x04 >> 7) & 1)) {
        return NULL;
    }
    if ((item->field_0x04 >> 3) & 1) {
        return NULL;
    }
    u8 first = 1;
    for (;;) {
        u8 start = (u8)(item->mField05 + item->mField07);
        u8* ret = func_804E0188(base, stride, start, &item->mField06, &first);
        u16 lim = *(u16*)ret;
        if (lim == 0x4000) {
            item->field_0x04 |= 8;
            break;
        }
        f32 conv = (f32)lim;
        u8 useMask = 0;
        if (conv <= item->mField00) {
            s8 r9 = (s8)((u8*)ret)[2];
            if (r9 >= 0) {
                u8 r8 = ((u8*)ret)[3];
                u8 take = 1;
                if (r8 != 0) {
                    s8 m = (s8)*(u8*)(obj + 0x59);
                    u8 hit = 0;
                    if (m != 0) {
                        if ((r8 & (1 << (m - 1))) != 0) {
                            hit = 1;
                        }
                    }
                    if (hit != 0) {
                        take = 0;
                    }
                }
                if (take == 0) {
                    useMask = 1;
                }
            }
            if (useMask != 0) {
                // accepted: quantize and loop again
                item->mField05 = (u8)r9;
                u8* ret2 = func_804E0188(base, stride, ((u8*)ret)[2],
                                         &item->mField05, &item->mField07);
                u16 lim2 = *(u16*)ret2;
                s32 qi = (s32)item->mField00;
                item->mField00 = (f32)(lim2 + (qi % lim2));
                continue;
            }
        }
        // reject path: only proceed when the limit fits mField00 and rate >= 1
        if (!(conv <= item->mField00)) {
            break;
        }
        if (!(lbl_eu_8066B290 <= f1)) {
            break;
        }
        item->mField05 = item->mField06;
        item->mField07 = first;
        u8* ret3 = func_804E0188(
            base, stride, (u8)(item->mField06 + first), &item->mField06,
            &item->mField07);
        u8 b3 = ((u8*)ret3)[3];
        if (b3 == 0xFF) {
            *(u16*)(obj + 0) |= 0x40;
        } else if ((b3 & 0x80) != 0) {
            u32 i = 0;
            do {
                i++;
            } while ((((s32)b3) >> i) & 1);
            *(u8*)(obj + 0x59) = (u8)(i + 1);
        }
        break;
    }
    u32 off = stride * item->mField05;
    item->mField00 += f1;
    return (CEntryElem*)(base + off);
}

// Paired-single vector lerp kernel (monolib/src/effect/code_804DB938.cpp,
// retail symbol func_804DD89C): out = a + (b - a) * t for three f32s.
extern "C" void func_804DD89C(void* out, const void* a, const void* b, f32 t);

// Frame-span rate helper: reads the u16 frame values at entries[index*span],
// converts them to float (via the 0x43300000 magic-double bit pattern so the
// 2^52 adjustors reference the retail pool labels, docs/MWCC_CASES.md §7i),
// and clamps the result into [1.0, lbl_eu_8066B2A8]. A 0x4000 entry
// (terminator) yields rate 1.0.
extern "C" f32 func_804E04D4(CSchedAnimItem* item, CEntryElem* entries, s32 index) {
    f32 result = lbl_eu_8066B290;
    u8 a = item->mField06;
    u8 b = item->mField05;
    u16 hi = *(u16*)((u8*)entries + index * a);
    if (hi == 0x4000) {
        return result;
    }
    u16 lo = *(u16*)((u8*)entries + index * b);
    s32 diff = (s32)hi - (s32)lo;
    // builtin int->float casts: MWCC emits its single-rounded fsubs idiom
    // against the 0x4330 magic constants (retail shape)
    f32 flo = lo;
    f32 fdiff = diff;
    f32 f = item->mField00 - flo;
    f32 d = fdiff;
    if (d == result) {
        d = lbl_eu_8066B2A8;
    }
    f32 rate = lbl_eu_8066B2A8 / d;
    if (f == result) {
        return result;
    }
    f32 r = f * rate;
    if (r < lbl_eu_8066B290) {
        r = lbl_eu_8066B290;
    } else if (r > lbl_eu_8066B2A8) {
        r = lbl_eu_8066B2A8;
    }
    return r;
}

// Initialize one anim item from a 0x28-stride entry blob: reset the frame
// state and copy the selected element's three vectors into item+0x0C/0x18/0x24
// (all zeros when no blob), then run the usual terminator flag check.
void func_804E0580(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base == NULL) {
        item->mVecColor = schedZero;
        item->mVecPos = schedZero;
        item->mVecScale = schedZero;
        return;
    }
    CEntryWideElem* ret = (CEntryWideElem*)func_804E0188(base, 0x28, 0,
                                                         &item->mField05,
                                                         &item->mField07);
    item->mVecScale = ret->mV0;
    item->mVecPos = ret->mV1;
    item->mVecColor = ret->mV2;
    u8 idx = item->mField05 + item->mField07;
    u16 w = *(u16*)(base + idx * 0x28);
    if (w != 0x4000) {
        item->field_0x04 |= 0x80;
    } else {
        // 32-bit intermediate so MWCC emits rlwinm instead of andi.
        item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
    }
}

// Advance the item's three anim vectors: look up entry element 0x28, then
// lerp each of the element's +0x04 / +0x10 / +0x1C vectors toward the
// (frame span & 0xFF)-th element (0x28-byte stride) at `f1`, writing into
// the item's +0x0C / +0x18 / +0x24 vectors.
void func_804E06B4(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x28, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = func_804E04D4(item, entries, 0x28);
    if (rate < lbl_eu_8066B290) {
        return;
    }
    u8* b = (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x28;
    func_804DD89C(&item->mField0C, (u8*)ret + 4, b + 4, rate);
    func_804DD89C(&item->mField18, (u8*)ret + 0x10, b + 0x10, rate);
    func_804DD89C(&item->mField24, (u8*)ret + 0x1C, b + 0x1C, rate);
}

// Retail duplicate of func_804E0580.
void func_804E0788(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base == NULL) {
        item->mVecColor = schedZero;
        item->mVecPos = schedZero;
        item->mVecScale = schedZero;
        return;
    }
    CEntryWideElem* ret = (CEntryWideElem*)func_804E0188(base, 0x28, 0,
                                                         &item->mField05,
                                                         &item->mField07);
    item->mVecScale = ret->mV0;
    item->mVecPos = ret->mV1;
    item->mVecColor = ret->mV2;
    u8 idx = item->mField05 + item->mField07;
    u16 w = *(u16*)(base + idx * 0x28);
    if (w != 0x4000) {
        item->field_0x04 |= 0x80;
    } else {
        // 32-bit intermediate so MWCC emits rlwinm instead of andi.
        item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
    }
}

// Same anim-vector refresh as func_804E06B4 (retail duplicate body).
void func_804E08BC(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x28, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = func_804E04D4(item, entries, 0x28);
    if (rate < lbl_eu_8066B290) {
        return;
    }
    u8* b = (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x28;
    func_804DD89C(&item->mField0C, (u8*)ret + 4, b + 4, rate);
    func_804DD89C(&item->mField18, (u8*)ret + 0x10, b + 0x10, rate);
    func_804DD89C(&item->mField24, (u8*)ret + 0x1C, b + 0x1C, rate);
}

// Re-roll the two slot frame lengths of an anim item: for each channel draw
// mtRand, blend it with the float preceding the blob (or 1.0 when absent)
// into the channel rate (+0x14 / +0x10), then quantize the selected entry
// element's u16 frame count against the new rate into the slot pair
// (+0x0E / +0x0C). Finally run the usual terminator flag check.
void func_804E0990(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mSlot0E = 1;
    item->mSlot0C = 1;
    item->mField10 = lbl_eu_8066B2A8;
    item->mField14 = lbl_eu_8066B2A8;
    if (base == NULL) {
        return;
    }
    CEntryU16PairElem* elem =
        (CEntryU16PairElem*)(base + item->mField05 * 8);

    // Channel A
    f32 d = (f32)(s32)ml::math::mtRand() / lbl_eu_8066B2AC - lbl_eu_8066B2B0;
    f32 scale;
    if (base != NULL) {
        scale = *(f32*)(base - 8);
    } else {
        scale = lbl_eu_8066B290;
    }
    f32 t = lbl_eu_8066B2A8 - d * scale;
    item->mField14 = t;
    s32 v = (s32)((f64)(u32)elem->mA * t);
    if ((v & 0xFFFF) != 0) {
        item->mSlot0E = (s16)v;
    }

    // Channel B
    d = (f32)(s32)ml::math::mtRand() / lbl_eu_8066B2AC - lbl_eu_8066B2B0;
    if (base != NULL) {
        scale = *(f32*)(base - 4);
    } else {
        scale = lbl_eu_8066B290;
    }
    t = lbl_eu_8066B2A8 - d * scale;
    item->mField10 = t;
    v = (s32)((f64)(u32)elem->mB * t);
    if ((v & 0xFFFF) != 0) {
        item->mSlot0C = (s16)v;
    }

    u8 idx = item->mField05 + item->mField07;
    u16 w = *(u16*)(base + ((u32)idx << 3));
    if (w != 0x4000) {
        item->field_0x04 |= 0x80;
    } else {
        item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
    }
}

// Blend the two u16 frame values of an 8-stride entry element toward their
// run-end targets using random factors derived from two mtRand draws scaled
// by the floats preceding the blob (-8 / -4); results land in the item's rate
// floats (+0x10/+0x14) and slot handle pair (+0x0E/+0x0C).
void func_804E0CF0(CSchedAnimItem* item, u8* base) {
    if (base == NULL) {
        return;
    }
    if (((item->field_0x04 >> 4) & 1) == 0) {
        return;
    }
    CEntryU16PairElem* ret = (CEntryU16PairElem*)(base + item->mField05 * 8);
    f32 rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC - lbl_eu_8066B2B0;
    f32 scale;
    if (base != NULL) {
        scale = *(f32*)(base - 8);
    } else {
        scale = lbl_eu_8066B290;
    }
    f32 t = lbl_eu_8066B2A8 - rnd * scale;
    item->mField14 = t;
    u16 aw = ret->mA;
    f32 fha = aw;
    s32 v = (s32)(fha * t);
    if ((v & 0xFFFF) != 0) {
        item->mSlot0E = (s16)v;
    }
    rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC - lbl_eu_8066B2B0;
    if (base != NULL) {
        scale = *(f32*)(base - 4);
    } else {
        scale = lbl_eu_8066B290;
    }
    t = lbl_eu_8066B2A8 - rnd * scale;
    item->mField10 = t;
    u16 bw = ret->mB;
    f32 fhb = bw;
    v = (s32)(fhb * t);
    if ((v & 0xFFFF) != 0) {
        item->mSlot0C = (s16)v;
    }
}

// Advance the item's two color channels: lerp the selected 8-stride entry
// element's two u16 values toward their run-end targets at the current rate,
// scaling channel A by the item's 0x14 factor (stored to slot +0x0E), and
// deriving channel B from a constant chain fed by the item's 0x10 factor and
// channel A's quantized result (stored to slot +0x0C).
void func_804E0B94(CSchedAnimItem* item, CEntryElem* entries, u8* arg3,
                   f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 8, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = func_804E04D4(item, entries, 8);
    if (rate < lbl_eu_8066B290) {
        return;
    }
    u8 lo = item->mField05;
    u8 hi = item->mField06;
    CEntryU16PairElem* p = (CEntryU16PairElem*)ret;
    CEntryU16PairElem* end =
        (CEntryU16PairElem*)((u8*)ret + ((hi - lo) & 0xFF) * 8);
    u32 aw = p->mA;
    u32 bw = end->mA;
    // Channel A: u16 at elem+4, lerped and scaled.
    f32 fa = aw;
    f32 fb = bw;
    item->mSlot0E = (s16)((rate * (fb - fa) + fa) * item->mField14);
    // Channel B: u16 at elem+6, lerped, offset/scaled, then re-quantized.
    u32 cw = p->mB;
    u32 dw = end->mB;
    f32 fc = cw;
    f32 fd = dw;
    s16 s = (s16)(lbl_eu_8066B2B4 *
                  (lbl_eu_8066B2B8 + (rate * (fd - fc) + fc) * item->mField10));
    item->mSlot0C = (s16)(lbl_eu_8066B2BC * s);
}

// Initialize one anim item from a 0x14-stride entry blob with a random
// direction vector: reset the frame state, copy the selected element's first
// vector into item+0x10, clear the 2-bit deviation field based on the blob
// header byte, then (when the scale factor preceding the blob exceeds 1.0)
// fill item+0x1C with a random vector, normalize it and scale everything by
// that factor; otherwise zero it.
void func_804E0E48(CSchedAnimItem* item, u8* base, f32* ref) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mRef0 = ref;
    if (base == NULL) {
        *(AnimVec3*)&item->mField1C = schedZero;
        *(AnimVec3*)&item->mField10 = schedZero;
        item->mField28 = lbl_eu_8066B290;
        return;
    }
    CEntryWideElem* ret =
        (CEntryWideElem*)func_804E0188(base, 0x14, 0, &item->mField05,
                                       &item->mField07);
    *(AnimVec3*)&item->mField10 = ret->mV0;
    // clear the 2-bit deviation field from the blob's type byte
    item->field_0x04 &= (u8)~0x06;
    item->mField28 = ret->mV1.x;
    f32 scale;
    if (base != NULL) {
        scale = *(f32*)(base - 0xC);
    } else {
        scale = lbl_eu_8066B290;
    }
    if (scale > lbl_eu_8066B290) {
        randVec__6CERandFPQ22ml5CVec3((ml::CVec3*)&item->mField1C);
        f32 len2 = item->mField1C * item->mField1C +
                   item->mField20 * item->mField20 +
                   item->mField24 * item->mField24;
        if (len2 == lbl_eu_8066B290) {
            *(AnimVec3*)&item->mField1C = schedZero;
        } else {
            PSVECNormalize((const Vec*)&item->mField1C,
                           (Vec*)&item->mField1C);
        }
        item->mField1C *= scale;
        item->mField20 *= scale;
        item->mField24 *= scale;
        item->mField28 *= scale;
    } else {
        *(AnimVec3*)&item->mField1C = schedZero;
    }
    u8 idx = item->mField05 + item->mField07;
    u16 w = *(u16*)(base + idx * 0x14);
    if (w != 0x4000) {
        item->field_0x04 |= 0x80;
    } else {
        item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
    }
}

void func_804E1044(){}

// Point record consumed by the spline evaluator func_804E1294: ml::CVec3
// position at +0x04, scalar channel at +0x10.
struct SplinePt {
    u8 _00[4];
    AnimVec3 mPos;
    f32 mVal;
};

void func_804E1294(CScheduleItem* unused, AnimVec3* outPos, f32* outVal,
                   s32 mode, void** args, f32 t) {
    if (mode == 9 || mode == 10) {
        // Cubic Hermite over the middle two points with Catmull-Rom end
        // tangents m0 = k*(p2-p0), m1 = k*(p3-p1). The pool constants are
        // the basis coefficients (B2C0 = 2, B2C4 = 3, B2C8 = -2,
        // B2B0 = tangent scale).
        SplinePt* p0 = (SplinePt*)args[0];
        SplinePt* p1 = (SplinePt*)args[1];
        SplinePt* p2 = (SplinePt*)args[2];
        SplinePt* p3 = (SplinePt*)args[3];
        f32 t2 = t * t;
        f32 t3 = t2 * t;
        f32 h10 = t3 - lbl_eu_8066B2C0 * t2 + t;
        f32 h00 = lbl_eu_8066B290 + lbl_eu_8066B2C0 * t3 - lbl_eu_8066B2C4 * t2;
        f32 h01 = lbl_eu_8066B2C8 * t3 + lbl_eu_8066B2C4 * t2;
        f32 h11 = t3 - t2;
        f32 k = lbl_eu_8066B2B0;
        AnimVec3 m0;
        m0.x = (p2->mPos.x - p0->mPos.x) * k;
        m0.y = (p2->mPos.y - p0->mPos.y) * k;
        m0.z = (p2->mPos.z - p0->mPos.z) * k;
        AnimVec3 m1;
        m1.x = (p3->mPos.x - p1->mPos.x) * k;
        m1.y = (p3->mPos.y - p1->mPos.y) * k;
        m1.z = (p3->mPos.z - p1->mPos.z) * k;
        outPos->x = h00 * p1->mPos.x + h10 * m0.x + h01 * p2->mPos.x +
                    h11 * m1.x;
        outPos->y = h00 * p1->mPos.y + h10 * m0.y + h01 * p2->mPos.y +
                    h11 * m1.y;
        outPos->z = h00 * p1->mPos.z + h10 * m0.z + h01 * p2->mPos.z +
                    h11 * m1.z;
        if (mode == 9) {
            // Scalar channel follows the same Hermite blend with tangent
            // scalars.
            f32 dm0 = (p2->mVal - p0->mVal) * k;
            f32 dm1 = (p3->mVal - p1->mVal) * k;
            *outVal = h00 * p1->mVal + h10 * dm0 + h01 * p2->mVal + h11 * dm1;
        } else {
            // Scalar channel is a plain lerp.
            *outVal = p1->mVal + t * (p2->mVal - p1->mVal);
        }
    } else if (mode == 11) {
        // Hard step at t >= 1: snap to point 1 below the threshold, point 2
        // above it.
        SplinePt* src;
        if (t >= lbl_eu_8066B2A8) {
            src = (SplinePt*)args[2];
        } else {
            src = (SplinePt*)args[1];
        }
        *outPos = src->mPos;
        *outVal = src->mVal;
    }
}

// Initialize one anim item from an 8-stride entry blob with a random factor:
// reset the frame state, draw mtRand scaled via the sdata2 pool constants,
// blend it with the float preceding the blob (or 1.0 when absent) into
// item+0x0C, scale the selected element's float by the random factor into
// item+0x10, then run the usual terminator flag check.
void func_804E17A4(CSchedAnimItem* item, u8* base) {
    f32 one = lbl_eu_8066B290;
    item->mField00 = one;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base == NULL) {
        item->mField10 = one;
        item->mField0C = lbl_eu_8066B2A8;
        return;
    }
    u8* ret = func_804E0188(base, 8, 0, &item->mField05, &item->mField07);
    f32 rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC - lbl_eu_8066B2B0;
    f32 scale;
    if (base == NULL) {
        scale = one;
    } else {
        scale = *(f32*)(base - 4);
    }
    item->mField0C = lbl_eu_8066B2A8 - rnd * scale;
    item->mField10 = ((CEntryFloatElem*)ret)->mF32 * rnd;
    u8 idx = item->mField05 + item->mField07;
    u16 w = *(u16*)(base + idx * 8);
    if (w != 0x4000) {
        item->field_0x04 |= 0x80;
    } else {
        item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
    }
}

// Advance the item's animation cursor: look up the entry element at index 8,
// then interpolate the element's float table between slot 0 and the
// (span & 0x1F)-th entry, scaled by the item's 0x0C factor.
extern "C" void func_804E18CC(CSchedAnimItem* item, CEntryElem* entries, u8* arg4, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 8, arg4, f1);
    if (ret == NULL) {
        return;
    }
    f32 r = func_804E04D4(item, entries, 8);
    if (r < lbl_eu_8066B290) {
        return;
    }
    CEntryFloatElem* arr = (CEntryFloatElem*)ret;
    u8 lo = item->mField05;
    u8 hi = item->mField06;
    // Lerp between slot 0 and slot (hi-lo) of the float table, then scale
    // by the item's 0x0C factor (named temp keeps retail's fmuls order).
    f32 lerp = r * (arr[(u8)(hi - lo)].mF32 - arr[0].mF32) + arr[0].mF32;
    item->mField10 = lerp * item->mField0C;
}

// Initialize one schedule item's anim/entry state: set the base frame
// factor, reset the run-window bytes and slot handles, then (when an entry
// blob is present) load the run's slot handles and set the 0x04 flag bits
// according to whether the run extends to the blob terminator.
extern "C" void func_804E196C(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    CScheduleItem* si = (CScheduleItem*)item;
    si->mSlots[2] = -1;
    si->mSlots[3] = -1;
    if (base != NULL) {
        u8* ret = func_804E0188(base, 8, 0, &item->mField05, &item->mField07);
        CEntryElem* elem = (CEntryElem*)ret;
        if (elem->mField00 != 0x4000) {
            si->mSlots[2] = elem->mField04;
            si->mSlots[3] = elem->mField06;
        }
        u8 idx = item->mField05 + item->mField07;
        u16 w = *(u16*)(base + idx * 8);
        if (w != 0x4000) {
            item->field_0x04 |= 0x80;
        } else {
            // 32-bit intermediate (cast the whole AND, not the mask) so MWCC
            // emits rlwinm instead of andi. (docs/MWCC_CASES.md btm_sec)
            item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
        }
    }
}

// Refresh slot handles 2/3 from the entry-blob element selected by
// func_804E0248 at index 8.
void func_804E1A44(CScheduleItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248(item, entries, 8, arg3, f1);
    if (ret != NULL) {
        if (item->mSlots[2] != ret->mField04) {
            item->mSlots[2] = ret->mField04;
        }
        if (item->mSlots[3] != ret->mField06) {
            item->mSlots[3] = ret->mField06;
        }
    }
}

// Initialize one anim item from a 0x14-stride entry blob: reset the frame
// state, zero the item's 0x0C..0x1C vector pair, then (when an entry blob is
// present) copy the selected element's vector and set the 0x04 flag bits:
// scale-deviation bits when the copied vector deviates from (1,1,1,1), plus
// the usual run-tail terminator check.
void func_804E1AA8(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mQuad0C = schedZero4;
    if (base != NULL) {
        CEntryWide4Elem* ret =
            (CEntryWide4Elem*)func_804E0188(base, 0x14, 0, &item->mField05,
                                            &item->mField07);
        if (ret->mType == 0x4000) {
            item->field_0x04 |= 0x8;
        } else {
            item->mQuad0C = ret->mVec;
            // Flag bit pairs when the stored vector deviates from (1,1,1,1).
            item->mScaleFlag = (item->mField0C != lbl_eu_8066B290 ||
                                item->mField10 != lbl_eu_8066B290);
            item->mScaleFlag =
                (u8)(item->mScaleFlag |
                     (u8)((item->mField14 != lbl_eu_8066B290 ||
                           item->mField18 != lbl_eu_8066B290)
                          << 1));
            u8 idx = item->mField05 + item->mField07;
            u16 w = *(u16*)(base + idx * 0x14);
            if (w != 0x4000) {
                item->field_0x04 |= 0x80;
            } else {
                // 32-bit intermediate so MWCC emits rlwinm instead of andi.
                item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
            }
        }
    }
}

// HSV-style channel decomposition of a scaled RGB triple into `out`:
// out+8 = max channel, out+4 = delta/max, out+0 = hue sector fraction
// normalized to [-1, 1) via the sdata2 constants.
struct HueSplitOut {
    f32 mHue;   // 0x00
    f32 mDelta; // 0x04
    f32 mMax;   // 0x08
};

void func_804E2F7C(CScheduleItem* unused, HueSplitOut* out,
                   const AnimVec3* src, f32 div) {
    // Scaled channels kept in a local array (retail spills them to the stack).
    f32 ch[3];
    ch[0] = src->x / div;
    ch[1] = src->y / div;
    ch[2] = src->z / div;
    // Nested pairwise max/min chains (retail branch shapes verbatim).
    f32 mx = ch[2];
    if (ch[0] <= ch[1]) {
        if (ch[1] <= ch[2]) {
            mx = ch[2];
        } else {
            mx = ch[1];
        }
    } else if (ch[0] > ch[2]) {
        mx = ch[0];
    }
    f32 mn = ch[2];
    if (ch[0] >= ch[1]) {
        if (ch[1] >= ch[2]) {
            mn = ch[2];
        } else {
            mn = ch[1];
        }
    } else if (ch[0] < ch[2]) {
        mn = ch[0];
    }
    out->mMax = mx;
    if (lbl_eu_8066B290 == mx) {
        return;
    }
    f32 y = (mx - mn) / mx;
    out->mDelta = y;
    if (y == lbl_eu_8066B290) {
        return;
    }
    // Hue sector selected by which channel equals the max.
    if (ch[0] == mx) {
        out->mHue = (ch[1] - ch[2]) / (mx - mn);
    } else if (ch[1] == mx) {
        out->mHue = lbl_eu_8066B2C0 + (ch[2] - ch[0]) / (mx - mn);
    } else { // ch[2] == mx
        out->mHue = lbl_eu_8066B2D4 + (ch[0] - ch[1]) / (mx - mn);
    }
    f32 h = out->mHue / lbl_eu_8066B2D8;
    out->mHue = h;
    if (h < lbl_eu_8066B290) {
        out->mHue = lbl_eu_8066B2A8 + h;
    }
}

// 4-float paired lerp kernel (monolib/src/effect/code_804DB938.cpp,
// retail symbol func_804DD8C8): out = a + (b - a) * t for four f32s.
extern "C" void func_804DD8C8(void* out, const void* a, const void* b, f32 t);

// Advance the item's 4-float scale channel (+0x0C..+0x18): lerp the selected
// element's +4 vector toward the (frame span & 0xFF)-th element (0x14 stride)
// at the current rate, then update the two deviate bits of the 0x04 flag
// byte's 2-bit field (bits 5-6): bit5 = x/y left the 1.0 identity, bit6 = z/w.
void func_804E1C1C(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x14, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = func_804E04D4(item, entries, 0x14);
    if (rate < lbl_eu_8066B290) {
        return;
    }
    func_804DD8C8(&item->mField0C, (u8*)ret + 4,
                  (u8*)ret + (item->mField06 - item->mField05) * 0x14 + 4,
                  rate);
    // deviate bit 5: x/y
    u8 devXY = 0;
    if (item->mField0C != lbl_eu_8066B290 || item->mField10 != lbl_eu_8066B290) {
        devXY = 1;
    }
    f32 zw0 = item->mField14;
    if (devXY != 0) {
        item->mScaleFlag |= 1;
    } else {
        item->mScaleFlag &= (u8)~1;
    }
    // deviate bit 6: z/w
    u8 devZW = 0;
    if (zw0 != lbl_eu_8066B290 || item->mField18 != lbl_eu_8066B290) {
        devZW = 1;
    }
    if (devZW != 0) {
        item->mScaleFlag |= 2;
    } else {
        item->mScaleFlag &= (u8)~2;
    }
}

// Re-initialize a schedule item from a 0x10-stride entry blob with three
// pre-scaled random channels: reset the frame state, run the terminator scan
// (func_804E0188), read the three floats preceding the blob into the rate
// slots (+0x0C/+0x10/+0x14), then draw three mtRand rates (+0x18/+0x1C/+0x20)
// - each channel reuses the previous rate while its pre-scale is < 1.0, and
// sets flag 0x10 when its pre-scale exceeds 1.0. When flag 0x10 was set the
// rates are folded into the pre-scales, the element's +4 vector (copied to
// +0x24..+0x2C) is scaled by the new rates, and the usual terminator check
// runs. All rate/scale slots default to 2.0 when no blob is present.
void func_804E1D50(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base == NULL) {
        item->mField14 = lbl_eu_8066B2A8;
        item->mField10 = lbl_eu_8066B2A8;
        item->mField0C = lbl_eu_8066B2A8;
        item->mField20 = lbl_eu_8066B2A8;
        item->mField1C = lbl_eu_8066B2A8;
        item->mField18 = lbl_eu_8066B2A8;
        item->mField2C = lbl_eu_8066B2A8;
        item->mField28 = lbl_eu_8066B2A8;
        item->mField24 = lbl_eu_8066B2A8;
        return;
    }
    u8* ret = func_804E0188(base, 0x10, 0, &item->mField05, &item->mField07);
    if (base != NULL) {
        item->mField0C = *(f32*)(base - 0x18);
    } else {
        item->mField0C = lbl_eu_8066B2A8;
    }
    if (base != NULL) {
        item->mField10 = *(f32*)(base - 0x14);
    } else {
        item->mField10 = lbl_eu_8066B2A8;
    }
    if (base != NULL) {
        item->mField14 = *(f32*)(base - 0x10);
    } else {
        item->mField14 = lbl_eu_8066B2A8;
    }
    f32* src24 = (f32*)((u8*)ret + 4);
    item->mField24 = src24[0];
    item->mField28 = src24[1];
    item->mField2C = src24[2];
    // channel A
    f32 rnd = s32ToF32_B2A0((s32)ml::math::mtRand()) / lbl_eu_8066B2AC -
              lbl_eu_8066B2B0;
    f32 dScale = base != NULL ? *(f32*)(base - 0xc) : lbl_eu_8066B290;
    item->mField18 = lbl_eu_8066B2A8 - dScale * rnd;
    if (dScale > lbl_eu_8066B290) {
        item->field_0x04 |= 0x10;
    }
    // channel B: reuse channel A's rate while the pre-scale is < 1.0
    f32 dScale2 = base != NULL ? *(f32*)(base - 8) : lbl_eu_8066B2CC;
    if (dScale2 < lbl_eu_8066B290) {
        item->mField1C = item->mField18;
    } else {
        f32 rnd2 = s32ToF32_B2A0((s32)ml::math::mtRand()) / lbl_eu_8066B2AC -
                   lbl_eu_8066B2B0;
        item->mField1C = lbl_eu_8066B2A8 - dScale2 * rnd2;
        if (dScale2 > lbl_eu_8066B290) {
            item->field_0x04 |= 0x10;
        }
    }
    // channel C: reuse channel A's rate while the pre-scale is < 1.0
    f32 dScale3 = base != NULL ? *(f32*)(base - 4) : lbl_eu_8066B2CC;
    if (dScale3 < lbl_eu_8066B290) {
        item->mField20 = item->mField18;
    } else {
        f32 rnd3 = s32ToF32_B2A0((s32)ml::math::mtRand()) / lbl_eu_8066B2AC -
                   lbl_eu_8066B2B0;
        item->mField20 = lbl_eu_8066B2A8 - dScale3 * rnd3;
        if (dScale3 > lbl_eu_8066B290) {
            item->field_0x04 |= 0x10;
        }
    }
    if (((item->field_0x04 >> 4) & 1) == 1) {
        item->mField0C *= item->mField18;
        item->mField10 *= item->mField1C;
        item->mField14 *= item->mField20;
    }
    item->mField24 *= item->mField0C;
    item->mField28 *= item->mField10;
    item->mField2C *= item->mField14;
    u8 idx = item->mField05 + item->mField07;
    u16 w = *(u16*)(base + ((idx & 0xFF) * 0x10));
    if (w != 0x4000) {
        item->field_0x04 |= 0x80;
    } else {
        item->field_0x04 = (u8)((item->field_0x04 | 8) & ~0x80);
    }
}


// Advance the item's anim vector at +0x24: lerp it from entry element
// 0x10's +4 vector toward the (frame span & 0xFF)-th element (0x10-byte
// stride) at the current rate, then scale it by the item's 0x0C..0x14
// vector.
void func_804E2088(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x10, arg3, f1);
    if (ret != NULL) {
        f32 rate = func_804E04D4(item, entries, 0x10);
        if (rate < lbl_eu_8066B290) {
            return;
        }
        func_804DD89C(&item->mField24, (u8*)ret + 4,
                      (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x10 + 4,
                      rate);
        item->mField24 = item->mField24 * item->mField0C;
        item->mField28 = item->mField28 * item->mField10;
        item->mField2C = item->mField2C * item->mField14;
    }
}

// Initialize one anim item from a 0x10-stride entry blob with a position
// vector and a random-sign direction: reset the frame state, copy the
// selected element's vector into item+0x28 and ml::CVec3::unit into
// item+0x34 (re-randomized when the blob mode byte is set), rescale the
// +0x10..+0x18 factors by fresh random rates whenever the stored rate triple
// deviates from (1,1,1), advance item+0x28 by the item velocity (+0x10),
// decay it while the 0x0C byte is nonzero, then run the terminator check.
void func_804E214C(CSchedAnimItem* item, u8* base, u8 arg3) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mByte0C = arg3;
    if (base != NULL) {
        CEntryVecElem* ret =
            (CEntryVecElem*)func_804E0188(base, 0x10, 0, &item->mField05,
                                          &item->mField07);
        f32 sc;
    if (base != NULL) {
        sc = *(f32*)(base - 0x18);
    } else {
        sc = lbl_eu_8066B290;
    }
    item->mField10 = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 0x14);
    } else {
        sc = lbl_eu_8066B290;
    }
    item->mField14 = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 0x10);
    } else {
        sc = lbl_eu_8066B290;
    }
    item->mField18 = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 0xC);
    } else {
        sc = lbl_eu_8066B290;
    }
    item->mField1C = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 8);
    } else {
        sc = lbl_eu_8066B290;
    }
    item->mField20 = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 4);
    } else {
        sc = lbl_eu_8066B290;
    }
    item->mField24 = sc;
    *(AnimVec3*)&item->mExt28 = ret->mVec;
    *(AnimVec3*)&item->mExt34 = schedUnit;
    u8 signMode;
    if (base != NULL) {
        signMode = *(u8*)(base - 0x1B);
    } else {
        signMode = 0;
    }
    if (signMode == 1) {
        CERand::randSignVec((ml::CVec3*)&item->mExt34);
    }
    if (!(item->mField1C == lbl_eu_8066B290 &&
          item->mField20 == lbl_eu_8066B290 &&
          item->mField24 == lbl_eu_8066B290)) {
        // volatile: retail re-loads these floats instead of CSE-ing them
        // with the copies taken above.
        f32 rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC -
                  lbl_eu_8066B2B0;
        if (base != NULL) {
            sc = *(volatile f32*)(base - 0xC);
        } else {
            sc = lbl_eu_8066B290;
        }
        item->mField1C = lbl_eu_8066B2A8 - rnd * sc;
        rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC -
              lbl_eu_8066B2B0;
        if (base != NULL) {
            sc = *(volatile f32*)(base - 8);
        } else {
            sc = lbl_eu_8066B290;
        }
        item->mField20 = lbl_eu_8066B2A8 - rnd * sc;
        rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC -
              lbl_eu_8066B2B0;
        if (base != NULL) {
            sc = *(volatile f32*)(base - 4);
        } else {
            sc = lbl_eu_8066B290;
        }
        item->mField24 = lbl_eu_8066B2A8 - rnd * sc;
        item->field_0x04 |= 0x10;
        item->mField10 *= item->mField1C;
        item->mField14 *= item->mField20;
        item->mField18 *= item->mField24;
    }
    item->mExt28 += item->mField10;
    item->mExt2C += item->mField14;
    item->mExt30 += item->mField18;
    if (item->mByte0C != 0) {
        item->mExt28 *= lbl_eu_8066B2CC;
        item->mExt2C *= lbl_eu_8066B2CC;
        item->mExt30 *= lbl_eu_8066B2CC;
    }
    u8 idx = item->mField05 + item->mField07;
    u16 w = *(u16*)(base + ((u32)idx << 4));
    if (w != 0x4000) {
        item->field_0x04 |= 0x80;
    } else {
        // 32-bit intermediate so MWCC emits rlwinm instead of andi.
        item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
    }
    } else {
        item->mExt30 = lbl_eu_8066B290;
        item->mExt2C = lbl_eu_8066B290;
        item->mExt28 = lbl_eu_8066B290;
        item->mField18 = lbl_eu_8066B290;
        item->mField14 = lbl_eu_8066B290;
        item->mField10 = lbl_eu_8066B290;
        item->mField24 = lbl_eu_8066B2A8;
        item->mField20 = lbl_eu_8066B2A8;
        item->mField1C = lbl_eu_8066B2A8;
    }
}

// Advance the item's working quad at +0x28 according to the blob's mode
// byte: mode 0 lerps it toward the frame-span-th element and adds the
// item's velocity (+0x10); mode 1 additionally scales it by the factors at
// +0x34..+0x3C first; mode 2 scales it down by the +0x0C amount, then adds
// the span element scaled by the caller's factor. The final +0x0C decay is
// applied whenever it is nonzero.
void func_804E24A8(CSchedAnimItem* item, CEntryElem* entries, u8* arg3,
                   f32 f1) {
    CSchedAnimItem* self = item;
    CEntryElem* blob = entries;
    f32 mult = f1;
    CEntryElem* ret = func_804E0248((CScheduleItem*)self, blob, 0x10,
                                    arg3, mult);
    if (ret == NULL) {
        return;
    }
    f32 rate = func_804E04D4(self, blob, 0x10);
    if (rate < lbl_eu_8066B290) {
        return;
    }
    u8 type = blob ? *((u8*)blob - 0x1B) : 0;
    u8* retp = (u8*)ret;
    u8 span = item->mField06 - item->mField05;
    u8* src = retp + span * 0x10 + 4;
    if (type == 0) {
        func_804DD89C((u8*)self + 0x28, retp + 4, src, rate);
        self->mExt28 += self->mField10;
        self->mExt2C += self->mField14;
        self->mExt30 += self->mField18;
    } else if (type == 1) {
        func_804DD89C((u8*)self + 0x28, retp + 4, src, rate);
        self->mExt28 *= self->mExt34;
        self->mExt2C *= self->mExt38;
        self->mExt30 *= self->mExt3C;
        self->mExt28 += self->mField10;
        self->mExt2C += self->mField14;
        self->mExt30 += self->mField18;
    } else if (type == 2) {
        if (self->mByte0C != 0) {
            self->mExt28 *= lbl_eu_8066B2CC;
            self->mExt2C *= lbl_eu_8066B2CC;
            self->mExt30 *= lbl_eu_8066B2CC;
        }
        AnimVec3 tmp;
        tmp.x = (*(f32*)(src + 0)) * mult;
        tmp.y = (*(f32*)(src + 4)) * mult;
        tmp.z = (*(f32*)(src + 8)) * mult;
        self->mExt28 += tmp.x;
        self->mExt2C += tmp.y;
        self->mExt30 += tmp.z;
    }
    if (self->mByte0C != 0) {
        self->mExt28 *= lbl_eu_8066B2CC;
        self->mExt2C *= lbl_eu_8066B2CC;
        self->mExt30 *= lbl_eu_8066B2CC;
    }
}

// Initialize one anim item from a 0x10-stride entry blob (color variant):
// reset the frame state, validate the four floats preceding the blob against
// 1.0, then either snap the rate quad (+0x0C..+0x18) to the pool upper bound
// or rebuild it from mtRand draws blended with those floats, copy the selected
// element's color halfword/float data into item+0x1C..+0x20, set the 0x10
// rate flag, and run the usual terminator check.
void func_804E26D8(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
    CEntryVecElem* ret =
        (CEntryVecElem*)func_804E0188(base, 0x10, 0, &item->mField05,
                                      &item->mField07);
    f32 v14 = (base != NULL) ? *(f32*)(base - 0x14) : lbl_eu_8066B290;
    bool eq;
    if (v14 == lbl_eu_8066B290) {
        // fcmpo/bgt: continues only when not greater (lt/eq/unordered)
        f32 v10 = (base != NULL) ? *(f32*)(base - 0x10) : lbl_eu_8066B2CC;
        if (!(v10 > lbl_eu_8066B290)) {
            f32 v0C = (base != NULL) ? *(f32*)(base - 0xC) : lbl_eu_8066B2CC;
            if (!(v0C > lbl_eu_8066B290)) {
                f32 v08 =
                    (base != NULL) ? *(f32*)(base - 8) : lbl_eu_8066B290;
                eq = (v08 == lbl_eu_8066B290);
            } else {
                eq = false;
            }
        } else {
            eq = false;
        }
    } else {
        eq = false;
    }
    if (eq) {
        f32 c = lbl_eu_8066B2A8;
        item->mField24 = c;
        item->mField18 = c;
        item->mField14 = c;
        item->mField10 = c;
        item->mField0C = c;
    } else {
        // volatile: these floats are re-loaded rather than reused from the
        // validation reads above.
        f32 s10 = (base != NULL) ? *(volatile f32*)(base - 0x10)
                                 : lbl_eu_8066B2CC;
        f32 s0C = (base != NULL) ? *(volatile f32*)(base - 0xC)
                                 : lbl_eu_8066B2CC;
        if (s10 < lbl_eu_8066B290 && s0C < lbl_eu_8066B290) {
            item->mField14 = lbl_eu_8066B2A8;
            item->mField10 = lbl_eu_8066B2A8;
            item->mField0C = lbl_eu_8066B2A8;
            f32 rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC -
                      lbl_eu_8066B2B0;
            f32 sc = (base != NULL) ? *(volatile f32*)(base - 0x14)
                                    : lbl_eu_8066B290;
            item->mField24 = lbl_eu_8066B2A8 - rnd * sc;
        } else {
            f32 rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC -
                      lbl_eu_8066B2B0;
            f32 sc = (base != NULL) ? *(volatile f32*)(base - 0x14)
                                    : lbl_eu_8066B290;
            item->mField0C = lbl_eu_8066B2A8 - rnd * sc;
            rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC -
                  lbl_eu_8066B2B0;
            sc = (base != NULL) ? *(volatile f32*)(base - 0x10)
                                : lbl_eu_8066B2CC;
            item->mField10 = lbl_eu_8066B2A8 - rnd * sc;
            f32 c = lbl_eu_8066B2A8;
            item->mField24 = c;
            rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC -
                  lbl_eu_8066B2B0;
            sc = (base != NULL) ? *(volatile f32*)(base - 0xC)
                                : lbl_eu_8066B2CC;
            item->mField14 = c - rnd * sc;
        }
        f32 rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC -
                  lbl_eu_8066B2B0;
        f32 sc = (base != NULL) ? *(volatile f32*)(base - 8) : lbl_eu_8066B290;
        u8 flag = item->field_0x04 | 0x10;
        item->mField18 = lbl_eu_8066B2A8 - rnd * sc;
        item->field_0x04 = flag;
    }
    // Color bytes arrive as u16 halves; the truncating stores emit lhz/stb.
    u16* cw = (u16*)((u8*)ret + 4);
    item->mByte1C = (u8)cw[0];
    item->mByte1D = (u8)cw[1];
    item->mByte1E = (u8)cw[2];
    item->mByte1F = (u8)cw[3];
    item->mField20 = *(f32*)((u8*)ret + 0xC);
    u8 idx = item->mField05 + item->mField07;
    u16 w = *(u16*)(base + ((u32)idx << 4));
    if (w != 0x4000) {
        item->field_0x04 |= 0x80;
    } else {
        // 32-bit intermediate so MWCC emits rlwinm instead of andi.
        item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
    }
    } else {
        item->mField18 = lbl_eu_8066B2A8;
        item->mField14 = lbl_eu_8066B2A8;
        item->mField10 = lbl_eu_8066B2A8;
        item->mField0C = lbl_eu_8066B2A8;
        item->mByte1F = 0xFF;
        item->mByte1E = 0xFF;
        item->mByte1D = 0xFF;
        item->mByte1C = 0xFF;
        item->mField24 = lbl_eu_8066B2A8;
        item->mField20 = lbl_eu_8066B2A8;
    }
}

// Advance the item's 4-byte color at +0x1C and float at +0x20: lerp entry
// element 0x10's +4 byte components toward the frame-span-th element
// (0x10-byte stride) at the current rate.
void func_804E2A5C(CSchedAnimItem* item, CEntryElem* entries, u8* arg3,
                   f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x10, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = func_804E04D4(item, entries, 0x10);
    if (rate < lbl_eu_8066B290) {
        return;
    }
    u8* a = (u8*)ret + 4;
    u8* b = (u8*)ret + (item->mField06 - item->mField05) * 0x10 + 4;
    u8* dst = (u8*)item + 0x1C;
    // Retail vectorizes these four byte lerps into two paired-single ops.
    dst[0] = (u8)(a[0] + f1 * (b[0] - a[0]));
    dst[1] = (u8)(a[1] + f1 * (b[1] - a[1]));
    dst[2] = (u8)(a[2] + f1 * (b[2] - a[2]));
    dst[3] = (u8)(a[3] + f1 * (b[3] - a[3]));
    f32 ca = *(f32*)((u8*)ret + 0xC);
    f32 cb = *(f32*)(b + 8);
    item->mField20 = ca + f1 * (cb - ca);
}

void func_804E2B54(){}

// Initialize one anim item from a 0x10-stride entry blob: reset the frame
// state, copy the selected element's three component references into
// item+0x0C..0x18, flag bit 0x60 when the referenced components deviate from
// 1.0, then check the run tail for the blob terminator (0x80 / 0x08 flags).
void func_804E2D8C(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
        u8* ret = func_804E0188(base, 0x10, 0, &item->mField05, &item->mField07);
        CEntryVecElem* elem = (CEntryVecElem*)ret;
        item->mVecScale = elem->mVec;
        // Flag bit 0x60 when the stored vector deviates from (1,1,1).
        u8 flag = false;
        if (item->mField0C != lbl_eu_8066B290 || item->mField10 != lbl_eu_8066B290 ||
            item->mField14 != lbl_eu_8066B290) {
            flag = true;
        }
        item->mScaleFlag = flag;
        u8 idx = item->mField05 + item->mField07;
        u16 w = *(u16*)(base + idx * 0x10);
        if (w != 0x4000) {
            item->field_0x04 |= 0x80;
        } else {
            // 32-bit intermediate so MWCC emits rlwinm instead of andi.
            item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
        }
    } else {
        item->mVecScale = schedZero;
    }
}

// Refresh the item's scale vector at +0x0C: lerp it from entry element
// 0x10's +4 vector toward the (frame span & 0xFF)-th element (0x10-byte
// stride) at the current rate, then set bit 0x20 of the 0x04 flag byte when
// the vector is not (1,1,1).
void func_804E2EAC(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x10, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = func_804E04D4(item, entries, 0x10);
    if (rate < lbl_eu_8066B290) {
        return;
    }
    func_804DD89C(&item->mField0C, (u8*)ret + 4,
                  (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x10 + 4,
                  rate);
    // flag starts cleared and is set only when the vector deviates from 1.0
    u8 flag = false;
    if (item->mField0C != lbl_eu_8066B290 || item->mField10 != lbl_eu_8066B290 ||
        item->mField14 != lbl_eu_8066B290) {
        flag = true;
    }
    item->mScaleFlag = flag;
}


// HSV-like -> RGB conversion: `hsv` holds (hue, saturation, value); the hue
// is scaled by the pool constant and truncated to pick one of six sector
// permutations of (v, t, p) = (value, value*(1-s*f), value*(1-s)), each
// clamped to the upper bound; all three results are finally scaled by `mul`.
// `self` is unused in retail.
void func_804E30F0(CScheduleItem* self, AnimVec3* out, const AnimVec3* hsv,
                   f32 mul) {
    f32 h = hsv->x;
    if (h < lbl_eu_8066B2A8) {
        h = lbl_eu_8066B2DC;
    }
    f32 hf = lbl_eu_8066B2D8 * h;
    s32 sector = (s32)hf;
    f32 s = hsv->y;
    if (s == lbl_eu_8066B290) {
        out->x = hsv->z;
        out->y = hsv->z;
        out->z = hsv->z;
    } else {
        f32 v = hsv->z;
        f32 frac = hf - (f32)sector;
        f32 p = v * (lbl_eu_8066B2A8 - s);
        f32 q = v * (lbl_eu_8066B2A8 - s * frac);
        f32 t = v * (lbl_eu_8066B2A8 - s * (lbl_eu_8066B2A8 - frac));
        if (p > lbl_eu_8066B2A8) {
            p = lbl_eu_8066B2A8;
        }
        if (q > lbl_eu_8066B2A8) {
            q = lbl_eu_8066B2A8;
        }
        if (t > lbl_eu_8066B2A8) {
            t = lbl_eu_8066B2A8;
        }
        if (sector == 0) {
            out->x = hsv->z;
            out->y = t;
            out->z = p;
        } else if (sector == 1) {
            out->x = q;
            out->y = hsv->z;
            out->z = p;
        } else if (sector == 2) {
            out->x = p;
            out->y = hsv->z;
            out->z = t;
        } else if (sector == 3) {
            out->x = p;
            out->y = q;
            out->z = hsv->z;
        } else if (sector == 4) {
            out->x = t;
            out->y = p;
            out->z = hsv->z;
        } else if (sector == 5) {
            out->x = hsv->z;
            out->y = p;
            out->z = q;
        }
    }
    out->x *= mul;
    out->y *= mul;
    out->z *= mul;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.bss] 0x80660038-0x80661748 (0x1710 = 5904B) zero-fill
u8 lbl_eu_80660038[2880];
u8 lbl_eu_80660B78[2880];
u8 lbl_eu_806616B8[96];
CScheduleItemPool lbl_eu_80661718;
CEffectPool lbl_eu_80661728;
CEffectNodePool lbl_eu_80661738;
// [.sbss] 0x80665A20-0x80665A50 (48B) zero-fill (80665A20..80665A40 already defined above)
void* lbl_eu_80665A44;
u8 lbl_eu_80665A48[8];
