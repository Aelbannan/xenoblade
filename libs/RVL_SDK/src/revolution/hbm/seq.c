// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/hbm/seq
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" {
#include <revolution/OS.h>
}

typedef signed short s16;
typedef float f32;
typedef double f64;

typedef struct _HBMSEQSEQUENCE HBMSEQSEQUENCE;

/* Per-track playback state (0x28 bytes per track). */
typedef struct _HBMSEQTRACK {
    HBMSEQSEQUENCE *seq;          /* 0x00 owning sequence */
    u8 *start;                    /* 0x04 first event byte */
    u8 *end;                      /* 0x08 one past the last event byte */
    u8 *cur;                      /* 0x0C current event pointer */
    u8 running_status;            /* 0x10 MIDI running status byte */
    u8 pad_11[3];                 /* 0x11 */
    f32 tempo;                    /* 0x14 microseconds per quarter note */
    u32 field_0x18;               /* 0x18 */
    u32 delay_count;              /* 0x1C ticks remaining before the event */
    u32 delay;                    /* 0x20 tick interval */
    u32 sub_state;                /* 0x24 playback sub-state */
} HBMSEQTRACK;

/* Sequence control block. Tracks start at offset 0x241C. */
typedef struct _HBMSEQSEQUENCE {
    struct _HBMSEQSEQUENCE *next; /* 0x00 linked list */
    u32 state;                    /* 0x04 playback state */
    u16 num_tracks;               /* 0x08 */
    s16 field_0x0A;               /* 0x0A tempo divisor */
    u32 field_0x0C;               /* 0x0C active track count */
    u32 field_0x10;               /* 0x10 completion flag */
    u8 midi_input[0x2408];        /* 0x14 MIDI input scratch area */
    HBMSEQTRACK tracks[1];        /* 0x241C */
} HBMSEQSEQUENCE;

