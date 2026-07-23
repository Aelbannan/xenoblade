#ifndef RVL_SDK_AXFX_REVERB_STD_EXP_H
#define RVL_SDK_AXFX_REVERB_STD_EXP_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct AXFX_BUS;
typedef struct AXFX_BUFFERUPDATE;

typedef struct AXFX_REVERBSTD_EXP {
    f32* earlyLine[3];       // at 0x0
    u32 earlyPos;            // at 0xC
    u32 earlyLength;         // at 0x10
    u32 earlyMaxLength;      // at 0x14
    f32 earlyCoef;           // at 0x18

    f32* preDelayLine[3];    // at 0x1C
    u32 preDelayPos;         // at 0x28
    u32 preDelayLength;      // at 0x2C
    u32 preDelayMaxLength;   // at 0x30

    f32* combLine[3][2];     // at 0x34
    u32 combPos[2];          // at 0x4C
    u32 combLength[2];       // at 0x54
    u32 combMaxLength[2];    // at 0x5C
    f32 combCoef[2];         // at 0x64

    f32* allpassLine[3][2];  // at 0x6C
    u32 allpassPos[2];       // at 0x84
    u32 allpassLength[2];    // at 0x8C
    u32 allpassMaxLength[2]; // at 0x94

    f32 allpassCoef;         // at 0x9C
    f32 lastLpfOut[3];       // at 0xA0
    f32 lpfCoef;             // at 0xAC
    u32 active;              // at 0xB0
    u32 earlyMode;           // at 0xB4
    f32 preDelayTimeMax;     // at 0xB8
    f32 preDelayTime;        // at 0xBC
    u32 fusedMode;           // at 0xC0
    f32 fusedTime;           // at 0xC4
    f32 coloration;          // at 0xC8
    f32 damping;             // at 0xCC
    f32 earlyGain;           // at 0xD0
    f32 fusedGain;           // at 0xD4
    struct AXFX_BUS* busIn;  // at 0xD8
    struct AXFX_BUS* busOut; // at 0xDC
    f32 outGain;             // at 0xE0
    f32 sendGain;            // at 0xE4
} AXFX_REVERBSTD_EXP;

u32 AXFXReverbStdExpGetMemSize(const AXFX_REVERBSTD_EXP* fx);
BOOL AXFXReverbStdExpInit(AXFX_REVERBSTD_EXP* fx);
BOOL AXFXReverbStdExpSettings(AXFX_REVERBSTD_EXP* fx);
BOOL AXFXReverbStdExpSettingsUpdate(AXFX_REVERBSTD_EXP* fx);
void AXFXReverbStdExpShutdown(AXFX_REVERBSTD_EXP* fx);
void AXFXReverbStdExpCallback(AXFX_BUFFERUPDATE* update, AXFX_REVERBSTD_EXP* fx);

#ifdef __cplusplus
}
#endif
#endif
