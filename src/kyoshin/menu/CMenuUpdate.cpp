// Auto-scaffolded catalog TU for kyoshin/menu/CMenuUpdate
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__CMenuUpdate(){}

void __ct__IUIWindow(){}

void __ct__CTTask_IUIWindow(){}

extern "C" void __ct__IWorkEvent(void* self) {
    extern char lbl_eu_80525A10[];
    *(void**)self = (void*)lbl_eu_80525A10;
}

struct CMenuUpdate_8014274C {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_C;
};

extern "C" void __ct__8014274C(CMenuUpdate_8014274C* self) {
    self->field_0 = 0;
    self->field_4 = 0;
    self->field_8 = 0;
    self->field_C = 0;
}

extern "C" void __ct__IScnRender(void* self) {
    extern char lbl_eu_80525A00[];
    *(void**)self = (void*)lbl_eu_80525A00;
}

void CMenuUpdate::~CMenuUpdate() {}

void CMenuUpdate::Init() {}

extern "C" u32 func_80142954(void* self) { return *(u32*)((u8*)self + 0x10); }

extern "C" void func_8014295C(void* self, u8 val) { ((u8*)self)[0x67] = val; }

void CMenuUpdate::Term() {}

void CMenuUpdate::Move() {}

void CMenuUpdate::cbRenderBefore() {}

void func_80142B4C(){}

void func_80142C64(){}

extern "C" bool func_80142C70() { return false; }

extern u32 lbl_eu_806640E0;
extern "C" int func_80142C78() { return lbl_eu_806640E0; }

void func_80142C80(){}

extern "C" void func_80142C98(void* self, u8 val) { ((u8*)self)[0x64] = val; }

void func_80142CA0(){}

extern "C" void func_80142D5C(void) {}

void func_80142D60(){}

extern "C" void func_80143AC8(void* self, float a, float b) {
    *(float*)((char*)self + 0) = a;
    *(float*)((char*)self + 4) = b;
}

extern "C" void func_80143AD4(void* self, float val) { *(float*)((u8*)self + 0x10) = val; }

void func_80143ADC(){}

extern "C" void func_80143F38(void* self, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
    uint32_t* p = (uint32_t*)self;
    p[0] = v0;
    p[1] = v1;
    p[2] = v2;
    p[3] = v3;
}

extern "C" u8 func_80143F4C(void* self) { return ((u8*)self)[0x64]; }

void func_80143F54(void* dest, const void* src){
    unsigned int* d = (unsigned int*)dest;
    const unsigned int* s = (const unsigned int*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

void func_80143F78(){}

void func_80144070(){}

void func_801440A8(){}

void func_801443E4(){}

void func_80144410(){}

extern "C" void func_80144590(void* self) { ((void(*)(void*))__dt__11CMenuUpdateFv)((char*)self - 0x6c); }

extern "C" void func_80144598(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuUpdateFv)((char*)self - 0x70); }

extern "C" void func_801445A0(void* self) { ((void(*)(void*))__dt__11CMenuUpdateFv)((char*)self - 0x70); }
