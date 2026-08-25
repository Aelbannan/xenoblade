// Decompiled for monolib/src/scn/CVirtualLightAmb

#include <types.h>
#include "libs/monolib/src/scn/CVirtualLightAmb.hpp"

extern "C" {
    extern void __dl__FPv(void* ptr);
    extern void __ct__CVirtualLightObj(void* self);
    extern u32 lbl_eu_8056E878[];  // CVirtualLightAmb vtable
}

extern "C" CVirtualLightAmb* __ct__CVirtualLightAmb(CVirtualLightAmb* self) {
    __ct__CVirtualLightObj(self);  // Initialize base portion
    *(void**)self = (void*)lbl_eu_8056E878;  // Set vtable pointer
    self->mField2C = 1;
    return self;
}

CVirtualLightAmb::~CVirtualLightAmb() {
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056E878-0x8056E898 (32B): CVirtualLightAmb vtable (typeinfo pair
// lbl_eu_806639C8 and base label lbl_eu_806639D0 live in a foreign TU).
extern "C" u32 lbl_eu_806639C8;
extern "C" u32 lbl_eu_806639D0;
extern "C" void __dt__16CVirtualLightAmbFv();
extern "C" void func_804947EC();
extern "C" void func_8049488C();
extern "C" u32 lbl_eu_8056E878[8] = {
    (u32)&lbl_eu_806639C8, 0x00000000,
    (u32)&__dt__16CVirtualLightAmbFv,
    (u32)&func_804947EC,
    (u32)&func_8049488C,
    (u32)&lbl_eu_806639D0,
    0x00000000, 0x00000000,
};

// [.rodata] 0x80524050-0x80524078 (40B): RTTI name strings.
extern "C" __declspec(align(8)) const char lbl_eu_80524050[0x11] = { 0x43,0x56,0x69,0x72,0x74,0x75,0x61,0x6C,0x4C,0x69,0x67,0x68,0x74,0x41,0x6D,0x62,0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80524064[0x14] = { 0x43,0x56,0x69,0x72,0x74,0x75,0x61,0x6C,0x4C,0x69,0x67,0x68,0x74,0x4F,0x62,0x6A,0x00,0x00,0x00,0x00 };

