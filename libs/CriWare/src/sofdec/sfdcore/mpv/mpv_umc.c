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

/* Sub-frame descriptor block: one luma + one chroma plane descriptor.
 * ctx+0xbf0 is the reference (source) descriptor, ctx+0xc00 the output. */
typedef struct MPVUMCSUB {
    u8* plane0;     /* +0x0 - luma plane base */
    u32 field_0x4;
    u8* plane1;     /* +0x8 - chroma plane base */
    s16 stride;     /* +0xc - luma stride (s16) */
    s16 field_0xe;  /* +0xe - chroma stride (s16) */
} MPVUMCSUB;    /* 0x10 */

/* Motion-compensation working set handed to the MakeMb workers.
 * Lives at ctx+0xa90; the OneReadMb block indices alias the last two words. */
typedef struct MPVUMCMC {
    u8* tbl;    /* +0x0 - quantiser table */
    s16* idx;   /* +0x4 - MC index table */
    u8* src;    /* +0x8 - ref 0 plane (also read as s32 block index) */
    u8* src2;   /* +0xc - ref 1 plane (also read as s32 block index) */
} MPVUMCMC;    /* 0x10 */

/* Skip-MB row list: one header word, then {base,stride} pairs. */
typedef struct MPVMBROWS {
    u32 hdr;        /* +0x0 */
    s32 pair[10];   /* +0x4 - {base,stride} x5 */
} MPVMBROWS;

/* MPV UMC context (fields used by InitOutRfb) */
typedef struct MPVUMC_ {
    u8* field_0x0;      /* 0x0   - MC output table */
    s16* field_0x4;     /* 0x4   - MC index table */
    u8* field_0x8;      /* 0x8   - MC source (ref 0) */
    u8* field_0xc;      /* 0xc   - MC source (ref 1) */
    u8 pad0[0xa90 - 0x10];
    MPVUMCMC mc;                /* 0xa90 - MakeMb working set */
    u8 rowsHdr[4];              /* 0xaa0 - MakeMb rows header */
    s32 out[11];                /* 0xaa4 - MakeMb rows: {base,stride} x5 + final base */
    u8 pad0b[0xb5c - 0xad0];
    s32 mbw;        /* 0xb5c */
    s32 mbh;        /* 0xb60 */
    s32 field_0xb64;    /* 0xb64 - macroblock row width */
    u8 pad1[0xbf0 - 0xb68];
    MPVUMCSUB sub[2];   /* 0xbf0: reference, 0xc00: output */
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
 * quantiser table (cache-line zeroing when the destination is 32B aligned).
 * The quantiser index stream starts at the context head and advances 8
 * entries per output row; the unaligned path is the same row loop unrolled
 * x2 by MWCC (retail ctr=4). Statements are sequenced to mirror retail's
 * evaluation order so loads interleave identically. */
void fn_803AFDB0(MPVUMC* ctx) {
    u32* rows = (u32*)((u8*)ctx + 0xaa0);
    s32 col = ctx->field_0xcdc;
    s32 t4 = ctx->t4;
    s32 col16 = col * 16;
    s32 t3 = ctx->t3;
    s32 col8 = col * 8;
    s32 row = ctx->field_0xce0;
    s32 lprod = col16 * t4;
    s32 t2 = ctx->t2;
    s32 row16 = row * 16;
    s32 t0 = ctx->t0;
    s32 row8 = row * 8;
    s32 t1 = ctx->t1;
    s32 cprod = col8 * t3;
    s32 lumsum = t2 + row16;
    s32 midoff = t4 * 8;
    u8* tbl = *(u8**)((u8*)ctx + 0x9C0);
    s16* ix = (s16*)ctx;
    s32 lum = lprod + lumsum;
    s32 mid;
    s32 chr;
    s32* p;
    s32 n = 6;

    rows[5] = lum;
    mid = lum + midoff;
    chr = row8 + cprod;
    rows[1] = t0 + chr;
    rows[7] = lum + 8;
    rows[3] = t1 + chr;
    rows[9] = mid;
    rows[11] = mid + 8;

    p = (s32*)(rows + 1);
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
                    dst[j] = tbl[ix[j]];
                }
                ix += 8;
                dst += stride;
            }
        } else {
            s32 i;
            for (i = 0; i < 8; i++) {
                s32 j;
                for (j = 0; j < 8; j++) {
                    dst[j] = tbl[ix[j]];
                }
                ix += 8;
                dst += stride;
            }
        }
    } while (--n > 0);
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

    sizes[0] = x8 * ctx->sub[0].stride + row * 8;
    sizes[1] = x16 * ctx->sub[0].field_0xe + row * 16;
    mpvumc_OneReadMb(ctx, (s32)ctx->mc.src, sizes, (u8*)ctx + 0xbf0, (u8*)ctx + 0xc90);

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
    s32 v1;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 sz0;
    s32 sz1;
    s32 b;
    s32 mid;

    col = ctx->field_0xcdc;
    st8 = ctx->sub[1].stride;
    row = ctx->field_0xce0;
    x16 = col * 16;
    y8 = row * 8;
    y16 = row * 16;
    sizes[0] = y8 + col * 8 * st8;
    sizes[1] = y16 + x16 * ctx->sub[1].field_0xe;
    mpvumc_OneReadMb(ctx, (s32)ctx->mc.src, sizes, (u8*)ctx + 0xc00, (u8*)ctx + 0xcb4);

    /* chroma bases derive from the out[4] value in-register */
    col = ctx->field_0xcdc;
    t3 = ctx->t3;
    row = ctx->field_0xce0;
    x16 = col * 16;
    y8 = row * 8;
    y16 = row * 16;
    v0 = y8 + col * 8 * t3;
    sizes[0] = v0;
    t4 = ctx->t4;
    v1 = y16 + x16 * t4;
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
    mid = b + t4 * 8;
    ctx->out[8] = mid;
    ctx->out[10] = mid + 8;
    mpvumc_OneMakeMb((u8*)ctx + 0xa90, (u8*)ctx + 0xaa0, ctx->field_0xcec);
}

