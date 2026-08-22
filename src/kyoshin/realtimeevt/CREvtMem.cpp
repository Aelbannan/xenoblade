// TU: kyoshin/realtimeevt/CREvtMem
// Class: cf::CREvtMem (namespace cf)
// Vtable at 0x80530A30
// Singleton pointer at lbl_eu_80664260
// Size: 0x2C

#include <types.h>
#include "kyoshin/realtimeevt/CREvtMem.hpp"
#include "monolib/util/MemManager.hpp"

// ============================================================================
// Constructor: __ct__cf_CREvtMem
// Initializes all fields to 0, stores a float, sets singleton
// ============================================================================
cf::CREvtMem::CREvtMem() {
    u8* vt = &lbl_eu_80530A30[0];
    const f32 ps = lbl_eu_80667650;
    flags = 0;
    vtable = vt;
    field_08 = 0;
    field_0C = 0;
    arenaStart = 0;
    arenaEnd = 0;
    arenaSize = 0;
    currentPos = 0;
    someFloat = ps;
    lbl_eu_80664260 = this;
    ptr1 = nullptr;
    ptr2 = nullptr;
}

// ============================================================================
// Destructor: __dt__Q22cf8CREvtMemFv
// Deallocates MEM2 allocations if flags are set, clears singleton; the
// compiler-generated epilogue performs operator delete when requested.
// ============================================================================
cf::CREvtMem::~CREvtMem() {
    // Update vtable
    vtable = &lbl_eu_80530A30[0];

    // Check flag bit 1 (0x2) - MEM2 allocation 1
    if (lbl_eu_80664260->flags & 0x2) {
        if (lbl_eu_80664260->ptr1 != nullptr) {
            mtl::MemManager::deallocate(lbl_eu_80664260->ptr1);
            lbl_eu_80664260->ptr1 = nullptr;
        }
        lbl_eu_80664260->flags &= ~0x2;
    }

    // Check flag bit 2 (0x4) - MEM2 allocation 2
    if (lbl_eu_80664260->flags & 0x4) {
        if (lbl_eu_80664260->ptr2 != nullptr) {
            mtl::MemManager::deallocate(lbl_eu_80664260->ptr2);
            lbl_eu_80664260->ptr2 = nullptr;
        }
        lbl_eu_80664260->flags &= ~0x4;
    }

    // Clear singleton; the deleting epilogue then frees the object itself.
    lbl_eu_80664260 = nullptr;
}

// ============================================================================
// func_80167D40: Initialize arena and allocate two chunks
// r3 = size, r4 = offset
// Returns the chunk size
// ============================================================================
extern "C" u32 func_80167D40(u32 size, u32 offset) {
    // Establish the arena bounds from the realtime event region.
    lbl_eu_80664260->field_08 = func_8016676C();
    lbl_eu_80664260->field_0C = func_80166778();
    lbl_eu_80664260->arenaStart = lbl_eu_80664260->field_08;
    lbl_eu_80664260->arenaEnd = lbl_eu_80664260->field_08 + lbl_eu_80664260->field_0C;
    lbl_eu_80664260->arenaSize = lbl_eu_80664260->arenaEnd - lbl_eu_80664260->arenaStart;
    lbl_eu_80664260->currentPos = lbl_eu_80664260->arenaStart;

    u32 chunk = size;
    if (chunk == 0) chunk = 0x100000;

    // ---- First allocation -> ptr1 ----
    // Cache the singleton in a local; its last use is the currentPos update,
    // so the ptr1 store below reloads the global.
    cf::CREvtMem* m1 = lbl_eu_80664260;
    if (offset + chunk + 0x1000 < m1->arenaSize) {
        void* res;
        u32 pos = m1->currentPos;
        if (pos + chunk > m1->arenaEnd) {
            res = nullptr;
        } else {
            u32 low = pos & 0xFF;
            u32 pad = 0;
            if (low != 0) pad = 0x100 - low;
            res = (void*)(pos + pad);
            m1->currentPos += chunk + pad;
        }
        lbl_eu_80664260->ptr1 = res;
    } else {
        // Too big for the arena - allocate from MEM2 instead.
        mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
        lbl_eu_80664260->ptr1 = mtl::MemManager::allocate_head(handle, chunk, 0x20);
        lbl_eu_80664260->flags |= 0x2;
    }

    // ---- Second allocation -> ptr2 ----
    cf::CREvtMem* m2 = lbl_eu_80664260;
    if (offset + (chunk * 2) + 0x1000 < m2->arenaSize) {
        void* res;
        u32 pos = m2->currentPos;
        if (pos + chunk > m2->arenaEnd) {
            res = nullptr;
        } else {
            u32 low = pos & 0xFF;
            u32 pad = 0;
            if (low != 0) pad = 0x100 - low;
            res = (void*)(pos + pad);
            m2->currentPos += chunk + pad;
        }
        lbl_eu_80664260->ptr2 = res;
    } else {
        mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
        lbl_eu_80664260->ptr2 = mtl::MemManager::allocate_head(handle, chunk, 0x20);
        lbl_eu_80664260->flags |= 0x4;
    }

    return chunk;
}

