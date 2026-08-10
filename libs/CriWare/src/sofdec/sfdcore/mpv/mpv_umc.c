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
    u8 pad0[0xb5c - 0x10];
    s32 mbw;        /* 0xb5c */
    s32 mbh;        /* 0xb60 */
    s32 field_0xb64;    /* 0xb64 - macroblock row width */
    u8 pad1[0xc10 - 0xb68];
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

void fn_803AFDB0(void* ctx) {
    u8* c = (u8*)ctx;
    s32 mbw = *(s32*)(c + 0xCDC);
    s32 mbh = *(s32*)(c + 0xCE0);
    s16 h = *(s16*)(c + 0xC42);
    s16 w = *(s16*)(c + 0xC40);
    s32 a = *(s32*)(c + 0xC3C);
    s32 b = *(s32*)(c + 0xC34);
    s32 cc = *(s32*)(c + 0xC38);
    u8* tbl = *(u8**)(c + 0x9C0);
    s32 row0 = mbw * 16 * h + a + mbh * 16;
    s32 row1 = b + mbw * 8 * w;
    s32 row2 = cc + mbw * 8 * w;
    s32 row3 = row0 + 8;
    s32 row4 = row0 + h * 8;
    s32 row5 = row0 + h * 8 + 8;
    *(s32*)(c + 0xAB4) = row0;
    *(s32*)(c + 0xAA4) = row1;
    *(s32*)(c + 0xAAC) = row2;
    *(s32*)(c + 0xABC) = row3;
    *(s32*)(c + 0xAC4) = row4;
    *(s32*)(c + 0xACC) = row5;
    {
        s32* p = (s32*)(c + 0xAA4);
        s32 n = 6;
        do {
            s32 v = p[0];
            s32 stride = p[1];
            u8* dst = (u8*)v;
            s32 i;
            s16* idx = (s16*)c;
            p += 2;
            if ((v & 0x1F) == 0) {
                for (i = 0; i < 8; i++) {
                    __dcbz(dst, stride);
                    dst[0] = tbl[idx[0]];
                    dst[1] = tbl[idx[1]];
                    dst[2] = tbl[idx[2]];
                    dst[3] = tbl[idx[3]];
                    dst[4] = tbl[idx[4]];
                    dst[5] = tbl[idx[5]];
                    dst[6] = tbl[idx[6]];
                    dst[7] = tbl[idx[7]];
                    idx += 8;
                    dst += stride;
                }
            } else {
                for (i = 0; i < 4; i++) {
                    dst[0] = tbl[idx[0]];
                    dst[1] = tbl[idx[1]];
                    dst[2] = tbl[idx[2]];
                    dst[3] = tbl[idx[3]];
                    dst[4] = tbl[idx[4]];
                    dst[5] = tbl[idx[5]];
                    dst[6] = tbl[idx[6]];
                    dst[7] = tbl[idx[7]];
                    dst[8] = tbl[idx[8]];
                    dst[9] = tbl[idx[9]];
                    dst[10] = tbl[idx[10]];
                    dst[11] = tbl[idx[11]];
                    dst[12] = tbl[idx[12]];
                    dst[13] = tbl[idx[13]];
                    dst[14] = tbl[idx[14]];
                    dst[15] = tbl[idx[15]];
                    idx += 16;
                    dst += stride;
                }
            }
            n--;
        } while (n != 0);
    }
}

void fn_803AFFB4(void* ctx) {
    u8* c = (u8*)ctx;
    s32 sizes[2];
    sizes[0] = *(s32*)(c + 0xCE0) * 8 + *(s32*)(c + 0xCDC) * 8 * (s32)*(s16*)(c + 0xBFC);
    sizes[1] = *(s32*)(c + 0xCE0) * 16 + *(s32*)(c + 0xCDC) * 16 * (s32)*(s16*)(c + 0xBFE);
    mpvumc_OneReadMb(ctx, *(s32*)(c + 0xA98), sizes, c + 0xBF0, c + 0xC90);
    {
        s32 v0 = *(s32*)(c + 0xCE0) * 8 + *(s32*)(c + 0xCDC) * 8 * (s32)*(s16*)(c + 0xC40);
        s32 v1 = *(s32*)(c + 0xCE0) * 16 + *(s32*)(c + 0xCDC) * 16 * (s32)*(s16*)(c + 0xC42);
        *(s32*)(c + 0xAA4) = *(s32*)(c + 0xC34) + v0;
        *(s32*)(c + 0xAAC) = *(s32*)(c + 0xC38) + v0;
        *(s32*)(c + 0xAB4) = *(s32*)(c + 0xC3C) + v1;
        *(s32*)(c + 0xABC) = *(s32*)(c + 0xC3C) + v1 + 8;
        *(s32*)(c + 0xAC4) = *(s32*)(c + 0xC3C) + v1 + (s32)*(s16*)(c + 0xC42) * 8;
        *(s32*)(c + 0xACC) = *(s32*)(c + 0xC3C) + v1 + (s32)*(s16*)(c + 0xC42) * 8 + 8;
    }
    mpvumc_OneMakeMb(c + 0xA90, c + 0xAA0, *(s32*)(c + 0xCEC));
}

