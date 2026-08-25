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
extern void MPVMC08_OneRefH2_TuneC(void);
extern void MPVMC08_OneRef4p_TuneC(void);
extern void MPVMC16_OneRef1p_TuneC(void);
extern void MPVMC16_OneRefV2_TuneC(void);
extern void MPVMC16_OneRefH2_TuneC(void);
extern void MPVMC16_OneRef4p_TuneC(void);

extern u32 lbl_eu_80604668[];
extern u32 lbl_eu_80604688[];

void mpvumc_OneReadMb(void* ctx, s32 arg, s32* sizes, u8* sub1, u8* sub2);
void mpvumc_OneMakeMb(void* umc, u8* rows, s32 v);

/* MPV UMC context (fields used by InitOutRfb) */
typedef struct MPVUMC_ {
    u8* field_0x0;      /* 0x0   - MC output table */
    s16* field_0x4;     /* 0x4   - MC index table */
    u8* field_0x8;      /* 0x8   - MC source (ref 0) */
    u8* field_0xc;      /* 0xc   - MC source (ref 1) */
    u8 pad0[0xa90 - 0x10];
    u8 onePad[0xa98 - 0xa90];   /* 0xa90 - OneReadMb sub-buffer */
    s32 rdArg0;                 /* 0xa98 */
    s32 rdArg1;                 /* 0xa9c */
    u8 rowsHdr[4];              /* 0xaa0 - MakeMb rows header */
    s32 out[11];                /* 0xaa4 - MakeMb rows: {base,stride} x5 + final base */
    u8 pad0b[0xb5c - 0xad0];
    s32 mbw;        /* 0xb5c */
    s32 mbh;        /* 0xb60 */
    s32 field_0xb64;    /* 0xb64 - macroblock row width */
    u8 pad1[0xbfc - 0xb68];
    s16 field_0xbfc;
    s16 field_0xbfe;
    u8 pad1b[0xc0c - 0xc00];
    s16 field_0xc0c;
    s16 field_0xc0e;
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
    u8 pad3[0xc74 - 0xc44];
    void (*field_0xc74)(struct MPVUMC_*); /* 0xc74 - skip-MB dispatcher */
    u8 pad4[0xcd8 - 0xc78];
    s32 field_0xcd8;    /* 0xcd8 - macroblock counter */
    s32 field_0xcdc;    /* 0xcdc - current column */
    s32 field_0xce0;    /* 0xce0 - current row */
    u8 pad5[0xcec - 0xce4];
    s32 field_0xcec;    /* 0xcec - MC mode flag */
    u8 pad6[0x1000 - 0xcf0];
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

/* Skipped-macroblock reconstruction: lay out the six output row bases from
 * the current column/row position, then rebuild each 8x8 block through the
 * quantiser table (cache-line zeroing when the destination is 32B aligned). */
void fn_803AFDB0(MPVUMC* ctx) {
    u8* tbl = *(u8**)((u8*)ctx + 0x9C0);
    s16* idx = (s16*)ctx;
    s32 col = ctx->field_0xcdc;
    s32 row = ctx->field_0xce0;
    s32 h = ctx->t4;
    s32 w = ctx->t3;
    s32 a = ctx->t2;
    s32 b = ctx->t0;
    s32 cc = ctx->t1;
    s32* p = ctx->out;
    s32 lum = col * 16 * h + a + row * 16;
    s32 chr = row * 8 + col * 8 * w;
    s32 n = 6;

    p[0] = b + chr;
    p[2] = cc + chr;
    p[4] = lum;
    p[6] = lum + 8;
    p[8] = lum + h * 8;
    p[10] = lum + h * 8 + 8;

    do {
        s32 dv = p[0];
        s32 stride = p[1];
        u8* dst = (u8*)dv;
        p += 2;
        if ((dv & 0x1F) == 0) {
            s32 i;
            for (i = 0; i < 8; i++) {
                s32 j;
                __dcbz(dst, stride);
                for (j = 0; j < 8; j++) {
                    dst[j] = tbl[idx[j]];
                }
                idx += 8;
                dst += stride;
            }
        } else {
            s32 i;
            for (i = 0; i < 4; i++) {
                s32 j;
                for (j = 0; j < 16; j++) {
                    dst[j] = tbl[idx[j]];
                }
                idx += 16;
                dst += stride;
            }
        }
    } while (--n != 0);
}

/* fn_803AFFB4: forward-mode macroblock - read it, then lay out the six
 * output row bases (luma + chroma, with 8-pixel offsets for half rows). */
void fn_803AFFB4(MPVUMC* ctx) {
    /* column/row in locals so each is loaded once and shifted twice */
    register s32 row = ctx->field_0xce0;
    register s32 col = ctx->field_0xcdc;
    s32 x8 = col * 8;
    s32 x16 = col * 16;
    s32 sizes[2];

    sizes[0] = x8 * ctx->field_0xbfc + row * 8;
    sizes[1] = x16 * ctx->field_0xbfe + row * 16;
    mpvumc_OneReadMb(ctx, ctx->rdArg0, sizes, (u8*)ctx + 0xbf0, (u8*)ctx + 0xc90);

    col = ctx->field_0xcdc;
    row = ctx->field_0xce0;
    x8 = col * 8;
    x16 = col * 16;
    /* reuse the sizes slots for the output row bases */
    sizes[0] = x8 * ctx->t3 + row * 8;
    sizes[1] = x16 * ctx->t4 + row * 16;
    {
        s32 base = ctx->t2 + sizes[1];
        s32 mid = base + ctx->t4 * 8;
        ctx->out[0] = ctx->t0 + sizes[0];
        ctx->out[2] = ctx->t1 + sizes[0];
        ctx->out[4] = base;
        ctx->out[6] = base + 8;
        ctx->out[8] = mid;
        ctx->out[10] = mid + 8;
    }
    mpvumc_OneMakeMb((u8*)ctx + 0xa90, (u8*)ctx + 0xaa0, ctx->field_0xcec);
}

/* fn_803B00B8: backward-mode variant of fn_803AFFB4 (different MV vectors
 * and second reference sub-buffers). */
void fn_803B00B8(MPVUMC* ctx) {
    /* Temporaries pinned so each column/row field is loaded exactly once per
     * call phase; all four shifted products are formed before each size sum. */
    s32 sizes[2];
    s32 col;
    s32 st8;
    s32 row;
    s32 y8;
    s32 x16;
    s32 y16;
    s32 t3;
    s32 v0;
    s32 t4;
    s32 t4x8;
    s32 v1;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 sz0;
    s32 sz1;
    s32 b;
    s32 mid;

    col = ctx->field_0xcdc;
    st8 = ctx->field_0xc0c;
    row = ctx->field_0xce0;
    x16 = col * 16;
    y8 = row * 8;
    y16 = row * 16;
    sizes[0] = col * 8 * st8 + y8;
    sizes[1] = x16 * ctx->field_0xc0e + y16;
    mpvumc_OneReadMb(ctx, ctx->rdArg0, sizes, (u8*)ctx + 0xc00, (u8*)ctx + 0xcb4);

    /* chroma bases derive from the out[4] value in-register */
    col = ctx->field_0xcdc;
    t3 = ctx->t3;
    row = ctx->field_0xce0;
    x16 = col * 16;
    y8 = row * 8;
    y16 = row * 16;
    v0 = col * 8 * t3 + y8;
    sizes[0] = v0;
    t4 = ctx->t4;
    t4x8 = t4 * 8;
    v1 = x16 * t4 + y16;
    sizes[1] = v1;
    t0 = ctx->t0;
    t1 = ctx->t1;
    ctx->out[0] = t0 + v0;
    t2 = ctx->t2;
    sz0 = sizes[0];
    ctx->out[2] = t1 + sz0;
    sz1 = sizes[1];
    b = t2 + sz1;
    ctx->out[4] = b;
    ctx->out[6] = b + 8;
    mid = b + t4x8;
    ctx->out[8] = mid;
    ctx->out[10] = mid + 8;
    mpvumc_OneMakeMb((u8*)ctx + 0xa90, (u8*)ctx + 0xaa0, ctx->field_0xcec);
}

void mpvumc_BiMakeMb(MPVUMC* ctx, s32* rows, s32 v);

/* fn_803B01BC: bidirectional macroblock - read both references, then make. */
void fn_803B01BC(MPVUMC* ctx) {
    s32 sizes[2];
    sizes[0] = ctx->field_0xcdc * 8 * ctx->field_0xbfc + ctx->field_0xce0 * 8;
    sizes[1] = ctx->field_0xcdc * 16 * ctx->field_0xbfe + ctx->field_0xce0 * 16;
    mpvumc_OneReadMb(ctx, ctx->rdArg0, sizes, (u8*)ctx + 0xbf0, (u8*)ctx + 0xc90);
    mpvumc_OneReadMb(ctx, ctx->rdArg1, sizes, (u8*)ctx + 0xc00, (u8*)ctx + 0xcb4);

    {
        s32 v0 = ctx->field_0xce0 * 8 + ctx->field_0xcdc * 8 * ctx->t3;
        s32 v1 = ctx->field_0xce0 * 16 + ctx->field_0xcdc * 16 * ctx->t4;
        ctx->out[0] = ctx->t0 + v0;
        ctx->out[2] = ctx->t1 + v0;
        ctx->out[4] = ctx->t2 + v1;
        ctx->out[6] = ctx->out[4] + 8;
        ctx->out[8] = ctx->t2 + v1 + ctx->t4 * 8;
        ctx->out[10] = ctx->out[8] + 8;
    }
    mpvumc_BiMakeMb(ctx, (s32*)((u8*)ctx + 0xaa0), ctx->field_0xcec);
}

/* One-reference macroblock read: picks the MC kernels for luma/chroma from
 * the dispatch tables (indexed by MB column and rounded MV half-pel bits),
 * computes the three block source offsets and drives the three kernel
 * calls (two luma blocks at +0/+64, one chroma pair at +128). */
void mpvumc_OneReadMb(void* ctx, s32 arg, s32* sizes, u8* sub1, u8* sub2) {
    const u32* t16 = lbl_eu_80604688;
    const u32* t08 = lbl_eu_80604668;
    u8* c = (u8*)ctx;
    s32* out = (s32*)(c + 0xA4C);
    s32 b = *(s32*)(sub2 + 24);
    s32 v = *(s32*)(sub2 + 28);
    s32 cc = *(s32*)((u8*)ctx + 0xB1C);
    /* round the raw vectors, then split off the interpolation-mode bits */
    s32 bx = b + ((b >> 30) & 1);
    s32 vx = v + ((v >> 30) & 1);
    u32 fn1 = t16[cc * 4 + ((vx >> 2) & 1) * 2 + ((bx >> 2) & 1)];
    u32 fn2 = t08[cc * 4 + ((vx >> 2) & 1) * 2 + ((bx >> 1) & 1)];
    s32 h1 = *(s16*)(sub1 + 12);
    s32 h2 = *(s16*)(sub1 + 14);
    s32 da = sizes[0] + (bx >> 2) + (vx >> 2) * h1;
    s32 db = sizes[1] + (b >> 1) + (v >> 1) * h2;
    s32 fl = ((bx >> 30) & 1) & cc;
    s32 d9;
    s32 d10;
    s32 d11;

    /* luma blocks: bases differ only in the sub-block origin word */
    d9 = *(s32*)sub1 + da;
    d10 = d9 + fl + h1;
    out[8] = h1;
    out[6] = arg;
    out[9] = d9;
    out[10] = d10;
    ((void (*)(void*, s32, s32, s32))fn2)(out, arg, d9, d10);

    d9 = *(s32*)(sub1 + 4) + da;
    d10 = d9 + fl + h1;
    out[6] = arg + 64;
    out[9] = d9;
    out[10] = d10;
    ((void (*)(void*, s32, s32, s32))fn2)(out, arg + 64, d9, d10);

    /* chroma: table 16 kernel, half-pel edge flag from the raw LSB */
    d11 = *(s32*)(sub1 + 8) + db;
    d10 = d11 + h2 + ((b & 1) & cc);
    out[6] = arg + 128;
    out[8] = h2;
    out[9] = d11;
    out[10] = d10;
    ((void (*)(void*, s32, s32, s32))fn1)(out, arg + 128, d11, d10);
}

/* Lay the six decoded blocks back over the frame: with v < 0 every pixel is
 * remapped through the quantiser table (idx[4k+j] selects the table bias for
 * byte j of word k), otherwise the source words are copied verbatim into
 * stride-aligned rows. */
void mpvumc_OneMakeMb(void* umc, u8* rows, s32 v) {
    u8* c = (u8*)umc;
    u8* src = *(u8**)(c + 8);
    const u8* tbl = *(const u8**)(c + 0);
    s16* idx = *(s16**)(c + 4);
    s32* pairs = (s32*)(rows + 4);
    s32 i;
    for (i = 0; i < 6; i++) {
        u8* d = (u8*)pairs[0];
        s32 stride = pairs[1];
        pairs += 2;
        if (v >= 0) {
            /* aligned copy: 8 rows of two words, stride rounded down */
            u32* dw = (u32*)d;
            const u32* sw = (const u32*)src;
            s32 st = (stride & ~7) >> 2;
            u32 a0 = sw[0];
            u32 a1 = sw[1];
            u32 a2 = sw[2];
            u32 a3 = sw[3];
            u32 a4 = sw[4];
            u32 a5 = sw[5];
            u32 a6 = sw[6];
            u32 a7 = sw[7];
            idx += 64;
            dw[0] = a0;
            dw[1] = a1;
            dw += st;
            dw[0] = a2;
            dw[1] = a3;
            dw += st;
            dw[0] = a4;
            dw[1] = a5;
            dw += st;
            dw[0] = a6;
            dw[1] = a7;
            dw += st;
            a0 = sw[8];
            a1 = sw[9];
            a2 = sw[10];
            a3 = sw[11];
            a4 = sw[12];
            a5 = sw[13];
            a6 = sw[14];
            a7 = sw[15];
            dw[0] = a0;
            dw[1] = a1;
            dw += st;
            dw[0] = a2;
            dw[1] = a3;
            dw += st;
            dw[0] = a4;
            dw[1] = a5;
            dw += st;
            dw[0] = a6;
            dw[1] = a7;
            src += 64;
        } else {
            s32 k;
            for (k = 0; k < 8; k++) {
                u32 w = *(u32*)src;
                u32 w2 = *(u32*)(src + 4);
                ((u32*)d)[0] = (u32)tbl[idx[0] + (w >> 24)] << 24
                             | (u32)tbl[idx[1] + (w & 0xFF)] << 16
                             | (u32)tbl[idx[2] + ((w >> 8) & 0xFF)] << 8
                             | (u32)tbl[idx[3] + (w & 0xFF)];
                ((u32*)d)[1] = (u32)tbl[idx[4] + (w2 >> 24)] << 24
                             | (u32)tbl[idx[5] + (w2 & 0xFF)] << 16
                             | (u32)tbl[idx[6] + ((w2 >> 8) & 0xFF)] << 8
                             | (u32)tbl[idx[7] + (w2 & 0xFF)];
                src += 8;
                idx += 8;
                d += stride;
            }
        }
        v *= 2;
    }
}

/* Per-byte bidirectional average: each output byte is (a_i + b_i + 1) >> 1,
 * computed byte-wise so carries never cross bytes. */
static inline u32 mpvBiAvg(u32 a, u32 b) {
    return ((a >> 24) + (b >> 24) + 1) >> 1 << 24
         | (((a >> 16) & 0xFF) + ((b >> 16) & 0xFF) + 1) >> 1 << 16
         | (((a >> 8) & 0xFF) + ((b >> 8) & 0xFF) + 1) >> 1 << 8
         | ((a & 0xFF) + (b & 0xFF) + 1) >> 1;
}

/* Bidirectional average remapped through the quantiser table: the averaged
 * byte is used as an offset into tbl, indexed per byte by the s16 values in
 * ix (ix[0] pairs with the top byte, ix[1] with bits 16-23, ...). */
static inline u32 mpvBiAvgTbl(u32 a, u32 b, const s16* ix, const u8* tbl) {
    return (u32)tbl[(((a >> 24) + (b >> 24) + 1) >> 1) + ix[0]] << 24
         | (u32)tbl[((((a >> 16) & 0xFF) + ((b >> 16) & 0xFF) + 1) >> 1) + ix[1]] << 16
         | (u32)tbl[((((a >> 8) & 0xFF) + ((b >> 8) & 0xFF) + 1) >> 1) + ix[2]] << 8
         | (u32)tbl[(((a & 0xFF) + (b & 0xFF) + 1) >> 1) + ix[3]];
}

/* Bidirectional motion-compensation make: renders one macroblock row into
 * each of the six 16-byte-row destinations listed in rows[1..12], averaging
 * the two reference pictures. With v < 0 the averaged pixels are remapped
 * through the quantiser table. The direct path zeroes the destination cache
 * line first (dcbz), the table path skips it. */
void mpvumc_BiMakeMb(MPVUMC* ctx, s32* rows, s32 v) {
    s16* idx = ctx->field_0x4;
    u8* src = ctx->field_0x8;
    u8* src2 = ctx->field_0xc;
    u8* tbl = ctx->field_0x0;
    s32* pairs = rows + 1;
    s32 i;
    for (i = 0; i < 6; i++) {
        u8* d = (u8*)pairs[0];
        s32 stride = pairs[1];
        pairs += 2;
        if (((u32)d & 0x1F) == 0) {
            if (v >= 0) {
                s32 k;
                idx += 64;
                for (k = 0; k < 8; k++) {
                    u32 a = ((u32*)src)[0];
                    u32 b = ((u32*)src2)[0];
                    __dcbz(d, 0);
                    u32 c = ((u32*)src)[1];
                    u32 e = ((u32*)src2)[1];
                    ((u32*)d)[0] = mpvBiAvg(a, b);
                    ((u32*)d)[1] = mpvBiAvg(c, e);
                    src += 8;
                    src2 += 8;
                    d += stride;
                }
            } else {
                s32 k;
                for (k = 0; k < 8; k++) {
                    u32 a = ((u32*)src)[0];
                    u32 b = ((u32*)src2)[0];
                    __dcbz(d, 0);
                    u32 c = ((u32*)src)[1];
                    u32 e = ((u32*)src2)[1];
                    ((u32*)d)[0] = mpvBiAvgTbl(a, b, idx, tbl);
                    ((u32*)d)[1] = mpvBiAvgTbl(c, e, idx + 4, tbl);
                    src += 8;
                    src2 += 8;
                    idx += 8;
                    d += stride;
                }
            }
        } else {
            if (v >= 0) {
                s32 k;
                idx += 64;
                for (k = 0; k < 8; k++) {
                    u32 a = ((u32*)src)[0];
                    u32 b = ((u32*)src2)[0];
                    u32 c = ((u32*)src)[1];
                    u32 e = ((u32*)src2)[1];
                    ((u32*)d)[0] = mpvBiAvg(a, b);
                    ((u32*)d)[1] = mpvBiAvg(c, e);
                    src += 8;
                    src2 += 8;
                    d += stride;
                }
            } else {
                s32 k;
                for (k = 0; k < 8; k++) {
                    u32 a = ((u32*)src)[0];
                    u32 b = ((u32*)src2)[0];
                    u32 c = ((u32*)src)[1];
                    u32 e = ((u32*)src2)[1];
                    ((u32*)d)[0] = mpvBiAvgTbl(a, b, idx, tbl);
                    ((u32*)d)[1] = mpvBiAvgTbl(c, e, idx + 4, tbl);
                    src += 8;
                    src2 += 8;
                    idx += 8;
                    d += stride;
                }
            }
        }
        v *= 2;
    }
}

void mpvumc_PpicSkipMb(s32* sizes, u8* sub1, u8* sub2);

/* P-picture skipped macroblock: plain copy of the reference macroblock into
 * the output frame (no MC filtering). Luma is copied as 8 rows of 8 bytes
 * spaced by the rounded stride, chroma as 16 rows of 16 bytes; when the
 * destination offset is 32-byte aligned the cache lines are zeroed first,
 * otherwise a straight copy is emitted. */
static void mpvUmcCopyMb8(u8* dst, const u8* src, s32 st, int z) {
    s32 st2 = st * 2;
    int k;
    for (k = 0; k < 4; k++) {
        if (z) {
            __dcbz(dst, 0);
        }
        ((u32*)dst)[0] = ((u32*)src)[0];
        ((u32*)dst)[1] = ((u32*)src)[1];
        if (z) {
            __dcbz(dst + st, 0);
        }
        ((u32*)(dst + st))[0] = ((u32*)(src + st))[0];
        ((u32*)(dst + st))[1] = ((u32*)(src + st))[1];
        dst += st2;
        src += st2;
    }
}

static void mpvUmcCopyMb16(u8* dst, const u8* src, s32 st, int z) {
    int k;
    for (k = 0; k < 16; k++) {
        if (z) {
            __dcbz(dst, 0);
        }
        ((u32*)dst)[0] = ((u32*)src)[0];
        ((u32*)dst)[1] = ((u32*)src)[1];
        ((u32*)dst)[2] = ((u32*)src)[2];
        ((u32*)dst)[3] = ((u32*)src)[3];
        dst += st;
        src += st;
    }
}

void mpvumc_PpicSkipMb(s32* sizes, u8* sub1, u8* sub2) {
    /* strides come from the frame descriptors, rounded via trunc division */
    s32 q = *(s16*)(sub2 + 12);
    s32 h = q / 8;
    if ((sizes[0] & 0x1F) == 0) {
        mpvUmcCopyMb8(*(u8**)sub2 + sizes[0], *(u8**)sub1 + sizes[0], h * 8, 1);
    } else {
        mpvUmcCopyMb8(*(u8**)sub2 + sizes[0], *(u8**)sub1 + sizes[0], h * 8, 0);
    }

    q = *(s16*)(sub2 + 14);
    h = q / 8;
    if ((sizes[1] & 0x1F) == 0) {
        mpvUmcCopyMb16(*(u8**)(sub2 + 8) + sizes[1], *(u8**)(sub1 + 8) + sizes[1], h * 8, 1);
    } else {
        mpvUmcCopyMb16(*(u8**)(sub2 + 8) + sizes[1], *(u8**)(sub1 + 8) + sizes[1], h * 8, 0);
    }
}

void MPVUMC_PpicSkipped(MPVUMC* ctx, s32 n) {
    s32 nm1 = n - 1;
    s32 end = ctx->field_0xcd8;
    s32 zero = 0;
    ctx->field_0xcd8 = end - nm1;
    ctx->field_0xce0 -= nm1;
    while (ctx->field_0xce0 < 0) {
        ctx->field_0xcdc -= 1;
        ctx->field_0xce0 += ctx->field_0xb64;
    }
    while (ctx->field_0xcd8 < end) {
        s32 sizes[2];
        sizes[0] = ctx->field_0xce0 * 8 + ctx->field_0xcdc * 8 * ctx->field_0xbfc;
        sizes[1] = ctx->field_0xce0 * 16 + ctx->field_0xcdc * 16 * ctx->field_0xbfe;
        mpvumc_PpicSkipMb(sizes, (u8*)ctx + 0xbf0, (u8*)ctx + 0xc00);
        if (++ctx->field_0xce0 >= ctx->field_0xb64) {
            ctx->field_0xce0 = zero;
            ctx->field_0xcdc += 1;
        }
        ctx->field_0xcd8 += 1;
    }
}

/* B-picture skip: rewind the macroblock cursor and replay the skipped
 * macroblocks through the P-picture skip dispatcher (field_0xc74). */
void MPVUMC_BpicSkipped(MPVUMC* ctx, s32 n) {
    s32 end = ctx->field_0xcd8;
    void (*fn)(MPVUMC*);
    ctx->field_0xcec = 0;
    fn = ctx->field_0xc74;
    ctx->field_0xcd8 = end - (n - 1);
    ctx->field_0xce0 -= n - 1;
    while (ctx->field_0xce0 < 0) {
        ctx->field_0xce0 += ctx->field_0xb64;
        ctx->field_0xcdc -= 1;
    }
    s32 zero = 0;
    while (ctx->field_0xcd8 < end) {
        fn(ctx);
        ctx->field_0xce0 += 1;
        if (ctx->field_0xce0 >= ctx->field_0xb64) {
            ctx->field_0xce0 = zero;
            ctx->field_0xcdc += 1;
        }
        ctx->field_0xcd8 += 1;
    }
}
