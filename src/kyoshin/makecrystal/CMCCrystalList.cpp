// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMCCrystalList.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/work/CEventFile.hpp"

#include <revolution/os/OSError.h>

void __ct__CMCCrystalList(){}

// UnkClass_8045F564 member dtor and operator delete (retail names).
extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int flags);
extern "C" void __dl__FPv(void* p);

// Retail dtor is a plain free function: destroys the two embedded region
// members and frees the object when the delete flag is positive.
#pragma optimize_for_size on
extern "C" void* __dt__14CMCCrystalListFv(void* self, int flags) {
    if (self != 0) {
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x14, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flags > 0)
            __dl__FPv(self);
    }
    return self;
}
#pragma optimize_for_size off

void func_80222848(){}

// Per-state animation handlers for the func_802228B8 dispatcher (defined below).
// extern "C" so call relocs resolve to the unmangled retail names; noinline
// keeps MWCC's -inline auto from folding the handlers into the dispatcher.
extern "C" void __declspec(noinline) func_802233AC(CMCCrystalList* self);
extern "C" void __declspec(noinline) func_802233F8(CMCCrystalList* self);
extern "C" void __declspec(noinline) func_80223444(CMCCrystalList* self);
extern "C" void __declspec(noinline) func_80223498(CMCCrystalList* self);
extern "C" void __declspec(noinline) func_802234E4(CMCCrystalList* self);
extern "C" void __declspec(noinline) func_80223530(CMCCrystalList* self);
extern "C" void __declspec(noinline) func_8022357C(CMCCrystalList* self);
extern "C" void __declspec(noinline) func_802235C8(CMCCrystalList* self);

// Retail 0x802246F8: state-machine dispatcher. When mState != 0, dispatch on
// mStateIdx (0..7) to the per-state animation handlers, then re-animate the
// layout (virtual Animate at vtable+0x38, r4 = 0 option). Values 8+ (and the
// switch default) fall through to the same Animate call.
void func_802228B8(CMCCrystalList* self)
{
    if (self->mState != 0) {
        switch (self->mStateIdx) {
        case 0: func_802233AC(self); break;
        case 1: func_802233F8(self); break;
        case 2: func_80223444(self); break;
        case 3: func_80223498(self); break;
        case 4: func_802234E4(self); break;
        case 5: func_80223530(self); break;
        case 6: func_8022357C(self); break;
        case 7: func_802235C8(self); break;
        case 8:
        default:
            break;
        }
        self->mLayout->Animate(0);
    }
}

namespace nw4r { namespace lyt { class Layout; class DrawInfo; } }
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

void CMCCrystalList::func_80222964(nw4r::lyt::DrawInfo* drawInfo)
{
    if (mState != 0) {
        func_80137038(*reinterpret_cast<nw4r::lyt::Layout**>(reinterpret_cast<unsigned char*>(this) + 0x34), drawInfo, 0, 1);
    }
}

void __dt__80222984(){}

u8 CMCCrystalList::func_80222A50() { return mDataFlag; }

u8 CMCCrystalList::func_80222A58() { return mIsActive; }

// Forward decls for functions defined later in this TU. They are C-linkage in
// retail (unmangled symbols), so extern "C" keeps the call relocs unmangled.
extern "C" void func_80223698(CMCCrystalList* self);
extern "C" void func_80223614(CMCCrystalList* self, u32 idx);
extern "C" void func_80223754(CMCCrystalList* self);
extern "C" int func_8013BC0C(void*, void*);
extern "C" void func_80223810(CMCCrystalList* self);
extern "C" void func_802238CC(CMCCrystalList* self);

void func_80222A60(CMCCrystalList* self)
{
    if (self->mStateIdx == 0) {
        self->mStateIdx = 1;
        self->mIsActive = 0;
        func_80223698(self);
    }
}

void func_80222A84(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 6;
        self->mIsActive = 0;
        func_80223810(self);
    }
}

void func_80222AA8(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 7;
        self->mIsActive = 0;
        func_80223810(self);
    }
}

void func_80222ACC(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 8;
        self->mIsActive = 0;
        func_802238CC(self);
    }
}

void func_80222AF0(CMCCrystalList* self)
{
    if (self->mStateIdx == 3) {
        self->mStateIdx = 4;
        self->mIsActive = 0;
        func_80223754(self);
    }
}

void func_80222B14(){}

void func_80222D9C(){}

void func_80222F64(){}

void func_80223004(){}

// Retail 0x802232E4: reset anim trans 4's frame to 0, then re-animate the
// layout (virtual Animate at vtable+0x38, r4 = 0 option).
void func_802232E4(CMCCrystalList* self)
{
    func_802238CC(self);
    self->mAnimTrans4->SetFrame(0.0f);
    self->mLayout->Animate(0);
}

