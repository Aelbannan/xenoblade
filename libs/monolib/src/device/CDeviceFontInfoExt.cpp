// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFontInfoExt
#include <harness_catalog.h>
#include "monolib/device/CDeviceFontInfoExt.hpp"

extern "C" {
// .sbss2:0x8066B558 (4B) | lbl_8066DCF8 - font-name blob; its address is passed
// as the pName/tag to nw4r::ut::PackedFont (retail li r4, lbl_8066DCF8@sda21).
// Zero-filled const+initializer aggregate lands in .sbss2 (NOBITS) per the
// MWCC small-data placement rules (docs/MWCC_REFERENCE.md "Data-only TU").
extern const char lbl_8066DCF8[4] = {0, 0, 0, 0};
}

extern "C" void* func_80453624__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return (void*)((u8*)self + 0x1c); }
extern "C" void* func_8045362C__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return (void*)((u8*)self + 0x1c); }
extern "C" u16 func_80453634__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->field_5C; }
extern "C" u32 func_8045363C__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->field_58; }
extern "C" u16 func_80453644__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->mMode; }
extern "C" u16 func_8045364C__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->mState; }
extern "C" u32 func_80453654__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->field_04; }

extern "C" void func_804535C0__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) {
    u32 v = self->field_60;
    if (v - 1 <= 1) self->field_60 = v + 1;
}
extern "C" void func_804535DC__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) {
    u32 v = self->field_60;
    if (v == 0) self->field_60 = v + 1;
}
extern "C" u32 func_804535F4__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) {
    u32 v = self->field_60;
    return ((u32)(-(s32)v | (s32)v)) >> 31;
}
extern "C" u32 func_80453608__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) {
    s32 v = (s32)self->field_60;
    return v >= 3;
}


extern "C" void func_80453468__18CDeviceFontInfoExtFv() {}
extern "C" void create__18CDeviceFontInfoExtFv() {}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// forward refs
namespace D2Blob {
extern "C" void __dt__18CDeviceFontInfoExtFv();
}
extern "C" u32 lbl_eu_80663718[0x2];
extern "C" u32 lbl_eu_80663720[0x2];
extern "C" const char lbl_eu_80522E00[0x18];
extern "C" const char lbl_eu_80522E18[0x10];
extern "C" u32 lbl_eu_8056C740[0xF];
extern "C" u32 lbl_eu_8056C77C[0x3];

// [.data] 0x8056C740-0x8056C788 (72 bytes)
extern "C" u32 lbl_eu_8056C740[0xF] = {
    (u32)&lbl_eu_80663718[0], 0x00000000,
    (u32)&D2Blob::__dt__18CDeviceFontInfoExtFv,
    (u32)func_80453654__18CDeviceFontInfoExtFv,
    (u32)func_8045364C__18CDeviceFontInfoExtFv,
    (u32)func_80453644__18CDeviceFontInfoExtFv,
    (u32)func_8045363C__18CDeviceFontInfoExtFv,
    (u32)func_80453634__18CDeviceFontInfoExtFv,
    (u32)func_80453468__18CDeviceFontInfoExtFv,
    (u32)func_80453624__18CDeviceFontInfoExtFv,
    (u32)func_8045362C__18CDeviceFontInfoExtFv,
    (u32)func_804535C0__18CDeviceFontInfoExtFv,
    (u32)func_804535DC__18CDeviceFontInfoExtFv,
    (u32)func_804535F4__18CDeviceFontInfoExtFv,
    (u32)func_80453608__18CDeviceFontInfoExtFv,
};
extern "C" u32 lbl_eu_8056C77C[0x3] = { (u32)&lbl_eu_80663720[0], 0x00000000, 0x00000000 };

// [.rodata] 0x80522E00-0x80522E28 (40 bytes)
extern "C" const char lbl_eu_80522E00[0x18] = {
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6f,0x6e,0x74,0x49,0x6e,0x66,0x6f,0x45,
    0x78,0x74,0x00,0x00,0x00,0x00,0x00,0x00 };
extern "C" const char lbl_eu_80522E18[0x10] = {
    0x49,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6f,0x6e,0x74,0x49,0x6e,0x66,0x6f,0x00 };

// [.sdata] 0x80663718-0x80663728 (16 bytes)
extern "C" u32 lbl_eu_80663718[0x2] = { (u32)&lbl_eu_80522E00[0], (u32)&lbl_eu_8056C77C[0] };
extern "C" u32 lbl_eu_80663720[0x2] = { (u32)&lbl_eu_80522E18[0], 0x00000000 };

DECOMP_FORCEACTIVE(CDeviceFontInfoExt_cpp, lbl_eu_8056C740);
DECOMP_FORCEACTIVE(CDeviceFontInfoExt_cpp, lbl_eu_8056C77C);
DECOMP_FORCEACTIVE(CDeviceFontInfoExt_cpp, lbl_eu_80522E00);
DECOMP_FORCEACTIVE(CDeviceFontInfoExt_cpp, lbl_eu_80522E18);
DECOMP_FORCEACTIVE(CDeviceFontInfoExt_cpp, lbl_eu_80663718);
DECOMP_FORCEACTIVE(CDeviceFontInfoExt_cpp, lbl_eu_80663720);
