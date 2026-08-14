// Auto-scaffolded catalog TU for kyoshin/CEquipItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipItemBox.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <nw4r/math/math_types.h>
#include "nw4r/lyt/lyt_pane.h"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

// --- referenced retail symbols (C linkage: plain symbol names) ---
// C-linkage imports live in kyoshin/CEquipItemBox.hpp.
// func_80157C4C stays inline: CEquipChange.hpp / CItemBoxInfo.hpp declare it
// with different signatures and CEquipChange.cpp includes both (via
// CEquipChange.hpp -> CEquipItemBox.hpp).
extern "C" void* func_80157C4C(u8, s16);
// C-linkage import: CEquipChange.hpp declares func_801D47D4 with a u16 arg4,
// but retail passes the full 32-bit word (r6 -> mr -> func_801D8E34), so the
// u32 form is declared here to avoid a call-site mask. Kept local like
// func_80157C4C (the sibling headers disagree on the signature).
extern "C" void func_801D47D4(CItemBoxInfo*, u16, void*, u32);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
// Pane-visibility helpers (C-ABI retail symbols; declared in CEquipItemBox.hpp
// with extern "C" so MWCC emits the plain names).
// Float constants in .sdata2, referenced via sda21 reloc.
extern const float lbl_eu_80668B28;
extern const float lbl_eu_80668B04;

// varargs name formatter (MWCC keeps the unmangled library name).
int sprintf(char*, const char*, ...);

u8 CEquipItemBox::func_802865A0() { return unk_40; }

#pragma push
#pragma auto_inline off
#pragma optimize_for_size on
// Initialise a full item grid: clear every cell and reset the header fields.
extern "C" CEquipItemGrid* func_802824B4(CEquipItemGrid* grid) {
    // Reset each 8-byte cell to { unk0 = -1, rest = 0 }.
    CEquipItemData* const end = &grid->data[0x400];
    CEquipItemData* p = grid->data;
    do {
        func_80282574(p, -1, 0, 0, 0, 0, 0, 0);
        p++;
    } while (p < end);
    grid->count = 0;
    grid->cat = 0;
    grid->_pad2003 = 0;
    grid->idx = 0;
    grid->_pad2005 = 0;
    // Repopulate every cell from a cleared local (round-trips through copy).
    u32 i = 0;
    do {
        CEquipItemData temp;
        func_80282594(&grid->data[(u16)i], func_80282574(&temp, -1, 0, 0, 0, 0, 0, 0));
        i++;
    } while (i < 0x400);
    return grid;
}
#pragma pop

// Write a CEquipItemData from 8 separate scalar inputs.
#pragma push
#pragma auto_inline off
extern "C" CEquipItemData* func_80282574(CEquipItemData* dst, s16 a0, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7) {
    dst->unk0 = a0;
    dst->unk2 = a2;
    dst->unk3 = a3;
    dst->unk4 = a4;
    dst->unk5 = a5;
    dst->unk6 = a6;
    dst->unk7 = a7;
    return dst;
}

// Copy a CEquipItemData struct (src -> dst).
#pragma auto_inline off
extern "C" void func_80282594(CEquipItemData* dst, const CEquipItemData* src) {
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk3 = src->unk3;
    dst->unk4 = src->unk4;
    dst->unk5 = src->unk5;
    dst->unk6 = src->unk6;
    dst->unk7 = src->unk7;
}
#pragma pop


// Look up a grid element and return the item object pointer when present.
// noinline: retail callers emit a `bl` (same-TU helpers stay out of line).
#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) void* func_80282F34(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        u32* obj = (u32*)func_80157C4C(grid->cat, grid->data[offset].unk0);
        if (obj && *obj) {
            return obj;
        }
    }
    return 0;
}
#pragma pop

// Return the item kind (word >> 20) for the grid element, or 0.
#pragma push
#pragma auto_inline off
extern "C" u32 func_80282EC4(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        u32* obj = (u32*)func_80157C4C(grid->cat, grid->data[offset].unk0);
        if (obj && *obj) {
            return *obj >> 20;
        }
    }
    return 0;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
// Return the stored item short for a valid grid cell, or -1.
// noinline: retail callers emit a `bl` (cross-TU in the original build).
extern "C" __declspec(noinline) s16 func_80282E4C(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        CEquipItemData* item = &grid->data[offset];
        u32* obj = (u32*)func_80157C4C(grid->cat, item->unk0);
        if (obj && *obj) {
            return item->unk0;
        }
    }
    return -1;
}
#pragma pop

// Advance grid cursor index; wrap to 0 when it reaches the count (field_2003).
#pragma push
#pragma auto_inline off
void CEquipItemBox::func_80282DF8() {
    u8 v = (u8)field_2004 + 1;
    field_2004 = v;
    if ((s8)v >= field_2003) {
        field_2004 = 0;
    }
}
#pragma pop

// Step grid cursor index backwards; wrap to (count-1) when it goes below 0.
void CEquipItemBox::func_80282E24() {
    u8 v = (u8)field_2004 - 1;
    field_2004 = v;
    if ((s8)v < 0) {
        field_2004 = (u8)field_2003 - 1;
    }
}

// Validate the grid cell selected by (cursor row * 30 + param): for the
// equippable categories (2 and 4..8) scan the item's instance list for a
// "full" slot (slot id -1 with its flag bit 0 set -> return -3); an empty
// instance list and category 3 fall back to the cell's byte-3 clamp
// (-1 when the cell holds any count, else 0).
#pragma push
#pragma optimize_for_size on
extern "C" int func_80282FA0(CEquipItemGrid* grid, u16 param) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + param);
    if ((u32)offset < (u32)grid->count) {
        CEquipItemData* item = &grid->data[offset];
        CItemInstance* obj = (CItemInstance*)func_80157C4C(grid->cat, item->unk0);
        if (obj != 0 && obj->word != 0) {
            int cat = grid->cat;
            switch (cat) {
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8: {
                u8 count = (u8)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf30(obj);
                if (count != 0) {
                    for (u8 i = 0; i < count; i++) {
                        if ((s16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf40(obj, i) == -1) {
                            CItemInstance* entry = (CItemInstance*)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf2C(obj, i);
                            if (entry != 0 && (entry->flags & 1) != 0) {
                                return -3;
                            }
                        }
                    }
                    return -2;
                }
                return (item->unk3 >= 1) ? -1 : 0;
            }
            case 3:
                return (item->unk3 >= 1) ? -1 : 0;
            }
        }
    }
    return 0;
}
#pragma pop

// Return the grid cell's byte 3 when its item object is valid, else 0.
#pragma push
#pragma auto_inline off
extern "C" u8 func_80283118(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        CEquipItemData* item = &grid->data[offset];
        u32* obj = (u32*)func_80157C4C(grid->cat, item->unk0);
        if (obj && *obj) {
            return item->unk3;
        }
    }
    return 0;
}
#pragma pop

// Return the grid cell's byte 4 when its item object is valid, else 0.
extern "C" u8 func_80283190(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        CEquipItemData* item = &grid->data[offset];
        u32* obj = (u32*)func_80157C4C(grid->cat, item->unk0);
        if (obj && *obj) {
            return item->unk4;
        }
    }
    return 0;
}

// Return the grid cell's byte 6 when its item object is valid, else 0.
#pragma push
#pragma auto_inline off
extern "C" __declspec(noinline) u32 func_80283208(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        CEquipItemData* item = &grid->data[offset];
        u32* obj = (u32*)func_80157C4C(grid->cat, item->unk0);
        if (obj && *obj) {
            return item->unk6;
        }
    }
    return 0;
}
#pragma pop

// Look up the grid cell selected by (cursor row * 30 + param); return its
// byte-7 (unk7) value, or 0 when the cell index is out of range.
extern "C" u8 func_80283280(CEquipItemGrid* grid, u32 param) {
    u16 off = (u16)((s8)grid->idx * 30 + (int)param);
    if ((u32)off < 0x400) {
        return grid->data[off].unk7;
    }
    return 0;
}

// Fetch byte at grid index field_2004, or 0 if index >= 0x400.
#pragma push
#pragma auto_inline off
u8 CEquipItemBox::func_802832B4() {
    s8 idx = field_2004;
    if (idx >= 0x400) return 0;
    return field_2026[idx];
}
#pragma pop