// ============================================================================
// func_80167EF8: Deallocate both MEM2 allocations
// ============================================================================
extern "C" void func_80167EF8(void) {
    // Check flag bit 1 (0x2)
    if (lbl_eu_80664260->flags & 0x2) {
        if (lbl_eu_80664260->ptr1 != nullptr) {
            mtl::MemManager::deallocate(lbl_eu_80664260->ptr1);
            lbl_eu_80664260->ptr1 = nullptr;
        }
        lbl_eu_80664260->flags &= ~0x2;
    }

    // Check flag bit 2 (0x4)
    if (lbl_eu_80664260->flags & 0x4) {
        if (lbl_eu_80664260->ptr2 != nullptr) {
            mtl::MemManager::deallocate(lbl_eu_80664260->ptr2);
            lbl_eu_80664260->ptr2 = nullptr;
        }
        lbl_eu_80664260->flags &= ~0x4;
    }
}

// ============================================================================
// func_80167F6C: Aligned allocation from arena or heap
// r3 = size, r4 = alignment, r5 = useMEM1 flag
// Returns pointer to allocated memory, or 0 if fails
// ============================================================================
extern "C" void* func_80167F6C(u32 size, u32 alignment, int useMEM1) {
    cf::CREvtMem* m = lbl_eu_80664260;
    u32 end = m->arenaEnd;

    if (m->currentPos + size > end) {
        // Doesn't fit in the arena - allocate fresh from the heap.
        if (useMEM1) {
            mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM1();
            return mtl::MemManager::allocate_head(handle, size, 0x20);
        }
        return nullptr;
    }

    // Fits in the arena - round currentPos up to the alignment.
    u32 rem = m->currentPos % alignment;
    u32 pad = 0;
    if (rem != 0) pad = alignment - rem;
    void* result = (void*)(m->currentPos + pad);
    m->currentPos += size + pad;
    return result;
}

// ============================================================================
// func_80167FFC: Bounds-checked deallocation
// If ptr is within the arena, do nothing. Otherwise, deallocate from heap.
// ============================================================================
extern "C" void func_80167FFC(void* ptr) {
    cf::CREvtMem* mem = lbl_eu_80664260;

    // If ptr lies inside the arena, it was arena-allocated - do nothing.
    if (mem->arenaStart <= (u32)ptr && mem->arenaEnd > (u32)ptr) {
        return;
    }

    // Otherwise the pointer came from the heap - free it.
    if (ptr != nullptr) {
        mtl::MemManager::deallocate(ptr);
    }
}

// ============================================================================
// func_80168028: Get pointer from indexed slot
// ============================================================================
extern "C" void* func_80168028(u32 idx) {
    // Reads slot [idx] from the pointer array that begins at ptr1 (+0x10).
    return ((void**)&lbl_eu_80664260->ptr1)[idx];
}
