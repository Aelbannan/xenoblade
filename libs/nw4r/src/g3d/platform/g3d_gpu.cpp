// nw4r/src/g3d/platform/g3d_gpu - fifo GPU command emitters
// Matched against retail via hexdiff; GX fifo writes use the SDK WGPIPE.

#include <nw4r/g3d/platform/g3d_gpu.h>

namespace nw4r {
namespace g3d {
namespace fifo {

void GDSetGenMode2(u8 numTexGens, u8 numChans, u8 numTevs, u8 numInds,
                   GXCullMode cullMode) {
    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui = 0xFE07FC3F;

    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui =
        numTexGens | (numChans << 4) | ((numTevs - 1) << 10) |
        (cm2hw[cullMode] << GX_BP_GENMODE_CULLMODE_SHIFT) |
        (numInds << GX_BP_GENMODE_NUMINDSTAGES_SHIFT);

    WGPIPE.uc = GX_FIFO_CMD_LOAD_XF_REG;
    WGPIPE.us = 0;
    WGPIPE.us = GX_XF_REG_NUMCOLORS;
    WGPIPE.ui = numChans;

    WGPIPE.uc = GX_FIFO_CMD_LOAD_XF_REG;
    WGPIPE.us = 0;
    WGPIPE.us = GX_XF_REG_NUMTEX;
    WGPIPE.ui = numTexGens;
}

void GDSetCullMode(GXCullMode cullMode) {
    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui = 0xFE00C000;

    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui = cm2hw[cullMode] << GX_BP_GENMODE_CULLMODE_SHIFT;
}

void GDSetTexCoordScale2(GXTexCoordID coord, u16 scaleS, GXBool biasS,
                         GXBool wrapS, u16 scaleT, GXBool biasT,
                         GXBool wrapT) {
    u32 tex0 = (scaleS - 1) | (biasS << 16) | (wrapS << 17) |
               ((coord * 2 + GX_CP_REG_MATRIXINDEXA) << 24);
    u32 tex1 = (scaleT - 1) | (biasT << 16) | (wrapT << 17) |
               ((coord * 2 + GX_CP_REG_MATRIXINDEXA + 1) << 24);

    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui = 0xFE03FFFF;

    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui = tex0;

    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui = tex1;
}

void GDSetIndTexMtx(u32 id, const math::MTX34& rMtx) {
    // Top two rows of the 3x4 matrix (the offset matrix), with absolute
    // values kept alongside for the normalization search below.
    f32 m00, m01, m10, a00, m02, m11, m12, a01, a02, a10, a11, a12;

    m00 = rMtx.m[0][0];
    m01 = rMtx.m[0][1];
    a00 = __fabsf(m00);
    m02 = rMtx.m[0][2];
    a01 = __fabsf(m01);
    m10 = rMtx.m[1][0];
    m11 = rMtx.m[1][1];
    m12 = rMtx.m[1][2];
    a02 = __fabsf(m02);
    a10 = __fabsf(m10);
    a11 = __fabsf(m11);
    a12 = __fabsf(m12);

    s32 scale = 0;

    if (a00 >= 1.0f || a01 >= 1.0f ||
        a02 >= 1.0f || a10 >= 1.0f ||
        a11 >= 1.0f || a12 >= 1.0f) {
        // Scale down: halve every element until the largest |value| < 1.0f.
        do {
            if ((s8)scale >= 46) {
                break;
            }
            a00 *= 0.5f;
            m00 *= 0.5f;
            m01 *= 0.5f;
            m02 *= 0.5f;
            m10 *= 0.5f;
            m11 *= 0.5f;
            m12 *= 0.5f;
            a01 *= 0.5f;
            a02 *= 0.5f;
            a10 *= 0.5f;
            a11 *= 0.5f;
            a12 *= 0.5f;
            scale++;
        } while (a00 >= 1.0f || a01 >= 1.0f ||
                 a02 >= 1.0f || a10 >= 1.0f ||
                 a11 >= 1.0f || a12 >= 1.0f);
    } else if (a00 < 0.5f && a01 < 0.5f &&
               a02 < 0.5f && a10 < 0.5f &&
               a11 < 0.5f && a12 < 0.5f) {
        // Scale up: double every element until the largest |value| >= 0.5f.
        do {
            a00 *= 2.0f;
            m00 *= 2.0f;
            m01 *= 2.0f;
            m02 *= 2.0f;
            m10 *= 2.0f;
            m11 *= 2.0f;
            m12 *= 2.0f;
            a01 *= 2.0f;
            a02 *= 2.0f;
            a10 *= 2.0f;
            a11 *= 2.0f;
            a12 *= 2.0f;
            scale--;
        } while (a00 < 0.5f && a01 < 0.5f &&
                 a02 < 0.5f && a10 < 0.5f &&
                 a11 < 0.5f && a12 < 0.5f &&
                 (s8)scale > -17);
    }


    // Encode as three XF indirect-matrix register writes. Each word carries an
    // 11-bit fixed-point element pair plus a 2-bit slice of the 6-bit scale
    // exponent (scale + 17) and the XF register id in the top byte.
    f32 scl = 1024.0f;
    u32 exp = scale + 17;

    s32 w00 = (s32)(scl * m00);
    u32 word0 = ((id + 6) << 24) | (((exp) & 3) << 22);
    s32 w10 = (s32)(scl * m10);
    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui =
        word0 | (((u32)w00 & 0x7FF) | (((u32)w10 & 0x7FF) << 11));

    s32 w01 = (s32)(scl * m01);
    s32 w11 = (s32)(scl * m11);
    u32 word1 = ((id + 7) << 24) | ((((exp) >> 2) & 3) << 22);
    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui =
        word1 | (((u32)w01 & 0x7FF) | (((u32)w11 & 0x7FF) << 11));

    s32 w02 = (s32)(scl * m02);
    s32 w12 = (s32)(scl * m12);
    u32 word2 = ((id + 8) << 24) | ((((exp) >> 4) & 3) << 22);
    WGPIPE.uc = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.ui =
        word2 | (((u32)w02 & 0x7FF) | (((u32)w12 & 0x7FF) << 11));
}

void GDResetCurrentMtx() {
    WGPIPE.uc = GX_FIFO_CMD_LOAD_CP_REG;
    WGPIPE.uc = GX_CP_REG_MATRIXINDEXA;
    WGPIPE.ui = 0x3CF3CF00;

    WGPIPE.uc = GX_FIFO_CMD_LOAD_CP_REG;
    WGPIPE.uc = GX_CP_REG_MATRIXINDEXB;
    WGPIPE.ui = 0x00F3CF3C;

    WGPIPE.uc = GX_FIFO_CMD_LOAD_XF_REG;
    WGPIPE.us = 1;
    WGPIPE.us = GX_XF_REG_MATRIXINDEX0;
    WGPIPE.ui = 0x3CF3CF00;
    WGPIPE.ui = 0x00F3CF3C;
}

void GDSetCurrentMtx(const u32* pIdArray) {
    u32 w0 = (pIdArray[0] << 6) | (pIdArray[1] << 12) |
             (pIdArray[2] << 18) | (pIdArray[3] << 24);
    u32 w1 = pIdArray[4] | (pIdArray[5] << 6) |
             (pIdArray[6] << 12) | (pIdArray[7] << 18);

    WGPIPE.uc = GX_FIFO_CMD_LOAD_XF_REG;
    WGPIPE.us = 1;
    WGPIPE.us = GX_XF_REG_MATRIXINDEX0;
    WGPIPE.ui = w0;
    WGPIPE.ui = w1;
}

void GDLoadTexMtxImm3x3(const math::MTX33& rMtx, u32 id) {
    // Promote the 3x3 matrix to a 3x4 (identity translation column) and pass
    // it straight to GXLoadTexMtxImm.
    math::MTX34 mtx;
    mtx.m[0][0] = rMtx.m[0][0];
    mtx.m[0][1] = rMtx.m[0][1];
    mtx.m[0][2] = rMtx.m[0][2];
    mtx.m[0][3] = 0.0f;
    mtx.m[1][0] = rMtx.m[1][0];
    mtx.m[1][1] = rMtx.m[1][1];
    mtx.m[1][2] = rMtx.m[1][2];
    mtx.m[1][3] = 0.0f;
    mtx.m[2][0] = rMtx.m[2][0];
    mtx.m[2][1] = rMtx.m[2][1];
    mtx.m[2][2] = rMtx.m[2][2];
    mtx.m[2][3] = 0.0f;
    GXLoadTexMtxImm(mtx.mtx, id, GX_MTX_3x4);
}

} // namespace fifo
} // namespace g3d
} // namespace nw4r
