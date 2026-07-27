// Auto-scaffolded catalog TU for kyoshin/makecrystal/code_80213488
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void func_80213488(){}

void func_8021351C(void* dst, const void* src) {
    *(short*)dst = *(short*)src;
    ((unsigned char*)dst)[2] = ((unsigned char*)src)[2];
}

void __dt__80213530(){}

void func_80213570(){}

void func_802136E0(char* self, int idx, unsigned char val)
{
    int adj = idx + ((signed char)(self[0x1004])) * 30;
    adj &= 0xFFFF;
    if (adj >= *(unsigned short*)(self + 0x1000)) return;
    int offset = adj * 4;
    self[offset + 2] = val;
}

void func_80213710(){}

int func_80213748(void* this_ptr) {
    struct Layout {
        struct Element {
            char pad[2];
            unsigned char flag;
            char pad2;
        };
        Element array[1024];
        unsigned short count;
    };
    Layout* self = (Layout*)this_ptr;
    unsigned short count = self->count;
    for (unsigned short i = 0; i < count; ++i) {
        if (self->array[i].flag == 0) {
            return 0;
        }
    }
    return 1;
}

void func_80213788(){}

void func_802137B4(void* self) {
    unsigned char* base = (unsigned char*)self;
    signed char val = (signed char)(base[0x1004] - 1);
    base[0x1004] = (unsigned char)val;
    if (val >= 0) return;
    base[0x1004] = base[0x1003] - 1;
}

void func_802137DC(){}

void func_8021384C(){}
