#pragma once

#include <types.h>

// Scene ID manager: registers a render callback with CScn.
struct __declspec(novtable) CScnIdMan {
    virtual ~CScnIdMan();
    virtual void cbRenderBefore();

    // +0x00: vtable pointer (auto/compiler-managed)
    void* mParam;    // +0x04
    u8 mFlag;        // +0x08
};
