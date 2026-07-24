// Cleaned TU for kyoshin/menu/CMenuPTState.
// FULL_MATCH: func_80192BE4, func_80192BEC (vtable adjustor thunks).

#include "kyoshin/menu/CMenuPTState.hpp"

// Real member functions defined in code_8018F8D8.cpp -- accessed via their
// mangled linker names (extern "C" suppresses further name decoration).
extern "C" void cbRenderBefore__12CMenuPTStateFv(CMenuPTState* self);
extern "C" void __dt__12CMenuPTStateFv(CMenuPTState* self);

// LLM-HARNESS-BEGIN: us-801941d4
extern "C" void __ct__CMenuPTState() {}
// LLM-HARNESS-END: us-801941d4

// LLM-HARNESS-BEGIN: us-801942ec
extern "C" void func_80192BD0() {}
// LLM-HARNESS-END: us-801942ec

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

// LLM-HARNESS-BEGIN: us-80194310
extern "C" void __ct__Q22cf17UnkClass_80192BF4Fv() {}
// LLM-HARNESS-END: us-80194310

// LLM-HARNESS-BEGIN: us-8019432c
extern "C" void __ct__80192C10() {}
// LLM-HARNESS-END: us-8019432c

// LLM-HARNESS-BEGIN: us-80194348
extern "C" void func_80192C2C() {}
// LLM-HARNESS-END: us-80194348

// LLM-HARNESS-BEGIN: us-801943cc
extern "C" void func_80192CB0() {}
// LLM-HARNESS-END: us-801943cc
