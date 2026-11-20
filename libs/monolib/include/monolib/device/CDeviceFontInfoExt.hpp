#pragma once

#include <types.h>
#include "monolib/device/CDeviceFont.hpp"

class CDeviceFontInfoExt : public IDeviceFontInfo {
public:
    CDeviceFontInfoExt();
    virtual ~CDeviceFontInfoExt();

    virtual const char* getCharGlyph(const char* pStr, void** ppTexture, u32* pCellX,
                                     u32* pCellY, u32* pGlyphWidth);
    static CDeviceFontInfoExt* create();
    virtual void advanceState();
    virtual void initState();
    virtual u32 isStateNonZero();
    virtual u32 isStateReady();

    virtual nw4r::ut::Font* getFont();
    virtual const nw4r::ut::Font* getFontConst();
    virtual u16 getLineHeight();
    virtual u32 getBufferSize();
    virtual u32 getMode();
    virtual u32 getState();
    virtual s32 getFlags();

    u32 field_04;
    void* mFontData;  //written from allocate_tail void*
    void* mFontInfo;
    u16 mState;
    u16 mMode;
    u8 _14[0x44];
    u32 field_58;
    u16 field_5C;
    u16 _5E;
    u32 field_60;
};
