#ifndef CF_OBJECT_ENE_HPP
#define CF_OBJECT_ENE_HPP

#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/CfResReloadImpl.hpp"

namespace cf {

class CfObjectEne : public CfObjectActor {
public:
    virtual ~CfObjectEne();
    virtual void func_800ADB2C();
    virtual void func_800ADBD4();
    virtual void func_800ADDA8();
    virtual void func_800AEC68();
    void CActorParam_UnkVirtualFunc166();
    void CActorParam_UnkVirtualFunc167();

private:
    // field_0x8 is a secondary vtable pointer inherited from CfObjectActor
    // Multiple inheritance vtables:
    // 0x0: primary vtable
    // 0x8: secondary vtable
    // ...
    // 0x3380: another interface vtable
    // 0x3e9c: another interface vtable

    // member variables:
    u16 field_0x45c0;
    u16 field_0x45c2;
    u16 field_0x45c4;
    u16 field_0x45c6;
    u16 field_0x45c8;
    u16 field_0x45ca;
    f32 field_0x45cc;
    // ...
    CfResReloadImpl* field_0x3f4c;
};

} // namespace cf

#endif
