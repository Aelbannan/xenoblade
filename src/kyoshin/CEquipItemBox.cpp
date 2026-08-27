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
// TU-local: declared as int here (retail defers narrowing); other TUs
// declare it as u16 independently -- no cross-TU conflict since each TU
// compiles separately.
extern "C" int func_80136254(const void*, const void*, int);

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
u32 advanceAnimTransform(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
// Pane-visibility helpers (C-ABI retail symbols; declared in CEquipItemBox.hpp
// with extern "C" so MWCC emits the plain names).
// Float constants in .sdata2, referenced via sda21 reloc.
extern const float lbl_eu_80668B28;
extern const float lbl_eu_80668B04;

// varargs name formatter (MWCC keeps the unmangled library name).
int sprintf(char*, const char*, ...);
// SysWin name probe + pane-position reader (func_80289CC0 tail).
extern "C" void func_8022C1B4(void* out, void* syswin, u8 sel);
extern "C" void func_801375A0(nw4r::math::VEC3* out, nw4r::lyt::Pane* pane);

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
extern "C" __declspec(noinline) CEquipItemData* func_80282574(CEquipItemData* dst, s16 a0, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7) {
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
extern "C" __declspec(noinline) void func_80282594(CEquipItemData* dst, const CEquipItemData* src) {
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
#pragma auto_inline off
#pragma optimize_for_size on
extern "C" __declspec(noinline) s8 func_80282FA0(CEquipItemGrid* grid, u16 param) {
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
                return item->unk3 ? -1 : 0;
            }
            case 3:
                return item->unk3 ? -1 : 0;
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

// Return the grid cell's byte 7 (unk7) value, or 0 when out of range.
// noinline: retail callers emit a `bl` (func_80289754's 30-cell loop).
extern "C" __declspec(noinline) u8 func_80283280(CEquipItemGrid* grid, u32 param) {
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
    if ((u32)offset < (u32)grid->count) {
        CItemInstance* obj = (CItemInstance*)func_80157C4C(grid->cat, grid->data[offset].unk0);
        if (obj != 0) {
            if (obj->word != 0) {
                sprintf(&grid->field_2006[0], &lbl_eu_8050EFDC[0x2a],
                        ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf20(obj));
                if (grid->cat == 3) {
                    // Retail re-fetches the impl accessor before the slot probe
                    // and keeps v unmasked until the count expression.
                    u32 v = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf08(obj);
                    char* str = func_80136190(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], 0x1e - ((u8)v - 1));
                    sprintf(&grid->field_2006[0], &lbl_eu_8050EFDC[0x3b], &grid->field_2006[0], str);
                }
                return &grid->field_2006[0];
            }
        }
    }
    return 0;
}
#pragma pop

// Dispatch the equip-box grid sort/refresh by category and page: categories
// 4..8 use the page-indexed sort chain, category 2 the flag-scan chain, and
// category 3 the name-byte chain (page 3..0xB special-cases re-sort after a
// category rebuild). optimize_for_size selects retail's stmw r30 prologue.
#pragma push
#pragma optimize_for_size on
#pragma auto_inline off
extern "C" void func_8028345C(CEquipItemGrid* grid) {
    u8 cat = grid->cat;
    func_80283E64(grid, cat);
    u8 page = (u8)func_8015780C(cat);
    // Category dispatch first (retail test order), then the three per-category
    // page dispatches with the cat-2 body laid out first (retail body order).
    if ((u32)(cat - 4) <= 4) goto cat48;
    if (cat == 2) goto cat2;
    if (cat == 3) goto cat3;
    goto done;
cat2:
    func_80284144(grid);
    switch (page) {
    case 0:
        func_80284C30(grid, 1);
        break;
    case 1:
        func_80284A00(grid); func_802848C4(grid); func_802847A4(grid);
        func_80284C30(grid, 1);
        break;
    case 2:
        func_80284A00(grid); func_802848C4(grid); func_802847A4(grid);
        func_80284DCC(grid); func_80284C30(grid, 1);
        break;
    case 3:
        func_80284A00(grid); func_802848C4(grid); func_802847A4(grid);
        func_80284F1C(grid); func_80284C30(grid, 1);
        break;
    case 4:
        func_802847A4(grid); func_802848C4(grid); func_80284B18(grid);
        func_80284C30(grid, 1);
        break;
    case 5:
        func_802847A4(grid); func_802848C4(grid); func_80284A00(grid);
        func_80284C30(grid, 1);
        break;
    case 6:
        func_80284244(grid);
        break;
    }
    goto done;
cat48:
    func_80284144(grid);
    if (page <= 9) {
        switch (page) {
        case 0:
            func_8028530C(grid, 1); func_8028530C(grid, 2); func_8028530C(grid, 3);
            func_80284A00(grid); func_802848C4(grid); func_8028506C(grid);
            break;
        case 1:
            func_8028530C(grid, 1); func_8028530C(grid, 2); func_8028530C(grid, 3);
            func_80284A00(grid); func_802848C4(grid); func_802851BC(grid);
            break;
        case 2:
            func_80284A00(grid); func_802848C4(grid); func_8028506C(grid);
            func_8028530C(grid, 3); func_8028530C(grid, 2); func_8028530C(grid, 1);
            break;
        case 3:
            func_80284A00(grid); func_802848C4(grid); func_8028506C(grid);
            func_8028530C(grid, 3); func_8028530C(grid, 1); func_8028530C(grid, 2);
            break;
        case 4:
            func_80284A00(grid); func_802848C4(grid); func_8028506C(grid);
            func_8028530C(grid, 2); func_8028530C(grid, 1); func_8028530C(grid, 3);
            break;
        case 5:
            func_8028530C(grid, 1); func_8028530C(grid, 2); func_8028530C(grid, 3);
            func_8028506C(grid); func_80284A00(grid); func_802848C4(grid);
            func_80285478(grid);
            break;
        case 6:
            func_8028530C(grid, 1); func_8028530C(grid, 2); func_8028530C(grid, 3);
            func_8028506C(grid); func_802848C4(grid); func_80284B18(grid);
            break;
        case 7:
            func_8028530C(grid, 1); func_8028530C(grid, 2); func_8028530C(grid, 3);
            func_8028506C(grid); func_802848C4(grid); func_80284A00(grid);
            break;
        case 8:
        case 9:
            func_80284244(grid);
            break;
        }
    }
    goto done;
cat3:
    if (page <= 0xB) {
        switch (page) {
        case 0:
            func_80284144(grid);
            break;
        case 1:
            func_802855C8(grid); func_80283E64(grid, cat); func_80284358(grid);
            break;
        case 2:
            func_80283E64(grid, cat); func_802855C8(grid); func_80284358(grid);
            func_8028461C(grid, 2); func_8028461C(grid, 0); func_8028461C(grid, 1);
            break;
        case 3:
            func_80283E64(grid, cat); func_802855C8(grid); func_80284358(grid);
            func_8028461C(grid, 1); func_8028461C(grid, 0); func_8028461C(grid, 2);
            break;
        case 4:
            func_80283E64(grid, cat); func_802855C8(grid); func_80284358(grid);
            func_8028461C(grid, 2); func_8028461C(grid, 1); func_8028461C(grid, 0);
            break;
        case 5:
            func_8028461C(grid, 2); func_8028461C(grid, 1); func_8028461C(grid, 0);
            func_802855C8(grid); func_80284358(grid); func_80283E64(grid, cat);
            func_80284490(grid, 1);
            break;
        case 6:
            func_8028461C(grid, 2); func_8028461C(grid, 1); func_8028461C(grid, 0);
            func_802855C8(grid); func_80284358(grid); func_80283E64(grid, cat);
            func_80284490(grid, 2);
            break;
        case 7:
            func_8028461C(grid, 2); func_8028461C(grid, 1); func_8028461C(grid, 0);
            func_802855C8(grid); func_80284358(grid); func_80283E64(grid, cat);
            func_80284490(grid, 3);
            break;
        case 8:
            func_8028461C(grid, 2); func_8028461C(grid, 1); func_8028461C(grid, 0);
            func_802855C8(grid); func_80284358(grid); func_80283E64(grid, cat);
            func_80284490(grid, 4);
            break;
        case 9:
            func_8028461C(grid, 2); func_8028461C(grid, 1); func_8028461C(grid, 0);
            func_802855C8(grid); func_80284358(grid); func_80283E64(grid, cat);
            func_80284490(grid, 5);
            break;
        case 10:
            func_8028461C(grid, 2); func_8028461C(grid, 1); func_8028461C(grid, 0);
            func_802855C8(grid); func_80284358(grid); func_80283E64(grid, cat);
            func_80284490(grid, 6);
            break;
        case 11:
            func_80284244(grid);
            break;
        }
    }
    goto done;
done: ;
}
#pragma pop

extern "C" __declspec(noinline) CEquipItemData* func_80283B24(CEquipItemData* dst, const CEquipItemData* src) {
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
// Per-item display-count key: category band selects the computation -
// cat 3 scales the bdat count by a per-item float record, cat 9 sums four
// instance-slot records scaled by the same table, everything else multiplies
// by an accumulated per-count increment. Result is returned *10.
extern "C" __declspec(noinline) u32 func_80283B60(CEquipItemGrid* grid, CItemInstance* item) {
    u32 word = item->word;
    u16 kind = func_80139358(word >> 20);
    u8 cat = (word >> 12) & 0xF;
    u32 result = 0;
    if (cat == 3) {
        u32 key = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf54(item);
        u32 id = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf08(item);
        u16 count =
            (u16)func_80136254((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0x40], key);
        CEquipItemBoxF32Record rec = lbl_eu_8050EF68;
        // int->double via the 0x4330 magic, then scale by rec[id] and truncate.
        result = __cvt_fp2unsigned((double)(u32)count * (double)rec.f[id & 0x3F]);
    } else if (cat == 9) {
        u32 id = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf08(item);
        CEquipItemBoxF32Record rec = lbl_eu_8050EF68;
        for (u8 i = 0; i < 4; i++) {
            CItemInstance* entry =
                ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf2C(item, i);
            if (entry != 0) {
                u16 count = (u16)func_80136254((const void*)lbl_eu_806640D8,
                                               &lbl_eu_8050EFDC[0x40], i);
                double dcount = (double)(u32)count;
                double dacc = (double)(u32)result;
                result = __cvt_fp2unsigned(
                    lbl_eu_80668B24 * (dcount * rec.f[id & 0x3F]) + dacc);
            }
        }
    } else {
        u32 v = func_801393CC(kind);
        u32 count = func_80136254((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0x40], kind);
        u8 n = (u8)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf30(item);
        float inc = lbl_eu_80668B28;
        for (u8 i = 0; i < n; i++) {
            inc += lbl_eu_80668B08;
        }
        result = __cvt_fp2unsigned((double)(u32)(u16)count * inc);
    }
    return result * 10;
}
#pragma pop

// Category-dispatching bubble sort over the grid entries (the shared first
// pass of the page sort chain): cat 3 sorts descending by the vf54 name key,
// cat 9 by slot occupancy / byte-7 clamp, everything else by the raw kind.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80283E64(CEquipItemGrid* grid, u32 cat) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            // objB declared first so its web claims the higher saved
            // register (retail r23), matching the sibling sorts.
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            if (cat == 3) {
                // Descending vf54 name-key sort (u16 compare).
                CItemInstance* implA =
                    (CItemInstance*)CItem_initItemImplInstances(objA);
                CItemInstance* implB =
                    (CItemInstance*)CItem_initItemImplInstances(objB);
                u16 keyA = (u16)((CEquipItemBoxItemImplView*)implB)->vf54(objB);
                u16 keyB = (u16)((CEquipItemBoxItemImplView*)implA)->vf54(objA);
                if (keyA < keyB) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    func_80283B24(&tA, a);
                    func_80283B24(&tB, b);
                    func_80282594(a, &tB);
                    func_80283B24(&tC, &tA);
                    func_80282594(b, &tC);
                    swapped = 1;
                }
            } else if (cat == 9) {
                // Slot-occupancy probe: items with instances float to the
                // front, ties fall back to the byte-7 clamp compare.
                int kA = func_801C6E90(objA);
                int kB = func_801C6E90(objB);
                if (((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objA))->vf80(objA)
                    != 0) {
                    kA = 0;
                }
                if (((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(objB))->vf80(objB)
                    != 0) {
                    kB = 0;
                }
                int doSwap;
                if (kA != 0 || kB != 0) {
                    doSwap = kA > kB;
                } else {
                    doSwap = (objA->unk7 & 0x3F) > (objB->unk7 & 0x3F);
                }
                if (doSwap) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    func_80283B24(&tA, a);
                    func_80283B24(&tB, b);
                    func_80282594(a, &tB);
                    func_80283B24(&tC, &tA);
                    func_80282594(b, &tC);
                    swapped = 1;
                }
            } else {
                // Default: descending raw-kind order.
                u16 kindA = func_80139358(objA->word >> 20);
                u16 kindB = func_80139358(objB->word >> 20);
                if (kindA > kindB) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    func_80283B24(&tA, a);
                    func_80283B24(&tB, b);
                    func_80282594(a, &tB);
                    func_80283B24(&tC, &tA);
                    func_80282594(b, &tC);
                    swapped = 1;
                }
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by the item record's +4 flag
// halfword (sort key). Sibling of func_80285478's name-byte sort.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80284144(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            if ((u32)objA->flags > (u32)objB->flags) {
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

// Bubble-sort the grid entries ascending by func_80283B60's per-item key
// (unsigned compare). Same 5-copy chained swap as func_80284144.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80284244(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            if (func_80283B60(grid, objA) < func_80283B60(grid, objB)) {
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

// Bubble-sort the grid entries ascending by the item impl's slot-0x08 key
// (u16 compare). Same 3-temp chained copy swap as func_80284144.
#pragma push
#pragma dont_inline on
extern "C" __declspec(noinline) void func_80284358(CEquipItemGrid* grid) {
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
extern "C" __declspec(noinline) void func_80284490(CEquipItemGrid* grid, int param) {
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
extern "C" __declspec(noinline) void func_8028461C(CEquipItemGrid* grid, int param) {
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
extern "C" __declspec(noinline) void func_802847A4(CEquipItemGrid* grid) {
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
extern "C" __declspec(noinline) void func_802848C4(CEquipItemGrid* grid) {
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
extern "C" __declspec(noinline) void func_80284A00(CEquipItemGrid* grid) {
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
extern "C" __declspec(noinline) void func_80284B18(CEquipItemGrid* grid) {
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

// Dual-mode bubble sort over the grid entries keyed by func_80285890's
// name-table key. mode 0: ascending u8-key sort; other: move entries whose
// key equals the grid's _pad2005 byte toward the front (one swap per pass).
extern "C" u8 func_80285890(CEquipItemBox* self, CItemInstance* item);
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80284C30(CEquipItemGrid* grid, u32 mode) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            CItemInstance* objA = (CItemInstance*)func_80157C4C(grid->cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)func_80157C4C(grid->cat, b->unk0);
            u32 keyA = func_80285890((CEquipItemBox*)grid, objA);
            u32 keyB = func_80285890((CEquipItemBox*)grid, objB);
            if (mode != 0) {
                u8 g = grid->_pad2005;
                if ((g == (u8)keyA) < (g == (u8)keyB)) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    func_80283B24(&tA, a);
                    func_80283B24(&tB, b);
                    func_80282594(a, &tB);
                    func_80283B24(&tC, &tA);
                    func_80282594(b, &tC);
                    swapped = 1;
                }
            } else {
                if ((u8)keyA > (u8)keyB) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    func_80283B24(&tA, a);
                    func_80283B24(&tB, b);
                    func_80282594(a, &tB);
                    func_80283B24(&tC, &tA);
                    func_80282594(b, &tC);
                    swapped = 1;
                }
            }
        }
        if (swapped == 0) break;
    }
}
#pragma pop

// Bubble-sort the grid entries ascending by the item name-byte key from
// func_801361E8's bdat lookup (u8 compare). Same 3-temp chained copy swap.
#pragma push
#pragma dont_inline on
extern "C" __declspec(noinline) void func_80284DCC(CEquipItemGrid* grid) {
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
extern "C" __declspec(noinline) void func_80284F1C(CEquipItemGrid* grid) {
    const char* base = lbl_eu_8050EFDC;
    CEquipItemData* a;
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            a = &grid->data[j];
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
extern "C" __declspec(noinline) void func_8028506C(CEquipItemGrid* grid) {
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
extern "C" __declspec(noinline) void func_802851BC(CEquipItemGrid* grid) {
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
extern "C" __declspec(noinline) void func_8028530C(CEquipItemGrid* grid, int key) {
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
extern "C" __declspec(noinline) void func_80285478(CEquipItemGrid* grid) {
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
extern "C" __declspec(noinline) void func_802855C8(CEquipItemGrid* grid) {
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
// Scaled item-progress reader: look up the item's stored count and the row's
// flag word; when bit 4 is set, rescale the count by the drop-rate table and
// clamp the result at 999. All FP math is single precision: the (f32) casts
// make MWCC emit its own 0x4330/lfd/fsubs s32->f32 magic (pool constants
// lbl_eu_80668B18/B10 are compiler-generated, not source references).
extern "C" double func_80285708(CEquipItemBox* self, CItemInstance* item) {
    u32 word = item->word;
    u32 g = lbl_eu_806640F4;
    u16 kind = func_80139358(word >> 20);
    // Int-typed variable + (u16) masks at USE sites only (retail defers
    // narrowing; the TU-local int declaration avoids the store mask).
    int result = func_80136254((const void*)g, &lbl_eu_8050EFDC[0x8b], kind);
    u32 v2 = func_801361E8(g, &lbl_eu_8050EFDC[0x92], kind);
    if (v2 & 4) {
        void* obj = func_8009EC9C(1);
        u16 n = (u16)func_800A082C(obj);
        result = (int)(lbl_eu_80668B2C * (f32)(int)((u32)(u16)result * (u32)n));
        if ((u16)result >= 0x3E7) result = 0x3E7;
    }
    return (f32)(u16)result;
}
#pragma pop

// Scan the item instance list: return 1 when any entry's flag word bit 0 is
// set, else 0. The instance accessor is re-fetched per iteration (retail
// shape); vtable slots [12]/[11] are the count and get-item methods.
#pragma push
#pragma optimize_for_size on
extern "C" u32 func_802857F0(CEquipItemBox* self, CItemInstance* item) {
    u16 count = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf30(item);
    for (u8 i = 0; i < count; i++) {
        CItemInstance* entry =
            ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf2C(item, i);
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
extern "C" __declspec(noinline) u8 func_80285890(CEquipItemBox* self, CItemInstance* item) {
    u32 word = item->word;
    u32 v = lbl_eu_806640F4;
    u16 kind = func_80139358(word >> 20);
    for (u8 i = 1; i <= 10; i++) {
        char buf[0x28];
        sprintf(buf, &lbl_eu_8050EFDC[0x7], i);
        if ((u8)func_801361E8(v, buf, kind)) {
            return i;
        }
    }
    // Miss fallback: MWCC lowers the equality test as a subi/subic/subfe
    // carry chain masked with 0xB.
    return (kind == 0x4C) ? 0xB : 0;
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

// func_80285994 (us-80287e18): layout-cursor setup. Retail calls (relocs now
// resolved -- the old comment's "MetroTRK/undecompiled-TU" blocker is stale):
//   1. buildLayout(&mpLayout, mArcResAcc, "mf00_reg00_curs07.brlyt")
//   2. bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, "..._roop.brlan")
//   3. bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, "..._on.brlan")
//   4. virtual slot 36 (vtable+0x24) on mpLayout
//   5. func_80285B70(cur)
// The name strings are pooled at lbl_eu_8050EFDC+0x97/0xAF/0xCC.
struct CEIBCurLayoutVt {
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();  // vtable+0x24 (slot 36)
};
#pragma push
#pragma optimize_for_size on
extern "C" void func_80285994(CEIBCur* cur) {
    buildLayout((nw4r::lyt::Layout**)&cur->mpLayout,
                  (nw4r::lyt::ArcResourceAccessor*)cur->mArcResAcc,
                  &lbl_eu_8050EFDC[0x97]);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)cur->mpLayout,
                  (nw4r::lyt::AnimTransform**)&cur->mpAnimTrans0,
                  (nw4r::lyt::ArcResourceAccessor*)cur->mArcResAcc,
                  &lbl_eu_8050EFDC[0xAF]);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)cur->mpLayout,
                  (nw4r::lyt::AnimTransform**)&cur->mpAnimTrans1,
                  (nw4r::lyt::ArcResourceAccessor*)cur->mArcResAcc,
                  &lbl_eu_8050EFDC[0xCC]);
    reinterpret_cast<CEIBCurLayoutVt*>(cur->mpLayout)->_v24();
    func_80285B70(cur);
}
#pragma pop

// Per-frame cursor update: play the entry animation (mpAnimTrans0) when idle,
// advance the page-cursor state when active, then animate the layout.
void CEIBCur::func_80285A18() {
    if (mpLayout == nullptr) return;
    switch (mVisible) {
    case 0:
        advanceAnimTransform((nw4r::lyt::AnimTransform*)mpAnimTrans0, lbl_eu_80668B28);
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
    drawLayout(layout, drawInfo, 0, 1);
}
#pragma pop

// Reset a cursor object: stop it, clear its animation transforms, and release
// the bound layout if one is present.
// delete layout (not ~Layout()): the retail passes the deleting flag 1 to the
// virtual dtor; the delete's own null-check is the retail's dead second beq
// (beq skip-all from the guard, beq mpLayout-zero block from the delete).
extern "C" __declspec(noinline) void func_80285ABC(CEIBCur* self) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mpLayout;
    self->mActive = 0;
    self->mpAnimTrans0 = 0;
    self->mpAnimTrans1 = 0;
    if (layout != 0) {
        delete layout;
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
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)mpAnimTrans1, lbl_eu_80668B28) != 0) {
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


extern "C" __declspec(noinline) CEIBPageCur* __ct__CEIBPageCur(CEIBPageCur* self, void* arcResAcc) {
    __ct__CEIBCur(self, arcResAcc);  // base ctor (C-ABI)
    self->mVtable = (void*)lbl_eu_805386EC;
    return self;
}
// noinline: retail callers (OnFileEvent) emit a `bl` to __ct__CEIBPageCur.

// Destructor clone: free object memory when this is non-null and delete flag set.
// noinline: the dtor body's flag test is constant-foldable at inlined call
// sites (mode -1) and would be elided entirely -- the retail keeps the `bl`.
extern "C" __declspec(noinline) void* __dt__80285C44(void* self, int mode) {
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

// Bind the cursor's layout and entry animation (buildLayout/F08), position
// the two named panes at their .sdata2 offsets, unbind all layout animations,
// then hand the cursor to the deactivation helper (func_80285B70).
extern "C" void func_80285C84(CEIBCur* self) {
    buildLayout((nw4r::lyt::Layout**)&self->mpLayout, (nw4r::lyt::ArcResourceAccessor*)self->mArcResAcc, &lbl_eu_8050EFDC[0xe7]);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)self->mpLayout, (nw4r::lyt::AnimTransform**)&self->mpAnimTrans0, (nw4r::lyt::ArcResourceAccessor*)self->mArcResAcc, &lbl_eu_8050EFDC[0xff]);

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

// Construct every embedded sub-object in retail order: vtable, the two
// memory regions, the zeroed handle/layout/state fields, the three cursors,
// sort menu and both system windows. The member CSortMenu is populated from
// a destroyed stack temp (sub-object copy + scalar copies), as are both
// system windows (via func_8016742C). The grid is initialized in place and
// then overwritten with a fresh stack grid via a whole-struct assignment;
// finally seven func_80157824 preload calls run.
#pragma push
#pragma optimize_for_size on
CEquipItemBox::CEquipItemBox() {
    mVtbl = (void*)lbl_eu_80538658;
    __ct__17UnkClass_8045F564Fv(&_pad04[0]);
    __ct__17UnkClass_8045F564Fv(&_pad04[0x10]);
    field_24 = 0;
    field_28 = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
    field_38 = 0;
    field_3C = 0;
    unk_40 = 0;
    unk_41 = 0;
    unk_42 = 0;
    unk_43 = 1;
    __ct__CEIBCur((CEIBCur*)&_pad44[0], 0);
    __ct__CEIBPageCur((CEIBPageCur*)&pagecur[0], 0);
    __ct__CCur18(&ccur18[0], 0);
    __ct__CSortMenu(&_padSortMenu[0]);
    __ct__CSysWin(&_padSysWin1[0], 0);
    __ct__CSysWin(&_padSysWin2[0], 0);
    unk_1f4 = 0;
    unk_1f5 = 0;
    unk_1f6 = 0;
    unk_1fa = 0;
    unk_1fc = 0;
    unk_1fe = 0;
    unk_20c = 0;
    unk_372 = 0;
    unk_373 = 0;
    unk_374 = 0;
    unk_375 = 0;
    unk_376 = 0;
    unk_377 = 0;
    unk_378 = 0;
    unk_379 = 0;
    unk_37a = 0;
    unk_37b = 1;
    unk_37c = 0;
    // Initialize the member grid in place ...
    CEquipItemGrid* grid = (CEquipItemGrid*)&_pad37D[1];
    func_802824B4(grid);
    field_27A4 = 0;
    memset(unk_36c, 0, 6);
    {
        // ... then rebuild it from a stack temp via struct assignment.
        CEquipItemGrid tempGrid;
        func_802824B4(&tempGrid);
        *grid = tempGrid;
    }
    {
        // Member sort menu populated from a destroyed stack temp.
        CEquipItemBoxSortMenuBlock tempMenu;
        __ct__CSortMenu(&tempMenu);
        CEquipItemBoxSortMenuBlock* menu = (CEquipItemBoxSortMenuBlock*)&_padSortMenu[0];
        __ct__UnkClass_8011C974(&menu->field_04[0], &tempMenu.field_04[0]);
        menu->field_14 = tempMenu.field_14;
        menu->field_18 = tempMenu.field_18;
        menu->field_1C = tempMenu.field_1C;
        menu->field_20 = tempMenu.field_20;
        menu->field_24 = tempMenu.field_24;
        menu->field_28 = tempMenu.field_28;
        menu->field_29 = tempMenu.field_29;
        menu->field_2A = tempMenu.field_2A;
        menu->field_2B = tempMenu.field_2B;
        __dt__9CSortMenuFv(&tempMenu, -1);
    }
    {
        u8 tempWin[0x44];
        __ct__CSysWin(tempWin, 2);
        func_8016742C(&_padSysWin1[0], tempWin);
        __dt__7CSysWinFv(tempWin, -1);
    }
    {
        u8 tempWin[0x44];
        __ct__CSysWin(tempWin, 0);
        func_8016742C(&_padSysWin2[0], tempWin);
        __dt__7CSysWinFv(tempWin, -1);
    }
    func_8013B2D4();
    func_80157824(2, 0);
    func_80157824(4, 0);
    func_80157824(5, 0);
    func_80157824(6, 0);
    func_80157824(7, 0);
    func_80157824(8, 0);
    func_80157824(3, 0);
}
#pragma pop

// Destroy the embedded sub-objects in reverse construction order (retail
// __dt__13CEquipItemBoxFv): syswins, sort menu, cursors, then the two memory
// regions. MWCC supplies the this-null guard, the member-dtor dispatch and
// the flags>0 operator-delete tail from the dtor shape itself.
CEquipItemBox::~CEquipItemBox() {
#pragma push
#pragma optimize_for_size on
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
    self->field_2C = CDeviceFile::readCommonArchiveFile(func_800A9D90(),
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
    drawLayout(self->field_38, drawInfo, 0, 1);
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
    getEntry__5CBdatFUl(5);
    closeFileHandle__FPP11CFileHandle(&self->field_24);
    closeFileHandle__FPP11CFileHandle(&self->field_28);
    closeFileHandle__FPP11CFileHandle(&self->field_2C);
    self->unk_40 = 0;
    if (self->field_38) {
        delete self->field_38;
        self->field_38 = 0;
    }
    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(self->field_30);
    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(self->field_34);
    deleteRegion__17UnkClass_8045F564Fv(&self->_pad04[0]);
    deleteRegion__17UnkClass_8045F564Fv(&self->_pad04[0x10]);
    func_80285ABC((CEIBCur*)((u8*)self + 0x44));
    func_80285ABC((CEIBCur*)((u8*)self + 0x5c));
    ((CEquipItemBoxSysWinView*)&self->ccur18[0])->v01();
    func_801D3258(self->_padSortMenu);
    func_8022B7F4(&self->_padSysWin1[0]);
    func_8022B7F4(&self->_padSysWin2[0]);
}

// Check the sort menu and both system windows are ready; return unk_42.
extern "C" u8 func_8028652C(CEquipItemBox* self) {
    if (func_801D32DC(self->_padSortMenu) == 0) return 0;
    if (CSysWin_isReady(self->_padSysWin1) == 0) return 0;
    // Inverted form: retail's last check falls through to the unk_42 return
    // and branches to the li 0 block (beq -> li0; fall -> lbz).
    if (CSysWin_isReady(self->_padSysWin2) != 0) return self->unk_42;
    return 0;
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
    playUISound__FUl(0x6d);
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
    playUISound__FUl(0x6);
    nw4r::lyt::Pane* pane =
        field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (pane != 0) {
        func_80124270(pane, 0);
        unk_378 = 0;
    }
}
#pragma pop

extern "C" void func_802867E0(CEquipItemBox* self) {
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) return;

    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) {
        // Syswin1 active: decrement page, wrap, rebuild cursor
        if (CSysWin_isActive(&self->_padSysWin1[0]) == 0) return;
        u8 val = self->unk_374 - 1;
        self->unk_374 = val;
        if ((s8)(u8)val < 0) self->unk_374 = 1;
        func_80289CC0(self);
        playUISound__FUl(1);
        return;
    }

    // Syswin1 not active: check sort menu
    if (func_801D3320(&self->_padSortMenu[0]) != 0) {
        // Sort menu busy: close it and move cursor
        if (func_801D3328(&self->_padSortMenu[0]) == 0) return;
        func_801D3620(&self->_padSortMenu[0]);
        char tmp[0xC];
        func_801D3454(tmp, &self->_padSortMenu[0]);
        ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(tmp);
    } else {
        // Sort menu idle: retail dispatches on unk_375 FIRST -- no sel==-1
        // gate here; that check happens in the caller/dispatch above.
        if (self->unk_375 != 0) {
            // Scan backward for next non-empty page. On a hit retail stores
            // pg back into unk_377 INSIDE the loop, and the cursor lookup
            // below uses the combined index unk_376*4 + unk_377.
            const u16* arr = (const u16*)(self->unk_20c + 0xb0);
            s8 pg = (s8)(self->unk_377 - 1);
            while (pg != (s8)self->unk_377) {
                if (pg < 0) pg = 3;
                if (ArrayGet12(arr, (u8)((s8)self->unk_376 * 4 + pg)) != 0) {
                    self->unk_377 = pg;
                    break;
                }
                pg--;
            }
            nw4r::math::VEC3 tmp;
            func_801CB9D8(&tmp, arr,
                          (u32)((u8)((s8)self->unk_376 * 4 + self->unk_377)));
            ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp);
        } else {
            // Decrement unk_1f5 with wrap
            u8 cur = self->unk_1f5;
            CEquipItemGrid* wrapGrid = (CEquipItemGrid*)((u8*)self + 0x37e);
            u8 val = cur - 1;
            self->unk_1f5 = val;
            if ((s8)(u8)val < -1)
                self->unk_1f5 = ::func_802832B4((CEquipItemBox*)wrapGrid) - 1;
            func_80289CC0(self);
            func_80289AA4(self);
        }
    }
    playUISound__FUl(1);
}

// Down/confirm handler: page the syswin sub-window forward, finish the sort
// menu scroll, advance the name-pane column selection, or move the row
// cursor down (wrapping at 5 with a grid-page step when it wraps).
extern "C" __declspec(noinline) void func_802869B4(CEquipItemBox* self) {
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) return;
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) {
        if (CSysWin_isActive(&self->_padSysWin1[0]) != 0) {
            u8 v = self->unk_374 + 1;
            self->unk_374 = v;
            if ((s8)v > 1) self->unk_374 = 0;
            func_80289CC0(self);
            playUISound__FUl(1);
        }
        return;
    }
    if (func_801D3320(&self->_padSortMenu[0]) != 0) {
        if (func_801D3328(&self->_padSortMenu[0]) != 0) {
            func_801D3698(&self->_padSortMenu[0]);
            nw4r::math::VEC3 tmp;
            func_801D3454(&tmp, &self->_padSortMenu[0]);
            ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp);
            playUISound__FUl(1);
            return;
        }
        // Scroll finished: retail falls through to the name-pane dispatch.
    }
    if (self->unk_375 != 0) {
        // Name pane is up: scan the item list forward for the next column
        // (of 4) whose first entry is non-empty and move the cursor there.
        u16* arr = (u16*)(self->unk_20c + 0xb0);
        s8 i = (s8)(self->unk_377 + 1);
        while (i != (s8)self->unk_377) {
            if ((s8)i >= 4) i = 0;
            if (ArrayGet12(arr,
                            (u8)((s8)self->unk_377 + (s8)self->unk_376 * 4 + i)) != 0) {
                self->unk_377 = (u8)i;
                break;
            }
            i++;
        }
        nw4r::math::VEC3 tmp2;
        func_801CB9D8(&tmp2, arr,
                      (u8)((s8)self->unk_377 + (s8)self->unk_376 * 4));
        ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp2);
        playUISound__FUl(1);
        return;
    }
    u8 v = self->unk_1f5 + 1;
    self->unk_1f5 = v;
    if ((s8)v >= ::func_802832B4((CEquipItemBox*)((u8*)self + 0x37e))) {
        self->unk_1f5 = -1;
    }
    func_80289CC0(self);
    func_80289AA4(self);
    playUISound__FUl(1);
}

// Up/back handler: mirror of func_802869B4 - pages backward through the
// syswin window / sort menu scroll / name-pane columns / row cursor.
extern "C" __declspec(noinline) void func_80286B94(CEquipItemBox* self) {
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) return;
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) return;
    if (func_801D3320(&self->_padSortMenu[0]) != 0) {
        if (func_801D3328(&self->_padSortMenu[0]) != 0) {
            func_801D3724(&self->_padSortMenu[0]);
            nw4r::math::VEC3 tmp;
            func_801D3454(&tmp, &self->_padSortMenu[0]);
            ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp);
            playUISound__FUl(1);
        }
        return;
    }
    if (self->unk_375 != 0) {
        // Name pane is up: scan the item list backward for the previous
        // non-empty column (of 3) and move the cursor there.
        u16* arr = (u16*)(self->unk_20c + 0xb0);
        s8 j = (s8)(self->unk_376 - 1);
        while (j != (s8)self->unk_376) {
            if ((s8)j < 0) j = 2;
            if (ArrayGet12(arr,
                            (u8)(self->unk_377 + j * 4)) != 0) {
                self->unk_376 = (u8)j;
                break;
            }
            j--;
        }
        nw4r::math::VEC3 tmp2;
        func_801CB9D8(&tmp2, arr,
                      (u8)(self->unk_377 + (s8)self->unk_376 * 4));
        ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp2);
        playUISound__FUl(1);
        return;
    }
    if ((s8)self->unk_1f5 == -1) {
        if (self->unk_372 > 1) {
            func_8028A160(self);
            func_80289CC0(self);
        }
        return;
    }
    u8 b = self->unk_1f4;
    if ((s8)b == 0) {
        self->unk_1f4 = 4;
        func_80287024(self);
        func_80289CC0(self);
    } else {
        u8 v = (u8)(b - 1);
        self->unk_1f4 = v;
        if ((s8)v < 0) self->unk_1f4 = 4;
        func_80289CC0(self);
        func_80289AA4(self);
    }
    playUISound__FUl(1);
}

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
        playUISound__FUl(1);
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
        playUISound__FUl(1);
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
    playUISound__FUl(1);
}
#pragma pop

