// CDeviceFontInfoRom - ROM font info provider.
//
// This TU keeps its own non-virtual class view of CDeviceFontInfoRom: retail
// stores the shared vtable label (lbl_eu_8056C788) manually and calls no base
// ctor, so the vptr is an explicit member and the ctor init-list store order
// reproduces retail exactly (vtable, 0x4, 0x8, 0xC, 0x10, RomFont ctor, 0x30).

#include <harness_catalog.h>
#include "monolib/device/CDeviceFont.hpp"
#include "nw4r/ut/ut_RomFont.h"
#include <revolution/os/OSFont.h>

// Size: 0x34.
class CDeviceFontInfoRom {
public:
    CDeviceFontInfoRom();
    ~CDeviceFontInfoRom();

    static CDeviceFontInfoRom* create();

    // Returns a pointer to the embedded RomFont (non-const / const).
    nw4r::ut::RomFont* getRomFont() { return &mRomFont; }
    const nw4r::ut::RomFont* getRomFont() const { return &mRomFont; }

    const char* func_804536DC(const char* str, void** texOut, u32* xOut,
                              u32* yOut, u32* widthOut);
    void func_8045378C();   // state machine step
    void func_804537A8();   // state check
    int func_804537C0();    // returns (mState != 0)
    int func_804537D4();    // state comparison
    u16 func_80453800();    // returns mFontInfo->cellHeight
    u16 func_8045380C();    // returns mFontInfo->sheetFormat
    u16 func_80453818();    // returns mFontInfo->sheetHeight
    u16 func_80453824();    // returns mFontInfo->sheetWidth
    u32 func_80453830();    // returns field_0x04

    u32* m_vtable;               // 0x00 - retail vtable (lbl_eu_8056C788)
    u32 field_0x04;              // 0x04 - unknown init flag/counter
    u8* mFontData;               // 0x08 - allocated font data buffer
    u8* mFontInfo;               // 0x0C - font info pointer (set if OSInitFont succeeds)
    u32 mState;                  // 0x10 - state machine value
    nw4r::ut::RomFont mRomFont;  // 0x14 - embedded ROM font instance
    u8* mFontBuffer;             // 0x30 - allocated buffer for RomFont
};

// Retail CDeviceFontInfoRom vtable (.data) - stored manually in the ctor/dtor
// (array extern so the address materialises via lis/addi HA/LO, not sda21).
extern u32 lbl_eu_8056C788[];

// Retail singleton pointer (sdata2), set in the ctor, cleared in the dtor.
extern CDeviceFontInfoRom* lbl_eu_80665688;

// No-arg import of the retail RomFont::GetRequireBufferSize symbol (retail
// callers invoke it without a this-arg; see the ctor comment).
extern "C" u32 GetRequireBufferSize__Q34nw4r2ut7RomFontFv();