// Return the grid cell's byte 2 when its item object is valid, else 0.
#pragma push
#pragma auto_inline off
extern "C" u8 func_802832D8(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        CEquipItemData* item = &grid->data[offset];
        u32* obj = (u32*)func_80157C4C(grid->cat, item->unk0);
        if (obj && *obj) {
            return item->unk2;
        }
    }
    return 0;
}
#pragma pop

// Format the grid cell's item name into the grid's name buffer (grid+0x2006);
// the cat-3 path appends a per-slot count string via a second sprintf.
// Returns the buffer, or 0 when the cell is out of range / empty.
#pragma push
#pragma optimize_for_size on
#pragma auto_inline off
extern "C" __declspec(noinline) char* func_80283350(CEquipItemGrid* grid, u32 param) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + param);
    if ((u32)offset >= (u32)grid->count) return 0;
    CItemInstance* obj = (CItemInstance*)func_80157C4C(grid->cat, grid->data[offset].unk0);
    if (obj == 0) return 0;
    if (obj->word == 0) return 0;
    CEquipItemBoxItemImplView* impl = (CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj);
    sprintf(&grid->field_2006[0], &lbl_eu_8050EFDC[0x2a], impl->vf20(obj));
    if (grid->cat == 3) {
        u8 v = (u8)impl->vf08(obj);
        char* str = func_80136190(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], 0x1e - (v - 1));
        sprintf(&grid->field_2006[0], &lbl_eu_8050EFDC[0x3b], &grid->field_2006[0], str);
    }
    return &grid->field_2006[0];
}
#pragma pop

extern "C" void func_8028345C(){}

extern "C" CEquipItemData* func_80283B24(CEquipItemData* dst, const CEquipItemData* src) {
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk3 = src->unk3;
    dst->unk4 = src->unk4;
    dst->unk5 = src->unk5;
    dst->unk6 = src->unk6;
    dst->unk7 = src->unk7;
    return dst;
}

#pragma push
#pragma auto_inline off
extern "C" u32 func_80283B60(CEquipItemGrid* grid, CItemInstance* obj) {
    return 0;
}
#pragma pop

extern "C" void func_80283E64(){}

