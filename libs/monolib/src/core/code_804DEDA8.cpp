// Auto-scaffolded catalog TU for monolib/src/core/code_804DEDA8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <string.h>

#include "monolib/core/CScheduleItem.hpp"
#include "monolib/core/code_804DEDA8.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/MemManager.hpp"

#include <revolution/GX.h>
#include <revolution/os/OSFastCast.h>

// View of the schedule item used by the anim/entry helpers (func_804E04D4,
// func_804E18CC): f32 at 0x00, byte frame fields at 0x05/0x06, f32s at
// 0x0C/0x10. Overlaps CScheduleItem (same object, different class view).
struct CSchedAnimItem {
    f32 mField00;   // 0x00
    u8 field_0x04;  // 0x04: anim flag byte (bit 0x20 = non-identity scale)
    u8 mField05;    // 0x05: entry-run start (written by func_804E0188)
    u8 mField06;    // 0x06
    u8 mField07;    // 0x07: entry-run tail count (written by func_804E0188)
    u8 _pad08[4];   // 0x08..0x0B
    f32 mField0C;   // 0x0C
    f32 mField10;   // 0x10
    f32 mField14;   // 0x14
    f32 mField18;   // 0x18
    f32 mField1C;   // 0x1C
    f32 mField20;   // 0x20
    f32 mField24;   // 0x24
    f32 mField28;   // 0x28
    f32 mField2C;   // 0x2C
};

// Bitfield view of the anim flag byte at +0x04: bit 0x80 = entries-loaded,
// bits 0x40/0x20 = 2-bit scale/rot mode (bit 1 = 0x40 rot, bit 0 = 0x20
// scale), bit 0x08 = run-touches-terminator flag. Named bitfields make MWCC
// emit the retail extrwi/rlwimi field ops (ScheduleList.cpp pattern).
struct CSchedAnimFlags {
    u8 mLoaded : 1;  // 0x80
    u8 mMode : 2;    // 0x40 (bit 1), 0x20 (bit 0)
    u8 : 1;          // 0x10
    u8 mTerm : 1;    // 0x08
    u8 : 3;          // 0x04..0x01
};

// View of the schedule item used by the slot-handle anim helpers
// (func_804E0CF0 / func_804E0B94): the 0x0C/0x0E s16 slot handles overlap
// the anim view's f32 mField0C (same object, different class view).
struct CSchedAnimSlots {
    f32 mField00;   // 0x00
    u8 field_0x04;  // 0x04: flag byte (bit 0x10 = jitter enabled)
    u8 mField05;    // 0x05: entry-run start
    u8 mField06;    // 0x06
    u8 mField07;    // 0x07
    u8 _pad08[4];   // 0x08..0x0B
    s16 mSlot2;     // 0x0C
    s16 mSlot3;     // 0x0E
    f32 mField10;   // 0x10
    f32 mField14;   // 0x14
};

