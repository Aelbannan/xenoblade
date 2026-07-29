// Auto-scaffolded catalog TU for kyoshin/CSortMenu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSortMenu.hpp"

extern "C" u8 func_801D3320(void* self) { return ((CSortMenu*)self)->field_0x28; }








extern "C" u8 func_801D3328(void* self) { return ((CSortMenu*)self)->field_0x2B; }




extern "C" void func_801D3454(){}

void func_801D350C(void){}

void CSortMenu::func_801D3518(int value) {
    unsigned char count = field_0xEC;
    if (count >= 32) return;
    mArray[count] = value;
    field_0xEC = count + 1;
}

void func_801D353C(){}

extern "C" void func_801D3620(){}

extern "C" void func_801D3698(){}

extern "C" void func_801D3724(){}

extern "C" void func_801D377C(){}

u8 CSortMenu::func_801D37F4() {
    return field_0xED + field_0xEE;
}

u8 CSortMenu::func_801D3808() { return field_0xED; }

u8 CSortMenu::func_801D3810() { return field_0xEE; }

void func_801D3818(){}

void func_801D3878(){}

void func_801D390C(){}

void func_801D3958(){}

void func_801D39EC(){}

void func_801D3A3C(){}

void CSortMenu::OnFileEvent() {}
