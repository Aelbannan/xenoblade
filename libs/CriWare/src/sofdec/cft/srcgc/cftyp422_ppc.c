#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

/* YCbCr conversion constants shared by the sofdec colour-space converters.
 * +0x00..+0x14: six f32 coefficients; +0x18: f64 int->float conversion
 * bias (0x4330000080000000 = 2^52 + 2^31). */
struct CFT_YCCConsts {
    float k0;   /* +0x00 Y scale */
    float k1;   /* +0x04 Y offset */
    float k2;   /* +0x08 Cr scale */
    float k3;   /* +0x0C Cb scale */
    float k4;   /* +0x10 G scale */
    float k5;   /* +0x14 B scale */
    f64   bias; /* +0x18 itof bias */
};
extern float lbl_eu_805197B0[];  /* YCbCr conversion constants + f64 conversion bias */
extern float lbl_eu_805FDDC0[];  /* output lookup tables (5 tables of 256 floats) */

/* Y84C44 word-packing from an (A) and (B) chroma source word.
 * Expressed with byte shifts so MWCC collapses them into rlwimi chains. */
#define Y84_PACK0(A, B)                                                            \
    (((B) & 0xff) | ((((A) >> 16) & 0xff) << 8) | ((((B) >> 24) & 0xff) << 16) | ((((A) >> 8) & 0xff) << 24))
#define Y84_PACK1(A, B)                                                            \
    ((((A) & 0xff) << 8) | (((B) >> 16) & 0xff) | (((B) & 0xff) << 16) | ((((A) >> 24) & 0xff) << 24))

/* Build the five YCbCr 4:2:0 -> ARGB8888 lookup tables.
 * Int->float uses the shared 0x4330-exponent bit-reform idiom with the
 * bias constant read from the literal pool (+0x18).
 * Even entries use centres one step early for the +0xC00/+0x1000 tables
 * ((i - 127)); preserved verbatim from retail.
 */
void CFT_Ycc420plnToArgb8888Init(void) {
    struct CFT_YCCConsts *cf = (struct CFT_YCCConsts *)lbl_eu_805197B0;
    float *tblBase = lbl_eu_805FDDC0;
    /* two conversion slots so both int->double results stay live */
    union {
        f64 d;
        u32 w[2];
    } ca, cb;
    float *tblY = &tblBase[0x000];
    float *tblCr = &tblBase[0x100];
    float *tblCb = &tblBase[0x200];
    float *tblG = &tblBase[0x300];
    float *tblB = &tblBase[0x400];
    /* coefficients cached in locals so they hoist out of the loop */
    f64 bias = cf->bias;
    float k0 = cf->k0;
    float k1 = cf->k1;
    float k2 = cf->k2;
    float k3 = cf->k3;
    float k4 = cf->k4;
    float k5 = cf->k5;
    int i;

    ca.w[0] = 0x43300000;
    cb.w[0] = 0x43300000;
    for (i = 0; i < 256; i += 2) {
        f64 v128, v15, v127, v16;
        cb.w[1] = (u32)(i - 128) ^ 0x80000000;
        v128 = cb.d - bias;
        tblCr[i] = k2 * (float)v128;
        tblCb[i] = k3 * (float)v128;
        ca.w[1] = (u32)(i - 127) ^ 0x80000000;
        v127 = ca.d - bias;
        tblG[i] = k4 * (float)v127;
        tblB[i] = k5 * (float)v127;
        cb.w[1] = (u32)(i - 16) ^ 0x80000000;
        v16 = cb.d - bias;
        tblY[i] = k1 + k0 * (float)v16;

        ca.w[1] = (u32)(i - 15) ^ 0x80000000;
        v15 = ca.d - bias;
        tblCr[i + 1] = k2 * (float)v15;
        tblCb[i + 1] = k3 * (float)v15;
        tblY[i + 1] = k1 + k0 * (float)v15;
        tblG[i + 1] = k4 * (float)v15;
        tblB[i + 1] = k5 * (float)v15;
    }
}

/* Convert a YCbCr 4:2:0 planar frame to the packed Y84C44 format.
 *
 * Loop 1 copies the luma (Y) plane, four rows at a time, into the destination
 * cache lines.  Loop 2 re-packs the two chroma planes (Cr/Cb) into the Y84C44
 * 4:2:2 word layout using byte-permutation gathers.
 */
