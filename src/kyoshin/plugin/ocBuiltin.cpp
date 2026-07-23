#include "kyoshin/plugin/ocBuiltin.hpp"

extern "C" {
void vmBuiltinOCRegist(OCData* pOC);
extern OCData lbl_eu_80524BF8;
}

// LLM-HARNESS-BEGIN: us-8003a58c
extern "C" int isExistProperty(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a58c

// LLM-HARNESS-BEGIN: us-8003a608
extern "C" int isExistSelector(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a608

// LLM-HARNESS-BEGIN: us-8003a684
extern "C" int getOCName(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a684

// LLM-HARNESS-BEGIN: us-8003a6e4
extern "C" void ocBuiltinRegist() {
    vmBuiltinOCRegist(&lbl_eu_80524BF8);
}
// LLM-HARNESS-END: us-8003a6e4
