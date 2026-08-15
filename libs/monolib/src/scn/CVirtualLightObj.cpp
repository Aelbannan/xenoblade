// Auto-scaffolded catalog TU for monolib/src/scn/CVirtualLightObj
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>
#include <string.h>
#include "libs/monolib/src/scn/CVirtualLightObj.hpp"
// The catalog header declares the post-update hook func_804BF940 as (void),
// which conflicts with the real signature declared in CVirtualLightObj.hpp
// (the retail call site passes the data pointer in r3). Rename the catalog's
// declaration out of the way so only the correct one is visible.
#define func_804BF940 func_804BF940_catalog_unused
#include "monolib/scn/code_804BF59C.hpp"
#undef func_804BF940

// Sets byte at offset 0x1174.
extern "C" void func_804954AC(CVirtualLightObj* self, u8 val) {
    self->mUnk1174 = val;
}

// ---------------------------------------------------------------------------
// CVirtualLightObj constructor: default light state - white color, unit
// intensity, disabled slot index (-1). The retail vtable is an unnamed data
// symbol (lbl_eu_8056E8B8), so the pointer is stored manually instead of
// going through the compiler's vtable machinery.
// ---------------------------------------------------------------------------
CVirtualLightObj::CVirtualLightObj() {
    mVtable = (u8*)lbl_eu_8056E8B8;
    mField04 = lbl_eu_8066AA88;
    mField08 = lbl_eu_8066AA88;
    mField0C = lbl_eu_8066AA88;
    mField10 = lbl_eu_8066AA88;
    mField14 = lbl_eu_8066AA88;
    mByte18 = 1;
    mByte19 = 0;
    mField1C = lbl_eu_8066AA88;
    mField20 = lbl_eu_8066AA88;
    mField24 = lbl_eu_8066AA88;
    mField28 = lbl_eu_8066AA88;
    mField2C = 0;
    mField30 = -1;
}

// ---------------------------------------------------------------------------
// Query whether this virtual light still carries its default (white) state.
// Returns the "default" sentinel while the color fields are untouched, and
// the "white" sentinel once any field has been customized.
// ---------------------------------------------------------------------------
extern "C" f32 func_8049488C(CVirtualLightObj* self) {
    if (self->mField14 <= lbl_eu_8066AA8C) {
        return lbl_eu_8066AA8C;
    }
    if (lbl_eu_8066AA8C == self->mField1C &&
        lbl_eu_8066AA8C == self->mField20 &&
        lbl_eu_8066AA8C == self->mField24) {
        return lbl_eu_8066AA8C;
    }
    if (lbl_eu_8066AA8C == self->mField04 &&
        lbl_eu_8066AA8C == self->mField08 &&
        lbl_eu_8066AA8C == self->mField0C) {
        return lbl_eu_8066AA8C;
    }
    return lbl_eu_8066AA88;
}

// ---------------------------------------------------------------------------
// Resolve the virtual light's current color into out. Lights whose type byte
// is 0 use the static black color; otherwise the two color triples (ambient
// at +0x04, diffuse at +0x1C) are scaled by the intensity at +0x14 and the
// alpha is the product of +0x10 and +0x28.
// ---------------------------------------------------------------------------
extern "C" void func_804947EC(ml::CCol4* out, CVirtualLightObj* in) {
    if (in->mByte18 == 0) {
        *out = ml::CCol4::black;
    } else {
        *out = ml::CCol4(in->mField04 * in->mField1C * in->mField14,
                         in->mField08 * in->mField20 * in->mField14,
                         in->mField0C * in->mField24 * in->mField14,
                         in->mField10 * in->mField28);
    }
}

void func_804952C4() {}

