#pragma once

#include <types.h>

namespace cf {

class CfObjectImplTbox {
public:
    virtual ~CfObjectImplTbox();

    // TODO: add fields
};

class CfObjectMove;
class CfObjectEff;

} // namespace cf

// Target data struct pointed to by CfObjectMove::mTargetC4.
// Real name unknown; used by func_80275454.
struct UnkTargetData {
    u8 _00[0xC];
    u32 field_0xC;           // bit flags
    u8 _10[0x3C4 - 0x10];
    float field_0x3C4;
    u8 _3C8[0x4EC - 0x3C8];
    u32 field_0x4EC;          // bit flags
    u8 _4F0[0x4F8 - 0x4F0];
    float field_0x4F8;
    u8 _4FC[0x504 - 0x4FC];
    float field_0x504;
    u8 _508[0x50C - 0x508];
    float field_0x50C;
};

// Position container pointed to by CfObjectModel::field_0x90[8].
// Real name unknown; contains world-space coordinates for effects.
struct UnkPosContainer {
    u8 _000[0x760];
    float posX;    // 0x760
    float posY;    // 0x764
    float posZ;    // 0x768
};

// Camera/scene settings object returned by func_8049603C.
// Real name unknown; float at 0xC read by func_80275454.
struct UnkCamObj {
    u8 _00[0xC];
    float field_0xC;
};

// Layout wrapper for CfObjectEff to access field at offset 0xB0
// without pointer arithmetic. Real field name unknown.
struct CfObjectEffB0 {
    u8 _00[0xB0];
    u8* field_0xB0;
};

// TODO: identify real class name; contains embedded cf::CfObjectImplTbox subobjects
class UnkCode8027513C {
public:
    u8 _00[0x14];
    cf::CfObjectMove* field_0x14;   // pointer to CfObjectMove-derived instance
    u8 _18[0x1C - 0x18];
    int field_0x1C;
    int field_0x20;
    u8 _24[0x6C - 0x24];
    u32 field_0x6C;                 // state counter for dispatch logic
    u8 _70[0x74 - 0x70];
    cf::CfObjectEff* field_0x74;
    s16 field_0x78;
};

