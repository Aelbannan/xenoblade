#include <revolution/GX.h>

extern unsigned long __cvt_fp2unsigned(double d);

void GXSetDispCopyFrame2Field(u32 field);
void GXClearBoundingBox(void);

static u32 GetNumXfbLines(u16 efbHeight, u32 stepLo) {
    u32 lines;

    lines = (((u32)(efbHeight - 1) << 8) / stepLo) + 1;

    if (stepLo >= 0x81 && stepLo <= 0xFF) {
        while (!(stepLo & 1)) {
            stepLo >>= 1;
        }

        if (!(efbHeight % stepLo)) {
            lines++;
        }
    }

    if (lines > 1024) {
        lines = 1024;
    }

    return lines;
}

void GXAdjustForOverscan(GXRenderModeObj* rmin, GXRenderModeObj* rmout,
                         u16 hor, u16 ver) {
    if (rmin != rmout) {
        *rmout = *rmin;
    }

    rmout->fbWidth = rmin->fbWidth - (hor << 1);
    rmout->efbHeight =
        rmin->efbHeight -
        (((u32)(ver << 1) * rmin->efbHeight) / rmin->xfbHeight);

    if (rmin->xFBmode == VI_XFBMODE_SF && (rmin->viTVmode & 3) == 0) {
        rmout->xfbHeight = rmin->xfbHeight - ver;
    } else {
        rmout->xfbHeight = rmin->xfbHeight - (ver << 1);
    }

    rmout->viWidth = rmin->viWidth - (hor << 1);

    if ((u32)(rmin->viTVmode & 3) == 1) {
        rmout->viHeight = rmin->viHeight - (ver << 2);
    } else {
        rmout->viHeight = rmin->viHeight - (ver << 1);
    }

    rmout->viYOrigin = rmin->viYOrigin + ver;
    rmout->viXOrigin = rmin->viXOrigin + hor;
}

void GXSetDispCopySrc(u16 x, u16 y, u16 w, u16 h) {
    u32 reg;

    reg = 0;
    reg = GX_BITSET(reg, 22, 10, x);
    reg = GX_BITSET(reg, 12, 10, y);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_TEXCOPYSRCXY);
    gxdt->cpDispSrc = reg;

    reg = 0;
    reg = GX_BITSET(reg, 22, 10, w - 1);
    reg = GX_BITSET(reg, 12, 10, h - 1);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_TEXCOPYSRCWH);
    gxdt->cpDispSize = reg;
}

void GXSetTexCopySrc(u16 x, u16 y, u16 w, u16 h) {
    u32 reg;

    reg = 0;
    reg = GX_BITSET(reg, 22, 10, x);
    reg = GX_BITSET(reg, 12, 10, y);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_TEXCOPYSRCXY);
    gxdt->cpTexSrc = reg;

    reg = 0;
    reg = GX_BITSET(reg, 22, 10, w - 1);
    reg = GX_BITSET(reg, 12, 10, h - 1);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_TEXCOPYSRCWH);
    gxdt->cpTexSize = reg;
}

void GXSetDispCopyDst(u16 w, u16 numXfbLines) {
    u16 stride;

    stride = (u16)(w << 1);
    gxdt->cpDispStride = 0;
    gxdt->cpDispStride = GX_BITSET(gxdt->cpDispStride, 22, 10, stride >> 5);
    GX_BP_SET_OPCODE(gxdt->cpDispStride, GX_BP_REG_DISPCOPYSTRIDE);
}

