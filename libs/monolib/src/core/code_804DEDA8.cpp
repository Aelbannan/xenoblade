// Auto-scaffolded catalog TU for monolib/src/core/code_804DEDA8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <string.h>

#include "monolib/core/CScheduleItem.hpp"
#include "monolib/core/code_804DEDA8.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/MemManager.hpp"

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
extern f32 lbl_eu_8066B290;
extern f64 lbl_eu_8066B298;
extern f64 lbl_eu_8066B2A0;
extern f32 lbl_eu_8066B2A8;

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

void func_804DF164(){}

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

void func_804DF4BC(){}

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
extern "C" void func_804DFB88(s16 handle) {
    if (handle < 0 || lbl_eu_80661718.count <= handle) {
        return;
    }
    lbl_eu_80661718.lastHandle = handle;
    func_804E3E2C(&lbl_eu_80661718.base[handle]);
    if (lbl_eu_80661718.freeCount > 0) {
        lbl_eu_80661718.freeCount--;
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

void func_804DFCC4(){}

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

// Paired-single vector lerp kernel (monolib/src/effect/code_804DB938.cpp,
// retail symbol func_804DD89C): out = a + (b - a) * t for three f32s.
extern "C" void func_804DD89C(void* out, const void* a, const void* b, f32 t);

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

void func_804E0580(){}

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

void func_804E0788(){}

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

void func_804E0B94(){}

void func_804E0CF0(){}

void func_804E0E48(){}

void func_804E1044(){}

void func_804E1294(){}

void func_804E17A4(){}

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

// Initialize one schedule item's anim/entry state: set the base frame
// factor, reset the run-window bytes and slot handles, then (when an entry
// blob is present) load the run's slot handles and set the 0x04 flag bits
// according to whether the run extends to the blob terminator.
extern "C" void func_804E196C(CSchedAnimItem* item, u8* base) {
    f32 f = lbl_eu_8066B290;
    item->mField00 = f;
    item->field_0x04 = 0;
    item->mField05 = 0;
    item->mField07 = 1;
    item->mField06 = 1;
    ((CScheduleItem*)item)->mSlots[2] = -1;
    ((CScheduleItem*)item)->mSlots[3] = -1;
    if (base != NULL) {
        u8* ret = func_804E0188(base, 8, 0, &item->mField05, &item->mField07);
        CEntryElem* elem = (CEntryElem*)ret;
        if (elem->mField00 != 0x4000) {
            ((CScheduleItem*)item)->mSlots[2] = elem->mField04;
            ((CScheduleItem*)item)->mSlots[3] = elem->mField06;
        }
        u8 idx = item->mField05 + item->mField07;
        u16 w = *(u16*)(base + idx * 8);
        if (w == 0x4000) {
            item->field_0x04 = (item->field_0x04 | 0x8) & ~0x80;
        } else {
            item->field_0x04 |= 0x80;
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

void func_804E1C1C(){}

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

void func_804E2D8C(){}

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
