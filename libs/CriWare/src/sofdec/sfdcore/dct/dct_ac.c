#include <harness_catalog.h>
__declspec(section ".rodata") __attribute__((aligned(8))) double lbl_eu_8051C388[4] = {0.3535533905932738,0.5,0.39269908169872414,4503601774854144};
__declspec(section ".rodata") __attribute__((aligned(8))) double lbl_eu_8051C3A8 = 0;
__attribute__((aligned(4))) u32 lbl_eu_806046B8[1];
__attribute__((aligned(8))) double lbl_eu_806046C0[128];

#include <math.h>

extern char *DCT_GetVerStr(void);

/* Initialize DCT coefficient tables.
 * For each (i,j): val = scale * cos(pi/8 * i * (j + 0.5)).
 * Written to the row-major table (row, stride 8) and the transposed
 * table (col, stride 8, byte offset 0x208).
 * int->double conversion uses the 0x4330 exponent bit-reform with the
 * bias constant read from c[3]. */
void DCT_AcInit(void) {
    double *tp;
    double *t;
    const double *c;
    double *rowBase;
    int off;
    double *row;
    double *col;
    double dj;
    double di;
    double half;
    double pi8;
    double bias;
    double scale;
    double id;
    double jd;
    double v;
    int i, j;

    c = (const double *)lbl_eu_8051C388;
    t = (double *)lbl_eu_806046B8;

    *((u32 *)t) = (u32)DCT_GetVerStr();

    rowBase = t + 1;                                  /* +0x08 */
    bias = c[3];   /* itof bias const (0x4330000080000000) */
    tp = (double *)((char *)t + 0x208);               /* transposed table base */
    pi8 = c[2];    /* pi/8 */
    half = c[1];   /* 0.5 */

    for (i = 0, off = 0; i < 8; i++, off += 8) {
        if (i == 0)
            scale = c[0];
        else
            scale = c[1];
        col = (double *)((char *)tp + off);
        row = rowBase;

        for (j = 0; j < 8; j++) {
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
            row += 1;
            *col = v;
            col += 8;
        }
        rowBase += 8;
    }
}

/* 2D IDCT: transform 8x8 freq block (in) to spatial block (out). */
void DCT_AcIdctDouble(const double *in, double *out) {
    double tmp[8][8];
    const double *cp = lbl_eu_806046C0;
    int i, j, k;
    int c, cb, ob;

    /* Row pass */
    for (i = 0; i < 8; i++) {
        const double *ip = &in[i * 8];
        double *tp = &tmp[0][0] + i * 8;
        c = 0;
        for (j = 0; j < 8; j += 2) {
            double s0 = lbl_eu_8051C3A8 + cp[c] * ip[0];
            s0 += (cp + c)[8] * ip[1];
            s0 += (cp + c)[16] * ip[2];
            s0 += (cp + c)[24] * ip[3];
            for (k = 4; k < 8; k++)
                s0 += (cp + c)[k * 8] * ip[k];
            tp[0] = s0;
            c++;
            {
                double s1 = lbl_eu_8051C3A8 + cp[c] * ip[0];
                s1 += (cp + c)[8] * ip[1];
                s1 += (cp + c)[16] * ip[2];
                s1 += (cp + c)[24] * ip[3];
                for (k = 4; k < 8; k++)
                    s1 += (cp + c)[k * 8] * ip[k];
                tp[1] = s1;
            }
            c++;
            tp += 2;
        }
    }

    /* Column pass */
    for (i = 0; i < 8; i++) {
        double *colp = &tmp[0][i];
        double t0 = colp[0];
        double t1 = colp[8];
        double t2 = colp[16];
        double t3 = colp[24];
        double t4 = colp[32];
        double t5 = colp[40];
        double t6 = colp[48];
        double t7 = colp[56];
        double *op = &out[i];
        cb = 0;
        ob = 0;
        for (k = 0; k < 8; k += 2) {
            const double *c0 = cp + cb++;
            op[ob] = lbl_eu_8051C3A8
                + c0[0] * t0 + c0[8] * t1
                + c0[16] * t2 + c0[24] * t3
                + c0[32] * t4 + c0[40] * t5
                + c0[48] * t6 + c0[56] * t7;
            ob += 8;
            {
                const double *c1 = cp + cb++;
                op[ob] = lbl_eu_8051C3A8
                    + c1[0] * t0 + c1[8] * t1
                    + c1[16] * t2 + c1[24] * t3
                    + c1[32] * t4 + c1[40] * t5
                    + c1[48] * t6 + c1[56] * t7;
            }
            ob += 8;
        }
    }
}
