// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTutorial
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuTutorial.hpp"

extern "C" void __ct__CMenuTutorial() {}

extern "C" void __dt__13CMenuTutorialFv(void* self) {}

extern "C" void Init__13CMenuTutorialFv() {}

extern "C" void Term__13CMenuTutorialFv() {}

extern "C" void Move__13CMenuTutorialFv() {}

extern "C" void cbRenderBefore__13CMenuTutorialFv(void* self) {}

extern "C" void func_8029A5DC() {}

extern "C" void func_8029A658() {}

extern "C" void func_8029A668() {}

extern "C" void func_8029A764() {}

extern "C" void func_8029A7B4() {}

extern "C" void func_8029A8D4() {}

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
extern "C" void func_8029A924(IScnRender* self) {
    ((void(*)(CMenuTutorial*))cbRenderBefore__13CMenuTutorialFv)(
        (CMenuTutorial*)((uintptr_t)self - 0x58));
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
extern "C" void func_8029A92C(IScnRender* self) {
    ((void(*)(CMenuTutorial*))__dt__13CMenuTutorialFv)(
        (CMenuTutorial*)((uintptr_t)self - 0x58));
}
