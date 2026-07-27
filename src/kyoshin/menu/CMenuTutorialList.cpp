// Auto-scaffolded catalog TU for kyoshin/menu/CMenuTutorialList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/menu/CMenuTutorialList.hpp"
#include <types.h>

void __ct__CMenuTutorialList(){}

void CMenuTutorialList::~CMenuTutorialList() {}

void CMenuTutorialList::Init() {}

void CMenuTutorialList::Term() {}

void CMenuTutorialList::Move() {}

void CMenuTutorialList::cbRenderBefore() {}

void func_802AC494(){}

void func_802AC510(){}

void func_802AC520(){}

void func_802AC598(){}

void func_802AC5E8(){}

void func_802AC8A4(){}

void func_802AC8F4(){}

void func_802AC970(){}

void func_802ACA38(){}

void func_802ACB50(){}

// --- FULL_MATCH thunks ---
// These are IScnRender vtable trampolines.  When CMenuTutorialList inherits from
// IScnRender (which sits at offset +0x58 in the MI layout), calling a virtual
// function through the IScnRender vtable delivers an IScnRender* (pointing at
// the subobject within the full object).  Each thunk adjusts 'this' back by
// -0x58 so the real CMenuTutorialList implementation receives the correct pointer.

class IScnRender;
typedef void (*CbRenderBeforeFunc)(CMenuTutorialList*);
extern "C" void func_802ACBCC(IScnRender* self) {
    ((CbRenderBeforeFunc)cbRenderBefore__17CMenuTutorialListFv)(
        reinterpret_cast<CMenuTutorialList*>(
            reinterpret_cast<char*>(self) - 0x58));
}

typedef void (*DtorFunc)(CMenuTutorialList*);
extern "C" void func_802ACBD4(IScnRender* self) {
    ((DtorFunc)__dt__17CMenuTutorialListFv)(
        reinterpret_cast<CMenuTutorialList*>(
            reinterpret_cast<char*>(self) - 0x58));
}
