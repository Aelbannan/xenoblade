// CriWare Sofdec MPEG-2 motion compensation (8-bit).

#include <harness_catalog.h>

const unsigned int mpvmc_oneref1p_func_table[4] = {0, 0, 0, 0}; // .rodata 0x10 at 0x8051C2A8

void MPVMC08_Init(void *dst_param) {
    unsigned int *dst = (unsigned int *)dst_param;
    dst[0] = mpvmc_oneref1p_func_table[0];
    dst[1] = mpvmc_oneref1p_func_table[1];
    dst[2] = mpvmc_oneref1p_func_table[2];
    dst[3] = mpvmc_oneref1p_func_table[3];
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
    u8 *dst;
    u8 *s1;
    u8 *s2;
    u32 stride;
    int i;
    s1 = prm->src1;
    s2 = prm->src2;
    dst = prm->dst;
    stride = prm->stride;

    for (i = 0; i < 8; i++) {
        u32 c0 = s1[0] + s2[0];
        __dcbt(s2, stride);
        u32 c1 = s1[1] + s2[1];
        u32 c2 = s1[2] + s2[2];
        u32 c3 = s1[3] + s2[3];
        u32 c4 = s1[4] + s2[4];
        u32 c5 = s1[5] + s2[5];
        u32 c6 = s1[6] + s2[6];
        u32 c7 = s1[7] + s2[7];
        u32 c8 = s1[8] + s2[8];
u32 m0 = c0 + c1;
        u32 m1 = c1 + c2;
        u32 m2 = c2 + c3;
        u32 m3 = c3 + c4;
        u32 m4 = c4 + c5;
        u32 w0 = (((m0 + 2) >> 2) << 24) | (((m1 + 2) >> 2) << 16) |
                 (((m2 + 2) >> 2) << 8) | ((m3 + 2) >> 2);
        m0 = c5 + c6;
        m1 = c6 + c7;
        m2 = c7 + c8;
        u32 w1 = (((m4 + 2) >> 2) << 24) | (((m0 + 2) >> 2) << 16) |
                 (((m1 + 2) >> 2) << 8) | ((m2 + 2) >> 2);
        *(u32 *)&dst[0] = w0;
        *(u32 *)&dst[4] = w1;
        s1 += stride;
        s2 += stride;
        dst += 8;
    }
}

/* Round-up byte average across four lanes: (a + b + 1) >> 1 per byte. */
#define MPVMC08_AVG(a, b)                                                      \
    ((((a) ^ (b)) & 0x01010101u) + ((a) & (b)) +                               \
     ((((a) ^ (b)) & 0xFEFEFEFEu) >> 1))

/* Half-pel horizontal one-ref compensation: (src[i] + src[i+1] + 1) >> 1.
   Dispatches on the source alignment so each case can use word/halfword
   loads with the appropriate shift recombination. */