// Bubble-sort the grid entries ascending by the item record's +4 flag
// halfword (sort key). Sibling of func_80285478's name-byte sort.
#pragma push
#pragma optimize_for_size on
extern "C" void func_80284144(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            if ((u32)objA->flags > (u32)objB->flags) {
                CEquipItemData tA;
                CEquipItemData tB;
                CEquipItemData tC;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by func_80283B60's per-item key
// (unsigned compare). Same 5-copy chained swap as func_80284144.
#pragma push
#pragma optimize_for_size on
extern "C" void func_80284244(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            if (func_80283B60(grid, objA) < func_80283B60(grid, objB)) {
                CEquipItemData tA;
                CEquipItemData tB;
                CEquipItemData tC;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by the item impl's slot-0x08 key
// (u16 compare). Same 3-temp chained copy swap as func_80284144.
#pragma push
#pragma dont_inline on
extern "C" void func_80284358(CEquipItemGrid* grid) {
    CItemInstance* objB;  // born early so its web claims a high saved register (retail r29)
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u16 keyB = (u16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objB))->vf08(objB);
            if ((u16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objA))->vf08(objA) < keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries so items whose name byte (bdat lookup at
// pattern offset 0x58 of the vf54 key) equals the search key float to the
// front (ascending equality flag). The key is the item impl's slot-0x54
// value; the 3-temp copy swap matches func_8028530C's shape.
#pragma push
#pragma dont_inline on
extern "C" void func_80284490(CEquipItemGrid* grid, int param) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 keyA = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objA))->vf54(objA);
            u32 keyB = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objB))->vf54(objB);
            u32 nameA = func_801361E8(lbl_eu_806640D8, &lbl_eu_8050EFDC[0x58], (u16)keyA);
            u32 nameB = func_801361E8(lbl_eu_806640D8, &lbl_eu_8050EFDC[0x58], (u16)keyB);
            u32 eqA = (u32)((u8)nameA == param);
            u32 eqB = (u32)((u8)nameB == param);
            if (eqA < eqB) {
                CEquipItemData v;
                CEquipItemData u;
                CEquipItemData t;
                func_80283B24(&t, a);
                func_80282594(a, func_80283B24(&u, b));
                func_80282594(b, func_80283B24(&v, &t));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries so items whose name byte (bdat lookup at the
// base pattern of the vf54 key) equals the search key float to the front.
// Sibling of func_80284490 with a different name-pattern offset.
#pragma push
#pragma dont_inline on
extern "C" void func_8028461C(CEquipItemGrid* grid, int param) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 keyA = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objA))->vf54(objA);
            u32 keyB = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objB))->vf54(objB);
            u32 nameA = func_801361E8(lbl_eu_806640D8, &lbl_eu_8050EFDC[0], (u16)keyA);
            u32 nameB = func_801361E8(lbl_eu_806640D8, &lbl_eu_8050EFDC[0], (u16)keyB);
            u32 eqA = (u32)((u8)nameA == param);
            u32 eqB = (u32)((u8)nameB == param);
            if (eqA < eqB) {
                CEquipItemData v;
                CEquipItemData u;
                CEquipItemData t;
                func_80283B24(&t, a);
                func_80282594(a, func_80283B24(&u, b));
                func_80282594(b, func_80283B24(&v, &t));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries by func_80285708's scaled item-progress key
// (double compare, swap when the left key is smaller so higher-progress
// entries bubble to the front). Same 3-temp chained copy swap.
#pragma push
#pragma dont_inline on
extern "C" void func_802847A4(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            double keyA = func_80285708((CEquipItemBox*)grid, objA);
            double keyB = func_80285708((CEquipItemBox*)grid, objB);
            if (keyA < keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by the item impl's slot-0x30 key
// (u16 compare applied at the compare site). Same 3-temp chained copy swap.
#pragma push
#pragma dont_inline on
extern "C" void func_802848C4(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        CItemInstance* objB;  // born early so its web claims a high saved register (retail r29)
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 keyB = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objB))->vf30(objB);
            u32 keyA = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objA))->vf30(objA);
            if ((u16)keyA < (u16)keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries by func_802857F0's flag-scan key (unsigned
// compare, swap when the left key is smaller so flag-set entries bubble to
// the front). Same 3-temp chained copy swap as func_80284144.
#pragma push
#pragma dont_inline on
extern "C" void func_80284A00(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 keyA = func_802857F0((CEquipItemBox*)grid, objA);
            u32 keyB = func_802857F0((CEquipItemBox*)grid, objB);
            if (keyA < keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Destructor clone: free object memory when this is non-null and delete flag set.
void* __dt__802825D0(void* self, int mode) {
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// Bubble-sort the grid entries by func_802857F0's flag-scan key (unsigned
// compare, swap when the left key is larger; descending sibling of
// func_80284A00). Same 3-temp chained copy swap.
#pragma push
#pragma dont_inline on
extern "C" void func_80284B18(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 keyA = func_802857F0((CEquipItemBox*)grid, objA);
            u32 keyB = func_802857F0((CEquipItemBox*)grid, objB);
            if (keyA > keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

extern "C" void func_80284C30(){}

// Bubble-sort the grid entries ascending by the item name-byte key from
// func_801361E8's bdat lookup (u8 compare). Same 3-temp chained copy swap.
#pragma push
#pragma dont_inline on
extern "C" void func_80284DCC(CEquipItemGrid* grid) {
    const char* base = lbl_eu_8050EFDC;
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 gA = lbl_eu_806640F4;
            u32 nameA = func_801361E8(gA, base + 0x61, func_80139358(objA->word >> 20));
            u32 gB = lbl_eu_806640F4;
            u32 nameB = func_801361E8(gB, base + 0x61, func_80139358(objB->word >> 20));
            if ((u8)nameA < (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by the item name-byte key from
// func_801361E8's bdat lookup (u8 compare, name pattern offset 0x69). Same
// 3-temp chained copy swap.
#pragma push
#pragma dont_inline on
extern "C" void func_80284F1C(CEquipItemGrid* grid) {
    const char* base = lbl_eu_8050EFDC;
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 gA = lbl_eu_806640F4;
            u32 nameA = func_801361E8(gA, base + 0x69, func_80139358(objA->word >> 20));
            u32 gB = lbl_eu_806640F4;
            u32 nameB = func_801361E8(gB, base + 0x69, func_80139358(objB->word >> 20));
            if ((u8)nameA < (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by the item name-byte key from
// func_801361E8's bdat lookup at name pattern offset 0x72 (u8 compare).
#pragma push
#pragma dont_inline on
extern "C" void func_8028506C(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u16 kindA = func_80139358(objA->word >> 20);
            u16 kindB = func_80139358(objB->word >> 20);
            u32 nameA = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050EFDC[0x72], kindA);
            u32 nameB = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050EFDC[0x72], kindB);
            if ((u8)nameA < (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by the item name-byte key from
// func_801361E8's bdat lookup at name pattern offset 0x7A (u8 compare).
#pragma push
#pragma dont_inline on
extern "C" void func_802851BC(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u16 kindA = func_80139358(objA->word >> 20);
            u16 kindB = func_80139358(objB->word >> 20);
            u32 nameA = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050EFDC[0x7a], kindA);
            u32 nameB = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050EFDC[0x7a], kindB);
            if ((u8)nameA < (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries so that items whose name byte matches the
// search key float to the front (ascending equality flag, u8 name lookup at
// pattern offset 0x17). The equality is evaluated via a subtraction so MWCC
// emits the cntlzw/srwi 0-or-1 idiom for both sides.
#pragma push
#pragma dont_inline on
extern "C" void func_8028530C(CEquipItemGrid* grid, int key) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u16 kindA = func_80139358(objA->word >> 20);
            u16 kindB = func_80139358(objB->word >> 20);
            u32 nameA = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050EFDC[0x17], kindA);
            u32 nameB = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050EFDC[0x17], kindB);
            u32 eqA = (u32)((u8)nameA == key);
            u32 eqB = (u32)((u8)nameB == key);
            if (eqA < eqB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries descending by the item name lookup byte
// (name pattern offset 0x82; swap when the left name byte is larger).
#pragma push
#pragma dont_inline on
extern "C" void func_80285478(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u16 kindA = func_80139358(objA->word >> 20);
            u16 kindB = func_80139358(objB->word >> 20);
            u32 nameA = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050EFDC[0x82], kindA);
            u32 nameB = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050EFDC[0x82], kindB);
            if ((u8)nameA > (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by the item impl's slot-0x90 key
// (u8 compare applied at the compare site). Same 3-temp chained copy swap.
#pragma push
#pragma dont_inline on
extern "C" void func_802855C8(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        CItemInstance* objA;  // born early so its web claims a high saved register (retail r29)
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 keyA = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objA))->vf90(objA);
            u32 keyB = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objB))->vf90(objB);
            if ((u8)keyA < (u8)keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                func_80283B24(&tA, a);
                func_80282594(a, func_80283B24(&tB, b));
                func_80282594(b, func_80283B24(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Scaled item-progress reader: look up the item's stored count and the row's
// flag word; when bit 4 is set, rescale the count by the drop-rate table and
// clamp the result at 999. Returns the value as a float (double-precision
// container, MWCC's fsubs magic-trick conversion).
#pragma push
#pragma optimize_for_size on
extern "C" double func_80285708(CEquipItemBox* self, CItemInstance* item) {
    u32 word = item->word;
    u32 g = lbl_eu_806640F4;
    u16 kind = func_80139358(word >> 20);
    u16 v1 = func_80136254(g, &lbl_eu_8050EFDC[0x8b], kind);
    u32 v2 = func_801361E8(g, &lbl_eu_8050EFDC[0x92], kind);
    u16 result = v1;
    CEquipItemBoxF64Conv c2;
    if (v2 & 4) {
        func_8009EC9C(1);
        u16 n = (u16)func_800A082C();
        int m = (int)((u32)v1 * (u32)n);
        CEquipItemBoxF64Conv c;
        c.w[0] = 0x43300000;
        c.w[1] = (u32)m ^ 0x80000000;
        result = (int)(lbl_eu_80668B2C * (c.d - lbl_eu_80668B18));
        if ((u16)result >= 0x3E7) result = 0x3E7;
    }
    c2.w[1] = (u32)result;
    c2.w[0] = 0x43300000;
    return c2.d - lbl_eu_80668B10;
}
#pragma pop

// Scan the item instance list: return 1 when any entry's flag word bit 0 is
// set, else 0. The instance accessor is re-fetched per iteration (retail
// shape); vtable slots [12]/[11] are the count and get-item methods.
#pragma push
#pragma optimize_for_size on
extern "C" u32 func_802857F0(CEquipItemBox* self, CItemInstance* item) {
    void* inst = CItem_initItemImplInstances(item);
    u16 count = ((u16(*)(void*, void*))(*(void***)inst)[12])(inst, item);
    for (u8 i = 0; i < count; i++) {
        void* inst2 = CItem_initItemImplInstances(item);
        CItemInstance* entry = (CItemInstance*)((void*(*)(void*, void*, u8))(*(void***)inst2)[11])(inst2, item, i);
        if (entry != 0 && (entry->flags & 1) != 0) {
            return 1;
        }
    }
    return 0;
}
#pragma pop

// Name-table scan: build a 1..10 name key, look it up; the hit index is the
// result. A miss falls back to 0xB when the kind is at most 0x4C, else 0.
#pragma push
#pragma optimize_for_size on
extern "C" u8 func_80285890(CEquipItemBox* self, CItemInstance* item) {
    u32 word = item->word;
    u32 v = lbl_eu_806640F4;
    u16 kind = func_80139358(word >> 20);
    for (u8 i = 1; i <= 10; i++) {
        char buf[0x28];
        sprintf(buf, &lbl_eu_8050EFDC[0x7], i);
        if (func_801361E8(v, buf, kind)) {
            return i;
        }
    }
    return (kind < 0x4D) ? 0xB : 0;
}
#pragma pop

#pragma push
#pragma auto_inline off
// C-ABI ctor (retail symbol __ct__CEIBCur, no class-length mangling).
extern "C" CEIBCur* __ct__CEIBCur(CEIBCur* self, void* arcResAcc) {
    self->mVtable = (void*)lbl_eu_80538704;
    self->mArcResAcc = arcResAcc;
    self->mpLayout = 0;
    self->mpAnimTrans0 = 0;
    self->mpAnimTrans1 = 0;
    self->mActive = 0;
    self->mVisible = 0;
    return self;
}
#pragma pop

// Destructor clone: free object memory when this is non-null and delete flag set.
void* __dt__80285954(void* self, int mode) {
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

extern "C" void func_80285994(){}

// Per-frame cursor update: play the entry animation (mpAnimTrans0) when idle,
// advance the page-cursor state when active, then animate the layout.
void CEIBCur::func_80285A18() {
    if (mpLayout == nullptr) return;
    switch (mVisible) {
    case 0:
        func_80137444((nw4r::lyt::AnimTransform*)mpAnimTrans0, lbl_eu_80668B28);
        break;
    case 1:
        func_80285B24();
        break;
    }
    ((nw4r::lyt::Layout*)mpLayout)->Animate(0);
}

#pragma push
#pragma auto_inline off
extern "C" void func_80285A90(CEIBCur* self, nw4r::lyt::DrawInfo* drawInfo) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mpLayout;
    if (layout == nullptr) return;
    if (self->mActive == 0) return;
    func_80137038(layout, drawInfo, 0, 1);
}
#pragma pop

// Reset a cursor object: stop it, clear its animation transforms, and release
// the bound layout if one is present.
extern "C" void func_80285ABC(CEIBCur* self) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mpLayout;
    self->mActive = 0;
    self->mpAnimTrans0 = 0;
    self->mpAnimTrans1 = 0;
    if (layout != 0) {
        layout->~Layout();
        self->mpLayout = 0;
    }
}

// Retail compiled func_802832D8/80285B24/80285B70 as separate functions; keep
// them out-of-line so callers reproduce the retail `bl` (sibling-TU pattern).
#pragma push
#pragma auto_inline off
void CEIBCur::func_80285B24() {
    // Advance the layout's animation; when the animation finishes, hide the
    // cursor and update the page state.
    if (func_80137444((nw4r::lyt::AnimTransform*)mpAnimTrans1, lbl_eu_80668B28) != 0) {
        mVisible = 0;
        func_80285B70(this);
    }
}

// Deactivate the cursor layout: unbind the second animation transform, bind
// the entry transform with playback enabled, reset its frame to 0 and step
// the layout animation.
void func_80285B70(CEIBCur* self) {
    ((nw4r::lyt::Layout*)self->mpLayout)->UnbindAnimation((nw4r::lyt::AnimTransform*)self->mpAnimTrans1);
    ((nw4r::lyt::Layout*)self->mpLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mpAnimTrans0);
    ((nw4r::lyt::Layout*)self->mpLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mpAnimTrans0, true);
    ((nw4r::lyt::AnimTransform*)self->mpAnimTrans0)->SetFrame(lbl_eu_80668B04);
    ((nw4r::lyt::Layout*)self->mpLayout)->Animate(0);
}
#pragma pop


CEIBPageCur::CEIBPageCur(void* arcResAcc) {
    __ct__CEIBCur(this, arcResAcc);  // base ctor (C-ABI)
    mVtable = (void*)lbl_eu_805386EC;
}

// Destructor clone: free object memory when this is non-null and delete flag set.
void* __dt__80285C44(void* self, int mode) {
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// Bind the cursor's layout and entry animation (func_80136E84/F08), position
// the two named panes at their .sdata2 offsets, unbind all layout animations,
// then hand the cursor to the deactivation helper (func_80285B70).
extern "C" void func_80285C84(CEIBCur* self) {
    func_80136E84((nw4r::lyt::Layout**)&self->mpLayout, (nw4r::lyt::ArcResourceAccessor*)self->mArcResAcc, &lbl_eu_8050EFDC[0xe7]);
    func_80136F08((nw4r::lyt::Layout*)self->mpLayout, (nw4r::lyt::AnimTransform**)&self->mpAnimTrans0, (nw4r::lyt::ArcResourceAccessor*)self->mArcResAcc, &lbl_eu_8050EFDC[0xff]);

    float vec1[3];
    float vec2[3];
    code80135FDC_setVec3(vec1, lbl_eu_80668B30, lbl_eu_80668B34, lbl_eu_80668B04);
    nw4r::lyt::Pane* pane1 =
        ((nw4r::lyt::Layout*)self->mpLayout)->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x11c], true);
    copyVEC3(&((CEquipItemBoxPaneView*)pane1)->mTranslate[0], vec1);

    code80135FDC_setVec3(vec2, lbl_eu_80668B38, lbl_eu_80668B34, lbl_eu_80668B04);
    nw4r::lyt::Pane* pane2 =
        ((nw4r::lyt::Layout*)self->mpLayout)->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x128], true);
    copyVEC3(&((CEquipItemBoxPaneView*)pane2)->mTranslate[0], vec2);

    ((nw4r::lyt::Layout*)self->mpLayout)->UnbindAllAnimation();
    func_80285B70(self);
}

extern "C" void __ct__CEquipItemBox(){}

// Destroy the embedded sub-objects in reverse construction order (retail
// __dt__13CEquipItemBoxFv): syswins, sort menu, cursors, then the two memory
// regions. MWCC supplies the this-null guard, the member-dtor dispatch and
// the flags>0 operator-delete tail from the dtor shape itself.
CEquipItemBox::~CEquipItemBox() {
    __dt__7CSysWinFv(&_padSysWin2[0], -1);
    __dt__7CSysWinFv(&_padSysWin1[0], -1);
    __dt__9CSortMenuFv(&_padSortMenu[0], -1);
    __dt__6CCur18Fv(&ccur18[0], -1);
    __dt__80285C44(&pagecur[0], -1);
    __dt__17UnkClass_8045F564Fv(&_pad04[0x10], -1);
    __dt__17UnkClass_8045F564Fv(&_pad04[0], -1);
}

// Load the three equip-box bind files (two CDeviceFile reads + one common
// archive read) into the file-handle slots, initialise the sort menu at +0x8C,
// then dispatch the layout-build virtual on both embedded system windows.
extern "C" void func_802861A8(CEquipItemBox* self) {
    self->field_24 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                           &lbl_eu_8050EFDC[0x134],
                                           reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::setHandleFlag1((CFileHandle*)self->field_24);
    self->field_28 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                           &lbl_eu_8050EFDC[0x14d],
                                           reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::setHandleFlag1((CFileHandle*)self->field_28);
    func_800A9D90();
    self->field_2C = CDeviceFile::readCommonArchiveFile(mtl::MemManager::getHandleMEM2(),
                                                        &lbl_eu_8050EFDC[0x166],
                                                        reinterpret_cast<IWorkEvent*>(self), 0, 0);
    func_801D3064(&self->_padSortMenu[0]);
    ((CEquipItemBoxSysWinView*)&self->_padSysWin1[0])->v32();
    ((CEquipItemBoxSysWinView*)&self->_padSysWin2[0])->v32();
}

// Per-frame update of the equip box: dispatch the entry-state machine by
// unk_41, animate the layout, then step all embedded cursors and windows.
extern "C" void func_80286264(CEquipItemBox* self) {
    if (self->unk_40 != 0 && self->unk_41 != 0) {
        switch (self->unk_41) {
        case 1:
            func_802889C0(self);
            break;
        case 3:
            func_80288A1C(self);
            break;
        case 4:
            func_80288A6C(self);
            break;
        case 5:
            if (CSysWin_isActive(&self->_padSysWin1[0]) != 0) {
                self->unk_41 = 2;
            }
            break;
        }
        self->field_38->Animate(0);
        func_80285A18((CEIBCur*)&self->_pad44[0]);
        func_80285A18((CEIBCur*)&self->pagecur[0]);
        func_801D202C(&self->ccur18[0]);
        func_801D3160(&self->_padSortMenu[0]);
        func_8022B748(&self->_padSysWin1[0]);
        func_8022B748(&self->_padSysWin2[0]);
    }
}

// Draw the equip box: layout + sort menu first, then the page cursor (when
// the grid spans more than one page), both system windows, and the selection
// cursor when every window is idle.
#pragma push
#pragma optimize_for_size on
extern "C" void func_80286340(CEquipItemBox* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->unk_40 == 0) return;
    if (self->unk_41 == 0) return;
    func_80137038(self->field_38, drawInfo, 0, 1);
    func_801D31F8(&self->_padSortMenu[0], drawInfo);
    u8 v = self->field_2026[0x35b];
    u8 x = (v != 0) ? v : 1;
    if (x > 1) {
        func_80285A90((CEIBCur*)&self->pagecur[0], drawInfo);
    }
    func_8022B7C8(&self->_padSysWin1[0], drawInfo);
    if (self->unk_37b != 0 && CSysWin_getUnk34(&self->_padSysWin2[0]) == 0) {
        func_80285A90((CEIBCur*)&self->_pad44[0], drawInfo);
    }
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) == 0) {
        if (func_801D3320(&self->_padSortMenu[0]) != 0 ||
            CSysWin_getUnk34(&self->_padSysWin1[0]) != 0 ||
            self->unk_375 != 0) {
            func_801D20B0(&self->ccur18[0], drawInfo);
        }
    }
    func_8022B7C8(&self->_padSysWin2[0], drawInfo);
}
#pragma pop


// Close/release the equip-item box: unload bdat, free the three file handles,
// delete the layout object, release the arc accessors and both memory regions,
// then tear down the embedded cursors, sort menu and both system windows.
extern "C" void func_80286454(CEquipItemBox* self) {
    func_8003AA8C__5CBdatFUl(5);
    func_801390E0__FPP11CFileHandle(&self->field_24);
    func_801390E0__FPP11CFileHandle(&self->field_28);
    func_801390E0__FPP11CFileHandle(&self->field_2C);
    self->unk_40 = 0;
    if (self->field_38) {
        delete self->field_38;
        self->field_38 = 0;
    }
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(self->field_30);
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(self->field_34);
    func_8045F778__17UnkClass_8045F564Fv(&self->_pad04[0]);
    func_8045F778__17UnkClass_8045F564Fv(&self->_pad04[0x10]);
    func_80285ABC((CEIBCur*)((u8*)self + 0x44));
    func_80285ABC((CEIBCur*)((u8*)self + 0x5c));
    func_801D3258(self->_padSortMenu);
}

// Check the sort menu and both system windows are ready; return unk_42.
extern "C" u8 func_8028652C(CEquipItemBox* self) {
    if (func_801D32DC(self->_padSortMenu) == 0) return 0;
    if (CSysWin_isReady(self->_padSysWin1) == 0) return 0;
    if (CSysWin_isReady(self->_padSysWin2) == 0) return 0;
    return self->unk_42;
}


u8 CEquipItemBox::func_802865A8() { return unk_43; }

// Return whether the equip box may be interacted with: a busy system window,
// an open cursor animation, or an active name pane all block further input.
int CEquipItemBox::func_802865B0() {
    if (CSysWin_getUnk34(_padSysWin1) != 0) return 1;
    if (unk_375 != 0) return 1;
    nw4r::lyt::Pane* pane =
        field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (pane != 0 && func_801C4648(pane) != 0) return 1;
    return func_801D3320(_padSortMenu);
}

int CEquipItemBox::func_80286650() {
    if (CSysWin_getUnk34(_padSysWin1) != 0) {
        return 1;
    }
    return func_801D3320(_padSortMenu);
}

u8 CEquipItemBox::func_80286698() { return unk_375; }

// Returns 1 when the first system window is active, otherwise queries the
// sort menu window's state.
int CEquipItemBox::func_802866A0() {
    if (CSysWin_getUnk34(_padSysWin1) != 0) {
        return 1;
    }
    return CSysWin_getUnk34(_padSysWin2);
}

// Initialise the equip item box state (idempotent via unk_41 guard).
void CEquipItemBox::func_802866E8() {
    if (unk_41 != 0) return;
    unk_41 = 1;
    unk_43 = 0;
    unk_1fe = 0;
    func_80289E70(this);
    unk_40 = 1;
    func_80138078__FUl(0x6d);
}

// Close the box when the entry animation (unk_41 == 2) has finished: reset
// the state bytes, hide the name pane and stop the sort-menu sound effects.
#pragma push
#pragma optimize_for_size on
void CEquipItemBox::func_80286740() {
    if (unk_41 != 2) return;
    if (func_801D3320(_padSortMenu) != 0) return;
    unk_41 = 3;
    unk_43 = 0;
    unk_58 = 0;
    pagecur[0x14] = 0;
    func_80138078__FUl(0x6);
    nw4r::lyt::Pane* pane =
        field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (pane != 0) {
        func_80124270(pane, 0);
        unk_378 = 0;
    }
}
#pragma pop

extern "C" void func_802867E0(){}

extern "C" void func_802869B4(){}

extern "C" void func_80286B94(){}

// Equip-box button dispatch (down/confirm handler): while both system windows
// are idle, either finish the sort-menu scroll (move the selection cursor to
// the menu state and play the confirm sound), page forward through the item
// list when the name pane is up, step the sort-menu page when unk_1f5 is -1,
// or advance the row/column selection and refresh the list.
#pragma push
#pragma optimize_for_size on
extern "C" void func_80286D7C(CEquipItemBox* self) {
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) return;
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) return;
    if (func_801D3320(&self->_padSortMenu[0]) != 0) {
        if (func_801D3328(&self->_padSortMenu[0]) == 0) return;
        func_801D377C(&self->_padSortMenu[0]);
        nw4r::math::VEC3 tmp;
        func_801D3454(&tmp, &self->_padSortMenu[0]);
        ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp);
        func_80138078__FUl(1);
        return;
    }
    if (self->unk_375 != 0) {
        // Name pane is up: scan the item list forward for the next non-empty
        // page (wrapping at 3 pages) and move the selection cursor to it.
        u16* arr = (u16*)(self->unk_20c + 0xb0);
        s8 i = (s8)(self->unk_376 + 1);
        while (i != (s8)self->unk_376) {
            if (i >= 3) i = 0;
            if (ArrayGet12(arr, (u8)(self->unk_377 + i * 4)) != 0) {
                self->unk_376 = (u8)i;
                break;
            }
            i++;
        }
        nw4r::math::VEC3 tmp2;
        func_801CB9D8(&tmp2, arr, (u8)(self->unk_377 + (s8)self->unk_376 * 4));
        ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp2);
        func_80138078__FUl(1);
        return;
    }
    if ((s8)self->unk_1f5 == -1) {
        if (self->unk_372 > 1) {
            func_8028A0E0(self);
            func_80289CC0(self);
        }
        return;
    }
    if ((int)self->unk_1f4 == 4) {
        self->unk_1f4 = 0;
        func_80286F6C(self);
        func_80289CC0(self);
    } else {
        u8 v = (u8)(self->unk_1f4 + 1);
        self->unk_1f4 = v;
        if ((s8)v >= 5) self->unk_1f4 = 0;
        func_80289CC0(self);
        func_80289AA4(self);
    }
    func_80138078__FUl(1);
}
#pragma pop

// Advance the equip-grid page cursor when the box is idle; wrap the page
// index at the grid extent and play the page-change sound for multi-page
// lists.
#pragma push
#pragma optimize_for_size on
void CEquipItemBox::func_80286F6C() {
    if (CSysWin_getUnk34(_padSysWin1) != 0) return;
    if (func_801D3320(_padSortMenu) != 0) return;
    CEquipItemGrid* grid = (CEquipItemGrid*)&_pad37D[1];
    ((CEquipItemBox*)grid)->func_80282DF8();
    if ((s8)unk_1f5 >= ((CEquipItemBox*)grid)->func_802832B4()) {
        unk_1f5 = (u8)(((CEquipItemBox*)grid)->func_802832B4() - 1);
    }
    func_80289754();
    func_80289AA4();
    u8 c = grid->_pad2003;
    u8 v = (c != 0) ? c : 1;
    if (v != 1) {
        func_80138078__FUl(0xa);
    }
}
#pragma pop

// Step the equip-grid page cursor backward when the box is idle; wrap the
// page index at the grid extent and play the page-change sound for multi-page
// lists (backward mirror of func_80286F6C).
extern "C" void func_80287024(CEquipItemBox* self) {
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) return;
    if (func_801D3320(&self->_padSortMenu[0]) != 0) return;
    CEquipItemGrid* grid = (CEquipItemGrid*)&self->_pad37D[1];
    func_80282E24((CEquipItemBox*)grid);
    if ((s8)self->unk_1f5 >= func_802832B4((CEquipItemBox*)grid)) {
        self->unk_1f5 = (u8)(func_802832B4((CEquipItemBox*)grid) - 1);
    }
    func_80289754(self);
    func_80289AA4(self);
    u8 c = grid->_pad2003;
    u8 v = (c != 0) ? c : 1;
    if (v != 1) {
        func_80138078__FUl(0xa);
    }
}

// Equip-box confirm: while the sort menu is busy, close the page state and
// re-open; otherwise move the sort menu to the cursor position and start its
// scroll (page 2 sound when the call came from the sub-page).
#pragma push
#pragma optimize_for_size on
extern "C" void func_802870DC(CEquipItemBox* self) {
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) return;
    if (self->unk_375 != 0) return;
    if (func_801D3320(&self->_padSortMenu[0]) != 0) {
        if (func_801D3328(&self->_padSortMenu[0]) == 0) return;
        func_80289CC0(self);
        self->unk_58 = 1;
        func_801D216C(&self->ccur18[0], 0);
        func_801D3408(&self->_padSortMenu[0]);
        func_80138078__FUl(6);
        return;
    }
    if (func_801D3328(&self->_padSortMenu[0]) == 0) return;
    nw4r::lyt::Pane* root = self->field_38->GetRootPane();
    nw4r::lyt::Pane* paneA = root->FindPaneByName(&lbl_eu_8050EFDC[0x193], true);
    nw4r::lyt::Pane* paneB = root->FindPaneByName(&lbl_eu_8050EFDC[0x18a], true);
    nw4r::math::VEC3 result;
    nw4r::math::VEC3 tmp;
    func_80137924(&result, paneB, paneA, root);
    func_801D3430(&self->_padSortMenu[0], &result);
    func_801D353C(&self->_padSortMenu[0], (u8)(self->unk_379 + self->unk_37a));
    self->unk_58 = 0;
    func_801D3454(&tmp, &self->_padSortMenu[0]);
    ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp);
    func_801D216C(&self->ccur18[0], 1);
    func_801D3330(&self->_padSortMenu[0]);
    self->unk_1f6 = 0;
    func_80138078__FUl(2);
}
#pragma pop

