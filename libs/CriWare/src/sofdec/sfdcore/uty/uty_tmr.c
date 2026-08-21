// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/uty/uty_tmr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char lbl_eu_8051CF20[];
extern s32 lbl_eu_80619BF0;
extern s32 CRICFG_Read(const char* key, s32* out);
extern u32 lbl_eu_80619BF8[2];

struct UtyTmrPair {
    u32 active;           // 0x00
    u32 unit;             // 0x04
};

struct UtyTmrData {
    volatile s32 count;   // 0x00
    s32 id;               // 0x04
    struct UtyTmrPair pair; // 0x08
};

void UTY_InitTmr(s32 newId) {
    s32 val;
    volatile struct UtyTmrData* t = (volatile struct UtyTmrData*)&lbl_eu_80619BF0;

    // Allow a config override of the id.
    if (CRICFG_Read(lbl_eu_8051CF20, &val) == 0) {
        newId = val;
    }

    t->count++;
    // Only (re)arm once for a given id.
    if (t->count > 1 && t->id == newId) {
        return;
    }

    t->id = newId;
    // NOTE: retail materializes the tick-pair address (addi r3, base, 8)
    // before each unit store; MWCC folds every source reconstruction tried
    // to a direct displacement store (see MWCC_CASES.md "UTY_InitTmr p2 base").
    if (newId == -1) {
        // Uninitialised: unit = 1, timer not running.
        t->pair.unit = 1;
        t->pair.active = 0;
    } else {
        // Otherwise unit = (bus clock / 4), timer running.
        t->pair.unit = *(volatile u32*)0x800000F8 >> 2;
        t->pair.active = 0;
    }
}
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
