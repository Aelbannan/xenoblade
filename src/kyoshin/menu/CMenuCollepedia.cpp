// Catalog TU for kyoshin/menu/CMenuCollepedia
// FULL_MATCH: func_80253188, func_80253190

#include "kyoshin/menu/CMenuCollepedia.hpp"

#include "kyoshin/harness_catalog.hpp"

void __ct__CMenuCollepedia(){}

void CMenuCollepedia::~CMenuCollepedia() {}

void CMenuCollepedia::Init() {}

void CMenuCollepedia::Term() {}

void CMenuCollepedia::Move() {}

void CMenuCollepedia::cbRenderBefore() {}

void func_80252C60(){}

void stub_us_80254f10() {}

void func_80252CE4(){}

void func_80252D88(){}

void func_80252DD8(){}

void func_8025306C(){}

void func_802530BC(){}

void func_80253128(){}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuCollepedia.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__15CMenuCollepediaFv
 */
void CMenuCollepedia::func_80253188() {
    cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk for destructor.
 *
 * Same adjustment as func_80253188.
 *
 * Retail: subi r3, r3, 0x58; b __dt__15CMenuCollepediaFv
 */
void CMenuCollepedia::func_80253190() {
    this->~CMenuCollepedia();
}
