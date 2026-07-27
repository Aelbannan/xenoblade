// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/menu/CMenuKizunagram.hpp"

void __ct__CMenuKizunagram(){}

void CMenuKizunagram::~CMenuKizunagram() {}

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

// Forward declarations for member functions called by adjustor thunks
// (used via direct function pointer to guarantee tail-call match).
void cbRenderBefore__15CMenuKizunagramFv(CMenuKizunagram* self);
void __dt__15CMenuKizunagramFv(CMenuKizunagram* self, int deletionFlag);
// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
// CMenuKizunagram. This thunk adjusts it back and tail-calls the
// real implementation.
void func_80257A7C(IScnRender* self) {
    ((void(*)(CMenuKizunagram*))cbRenderBefore__15CMenuKizunagramFv)(
        reinterpret_cast<CMenuKizunagram*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}

// IScnRender vtable adjustor thunk for ~CMenuKizunagram.
// Same pointer adjustment as func_80257A7C. r4 (deletion flag)
// is passed through from the IScnRender vtable caller unchanged.
// The single-arg function pointer cast avoids MWCC zeroing r4.
void func_80257A84(IScnRender* self) {
    ((void(*)(CMenuKizunagram*))__dt__15CMenuKizunagramFv)(
        reinterpret_cast<CMenuKizunagram*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
