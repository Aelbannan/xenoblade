// Auto-scaffolded catalog TU for kyoshin/CNumSelect
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CNumSelect.hpp"
#include <cstdio>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/lib/CLibLayout.hpp"
// Forward decls so the dispatch below stays independent of the helper bodies
// (retail keeps every helper as an out-of-line bl call).
extern "C" void func_801EB49C(CNumSelect* self);
extern "C" void func_801EB530(CNumSelect* self);
extern "C" void func_801EB580(CNumSelect* self);
extern "C" void func_801EB644(CNumSelect* self);

// Keep every helper out-of-line: MWCC -inline auto would otherwise fold them
// into the state dispatcher below (retail keeps all five as bl calls).
#pragma push
#pragma auto_inline off
struct CNumSelectFull;

u8 func_801EB018(CNumSelectFull* self) { return self->field_2D; }


u8 func_801EB020(CNumSelectFull* self) { return self->field_2C; }

u8 func_801EB028(CNumSelectFull* self) { return self->field_2E; }


/* State step 3: rebind the open animation (anim at 0x20) and play cue 0xe.
 * States 1 and 3 leave the widget untouched. */
extern "C" void func_801EB178(CNumSelect* self) {
    u8 state = self->field_2F;
    if (state == 1 || state == 3) {
        return;
    }
    self->field_2F = 3;
    self->field_2E = 0;
    self->mpLayout->UnbindAllAnimation();
    self->mpLayout->BindAnimation(self->field_20);
    self->mpLayout->SetAnimationEnable(self->field_20, true);
    func_80138078(0xe);
}

/* State step 4: bind the loop animation at +0x28, then reveal both number
 * panes (first pane shown via flag 1, second hidden via flag 0). */
extern "C" void func_801EB218(CNumSelect* self) {
    u8 state = self->field_2F;
    if (state == 1 || state == 3) {
        return;
    }
    self->mpLayout->UnbindAllAnimation();
    self->mpLayout->BindAnimation(self->field_28);
    self->mpLayout->SetAnimationEnable(self->field_28, true);
    self->field_28->SetFrame(lbl_eu_8066808C);
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0x47], true), 1);
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0x55], true), 0);
    self->field_2F = 4;
}

/* State step 5: same as step 4 but with the pane visibility flags swapped. */
extern "C" void func_801EB314(CNumSelect* self) {
    u8 state = self->field_2F;
    if (state == 1 || state == 3) {
        return;
    }
    self->mpLayout->UnbindAllAnimation();
    self->mpLayout->BindAnimation(self->field_28);
    self->mpLayout->SetAnimationEnable(self->field_28, true);
    self->field_28->SetFrame(lbl_eu_8066808C);
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0x47], true), 0);
    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0x55], true), 1);
    self->field_2F = 5;
}

// Shows/hides the two number panes according to the given value.
// Retail frame merges the three callee-saves via stmw/lmw (-O4,s).
#pragma push
#pragma optimize_for_size on
extern "C" void func_801EB410(CNumSelect* self, int value) {
    if (self->mpLayout != NULL) {
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0x63], true), value);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0x6a], true), value);
    }
}
#pragma pop

// Fade-out step: once anim 0x20 reaches 1.0, swap in anim 0x24 and enable it.
extern "C" void func_801EB49C(CNumSelect* self) {
    if (func_80137444(self->field_20, lbl_eu_80668088)) {
        self->field_2F = 2;
        self->field_2E = 1;
        self->mpLayout->UnbindAllAnimation();
        self->mpLayout->BindAnimation(self->field_24);
        self->mpLayout->SetAnimationEnable(self->field_24, true);
    }
}

// When the +0x20 animation has finished (func_80137510 with the 1.0 constant),
// reset the selection state bytes (m2C/m2E/m2F) for a fresh pass.
void func_801EB530(CNumSelect* self) {
    if (func_80137510(self->field_20, lbl_eu_80668088)) {
        self->field_2F = 0;
        self->field_2E = 1;
        self->field_2C = 0;
    }
}

/* Restart the +0x28 animation from frame 0: unbind everything, attach anim
 * +0x24, rewind its frame, and advance the state machine to step 2. */
extern "C" void func_801EB580(CNumSelect* self) {
    if (func_80137444(self->field_28, lbl_eu_80668088) == 0) {
        return;
    }
    self->field_28->SetFrame(lbl_eu_8066808C);
    self->mpLayout->Animate();
    self->mpLayout->UnbindAllAnimation();
    self->mpLayout->BindAnimation(self->field_24);
    self->mpLayout->SetAnimationEnable(self->field_24, true);
    self->field_24->SetFrame(lbl_eu_8066808C);
    self->field_2F = 2;
    self->field_2E = 1;
}

