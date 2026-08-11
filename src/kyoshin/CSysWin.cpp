// Auto-scaffolded catalog TU for kyoshin/CSysWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CTagProcessor.hpp"

#include "kyoshin/code_80135FDC.hpp"

#include "monolib/device.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <new>

extern "C" u8 CSysWin_isReady(CSysWinFull* self) { return self->field_28; }


extern "C" u8 CSysWin_getUnk34(CSysWinFull* self) { return self->field_34; }

extern "C" u8 CSysWin_isActive(CSysWinFull* self) { return self->field_36; }

// nw4r::lyt::Pane::mSize (2 floats at +0x4C) is protected; overlay struct for
// the label-pane size reads in func_8022B9B4 (offset == absolute pane offset).
struct CSysWinPaneOverlay {
    u8 _pad[0x4C];
    f32 size[2];  // +0x4C - Pane::mSize (width, height)
};

// Target 5: us-8022d450 - CSysWin constructor (US retail short C-linkage
// symbol __ct__CSysWin; the class declares no virtuals so MWCC never emits a
// __vt__7CSysWin of its own). Stores the retail vtable BEFORE the embedded
// UnkClass_8045F564 is constructed (retail ctor order), then zero/one-inits
// the state fields and returns `this` in r3. optimize_for_size matches the
// retail stmw/lmw prologue (MWCC_REFERENCE #16); the UnkClass ctor is invoked
// via its C-linkage symbol so no placement-new null check is emitted.
#pragma push
#pragma optimize_for_size on
extern "C" CSysWin* __ct__CSysWin(CSysWin* self, int arg) {
    self->mVtbl = lbl_eu_80536510;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    self->mFileHandle = 0;
    self->mTagProcessor = 0;
    self->mArcAccessor = 0;
    self->mLayout = 0;
    self->mAnimTrans = 0;
    self->field_28 = 1;
    self->field_2C = 0;
    self->field_30 = 0;
    self->field_34 = 0;
    self->field_35 = 0;
    self->field_36 = 1;
    self->field_37 = (u8)arg;
    self->field_38 = 5;
    self->field_39 = 0;
    return self;
}
#pragma pop

