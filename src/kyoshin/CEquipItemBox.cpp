// Auto-scaffolded catalog TU for kyoshin/CEquipItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipItemBox.hpp"
#include <nw4r/lyt/lyt_layout.h>

// --- referenced retail symbols (C linkage: plain symbol names) ---
// C-linkage imports live in kyoshin/CEquipItemBox.hpp.
// func_80157C4C stays inline: CEquipChange.hpp / CItemBoxInfo.hpp declare it
// with different signatures and CEquipChange.cpp includes both (via
// CEquipChange.hpp -> CEquipItemBox.hpp).
extern "C" void* func_80157C4C(u8, s16);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
// Float constant in .sdata2, referenced via sda21 reloc.
extern const float lbl_eu_80668B28;

u8 CEquipItemBox::func_802865A0() { return unk_40; }

#pragma push
#pragma auto_inline off
// Initialise a full item grid: clear every cell and reset the header fields.
extern "C" CEquipItemGrid* func_802824B4(CEquipItemGrid* grid) {
    // Reset each 8-byte cell to { unk0 = -1, rest = 0 }.
    CEquipItemData* p = grid->data;
    CEquipItemData* const end = &grid->data[0x400];
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
    u16 i = 0;
    do {
        CEquipItemData temp;
        func_80282594(&grid->data[i], func_80282574(&temp, -1, 0, 0, 0, 0, 0, 0));
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
extern "C" void* func_80282F34(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        u32* obj = (u32*)func_80157C4C(grid->cat, grid->data[offset].unk0);
        if (obj && *obj) {
            return obj;
        }
    }
    return 0;
}

// Return the item kind (word >> 20) for the grid element, or 0.
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

// Return the stored item short for a valid grid cell, or -1.
extern "C" s16 func_80282E4C(CEquipItemGrid* grid, u16 idx) {
    u16 offset = (u16)((s8)grid->idx * 0x1e + idx);
    if ((u32)offset < (u32)grid->count) {
        u32* obj = (u32*)func_80157C4C(grid->cat, grid->data[offset].unk0);
        if (obj && *obj) {
            return grid->data[offset].unk0;
        }
    }
    return -1;
}

// Advance grid cursor index; wrap to 0 when it reaches the count (field_2003).
void CEquipItemBox::func_80282DF8() {
    s8 v = field_2004 + 1;
    field_2004 = v;
    if (v >= field_2003) {
        field_2004 = 0;
    }
}

// Step grid cursor index backwards; wrap to (count-1) when it goes below 0.
void CEquipItemBox::func_80282E24() {
    s8 v = field_2004 - 1;
    field_2004 = v;
    if (v < 0) {
        field_2004 = field_2003 - 1;
    }
}

extern "C" void func_80282FA0(){}

// Return the grid cell's byte 3 when its item object is valid, else 0.
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
extern "C" u8 func_80283208(CEquipItemGrid* grid, u16 idx) {
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

// Fetch the byte at grid index field_2004; bounds-guard against 0x400.
u8 CEquipItemBox::func_80283280(u32 param) {
    s8 idx = field_2004;
    int off = (idx * 30 + (int)param) & 0xffff;
    if (off >= 0x400) return 0;
    return field_2026[off * 8];
}

// Fetch byte at grid index field_2004, or 0 if index >= 0x400.
u8 CEquipItemBox::func_802832B4() {
    s8 idx = field_2004;
    if (idx >= 0x400) return 0;
    return field_2026[idx];
}

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

extern "C" void func_80283350(){}

extern "C" void func_8028345C(){}

extern "C" void func_80283B24(CEquipItemData* dst, const CEquipItemData* src) {
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk3 = src->unk3;
    dst->unk4 = src->unk4;
    dst->unk5 = src->unk5;
    dst->unk6 = src->unk6;
    dst->unk7 = src->unk7;
}

extern "C" void func_80283B60(){}

extern "C" void func_80283E64(){}

extern "C" void func_80284144(){}

extern "C" void func_80284244(){}

extern "C" void func_80284358(){}

extern "C" void func_80284490(){}

extern "C" void func_8028461C(){}

extern "C" void func_802847A4(){}

extern "C" void func_802848C4(){}

extern "C" void func_80284A00(){}

// Destructor clone: free object memory when this is non-null and delete flag set.
void* __dt__802825D0(void* self, int mode) {
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

extern "C" void func_80284B18(){}

extern "C" void func_80284C30(){}

extern "C" void func_80284DCC(){}

extern "C" void func_80284F1C(){}

extern "C" void func_8028506C(){}

extern "C" void func_802851BC(){}

extern "C" void func_8028530C(){}

// Bubble-sort the grid entries ascending by the item name lookup byte.
extern "C" void func_80285478(CEquipItemGrid* grid) {
    for (u16 i = 0; i < grid->count - 1; i++) {
        int swapped = 0;
        for (u16 j = 0; j < grid->count - 1 - i; j++) {
            CEquipItemData* a = &grid->data[j];
            CEquipItemData* b = &grid->data[j + 1];
            u32* objA = (u32*)func_80157C4C(grid->cat, a->unk0);
            u32* objB = (u32*)func_80157C4C(grid->cat, b->unk0);
            u16 kindA = func_80139358(*objA >> 20);
            u16 kindB = func_80139358(*objB >> 20);
            u32 nameA = func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x82], kindA);
            if ((u8)nameA > (u8)func_801361E8((u32)lbl_eu_806640F8, &lbl_eu_8050EFDC[0x82], kindB)) {
                CEquipItemData tA;
                CEquipItemData tB;
                func_80283B24(&tA, a);
                func_80283B24(&tB, b);
                func_80282594(a, &tB);
                func_80282594(b, &tA);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

extern "C" void func_802855C8(){}

extern "C" void func_80285708(){}

extern "C" void func_802857F0(){}

extern "C" void func_80285890(){}

CEIBCur::CEIBCur(void* arcResAcc) {
    mVtable = (void*)lbl_eu_80538704;
    mArcResAcc = arcResAcc;
    mpLayout = 0;
    mpAnimTrans0 = 0;
    mpAnimTrans1 = 0;
    mActive = 0;
    mVisible = 0;
}

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

extern "C" void func_80285A90(CEIBCur* self, nw4r::lyt::DrawInfo* drawInfo) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mpLayout;
    if (layout == nullptr) return;
    if (self->mActive == 0) return;
    func_80137038(layout, drawInfo, 0, 1);
}

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

extern "C" void func_80285B70(CEIBCur* self){}
#pragma pop


CEIBPageCur::CEIBPageCur(void* arcResAcc) : CEIBCur(arcResAcc) {
    mVtable = (void*)lbl_eu_805386EC;
}

// Destructor clone: free object memory when this is non-null and delete flag set.
void* __dt__80285C44(void* self, int mode) {
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

extern "C" void func_80285C84(){}

extern "C" void __ct__CEquipItemBox(){}

CEquipItemBox::~CEquipItemBox() {}

extern "C" void func_802861A8(){}

extern "C" void func_80286264(){}

extern "C" void func_80286340(){}


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

extern "C" void func_802865B0(){}

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

extern "C" void func_80286740(){}

extern "C" void func_802867E0(){}

extern "C" void func_802869B4(){}

extern "C" void func_80286B94(){}

extern "C" void func_80286D7C(){}

extern "C" void func_80286F6C(){}

extern "C" void func_80287024(){}

extern "C" void func_802870DC(){}

extern "C" void func_80287250(){}

extern "C" void func_802873D8(){}

// Advance page-2 system-window state while it is visible and active.
void CEquipItemBox::func_80287D58() {
    if (unk_375 != 0 && CSysWin_getUnk34(_padSysWin2) != 0
        && CSysWin_isActive(_padSysWin2) != 0) {
        func_8022B8E4(_padSysWin2);
    }
}

extern "C" void func_80287DB4(){}

u8 CEquipItemBox::func_80287EE8() {
    u8 val = unk_1fe;
    unk_1fe = 0;
    return val;
}

void CEquipItemBox::func_80287EFC(u32 val) { unk_20c = val; }

extern "C" void func_80287F04(){}

extern "C" void func_80287FE0(){}

extern "C" void func_802882A4(){}

extern "C" void func_8028847C(){}

extern "C" unsigned char func_80288530(u8* self) {
    s8 idx = *(s8*)(self + 0x373);
    return *(unsigned char*)(self + 0x36c + idx);
}

extern "C" void func_80288544(){}

extern "C" void func_802886D8(){}

extern "C" void func_8028876C(){}

// Returns 1 when the lookup byte of the grid cell selected by (unk_1f5) equals
// func_801392B4's category byte; an unk_1f5 of -1 always returns 0.
extern "C" u8 func_80288948(CEquipItemBox* self) {
    s8 row = (s8)self->unk_1f5;
    if (row == -1) return 0;
    int idx = row * 5 + (int)self->unk_1f4;
    int cat = (self->unk_1fc >> 16) & 0xff;
    u8 a = func_802832D8((CEquipItemGrid*)((u8*)self + 0x37E), (u8)idx);
    u8 b = func_801392B4(cat);
    return (a == b);
}

// Play the entry animation at 0x3C; when it completes mark the cursors active.
extern "C" void func_802889C0(CEquipItemBox* self) {
    float f = lbl_eu_80668B28;
    if (func_80137444((nw4r::lyt::AnimTransform*)self->field_3C, f) != 0) {
        self->unk_41 = 2;
        self->unk_43 = 1;
        self->unk_58 = 1;
        self->unk_70 = 1;
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

extern "C" void func_8028939C(){}

#pragma push
#pragma auto_inline off
extern "C" void func_80289500(CEquipItemBox* self, int a) {}

extern "C" void func_80289754(){}

extern "C" void func_80289AA4(){}

extern "C" void func_80289CC0(CEquipItemBox* self){}
#pragma pop

extern "C" void func_80289E70(CEquipItemBox* self){}

void CEquipItemBox::func_8028A07C() {
    memset(unk_36c, 0, 6);
    unk_372 = 0;
    unk_373 = 0;
}

void CEquipItemBox::func_8028A0C0(u8 val) {
    u8 idx = unk_372;
    if (idx >= 6) return;
    unk_36c[idx] = val;
    unk_372 = idx + 1;
}

// Step the sort-menu page selection backwards (wrapping from the first page to
// the last) with the current page stored in unk_373.
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

extern "C" void func_8028A1DC(){}

extern "C" void func_8028A374(){}

#pragma push
#pragma auto_inline off
extern "C" void func_8028A5D8(CEquipItemBox* self, int a) {}


extern "C" void func_8028A9CC(){}

extern "C" void func_8028AA64(CEquipItemBox* self){}

extern "C" void func_8028AF98(){}

extern "C" void func_8028B7CC(){}

extern "C" void func_8028BE74(){}
#pragma pop


extern "C" void func_8028C280(){}

extern "C" void func_8028CBCC(){}

// Return a sound/SE id (0x33-0x3e) derived from the page byte, or tail-call a
// string lookup for the current page when a valid id was selected.
extern "C" char* func_8028D0EC(CEquipItemBox* self) {
    u32 val = 0;
    u8 v = self->unk_36c[(s8)self->unk_373];
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
    case 0x0b: val = 0x3a; break;
    case 0x0d: break;
    }
    if (val == 0) return 0;
    return func_80136190(&lbl_eu_8050EFDC[0x2d], &lbl_eu_8050EFDC[0x36], val);
}

extern "C" void OnFileEvent__13CEquipItemBoxFP10CEventFile(){}

void CEquipItemBox::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8028DAB0(){}

extern "C" void func_80282610() {}
extern "C" void func_80282D60() {}
