// Auto-scaffolded catalog TU for kyoshin/cf/CPcEffect07
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-801b3194
extern "C" void __ct__cf_CPcEffect07() {}
// LLM-HARNESS-END: us-801b3194

// LLM-HARNESS-BEGIN: us-801b3238
extern "C" void __dt__Q22cf11CPcEffect07Fv(void* self) {}
// LLM-HARNESS-END: us-801b3238

// LLM-HARNESS-BEGIN: us-801b32b4
extern "C" void func_801B19F0() {}
// LLM-HARNESS-END: us-801b32b4

// LLM-HARNESS-BEGIN: us-801b3520
extern "C" void func_801B1C5C() {}
// LLM-HARNESS-END: us-801b3520

// LLM-HARNESS-BEGIN: us-801b3574
extern "C" unsigned char* lbl_eu_80664398;

extern "C" int func_801B1CB0() {
    if (lbl_eu_80664398 == 0) {
        return 0;
    }
    return lbl_eu_80664398[0x58];
}
// LLM-HARNESS-END: us-801b3574

// LLM-HARNESS-BEGIN: us-801b3590
extern "C" void func_801B1CCC() {}
// LLM-HARNESS-END: us-801b3590

// LLM-HARNESS-BEGIN: us-801b3610
extern "C" int func_801B1D4C(int index)
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
// LLM-HARNESS-END: us-801b3610

// LLM-HARNESS-BEGIN: us-801b3690
extern "C" void func_801B1DCC() {}
// LLM-HARNESS-END: us-801b3690

// LLM-HARNESS-BEGIN: us-801b3738
extern "C" void func_801B1E74() {}
// LLM-HARNESS-END: us-801b3738

// LLM-HARNESS-BEGIN: us-801b3868
extern "C" s32 func_801B1FA4() {
    void* r3 = lbl_eu_80664398;
    if (!r3) return -1;
    if (*(s16*)((u32)r3 + 0x14) == 0) return 0;
    if (*(s16*)((u32)r3 + 0x2c) == 0) return 1;
    if (*(s16*)((u32)r3 + 0x44) == 0) return 2;
    return -1;
}
// LLM-HARNESS-END: us-801b3868

// LLM-HARNESS-BEGIN: us-801b38c0
extern "C" int func_801B1FFC(int index) {
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
// LLM-HARNESS-END: us-801b38c0

// LLM-HARNESS-BEGIN: us-801b38f0
extern "C" bool func_801B202C() {
    void* obj = lbl_eu_80664398;
    if (!obj) return false;
    if (*(short*)((int)obj + 0x14) != 0) return true;
    if (*(short*)((int)obj + 0x2c) != 0) return true;
    if (*(short*)((int)obj + 0x44) != 0) return true;
    return false;
}
// LLM-HARNESS-END: us-801b38f0

// LLM-HARNESS-BEGIN: us-801b3948
extern "C" int func_801B2084() {
    int* r4 = (int*)lbl_eu_80664398;
    if (!r4) return 0;
    int result = 0;
    if ((short)(*(short*)((char*)r4 + 0x14)) == 1) result = 1;
    if ((short)(*(short*)((char*)r4 + 0x2c)) == 1) result++;
    if ((short)(*(short*)((char*)r4 + 0x44)) == 1) result++;
    return result;
}
// LLM-HARNESS-END: us-801b3948

// LLM-HARNESS-BEGIN: us-801b398c
extern "C" void func_801B20C8(void* self) {}
// LLM-HARNESS-END: us-801b398c

// LLM-HARNESS-BEGIN: us-801b3a50
extern "C" void func_801B218C() {}
// LLM-HARNESS-END: us-801b3a50

// LLM-HARNESS-BEGIN: us-801b3aa4
extern "C" void func_801B21E0(void* self) {}
// LLM-HARNESS-END: us-801b3aa4

// LLM-HARNESS-BEGIN: us-801b3bdc
extern "C" void func_801B2318(void* self) {}
// LLM-HARNESS-END: us-801b3bdc

// LLM-HARNESS-BEGIN: us-801b3c94
extern "C" void func_801B23D0() {}
// LLM-HARNESS-END: us-801b3c94

// LLM-HARNESS-BEGIN: us-801b3d50
extern "C" void func_801B248C() {}
// LLM-HARNESS-END: us-801b3d50

// LLM-HARNESS-BEGIN: us-801b3e08
extern "C" void func_801B2544(void* self) { ((void(*)(void*))func_801B21E0)((char*)self - 0x4); }
// LLM-HARNESS-END: us-801b3e08

// LLM-HARNESS-BEGIN: us-801b3e10
extern "C" void func_801B254C(void* self) { ((void(*)(void*))func_801B2318)((char*)self - 0x4); }
// LLM-HARNESS-END: us-801b3e10

// LLM-HARNESS-BEGIN: us-801b3e18
extern "C" void func_801B2554(void* self) { ((void(*)(void*))func_801B20C8)((char*)self - 0x4); }
// LLM-HARNESS-END: us-801b3e18

// LLM-HARNESS-BEGIN: us-801b3e20
extern "C" void func_801B255C(void* self) { ((void(*)(void*))__dt__Q22cf11CPcEffect07Fv)((char*)self - 0x4); }
// LLM-HARNESS-END: us-801b3e20
