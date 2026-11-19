#define IWORK_EVENT_INLINE_DTOR // ~CEquipItemBox elides the empty base dtor (retail)
// Auto-scaffolded catalog TU for kyoshin/CEquipItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/cf/CItemImpl.hpp"
#include "kyoshin/CEquipItemBox.hpp"
#include "kyoshin/cf/CtrlObjectParam.hpp"
#include "kyoshin/cf/CItem.hpp"
#include "kyoshin/CItemBoxInfo.hpp"
#include "kyoshin/CBaseCur.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <nw4r/math/math_types.h>
#include "nw4r/lyt/lyt_pane.h"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/CfEnumListHolder.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// --- referenced retail symbols (C linkage: plain symbol names) ---
// KEEP (blockers-A 2026-10-23): BdatGetU16Direct is declared int here
// (retail getEIBProgress defers narrowing, no clrlwi after the call); the
// canonical u16 form lives in kyoshin/code_80135FDC_Bdat.hpp.

// CItemImpl comes from the canonical owner header kyoshin/cf/CItemImpl.hpp.
// C-linkage import: CEquipChange.hpp declares renderItemBoxByKind with a u16 arg4,
// but retail passes the full 32-bit word (r6 -> mr -> setupItemBoxDetail), so the
// u32 form is declared here to avoid a call-site mask. Kept local like
// CItem_kindPtr (the sibling headers disagree on the signature).
// Pane-visibility helpers (C-ABI retail symbols; declared in CEquipItemBox.hpp
// with extern "C" so MWCC emits the plain names).
// Float constants in .sdata2, referenced via sda21 reloc.
extern const float lbl_eu_80668B28;
extern const float lbl_eu_80668B04;

// Constant records owned by this translation unit; MWCC emits them into .rodata.
static const CEquipItemBoxCatTable12 sEquipSlotClearCategories = {{ 2, 4, 5, 6, 7, 8, 2, 4, 5, 6, 7, 8 }};
static const CEquipItemBoxMoveTable sEquipMoveSearchTypes = {{ 0x10, 0x100000, 0x200000 }};

// varargs name formatter (MWCC keeps the unmangled library name).
int sprintf(char*, const char*, ...);
// SysWin name probe + pane-position reader (updateEIBCursorPos tail).

int CEquipItemBox::getEIBOpenFlag() { return unk_40; }

// Initialise a full item grid: clear every cell and reset the header fields.
CEquipItemGrid::CEquipItemGrid() {
    // Reset each 8-byte cell to { unk0 = -1, rest = 0 }.
    CEquipItemData* const end = &data[0x400];
    CEquipItemData* p = data;
    do {
        setEIBItem(p, -1, 0, 0, 0, 0, 0, 0);
        p++;
    } while (p < end);
    count = 0;
    cat = 0;
    _pad2003 = 0;
    idx = 0;
    _pad2005 = 0;
    // Repopulate every cell from a cleared local (round-trips through copy).
    u32 i = 0;
    do {
        CEquipItemData temp;
        copyEIBItem(&data[(u16)i], setEIBItem(&temp, -1, 0, 0, 0, 0, 0, 0));
        i++;
    } while (i < 0x400);
}

