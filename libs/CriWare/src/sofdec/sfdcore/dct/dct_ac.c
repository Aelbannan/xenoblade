#include <harness_catalog.h>
#include <math.h>

extern char *DCT_GetVerStr(void);
extern u32 lbl_eu_806046B8[];     /* BSS: [0]=verstr ptr, [1..0x40]=row table, [0x41..]=trans table */
extern double lbl_eu_8051C388[];  /* rodata: [0]=scale i==0, [1]=0.5, [2]=pi/8, [3]=itof bias */
extern double lbl_eu_8051C3A8;    /* rodata: IDCT rounding bias (0.0) */
extern double lbl_eu_806046C0[];  /* BSS: IDCT coefficient table */

/* Initialize DCT coefficient tables.
 * For each (i,j): val = scale * cos(pi/8 * i * (j + 0.5)).
 * Written to the row-major table and the transposed table.
 * int->double uses the itof reform (bias read from c[3]). */
void DCT_AcInit(void) {
    double *tbl_base = (double *)&lbl_eu_806046B8[1];  /* +0x08 */
    double *tbl_trans = tbl_base + 0x40;               /* +0x208 */
    int i, j;

    lbl_eu_806046B8[0] = (u32)DCT_GetVerStr();

    for (i = 0; i < 8; i++) {
        double scale = (i == 0) ? lbl_eu_8051C388[0] : lbl_eu_8051C388[1];
        double pi8 = lbl_eu_8051C388[2];
        double half = lbl_eu_8051C388[1];
        double *row = tbl_base + i * 8;
        double *col = tbl_trans + i;

        for (j = 0; j < 8; j++) {
            double v = cos(pi8 * (double)i * (half + (double)j)) * scale;
            *row++ = v;
            *col = v;
            col += 8;
        }
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