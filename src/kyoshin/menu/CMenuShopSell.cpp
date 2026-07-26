// Auto-scaffolded catalog TU for kyoshin/menu/CMenuShopSell
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuShopSell.hpp"

extern "C" void __ct__CMenuShopSell() {}

extern "C" void __dt__13CMenuShopSellFv(void* self) {}

extern "C" void Init__13CMenuShopSellFv() {}

extern "C" void func_8018B0FC() {}

extern "C" void func_8018B130() {}

extern "C" void Term__13CMenuShopSellFv() {}

extern "C" void Move__13CMenuShopSellFv() {}

extern "C" void cbRenderBefore__13CMenuShopSellFv(void* self) {}

extern "C" void func_8018B324() {}

extern "C" void func_8018B398() {}

extern "C" void func_8018B3A8() {}

extern "C" void func_8018B420() {}

extern "C" void func_8018B470() {}

extern "C" void func_8018B658() {}

// Adjusting thunk: upcasts from secondary base (at offset +0x58 within CMenuShopSell)
// to the full object, then tail-calls cbRenderBefore.
// NOTE: Function-pointer call prevents MWCC from inlining the empty stub.
extern "C" void func_8018B6A8(CMenuShopSell* self) {
    ((void(*)(void*))cbRenderBefore__13CMenuShopSellFv)(reinterpret_cast<char*>(self) - 0x58);
}

// Adjusting thunk: upcasts from secondary base (at offset +0x58 within CMenuShopSell)
// to the full object, then tail-calls the destructor.
// NOTE: Function-pointer call prevents MWCC from inlining the empty stub.
extern "C" void func_8018B6B0(CMenuShopSell* self) {
    ((void(*)(void*))__dt__13CMenuShopSellFv)(reinterpret_cast<char*>(self) - 0x58);
}