// Equip-box sub-page handler: while the first system window is active switch
// to page 2 of the sort menu; otherwise with the second window active step it;
// with both idle either re-open the busy sort menu or hide the name pane, and
// play the confirm sound when the call had no explicit sound flag.
#pragma push
#pragma optimize_for_size on
extern "C" void func_80287250(CEquipItemBox* self, int param) {
    if (self->unk_41 != 2) return;
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) {
        if (CSysWin_isActive(&self->_padSysWin1[0]) != 0) {
            self->unk_41 = 5;
            func_8022B8E4(&self->_padSysWin1[0]);
            func_801D216C(&self->ccur18[0], 0);
            self->unk_58 = 1;
            self->unk_374 = 0;
        }
        return;
    }
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) {
        if (CSysWin_isActive(&self->_padSysWin2[0]) != 0) {
            func_8022B8E4(&self->_padSysWin2[0]);
        }
        return;
    }
    nw4r::lyt::Pane* pane =
        self->field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (func_801D3320(&self->_padSortMenu[0]) != 0) {
        if (func_801D3328(&self->_padSortMenu[0]) == 0) return;
        func_80289CC0(self);
        self->unk_58 = 1;
        func_801D216C(&self->ccur18[0], 0);
        func_801D3408(&self->_padSortMenu[0]);
    } else if (self->unk_375 != 0) {
        self->unk_375 = 0;
        self->unk_58 = 1;
        func_801D216C(&self->ccur18[0], 0);
    } else if (func_801C4648(pane) != 0) {
        func_80124270(pane, 0);
        self->unk_378 = 0;
    }
    if (param == 0) {
        func_80138078__FUl(6);
    }
}
#pragma pop

