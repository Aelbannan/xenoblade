// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplWalker
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cfsys/CfObjectImplWalker.hpp"
void func_800C1F44(){}

void func_800C1FB8(){}

void* func_800C22C4(cf::CfObjectImplWalker* self) { return (void*)((u8*)self + 0x68); }

void func_800C22CC(){}

void func_800C2714(){}

void func_800C2C90(){}

void func_800C2E3C(){}

void func_800C3658(){}

void func_800C36AC(){}

void func_800C3878(){}

void func_800C3A88(){}

void func_800C3AD4(){}

void func_800C3BF0(){}

void func_800C4244(){}

void func_800C4888(){}

void func_800C4BD4(){}

void func_800C4FB8(){}

void func_800C50F4(){}

void func_800C525C(){}

void func_800C551C(){}

void func_800C5928(void) {}

extern "C" void* __dt__Q22cf18CfObjectImplWalkerFv(void* self, int deleteFlag) {
    if (self != nullptr && deleteFlag > 0) {
        operator delete(self);
    }
    return self;
}

void func_800C596C(void) {}

u32 func_800C5970(cf::CfObjectImplWalker* self) { return *(u32*)((u8*)self + 0x380); }

extern "C" void func_800C6EC0(void* self);
void func_800C5978(char* self) { ((void(*)(void*))func_800C6EC0)(self - 0xc); }

void func_800C5980(char* self) { ((void(*)(void*))__dt__Q22cf18CfObjectImplWalkerFv)(self - 0xc); }

extern "C" void func_800CFFA0(void* self);
void func_800C5988(char* self) { ((void(*)(void*))func_800CFFA0)(self - 0x10); }

void func_800C5990(char* self) { ((void(*)(void*))__dt__Q22cf18CfObjectImplWalkerFv)(self - 0x10); }
