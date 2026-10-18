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
// CScrollBar call sites; extern "C" stops MWCC from emitting __F... tails.
extern "C" void Cur_SetPaneTranslate(nw4r::lyt::Pane*, const nw4r::math::VEC3*);
extern "C" void writePanePos(nw4r::lyt::Pane*, float*);
void closeFileHandle(CFileHandle**);

// Retail data externs for the hand-built tables at the bottom of this TU.
// Free-function spellings name the exact retail (link) symbols; the .text
// never calls them (11/17 matched), they only feed ADDR32 relocs. The dtor
// and OnFileEvent name this TU's own members (member scope defines them).
// __RTTI__10IWorkEvent follows the CMCEffStart RTTIExterns workaround.
namespace RTTIExterns {
extern "C" const void* __RTTI__10IWorkEvent[];
}
extern "C" {
void __dt__10CScrollBarFv(void*, int); // ABI deleting dtor
void OnFileEvent__10CScrollBarFP10CEventFile(void);
void WorkEvent1__10IWorkEventFPvPCc(void);
void WorkEvent3__10IWorkEventFPv(void);
void WorkEvent4__10IWorkEventFv(void);
void OnPauseTrigger__10IWorkEventFb(void);
void WorkEvent6__10IWorkEventFv(void);
void WorkEvent7__10IWorkEventFv(void);
void WorkEvent8__10IWorkEventFv(void);
void WorkEvent9__10IWorkEventFv(void);
void WorkEvent10__10IWorkEventFv(void);
void WorkEvent11__10IWorkEventFv(void);
void WorkEvent12__10IWorkEventFv(void);
void WorkEvent13__10IWorkEventFv(void);
void WorkEvent14__10IWorkEventFv(void);
void WorkEvent15__10IWorkEventFv(void);
void WorkEvent16__10IWorkEventFv(void);
void WorkEvent17__10IWorkEventFv(void);
void WorkEvent18__10IWorkEventFv(void);
void WorkEvent19__10IWorkEventFv(void);
void WorkEvent20__10IWorkEventFv(void);
void WorkEvent21__10IWorkEventFv(void);
void WorkEvent22__10IWorkEventFv(void);
void WorkEvent23__10IWorkEventFv(void);
void WorkEvent24__10IWorkEventFv(void);
void WorkEvent25__10IWorkEventFv(void);
void WorkEvent26__10IWorkEventFv(void);
void WorkEvent27__10IWorkEventFv(void);
void WorkEvent28__10IWorkEventFv(void);
void WorkEvent29__10IWorkEventFv(void);
void WorkEvent30__10IWorkEventFv(void);
void WorkEvent31__10IWorkEventFv(void);
}
extern void* lbl_eu_80534E60[]; // .data RTTI base, defined below
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

// .sdata2 retail table (splits: 80668138-80668158). Declaring the six
// objects here -- before any code -- emits them in retail declaration order at
// the retail offsets (+0x0 zero, +0x4 4.0, +0x8/+0x10 the 2^52 / 2^52+2^31
// debias doubles, +0x18 1.0, +0x1c 5.0) so all six retail labels are defined
// at their retail offsets. The code keeps the retail lfs schedule: MWCC may
// route a const use through its own @N code pool, but the instruction stream
// is byte-identical to the previous EOF-placement build (verified).
// RESIDUAL (w4 probed, .scratch/dlabels7/w4/probe/): MWCC still emits its own
// code-const pool (four float copies + u32/s16 debias doubles) after the six
// objects, so the plain section is 0x48, not retail 0x20. Probe P1 proves
// hidden-value `extern const` (no init) emits hoisted NAMED loads on the
// exact retail schedule with no pool; probe P2 proves any visible/non-const
// spelling sinks the loads and breaks the prologue interleave (StepScrollIn
// fell to 68%, UpdateThumb to 36.3% when tried). Same-TU definitions always
// expose their values, so the hoisted+named combination is unreachable in
// source: reaching OK needs either the union conversion rewrite + a load
// schedule MWCC will not produce, or post-link help. Still pooling: 0.0f /
// 4.0f / 1.0f / 5.0f copies plus the 2^52 and 2^52+2^31 debias doubles.
// The rule keeps retargeting the 3 magic loads onto lbl_eu_80668140/-48.
#pragma section data_type ".sdata2" ".sdata2"
extern __declspec(section ".sdata2") const float lbl_eu_80668138 = 0.0f;
extern __declspec(section ".sdata2") const float lbl_eu_8066813C = 4.0f;
extern __declspec(section ".sdata2") const double lbl_eu_80668140 = 4503599627370496.0;
extern __declspec(section ".sdata2") const double lbl_eu_80668148 = 4503601774854144.0;
extern __declspec(section ".sdata2") const float lbl_eu_80668150 = 1.0f;
extern __declspec(section ".sdata2") const float lbl_eu_80668154 = 5.0f;

u8 CScrollBar::isVisible() { return mVisible; }


u8 CScrollBar::isActive() { return mActive; }

// Mark the widget ready once its layout has been built.
// Single-arg call: retail leaves r4 untouched; cast keeps the same
// unmangled reloc (C forbids arity overloads on extern "C").
extern "C" void CScrollBar_InitRootPane(CScrollBar* self) {
    ((void (*)(nw4r::lyt::Pane*))Cur_SetPaneTranslate)(self->mLayout->GetRootPane());
}

