// Auto-scaffolded catalog TU for kyoshin/menu/CMenuShopBuy
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuShopBuy.hpp"
void __ct__CMenuShopBuy(){}

CMenuShopBuy::~CMenuShopBuy() {}

void CMenuShopBuy::Init() {}

void func_8018BE74(){}

void CMenuShopBuy::Term() {}

void CMenuShopBuy::Move() {}

void CMenuShopBuy::cbRenderBefore() {}

void func_8018C104(){}

void func_8018C180(){}

void func_8018C190(){}

void func_8018C208(){}

void func_8018C258(){}

void func_8018C59C(){}

// Adjusting thunk: upcasts from base sub-object (at offset +0x58 within CMenuShopBuy)
// to the full CMenuShopBuy, then tail-calls cbRenderBefore.
// Uses function-pointer cast to prevent MWCC from inlining the (empty stub) callee.
void func_8018C5EC(void* self) {
    ((void(*)(void*))cbRenderBefore__12CMenuShopBuyFv)(static_cast<char*>(self) - 0x58);
}

// Adjusting thunk: upcasts from base sub-object (at offset +0x58 within CMenuShopBuy)
// to the full CMenuShopBuy, then tail-calls the destructor.
// Uses function-pointer cast to prevent MWCC from inlining the (empty stub) callee.
void func_8018C5F4(void* self) {
    ((void(*)(void*))__dt__12CMenuShopBuyFv)(static_cast<char*>(self) - 0x58);
}
