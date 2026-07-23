#pragma once

#include <types.h>
#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/lib.hpp"
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>

// IWorkEvent vtable for CBattery (split1 .data on US/EU; __vt__8CBattery on JP).
extern "C" void* lbl_eu_8053B890[];

/* Sets mVtbl before UnkClass_8045F564 is constructed (retail ctor order). */
struct CBatteryVtblBase {
    void* mVtbl; // 0x0 - lbl_eu_8053B890

    CBatteryVtblBase() {
        mVtbl = lbl_eu_8053B890;
    }
};

/* Battery HUD widget. Layout-compatible with IWorkEvent (vptr @ +0) for
CDeviceFile::readFile, but not a C++ IWorkEvent subclass - that would emit weak
default stubs into this TU's .text and blow the split budget. */
class CBattery : public CBatteryVtblBase {
public:
    CBattery(u8 batteryLevel);
    ~CBattery();
    void func_802B92A4();
    void func_802B92FC();
    void func_802B9334(void*);
    void func_802B9364();
    void setBatteryLevel(u8 level);
    void updateBatteryImage();
    void func_802B94B0();
    bool OnFileEvent(CEventFile* pEventFile);

    UnkClass_8045F564 mMemRegion; // 0x4 - scratch region for layout build
    CFileHandle* mFileHandle; // 0x14
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x18
    nw4r::lyt::Layout* mLayout; // 0x1C
    bool mLayoutReady; // 0x20 - set once brlyt is built; cleared on unload
    s8 unk21; // 0x21
    bool mDrawn; // 0x22 - animate/draw gate
    u8 pad23; // 0x23
    u8 mBatteryLevel; // 0x24
};
