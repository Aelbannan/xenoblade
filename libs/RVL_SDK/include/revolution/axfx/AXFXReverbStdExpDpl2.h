#ifndef RVL_SDK_AXFX_REVERB_STD_EXP_DPL2_H
#define RVL_SDK_AXFX_REVERB_STD_EXP_DPL2_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_BUFFERUPDATE_DPL2;

typedef struct AXFX_REVERBSTD_EXP_DPL2 {
    f32* earlyLine[4];       // at 0x0
    u32 earlyPos;            // at 0x10
    u32 earlyLength;         // at 0x14
    u32 earlyMaxLength;      // at 0x18
    f32 earlyCoef;           // at 0x1C

    f32* preDelayLine[4];    // at 0x20
    u32 preDelayPos;         // at 0x30
    u32 preDelayLength;      // at 0x34
    u32 preDelayMaxLength;   // at 0x38

    f32* combLine[4][2];     // at 0x3C
    u32 combPos[2];          // at 0x5C
    u32 combLength[2];       // at 0x64
    u32 combMaxLength[2];    // at 0x6C
    f32 combCoef[2];         // at 0x74

    f32* allpassLine[4][2];  // at 0x7C
    u32 allpassPos[2];       // at 0x9C
    u32 allpassLength[2];    // at 0xA4
    u32 allpassMaxLength[2]; // at 0xAC

    f32 allpassCoef;         // at 0xB4
    f32 lastLpfOut[4];       // at 0xB8
    f32 lpfCoef;             // at 0xC8
    u32 active;              // at 0xCC
    u32 earlyMode;           // at 0xD0
    f32 preDelayTimeMax;     // at 0xD4
    f32 preDelayTime;        // at 0xD8
    u32 fusedMode;           // at 0xDC
    f32 fusedTime;           // at 0xE0
    f32 coloration;          // at 0xE4
    f32 damping;             // at 0xE8
    f32 earlyGain;           // at 0xEC
    f32 fusedGain;           // at 0xF0
    struct AXFX_BUFFERUPDATE_DPL2* busIn;  // at 0xF4
    struct AXFX_BUFFERUPDATE_DPL2* busOut; // at 0xF8
    f32 outGain;             // at 0xFC
    f32 sendGain;            // at 0x100
} AXFX_REVERBSTD_EXP_DPL2;

u32 AXFXReverbStdExpGetMemSizeDpl2(const AXFX_REVERBSTD_EXP_DPL2* fx);
BOOL AXFXReverbStdExpInitDpl2(AXFX_REVERBSTD_EXP_DPL2* fx);
BOOL AXFXReverbStdExpSettingsDpl2(AXFX_REVERBSTD_EXP_DPL2* fx);
BOOL AXFXReverbStdExpSettingsUpdateDpl2(AXFX_REVERBSTD_EXP_DPL2* fx);
void AXFXReverbStdExpShutdownDpl2(AXFX_REVERBSTD_EXP_DPL2* fx);
void AXFXReverbStdExpCallbackDpl2(struct AXFX_BUFFERUPDATE_DPL2* update,
                                  AXFX_REVERBSTD_EXP_DPL2* fx);

#ifdef __cplusplus
}
#endif
#endif
