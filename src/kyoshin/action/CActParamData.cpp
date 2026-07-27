// Auto-scaffolded catalog TU for kyoshin/action/CActParamData
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

CActParamData::CActParamData() {}

void CActParamData::~CActParamData() {}

void func_80053A90(){}

void func_80053B24(){}

void func_80053DE8(){}

void func_80053F40(){}

void func_80053F7C(){}

void func_80054170(){}

void func_80054438(){}

void func_80054614(){}

void func_800547D4(){}

void func_80054980(){}

extern "C" void func_80054A20() {}

extern "C" unsigned char func_80054A24(unsigned char* p, int sel) {
    if (sel != 0) {
        return p[0x344];
    }
    return p[0x2c4];
}

void func_80054A3C(){}

void func_80054A94(){}

void func_80054D3C(void* self);
void func_80054D34(void* self){ ((void(*)(void*))func_80054D3C)((char*)self + 0x2e0); }


extern "C" void func_800550D8() {}

extern "C" void func_800550DC() {}

extern "C" void func_800550E0() {}

extern "C" void func_800550E4() {}

void func_800550E8(){}

extern "C" void func_800554D0() {}

extern "C" void func_800554D4() {}

extern "C" void func_800554D8() {}

void func_800554DC(){}

void func_800555EC(){}

void func_80055700(){}

void func_8005577C(){}

void func_800557E8(){}

void func_80055960(){}

void func_80055AC4(){}

void func_80055B88(){}

void func_80055DB8(){}

void func_80055DD4(){}

void func_80055DF0(){}

extern "C" void* func_80055EA0(void* param) {
    void* val = *(void**)((char*)param + 0x2f4);
    if (!val) return 0;
    return (char*)val + 0x14;
}

void func_80055EBC(){}

void func_80055EE4(){}

void func_80055F08(){}

extern "C" int func_80055F24(void* self) {
    char* base = (char*)self + 0x260;
    if (*(int*)((char*)self + 0x2d4) == 0)
        base = (char*)self + 0x2e0;
    void* ptr = *(void**)(base + 0x74);
    if (ptr == NULL)
        return -1;
    return *(short*)((char*)ptr + 8);
}

extern "C" s16 func_80055F54(void* self) {
    u8* base = (u8*)self + 0x260;
    if (*(s32*)((u8*)self + 0x2d4) == 0)
        base = (u8*)self + 0x2e0;
    void* ptr = *(void**)(base + 0x74);
    if (!ptr) return -1;
    return *(s16*)((u8*)ptr + 0xa);
}

extern "C" void func_80055F84(void* self) {
    *(unsigned long*)((char*)self + 0x2d4) = 0;
    *(unsigned long*)((char*)self + 0x354) = 0;
}

void func_80055F94(){}

extern "C" void* func_8005605C(void* param) {
    if (*(int*)((int)param + 0x274) != 0) {
        void* ptr = *(void**)((int)param + 0x2d8);
        if (ptr != 0) return ptr;
    }
    if (*(int*)((int)param + 0x2f4) != 0) {
        void* result = *(void**)((int)param + 0x358);
        if (result != 0) return result;
    }
    return 0;
}

extern "C" bool func_8005609C() { return false; }

void func_800560A4(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x34 / 4];
    func(self);
}

void func_800560B4(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x98 / 4];
    func(self);
}

void func_800560C4(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x88 / 4];
    func(self);
}

void func_800560D4(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x8c / 4];
    func(self);
}

void func_800560E4(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0x90 / 4];
    func(self);
}

void func_800560F4(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[148 / 4];
    func(self);
}

void func_80056104(){}

void func_80056118(void* self){
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[48 / 4];
    func(self);
}

void func_80056128(){}

void func_80056138(){}

void func_80056148(){}

void func_80056158(){}

void func_80056168(){}

void func_80056178(){}

void func_80056188(){}

void func_80056198(){}

void func_800561A8(){}

void func_800561B8(){}

void func_800561C8(){}

void func_800561D8(){}