void mpvumc_BiMakeMb(MPVUMCMC* mc, s32* rows, s32 v);

/* fn_803B01BC: bidirectional macroblock - read both references, then make.
 * Column/row are re-read per phase so MWCC spills the row bases through
 * sizes[] exactly like retail (no cross-call CSE of the offsets). */
void fn_803B01BC(MPVUMC* ctx) {
    s32 sizes[2];
    s32 st = ctx->sub[0].stride;
    s32 col = ctx->field_0xcdc;
    s32 row = ctx->field_0xce0;
    s32 p0 = col * 8 * st;
    col = col * 16;
    s32 r16 = row * 16;
    s32 r8 = row * 8;
    sizes[0] = r8 + p0;
    sizes[1] = r16 + col * ctx->sub[0].field_0xe;
    mpvumc_OneReadMb(ctx, (s32)ctx->mc.src, sizes, (u8*)ctx + 0xbf0, (u8*)ctx + 0xc90);
    mpvumc_OneReadMb(ctx, (s32)ctx->mc.src2, sizes, (u8*)ctx + 0xc00, (u8*)ctx + 0xcb4);

    col = ctx->field_0xcdc;
    s32 t3 = ctx->t3;
    s32 q0 = col * 8;
    row = ctx->field_0xce0;
    q0 = q0 * t3;
    col = col * 16;
    s32 rr8 = row * 8;
    s32 rr16 = row * 16;
    sizes[0] = rr8 + q0;
    s32 t4 = ctx->t4;
    s32 v1 = col * t4;
    s32 t48 = t4 * 8;
    sizes[1] = rr16 + v1;
    ctx->out[0] = ctx->t0 + sizes[0];
    ctx->out[2] = ctx->t1 + sizes[0];
    {
        s32 b = ctx->t2 + sizes[1];
        ctx->out[4] = b;
        ctx->out[6] = b + 8;
        ctx->out[8] = b + t48;
        ctx->out[10] = b + t48 + 8;
    }
    mpvumc_BiMakeMb((MPVUMCMC*)((u8*)ctx + 0xa90), (s32*)((u8*)ctx + 0xaa0), ctx->field_0xcec);
}

/* One-reference macroblock read: picks the MC kernels for luma/chroma from
 * the dispatch tables (indexed by MB column and rounded MV half-pel bits),
 * computes the three block source offsets and drives the three kernel
 * calls (two luma blocks at +0/+64, one chroma pair at +128). */
