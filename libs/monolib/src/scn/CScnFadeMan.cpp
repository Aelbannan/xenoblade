// Decompiled CScnFadeMan catalog TU for monolib/src/scn/CScnFadeMan
#include "libs/monolib/src/scn/CScnFadeMan.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CViewRoot.hpp"

// Retail SDA2 pool constants (monolibdata2-owned; named refs keep the lfs
// relocs pinned and avoid a local .sdata2 pool).
extern "C" const f32 lbl_eu_8066AB78; // 0.0f (AB80/AB90 in the header)

// --- Scene sink: owned CScn (IScnRender callback) -------------------------

// Publishes the current fade color pointer (address of mCurrentColor).
extern "C" void* func_8049C7A8(u8* self) { return (void*)((u8*)self + 0x8); }

// True while the fade timer has not started moving (mCurrentFrame == 0).
extern "C" u32 func_8049C794(u8* self) { return (s32)(*(u32*)((u8*)self + 0x38)) <= 0; }

// --- Constructor ----------------------------------------------------------

CScnFadeMan::CScnFadeMan(CScn* scene) {
    mScene = scene;
    // Store the retail vtable blob pointer manually (see CScnFadeMan.hpp) so
    // the ctor vptr-store relocs name the retail blob instead of a compiler
    // __vt__ symbol.
    m_vtable = lbl_eu_8056EB50;
    mCurrentColor.r = lbl_eu_8066AB78;
    mCurrentColor.g = lbl_eu_8066AB78;
    mCurrentColor.b = lbl_eu_8066AB78;
    mCurrentColor.a = lbl_eu_8066AB78;
    mStartColor.r = lbl_eu_8066AB78;
    mStartColor.g = lbl_eu_8066AB78;
    mStartColor.b = lbl_eu_8066AB78;
    mStartColor.a = lbl_eu_8066AB78;
    mDestColor.r = lbl_eu_8066AB78;
    mDestColor.g = lbl_eu_8066AB78;
    mDestColor.b = lbl_eu_8066AB78;
    mDestColor.a = lbl_eu_8066AB78;
    mCurrentFrame = 0;
    mFrameCount = 0;
    // Non-virtual class (see hpp) - explicit IScnRender* conversion for the
    // render-CB registration; the object address doubles as the IScnRender*.
    mScene->addRenderCB((IScnRender*)this, 0xc, 0x1);
}

// --- Destructor -----------------------------------------------------------

CScnFadeMan::~CScnFadeMan() {
    // Re-store the vtable pointer like the virtual-class dtor would (see hpp).
    m_vtable = lbl_eu_8056EB50;
    // Same explicit IScnRender* conversion as the ctor.
    mScene->removeRenderCB((IScnRender*)this);
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
    mCurrentColor.r = mStartColor.r * prog + mDestColor.r * (lbl_eu_8066AB80 - prog);
    mCurrentColor.g = mStartColor.g * prog + mDestColor.g * (lbl_eu_8066AB80 - prog);
    mCurrentColor.b = mStartColor.b * prog + mDestColor.b * (lbl_eu_8066AB80 - prog);
    mCurrentColor.a = mStartColor.a * prog + mDestColor.a * (lbl_eu_8066AB80 - prog);
}

// --- cbRenderBefore ---------------------------------------------------------

void CScnFadeMan::cbRenderBefore() {
    if (mCurrentColor.a == lbl_eu_8066AB78) {
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
        count = (u32)((f32)count / lbl_eu_8066AB90);
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