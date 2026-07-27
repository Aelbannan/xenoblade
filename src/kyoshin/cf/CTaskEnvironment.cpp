// Auto-scaffolded catalog TU for kyoshin/cf/CTaskEnvironment
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CTaskEnvironment.hpp"
void __ct__CTaskEnvironment(){}

void copyWord4(u32* dst, const u32* src) {
    const u32* s = src;
    u32* d = dst;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

bool func_8005870C(){ return false; }

u32 getSubField78(void* self) { return *(u32*)((u8*)self + 0x78); }

bool func_8005871C(){ return false; }

bool func_80058724(){ return false; }

void CTaskEnvironment::~CTaskEnvironment() {}

bool getGlobalSda() { return false; }

u32 getSubField7C(void* self) { return *(u32*)((u8*)self + 0x7C); }

void func_800587F8(){}

u32 getField5C(void* self) { return *(u32*)((u8*)self + 0x5C); }

void func_80058844(){}

void func_80058BD8(){}

void CTaskEnvironment::Init() {}

void CTaskEnvironment::Term() {}

void CTaskEnvironment::Move() {}

void nopFunc() {}

void func_80059610(){}

extern float lbl_eu_8066A210;
float scaleByGlobal(float val) { return val * lbl_eu_8066A210; }

void func_800596B8(){}

void func_800598A8(){}

u32 checkBitFlag(void* self) { return (*(u32*)((u8*)self + 0) >> 1) & 0x1u; }

u32 getField68(CTaskEnvironment* self) { return *(u32*)((u8*)self + 0x68); }

void func_80059920(){}

void func_80059944(){}

void func_80059974(){}

void copyWord4Offset(u32* dst, const u32* src) {
    dst[1] = src[0];
    dst[2] = src[1];
    dst[3] = src[2];
    dst[4] = src[3];
}



void func_800599E0(){}

void func_80059A48(){}

void func_80059C14(){}

void func_80059C58(){}

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
void sinit_80059D60(){}
