// Auto-scaffolded catalog TU for kyoshin/cf/IResInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void func_80065F24(){}

void func_eu_80066938(){}

extern "C" void func_eu_8006693C() {}

void func_eu_80066940(){}

void func_80065FB4(){}

void func_8006611C(){}

void func_80066160(){}

void func_80066184(){}

void func_800661A8(){}

void func_8006626C(){}

void func_80066290(){}

extern "C" unsigned int func_80066374(int unused, void* p)
{
    if (p != 0) {
        return *(unsigned int*)((char*)p + 8);
    }
    return 0;
}

void func_8006638C(){}

void func_800663D8(){}

void func_80066424(){}

void func_8006646C(){}

void func_800664B8(){}

void func_8006650C(){}

void func_80066554(){}

void func_800665A0(){}

void func_800665F4(){}

void func_8006660C(){}

void func_80066714(){}

void func_80066788(){}

extern "C" unsigned int lbl_eu_80663E28;

extern "C" bool func_80066C5C(unsigned int flags) {
    return (lbl_eu_80663E28 & flags) != 0;
}

void func_80066C74(){}

void func_80066CF8(){}

void func_80066DAC(){}

void func_80066E7C(){}

void __ct__80066F9C(){}

void __dt___reslist_base_unsigned_short(){}

void __dt__reslist_unsigned_short(){}

void __dt__8006754C(){}

void __dt__80067670(){}

void func_800676F8(){}

void func_80067D38(){}

void func_80067DB4(){}

void func_80067E78(){}

void func_80067F10(){}

void func_80067FE0(){}

void func_80068078(){}

void func_80068110(){}

void func_80068254(){}

void func_80068358(){}

extern "C" bool func_8006842C(const unsigned long* p, unsigned long mask) {
    return (*p & mask) != 0;
}

extern "C" void func_80068444(void* self, unsigned long mask) {
    *(unsigned long*)self &= ~mask;
}

extern "C" char* func_80068454(char* base, int a, int b) {
    return base + (a + b + 0x59) * 0x3C + 4;
}

void func_8006846C(){}

extern "C" char* func_80068478(char* self, int a, int b) {
    return self + (b + a * 11 + 12) * 60 + 4;
}

void func_80068494(){}

void func_80068564(){}

void func_800685BC(){}

void func_800685C8(){}

void func_8006861C(){}

extern "C" void* func_80068680(void* self, unsigned int id, unsigned int* outIndex, unsigned int* outValue) {
    unsigned char* entry = static_cast<unsigned char*>(self) + 0x14DC;
    unsigned int index = 0x59;
    *outIndex = 0;
    *outValue = static_cast<unsigned int>(-1);
    if (id == 0)
        return 0;
    for (unsigned int i = 0; i < 0x28; ++i, ++index, entry += 0x3C) {
        if (*reinterpret_cast<unsigned int*>(entry + 8) == id) {
            *outIndex = index;
            *outValue = *(entry + 0x36);
            return entry + 4;
        }
    }
    return 0;
}

void func_800686E4(){}

void func_80068928(){}

extern "C" bool func_80068990() { return false; }

bool func_80068998(){ return false; }

extern "C" void func_800689A0() {}

extern "C" bool func_800689A4() { return false; }

void func_800689AC(){}

bool func_800689B8(){ return false; }

extern "C" bool func_800689C0() { return false; }

bool func_800689C8(){ return false; }

extern "C" bool func_800689D0() { return false; }

extern "C" bool func_800689D8() { return false; }

bool func_eu_800693E8(){ return false; }

bool func_800689E0(){ return false; }

bool func_800689E8(){ return false; }

bool func_800689F0(){ return false; }

bool func_800689F8(){ return false; }

bool func_80068A00(){ return false; }

bool func_80068A08(){ return false; }

bool func_80068A10(){ return false; }

extern "C" int func_80068A18(void* self) { return 512; }
