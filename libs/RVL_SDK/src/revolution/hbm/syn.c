// High-level C reconstruction of RVL_SDK/src/revolution/hbm/syn
// No inline asm, no register tricks. Matches retail via EQUIVALENT_MATCH.

#include <types.h>
#include <revolution/os/OSInterrupt.h>
#include <revolution/ax/AXVPB.h>
#include <revolution/ax/AXAlloc.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations of external functions in the HBM/OS/AX subsystems
void __HBMSYNResetAllControllers(struct HBMSYNSYNTH*);
void __HBMSYNRunInputBufferEvents(struct HBMSYNSYNTH*);
void __HBMSYNServiceVoice(s32);
void HBMMIXReleaseChannel(void*);
void HBMFreeIndexByKey(void*);
BOOL AXIsInit(void);

// ---- BSS globals (single contiguous .bss block) ----
extern struct HBMSYNSYNTH* __HBMSYNSynthList; // .bss:0x0 | size 4
extern void* __HBMSYNVoice;                   // .bss:0x4 | size 4 — pointer to voice array
extern u32 __init;                             // .bss:0x4C8 | size 4 — init flag

// ---- HBMSYNSYNTH — synthesizer instance structure ----
// Offsets inferred from retail ASM (syn.c, synctrl.c, synvoice.c, seq.c)
typedef struct HBMSYNSYNTH {
    struct HBMSYNSYNTH* next;   // 0x00 — linked-list next
    void* dataSections[6];      // 0x04-0x18 — pointers into sound data
    u32  field1C;               // 0x1C — playback speed param (from r5)
    u32  field20;               // 0x20 — playback speed param (from r5 + 0x80000000)
    u32  field24;               // 0x24 — playback speed param (from r5 << 1)
    u8   pad28[0x68 - 0x28];   // 0x28-0x67
    u32  masterVolume;          // 0x68 — master volume (<< 16)
    u8   pad6C[0xFC - 0x6C];   // 0x6C-0xFB
    u8   midiBuffer[0x300];    // 0xFC-0x3FB — MIDI input buffer
    u8*  midiWritePtr;          // 0x3FC — current write pos in midiBuffer
    u32  midiCount;             // 0x400 — number of MIDI bytes received
    u32  activeVoiceFlag;       // 0x404 — non-zero when voices are active
    u8   voiceData[0x2000];    // 0x408-0x2407 — per-voice state blocks
} HBMSYNSYNTH;

// ---- Per-voice entry (size 0x4C) ----
typedef struct {
    u8   pad00[0x04];          // 0x00
    void* mixChannel;           // 0x04 — HBMMIXChannel*
    struct HBMSYNSYNTH* synth; // 0x08 — owning synthesizer
    u8   pad0C[0x4C - 0x0C];   // 0x0C-0x4B — rest of voice state
} HBMSYNVoiceEntry;

// ---- Function implementations ----

void __HBMSYNRemoveSynthFromList__FP11HBMSYNSYNTH(HBMSYNSYNTH* synth)
{
    BOOL intr = OSDisableInterrupts();
    HBMSYNSYNTH* cur = __HBMSYNSynthList;
    HBMSYNSYNTH* last = NULL;
    HBMSYNSYNTH* first = NULL;

    while (cur != NULL) {
        if (cur != synth) {
            if (first != NULL) {
                last->next = cur;
                last = cur;
            } else {
                last = cur;
                first = cur;
            }
        }
        cur = cur->next;
    }

    if (last != NULL) {
        last->next = NULL;
    }

    __HBMSYNSynthList = first;
    OSRestoreInterrupts(intr);
}

void HBMSYNInit(void)
{
    u8* base = (u8*)&__HBMSYNSynthList;

    if (!AXIsInit()) {
        return;
    }

    u32* initFlag = (u32*)(base + 0x4C8);

    if (*initFlag != 0) {
        return;
    }

    // __HBMSYNVoice = &__s_HBMSYNVoice (at base + 8)
    *(void**)(base + 4) = base + 8;

    // Clear the synth reference in every voice (16 voices x 0x4C bytes)
    {
        s32 i;
        for (i = 0; i < 16; i++) {
            ((HBMSYNVoiceEntry*)__HBMSYNVoice)[i].synth = NULL;
        }
    }

    // __HBMSYNSynthList = NULL
    *(HBMSYNSYNTH**)base = NULL;

    *initFlag = 1;
}

