// Decompiled: RVL_SDK/src/revolution/gx/GXTev.c
// High-level C reconstruction matching retail Wii GX TEV API.
#include <revolution/GX.h>

// TEV op tables indexed by GXTevMode (5 simple modes: ADD..BLEND).
// Layout (retail .data order): ST0 color, ST1 color, ST0 alpha, ST1 alpha.
u32 TEVCOpTableST0[5] = {0xC008F8AF, 0xC008A89F, 0xC008AC8F, 0xC008FFF8, 0xC008FFFA};
u32 TEVCOpTableST1[5] = {0xC008F80F, 0xC008089F, 0xC0080C8F, 0xC008FFF8, 0xC008FFF0};
u32 TEVAOpTableST0[5] = {0xC108F2F0, 0xC108FFD0, 0xC108F2F0, 0xC108FFC0, 0xC108FFD0};
u32 TEVAOpTableST1[5] = {0xC108F070, 0xC108FF80, 0xC108F070, 0xC108FFC0, 0xC108FF80};

void GXSetNumTevStages(u8 nStages)
{
    u32 reg = __GXData->genMode;
    reg = __rlwimi(reg, nStages - 1, 10, 18, 21);
    __GXData->genMode = reg;
    __GXData->gxDirtyFlags |= 4;
}

void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp op,
                       GXCompare comp1, u8 ref1)
{
    u32 reg = 0xF3000000;
    reg = __rlwimi(reg, ref0,  0, 24, 31);
    reg = __rlwimi(reg, ref1,  8, 16, 23);
    reg = __rlwimi(reg, comp0, 16, 13, 15);
    reg = __rlwimi(reg, comp1, 19, 10, 12);
    reg = __rlwimi(reg, op,    22,  8,  9);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;
    __GXData->lastWriteWasXF = 0;
}

void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                     GXTevColorArg c, GXTevColorArg d)
{
    u32 idx = (u32)stage;
    u32 reg = __GXData->tevc[idx];
    reg = __rlwimi(reg, (u32)a, 12, 16, 19);
    reg = __rlwimi(reg, (u32)b,  8, 20, 23);
    reg = __rlwimi(reg, (u32)c,  4, 24, 27);
    reg = __rlwimi(reg, (u32)d,  0, 28, 31);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;
    __GXData->tevc[idx] = reg;
    __GXData->lastWriteWasXF = 0;
}

void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b,
                     GXTevAlphaArg c, GXTevAlphaArg d)
{
    u32 idx = (u32)stage;
    u32 reg = __GXData->teva[idx];
    reg = __rlwimi(reg, (u32)a, 13, 16, 18);
    reg = __rlwimi(reg, (u32)b, 10, 19, 21);
    reg = __rlwimi(reg, (u32)c,  7, 22, 24);
    reg = __rlwimi(reg, (u32)d,  4, 25, 27);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;
    __GXData->teva[idx] = reg;
    __GXData->lastWriteWasXF = 0;
}

void GXSetTevSwapMode(GXTevStageID stage, GXTevSwapSel rasSel, GXTevSwapSel texSel)
{
    u32 idx = (u32)stage;
    u32 *ptr = &__GXData->teva[idx];
    u32 reg = *ptr;
    reg = __rlwimi(reg, (u32)rasSel, 0, 30, 31);
    reg = __rlwimi(reg, (u32)texSel, 2, 28, 29);
    *ptr = reg;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = *ptr;
    __GXData->lastWriteWasXF = 0;
}

void GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel)
{
    u32 idx = (u32)stage;
    u32 *ptr = &__GXData->tevKsel[idx >> 1];
    u32 reg = *ptr;
    if (stage & 1) {
        reg = __rlwimi(reg, (u32)sel, 14, 13, 17);
    } else {
        reg = __rlwimi(reg, (u32)sel,  4, 23, 27);
    }
    *ptr = reg;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;
    __GXData->lastWriteWasXF = 0;
}

void GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel)
{
    u32 idx = (u32)stage;
    u32 *ptr = &__GXData->tevKsel[idx >> 1];
    u32 reg = *ptr;
    if (stage & 1) {
        reg = __rlwimi(reg, (u32)sel, 19,  8, 12);
    } else {
        reg = __rlwimi(reg, (u32)sel,  9, 18, 22);
    }
    *ptr = reg;
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;
    __GXData->lastWriteWasXF = 0;
}

