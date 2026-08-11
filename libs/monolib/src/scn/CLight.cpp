#include "monolib/scn/CLight.hpp"
#include "monolib/math/CMat33.hpp"
#include <nw4r/math/math_triangular.h>

using namespace ml;

extern const f32 lbl_eu_8066A210;
extern const f32 lbl_eu_8066AFA8;
extern const f32 lbl_eu_8066AFB0;
extern const f32 lbl_eu_8066AFB8;
extern const f32 lbl_eu_8066AFBC;
extern const f32 lbl_eu_8066AFAC;
extern const f32 lbl_eu_8066AFB4;

CLight::CLight(){
    u8* self = (u8*)this;
    extern char lbl_eu_8056F950[];
    *(void**)self = (void*)&lbl_eu_8056F950;
    *(float*)(self + 0x04) = lbl_eu_8066AFA8;
    *(float*)(self + 0x08) = lbl_eu_8066AFA8;
    *(float*)(self + 0x0C) = lbl_eu_8066AFA8;
    *(float*)(self + 0x10) = lbl_eu_8066AFAC;
    *(float*)(self + 0x14) = lbl_eu_8066AFAC;
    *(float*)(self + 0x18) = lbl_eu_8066AFAC;
    *(float*)(self + 0x1C) = lbl_eu_8066AFB0;
    *(float*)(self + 0x20) = lbl_eu_8066AFA8;
    *(float*)(self + 0x24) = lbl_eu_8066AFA8;
    *(float*)(self + 0x28) = lbl_eu_8066AFB0;
    void* lp = nullptr;
    *(u32*)(self + 0x2C) = (u32)lp;
    *(u32*)(self + 0x30) = (u32)(lp != 0) | 0xF;
    *(u32*)(self + 0x34) = (u32)lp;
    *(float*)(self + 0x38) = lbl_eu_8066AFB0;
    *(float*)(self + 0x3C) = lbl_eu_8066AFB4;
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
// Computes a light direction by building the XYZ Euler rotation matrix for
// (X = angleY, Y = angleX, Z fixed at 0) and applying it to the +Z unit
// vector. Input angles are in degrees (radians via lbl_eu_8066A210); the
// matrix math looks up nw4r trig from the radian values (index via
// lbl_eu_8066AFBC).
void func_804C0570(CLight* self, f32 angleX, f32 angleY) {
    // Euler angles in radians; angle.x gets the angleY contribution (X-axis
    // rotation) computed first so it spills to the frame like retail.
    ml::CVec3 angle(
        angleY * lbl_eu_8066A210,
        angleX * lbl_eu_8066A210,
        0.0f
    );

    // Base +Z unit vector that gets rotated.
    ml::CVec3 base(0.0f, 0.0f, 1.0f);

    // XYZ rotation matrix elements (matches ml::CMat33::setRotXYZ layout).
    f32 srr = nw4r::math::SinFIdx(lbl_eu_8066AFBC * angle.x);
    f32 crr = nw4r::math::CosFIdx(lbl_eu_8066AFBC * angle.x);
    f32 spy = nw4r::math::SinFIdx(lbl_eu_8066AFBC * angle.y);
    f32 cpy = nw4r::math::CosFIdx(lbl_eu_8066AFBC * angle.y);
    f32 sw = nw4r::math::SinFIdx(lbl_eu_8066AFBC * angle.z);
    f32 cw = nw4r::math::CosFIdx(lbl_eu_8066AFBC * angle.z);

    ml::CMat33 mat;
    mat.set(
        cpy * cw, srr * spy * cw - crr * sw, crr * spy * cw + srr * sw,
        cpy * sw, srr * spy * sw + crr * cw, crr * spy * sw - srr * cw,
        -spy,     srr * cpy,                  crr * cpy
    );

    // Direction = rotation matrix * base (matrix Z column since base == +Z).
    f32 dirX = mat.m[0][0] * base.x + mat.m[0][1] * base.y + mat.m[0][2] * base.z;
    f32 dirY = mat.m[1][0] * base.x + mat.m[1][1] * base.y + mat.m[1][2] * base.z;
    f32 dirZ = mat.m[2][0] * base.x + mat.m[2][1] * base.y + mat.m[2][2] * base.z;

    self->unk20 = dirX;
    self->unk24 = dirY;
    self->unk28 = dirZ;

    switch (self->unk34) {
    case 1: {
        // Scale the direction to a position distance and set the light position.
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
// Toggles a light-enable flag (bit 16 of unk30) and propagates it
// to the GX LightObj's enable bit (bit 2 of its internal flag).
void func_804C08C8(CLight* self, int enable) {
    if (enable)
        self->unk30 |= 0x10000;
    else
        self->unk30 &= ~0x10000;

    if (self->unk30 & 0x10000)
        self->mpLightObj->Enable();
    else
        self->mpLightObj->Disable();
}
void func_804C0920(CLight* self, float cutoff, _GXSpotFn spotFn) {
    self->mpLightObj->InitLightSpot(cutoff, spotFn);
}
void func_804C0928(){}
extern void InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(void*, float, float, int);
// retail: lwz r3,0x2c(r3); b InitLightDistAttn__... (args pass through)
extern "C" void func_804C09E0(u8* self, float a, float b, int c) {
    InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(*(void**)((char*)self + 0x2c), a, b, c);
}
void func_804C09E8(){}
