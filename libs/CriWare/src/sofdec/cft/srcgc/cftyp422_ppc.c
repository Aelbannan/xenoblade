#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfx/sfx_types.h"

extern float lbl_eu_805197B0[];  /* YCbCr conversion constants + f64 conversion bias */
extern float lbl_eu_805FDDC0[];  /* output lookup tables (5 tables of 256 floats) */

/* Y84C44 word-packing from an (A) and (B) chroma source word.
 * Expressed with byte shifts so MWCC collapses them into rlwimi chains.
 */
#define Y84_PACK0(A, B)                                                            \
    (((B) & 0xff) | ((((A) >> 16) & 0xff) << 8) | ((((B) >> 24) & 0xff) << 16) | ((((A) >> 8) & 0xff) << 24))
#define Y84_PACK1(A, B)                                                            \
    (((((A) >> 24) & 0xff) << 24) | (((A) & 0xff) << 8) | (((B) >> 16) & 0xff))

/* Build the five YCbCr 4:2:0 -> ARGB8888 lookup tables:
 *   +0x0000: Y  table: tbl[i] = 0.5 + 1.1643835*(i - 16)
 *   +0x0400: Cr table: tbl[i] = 1.5960267*(i - 128)
 *   +0x0800: Cb table: tbl[i] = 2.0172322*(i - 128)
 *   +0x0C00: G  table: tbl[i] = -0.3916071*(i - 128)
 *   +0x1000: B  table: tbl[i] = -0.8128125*(i - 128)
 */
void CFT_Ycc420plnToArgb8888Init(void) {
    float *tblY = &lbl_eu_805FDDC0[0x000];
    float *tblCr = &lbl_eu_805FDDC0[0x100];
    float *tblCb = &lbl_eu_805FDDC0[0x200];
    float *tblG = &lbl_eu_805FDDC0[0x300];
    float *tblB = &lbl_eu_805FDDC0[0x400];

    int i;
    for (i = 0; i < 256; i += 2) {
        float y0 = lbl_eu_805197B0[0] * (float)(i - 16);

        tblCr[i] = lbl_eu_805197B0[2] * (float)(i - 128);
        tblCb[i] = lbl_eu_805197B0[3] * (float)(i - 128);
        tblG[i] = lbl_eu_805197B0[4] * (float)(i - 128);
        tblB[i] = lbl_eu_805197B0[5] * (float)(i - 128);
        tblY[i] = lbl_eu_805197B0[1] + y0;

        float y1 = lbl_eu_805197B0[0] * (float)(i - 15);

        tblCr[i + 1] = lbl_eu_805197B0[2] * (float)(i - 127);
        tblCb[i + 1] = lbl_eu_805197B0[3] * (float)(i - 127);
        tblG[i + 1] = lbl_eu_805197B0[4] * (float)(i - 127);
        tblB[i + 1] = lbl_eu_805197B0[5] * (float)(i - 127);
        tblY[i + 1] = lbl_eu_805197B0[1] + y1;
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
    u32 W = src->_10;                  /* luma plane pitch in pixels */
    s32 hRows = (s32)(3 * (s32)W) / 8; /* src advance (x8) per outer iter */
    s32 inCnt = (s32)W / 8;            /* inner cache-line count */
    u32 dstPitch = dst->_10;           /* destination luma stride */
    s32 dAdv = (s32)((s32)dstPitch - (s32)W) / 8; /* dst advance (x32) per outer iter */
    s32 oCnt = (s32)(dst->_0C) / 4;    /* outer row count */

    u8* dRow = (u8*)(dst->_04) - 8;    /* destination luma base - 8 */
    u8* yRow = (u8*)(src->_04);        /* source luma plane */

    s32 oi;
    for (oi = 0; oi < oCnt; oi++) {
        u8* r1 = yRow + W;
        u8* r2 = yRow + 2 * (ptrdiff_t)W;
        u8* r3 = yRow + 3 * (ptrdiff_t)W;
        s32 ii;
        for (ii = 0; ii < inCnt; ii++) {
            /* dRow holds base-8; index 8 targets the line preceding the first store */
            __dcbz(dRow, 8);
            f64 v1 = *(f64*)r1;
            f64 v2 = *(f64*)r2;
            f64 v3 = *(f64*)r3;
            f64 v0 = *(f64*)yRow;
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
        dRow += 32 * dAdv;
        yRow += 8 * hRows;
    }

    /* ---- chroma re-pack loop ---- */
    {
        u32 cbBase = src->_24;        /* first chroma plane */
        u32 crBase = src->_14;        /* second chroma plane */
        u32 cLen = src->_20;          /* chroma row byte length */

        s32 sOff = (s32)cLen / 4;                        /* sub-plane word stride */
        s32 bOff = (s32)((s32)cLen - (s32)(W / 2)) / 4;  /* dst row advance (x32) */
        s32 inCnt2 = (s32)(W + (u32)(W >> 31)) / 8;      /* inner word count */
        s32 oCnt2 = (s32)(dst->_0C + (u32)(dst->_0C >> 31)) / 8; /* outer count */

        u32* cb = (u32*)cbBase;
        u32* cr = (u32*)crBase;
        u32* d2 = (u32*)(dst->_14);   /* chroma destination */

        /* d2 advance per outer iter (bytes), chroma base advance (bytes) */
        s32 rowAdv = 32 * bOff;
        s32 colAdv = 4 * (3 * sOff + bOff);

        s32 oj2;
        for (oj2 = 0; oj2 < oCnt2; oj2++) {
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