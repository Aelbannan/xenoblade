// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/uty/uty_tmr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void UTY_InitTmr() {}

extern s32 lbl_eu_80619BF0;

void UTY_FinishTmr(void) {
    if (--lbl_eu_80619BF0 < 0) {
        lbl_eu_80619BF0 = 0;
    }
}

extern s32 lbl_eu_80619BF4;

extern u32 lbl_eu_80619BF8[2];
u64 UTY_GetTmr(void) {
    if (lbl_eu_80619BF0 <= 0 || lbl_eu_80619BF4 == -1) return 0;
    return __mftb();
}

s32 UTY_IsTmrVoid(s32 idx) {
    u64 val = *(u64*)lbl_eu_80619BF8;
    return val <= 1 ? 1 : 0;
}

u64 UTY_GetTmrUnit(void) {
    return *(u64*)lbl_eu_80619BF8;
}
