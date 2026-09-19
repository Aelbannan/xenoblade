#include "kyoshin/menu/CMenuFade.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"

extern int lbl_eu_80663FA0;

extern "C" {
extern char lbl_eu_804FDEA8[];  // String table: +0xa layout, +0x18 anim, +0x26 pane name
nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();  // Shared ARC resource accessor
void func_80137B44(nw4r::lyt::Layout*, const char*, u32);  // Pane color setter
}

CMenuFade::CMenuFade(){

}

// Deleting virtual destructor (D1 shape), written as a plain free C-ABI shim
// on the retail mangled symbol (CMenuBattleMode idiom): destroys mLayoutMem
// (flag -1), then calls __dt__8CProcessFv(self, 0) directly -- naming CProcess
// instead of letting MWCC route base destruction through an out-of-line
// ~CTask<IUICf> UNDEF -- then frees the block when deleteFlag > 0. The
// redundant `if (self != 0)` re-checks reproduce the retail dead double-beq.
extern "C" void __dt__8CProcessFv(CProcess* self, int flags);
extern "C" void __dl__FPv(void* p);
extern "C" CMenuFade* __dt__9CMenuFadeFv(CMenuFade* self, int deleteFlag) {
    if (self == 0) goto end;
    self->mLayoutMem.~UnkClass_8045F564();
    if (self != 0) {
        if (self != 0) {
            __dt__8CProcessFv(self, 0);
        }
    }
    if (deleteFlag > 0) __dl__FPv(self);
end:
    return self;
}

/**
 * Initialises the fade layout and animation.
 *
 * Allocates a 0x4000-byte MEM2 region via mLayoutMem, then builds the
 * primary layout (mLayout) and default animation (mAnimDefault) from the
 * shared ARC resource at string offsets 0xa and 0x18 respectively. Enables
 * mAnimDefault, computes per-frame durations from the animation frame size,
 * and sets the initial frame based on field_0x90. Configures pane alpha via
 * func_80137B44 based on field_0x98. Registers this object as an IScnRender
 * render callback at priority 0xd on the owning scene.
 */
void CMenuFade::Init() {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    mLayoutMem.createRegion(handle, 0x4000, lbl_eu_804FDEA8, 0);
    // Scoped region guard - destructor releases the region when Init finishes
    Class_8045F858 regionGuard(&mLayoutMem);

    nw4r::lyt::ArcResourceAccessor* accessor = CUICfManager_getArcResourceAccessor();
    buildLayout(&mLayout, accessor, lbl_eu_804FDEA8 + 0xa);

    accessor = CUICfManager_getArcResourceAccessor();
    bindLayoutAnimTransform(mLayout, &mAnimDefault, accessor, lbl_eu_804FDEA8 + 0x18);

    mLayout->SetAnimationEnable(mAnimDefault, true);

    // Convert animation frame size to float (int-to-float via type-pun).
    // The float result is used in both the field_0x80 and field_0x88
    // computations below.
    float frameSizeF = (float)mAnimDefault->GetFrameSize();

    // Compute per-frame durations: if the divisor is negative, use a
    // fallback constant instead of dividing.
    if (field_0x80 >= 0.0f) {
        field_0x80 = frameSizeF / field_0x80;
    } else {
        field_0x80 = 1.0f;
    }

    // field_0x84: if negative, clamp to fallback value 25.0
    if (field_0x84 < 0.0f) {
        field_0x84 = 25.0f;
    }

    if (field_0x88 >= 0.0f) {
        field_0x88 = frameSizeF / field_0x88;
    } else {
        field_0x88 = 1.0f;
    }

    // Set initial animation frame based on state (field_0x90)
    // States 0/1 start at frame 0; state 2 starts at the last frame.
    switch (field_0x90) {
    case 0:
    case 1:
        field_0x7c = 0.0f;
        break;
    case 2:
        field_0x7c = (float)mAnimDefault->GetFrameSize() - 1.0f;
        break;
    }

    // Configure pane alpha via string offset 0x26
    switch (field_0x98) {
    case 0:
        func_80137B44(mLayout, lbl_eu_804FDEA8 + 0x26, 0xff);
        break;
    case 1:
        func_80137B44(mLayout, lbl_eu_804FDEA8 + 0x26, (u32)-1);
        break;
    }

    // Apply the computed frame and tick the animation
    mAnimDefault->SetFrame(field_0x7c);
    mLayout->Animate(0);

    // Register as IScnRender callback at priority 0xd on the owning scene
    mScn->addRenderCB(this, 0xd, 0);
    mLayoutMem.validateHeap();
}

void CMenuFade::Draw() {

}

void CMenuFade::Term() {
    CDeviceVI::waitForDrawDone();
    mScn->removeRenderCB(this);
    if (mLayout) {
        delete mLayout;
        mLayout = 0;
    }
    mLayoutMem.deleteRegion();
    lbl_eu_80663FA0 = 0;
}

extern "C" int getFadeMenu() {
    return lbl_eu_80663FA0;
}
extern "C" u8 isFadeActive(u8* pthis) {
    return pthis[0x94];
}
extern "C" void triggerFadeMenu(CMenuFade* pthis) {
    pthis->field_0x54 = 1;
}
extern "C" CMenuFade* __dt__9CMenuFadeFv(CMenuFade* self, int deleteFlag);
extern "C" void fwdFadeDtor58(CMenuFade* p) {
    ((void(*)(CMenuFade*))__dt__9CMenuFadeFv)((CMenuFade*)((char*)p - 0x58));
}
extern "C" void cbRenderBefore__9CMenuFadeFv(void* self);
extern "C" void fwdFadeCbRender5C(void* self) { ((void(*)(void*))cbRenderBefore__9CMenuFadeFv)((char*)self - 0x5c); }
extern "C" void fwdFadeDtor5C(void* arg0) {
    ((void(*)(CMenuFade*))__dt__9CMenuFadeFv)((struct CMenuFade*)((char*)arg0 - 0x5C));
}

void func_80113C84(){}
