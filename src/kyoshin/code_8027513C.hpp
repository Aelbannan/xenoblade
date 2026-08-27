#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// ---------------------------------------------------------------------------
// C-linkage imports
// ---------------------------------------------------------------------------

class CScn;

extern "C" float lbl_eu_806689D8; // -4.0
extern "C" float lbl_eu_806689DC; // 0.9
extern "C" float lbl_eu_806689E0; // 0.0
extern "C" float lbl_eu_806689E4; // 0.01
extern "C" float lbl_eu_806689E8; // 1.0
extern "C" float lbl_eu_806689EC; // 30.0
extern "C" float lbl_eu_806689F0; // 0.001
extern "C" float lbl_eu_806689F4; // 0.1
extern "C" float lbl_eu_806689F8; // 0.4

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

// State-dispatch ptmf table (.data: lbl_eu_80537F98), indexed by field_0x6C.
// Entries: state 0 -> func_80275454, state 1 -> func_802756F0. MWCC lowers
// (self->*table[idx])() to mulli/lis/addi + `bl __ptmf_scall`.
typedef void (UnkCode8027513C::*UnkCode8027513CStateFn)();
extern UnkCode8027513CStateFn lbl_eu_80537F98[2];

// Child-effect state notifier (retail symbol is unmangled C-linkage).
extern "C" void setChildB59__(void* obj, s8 val);

// setChildV40__/bindPartnerO_/func_801BFDE8: retail-unmangled C-linkage
// imports already declared by CfObjectImplMove.hpp.
// Retail calls createNpcActor through the cf::CfGameManager member declaration
// (mangled createNpcActor__Q22cf13CfGameManagerFv) even though the callee
// actually takes a type code in r3 and returns a CfObjectEff*. Declare the
// literal retail symbol with C linkage so the bl reloc name matches.
extern "C" void* createNpcActor__Q22cf13CfGameManagerFv(u32 index);

