// Auto-scaffolded catalog TU for kyoshin/CEquipChange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_802023C0(void* self) { return ((u8*)self)[0x4D]; }








extern "C" void func_802865A0(void* self);
extern "C" void func_802023C8(void* self) { ((void(*)(void*))func_802865A0)((char*)self + 0x2b0); }



extern "C" void func_80286650(void* self);
extern "C" void func_8020247C(void* self) { ((void(*)(void*))func_80286650)((char*)self + 0x2b0); }


extern "C" void func_802024CC() {}

extern "C" void func_80202578() {}

extern "C" void func_80202644() {}

extern "C" void func_80202790() {}

extern "C" void func_802028E4() {}

extern "C" void func_80202A70() {}

extern "C" void func_80202BFC() {}

extern "C" void func_80202C4C() {}

extern "C" void func_80202CCC() {}

extern "C" void func_80202EB4() {}

extern "C" int func_80203138(void* self) {
    signed char value = *reinterpret_cast<const signed char*>(reinterpret_cast<const unsigned char*>(self) + 0x98);
    if (value == 0)
        return 2;
    if (value == 4)
        return 4;
    if (value == 6)
        return 5;
    if (value == 8)
        return 6;
    if (value == 10)
        return 7;
    if (value == 12)
        return 8;
    return 3;
}

extern "C" void func_802031A0() {}

extern "C" void func_80203210() {}

extern "C" void func_8020392C() {}

extern "C" void func_801D2E4C(void* self);
extern "C" void func_8020397C(void* self) { ((void(*)(void*))func_801D2E4C)((char*)self + 0x80); }

extern "C" void func_80287FE0(void* self);
extern "C" void func_80203984(void* self) { ((void(*)(void*))func_80287FE0)((char*)self + 0x2b0); }

extern "C" void func_802886D8(void* self);
extern "C" void func_8020398C(void* self) { ((void(*)(void*))func_802886D8)((char*)self + 0x2b0); }

extern "C" void func_80203994() {}

extern "C" void func_802039F4() {}

extern "C" void func_80203A98() {}

extern "C" void func_80203C9C() {}

extern "C" void func_80203CE0() {}

extern "C" void func_80203D78() {}

extern "C" void func_80203E00() {}

extern "C" void func_80203E98() {}

extern "C" void func_80203EE4() {}

extern "C" void func_80203F84() {}

extern "C" void func_80203FCC() {}

extern "C" void func_8020404C() {}

extern "C" void func_802040A0() {}

extern "C" void func_802040FC() {}

extern "C" void func_802042C0() {}

extern "C" void func_80205294(void* dst, void* src) {
    u16 a = *(u16*)src;
    u32 b = *(u32*)((u8*)src + 4);
    *(u16*)dst = a;
    *(u32*)((u8*)dst + 4) = b;
}

extern "C" void func_802052A8() {}

extern "C" void OnFileEvent__12CEquipChangeFP10CEventFile() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_802059E8() {}
