#include "monolib/scn/CLight.hpp"
#include "monolib/math/CMat33.hpp"
#include <nw4r/math/math_triangular.h>

using namespace ml;


CLight::CLight(){
    u8* self = (u8*)this;
    extern char lbl_eu_8056F950[];
    void* lp = nullptr;
    *(void**)self = (void*)&lbl_eu_8056F950;
    *(float*)(self + 0x04) = 0.0f;
    *(float*)(self + 0x08) = 0.0f;
    *(float*)(self + 0x0C) = 0.0f;
    *(float*)(self + 0x10) = 0.5f;
    *(float*)(self + 0x14) = 0.5f;
    *(float*)(self + 0x18) = 0.5f;
    *(float*)(self + 0x1C) = 1.0f;
    *(float*)(self + 0x20) = 0.0f;
    *(float*)(self + 0x24) = 0.0f;
    *(float*)(self + 0x28) = 1.0f;
    *(u32*)(self + 0x2C) = (u32)lp;
    *(u32*)(self + 0x34) = (u32)lp;
    *(float*)(self + 0x38) = 1.0f;
    *(float*)(self + 0x3C) = 10000.0f;
    this->unk30 = (u32)this->mpLightObj | 0xF;
}

// Copies light state from src into an existing CLight without disturbing its
// vtable (offset 0) or its LightObj pointer (0x2C): plain fields first, then
// the LightObj contents through operator=, then the trailing field.
extern "C" CLight* LightCtlCopyObj(CLight* dst, const CLight* src) {
    dst->unk4 = src->unk4;
    dst->unk10 = src->unk10;
    // The four scalar words (unk1C_x..unk28) move as raw u32 words
    // (retail lwz/stw), matching the LightCtlSetPos idiom in this TU.
    const u32* s = (const u32*)&src->unk1C_x;
    u32* d = (u32*)&dst->unk1C_x;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    dst->unk30 = src->unk30;
    dst->unk34 = src->unk34;
    dst->unk38 = src->unk38;
    *dst->mpLightObj = *src->mpLightObj;
    dst->unk3C = src->unk3C;
    return dst;
}
void LightCtlSetObj(CLight* self, int lightObjPtr) {
    self->mpLightObj = (nw4r::g3d::LightObj*)lightObjPtr;
}
extern "C" void LightCtlSetAttnMode(CLight* self, int mode) {
    self->unk34 = mode;
    self->mpLightObj->Clear();
    switch (self->unk34) {
    case 1:
        self->mpLightObj->InitLightAttnA(1.0f, 0.0f, 0.0f);
        self->mpLightObj->InitLightAttnK(1.0f, 0.0f, 0.0f);
        break;
    case 3:
        self->mpLightObj->InitLightAttnA(1.0f, 0.0f, 0.0f);
        break;
    case 0:
        self->mpLightObj->Disable();
        self->unk30 &= ~0x10000;
        break;
    default:
        break;
    }
}
// Set the light position: copy the vector as raw u32 words (retail lwz/stw),
// then forward to the LightObj (tail call).
extern "C" void LightCtlSetPos(CLight* self, const ml::CVec3* v) {
    u32* dst = (u32*)&self->unk4;
    const u32* s = (const u32*)v;
    dst[0] = s[0];
    dst[1] = s[1];
    dst[2] = s[2];
    self->mpLightObj->InitLightPos(self->unk4.x, self->unk4.y, self->unk4.z);
}
// Set the light direction: copy the vector as raw u32 words (retail lwz/stw),
// then forward to the LightObj per the unk34 light type. Case 1 scales the
// direction to a position (then InitLightPos with unk4, as in LightCtlSetPos).
extern "C" void LightCtlSetDir(CLight* self, const ml::CVec3* dir) {
    u32* dst = (u32*)&self->unk20;
    const u32* s = (const u32*)dir;
    dst[0] = s[0];
    dst[1] = s[1];
    dst[2] = s[2];
    switch (self->unk34) {
    case 1: {
        ml::CVec3 tmp;
        ml::CVec3 pos;
        const f32 dist = 1000000.0f;
        nw4r::math::VEC3Scale(tmp, *(const ml::CVec3*)&self->unk20, dist);
        pos.x = tmp.x;
        pos.y = tmp.y;
        pos.z = tmp.z;
        self->unk4 = pos;
        self->mpLightObj->InitLightPos(self->unk4.x, self->unk4.y, self->unk4.z);
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
// Computes a light direction by building the XYZ Euler rotation matrix for
// (X = angleY, Y = angleX, Z fixed at 0) and applying it to the +Z unit
// vector. Input angles are in degrees (radians via ml::deg2rad); the
// matrix math looks up nw4r trig from the radian values (index via
// 40.743664f).
void LightCtlSetDirFromAngle(CLight* self, f32 angleX, f32 angleY) {
    // Euler angles in radians; angle.x gets the angleY contribution (X-axis
    // rotation) computed first so it spills to the frame like retail.
    ml::CVec3 angle(
        angleY * ml::deg2rad,
        angleX * ml::deg2rad,
        0.0f
    );

    // Base +Z unit vector that gets rotated.
    ml::CVec3 base(0.0f, 0.0f, 1.0f);

    // XYZ rotation matrix elements (matches ml::CMat33::setRotXYZ layout).
    f32 srr = nw4r::math::SinFIdx(40.743664f * angle.x);
    f32 crr = nw4r::math::CosFIdx(40.743664f * angle.x);
    f32 spy = nw4r::math::SinFIdx(40.743664f * angle.y);
    f32 cpy = nw4r::math::CosFIdx(40.743664f * angle.y);
    f32 sw = nw4r::math::SinFIdx(40.743664f * angle.z);
    f32 cw = nw4r::math::CosFIdx(40.743664f * angle.z);

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
        ml::CVec3 pos = ml::CVec3(self->unk20, self->unk24, self->unk28) * 1000000.0f;
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
void LightCtlSetColor(u8* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
// Toggles a light-enable flag (bit 16 of unk30) and propagates it
// to the GX LightObj's enable bit (bit 2 of its internal flag).
void LightCtlSetEnable(CLight* self, int enable) {
    if (enable)
        self->unk30 |= 0x10000;
    else
        self->unk30 &= ~0x10000;

    if (self->unk30 & 0x10000)
        self->mpLightObj->Enable();
    else
        self->mpLightObj->Disable();
}
void LightCtlInitSpot(CLight* self, float cutoff, _GXSpotFn spotFn) {
    self->mpLightObj->InitLightSpot(cutoff, spotFn);
}
void LightCtlScaleColor(){}
extern "C" void InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(void*, float, float, int);
// retail: lwz r3,0x2c(r3); b InitLightDistAttn__... (args pass through)
extern "C" void LightCtlInitDistAttn(u8* self, float a, float b, int c) {
    InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(*(void**)((char*)self + 0x2c), a, b, c);
}
void LightCtlInitGXLight(){}
