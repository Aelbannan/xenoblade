// Auto-scaffolded catalog TU for kyoshin/menu/CMenuKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuKizunaTalkList.hpp"

void __ct__CMenuKizunaTalkList(){}

void CMenuKizunaTalkList::~CMenuKizunaTalkList(int deletionFlag) {}

void CMenuKizunaTalkList::Init() {}

void CMenuKizunaTalkList::Term() {}

void CMenuKizunaTalkList::Move() {}

void CMenuKizunaTalkList::cbRenderBefore() {}

void func_80272414(){}

extern "C" void stub_us_8027490c() {}

void func_80272498(){}

void func_80272510(){}

void func_80272560(){}

void func_80272694(){}

// Forward declarations for member functions called by adjustor thunks
// (used via direct function pointer to guarantee tail-call match).
void cbRenderBefore__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self);
void __dt__19CMenuKizunaTalkListFv(CMenuKizunaTalkList* self, int deletionFlag);
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
