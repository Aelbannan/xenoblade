#pragma once

#include <types.h>
#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/lib.hpp"
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>

/* Battery HUD widget; receives its archive through IWorkEvent::OnFileEvent. */
class CBattery : public IWorkEvent {
public:
    CBattery(u8 batteryLevel);
    virtual ~CBattery();
    void loadBatteryArchive();
    void updateLayout();
    void drawBattery(void*);
    void releaseLayout();
    void setBatteryLevel(u8 level);
    void updateBatteryImage();
    void onLayoutReady();
    virtual bool OnFileEvent(CEventFile* pEventFile);

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
