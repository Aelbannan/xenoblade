// Decompiled: synsample.c - ADPCM/PCM16/PCM8 sample setup
// High-level C reconstruction from retail ASM reference.

#include <harness_catalog.h>
#include <revolution/AX/AXPB.h>

#define ADPCM_SAMPLES_PER_FRAME 14
#define ADPCM_FRAME_SIZE 8  // 1 header + 7 nibble data

// name-guess: local name for the ADPCM coefficient/loop-tail block consumed by
// __HBMSYNSetupAdpcm; the three u16 tail fields use the SDK's AXPBADPCMLOOP layout.
typedef struct HBMSYNADPCM {
    u32 coef[10];
    AXPBADPCMLOOP loop;
} HBMSYNADPCM;

typedef struct HBMSYNVOICE {
    u32     _00;
    u8*     pOut;
    u32*    pWave;
    u32     _0C;
    u32*    pSample;
    u32     _14;
    u32*    pRegion;
    HBMSYNADPCM* pAdpcm;
    s32     active;
} HBMSYNVOICE;

// Output byte offsets (packed)
#define O_W32(ofs, val)  *(s32*)(out + (ofs)) = (val)
#define O_WU32(ofs, val) *(u32*)(out + (ofs)) = (val)
#define O_W16(ofs, val)  *(u16*)(out + (ofs)) = (val)
#define O_RU32(ofs)      (*(u32*)(out + (ofs)))

// All exported functions must use C linkage to match retail symbol names.
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
void __HBMSYNSetupAdpcm__FP11HBMSYNVOICE(HBMSYNVOICE*);
void __HBMSYNSetupPcm16__FP11HBMSYNVOICE(HBMSYNVOICE*);
void __HBMSYNSetupPcm8__FP11HBMSYNVOICE(HBMSYNVOICE*);
void __HBMSYNSetupSample(HBMSYNVOICE*);

// ---------------------------------------------------------------------------
// __HBMSYNSetupAdpcm__FP11HBMSYNVOICE
// ---------------------------------------------------------------------------
void __HBMSYNSetupAdpcm__FP11HBMSYNVOICE(HBMSYNVOICE* voice)
{
    u32* sp = voice->pSample;
    u8*  out = voice->pOut;

    if (sp[2] + sp[3] != 0) {
        voice->active = 1;

        u32 loopStart = sp[2];
        u32 loopEnd   = sp[3];
        u32 basePtr   = voice->pWave[9] + voice->pRegion[1];
        HBMSYNADPCM* ap = voice->pAdpcm;

        // endAddr = basePtr + ((total-1) % 14) + ((total-1) / 14) * 16 + 2
        u32 t2 = loopStart + loopEnd - 1;
        u32 q2 = t2 / 14;
        u32 m2 = t2 % 14;

        // startAddr = basePtr + (loopStart % 14) + (loopStart / 14) * 16 + 2
        u32 q1 = loopStart / 14;
        u32 m1 = loopStart % 14;

        u32 a2 = (m2 + basePtr) + ((q2 * 16) + 2);
        u32 a1 = (m1 + basePtr) + ((q1 * 16) + 2);

        O_WU32(0x96, 0x10000);
        O_W32(0x9A, a1);
        O_W32(0x9E, a2);
        O_W32(0xA2, basePtr + 2);

        // Copy ADPCM coefficients - 10 u32 + 3 u16 (then-branch only)
        O_W32(0xA6, ap->coef[0]);  O_W32(0xAA, ap->coef[1]);
        O_W32(0xAE, ap->coef[2]);  O_W32(0xB2, ap->coef[3]);
        O_W32(0xB6, ap->coef[4]);  O_W32(0xBA, ap->coef[5]);
        O_W32(0xBE, ap->coef[6]);  O_W32(0xC2, ap->coef[7]);
        O_W32(0xC6, ap->coef[8]);  O_W32(0xCA, ap->coef[9]);
        O_W16(0xDC, ap->loop.loop_pred_scale);
        O_W16(0xDE, ap->loop.loop_yn1);
        O_W16(0xE0, ap->loop.loop_yn2);

        O_WU32(0x1C, (O_RU32(0x1C) & ~0x7800) | 0x40000 | 0x8400);
    } else {
        voice->active = 0;

        HBMSYNADPCM* ap = voice->pAdpcm;
        u32 totalSamp = voice->pRegion[2];
        u32 basePtr   = voice->pWave[9] + voice->pRegion[1];

        O_WU32(0x96, 0);
        O_W32(0x9A, basePtr);

        // endAddr = basePtr + ((totalSamp-1) % 14) + ((totalSamp-1) / 14) * 16 + 2
        {
            u32 q = (totalSamp - 1) / 14;
            u32 m = (totalSamp - 1) % 14;
            O_W32(0x9E, (m + basePtr) + ((q << 4) + 2));
        }

        O_W32(0xA2, basePtr + 2);

        // Copy ADPCM coefficients - 10 u32 (else-branch has no u16 tail)
        O_W32(0xA6, ap->coef[0]);  O_W32(0xAA, ap->coef[1]);
        O_W32(0xAE, ap->coef[2]);  O_W32(0xB2, ap->coef[3]);
        O_W32(0xB6, ap->coef[4]);  O_W32(0xBA, ap->coef[5]);
        O_W32(0xBE, ap->coef[6]);  O_W32(0xC2, ap->coef[7]);
        O_W32(0xC6, ap->coef[8]);  O_W32(0xCA, ap->coef[9]);

        O_WU32(0x1C, (O_RU32(0x1C) & ~0x7800) | 0x8400);
    }
}

