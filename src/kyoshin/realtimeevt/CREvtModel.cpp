// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtModel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModel.hpp"

void __ct__CREvtModel(){}

void __ct__80172668(){}

void func_801726DC(){}

void func_80172768(){}

void func_801727D0(){}

void func_801727DC(){}

void func_801728F8(){}

void func_8017298C(){}

extern "C" int func_801729D0(char* p)
{
    char* q = *(char**)(p + 0x1c);
    if (q == 0) {
        return 0;
    }
    return (*(unsigned int*)(q + 0x58) >> 4) & 1;
}

void func_801729F0(){}

// Trivial no-op placeholder. The retail function is a bare blr — no
// initialization or side effects are required at this stub stage.
extern "C" void func_80172CC0(void) {}

void func_80172CC4(){}

void func_80172CE4(){}

void func_80172EA4(){}

void func_801730D0(){}

void func_80173194(){}

extern "C" u32 func_801731A0(void* self) { return (*(u32*)((u8*)self + 24) >> 4) & 0x1u; }

// Stub: unconditionally returns 0. Accepts a CREvtModel pointer for ABI
// compatibility but does not access it.
extern "C" int func_801731AC(CREvtModel* self) { return 0; }