// Advance the equip-grid page cursor when the box is idle; wrap the page
// index at the grid extent and play the page-change sound for multi-page
// lists.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CEquipItemBox::func_80286F6C() {
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
        playUISound__FUl(0xa);
    }
}
#pragma pop

// Step the equip-grid page cursor backward when the box is idle; wrap the
// page index at the grid extent and play the page-change sound for multi-page
// lists (backward mirror of func_80286F6C).
extern "C" __declspec(noinline) void func_80287024(CEquipItemBox* self) {
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
        playUISound__FUl(0xa);
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
        playUISound__FUl(6);
        return;
    }
    if (func_801D3328(&self->_padSortMenu[0]) == 0) return;
    nw4r::math::VEC3 result;
    nw4r::math::VEC3 tmp;
    nw4r::lyt::Pane* root = self->field_38->GetRootPane();
    // Args evaluated right-to-left by MWCC: paneA's lookup runs first and its
    // result stays live across paneB's call (retail saves it to r31).
    func_80137924(&result,
                  root->FindPaneByName(&lbl_eu_8050EFDC[0x18a], true),
                  root->FindPaneByName(&lbl_eu_8050EFDC[0x193], true),
                  root);
    func_801D3430(&self->_padSortMenu[0], &result);
    func_801D353C(&self->_padSortMenu[0], (u8)(self->unk_379 + self->unk_37a));
    self->unk_58 = 0;
    func_801D3454(&tmp, &self->_padSortMenu[0]);
    ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp);
    func_801D216C(&self->ccur18[0], 1);
    func_801D3330(&self->_padSortMenu[0]);
    self->unk_1f6 = 0;
    playUISound__FUl(2);
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
        playUISound__FUl(6);
    }
}
#pragma pop

