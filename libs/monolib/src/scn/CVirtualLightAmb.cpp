// Decompiled for monolib/src/scn/CVirtualLightAmb

#include <types.h>
#include "libs/monolib/src/scn/CVirtualLightAmb.hpp"

extern "C" u32 lbl_eu_8056E878[];  // CVirtualLightAmb vtable

CVirtualLightAmb::CVirtualLightAmb() {
    mVtable = (u8*)lbl_eu_8056E878;
    mField2C = 1;
}

CVirtualLightAmb::~CVirtualLightAmb() {
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056E878-0x8056E898 (32B): CVirtualLightAmb vtable (typeinfo pair
// lbl_eu_806639C8 and base label lbl_eu_806639D0 live in a foreign TU).
extern "C" u32 lbl_eu_806639C8;
extern "C" u32 lbl_eu_806639D0;
extern "C" void __dt__16CVirtualLightAmbFv();
extern "C" void getVirtualLightColor();
extern "C" void getVirtualLightSentinel();
extern "C" u32 lbl_eu_8056E878[5] = {
    (u32)&lbl_eu_806639C8, 0x00000000,
    (u32)&__dt__16CVirtualLightAmbFv,
    (u32)&getVirtualLightColor,
    (u32)&getVirtualLightSentinel,
};
// [.data] 0x8056E88C-0x8056E898 (12B): vtable tail (base label + 2 zero
// slots); retail holds this tail under its own label (referenced by
// CScnItemCamera). Split point 0x14: u32+reloc tails pack at 4 (CScnMem).
extern "C" u32 lbl_eu_8056E88C[3] = {
    (u32)&lbl_eu_806639D0,
    0x00000000, 0x00000000,
};

// [.rodata] 0x80524050-0x80524078 (40B): RTTI name strings.
extern "C" __declspec(align(8)) const char lbl_eu_80524050[0x11] = { 0x43,0x56,0x69,0x72,0x74,0x75,0x61,0x6C,0x4C,0x69,0x67,0x68,0x74,0x41,0x6D,0x62,0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80524064[0x14] = { 0x43,0x56,0x69,0x72,0x74,0x75,0x61,0x6C,0x4C,0x69,0x67,0x68,0x74,0x4F,0x62,0x6A,0x00,0x00,0x00,0x00 };

