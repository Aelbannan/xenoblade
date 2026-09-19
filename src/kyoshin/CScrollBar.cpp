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

// Cross-TU layout helpers (kyoshin .text). Retail keeps these unmangled at
// CScrollBar call sites; extern "C" stops MWCC from emitting __F… tails.
extern "C" void Cur_SetPaneTranslate(nw4r::lyt::Pane*, const nw4r::math::VEC3*);
extern "C" void writePanePos(nw4r::lyt::Pane*, float*);
void closeFileHandle(CFileHandle**);
// Retail code80135FDC_setVec3 leaves its first arg (a pointer) in r3, so
// callers (CScrollBar_UpdateThumb) reuse it as the returned pointer for Cur_SetPaneTranslate.
extern "C" float* code80135FDC_setVec3(float*, float, float, float);

// Defined at the bottom of this file (declaration NOT yet visible, plus
// noinline) so OnFileEvent emits a direct `bl` to the retail symbol instead
// of inlining the body into the call site (retail calls it out-of-line).
__attribute__((noinline)) void CScrollBar_MarkLayoutReady(CScrollBar* bar);

// The scroll-bar drag pane carries a small data block at +0x2C..+0x50 (thumb
// dimensions / track position). The nw4r Pane layout doesn't model these
// private fields, so read them through this local overlay struct (offset ==
// absolute pane offset).
struct CScrollBarPane {
    u8 pad_00[0x2C]; // +0x00..+0x2B (nw4r Pane internal state)
    nw4r::math::VEC3 mDims; // +0x2C thumb/track size (x=thumb height, y=content)
    u8 pad_38[0x4C - 0x38];
    f32 mDrag[2]; // +0x4C drag position (copied via TagCopyVec2f)
};

u8 CScrollBar::isVisible() { return mVisible; }


u8 CScrollBar::isActive() { return mActive; }

// Mark the widget ready once its layout has been built.
// Single-arg call: retail leaves r4 untouched; cast keeps the same
// unmangled reloc (C forbids arity overloads on extern "C").
void CScrollBar_InitRootPane(CScrollBar* self) {
    ((void (*)(nw4r::lyt::Pane*))Cur_SetPaneTranslate)(self->mLayout->GetRootPane());
}

void CScrollBar_PlaceThumb(CScrollBar* self, u32 count) {
    nw4r::lyt::Pane* pane =
        self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DC0[self->mDirection], true);
    nw4r::math::VEC3 vec;
    // Place the drag thumb on the track: X is the thumb height, Y pinned so
    // the thumb stays inside the content as the scroll ratio advances.
    Cur_SetPaneTranslate(pane,
                  reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                      &vec.x, self->mThumbHeight,
                      -self->mScrollRatio * (f32)count + self->mContentHeight,
                      lbl_eu_80668138)));
}

/* Per-frame update dispatch helpers (defined below). */
__attribute__((noinline)) void CScrollBar_StepScrollIn(CScrollBar* self);
__attribute__((noinline)) void CScrollBar_StepScrollOut(CScrollBar* self);

/* Per-frame update dispatch: while the bar is ready, step the entering/leaving
animations and always call the layout's Animate. */
void CScrollBar_UpdateDispatch(CScrollBar* self) {
    if (self->mReady == 0) return;
    switch (self->mState) {
    case 1:
        CScrollBar_StepScrollIn(self);
        break;
    case 3:
        CScrollBar_StepScrollOut(self);
        break;
    }
    self->mLayout->Animate(0);
}

/* Entering state: advance the scroll-in animation offset by a frame step; once
it passes the threshold, finish the animation and become visible/idle. */
__attribute__((noinline)) void CScrollBar_StepScrollIn(CScrollBar* self) {
    float step = lbl_eu_80668150;
    self->mAnimOffset += step;
    if (self->mAnimOffset < lbl_eu_80668154) return;
    if (advanceAnimTransform(self->mAnimTransform, step)) {
        self->mState = 2;
        self->mActive = 1;
    }
}

