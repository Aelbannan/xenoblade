#pragma once

#include <types.h>
#include <nw4r/ut/ut_Font.h>

class CDeviceFontInfoExt {
public:
    CDeviceFontInfoExt();
    virtual ~CDeviceFontInfoExt();
    // Decodes the first (possibly double-byte) character of pStr to UTF-16,
    // fetches its glyph from the embedded font and copies out its metrics.
    // NOTE: matched as the retail-mangled free symbol
    // func_80453468__18CDeviceFontInfoExtFv in CDeviceFontInfoExt.cpp.
    const char* func_80453468(const char* pStr, u32** ppTexture, u32* pCellX,
                              u32* pCellY, u32* pGlyphWidth);
    void create();
    void func_804535C0();
    void func_804535DC();
    void func_804535F4();
    void func_80453608();

    // TODO: add fields
    void func_80453624();
    void func_8045362C();
    void func_80453634();
    void func_8045363C();
    void func_80453644();
    void func_8045364C();
    void func_80453654();

private:
    u32 field_04;
    u8 field_08[0x14];        // 0x08..0x1C
    nw4r::ut::Font mFont;     // 0x1C
    u8 field_24[0x34];        // 0x24..0x58
    u32 field_58;
    u16 field_5C;
    u16 mMode;
    u16 mState;
    u16 field_5E_pad;
    u32 field_60;
};

