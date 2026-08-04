// Auto-scaffolded catalog TU for monolib/src/scn/CScnItemPool
#include <harness_catalog.h>

struct CScnItemPoolState {
    u32 value00;
    u8 _04[4];
    u16 value08;
    u8 _0A[0x0E];
    u32 value18;
    u8 _1C[0x70];
    u32 value8C;
};

u32 func_8048C478(void* self) { return ((CScnItemPoolState*)self)->value18; }
u16 func_8048C690(void* self) { return ((CScnItemPoolState*)self)->value08; }
u32 func_8048C8BC(void* self) { return ((CScnItemPoolState*)self)->value8C; }
u32 func_8048C9F4(void* self) { return ((CScnItemPoolState*)self)->value00; }
void func_8048C524() {}
extern "C" void func_8048C5AC(int* dst, const void* src) { *(u32*)dst = *(const u32*)((const u8*)src + 4); }
void func_8048C5B8() {}
void func_8048C60C() {}
void func_8048C630() {}
extern "C" void* func_8048C698(void* self, int kind) {
    switch (kind) {
    case 1:
        return (void*)((char*)self + 0x0C);
    case 2:
        return (void*)((char*)self + 0x2C);
    case 3:
        return (void*)((char*)self + 0x4C);
    case 4:
        return (void*)((char*)self + 0x6C);
    case 5:
        return (void*)((char*)self + 0x8C);
    default:
        return nullptr;
    }
}
extern "C" void* func_8048C6F4(void* self, s32 kind) {
    switch (kind) {
    case 1: return (u8*)self + 0xC;
    case 2: return (u8*)self + 0x2C;
    case 3: return (u8*)self + 0x4C;
    case 4: return (u8*)self + 0x6C;
    case 5: return (u8*)self + 0x8C;
    default: return 0;
    }
}
void func_8048C750() {}
void func_8048C8C4() {}
void func_8048C994() {}
extern "C" void func_8048C9C8(int* self) { *(u32*)self = *(u32*)(*(u32**)self); }
void func_8048C9D8() {}
extern "C" void func_8048CA00(u32* dst, const void* src) { *(u32*)dst = *(u32*)(*(u32**)((const u8*)src + 4)); }
void func_8048CA10() {}
extern "C" void func_8048CA28(int* dst, int* src) { *dst = *src; }
void func_8048CA34() {}
void func_8048CA5C() {}
void func_8048CB14() {}
void func_8048CBC0() {}
void func_8048CC40() {}
void func_8048CCC0() {}
void func_8048CD0C() {}
void func_8048CDA8(void) {}
extern "C" void func_8048CDAC(int* dst, int* src) { *dst = *src; }
