// Loading-screen layout controller (kyoshin/CLoad).
// Owns the lyt layout shown while a file request completes; the file-load
// callback builds the layout from the loaded archive and drives a small
// animation state machine per frame.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CLoad.hpp"
#include "monolib/device.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/lib/CLibLayout.hpp"

// Data imports (retail symbol names; global data is not C++-mangled).
extern const float lbl_eu_80668DF0;
extern const float lbl_eu_80668DF4;
extern char lbl_eu_80510CC8[];

// nw4r layout animation helpers (retail names; resolved by symbol tooling).
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);

u8 func_802AE6AC(CLoad* self) { return self->field_2B; }

u8 func_802AE6B4(CLoad* self) { return self->mIsLoaded; }

u8 func_802AE6BC(CLoad* self) { return self->field_2C; }

__attribute__((noinline)) void func_802AE8E0(CLoadFull* self) {
    if (self->field_1C != 0) {
        self->field_2B = 1;
        self->field_28 = 1;
    }
}

// File-load completion callback: builds the loading-screen layout from the
// freshly loaded archive and starts its animations. Retail builds this with
// the size pipeline ([stmw r29]/lmw frame).
#pragma push
#pragma optimize_for_size on
bool CLoad::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x1400,
                                &lbl_eu_80510CC8[0x11], 0);
        Class_8045F858 regionGuard(&mMemRegion);

        // Take ownership of the loaded buffer (inlined CFileHandle::getData),
        // then flush the MEM2 allocator state before building resources.
        void* data = mFileHandle->mData;
        mFileHandle->mData = nullptr;
        mtl::MemManager::setMemInitFlag(false);

        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, &lbl_eu_80510CC8[0x17]);

        // field_2D selects the language-specific resource names.
        func_80136E84(&mLayout, mAccessor,
                      (field_2D != 0) ? &lbl_eu_80510CC8[0x1B] : &lbl_eu_80510CC8[0x2D]);
        func_80136F08(mLayout, &mAnimTrans0, mAccessor,
                      (field_2D != 0) ? &lbl_eu_80510CC8[0x3F] : &lbl_eu_80510CC8[0x54]);
        func_80136F08(mLayout, &mAnimTrans1, mAccessor,
                      (field_2D != 0) ? &lbl_eu_80510CC8[0x69] : &lbl_eu_80510CC8[0x80]);

        // Fade-in on mAnimTrans0 first; mAnimTrans1 stays bound but disabled.
        mLayout->SetAnimationEnable(mAnimTrans1, false);
        mLayout->SetAnimationEnable(mAnimTrans0, true);
        mLayout->Animate(0);

        func_802AE8E0(reinterpret_cast<CLoadFull*>(this));

        mFileHandle = nullptr;
        mMemRegion.func_8045F810();
        return true;
    }
    return false;
}
#pragma pop

// retail frame: stmw/lmw size form
#pragma push
#pragma optimize_for_size on
CLoad::CLoad(u8 arg) {
    mFileHandle = nullptr;
    mAccessor = nullptr;
    mLayout = nullptr;
    mAnimTrans0 = nullptr;
    mAnimTrans1 = nullptr;
    mIsLoaded = 0;
    field_29 = 1;
    mAnimStep = 0;
    field_2B = 0;
    field_2C = 1;
    field_2D = arg;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
CLoad::~CLoad() {
    // mMemRegion's destructor runs automatically after the body.
}
#pragma pop

// Begin the file load request.
void func_802AE508(CLoad* self) {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    self->mFileHandle = CDeviceFile::readFile(
        handle, lbl_eu_80510CC8, reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::func_8044F154(self->mFileHandle, 3);
    CDeviceFile::setHandleFlag2(self->mFileHandle);
}

// Per-frame state-driven layout update: advances the animation state
// machine, then ticks the layout once.
__attribute__((noinline)) void func_802AE560(CLoad* self) {
    if (self->mIsLoaded != 0 && self->mAnimStep != 0) {
        switch (self->mAnimStep) {
        case 1:
            func_802AE7EC(self);
            break;
        case 2:
            func_80137444(self->mAnimTrans1, lbl_eu_80668DF0);
            break;
        case 3:
            func_802AE894(self);
            break;
        }
        self->mLayout->Animate(0);
    }
}

// Draw the loading layout when loaded and visible.
void func_802AE5F0(CLoad* self, nw4r::lyt::DrawInfo* drawInfo) {
    // short-circuit || lowers as test-return + branch-over-return (retail shape)
    if (self->mIsLoaded == 0 || self->field_29 == 0)
        return;
    if (self->mAnimStep != 0)
        func_80137038(self->mLayout, drawInfo, 0, 1);
}

// Tear down the loaded layout, accessor and mem region.
void func_802AE62C(CLoad* self) {
    CDeviceVI::waitForDrawDone();
    func_801390E0(&self->mFileHandle);
    self->mIsLoaded = 0;
    // Redundant inner null check: MWCC CSEs the delete's repeated test into
    // one cmpi and re-tests it, reproducing the retail's second (dead) beq.
    if (self->mLayout != nullptr) {
        delete self->mLayout;
        self->mLayout = nullptr;
    }
    func_80139124(self->mAccessor);
    self->mAccessor = nullptr;
    self->mMemRegion.func_8045F778();
}

// Begin the fade-in animation (step 1).
void func_802AE6C4(CLoad* self) {
    if (self->mAnimStep == 0) {
        self->mAnimStep = 1;
        self->mLayout->SetAnimationEnable(self->mAnimTrans1, 0);
        self->mLayout->SetAnimationEnable(self->mAnimTrans0, 1);
        self->mLayout->Animate(0);
        self->field_2C = 0;
    }
}

// Restart as the retry animation (step 3).
void func_802AE758(CLoad* self) {
    if (self->mAnimStep == 2) {
        self->mAnimStep = 3;
        self->mLayout->SetAnimationEnable(self->mAnimTrans1, 0);
        self->mLayout->SetAnimationEnable(self->mAnimTrans0, 1);
        self->mLayout->Animate(0);
        self->field_2C = 0;
    }
}

// Step 1 -> 2: fade-in finished; swap to mAnimTrans1 held on its last frame.
void func_802AE7EC(CLoad* self) {
    if (func_80137444(self->mAnimTrans0, lbl_eu_80668DF0) == 0) {
        return;
    }
    self->mAnimStep = 2;
    self->mLayout->SetAnimationEnable(self->mAnimTrans0, 0);
    self->mLayout->SetAnimationEnable(self->mAnimTrans1, 1);
    self->mAnimTrans1->SetFrame(lbl_eu_80668DF4);
    self->mLayout->Animate(0);
    self->field_2C = 1;
}

// Step 3 -> idle: retry animation finished.
__attribute__((noinline)) void func_802AE894(CLoad* self) {
    if (func_80137510(self->mAnimTrans0, lbl_eu_80668DF0) != 0) {
        self->mAnimStep = 0;
        self->field_2C = 1;
    }
}
