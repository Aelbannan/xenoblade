// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__Q22cf11CfObjectEffFv() {}

extern "C" void CfObject_UnkVirtualFunc3__Q22cf8CfObjectFv() {}

extern "C" void __dt__Q22cf8CfObjectFv() {}

extern "C" void CfObject_UnkVirtualFunc6__Q22cf8CfObjectFv() {}

extern "C" void func_800AC7CC__Q22cf11CfObjectEffFv() {}

extern "C" void func_800AC7FC__Q22cf11CfObjectEffFv() {}

extern "C" void func_800AC810__Q22cf11CfObjectEffFv() {}

extern "C" void func_800AC86C__Q22cf11CfObjectEffFv() {}

extern "C" void CfObjectModel_UnkVirtualFunc18__Q22cf13CfObjectModelFv() {}

extern "C" void func_800AC990__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACA58__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACAE8__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACB08__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACBA4__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACBCC__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACC14(void* self, unsigned char val) {
    void* ptr = *(void**)((char*)self + 0x94);
    if (ptr != nullptr)
        *(unsigned char*)((char*)ptr + 0x59) = val;
}

extern "C" void func_800ACC28() {}

extern "C" void func_800ACC3C__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACC50() {}

extern "C" void func_800ACC64(void* obj, const void* src) {
    void* dest = *(void**)((unsigned char*)obj + 0x94);
    if (dest == 0) return;
    unsigned int* d = (unsigned int*)((unsigned char*)dest + 0x40);
    const unsigned int* s = (const unsigned int*)src;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
}

extern "C" void func_800ACC94__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACCD4__Q22cf11CfObjectEffFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[156 / 4];
    func(self);
}

extern "C" void func_800ACCE4__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACD4C__Q22cf11CfObjectEffFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[180 / 4];
    func(self);
}

extern "C" void func_800ACD5C__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACDA0__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACDE0__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACDFC__Q22cf11CfObjectEffFv() {}

extern "C" float func_800ACE44__Q22cf11CfObjectEffFv(void* self) {
    char* p = *(char**)((char*)self + 0x94);
    if (p != 0) {
        return *(float*)(p + 0x2c);
    }
    return *(float*)((char*)self + 0x4c);
}

extern "C" void func_800ACE60__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACE78__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACEAC__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACEF8(void* obj, const void* src) {
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

extern "C" float func_800ACF34__Q22cf11CfObjectEffFv(char* self) {
    char* unk = *(char**)(self + 0x94);
    if (unk != 0) {
        return *(float*)(unk + 0x38);
    }
    return *(float*)(self + 0x60);
}

extern "C" void func_800ACF50__Q22cf11CfObjectEffFv() {}

extern "C" void func_800ACF78() {}

extern "C" void func_800ACFD8() {}

extern "C" void func_800AD040(char* obj, int flag) {
    char* child = *(char**)(obj + 0x94);
    if (child == 0) {
        return;
    }
    *(int*)(child + 0x5c) = (flag != 0);
}

extern "C" void func_800AD060() {}

extern "C" void func_800AD378(void* obj) {
    unsigned short count = *(unsigned short*)((char*)obj + 0xA6);
    if (count == 0) return;
    count--;
    *(unsigned short*)((char*)obj + 0xA6) = count;
    if (count != 0) return;
    unsigned int flags = *(unsigned int*)((char*)obj + 0x68);
    flags |= 0x40;
    *(unsigned int*)((char*)obj + 0x68) = flags;
}

extern "C" void func_800AD3A4__Q22cf11CfObjectEffFv() {}

extern "C" void func_800AD4A4__Q22cf11CfObjectEffFv() {}

extern "C" void func_800AD4B0() {}

extern "C" void func_800AD558() {}

extern "C" void func_800AD5EC() {}

extern "C" void func_800AD68C__Q22cf11CfObjectEffFv(void* self) {}

extern "C" void __dt__Q22cf11CfObjectEffFv(void* self) {}

extern "C" void func_800AD818__Q22cf11CfObjectEffFv() {}

extern "C" void func_800AD830__Q22cf11CfObjectEffFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[208 / 4];
    func(self);
}

extern "C" void func_800AD840__Q22cf11CfObjectEffFv(void* self) {
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0xc4 / 4];
    func(self);
}

extern "C" void func_800AD850__Q22cf11CfObjectEffFv(void* self) { ((void(*)(void*))func_800AD68C__Q22cf11CfObjectEffFv)((char*)self - 0x90); }

extern "C" void func_800AD858__Q22cf11CfObjectEffFv(void* self) { ((void(*)(void*))__dt__Q22cf11CfObjectEffFv)((char*)self - 0x90); }

extern "C" void func_800AD860__FPv() {}
