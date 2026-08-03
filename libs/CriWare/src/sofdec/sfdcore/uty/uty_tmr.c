// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/uty/uty_tmr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char lbl_eu_8051CF20[];
extern s32 lbl_eu_80619BF0;
extern s32 CRICFG_Read(const char* key, s32* out);

void UTY_InitTmr(void* self) {
    s32 val;
    volatile u32* tmr = (volatile u32*)&lbl_eu_80619BF0;
    volatile u32* p2 = tmr + 2;
    if (CRICFG_Read(lbl_eu_8051CF20, &val) == 0) {
        self = (void*)val;
    }
    tmr[0]++;
    if (!((s32)tmr[0] > 1 && (s32)tmr[1] == (s32)self)) {
        tmr[1] = (u32)self;
        if ((s32)self == -1) {
            p2[1] = 1;
            p2[0] = 0;
        } else {
            p2[1] = (*(volatile u32*)0x800000F8) >> 2;
            p2[0] = 0;
        }
    }
}

extern s32 lbl_eu_80619BF0;

void UTY_FinishTmr(void) {
    if (--lbl_eu_80619BF0 < 0) {
        lbl_eu_80619BF0 = 0;
    }
}

extern s32 lbl_eu_80619BF4;

extern u32 lbl_eu_80619BF8[2];

u64 __mftb(void);

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
