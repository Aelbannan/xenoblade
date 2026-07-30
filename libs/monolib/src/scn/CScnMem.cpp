// Auto-scaffolded catalog TU for monolib/src/scn/CScnMem
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

u32 func_8048ECD0(void* self) { return *(u32*)((u8*)self + 0x8c); }







void func_8048ECD8(void){}

extern void func_8048FDDC();
void func_8048ECE4(void* self){ func_8048FDDC(); }

extern void func_8048FDE4();
void func_8048ECEC(void* self){ func_8048FDE4(); }

extern void func_8048FDEC();
void func_8048ECF4(void* self){ func_8048FDEC(); }

extern void func_8048FE0C();
void func_8048ECFC(void* self){ func_8048FE0C(); }

extern void func_8048FE2C();
void func_8048ED04(void* self){ func_8048FE2C(); }

extern void func_80496970(void* self);
extern "C" void WorkEvent4__4CScnFv(void* self) { ((void(*)(void*))func_80496970)((char*)self - 0x54); }

extern "C" void func_8049695C(void* self);
extern "C" void WorkEvent3__4CScnFPv(void* self, void* r4) { ((void(*)(void*))func_8049695C)((char*)self - 0x54); }

extern "C" void __dt__8CScnNw4rFv(void* self, int deleteFlag);
void func_8048ED1C(void* self){ ((void(*)(void*))__dt__8CScnNw4rFv)((char*)self - 0x54); }
