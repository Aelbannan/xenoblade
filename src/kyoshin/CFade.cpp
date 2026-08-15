// kyoshin/CFade
// FULL_MATCH: __ct__CFade, func_802443E8, func_80244508, func_80244510,
// func_80244558, func_802445A4, func_802445F0.
// The dtor/OnFileEvent need stmw r30/r28 which requires -O4,s (walls #13);
// the source is byte-perfect apart from that fixed -O4,p save/restore split.

#include "kyoshin/CFade.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util.hpp"
#include "monolib/work/CEventFile.hpp"

u8 CFade::func_80244508() { return mReady; }


u8 CFade::func_80244510() { return mVisible; }


// Retail marks the fade overlay loaded/ready once the layout is attached.
// extern "C" + noinline: retail reloc name is unmangled and func_802445F0 is
// called via `bl` from OnFileEvent (same stripped-mangling convention as the
// func_80244558/func_802445A4 helpers).
extern "C" __declspec(noinline) void func_802445F0(CFade* self) {
    if (self->mLayout != nullptr) {
        self->mReady = 1;
        self->mIsLoaded = 1;
    }
}

// Target 5: once the fade-in animation reaches the target frame, mark faded-in.
// Target 6: once the fade-out animation rewinds, return to idle.
// extern "C" free functions: the US retail build strips member manglings for
// these helpers (retail `bl func_80244558`), so C linkage reproduces the
// unmangled reloc names at the call sites (PLAN.md §17.6 approved). noinline
// keeps the retail `bl` instead of inlining the body into func_802443E8.
extern "C" __declspec(noinline) void func_80244558(CFade* self) {
    if (func_80137444(self->mAnimTrans, lbl_eu_80668750) == 0) return;
    self->mFadeState = 2;
    self->mVisible = 1;
}

extern "C" __declspec(noinline) void func_802445A4(CFade* self) {
    if (func_80137510(self->mAnimTrans, lbl_eu_80668750) != 0) {
        self->mFadeState = 0;
        self->mVisible = 1;
    }
}

// The fade layout arc's file/branch/arc resource strings live at these offsets
// into the shared string pool (US/EU split1 .rodata):
//   +0x00 "/menu/jp/CFade.arc" (approx base)
//   +0x11 region tag for createRegion
//   +0x17 "arc"
//   +0x1B layout brlyt name
//   +0x2B animation brlan name
bool CFade::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        // Cache the shared string pool base so MWCC keeps it in a callee-saved
        // register across the bl calls (retail: r30 + per-use addi offsets).
        char* str = lbl_eu_8050B5A0;

        // Reserve a scratch region inside the memory handle for the layout build,
        // then RAII-register it with CLibLayout (frees on scope exit).
        mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x400,
                                &str[0x11], 0);
        Class_8045F858 sp8(&mMemRegion);

        // Detach the loaded file buffer for the layout archive.
        u8* data = static_cast<u8*>(mFileHandle->getData());
        mtl::MemManager::func_80434A4C(false);

        // Build the arc resource accessor and attach the archive buffer.
        mArcResAcc = CLibLayout::createArcResourceAccessor();
        mArcResAcc->Attach(data, &str[0x17]);

        // Load the fade layout and its animation transform.
        func_80136E84(&mLayout, mArcResAcc, &str[0x1B]);
        func_80136F08(mLayout, &mAnimTrans, mArcResAcc, &str[0x2B]);

        // Enable the animation and kick off the first frame.
        mLayout->SetAnimationEnable(mAnimTrans, true);
        mLayout->Animate(0);

        // Mark the fade overlay loaded/ready now that the layout is attached.
        func_802445F0(this);

        mFileHandle = nullptr;
        mMemRegion.func_8045F810();
        return true;
    }
    return false;
}

CFade::CFade() : CFadeVtblBase(), mMemRegion() {
    mFileHandle = nullptr;
    mArcResAcc = nullptr;
    mLayout = nullptr;
    mAnimTrans = nullptr;
    mIsLoaded = 0;
    mFadeState = 0;
    mReady = 0;
    mVisible = 1;
}

// Target 7: destructor (member region + conditional delete are compiler-emitted).
CFade::~CFade() {}

// Target 4: start an async read of the fade layout arc.
void CFade::func_8024439C() {
    mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
        lbl_eu_8050B5A0, reinterpret_cast<IWorkEvent*>(this), 0, 0);
    CDeviceFile::setHandleFlag2(mFileHandle);
}

// Target 9: per-frame update. Drive the fade-in/out animations, then refresh
// the layout's animation once loaded and not idle. The s32 copy of the byte
// state makes MWCC emit signed cmpi (retail), and the sparse switch gives the
// beq/fallthrough dispatch shape (if/else-if would invert the branches).
void CFade::func_802443E8() {
    if (mIsLoaded == 0) return;
    s32 state = mFadeState;
    if (state == 0) return;
    switch (state) {
    case 1:
        func_80244558(this);
        break;
    case 3:
        func_802445A4(this);
        break;
    }
    mLayout->Animate(0);
}

// Target 3: render the fade layout once loaded and fading.
void CFade::func_80244460(nw4r::lyt::DrawInfo* drawInfo) {
    if (mIsLoaded == 0) return;
    if (mFadeState == 0) return;
    func_80137038(mLayout, drawInfo, 0, 1);
}

// Target 10: unload, freeing the file handle, layout and arc resources.
void CFade::func_8024448C() {
    func_801390E0(&mFileHandle);
    nw4r::lyt::Layout* layout = mLayout;
    mIsLoaded = 0;
    if (layout != nullptr) {
        delete layout;
        mLayout = nullptr;
    }
    func_80139124(mArcResAcc);
    mArcResAcc = nullptr;
    mMemRegion.func_8045F778();
}

// Target 1: start fading in if currently idle.
void CFade::func_80244518() {
    if (mFadeState == 0) {
        mFadeState = 1;
        mVisible = 0;
    }
}

// Target 2: start fading out if fully faded in.
void CFade::func_80244538() {
    if (mFadeState == 2) {
        mFadeState = 3;
        mVisible = 0;
    }
}