#pragma once

#include <types.h>

namespace nw4r { namespace ut { class Font; } }

/**
 * @brief Base class for font info providers (ROM and Extended).
 *
 * Retail: dtor __dt__15CDeviceFontInfoFv (inline-empty; the surviving weak
 * copy links from the CDeviceFont unit), RTTI __RTTI__15CDeviceFontInfo
 * (surviving copy in CDeviceFontInfoExt). Vtable: RTTI @0x0, 0 @0x4, dtor
 * @0x8, then the virtuals below in declaration order (0xC..0x38).
 * Same layout as the IDeviceFontInfo pointer-only interface view in
 * monolib/device/CDeviceFont.hpp.
 * Size: 0x4.
 */
class __declspec(novtable) CDeviceFontInfo {
public:
    virtual ~CDeviceFontInfo() {}                // 0x8
    virtual s32 getFlags() = 0;                  // 0xC
    virtual u32 getState() = 0;                  // 0x10
    virtual u32 getMode() = 0;                   // 0x14
    virtual u32 getBufferSize() = 0;             // 0x18
    virtual u16 getLineHeight() = 0;             // 0x1C
    virtual const char* getCharGlyph(const char* str, void** texOut, u32* xOut,
                                     u32* yOut, u32* widthOut) = 0; // 0x20
    virtual nw4r::ut::Font* getFont() = 0;                 // 0x24
    virtual const nw4r::ut::Font* getFontConst() = 0;      // 0x28
    virtual void advanceState() = 0;             // 0x2C
    virtual void initState() = 0;                // 0x30
    virtual u32 isStateNonZero() = 0;            // 0x34
    virtual u32 isStateReady() = 0;              // 0x38

    // 0x00: vtable pointer (no data members: the ctor stores only the vptr)
};