extern "C" int func_8028CBCC(CEquipItemBox* self);
// Main confirm-button handler (state 2, name pane down). Three top-level
// paths: (1) syswin1 open -> close it, optionally run func_8028CBCC;
// (2) sort menu open -> commit selection, reset cursor state; (3) neither ->
// item move/equip dispatch for the selected cell. Sets unk_1fe when an
// action fired and the page table reports 8 equipped slots.
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
    u8 arg = (param >> 8) & 0xFF;
    u8 b = (param != 0) ? func_801392B4(arg) : 0;
    CEquipItemGrid* grid = (CEquipItemGrid*)&self->_pad37D[1];
    u8 lo = self->unk_1fc & 0xF;
    u8 hi = (self->unk_1fc >> 4) & 0xF;
    switch (lo) {
    case 1:
    case 2:
    case 3:
        hi = 2;
        break;
    case 4: hi = 4; break;
    case 5: hi = 5; break;
    case 6: hi = 6; break;
    case 7: hi = 7; break;
    case 8: hi = 8; break;
    }
    func_80282610(grid, self->unk_36c[(s8)self->unk_373], b, hi);
    func_80289754(self);
}

// Name-pane confirm handler (entry states 2/4): with the name pane visible,
// either page the second system window or jump the item-box selection to the
// column chosen by unk_376/unk_377; otherwise reveal the pane and pick the
// first non-empty list column (column 8 preferred when populated).
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80287FE0(CEquipItemBox* self) {
    if (self->unk_41 != 2 && self->unk_41 != 4) return;
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) return;
    if (func_801D3320(&self->_padSortMenu[0]) != 0) return;
    nw4r::lyt::Pane* pane =
        self->field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (pane == 0) return;
    // Retail dispatches the VISIBLE-pane cases immediately and defers the
    // invisible-pane handler to a tail block after them.
    if (func_801C4648(pane) != 0) {
        if (self->unk_375 != 0) {
            // Name pane up: close the sub-window or commit its selection.
            if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) {
                if (CSysWin_isActive(&self->_padSysWin2[0]) != 0) {
                    func_8022B8E4(&self->_padSysWin2[0]);
                }
                return;
            }
            CEquipItemBoxItemListView* list =
                (CEquipItemBoxItemListView*)(self->unk_20c + 0xb0);
            u8 idx = (u8)(self->unk_377 + self->unk_376 * 4);
            u16 v = ArrayGet12(&list->field_00[0], idx);
            if (v == 0) return;
            void* text;
            if (func_801EF034((const u8*)list, idx) == 3) {
                text = func_801D3C74(list, idx);
            } else {
                text = func_8013639C((const void*)lbl_eu_806649E0,
                                     &lbl_eu_8050EFDC[0x1a2], v);
            }
            func_8022B90C(&self->_padSysWin2[0], 0);
            func_8022B9B4(&self->_padSysWin2[0], text, 0);
            func_8022BFC8(&self->_padSysWin2[0], 1);
            func_8022B8B8(&self->_padSysWin2[0]);
            self->unk_58 = 0;
            return;
        }
        // List-selection refresh: pick the first non-empty column (column 8
        // wins whenever it holds any item), scale the row into a page/column
        // pair via the 0x4330 magic, move the cursor, mark state.
        if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) return;
        CEquipItemBoxItemListView* list =
            (CEquipItemBoxItemListView*)(self->unk_20c + 0xb0);
        int found = 0;
        for (u8 i = 0; i < 0xc; i++) {
            if (list->field_00[i] != 0) {
                list->field_D8 = i;
                if (list->field_00[8] != 0) {
                    list->field_D8 = 8;
                }
                found = 1;
                break;
            }
        }
        if (found == 0) {
            playUISound__FUl(5);
            return;
        }
        u8 sel = list->field_D8;
        s32 pos = (s32)(lbl_eu_80668B24 * (f32)(u32)sel);
        self->unk_58 = 0;
        self->unk_375 = 1;
        self->unk_376 = (u8)pos;
        self->unk_377 = (u8)(sel - (s8)pos * 4);
        func_801D216C(&self->ccur18[0], 1);
        nw4r::math::VEC3 tmp;
        func_801CB9D8(&tmp, &list->field_00[0], sel);
        ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(&tmp);
        self->unk_378 = 1;
        playUISound__FUl(2);
        return;
    }
    // Invisible pane: show it (unless syswin2 is busy), then sound state 2.
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) return;
    func_80124270(pane, 1);
    playUISound__FUl(2);
}
#pragma pop

// Arm-check for the "register/equip item" action: clear the armed flag
// (unk_37c), then run the gate chain - sort menu closed, current cell has an
// item with a kind and a category, category < 9, not the page-2 same-category
// case. When all pass, build the per-page slot table from the item view's
// six slot-id shorts, look up the selected page's entry, resolve its item,
// check its name-table kind is not 1 and that sound 0x3508 is not playing;
// only then set unk_37c = 1. Returns 1 = action blocked, 0 = ok/armed.
#pragma push
#pragma optimize_for_size on
#pragma auto_inline off
extern "C" __declspec(noinline) int func_802882A4(CEquipItemBox* self) {
    self->unk_37c = 0;
    if (func_801D3320(&self->_padSortMenu[0]) != 0) return 1;
    CEquipItemGrid* grid = (CEquipItemGrid*)&self->_pad37D[1];
    u8 idx = (u8)(self->unk_1f4 + self->unk_1f5 * 5);
    if (func_80283208(grid, idx) == 0) return 0;
    if (func_80282EC4(grid, idx) == 0) return 0;
    u8 cat = (u8)((self->unk_1fc >> 8) & 0xff);
    u8 catB = (u8)func_801392B4(cat);
    CEquipItemBoxItemView* cfg = (CEquipItemBoxItemView*)func_8009EC9C(catB);
    if (catB >= 9) return 0;
    u8 page = self->unk_36c[(s8)self->unk_373];
    if (page == 2 && (u8)func_802832D8(grid, idx) == catB) return 0;

    // Per-page slot table: zeroed words at [0],[4],[8],[12], slot ids patched
    // in from the item view.
    s16 table[14] = {0};
    table[2] = cfg->field_26;
    table[4] = cfg->field_1c;
    table[5] = cfg->field_1e;
    table[6] = cfg->field_20;
    table[7] = cfg->field_22;
    table[8] = cfg->field_24;
    // The selected entry; -1 means "no item" (item stays null). Retail
    // passes the selected unk_36c[] byte itself as the category argument.
    s16 elem = table[page];
    void* item = 0;
    if (elem != -1) {
        item = func_80157C4C(page, elem);
    }
    if (item == 0) return 1;
    u32 w = *(u32*)item;
    if (w == 0) return 1;
    u8 v = (u8)func_801361E8(lbl_eu_806640EC, &lbl_eu_8050EFDC[0x20],
                             w >> 20);
    if ((((u32)(s8)(v - 1)) >> 5) == 0) return 1;
    if (func_8009CF8C(0x3508) != 0) return 1;
    self->unk_37c = 1;
    return 0;
}
#pragma pop

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

