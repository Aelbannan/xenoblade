// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQstCnt
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void __ct__CMenuQstCnt(){}

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

void CMenuQstCnt::~CMenuQstCnt() {}

void CMenuQstCnt::Init() {}

void CMenuQstCnt::Term() {}

void CMenuQstCnt::Move() {}

void CMenuQstCnt::cbRenderBefore() {}

void func_802269D8(){}

void func_80226B94(){}

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

void func_80226BBC(){}

void func_80226C18(){}

void func_80226C5C(){}

void func_80226C88(){}

extern "C" void func_80226E28(void *dest, const void *src) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    *(unsigned short *)(d + 0) = *(const unsigned short *)(s + 0);
    *(unsigned short *)(d + 2) = *(const unsigned short *)(s + 2);
    d[4] = s[4];
    d[5] = s[5];
    d[6] = s[6];
}

void func_80226E54(){}

extern "C" void func_80226F94(void* self) { ((void(*)(void*))__dt__11CMenuQstCntFv)((char*)self - 0x6c); }

extern "C" void func_80226F9C(void* self) { ((void(*)(void*))cbRenderBefore__11CMenuQstCntFv)((char*)self - 0x70); }

extern "C" void func_80226FA4(void* self) { ((void(*)(void*))__dt__11CMenuQstCntFv)((char*)self - 0x70); }

void func_80226FAC(){}

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

void __dt__80227030(){}

void __dt__80227070(){}

void func_802270CC(){}

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

void func_80227260(){}

void func_80227660(){}

void func_8022769C(){}

extern "C" unsigned short func_802276D8(unsigned char* p) {
    short v = *(short*)(p + 0x2002);
    if (v < 0) {
        v = *(unsigned short*)(p + 0x2000);
    }
    return (unsigned short)v;
}

void func_802276F4(){}

void func_80227710(){}
