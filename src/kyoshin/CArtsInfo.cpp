// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_80235A90(void* self) { return ((u8*)self)[0x48]; }







extern "C" u8 func_80235A98(void* self) { return ((u8*)self)[0x49]; }





extern "C" void func_80235DD8() {}

extern "C" void func_80235E84(void* self, u8 val) { ((u8*)self)[0x54] = val; }

extern "C" void func_80235E8C(void* self, u8 val) { ((u8*)self)[0x55] = val; }

extern "C" void func_80235E94(void* self, u8 val) { ((u8*)self)[0x56] = val; }

extern "C" void func_80235E9C(void* self, u16 val) { *(u16*)((u8*)self + 0x58) = val; }

extern "C" void func_80235EA4() {}

extern "C" void func_80235EF0() {}

extern "C" void func_80235F14() {}

extern "C" u32 func_80235F3C(void* self) {
    s8 val = *(s8*)((u8*)self + 0x5A);
    u32 result = __cntlzw(val);
    return result >> 5;
}

extern "C" int func_80235F50(void* thisPtr) {
    int field = *(int*)((char*)thisPtr + 0x44);
    return field >= 6 ? 1 : 0;
}

extern "C" void func_80235F6C() {}

extern "C" void func_80236020() {}

extern "C" void func_8023606C() {}

extern "C" void func_80236120() {}

extern "C" void func_8023616C() {}

extern "C" void func_80236220() {}

extern "C" void func_802362D4() {}

extern "C" void func_80236334() {}

extern "C" void func_80236408() {}

extern "C" void func_80236454() {}

extern "C" void func_80236508() {}

extern "C" void func_802369C0() {}

extern "C" void func_80236CF4() {}

extern "C" void func_80236DB8() {}

extern "C" void func_80236DF0() {}

extern "C" void func_80236E28() {}

extern "C" void func_80236E6C() {}

extern "C" void func_80237050() {}

extern "C" void func_802370A8() {}

extern "C" void func_80237100() {}

extern "C" void func_8023719C() {}

extern "C" void func_80237238() {}

extern "C" void func_80237394() {}

extern "C" void func_802374F0() {}

extern "C" void func_8023754C() {}

extern "C" void func_802375A8() {}

extern "C" void func_80237A0C() {}

extern "C" void func_80237B88() {}

extern "C" void func_80237D58() {}

extern "C" void func_80237E24() {}

extern "C" void func_80238038() {}

extern "C" void func_80238298() {}

extern "C" void func_802384F4() {}

extern "C" void func_80238904() {}

extern "C" void func_80239030() {}

extern "C" void func_8023916C() {}

extern "C" void func_8023939C() {}

extern "C" void func_8023959C() {}

extern "C" void func_802397F4() {}

extern "C" void func_80239964() {}

extern "C" void func_80239AA0() {}

extern "C" void func_80239BDC() {}

extern "C" void func_80239D20() {}

extern "C" void func_80239EFC() {}

extern "C" void func_80239FC4() {}

extern "C" void func_8023A148() {}

extern "C" void func_8023A210() {}

extern "C" void func_8023A2D8() {}

extern "C" void func_8023A398() {}

extern "C" void func_8023A460() {}

extern "C" void func_8023A55C() {}

extern "C" void func_8023A60C() {}

extern "C" void func_8023A6BC() {}

extern "C" void func_8023A76C() {}

extern "C" void func_8023A81C() {}

extern "C" void func_8023A8CC() {}

extern "C" void func_8023A97C() {}

extern "C" void func_8023AA2C() {}

extern "C" void func_8023AADC() {}

extern "C" void func_8023AB8C() {}

extern "C" void func_8023AD5C() {}

extern "C" void func_8023AE24() {}

extern "C" void func_8023AF60() {}

extern "C" void func_8023B074() {}

extern "C" void func_8023B12C() {}

extern "C" void func_8023B280() {}

extern "C" void func_8023B368() {}

extern "C" void func_eu_8023D490() {}

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

extern "C" void OnFileEvent__9CArtsInfoFP10CEventFile() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8023BC8C() {}
