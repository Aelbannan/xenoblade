#include <harness_catalog.h>

extern u32 lbl_eu_8051C2A8[4];

/* Init motion compensation table from global */
void MPVMC08_Init(u32 *dst) {
    const u32 *src = lbl_eu_8051C2A8;
    u32 v0 = src[0];
    u32 v1 = src[1];
    u32 v2 = src[2];
    u32 v3 = src[3];
    dst[0] = v0;
    dst[1] = v1;
    dst[2] = v2;
    dst[3] = v3;
}

/* --- Helper: average two byte-aligned 8-byte rows --- */
static void avg8(u8 *dst, const u8 *a, const u8 *b) {
    int i;
    for (i = 0; i < 8; i++)
        dst[i] = (u8)(((u16)a[i] + (u16)b[i]) >> 1);
}

/* --- Helper: average two 8-byte rows with 1-byte offset (half-pel horizontal) --- */
static void avg8_h1(u8 *dst, const u8 *src, int off) {
    int i;
    for (i = 0; i < 8; i++)
        dst[i] = (u8)(((u16)src[i] + (u16)src[i + off]) >> 1);
}

/* --- Helper: average two 8-byte rows from different pointers (half-pel vertical) --- */
static void avg8_v(u8 *dst, const u8 *src, int stride) {
    int i;
    for (i = 0; i < 8; i++)
        dst[i] = (u8)(((u16)src[i] + (u16)src[i + stride]) >> 1);
}

/* --- Helper: bilinear (4-pixel average) for quarter-pel --- */
static u8 avg4(u8 a, u8 b, u8 c, u8 d) {
    return (u8)(((u16)a + (u16)b + (u16)c + (u16)d + 2) >> 2);
}

/* One-ref 1-pixel (full-pel copy + half-pel horizontal variants) */
void MPVMC08_OneRef1p_TuneC(u32 *params) {
    int mode = params[9] & 7;  /* params[0x24/4] & 7 */
    int stride = params[8];     /* params[0x20/4] */
    u8 *src = (u8 *)(u32)params[9];  /* params[0x24/4] -- wait, need to re-check */
    
    /* Actually need to re-analyze struct layout */
}

/* Let me just implement the simplest one first */
void MPVMC08_OneRef4p_TuneC(u32 *params);
void MPVMC08_OneRefV2_TuneC(u32 *params);
void MPVMC08_OneRefH2_TuneC(u32 *params);
