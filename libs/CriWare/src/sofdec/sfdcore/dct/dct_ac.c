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
 * int->double conversions use the itof reform (bias read from c[3]). */
void DCT_AcInit(void) {
    u32 *b8 = lbl_eu_806046B8;
    const double *c = (const double *)lbl_eu_8051C388;
    int i, j;

    b8[0] = (u32)DCT_GetVerStr();

    double *rowBase = (double *)(b8 + 2);             /* +0x08 */
    double bias = c[3];   /* itof bias const (0x4330000080000000) */
    double *trans = (double *)((char *)b8 + 0x208);   /* +0x208 */
    double pi8 = c[2];    /* pi/8 */
    double half = c[1];   /* 0.5 */
    u32 transCol = 0;

    for (i = 0; i < 8; i++) {
        double scale = (i == 0) ? c[0] : c[1];
        double *row = rowBase;
        double *col = (double *)((char *)trans + transCol);
        u32 iv = (u32)i;

        for (j = 0; j < 8; j++) {
            /* itof: int -> double via 0x4330 magic sweep, bias pulled from c[3]. */
            union { f64 d; u32 u[2]; } cvt;
            cvt.u[0] = 0x43300000;
            cvt.u[1] = iv ^ 0x80000000;
            double id = cvt.d - bias;
            cvt.u[0] = 0x43300000;
            cvt.u[1] = (u32)j ^ 0x80000000;
            double jd = cvt.d - bias;
            double v = scale * cos(pi8 * id * (half + jd));
            *row++ = v;
            *col = v;
            col += 8;
        }
        rowBase += 8;
        transCol += 8;
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