// Target 5 (prior): us-8022d4d8 - complete-object destructor.
// Written as the C-linkage free function the retail symbol names: null-check,
// destroy the embedded UnkClass_8045F564 scratch region, then conditional
// operator delete when flags > 0.
extern "C" CSysWin* __dt__7CSysWinFv(CSysWin* _this, int flags) {
    if (_this != 0) {
        __dt__17UnkClass_8045F564Fv(&_this->mMemRegion, -1);
        int f = flags;
        if (f > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// TEMP PROBE: two-register function to test save order codegen.
extern "C" void probe_2reg(CSysWin* self, int kind) {
    func_80137038(self->mLayout, 0, kind, 1);
    func_80137038(self->mLayout, 0, kind, 2);
}

// Target 3: us-8022de64 - set the two window label texts from the shared
// string pool (offsets +0x77 / +0x82) into the layout. optimize_for_size
// matches the retail stmw/lmw prologue for the 3 callee-saved regs (r29-r31)
// under the unit's -O4,p flags (see MWCC_REFERENCE #16).
#pragma push
#pragma optimize_for_size on
extern "C" void func_8022BF6C(CSysWin* self, char* a, char* b) {
    func_80136B4C(self->mLayout, lbl_eu_8050A478 + 0x77, a, 0);
    func_80136B4C(self->mLayout, lbl_eu_8050A478 + 0x82, b, 0);
}
#pragma pop

// Stub for us-8022dec0 (func_8022BFC8) - declared in CSysWin.hpp (C-linkage).
// No definition here: an empty body would be inlined into callers and drop the
// call; the matcher for us-8022dec0 will provide the real body.

// Target 1: us-8022e0ac - format a pane name (idx+1), find two panes in the
// second window's layout, and compute the second pane's absolute screen
// position into out (ancestor translate sum via func_80137924). Same shape as
// func_8022C930 / func_80253970: each GetRootPane() is a fresh load (retail
// reloads window->mLayout per use). optimize_for_size matches the retail
// stmw/lmw prologue for the 4 callee-saved regs (r28-r31).
#pragma push
#pragma optimize_for_size on
extern "C" void func_8022C1B4(nw4r::math::VEC3* out, CSysWin* window, int idx) {
    char buf[0x28];
    sprintf(buf, &lbl_eu_8050A478[0xde], idx + 1);
    nw4r::lyt::Pane* pane1 = window->mLayout->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = window->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0xeb], true);
    func_80137924(out, pane1, pane2, window->mLayout->GetRootPane());
}
#pragma pop

// Target 3: us-8022e150 - advance the window animation; mark it active (phase 2)
// when the anim transform consumed the step.
#pragma dont_inline on
extern "C" void func_8022C258(CSysWin* self) {
    const f32 duration = lbl_eu_806685F0;
    if (func_80137444(self->mAnimTrans, duration) != 0) {
        self->field_35 = 2;
        self->field_36 = 1;
    }
}
#pragma dont_inline off

// Target 2: us-8022e19c - advance the window when the game-manager's active
// kind differs from the window's current kind (window kind must be < 5).
#pragma dont_inline on
extern "C" void func_8022C2A4(CSysWin* self) {
    if (self->field_38 >= 5) return;
    if (self->field_39 == (u32)func_80086F9C__Q22cf13CfGameManagerFv(-1)) return;
    func_8022BFC8(self, self->field_38);
}
#pragma dont_inline off

// Target 4: us-8022e1f0 - rewind the window animation; clear the open flag
// (phase 0) once the reverse anim finishes.
#pragma dont_inline on
extern "C" void func_8022C2F8(CSysWin* self) {
    const f32 duration = lbl_eu_806685F0;
    if (func_80137510(self->mAnimTrans, duration) != 0) {
        self->field_35 = 0;
        self->field_36 = 1;
        self->field_34 = 0;
    }
}
#pragma dont_inline off

// Target 1: us-8022e240 - layout/kind setup after a file load (called by
// OnFileEvent once the System window layout is built): find the two label
// panes, write all six label texts, bind the tag processor into each label
// pane, then re-apply the current kind's pane visibility via func_8022B90C.
// optimize_for_size matches the retail stmw/lmw prologue + shared string
// base for the 2 callee-saved regs (r30-r31) under the unit's -O4,p flags.
#pragma push
#pragma optimize_for_size on
extern "C" void func_8022C348(CSysWin* self) {
    if (self->mLayout == 0) return;
    self->field_28 = 1;
    self->field_2C = reinterpret_cast<u32>(
        self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0xfa], true));
    self->field_30 = reinterpret_cast<u32>(
        self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x106], true));
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x1c], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x29], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x3a], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x4a], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x57], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x67], &lbl_eu_8050A478[0x39], 0);
    // TextBox::mpTagProcessor at +0xF8 (MonolithSoft nw4r extension).
    struct TextBoxLayout {
        u8 _[0xF8];
        void* mpTagProcessor;
    };
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x1c], true));
        void* tag = self->mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x29], true));
        void* tag = self->mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x3a], true));
        void* tag = self->mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x4a], true));
        void* tag = self->mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x57], true));
        void* tag = self->mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x67], true));
        void* tag = self->mTagProcessor;
        t->mpTagProcessor = tag;
    }
    func_8022B90C(self, self->field_37);
}
#pragma pop

// Target 4: us-8022e43c - build the System window layout from the loaded arc
// file (the mFileHandle match gate). Mirrors the CSelShopWin load path: mem
// region, tag processor, resource accessor, layout + anim transform, font
// binding, then hand off to func_8022C348 and release the handle/region. The
// Class_8045F858 stack guard destructs at scope end (after func_8045F810).
// optimize_for_size matches the retail stmw/lmw prologue (r29-r31).
#pragma push
#pragma optimize_for_size on
bool CSysWin::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        u32 handle = mtl::MemManager::getHandleMEM2();
    mMemRegion.createRegion(handle, 0x3000, &lbl_eu_8050A478[0x10f], 1);
    Class_8045F858 regionGuard(&mMemRegion);
    u8* archive = reinterpret_cast<u8*>(mFileHandle->getData());
    mtl::MemManager::func_80434A4C(false);
    CTagProcessor* tag = (CTagProcessor*)mtl::MemManager::allocate(
        0x858, getAllocHandle__10CLibLayoutFv());
    if (tag != 0) {
        __ct__CTagProcessor(tag);
    }
    mTagProcessor = tag;
    mArcAccessor = createArcResourceAccessor__10CLibLayoutFv();
    mArcAccessor->Attach(archive, &lbl_eu_8050A478[0x117]);
    func_80136E84(&mLayout, mArcAccessor, &lbl_eu_8050A478[0x11b]);
    func_80136F08(mLayout, &mAnimTrans, mArcAccessor, &lbl_eu_8050A478[0x134]);
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    u8* font = (u8*)CDeviceFont::func_80452C10(1, mLayout);
    typedef u32 (*FontVFn)(void*);
    u32 fontRes = (*reinterpret_cast<FontVFn**>(font))[0x24 / 4](font);
    func_8013676C(rootPane, fontRes);
    mLayout->SetAnimationEnable(mAnimTrans, true);
    mLayout->Animate(0);
        func_8022C348(this);
        mFileHandle = 0;
        mMemRegion.func_8045F810();
        return true;
    }
    return false;
}
#pragma pop

