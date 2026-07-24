// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTutorial
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuTutorial.hpp"

// LLM-HARNESS-BEGIN: us-8029c780
extern "C" void __ct__CMenuTutorial() {}
// LLM-HARNESS-END: us-8029c780

// LLM-HARNESS-BEGIN: us-8029c8a0
extern "C" void __dt__13CMenuTutorialFv(void* self) {}
// LLM-HARNESS-END: us-8029c8a0

// LLM-HARNESS-BEGIN: us-8029c90c
extern "C" void Init__13CMenuTutorialFv() {}
// LLM-HARNESS-END: us-8029c90c

// LLM-HARNESS-BEGIN: us-8029cac4
extern "C" void Term__13CMenuTutorialFv() {}
// LLM-HARNESS-END: us-8029cac4

// LLM-HARNESS-BEGIN: us-8029cb60
extern "C" void Move__13CMenuTutorialFv() {}
// LLM-HARNESS-END: us-8029cb60

// LLM-HARNESS-BEGIN: us-8029cc0c
extern "C" void cbRenderBefore__13CMenuTutorialFv(void* self) {}
// LLM-HARNESS-END: us-8029cc0c

// LLM-HARNESS-BEGIN: us-8029cca4
extern "C" void func_8029A5DC() {}
// LLM-HARNESS-END: us-8029cca4

// LLM-HARNESS-BEGIN: us-8029cd20
extern "C" void func_8029A658() {}
// LLM-HARNESS-END: us-8029cd20

// LLM-HARNESS-BEGIN: us-8029cd30
extern "C" void func_8029A668() {}
// LLM-HARNESS-END: us-8029cd30

// LLM-HARNESS-BEGIN: us-8029ce2c
extern "C" void func_8029A764() {}
// LLM-HARNESS-END: us-8029ce2c

// LLM-HARNESS-BEGIN: us-8029ce7c
extern "C" void func_8029A7B4() {}
// LLM-HARNESS-END: us-8029ce7c

// LLM-HARNESS-BEGIN: us-8029cf9c
extern "C" void func_8029A8D4() {}
// LLM-HARNESS-END: us-8029cf9c

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
// LLM-HARNESS-BEGIN: us-8029cfec
extern "C" void func_8029A924(IScnRender* self) {
    ((void(*)(CMenuTutorial*))cbRenderBefore__13CMenuTutorialFv)(
        (CMenuTutorial*)((uintptr_t)self - 0x58));
}
// LLM-HARNESS-END: us-8029cfec

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
// LLM-HARNESS-BEGIN: us-8029cff4
extern "C" void func_8029A92C(IScnRender* self) {
    ((void(*)(CMenuTutorial*))__dt__13CMenuTutorialFv)(
        (CMenuTutorial*)((uintptr_t)self - 0x58));
}
// LLM-HARNESS-END: us-8029cff4
