// Auto-scaffolded catalog TU for kyoshin/CEquipItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CEquipItemBox.hpp"

u8 CEquipItemBox::func_802865A0() { return unk_40; }










extern "C" void func_80282F34(){}

extern "C" void func_80282FA0(){}

extern "C" void func_80283118(){}

extern "C" void func_80283190(){}

extern "C" void func_80283208(){}

extern "C" void func_80283280(){}

extern "C" void func_802832B4(){}

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

extern "C" void __ct__CEIBCur(){}

extern "C" void __dt__80285954(){}

extern "C" void func_80285994(){}

extern "C" void func_80285A18(){}

extern "C" void func_80285A90(){}

extern "C" void func_80285ABC(){}

extern "C" void func_80285B24(){}

extern "C" void func_80285B70(){}

extern "C" void __ct__CEIBPageCur(){}

extern "C" void __dt__80285C44(){}

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

extern "C" void func_802866A0(){}

extern "C" void func_802866E8(){}

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

extern "C" void func_80287D58(){}

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

extern "C" void func_80288A1C(){}

extern "C" void func_80288A6C(){}

extern "C" void func_80288AC0(){}

extern "C" void func_80288B14(){}

extern "C" void func_80288E14(){}

extern "C" void func_802891B8(){}

extern "C" void func_8028939C(){}

extern "C" void func_80289500(){}

extern "C" void func_80289754(){}

extern "C" void func_80289AA4(){}

extern "C" void func_80289CC0(){}

extern "C" void func_80289E70(){}

void CEquipItemBox::func_8028A07C() {
    memset(unk_36c, 0, 6);
    unk_372 = 0;
    unk_373 = 0;
}

extern "C" void func_8028A0C0(){}

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
