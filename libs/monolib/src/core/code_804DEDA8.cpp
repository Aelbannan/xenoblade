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

// View of the schedule item used by the anim/entry helpers (SchedAnim_FrameRate,
// SchedAnim_AdvanceCursor): f32 at 0x00, byte frame fields at 0x05/0x06, f32s at
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
    u8 mField05;    // 0x05: entry-run start (written by SchedEntry_PickRandom)
    u8 mField06;    // 0x06
    u8 mField07;    // 0x07: entry-run tail count (written by SchedEntry_PickRandom)
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
        struct { // byte/extended-float view used by SchedAnim_AdvanceQuad
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

// 8-byte element of the entry array returned by SchedEntry_ResolveElem (f32 at +4).
struct CEntryFloatElem {
    u8 mPad[4];   // 0x00
    f32 mF32;     // 0x04
};

// 8-byte entry element carrying two u16 frame values (read by SchedAnim_BlendFrameU16
// and the channel lerp in SchedAnim_AdvanceColor8).
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

// The float constants used by the schedule anim helpers are written as
// literals at their use sites; MWCC emits the retail .sdata2 pool entries.

// Signed integer to float conversion for the random-unit helpers.
static inline f32 s32ToF32(s32 v) {
    return (f32)v;
}

// Import: random unit-ish vector generator (monolib/src/effect/CERand.cpp,
// retail symbol randVec__6CERandFPQ22ml5CVec3).
extern "C" void randVec__6CERandFPQ22ml5CVec3(ml::CVec3* v);

extern void* spEffectNodeWork; // .sbss pointer slot (4 bytes)

// Zero one 0x18-byte schedule-resource entry. Callers (e.g. SchedRes_AllocEntries)
// keep the retail direct `bl` instead of an inlined loop body.
// extern "C" keeps the call-site reloc name matching retail (SchedRes_ZeroEntry).
// No inline attribute is needed: the definition sits below SchedRes_AllocEntries and
// inside a `#pragma auto_inline off` region, so MWCC cannot inline it into the
// allocation loop at -O4,p (verified: hexdiff --all byte-identical without it).
// Forward decl (full definition below, before SchedRes_AllocEntries).
struct CResEntry;
extern "C" void SchedRes_ZeroEntry(CResEntry* pEntry);

// Destructor of the static schedule-resource manager (object at 0x804DF068):
// release the two heap arrays and the MemManager block, then zero the object.
// extern "C" keeps the symbol/reloc name matching retail (__dt__804DF068).
// Definition sits below SchedRes_ReleaseEntries so MWCC's same-TU IPA cannot prove it
// leaves the pool globals untouched (retail reloads them per iteration).
extern "C" void __dt__804DF068(CSchedResMgr* obj);

// Lookup-table object shared by SchedRes_LookupKeyIndexed / SchedRes_ResetAnimViaTable / SchedRes_FindValueByKey
// (SchedRes_EntryByIndex reads the same object's res-table pointer at +0x10).
struct ResTable;  // defined below; only the pointer at +0x10 is read here.
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
    u32* mTable;               // 0x0C: index -> u32 table (SchedRes_ResetAnimViaTable)
    ResTable* mResTable;       // 0x10: res-table pointer (SchedRes_EntryByIndex)
};

// Indexed key accessor: index must be < 0x100 and within the entry count.
// extern "C" keeps the call-site reloc name matching retail.
extern "C" DECOMP_DONT_INLINE void* SchedRes_LookupKeyIndexed(CResLookup* self, u32 index) {  // noinline-ok: retail bl 0x804E3AE4 targets this in-TU copy; -ipa off, -inline noauto (loses 3 *_ReleaseHandle), -O4,s (loses AdvanceVec28/B), `static` all tried
    if (index >= 0x100) {
        return NULL;
    }
    if (self->mCount <= index) {
        return NULL;
    }
    return self->mEntries[index].mKey;
}

// One 0x18-byte schedule-resource entry. SchedRes_FindEntryValue matches the key at
// 0x04; the lookup-table fields (count at 0x02, entry array at 0x08) are read
// through the CResLookup view by SchedRes_LookupKeyIndexed. Each element is zeroed by
// SchedRes_ZeroEntry at allocation time (SchedRes_AllocEntries).
struct CResEntry {
    u8 mField00;                // 0x00
    u8 mField01;                // 0x01
    u8 mCount;                  // 0x02 (CResLookup view)
    u8 mField03;                // 0x03
    u32 mKey;                   // 0x04 resource key
    CResLookupEntry* mEntries;  // 0x08 (CResLookup view)
    u32 mField0C;               // 0x0C
    u32 mField10;               // 0x10
    u32 mField14;               // 0x14
};

extern u8 sTexLoadCache[0x60]; // .bss buffer cleared by SchedRes_ClearBuf60

// Zero a 0x60-byte global buffer (tail call to memset).
extern "C" void SchedRes_ClearBuf60() {
    memset(sTexLoadCache, 0, 0x60);
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

// 12-byte last-loaded-texture cache entry (array at sTexLoadCache).
struct TexCacheEntryView {
    void* mMgr; // 0x00
    s32 mKey;   // 0x04
    s32 mWrap;  // 0x08
};

// Load texture `index` of `mgr` into tex map slot `texMap` with wrap mode
// `wrapMode`, unless that exact combination is already the cached last load
// (the cache is indexed by tex map slot).
extern "C" void SchedTexLoadCached(TexLoaderView* mgr, s32 index, GXTexMapID texMap,
                   GXTexWrapMode wrapMode) {
    TexCacheEntryView* cache = (TexCacheEntryView*)sTexLoadCache;
    if (!(cache[texMap].mMgr == mgr && index == cache[texMap].mKey &&
          wrapMode == cache[texMap].mWrap)) {
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
            f32 minLod = (f32)info->mMinLod;
            f32 maxLod = (f32)info->mMaxLod;
            GXInitTexObjLOD(&obj, (GXTexFilter)info->mMinFilt,
                            (GXTexFilter)info->mMagFilt, minLod, maxLod,
                            info->mLodBias, false, info->mEdgeLod, GX_ANISO_1);
        }
        GXLoadTexObj(&obj, texMap);
        cache[texMap].mMgr = mgr;
        cache[texMap].mKey = index;
        cache[texMap].mWrap = wrapMode;
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

extern "C" void* SchedRes_EntryByIndex(u8* self, int index) {
    ResTable* base = ((CResLookup*)self)->mResTable;
    ResTableIndex* entries = (ResTableIndex*)((u8*)base + base->indexOffset);
    return (u8*)base + entries[index].offset;
}

// Import from monolib/src/scn/CScn.cpp (retail symbol Scn_ResetAnim, C linkage).
extern "C" int Scn_ResetAnim(int a, int b, int c);

// Look up mTable[index]; a non-zero entry is forwarded to Scn_ResetAnim along
// with the caller's argument. The -1 third argument is dead at the callee
// (Scn_ResetAnim hard-codes 8 into r5) but kept for byte-identical codegen.
extern "C" int SchedRes_ResetAnimViaTable(CResLookup* self, int a, int index) {
    u32 entry = self->mTable[index];
    if (entry != 0) {
        return Scn_ResetAnim(a, entry, -1);
    }
    return 0;
}

// Search the entry table for a matching key and return its value.
extern "C" void* SchedRes_FindValueByKey(CResLookup* self, void* key) {
    if (key != NULL) {
        int count = self->mCount;
        int off;             // byte offset of the current entry
        int i = 0;
        off = 0;
        for (; i < count; i++) {
            CResLookupEntry* entry = (CResLookupEntry*)((u8*)self->mEntries + off);
            if (entry->mKey == key) {
                return self->mEntries[i].mValue;
            }
            off += 8;
        }
    }
    return NULL;
}

// Schedule-resource tables (.sbss): element base, element count and a live
// counter. Entries are 0x18 bytes (CResEntry).
CResEntry* spSchedResTable;
s32 sSchedResCount;
s32 sSchedResLiveCount[2];  // retail object is 8 bytes; only word 0 is written

// Allocate/initialize the schedule-resource table: round the byte size up
// to 32, record base/count, then zero-initialize each 0x18-byte entry.
extern "C" void* SchedRes_AllocEntries(u32 size, u32 count) {
    u32 off;
    s32 i;
    u32 aligned = ((size + 0x1f) & ~0x1f);
    u32 total = count * 0x18;
    void* end = (u8*)aligned + total;
    sSchedResCount = count;
    spSchedResTable = (CResEntry*)aligned;
    sSchedResLiveCount[0] = 0;
    off = 0;
    for (i = 0; i < (s32)count; i++) {
        SchedRes_ZeroEntry((CResEntry*)((u8*)spSchedResTable + off));
        off += 0x18;
    }
    return end;
}

#pragma push
#pragma auto_inline off  // noinline-ok: local one-shot helpers; -ipa off / -inline noauto both shrink __dt__804DEDCC and break extab-gate
extern "C" void SchedRes_ZeroEntry(CResEntry* pEntry) {
    pEntry->mField00 = 0;
    pEntry->mCount = 0;
    pEntry->mField03 = 0;
    pEntry->mKey = 0;
    pEntry->mEntries = 0;
    pEntry->mField10 = 0;
    pEntry->mField0C = 0;
}
#pragma pop

// Element destructor placeholder: retail callers test a nonzero
// "destroyed" result. extern "C" keeps the symbol/reloc name matching
// retail (__dt__804DEDCC); definition sits below SchedRes_FindOrClaim so MWCC's
// same-TU IPA cannot inline it into the free-slot scan.
// Resource-chain manager (retail symbol __dt__804DEDCC): allocates the
// entry's node/index arrays from the resource descriptor's count bytes,
// walks the chunked resource chain at entry+0x20 building free lists,
// then conditionally tears down. Returns 0 only on full teardown.
extern "C" int __dt__804DEDCC(CResEntry* entry, void* res,
                                                  u32 flagBit);

// Find the schedule-resource entry whose key matches `key`; if no live
// entry matches and the pool still has capacity, claim the first free
// (key == 0) entry instead. Returns 1 when an entry was claimed/found.
extern "C" s32 SchedRes_FindOrClaim(u32 key) {
    CResEntry* p = spSchedResTable;
    s32 n = sSchedResCount;
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
    if (n <= sSchedResLiveCount[0]) {
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
                    sSchedResLiveCount[0]++;
                    return 1;
                }
                return 0;
            }
            q++;
        } while (--c);
    }
    return 0;
}