void CFT_Ycc420plnToY84C44(SFXCnvSrcBuf* src, SFXDstBufInf* dst, u32* table) {
    /* Load the luma-plane parameters and derive the cache-line loop bounds. */
    s32 oi;    u32 W = src->_10;                  /* luma plane pitch in pixels */
    s32 w3 = 3 * (s32)W;               /* 3W row delta (shared with r3) */
    s32 hRows = w3 / 8;                /* src advance (x8) per outer iter */
    u32 dstPitch = dst->_10;           /* destination luma stride */
    s32 inCnt = (s32)W / 8;            /* inner cache-line count */
    s32 oCnt = (s32)(dst->_0C) / 4;    /* outer row count */
    u8* yRow = (u8*)(src->_04);        /* source luma plane */
    u8* dRow = (u8*)(dst->_04) - 8;    /* destination luma base - 8 */
    s32 dAdv = (s32)((s32)dstPitch - (s32)W) / 8; /* dst advance (x32) per outer iter */
    u32 twoW = 2 * W;                  /* row pointer delta */
    u32 h8 = 8 * hRows;                /* yRow advance per outer iter */
    s32 d32 = 32 * dAdv;               /* dRow advance per outer iter */

    for (oi = 0; oi < oCnt; oi++) {
        u8* r1 = yRow + W;
        u8* r2 = yRow + twoW;
        u8* r3 = yRow + w3;
        s32 ii;
        for (ii = 0; ii < inCnt; ii++) {
            /* dRow holds base-8; index 8 targets the line preceding the first store */
            __dcbz(dRow, 8);
            f64 v0 = *(f64*)yRow;
            f64 v1 = *(f64*)r1;
            f64 v2 = *(f64*)r2;
            f64 v3 = *(f64*)r3;
            __dcbt(yRow, 8);
            yRow += 8;
            r1 += 8;
            r2 += 8;
            r3 += 8;
            dRow += 8;
            *(f64*)dRow = v0;
            dRow += 8;
            *(f64*)dRow = v1;
            dRow += 8;
            *(f64*)dRow = v2;
            dRow += 8;
            *(f64*)dRow = v3;
        }
        dRow += d32;
        yRow += h8;
    }

    /* ---- chroma re-pack loop ---- */
    {
        /* fields reloaded in retail order: _10, _0C, _14, _20, _14, _24 */
        s32 W2 = src->_10;
        s32 H2 = dst->_0C;
        u32* d2 = (u32*)(dst->_14);   /* chroma destination */
        s32 cLen = src->_20;          /* chroma row byte length */
        u32* cr = (u32*)src->_14;     /* Cr plane */
        u32* cb = (u32*)src->_24;     /* Cb plane */
        s32 inCnt2 = W2 / 8;                              /* inner word count */
        s32 halfW = W2 / 2;
        s32 bOff = (cLen - halfW) / 4;                    /* dst row advance (x32) */
        s32 oCnt2 = H2 / 8;                               /* outer count */

        /* d2 advance per outer iter (bytes), chroma base advance (bytes) */
        s32 rowAdv = 32 * bOff;
        s32 sOff = cLen / 4;                              /* sub-plane word stride */
        s32 colAdv = 4 * (3 * sOff + bOff);

        for (oi = 0; oi < oCnt2; oi++) {
            /* derived sub-plane pointers, fixed per outer iteration */
            u32* b2 = cb + sOff;
            u32* b3 = cb + 2 * sOff;
            u32* b4 = cb + 3 * sOff;
            u32* c2 = cr + sOff;
            u32* c3 = cr + 2 * sOff;
            u32* c4 = cr + 3 * sOff;
            s32 k;
            for (k = 0; k < inCnt2; k++) {
                __dcbz((u8*)d2 - 4, 4);
                u32 a = *cb++;
                u32 cc = *cr++;
                d2[0] = Y84_PACK0(a, cc);
                d2[1] = Y84_PACK1(a, cc);
                a = *b2++;
                cc = *c2++;
                d2[2] = Y84_PACK0(a, cc);
                d2[3] = Y84_PACK1(a, cc);
                a = *b3++;
                cc = *c3++;
                d2[4] = Y84_PACK0(a, cc);
                d2[5] = Y84_PACK1(a, cc);
                a = *b4++;
                cc = *c4++;
                d2[6] = Y84_PACK0(a, cc);
                d2[7] = Y84_PACK1(a, cc);
                d2 += 8;
            }
            d2 = (u32*)((u8*)d2 + rowAdv);
            cb = (u32*)((u8*)cb + colAdv);
            cr = (u32*)((u8*)cr + colAdv);
        }
    }
}
