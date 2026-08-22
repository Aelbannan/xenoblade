// Auto-scaffolded catalog TU for kyoshin/CScrollBar
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CScrollBar.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// Cross-TU layout helpers (kyoshin .text). Declared as plain C++ like their
// matched call sites (CItemBoxGridSubMenu.cpp) so the reloc binds correctly.
void func_801D2150(nw4r::lyt::Pane*, const nw4r::math::VEC3*);
void func_80124288(void*, float*);
// Retail code80135FDC_setVec3 leaves its first arg (a pointer) in r3, so
// callers (func_801F36BC) reuse it as the returned pointer for func_801D2150.
float* code80135FDC_setVec3(float*, float, float, float);

// Defined at the bottom of this file (declaration NOT yet visible, plus
// noinline) so OnFileEvent emits a direct `bl` to the retail symbol instead
// of inlining the body into the call site (retail calls it out-of-line).
__attribute__((noinline)) void func_801F39B4(CScrollBar* bar);

// The scroll-bar drag pane carries a small data block at +0x2C..+0x50 (thumb
// dimensions / track position). The nw4r Pane layout doesn't model these
// private fields, so read them through this local overlay struct (offset ==
// absolute pane offset).
struct CScrollBarPane {
    u8 pad_00[0x2C]; // +0x00..+0x2B (nw4r Pane internal state)
    f32 field_2C;
    f32 field_30;
    f32 field_34;
    f32 field_38;
    f32 field_3C;
    f32 field_40;
    f32 field_44;
    f32 field_48;
    f32 field_4C;
    f32 field_50;
};

u8 CScrollBar::isVisible() { return mVisible; }


u8 CScrollBar::func_801F3668() { return mActive; }

// Mark the widget ready once its layout has been built.
// Single-arg overload: retail's func_801D2150 leaves its second
// (VEC3*) argument untouched at this call site, so it is not materialized.
void func_801D2150(nw4r::lyt::Pane* pane);
void func_801F3670(CScrollBar* self) {
    func_801D2150(self->mLayout->GetRootPane());
}

void func_801F3850(CScrollBar* self, u32 count) {
    nw4r::lyt::Pane* pane =
        self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DC0[self->mDirection], true);
    nw4r::math::VEC3 vec;
    // Place the drag thumb on the track: X is the thumb height, Y pinned so
    // the thumb stays inside the content as the scroll ratio advances.
    func_801D2150(pane,
                  reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                      &vec.x, self->mThumbHeight,
                      -self->mScrollRatio * (f32)count + self->mContentHeight,
                      lbl_eu_80668138)));
}

/* Per-frame update dispatch helpers (defined below). */
__attribute__((noinline)) void func_801F38FC(CScrollBar* self);
__attribute__((noinline)) void func_801F3960(CScrollBar* self);

/* Per-frame update dispatch: while the bar is ready, step the entering/leaving
animations and always call the layout's Animate. */
void func_801F3540(CScrollBar* self) {
    if (self->mReady == 0) return;
    switch (self->mState) {
    case 1:
        func_801F38FC(self);
        break;
    case 3:
        func_801F3960(self);
        break;
    }
    self->mLayout->Animate(0);
}

/* Entering state: advance the scroll-in animation offset by a frame step; once
it passes the threshold, finish the animation and become visible/idle. */
__attribute__((noinline)) void func_801F38FC(CScrollBar* self) {
    float step = lbl_eu_80668150;
    self->mAnimOffset += step;
    if (self->mAnimOffset < lbl_eu_80668154) return;
    if (func_80137444(self->mAnimTransform, step)) {
        self->mState = 2;
        self->mActive = 1;
    }
}

/* Leaving/shutdown: when the scroll-out animation is done, hide the bar. */
__attribute__((noinline)) void func_801F3960(CScrollBar* self) {
    float frame = lbl_eu_80668150;
    if (func_80137510(self->mAnimTransform, frame)) {
        self->mState = 0;
        self->mActive = 1;
        self->mAnimOffset = lbl_eu_80668138;
    }
}

// Retail OnFileEvent uses the _savegpr_28/_restgpr_28 helper pair
// (size-mode save shape); this TU compiles with -O4,s via configure.py
// extra_cflags rather than a local #pragma.
bool CScrollBar::OnFileEvent(CEventFile* pEventFile) {
    // Only build the layout when the event's file handle matches our own.
    if (mFileHandle == pEventFile->mFileHandle) {
        u32 mem2 = mtl::MemManager::getHandleMEM2();
        // Arc path base stays live across the build (retail caches it in a
        // callee-saved register and indexes +0x16/+0x21/+0x25/+0x36).
        char* path = lbl_eu_80507A4C;
        // Open a scratch region for layout allocation behind this event.
        mMemRegion.createRegion(mem2, 0x3000, path + 0x16, 1);
        Class_8045F858 sp8 = Class_8045F858(&mMemRegion);
        void* data = mFileHandle->getData();
        mtl::MemManager::func_80434A4C(0);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, path + 0x21);
        func_80136E84(&mLayout, mAccessor, path + 0x25);
        func_80136F08(mLayout, &mAnimTransform, mAccessor, path + 0x36);
        mLayout->SetAnimationEnable(mAnimTransform, true);
        mLayout->Animate(0);

        // Pull the thumb dimensions off the layout's drag pane.
        nw4r::lyt::Pane* pane =
            mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DC0[mDirection], true);
        float drag[2];
        func_80127BC4(drag, reinterpret_cast<float*>(reinterpret_cast<u8*>(pane) + 0x4C));
        nw4r::math::VEC3 dims =
            *reinterpret_cast<nw4r::math::VEC3*>(reinterpret_cast<u8*>(pane) + 0x2C);
        mScrollPosY = drag[1];
        mThumbHeight = dims.x;
        mContentHeight = dims.y;

        // Hide all six bar panes, then show only the active direction's thumb.
        for (u8 i = 0; i < 6; i++) {
            nw4r::lyt::Pane* hidePane =
            mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DA8[i], true);
            func_80124270(hidePane, 0);
        }
        nw4r::lyt::Pane* showPane =
            mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DA8[mDirection], true);
        func_80124270(showPane, 1);

        func_801F39B4(this);
        mFileHandle = nullptr;
        mMemRegion.func_8045F810();
        return true;
    }
    return false;
}

