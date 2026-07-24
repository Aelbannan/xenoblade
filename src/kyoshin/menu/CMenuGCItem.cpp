// FULL_MATCH: func_802B0F08, func_802B0F10

#include "kyoshin/menu/CMenuGCItem.hpp"

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802b2e40
extern "C" void __dt__11CMenuGCItemFv(void* self) {}
// LLM-HARNESS-END: us-802b2e40

// LLM-HARNESS-BEGIN: us-802b2ecc
extern "C" void Init__11CMenuGCItemFv() {}
// LLM-HARNESS-END: us-802b2ecc

// LLM-HARNESS-BEGIN: us-802b341c
extern "C" void Term__11CMenuGCItemFv() {}
// LLM-HARNESS-END: us-802b341c

// LLM-HARNESS-BEGIN: us-802b3480
extern "C" void Move__11CMenuGCItemFv() {}
// LLM-HARNESS-END: us-802b3480

// LLM-HARNESS-BEGIN: us-802b35bc
extern "C" void cbRenderBefore__11CMenuGCItemFv(void* self) {}
// LLM-HARNESS-END: us-802b35bc

// LLM-HARNESS-BEGIN: us-802b3660
extern "C" void __ct__CMenuGCItem() {}
// LLM-HARNESS-END: us-802b3660

// LLM-HARNESS-BEGIN: us-802b3780
extern "C" void func_802B0D10() {}
// LLM-HARNESS-END: us-802b3780

// LLM-HARNESS-BEGIN: us-802b3794
extern "C" void func_802B0D24() {}
// LLM-HARNESS-END: us-802b3794

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuGCItem.
 * Adjusts the subobject pointer back to CMenuGCItem* before forwarding
 * to the real cbRenderBefore override.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuGCItemFv
 */
// LLM-HARNESS-BEGIN: us-802b3978
extern "C" void func_802B0F08(IScnRender* self) {
    ((void(*)(CMenuGCItem*))cbRenderBefore__11CMenuGCItemFv)(
        (CMenuGCItem*)((uintptr_t)self - 0x58));
}
// LLM-HARNESS-END: us-802b3978

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuGCItem.
 *
 * Same adjustment as func_802B0F08 but forwards to the destructor.
 * r4 (the MWCC deletion flag) is preserved from the caller because the cast
 * to void(*)(CMenuGCItem*) declares only one parameter, leaving r4 unmodified
 * in the tail call.
 *
 * Retail: subi r3, r3, 0x58; b __dt__11CMenuGCItemFv
 */
// LLM-HARNESS-BEGIN: us-802b3980
extern "C" void func_802B0F10(IScnRender* self) {
    ((void(*)(CMenuGCItem*))__dt__11CMenuGCItemFv)(
        (CMenuGCItem*)((uintptr_t)self - 0x58));
}
// LLM-HARNESS-END: us-802b3980
