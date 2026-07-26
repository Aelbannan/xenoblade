// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/uty/uty_tmr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803d6dc0
void UTY_InitTmr() {}
// LLM-HARNESS-END: us-803d6dc0

// LLM-HARNESS-BEGIN: us-803d6e78
extern s32 lbl_eu_80619BF0;
void UTY_FinishTmr(void) {
    --lbl_eu_80619BF0;
}
// LLM-HARNESS-END: us-803d6e78

// LLM-HARNESS-BEGIN: us-803d6ea4
extern s32 lbl_eu_80619BF0;
extern s64 lbl_eu_80619BF4;
u64 UTY_GetTmr(void) {
    if (lbl_eu_80619BF0 <= 0 || lbl_eu_80619BF4 == -1) return 0;
    return __mftb();
}
// LLM-HARNESS-END: us-803d6ea4

// LLM-HARNESS-BEGIN: us-803d6ee8
extern u32 lbl_eu_80619BF8[2];
s32 UTY_IsTmrVoid(s32 idx) {
    u32* t = &lbl_eu_80619BF8[0];
    s32 result;
    if (t[1] >= 1 && t[0] >= 0) result = 0;
    else result = 1;
    return 1 - result;
}
// LLM-HARNESS-END: us-803d6ee8

// LLM-HARNESS-BEGIN: us-803d6f18
extern u32 lbl_eu_80619BF8[];
u64 UTY_GetTmrUnit(void) {
    return *(u64*)lbl_eu_80619BF8;
}
// LLM-HARNESS-END: us-803d6f18
