// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplPc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void func_800C5998() {}

extern "C" void func_800C5AA0() {}

extern "C" void func_800C5B00() {}

extern "C" void func_800C6A58() {}

extern "C" void func_800C6EC0() {}

extern "C" void func_800C6F30() {}

extern "C" void func_800C70BC() {}

extern "C" void func_800C75D4() {}

extern "C" void func_800C819C() {}

extern "C" void func_800C86E8() {}

extern "C" void func_800C891C() {}

extern "C" void func_800C969C() {}

extern "C" void func_800C9A20() {}

extern "C" void func_800C9CEC() {}

extern "C" void func_800CA084() {}

extern "C" void func_800CA104() {}

extern "C" void func_800CEA34(void*);

extern "C" void func_800CA274(void* self, int value)
{
    char* sub = *(char**)((char*)self + 0x18);
    if (*(int*)(sub + 0x3e98) != 0 && value != 0) {
        *(int*)(sub + 0x3e98) = value;
    }
    func_800CEA34(self);
}

extern "C" void func_800CA294() {}

extern "C" void func_800CA42C() {}

extern "C" void func_800CA458() {}

extern "C" void __dt__Q22cf14CfObjectImplPcFv(void* self);

// Adjuster thunk for CfObjectImplPc virtual destructor at vtable slot this-0xc.
// Adjusts this pointer to the complete object, then tail-calls the real destructor.
extern "C" void func_800CA4DC(void* self)
{
    __dt__Q22cf14CfObjectImplPcFv((u8*)self - 0xc);
}

// Adjuster thunk for CfObjectImplPc virtual destructor at vtable slot this-0x10.
// Adjusts this pointer to the complete object, then tail-calls the real destructor.
extern "C" void func_800CA4E4(void* self)
{
    __dt__Q22cf14CfObjectImplPcFv((u8*)self - 0x10);
}
