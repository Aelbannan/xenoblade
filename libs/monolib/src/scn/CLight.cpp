#include "monolib/scn/CLight.hpp"

using namespace ml;

CLight::CLight(){
    u32 r4 = 0;
    u32 r0 = r4 & 0xF;
    unk4 = CVec3(0,0,0);
    unk10 = CVec3(0.5f,0.5f,0.5f);
    unk1C = CVec3(1,0,0);
    unk28 = 1;
    unk2C = 0;
    unk34 = 0;
    unk38 = 1;
    unk3C = 10000;
    unk30 = r0;
}

// LLM-HARNESS-BEGIN: us-804c4440
extern "C" void func_804C02E4() {}
// LLM-HARNESS-END: us-804c4440
// LLM-HARNESS-BEGIN: us-804c44f4
extern "C" void func_804C0398(void* self, int value) {
    *(int*)((char*)self + 0x2c) = value;
}
// LLM-HARNESS-END: us-804c44f4
// LLM-HARNESS-BEGIN: us-804c44fc
extern "C" void func_804C03A0() {}
// LLM-HARNESS-END: us-804c44fc
// LLM-HARNESS-BEGIN: us-804c45b0
extern "C" void func_804C0454() {}
// LLM-HARNESS-END: us-804c45b0
// LLM-HARNESS-BEGIN: us-804c45e0
extern "C" void func_804C0484() {}
// LLM-HARNESS-END: us-804c45e0
// LLM-HARNESS-BEGIN: us-804c46cc
extern "C" void func_804C0570() {}
// LLM-HARNESS-END: us-804c46cc
// LLM-HARNESS-BEGIN: us-804c494c
extern "C" void func_804C07F0() {}
// LLM-HARNESS-END: us-804c494c
// LLM-HARNESS-BEGIN: us-804c4a24
extern "C" void func_804C08C8(void* self, int param) {
    struct Obj {
        char pad[0x2c];
        uint32_t* ptr;
        uint32_t flags;
    };
    Obj* obj = (Obj*)self;
    if (param)
        obj->flags |= 0x10000;
    else
        obj->flags &= ~0x10000;
    if (obj->flags & 0x10000)
        *obj->ptr |= 0x4;
    else
        *obj->ptr &= ~0x4;
}
// LLM-HARNESS-END: us-804c4a24
// LLM-HARNESS-BEGIN: us-804c4a7c
extern "C" bool func_804C0920() { return false; }
// LLM-HARNESS-END: us-804c4a7c
// LLM-HARNESS-BEGIN: us-804c4a84
extern "C" void func_804C0928() {}
// LLM-HARNESS-END: us-804c4a84
// LLM-HARNESS-BEGIN: us-804c4b3c
extern "C" bool func_804C09E0() { return false; }
// LLM-HARNESS-END: us-804c4b3c
// LLM-HARNESS-BEGIN: us-804c4b44
extern "C" void func_804C09E8() {}
// LLM-HARNESS-END: us-804c4b44
