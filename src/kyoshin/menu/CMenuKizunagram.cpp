// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunagram
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/menu/CMenuKizunagram.hpp"

extern "C" void __ct__CMenuKizunagram() {}

extern "C" void __dt__15CMenuKizunagramFv(void* self) {}

extern "C" void Init__15CMenuKizunagramFv() {}

extern "C" void Term__15CMenuKizunagramFv() {}

extern "C" void Move__15CMenuKizunagramFv() {}

extern "C" void cbRenderBefore__15CMenuKizunagramFv(void* self) {}

extern "C" void func_8025728C() {}

extern "C" void stub_us_80259544() {}

extern "C" void func_80257318() {}

extern "C" void func_80257360() {}

extern "C" void func_802573B8() {}

extern "C" void func_80257448() {}

extern "C" void func_80257498() {}

extern "C" void func_80257704() {}

extern "C" void func_80257754() {}

extern "C" void func_802577F0() {}

extern "C" void func_80257840() {}

extern "C" void func_80257994() {}

extern "C" void func_80257A2C() {}

// Forward declarations for member functions called by adjustor thunks
// (used via direct function pointer to guarantee tail-call match).
extern "C" void cbRenderBefore__15CMenuKizunagramFv(CMenuKizunagram* self);
extern "C" void __dt__15CMenuKizunagramFv(CMenuKizunagram* self, int deletionFlag);

// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
// CMenuKizunagram. This thunk adjusts it back and tail-calls the
// real implementation.
extern "C" void func_80257A7C(IScnRender* self) {
    ((void(*)(CMenuKizunagram*))cbRenderBefore__15CMenuKizunagramFv)(
        reinterpret_cast<CMenuKizunagram*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}

// IScnRender vtable adjustor thunk for ~CMenuKizunagram.
// Same pointer adjustment as func_80257A7C. r4 (deletion flag)
// is passed through from the IScnRender vtable caller unchanged.
// The single-arg function pointer cast avoids MWCC zeroing r4.
extern "C" void func_80257A84(IScnRender* self) {
    ((void(*)(CMenuKizunagram*))__dt__15CMenuKizunagramFv)(
        reinterpret_cast<CMenuKizunagram*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