void mpvumc_OneReadMb(void* ctx, s32 arg, s32* sizes, u8* sub1, u8* sub2) {
    s32* out = (s32*)((u8*)ctx + 0xA4C);
    s32 v = *(s32*)(sub2 + 28);
    s32 b = *(s32*)(sub2 + 24);
    s32 cc = *(s32*)((u8*)ctx + 0xB1C);
    /* round toward zero by adding the sign bit; the low bits select the
     * interpolation kernels (luma: bit 1 of the rounded vectors, chroma:
     * raw LSBs) */
    s32 vx = v + ((u32)v >> 31);
    s32 bx = b + ((u32)b >> 31);
    u32 fn1 = lbl_eu_80604688[cc * 4 + (v & 1) * 2 + (b & 1)];
    u32 fn2 = lbl_eu_80604668[cc * 4 + ((vx >> 1) & 1) * 2 + ((bx >> 1) & 1)];
    s32 h1 = *(s16*)(sub1 + 12);
    s32 h2 = *(s16*)(sub1 + 14);
    s32 da = sizes[0] + (bx >> 2) + (vx >> 2) * h1;
    s32 db = sizes[1] + (b >> 1) + (v >> 1) * h2;
    s32 fl = ((bx >> 30) & 1) & cc;
    s32 fc = (b & 1) & cc;
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
    d10 = d11 + h2 + fc;
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
    u8* src = *(u8**)((u8*)umc + 8);
    s16* idx = *(s16**)((u8*)umc + 4);
    const u8* tbl = *(const u8**)umc;
    s32* pairs = (s32*)(rows + 4);
    s32 i;
    for (i = 0; i < 6; i++) {
        u8* d = (u8*)pairs[0];
        s32 stride = pairs[1];
        pairs += 2;
        if (v >= 0) {
            /* aligned copy: read the whole 64-byte block up front, then lay
             * eight 2-word rows spaced by the stride rounded down to 8 */
            s32 off = stride & ~7;
            u8* p = d + off;
            u32 w0 = ((u32*)src)[0];
            u32 w1 = ((u32*)src)[1];
            u32 w2 = ((u32*)src)[2];
            u32 w3 = ((u32*)src)[3];
            u32 w4 = ((u32*)src)[4];
            u32 w5 = ((u32*)src)[5];
            u32 w6 = ((u32*)src)[6];
            u32 w7 = ((u32*)src)[7];
            idx += 64;
            ((u32*)d)[0] = w0;
            ((u32*)d)[1] = w1;
            *(u32*)p = w2;
            ((u32*)p)[1] = w3;
            p += off;
            ((u32*)p)[0] = w4;
            ((u32*)p)[1] = w5;
            p += off;
            ((u32*)p)[0] = w6;
            ((u32*)p)[1] = w7;
            p += off;
            w0 = ((u32*)src)[8];
            w1 = ((u32*)src)[9];
            w2 = ((u32*)src)[10];
            w3 = ((u32*)src)[11];
            w4 = ((u32*)src)[12];
            w5 = ((u32*)src)[13];
            w6 = ((u32*)src)[14];
            w7 = ((u32*)src)[15];
            ((u32*)p)[0] = w0;
            ((u32*)p)[1] = w1;
            p += off;
            ((u32*)p)[0] = w2;
            ((u32*)p)[1] = w3;
            p += off;
            ((u32*)p)[0] = w4;
            ((u32*)p)[1] = w5;
            p += off;
            ((u32*)p)[0] = w6;
            ((u32*)p)[1] = w7;
            src += 64;
        } else {
            /* remap through the quantiser table: the s16 entries select the
             * bias per destination byte, sourced from rotated source bytes */
            s32 k;
            for (k = 0; k < 8; k++) {
                u32 w = *(u32*)src;
                u32 w2 = *(u32*)(src + 4);
                ((u32*)d)[0] = (u32)tbl[idx[0] + ((w >> 16) & 0xFF)] << 24
                             | (u32)tbl[idx[1] + ((w >> 8) & 0xFF)] << 16
                             | (u32)tbl[idx[2] + (w >> 24)] << 8
                             | (u32)tbl[idx[3] + (w & 0xFF)];
                ((u32*)d)[1] = (u32)tbl[idx[4] + ((w2 >> 16) & 0xFF)] << 24
                             | (u32)tbl[idx[5] + ((w2 >> 8) & 0xFF)] << 16
                             | (u32)tbl[idx[6] + (w2 >> 24)] << 8
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
    /* sequential accumulation keeps peak temporaries low in the
     * quantiser-remap path */
    u32 r = (u32)tbl[(((a >> 24) + (b >> 24) + 1) >> 1) + ix[0]] << 24;
    r |= (u32)tbl[((((a >> 16) & 0xFF) + ((b >> 16) & 0xFF) + 1) >> 1) + ix[1]] << 16;
    r |= (u32)tbl[((((a >> 8) & 0xFF) + ((b >> 8) & 0xFF) + 1) >> 1) + ix[2]] << 8;
    r |= (u32)tbl[(((a & 0xFF) + (b & 0xFF) + 1) >> 1) + ix[3]];
    return r;
}

/* Bidirectional motion-compensation make: renders one macroblock row into
 * each of the six 16-byte-row destinations listed in rows[1..12], averaging
 * the two reference pictures. With v < 0 the averaged pixels are remapped
 * through the quantiser table. The direct path zeroes the destination cache
 * line first (dcbz), the table path skips it. */
void mpvumc_BiMakeMb(MPVUMCMC* mc, s32* rows, s32 v) {
    s16* idx = mc->idx;
    u8* src = mc->src;
    u8* src2 = mc->src2;
    u8* tbl = mc->tbl;
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
                    /* one word pair live at a time - mirrors retail's
                     * register reuse across both stores */
                    u32 a = ((u32*)src)[0];
                    u32 b = ((u32*)src2)[0];
                    __dcbz(d, 0);
                    ((u32*)d)[0] = mpvBiAvg(a, b);
                    a = ((u32*)src)[1];
                    b = ((u32*)src2)[1];
                    ((u32*)d)[1] = mpvBiAvg(a, b);
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
                    ((u32*)d)[0] = mpvBiAvgTbl(a, b, idx, tbl);
                    a = ((u32*)src)[1];
                    b = ((u32*)src2)[1];
                    ((u32*)d)[1] = mpvBiAvgTbl(a, b, idx + 4, tbl);
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
                    ((u32*)d)[0] = mpvBiAvg(a, b);
                    a = ((u32*)src)[1];
                    b = ((u32*)src2)[1];
                    ((u32*)d)[1] = mpvBiAvg(a, b);
                    src += 8;
                    src2 += 8;
                    d += stride;
                }
            } else {
                s32 k;
                for (k = 0; k < 8; k++) {
                    u32 a = ((u32*)src)[0];
                    u32 b = ((u32*)src2)[0];
                    ((u32*)d)[0] = mpvBiAvgTbl(a, b, idx, tbl);
                    a = ((u32*)src)[1];
                    b = ((u32*)src2)[1];
                    ((u32*)d)[1] = mpvBiAvgTbl(a, b, idx + 4, tbl);
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

/* P-picture skipped macroblock handler. Defined before mpvumc_PpicSkipMb so
 * MWCC cannot inline the skip-MB worker into this caller (retail keeps it as
 * an out-of-line bl). */
void mpvumc_PpicSkipMb(s32* sizes, MPVUMCSUB* src, MPVUMCSUB* dst);

void MPVUMC_PpicSkipped(MPVUMC* ctx, s32 n) {
    s32 end = ctx->field_0xcd8;
    ctx->field_0xcd8 = end - (n - 1);
    ctx->field_0xce0 -= n - 1;
    while (ctx->field_0xce0 < 0) {
        ctx->field_0xcdc -= 1;
        ctx->field_0xce0 += ctx->field_0xb64;
    }
    s32 zero = 0;
    while (ctx->field_0xcd8 < end) {
        s32 sizes[2];
        sizes[0] = ctx->field_0xce0 * 8 + ctx->field_0xcdc * 8 * ctx->sub[0].stride;
        sizes[1] = ctx->field_0xce0 * 16 + ctx->field_0xcdc * 16 * ctx->sub[0].field_0xe;
        mpvumc_PpicSkipMb(sizes, &ctx->sub[0], &ctx->sub[1]);
        if (++ctx->field_0xce0 >= ctx->field_0xb64) {
            ctx->field_0xce0 = zero;
            ctx->field_0xcdc += 1;
        }
        ctx->field_0xcd8 += 1;
    }
}

/* P-picture skipped macroblock: plain copy of the reference macroblock into
 * the output frame (no MC filtering). Luma is copied as 5 chunks of two 8-byte
 * rows spaced by the rounded stride, chroma as 16 rows of 16 bytes; when the
 * destination offset is 32-byte aligned the destination cache lines are zeroed
 * first (dcbz), otherwise a straight copy is emitted. Strides come from the
 * frame descriptors, divided by 8 with truncation. */
void mpvumc_PpicSkipMb(s32* sizes, MPVUMCSUB* src, MPVUMCSUB* dst) {
    /* strides come from the frame descriptors, divided by 8 with truncation */
    s32 h;
    s32 h8;
    s32 h16;
    s32 o;
    u8* d;
    u8* d2;
    u8* s;
    u32 a0;
    u32 a1;
    u32 b0;
    u32 b1;

    h = dst->stride / 8;
    if ((sizes[0] & 0x1F) == 0) {
        d = dst->plane0 + sizes[0];
        s = src->plane0 + sizes[0];
        __dcbz(d, 0);
        h8 = h * 8;
        a0 = *(u32*)(src->plane0 + sizes[0]);
        a1 = *(u32*)(s + 4);
        __dcbz(d, h8);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d2 = d + h8;
        h16 = h * 16;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h16);
        a1 = *(u32*)(s + 4);
        __dcbz(d, h8);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d2 = d + h8;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h16);
        a1 = *(u32*)(s + 4);
        __dcbz(d, h8);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d2 = d + h8;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h16);
        a1 = *(u32*)(s + 4);
        __dcbz(d, h8);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d2 = d + h8;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h16);
        a1 = *(u32*)(s + 4);
        __dcbz(d, h8);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d2 = d + h8;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
    } else {
        d = dst->plane0 + sizes[0];
        s = src->plane0 + sizes[0];
        h8 = h * 8;
        a0 = *(u32*)(src->plane0 + sizes[0]);
        a1 = *(u32*)(s + 4);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        d2 = d + h8;
        h16 = h * 16;
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
        a0 = *(u32*)(s += h16);
        a1 = *(u32*)(s + 4);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        d2 = d + h8;
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
        a0 = *(u32*)(s += h16);
        a1 = *(u32*)(s + 4);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        d2 = d + h8;
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
        a0 = *(u32*)(s += h16);
        a1 = *(u32*)(s + 4);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        d2 = d + h8;
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
        a0 = *(u32*)(s += h16);
        a1 = *(u32*)(s + 4);
        b0 = *(u32*)(s + h8);
        b1 = *(u32*)(s + h8 + 4);
        d2 = d + h8;
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        d += h16;
        ((u32*)d2)[1] = b1;
        ((u32*)d2)[0] = b0;
    }

    h = dst->field_0xe / 8;
    if ((sizes[1] & 0x1F) == 0) {
        d = dst->plane1 + sizes[1];
        s = src->plane1;
        __dcbz(d, 0);
        h8 = h * 8;
        o = sizes[1];
        a0 = *(u32*)(s += o);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        __dcbz(d, 0);
        a0 = ((u32*)s)[0];
        a1 = ((u32*)s)[1];
        b0 = ((u32*)s)[2];
        b1 = ((u32*)s)[3];
        ((u32*)d)[0] = a0;
        ((u32*)d)[1] = a1;
        ((u32*)d)[2] = b0;
        ((u32*)d)[3] = b1;
        d += h8;
        s += h8;
        __dcbz(d, 0);
        a0 = ((u32*)s)[0];
        a1 = ((u32*)s)[1];
        b0 = ((u32*)s)[2];
        b1 = ((u32*)s)[3];
        ((u32*)d)[0] = a0;
        ((u32*)d)[1] = a1;
        ((u32*)d)[2] = b0;
        ((u32*)d)[3] = b1;
        d += h8;
        s += h8;
        __dcbz(d, 0);
        a0 = ((u32*)s)[0];
        a1 = ((u32*)s)[1];
        b0 = ((u32*)s)[2];
        b1 = ((u32*)s)[3];
        ((u32*)d)[0] = a0;
        ((u32*)d)[1] = a1;
        ((u32*)d)[2] = b0;
        ((u32*)d)[3] = b1;
        d += h8;
        s += h8;
    } else {
        d = dst->plane1 + sizes[1];
        s = src->plane1;
        o = sizes[1];
        h8 = h * 8;
        a0 = *(u32*)(s += o);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = ((u32*)s)[0];
        a1 = ((u32*)s)[1];
        b0 = ((u32*)s)[2];
        b1 = ((u32*)s)[3];
        ((u32*)d)[0] = a0;
        ((u32*)d)[1] = a1;
        ((u32*)d)[2] = b0;
        ((u32*)d)[3] = b1;
        d += h8;
        s += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
        a0 = *(u32*)(s += h8);
        b0 = *(u32*)(s + 8);
        b1 = *(u32*)(s + 12);
        a1 = *(u32*)(s + 4);
        ((u32*)d)[1] = a1;
        ((u32*)d)[0] = a0;
        ((u32*)d)[3] = b1;
        ((u32*)d)[2] = b0;
        d += h8;
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
