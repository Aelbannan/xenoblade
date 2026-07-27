// Auto-scaffolded catalog TU for kyoshin/code_801A929C
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void func_801A9338(void* self) {
    *(unsigned short*)((char*)self + 0x2c) = 0xFFFF;
}

void func_801A9348(){}

void func_801A96A0(){}

void func_801A9CCC(){}

void __dt__801A9F78(){}

void func_801A9FC0(){}

void func_801AA04C(){}

void func_801AA2A8(){}

void func_801AA960(){}

void func_801AAAA0(){}

void func_801AAB64(){}

void func_801AAC70(void* self){ func_801AAB64(); }

void func_801AAC78(){}

void func_801AACA8(){}

extern "C" void func_801AACBC(void *r3, void *r4) {
    extern unsigned char lbl_eu_80664330;
    unsigned int *p = (unsigned int *)&lbl_eu_80664330;
    if (!*p) return;
    unsigned int *dst = (unsigned int *)*p;
    unsigned int *src1 = (unsigned int *)r3;
    unsigned int *src2 = (unsigned int *)r4;
    dst[0] = src1[0];
    dst[1] = src1[1];
    dst[2] = src1[2];
    dst[3] = src2[0];
    dst[4] = src2[1];
    dst[5] = src2[2];
    ((unsigned char *)dst)[0x18] = 1;
}

void func_801AAD08(){}