// us-8028ab5c: guarded teardown -- skips while the sort/syswin sub-objects are
// busy; on success flags unk_1f5 = -1 and runs the close sequence
// (func_80289CC0, func_80289AA4, playUISound(2)).
extern "C" void func_802886D8(CEquipItemBox* self) {
    if ((s8)self->unk_1f5 == -1) return;
    if (self->unk_375 != 0) return;
    if (func_801D3320(&self->_padSortMenu[0]) != 0) return;
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) return;
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) return;
    self->unk_1f5 = -1;
    func_80289CC0(self);
    func_80289AA4(self);
    playUISound__FUl(2);
}

// State probe used by the HUD/button layer: returns a code describing what
// the equip box is currently showing (idle/menu/name-pane combinations plus
// the pane visibility of the confirm label and the getByte gate).
extern "C" __declspec(noinline) int func_8028876C(CEquipItemBox* self) {
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) return 0;
    if (CSysWin_getUnk34(&self->_padSysWin2[0]) != 0) return 0;
    // MWCC lowers both flags via the subfic/srwi / bool-move idioms.
    bool manyParty = code80135FDC_getByte_64077() > 1;
    nw4r::lyt::Pane* pane =
        self->field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    bool vis = func_801C4648(pane);
    if (func_801D3320(&self->_padSortMenu[0]) != 0) return 0x2b;
    if (self->unk_375 != 0) return 0x22;
    if ((s8)self->unk_1f5 == -1) {
        u32 cat = self->unk_36c[(s8)self->unk_373];
        if (cat == 3) {
            if (vis) {
                if (manyParty) return 0x28;
                return 0x2a;
            }
            if (manyParty) return 0x27;
            return 0x29;
        }
        if (vis) {
            if (manyParty) return 0x24;
            return 0x26;
        }
        if (manyParty) return 0x23;
        return 0x25;
    } else {
        u32 cat = self->unk_36c[(s8)self->unk_373];
        // Adjacent (diff-1) constant pairs go through a named local: a direct
        // "if (c) return K; return K+1;" gets lowered to branchless
        // addic/subfe select which retail does not do.
        int ret;
        if (cat == 3) {
            ret = 0x1c;
            if (vis) ret = 0x1d;
            return ret;
        }
        if (cat == 2) {
            if (vis) {
                ret = 0x21;
                if (manyParty) ret = 0x20;
                return ret;
            }
            // Retail bytes: many->0x1e, !many->0x1f (the old source had
            // this pair reversed).
            ret = 0x1f;
            if (manyParty) ret = 0x1e;
            return ret;
        }
        if (vis) {
            ret = 0x1b;
            if (manyParty) ret = 0x1a;
        } else {
            // Retail bytes: many->0x18, !many->0x19.
            ret = 0x19;
            if (manyParty) ret = 0x18;
        }
        return ret;
    }
}

#pragma push
#pragma optimize_for_size on
// Returns 1 when the lookup byte of the grid cell selected by (unk_1f5) equals
// func_801392B4's category byte; an unk_1f5 of -1 always returns 0.
extern "C" int func_80288948(CEquipItemBox* self) {
    if ((s8)self->unk_1f5 == -1) return 0;
    u32 b;
    int idx;
    u8 cat;
    u32 a;
    idx = self->unk_1f4 + self->unk_1f5 * 5;
    cat = (u8)((self->unk_1fc >> 8) & 0xff);
    a = func_802832D8((CEquipItemGrid*)((u8*)self + 0x37E), (u8)idx);
    b = func_801392B4(cat);
    return a == b;
}
#pragma pop

// Play the entry animation at 0x3C; when it completes mark the cursors active.
extern "C" __declspec(noinline) void func_802889C0(CEquipItemBox* self) {
    float f = lbl_eu_80668B28;
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->field_3C, f) != 0) {
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

// If both pointer fields 0x34 and 0x38 are non-null, clear the sort menu
// (passing self) and set unk_42. noinline: retail callers (OnFileEvent)
// emit a `bl` to the unmangled retail name.
extern "C" __declspec(noinline) void func_80288AC0(CEquipItemBox* self) {
    if (self->field_38 == 0) return;
    if (!(self->field_34 == 0)) {
        func_80139198(0);
        self->unk_42 = 1;
    }
}

// Per-cell texture bind: kind-3 items resolve an id-keyed icon first, then
// the generic kind lookup; the b7 flag finally overrides with one of two
// fixed textures selected by the current sort page.
extern "C" __declspec(noinline) void func_80288B14(CEquipItemBox* self, u16 kind, void* itemPtr,
                                                   u8 i, u8 b7) {
    char buf[0x20];
    // Retail reuses one register for the icon name and the bound resource.
    void* res = 0;
    if (itemPtr != 0 && ((((CItemInstance*)itemPtr)->word >> 12) & 0xF) == 3) {
        u32 id = (u32)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances((CItemInstance*)itemPtr))
                     ->vf08((CItemInstance*)itemPtr);
        switch ((u8)func_801361E8(lbl_eu_806640D8, &lbl_eu_8050EFDC[0x1a7], (u16)id)) {
        case 4: res = func_80138F78(0x144); break;
        case 5: res = func_80138F78(0x145); break;
        case 6: res = func_80138F78(0x146); break;
        case 7: res = func_80138F78(0x147); break;
        case 8: res = func_80138F78(0x148); break;
        case 9: res = func_80138F78(0x149); break;
        }
        if (res != 0) {
            res = self->field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, (char*)res, 0);
        } else {
            res = self->field_30->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1b0], 0);
        }
    }
    if (res == 0) {
        if (kind != 0) {
            res = func_80138F78(
                func_80136254((const void*)lbl_eu_806640EC, &lbl_eu_8050EFDC[0x1c3], kind));
            res = self->field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, (char*)res, 0);
            if (res == 0) {
                res = self->field_30->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_8050EFDC[0x1b0], 0);
            }
        } else {
            res = self->field_30->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1b0], 0);
        }
    }
    if (b7 != 0) {
        // b7 override: page 3 uses the alternate texture unconditionally.
        if (self->unk_36c[(s8)self->unk_373] == 3) {
            res = self->field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1cd], 0);
        } else {
            res = self->field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1e3], 0);
        }
    }
    if (res != 0) {
        sprintf(buf, &lbl_eu_8050EFDC[0x1f9], i + 1);
        func_80137E7C(self->field_38, buf, (u32)res);
    }
}

// Item-kind refresh helper: format the textbox pane name for cell i, then
// build its text - items of kind 3/9 append a per-slot count suffix, positive
// cell states show the carry-count string, negative states show error text.
// bufName lands at sp+0x28 and bufText at sp+0x8 in retail (declaration order).
extern "C" __declspec(noinline) void func_802891B8(CEquipItemBox* self, s8 v,
                                                   CItemInstance* item, u8 i) {
    char bufName[0x20];
    char bufText[0x20];
    // Retail evaluates the null check before any buffer setup, but defers
    // the item->word load until after the name sprintf.
    CItemInstance* p = (item != 0) ? item : 0;
    sprintf(bufName, &lbl_eu_8050EFDC[0x219], i + 1);
    u32 kind = (p != 0) ? ((p->word >> 16) & 0xF) : 0;
    if (kind == 3 || kind == 9) {
        CItemInstance* obj = (CItemInstance*)CItem_initItemImplInstances(item);
        u32 n = ((CEquipItemBoxItemImplView*)obj)->vf08(obj);
        char* str = func_80136190(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36],
                                  0x1e - ((u8)n - 1));
        sprintf(bufText, &lbl_eu_8050EFDC[0x2a], str);
        func_80139A18(self->field_38, bufName, lbl_eu_806649B0, lbl_eu_806649B8);
    } else if (v > 0) {
        sprintf(bufText, &lbl_eu_8050EFDC[0x225]);
        func_80139A18(self->field_38, bufName, lbl_eu_806649B0, lbl_eu_806649B8);
    } else if (v == 0) {
        sprintf(bufText, &lbl_eu_8050EFDC[0x228]);
    } else if (v == -2) {
        sprintf(bufText, func_eu_802B148C());
        func_80139A18(self->field_38, bufName, lbl_eu_806649C0, lbl_eu_806649C8);
    } else if (v == -3) {
        sprintf(bufText, func_eu_802B1474());
        func_80139A18(self->field_38, bufName, lbl_eu_806649D0, lbl_eu_806649D8);
    } else {
        sprintf(bufText, &lbl_eu_8050EFDC[0x228]);
    }
    func_80136A1C(self->field_38, bufName, bufText, 0);
}

extern "C" __declspec(noinline) void func_80288E14(CEquipItemBox* self, u16 kind, void* item, u8 i) {
    // Pick the icon texture for the grid cell: item-kind-specific icons first
    // (vf08 select), falling back to the generic name lookup by kind.
    void* obj = item != 0 ? item : 0;
    void* res = 0;
    if (obj != 0) {
        if (((((CItemInstance*)obj)->word >> 16) & 0xF) == 3) {
            char* name = 0;
            u16 id =
                (u16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf08((CItemInstance*)obj);
            switch (id) {
            case 1: name = func_80138F78(0x197); break;
            case 2: name = func_80138F78(0x196); break;
            case 3: name = func_80138F78(0x195); break;
            case 4: name = func_80138F78(0x194); break;
            case 5: name = func_80138F78(0x193); break;
            case 6: name = func_80138F78(0x192); break;
            }
            if (name != 0) {
                res = self->field_34->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, name, 0);
            } else {
                res = self->field_30->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_8050EFDC[0x1b0], 0);
            }
        } else {
            int check = func_801C6E90(obj);
            if (check == 0) {
                int flag = 0;
                if (((((CItemInstance*)obj)->word >> 16) & 0xF) == 9 &&
                    (((u32)((CItemInstance*)obj)->unk7) & 3) == 1) {
                    flag = 1;
                }
                if (flag == 0) {
                    goto after_switch;
                }
            }
            {
                char* name = 0;
                u16 id =
                    (u16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf08((CItemInstance*)obj);
                switch (id) {
                case 1: name = func_80138F78(0x191); break;
                case 2: name = func_80138F78(0x190); break;
                case 3: name = func_80138F78(0x18f); break;
                case 4: name = func_80138F78(0x18e); break;
                case 5: name = func_80138F78(0x18d); break;
                }
                if (name != 0) {
                    res = self->field_34->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, name, 0);
                } else {
                    res = self->field_30->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                        &lbl_eu_8050EFDC[0x1b0], 0);
                }
            }
        after_switch:;
        }
    }
    if (res == 0) {
        if (kind != 0) {
            char* name = func_80138F78((u16)func_80136254((const void*)lbl_eu_806640EC, &lbl_eu_8050EFDC[0x207], kind));
            res = self->field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, name, 0);
            if (res == 0) {
                res = self->field_30->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_8050EFDC[0x1b0], 0);
            }
        } else {
            res = self->field_30->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                &lbl_eu_8050EFDC[0x1b0], 0);
        }
    }
    if (res != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050EFDC[0x20c], i + 1);
        func_80137E7C(self->field_38, buf, (u32)res);
    }
}

