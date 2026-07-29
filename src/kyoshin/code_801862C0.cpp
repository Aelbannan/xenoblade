// Auto-scaffolded catalog TU for kyoshin/code_801862C0
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/code_801862C0.hpp"

// SDA singleton pointer for the arts select object (opaque pointer)
extern "C" u32 lbl_eu_806642D0;

// memset prototype
extern "C" void* memset(void*, int, unsigned long);

// CArtsSelectSlot: per-slot entry in the arts selection array (stride 0x170)
struct CArtsSelectSlot {
    u32 unk00;  // +0x00: opaque pointer value
    u32 unk04;  // +0x04: source pointer value (cleared on match)
};

// CArtsSelectContainer: holds 46 slots of arts data (size includes 0x1700/0x1704 fields)
struct CArtsSelectContainer {
    u8 _pad00[0x100];            // +0x00..0xFF: cleared in ctor
    u8 _pad100[0x70];           // +0x100..0x16F: cleared in ctor (0x80 bytes total here)
    u32 unk1700;                // +0x1700: arts id from src->unk74
    u32 unk1704;                // +0x1704: src pointer value
    CArtsSelectSlot slots[46];   // 46 slots at appropriate offsets
};

void* func_801862C0(VMThread* pThread){ return 0; }

void* func_801862E0(void* p){ return 0; }

void* func_801863F4(void* p){ return 0; }

void* func_80186460(void* dst, void* src){
    // Copy src->unk74 to dst->unk1700 and src pointer to dst->unk1704
    CArtsSelectContainer* container = static_cast<CArtsSelectContainer*>(dst);
    const u8* srcBytes = static_cast<const u8*>(src);
    u32 id = *reinterpret_cast<const u32*>(srcBytes + 0x74);
    container->unk1700 = id;
    container->unk1704 = reinterpret_cast<u32>(src);
    return reinterpret_cast<void*>(id);
}

void* func_80186474(void* self, void* src){
    // Search through slots at (self + idx*0x170) for src->unk74 match
    // On match: clear slot[i].unk00 and slot[i].unk04, return srcId
    // On no match: return srcId
    u32 srcId = *reinterpret_cast<const u32*>(static_cast<const u8*>(src) + 0x74);
    
    // Match retail idx calculation: rlwinm r4, r6, 28, 0, 3; rlwinm r5, r6, 1, 31, 31
    //                          subf r4, r5, r4; rlwinm r4, 4; add r4, r4, r5
    u32 idx = __rlwinm(srcId, 28, 0, 3);
    u32 sign = __rlwinm(srcId, 1, 31, 31);
    idx = __rlwinm(idx - sign, 4, 0, 31);
    idx += sign;
    
    u32 slotPtr = reinterpret_cast<u32>(self) + idx * 0x170;
    
    for (int i = 0; i < 46; i++) {
        u32 val = *reinterpret_cast<u32*>(slotPtr);
        if (val == srcId) {
            *reinterpret_cast<u32*>(slotPtr) = 0;
            *reinterpret_cast<u32*>(slotPtr + 4) = 0;
            return reinterpret_cast<void*>(srcId);
        }
        slotPtr += 8;
    }
    return reinterpret_cast<void*>(srcId);
}

// Constructor: clears structure and stores this pointer to singleton
extern "C" void* __ct__80186578(CArtsSelectContainer* self){
    lbl_eu_806642D0 = reinterpret_cast<u32>(self);
    memset(self, 0, 0x100);
    memset(reinterpret_cast<u8*>(self) + 0x100, 0, 0x80);
    return self;
}

void* func_801864DC(void* pObj, int slot){ return 0; }

void __dt__801865C4(){}

void* func_80186664(void* p){ return 0; }

void* func_801866F0(void* p){ return 0; }

void* func_80186A70(void* p){ return 0; }

void* func_80186BC8(void* p){ return 0; }

void* func_80186C7C(void* p){ return 0; }

void* func_80186D20(void* p){ return 0; }
