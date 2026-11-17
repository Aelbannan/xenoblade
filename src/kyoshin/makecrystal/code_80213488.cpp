// Auto-scaffolded catalog TU for kyoshin/makecrystal/code_80213488
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include <math.h>

#include "kyoshin/makecrystal/CMCCrystalData.hpp"

// --- imports ---
// Item-table helpers (retail unmangled symbols): declared with the proper
// C-linkage signatures in the shared kyoshin menu header.
#include "kyoshin/CItemBoxGrid.hpp"   // CItem_kindPtr, CItemBlock_countKindSlots, CItemBlock_getFlag120EC
// CItem_initItemImplInstances comes from CfGameManager.hpp (harness chain).

// Item-instance vtable shim (crystal-id getter lives at vtable offset 8). The
// object comes from CItem_initItemImplInstances(obj); only the getter is
// dispatched. The first declared virtual lands at memory slot 2 because MWCC
// reserves two RTTI slots (0x0/0x4) at the head of the vtable.
struct CrystalItemImpl {
    virtual u32 getCrystalItemId(CMCCrystalItemObj* slot) = 0; // vtable offset 8
    virtual void vf04() = 0;
    virtual void vf08() = 0;
};

// sdata2 constants used by MakeCrystal_CollectByTarget's ceil(count / scale) computation.
// NOTE: defining these two in source makes MWCC const-propagate the literals
// and emit extra .sdata2 pool words (measured: 0x20 vs retail 0x18).  They must
// come from the code path that uses them once that is reconstructed.
#pragma section data_type ".sdata2" ".sdata2"
extern __declspec(section ".sdata2") f32 lbl_eu_80668458 = 30.0f;  // 30.0f scale
// lbl_eu_8066845C is the 0.0f used for the fractional-part comparison.
extern __declspec(section ".sdata2") f32 lbl_eu_8066845C = 0.0f;
// NOTE: the two 2^52 (4503599627370496.0) f64 constants the retail references
// (lbl_eu_80668460 for the u16->float and lbl_eu_80668468 for the int->float
// conversion) are synthesised internally by MWCC for the (f32)d->count and
// (f32)t casts; they cannot be named in source, so their reloc labels stay as
// decomp-pool entries (@...) and register as un-fixable reloc drift.

CMCCrystalData::CMCCrystalData() {
    count = 0;
    category = 9;
    limit = 0;
    current = 0;
    for (u16 i = 0; i < 0x400; ++i) {
        entries[i] = CMCCrystalDataEntry();
    }
}

// Retail calls this out-of-line from the reset loops; it copies the two
// named fields only.
CMCCrystalDataEntry& CMCCrystalDataEntry::operator=(const CMCCrystalDataEntry& other) {
    id = other.id;
    flag = other.flag;
    return *this;
}

// Out of line here: retail emits it in this unit right after the ctor. (The
// retail CMCCrystalBox dtor inlines it, which an out-of-line definition
// cannot reproduce; see docs/MWCC_CASES.md.)
CMCCrystalData::~CMCCrystalData() {}




// Retail 0x80213570: rebuild the crystal table. Resets every entry, walks the
// item category and keeps the rows whose item id matches `target`, computes
// the page count as ceil(filled-count / 30), then sorts.
void CMCCrystalData::collectByTarget(u8 target) {
    count = 0;
    for (u16 i = 0; i < 0x400; ++i) {
        entries[i] = CMCCrystalDataEntry();
    }
    int total = (int)CItemBlock_countKindSlots(category);
    u16 i = 0;
    while ((u16)i < total) {
        CMCCrystalItemObj* obj =
            (CMCCrystalItemObj*)CItem_kindPtr(category, (s16)i);
        if (obj != 0 && obj->word0 != 0) {
                CrystalItemImpl* inst =
                    (CrystalItemImpl*)CItem_initItemImplInstances(obj);
            u32 rid = inst->getCrystalItemId(obj);
            if (target == (u16)rid) {
                u16 n = count;
                count = n + 1;
                entries[n].id = (s16)i;
            }
        }
        i++;
    }
    // Page count = ceil(filled count / 30), rounded by hand.
    f32 v = (f32)count;
    v = v / lbl_eu_80668458;
    int t = (int)v;
    f32 frac = v - (f32)t;
    if (lbl_eu_8066845C != frac)
        t++;
    limit = (u8)t;
    current = 0;
    sortIdOrder();
    if ((CItemBlock_getFlag120EC(9) & 0xffffU) == 0) {
        sortKeyOrder();
    }
}

void CMCCrystalData::setFlag(int idx, u8 val) {
    u16 adj = (u16)(idx + current * 30);
    if (adj >= count) return;
    entries[adj].flag = val;
}

int CMCCrystalData::getFlag(u8 idx) {
    u16 adj = (u16)(idx + current * 30);
    if (adj < count)
        return entries[adj].flag;
    return 0;
}

int CMCCrystalData::allFlagsSet() {
    u16 n = count;
    for (u16 i = 0; i < n; ++i) {
        if (entries[i].flag == 0) {
            return 0;
        }
    }
    return 1;
}

void CMCCrystalData::nextPage() {
    u8 v = current + 1;
    current = v;
    if ((s8)v < limit) return;
    current = 0;
}

void CMCCrystalData::prevPage() {
    current = current - 1;
    if (current < 0)
        current = limit - 1;
}

u16 CMCCrystalData::getSubCategory(u8 idx) {
    u16 adj = (u16)(idx + current * 30);
    if (adj < count) {
        CMCCrystalItemObj* obj =
            (CMCCrystalItemObj*)CItem_kindPtr(category, entries[adj].id);
        if (obj != 0) {
            u32 v = obj->word0;
            if (v != 0) return v >> 20;
        }
    }
    return 0;
}

CMCCrystalItemObj* CMCCrystalData::getItemObject(u8 idx) {
    u16 adj = (u16)(idx + current * 30);
    if (adj < count) {
        CMCCrystalItemObj* obj =
            (CMCCrystalItemObj*)CItem_kindPtr(category, entries[adj].id);
        if (obj != 0 && obj->word0 != 0) return obj;
    }
    return 0;
}
