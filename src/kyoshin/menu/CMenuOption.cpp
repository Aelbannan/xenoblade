// FULL_MATCH: func_8029BECC, func_8029BED4

#include "kyoshin/menu/CMenuOption.hpp"

#include "kyoshin/harness_catalog.hpp"

void __ct__CMenuOption(){}

void CMenuOption::~CMenuOption() {}

void CMenuOption::Init() {}

void CMenuOption::Term() {}

void CMenuOption::Move() {}

void CMenuOption::cbRenderBefore() {}

void func_8029BB24(){}

void func_8029BBA0(){}

void func_8029BBB0(){}

void func_8029BC28(){}

void func_8029BC78(){}

void func_8029BE7C(){}

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
