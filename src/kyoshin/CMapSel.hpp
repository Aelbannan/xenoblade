#pragma once

#include <types.h>

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
    u8 mFlag33;                               // +0x33
    u8 func_802436C4() { return mFlag33; }
    void OnFileEvent();
    u8 mScrollBar[0x40];                      // +0x34 - CScrollBar
    u8 mCursor[0x18];                         // +0x74 - CCur18
    s8 mSelY;                                 // +0x8C - selection Y coordinate
    s8 mSelX;                                 // +0x8D - selection X coordinate
    u8 mGridData[0x26];                       // +0x8E - grid lookup data
};

// Extended layout for free-function accessors
struct CMapSelFull {
    u8 _00[0x8C];
    s8 field_8C;  // y coordinate
    s8 field_8D;  // x coordinate
    u8 mGridData[128]; // grid lookup data starting at +0x8E
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
namespace nw4r { namespace lyt { class DrawInfo; } }
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dl__FPv(void*);
extern "C" void func_801F35B0(void*, nw4r::lyt::DrawInfo*);
extern "C" void func_801D20B0(void*, nw4r::lyt::DrawInfo*);
extern "C" void func_801F369C(void*);
// The canonical func_80137510 (nw4r::lyt::AnimTransform*, float) is declared in
// code_80135FDC.hpp. CMapSel.cpp's call site passes a raw u32-held pointer, so
// this legacy void*-typed pseudo-import is kept for that TU; TUs that include
// code_80135FDC.hpp (which redeclares the same extern "C" symbol with the
// canonical signature) define KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
// before including this header to avoid the redeclaration clash (10197).
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
extern "C" u32 func_80137510(void*, float);
#endif
extern "C" float lbl_eu_8066873C;
