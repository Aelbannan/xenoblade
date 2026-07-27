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

void CMenuBattleDamage::thunk_IWorkEvent_dtor() {
    this->~CMenuBattleDamage();
}

void CMenuBattleDamage::thunk_IScnRender_cbRenderBefore() {
    this->cbRenderBefore();
}

void CMenuBattleDamage::thunk_IScnRender_dtor() {
    this->~CMenuBattleDamage();
}

CPcSelectCursor01::~CPcSelectCursor01() {}

void CPcSelectCursor01::cursorInit() {
    if (this->mState != 0) {
        return;
    }
    this->mState = 1;
    this->mAnimFrame = 0;
}

void cursorMoveRight() {}

void cursorMoveLeft() {}

u8 CPcSelectCursor01::cursorGetAnimFrame() { return this->mAnimFrame; }

void cursorShow() {}

void cursorHide() {}

void cursorAnimate() {}

void cursorFinalize() {}
