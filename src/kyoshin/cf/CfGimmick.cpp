// Auto-scaffolded catalog TU for kyoshin/cf/CfGimmick
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CfGimmick() {}

extern "C" void __dt__Q22cf9CfGimmickFv() {}

extern "C" void func_8020896C(void* self, void* other) {
    void* cur = *(void**)((char*)self + 0x78);
    if (cur != other) return;
    *(void**)((char*)cur + 0xB0) = 0;
    *(void**)((char*)self + 0x78) = 0;
}

extern "C" void func_80208988(void* self) {
    extern void func_802089BC(void*, void*, void*);
    func_802089BC((char*)self + 0x1c, (char*)self + 4, (char*)self + 0x10);
}

extern "C" void func_8020899C() {}

extern "C" void func_802089BC() {}

extern "C" void func_80208C48() {}

extern "C" void func_80208C60() {}

extern "C" void func_80208C78() {}

extern "C" void func_80208CC0() {}

extern "C" void func_80208E98() {}

extern "C" bool func_80208EDC() { return false; }

extern "C" void func_80208EE4() {}

extern "C" void func_80208F34() {}

extern "C" void func_80209020() {}

extern "C" void func_8020915C() {}

extern "C" void func_80209288() {}

extern "C" void func_8020938C() {}

extern "C" void func_80209488() {}

extern "C" void func_802095D8() {}

extern "C" void func_802096EC() {}

extern "C" void func_8020971C() {}

extern "C" void func_8009D018(void* self);
extern "C" void func_8020974C(void* self) { ((void(*)(void*))func_8009D018)((char*)self + 0x2cc8); }

extern "C" void func_80209754() {}

extern "C" void func_802098EC() {}

extern "C" void func_80209F2C() {}

extern "C" void func_80209F5C() {
    extern void* getUnk80664658();
    void* p = getUnk80664658();
    *(volatile unsigned int*)((unsigned char*)p + 0x214) |= 0x000C0002;
}

extern "C" void* getUnk80664658();
extern "C" void func_80209F8C() {
    void* p = getUnk80664658();
    *(unsigned int*)((char*)p + 0x214) |= 0x8008;
}

extern "C" void func_80209FB8() {}

extern "C" void func_80209FE4() {}

extern "C" void func_8020A010() {}

extern "C" void func_8020A03C() {}

extern "C" void func_8020A068() {}

extern "C" void func_8020A0CC() {}

extern "C" void func_8020A0F8() {}

extern "C" void func_8020A124() {}

extern "C" void func_8020A1DC() {}

extern "C" void func_8020A294() {}

extern "C" void func_8020A35C() {}

extern "C" void func_8020A434() {}

extern "C" void func_8020A484() {}

extern "C" void func_8020A5DC() {}

extern "C" void func_8020A608() {}

extern "C" void func_8020A6B0() {}

extern "C" void func_8020A87C() {}

extern "C" int func_8020A8AC(void* self) { return 1; }

extern "C" void func_8020A8B4() {}

extern "C" void func_8020A928() {}

extern "C" void func_8020A9F4() {}

extern "C" void func_8020AA8C() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8020AB7C() {}
