// Auto-scaffolded catalog TU for kyoshin/menu/CMenuMapSelectSC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuMapSelectSC.hpp"
#include "kyoshin/harness_catalog.hpp"

void __ct__CMenuMapSelectSC(){}

void CMenuMapSelectSC::~CMenuMapSelectSC(int deletionFlag) const {}

void CMenuMapSelectSC::Init() const {}

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

void CMenuMapSelectSC::Term() const {}

void CMenuMapSelectSC::Move() const {}

void CMenuMapSelectSC::cbRenderBefore() const {}

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
// The thunk converts the subobject pointer (IScnRender*) to the enclosing
// CMenuMapSelectSC* via integer-offset adjustment before forwarding to the
// real override. The function-pointer cast prevents MWCC from null-checking
// the static_cast chain (which would add a cmpwi/beq that the retail thunk
// does not have).
void func_80251DE8(IScnRender* self) {
    ((void(*)(CMenuMapSelectSC*))cbRenderBefore__16CMenuMapSelectSCFv)(
        (CMenuMapSelectSC*)((u32)self - 0x58));
}

// IScnRender vtable this-adjusting thunk for destructor.
// Adjusts from IScnRender* (at offset 0x58) to full CMenuMapSelectSC* and
// forwards to ~CMenuMapSelectSC(). r4 (deletion flag) is preserved from the
// caller because the cast to void(*)(CMenuMapSelectSC*) declares only one
// parameter, leaving r4 untouched.
void func_80251DF0(IScnRender* self) {
    ((void(*)(CMenuMapSelectSC*))__dt__16CMenuMapSelectSCFv)(
        (CMenuMapSelectSC*)((u32)self - 0x58));
}
