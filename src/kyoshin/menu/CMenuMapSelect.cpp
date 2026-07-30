// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelect
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelect.hpp"

// forward declarations for scaffold thunk references
extern const char lbl_eu_80664790[];


void __ct__CMenuMapSelect(){}

unsigned long func_80242354(){
    unsigned long v = *(unsigned long*)(lbl_eu_80664790);
    return !!v;
}

void func_80242368(){}

void func_80242524(){}

void func_80242A28(){}

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMapSelect.
void CMenuMapSelect::func_8024309C() {
    cbRenderBefore();
}

// IScnRender vtable this-adjusting thunk for destructor.
void CMenuMapSelect::func_802430A4() {
    this->~CMenuMapSelect();
}
