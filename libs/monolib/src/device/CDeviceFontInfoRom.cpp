// CDeviceFontInfoRom - ROM font info provider.
// FULL_MATCH getters: getRomFont / getRomFont const are inline in header.

#include <harness_catalog.h>
#include "monolib/device/CDeviceFontInfoRom.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "nw4r/ut/ut_RomFont.h"
#include <revolution/os/OSFont.h>

// Singleton pointer set in constructor, cleared in destructor.
extern CDeviceFontInfoRom* lbl_eu_80665688;

// ---- CDeviceFontInfoRom constructor (0x804576A8) ----
CDeviceFontInfoRom::CDeviceFontInfoRom(void) {
    field_0x04 = 0;
    mFontData = NULL;
    mFontInfo = NULL;
    mState = 0;
    mFontBuffer = NULL;

    // Allocate the OS font header buffer (0x145000 bytes).
    mFontData = mtl::MemManager::allocate_tail(
        mtl::MemManager::getHandleStatic(), 0x145000, 0x20);

    // If OSInitFont succeeds, the returned buffer is a valid font header.
    if (OSInitFont(static_cast<OSFontHeader*>(mFontData))) {
        mFontInfo = mFontData;
    }

    // Allocate the RomFont runtime buffer and load.
    u32 bufSize = mRomFont.GetRequireBufferSize();
    mFontBuffer = mtl::MemManager::allocate_tail(
        mtl::MemManager::getHandleStatic(), bufSize, 0x20);
    mRomFont.Load(mFontBuffer);

    lbl_eu_80665688 = this;
}

// ---- CDeviceFontInfoRom destructor (0x80457760) ----
CDeviceFontInfoRom::~CDeviceFontInfoRom(void) {
    mRomFont.Unload();

    if (mFontBuffer != NULL) {
        mtl::MemManager::deallocate(mFontBuffer);
        mFontBuffer = NULL;
    }

    if (mFontData != NULL) {
        mtl::MemManager::deallocate(mFontData);
        mFontData = NULL;
    }

    lbl_eu_80665688 = NULL;
}

// ---- func_804536DC (0x80457808) - OSGetFontTexture wrapper ----
const char* CDeviceFontInfoRom::func_804536DC(const char* str, void** texOut,
                                              u32* xOut, u32* yOut,
                                              u32* widthOut) {
    return OSGetFontTexture(str, texOut, xOut, yOut, widthOut);
}

// ---- create() (0x80457820) - static factory ----
CDeviceFontInfoRom* CDeviceFontInfoRom::create() {
    return new (CDeviceFont::getInstance()->mAllocHandle) CDeviceFontInfoRom();
}

// ---- func_8045378C (0x8045785C) - state machine step ----
void CDeviceFontInfoRom::func_8045378C(void) {
    // Increment mState when it is 1 or 2 (unsigned range check)
    if (mState - 1 <= 1) {
        mState++;
    }
}

// ---- func_804537A8 (0x80457878) - state check ----
void CDeviceFontInfoRom::func_804537A8(void) {
    if (mState == 0) {
        mState++;
    }
}

// ---- func_804537C0 (0x80457890) - returns (mState != 0) ----
int CDeviceFontInfoRom::func_804537C0() {
    return mState != 0;
}

// ---- func_804537D4 (0x804578A4) - state comparison ----
int CDeviceFontInfoRom::func_804537D4(void) {
    return (int)mState >= 3;
}

// ---- func_804537F0 (0x804578C0) - returns &mRomFont (non-const) ----
// FULL_MATCH: size 0x8


// ---- func_804537F8 (0x804578C8) - returns &mRomFont (const) ----
// FULL_MATCH: size 0x8


// ---- func_80453800 (0x804578D0) - returns mFontInfo->cellHeight ----
u16 CDeviceFontInfoRom::func_80453800() {
    return static_cast<OSFontHeader*>(mFontInfo)->cellHeight;
}

// ---- func_8045380C (0x804578DC) - returns mFontInfo->sheetFormat ----
u16 CDeviceFontInfoRom::func_8045380C() {
    return static_cast<OSFontHeader*>(mFontInfo)->sheetFormat;
}

// ---- func_80453818 (0x804578E8) - returns mFontInfo->sheetHeight ----
u16 CDeviceFontInfoRom::func_80453818() {
    return static_cast<OSFontHeader*>(mFontInfo)->sheetHeight;
}

// ---- func_80453824 (0x804578F4) - returns mFontInfo->sheetWidth ----
u16 CDeviceFontInfoRom::func_80453824() {
    return static_cast<OSFontHeader*>(mFontInfo)->sheetWidth;
}

// ---- func_80453830 (0x80457900) - returns field_0x04 ----

