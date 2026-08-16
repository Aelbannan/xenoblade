// Auto-scaffolded catalog TU for kyoshin/code_801A929C
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __dl__FPv(void*);

void func_801A9338(void* self) {
    *(unsigned short*)((char*)self + 0x2c) = 0xFFFF;
}

void func_801A9348(){}

void func_801A96A0(){}

void func_801A9CCC(){}

extern "C" void* __dt__801A9F78(void* self, int dealloc) {
    if (self != nullptr) {
        extern unsigned char lbl_eu_80664330;
        *(void**)(&lbl_eu_80664330) = 0;
        if (dealloc > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void func_801A9FC0(){}

void func_801AA04C(){}

void func_801AA2A8(){}

extern "C" int func_801AA960(int, int b, int c, int d) {
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


void func_801AAAA0(){}

void func_801AAB64(){}

extern "C" void func_801AAB64(void* a, void* b, int c, float d);
extern "C" void func_801AAC70(void* self){ void* d; float dummy; func_801AAB64(self, d, 1, dummy); }

extern "C" void func_801AAC78(u8 v) {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p == 0) return;
    *(u8*)((u8*)p + 0x1A) = v;
    if (v == 0) return;
    extern float lbl_eu_80667D64;
    func_801AAB64(0, 0, 0, lbl_eu_80667D64);
}

extern "C" void func_801AACA8(unsigned char v) {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p) *(unsigned char*)((u8*)p + 0x1b) = v;
}

void func_801AACBC(unsigned int* r3, unsigned int* r4) {
    extern unsigned char lbl_eu_80664330;
    unsigned int *p = (unsigned int *)&lbl_eu_80664330;
    if (!*p) return;
    unsigned int *dst = (unsigned int *)*p;
    unsigned int *src1 = r3;
    unsigned int *src2 = r4;
    dst[0] = src1[0];
    dst[1] = src1[1];
    dst[2] = src1[2];
    dst[3] = src2[0];
    dst[4] = src2[1];
    dst[5] = src2[2];
    ((unsigned char *)dst)[0x18] = 1;
}

extern "C" void func_801AAD08() {
    extern unsigned char lbl_eu_80664330;
    void* p = *(void**)(&lbl_eu_80664330);
    if (p) *(unsigned char*)((u8*)p + 0x18) = 0;
}
