// CRI sofdec software timer utilities (uty_tmr).
//
// Global timer state lives in one .bss block:
//   the init counter (number of UTY_InitTmr calls),
//   the id the timer was last armed with (-1 = none), and
//   the { active flag, tick unit } pair.
// Once armed, UTY_GetTmr returns the raw PPC time base value.

#include <harness_catalog.h>
#include <revolution/os/OSHardware.h>

// name-guess: no retail name survives at 0x8051CF20; the string is the only
// config key read by this TU.
__attribute__((aligned(8))) const char sTmrConfigKey[8] = "TMR_CH";
extern s32 CRICFG_Read(const char* key, s32* out);

// name-guess: no retail name survives at 0x80619BF0..0x80619BFC; these three
// code-visible words are the only state used by the UTY timer entry points.
s32 UTY_InitCount;
s32 UTY_ArmedId;
u64 UTY_TimerPair;

void UTY_InitTmr(s32 newId) {
    s32 val;

    // Allow a config override of the armed id.
    if (CRICFG_Read(sTmrConfigKey, &val) == 0) {
        newId = val;
    }

    UTY_InitCount++;
    // Only (re)arm once for a given id.
    if (UTY_InitCount > 1 && UTY_ArmedId == newId) {
        return;
    }

    UTY_ArmedId = newId;
    if (newId == -1) {
        // Uninitialised id: tick unit = 1, timer not running.
        UTY_TimerPair = 1;
    } else {
        // Otherwise tick unit = bus clock / 4, timer running.
        UTY_TimerPair = OS_BUS_CLOCK_SPEED >> 2;
    }
}

void UTY_FinishTmr(void) {
    if (--UTY_InitCount < 0) {
        UTY_InitCount = 0;
    }
}

u64 __mftb(void);

u64 UTY_GetTmr(void) {
    // Raw time base while the timer is armed; 0 otherwise.
    if (UTY_InitCount <= 0 || UTY_ArmedId == -1) {
        return 0;
    }
    return __mftb();
}

s32 UTY_IsTmrVoid(s32 idx) {
    // Timer idle when the {active, unit} pair as a u64 is 0 or 1.
    u64 val = UTY_TimerPair;
    return val <= 1 ? 1 : 0;
}

u64 UTY_GetTmrUnit(void) {
    return UTY_TimerPair;
}
