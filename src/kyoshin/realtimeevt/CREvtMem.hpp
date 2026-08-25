#pragma once

#include <types.h>

namespace cf {

// CREvtMem - realtime event memory manager (singleton)
// Vtable at 0x80530A30
// Size: 0x2C
// Singleton pointer at lbl_eu_80664260
struct CREvtMem {
    /* 0x00 */ void* vtable;
    /* 0x04 */ u32 flags;
    /* 0x08 */ u32 field_08;  // start1
    /* 0x0C */ u32 field_0C;  // size1
    /* 0x10 */ void* ptr1;    // MEM2 allocation 1
    /* 0x14 */ void* ptr2;    // MEM2 allocation 2
    /* 0x18 */ u32 arenaStart;
    /* 0x1C */ u32 arenaEnd;
    /* 0x20 */ u32 arenaSize;
    /* 0x24 */ u32 currentPos;
    /* 0x28 */ f32 someFloat;

    CREvtMem();
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

// Float constant loaded by the ctor (retail sdata2 pool).
extern const f32 lbl_eu_80667650;

// Arena bounds helpers imported from the CInfoCf TU (bodies in
// kyoshin/cf/CInfoCf.cpp, reading singleton lbl_eu_80664250).
u32 func_8016676C(void);
u32 func_80166778(void);

// Initialise the arena and allocate the two realtime-event chunks.
u32 func_80167D40(u32 size, u32 offset);
// Release both chunks if they were backed by MEM2 allocations.
void func_80167EF8(void);
// Aligned allocation: carve from the arena while it fits, else heap/MEM1.
u8* func_80167F6C(u32 size, u32 alignment, int useMEM1);
// Free a pointer only if it came from outside the arena (heap-backed).
void func_80167FFC(u8* ptr);
// Indexed accessor over the pointer slot array starting at ptr1 (+0x10).
u8* func_80168028(u32 idx);
