// CDeviceFontInfoRom — ROM font info provider.
// FULL_MATCH getters: getRomFont / getRomFont const are inline in header.

#include <harness_catalog.h>
#include "monolib/device/CDeviceFontInfoRom.hpp"
#include "nw4r/ut/ut_RomFont.h"

// ---- CDeviceFontInfoRom constructor (0x804576A8) ----
extern "C" void __ct__18CDeviceFontInfoRomFv(void) {}

// ---- CDeviceFontInfoRom destructor (0x80457760) ----
extern "C" void __dt__18CDeviceFontInfoRomFv(void) {}

// ---- func_804536DC (0x80457808) — OSGetFontTexture wrapper ----
extern "C" void func_804536DC__18CDeviceFontInfoRomFv(void) {}

// ---- create() (0x80457820) — static factory ----
extern "C" void* create__18CDeviceFontInfoRomFv() { return nullptr; }

// ---- func_8045378C (0x8045785C) — state machine step ----
extern "C" void func_8045378C__18CDeviceFontInfoRomFv(void) {}

// ---- func_804537A8 (0x80457878) — state check ----
extern "C" void func_804537A8__18CDeviceFontInfoRomFv(void) {}

// ---- func_804537C0 (0x80457890) — bool return ----
extern "C" void func_804537C0__18CDeviceFontInfoRomFv(void) {}

// ---- func_804537D4 (0x804578A4) — state comparison ----
extern "C" void func_804537D4__18CDeviceFontInfoRomFv(void) {}

// ---- func_804537F0 (0x804578C0) — returns &mRomFont (non-const) ----
// FULL_MATCH: size 0x8
extern "C" nw4r::ut::RomFont* func_804537F0__18CDeviceFontInfoRomFv(CDeviceFontInfoRom* self) {
    return self->getRomFont();
}

// ---- func_804537F8 (0x804578C8) — returns &mRomFont (const) ----
// FULL_MATCH: size 0x8
extern "C" const nw4r::ut::RomFont* func_804537F8__18CDeviceFontInfoRomFv(const CDeviceFontInfoRom* self) {
    return self->getRomFont();
}

// ---- func_80453800 (0x804578D0) — reads mFontInfo->field_0x12 ----
extern "C" u32 func_80453800__18CDeviceFontInfoRomFv(void* self) {
    return *(u32*)((u8*)self + 0x0C);
}

// ---- func_8045380C (0x804578DC) — reads mFontInfo->field_0x18 ----
extern "C" u16 func_8045380C__18CDeviceFontInfoRomFv(void* self) {
    return *(u16*)((u8*)self + 0x0C + 0x18);
}

// ---- func_80453818 (0x804578E8) — reads mFontInfo->field_0x20 ----
extern "C" u16 func_80453818__18CDeviceFontInfoRomFv(void* self) {
    return *(u16*)((u8*)self + 0x0C + 0x20);
}

// ---- func_80453824 (0x804578F4) — reads mFontInfo->field_0x1E ----
u16 func_80453824__18CDeviceFontInfoRomFv(void* self) {
    return *(u16*)((u8*)self + 0x0C + 0x1E);
}

// ---- func_80453830 (0x80457900) — returns field_0x04 ----
extern "C" u32 func_80453830__18CDeviceFontInfoRomFv(void* self) {
    return *(u32*)((u8*)self + 0x04);
}
