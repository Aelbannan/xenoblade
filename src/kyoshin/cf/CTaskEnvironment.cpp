// Auto-scaffolded catalog TU for kyoshin/cf/CTaskEnvironment
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-80058be4
extern "C" void __ct__CTaskEnvironment() {}
// LLM-HARNESS-END: us-80058be4

// LLM-HARNESS-BEGIN: us-80058d50
extern "C" void func_800586E8() {}
// LLM-HARNESS-END: us-80058d50

// LLM-HARNESS-BEGIN: us-80058d74
extern "C" bool func_8005870C() { return false; }
// LLM-HARNESS-END: us-80058d74

// LLM-HARNESS-BEGIN: us-80058d7c
extern "C" u32 func_80058714(void* self) { return *(u32*)((u8*)self + 0x78); }
// LLM-HARNESS-END: us-80058d7c

// LLM-HARNESS-BEGIN: us-80058d84
extern "C" bool func_8005871C() { return false; }
// LLM-HARNESS-END: us-80058d84

// LLM-HARNESS-BEGIN: us-80058d8c
extern "C" bool func_80058724() { return false; }
// LLM-HARNESS-END: us-80058d8c

// LLM-HARNESS-BEGIN: us-80058dec
extern "C" void __dt__16CTaskEnvironmentFv() {}
// LLM-HARNESS-END: us-80058dec

// LLM-HARNESS-BEGIN: us-80058e50
extern "C" bool func_800587E8() { return false; }
// LLM-HARNESS-END: us-80058e50

// LLM-HARNESS-BEGIN: us-80058e58
extern "C" u32 func_800587F0(void* self) { return *(u32*)((u8*)self + 0x7C); }
// LLM-HARNESS-END: us-80058e58

// LLM-HARNESS-BEGIN: us-80058e60
extern "C" void func_800587F8() {}
// LLM-HARNESS-END: us-80058e60

// LLM-HARNESS-BEGIN: us-80058ea4
extern "C" u32 func_8005883C(void* self) { return *(u32*)((u8*)self + 0x5C); }
// LLM-HARNESS-END: us-80058ea4

// LLM-HARNESS-BEGIN: us-80058eac
extern "C" void func_80058844() {}
// LLM-HARNESS-END: us-80058eac

// LLM-HARNESS-BEGIN: us-80059240
extern "C" void func_80058BD8() {}
// LLM-HARNESS-END: us-80059240

// LLM-HARNESS-BEGIN: us-80059270
extern "C" void Init__16CTaskEnvironmentFv() {}
// LLM-HARNESS-END: us-80059270

// LLM-HARNESS-BEGIN: us-800592c0
extern "C" void Term__16CTaskEnvironmentFv() {}
// LLM-HARNESS-END: us-800592c0

// LLM-HARNESS-BEGIN: us-80059350
extern "C" void Move__16CTaskEnvironmentFv() {}
// LLM-HARNESS-END: us-80059350

// LLM-HARNESS-BEGIN: us-80059c74
extern "C" void func_8005960C() {}
// LLM-HARNESS-END: us-80059c74

// LLM-HARNESS-BEGIN: us-80059c78
extern "C" void func_80059610() {}
// LLM-HARNESS-END: us-80059c78

// LLM-HARNESS-BEGIN: us-80059d14
extern "C" void func_800596AC() {}
// LLM-HARNESS-END: us-80059d14

// LLM-HARNESS-BEGIN: us-80059d20
extern "C" void func_800596B8() {}
// LLM-HARNESS-END: us-80059d20

// LLM-HARNESS-BEGIN: us-80059f10
extern "C" void func_800598A8() {}
// LLM-HARNESS-END: us-80059f10

// LLM-HARNESS-BEGIN: us-80059f74
extern "C" void func_8005990C() {}
// LLM-HARNESS-END: us-80059f74

// LLM-HARNESS-BEGIN: us-80059f80
extern "C" u32 func_80059918(void* self) { return *(u32*)((u8*)self + 0x68); }
// LLM-HARNESS-END: us-80059f80

// LLM-HARNESS-BEGIN: us-80059f88
extern "C" void func_80059920() {}
// LLM-HARNESS-END: us-80059f88

// LLM-HARNESS-BEGIN: us-80059fac
extern "C" void func_80059944() {}
// LLM-HARNESS-END: us-80059fac

// LLM-HARNESS-BEGIN: us-80059fdc
extern "C" void func_80059974() {}
// LLM-HARNESS-END: us-80059fdc

// LLM-HARNESS-BEGIN: us-8005a020
extern "C" void func_800599B8() {}
// LLM-HARNESS-END: us-8005a020

// LLM-HARNESS-BEGIN: us-8005a044
extern "C" void Draw__16CTaskEnvironmentFv() {}
// LLM-HARNESS-END: us-8005a044

// LLM-HARNESS-BEGIN: us-8005a048
extern "C" void func_800599E0() {}
// LLM-HARNESS-END: us-8005a048

// LLM-HARNESS-BEGIN: us-8005a0b0
extern "C" void func_80059A48() {}
// LLM-HARNESS-END: us-8005a0b0

// LLM-HARNESS-BEGIN: us-8005a27c
extern "C" void func_80059C14() {}
// LLM-HARNESS-END: us-8005a27c

// LLM-HARNESS-BEGIN: us-8005a2c0
extern "C" void func_80059C58() {}
// LLM-HARNESS-END: us-8005a2c0

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

class CTaskEnvironment;
// LLM-HARNESS-BEGIN: us-80058d94
template<> CTTask<CTaskEnvironment>::~CTTask() {}
// LLM-HARNESS-END: us-80058d94
// LLM-HARNESS-BEGIN: us-8005a338
template<> void CTTask<CTaskEnvironment>::Move() {}
// LLM-HARNESS-END: us-8005a338
// LLM-HARNESS-BEGIN: us-8005a380
template<> void CTTask<CTaskEnvironment>::Draw() {}
// LLM-HARNESS-END: us-8005a380
// LLM-HARNESS-BEGIN: us-8005a3c8
extern "C" void sinit_80059D60() {}
// LLM-HARNESS-END: us-8005a3c8
