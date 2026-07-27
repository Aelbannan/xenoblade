// Auto-scaffolded catalog TU for kyoshin/cf/CfMapEffectManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__cf_CfMapEffectManager(){}

void cf::CfMapEffectManager::~CfMapEffectManager() {}

void func_8016EEB0(){}

void func_8016EF2C(){}

void func_8016EFD8(){}

void func_8016F140(void){}

void func_8016F144(){}

void func_8016F2A4(){}

void func_8016F9D4(){}

void func_8016FA68(){}

void func_8016FBA8(){}

void func_8016FC0C(){}

void func_8016FC98(){}

void func_8016FD84(){}

extern float lbl_eu_806623E8;
extern "C" void func_8016FE2C(float val) { lbl_eu_806623E8 = val; }

extern "C" void* func_8016FE34(void* r3) {
    if (r3 == 0) return 0;
    int word = *(int*)((char*)r3 + 0x64);
    if ((word & 0x2) != 0 || (word & 0x4) != 0) {
        if (r3 != 0)
            return (char*)r3 - 0x3e9c;
    }
    return 0;
}