// Write a CEquipItemData from 8 separate scalar inputs.
extern "C" CEquipItemData* setEIBItem(CEquipItemData* dst, s16 a0, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7) {
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
extern "C" void copyEIBItem(CEquipItemData* dst, const CEquipItemData* src) {
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk3 = src->unk3;
    dst->unk4 = src->unk4;
    dst->unk5 = src->unk5;
    dst->unk6 = src->unk6;
    dst->unk7 = src->unk7;
}


// Look up a grid element and return the item object pointer when present.
void* CEquipItemGrid::getEIBItemObj(u16 index) {
    u16 offset = (u16)((s8)idx * 0x1e + index);
    if ((u32)offset < (u32)count) {
        u32* obj = (u32*)CItem_kindPtr(cat, data[offset].unk0);
        if (obj && *obj) {
            return obj;
        }
    }
    return 0;
}

// Return the item kind (word >> 20) for the this element, or 0.
u32 CEquipItemGrid::getEIBItemKind(u16 index) {
    u16 offset = (u16)((s8)idx * 0x1e + index);
    if ((u32)offset < (u32)count) {
        u32* obj = (u32*)CItem_kindPtr(cat, data[offset].unk0);
        if (obj && *obj) {
            return *obj >> 20;
        }
    }
    return 0;
}

// Return the stored item short for a valid this cell, or -1.
s16 CEquipItemGrid::getEIBItemId(u16 index) {
    u16 offset = (u16)((s8)idx * 0x1e + index);
    if ((u32)offset < (u32)count) {
        CEquipItemData* item = &data[offset];
        u32* obj = (u32*)CItem_kindPtr(cat, item->unk0);
        if (obj && *obj) {
            return item->unk0;
        }
    }
    return -1;
}

// Advance this cursor index; wrap to 0 when it reaches the count (field_2003).
void CEquipItemGrid::eibGridNext() {
    u8 v = (u8)idx + 1;
    idx = v;
    if ((s8)v >= _pad2003) {
        idx = 0;
    }
}

// Step this cursor index backwards; wrap to (count-1) when it goes below 0.
void CEquipItemGrid::eibGridPrev() {
    u8 v = (u8)idx - 1;
    idx = v;
    if ((s8)v < 0) {
        idx = (u8)_pad2003 - 1;
    }
}

// Validate the this cell selected by (cursor row * 30 + param): for the
// equippable categories (2 and 4..8) scan the item's instance list for a
// "full" slot (slot id -1 with its flag bit 0 set -> return -3); an empty
// instance list and category 3 fall back to the cell's byte-3 clamp
// (-1 when the cell holds any count, else 0).
s8 CEquipItemGrid::chkEIBEquip(u16 param) {
    u16 offset = (u16)((s8)idx * 0x1e + param);
    if ((u32)offset < (u32)count) {
        CEquipItemData* item = &data[offset];
        CItemInstance* obj = (CItemInstance*)CItem_kindPtr(cat, item->unk0);
        if (obj != 0 && obj->word != 0) {
            int cat = this->cat;
            switch (cat) {
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8: {
                u8 count = (u8)CItem_initItemImplInstances((CItemData*)obj)->vf30((CItemData*)obj);
                if (count != 0) {
                    for (u8 i = 0; i < count; i++) {
                        if ((s16)CItem_initItemImplInstances((CItemData*)obj)->vf40((CItemData*)obj, i) == -1) {
                            CItemInstance* entry = (CItemInstance*)CItem_initItemImplInstances((CItemData*)obj)->vf2C((CItemData*)obj, i);
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

// Return the this cell's byte 3 when its item object is valid, else 0.
u8 CEquipItemGrid::getEIBByte3(u16 index) {
    u16 offset = (u16)((s8)idx * 0x1e + index);
    if ((u32)offset < (u32)count) {
        CEquipItemData* item = &data[offset];
        u32* obj = (u32*)CItem_kindPtr(cat, item->unk0);
        if (obj && *obj) {
            return item->unk3;
        }
    }
    return 0;
}

// Return the this cell's byte 4 when its item object is valid, else 0.
u8 CEquipItemGrid::getEIBByte4(u16 index) {
    u16 offset = (u16)((s8)idx * 0x1e + index);
    if ((u32)offset < (u32)count) {
        CEquipItemData* item = &data[offset];
        u32* obj = (u32*)CItem_kindPtr(cat, item->unk0);
        if (obj && *obj) {
            return item->unk4;
        }
    }
    return 0;
}

// Return the this cell's byte 6 when its item object is valid, else 0.
u32 CEquipItemGrid::getEIBByte6(u16 index) {
    u16 offset = (u16)((s8)idx * 0x1e + index);
    if ((u32)offset < (u32)count) {
        CEquipItemData* item = &data[offset];
        u32* obj = (u32*)CItem_kindPtr(cat, item->unk0);
        if (obj && *obj) {
            return item->unk6;
        }
    }
    return 0;
}

// Return the this cell's byte 7 (unk7) value, or 0 when out of range.
u8 CEquipItemGrid::getEIBByte7(u32 param) {
    u16 off = (u16)((s8)idx * 30 + (int)param);
    if ((u32)off < 0x400) {
        return data[off].unk7;
    }
    return 0;
}

// Fetch byte at this index field_2004, or 0 if index >= 0x400.
u8 CEquipItemGrid::getEIBColCount() {
    s8 idx = this->idx;
    if (idx >= 0x400) return 0;
    return field_2026[idx];
}

// Return the this cell's byte 2 when its item object is valid, else 0.
u8 CEquipItemGrid::getEIBByte2(u16 index) {
    u16 offset = (u16)((s8)idx * 0x1e + index);
    if ((u32)offset < (u32)count) {
        CEquipItemData* item = &data[offset];
        u32* obj = (u32*)CItem_kindPtr(cat, item->unk0);
        if (obj && *obj) {
            return item->unk2;
        }
    }
    return 0;
}

// Format the this cell's item name into the this's name buffer (this+0x2006);
// the cat-3 path appends a per-slot count string via a second sprintf.
// Returns the buffer, or 0 when the cell is out of range / empty.
char* CEquipItemGrid::fmtEIBItemName(u32 param) {
    u16 offset = (u16)((s8)idx * 0x1e + param);
    if ((u32)offset < (u32)count) {
        CItemInstance* obj = (CItemInstance*)CItem_kindPtr(cat, data[offset].unk0);
        if (obj != 0) {
            if (obj->word != 0) {
                sprintf(&field_2006[0], &lbl_eu_8050EFDC[0x2a],
                        CItem_initItemImplInstances((CItemData*)obj)->vf20((CItemData*)obj));
                if (cat == 3) {
                    // Retail re-fetches the impl accessor before the slot probe
                    // and keeps v unmasked until the count expression.
                    u32 v = CItem_initItemImplInstances((CItemData*)obj)->vf08((CItemData*)obj);
                    char* str = BdatTouchStringCell(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], 0x1e - ((u8)v - 1));
                    sprintf(&field_2006[0], &lbl_eu_8050EFDC[0x3b], &field_2006[0], str);
                }
                return &field_2006[0];
            }
        }
    }
    return 0;
}

// Dispatch the equip-box this sort/refresh by category and page: categories
// 4..8 use the page-indexed sort chain, category 2 the flag-scan chain, and
// category 3 the name-byte chain (page 3..0xB special-cases re-sort after a
// category rebuild). -O4,s selects retail's stmw r30 prologue.
void CEquipItemGrid::sortEIBGridByPage() {
    sortEIBGridByCat(this->cat);
    u8 page = (u8)CItemBlock_getFlag120EC(this->cat);
    // Category dispatch first
    // page dispatches with the cat-2 body laid out first (retail body order).
    if ((u32)(this->cat - 4) <= 4) goto cat48;
    if ((int)this->cat == 2) goto cat2;
    if ((int)this->cat == 3) goto cat3;
    goto done;
cat2:
    sortEIBByFlags();
    switch (page) {
    case 0:
        sortEIBByNameMode(1);
        break;
    case 1:
        sortEIBFlagAsc(); sortEIBByVf30(); sortEIBByProg();
        sortEIBByNameMode(1);
        break;
    case 2:
        sortEIBFlagAsc(); sortEIBByVf30(); sortEIBByProg();
        sortEIBByNameKey(); sortEIBByNameMode(1);
        break;
    case 3:
        sortEIBFlagAsc(); sortEIBByVf30(); sortEIBByProg();
        sortEIBByNameKey69(); sortEIBByNameMode(1);
        break;
    case 4:
        sortEIBByProg(); sortEIBByVf30(); sortEIBFlagDesc();
        sortEIBByNameMode(1);
        break;
    case 5:
        sortEIBByProg(); sortEIBByVf30(); sortEIBFlagAsc();
        sortEIBByNameMode(1);
        break;
    case 6:
        sortEIBByCount();
        break;
    }
    goto done;
cat48:
    sortEIBByFlags();
    switch (page) {
    case 0:
        sortEIBByNm17(1); sortEIBByNm17(2); sortEIBByNm17(3);
        sortEIBFlagAsc(); sortEIBByVf30(); sortEIBByNm72();
        break;
    case 1:
        sortEIBByNm17(1); sortEIBByNm17(2); sortEIBByNm17(3);
        sortEIBFlagAsc(); sortEIBByVf30(); sortEIBByNm7A();
        break;
    case 2:
        sortEIBFlagAsc(); sortEIBByVf30(); sortEIBByNm72();
        sortEIBByNm17(3); sortEIBByNm17(2); sortEIBByNm17(1);
        break;
    case 3:
        sortEIBFlagAsc(); sortEIBByVf30(); sortEIBByNm72();
        sortEIBByNm17(3); sortEIBByNm17(1); sortEIBByNm17(2);
        break;
    case 4:
        sortEIBFlagAsc(); sortEIBByVf30(); sortEIBByNm72();
        sortEIBByNm17(2); sortEIBByNm17(1); sortEIBByNm17(3);
        break;
    case 5:
        sortEIBByNm17(1); sortEIBByNm17(2); sortEIBByNm17(3);
        sortEIBByNm72(); sortEIBFlagAsc(); sortEIBByVf30();
        sortEIBByNm82();
        break;
    case 6:
        sortEIBByNm17(1); sortEIBByNm17(2); sortEIBByNm17(3);
        sortEIBByNm72(); sortEIBByVf30(); sortEIBFlagDesc();
        break;
    case 7:
        sortEIBByNm17(1); sortEIBByNm17(2); sortEIBByNm17(3);
        sortEIBByNm72(); sortEIBByVf30(); sortEIBFlagAsc();
        break;
    case 8:
    case 9:
        sortEIBByCount();
        break;
    }
    goto done;
cat3:
    switch (page) {
    case 0:
        sortEIBByFlags();
        break;
    case 1:
        sortEIBByVf90(); sortEIBGridByCat(cat); sortEIBByVf08();
        break;
    case 2:
        sortEIBGridByCat(cat); sortEIBByVf90(); sortEIBByVf08();
        sortEIBByNm00(2); sortEIBByNm00(0); sortEIBByNm00(1);
        break;
    case 3:
        sortEIBGridByCat(cat); sortEIBByVf90(); sortEIBByVf08();
        sortEIBByNm00(1); sortEIBByNm00(0); sortEIBByNm00(2);
        break;
    case 4:
        sortEIBGridByCat(cat); sortEIBByVf90(); sortEIBByVf08();
        sortEIBByNm00(2); sortEIBByNm00(1); sortEIBByNm00(0);
        break;
    case 5:
        sortEIBByNm00(2); sortEIBByNm00(1); sortEIBByNm00(0);
        sortEIBByVf90(); sortEIBByVf08(); sortEIBGridByCat(cat);
        sortEIBByNm58(1);
        break;
    case 6:
        sortEIBByNm00(2); sortEIBByNm00(1); sortEIBByNm00(0);
        sortEIBByVf90(); sortEIBByVf08(); sortEIBGridByCat(cat);
        sortEIBByNm58(2);
        break;
    case 7:
        sortEIBByNm00(2); sortEIBByNm00(1); sortEIBByNm00(0);
        sortEIBByVf90(); sortEIBByVf08(); sortEIBGridByCat(cat);
        sortEIBByNm58(3);
        break;
    case 8:
        sortEIBByNm00(2); sortEIBByNm00(1); sortEIBByNm00(0);
        sortEIBByVf90(); sortEIBByVf08(); sortEIBGridByCat(cat);
        sortEIBByNm58(4);
        break;
    case 9:
        sortEIBByNm00(2); sortEIBByNm00(1); sortEIBByNm00(0);
        sortEIBByVf90(); sortEIBByVf08(); sortEIBGridByCat(cat);
        sortEIBByNm58(5);
        break;
    case 10:
        sortEIBByNm00(2); sortEIBByNm00(1); sortEIBByNm00(0);
        sortEIBByVf90(); sortEIBByVf08(); sortEIBGridByCat(cat);
        sortEIBByNm58(6);
        break;
    case 11:
        sortEIBByCount();
        break;
    }
    goto done;
done: ;
}

extern "C" CEquipItemData* copyEIBItemRet(CEquipItemData* dst, const CEquipItemData* src) {
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk3 = src->unk3;
    dst->unk4 = src->unk4;
    dst->unk5 = src->unk5;
    dst->unk6 = src->unk6;
    dst->unk7 = src->unk7;
    return dst;
}

// Per-item display-count key: category band selects the computation -
// cat 3 scales the bdat count by a per-item float record, cat 9 sums four
// instance-slot records scaled by the same table, everything else multiplies
// by an accumulated per-count increment. Result is returned *10.
u32 CEquipItemGrid::calcEIBCountKey(CItemInstance* item) {
    u32 word = item->word;
    u16 kind = BdatGetItemId(word >> 20);
    u8 cat = (word >> 12) & 0xF;
    u32 result = 0;
    if (cat == 3) {
        u32 key = CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
        u32 id = CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item);
        u16 count =
            (u16)BdatGetU16Direct((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0x40], key);
        CEquipItemBoxF32Record rec = lbl_eu_8050EF68;
        // int->double via the 0x4330 magic, then scale by rec[id] and truncate.
        result = __cvt_fp2unsigned((double)(u32)count * (double)rec.f[id & 0x3F]);
    } else if (cat == 9) {
        u32 id = CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item);
        CEquipItemBoxF32Record rec = lbl_eu_8050EF68;
        for (u8 i = 0; i < 4; i++) {
            CItemInstance* entry =
                (CItemInstance*)CItem_initItemImplInstances((CItemData*)item)->vf2C((CItemData*)item, i);
            if (entry != 0) {
                u16 count = (u16)BdatGetU16Direct((const void*)lbl_eu_806640D8,
                                               &lbl_eu_8050EFDC[0x40], i);
                double dcount = (double)(u32)count;
                double dacc = (double)(u32)result;
                result = __cvt_fp2unsigned(
                    0.25f * (dcount * rec.f[id & 0x3F]) + dacc);
            }
        }
    } else {
        u32 v = LookupItemTypeTable(kind);
        u32 count = BdatGetU16Direct((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0x40], kind);
        u8 n = (u8)CItem_initItemImplInstances((CItemData*)item)->vf30((CItemData*)item);
        float inc = lbl_eu_80668B28;
        for (u8 i = 0; i < n; i++) {
            inc += 0.2f;
        }
        result = __cvt_fp2unsigned((double)(u32)(u16)count * inc);
    }
    return result * 10;
}

// Category-dispatching bubble sort over the this entries (the shared first
// pass of the page sort chain): cat 3 sorts descending by the vf54 name key,
// cat 9 by slot occupancy / byte-7 clamp, everything else by the raw kind.
void CEquipItemGrid::sortEIBGridByCat(u32 cat) {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            // objB declared first so its web claims the higher saved
            // register (retail r23), matching the sibling sorts.
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(this->cat, b->unk0);
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(this->cat, a->unk0);
            if (cat == 3) {
                // Descending vf54 name-key sort (u16 compare).
                CItemImpl* implA =
                    CItem_initItemImplInstances((CItemData*)objA);
                CItemImpl* implB =
                    CItem_initItemImplInstances((CItemData*)objB);
                u16 keyA = (u16)implB->vf54((CItemData*)objB);
                u16 keyB = (u16)implA->vf54((CItemData*)objA);
                if (keyA < keyB) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    copyEIBItemRet(&tA, a);
                    copyEIBItemRet(&tB, b);
                    copyEIBItem(a, &tB);
                    copyEIBItemRet(&tC, &tA);
                    copyEIBItem(b, &tC);
                    swapped = 1;
                }
            } else if (cat == 9) {
                // Slot-occupancy probe: items with instances float to the
                // front, ties fall back to the byte-7 clamp compare.
                int kA = IsSkillItem(objA);
                int kB = IsSkillItem(objB);
                if (CItem_initItemImplInstances((CItemData*)objA)->vf80((CItemData*)objA)
                    != 0) {
                    kA = 0;
                }
                if (CItem_initItemImplInstances((CItemData*)objB)->vf80((CItemData*)objB)
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
                    copyEIBItemRet(&tA, a);
                    copyEIBItemRet(&tB, b);
                    copyEIBItem(a, &tB);
                    copyEIBItemRet(&tC, &tA);
                    copyEIBItem(b, &tC);
                    swapped = 1;
                }
            } else {
                // Default: descending raw-kind order.
                u16 kindA = BdatGetItemId(objA->word >> 20);
                u16 kindB = BdatGetItemId(objB->word >> 20);
                if (kindA > kindB) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    copyEIBItemRet(&tA, a);
                    copyEIBItemRet(&tB, b);
                    copyEIBItem(a, &tB);
                    copyEIBItemRet(&tC, &tA);
                    copyEIBItem(b, &tC);
                    swapped = 1;
                }
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by the item record's +4 flag
// halfword (sort key). Sibling of sortEIBByNm82's name-byte sort.
void CEquipItemGrid::sortEIBByFlags() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            if ((u32)objA->flags > (u32)objB->flags) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by calcEIBCountKey's per-item key
// (unsigned compare). Same 5-copy chained swap as sortEIBByFlags.
void CEquipItemGrid::sortEIBByCount() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            if (calcEIBCountKey(objA) < calcEIBCountKey(objB)) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by the item impl's slot-0x08 key
// (u16 compare). Same 3-temp chained copy swap as sortEIBByFlags.
void CEquipItemGrid::sortEIBByVf08() {
    CItemInstance* objB;  // born early so its web claims a high saved register (retail r29)
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u16 keyB = (u16)CItem_initItemImplInstances((CItemData*)objB)->vf08((CItemData*)objB);
            if ((u16)CItem_initItemImplInstances((CItemData*)objA)->vf08((CItemData*)objA) < keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries so items whose name byte (bdat lookup at
// pattern offset 0x58 of the vf54 key) equals the search key float to the
// front (ascending equality flag). The key is the item impl's slot-0x54
// value; the 3-temp copy swap matches sortEIBByNm17's shape.
void CEquipItemGrid::sortEIBByNm58(int param) {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 keyA = CItem_initItemImplInstances((CItemData*)objA)->vf54((CItemData*)objA);
            u32 keyB = CItem_initItemImplInstances((CItemData*)objB)->vf54((CItemData*)objB);
            u8 nameA = BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0x58], (u16)keyA);
            u8 nameB = BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0x58], (u16)keyB);
            u32 eqA = (u32)((u8)nameA == param);
            u32 eqB = (u32)((u8)nameB == param);
            if (eqA < eqB) {
                CEquipItemData v;
                CEquipItemData u;
                CEquipItemData t;
                copyEIBItemRet(&t, a);
                copyEIBItem(a, copyEIBItemRet(&u, b));
                copyEIBItem(b, copyEIBItemRet(&v, &t));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries so items whose name byte (bdat lookup at the
// base pattern of the vf54 key) equals the search key float to the front.
// Sibling of sortEIBByNm58 with a different name-pattern offset.
void CEquipItemGrid::sortEIBByNm00(int param) {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 keyA = CItem_initItemImplInstances((CItemData*)objA)->vf54((CItemData*)objA);
            u32 keyB = CItem_initItemImplInstances((CItemData*)objB)->vf54((CItemData*)objB);
            u8 nameA = BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0], (u16)keyA);
            u8 nameB = BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0], (u16)keyB);
            u32 eqA = (u32)((u8)nameA == param);
            u32 eqB = (u32)((u8)nameB == param);
            if (eqA < eqB) {
                CEquipItemData v;
                CEquipItemData u;
                CEquipItemData t;
                copyEIBItemRet(&t, a);
                copyEIBItem(a, copyEIBItemRet(&u, b));
                copyEIBItem(b, copyEIBItemRet(&v, &t));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries by getEIBProgress's scaled item-progress key
// (double compare, swap when the left key is smaller so higher-progress
// entries bubble to the front). Same 3-temp chained copy swap.
void CEquipItemGrid::sortEIBByProg() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            double keyA = getEIBProgress(objA);
            double keyB = getEIBProgress(objB);
            if (keyA < keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by the item impl's slot-0x30 key
// (u16 compare applied at the compare site). Same 3-temp chained copy swap.
void CEquipItemGrid::sortEIBByVf30() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        CItemInstance* objB;  // born early so its web claims a high saved register (retail r29)
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 keyB = CItem_initItemImplInstances((CItemData*)objB)->vf30((CItemData*)objB);
            u32 keyA = CItem_initItemImplInstances((CItemData*)objA)->vf30((CItemData*)objA);
            if ((u16)keyA < (u16)keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries by hasEIBSlotFlag's flag-scan key (unsigned
// compare, swap when the left key is smaller so flag-set entries bubble to
// the front). Same 3-temp chained copy swap as sortEIBByFlags.
void CEquipItemGrid::sortEIBFlagAsc() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 keyA = hasEIBSlotFlag(objA);
            u32 keyB = hasEIBSlotFlag(objB);
            if (keyA < keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Destructor clone: free object memory when this is non-null and delete flag set.

// Bubble-sort the this entries by hasEIBSlotFlag's flag-scan key (unsigned
// compare, swap when the left key is larger; descending sibling of
// sortEIBFlagAsc). Same 3-temp chained copy swap.
void CEquipItemGrid::sortEIBFlagDesc() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 keyA = hasEIBSlotFlag(objA);
            u32 keyB = hasEIBSlotFlag(objB);
            if (keyA > keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Dual-mode bubble sort over the this entries keyed by getEIBNameKey's
// name-table key. mode 0: ascending u8-key sort; other: move entries whose
// key equals the this's _pad2005 byte toward the front (one swap per pass).
void CEquipItemGrid::sortEIBByNameMode(u32 mode) {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 keyA = getEIBNameKey(objA);
            u32 keyB = getEIBNameKey(objB);
            if (mode != 0) {
                u8 g = _pad2005;
                if ((g == (u8)keyA) < (g == (u8)keyB)) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    copyEIBItemRet(&tA, a);
                    copyEIBItemRet(&tB, b);
                    copyEIBItem(a, &tB);
                    copyEIBItemRet(&tC, &tA);
                    copyEIBItem(b, &tC);
                    swapped = 1;
                }
            } else {
                if ((u8)keyA > (u8)keyB) {
                    CEquipItemData tA;
                    CEquipItemData tB;
                    CEquipItemData tC;
                    copyEIBItemRet(&tA, a);
                    copyEIBItemRet(&tB, b);
                    copyEIBItem(a, &tB);
                    copyEIBItemRet(&tC, &tA);
                    copyEIBItem(b, &tC);
                    swapped = 1;
                }
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by the item name-byte key from
// BdatGetU8Direct's bdat lookup (u8 compare). Same 3-temp chained copy swap.
void CEquipItemGrid::sortEIBByNameKey() {
    const char* base = lbl_eu_8050EFDC;
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 gA = (u32)lbl_eu_806640F4;
            u8 nameA = BdatGetU8Direct((const void*)gA, base + 0x61, (u16)BdatGetItemId(objA->word >> 20));
            u32 gB = (u32)lbl_eu_806640F4;
            u8 nameB = BdatGetU8Direct((const void*)gB, base + 0x61, (u16)BdatGetItemId(objB->word >> 20));
            if ((u8)nameA < (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by the item name-byte key from
// BdatGetU8Direct's bdat lookup (u8 compare, name pattern offset 0x69). Same
// 3-temp chained copy swap.
void CEquipItemGrid::sortEIBByNameKey69() {
    const char* base = lbl_eu_8050EFDC;
    CEquipItemData* a;
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 gA = (u32)lbl_eu_806640F4;
            u8 nameA = BdatGetU8Direct((const void*)gA, base + 0x69, (u16)BdatGetItemId(objA->word >> 20));
            u32 gB = (u32)lbl_eu_806640F4;
            u8 nameB = BdatGetU8Direct((const void*)gB, base + 0x69, (u16)BdatGetItemId(objB->word >> 20));
            if ((u8)nameA < (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by the item name-byte key from
// BdatGetU8Direct's bdat lookup at name pattern offset 0x72 (u8 compare).
void CEquipItemGrid::sortEIBByNm72() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u16 kindA = BdatGetItemId(objA->word >> 20);
            u16 kindB = BdatGetItemId(objB->word >> 20);
            u8 nameA = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x72], kindA);
            u8 nameB = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x72], kindB);
            if ((u8)nameA < (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by the item name-byte key from
// BdatGetU8Direct's bdat lookup at name pattern offset 0x7A (u8 compare).
void CEquipItemGrid::sortEIBByNm7A() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u16 kindA = BdatGetItemId(objA->word >> 20);
            u16 kindB = BdatGetItemId(objB->word >> 20);
            u8 nameA = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x7a], kindA);
            u8 nameB = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x7a], kindB);
            if ((u8)nameA < (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries so that items whose name byte matches the
// search key float to the front (ascending equality flag, u8 name lookup at
// pattern offset 0x17). The equality is evaluated via a subtraction so MWCC
// emits the cntlzw/srwi 0-or-1 idiom for both sides.
void CEquipItemGrid::sortEIBByNm17(int key) {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u16 kindA = BdatGetItemId(objA->word >> 20);
            u16 kindB = BdatGetItemId(objB->word >> 20);
            u8 nameA = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x17], kindA);
            u8 nameB = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x17], kindB);
            u32 eqA = (u32)((u8)nameA == key);
            u32 eqB = (u32)((u8)nameB == key);
            if (eqA < eqB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries descending by the item name lookup byte
// (name pattern offset 0x82; swap when the left name byte is larger).
void CEquipItemGrid::sortEIBByNm82() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            CItemInstance* objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u16 kindA = BdatGetItemId(objA->word >> 20);
            u16 kindB = BdatGetItemId(objB->word >> 20);
            u8 nameA = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x82], kindA);
            u8 nameB = BdatGetU8Direct((const void*)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x82], kindB);
            if ((u8)nameA > (u8)nameB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Bubble-sort the this entries ascending by the item impl's slot-0x90 key
// (u8 compare applied at the compare site). Same 3-temp chained copy swap.
void CEquipItemGrid::sortEIBByVf90() {
    for (u16 i = 0; i < count - 1; i++) {
        int swapped = 0;
        CItemInstance* objA;  // born early so its web claims a high saved register (retail r29)
        for (u16 j = 0; j < count - 1 - i; j++) {
            CEquipItemData* a = &data[j];
            CEquipItemData* b = &data[j + 1];
            objA = (CItemInstance*)CItem_kindPtr(cat, a->unk0);
            CItemInstance* objB = (CItemInstance*)CItem_kindPtr(cat, b->unk0);
            u32 keyA = CItem_initItemImplInstances((CItemData*)objA)->vf90((CItemData*)objA);
            u32 keyB = CItem_initItemImplInstances((CItemData*)objB)->vf90((CItemData*)objB);
            if ((u8)keyA < (u8)keyB) {
                CEquipItemData tC;
                CEquipItemData tB;
                CEquipItemData tA;
                copyEIBItemRet(&tA, a);
                copyEIBItem(a, copyEIBItemRet(&tB, b));
                copyEIBItem(b, copyEIBItemRet(&tC, &tA));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

// Scaled item-progress reader: look up the item's stored count and the row's
// flag word; when bit 4 is set, rescale the count by the drop-rate table and
// Scaled item-progress reader: look up the item's stored count and the row's
// flag word; when bit 4 is set, rescale the count by the drop-rate table and
// clamp the result at 999. All FP math is single precision: the (f32) casts
// make MWCC emit its own 0x4330/lfd/fsubs s32->f32 magic.
double CEquipItemGrid::getEIBProgress(CItemInstance* item) {
    u32 word = item->word;
    u32 g = (u32)lbl_eu_806640F4;
    u16 kind = BdatGetItemId(word >> 20);
    u16 result = BdatGetU16Direct((const void*)g, &lbl_eu_8050EFDC[0x8b], kind);
    u8 v2 = BdatGetU8Direct((const void*)g, &lbl_eu_8050EFDC[0x92], kind);
    if (v2 & 4) {
        void* obj = CtrlObjectParam_GetCharEntry(1);
        u16 n = (u16)CtrlObjectParam_GetArtsDataWord(obj);
        result = 1.5f * (f32)(int)(result * n);
        if (result >= 0x3E7) result = 0x3E7;
    }
    return (f32)result;
}

// Scan the item instance list: return 1 when any entry's flag word bit 0 is
// set, else 0. The instance accessor is re-fetched per iteration (retail
// shape); vtable slots [12]/[11] are the count and get-item methods.
u32 CEquipItemGrid::hasEIBSlotFlag(CItemInstance* item) {
    u16 count = CItem_initItemImplInstances((CItemData*)item)->vf30((CItemData*)item);
    for (u8 i = 0; i < count; i++) {
        CItemInstance* entry =
                (CItemInstance*)CItem_initItemImplInstances((CItemData*)item)->vf2C((CItemData*)item, i);
        if (entry != 0 && (entry->flags & 1) != 0) {
            return 1;
        }
    }
    return 0;
}

// Name-table scan: build a 1..10 name key, look it up; the hit index is the
// result. A miss falls back to 0xB when the kind is at most 0x4C, else 0.
u8 CEquipItemGrid::getEIBNameKey(CItemInstance* item) {
    u32 word = item->word;
    u32 v = (u32)lbl_eu_806640F4;
    u16 kind = BdatGetItemId(word >> 20);
    for (u8 i = 1; i <= 10; i++) {
        char buf[0x28];
        sprintf(buf, &lbl_eu_8050EFDC[0x7], i);
        if ((u8)BdatGetU8Direct((const void*)v, (const char*)buf, kind)) {
            return i;
        }
    }
    // Miss fallback: MWCC lowers the equality test as a subi/subic/subfe
    // carry chain masked with 0xB.
    return (kind == 0x4C) ? 0xB : 0;
}

CEIBCur::CEIBCur(nw4r::lyt::ArcResourceAccessor* arcResAcc)
    : mArcResAcc(arcResAcc), mpLayout(0), mpAnimTrans0(0), mpAnimTrans1(0), mActive(0), mVisible(0) {}

// Empty destructor (retail __dt__7CEIBCurFv); auto-inlined into the
// CEquipItemBox dtor, kept out of line for the ctor's DESTROYMEMBER action.
CEIBCur::~CEIBCur() {}

// setupEIBCur (us-80287e18): layout-cursor setup. Retail calls (relocs now
// resolved -- the old comment's "MetroTRK/undecompiled-TU" blocker is stale):
//   1. buildLayout(&mpLayout, mArcResAcc, "mf00_reg00_curs07.brlyt")
//   2. bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, "..._roop.brlan")
//   3. bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, "..._on.brlan")
//   4. virtual slot 36 (vtable+0x24) on mpLayout
//   5. cur->Idle()
// The name strings are pooled at lbl_eu_8050EFDC+0x97/0xAF/0xCC.
void CEIBCur::initLayout() {
    buildLayout((nw4r::lyt::Layout**)&mpLayout,
                  (nw4r::lyt::ArcResourceAccessor*)mArcResAcc,
                  &lbl_eu_8050EFDC[0x97]);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)mpLayout,
                  (nw4r::lyt::AnimTransform**)&mpAnimTrans0,
                  (nw4r::lyt::ArcResourceAccessor*)mArcResAcc,
                  &lbl_eu_8050EFDC[0xAF]);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)mpLayout,
                  (nw4r::lyt::AnimTransform**)&mpAnimTrans1,
                  (nw4r::lyt::ArcResourceAccessor*)mArcResAcc,
                  &lbl_eu_8050EFDC[0xCC]);
    ((nw4r::lyt::Layout*)mpLayout)->UnbindAllAnimation();
    Idle();
}

// Per-frame cursor update: play the entry animation (mpAnimTrans0) when idle,
// advance the page-cursor state when active, then animate the layout.
void CEIBCur::Update() {
    if (mpLayout == nullptr) return;
    switch (mVisible) {
    case 0:
        advanceAnimTransform((nw4r::lyt::AnimTransform*)mpAnimTrans0, lbl_eu_80668B28);
        break;
    case 1:
        StepAnim();
        break;
    }
    ((nw4r::lyt::Layout*)mpLayout)->Animate(0);
}

void CEIBCur::Draw(nw4r::lyt::DrawInfo* drawInfo) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)mpLayout;
    if (layout == nullptr) return;
    if (mActive == 0) return;
    drawLayout(layout, drawInfo, 0, 1);
}

// Reset a cursor object: stop it, clear its animation transforms, and release
// the bound layout if one is present.
// delete layout (not ~Layout()): the retail passes the deleting flag 1 to the
// virtual dtor; the delete's own null-check is the retail's dead second beq
// (beq skip-all from the guard, beq mpLayout-zero block from the delete).
void CEIBCur::Reset() {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)mpLayout;
    mActive = 0;
    mpAnimTrans0 = 0;
    mpAnimTrans1 = 0;
    if (layout != 0) {
        delete layout;
        mpLayout = 0;
    }
}

// Retail compiled getEIBByte2/80285B24/80285B70 as separate functions; keep
// them out-of-line so callers reproduce the retail `bl` (sibling-TU pattern).
void CEIBCur::StepAnim() {
    // Advance the layout's animation; when the animation finishes, hide the
    // cursor and update the page state.
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)mpAnimTrans1, lbl_eu_80668B28) != 0) {
        mVisible = 0;
        Idle();
    }
}

// Deactivate the cursor layout: unbind the second animation transform, bind
// the entry transform with playback enabled, reset its frame to 0 and step
// the layout animation.
void CEIBCur::Idle() {
    ((nw4r::lyt::Layout*)mpLayout)->UnbindAnimation((nw4r::lyt::AnimTransform*)mpAnimTrans1);
    ((nw4r::lyt::Layout*)mpLayout)->BindAnimation((nw4r::lyt::AnimTransform*)mpAnimTrans0);
    ((nw4r::lyt::Layout*)mpLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)mpAnimTrans0, true);
    ((nw4r::lyt::AnimTransform*)mpAnimTrans0)->SetFrame(lbl_eu_80668B04);
    ((nw4r::lyt::Layout*)mpLayout)->Animate(0);
}


CEIBPageCur::CEIBPageCur(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CEIBCur(arcResAcc) {}

// Destructor clone: free object memory when this is non-null and delete flag set.
// sites (mode -1) and would be elided entirely -- the retail keeps the `bl`.
CEIBPageCur::~CEIBPageCur() {}

// Bind the cursor's layout and entry animation (buildLayout/F08), position
// the two named panes at their .sdata2 offsets, unbind all layout animations,
// then hand the cursor to the deactivation helper (idleEIBCur).
void CEIBPageCur::initLayout() {
    buildLayout((nw4r::lyt::Layout**)&mpLayout, (nw4r::lyt::ArcResourceAccessor*)mArcResAcc, &lbl_eu_8050EFDC[0xe7]);
    bindLayoutAnimTransform((nw4r::lyt::Layout*)mpLayout, (nw4r::lyt::AnimTransform**)&mpAnimTrans0, (nw4r::lyt::ArcResourceAccessor*)mArcResAcc, &lbl_eu_8050EFDC[0xff]);

    float vec1[3];
    float vec2[3];
    code80135FDC_setVec3((nw4r::math::VEC3*)vec1, lbl_eu_80668B30, lbl_eu_80668B34, lbl_eu_80668B04);
    nw4r::lyt::Pane* pane1 =
        ((nw4r::lyt::Layout*)mpLayout)->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x11c], true);
    copyVEC3((float*)&pane1->GetTranslate(), vec1);

    code80135FDC_setVec3((nw4r::math::VEC3*)vec2, lbl_eu_80668B38, lbl_eu_80668B34, lbl_eu_80668B04);
    nw4r::lyt::Pane* pane2 =
        ((nw4r::lyt::Layout*)mpLayout)->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x128], true);
    copyVEC3((float*)&pane2->GetTranslate(), vec2);

    ((nw4r::lyt::Layout*)mpLayout)->UnbindAllAnimation();
    Idle();
}

