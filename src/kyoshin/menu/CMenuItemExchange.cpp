// Auto-scaffolded catalog TU for kyoshin/menu/CMenuItemExchange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuItemExchange.hpp"

/**
 * Offset from the OC thunk interface (CMenuItemExchangeOC*) to the
 * containing CMenuItemExchange object. The OC system passes a pointer
 * to a sub-object embedded at this offset; thunks subtract this value
 * to recover the real this pointer before delegating.
 */
static const u32 CMENU_ITEM_EXCHANGE_OC_OFFSET = 0x6c;

extern "C" void __ct__CMenuItemExchange() {}

extern "C" void __dt__17CMenuItemExchangeFv(CMenuItemExchange* self) {}

extern "C" void Init__17CMenuItemExchangeFv() {}

extern "C" void func_801BE108() {}

extern "C" void func_801BE16C() {}

extern "C" void func_801BE208() {}

extern "C" void func_801BE590() {}

extern "C" void Term__17CMenuItemExchangeFv() {}

extern "C" void Move__17CMenuItemExchangeFv() {}

extern "C" void cbRenderBefore__17CMenuItemExchangeFv(CMenuItemExchange* self) {}

extern "C" void func_801BEDE0() {}

extern "C" void func_801BEE5C() {}

extern u8 lbl_eu_8066442C;
extern "C" u8 func_801BEE6C() { return lbl_eu_8066442C; }

extern "C" void func_801BEE74() {}

extern "C" void func_801BEEF4() {}

extern "C" void func_801BEF44() {}

extern "C" void func_801BF2E8() {}

extern "C" void func_801BF348() {}

extern "C" void func_801BF414() {}

extern "C" void func_801BF464() {}

extern "C" void func_801BF6A0() {}

extern "C" void func_801BF70C() {}

/**
 * OC thunk for cbRenderBefore. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to cbRenderBefore.
 */
extern "C" void func_801BF75C(CMenuItemExchangeOC* param) {
    ((void(*)(CMenuItemExchange*))cbRenderBefore__17CMenuItemExchangeFv)(
        (CMenuItemExchange*)((u32)param - CMENU_ITEM_EXCHANGE_OC_OFFSET));
}

/**
 * OC thunk for the destructor. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to the real destructor.
 */
extern "C" void func_801BF764(CMenuItemExchangeOC* param) {
    ((void(*)(CMenuItemExchange*))__dt__17CMenuItemExchangeFv)(
        (CMenuItemExchange*)((u32)param - CMENU_ITEM_EXCHANGE_OC_OFFSET));
}
