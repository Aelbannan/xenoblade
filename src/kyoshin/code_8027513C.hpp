#pragma once

#include <types.h>

namespace cf {

class CfObjectImplTbox {
public:
    virtual ~CfObjectImplTbox();

    // TODO: add fields
};

} // namespace cf

class CfObjectEff;

class UnkCode8027513C;

// Camera control instance base type returned by initCamControlInstances.
// With RTTI on, the typeinfo pointer occupies vtable slot 0; the complete
// destructor pair occupies slots 1-2. Subsequent virtual methods start at
// slot 3 (offset 0x0C).
class ICamControlBase {
public:
    virtual ~ICamControlBase();
    virtual void vfunc_04() = 0;
    virtual void vfunc_10(UnkCode8027513C* self) = 0;
    virtual void vfunc_14(UnkCode8027513C* self) = 0;
    virtual void vfunc_18(UnkCode8027513C* self) = 0;
    virtual void vfunc_1C(UnkCode8027513C* self) = 0;
    virtual void vfunc_20(UnkCode8027513C* self) = 0;
    virtual void vfunc_24(UnkCode8027513C* self) = 0;
    virtual void vfunc_28(UnkCode8027513C* self) = 0;
};

ICamControlBase* initCamControlInstances();

// TODO: identify real class name; contains embedded cf::CfObjectImplTbox subobjects
class UnkCode8027513C {
public:
    u8 _00[0x1C];
    int field_0x1C;
    int field_0x20;
    u8 _24[0x74 - 0x24];
    CfObjectEff* field_0x74;
    s16 field_0x78;
};

