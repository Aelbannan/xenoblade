// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTutorial
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuTutorial.hpp"

void __ct__CMenuTutorial(){}

void CMenuTutorial::~CMenuTutorial() {}

void CMenuTutorial::Init() {}

void CMenuTutorial::Term() {}

void CMenuTutorial::Move() {}

void CMenuTutorial::cbRenderBefore() {}

void func_8029A5DC(){}

void func_8029A658(){}

void func_8029A668(){}

void func_8029A764(){}

void func_8029A7B4(){}

void func_8029A8D4(){}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuTutorial.
 * This thunk converts the subobject pointer (IScnRender*) back to the
 * enclosing CMenuTutorial* via offset adjustment before forwarding to
 * the real override.
 * Uses uintptr_t arithmetic to avoid C++ pointer arithmetic rules.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__13CMenuTutorialFv
 */
void func_8029A924(IScnRender* self) {
    ((void(*)(CMenuTutorial*))cbRenderBefore__13CMenuTutorialFv)(
        static_cast<CMenuTutorial*>(self));
}

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuTutorial.
 *
 * Same adjustment as func_8029A924 but forwards to the destructor.
 * The function-pointer type declares only one parameter (CMenuTutorial*),
 * so r4 (the MWCC deletion flag) is preserved from the caller through
 * the tail call without modification.
 *
 * Retail: subi r3, r3, 0x58; b __dt__13CMenuTutorialFv
 */
void func_8029A92C(IScnRender* self){
    ((void(*)(CMenuTutorial*))__dt__13CMenuTutorialFv)(
        static_cast<CMenuTutorial*>(self));
}