void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias,
                     GXTevScale scale, GXBool clamp, GXTevRegID regID)
{
    u32 idx = (u32)stage;
    u32 reg = __GXData->tevc[idx];
    reg = __rlwimi(reg, (u32)op, 18, 13, 13);
    if ((s32)op <= 1) {
        reg = __rlwimi(reg, (u32)scale, 20, 10, 11);
        reg = __rlwimi(reg, (u32)bias,  16, 14, 15);
    } else {
        reg = __rlwimi(reg, (u32)op, 19, 10, 11);
        reg |= (3 << 16);
    }
    reg = __rlwimi(reg, (u32)clamp, 19, 12, 12);
    reg = __rlwimi(reg, (u32)regID, 22,  8,  9);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;
    __GXData->tevc[idx] = reg;
    __GXData->lastWriteWasXF = 0;
}

void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias,
                     GXTevScale scale, GXBool clamp, GXTevRegID regID)
{
    u32 idx = (u32)stage;
    u32 reg = __GXData->teva[idx];
    reg = __rlwimi(reg, (u32)op, 18, 13, 13);
    if ((s32)op <= 1) {
        reg = __rlwimi(reg, (u32)scale, 20, 10, 11);
        reg = __rlwimi(reg, (u32)bias,  16, 14, 15);
    } else {
        reg = __rlwimi(reg, (u32)op, 19, 10, 11);
        reg |= (3 << 16);
    }
    reg = __rlwimi(reg, (u32)clamp, 19, 12, 12);
    reg = __rlwimi(reg, (u32)regID, 22,  8,  9);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;
    __GXData->teva[idx] = reg;
    __GXData->lastWriteWasXF = 0;
}

void GXSetTevColor(GXTevRegID reg, GXColor color)
{
    u32 w1;
    u32 col = *(const u32 *)&color;
    u32 base = (u32)reg * 2;
    u32 w0 = ((0xE0 + base) << 24);
    w0 = __rlwimi(w0, col,  8, 24, 31);
    w0 = __rlwimi(w0, col, 12, 12, 19);

    w1 = ((0xE1 + base) << 24);
    w1 = __rlwimi(w1, col, 24, 24, 31);
    w1 = __rlwimi(w1, col, 28, 12, 19);

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w0;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w1;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w1;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w1;

    __GXData->lastWriteWasXF = 0;
}

void GXSetTevColorS10(GXTevRegID reg, GXColorS10 color)
{
    u32 lo = *(const u32 *)&color;
    u32 hi = *(((const u32 *)&color) + 1);
    u32 base = (u32)reg * 2;
    u32 addr0 = 0xE0 + base;
    u32 addr1 = 0xE1 + base;
    u32 w0, w1;

    w0 = (addr0 << 24);
    w0 = __rlwimi(w0, lo, 16, 21, 31);
    w0 = __rlwimi(w0, hi, 12,  9, 19);

    w1 = (addr1 << 24);
    w1 = __rlwimi(w1, hi, 16, 21, 31);
    w1 = __rlwimi(w1, lo, 12,  9, 19);

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w0;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w1;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w1;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w1;

    __GXData->lastWriteWasXF = 0;
}

void GXSetTevKColor(GXTevKColorID id, GXColor color)
{
    u32 col = *(const u32 *)&color;
    u32 base = (u32)id * 2;
    u32 addr0 = 0xE0 + base;
    u32 addr1 = 0xE1 + base;
    u32 w0, w1;

    w0 = (addr0 << 24);
    w0 = __rlwimi(w0, col,  8, 24, 31);
    w0 = __rlwimi(w0, col, 12, 12, 19);
    w0 = __rlwimi(w0, 8,    20,  8, 11);

    w1 = (addr1 << 24);
    w1 = __rlwimi(w1, col, 24, 24, 31);
    w1 = __rlwimi(w1, col, 28, 12, 19);
    w1 = __rlwimi(w1, 8,    20,  8, 11);

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w0;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w1;

    __GXData->lastWriteWasXF = 0;
}

void GXSetTevSwapModeTable(GXTevSwapSel sel, GXTevColorChan r, GXTevColorChan g,
                           GXTevColorChan b, GXTevColorChan a)
{
    u32 *basePtr = &__GXData->tevKsel[0];
    u32 base = (u32)sel * 2;
    u32 reg;

    reg = basePtr[base + 0];
    reg = __rlwimi(reg, (u32)r, 0, 30, 31);
    reg = __rlwimi(reg, (u32)g, 2, 28, 29);
    basePtr[base + 0] = reg;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;

    reg = basePtr[base + 1];
    reg = __rlwimi(reg, (u32)b, 0, 30, 31);
    reg = __rlwimi(reg, (u32)a, 2, 28, 29);
    basePtr[base + 1] = reg;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = reg;

    __GXData->lastWriteWasXF = 0;
}

void GXSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias)
{
    u32 w0, w1;
    u32 ztype;

    if (fmt == 0x11) {
        ztype = 0;
    } else if (fmt == 0x13) {
        ztype = 1;
    } else if (fmt == 0x16) {
        ztype = 2;
    } else {
        ztype = 2;
    }

    w0 = 0;
    w0 = __rlwimi(w0, bias, 0, 8, 31);
    w0 = __rlwimi(w0, 0xF4, 24, 0, 7);

    w1 = 0;
    w1 = __rlwimi(w1, ztype, 0, 30, 31);
    w1 = __rlwimi(w1, (u32)op, 2, 28, 29);
    w1 = __rlwimi(w1, 0xF5, 24, 0, 7);

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w0;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = w1;

    __GXData->lastWriteWasXF = 0;
}

void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map,
                   GXChannelID channel)
{
    static const u32 c2r[10] = { 0, 1, 0, 1, 0, 1, 7, 5, 6, 0 };
    u32 idx = (u32)stage;
    u32 texMapVal = (u32)map & 0x7F;
    u32 texCoordVal = (u32)coord;
    u32 chanVal = (u32)channel;
    u32 trefReg, ccSel;
    u32 saturated;

    __GXData->texmapId[idx] = (u32)map;

    /* Compute texmap-saturated value: clamped to valid range */
    {
        u32 tmp = texMapVal;
        u32 carry = (tmp >= 8) ? 1 : 0;
        u32 r0 = 8 + carry;
        saturated = texMapVal & ~(8 - r0);
    }

    /* Enable/disable TC based on coord */
    if (texCoordVal < 8) {
        __GXData->tevTcEnab |= (1 << idx);
    } else {
        __GXData->tevTcEnab &= ~(1 << idx);
    }

    trefReg = __GXData->tref[idx >> 1];

    if (stage & 1) {
        u32 r0;
        trefReg = __rlwimi(trefReg, saturated,    12, 17, 19);
        trefReg = __rlwimi(trefReg, texCoordVal,  15, 14, 16);

        if (chanVal == 0xFF) {
            r0 = 7;
        } else {
            r0 = c2r[chanVal];
        }
        trefReg = __rlwimi(trefReg, r0, 19,  9, 11);

        if (texMapVal == 0xFF) {
            ccSel = 0;
        } else {
            ccSel = (texMapVal & 0x100) ? 0 : 1;
        }
        trefReg = __rlwimi(trefReg, ccSel, 18, 13, 13);
    } else {
        u32 r0;
        trefReg = __rlwimi(trefReg, saturated,    0, 29, 31);
        trefReg = __rlwimi(trefReg, texCoordVal,  3, 26, 28);

        if (chanVal == 0xFF) {
            r0 = 7;
        } else {
            r0 = c2r[chanVal];
        }
        trefReg = __rlwimi(trefReg, r0, 7, 22, 24);

        if (texMapVal == 0xFF) {
            ccSel = 0;
        } else {
            ccSel = (texMapVal & 0x100) ? 0 : 1;
        }
        trefReg = __rlwimi(trefReg, ccSel, 6, 25, 25);
    }

    __GXData->tref[idx >> 1] = trefReg;

    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = trefReg;

    __GXData->lastWriteWasXF = 0;
    __GXData->gxDirtyFlags |= 1;
}

void GXSetTevOp(GXTevStageID stage, GXTevMode mode)
{
    u32 *ctmp, *atmp;
    u32 idx = (u32)stage;
    u32 tevc, teva;

    if (stage == 0) {
        ctmp = TEVCOpTableST0 + (u32)mode;
        atmp = TEVAOpTableST0 + (u32)mode;
    } else {
        ctmp = TEVCOpTableST1 + (u32)mode;
        atmp = TEVAOpTableST1 + (u32)mode;
    }

    tevc = __GXData->tevc[idx];
    tevc = (*ctmp & ~0xFF000000) | (tevc & 0xFF000000);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = tevc;
    __GXData->tevc[idx] = tevc;

    teva = __GXData->teva[idx];
    teva = (*atmp & ~0xFF00000F) | (teva & 0xFF00000F);
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;
    WGPIPE.i = teva;
    __GXData->teva[idx] = teva;

    __GXData->lastWriteWasXF = 0;
}
