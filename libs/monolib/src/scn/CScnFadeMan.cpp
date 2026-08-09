// Decompiled CScnFadeMan catalog TU for monolib/src/scn/CScnFadeMan
#include "libs/monolib/src/scn/CScnFadeMan.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CViewRoot.hpp"

// --- Scene sink: owned CScn (IScnRender callback) -------------------------

// Publishes the current fade color pointer (address of mCurrentColor).
extern "C" void* func_8049C7A8(void* self) { return (void*)((u8*)self + 0x8); }

// True while the fade timer has not started moving (mCurrentFrame == 0).
extern "C" u32 func_8049C794(void* self) { return (s32)(*(u32*)((u8*)self + 0x38)) <= 0; }

// --- Constructor ----------------------------------------------------------

CScnFadeMan::CScnFadeMan(CScn* scene) {
    mScene = scene;
    mCurrentColor.r = 0.0f;
    mCurrentColor.g = 0.0f;
    mCurrentColor.b = 0.0f;
    mCurrentColor.a = 0.0f;
    mStartColor.r = 0.0f;
    mStartColor.g = 0.0f;
    mStartColor.b = 0.0f;
    mStartColor.a = 0.0f;
    mDestColor.r = 0.0f;
    mDestColor.g = 0.0f;
    mDestColor.b = 0.0f;
    mDestColor.a = 0.0f;
    mCurrentFrame = 0;
    mFrameCount = 0;
    mScene->addRenderCB(this, 0xc, 0x1);
}

// --- Destructor -----------------------------------------------------------

CScnFadeMan::~CScnFadeMan() {
    mScene->removeRenderCB(this);
}

// --- Update ----------------------------------------------------------------

void CScnFadeMan::update() {
    if (mCurrentFrame == 0) {
        return;
    }
    mCurrentFrame -= 0x100;
    if (mCurrentFrame <= 0) {
        mCurrentColor = mDestColor;
        mCurrentFrame = 0;
        return;
    }
    // The timers are 8.8 fixed-point (1 frame = 0x100), so convert each to a
    // real-valued float (integer part + fraction byte) before dividing.
    f32 prog =
        ((f32)(mCurrentFrame % 256) * lbl_eu_8066AB7C + (f32)(mCurrentFrame / 256)) /
        ((f32)(mFrameCount % 256) * lbl_eu_8066AB7C + (f32)(mFrameCount / 256));
    // Interpolate from mStartColor toward mDestColor, writing mCurrentColor.
    mCurrentColor.r = mStartColor.r * prog + mDestColor.r * (1.0f - prog);
    mCurrentColor.g = mStartColor.g * prog + mDestColor.g * (1.0f - prog);
    mCurrentColor.b = mStartColor.b * prog + mDestColor.b * (1.0f - prog);
    mCurrentColor.a = mStartColor.a * prog + mDestColor.a * (1.0f - prog);
}

// --- cbRenderBefore ---------------------------------------------------------

void CScnFadeMan::cbRenderBefore() {
    if (mCurrentColor.a == 0.0f) {
        return;
    }
    CDeviceGX::getCacheInstance()->func_8044BE38();
    CViewRoot::getInstance();
    CViewRoot::func_80442DA8();
    {
        CDrawGX draw;
        draw.func_80456570(0);
        draw.func_8045657C(0);
        draw.setCol(mCurrentColor);
        draw.begin(0x9, 0x1);
        ml::CRect rect;
        CView::func_8043EA88(rect, CView::getCurrentView());
        draw.add(*(ml::CRect16*)&rect);
        draw.end();
    }
}

// --- func_8049C72C -----------------------------------------------------------

void func_8049C72C(CScnFadeMan* self, u32 count, const ml::CCol4* src) {
    if (CDeviceVI::isTvFormatPal() && count > 1) {
        count = (u32)((f32)count / 1.2f);
    }
    self->mStartColor = self->mCurrentColor;
    self->mDestColor = *src;
    u32 shift = count << 8;
    self->mCurrentFrame = shift;
    self->mFrameCount = shift;
    if (count == 0) {
        self->mCurrentColor = *src;
    }
}

// --- func_8049C7B0 ------------------------------------------------------------

u32 func_8049C7B0(CScnFadeMan* self) {
    return (lbl_eu_8066AB80 == self->mCurrentColor.a && self->mCurrentFrame == 0) ||
           (lbl_eu_8066AB80 == self->mDestColor.a && self->mCurrentFrame != 0);
}