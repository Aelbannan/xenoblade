// Auto-scaffolded catalog TU for kyoshin/code_8025FB10
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void func_8025FB10() {}

extern "C" void func_8025FD60() {}

extern "C" void func_8025FDB8() {}

extern "C" void func_80260010() {}

extern "C" void func_80260264() {}

extern "C" void func_80260518() {}

extern "C" void func_80260A6C() {}

extern "C" void func_80260FB0() {}

extern "C" void func_8026178C() {}

extern "C" void func_802617B8() {}

extern "C" void func_80261844() {}

extern "C" void func_8026187C() {}

extern "C" void func_802618AC(void* obj, int value) {
    int* field = (int*)((char*)obj + 0x884);
    *field = value;
    if (value < 0) *field = 0;
    else if (value > 0x3e7) *field = 0x3e7;
}

extern "C" void func_eu_80263A24() {}

extern "C" void func_802618D8() {}

extern "C" int lbl_eu_80664864;
extern "C" int lbl_eu_80664868;
extern "C" int lbl_eu_80662980;
extern "C" int lbl_eu_80664874;

extern "C" void func_80261944(int arg) {
    lbl_eu_80664864 = arg;
    lbl_eu_80664868 = arg;
    lbl_eu_80662980 = -1;
    lbl_eu_80664874 = 0;
}

extern "C" void func_80261960() {}

extern "C" void func_80261A80() {}

extern "C" void __dt__80261B1C() {}

extern "C" void func_80261B98() {}
