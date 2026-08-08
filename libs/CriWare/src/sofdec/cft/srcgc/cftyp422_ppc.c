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
    (((B) >> 16 & 0xff) | ((((B) >> 16 & 0xff) | ((A) & 0xff)) << 8) | ((((B) >> 24) & 0xff) << 16) | ((B) << 24))

/* Build the five YCbCr 4:2:0 -> ARGB8888 lookup tables:
 *   +0x0000: Y  table: tbl[i] = 0.5 + 1.1643835*(i - 16)
 *   +0x0400: Cr table: tbl[i] = 1.5960267*(i - 128)
 *   +0x0800: Cb table: tbl[i] = 2.0172322*(i - 128)
 *   +0x0C00: G  table: tbl[i] = -0.3916071*(i - 128)
 *   +0x1000: B  table: tbl[i] = -0.8128125*(i - 128)
 */
void CFT_Ycc420plnToArgb8888Init(void) {
    float c0 = lbl_eu_805197B0[0];
    float c1 = lbl_eu_805197B0[1];
    float c2 = lbl_eu_805197B0[2];
    float c3 = lbl_eu_805197B0[3];
    float c4 = lbl_eu_805197B0[4];
    float c5 = lbl_eu_805197B0[5];

    float *tblY = &lbl_eu_805FDDC0[0x000];
    float *tblCr = &lbl_eu_805FDDC0[0x100];
    float *tblCb = &lbl_eu_805FDDC0[0x200];
    float *tblG = &lbl_eu_805FDDC0[0x300];
    float *tblB = &lbl_eu_805FDDC0[0x400];

    int i;
    for (i = 0; i < 256; i += 2) {
        float y0 = c0 * (float)(i - 16);

        tblCr[i] = c2 * (float)(i - 128);
        tblCb[i] = c3 * (float)(i - 128);
        tblG[i] = c4 * (float)(i - 128);
        tblB[i] = c5 * (float)(i - 128);
        tblY[i] = c1 + y0;

        float y1 = c0 * (float)(i - 15);

        tblCr[i + 1] = c2 * (float)(i - 127);
        tblCb[i + 1] = c3 * (float)(i - 127);
        tblG[i + 1] = c4 * (float)(i - 127);
        tblB[i + 1] = c5 * (float)(i - 127);
        tblY[i + 1] = c1 + y1;
    }
}

/* Convert a YCbCr 4:2:0 planar frame to the packed Y84C44 format.
 *
 * Loop 1 copies the luma (Y) plane, four rows at a time, into the destination
 * cache lines.  Loop 2 re-packs the two chroma planes (Cr/Cb) into the Y84C44
 * 4:2:2 word layout using byte-permutation gathers.
 */
void CFT_Ycc420plnToY84C44(SFXCnvSrcBuf* src, SFXDstBufInf* dst, u32* table) {
    u32 W = src->_10;                  /* luma plane pitch in pixels */
    u32 pitch = dst->_04;              /* destination luma base */
    u32 yfield = src->_04;             /* source luma plane */
    u32 dstPitch = dst->_10;           /* destination luma stride */

    s32 hRows = (s32)(3 * (s32)W) / 8;             /* src advance (x8) per outer iter */
    s32 inCnt = (s32)W / 8;                        /* inner cache-line count */
    s32 oCnt = (s32)(dst->_0C) / 4;                /* outer row count */
    s32 dAdv = (s32)((s32)dstPitch - (s32)W) / 8;  /* dst advance (x32) per outer iter */

    u8* yRow = (u8*)yfield;
    u8* dRow = (u8*)pitch - 8;

    s32 oi;
    for (oi = 0; oi < oCnt; oi++) {
        u8* r1 = yRow + W;
        u8* r2 = yRow + 2 * (ptrdiff_t)W;
        u8* r3 = yRow + 3 * (ptrdiff_t)W;
        s32 ii;
        for (ii = 0; ii < inCnt; ii++) {
            __dcbz(dRow + 8, 8);
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

        s32 ra = 4 * sOff;                  /* 1st sub-plane byte offset */
        s32 rb = 8 * sOff;                  /* 2nd */
        s32 rc = 12 * sOff;                 /* 3rd */
        s32 rowAdv = 32 * bOff;             /* dst advance per outer iter */
        s32 colAdv = 4 * (3 * sOff + bOff); /* chroma base advance */

        s32 oj2;
        for (oj2 = 0; oj2 < oCnt2; oj2++) {
            u8* b1 = (u8*)cb;
            u8* b2 = (u8*)b1 + ra;
            u8* b3 = (u8*)b1 + rb;
            u8* b4 = (u8*)b1 + rc;
            u8* c1 = (u8*)cr;
            u8* c2 = (u8*)c1 + ra;
            u8* c3 = (u8*)c1 + rb;
            u8* c4 = (u8*)c1 + rc;
            u8* dp = (u8*)d2;
            s32 k;
            for (k = 0; k < inCnt2; k++) {
                __dcbz(dp + 4, 4);
                u32 a = *(u32*)b1;
                u32 c0b = *(u32*)c1;
                dp += 4;
                b1 += 4;
                c1 += 4;
                *(u32*)dp = Y84_PACK0(a, c0b);
                dp += 4;
                *(u32*)dp = Y84_PACK1(a, c0b);
                b2 += 4;
                c2 += 4;
                a = *(u32*)(b2 - 4);
                c0b = *(u32*)(c2 - 4);
                dp += 4;
                *(u32*)dp = Y84_PACK0(a, c0b);
                dp += 4;
                *(u32*)dp = Y84_PACK1(a, c0b);
                b3 += 4;
                c3 += 4;
                a = *(u32*)(b3 - 4);
                c0b = *(u32*)(c3 - 4);
                dp += 4;
                *(u32*)dp = Y84_PACK0(a, c0b);
                dp += 4;
                *(u32*)dp = Y84_PACK1(a, c0b);
                b4 += 4;
                c4 += 4;
                a = *(u32*)(b4 - 4);
                c0b = *(u32*)(c4 - 4);
                dp += 4;
                *(u32*)dp = Y84_PACK0(a, c0b);
                dp += 4;
                *(u32*)dp = Y84_PACK1(a, c0b);
            }
            d2 = (u32*)((u8*)d2 + rowAdv);
            cb = (u32*)((u8*)cb + colAdv);
            cr = (u32*)((u8*)cr + colAdv);
        }
    }
}