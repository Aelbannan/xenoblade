// Inline-empty ~IWorkEvent: the ctor's cleanup action references the (link-deduped)
// weak copy, and ~CScrollBar elides the base call (retail shape; see CBattery).
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CScrollBar.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"

#include <nw4r/lyt.h>
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Cross-TU layout helpers (kyoshin .text). Retail keeps these unmangled at
// CScrollBar call sites; extern "C" stops MWCC from emitting __F... tails.
extern "C" void Cur_SetPaneTranslate(nw4r::lyt::Pane*, const nw4r::math::VEC3*);
extern "C" void writePanePos(nw4r::lyt::Pane*, float*);
// Retail code80135FDC_setVec3 leaves its first arg (a pointer) in r3, so
// callers (UpdateThumb) reuse it as the returned pointer for Cur_SetPaneTranslate.

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

// Pane-name tables: sBarPaneNames[] are the 6 scroll-position/bar panes hidden
// while idle (sBarPaneNames[mDirection] is re-shown); sThumbPaneNames[mDirection]
// is the drag thumb.
static const char* sBarPaneNames[6] = {
    "nul_scr00", "nul_scr01", "nul_scr02", "nul_scr03", "nul_scr04", "nul_scr05",
};
static const char* sThumbPaneNames[6] = {
    "pic_scr00_bar", "pic_scr01_bar", "pic_scr02_bar",
    "pic_scr03_bar", "pic_scr04_bar", "pic_scr05_bar",
};

// .sdata2 retail table (splits: 80668138-80668158), declared before any code so
// the six labels land at their retail offsets. RESIDUAL: MWCC still emits its
// own code-const pool after them (decomp .sdata2 0x40 vs retail 0x20), which
// leaves the float-load relocs of the ctor/UpdateThumb/PlaceThumb/StepScroll*
// on pool symbols (see .scratch/dlabels7/w4 probe notes in git history).
#pragma section data_type ".sdata2" ".sdata2"
extern __declspec(section ".sdata2") const float lbl_eu_80668138 = 0.0f;
extern __declspec(section ".sdata2") const float lbl_eu_8066813C = 4.0f;
extern __declspec(section ".sdata2") const double lbl_eu_80668140 = 4503599627370496.0;
extern __declspec(section ".sdata2") const double lbl_eu_80668148 = 4503601774854144.0;
extern __declspec(section ".sdata2") const float lbl_eu_80668150 = 1.0f;
extern __declspec(section ".sdata2") const float lbl_eu_80668154 = 5.0f;
// Restore the default so the compiler RTTI base list stays in .data.
#pragma section data_type ".data" ".bss"

CScrollBar::CScrollBar(u8 direction) : mMemRegion() {
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

CScrollBar::~CScrollBar() {}

/* Read the scroll-bar layout arc; keep the file handle for the load event. */
void CScrollBar::loadLayoutArc() {
    u32 handle = mtl::MemManager::getHandleMEM2();
    mFileHandle = CDeviceFile::readFile(handle, "menu/jp/ScrollBar.arc", this, 0, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);
}

/* Per-frame update dispatch: while the bar is ready, step the entering/leaving
animations and always call the layout's Animate. */
void CScrollBar::UpdateDispatch() {
    if (mReady == 0) return;
    switch (mState) {
    case 1:
        StepScrollIn();
        break;
    case 3:
        StepScrollOut();
        break;
    }
    mLayout->Animate(0);
}

/* Draw the layout once it is ready and the scroll bar is active. */
void CScrollBar::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (mReady != 0 && mState != 0) {
        drawLayout(mLayout, drawInfo, 0, 1);
    }
}

/* Teardown: release the file handle, destroy the layout/accessor/scratch region,
and reset the widget to its blank state. */
void CScrollBar::Teardown() {
    closeFileHandle(&mFileHandle);
    mReady = 0;
    mAnimTransform = 0;
    mVisible = 0;
    if (mLayout != 0) {
        delete mLayout;
        mLayout = 0;
    }
    releaseArcResourceAccessor(mAccessor);
    mAccessor = 0;
    mMemRegion.deleteRegion();
}

int CScrollBar::isVisible() { return mVisible; }

int CScrollBar::isActive() { return mActive; }