/* Length of each MIDI channel-voice event (indexed by status - 0x80). */
u8 __HBMSEQMidiEventLength[128] = {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* .bss state owned by this unit in the retail DOL (retail .bss is 0x20
   bytes: sequence list head + init flag + 0x18 gap padding). */
static u32 __init;
HBMSEQSEQUENCE *__HBMSEQSequenceList;
/* Unreferenced .bss pad matching the 0x18-byte tail of the retail .bss. */
u8 HBMSEQPad[0x18];

extern "C" void HBMSYNMidiInput(void *syn, const u8 *data);
extern "C" void HBMSYNInitSynth(void *syn, u32 config, u32 p3, u32 p4);
extern "C" void HBMSYNQuitSynth(void *syn);

#pragma push
#pragma auto_inline off
void __HBMSEQInitTracks(HBMSEQSEQUENCE *seq, u8 *data, int count)
{
    HBMSEQTRACK *track;
    u32 tag;
    u32 len;

    track = &seq->tracks[0];
    while (count != 0) {
    retry:
        tag = *(u32 *)data;
        len = *(u32 *)(data + 4);
        data += 8;
        if (tag == 0x4D54726B) {
            track->seq = seq;
            track->start = data;
            track->end = data + len;
            track->cur = data;
            track->field_0x18 =
                (u32)(65536.0f *
                      (96.0f / (16000.0f / (f32)(s16)seq->field_0x0A)));
            data += len;
            track->sub_state = 0;
        } else {
            data += len;
            goto retry;
        }
        count--;
        track++;
    }
}

/* Track data begins after the 14-byte header plus the MThd body length
   (data_size includes the 6-byte signature+length prefix). */
void __HBMSEQReadHeader(HBMSEQSEQUENCE *seq, u8 *data)
{
    u8 *track_data;
    u8 *event_data;
    u32 num;
    u32 data_size;
    u16 field_a;

    data_size = *(u32 *)(data + 4);
    track_data = data + 14;
    num = *(u16 *)(data + 8);
    field_a = *(u16 *)(data + 0xA);

    seq->num_tracks = field_a;
    track_data += data_size;
    seq->field_0x0A = *(s16 *)(data + 0xC);
    event_data = track_data - 6;

    switch (num) {
    case 0:
        seq->num_tracks = 1;
        __HBMSEQInitTracks(seq, event_data, 1);
        break;
    case 1:
        __HBMSEQInitTracks(seq, event_data, field_a);
        break;
    }

    seq->field_0x0C = seq->num_tracks;
}
#pragma pop

extern "C" void HBMSEQInit()
{
    if (__init != 0) {
        return;
    }
    __HBMSEQSequenceList = NULL;
    __init = 1;
}

extern "C" void HBMSEQQuit()
{
    __HBMSEQSequenceList = NULL;
    __init = 0;
}

/* Reads a MIDI variable-length quantity at *pp and advances *pp past it. */
static u32 HBMSEQReadVarInt(u8 **pp)
{
    u32 v;
    u8 b;

    b = **pp;
    v = b & 0x7F;
    while (b & 0x80) {
        (*pp)++;
        b = **pp;
        v = (v << 7) + (b & 0x7F);
    }
    return v;
}

extern "C" void HBMSEQSetState(HBMSEQSEQUENCE *seq, u32 state)
{
    u32 intr;

    switch (state) {
    case 1:
    case 2:
        if (seq->state == 0) {
            u32 v;
            HBMSEQTRACK *track;
            u8 b;
            int i;

            intr = OSDisableInterrupts();
            track = &seq->tracks[0];
            for (i = 0; i < seq->num_tracks; i++) {
                track->cur = track->start;
                track->delay_count = track->field_0x18;

                /* Read a MIDI variable-length quantity (consumes the final
                   byte, so cur ends one past the value). */
                b = *track->cur;
                v = b & 0x7F;
                while (b & 0x80) {
                    track->cur++;
                    b = *track->cur;
                    v = (v << 7) + (b & 0x7F);
                }
                track->cur++;

                track->delay = v << 16;
                track->sub_state = 1;
                track++;
            }
            seq->field_0x0C = seq->num_tracks;
            OSRestoreInterrupts(intr);
        }
        seq->field_0x10 = 0;
        break;

    case 0:
    case 3:
        {
            u8 data[3];
            int j;

            for (j = 0; j < 16; j++) {
                u32 intr2 = OSDisableInterrupts();
                data[0] = 0xB0 | j;
                data[1] = 0x7B;
                data[2] = 0;
                HBMSYNMidiInput(&seq->midi_input, data);
                OSRestoreInterrupts(intr2);
            }
        }
        break;
    }

    seq->state = state;
}

extern "C" void HBMSEQRunAudioFrame(void)
{
    HBMSEQSEQUENCE *seq;
    HBMSEQSEQUENCE *seqp;
    HBMSEQTRACK *track;
    u32 i;
    u32 remaining;
    u8 b;
    u8 rs;
    u8 b1;
    u8 b2;
    u8 b3;
    u32 v;
    char data[3];
    f32 f;
    u32 tempo32;

    seq = __HBMSEQSequenceList;
    if (__init == 0) {
        return;
    }
    while (seq != NULL) {
        if (seq->state == 1 || seq->state == 2) {
            for (i = 0; i < seq->num_tracks; i++) {
                track = &seq->tracks[i];
                if (track->sub_state == 1 || track->sub_state == 2) {
                    remaining = track->delay_count;
                    if (track->delay > remaining) {
                        track->delay -= remaining;
                        continue;
                    }
                    while (remaining >= track->delay) {
                        remaining -= track->delay;

                        /* play one MIDI event */
                        b = *track->cur;
                        if (b >= 0x80) {
                            track->running_status = b;
                            track->cur++;
                        }
                        rs = track->running_status;
                        switch (rs) {
                        case 0xF0:
                        case 0xF7:
                            /* system common: skip the payload */
                            v = HBMSEQReadVarInt(&track->cur);
                            track->cur++;
                            track->cur += v;
                            break;
                        case 0xFF:
                            /* meta event */
                            b = *track->cur;
                            track->cur++;
                            switch (b) {
                            case 0x2F:
                                /* end of track */
                                seqp = track->seq;
                                seqp->field_0x0C--;
                                track->sub_state = 0;
                                if (seqp->field_0x0C == 0) {
                                    seqp->field_0x10 = 1;
                                }
                                break;
                            case 0x51:
                                /* tempo change */
                                v = HBMSEQReadVarInt(&track->cur);
                                track->cur++;
                                b1 = *track->cur;
                                track->cur++;
                                b2 = *track->cur;
                                track->cur++;
                                b3 = *track->cur;
                                track->cur++;
                                tempo32 =
                                    ((((u32)b1 << 8) + b2) << 8) + b3;
                                f = (f32)tempo32;
                                seqp = track->seq;
                                track->tempo = 1000000.0f / f;
                                f = 32000.0f / track->tempo;
                                f = f / (f32)seqp->field_0x0A;
                                f = 96.0f / f;
                                track->delay_count =
                                    (u32)(65536.0f * f);
                                break;
                            default:
                                v = HBMSEQReadVarInt(&track->cur);
                                track->cur++;
                                track->cur += v;
                                break;
                            }
                            break;
                        default:
                            /* channel voice event */
                            data[0] = rs;
                            v = __HBMSEQMidiEventLength[rs - 0x80];
                            switch (v) {
                            case 1:
                                data[1] = *track->cur;
                                track->cur++;
                                break;
                            case 2:
                                data[1] = *track->cur;
                                track->cur++;
                                data[2] = *track->cur;
                                track->cur++;
                                break;
                            }
                            HBMSYNMidiInput(&seq->midi_input,
                                             (u8 *)data);
                            break;
                        }

                        /* track finished? */
                        if (track->cur >= track->end) {
                            seqp = track->seq;
                            seqp->field_0x0C--;
                            track->sub_state = 0;
                            if (seqp->field_0x0C == 0) {
                                seqp->field_0x10 = 1;
                            }
                        }
                        if (track->sub_state == 0) {
                            break;
                        }
                        v = HBMSEQReadVarInt(&track->cur);
                        track->cur++;
                        track->delay = v << 16;
                    }
                    track->delay -= remaining;
                }
            }
        }
        if (seq->field_0x10 != 0) {
            if (seq->state == 2) {
                HBMSEQSetState(seq, 0);
                HBMSEQSetState(seq, 2);
            } else {
                HBMSEQSetState(seq, 0);
            }
        }
        seq = seq->next;
    }
}

extern "C" void HBMSEQAddSequence(HBMSEQSEQUENCE *seq, u8 *data, u32 config,
                                  u32 p3, u32 p4)
{
    u32 intr;

    HBMSYNInitSynth(&seq->midi_input, config, p3, p4);
    seq->state = 0;
    __HBMSEQReadHeader(seq, data);
    intr = OSDisableInterrupts();
    if (__HBMSEQSequenceList != NULL) {
        seq->next = __HBMSEQSequenceList;
    } else {
        seq->next = NULL;
    }
    __HBMSEQSequenceList = seq;
    OSRestoreInterrupts(intr);
}

extern "C" void HBMSEQRemoveSequence(HBMSEQSEQUENCE *seq)
{
    HBMSEQSEQUENCE *next;
    HBMSEQSEQUENCE *cur;
    u32 intr;

    intr = OSDisableInterrupts();
    cur = __HBMSEQSequenceList;
    __HBMSEQSequenceList = NULL;
    while (cur != NULL) {
        next = cur->next;
        if (cur != seq) {
            /* Re-insert every other sequence (interrupt lock is re-taken so
               the list rebuild is atomic with respect to the audio thread). */
            u32 intr2 = OSDisableInterrupts();
            if (__HBMSEQSequenceList != NULL) {
                cur->next = __HBMSEQSequenceList;
            } else {
                cur->next = NULL;
            }
            __HBMSEQSequenceList = cur;
            OSRestoreInterrupts(intr2);
        }
        cur = next;
    }
    OSRestoreInterrupts(intr);
    HBMSYNQuitSynth(&seq->midi_input);
}

extern "C" u32 HBMSEQGetState(HBMSEQSEQUENCE *seq) { return seq->state; }

extern "C" void HBMSYNSetMasterVolume(void*);
extern "C" void HBMSEQSetVolume(HBMSEQSEQUENCE *seq) { HBMSYNSetMasterVolume(seq->midi_input); }
