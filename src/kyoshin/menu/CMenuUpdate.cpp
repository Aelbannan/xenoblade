// Auto-scaffolded catalog TU for kyoshin/menu/CMenuUpdate
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuUpdate.hpp"
void __ct__CMenuUpdate(){}

void __ct__IUIWindow(){}

void __ct__CTTask_IUIWindow(){}



struct CMenuUpdate_8014274C {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_C;

    void __ct__8014274C() {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        field_C = 0;
    }

    void init_8014274C(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
        uint32_t* p = (uint32_t*)this;
        p[0] = v0;
        p[1] = v1;
        p[2] = v2;
        p[3] = v3;
    }
};



void CMenuUpdate::~CMenuUpdate() {}

void CMenuUpdate::Init() {}

u32 getField10(void* self) { return *(u32*)((u8*)self + 0x10); }

void setField67(void* self, u8 val) { static_cast<CMenuUpdateFull*>(self)->field_67 = val; }

void CMenuUpdate::Term() {}

void CMenuUpdate::Move() {}

void CMenuUpdate::cbRenderBefore() {}

void func_80142B4C(){}

void func_80142C64(){}

int getGlobalA10() { return false; }

extern u32 lbl_eu_806640E0;
int getGlobal0E0() { return lbl_eu_806640E0; }

void func_80142C80(){}

void setField64(void* self, u8 val) { static_cast<CMenuUpdateFull*>(self)->field_64 = val; }

void func_80142CA0(){}

void noop_80142D5C(void) {}

void func_80142D60(){}

void setVec2(void* self, float a, float b) {
    *(float*)((char*)self + 0) = a;
    *(float*)((char*)self + 4) = b;
}

void setFieldFloat10(void* self, float val) { *(float*)((u8*)self + 0x10) = val; }

void func_80143ADC(){}

// Converted to inline member function in struct

u8 getField64(void* self) { return static_cast<CMenuUpdateFull*>(self)->field_64; }

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

void dtorThunk_6c(void* self) { ((void(*)(void*))__dt__11CMenuUpdateFv)((char*)self - 0x6c); }

void renderThunk_70(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuUpdateFv)((char*)self - 0x70); }

extern "C" void dtorThunk_70(void* self) { ((void(*)(void*))__dt__11CMenuUpdateFv)((char*)self - 0x70); }
