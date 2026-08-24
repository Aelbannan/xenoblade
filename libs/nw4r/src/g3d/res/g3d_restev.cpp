#include <nw4r/g3d.h>

#include <revolution/BASE.h>
#include <revolution/GX.h>

namespace nw4r {
namespace g3d {

bool ResTev::GXGetTevSwapModeTable(GXTevSwapSel swap, GXTevColorChan* pR,
                                   GXTevColorChan* pG, GXTevColorChan* pB,
                                   GXTevColorChan* pA) const {
    const u8* pCmd = ref().dl.dl.common.dl.swapModeTable[swap];
    u32 cmd;

    if (pCmd[0] == 0) {
        return false;
    }

    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 1], &cmd);

    if (pR != NULL) {
        *pR = static_cast<GXTevColorChan>(cmd >> GX_BP_TEVKSEL_SWAP_RB_SHIFT &
                                          GX_BP_TEVKSEL_SWAP_RB_LMASK);
    }
    if (pG != NULL) {
        *pG = static_cast<GXTevColorChan>(cmd >> GX_BP_TEVKSEL_SWAP_GA_SHIFT &
                                          GX_BP_TEVKSEL_SWAP_GA_LMASK);
    }

    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 3], &cmd);

    if (pB != NULL) {
        *pB = static_cast<GXTevColorChan>(cmd >> GX_BP_TEVKSEL_SWAP_RB_SHIFT &
                                          GX_BP_TEVKSEL_SWAP_RB_LMASK);
    }
    if (pA != NULL) {
        *pA = static_cast<GXTevColorChan>(cmd >> GX_BP_TEVKSEL_SWAP_GA_SHIFT &
                                          GX_BP_TEVKSEL_SWAP_GA_LMASK);
    }

    return true;
}

void ResTev::GXSetTevSwapModeTable(GXTevSwapSel swap, GXTevColorChan r,
                                   GXTevColorChan g, GXTevColorChan b,
                                   GXTevColorChan a) {
    u8* pCmd = ref().dl.dl.common.dl.swapModeTable[swap];
    u32 cmd;

    // clang-format off
    detail::ResWriteSSMask(&pCmd[GX_BP_CMD_SZ * 0],
                           GX_BP_TEVKSEL_SWAP_RB_MASK | GX_BP_TEVKSEL_SWAP_GA_MASK);

    cmd = 0;
    cmd |= r << GX_BP_TEVKSEL_SWAP_RB_SHIFT;
    cmd |= g << GX_BP_TEVKSEL_SWAP_GA_SHIFT;
    cmd |= (swap * 2 + GX_BP_REG_TEVKSEL0) << GX_BP_OPCODE_SHIFT;

    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 1], cmd,
        ~(GX_BP_TEVKSEL_KASEL_ODD_MASK | GX_BP_TEVKSEL_KCSEL_ODD_MASK |
          GX_BP_TEVKSEL_KASEL_EVEN_MASK | GX_BP_TEVKSEL_KCSEL_EVEN_MASK));

    detail::ResWriteSSMask(&pCmd[GX_BP_CMD_SZ * 2],
                           GX_BP_TEVKSEL_SWAP_RB_MASK | GX_BP_TEVKSEL_SWAP_GA_MASK);

    cmd = 0;
    cmd |= b << GX_BP_TEVKSEL_SWAP_RB_SHIFT;
    cmd |= a << GX_BP_TEVKSEL_SWAP_GA_SHIFT;
    cmd |= (swap * 2 + GX_BP_REG_TEVKSEL1) << GX_BP_OPCODE_SHIFT;

    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 3], cmd,
        ~(GX_BP_TEVKSEL_KASEL_ODD_MASK | GX_BP_TEVKSEL_KCSEL_ODD_MASK |
          GX_BP_TEVKSEL_KASEL_EVEN_MASK | GX_BP_TEVKSEL_KCSEL_EVEN_MASK));
    // clang-format on
}

