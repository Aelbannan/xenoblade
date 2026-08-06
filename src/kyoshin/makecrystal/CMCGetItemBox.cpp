// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCGetItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/makecrystal/CMCGetItemBox.hpp"

// C-linkage menu/widget helpers (retail symbols are unmangled).
extern "C" {
void* CItem_initItemImplInstances(void*);
void func_801D216C(CMCItemBoxSubObj*, int);
void func_801599D4(CMCItemBoxEntry*, int);
u32 CSysWin_isReady(CMCGetItemBoxSysWin*);
u32 CSysWin_getUnk34(CMCGetItemBoxSysWin*);
int  CSysWin_isActive(CMCGetItemBoxSysWin*);
void func_8022B8E4(CMCGetItemBoxSysWin*);
u32 func_80157CD0(u8);
void advanceItemBoxState__FP12CItemBoxInfo(CItemBoxInfo*);
}

// Foreign retail helpers (C++ visibility matched through the symbol map).
namespace nw4r { namespace lyt { class AnimTransform; } }
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
void func_80138078(u32);
extern float lbl_eu_80668BF0;

// Same-unit helper functions (implemented as stubs below).
void func_8029967C(CMCGetItemBox*);
void func_802998C8(CMCGetItemBox*);
void func_802999B0(CMCGetItemBox*);

// Initialise a CMCItemBoxSub: clear the offset table to 0xFFFF, zero the
// counters and index fields, then reset the whole table again.
void func_80296B44(CMCItemBoxSub* x) {
    for (int i = 0; i < 0x80; i++) x->table[i] = -1;
    x->count = 0;
    x->pad_102 = 0;
    x->limit = 0;
    x->counter = 0;
    x->field_108 = 0;
    x->field_148 = 0;
    x->field_14C = 0;
    x->field_1CC = 0;
    x->listBase = (CMCItemBoxEntry*)0;
    x->field_1D4 = 0;
    for (int i = 0; i < 0x80; i++) x->table[i] = -1;
}

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

s8 func_80296E00(CMCItemBoxSub* x, u32 index) {
    CMCItemBoxEntry* base = x->listBase;
    if (base == 0) return 0;
    u32 idx = (u16)(index + (s8)x->counter * 30);
    if (idx >= x->count) return 0;
    s16 off = x->table[idx];
    CMCItemBoxEntry* p = base + off;
    if (!p) return 0;
    void* impl = CItem_initItemImplInstances(p);
    return (s8)(u16)((u16(*)(void*, void*))(*(void***)impl)[2])(impl, p);
}

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

// When the item-box widget is done (state 3), advance to state 4, detach the
// helper widgets and advance the item-box state machine.
void func_80297E18(CMCGetItemBox* self) {
    if (self->field_4D != 3) return;
    self->field_4D = 4;
    self->mField55 = 0;
    func_801D216C(&self->subObj_58, 0);
    func_801D216C(&self->subObj_70, 0);
    func_801D216C(&self->subObj_88, 0);
    advanceItemBoxState__FP12CItemBoxInfo(&self->itemBox);
    func_80138078(0x6);
}

void func_80297E90(){}

void func_80297FB4(){}

void func_802980DC(){}

void func_80298228(){}

// Increment the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
void func_80298378(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    func_80296D00(x);
    func_8029967C(self);
    func_802998C8(self);
    u8 lim = x->limit;
    if (lim == 0) lim = 1;
    if (lim != 1) func_80138078(0xa);
}

// Decrement the sub counter, refresh helper widgets, and play a sound when the
// item-box limit is anything other than 1.
void func_802983E4(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    func_80296D2C(x);
    func_8029967C(self);
    func_802998C8(self);
    u8 lim = x->limit;
    if (lim == 0) lim = 1;
    if (lim != 1) func_80138078(0xa);
}

// Toggle the item-box help window: close it when active, otherwise detach the
// helper widgets when the layout flag is set.
void func_80298450(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) {
        if (CSysWin_isActive(&self->sysWin_B8) != 0) {
            func_8022B8E4(&self->sysWin_B8);
            func_801D216C(&self->subObj_A0, 1);
        }
    } else {
        if (self->mField303 != 0) {
            self->mField303 = 0;
            func_801D216C(&self->subObj_58, 1);
            func_801D216C(&self->subObj_A0, 0);
            func_80138078(0x6);
        }
    }
}

// When the sys-win is both present and active, close it and reset the A0 widget.
void func_802984E4(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) == 0) return;
    if (CSysWin_isActive(&self->sysWin_B8) == 0) return;
    func_8022B8E4(&self->sysWin_B8);
    func_801D216C(&self->subObj_A0, 1);
}

// Unless the widget is busy and the sort field is -1, report the selected
// item-box entry's place relative to its full range.
u32 func_80298540(CMCGetItemBox* self) {
    if (self->field_4D == 0) return 0;
    if ((s8)self->field_301 == -1) {
        u16 count = self->sub_314.count;
        u16 v = (u16)func_80157CD0(self->sub_314.pad_102);
        return (v >= count) ? 1 : 2;
    }
    return 0;
}

// Visit every item-box entry and hand it to the C-linkage cleanup helper.
void func_802985B4(CMCGetItemBox* self) {
    CMCItemBoxSub* x = &self->sub_314;
    for (u32 i = 0; (u16)i < x->count; i++) {
        func_801599D4(func_80296DB0(x, (u16)i), 0);
    }
}

void func_8029860C(void* self) {}

void func_80298614(){}

// Return a UI part index: 0 when the window exists, 0x36 when the layout flag
// is set, otherwise 0x39/0x3A based on the sign of the sort field.
u32 func_80298850(CMCGetItemBox* self) {
    if (CSysWin_getUnk34(&self->sysWin_B8) != 0) return 0;
    if (self->mField303 != 0) return 0x36;
    return 0x39 + ((s8)self->field_301 == -1);
}

void func_802988BC(){}

void func_80298938(){}

void func_802989A4(){}

void func_80298A20(){}

void func_80298A78(){}

void func_80298AC8(){}

void func_80298FB4(){}

void func_80299490(){}

void func_80299530(){}

void func_8029967C(CMCGetItemBox*) {}

void func_802998C8(CMCGetItemBox*) {}

void func_802999B0(CMCGetItemBox*) {}

void CMCGetItemBox::OnFileEvent() {}