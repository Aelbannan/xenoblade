#pragma once

#include <types.h>

#include "monolib/device/CDeviceFontInfo.hpp"
#include "nw4r/ut/ut_RomFont.h"

/**
 * @brief ROM-based font info provider.
 *
 * CDeviceFontInfoRom loads the system font via OSInitFont and wraps
 * an nw4r::ut::RomFont for rendering. Created as a singleton via create().
 * Size: 0x34.
 */
class CDeviceFontInfoRom : public CDeviceFontInfo {
public:
    CDeviceFontInfoRom();
    virtual ~CDeviceFontInfoRom();

    static CDeviceFontInfoRom* create();

    virtual s32 getFlags();             // returns field_0x04
    virtual u32 getState();             // OS font sheetWidth
    virtual u32 getMode();              // OS font sheetHeight
    virtual u32 getBufferSize();        // OS font sheetFormat
    virtual u16 getLineHeight();        // OS font cellHeight
    virtual const char* getCharGlyph(const char* str, void** texOut, u32* xOut,
                                     u32* yOut, u32* widthOut);
    virtual nw4r::ut::Font* getFont();            // &mRomFont
    virtual const nw4r::ut::Font* getFontConst(); // &mRomFont
    virtual void advanceState();
    virtual void initState();
    virtual u32 isStateNonZero();
    virtual u32 isStateReady();

    // 0x00: CDeviceFontInfo base (vptr)
    u32 field_0x04;              // 0x04 - unknown init flag/counter
    void* mFontData;             // 0x08 - allocated font data buffer
    void* mFontInfo;             // 0x0C - font info pointer (set if OSInitFont succeeds)
    u32 mState;                  // 0x10 - state machine value
    nw4r::ut::RomFont mRomFont;  // 0x14 - embedded ROM font instance
    void* mFontBuffer;           // 0x30 - allocated buffer for RomFont
};
