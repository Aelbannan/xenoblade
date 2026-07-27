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

struct CREvtModelOuter {
    u8 _pad00[0x1C];
    void* mPtr;  // 0x1C
};

struct CREvtModelInner {
    u8 _pad00[0x58];
    u32 mFlags;  // 0x58
};

int func_801729D0(char* p)
{
    void* q = reinterpret_cast<CREvtModelOuter*>(p)->mPtr;
    if (q == 0) {
        return 0;
    }
    return (static_cast<CREvtModelInner*>(q)->mFlags >> 4) & 1;
}

void func_801729F0(){}

// Trivial no-op placeholder. The retail function is a bare blr — no
// initialization or side effects are required at this stub stage.
void func_80172CC0(void) {}

void func_80172CC4(){}

void func_80172CE4(){}

void func_80172EA4(){}

void func_801730D0(){}

void func_80173194(){}

extern "C" u32 func_801731A0(void* self) { return (*(u32*)((u8*)self + 24) >> 4) & 0x1u; }

// Stub: unconditionally returns 0. Accepts a CREvtModel pointer for ABI
// compatibility but does not access it.
extern "C" int func_801731AC(CREvtModel* self) { return 0; }
