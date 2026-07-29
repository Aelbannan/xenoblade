#include "monolib/scn/CLight.hpp"

using namespace ml;

CLight::CLight() {
    unk4.x = 0.0f;
    unk4.y = 0.0f;
    unk4.z = 0.0f;
    unk10.x = 0.5f;
    unk10.y = 0.5f;
    unk10.z = 0.5f;
    unk1C.x = 1.0f;
    unk1C.y = 0.0f;
    unk1C.z = 0.0f;
    unk28 = 1.0f;
    mpLightObj = nullptr;
    unk34 = 0;
    unk38 = 1.0f;
    unk3C = 10000.0f;
    mFlags = 0xF;
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
void func_804C0454(CLight* self, const ml::CVec3* pos) {
    self->unk4 = *pos;
    self->mpLightObj->InitLightPos(self->unk4.x, self->unk4.y, self->unk4.z);
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
void func_804C0928(){}
void func_804C09E0(CLight* self, f32 distance, f32 brightness, GXDistAttnFn distAttnFn) {
    self->mpLightObj->InitLightDistAttn(distance, brightness, distAttnFn);
}
void func_804C09E8(){}
