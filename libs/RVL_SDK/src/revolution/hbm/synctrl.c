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
extern "C" void HBMMIXInitChannel(AXVPB*, s32, s32, s32, s32);
extern "C" void HBMMIXReleaseChannel(AXVPB*);
extern "C" void __HBMSYNSetupPitch(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupVolume(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupPan(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupVolumeEnvelope(struct HBMSYNVOICE*);
extern "C" s32 __HBMSYNGetVoiceFader(struct HBMSYNVOICE*);
extern "C" s32 __HBMSYNGetVoiceInput(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupSample(struct HBMSYNVOICE*);
extern "C" void __HBMSYNSetupSrc(struct HBMSYNVOICE*);
extern "C" void __HBMSYNClearVoiceReferences(void*);

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
    s32 pan[16];
    u8 ctrl[16];
    u8 inBuf[0x300];
    u32 inBufBase;
    u32 inBufCount;
    u32 activeVoiceCount;
    HBMSYNVOICE* voiceTable[16][128];
} HBMSYNSYNTH;

extern "C" {

//==============================================================================
// __HBMSYNResetAllControllers
//==============================================================================
void __HBMSYNResetAllControllers(HBMSYNSYNTH* synth)
{
    u32 volAtt0;
    u32 volAtt190;
    u8 i;

    volAtt0 = __HBMSYNVolumeAttenuation[0];
    volAtt190 = __HBMSYNVolumeAttenuation[0x190 / 4];

    for (i = 0; i < 16; i++) {
        synth->instrPtr[i] = synth->baseValue;
        synth->volume[i] = volAtt190;
        synth->ctrl[i] = 0x40;
        synth->pan[i] = volAtt0;
    }
}

} // extern "C"

//==============================================================================
// __HBMSYNNoteOn__FP11HBMSYNSYNTHUcUcUc
//==============================================================================
void __HBMSYNNoteOn(HBMSYNSYNTH* synth, u8 channel, u8 key, u8 velocity)
{
    if (velocity != 0) {
        AXVPB* axVoice;
        s32 vIdx;
        HBMSYNVOICE* sv;
        u16 iid;
        u8* ie;
        u32 ok;

        if (synth->voiceTable[channel][key] != NULL) {
            __HBMSYNSetVoiceToRelease(synth->voiceTable[channel][key]);
            synth->voiceTable[channel][key] = NULL;
        }

        axVoice = AXAcquireVoice(31, (AXVoiceCallback)__HBMSYNClearVoiceReferences,
                                 (u32)synth);
        if (axVoice == NULL) {
            return;
        }

        vIdx = HBMAllocIndex(axVoice->index);
        if (vIdx >= 0) {
            sv = &__HBMSYNVoice[vIdx];
            sv->axvpb = axVoice;
            sv->synth = synth;
            sv->channel = channel;
            sv->key = key;
            sv->velocity = velocity;

            iid = ((u16*)synth->instrPtr[channel])[key];
            if (iid == 0xFFFF) {
                ok = 0;
            } else {
                u32 kgi;
                u32 kgdi;
                u16 si;

                ok = 1;
                ie = (u8*)synth->instruments + iid * 0x18;
                sv->instrumentEntry = (void*)ie;

                kgi = *(u32*)(ie + 0x10);
                sv->region = (void*)((u8*)synth->regions + kgi * 0x50);

                kgdi = *(u32*)(ie + 0x14);
                sv->keygroup = (void*)((u8*)synth->keygroups + (kgdi << 4));

                si = *(u16*)((u8*)sv->keygroup + 0x0C);
                sv->sample = (void*)((u8*)synth->samples + si * 0x2E);
            }

            if (ok) {
                synth->voiceTable[channel][key] = sv;
                synth->activeVoiceCount = synth->activeVoiceCount + 1;

                __HBMSYNSetupPitch(sv);
                __HBMSYNSetupVolume(sv);
                __HBMSYNSetupPan(sv);
                __HBMSYNSetupVolumeEnvelope(sv);

                {
                    s32 fader = __HBMSYNGetVoiceFader(sv);
                    u32 vi = __HBMSYNGetVoiceInput(sv);
                    HBMMIXInitChannel(
                        axVoice, vi,
                        *(s32*)((u8*)synth + 0xAC + ((u32)channel << 2)) >> 16,
                        *(u8*)((u8*)synth + 0xEC + channel), fader);
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
        } else {
            AXFreeVoice(axVoice);
            return;
        }
    } else {
        // Note-off: release the voice bound to this channel/key slot.
        HBMSYNVOICE* voice;

        voice = synth->voiceTable[channel][key];
        if (voice != NULL) {
            __HBMSYNSetVoiceToRelease(voice);
            synth->voiceTable[channel][key] = NULL;
        }
    }
}

//==============================================================================
// __HBMSYNMidiIn__FP11HBMSYNSYNTHPUc
//==============================================================================
void __HBMSYNMidiIn(HBMSYNSYNTH* synth, u8* msg)
{
    u8 d1;
    u8 chan;
    u8 cmd;

    d1 = msg[1];
    chan = msg[0] & 0xF;
    cmd = msg[0] >> 4;

    switch (cmd) {
    case 8: {
        HBMSYNVOICE* voice;

        voice = synth->voiceTable[chan][d1];
        if (voice != NULL) {
            __HBMSYNSetVoiceToRelease(voice);
            synth->voiceTable[chan][d1] = NULL;
        }
        break;
    }
    case 9:
        __HBMSYNNoteOn(synth, chan, d1, msg[2]);
        break;
    case 11: {
        u8 d2 = msg[2];
        switch (d1) {
        case 7:
            synth->volume[chan] = __HBMSYNVolumeAttenuation[d2];
            break;
        case 10:
            synth->ctrl[chan] = d2;
            break;
        case 91:
            synth->pan[chan] = __HBMSYNVolumeAttenuation[d2];
            break;
        }
        break;
    }
    case 12:
        synth->instrPtr[chan] = synth->baseValue + (d1 << 8);
        break;
    }
}

extern "C" {

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
