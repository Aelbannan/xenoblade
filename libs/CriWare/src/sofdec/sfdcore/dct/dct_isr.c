// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/dct/dct_isr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <math.h>

extern u32 lbl_eu_80604AC0[];       /* BSS, 0x208 bytes: [0]=verstr ptr, [2..]=8x8 doubles */
extern s16 lbl_eu_80604CC8[];       /* BSS, 0x100 bytes: first 64 s16 (scale1), next 64 s16 (scale2) */
extern double lbl_eu_8051C3B0[];    /* rodata, 0x80 bytes: [0..7]=cos coeffs, [10]=0.5, [11]=131072, [12]=2097152, [13]=bias */
extern s16 lbl_eu_80604DC8[];       /* BSS, 0x2000 bytes: sparse table */

extern char *DCT_GetVerStr(void);
extern void initSparseTbl(void);
extern void DCT_AcInit(void);
extern void DCT_AcIdctDouble(const double *in, double *out);

/* Initialize DCT ISR coefficient tables (8x8 cos matrix scaled to s16). */
void DCT_IsrInit(void) {
    double *cos_tbl = lbl_eu_8051C3B0;
    double s1 = cos_tbl[11];
    double bias = cos_tbl[10];
    double s2 = cos_tbl[12];
    int i, j;

    lbl_eu_80604AC0[0] = (u32)DCT_GetVerStr();

    for (i = 0; i < 8; i++) {
        double row = cos_tbl[i];
        double *pd = (double *)&lbl_eu_80604AC0[2] + i * 8;
        s16 *ps1 = (s16 *)&lbl_eu_80604AC0[0x82] + i * 8;
        s16 *ps2 = (s16 *)&lbl_eu_80604AC0[0xA2] + i * 8;
        double *pc = cos_tbl;
        for (j = 0; j < 8; j++) {
            double p = row * *pc++;
            *pd++ = p;
            /* floor(x + 0.5): round-to-nearest (MWCC inlines floor) */
            *ps1++ = (s16)floor(s1 * p + bias);
            *ps2++ = (s16)floor(s2 * p + bias);
        }
    }

    initSparseTbl();
}

void* memcpy(void* dst, const void* src, size_t n);
void DCT_IsrInitScaleTbl(void* dst) {
    memcpy(dst, lbl_eu_80604CC8, 0x80);
}

void MSID_JRevDctDense();
void MSID_JRevDctSparse();
void DCT_IsrTrans(void* self, s32 val) {
    if (val < 0) {
        ((void(*)(void*,u32))MSID_JRevDctDense)(self, -val);
    } else {
        ((void(*)(void*,u32))MSID_JRevDctSparse)(self, val);
    }
}

void initSparseTbl() {
    memset(lbl_eu_80604DC8, 0, 0x2000);
    DCT_AcInit();

    double *cos = lbl_eu_8051C3B0;
    double bias = cos[10];  /* 0.5 */
    double scale = cos[14]; /* 4194304.0 */
    double zero = cos[15];  /* 0.0 */

    for (int i = 0; i < 0x40; i++) {
        double tmp[64];
        s16 *row = lbl_eu_80604DC8 + i * 64;
        int col;

        for (col = 0; col < 0x40; col++) {
            if (col == i)
                tmp[col] = scale / (double)lbl_eu_80604CC8[i];
            else
                tmp[col] = zero;
        }

        {
            double out[64];
            DCT_AcIdctDouble(tmp, out);
            for (col = 0; col < 0x40; col++) {
                double v = out[col];
                if (v >= 0.0)
                    row[col] = (s16)(s32)(v + bias);
                else
                    row[col] = (s16)(s32)(-(bias - v));
            }
        }

        /* mirror the row: forward-copy to stack buffer, then write back reversed */
        {
            s16 buf[64];
            for (col = 0; col < 0x40; col++)
                buf[col] = row[col];
            for (col = 0; col < 0x40; col++)
                row[0x3F - col] = buf[col];
        }
    }
}

void MSID_JRevDctSparse() {}

void MSID_JRevDctDense() {}
