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

    // FULL_MATCH: Returns a pointer to the embedded RomFont (non-const).
    nw4r::ut::RomFont* getRomFont() { return &mRomFont; }

    // FULL_MATCH: Returns a pointer to the embedded RomFont (const).
    const nw4r::ut::RomFont* getRomFont() const { return &mRomFont; }

    // 0x00-0x13: CDeviceFontInfo base class
    nw4r::ut::RomFont mRomFont;  // 0x14 - embedded ROM font instance
    void* mFontBuffer;            // 0x30 - allocated buffer for RomFont
};