bool ResTev::GXGetTevOrder(GXTevStageID stage, GXTexCoordID* pCoord,
                           GXTexMapID* pMap, GXChannelID* pChannel) const {
    // Convert RAS channel ID to GX channel ID
    static const GXChannelID r2c[GX_RAS_MAX_CHANNEL] = {
        GX_COLOR0A0,   GX_COLOR1A1,   GX_COLOR_NULL,  GX_COLOR_NULL,
        GX_COLOR_NULL, GX_ALPHA_BUMP, GX_ALPHA_BUMPN, GX_COLOR_ZERO};

    const u8* pCmd = ref().dl.dl.var[stage / TEV_STAGES_PER_DL].dl.tevOrder;

    if (pCmd[0] == 0) {
        return false;
    }

    u32 cmd;
    detail::ResReadBPCmd(pCmd, &cmd);

    bool enabled;
    GXTexCoordID coord;
    GXTexMapID map;
    GXChannelID channel;

    if (stage & 1) {
        channel = r2c[cmd >> GX_BP_RAS1_TREF_COLORCHAN_ODD_SHIFT &
                      GX_BP_RAS1_TREF_COLORCHAN_ODD_LMASK];

        coord = static_cast<GXTexCoordID>(
            cmd >> GX_BP_RAS1_TREF_TEXCOORD_ODD_SHIFT &
            GX_BP_RAS1_TREF_TEXCOORD_ODD_LMASK);

        enabled = cmd >> GX_BP_RAS1_TREF_ENABLE_TEX_ODD_SHIFT &
                  GX_BP_RAS1_TREF_ENABLE_TEX_ODD_LMASK;

        map = static_cast<GXTexMapID>(cmd >> GX_BP_RAS1_TREF_TEXMAP_ODD_SHIFT &
                                      GX_BP_RAS1_TREF_TEXMAP_ODD_LMASK);
    } else {
        channel = r2c[cmd >> GX_BP_RAS1_TREF_COLORCHAN_EVEN_SHIFT &
                      GX_BP_RAS1_TREF_COLORCHAN_EVEN_LMASK];

        coord = static_cast<GXTexCoordID>(
            cmd >> GX_BP_RAS1_TREF_TEXCOORD_EVEN_SHIFT &
            GX_BP_RAS1_TREF_TEXCOORD_EVEN_LMASK);

        enabled = cmd >> GX_BP_RAS1_TREF_ENABLE_TEX_EVEN_SHIFT &
                  GX_BP_RAS1_TREF_ENABLE_TEX_EVEN_LMASK;

        map = static_cast<GXTexMapID>(cmd >> GX_BP_RAS1_TREF_TEXMAP_EVEN_SHIFT &
                                      GX_BP_RAS1_TREF_TEXMAP_EVEN_LMASK);
    }

    if (pCoord != NULL) {
        *pCoord = coord;
    }

    if (pChannel != NULL) {
        *pChannel = channel;
    }

    if (!enabled) {
        map = GX_TEXMAP_NULL;
    }

    if (pMap) {
        *pMap = map;
    }

    return true;
}

void ResTev::GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a,
                             GXTevColorArg b, GXTevColorArg c,
                             GXTevColorArg d) {
    u8* pCmd = ref()
                   .dl.dl.var[stage / TEV_STAGES_PER_DL]
                   .dl.tevColorCalc[stage % TEV_STAGES_PER_DL];

    // clang-format off
    detail::ResWriteBPCmd(pCmd,
        (d << GX_BP_TEVCOLORCOMBINER_D_SHIFT) |
        (c << GX_BP_TEVCOLORCOMBINER_C_SHIFT) |
        (b << GX_BP_TEVCOLORCOMBINER_B_SHIFT) |
        (a << GX_BP_TEVCOLORCOMBINER_A_SHIFT) |
        ((GX_BP_REG_TEVCOLORCOMBINER0 + stage * 2)
            << GX_BP_OPCODE_SHIFT),

        ~(GX_BP_TEVCOLORCOMBINER_DEST_MASK |
          GX_BP_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE_MASK |
          GX_BP_TEVCOLORCOMBINER_CLAMP_MASK |
          GX_BP_TEVCOLORCOMBINER_OP_OR_COMPARISON_MASK |
          GX_BP_TEVCOLORCOMBINER_BIAS_MASK));
    // clang-format on
}

void ResTev::CallDisplayList(bool sync) const {
    // Variable DL holds data for two GX tev stages
    static const u32 dlsize[GX_MAX_TEVSTAGE] = {
        ROUND_UP(sizeof(ResTevCommonDL) + 1 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 1 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 2 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 2 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 3 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 3 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 4 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 4 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 5 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 5 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 6 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 6 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 7 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 7 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 8 * sizeof(ResTevVariableDL), 32),
        ROUND_UP(sizeof(ResTevCommonDL) + 8 * sizeof(ResTevVariableDL), 32)};

    if (sync) {
        PPCSync();
    }

    GXCallDisplayList(const_cast<ResTevDL*>(&ref().dl),
                      dlsize[GetNumTevStages() - 1]);
}

