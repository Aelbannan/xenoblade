// Decompiled: synsample.c — ADPCM/PCM16/PCM8 sample setup
// High-level C reconstruction from retail ASM reference.

#include <harness_catalog.h>

#define ADPCM_SAMPLES_PER_FRAME 14
#define ADPCM_FRAME_SIZE 8  // 1 header + 7 nibble data

typedef struct HBMSYNVOICE {
    u32     _00;
    u8*     pOut;
    u32*    pWave;
    u32     _0C;
    u32*    pSample;
    u32     _14;
    u32*    pRegion;
    u32*    pAdpcm;
    s32     active;
} HBMSYNVOICE;

// Output byte offsets (packed)
#define O_W32(ofs, val)  *(s32*)(out + (ofs)) = (val)
#define O_WU32(ofs, val) *(u32*)(out + (ofs)) = (val)
#define O_W16(ofs, val)  *(u16*)(out + (ofs)) = (val)
#define O_RU32(ofs)      (*(u32*)(out + (ofs)))

// Forward declarations
void __HBMSYNSetupAdpcm__FP11HBMSYNVOICE(HBMSYNVOICE*);
void __HBMSYNSetupPcm16__FP11HBMSYNVOICE(HBMSYNVOICE*);
void __HBMSYNSetupPcm8__FP11HBMSYNVOICE(HBMSYNVOICE*);

// ---------------------------------------------------------------------------
// __HBMSYNSetupSample
// ---------------------------------------------------------------------------
void __HBMSYNSetupSample(HBMSYNVOICE* voice)
{
    u32* rp = voice->pRegion;
    u16 fmt = (u16)rp[0];
    if (fmt == 0)
        __HBMSYNSetupAdpcm__FP11HBMSYNVOICE(voice);
    else if (fmt == 1)
        __HBMSYNSetupPcm16__FP11HBMSYNVOICE(voice);
    else
        __HBMSYNSetupPcm8__FP11HBMSYNVOICE(voice);
}

// ---------------------------------------------------------------------------
// __HBMSYNSetupAdpcm__FP11HBMSYNVOICE
// ---------------------------------------------------------------------------
void __HBMSYNSetupAdpcm__FP11HBMSYNVOICE(HBMSYNVOICE* voice)
{
    u32* sp = voice->pSample;
    u8*  out = voice->pOut;
    u32* wp = voice->pWave;
    u32* rp = voice->pRegion;
    u32* ap = voice->pAdpcm;

    s32 loopStart = (s32)sp[2];
    s32 loopEnd   = (s32)sp[3];
    s32 total     = loopStart + loopEnd;
    s32 basePtr   = (s32)wp[9] + (s32)rp[1];

    if (total != 0) {
        voice->active = 1;
        O_WU32(0x96, 0x10000);

        // startAddr = basePtr + (loopStart - (loopStart/14)*14) + (((loopStart*4/7)*2) & ~0xF) + 2
        {
            s32 n = loopStart / 14;
            s32 x = (loopStart * 4) / 7;
            s32 y = (x * 2) & ~0xF;
            O_W32(0x9A, basePtr + (loopStart - n * 14) + y + 2);
        }

        // endAddr = basePtr + ((total-1) - ((total-1)/14)*14) + ((((total-1)*4/7)*2) & ~0xF) + 2
        {
            s32 t = total - 1;
            s32 n = t / 14;
            s32 x = (t * 4) / 7;
            s32 y = (x * 2) & ~0xF;
            O_W32(0x9E, basePtr + (t - n * 14) + y + 2);
        }

        O_W32(0xA2, basePtr + 2);
    } else {
        voice->active = 0;
        O_WU32(0x96, 0);
        O_W32(0x9A, basePtr + 2);
        O_W32(0xA2, basePtr + 2);

        {
            s32 totalSamp = (s32)rp[2];
            s32 t = totalSamp - 1;
            s32 n = t / 14;
            s32 x = (t * 4) / 7;
            s32 y = (x * 2) & ~0xF;
            O_W32(0x9E, basePtr + (t - n * 14) + y + 2);
        }
    }

    // Copy ADPCM coefficients — 10 u32 + 3 u16
    // Unrolled to match retail byte pattern.
    O_W32(0xA6, (s32)ap[0]);  O_W32(0xAA, (s32)ap[1]);
    O_W32(0xAE, (s32)ap[2]);  O_W32(0xB2, (s32)ap[3]);
    O_W32(0xB6, (s32)ap[4]);  O_W32(0xBA, (s32)ap[5]);
    O_W32(0xBE, (s32)ap[6]);  O_W32(0xC2, (s32)ap[7]);
    O_W32(0xC6, (s32)ap[8]);  O_W32(0xCA, (s32)ap[9]);
    // Three u16 values at AP+0x28 (predictor), +0x2A (stepIdx), +0x2C (loopCtx)
    O_W16(0xDC, *(u16*)((u8*)ap + 0x28));
    O_W16(0xDE, *(u16*)((u8*)ap + 0x2A));
    O_W16(0xE0, *(u16*)((u8*)ap + 0x2C));

    // Set output flags
    {
        u32 f = O_RU32(0x1C);
        f = (f & ~0x7800) | 0x40000 | 0x8400;
        O_WU32(0x1C, f);
    }
}

