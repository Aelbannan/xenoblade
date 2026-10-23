#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"

// Forward decls for nw4r layout types used in the layout below.
namespace nw4r { namespace math { struct VEC3; } }
namespace nw4r { namespace lyt { class Layout; class AnimTransform; class DrawInfo; class Pane; class ArcResourceAccessor; } }
struct CFileHandle;
class CEventFile;
class UnkClass_8045F564;

/* Map selection UI widget. Displays a grid of available maps for the player
   to choose from. Uses IWorkEvent-compatible vtable at +0x00 (set by the
   constructor). Sub-objects are opaque byte arrays; their constructors and
   destructors are called via extern "C" retail symbols.

   Layout (0xB4 bytes):
     +0x00: IWorkEvent vtable pointer (set to lbl_eu_80536E10)
     +0x04: UnkClass_8045F564 (0x10 bytes)
     +0x14-+0x33: various fields
     +0x34: CScrollBar (0x40 bytes)
     +0x74: CCur18 (0x18 bytes)
     +0x8C: selection Y coordinate (s8)
     +0x8D: selection X coordinate (s8)
     +0x8E: grid data (0x26 bytes)

   State machine (mState at +0x31):
     0 = uninitialized/hidden
     1 = loading map data (active)
     2 = fully loaded and visible
     3 = transitioning out
     4 = hidden/closed
     5 = post-close cleanup */
class CMapSel {
public:
    CMapSel();
    ~CMapSel();  // defined in CMapSel.cpp as extern "C" __dt__7CMapSelFv
    u8 mVtbl[4];                              // +0x00 - IWorkEvent vtable ptr
    u8 mMemRegion[0x10];                      // +0x04 - UnkClass_8045F564
    CFileHandle* mFileHandle;                 // +0x14 - load-in-progress file handle
    CFileHandle* mFileHandle2;                // +0x18 - second (map-data) file handle
    nw4r::lyt::ArcResourceAccessor* mArcAccessor; // +0x1C
    void* mAllocatedMem;                      // +0x20 - loaded map-data buffer
    nw4r::lyt::Layout* mLayout;               // +0x24
    nw4r::lyt::AnimTransform* mAnimTransform1; // +0x28
    nw4r::lyt::AnimTransform* mAnimTransform2; // +0x2C
    u8 field_0x30;                            // +0x30
    u8 mState;                                // +0x31
    u8 field_0x32;                            // +0x32
    u8 mFlag33;                               // +0x33
    u8 getMapSelInitFlag() { return mFlag33; }
    u8 getMapGridCell();
    bool OnFileEvent(CEventFile* event);
    u8 mScrollBar[0x40];                      // +0x34 - CScrollBar
    CCur18 mCursor;                           // +0x74
    s8 mSelY;                                 // +0x8C - selection Y coordinate
    s8 mSelX;                                 // +0x8D - selection X coordinate
    u8 mGridData[0x22];                       // +0x8E - grid lookup data
    float mTimer;                             // +0xB0 - transition countdown
};
