#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "kyoshin/CSortMenu.hpp"      // CScrollBarData, CScrollBar_isVisible, func_801F367C
#include "kyoshin/code_80135FDC.hpp"  // func_80137510, func_80137444, func_80136190, func_80136B4C

/* CTutorialList -- tutorial list widget.
   Vtable at lbl_eu_8053A2A8 (split1 .data). Layout-compatible with IWorkEvent
   via vptr at +0x00 for CDeviceFile::readFile, but not a C++ IWorkEvent
   subclass (avoids emitting weak default stubs into this TU's .text).

   Field layout (from retail __ct__CTutorialList 802AF55C + matched call sites):
   +0x00 vptr | +0x04 UnkClass_8045F564 (0x10) | +0x14 six pointers
   (+0x20 Layout*, +0x24/+0x28 AnimTransform*) | +0x2C CCur (0x18)
   | +0x44 CScrollBar (0x40) | +0x84 CSortMenu sub-object | +0x175 control bytes
   | +0x177 mInitialized | +0x17E s8 */
struct CTutorialListLayout {
    void* mVtbl;            // 0x00

    // 0x04-0x83 -- UnkClass_8045F564 (0x10), 6 pointers, CCur (0x18), CScrollBar (0x40)
    u8 gap04[0x80];

    // 0x84 -- CSortMenu sub-object (in-place, ~0x68 bytes)
    u8 mSortMenu[4];        // minimal placeholder for address-of

    // 0x88-0x176 -- copy proxies, data blocks, control flags
    u8 gap88[0xEF];

    u8 mInitialized;        // 0x177 -- non-zero after construction
};

// Data imports (retail symbol names; global data is not C++-mangled).
extern const float lbl_eu_80668DE4;  // anim finish frame bound (.sdata2, sda21)
extern char lbl_eu_80510B78[];       // string pool base

// CSortMenu visibility gate (retail unmangled symbol).
extern "C" u32 func_801D32DC(void*);

class CTutorialList {
public:
    CTutorialList();
    virtual ~CTutorialList();
    void OnFileEvent();

    u8 func_802AD300();
    u8 func_802AD984();

    void func_802ADAE8();
    void func_802ADB3C();
    void func_802ADBDC();
    u8 func_802AD2A4();
    void func_802ADFA8();

private:
    // vptr at +0x00 (implicit from virtual dtor)
    u8 mGap04[0x10];                      // 0x04: UnkClass_8045F564 (0x10)
    void* mField14;                       // 0x14: CFileHandle*
    void* mField18;                       // 0x18: ArcResourceAccessor*
    void* mField1C;                       // 0x1C: Layout*
    nw4r::lyt::Layout* mLayout20;         // 0x20
    nw4r::lyt::AnimTransform* mAnim24;    // 0x24
    nw4r::lyt::AnimTransform* mAnim28;    // 0x28
    u8 mGap2C[0x18];                      // 0x2C: CCur (0x18 bytes)
    CScrollBarData mScrollBar;            // 0x44: CScrollBar (0x40 bytes)
    u8 mSortMenu84[0xF1];                 // 0x84: CSortMenu sub-object (opaque)
    u8 mState175;                         // 0x175: anim/state byte
    u8 mField176;                         // 0x176: visible-selection byte
    u8 mInitialized;                      // 0x177: non-zero after construction
    u8 mGap178[0x6];                      // 0x178-0x17D
    s8 mField17E;                         // 0x17E: sign-extended counter base
};
