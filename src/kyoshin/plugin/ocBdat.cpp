// Auto-scaffolded catalog TU for kyoshin/plugin/ocBdat
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "monolib/vm/yvm2.h"

extern "C" {
extern s8 lbl_eu_80663D10;
extern u32 lbl_eu_80663D14;
extern void* lbl_eu_805705D0[8];
}

extern "C" void* func_eu_8003B720(void* p);

// LLM-HARNESS-BEGIN: us-8003ae14
extern "C" void* func_8003AA34() {
    if (!lbl_eu_80663D10) {
        lbl_eu_80663D10 = 1;
    }
    return &lbl_eu_80663D14;
}
// LLM-HARNESS-END: us-8003ae14

// LLM-HARNESS-BEGIN: us-8003ae30
void* CBdat::func_8003AA50() {
    void** table = lbl_eu_805705D0;
    table[0] = 0;
    table[1] = 0;
    table[2] = 0;
    table[3] = 0;
    table[4] = 0;
    table[5] = 0;
    table[6] = 0;
    return table;
}
// LLM-HARNESS-END: us-8003ae30

// LLM-HARNESS-BEGIN: us-8003ae5c
void* CBdat::func_8003AA78(u32 idx, void* p) {
    lbl_eu_805705D0[idx] = p;
    return func_eu_8003B720(p);
}
// LLM-HARNESS-END: us-8003ae5c

// LLM-HARNESS-BEGIN: us-8003ae74
void CBdat::func_8003AA8C(u32 idx) {
    lbl_eu_805705D0[idx] = 0;
}
// LLM-HARNESS-END: us-8003ae74

// LLM-HARNESS-BEGIN: us-8003ae8c
extern "C" void getFP__FPCc() {}
// LLM-HARNESS-END: us-8003ae8c

// LLM-HARNESS-BEGIN: us-8003af48
extern "C" void func_8003B4B0() {}
// LLM-HARNESS-END: us-8003af48

// LLM-HARNESS-BEGIN: us-8003b148
extern "C" void getBdatStringColumnValue() {}
// LLM-HARNESS-END: us-8003b148

// LLM-HARNESS-BEGIN: us-8003b220
extern "C" void func_8003AD98() {}
// LLM-HARNESS-END: us-8003b220

// LLM-HARNESS-BEGIN: us-8003b2e4
extern "C" void func_8003AFC0() {}
// LLM-HARNESS-END: us-8003b2e4

// LLM-HARNESS-BEGIN: us-8003b360
extern "C" u16 func_8003B1EC(void* p) {
    if (p == 0) {
        return 0;
    }
    return *reinterpret_cast<u16*>(reinterpret_cast<char*>(p) + 0x10);
}
// LLM-HARNESS-END: us-8003b360

// LLM-HARNESS-BEGIN: us-8003b378
extern "C" void func_8003B434() {}
// LLM-HARNESS-END: us-8003b378

// LLM-HARNESS-BEGIN: us-8003b408
extern "C" void func_8003B204() {}
// LLM-HARNESS-END: us-8003b408

// LLM-HARNESS-BEGIN: us-8003b470
extern "C" u16 func_8003B41C(void* p) {
    if (p == 0) {
        return 0;
    }
    return *reinterpret_cast<u16*>(reinterpret_cast<char*>(p) + 0x12);
}
// LLM-HARNESS-END: us-8003b470

// LLM-HARNESS-BEGIN: us-8003b488
extern "C" void func_eu_8003B488() {}
// LLM-HARNESS-END: us-8003b488

// LLM-HARNESS-BEGIN: us-8003b5d8
extern "C" void func_8003B6A0() {}
// LLM-HARNESS-END: us-8003b5d8

// LLM-HARNESS-BEGIN: us-8003b670
extern "C" void func_8003B748() {}
// LLM-HARNESS-END: us-8003b670

// LLM-HARNESS-BEGIN: us-8003b720
#pragma dont_inline on
extern "C" void* func_eu_8003B720(void* p) {
    return p;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b720

// LLM-HARNESS-BEGIN: us-8003bc60
extern "C" void func_8003B800() {}
// LLM-HARNESS-END: us-8003bc60

// LLM-HARNESS-BEGIN: us-8003bd4c
extern "C" void bdat() {}
// LLM-HARNESS-END: us-8003bd4c

// LLM-HARNESS-BEGIN: us-8003bdb8
extern "C" void getVal() {}
// LLM-HARNESS-END: us-8003bdb8

// LLM-HARNESS-BEGIN: us-8003be70
extern "C" void getArrayVal() {}
// LLM-HARNESS-END: us-8003be70

// LLM-HARNESS-BEGIN: us-8003bf48
extern "C" void getArrayCount() {}
// LLM-HARNESS-END: us-8003bf48

// LLM-HARNESS-BEGIN: us-8003bfb8
extern "C" void getVarType() {}
// LLM-HARNESS-END: us-8003bfb8

// LLM-HARNESS-BEGIN: us-8003c028
extern "C" void getIdCount() {}
// LLM-HARNESS-END: us-8003c028

// LLM-HARNESS-BEGIN: us-8003c074
extern "C" void getIdTop() {}
// LLM-HARNESS-END: us-8003c074

// LLM-HARNESS-BEGIN: us-8003c0c0
extern "C" void getFlagVal() {}
// LLM-HARNESS-END: us-8003c0c0

// LLM-HARNESS-BEGIN: us-8003c180
extern "C" void ocBdatRegist() {}
// LLM-HARNESS-END: us-8003c180