// 8-byte element of the entry array returned by func_804E0248 (f32 at +4).
struct CEntryFloatElem {
    u8 mPad[4];   // 0x00
    f32 mF32;     // 0x04
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
// above the frame stores (docs/MWCC_REFERENCE.md lfs-hoist scheduling fix).
extern f64 lbl_eu_8066B288;      // 2^52 (u8 -> f32 magic) for the GX LOD setup
extern const f32 lbl_eu_8066B290;
extern f64 lbl_eu_8066B298;
extern f64 lbl_eu_8066B2A0;
extern f32 lbl_eu_8066B2A8;
extern const f32 lbl_eu_8066B2AC; // 2147483647.0f (INT_MAX), rand()/INT_MAX divisor
// 0.5f: subtracts the 0.5 offset from rand()/INT_MAX (range [-0.5, 0.5))
extern const f32 lbl_eu_8066B2B0;
extern const f32 lbl_eu_8066B2B4; // slot-handle re-scale constants (func_804E0B94)
extern const f32 lbl_eu_8066B2B8;
extern const f32 lbl_eu_8066B2BC;

// Forward declaration for the schedule-resource entry destructor below.
struct CResEntry;

// Resource-entry destructor (retail symbol __dt__804DEDCC): re-initialize a
// free 0x18-byte entry (self) from a source entry (src) and a flag bit, then
// report whether the slot was re-used. Stub body until that target is
// matched; the 3-arg signature keeps callers (func_804DF3D0) passing the
// retail (entry, src, flag) register order.
s32 __dt__804DEDCC(CResEntry* self, CResEntry* src, s32 flag) { return 0; }

extern void* lbl_eu_80665A44; // .sbss pointer slot (4 bytes)

// Zero one 0x18-byte schedule-resource entry. never_inline so callers (e.g.
// func_804DF344) keep the retail direct `bl` instead of an inlined loop body.
// extern "C" keeps the call-site reloc name matching retail (func_804DEDA8).
extern "C" DECOMP_DONT_INLINE void func_804DEDA8(void* r3) {
    char* base = (char*)r3;
    base[0] = 0;
    base[2] = 0;
    base[3] = 0;
    *(int*)(base + 4) = 0;
    *(int*)(base + 8) = 0;
    *(int*)(base + 0x10) = 0;
    *(int*)(base + 0xc) = 0;
}

void __dt__804DEDCC(){}

// Destructor of the static schedule-resource manager (object at 0x804DF068):
// release the two heap arrays and the MemManager block, then zero the object.
// extern "C" keeps the symbol/reloc name matching retail (__dt__804DF068).
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

// 12-byte texture cache slots (lbl_eu_806616B8), keyed by (res, index, wrap).
struct CTexCacheEntry {
    void* mRes;   // 0x00
    s32 mIndex;   // 0x04
    s32 mWrap;    // 0x08
};

// Texture resource header: base pointer at +0x10, index-table offset at +8.
struct CTexResHeader {
    u8 mPad[0x10];
    u8* mBase;  // 0x10
};

// 8-byte index entries: u32 offset into the resource base.
struct CTexIndexEntry {
    u32 mOffset;  // 0x00
    u32 mPad04;   // 0x04
};

// One texture entry inside the resource base: GX init parameters.
struct CTexEntry {
    u16 mHeight;   // 0x00
    u16 mWidth;    // 0x02
    u32 mFormat;   // 0x04
    u32 mImageOff; // 0x08: image pointer offset relative to the base
    u8 mPad0C[8];  // 0x0C..0x14
    u32 mMinFilt;  // 0x14
    u32 mMagFilt;  // 0x18
    f32 mLodBias;  // 0x1C
    u8 mEdgeLod;   // 0x20
    u8 mMinLod;    // 0x21
    u8 mMaxLod;    // 0x22
    u8 mPad23;     // 0x23
};

// Load a texture entry into GX unless the (res, index, wrap) triple is
// already cached: resolve base->indexTable[index] (8-byte stride u32
// offsets), build a stack GXTexObj, then remember the triple. mipmap is the
// (minLod != maxLod) comparison; the LOD block only runs when it is set.
void func_804DF164(void* res, s32 index, s32 cacheIndex, s32 wrap) {
    u8* e = &lbl_eu_806616B8[cacheIndex * 12];
    if (*(void**)e == res && *(s32*)(e + 4) == index && *(s32*)(e + 8) == wrap) {
        return;
    }
    u8* base = ((CTexResHeader*)res)->mBase;
    CTexIndexEntry* indexTable =
        (CTexIndexEntry*)(base + *(u32*)(base + 8));
    CTexEntry* entry = (CTexEntry*)(base + indexTable[index].mOffset);
    GXTexObj obj;
    bool mip = entry->mMinLod != entry->mMaxLod;
    GXInitTexObj(&obj, base + entry->mImageOff, entry->mWidth, entry->mHeight,
                 (GXTexFmt)entry->mFormat, (GXTexWrapMode)wrap,
                 (GXTexWrapMode)wrap, (GXBool)mip);
    if (mip) {
        GXInitTexObjLOD(&obj, (GXTexFilter)entry->mMinFilt,
                        (GXTexFilter)entry->mMagFilt, (f32)entry->mMinLod,
                        (f32)entry->mMaxLod, entry->mLodBias, (GXBool)0,
                        (GXBool)entry->mEdgeLod, (GXAnisotropy)0);
    }
    GXLoadTexObj(&obj, (GXTexMapID)cacheIndex);
    *(void**)e = res;
    ((CTexCacheEntry*)lbl_eu_806616B8)[cacheIndex].mIndex = index;
    ((CTexCacheEntry*)lbl_eu_806616B8)[cacheIndex].mWrap = wrap;
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

// Allocate/initialize the schedule-resource table: round the byte size up to
// 32, record base/count, then zero-initialize each 0x18-byte entry.
void* func_804DF344(u32 size, u32 count) {
    s32 off;
    s32 i;
    s32 n = (s32)count;
    u32 aligned = (size + 0x1f) & ~0x1f;
    u8* end = (u8*)aligned + count * 0x18;
    lbl_eu_80665A24 = (s32)count;
    lbl_eu_80665A20 = (CResEntry*)aligned;
    lbl_eu_80665A28[0] = 0;
    off = 0;
    i = 0;
    while (i < n) {
        func_804DEDA8(&lbl_eu_80665A20[i]);
        i++;
    }
    return end;
}

void func_804DF3D0(){}

// Release schedule-resource entries (retail func_804DF4BC): with a non-zero
// key, release the first entry whose key matches AND whose bit-7 flag equals
// the global mode flag (bit 12 of lbl_eu_8065FC18); with a NULL key, release
// every entry whose bit-7 flag matches. Each release drops the live counter
// (lbl_eu_80665A28[0]). Returns 1 when at least one entry was released.
s32 func_804DF4BC(s32 key) {
    u32 flag = ((lbl_eu_8065FC18.mFlags & 0x1000) >> 12) != 0;
    if (key != 0) {
        s32 i;
        for (i = 0; i < lbl_eu_80665A24; i++) {
            CResEntry* entry = &lbl_eu_80665A20[i];
            if (entry->mKey != 0 && key == entry->mKey &&
                flag == (((entry->mField01 & 0x80) >> 7) != 0)) {
                __dt__804DF068((CSchedResMgr*)entry);
                if (lbl_eu_80665A28[0] > 0) {
                    lbl_eu_80665A28[0]--;
                }
                return 1;
            }
        }
    } else {
        s32 i;
        for (i = 0; i < lbl_eu_80665A24; i++) {
            CResEntry* entry = &lbl_eu_80665A20[i];
            if (entry->mKey != 0 &&
                flag == (((entry->mField01 & 0x80) >> 7) != 0)) {
                __dt__804DF068((CSchedResMgr*)entry);
                if (lbl_eu_80665A28[0] > 0) {
                    lbl_eu_80665A28[0]--;
                }
            }
        }
        return 1;
    }
    return 0;
}

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
// array, the 0xC-byte element array and the element count.
void* lbl_eu_80665A30;
void* lbl_eu_80665A34;
void* lbl_eu_80665A38;
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
        lbl_eu_80665A34 = mtl::MemManager::allocate_array(s1, lbl_eu_8065FC18.mHandle);
    } else {
        lbl_eu_80665A34 = 0;
    }
    u32 s2 = count * 0xc;
    if (s2 <= mtl::MemManager::getMaxAllocSize(lbl_eu_8065FC18.mHandle)) {
        lbl_eu_80665A38 = mtl::MemManager::allocate_array(s2, lbl_eu_8065FC18.mHandle);
    } else {
        lbl_eu_80665A38 = 0;
    }
    lbl_eu_80665A3C = count;
}