void GXSetTexCopyDst(u16 w, u16 h, GXTexFmt fmt, GXBool mipmap) {
    u32 rowTiles, colTiles, zTiles;
    u32 hwfmt;
    u32 reg;

    hwfmt = fmt & 0xF;

    gxdt->cpTexZ = GX_FALSE;

    if (fmt == GX_TF_Z16) {
        hwfmt = 0xB;
    }

    if ((u32)fmt <= 3 || fmt == 0x26) {
        gxdt->cpTex = GX_BITSET(gxdt->cpTex, 15, 2, 3);
    } else {
        gxdt->cpTex = GX_BITSET(gxdt->cpTex, 15, 2, 2);
    }

    gxdt->cpTexZ = (u8)((u32)fmt >> 4);
    gxdt->cpTex = GX_BITSET_TRUNC(gxdt->cpTex, 28, 1, hwfmt);

    __GetImageTileCount(fmt, w, h, &rowTiles, &colTiles, &zTiles);

    gxdt->cpTexStride = 0;
    reg = 0;
    reg = GX_BITSET(reg, 22, 10, rowTiles * zTiles);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_DISPCOPYSTRIDE);
    gxdt->cpTexStride = reg;

    gxdt->cpTex = GX_BITSET(gxdt->cpTex, 22, 1, mipmap);
    gxdt->cpTex = GX_BITSET(gxdt->cpTex, 25, 3, hwfmt);
}

void GXSetDispCopyFrame2Field(u32 field) {
    gxdt->cpDisp = GX_BITSET(gxdt->cpDisp, 18, 2, field);
    gxdt->cpTex = GX_BITSET(gxdt->cpTex, 18, 2, 0);
}

void GXSetCopyClamp(GXCopyClamp clamp) {
    gxdt->cpDisp = GX_BITSET(gxdt->cpDisp, 31, 1, clamp);
    gxdt->cpDisp = GX_BITSET_TRUNC(gxdt->cpDisp, 30, 1, clamp);
    gxdt->cpTex = GX_BITSET(gxdt->cpTex, 31, 1, clamp);
    gxdt->cpTex = GX_BITSET_TRUNC(gxdt->cpTex, 30, 1, clamp);
}

f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight) {
    f32 yscale;
    f32 bestyscale;
    u32 lines;
    u16 height;

    yscale = (f32)xfbHeight / (f32)efbHeight;
    height = xfbHeight;
    lines = GetNumXfbLines(efbHeight, __cvt_fp2unsigned(256.0f / yscale) & 0x1FF);

    while (lines > xfbHeight) {
        height--;
        yscale = (f32)height / (f32)efbHeight;
        lines = GetNumXfbLines(efbHeight, __cvt_fp2unsigned(256.0f / yscale) & 0x1FF);
    }

    bestyscale = yscale;

    while (lines < xfbHeight) {
        height++;
        bestyscale = yscale;
        yscale = (f32)height / (f32)efbHeight;
        lines = GetNumXfbLines(efbHeight, __cvt_fp2unsigned(256.0f / yscale) & 0x1FF);
    }

    return bestyscale;
}

void GXSetDispCopyYScale(f32 scaleY) {
    u32 reg;
    u32 step;
    u32 stepLo;
    u32 lines;
    u32 efbHeight;

    step = __cvt_fp2unsigned(256.0f / scaleY);
    stepLo = step & 0x1FF;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    reg = 0;
    reg = GX_BITSET(reg, 23, 9, step);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_DISPCOPYSCALEY);
    WGPIPE.i = reg;

    gxdt->lastWriteWasXF = FALSE;
    gxdt->cpDisp = GX_BITSET(gxdt->cpDisp, 21, 1, stepLo != 0x100);

    efbHeight = GX_BITGET(gxdt->cpDispSize, 10, 12) + 1;
    lines = GetNumXfbLines((u16)efbHeight, stepLo);
    (void)lines;
}

void GXSetCopyClear(GXColor color, u32 z) {
    u32 reg;

    reg = 0;
    reg = GX_BITSET(reg, 24, 8, color.r);
    reg = GX_BITSET(reg, 16, 8, color.a);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_COPYCLEARAR);
    GX_BP_LOAD_REG(reg);

    reg = 0;
    reg = GX_BITSET(reg, 24, 8, color.b);
    reg = GX_BITSET(reg, 16, 8, color.g);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_COPYCLEARGB);
    GX_BP_LOAD_REG(reg);

    reg = 0;
    reg = GX_BITSET(reg, 8, 24, z);
    GX_BP_SET_OPCODE(reg, GX_BP_REG_COPYCLEARZ);
    GX_BP_LOAD_REG(reg);

    gxdt->lastWriteWasXF = FALSE;
}

