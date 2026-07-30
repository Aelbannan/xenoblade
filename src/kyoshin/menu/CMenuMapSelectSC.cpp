// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelectSC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelectSC.hpp"
#include "kyoshin/harness_catalog.hpp"

void __ct__CMenuMapSelectSC(){}

CMenuMapSelectSC::~CMenuMapSelectSC() {}

void CMenuMapSelectSC::Init() {}

void func_80251294(void* r3, const void* r4) {
    struct S { int dummy; int a; int b; int c; int d; unsigned char e; unsigned char f; };
    S* dst = (S*)r3;
    const S* src = (const S*)r4;
    dst->a = src->a;
    dst->b = src->b;
    dst->c = src->c;
    dst->d = src->d;
    dst->e = src->e;
    dst->f = src->f;
}

void CMenuMapSelectSC::Term() {}

void CMenuMapSelectSC::Move() {}

void CMenuMapSelectSC::cbRenderBefore() {}

void func_802514D4(){}

void func_80251550(){}

void func_80251560(){}

void func_802515B8(){}

void func_80251628(){}

void func_802516DC(){}

void func_80251D4C(){}

void func_80251D94(){}

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMapSelectSC.
void CMenuMapSelectSC::func_80251DE8() {
    cbRenderBefore();
}

// IScnRender vtable this-adjusting thunk for destructor.
void CMenuMapSelectSC::func_80251DF0() {
    this->~CMenuMapSelectSC();
}
