#pragma once

#include <types.h>

/**
 * @brief Base class for font info providers (ROM and Extended).
 *
 * CDeviceFontInfo provides the common fields shared by
 * CDeviceFontInfoRom and CDeviceFontInfoExt.
 * Size: 0x14.
 */
class CDeviceFontInfo {
public:
    CDeviceFontInfo();
    virtual ~CDeviceFontInfo();

    // 0x00: vtable pointer
    u32 field_0x04;   // 0x04 - unknown init flag/counter
    void* mFontData;  // 0x08 - allocated font data buffer
    void* mFontInfo;  // 0x0C - font info pointer (set if OSInitFont succeeds)
    u32 mState;       // 0x10 - state machine value
};
