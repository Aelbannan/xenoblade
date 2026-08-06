// Auto-scaffolded catalog TU for kyoshin/CEquipItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipItemBox.hpp"

// --- referenced retail symbols (C linkage: plain symbol names) ---
extern "C" void* __dl__FPv(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" void func_801D216C(void*, int);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_80139198(void*);
extern "C" void func_80138078(u32);
extern "C" void func_8022B8E4(void*);
extern "C" void func_80285B70(CEIBCur*);
extern "C" void func_80289CC0(CEquipItemBox*);
extern "C" void func_80289E70(CEquipItemBox*);
// Float constant in .sdata2, referenced via sda21 reloc.
extern float lbl_eu_80668B28;

u8 CEquipItemBox::func_802865A0() { return unk_40; }

// Write a CEquipItemData from 8 separate scalar inputs.
extern "C" void func_80282574(CEquipItemData* dst, s16 a0, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7) {
    dst->unk0 = a0;
    dst->unk2 = a2;
    dst->unk3 = a3;
    dst->unk4 = a4;
    dst->unk5 = a5;
    dst->unk6 = a6;
    dst->unk7 = a7;
}

// Copy a CEquipItemData struct (src -> dst).
extern "C" void func_80282594(CEquipItemData* dst, const CEquipItemData* src) {
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk3 = src->unk3;
    dst->unk4 = src->unk4;
    dst->unk5 = src->unk5;
    dst->unk6 = src->unk6;
    dst->unk7 = src->unk7;
}










extern "C" void func_80282F34(){}

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

extern "C" void func_80283118(){}

extern "C" void func_80283190(){}

extern "C" void func_80283208(){}

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

extern "C" void func_802832D8(){}

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

extern "C" void func_80285478(){}

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

extern "C" void func_80285A18(){}

extern "C" void func_80285A90(CEIBCur* self, nw4r::lyt::DrawInfo* drawInfo) {
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mpLayout;
    if (layout == nullptr) return;
    if (self->mActive == 0) return;
    func_80137038(layout, drawInfo, 0, 1);
}

extern "C" void func_80285ABC(){}

void CEIBCur::func_80285B24() {
    // Advance the layout's animation; when the animation finishes, hide the
    // cursor and update the page state.
    if (func_80137444((nw4r::lyt::AnimTransform*)mpAnimTrans1, lbl_eu_80668B28) != 0) {
        mVisible = 0;
        func_80285B70(this);
    }
}

extern "C" void func_80285B70(CEIBCur* self){}

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

extern "C" void func_80286454(){}

extern "C" void func_8028652C(){}


u8 CEquipItemBox::func_802865A8() { return unk_43; }

extern "C" void func_802865B0(){}

extern "C" int CSysWin_getUnk34(void*);
extern "C" int func_801D3320(void*);

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
    func_80138078(0x6d);
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

extern "C" unsigned char func_80288530(void* self) {
    s8 idx = *(s8*)((u8*)self + 0x373);
    return *(unsigned char*)((u8*)self + 0x36c + idx);
}

extern "C" void func_80288544(){}

extern "C" void func_802886D8(){}

extern "C" void func_8028876C(){}

extern "C" void func_80288948(){}

extern "C" void func_802889C0(){}

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
        func_801D216C(&_pad44[0x30], 1);
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

extern "C" void func_80289500(){}

extern "C" void func_80289754(){}

extern "C" void func_80289AA4(){}

extern "C" void func_80289CC0(CEquipItemBox* self){}

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

extern "C" void func_8028A0E0(){}

extern "C" void func_8028A160(){}

extern "C" void func_8028A1DC(){}

extern "C" void func_8028A374(){}

extern "C" void func_8028A5D8(){}

extern "C" void func_8028A9CC(){}

extern "C" void func_8028AA64(){}

extern "C" void func_8028AF98(){}

extern "C" void func_8028B7CC(){}

extern "C" void func_8028BE74(){}

extern "C" void func_8028C280(){}

extern "C" void func_8028CBCC(){}

extern "C" void func_8028D0EC(){}

extern "C" void OnFileEvent__13CEquipItemBoxFP10CEventFile(){}

void CEquipItemBox::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8028DAB0(){}
