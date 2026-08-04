// Auto-scaffolded catalog TU for kyoshin/menu/CMenuGameClear
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuGameClear.hpp"

// forward declarations for scaffold thunk references
extern unsigned long lbl_eu_80664C08;
void cbRenderBefore__14CMenuGameClearFv(void*);
void __dt__14CMenuGameClearFv(void*);

CMenuGameClear::~CMenuGameClear() {}

void CMenuGameClear::Init() {}

void CMenuGameClear::Term() {}

void CMenuGameClear::Move() {}

void CMenuGameClear::cbRenderBefore() {}

void __ct__CMenuGameClear(){}

extern "C" unsigned long func_802B22E0() {
    return lbl_eu_80664C08 != 0;
}

void func_802B22F4(){}

void func_802B2488(){}

void func_802B261C(void* self) { ((void(*)(void*))cbRenderBefore__14CMenuGameClearFv)((char*)self - 0x58); }

void func_802B2624(void* self) { ((void(*)(void*))__dt__14CMenuGameClearFv)((char*)self - 0x58); }

extern "C" bool func_802B262C() { return false; }