void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf,
                     const u8 vfilter[GX_VFILTER_SZ]) {
    u32 reg0, reg1, reg2, reg3;
    u32 cf0, cf1;

    if (aa) {
        reg0 = 0;
        reg0 = GX_BITSET(reg0, 28, 4, sample_pattern[0][0]);
        reg0 = GX_BITSET(reg0, 24, 4, sample_pattern[0][1]);
        reg0 = GX_BITSET(reg0, 20, 4, sample_pattern[1][0]);
        reg0 = GX_BITSET(reg0, 16, 4, sample_pattern[1][1]);
        reg0 = GX_BITSET(reg0, 12, 4, sample_pattern[2][0]);
        reg0 = GX_BITSET(reg0, 8, 4, sample_pattern[2][1]);
        GX_BP_SET_OPCODE(reg0, GX_BP_REG_DISPCOPYFILTER0);

        reg1 = 0;
        reg1 = GX_BITSET(reg1, 28, 4, sample_pattern[3][0]);
        reg1 = GX_BITSET(reg1, 24, 4, sample_pattern[3][1]);
        reg1 = GX_BITSET(reg1, 20, 4, sample_pattern[4][0]);
        reg1 = GX_BITSET(reg1, 16, 4, sample_pattern[4][1]);
        reg1 = GX_BITSET(reg1, 12, 4, sample_pattern[5][0]);
        reg1 = GX_BITSET(reg1, 8, 4, sample_pattern[5][1]);
        GX_BP_SET_OPCODE(reg1, GX_BP_REG_DISPCOPYFILTER1);

        reg2 = 0;
        reg2 = GX_BITSET(reg2, 28, 4, sample_pattern[6][0]);
        reg2 = GX_BITSET(reg2, 24, 4, sample_pattern[6][1]);
        reg2 = GX_BITSET(reg2, 20, 4, sample_pattern[7][0]);
        reg2 = GX_BITSET(reg2, 16, 4, sample_pattern[7][1]);
        reg2 = GX_BITSET(reg2, 12, 4, sample_pattern[8][0]);
        reg2 = GX_BITSET(reg2, 8, 4, sample_pattern[8][1]);
        GX_BP_SET_OPCODE(reg2, GX_BP_REG_DISPCOPYFILTER2);

        reg3 = 0;
        reg3 = GX_BITSET(reg3, 28, 4, sample_pattern[9][0]);
        reg3 = GX_BITSET(reg3, 24, 4, sample_pattern[9][1]);
        reg3 = GX_BITSET(reg3, 20, 4, sample_pattern[10][0]);
        reg3 = GX_BITSET(reg3, 16, 4, sample_pattern[10][1]);
        reg3 = GX_BITSET(reg3, 12, 4, sample_pattern[11][0]);
        reg3 = GX_BITSET(reg3, 8, 4, sample_pattern[11][1]);
        GX_BP_SET_OPCODE(reg3, GX_BP_REG_DISPCOPYFILTER3);
    } else {
        reg0 = 0x01666666;
        reg1 = 0x02666666;
        reg2 = 0x03666666;
        reg3 = 0x04666666;
    }

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg0;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg1;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg2;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg3;

    cf0 = 0;
    cf1 = 0;
    GX_BP_SET_OPCODE(cf0, GX_BP_REG_COPYFILTER0);
    GX_BP_SET_OPCODE(cf1, GX_BP_REG_COPYFILTER1);

    if (vf) {
        cf0 = GX_BITSET(cf0, 26, 6, vfilter[0]);
        cf0 = GX_BITSET(cf0, 20, 6, vfilter[1]);
        cf0 = GX_BITSET(cf0, 14, 6, vfilter[2]);
        cf0 = GX_BITSET(cf0, 8, 6, vfilter[3]);
        cf1 = GX_BITSET(cf1, 26, 6, vfilter[4]);
        cf1 = GX_BITSET(cf1, 20, 6, vfilter[5]);
        cf1 = GX_BITSET(cf1, 14, 6, vfilter[6]);
    } else {
        cf0 = GX_BITSET(cf0, 26, 6, 0);
        cf0 = GX_BITSET(cf0, 20, 6, 0);
        cf0 = GX_BITSET(cf0, 14, 6, 21);
        cf0 = GX_BITSET(cf0, 8, 6, 22);
        cf1 = GX_BITSET(cf1, 26, 6, 21);
        cf1 = GX_BITSET(cf1, 20, 6, 0);
        cf1 = GX_BITSET(cf1, 14, 6, 0);
    }

    GX_BP_LOAD_REG(cf0);
    GX_BP_LOAD_REG(cf1);

    gxdt->lastWriteWasXF = FALSE;
}

