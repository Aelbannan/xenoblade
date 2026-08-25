#include <harness_catalog.h>
#include <string.h>

/* MPEG video bitstream decoder context (retail offsets).
   The "vlc" handle passed to MPVBDEC_Init / MPVBDEC_StartFrame points
   into this block (allocated by MPV_Init inside the Sofdec work area). */
typedef struct MPVBDEC_CTX {
    u8 field_0x0000[0x1100];  /* 0x0000 */
    u32 dct_table[8];         /* 0x1100 reference DCT tables (32 bytes) */
    u8 block_tbl[0x40];       /* 0x1120 permuted block-position table (64 bytes) */
    u8 field_0x1160[0x80];    /* 0x1160 */
    u8 clip_tbl[0x20];        /* 0x11E0 clip table (32 bytes) */
    u32 vlc_param[12];        /* 0x1200 six (table ptr, size) pairs */
} MPVBDEC_CTX;

/* Sofdec work area laid out at lbl_eu_80602A10. */
typedef struct MPVBDEC_WORK {
    u8 field_0x00[0x48];
    u32 p_block_tbl;          /* 0x48 -> ctx->block_tbl */
    u8 field_0x4C[0xC];
    u32 p_self;               /* 0x54 -> work area itself */
} MPVBDEC_WORK;

extern int UTY_MemcpyDword(u32 *, const u32 *, int);
extern void MPVABDEC_Init(void);

extern u32 lbl_eu_8051BFC0[];  /* reference tables */
extern u32 lbl_eu_80602A10[];  /* work area */
extern u32 lbl_eu_80604660[];
extern u32 lbl_eu_8060465C[];
extern u32 lbl_eu_80604658[];
extern u32 lbl_eu_80604654[];
extern u32 lbl_eu_80604650[];
extern u32 lbl_eu_8060464C[];

extern u32 lbl_eu_806046A8[];
extern u32 lbl_eu_806033F8[];
extern u32 lbl_eu_80603480[];
extern u32 lbl_eu_80604644[];
extern u32 lbl_eu_80604648[];

/* Initialize MPEG video bitstream decoder.
   Builds the 64-byte permuted block-position table at ctx+0x1120 and the
   companion work table at 0x80602A10, then loads the static DCT/VLC
   reference tables and decoder parameters. */
void MPVBDEC_Init(MPVBDEC_CTX *ctx) {
    u8 table[0x40];
    u8 *tbl = (u8 *)lbl_eu_8051BFC0;     /* reference tables (live across calls) */
    MPVBDEC_WORK *work = (MPVBDEC_WORK *)lbl_eu_80602A10;
    int i, j;
    int k;   /* identity-fill cursor; dead after the fill loop */
    u8 *pp;
    u8 *vv;
    u8 *tt;

    /* Identity table consumed by the permutation loop below. */
    for (k = 0; k < 0x40; k++)
        table[k] = (u8)k;

    /* Permute: block_tbl[k] = table[perm[k]]; work[table[k]] = val[k].
       Table indices are signed bytes (retail sign-extends before lbzx/stbx). */
    pp = tbl + 0x20;
    vv = tbl + 0x60;
    tt = &table[0];

    for (j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            /* Lookup uses the FIXED table base with the raw perm byte;
               tt only walks the table to read its own entries. */
            ctx->block_tbl[j * 8 + i] = table[(s8)pp[i]];
            work->field_0x00[(s8)tt[i]] = vv[i];
        }
        pp += 8;
        vv += 8;
        tt += 8;
    }

    /* Publish work-area back-pointers: (work+0x40)[2] = &block_tbl, (work+0x4C)[2] = work */
    {
        u32 *work2 = (u32 *)work + 0x10;     /* +0x40 */
        u32 *work3 = (u32 *)work + 0x13;     /* +0x4C */
        work2[2] = (u32)&ctx->block_tbl[0];
        work3[2] = (u32)work;
    }

    /* Copy static reference tables into the context. */
    UTY_MemcpyDword(ctx->dct_table, (u32 *)tbl, 8);
    memcpy(ctx->clip_tbl, tbl + 0xa0, 0x20);

    /* Decoder parameter pairs: (table pointer loaded from data, table size). */
    ctx->vlc_param[0] = lbl_eu_80604660[0] - 0x10;
    ctx->vlc_param[1] = 0x15;
    ctx->vlc_param[2] = lbl_eu_8060465C[0] - 0x20;
    ctx->vlc_param[3] = 0x13;
    ctx->vlc_param[4] = lbl_eu_80604658[0] - 0x20;
    ctx->vlc_param[5] = 0x12;
    ctx->vlc_param[6] = lbl_eu_80604654[0] - 0x20;
    ctx->vlc_param[7] = 0x11;
    ctx->vlc_param[8] = lbl_eu_80604650[0] - 0x20;
    ctx->vlc_param[9] = 0x10;
    ctx->vlc_param[10] = lbl_eu_8060464C[0] - 0x20;
    ctx->vlc_param[11] = 0x0f;
}

/* Start decoding a frame */
void MPVBDEC_StartFrame(void *handle) {
    u32 *p = (u32 *)handle;
    s32 flag = (s32)p[0xD00 / 4];
    u32 *tbl = (u32 *)lbl_eu_806046A8;
    u32 *param_tbl = (u32 *)tbl[2];  /* tbl[2] = ptr to param array */

    if ((s32)flag == 8) {
        /* progressive frame - skip field setup */
    } else {
        /* interlaced - select field parameters */
        if (p[0xD38 / 4] == 0) {
            p[0xD3C / 4] = param_tbl[0];
        } else {
            p[0xD3C / 4] = param_tbl[1];
        }
        p[0xD40 / 4] = param_tbl[2];
        p[0xD44 / 4] = 0;
    }

    /* Set decode parameters */
    if (p[0xD38 / 4] == 0) {
        p[0xD4C / 4] = lbl_eu_806033F8[0];
        p[0xD50 / 4] = lbl_eu_80603480[0];
    } else {
        p[0xD4C / 4] = lbl_eu_80604644[0];
        p[0xD50 / 4] = lbl_eu_80604648[0];
    }
}