extern "C" void CScrollBar_PlaceThumb(CScrollBar* self, u32 count) {
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
extern "C" void CScrollBar_UpdateDispatch(CScrollBar* self) {
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
        TagCopyVec2f(drag, reinterpret_cast<CScrollBarPane*>(pane)->mDrag);
        nw4r::math::VEC3 dims = reinterpret_cast<CScrollBarPane*>(pane)->mDims;
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
extern "C" void CScrollBar_Teardown(CScrollBar* self) {
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

extern "C" void CScrollBar_UpdateThumb(CScrollBar* self, u32 scrollFrom, u32 scrollTo) {
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


// --- hand-built retail data (splits: .data 80534DA8-80534E70,
// .rodata 80507998-80507B3C, .sdata 806626F8-80662700, .sdata2 80668138-58) ---
// .rodata pane-name strings (each sized content+retail pad, MWCC packs them
// in order; the .data tables below take their addresses).
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507998[0x0C] = "nul_scr00";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_805079A4[0x0C] = "nul_scr01";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_805079B0[0x0C] = "nul_scr02";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_805079BC[0x0C] = "nul_scr03";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_805079C8[0x0C] = "nul_scr04";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_805079D4[0x0C] = "nul_scr05";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_805079E0[] = "pic_scr00_bar";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_805079F0[] = "pic_scr01_bar";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507A00[] = "pic_scr02_bar";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507A10[] = "pic_scr03_bar";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507A20[] = "pic_scr04_bar";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507A30[] = "pic_scr05_bar";
// RTTI class-name string (referenced by the .sdata pair below).
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507A40[0x0C] = "CScrollBar";
// Arc/path table read by OnFileEvent/loadLayoutArc as path+0x16/+0x21/+0x25/+0x36.
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507A4C[0x54] = "menu/jp/ScrollBar.arc\0CScrollBar\0arc\0mf00_scr00.brlyt\0mf00_scr00_info_in.brlan\0";
// bdat column-name strings (cross-unit readers; sizes content+retail pad).
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507AA0[] = "r1_quest_FLG";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507AB0[] = "r1_quest_STFLG";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507AC0[] = "r1_S_FLG_MIN";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507AD0[] = "r1_S_FLG_MAX";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507AE0[0x0C] = "r1_gimID";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507AEC[] = "r1_named_FLG";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507AFC[] = "r1_item_lost";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507B0C[0x0C] = "s1pcwait";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507B18[0x0C] = "s1LODtype";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507B24[0x0C] = "s1LODsub";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80507B30[0x0C] = "s1LODSTOP";
// .sdata RTTI pair (classname + RTTI base), referenced by the vtable head.
__declspec(section ".sdata") __attribute__((used, aligned(8)))
void* lbl_eu_806626F8[2] = { lbl_eu_80507A40, lbl_eu_80534E60 };
// .data pane-pointer tables (nul names / pic names).
__declspec(section ".data") __attribute__((used, aligned(4)))
const char* lbl_eu_80534DA8[6] = {
    lbl_eu_80507998, lbl_eu_805079A4, lbl_eu_805079B0,
    lbl_eu_805079BC, lbl_eu_805079C8, lbl_eu_805079D4,
};
__declspec(section ".data") __attribute__((used, aligned(4)))
const char* lbl_eu_80534DC0[6] = {
    lbl_eu_805079E0, lbl_eu_805079F0, lbl_eu_80507A00,
    lbl_eu_80507A10, lbl_eu_80507A20, lbl_eu_80507A30,
};
// .data vtable (RTTI, null, dtor, IWorkEvent/CScrollBar virtuals).
__declspec(section ".data") __attribute__((used, aligned(8)))
void* lbl_eu_80534DD8[34] = {
    lbl_eu_806626F8, 0, (void*)__dt__10CScrollBarFv,
    (void*)WorkEvent1__10IWorkEventFPvPCc,
    (void*)OnFileEvent__10CScrollBarFP10CEventFile,
    (void*)WorkEvent3__10IWorkEventFPv, (void*)WorkEvent4__10IWorkEventFv,
    (void*)OnPauseTrigger__10IWorkEventFb, (void*)WorkEvent6__10IWorkEventFv,
    (void*)WorkEvent7__10IWorkEventFv, (void*)WorkEvent8__10IWorkEventFv,
    (void*)WorkEvent9__10IWorkEventFv, (void*)WorkEvent10__10IWorkEventFv,
    (void*)WorkEvent11__10IWorkEventFv, (void*)WorkEvent12__10IWorkEventFv,
    (void*)WorkEvent13__10IWorkEventFv, (void*)WorkEvent14__10IWorkEventFv,
    (void*)WorkEvent15__10IWorkEventFv, (void*)WorkEvent16__10IWorkEventFv,
    (void*)WorkEvent17__10IWorkEventFv, (void*)WorkEvent18__10IWorkEventFv,
    (void*)WorkEvent19__10IWorkEventFv, (void*)WorkEvent20__10IWorkEventFv,
    (void*)WorkEvent21__10IWorkEventFv, (void*)WorkEvent22__10IWorkEventFv,
    (void*)WorkEvent23__10IWorkEventFv, (void*)WorkEvent24__10IWorkEventFv,
    (void*)WorkEvent25__10IWorkEventFv, (void*)WorkEvent26__10IWorkEventFv,
    (void*)WorkEvent27__10IWorkEventFv, (void*)WorkEvent28__10IWorkEventFv,
    (void*)WorkEvent29__10IWorkEventFv, (void*)WorkEvent30__10IWorkEventFv,
    (void*)WorkEvent31__10IWorkEventFv,
};
// .data RTTI base (IWorkEvent typeinfo + zeros; the +0xB8 __RTTI reloc).
__declspec(section ".data") __attribute__((used, aligned(4)))
void* lbl_eu_80534E60[4] = { (void*)RTTIExterns::__RTTI__10IWorkEvent, 0, 0, 0 };
// .sdata2 retail table now lives just after the CScrollBarPane definition
// above (retail order and offsets; pool-size residual documented there).
