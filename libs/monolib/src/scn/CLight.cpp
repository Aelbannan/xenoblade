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

extern "C" void func_804C02E4(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
extern "C" void func_804C0398(CLight* self, int lightObjPtr) {
    self->mpLightObj = (nw4r::g3d::LightObj*)lightObjPtr;
}
extern "C" void func_804C03A0(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
extern "C" void func_804C0454(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
extern "C" void func_804C0484(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
extern "C" void func_804C0570(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
extern "C" void func_804C07F0(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
// Toggles a light-enable flag (bit 16 of mFlags) and propagates it
// to the GX LightObj's enable bit (bit 2 of its internal flag).
extern "C" void func_804C08C8(CLight* self, int enable) {
    if (enable)
        self->mFlags |= 0x10000;
    else
        self->mFlags &= ~0x10000;

    if (self->mFlags & 0x10000)
        self->mpLightObj->Enable();
    else
        self->mpLightObj->Disable();
}
extern "C" void func_804C0920(CLight* self, float cutoff, _GXSpotFn spotFn) {
    self->mpLightObj->InitLightSpot(cutoff, spotFn);
}
extern "C" void func_804C0928() {}
extern void InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(void*, float, float, int);
extern "C" void func_804C09E0(void* self) { InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(*(void**)((char*)self + 0x2c), 0.0f, 0.0f, 0); }
extern "C" void func_804C09E8() {}
