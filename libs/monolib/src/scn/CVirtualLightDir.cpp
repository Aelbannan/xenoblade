// Decompiled for monolib/src/scn/CVirtualLightDir

#include <types.h>
#include "monolib/scn/CVirtualLightDir.hpp"

// Retail CVirtualLightDir vtable data (stored manually; see
// CVirtualLightObj.hpp) and the shared sdata2 {0.0f, 0.0f} constant.
extern "C" u32 lbl_eu_8056E898[];
extern "C" const float lbl_eu_8066AA80; // 0.0f (first float of the 8-byte pair)

CVirtualLightDir::CVirtualLightDir() {
    mVtable = (u8*)lbl_eu_8056E898;
    mField34 = lbl_eu_8066AA80;
    mField38 = lbl_eu_8066AA80;
    mField2C = 2;
}

// Stores the two f32 fields at 0x34/0x38 (retail symbol is unmangled C).
extern "C" void VirtLight_SetPair(CVirtualLightDir* self, float a, float b) {
    self->mField34 = a;
    self->mField38 = b;
}

CVirtualLightDir::~CVirtualLightDir() {}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace VLBlob {
extern "C" void __dt__16CVirtualLightDirFv();
extern "C" void getVirtualLightColor();
extern "C" void getVirtualLightSentinel();
}
extern "C" u32 lbl_eu_806639D8;   // .sdata (foreign)
extern "C" u32 lbl_eu_806639D0;   // .sdata (foreign)

// [.data] 0x8056E898-0x8056E8B8 (32B): CVirtualLightDir vtable
extern "C" u32 lbl_eu_8056E898[8] = {
    (u32)&lbl_eu_806639D8, 0x00000000,
    (u32)&VLBlob::__dt__16CVirtualLightDirFv,
    (u32)&VLBlob::getVirtualLightColor,
    (u32)&VLBlob::getVirtualLightSentinel,
    (u32)&lbl_eu_806639D0, 0x00000000, 0x00000000,
};

// [.rodata] 0x80524078-0x805240A0 (40B)
extern "C" __declspec(align(4)) const char lbl_eu_80524078[0x28] = {
    0x43,0x56,0x69,0x72,0x74,0x75,0x61,0x6C,0x4C,0x69,0x67,0x68,0x74,0x44,0x69,0x72,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x25,0x73,0x28,0x25,0x73,0x29,0x00,0x43,0x53,0x63,0x6E,0x00,0x00,0x00,0x00,
};
