// Auto-scaffolded catalog TU for kyoshin/CSortMenu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_801D3320(void* self) { return ((u8*)self)[0x28]; }








extern "C" u8 func_801D3328(void* self) { return ((u8*)self)[0x2B]; }




extern "C" void func_801D3454() {}

extern "C" void func_801D350C(void) {}

extern "C" void func_801D3518(void* this_, int value) {
    unsigned char* countPtr = (unsigned char*)this_ + 0xec;
    unsigned char count = *countPtr;
    if (count >= 32) return;
    int* array = (int*)((char*)this_ + 0x6c);
    array[count] = value;
    *countPtr = count + 1;
}

extern "C" void func_801D353C() {}

extern "C" void func_801D3620() {}

extern "C" void func_801D3698() {}

extern "C" void func_801D3724() {}

extern "C" void func_801D377C() {}

extern "C" u8 func_801D37F4(void* self) {
    return *(u8*)((u8*)self + 0xED) + *(u8*)((u8*)self + 0xEE);
}

extern "C" u8 func_801D3808(void* self) { return ((u8*)self)[0xED]; }

extern "C" u8 func_801D3810(void* self) { return ((u8*)self)[0xEE]; }

extern "C" void func_801D3818() {}

extern "C" void func_801D3878() {}

extern "C" void func_801D390C() {}

extern "C" void func_801D3958() {}

extern "C" void func_801D39EC() {}

extern "C" void func_801D3A3C() {}

extern "C" void OnFileEvent__9CSortMenuFP10CEventFile() {}
