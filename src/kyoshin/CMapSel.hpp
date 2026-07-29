#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CBaseCur.hpp"

/* Map selection UI widget. Displays a grid of available maps for the player
   to choose from. Contains a CScrollBar for scrolling through available maps
   and a CCur18 cursor for highlighting the current selection.

   State machine (mState at +0x31):
     0 = uninitialized/hidden
     1 = loading map data (active)
     2 = fully loaded and visible
     3 = transitioning out
     4 = hidden/closed
     5 = post-close cleanup

   mFlag33 (+0x33) is a boolean that tracks whether the widget needs an initial
   setup pass; it is set to 1 in the constructor and cleared when a "play" or
   "close" state is entered. */
class CMapSel : public IWorkEvent {
private:
    UnkClass_8045F564 mMemRegion;            // +0x04, size 0x10
    u32 mFileHandle;                          // +0x14
    u32 mFileHandle2;                         // +0x18
    u32 mArcAccessor;                         // +0x1C
    u32 mAllocatedMem;                        // +0x20
    u32 mLayout;                              // +0x24
    u32 mAnimTransform1;                      // +0x28
    u32 mAnimTransform2;                      // +0x2C
    u8 field_0x30;                            // +0x30
    u8 mState;                                // +0x31
    u8 field_0x32;                            // +0x32

public:
    u8 mFlag33;                               // +0x33 -- initial-setup flag
    u8 func_802436C4() { return mFlag33; }
    virtual ~CMapSel();
    void OnFileEvent();

private:
    CScrollBar mScrollBar;                    // +0x34, size 0x40
    CBaseCur mCursor;                         // +0x74, size 0x18 (CCur18)
};

// Extended layout for free-function accessors
struct CMapSelFull {
    u8 _00[0x8C];
    s8 field_8C;  // y coordinate
    s8 field_8D;  // x coordinate
    u8 mGridData[128]; // grid lookup data starting at +0x8E
};
