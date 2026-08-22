#pragma once

#include <types.h>
#include "monolib/math/CCol4.hpp"
#include "monolib/math/CRect16.hpp"
#include "monolib/scn/CLight.hpp"
#include <nw4r/g3d/g3d_light.h>

// Retail data symbols (C-linkage names; global scope is not C++-mangled).
extern char lbl_eu_8056E8B8[];     // CVirtualLightObj vtable
extern const f32 lbl_eu_8066AA88;  // default light color components
extern const f32 lbl_eu_8066AA8C;  // "active" sentinel compared in func_8049488C
extern const f32 lbl_eu_8066AA90;  // color scale for quad verts (func_80494D84)
extern const f32 lbl_eu_8066AA98;  // slot color alpha default (func_80495644)
extern const f32 lbl_eu_8066AA9C;  // color -> u8 scale (func_804952C4)
extern double lbl_eu_8066AAA0;     // 0x4330000000000000 int->double bias
extern const f32 lbl_eu_8066AAA8;  // slot color rgb default (func_80495644)
extern const f32 lbl_eu_8066AAAC;  // distance-attenuation brightness (func_804957E4/804958B8)

// Cross-TU catalog imports (CLight.cpp). The retail symbols are unmangled
// free functions, so extern "C" keeps the emitted reloc names byte-identical.
extern "C" void func_804C03A0(u8* self, int value);
extern "C" void func_804C0398(CLight* self, nw4r::g3d::LightObj* lightObj);
extern "C" void func_804C0454(u8* self, int value);
extern "C" void func_804C0484(CLight* self, const f32* dir);
extern "C" void func_804C07F0(u8* self, int value);
extern "C" void func_804C0928(CLight* self, f32 intensity);
extern "C" void func_804C0570(CLight* self, f32 angleX, f32 angleY);
extern "C" void func_804C08C8(CLight* self, int enable);
extern "C" void func_804C0920(CLight* self, float cutoff, _GXSpotFn spotFn);
extern "C" void func_804C09E0(u8* self, float a, float b, int c);

// Post-light-update hook on the scene light data. The catalog header
// (code_804BF59C.hpp) declares it (void), but the retail call site passes the
// data pointer in r3, so declare the real signature here for correct argument
// setup.
class CScnEnvLgtData;
extern "C" void func_804BF940(CScnEnvLgtData* self);
extern "C" void func_804BF8A8(CScnEnvLgtData* self);  // clears the data object's light state

// nw4r g3d G3DState::SetLightObj is missing from the (read-only) g3d_state.h;
// declare it here so calls emit the retail mangled name.
namespace nw4r {
namespace g3d {
namespace G3DState {
void SetLightObj(const LightObj& rLightObj, int idx);
}
}
}

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
    nw4r::g3d::AmbLightObj mAmbLight[0x20];       // +0x880..0x900
    nw4r::g3d::LightSetData mLightSetData[0x4];   // +0x900..0x930
    nw4r::g3d::LightSetting mLightSetting;        // +0x930..0x940
    CLight mSlotLights[4][8];                     // +0x940..0x1140
    CLight* mSlotPtrs[4];                         // +0x1140
    u32 mSlotCounts[4];                           // +0x1150
    u32 mSlotFields[4];                           // +0x1160
    s32 mField1170;                               // +0x1170
    u8 mByte1174;                                 // +0x1174
    u8 mPad1175[3];                               // +0x1175
    u32 mField1178;                               // +0x1178 (nw4r scene handle, set by func_804950F4)
    u32 mField117C;                               // +0x117C
    u32 mField1180;                               // +0x1180
    struct SLightColor { f32 x, y, z, w; } mSlotColors[4];  // +0x1184
};
