// FULL_MATCH: func_8029BECC, func_8029BED4

#include "kyoshin/menu/CMenuOption.hpp"

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-8029dbc4
extern "C" void __ct__CMenuOption() {}
// LLM-HARNESS-END: us-8029dbc4

// LLM-HARNESS-BEGIN: us-8029dc90
extern "C" void __dt__11CMenuOptionFv(void* self) {}
// LLM-HARNESS-END: us-8029dc90

// LLM-HARNESS-BEGIN: us-8029dd08
extern "C" void Init__11CMenuOptionFv() {}
// LLM-HARNESS-END: us-8029dd08

// LLM-HARNESS-BEGIN: us-8029e038
extern "C" void Term__11CMenuOptionFv() {}
// LLM-HARNESS-END: us-8029e038

// LLM-HARNESS-BEGIN: us-8029e0a4
extern "C" void Move__11CMenuOptionFv() {}
// LLM-HARNESS-END: us-8029e0a4

// LLM-HARNESS-BEGIN: us-8029e158
extern "C" void cbRenderBefore__11CMenuOptionFv(void* self) {}
// LLM-HARNESS-END: us-8029e158

// LLM-HARNESS-BEGIN: us-8029e1fc
extern "C" void func_8029BB24() {}
// LLM-HARNESS-END: us-8029e1fc

// LLM-HARNESS-BEGIN: us-8029e278
extern "C" void func_8029BBA0() {}
// LLM-HARNESS-END: us-8029e278

// LLM-HARNESS-BEGIN: us-8029e288
extern "C" void func_8029BBB0() {}
// LLM-HARNESS-END: us-8029e288

// LLM-HARNESS-BEGIN: us-8029e300
extern "C" void func_8029BC28() {}
// LLM-HARNESS-END: us-8029e300

// LLM-HARNESS-BEGIN: us-8029e350
extern "C" void func_8029BC78() {}
// LLM-HARNESS-END: us-8029e350

// LLM-HARNESS-BEGIN: us-8029e554
extern "C" void func_8029BE7C() {}
// LLM-HARNESS-END: us-8029e554

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuOption.
 * The thunk converts the subobject pointer (IScnRender*) back to the enclosing
 * CMenuOption* via offset adjustment before forwarding to the real override.
 * The function-pointer cast prevents MWCC from null-checking the static_cast
 * chain (which would add a cmpwi/beq that the retail thunk does not have).
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuOptionFv
 */
// LLM-HARNESS-BEGIN: us-8029e5a4
extern "C" void func_8029BECC(IScnRender* self) {
    ((void(*)(CMenuOption*))cbRenderBefore__11CMenuOptionFv)(
        (CMenuOption*)((uintptr_t)self - 0x58));
}
// LLM-HARNESS-END: us-8029e5a4

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuOption.
 *
 * Same adjustment as func_8029BECC but forwards to the destructor.
 * r4 (the MWCC deletion flag) is preserved from the caller because the cast
 * to void(*)(CMenuOption*) declares only one parameter, leaving r4 unmodified
 * in the tail call.
 *
 * Retail: subi r3, r3, 0x58; b __dt__11CMenuOptionFv
 */
// LLM-HARNESS-BEGIN: us-8029e5ac
extern "C" void func_8029BED4(IScnRender* self) {
    ((void(*)(CMenuOption*))__dt__11CMenuOptionFv)(
        (CMenuOption*)((uintptr_t)self - 0x58));
}
// LLM-HARNESS-END: us-8029e5ac
