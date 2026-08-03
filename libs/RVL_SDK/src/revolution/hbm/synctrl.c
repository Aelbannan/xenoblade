// High-level C reconstruction of RVL_SDK/src/revolution/hbm/synctrl
#include <types.h>
#include <revolution/ax/AXVPB.h>
#include <revolution/ax/AXAlloc.h>

struct HBMSYNSYNTH;
struct HBMSYNVOICE;

extern u32 __HBMSYNVolumeAttenuation[];
extern struct HBMSYNVOICE* __HBMSYNVoice;

// Retail .data (0x200): coefficient table used by synenv.c's __HBMSYNn128
// references; defined here because the retail places it in this unit.
s32 __HBMSYNn128[128] = {
    0x00000000, 0x3C000219, 0x3C800000, 0x3CC0010C, 0x3D000000, 0x3D200086, 0x3D400000, 0x3D600086,
    0x3D800000, 0x3D900043, 0x3DA00000, 0x3DB00043, 0x3DC00000, 0x3DD00043, 0x3DE00000, 0x3DF00043,
    0x3E000000, 0x3E080022, 0x3E100000, 0x3E180022, 0x3E200000, 0x3E280022, 0x3E300000, 0x3E380022,
    0x3E400000, 0x3E480022, 0x3E500000, 0x3E580022, 0x3E600000, 0x3E680022, 0x3E700000, 0x3E780022,
    0x3E800000, 0x3E840011, 0x3E880000, 0x3E8C0011, 0x3E900000, 0x3E940011, 0x3E980000, 0x3E9C0011,
    0x3EA00000, 0x3EA40011, 0x3EA80000, 0x3EAC0011, 0x3EB00000, 0x3EB40011, 0x3EB80000, 0x3EBC0011,
    0x3EC00000, 0x3EC40011, 0x3EC80000, 0x3ECC0011, 0x3ED00000, 0x3ED40011, 0x3ED80000, 0x3EDC0011,
    0x3EE00000, 0x3EE40011, 0x3EE80000, 0x3EEC0011, 0x3EF00000, 0x3EF40011, 0x3EF80000, 0x3EFC0011,
    0x3F000000, 0x3F020008, 0x3F040000, 0x3F060008, 0x3F080000, 0x3F0A0008, 0x3F0C0000, 0x3F0E0008,
    0x3F100000, 0x3F120008, 0x3F140000, 0x3F160008, 0x3F180000, 0x3F1A0008, 0x3F1C0000, 0x3F1E0008,
    0x3F200000, 0x3F220008, 0x3F240000, 0x3F260008, 0x3F280000, 0x3F2A0008, 0x3F2C0000, 0x3F2E0008,
    0x3F300000, 0x3F320008, 0x3F340000, 0x3F360008, 0x3F380000, 0x3F3A0008, 0x3F3C0000, 0x3F3E0008,
    0x3F400000, 0x3F420008, 0x3F440000, 0x3F460008, 0x3F480000, 0x3F4A0008, 0x3F4C0000, 0x3F4E0008,
    0x3F500000, 0x3F520008, 0x3F540000, 0x3F560008, 0x3F580000, 0x3F5A0008, 0x3F5C0000, 0x3F5E0008,
    0x3F600000, 0x3F620008, 0x3F640000, 0x3F660008, 0x3F680000, 0x3F6A0008, 0x3F6C0000, 0x3F6E0008,
    0x3F700000, 0x3F720008, 0x3F740000, 0x3F760008, 0x3F780000, 0x3F7A0008, 0x3F7C0000, 0x3F7E0008,
};

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
