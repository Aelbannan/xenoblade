#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

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
    /* Layout (from constructor and extab analysis):
       +0x00: IWorkEvent base (vptr, 4 bytes)
       +0x04: UnkClass_8045F564 -- memory region for layout build (0x10 bytes)
       +0x14: CFileHandle* -- file handle 1
       +0x18: CFileHandle* -- file handle 2
       +0x1C: nw4r::lyt::ArcResourceAccessor*
       +0x20: void* -- allocated memory block
       +0x24: nw4r::lyt::Layout*
       +0x28: nw4r::lyt::AnimTransform*
       +0x2C: nw4r::lyt::AnimTransform*
       +0x30: u8 -- unknown
       +0x31: u8 -- state
       +0x32: u8 -- unknown
       +0x33: u8 -- initial-setup flag (extern C-linkage functions need access)
    */
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
};

// Extended layout for free-function accessors.
// Mirrors the full wrapper object (CMenuMapSelect) at the offsets
// accessed by free functions in this TU.
struct CMapSelFull {
    u8 _00[0x14];                             // +0x00 to +0x13: IWorkEvent base + mMemRegion
    u32 mFileHandle;                          // +0x14
    u8 _18[0x24 - 0x18];                      // +0x18 to +0x23: mFileHandle2, mArcAccessor, mAllocatedMem
    u32 mLayout;                              // +0x24
    u32 mAnimTransform1;                      // +0x28
    u32 mAnimTransform2;                      // +0x2C
    u8 field_0x30;                            // +0x30
    u8 mState;                                // +0x31
    u8 field_0x32;                            // +0x32
    u8 mFlag33;                               // +0x33
    u8 _34[0x74 - 0x34];                      // +0x34 to +0x73: subobject for func_801F34F4
    u8 _74[0x8C - 0x74];                      // +0x74 to +0x8B: CBaseCur-like subobject
    s8 field_8C;                              // +0x8C: y coordinate
    s8 field_8D;                              // +0x8D: x coordinate
    u8 mGridData[0x21];                       // +0x8E: grid lookup (33 entries, up to 0xAE)
    u8 field_0xAF;                            // +0xAF: loading/active flag
    float field_0xB0;                         // +0xB0: animation progress value
};
