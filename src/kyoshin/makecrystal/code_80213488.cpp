// Auto-scaffolded catalog TU for kyoshin/makecrystal/code_80213488
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include <math.h>

// ---------------------------------------------------------------------------
// Crystal-making state table. Same layout as CMCCrystalData (CMCCrystalBox.hpp),
// kept TU-local because that header also declares these targets with
// placeholder signatures that conflict with the real definitions below.
// ---------------------------------------------------------------------------
struct MakeCrystalEntry {
    s16 id;      // +0x00 crystal id
    u8  flag;    // +0x02
    u8  pad3;    // +0x03
};

struct MakeCrystalTable {
    MakeCrystalEntry entries[0x400]; // +0x0000 1024 entries x 4 bytes
    u16 count;                       // +0x1000 filled-entry count
    u8  byte_1002;                   // +0x1002 item category
    u8  limit;                       // +0x1003 row count
    u8  current;                     // +0x1004 current row cursor
};

// --- imports ---
// Item-table helpers (retail unmangled symbols): declared with the proper
// C-linkage signatures in the shared kyoshin menu header.
#include "kyoshin/CItemBoxGrid.hpp"   // func_80157C4C, func_80157C20, func_8015780C
// CItem_initItemImplInstances comes from CfGameManager.hpp (harness chain).

// Item-instance vtable shim (crystal-id getter lives at vtable offset 8). The
// object comes from CItem_initItemImplInstances(); only the getter is
// dispatched. The first declared virtual lands at memory slot 2 because MWCC
// reserves two RTTI slots (0x0/0x4) at the head of the vtable.
struct CrystalItemImpl {
    virtual u32 getCrystalItemId(void* slot) = 0; // vtable offset 8
    virtual void vf04() = 0;
    virtual void vf08() = 0;
};

// sdata2 constants used by func_80213570's ceil(count / scale) computation.
extern f32 lbl_eu_80668458; // 30.0f scale
// lbl_eu_8066845C is the 0.0f used for the fractional-part comparison.
extern f32 lbl_eu_8066845C;
// NOTE: the two 2^52 (4503599627370496.0) f64 constants the retail references
// (lbl_eu_80668460 for the u16->float and lbl_eu_80668468 for the int->float
// conversion) are synthesised internally by MWCC for the (f32)d->count and
// (f32)t casts; they cannot be named in source, so their reloc labels stay as
// decomp-pool entries (@...) and register as un-fixable reloc drift.

// sibling unit (CMCCrystalBox.cpp) helpers
extern "C" void func_80213988(MakeCrystalTable* d);
extern "C" void func_80213B1C(MakeCrystalTable* d);

// 4-byte {s16, u8} crystal-entry copy helper (defined below the callers so
// MWCC treats it out-of-line, matching retail's bl).
static __declspec(noinline) void copyCrystalEntry(MakeCrystalEntry* dst,
                                                  const MakeCrystalEntry* src);

// Returns the table pointer (retail mr r3,r28 on the way out).
MakeCrystalTable* func_80213488(MakeCrystalTable* d) {
    // Rolled do-while pointer walk (sth/stb per entry); bound kept inline in
    // the condition.
    MakeCrystalEntry* p = d->entries;
    s16 clearId = -1;
    u8 clearFlag = 0;
    do {
        p->id = clearId;
        p->flag = clearFlag;
        ++p;
    } while (p < d->entries + 0x400);
    d->count = 0;
    d->byte_1002 = 9;
    d->limit = 0;
    d->current = 0;
    for (u16 i = 0; i < 0x400; ++i) {
        MakeCrystalEntry tmp;
        tmp.id = -1;
        tmp.flag = 0;
        copyCrystalEntry(&d->entries[i], &tmp);
    }
    return d;
}

// Copy a 4-byte {s16, u8} crystal entry (id + flag). Retail calls it
// out-of-line (bl) from the second reset loop; noinline keeps MWCC from
// folding the body into the callers.
static __declspec(noinline) void copyCrystalEntry(MakeCrystalEntry* dst,
                                                  const MakeCrystalEntry* src) {
    // Retail copies only the two meaningful bytes (lha/sth id + lbz/stb flag);
    // a struct assignment would also copy pad3 and change the emitted code.
    dst->id = src->id;
    dst->flag = src->flag;
}

// Retail 0x80213530: trivial deleting destructor (no members to destruct),
// frees the object only when both the object and the flags-bit are non-zero.
void* __dt__80213530(void* self, int flags) {
    if (self != 0 && flags > 0)
        operator delete(self);
    return self;
}

