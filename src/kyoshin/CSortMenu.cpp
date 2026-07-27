// Auto-scaffolded catalog TU for kyoshin/CSortMenu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

u8 func_801D3320(void* self) { return ((u8*)self)[0x28]; }








u8 func_801D3328(void* self) { return ((u8*)self)[0x2B]; }




void func_801D3454(){}

void func_801D350C(void){}

void func_801D3518(void* this_, int value) {
    unsigned char* countPtr = (unsigned char*)this_ + 0xec;
    unsigned char count = *countPtr;
    if (count >= 32) return;
    int* array = (int*)((char*)this_ + 0x6c);
    array[count] = value;
    *countPtr = count + 1;
}

void func_801D353C(){}

void func_801D3620(){}

void func_801D3698(){}

void func_801D3724(){}

void func_801D377C(){}

u8 func_801D37F4(void* self) {
    return *(u8*)((u8*)self + 0xED) + *(u8*)((u8*)self + 0xEE);
}

u8 func_801D3808(void* self) { return ((u8*)self)[0xED]; }

u8 func_801D3810(void* self) { return ((u8*)self)[0xEE]; }

void func_801D3818(){}

void func_801D3878(){}

void func_801D390C(){}

void func_801D3958(){}

void func_801D39EC(){}

void func_801D3A3C(){}

void CSortMenu::OnFileEvent() {}