#pragma push
#pragma auto_inline off  // noinline-ok: local one-shot helpers; -ipa off / -inline noauto both shrink __dt__804DEDCC and break extab-gate
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
            operator delete[](e->p8);
            e->p8 = NULL;
        }
        if (e->pc != NULL) {
            operator delete[](e->pc);
            e->pc = NULL;
        }
        u32 memActive = lbl_eu_8065FC18.mFlags;
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

// Release schedule-resource entries (retail SchedRes_ReleaseEntries):
// - key != 0: scan the entry array (counted do-while over the base pointer)
//   for a live entry whose key matches and whose 0x01 bit-6 flag equals
//   mFlags bit 11; on a match run the manager dtor over it, drop the live
//   counter when positive, return 1.
// - key == 0: sweep every live entry whose bit-6 flag matches, running the
//   dtor + counter drop on each, then return 1.
// Returns 0 only when the keyed scan found nothing.
extern "C" s32 SchedRes_ReleaseEntries(u32 key) {
    // bool-normalize via the neg/or/srwi idiom (MWCC_CASES: (u32)(-x|x)>>31,
    // neg operand FIRST, (u32) cast -> logical srwi).
    s32 flagBit = (lbl_eu_8065FC18.mFlags >> 12) & 1;
    flagBit = (u32)(-flagBit | flagBit) >> 31;
    if (key != 0) {
        CResEntry* p = spSchedResTable;
        for (s32 j = 0; j < sSchedResCount; j++, p++) {
            if (p->mKey != 0 && key == p->mKey &&
                (u32)(-((p->mField01 >> 7) & 1) |
                      ((p->mField01 >> 7) & 1)) >>
                        31 ==
                    flagBit) {
                __dt__804DF068((CSchedResMgr*)p);
                if (sSchedResLiveCount[0] > 0) {
                    sSchedResLiveCount[0]--;
                }
                return 1;
            }
        }
        return 0;
    }
    {
        CResEntry* base = spSchedResTable;
        for (s32 i = 0; i < sSchedResCount; i++) {
            CResEntry* e = &base[i];
            if (e->mKey != 0 &&
                (u32)(-((e->mField01 >> 7) & 1) |
                      ((e->mField01 >> 7) & 1)) >>
                        31 ==
                    flagBit) {
                __dt__804DF068((CSchedResMgr*)e);
                if (sSchedResLiveCount[0] > 0) {
                    sSchedResLiveCount[0]--;
                }
            }
        }
    }
    return 1;
}

