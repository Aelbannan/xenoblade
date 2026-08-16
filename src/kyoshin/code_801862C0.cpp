// Auto-scaffolded catalog TU for kyoshin/code_801862C0
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/code_801862C0.hpp"

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

// Returned singleton object backing func_801862C0's accessor.
// Sized to land in normal .bss (not SDA) so its address materializes as lis+addi.
u8 lbl_eu_80574090[0x20];

/* One-time init guard: set the SDA flag on first call, then hand out the
   singleton object's address. Sign-extension in retail matches a signed byte. */
void* func_801862C0(void) {
    if (lbl_eu_806642C8 == 0) {
        lbl_eu_806642C8 = 1;
    }
    return &lbl_eu_80574090;
}

void* func_801862E0(void* p){ return 0; }

void* func_801863F4(CArtsSelectContainer* self, void* src) {
    // Find a free slot (val == 0) in the hash bucket and fill it.
    u32 srcId = *reinterpret_cast<const u32*>((const u8*)src + 0x74);

    // Same bucket index computation as func_80186474.
    u32 idx = __rlwinm(srcId, 28, 0, 3);
    u32 sign = __rlwinm(srcId, 1, 31, 31);
    idx = __rlwinm(idx - sign, 4, 0, 31);
    idx += sign;

    // Bucket search: find a free slot (val == 0) and fill it. Retail keeps
    // the bucket base in a callee-saved register and tracks i separately for
    // the indexed fill-write (stwx base+i*8), then recomputes base + i*8 for
    // the +4 store. MWCC CSEs the base recompute from high-level C, leaving
    // the function one instruction short (0x68 vs 0x6c) -- open item.
    CArtsSelectSlot* slot =
        reinterpret_cast<CArtsSelectSlot*>((u8*)self + idx * 0x170);
    for (int i = 0; i < 46; i++) {
        if (slot[i].unk00 == 0) {
            slot[i].unk00 = srcId;
            slot[i].unk04 = reinterpret_cast<u32>(src);
            return reinterpret_cast<void*>(srcId);
        }
    }
    return reinterpret_cast<void*>(srcId);
}

extern "C" void* func_80186460(void* dst, void* src) {
    CArtsSelectContainer* container = static_cast<CArtsSelectContainer*>(dst);
    const u8* srcBytes = static_cast<const u8*>(src);
    u32 id = *reinterpret_cast<const u32*>(srcBytes + 0x74);
    *(u32*)((u8*)dst + 0x1700) = id;
    *(u32*)((u8*)dst + 0x1704) = reinterpret_cast<u32>(src);
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

// func_801864DC: widget id lookup. Fast path: field_1700 == id returns
// field_1704. Otherwise a hash-indexed table search: index =
// ((((id>>4)&0xF) - sign) << 4) + sign (sign = bit 31), stride 0x170, 46
// entries of 8 bytes; miss falls back to func_800B708C(id).
extern "C" void* func_800B708C(int id);
void* func_801864DC(void* pObj, int slot) {
    u8* self = (u8*)pObj;
    u32 id = (u32)slot;
    if (*(u32*)(self + 0x1700) == id) {
        return *(void**)(self + 0x1704);
    }
    u32 sign = (id >> 31) & 1;
    // Retail hash: t = rotl((id << 28) - sign, 4) + sign.
    u32 t = (id << 28) - sign;
    t = (t << 4) | (t >> 28);
    t += sign;
    u32 off = t * 0x170;
    u8* base = self + off;
    int i;
    for (i = 0; i < 46; i++) {
        if (id == *(u32*)(base + i * 8)) {
            return *(void**)(((u8*)self + off) + i * 8 + 4);
        }
    }
    void* r = func_800B708C(slot);
    if (r) {
        return r;
    }
    return 0;
}

void __dt__801865C4(){}

extern "C" int func_800B8920(void*);
extern "C" void func_800B9404(void*);
// func_80186664 (recovered): teardown loop over 64 widget slots.  For each
// slot: if the slot pointer is armed, run the check helper (func_800B8920)
// and, when it passes, the free helper (func_800B9404); the slot pointer is
// cleared on both paths (the retail keeps two ptr=0 stores on the success
// path — an unreduced duplicate of the shared tail store) and the slot's
// halfword flag at +0x100 is cleared unconditionally.
extern "C" void func_80186664(u8* self) {
    u32 zero = 0;
    void** p = (void**)self;
    u16* h = (u16*)self;
    for (s32 i = 0; i < 64; i++) {
        void* ptr = *p;
        if (ptr != 0) {
            if (func_800B8920(ptr) != 0) {
                // Retail re-reads the slot pointer for the second call (no
                // local kept across calls; the calls may alias the array).
                func_800B9404(*p);
                *p = (void*)0;
            }
            *p = (void*)0;
        }
        *(h + 0x80) = (u16)0;
        h++;
        p++;
    }
}

void* func_801866F0(void* p){ return 0; }

void* func_80186A70(void* p){ return 0; }

void* func_80186BC8(void* p){ return 0; }

void* func_80186C7C(void* p){ return 0; }

void* func_80186D20(void* p){ return 0; }
