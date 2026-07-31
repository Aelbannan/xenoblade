// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/dct/dct_isr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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

/* Initialize DCT ISR coefficient tables and sparse lookup */
void DCT_IsrInit(void) {
    double *cos_tbl = lbl_eu_8051C3B0;
    double *out_dbl = (double *)&lbl_eu_80604AC0[2];
    s16 *out_s1 = (s16 *)&lbl_eu_80604AC0[0x82]; /* lbl_eu_80604CC8 */
    s16 *out_s2 = (s16 *)&lbl_eu_80604AC0[0xA2]; /* lbl_eu_80604CC8 + 64 */
    double s1 = cos_tbl[11];   /* 131072.0 */
    double bias = cos_tbl[10]; /* 0.5 */
    double s2 = cos_tbl[12];   /* 2097152.0 */
    int i, j;

    lbl_eu_80604AC0[0] = (u32)DCT_GetVerStr();

    for (i = 0; i < 8; i++) {
        double row = cos_tbl[i];
        for (j = 0; j < 8; j++) {
            double p = row * cos_tbl[j];
            out_dbl[i * 8 + j] = p;
            out_s1[i * 8 + j] = (s16)(int)(s1 * p + bias);
            out_s2[i * 8 + j] = (s16)(int)(s2 * p + bias);
        }
    }

    initSparseTbl();
}

extern char lbl_eu_80604CC8[];
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

void initSparseTbl() {}

void MSID_JRevDctSparse() {}

void MSID_JRevDctDense() {}
