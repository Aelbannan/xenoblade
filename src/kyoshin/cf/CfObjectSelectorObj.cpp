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

cf::CfObjectSelectorObj* lbl_eu_80663F14;

cf::CfObjectSelectorObj* func_800FE68C() {
    return lbl_eu_80663F14;
}

void func_800FE694(cf::CfObjectSelectorObj* self, float val) {
    self->mField90F8 = val;
    self->mFieldC164 = val;
}

void func_800FE6A4(){}

void func_800FE738(){}

void func_800FE7D8(){}

void func_800FE860(){}

unsigned long func_800FE910(cf::CfObjectSelectorObj* self) {
    return (self->mFieldC180 >> 10) & 1;
}

void func_800FE920(cf::CfObjectSelectorObj* p)
{
    char* obj = static_cast<char*>(p->mPtrC178);
    *reinterpret_cast<unsigned int*>(obj + 0x3068) |= 4;
}

void func_800FE938(cf::CfObjectSelectorObj* param_1) {
    char* obj = static_cast<char*>(param_1->mPtrC178);
    *reinterpret_cast<unsigned int*>(obj + 0x3068) |= 8u;
}

void func_800FE950(cf::CfObjectSelectorObj* this_, unsigned int a, unsigned int b, unsigned int c) {
    this_->mField608C = a;
    this_->mField6094 = b;
    this_->mField6098 = c;
    this_->mField90E8 = a;
    this_->mField90EC = b;
}

void func_800FE96C(){}

void sinit_800FEA14(){}
