// Auto-scaffolded catalog TU for kyoshin/cf/CTaskEnvironment
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CTaskEnvironment() {}

extern "C" void func_800586E8(void *dst, const void *src) {
    const unsigned int *s = (const unsigned int *)src;
    unsigned int *d = (unsigned int *)dst;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

extern "C" bool func_8005870C() { return false; }

extern "C" u32 func_80058714(void* self) { return *(u32*)((u8*)self + 0x78); }

extern "C" bool func_8005871C() { return false; }

extern "C" bool func_80058724() { return false; }

extern "C" void __dt__16CTaskEnvironmentFv() {}

extern "C" bool func_800587E8() { return false; }

extern "C" u32 func_800587F0(void* self) { return *(u32*)((u8*)self + 0x7C); }

extern "C" void func_800587F8() {}

extern "C" u32 func_8005883C(void* self) { return *(u32*)((u8*)self + 0x5C); }

extern "C" void func_80058844() {}

extern "C" void func_80058BD8() {}

extern "C" void Init__16CTaskEnvironmentFv() {}

extern "C" void Term__16CTaskEnvironmentFv() {}

extern "C" void Move__16CTaskEnvironmentFv() {}

extern "C" void func_8005960C() {}

extern "C" void func_80059610() {}

extern float lbl_eu_8066A210;
extern "C" float func_800596AC(float val) { return val * lbl_eu_8066A210; }

extern "C" void func_800596B8() {}

extern "C" void func_800598A8() {}

extern "C" u32 func_8005990C(void* self) { return (*(u32*)((u8*)self + 0) >> 1) & 0x1u; }

extern "C" u32 func_80059918(void* self) { return *(u32*)((u8*)self + 0x68); }

extern "C" void func_80059920() {}

extern "C" void func_80059944() {}

extern "C" void func_80059974() {}

extern "C" void func_800599B8(unsigned int* dst, const unsigned int* src) {
    dst[1] = src[0];
    dst[2] = src[1];
    dst[3] = src[2];
    dst[4] = src[3];
}

extern "C" void Draw__16CTaskEnvironmentFv() {}

extern "C" void func_800599E0() {}

extern "C" void func_80059A48() {}

extern "C" void func_80059C14() {}

extern "C" void func_80059C58() {}

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
template<> CTTask<CTaskEnvironment>::~CTTask() {}
template<> void CTTask<CTaskEnvironment>::Move() {}
template<> void CTTask<CTaskEnvironment>::Draw() {}
extern "C" void sinit_80059D60() {}