void CScrollBar::InitRootPane(const nw4r::math::VEC3* pos) {
    Cur_SetPaneTranslate(mLayout->GetRootPane(), pos);
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

void CScrollBar::UpdateThumb(u32 scrollFrom, u32 scrollTo) {
    nw4r::lyt::Pane* pane =
        mLayout->GetRootPane()->FindPaneByName(sThumbPaneNames[mDirection], true);
    CScrollBarPane* pdata = reinterpret_cast<CScrollBarPane*>(pane);
    s16 delta = (s16)(scrollTo - scrollFrom);
    if (delta <= 0) {
        // Bottom of the scroll range: park the thumb at the tail position.
        float tmp[2];
        TagCopyVec2f(tmp, pdata->mDrag);
        tmp[1] = mScrollPosY;
        writePanePos(pane, tmp);
        mScrollRatio = lbl_eu_80668138;
        nw4r::math::VEC3 vec;
        Cur_SetPaneTranslate(pane,
                      reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                          (nw4r::math::VEC3*)&vec.x, mThumbHeight, mContentHeight,
                          lbl_eu_80668138)));
        setPaneVisible(pane, 0);
    } else {
        // Map the current scroll position onto the thumb track; the stick
        // ratio is the fraction clamped through the lower bound.
        f32 fFrom = (f32)scrollFrom;
        f32 fTo = (f32)scrollTo;
        f32 ratio = mScrollPosY * (fFrom / fTo);
        if (ratio < lbl_eu_8066813C)
            ratio = lbl_eu_8066813C;
        float tmp[2];
        TagCopyVec2f(tmp, pdata->mDrag);
        tmp[1] = ratio;
        writePanePos(pane, tmp);
        mScrollRatio = (mScrollPosY - ratio) / (f32)delta;
        nw4r::math::VEC3 vec;
        Cur_SetPaneTranslate(pane,
                      reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                          (nw4r::math::VEC3*)&vec.x, mThumbHeight, mContentHeight,
                          lbl_eu_80668138)));
        setPaneVisible(pane, 1);
    }
}

void CScrollBar::PlaceThumb(u16 count) {
    nw4r::lyt::Pane* pane =
        mLayout->GetRootPane()->FindPaneByName(sThumbPaneNames[mDirection], true);
    nw4r::math::VEC3 vec;
    // Place the drag thumb on the track: X is the thumb height, Y pinned so
    // the thumb stays inside the content as the scroll ratio advances.
    Cur_SetPaneTranslate(pane,
                  reinterpret_cast<nw4r::math::VEC3*>(code80135FDC_setVec3(
                      (nw4r::math::VEC3*)&vec.x, mThumbHeight,
                      -mScrollRatio * (f32)count + mContentHeight,
                      lbl_eu_80668138)));
}

/* Entering state: advance the scroll-in animation offset by a frame step; once
it passes the threshold, finish the animation and become visible/idle. */
void CScrollBar::StepScrollIn() {
    float step = lbl_eu_80668150;
    mAnimOffset += step;
    if (mAnimOffset < lbl_eu_80668154) return;
    if (advanceAnimTransform(mAnimTransform, step)) {
        mState = 2;
        mActive = 1;
    }
}

/* Leaving/shutdown: when the scroll-out animation is done, hide the bar. */
void CScrollBar::StepScrollOut() {
    float frame = lbl_eu_80668150;
    if (AnimRewindFrame(mAnimTransform, frame)) {
        mState = 0;
        mActive = 1;
        mAnimOffset = lbl_eu_80668138;
    }
}

// Layout built successfully: mark the bar ready and visible.
void CScrollBar::MarkLayoutReady() {
    if (mLayout != 0) {
        mVisible = 1;
        mReady = 1;
    }
}

// Retail OnFileEvent uses the _savegpr_28/_restgpr_28 helper pair
// (size-mode save shape); this TU compiles with -O4,s via configure.py
// extra_cflags rather than a local #pragma.
bool CScrollBar::OnFileEvent(CEventFile* pEventFile) {
    // Only build the layout when the event's file handle matches our own.
    if (mFileHandle == pEventFile->mFileHandle) {
        u32 mem2 = mtl::MemManager::getHandleMEM2();
                // Open a scratch region for layout allocation behind this event.
        mMemRegion.createRegion(mem2, 0x3000, "CScrollBar", 1);
        Class_8045F858 sp8 = Class_8045F858(&mMemRegion);
        void* data = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(0);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, "arc");
        buildLayout(&mLayout, mAccessor, "mf00_scr00.brlyt");
        bindLayoutAnimTransform(mLayout, &mAnimTransform, mAccessor, "mf00_scr00_info_in.brlan");
        mLayout->SetAnimationEnable(mAnimTransform, true);
        mLayout->Animate(0);

        // Pull the thumb dimensions off the layout's drag pane.
        nw4r::lyt::Pane* pane =
            mLayout->GetRootPane()->FindPaneByName(sThumbPaneNames[mDirection], true);
        float drag[2];
        TagCopyVec2f(drag, reinterpret_cast<CScrollBarPane*>(pane)->mDrag);
        nw4r::math::VEC3 dims = reinterpret_cast<CScrollBarPane*>(pane)->mDims;
        mScrollPosY = drag[1];
        mThumbHeight = dims.x;
        mContentHeight = dims.y;

        // Hide all six bar panes, then show only the active direction's thumb.
        for (u8 i = 0; i < 6; i++) {
            nw4r::lyt::Pane* hidePane =
            mLayout->GetRootPane()->FindPaneByName(sBarPaneNames[i], true);
            setPaneVisible(hidePane, 0);
        }
        nw4r::lyt::Pane* showPane =
            mLayout->GetRootPane()->FindPaneByName(sBarPaneNames[mDirection], true);
        setPaneVisible(showPane, 1);

        MarkLayoutReady();
        mFileHandle = nullptr;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}
