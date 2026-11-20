// TU: kyoshin/realtimeevt/CREvtMem
// Class: cf::CREvtMem (namespace cf)
// Vtable at 0x80530A30
// Singleton pointer at cf::CREvtMem::sInstance
// Size: 0x2C

#include <types.h>
#include "kyoshin/realtimeevt/CREvtMem.hpp"
#include "monolib/util/MemManager.hpp"

// Zeroes the arena state and registers the singleton.
cf::CREvtMem::CREvtMem()
    : flags(0), field_08(0), field_0C(0), arenaStart(0), arenaEnd(0), arenaSize(0), currentPos(0),
      someFloat(0.0f) {
    cf::CREvtMem::sInstance = this;
    ptr1 = nullptr;
    ptr2 = nullptr;
}

// ============================================================================
// Destructor: __dt__Q22cf8CREvtMemFv
// Deallocates MEM2 allocations if flags are set, clears singleton; the
// compiler-generated epilogue performs operator delete when requested.
// ============================================================================
cf::CREvtMem::~CREvtMem() {
    // Check flag bit 1 (0x2) - MEM2 allocation 1
    if (cf::CREvtMem::sInstance->flags & 0x2) {
        if (cf::CREvtMem::sInstance->ptr1 != nullptr) {
            mtl::MemManager::deallocate(cf::CREvtMem::sInstance->ptr1);
            cf::CREvtMem::sInstance->ptr1 = nullptr;
        }
        cf::CREvtMem::sInstance->flags &= ~0x2;
    }

    // Check flag bit 2 (0x4) - MEM2 allocation 2
    if (cf::CREvtMem::sInstance->flags & 0x4) {
        if (cf::CREvtMem::sInstance->ptr2 != nullptr) {
            mtl::MemManager::deallocate(cf::CREvtMem::sInstance->ptr2);
            cf::CREvtMem::sInstance->ptr2 = nullptr;
        }
        cf::CREvtMem::sInstance->flags &= ~0x4;
    }

    // Clear singleton; the deleting epilogue then frees the object itself.
    cf::CREvtMem::sInstance = nullptr;
}

// ============================================================================
// initEvtMemArena: Initialize arena and allocate two chunks
// r3 = size, r4 = offset
// Returns the chunk size
// ============================================================================
u32 initEvtMemArena(u32 size, u32 offset) {
    // Establish the arena bounds from the realtime event region.
    cf::CREvtMem::sInstance->field_08 = (u32)InfoCfGetUnk0C();
    cf::CREvtMem::sInstance->field_0C = (u32)InfoCfGetUnk10();
    cf::CREvtMem::sInstance->arenaStart = cf::CREvtMem::sInstance->field_08;
    cf::CREvtMem::sInstance->arenaEnd = cf::CREvtMem::sInstance->field_08 + cf::CREvtMem::sInstance->field_0C;
    cf::CREvtMem::sInstance->arenaSize = cf::CREvtMem::sInstance->arenaEnd - cf::CREvtMem::sInstance->arenaStart;
    cf::CREvtMem::sInstance->currentPos = cf::CREvtMem::sInstance->arenaStart;

    u32 chunk = size;
    if (chunk == 0) chunk = 0x100000;

    if (offset + chunk + 0x1000 < cf::CREvtMem::sInstance->arenaSize) {
        cf::CREvtMem::sInstance->ptr1 = cf::CREvtMem::sInstance->allocArena(chunk);
    } else {
        // Too big for the arena - allocate from MEM2 instead.
        cf::CREvtMem::sInstance->ptr1 = (u8*)mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), chunk, 0x20);
        cf::CREvtMem::sInstance->flags |= 0x2;
    }

    if (offset + (chunk * 2) + 0x1000 < cf::CREvtMem::sInstance->arenaSize) {
        cf::CREvtMem::sInstance->ptr2 = cf::CREvtMem::sInstance->allocArena(chunk);
    } else {
        cf::CREvtMem::sInstance->ptr2 = (u8*)mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), chunk, 0x20);
        cf::CREvtMem::sInstance->flags |= 0x4;
    }

    return chunk;
}

// ============================================================================
// freeEvtMemAll: Deallocate both MEM2 allocations
// ============================================================================
void freeEvtMemAll(void) {
    // Check flag bit 1 (0x2)
    if (cf::CREvtMem::sInstance->flags & 0x2) {
        if (cf::CREvtMem::sInstance->ptr1 != nullptr) {
            mtl::MemManager::deallocate(cf::CREvtMem::sInstance->ptr1);
            cf::CREvtMem::sInstance->ptr1 = nullptr;
        }
        cf::CREvtMem::sInstance->flags &= ~0x2;
    }

    // Check flag bit 2 (0x4)
    if (cf::CREvtMem::sInstance->flags & 0x4) {
        if (cf::CREvtMem::sInstance->ptr2 != nullptr) {
            mtl::MemManager::deallocate(cf::CREvtMem::sInstance->ptr2);
            cf::CREvtMem::sInstance->ptr2 = nullptr;
        }
        cf::CREvtMem::sInstance->flags &= ~0x4;
    }
}

// ============================================================================
// allocEvtMemAligned: Aligned allocation from arena or heap
// r3 = size, r4 = alignment, r5 = useMEM1 flag
// Returns pointer to allocated memory, or 0 if fails
// ============================================================================
u8* allocEvtMemAligned(u32 size, u32 alignment, int useMEM1) {
    cf::CREvtMem* m = cf::CREvtMem::sInstance;
    u32 end = m->arenaEnd;

    if (m->currentPos + size > end) {
        // Doesn't fit in the arena - allocate fresh from the heap.
        if (useMEM1) {
            mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM1();
            return (u8*)mtl::MemManager::allocate_head(handle, size, 0x20);
        }
        return nullptr;
    }

    // Fits in the arena - round currentPos up to the alignment.
    u32 rem = m->currentPos % alignment;
    u32 pad = 0;
    if (rem != 0) pad = alignment - rem;
    u8* result = (u8*)(m->currentPos + pad);
    m->currentPos += size + pad;
    return result;
}

// ============================================================================
// freeEvtMemPtr: Bounds-checked deallocation
// If ptr is within the arena, do nothing. Otherwise, deallocate from heap.
// ============================================================================
void freeEvtMemPtr(u8* ptr) {
    cf::CREvtMem* mem = cf::CREvtMem::sInstance;

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
// getEvtMemSlot: Get pointer from indexed slot
// ============================================================================
u8* getEvtMemSlot(u32 idx) {
    // Reads slot [idx] from the pointer array that begins at ptr1 (+0x10).
    return ((u8**)&cf::CREvtMem::sInstance->ptr1)[idx];
}

// [.sbss] 0x80664260-0x80664268 (8B): event-memory singleton pointer + 4
// retail pad bytes (label spans 0x8; only the pointer is ever referenced).
cf::CREvtMem* cf::CREvtMem::sInstance = nullptr;
u8 gap_10_80664264_sbss[4];
