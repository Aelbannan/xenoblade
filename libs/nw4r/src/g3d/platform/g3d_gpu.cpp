// nw4r/src/g3d/platform/g3d_gpu - fifo GPU command emitters
// Matched against retail via hexdiff; GX fifo writes via the 0xCC008000 port.

#include <nw4r/g3d/platform/g3d_gpu.h>

// Retail nw4r data-pool float constants (sdata2, owned by nw4r_data.o -
// extern only). Named after the retail symbols so the generated sda21 float
// relocations match retail exactly (instead of TU-local pool labels).
extern const f32 lbl_eu_80669BCC;  // 1.0f
extern const f32 lbl_eu_80669BD0;  // 0.5f
extern const f32 lbl_eu_80669BD4;  // 2.0f
extern const f32 lbl_eu_80669BD8;  // 256.0f (s11 fixed-point scale)
extern const f32 lbl_eu_80669BDC;  // 1.0f

namespace nw4r {
namespace g3d {
namespace fifo {

void GDSetGenMode2(u8 numTexGens, u8 numChans, u8 numTevs, u8 numInds,
                   GXCullMode cullMode) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    register const f32* cullTbl = &lbl_eu_80669BC8;
    const unsigned char* cull = (const unsigned char*)cullTbl;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = 0xFE07FC3F;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo =
        numTexGens | (numChans << 4) | ((numTevs - 1) << 10) |
        (cull[cullMode] << 14) | (numInds << 16);

    *fifo = 0x10;
    *(volatile unsigned short*)fifo = 0x0000;
    *(volatile unsigned short*)fifo = 0x1009;
    *(volatile unsigned int*)fifo = numChans;

    *fifo = 0x10;
    *(volatile unsigned short*)fifo = 0x0000;
    *(volatile unsigned short*)fifo = 0x103F;
    *(volatile unsigned int*)fifo = numTexGens;
}

void GDSetCullMode(GXCullMode cullMode) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    register const f32* cullTbl = &lbl_eu_80669BC8;
    const unsigned char* cull = (const unsigned char*)cullTbl;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = 0xFE00C000;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = cull[cullMode] << 14;
}

void GDSetTexCoordScale2(GXTexCoordID coord, u16 scaleS, GXBool biasS,
                         GXBool wrapS, u16 scaleT, GXBool biasT,
                         GXBool wrapT) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    u32 tex0 = (scaleS - 1) | (biasS << 16) | (wrapS << 17) |
               ((coord * 2 + 0x30) << 24);
    u32 tex1 = (scaleT - 1) | (biasT << 16) | (wrapT << 17) |
               ((coord * 2 + 0x31) << 24);

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = 0xFE03FFFF;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = tex0;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo = tex1;
}

void GDResetCurrentMtx() {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;
    *fifo = 0x08;
    *fifo = 0x30;
    *(volatile unsigned int*)fifo = 0x3cf40000 - 0x3100;
    *fifo = 0x08;
    *fifo = 0x40;
    *(volatile unsigned int*)fifo = 0x00f40000 - 0x30c4;
    *fifo = 0x10;
    *(volatile unsigned short*)fifo = 0x0001;
    *(volatile unsigned short*)fifo = 0x1018;
    *(volatile unsigned int*)fifo = 0x3cf40000 - 0x3100;
    *(volatile unsigned int*)fifo = 0x00f40000 - 0x30c4;
}

void GDSetCurrentMtx(const u32* pIdArray) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    // BP register 0x18 (current matrix id): pack both matrix ids into the
    // two 32-bit words with their 6-bit fields pre-shifted.
    u32 w0 = (pIdArray[0] << 6) | (pIdArray[1] << 12) | (pIdArray[2] << 18) |
             (pIdArray[3] << 24);
    u32 w1 = pIdArray[4] | (pIdArray[5] << 6) | (pIdArray[6] << 12) |
             (pIdArray[7] << 18);

    *fifo = 0x10;
    *(volatile unsigned short*)fifo = 0x0001;
    *(volatile unsigned short*)fifo = 0x1018;
    *(volatile unsigned int*)fifo = w0;
    *(volatile unsigned int*)fifo = w1;
}

void GDLoadTexMtxImm3x3(const math::MTX33& rMtx, u32 id) {
    // Promote the 3x3 matrix to a 3x4 (identity translation column) and pass
    // it straight to GXLoadTexMtxImm.
    math::MTX34 mtx;
    mtx.m[0][0] = rMtx.m[0][0];
    mtx.m[0][1] = rMtx.m[0][1];
    mtx.m[0][2] = rMtx.m[0][2];
    mtx.m[0][3] = lbl_eu_80669BDC;
    mtx.m[1][0] = rMtx.m[1][0];
    mtx.m[1][1] = rMtx.m[1][1];
    mtx.m[1][2] = rMtx.m[1][2];
    mtx.m[1][3] = lbl_eu_80669BDC;
    mtx.m[2][0] = rMtx.m[2][0];
    mtx.m[2][1] = rMtx.m[2][1];
    mtx.m[2][2] = rMtx.m[2][2];
    mtx.m[2][3] = lbl_eu_80669BDC;
    GXLoadTexMtxImm(mtx.mtx, id, GX_MTX_3x4);
}

