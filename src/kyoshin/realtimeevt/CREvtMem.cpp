// TU: kyoshin/realtimeevt/CREvtMem
// Class: cf::CREvtMem (namespace cf)
// Vtable at 0x80530A30
// Singleton pointer at lbl_eu_80664260
// Size: 0x2C

#include <types.h>
#include "kyoshin/realtimeevt/CREvtMem.hpp"

extern "C" {
// MemManager functions
void* getHandleMEM1__Q23mtl10MemManagerFv(void);
void* getHandleMEM2__Q23mtl10MemManagerFv(void);
void* allocate_head__Q23mtl10MemManagerFUlUli(void* handle, u32 size, int align);
void  deallocate__Q23mtl10MemManagerFPv(void* ptr);
void  __dl__FPv(void* ptr);

// Helper functions
u32 func_8016676C(void);
u32 func_80166778(void);

// Float constant
extern f32 lbl_eu_80667650;
}

// ============================================================================
// Constructor: __ct__cf_CREvtMem
// Initializes all fields to 0, stores a float, sets singleton
// ============================================================================
extern "C" void __ct__cf_CREvtMem(cf::CREvtMem* self) {
    self->vtable = (void*)lbl_eu_80530A30;
    self->flags = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->ptr1 = nullptr;
    self->ptr2 = nullptr;
    self->arenaStart = 0;
    self->arenaEnd = 0;
    self->arenaSize = 0;
    self->currentPos = 0;
    self->someFloat = lbl_eu_80667650;
    lbl_eu_80664260 = self;
}

// ============================================================================
// Destructor: __dt__Q22cf8CREvtMemFv
// Deallocates MEM2 allocations if flags are set, clears singleton
// ============================================================================
extern "C" void __dt__Q22cf8CREvtMemFv(cf::CREvtMem* self, int dealloc_flag) {
    if (self == nullptr) return;

    // Update vtable
    self->vtable = (void*)lbl_eu_80530A30;

    cf::CREvtMem* singleton = lbl_eu_80664260;

    // Check flag bit 1 (0x2) - MEM2 allocation 1
    if (singleton->flags & 0x2) {
        if (singleton->ptr1 != nullptr) {
            deallocate__Q23mtl10MemManagerFPv(singleton->ptr1);
            singleton->ptr1 = nullptr;
        }
        singleton->flags &= ~0x2;
    }

    // Check flag bit 2 (0x4) - MEM2 allocation 2
    if (singleton->flags & 0x4) {
        if (singleton->ptr2 != nullptr) {
            deallocate__Q23mtl10MemManagerFPv(singleton->ptr2);
            singleton->ptr2 = nullptr;
        }
        singleton->flags &= ~0x4;
    }

    // Clear singleton and deallocate if needed
    if (dealloc_flag > 0) {
        lbl_eu_80664260 = nullptr;
        __dl__FPv(self);
    }
}

// ============================================================================
// func_80167D40: Initialize arena and allocate two chunks
// r3 = size, r4 = offset
// Returns the chunk size
// ============================================================================
extern "C" u32 func_80167D40(u32 size, u32 offset) {
    cf::CREvtMem* mem = lbl_eu_80664260;

    // Get arena boundaries
    mem->field_08 = func_8016676C();
    mem->field_0C = func_80166778();

    // Set up arena
    mem->arenaStart = mem->field_08;
    mem->arenaEnd = mem->field_08 + mem->field_0C;
    mem->arenaSize = mem->arenaEnd - mem->arenaStart;
    mem->currentPos = mem->arenaStart;

    // Default chunk size if 0
    u32 chunkSize = size;
    if (chunkSize == 0) {
        chunkSize = 0x100000;  // 1MB
    }

    // ---- First allocation ----
    u32 total = offset + chunkSize + 0x1000;
    if (total <= mem->arenaSize) {
        // Fits in arena - align to 0x100 boundary
        u32 pos = mem->currentPos;
        u32 padding = (pos & 0xFF) ? (0x100 - (pos & 0xFF)) : 0;
        mem->ptr1 = (void*)(pos + padding);
        mem->currentPos += chunkSize + padding;
    } else {
        // Allocate from MEM2
        void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
        mem->ptr1 = allocate_head__Q23mtl10MemManagerFUlUli(handle, chunkSize, 0x20);
        mem->flags |= 0x2;
    }

    // ---- Second allocation ----
    u32 total2 = offset + (chunkSize * 2) + 0x1000;
    if (total2 <= mem->arenaSize) {
        // Fits in arena
        u32 pos = mem->currentPos;
        u32 padding = (pos & 0xFF) ? (0x100 - (pos & 0xFF)) : 0;
        mem->ptr2 = (void*)(pos + padding);
        mem->currentPos += chunkSize + padding;
    } else {
        // Allocate from MEM2
        void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
        mem->ptr2 = allocate_head__Q23mtl10MemManagerFUlUli(handle, chunkSize, 0x20);
        mem->flags |= 0x4;
    }

    return chunkSize;
}

