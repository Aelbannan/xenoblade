#include <harness_catalog.h>
#include <math.h>

extern int DCT_GetVerStr(void);
extern u32 lbl_eu_806046B8[];
extern double lbl_eu_8051C388[];
extern double lbl_eu_8051C3A8;
extern double lbl_eu_806046C0[8][8];

/* Precomputed cosine table for DCT */
/* [0] = scale for i==0, [1] = scale for i>0, [2] = 0.5, [3] = pi/8, [4] = int-to-double bias */

/* Initialize DCT coefficient tables */
void DCT_AcInit(void) {
    double *tbl_base = (double *)&lbl_eu_806046B8[2]; /* +0x08 */
    double *tbl_trans = (double *)&lbl_eu_806046B8[2] + 0x40; /* +0x208 */
    int i, j;

    /* Store version string */
    lbl_eu_806046B8[0] = (u32)DCT_GetVerStr();

    const double *c = (const double *)lbl_eu_8051C388;
    double pi8 = c[2];  /* pi/8 */
    double half = c[1]; /* 0.5 */

    for (i = 0; i < 8; i++) {
        double scale = (i != 0) ? c[1] : c[0];
        double *row = tbl_base + i * 8;
        double *col = tbl_trans + i;

        for (j = 0; j < 8; j++) {
            double arg = pi8 * (double)i;      /* pi/8 * i */
            double offset = half + (double)j;  /* 0.5 + j */
            double val = cos(arg * offset);    /* cos(pi/8 * i * (j+0.5)) */
            val *= scale;

            row[j] = val;
            col[j * 8] = val;
        }
    }
}

/* 2D IDCT: transform 8x8 block from freq domain to spatial domain
 * Row pass: tmp[i][j] = bias + sum_k tbl[k][j] * in[i][k]
 * Column pass: out[i][j] = bias + sum_k tmp[i][k] * tbl[j][k] */
void DCT_AcIdctDouble(const double *in, double *out) {
    double tmp[8][8];
    int i, j, k;

    /* Row IDCT: transform each row of input */
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            double sum = lbl_eu_8051C3A8; /* rounding bias (0.0) */
            for (k = 0; k < 8; k++) {
                sum += lbl_eu_806046C0[k][j] * in[i * 8 + k];
            }
            tmp[i][j] = sum;
        }
    }

    /* Column IDCT: transform each column of tmp to output */
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            double sum = lbl_eu_8051C3A8; /* rounding bias (0.0) */
            for (k = 0; k < 8; k++) {
                sum += lbl_eu_806046C0[k][j] * tmp[k][i];
            }
            out[i * 8 + j] = sum;
        }
    }
}