// ---- CDeviceFontInfoRom constructor (0x804576A8) ----
// All zero fields live in the member-init list in declaration order; MWCC
// keeps the zero in r31 across the RomFont ctor call for the 0x30 store,
// matching retail's store order.
__declspec(noinline) CDeviceFontInfoRom::CDeviceFontInfoRom(void)
    : m_vtable((u32*)lbl_eu_8056C788),
      field_0x04(0),
      mFontData(0),
      mFontInfo(0),
      mState(0),
      mRomFont(),
      mFontBuffer(0) {
    // Allocate the OS font header buffer (0x145000 bytes).
    mFontData = static_cast<u8*>(mtl::MemManager::allocate_tail(
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
    mFontBuffer = static_cast<u8*>(mtl::MemManager::allocate_tail(
        mtl::MemManager::getHandleStatic(), bufSize, 0x20));
    mRomFont.Load(mFontBuffer);

    lbl_eu_80665688 = this;
}

// ---- CDeviceFontInfoRom destructor (0x80457760) ----
// Resets the vptr, unloads the RomFont, frees the two buffers and clears the
// singleton. The embedded RomFont dtor (-1 flag) and the deleting-flag
// operator delete are emitted implicitly by MWCC.
CDeviceFontInfoRom::~CDeviceFontInfoRom(void) {
    *(u32**)this = (u32*)lbl_eu_8056C788;
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

// ---- func_804536DC (0x80457808) - OSGetFontTexture passthrough wrapper ----
// Retail symbol is func_804536DC__18CDeviceFontInfoRomFv (extern "C" keeps it
// verbatim). The body shifts r4-r8 up and tail-branches OSGetFontTexture: the
// leading `this_` param (unused, in r3) forces the register shift since the 5
// OSGetFontTexture args arrive in r4-r8.
extern "C" const char* func_804536DC__18CDeviceFontInfoRomFv(
    void* this_, const char* str, void** texOut, u32* xOut, u32* yOut,
    u32* widthOut) {
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
// (inline in the class definition above)

// ---- func_804537F8 (0x804578C8) - returns &mRomFont (const) ----
// (inline in the class definition above)

// ---- func_80453800 (0x804578D0) - returns mFontInfo->cellHeight ----
u16 CDeviceFontInfoRom::func_80453800() {
    return ((OSFontHeader*)mFontInfo)->cellHeight;
}

// ---- func_8045380C (0x804578DC) - returns mFontInfo->sheetFormat ----
u16 CDeviceFontInfoRom::func_8045380C() {
    return ((OSFontHeader*)mFontInfo)->sheetFormat;
}

// ---- func_80453818 (0x804578E8) - returns mFontInfo->sheetHeight ----
u16 CDeviceFontInfoRom::func_80453818() {
    return ((OSFontHeader*)mFontInfo)->sheetHeight;
}

// ---- func_80453824 (0x804578F4) - returns mFontInfo->sheetWidth ----
u16 CDeviceFontInfoRom::func_80453824() {
    return ((OSFontHeader*)mFontInfo)->sheetWidth;
}

// ---- func_80453830 (0x80457900) - returns field_0x04 ----
u32 CDeviceFontInfoRom::func_80453830() {
    return field_0x04;
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace FIRBlob {
extern "C" void __dt__18CDeviceFontInfoRomFv();
extern "C" void func_80453830__18CDeviceFontInfoRomFv();
extern "C" void func_80453824__18CDeviceFontInfoRomFv();
extern "C" void func_80453818__18CDeviceFontInfoRomFv();
extern "C" void func_8045380C__18CDeviceFontInfoRomFv();
extern "C" void func_80453800__18CDeviceFontInfoRomFv();
extern "C" void func_804536DC__18CDeviceFontInfoRomFv();
extern "C" void func_804537F0__18CDeviceFontInfoRomFv();
extern "C" void func_804537F8__18CDeviceFontInfoRomFv();
extern "C" void func_8045378C__18CDeviceFontInfoRomFv();
extern "C" void func_804537A8__18CDeviceFontInfoRomFv();
extern "C" void func_804537C0__18CDeviceFontInfoRomFv();
extern "C" void func_804537D4__18CDeviceFontInfoRomFv();
}
extern "C" u32 lbl_eu_80663728[2]; // this unit's sdata
extern "C" u32 lbl_eu_80663720;    // foreign .sdata

// [.data] 0x8056C788-0x8056C7D0 (72B): CDeviceFontInfoRom vtable + sub-view
extern "C" u32 lbl_eu_8056C788[15] = {
    (u32)&lbl_eu_80663728, 0x00000000,
    (u32)&FIRBlob::__dt__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_80453830__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_80453824__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_80453818__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_8045380C__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_80453800__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_804536DC__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_804537F0__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_804537F8__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_8045378C__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_804537A8__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_804537C0__18CDeviceFontInfoRomFv,
    (u32)&FIRBlob::func_804537D4__18CDeviceFontInfoRomFv,
};
extern "C" u32 lbl_eu_8056C7C4[3] = {
    (u32)&lbl_eu_80663720, 0x00000000, 0x00000000,
};

// [.rodata] 0x80522E28-0x80522E40 (24B)
extern "C" __declspec(align(4)) const char lbl_eu_80522E28[0x18] = {
    0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x46,0x6F,0x6E,0x74,0x49,0x6E,0x66,0x6F,0x52,
    0x6F,0x6D,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x80663728-0x80663730 (8B)
extern "C" u32 lbl_eu_80663728[2] = { (u32)&lbl_eu_80522E28, (u32)&lbl_eu_8056C7C4 };

// [.sbss] 0x80665688-0x80665690 (8B) zero-fill
CDeviceFontInfoRom* lbl_eu_80665688;
u32 lbl_eu_8066568C;