// Construct every embedded sub-object in retail order: vtable, the two
// memory regions, the zeroed handle/layout/state fields, the three cursors,
// sort menu and both system windows. The member CSortMenu is re-assigned from
// a temporary, as are both
// system windows (via InfoCfCopySysWin). The grid is initialized in place and
// then overwritten with a fresh stack grid via a whole-struct assignment;
// finally seven CItemBlock_setFlag120EC preload calls run.
CEquipItemBox::CEquipItemBox()
    : field_24(0), field_28(0), field_2C(0), field_30(0), field_34(0), field_38(0), field_3C(0),
      unk_40(0), unk_41(0), unk_42(0), unk_43(1), mCur44(0), mPageCur(0), ccur18(0), mSysWin1(0),
      mSysWin2(0), unk_1f4(0), unk_1f5(0), unk_1f6(0), unk_1fa(0), unk_1fc(0), unk_1fe(0), unk_20c(0),
      unk_372(0), unk_373(0), unk_374(0), unk_375(0), unk_376(0), unk_377(0), unk_378(0), unk_379(0),
      unk_37a(0), unk_37b(1), unk_37c(0), field_27A4(0) {
    memset(unk_36c, 0, 6);
    mSortMenu = CSortMenu();
    {
        CSysWin temp(2);
        mSysWin1 = temp;
    }
    {
        CSysWin temp(0);
        mSysWin2 = temp;
    }
    ItemBlockSetDefaultFlags();
    mGrid = CEquipItemGrid();
    equipItemBoxEquipListFilePtr = 0;
    CItemBlock_setFlag120EC(2, 0);
    CItemBlock_setFlag120EC(4, 0);
    CItemBlock_setFlag120EC(5, 0);
    CItemBlock_setFlag120EC(6, 0);
    CItemBlock_setFlag120EC(7, 0);
    CItemBlock_setFlag120EC(8, 0);
    CItemBlock_setFlag120EC(3, 0);
}

// Destroy the embedded sub-objects in reverse construction order (retail
// __dt__13CEquipItemBoxFv): syswins, sort menu, cursors, then the two memory
// regions. MWCC supplies the this-null guard, the member-dtor dispatch and
// the flags>0 operator-delete tail from the dtor shape itself.
CEquipItemBox::~CEquipItemBox() {}

// Load the three equip-box bind files (two CDeviceFile reads + one common
// archive read) into the file-handle slots, initialise the sort menu at +0x8C,
// then dispatch the layout-build virtual on both embedded system windows.
void CEquipItemBox::loadEIBFiles() {
    field_24 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                           &lbl_eu_8050EFDC[0x134],
                                           this, 0, 0);
    CDeviceFile::setHandleFlag1((CFileHandle*)field_24);
    field_28 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                           &lbl_eu_8050EFDC[0x14d],
                                           this, 0, 0);
    CDeviceFile::setHandleFlag1((CFileHandle*)field_28);
    field_2C = CDeviceFile::readCommonArchiveFile(KyoshinHeap_GetField44(),
                                                        &lbl_eu_8050EFDC[0x166],
                                                        this, 0, 0);
    mSortMenu.Init();
    ((CEquipItemBoxSysWinView*)&mSysWin1)->v32();
    ((CEquipItemBoxSysWinView*)&mSysWin2)->v32();
}

// Per-frame update of the equip box: dispatch the entry-state machine by
// unk_41, animate the layout, then step all embedded cursors and windows.
void CEquipItemBox::updateEIBBox() {
    if (unk_40 != 0 && unk_41 != 0) {
        switch (unk_41) {
        case 1:
            eibPlayEntry();
            break;
        case 3:
            eibCloseAnim();
            break;
        case 4:
            eibSysWin1Done();
            break;
        case 5:
            if (reinterpret_cast<CSysWin*>(&mSysWin1)->isActive() != 0) {
                unk_41 = 2;
            }
            break;
        }
        field_38->Animate(0);
        mCur44.Update();
        mPageCur.Update();
        ccur18.Update();
        mSortMenu.Move();
        reinterpret_cast<CSysWin*>(&mSysWin1)->dispatchPhase();
        reinterpret_cast<CSysWin*>(&mSysWin2)->dispatchPhase();
    }
}

// Draw the equip box: layout + sort menu first, then the page cursor (when
// the grid spans more than one page), both system windows, and the selection
// cursor when every window is idle.
void CEquipItemBox::drawEIBBox(nw4r::lyt::DrawInfo* drawInfo) {
    if (unk_40 == 0) return;
    if (unk_41 == 0) return;
    drawLayout(field_38, drawInfo, 0, 1);
    mSortMenu.Draw(drawInfo);
    u8 v = mGrid._pad2003;
    u8 x = (v != 0) ? v : 1;
    if (x > 1) {
        mPageCur.Draw(drawInfo);
    }
    reinterpret_cast<CSysWin*>(&mSysWin1)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    if (unk_37b != 0 && reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() == 0) {
        mCur44.Draw(drawInfo);
    }
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() == 0) {
        if (mSortMenu.IsOpen() != 0 ||
            reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0 ||
            unk_375 != 0) {
            ccur18.DrawLayout(drawInfo);
        }
    }
    reinterpret_cast<CSysWin*>(&mSysWin2)->drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
}


// Close/release the equip-item box: unload bdat, free the three file handles,
// delete the layout object, release the arc accessors and both memory regions,
// then tear down the embedded cursors, sort menu and both system windows.
void CEquipItemBox::closeEIBBox() {
    CBdat::getEntry(5);
    closeFileHandle((CFileHandle**)&field_24);
    closeFileHandle((CFileHandle**)&field_28);
    closeFileHandle((CFileHandle**)&field_2C);
    unk_40 = 0;
    if (field_38) {
        delete field_38;
        field_38 = 0;
    }
    releaseArcResourceAccessor(field_30);
    releaseArcResourceAccessor(field_34);
    mRegion04.deleteRegion();
    mRegion14.deleteRegion();
    mCur44.Reset();
    mPageCur.Reset();
    ccur18.cleanup();
    mSortMenu.Term();
    reinterpret_cast<CSysWin*>(&mSysWin1)->termLayout();
    reinterpret_cast<CSysWin*>(&mSysWin2)->termLayout();
}

// Check the sort menu and both system windows are ready; return unk_42.
int CEquipItemBox::eibWindowsReady() {
    if (mSortMenu.IsReady() == 0) return 0;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isReady() == 0) return 0;
    // Inverted form: retail's last check falls through to the unk_42 return
    // and branches to the li 0 block (beq -> li0; fall -> lbz).
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isReady() != 0) return unk_42;
    return 0;
}


int CEquipItemBox::getEIBActiveMark() { return unk_43; }

// Return whether the equip box may be interacted with: a busy system window,
// an open cursor animation, or an active name pane all block further input.
int CEquipItemBox::eibInputBlocked() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return 1;
    if (unk_375 != 0) return 1;
    nw4r::lyt::Pane* pane =
        field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (pane != 0 && isPaneVisible(pane) != 0) return 1;
    return mSortMenu.IsOpen();
}

int CEquipItemBox::eibMenuBusy() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) {
        return 1;
    }
    return mSortMenu.IsOpen();
}

int CEquipItemBox::getEIBNamePane() { return unk_375; }

// Returns 1 when the first system window is active, otherwise queries the
// sort menu window's state.
int CEquipItemBox::eibSysWinBusy() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) {
        return 1;
    }
    return reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened();
}

// Initialise the equip item box state (idempotent via unk_41 guard).
void CEquipItemBox::openEIBBox() {
    if (unk_41 != 0) return;
    unk_41 = 1;
    unk_43 = 0;
    unk_1fe = 0;
    fillEIBSortMenu();
    unk_40 = 1;
    playUISound(0x6d);
}

// Close the box when the entry animation (unk_41 == 2) has finished: reset
// the state bytes, hide the name pane and stop the sort-menu sound effects.
void CEquipItemBox::finishEIBEntry() {
    if (unk_41 != 2) return;
    if (mSortMenu.IsOpen() != 0) return;
    unk_41 = 3;
    unk_43 = 0;
    mCur44.mActive = 0;
    mPageCur.mActive = 0;
    playUISound(0x6);
    nw4r::lyt::Pane* pane =
        field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (pane != 0) {
        setPaneVisible(pane, 0);
        unk_378 = 0;
    }
}

void CEquipItemBox::eibNavLeft() {
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) return;

    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) {
        // Syswin1 active: decrement page, wrap, rebuild cursor
        if (reinterpret_cast<CSysWin*>(&mSysWin1)->isActive() == 0) return;
        u8 val = unk_374 - 1;
        unk_374 = val;
        if ((s8)(u8)val < 0) unk_374 = 1;
        updateEIBCursorPos();
        playUISound(1);
        return;
    }

    // Syswin1 not active: check sort menu
    if (mSortMenu.IsOpen() != 0) {
        // Sort menu busy: close it and move cursor
        if (mSortMenu.IsIdle() == 0) return;
        mSortMenu.CursorUp();
        nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
        ccur18.setRootPaneTranslate(&pos);
    } else {
        // Sort menu idle: retail dispatches on unk_375 FIRST -- no sel==-1
        // gate here; that check happens in the caller/dispatch above.
        if (unk_375 != 0) {
            // Scan backward for next non-empty page. On a hit retail stores
            // pg back into unk_377 INSIDE the loop, and the cursor lookup
            // below uses the combined index unk_376*4 + unk_377.
            const u16* arr = (const u16*)(unk_20c + 0xb0);
            s8 pg = (s8)(unk_377 - 1);
            while (pg != (s8)unk_377) {
                if (pg < 0) pg = 3;
                if (ArrayGet12(arr, (u8)((s8)unk_376 * 4 + pg)) != 0) {
                    unk_377 = pg;
                    break;
                }
                pg--;
            }
            nw4r::math::VEC3 tmp;
            CopyTabSlotVec(&tmp, arr,
                          (u32)((u8)((s8)unk_376 * 4 + unk_377)));
            ccur18.setRootPaneTranslate((nw4r::math::VEC3*)&tmp);
        } else {
            // Decrement unk_1f5 with wrap
            u8 cur = unk_1f5;
            CEquipItemGrid* wrapGrid = &mGrid;
            u8 val = cur - 1;
            unk_1f5 = val;
            if ((s8)(u8)val < -1)
                unk_1f5 = wrapGrid->getEIBColCount() - 1;
            updateEIBCursorPos();
            refreshEIBItemList();
        }
    }
    playUISound(1);
}

// Down/confirm handler: page the syswin sub-window forward, finish the sort
// menu scroll, advance the name-pane column selection, or move the row
// cursor down (wrapping at 5 with a grid-page step when it wraps).
void CEquipItemBox::eibNavDown() {
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&mSysWin1)->isActive() != 0) {
            u8 v = unk_374 + 1;
            unk_374 = v;
            if ((s8)v > 1) unk_374 = 0;
            updateEIBCursorPos();
            playUISound(1);
        }
        return;
    }
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() != 0) {
            mSortMenu.CursorDown();
            nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
            ccur18.setRootPaneTranslate(&pos);
            playUISound(1);
            return;
        }
        // Scroll finished: retail falls through to the name-pane dispatch.
    }
    if (unk_375 != 0) {
        // Name pane is up: scan the item list forward for the next column
        // (of 4) whose first entry is non-empty and move the cursor there.
        u16* arr = (u16*)(unk_20c + 0xb0);
        s8 i = (s8)(unk_377 + 1);
        while (i != (s8)unk_377) {
            if ((s8)i >= 4) i = 0;
            if (ArrayGet12(arr,
                            (u8)((s8)unk_377 + (s8)unk_376 * 4 + i)) != 0) {
                unk_377 = (u8)i;
                break;
            }
            i++;
        }
        nw4r::math::VEC3 tmp2;
        CopyTabSlotVec(&tmp2, arr,
                      (u8)((s8)unk_377 + (s8)unk_376 * 4));
        ccur18.setRootPaneTranslate((nw4r::math::VEC3*)&tmp2);
        playUISound(1);
        return;
    }
    u8 v = unk_1f5 + 1;
    unk_1f5 = v;
    if ((s8)v >= mGrid.getEIBColCount()) {
        unk_1f5 = -1;
    }
    updateEIBCursorPos();
    refreshEIBItemList();
    playUISound(1);
}

// Up/back handler
// syswin window / sort menu scroll / name-pane columns / row cursor.
void CEquipItemBox::eibNavUp() {
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return;
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() != 0) {
            mSortMenu.PageUp();
            nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
            ccur18.setRootPaneTranslate(&pos);
            playUISound(1);
        }
        return;
    }
    if (unk_375 != 0) {
        // Name pane is up: scan the item list backward for the previous
        // non-empty column (of 3) and move the cursor there.
        u16* arr = (u16*)(unk_20c + 0xb0);
        s8 j = (s8)(unk_376 - 1);
        while (j != (s8)unk_376) {
            if ((s8)j < 0) j = 2;
            if (ArrayGet12(arr,
                            (u8)(unk_377 + j * 4)) != 0) {
                unk_376 = (u8)j;
                break;
            }
            j--;
        }
        nw4r::math::VEC3 tmp2;
        CopyTabSlotVec(&tmp2, arr,
                      (u8)(unk_377 + (s8)unk_376 * 4));
        ccur18.setRootPaneTranslate((nw4r::math::VEC3*)&tmp2);
        playUISound(1);
        return;
    }
    if ((s8)unk_1f5 == -1) {
        if (unk_372 > 1) {
            eibSortPagePrev();
            updateEIBCursorPos();
        }
        return;
    }
    u8 b = unk_1f4;
    if ((s8)b == 0) {
        unk_1f4 = 4;
        eibPagePrev();
        updateEIBCursorPos();
    } else {
        u8 v = (u8)(b - 1);
        unk_1f4 = v;
        if ((s8)v < 0) unk_1f4 = 4;
        updateEIBCursorPos();
        refreshEIBItemList();
    }
    playUISound(1);
}

// Equip-box button dispatch (down/confirm handler): while both system windows
// are idle, either finish the sort-menu scroll (move the selection cursor to
// the menu state and play the confirm sound), page forward through the item
// list when the name pane is up, step the sort-menu page when unk_1f5 is -1,
// or advance the row/column selection and refresh the list.
void CEquipItemBox::eibNavRight() {
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return;
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() == 0) return;
        mSortMenu.PageDown();
        nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
        ccur18.setRootPaneTranslate(&pos);
        playUISound(1);
        return;
    }
    if (unk_375 != 0) {
        // Name pane is up: scan the item list forward for the next non-empty
        // page (wrapping at 3 pages) and move the selection cursor to it.
        u16* arr = (u16*)(unk_20c + 0xb0);
        s8 i = (s8)(unk_376 + 1);
        while (i != (s8)unk_376) {
            if (i >= 3) i = 0;
            if (ArrayGet12(arr, (u8)(unk_377 + i * 4)) != 0) {
                unk_376 = (u8)i;
                break;
            }
            i++;
        }
        nw4r::math::VEC3 tmp2;
        CopyTabSlotVec(&tmp2, arr, (u8)(unk_377 + (s8)unk_376 * 4));
        ccur18.setRootPaneTranslate((nw4r::math::VEC3*)&tmp2);
        playUISound(1);
        return;
    }
    if ((s8)unk_1f5 == -1) {
        if (unk_372 > 1) {
            eibSortPageNext();
            updateEIBCursorPos();
        }
        return;
    }
    if ((int)unk_1f4 == 4) {
        unk_1f4 = 0;
        eibPageNext();
        updateEIBCursorPos();
    } else {
        u8 v = (u8)(unk_1f4 + 1);
        unk_1f4 = v;
        if ((s8)v >= 5) unk_1f4 = 0;
        updateEIBCursorPos();
        refreshEIBItemList();
    }
    playUISound(1);
}