void GXSetDispCopyGamma(u32 gamma) {
    gxdt->cpDisp = GX_BITSET(gxdt->cpDisp, 23, 2, gamma);
}

void GXCopyDisp(void* dest, GXBool clear) {
    u32 reg;
    GXBool restoreZ;
    u32 zm;

    if (clear) {
        zm = gxdt->zMode;
        WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
        WGPIPE.i = zm | 0xF;
        zm = gxdt->blendMode;
        WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
        WGPIPE.i = zm & ~0x3;
    }

    restoreZ = GX_FALSE;
    if (clear || (gxdt->zControl & 0x7) == 0x3) {
        u32 zc = gxdt->zControl;
        if (GX_BITGET(zc, 25, 1) == 0x1) {
            GX_BP_LOAD_REG(zc & ~0x40);
            restoreZ = GX_TRUE;
        }
    }

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    reg = 0;
    reg = GX_BITSET(reg, 8, 24, (u32)dest >> 5);
    WGPIPE.i = gxdt->cpDispSrc;
    GX_BP_SET_OPCODE(reg, GX_BP_REG_TEXCOPYDST);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = gxdt->cpDispSize;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = gxdt->cpDispStride;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;

    gxdt->cpDisp = GX_BITSET(gxdt->cpDisp, 20, 1, clear);
    gxdt->cpDisp |= 0x4000;
    GX_BP_SET_OPCODE(gxdt->cpDisp, 0x52);
    GX_BP_LOAD_REG(gxdt->cpDisp);

    if (clear) {
        GX_BP_LOAD_REG(gxdt->zMode);
        GX_BP_LOAD_REG(gxdt->blendMode);
    }

    if (restoreZ) {
        GX_BP_LOAD_REG(gxdt->zControl);
    }

    gxdt->lastWriteWasXF = FALSE;
}

void GXCopyTex(void* dest, GXBool clear) {
    u32 reg;
    GXBool restoreZ;
    u32 zControl;
    u32 zm;

    if (clear) {
        zm = gxdt->zMode;
        WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
        WGPIPE.i = zm | 0xF;
        zm = gxdt->blendMode;
        WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
        WGPIPE.i = zm & ~0x3;
    }

    restoreZ = GX_FALSE;

    zControl = gxdt->zControl;

    if (gxdt->cpTexZ && (zControl & 0x7) != 0x3) {
        zControl = GX_BITSET(zControl, 29, 3, 3);
        restoreZ = GX_TRUE;
    }

    if (clear || (zControl & 0x7) == 0x3) {
        if (GX_BITGET(zControl, 25, 1) == 0x1) {
            restoreZ = GX_TRUE;
            zControl &= ~0x40;
        }
    }

    if (restoreZ) {
        GX_BP_LOAD_REG(zControl);
    }

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    reg = 0;
    reg = GX_BITSET(reg, 8, 24, (u32)dest >> 5);
    WGPIPE.i = gxdt->cpTexSrc;
    GX_BP_SET_OPCODE(reg, GX_BP_REG_TEXCOPYDST);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = gxdt->cpTexSize;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = gxdt->cpTexStride;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;

    gxdt->cpTex = GX_BITSET(gxdt->cpTex, 20, 1, clear);
    gxdt->cpTex = GX_BITSET(gxdt->cpTex, 17, 1, 0);
    GX_BP_SET_OPCODE(gxdt->cpTex, 0x52);
    GX_BP_LOAD_REG(gxdt->cpTex);

    if (clear) {
        GX_BP_LOAD_REG(gxdt->zMode);
        GX_BP_LOAD_REG(gxdt->blendMode);
    }

    if (restoreZ) {
        GX_BP_LOAD_REG(gxdt->zControl);
    }

    gxdt->lastWriteWasXF = FALSE;
}

