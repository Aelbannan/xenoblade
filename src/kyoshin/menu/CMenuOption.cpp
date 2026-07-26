// FULL_MATCH: func_8029BECC, func_8029BED4

#include "kyoshin/menu/CMenuOption.hpp"

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CMenuOption() {}

extern "C" void __dt__11CMenuOptionFv(void* self) {}

extern "C" void Init__11CMenuOptionFv() {}

extern "C" void Term__11CMenuOptionFv() {}

extern "C" void Move__11CMenuOptionFv() {}

extern "C" void cbRenderBefore__11CMenuOptionFv(void* self) {}

extern "C" void func_8029BB24() {}

extern "C" void func_8029BBA0() {}

extern "C" void func_8029BBB0() {}

extern "C" void func_8029BC28() {}

extern "C" void func_8029BC78() {}

extern "C" void func_8029BE7C() {}

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
extern "C" void func_8029BECC(IScnRender* self) {
    ((void(*)(CMenuOption*))cbRenderBefore__11CMenuOptionFv)(
        (CMenuOption*)((uintptr_t)self - 0x58));
}

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
extern "C" void func_8029BED4(IScnRender* self) {
    ((void(*)(CMenuOption*))__dt__11CMenuOptionFv)(
        (CMenuOption*)((uintptr_t)self - 0x58));
}
