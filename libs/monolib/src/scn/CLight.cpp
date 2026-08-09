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

void func_804C02E4(u8* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
void func_804C0398(CLight* self, int lightObjPtr) {
    self->mpLightObj = (nw4r::g3d::LightObj*)lightObjPtr;
}
void func_804C03A0(u8* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
void func_804C0454(u8* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
void func_804C0484(u8* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
// Computes a direction vector by rotating the +Z unit vector (0,0,1)
// by the given Euler angles (X=angleX, Y=angleY, Z=0), then applies the
// resulting direction to the light object based on its light type.
void func_804C0570(CLight* self, f32 angleX, f32 angleY) {
    // Convert angles into the SinFIdx lookup index space.
    f32 radZ = lbl_eu_8066AFA8; // 0.0f (angleZ is fixed at 0)
    f32 radY = angleY * lbl_eu_8066A210;
    f32 radX = angleX * lbl_eu_8066A210;

    // Base +Z unit vector that gets rotated. Written to memory before the
    // trig calls below (so it is read back as an unknown value, echoing the
    // retail frame layout at [0x20..0x28]).
    ml::CVec3 v;
    v.x = lbl_eu_8066AFA8; // 0
    v.y = lbl_eu_8066AFA8; // 0
    v.z = lbl_eu_8066AFB0; // 1

    // Compute sin/cos via nw4r lookup tables. Matrix uses x=angleY,
    // y=angleX, z=0 (so sinX below is the trig of angleY).
    f32 sinX = nw4r::math::SinFIdx(radY * lbl_eu_8066AFBC);
    f32 cosX = nw4r::math::CosFIdx(radY * lbl_eu_8066AFBC);
    f32 sinY = nw4r::math::SinFIdx(radX * lbl_eu_8066AFBC);
    f32 cosY = nw4r::math::CosFIdx(radX * lbl_eu_8066AFBC);
    f32 sinZ = nw4r::math::SinFIdx(radZ * lbl_eu_8066AFBC);
    f32 cosZ = nw4r::math::CosFIdx(radZ * lbl_eu_8066AFBC);

    // XYZ rotation matrix elements (matches ml::CMat33::setRotXYZ).
    f32 m00 = cosY * cosZ;
    f32 m01 = sinX * sinY * cosZ - cosX * sinZ;
    f32 m02 = cosX * sinY * cosZ + sinX * sinZ;
    f32 m10 = cosY * sinZ;
    f32 m11 = sinX * sinY * sinZ + cosX * cosZ;
    f32 m12 = cosX * sinY * sinZ - sinX * cosZ;
    f32 m20 = -sinY;
    f32 m21 = sinX * cosY;
    f32 m22 = cosX * cosY;

    // Rotate the +Z base vector by the matrix and store the direction.
    f32 dirX = v.x * m00 + v.y * m01 + v.z * m02;
    f32 dirY = v.x * m10 + v.y * m11 + v.z * m12;
    f32 dirZ = v.x * m20 + v.y * m21 + v.z * m22;
    self->unk20 = dirX;
    self->unk24 = dirY;
    self->unk28 = dirZ;

    switch (self->unk34) {
    case 1: {
        // Scale direction to a position and set the light position.
        ml::CVec3 pos = ml::CVec3(self->unk20, self->unk24, self->unk28) * lbl_eu_8066AFB8;
        self->unk4 = pos;
        self->mpLightObj->InitLightPos(pos.x, pos.y, pos.z);
        break;
    }
    case 2:
        self->mpLightObj->InitSpecularDir(self->unk20, self->unk24, self->unk28);
        break;
    case 4:
        self->mpLightObj->InitLightDir(self->unk20, self->unk24, self->unk28);
        break;
    }
}
void func_804C07F0(u8* self, int value){
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
void func_804C09E0(u8* self){ InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(*(void**)((char*)self + 0x2c), 0.0f, 0.0f, 0); }
void func_804C09E8(){}
