// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFontInfoExt
#include <harness_catalog.h>
#include "monolib/device/CDeviceFontInfoExt.hpp"

extern "C" void* func_80453624__18CDeviceFontInfoExtFv(void* self) { return (void*)((u8*)self + 0x1c); }
extern "C" void* func_8045362C__18CDeviceFontInfoExtFv(void* self) { return (void*)((u8*)self + 0x1c); }
extern "C" u16 func_80453634__18CDeviceFontInfoExtFv(void* self) { return ((CDeviceFontInfoExt*)self)->field_5C; }
extern "C" u32 func_8045363C__18CDeviceFontInfoExtFv(void* self) { return ((CDeviceFontInfoExt*)self)->field_58; }
extern "C" u16 func_80453644__18CDeviceFontInfoExtFv(void* self) { return ((CDeviceFontInfoExt*)self)->mMode; }
extern "C" u16 func_8045364C__18CDeviceFontInfoExtFv(void* self) { return ((CDeviceFontInfoExt*)self)->mState; }
extern "C" u32 func_80453654__18CDeviceFontInfoExtFv(void* self) { return ((CDeviceFontInfoExt*)self)->field_04; }

extern "C" void func_804535C0__18CDeviceFontInfoExtFv(void* self) {
    u32 v = ((CDeviceFontInfoExt*)self)->field_60;
    if (v - 1 <= 1) ((CDeviceFontInfoExt*)self)->field_60 = v + 1;
}
extern "C" void func_804535DC__18CDeviceFontInfoExtFv(void* self) {
    u32 v = ((CDeviceFontInfoExt*)self)->field_60;
    if (v == 0) ((CDeviceFontInfoExt*)self)->field_60 = v + 1;
}
extern "C" u32 func_804535F4__18CDeviceFontInfoExtFv(void* self) {
    u32 v = ((CDeviceFontInfoExt*)self)->field_60;
    return ((u32)(-(s32)v | (s32)v)) >> 31;
}
extern "C" u32 func_80453608__18CDeviceFontInfoExtFv(void* self) {
    s32 v = (s32)((CDeviceFontInfoExt*)self)->field_60;
    return v >= 3;
}


extern "C" void func_80453468__18CDeviceFontInfoExtFv() {}
extern "C" void create__18CDeviceFontInfoExtFv() {}
