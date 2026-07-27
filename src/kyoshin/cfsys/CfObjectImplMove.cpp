// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cfsys/CfObjectImplMove.hpp"

void func_800CA948(){}

void func_800CA964(){}

void func_800CAA44(){}

void func_800CAB00(){}

void func_800CAB2C(void) {}

void func_800CAB30(){}

void func_800CB21C(){}

void func_800CB454(){}

void func_800CB94C(){}

void func_800CB9AC(){}

void func_800CBBD8(){}

void func_800CC01C(void) {}

void func_800CC020(){}

void func_800CC5DC(){}

void func_800CC638(){}

void func_800CC964(){}

void func_800CD268(){}

void func_800CD460(){}

void func_800BE824(void*, unsigned int);

void func_800CD5C0(CfObjectImplMoveData* self, unsigned int a, unsigned int b) {
    CfObjectImplMoveSubObj* inner = (CfObjectImplMoveSubObj*)self->mSubObj;
    if (a == inner->mSomeId) {
        func_800BE824(inner, b);
    }
}

void func_800CD5DC(){}

void func_800CE544(){}

void func_800CE6A0(){}

void func_800CE8AC(){}

void func_800CE8E4(){}

void func_800CEA34(){}

void func_800CEB68(){}

void func_800CEBE0(){}

void func_800CED64(){}

void func_800CEE28(){}

void func_800CEE7C(void) {}

void func_800CEE80(){}

void func_800CF064(){}

void func_800CF810(){}

void func_800CFFA0(unsigned int* self, unsigned int* param) {
    if (param == (unsigned int*)self[7]) {
        param[44] = 0;
        self[7] = 0;
    }
}

extern "C" void __dt__Q22cf16CfObjectImplMoveFv(void* self);
void func_800CFFBC(char* self) { ((void(*)(void*))__dt__Q22cf16CfObjectImplMoveFv)(self - 0xc); }

void func_800CFFC4(char* self) { ((void(*)(void*))__dt__Q22cf16CfObjectImplMoveFv)(self - 0x10); }
