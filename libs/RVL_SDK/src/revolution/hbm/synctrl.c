// High-level C reconstruction of RVL_SDK/src/revolution/hbm/synctrl
#include <types.h>
#include <revolution/ax/AXVPB.h>
#include <revolution/ax/AXAlloc.h>

struct HBMSYNSYNTH;
struct HBMSYNVOICE;

extern u32 __HBMSYNVolumeAttenuation[];
extern struct HBMSYNVOICE* __HBMSYNVoice;

extern "C" void __HBMSYNSetVoiceToRelease(struct HBMSYNVOICE*);
extern "C" s32 HBMAllocIndex(u32);
extern "C" void HBMFreeIndex(s32);
extern "C" void HBMMIXInitChannel(AXVPB*, u32, s16, u8, f32);
extern "C" void HBMMIXReleaseChannel(AXVPB*);
extern "C" void __HBMSYNSetupPitch(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupVolume(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupPan(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupVolumeEnvelope(struct HBMSYNVOICE*);
extern "C" f32 __HBMSYNGetVoiceFader(struct HBMSYNVOICE*);
extern "C" u32 __HBMSYNGetVoiceInput(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupSample(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupSrc(struct HBMSYNVOICE*);
extern "C" void __HBMSYNClearVoiceReferences(void*);
extern "C" void __HBMSYNNoteOn(struct HBMSYNSYNTH*, u8, u8, u8);

typedef struct HBMSYNVOICE {
    u32 index;
    AXVPB* axvpb;
    struct HBMSYNSYNTH* synth;
    u8 channel;
    u8 key;
    u8 velocity;
    u8 _pad0F;
    void* instrumentEntry;
    void* region;
    void* keygroup;
    void* sample;
    u8 rest[0x4C - 0x20];
} HBMSYNVOICE;

typedef struct HBMSYNSYNTH {
    struct HBMSYNSYNTH* next;
    struct HBMSYNSYNTH* prev;
    u32 baseValue;
    void* instruments;
    void* regions;
    void* keygroups;
    void* samples;
    u8 _pad1[0x28 - 0x1C];
    u32 instrPtr[16];
    u8 _pad2[0x6C - 0x68];
    u32 volume[16];
    u32 pan[16];
    u8 ctrl[16];
    u8 inBuf[0x300];
    u32 inBufBase;
    u32 inBufCount;
    u32 activeVoiceCount;
    HBMSYNVOICE* voiceTable[2048];
} HBMSYNSYNTH;

extern "C" {

//==============================================================================
// __HBMSYNResetAllControllers
//==============================================================================
void __HBMSYNResetAllControllers(HBMSYNSYNTH* synth)
{
    u32 baseVal;
    u32 volAtt0;
    u32 volAtt190;
    s32 i;

    baseVal = synth->baseValue;
    volAtt0 = __HBMSYNVolumeAttenuation[0];
    volAtt190 = __HBMSYNVolumeAttenuation[0x190 / 4];

    for (i = 0; i < 16; i++) {
        u32* pw;
        u8*  pb;

        pw = (u32*)((u8*)synth + (i * 32));
        pb = (u8*)synth + (i * 8);
        pw[0x28 >> 2] = baseVal;
        pw[0x6C >> 2] = volAtt190;
        pb[0xEC] = 0x40;
        pw[0xAC >> 2] = volAtt0;
    }
}

//==============================================================================
// __HBMSYNNoteOn__FP11HBMSYNSYNTHUcUcUc
//==============================================================================
void __HBMSYNNoteOn(HBMSYNSYNTH* synth, u8 channel, u8 key, u8 velocity)
{
    u32 tblBase;
    u32 kIdx;
    HBMSYNVOICE* existing;

    tblBase = (channel << 9) + 0x408;
    kIdx = (key << 2);

    existing = *(HBMSYNVOICE**)((u8*)synth + tblBase + kIdx);
    if (existing != NULL) {
        __HBMSYNSetVoiceToRelease(existing);
        *(HBMSYNVOICE**)((u8*)synth + tblBase + kIdx) = NULL;
    }

    if (velocity == 0) {
        return;
    }

    {
        AXVPB* axVoice;
        s32 vIdx;
        HBMSYNVOICE* sv;
        u16 iid;

        axVoice = AXAcquireVoice(31, (AXVoiceCallback)__HBMSYNClearVoiceReferences,
                                 (u32)synth);
        if (axVoice == NULL) {
            return;
        }

        vIdx = HBMAllocIndex(axVoice->index);
        if (vIdx < 0) {
            AXFreeVoice(axVoice);
            return;
        }

        sv = &__HBMSYNVoice[vIdx];
        sv->axvpb = axVoice;
        sv->synth = synth;
        sv->channel = channel;
        sv->key = key;
        sv->velocity = velocity;

        iid = ((u16*)((u8*)synth->instrPtr[channel]))[key];

        if (iid != 0xFFFF) {
            u8* ie;

            ie = (u8*)synth->instruments + iid * 0x18;
            sv->instrumentEntry = (void*)ie;

            {
                u32 kgi = *(u32*)(ie + 0x10);
                sv->region = (void*)((u8*)synth->regions + kgi * 0x50);
            }
            {
                u32 kgdi = *(u32*)(ie + 0x14);
                sv->keygroup = (void*)((u8*)synth->keygroups + (kgdi << 4));
            }
            {
                u16 si = *(u16*)((u8*)sv->keygroup + 0x0C);
                sv->sample = (void*)((u8*)synth->samples + si * 0x2E);
            }

            *(HBMSYNVOICE**)((u8*)synth + tblBase + kIdx) = sv;
            synth->activeVoiceCount = synth->activeVoiceCount + 1;

            __HBMSYNSetupPitch(sv);
            __HBMSYNSetupVolume(sv);
            __HBMSYNSetupPan(sv);
            __HBMSYNSetupVolumeEnvelope(sv);

            {
                f32 fader = __HBMSYNGetVoiceFader(sv);
                u32 vi = __HBMSYNGetVoiceInput(sv);
                HBMMIXInitChannel(axVoice, vi, (s16)(synth->pan[channel] >> 16),
                                  synth->ctrl[channel], fader);
            }

            __HBMSYNSetupSample(sv);
            __HBMSYNSetupSrc(sv);

            axVoice->pb.state = 1;
            axVoice->sync |= 4;
        } else {
            sv->synth = NULL;
            HBMMIXReleaseChannel(axVoice);
            HBMFreeIndex(vIdx);
            AXFreeVoice(axVoice);
        }
    }
}

//==============================================================================
// __HBMSYNMidiIn__FP11HBMSYNSYNTHPUc
//==============================================================================
void __HBMSYNMidiIn(HBMSYNSYNTH* synth, u8* msg)
{
    u8 status;
    u8 d1;
    u32 cmd;
    u8 chan;

    status = msg[0];
    d1 = msg[1];

    cmd = status >> 4;
    chan = status & 0xF;

    if (cmd == 8) {
        u32 off;
        HBMSYNVOICE* voice;
        off = (chan << 9) + (d1 << 2) + 0x408;
        voice = *(HBMSYNVOICE**)((u8*)synth + off);
        if (voice != NULL) {
            __HBMSYNSetVoiceToRelease(voice);
            *(HBMSYNVOICE**)((u8*)synth + off) = NULL;
        }
    } else if (cmd == 9) {
        __HBMSYNNoteOn(synth, chan, d1, msg[2]);
    } else if (cmd == 11) {
        u8 d2 = msg[2];
        if (d1 == 7) {
            synth->volume[chan] = __HBMSYNVolumeAttenuation[d2];
        } else if (d1 == 10) {
            synth->ctrl[chan] = d2;
        } else if (d1 == 91) {
            synth->pan[chan] = __HBMSYNVolumeAttenuation[d2];
        }
    } else if (cmd == 12) {
        synth->instrPtr[chan] = synth->baseValue + (d1 << 8);
    }
}

//==============================================================================
// __HBMSYNRunInputBufferEvents
//==============================================================================
void __HBMSYNRunInputBufferEvents(HBMSYNSYNTH* synth)
{
    u8* p;
    p = synth->inBuf;
    while (synth->inBufCount > 0) {
        __HBMSYNMidiIn(synth, p);
        p += 3;
        synth->inBufCount--;
    }
    synth->inBufBase = (u32)(synth->inBuf);
}

} // extern "C"
