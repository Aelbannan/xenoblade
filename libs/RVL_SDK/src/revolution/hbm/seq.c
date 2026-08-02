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
const u8 __HBMSEQMidiEventLength[128] = {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
};

extern u32 __init;
extern HBMSEQSEQUENCE *__HBMSEQSequenceList;

extern "C" void HBMSYNMidiInput(void *syn, const u8 *data);

void __HBMSEQInitTracks(HBMSEQSEQUENCE *seq, u8 *data, int count)
{
    HBMSEQTRACK *track;
    u32 tag;
    u32 len;
    f32 f;

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
            f = 16000.0f / (f32)(s16)seq->field_0x0A;
            f = 96.0f / f;
            track->field_0x18 = (u32)(65536.0f * f);
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

void __HBMSEQReadHeader__FP15_HBMSEQSEQUENCEPUc() {}

void HBMSEQInit() {}

void HBMSEQQuit() {}

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
    int i;
    u32 intr;

    switch (state) {
    case 1:
    case 2:
        if (seq->state == 0) {
            intr = OSDisableInterrupts();
            for (i = 0; i < seq->num_tracks; i++) {
                HBMSEQTRACK *track = &seq->tracks[i];
                u8 b;
                u32 v;

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
            }
            seq->field_0x0C = seq->num_tracks;
            OSRestoreInterrupts(intr);
        }
        seq->field_0x10 = 0;
        break;

    case 0:
    case 3:
        for (i = 0; i < 16; i++) {
            u8 data[3];
            intr = OSDisableInterrupts();
            data[0] = 0xB0 | i;
            data[1] = 0x7B;
            data[2] = 0;
            HBMSYNMidiInput(&seq->midi_input, data);
            OSRestoreInterrupts(intr);
        }
        break;
    }

    seq->state = state;
}

extern "C" void HBMSEQRunAudioFrame(void)
{
    HBMSEQSEQUENCE *seq;
    HBMSEQTRACK *track;
    u32 i;
    u32 remaining;
    u8 b;
    u32 v;
    u8 data[3];
    f32 f;
    u32 tempo32;

    if (__init == 0) {
        return;
    }

    seq = __HBMSEQSequenceList;
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
                    if (remaining < track->delay) {
                        track->delay -= remaining;
                        continue;
                    }
                    for (;;) {
                        remaining -= track->delay;

                        /* play one MIDI event */
                        b = *track->cur;
                        if (b >= 0x80) {
                            track->running_status = b;
                            track->cur++;
                        }
                        if (track->running_status == 0xF0 ||
                            track->running_status == 0xF7) {
                            /* system common: skip the payload */
                            v = HBMSEQReadVarInt(&track->cur);
                            track->cur += 1 + v;
                        } else if (track->running_status == 0xFF) {
                            /* meta event */
                            b = *track->cur++;
                            if (b == 0x2F) {
                                /* end of track */
                                track->seq->field_0x0C--;
                                track->sub_state = 0;
                                if (track->seq->field_0x0C == 0) {
                                    track->seq->field_0x10 = 1;
                                }
                            } else if (b == 0x51) {
                                /* tempo change */
                                v = HBMSEQReadVarInt(&track->cur);
                                tempo32 = ((u32)track->cur[1] << 16) |
                                          ((u32)track->cur[2] << 8) |
                                          track->cur[3];
                                track->cur += 4;
                                f = (f32)((f64)tempo32);
                                track->tempo = 1000000.0f / f;
                                f = 32000.0f / track->tempo;
                                f = f / (f32)((f64)seq->field_0x0A);
                                f = 96.0f / f;
                                track->delay_count =
                                    (u32)(65536.0f * f);
                            } else {
                                v = HBMSEQReadVarInt(&track->cur);
                                track->cur += 1 + v;
                            }
                        } else {
                            /* channel voice event */
                            v = __HBMSEQMidiEventLength[track->running_status -
                                                        0x80];
                            data[0] = track->running_status;
                            if (v == 1) {
                                data[1] = *track->cur++;
                            } else if (v == 2) {
                                data[1] = *track->cur++;
                                data[2] = *track->cur++;
                            }
                            HBMSYNMidiInput(&seq->midi_input, data);
                        }

                        /* track finished? */
                        if (track->cur >= track->end) {
                            track->seq->field_0x0C--;
                            track->sub_state = 0;
                            if (track->seq->field_0x0C == 0) {
                                track->seq->field_0x10 = 1;
                            }
                        }
                        if (track->sub_state == 0) {
                            track->delay -= remaining;
                            break;
                        }
                        track->delay = HBMSEQReadVarInt(&track->cur) << 16;
                        if (remaining < track->delay) {
                            break;
                        }
                    }
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

void HBMSEQAddSequence() {}

void HBMSEQRemoveSequence() {}

extern "C" u32 HBMSEQGetState(void* self) { return *(u32*)((u8*)self + 0x4); }

extern "C" void HBMSYNSetMasterVolume(void*);
extern "C" void HBMSEQSetVolume(void* self) { HBMSYNSetMasterVolume((char*)self + 0x14); }