// Target 1: us-8022d52c - load the System.arc file (IWorkEvent callback = this)
// into the window's file-handle slot, mark the handle, and clear the ready flag.
extern "C" void func_8022B6F4(CSysWin* self) {
    u32 allocHandle = mtl::MemManager::getHandleMEM2();
    self->mFileHandle = CDeviceFile::readFile(
        allocHandle, lbl_eu_8050A478, reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::setHandleFlag1(self->mFileHandle);
    self->field_28 = 0;
}

// Target 4: us-8022d580 - per-frame update: run the phase handler for the
// current animation phase, then advance the layout animation (open-flag gate).
// The three phase handlers carry dont_inline so the dispatch calls stay direct
// bl's. Goto-chain dispatch (MWCC_REFERENCE §7d2): switch/if-else shapes don't
// reproduce retail's linear equality chain with bodies appended after the
// tests; the int temp keeps the compares signed (cmpi) without an extsb.
extern "C" void func_8022B748(CSysWin* self) {
    if (self->field_34 != 0) {
        int phase = self->field_35;
        if (phase == 1) goto case1;
        if (phase == 2) goto case2;
        if (phase == 3) goto case3;
        goto tail;
    case1:
        func_8022C258(self);
        goto tail;
    case2:
        func_8022C2A4(self);
        goto tail;
    case3:
        func_8022C2F8(self);
    tail:
        self->mLayout->Animate(0);
    }
}

// Target 1: us-8022d600 - draw the system-window layout when it is open and
// animated (field_34/field_35 guards), passing through the caller's DrawInfo.
extern "C" void func_8022B7C8(CSysWin* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_34 == 0) return;
    if (self->field_35 == 0) return;
    return func_80137038(self->mLayout, drawInfo, 0, 1);
}

// Target 3: us-8022d62c - tear down the window layout and free its resources.
// The `if (x != 0) { delete x; x = 0; }` shape reproduces retail's double-beq
// prologue check (same as CBattery::func_802B9364).
#pragma push
#pragma optimize_for_size on
extern "C" void func_8022B7F4(CSysWin* self) {
    func_801390E0(&self->mFileHandle);
    nw4r::lyt::Layout* layout = self->mLayout;
    self->field_34 = 0;
    self->mAnimTrans = 0;
    if (layout != 0) {
        delete layout;
        self->mLayout = 0;
    }
    func_80139124(self->mArcAccessor);
    self->mArcAccessor = 0;
    if (self->mTagProcessor != 0) {
        delete self->mTagProcessor;
        self->mTagProcessor = 0;
    }
    self->mMemRegion.func_8045F778();
}
#pragma pop

// Target 2: us-8022d6f0 - open the window (only when idle): mark phase 1 and
// play the opening UI sound.
extern "C" void func_8022B8B8(CSysWin* self) {
    if (self->field_35 != 0) return;
    self->field_35 = 1;
    self->field_36 = 0;
    self->field_34 = 1;
    return func_80138078(0xd);
}

