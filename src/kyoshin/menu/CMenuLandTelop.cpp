// Auto-scaffolded catalog TU for kyoshin/menu/CMenuLandTelop
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuLandTelop.hpp"
void __ct__CMenuLandTelop(){}

void CMenuLandTelop::~CMenuLandTelop() {}

void CMenuLandTelop::Init() {}

void CMenuLandTelop::Term() {}

void CMenuLandTelop::Move() {}

void CMenuLandTelop::cbRenderBefore() {}

void func_80144EE4(){}

void func_80144FC8(){}

void func_80144FF0(){}

extern int lbl_eu_806641A0;

void func_80145018() {
    void* p = (void*)lbl_eu_806641A0;
    if (p != 0) {
        *(unsigned char*)((char*)p + 0x64) = 1;
    }
}

unsigned char func_80145030(void) {
    if (lbl_eu_806641A0 != 0) {
        return *((unsigned char*)lbl_eu_806641A0 + 0xe0);
    }
    return 0;
}

void func_8014504C(){}

void func_801453B8(){}

void func_8014548C(){}

void func_80145A90(void* self) { ((void(*)(void*))__dt__14CMenuLandTelopFv)((char*)self - 0x6c); }

void func_80145A98(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuLandTelopFv)((char*)self - 0x70); }

void func_80145AA0(void* self) { ((void(*)(void*))__dt__14CMenuLandTelopFv)((char*)self - 0x70); }

void func_80145AA8(){}