void fn_803B00B8(void* ctx) {
    u8* c = (u8*)ctx;
    s32 sizes[2];
    sizes[0] = *(s32*)(c + 0xCE0) * 8 + *(s32*)(c + 0xCDC) * 8 * (s32)*(s16*)(c + 0xC0C);
    sizes[1] = *(s32*)(c + 0xCE0) * 16 + *(s32*)(c + 0xCDC) * 16 * (s32)*(s16*)(c + 0xC0E);
    mpvumc_OneReadMb(ctx, *(s32*)(c + 0xA98), sizes, c + 0xC00, c + 0xCB4);
    {
        s32 v0 = *(s32*)(c + 0xCE0) * 8 + *(s32*)(c + 0xCDC) * 8 * (s32)*(s16*)(c + 0xC40);
        s32 v1 = *(s32*)(c + 0xCE0) * 16 + *(s32*)(c + 0xCDC) * 16 * (s32)*(s16*)(c + 0xC42);
        *(s32*)(c + 0xAA4) = *(s32*)(c + 0xC34) + v0;
        *(s32*)(c + 0xAAC) = *(s32*)(c + 0xC38) + v0;
        *(s32*)(c + 0xAB4) = *(s32*)(c + 0xC3C) + v1;
        *(s32*)(c + 0xABC) = *(s32*)(c + 0xC3C) + v1 + 8;
        *(s32*)(c + 0xAC4) = *(s32*)(c + 0xC3C) + v1 + (s32)*(s16*)(c + 0xC42) * 8;
        *(s32*)(c + 0xACC) = *(s32*)(c + 0xC3C) + v1 + (s32)*(s16*)(c + 0xC42) * 8 + 8;
    }
    mpvumc_OneMakeMb(c + 0xA90, c + 0xAA0, *(s32*)(c + 0xCEC));
}

void fn_803B01BC(void* ctx) {
    u8* c = (u8*)ctx;
    s32 sizes[2];
    sizes[0] = *(s32*)(c + 0xCE0) * 8 + *(s32*)(c + 0xCDC) * 8 * (s32)*(s16*)(c + 0xBFC);
    sizes[1] = *(s32*)(c + 0xCE0) * 16 + *(s32*)(c + 0xCDC) * 16 * (s32)*(s16*)(c + 0xBFE);
    mpvumc_OneReadMb(ctx, *(s32*)(c + 0xA98), sizes, c + 0xBF0, c + 0xC90);
    mpvumc_OneReadMb(ctx, *(s32*)(c + 0xA98), sizes, c + 0xC00, c + 0xCB4);
    {
        s32 v0 = *(s32*)(c + 0xCE0) * 8 + *(s32*)(c + 0xCDC) * 8 * (s32)*(s16*)(c + 0xC40);
        s32 v1 = *(s32*)(c + 0xCE0) * 16 + *(s32*)(c + 0xCDC) * 16 * (s32)*(s16*)(c + 0xC42);
        *(s32*)(c + 0xAA4) = *(s32*)(c + 0xC34) + v0;
        *(s32*)(c + 0xAAC) = *(s32*)(c + 0xC38) + v0;
        *(s32*)(c + 0xAB4) = *(s32*)(c + 0xC3C) + v1;
        *(s32*)(c + 0xABC) = *(s32*)(c + 0xC3C) + v1 + 8;
        *(s32*)(c + 0xAC4) = *(s32*)(c + 0xC3C) + v1 + (s32)*(s16*)(c + 0xC42) * 8;
        *(s32*)(c + 0xACC) = *(s32*)(c + 0xC3C) + v1 + (s32)*(s16*)(c + 0xC42) * 8 + 8;
    }
    mpvumc_OneMakeMb(c + 0xA90, c + 0xAA0, *(s32*)(c + 0xCEC));
}

void mpvumc_OneReadMb(void* ctx, s32 arg, s32* sizes, u8* sub1, u8* sub2) {
    u8* c = (u8*)ctx;
    s32* out = (s32*)(c + 0xA4C);
    s32 b = *(s32*)(sub2 + 24);
    s32 v = *(s32*)(sub2 + 28);
    s32 cc = *(s32*)(c + 0xB1C);
    s32 x = b + (b >> 31);
    s32 y = v + (v >> 31);
    s32 h1 = *(s16*)(sub1 + 12);
    s32 h2 = *(s16*)(sub1 + 14);
    s32 fn1 = lbl_eu_80604688[cc * 4 + ((v >> 2) & 1) * 2 + ((b >> 2) & 1)];
    s32 fn2 = lbl_eu_80604668[cc * 4 + ((y >> 2) & 1) * 2 + ((x >> 1) & 1)];
    s32 d9 = *(s32*)sub1 + sizes[0] + ((b + (b >> 31)) >> 2) + (((v + (v >> 31)) >> 2) * h1);
    s32 d10 = d9 + (((b + (b >> 31)) & 1) & cc) + h1;
    s32 d11 = sizes[1] + (b >> 1) + (v >> 1) * h2;
    out[6] = arg;
    out[8] = h1;
    out[9] = d9;
    out[10] = d10;
    ((void (*)(void*, s32, s32, s32))fn2)(out, arg, d9, d10);
    out[6] = arg + 64;
    out[9] = d9 + 64;
    out[10] = d10 + 64;
    ((void (*)(void*, s32, s32, s32))fn2)(out, arg + 64, d9 + 64, d10 + 64);
    out[6] = arg + 128;
    out[8] = h2;
    out[9] = *(s32*)(sub1 + 8) + d11;
    out[10] = *(s32*)(sub1 + 8) + d11 + h2 + ((b & 1) & cc);
    ((void (*)(void*, s32, s32, s32))fn1)(out, arg + 128, out[9], out[10]);
}

