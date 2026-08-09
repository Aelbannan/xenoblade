// Catalog TU for kyoshin/CFade
// FULL_MATCH: func_80244508 (returns mReady), func_80244510 (returns mVisible).
// Remaining stubs are NOT_STARTED.

#include "kyoshin/CFade.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util.hpp"
#include "monolib/work/CEventFile.hpp"

u8 CFade::func_80244508() { return mReady; }


u8 CFade::func_80244510() { return mVisible; }


extern "C" void func_802445F0(u8* self) {
    if (*(u32*)(self + 0x1C) != 0) {
        self[0x26] = 1;
        self[0x24] = 1;
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
        void* data = mFileHandle->getData();
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
        func_802445F0((u8*)this);

        mFileHandle = nullptr;
        mMemRegion.func_8045F810();
        return true;
    }
    return false;
}

// Target 8: constructor. Store the shared vtable first, then init the scratch
// region (retail manual-vptr ordering; see CBgTex.cpp).
CFade::CFade() {
    mVtbl = lbl_eu_80536EA8;
    __ct__17UnkClass_8045F564Fv(&mMemRegion);
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
// the layout's visibility once loaded and no longer idle.
void CFade::func_802443E8() {
    if (mIsLoaded == 0) return;
    if (mFadeState == 0) return;
    if (mFadeState == 1)
        func_80244558();
    else if (mFadeState == 3)
        func_802445A4();
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

// Target 5: once the fade-in animation reaches the target frame, mark faded-in.
void CFade::func_80244558() {
    if (func_80137444(mAnimTrans, lbl_eu_80668750) == 0) return;
    mFadeState = 2;
    mVisible = 1;
}

// Target 6: once the fade-out animation rewinds, return to idle.
void CFade::func_802445A4() {
    if (func_80137510(mAnimTrans, lbl_eu_80668750) != 0) {
        mFadeState = 0;
        mVisible = 1;
    }
}