// Post-construction light-environment setup (retail 0x190-byte function, not
// yet matched; kept out-of-line so func_804950F4 emits the retail call).
// ---------------------------------------------------------------------------
// func_804954B4: post-construction light-environment setup.
// Initializes the four slot colors (white rgb, 0.5 alpha), wires the slot
// pointer banks to the four CLight[8] arrays, sets each slot's light count to
// 8, zeroes the slot field counters, then for every slot selects its lights
// into the LightSetting's LightObj array: each LightObj is cleared and bound
// to the slot's CLight, and the slot's ambient light is selected.
// ---------------------------------------------------------------------------
extern "C" __declspec(noinline) void func_804954B4(CLightEnv* self) {
    self->mField1180 = 0;
    for (int i = 0; i < 4; i++) {
        self->mSlotColors[i].x = lbl_eu_8066AAA8;
        self->mSlotColors[i].y = lbl_eu_8066AAA8;
        self->mSlotColors[i].z = lbl_eu_8066AAA8;
        self->mSlotColors[i].w = lbl_eu_8066AA98;
    }
    self->mSlotPtrs[0] = self->mSlotLights[0];
    self->mSlotPtrs[1] = self->mSlotLights[1];
    self->mSlotPtrs[2] = self->mSlotLights[2];
    self->mSlotPtrs[3] = self->mSlotLights[3];
    self->mSlotCounts[0] = 8;
    self->mSlotCounts[1] = 8;
    self->mSlotCounts[2] = 8;
    self->mSlotCounts[3] = 8;
    memset(self->mSlotFields, 0, sizeof(self->mSlotFields));

    CLight* light;
    int lightIdx = 0;
    for (int slot = 0; slot < 4; slot++) {
        nw4r::g3d::LightSet lightSet = self->mLightSetting.GetLightSet(slot);
        light = self->mSlotPtrs[slot];
        for (u32 i = 0; i < self->mSlotCounts[slot]; i++) {
            lightSet.SelectLightObj(i, lightIdx);
            lightSet.GetLightObj(i)->Clear();
            func_804C0398(light, lightSet.GetLightObj(i));
            lightIdx++;
            light++;
        }
        lightSet.SelectAmbLightObj(slot);
        self->mField117C = -1;
    }
}


// ---------------------------------------------------------------------------
// Select light slot `idx` on the environment: reset the slot color, clear
// every CLight referenced by the slot's bank, reset the slot state, and (for
// the first slot) release the shared "current" index.
// ---------------------------------------------------------------------------
extern "C" void func_80495644(CLightEnv* self, u32 idx) {
    self->mField1180 = 0;
    self->mSlotColors[idx].x = lbl_eu_8066AAA8;
    self->mSlotColors[idx].y = lbl_eu_8066AAA8;
    self->mSlotColors[idx].z = lbl_eu_8066AAA8;
    self->mSlotColors[idx].w = lbl_eu_8066AA98;
    for (s32 i = 0; i < (s32)self->mSlotCounts[idx]; i++) {
        func_804C03A0(reinterpret_cast<u8*>(&self->mSlotPtrs[idx][i]), 0);
    }
    self->mSlotFields[idx] = 0;
    if (idx == 0) {
        self->mField1170 = -1;
    }
    self->mField117C = idx;
}

extern "C" void func_804956F8(void* self) { *(u32*)((u8*)self + 0x117C) = -1; }

// ---------------------------------------------------------------------------
// func_80495704: bind the slot's next light, orient it and arm slot 0.
// Resets the current slot's next light (func_804C03A0 with 1), binds a
// color, orients it from two Euler angles, applies the slot enable byte,
// then (for slot 0 with no armed light) records the light index in
// mField1170 and bumps the slot's field counter.
// ---------------------------------------------------------------------------
extern "C" void func_80495704(CLightEnv* self, u32 a, f32 f1, f32 f2) {
    u32 n = self->mSlotFields[self->mField117C];
    CLight* light = self->mSlotPtrs[self->mField117C] + n;
    func_804C03A0(reinterpret_cast<u8*>(light), 1);
    func_804C07F0(reinterpret_cast<u8*>(light), a);
    func_804C0570(light, f1, f2);
    func_804C08C8(light, self->mByte1174);
    if (self->mField117C == 0 && self->mField1170 < 0) {
        self->mField1170 = n;
    }
    self->mSlotFields[self->mField117C]++;
}

// ---------------------------------------------------------------------------
// func_804957E4: bind the slot's next light, color it and enable it.
// Resets the light (func_804C03A0 with 3), binds a color, binds a second
// color, sets distance attenuation with the given brightness, applies the
// slot enable byte and bumps the slot's field counter.
// ---------------------------------------------------------------------------
extern "C" void func_804957E4(CLightEnv* self, u32 a, u32 b, f32 f) {
    u32 n = self->mSlotFields[self->mField117C];
    CLight* light = self->mSlotPtrs[self->mField117C] + n;
    func_804C03A0(reinterpret_cast<u8*>(light), 3);
    func_804C07F0(reinterpret_cast<u8*>(light), a);
    func_804C0454(reinterpret_cast<u8*>(light), b);
    func_804C09E0(reinterpret_cast<u8*>(light), f, lbl_eu_8066AAAC, 2);
    func_804C08C8(light, self->mByte1174);
    self->mSlotFields[self->mField117C]++;
}