extern "C" void func_802873D8(){}

// Advance page-2 system-window state while it is visible and active.
void CEquipItemBox::func_80287D58() {
    if (unk_375 != 0 && CSysWin_getUnk34(_padSysWin2) != 0
        && CSysWin_isActive(_padSysWin2) != 0) {
        func_8022B8E4(_padSysWin2);
    }
}

// Rebuild the sort-menu page list: stash the input value/count, pick the
// current page byte (cursor page when flag is set, else the value's bits 4-7),
// reset the page list, then repopulate it (page 3 alone, otherwise pages
// 2 + 4..8) and restore the page cursor for the 4..8 band.
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" void func_80287DB4(CEquipItemBox* self, u32 v, u32 w, int flag) {
    self->unk_1fc = (u16)v;
    self->field_27A4 = w;
    u32 raw;
    if (flag != 0) {
        raw = self->unk_36c[(s8)self->unk_373];
    } else {
        raw = (v >> 4) & 0xF;
    }
    u8 x = (u8)raw;
    func_8028A07C(self);
    if (x == 3) {
        func_8028A0C0(self, 3);
    } else {
        func_8028A0C0(self, 2);
        func_8028A0C0(self, 4);
        func_8028A0C0(self, 5);
        func_8028A0C0(self, 6);
        func_8028A0C0(self, 7);
        func_8028A0C0(self, 8);
        switch (x) {
        case 4: self->unk_373 = 1; break;
        case 5: self->unk_373 = 2; break;
        case 6: self->unk_373 = 3; break;
        case 7: self->unk_373 = 4; break;
        case 8: self->unk_373 = 5; break;
        }
    }
    func_8028A1DC(self);
    func_80289500(self, 1);
}
#pragma pop

