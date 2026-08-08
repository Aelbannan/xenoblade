// Decompiled RVL_SDK/src/revolution/hbm/synmix
// High-level C reconstruction - no inline asm, no register tricks

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

// Retail .data at 0x8054C598 (size 0x200) - volume attenuation table.
s32 __HBMSYNVolumeAttenuation[128] = {
    0xFC400000, 0xFCB67A80, 0xFD2EE3F9, 0xFD7553B8, 0xFDA74D72, 0xFDCE1108, 0xFDEDBD30, 0xFE08848A,
    0xFE1FB6EA, 0xFE342CEF, 0xFE467A80, 0xFE57091D, 0xFE6626A9, 0xFE740E4D, 0xFE80EE03, 0xFE8CEA3F,
    0xFE982063, 0xFEA2A878, 0xFEAC9668, 0xFEB5FADF, 0xFEBEE3F9, 0xFEC75DC2, 0xFECF7295, 0xFED72B6E,
    0xFEDE9022, 0xFEE5A78F, 0xFEEC77C6, 0xFEF30626, 0xFEF9577C, 0xFEFF700E, 0xFF0553B8, 0xFF0B05F0,
    0xFF1089DC, 0xFF15E254, 0xFF1B11F1, 0xFF201B12, 0xFF24FFE1, 0xFF29C25C, 0xFF2E6457, 0xFF32E784,
    0xFF374D72, 0xFF3B9791, 0xFF3FC73A, 0xFF43DDAC, 0xFF47DC0E, 0xFF4BC376, 0xFF4F94E7, 0xFF535152,
    0xFF56F99B, 0xFF5A8E94, 0xFF5E1108, 0xFF6181B0, 0xFF64E13E, 0xFF68305A, 0xFF6B6F9F, 0xFF6E9FA4,
    0xFF71C0F4, 0xFF74D416, 0xFF77D987, 0xFF7AD1BF, 0xFF7DBD30, 0xFF809C47, 0xFF836F69, 0xFF8636F9,
    0xFF88F355, 0xFF8BA4D4, 0xFF8E4BCD, 0xFF90E890, 0xFF937B6A, 0xFF9604A6, 0xFF98848A, 0xFF9AFB5B,
    0xFF9D6959, 0xFF9FCEC3, 0xFFA22BD4, 0xFFA480C6, 0xFFA6CDD0, 0xFFA91327, 0xFFAB50FD, 0xFFAD8784,
    0xFFAFB6EA, 0xFFB1DF5E, 0xFFB4010A, 0xFFB61C19, 0xFFB830B3, 0xFFBA3F00, 0xFFBC4724, 0xFFBE4946,
    0xFFC04587, 0xFFC23C0A, 0xFFC42CEF, 0xFFC61857, 0xFFC7FE60, 0xFFC9DF28, 0xFFCBBACB, 0xFFCD9166,
    0xFFCF6313, 0xFFD12FED, 0xFFD2F80D, 0xFFD4BB8B, 0xFFD67A80, 0xFFD83502, 0xFFD9EB29, 0xFFDB9D08,
    0xFFDD4AB7, 0xFFDEF449, 0xFFE099D2, 0xFFE23B66, 0xFFE3D918, 0xFFE572F9, 0xFFE7091D, 0xFFE89B93,
    0xFFEA2A6D, 0xFFEBB5BC, 0xFFED3D8F, 0xFFEEC1F6, 0xFFF04300, 0xFFF1C0BC, 0xFFF33B38, 0xFFF4B283,
    0xFFF626A9, 0xFFF797B9, 0xFFF905BF, 0xFFFA70C9, 0xFFFBD8E2, 0xFFFD3E16, 0xFFFEA072, 0x00000000,
};