// ---------------------------------------------------------------------------
// func_804958B8: full light setup for the slot's next light: reset, color,
// direction, second color, distance attenuation, spot cutoff and enable.
// ---------------------------------------------------------------------------
extern "C" void func_804958B8(CLightEnv* self, u32 a, u32 b, f32 f1, f32 f2,
                              f32 f3, f32 f4) {
    u32 n = self->mSlotFields[self->mField117C];
    CLight* light = self->mSlotPtrs[self->mField117C] + n;
    func_804C03A0(reinterpret_cast<u8*>(light), 4);
    func_804C07F0(reinterpret_cast<u8*>(light), a);
    func_804C0570(light, f1, f2);
    func_804C0454(reinterpret_cast<u8*>(light), b);
    func_804C09E0(reinterpret_cast<u8*>(light), f3, lbl_eu_8066AAAC, 2);
    func_804C0920(light, f4, (_GXSpotFn)2);
    func_804C08C8(light, self->mByte1174);
    self->mSlotFields[self->mField117C]++;
}

// ---------------------------------------------------------------------------
// func_804959E8: push this light set's lights into G3DState.
// The per-slot light range starts at the sum of the slot counts below `idx`
// (constant-trip loop, unrolled by MWCC into the retail flat beq chain); each
// light in the range is uploaded with G3DState::SetLightObj, then the ambient
// light selected by `idx` (via the LightSetting's LightSet) is uploaded with
// SetAmbLightObj.
// ---------------------------------------------------------------------------
extern "C" void func_804959E8(CLightEnv* self, int idx) {
    nw4r::g3d::LightObj* pLight;
    u32 bound = self->mSlotCounts[idx];
    u32 count = 0;
    for (int k = 0; k < 4; k++) {
        if (idx == k) {
            break;
        }
        count += self->mSlotCounts[k];
    }
    nw4r::g3d::G3DState::Invalidate(0x200);
    pLight = &self->mGxLights[count];
    int lightIdx = count;
    for (int i = 0; i < (int)bound; i++) {
        nw4r::g3d::G3DState::SetLightObj(*pLight, lightIdx);
        pLight++;
        lightIdx++;
    }
    nw4r::g3d::LightSet lightSet = self->mLightSetting.GetLightSet(idx);
    nw4r::g3d::AmbLightObj* amb = lightSet.GetAmbLightObj();
    nw4r::g3d::G3DState::SetAmbLightObj(*amb, idx);
}

// ---------------------------------------------------------------------------
// func_80495AF4: push the virtual light environment's slot `slot` into the
// scene light data object.
// Clears the data object's light state, copies the slot's ambient color into
// the data object's ambient color base, then walks the 8 GX lights of the
// slot's LightSet. For each enabled light, copies color/intensity/direction
// from the environment's slot light bank into the data object's CLight and
// enables it. The number of enabled lights is recorded in mActiveLightCount.
// ---------------------------------------------------------------------------
extern "C" void func_80495AF4(CLightEnv* env, CScnEnvLgtData* data, int slot) {
    func_804BF8A8(data);
    data->mAmbColorBase[0] = env->mSlotColors[slot].x;
    data->mAmbColorBase[1] = env->mSlotColors[slot].y;
    data->mAmbColorBase[2] = env->mSlotColors[slot].z;

    nw4r::g3d::LightSet lightSet = env->mLightSetting.GetLightSet(slot);
    CLight* dst = data->mLights;
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (lightSet.GetLightObj(i)->IsEnable()) {
            func_804C03A0(reinterpret_cast<u8*>(dst), 1);
            CLight* src = env->mSlotPtrs[slot] + i;
            func_804C07F0(reinterpret_cast<u8*>(dst), reinterpret_cast<int>(&src->unk10));
            func_804C0928(dst, src->unk38);
            func_804C0484(dst, &src->unk20);
            func_804C08C8(dst, 1);
            dst++;
            count++;
        }
    }
    func_804BF940(data);
    data->mActiveLightCount = count;
}