void mpvumc_OneMakeMb(void* umc, u8* rows, s32 v) {
    u8* c = (u8*)umc;
    u8* src = *(u8**)(c + 8);
    u8* tbl = *(u8**)(c + 0);
    s16* idx = *(s16**)(c + 4);
    s32* pairs = (s32*)(rows + 4);
    s32 i;
    for (i = 0; i < 6; i++) {
        s32 d = pairs[0];
        s32 stride = pairs[1];
        pairs += 2;
        if (v != 0) {
            s32 st = stride & ~7;
            s32 k;
            for (k = 0; k < 8; k++) {
                ((u32*)d)[(k * st) / 4 + 0] = ((u32*)src)[2 * k + 0];
                ((u32*)d)[(k * st) / 4 + 1] = ((u32*)src)[2 * k + 1];
            }
        } else {
            s32 k;
            for (k = 0; k < 8; k++) {
                u32 w = *(u32*)src;
                u32 w2 = *(u32*)(src + 4);
                s32 a0 = idx[0];
                s32 a1 = idx[2];
                s32 a2 = idx[1];
                s32 a3 = idx[3];
                ((u32*)d)[0] = (u32)tbl[a2 + ((w >> 8) & 0xFF)] << 16 | (u32)tbl[a3 + (w & 0xFF)];
                ((u32*)d)[1] = (u32)tbl[a1 + ((w2 >> 8) & 0xFF)] << 8 | (u32)tbl[a2 + ((w2 >> 8) & 0xFF)] << 16;
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

void MPVUMC_PpicSkipped(void* ctx, s32 n) {
    u8* c = (u8*)ctx;
    s32 r31 = *(s32*)(c + 0xCD8);
    s32 v0 = *(s32*)(c + 0xCE0);
    *(s32*)(c + 0xCD8) = r31 - (n - 1);
    *(s32*)(c + 0xCE0) = v0 - (n - 1);
    while (*(s32*)(c + 0xCE0) < 0) {
        *(s32*)(c + 0xCDC) -= 1;
        *(s32*)(c + 0xCE0) += *(s32*)(c + 0xB64);
    }
    while (*(s32*)(c + 0xCD8) < r31) {
        s32 mbw = *(s32*)(c + 0xCDC);
        s32 mbh = *(s32*)(c + 0xCE0);
        s32 sizes[2];
        sizes[0] = mbh * 8 + mbw * 8 * (s32)*(s16*)(c + 0xBFC);
        sizes[1] = mbh * 16 + mbw * 16 * (s32)*(s16*)(c + 0xBFE);
        mpvumc_PpicSkipMb(sizes, c + 0xBF0, c + 0xC00);
        mbh += 1;
        *(s32*)(c + 0xCE0) = mbh;
        if (mbh >= *(s32*)(c + 0xB64)) {
            mbh = 0;
            *(s32*)(c + 0xCE0) = mbh;
            *(s32*)(c + 0xCDC) += 1;
        }
        *(s32*)(c + 0xCD8) += 1;
    }
}

/* B-picture skip: rewind the macroblock cursor and replay the skipped
 * macroblocks through the P-picture skip dispatcher (field_0xc74). */
void MPVUMC_BpicSkipped(MPVUMC* ctx, s32 n) {
    s32 nm1 = n - 1;
    s32 end = ctx->field_0xcd8;
    s32 v0 = ctx->field_0xce0;
    ctx->field_0xcec = 0;
    void (*fn)(MPVUMC*) = ctx->field_0xc74;
    ctx->field_0xcd8 = end - nm1;
    ctx->field_0xce0 = v0 - nm1;
    while (ctx->field_0xce0 < 0) {
        ctx->field_0xcdc -= 1;
        ctx->field_0xce0 += ctx->field_0xb64;
    }
    while (ctx->field_0xcd8 < end) {
        fn(ctx);
        if (++ctx->field_0xce0 >= ctx->field_0xb64) {
            ctx->field_0xce0 = 0;
            ctx->field_0xcdc = ctx->field_0xcdc + 1;
        }
        ctx->field_0xcd8 = ctx->field_0xcd8 + 1;
    }
}
