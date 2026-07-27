// Auto-scaffolded catalog TU for kyoshin/menu/CMenuShopSell
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuShopSell.hpp"

void __ct__CMenuShopSell(){}

void CMenuShopSell::~CMenuShopSell() {}

void CMenuShopSell::Init() {}

void func_8018B0FC(){}

void func_8018B130(){}

void CMenuShopSell::Term() {}

void CMenuShopSell::Move() {}

void CMenuShopSell::cbRenderBefore() {}

void func_8018B324(){}

void func_8018B398(){}

void func_8018B3A8(){}

void func_8018B420(){}

void func_8018B470(){}

void func_8018B658(){}

// Adjusting thunk: upcasts from secondary base (at offset +0x58 within CMenuShopSell)
// to the full object, then tail-calls cbRenderBefore.
void CMenuShopSell::func_8018B6A8() {
    cbRenderBefore();
}

// Adjusting thunk: upcasts from secondary base (at offset +0x58 within CMenuShopSell)
// to the full object, then tail-calls the destructor.
void CMenuShopSell::func_8018B6B0() {
    this->~CMenuShopSell();
}
