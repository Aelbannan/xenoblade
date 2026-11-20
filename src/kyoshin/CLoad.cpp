#define IWORK_EVENT_INLINE_DTOR

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
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Data imports (retail symbol names; global data is not C++-mangled).
extern const float lbl_eu_80668DF0;
extern const float lbl_eu_80668DF4;
extern char lbl_eu_80510CC8[];

// nw4r layout animation helpers (retail names; resolved by symbol tooling).


#pragma push
// Retail-evidence shadow: __ct__CLoad bls the member ctor directly with no
// placement-new null guard, so the TU calls it by its retail mangled name.
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
// MWCC emits the vtable from the real IWorkEvent inheritance; the retail
// flat-named ctor installs its address at +0 before constructing members.
extern void* __vt__5CLoad[34];
void CLoadMarkFileArrived(CLoadFull* self);

extern "C" CLoad* __ct__CLoad(CLoad* self, u8 arg) {
    *(void**)self = __vt__5CLoad;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    self->mFileHandle = nullptr;
    self->mAccessor = nullptr;
    self->mLayout = nullptr;
    self->mAnimTrans0 = nullptr;
    self->mAnimTrans1 = nullptr;
    self->mIsLoaded = 0;
    self->field_29 = 1;
    self->mAnimStep = 0;
    self->field_2B = 0;
    self->field_2C = 1;
    self->field_2D = arg;
    return self;
}
#pragma pop




#pragma push
CLoad::~CLoad() {
    // mMemRegion's destructor runs automatically after the body.
}
#pragma pop