ResTev ResTev::CopyTo(void* pDst) {
    const ResTevData* pSrc = &ref();
    detail::Copy32ByteBlocks(pDst, pSrc, sizeof(ResTevData));

    ResTev tev(pDst);
    tev.ref().toResMdlData -= reinterpret_cast<std::uintptr_t>(pDst) -
                              reinterpret_cast<std::uintptr_t>(pSrc);

    tev.DCStore(false);
    return tev;
}

void ResTev::DCStore(bool sync) {
    ResTevData* pBase = &ref();
    u32 size = ref().size;

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void ResTev::GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a,
                             GXTevAlphaArg b, GXTevAlphaArg c,
                             GXTevAlphaArg d) {
    u8* pCmd = ref()
                   .dl.dl.var[stage / TEV_STAGES_PER_DL]
                   .dl.alphaCalcAndSwap[stage % TEV_STAGES_PER_DL];

    // clang-format off
    detail::ResWriteBPCmd(pCmd,
        (d << 4) | (c << 7) | (b << 10) | (a << 13) |
        ((GX_BP_REG_TEVALPHACOMBINER0 + stage * 2)
            << GX_BP_OPCODE_SHIFT),

        ~(GX_BP_TEVCOLORCOMBINER_DEST_MASK |
          GX_BP_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE_MASK |
          GX_BP_TEVCOLORCOMBINER_CLAMP_MASK |
          GX_BP_TEVCOLORCOMBINER_OP_OR_COMPARISON_MASK |
          GX_BP_TEVCOLORCOMBINER_BIAS_MASK |
          0xF));
    // clang-format on
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void ResTev::GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias,
                             GXTevScale scale, GXBool clamp, GXTevRegID reg) {
    u8* pCmd = ref()
                   .dl.dl.var[stage / TEV_STAGES_PER_DL]
                   .dl.alphaCalcAndSwap[stage % TEV_STAGES_PER_DL];

    u32 cmd;
    if (op <= 1) {
        cmd = (clamp << 19) | (bias << 16) | ((op & 1) << 18) |
              ((stage * 2 + GX_BP_REG_TEVALPHACOMBINER0) << GX_BP_OPCODE_SHIFT);
        cmd = (reg << 22) | (scale << 20) | cmd;
    } else {
        cmd = (clamp << 19) | (3 << 16);
        cmd = (cmd & ~(1 << 18)) | ((op & 1) << 18);
        cmd = (cmd & ~(3 << 20)) | (((op >> 1) & 3) << 20);
        cmd = (cmd & ~(0x3FF << 22)) | (reg << 22);
    }
    cmd |= (stage * 2 + GX_BP_REG_TEVALPHACOMBINER0) << GX_BP_OPCODE_SHIFT;

    detail::ResWriteBPCmd(pCmd, cmd, 0xFFFF0000);
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void ResTev::GXSetTevKColorSel(GXTevStageID stage, GXTevKColorSel sel) {
    u32 n = stage / 2;
    u8* pCmd = ref().dl.dl.var[n].dl.tevKonstantSel;
    u32 shift = (stage & 1) ? 0xE : 4;
    u32 mask = 0x1F << shift;

    detail::ResWriteSSMask(pCmd, mask);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ],
        ((n + GX_BP_REG_TEVKSEL0) << GX_BP_OPCODE_SHIFT) | (sel << shift),
        mask | (0xFF << GX_BP_OPCODE_SHIFT));
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void ResTev::GXSetTevKAlphaSel(GXTevStageID stage, GXTevKAlphaSel sel) {
    u32 n = stage / 2;
    u8* pCmd = ref().dl.dl.var[n].dl.tevKonstantSel;
    u32 shift = (stage & 1) ? 0x13 : 9;
    u32 mask = 0x1F << shift;

    detail::ResWriteSSMask(pCmd, mask);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ],
        ((n + GX_BP_REG_TEVKSEL0) << GX_BP_OPCODE_SHIFT) | (sel << shift),
        mask | (0xFF << GX_BP_OPCODE_SHIFT));
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

