// CDeviceFontInfoRom - ROM font info provider (derives CDeviceFontInfo; the
// inline base ctor/dtor give retail's vtable/zero-field stores and the
// RomFont + base cleanup actions in the ctor/dtor exception records).

#include <harness_catalog.h>
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceFontInfoRom.hpp"
#include <revolution/os/OSFont.h>

// name-guess: current ROM font-info singleton, set in the ctor and cleared in
// the dtor (adjacent to spDeviceFont/sDeviceFontInitCount in retail .sbss).
CDeviceFontInfoRom* spDeviceFontInfoRom;

// No-arg import of the retail RomFont::GetRequireBufferSize symbol (retail
// callers invoke it without a this-arg; see the ctor comment).
extern "C" u32 GetRequireBufferSize__Q34nw4r2ut7RomFontFv();

// ---- CDeviceFontInfoRom constructor (0x804576A8) ----
// All zero fields live in the member-init list in declaration order; MWCC
// keeps the zero in r31 across the RomFont ctor call for the 0x30 store,
// matching retail's store order.
CDeviceFontInfoRom::CDeviceFontInfoRom(void)
    : field_0x04(0),
      mFontData(0),
      mFontInfo(0),
      mState(0),
      mRomFont(),
      mFontBuffer(0) {
    // Allocate the OS font header buffer (0x145000 bytes).
    mFontData = (mtl::MemManager::allocate_tail(
        mtl::MemManager::getHandleStatic(), 0x145000, 0x20));

    // If OSInitFont succeeds, the returned buffer is a valid font header.
    if (OSInitFont((OSFontHeader*)mFontData)) {
        mFontInfo = mFontData;
    }

    // Allocate the RomFont runtime buffer and load. Retail calls
    // GetRequireBufferSize WITHOUT setting up `this` (the callee reads no
    // members - it switches on OSGetFontEncode), so the call is expressed as
    // a no-arg extern "C" import (same arrangement as the CDeviceFont.cpp
    // layer-dispatcher imports) to avoid the extra `addi r3,r30,0x14`.
    u32 bufSize = GetRequireBufferSize__Q34nw4r2ut7RomFontFv();
    mFontBuffer = mtl::MemManager::allocate_tail(
        mtl::MemManager::getHandleStatic(), bufSize, 0x20);
    mRomFont.Load(mFontBuffer);

    spDeviceFontInfoRom = this;
}

// ---- CDeviceFontInfoRom destructor (0x80457760) ----
// Resets the vptr, unloads the RomFont, frees the two buffers and clears the
// singleton. The embedded RomFont dtor (-1 flag) and the deleting-flag
// operator delete are emitted implicitly by MWCC.
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

    spDeviceFontInfoRom = NULL;
}

// ---- getCharGlyph (0x80457808) - OSGetFontTexture passthrough ----
const char* CDeviceFontInfoRom::getCharGlyph(const char* str, void** texOut,
                                             u32* xOut, u32* yOut, u32* widthOut) {
    return OSGetFontTexture(str, texOut, xOut, yOut, widthOut);
}

// ---- create() (0x80457820) - static factory ----
// The getInstance() result is held in a named temp: MWCC then computes the
// size constant (r3) before loading mAllocHandle from the temp's register,
// matching retail's `mr r4,r3 / li r3,0x34 / lwz r4,0x54(r4)` schedule.
CDeviceFontInfoRom* CDeviceFontInfoRom::create() {
    CDeviceFont* font = CDeviceFont::getInstance();
    return new (font->mAllocHandle) CDeviceFontInfoRom();
}

// ---- func_8045378C (0x8045785C) - state machine step ----
void CDeviceFontInfoRom::advanceState(void) {
    // Increment mState when it is 1 or 2 (unsigned range check)
    if (mState - 1 <= 1) {
        mState++;
    }
}

// ---- func_804537A8 (0x80457878) - state check ----
void CDeviceFontInfoRom::initState(void) {
    if (mState == 0) {
        mState++;
    }
}

// ---- func_804537C0 (0x80457890) - returns (mState != 0) ----
u32 CDeviceFontInfoRom::isStateNonZero() {
    return mState != 0;
}

// ---- func_804537D4 (0x804578A4) - state comparison ----
u32 CDeviceFontInfoRom::isStateReady(void) {
    return (int)mState >= 3;
}

// ---- func_804537F0 (0x804578C0) - returns &mRomFont (non-const) ----
nw4r::ut::Font* CDeviceFontInfoRom::getFont() { return &mRomFont; }

// ---- func_804537F8 (0x804578C8) - returns &mRomFont (const wrapper) ----
const nw4r::ut::Font* CDeviceFontInfoRom::getFontConst() { return &mRomFont; }

// ---- func_80453800 (0x804578D0) - returns mFontInfo->cellHeight ----
u16 CDeviceFontInfoRom::getLineHeight() {
    return ((OSFontHeader*)mFontInfo)->cellHeight;
}

// ---- func_8045380C (0x804578DC) - returns mFontInfo->sheetFormat ----
u32 CDeviceFontInfoRom::getBufferSize() {
    return ((OSFontHeader*)mFontInfo)->sheetFormat;
}

// ---- func_80453818 (0x804578E8) - returns mFontInfo->sheetHeight ----
u32 CDeviceFontInfoRom::getMode() {
    return ((OSFontHeader*)mFontInfo)->sheetHeight;
}

// ---- func_80453824 (0x804578F4) - returns mFontInfo->sheetWidth ----
u32 CDeviceFontInfoRom::getState() {
    return ((OSFontHeader*)mFontInfo)->sheetWidth;
}

// ---- func_80453830 (0x80457900) - returns field_0x04 ----
s32 CDeviceFontInfoRom::getFlags() {
    return field_0x04;
}

// [.sbss] 0x80665688-0x80665690 (8B) zero-fill
// name-guess: paired zero word at 0x8066568C; no live retail reference found.
u32 sDeviceFontInfoRomReserved;
