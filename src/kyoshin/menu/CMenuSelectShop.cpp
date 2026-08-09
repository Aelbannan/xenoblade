// Auto-scaffolded catalog TU for kyoshin/menu/CMenuSelectShop.
// The retail select-shop menu object is laid out as a plain struct with the
// IUIWindow region inline; its ctor/dtor and member hooks are emitted under the
// retail symbol names.

#include "kyoshin/harness_catalog.hpp"

#include "monolib/device/CDeviceVI.hpp"

#include "kyoshin/menu/CMenuSelectShop.hpp"

// ---------------------------------------------------------------------------
// Target 2: CMenuSelectShop ctor (unmangled retail symbol; not a match target)
// ---------------------------------------------------------------------------
void __ct__CMenuSelectShop(){}

// ---------------------------------------------------------------------------
// Target 4: CMenuSelectShop::~CMenuSelectShop()  (us-8018b48c)
// Complete-object dtor with delete flag. Subobjects are destroyed in reverse
// construction order: CCur18@0xB0, CSelShopWin@0x88, UnkClass_8045F564@0x78,
// then the IUIWindow base. The nested flag dispatch selects operator delete.
// ---------------------------------------------------------------------------
extern "C" CMenuSelectShop* __dt__15CMenuSelectShopFv(CMenuSelectShop* self, int flags) {
    if (self != 0) {
        __dt__6CCur18Fv(&self->mCursor[0], -1);
        __dt__11CSelShopWinFv(&self->mSelShop, -1);
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion, -1);
        __dt__9IUIWindowFv(self, 0);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void CMenuSelectShop::Init() {}

// ---------------------------------------------------------------------------
// Target 3: CMenuSelectShop::Term()  (us-8018b670)
// Wait for draw-done, detach the render callback from the owning scene, tear
// down the shop window, run the CCur18 cursor update virtual, release the
// memory region and clear the singleton global. The `if (this)` prepending on
// the render-cb arg reproduces the retail `mr / beq / addi r4,+0x70` idiom.
// ---------------------------------------------------------------------------
void CMenuSelectShop::Term() {
    CDeviceVI::waitForDrawDone();

    void* render = this;
    if (this) render = &mOcc70;
    removeRenderCB__4CScnFP10IScnRender(mScene, render);

    func_8022C85C(&mSelShop);
    reinterpret_cast<CCursor18*>(&mCursor[0])->vf3();
    mMemRegion.func_8045F778();
    lbl_eu_806642E8 = 0;
}

void CMenuSelectShop::Move() {}

// ---------------------------------------------------------------------------
// Target 1: func_8018A200  (us-8018b7b4)
// When the shop window has finished loading, start its show animation and flag
// the menu state.
// ---------------------------------------------------------------------------
void func_8018A200(CMenuSelectShop* self) {
    if (func_8022C8D0(&self->mSelShop) != 0) {
        func_8022C8E0(&self->mSelShop);
        self->mState = 1;
    }
}

// ---------------------------------------------------------------------------
// Target 5: func_8018A248  (us-8018b7fc)
// When the shop window animation is running: activate the cursor, resolve the
// selected item's screen position into a stack VEC3 (func_8022C930), hand it to
// the cursor move virtual, and advance the menu state.
// ---------------------------------------------------------------------------
void func_8018A248(CMenuSelectShop* self) {
    if (func_8022C8D8(&self->mSelShop) != 0) {
        func_801D216C(&self->mCursor[0], 1);
        nw4r::math::VEC3 out;
        func_8022C930(&out, &self->mSelShop, (u8)self->mSelIndex);
        reinterpret_cast<CCur18View*>(&self->mCursor[0])->vf04(&out);
        self->mState = 2;
    }
}

void func_8018A2C0(){}

// ---------------------------------------------------------------------------
// Target 2: func_8018A4A0  (us-8018ba54)
// When the shop window animation is running, mark the menu state and the field
// 0x64 busy flag.
// ---------------------------------------------------------------------------
void func_8018A4A0(CMenuSelectShop* self) {
    if (func_8022C8D8(&self->mSelShop) != 0) {
        self->mState = 4;
        self->mField64 = 1;
    }
}

void CMenuSelectShop::cbRenderBefore() {}

void func_8018A58C(){}

bool func_8018A608() { return false; }

// OC/render subobject this-adjusting thunks (not match targets). The IScnRender
// subobject sits at +0x70, the IWorkEvent/OC subobject at +0x6C.
void func_8018A610(void* sub) {
    __dt__15CMenuSelectShopFv((CMenuSelectShop*)((char*)sub - 0x6c), 0);
}

void func_8018A618(void* sub) {
    reinterpret_cast<CMenuSelectShop*>((char*)sub - 0x70)->cbRenderBefore();
}

extern "C" void func_8018A620(void* sub) {
    __dt__15CMenuSelectShopFv((CMenuSelectShop*)((char*)sub - 0x70), 0);
}