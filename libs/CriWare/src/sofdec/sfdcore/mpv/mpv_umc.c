// CriWare Sofdec MPEG-2 motion compensation dispatch.
// One-reference / output-frame setup for the UMC (unrestricted MC) path.

#include <harness_catalog.h>

extern void MPVMC08_OneRef1p_TuneC(void);
extern void MPVMC08_OneRefV2_TuneC(void);
extern void MPVMC08_OneRefH2_TuneC(void);
extern void MPVMC08_OneRef4p_TuneC(void);
extern void MPVMC16_OneRef1p_TuneC(void);
extern void MPVMC16_OneRefV2_TuneC(void);
extern void MPVMC16_OneRefH2_TuneC(void);
extern void MPVMC16_OneRef4p_TuneC(void);

extern u32 lbl_eu_80604668[];
extern u32 lbl_eu_80604688[];

/* MPV UMC context (fields used by InitOutRfb) */
typedef struct MPVUMC_ {
    u8 pad0[0xb5c];
    s32 mbw;        /* 0xb5c */
    s32 mbh;        /* 0xb60 */
    u8 pad1[0xc10 - 0xb64];
    u32 rf0;        /* 0xc10 */
    u32 rf1;        /* 0xc14 */
    u32 rf2;        /* 0xc18 */
    s16 width;      /* 0xc1c */
    s16 height;     /* 0xc1e */
    u32 rf3;        /* 0xc20 */
    u8 pad2[0xc34 - 0xc24];
    u32 t0;         /* 0xc34 */
    u32 t1;         /* 0xc38 */
    u32 t2;         /* 0xc3c */
    s16 t3;         /* 0xc40 */
    s16 t4;         /* 0xc42 */
} MPVUMC;

void mpvumc_InitOneRef(void);

void MPVUMC_Init(void) { mpvumc_InitOneRef(); }

void MPVUMC_Finish() {}

/* Install the one-reference motion compensation function tables */
void mpvumc_InitOneRef(void) {
    u32 *t08 = lbl_eu_80604668;
    u32 *t16 = lbl_eu_80604688;

    t08[0] = (u32)MPVMC08_OneRef1p_TuneC;
    t08[1] = (u32)MPVMC08_OneRefH2_TuneC;
    t08[2] = (u32)MPVMC08_OneRefV2_TuneC;
    t08[3] = (u32)MPVMC08_OneRef4p_TuneC;
    t08[4] = (u32)MPVMC08_OneRef1p_TuneC;
    t08[5] = (u32)MPVMC08_OneRefH2_TuneC;
    t08[6] = (u32)MPVMC08_OneRefV2_TuneC;
    t08[7] = (u32)MPVMC08_OneRefV2_TuneC;

    t16[0] = (u32)MPVMC16_OneRef1p_TuneC;
    t16[1] = (u32)MPVMC16_OneRefH2_TuneC;
    t16[2] = (u32)MPVMC16_OneRefV2_TuneC;
    t16[3] = (u32)MPVMC16_OneRef4p_TuneC;
    t16[4] = (u32)MPVMC16_OneRef1p_TuneC;
    t16[5] = (u32)MPVMC16_OneRefH2_TuneC;
    t16[6] = (u32)MPVMC16_OneRefV2_TuneC;
    t16[7] = (u32)MPVMC16_OneRefV2_TuneC;
}

/* Validate and set up the output frame buffers (with rounded fallback) */
void MPVUMC_InitOutRfb(MPVUMC *ctx) {
    u32 frm0 = ctx->rf2;
    u32 frm1 = ctx->rf3;
    s32 mbw = ctx->mbw;
    s32 mbh = ctx->mbh;
    u32 x;
    u32 y;

    if (frm0 != 0 && (x = ctx->rf0) != 0 && (y = ctx->rf1) != 0 &&
        (frm0 & 0x1f) == 0 && (x & 0x1f) == 0 && (y & 0x1f) == 0 &&
        frm0 != x && frm0 != y && x != y) {
        if (ctx->height > 0 && ctx->width > 0 && ctx->height <= 0x4000 &&
            ctx->width <= 0x4000 && (ctx->height & 0x1f) == 0 &&
            (ctx->width & 0x1f) == 0) {
            ctx->t0 = x;
            ctx->t1 = y;
            ctx->t2 = frm0;
            *(u32 *)&ctx->t3 = *(u32 *)&ctx->width;
            return;
        }
    }

    ctx->t2 = frm1;
    {
        s32 w1 = (mbw + 15) / 16;
        s32 w2 = ((w1 << 4) + 31) / 32;
        s32 w3 = ((w1 >> 4) & 1) + (w1 << 4);
        s32 w4 = ((w3 >> 1) + 31) / 32;
        s32 h1 = (mbh + 15) / 16;
        s32 h3 = ((h1 >> 4) & 1) + (h1 << 4);
        s32 h4 = h3 >> 1;

        ctx->t4 = (s16)(w2 << 5);
        ctx->t3 = (s16)(w4 << 5);
        ctx->t0 = frm1 + (h1 << 4) * (w2 << 5);
        ctx->t1 = ctx->t0 + h4 * (w4 << 5);
    }
}

void MPVUMC_EndOfFrame() {}

void fn_803AFDB0() {}

void fn_803AFFB4() {}

void fn_803B00B8() {}

void fn_803B01BC() {}

void mpvumc_OneReadMb() {}

void mpvumc_OneMakeMb() {}

void mpvumc_BiMakeMb() {}

void MPVUMC_PpicSkipped() {}

void mpvumc_PpicSkipMb() {}

void MPVUMC_BpicSkipped() {}