// Identical body to func_801EB580 (retail duplicates it byte for byte).
extern "C" void func_801EB644(CNumSelect* self) {
    if (func_80137444(self->field_28, lbl_eu_80668088) == 0) {
        return;
    }
    self->field_28->SetFrame(lbl_eu_8066808C);
    self->mpLayout->Animate();
    self->mpLayout->UnbindAllAnimation();
    self->mpLayout->BindAnimation(self->field_24);
    self->mpLayout->SetAnimationEnable(self->field_24, true);
    self->field_24->SetFrame(lbl_eu_8066808C);
    self->field_2F = 2;
    self->field_2E = 1;
}
#pragma pop

/* File-load completion handler: builds the whole number-select HUD from the
 * freshly loaded archive - MEM2 scratch region (RAII guard), arc accessor,
 * layout + three animation transforms, font bind, text seeding, the 'timg'
 * sized highlight panes - then releases the file handle and region.
 * Retail keeps the _savegpr_29 prologue, so match its -O4,s codegen. */
#pragma push
#pragma optimize_for_size on
bool CNumSelect::OnFileEvent(CEventFile* evt) {
    if (field_14 == evt->mFileHandle) {
    mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
    mMemRegion.createRegion(mem2, 0x8000, &lbl_eu_80506C14[0x73], 0);
    Class_8045F858 host(&mMemRegion);
    void* data = field_14->getData();
    mtl::MemManager::setMemInitFlag(false);

    field_18 = CLibLayout::createArcResourceAccessor();
    field_18->Attach(data, &lbl_eu_80506C14[0x7e]);
    func_80136E84(&mpLayout, field_18, &lbl_eu_80506C14[0x82]);
    func_80136F08(mpLayout, &field_20, field_18, &lbl_eu_80506C14[0x9b]);
    func_80136F08(mpLayout, &field_24, field_18, &lbl_eu_80506C14[0xbd]);
    func_80136F08(mpLayout, &field_28, field_18, &lbl_eu_80506C14[0xdb]);

    nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
    void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout);
    func_8013676C(rootPane,
                  reinterpret_cast<CNumSelectFontView*>(fontObj)->vf7());
    func_801368C0(mpLayout, &lbl_eu_80506C14[0x20], (u32)func_801355BC());
    func_801368C0(mpLayout, &lbl_eu_80506C14[0x3d], (u32)func_801355BC());

    mpLayout->UnbindAllAnimation();
    func_80124270(
        mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0xf7], true), 0);

    // Digit-count depends on the region flag at +0x30 (11 vs 12 entries).
    // Materialize the format-string addresses before the region check so
    // MWCC schedules them ahead of the count select (retail order).
    char* fmtStr = &lbl_eu_80506C14[0x102];
    char* idxStr = &lbl_eu_80506C14[0x33];
    u32 numCount = 11;
    if (field_30 == 1) {
        numCount = 12;
    }
    func_80136B4C(mpLayout, &lbl_eu_80506C14[0x10b],
                  func_80136190(fmtStr, idxStr, numCount),
                  0);
    func_80136B4C(mpLayout, &lbl_eu_80506C14[0x115],
                  func_80136190(&lbl_eu_80506C14[0x102], &lbl_eu_80506C14[0x33], 13),
                  0);
    func_80136B4C(mpLayout, &lbl_eu_80506C14[0x11f],
                  func_80136190(&lbl_eu_80506C14[0x102], &lbl_eu_80506C14[0x33], 14),
                  0);

    char* rangeText = func_80136190(&lbl_eu_80506C14[0x129], &lbl_eu_80506C14[0x137], 0x2b);
    func_80136B4C(mpLayout, &lbl_eu_80506C14[0x13c], rangeText, 0);
    func_80136B4C(mpLayout, &lbl_eu_80506C14[0x148], rangeText, 0);

    // Resolve the 'timg' texture named by the language-dependent message and
    // size both highlight panes to its dimensions.
    // Ternary form reproduces retail's select: default 0x15d, bt-eq skips
    // the 0x154 overwrite.
    const char* msgKey = (isClassicController__Q22cf13CfGameManagerFv(-1) != 0)
                             ? &lbl_eu_80506C14[0x154]
                             : &lbl_eu_80506C14[0x15d];
    u16 msgId = func_8013606C(&lbl_eu_80506C14[0x129], msgKey, 0x2b);
    char* timgName = func_80138F78(msgId);
    CNumSelectTimg* timg = func_801355F4()->vf01(0x74696d67, timgName, 0);
    if (timg != NULL) {
        func_80137E7C(mpLayout, &lbl_eu_80506C14[0x166], (u32)timg);
        func_80137E7C(mpLayout, &lbl_eu_80506C14[0x170], (u32)timg);
        u16 timgH = timg->unk8->dims->m02;
        u16 timgW = timg->unk8->dims->m00;

        nw4r::lyt::Pane* paneA =
            mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0x166], true);
        if (paneA != NULL) {
            // MWCC lowers u16->f32 via the sdata2 cast-magic double itself.
            f32 sz[2];
            sz[0] = static_cast<f32>(timgH);
            sz[1] = static_cast<f32>(timgW);
            func_80124288(paneA, sz);
        }
        nw4r::lyt::Pane* paneB =
            mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_80506C14[0x170], true);
        if (paneB != NULL) {
            f32 sz[2];
            sz[0] = static_cast<f32>(timgH);
            sz[1] = static_cast<f32>(timgW);
            func_80124288(paneB, sz);
        }
    }

    if (mpLayout != NULL) {
        field_2D = 1;
    }
    field_14 = NULL;
    mMemRegion.func_8045F810();
    return true;
    }
    return false;
}
#pragma pop