void MPVMC08_OneRefH2_TuneC(MPC08Params *prm) {
    u8 *src = prm->src1;
    u8 *dst = prm->dst;
    u32 stride = prm->stride;
    int i;

    if (((u32)src & 3) == 0) {
        for (i = 0; i < 8; i++) {
            u32 a0 = *(const u32 *)src;
            u32 a1 = *(const u32 *)(src + 4);
            u32 b8 = src[8];
            __dcbt(src, stride);
            u32 s0 = (a1 >> 24) | (a0 << 8);
            u32 s1 = (a1 << 8) | b8;
            *(u32 *)dst = MPVMC08_AVG(a0, s0);
            *(u32 *)(dst + 4) = MPVMC08_AVG(a1, s1);
            src += stride;
            dst += 8;
        }
        return;
    }
    if (((u32)src & 3) == 1) {
        for (i = 0; i < 8; i++) {
            u32 wp3 = *(const u32 *)(src + 3);
            u32 wm1 = *(const u32 *)(src - 1);
            u16 h78 = *(const u16 *)(src + 6);
            __dcbt(src, stride);
            u32 w0 = (wm1 << 8) | (wp3 >> 24);
            u32 s0 = (wm1 << 16) | (wp3 >> 16);
            u32 s1 = (wp3 << 16) | h78;
            u32 w1 = (wp3 << 8) | (h78 >> 8);
            *(u32 *)dst = MPVMC08_AVG(w0, s0);
            *(u32 *)(dst + 4) = MPVMC08_AVG(w1, s1);
            src += stride;
            dst += 8;
        }
        return;
    }
    if (((u32)src & 3) == 2) {
        for (i = 0; i < 8; i++) {
            u32 wm2 = *(const u32 *)(src - 2);
            u32 wp2 = *(const u32 *)(src + 2);
            u32 wp6 = *(const u32 *)(src + 6);
            __dcbt(src, stride);
            u32 w0 = (wm2 << 16) | (wp2 >> 16);
            u32 s0 = (wm2 << 24) | ((wp2 >> 8) & 0x00FFFFFF);
            u32 w1 = (wp2 << 16) | (wp6 >> 16);
            u32 s1 = (wp2 << 24) | ((wp6 >> 8) & 0x00FFFFFF);
            *(u32 *)dst = MPVMC08_AVG(w0, s0);
            *(u32 *)(dst + 4) = MPVMC08_AVG(w1, s1);
            src += stride;
            dst += 8;
        }
        return;
    }
    for (i = 0; i < 8; i++) {
        u32 wp1 = *(const u32 *)(src + 1);
        u32 wp5 = *(const u32 *)(src + 5);
        u32 b0 = __lwbrx(src - 3, 0);
        __dcbt(src, stride);
        u32 w0 = (b0 & 0xFF000000) | ((wp1 >> 8) & 0x00FFFFFF);
        u32 w1 = (wp1 << 24) | ((wp5 >> 8) & 0x00FFFFFF);
        *(u32 *)dst = MPVMC08_AVG(w0, wp1);
        *(u32 *)(dst + 4) = MPVMC08_AVG(w1, wp5);
        src += stride;
        dst += 8;
    }
    return;
}

/* Half-pel vertical one-ref compensation: (src1[i] + src2[i] + 1) >> 1.
   Dispatches on the src1 alignment; two rows per loop iteration. */
