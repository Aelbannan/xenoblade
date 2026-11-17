#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CEventFile.hpp"

class CTagProcessor;

// CSysWin - system message window widget.
class CSysWin : public IWorkEvent {
public:
    CSysWin(u8 kind);
    virtual ~CSysWin();
    virtual bool OnFileEvent(CEventFile* pEventFile);
    virtual void loadSystemArc();

    UnkClass_8045F564 mMemRegion;     // +0x04 - scratch region for layout build
    CFileHandle* mFileHandle;         // +0x14 - loaded System.arc file handle
    CTagProcessor* mTagProcessor;     // +0x18 - window tag processor (allocated in OnFileEvent)
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // +0x1C - System.arc resource accessor
    nw4r::lyt::Layout* mLayout;       // +0x20
    nw4r::lyt::AnimTransform* mAnimTrans;  // +0x24
    u8 field_28;                      // +0x28 - ready flag (1 in ctor, 0 after load)
    u32 field_2C;                     // +0x2C
    u32 field_30;                     // +0x30
    u8 field_34;                      // +0x34 - open flag
    u8 field_35;                      // +0x35 - anim phase (0 idle, 1 open, 2 advance, 3 rewind)
    u8 field_36;                      // +0x36 - drawn flag
    u8 field_37;                      // +0x37 - ctor arg (window kind selector)
    u8 field_38;                      // +0x38 - window kind (ctor init 5)
    u8 field_39;                      // +0x39
};
