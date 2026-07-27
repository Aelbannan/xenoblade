// Auto-scaffolded catalog TU for kyoshin/CEquipItemBox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_802865A0(void* self) { return ((u8*)self)[0x40]; }










void func_80282F34(){}

void func_80282FA0(){}

void func_80283118(){}

void func_80283190(){}

void func_80283208(){}

void func_80283280(){}

void func_802832B4(){}

void func_802832D8(){}

void func_80283350(){}

void func_8028345C(){}

extern "C" void func_80283B24(void *r3, const void *r4) {
    *(short*)((char*)r3 + 0) = *(short*)((char*)r4 + 0);
    *(unsigned char*)((char*)r3 + 2) = *(unsigned char*)((char*)r4 + 2);
    *(unsigned char*)((char*)r3 + 3) = *(unsigned char*)((char*)r4 + 3);
    *(unsigned char*)((char*)r3 + 4) = *(unsigned char*)((char*)r4 + 4);
    *(unsigned char*)((char*)r3 + 5) = *(unsigned char*)((char*)r4 + 5);
    *(unsigned char*)((char*)r3 + 6) = *(unsigned char*)((char*)r4 + 6);
    *(unsigned char*)((char*)r3 + 7) = *(unsigned char*)((char*)r4 + 7);
}

void func_80283B60(){}

void func_80283E64(){}

void func_80284144(){}

void func_80284244(){}

void func_80284358(){}

void func_80284490(){}

void func_8028461C(){}

void func_802847A4(){}

void func_802848C4(){}

void func_80284A00(){}

void func_80284B18(){}

void func_80284C30(){}

void func_80284DCC(){}

void func_80284F1C(){}

void func_8028506C(){}

void func_802851BC(){}

void func_8028530C(){}

void func_80285478(){}

void func_802855C8(){}

void func_80285708(){}

void func_802857F0(){}

void func_80285890(){}

void __ct__CEIBCur(){}

void __dt__80285954(){}

void func_80285994(){}

void func_80285A18(){}

void func_80285A90(){}

void func_80285ABC(){}

void func_80285B24(){}

void func_80285B70(){}

void __ct__CEIBPageCur(){}

void __dt__80285C44(){}

void func_80285C84(){}

void __ct__CEquipItemBox(){}

void CEquipItemBox::~CEquipItemBox() {}

void func_802861A8(){}

void func_80286264(){}

void func_80286340(){}

void func_80286454(){}

void func_8028652C(){}


extern "C" u8 func_802865A8(void* self) { return ((u8*)self)[0x43]; }

void func_802865B0(){}

extern "C" int CSysWin_getUnk34(void*);
extern "C" int func_801D3320(void*);

extern "C" int func_80286650(void* arg1) {
    if (CSysWin_getUnk34((char*)arg1 + 0x17c) != 0) {
        return 1;
    }
    return func_801D3320((char*)arg1 + 0x8c);
}

extern "C" u8 func_80286698(void* self) { return ((u8*)self)[0x375]; }

void func_802866A0(){}

void func_802866E8(){}

void func_80286740(){}

void func_802867E0(){}

void func_802869B4(){}

void func_80286B94(){}

void func_80286D7C(){}

void func_80286F6C(){}

void func_80287024(){}

void func_802870DC(){}

void func_80287250(){}

void func_802873D8(){}

void func_80287D58(){}

void func_80287DB4(){}

extern "C" u8 func_80287EE8(void* self) {
    u8 val = *(u8*)((u8*)self + 0x1FE);
    *(u8*)((u8*)self + 0x1FE) = 0;
    return val;
}

extern "C" void func_80287EFC(void* self, u32 val) { *(u32*)((u8*)self + 0x20C) = val; }

void func_80287F04(){}

void func_80287FE0(){}

void func_802882A4(){}

void func_8028847C(){}

void func_80288530(){}

void func_80288544(){}

void func_802886D8(){}

void func_8028876C(){}

void func_80288948(){}

void func_802889C0(){}

void func_80288A1C(){}

void func_80288A6C(){}

void func_80288AC0(){}

void func_80288B14(){}

void func_80288E14(){}

void func_802891B8(){}

void func_8028939C(){}

void func_80289500(){}

void func_80289754(){}

void func_80289AA4(){}

void func_80289CC0(){}

void func_80289E70(){}

extern "C" void func_8028A07C(void* this_) {
    char* self = (char*)this_;
    memset(self + 0x36c, 0, 6);
    self[0x372] = 0;
    self[0x373] = 0;
}

void func_8028A0C0(){}

void func_8028A0E0(){}

void func_8028A160(){}

void func_8028A1DC(){}

void func_8028A374(){}

void func_8028A5D8(){}

void func_8028A9CC(){}

void func_8028AA64(){}

void func_8028AF98(){}

void func_8028B7CC(){}

void func_8028BE74(){}

void func_8028C280(){}

void func_8028CBCC(){}

void func_8028D0EC(){}

void CEquipItemBox::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8028DAB0(){}