u8 CEquipItemBox::func_80287EE8() {
    u8 val = unk_1fe;
    unk_1fe = 0;
    return val;
}

void CEquipItemBox::func_80287EFC(u32 val) { unk_20c = val; }

// Advance the equip-grid selection: map the low nibble of unk_1fc to a window
// kind (2..8), then hand the page grid, the selected page byte, the input
// category and the kind to func_80282610 and refresh the page list.
extern "C" void func_80287F04(CEquipItemBox* self, u32 param) {
    u8 b = (param != 0) ? func_801392B4((param >> 16) & 0xFF) : 0;
    u32 w = self->unk_1fc;
    CEquipItemGrid* grid = (CEquipItemGrid*)&self->_pad37D[1];
    u8 lo = (u8)(w & 0xF);
    u8 hi = (u8)((w >> 24) & 0xF);
    if (lo >= 1 && lo <= 3) {
        hi = 2;
    } else {
        switch (lo) {
        case 4: hi = 4; break;
        case 5: hi = 5; break;
        case 6: hi = 6; break;
        case 7: hi = 7; break;
        case 8: hi = 8; break;
        default: break;
        }
    }
    func_80282610(grid, self->unk_36c[(s8)self->unk_373], b, hi);
    func_80289754(self);
}

extern "C" void func_80287FE0(){}

extern "C" void func_802882A4(){}

// Test the equip-grid cursor row: return 1 when the selected cell holds the
// expected item category (kind >= 3) at the current page, else 0.
#pragma push
#pragma optimize_for_size on
int CEquipItemBox::func_8028847C() {
    u8 idx;
    CEquipItemGrid* grid = (CEquipItemGrid*)&_pad37D[1];
    idx = (u8)(unk_1f4 + unk_1f5 * 5);
    if (func_80283118(grid, idx) == 0) return 0;
    u8 cat = (u8)((unk_1fc >> 8) & 0xff);
    u8 b = func_801392B4(cat);
    u8 a = (u8)func_802832D8(grid, idx);
    if (b != a) return 0;
    u16 kind = (u16)func_80282EC4(grid, idx);
    u8 v = (u8)func_801392E4(kind);
    return (v - 2) != 0;
}
#pragma pop

extern "C" unsigned char func_80288530(u8* self) {
    s8 idx = *(s8*)(self + 0x373);
    return *(unsigned char*)(self + 0x36c + idx);
}

// Scan the 6 equipment slots for the item that owns the slot id selected by
// the current grid row. For each category definition, look up the item
// object, then walk its instance list for a slot whose id matches the grid
// row's item; the result is the global slot number (per-category base byte +
// instance index), or 0 when no slot matches.
#pragma push
#pragma optimize_for_size on
extern "C" u32 func_80288544(CEquipItemBox* self) {
    s16 target;  // declared before items so MWCC colors it r31 (retail order)
    CEquipItemBoxItemView* items;
    items = (CEquipItemBoxItemView*)func_8009EC9C(func_801392B4((self->unk_1fc >> 8) & 0xFF));
    u8 idx = (u8)((u8)self->unk_1f5 * 5 + self->unk_1f4);
    target = func_80282E4C((CEquipItemGrid*)&self->_pad37D[1], idx);
    CEquipItemBoxSlotTable table = lbl_eu_8050EF90;
    table.slots[0].item = items->field_26;
    table.slots[1].item = items->field_1c;
    table.slots[2].item = items->field_1e;
    table.slots[3].item = items->field_20;
    table.slots[4].item = items->field_22;
    table.slots[5].item = items->field_24;
    for (u8 e = 0; e < 6; e++) {
        s16 item = table.slots[e].item;
        if (item == -1) continue;
        CItemInstance* obj = (CItemInstance*)func_80157C4C(table.slots[e].cat, item);
        if (obj == 0) continue;
        if (obj->word == 0) continue;
        u8 count = (u8)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf30(obj);
        for (u8 i = 0; i < count; i++) {
            CEquipItemBoxItemImplView* impl = (CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj);
            s16 slot = impl->vf40(obj, i);
            if (slot == -1) continue;
            if (slot == target) return (u8)(i + table.slots[e].extra);
        }
    }
    return 0;
}
#pragma pop

extern "C" void func_802886D8(){}

extern "C" void func_8028876C(){}

#pragma push
#pragma optimize_for_size on
// Returns 1 when the lookup byte of the grid cell selected by (unk_1f5) equals
// func_801392B4's category byte; an unk_1f5 of -1 always returns 0.
extern "C" int func_80288948(CEquipItemBox* self) {
    if ((s8)self->unk_1f5 == -1) return 0;
    u8 idx = (u8)(self->unk_1f4 + (u8)self->unk_1f5 * 5);
    u8 cat = (u8)((self->unk_1fc >> 8) & 0xff);
    u32 b;
    u32 a = func_802832D8((CEquipItemGrid*)((u8*)self + 0x37E), idx);
    b = func_801392B4(cat);
    return a == b;
}
#pragma pop

