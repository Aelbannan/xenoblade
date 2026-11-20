#pragma once

#include <types.h>

#include "monolib/device/CDeviceBase.hpp"
#include "monolib/device/CDeviceFontInfo.hpp"
#include "monolib/util/reslist.hpp"

#include <nw4r/lyt/lyt_layout.h>

// IDeviceFontInfo - font-info provider interface (impls CDeviceFontInfoRom /
// CDeviceFontInfoExt live in the CDeviceFont unit; see
// libs/monolib/src/device/CDeviceFont.hpp). Retail vtable: RTTI @0x0,
// 0 @0x4, dtor @0x8, then user virtuals in declaration order. Slot +0x24
// (getFont) yields the font handle bound into layout panes by
// ApplyFontToTxtPanes. novtable: tables live in the data blob; the interface is
// used only through pointers, so no TU emits a vtable for it.
class __declspec(novtable) IDeviceFontInfo {
public:
    virtual ~IDeviceFontInfo();  // 0x8
    virtual s32 getFlags();      // 0xC
    virtual u32 getState();      // 0x10
    virtual u32 getMode();       // 0x14
    virtual u32 getBufferSize(); // 0x18
    virtual u16 getLineHeight(); // 0x1C
    virtual void getCharGlyph();// 0x20
    virtual void* getFont();     // 0x24
    virtual void* getFontConst();// 0x28
    virtual void advanceState(); // 0x2C
    virtual void initState();    // 0x30
    virtual u32 isStateNonZero();// 0x34
    virtual u32 isStateReady();  // 0x38
};

namespace nw4r { namespace lyt { class Layout; } }

//size: 0x1f0
class CDeviceFont : public CDeviceBase {
public:
    CDeviceFont(const char* pName, CWorkThread* pParent);
    virtual ~CDeviceFont();
    static CDeviceFont* getInstance();

    DECL_WORKTHREAD_CREATE(CDeviceFont);

    virtual void wkUpdate();
    virtual void wkRender();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    u32 getFontId();
    u32 func_804525F0();
    static void* getFontInfo(u32 fontId, nw4r::lyt::Layout* layout);
    void dispatchLayer4B70();
    CDeviceFontInfo* findFontByDevice();
    u32 createFontInfo();

    //0x0: vtable
    //0x0-1c8: CDeviceBase
    reslist<IDeviceFontInfo*> mInfoList; //0x1C8
    u32 mFontId;                         //0x1E8
    s32 field_0x1EC;                     //0x1EC
};
