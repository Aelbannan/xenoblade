#ifndef RVL_SDK_AXFX_DELAY_EXP_H
#define RVL_SDK_AXFX_DELAY_EXP_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct AXFX_BUS;
typedef struct AXFX_BUFFERUPDATE;

typedef struct AXFX_DELAY_EXP {
    s32* line[3];            // at 0x0
    u32 curPos;              // at 0xC
    u32 length;              // at 0x10
    u32 maxLength;           // at 0x14
    s32 feedbackGain;        // at 0x18
    s32 last[3];             // at 0x1C
    s32 iirGain;             // at 0x28
    s32 outGainI;            // at 0x2C
    s32 sendGainI;           // at 0x30
    u32 active;              // at 0x34
    f32 delayTimeMax;        // at 0x38
    f32 delayTime;           // at 0x3C
    f32 feedback;            // at 0x40
    f32 iir;                 // at 0x44
    struct AXFX_BUS* busIn;  // at 0x48
    struct AXFX_BUS* busOut; // at 0x4C
    f32 outGain;             // at 0x50
    f32 sendGain;            // at 0x54
} AXFX_DELAY_EXP;

u32 AXFXDelayExpGetMemSize(const AXFX_DELAY_EXP* fx);
BOOL AXFXDelayExpInit(AXFX_DELAY_EXP* fx);
BOOL AXFXDelayExpSettings(AXFX_DELAY_EXP* fx);
BOOL AXFXDelayExpSettingsUpdate(AXFX_DELAY_EXP* fx);
void AXFXDelayExpShutdown(AXFX_DELAY_EXP* fx);
void AXFXDelayExpCallback(struct AXFX_BUFFERUPDATE* update, AXFX_DELAY_EXP* fx);

#ifdef __cplusplus
}
#endif
#endif
