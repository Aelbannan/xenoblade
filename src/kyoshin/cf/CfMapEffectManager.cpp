// Auto-scaffolded catalog TU for kyoshin/cf/CfMapEffectManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CfMapEffectManager.hpp"

namespace cf {}
using namespace cf;
void __ct__cf_CfMapEffectManager(){}

cf::CfMapEffectManager::~CfMapEffectManager() {}

void func_8016EEB0(){}

void func_8016EF2C(){}

void func_8016EFD8(){}

void func_8016F140(void){}

void func_8016F144(){}

void func_8016F2A4(){}

void func_8016F9D4(){}

extern "C" int func_8016FA68(int, int b, int c, int d) {
    if (d == 1 && b == 1) return 1;
    if (d == 2 && b == 2) return 1;
    if (d == 3 && b == 3) return 1;
    if (d == 4 && b == 4) return 1;
    if (d == 5 && (unsigned int)c <= 2) return 1;
    if (d == 6 && (unsigned int)(c - 3) <= 2) return 1;
    if (d == 7 && (unsigned int)(c - 6) <= 2) return 1;
    if (d == 8 && (unsigned int)(c - 9) <= 2) return 1;
    if (d == 9 && (unsigned int)(c - 0xc) <= 2) return 1;
    if (d == 0xa && (unsigned int)(c - 0xf) <= 2) return 1;
    if (d == 0xb && (unsigned int)(c - 0x12) <= 2) return 1;
    if (d == 0xc && (unsigned int)(c - 0x15) <= 2) return 1;
    return 0;
}


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
