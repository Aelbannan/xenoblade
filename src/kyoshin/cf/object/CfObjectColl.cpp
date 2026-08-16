// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectColl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/object/CfObjectColl.hpp"

struct CfObjIf {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void _v0048();
    virtual void _v004C();
    virtual void _v0050();
    virtual void _v0054();
    virtual void _v0058();
    virtual void _v005C();
    virtual void _v0060();
    virtual void _v0064();
    virtual void _v0068();
    virtual void _v006C();
    virtual void _v0070();
    virtual void vf0074();
    virtual void _v0078();
    virtual void _v007C();
    virtual void _v0080();
    virtual void _v0084();
    virtual void _v0088();
    virtual void _v008C();
    virtual void _v0090();
    virtual void _v0094();
    virtual void _v0098();
    virtual void vf009C();
};



// Null-guarded bit-15 flag test on +0x64; returns self when the flag is
// set (retail keeps r3 = self and tests with r0 via bclr 4,2).
extern "C" void* func_800AB3D0(void* self) {
    if (self && (*(u32*)((u8*)self + 0x64) & 0x8000))
        return self;
    return 0;
}

void cf::CfObjectColl::func_800AB3EC() {}

cf::CfObjectColl::~CfObjectColl() {}

struct If158 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8(); virtual void _v0DC(); virtual void _v0E0(); virtual void _v0E4();
    virtual void _v0E8(); virtual void _v0EC(); virtual void _v0F0(); virtual void _v0F4();
    virtual void _v0F8(); virtual void _v0FC(); virtual void _v100(); virtual void _v104();
    virtual void _v108(); virtual void _v10C(); virtual void _v110(); virtual void _v114();
    virtual void _v118(); virtual void _v11C(); virtual void _v120(); virtual void _v124();
    virtual void _v128(); virtual void _v12C(); virtual void _v130(); virtual void _v134();
    virtual void _v138(); virtual void _v13C(); virtual void _v140(); virtual void _v144();
    virtual void _v148(); virtual void _v14C(); virtual void _v150(); virtual void _v154();
    virtual void vf158(u32 a);
};
extern "C" void func_800AB498__Q22cf12CfObjectCollFv(cf::CfObjectColl* self) {
    reinterpret_cast<If158*>(self)->vf158(1);
}

void CfObjectColl_initCollImplInstances(){}

void cf::CfObjectColl::func_800AB57C() {
    func_800C1638();
}

void func_800AB580(){}


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

// Sign test on a call through the u16 index at +0x9C (offset 0x20C8 table).
extern "C" int func_8009CF8C(u32 resourceId);
extern "C" bool func_800AC470(void* self) {
    return func_8009CF8C((*(u32*)((u8*)self + 0x9C) >> 16) + 0x20C8) != 0;
}

extern "C" void func_800AC4A8(void* self, u32 val) { *(u32*)((u8*)self + 0x9C) = val; }

extern float lbl_eu_80666914;
extern "C" float CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv() { return lbl_eu_80666914; }

extern "C" void CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv() {}

extern float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

void cf::CfObject::CfObject_UnkVirtualFunc67() {}

void cf::CfObject::CfObject_UnkVirtualFunc65() {}

extern "C" int CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" void CfObject_UnkVirtualFunc61__Q22cf8CfObjectFv() {}

extern float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv() {}

extern "C" void CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv(cf::CfObject* self) { return -1; }

extern "C" void CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv() {}

extern "C" int CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" int CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv(cf::CfObject* self) { return 0; }

extern "C" float CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv(cf::CfObject* self) { return *(float*)((u8*)self + 0x60); }

extern "C" void CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv(cf::CfObject* self, float val) { *(float*)((u8*)self + 0x60) = val; }

extern "C" void* CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv(cf::CfObject* self) { return (void*)((u8*)self + 0x48); }

extern "C" void* CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv(cf::CfObject* self) { return (void*)((u8*)self + 0x3c); }

extern "C" void CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv(cf::CfObject* self, float a, float b, float c) {
    *(float*)((char*)self + 0x3c) = a;
    *(float*)((char*)self + 0x40) = b;
    *(float*)((char*)self + 0x44) = c;
}

extern "C" int CfObject_UnkVirtualFunc18__Q22cf8CfObjectFv(cf::CfObject* self) { return 1; }

extern float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc17__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

extern "C" void* CfObject_UnkVirtualFunc16__Q22cf8CfObjectFv(cf::CfObject* self) { return self; }

extern float lbl_eu_80666910;
extern "C" float CfObject_UnkVirtualFunc15__Q22cf8CfObjectFv() { return lbl_eu_80666910; }

void* cf::CfObject::CfObject_UnkVirtualFunc14(float value) { return this; }

extern "C" int CfObject_UnkVirtualFunc13__Q22cf8CfObjectFv(cf::CfObject* self) { return 1; }

extern "C" void CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv() {}

int cf::CfObject::CfObject_UnkVirtualFunc11() {
    return *(u32*)((u8*)this + 0x6C) & 1;
}

void cf::CfObject::CfObject_UnkVirtualFunc10() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0x74 / 4];
    func(this);
}

extern "C" void CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv() {}

int cf::CfObjectPoint::func_800AC604() {
    return (field_0x68 >> 20) & 1;
}

extern "C" void* func_800AC610(void* param_1) {
    if (param_1 != NULL && (*(unsigned int*)((char*)param_1 + 0x64) & 0x20)) {
        return param_1;
    }
    return NULL;
}

extern "C" void func_800AB798__Q22cf12CfObjectCollFv(cf::CfObjectColl* self) { reinterpret_cast<CfObjIf*>(self)->vf009C(); }

extern "C" void CfObject_UnkVirtualFunc10__Q22cf8CfObjectFv(cf::CfObject* self) { reinterpret_cast<CfObjIf*>(self)->vf0074(); }
