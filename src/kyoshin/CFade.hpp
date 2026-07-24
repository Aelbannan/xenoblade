#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// IWorkEvent-compatible vtable (split1 .data).
extern "C" void* lbl_eu_80536EA8[];

/* Fade overlay layout widget. Loads and animates a nw4r layout for fade-in/out
effects. Inherits from IWorkEvent for CDeviceFile::readFile completion callback. */
class CFade : public IWorkEvent {
public:
    UnkClass_8045F564 mMemRegion;                // 0x04
    CFileHandle* mFileHandle;                    // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;  // 0x18
    nw4r::lyt::Layout* mLayout;                  // 0x1C
    nw4r::lyt::AnimTransform* mAnimTrans;         // 0x20

    u8 mIsLoaded;   // 0x24 - true after OnFileEvent completes loading
    u8 mFadeState;  // 0x25 - 0:idle, 1:fading in, 2:faded in, 3:fading out
    u8 mReady;      // 0x26 - true after layout is attached and visible
    u8 mVisible;    // 0x27 - visibility flag (toggled during fade animation)
};
