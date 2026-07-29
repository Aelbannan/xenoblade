#include "kyoshin/menu/CMenuFade.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"

extern int lbl_eu_80663FA0;

extern "C" {
extern char lbl_eu_804FDEA8[];  // String table: +0xa layout, +0x18 anim, +0x26 pane name
nw4r::lyt::ArcResourceAccessor* func_801355F4();  // Shared ARC resource accessor
void func_80137B44(nw4r::lyt::Layout*, const char*, u32);  // Pane color setter
}

CMenuFade::CMenuFade(){

}

CMenuFade::~CMenuFade() {

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
    // Scoped region guard — destructor releases the region when Init finishes
    Class_8045F858 regionGuard(&mLayoutMem);

    nw4r::lyt::ArcResourceAccessor* accessor = func_801355F4();
    func_80136E84(&mLayout, accessor, lbl_eu_804FDEA8 + 0xa);

    accessor = func_801355F4();
    func_80136F08(mLayout, &mAnimDefault, accessor, lbl_eu_804FDEA8 + 0x18);

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
    mLayoutMem.func_8045F810();
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
    mLayoutMem.func_8045F778();
    lbl_eu_80663FA0 = 0;
}

int func_80113E1C() {
    return lbl_eu_80663FA0;
}
u8 func_80113E24(void* pthis) {
    return *(u8*)((char*)pthis + 0x94);
}
// Converted to inline member function in header
void __dt__9CMenuFadeFv(CMenuFade*);
void func_80113E38(CMenuFade* p) {
    __dt__9CMenuFadeFv((CMenuFade*)((char*)p - 0x58));
}
void cbRenderBefore__9CMenuFadeFv(void* self);
void func_80113E40(void* self) { ((void(*)(void*))cbRenderBefore__9CMenuFadeFv)((char*)self - 0x5c); }
void func_80113E48(void* arg0) {
    __dt__9CMenuFadeFv((struct CMenuFade*)((char*)arg0 - 0x5C));
}

void func_80113C84(){}
