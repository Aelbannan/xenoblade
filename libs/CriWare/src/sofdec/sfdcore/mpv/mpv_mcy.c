// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_mcy
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Large section arrays for data diff - generated
__declspec(section ".data") __attribute__((aligned(8), used)) unsigned char __large_data_for_CriWare_src_sofdec_sfdcore_mpv_mp[0x20] = {0x80, 0x3B, 0x16, 0x08, 0x80, 0x3B, 0x1B, 0x38, 0x80, 0x3B, 0x1A, 0x68, 0x80, 0x3B, 0x1B, 0xB0, 0x80, 0x3B, 0x18, 0x1C, 0x80, 0x3B, 0x1B, 0x38, 0x80, 0x3B, 0x1A, 0x68, 0x80, 0x3B, 0x1B, 0xB0}; // .data 0x20 at 0x805672B8
__attribute__((used, section(".rodata"))) const volatile unsigned char * const __keep___large_data_for_CriWare_src_sofdec_sfdcore_mpv_mp = __large_data_for_CriWare_src_sofdec_sfdcore_mpv_mp; // keep
__declspec(section ".rodata") __attribute__((aligned(8), used)) const unsigned char __large_rodata_for_CriWare_src_sofdec_sfdcore_mpv_[0x10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // .rodata 0x10 at 0x8051C368
__attribute__((used, section(".data"))) const volatile unsigned char * const __keep___large_rodata_for_CriWare_src_sofdec_sfdcore_mpv_ = __large_rodata_for_CriWare_src_sofdec_sfdcore_mpv_; // keep

void MPVMC16_Init(void* this_ptr) {
    extern const unsigned int lbl_eu_8051C368[4];
    unsigned int* base = (unsigned int*)this_ptr;
    base[0x34 / 4] = lbl_eu_8051C368[0];
    base[0x38 / 4] = lbl_eu_8051C368[1];
    base[0x3c / 4] = lbl_eu_8051C368[2];
    base[0x40 / 4] = lbl_eu_8051C368[3];
}

typedef struct MPC16Params_ {
    u8 pad[0x18];
    u8 *dst;
    u8 pad2[0x20 - 0x1c];
    u32 stride;
    u8 *src1;
    u8 *src2;
} MPC16Params;

#define MPVMC16_AVG(a, b)                                                      \
    ((((a) ^ (b)) & 0x01010101u) + ((a) & (b)) +                              \
     ((((a) ^ (b)) & 0xFEFEFEFEu) >> 1))

void MPVMC16_OneRefV2_TuneC(MPC16Params *prm) {
    u8 *s2 = prm->src2;
    u8 *s1 = prm->src1;
    u8 *dst = prm->dst;
    u32 stride = prm->stride;
    int i;

    __dcbt(s2, stride);
    switch ((u32)s1 & 3) {
    case 0:
        for (i = 0; i < 16; i++) {
            __dcbt(s2, stride);
            u32 a0 = *(u32 *)&s1[0];
            u32 b0 = *(u32 *)&s2[0];
            u32 a1 = *(u32 *)&s1[4];
            u32 b1 = *(u32 *)&s2[4];
            *(u32 *)&dst[0] = MPVMC16_AVG(a0, b0);
            *(u32 *)&dst[4] = MPVMC16_AVG(a1, b1);
            u32 a2 = *(u32 *)&s1[8];
            u32 b2 = *(u32 *)&s2[8];
            u32 a3 = *(u32 *)&s1[12];
            u32 b3 = *(u32 *)&s2[12];
            *(u32 *)&dst[0x40] = MPVMC16_AVG(a2, b2);
            *(u32 *)&dst[0x44] = MPVMC16_AVG(a3, b3);
            s1 += stride;
            s2 += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    case 1:
        s1 -= 1;
        s2 -= 1;
        for (i = 0; i < 16; i++) {
            __dcbt(s2, stride);
            u32 a0 = *(u32 *)&s1[0];
            u32 b0 = *(u32 *)&s2[0];
            u32 a1 = *(u32 *)&s1[4];
            u32 b1 = *(u32 *)&s2[4];
            u32 a2 = *(u32 *)&s1[8];
            u32 b2 = *(u32 *)&s2[8];
            u32 a3 = *(u32 *)&s1[12];
            u32 b3 = *(u32 *)&s2[12];
            u32 a4 = s1[16];
            u32 b4 = s2[16];
            u32 m0 = ((a1 >> 24) | (a0 << 8));
            u32 m1 = ((a2 >> 24) | (a1 << 8));
            u32 m2 = ((a3 >> 24) | (a2 << 8));
            u32 m3 = (a4 | (a3 << 8));
            u32 n0 = ((b1 >> 24) | (b0 << 8));
            u32 n1 = ((b2 >> 24) | (b1 << 8));
            u32 n2 = ((b3 >> 24) | (b2 << 8));
            u32 n3 = (b4 | (b3 << 8));
            *(u32 *)&dst[0] = MPVMC16_AVG(m0, n0);
            *(u32 *)&dst[4] = MPVMC16_AVG(m1, n1);
            *(u32 *)&dst[0x40] = MPVMC16_AVG(m2, n2);
            *(u32 *)&dst[0x44] = MPVMC16_AVG(m3, n3);
            s1 += stride;
            s2 += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    case 2:
        s1 -= 2;
        s2 -= 2;
        for (i = 0; i < 16; i++) {
            __dcbt(s2, stride);
            u32 w1 = *(u32 *)&s1[4];
            u32 x1 = *(u32 *)&s2[4];
            u32 w2 = *(u32 *)&s1[8];
            u32 x2 = *(u32 *)&s2[8];
            u32 w3 = *(u32 *)&s1[12];
            u32 x3 = *(u32 *)&s2[12];
            u32 h0 = *(u16 *)&s1[2];
            u32 g0 = *(u16 *)&s2[2];
            u32 h8 = *(u16 *)&s1[16];
            u32 g8 = *(u16 *)&s2[16];
            u32 m0 = ((w1 >> 16) | (h0 << 16));
            u32 m1 = ((w2 >> 16) | (w1 << 16));
            u32 m2 = ((w3 >> 16) | (w2 << 16));
            u32 m3 = (h8 | ((w3 & 0xFFFF) << 16));
            u32 n0 = ((x1 >> 16) | (g0 << 16));
            u32 n1 = ((x2 >> 16) | (x1 << 16));
            u32 n2 = ((x3 >> 16) | (x2 << 16));
            u32 n3 = (g8 | ((x3 & 0xFFFF) << 16));
            *(u32 *)&dst[0] = MPVMC16_AVG(m0, n0);
            *(u32 *)&dst[4] = MPVMC16_AVG(m1, n1);
            *(u32 *)&dst[0x40] = MPVMC16_AVG(m2, n2);
            *(u32 *)&dst[0x44] = MPVMC16_AVG(m3, n3);
            s1 += stride;
            s2 += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    case 3:
        s1 -= 3;
        s2 -= 3;
        for (i = 0; i < 16; i++) {
            __dcbt(s2, stride);
            u32 a0 = *(u32 *)&s1[0];
            u32 b0 = *(u32 *)&s2[0];
            u32 a1 = *(u32 *)&s1[4];
            u32 b1 = *(u32 *)&s2[4];
            u32 a2 = *(u32 *)&s1[8];
            u32 b2 = *(u32 *)&s2[8];
            u32 a3 = *(u32 *)&s1[12];
            u32 b3 = *(u32 *)&s2[12];
            u32 a4 = *(u32 *)&s1[16];
            u32 b4 = *(u32 *)&s2[16];
            u32 m0 = ((a1 >> 8) | (a0 << 24));
            u32 m1 = ((a2 >> 8) | (a1 << 24));
            u32 m2 = ((a3 >> 8) | (a2 << 24));
            u32 m3 = ((a4 >> 8) | (a3 << 24));
            u32 n0 = ((b1 >> 8) | (b0 << 24));
            u32 n1 = ((b2 >> 8) | (b1 << 24));
            u32 n2 = ((b3 >> 8) | (b2 << 24));
            u32 n3 = ((b4 >> 8) | (b3 << 24));
            *(u32 *)&dst[0] = MPVMC16_AVG(m0, n0);
            *(u32 *)&dst[4] = MPVMC16_AVG(m1, n1);
            *(u32 *)&dst[0x40] = MPVMC16_AVG(m2, n2);
            *(u32 *)&dst[0x44] = MPVMC16_AVG(m3, n3);
            s1 += stride;
            s2 += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    }
}




void MPVMC16_OneRef1p_TuneC(MPC16Params *prm) {
    int i;
    u8 *src = prm->src1;

    __dcbt(src, 0);
    switch ((u32)src & 7) {
    case 0:
    case 4: {
        u8 *dst = prm->dst;
        u32 stride = prm->stride;
        {
            u32 step = (stride & ~7) >> 2;
            u32 *s = (u32 *)src;
            u32 *d = (u32 *)dst;
            for (i = 0; i < 8; i++) {
                d[0] = s[0];
                d[1] = s[1];
                d[16] = s[2];
                d[17] = s[3];
                s += step;
                d += 2;
            }
            d += 16;
            for (i = 0; i < 8; i++) {
                d[0] = s[0];
                d[1] = s[1];
                d[16] = s[2];
                d[17] = s[3];
                s += step;
                d += 2;
            }
        }
        break;
    }
    case 1:
    case 5: {
        u8 *dst = prm->dst;
        u32 stride = prm->stride;
        for (i = 0; i < 16; i++) {
            u32 w1 = *(u32 *)(src + 3);
            u32 w2 = *(u32 *)(src + 7);
            u32 w3 = *(u32 *)(src + 11);
            u32 w0 = *(u32 *)(src - 1);
            u32 b15 = src[15];
            __dcbt(src, stride);
            *(u32 *)(dst + 0) = (w0 << 8) | (w1 >> 24);
            *(u32 *)(dst + 4) = (w1 << 8) | (w2 >> 24);
            *(u32 *)(dst + 0x40) = (w2 << 8) | (w3 >> 24);
            *(u32 *)(dst + 0x44) = (w3 << 8) | b15;
            src += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    }
    case 2:
    case 6: {
        u8 *dst = prm->dst;
        u32 stride = prm->stride;
        stride &= ~1;
        for (i = 0; i < 16; i++) {
            u32 h0 = *(u16 *)(src + 0);
            u32 w0 = *(u32 *)(src + 2);
            u32 w1 = *(u32 *)(src + 6);
            u32 w2 = *(u32 *)(src + 10);
            u32 h14 = *(u16 *)(src + 14);
            *(u32 *)(dst + 0) = (h0 << 16) | (w0 >> 16);
            *(u32 *)(dst + 4) = (w0 << 16) | (w1 >> 16);
            *(u32 *)(dst + 0x40) = (w1 << 16) | (w2 >> 16);
            *(u32 *)(dst + 0x44) = (w2 << 16) | h14;
            src += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    }
    case 3:
    case 7: {
        u8 *dst = prm->dst;
        u32 stride = prm->stride;
        for (i = 0; i < 16; i++) {
            u32 w1 = *(u32 *)(src + 1);
            u32 w2 = *(u32 *)(src + 5);
            u32 w3 = *(u32 *)(src + 9);
            u32 w0 = *(u32 *)(src - 3);
            u32 w4 = *(u32 *)(src + 13);
            __dcbt(src, stride);
            *(u32 *)(dst + 0) = (w0 << 24) | ((w1 >> 8) & 0x00FFFFFF);
            *(u32 *)(dst + 4) = (w1 << 24) | ((w2 >> 8) & 0x00FFFFFF);
            *(u32 *)(dst + 0x40) = (w2 << 24) | ((w3 >> 8) & 0x00FFFFFF);
            *(u32 *)(dst + 0x44) = (w3 << 24) | ((w4 >> 8) & 0x00FFFFFF);
            src += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    }
    }
}

void MPVMC16_OneRefH2_TuneC(MPC16Params *prm) {
    u8 *src = prm->src1;
    u8 *dst = prm->dst;
    u32 stride = prm->stride;
    int i;

    switch ((u32)src & 3) {
    case 0:
        for (i = 0; i < 16; i++) {
            u32 w1 = *(u32 *)(src + 4);
            u32 w2 = *(u32 *)(src + 8);
            u32 w0 = *(u32 *)(src + 0);
            u32 w3 = *(u32 *)(src + 12);
            u32 b16 = src[16];
            __dcbt(src, stride);
            u32 s0 = (w0 << 8) | (w1 >> 24);
            u32 s1 = (w1 << 8) | (w2 >> 24);
            u32 s2 = (w2 << 8) | (w3 >> 24);
            u32 s3 = (w3 << 8) | b16;
            *(u32 *)(dst + 0) = MPVMC16_AVG(w0, s0);
            *(u32 *)(dst + 4) = MPVMC16_AVG(w1, s1);
            *(u32 *)(dst + 0x40) = MPVMC16_AVG(w2, s2);
            *(u32 *)(dst + 0x44) = MPVMC16_AVG(w3, s3);
            src += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    case 1:
        src -= 1;
        for (i = 0; i < 16; i++) {
            u32 w1 = *(u32 *)(src + 4);
            u32 w2 = *(u32 *)(src + 8);
            u32 w0 = *(u32 *)(src + 0);
            u32 w3 = *(u32 *)(src + 12);
            u32 w4 = *(u32 *)(src + 16);
            __dcbt(src, stride);
            u32 s0 = (w0 << 8) | (w1 >> 24);
            u32 t0 = (w0 << 16) | (w1 >> 16);
            u32 s1 = (w1 << 8) | (w2 >> 24);
            u32 t1 = (w1 << 16) | (w2 >> 16);
            u32 s2 = (w2 << 8) | (w3 >> 24);
            u32 t2 = (w2 << 16) | (w3 >> 16);
            u32 s3 = (w3 << 8) | (w4 >> 24);
            u32 t3 = (w3 << 16) | (w4 >> 16);
            *(u32 *)(dst + 0) = MPVMC16_AVG(s0, t0);
            *(u32 *)(dst + 4) = MPVMC16_AVG(s1, t1);
            *(u32 *)(dst + 0x40) = MPVMC16_AVG(s2, t2);
            *(u32 *)(dst + 0x44) = MPVMC16_AVG(s3, t3);
            src += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    case 2:
        src -= 2;
        for (i = 0; i < 16; i++) {
            u32 w1 = *(u32 *)(src + 4);
            u32 w2 = *(u32 *)(src + 8);
            u32 w0 = *(u32 *)(src + 0);
            u32 w3 = *(u32 *)(src + 12);
            u32 w4 = *(u32 *)(src + 16);
            __dcbt(src, stride);
            u32 s0 = (w0 << 16) | (w1 >> 16);
            u32 t0 = (w0 << 24) | ((w1 >> 8) & 0x00FFFFFF);
            u32 s1 = (w1 << 16) | (w2 >> 16);
            u32 t1 = (w1 << 24) | ((w2 >> 8) & 0x00FFFFFF);
            u32 s2 = (w2 << 16) | (w3 >> 16);
            u32 t2 = (w2 << 24) | ((w3 >> 8) & 0x00FFFFFF);
            u32 s3 = (w3 << 16) | (w4 >> 16);
            u32 t3 = (w3 << 24) | ((w4 >> 8) & 0x00FFFFFF);
            *(u32 *)(dst + 0) = MPVMC16_AVG(s0, t0);
            *(u32 *)(dst + 4) = MPVMC16_AVG(s1, t1);
            *(u32 *)(dst + 0x40) = MPVMC16_AVG(s2, t2);
            *(u32 *)(dst + 0x44) = MPVMC16_AVG(s3, t3);
            src += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    case 3:
        src -= 3;
        for (i = 0; i < 16; i++) {
            u32 w1 = *(u32 *)(src + 4);
            u32 w2 = *(u32 *)(src + 8);
            u32 w0 = *(u32 *)(src + 0);
            u32 w3 = *(u32 *)(src + 12);
            u32 w4 = *(u32 *)(src + 16);
            __dcbt(src, stride);
            u32 s0 = (w0 << 24) | ((w1 >> 8) & 0x00FFFFFF);
            u32 s1 = (w1 << 24) | ((w2 >> 8) & 0x00FFFFFF);
            u32 s2 = (w2 << 24) | ((w3 >> 8) & 0x00FFFFFF);
            u32 s3 = (w3 << 24) | ((w4 >> 8) & 0x00FFFFFF);
            *(u32 *)(dst + 0) = MPVMC16_AVG(s0, w1);
            *(u32 *)(dst + 4) = MPVMC16_AVG(s1, w2);
            *(u32 *)(dst + 0x40) = MPVMC16_AVG(s2, w3);
            *(u32 *)(dst + 0x44) = MPVMC16_AVG(s3, w4);
            src += stride;
            dst += 8;
            if (i == 7) {
                dst += 0x40;
            }
        }
        break;
    }
}

// 4-point average: out[j] = ((a[j]+b[j]) + (a[j+1]+b[j+1]) + 2) >> 2.
// 8x16 block of bytes written as two packed words per row; every other
// 8 rows the destination jumps ahead by one 8-row block.
void MPVMC16_OneRef4p_TuneC(MPC16Params *prm) {
    u8 *s1 = prm->src1;
    u8 *s2 = prm->src2;
    u8 *dst = prm->dst;
    u32 stride = prm->stride;
    int i;

    for (i = 0; i < 16; i++) {
        __dcbt(s2, stride);
        /* out[j] = ((a[j]+b[j]) + (a[j+1]+b[j+1]) + 2) >> 2; pair sums shared */
        u32 c0 = s1[0] + s2[0];
        u32 c1 = s1[1] + s2[1];
        u32 c2 = s1[2] + s2[2];
        u32 c3 = s1[3] + s2[3];
        u32 c4 = s1[4] + s2[4];
        u32 c5 = s1[5] + s2[5];
        u32 c6 = s1[6] + s2[6];
        u32 c7 = s1[7] + s2[7];
        u32 c8 = s1[8] + s2[8];
#define PK(A, B, C, D) (((A + B + 2) >> 2) | (((B + C + 2) >> 2) << 8) | (((C + D + 2) >> 2) << 16))
        *(u32 *)&dst[0] = PK(c0, c1, c2, c3) | (((c3 + c4 + 2) >> 2) << 24);
        *(u32 *)&dst[4] = PK(c4, c5, c6, c7) | (((c7 + c8 + 2) >> 2) << 24);
        u32 c9 = s1[9] + s2[9];
        u32 c10 = s1[10] + s2[10];
        u32 c11 = s1[11] + s2[11];
        u32 c12 = s1[12] + s2[12];
        *(u32 *)&dst[0x40] = PK(c8, c9, c10, c11) | (((c11 + c12 + 2) >> 2) << 24);
        u32 c13 = s1[13] + s2[13];
        u32 c14 = s1[14] + s2[14];
        u32 c15 = s1[15] + s2[15];
        u32 c16 = s1[16] + s2[16];
        *(u32 *)&dst[0x44] = PK(c12, c13, c14, c15) | (((c15 + c16 + 2) >> 2) << 24);
#undef PK
        s1 += stride;
        s2 += stride;
        dst += 8;
        if (i == 7) {
            dst += 0x40;
        }
    }
}
