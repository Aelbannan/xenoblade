// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuKizunaTalkList.hpp"

extern "C" void __ct__CMenuKizunaTalkList() {}

extern "C" void __dt__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self, int deletionFlag) {}

extern "C" void Init__19CMenuKizunaTalkListFv() {}

extern "C" void Term__19CMenuKizunaTalkListFv() {}

extern "C" void Move__19CMenuKizunaTalkListFv() {}

extern "C" void cbRenderBefore__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self) {}

extern "C" void func_80272414() {}

extern "C" void stub_us_8027490c() {}

extern "C" void func_80272498() {}

extern "C" void func_80272510() {}

extern "C" void func_80272560() {}

extern "C" void func_80272694() {}

// Forward declarations for member functions called by adjustor thunks
// (used via direct function pointer to guarantee tail-call match).
extern "C" void cbRenderBefore__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self);
extern "C" void __dt__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self, int deletionFlag);

// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
// CMenuKizunaTalkList. This thunk adjusts it back and tail-calls the
// real implementation.
extern "C" void func_802726E4(IScnRender* self) {
    ((void(*)(CMenuKizunaTalkList*))cbRenderBefore__19CMenuKizunaTalkListFv)(
        reinterpret_cast<CMenuKizunaTalkList*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}

// IScnRender vtable adjustor thunk for ~CMenuKizunaTalkList.
// Same pointer adjustment as func_802726E4. r4 (deletion flag)
// is passed through from the IScnRender vtable caller unchanged.
// The single-arg function pointer cast avoids MWCC zeroing r4.
extern "C" void func_802726EC(IScnRender* self) {
    ((void(*)(CMenuKizunaTalkList*))__dt__19CMenuKizunaTalkListFv)(
        reinterpret_cast<CMenuKizunaTalkList*>(
            reinterpret_cast<uintptr_t>(self) - 0x58));
}
