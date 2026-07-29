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

// Helper to access fields of CfObjectEff that aren't yet named in its header.
// Avoids needing the complete CfObjectEff type for matching.
struct CfObjectEffLayout {
    u8 _00[0x68];
    u32 mFlags68;       // 0x68 - CfObject::mFlags68
    u8 _6C[0xB0 - 0x6C];
    u32 field_0xB0;     // 0xB0 - unnamed field in CfObjectEff::_padA8
};

// Forward declarations for callees
void func_800CFFA0(void* self);

// Base type for camera control instances returned by initCamControlInstances.
// Vtable index 4 (offset 0x10) is the method invoked by func_802751F8.
class ICamControlBase {
public:
    virtual ~ICamControlBase();
    virtual void vfunc_04() = 0;
    virtual void vfunc_08() = 0;
    virtual void vfunc_0C() = 0;
    virtual void vfunc_10(class UnkCode8027513C* self) = 0;
};

ICamControlBase* initCamControlInstances();