void GXClearBoundingBox(void) {
    GX_BP_LOAD_REG((GX_BP_REG_BOUNDINGBOX0 << 24) | 0x3FF);
    GX_BP_LOAD_REG((GX_BP_REG_BOUNDINGBOX1 << 24) | 0x3FF);
    gxdt->lastWriteWasXF = FALSE;
}

GXRenderModeObj GXNtsc240Ds = {
    VI_TVMODE_NTSC_DS, 640, 240, 240, 40, 0, 640, 480, VI_XFBMODE_SF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXNtsc480IntDf = {
    VI_TVMODE_NTSC_INT, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_DF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 8, 10, 12, 10, 8, 8}};

GXRenderModeObj GXNtsc480Int = {
    VI_TVMODE_NTSC_INT, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_DF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXNtsc480Prog = {
    VI_TVMODE_NTSC_PROG, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_SF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXNtsc480ProgSoft = {
    VI_TVMODE_NTSC_PROG, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_SF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 8, 10, 12, 10, 8, 8}};

GXRenderModeObj GXMpal240Ds = {
    VI_TVMODE_MPAL_DS, 640, 240, 240, 40, 0, 640, 480, VI_XFBMODE_SF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXMpal480IntDf = {
    VI_TVMODE_MPAL_INT, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_DF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 8, 10, 12, 10, 8, 8}};

GXRenderModeObj GXMpal480Int = {
    VI_TVMODE_MPAL_INT, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_DF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXMpal480Prog = {
    VI_TVMODE_MPAL_PROG, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_SF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXMpal480ProgSoft = {
    VI_TVMODE_MPAL_PROG, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_SF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 8, 10, 12, 10, 8, 8}};

GXRenderModeObj GXPal264Ds = {
    VI_TVMODE_PAL_DS, 640, 264, 264, 40, 11, 640, 528, VI_XFBMODE_SF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXPal528IntDf = {
    VI_TVMODE_PAL_INT, 640, 528, 528, 40, 23, 640, 528, VI_XFBMODE_DF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 8, 10, 12, 10, 8, 8}};

GXRenderModeObj GXPal528Int = {
    VI_TVMODE_PAL_INT, 640, 528, 528, 40, 23, 640, 528, VI_XFBMODE_DF, GX_FALSE,
    GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXEurgb60Hz240Ds = {
    VI_TVMODE_EURGB60_DS, 640, 240, 240, 40, 0, 640, 480, VI_XFBMODE_SF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXEurgb60Hz480IntDf = {
    VI_TVMODE_EURGB60_INT, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_DF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 8, 10, 12, 10, 8, 8}};

GXRenderModeObj GXEurgb60Hz480Int = {
    VI_TVMODE_EURGB60_INT, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_DF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXEurgb60Hz480Prog = {
    VI_TVMODE_EURGB60_PROG, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_SF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {0, 0, 21, 22, 21, 0, 0}};

GXRenderModeObj GXEurgb60Hz480ProgSoft = {
    VI_TVMODE_EURGB60_PROG, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_SF,
    GX_FALSE, GX_FALSE,
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {8, 8, 10, 12, 10, 8, 8}};
