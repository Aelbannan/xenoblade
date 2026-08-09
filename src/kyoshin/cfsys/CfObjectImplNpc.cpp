// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplNpc
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cfsys/CfObjectImplNpc.hpp"

void func_800CA6E4(){}

void func_800CA7B4(){}

void func_800CA850(void) {}

void func_800CA854(){}

extern "C" void* __dt__Q22cf15CfObjectImplNpcFv(cf::CfObjectImplNpc* self,
                                               int deleteFlag) {
    if (self != nullptr && deleteFlag > 0) {
        operator delete(self);
    }
    return self;
}

u8* cf::CfObjectImplNpc::func_800CA924() { return this->mUnk_0x68; }

void func_800CA92C() {}

void func_800CA930() {}

void func_800CA934() {}

void cf::CfObjectImplNpc::func_800CA938() { __dt__Q22cf15CfObjectImplNpcFv(this, 0); }

void cf::CfObjectImplNpc::func_800CA940() { __dt__Q22cf15CfObjectImplNpcFv(this, 0); }