/* Read the scroll-bar layout arc; keep the file handle for the load event. */
void CScrollBar::func_801F34F4() {
    u32 handle = mtl::MemManager::getHandleMEM2();
    mFileHandle = CDeviceFile::readFile(
        handle, lbl_eu_80507A4C, reinterpret_cast<IWorkEvent*>(this), 0, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);
}

/* Draw the layout once it is ready and the scroll bar is active. */
void CScrollBar::func_801F35B0(nw4r::lyt::DrawInfo* drawInfo) {
    if (mReady != 0 && mState != 0) {
        func_80137038(mLayout, drawInfo, 0, 1);
    }
}

/* Teardown: release the file handle, destroy the layout/accessor/scratch region,
and reset the widget to its blank state. */
void func_801F35DC(CScrollBar* self) {
    func_801390E0(&self->mFileHandle);
    self->mReady = 0;
    self->mAnimTransform = 0;
    self->mVisible = 0;
    if (self->mLayout != 0) {
        delete self->mLayout;
        self->mLayout = 0;
    }
    func_80139124(self->mAccessor);
    self->mAccessor = 0;
    self->mMemRegion.func_8045F778();
}

/* Request scroll-in: if hidden, transition to entering and mark inactive. */
void CScrollBar::func_801F367C() {
    if (mState == 0) {
        mState = 1;
        mActive = 0;
    }
}

/* Request scroll-out: if visible, transition to leaving and mark inactive. */
void CScrollBar::func_801F369C() {
    if (mState == 2) {
        mState = 3;
        mActive = 0;
    }
}

void func_801F36BC(CScrollBar* self, u32 scrollFrom, u32 scrollTo) {
    nw4r::lyt::Pane* pane =
        self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DC0[self->mDirection], true);
    CScrollBarPane* pdata = reinterpret_cast<CScrollBarPane*>(pane);
    s16 delta = (s16)(scrollTo - scrollFrom);
    if (delta <= 0) {
        // Bottom of the scroll range: park the thumb at the tail position.
        float tmp[2];
        func_80127BC4(tmp, &pdata->field_4C);
        tmp[1] = self->mScrollPosY;
        func_80124288(pane, tmp);
        self->mScrollRatio = lbl_eu_80668138;
        nw4r::math::VEC3 vec;
        func_801D2150(pane,
                      reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                          &vec.x, self->mThumbHeight, self->mContentHeight,
                          lbl_eu_80668138)));
        func_80124270(pane, 0);
    } else {
        // Map the current scroll position onto the thumb track; the stick
        // ratio is the fraction clamped through the lower bound.
        f32 fFrom = (f32)scrollFrom;
        f32 fTo = (f32)scrollTo;
        f32 ratio = self->mScrollPosY * (fFrom / fTo);
        if (ratio < lbl_eu_8066813C)
            ratio = lbl_eu_8066813C;
        float tmp[2];
        func_80127BC4(tmp, &pdata->field_4C);
        tmp[1] = ratio;
        func_80124288(pane, tmp);
        self->mScrollRatio = (self->mScrollPosY - ratio) / (f32)delta;
        nw4r::math::VEC3 vec;
        func_801D2150(pane,
                      reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                          &vec.x, self->mThumbHeight, self->mContentHeight,
                          lbl_eu_80668138)));
        func_80124270(pane, 1);
    }
}

/* Complete-object destructor. The mMemRegion member is destroyed implicitly
(its dtor is a real C++ member dtor, so MWCC emits the external call with
flags=-1), then the deleting-flag check and operator delete. */

CScrollBar::~CScrollBar() {}

/* Construct the scroll bar: the base ctor stores the vtable first, then the
mMemRegion member ctor runs (retail order), then every field is initialized.
mActive starts at 1 (idle) and all offsets at 0. */
CScrollBar::CScrollBar(u8 direction) : CScrollBarVtblBase(), mMemRegion() {
    mFileHandle = 0;
    mAccessor = 0;
    mLayout = 0;
    mAnimTransform = 0;
    mReady = 0;
    mVisible = 0;
    mState = 0;
    mActive = 1;
    mAnimOffset = lbl_eu_80668138;
    mScrollPosY = lbl_eu_80668138;
    mScrollRatio = lbl_eu_80668138;
    mThumbHeight = lbl_eu_80668138;
    mContentHeight = lbl_eu_80668138;
    mDirection = direction;
}

/* Defined last in the TU (declaration at the top) so callers emit a direct
`bl` rather than an inlined copy - retail calls it out-of-line. */
// Layout built successfully: mark the bar ready and visible.
__attribute__((noinline)) void func_801F39B4(CScrollBar* bar) {
    if (bar->mLayout != 0) {
        bar->mVisible = 1;
        bar->mReady = 1;
    }
}
