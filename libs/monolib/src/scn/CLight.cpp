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

// LLM-HARNESS-BEGIN: us-804c4440
extern "C" void func_804C02E4(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
// LLM-HARNESS-END: us-804c4440
// LLM-HARNESS-BEGIN: us-804c44f4
extern "C" void func_804C0398(CLight* self, int lightObjPtr) {
    self->mpLightObj = (nw4r::g3d::LightObj*)lightObjPtr;
}
// LLM-HARNESS-END: us-804c44f4
// LLM-HARNESS-BEGIN: us-804c44fc
extern "C" void func_804C03A0(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
// LLM-HARNESS-END: us-804c44fc
// LLM-HARNESS-BEGIN: us-804c45b0
extern "C" void func_804C0454(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
// LLM-HARNESS-END: us-804c45b0
// LLM-HARNESS-BEGIN: us-804c45e0
extern "C" void func_804C0484(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
// LLM-HARNESS-END: us-804c45e0
// LLM-HARNESS-BEGIN: us-804c46cc
extern "C" void func_804C0570(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
// LLM-HARNESS-END: us-804c46cc
// LLM-HARNESS-BEGIN: us-804c494c
extern "C" void func_804C07F0(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
// LLM-HARNESS-END: us-804c494c
// LLM-HARNESS-BEGIN: us-804c4a24
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
// LLM-HARNESS-END: us-804c4a24
// LLM-HARNESS-BEGIN: us-804c4a7c
extern "C" void func_804C0920(CLight* self, float cutoff, _GXSpotFn spotFn) {
    self->mpLightObj->InitLightSpot(cutoff, spotFn);
}
// LLM-HARNESS-END: us-804c4a7c
// LLM-HARNESS-BEGIN: us-804c4a84
extern "C" void func_804C0928() {}
// LLM-HARNESS-END: us-804c4a84
// LLM-HARNESS-BEGIN: us-804c4b3c
extern void InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(void*, float, float, int);
extern "C" void func_804C09E0(void* self) { InitLightDistAttn__Q34nw4r3g3d8LightObjFff13_GXDistAttnFn(*(void**)((char*)self + 0x2c), 0.0f, 0.0f, 0); }
// LLM-HARNESS-END: us-804c4b3c
// LLM-HARNESS-BEGIN: us-804c4b44
extern "C" void func_804C09E8() {}
// LLM-HARNESS-END: us-804c4b44
