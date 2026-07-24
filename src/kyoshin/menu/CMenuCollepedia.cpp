// Catalog TU for kyoshin/menu/CMenuCollepedia
// FULL_MATCH: func_80253188, func_80253190

#include "kyoshin/menu/CMenuCollepedia.hpp"

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-8025487c
extern "C" void __ct__CMenuCollepedia() {}
// LLM-HARNESS-END: us-8025487c

// LLM-HARNESS-BEGIN: us-8025494c
extern "C" void __dt__15CMenuCollepediaFv(void* self) {}
// LLM-HARNESS-END: us-8025494c

// LLM-HARNESS-BEGIN: us-802549c4
extern "C" void Init__15CMenuCollepediaFv() {}
// LLM-HARNESS-END: us-802549c4

// LLM-HARNESS-BEGIN: us-80254d08
extern "C" void Term__15CMenuCollepediaFv() {}
// LLM-HARNESS-END: us-80254d08

// LLM-HARNESS-BEGIN: us-80254d74
extern "C" void Move__15CMenuCollepediaFv() {}
// LLM-HARNESS-END: us-80254d74

// LLM-HARNESS-BEGIN: us-80254df8
extern "C" void cbRenderBefore__15CMenuCollepediaFv(void* self) {}
// LLM-HARNESS-END: us-80254df8

// LLM-HARNESS-BEGIN: us-80254e9c
extern "C" void func_80252C60() {}
// LLM-HARNESS-END: us-80254e9c

// LLM-HARNESS-BEGIN: us-80254f10
extern "C" void func_80252CD4() {}
// LLM-HARNESS-END: us-80254f10

// LLM-HARNESS-BEGIN: us-80254f20
extern "C" void func_80252CE4() {}
// LLM-HARNESS-END: us-80254f20

// LLM-HARNESS-BEGIN: us-80254fc4
extern "C" void func_80252D88() {}
// LLM-HARNESS-END: us-80254fc4

// LLM-HARNESS-BEGIN: us-80255014
extern "C" void func_80252DD8() {}
// LLM-HARNESS-END: us-80255014

// LLM-HARNESS-BEGIN: us-802552a8
extern "C" void func_8025306C() {}
// LLM-HARNESS-END: us-802552a8

// LLM-HARNESS-BEGIN: us-802552f8
extern "C" void func_802530BC() {}
// LLM-HARNESS-END: us-802552f8

// LLM-HARNESS-BEGIN: us-80255364
extern "C" void func_80253128() {}
// LLM-HARNESS-END: us-80255364

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
// LLM-HARNESS-BEGIN: us-802553c4
extern "C" void func_80253188(IScnRender* self) {
    ((void(*)(CMenuCollepedia*))cbRenderBefore__15CMenuCollepediaFv)(
        (CMenuCollepedia*)((u32)self - 0x58));
}
// LLM-HARNESS-END: us-802553c4

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
// LLM-HARNESS-BEGIN: us-802553cc
extern "C" void func_80253190(IScnRender* self) {
    ((void(*)(CMenuCollepedia*))__dt__15CMenuCollepediaFv)(
        (CMenuCollepedia*)((u32)self - 0x58));
}
// LLM-HARNESS-END: us-802553cc
