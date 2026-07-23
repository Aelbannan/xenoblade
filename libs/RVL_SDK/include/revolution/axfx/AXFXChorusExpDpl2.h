#ifndef RVL_SDK_AXFX_CHORUS_EXP_DPL2_H
#define RVL_SDK_AXFX_CHORUS_EXP_DPL2_H
#include <types.h>
#include <revolution/AXFX/AXFXChorusExp.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct AXFX_BUFFERUPDATE_DPL2;

typedef struct AXFX_CHORUS_EXP_DELAY_DPL2 {
    f32* line[4]; // at 0x0
    u32 inPos;    // at 0x10
    u32 outPos;   // at 0x14
    u32 lastPos;  // at 0x18
    u32 sizeFP;   // at 0x1C
    u32 size;     // at 0x20
} AXFX_CHORUS_EXP_DELAY_DPL2;

typedef struct AXFX_CHORUS_EXP_DPL2 {
    AXFX_CHORUS_EXP_DELAY_DPL2 delay; // at 0x0
    AXFX_CHORUS_EXP_LFO lfo;          // at 0x24
    f32 history[4][4];                // at 0x4C
    u32 histIndex;                    // at 0x8C
    u32 active;                       // at 0x90
    f32 delayTime;                    // at 0x94
    f32 depth;                        // at 0x98
    f32 rate;                         // at 0x9C
    f32 feedback;                     // at 0xA0
    struct AXFX_BUFFERUPDATE_DPL2* busIn;  // at 0xA4
    struct AXFX_BUFFERUPDATE_DPL2* busOut; // at 0xA8
    f32 outGain;                      // at 0xAC
    f32 sendGain;                     // at 0xB0
} AXFX_CHORUS_EXP_DPL2;

u32 AXFXChorusExpGetMemSizeDpl2(const AXFX_CHORUS_EXP_DPL2* fx);
BOOL AXFXChorusExpInitDpl2(AXFX_CHORUS_EXP_DPL2* fx);
void AXFXChorusExpShutdownDpl2(AXFX_CHORUS_EXP_DPL2* fx);
BOOL AXFXChorusExpSettingsUpdateDpl2(AXFX_CHORUS_EXP_DPL2* fx);
void AXFXChorusExpCallbackDpl2(struct AXFX_BUFFERUPDATE_DPL2* update, AXFX_CHORUS_EXP_DPL2* fx);

#ifdef __cplusplus
}
#endif
#endif
