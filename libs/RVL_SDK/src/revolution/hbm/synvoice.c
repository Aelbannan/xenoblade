// Decompiled RVL_SDK/src/revolution/hbm/synvoice
// High-level C reconstruction - no inline asm, no register tricks
// Typed-struct variant: raw byte-offset arithmetic replaced by struct field access.

#include <harness_catalog.h>
#include <revolution/AX/AXVPB.h>
#include <revolution/AX/AXAlloc.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations of external HBM functions
void HBMMIXReleaseChannel(AXVPB* channel);
s32 HBMGetIndex(s32 key);
void HBMFreeIndex(s32 key);
void HBMFreeIndexByKey(s32 key);

// Forward declarations of other syn functions called from this TU
void __HBMSYNRunVolumeEnvelope(void* voice);
void __HBMSYNUpdateMix(void* voice);
void __HBMSYNUpdateSrc(void* voice);

// Per-voice state struct (must be 0x4C total for voice array indexing)
typedef struct HBMSYNVoice {
    u8  _pad0[0x04];          // 0x00
    AXVPB* mixChannel;        // 0x04 - AXVPB* mix channel (was void*)
    struct HBMSYNSYNTH* synth; // 0x08 - owning synthesizer (was void* voiceDataBase)
    u8   voiceIndex;          // 0x0C - row in synth's voice grid
    u8   voiceColumn;         // 0x0D - col in synth's voice grid
    u8   _pad0E[0x12];        // 0x0E-0x1F
    u32  flags;               // 0x20 - if 0, voice can transition to release
    u8   _pad24[0x0C];        // 0x24-0x2F
    u32  state;               // 0x30 - 3=release, 4=done
    u8   _pad34[0x18];        // 0x34-0x4B (0x4C total)
} HBMSYNVoice;

// Synthesizer instance layout (mirrors syn.c HBMSYNSYNTH; only the
// fields this TU touches are named).
typedef struct HBMSYNSYNTH {
    u8  _pad0[0x404];          // 0x00-0x403
    u32 activeVoiceFlag;       // 0x404 - non-zero when voices are active
    u32 voiceTable[16][128];   // 0x408 - 16 rows x 128 cols, row stride 0x200
} HBMSYNSYNTH;

// Voice pointer — points to the dynamically allocated voice state table
extern HBMSYNVoice* __HBMSYNVoice;

void __HBMSYNClearVoiceReferences(AXVPB* vpb)
{
    HBMSYNVoice* voice;
    s32 voiceIndex;
    HBMSYNSYNTH* synth;
    u32 row;
    u32 col;
    u32* gridCell;

    synth = (HBMSYNSYNTH*)vpb->userContext;
    voiceIndex = HBMGetIndex((s32)vpb->index);
    HBMFreeIndex(voiceIndex);
    voice = &__HBMSYNVoice[voiceIndex];
    HBMMIXReleaseChannel(vpb);

    row = (u32)voice->voiceIndex;
    col = (u32)voice->voiceColumn;
    gridCell = &synth->voiceTable[row][col];
    if (*gridCell == (u32)voice) {
        *gridCell = 0;
    }

    voice->synth = NULL;
    synth->activeVoiceFlag--;
}

void __HBMSYNSetVoiceToRelease(HBMSYNVoice* voice)
{
    voice->state = 3;
}

void __HBMSYNServiceVoice(u32 voiceIndex)
{
    HBMSYNVoice* voice = &__HBMSYNVoice[voiceIndex];
    HBMSYNSYNTH* synth = voice->synth;

    if (synth == NULL) {
        return;
    }

    if (voice->flags == 0 && voice->mixChannel->pb.state == 0) {
        u32 index = voice->voiceIndex;
        u32 col = voice->voiceColumn;
        u32* gridCell = &synth->voiceTable[index][col];
        if (*gridCell == (u32)voice) {
            *gridCell = 0;
        }
        voice->state = 4;
    }

    __HBMSYNRunVolumeEnvelope(voice);

    if (voice->state == 4) {
        voice->synth = NULL;
        HBMMIXReleaseChannel(voice->mixChannel);
        HBMFreeIndexByKey(voice->mixChannel->index);
        AXFreeVoice(voice->mixChannel);
        synth->activeVoiceFlag--;
    } else {
        __HBMSYNUpdateMix(voice);
        __HBMSYNUpdateSrc(voice);
    }
}

#ifdef __cplusplus
}
#endif