// Retail 0x80213570: rebuild the crystal table. Resets every entry, walks the
// item category and keeps the rows whose item id matches `target`, computes
// the row count (limit) as ceil(filled-count / 30), then refreshes/sorts.
void func_80213570(MakeCrystalTable* d, u8 target) {
    d->count = 0;
    for (u16 i = 0; i < 0x400; ++i) {
        MakeCrystalEntry tmp;
        tmp.id = -1;
        tmp.flag = 0;
        copyCrystalEntry(&d->entries[i], &tmp);
    }
    int total = (int)func_80157C20(d->byte_1002);
    u16 i = 0;
    while ((u16)i < total) {
        void* obj = func_80157C4C(d->byte_1002, (s16)i);
        if (obj != 0 && *(void**)obj != 0) {
                CrystalItemImpl* inst =
                    (CrystalItemImpl*)CItem_initItemImplInstances(obj);
            u32 rid = inst->getCrystalItemId(obj);
            if (target == (u16)rid) {
                u16 n = d->count;
                d->count = n + 1;
                d->entries[n].id = (s16)i;
            }
        }
        i++;
    }
    // Row count = ceil(filled count / 30); each row holds up to 30 crystals.
    // Manual ceiling (retail inlines the rounding instead of calling libm).
    f32 v = (f32)d->count;
    v = v / lbl_eu_80668458;
    int t = (int)v;                      // floor since v >= 0 (fctiwz trunc)
    f32 frac = v - (f32)t;
    if (lbl_eu_8066845C != frac)
        t++;
    d->limit = (u8)t;
    d->current = 0;
    func_80213988(d);
    if ((func_8015780C(9) & 0xffffU) == 0) {
        func_80213B1C(d);
    }
}

// Retail 0x802136E0: set the flag byte of the entry at the adjusted index.
void func_802136E0(MakeCrystalTable* d, int idx, unsigned char val) {
    u16 adj = (u16)(idx + (s8)d->current * 30);
    if (adj >= d->count) return;
    d->entries[adj].flag = val;
}

// Retail 0x80213710: return the flag of the entry at the adjusted index
// (0 when out of range).
u8 func_80213710(MakeCrystalTable* d, u8 idx) {
    u16 adj = (u16)(idx + (s8)d->current * 30);
    if (adj < d->count)
        return d->entries[adj].flag;
    return 0;
}

// Retail 0x80213748: return 1 iff every entry's flag is non-zero.
// NOTE: retail exports an unmangled `func_80213748`; the mangled definition
// here is mapped back to the retail name by the symbol-recovery tooling
// (same mechanism as copyCrystalEntry -> func_8021351C).
int func_80213748_allFlagsSet(MakeCrystalTable* d) {
    // Retail caches count before the loop (read once into a register).
    u16 count = d->count;
    for (u16 i = 0; i < count; ++i) {
        if (d->entries[i].flag == 0) {
            return 0;
        }
    }
    return 1;
}

// Retail 0x80213788: advance the current row cursor, wrapping to 0 when it
// reaches the row limit.
void func_80213788(MakeCrystalTable* d) {
    u8 v = d->current + 1;
    d->current = v;
    if ((s8)v < d->limit) return;
    d->current = 0;
}

// Retail 0x802137B4: step the row cursor back one, wrapping to limit - 1 at 0.
void func_802137B4(MakeCrystalTable* d) {
    d->current = d->current - 1;
    if ((s8)d->current < 0)
        d->current = d->limit - 1;
}

// Retail 0x802137DC: resolve the item at the adjusted index, return its
// sub-category (bits 20..31) or 0 when out of range / unresolvable.
u32 func_802137DC(MakeCrystalTable* d, u8 idx) {
    u16 adj = (u16)(idx + (s8)d->current * 30);
    if (adj < d->count) {
        void* obj = func_80157C4C(d->byte_1002, d->entries[adj].id);
        if (obj != 0) {
            u32 v = *(u32*)obj;
            if (v != 0) return v >> 20;
        }
    }
    return 0;
}

// Retail 0x8021384C: resolve the item at the adjusted index and return the
// object pointer (0 when out of range or unresolvable).
void* func_8021384C(MakeCrystalTable* d, u8 idx) {
    u16 adj = (u16)(idx + (s8)d->current * 30);
    if (adj < d->count) {
        void* obj = func_80157C4C(d->byte_1002, d->entries[adj].id);
        if (obj != 0 && *(void**)obj != 0) return obj;
    }
    return 0;
}

// (definition above the callers so MWCC treats it out-of-line, matching retail)
