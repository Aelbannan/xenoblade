// CDeviceFontInfoRom — ROM font info provider.
// FULL_MATCH getters: getRomFont / getRomFont const are inline in header.

#include <harness_catalog.h>
#include "monolib/device/CDeviceFontInfoRom.hpp"
#include "nw4r/ut/ut_RomFont.h"

// ---- CDeviceFontInfoRom constructor (0x804576A8) ----
CDeviceFontInfoRom::CDeviceFontInfoRom(void) {}

// ---- CDeviceFontInfoRom destructor (0x80457760) ----
void CDeviceFontInfoRom::~CDeviceFontInfoRom(void) {}

// ---- func_804536DC (0x80457808) — OSGetFontTexture wrapper ----
void CDeviceFontInfoRom::func_804536DC(void) {}

// ---- create() (0x80457820) — static factory ----
void* CDeviceFontInfoRom::create() { return nullptr; }

// ---- func_8045378C (0x8045785C) — state machine step ----
void CDeviceFontInfoRom::func_8045378C(void) {}

// ---- func_804537A8 (0x80457878) — state check ----
void CDeviceFontInfoRom::func_804537A8(void) {}

// ---- func_804537C0 (0x80457890) — bool return ----
void CDeviceFontInfoRom::func_804537C0(void) {}

// ---- func_804537D4 (0x804578A4) — state comparison ----
void CDeviceFontInfoRom::func_804537D4(void) {}

// ---- func_804537F0 (0x804578C0) — returns &mRomFont (non-const) ----
// FULL_MATCH: size 0x8


// ---- func_804537F8 (0x804578C8) — returns &mRomFont (const) ----
// FULL_MATCH: size 0x8


// ---- func_80453800 (0x804578D0) — reads mFontInfo->field_0x12 ----
u32 CDeviceFontInfoRom::func_80453800() {
    return (u32)mFontInfo;
}

// ---- func_8045380C (0x804578DC) — reads mFontInfo->field_0x18 ----
u16 CDeviceFontInfoRom::func_8045380C() {
    return *(u16*)((u8*)mFontInfo + 0x18);
}

// ---- func_80453818 (0x804578E8) — reads mFontInfo->field_0x20 ----
u16 CDeviceFontInfoRom::func_80453818() {
    return *(u16*)((u8*)mFontInfo + 0x20);
}

// ---- func_80453824 (0x804578F4) — reads mFontInfo->field_0x1E ----
u16 func_80453824__18CDeviceFontInfoRomFv(void* self) {
    return *(u16*)((u8*)((CDeviceFontInfoRom*)self)->mFontInfo + 0x1E);
}

// ---- func_80453830 (0x80457900) — returns field_0x04 ----

