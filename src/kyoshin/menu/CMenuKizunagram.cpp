// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/menu/CMenuKizunagram.hpp"

extern "C" void __dt__15CMenuKizunagramFv(void*, int);
extern "C" void cbRenderBefore__15CMenuKizunagramFv(void*);

void __ct__CMenuKizunagram(){}

CMenuKizunagram::~CMenuKizunagram() {}

void CMenuKizunagram::Init() {}

void CMenuKizunagram::Term() {}

void CMenuKizunagram::Move() {}

void CMenuKizunagram::cbRenderBefore() {}

void func_8025728C(){}

void stub_us_80259544() {}

void func_80257318(){}

void func_80257360(){}

void func_802573B8(){}

void func_80257448(){}

void func_80257498(){}

void func_80257704(){}

void func_80257754(){}

void func_802577F0(){}

void func_80257840(){}

void func_80257994(){}

void func_80257A2C(){}

// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
extern "C" void func_80257A7C(void* self) {
    ((void(*)(void*))cbRenderBefore__15CMenuKizunagramFv)((char*)self - 0x58);
}

// IScnRender vtable adjustor thunk for ~CMenuKizunagram.
extern "C" void func_80257A84(void* self) {
    ((void(*)(void*))__dt__15CMenuKizunagramFv)((char*)self - 0x58);
}

extern unsigned long lbl_eu_806647E0;
extern "C" unsigned long func_80257308(void) { return lbl_eu_806647E0 != 0; }