// Equip-box flavour handler: (re)bind the item texture named by the current
// page/state into the layout, set its visibility pane, and refresh the
// name-pane text. mode 1 picks a per-flag texture, mode 2 a fixed one;
// otherwise the default page texture is used.
extern "C" __declspec(noinline) void func_8028939C(CEquipItemBox* self, u32 mode, int num, int flag, u32 show) {
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
// Rebuild the equip item box for a page change: map the selection window kind
// from unk_1fc's low nibble, dispatch the grid rebuild, and when the caller
// asks (a != 0) and a target item is pending (field_27A4), scan the 5x6 grid
// for that item (advancing the page cursor and rescanning when absent), then
// refresh the list, layout text and item-box selection.
#pragma push
#pragma optimize_for_size on
// noinline: retail callers (func_8028A0E0, func_8028A1DC etc.) emit a `bl`.
extern "C" __declspec(noinline) void func_80289500(CEquipItemBox* self, int a) {
    CEquipItemGrid* grid = (CEquipItemGrid*)&self->_pad37D[1];
    u8 page = self->unk_36c[(s8)self->unk_373];
    u8 b;
    u8 cat = (u8)((self->unk_1fc >> 8) & 0xFF);
    if (self->unk_1fc != 0) b = (u8)func_801392B4(cat);
    else b = 0;
    u8 lo = (u8)(self->unk_1fc & 0xF);
    u8 hi = (u8)((self->unk_1fc >> 4) & 0xF);
    switch (lo) {
    case 1:
    case 2:
    case 3:
        hi = 2;
        break;
    case 4: hi = 4; break;
    case 5: hi = 5; break;
    case 6: hi = 6; break;
    case 7: hi = 7; break;
    case 8: hi = 8; break;
    }
    func_80282610(grid, page, b, hi);
    if (a != 0 && self->field_27A4 != 0) {
        grid->idx = 0;
        int found = 0;
        self->unk_1f4 = 0;
        self->unk_1f5 = 0;
        do {
            for (u8 r = 0; r < 5; r++) {
                for (u8 c = 0; c < 6; c++) {
                    void* item = func_80282F34(grid, (u8)(r + c * 5));
                    if ((u32)self->field_27A4 == (u32)item) {
                        self->unk_1f4 = r;
                        found = 1;
                        self->unk_1f5 = c;
                        break;
                    }
                }
                if (found) break;
            }
            if (found) break;
            u8 v = grid->_pad2003;
            u8 next = (u8)(grid->idx + 1);
            u8 limit = (v != 0) ? v : 1;
            if (next >= limit) break;
            func_80282DF8((CEquipItemBox*)grid);
        } while (1);
        self->field_27A4 = 0;
    }
    u8 idx2 = (u8)(self->unk_1f4 + self->unk_1f5 * 5);
    void* item2 = func_80282F34(grid, idx2);
    func_801D47D4((CItemBoxInfo*)self->unk_20c, self->unk_1fc, item2, 1);
    func_801D4AE0((CItemBoxInfo*)self->unk_20c, 1, func_80283350(grid, idx2));
    u32 x = func_80282D60(grid);
    setLayoutTextBoxNumber(self->field_38, &lbl_eu_8050EFDC[0x27c], (u8)x);
    func_80289754(self);
    func_80289AA4(self);
    func_80289E70(self);
    if (getItemBoxState__FP12CItemBoxInfo((void*)self->unk_20c) != 0) {
        func_801D4260((CItemBoxInfo*)self->unk_20c, page);
    }
}
#pragma pop

// Refresh the equip-box page display: for multi-page grids bind the page
// cursor name panes (10 per page) with the first `pages` visible, move the
// cursor pane next to the current row, then refresh the 6 column panes and
// dispatch the per-cell refresh helpers for the whole 5x6 grid.
#pragma push
#pragma auto_inline off
void CEquipItemBox::func_80289754() {
    u32 vec[3];
    char buf2[0x20];
    char buf1[0x20];
    u8 b = func_801392B4((unk_1fc >> 8) & 0xFF);
    CEquipItemGrid* grid = (CEquipItemGrid*)&_pad37D[1];
    u8 pages = func_80282D60(grid);
    if (pages > 1) {
        nw4r::lyt::Pane* pane =
            field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x285], true);
        func_80124270(pane, 1);
        for (u8 i = 0; i < 10; i++) {
            sprintf(buf1, &lbl_eu_8050EFDC[0x28c], i + 1);
            nw4r::lyt::Pane* p = field_38->GetRootPane()->FindPaneByName(buf1, true);
            func_80124270(p, (u8)i < pages);
        }
        setLayoutTextBoxNumber(field_38, &lbl_eu_8050EFDC[0x29b], (u8)((u8)grid->idx + 1));
        nw4r::lyt::Pane* cur =
            field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x2a4], true);
        // Move the cursor pane: start from the base position at +0x200 and
        // shift x by scale * (row + 10 - pages). The (f32) casts make MWCC
        // emit its own s32->f32 magic (pool consts B18/B10 are compiler-
        // generated; do NOT hand-roll the 0x4330 union here).
        vec[0] = ((CEquipItemBoxPagePosView*)this)->pos[0];
        vec[1] = ((CEquipItemBoxPagePosView*)this)->pos[1];
        vec[2] = ((CEquipItemBoxPagePosView*)this)->pos[2];
        float fp = (f32)(u32)(u8)(10 - pages);
        float fi = (f32)(int)((u8)((u8)grid->idx + 1) - 1);
        ((float*)vec)[0] += lbl_eu_80668B44 * fi + lbl_eu_80668B44 * fp;
        copyVEC3(&((CEquipItemBoxPaneView*)cur)->mTranslate[0], (const float*)vec);
    } else {
        nw4r::lyt::Pane* pane =
            field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x285], true);
        func_80124270(pane, 0);
    }
    for (u8 i = 0; i < 6; i++) {
        sprintf(buf2, &lbl_eu_8050EFDC[0x2b2], i + 1);
        nw4r::lyt::Pane* p = field_38->GetRootPane()->FindPaneByName(buf2, true);
        if (p != 0) {
            func_80124270(p, (u8)i < ::func_802832B4((CEquipItemBox*)grid));
        }
    }
    for (u8 i = 0; i < 30; i++) {
        u32 kind = func_80282EC4(grid, i);
        void* item = func_80282F34(grid, i);
        u8 b7 = func_80283280(grid, i);
        func_80288B14(this, (u16)kind, item, i, b7);
        func_80288E14(this, (u16)kind, item, i);
        func_802891B8(this, (s8)func_80282FA0(grid, i), (CItemInstance*)item, i);
        u32 show;
        if ((u16)kind != 0) {
            show = func_80283208(grid, i);
        } else {
            show = 1;
        }
        u8 b2 = func_802832D8(grid, i);
        u8 b3 = func_80283118(grid, i);
        func_8028939C(this, b3, i, (u32)(b2 == b), show);
    }
#pragma pop
}

// Refresh the equip item box list: bind the current selection's name into the
// layout, re-dispatch the interaction for the selected grid cell, then copy
// the per-page item ids/records into the item-box info list (+0xB0 region).
// noinline: retail callers (func_80289500 etc.) emit a `bl`.
__declspec(noinline) void CEquipItemBox::func_80289AA4() {
    u8 idx = (u8)(unk_1f4 + unk_1f5 * 5);
    CEquipItemGrid* grid = (CEquipItemGrid*)&_pad37D[1];
    if ((s8)unk_1f5 == -1) {
        func_80136B4C(field_38, &lbl_eu_8050EFDC[0x2bf], ::func_8028D0EC(this), 0);
    } else {
        func_80136B4C(field_38, &lbl_eu_8050EFDC[0x2bf], func_80283350(grid, idx), 0);
    }
    if (getItemBoxState__FP12CItemBoxInfo((void*)unk_20c) == 0) return;
    void* item;
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

// Rebuild the cursor target after a page/item change. With no active syswin
// window the cursor receives the current window name directly; otherwise a
// page-/item-name string is formatted, its pane position is read and scaled
// by the anchor pane's scale, and the result is written into the anim target
// hanging off self+0x4C (sub-view +0x2C slot). The tail is duplicated per
#pragma push
#pragma optimize_for_size on
// branch in retail (no shared helper), so keep both copies textual.
extern "C" __declspec(noinline) void func_80289CC0(CEquipItemBox* self) {
    u8 winName[0x18];
    // Retail allocates SEPARATE stack copies per branch (pageName sp+80,
    // itemName sp+48, and two distinct 'pos' slots) because none of them
    // live across the branch join. Keep declarations branch-local.
    // Polarity per retail: menu OPEN (!= 0) takes the win-name path.
    if (CSysWin_getUnk34(&self->_padSysWin1[0]) != 0) {
        func_8022C1B4(winName, &self->_padSysWin1[0], self->unk_374);
        ((CEquipItemBoxCur18View*)&self->ccur18[0])->vf04(winName);
    } else {
        s8 sel = self->unk_1f5;
        if (sel == -1) {
            char pageName[0x28];
            nw4r::math::VEC3 pos;
            sprintf(pageName, &lbl_eu_8050EFDC[0x2C8], (s8)self->unk_373 + 1);
            nw4r::lyt::Pane* pane =
                ((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                    ->v13((u32)pageName, 1);
            func_801375A0(&pos, pane);
            pane = ((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                       ->v13((u32)&lbl_eu_8050EFDC[0x193], 1);
            pos.x *= pane->GetScale().x;
            copyVEC3((float*)(*(void**)((char*)(*(void**)((char*)self + 0x4C)) + 0x10)) + 11,
                     &pos);
        } else {
            char itemName[0x20];
            nw4r::math::VEC3 pos;
            sprintf(itemName, &lbl_eu_8050EFDC[0x20C], (s8)self->unk_1f4 + sel * 5 + 1);
            nw4r::lyt::Pane* pane =
                ((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                    ->v13((u32)itemName, 1);
            func_801375A0(&pos, pane);
            pane = ((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                       ->v13((u32)&lbl_eu_8050EFDC[0x193], 1);
            pos.x *= pane->GetScale().x;
            copyVEC3((float*)(*(void**)((char*)(*(void**)((char*)self + 0x4C)) + 0x10)) + 11,
                     &pos);
        }
    }
}
#pragma pop

// Rebuild the sort-menu page-list contents for the current page (unk_36c
// entry selected by unk_373): feed the page value to the sort menu, fill a
// per-page sound-key table (7/12/10 entries for pages 2/3/4..8), bind the
// page sound name into the layout, then re-add every entry whose key is
// positive and refresh the page-list cursor position.
#pragma push
#pragma optimize_for_size on
// noinline: retail callers (func_80289500 etc.) emit a `bl`.
extern "C" __declspec(noinline) void func_80289E70(CEquipItemBox* self) {
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
extern "C" __declspec(noinline) void func_8028A0E0(CEquipItemBox* self) {
    if (func_801D3320(self->_padSortMenu) != 0) return;
    u8 v = self->unk_373 + 1;
    self->unk_373 = v;
    if ((int)(s8)v >= self->unk_372) {
        self->unk_373 = 0;
    }
    func_8028A5D8(self, 0);
    func_80289500(self, 0);
    playUISound__FUl(0x70);
}
#pragma pop

// Step the sort-menu page selection forwards (wrapping back to page 0 after the
// last page) with the current page stored in unk_373.
extern "C" __declspec(noinline) void func_8028A160(CEquipItemBox* self) {
    if (func_801D3320(self->_padSortMenu) != 0) return;
    u8 v = self->unk_373 - 1;
    self->unk_373 = v;
    if ((s8)v < 0) {
        self->unk_373 = self->unk_372 - 1;
    }
    func_8028A5D8(self, 1);
    func_80289500(self, 0);
    playUISound__FUl(0x70);
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
            if ((u32)(v - 4) <= 4 || (int)v == 2 || (int)v == 0xb) {
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

// Per-page texture bind: build the page's texture name from the per-kind
// format-string table, fetch the 'timg' resource via the layout's arc
// accessor (falling back to the shared accessor's default name), then bind
// it to the pane named by the page index.
extern "C" __declspec(noinline) void func_8028A374(CEquipItemBox* self, u8 v, u8 i) {
    char texName[0x20];
    char paneName[0x10];
    nw4r::lyt::Pane* pane;
    // Single dense switch (cases 0..12) so MWCC emits the retail range-check
    // + jumptable dispatch; case 0 jumps straight to the bind.
    switch (v) {
    case 0:
        pane = (nw4r::lyt::Pane*)self->field_30->GetResource(0x74696D67, &lbl_eu_8050EFDC[0x1B0], 0);
        goto bind;
    case 1: sprintf(texName, &lbl_eu_8050EFDC[0x2FD]); goto fetch;
    case 2: sprintf(texName, &lbl_eu_8050EFDC[0x312]); goto fetch;
    case 3: sprintf(texName, &lbl_eu_8050EFDC[0x327]); goto fetch;
    case 4: sprintf(texName, &lbl_eu_8050EFDC[0x33C]); goto fetch;
    case 5: sprintf(texName, &lbl_eu_8050EFDC[0x351]); goto fetch;
    case 6: sprintf(texName, &lbl_eu_8050EFDC[0x366]); goto fetch;
    case 7: sprintf(texName, &lbl_eu_8050EFDC[0x37B]); goto fetch;
    case 8: sprintf(texName, &lbl_eu_8050EFDC[0x390]); goto fetch;
    case 9: sprintf(texName, &lbl_eu_8050EFDC[0x3A5]); goto fetch;
    case 10: sprintf(texName, &lbl_eu_8050EFDC[0x3BA]); goto fetch;
    case 11: sprintf(texName, &lbl_eu_8050EFDC[0x3CF]); goto fetch;
    case 12: sprintf(texName, &lbl_eu_8050EFDC[0x3E4]); goto fetch;
    default:
    fetch:
        pane = (nw4r::lyt::Pane*)self->field_34->GetResource(0x74696D67, texName, 0);
        if (pane == 0) {
            pane = (nw4r::lyt::Pane*)self->field_30->GetResource(0x74696D67, &lbl_eu_8050EFDC[0x1B0], 0);
        }
    bind:;
    }
    if (pane != 0) {
        sprintf(paneName, &lbl_eu_8050EFDC[0x2C8], i + 1);
        func_80137E7C(self->field_38, paneName, (u32)pane);
    }
}
#pragma pop

#pragma push
#pragma auto_inline off
// Rebuild the sort-page selection display: step the page cursor (next/prev
// page around unk_373, clamped to the page range), toggle the two pane names
// per page slot, then recolour the 15 page-name panes (highlighted pair for
// pages 2/4..8/0xB, idle pair otherwise).
extern "C" void func_8028A5D8(CEquipItemBox* self, int a) {
    u8 buf[2];
    buf[0] = self->unk_373;
    s8 v = (s8)self->unk_373;
    buf[1] = (u8)((a != 0) ? v + 1 : v - 1);
    if ((s8)buf[1] < 0) {
        buf[1] = (u8)(self->unk_372 - 1);
    } else if ((s8)buf[1] >= self->unk_372) {
        buf[1] = 0;
    }
    u8 flagA = 0;
    u8 flagB = 0;
    u8 i;
    for (i = 0; i < 2; i++) {
        s8 b = (s8)buf[i];
        char name1[0x20];
        char name2[0x20];
        s8 p = b + 1;
        sprintf(name1, &lbl_eu_8050EFDC[0x2df], p);
        sprintf(name2, &lbl_eu_8050EFDC[0x2ee], p);
        flagA = 0;
        flagB = 0;
        if (self->unk_36c[b] != 0) {
            if (b == (s8)self->unk_373) {
                flagA = 1;
                flagB = 0;
            } else {
                flagA = 0;
                flagB = 1;
            }
        }
        nw4r::lyt::Pane* pane = self->field_38->GetRootPane()->FindPaneByName(name1, true);
        func_80124270(pane, flagA);
        pane = self->field_38->GetRootPane()->FindPaneByName(name2, true);
        func_80124270(pane, flagB);
    }
    u8 v2 = self->unk_36c[(s8)self->unk_373];
    if ((u32)(v2 - 4) <= 4 || v2 == 2 || v2 == 0xb) {
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x3f9], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x405], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x411], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x41d], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x429], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x435], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x441], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x44d], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x459], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x465], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x470], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x47b], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x486], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x491], lbl_eu_80664940, lbl_eu_80664948);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x49c], lbl_eu_80664940, lbl_eu_80664948);
    } else {
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x3f9], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x405], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x411], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x41d], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x429], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x435], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x441], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x44d], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x459], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x465], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x470], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x47b], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x486], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x491], lbl_eu_80664930, lbl_eu_80664938);
        func_80139A18(self->field_38, &lbl_eu_8050EFDC[0x49c], lbl_eu_80664930, lbl_eu_80664938);
    }
}


// Dispatch an equip-box interaction by the window kind byte: kinds 2/3 and
// the 4..8 band go to their dedicated handlers, anything else is ignored.
// noinline: retail callers emit a `bl` (same-TU dispatch stays out of line).
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
#pragma function_align 4
extern "C" __declspec(noinline) void func_8028A9CC(CEquipItemBox* self, int a, int b) {
    func_8028AA64(self);
    int v = (u8)func_801392E4(a);
    switch (v) {
    case 2:
        func_8028AF98(self, a, b);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        func_8028B7CC(self, a, b);
        break;
    case 3:
        func_8028BE74(self, a, b);
        break;
    }
}
#pragma pop