// ---------------------------------------------------------------------------
// __HBMSYNSetupPcm16__FP11HBMSYNVOICE
// ---------------------------------------------------------------------------
void __HBMSYNSetupPcm16__FP11HBMSYNVOICE(HBMSYNVOICE* voice)
{
    u32* sp = voice->pSample;
    u8*  out = voice->pOut;

    if ((s32)sp[2] + (s32)sp[3] != 0) {
        voice->active = 1;

        s32 basePtr  = (s32)voice->pWave[7] + (s32)voice->pRegion[1];
        s32 startAddr = basePtr + (s32)sp[2];
        s32 endAddr   = startAddr + (s32)sp[3] - 1;

        O_WU32(0x96, 0x1000A);
        O_W32(0x9A, startAddr);
        O_W32(0x9E, endAddr);
        O_W32(0xA2, basePtr);

        O_W32(0xA6, 0); O_W32(0xAA, 0); O_W32(0xAE, 0); O_W32(0xB2, 0);
        O_W32(0xB6, 0); O_W32(0xBA, 0); O_W32(0xBE, 0); O_W32(0xC2, 0);
        O_W32(0xC6, 0x08000000); O_W32(0xCA, 0);
    } else {
        voice->active = 0;

        s32 basePtr = (s32)voice->pWave[7] + (s32)voice->pRegion[1];
        s32 endAddr = basePtr + (s32)voice->pRegion[2] - 1;

        O_WU32(0x96, 0xA);
        O_W32(0x9A, basePtr);
        O_W32(0x9E, endAddr);
        O_W32(0xA2, basePtr);

        O_W32(0xA6, 0); O_W32(0xAA, 0); O_W32(0xAE, 0); O_W32(0xB2, 0);
        O_W32(0xB6, 0); O_W32(0xBA, 0); O_W32(0xBE, 0); O_W32(0xC2, 0);
        O_W32(0xC6, 0x08000000); O_W32(0xCA, 0);
    }

    {
        u32 f = O_RU32(0x1C);
        f = (f & ~0x7800) | 0x8400;
        O_WU32(0x1C, f);
    }
}

// ---------------------------------------------------------------------------
// __HBMSYNSetupPcm8__FP11HBMSYNVOICE
// ---------------------------------------------------------------------------
void __HBMSYNSetupPcm8__FP11HBMSYNVOICE(HBMSYNVOICE* voice)
{
    u32* sp = voice->pSample;
    u8*  out = voice->pOut;

    if ((s32)sp[2] + (s32)sp[3] != 0) {
        voice->active = 1;

        s32 basePtr  = (s32)voice->pWave[8] + (s32)voice->pRegion[1];
        s32 startAddr = basePtr + (s32)sp[2];
        s32 endAddr   = startAddr + (s32)sp[3] - 1;

        O_WU32(0x96, 0x10019);
        O_W32(0x9A, startAddr);
        O_W32(0x9E, endAddr);
        O_W32(0xA2, basePtr);

        O_W32(0xA6, 0); O_W32(0xAA, 0); O_W32(0xAE, 0); O_W32(0xB2, 0);
        O_W32(0xB6, 0); O_W32(0xBA, 0); O_W32(0xBE, 0); O_W32(0xC2, 0);
        O_W32(0xC6, 0x01000000); O_W32(0xCA, 0);
    } else {
        voice->active = 0;

        s32 basePtr = (s32)voice->pWave[8] + (s32)voice->pRegion[1];
        s32 endAddr = basePtr + (s32)voice->pRegion[2] - 1;

        O_WU32(0x96, 0x19);
        O_W32(0x9A, basePtr);
        O_W32(0x9E, endAddr);
        O_W32(0xA2, basePtr);

        O_W32(0xA6, 0); O_W32(0xAA, 0); O_W32(0xAE, 0); O_W32(0xB2, 0);
        O_W32(0xB6, 0); O_W32(0xBA, 0); O_W32(0xBE, 0); O_W32(0xC2, 0);
        O_W32(0xC6, 0x01000000); O_W32(0xCA, 0);
    }

    {
        u32 f = O_RU32(0x1C);
        f = (f & ~0x7800) | 0x8400;
        O_WU32(0x1C, f);
    }
}

// ---------------------------------------------------------------------------
// __HBMSYNSetupSample
// ---------------------------------------------------------------------------
void __HBMSYNSetupSample(HBMSYNVOICE* voice)
{
    switch (*(u16*)voice->pRegion) {
    case 0:
        __HBMSYNSetupAdpcm__FP11HBMSYNVOICE(voice);
        break;
    case 1:
        __HBMSYNSetupPcm16__FP11HBMSYNVOICE(voice);
        break;
    case 2:
        __HBMSYNSetupPcm8__FP11HBMSYNVOICE(voice);
        break;
    }
}

#ifdef __cplusplus
}
#endif
