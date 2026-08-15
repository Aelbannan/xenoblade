#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"

#include <nw4r/lyt.h>

// IWorkEvent-compatible vtable for CScrollBar (split1 .data).
extern "C" void* lbl_eu_80534DD8[];

// Scroll-bar arc file path (split1 .rodata).
extern "C" char lbl_eu_80507A4C[];

// Pane-name tables for the scroll-bar layout (split1 .data).
// tbl_80534DC0[mDirection] is the drag thumb; tbl_80534DA8[] are the 6
// scroll-position/bar panes hidden while idle.
extern "C" const char* lbl_eu_80534DC0[];
extern "C" const char* lbl_eu_80534DA8[];

// Sdata2 float constants for the scroll animation (split2 .sdata2).
extern const float lbl_eu_80668138; // 0.0 - zero / reset offset
extern const float lbl_eu_8066813C; // 0.0 - scroll-stick ratio lower clamp
extern const float lbl_eu_80668150; // scroll animation frame step
extern const float lbl_eu_80668154; // entering-done threshold

// Cross-TU layout helpers (kyoshin .text). Retail references these by their
// unmangled C names (US split strips the C++ mangling), so they need C
// linkage declarations - same convention as CSysWin/CKizunagram/etc.
extern "C" void func_80127BC4(float* dst, float* src); // copy 2 floats (VEC2)
extern "C" void func_80124270(void* pane, u32 a); // set pane visible flag

/* Sets mVtbl before UnkClass_8045F564 is constructed (retail ctor order:
vtable store first, then the member ctor). Same idiom as CBatteryVtblBase. */
struct CScrollBarVtblBase {
    void* mVtbl; // 0x0 - lbl_eu_80534DD8

    CScrollBarVtblBase() {
        mVtbl = lbl_eu_80534DD8;
    }
};

/* Vertical/horizontal scroll bar widget. Layout-compatible with IWorkEvent
(vptr @ +0) for CDeviceFile::readFile, but not a C++ IWorkEvent subclass
(avoids emitting weak default stubs into this TU's .text).

State machine (mState):
  - 0: hidden / idle (not visible)
  - 1: entering (animating in via mAnimOffset)
  - 2: visible (scroll bar is shown, scroll active)
  - 3: leaving (animating out)

mActive is 1 when idle/complete, 0 during transitions. */
struct CScrollBar : CScrollBarVtblBase {
    UnkClass_8045F564 mMemRegion; // 0x4 - scratch region for layout build
    CFileHandle* mFileHandle; // 0x14
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x18
    nw4r::lyt::Layout* mLayout; // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransform; // 0x20
    u8 mReady; // 0x24 - layout built and ready to draw
    u8 mVisible; // 0x25 - display requested (triggers show animation)
    u8 mState; // 0x26 - 0=hidden, 1=entering, 2=visible, 3=leaving
    u8 mActive; // 0x27 - 1 when idle, 0 during transitions
    f32 mAnimOffset; // 0x28 - scroll-in/out animation offset
    f32 mScrollPosY; // 0x2C - Y position from layout
    f32 mScrollRatio; // 0x30 - scroll position ratio (0.0-1.0)
    f32 mThumbHeight; // 0x34 - visible thumb area height
    f32 mContentHeight; // 0x38 - full content height
    u8 mDirection; // 0x3C - 0=horizontal, 1=vertical (constructor param)

    CScrollBar(u8 direction);
    ~CScrollBar();
    bool OnFileEvent(CEventFile* pEventFile);

    u8 isVisible();
    u8 func_801F3668();

    void func_801F34F4();                              // read layout arc
    void func_801F35B0(nw4r::lyt::DrawInfo* drawInfo); // draw when ready/active
    void func_801F367C();                              // request scroll-in
    void func_801F369C();                              // request scroll-out
};
