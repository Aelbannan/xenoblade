/**
 * synpitch.c — HBM synth pitch/sample-rate conversion routines
 */

#include <harness_catalog.h>
#include <revolution/AX/AXVPB.h>

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned long __cvt_fp2unsigned(double);

/* ---- lookup tables (contiguous in .data) ---- */

extern f32 __HBMSYNCentsTable[];

/* ================================================================== */
/*  HBMSYNVOICE — local struct                                         */
/* ================================================================== */

struct HBMSYNPitchPreset {
    u8  note;
    s16 fineTune;
};

struct HBMWaveInfo {
    u16 format;
    u16 sampleRate;
};

typedef struct /* HBMSYNVOICE */ {
    u8                     _00[0x04];
    AXVPB*                 pb;              /* 0x04 */
    void*                  sampleData;      /* 0x08 */
    u8                     _0C;             /* 0x0C */
    u8                     baseNote;        /* 0x0D */
    u8                     _0E[0x02];       /* 0x0E-0x0F */
    struct HBMSYNPitchPreset* preset;       /* 0x10 */
    u8                     _14[0x04];       /* 0x14-0x17 */
    struct HBMWaveInfo*    waveInfo;        /* 0x18 */
    void*                  adpcmCoef;       /* 0x1C */
    u32                    loopFlag;        /* 0x20 */
    f32                    pitch;           /* 0x24 */
    s32                    pitchFull;       /* 0x28  Q16.16 */
} HBMSYNVOICE;

/* ================================================================== */

f32 __HBMSYNGetRelativePitch(HBMSYNVOICE* voice)
{
    /* Load base pointer early so the lis/la pair is before the branch. */
    f32* ct   = __HBMSYNCentsTable;
    s32 v     = voice->pitchFull / 65536;

    if (v > 0) {
        s32 oct   = v / 1200;
        s32 rem   = v - oct * 1200;
        s32 sem   = rem / 100;
        s32 cent  = v % 100;

        f32* octTbl = ct + 100;   /* byte offset 0x190 */
        f32* semUp  = ct + 112;   /* byte offset 0x1C0 */

        f32 r  = octTbl[oct];
        r     *= semUp[sem];
        r     *= ct[cent];
        return r;
    }

    if (v < 0) {
        f32* semDn  = ct + 124;   /* byte offset 0x1F0 */

        s32 sem = v / 100;
        s32 cent = v % 100;
        if (cent < 0) {
            cent += 100;
            sem--;
        }
        f32 r  = semDn[-sem];
        r     *= ct[cent];
        return r;
    }

    return 1.0f;
}

/* ================================================================== */

void __HBMSYNSetupPitch(HBMSYNVOICE* voice)
{
    voice->pitch = (f32)voice->waveInfo->sampleRate / 32000.0f;

    voice->pitchFull = (voice->baseNote - voice->preset->note) * 100;
    voice->pitchFull += voice->preset->fineTune;
    voice->pitchFull <<= 16;
}

/* ================================================================== */

void __HBMSYNSetupSrc(HBMSYNVOICE* voice)
{
    f32 mul = voice->pitch * __HBMSYNGetRelativePitch(voice);
    u32 sr  = __cvt_fp2unsigned(65536.0f * mul);

    voice->pb->pb.srcSelect = AX_SRC_TYPE_LINEAR;

    {
        AXVPB* pb = voice->pb;
        pb->pb.src.ratioHi = sr >> 16;
        pb->pb.src.ratioLo = sr;
        pb->pb.src.currentAddressFrac = 0;
        pb->pb.src.last_samples[0] = 0;
        pb->pb.src.last_samples[1] = 0;
        pb->pb.src.last_samples[2] = 0;
        pb->pb.src.last_samples[3] = 0;
    }

    voice->pb->sync &= ~AX_PBSYNC_SRC_RATIO;
    voice->pb->sync |= AX_PBSYNC_SRC | AX_PBSYNC_SELECT;
}

/* ================================================================== */

void __HBMSYNUpdateSrc(HBMSYNVOICE* voice)
{
    f32 mul = voice->pitch * __HBMSYNGetRelativePitch(voice);
    u32 sr  = __cvt_fp2unsigned(65536.0f * mul);

    *((u32*)&voice->pb->pb.src.ratioHi) = sr;

    voice->pb->sync |= AX_PBSYNC_SRC_RATIO;
}

#ifdef __cplusplus
} /* extern "C" */
#endif