// Advance the equip-grid page cursor when the box is idle; wrap the page
// index at the grid extent and play the page-change sound for multi-page
// lists.
void CEquipItemBox::eibPageNext() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return;
    if (mSortMenu.IsOpen() != 0) return;
    CEquipItemGrid* grid = &mGrid;
    grid->eibGridNext();
    if ((s8)unk_1f5 >= grid->getEIBColCount()) {
        unk_1f5 = (u8)(grid->getEIBColCount() - 1);
    }
    refreshEIBPageDisp();
    refreshEIBItemList();
    u8 c = grid->_pad2003;
    u8 v = (c != 0) ? c : 1;
    if (v != 1) {
        playUISound(0xa);
    }
}

// Step the equip-grid page cursor backward when the box is idle; wrap the
// page index at the grid extent and play the page-change sound for multi-page
// lists (backward mirror of eibPageNext).
void CEquipItemBox::eibPagePrev() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return;
    if (mSortMenu.IsOpen() != 0) return;
    CEquipItemGrid* grid = &mGrid;
    grid->eibGridPrev();
    if ((s8)unk_1f5 >= grid->getEIBColCount()) {
        unk_1f5 = (u8)(grid->getEIBColCount() - 1);
    }
    refreshEIBPageDisp();
    refreshEIBItemList();
    u8 c = grid->_pad2003;
    u8 v = (c != 0) ? c : 1;
    if (v != 1) {
        playUISound(0xa);
    }
}

// Equip-box confirm: while the sort menu is busy, close the page state and
// re-open; otherwise move the sort menu to the cursor position and start its
// scroll (page 2 sound when the call came from the sub-page).
void CEquipItemBox::eibConfirmSort() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return;
    if (unk_375 != 0) return;
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() == 0) return;
        updateEIBCursorPos();
        mCur44.mActive = 1;
        ccur18.SetVisible(0);
        mSortMenu.Close();
        playUISound(6);
        return;
    }
    if (mSortMenu.IsIdle() == 0) return;
    nw4r::math::VEC3 result;
    nw4r::lyt::Pane* root = field_38->GetRootPane();
    // Args evaluated right-to-left by MWCC: paneA's lookup runs first and its
    // result stays live across paneB's call (retail saves it to r31).
    ScalePaneWorldPos(&result,
                  root->FindPaneByName(&lbl_eu_8050EFDC[0x18a], true),
                  root->FindPaneByName(&lbl_eu_8050EFDC[0x193], true),
                  root);
    mSortMenu.SetPos(&result);
    mSortMenu.SetSelect((u8)(unk_379 + unk_37a));
    mCur44.mActive = 0;
    nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
    ccur18.setRootPaneTranslate(&pos);
    ccur18.SetVisible(1);
    mSortMenu.Open();
    unk_1f6 = 0;
    playUISound(2);
}

// Equip-box sub-page handler: while the first system window is active switch
// to page 2 of the sort menu; otherwise with the second window active step it;
// with both idle either re-open the busy sort menu or hide the name pane, and
// play the confirm sound when the call had no explicit sound flag.
void CEquipItemBox::eibHandleSubPage(int param) {
    if (unk_41 != 2) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&mSysWin1)->isActive() != 0) {
            unk_41 = 5;
            reinterpret_cast<CSysWin*>(&mSysWin1)->advancePhase3();
            ccur18.SetVisible(0);
            mCur44.mActive = 1;
            unk_374 = 0;
        }
        return;
    }
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) {
        if (reinterpret_cast<CSysWin*>(&mSysWin2)->isActive() != 0) {
            reinterpret_cast<CSysWin*>(&mSysWin2)->advancePhase3();
        }
        return;
    }
    nw4r::lyt::Pane* pane =
        field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() == 0) return;
        updateEIBCursorPos();
        mCur44.mActive = 1;
        ccur18.SetVisible(0);
        mSortMenu.Close();
    } else if (unk_375 != 0) {
        unk_375 = 0;
        mCur44.mActive = 1;
        ccur18.SetVisible(0);
    } else if (isPaneVisible(pane) != 0) {
        setPaneVisible(pane, 0);
        unk_378 = 0;
    }
    if (param == 0) {
        playUISound(6);
    }
}

// Main confirm-button handler (state 2, name pane down). Three top-level
// paths: (1) syswin1 open -> close it, optionally run useEIBSelectedItem;
// (2) sort menu open -> commit selection, reset cursor state; (3) neither ->
// item move/equip dispatch for the selected cell. Sets unk_1fe when an
// action fired and the page table reports 8 equipped slots.
void CEquipItemBox::handleEIBConfirm() {}

// Advance page-2 system-window state while it is visible and active.
void CEquipItemBox::stepEIBSysWin2() {
    if (unk_375 != 0 && reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0
        && reinterpret_cast<CSysWin*>(&mSysWin2)->isActive() != 0) {
        reinterpret_cast<CSysWin*>(&mSysWin2)->advancePhase3();
    }
}

// Rebuild the sort-menu page list: stash the input value/count, pick the
// current page byte (cursor page when flag is set, else the value's bits 4-7),
// reset the page list, then repopulate it (page 3 alone, otherwise pages
// 2 + 4..8) and restore the page cursor for the 4..8 band.
void CEquipItemBox::eibOpenPages(u32 v, void* w, int flag) {
    unk_1fc = (u16)v;
    field_27A4 = w;
    u32 raw;
    if (flag != 0) {
        raw = unk_36c[(s8)unk_373];
    } else {
        raw = (v >> 4) & 0xF;
    }
    u8 x = (u8)raw;
    clearEIBCatList();
    if (x == 3) {
        pushEIBCatList(3);
    } else {
        pushEIBCatList(2);
        pushEIBCatList(4);
        pushEIBCatList(5);
        pushEIBCatList(6);
        pushEIBCatList(7);
        pushEIBCatList(8);
        switch (x) {
        case 4: unk_373 = 1; break;
        case 5: unk_373 = 2; break;
        case 6: unk_373 = 3; break;
        case 7: unk_373 = 4; break;
        case 8: unk_373 = 5; break;
        }
    }
    refreshEIBSortTabs();
    rebuildEIBPage(1);
}

int CEquipItemBox::takeEIBAction() {
    u8 val = unk_1fe;
    unk_1fe = 0;
    return val;
}

void CEquipItemBox::setEIBBoxInfo(u32 val) { unk_20c = val; }

// Advance the equip-grid selection: map the low nibble of unk_1fc to a window
// kind (2..8), then hand the page grid, the selected page byte, the input
// category and the kind to rebuildEIBGridPage and refresh the page list.
void CEquipItemBox::eibApplySelect(u32 param, void* item) {
    u8 arg = (param >> 8) & 0xFF;
    u8 b = (param != 0) ? GetCollectedFlagByte(arg) : 0;
    CEquipItemGrid* grid = &mGrid;
    u8 lo = unk_1fc & 0xF;
    u8 hi = (unk_1fc >> 4) & 0xF;
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
    grid->rebuildEIBGridPage(unk_36c[(s8)unk_373], b, hi);
    refreshEIBPageDisp();
}

// Name-pane confirm handler (entry states 2/4): with the name pane visible,
// either page the second system window or jump the item-box selection to the
// column chosen by unk_376/unk_377; otherwise reveal the pane and pick the
// first non-empty list column (column 8 preferred when populated).
void CEquipItemBox::handleEIBNamePane() {
    if (unk_41 != 2 && unk_41 != 4) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return;
    if (mSortMenu.IsOpen() != 0) return;
    nw4r::lyt::Pane* pane =
        field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    if (pane == 0) return;
    // Retail dispatches the VISIBLE-pane cases immediately and defers the
    // invisible-pane handler to a tail block after them.
    if (isPaneVisible(pane) != 0) {
        if (unk_375 != 0) {
            // Name pane up: close the sub-window or commit its selection.
            if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) {
                if (reinterpret_cast<CSysWin*>(&mSysWin2)->isActive() != 0) {
                    reinterpret_cast<CSysWin*>(&mSysWin2)->advancePhase3();
                }
                return;
            }
            CItemBoxPage* list =
                &((CItemBoxInfo*)unk_20c)->mPage;
            u8 idx = (u8)(unk_377 + unk_376 * 4);
            u16 v = ArrayGet12(&list->mItemIds[0], idx);
            if (v == 0) return;
            void* text;
            if (ItemBoxGetTab((const u8*)&list->mItemIds[0], idx) == 3) {
                text = ((CItemBoxPage*)(list))->GetItemName(idx);
            } else {
                text = BdatGetPtrDirect((const void*)equipItemBoxEquipListFilePtr,
                                     &lbl_eu_8050EFDC[0x1a2], v);
            }
            reinterpret_cast<CSysWin*>(&mSysWin2)->switchKindPane(0);
            reinterpret_cast<CSysWin*>(&mSysWin2)->setKindContent(reinterpret_cast<char*>(text), 0);
            reinterpret_cast<CSysWin*>(&mSysWin2)->applyKind(1);
            reinterpret_cast<CSysWin*>(&mSysWin2)->openPhase1();
            mCur44.mActive = 0;
            return;
        }
        // List-selection refresh: pick the first non-empty column (column 8
        // wins whenever it holds any item), scale the row into a page/column
        // pair via the 0x4330 magic, move the cursor, mark state.
        if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) return;
        CItemBoxPage* list =
            &((CItemBoxInfo*)unk_20c)->mPage;
        int found = 0;
        for (u8 i = 0; i < 0xc; i++) {
            if (list->mItemIds[i] != 0) {
                list->mSelSlot = i;
                if (list->mItemIds[8] != 0) {
                    list->mSelSlot = 8;
                }
                found = 1;
                break;
            }
        }
        if (found == 0) {
            playUISound(5);
            return;
        }
        u8 sel = list->mSelSlot;
        s32 pos = (s32)(0.25f * (f32)(u32)sel);
        mCur44.mActive = 0;
        unk_375 = 1;
        unk_376 = (u8)pos;
        unk_377 = (u8)(sel - (s8)pos * 4);
        ccur18.SetVisible(1);
        nw4r::math::VEC3 tmp;
        CopyTabSlotVec(&tmp, &list->mItemIds[0], sel);
        ccur18.setRootPaneTranslate((nw4r::math::VEC3*)&tmp);
        unk_378 = 1;
        playUISound(2);
        return;
    }
    // Invisible pane: show it (unless syswin2 is busy), then sound state 2.
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) return;
    setPaneVisible(pane, 1);
    playUISound(2);
}

// Arm-check for the "register/equip item" action: clear the armed flag
// (unk_37c), then run the gate chain - sort menu closed, current cell has an
// item with a kind and a category, category < 9, not the page-2 same-category
// case. When all pass, build the per-page slot table from the item view's
// six slot-id shorts, look up the selected page's entry, resolve its item,
// check its name-table kind is not 1 and that sound 0x3508 is not playing;
// only then set unk_37c = 1. Returns 1 = action blocked, 0 = ok/armed.
int CEquipItemBox::isEIBSelectBlocked() {
    unk_37c = 0;
    if (mSortMenu.IsOpen() != 0) return 1;
    CEquipItemGrid* grid = &mGrid;
    u8 idx = (u8)(unk_1f4 + unk_1f5 * 5);
    if (grid->getEIBByte6(idx) == 0) return 0;
    if (grid->getEIBItemKind(idx) == 0) return 0;
    u8 cat = (u8)((unk_1fc >> 8) & 0xff);
    u8 catB = (u8)GetCollectedFlagByte(cat);
    CItemBoxCharEntry* cfg = (CItemBoxCharEntry*)CtrlObjectParam_GetCharEntry(catB);
    if (catB >= 9) return 0;
    u8 page = unk_36c[(s8)unk_373];
    if (page == 2 && (u8)grid->getEIBByte2(idx) == catB) return 0;

    // Per-page slot table: zeroed words at [0],[4],[8],[12], slot ids patched
    // in from the item view.
    s16 table[14] = {0};
    table[2] = cfg->slotIds[5];
    table[4] = cfg->slotIds[0];
    table[5] = cfg->slotIds[1];
    table[6] = cfg->slotIds[2];
    table[7] = cfg->slotIds[3];
    table[8] = cfg->slotIds[4];
    // The selected entry; -1 means "no item" (item stays null). Retail
    // passes the selected unk_36c[] byte itself as the category argument.
    s16 elem = table[page];
    void* item = 0;
    if (elem != -1) {
        item = CItem_kindPtr(page, elem);
    }
    if (item == 0) return 1;
    u32 w = *(u32*)item;
    if (w == 0) return 1;
    u8 v = (u8)BdatGetU8Direct((const void*)lbl_eu_806640EC, &lbl_eu_8050EFDC[0x20],
                             w >> 20);
    if ((((u32)(s8)(v - 1)) >> 5) == 0) return 1;
    if (CtrlRemote_TouchBitByArg(0x3508) != 0) return 1;
    unk_37c = 1;
    return 0;
}

// Test the equip-grid cursor row: return 1 when the selected cell holds the
// expected item category (kind >= 3) at the current page, else 0.
int CEquipItemBox::eibRowMatches() {
    u8 idx;
    CEquipItemGrid* grid = &mGrid;
    idx = (u8)(unk_1f4 + unk_1f5 * 5);
    if (grid->getEIBByte3(idx) == 0) return 0;
    u8 cat = (u8)((unk_1fc >> 8) & 0xff);
    u8 b = GetCollectedFlagByte(cat);
    u8 a = (u8)grid->getEIBByte2(idx);
    if (b != a) return 0;
    u16 kind = (u16)grid->getEIBItemKind(idx);
    u8 v = (u8)BdatGetItemType(kind);
    return (v - 2) != 0;
}

int CEquipItemBox::getEIBSelCat() {
    s8 idx = (s8)unk_373;
    return unk_36c[idx];
}

// Scan the 6 equipment slots for the item that owns the slot id selected by
// the current grid row. For each category definition, look up the item
// object, then walk its instance list for a slot whose id matches the grid
// row's item; the result is the global slot number (per-category base byte +
// instance index), or 0 when no slot matches.
u32 CEquipItemBox::findEIBEquipSlot() {
    s16 target;  // declared before items so MWCC colors it r31 (retail order)
    CItemBoxCharEntry* items;
    items = (CItemBoxCharEntry*)CtrlObjectParam_GetCharEntry(GetCollectedFlagByte((unk_1fc >> 8) & 0xFF));
    u8 idx = (u8)((u8)unk_1f5 * 5 + unk_1f4);
    target = mGrid.getEIBItemId(idx);
    CEquipItemBoxSlotTable table = equipItemBoxSlotDefsTable;
    table.slots[0].item = items->slotIds[5];
    table.slots[1].item = items->slotIds[0];
    table.slots[2].item = items->slotIds[1];
    table.slots[3].item = items->slotIds[2];
    table.slots[4].item = items->slotIds[3];
    table.slots[5].item = items->slotIds[4];
    for (u8 e = 0; e < 6; e++) {
        s16 item = table.slots[e].item;
        if (item == -1) continue;
        CItemInstance* obj = (CItemInstance*)CItem_kindPtr(table.slots[e].cat, item);
        if (obj == 0) continue;
        if (obj->word == 0) continue;
        u8 count = (u8)CItem_initItemImplInstances((CItemData*)obj)->vf30((CItemData*)obj);
        for (u8 i = 0; i < count; i++) {
            int slot = (s16)CItem_initItemImplInstances((CItemData*)obj)
                           ->vf40((CItemData*)obj, i);
            if (slot == -1) continue;
            if (slot == target) return (u8)(i + table.slots[e].extra);
        }
    }
    return 0;
}

// us-8028ab5c: guarded teardown -- skips while the sort/syswin sub-objects are
// busy; on success flags unk_1f5 = -1 and runs the close sequence
// (updateEIBCursorPos, refreshEIBItemList, playUISound(2)).
void CEquipItemBox::eibTryCloseRow() {
    if ((s8)unk_1f5 == -1) return;
    if (unk_375 != 0) return;
    if (mSortMenu.IsOpen() != 0) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) return;
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return;
    unk_1f5 = -1;
    updateEIBCursorPos();
    refreshEIBItemList();
    playUISound(2);
}

// State probe used by the HUD/button layer: returns a code describing what
// the equip box is currently showing (idle/menu/name-pane combinations plus
// the pane visibility of the confirm label and the getByte gate).
int CEquipItemBox::eibHudPrompt() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) return 0;
    if (reinterpret_cast<CSysWin*>(&mSysWin2)->isOpened() != 0) return 0;
    // MWCC lowers both flags via the subfic/srwi / bool-move idioms.
    bool manyParty = code80135FDC_getByte_64077() > 1;
    nw4r::lyt::Pane* pane =
        field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x182], true);
    bool vis = isPaneVisible(pane);
    if (mSortMenu.IsOpen() != 0) return 0x2b;
    if (unk_375 != 0) return 0x22;
    if ((s8)unk_1f5 == -1) {
        u32 cat = unk_36c[(s8)unk_373];
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
        u32 cat = unk_36c[(s8)unk_373];
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

// Returns 1 when the lookup byte of the grid cell selected by (unk_1f5) equals
// GetCollectedFlagByte's category byte; an unk_1f5 of -1 always returns 0.
int CEquipItemBox::isEIBCatMatch() {
    if ((s8)unk_1f5 == -1) return 0;
    u8 cat = (u8)((unk_1fc >> 8) & 0xff);
    CEquipItemGrid* grid = &mGrid;
    u32 a = grid->getEIBByte2((u8)(unk_1f4 + unk_1f5 * 5));
    u32 b = GetCollectedFlagByte(cat);
    return b == a;
}

// Play the entry animation at 0x3C; when it completes mark the cursors active.
void CEquipItemBox::eibPlayEntry() {
    float f = lbl_eu_80668B28;
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)field_3C, f) != 0) {
        unk_41 = 2;
        unk_43 = 1;
        mCur44.mActive = 1;
        mPageCur.mActive = 1;
        updateEIBCursorPos();
    }
}

