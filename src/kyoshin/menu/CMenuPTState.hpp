#pragma once

#include <types.h>
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

// Vtable adjustor thunks -- when a derived class overrides CMenuPTState
// virtual functions and the vtable slot is at offset +0x58 from the
// CMenuPTState subobject, these thunks adjust 'this' by -0x58 before
// tail-calling the real member function.

extern "C" {
    void func_80192BE4(void* self);
    void func_80192BEC(void* self);
}

class CMenuPTState : public CProcess, public IScnRender {
public:
    CMenuPTState();
    virtual ~CMenuPTState();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // TODO: add fields
};

namespace cf {

class UnkClass_80192BF4 {
public:
    UnkClass_80192BF4();

    // TODO: add fields
};
} // namespace cf