// Retail .data at 0x8054C798 (size 0x190) - attack attenuation table.
s32 __HBMSYNAttackAttnTable[100] = {
    0xFC400000, 0xFE70DF7B, 0xFEAD1437, 0xFED04C17, 0xFEE948F4, 0xFEFCAABF, 0xFF0C80D3, 0xFF19E480,
    0xFF257DB0, 0xFF2FB8B2, 0xFF38DF7B, 0xFF4126C9, 0xFF48B58F, 0xFF4FA961, 0xFF56193C, 0xFF5C175A,
    0xFF61B26C, 0xFF66F677, 0xFF6BED6F, 0xFF709FAA, 0xFF751437, 0xFF79511C, 0xFF7D5B85, 0xFF8137F2,
    0xFF84EA4C, 0xFF887602, 0xFF8BDE1E, 0xFF8F254E, 0xFF924DF9, 0xFF955A42, 0xFF984C17, 0xFF9B2533,
    0xFF9DE729, 0xFFA09365, 0xFFA32B33, 0xFFA5AFC4, 0xFFA8222B, 0xFFAA8369, 0xFFACD466, 0xFFAF15FD,
    0xFFB148F4, 0xFFB36E03, 0xFFB585D8, 0xFFB79111, 0xFFB99042, 0xFFBB83F6, 0xFFBD6CAE, 0xFFBF4AE4,
    0xFFC11F08, 0xFFC2E985, 0xFFC4AABF, 0xFFC66313, 0xFFC812DA, 0xFFC9BA68, 0xFFCB5A0A, 0xFFCCF20D,
    0xFFCE82B5, 0xFFD00C46, 0xFFD18EFE, 0xFFD30B1A, 0xFFD480D3, 0xFFD5F05E, 0xFFD759EF, 0xFFD8BDB7,
    0xFFDA1BE5, 0xFFDB74A5, 0xFFDCC821, 0xFFDE1683, 0xFFDF5FF0, 0xFFE0A48E, 0xFFE1E480, 0xFFE31FE8,
    0xFFE456E7, 0xFFE5899C, 0xFFE6B825, 0xFFE7E29E, 0xFFE90923, 0xFFEA2BCE, 0xFFEB4AB9, 0xFFEC65FD,
    0xFFED7DB0, 0xFFEE91EA, 0xFFEFA2C0, 0xFFF0B047, 0xFFF1BA94, 0xFFF2C1BB, 0xFFF3C5CD, 0xFFF4C6DE,
    0xFFF5C4FE, 0xFFF6C040, 0xFFF7B8B2, 0xFFF8AE66, 0xFFF9A16B, 0xFFFA91CF, 0xFFFB7FA0, 0xFFFC6AEE,
    0xFFFD53C4, 0xFFFE3A31, 0xFFFF1E41, 0x00000000,
};

// Per-voice mixer state (layout inferred from retail ASM offsets)
typedef struct HBMSYNVoice {
    u8  _pad0[0x04];          // 0x00
    void* mixChannel;          // 0x04 - HBMMIXChannel pointer
    struct HBMSYNSYNTH* voiceDataBase; // 0x08 - owning synthesizer
    u8   voiceIndex;           // 0x0C
    u8   _padD;                // 0x0D
    u8   volumeIndex;          // 0x0E - index into __HBMSYNVolumeAttenuation
    u8   pan;                  // 0x0F
    void* volumeInfo;          // 0x10 - volume info struct (value at +0x04)
    u8   _pad14[0x18];         // 0x14-0x2B
    s32  accumInput;           // 0x2C
    u8   _pad30[0x04];         // 0x30
    s32  accumInput2;          // 0x34
} HBMSYNVoice;

// Synthesizer layout (mirrors synctrl.c HBMSYNSYNTH; only the fields this
// TU reads are named).
typedef struct HBMSYNSYNTH {
    u8  _pad0[0x68];           // 0x00-0x67
    s32 masterVolume;          // 0x68 - base fader
    s32 volume[16];            // 0x6C - per-voice volume table
    s32 pan[16];               // 0xAC - per-voice pan table
    u8  ctrl[16];              // 0xEC - per-voice control table
} HBMSYNSYNTH;

// Read a u32 value from a per-voice s32 table, indexed by voiceIndex.
static s32 voiceTableS32(const s32* table, u8 index) {
    return table[index];
}

// Read a u8 value from a per-voice byte table, indexed by voiceIndex.
static u8 voiceTableU8(const u8* table, u8 index) {
    return table[index];
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
    voice->pan = voiceTableU8(voice->voiceDataBase->ctrl, voice->voiceIndex);
}

s32 __HBMSYNGetVoiceInput(HBMSYNVoice* voice)
{
    return (voice->accumInput + voice->accumInput2) >> 16;
}

s32 __HBMSYNGetVoiceFader(HBMSYNVoice* voice)
{
    s32 baseFader = voice->voiceDataBase->masterVolume;
    s32 voiceFader = voiceTableS32(voice->voiceDataBase->volume, voice->voiceIndex);
    return (baseFader + voiceFader) >> 16;
}

void __HBMSYNUpdateMix(HBMSYNVoice* voice)
{
    // Combined input = (accumInput + accumInput2) >> 16
    s32 input = (voice->accumInput + voice->accumInput2) >> 16;
    HBMMIXSetInput(voice->mixChannel, input);

    // AuxA = pan[voiceIndex] >> 16
    s32 auxA = voiceTableS32(voice->voiceDataBase->pan, voice->voiceIndex) >> 16;
    HBMMIXSetAuxA(voice->mixChannel, auxA);

    // Fader = (masterVolume + volume[voiceIndex]) >> 16
    s32 baseFader = voice->voiceDataBase->masterVolume;
    s32 voiceFader = voiceTableS32(voice->voiceDataBase->volume, voice->voiceIndex);
    s32 fader = (baseFader + voiceFader) >> 16;
    HBMMIXSetFader(voice->mixChannel, fader);

    // Pan = ctrl[voiceIndex] (byte table, no shift)
    u8 panVal = voiceTableU8(voice->voiceDataBase->ctrl, voice->voiceIndex);
    HBMMIXSetPan(voice->mixChannel, panVal);
}

#ifdef __cplusplus
}
#endif
