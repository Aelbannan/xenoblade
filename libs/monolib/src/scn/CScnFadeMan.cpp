// Decompiled CScnFadeMan catalog TU for monolib/src/scn/CScnFadeMan
#include "libs/monolib/src/scn/CScnFadeMan.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CViewRoot.hpp"


// --- Scene sink: owned CScn (IScnRender callback) -------------------------

// Retail sdata2 magic doubles for int->float conversion, referenced by name so
// no local .sdata2 pool is emitted (retail CScnFadeMan.o has an empty
// .sdata2). AB88 = 0x4330000080000000 (signed), AB98 = 0x4330000000000000
// (unsigned). See MWCC_CASES 7i for the union trick.
extern double lbl_eu_8066AB88;
extern double lbl_eu_8066AB98;

union F64Conv_AB {
    f64 d;
    u32 w[2];
};

static inline f32 s32ToF_AB88(s32 v) {
    F64Conv_AB c;
    c.w[0] = 0x43300000u;
    c.w[1] = (u32)v ^ 0x80000000u;
    return (f32)(c.d - lbl_eu_8066AB88);
}

static inline f32 u32ToF_AB98(u32 v) {
    F64Conv_AB c;
    c.w[0] = 0x43300000u;
    c.w[1] = v;
    return (f32)(c.d - lbl_eu_8066AB98);
}

// Retail vtable words: dtor/cbRenderBefore are defined by this TU's class
// members (symbols __dt__11CScnFadeManFv / cbRenderBefore__11CScnFadeManFv);
// the extern "C" declarations below bind to those same symbols (MWCC keeps
// the literal names for address-of).
extern "C" void __dt__11CScnFadeManFv();
extern "C" void cbRenderBefore__11CScnFadeManFv(const CScnFadeMan* self);

// [.data] 0x8056EB50-0x8056EB60 (16B): CScnFadeMan vtable.
extern "C" u32 lbl_eu_80663A38;
extern "C" u32 lbl_eu_8056EB50[4] = {
    (u32)&lbl_eu_80663A38, 0x00000000,
    (u32)&__dt__11CScnFadeManFv, (u32)&cbRenderBefore__11CScnFadeManFv,
};


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
    m_vtable = (u8*)lbl_eu_8056EB50;
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
    m_vtable = (u8*)lbl_eu_8056EB50;
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
        (s32ToF_AB88(mCurrentFrame % 256) * lbl_eu_8066AB7C + s32ToF_AB88(mCurrentFrame / 256)) /
        (s32ToF_AB88(mFrameCount % 256) * lbl_eu_8066AB7C + s32ToF_AB88(mFrameCount / 256));
    // Interpolate from mStartColor toward mDestColor, writing mCurrentColor.
    mCurrentColor.r = mStartColor.r * prog + mDestColor.r * (lbl_eu_8066AB80 - prog);
    mCurrentColor.g = mStartColor.g * prog + mDestColor.g * (lbl_eu_8066AB80 - prog);
    mCurrentColor.b = mStartColor.b * prog + mDestColor.b * (lbl_eu_8066AB80 - prog);
    mCurrentColor.a = mStartColor.a * prog + mDestColor.a * (lbl_eu_8066AB80 - prog);
}

// --- cbRenderBefore ---------------------------------------------------------

// Forced-name free function with const self (MWCC_CASES "const-self lever"):
// constness lets MWCC hoist the sdata2 global lfs above the prologue stores,
// matching retail's load schedule; the vtable word references this exact name.
extern "C" void cbRenderBefore__11CScnFadeManFv(const CScnFadeMan* self) {
    if (self->mCurrentColor.a == lbl_eu_8066AB78) {
        return;
    }
    CDeviceGX::getCacheInstance()->func_8044BE38();
    CViewRoot::getInstance();
    CViewRoot::func_80442DA8();
    {
        CDrawGX draw;
        draw.func_80456570(0);
        draw.func_8045657C(0);
        draw.setCol(self->mCurrentColor);
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
        count = (u32)(u32ToF_AB98(count) / lbl_eu_8066AB90);
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