void __dla__FPv(void*);

void __dt__804DF744() {
    void* p1 = lbl_eu_80665A34;
    lbl_eu_80665A30 = 0;
    if (p1) {
        __dla__FPv(p1);
        lbl_eu_80665A34 = 0;
    }
    void* p2 = lbl_eu_80665A38;
    if (p2) {
        __dla__FPv(p2);
        lbl_eu_80665A38 = 0;
    }
    lbl_eu_80665A3C = 0;
}

// Effect object pool at lbl_eu_80661728, looked up by handle via func_804DFEAC.
// CEffectObj in CScheduleItem.hpp is a partial view of the 0x22C-byte elements.
struct CEffectObject {
    u8 mData[0x1E];
    s16 mField1E;  // 0x1E: slot handle (-1 = free slot)
    u8 mRest[0x22C - 0x20];
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
    u8 mData[0x338];
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

// Effect-object constructor (monolib/src/effect/code_804C8718.cpp): returns
// nonzero when the object was initialized. Signature mirrors the retail
// call-site register order (r3 = object, r4..r8 = the five init args).
extern "C" s32 func_804C8830(void* self, const u8* base, const void* data,
                             void* arg6, s32 arg7, u8 arg8);

// Re-initialize the schedule resource tables: clear both element arrays and
// reset the pool counters.
void func_804DF7A4() {
    lbl_eu_80665A30 = 0;
    memset(lbl_eu_80665A34, 0, (u32)lbl_eu_80665A3C * 0x14);
    memset(lbl_eu_80665A38, 0, (u32)lbl_eu_80665A3C * 0xC);
    lbl_eu_80665A40 = 0;
}

void func_804DF7FC() { lbl_eu_80665A30 = 0; }

void func_804DF808(){}

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

void func_804DFA84(){}

// Release a schedule item by handle (idempotent for invalid handles).
// Retail duplicates the `handle < 0` guard (two blt to the same exit - the
// dead-branch CSE family, reproducible under GC/3.0a5.2 with three separate
// goto guards; Wii/1.1 merges them).
extern "C" void func_804DFB88(s16 handle) {
    if (handle < 0) goto end;
    if (handle < 0) goto end;
    if (lbl_eu_80661718.count <= handle) goto end;
    lbl_eu_80661718.lastHandle = handle;
    func_804E3E2C(&lbl_eu_80661718.base[handle]);
    if (lbl_eu_80661718.freeCount > 0) {
        lbl_eu_80661718.freeCount--;
    }
end:
    return;
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

// Allocate an effect object from the pool (lbl_eu_80661728): scan from the
// last released handle for a free slot (field_0x1E < 0), initialize it with
// the effect manager (func_804C8718), then construct the object
// (func_804C8830). On construction failure the slot is released again and -1
// is returned; success returns the object's slot handle.
s32 func_804DFCC4(const u8* a, const void* b, void* c, s32 d, u8 e) {
    CEffectObject* obj;
    if (lbl_eu_80661728.mActiveCount >= lbl_eu_80661728.mCount) {
        obj = NULL;
    } else {
        s32 handle = lbl_eu_80661728.mLastHandle;
        while (true) {
            CEffectObject* cur = &lbl_eu_80661728.mBase[handle];
            if (cur->mField1E < 0) {
                func_804C8718((CEffectObj*)cur);
                cur->mField1E = (s16)handle;
                lbl_eu_80661728.mActiveCount++;
                lbl_eu_80661728.mLastHandle =
                    (handle + 1) % lbl_eu_80661728.mCount;
                obj = cur;
                break;
            }
            handle = (handle + 1) % lbl_eu_80661728.mCount;
            if (handle == lbl_eu_80661728.mLastHandle) {
                obj = NULL;
                break;
            }
        }
    }
    if (obj == NULL) {
        return -1;
    }
    if (func_804C8830(obj, a, b, c, d, e) != 0) {
        return obj->mField1E;
    }
    s16 h = obj->mField1E;
    if (h >= 0 && h < lbl_eu_80661728.mCount) {
        lbl_eu_80661728.mLastHandle = h;
        func_804C8790((CEffectObj*)&lbl_eu_80661728.mBase[h]);
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
    if (!(handle < 0) && handle < lbl_eu_80661728.mCount) {
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
    u32 base = (a + 0x1f) & ~0x1f;
    CEffectNodePool* pool = &lbl_eu_80661738;
    pool->mCount = count;
    pool->mBase = (CEffectNode*)base;
    pool->mField08 = 0;
    pool->mLastHandle = 0;
    for (s32 i = 0; i < (s32)count; i++) {
        func_804CC2B8((CEffectNode*)((u8*)lbl_eu_80661738.mBase + i * 0x338));
    }
    u32 tail = (base + count * 0x338 + 0x1f) & ~0x1f;
    lbl_eu_80665A44 = (void*)tail;
    memset((void*)tail, 0, count * 0x34);
    return (void*)((u8*)lbl_eu_80665A44 + count * 0x34);
}

void func_804DFFA8(){}

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
    if (handle < 0 || handle < 0 || lbl_eu_80661738.mCount <= handle) {
        return;
    }
    CEffectNodePool* pool = &lbl_eu_80661738;
    pool->mLastHandle = handle;
    __dt__804CC2E4(&pool->mBase[handle]);
    if (pool->mField08 > 0) {
        pool->mField08--;
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

// Entry lookup (retail symbol func_804E0248): returns entries[index] for a
// valid schedule item. Not decompiled yet - pure import so callers
// (func_804E1A44 / func_804E18CC) keep their retail direct `bl`.
extern "C" CEntryElem* func_804E0248(CScheduleItem* item, CEntryElem* entries,
                                     s32 index, u8* arg4, f32 f1);

// Paired-single vector lerp kernels (monolib/src/effect/code_804DB938.cpp,
// retail symbols func_804DD89C / func_804DD8C8): out = a + (b - a) * t for
// three / four f32s.
extern "C" void func_804DD89C(void* out, const void* a, const void* b, f32 t);
extern "C" void func_804DD8C8(void* out, const void* a, const void* b, f32 t);

// Frame-span rate helper: reads the u16 frame values at entries[index*span],
// converts the span (hi - lo) to double for the rate denominator, and clamps
// the result into [1.0, lbl_eu_8066B2A8]. A 0x4000 entry (terminator) yields
// rate 1.0. The int->double conversions build the 0x43300000 bit pattern by
// hand so the 2^52 magic subtract references the retail pool labels
// (docs/MWCC_REFERENCE.md §7i) instead of a TU-local @N constant.
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
    union { f64 d; u32 w[2]; } u1;
    union { f64 d; u32 w[2]; } u2;
    u1.w[1] = (u32)lo;
    u1.w[0] = 0x43300000;
    f64 lo_d = u1.d - lbl_eu_8066B298;
    u2.w[1] = (u32)diff ^ 0x80000000;
    u2.w[0] = 0x43300000;
    f64 d = u2.d - lbl_eu_8066B2A0;
    f32 f = item->mField00 - d;
    if (d == result) {
        d = lbl_eu_8066B2A8;
    }
    f32 rate = (f32)(lbl_eu_8066B2A8 / d);
    f32 r = f * rate;
    if (r < lbl_eu_8066B290) {
        r = lbl_eu_8066B290;
    } else if (r > lbl_eu_8066B2A8) {
        r = lbl_eu_8066B2A8;
    }
    return r;
}

// Initialize the item's three anim vectors (scale/rot/pos block at +0x0C)
// from entry element 0x00 (0x28-byte stride): copy the element's three
// 3-float vectors, then set the 0x80/0x8 flag bits according to whether the
// run extends to the blob terminator. With no entry blob the three vectors
// are zeroed.
void func_804E0580(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
        CEntryElem* ret =
            (CEntryElem*)func_804E0188(base, 0x28, 0, &item->mField05, &item->mField07);
        *(ml::CVec3*)&item->mField0C = *(ml::CVec3*)((u8*)ret + 4);
        *(ml::CVec3*)&item->mField18 = *(ml::CVec3*)((u8*)ret + 0x10);
        *(ml::CVec3*)&item->mField24 = *(ml::CVec3*)((u8*)ret + 0x1C);
        if (*(u16*)(base + (u8)(item->mField05 + item->mField07) * 0x28) != 0x4000) {
            item->field_0x04 |= 0x80;
        } else {
            s32 flags = item->field_0x04;
            item->field_0x04 = (u8)((flags | 0x8) & ~0x80);
        }
    } else {
        ml::CVec3 zero = ml::CVec3::zero;
        *(ml::CVec3*)&item->mField24 = zero;
        *(ml::CVec3*)&item->mField18 = zero;
        *(ml::CVec3*)&item->mField0C = zero;
    }
}

// Advance the item's three anim vectors: look up entry element 0x28, then
// lerp each of the element's +0x04 / +0x10 / +0x1C vectors toward the
// (frame span & 0xFF)-th element (0x28-byte stride) at `f1`, writing into
// the item's +0x0C / +0x18 / +0x24 vectors.
void func_804E06B4(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x28, arg3, f1);
    if (ret != NULL) {
        f32 rate = func_804E04D4(item, entries, 0x28);
        if (rate >= lbl_eu_8066B290) {
            u8* b = (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x28;
            func_804DD89C(&item->mField0C, (u8*)ret + 4, b + 4, rate);
            func_804DD89C(&item->mField18, (u8*)ret + 0x10, b + 0x10, rate);
            func_804DD89C(&item->mField24, (u8*)ret + 0x1C, b + 0x1C, rate);
        }
    }
}

// Retail duplicate of func_804E0580 (same body: copy the three anim vectors
// from entry element 0x00 or zero them, then set the terminator flag bits).
void func_804E0788(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
        CEntryElem* ret =
            (CEntryElem*)func_804E0188(base, 0x28, 0, &item->mField05, &item->mField07);
        *(ml::CVec3*)&item->mField0C = *(ml::CVec3*)((u8*)ret + 4);
        *(ml::CVec3*)&item->mField18 = *(ml::CVec3*)((u8*)ret + 0x10);
        *(ml::CVec3*)&item->mField24 = *(ml::CVec3*)((u8*)ret + 0x1C);
        if (*(u16*)(base + (u8)(item->mField05 + item->mField07) * 0x28) != 0x4000) {
            item->field_0x04 |= 0x80;
        } else {
            s32 flags = item->field_0x04;
            item->field_0x04 = (u8)((flags | 0x8) & ~0x80);
        }
    } else {
        ml::CVec3 zero = ml::CVec3::zero;
        *(ml::CVec3*)&item->mField24 = zero;
        *(ml::CVec3*)&item->mField18 = zero;
        *(ml::CVec3*)&item->mField0C = zero;
    }
}

// Same anim-vector refresh as func_804E06B4 (retail duplicate body).
void func_804E08BC(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x28, arg3, f1);
    if (ret != NULL) {
        f32 rate = func_804E04D4(item, entries, 0x28);
        if (rate >= lbl_eu_8066B290) {
            u8* b = (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x28;
            func_804DD89C(&item->mField0C, (u8*)ret + 4, b + 4, rate);
            func_804DD89C(&item->mField18, (u8*)ret + 0x10, b + 0x10, rate);
            func_804DD89C(&item->mField24, (u8*)ret + 0x1C, b + 0x1C, rate);
        }
    }
}

void func_804E0990(){}

// Advance the item's two slot handles (0x0C/0x0E) from the entry blob at
// index 8: interpolate the u16 handle pairs (stride 8) at the current rate,
// scale by the item's 0x14/0x10 factors, and fast-cast to u16 through the
// GQR3 helper (retail psq_st qr3). The second handle is re-scaled by the
// sdata2 constants back into the handle range before rounding to s16.
void func_804E0B94(CSchedAnimSlots* item, CEntryElem* entries, u8* arg3,
                   f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 8, arg3, f1);
    if (ret == NULL) {
        return;
    }
    f32 rate = func_804E04D4((CSchedAnimItem*)item, entries, 8);
    if (rate < lbl_eu_8066B290) {
        return;
    }
    u8* next = (u8*)ret + (item->mField06 - item->mField05) * 8;
    f32 cur = (f32)*(u16*)((u8*)ret + 4);
    f32 nxt = (f32)*(u16*)(next + 4);
    item->mSlot3 = __OSf32tou16((rate * (nxt - cur) + cur) * item->mField14);
    cur = (f32)*(u16*)((u8*)ret + 6);
    nxt = (f32)*(u16*)(next + 6);
    f32 interp = rate * (nxt - cur) + cur;
    u16 v = __OSf32tou16(lbl_eu_8066B2B4 *
                         (lbl_eu_8066B2B8 + interp * item->mField10));
    item->mSlot2 = (s16)(lbl_eu_8066B2BC * (f32)v);
}

// Randomize the item's 0x10/0x14 factor slots and refresh its 0x0C/0x0E slot
// handles from the entry blob (stride 8) when the 0x10 flag bit is set: the
// factor is jittered around lbl_eu_8066B2A8 by rand()/INT_MAX - 0.5 scaled
// by the rate float before the entry blob (1.0 when none), then the u16
// element times the factor is rounded into the slot handle.
void func_804E0CF0(CSchedAnimSlots* item, u8* base) {
    if (base == NULL || ((item->field_0x04 >> 4) & 1) == 0) {
        return;
    }
    u8* entry = base + item->mField05 * 8;
    f32 rnd = (f32)ml::math::mtRand() / lbl_eu_8066B2AC - lbl_eu_8066B2B0;
    f32 rate = (base != NULL) ? *(f32*)(base - 8) : lbl_eu_8066B290;
    item->mField14 = lbl_eu_8066B2A8 - rnd * rate;
    s32 v = (s32)((f32)*(u16*)(entry + 4) * item->mField14);
    if ((v & 0xFFFF) != 0) {
        item->mSlot3 = (s16)v;
    }
    f32 rnd2 = (f32)ml::math::mtRand() / lbl_eu_8066B2AC - lbl_eu_8066B2B0;
    f32 rate2 = (base != NULL) ? *(f32*)(base - 4) : lbl_eu_8066B290;
    item->mField10 = lbl_eu_8066B2A8 - rnd2 * rate2;
    s32 v2 = (s32)((f32)*(u16*)(entry + 6) * item->mField10);
    if ((v2 & 0xFFFF) != 0) {
        item->mSlot2 = (s16)v2;
    }
}

void func_804E0E48(){}

void func_804E1044(){}

void func_804E1294(){}

// Initialize a random-jitter anim item: run the entry scan at stride 8, then
// jitter the 0x0C factor around lbl_eu_8066B2A8 by rand()/INT_MAX - 0.5f
// scaled by the rate float just before the entry blob (1.0f when none), and
// scale the element's +4 float into +0x10. Sets the terminator flag bits.
void func_804E17A4(CSchedAnimItem* item, u8* base) {
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (base != NULL) {
        CEntryElem* ret =
            (CEntryElem*)func_804E0188(base, 8, 0, &item->mField05, &item->mField07);
        union {
            f64 d;
            u32 w[2];
        } u;
        u.w[1] = (u32)ml::math::mtRand() ^ 0x80000000;
        u.w[0] = 0x43300000;
        f64 d = u.d - lbl_eu_8066B2A0;
        f32 rnd = (f32)d / lbl_eu_8066B2AC - lbl_eu_8066B2B0;
        f32 rate = (base != NULL) ? *(f32*)(base - 4) : lbl_eu_8066B290;
        item->mField0C = lbl_eu_8066B2A8 - rnd * rate;
        item->mField10 = ((CEntryFloatElem*)ret)->mF32 * item->mField0C;
        if (*(u16*)(base + (u8)(item->mField05 + item->mField07) * 8) != 0x4000) {
            item->field_0x04 |= 0x80;
        } else {
            item->field_0x04 = (item->field_0x04 | 0x8) & ~0x80;
        }
    } else {
        item->mField10 = lbl_eu_8066B290;
        item->mField0C = lbl_eu_8066B2A8;
    }
}

// Advance the item's animation cursor: look up the entry element at index 8,
// then interpolate the element's float table between slot 0 and the
// (span & 0x1F)-th entry, scaled by the item's 0x0C factor.
extern "C" void func_804E18CC(CSchedAnimItem* item, CEntryElem* entries, u8* arg4, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 8, arg4, f1);
    if (ret != NULL) {
        f32 r = func_804E04D4(item, entries, 8);
        if (r >= lbl_eu_8066B290) {
            CEntryFloatElem* arr = (CEntryFloatElem*)ret;
            f32 f3 = arr[0].mF32;
            u8 diff = item->mField06 - item->mField05;
            f32 f0 = item->mField0C;
            f32 f2 = arr[diff & 0x1F].mF32;
            item->mField10 = (r * (f2 - f3) + f3) * f0;
        }
    }
}

