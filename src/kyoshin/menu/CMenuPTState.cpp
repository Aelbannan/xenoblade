// Cleaned TU for kyoshin/menu/CMenuPTState.
// FULL_MATCH: func_80192BE4, func_80192BEC (vtable adjustor thunks).

#include "kyoshin/menu/CMenuPTState.hpp"
extern f32 lbl_eu_80664300;

// Real member functions defined in code_8018F8D8.cpp -- accessed via their
// mangled linker names (suppresses further name decoration).
void cbRenderBefore__12CMenuPTStateFv(CMenuPTState* self);
void __dt__12CMenuPTStateFv(CMenuPTState* self);

void __ct__CMenuPTState(){}

unsigned long func_80192BD0(){
    unsigned long v = *(unsigned long*)(lbl_eu_80664300);
    return !!v;
}

// FULL_MATCH: vtable adjustor thunks (IScnRender secondary base at offset +0x58).
void func_80192BE4(void* self) {
    cbRenderBefore__12CMenuPTStateFv(
        static_cast<CMenuPTState*>(reinterpret_cast<IScnRender*>(self)));
}

void func_80192BEC(void* self) {
    __dt__12CMenuPTStateFv(
        static_cast<CMenuPTState*>(reinterpret_cast<IScnRender*>(self)));
}

// ---------------------------------------------------------------------------
// Placeholder stubs (not yet FULL_MATCH — kept for label/catalog coverage)
// ---------------------------------------------------------------------------

cf::UnkClass_80192BF4::UnkClass_80192BF4() {}

void __ct__80192C10(){}

void func_80192C2C(){}

void func_80192CB0(){}
