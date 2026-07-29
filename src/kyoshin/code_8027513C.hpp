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

