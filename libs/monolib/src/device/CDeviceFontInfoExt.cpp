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
