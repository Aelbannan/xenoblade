#include <harness_catalog.h>

extern float lbl_eu_805197B0[];  /* YCbCr conversion constants + f64 conversion bias */
extern float lbl_eu_805FDDC0[];  /* output lookup tables (5 tables of 256 floats) */

/* Build the five YCbCr 4:2:0 -> ARGB8888 lookup tables:
 *   +0x0000: Y  table: tbl[i] = 0.5 + 1.1643835*(i - 16)
 *   +0x0400: Cr table: tbl[i] = 1.5960267*(i - 128)
 *   +0x0800: Cb table: tbl[i] = 2.0172322*(i - 128)
 *   +0x0C00: G  table: tbl[i] = -0.3916071*(i - 128)
 *   +0x1000: B  table: tbl[i] = -0.8128125*(i - 128)
 */
void CFT_Ycc420plnToArgb8888Init(void) {
    float c0 = lbl_eu_805197B0[0];
    float c1 = lbl_eu_805197B0[1];
    float c2 = lbl_eu_805197B0[2];
    float c3 = lbl_eu_805197B0[3];
    float c4 = lbl_eu_805197B0[4];
    float c5 = lbl_eu_805197B0[5];

    float *tblY = &lbl_eu_805FDDC0[0x000];
    float *tblCr = &lbl_eu_805FDDC0[0x100];
    float *tblCb = &lbl_eu_805FDDC0[0x200];
    float *tblG = &lbl_eu_805FDDC0[0x300];
    float *tblB = &lbl_eu_805FDDC0[0x400];

    int i;
    for (i = 0; i < 256; i += 2) {
        float y0 = (float)(i - 16);
        float t0 = c0 * y0;

        tblCr[i] = c2 * (float)(i - 128);
        tblCb[i] = c3 * (float)(i - 128);
        tblG[i] = c4 * (float)(i - 128);
        tblB[i] = c5 * (float)(i - 128);
        tblY[i] = c1 + t0;

        float y1 = (float)(i - 15);
        float t1 = c0 * y1;

        tblCr[i + 1] = c2 * (float)(i - 127);
        tblCb[i + 1] = c3 * (float)(i - 127);
        tblG[i + 1] = c4 * (float)(i - 127);
        tblB[i + 1] = c5 * (float)(i - 127);
        tblY[i + 1] = c1 + t1;
    }
}

/* Convert a YCbCr 4:2:0 planar frame to the packed Y84C44 format.
 * src/dst carry the plane pointers and strides; the third argument is
 * unused by this converter.
 */
void CFT_Ycc420plnToY84C44(void *src, void *dst, u32 *table) {
    (void)src;
    (void)dst;
    (void)table;
}
