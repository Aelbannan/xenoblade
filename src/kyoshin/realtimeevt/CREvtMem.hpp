#pragma once

#include <types.h>

namespace cf {

// CREvtMem - realtime event memory manager (singleton)
// Vtable at 0x80530A30
// Size: 0x2C
// Singleton pointer at lbl_eu_80664260
struct CREvtMem {
    /* 0x00 */ u8* vtable;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u32 field_08;  // start1
    /* 0x0C */ u32 field_0C;  // size1
    /* 0x10 */ u8* ptr1;      // MEM2 allocation 1 / arena carve
    /* 0x14 */ u8* ptr2;      // MEM2 allocation 2 / arena carve
    /* 0x18 */ u32 arenaStart;
    /* 0x1C */ u32 arenaEnd;
    /* 0x20 */ u32 arenaSize;
    /* 0x24 */ u32 currentPos;
    /* 0x28 */ f32 someFloat;
    // (no ctor declaration: the ctor __ct__cf_CREvtMem is provided by
    // CREvtMem.cpp as an extern "C" retail-symbol function.)
    // Frees any pending MEM2 allocations, clears the singleton pointer and,
    // when the compiler-generated destruction flag requests it, deletes the
    // object (retail __dt__Q22cf8CREvtMemFv / __dl__FPv epilogue).
    ~CREvtMem();
};

} // namespace cf

// Vtable (retail .data:0x80530A30) and the realtime-event memory singleton
// (sbss; stored by the ctor, cleared by the dtor).
extern u8 lbl_eu_80530A30[];
extern cf::CREvtMem* lbl_eu_80664260;

// Float constant loaded by the ctor (retail sdata2 pool; UNDEF slot kept
// for rule-side pool naming -- see the sbss note in CREvtMem.cpp).
extern const f32 lbl_eu_80667650;

// Arena bounds helpers imported from the CInfoCf TU (bodies in
// kyoshin/cf/CInfoCf.cpp, reading singleton lbl_eu_80664250).
extern "C" void* InfoCfGetUnk0C(void);
extern "C" void* InfoCfGetUnk10(void);

// Initialise the arena and allocate the two realtime-event chunks.
extern "C" u32 initEvtMemArena(u32 size, u32 offset);
// Release both chunks if they were backed by MEM2 allocations.
extern "C" void freeEvtMemAll(void);
// Aligned allocation: carve from the arena while it fits, else heap/MEM1.
extern "C" u8* allocEvtMemAligned(u32 size, u32 alignment, int useMEM1);
// Free a pointer only if it came from outside the arena (heap-backed).
extern "C" void freeEvtMemPtr(u8* ptr);
// Indexed accessor over the pointer slot array starting at ptr1 (+0x10).
extern "C" u8* getEvtMemSlot(u32 idx);
