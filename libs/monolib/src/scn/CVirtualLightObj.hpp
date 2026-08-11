#pragma once

#include <types.h>
#include "monolib/math/CCol4.hpp"
#include "monolib/scn/CLight.hpp"
#include <nw4r/g3d/g3d_light.h>

// Retail data symbols (C-linkage names; global scope is not C++-mangled).
extern char lbl_eu_8056E8B8[];     // CVirtualLightObj vtable
extern const f32 lbl_eu_8066AA88;  // default light color components
extern const f32 lbl_eu_8066AA8C;  // "active" sentinel compared in func_8049488C
extern const f32 lbl_eu_8066AA98;  // slot color alpha default (func_80495644)
extern const f32 lbl_eu_8066AAA8;  // slot color rgb default (func_80495644)

// Cross-TU catalog import: stores `value` into CLight::mpLightObj (+0x2C).
extern "C" void func_804C03A0(u8* self, int value);

class CVirtualLightObj {
public:
    CVirtualLightObj();

    u8* mVtable;      // +0x00: vtable pointer (stored manually from lbl_eu_8056E8B8)
    f32 mField04;     // +0x04
    f32 mField08;     // +0x08
    f32 mField0C;     // +0x0C
    f32 mField10;     // +0x10
    f32 mField14;     // +0x14
    u8 mByte18;       // +0x18
    u8 mByte19;       // +0x19
    f32 mField1C;     // +0x1C
    f32 mField20;     // +0x20
    f32 mField24;     // +0x24
    f32 mField28;     // +0x28
    s32 mField2C;     // +0x2C
    s32 mField30;     // +0x30
    u8 _34[0x1140];   // legacy catalog padding (func_804954AC touches +0x1174)
    u8 mUnk1174;      // +0x1174
};

// Light environment owned by CScnItemLight (+0x14): 32 nw4r GX light objects
// (0x44 each), four CLight[8] banks, and per-slot bookkeeping. Constructed by
// func_804950F4, destroyed by the catalog destructor __dt__80495200.
class CLightEnv {
public:
    nw4r::g3d::LightObj mGxLights[0x20];          // +0x000..0x880
    u8 mPad880[0xc0];                             // +0x880..0x940
    CLight mSlotLights[4][8];                     // +0x940..0x1140
    CLight* mSlotPtrs[4];                         // +0x1140
    u32 mSlotCounts[4];                           // +0x1150
    u32 mSlotFields[4];                           // +0x1160
    s32 mField1170;                               // +0x1170
    u8 mByte1174;                                 // +0x1174
    u8 mPad1175[0x7];                             // +0x1175
    u32 mField117C;                               // +0x117C
    u32 mField1180;                               // +0x1180
    struct SLightColor { f32 x, y, z, w; } mSlotColors[4];  // +0x1184
};