// Playback of the layout animation at 0x3C; when it finishes reset the state
// bytes 0x40-0x43 (unk_43=active-mark, unk_41/unk_40 cleared).
void CEquipItemBox::eibCloseAnim() {
    if (AnimRewindFrame((nw4r::lyt::AnimTransform*)field_3C, lbl_eu_80668B28) != 0) {
        unk_43 = 1;
        unk_41 = 0;
        unk_40 = 0;
    }
}

// When the first system window is active, switch page-cursor state and refresh.
void CEquipItemBox::eibSysWin1Done() {
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isActive() != 0) {
        unk_41 = 2;
        ccur18.SetVisible(1);
        updateEIBCursorPos();
    }
}

// If both pointer fields 0x34 and 0x38 are non-null, clear the sort menu
// (passing this) and set unk_42; retail callers (OnFileEvent) emit a `bl`
// to the unmangled retail name.
void CEquipItemBox::closeEIBSortMenu() {
    if (field_38 == 0) return;
    if (!(field_34 == 0)) {
        BuildSlotItemLists(0);
        unk_42 = 1;
    }
}

// Per-cell texture bind: kind-3 items resolve an id-keyed icon first, then
// the generic kind lookup; the b7 flag finally overrides with one of two
// fixed textures selected by the current sort page.
void CEquipItemBox::bindEIBCellTexture(u16 kind, void* itemPtr,
                                                   u8 i, u8 b7) {
    char buf[0x20];
    // Retail reuses one register for the icon name and the bound resource.
    void* res = 0;
    if (itemPtr != 0 && ((((CItemInstance*)itemPtr)->word >> 12) & 0xF) == 3) {
        u32 id = (u32)CItem_initItemImplInstances((CItemData*)itemPtr)
                     ->vf08((CItemData*)itemPtr);
        switch ((u8)BdatGetU8Direct((const void*)lbl_eu_806640D8, &lbl_eu_8050EFDC[0x1a7], (u16)id)) {
        case 4: res = MakeTplNameSysFile(0x144); break;
        case 5: res = MakeTplNameSysFile(0x145); break;
        case 6: res = MakeTplNameSysFile(0x146); break;
        case 7: res = MakeTplNameSysFile(0x147); break;
        case 8: res = MakeTplNameSysFile(0x148); break;
        case 9: res = MakeTplNameSysFile(0x149); break;
        }
        if (res != 0) {
            res = field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, (char*)res, 0);
        } else {
            res = field_30->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1b0], 0);
        }
    }
    if (res == 0) {
        if (kind != 0) {
            res = MakeTplNameSysFile(
                BdatGetU16Direct((const void*)lbl_eu_806640EC, &lbl_eu_8050EFDC[0x1c3], kind));
            res = field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, (char*)res, 0);
            if (res == 0) {
                res = field_30->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_8050EFDC[0x1b0], 0);
            }
        } else {
            res = field_30->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1b0], 0);
        }
    }
    if (b7 != 0) {
        // b7 override: page 3 uses the alternate texture unconditionally.
        if (unk_36c[(s8)unk_373] == 3) {
            res = field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1cd], 0);
        } else {
            res = field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1e3], 0);
        }
    }
    if (res != 0) {
        sprintf(buf, &lbl_eu_8050EFDC[0x1f9], i + 1);
        PaneSetTexPaletteByName(field_38, (const char*)buf, (TPLPalette*)((u32)res));
    }
}

// Item-kind refresh helper: format the textbox pane name for cell i, then
// build its text - items of kind 3/9 append a per-slot count suffix, positive
// cell states show the carry-count string, negative states show error text.
// bufName lands at sp+0x28 and bufText at sp+0x8 in retail (declaration order).
void CEquipItemBox::refreshEIBCellText(s8 v,
                                                   CItemInstance* item, u8 i) {
    char bufName[0x20];
    char bufText[0x20];
    // Retail evaluates the null check before any buffer setup, but defers
    // the item->word load until after the name sprintf.
    CItemInstance* p = (item != 0) ? item : 0;
    sprintf(bufName, &lbl_eu_8050EFDC[0x219], i + 1);
    u32 kind = (p != 0) ? ((p->word >> 16) & 0xF) : 0;
    if (kind == 3 || kind == 9) {
        CItemImpl* impl = CItem_initItemImplInstances((CItemData*)item);
        u32 n = impl->vf08((CItemData*)item);
        char* str = BdatTouchStringCell(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36],
                                  0x1e - ((u8)n - 1));
        sprintf(bufText, &lbl_eu_8050EFDC[0x2a], str);
        PaneMatSetTevColorsByName(field_38, bufName, (const GXColorS10*)equipItemBox_sortOptionLabelNeutralTop, (const GXColorS10*)equipItemBox_sortOptionLabelNeutralBottom);
    } else if (v > 0) {
        sprintf(bufText, &lbl_eu_8050EFDC[0x225]);
        PaneMatSetTevColorsByName(field_38, bufName, (const GXColorS10*)equipItemBox_sortOptionLabelNeutralTop, (const GXColorS10*)equipItemBox_sortOptionLabelNeutralBottom);
    } else if (v == 0) {
        sprintf(bufText, &lbl_eu_8050EFDC[0x228]);
    } else if (v == -2) {
        sprintf(bufText, getErrMesText16());
        PaneMatSetTevColorsByName(field_38, bufName, (const GXColorS10*)equipItemBox_sortOptionLabelSelectedTop, (const GXColorS10*)equipItemBox_sortOptionLabelSelectedBottom);
    } else if (v == -3) {
        sprintf(bufText, getErrMesText15());
        PaneMatSetTevColorsByName(field_38, bufName, (const GXColorS10*)equipItemBox_sortOptionLabelAltTop, (const GXColorS10*)equipItemBox_sortOptionLabelAltBottom);
    } else {
        sprintf(bufText, &lbl_eu_8050EFDC[0x228]);
    }
    LayoutSetTextBoxText(field_38, bufName, bufText, 0);
}

void CEquipItemBox::bindEIBCellIcon(u16 kind, void* item, u8 i) {
    // Pick the icon texture for the grid cell: item-kind-specific icons first
    // (vf08 select), falling back to the generic name lookup by kind.
    void* obj = item != 0 ? item : 0;
    void* res = 0;
    if (obj != 0) {
        if (((((CItemInstance*)obj)->word >> 16) & 0xF) == 3) {
            char* name = 0;
            u16 id =
                (u16)CItem_initItemImplInstances((CItemData*)obj)->vf08((CItemData*)obj);
            switch (id) {
            case 1: name = MakeTplNameSysFile(0x197); break;
            case 2: name = MakeTplNameSysFile(0x196); break;
            case 3: name = MakeTplNameSysFile(0x195); break;
            case 4: name = MakeTplNameSysFile(0x194); break;
            case 5: name = MakeTplNameSysFile(0x193); break;
            case 6: name = MakeTplNameSysFile(0x192); break;
            }
            if (name != 0) {
                res = field_34->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, name, 0);
            } else {
                res = field_30->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_8050EFDC[0x1b0], 0);
            }
        } else {
            int check = IsSkillItem(obj);
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
                    (u16)CItem_initItemImplInstances((CItemData*)obj)->vf08((CItemData*)obj);
                switch (id) {
                case 1: name = MakeTplNameSysFile(0x191); break;
                case 2: name = MakeTplNameSysFile(0x190); break;
                case 3: name = MakeTplNameSysFile(0x18f); break;
                case 4: name = MakeTplNameSysFile(0x18e); break;
                case 5: name = MakeTplNameSysFile(0x18d); break;
                }
                if (name != 0) {
                    res = field_34->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, name, 0);
                } else {
                    res = field_30->GetResource(
                        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                        &lbl_eu_8050EFDC[0x1b0], 0);
                }
            }
        after_switch:;
        }
    }
    if (res == 0) {
        if (kind != 0) {
            char* name = MakeTplNameSysFile((u16)BdatGetU16Direct((const void*)lbl_eu_806640EC, &lbl_eu_8050EFDC[0x207], kind));
            res = field_34->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, name, 0);
            if (res == 0) {
                res = field_30->GetResource(
                    nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                    &lbl_eu_8050EFDC[0x1b0], 0);
            }
        } else {
            res = field_30->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                &lbl_eu_8050EFDC[0x1b0], 0);
        }
    }
    if (res != 0) {
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050EFDC[0x20c], i + 1);
        PaneSetTexPaletteByName(field_38, (const char*)buf, (TPLPalette*)((u32)res));
    }
}

// Equip-box flavour handler: (re)bind the item texture named by the current
// page/state into the layout, set its visibility pane, and refresh the
// name-pane text. mode 1 picks a per-flag texture, mode 2 a fixed one;
// otherwise the default page texture is used.
void CEquipItemBox::bindEIBCellMark(u32 mode, int num, int flag, u32 show) {
    char buf[0x20];
    int n = num + 1;
    sprintf(buf, &lbl_eu_8050EFDC[0x229], n);
    void* res = field_30->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x1b0], 0);
    if (mode == 1) {
        if (flag != 0) {
            res = field_30->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x234], 0);
        } else {
            res = field_30->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x248], 0);
        }
    } else if (mode == 2) {
        res = field_30->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, &lbl_eu_8050EFDC[0x25c], 0);
    }
    if (res != 0) {
        PaneSetTexPaletteByName(field_38, (const char*)buf, (TPLPalette*)((u32)res));
    }
    sprintf(buf, &lbl_eu_8050EFDC[0x270], n);
    nw4r::lyt::Pane* pane = field_38->GetRootPane()->FindPaneByName(buf, true);
    setPaneVisible(pane, show == 0 ? 1 : 0);
}

// Rebuild the equip item box for a page change: map the selection window kind
// from unk_1fc's low nibble, dispatch the grid rebuild, and when the caller
// asks (a != 0) and a target item is pending (field_27A4), scan the 5x6 grid
// for that item (advancing the page cursor and rescanning when absent), then
// refresh the list, layout text and item-box selection.
void CEquipItemBox::rebuildEIBPage(int a) {
    CEquipItemGrid* grid = &mGrid;
    u8 page = unk_36c[(s8)unk_373];
    u8 b;
    u8 cat = (u8)((unk_1fc >> 8) & 0xFF);
    if (unk_1fc != 0) b = (u8)GetCollectedFlagByte(cat);
    else b = 0;
    u8 lo = (u8)(unk_1fc & 0xF);
    u8 hi = (u8)((unk_1fc >> 4) & 0xF);
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
    grid->rebuildEIBGridPage(page, b, hi);
    if (a != 0 && field_27A4 != 0) {
        grid->idx = 0;
        int found = 0;
        unk_1f4 = 0;
        unk_1f5 = 0;
        do {
            for (u8 r = 0; r < 5; r++) {
                for (u8 c = 0; c < 6; c++) {
                    void* item = grid->getEIBItemObj((u8)(r + c * 5));
                    if (field_27A4 == item) {
                        unk_1f4 = r;
                        found = 1;
                        unk_1f5 = c;
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
            grid->eibGridNext();
        } while (1);
        field_27A4 = 0;
    }
    u8 idx2 = (u8)(unk_1f4 + unk_1f5 * 5);
    void* item2 = grid->getEIBItemObj(idx2);
    ((CItemBoxInfo*)(unk_20c))->SetItem(unk_1fc, item2, 1);
    ((CItemBoxInfo*)(unk_20c))->SetNamedText(1, grid->fmtEIBItemName(idx2));
    u32 x = grid->countEIBPages();
    setLayoutTextBoxNumber(field_38, &lbl_eu_8050EFDC[0x27c], (u8)x);
    refreshEIBPageDisp();
    refreshEIBItemList();
    fillEIBSortMenu();
    if (((CItemBoxInfo*)((void*)unk_20c))->GetState() != 0) {
        ((CItemBoxInfo*)(unk_20c))->HidePanes(page);
    }
}

// Refresh the equip-box page display: for multi-page grids bind the page
// cursor name panes (10 per page) with the first `pages` visible, move the
// cursor pane next to the current row, then refresh the 6 column panes and
// dispatch the per-cell refresh helpers for the whole 5x6 grid.
void CEquipItemBox::refreshEIBPageDisp() {
    u32 vec[3];
    char buf2[0x20];
    char buf1[0x20];
    u8 b = GetCollectedFlagByte((unk_1fc >> 8) & 0xFF);
    CEquipItemGrid* grid = &mGrid;
    u8 pages = grid->countEIBPages();
    if (pages > 1) {
        nw4r::lyt::Pane* pane =
            field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x285], true);
        setPaneVisible(pane, 1);
        for (u8 i = 0; i < 10; i++) {
            sprintf(buf1, &lbl_eu_8050EFDC[0x28c], i + 1);
            nw4r::lyt::Pane* p = field_38->GetRootPane()->FindPaneByName(buf1, true);
            setPaneVisible(p, (u8)i < pages);
        }
        setLayoutTextBoxNumber(field_38, &lbl_eu_8050EFDC[0x29b], (u8)((u8)grid->idx + 1));
        nw4r::lyt::Pane* cur =
            field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x2a4], true);
        // Move the cursor pane: start from the base position at +0x200 and
        // shift x by scale * (row + 10 - pages). The (f32) casts make MWCC
        // emit its own s32->f32 magic (pool consts B18/B10 are compiler-
        // generated; do NOT hand-roll the 0x4330 union here).
        vec[0] = unk_200;
        vec[1] = unk_204;
        vec[2] = unk_208;
        float fp = (f32)(u32)(u8)(10 - pages);
        float fi = (f32)(int)((u8)((u8)grid->idx + 1) - 1);
        ((float*)vec)[0] += lbl_eu_80668B44 * fi + lbl_eu_80668B44 * fp;
        copyVEC3((float*)&cur->GetTranslate(), (const float*)vec);
    } else {
        nw4r::lyt::Pane* pane =
            field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050EFDC[0x285], true);
        setPaneVisible(pane, 0);
    }
    for (u8 i = 0; i < 6; i++) {
        sprintf(buf2, &lbl_eu_8050EFDC[0x2b2], i + 1);
        nw4r::lyt::Pane* p = field_38->GetRootPane()->FindPaneByName(buf2, true);
        if (p != 0) {
            setPaneVisible(p, (u8)i < grid->getEIBColCount());
        }
    }
    for (u8 i = 0; i < 30; i++) {
        u32 kind = grid->getEIBItemKind(i);
        void* item = grid->getEIBItemObj(i);
        u8 b7 = grid->getEIBByte7(i);
        bindEIBCellTexture((u16)kind, item, i, b7);
        bindEIBCellIcon((u16)kind, item, i);
        refreshEIBCellText((s8)grid->chkEIBEquip(i), (CItemInstance*)item, i);
        u32 show;
        if ((u16)kind != 0) {
            show = grid->getEIBByte6(i);
        } else {
            show = 1;
        }
        u8 b2 = grid->getEIBByte2(i);
        u8 b3 = grid->getEIBByte3(i);
        bindEIBCellMark(b3, i, (u32)(b2 == b), show);
    }
}

// Refresh the equip item box list: bind the current selection's name into the
// layout, re-dispatch the interaction for the selected grid cell, then copy
// the per-page item ids/records into the item-box info list (+0xB0 region).
void CEquipItemBox::refreshEIBItemList() {
    u8 idx = (u8)(unk_1f4 + unk_1f5 * 5);
    CEquipItemGrid* grid = &mGrid;
    if ((s8)unk_1f5 == -1) {
        LayoutSetTextBoxFmtValue(field_38, &lbl_eu_8050EFDC[0x2bf], getEIBPageHint(), 0);
    } else {
        LayoutSetTextBoxFmtValue(field_38, &lbl_eu_8050EFDC[0x2bf], grid->fmtEIBItemName(idx), 0);
    }
    if (((CItemBoxInfo*)((void*)unk_20c))->GetState() == 0) return;
    void* item;
    if ((s8)unk_1f5 == -1) item = 0;
    else item = grid->getEIBItemObj(idx);
    ((CItemBoxInfo*)(unk_20c))->SetItem(unk_1fc, item, grid->getEIBByte6(idx));
    ((CItemBoxInfo*)(unk_20c))->SetNamedText(1, grid->fmtEIBItemName(idx));
    if ((s8)unk_1f5 == -1) item = 0;
    else item = grid->getEIBItemObj(idx);
    u16 kind;
    if ((s8)unk_1f5 == -1) kind = 0;
    else kind = grid->getEIBItemKind(idx);
    dispatchEIBKind(kind, (int)item);
    CItemBoxPage* list = &((CItemBoxInfo*)unk_20c)->mPage;
    for (u32 i = 0; i < 0xc; i++) {
                u16 v = ArrayGet12(&mPage.mItemIds[0], (u8)i);
        if (v == 0) continue;
        list->mItemIds[(u8)i] = v;
        list->mBytesA8[(u8)i] = ItemBoxGetTab((const u8*)&mPage.mItemIds[0], (u8)i);
        u8 b;
        if ((u8)i < 0xc) b = mPage.mBytesB4[(u8)i];
        else b = 0;
        list->mBytesA8[(u8)i + 0xc] = b;
        s16 s;
        if ((u8)i < 0xc) s = mPage.mValues[(u8)i];
        else s = 0;
        list->mValues[(u8)i] = s;
        nw4r::math::VEC3 tmp;
        CopyTabSlotVec(&tmp, &mPage.mItemIds[0], (u8)i);
        copyVEC3(&list->mPos[(u8)i], (const float*)&tmp);
    }
}

