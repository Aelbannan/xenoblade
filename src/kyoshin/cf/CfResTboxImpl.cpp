// Translation unit for kyoshin/cf/CfResTboxImpl
// Resource textbox implementation.

#include "kyoshin/cf/CfResTboxImpl.hpp"

using namespace cf;

void __ct__cf_CfResTboxImpl(){}

int func_801F8E70() { return 256; }

void func_801F8E78(){}

void func_801F8EB0(){}

void func_801F91B0(void) {}

void func_801F91B4(){}

cf::CfResTboxImpl::~CfResTboxImpl() {}

bool func_801F9268(unsigned char* p, int i, int j) {
    return p[i * 0x49 + j * 2] != 0;
}

void func_801F9288(){}

extern "C" void func_801F92B0(unsigned char* base, int idx1, int idx2, int idx3) {
    int offset = idx1 * 73 + idx2 * 16 + idx3 * 2;
    base[offset + 1] |= 0x40;
}
