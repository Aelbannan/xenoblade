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
 * The thunk converts the subobject pointer (IScnRender*) back to the enclosing
 * CMenuCollepedia* via integer-offset adjustment before forwarding to the
 * real override. The function-pointer cast prevents MWCC from null-checking
 * the static_cast chain (which would add a cmpwi/beq that the retail thunk
 * does not have).
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__15CMenuCollepediaFv
 */
void func_80253188(IScnRender* self) {
    ((void(*)(CMenuCollepedia*))cbRenderBefore__15CMenuCollepediaFv)(
        static_cast<CMenuCollepedia*>(self));
}

/**
 * IScnRender vtable this-adjusting thunk for destructor.
 *
 * Same adjustment as func_80253188: converts IScnRender* (at offset 0x58)
 * back to CMenuCollepedia* and forwards to ~CMenuCollepedia(). r4 (the
 * MWCC deletion flag) is preserved from the caller because the cast to
 * void(*)(CMenuCollepedia*) declares only one parameter, leaving r4
 * unmodified in the tail call.
 *
 * Retail: subi r3, r3, 0x58; b __dt__15CMenuCollepediaFv
 */
void func_80253190(IScnRender* self){
    ((void(*)(CMenuCollepedia*))__dt__15CMenuCollepediaFv)(
        static_cast<CMenuCollepedia*>(self));
}
