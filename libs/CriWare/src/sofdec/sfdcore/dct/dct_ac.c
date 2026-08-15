#include <harness_catalog.h>
#include <math.h>

extern char *DCT_GetVerStr(void);
extern u32 lbl_eu_806046B8[];     /* BSS: [0]=verstr ptr, [1..0x40]=row table, [0x41..]=trans table */
extern double lbl_eu_8051C388[];  /* rodata: [0]=scale i==0, [1]=0.5, [2]=pi/8, [3]=itof bias */
extern double lbl_eu_8051C3A8;    /* rodata: IDCT rounding bias (0.0) */
extern double lbl_eu_806046C0[];  /* BSS: IDCT coefficient table */

/* Initialize DCT coefficient tables.
 * For each (i,j): val = scale * cos(pi/8 * i * (j + 0.5)).
 * Written to the row-major table (row, stride 8) and the transposed
 * table (col, stride 8, byte offset transCol).
 * int->double conversions use the itof bit-reform (bias read from c[3]). */
void DCT_AcInit(void) {
    const double *c = (const double *)lbl_eu_8051C388;
    double *t = (double *)lbl_eu_806046B8;
    double *rowBase;
    double *row;
    double *col;
    double bias;
    double pi8;
    double half;
    double scale;
    int i, j;
    union { double d; unsigned int w[2]; } ui, uj;

    *((u32 *)t) = (u32)DCT_GetVerStr();

    rowBase = t + 1;                                  /* +0x08 */
    bias = c[3];   /* itof bias const (0x4330000080000000) */
    t = (double *)((char *)t + 0x208);                /* reuse t as the transposed table */
    pi8 = c[2];    /* pi/8 */
    half = c[1];   /* 0.5 */

    for (i = 0; i < 8; i++) {
        scale = (i == 0) ? c[0] : c[1];
        row = rowBase;
        col = (double *)((char *)t + i * 8);
        unsigned int iv = (unsigned int)i ^ 0x80000000;

        for (j = 0; j < 8; j++) {
            /* itof: int -> double via 0x4330 magic sweep, bias pulled from c[3].
             * Low word written first (MWCC 7i manual-bit pattern). */
            ui.w[1] = iv;
            uj.w[1] = (unsigned int)j ^ 0x80000000;
            ui.w[0] = 0x43300000;
            double id = ui.d - bias;
            id *= pi8;                                  /* pi8*id in place */
            uj.w[0] = 0x43300000;
            double jd = uj.d - bias;
            double v = scale * cos(id * (half + jd));
            *row = v;
            row += 8;
            *col = v;
            col += 8;
        }
        rowBase += 8;
    }
}

/* 2D IDCT: transform 8x8 freq block (in) to spatial block (out). */
void DCT_AcIdctDouble(const double *in, double *out) {
    double tmp[8][8];
    const double *coeff = lbl_eu_806046C0;
    int i, j, k;

    /* Row pass */
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            double sum = lbl_eu_8051C3A8;
            for (k = 0; k < 8; k++) {
                sum += coeff[k * 8 + j] * in[i * 8 + k];
            }
            tmp[i][j] = sum;
        }
    }

    /* Column pass */
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            double sum = lbl_eu_8051C3A8;
            for (k = 0; k < 8; k++) {
                sum += coeff[i * 8 + k] * tmp[k][j];
            }
            out[i * 8 + j] = sum;
        }
    }
}