void GDSetIndTexMtx(u32 id, const math::MTX34& rMtx) {
    volatile unsigned char* const fifo = (volatile unsigned char*)0xCC008000;

    // Top two rows of the 3x4 matrix (the offset matrix), with absolute
    // values kept alongside for the normalization search below.
    f32 m00 = rMtx.m[0][0];
    f32 m01 = rMtx.m[0][1];
    f32 m10 = rMtx.m[1][0];
    f32 a00 = __fabsf(m00);
    f32 m02 = rMtx.m[0][2];
    f32 m11 = rMtx.m[1][1];
    f32 m12 = rMtx.m[1][2];
    f32 a01 = __fabsf(m01);
    f32 a02 = __fabsf(m02);
    f32 a10 = __fabsf(m10);
    f32 a11 = __fabsf(m11);
    f32 a12 = __fabsf(m12);

    s8 scale = 0;

    if (a00 >= lbl_eu_80669BCC || a01 >= lbl_eu_80669BCC ||
        a02 >= lbl_eu_80669BCC || a10 >= lbl_eu_80669BCC ||
        a11 >= lbl_eu_80669BCC || a12 >= lbl_eu_80669BCC) {
        // Scale down: halve every element until the largest |value| < 1.0f.
        do {
            if (scale >= 46) {
                break;
            }
            a00 *= lbl_eu_80669BD0;
            m00 *= lbl_eu_80669BD0;
            m01 *= lbl_eu_80669BD0;
            m02 *= lbl_eu_80669BD0;
            m10 *= lbl_eu_80669BD0;
            m11 *= lbl_eu_80669BD0;
            m12 *= lbl_eu_80669BD0;
            a01 *= lbl_eu_80669BD0;
            a02 *= lbl_eu_80669BD0;
            a10 *= lbl_eu_80669BD0;
            a11 *= lbl_eu_80669BD0;
            a12 *= lbl_eu_80669BD0;
            scale++;
        } while (a00 >= lbl_eu_80669BCC || a01 >= lbl_eu_80669BCC ||
                 a02 >= lbl_eu_80669BCC || a10 >= lbl_eu_80669BCC ||
                 a11 >= lbl_eu_80669BCC || a12 >= lbl_eu_80669BCC);
    } else if (a00 < lbl_eu_80669BD0 && a01 < lbl_eu_80669BD0 &&
               a02 < lbl_eu_80669BD0 && a10 < lbl_eu_80669BD0 &&
               a11 < lbl_eu_80669BD0 && a12 < lbl_eu_80669BD0) {
        // Scale up: double every element until the largest |value| >= 0.5f.
        do {
            a00 *= lbl_eu_80669BD4;
            m00 *= lbl_eu_80669BD4;
            m01 *= lbl_eu_80669BD4;
            m02 *= lbl_eu_80669BD4;
            m10 *= lbl_eu_80669BD4;
            m11 *= lbl_eu_80669BD4;
            m12 *= lbl_eu_80669BD4;
            a01 *= lbl_eu_80669BD4;
            a02 *= lbl_eu_80669BD4;
            a10 *= lbl_eu_80669BD4;
            a11 *= lbl_eu_80669BD4;
            a12 *= lbl_eu_80669BD4;
            scale--;
        } while (a00 < lbl_eu_80669BD0 && a01 < lbl_eu_80669BD0 &&
                 a02 < lbl_eu_80669BD0 && a10 < lbl_eu_80669BD0 &&
                 a11 < lbl_eu_80669BD0 && a12 < lbl_eu_80669BD0 &&
                 scale > -17);
    }


    // Encode as three XF indirect-matrix register writes. Each word carries an
    // 11-bit fixed-point element pair plus a 2-bit slice of the 6-bit scale
    // exponent (scale + 17) and the XF register id in the top byte.
    f32 scl = lbl_eu_80669BD8;
    u32 exp = scale + 17;

    s32 w00 = (s32)(scl * m00);

    u32 word0 = (u32)(exp & 3) << 22;
    s32 w10 = (s32)(scl * m10);
    word0 |= (id + 6) << 24;
    s32 w01 = (s32)(scl * m01);
    u32 word1 = (u32)((exp >> 2) & 3) << 22;
    s32 w11 = (s32)(scl * m11);
    word1 |= (id + 7) << 24;
    s32 w02 = (s32)(scl * m02);
    s32 w12 = (s32)(scl * m12);
    u32 word2 = (u32)((exp >> 4) & 3) << 22;
    word2 |= (id + 8) << 24;

    *fifo = 0x61;
    *(volatile unsigned int*)fifo =
        word0 | (((u32)w10 & 0x7FF) << 11) | ((u32)w00 & 0x7FF);
    *fifo = 0x61;
    *(volatile unsigned int*)fifo =
        word1 | (((u32)w11 & 0x7FF) << 11) | ((u32)w01 & 0x7FF);
    *fifo = 0x61;
    *(volatile unsigned int*)fifo =
        word2 | (((u32)w12 & 0x7FF) << 11) | ((u32)w02 & 0x7FF);
}

} // namespace fifo
} // namespace g3d
} // namespace nw4r