// Play the entry animation at 0x3C; when it completes mark the cursors active.
extern "C" void func_802889C0(CEquipItemBox* self) {
    float f = lbl_eu_80668B28;
    if (func_80137444((nw4r::lyt::AnimTransform*)self->field_3C, f) != 0) {
        self->unk_41 = 2;
        self->unk_43 = 1;
        self->unk_58 = 1;
        self->pagecur[0x14] = 1;
        func_80289CC0(self);
    }
}

// Playback of the layout animation at 0x3C; when it finishes reset the state
// bytes 0x40-0x43 (unk_43=active-mark, unk_41/unk_40 cleared).
void CEquipItemBox::func_80288A1C() {
    if (func_80137510((nw4r::lyt::AnimTransform*)field_3C, lbl_eu_80668B28) != 0) {
        unk_43 = 1;
        unk_41 = 0;
        unk_40 = 0;
    }
}

// When the first system window is active, switch page-cursor state and refresh.
void CEquipItemBox::func_80288A6C() {
    if (CSysWin_isActive(_padSysWin1) != 0) {
        unk_41 = 2;
        func_801D216C(ccur18, 1);
        func_80289CC0(this);
    }
}

// If both pointer fields 0x34 and 0x38 are non-null, clear the sort menu and set unk_42.
void CEquipItemBox::func_80288AC0() {
    if (field_38 != 0 && field_34 != 0) {
        func_80139198(0);
        unk_42 = 1;
    }
}

extern "C" void func_80288B14(){}

extern "C" void func_80288E14(){}

extern "C" void func_802891B8(){}

// Equip-box flavour handler: (re)bind the item texture named by the current
// page/state into the layout, set its visibility pane, and refresh the
// name-pane text. mode 1 picks a per-flag texture, mode 2 a fixed one;
// otherwise the default page texture is used.
extern "C" void func_8028939C(CEquipItemBox* self, u32 mode, int num, int flag, u32 show) {
    char buf[0x20];
    int n = num + 1;
    sprintf(buf, &lbl_eu_8050EFDC[0x229], n);
    void* res = self->field_30->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1b0], 0);
    if (mode == 1) {
        if (flag != 0) {
            res = self->field_30->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x234], 0);
        } else {
            res = self->field_30->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x248], 0);
        }
    } else if (mode == 2) {
        res = self->field_30->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x25c], 0);
    }
    if (res != 0) {
        func_80137E7C(self->field_38, buf, (u32)res);
    }
    sprintf(buf, &lbl_eu_8050EFDC[0x270], n);
    nw4r::lyt::Pane* pane = self->field_38->GetRootPane()->FindPaneByName(buf, true);
    func_80124270(pane, show == 0 ? 1 : 0);
}

#pragma push
#pragma auto_inline off
extern "C" void func_80289500(CEquipItemBox* self, int a) {}

void CEquipItemBox::func_80289754() {}

// Refresh the equip item box list: bind the current selection's name into the
// layout, re-dispatch the interaction for the selected grid cell, then copy
// the per-page item ids/records into the item-box info list (+0xB0 region).
#pragma push
#pragma optimize_for_size on
void CEquipItemBox::func_80289AA4() {
    CEquipItemGrid* grid = (CEquipItemGrid*)&_pad37D[1];
    void* item;
    u8 idx = (u8)(unk_1f4 + unk_1f5 * 5);
    if ((s8)unk_1f5 == -1) {
        func_80136B4C(field_38, &lbl_eu_8050EFDC[0x2bf], ::func_8028D0EC(this), 0);
    } else {
        func_80136B4C(field_38, &lbl_eu_8050EFDC[0x2bf], func_80283350(grid, idx), 0);
    }
    if (getItemBoxState__FP12CItemBoxInfo((void*)unk_20c) == 0) return;
    if ((s8)unk_1f5 == -1) item = 0;
    else item = func_80282F34(grid, idx);
    func_801D47D4((CItemBoxInfo*)unk_20c, unk_1fc, item, func_80283208(grid, idx));
    func_801D4AE0((CItemBoxInfo*)unk_20c, 1, func_80283350(grid, idx));
    if ((s8)unk_1f5 == -1) item = 0;
    else item = func_80282F34(grid, idx);
    u16 kind;
    if ((s8)unk_1f5 == -1) kind = 0;
    else kind = func_80282EC4(grid, idx);
    func_8028A9CC(this, kind, (int)item);
    CEquipItemBoxItemListView* list = (CEquipItemBoxItemListView*)(unk_20c + 0xb0);
    for (u32 i = 0; i < 0xc; i++) {
        CEquipItemBoxPageDataView* view = (CEquipItemBoxPageDataView*)this;
        u16 v = ArrayGet12(&view->field_210[0], (u8)i);
        if (v == 0) continue;
        list->field_00[(u8)i] = v;
        list->field_A8[(u8)i] = func_801EF034((const u8*)&view->field_210[0], (u8)i);
        u8 b;
        if ((u8)i < 0xc) b = view->field_2C4[(u8)i];
        else b = 0;
        list->field_A8[(u8)i + 0xc] = b;
        s16 s;
        if ((u8)i < 0xc) s = view->field_2D0[(u8)i];
        else s = 0;
        list->field_C0[(u8)i] = s;
        nw4r::math::VEC3 tmp;
        func_801CB9D8(&tmp, &view->field_210[0], (u8)i);
        copyVEC3((float*)&list->field_18[(u8)i * 0xc], (const float*)&tmp);
    }
}
#pragma pop

extern "C" void func_80289CC0(CEquipItemBox* self){}
#pragma pop

// Rebuild the sort-menu page-list contents for the current page (unk_36c
// entry selected by unk_373): feed the page value to the sort menu, fill a
// per-page sound-key table (7/12/10 entries for pages 2/3/4..8), bind the
// page sound name into the layout, then re-add every entry whose key is
// positive and refresh the page-list cursor position.
#pragma push
#pragma optimize_for_size on
extern "C" void func_80289E70(CEquipItemBox* self) {
    u8 page = self->unk_36c[(s8)self->unk_373];
    // The 3rd/4th args are the two count bytes this function later re-reads;
    // MWCC hoists their address constants (r5/r6) into the prologue.
    func_801D3818(&self->_padSortMenu[0], page, &self->unk_379, &self->unk_37a);
    int buf[12];
    memset(buf, 0, 0x30);
    switch (page) {
    case 2:
        buf[0] = 0x44; buf[1] = 0x46; buf[2] = 0x55; buf[3] = 0x56;
        buf[4] = 0x47; buf[5] = 0x48; buf[6] = 0x45;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        buf[0] = 0x44; buf[1] = 0x49; buf[2] = 0x4a; buf[3] = 0x4b;
        buf[4] = 0x4c; buf[5] = 0x4d; buf[6] = 0x54; buf[7] = 0x47;
        buf[8] = 0x4f; buf[9] = 0x45;
        break;
    case 3:
        buf[0] = 0x44; buf[1] = 0x59; buf[2] = 0x5a; buf[3] = 0x5b;
        buf[4] = 0x5c; buf[5] = 0x5d; buf[6] = 0x5e; buf[7] = 0x5f;
        buf[8] = 0x60; buf[9] = 0x61; buf[10] = 0x62; buf[11] = 0x45;
        break;
    }
    u16 idx = func_8015780C(page);
    char* name = func_80136190(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], (u32)buf[idx]);
    func_80136B4C(self->field_38, &lbl_eu_8050EFDC[0x2d4], name, 0);
    if (func_801D32DC(&self->_padSortMenu[0]) != 0) {
        func_801D350C(&self->_padSortMenu[0]);
        u8 i = 0;
        while (1) {
            if ((int)buf[i] <= 0) break;
            func_801D3518(&self->_padSortMenu[0],
                          func_80136190(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], (u32)buf[i]));
            i++;
        }
        func_801D353C(&self->_padSortMenu[0], (u8)(self->unk_379 + self->unk_37a));
    }
}
#pragma pop

extern "C" void func_8028A07C(CEquipItemBox* self) {
    memset(self->unk_36c, 0, 6);
    self->unk_372 = 0;
    self->unk_373 = 0;
}

