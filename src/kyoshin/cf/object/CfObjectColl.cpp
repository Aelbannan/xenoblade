// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectColl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/object/CfObjectColl.hpp"

namespace cf {
class CfObjectPoint {
public:
    void func_800AC604();
};
} // namespace cf

void func_800AB3D0(){}

void cf::CfObjectColl::func_800AB3EC() {}

cf::CfObjectColl::~CfObjectColl() {}

void cf::CfObjectColl::func_800AB498() {}

void CfObjectColl_initCollImplInstances(){}

void cf::CfObjectColl::func_800AB57C(void) {}

void func_800AB580(){}

void cf::CfObjectColl::func_800AB798() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0x9c / 4];
    func(this);
}

void cf::CfObjectColl::func_800AB7A8() {}

void func_800AB7F8(){}

void func_800AB8CC(){}

void func_800AB978(){}

void func_800ABA18(){}

void func_800ABB9C(){}

void func_800ABC5C(){}

void func_800ABD44(){}

void func_800ABDE4(){}

void func_800ABE84(){}

void func_800ABF24(){}

void func_800ABFC4(){}

void func_800AC110(){}

void func_800AC1BC(){}

void func_800AC30C(){}

void func_800AC378(){}

void func_800AC3F4(){}

void func_800AC450(void* self, unsigned long a, unsigned long b) {
    *(unsigned long*)((char*)self + 0x9c) = (a << 16) + b;
}

unsigned long func_800AC460(void* self) {
    extern unsigned long func_8009D018(unsigned long);
    unsigned long v = *(unsigned long*)((char*)self + 0x9c);
    return func_8009D018((v >> 16) + 0x20c8);
}

void func_800AC470(){}

extern "C" void func_800AC4A8(void* self, u32 val) { *(u32*)((u8*)self + 0x9C) = val; }

extern float lbl_eu_80666914;
extern "C" float CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv() { return lbl_eu_80666914; }

extern "C" void CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv() {}

extern float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv(void* self) { return 0; }

void cf::CfObject::CfObject_UnkVirtualFunc67() {}

void cf::CfObject::CfObject_UnkVirtualFunc65() {}

extern "C" int CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" void CfObject_UnkVirtualFunc61__Q22cf8CfObjectFv() {}

extern float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv() {}

extern "C" void CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv(void* self) { return -1; }

extern "C" void CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv(void* self) { return 0; }

extern "C" float CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv(void* self) { return *(float*)((u8*)self + 0x60); }

extern "C" void CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv(void* self, float val) { *(float*)((u8*)self + 0x60) = val; }

extern "C" void* CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv(void* self) { return (void*)((u8*)self + 0x48); }

extern "C" void* CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv(void* self) { return (void*)((u8*)self + 0x3c); }

extern "C" void CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv(void* self, float a, float b, float c) {
    *(float*)((char*)self + 0x3c) = a;
    *(float*)((char*)self + 0x40) = b;
    *(float*)((char*)self + 0x44) = c;
}

extern "C" int CfObject_UnkVirtualFunc18__Q22cf8CfObjectFv(void* self) { return 1; }

extern float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc17__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void CfObject_UnkVirtualFunc16__Q22cf8CfObjectFv() {}

extern float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc15__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void CfObject_UnkVirtualFunc14__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc13__Q22cf8CfObjectFv(void* self) { return 1; }

extern "C" void CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv() {}

void cf::CfObject::CfObject_UnkVirtualFunc11() {}

void cf::CfObject::CfObject_UnkVirtualFunc10() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0x74 / 4];
    func(this);
}

extern "C" void CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv() {}

void cf::CfObjectPoint::func_800AC604() {}

extern "C" void* func_800AC610(void* param_1) {
    if (param_1 != NULL && (*(unsigned int*)((char*)param_1 + 0x64) & 0x20)) {
        return param_1;
    }
    return NULL;
}
