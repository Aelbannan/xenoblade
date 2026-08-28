// CRI sofdec software timer utilities (uty_tmr).
//
// Global timer state lives in one .bss block:
//   lbl_eu_80619BF0 (+0x00): init counter (number of UTY_InitTmr calls)
//   lbl_eu_80619BF4 (+0x04): id the timer was last armed with (-1 = none)
//   lbl_eu_80619BF8 (+0x08): { active flag, tick unit } pair
// Once armed, UTY_GetTmr returns the raw PPC time base value.

#include <harness_catalog.h>

// Config key string ("..." rodata) and the timer state block anchors.
__declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_8051CF20[8] = "TMR_CH";
extern s32 CRICFG_Read(const char* key, s32* out);
s32 lbl_eu_80619BF0; /* .bss 0x80619BF0 size 0x4 */
s32 lbl_eu_80619BF4; /* .bss 0x80619BF4 size 0x4 */
u32 lbl_eu_80619BF8[2]; /* .bss 0x80619BF8 size 0x8 */

struct UtyTmrPair {
    u32 active;  // 0x08
    u32 unit;    // 0x0C
};

struct UtyTmrData {
    volatile s32 count;     // 0x00: init counter
    s32 id;                 // 0x04: armed id
    struct UtyTmrPair pair; // 0x08: run flag + tick unit
};

void UTY_InitTmr(s32 newId) {
    s32 val;
    volatile struct UtyTmrData* t = (volatile struct UtyTmrData*)&lbl_eu_80619BF0;

    // Allow a config override of the armed id.
    if (CRICFG_Read(lbl_eu_8051CF20, &val) == 0) {
        newId = val;
    }

    t->count++;
    // Only (re)arm once for a given id.
    if (t->count > 1 && t->id == newId) {
        return;
    }

    t->id = newId;
    if (newId == -1) {
        // Uninitialised id: tick unit = 1, timer not running.
        t->pair.unit = 1;
        t->pair.active = 0;
    } else {
        // Otherwise tick unit = bus clock / 4, timer running.
        t->pair.unit = *(volatile u32*)0x800000F8 >> 2;
        t->pair.active = 0;
    }
}

void UTY_FinishTmr(void) {
    if (--lbl_eu_80619BF0 < 0) {
        lbl_eu_80619BF0 = 0;
    }
}

u64 __mftb(void);

u64 UTY_GetTmr(void) {
    // Raw time base while the timer is armed; 0 otherwise.
    if (lbl_eu_80619BF0 <= 0 || lbl_eu_80619BF4 == -1) {
        return 0;
    }
    return __mftb();
}

s32 UTY_IsTmrVoid(s32 idx) {
    // Timer idle when the {active, unit} pair as a u64 is 0 or 1.
    u64 val = *(u64*)lbl_eu_80619BF8;
    return val <= 1 ? 1 : 0;
}

u64 UTY_GetTmrUnit(void) {
    return *(u64*)lbl_eu_80619BF8;
}
