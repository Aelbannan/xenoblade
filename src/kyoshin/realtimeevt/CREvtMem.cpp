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
    f32 ps = lbl_eu_80667650;
    void* v = (void*)lbl_eu_80530A30;
    self->flags = 0;
    self->vtable = v;
    self->field_08 = 0;
    self->field_0C = 0;
    self->arenaStart = 0;
    self->arenaEnd = 0;
    self->arenaSize = 0;
    self->currentPos = 0;
    self->someFloat = ps;
    lbl_eu_80664260 = self;
    self->ptr1 = nullptr;
    self->ptr2 = nullptr;
}

// ============================================================================
// Destructor: __dt__Q22cf8CREvtMemFv
// Deallocates MEM2 allocations if flags are set, clears singleton
// ============================================================================
extern "C" cf::CREvtMem* __dt__Q22cf8CREvtMemFv(cf::CREvtMem* self, int dealloc_flag) {
    if (self != nullptr) {
        // Update vtable
        self->vtable = (void*)lbl_eu_80530A30;

        // Check flag bit 1 (0x2) - MEM2 allocation 1
        if (lbl_eu_80664260->flags & 0x2) {
            if (lbl_eu_80664260->ptr1 != nullptr) {
                deallocate__Q23mtl10MemManagerFPv(lbl_eu_80664260->ptr1);
                lbl_eu_80664260->ptr1 = nullptr;
            }
            lbl_eu_80664260->flags &= ~0x2;
        }

        // Check flag bit 2 (0x4) - MEM2 allocation 2
        if (lbl_eu_80664260->flags & 0x4) {
            if (lbl_eu_80664260->ptr2 != nullptr) {
                deallocate__Q23mtl10MemManagerFPv(lbl_eu_80664260->ptr2);
                lbl_eu_80664260->ptr2 = nullptr;
            }
            lbl_eu_80664260->flags &= ~0x4;
        }

        // Clear singleton, then deallocate if requested
        lbl_eu_80664260 = nullptr;
        if (dealloc_flag > 0) {
            __dl__FPv(self);
        }
    }

    return self;
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
    if (offset + chunk + 0x1000 < lbl_eu_80664260->arenaSize) {
        void* res;
        u32 pos = lbl_eu_80664260->currentPos;
        if (pos + chunk > lbl_eu_80664260->arenaEnd) {
            res = nullptr;
        } else {
            u32 low = pos & 0xFF;
            u32 pad = 0;
            if (low != 0) pad = 0x100 - low;
            res = (void*)(pos + pad);
            lbl_eu_80664260->currentPos += chunk + pad;
        }
        lbl_eu_80664260->ptr1 = res;
    } else {
        // Too big for the arena - allocate from MEM2 instead.
        void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
        lbl_eu_80664260->ptr1 = allocate_head__Q23mtl10MemManagerFUlUli(handle, chunk, 0x20);
        lbl_eu_80664260->flags |= 0x2;
    }

    // ---- Second allocation -> ptr2 ----
    if (offset + (chunk * 2) + 0x1000 < lbl_eu_80664260->arenaSize) {
        void* res;
        u32 pos = lbl_eu_80664260->currentPos;
        if (pos + chunk > lbl_eu_80664260->arenaEnd) {
            res = nullptr;
        } else {
            u32 low = pos & 0xFF;
            u32 pad = 0;
            if (low != 0) pad = 0x100 - low;
            res = (void*)(pos + pad);
            lbl_eu_80664260->currentPos += chunk + pad;
        }
        lbl_eu_80664260->ptr2 = res;
    } else {
        void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
        lbl_eu_80664260->ptr2 = allocate_head__Q23mtl10MemManagerFUlUli(handle, chunk, 0x20);
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
            deallocate__Q23mtl10MemManagerFPv(lbl_eu_80664260->ptr1);
            lbl_eu_80664260->ptr1 = nullptr;
        }
        lbl_eu_80664260->flags &= ~0x2;
    }

    // Check flag bit 2 (0x4)
    if (lbl_eu_80664260->flags & 0x4) {
        if (lbl_eu_80664260->ptr2 != nullptr) {
            deallocate__Q23mtl10MemManagerFPv(lbl_eu_80664260->ptr2);
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
    u32 cur = m->currentPos;
    u32 end = m->arenaEnd;

    if (cur + size > end) {
        // Doesn't fit in the arena - allocate fresh from the heap.
        if (useMEM1) {
            void* handle = getHandleMEM1__Q23mtl10MemManagerFv();
            return allocate_head__Q23mtl10MemManagerFUlUli(handle, size, 0x20);
        }
        return nullptr;
    }

    // Fits in the arena - round currentPos up to the alignment.
    u32 rem = cur % alignment;
    u32 pad = 0;
    if (rem != 0) pad = alignment - rem;
    void* result = (void*)(cur + pad);
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