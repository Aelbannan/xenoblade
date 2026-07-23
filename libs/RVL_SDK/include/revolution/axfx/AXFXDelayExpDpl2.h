#ifndef RVL_SDK_AXFX_DELAY_EXP_DPL2_H
#define RVL_SDK_AXFX_DELAY_EXP_DPL2_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct AXFX_BUFFERUPDATE_DPL2;

typedef struct AXFX_DELAY_EXP_DPL2 {
    s32* line[4];                              // at 0x0
    u32 curPos;                                // at 0x10
    u32 length;                                // at 0x14
    u32 maxLength;                             // at 0x18
    s32 feedbackGain;                          // at 0x1C
    s32 last[4];                               // at 0x20
    s32 iirGain;                               // at 0x30
    s32 outGainI;                              // at 0x34
    s32 sendGainI;                             // at 0x38
    u32 active;                                // at 0x3C
    f32 delayTimeMax;                          // at 0x40
    f32 delayTime;                             // at 0x44
    f32 feedback;                              // at 0x48
    f32 iir;                                   // at 0x4C
    struct AXFX_BUFFERUPDATE_DPL2* busIn;  // at 0x50
    struct AXFX_BUFFERUPDATE_DPL2* busOut; // at 0x54
    f32 outGain;                               // at 0x58
    f32 sendGain;                              // at 0x5C
} AXFX_DELAY_EXP_DPL2;

u32 AXFXDelayExpGetMemSizeDpl2(const AXFX_DELAY_EXP_DPL2* fx);
BOOL AXFXDelayExpInitDpl2(AXFX_DELAY_EXP_DPL2* fx);
BOOL AXFXDelayExpSettingsDpl2(AXFX_DELAY_EXP_DPL2* fx);
BOOL AXFXDelayExpSettingsUpdateDpl2(AXFX_DELAY_EXP_DPL2* fx);
void AXFXDelayExpShutdownDpl2(AXFX_DELAY_EXP_DPL2* fx);
void AXFXDelayExpCallbackDpl2(struct AXFX_BUFFERUPDATE_DPL2* update, AXFX_DELAY_EXP_DPL2* fx);

#ifdef __cplusplus
}
#endif
#endif
