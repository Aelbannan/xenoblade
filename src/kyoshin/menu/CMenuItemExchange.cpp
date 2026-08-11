// Auto-scaffolded catalog TU for kyoshin/menu/CMenuItemExchange
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuItemExchange.hpp"

extern "C" void __dt__17CMenuItemExchangeFv(void*, int);
extern "C" void cbRenderBefore__17CMenuItemExchangeFv(void*);

/**
 * Offset from the OC thunk interface (CMenuItemExchangeOC*) to the
 * containing CMenuItemExchange object. The OC system passes a pointer
 * to a sub-object embedded at this offset; thunks subtract this value
 * to recover the real this pointer before delegating.
 */
static const u32 CMENU_ITEM_EXCHANGE_OC_OFFSET = 0x6c;

void __ct__CMenuItemExchange(){}

CMenuItemExchange::~CMenuItemExchange() {}

void CMenuItemExchange::Init() {}

void func_801BE108(){}

void func_801BE16C(){}

void func_801BE208(){}

void func_801BE590(){}

void CMenuItemExchange::Term() {}

void CMenuItemExchange::Move() {}

void CMenuItemExchange::cbRenderBefore() {}

void func_801BEDE0(){}

// (lbl_eu_80664428 != 0)
extern "C" bool func_801BEE5C() { extern u32 lbl_eu_80664428; return lbl_eu_80664428 != 0; }

extern u8 lbl_eu_8066442C;
u8 func_801BEE6C() { return lbl_eu_8066442C; }

void func_801BEE74(){}

void func_801BEEF4(){}

void func_801BEF44(){}

void func_801BF2E8(){}

void func_801BF348(){}

void func_801BF414(){}

void func_801BF464(){}

void func_801BF6A0(){}

void func_801BF70C(){}

/**
 * OC thunk for cbRenderBefore. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to cbRenderBefore.
 */
extern "C" void func_801BF75C(CMenuItemExchangeOC* param) {
    ((void(*)(void*))cbRenderBefore__17CMenuItemExchangeFv)((char*)param - 0x6c);
}

/**
 * OC thunk for the destructor. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to the real destructor.
 */
extern "C" void func_801BF764(CMenuItemExchangeOC* param) {
    ((void(*)(void*))__dt__17CMenuItemExchangeFv)((char*)param - 0x6c);
}