// ---------------------------------------------------------------------------
// func_804948F4: configure the GX vertex description / attribute formats for
// the given vertex format. Always enables POS and CLR0 as direct data; mode 1
// additionally enables TEX0, mode 2 additionally enables TEX0 and TEX1.
// ---------------------------------------------------------------------------
extern "C" void func_804948F4(GXVtxFmt fmt, int mode) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    switch (mode) {
    case 0:
        GXSetVtxAttrFmt(fmt, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(fmt, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        break;
    case 1:
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(fmt, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(fmt, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxAttrFmt(fmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        break;
    case 2:
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
        GXSetVtxAttrFmt(fmt, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(fmt, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxAttrFmt(fmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(fmt, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
        break;
    }
}
// ---------------------------------------------------------------------------
// func_80494A64: configure a TEV stage's color combine op and inputs for one
// of seven blend modes. All modes use ADD with zero bias/scale, clamped; only
// the color input register combination differs per mode.
// ---------------------------------------------------------------------------
extern "C" void func_80494A64(GXTevStageID stage, int mode, GXTevRegID reg) {
    switch (mode) {
    case 0:
        GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        break;
    case 1:
        GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        break;
    case 2:
        GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
        break;
    case 3:
        GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_RASC, GX_CC_RASA, GX_CC_ZERO);
        break;
    case 4:
        GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_RASA, GX_CC_C0, GX_CC_ZERO);
        break;
    case 5:
        GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
        break;
    case 6:
        GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
        break;
    }
}
// ---------------------------------------------------------------------------
// func_80494C30: configure a TEV stage's alpha combine op and inputs for one
// of five blend modes. All modes use ADD with zero bias/scale, clamped; only
// the alpha input register combination differs per mode.
// ---------------------------------------------------------------------------
extern "C" void func_80494C30(GXTevStageID stage, int mode, GXTevRegID reg) {
    switch (mode) {
    case 0:
        GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
        break;
    case 1:
        GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        break;
    case 2:
        GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
        break;
    case 3:
        GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        break;
    case 4:
        GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, reg);
        GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
        break;
    }
}
// ---------------------------------------------------------------------------
// func_80494D84: draw a flat quad (4 vertices, fmt 0) covering the rect
// (x, y, w, h) at z = 0, colored with the scaled RGBA color. Position is
// written as s16 triples and color as 4 u8 bytes into the GX FIFO pipe.
// ---------------------------------------------------------------------------
extern "C" void func_80494D84(const ml::CRect16* rect, const ml::CCol4* color) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    const f32 scale = lbl_eu_8066AA90;
    s16 y2 = rect->mPos.y + rect->mSize.y;
    GXPosition3s16(rect->mPos.x, rect->mPos.y, 0);
    GXColor4u8((u8)(scale * color->r), (u8)(scale * color->g), (u8)(scale * color->b),
               (u8)(scale * color->a));
    GXPosition3s16(rect->mPos.x + rect->mSize.x, rect->mPos.y, 0);
    GXColor4u8((u8)(scale * color->r), (u8)(scale * color->g), (u8)(scale * color->b),
               (u8)(scale * color->a));
    GXPosition3s16(rect->mPos.x + rect->mSize.x, y2, 0);
    GXColor4u8((u8)(scale * color->r), (u8)(scale * color->g), (u8)(scale * color->b),
               (u8)(scale * color->a));
    GXPosition3s16(rect->mPos.x, y2, 0);
    GXColor4u8((u8)(scale * color->r), (u8)(scale * color->g), (u8)(scale * color->b),
               (u8)(scale * color->a));
}
// ---------------------------------------------------------------------------
// func_80494F10: draw a textured quad (4 vertices, fmt 0) covering the rect
// (x, y, w, h) at z = 0, colored with the scaled RGBA color and UVs taken
// from the four texture coordinates (u0, v0, u1, v1).
// ---------------------------------------------------------------------------
extern "C" void func_80494F10(const ml::CRect16* rect, const ml::CCol4* color, const f32 tex[4]) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    const f32 scale = lbl_eu_8066AA90;
    // Per-vertex texture coordinates: (u0,v0), (u1,v0), (u1,v1), (u0,v1).
    // MWCC materializes this array on the stack (retail keeps the dead stores)
    // while writing the FIFO from the register copies.
    f32 uv[8];
    uv[0] = tex[0];
    uv[1] = tex[1];
    uv[2] = tex[2];
    uv[3] = tex[1];
    GXPosition3s16(rect->mPos.x, rect->mPos.y, 0);
    GXColor4u8((u8)(scale * color->r), (u8)(scale * color->g), (u8)(scale * color->b),
               (u8)(scale * color->a));
    GXTexCoord2f32(uv[0], uv[1]);
    GXPosition3s16(rect->mPos.x + rect->mSize.x, rect->mPos.y, 0);
    GXColor4u8((u8)(scale * color->r), (u8)(scale * color->g), (u8)(scale * color->b),
               (u8)(scale * color->a));
    GXTexCoord2f32(uv[2], uv[3]);
    uv[4] = tex[2];
    uv[5] = tex[3];
    uv[6] = tex[0];
    uv[7] = tex[3];
    GXPosition3s16(rect->mPos.x + rect->mSize.x, rect->mPos.y + rect->mSize.y, 0);
    GXColor4u8((u8)(scale * color->r), (u8)(scale * color->g), (u8)(scale * color->b),
               (u8)(scale * color->a));
    GXTexCoord2f32(uv[4], uv[5]);
    GXPosition3s16(rect->mPos.x, rect->mPos.y + rect->mSize.y, 0);
    GXColor4u8((u8)(scale * color->r), (u8)(scale * color->g), (u8)(scale * color->b),
               (u8)(scale * color->a));
    GXTexCoord2f32(uv[6], uv[7]);
}
// ---------------------------------------------------------------------------
// func_804950F4: light-environment constructor.
// In-place construction of the 0x20 LightObj array (0x44 each), the
// LightSetting over the ambient/light-set-data arrays, and the four CLight[8]
// banks, then per-slot bookkeeping and func_804954B4 post-init.
// ---------------------------------------------------------------------------
extern "C" CLightEnv* func_804950F4(CLightEnv* self, u32 param) {
    extern void __ct__Q34nw4r3g3d8LightObjFv(void*, int);
    extern void __dt__Q34nw4r3g3d8LightObjFv(void*, int);
    extern void __ct__6CLightFv(void*, int);
    extern void __dt__6CLightFv(void*, int);
    extern void __ct__Q34nw4r3g3d12LightSettingFPQ34nw4r3g3d8LightObjPQ34nw4r3g3d11AmbLightObjUlPQ34nw4r3g3d12LightSetDataUl(void*, void*, void*, u32, void*, u32);

    __construct_array(self,
                      reinterpret_cast<ConstructorDestructor>(&__ct__Q34nw4r3g3d8LightObjFv),
                      reinterpret_cast<ConstructorDestructor>(&__dt__Q34nw4r3g3d8LightObjFv),
                      0x44, 0x20);
    // In-place LightSetting over the light / ambient / light-set data arrays
    // (called via the mangled name: placement new would emit a null check).
    __ct__Q34nw4r3g3d12LightSettingFPQ34nw4r3g3d8LightObjPQ34nw4r3g3d11AmbLightObjUlPQ34nw4r3g3d12LightSetDataUl(
        &self->mLightSetting, self->mGxLights, self->mAmbLight, 0x20,
        self->mLightSetData, 0x4);
    __construct_array(&self->mSlotLights[0][0],
                      reinterpret_cast<ConstructorDestructor>(&__ct__6CLightFv),
                      reinterpret_cast<ConstructorDestructor>(&__dt__6CLightFv),
                      0x40, 8);
    __construct_array(&self->mSlotLights[1][0],
                      reinterpret_cast<ConstructorDestructor>(&__ct__6CLightFv),
                      reinterpret_cast<ConstructorDestructor>(&__dt__6CLightFv),
                      0x40, 8);
    __construct_array(&self->mSlotLights[2][0],
                      reinterpret_cast<ConstructorDestructor>(&__ct__6CLightFv),
                      reinterpret_cast<ConstructorDestructor>(&__dt__6CLightFv),
                      0x40, 8);
    __construct_array(&self->mSlotLights[3][0],
                      reinterpret_cast<ConstructorDestructor>(&__ct__6CLightFv),
                      reinterpret_cast<ConstructorDestructor>(&__dt__6CLightFv),
                      0x40, 8);
    self->mField1170 = -1;
    self->mByte1174 = 0;
    self->mField1178 = param;
    self->mField117C = -1;
    self->mField1180 = 0;
    func_804954B4(self);
    return self;
}

// ---------------------------------------------------------------------------
// CLightEnv deleting destructor. Destroys the four CLight[8] banks in reverse
// declaration order (highest address first) followed by the 0x20 nw4r
// LightObj array at +0x0, then frees the object when the deleting flag is
// positive. Emitted as a stand-alone function because the retail symbol is
// fragment-anchored to 0x80495200 (no recoverable class name).
// ---------------------------------------------------------------------------
extern "C" void* __dt__80495200(CLightEnv* self, int deleting) {
    if (self != nullptr) {
        // MWCC runtime helpers: __destroy_arr walks the array calling the
        // given destructor in reverse order.
        extern void __dt__Q34nw4r3g3d8LightObjFv(void*, int);
        extern void __dt__6CLightFv(void*, int);
        __destroy_arr(self->mSlotLights[3],
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);
        __destroy_arr(self->mSlotLights[2],
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);
        __destroy_arr(self->mSlotLights[1],
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);
        __destroy_arr(self->mSlotLights[0],
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);
        __destroy_arr(self->mGxLights,
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__Q34nw4r3g3d8LightObjFv))),
                      0x44, 0x20);

        // When the deleting flag is positive, free the object via operator delete.
        if (deleting > 0) {
            extern void __dl__FPv(void*);
            __dl__FPv(self);
        }
    }
    return self;
}
