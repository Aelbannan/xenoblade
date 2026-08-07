// Auto-scaffolded catalog TU for kyoshin/cf/CfObjectSelectorObj
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"

namespace cf {
    void CfObjectSelectorObj::func_800FE694(float val) {
        mField90F8 = val;
        mFieldC164 = val;
    }

    unsigned long CfObjectSelectorObj::func_800FE910() {
        return (mFieldC180 >> 10) & 1;
    }

    void CfObjectSelectorObj::func_800FE920() {
        char* obj = static_cast<char*>(mPtrC178);
        *reinterpret_cast<unsigned int*>(obj + 0x3068) |= 4;
    }

    void CfObjectSelectorObj::func_800FE938() {
        char* obj = static_cast<char*>(mPtrC178);
        *reinterpret_cast<unsigned int*>(obj + 0x3068) |= 8u;
    }

    void CfObjectSelectorObj::func_800FE950(unsigned int a, unsigned int b, unsigned int c) {
        mField608C = a;
        mField6094 = b;
        mField6098 = c;
        mField90E8 = a;
        mField90EC = b;
    }
}

cf::CfObjectSelectorObj* lbl_eu_80663F14;

cf::CfObjectSelectorObj* func_800FE68C() {
    return lbl_eu_80663F14;
}

void func_800FE6A4(){}

void func_800FE738(){}

void func_800FE7D8(){}

void func_800FE860(){}

void func_800FE96C(){}

void sinit_800FEA14(){}

extern "C" void func_800FD774() {}
extern "C" void func_800FDE4C() {}
extern "C" void func_800FE104() {}