// ---------------------------------------------------------------------------
// __HBMSYNSetupPcm16__FP11HBMSYNVOICE
// ---------------------------------------------------------------------------
void __HBMSYNSetupPcm16__FP11HBMSYNVOICE(HBMSYNVOICE* voice)
{
    u32* sp = voice->pSample;
    u8*  out = voice->pOut;
    u32* wp = voice->pWave;
    u32* rp = voice->pRegion;

    s32 loopStart = (s32)sp[2];
    s32 loopEnd   = (s32)sp[3];
    s32 total     = loopStart + loopEnd;
    s32 basePtr   = (s32)wp[7] + (s32)rp[1];

    if (total != 0) {
        s32 startAddr = basePtr + loopStart;
        s32 endAddr   = startAddr + loopEnd - 1;

        voice->active = 1;
        O_WU32(0x96, 0x1000A);
        O_W32(0x9A, startAddr);
        O_W32(0x9E, endAddr);
        O_W32(0xA2, basePtr);

        O_W32(0xA6, 0); O_W32(0xAA, 0); O_W32(0xAE, 0); O_W32(0xB2, 0);
        O_W32(0xB6, 0); O_W32(0xBA, 0); O_W32(0xBE, 0); O_W32(0xC2, 0);
        O_W32(0xC6, 0x800); O_W32(0xCA, 0);
    } else {
        s32 totalSamp = (s32)rp[2];
        s32 endAddr   = basePtr + totalSamp - 1;

        voice->active = 0;
        O_WU32(0x96, 0xA);
        O_W32(0x9A, basePtr);
        O_W32(0x9E, endAddr);
        O_W32(0xA2, basePtr);

        O_W32(0xA6, 0); O_W32(0xAA, 0); O_W32(0xAE, 0); O_W32(0xB2, 0);
        O_W32(0xB6, 0); O_W32(0xBA, 0); O_W32(0xBE, 0); O_W32(0xC2, 0);
        O_W32(0xC6, 0x800); O_W32(0xCA, 0);
    }

    {
        u32 f = O_RU32(0x1C);
        f = (f & ~0x7800) | 0x40000 | 0x8400;
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
    u32* wp = voice->pWave;
    u32* rp = voice->pRegion;

    s32 loopStart = (s32)sp[2];
    s32 loopEnd   = (s32)sp[3];
    s32 total     = loopStart + loopEnd;
    s32 basePtr   = (s32)wp[8] + (s32)rp[1];

    if (total != 0) {
        s32 startAddr = basePtr + loopStart;
        s32 endAddr   = startAddr + loopEnd - 1;

        voice->active = 1;
        O_WU32(0x96, 0x10019);
        O_W32(0x9A, startAddr);
        O_W32(0x9E, endAddr);
        O_W32(0xA2, basePtr);

        O_W32(0xA6, 0); O_W32(0xAA, 0); O_W32(0xAE, 0); O_W32(0xB2, 0);
        O_W32(0xB6, 0); O_W32(0xBA, 0); O_W32(0xBE, 0); O_W32(0xC2, 0);
        O_W32(0xC6, 0x100); O_W32(0xCA, 0);
    } else {
        s32 totalSamp = (s32)rp[2];
        s32 endAddr   = basePtr + totalSamp - 1;

        voice->active = 0;
        O_WU32(0x96, 0x19);
        O_W32(0x9A, basePtr);
        O_W32(0x9E, endAddr);
        O_W32(0xA2, basePtr);

        O_W32(0xA6, 0); O_W32(0xAA, 0); O_W32(0xAE, 0); O_W32(0xB2, 0);
        O_W32(0xB6, 0); O_W32(0xBA, 0); O_W32(0xBE, 0); O_W32(0xC2, 0);
        O_W32(0xC6, 0x100); O_W32(0xCA, 0);
    }

    {
        u32 f = O_RU32(0x1C);
        f = (f & ~0x7800) | 0x40000 | 0x8400;
        O_WU32(0x1C, f);
    }
}
