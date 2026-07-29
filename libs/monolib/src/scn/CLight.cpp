#include <monolib/scn/CLight.hpp>

using namespace ml;

CLight::CLight(){
    u32 r4 = 0;
    u32 r0 = r4 & 0xF;
    unk4 = CVec3(0.0f, 0.0f, 0.0f);
    unk10 = CVec3(0.5f, 0.5f, 0.5f);
    unk1C_x = 1.0f;
    unk20 = 0.0f;
    unk24 = 0.0f;
    unk28 = 1.0f;
    mpLightObj = nullptr;
    unk30 = 0;
    unk34 = 0;
    unk38 = 1.0f;
    unk3C = 10000.0f;
    mFlags = r0;
}

void func_804C02E4(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
void func_804C0398(CLight* self, int lightObjPtr) {
    self->mpLightObj = (nw4r::g3d::LightObj*)lightObjPtr;
}
void func_804C03A0(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
void func_804C0454(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
// Sets the light direction and calls the appropriate LightObj init based on light type.
// type 1 (point): scales direction to position, calls InitLightPos
// type 2 (specular): calls InitSpecularDir
// type 4 (directional): calls InitLightDir
void CLight::func_804C0484(const float* dir) {
    unk20 = dir[0];
    unk24 = dir[1];
    unk28 = dir[2];

    switch (unk34) {
    case 1: {
        float scale = 1.0f;
        unk4.x = unk20 * scale;
        unk4.y = unk24 * scale;
        unk4.z = unk28 * scale;
        mpLightObj->InitLightPos(unk4.x, unk4.y, unk4.z);
        break;
    }
    case 2:
        mpLightObj->InitSpecularDir(unk20, unk24, unk28);
        break;
    case 4:
        mpLightObj->InitLightDir(unk20, unk24, unk28);
        break;
    }
}
void func_804C0570(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
// lbl_eu_8066AFC0 = 255.0f used in color float-to-byte conversion
extern f32 lbl_eu_8066AFC0;

// Copies a 4-float color into the light, scales RGB by intensity,
// converts to GXColor bytes, and applies via InitLightColor.
void CLight::func_804C07F0(const f32* color) {
    // Copy 4 color components as words (matches retail lwz/stw)
    *(u32*)&unk10.x = *(const u32*)&color[0];
    *(u32*)&unk10.y = *(const u32*)&color[1];
    *(u32*)&unk10.z = *(const u32*)&color[2];
    *(u32*)&unk1C_x = *(const u32*)&color[3];

    // Scale RGB by intensity, then by 255.0f for byte conversion
    f32 intensity = unk38;
    f32 r = unk10.x * intensity * lbl_eu_8066AFC0;
    f32 g = unk10.y * intensity * lbl_eu_8066AFC0;
    f32 b = unk10.z * intensity * lbl_eu_8066AFC0;
    f32 a = unk1C_x * lbl_eu_8066AFC0;

    GXColor col;
    col.r = (u8)(s32)r;
    col.g = (u8)(s32)g;
    col.b = (u8)(s32)b;
    col.a = (u8)(s32)a;

    mpLightObj->InitLightColor(col);
}
// Toggles a light-enable flag (bit 16 of mFlags) and propagates it
// to the GX LightObj's enable bit (bit 2 of its internal flag).
void func_804C08C8(CLight* self, int enable) {
    if (enable)
        self->mFlags |= 0x10000;
    else
        self->mFlags &= ~0x10000;

    if (self->mFlags & 0x10000)
        self->mpLightObj->Enable();
    else
        self->mpLightObj->Disable();
}
void func_804C0920(CLight* self, float cutoff, _GXSpotFn spotFn) {
    self->mpLightObj->InitLightSpot(cutoff, spotFn);
}
void func_804C0928(){}
extern void InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(void*, float, float, int);
void func_804C09E0(void* self){ InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(*(void**)((char*)self + 0x2c), 0.0f, 0.0f, 0); }
// Applies a matrix transform to the light and writes the resulting GXLightObj to outLight.
// Copies the internal GXLightObj from mpLightObj, then transforms position/direction
// based on the light type:
//   type 4 (spot/dir): transforms direction (normalize), also transforms position
//   type 1 or 3 (point): transforms position
//   type 2 (specular): transforms position as specular direction (normalize)
void CLight::func_804C09E8(GXLightObj* outLight, const Mtx matrix) {
    // Copy the GXLightObj from our internal LightObj to the output
    *outLight = *static_cast<GXLightObj*>(*mpLightObj);

    CVec3 temp;

    if (unk34 == 4) {
        // Transform the direction vector by the matrix
        CVec3 dir(unk20, unk24, unk28);
        nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&temp, (const nw4r::math::MTX34*)matrix, (const nw4r::math::VEC3*)&dir);
        if (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z == 0.0f) {
            temp = CVec3::zero;
        } else {
            PSVECNormalize((Vec*)&temp, (Vec*)&temp);
        }
        GXInitLightDir(outLight, temp.x, temp.y, temp.z);
    }

    if (unk34 == 1 || unk34 == 3 || unk34 == 4) {
        PSMTXMultVec(matrix, (const Vec*)&unk4, (Vec*)&temp);
        GXInitLightPos(outLight, temp.x, temp.y, temp.z);
    } else if (unk34 == 2) {
        nw4r::math::VEC3TransformNormal((nw4r::math::VEC3*)&temp, (const nw4r::math::MTX34*)matrix, (const nw4r::math::VEC3*)&unk4);
        if (temp.x * temp.x + temp.y * temp.y + temp.z * temp.z == 0.0f) {
            temp = CVec3::zero;
        } else {
            PSVECNormalize((Vec*)&temp, (Vec*)&temp);
        }
        GXInitSpecularDir(outLight, temp.x, temp.y, temp.z);
    }
}
