#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"

// Cast-only SI interface.  Omitting the RTTI slots makes the declared
// virtual at +0xAC line up with the retail vtable; MWCC consequently emits
// the same r12 virtual-call sequence instead of a function-pointer temp.
struct CfCollSphereVTableIf {
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
    virtual void* _v0AC();
    virtual void _v0B0();
};

// func_800AAD28: debug draw for sphere collision shape (single call).
extern "C" void func_800AAD28(void* context, cf::CfCollSphereImpl* shape) {
    u32 uval = static_cast<u32>(shape->mRadius);
    void* data = reinterpret_cast<CfCollSphereVTableIf*>(shape)->_v0AC();
    renderSphere__Q22cf18CfDebugDrawManagerFv(data, static_cast<float>(uval));
}

// func_800AAD94: debug draw for sphere with extra transform params.
extern "C" void func_800AAD94(void* context, cf::CfCollSphereImpl* shape, void* a, void* b) {
    u32 uval = static_cast<u32>(shape->mRadius);
    void* data = reinterpret_cast<CfCollSphereVTableIf*>(shape)->_v0AC();
    func_800A5738(a, data, static_cast<float>(uval), b);
}