// Rebuild the cursor target after a page/item change. With no active syswin
// window the cursor receives the current window name directly; otherwise a
// page-/item-name string is formatted, its pane position is read and scaled
// by the anchor pane's scale, and the result is written into the anim target
// hanging off this+0x4C (sub-view +0x2C slot). The tail is duplicated per
// branch in retail (no shared helper), so keep both copies textual.
void CEquipItemBox::updateEIBCursorPos() {
    // One shared VEC3 scratch slot and three name buffers; the retail frame
    // reuses the low slot for winName/pos because their branches do not overlap.
    char pageName[0x28];
    char itemName[0x20];
    u8 winName[0x18];
    nw4r::math::VEC3 pos;
    // Polarity per retail: menu OPEN (!= 0) takes the win-name path.
    if (reinterpret_cast<CSysWin*>(&mSysWin1)->isOpened() != 0) {
        CSysWin::getPaneScreenPos(reinterpret_cast<nw4r::math::VEC3*>(winName), reinterpret_cast<CSysWin*>(&mSysWin1), unk_374);
        ccur18.setRootPaneTranslate((nw4r::math::VEC3*)winName);
    } else {
        s8 sel = unk_1f5;
        if (sel == -1) {
            sprintf(pageName, &lbl_eu_8050EFDC[0x2C8], (s8)unk_373 + 1);
            nw4r::lyt::Pane* pane =
                field_38->GetRootPane()
                    ->FindPaneByName(pageName, true);
            AccumPaneWorldPos(&pos, pane);
            pane = field_38->GetRootPane()
                       ->FindPaneByName(&lbl_eu_8050EFDC[0x193], true);
            pos.x *= pane->GetScale().x;
            copyVEC3((float*)&mCur44.mpLayout->GetRootPane()->GetTranslate(),
                     &pos);
        } else {
            sprintf(itemName, &lbl_eu_8050EFDC[0x20C], (s8)unk_1f4 + sel * 5 + 1);
            nw4r::lyt::Pane* pane =
                field_38->GetRootPane()
                    ->FindPaneByName(itemName, true);
            AccumPaneWorldPos(&pos, pane);
            pane = field_38->GetRootPane()
                       ->FindPaneByName(&lbl_eu_8050EFDC[0x193], true);
            pos.x *= pane->GetScale().x;
            copyVEC3((float*)&mCur44.mpLayout->GetRootPane()->GetTranslate(),
                     &pos);
        }
    }
}

// Rebuild the sort-menu page-list contents for the current page (unk_36c
// entry selected by unk_373): feed the page value to the sort menu, fill a
// per-page sound-key table (7/12/10 entries for pages 2/3/4..8), bind the
// page sound name into the layout, then re-add every entry whose key is
// positive and refresh the page-list cursor position.
void CEquipItemBox::fillEIBSortMenu() {
    u8 page = unk_36c[(s8)unk_373];
    // The 3rd/4th args are the two count bytes this function later re-reads;
    // MWCC hoists their address constants (r5/r6) into the prologue.
    mSortMenu.CalcSelect(page, &unk_379, &unk_37a);
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
    u16 idx = CItemBlock_getFlag120EC(page);
    char* name = BdatTouchStringCell(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], (u32)buf[idx]);
    LayoutSetTextBoxFmtValue(field_38, &lbl_eu_8050EFDC[0x2d4], name, 0);
    if (mSortMenu.IsReady() != 0) {
        mSortMenu.Clear();
        u8 i = 0;
        while (1) {
            if ((int)buf[i] <= 0) break;
            mSortMenu.Add(BdatTouchStringCell(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], (u32)buf[i]));
            i++;
        }
        mSortMenu.SetSelect((u8)(unk_379 + unk_37a));
    }
}

void CEquipItemBox::clearEIBCatList() {
    memset(unk_36c, 0, 6);
    unk_372 = 0;
    unk_373 = 0;
}

void CEquipItemBox::pushEIBCatList(u8 val) {
    u8 idx = unk_372;
    if (idx >= 6) return;
    unk_36c[idx] = val;
    unk_372 = idx + 1;
}

// Step the sort-menu page selection backwards (wrapping from the first page to
// the last) with the current page stored in unk_373.
void CEquipItemBox::eibSortPageNext() {
    if (mSortMenu.IsOpen() != 0) return;
    u8 v = unk_373 + 1;
    unk_373 = v;
    if ((int)(s8)v >= unk_372) {
        unk_373 = 0;
    }
    refreshEIBSortPages(0);
    rebuildEIBPage(0);
    playUISound(0x70);
}

// Step the sort-menu page selection forwards (wrapping back to page 0 after the
// last page) with the current page stored in unk_373.
void CEquipItemBox::eibSortPagePrev() {
    if (mSortMenu.IsOpen() != 0) return;
    u8 v = unk_373 - 1;
    unk_373 = v;
    if ((s8)v < 0) {
        unk_373 = unk_372 - 1;
    }
    refreshEIBSortPages(1);
    rebuildEIBPage(0);
    playUISound(0x70);
}

// Rebuild the sort-menu page list after a page change: for each of the 6 page
// slots, format its two pane names, set the pane colours (a highlighted pair
// when the slot is the current page, an idle pair otherwise), look the panes
// up by name and toggle their visibility, then hand the slot value to the
// page handler. Finally refresh the current-page cursor and the page list.
void CEquipItemBox::refreshEIBSortTabs() {
    for (u8 i = 0; i < 6; i++) {
        char buf1[0x20];
        char buf2[0x20];
        sprintf(buf1, &lbl_eu_8050EFDC[0x2df], i + 1);
        sprintf(buf2, &lbl_eu_8050EFDC[0x2ee], i + 1);
        bool flagA = false;
        bool flagB = false;
        u8 v = unk_36c[i];
        if (v != 0) {
            if (i == (s8)unk_373) {
                flagA = true;
                flagB = false;
            } else {
                flagA = false;
                flagB = true;
            }
            if ((u32)(v - 4) <= 4 || (int)v == 2 || (int)v == 0xb) {
                PaneMatSetTevColorsByName(field_38, buf1, (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
                PaneMatSetTevColorsByName(field_38, buf2, (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
            } else {
                PaneMatSetTevColorsByName(field_38, buf1, (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
                PaneMatSetTevColorsByName(field_38, buf2, (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
            }
        }
        nw4r::lyt::Pane* pane1 = field_38->GetRootPane()->FindPaneByName(buf1, true);
        setPaneVisible(pane1, flagA);
        nw4r::lyt::Pane* pane2 = field_38->GetRootPane()->FindPaneByName(buf2, true);
        setPaneVisible(pane2, flagB);
        bindEIBPageTexture(v, i);
    }
    refreshEIBSortPages(unk_373);
    rebuildEIBPage(0);
}

// Per-page texture bind: build the page's texture name from the per-kind
// format-string table, fetch the 'timg' resource via the layout's arc
// accessor (falling back to the shared accessor's default name), then bind
// it to the pane named by the page index.
void CEquipItemBox::bindEIBPageTexture(u8 v, u8 i) {
    char texName[0x20];
    char paneName[0x10];
    nw4r::lyt::Pane* pane;
    // Single dense switch (cases 0..12) so MWCC emits the retail range-check
    // + jumptable dispatch; case 0 jumps straight to the bind.
    switch (v) {
    case 0:
        pane = (nw4r::lyt::Pane*)field_30->GetResource(0x74696D67, &lbl_eu_8050EFDC[0x1B0], 0);
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
        pane = (nw4r::lyt::Pane*)field_34->GetResource(0x74696D67, texName, 0);
        if (pane == 0) {
            pane = (nw4r::lyt::Pane*)field_30->GetResource(0x74696D67, &lbl_eu_8050EFDC[0x1B0], 0);
        }
    bind:;
    }
    if (pane != 0) {
        sprintf(paneName, &lbl_eu_8050EFDC[0x2C8], i + 1);
        PaneSetTexPaletteByName(field_38, (const char*)paneName, (TPLPalette*)((u32)pane));
    }
}

// Rebuild the sort-page selection display: step the page cursor (next/prev
// page around unk_373, clamped to the page range), toggle the two pane names
// per page slot, then recolour the 15 page-name panes (highlighted pair for
// pages 2/4..8/0xB, idle pair otherwise).
void CEquipItemBox::refreshEIBSortPages(int a) {
    u8 buf[2];
    buf[0] = unk_373;
    s8 v = (s8)unk_373;
    buf[1] = (u8)((a != 0) ? v + 1 : v - 1);
    if ((s8)buf[1] < 0) {
        buf[1] = (u8)(unk_372 - 1);
    } else if ((s8)buf[1] >= unk_372) {
        buf[1] = 0;
    }
    for (u32 i = 0; i < 2; i++) {
        char name1[0x20];
        char name2[0x20];
        sprintf(name1, &lbl_eu_8050EFDC[0x2df], (s8)buf[(u8)i] + 1);
        sprintf(name2, &lbl_eu_8050EFDC[0x2ee], (s8)buf[(u8)i] + 1);
        bool flagA = false;
        s32 b = (s8)buf[(u8)i];
        bool flagB = false;
        if (unk_36c[b] != 0) {
            if (b == (s8)unk_373) {
                flagA = true;
                flagB = false;
            } else {
                flagA = false;
                flagB = true;
            }
        }
        nw4r::lyt::Pane* pane = field_38->GetRootPane()->FindPaneByName(name1, true);
        setPaneVisible(pane, flagA);
        pane = field_38->GetRootPane()->FindPaneByName(name2, true);
        setPaneVisible(pane, flagB);
    }
    u8 v2 = unk_36c[(s8)unk_373];
    if ((u32)(v2 - 4) <= 4 || (int)v2 == 2 || (int)v2 == 0xb) {
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x3f9], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x405], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x411], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x41d], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x429], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x435], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x441], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x44d], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x459], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x465], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x470], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x47b], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x486], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x491], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x49c], (const GXColorS10*)equipItemBox_sortMenuTabSelectedTop, (const GXColorS10*)equipItemBox_sortMenuTabSelectedBottom);
    } else {
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x3f9], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x405], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x411], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x41d], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x429], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x435], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x441], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x44d], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x459], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x465], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x470], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x47b], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x486], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x491], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
        PaneMatSetTevColorsByName(field_38, &lbl_eu_8050EFDC[0x49c], (const GXColorS10*)equipItemBox_sortMenuTabNeutralTop, (const GXColorS10*)equipItemBox_sortMenuTabNeutralBottom);
    }
}


// Dispatch an equip-box interaction by the window kind byte: kinds 2/3 and
// the 4..8 band go to their dedicated handlers, anything else is ignored.
#pragma function_align 4
void CEquipItemBox::dispatchEIBKind(int a, int b) {
    resetEIBPageView();
    int v = (u8)BdatGetItemType(a);
    switch (v) {
    case 2:
        rebuildEIBEquipPage(a, b);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        rebuildEIBKind48Page(a, b);
        break;
    case 3:
        rebuildEIBKind3Page(a, b);
        break;
    }
}

// Reset the per-page item state: clear the 12 item-id shorts at +0x210 and
// zero the 12 page-position VEC3 records at +0x228, then hide every one of
// the 30 equip-item panes in the layout (string base hoisted into a saved
// register like retail's r31). -O4,s selects retail's
// _savegpr_29 prologue for this 3-saved-reg frame.
void CEquipItemBox::resetEIBPageView() {
    for (u8 i = 0; i < 0xc; i++) {
        mPage.mItemIds[i] = 0;
        float vec[3];
        float* p = (float*)code80135FDC_setVec3((nw4r::math::VEC3*)vec, lbl_eu_80668B04, lbl_eu_80668B04, lbl_eu_80668B04);
        copyVEC3(&mPage.mPos[i], p);
    }
    const char* base = lbl_eu_8050EFDC;
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4a7, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4b3, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4bf, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4ca, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4d5, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4e0, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4eb, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4f6, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x501, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x50c, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x517, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x522, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x52e, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x53a, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x546, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x552, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x55e, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x56a, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x577, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x584, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x591, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x59b, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5a5, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5af, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5be, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5cd, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5dc, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5e7, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5f2, true), 0);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5fe, true), 0);
}

// Kind-2 (equipment) rebuild handler: show the 24 item panes, read the item's
// stat/name tables (bdat lookups keyed by the kind word), scale the two
// drop-rate counters by the RNG when the flag word says so, then bind the
// computed texts (0x522/0x546/0x552/0x52e/0x53a), re-tint the stat panes and
// hand the box to the per-cell grid rebuild (rebuildEIBKind2Cells). -O4,s
// selects retail's _savegpr prologue for this 13-saved-reg frame.
void CEquipItemBox::rebuildEIBEquipPage(int a, int b) {
    // Declaration order follows retail's saved-register allocation
    // (this=r19, a=r20, b=r21, rateB=r22, rateA=r23, g=r24, base=r25,
    // kind2=r26, name72=r27, name7a=r28, name61=r29, hi=r30, name69=r31).
    // Int-typed (retail defers narrow-masking to use sites; rates are
    // small table ids so no value ever exceeds u16 range).
    u16 rateB;
    u16 rateA;
    u32 g;
    char* base;
    u16 kind2;
    u8 name72;
    u8 name7a;
    u8 name61;
    u32 hi;
    u8 name69;
    char buf[0x20];
    char nb[0x28];
    base = lbl_eu_8050EFDC;
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4a7, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4bf, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4ca, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4d5, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4e0, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4eb, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4f6, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x501, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x50c, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x517, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x522, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x52e, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x53a, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x546, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x552, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x56a, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x577, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x584, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x591, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x59b, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5a5, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5af, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5be, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5cd, true), 1);
    g = (u32)lbl_eu_806640F4;
    BdatGetItemType(a);
    kind2 = BdatGetItemId(a);
    rateA = BdatGetU16Direct((const void*)g, base + 0x60a, kind2);
    rateB = BdatGetU16Direct((const void*)g, base + 0x8b, kind2);
    name72 = BdatGetU8Direct((const void*)g, base + 0x72, kind2);
    name7a = BdatGetU8Direct((const void*)g, base + 0x7a, kind2);
    name61 = BdatGetU8Direct((const void*)g, base + 0x61, kind2);
    // The three int->float conversions use MWCC's own s32->f32 magic
    // ((f32) casts emit lis-0x4330/xoris/lfd/fsubs inline, sharing one
    // constant copy in r30 like retail); do NOT hand-roll unions here.
    float f = (f32)(u32)(u8)BdatGetU8Direct((const void*)g, base + 0x612, kind2) / 10.0f;
    name69 = BdatGetU8Direct((const void*)g, base + 0x69, kind2);
    u8 flag92 = BdatGetU8Direct((const void*)g, base + 0x92, kind2);
    if (flag92 & 4) {
        void* obj = CtrlObjectParam_GetCharEntry(1);
        rateA = 1.4f * (f32)(int)(rateA * (u32)CtrlObjectParam_GetArtsDataWord(obj));
        rateB = 1.5f * (f32)(int)(rateB * (u32)CtrlObjectParam_GetArtsDataWord(obj));
        if (rateA >= 0x3e7) rateA = 0x3e7;
        if (rateB >= 0x3e7) rateB = 0x3e7;
    }
    for (u8 i = 1; i <= 10; i++) {
        sprintf(nb, base + 0x7, i);
        if ((u8)BdatGetU8Direct((const void*)g, (const char*)nb, kind2)) {
            f += GetFloatTableEntry(i);
            break;
        }
    }
    BlendFloatAvgScale((u32)rateA, (u32)rateB, 0, f);
    char* str = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0xb);
    sprintf(buf, base + 0x618, rateA, str, rateB);
    LayoutSetTextBoxText(field_38, base + 0x522, buf, 0);
    setLayoutTextBoxNumber(field_38, base + 0x546, (u8)name72);
    setLayoutTextBoxNumber(field_38, base + 0x552, (u8)name7a);
    char* n1 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x7f);
    char* n2 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x80);
    if ((u8)name61) {
        sprintf(buf, base + 0x61f, n1, n2);
    } else {
        sprintf(buf, base + 0x626, (u8)name61, n2);
    }
    LayoutSetTextBoxText(field_38, base + 0x52e, buf, 0);
    n1 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x7f);
    n2 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x80);
    if ((u8)name69) {
        sprintf(buf, base + 0x61f, n1, n2);
    } else {
        sprintf(buf, base + 0x626, (u8)name69, n2);
    }
    LayoutSetTextBoxText(field_38, base + 0x53a, buf, 0);
    LayoutSetTextBoxFmtValue(field_38, base + 0x55e, base + 0x228, 0);
    LayoutSetTextBoxFmtValue(field_38, base + 0x50c, base + 0x228, 0);
    LayoutSetTextBoxFmtValue(field_38, base + 0x517, base + 0x228, 0);
    PaneMatSetTevColorsByName(field_38, base + 0x4bf, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4e0, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4eb, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4ca, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4d5, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4f6, (const GXColorS10*)equipItemBox_cellLabelMutedTop, (const GXColorS10*)equipItemBox_cellLabelMutedBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x501, (const GXColorS10*)equipItemBox_cellLabelMutedTop, (const GXColorS10*)equipItemBox_cellLabelMutedBottom);
    rebuildEIBKind2Cells(a, b);
}

// Rebuild the equip-box panes for a window kind in the 4..8 band: show the 22
// item cells, bind the 3 name texts (0x72/0x7a/0x82 bdat lookups), pick the
// sort-menu sound by the name byte, recolour the 7 stat panes, then hand the
// box to the kind-specific helper. -O4,s selects retail's stmw
// r24 prologue for this 8-saved-reg frame.
void CEquipItemBox::rebuildEIBKind48Page(int kind, int item) {
    char* base = lbl_eu_8050EFDC;
    u16 kind2;
    u8 nameA, nameB, nameC;
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4a7, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4bf, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4ca, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4d5, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4e0, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4eb, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x4f6, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x501, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x50c, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x517, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x546, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x552, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x55e, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x56a, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x577, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x584, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x591, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x59b, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5a5, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5af, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5be, true), 1);
    setPaneVisible(field_38->GetRootPane()->FindPaneByName(base + 0x5cd, true), 1);
    // Loaded into a register only here (retail keeps it live across the
    // bdat-name lookups below).
    u32 g = (u32)lbl_eu_806640F8;
    BdatGetItemType(kind);
    kind2 = (u16)BdatGetItemId(kind);
    nameA = BdatGetU8Direct((const void*)g, base + 0x72, kind2);
    nameB = BdatGetU8Direct((const void*)g, base + 0x7a, kind2);
    nameC = BdatGetU8Direct((const void*)g, base + 0x82, kind2);
    setLayoutTextBoxNumber(field_38, base + 0x546, (u8)nameA);
    setLayoutTextBoxNumber(field_38, base + 0x552, (u8)nameB);
    setLayoutTextBoxNumber(field_38, base + 0x55e, (u8)nameC);
    // Retail reloads lbl_eu_806640F8 here: the three setLayoutTextBoxNumber calls
    // above are opaque, so the global must not be cached across them.
    u8 v = BdatGetU8Direct((const void*)lbl_eu_806640F8, base + 0x17, (u16)BdatGetItemId(kind));
    u8 b = (u8)v;
    char* str;
    if ((u32)(b - 4) <= 9) {
        str = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x2e);
    } else if (b == 3) {
        str = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x32);
    } else if (b == 2) {
        str = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x31);
    } else if (b == 1) {
        str = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x30);
    } else {
        str = 0;
    }
    LayoutSetTextBoxFmtValue(field_38, base + 0x50c, str, 0);
    u8 b2 = (u8)v;
    char* str2 = 0;
    if ((u32)(b2 - 4) <= 8) {
        switch (b2) {
        case 4: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x77); break;
        case 5: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x78); break;
        case 6: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x79); break;
        case 7: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x7a); break;
        case 8: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x7b); break;
        case 9: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x7c); break;
        case 10: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x7d); break;
        case 11: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x7e); break;
        case 12: str2 = BdatTouchStringCell((char*)base + 0x62b, base + 0x36, 0x7f); break;
        }
    }
    LayoutSetTextBoxFmtValue(field_38, base + 0x517, str2, 0);
    PaneMatSetTevColorsByName(field_38, base + 0x4bf, (const GXColorS10*)equipItemBox_cellLabelMutedTop, (const GXColorS10*)equipItemBox_cellLabelMutedBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4e0, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4eb, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4ca, (const GXColorS10*)equipItemBox_cellLabelMutedTop, (const GXColorS10*)equipItemBox_cellLabelMutedBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4d5, (const GXColorS10*)equipItemBox_cellLabelMutedTop, (const GXColorS10*)equipItemBox_cellLabelMutedBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x4f6, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    PaneMatSetTevColorsByName(field_38, base + 0x501, (const GXColorS10*)equipItemBox_cellLabelNeutralTop, (const GXColorS10*)equipItemBox_cellLabelNeutralBottom);
    rebuildEIBKind2Cells(kind, item);
}

