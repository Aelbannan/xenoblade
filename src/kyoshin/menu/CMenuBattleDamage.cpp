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

extern "C" void __dt__17CMenuBattleDamageFv();
extern "C" void cbRenderBefore__17CMenuBattleDamageFv();

void thunk_IWorkEvent_dtor(IWorkEvent* self) { ((void(*)(void*))__dt__17CMenuBattleDamageFv)(reinterpret_cast<char*>(self) - 0x58); }

void thunk_IScnRender_cbRenderBefore(IScnRender* self) { ((void(*)(void*))cbRenderBefore__17CMenuBattleDamageFv)(reinterpret_cast<char*>(self) - 0x5c); }

void thunk_IScnRender_dtor(IScnRender* self) { ((void(*)(void*))__dt__17CMenuBattleDamageFv)(reinterpret_cast<char*>(self) - 0x5c); }

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
