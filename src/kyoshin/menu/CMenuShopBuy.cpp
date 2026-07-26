// Auto-scaffolded catalog TU for kyoshin/menu/CMenuShopBuy
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CMenuShopBuy() {}

extern "C" void __dt__12CMenuShopBuyFv(void* self) {}

extern "C" void Init__12CMenuShopBuyFv() {}

extern "C" void func_8018BE74() {}

extern "C" void Term__12CMenuShopBuyFv() {}

extern "C" void Move__12CMenuShopBuyFv() {}

extern "C" void cbRenderBefore__12CMenuShopBuyFv(void* self) {}

extern "C" void func_8018C104() {}

extern "C" void func_8018C180() {}

extern "C" void func_8018C190() {}

extern "C" void func_8018C208() {}

extern "C" void func_8018C258() {}

extern "C" void func_8018C59C() {}

// Adjusting thunk: upcasts from base sub-object (at offset +0x58 within CMenuShopBuy)
// to the full CMenuShopBuy, then tail-calls cbRenderBefore.
// Uses function-pointer cast to prevent MWCC from inlining the (empty stub) callee.
extern "C" void func_8018C5EC(void* self) {
    ((void(*)(void*))cbRenderBefore__12CMenuShopBuyFv)(static_cast<char*>(self) - 0x58);
}

// Adjusting thunk: upcasts from base sub-object (at offset +0x58 within CMenuShopBuy)
// to the full CMenuShopBuy, then tail-calls the destructor.
// Uses function-pointer cast to prevent MWCC from inlining the (empty stub) callee.
extern "C" void func_8018C5F4(void* self) {
    ((void(*)(void*))__dt__12CMenuShopBuyFv)(static_cast<char*>(self) - 0x58);
}
