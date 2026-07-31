// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_mcy
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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
            u32 a2 = *(u32 *)&s1[8];
            u32 b2 = *(u32 *)&s2[8];
            u32 a3 = *(u32 *)&s1[12];
            u32 b3 = *(u32 *)&s2[12];
            *(u32 *)&dst[0] = (a0 & b0) + (((a0 ^ b0) & 0xFEFEFEFE) >> 1) + ((a0 ^ b0) & 0x01010101);
            *(u32 *)&dst[4] = (a1 & b1) + (((a1 ^ b1) & 0xFEFEFEFE) >> 1) + ((a1 ^ b1) & 0x01010101);
            *(u32 *)&dst[0x40] = (a2 & b2) + (((a2 ^ b2) & 0xFEFEFEFE) >> 1) + ((a2 ^ b2) & 0x01010101);
            *(u32 *)&dst[0x44] = (a3 & b3) + (((a3 ^ b3) & 0xFEFEFEFE) >> 1) + ((a3 ^ b3) & 0x01010101);
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
            *(u32 *)&dst[0] = (m0 & n0) + (((m0 ^ n0) & 0xFEFEFEFE) >> 1) + ((m0 ^ n0) & 0x01010101);
            *(u32 *)&dst[4] = (m1 & n1) + (((m1 ^ n1) & 0xFEFEFEFE) >> 1) + ((m1 ^ n1) & 0x01010101);
            *(u32 *)&dst[0x40] = (m2 & n2) + (((m2 ^ n2) & 0xFEFEFEFE) >> 1) + ((m2 ^ n2) & 0x01010101);
            *(u32 *)&dst[0x44] = (m3 & n3) + (((m3 ^ n3) & 0xFEFEFEFE) >> 1) + ((m3 ^ n3) & 0x01010101);
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
            *(u32 *)&dst[0] = (m0 & n0) + (((m0 ^ n0) & 0xFEFEFEFE) >> 1) + ((m0 ^ n0) & 0x01010101);
            *(u32 *)&dst[4] = (m1 & n1) + (((m1 ^ n1) & 0xFEFEFEFE) >> 1) + ((m1 ^ n1) & 0x01010101);
            *(u32 *)&dst[0x40] = (m2 & n2) + (((m2 ^ n2) & 0xFEFEFEFE) >> 1) + ((m2 ^ n2) & 0x01010101);
            *(u32 *)&dst[0x44] = (m3 & n3) + (((m3 ^ n3) & 0xFEFEFEFE) >> 1) + ((m3 ^ n3) & 0x01010101);
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
            *(u32 *)&dst[0] = (m0 & n0) + (((m0 ^ n0) & 0xFEFEFEFE) >> 1) + ((m0 ^ n0) & 0x01010101);
            *(u32 *)&dst[4] = (m1 & n1) + (((m1 ^ n1) & 0xFEFEFEFE) >> 1) + ((m1 ^ n1) & 0x01010101);
            *(u32 *)&dst[0x40] = (m2 & n2) + (((m2 ^ n2) & 0xFEFEFEFE) >> 1) + ((m2 ^ n2) & 0x01010101);
            *(u32 *)&dst[0x44] = (m3 & n3) + (((m3 ^ n3) & 0xFEFEFEFE) >> 1) + ((m3 ^ n3) & 0x01010101);
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




void MPVMC16_OneRef1p_TuneC() {}

void MPVMC16_OneRefH2_TuneC() {}

void MPVMC16_OneRef4p_TuneC() {}