void func_80223334(){}

// Retail 0x802233AC: wait for anim trans 1 (via func_80137444) to finish,
// then enter state 2 and run func_80223754.
extern "C" void __declspec(noinline) func_802233AC(CMCCrystalList* self)
{
    if (func_80137444(self->mAnimTrans1, lbl_eu_80668544)) {
        self->mStateIdx = 2;
        func_80223754(self);
    }
}

// Retail 0x802233F8: wait for anim trans 2 to finish, then activate state 3.
extern "C" void __declspec(noinline) func_802233F8(CMCCrystalList* self)
{
    if (func_80137444(self->mAnimTrans2, lbl_eu_80668544) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x80223444: for each crystal slot in state 1, run func_80223614.
// u8 loop counter (retail masks the slot index to 8 bits before indexing).
// The retail call passes only r3 (the index rides in the leftover r4 from
// the loop's rlwinm, which the callee uses), so the call is made through a
// 1-arg cast of the 2-arg function.
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
extern "C" void __declspec(noinline) func_80223444(CMCCrystalList* self)
{
    for (u8 i = 0; i < 8; i++) {
        if ((s8)self->mSlotStates[i] == 1) {
            ((void (*)(CMCCrystalList*))func_80223614)(self);
        }
    }
}
#pragma optimize_for_size off

// func_80223614 (recovered): format the slot icon pane name (fmt @ 0x13E),
// resolve the pane through the layout root FindPaneByName virtual, and when
// the func_8013BC0C check passes, clear the slot's state byte.
// optimize_for_size merges the r30/r31 saves into the retail stmw r30
// prologue (MWCC_REFERENCE: two-param leaf prologue pattern).
#pragma optimize_for_size on
extern "C" void func_80223614(CMCCrystalList* self, u32 idx) {
    char buf[0x20];
    sprintf(buf, &lbl_eu_805092C0[0x13e], idx + 1);
    nw4r::lyt::Pane* pane =
        self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    if (func_8013BC0C(pane, self->mAnimRes5) != 0) {
        self->mSlotStates[idx] = 0;
    }
}
#pragma optimize_for_size off

// Retail 0x80223498: wait for anim trans 2 (via func_80137510) to finish,
// then enter state 5 and run func_80223698.
extern "C" void __declspec(noinline) func_80223498(CMCCrystalList* self)
{
    if (func_80137510(self->mAnimTrans2, lbl_eu_80668544) != 0) {
        self->mStateIdx = 5;
        func_80223698(self);
    }
}

// Retail 0x802234E4: wait for anim trans 1 (via func_80137510) to finish,
// then reset to state 0 and activate.
extern "C" void __declspec(noinline) func_802234E4(CMCCrystalList* self)
{
    if (func_80137510(self->mAnimTrans1, lbl_eu_80668544) != 0) {
        self->mStateIdx = 0;
        self->mIsActive = 1;
    }
}

// Retail 0x80223530: wait for anim trans 3 to finish, then activate state 3.
extern "C" void __declspec(noinline) func_80223530(CMCCrystalList* self)
{
    if (func_80137444(self->mAnimTrans3, lbl_eu_80668544) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x8022357C: wait for anim trans 3 (via func_80137510) to finish,
// then activate state 3.
extern "C" void __declspec(noinline) func_8022357C(CMCCrystalList* self)
{
    if (func_80137510(self->mAnimTrans3, lbl_eu_80668544) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x802235C8: wait for anim trans 4 (via func_80137444) to finish,
// then activate state 3.
extern "C" void __declspec(noinline) func_802235C8(CMCCrystalList* self)
{
    if (func_80137444(self->mAnimTrans4, lbl_eu_80668544) != 0) {
        self->mStateIdx = 3;
        self->mIsActive = 1;
    }
}

// Retail 0x802254D8: layout animation setup. Disable anim resource 5 across
// the whole pane tree (recursive via the root pane), then toggle the
// per-layout anim transforms: disable 4/3/2, enable 1.
extern "C" void __declspec(noinline) func_80223698(CMCCrystalList* self)
{
    self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, false, true);
    self->mLayout->SetAnimationEnable(self->mAnimTrans4, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans3, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, true);
}

// Retail 0x80225554: same setup, disable 4/3/1, enable 2.
extern "C" void __declspec(noinline) func_80223754(CMCCrystalList* self)
{
    self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, false, true);
    self->mLayout->SetAnimationEnable(self->mAnimTrans4, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans3, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, true);
}

// Retail 0x80225650: same setup, disable 4/1/2, enable 3.
extern "C" void __declspec(noinline) func_80223810(CMCCrystalList* self)
{
    self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, false, true);
    self->mLayout->SetAnimationEnable(self->mAnimTrans4, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans3, true);
}

// Retail 0x8022570C: same setup, disable 1/2/3, enable 4.
extern "C" void __declspec(noinline) func_802238CC(CMCCrystalList* self)
{
    self->mLayout->GetRootPane()->SetAnimationEnable(*self->mAnimRes5, false, true);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans2, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans3, false);
    self->mLayout->SetAnimationEnable(self->mAnimTrans4, true);
}

void func_80223988(){}

// Retail 0x80225890: file-load callback for the crystal list. Two files can
// load asynchronously: the main crystal-list layout (mFileHandle1) and a
// second resource archive (mFileHandle2). Each branch mirrors the
// CMCCrystalInfo::OnFileEvent shape: region-guarded file buffer, layout
// build (layout + 4 anim transforms + anim resource 5), font pane bind,
// per-slot text stamping, and final activation flags.
bool CMCCrystalList::OnFileEvent(CEventFile* pEventFile)
{
    if (mFileHandle1 == pEventFile->mFileHandle) {
        // === crystal list file 1 loaded ===
        mMemRegion1.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x10000,
            &lbl_eu_805092C0[0x18f], 0);
        Class_8045F858 regionGuard1(&mMemRegion1);

        CFileHandle* h1 = mFileHandle1;
        void* fileData = h1->mData;
        h1->mData = nullptr;
        mtl::MemManager::func_80434A4C(false);

        mArcResAccessor = CLibLayout::createArcResourceAccessor();
        mArcResAccessor->Attach(fileData, &lbl_eu_805092C0[0x19e]);

        func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            &mLayout, mArcResAccessor, &lbl_eu_805092C0[0x1a2]);

        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mLayout, &mAnimTrans1, mArcResAccessor, &lbl_eu_805092C0[0x1b9]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mLayout, &mAnimTrans2, mArcResAccessor, &lbl_eu_805092C0[0x1d3]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mLayout, &mAnimTrans3, mArcResAccessor, &lbl_eu_805092C0[0x1f2]);
        func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            mLayout, &mAnimTrans4, mArcResAccessor, &lbl_eu_805092C0[0x20f]);
        func_80136FA0(mLayout, &mAnimRes5, mArcResAccessor, &lbl_eu_805092C0[0x22b]);

        // Bind the loaded font's pane into the layout root.
        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        func_8013676C(rootPane,
            reinterpret_cast<CMCCrystalListFontView*>(
                CDeviceFont::func_80452C10(1, mLayout))
                ->vf7());

        // If a character set is loaded, stamp every crystal-slot text pane
        // (8 slots, two text panes each) with it.
        u32 sh = func_801355BC();
        if (sh != 0) {
            char buf[0x20];
            for (u8 i = 1; i <= 8; i++) {
                sprintf(buf, &lbl_eu_805092C0[0x4c], i);
                func_801368C0(mLayout, buf, sh);
                sprintf(buf, &lbl_eu_805092C0[0x5e], i);
                func_801368C0(mLayout, buf, sh);
            }
        }

        func_80136B4C(mLayout, &lbl_eu_805092C0[0x258],
            func_80136190(&lbl_eu_805092C0[0x247], &lbl_eu_805092C0[0x253], 0x2c), 0);
        func_80136B4C(mLayout, &lbl_eu_805092C0[0x267],
            func_80136190(&lbl_eu_805092C0[0x247], &lbl_eu_805092C0[0x253], 0x2d), 0);

        func_80223698(this);
        mLayout->Animate(0);

        // Second archive present -> mark the list ready.
        if (mArcResAccessor2 != 0) {
            mDataFlag = 1;
            mState = 1;
        }
        mFileHandle1 = nullptr;
        mMemRegion1.func_8045F810();
        return true;
    } else if (mFileHandle2 == pEventFile->mFileHandle) {
        // === crystal list file 2 loaded ===
        mMemRegion2.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x100,
            &lbl_eu_805092C0[0x277], 0);
        Class_8045F858 regionGuard2(&mMemRegion2);

        CFileHandle* h2 = mFileHandle2;
        void* fileData = h2->mData;
        h2->mData = nullptr;
        mtl::MemManager::func_80434A4C(false);

        mArcResAccessor2 = CLibLayout::createArcResourceAccessor();
        mArcResAccessor2->Attach(fileData, &lbl_eu_805092C0[0x19e]);

        // Main layout already built -> mark the list ready.
        if (mArcResAccessor != 0) {
            mDataFlag = 1;
            mState = 1;
        }
        mFileHandle2 = nullptr;
        mMemRegion2.func_8045F810();
        return true;
    }

    return false;
}
