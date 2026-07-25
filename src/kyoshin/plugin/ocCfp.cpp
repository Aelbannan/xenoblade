// Auto-scaffolded catalog TU for kyoshin/plugin/ocCfp
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocCfp.hpp"

// LLM-HARNESS-BEGIN: us-80045b00
extern "C" void func_80045560() {}
// LLM-HARNESS-END: us-80045b00

// LLM-HARNESS-BEGIN: us-80045b3c
extern "C" void func_8004559C() {}
// LLM-HARNESS-END: us-80045b3c

// LLM-HARNESS-BEGIN: us-80045b88
extern "C" void func_800455E8() {}
// LLM-HARNESS-END: us-80045b88

// LLM-HARNESS-BEGIN: us-80045bd4
extern "C" void func_80045634() {}
// LLM-HARNESS-END: us-80045bd4

// LLM-HARNESS-BEGIN: us-80045c34
extern "C" int func_80045694(void* self) {
    extern void* vmOCPropertyGet(void*);
    extern void func_80086D90__Q22cf13CfGameManagerFv(unsigned short);
    void* prop = vmOCPropertyGet(self);
    func_80086D90__Q22cf13CfGameManagerFv(*(unsigned short*)((char*)prop + 4));
    return 0;
}
// LLM-HARNESS-END: us-80045c34

// LLM-HARNESS-BEGIN: us-80045c64
extern "C" int func_800456C4(void* self) {
    extern void* vmOCPropertyGet(void*);
    extern void func_80086D94__Q22cf13CfGameManagerFv(unsigned short);
    void* prop = vmOCPropertyGet(self);
    func_80086D94__Q22cf13CfGameManagerFv(*(unsigned short*)((char*)prop + 4));
    return 0;
}
// LLM-HARNESS-END: us-80045c64

// LLM-HARNESS-BEGIN: us-80045c94
extern "C" void getTimeIdxMin() {}
// LLM-HARNESS-END: us-80045c94

// LLM-HARNESS-BEGIN: us-80045d1c
extern "C" void getTimeIdxMax() {}
// LLM-HARNESS-END: us-80045d1c

// LLM-HARNESS-BEGIN: us-80045da8
extern "C" {
BOOL vmOCRegist(OCData* pOC);
}
extern OCData lbl_eu_80525D58[];
void ocCfpRegist() {
    vmOCRegist(lbl_eu_80525D58);
}
// LLM-HARNESS-END: us-80045da8
