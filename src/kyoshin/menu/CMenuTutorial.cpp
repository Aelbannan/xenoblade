// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTutorial
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuTutorial.hpp"

void __ct__CMenuTutorial(){}

CMenuTutorial::~CMenuTutorial() {}

void CMenuTutorial::Init() {}

void CMenuTutorial::Term() {}

void CMenuTutorial::Move() {}

void CMenuTutorial::cbRenderBefore() {}

void func_8029A5DC(){}


void func_8029A668(){}

void func_8029A764(){}

void func_8029A7B4(){}

void func_8029A8D4(){}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuTutorial.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__13CMenuTutorialFv
 */
void CMenuTutorial::func_8029A924() {
    cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuTutorial.
 *
 * Same adjustment as func_8029A924 but forwards to the destructor.
 *
 * Retail: subi r3, r3, 0x58; b __dt__13CMenuTutorialFv
 */
void CMenuTutorial::func_8029A92C() {
    this->~CMenuTutorial();
}

extern unsigned long lbl_eu_80664A28;
extern "C" unsigned long func_8029A658(void) { return lbl_eu_80664A28 != 0; }
