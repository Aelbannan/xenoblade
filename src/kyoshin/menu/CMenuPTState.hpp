#pragma once

#include <types.h>

// CPartyStateWin.hpp references cf::CfObjectMove before its own forward decl;
// pre-declare it here (same pattern as other includers).
namespace cf { class CfObjectMove; }
#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CPartyStateWin.hpp"

// Vtable adjustor thunks -- when a derived class overrides CMenuPTState
// virtual functions and the vtable slot is at offset +0x58 from the
// CMenuPTState subobject, these thunks adjust 'this' by -0x58 before
// tail-calling the real member function.

// Byte-range shim over the CProcess header + PTMF zone, so the allocating
// factory constructor can write the vtable (+0x10) and callback slots
// (+0x3C..0x53) without raw pointer arithmetic. Layout matches CProcess
// (0x00-0x3B) exactly.
struct CMenuPTStateCtorShim {
    u8 _00[0x10];
    void* vtable;          // 0x10 -- CProcess vtable, overwritten by this ctor
    u8 _14[0x28];          // 0x14-0x3B -- rest of CProcess
    u32 callbacks[6];      // 0x3C-0x53 -- PTMF callback slots (__ptmf_null copies)
    u8 field54;            // 0x54
    u8 field55;            // 0x55
};

// CProcess header + PTMF imports for the retail singleton factory (see .cpp).
class CMenuPTState : public CProcess, public IScnRender {
public:
    virtual ~CMenuPTState();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks.
    void func_80192BE4();
    void func_80192BEC();

    // +0x3C-0x57: compiler-managed vtable/ptmf data
    // +0x58: IScnRender vtable (implicit)
    CProcess* mStoredParent; // 0x5C
    CBgTex mBgTex; // 0x60
    CPartyStateWin mPartyStateWin; // 0x80
    u8 mField_6C6C; // 0x6C6C
};

namespace cf {

class UnkClass_80192BF4 {
public:
    UnkClass_80192BF4();
    void __ct__80192C10();

    // +0x00: counter/state
    u32 field_0x00;
    // +0x04: float timer/accumulator
    f32 field_0x04;
    // +0x08: float value
    f32 field_0x08;
};
} // namespace cf