// Reset the per-page item state: clear the 12 item-id shorts at +0x210 and
// zero the 12 page-position VEC3 records at +0x228, then hide every one of
// the 30 equip-item panes in the layout (string base hoisted into a saved
// register like retail's r31). optimize_for_size selects retail's
// _savegpr_29 prologue for this 3-saved-reg frame.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_8028AA64(CEquipItemBox* self) {
    CEquipItemBoxPageStateView* view = (CEquipItemBoxPageStateView*)self;
    for (u8 i = 0; i < 0xc; i++) {
        view->field_210[i] = 0;
        float vec[3];
        float* p = (float*)code80135FDC_setVec3(vec, lbl_eu_80668B04, lbl_eu_80668B04, lbl_eu_80668B04);
        copyVEC3(view->field_228[i], p);
    }
    const char* base = lbl_eu_8050EFDC;
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4a7, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4b3, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4bf, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4ca, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4d5, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4e0, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4eb, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4f6, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x501, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x50c, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x517, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x522, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x52e, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x53a, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x546, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x552, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x55e, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x56a, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x577, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x584, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x591, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x59b, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5a5, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5af, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5be, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5cd, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5dc, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5e7, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5f2, true), 0);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5fe, true), 0);
}
#pragma pop

// Kind-2 (equipment) rebuild handler: show the 24 item panes, read the item's
// stat/name tables (bdat lookups keyed by the kind word), scale the two
// drop-rate counters by the RNG when the flag word says so, then bind the
// computed texts (0x522/0x546/0x552/0x52e/0x53a), re-tint the stat panes and
// hand the box to the per-cell grid rebuild (func_8028C280). optimize_for_size
// selects retail's _savegpr prologue for this 13-saved-reg frame.
#pragma push
#pragma optimize_for_size on
extern "C" void func_8028AF98(CEquipItemBox* self, int a, int b) {
    // Declaration order follows retail's saved-register allocation
    // (self=r19, a=r20, b=r21, rateB=r22, rateA=r23, g=r24, base=r25,
    // kind2=r26, name72=r27, name7a=r28, name61=r29, hi=r30, name69=r31).
    // Int-typed (retail defers narrow-masking to use sites; rates are
    // small table ids so no value ever exceeds u16 range).
    int rateB;
    int rateA;
    u32 g;
    char* base;
    u16 kind2;
    u32 name72;
    u32 name7a;
    u32 name61;
    u32 hi;
    u32 name69;
    char buf[0x20];
    char nb[0x28];
    base = lbl_eu_8050EFDC;
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4a7, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4bf, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4ca, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4d5, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4e0, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4eb, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4f6, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x501, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x50c, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x517, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x522, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x52e, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x53a, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x546, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x552, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x56a, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x577, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x584, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x591, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x59b, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5a5, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5af, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5be, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5cd, true), 1);
    g = lbl_eu_806640F4;
    func_801392E4(a);
    kind2 = func_80139358(a);
    rateA = func_80136254((const void*)g, base + 0x60a, kind2);
    rateB = func_80136254((const void*)g, base + 0x8b, kind2);
    name72 = func_801361E8(g, base + 0x72, kind2);
    name7a = func_801361E8(g, base + 0x7a, kind2);
    name61 = func_801361E8(g, base + 0x61, kind2);
    // The three int->float conversions use MWCC's own s32->f32 magic
    // ((f32) casts emit lis-0x4330/xoris/lfd/fsubs inline, sharing one
    // constant copy in r30 like retail); do NOT hand-roll unions here.
    float f = (f32)(u32)(u8)func_801361E8(g, base + 0x612, kind2) / lbl_eu_80668B48;
    name69 = func_801361E8(g, base + 0x69, kind2);
    u32 flag92 = func_801361E8(g, base + 0x92, kind2);
    if (flag92 & 4) {
        void* obj = func_8009EC9C(1);
        rateA = (int)(lbl_eu_80668B4C * (f32)(int)((u32)(u16)rateA * (u32)func_800A082C(obj)));
        rateB = (int)(lbl_eu_80668B2C * (f32)(int)((u32)(u16)rateB * (u32)func_800A082C(obj)));
        if ((u32)(u16)rateA >= 0x3e7) rateA = 0x3e7;
        if ((u32)(u16)rateB >= 0x3e7) rateB = 0x3e7;
    }
    for (u8 i = 1; i <= 10; i++) {
        sprintf(nb, base + 0x7, i);
        if ((u8)func_801361E8(g, nb, kind2)) {
            f += func_8013B380(i);
            break;
        }
    }
    func_80139C98((u32)rateA, (u32)rateB, 0, f);
    char* str = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0xb);
    sprintf(buf, base + 0x618, rateA, str, rateB);
    func_80136A1C(self->field_38, base + 0x522, buf, 0);
    setLayoutTextBoxNumber(self->field_38, base + 0x546, (u8)name72);
    setLayoutTextBoxNumber(self->field_38, base + 0x552, (u8)name7a);
    char* n1 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x7f);
    char* n2 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x80);
    if ((u8)name61) {
        sprintf(buf, base + 0x61f, n1, n2);
    } else {
        sprintf(buf, base + 0x626, (u8)name61, n2);
    }
    func_80136A1C(self->field_38, base + 0x52e, buf, 0);
    n1 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x7f);
    n2 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x80);
    if ((u8)name69) {
        sprintf(buf, base + 0x61f, n1, n2);
    } else {
        sprintf(buf, base + 0x626, (u8)name69, n2);
    }
    func_80136A1C(self->field_38, base + 0x53a, buf, 0);
    func_80136B4C(self->field_38, base + 0x55e, base + 0x228, 0);
    func_80136B4C(self->field_38, base + 0x50c, base + 0x228, 0);
    func_80136B4C(self->field_38, base + 0x517, base + 0x228, 0);
    func_80139A18(self->field_38, base + 0x4bf, lbl_eu_80664970, lbl_eu_80664978);
    func_80139A18(self->field_38, base + 0x4e0, lbl_eu_80664970, lbl_eu_80664978);
    func_80139A18(self->field_38, base + 0x4eb, lbl_eu_80664970, lbl_eu_80664978);
    func_80139A18(self->field_38, base + 0x4ca, lbl_eu_80664970, lbl_eu_80664978);
    func_80139A18(self->field_38, base + 0x4d5, lbl_eu_80664970, lbl_eu_80664978);
    func_80139A18(self->field_38, base + 0x4f6, lbl_eu_80664980, lbl_eu_80664988);
    func_80139A18(self->field_38, base + 0x501, lbl_eu_80664980, lbl_eu_80664988);
    func_8028C280(self, a, b);
}
#pragma pop

// Rebuild the equip-box panes for a window kind in the 4..8 band: show the 22
// item cells, bind the 3 name texts (0x72/0x7a/0x82 bdat lookups), pick the
// sort-menu sound by the name byte, recolour the 7 stat panes, then hand the
// box to the kind-specific helper. optimize_for_size selects retail's stmw
// r24 prologue for this 8-saved-reg frame.
#pragma push
#pragma optimize_for_size on
extern "C" void func_8028B7CC(CEquipItemBox* self, int kind, int item) {
    char* base = lbl_eu_8050EFDC;
    u16 kind2;
    u32 nameA, nameB, nameC;
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4a7, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4bf, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4ca, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4d5, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4e0, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4eb, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x4f6, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x501, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x50c, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x517, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x546, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x552, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x55e, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x56a, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x577, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x584, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x591, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x59b, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5a5, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5af, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5be, true), 1);
    func_80124270(self->field_38->GetRootPane()->FindPaneByName(base + 0x5cd, true), 1);
    // Loaded into a register only here (retail keeps it live across the
    // bdat-name lookups below).
    u32 g = lbl_eu_806640F8;
    func_801392E4(kind);
    kind2 = func_80139358(kind);
    nameA = func_801361E8(g, base + 0x72, kind2);
    nameB = func_801361E8(g, base + 0x7a, kind2);
    nameC = func_801361E8(g, base + 0x82, kind2);
    setLayoutTextBoxNumber(self->field_38, base + 0x546, (u8)nameA);
    setLayoutTextBoxNumber(self->field_38, base + 0x552, (u8)nameB);
    setLayoutTextBoxNumber(self->field_38, base + 0x55e, (u8)nameC);
    // Retail reloads lbl_eu_806640F8 here: the three setLayoutTextBoxNumber calls
    // above are opaque, so the global must not be cached across them.
    u32 v = func_801361E8(lbl_eu_806640F8, base + 0x17, func_80139358(kind));
    u8 b = (u8)v;
    char* str;
    if ((u32)(b - 4) <= 9) {
        str = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x2e);
    } else if (b == 3) {
        str = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x32);
    } else if (b == 2) {
        str = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x31);
    } else if (b == 1) {
        str = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x30);
    } else {
        str = 0;
    }
    func_80136B4C(self->field_38, base + 0x50c, str, 0);
    u8 b2 = (u8)v;
    char* str2 = 0;
    if ((u32)(b2 - 4) <= 8) {
        switch (b2) {
        case 4: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x77); break;
        case 5: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x78); break;
        case 6: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x79); break;
        case 7: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x7a); break;
        case 8: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x7b); break;
        case 9: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x7c); break;
        case 10: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x7d); break;
        case 11: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x7e); break;
        case 12: str2 = func_80136190((char*)base + 0x62b, base + 0x36, 0x7f); break;
        }
    }
    func_80136B4C(self->field_38, base + 0x517, str2, 0);
    func_80139A18(self->field_38, base + 0x4bf, lbl_eu_80664980, lbl_eu_80664988);
    func_80139A18(self->field_38, base + 0x4e0, lbl_eu_80664970, lbl_eu_80664978);
    func_80139A18(self->field_38, base + 0x4eb, lbl_eu_80664970, lbl_eu_80664978);
    func_80139A18(self->field_38, base + 0x4ca, lbl_eu_80664980, lbl_eu_80664988);
    func_80139A18(self->field_38, base + 0x4d5, lbl_eu_80664980, lbl_eu_80664988);
    func_80139A18(self->field_38, base + 0x4f6, lbl_eu_80664970, lbl_eu_80664978);
    func_80139A18(self->field_38, base + 0x501, lbl_eu_80664970, lbl_eu_80664978);
    func_8028C280(self, kind, item);
}
#pragma pop