void HBMSYNQuit(void)
{
    __HBMSYNVoice = NULL;
    __init = 0;
}

void HBMSYNRunAudioFrame(void)
{
    if (__init == 0) {
        return;
    }

    s32 i;
    for (i = 0; i < 16; i++) {
        __HBMSYNServiceVoice(i);
    }

    HBMSYNSYNTH* syn = __HBMSYNSynthList;
    while (syn != NULL) {
        __HBMSYNRunInputBufferEvents(syn);
        syn = syn->next;
    }
}

void HBMSYNInitSynth(HBMSYNSYNTH* syn, u32* config, u32 param3)
{
    u32 v = param3 + 0x80000000;

    syn->dataSections[0] = (u8*)config + config[0];
    syn->dataSections[1] = (u8*)config + config[1];
    syn->dataSections[2] = (u8*)config + config[2];
    syn->dataSections[3] = (u8*)config + config[3];
    syn->dataSections[4] = (u8*)config + config[4];
    syn->dataSections[5] = (u8*)config + config[5];

    syn->field1C = v >> 1;
    syn->field20 = v;
    syn->field24 = v << 1;

    syn->masterVolume = 0;

    __HBMSYNResetAllControllers(syn);

    syn->midiWritePtr = syn->midiBuffer;
    syn->midiCount = 0;
    syn->activeVoiceFlag = 0;

    // Clear voice data region: 16 groups x 0x200 bytes, zeroed 4 words at a time
    {
        u32* p = (u32*)syn->voiceData;
        u32* end = (u32*)(syn->voiceData + sizeof(syn->voiceData));
        while (p < end) {
            *p++ = 0;
        }
    }

    // Insert this synth into the global linked list (under interrupt lock)
    {
        BOOL intr = OSDisableInterrupts();
        HBMSYNSYNTH* head = __HBMSYNSynthList;
        if (head != NULL) {
            syn->next = head;
        } else {
            syn->next = NULL;
        }
        __HBMSYNSynthList = syn;
        OSRestoreInterrupts(intr);
    }
}

void HBMSYNQuitSynth(HBMSYNSYNTH* syn)
{
    BOOL intr = OSDisableInterrupts();

    if (syn->activeVoiceFlag != 0) {
        HBMSYNVoiceEntry* voices = (HBMSYNVoiceEntry*)__HBMSYNVoice;
        s32 i;
        for (i = 0; i < 16; i++) {
            HBMSYNVoiceEntry* v = &voices[i];
            if (v->synth == syn) {
                HBMMIXReleaseChannel(v->mixChannel);
                HBMFreeIndexByKey(*(void**)((u8*)v->mixChannel + 0x18));
                AXFreeVoice((AXVPB*)v->mixChannel);
                v->synth = NULL;
            }
        }
    }

    __HBMSYNRemoveSynthFromList__FP11HBMSYNSYNTH(syn);

    OSRestoreInterrupts(intr);
}

void HBMSYNMidiInput(HBMSYNSYNTH* syn, const u8* data)
{
    u8* p;
    p = syn->midiWritePtr;
    *p = data[0];
    p = syn->midiWritePtr;
    syn->midiWritePtr = p + 1;
    p = syn->midiWritePtr;
    *p = data[1];
    p = syn->midiWritePtr;
    syn->midiWritePtr = p + 1;
    p = syn->midiWritePtr;
    *p = data[2];
    p = syn->midiWritePtr;
    syn->midiWritePtr = p + 1;
    syn->midiCount++;
}

void HBMSYNSetMasterVolume(HBMSYNSYNTH* syn, u32 vol)
{
    syn->masterVolume = vol << 16;
}

#ifdef __cplusplus
}
#endif
