// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_80235A90(void* self) { return ((u8*)self)[0x48]; }







extern "C" u8 func_80235A98(void* self) { return ((u8*)self)[0x49]; }





void func_80235DD8(){}

extern "C" void func_80235E84(void* self, u8 val) { ((u8*)self)[0x54] = val; }

extern "C" void func_80235E8C(void* self, u8 val) { ((u8*)self)[0x55] = val; }

extern "C" void func_80235E94(void* self, u8 val) { ((u8*)self)[0x56] = val; }

extern "C" void func_80235E9C(void* self, u16 val) { *(u16*)((u8*)self + 0x58) = val; }

void func_80235EA4(){}

void func_80235EF0(){}

void func_80235F14(){}

extern "C" u32 func_80235F3C(void* self) {
    s8 val = *(s8*)((u8*)self + 0x5A);
    u32 result = __cntlzw(val);
    return result >> 5;
}

extern "C" int func_80235F50(void* thisPtr) {
    int field = *(int*)((char*)thisPtr + 0x44);
    return field >= 6 ? 1 : 0;
}

void func_80235F6C(){}

void func_80236020(){}

void func_8023606C(){}

void func_80236120(){}

void func_8023616C(){}

void func_80236220(){}

void func_802362D4(){}

void func_80236334(){}

void func_80236408(){}

void func_80236454(){}

void func_80236508(){}

void func_802369C0(){}

void func_80236CF4(){}

void func_80236DB8(){}

void func_80236DF0(){}

void func_80236E28(){}

void func_80236E6C(){}

void func_80237050(){}

void func_802370A8(){}

void func_80237100(){}

void func_8023719C(){}

void func_80237238(){}

void func_80237394(){}

void func_802374F0(){}

void func_8023754C(){}

void func_802375A8(){}

void func_80237A0C(){}

void func_80237B88(){}

void func_80237D58(){}

void func_80237E24(){}

void func_80238038(){}

void func_80238298(){}

void func_802384F4(){}

void func_80238904(){}

void func_80239030(){}

void func_8023916C(){}

void func_8023939C(){}

void func_8023959C(){}

void func_802397F4(){}

void func_80239964(){}

void func_80239AA0(){}

void func_80239BDC(){}

void func_80239D20(){}

void func_80239EFC(){}

void func_80239FC4(){}

void func_8023A148(){}

void func_8023A210(){}

void func_8023A2D8(){}

void func_8023A398(){}

void func_8023A460(){}

void func_8023A55C(){}

void func_8023A60C(){}

void func_8023A6BC(){}

void func_8023A76C(){}

void func_8023A81C(){}

void func_8023A8CC(){}

void func_8023A97C(){}

void func_8023AA2C(){}

void func_8023AADC(){}

void func_8023AB8C(){}

void func_8023AD5C(){}

void func_8023AE24(){}

void func_8023AF60(){}

void func_8023B074(){}

void func_8023B12C(){}

void func_8023B280(){}

void func_8023B368(){}

void func_eu_8023D490(){}

extern "C" void func_8023B430(void* r3) {
    int* field20 = (int*)((char*)r3 + 0x20);
    if (*field20 == 0) return;
    int* field4c = (int*)((char*)r3 + 0x4c);
    if (*field4c == 0) return;
    int* field50 = (int*)((char*)r3 + 0x50);
    if (*field50 != 0) {
        char* field48 = (char*)r3 + 0x48;
        char* field40 = (char*)r3 + 0x40;
        *field48 = 1;
        *field40 = 1;
    }
}

void CArtsInfo::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8023BC8C(){}
