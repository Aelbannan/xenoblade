// Cleaned TU for kyoshin/menu/CMenuPTState.
// FULL_MATCH: func_80192BE4, func_80192BEC (vtable adjustor thunks).

#include "kyoshin/menu/CMenuPTState.hpp"

// Real member functions defined in code_8018F8D8.cpp -- accessed via their
// mangled linker names (extern "C" suppresses further name decoration).
extern "C" void cbRenderBefore__12CMenuPTStateFv(CMenuPTState* self);
extern "C" void __dt__12CMenuPTStateFv(CMenuPTState* self);

void __ct__CMenuPTState(){}

unsigned long func_80192BD0(){
    unsigned long v = *(unsigned long*)(lbl_eu_80664300);
    return !!v;
}

// FULL_MATCH: vtable adjustor thunks (offset +0x58).
extern "C" void func_80192BE4(void* self) {
    cbRenderBefore__12CMenuPTStateFv(
        reinterpret_cast<CMenuPTState*>(
            reinterpret_cast<char*>(self) - 0x58));
}

extern "C" void func_80192BEC(void* self) {
    __dt__12CMenuPTStateFv(
        reinterpret_cast<CMenuPTState*>(
            reinterpret_cast<char*>(self) - 0x58));
}

// ---------------------------------------------------------------------------
// Placeholder stubs (not yet FULL_MATCH — kept for label/catalog coverage)
// ---------------------------------------------------------------------------

cf::UnkClass_80192BF4::UnkClass_80192BF4() {}

void __ct__80192C10(){}

void func_80192C2C(){}

void func_80192CB0(){}