// View of a schedule item used by func_804E196C's init path: byte fields at
// 0x04..0x07 plus the s16 slot-handle pair at 0x0C/0x0E (overlaps the anim
// view's f32 mField0C - same object, different class view).
struct CSchedInitView {
    f32 mField00;   // 0x00
    u8 mFlags;      // 0x04
    u8 mField05;    // 0x05
    u8 mField06;    // 0x06
    u8 mField07;    // 0x07
    u8 _pad08[4];   // 0x08..0x0B
    s16 mSlot2;     // 0x0C
    s16 mSlot3;     // 0x0E
};

// Initialize one schedule item's anim/entry state: set the base frame
// factor, reset the run-window bytes and slot handles, then (when an entry
// blob is present) load the run's slot handles and set the 0x04 flag bits
// according to whether the run extends to the blob terminator.
extern "C" void func_804E196C(CSchedInitView* item, u8* base) {
    bool hasBase = base != NULL;
    item->mField00 = lbl_eu_8066B290;
    item->mFlags = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    item->mSlot2 = -1;
    item->mSlot3 = -1;
    if (hasBase) {
        CEntryElem* elem =
            (CEntryElem*)func_804E0188(base, 8, 0, &item->mField05, &item->mField07);
        if (elem->mField00 != 0x4000) {
            item->mSlot2 = elem->mField04;
            item->mSlot3 = elem->mField06;
        }
        if (*(u16*)(base + (u8)(item->mField05 + item->mField07) * 8) != 0x4000) {
            item->mFlags |= 0x80;
        } else {
            item->mFlags = (item->mFlags | 0x8) & ~0x80;
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

void func_804E1AA8(){}

// Advance the item's 4-float anim block at +0x0C: lerp it from entry
// element 0x14's +4 floats toward the (frame span)-th element (0x14-byte
// stride) at the current rate, then refresh the 2-bit scale/rot mode in the
// 0x04 flag byte (bit 0x20/0x40) from whether the two vector halves are
// still (1,1,1).
void func_804E1C1C(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x14, arg3, f1);
    if (ret != NULL) {
        f32 rate = func_804E04D4(item, entries, 0x14);
        if (rate < lbl_eu_8066B290) {
            return;
        }
        u8* b = (u8*)ret + (item->mField06 - item->mField05) * 0x14;
        func_804DD8C8(&item->mField0C, (u8*)ret + 4, b + 4, rate);
        bool scale1 = item->mField0C != lbl_eu_8066B290 ||
                      item->mField10 != lbl_eu_8066B290;
        CSchedAnimFlags* flags = (CSchedAnimFlags*)&item->field_0x04;
        u8 mode1;
        if (scale1) {
            mode1 = flags->mMode | 1;
        } else {
            mode1 = flags->mMode & 2;
        }
        flags->mMode = mode1;
        bool scale2 = item->mField14 != lbl_eu_8066B290 ||
                      item->mField18 != lbl_eu_8066B290;
        u8 mode2;
        if (scale2) {
            mode2 = flags->mMode | 2;
        } else {
            mode2 = flags->mMode & 1;
        }
        flags->mMode = mode2;
    }
}

void func_804E1D50(){}

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

void func_804E214C(){}

void func_804E24A8(){}

void func_804E26D8(){}

void func_804E2A5C(){}

void func_804E2B54(){}

// Initialize the item's scale vector at +0x0C from entry element 0x00
// (0x10-byte stride): copy the element's 3-float vector, set the 0x20 flag
// bit when the vector is not (1,1,1), then set the terminator flag bits.
// With no entry blob the vector is zeroed.
void func_804E2D8C(CSchedAnimItem* item, u8* base) {
    u8* b = base;
    item->mField00 = lbl_eu_8066B290;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    if (b != NULL) {
        CEntryElem* ret =
            (CEntryElem*)func_804E0188(b, 0x10, 0, &item->mField05, &item->mField07);
        *(ml::CVec3*)&item->mField0C = *(ml::CVec3*)((u8*)ret + 4);
        if (item->mField0C != lbl_eu_8066B290 || item->mField10 != lbl_eu_8066B290 ||
            item->mField14 != lbl_eu_8066B290) {
            ((CSchedAnimFlags*)&item->field_0x04)->mMode = 1;
        }
        if (*(u16*)(b + (u8)(item->mField05 + item->mField07) * 0x10) != 0x4000) {
            item->field_0x04 |= 0x80;
        } else {
            item->field_0x04 = (item->field_0x04 | 0x8) & ~0x80;
        }
    } else {
        *(ml::CVec3*)&item->mField0C = ml::CVec3::zero;
    }
}

// Refresh the item's scale vector at +0x0C: lerp it from entry element
// 0x10's +4 vector toward the (frame span & 0xFF)-th element (0x10-byte
// stride) at the current rate, then set bit 0x20 of the 0x04 flag byte when
// the vector is not (1,1,1).
void func_804E2EAC(CSchedAnimItem* item, CEntryElem* entries, u8* arg3, f32 f1) {
    bool flag;
    CEntryElem* ret = func_804E0248((CScheduleItem*)item, entries, 0x10, arg3, f1);
    if (ret != NULL) {
        f32 rate = func_804E04D4(item, entries, 0x10);
        if (rate < lbl_eu_8066B290) {
            return;
        }
        func_804DD89C(&item->mField0C, (u8*)ret + 4,
                      (u8*)ret + ((item->mField06 - item->mField05) & 0xFF) * 0x10 + 4,
                      rate);
        item->field_0x04 = (item->field_0x04 & ~0x60) |
                           ((item->mField0C != lbl_eu_8066B290 ||
                             item->mField10 != lbl_eu_8066B290 ||
                             item->mField14 != lbl_eu_8066B290)
                                << 5);
    }
}

void func_804E2F7C(){}

void func_804E30F0(){}