// Definition moved below SchedRes_ReleaseEntries (see prototype above).
#pragma push
#pragma auto_inline off  // noinline-ok: local one-shot helpers; -ipa off / -inline noauto both shrink __dt__804DEDCC and break extab-gate
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
// entry's lookup table (SchedRes_LookupKeyIndexed) and report the entry/value pointers.
// Returns 1 on a match, 0 otherwise.
extern "C" s32 SchedRes_FindEntryValue(CResEntry** outEntry, u32* outValue, u32 key, u32 index) {
    s32 i = 0;
    while (i < sSchedResCount) {
        CResEntry* entry = &spSchedResTable[i];
        if (entry->mKey != 0 && key == entry->mKey) {
            u32 v = (u32)SchedRes_LookupKeyIndexed((CResLookup*)entry, index);
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
// 0xC-stride nodes (SchedElem_ClaimNode builds/traverses these lists).
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

// Effect object carrying the priority byte at +0x58 used by SchedElem_ClaimNode's
// sorted insertion.
struct CSchedFxObjView {
    u8 _00[0x58];
    u8 mKind58; // 0x58
};

void* lbl_eu_80665A30;
CSchedElem14* spSchedElemArray;
CSchedNodeC* spSchedNodeArray;
u32 sSchedElemCount;
u32 sSchedNodeCount;

// (Re)allocate the schedule-resource element arrays: 0x14-byte and 0xC-byte
// tables sized to `count`, each allocated from the MemManager handle when it
// fits the region's max alloc size (else NULL). Records the count in
// sSchedElemCount and clears the live counter first.
extern "C" void SchedRes_AllocElemTables(u32 count) {
    lbl_eu_80665A30 = 0;
    u32 s1 = count * 0x14;
    if (s1 <= mtl::MemManager::getMaxAllocSize(lbl_eu_8065FC18.mHandle)) {
        spSchedElemArray =
            (CSchedElem14*)mtl::MemManager::allocate_array(s1, lbl_eu_8065FC18.mHandle);
    } else {
        spSchedElemArray = NULL;
    }
    u32 s2 = count * 0xc;
    if (s2 <= mtl::MemManager::getMaxAllocSize(lbl_eu_8065FC18.mHandle)) {
        spSchedNodeArray =
            (CSchedNodeC*)mtl::MemManager::allocate_array(s2, lbl_eu_8065FC18.mHandle);
    } else {
        spSchedNodeArray = NULL;
    }
    sSchedElemCount = count;
}


// Release the schedule-resource arrays (element dtor).
void __dt__804DF744() {
    CSchedElem14* p1 = spSchedElemArray;
    lbl_eu_80665A30 = 0;
    if (p1) {
        operator delete[](p1);
        spSchedElemArray = NULL;
    }
    CSchedNodeC* p2 = spSchedNodeArray;
    if (p2) {
        operator delete[](p2);
        spSchedNodeArray = NULL;
    }
    sSchedElemCount = 0;
}

// Effect object pool at sEffectPool, looked up by handle via SchedFx_FetchByHandle.
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
extern CEffectPool sEffectPool;

// Effect-node pool at sEffectNodePool (node stride 0x338), looked up via
// SchedNode_FetchByIndex(index). Cross-TU callers treat nodes as EffectNode*.
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
extern CEffectNodePool sEffectNodePool;

// Effect-manager release (monolib/src/effect/code_804C8718.cpp).
extern "C" void EffRoot_InitDefaults(CEffectObj* obj);

// Effect-object constructor (monolib/src/effect/code_804C8718.cpp).
// EffObject is the full view of the 0x22C-byte pool element defined in that
// TU; this TU declares only the member it calls.
struct EffInitBlob;
struct EffObject {
    s32 Obj_Construct(const u8* base, const EffInitBlob* data, void* arg6, s32 arg7, u8 arg8);
};
//

// Re-initialize the schedule resource tables: clear both element arrays and
// reset the pool counters.
extern "C" void SchedRes_ReinitTables() {
    lbl_eu_80665A30 = 0;
    memset(spSchedElemArray, 0, (u32)sSchedElemCount * 0x14);
    memset(spSchedNodeArray, 0, (u32)sSchedElemCount * 0xC);
    sSchedNodeCount = 0;
}

extern "C" void SchedRes_ClearLiveCount() { lbl_eu_80665A30 = 0; }

// Claim or refresh a schedule-resource element for `key`:
// - scan the 0x14-byte element array for an entry whose key matches; if none
//   matches, claim the first free (key == 0) entry instead.
// - the claimed/matched entry gets a fresh node (from the 0xC-byte node pool
//   at the live-counter slot) pushed onto the appropriate sorted linked list
//   and the live counter is bumped.
// `obj` carries a priority byte at +0x58: entries with kind 0x40 are ordered
// by the float priority, all others by the signed kind byte.
extern "C" void SchedElem_ClaimNode(u32 key, CSchedFxObjView* obj, void* owner, f32 prio) {
    s32 count = sSchedElemCount;
    s32 i;
    bool claimed = false;
    for (i = 0; i < count; i++) {
        u32 v = spSchedElemArray[i].mVal;
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
    CSchedElem14* elem = &spSchedElemArray[i];
    s32 n = sSchedNodeCount;
    CSchedNodeC* node = &spSchedNodeArray[n];
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
        if (((*(u16*)((u8*)owner + 0x1C)) >> 4) & 1) {
            elem->mCount++;
        }
    } else {
        // Free entry: bind it to this key and insert the node into the
        // global priority list rooted at lbl_eu_80665A30.
        elem->mList = node;
        elem->mPrio = prio;
        elem->mVal = (u32)obj;
        elem->mCount = ((*(u16*)((u8*)owner + 0x1C)) >> 4) & 1;
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
    sSchedNodeCount = n + 1;
}

// Import from monolib/src/core/code_804E36DC.cpp (retail symbol schedClearItemSlots):
// initialize one pool item (all slots closed).
// (schedClearItemSlots is now CScheduleItem::ClearSlots; stale decl retired.)

// Allocate/initialize the schedule-item pool: round the byte size up to 32,
// record base/count, then run the item initializer over each 0x58-byte item.
extern "C" void* SchedItem_AllocPool(u32 size, s32 count) {
    s32 i;
    u32 aligned = (size + 0x1f) & ~0x1f;
    sScheduleItemPool.base = (CScheduleItem*)aligned;
    sScheduleItemPool.count = count;
    sScheduleItemPool.freeCount = 0;
    sScheduleItemPool.lastHandle = 0;
    i = 0;
    s32 off = 0;
    while (i < count) {
        ((CScheduleItem*)((u8*)sScheduleItemPool.base + off))->ClearSlots();
        off += 0x58;
        i++;
    }
    return (u8*)aligned + count * 0x58;
}

// Entry-blob resolver for allocated schedule items (retail symbol
// schedInitItemFromData, monolib/src/core/code_804E36DC.cpp).
// (schedInitItemFromData is now CScheduleItem::InitFromData; stale decl retired.)

// Allocate a free schedule item from the pool ring for (a, b, c): starting
// past the most recently released handle, claim the first slot whose
// mPoolIndex is negative, re-initialize it, stamp its pool index and
// advance the handle cursor. Returns its index, or -1 when the pool is
// full / fully scanned / entry resolution fails.
extern "C" s32 SchedItem_ClaimFree(u32 a, u32 b, u32 c) {
    CScheduleItem* node = NULL;
    if (sScheduleItemPool.freeCount < sScheduleItemPool.count) {
        s32 last = sScheduleItemPool.lastHandle;
        s32 idx = last;
        do {
            node = &sScheduleItemPool.base[idx];
            if (node->mPoolIndex >= 0) {
                idx = (idx + 1) % sScheduleItemPool.count;
            } else {
                node->ClearSlots();
                node->mPoolIndex = (s16)idx;
                sScheduleItemPool.freeCount++;
                sScheduleItemPool.lastHandle =
                    (idx + 1) % sScheduleItemPool.count;
                break;
            }
        } while (idx != last);
    }
    if (node == NULL) {
        return -1;
    }
    if (node->InitFromData((u8*)a, (const CScheduleEntryData*)b, (CSchedule*)c) != 0) {
        return node->mPoolIndex;
    }
    return -1;
}

// Release-by-index worker: pool indices are s32, so this layer re-validates
// the index it received. Retail keeps BOTH guards (s16 `handle < 0` in the
// handle-taking entry point and the s32 `index < 0` re-check here) as two
// branches off one compare - at Wii/1.1 a duplicated `x < 0` test on the SAME
// width folds to one branch, but the s16-vs-s32 pair does not (MWCC_CASES
// "duplicated handle guard").
static void SchedItem_ReleaseIndex(s32 index) {
    if (index < 0) {
        return;
    }
    if (sScheduleItemPool.count > index) {
        sScheduleItemPool.lastHandle = index;
        (&sScheduleItemPool.base[index])->ReleaseSlots();
        if (sScheduleItemPool.freeCount > 0) {
            sScheduleItemPool.freeCount--;
        }
    }
}

// Release a schedule item by handle (idempotent for invalid handles).
extern "C" void SchedItem_ReleaseHandle(s16 handle) {
    if (handle < 0) {
        return;
    }
    SchedItem_ReleaseIndex(handle);
}

// Look up a schedule item by handle; returns NULL for invalid handles.
extern "C" CScheduleItem* SchedItem_FetchByHandle(s16 handle) {
    if (handle < 0) {
        return NULL;
    }
    bool inRange = handle >= 0 && handle < sScheduleItemPool.count;
    if (inRange) {
        return &sScheduleItemPool.base[handle];
    }
    return NULL;
}

// Import from monolib/src/effect/code_804C8718.cpp (retail symbol
// EffSys_InitRoot): effect-manager initializer, run per pool element.
extern "C" void EffSys_InitRoot(CEffectObj* obj);

// Allocate/initialize the effect-object pool: round the byte size up to 32,
// record base/count, then run the effect-manager init over each 0x22C-byte
// object.
extern "C" void* SchedFx_AllocPool(u32 size, s32 count) {
    s32 i;
    u32 aligned = (size + 0x1f) & ~0x1f;
    sEffectPool.mBase = (CEffectObject*)aligned;
    sEffectPool.mCount = count;
    sEffectPool.mActiveCount = 0;
    sEffectPool.mLastHandle = 0;
    i = 0;
    s32 off = 0;
    while (i < count) {
        EffSys_InitRoot((CEffectObj*)((u8*)sEffectPool.mBase + off));
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
extern "C" s32 SchedFx_ClaimConstruct(const u8* base, const u8* data, u8* arg3, s32 arg4,
                  u8 arg5) {
    CEffectObj* node = NULL;
    if (sEffectPool.mActiveCount < sEffectPool.mCount) {
        s32 last = sEffectPool.mLastHandle;
        s32 idx = last;
        do {
            node = (CEffectObj*)((u8*)sEffectPool.mBase + idx * 0x22c);
            if (node->mPoolSlot >= 0) {
                idx = (idx + 1) % sEffectPool.mCount;
            } else {
                EffSys_InitRoot(node);
                node->mPoolSlot = (s16)idx;
                sEffectPool.mActiveCount++;
                sEffectPool.mLastHandle =
                    (idx + 1) % sEffectPool.mCount;
                break;
            }
        } while (idx != last);
    }
    if (node == NULL) {
        return -1;
    }
    if (reinterpret_cast<EffObject*>(node)->Obj_Construct(
            base, reinterpret_cast<const EffInitBlob*>(data), arg3, arg4, arg5) != 0) {
        return node->mPoolSlot;
    }
    // Constructor failed: rewind the handle cursor and release the object.
    s16 h = node->mPoolSlot;
    if (h >= 0 && h < sEffectPool.mCount) {
        sEffectPool.mLastHandle = h;
        EffRoot_InitDefaults((CEffectObj*)((u8*)sEffectPool.mBase + h * 0x22c));
        if (sEffectPool.mActiveCount > 0) {
            sEffectPool.mActiveCount--;
        }
    }
    return -1;
}

// Release-by-index worker (s32 pool index; see SchedItem_ReleaseIndex for the
// duplicated-guard rule this reproduces in the retail bytes).
static void SchedFx_ReleaseIndex(s32 index) {
    if (index < 0) {
        return;
    }
    if (sEffectPool.mCount > index) {
        sEffectPool.mLastHandle = index;
        EffRoot_InitDefaults((CEffectObj*)&sEffectPool.mBase[index]);
        if (sEffectPool.mActiveCount > 0) {
            sEffectPool.mActiveCount--;
        }
    }
}

// Release an effect object handle: remember it, run the effect-manager
// release, and drop the active-object count.
extern "C" void SchedFx_ReleaseHandle(s16 handle) {
    if (handle < 0) {
        return;
    }
    SchedFx_ReleaseIndex(handle);
}

extern "C" u32 SchedFx_CountActive() { return sEffectPool.mActiveCount; }

extern "C" u32 SchedFx_CountTotal() { return sEffectPool.mCount; }

// Look up an effect object by handle; NULL for out-of-range handles.
extern "C" CEffectObj* SchedFx_FetchByHandle(s16 handle) {
    if (handle < 0) {
        return NULL;
    }
    bool inRange = handle >= 0 && handle < sEffectPool.mCount;
    if (inRange) {
        return (CEffectObj*)&sEffectPool.mBase[handle];
    }
    return NULL;
}

// Effect-node pool element initializer (monolib/src/effect/code_804CC2B8.cpp).
// The owning header conflicts with this TU's lbl_eu_8065FC18 view, so use the
// real C++ declaration locally.
struct EffectStruct;
void EffFxInitStructLinks(EffectStruct* self);


// table: round the byte size up twice (32-byte granularity), record the pool
// base/count, run the node initializer over each element, then zero the
// follow-on table (returning its end pointer).
extern "C" void* SchedNode_AllocPool(u32 size, s32 count) {
    s32 i;
    u32 a = (size + 0x1f) & ~0x1f;
    u32 base = (a + 0x1f) & ~0x1f;
    sEffectNodePool.mCount = count;
    sEffectNodePool.mBase = (CEffectNode*)base;
    sEffectNodePool.mField08 = 0;
    sEffectNodePool.mLastHandle = 0;
    i = 0;
    s32 off = 0;
    while (i < count) {
        EffFxInitStructLinks(reinterpret_cast<EffectStruct*>((u8*)sEffectNodePool.mBase + off));
        off += 0x338;
        i++;
    }
    spEffectNodeWork = (void*)((base + count * 0x338 + 0x1f) & ~0x1f);
    s32 sz = count * 0x34;
    memset(spEffectNodeWork, 0, sz);
    return (void*)((u8*)spEffectNodeWork + sz);
}

// Effect-node activator (monolib/src/effect/code_804CC2B8.cpp,
// retail symbol EffFxInitSceneFromSrc).
extern "C" void EffFxInitSceneFromSrc(CEffectNode* node, CScheduleItem* item);

// Allocate a free effect node from the pool ring: starting past the most
// recently released handle, claim the first slot whose mark is negative
// (free), re-initialize it, stamp it with its index and advance the handle
// cursor. Returns its mark, or -1 when the pool is full / fully scanned.
extern "C" s32 SchedNode_ClaimActivate(CScheduleItem* item) {
    CEffectNode* node = NULL;
    if (sEffectNodePool.mField08 < sEffectNodePool.mCount) {
        s32 last = sEffectNodePool.mLastHandle;
        s32 idx = last;
        do {
            node = &sEffectNodePool.mBase[idx];
            if (node->mMark >= 0) {
                idx = (idx + 1) % sEffectNodePool.mCount;
            } else {
                EffFxInitStructLinks(reinterpret_cast<EffectStruct*>(node));
                node->mMark = (s16)idx;
                sEffectNodePool.mField08++;
                sEffectNodePool.mLastHandle =
                    (idx + 1) % sEffectNodePool.mCount;
                break;
            }
        } while (idx != last);
    }
    if (node == NULL) {
        return -1;
    }
    EffFxInitSceneFromSrc(node, item);
    return node->mMark;
}

// Import: effect-node pool element destructor (monolib/src/effect/
// code_804CC2B8.cpp, retail symbol __dt__804CC2E4). The node class is not
// decompiled yet; the element is passed as its opaque 0x338-byte view.
extern "C" void __dt__804CC2E4(CEffectNode* node);

// Release-by-index worker (s32 pool index; see SchedItem_ReleaseIndex).
static void SchedNode_ReleaseIndex(s32 index) {
    if (index < 0) {
        return;
    }
    if (sEffectNodePool.mCount > index) {
        sEffectNodePool.mLastHandle = index;
        __dt__804CC2E4((CEffectNode*)&sEffectNodePool.mBase[index]);
        if (sEffectNodePool.mField08 > 0) {
            sEffectNodePool.mField08--;
        }
    }
}

// Release an effect node by handle (idempotent for invalid handles).
extern "C" void SchedNode_ReleaseHandle(s16 handle) {
    if (handle < 0) {
        return;
    }
    SchedNode_ReleaseIndex(handle);
}

extern "C" u32 SchedNode_CountTotal() { return sEffectNodePool.mCount; }

// Look up an effect node by index; NULL for out-of-range indices.
extern "C" CEffectNode* SchedNode_FetchByIndex(s32 index) {
    if (index < 0) {
        return NULL;
    }
    bool inRange = index >= 0 && index < sEffectNodePool.mCount;
    if (inRange) {
        return &sEffectNodePool.mBase[index];
    }
    return NULL;
}

// Indexed handle lookup: returns the sbss pointer-slot value advanced by
// arg*0x34 (retail folds the constant slot address into an sda21 load).
extern "C" void* SchedNode_FetchSlotPtr(s32 arg) {
    if (arg < 0) {
        return NULL;
    }
    // Indexed 0x34-stride table: retail already multiplies (mulli), so the
    // typed indexed access matches.
    return ((u8(*)[0x34])spEffectNodeWork)[arg];
}

// Pick a random entry from a run of equal u16 values in a schedule entry
// blob. `base` holds u16 items `stride` bytes apart; the scan starts at
// `start` and continues while items equal the first (0x4000 is the
// terminator that ends the scan). When the run is longer than one item a
// random item is chosen: *out1 = start + rand, *out2 = run len - rand, and
// that item's pointer is returned; a single-item run stores start / 1.
extern "C" u8* SchedEntry_PickRandom(u8* base, u32 stride, s32 start, u8* out1, u8* out2) {
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
            s32 idx = start + r;
            *out1 = (u8)idx;
            result = base + stride * idx;
            *out2 = (u8)(count - r);
        } else {
            *out1 = (u8)start;
            *out2 = 1;
        }
    }
    return result;
}

// Entry-blob element accessed by SchedEntry_ResolveElem (s16 fields at +4/+6).
struct CEntryElem {
    u16 mField00;    // 0x00: element type (0x4000 = terminator)
    s16 mField02;    // 0x02
    s16 mField04;    // 0x04
    s16 mField06;    // 0x06
    u8 mRest[0x0E];  // 0x08
};

// 0x10-stride element kind with a vec3 at +0x04 (SchedAnim_LerpVec24 /
// SchedAnim_RefreshScaleVec): 4 + 12 = 16 = the caller stride.
struct CEntryVec10 {
    u8 _00[4];     // 0x00
    f32 mVec[3];   // 0x04 lerped triple
};

// Entry-blob interpreter (retail symbol SchedEntry_ResolveElem): resolves the element
// for an anim item by walking the blob with SchedEntry_PickRandom. Guards on the
// item flag byte (bit 7 armed, bit 3 clear), honors each candidate's u16
// limit against mField00, applies the per-object channel mask at obj+0x59,
// quantizes mField00 modulo the chosen element's limit, and advances
// mField00 by the caller rate. Returns the resolved element pointer.
extern "C" CEntryElem* SchedEntry_ResolveElem(CScheduleItem* itemArg, CEntryElem* entries,
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
        u8* ret = SchedEntry_PickRandom(base, stride, start, &item->mField06, &first);
        u16 lim = *(u16*)ret;
        if (lim == 0x4000) {
            item->field_0x04 |= 8;
            break;
        }
        f32 conv = (f32)lim;
        u8 useMask = 0;
        if (conv <= item->mField00) {
            s8 entryVal = (s8)((u8*)ret)[2];
            if (entryVal >= 0) {
                u8 entryMask = ((u8*)ret)[3];
                u8 take = 1;
                if (entryMask != 0) {
                    s8 m = (s8)*(u8*)(obj + 0x59);
                    u8 hit = 0;
                    if (m != 0) {
                        if ((entryMask & (1 << (m - 1))) != 0) {
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
                item->mField05 = (u8)entryVal;
                u8* ret2 = SchedEntry_PickRandom(base, stride, ((u8*)ret)[2],
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
        if (!(0.0f <= f1)) {
            break;
        }
        item->mField05 = item->mField06;
        item->mField07 = first;
        u8* ret3 = SchedEntry_PickRandom(
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
// retail symbol CEVec3LerpPS): out = a + (b - a) * t for three f32s.
extern "C" void CEVec3LerpPS(void* out, const void* a, const void* b, f32 t);

// Frame-span rate helper: reads the u16 frame values at entries[index*span],
// converts them to float (via the 0x43300000 magic-double bit pattern so the
// 2^52 adjustors reference the retail pool labels, docs/MWCC_CASES.md §7i),
// and clamps the result to 1.0. A 0x4000 entry
// (terminator) yields rate 1.0.
extern "C" f32 SchedAnim_FrameRate(CSchedAnimItem* item, CEntryElem* entries, s32 index) {
    f32 result = 0.0f;
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
        d = 1.0f;
    }
    f32 rate = 1.0f / d;
    if (f == result) {
        return result;
    }
    f32 r = f * rate;
    if (r < 0.0f) {
        r = 0.0f;
    } else if (r > 1.0f) {
        r = 1.0f;
    }
    return r;
}

// Initialize one anim item from a 0x28-stride entry blob: reset the frame
// state and copy the selected element's three vectors into item+0x0C/0x18/0x24
// (all zeros when no blob), then run the usual terminator flag check.
extern "C" void SchedAnim_InitVec28(CSchedAnimItem* item, u8* base) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
        CEntryWideElem* ret = (CEntryWideElem*)SchedEntry_PickRandom(base, 0x28, 0,
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
    } else {
        item->mVecColor = schedZero;
        item->mVecPos = schedZero;
        item->mVecScale = schedZero;
    }
}

// Advance the item's three anim vectors: look up entry element 0x28, then
// lerp each of the element's +0x04 / +0x10 / +0x1C vectors toward the
// (frame span & 0xFF)-th element (0x28-byte stride) at `f1`, writing into
// the item's +0x0C / +0x18 / +0x24 vectors.
extern "C" void SchedAnim_AdvanceVec28(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 0x28, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = SchedAnim_FrameRate(item, entries, 0x28);
    if (rate < 0.0f) {
        return;
    }
    u8* b = (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x28;
    CEVec3LerpPS(&item->mField0C, (u8*)ret + 4, b + 4, rate);
    CEVec3LerpPS(&item->mField18, (u8*)ret + 0x10, b + 0x10, rate);
    CEVec3LerpPS(&item->mField24, (u8*)ret + 0x1C, b + 0x1C, rate);
}

// Retail duplicate of SchedAnim_InitVec28.
extern "C" void SchedAnim_InitVec28B(CSchedAnimItem* item, u8* base) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
        CEntryWideElem* ret = (CEntryWideElem*)SchedEntry_PickRandom(base, 0x28, 0,
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
    } else {
        item->mVecColor = schedZero;
        item->mVecPos = schedZero;
        item->mVecScale = schedZero;
    }
}

// Same anim-vector refresh as SchedAnim_AdvanceVec28 (retail duplicate body).
extern "C" void SchedAnim_AdvanceVec28B(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 0x28, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = SchedAnim_FrameRate(item, entries, 0x28);
    if (rate < 0.0f) {
        return;
    }
    u8* b = (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x28;
    CEVec3LerpPS(&item->mField0C, (u8*)ret + 4, b + 4, rate);
    CEVec3LerpPS(&item->mField18, (u8*)ret + 0x10, b + 0x10, rate);
    CEVec3LerpPS(&item->mField24, (u8*)ret + 0x1C, b + 0x1C, rate);
}

// Re-roll the two slot frame lengths of an anim item: for each channel draw
// mtRand, blend it with the float preceding the blob (or 1.0 when absent)
// into the channel rate (+0x14 / +0x10), then quantize the selected entry
// element's u16 frame count against the new rate into the slot pair
// (+0x0E / +0x0C). Finally run the usual terminator flag check.
extern "C" void SchedAnim_RerollSlots(CSchedAnimItem* item, u8* base) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mSlot0E = 1;
    item->mSlot0C = 1;
    item->mField10 = 1.0f;
    item->mField14 = 1.0f;
    if (base == NULL) {
        return;
    }
    CEntryU16PairElem* elem =
        (CEntryU16PairElem*)(base + item->mField05 * 8);

    // Channel A
    f32 d = (f32)(s32)ml::math::mtRand() / 2147483600.0f - 0.5f;
    f32 scale;
    if (base != NULL) {
        scale = *(f32*)(base - 8);
    } else {
        scale = 0.0f;
    }
    f32 t = 1.0f - d * scale;
    item->mField14 = t;
    s32 v = (s32)((f64)(u32)elem->mA * t);
    if ((v & 0xFFFF) != 0) {
        item->mSlot0E = (s16)v;
    }

    // Channel B
    d = (f32)(s32)ml::math::mtRand() / 2147483600.0f - 0.5f;
    if (base != NULL) {
        scale = *(f32*)(base - 4);
    } else {
        scale = 0.0f;
    }
    t = 1.0f - d * scale;
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
extern "C" void SchedAnim_BlendFrameU16(CSchedAnimItem* item, u8* base) {
    if (base == NULL) {
        return;
    }
    if (((item->field_0x04 >> 4) & 1) == 0) {
        return;
    }
    CEntryU16PairElem* ret = (CEntryU16PairElem*)(base + item->mField05 * 8);
    s32 rdraw = ml::math::mtRand();
    f32 rnd = (f32)rdraw / 2147483600.0f - 0.5f;
    f32 scale;
    if (base != NULL) {
        scale = *(f32*)(base - 8);
    } else {
        scale = 0.0f;
    }
    f32 t = 1.0f - rnd * scale;
    item->mField14 = t;
    u16 aw = ret->mA;
    f32 fha = aw;
    s32 v = (s32)(fha * t);
    if ((v & 0xFFFF) != 0) {
        item->mSlot0E = (s16)v;
    }
    rnd = (f32)ml::math::mtRand() / 2147483600.0f - 0.5f;
    if (base != NULL) {
        scale = *(f32*)(base - 4);
    } else {
        scale = 0.0f;
    }
    t = 1.0f - rnd * scale;
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
extern "C" void SchedAnim_AdvanceColor8(CSchedAnimItem* item, CEntryElem* entries, u8* arg3,
                   f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 8, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = SchedAnim_FrameRate(item, entries, 8);
    if (rate < 0.0f) {
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
    s16 s = (s16)(10.0f *
                  (0.001f + (rate * (fd - fc) + fc) * item->mField10));
    item->mSlot0C = (s16)(0.1f * s);
}

// Initialize one anim item from a 0x14-stride entry blob with a random
// direction vector: reset the frame state, copy the selected element's first
// vector into item+0x10, clear the 2-bit deviation field based on the blob
// header byte, then (when the scale factor preceding the blob exceeds 1.0)
// fill item+0x1C with a random vector, normalize it and scale everything by
// that factor; otherwise zero it.
extern "C" void SchedAnim_InitRandDir14(CSchedAnimItem* item, u8* base, f32* ref) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mRef0 = ref;
    if (base == NULL) {
        *(AnimVec3*)&item->mField1C = schedZero;
        *(AnimVec3*)&item->mField10 = schedZero;
        item->mField28 = 0.0f;
        return;
    }
    CEntryWideElem* ret =
        (CEntryWideElem*)SchedEntry_PickRandom(base, 0x14, 0, &item->mField05,
                                       &item->mField07);
    *(AnimVec3*)&item->mField10 = ret->mV0;
    // clear the 2-bit deviation field from the blob's type byte
    item->field_0x04 &= (u8)~0x06;
    item->mField28 = ret->mV1.x;
    f32 scale;
    if (base != NULL) {
        scale = *(f32*)(base - 0xC);
    } else {
        scale = 0.0f;
    }
    if (scale > 0.0f) {
        randVec__6CERandFPQ22ml5CVec3((ml::CVec3*)&item->mField1C);
        f32 len2 = item->mField1C * item->mField1C +
                   item->mField20 * item->mField20 +
                   item->mField24 * item->mField24;
        if (len2 == 0.0f) {
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

// Spline evaluator (defined below; retail symbol SchedAnim_EvalSpline).
extern "C" void SchedAnim_EvalSpline(CScheduleItem* unused, AnimVec3* outPos, f32* outVal,
                   s32 mode, void** args, f32 t);

// Spline-mode anim updater (0x14-stride entry blob). Two modes selected by
// the raw word at item+0x00: modes 9-11 feed a four-point window around the
// resolved element to the spline evaluator SchedAnim_EvalSpline; other values run
// the vector-advance path (lerp the +4 vector toward the frame-offset
// element, accumulate the +0x10/+0x14/+0x18 channels, and advance the
// +0x28 scalar channel with clamping against the element value).
extern "C" void SchedAnim_UpdateSpline14(CSchedAnimItem* item, CEntryElem* entries, u8* arg3,
                   f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 0x14,
                                    arg3, f1);
    u32 modeBits = *(u32*)(void*)&item->mField00;
    if (modeBits - 9 > 2) {
        // standard advance path
        if (ret != NULL) {
            f32 rate = SchedAnim_FrameRate(item, entries, 0x14);
            if (!(rate < 0.0f)) {
                u8* b = (u8*)ret +
                        ((item->mField06 - item->mField05) & 0xFF) * 0x14;
                CEVec3LerpPS(&item->mField10, (u8*)ret + 4, b + 4, rate);
                item->mField10 += item->mField1C;
                item->mField14 += item->mField20;
                item->mField18 += item->mField24;
                if (((item->field_0x04 >> 1) & 3) == 0) {
                    f32 startF = *(f32*)((u8*)ret + 0x10);
                    f32 endF = *(f32*)((u8*)ret +
                                       (item->mField06 - item->mField05) *
                                           0x14 +
                                       0x10);
                    item->mField28 = rate * (endF - startF) + startF;
                }
            }
        }
        if (((item->field_0x04 >> 1) & 3) != 0) {
            u8* p = (u8*)ret;
            if (p == NULL) {
                p = (u8*)entries + item->mField05 * 0x14;
            }
            u16 w = *(u16*)(p + (item->mField06 - item->mField05) * 0x14);
            if (w < 0x4000) {
                f32 rate = SchedAnim_FrameRate(item, entries, 0x14);
                if (!(rate < 0.0f)) {
                    f32 startF = *(f32*)(p + 0x10);
                    f32 endF = *(f32*)(p + (item->mField06 - item->mField05) *
                                               0x14 +
                                           0x10);
                    f32 v =
                        item->mField28 + (rate * (endF - startF) + startF);
                    item->mField28 = v;
                    if (v > endF) {
                        item->mField28 = endF;
                    }
                }
            } else {
                item->mField28 = item->mField28 + *(f32*)(p + 0x10);
            }
        }
        return;
    }
    // spline path (modes 9-11)
    if (ret == NULL) {
        return;
    }
    f32 rate = SchedAnim_FrameRate(item, entries, 0x14);
    u8* pts[4] = {(u8*)ret - 0x14, (u8*)ret, (u8*)ret + 0x14,
                  (u8*)ret + 0x28};
    if (item->mField05 == 0) {
        pts[0] = (u8*)ret;
    }
    if (*(u16*)pts[1] == 0x4000) {
        pts[1] -= 0x14;
        pts[2] -= 0x14;
    }
    if (*(u16*)pts[2] == 0x4000) {
        pts[2] -= 0x14;
    }
    SchedAnim_EvalSpline((CScheduleItem*)item, (AnimVec3*)&item->mField10,
                  &item->mField28, (s32)modeBits, (void**)pts, rate);
}

// Point record consumed by the spline evaluator SchedAnim_EvalSpline: ml::CVec3
// position at +0x04, scalar channel at +0x10.
struct SplinePt {
    u8 _00[4];
    AnimVec3 mPos;
    f32 mVal;
};

void SchedAnim_EvalSpline(CScheduleItem* unused, AnimVec3* outPos, f32* outVal,
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
        f32 h10 = t3 - 2.0f * t2 + t;
        f32 h00 = 0.0f + 2.0f * t3 - 3.0f * t2;
        f32 h01 = (-2.0f) * t3 + 3.0f * t2;
        f32 h11 = t3 - t2;
        f32 k = 0.5f;
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
        if (t >= 1.0f) {
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
extern "C" void SchedAnim_InitRand08(CSchedAnimItem* item, u8* base) {
    f32 one = 0.0f;
    item->mField00 = one;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base == NULL) {
        item->mField10 = one;
        item->mField0C = 1.0f;
        return;
    }
    u8* ret = SchedEntry_PickRandom(base, 8, 0, &item->mField05, &item->mField07);
    f32 rnd = (f32)ml::math::mtRand() / 2147483600.0f - 0.5f;
    f32 scale;
    if (base == NULL) {
        scale = one;
    } else {
        scale = *(f32*)(base - 4);
    }
    item->mField0C = 1.0f - rnd * scale;
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
extern "C" void SchedAnim_AdvanceCursor(CSchedAnimItem* item, CEntryElem* entries, u8* arg4, f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 8, arg4, f1);
    if (ret == NULL) {
        return;
    }
    f32 r = SchedAnim_FrameRate(item, entries, 8);
    if (r < 0.0f) {
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
extern "C" void SchedAnim_InitSlots(CSchedAnimItem* item, u8* base) {
    CScheduleItem* si = (CScheduleItem*)item;
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    si->mSlots[2] = -1;
    si->mSlots[3] = -1;
    if (base != NULL) {
        u8* ret = SchedEntry_PickRandom(base, 8, 0, &item->mField05, &item->mField07);
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
            // NOTE(open-item us-804e5e08): single-expression, split-statement,
            // named-temp and u32-temp shapes all emit rlwinm 25,31; retail has
            // 25,23 (clear-only-bit24 without subreg narrowing). Residual is
            // this one mask plus arg-color reg swaps.
            item->field_0x04 = (u8)((item->field_0x04 | 0x8) & ~0x80);
        }
    }
}

// Refresh slot handles 2/3 from the entry-blob element selected by
// SchedEntry_ResolveElem at index 8.
extern "C" void SchedAnim_RefreshSlotHandles(CScheduleItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem(item, entries, 8, arg3, f1);
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
extern "C" void SchedAnim_InitQuad14(CSchedAnimItem* item, u8* base) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mQuad0C = schedZero4;
    if (base != NULL) {
        CEntryWide4Elem* ret =
            (CEntryWide4Elem*)SchedEntry_PickRandom(base, 0x14, 0, &item->mField05,
                                            &item->mField07);
        if (ret->mType == 0x4000) {
            item->field_0x04 |= 0x8;
        } else {
            item->mQuad0C = ret->mVec;
            // Flag bit pairs when the stored vector deviates from (1,1,1,1).
            item->mScaleFlag = (item->mField0C != 0.0f ||
                                item->mField10 != 0.0f);
            item->mScaleFlag =
                (u8)(item->mScaleFlag |
                     (u8)((item->mField14 != 0.0f ||
                           item->mField18 != 0.0f)
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

extern "C" void SchedAnim_SplitHue(CScheduleItem* unused, HueSplitOut* out,
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
    if (0.0f == mx) {
        return;
    }
    f32 y = (mx - mn) / mx;
    out->mDelta = y;
    if (y == 0.0f) {
        return;
    }
    // Hue sector selected by which channel equals the max.
    if (ch[0] == mx) {
        out->mHue = (ch[1] - ch[2]) / (mx - mn);
    } else if (ch[1] == mx) {
        out->mHue = 2.0f + (ch[2] - ch[0]) / (mx - mn);
    } else { // ch[2] == mx
        out->mHue = 4.0f + (ch[0] - ch[1]) / (mx - mn);
    }
    f32 h = out->mHue / 6.0f;
    out->mHue = h;
    if (h < 0.0f) {
        out->mHue = 1.0f + h;
    }
}

// 4-float paired lerp kernel (monolib/src/effect/code_804DB938.cpp,
// retail symbol CEVec4LerpPS): out = a + (b - a) * t for four f32s.
extern "C" void CEVec4LerpPS(void* out, const void* a, const void* b, f32 t);

// Advance the item's 4-float scale channel (+0x0C..+0x18): lerp the selected
// element's +4 vector toward the (frame span & 0xFF)-th element (0x14 stride)
// at the current rate, then update the two deviate bits of the 0x04 flag
// byte's 2-bit field (bits 5-6): bit5 = x/y left the 1.0 identity, bit6 = z/w.
extern "C" void SchedAnim_AdvanceScale4(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 0x14, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = SchedAnim_FrameRate(item, entries, 0x14);
    if (rate < 0.0f) {
        return;
    }
    CEVec4LerpPS(&item->mField0C, (u8*)ret + 4,
                  (u8*)ret + (item->mField06 - item->mField05) * 0x14 + 4,
                  rate);
    // deviate bit 5: x/y
    u8 devXY = 0;
    if (item->mField0C || item->mField10) {
        devXY = 1;
    }
    u8 sf2;
    u8 sf;
    if (devXY != 0) {
        sf = item->mScaleFlag | 1;
    } else {
        sf = item->mScaleFlag & (u8)~1;
    }
    item->mScaleFlag = sf;
    // deviate bit 6: z/w
    u8 devZW = 0;
    if (item->mField14 || item->mField18) {
        devZW = 1;
    }
    if (devZW != 0) {
        sf2 = item->mScaleFlag | 2;
    } else {
        sf2 = item->mScaleFlag & 1;
    }
    item->mScaleFlag = sf2;
}

// Re-initialize a schedule item from a 0x10-stride entry blob with three
// pre-scaled random channels: reset the frame state, run the terminator scan
// (SchedEntry_PickRandom), read the three floats preceding the blob into the rate
// slots (+0x0C/+0x10/+0x14), then draw three mtRand rates (+0x18/+0x1C/+0x20)
// - each channel reuses the previous rate while its pre-scale is < 1.0, and
// sets flag 0x10 when its pre-scale exceeds 1.0. When flag 0x10 was set the
// rates are folded into the pre-scales, the element's +4 vector (copied to
// +0x24..+0x2C) is scaled by the new rates, and the usual terminator check
// runs. All rate/scale slots default to 2.0 when no blob is present.
extern "C" void SchedAnim_InitRand3Ch(CSchedAnimItem* item, u8* base) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base == NULL) {
        item->mField14 = 1.0f;
        item->mField10 = 1.0f;
        item->mField0C = 1.0f;
        item->mField20 = 1.0f;
        item->mField1C = 1.0f;
        item->mField18 = 1.0f;
        item->mField2C = 1.0f;
        item->mField28 = 1.0f;
        item->mField24 = 1.0f;
        return;
    }
    u8* ret = SchedEntry_PickRandom(base, 0x10, 0, &item->mField05, &item->mField07);
    if (base != NULL) {
        item->mField0C = *(f32*)(base - 0x18);
    } else {
        item->mField0C = 1.0f;
    }
    if (base != NULL) {
        item->mField10 = *(f32*)(base - 0x14);
    } else {
        item->mField10 = 1.0f;
    }
    if (base != NULL) {
        item->mField14 = *(f32*)(base - 0x10);
    } else {
        item->mField14 = 1.0f;
    }
    f32* src24 = (f32*)((u8*)ret + 4);
    item->mField24 = src24[0];
    item->mField28 = src24[1];
    item->mField2C = src24[2];
    // channel A
    f32 rnd = s32ToF32((s32)ml::math::mtRand()) / 2147483600.0f -
              0.5f;
    f32 dScale = base != NULL ? *(f32*)(base - 0xc) : 0.0f;
    item->mField18 = 1.0f - dScale * rnd;
    if (dScale > 0.0f) {
        item->field_0x04 |= 0x10;
    }
    // channel B: reuse channel A's rate while the pre-scale is < 1.0
    f32 dScale2 = base != NULL ? *(f32*)(base - 8) : (-1.0f);
    if (dScale2 < 0.0f) {
        item->mField1C = item->mField18;
    } else {
        f32 rnd2 = s32ToF32((s32)ml::math::mtRand()) / 2147483600.0f -
                   0.5f;
        item->mField1C = 1.0f - dScale2 * rnd2;
        if (dScale2 > 0.0f) {
            item->field_0x04 |= 0x10;
        }
    }
    // channel C: reuse channel A's rate while the pre-scale is < 1.0
    f32 dScale3 = base != NULL ? *(f32*)(base - 4) : (-1.0f);
    if (dScale3 < 0.0f) {
        item->mField20 = item->mField18;
    } else {
        f32 rnd3 = s32ToF32((s32)ml::math::mtRand()) / 2147483600.0f -
                   0.5f;
        item->mField20 = 1.0f - dScale3 * rnd3;
        if (dScale3 > 0.0f) {
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
extern "C" void SchedAnim_LerpVec24(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 0x10, arg3, f1);
    if (ret != NULL) {
        f32 rate = SchedAnim_FrameRate(item, entries, 0x10);
        if (rate < 0.0f) {
            return;
        }
        CEVec3LerpPS(&item->mField24, ((CEntryVec10*)ret)->mVec,
                      ((CEntryVec10*)ret + ((item->mField06 - item->mField05) & 0xFF))->mVec,
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
extern "C" void SchedAnim_InitPosDir(CSchedAnimItem* item, u8* base, u8 arg3) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mByte0C = arg3;
    if (base != NULL) {
        CEntryVecElem* ret =
            (CEntryVecElem*)SchedEntry_PickRandom(base, 0x10, 0, &item->mField05,
                                          &item->mField07);
        f32 sc;
    if (base != NULL) {
        sc = *(f32*)(base - 0x18);
    } else {
        sc = 0.0f;
    }
    item->mField10 = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 0x14);
    } else {
        sc = 0.0f;
    }
    item->mField14 = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 0x10);
    } else {
        sc = 0.0f;
    }
    item->mField18 = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 0xC);
    } else {
        sc = 0.0f;
    }
    item->mField1C = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 8);
    } else {
        sc = 0.0f;
    }
    item->mField20 = sc;
    if (base != NULL) {
        sc = *(f32*)(base - 4);
    } else {
        sc = 0.0f;
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
    if (!(item->mField1C == 0.0f &&
          item->mField20 == 0.0f &&
          item->mField24 == 0.0f)) {
        // Retail re-loads these floats instead of CSE-ing them
        // with the copies taken above.
        f32 rnd = (f32)ml::math::mtRand() / 2147483600.0f -
                  0.5f;
        if (base != NULL) {
            sc = *(f32*)(base - 0xC);
        } else {
            sc = 0.0f;
        }
        item->mField1C = 1.0f - rnd * sc;
        rnd = (f32)ml::math::mtRand() / 2147483600.0f -
              0.5f;
        if (base != NULL) {
            sc = *(f32*)(base - 8);
        } else {
            sc = 0.0f;
        }
        item->mField20 = 1.0f - rnd * sc;
        rnd = (f32)ml::math::mtRand() / 2147483600.0f -
              0.5f;
        if (base != NULL) {
            sc = *(f32*)(base - 4);
        } else {
            sc = 0.0f;
        }
        item->mField24 = 1.0f - rnd * sc;
        item->field_0x04 |= 0x10;
        item->mField10 *= item->mField1C;
        item->mField14 *= item->mField20;
        item->mField18 *= item->mField24;
    }
    item->mExt28 += item->mField10;
    item->mExt2C += item->mField14;
    item->mExt30 += item->mField18;
    if (item->mByte0C != 0) {
        item->mExt28 *= (-1.0f);
        item->mExt2C *= (-1.0f);
        item->mExt30 *= (-1.0f);
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
        item->mExt30 = 0.0f;
        item->mExt2C = 0.0f;
        item->mExt28 = 0.0f;
        item->mField18 = 0.0f;
        item->mField14 = 0.0f;
        item->mField10 = 0.0f;
        item->mField24 = 1.0f;
        item->mField20 = 1.0f;
        item->mField1C = 1.0f;
    }
}

// Advance the item's working quad at +0x28 according to the blob's mode
// byte: mode 0 lerps it toward the frame-span-th element and adds the
// item's velocity (+0x10); mode 1 additionally scales it by the factors at
// +0x34..+0x3C first; mode 2 scales it down by the +0x0C amount, then adds
// the span element scaled by the caller's factor. The final +0x0C decay is
// applied whenever it is nonzero.
extern "C" void SchedAnim_AdvanceQuad(CSchedAnimItem* item, CEntryElem* entries, u8* arg3,
                   f32 f1) {
    CSchedAnimItem* self = item;
    CEntryElem* blob = entries;
    f32 mult = f1;
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)self, blob, 0x10,
                                    arg3, mult);
    if (ret == NULL) {
        return;
    }
    f32 rate = SchedAnim_FrameRate(self, blob, 0x10);
    if (rate < 0.0f) {
        return;
    }
    u8 type = blob ? *((u8*)blob - 0x1B) : 0;
    u8* retp = (u8*)ret;
    u8 span = item->mField06 - item->mField05;
    u8* src = retp + span * 0x10 + 4;
    if (type == 0) {
        CEVec3LerpPS((u8*)self + 0x28, retp + 4, src, rate);
        self->mExt28 += self->mField10;
        self->mExt2C += self->mField14;
        self->mExt30 += self->mField18;
    } else if (type == 1) {
        CEVec3LerpPS((u8*)self + 0x28, retp + 4, src, rate);
        self->mExt28 *= self->mExt34;
        self->mExt2C *= self->mExt38;
        self->mExt30 *= self->mExt3C;
        self->mExt28 += self->mField10;
        self->mExt2C += self->mField14;
        self->mExt30 += self->mField18;
    } else if (type == 2) {
        if (self->mByte0C != 0) {
            self->mExt28 *= (-1.0f);
            self->mExt2C *= (-1.0f);
            self->mExt30 *= (-1.0f);
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
        self->mExt28 *= (-1.0f);
        self->mExt2C *= (-1.0f);
        self->mExt30 *= (-1.0f);
    }
}

// Initialize one anim item from a 0x10-stride entry blob (color variant):
// reset the frame state, validate the four floats preceding the blob against
// 1.0, then either snap the rate quad (+0x0C..+0x18) to the pool upper bound
// or rebuild it from mtRand draws blended with those floats, copy the selected
// element's color halfword/float data into item+0x1C..+0x20, set the 0x10
// rate flag, and run the usual terminator check.
extern "C" void SchedAnim_InitColor10(CSchedAnimItem* item, u8* base) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
    CEntryVecElem* ret =
        (CEntryVecElem*)SchedEntry_PickRandom(base, 0x10, 0, &item->mField05,
                                      &item->mField07);
    f32 v14 = (base != NULL) ? *(f32*)(base - 0x14) : 0.0f;
    bool eq;
    if (v14 == 0.0f) {
        // fcmpo/bgt: continues only when not greater (lt/eq/unordered)
        f32 v10 = (base != NULL) ? *(f32*)(base - 0x10) : (-1.0f);
        if (!(v10 > 0.0f)) {
            f32 v0C = (base != NULL) ? *(f32*)(base - 0xC) : (-1.0f);
            if (!(v0C > 0.0f)) {
                f32 v08 =
                    (base != NULL) ? *(f32*)(base - 8) : 0.0f;
                eq = (v08 == 0.0f);
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
        f32 c = 1.0f;
        item->mField24 = c;
        item->mField18 = c;
        item->mField14 = c;
        item->mField10 = c;
        item->mField0C = c;
    } else {
        // These floats are re-loaded rather than reused from the
        // validation reads above.
        f32 s10 = (base != NULL) ? *(f32*)(base - 0x10)
                                 : (-1.0f);
        f32 s0C = (base != NULL) ? *(f32*)(base - 0xC)
                                 : (-1.0f);
        if (s10 < 0.0f && s0C < 0.0f) {
            item->mField14 = 1.0f;
            item->mField10 = 1.0f;
            item->mField0C = 1.0f;
            f32 rnd = (f32)ml::math::mtRand() / 2147483600.0f -
                      0.5f;
            f32 sc = (base != NULL) ? *(f32*)(base - 0x14)
                                    : 0.0f;
            item->mField24 = 1.0f - rnd * sc;
        } else {
            f32 rnd = (f32)ml::math::mtRand() / 2147483600.0f -
                      0.5f;
            f32 sc = (base != NULL) ? *(f32*)(base - 0x14)
                                    : 0.0f;
            item->mField0C = 1.0f - rnd * sc;
            rnd = (f32)ml::math::mtRand() / 2147483600.0f -
                  0.5f;
            sc = (base != NULL) ? *(f32*)(base - 0x10)
                                : (-1.0f);
            item->mField10 = 1.0f - rnd * sc;
            f32 c = 1.0f;
            item->mField24 = c;
            rnd = (f32)ml::math::mtRand() / 2147483600.0f -
                  0.5f;
            sc = (base != NULL) ? *(f32*)(base - 0xC)
                                : (-1.0f);
            item->mField14 = c - rnd * sc;
        }
        f32 rnd = (f32)ml::math::mtRand() / 2147483600.0f -
                  0.5f;
        f32 sc = (base != NULL) ? *(f32*)(base - 8) : 0.0f;
        u8 flag = item->field_0x04 | 0x10;
        item->mField18 = 1.0f - rnd * sc;
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
        item->mField18 = 1.0f;
        item->mField14 = 1.0f;
        item->mField10 = 1.0f;
        item->mField0C = 1.0f;
        item->mByte1F = 0xFF;
        item->mByte1E = 0xFF;
        item->mByte1D = 0xFF;
        item->mByte1C = 0xFF;
        item->mField24 = 1.0f;
        item->mField20 = 1.0f;
    }
}

// Advance the item's 4-byte color at +0x1C and float at +0x20: lerp entry
// element 0x10's +4 byte components toward the frame-span-th element
// (0x10-byte stride) at the current rate.
extern "C" void SchedAnim_AdvanceColor10(CSchedAnimItem* item, CEntryElem* entries, u8* arg3,
                   f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 0x10, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = SchedAnim_FrameRate(item, entries, 0x10);
    if (rate < 0.0f) {
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

extern "C" void SchedAnim_NoOp(){}

// Initialize one anim item from a 0x10-stride entry blob: reset the frame
// state, copy the selected element's three component references into
// item+0x0C..0x18, flag bit 0x60 when the referenced components deviate from
// 1.0, then check the run tail for the blob terminator (0x80 / 0x08 flags).
extern "C" void SchedAnim_InitVec10(CSchedAnimItem* item, u8* base) {
    item->mField00 = 0.0f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
        u8* ret = SchedEntry_PickRandom(base, 0x10, 0, &item->mField05, &item->mField07);
        CEntryVecElem* elem = (CEntryVecElem*)ret;
        item->mVecScale = elem->mVec;
        // Flag bit 0x60 when the stored vector deviates from (1,1,1).
        u8 flag = false;
        if (item->mField0C != 0.0f || item->mField10 != 0.0f ||
            item->mField14 != 0.0f) {
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
extern "C" void SchedAnim_RefreshScaleVec(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = SchedEntry_ResolveElem((CScheduleItem*)item, entries, 0x10, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = SchedAnim_FrameRate(item, entries, 0x10);
    if (rate < 0.0f) {
        return;
    }
    CEVec3LerpPS(&item->mField0C, ((CEntryVec10*)ret)->mVec,
                  ((CEntryVec10*)ret + ((item->mField06 - item->mField05) & 0xFF))->mVec,
                  rate);
    // flag starts cleared and is set only when the vector deviates from 1.0
    u8 flag = false;
    if (item->mField0C || item->mField10 || item->mField14) {
        flag = true;
    }
    item->mScaleFlag = flag;
}


// HSV-like -> RGB conversion: `hsv` holds (hue, saturation, value); the hue
// is scaled by the pool constant and truncated to pick one of six sector
// permutations of (v, t, p) = (value, value*(1-s*f), value*(1-s)), each
// clamped to the upper bound; all three results are finally scaled by `mul`.
// `self` is unused in retail.
extern "C" void SchedAnim_HsvToRgb(CScheduleItem* self, AnimVec3* out, const AnimVec3* hsv,
                   f32 mul) {
    f32 h = hsv->x;
    if (h < 1.0f) {
        h = 0.9999f;
    }
    f32 hf = 6.0f * h;
    s32 sector = (s32)hf;
    f32 s = hsv->y;
    if (s == 0.0f) {
        out->x = hsv->z;
        out->y = hsv->z;
        out->z = hsv->z;
    } else {
        f32 v = hsv->z;
        f32 frac = hf - (f32)sector;
        f32 p = v * (1.0f - s);
        f32 q = v * (1.0f - s * frac);
        f32 t = v * (1.0f - s * (1.0f - frac));
        if (p > 1.0f) {
            p = 1.0f;
        }
        if (q > 1.0f) {
            q = 1.0f;
        }
        if (t > 1.0f) {
            t = 1.0f;
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
u8 sTexLoadCache[96];
CScheduleItemPool sScheduleItemPool;
CEffectPool sEffectPool;
CEffectNodePool sEffectNodePool;
// [.sbss] 0x80665A20-0x80665A50 (48B) zero-fill (80665A20..80665A40 already defined above)
void* spEffectNodeWork;
u8 sSchedScratch[8];
