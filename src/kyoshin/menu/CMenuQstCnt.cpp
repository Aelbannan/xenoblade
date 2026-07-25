// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQstCnt
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802280f8
extern "C" void __ct__CMenuQstCnt() {}
// LLM-HARNESS-END: us-802280f8

// LLM-HARNESS-BEGIN: us-802282ac
extern "C" void func_8022646C(void* self, short a, short b, unsigned char c, unsigned char d, unsigned char e)
{
    struct S {
        short f0;
        short f2;
        unsigned char f4;
        unsigned char f5;
        unsigned char f6;
    }* p = (struct S*)self;
    p->f0 = a;
    p->f2 = b;
    p->f4 = c;
    p->f5 = d;
    p->f6 = e;
}
// LLM-HARNESS-END: us-802282ac

// LLM-HARNESS-BEGIN: us-802282c4
extern "C" void func_80226484(void* dst, const void* src) {
    unsigned short tmp0 = *(unsigned short*)((unsigned char*)src + 0);
    unsigned short tmp1 = *(unsigned short*)((unsigned char*)src + 2);
    unsigned char tmp2 = *((unsigned char*)src + 4);
    unsigned char tmp3 = *((unsigned char*)src + 5);
    unsigned char tmp4 = *((unsigned char*)src + 6);
    *(unsigned short*)((unsigned char*)dst + 0) = tmp0;
    *(unsigned short*)((unsigned char*)dst + 2) = tmp1;
    *((unsigned char*)dst + 4) = tmp2;
    *((unsigned char*)dst + 5) = tmp3;
    *((unsigned char*)dst + 6) = tmp4;
}
// LLM-HARNESS-END: us-802282c4

// LLM-HARNESS-BEGIN: us-802282f0
extern "C" void __dt__11CMenuQstCntFv(void* self) {}
// LLM-HARNESS-END: us-802282f0

// LLM-HARNESS-BEGIN: us-80228350
extern "C" void Init__11CMenuQstCntFv() {}
// LLM-HARNESS-END: us-80228350

// LLM-HARNESS-BEGIN: us-80228540
extern "C" void Term__11CMenuQstCntFv() {}
// LLM-HARNESS-END: us-80228540

// LLM-HARNESS-BEGIN: us-802285c0
extern "C" void Move__11CMenuQstCntFv() {}
// LLM-HARNESS-END: us-802285c0

// LLM-HARNESS-BEGIN: us-80228708
extern "C" void cbRenderBefore__11CMenuQstCntFv(void* self) {}
// LLM-HARNESS-END: us-80228708

// LLM-HARNESS-BEGIN: us-80228818
extern "C" void func_802269D8() {}
// LLM-HARNESS-END: us-80228818

// LLM-HARNESS-BEGIN: us-802289cc
extern "C" void func_80226B94() {}
// LLM-HARNESS-END: us-802289cc

// LLM-HARNESS-BEGIN: us-802289dc
struct Unk80226BA4 {
    unsigned char pad[0x64];
    unsigned char flag;
};

extern "C" Unk80226BA4* lbl_eu_80664720;

extern "C" void func_80226BA4() {
    if (lbl_eu_80664720 != 0) {
        lbl_eu_80664720->flag = 1;
    }
}
// LLM-HARNESS-END: us-802289dc

// LLM-HARNESS-BEGIN: us-802289f4
extern "C" void func_80226BBC() {}
// LLM-HARNESS-END: us-802289f4

// LLM-HARNESS-BEGIN: us-80228a50
extern "C" void func_80226C18() {}
// LLM-HARNESS-END: us-80228a50

// LLM-HARNESS-BEGIN: us-80228a94
extern "C" void func_80226C5C() {}
// LLM-HARNESS-END: us-80228a94

// LLM-HARNESS-BEGIN: us-80228ac0
extern "C" void func_80226C88() {}
// LLM-HARNESS-END: us-80228ac0