void func_800561E8(){}

void func_800561F8(){}

void func_80056208(){}

void func_80056218(){}

void func_80056228(){}

void func_80056238(){}

void func_80056248(){}

void func_80056258(){}

void func_80056268(){}

void func_8005627C(){}

// Fake SI iface for vt+0x7c tail-call (-RTTI on: omit _v000/_v004).
struct CActParamDataVTableIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual int vf7C(int a, int b);
};

extern "C" int func_80056290(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 3);
}

extern "C" int func_800562A4(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 4);
}

extern "C" int func_800562B8(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 5);
}

extern "C" int func_800562CC(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 6);
}

extern "C" int func_800562E0(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 7);
}

extern "C" int func_800562F4(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 8);
}

extern "C" int func_80056308(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 9);
}

extern "C" int func_8005631C(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 10);
}

extern "C" int func_80056330(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 11);
}

extern "C" int func_80056344(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 12);
}

extern "C" int func_80056358(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 13);
}

extern "C" int func_8005636C(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 14);
}

extern "C" int func_80056380(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 15);
}

extern "C" int func_80056394(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 16);
}

extern "C" int func_800563A8(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 17);
}

extern "C" int func_800563BC(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 18);
}

extern "C" int func_800563D0(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 19);
}

extern "C" int func_800563E4(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 20);
}

extern "C" int func_800563F8(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 21);
}

extern "C" int func_8005640C(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 22);
}

extern "C" int func_80056420(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 23);
}

extern "C" int func_80056434(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 24);
}

extern "C" int func_80056448(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 25);
}

extern "C" int func_8005645C(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 26);
}

extern "C" int func_80056470(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 27);
}

extern "C" int func_80056484(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 28);
}

extern "C" int func_80056498(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 29);
}

extern "C" int func_800564AC(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 30);
}

extern "C" int func_800564C0(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 31);
}

void func_800564D4(){}

void func_800564E8(){}

void func_800564FC(){}

void func_80056510(){}

void func_80056524(){}

void func_80056538(){}

void func_8005654C(){}

void func_80056560(){}

void func_80056574(){}

void func_80056588(){}

void func_8005659C(){}

void func_800565B0(){}

void func_800565C4(){}

void func_800565D8(){}

void func_800565EC(){}

void func_80056600(){}

void func_80056614(){}

void func_80056624(){}

void func_80056638(){}

void func_8005664C(){}

void func_80056660(){}

void func_80056670(){}

void func_80056680(){}

void func_80056694(){}

void func_800566A8(){}

void func_800566BC(){}

void func_800566D0(){}

void func_800566E4(){}

void func_800566F8(){}

void func_80056708(){}

void func_8005671C(){}

void func_80056730(){}

void func_80056760(){}

void func_80056790(){}

void func_800567A0(){}

void func_800567B0(){}

void func_800567C0(){}

void func_800567D0(){}

void func_800567E0(){}

extern "C" bool func_800567F0() { return false; }

extern "C" bool func_800567F8() { return false; }

extern "C" bool func_80056800() { return false; }

void func_80056808(){}

void func_80056828(){}

void func_80056888(){}

void func_800568A8(){}

void func_800568C8(){}

void func_800568E8(){}

void func_80056A98(){}

void func_80056C34(){}

void func_80056C54(){}

void func_80056C7C(){}

void func_80056CC8(){}

void func_80056CE4(){}

void func_80056D00(){}

void func_80056EAC(){}

void func_80056EC8(){}

void func_80057084(){}

void func_80057244(){}

void func_80057264(){}

void func_80057280(){}

void func_8005742C(){}

void func_80057470(){}

void func_80057490(){}

void func_80057654(){}

void func_80057670(){}

void func_80057828(){}

void func_8005789C(){}

void func_80057A64(){}

void func_80057BA0(){}

void CActParamData::func_80057CDC() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace ml {
template <int N> class FixStr { public: void format(const char*, ...); };
template <> void FixStr<16>::format(const char*, ...) {}
}
