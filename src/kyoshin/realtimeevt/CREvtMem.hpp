#pragma once

#include <types.h>

namespace cf {

// CREvtMem - realtime event memory manager (singleton)
// Size: 0x2C
// Singleton pointer is CREvtMem::sInstance.
class CREvtMem {
public:
    CREvtMem();
    virtual ~CREvtMem();

    // name-guess: sole singleton pointer at 0x80664260, written by the ctor.
    static CREvtMem* sInstance;

    // Carve a 256-byte aligned block from the realtime-event arena.
    u8* allocArena(u32 size) {
        u32 pos = currentPos;
        if (pos + size > arenaEnd) {
            return nullptr;
        }
        u32 low = pos & 0xFF;
        u32 pad = 0;
        if (low != 0) pad = 0x100 - low;
        currentPos += size + pad;
        return (u8*)(pos + pad);
    }

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
};

} // namespace cf

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
