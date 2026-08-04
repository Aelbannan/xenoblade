// Auto-scaffolded catalog TU for monolib/src/scn/CScnFogMan
#include <harness_catalog.h>
#include "libs/monolib/src/scn/CScnFogMan.hpp"

extern "C" void func_8049DE68(void* self, u32 val) {
    ((CScnFogMan*)self)->value08 = val;
}
void func_8049DE70(void) {}
void func_8049E374(void* self, float a, float b) {}

CScnFogMan::~CScnFogMan() {}

extern "C" void func_8049E350(void* self, const void* src) {
    *(u32*)((u8*)self + 0xC) = *(u32*)((u8*)src + 0);
    *(u32*)((u8*)self + 0x10) = *(u32*)((u8*)src + 4);
    *(u32*)((u8*)self + 0x14) = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x18) = *(u32*)((u8*)src + 0xC);
}