// Kind-3 rebuild handler: reveal the five detail panes, bind the item's
// count/name/detail texts, position the cursor anchor between the detail and
// anchor panes, then store the selection into the page tables (+0x220 kind,
// +0x2c0 state byte 3, +0x2cc count byte) and the embedded item-box index.
void CEquipItemBox::rebuildEIBKind3Page(int kind, int item) {
    char* base = lbl_eu_8050EFDC;
    // All pane lookups in this handler go through the layout's pane-finder
    // sub-object (vtable slot 13), not the root pane.
    setPaneVisible(field_38->GetRootPane()
                      ->FindPaneByName(base + 0x4b3, true),
                  1);
    setPaneVisible(field_38->GetRootPane()
                      ->FindPaneByName(base + 0x5dc, true),
                  1);
    setPaneVisible(field_38->GetRootPane()
                      ->FindPaneByName(base + 0x5e7, true),
                  1);
    setPaneVisible(field_38->GetRootPane()
                      ->FindPaneByName(base + 0x5f2, true),
                  1);
    setPaneVisible(field_38->GetRootPane()
                      ->FindPaneByName(base + 0x5fe, true),
                  1);
    BdatGetItemType(kind);
    BdatGetItemId(kind);
    CItemInstance* obj = (CItemInstance*)item;
    u32 key = CItem_initItemImplInstances((CItemData*)obj)->vf54((CItemData*)obj);
    // Count text: "xN" scaled by the vf08 slot count.
    u32 id = CItem_initItemImplInstances((CItemData*)obj)->vf08((CItemData*)obj);
    char* str = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x1e - ((u8)id - 1));
    LayoutSetTextBoxFmtValue(field_38, base + 0x5f2, str, 0);
    // Name text from the bdat lookup keyed by the vf54 name key.
    u32 name = BdatGetU8Direct(
        (const void*)lbl_eu_806640D8, base + 0x36,
        (u16)CItem_initItemImplInstances((CItemData*)obj)->vf54((CItemData*)obj));
    LayoutSetTextBoxFmtValue(field_38, base + 0x5e7, (char*)name, 0);
    // Detail text: when the vf90 value is present, prefix a per-name-byte
    // unit string for name bytes 3/4; otherwise bind the empty placeholder.
    if (CItem_initItemImplInstances((CItemData*)obj)->vf90((CItemData*)obj) == 0) {
        LayoutSetTextBoxFmtValue(field_38, base + 0x5fe, base + 0x228, 0);
    } else {
        char buf[0x20];
        u8 v = BdatGetU8Direct((const void*)lbl_eu_806640D8, base + 0x633, (u16)key);
        if (v != 0 && (u32)(v - 3) <= 1) {
            char* str2 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x21);
            sprintf(buf, base + 0x626,
                    CItem_initItemImplInstances((CItemData*)obj)->vf90((CItemData*)obj), str2);
        } else {
            sprintf(buf, base + 0x63c,
                    CItem_initItemImplInstances((CItemData*)obj)->vf90((CItemData*)obj));
        }
        LayoutSetTextBoxText(field_38, base + 0x5fe, buf, 0);
    }
    // Category label: name-byte + 0x15 selects the category string.
    u32 cat = BdatGetU8Direct((const void*)lbl_eu_806640D8, base + 0, (u16)key);
    char* str3 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, (u8)cat + 0x15);
    LayoutSetTextBoxFmtValue(field_38, base + 0x640, str3, 0);
    // Cursor anchor: midpoint helper between the detail pane and anchor pane
    // (both looked up again through the pane-finder sub-object).
    nw4r::lyt::Pane* paneDetail =
        field_38->GetRootPane()
            ->FindPaneByName(base + 0x5e7, true);
    nw4r::lyt::Pane* paneAnchor =
        field_38->GetRootPane()
            ->FindPaneByName(base + 0x193, true);
    float pos[3];
    ScalePaneWorldPos((nw4r::math::VEC3*)pos, paneDetail, paneAnchor,
                  field_38->GetRootPane());
    mPage.mItemIds[8] = (u16)key;
    mPage.mBytesA8[8] = 3;
    mPage.mBytesB4[8] = (u8)CItem_initItemImplInstances((CItemData*)obj)->vf08((CItemData*)obj);
    (&mPage)->SetValue(8, (short)CItem_initItemImplInstances((CItemData*)obj)->vf90((CItemData*)obj));
    copyVEC3(&mPage.mPos[5], pos);
}


// Per-cell equip-box rebuild for the kind-2 page: for each of the three
// slots, look the slot's item up, format the cell name/icon/stat texts into
// the pane, and store the per-cell ids/bytes/shorts into the page tables at
// +0x210/+0x2B8/+0x2C4/+0x2D0 (+ the page VEC3 records at +0x228). The
// slot==-1 path reads the item's flag list instead.
void CEquipItemBox::rebuildEIBKind2Cells(int a, int b) {
    char* base = lbl_eu_8050EFDC;
    if (b == 0) return;
    if (((CItemInstance*)b)->word == 0) return;
    CItemImpl* impl = CItem_initItemImplInstances((CItemData*)b);
    u16 count = (u16)impl->vf30((CItemData*)b);
    int idx = 0;
    do {
        char buf[0x20];
        char buf40[0x20];
        u8 i1 = (u8)idx + 1;
        sprintf(buf, base + 0x64b, i1);
        nw4r::lyt::Pane* pane = field_38->GetRootPane()->FindPaneByName(buf, true);
        void* icon = 0;
        u16 storeKind = 0;
        u8 storeByte = 0;
        s16 storeKey = 0;
        if (idx >= count) {
            setPaneVisible(pane, 0);
            sprintf(buf, base + 0x6f4, i1);
            LayoutSetTextBoxFmtValue(field_38, buf, base + 0x228, 0);
            sprintf(buf, base + 0x705, i1);
            LayoutSetTextBoxFmtValue(field_38, buf, base + 0x228, 0);
        } else {
            setPaneVisible(pane, 1);
            s16 slot = CItem_initItemImplInstances((CItemData*)b)
                           ->vf40((CItemData*)b, (u8)idx);
            if (slot != -1) {
                CItemInstance* item = (CItemInstance*)CItem_kindPtr(3, slot);
                u32 g = (u32)lbl_eu_806640D8;
                u16 kind = (u16)CItem_initItemImplInstances((CItemData*)item)->vf54((CItemData*)item);
                storeKind = kind;
                char* name = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36,
                                            0x1e - ((u16)CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item) - 1));
                switch ((u8)BdatGetU8Direct((const void*)g, base + 0x1a7, kind)) {
                case 0:
                    icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x65a, 0);
                    break;
                case 4:
                    icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x670, 0);
                    break;
                case 5:
                    icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x686, 0);
                    break;
                case 6:
                    icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x69c, 0);
                    break;
                case 7:
                    icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6b2, 0);
                    break;
                case 8:
                    icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6c8, 0);
                    break;
                case 9:
                    icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6de, 0);
                    break;
                }
                u32 key90 = CItem_initItemImplInstances((CItemData*)item)->vf90((CItemData*)item);
                if (key90 == 0) {
                    sprintf(buf, base + 0x6f4, i1);
                    LayoutSetTextBoxFmtValue(field_38, buf, base + 0x228, 0);
                } else {
                    u8 v = BdatGetU8Direct((const void*)g, base + 0x633, kind);
                    if (v != 0 && (u8)(v - 3) <= 1) {
                        char* str = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x21);
                        sprintf(buf40, base + 0x626, key90, str);
                    } else {
                        sprintf(buf40, base + 0x63c, key90);
                    }
                    sprintf(buf, base + 0x6f4, i1);
                    LayoutSetTextBoxText(field_38, buf, buf40, 0);
                }
                char* str2 = BdatGetPtrDirect((const void*)g, base + 0x36, kind);
                sprintf(buf40, base + 0x3b, str2, name);
                storeByte = (u8)CItem_initItemImplInstances((CItemData*)item)->vf08((CItemData*)item);
                storeKey = (s16)CItem_initItemImplInstances((CItemData*)item)->vf90((CItemData*)item);
                PaneSetTexPalette(pane, (TPLPalette*)((unsigned long)icon));
            } else {
                CItemInstance* entry = (CItemInstance*)CItem_initItemImplInstances((CItemData*)b)
                                           ->vf2C((CItemData*)b, (u8)idx);
                if (entry != 0 && (entry->flags & 1)) {
                    u16 kind2 = (u16)((entry->word >> 16) & 0xFFF);
                    u32 g2 = (u32)lbl_eu_806640D8;
                    u32 bits3 = (entry->word >> 22) & 7;
                    storeKind = kind2;
                    char* name2 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x1e - (bits3 - 1));
                    switch ((u8)BdatGetU8Direct((const void*)g2, base + 0x1a7, kind2)) {
                    case 0:
                        icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x65a, 0);
                        break;
                    case 4:
                        icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x670, 0);
                        break;
                    case 5:
                        icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x686, 0);
                        break;
                    case 6:
                        icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x69c, 0);
                        break;
                    case 7:
                        icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6b2, 0);
                        break;
                    case 8:
                        icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6c8, 0);
                        break;
                    case 9:
                        icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x6de, 0);
                        break;
                    }
                    s16 bit11 = (s16)((entry->word >> 11) & 0x7FF);
                    if (bit11 == 0) {
                        sprintf(buf, base + 0x6f4, i1);
                        LayoutSetTextBoxFmtValue(field_38, buf, base + 0x228, 0);
                    } else {
                        u32 v2 = BdatGetU8Direct((const void*)g2, base + 0x633, kind2);
                        if (v2 != 0 && (u8)((u8)v2 - 3) <= 1) {
                            char* str3 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x21);
                            sprintf(buf40, base + 0x626, bit11, str3);
                        } else {
                            sprintf(buf40, base + 0x63c, bit11);
                        }
                        sprintf(buf, base + 0x6f4, i1);
                        LayoutSetTextBoxText(field_38, buf, buf40, 0);
                    }
                    char* str4 = BdatGetPtrDirect((const void*)g2, base + 0x36, kind2);
                    sprintf(buf40, base + 0x3b, str4, name2);
                    storeByte = (u8)bits3;
                    storeKey = (s16)((entry->word >> 11) & 0x7FF);
                    PaneSetTexPalette(pane, (TPLPalette*)((unsigned long)icon));
                } else {
                    icon = field_34->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x65a, 0);
                    sprintf(buf, base + 0x6f4, i1);
                    LayoutSetTextBoxFmtValue(field_38, buf, base + 0x228, 0);
                    sprintf(buf40, base + 0x2a, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x2a));
                    sprintf(buf, base + 0x705, i1);
                    LayoutSetTextBoxText(field_38, buf, buf40, 0);
                    PaneSetTexPalette(pane, (TPLPalette*)((unsigned long)icon));
                }
            }
        }
        nw4r::lyt::Pane* pane193 =
            field_38->GetRootPane()->FindPaneByName(base + 0x193, true);
        float out[3];
        ScalePaneWorldPos((nw4r::math::VEC3*)out, pane, pane193, field_38->GetRootPane());
        u8 v = (u8)(idx + 8);
        if (v < 0xc) {
            mPage.mItemIds[v] = storeKind;
        }
        if (v < 0xc) {
            mPage.mBytesA8[v] = 3;
        }
        if (v < 0xc) {
            mPage.mBytesB4[v] = storeByte;
        }
        if (v < 0xc) {
            mPage.mValues[v] = storeKey;
        }
        if (v < 0xc) {
            copyVEC3(&mPage.mPos[v], out);
        }
        idx++;
    } while (idx < 3);
}

// Equip/use the grid item selected by (unk_1f4 + unk_1f5*5): resolve its
// owning category, then clear the item from the owner's slots (for window
// kind 3, from the matched CfObjectMove's move list). Returns 0 when the
// cell is empty or the kind is rejected (BdatGetItemType == 2), else 1.
int CEquipItemBox::useEIBSelectedItem() {
    CEquipItemGrid* grid = &mGrid;
    u8 idx = (u8)((u8)unk_1f4 + unk_1f5 * 5);
    s16 itemId = grid->getEIBItemId(idx);
    u16 kind = (u16)grid->getEIBItemKind(idx);
    if (kind == 0) {
        return 0;
    }
    u32 kindByte = BdatGetItemType(kind);
    if ((u16)kindByte == 2) {
        // Item type cannot be unequipped this way.
        return 0;
    }
    // Find the party index whose category byte matches the cell's category
    // (skipped entirely for window kind 3).
    u32 slot = 0;
    if (kindByte != 3) {
        for (u8 i = 0; i < code80135FDC_getByte_64077(); i++) {
            if (grid->getEIBByte2(idx) == GetCollectedFlagByte(i)) {
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
        CEquipItemBoxMoveTable tbl = sEquipMoveSearchTypes;
        CfEnumListHolder holder;
        void* list = holder.get();
        startEnumObjects(list, tbl.w[slot], 0);
        list = holder.get();
        if (((CEquipItemBoxEnumList*)list)->field_620 >= 1) {
            CEquipItemBoxMoveEntryView* entry =
                (CEquipItemBoxMoveEntryView*)getEntryAt(list, 0);
            if (entry != 0 && entry->field_4 != 0) {
                void* moveObj = getCfObjectPc((cf::CfObjectMove*)entry);
                if (kindByte != 3) {
                    // Clear the item from the owning party member's slots.
                    u8 cat = GetCollectedFlagByte(slot);
                    void* owner = CtrlObjectParam_GetCharEntry(cat);
                    switch (BdatGetItemType(cat)) {
                    case 2: CtrlObjectParam_SetEquipSlot5(owner, -1); break;
                    case 4: CtrlObjectParam_SetEquipSlot0(owner, -1); break;
                    case 5: CtrlObjectParam_SetEquipSlot1(owner, -1); break;
                    case 6: reinterpret_cast<cf::CtrlObjectParamData*>(owner)->setArgType2((void*)-1); break;
                    case 7: reinterpret_cast<cf::CtrlObjectParamData*>(owner)->setArgType3((void*)-1); break;
                    case 8: CtrlObjectParam_SetEquipSlot4(owner, -1); break;
                    }
                    CtrlObjectParam_SyncParamFromActor(owner);
                    CfObjectPc_setupFrame(moveObj, 0);
                }
            }
        }
    }
    // Shared tail: copy the 12-byte category table and free the first slot
    // (across all six categories) that still holds the item id.
    CEquipItemBoxCatTable12 cats = sEquipSlotClearCategories;
    bool cleared = false;
    for (u8 c2 = 0; c2 < 6; c2++) {
        if (cleared) break;
        u16 n = (u16)CItemBlock_countKindSlots(cats.b[c2]);
        for (u16 j = 0; j < n; j++) {
            CItemInstance* obj = (CItemInstance*)CItem_kindPtr(cats.b[c2], (s16)j);
            if (obj == 0 || obj->word == 0) continue;
            u32 cnt = CItem_initItemImplInstances((CItemData*)obj)->vf30((CItemData*)obj);
            for (u8 k = 0; k < (u8)cnt; k++) {
                if ((s16)CItem_initItemImplInstances((CItemData*)obj)->vf40((CItemData*)obj, k)
                    == itemId) {
                    CItem_initItemImplInstances((CItemData*)obj)
                        ->vf44((CItemData*)obj, k, -1);
                    cleared = true;
                }
            }
            if (cleared) break;
        }
    }
    return 1;
}

// Map the current sort-menu page (unk_36c[unk_373]) to a sound-effect id and
// return its name string, or 0 when the page has no sound.
char* CEquipItemBox::getEIBPageHint() {
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
        return BdatTouchStringCell(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], val);
    }
    return 0;
}

