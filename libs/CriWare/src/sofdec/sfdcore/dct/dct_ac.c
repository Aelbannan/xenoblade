#include <harness_catalog.h>
#include <math.h>

extern char *DCT_GetVerStr(void);

static char *dct_ac_ver;            // name-guess: version string pointer kept by DCT_AcInit
static double dct_ac_tbl[8][8];     // name-guess: 8x8 IDCT basis table
static double dct_ac_tbl_t[8][8];   // name-guess: transposed basis table

/* Initialize DCT coefficient tables: tbl[i][j] = C(i) * cos(pi/8 * i * (j + 0.5)). */
void DCT_AcInit(void) {
    double scale;
    double v;
    int i, j;

    dct_ac_ver = DCT_GetVerStr();

    for (i = 0; i < 8; i++) {
        if (i == 0)
            scale = 0.3535533905932738;
        else
            scale = 0.5;
        for (j = 0; j < 8; j++) {
            v = scale * cos(0.39269908169872414 * i * (0.5 + j));
            dct_ac_tbl[i][j] = v;
            dct_ac_tbl_t[j][i] = v;
        }
    }
}

/* 2D IDCT: transform 8x8 freq block (in) to spatial block (out). */
void DCT_AcIdctDouble(const double *in, double *out) {
    double tmp[8][8];
    double (*c)[8] = dct_ac_tbl;
    double s;
    int i, j, k;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            s = 0.0;
            for (k = 0; k < 8; k++)
                s += c[k][j] * in[i * 8 + k];
            tmp[i][j] = s;
        }
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            s = 0.0;
            for (k = 0; k < 8; k++)
                s += c[k][j] * tmp[k][i];
            out[j * 8 + i] = s;
        }
    }
}
