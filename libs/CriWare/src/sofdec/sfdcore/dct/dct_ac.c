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
 * table (col, stride 8, byte offset 0x208).
 * int->double conversion uses the 0x4330 exponent bit-reform with the
 * bias constant read from c[3]. */
void DCT_AcInit(void) {
    double *t = (double *)lbl_eu_806046B8;
    const double *c = (const double *)lbl_eu_8051C388;
    double *rowBase;
    double *row;
    double *col;
    double dj;
    double di;
    double scale;
    double half;
    double pi8;
    double bias;
    int i, j;

    *((u32 *)t) = (u32)DCT_GetVerStr();

    rowBase = t + 1;                                  /* +0x08 */
    bias = c[3];   /* itof bias const (0x4330000080000000) */
    t = (double *)((char *)t + 0x208);                /* reuse t as the transposed table */
    pi8 = c[2];    /* pi/8 */
    half = c[1];   /* 0.5 */


    for (i = 0; i < 8; i++) {
        scale = (i == 0) ? c[0] : c[1];
        col = (double *)((char *)t + i * 8);
        row = rowBase;

        for (j = 0; j < 8; j++) {
            double id;
            double jd;
            double v;
            /* itof: build the 0x4330-exponent double by hand, then subtract bias */
            ((u32 *)&di)[1] = (u32)i ^ 0x80000000;
            ((u32 *)&dj)[1] = (u32)j ^ 0x80000000;
            *(u32 *)&di = 0x43300000;
            id = di - bias;
            id = pi8 * id;
            *(u32 *)&dj = 0x43300000;
            jd = dj - bias;
            /* 0.5 comes from the const table; MWCC hoists the reload out of the loops */
            v = scale * cos(id * (half + jd));
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
