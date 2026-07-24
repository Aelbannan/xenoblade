// CDeviceFontInfoRom — ROM font info provider.
// FULL_MATCH getters: getRomFont / getRomFont const are inline in header.

#include <harness_catalog.h>
#include "monolib/device/CDeviceFontInfoRom.hpp"
#include "nw4r/ut/ut_RomFont.h"

// ---- CDeviceFontInfoRom constructor (0x804576A8) ----
// LLM-HARNESS-BEGIN: us-804576a8
extern "C" void __ct__18CDeviceFontInfoRomFv(void) {}
// LLM-HARNESS-END: us-804576a8

// ---- CDeviceFontInfoRom destructor (0x80457760) ----
// LLM-HARNESS-BEGIN: us-80457760
extern "C" void __dt__18CDeviceFontInfoRomFv(void) {}
// LLM-HARNESS-END: us-80457760

// ---- func_804536DC (0x80457808) — OSGetFontTexture wrapper ----
// LLM-HARNESS-BEGIN: us-80457808
extern "C" void func_804536DC__18CDeviceFontInfoRomFv(void) {}
// LLM-HARNESS-END: us-80457808

// ---- create() (0x80457820) — static factory ----
// LLM-HARNESS-BEGIN: us-80457820
extern "C" void* create__18CDeviceFontInfoRomFv() { return nullptr; }
// LLM-HARNESS-END: us-80457820

// ---- func_8045378C (0x8045785C) — state machine step ----
// LLM-HARNESS-BEGIN: us-8045785c
extern "C" void func_8045378C__18CDeviceFontInfoRomFv(void) {}
// LLM-HARNESS-END: us-8045785c

// ---- func_804537A8 (0x80457878) — state check ----
// LLM-HARNESS-BEGIN: us-80457878
extern "C" void func_804537A8__18CDeviceFontInfoRomFv(void) {}
// LLM-HARNESS-END: us-80457878

// ---- func_804537C0 (0x80457890) — bool return ----
// LLM-HARNESS-BEGIN: us-80457890
extern "C" void func_804537C0__18CDeviceFontInfoRomFv(void) {}
// LLM-HARNESS-END: us-80457890

// ---- func_804537D4 (0x804578A4) — state comparison ----
// LLM-HARNESS-BEGIN: us-804578a4
extern "C" void func_804537D4__18CDeviceFontInfoRomFv(void) {}
// LLM-HARNESS-END: us-804578a4

// ---- func_804537F0 (0x804578C0) — returns &mRomFont (non-const) ----
// FULL_MATCH: size 0x8
// LLM-HARNESS-BEGIN: us-804578c0
extern "C" nw4r::ut::RomFont* func_804537F0__18CDeviceFontInfoRomFv(CDeviceFontInfoRom* self) {
    return self->getRomFont();
}
// LLM-HARNESS-END: us-804578c0

// ---- func_804537F8 (0x804578C8) — returns &mRomFont (const) ----
// FULL_MATCH: size 0x8
// LLM-HARNESS-BEGIN: us-804578c8
extern "C" const nw4r::ut::RomFont* func_804537F8__18CDeviceFontInfoRomFv(const CDeviceFontInfoRom* self) {
    return self->getRomFont();
}
// LLM-HARNESS-END: us-804578c8

// ---- func_80453800 (0x804578D0) — reads mFontInfo->field_0x12 ----
// LLM-HARNESS-BEGIN: us-804578d0
extern "C" u32 func_80453800__18CDeviceFontInfoRomFv(void* self) {
    return *(u32*)((u8*)self + 0x0C);
}
// LLM-HARNESS-END: us-804578d0

// ---- func_8045380C (0x804578DC) — reads mFontInfo->field_0x18 ----
// LLM-HARNESS-BEGIN: us-804578dc
extern "C" u16 func_8045380C__18CDeviceFontInfoRomFv(void* self) {
    return *(u16*)((u8*)self + 0x0C + 0x18);
}
// LLM-HARNESS-END: us-804578dc

// ---- func_80453818 (0x804578E8) — reads mFontInfo->field_0x20 ----
// LLM-HARNESS-BEGIN: us-804578e8
extern "C" u16 func_80453818__18CDeviceFontInfoRomFv(void* self) {
    return *(u16*)((u8*)self + 0x0C + 0x20);
}
// LLM-HARNESS-END: us-804578e8

// ---- func_80453824 (0x804578F4) — reads mFontInfo->field_0x1E ----
// LLM-HARNESS-BEGIN: us-804578f4
extern "C" u16 func_80453824__18CDeviceFontInfoRomFv(void* self) {
    return *(u16*)((u8*)self + 0x0C + 0x1E);
}
// LLM-HARNESS-END: us-804578f4

// ---- func_80453830 (0x80457900) — returns field_0x04 ----
// LLM-HARNESS-BEGIN: us-80457900
extern "C" u32 func_80453830__18CDeviceFontInfoRomFv(void* self) {
    return *(u32*)((u8*)self + 0x04);
}
// LLM-HARNESS-END: us-80457900
