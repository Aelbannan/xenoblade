// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/dct/dct_isr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
// Retail .rodata 0x8051C3B0-0x8051C430 (0x80 B): 16 doubles (first 8 cos, 2 tiny denormals, 0.5, 131072, 2097152, magic, 4194304, 0)
// Use raw bytes to avoid literal range issues for the denormals.
__declspec(section ".rodata") __attribute__((aligned(8))) const unsigned char lbl_eu_8051C3B0[0x80] = {
    0x3F,0xD6,0xA0,0x9E,0x66,0x7F,0x3B,0xCD,0x3F,0xDF,0x62,0x97,0xCF,0xF7,0x5C,0xB0,
    0x3F,0xDD,0x90,0x6B,0xCF,0x32,0x8D,0x46,0x3F,0xDA,0x9B,0x66,0x29,0x0E,0xA1,0xA3,
    0x3F,0xD6,0xA0,0x9E,0x66,0x7F,0x3B,0xCD,0x3F,0xD1,0xC7,0x3B,0x39,0xAE,0x68,0xC8,
    0x3F,0xC8,0x7D,0xE2,0xA6,0xAE,0xA9,0x63,0x3F,0xB8,0xF8,0xB8,0x3C,0x69,0xA6,0x0B,
    0x00,0x00,0x00,0x0B,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,
    0x3F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x41,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x43,0x30,0x00,0x00,0x80,0x00,0x00,0x00,
    0x41,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
__attribute__((aligned(8))) u32 lbl_eu_80604AC0[130];
__attribute__((aligned(8))) s16 lbl_eu_80604CC8[128];
__attribute__((aligned(8))) s16 lbl_eu_80604DC8[4096];

#include <math.h>


extern char *DCT_GetVerStr(void);
extern void initSparseTbl(void);
extern void DCT_AcInit(void);
extern void DCT_AcIdctDouble(const double *in, double *out);

/* Initialize DCT ISR coefficient tables (8x8 cos matrix scaled to s16). */
void DCT_IsrInit(void) {
    double *cos_tbl = (double*)lbl_eu_8051C3B0;
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

    double *cos = (double*)lbl_eu_8051C3B0;
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