// Kind-3 rebuild handler: reveal the five detail panes, bind the item's
// count/name/detail texts, position the cursor anchor between the detail and
// anchor panes, then store the selection into the page tables (+0x220 kind,
// +0x2c0 state byte 3, +0x2cc count byte) and the embedded item-box index.
#pragma push
#pragma optimize_for_size on
extern "C" void func_8028BE74(CEquipItemBox* self, int kind, int item) {
    char* base = lbl_eu_8050EFDC;
    // All pane lookups in this handler go through the layout's pane-finder
    // sub-object (vtable slot 13), not the root pane.
    func_80124270(((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                      ->v13((u32)(base + 0x4b3), 1),
                  1);
    func_80124270(((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                      ->v13((u32)(base + 0x5dc), 1),
                  1);
    func_80124270(((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                      ->v13((u32)(base + 0x5e7), 1),
                  1);
    func_80124270(((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                      ->v13((u32)(base + 0x5f2), 1),
                  1);
    func_80124270(((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
                      ->v13((u32)(base + 0x5fe), 1),
                  1);
    func_801392E4(kind);
    CItemInstance* obj = (CItemInstance*)item;
    u32 key = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf54(obj);
    // Count text: "xN" scaled by the vf08 slot count.
    u32 id = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf08(obj);
    char* str = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x1e - ((u8)id - 1));
    func_80136B4C(self->field_38, base + 0x5f2, str, 0);
    // Name text from the bdat lookup keyed by the vf54 name key.
    u32 name = func_801361E8(
        lbl_eu_806640D8, base + 0x36,
        (u16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf54(obj));
    func_80136B4C(self->field_38, base + 0x5e7, (char*)name, 0);
    // Detail text: when the vf90 value is present, prefix a per-name-byte
    // unit string for name bytes 3/4; otherwise bind the empty placeholder.
    if (((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf90(obj) == 0) {
        func_80136B4C(self->field_38, base + 0x5fe, base + 0x228, 0);
    } else {
        char buf[0x20];
        u32 v = func_801361E8(lbl_eu_806640D8, base + 0x633, (u16)key);
        if ((u8)v != 0 && (u32)((u8)v - 3) <= 1) {
            char* str2 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x21);
            sprintf(buf, base + 0x626,
                    ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf90(obj), str2);
        } else {
            sprintf(buf, base + 0x63c,
                    ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf90(obj));
        }
        func_80136A1C(self->field_38, base + 0x5fe, buf, 0);
    }
    // Category label: name-byte + 0x15 selects the category string.
    u32 cat = func_801361E8(lbl_eu_806640D8, base + 0, (u16)key);
    char* str3 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, (u8)cat + 0x15);
    func_80136B4C(self->field_38, base + 0x640, str3, 0);
    // Cursor anchor: midpoint helper between the detail pane and anchor pane
    // (both looked up again through the pane-finder sub-object).
    nw4r::lyt::Pane* paneDetail =
        ((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
            ->v13((u32)(base + 0x5e7), 1);
    nw4r::lyt::Pane* paneAnchor =
        ((CEquipItemBoxLayoutSubVtbl13*)((CEquipItemBoxLayoutView*)self->field_38)->field_10)
            ->v13((u32)(base + 0x193), 1);
    float pos[3];
    func_80137924(pos, paneDetail, paneAnchor,
                  ((CEquipItemBoxLayoutView*)self->field_38)->field_10);
    CEquipItemBoxKind3View* view = (CEquipItemBoxKind3View*)self;
    view->field_220 = (u16)key;
    view->field_2c0 = 3;
    view->field_2cc = (u8)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf08(obj);
    setItemBoxIndex((CItemBoxInfo*)((u8*)self + 0x210), 8,
                    (short)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf90(obj));
    copyVEC3(view->field_288, pos);
}
#pragma pop


// Per-cell equip-box rebuild for the kind-2 page: for each of the three
// slots, look the slot's item up, format the cell name/icon/stat texts into
// the pane, and store the per-cell ids/bytes/shorts into the page tables at
// +0x210/+0x2B8/+0x2C4/+0x2D0 (+ the page VEC3 records at +0x228). The
// slot==-1 path reads the item's flag list instead.
extern "C" void func_8028C280(CEquipItemBox* self, int a, int b) {
    char* base = lbl_eu_8050EFDC;
    if (b == 0) return;
    if (((CItemInstance*)b)->word == 0) return;
    CEquipItemBoxItemImplView* impl = (CEquipItemBoxItemImplView*)CItem_initItemImplInstances((CItemInstance*)b);
    u16 count = (u16)impl->vf30((CItemInstance*)b);
    int idx = 0;
    do {
        char buf[0x20];
        char buf40[0x20];
        u8 i1 = (u8)idx + 1;
        sprintf(buf, base + 0x64b, i1);
        nw4r::lyt::Pane* pane = self->field_38->GetRootPane()->FindPaneByName(buf, true);
        void* icon = 0;
        u16 storeKind = 0;
        u8 storeByte = 0;
        s16 storeKey = 0;
        if (idx >= count) {
            func_80124270(pane, 0);
            sprintf(buf, base + 0x6f4, i1);
            func_80136B4C(self->field_38, buf, base + 0x228, 0);
            sprintf(buf, base + 0x705, i1);
            func_80136B4C(self->field_38, buf, base + 0x228, 0);
        } else {
            func_80124270(pane, 1);
            s16 slot = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances((CItemInstance*)b))
                           ->vf40((CItemInstance*)b, (u8)idx);
            if (slot != -1) {
                CItemInstance* item = (CItemInstance*)func_80157C4C(3, slot);
                u32 g = lbl_eu_806640D8;
                u16 kind = (u16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf54(item);
                storeKind = kind;
                char* name = func_80136190((char*)base + 0x2d, (char*)base + 0x36,
                                            0x1e - ((u16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf08(item) - 1));
                switch ((u8)func_801361E8(g, base + 0x1a7, kind)) {
                case 0:
                    icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x65a, 0);
                    break;
                case 4:
                    icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x670, 0);
                    break;
                case 5:
                    icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x686, 0);
                    break;
                case 6:
                    icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x69c, 0);
                    break;
                case 7:
                    icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6b2, 0);
                    break;
                case 8:
                    icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6c8, 0);
                    break;
                case 9:
                    icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6de, 0);
                    break;
                }
                u32 key90 = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf90(item);
                if (key90 == 0) {
                    sprintf(buf, base + 0x6f4, i1);
                    func_80136B4C(self->field_38, buf, base + 0x228, 0);
                } else {
                    u32 v = func_801361E8(g, base + 0x633, kind);
                    if (v != 0 && (u8)((u8)v - 3) <= 1) {
                        char* str = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x21);
                        sprintf(buf40, base + 0x626, key90, str);
                    } else {
                        sprintf(buf40, base + 0x63c, key90);
                    }
                    sprintf(buf, base + 0x6f4, i1);
                    func_80136A1C(self->field_38, buf, buf40, 0);
                }
                char* str2 = func_8013639C((const void*)g, base + 0x36, kind);
                sprintf(buf40, base + 0x3b, str2, name);
                storeByte = (u8)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf08(item);
                storeKey = (s16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(item))->vf90(item);
                func_80137F88(pane, (unsigned long)icon);
            } else {
                CItemInstance* entry = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances((CItemInstance*)b))
                                           ->vf2C((CItemInstance*)b, (u8)idx);
                if (entry != 0 && (entry->flags & 1)) {
                    u16 kind2 = (u16)((entry->word >> 16) & 0xFFF);
                    u32 g2 = lbl_eu_806640D8;
                    u32 bits3 = (entry->word >> 22) & 7;
                    storeKind = kind2;
                    char* name2 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x1e - (bits3 - 1));
                    switch ((u8)func_801361E8(g2, base + 0x1a7, kind2)) {
                    case 0:
                        icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x65a, 0);
                        break;
                    case 4:
                        icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x670, 0);
                        break;
                    case 5:
                        icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x686, 0);
                        break;
                    case 6:
                        icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x69c, 0);
                        break;
                    case 7:
                        icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6b2, 0);
                        break;
                    case 8:
                        icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6c8, 0);
                        break;
                    case 9:
                        icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6de, 0);
                        break;
                    }
                    s16 bit11 = (s16)((entry->word >> 11) & 0x7FF);
                    if (bit11 == 0) {
                        sprintf(buf, base + 0x6f4, i1);
                        func_80136B4C(self->field_38, buf, base + 0x228, 0);
                    } else {
                        u32 v2 = func_801361E8(g2, base + 0x633, kind2);
                        if (v2 != 0 && (u8)((u8)v2 - 3) <= 1) {
                            char* str3 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x21);
                            sprintf(buf40, base + 0x626, bit11, str3);
                        } else {
                            sprintf(buf40, base + 0x63c, bit11);
                        }
                        sprintf(buf, base + 0x6f4, i1);
                        func_80136A1C(self->field_38, buf, buf40, 0);
                    }
                    char* str4 = func_8013639C((const void*)g2, base + 0x36, kind2);
                    sprintf(buf40, base + 0x3b, str4, name2);
                    storeByte = (u8)bits3;
                    storeKey = (s16)((entry->word >> 11) & 0x7FF);
                    func_80137F88(pane, (unsigned long)icon);
                } else {
                    icon = self->field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x65a, 0);
                    sprintf(buf, base + 0x6f4, i1);
                    func_80136B4C(self->field_38, buf, base + 0x228, 0);
                    sprintf(buf40, base + 0x2a, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x2a));
                    sprintf(buf, base + 0x705, i1);
                    func_80136A1C(self->field_38, buf, buf40, 0);
                    func_80137F88(pane, (unsigned long)icon);
                }
            }
        }
        nw4r::lyt::Pane* pane193 =
            self->field_38->GetRootPane()->FindPaneByName(base + 0x193, true);
        float out[3];
        func_80137924(out, pane, pane193, self->field_38->GetRootPane());
        u8 v = (u8)(idx + 8);
        if (v < 0xc) {
            ((CEquipItemBoxPageDataView*)self)->field_210[v] = storeKind;
        }
        if (v < 0xc) {
            ((CEquipItemBoxPageStoreView*)self)->field_2B8[v] = 3;
        }
        if (v < 0xc) {
            ((CEquipItemBoxPageStoreView*)self)->field_2C4[v] = storeByte;
        }
        if (v < 0xc) {
            ((CEquipItemBoxPageStoreView*)self)->field_2D0[v] = storeKey;
        }
        if (v < 0xc) {
            copyVEC3(((CEquipItemBoxPageStateView*)self)->field_228[v], out);
        }
        idx++;
    } while (idx < 3);
}

// Equip/use the grid item selected by (unk_1f4 + unk_1f5*5): resolve its
// owning category, then clear the item from the owner's slots (for window
// kind 3, from the matched CfObjectMove's move list). Returns 0 when the
// cell is empty or the kind is rejected (func_801392E4 == 2), else 1.
#pragma push
#pragma optimize_for_size on
extern "C" int func_8028CBCC(CEquipItemBox* self) {
    CEquipItemGrid* grid = (CEquipItemGrid*)&self->_pad37D[1];
    u8 idx = (u8)((u8)self->unk_1f4 + self->unk_1f5 * 5);
    s16 itemId = func_80282E4C(grid, idx);
    u16 kind = (u16)func_80282EC4(grid, idx);
    if (kind == 0) {
        return 0;
    }
    u32 kindByte = func_801392E4(kind);
    if ((u16)kindByte == 2) {
        // Item type cannot be unequipped this way.
        return 0;
    }
    // Find the party index whose category byte matches the cell's category
    // (skipped entirely for window kind 3).
    u32 slot = 0;
    if (kindByte != 3) {
        for (u8 i = 0; i < code80135FDC_getByte_64077(); i++) {
            if (func_802832D8(grid, idx) == func_801392B4(i)) {
                slot = i + 1;
                break;
            }
        }
        if (slot == 0) {
            return 0;
        }
        slot--;
    }
    if (slot < 3) {
        // Move-list search: register the selected search type, take the first
        // resulting entry and resolve its CfObjectMove.
        CEquipItemBoxMoveTable tbl = lbl_eu_8050EFB4;
        u8 holder[0x10];
        func_80043D90(holder);
        void* list = func_80043F18(holder);
        func_800F4A98(list, tbl.w[slot], 0);
        list = func_80043F18(holder);
        if (((CEquipItemBoxEnumList*)list)->field_620 >= 1) {
            CEquipItemBoxMoveEntryView* entry =
                (CEquipItemBoxMoveEntryView*)func_800F6EC0(list, 0);
            if (entry != 0 && entry->field_4 != 0) {
                void* moveObj = getCfObjectPc__FPQ22cf12CfObjectMove(entry);
                if (kindByte != 3) {
                    // Clear the item from the owning party member's slots.
                    u8 cat = func_801392B4(slot);
                    void* owner = func_8009EC9C(cat);
                    switch (func_801392E4(cat)) {
                    case 2: func_8009E0A8(owner, -1); break;
                    case 4: func_8009E024(owner, -1); break;
                    case 5: func_8009E030(owner, -1); break;
                    case 6: func_8009E03C(owner, -1); break;
                    case 7: func_8009E048(owner, -1); break;
                    case 8: func_8009E054(owner, -1); break;
                    }
                    func_800A1370(owner);
                    func_800BFDE0(moveObj, 0);
                }
            }
        }
        __dt__80043E88(holder, -1);
    }
    // Shared tail: copy the 12-byte category table and free the first slot
    // (across all six categories) that still holds the item id.
    CEquipItemBoxCatTable12 cats = lbl_eu_80668B50;
    bool cleared = false;
    for (u8 c2 = 0; c2 < 6; c2++) {
        if (cleared) break;
        u16 n = (u16)func_80157C20(cats.b[c2]);
        for (u16 j = 0; j < n; j++) {
            CItemInstance* obj = (CItemInstance*)func_80157C4C(cats.b[c2], (s16)j);
            if (obj == 0 || obj->word == 0) continue;
            u32 cnt = ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf30(obj);
            for (u8 k = 0; k < (u8)cnt; k++) {
                if ((s16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf40(obj, k)
                    == itemId) {
                    ((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))
                        ->vf44(obj, k, -1);
                    cleared = true;
                }
            }
            if (cleared) break;
        }
    }
    return 1;
}
#pragma pop

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

// File-event handler: the three bind files (field_24 box layout, field_28 arc
// accessor, field_2C bdat) report completion through this callback. Each
// branch tears down the completed file's region and rebuilds the affected
// subsystem (layout/accessors for file 1, the second accessor for file 2, the
// bdat table for file 3); returns 1 when the handle matched, else 0.
#pragma push
#pragma optimize_for_size on
int CEquipItemBox::OnFileEvent(CEventFile* ev) {
    void* h = ((CEquipItemBoxEventFileView*)ev)->handle;
    if (field_24 == h) {
        char* base = lbl_eu_8050EFDC;
        void* mem = (void*)mtl::MemManager::getHandleMEM2();
        createRegion__17UnkClass_8045F564FiiPCci(&_pad04[0], (int)mem, 0x20000, base + 0x711, 1);
        u8 guard[0x10];
        __ct__14Class_8045F858FP17UnkClass_8045F564(guard, &_pad04[0]);
        CEquipItemBoxFileHandleView* fh = (CEquipItemBoxFileHandleView*)field_24;
        void* buf = fh->field_4;
        fh->field_4 = 0;
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        field_30 = createArcResourceAccessor__10CLibLayoutFv();
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(field_30, buf, base + 0x71f);
        buildLayout(&field_38, field_30, base + 0x723);
        bindLayoutAnimTransform(field_38, (nw4r::lyt::AnimTransform**)&field_3C, field_30, base + 0x734);
        nw4r::lyt::Pane* root = field_38->GetRootPane();
        void* font = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, field_38);
        func_8013676C(root, (void*)((CEquipItemBoxFontView*)font)->f9());
        char* msg = getPackedFont();
        setLayoutTextBoxFont(field_38, base + 0x29b, (u32)msg);
        setLayoutTextBoxFont(field_38, base + 0x27c, (u32)msg);
        char* msg2 = func_801355BC();
        for (u8 i = 1; i <= 0x1e; i++) {
            char buf2[0x20];
            sprintf(buf2, base + 0x219, i);
            setLayoutTextBoxFont(field_38, buf2, (u32)msg2);
        }
        field_38->SetAnimationEnable((nw4r::lyt::AnimTransform*)field_3C, true);
        field_38->Animate(0);
        char* s4 = func_80136190((char*)base + 0x2d, (char*)base + 0x36, 4);
        func_80136B4C(field_38, base + 0x74c, s4, 0);
        func_80136B4C(field_38, base + 0x2d4, base + 0x228, 0);
        const char* name = (isClassicController__Q22cf13CfGameManagerFv(-1) == 0) ? base + 0x761 : base + 0x758;
        u16 key = func_8013606C(base + 0x76a, name, 0x49);
        char* texName = func_80138F78(key);
        nw4r::lyt::ArcResourceAccessor* acc2 = func_801355F4();
        void* tex = acc2->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, texName, 0);
        if (tex != 0) {
            func_80137E7C(field_38, base + 0x778, (u32)tex);
            CEquipItemBoxTexDimsView* dims =
                ((CEquipItemBoxTexObjView*)tex)->mChain->mDims;
            u16 v2 = dims->field_2;
            u16 v0 = dims->field_0;
            nw4r::lyt::Pane* pane =
                field_38->GetRootPane()->FindPaneByName(base + 0x778, true);
            if (pane != 0) {
                CEquipItemBoxF64Conv c;
                c.w[1] = (u32)v2;
                c.w[0] = 0x43300000;
                float fw = (float)(c.d - lbl_eu_80668B10);
                c.w[1] = (u32)v0;
                float fh = (float)(c.d - lbl_eu_80668B10);
                ((CEquipItemBoxPaneSizeView*)pane)->mW = fw;
                float size[2];
                size[0] = fw;
                size[1] = fh;
                ((CEquipItemBoxPaneSizeView*)pane)->mH = fh;
            }
        }
        nw4r::lyt::Pane* posPane =
            field_38->GetRootPane()->FindPaneByName(base + 0x784, true);
        copyVEC3((float*)&((CEquipItemBoxPagePosView*)this)->pos[0],
                 &((CEquipItemBoxPaneView*)posPane)->mTranslate[0]);
        CEquipItemBoxFourShorts c0 = func_80139658(field_38, base + 0x791, 0);
        CopyVec4s(lbl_eu_80664920, &c0);
        CEquipItemBoxFourShorts c1 = func_80139658(field_38, base + 0x791, 1);
        CopyVec4s(lbl_eu_80664928, &c1);
        s16 v0c = lbl_eu_80664920[3];
        s16 v1c = lbl_eu_80664928[3];
        lbl_eu_80664930[3] = v0c;
        lbl_eu_80664938[3] = v1c;
        lbl_eu_80664940[3] = v0c;
        lbl_eu_80664948[3] = v1c;
        lbl_eu_80664950[3] = v0c;
        lbl_eu_80664958[3] = v1c;
        lbl_eu_80664960[3] = v0c;
        lbl_eu_80664968[3] = v1c;
        // Cursor objects: construct stack temps, copy into the members and
        // dispatch the member vtable slot (CEIBCur has no dtor, so its temp
        // is left; the page-cur and CCur18 temps are destroyed).
        CEIBCur temp1;
        __ct__CEIBCur(&temp1, field_30);
        func_801FA220((u8*)&_pad44[0], (const u8*)&temp1);
        ((CEquipItemBoxCurMemberView*)&_pad44[0])->vfSlot8();
        CEIBPageCur temp2;
        __ct__CEIBPageCur(&temp2, field_30);
        func_801FA220((u8*)&pagecur[0], (const u8*)&temp2);
        __dt__80285C44(&temp2, -1);
        ((CEquipItemBoxCurMemberView*)&pagecur[0])->vfSlot8();
        u8 tempCur[0x1A];
        __ct__CCur18(tempCur, func_801355F4());
        CEquipItemBoxCCur18CopyView* curSrc = (CEquipItemBoxCCur18CopyView*)&tempCur[4];
        CEquipItemBoxCCur18CopyView* curDst = (CEquipItemBoxCCur18CopyView*)&ccur18[4];
        curDst->word0 = curSrc->word0;
        curDst->word1 = curSrc->word1;
        curDst->word2 = curSrc->word2;
        curDst->word3 = curSrc->word3;
        curDst->b0 = curSrc->b0;
        curDst->b1 = curSrc->b1;
        __dt__6CCur18Fv(tempCur, -1);
        ((CEquipItemBoxCurMemberView*)&ccur18[0])->vfSlot8();
        char* msg3 = func_801355BC();
        setLayoutTextBoxFont(field_38, base + 0x522, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x546, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x552, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x52e, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x53a, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x55e, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x5f2, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x5fe, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x5af, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x5be, (u32)msg3);
        setLayoutTextBoxFont(field_38, base + 0x5cd, (u32)msg3);
        func_80136B4C(field_38, base + 0x4bf, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0xa), 0);
        func_80136B4C(field_38, base + 0x4ca, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0xd), 0);
        func_80136B4C(field_38, base + 0x4d5, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x11), 0);
        func_80136B4C(field_38, base + 0x4e0, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x12), 0);
        func_80136B4C(field_38, base + 0x4eb, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0xc), 0);
        func_80136B4C(field_38, base + 0x4f6, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x13), 0);
        func_80136B4C(field_38, base + 0x5dc, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x18), 0);
        func_80136B4C(field_38, base + 0x501, func_80136190((char*)base + 0x2d, (char*)base + 0x36, 0x2f), 0);
        func_80136B4C(field_38, base + 0x591, base + 0x228, 0);
        func_80136B4C(field_38, base + 0x59b, base + 0x228, 0);
        func_80136B4C(field_38, base + 0x5a5, base + 0x228, 0);
        nw4r::lyt::Pane* paneB =
            field_38->GetRootPane()->FindPaneByName(base + 0x4bf, true);
        CEquipItemBoxFourShorts p1 = func_801397AC(paneB, 0);
        CopyVec4s(lbl_eu_80664970, &p1);
        CEquipItemBoxFourShorts p2 = func_801397AC(paneB, 1);
        CopyVec4s(lbl_eu_80664978, &p2);
        lbl_eu_80664980[3] = lbl_eu_80664970[3];
        lbl_eu_80664988[3] = lbl_eu_80664978[3];
        nw4r::lyt::Pane* pane5 =
            field_38->GetRootPane()->FindPaneByName(base + 0x522, true);
        p1 = func_801397AC(pane5, 0);
        CopyVec4s(lbl_eu_80664990, &p1);
        p2 = func_801397AC(pane5, 1);
        CopyVec4s(lbl_eu_80664998, &p2);
        lbl_eu_806649A0[3] = lbl_eu_80664990[3];
        lbl_eu_806649A8[3] = lbl_eu_80664998[3];
        nw4r::lyt::Pane* pane79 =
            field_38->GetRootPane()->FindPaneByName(base + 0x79e, true);
        p1 = func_801397AC(pane79, 0);
        CopyVec4s(lbl_eu_806649B0, &p1);
        p2 = func_801397AC(pane79, 1);
        CopyVec4s(lbl_eu_806649B8, &p2);
        lbl_eu_806649C0[3] = lbl_eu_806649B0[3];
        lbl_eu_806649C8[3] = lbl_eu_806649B8[3];
        lbl_eu_806649D0[3] = lbl_eu_806649B0[3];
        lbl_eu_806649D8[3] = lbl_eu_806649B8[3];
        func_80288AC0(this);
        field_24 = 0;
        validateHeap__17UnkClass_8045F564Fv(&_pad04[0]);
        __dt__14Class_8045F858Fv(guard, -1);
        return 1;
    }
    if (field_28 == h) {
        char* base = lbl_eu_8050EFDC;
        void* mem = (void*)mtl::MemManager::getHandleMEM2();
        createRegion__17UnkClass_8045F564FiiPCci(&_pad04[0x14], (int)mem, 0x100, base + 0x7a8, 1);
        u8 guard2[0x10];
        __ct__14Class_8045F858FP17UnkClass_8045F564(guard2, &_pad04[0x14]);
        CEquipItemBoxFileHandleView* fh2 = (CEquipItemBoxFileHandleView*)field_28;
        void* buf2 = fh2->field_4;
        fh2->field_4 = 0;
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        field_34 = createArcResourceAccessor__10CLibLayoutFv();
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(field_34, buf2, base + 0x71f);
        func_80288AC0(this);
        field_28 = 0;
        validateHeap__17UnkClass_8045F564Fv(&_pad04[0x14]);
        __dt__14Class_8045F858Fv(guard2, -1);
        return 1;
    }
    if (field_2C == h) {
        CEquipItemBoxFileHandleView* fh3 = (CEquipItemBoxFileHandleView*)field_2C;
        void* buf3 = fh3->field_4;
        fh3->field_4 = 0;
        func_8003AA34();  // retail passes no args (r3 stale from the field_2C load)
        if (getFP__FPCc(&lbl_eu_8050EFDC[0x7b9]) == 0) {
            setBdatEntry__5CBdatFUlPv(5, buf3);
        }
        lbl_eu_806649E0 = getFP__FPCc(&lbl_eu_8050EFDC[0x7b9]);
        func_80288AC0(this);
        field_2C = 0;
        return 1;
    }
    return 0;
}
#pragma pop

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