// Out-of-line dtor (retail symbol __dt__10CNumSelectFv); the inline header
// body would not emit the symbol.
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
CNumSelect::~CNumSelect() {}
#pragma optimize_for_size off

extern "C" void func_801EB04C(CNumSelect* self, u8 r4) {
    func_80136910(self->mpLayout, &lbl_eu_80506C14[0x20], r4);
}

// Loads the number-select brlyt archive into the MEM2 scratch region and
// stores the resulting file handle at +0x14.
void CNumSelect::func_801EAE8C() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    field_14 = CDeviceFile::readFile(handle, &lbl_eu_80506C14[0], reinterpret_cast<IWorkEvent*>(this), 0, 0);
}

/* Per-state update dispatch: runs the step handler for field_2F, then
 * animates the layout while the widget is active (field_2C gate). */
// Per-state update dispatch; defined before the helper bodies so MWCC cannot
// inline them into this switch (retail emits five separate bl calls).
extern "C" void func_801EAED4(CNumSelect* self) {
    if (self->field_2C == 0) {
        return;
    }
    switch (self->field_2F) {
    case 1:
        func_801EB49C(self);
        break;
    case 2:
        func_80137444(self->field_24, lbl_eu_80668088);
        break;
    case 3:
        func_801EB530(self);
        break;
    case 4:
        func_801EB580(self);
        break;
    case 5:
        func_801EB644(self);
        break;
    }
    self->mpLayout->Animate();
}

// Draws the layout once the loaded file has been processed (field_2C gate).
// Retail forwards its own r4 (drawInfo) through to func_80137038 untouched.
void CNumSelect::func_801EAF7C(nw4r::lyt::DrawInfo* drawInfo) {
    if (field_2C == 0) {
        return;
    }
    func_80137038(mpLayout, drawInfo, 0, 1);
}

/* Destructor helper: releases the file handle, destroys the layout through its
 * deleting-dtor vtable slot, frees the arc accessor, then tears down the
 * memory region. Retail double-checks the non-null layout (nested identical
 * guards), so both are reproduced. */
extern "C" void func_801EAF9C(CNumSelect* self) {
    func_801390E0(&self->field_14);
    self->field_2C = 0;
    if (self->mpLayout != NULL) {
        if (self->mpLayout != NULL) {
            reinterpret_cast<CNumSelectLayoutDtorVt*>(self->mpLayout)->destroy(1);
        }
        self->mpLayout = NULL;
    }
    func_80139124(self->field_18);
    self->field_18 = NULL;
    self->mMemRegion.func_8045F778();
}

// Updates the pane named by lbl_eu_80506C14+0x17 with the given string.
void CNumSelect::func_801EB030(char* str) {
    func_80136B4C(mpLayout, &lbl_eu_80506C14[0x17], str, 0);
}

#pragma push
#pragma optimize_for_size on

// Builds "pic_NN"-style pane text: extracts the digits via func_80136190,
// formats them, and pushes the result into the layout pane.
void CNumSelect::func_801EB064(int value) {
    char buf[0x1C];
    char* digits = func_80136190(&lbl_eu_80506C14[0x2A], &lbl_eu_80506C14[0x33], 3);
    sprintf(buf, &lbl_eu_80506C14[0x38], value, digits);
    func_80136A1C(mpLayout, &lbl_eu_80506C14[0x3D], buf, 0);
}

/* State step 1: rebind the close animation (anim at 0x20), mark the pane
 * visible, and play cue 0xd. States 1 and 3 leave the widget untouched. */
extern "C" void func_801EB0D4(CNumSelect* self) {
    u8 state = self->field_2F;
    if (state == 1 || state == 3) {
        return;
    }
    self->field_2F = 1;
    self->field_2E = 0;
    self->field_2C = 1;
    self->mpLayout->UnbindAllAnimation();
    self->mpLayout->BindAnimation(self->field_20);
    self->mpLayout->SetAnimationEnable(self->field_20, true);
    func_80138078(0xd);
}

// Retail ctor stores the vtable label explicitly before constructing the
// memory-region member, then zeroes all state fields. Kept inside the
// -O4,s region: retail's stmw r30 frame requires it.
CNumSelect::CNumSelect(u8 arg)
    : mMemRegion(), field_14(0), field_18(0), mpLayout(0), field_20(0),
      field_24(0), field_28(0), field_2C(0), field_2D(0), field_2E(1),
      field_2F(0), field_30(arg) {}
#pragma pop