// ============================================================================
// func_80167EF8: Deallocate both MEM2 allocations
// ============================================================================
extern "C" void func_80167EF8(void) {
    cf::CREvtMem* mem = lbl_eu_80664260;

    // Check flag bit 1 (0x2)
    if (mem->flags & 0x2) {
        if (mem->ptr1 != nullptr) {
            deallocate__Q23mtl10MemManagerFPv(mem->ptr1);
            mem->ptr1 = nullptr;
        }
        mem->flags &= ~0x2;
    }

    // Check flag bit 2 (0x4)
    if (mem->flags & 0x4) {
        if (mem->ptr2 != nullptr) {
            deallocate__Q23mtl10MemManagerFPv(mem->ptr2);
            mem->ptr2 = nullptr;
        }
        mem->flags &= ~0x4;
    }
}

// ============================================================================
// func_80167F6C: Aligned allocation from arena or heap
// r3 = size, r4 = alignment, r5 = useMEM1 flag
// Returns pointer to allocated memory, or 0 if fails
// ============================================================================
extern "C" void* func_80167F6C(u32 size, u32 alignment, int useMEM1) {
    cf::CREvtMem* mem = lbl_eu_80664260;
    u32 currentPos = mem->currentPos;
    u32 newPos = currentPos + size;

    if (newPos <= mem->arenaEnd) {
        // Fits in arena - apply alignment padding
        u32 remainder = currentPos % alignment;
        u32 padding = (remainder != 0) ? (alignment - remainder) : 0;
        void* result = (void*)(currentPos + padding);
        mem->currentPos += size + padding;
        return result;
    }

    // Doesn't fit in arena - allocate from heap
    if (useMEM1) {
        void* handle = getHandleMEM1__Q23mtl10MemManagerFv();
        return allocate_head__Q23mtl10MemManagerFUlUli(handle, size, 0x20);
    }

    return nullptr;
}

// ============================================================================
// func_80167FFC: Bounds-checked deallocation
// If ptr is within the arena, do nothing. Otherwise, deallocate from heap.
// ============================================================================
extern "C" void func_80167FFC(void* ptr) {
    cf::CREvtMem* mem = lbl_eu_80664260;

    // Check if ptr is within the arena
    if (ptr >= (void*)mem->arenaStart && ptr < (void*)mem->arenaEnd) {
        return;  // Inside arena - do nothing
    }

    // Outside arena - deallocate
    if (ptr != nullptr) {
        deallocate__Q23mtl10MemManagerFPv(ptr);
    }
}

// ============================================================================
// func_80168028: Get pointer from indexed slot
// ============================================================================
extern "C" void* func_80168028(u32 idx) {
    cf::CREvtMem* mem = lbl_eu_80664260;
    u32* arr = (u32*)&mem->ptr1;  // ptr1 is at +0x10, ptr2 at +0x14
    return (void*)arr[idx + 4];
}