// Rebuild the equip grid for a page: clear all 0x400 cells (per-cell byte 6),
// then walk the page's items and populate the cells with their slot state.
// Category 3 cross-references the six equipment categories; the general path
// (categories 2 and 4..8) matches item slots directly. Ends by recomputing the
// page count and dispatching func_8028345C.
#pragma push
#pragma auto_inline off
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80282610(CEquipItemGrid* grid, u8 v, u8 b, u8 hi) {
    grid->count = 0;
    for (u16 i = 0; i < 0x400; i++) {
        CEquipItemData temp;
        func_80282594(&grid->data[i], func_80282574(&temp, -1, 0, 0, 0, 0, 0, 0));
        grid->field_2026[i] = 6;
    }
    u32 total = func_80157C20(v);
    CEquipItemBoxCatWordTable catsTemp = lbl_eu_8050EF50;
    const char* base = lbl_eu_8050EFDC;
    for (u16 n = 0; n < (u16)total; n++) {
        CItemInstance* obj = (CItemInstance*)func_80157C4C(v, (s16)n);
        if (obj == 0 || obj->word == 0) continue;
        u16 count = grid->count;
        grid->count = count + 1;
        grid->data[count].unk0 = (s16)n;
        CEquipItemData* cell = &grid->data[count];
    if (v == 3) {
            // Category 3: find which equipment slot (if any) owns this item id.
            CEquipItemBoxCatWordTable cats = catsTemp;
            for (u8 k = 0; k < 6; k++) {
                u8 cat = (u8)cats.w[k];
                u16 cnt2 = (u16)func_80157C20(cat);
                u32 kplus3 = (u32)(k + 3);
                for (u16 m = 0; m < cnt2; m++) {
                    CItemInstance* obj2 = (CItemInstance*)func_80157C4C(cat, (s16)m);
                    if (obj2 == 0 || obj2->word == 0) continue;
                    CEquipItemBoxItemImplView* impl2 = (CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj2);
                    u16 num = (u16)impl2->vf30(obj2);
                    for (u16 p = 0; p < num; p++) {
                        s16 slot = impl2->vf40(obj2, (u8)p);
                        if (slot == -1) continue;
                        if (slot != cell->unk0) continue;
                        cell->unk3 = 2;
                        u16 m2 = m;
                        for (u8 q = 0; q < (u8)code80135FDC_getByte_64077(); q++) {
                            u8 catB = (u8)func_801392B4(q);
                            CItemInstance* obj3 = (CItemInstance*)func_8009EC9C(catB);
                            s16 slot2 = 0;
                            switch (cat) {
                            case 2: slot2 = ((CEquipItemBoxItemView*)obj3)->field_26; break;
                            case 4: slot2 = ((CEquipItemBoxItemView*)obj3)->field_1c; break;
                            case 5: slot2 = ((CEquipItemBoxItemView*)obj3)->field_1e; break;
                            case 6: slot2 = ((CEquipItemBoxItemView*)obj3)->field_20; break;
                            case 7: slot2 = ((CEquipItemBoxItemView*)obj3)->field_22; break;
                            case 8: slot2 = ((CEquipItemBoxItemView*)obj3)->field_24; break;
                            }
                            if (slot2 == (s16)m2) {
                                cell->unk2 = catB;
                                cell->unk3 = 1;
                                if (k == 0) cell->unk4 = (u8)(p + 1);
                                else cell->unk4 = (u8)kplus3;
                                break;
                            }
                        }
                        break;
                    }
                    if (cell->unk3 != 0) break;
                }
                if (cell->unk3 != 0) break;
            }
            cell->unk6 = 1;
            // Name check: name-byte 1/2 clears the cell when the window kind
            // disagrees with the vf54 name.
            u32 name1 = func_801361E8(lbl_eu_806640D8, (char*)base, (u16)((CEquipItemBoxItemImplView*)CItem_initItemImplInstances(obj))->vf54(obj));
            if ((u8)name1 == 1) {
                if (hi != 2) cell->unk6 = 0;
            } else if ((u8)name1 == 2) {
                if (hi == 2) cell->unk6 = 0;
            }
            goto tail;
    }
general:
        if ((u32)v < 2 || (u32)v > 8) goto tail;
        if (b != 0) {
            u32 g2 = 0;
            switch (v) {
            case 2: g2 = lbl_eu_806640F4; break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8: g2 = lbl_eu_806640F8; break;
            }
            char buf[0x28];
            if (v == 2) {
                sprintf(buf, (char*)base + 0x7, b);
            } else {
                sprintf(buf, (char*)base + 0x12, b);
            }
            u16 kind2 = func_80139358(obj->word >> 20);
            if (func_801361E8(g2, buf, kind2) != 0) cell->unk6 = 1;
            if (v == 2 && b == 0xb && (obj->word >> 20) == 0x4c) cell->unk6 = 1;
            if (v != 2) {
                u32 name3 = func_801361E8(lbl_eu_806640F8, (char*)base + 0x17, func_80139358(obj->word >> 20));
                u8 n3 = (u8)name3;
                if ((u32)(n3 - 4) <= 9) {
                    /* no-op */
                } else if (n3 == 3) {
                    if (func_8026178C((u8*)func_8009EC9C(b) + 0x3534, 0x85) == 0) cell->unk6 = 0;
                }
                if (n3 == 2) {
                    if (func_8026178C((u8*)func_8009EC9C(b) + 0x3534, 0x84) == 0) cell->unk6 = 0;
                }
            }
            // Second category scan: match the cell's item id against the
            // equipment slot table of every category.
            for (u8 q = 0; q < (u8)code80135FDC_getByte_64077(); q++) {
                u8 catB = (u8)func_801392B4(q);
                CItemInstance* obj3 = (CItemInstance*)func_8009EC9C(catB);
                s16 slot2 = 0;
                switch (v) {
                case 2: slot2 = ((CEquipItemBoxItemView*)obj3)->field_26; break;
                case 4: slot2 = ((CEquipItemBoxItemView*)obj3)->field_1c; break;
                case 5: slot2 = ((CEquipItemBoxItemView*)obj3)->field_1e; break;
                case 6: slot2 = ((CEquipItemBoxItemView*)obj3)->field_20; break;
                case 7: slot2 = ((CEquipItemBoxItemView*)obj3)->field_22; break;
                case 8: slot2 = ((CEquipItemBoxItemView*)obj3)->field_24; break;
                }
                if (slot2 == cell->unk0) {
                    cell->unk3 = 1;
                    cell->unk2 = catB;
                    break;
                }
            }
        }
tail:
        u32 name4 = func_801361E8(lbl_eu_806640EC, (char*)base + 0x20, obj->word >> 20);
        cell->unk5 = ((u8)name4 == 1) ? 1 : 0;
        if (func_8009CF8C(0x3508) != 0) cell->unk5 = 0;
    }
    // Page-count update when the category changed (manual ceil: fctiwz +
    // fractional-part round-up, clamped to >= 1).
    if (grid->cat != v) {
        grid->cat = v;
        u32 cnt3 = grid->count;
        float fdiv = (float)(s32)cnt3 / lbl_eu_80668B00;
        s32 rows = (s32)fdiv;
        float frac = fdiv - (float)rows;
        if (lbl_eu_80668B04 != frac) rows++;
        if (!(u8)rows) rows = 1;
        grid->_pad2003 = (u8)rows;
        grid->idx = 0;
    }
    if ((u32)(v + 0xfe) <= 6) {
        // Mark the used equipment slots (unk7 = 1) and compute the per-cell
        // count threshold.
        u32 cnt = func_80157C20(v);
        u32 base = func_801576C8(v);
        s32 last = (s32)(cnt - 1);
        s32 diff = (s32)(cnt - base);
        s32 i2;
        for (i2 = last; i2 >= diff; i2--) {
            grid->data[i2].unk7 = 1;
        }
        int threshold = (int)(lbl_eu_80668B08 * (float)(s32)cnt);
        for (i2 = 0;; i2++) {
            int val = (i2 + 1) * 6;
            int diff2 = threshold - val;
            if (diff2 >= 0) {
                grid->field_2026[i2] = (u8)diff2;
            } else {
                grid->field_2026[i2] = (u8)(diff2 + 6);
                break;
            }
        }
    }
    func_8028345C(grid);
}
#pragma pop
// Page count for the current category: min(10, ceil(count / pagesize)).
// The (f32)(int) casts produce retail's 0x4330/xoris/lfd/fsubs s32->f32
// magic idiom; the named temp 'frac' is required to get retail's register
// allocation (constant into f0, conversion chain into f1).
extern "C" __declspec(noinline) u8 func_80282D60(CEquipItemGrid* grid) {
    f32 pages = (f32)(int)func_80157C20(grid->cat);
    if (pages >= lbl_eu_80668B20) {
        return 10;
    }
    f32 q = pages / lbl_eu_80668B00;
    int n = (int)q;
    f32 frac = q - (f32)n;
    if (lbl_eu_80668B04 != frac) {
        n = n + 1;
    }
    return (u8)n;
}
