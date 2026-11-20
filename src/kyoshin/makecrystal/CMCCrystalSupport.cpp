#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CfBdat.hpp"
#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/makecrystal/CMCCrystalSupport.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
extern "C" void func_8022EB0C(CMCCrystalSupport* self);

extern "C" {
extern char lbl_eu_8050AA3C[];
extern const float lbl_eu_80668630;
extern const float lbl_eu_80668634;

}

CMCCrystalSupport::CMCCrystalSupport(nw4r::lyt::ArcResourceAccessor* accessor)
    : mAccessor(accessor), mLayout(nullptr), mAnimTransform(nullptr), mLoaded(0), mState(0), mFinished(1) {}

CMCCrystalSupport::~CMCCrystalSupport() {}

#pragma push
#pragma optimize_for_size on
extern "C" void CrySupBuildLayout(CMCCrystalSupport* self) {
    buildLayout(&self->mLayout, self->mAccessor, lbl_eu_8050AA3C);
    bindLayoutAnimTransform(self->mLayout, &self->mAnimTransform, self->mAccessor, lbl_eu_8050AA3C + 0x15);
    self->mLayout->SetAnimationEnable(self->mAnimTransform, true);
    self->mLayout->Animate(0);
    self->mLoaded = 1;
}
#pragma pop

extern "C" void CrySupAdvanceAnim(CMCCrystalSupport* self);

extern "C" void CrySupTickAnim(CMCCrystalSupport* self) {
    if (self->mLoaded != 0) {
        if (self->mState == 1) {
            CrySupAdvanceAnim(self);
        }
        self->mLayout->Animate(0);
    }
}

extern "C" void CrySupDrawLayout(CMCCrystalSupport* self,
                              nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mLoaded == 0) {
        return;
    }
    drawLayout(
        self->mLayout, drawInfo, 0, 1);
}

extern "C" void CrySupUnloadLayout(CMCCrystalSupport* self) {
    self->mLoaded = 0;
    if (self->mLayout != nullptr) {
        delete self->mLayout;
        self->mLayout = nullptr;
    }
}

extern "C" void CrySupStartState(CMCCrystalSupport* self) {
    self->mState = 1;
    *(float*)((u8*)self->mAnimTransform + 0x10) = lbl_eu_80668630;
    self->mFinished = 0;
    playUISound(0x8e);
}

#pragma push
#pragma optimize_for_size on
extern "C" void CrySupSetTexByIndex(CMCCrystalSupport* self, u16 index) {
    const char* base = lbl_eu_8050AA3C;
    u16 key = BdatGetU16Direct(::cf::CfBdat::spBtlPcListFileData, base + 0x2d, index);
    char* name = MakeTplNameSysFile(key);
    void* resource = self->mAccessor->GetResource('timg', name, nullptr);
    if (resource != nullptr) {
        PaneSetTexPaletteByName(self->mLayout, lbl_eu_8050AA3C + 0x38, (TPLPalette*)resource);
    }
}
#pragma pop

// noinline: retail calls this via a real `bl` from CrySupTickAnim (sibling in this
// TU); without it MWCC inlines the body into the caller and the bl disappears.
extern "C" __declspec(noinline) void CrySupAdvanceAnim(CMCCrystalSupport* self) {
    if (advanceAnimTransform(self->mAnimTransform, lbl_eu_80668634) != 0) {
        self->mState = 0;
        self->mFinished = 1;
    }
}
