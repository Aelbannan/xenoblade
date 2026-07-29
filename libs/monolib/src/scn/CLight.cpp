#include "monolib/scn/CLight.hpp"
#include <nw4r/math/math_triangular.h>

using namespace ml;

extern const f32 lbl_eu_8066A210;
extern const f32 lbl_eu_8066AFA8;
extern const f32 lbl_eu_8066AFB0;
extern const f32 lbl_eu_8066AFB8;
extern const f32 lbl_eu_8066AFBC;

CLight::CLight(){
    u32 r4 = 0;
    u32 r0 = r4 & 0xF;
    unk4 = CVec3(0,0,0);
    unk10 = CVec3(0.5f,0.5f,0.5f);
    unk1C_x = 1.0f;
    unk20 = 0.0f;
    unk24 = 0.0f;
    unk28 = 1.0f;
    mpLightObj = nullptr;
    unk34 = 0;
    unk38 = 1;
    unk3C = 10000;
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
void func_804C0484(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
// Computes a direction vector from two Euler angles (degrees) and stores it.
// Then applies the direction to the light object based on the light type.
void func_804C0570(CLight* self, f32 angleX, f32 angleY) {
    // Convert degrees to radians, then radians to SinFIdx index
    f32 radX = angleX * lbl_eu_8066A210;
    f32 radY = angleY * lbl_eu_8066A210;
    f32 radZ = lbl_eu_8066AFA8; // 0.0f

    // Compute sin/cos via nw4r lookup tables
    f32 sinX = nw4r::math::SinFIdx(radX * lbl_eu_8066AFBC);
    f32 cosX = nw4r::math::CosFIdx(radX * lbl_eu_8066AFBC);
    f32 sinY = nw4r::math::SinFIdx(radY * lbl_eu_8066AFBC);
    f32 cosY = nw4r::math::CosFIdx(radY * lbl_eu_8066AFBC);
    f32 sinZ = nw4r::math::SinFIdx(radZ * lbl_eu_8066AFBC);
    f32 cosZ = nw4r::math::CosFIdx(radZ * lbl_eu_8066AFBC);

    // Rotate the Z unit vector (0,0,1) by the three angles.
    // Equivalent to extracting the third column of a YXZ rotation matrix.
    f32 dirX = cosZ * cosX * sinY + sinX * sinZ;
    f32 dirY = sinZ * cosX * sinY - sinX * cosZ;
    f32 dirZ = cosX * cosY;

    self->unk20 = dirX;
    self->unk24 = dirY;
    self->unk28 = dirZ;

    switch (self->unk34) {
    case 1: {
        // Scale direction to position and set light position
        f32 posX = dirX * lbl_eu_8066AFB8;
        f32 posY = dirY * lbl_eu_8066AFB8;
        f32 posZ = dirZ * lbl_eu_8066AFB8;
        self->unk4 = ml::CVec3(posX, posY, posZ);
        self->mpLightObj->InitLightPos(posX, posY, posZ);
        break;
    }
    case 2:
        self->mpLightObj->InitSpecularDir(dirX, dirY, dirZ);
        break;
    case 4:
        self->mpLightObj->InitLightDir(dirX, dirY, dirZ);
        break;
    }
}
void func_804C07F0(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
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
void func_804C09E8(){}
