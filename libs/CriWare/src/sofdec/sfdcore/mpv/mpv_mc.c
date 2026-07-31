// CriWare Sofdec MPEG-2 motion compensation (8-bit).

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

/* Motion compensation params: 0x18 = dst, 0x20 = stride, 0x24/0x28 = srcs */
typedef struct MPC08Params_ {
    u8 pad[0x18];
    u8 *dst;        /* 0x18 */
    u8 pad2[0x20 - 0x1c];
    u32 stride;     /* 0x20 */
    u8 *src1;       /* 0x24 */
    u8 *src2;       /* 0x28 */
} MPC08Params;

/* Quarter-pel one-ref compensation: 8x8 block from two ref rows */
void MPVMC08_OneRef4p_TuneC(MPC08Params *prm) {
    u8 *s1 = prm->src1;
    u8 *s2 = prm->src2;
    u8 *dst = prm->dst;
    u32 stride = prm->stride;
    int i;

    for (i = 0; i < 8; i++) {
        u32 v0 = ((u32)(s1[0] + s2[0] + s1[1] + s2[1] + 2) >> 2) << 24;
        u32 v1 = ((u32)(s1[1] + s2[1] + s1[2] + s2[2] + 2) >> 2) << 16;
        u32 v2 = ((u32)(s1[2] + s2[2] + s1[3] + s2[3] + 2) >> 2) << 8;
        u32 v3 = ((u32)(s1[3] + s2[3] + s1[4] + s2[4] + 2) >> 2);
        u32 v4 = ((u32)(s1[4] + s2[4] + s1[5] + s2[5] + 2) >> 2) << 24;
        u32 v5 = ((u32)(s1[5] + s2[5] + s1[6] + s2[6] + 2) >> 2) << 16;
        u32 v6 = ((u32)(s1[6] + s2[6] + s1[7] + s2[7] + 2) >> 2) << 8;
        u32 v7 = ((u32)(s1[7] + s2[7] + s1[8] + s2[8] + 2) >> 2);

        *(u32 *)&dst[0] = v0 | v1 | v2 | v3;
        *(u32 *)&dst[4] = v4 | v5 | v6 | v7;
        s1 += stride;
        s2 += stride;
        dst += 8;
    }
}

void MPVMC08_OneRef1p_TuneC(void);
void MPVMC08_OneRefV2_TuneC(void);
void MPVMC08_OneRefH2_TuneC(void);
