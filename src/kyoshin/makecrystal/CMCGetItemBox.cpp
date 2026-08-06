// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCGetItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CMCGetItemBox.hpp"

// C-linkage menu/widget helpers (retail symbols are unmangled).
extern "C" {
void func_801D216C(CMCItemBoxSubObj*, int);
void func_801599D4(CMCItemBoxEntry*, int);
u32 CSysWin_isReady(CMCGetItemBoxSysWin*);
u32 CSysWin_getUnk34(CMCGetItemBoxSysWin*);
int  CSysWin_isActive(CMCGetItemBoxSysWin*);
void func_8022B8E4(CMCGetItemBoxSysWin*);
}

void func_80296B44(){}

void __dt__80296BB0(){}

void func_80296BF0(){}

// Advance the counter; wrap to 0 when past the signed byte limit.
void func_80296D00(CMCItemBoxSub* x) {
    u8 v = (u8)(x->counter + 1);
    x->counter = v;
    if ((s8)v < x->limit) return;
    x->counter = 0;
}

// Count the counter down; wrap to (limit-1) when it goes negative.
void func_80296D2C(CMCItemBoxSub* x) {
    u8 v = (u8)(x->counter - 1);
    x->counter = v;
    if ((s8)v >= 0) return;
    x->counter = (u8)(x->limit - 1);
}

// Look up the entry at `index` in the offset table and return the derived
// index-table word >> 20, or 0 when out of range.
u32 func_80296D54(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u32 idx = (u16)(index + (s8)x->counter * 30);
    if (idx >= x->count) return 0;
    s16 off = x->table[idx];
    CMCItemBoxEntry* p = base + off;
    if (!p) return 0;
    return p->field_00 >> 20;
}

// Look up the entry at `index` in the offset table and return its pointer.
CMCItemBoxEntry* func_80296DB0(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u32 idx = (u16)(index + (s8)x->counter * 30);
    if (idx >= x->count) return 0;
    s16 off = x->table[idx];
    CMCItemBoxEntry* p = base + off;
    return p;
}

void func_80296E00(){}

void func_80296E98(){}

void func_80296FC0(){}

void __ct__CMCGetItemBox(){}

CMCGetItemBox::~CMCGetItemBox() {}

void func_80297928(){}

void func_802979E4(){}

void func_80297AAC(){}

void func_80297B68(){}

// Return 0 unless the item box is active and the sys-win is ready.
u8 func_80297CC0(CMCGetItemBox* self) {
    if (getItemBoxState(&self->itemBox) == 0) return 0;
    if (CSysWin_isReady(&self->sysWin_B8) == 0) return 0;
    return self->field_54;
}

u8 CMCGetItemBox::func_80297D1C() { return mField55; }

u8 CMCGetItemBox::func_80297D24() { return mField303; }

void func_80297D2C(){}

void func_80297E18(){}

void func_80297E90(){}

void func_80297FB4(){}

void func_802980DC(){}

void func_80298228(){}

void func_80298378(){}

void func_802983E4(){}

void func_80298450(){}

// When the sys-win is both present and active, close it and reset the A0 widget.
void func_802984E4(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) == 0) return;
    if (CSysWin_isActive(&self->sysWin_B8) == 0) return;
    func_8022B8E4(&self->sysWin_B8);
    func_801D216C(&self->subObj_A0, 1);
}

void func_80298540(){}

// Visit every item-box entry and hand it to the C-linkage cleanup helper.
void func_802985B4(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    for (u32 i = 0; (u16)i < x->count; i++) {
        func_801599D4(func_80296DB0(x, (u16)i), 0);
    }
}

void func_8029860C(void* self) {}

void func_80298614(){}

void func_80298850(){}

void func_802988BC(){}

void func_80298938(){}

void func_802989A4(){}

void func_80298A20(){}

void func_80298A78(){}

void func_80298AC8(){}

void func_80298FB4(){}

void func_80299490(){}

void func_80299530(){}

void func_8029967C(){}

void func_802998C8(){}

void func_802999B0(){}

void CMCGetItemBox::OnFileEvent() {}