void MPVMC08_OneRefV2_TuneC(MPC08Params *prm) {
    u8 *dst;
    u8 *s1;
    u8 *s2;
    u32 stride;
    int i;
    s1 = prm->src1;
    s2 = prm->src2;
    dst = prm->dst;
    stride = prm->stride;

    if (((u32)s1 & 3) == 0)
        goto mpvmc08_v2_case0;
    if (((u32)s1 & 3) == 1)
        goto mpvmc08_v2_case1;
    if (((u32)s1 & 3) == 2)
        goto mpvmc08_v2_case2;
    if (((u32)s1 & 3) == 3)
        goto mpvmc08_v2_case3;
    return;

mpvmc08_v2_case0:
        for (i = 0; i < 4; i++) {
            u32 a0 = *(const u32 *)s1;
            u32 b0 = *(const u32 *)s2;
            u32 a1 = *(const u32 *)(s1 + 4);
            u32 b1 = *(const u32 *)(s2 + 4);
            u32 x0 = a0 ^ b0;
            u32 x1 = a1 ^ b1;
            u32 r0 = (a0 & b0) + (x0 & 0x01010101);
            u32 r1 = (a1 & b1) + (x1 & 0x01010101);
            r0 += (x0 & 0xFEFEFEFE) >> 1;
            r1 += (x1 & 0xFEFEFEFE) >> 1;
            *(u32 *)dst = r0;
            *(u32 *)(dst + 4) = r1;
            s1 += stride;
            s2 += stride;
            dst += 8;
            a0 = *(const u32 *)s1;
            b0 = *(const u32 *)s2;
            a1 = *(const u32 *)(s1 + 4);
            b1 = *(const u32 *)(s2 + 4);
            x0 = a0 ^ b0;
            x1 = a1 ^ b1;
            r0 = (a0 & b0) + (x0 & 0x01010101);
            r1 = (a1 & b1) + (x1 & 0x01010101);
            r0 += (x0 & 0xFEFEFEFE) >> 1;
            r1 += (x1 & 0xFEFEFEFE) >> 1;
            *(u32 *)dst = r0;
            *(u32 *)(dst + 4) = r1;
            s1 += stride;
            s2 += stride;
            dst += 8;
        }
        goto mpvmc08_v2_done;
mpvmc08_v2_case1:
        for (i = 0; i < 4; i++) {
            u32 wm1 = *(const u32 *)(s1 - 1);
            u32 xm1 = *(const u32 *)(s2 - 1);
            u32 wp3 = *(const u32 *)(s1 + 3);
            u32 xp3 = *(const u32 *)(s2 + 3);
            u32 b7 = s1[7];
            u32 c7 = s2[7];
            u32 w0 = (wm1 << 8) | (wp3 >> 24);
            u32 x0 = (xm1 << 8) | (xp3 >> 24);
            u32 w1 = (wp3 << 8) | b7;
            u32 x1 = (xp3 << 8) | c7;
            u32 e0 = w0 ^ x0;
            u32 e1 = w1 ^ x1;
            u32 r0 = (w0 & x0) + (e0 & 0x01010101);
            u32 r1 = (w1 & x1) + (e1 & 0x01010101);
            r0 += (e0 & 0xFEFEFEFE) >> 1;
            r1 += (e1 & 0xFEFEFEFE) >> 1;
            *(u32 *)dst = r0;
            *(u32 *)(dst + 4) = r1;
            s1 += stride;
            s2 += stride;
            dst += 8;
            wm1 = *(const u32 *)(s1 - 1);
            xm1 = *(const u32 *)(s2 - 1);
            wp3 = *(const u32 *)(s1 + 3);
            xp3 = *(const u32 *)(s2 + 3);
            b7 = s1[7];
            c7 = s2[7];
            w0 = (wm1 << 8) | (wp3 >> 24);
            x0 = (xm1 << 8) | (xp3 >> 24);
            w1 = (wp3 << 8) | b7;
            x1 = (xp3 << 8) | c7;
            e0 = w0 ^ x0;
            e1 = w1 ^ x1;
            r0 = (w0 & x0) + (e0 & 0x01010101);
            r1 = (w1 & x1) + (e1 & 0x01010101);
            r0 += (e0 & 0xFEFEFEFE) >> 1;
            r1 += (e1 & 0xFEFEFEFE) >> 1;
            *(u32 *)dst = r0;
            *(u32 *)(dst + 4) = r1;
            s1 += stride;
            s2 += stride;
            dst += 8;
        }
        goto mpvmc08_v2_done;
mpvmc08_v2_case2:
        for (i = 0; i < 4; i++) {
            u32 h01 = *(const u16 *)(s1 + 0);
            u32 g01 = *(const u16 *)(s2 + 0);
            u32 wp2 = *(const u32 *)(s1 + 2);
            u32 xp2 = *(const u32 *)(s2 + 2);
            u32 h67 = *(const u16 *)(s1 + 6);
            u32 g67 = *(const u16 *)(s2 + 6);
            u32 w0 = (h01 << 16) | (wp2 >> 16);
            u32 x0 = (g01 << 16) | (xp2 >> 16);
            u32 w1 = (wp2 << 16) | h67;
            u32 x1 = (xp2 << 16) | g67;
            u32 e0 = w0 ^ x0;
            u32 e1 = w1 ^ x1;
            u32 r0 = (w0 & x0) + (e0 & 0x01010101);
            u32 r1 = (w1 & x1) + (e1 & 0x01010101);
            r0 += (e0 & 0xFEFEFEFE) >> 1;
            r1 += (e1 & 0xFEFEFEFE) >> 1;
            *(u32 *)dst = r0;
            *(u32 *)(dst + 4) = r1;
            s1 += stride;
            s2 += stride;
            dst += 8;
            h01 = *(const u16 *)(s1 + 0);
            g01 = *(const u16 *)(s2 + 0);
            wp2 = *(const u32 *)(s1 + 2);
            xp2 = *(const u32 *)(s2 + 2);
            h67 = *(const u16 *)(s1 + 6);
            g67 = *(const u16 *)(s2 + 6);
            w0 = (h01 << 16) | (wp2 >> 16);
            x0 = (g01 << 16) | (xp2 >> 16);
            w1 = (wp2 << 16) | h67;
            x1 = (xp2 << 16) | g67;
            e0 = w0 ^ x0;
            e1 = w1 ^ x1;
            r0 = (w0 & x0) + (e0 & 0x01010101);
            r1 = (w1 & x1) + (e1 & 0x01010101);
            r0 += (e0 & 0xFEFEFEFE) >> 1;
            r1 += (e1 & 0xFEFEFEFE) >> 1;
            *(u32 *)dst = r0;
            *(u32 *)(dst + 4) = r1;
            s1 += stride;
            s2 += stride;
            dst += 8;
        }
        goto mpvmc08_v2_done;
mpvmc08_v2_case3:
        for (i = 0; i < 4; i++) {
            u32 wm3 = *(const u32 *)(s1 - 3);
            u32 xm3 = *(const u32 *)(s2 - 3);
            u32 wp1 = *(const u32 *)(s1 + 1);
            u32 xp1 = *(const u32 *)(s2 + 1);
            u32 wp5 = *(const u32 *)(s1 + 5);
            u32 xp5 = *(const u32 *)(s2 + 5);
            u32 w0 = (wm3 << 24) | ((wp1 >> 8) & 0x00FFFFFF);
            u32 x0 = (xm3 << 24) | ((xp1 >> 8) & 0x00FFFFFF);
            u32 w1 = (wp1 << 24) | ((wp5 >> 8) & 0x00FFFFFF);
            u32 x1 = (xp1 << 24) | ((xp5 >> 8) & 0x00FFFFFF);
            u32 e0 = w0 ^ x0;
            u32 e1 = w1 ^ x1;
            u32 r0 = (w0 & x0) + (e0 & 0x01010101);
            u32 r1 = (w1 & x1) + (e1 & 0x01010101);
            r0 += (e0 & 0xFEFEFEFE) >> 1;
            r1 += (e1 & 0xFEFEFEFE) >> 1;
            *(u32 *)dst = r0;
            *(u32 *)(dst + 4) = r1;
            s1 += stride;
            s2 += stride;
            dst += 8;
            wm3 = *(const u32 *)(s1 - 3);
            xm3 = *(const u32 *)(s2 - 3);
            wp1 = *(const u32 *)(s1 + 1);
            xp1 = *(const u32 *)(s2 + 1);
            wp5 = *(const u32 *)(s1 + 5);
            xp5 = *(const u32 *)(s2 + 5);
            w0 = (wm3 << 24) | ((wp1 >> 8) & 0x00FFFFFF);
            x0 = (xm3 << 24) | ((xp1 >> 8) & 0x00FFFFFF);
            w1 = (wp1 << 24) | ((wp5 >> 8) & 0x00FFFFFF);
            x1 = (xp1 << 24) | ((xp5 >> 8) & 0x00FFFFFF);
            e0 = w0 ^ x0;
            e1 = w1 ^ x1;
            r0 = (w0 & x0) + (e0 & 0x01010101);
            r1 = (w1 & x1) + (e1 & 0x01010101);
            r0 += (e0 & 0xFEFEFEFE) >> 1;
            r1 += (e1 & 0xFEFEFEFE) >> 1;
            *(u32 *)dst = r0;
            *(u32 *)(dst + 4) = r1;
            s1 += stride;
            s2 += stride;
            dst += 8;
        }
mpvmc08_v2_done:
    return;
}