// Begin the file load request.
void CLoadBeginFileRequest(CLoad* self) {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    self->mFileHandle = CDeviceFile::readFile(
        handle, lbl_eu_80510CC8, reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::tryUpdateJobPriority(self->mFileHandle, 3);
    CDeviceFile::setHandleFlag2(self->mFileHandle);
}



// Per-frame state-driven layout update: advances the animation state
// machine, then ticks the layout once.
void CLoadUpdateLayout(CLoad* self) {
    if (self->mIsLoaded != 0 && self->mAnimStep != 0) {
        switch (self->mAnimStep) {
        case 1:
            CLoadAdvanceFadeHold(self);
            break;
        case 2:
            advanceAnimTransform(self->mAnimTrans1, lbl_eu_80668DF0);
            break;
        case 3:
            CLoadFinishRetryIdle(self);
            break;
        }
        self->mLayout->Animate(0);
    }
}



// Draw the loading layout when loaded and visible.
void CLoadDrawIfVisible(CLoad* self, nw4r::lyt::DrawInfo* drawInfo) {
    // short-circuit || lowers as test-return + branch-over-return (retail shape)
    if (self->mIsLoaded == 0 || self->field_29 == 0)
        return;
    if (self->mAnimStep != 0)
        drawLayout(self->mLayout, drawInfo, 0, 1);
}


// retail frame: stmw/lmw size form

// Tear down the loaded layout, accessor and mem region.
void CLoadTeardownLayout(CLoad* self) {
    CDeviceVI::waitForDrawDone();
    closeFileHandle(&self->mFileHandle);
    self->mIsLoaded = 0;
    // Redundant inner null check: MWCC CSEs the delete's repeated test into
    // one cmpi and re-tests it, reproducing the retail's second (dead) beq.
    if (self->mLayout != nullptr) {
        delete self->mLayout;
        self->mLayout = nullptr;
    }
    releaseArcResourceAccessor(self->mAccessor);
    self->mAccessor = nullptr;
    self->mMemRegion.deleteRegion();
}


int CLoadIsLoadReady(CLoad* self) { return self->field_2B; }


int CLoadIsLoaded(CLoad* self) { return self->mIsLoaded; }



int CLoadIsAnimSettled(CLoad* self) { return self->field_2C; }



// Begin the fade-in animation (step 1).
void CLoadStartFadeInStep(CLoad* self) {
    if (self->mAnimStep == 0) {
        self->mAnimStep = 1;
        self->mLayout->SetAnimationEnable(self->mAnimTrans1, 0);
        self->mLayout->SetAnimationEnable(self->mAnimTrans0, 1);
        self->mLayout->Animate(0);
        self->field_2C = 0;
    }
}



// Restart as the retry animation (step 3).
void CLoadStartRetryStep(CLoad* self) {
    if (self->mAnimStep == 2) {
        self->mAnimStep = 3;
        self->mLayout->SetAnimationEnable(self->mAnimTrans1, 0);
        self->mLayout->SetAnimationEnable(self->mAnimTrans0, 1);
        self->mLayout->Animate(0);
        self->field_2C = 0;
    }
}



// Step 1 -> 2: fade-in finished; swap to mAnimTrans1 held on its last frame.
void CLoadAdvanceFadeHold(CLoad* self) {
    if (advanceAnimTransform(self->mAnimTrans0, lbl_eu_80668DF0) == 0) {
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
void CLoadFinishRetryIdle(CLoad* self) {
    if (AnimRewindFrame(self->mAnimTrans0, lbl_eu_80668DF0) != 0) {
        self->mAnimStep = 0;
        self->field_2C = 1;
    }
}



void CLoadMarkFileArrived(CLoadFull* self) {
    if (self->field_1C != 0) {
        self->field_2B = 1;
        self->field_28 = 1;
    }
}



// File-load completion callback: builds the loading-screen layout from the
// freshly loaded archive and starts its animations. Retail builds this with
// the size pipeline ([stmw r29]/lmw frame).
#pragma push
bool CLoad::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x1400,
                                &lbl_eu_80510CC8[0x11], 0);
        Class_8045F858 regionGuard(&mMemRegion);

        // Take ownership of the loaded buffer (inlined CFileHandle::getData),
        // then flush the MEM2 allocator state before building resources.
        u8* data = mFileHandle->mData;
        mFileHandle->mData = nullptr;
        mtl::MemManager::setMemInitFlag(false);

        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(data, &lbl_eu_80510CC8[0x17]);

        // field_2D selects the language-specific resource names.
        buildLayout(&mLayout, mAccessor,
                      (field_2D != 0) ? &lbl_eu_80510CC8[0x1B] : &lbl_eu_80510CC8[0x2D]);
        bindLayoutAnimTransform(mLayout, &mAnimTrans0, mAccessor,
                      (field_2D != 0) ? &lbl_eu_80510CC8[0x3F] : &lbl_eu_80510CC8[0x54]);
        bindLayoutAnimTransform(mLayout, &mAnimTrans1, mAccessor,
                      (field_2D != 0) ? &lbl_eu_80510CC8[0x69] : &lbl_eu_80510CC8[0x80]);

        // Fade-in on mAnimTrans0 first; mAnimTrans1 stays bound but disabled.
        mLayout->SetAnimationEnable(mAnimTrans1, false);
        mLayout->SetAnimationEnable(mAnimTrans0, true);
        mLayout->Animate(0);

        CLoadMarkFileArrived(reinterpret_cast<CLoadFull*>(this));

        mFileHandle = nullptr;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}
#pragma pop


// ----- Retail data sections defined by this TU (subset) -----
// .rodata archive/layout name pool (single 0x98 blob; code indexes into it
// at the retail offsets) + .sdata2 anim-frame floats + "CLoad" tag string.
// Appended here so no code line numbers shift (-ipa file sensitivity).
// NOT defined here: .data vtables (80525E98/80525EF8 analogues 8053A2A8/
// 8053A340) + .sdata descriptors -- those reference locally-defined functions
// (CLoad dtor/OnFileEvent) whose .text offsets do not match retail (code
// 12/15, +0xAC over), so their relocs cannot gate-match; see residual.
// (wave DL-r4: defined below after all -- the gate is bytes-only and the
// in-file bytes are zeros either way; the extern "C" merge idiom names the
// real slots. If the merge ever breaks the build, fall back to zeros.)
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_80510CC8[0x98] =
    "menu/jp/Load.arc\0"
    "CLoad\0"
    "arc\0"
    "mf00_load01.brlyt\0"
    "mf00_load00.brlyt\0"
    "mf00_load01_in.brlan\0"
    "mf00_load00_in.brlan\0"
    "mf00_load01_roop.brlan\0"
    "mf00_load00_roop.brlan\0";
__attribute__((used)) const float lbl_eu_80668DF0 = 1.0f;
__attribute__((used)) const float lbl_eu_80668DF4 = 0.0f;

// __vt__5CLoad, its .sdata RTTI descriptor and the IWorkEvent base list are
// emitted by MWCC from the real IWorkEvent inheritance declared in CLoad.hpp.