// File-event handler: the three bind files (field_24 box layout, field_28 arc
// accessor, field_2C bdat) report completion through this callback. Each
// branch tears down the completed file's region and rebuilds the affected
// subsystem (layout/accessors for file 1, the second accessor for file 2, the
// bdat table for file 3); returns 1 when the handle matched, else 0.
bool CEquipItemBox::OnFileEvent(CEventFile* ev) {
    void* h = ev->mFileHandle;
    if (field_24 == h) {
        char* base = lbl_eu_8050EFDC;
        void* mem = (void*)mtl::MemManager::getHandleMEM2();
        mRegion04.createRegion((int)mem, 0x20000, base + 0x711, 1);
        Class_8045F858 host(&mRegion04);
        CFileHandle* fh = (CFileHandle*)field_24;
        void* buf = fh->mData;
        fh->mData = 0;
        mtl::MemManager::setMemInitFlag(false);
        field_30 = CLibLayout::createArcResourceAccessor();
        field_30->Attach(buf, base + 0x71f);
        buildLayout(&field_38, field_30, base + 0x723);
        bindLayoutAnimTransform(field_38, (nw4r::lyt::AnimTransform**)&field_3C, field_30, base + 0x734);
        nw4r::lyt::Pane* root = field_38->GetRootPane();
        void* font = CDeviceFont::getFontInfo(1, field_38);
        ApplyFontToTxtPanes(root, (const nw4r::ut::Font*)(((IDeviceFontInfo*)font)->getFont()));
        char* msg = (char*)getPackedFont();
        setLayoutTextBoxFont(field_38, base + 0x29b, (u32)msg);
        setLayoutTextBoxFont(field_38, base + 0x27c, (u32)msg);
        char* msg2 = (char*)CUICfManager_getPackedFont9C();
        for (u8 i = 1; i <= 0x1e; i++) {
            char buf2[0x20];
            sprintf(buf2, base + 0x219, i);
            setLayoutTextBoxFont(field_38, buf2, (u32)msg2);
        }
        field_38->SetAnimationEnable((nw4r::lyt::AnimTransform*)field_3C, true);
        field_38->Animate(0);
        char* s4 = BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 4);
        LayoutSetTextBoxFmtValue(field_38, base + 0x74c, s4, 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x2d4, base + 0x228, 0);
        const char* name = (cf::CfGameManager::isClassicController(-1) == 0) ? base + 0x761 : base + 0x758;
        u16 key = BdatGetU16ByTableKey(base + 0x76a, name, 0x49);
        char* texName = MakeTplNameSysFile(key);
        nw4r::lyt::ArcResourceAccessor* acc2 = CUICfManager_getArcResourceAccessor();
        void* tex = acc2->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, texName, 0);
        if (tex != 0) {
            PaneSetTexPaletteByName(field_38, base + 0x778, (TPLPalette*)((u32)tex));
            const TPLPalette* palette = (const TPLPalette*)tex;
            const TPLHeader* dims = palette->descriptorArray[0].textureHeader;
            u16 v2 = dims->width;
            u16 v0 = dims->height;
            nw4r::lyt::Pane* pane =
                field_38->GetRootPane()->FindPaneByName(base + 0x778, true);
            if (pane != 0) {
                float fw = (f32)(u32)v2;
                float fh = (f32)(u32)v0;
                ((float*)&pane->GetSize())[0] = fw;
                float size[2];
                size[0] = fw;
                size[1] = fh;
                ((float*)&pane->GetSize())[1] = fh;
            }
        }
        nw4r::lyt::Pane* posPane =
            field_38->GetRootPane()->FindPaneByName(base + 0x784, true);
        copyVEC3((float*)&unk_200,
                 (float*)&posPane->GetTranslate());
        GXColorS10 c0 = PaneMatGetS16QuadByName(field_38, base + 0x791, 0);
        CopyVec4s((GXColorS10*)equipItemBox_sortMenuColorSrcTop, &c0);
        GXColorS10 c1 = PaneMatGetS16QuadByName(field_38, base + 0x791, 1);
        CopyVec4s((GXColorS10*)equipItemBox_sortMenuColorSrcBottom, &c1);
        s16 v0c = equipItemBox_sortMenuColorSrcTop[3];
        s16 v1c = equipItemBox_sortMenuColorSrcBottom[3];
        equipItemBox_sortMenuTabNeutralTop[3] = v0c;
        equipItemBox_sortMenuTabNeutralBottom[3] = v1c;
        equipItemBox_sortMenuTabSelectedTop[3] = v0c;
        equipItemBox_sortMenuTabSelectedBottom[3] = v1c;
        equipItemBox_sortMenuTabColor2Top[3] = v0c;
        equipItemBox_sortMenuTabColor2Bottom[3] = v1c;
        equipItemBox_sortMenuTabColor3Top[3] = v0c;
        equipItemBox_sortMenuTabColor3Bottom[3] = v1c;
        // Cursor objects: construct stack temps and assign them (the implicit
        // CEIBCur::operator= is called out of line, weak in CPartyStateWin), then
        // dispatch initLayout (CEIBCur's empty dtor inlines away; the
        // page-cur temp keeps its out-of-line dtor call).
        {
            CEIBCur temp(field_30);
            mCur44 = temp;
        }
        mCur44.initLayout();
        {
            CEIBPageCur temp(field_30);
            mPageCur = temp;
        }
        mPageCur.initLayout();
        ccur18 = CCur18(CUICfManager_getArcResourceAccessor());
        ccur18.initLayout();
        char* msg3 = (char*)CUICfManager_getPackedFont9C();
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
        LayoutSetTextBoxFmtValue(field_38, base + 0x4bf, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0xa), 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x4ca, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0xd), 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x4d5, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x11), 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x4e0, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x12), 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x4eb, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0xc), 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x4f6, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x13), 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x5dc, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x18), 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x501, BdatTouchStringCell((char*)base + 0x2d, (char*)base + 0x36, 0x2f), 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x591, base + 0x228, 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x59b, base + 0x228, 0);
        LayoutSetTextBoxFmtValue(field_38, base + 0x5a5, base + 0x228, 0);
        nw4r::lyt::Pane* paneB =
            field_38->GetRootPane()->FindPaneByName(base + 0x4bf, true);
        GXColorS10 p1 = PaneMatGetS16Quad(paneB, 0);
        CopyVec4s((GXColorS10*)equipItemBox_cellLabelNeutralTop, &p1);
        GXColorS10 p2 = PaneMatGetS16Quad(paneB, 1);
        CopyVec4s((GXColorS10*)equipItemBox_cellLabelNeutralBottom, &p2);
        equipItemBox_cellLabelMutedTop[3] = equipItemBox_cellLabelNeutralTop[3];
        equipItemBox_cellLabelMutedBottom[3] = equipItemBox_cellLabelNeutralBottom[3];
        nw4r::lyt::Pane* pane5 =
            field_38->GetRootPane()->FindPaneByName(base + 0x522, true);
        p1 = PaneMatGetS16Quad(pane5, 0);
        CopyVec4s((GXColorS10*)equipItemBox_kind48LabelNeutralTop, &p1);
        p2 = PaneMatGetS16Quad(pane5, 1);
        CopyVec4s((GXColorS10*)equipItemBox_kind48LabelNeutralBottom, &p2);
        equipItemBox_kind48LabelMutedTop[3] = equipItemBox_kind48LabelNeutralTop[3];
        equipItemBox_kind48LabelMutedBottom[3] = equipItemBox_kind48LabelNeutralBottom[3];
        nw4r::lyt::Pane* pane79 =
            field_38->GetRootPane()->FindPaneByName(base + 0x79e, true);
        p1 = PaneMatGetS16Quad(pane79, 0);
        CopyVec4s((GXColorS10*)equipItemBox_sortOptionLabelNeutralTop, &p1);
        p2 = PaneMatGetS16Quad(pane79, 1);
        CopyVec4s((GXColorS10*)equipItemBox_sortOptionLabelNeutralBottom, &p2);
        equipItemBox_sortOptionLabelSelectedTop[3] = equipItemBox_sortOptionLabelNeutralTop[3];
        equipItemBox_sortOptionLabelSelectedBottom[3] = equipItemBox_sortOptionLabelNeutralBottom[3];
        equipItemBox_sortOptionLabelAltTop[3] = equipItemBox_sortOptionLabelNeutralTop[3];
        equipItemBox_sortOptionLabelAltBottom[3] = equipItemBox_sortOptionLabelNeutralBottom[3];
        closeEIBSortMenu();
        field_24 = 0;
        mRegion04.validateHeap();
        return 1;
    }
    if (field_28 == h) {
        char* base = lbl_eu_8050EFDC;
        void* mem = (void*)mtl::MemManager::getHandleMEM2();
        mRegion14.createRegion((int)mem, 0x100, base + 0x7a8, 1);
        Class_8045F858 host(&mRegion14);
        CFileHandle* fh2 = (CFileHandle*)field_28;
        void* buf2 = fh2->mData;
        fh2->mData = 0;
        mtl::MemManager::setMemInitFlag(false);
        field_34 = CLibLayout::createArcResourceAccessor();
        field_34->Attach(buf2, base + 0x71f);
        closeEIBSortMenu();
        field_28 = 0;
        mRegion14.validateHeap();
        return 1;
    }
    if (field_2C == h) {
        CFileHandle* fh3 = (CFileHandle*)field_2C;
        void* buf3 = fh3->mData;
        fh3->mData = 0;
        Bdat_GetTable_AA34();  // retail passes no args (r3 stale from the field_2C load)
        if (getFP(&lbl_eu_8050EFDC[0x7b9]) == 0) {
            CBdat::setBdatEntry(5, buf3);
        }
        equipItemBoxEquipListFilePtr = getFP(&lbl_eu_8050EFDC[0x7b9]);
        closeEIBSortMenu();
        field_2C = 0;
        return 1;
    }
    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static initialiser: reset/set the 24 .sbss colour-table entries used by the
// sort-menu page rebuild (PaneMatSetTevColorsByName pairs). SplitU32ToS16s clears a table,
// setGXColorS10 sets its RGBA values.
extern "C" void sinit_8028DAB0() {
    SplitU32ToS16s(equipItemBox_sortMenuColorSrcTop, 0);
    SplitU32ToS16s(equipItemBox_sortMenuColorSrcBottom, 0);
    setGXColorS10((GXColorS10*)equipItemBox_sortMenuTabNeutralTop, 0x79, 0x49, 0x7, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortMenuTabNeutralBottom, 0xed, 0xcd, 0x83, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortMenuTabSelectedTop, 0x1a, 0x43, 0x53, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortMenuTabSelectedBottom, 0xc4, 0xe8, 0xeb, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortMenuTabColor2Top, 0x74, 0x54, 0x1d, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortMenuTabColor2Bottom, 0xd5, 0xb9, 0x78, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortMenuTabColor3Top, 0x3d, 0x68, 0x78, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortMenuTabColor3Bottom, 0xc4, 0xe8, 0xeb, 0x0);
    SplitU32ToS16s(equipItemBox_cellLabelNeutralTop, 0);
    SplitU32ToS16s(equipItemBox_cellLabelNeutralBottom, 0);
    setGXColorS10((GXColorS10*)equipItemBox_cellLabelMutedTop, 0x80, 0x80, 0x80, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_cellLabelMutedBottom, 0x80, 0x80, 0x80, 0x0);
    SplitU32ToS16s(equipItemBox_kind48LabelNeutralTop, 0);
    SplitU32ToS16s(equipItemBox_kind48LabelNeutralBottom, 0);
    setGXColorS10((GXColorS10*)equipItemBox_kind48LabelMutedTop, 0xff, 0xff, 0xfa, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_kind48LabelMutedBottom, 0x80, 0x80, 0x80, 0x0);
    SplitU32ToS16s(equipItemBox_sortOptionLabelNeutralTop, 0);
    SplitU32ToS16s(equipItemBox_sortOptionLabelNeutralBottom, 0);
    setGXColorS10((GXColorS10*)equipItemBox_sortOptionLabelSelectedTop, 0x12, 0xa3, 0xe7, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortOptionLabelSelectedBottom, 0xff, 0xff, 0xff, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortOptionLabelAltTop, 0xb3, 0x9, 0xc0, 0x0);
    setGXColorS10((GXColorS10*)equipItemBox_sortOptionLabelAltBottom, 0xff, 0xff, 0xff, 0x0);
}

// Rebuild the equip grid for a page: clear all 0x400 cells (per-cell byte 6),
// then walk the page's items and populate the cells with their slot state.
// Category 3 cross-references the six equipment categories; the general path
// (categories 2 and 4..8) matches item slots directly. Ends by recomputing the
// page count and dispatching sortEIBGridByPage.
void CEquipItemGrid::rebuildEIBGridPage(u8 v, u8 b, u8 hi) {
    count = 0;
    for (u16 i = 0; i < 0x400; i++) {
        CEquipItemData temp;
        copyEIBItem(&data[i], setEIBItem(&temp, -1, 0, 0, 0, 0, 0, 0));
        field_2026[i] = 6;
    }
    u32 total = CItemBlock_countKindSlots(v);
    CEquipItemBoxCatWordTable catsTemp = lbl_eu_8050EF50;
    const char* base = lbl_eu_8050EFDC;
    for (u16 n = 0; n < (u16)total; n++) {
        CItemInstance* obj = (CItemInstance*)CItem_kindPtr(v, (s16)n);
        if (obj == 0 || obj->word == 0) continue;
        u16 slotIdx = count;
        count = slotIdx + 1;
        data[slotIdx].unk0 = (s16)n;
        CEquipItemData* cell = &data[slotIdx];
    if (v == 3) {
            // Category 3: find which equipment slot (if any) owns this item id.
            CEquipItemBoxCatWordTable cats = catsTemp;
            for (u8 k = 0; k < 6; k++) {
                u8 cat = (u8)cats.w[k];
                u16 cnt2 = (u16)CItemBlock_countKindSlots(cat);
                u32 kplus3 = (u32)(k + 3);
                for (u16 m = 0; m < cnt2; m++) {
                    CItemInstance* obj2 = (CItemInstance*)CItem_kindPtr(cat, (s16)m);
                    if (obj2 == 0 || obj2->word == 0) continue;
                    CItemImpl* impl2 = CItem_initItemImplInstances((CItemData*)obj2);
                    u16 num = (u16)impl2->vf30((CItemData*)obj2);
                    for (u16 p = 0; p < num; p++) {
                        s16 slot = impl2->vf40((CItemData*)obj2, (u8)p);
                        if (slot == -1) continue;
                        if (slot != cell->unk0) continue;
                        cell->unk3 = 2;
                        u16 m2 = m;
                        for (u8 q = 0; q < (u8)code80135FDC_getByte_64077(); q++) {
                            u8 catB = (u8)GetCollectedFlagByte(q);
                            CItemInstance* obj3 = (CItemInstance*)CtrlObjectParam_GetCharEntry(catB);
                            s16 slot2 = 0;
                            switch (cat) {
                            case 2: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[5]; break;
                            case 4: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[0]; break;
                            case 5: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[1]; break;
                            case 6: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[2]; break;
                            case 7: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[3]; break;
                            case 8: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[4]; break;
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
            u8 name1 = BdatGetU8Direct((const void*)lbl_eu_806640D8, (char*)base, (u16)CItem_initItemImplInstances((CItemData*)obj)->vf54((CItemData*)obj));
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
            case 2: g2 = (u32)lbl_eu_806640F4; break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8: g2 = (u32)lbl_eu_806640F8; break;
            }
            char buf[0x28];
            if (v == 2) {
                sprintf(buf, (char*)base + 0x7, b);
            } else {
                sprintf(buf, (char*)base + 0x12, b);
            }
            u16 kind2 = BdatGetItemId(obj->word >> 20);
            if (BdatGetU8Direct((const void*)g2, (const char*)buf, kind2) != 0) cell->unk6 = 1;
            if (v == 2 && b == 0xb && (obj->word >> 20) == 0x4c) cell->unk6 = 1;
            if (v != 2) {
                u8 name3 = BdatGetU8Direct((const void*)lbl_eu_806640F8, (char*)base + 0x17, BdatGetItemId(obj->word >> 20));
                if ((u32)(name3 - 4) <= 9) {
                    /* no-op */
                } else if (name3 == 3) {
                    if (Counter_TestBit((u8*)CtrlObjectParam_GetCharEntry(b) + 0x3534, 0x85) == 0) cell->unk6 = 0;
                }
                if (name3 == 2) {
                    if (Counter_TestBit((u8*)CtrlObjectParam_GetCharEntry(b) + 0x3534, 0x84) == 0) cell->unk6 = 0;
                }
            }
            // Second category scan: match the cell's item id against the
            // equipment slot table of every category.
            for (u8 q = 0; q < (u8)code80135FDC_getByte_64077(); q++) {
                u8 catB = (u8)GetCollectedFlagByte(q);
                CItemInstance* obj3 = (CItemInstance*)CtrlObjectParam_GetCharEntry(catB);
                s16 slot2 = 0;
                switch (v) {
                case 2: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[5]; break;
                case 4: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[0]; break;
                case 5: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[1]; break;
                case 6: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[2]; break;
                case 7: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[3]; break;
                case 8: slot2 = ((CItemBoxCharEntry*)obj3)->slotIds[4]; break;
                }
                if (slot2 == cell->unk0) {
                    cell->unk3 = 1;
                    cell->unk2 = catB;
                    break;
                }
            }
        }
tail:
        u8 name4 = BdatGetU8Direct((const void*)lbl_eu_806640EC, (char*)base + 0x20, obj->word >> 20);
        cell->unk5 = ((u8)name4 == 1) ? 1 : 0;
        if (CtrlRemote_TouchBitByArg(0x3508) != 0) cell->unk5 = 0;
    }
    // Page-count update when the category changed (manual ceil: fctiwz +
    // fractional-part round-up, clamped to >= 1).
    if (cat != v) {
        cat = v;
        u32 cnt3 = count;
        float fdiv = (float)(s32)cnt3 / 30.0f;
        s32 rows = (s32)fdiv;
        float frac = fdiv - (float)rows;
        if (lbl_eu_80668B04 != frac) rows++;
        if (!(u8)rows) rows = 1;
        _pad2003 = (u8)rows;
        idx = 0;
    }
    if ((u32)(v + 0xfe) <= 6) {
        // Mark the used equipment slots (unk7 = 1) and compute the per-cell
        // count threshold.
        u32 cnt = CItemBlock_countKindSlots(v);
        u32 base = CItem_slotCnt(v);
        s32 last = (s32)(cnt - 1);
        s32 diff = (s32)(cnt - base);
        s32 i2;
        for (i2 = last; i2 >= diff; i2--) {
            data[i2].unk7 = 1;
        }
        int threshold = (int)(0.2f * (float)(s32)cnt);
        for (i2 = 0;; i2++) {
            int val = (i2 + 1) * 6;
            int diff2 = threshold - val;
            if (diff2 >= 0) {
                field_2026[i2] = (u8)diff2;
            } else {
                field_2026[i2] = (u8)(diff2 + 6);
                break;
            }
        }
    }
    sortEIBGridByPage();
}
// Page count for the current category: min(10, ceil(count / pagesize)).
// The (f32)(int) casts produce retail's 0x4330/xoris/lfd/fsubs s32->f32
// magic idiom; the named temp 'frac' is required to get retail's register
// allocation (constant into f0, conversion chain into f1).
u8 CEquipItemGrid::countEIBPages() {
    f32 pages = (f32)(int)CItemBlock_countKindSlots(cat);
    if (pages >= 200.0f) {
        return 10;
    }
    f32 q = pages / 30.0f;
    int n = (int)q;
    f32 frac = q - (f32)n;
    if (lbl_eu_80668B04 != frac) {
        n = n + 1;
    }
    return (u8)n;
}
