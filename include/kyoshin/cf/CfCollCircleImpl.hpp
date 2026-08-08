#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_8049034C(void* a, void* b, int c);
extern "C" void func_800A50AC(void* a, void* b, float c, void* d);
extern "C" void* lbl_eu_80663E14;

namespace cf {

// Collision circle shape implementation.
// Layout offsets derived from CfCollCircleImpl.s.
struct CfCollCircleImpl {
    u8 pad_00[0xB8];    // 0x00: vtable + unknown fields
    float field_B8;     // 0xB8: circle radius/scale field
};

// Cast-only vtable interface.  Omitting the RTTI slots makes the declared
// virtual at +0xAC line up with the retail vtable; MWCC therefore emits the
// same r12 virtual-call sequence instead of a function-pointer temp (the
// manual void** extraction allocates the vtable base to a GPR like r4).
struct CfCollCircleVTableIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual void* _v0AC();  // position/data getter (slot 0xAC/4)
    virtual void _v0B0();
};

} // namespace cf