extern "C" void func_8028A0C0(CEquipItemBox* self, u8 val) {
    u8 idx = self->unk_372;
    if (idx >= 6) return;
    self->unk_36c[idx] = val;
    self->unk_372 = idx + 1;
}

// Step the sort-menu page selection backwards (wrapping from the first page to
// the last) with the current page stored in unk_373.
#pragma push
#pragma auto_inline off
extern "C" void func_8028A0E0(CEquipItemBox* self) {
    if (func_801D3320(self->_padSortMenu) != 0) return;
    u8 v = self->unk_373 + 1;
    self->unk_373 = v;
    if ((int)(s8)v >= self->unk_372) {
        self->unk_373 = 0;
    }
    func_8028A5D8(self, 0);
    func_80289500(self, 0);
    func_80138078__FUl(0x70);
}
#pragma pop

// Step the sort-menu page selection forwards (wrapping back to page 0 after the
// last page) with the current page stored in unk_373.
extern "C" void func_8028A160(CEquipItemBox* self) {
    if (func_801D3320(self->_padSortMenu) != 0) return;
    u8 v = self->unk_373 - 1;
    self->unk_373 = v;
    if ((s8)v < 0) {
        self->unk_373 = self->unk_372 - 1;
    }
    func_8028A5D8(self, 1);
    func_80289500(self, 0);
    func_80138078__FUl(0x70);
}

// Rebuild the sort-menu page list after a page change: for each of the 6 page
// slots, format its two pane names, set the pane colours (a highlighted pair
// when the slot is the current page, an idle pair otherwise), look the panes
// up by name and toggle their visibility, then hand the slot value to the
// page handler. Finally refresh the current-page cursor and the page list.
extern "C" void func_8028A1DC(CEquipItemBox* self) {
    for (u8 i = 0; i < 6; i++) {
        char buf1[0x20];
        char buf2[0x20];
        sprintf(buf1, &lbl_eu_8050EFDC[0x2df], i + 1);
        sprintf(buf2, &lbl_eu_8050EFDC[0x2ee], i + 1);
        u32 flagA = 0;
        u32 flagB = 0;
        u8 v = self->unk_36c[i];
        if (v != 0) {
            if (i == (s8)self->unk_373) {
                flagA = 1;
                flagB = 0;
            } else {
                flagA = 0;
                flagB = 1;
            }
            if ((u32)(v - 4) <= 4 || v == 2 || v == 0xb) {
                func_80139A18(self->field_38, buf1, lbl_eu_80664940, lbl_eu_80664948);
                func_80139A18(self->field_38, buf2, lbl_eu_80664940, lbl_eu_80664948);
            } else {
                func_80139A18(self->field_38, buf1, lbl_eu_80664930, lbl_eu_80664938);
                func_80139A18(self->field_38, buf2, lbl_eu_80664930, lbl_eu_80664938);
            }
        }
        nw4r::lyt::Pane* pane1 = self->field_38->GetRootPane()->FindPaneByName(buf1, true);
        func_80124270(pane1, flagA);
        nw4r::lyt::Pane* pane2 = self->field_38->GetRootPane()->FindPaneByName(buf2, true);
        func_80124270(pane2, flagB);
        func_8028A374(self, v, i);
    }
    func_8028A5D8(self, self->unk_373);
    func_80289500(self, 0);
}

extern "C" void func_8028A374(CEquipItemBox* self, u8 v, u8 i){}

#pragma push
#pragma auto_inline off
extern "C" void func_8028A5D8(CEquipItemBox* self, int a) {}


// Dispatch an equip-box interaction by the window kind byte: kinds 2/3 and
// the 4..8 band go to their dedicated handlers, anything else is ignored.
// noinline: retail callers emit a `bl` (same-TU dispatch stays out of line).
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_8028A9CC(CEquipItemBox* self, int a, int b) {
    func_8028AA64(self);
    u8 v = (u8)func_801392E4(a);
    if (v >= 4 && v <= 8) {
        func_8028B7CC(self, a, b);
    } else if (v == 2) {
        func_8028AF98(self, a, b);
    } else if (v == 3) {
        func_8028BE74(self, a, b);
    }
}
#pragma pop

extern "C" void func_8028AA64(CEquipItemBox* self){}

extern "C" void func_8028AF98(CEquipItemBox* self, int a, int b){}

extern "C" void func_8028B7CC(CEquipItemBox* self, int a, int b){}

extern "C" void func_8028BE74(CEquipItemBox* self, int a, int b){}
#pragma pop


extern "C" void func_8028C280(){}

extern "C" void func_8028CBCC(){}

// Map the current sort-menu page (unk_36c[unk_373]) to a sound-effect id and
// return its name string, or 0 when the page has no sound.
char* CEquipItemBox::func_8028D0EC() {
    u32 val = 0;
    u8 v = unk_36c[(s8)unk_373];
    switch (v) {
    case 0x00: val = 0x33; break;
    case 0x01: val = 0x3c; break;
    case 0x02: val = 0x34; break;
    case 0x03: val = 0x35; break;
    case 0x04: val = 0x36; break;
    case 0x05: val = 0x37; break;
    case 0x06: val = 0x38; break;
    case 0x07: val = 0x3d; break;
    case 0x08: val = 0x3e; break;
    case 0x09: val = 0x39; break;
    case 0x0a: val = 0x3b; break;
    case 0x0b:
    case 0x0c:
    case 0x0d:
        val = 0x3a;
        break;
    }
    if (val != 0) {
        return func_80136190(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], val);
    }
    return 0;
}

extern "C" void OnFileEvent__13CEquipItemBoxFP10CEventFile(){}

void CEquipItemBox::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static initialiser: reset/set the 24 .sbss colour-table entries used by the
// sort-menu page rebuild (func_80139A18 pairs). func_801D1F9C clears a table,
// func_801C4B60 sets its RGBA values.
extern "C" void sinit_8028DAB0() {
    func_801D1F9C(lbl_eu_80664920, 0);
    func_801D1F9C(lbl_eu_80664928, 0);
    func_801C4B60(lbl_eu_80664930, 0x79, 0x49, 0x7, 0x0);
    func_801C4B60(lbl_eu_80664938, 0xed, 0xcd, 0x83, 0x0);
    func_801C4B60(lbl_eu_80664940, 0x1a, 0x43, 0x53, 0x0);
    func_801C4B60(lbl_eu_80664948, 0xc4, 0xe8, 0xeb, 0x0);
    func_801C4B60(lbl_eu_80664950, 0x74, 0x54, 0x1d, 0x0);
    func_801C4B60(lbl_eu_80664958, 0xd5, 0xb9, 0x78, 0x0);
    func_801C4B60(lbl_eu_80664960, 0x3d, 0x68, 0x78, 0x0);
    func_801C4B60(lbl_eu_80664968, 0xc4, 0xe8, 0xeb, 0x0);
    func_801D1F9C(lbl_eu_80664970, 0);
    func_801D1F9C(lbl_eu_80664978, 0);
    func_801C4B60(lbl_eu_80664980, 0x80, 0x80, 0x80, 0x0);
    func_801C4B60(lbl_eu_80664988, 0x80, 0x80, 0x80, 0x0);
    func_801D1F9C(lbl_eu_80664990, 0);
    func_801D1F9C(lbl_eu_80664998, 0);
    func_801C4B60(lbl_eu_806649A0, 0xff, 0xff, 0xfa, 0x0);
    func_801C4B60(lbl_eu_806649A8, 0x80, 0x80, 0x80, 0x0);
    func_801D1F9C(lbl_eu_806649B0, 0);
    func_801D1F9C(lbl_eu_806649B8, 0);
    func_801C4B60(lbl_eu_806649C0, 0x12, 0xa3, 0xe7, 0x0);
    func_801C4B60(lbl_eu_806649C8, 0xff, 0xff, 0xff, 0x0);
    func_801C4B60(lbl_eu_806649D0, 0xb3, 0x9, 0xc0, 0x0);
    func_801C4B60(lbl_eu_806649D8, 0xff, 0xff, 0xff, 0x0);
}

extern "C" void func_80282610(CEquipItemGrid* grid, int v, int b, int hi) {}
extern "C" void func_80282D60() {}
