// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

cf::CfObjectEff::CfObjectEff() {}

void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv() {}

void cf::CfObject::~CfObject() {}

void CfObject_UnkVirtualFunc6__Q22cf8CfObjectFv() {}

void cf::CfObjectEff::func_800AC7CC() {}

void cf::CfObjectEff::func_800AC7FC() {}

void cf::CfObjectEff::func_800AC810() {}

void cf::CfObjectEff::func_800AC86C() {}

void CfObjectModel_UnkVirtualFunc18__Q22cf13CfObjectModelFv() {}

void cf::CfObjectEff::func_800AC990() {}

void cf::CfObjectEff::func_800ACA58() {}

void cf::CfObjectEff::func_800ACAE8() {}

void cf::CfObjectEff::func_800ACB08() {}

void cf::CfObjectEff::func_800ACBA4() {}

void cf::CfObjectEff::func_800ACBCC() {}

void func_800ACC14(void* self, unsigned char val) {
    void* ptr = *(void**)((char*)self + 0x94);
    if (ptr != nullptr)
        *(unsigned char*)((char*)ptr + 0x59) = val;
}

void func_800ACC28(){}

void cf::CfObjectEff::func_800ACC3C() {}

void func_800ACC50(){}

void func_800ACC64(void* obj, const void* src) {
    void* dest = *(void**)((unsigned char*)obj + 0x94);
    if (dest == 0) return;
    unsigned int* d = (unsigned int*)((unsigned char*)dest + 0x40);
    const unsigned int* s = (const unsigned int*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

void cf::CfObjectEff::func_800ACC94() {}

void cf::CfObjectEff::func_800ACCD4() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[156 / 4];
    func(this);
}

void cf::CfObjectEff::func_800ACCE4() {}

void cf::CfObjectEff::func_800ACD4C() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[180 / 4];
    func(this);
}

void cf::CfObjectEff::func_800ACD5C() {}

void cf::CfObjectEff::func_800ACDA0() {}

void cf::CfObjectEff::func_800ACDE0() {}

void cf::CfObjectEff::func_800ACDFC() {}

float func_800ACE44__Q22cf11CfObjectEffFv(void* self) {
    char* p = *(char**)((char*)self + 0x94);
    if (p != 0) {
        return *(float*)(p + 0x2c);
    }
    return *(float*)((char*)self + 0x4c);
}

void cf::CfObjectEff::func_800ACE60() {}

void cf::CfObjectEff::func_800ACE78() {}

void cf::CfObjectEff::func_800ACEAC() {}

void func_800ACEF8(void* obj, const void* src) {
    if (*(unsigned short*)((unsigned int)obj + 0xa4) & 0x4) return;
    void* ptr = *(void**)((unsigned int)obj + 0x94);
    if (ptr != 0) {
        unsigned int* dst = (unsigned int*)((unsigned int)ptr + 0x34);
        const unsigned int* s = (const unsigned int*)src;
        dst[0] = s[0];
        dst[1] = s[1];
        dst[2] = s[2];
    }
    float f = *(const float*)src;
    *(float*)((unsigned int)obj + 0x60) = f;
}

float func_800ACF34__Q22cf11CfObjectEffFv(char* self) {
    char* unk = *(char**)(self + 0x94);
    if (unk != 0) {
        return *(float*)(unk + 0x38);
    }
    return *(float*)(self + 0x60);
}

void cf::CfObjectEff::func_800ACF50() const {}

void func_800ACF78(){}

void func_800ACFD8(){}

void func_800AD040(char* obj, int flag) {
    char* child = *(char**)(obj + 0x94);
    if (child == 0) {
        return;
    }
    *(int*)(child + 0x5c) = (flag != 0);
}

void func_800AD060(){}

void func_800AD378(void* obj) {
    unsigned short count = *(unsigned short*)((char*)obj + 0xA6);
    if (count == 0) return;
    count--;
    *(unsigned short*)((char*)obj + 0xA6) = count;
    if (count != 0) return;
    unsigned int flags = *(unsigned int*)((char*)obj + 0x68);
    flags |= 0x40;
    *(unsigned int*)((char*)obj + 0x68) = flags;
}

void cf::CfObjectEff::func_800AD3A4() {}

void cf::CfObjectEff::func_800AD4A4() {}

void func_800AD4B0(){}

void func_800AD558(){}

void func_800AD5EC(){}

void cf::CfObjectEff::func_800AD68C() {}

void cf::CfObjectEff::~CfObjectEff() {}

void cf::CfObjectEff::func_800AD818() {}

void cf::CfObjectEff::func_800AD830() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[208 / 4];
    func(this);
}

void cf::CfObjectEff::func_800AD840() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0xc4 / 4];
    func(this);
}

void func_800AD850__Q22cf11CfObjectEffFv(void* self) { ((void(*)(void*))func_800AD68C__Q22cf11CfObjectEffFv)((char*)self - 0x90); }

void func_800AD858__Q22cf11CfObjectEffFv(void* self) { ((void(*)(void*))__dt__Q22cf11CfObjectEffFv)((char*)self - 0x90); }

void func_800AD860__FPv(){}
