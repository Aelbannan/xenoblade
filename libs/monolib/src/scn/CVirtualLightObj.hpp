#pragma once

#include <types.h>
#include "monolib/math/CCol4.hpp"
#include "monolib/math/CRect16.hpp"
#include "monolib/scn/CLight.hpp"
#include "monolib/scn/CVirtualLightObj.hpp"
#include <nw4r/g3d/g3d_light.h>

// Retail data symbols (C-linkage names; global scope is not C++-mangled).
extern const f32 lbl_eu_8066AA90;  // color scale for quad verts (drawFlatQuad)
extern const f32 lbl_eu_8066AA98;  // slot color alpha default (selectLightSlot)
extern const f32 lbl_eu_8066AA9C;  // color -> u8 scale (blendSlotAmbient)
extern double lbl_eu_8066AAA0;     // 0x4330000000000000 int->double bias
extern const f32 lbl_eu_8066AAA8;  // slot color rgb default (selectLightSlot)
extern const f32 lbl_eu_8066AAAC;  // distance-attenuation brightness (bindAttenuatedLight/804958B8)

// Cross-TU catalog imports (CLight.cpp). The retail symbols are unmangled
// free functions, so extern "C" keeps the emitted reloc names byte-identical.
extern "C" void LightCtlSetAttnMode(u8* self, int value);
extern "C" void LightCtlSetPos(u8* self, int value);
extern "C" void LightCtlSetDir(CLight* self, const f32* dir);
extern "C" void LightCtlSetColor(u8* self, int value);
extern "C" void LightCtlScaleColor(CLight* self, f32 intensity);
extern "C" void LightCtlSetDirFromAngle(CLight* self, f32 angleX, f32 angleY);
extern "C" void LightCtlInitDistAttn(u8* self, float a, float b, int c);

// Post-light-update hook on the scene light data. The catalog header
// (code_804BF59C.hpp) declares it (void), but the retail call site passes the
// data pointer in r3, so declare the real signature here for correct argument
// setup.
class CScnEnvLgtData;
extern "C" void ScnEnvLgt_noopF940(CScnEnvLgtData* self);
// (memberized: ScnEnvLgt_ClearLights -> CScnEnvLgtData::ClearLights; declaration moved into class CScnEnvLgtData) // clears the data object's light state

// nw4r g3d G3DState::SetLightObj is missing from the (read-only) g3d_state.h;
// declare it here so calls emit the retail mangled name.
namespace nw4r {
namespace g3d {
namespace G3DState {
void SetLightObj(const LightObj& rLightObj, int idx);
}
}
}


// Light environment owned by CScnItemLight (+0x14): 32 nw4r GX light objects
// (0x44 each), four CLight[8] banks, and per-slot bookkeeping. Constructed by
// constructLightEnv, destroyed by the catalog destructor __dt__80495200.
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
    u32 mField1178;                               // +0x1178 (nw4r scene handle, set by constructLightEnv)
    u32 mField117C;                               // +0x117C
    u32 mField1180;                               // +0x1180
    struct SLightColor { f32 x, y, z, w; } mSlotColors[4];  // +0x1184
};
