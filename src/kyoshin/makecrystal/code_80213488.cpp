// Auto-scaffolded catalog TU for kyoshin/makecrystal/code_80213488
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-802152e0
extern "C" void func_80213488() {}
// LLM-HARNESS-END: us-802152e0

// LLM-HARNESS-BEGIN: us-80215374
extern "C" void func_8021351C(void* dst, const void* src) {
    *(short*)dst = *(short*)src;
    ((unsigned char*)dst)[2] = ((unsigned char*)src)[2];
}
// LLM-HARNESS-END: us-80215374

// LLM-HARNESS-BEGIN: us-80215388
extern "C" void __dt__80213530() {}
// LLM-HARNESS-END: us-80215388

// LLM-HARNESS-BEGIN: us-802153c8
extern "C" void func_80213570() {}
// LLM-HARNESS-END: us-802153c8

// LLM-HARNESS-BEGIN: us-80215538
extern "C" void func_802136E0(char* self, int idx, unsigned char val)
{
    int adj = idx + ((signed char)(self[0x1004])) * 30;
    adj &= 0xFFFF;
    if (adj >= *(unsigned short*)(self + 0x1000)) return;
    int offset = adj * 4;
    self[offset + 2] = val;
}
// LLM-HARNESS-END: us-80215538

// LLM-HARNESS-BEGIN: us-80215568
extern "C" void func_80213710() {}
// LLM-HARNESS-END: us-80215568

// LLM-HARNESS-BEGIN: us-802155a0
extern "C" int func_80213748(void* this_ptr) {
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
// LLM-HARNESS-END: us-802155a0

// LLM-HARNESS-BEGIN: us-802155e0
extern "C" void func_80213788() {}
// LLM-HARNESS-END: us-802155e0

// LLM-HARNESS-BEGIN: us-8021560c
extern "C" void func_802137B4(void* self) {
    unsigned char* base = (unsigned char*)self;
    signed char val = (signed char)(base[0x1004] - 1);
    base[0x1004] = (unsigned char)val;
    if (val >= 0) return;
    base[0x1004] = base[0x1003] - 1;
}
// LLM-HARNESS-END: us-8021560c

// LLM-HARNESS-BEGIN: us-80215634
extern "C" void func_802137DC() {}
// LLM-HARNESS-END: us-80215634

// LLM-HARNESS-BEGIN: us-802156a4
extern "C" void func_8021384C() {}
// LLM-HARNESS-END: us-802156a4