// LLM-HARNESS-BEGIN: us-80228c60
extern "C" void func_80226E28(void *dest, const void *src) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    *(unsigned short *)(d + 0) = *(const unsigned short *)(s + 0);
    *(unsigned short *)(d + 2) = *(const unsigned short *)(s + 2);
    d[4] = s[4];
    d[5] = s[5];
    d[6] = s[6];
}
// LLM-HARNESS-END: us-80228c60

// LLM-HARNESS-BEGIN: us-80228c8c
extern "C" void func_80226E54() {}
// LLM-HARNESS-END: us-80228c8c

// LLM-HARNESS-BEGIN: us-80228dcc
extern "C" void func_80226F94(void* self) { ((void(*)(void*))__dt__11CMenuQstCntFv)((char*)self - 0x6c); }
// LLM-HARNESS-END: us-80228dcc

// LLM-HARNESS-BEGIN: us-80228dd4
extern "C" void func_80226F9C(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuQstCntFv)((char*)self - 0x70); }
// LLM-HARNESS-END: us-80228dd4

// LLM-HARNESS-BEGIN: us-80228ddc
extern "C" void func_80226FA4(void* self) { ((void(*)(void*))__dt__11CMenuQstCntFv)((char*)self - 0x70); }
// LLM-HARNESS-END: us-80228ddc

// LLM-HARNESS-BEGIN: us-80228de4
extern "C" void func_80226FAC() {}
// LLM-HARNESS-END: us-80228de4

// LLM-HARNESS-BEGIN: us-80228e44
extern "C" void func_8022700C(void* ptr) {
    unsigned char* p = (unsigned char*)ptr;
    *((short*)p) = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
    p[7] = 0;
}
// LLM-HARNESS-END: us-80228e44

// LLM-HARNESS-BEGIN: us-80228e68
extern "C" void __dt__80227030() {}
// LLM-HARNESS-END: us-80228e68

// LLM-HARNESS-BEGIN: us-80228ea8
extern "C" void __dt__80227070() {}
// LLM-HARNESS-END: us-80228ea8

// LLM-HARNESS-BEGIN: us-80228f04
extern "C" void func_802270CC() {}
// LLM-HARNESS-END: us-80228f04

// LLM-HARNESS-BEGIN: us-8022905c
extern "C" void func_80227224(void *dst, const void *src) {
    unsigned short *d16 = (unsigned short*)dst;
    unsigned char *d8 = (unsigned char*)dst;
    const unsigned short *s16 = (const unsigned short*)src;
    const unsigned char *s8 = (const unsigned char*)src;
    d16[0] = s16[0];
    d8[2] = s8[2];
    d8[3] = s8[3];
    d8[4] = s8[4];
    d8[5] = s8[5];
    d8[6] = s8[6];
    d8[7] = s8[7];
}
// LLM-HARNESS-END: us-8022905c

// LLM-HARNESS-BEGIN: us-80229098
extern "C" void func_80227260() {}
// LLM-HARNESS-END: us-80229098

// LLM-HARNESS-BEGIN: us-80229498
extern "C" void func_80227660() {}
// LLM-HARNESS-END: us-80229498

// LLM-HARNESS-BEGIN: us-802294d4
extern "C" void func_8022769C() {}
// LLM-HARNESS-END: us-802294d4

// LLM-HARNESS-BEGIN: us-80229510
extern "C" unsigned short func_802276D8(unsigned char* p) {
    short v = *(short*)(p + 0x2002);
    if (v < 0) {
        v = *(unsigned short*)(p + 0x2000);
    }
    return (unsigned short)v;
}
// LLM-HARNESS-END: us-80229510

// LLM-HARNESS-BEGIN: us-8022952c
extern "C" void func_802276F4() {}
// LLM-HARNESS-END: us-8022952c

// LLM-HARNESS-BEGIN: us-80229548
extern "C" void func_80227710() {}
// LLM-HARNESS-END: us-80229548
