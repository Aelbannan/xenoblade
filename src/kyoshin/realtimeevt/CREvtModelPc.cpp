// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtModelPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__CREvtModelPc(){}

void __ct__8018385C(void* self){}

void func_801838D8(){}

void func_80183978(){}

void func_80183A3C(){}

void func_80183C1C(){}

void func_80183C90(){}

void func_801845F0(){}

void func_801846C4(){}

extern "C" u32 func_80184728(void* self) { return *(u32*)((u8*)self + 0x20); }

void func_80184730(){}

void func_801848EC(){}

extern "C" void func_8018496C(void* self) {
    *(unsigned long*)((char*)self + 0x18) &= ~0x20;
}

void func_8018497C(){}

void func_80184A24(void* self){}

extern "C" bool func_80184AE4(const void* self) {
    unsigned int flags = *reinterpret_cast<const unsigned int*>(static_cast<const char*>(self) + 0x18);
    return (flags & 0x1u) && (flags & 0x40u);
}

extern "C" void OnFileEvent__12CREvtModelPcFP10CEventFile(void* self) { ((void(*)(void*))func_80184A24)((char*)self - 0x38); }

extern "C" void func_80184B0C(void* self) { ((void(*)(void*))__ct__8018385C)((char*)self - 0x38); }
