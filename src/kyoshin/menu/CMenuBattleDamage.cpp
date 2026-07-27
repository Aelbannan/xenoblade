// Auto-scaffolded catalog TU for kyoshin/menu/CMenuBattleDamage
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuBattleDamage.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"

CMenuBattleDamage::CMenuBattleDamage() {}

CMenuBattleDamage::~CMenuBattleDamage() {}

void CMenuBattleDamage::Init() {}

void CMenuBattleDamage::Term() {}

void CMenuBattleDamage::Move() {}

void CMenuBattleDamage::cbRenderBefore() {}

void createBattleDamage() {}

void addBattleDamage() {}

void setDamageType(unsigned char val) {
    extern CMenuBattleDamage* lbl_eu_80663F28;
    CMenuBattleDamage* p = lbl_eu_80663F28;
    if (p != 0) p->mDamageType = val;
}

void setDamageDir(unsigned char val) {
    extern CMenuBattleDamage* lbl_eu_80663F28;
    CMenuBattleDamage* p = lbl_eu_80663F28;
    if (p != 0) p->mDamageDir = val;
}

void addBattleDamageEx() {}

void setDamageStyle() {}

void setDamageFlag1() {}

void setDamageFlag2() {}

void addDamageSlot() {}

// MI thunk: IWorkEvent subobject is at +0x58; recover the containing object
void thunk_IWorkEvent_dtor(IWorkEvent* self) {
    uintptr_t base = reinterpret_cast<uintptr_t>(self) - offsetof(CMenuBattleDamageLayout, mIWorkEvent);
    auto* obj = reinterpret_cast<CMenuBattleDamage*>(base);
    obj->~CMenuBattleDamage();
}

// MI thunk: IScnRender subobject is at +0x5c
void thunk_IScnRender_cbRenderBefore(IScnRender* self) {
    uintptr_t base = reinterpret_cast<uintptr_t>(self) - offsetof(CMenuBattleDamageLayout, mIScnRender);
    auto* obj = reinterpret_cast<CMenuBattleDamage*>(base);
    obj->cbRenderBefore();
}

void thunk_IScnRender_dtor(IScnRender* self) {
    uintptr_t base = reinterpret_cast<uintptr_t>(self) - offsetof(CMenuBattleDamageLayout, mIScnRender);
    auto* obj = reinterpret_cast<CMenuBattleDamage*>(base);
    obj->~CMenuBattleDamage();
}

CPcSelectCursor01::~CPcSelectCursor01() {}

void cursorInit(CPcSelectCursor01* this_) {
    if (this_->mState != 0) {
        return;
    }
    this_->mState = 1;
    this_->mAnimFrame = 0;
}

void cursorMoveRight() {}

void cursorMoveLeft() {}

u8 cursorGetAnimFrame(CPcSelectCursor01* self) { return self->mAnimFrame; }

void cursorShow() {}

void cursorHide() {}

void cursorAnimate() {}

void cursorFinalize() {}
