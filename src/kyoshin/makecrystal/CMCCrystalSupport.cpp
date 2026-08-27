#define IWORK_EVENT_INLINE_DTOR
#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/makecrystal/CMCCrystalSupport.hpp"

extern "C" {
extern void* lbl_eu_80536770[];
extern char lbl_eu_8050AA3C[];
extern void* lbl_eu_80664090;
extern const float lbl_eu_80668630;
extern const float lbl_eu_80668634;

void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
    nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*,
    char*);
u16 func_80136254(const char*, const char*, const char*);
char* func_80138F78(u16);
void func_80137E7C(void*, const char*, void*);
u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(nw4r::lyt::AnimTransform*,
                                                  float);
void func_80138078__FUl(u32);
}

// Retail vtable lbl_eu_80536770: RTTI lbl_eu_80662898, dtor @+0x08, then the
// full IWorkEvent tree (WorkEvent1..31, OnFileEvent, OnPauseTrigger) -- so
// CMCCrystalSupport derives IWorkEvent. novtable + explicit label write in the
// ctor, same shape as CMCCrystalList / CMCCrystalInfo. IWORK_EVENT_INLINE_DTOR
// elides the base-dtor call so __dt__17CMCCrystalSupportFv stays 0x40 bytes
// (retail shape); the extra weak __dt__10IWorkEventFv is link-deduped and
// documented in IWorkEvent.hpp.
class __declspec(novtable) CMCCrystalSupport : public IWorkEvent {
public:
    virtual ~CMCCrystalSupport();

    nw4r::lyt::ArcResourceAccessor* mAccessor;
    nw4r::lyt::Layout* mLayout;
    nw4r::lyt::AnimTransform* mAnimTransform;
    u8 mLoaded;
    u8 _pad11[3];
    s32 mState;
    u8 mFinished;
};

extern "C" void __ct__CMCCrystalSupport(CMCCrystalSupport* self,
                                        nw4r::lyt::ArcResourceAccessor* accessor) {
    *(void**)self = lbl_eu_80536770;
    self->mAccessor = accessor;
    self->mLayout = nullptr;
    self->mAnimTransform = nullptr;
    self->mLoaded = 0;
    self->mState = 0;
    self->mFinished = 1;
}

CMCCrystalSupport::~CMCCrystalSupport() {}

#pragma push
#pragma optimize_for_size on
extern "C" void func_8022E8F8(CMCCrystalSupport* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->mLayout, self->mAccessor, lbl_eu_8050AA3C);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mLayout, &self->mAnimTransform, self->mAccessor, lbl_eu_8050AA3C + 0x15);
    self->mLayout->SetAnimationEnable(self->mAnimTransform, true);
    self->mLayout->Animate(0);
    self->mLoaded = 1;
}
#pragma pop

extern "C" void func_8022E988(CMCCrystalSupport* self) {
    if (self->mLoaded != 0) {
        if (self->mState == 1) {
            func_8022EB0C(self);
        }
        self->mLayout->Animate(0);
    }
}

extern "C" void func_8022E9E4(CMCCrystalSupport* self,
                              nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mLoaded == 0) {
        return;
    }
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(
        self->mLayout, drawInfo, 0, 1);
}

extern "C" void func_8022EA04(CMCCrystalSupport* self) {
    self->mLoaded = 0;
    if (self->mLayout != nullptr) {
        delete self->mLayout;
        self->mLayout = nullptr;
    }
}

extern "C" void func_8022EA64(CMCCrystalSupport* self) {
    self->mState = 1;
    *(float*)((u8*)self->mAnimTransform + 0x10) = lbl_eu_80668630;
    self->mFinished = 0;
    func_80138078__FUl(0x8e);
}

#pragma push
#pragma optimize_for_size on
extern "C" void func_8022EA88(CMCCrystalSupport* self, u16 index) {
    const char* base = lbl_eu_8050AA3C;
    u16 key = func_80136254((const char*)lbl_eu_80664090, base + 0x2d,
                            (const char*)index);
    char* name = func_80138F78(key);
    void* resource = self->mAccessor->GetResource('timg', name, nullptr);
    if (resource != nullptr) {
        func_80137E7C(self->mLayout, lbl_eu_8050AA3C + 0x38, resource);
    }
}
#pragma pop

// noinline: retail calls this via a real `bl` from func_8022E988 (sibling in this
// TU); without it MWCC inlines the body into the caller and the bl disappears.
extern "C" __declspec(noinline) void func_8022EB0C(CMCCrystalSupport* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(
            self->mAnimTransform, lbl_eu_80668634) != 0) {
        self->mState = 0;
        self->mFinished = 1;
    }
}
