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

void stub_us_8027490c() {}

void func_80272498(){}

void func_80272510(){}

void func_80272560(){}

void func_80272694(){}

// IScnRender vtable adjustor thunk for cbRenderBefore.
// When IScnRender virtual functions dispatch through IScnRender*,
// 'this' points to the IScnRender subobject at offset +0x58 within
// CMenuKizunaTalkList. This thunk adjusts it back and tail-calls the
// real implementation.
void CMenuKizunaTalkList::func_802726E4() {
    cbRenderBefore();
}

// IScnRender vtable adjustor thunk for ~CMenuKizunaTalkList.
void CMenuKizunaTalkList::func_802726EC() {
    this->~CMenuKizunaTalkList();
}
