// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFontInfoExt
#include <harness_catalog.h>
#include <string.h>
#include <revolution/enc/encunicode.h>
#include <nw4r/ut/ut_Font.h>
#include "monolib/device/CDeviceFontInfoExt.hpp"

extern "C" {
// .sbss2:0x8066B558 (4B) | lbl_8066DCF8 - font-name blob; its address is passed
// as the pName/tag to nw4r::ut::PackedFont (retail li r4, lbl_8066DCF8@sda21).
// Zero-filled const+initializer aggregate lands in .sbss2 (NOBITS) per the
// MWCC small-data placement rules (docs/MWCC_CASES.md "Data-only TU").
extern const char lbl_8066DCF8[4] = {0, 0, 0, 0};
}

extern "C" void* getFont__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return (void*)((u8*)self + 0x1c); }
extern "C" void* getFontConst__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return (void*)((u8*)self + 0x1c); }
extern "C" u16 getLineHeight__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->field_5C; }
extern "C" u32 getBufferSize__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->field_58; }
extern "C" u16 getMode__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->mMode; }
extern "C" u16 getState__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->mState; }
extern "C" u32 getFlags__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return self->field_04; }

extern "C" void advanceState__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) {
    u32 v = self->field_60;
    if (v - 1 <= 1) self->field_60 = v + 1;
}
extern "C" void initState__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) {
    u32 v = self->field_60;
    if (v == 0) self->field_60 = v + 1;
}
extern "C" u32 isStateNonZero__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) {
    u32 v = self->field_60;
    return ((u32)(-(s32)v | (s32)v)) >> 31;
}
extern "C" u32 isStateReady__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) {
    s32 v = (s32)self->field_60;
    return v >= 3;
}


// Byte-uniform view of CharWidths so the 3-byte copy lowers as halfword+byte.
struct PlainWidths {
    u8 left;
    u8 glyphWidth;
    u8 charWidth;
};

// Local work area for the character query (MWCC lays this out as one block:
// destLen/srcLen/widths copy/UTF-16 dest/SJIS-style src/glyph).
struct DeviceFontCharWork {
    u32 destLen;
    u32 srcLen;
    u8 widths[3];   // CharWidths copy: left/glyphWidth/charWidth
    u16 dest[2];
    char src[3];
    nw4r::ut::Glyph glyph;
};
extern "C" const char* func_80453468__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self,
                                                       const char* pStr, u32** ppTexture,
                                                       u32* pCellX, u32* pCellY,
                                                       u32* pGlyphWidth) {
    DeviceFontCharWork work;

    reinterpret_cast<u16&>(work.src[0]) = 0;
    work.src[2] = 0;
    work.src[0] = pStr[0];
    ++pStr;
    work.dest[0] = 0;
    work.dest[1] = 0;
    // Lead byte re-read from the buffer keeps the boundary compares on the
    // loaded value.
    u8 lead = static_cast<u8>(work.src[0]);
    switch ((u32)lead) {
    case 0x81:
    case 0x82:
    case 0x83:
    case 0x84:
    case 0x85:
    case 0x86:
    case 0x87:
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:
    case 0x8C:
    case 0x8D:
    case 0x8E:
    case 0x8F:
    case 0x90:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:
    case 0x97:
    case 0x98:
    case 0x99:
    case 0x9A:
    case 0x9B:
    case 0x9C:
    case 0x9D:
    case 0x9E:
    case 0x9F:
    case 0xE0:
    case 0xE1:
    case 0xE2:
    case 0xE3:
    case 0xE4:
    case 0xE5:
    case 0xE6:
    case 0xE7:
    case 0xE8:
    case 0xE9:
    case 0xEA:
    case 0xEB:
    case 0xEC:
    case 0xED:
    case 0xEE:
    case 0xEF:
        work.src[1] = pStr[0];
        ++pStr;
        break;
    default:
        break;
    }

    // Convert to UTF-16 and fetch that codepoint's glyph from the embedded font.
    work.destLen = 2;
    work.srcLen = strlen(work.src);
    ENCConvertStringUtf8ToUtf16(work.dest, &work.destLen, (const u8*)work.src, &work.srcLen);

    nw4r::ut::Font* font = reinterpret_cast<nw4r::ut::Font*>(reinterpret_cast<u8*>(self) + 0x1C);
    font->GetGlyph(&work.glyph, work.dest[0]);

    *pCellX = work.glyph.cellX;
    *pCellY = work.glyph.cellY;
    *ppTexture = static_cast<u32*>(work.glyph.pTexture);
    // 3-byte CharWidths copy: halfword first, trailing charWidth byte stored
    // after the metric exports.
    s8 cw = work.glyph.widths.charWidth;
    reinterpret_cast<u16&>(work.widths[0]) = reinterpret_cast<u16&>(work.glyph.widths);
    *pGlyphWidth = work.widths[1];
    work.widths[2] = cw;
    return pStr;
}
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
    (u32)getFlags__18CDeviceFontInfoExtFv,
    (u32)getState__18CDeviceFontInfoExtFv,
    (u32)getMode__18CDeviceFontInfoExtFv,
    (u32)getBufferSize__18CDeviceFontInfoExtFv,
    (u32)getLineHeight__18CDeviceFontInfoExtFv,
    (u32)func_80453468__18CDeviceFontInfoExtFv,
    (u32)getFont__18CDeviceFontInfoExtFv,
    (u32)getFontConst__18CDeviceFontInfoExtFv,
    (u32)advanceState__18CDeviceFontInfoExtFv,
    (u32)initState__18CDeviceFontInfoExtFv,
    (u32)isStateNonZero__18CDeviceFontInfoExtFv,
    (u32)isStateReady__18CDeviceFontInfoExtFv,
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

