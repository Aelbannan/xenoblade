#pragma once

#include <types.h>

/// Forward declaration -- CMenuPTState is defined across multiple TUs
/// (Init/cbRenderBefore/Move/Term in code_8018F8D8.cpp).
class CMenuPTState;

// Vtable adjustor thunks -- when a derived class overrides CMenuPTState
// virtual functions and the vtable slot is at offset +0x58 from the
// CMenuPTState subobject, these thunks adjust 'this' by -0x58 before
// tail-calling the real member function.

extern "C" {
    void func_80192BE4(void* self);
    void func_80192BEC(void* self);
}