// Target 2: us-8022d744 - switch the window's visible pane for the given kind:
// hide both panes first, then re-show the one selected by the 4-way switch
// (each case keeps its own call site in retail - no fall-through merging).
// optimize_for_size matches the retail stmw/lmw prologue (r30-r31).
#pragma push
#pragma optimize_for_size on
extern "C" void func_8022B90C(CSysWin* self, int kind) {
    func_80124270(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C), 0);
    func_80124270(reinterpret_cast<nw4r::lyt::Pane*>(self->field_30), 0);
    self->field_37 = (u8)kind;
    switch (kind) {
    case 0:
        func_80124270(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C), 1);
        break;
    case 1:
        func_80124270(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C), 1);
        break;
    case 2:
        func_80124270(reinterpret_cast<nw4r::lyt::Pane*>(self->field_30), 1);
        break;
    case 3:
        func_80124270(reinterpret_cast<nw4r::lyt::Pane*>(self->field_30), 1);
        break;
    }
}
#pragma pop
// Target 2: us-8022d7ec - set the system-window content for one of the four
// window kinds. Finds the shared label pane once, then per kind: writes the
// label texts (cases 0/2/3 use the incoming strings), binds the tag processor
// into the label pane(s), sizes the icon pane from the label pane's height
// (threshold ladder), and repositions the icon pane.
extern "C" void func_8022B9B4(CSysWin* self, char* textA, char* textB) {
    nw4r::lyt::Pane* pane0 =
        self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x13], true);
    // Goto-chain dispatch (cf. func_8022B748): retail's linear equality chain
    // with case bodies appended after the tests; the u8 loads as an int so the
    // compares stay plain cmpwi without an extsb.
    int kind = self->field_37;
    if (kind == 0) goto case0;
    if (kind == 1) goto case1;
    if (kind == 2) goto case2;
    if (kind == 3) goto case3;
    goto end;
case0:
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x1c], textA,
                  reinterpret_cast<u32>(self->mTagProcessor));
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x29], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x3a], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x4a], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x57], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x67], &lbl_eu_8050A478[0x39], 0);
    {
        nw4r::lyt::Pane* pane =
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x1c], true);
        func_80127BF4(self->mTagProcessor, pane, pane0);
        f32 size[2];
        func_80127BC4(size, reinterpret_cast<CSysWinPaneOverlay*>(pane0)->size);
        f32 h = size[1];
        if (h >= lbl_eu_80668598) {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685A0, lbl_eu_8066859C));
        } else if (h >= lbl_eu_806685A4) {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685A8, lbl_eu_8066859C));
        } else if (h >= lbl_eu_806685AC) {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685B0, lbl_eu_8066859C));
        } else if (h >= lbl_eu_806685B4) {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685B8, lbl_eu_8066859C));
        } else if (h >= lbl_eu_806685BC) {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685C0, lbl_eu_8066859C));
        } else {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685C4, lbl_eu_8066859C));
        }
    }
    goto end;
case1:
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x29], textA,
                  reinterpret_cast<u32>(self->mTagProcessor));
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x3a], textB,
                  reinterpret_cast<u32>(self->mTagProcessor));
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x1c], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x4a], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x57], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x67], &lbl_eu_8050A478[0x39], 0);
    {
        nw4r::lyt::Pane* paneA =
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x3a], true);
        func_80127BF4(self->mTagProcessor, paneA, pane0);
        f32 posA[2];
        func_80127BC4(posA, reinterpret_cast<CSysWinPaneOverlay*>(pane0)->size);
        nw4r::lyt::Pane* paneB =
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x29], true);
        func_80127BF4(self->mTagProcessor, paneB, pane0);
        f32 posB[2];
        func_80127BC4(posB, reinterpret_cast<CSysWinPaneOverlay*>(pane0)->size);
        f32 pick[2];
        func_80127BC4(pick, posA[0] > posB[0] ? posA : posB);
        pick[1] = lbl_eu_806685BC;
        func_80124288(pane0, pick);
        nw4r::math::VEC3 v;
        func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_2C),
                      code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685C0, lbl_eu_8066859C));
    }
    goto end;
