// Auto-scaffolded catalog TU for kyoshin/cf/CPcEffect07
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CPcEffect07.hpp"
void __ct__cf_CPcEffect07(){}

void cf::CPcEffect07::~CPcEffect07() {}

void func_801B19F0(){}

void func_801B1C5C(){}

unsigned char* lbl_eu_80664398;

int func_801B1CB0() {
    if (lbl_eu_80664398 == 0) {
        return 0;
    }
    return lbl_eu_80664398[0x58];
}

void func_801B1CCC(){}

int func_801B1D4C(int index)
{
    if (lbl_eu_80664398 == 0)
        return -1;

    if (index < 0)
    {
        for (int i = 2; i >= 0; --i)
        {
            unsigned char *entry = lbl_eu_80664398 + i * 0x18;
            if (*(short *)(entry + 0x14) == 1)
                return *(unsigned char *)(entry + 0x1c);
        }
        return -1;
    }

    unsigned char *entry = lbl_eu_80664398 + index * 0x18;
    if (*(short *)(entry + 0x14) == 1)
        return *(unsigned char *)(entry + 0x1c);
    return -1;
}

void func_801B1DCC(){}

void func_801B1E74(){}

s32 func_801B1FA4() {
    void* r3 = lbl_eu_80664398;
    if (!r3) return -1;
    if (*(s16*)((u32)r3 + 0x14) == 0) return 0;
    if (*(s16*)((u32)r3 + 0x2c) == 0) return 1;
    if (*(s16*)((u32)r3 + 0x44) == 0) return 2;
    return -1;
}

int func_801B1FFC(int index) {
    if (lbl_eu_80664398 == 0) return 0;
    struct Struct {
        char pad[20];
        short field14;
        char pad2[2];
    };
    Struct* arr = (Struct*)lbl_eu_80664398;
    short val = arr[index].field14;
    return (val == 1) ? 1 : 0;
}

bool func_801B202C() {
    void* obj = lbl_eu_80664398;
    if (!obj) return false;
    if (*(short*)((int)obj + 0x14) != 0) return true;
    if (*(short*)((int)obj + 0x2c) != 0) return true;
    if (*(short*)((int)obj + 0x44) != 0) return true;
    return false;
}

int func_801B2084() {
    int* r4 = (int*)lbl_eu_80664398;
    if (!r4) return 0;
    int result = 0;
    if ((short)(*(short*)((char*)r4 + 0x14)) == 1) result = 1;
    if ((short)(*(short*)((char*)r4 + 0x2c)) == 1) result++;
    if ((short)(*(short*)((char*)r4 + 0x44)) == 1) result++;
    return result;
}

void func_801B20C8(void* self){}

void func_801B218C(){}

void func_801B21E0(void* self){}

void func_801B2318(void* self){}

void func_801B23D0(){}

void func_801B248C(){}

void func_801B2544(void* self) { ((void(*)(void*))func_801B21E0)((char*)self - 0x4); }

void func_801B254C(void* self) { ((void(*)(void*))func_801B2318)((char*)self - 0x4); }

extern "C" void func_801B2554(void* self) { ((void(*)(void*))func_801B20C8)((char*)self - 0x4); }

extern "C" void func_801B255C(void* self) { ((void(*)(void*))__dt__Q22cf11CPcEffect07Fv)((char*)self - 0x4); }
