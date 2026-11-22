// Auto-scaffolded catalog TU for monolib/src/device/CDeviceFontInfoExt
#include <harness_catalog.h>
#include <string.h>
#include <revolution/enc/encunicode.h>
#include <nw4r/ut/ut_Font.h>
#include "monolib/device/CDeviceFontInfoExt.hpp"
#include "monolib/util/MemManager.hpp"

extern "C" {
// .sbss2:0x8066B558 (4B) | lbl_8066DCF8 - font-name blob; its address is passed
// as the pName/tag to nw4r::ut::PackedFont (retail li r4, lbl_8066DCF8@sda21).
// Zero-filled const+initializer aggregate lands in .sbss2 (NOBITS) per the
// MWCC small-data placement rules (docs/MWCC_CASES.md "Data-only TU").
extern const char lbl_8066DCF8[4] = {0, 0, 0, 0};
}

extern "C" void* getFont__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return (void*)((u8*)self + 0x1c); }
extern "C" void* getFontConst__18CDeviceFontInfoExtFv(CDeviceFontInfoExt* self) { return (void*)((u8*)self + 0x1c); }
u16 CDeviceFontInfoExt::getLineHeight() { return field_5C; }
u32 CDeviceFontInfoExt::getBufferSize() { return field_58; }
u32 CDeviceFontInfoExt::getMode() { return mMode; }
u32 CDeviceFontInfoExt::getState() { return mState; }
s32 CDeviceFontInfoExt::getFlags() { return field_04; }

void CDeviceFontInfoExt::advanceState() {
    u32 v = field_60;
    if (v - 1 <= 1) field_60 = v + 1;
}
void CDeviceFontInfoExt::initState() {
    u32 v = field_60;
    if (v == 0) field_60 = v + 1;
}
u32 CDeviceFontInfoExt::isStateNonZero() {
    u32 v = field_60;
    return ((u32)(-(s32)v | (s32)v)) >> 31;
}
u32 CDeviceFontInfoExt::isStateReady() {
    s32 v = (s32)field_60;
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
const char* CDeviceFontInfoExt::getCharGlyph(const char* pStr, void** ppTexture,
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

    nw4r::ut::Font* font = CDeviceFontInfoExt::getFont();
    font->GetGlyph(&work.glyph, work.dest[0]);

    *pCellX = work.glyph.cellX;
    *pCellY = work.glyph.cellY;
    *ppTexture = work.glyph.pTexture;
    // 3-byte CharWidths copy: halfword first, trailing charWidth byte stored
    // after the metric exports.
    s8 cw = work.glyph.widths.charWidth;
    reinterpret_cast<u16&>(work.widths[0]) = reinterpret_cast<u16&>(work.glyph.widths);
    *pGlyphWidth = work.widths[1];
    work.widths[2] = cw;
    return pStr;
}
CDeviceFontInfoExt* CDeviceFontInfoExt::create() {
    CDeviceFont* font = CDeviceFont::getInstance();
    return new (font->mAllocHandle) CDeviceFontInfoExt();
}

extern "C" void __ct__Q34nw4r2ut10PackedFontFv(void* self);
extern "C" void Destroy__Q34nw4r2ut10PackedFontFv(void* self);
extern "C" void __dt__Q34nw4r2ut10PackedFontFv(void* self, int flag);

CDeviceFontInfoExt::CDeviceFontInfoExt() {
    field_04 = 0;
    mFontData = 0;
    mFontInfo = 0;
    mFontBuffer = 0;
    __ct__Q34nw4r2ut10PackedFontFv((u8*)this + 0x1C);
    field_60 = 0;
}

// CDeviceFontInfoExt::~CDeviceFontInfoExt() (us-8045742c, 0x88) -- mirrors the
// accepted CDeviceFontInfoRom dtor recipe: compiler-emitted vptr re-store,
// PackedFont::Destroy, guarded deallocate of the +0x18 buffer, explicit
// PackedFont dtor call with the MWCC destroy-without-delete flag (-1, cf.
// drawGX()->~CDrawGX()), and the implicit deleting-flag operator delete.
CDeviceFontInfoExt::~CDeviceFontInfoExt() {
    Destroy__Q34nw4r2ut10PackedFontFv((u8*)this + 0x1C);
    if (mFontBuffer != NULL) {
        mtl::MemManager::deallocate(mFontBuffer);
        mFontBuffer = NULL;
    }
    __dt__Q34nw4r2ut10PackedFontFv((u8*)((u32)this + 0x1C), -1);
}