/* Leaving/shutdown: when the scroll-out animation is done, hide the bar. */
__attribute__((noinline)) void CScrollBar_StepScrollOut(CScrollBar* self) {
    float frame = lbl_eu_80668150;
    if (AnimRewindFrame(self->mAnimTransform, frame)) {
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
        mtl::MemManager::setMemInitFlag(0);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, path + 0x21);
        buildLayout(&mLayout, mAccessor, path + 0x25);
        bindLayoutAnimTransform(mLayout, &mAnimTransform, mAccessor, path + 0x36);
        mLayout->SetAnimationEnable(mAnimTransform, true);
        mLayout->Animate(0);

        // Pull the thumb dimensions off the layout's drag pane.
        nw4r::lyt::Pane* pane =
            mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DC0[mDirection], true);
        float drag[2];
        TagCopyVec2f(drag, reinterpret_cast<float*>(reinterpret_cast<u8*>(pane) + 0x4C));
        nw4r::math::VEC3 dims =
            *reinterpret_cast<nw4r::math::VEC3*>(reinterpret_cast<u8*>(pane) + 0x2C);
        mScrollPosY = drag[1];
        mThumbHeight = dims.x;
        mContentHeight = dims.y;

        // Hide all six bar panes, then show only the active direction's thumb.
        for (u8 i = 0; i < 6; i++) {
            nw4r::lyt::Pane* hidePane =
            mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DA8[i], true);
            setPaneVisible(hidePane, 0);
        }
        nw4r::lyt::Pane* showPane =
            mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DA8[mDirection], true);
        setPaneVisible(showPane, 1);

        CScrollBar_MarkLayoutReady(this);
        mFileHandle = nullptr;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}

/* Read the scroll-bar layout arc; keep the file handle for the load event. */
void CScrollBar::loadLayoutArc() {
    u32 handle = mtl::MemManager::getHandleMEM2();
    mFileHandle = CDeviceFile::readFile(
        handle, lbl_eu_80507A4C, reinterpret_cast<IWorkEvent*>(this), 0, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);
}

/* Draw the layout once it is ready and the scroll bar is active. */
void CScrollBar::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (mReady != 0 && mState != 0) {
        drawLayout(mLayout, drawInfo, 0, 1);
    }
}

/* Teardown: release the file handle, destroy the layout/accessor/scratch region,
and reset the widget to its blank state. */
void CScrollBar_Teardown(CScrollBar* self) {
    closeFileHandle(&self->mFileHandle);
    self->mReady = 0;
    self->mAnimTransform = 0;
    self->mVisible = 0;
    if (self->mLayout != 0) {
        delete self->mLayout;
        self->mLayout = 0;
    }
    releaseArcResourceAccessor(self->mAccessor);
    self->mAccessor = 0;
    self->mMemRegion.deleteRegion();
}

/* Request scroll-in: if hidden, transition to entering and mark inactive. */
void CScrollBar::requestScrollIn() {
    if (mState == 0) {
        mState = 1;
        mActive = 0;
    }
}

/* Request scroll-out: if visible, transition to leaving and mark inactive. */
void CScrollBar::requestScrollOut() {
    if (mState == 2) {
        mState = 3;
        mActive = 0;
    }
}

void CScrollBar_UpdateThumb(CScrollBar* self, u32 scrollFrom, u32 scrollTo) {
    nw4r::lyt::Pane* pane =
        self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80534DC0[self->mDirection], true);
    CScrollBarPane* pdata = reinterpret_cast<CScrollBarPane*>(pane);
    s16 delta = (s16)(scrollTo - scrollFrom);
    if (delta <= 0) {
        // Bottom of the scroll range: park the thumb at the tail position.
        float tmp[2];
        TagCopyVec2f(tmp, pdata->mDrag);
        tmp[1] = self->mScrollPosY;
        writePanePos(pane, tmp);
        self->mScrollRatio = lbl_eu_80668138;
        nw4r::math::VEC3 vec;
        Cur_SetPaneTranslate(pane,
                      reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                          &vec.x, self->mThumbHeight, self->mContentHeight,
                          lbl_eu_80668138)));
        setPaneVisible(pane, 0);
    } else {
        // Map the current scroll position onto the thumb track; the stick
        // ratio is the fraction clamped through the lower bound.
        f32 fFrom = (f32)scrollFrom;
        f32 fTo = (f32)scrollTo;
        f32 ratio = self->mScrollPosY * (fFrom / fTo);
        if (ratio < lbl_eu_8066813C)
            ratio = lbl_eu_8066813C;
        float tmp[2];
        TagCopyVec2f(tmp, pdata->mDrag);
        tmp[1] = ratio;
        writePanePos(pane, tmp);
        self->mScrollRatio = (self->mScrollPosY - ratio) / (f32)delta;
        nw4r::math::VEC3 vec;
        Cur_SetPaneTranslate(pane,
                      reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                          &vec.x, self->mThumbHeight, self->mContentHeight,
                          lbl_eu_80668138)));
        setPaneVisible(pane, 1);
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
__attribute__((noinline)) void CScrollBar_MarkLayoutReady(CScrollBar* bar) {
    if (bar->mLayout != 0) {
        bar->mVisible = 1;
        bar->mReady = 1;
    }
}

// --- restored from git history (base:gone); do not expand beyond these functions ---
// from commit 733d067c6f5a needle=CScrollBar_isVisible

/* Retail symbol is unmangled CScrollBar_isVisible(void*); typed here. */
extern "C" u8 CScrollBar_isVisible(CScrollBar* self) { return self->mVisible; }

