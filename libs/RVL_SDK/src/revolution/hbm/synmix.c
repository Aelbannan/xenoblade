// Decompiled RVL_SDK/src/revolution/hbm/synmix
// High-level C reconstruction — no inline asm, no register tricks

#include <harness_catalog.h>

// Forward declarations of external HBMMIX functions (called below)
// Retail uses C-linkage names for these symbols.
#ifdef __cplusplus
extern "C" {
#endif
void HBMMIXSetInput(void* channel, s32 value);
void HBMMIXSetAuxA(void* channel, s32 value);
void HBMMIXSetFader(void* channel, s32 value);
void HBMMIXSetPan(void* channel, s32 value);
#ifdef __cplusplus
}
#endif

// External data — symbol from the .data section
extern s32 __HBMSYNVolumeAttenuation[];

// Per-voice mixer state (layout inferred from retail ASM offsets)
typedef struct HBMSYNVoice {
    u8  _pad0[0x04];          // 0x00
    void* mixChannel;          // 0x04 — HBMMIXChannel pointer
    void* voiceDataBase;       // 0x08 — base of per-voice data array
    u8   voiceIndex;           // 0x0C
    u8   _padD;                // 0x0D
    u8   volumeIndex;          // 0x0E — index into __HBMSYNVolumeAttenuation
    u8   pan;                  // 0x0F
    void* volumeInfo;          // 0x10 — volume info struct (value at +0x04)
    u8   _pad14[0x18];         // 0x14-0x2B
    s32  accumInput;           // 0x2C
    u8   _pad30[0x04];         // 0x30
    s32  accumInput2;          // 0x34
} HBMSYNVoice;

// Read a u32 value from the per-voice table at a given base offset.
// Entries are 4 bytes each, indexed by voiceIndex.
static s32 voiceTableS32(void* base, u32 baseOffset, u8 index) {
    return ((s32*)((u8*)base + baseOffset))[index];
}

// Read a u8 value from the per-voice byte table at a given base offset.
static u8 voiceTableU8(void* base, u32 baseOffset, u8 index) {
    return ((u8*)base + baseOffset)[index];
}

// All exported functions must use C linkage to match retail symbol names.
#ifdef __cplusplus
extern "C" {
#endif

void __HBMSYNSetupVolume(HBMSYNVoice* voice)
{
    voice->accumInput = ((s32*)voice->volumeInfo)[1]
                      + __HBMSYNVolumeAttenuation[voice->volumeIndex];
}

void __HBMSYNSetupPan(HBMSYNVoice* voice)
{
    voice->pan = voiceTableU8(voice->voiceDataBase, 0xEC, voice->voiceIndex);
}

s32 __HBMSYNGetVoiceInput(HBMSYNVoice* voice)
{
    return (voice->accumInput + voice->accumInput2) >> 16;
}

s32 __HBMSYNGetVoiceFader(HBMSYNVoice* voice)
{
    s32 baseFader = *(s32*)((u8*)voice->voiceDataBase + 0x68);
    s32 voiceFader = voiceTableS32(voice->voiceDataBase, 0x6C, voice->voiceIndex);
    return (baseFader + voiceFader) >> 16;
}

void __HBMSYNUpdateMix(HBMSYNVoice* voice)
{
    // Combined input = (accumInput + accumInput2) >> 16
    s32 input = (voice->accumInput + voice->accumInput2) >> 16;
    HBMMIXSetInput(voice->mixChannel, input);

    // AuxA = voiceDataBase[voiceIndex*4 + 0xAC] >> 16
    s32 auxA = voiceTableS32(voice->voiceDataBase, 0xAC, voice->voiceIndex) >> 16;
    HBMMIXSetAuxA(voice->mixChannel, auxA);

    // Fader = (voiceDataBase[0x68] + voiceDataBase[voiceIndex*4 + 0x6C]) >> 16
    s32 baseFader = *(s32*)((u8*)voice->voiceDataBase + 0x68);
    s32 voiceFader = voiceTableS32(voice->voiceDataBase, 0x6C, voice->voiceIndex);
    s32 fader = (baseFader + voiceFader) >> 16;
    HBMMIXSetFader(voice->mixChannel, fader);

    // Pan = voiceDataBase[voiceIndex + 0xEC] (byte table, no shift)
    u8 panVal = voiceTableU8(voice->voiceDataBase, 0xEC, voice->voiceIndex);
    HBMMIXSetPan(voice->mixChannel, panVal);
}

#ifdef __cplusplus
}
#endif