#pragma dont_inline on
void ResTev::GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord,
                           GXTexMapID map, GXChannelID channel) {
    // Convert GX channel ID to GXRasChannelID (indexed by channel & 0xF)
    static const u8 r2c[16] = {
        GX_RAS_COLOR0A0,    GX_RAS_COLOR1A1,    GX_RAS_COLOR0A0,
        GX_RAS_COLOR1A1,    GX_RAS_COLOR0A0,    GX_RAS_COLOR1A1,
        GX_RAS_COLOR_ZERO,  GX_RAS_ALPHA_BUMP,  GX_RAS_ALPHA_BUMPN,
        GX_RAS_COLOR0A0,    GX_RAS_COLOR0A0,    GX_RAS_COLOR0A0,
        GX_RAS_COLOR0A0,    GX_RAS_COLOR0A0,    GX_RAS_COLOR0A0,
        GX_RAS_COLOR_ZERO};

    GXTexCoordID coord2;
    GXTexMapID map2;
    if (GXGetTevOrder(stage, &coord2, &map2, NULL) && coord2 != 0xFF &&
        map2 != 0xFF) {
        ref().texCoordToTexMapID[coord2] = 0xFF;
    }

    if (coord != 0xFF) {
        ref().texCoordToTexMapID[coord] = static_cast<u8>(map);
    }

    u32 n = stage / 2;
    u8* pCmd = ref().dl.dl.var[n].dl.tevOrder;
    u32 shift = (stage & 1) ? 12 : 0;
    u32 mask = 0x3FF << shift;
    u32 en = (map != 0xFF && (map & 0x100) == 0) ? 1 : 0;

    detail::ResWriteBPCmd(
        pCmd,
        ((n + 0x28) << GX_BP_OPCODE_SHIFT) |
            ((((((coord & 7) << 3) | (map & 7)) | (en << 6)) |
              (r2c[channel & 0xF] << 7))
             << shift),
        mask | (0xFF << GX_BP_OPCODE_SHIFT));
}

} // namespace g3d
} // namespace nw4r
#pragma dont_inline reset

namespace nw4r {
namespace g3d {

void ResTev::GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias,
                             GXTevScale scale, u8 clamp, GXTevRegID reg) {
    u8* pCmd = ref().dl.dl.var[stage / 2].dl.tevColorCalc[stage % 2];

    u32 cmd;
    if (op <= GX_TEV_SUB) {
        cmd = (clamp << 19) | (bias << 16);
        cmd |= (op & 1) << 18;
        cmd |= (reg << 22) | (scale << 20);
        cmd |= (GX_BP_REG_TEVCOLORCOMBINER0 + (stage << 1))
               << GX_BP_OPCODE_SHIFT;
        detail::ResWriteBPCmd(pCmd, cmd, 0xFFFF0000);
    } else {
        cmd = (clamp << 19) | (3 << 16);
        cmd = (cmd & ~(1 << 18)) | ((op & 1) << 18);
        u32 t = ((op >> 1) & 3) << 20;
        t = (t & ~(0x3FF << 22)) | (reg << 22);
        cmd = t | cmd;
        cmd |= (GX_BP_REG_TEVCOLORCOMBINER0 + (stage << 1))
               << GX_BP_OPCODE_SHIFT;
        detail::ResWriteBPCmd(pCmd, cmd, 0xFFFF0000);
    }
}

} // namespace g3d
} // namespace nw4r


namespace nw4r {
namespace g3d {

void ResTev::SetNumTevStages(u8 num) {
    if (num < 1) {
        return;
    }
    if (num > 0x10) {
        return;
    }

    int s;
    ResTevData& d = ref();
    if ((u32)d.nStages > num) {
        // Clear texCoord/texMap bindings for all stages past the new count
        for (s = num; (u32)s < d.nStages; s++) {
            GXSetTevOrder(static_cast<GXTevStageID>(s),
                         static_cast<GXTexCoordID>(0xFF),
                         static_cast<GXTexMapID>(0xFF),
                         static_cast<GXChannelID>(0xFF));
        }

        s = (num + 1) / 2;
        u8* p = reinterpret_cast<u8*>(&d.dl.dl.var[s]);
        // Zero whole variable DLs covering stage pairs beyond the new count
        for (; (u32)s < ((d.nStages + 1u) >> 1); s++) {
            detail::ZeroMemory16ByteBlocks(p, 0x30);
            DC::StoreRangeNoSync(p, 0x30);
            p += 0x30;
        }
    }

    d.nStages = num;
}

} // namespace g3d
} // namespace nw4r
