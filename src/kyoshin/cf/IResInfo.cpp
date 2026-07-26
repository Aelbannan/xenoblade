// Auto-scaffolded catalog TU for kyoshin/cf/IResInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void func_80065F24() {}

extern "C" void func_eu_80066938() {}

extern "C" void func_eu_8006693C() {}

extern "C" void func_eu_80066940() {}

extern "C" void func_80065FB4() {}

extern "C" void func_8006611C() {}

extern "C" void func_80066160() {}

extern "C" void func_80066184() {}

extern "C" void func_800661A8() {}

extern "C" void func_8006626C() {}

extern "C" void func_80066290() {}

extern "C" unsigned int func_80066374(int unused, void* p)
{
    if (p != 0) {
        return *(unsigned int*)((char*)p + 8);
    }
    return 0;
}

extern "C" void func_8006638C() {}

extern "C" void func_800663D8() {}

extern "C" void func_80066424() {}

extern "C" void func_8006646C() {}

extern "C" void func_800664B8() {}

extern "C" void func_8006650C() {}

extern "C" void func_80066554() {}

extern "C" void func_800665A0() {}

extern "C" void func_800665F4() {}

extern "C" void func_8006660C() {}

extern "C" void func_80066714() {}

extern "C" void func_80066788() {}

extern "C" unsigned int lbl_eu_80663E28;

extern "C" bool func_80066C5C(unsigned int flags) {
    return (lbl_eu_80663E28 & flags) != 0;
}

extern "C" void func_80066C74() {}

extern "C" void func_80066CF8() {}

extern "C" void func_80066DAC() {}

extern "C" void func_80066E7C() {}

extern "C" void __ct__80066F9C() {}

extern "C" void __dt___reslist_base_unsigned_short() {}

extern "C" void __dt__reslist_unsigned_short() {}

extern "C" void __dt__8006754C() {}

extern "C" void __dt__80067670() {}

extern "C" void func_800676F8() {}

extern "C" void func_80067D38() {}

extern "C" void func_80067DB4() {}

extern "C" void func_80067E78() {}

extern "C" void func_80067F10() {}

extern "C" void func_80067FE0() {}

extern "C" void func_80068078() {}

extern "C" void func_80068110() {}

extern "C" void func_80068254() {}

extern "C" void func_80068358() {}

extern "C" bool func_8006842C(const unsigned long* p, unsigned long mask) {
    return (*p & mask) != 0;
}

extern "C" void func_80068444(void* self, unsigned long mask) {
    *(unsigned long*)self &= ~mask;
}

extern "C" char* func_80068454(char* base, int a, int b) {
    return base + (a + b + 0x59) * 0x3C + 4;
}

extern "C" void func_8006846C() {}

extern "C" char* func_80068478(char* self, int a, int b) {
    return self + (b + a * 11 + 12) * 60 + 4;
}

extern "C" void func_80068494() {}

extern "C" void func_80068564() {}

extern "C" void func_800685BC() {}

extern "C" void func_800685C8() {}

extern "C" void func_8006861C() {}

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

extern "C" void func_800686E4() {}

extern "C" void func_80068928() {}

extern "C" bool func_80068990() { return false; }

extern "C" bool func_80068998() { return false; }

extern "C" void func_800689A0() {}

extern "C" bool func_800689A4() { return false; }

extern "C" void func_800689AC() {}

extern "C" bool func_800689B8() { return false; }

extern "C" bool func_800689C0() { return false; }

extern "C" bool func_800689C8() { return false; }

extern "C" bool func_800689D0() { return false; }

extern "C" bool func_800689D8() { return false; }

extern "C" bool func_eu_800693E8() { return false; }

extern "C" bool func_800689E0() { return false; }

extern "C" bool func_800689E8() { return false; }

extern "C" bool func_800689F0() { return false; }

extern "C" bool func_800689F8() { return false; }

extern "C" bool func_80068A00() { return false; }

extern "C" bool func_80068A08() { return false; }

extern "C" bool func_80068A10() { return false; }

extern "C" int func_80068A18(void* self) { return 512; }