/* Full-pel one-ref compensation: 8x8 block copy with stride.
   Dispatches on source alignment; misaligned cases recombine shifted
   word loads so each row needs only 8+overlap input bytes. */
void MPVMC08_OneRef1p_TuneC(MPC08Params *prm) {
    u8 *src = prm->src1;

    __dcbt(src, 0);
    switch ((u32)src & 7) {
    case 0: {
        u32 stride = prm->stride;
        u8 *p = src;
        u8 *dst = prm->dst;
        double v0;
        double v1;
        double v2;
        double v3;
        double v4;
        double v5;
        double v6;
        double v7;
        src += stride;
        v1 = *(const double *)src;
        src += stride;
        v2 = *(const double *)src;
        src += stride;
        v3 = *(const double *)src;
        src += stride;
        v4 = *(const double *)src;
        src += stride;
        v5 = *(const double *)src;
        src += stride;
        v6 = *(const double *)src;
        src += stride;
        v7 = *(const double *)src;
        v0 = *(const double *)p;
        ((double *)dst)[0] = v0;
        ((double *)dst)[1] = v1;
        ((double *)dst)[2] = v2;
        ((double *)dst)[3] = v3;
        ((double *)dst)[4] = v4;
        ((double *)dst)[5] = v5;
        ((double *)dst)[6] = v6;
        ((double *)dst)[7] = v7;
        break;
    }
    case 4: {
        u32 stride = prm->stride;
        u8 *p = src;
        u8 *dst = prm->dst;
        u32 x0;
        u32 x1;
        u32 y0;
        u32 y1;
        src += stride;
        y0 = *(const u32 *)src;
        x1 = *(const u32 *)(p + 4);
        y1 = *(const u32 *)(src + 4);
        x0 = *(const u32 *)p;
        ((u32 *)dst)[0] = x0;
        ((u32 *)dst)[1] = x1;
        src += stride;
        x0 = *(const u32 *)src;
        x1 = *(const u32 *)(src + 4);
        ((u32 *)dst)[2] = y0;
        ((u32 *)dst)[3] = y1;
        src += stride;
        y0 = *(const u32 *)src;
        y1 = *(const u32 *)(src + 4);
        ((u32 *)dst)[4] = x0;
        ((u32 *)dst)[5] = x1;
        src += stride;
        x0 = *(const u32 *)src;
        x1 = *(const u32 *)(src + 4);
        ((u32 *)dst)[6] = y0;
        ((u32 *)dst)[7] = y1;
        src += stride;
        y0 = *(const u32 *)src;
        y1 = *(const u32 *)(src + 4);
        ((u32 *)dst)[8] = x0;
        ((u32 *)dst)[9] = x1;
        src += stride;
        x0 = *(const u32 *)src;
        x1 = *(const u32 *)(src + 4);
        ((u32 *)dst)[10] = y0;
        ((u32 *)dst)[11] = y1;
        src += stride;
        y0 = *(const u32 *)src;
        y1 = *(const u32 *)(src + 4);
        ((u32 *)dst)[12] = x0;
        ((u32 *)dst)[13] = x1;
        ((u32 *)dst)[14] = y0;
        ((u32 *)dst)[15] = y1;
        break;
    }
    case 2:
    case 6: {
        u32 stride = prm->stride;
        u32 w = *(const u32 *)(src + 2);
        u16 h3 = *(const u16 *)(src + 6);
        u8 *dst;
        u32 hi;
        u16 h0;
        u32 lo;
        u32 v0;
        u8 *p;
        stride &= ~1u;
        dst = prm->dst;
        hi = w >> 16;
        h0 = *(const u16 *)src;
        lo = (w << 16) | h3;
        p = src + stride;
        v0 = ((u32)h0 << 16) | hi;
        ((u32 *)dst)[0] = v0;
        ((u32 *)dst)[1] = lo;
        w = *(const u32 *)(p + 2);
        h3 = *(const u16 *)(p + 6);
        hi = w >> 16;
        h0 = *(const u16 *)p;
        lo = (w << 16) | h3;
        p += stride;
        v0 = ((u32)h0 << 16) | hi;
        ((u32 *)dst)[2] = v0;
        ((u32 *)dst)[3] = lo;
        w = *(const u32 *)(p + 2);
        h3 = *(const u16 *)(p + 6);
        hi = w >> 16;
        h0 = *(const u16 *)p;
        lo = (w << 16) | h3;
        p += stride;
        v0 = ((u32)h0 << 16) | hi;
        ((u32 *)dst)[4] = v0;
        ((u32 *)dst)[5] = lo;
        w = *(const u32 *)(p + 2);
        h3 = *(const u16 *)(p + 6);
        hi = w >> 16;
        h0 = *(const u16 *)p;
        lo = (w << 16) | h3;
        p += stride;
        v0 = ((u32)h0 << 16) | hi;
        ((u32 *)dst)[6] = v0;
        ((u32 *)dst)[7] = lo;
        w = *(const u32 *)(p + 2);
        h3 = *(const u16 *)(p + 6);
        hi = w >> 16;
        h0 = *(const u16 *)p;
        lo = (w << 16) | h3;
        p += stride;
        v0 = ((u32)h0 << 16) | hi;
        ((u32 *)dst)[8] = v0;
        ((u32 *)dst)[9] = lo;
        w = *(const u32 *)(p + 2);
        h3 = *(const u16 *)(p + 6);
        hi = w >> 16;
        h0 = *(const u16 *)p;
        lo = (w << 16) | h3;
        p += stride;
        v0 = ((u32)h0 << 16) | hi;
        ((u32 *)dst)[10] = v0;
        ((u32 *)dst)[11] = lo;
        w = *(const u32 *)(p + 2);
        h3 = *(const u16 *)(p + 6);
        hi = w >> 16;
        h0 = *(const u16 *)p;
        lo = (w << 16) | h3;
        p += stride;
        v0 = ((u32)h0 << 16) | hi;
        ((u32 *)dst)[12] = v0;
        ((u32 *)dst)[13] = lo;
        w = *(const u32 *)(p + 2);
        h3 = *(const u16 *)(p + 6);
        hi = w >> 16;
        h0 = *(const u16 *)p;
        lo = (w << 16) | h3;
        p += stride;
        v0 = ((u32)h0 << 16) | hi;
        ((u32 *)dst)[14] = v0;
        ((u32 *)dst)[15] = lo;
        break;
    }
    case 1:
    case 5: {
        u32 stride = prm->stride;
        u8 *dst = prm->dst;
        u32 w1;
        u32 b7;
        u32 w0;
        u32 v1;
        u32 v0;
        u8 *p;
        __dcbt(src, stride);
        p = src + stride;
        w1 = *(const u32 *)(src + 3);
        b7 = src[7];
        w0 = *(const u32 *)(src - 1);
        v1 = (w1 << 8) | b7;
        v0 = (w1 >> 24) | (w0 << 8);
        ((u32 *)dst)[0] = v0;
        ((u32 *)dst)[1] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 3);
        b7 = p[7];
        w0 = *(const u32 *)(p - 1);
        v1 = (w1 << 8) | b7;
        p += stride;
        v0 = (w1 >> 24) | (w0 << 8);
        ((u32 *)dst)[2] = v0;
        ((u32 *)dst)[3] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 3);
        b7 = p[7];
        w0 = *(const u32 *)(p - 1);
        v1 = (w1 << 8) | b7;
        p += stride;
        v0 = (w1 >> 24) | (w0 << 8);
        ((u32 *)dst)[4] = v0;
        ((u32 *)dst)[5] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 3);
        b7 = p[7];
        w0 = *(const u32 *)(p - 1);
        v1 = (w1 << 8) | b7;
        p += stride;
        v0 = (w1 >> 24) | (w0 << 8);
        ((u32 *)dst)[6] = v0;
        ((u32 *)dst)[7] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 3);
        b7 = p[7];
        w0 = *(const u32 *)(p - 1);
        v1 = (w1 << 8) | b7;
        p += stride;
        v0 = (w1 >> 24) | (w0 << 8);
        ((u32 *)dst)[8] = v0;
        ((u32 *)dst)[9] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 3);
        b7 = p[7];
        w0 = *(const u32 *)(p - 1);
        v1 = (w1 << 8) | b7;
        p += stride;
        v0 = (w1 >> 24) | (w0 << 8);
        ((u32 *)dst)[10] = v0;
        ((u32 *)dst)[11] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 3);
        b7 = p[7];
        w0 = *(const u32 *)(p - 1);
        v1 = (w1 << 8) | b7;
        p += stride;
        v0 = (w1 >> 24) | (w0 << 8);
        ((u32 *)dst)[12] = v0;
        ((u32 *)dst)[13] = v1;
        w1 = *(const u32 *)(p + 3);
        b7 = p[7];
        w0 = *(const u32 *)(p - 1);
        v1 = (w1 << 8) | b7;
        p += stride;
        v0 = (w1 >> 24) | (w0 << 8);
        ((u32 *)dst)[14] = v0;
        ((u32 *)dst)[15] = v1;
        break;
    }
    case 3:
    case 7: {
        u32 stride = prm->stride;
        u8 *dst = prm->dst;
        u32 w1;
        u32 w2;
        u32 w0;
        u32 t1;
        u32 t2;
        u32 v0;
        u32 v1;
        u8 *p;
        __dcbt(src, stride);
        p = src + stride;
        w1 = *(const u32 *)(src + 1);
        w2 = *(const u32 *)(src + 5);
        w0 = *(const u32 *)(src - 3);
        t1 = w1 >> 8;
        t2 = w2 >> 8;
        v0 = t1 | (w0 << 24);
        ((u32 *)dst)[0] = v0;
        v1 = t2 | (w1 << 24);
        ((u32 *)dst)[1] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 1);
        w2 = *(const u32 *)(p + 5);
        w0 = *(const u32 *)(p - 3);
        t1 = w1 >> 8;
        t2 = w2 >> 8;
        p += stride;
        v0 = t1 | (w0 << 24);
        ((u32 *)dst)[2] = v0;
        v1 = t2 | (w1 << 24);
        ((u32 *)dst)[3] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 1);
        w2 = *(const u32 *)(p + 5);
        w0 = *(const u32 *)(p - 3);
        t1 = w1 >> 8;
        t2 = w2 >> 8;
        p += stride;
        v0 = t1 | (w0 << 24);
        ((u32 *)dst)[4] = v0;
        v1 = t2 | (w1 << 24);
        ((u32 *)dst)[5] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 1);
        w2 = *(const u32 *)(p + 5);
        w0 = *(const u32 *)(p - 3);
        t1 = w1 >> 8;
        t2 = w2 >> 8;
        p += stride;
        v0 = t1 | (w0 << 24);
        ((u32 *)dst)[6] = v0;
        v1 = t2 | (w1 << 24);
        ((u32 *)dst)[7] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 1);
        w2 = *(const u32 *)(p + 5);
        w0 = *(const u32 *)(p - 3);
        t1 = w1 >> 8;
        t2 = w2 >> 8;
        p += stride;
        v0 = t1 | (w0 << 24);
        ((u32 *)dst)[8] = v0;
        v1 = t2 | (w1 << 24);
        ((u32 *)dst)[9] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 1);
        w2 = *(const u32 *)(p + 5);
        w0 = *(const u32 *)(p - 3);
        t1 = w1 >> 8;
        t2 = w2 >> 8;
        p += stride;
        v0 = t1 | (w0 << 24);
        ((u32 *)dst)[10] = v0;
        v1 = t2 | (w1 << 24);
        ((u32 *)dst)[11] = v1;
        __dcbt(p, stride);
        w1 = *(const u32 *)(p + 1);
        w2 = *(const u32 *)(p + 5);
        w0 = *(const u32 *)(p - 3);
        t1 = w1 >> 8;
        t2 = w2 >> 8;
        p += stride;
        v0 = t1 | (w0 << 24);
        ((u32 *)dst)[12] = v0;
        v1 = t2 | (w1 << 24);
        ((u32 *)dst)[13] = v1;
        w1 = *(const u32 *)(p + 1);
        w2 = *(const u32 *)(p + 5);
        w0 = *(const u32 *)(p - 3);
        t1 = w1 >> 8;
        t2 = w2 >> 8;
        p += stride;
        v0 = t1 | (w0 << 24);
        ((u32 *)dst)[14] = v0;
        v1 = t2 | (w1 << 24);
        ((u32 *)dst)[15] = v1;
        break;
    }
    }
}
