#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// Cross-TU layout helpers (kyoshin .text). Retail references these by their
// unmangled C names (US split strips the C++ mangling), so they need C
// linkage declarations - same convention as CSysWin/CKizunagram/etc.
extern "C" void TagCopyVec2f(float* dst, float* src); // copy 2 floats (VEC2)
extern "C" void setPaneVisible(void* pane, u32 a); // set pane visible flag

/* Vertical/horizontal scroll bar widget; receives the layout archive through
IWorkEvent::OnFileEvent (retail __RTTI__10CScrollBar, IWorkEvent base list;
the ctor's exception record destroys the IWorkEvent base).

State machine (mState):
  - 0: hidden / idle (not visible)
  - 1: entering (animating in via mAnimOffset)
  - 2: visible (scroll bar is shown, scroll active)
  - 3: leaving (animating out)

mActive is 1 when idle/complete, 0 during transitions. */
class CScrollBar : public IWorkEvent {
public:
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
    virtual ~CScrollBar();

    void loadLayoutArc();
    void UpdateDispatch();
    void draw(nw4r::lyt::DrawInfo* drawInfo);
    void Teardown();
    int isVisible();
    int isActive();
    void InitRootPane(const nw4r::math::VEC3* pos);
    void requestScrollIn();
    void requestScrollOut();
    void UpdateThumb(u32 scrollFrom, u32 scrollTo);
    void PlaceThumb(u16 count);
    void StepScrollIn();
    void StepScrollOut();
    void MarkLayoutReady();

    virtual bool OnFileEvent(CEventFile* pEventFile);
};