case2:
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x4a], textA,
                  reinterpret_cast<u32>(self->mTagProcessor));
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x1c], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x29], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x3a], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x57], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x67], &lbl_eu_8050A478[0x39], 0);
    {
        nw4r::lyt::Pane* pane =
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x4a], true);
        func_80127BF4(self->mTagProcessor, pane, pane0);
        f32 size[2];
        func_80127BC4(size, reinterpret_cast<CSysWinPaneOverlay*>(pane0)->size);
        f32 h = size[1];
        if (h >= lbl_eu_806685AC) {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_30),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685C8, lbl_eu_8066859C));
        } else if (h >= lbl_eu_806685B4) {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_30),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685CC, lbl_eu_8066859C));
        } else if (h >= lbl_eu_806685BC) {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_30),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685D0, lbl_eu_8066859C));
        } else {
            nw4r::math::VEC3 v;
            func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_30),
                          code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685D4, lbl_eu_8066859C));
        }
    }
    goto end;
case3:
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x57], textA,
                  reinterpret_cast<u32>(self->mTagProcessor));
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x67], textB,
                  reinterpret_cast<u32>(self->mTagProcessor));
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x1c], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x29], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x3a], &lbl_eu_8050A478[0x39], 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0x4a], &lbl_eu_8050A478[0x39], 0);
    {
        nw4r::lyt::Pane* paneA =
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x67], true);
        func_80127BF4(self->mTagProcessor, paneA, pane0);
        f32 posA[2];
        func_80127BC4(posA, reinterpret_cast<CSysWinPaneOverlay*>(pane0)->size);
        nw4r::lyt::Pane* paneB =
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0x57], true);
        func_80127BF4(self->mTagProcessor, paneB, pane0);
        f32 posB[2];
        func_80127BC4(posB, reinterpret_cast<CSysWinPaneOverlay*>(pane0)->size);
        f32 pick[2];
        func_80127BC4(pick, posA[0] > posB[0] ? posA : posB);
        pick[1] = lbl_eu_806685D8;
        func_80124288(pane0, pick);
        nw4r::math::VEC3 v;
        func_801D2150(reinterpret_cast<nw4r::lyt::Pane*>(self->field_30),
                      code80135FDC_setVec3(&v.x, lbl_eu_8066859C, lbl_eu_806685D4, lbl_eu_8066859C));
    }
end:
    ;
}

// Target 5: us-8022dec0 - window-kind advance: pick the label/message set for
// the kind, set both window label texts, then bind the icon texture (from the
// arc 'timg' resource) and size the two icon panes to the texture dims. The
// 5-byte label index table is built from the two .sdata2 globals (word + byte).
#pragma dont_inline on
extern "C" void func_8022BFC8(CSysWin* self, u8 kind) {
    if (kind >= 5) return;
    u8 labelIdx[5];
    memcpy(labelIdx, &lbl_eu_806685DC, 4);
    labelIdx[4] = lbl_eu_806685E0;
    u8 idx = labelIdx[kind];
    self->field_38 = kind;
    char* str = func_80136190(&lbl_eu_8050A478[0x8d], &lbl_eu_8050A478[0x9b], idx);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0xa0], str, 0);
    func_80136B4C(self->mLayout, &lbl_eu_8050A478[0xac], str, 0);
    u32 gm = func_80086F9C__Q22cf13CfGameManagerFv(-1);
    self->field_39 = (u8)gm;
    u16 keyVal = func_8013606C(
        &lbl_eu_8050A478[0x8d],
        ((u8)gm != 0) ? &lbl_eu_8050A478[0xb8] : &lbl_eu_8050A478[0xc1], idx);
    char* texName = func_80138F78(keyVal);
    TPLPalette* pal = (TPLPalette*)self->mArcAccessor->GetResource(
        0x74696d67u, texName, 0);
    if (pal != 0) {
        func_80137E7C(self->mLayout, &lbl_eu_8050A478[0xca], pal);
        func_80137E7C(self->mLayout, &lbl_eu_8050A478[0xd4], pal);
        TPLHeader* hdr = pal->descriptorArray->textureHeader;
        u16 texW = hdr->width;
        u16 texH = hdr->height;
        nw4r::lyt::Pane* pane1 =
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0xca], true);
        if (pane1 != 0) {
            f32 sz[2];
            sz[0] = (f32)texW;
            sz[1] = (f32)texH;
            func_80124288(pane1, sz);
        }
        nw4r::lyt::Pane* pane2 =
            self->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A478[0xd4], true);
        if (pane2 != 0) {
            f32 sz[2];
            sz[0] = (f32)texW;
            sz[1] = (f32)texH;
            func_80124288(pane2, sz);
        }
    }
}
#pragma dont_inline off
