// Auto-scaffolded catalog TU for kyoshin/CSortMenu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSortMenu.hpp"

u8 func_801D3320(CSortMenu* self) { return self->field_0x28; }








u8 func_801D3328(CSortMenu* self) { return self->field_0x2B; }




void func_801D3454(){}

void func_801D350C(void){}

void func_801D3518(CSortMenu* this_, int value) {
    unsigned char count = this_->field_0xEC;
    if (count >= 32) return;
    this_->mArray[count] = value;
    this_->field_0xEC = count + 1;
}

void func_801D353C(){}

void func_801D3620(){}

void func_801D3698(){}

void func_801D3724(){}

void func_801D377C(){}

u8 func_801D37F4(CSortMenu* self) {
    return self->field_0xED + self->field_0xEE;
}

u8 func_801D3808(CSortMenu* self) { return self->field_0xED; }

u8 func_801D3810(CSortMenu* self) { return self->field_0xEE; }

void func_801D3818(){}

void func_801D3878(){}

void func_801D390C(){}

void func_801D3958(){}

void func_801D39EC(){}

void func_801D3A3C(){}

void CSortMenu::OnFileEvent() {}
