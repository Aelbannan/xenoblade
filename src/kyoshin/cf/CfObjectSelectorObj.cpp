// Auto-scaffolded catalog TU for kyoshin/cf/CfObjectSelectorObj
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"

void func_800FD774(){}

void __ct__800FDB4C(){}

void __dt__800FDC1C(){}

void func_800FDE4C(){}

void __dt__800FDEF8(){}

void func_800FE104(){}

// spInstance for the CfObjectSelectorObj singleton
cf::CfObjectSelectorObj* lbl_eu_80663F14;

// Returns the CfObjectSelectorObj singleton instance
cf::CfObjectSelectorObj* func_800FE68C() {
    return lbl_eu_80663F14;
}

void func_800FE694(void* self, float val) {
    float* base = (float*)((char*)self + 0x10000);
    base[-0x6f08 / 4] = val;
    base[-0x3e9c / 4] = val;
}

void func_800FE6A4(){}

void func_800FE738(){}

void func_800FE7D8(){}

void func_800FE860(){}

unsigned long func_800FE910(void* self) {
    unsigned long* base = (unsigned long*)((char*)self + 0x10000);
    unsigned long v = base[-0x3e80 / 4];
    return (v >> 10) & 1;
}

void func_800FE920(char* p)
{
    char* obj = *(char**)(p + 0xC178);
    *(unsigned int*)(obj + 0x3068) |= 4;
}

void func_800FE938(char* param_1) {
    char* obj = *(char**)(param_1 + 0xC178);
    *(unsigned int*)(obj + 0x3068) |= 8u;
}

void func_800FE950(char* this_, unsigned int a, unsigned int b, unsigned int c) {
    *(unsigned int*)(this_ + 0x608c) = a;
    *(unsigned int*)(this_ + 0x6094) = b;
    *(unsigned int*)(this_ + 0x6098) = c;
    *(unsigned int*)(this_ + 0x90e8) = a;
    *(unsigned int*)(this_ + 0x90ec) = b;
}

void func_800FE96C(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_800FEA14(){}
