#include <types.h>
#include "kyoshin/makecrystal/CMCCrystalSupport.hpp"

extern "C" {
extern void* lbl_eu_80536770[];
extern char lbl_eu_8050AA3C[];
extern void* lbl_eu_80664090;
extern const float lbl_eu_80668630;
extern const float lbl_eu_80668634;

void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    void**, void*, const char*);
void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
    void*, void**, void*, char*);
u16 func_80136254(void*, const char*);
char* func_80138F78(u16);
void func_80137E7C(void*, const char*, void*);
u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(void*, float);
void func_80138078__FUl(u32);
}

namespace {
struct LayoutEnableIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void setAnimationEnable(void*, int);
};

struct LayoutAnimateIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void animate(u32);
};

struct LayoutDestroyIf {
    virtual void destroy(int);
};

struct AccessorGetResourceIf {
    virtual void _v008();
    virtual void* getResource(const char*, const char*, void*);
};
}

class __declspec(novtable) CMCCrystalSupport {
public:
    virtual ~CMCCrystalSupport();

    void* mAccessor;
    void* mLayout;
    void* mAnimTransform;
    u8 mLoaded;
    u8 _pad11[3];
    s32 mState;
    u8 mFinished;
};

extern "C" void __ct__CMCCrystalSupport(CMCCrystalSupport* self, void* accessor) {
    *(void**)self = lbl_eu_80536770;
    self->mAccessor = accessor;
    self->mLayout = nullptr;
    self->mAnimTransform = nullptr;
    self->mLoaded = 0;
    self->mState = 0;
    self->mFinished = 1;
}

CMCCrystalSupport::~CMCCrystalSupport() {}

extern "C" void func_8022E8F8(CMCCrystalSupport* self) {
    func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
        &self->mLayout, self->mAccessor, lbl_eu_8050AA3C);
    func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
        self->mLayout, &self->mAnimTransform, self->mAccessor, lbl_eu_8050AA3C + 0x15);
    ((LayoutEnableIf*)self->mLayout)->setAnimationEnable(self->mAnimTransform, 1);
    ((LayoutAnimateIf*)self->mLayout)->animate(0);
    self->mLoaded = 1;
}

extern "C" void func_8022E988(CMCCrystalSupport* self) {
    if (self->mLoaded != 0) {
        if (self->mState == 1) {
            func_8022EB0C(self);
        }
        ((LayoutAnimateIf*)self->mLayout)->animate(0);
    }
}

extern "C" void func_8022E9E4(CMCCrystalSupport* self, void* drawInfo) {
    if (self->mLoaded == 0) {
        return;
    }
    func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(
        self->mLayout, drawInfo, 0, 1);
}

extern "C" void func_8022EA04(CMCCrystalSupport* self) {
    self->mLoaded = 0;
    if (self->mLayout != nullptr) {
        ((LayoutDestroyIf*)self->mLayout)->destroy(1);
        self->mLayout = nullptr;
    }
}

extern "C" void func_8022EA64(CMCCrystalSupport* self) {
    self->mState = 1;
    *(float*)((u8*)self->mAnimTransform + 0x10) = 0.0f;
    self->mFinished = 0;
    func_80138078__FUl(0x8e);
}

extern "C" void func_8022EA88(CMCCrystalSupport* self, u16 index) {
    u16 key = func_80136254(lbl_eu_80664090, lbl_eu_8050AA3C + 0x2d);
    char* name = func_80138F78(key);
    void* resource = ((AccessorGetResourceIf*)self->mAccessor)->getResource(
        "timg", name, nullptr);
    if (resource != nullptr) {
        func_80137E7C(self->mLayout, lbl_eu_8050AA3C + 0x38, resource);
    }
}

extern "C" void func_8022EB0C(CMCCrystalSupport* self) {
    if (func_80137444__FPQ34nw4r3lyt13AnimTransformf(
            self->mAnimTransform, lbl_eu_80668634) != 0) {
        self->mState = 0;
        self->mFinished = 1;
    }
}
