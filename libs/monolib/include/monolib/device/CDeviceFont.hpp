#pragma once

#include <types.h>

#include "monolib/device/CDeviceBase.hpp"

#include <nw4r/lyt/lyt_layout.h>

// IDeviceFontInfo - font-info provider interface (impls CDeviceFontInfoRom /
// CDeviceFontInfoExt live in the CDeviceFont unit; see
// libs/monolib/src/device/CDeviceFont.hpp). Retail vtable: RTTI @0x0,
// 0 @0x4, dtor @0x8, then user virtuals in declaration order. Slot +0x24
// (getFont) yields the font handle bound into layout panes by
// func_8013676C. novtable: tables live in the data blob; the interface is
// used only through pointers, so no TU emits a vtable for it.
class __declspec(novtable) IDeviceFontInfo {
public:
    virtual ~IDeviceFontInfo();  // 0x8
    virtual s32 getFlags();      // 0xC
    virtual u32 getState();      // 0x10
    virtual u32 getMode();       // 0x14
    virtual u32 getBufferSize(); // 0x18
    virtual u16 getLineHeight(); // 0x1C
    virtual void func_80453468();// 0x20
    virtual void* getFont();     // 0x24
    virtual void* getFontConst();// 0x28
    virtual void advanceState(); // 0x2C
    virtual void initState();    // 0x30
    virtual u32 isStateNonZero();// 0x34
    virtual u32 isStateReady();  // 0x38
};

//size: 0x1f0
class CDeviceFont : public CDeviceBase {
public:
    CDeviceFont(const char* pName, CWorkThread* pParent);
    static CDeviceFont* getInstance();

    //todo: when true return type is found clean :
    // CMCEffCrystal::MCCrystal_BuildLayouts
    // CTitleAHelp::OnFileEvent
    static void* getFontInfo(u32, nw4r::lyt::Layout*);
    /// Flush font rendering state.
    void dispatchLayer4B70();

    DECL_WORKTHREAD_CREATE(CDeviceFont);

    //0x0: vtable
    //0x0-1c8: CDeviceBase
    u8 unk1C8[0x1F0 - 0x1C8];
};
