#include "monolib/scn/CLight.hpp"

using namespace ml;

CLight::CLight(){
    u32 r4 = 0;
    u32 r0 = r4 & 0xF;
    unk4 = CVec3(0,0,0);
    unk10 = CVec3(0.5f,0.5f,0.5f);
    unk1C = CVec3(1,0,0);
    unk28 = 1;
    mpLightObj = nullptr;
    unk34 = 0;
    unk38 = 1;
    unk3C = 10000;
    mFlags = r0;
}

void func_804C02E4(CLight* self, const CLight* other) {
    self->unk4 = other->unk4;
    self->unk10 = other->unk10;
    self->unk1C = other->unk1C;
    self->unk28 = other->unk28;
    self->mFlags = other->mFlags;
    self->unk34 = other->unk34;
    self->unk38 = other->unk38;
    *self->mpLightObj = *other->mpLightObj;
    self->unk3C = other->unk3C;
}

void func_804C0398(CLight* self, int lightObjPtr) {
    self->mpLightObj = (nw4r::g3d::LightObj*)lightObjPtr;
}

// Sets the light type from unk34 and initialises the backing LightObj
// with the corresponding attenuation/spot parameters.
void func_804C03A0(CLight* self) {
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
        self->mFlags &= ~0x10000;
        break;
    }
}

void func_804C0454(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
void func_804C0484(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
}
void func_804C0570(void* self, int value){
    *(int*)((char*)self + 0x2c) = value;
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

// Scales the light colour by an intensity factor, stores the result into
// unk38 (and onto the stack as temporaries), then builds an 8-bit
// GXColor from the scaled RGB plus the direction's x-component as alpha
// and forwards it to the backing LightObj.
void func_804C0928(CLight* self, float intensity) {
    self->unk38 = intensity;

    float r = self->unk10.x * intensity;
    float g = self->unk10.y * intensity;
    float b = self->unk10.z * intensity;

    GXColor color;
    color.r = (u8)(int)(r * 255.0f);
    color.g = (u8)(int)(g * 255.0f);
    color.b = (u8)(int)(b * 255.0f);
    color.a = (u8)(int)(self->unk1C.x * 255.0f);

    self->mpLightObj->InitLightColor(color);
}

extern void InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(void*, float, float, int);
void func_804C09E0(void* self){ InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(*(void**)((char*)self + 0x2c), 0.0f, 0.0f, 0); }
void func_804C09E8(){}
