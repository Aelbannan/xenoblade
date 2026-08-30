// g3d_resmat.h only declares the 2-param GXGetIndTexMtx; this TU also matches
// the retail 3-param overload GXGetIndTexMtx(id, pMtx, pScaleExp).  The header
// is outside this session's writable scope, so inject the missing const-member
// declaration while the header is included (the original 2-param declaration is
// preserved by the trailing token).
#define GXGetIndTexMtx GXGetIndTexMtx(GXIndTexMtxID id, math::MTX34* pMtx, s8* pScaleExp) const; bool GXGetIndTexMtx
#include <nw4r/g3d.h>
#undef GXGetIndTexMtx

// Retail .sdata2 pool constants used by GXGetIndTexMtx (see port/data_defs.cpp).
// Per MWCC_PATTERNS.md section 1b: declare as scalars and reference verbatim
// at each site so MWCC emits lfs/lfd lbl@sda21(r0) pool loads.
extern "C" const f32 lbl_eu_80669A68; // 0.0f
extern "C" const f32 lbl_eu_80669A6C; // 1.0f
extern "C" const f32 lbl_eu_80669A88; // 1/1024.0f
extern "C" const f32 lbl_eu_80669A80; // 2.0f
extern "C" const f32 lbl_eu_80669A84; // 0.5f
extern "C" const f32 lbl_eu_80669A98; // 1024.0f (first float of lbl_eu_80669A98)
extern "C" const f64 lbl_eu_80669A90; // 2^52 + 2^31 (MWCC s32 -> f64 conversion magic)
extern "C" const f64 lbl_eu_80669A70; // 0.6 (ResMatFur::GetLyrRate pow exponent)

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * GetParent
 *
 ******************************************************************************/
ResMdl ResMat::GetParent() {
    return ofs_to_obj<ResMdl>(ref().toResMdlData);
}

/******************************************************************************
 *
 * DCStore
 *
 ******************************************************************************/
void ResMatPix::DCStore(bool sync) {
    ResPixDL& r = ref();

    if (sync) {
        DC::StoreRange(&r, sizeof(ResPixDL));
    } else {
        DC::StoreRangeNoSync(&r, sizeof(ResPixDL));
    }
}

void ResMatTevColor::DCStore(bool sync) {
    ResTevColorDL& r = ref();

    if (sync) {
        DC::StoreRange(&r, sizeof(ResTevColorDL));
    } else {
        DC::StoreRangeNoSync(&r, sizeof(ResTevColorDL));
    }
}

void ResMatIndMtxAndScale::DCStore(bool sync) {
    ResIndMtxAndScaleDL& r = ref();

    if (sync) {
        DC::StoreRange(&r, sizeof(ResIndMtxAndScaleDL));
    } else {
        DC::StoreRangeNoSync(&r, sizeof(ResIndMtxAndScaleDL));
    }
}

void ResMatTexCoordGen::DCStore(bool sync) {
    ResTexCoordGenDL& r = ref();

    if (sync) {
        DC::StoreRange(&r, sizeof(ResTexCoordGenDL));
    } else {
        DC::StoreRangeNoSync(&r, sizeof(ResTexCoordGenDL));
    }
}

/******************************************************************************
 *
 * CallDisplayList
 *
 ******************************************************************************/
void ResMatPix::CallDisplayList(bool sync) const {
    const ResPixDL& r = ref();

    if (sync) {
        GXCallDisplayList(const_cast<ResPixDL*>(&r), sizeof(ResPixDL));
    } else {
        GXFastCallDisplayList(const_cast<ResPixDL*>(&r), sizeof(ResPixDL));
    }
}

void ResMatTevColor::CallDisplayList(bool sync) const {
    const ResTevColorDL& r = ref();

    if (sync) {
        GXCallDisplayList(const_cast<ResTevColorDL*>(&r),
                          sizeof(ResTevColorDL));
    } else {
        GXFastCallDisplayList(const_cast<ResTevColorDL*>(&r),
                              sizeof(ResTevColorDL));
    }
}

/******************************************************************************
 *
 * CopyTo
 *
 ******************************************************************************/
ResMatPix ResMatPix::CopyTo(void* pDst) const {
    detail::Copy32ByteBlocks(pDst, ptr(), sizeof(ResPixDL));
    return ResMatPix(pDst);
}

ResMatTevColor ResMatTevColor::CopyTo(void* pDst) const {
    detail::Copy32ByteBlocks(pDst, ptr(), sizeof(ResTevColorDL));
    return ResMatTevColor(pDst);
}

ResMatIndMtxAndScale ResMatIndMtxAndScale::CopyTo(void* pDst) const {
    detail::Copy32ByteBlocks(pDst, ptr(), sizeof(ResIndMtxAndScaleDL));
    return ResMatIndMtxAndScale(pDst);
}

ResMatTexCoordGen ResMatTexCoordGen::CopyTo(void* pDst) const {
    detail::Copy32ByteBlocks(pDst, ptr(), sizeof(ResTexCoordGenDL));
    return ResMatTexCoordGen(pDst);
}

/******************************************************************************
 *
 * ResTexObj
 *
 ******************************************************************************/
const GXTexObj* ResTexObj::GetTexObj(GXTexMapID id) const {
    if (IsValid() && id >= GX_TEXMAP0 && id <= GX_TEXMAP7) {
        return &ptr()->texObj[id];
    }

    return NULL;
}

GXTexObj* ResTexObj::GetTexObj(GXTexMapID id) {
    if (IsValid() && id >= GX_TEXMAP0 && id <= GX_TEXMAP7) {
        return &ptr()->texObj[id];
    }

    return NULL;
}

bool ResTexObj::IsValidTexObj(GXTexMapID id) const {
    if (IsValid() && id >= GX_TEXMAP0 && id <= GX_TEXMAP7) {
        return ptr()->flagUsedTexMapID & (1 << id);
    }

    return false;
}

inline void ResTexObj::Validate(GXTexMapID id) {
    if (IsValid() && id >= GX_TEXMAP0 && id <= GX_TEXMAP7) {
        ptr()->flagUsedTexMapID |= (1 << id);
    }
}

inline void ResTexObj::Invalidate(GXTexMapID id) {
    if (IsValid() && id >= GX_TEXMAP0 && id <= GX_TEXMAP7) {
        ptr()->flagUsedTexMapID &= ~(1 << id);
    }
}

ResTexObj ResTexObj::CopyTo(void* pDst) const {
    ResTexObjData* pData = static_cast<ResTexObjData*>(pDst);
    const ResTexObjData& rSrc = ref();

    u32 i = 0;

    u32 flag = rSrc.flagUsedTexMapID;
    pData->flagUsedTexMapID = flag;

    for (; flag != 0; flag >>= 1, i++) {
        if (!(flag & 1)) {
            continue;
        }

        detail::Copy32ByteBlocks(&pData->texObj[i], &rSrc.texObj[i],
                                 sizeof(GXTexObj));
    }

    return ResTexObj(pData);
}

/******************************************************************************
 *
 * ResTlutObj
 *
 ******************************************************************************/
const GXTlutObj* ResTlutObj::GetTlut(GXTlut tlut) const {
    if (IsValid() && tlut >= GX_TLUT0 && tlut <= GX_TLUT7) {
        return &ptr()->tlutObj[tlut];
    }

    return NULL;
}

GXTlutObj* ResTlutObj::GetTlut(GXTlut tlut) {
    if (IsValid() && tlut >= GX_TLUT0 && tlut <= GX_TLUT7) {
        return &ptr()->tlutObj[tlut];
    }

    return NULL;
}

bool ResTlutObj::IsValidTlut(GXTlut tlut) const {
    if (IsValid() && tlut >= GX_TLUT0 && tlut <= GX_TLUT7) {
        return ptr()->flagUsedTlutID & (1 << tlut);
    }

    return false;
}

inline void ResTlutObj::Validate(GXTlut tlut) {
    if (IsValid() && tlut >= GX_TLUT0 && tlut <= GX_TLUT7) {
        ptr()->flagUsedTlutID |= (1 << tlut);
    }
}

inline void ResTlutObj::Invalidate(GXTlut tlut) {
    if (IsValid() && tlut >= GX_TLUT0 && tlut <= GX_TLUT7) {
        ptr()->flagUsedTlutID &= ~(1 << tlut);
    }
}

ResTlutObj ResTlutObj::CopyTo(void* pDst) const {
    ResTlutObjData* pData = static_cast<ResTlutObjData*>(pDst);
    const ResTlutObjData& rSrc = ref();

    u32 flag = rSrc.flagUsedTlutID;
    pData->flagUsedTlutID = flag;

    if (flag != 0) {
        if (!(flag & static_cast<u8>(~(1 << GX_TLUT0 | 1 << GX_TLUT1)))) {
            // At most two tlut objects can fit into 64 bytes
            detail::Copy32ByteBlocks(&pData->tlutObj, &rSrc.tlutObj, 32);

        } else if (!(flag & static_cast<u8>(~(1 << GX_TLUT0 | 1 << GX_TLUT1 |
                                              1 << GX_TLUT2 | 1 << GX_TLUT3 |
                                              1 << GX_TLUT4)))) {
            // At most five tlut objects can fit into 64 bytes
            detail::Copy32ByteBlocks(&pData->tlutObj, &rSrc.tlutObj, 64);

        } else {
            // Full copy is divisible by 32
            detail::Copy32ByteBlocks(&pData->tlutObj, &rSrc.tlutObj,
                                     sizeof(GXTlutObj) * (GX_TLUT7 + 1));
        }
    }

    return ResTlutObj(pData);
}

/******************************************************************************
 *
 * ResTexSrt
 *
 ******************************************************************************/
ResTexSrt ResTexSrt::CopyTo(void* pDst) const {
    ResTexSrtData* pData = static_cast<ResTexSrtData*>(pDst);
    const ResTexSrtData& r = ref();

    u32 flag = r.flag;
    pData->flag = flag;
    pData->texMtxMode = r.texMtxMode;

    for (u32 i = 0; flag != 0; flag >>= TexSrt::NUM_OF_FLAGS, i++) {
        if (!(flag & TexSrt::FLAGSET_IDENTITY)) {
            continue;
        }

        const TexSrt& rFrom = r.texSrt[i];
        const TexMtxEffect& rFromEffect = r.effect[i];

        TexSrt& rTo = pData->texSrt[i];
        TexMtxEffect& rToEffect = pData->effect[i];

        rTo.Su = rFrom.Su;
        rTo.Sv = rFrom.Sv;
        rTo.R = rFrom.R;
        rTo.Tu = rFrom.Tu;
        rTo.Tv = rFrom.Tv;

        rToEffect.ref_camera = rFromEffect.ref_camera;
        rToEffect.ref_light = rFromEffect.ref_light;
        rToEffect.map_mode = rFromEffect.map_mode;
        rToEffect.misc_flag = rFromEffect.misc_flag;

        math::MTX34Copy(
            static_cast<math::MTX34*>(&rToEffect.effectMtx),
            static_cast<const math::MTX34*>(&rFromEffect.effectMtx));
    }

    return ResTexSrt(pData);
}

bool ResTexSrt::SetEffectMtx(u32 id, const math::MTX34* pMtx) {
    if (id < ResTexSrtData::NUM_OF_TEXTURE) {
        TexMtxEffect& rEffect = ref().effect[id];

        if (pMtx != NULL) {
            math::MTX34Copy(static_cast<math::MTX34*>(&rEffect.effectMtx),
                            pMtx);

            rEffect.misc_flag &= ~TexMtxEffect::FLAG_IDENT;

        } else {
            math::MTX34Identity(static_cast<math::MTX34*>(&rEffect.effectMtx));

            rEffect.misc_flag |= TexMtxEffect::FLAG_IDENT;
        }

        return true;
    }

    return false;
}

inline bool ResTexSrt::GetEffectMtx(u32 id, math::MTX34* pMtx) const {
    if (pMtx != NULL && id < ResTexSrtData::NUM_OF_TEXTURE) {
        const TexMtxEffect& rEffect = ref().effect[id];

        math::MTX34Copy(pMtx,
                        static_cast<const math::MTX34*>(&rEffect.effectMtx));

        return true;
    }

    return false;
}

inline bool ResTexSrt::SetMapMode(u32 id, u32 mode, int camRef, int lightRef) {
    if (id < ResTexSrtData::NUM_OF_TEXTURE &&
        mode < G3DState::NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE) {

        ResTexSrtData& r = ref();
        TexMtxEffect& rEffect = r.effect[id];

        rEffect.map_mode = mode;

        rEffect.ref_camera = camRef >= 0 && camRef < G3DState::NUM_CAMERA
                                 ? static_cast<s8>(camRef)
                                 : -1;

        rEffect.ref_light = lightRef >= 0 && lightRef < G3DState::NUM_LIGHT
                                ? static_cast<s8>(lightRef)
                                : -1;

        return true;
    }

    return false;
}

inline bool ResTexSrt::GetMapMode(u32 id, u32* pMode, int* pCamRef,
                           int* pLightRef) const {
    if (id < ResTexSrtData::NUM_OF_TEXTURE) {
        const ResTexSrtData& r = ref();
        const TexMtxEffect& rEffect = r.effect[id];

        if (pMode != NULL) {
            *pMode = rEffect.map_mode;
        }

        if (pCamRef != NULL) {
            *pCamRef = rEffect.ref_camera;
        }

        if (pLightRef != NULL) {
            *pLightRef = rEffect.ref_light;
        }

        return true;
    }

    return false;
}

/******************************************************************************
 *
 * ResGenMode
 *
 ******************************************************************************/
inline void ResGenMode::GXSetNumTexGens(u8 num) {
    if (IsValid()) {
        ref().nTexGens = num;
    }
}

inline void ResGenMode::GXSetNumChans(u8 num) {
    if (IsValid()) {
        ref().nChans = num;
    }
}

void ResGenMode::GXSetNumTevStages(u8 num) {
    if (IsValid()) {
        ref().nTevs = num;
    }
}

inline void ResGenMode::GXSetNumIndStages(u8 num) {
    if (IsValid()) {
        ref().nInds = num;
    }
}

void ResGenMode::GXSetCullMode(GXCullMode mode) {
    if (IsValid()) {
        ref().cullMode = mode;
    }
}

ResGenMode ResGenMode::CopyTo(void* pDst) const {
    ResGenModeData* pData = static_cast<ResGenModeData*>(pDst);
    const ResGenModeData& r = ref();

    *pData = r;
    return ResGenMode(pData);
}

/******************************************************************************
 *
 * ResMatMisc
 *
 ******************************************************************************/
GXBool ResMatMisc::GXGetZCompLoc() const {
    if (IsValid()) {
        return ptr()->zCompLoc;
    }

    return FALSE;
}

int ResMatMisc::GetLightSetIdx() const {
    if (IsValid()) {
        return ptr()->light_set_idx;
    }

    return -1;
}

int ResMatMisc::GetFogIdx() const {
    if (IsValid()) {
        return ptr()->fog_idx;
    }

    return -1;
}

void ResMatMisc::GetIndirectTexMtxCalcMethod(
    GXIndTexMtxID id, ResMatMiscData::IndirectMethod* pMethod, s8* pLightRef) {

    if (IsValid() && id >= GX_ITM_0 && id <= GX_ITM_2) {
        ResMatMiscData& r = *ptr();

        if (pMethod != NULL) {
            *pMethod = static_cast<ResMatMiscData::IndirectMethod>(
                r.indirect_method[id - GX_ITM_0]);
        }

        if (pLightRef != NULL) {
            *pLightRef = r.normal_map_ref_light[id - GX_ITM_0];
        }
    }
}

ResMatMisc ResMatMisc::CopyTo(void* pDst) const {
    ResMatMiscData* pData = static_cast<ResMatMiscData*>(pDst);
    const ResMatMiscData& r = ref();

    *pData = r;
    return ResMatMisc(pData);
}

/******************************************************************************
 *
 * ResMatPix
 *
 ******************************************************************************/
bool ResMatPix::GXGetAlphaCompare(GXCompare* pComp0, u8* pRef0,
                                  GXAlphaOp* pLogic, GXCompare* pComp1,
                                  u8* pRef1) const {
    const u8* pCmd = ref().dl.alphaCompare;
    if (pCmd[0] == 0) {
        return false;
    }

    u32 reg;
    detail::ResReadBPCmd(pCmd, &reg);

    if (pRef0 != NULL) {
        *pRef0 = reg >> GX_BP_ALPHACOMPARE_REF0_SHIFT &
                 GX_BP_ALPHACOMPARE_REF0_LMASK;
    }

    if (pRef1 != NULL) {
        *pRef1 = reg >> GX_BP_ALPHACOMPARE_REF1_SHIFT &
                 GX_BP_ALPHACOMPARE_REF1_LMASK;
    }

    if (pComp0 != NULL) {
        *pComp0 = static_cast<GXCompare>(reg >> GX_BP_ALPHACOMPARE_COMP0_SHIFT &
                                         GX_BP_ALPHACOMPARE_COMP0_LMASK);
    }

    if (pComp1 != NULL) {
        *pComp1 = static_cast<GXCompare>(reg >> GX_BP_ALPHACOMPARE_COMP1_SHIFT &
                                         GX_BP_ALPHACOMPARE_COMP1_LMASK);
    }

    if (pLogic != NULL) {
        *pLogic = static_cast<GXAlphaOp>(reg >> GX_BP_ALPHACOMPARE_LOGIC_SHIFT &
                                         GX_BP_ALPHACOMPARE_LOGIC_LMASK);
    }

    return true;
}

void ResMatPix::GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp logic,
                                  GXCompare comp1, u8 ref1) {
    u8* pCmd = ref().dl.alphaCompare;

    u32 reg = ref0 << GX_BP_ALPHACOMPARE_REF0_SHIFT |
              ref1 << GX_BP_ALPHACOMPARE_REF1_SHIFT |
              comp0 << GX_BP_ALPHACOMPARE_COMP0_SHIFT |
              comp1 << GX_BP_ALPHACOMPARE_COMP1_SHIFT |
              logic << GX_BP_ALPHACOMPARE_LOGIC_SHIFT |
              GX_BP_REG_ALPHACOMPARE << GX_BP_OPCODE_SHIFT;

    detail::ResWriteBPCmd(pCmd, reg);
}

inline bool ResMatPix::GXGetZMode(GXBool* pTest, GXCompare* pCompare,
                           GXBool* pUpdate) const {
    const u8* pCmd = ref().dl.zMode;
    if (pCmd[0] == 0) {
        return false;
    }

    u32 reg;
    detail::ResReadBPCmd(pCmd, &reg);

    if (pTest != NULL) {
        *pTest = reg >> GX_BP_ZMODE_TEST_ENABLE_SHIFT &
                 GX_BP_ZMODE_TEST_ENABLE_LMASK;
    }

    if (pCompare != NULL) {
        *pCompare = static_cast<GXCompare>(reg >> GX_BP_ZMODE_COMPARE_SHIFT &
                                           GX_BP_ZMODE_COMPARE_LMASK);
    }

    if (pUpdate != NULL) {
        // @note Update enable is only one bit
        *pUpdate = reg >> GX_BP_ZMODE_UPDATE_ENABLE_SHIFT & 0xFF;
    }

    return true;
}

inline void ResMatPix::GXSetZMode(GXBool test, GXCompare compare, GXBool update) {
    u8* pCmd = ref().dl.zMode;

    u32 reg = test << GX_BP_ZMODE_TEST_ENABLE_SHIFT |
              compare << GX_BP_ZMODE_COMPARE_SHIFT |
              update << GX_BP_ZMODE_UPDATE_ENABLE_SHIFT |
              GX_BP_REG_ZMODE << GX_BP_OPCODE_SHIFT;

    detail::ResWriteBPCmd(pCmd, reg);
}

bool ResMatPix::GXGetBlendMode(GXBlendMode* pMode, GXBlendFactor* pSrcFactor,
                               GXBlendFactor* pDstFactor,
                               GXLogicOp* pLogic) const {
    const u8* pCmd = ref().dl.blendMode;
    if (pCmd[0] == 0) {
        return false;
    }

    u32 reg;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 1], &reg);

    GXBlendMode mode;
    if (reg >> GX_BP_BLENDMODE_LOGIC_OP_ENABLE_SHIFT &
        GX_BP_BLENDMODE_LOGIC_OP_ENABLE_LMASK) {
        mode = GX_BM_LOGIC;
    } else if (reg >> GX_BP_BLENDMODE_SUBTRACT_SHIFT &
               GX_BP_BLENDMODE_SUBTRACT_LMASK) {
        mode = GX_BM_SUBTRACT;
    } else if (reg >> GX_BP_BLENDMODE_BLEND_ENABLE_SHIFT &
               GX_BP_BLENDMODE_BLEND_ENABLE_LMASK) {
        mode = GX_BM_BLEND;
    } else {
        mode = GX_BM_NONE;
    }

    if (pMode != NULL) {
        *pMode = mode;
    }

    if (pSrcFactor != NULL) {
        *pSrcFactor =
            static_cast<GXBlendFactor>(reg >> GX_BP_BLENDMODE_SRC_FACTOR_SHIFT &
                                       GX_BP_BLENDMODE_SRC_FACTOR_LMASK);
    }

    if (pDstFactor != NULL) {
        *pDstFactor =
            static_cast<GXBlendFactor>(reg >> GX_BP_BLENDMODE_DST_FACTOR_SHIFT &
                                       GX_BP_BLENDMODE_DST_FACTOR_LMASK);
    }

    if (pLogic != NULL) {
        *pLogic = static_cast<GXLogicOp>(
            // @note Logic mode is only four bits
            reg >> GX_BP_BLENDMODE_LOGIC_MODE_SHIFT & 0xFFF);
    }

    return true;
}

void ResMatPix::GXSetBlendMode(GXBlendMode mode, GXBlendFactor srcFactor,
                               GXBlendFactor dstFactor, GXLogicOp logic) {
    u8* pCmd = ref().dl.blendMode;

    // clang-format off
    detail::ResWriteSSMask(&pCmd[GX_BP_CMD_SZ * 0],
        GX_BP_BLENDMODE_BLEND_ENABLE_MASK | GX_BP_BLENDMODE_LOGIC_OP_ENABLE_MASK |
        GX_BP_BLENDMODE_DST_FACTOR_MASK   | GX_BP_BLENDMODE_SRC_FACTOR_MASK      |
        GX_BP_BLENDMODE_SUBTRACT_MASK     | GX_BP_BLENDMODE_LOGIC_MODE_MASK);

    u32 reg = static_cast<u32>(mode == GX_BM_BLEND || mode == GX_BM_SUBTRACT) << GX_BP_BLENDMODE_BLEND_ENABLE_SHIFT    |
              static_cast<u32>(mode == GX_BM_LOGIC)                           << GX_BP_BLENDMODE_LOGIC_OP_ENABLE_SHIFT |
              static_cast<u32>(dstFactor)                                     << GX_BP_BLENDMODE_DST_FACTOR_SHIFT      |
              static_cast<u32>(srcFactor)                                     << GX_BP_BLENDMODE_SRC_FACTOR_SHIFT      |
              static_cast<u32>(mode == GX_BM_SUBTRACT)                        << GX_BP_BLENDMODE_SUBTRACT_SHIFT        |
              static_cast<u32>(logic)                                         << GX_BP_BLENDMODE_LOGIC_MODE_SHIFT      |
              static_cast<u32>(GX_BP_REG_BLENDMODE)                           << GX_BP_OPCODE_SHIFT;
    // clang-format on

    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 1], reg);
}

inline bool ResMatPix::GXGetDstAlpha(GXBool* pEnable, u8* pAlpha) const {
    const u8* pCmd = ref().dl.setDstAlpha;
    if (pCmd[0] == 0) {
        return false;
    }

    u32 reg;
    detail::ResReadBPCmd(pCmd, &reg);

    if (pEnable != NULL) {
        if (reg & GX_BP_DSTALPHA_ENABLE_MASK) {
            *pEnable = TRUE;
        } else {
            *pEnable = FALSE;
        }
    }

    if (pAlpha != NULL) {
        *pAlpha =
            reg >> GX_BP_DSTALPHA_ALPHA_SHIFT & GX_BP_DSTALPHA_ALPHA_LMASK;
    }

    return true;
}

inline void ResMatPix::GXSetDstAlpha(GXBool enable, u8 alpha) {
    u8* pCmd = ref().dl.setDstAlpha;

    u32 reg = alpha << GX_BP_DSTALPHA_ALPHA_SHIFT |
              enable << GX_BP_DSTALPHA_ENABLE_SHIFT |
              GX_BP_REG_DSTALPHA << GX_BP_OPCODE_SHIFT;

    detail::ResWriteBPCmd(pCmd, reg);
}

/******************************************************************************
 *
 * ResMatTevColor
 *
 ******************************************************************************/
bool ResMatTevColor::GXGetTevColor(GXTevRegID id, GXColor* pColor) const {
    const u8* pCmd = ref().dl.tevColor[id - 1];
    if (pCmd[0] == 0) {
        return false;
    }

    u32 regRA;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 0], &regRA);

    u32 regBG;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 1], &regBG);

    *pColor = detail::GetRGBA(
        regRA >> GX_BP_TEVREGLO_RED_SHIFT & GX_BP_TEVREGLO_RED_LMASK,
        regBG >> GX_BP_TEVREGLO_ALPHA_SHIFT & GX_BP_TEVREGLO_ALPHA_LMASK,
        regBG >> GX_BP_TEVREGHI_BLUE_SHIFT & GX_BP_TEVREGHI_BLUE_LMASK,
        regRA >> GX_BP_TEVREGHI_GREEN_SHIFT & GX_BP_TEVREGHI_GREEN_LMASK);

    return true;
}

void ResMatTevColor::GXSetTevColor(GXTevRegID id, GXColor color) {
    u8* pCmd = ref().dl.tevColor[id - 1];

    u32 regRA = color.r << GX_BP_TEVREGLO_RED_SHIFT |
                color.a << GX_BP_TEVREGLO_ALPHA_SHIFT |
                GX_TEVREG_COLOR << GX_BP_TEVREGLO_TYPE_SHIFT |
                (id * 2 + GX_BP_REG_TEVREG0LO) << GX_BP_OPCODE_SHIFT;

    u32 regBG = color.b << GX_BP_TEVREGHI_BLUE_SHIFT |
                color.g << GX_BP_TEVREGHI_GREEN_SHIFT |
                GX_TEVREG_COLOR << GX_BP_TEVREGHI_TYPE_SHIFT |
                (id * 2 + GX_BP_REG_TEVREG0HI) << GX_BP_OPCODE_SHIFT;

    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 0], regRA);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 1], regBG);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 2], regBG);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 3], regBG);
}

inline bool ResMatTevColor::GXGetTevColorS10(GXTevRegID id, GXColorS10* pColor) const {
    const u8* pCmd = ref().dl.tevColor[id - 1];
    if (pCmd[0] == 0) {
        return false;
    }

    u32 regRA;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 0], &regRA);

    u32 regBG;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 1], &regBG);

    s16 r = regRA >> GX_BP_TEVREGLO_RED_SHIFT & GX_BP_TEVREGLO_RED_LMASK;
    s16 g = regBG >> GX_BP_TEVREGHI_GREEN_SHIFT & GX_BP_TEVREGHI_GREEN_LMASK;
    s16 b = regBG >> GX_BP_TEVREGHI_BLUE_SHIFT & GX_BP_TEVREGHI_BLUE_LMASK;
    s16 a = regRA >> GX_BP_TEVREGLO_ALPHA_SHIFT & GX_BP_TEVREGLO_ALPHA_LMASK;

    // clang-format off
    *pColor = detail::GetRGBAS10(
        (r << (32 - GX_BP_TEVREGLO_RED_SZ)) >> (32 - GX_BP_TEVREGLO_RED_SZ),
        (g << (32 - GX_BP_TEVREGHI_GREEN_SZ)) >> (32 - GX_BP_TEVREGHI_GREEN_SZ),
        (b << (32 - GX_BP_TEVREGHI_BLUE_SZ)) >> (32 - GX_BP_TEVREGHI_BLUE_SZ),
        (a << (32 - GX_BP_TEVREGLO_ALPHA_SZ)) >> (32 - GX_BP_TEVREGLO_ALPHA_SZ));
    // clang-format on

    return true;
}

inline void ResMatTevColor::GXSetTevColorS10(GXTevRegID id, GXColorS10 color) {
    u8* pCmd = ref().dl.tevColor[id - 1];

    u32 mask = GX_BP_TEVREGLO_RED_LMASK;

    u32 regRA = (color.r & mask) << GX_BP_TEVREGLO_RED_SHIFT |
                (color.a & mask) << GX_BP_TEVREGLO_ALPHA_SHIFT |
                (id * 2 + GX_BP_REG_TEVREG0LO) << GX_BP_OPCODE_SHIFT |
                GX_TEVREG_COLOR << GX_BP_TEVREGLO_TYPE_SHIFT;

    u32 regBG = (color.b & mask) << GX_BP_TEVREGHI_BLUE_SHIFT |
                (color.g & mask) << GX_BP_TEVREGHI_GREEN_SHIFT |
                (id * 2 + GX_BP_REG_TEVREG0HI) << GX_BP_OPCODE_SHIFT |
                GX_TEVREG_COLOR << GX_BP_TEVREGHI_TYPE_SHIFT;

    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 0], regRA);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 1], regBG);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 2], regBG);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 3], regBG);
}

bool ResMatTevColor::GXGetTevKColor(GXTevKColorID id, GXColor* pColor) const {
    const u8* pCmd = ref().dl.tevKColor[id];
    if (pCmd[0] == 0) {
        return false;
    }

    u32 regRA;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 0], &regRA);

    u32 regBG;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 1], &regBG);

    *pColor = detail::GetRGBA(
        regRA >> GX_BP_TEVREGLO_RED_SHIFT & GX_BP_TEVREGLO_RED_LMASK,
        regBG >> GX_BP_TEVREGLO_ALPHA_SHIFT & GX_BP_TEVREGLO_ALPHA_LMASK,
        regBG >> GX_BP_TEVREGHI_BLUE_SHIFT & GX_BP_TEVREGHI_BLUE_LMASK,
        regRA >> GX_BP_TEVREGHI_GREEN_SHIFT & GX_BP_TEVREGHI_GREEN_LMASK);

    return true;
}

void ResMatTevColor::GXSetTevKColor(GXTevKColorID id, GXColor color) {
    u8* pCmd = ref().dl.tevKColor[id];

    u32 regRA = color.r << GX_BP_TEVREGLO_RED_SHIFT |
                color.a << GX_BP_TEVREGLO_ALPHA_SHIFT |
                GX_TEVREG_KONST << GX_BP_TEVREGLO_TYPE_SHIFT |
                (id * 2 + GX_BP_REG_TEVREG0LO) << GX_BP_OPCODE_SHIFT;

    u32 regBG = color.b << GX_BP_TEVREGHI_BLUE_SHIFT |
                color.g << GX_BP_TEVREGHI_GREEN_SHIFT |
                GX_TEVREG_KONST << GX_BP_TEVREGHI_TYPE_SHIFT |
                (id * 2 + GX_BP_REG_TEVREG0HI) << GX_BP_OPCODE_SHIFT;

    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 0], regRA);
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 1], regBG);
}

/******************************************************************************
 *
 * ResMatIndMtxAndScale
 *
 ******************************************************************************/
void ResMatIndMtxAndScale::CallDisplayList(u8 indNum, bool sync) const {
    if (indNum == 0) {
        return;
    }

    const ResIndMtxAndScaleDL& r = ref();

    if (sync) {
        GXCallDisplayList(const_cast<ResIndMtxAndScaleDL*>(&r),
                          sizeof(ResIndMtxAndScaleDL));
    } else {
        GXFastCallDisplayList(const_cast<ResIndMtxAndScaleDL*>(&r),
                              sizeof(ResIndMtxAndScaleDL));
    }
}

bool ResMatIndMtxAndScale::GXGetIndTexMtx(GXIndTexMtxID id,
                                          math::MTX34* pMtx) const {
    s8 scaleExp;
    const u8* pCmd;
    const ResIndMtxAndScaleDL& r = ref();

    switch (id) {
    case GX_ITM_0: {
        pCmd = r.dl.indTexMtx0;
        break;
    }

    case GX_ITM_1: {
        pCmd = r.dl.indTexMtx1;
        break;
    }

    case GX_ITM_2: {
        pCmd = r.dl.indTexMtx2;
        break;
    }

    default: {
        return false;
    }
    }

    if (pCmd[0] == 0) {
        return false;
    }

    u32 regA;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 0], &regA);

    u32 regB;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 1], &regB);

    u32 regC;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 2], &regC);

    u32 scaleExpReg =
        (regA >> GX_BP_INDMTXA_EXP_SHIFT & GX_BP_INDMTXA_EXP_LMASK) << 0 |
        (regB >> GX_BP_INDMTXB_EXP_SHIFT & GX_BP_INDMTXB_EXP_LMASK) << 2 |
        (regC >> GX_BP_INDMTXC_EXP_SHIFT & GX_BP_INDMTXC_EXP_LMASK) << 4;

    // Hardware stores as -17
    scaleExp = static_cast<s8>(scaleExpReg - 17);

    // Exponent to value
    f32 scale = lbl_eu_80669A6C;
    if (scaleExp > 0) {
        while (scaleExp != 0) {
            scaleExp--;
            scale *= lbl_eu_80669A80;
        }
    } else if (scaleExp < 0) {
        while (scaleExp != 0) {
            scaleExp++;
            scale *= lbl_eu_80669A84;
        }
    }

    if (pMtx != NULL) {
        // clang-format off
        pMtx->_00 = scale * static_cast<f32>(static_cast<int>((regA >> GX_BP_INDMTXA_M00_SHIFT & GX_BP_INDMTXA_M00_LMASK) << (32 - GX_BP_INDMTXA_M00_SZ)) >> (32 - GX_BP_INDMTXA_M00_SZ)) * lbl_eu_80669A88;
        pMtx->_01 = scale * static_cast<f32>(static_cast<int>((regB >> GX_BP_INDMTXB_M01_SHIFT & GX_BP_INDMTXB_M01_LMASK) << (32 - GX_BP_INDMTXB_M01_SZ)) >> (32 - GX_BP_INDMTXB_M01_SZ)) * lbl_eu_80669A88;
        pMtx->_02 = scale * static_cast<f32>(static_cast<int>((regC >> GX_BP_INDMTXC_M02_SHIFT & GX_BP_INDMTXC_M02_LMASK) << (32 - GX_BP_INDMTXC_M02_SZ)) >> (32 - GX_BP_INDMTXC_M02_SZ)) * lbl_eu_80669A88;
        pMtx->_03 = lbl_eu_80669A68;

        pMtx->_10 = scale * static_cast<f32>(static_cast<int>((regA >> GX_BP_INDMTXA_M10_SHIFT & GX_BP_INDMTXA_M10_LMASK) << (32 - GX_BP_INDMTXA_M10_SZ)) >> (32 - GX_BP_INDMTXA_M00_SZ)) * lbl_eu_80669A88;
        pMtx->_11 = scale * static_cast<f32>(static_cast<int>((regB >> GX_BP_INDMTXB_M11_SHIFT & GX_BP_INDMTXB_M11_LMASK) << (32 - GX_BP_INDMTXB_M11_SZ)) >> (32 - GX_BP_INDMTXB_M11_SZ)) * lbl_eu_80669A88;
        pMtx->_12 = scale * static_cast<f32>(static_cast<int>((regC >> GX_BP_INDMTXC_M12_SHIFT & GX_BP_INDMTXC_M12_LMASK) << (32 - GX_BP_INDMTXC_M12_SZ)) >> (32 - GX_BP_INDMTXC_M12_SZ)) * lbl_eu_80669A88;
        pMtx->_13 = lbl_eu_80669A68;

        pMtx->_20 = lbl_eu_80669A68;
        pMtx->_21 = lbl_eu_80669A68;
        pMtx->_22 = lbl_eu_80669A6C;
        pMtx->_23 = lbl_eu_80669A68;
        // clang-format on
    }

    return true;
}

// MWCC lowers s32 -> f64 via the classic 2^52 exponent trick; the (2^52 +
// 2^31) bias constant lives in the retail sdata2 pool (lbl_eu_80669A90).

bool ResMatIndMtxAndScale::GXGetIndTexMtx(GXIndTexMtxID id, math::MTX34* pMtx,
                                          s8* pScaleExp) const {
    const ResIndMtxAndScaleDL& r = ref();
    const u8* pCmd;

    switch (id) {
    case GX_ITM_0: {
        pCmd = r.dl.indTexMtx0;
        break;
    }

    case GX_ITM_1: {
        pCmd = r.dl.indTexMtx1;
        break;
    }

    case GX_ITM_2: {
        pCmd = r.dl.indTexMtx2;
        break;
    }

    default: {
        return false;
    }
    }

    if (pCmd[0] == 0) {
        return false;
    }

    // Raw GX BP command words (see GXHardwareBP.h INDMTXA/B/C).
    u32 regA;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 0], &regA);

    u32 regB;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 1], &regB);

    u32 regC;
    detail::ResReadBPCmd(&pCmd[GX_BP_CMD_SZ * 2], &regC);

    // 2-bit scale exponent field per register, packed low-first.
    u32 scaleExpReg =
        (regA >> GX_BP_INDMTXA_EXP_SHIFT & GX_BP_INDMTXA_EXP_LMASK) << 0 |
        (regB >> GX_BP_INDMTXB_EXP_SHIFT & GX_BP_INDMTXB_EXP_LMASK) << 2 |
        (regC >> GX_BP_INDMTXC_EXP_SHIFT & GX_BP_INDMTXC_EXP_LMASK) << 4;

    // Hardware stores the exponent biased by -17.
    if (pScaleExp != NULL) {
        *pScaleExp = static_cast<s8>(scaleExpReg - 17);
    }

    if (pMtx != NULL) {
        // Mantissa fields: row 0 occupies bits 10-0, row 1 bits 21-31 (both
        // sign-extended); the 1/1024 fixed-point scale comes from the pool.
        pMtx->_00 = lbl_eu_80669A88 * (f32)((s32)(regA << 21) >> 21);
        pMtx->_01 = lbl_eu_80669A88 * (f32)((s32)(regB << 21) >> 21);
        pMtx->_02 = lbl_eu_80669A88 * (f32)((s32)(regC << 21) >> 21);
        pMtx->_03 = lbl_eu_80669A68;

        // Row-1 fields sit in bits 21-31: retail extracts them left-justified
        // via rlwinm rotl(10) & top-mask before the sign-extending srawi.
        pMtx->_10 = lbl_eu_80669A88 * (f32)((s32)__rlwinm(regA, 10, 0, 10) >> 21);
        pMtx->_11 = lbl_eu_80669A88 * (f32)((s32)__rlwinm(regB, 10, 0, 10) >> 21);
        pMtx->_12 = lbl_eu_80669A88 * (f32)((s32)__rlwinm(regC, 10, 0, 10) >> 21);
        pMtx->_13 = lbl_eu_80669A68;

        pMtx->_20 = lbl_eu_80669A68;
        pMtx->_21 = lbl_eu_80669A68;
        pMtx->_22 = lbl_eu_80669A6C;
        pMtx->_23 = lbl_eu_80669A68;
    }

    return true;
}

void ResMatIndMtxAndScale::GXSetIndTexMtx(GXIndTexMtxID id,
                                          const math::MTX34& rMtx,
                                          s8 scaleExp) {
    u32 offset;
    u8* pCmd;
    ResIndMtxAndScaleDL& r = ref();

    switch (id) {
    case GX_ITM_0: {
        pCmd = r.dl.indTexMtx0;
        offset = GX_BP_REG_INDMTX0A - GX_BP_REG_INDMTX0A;
        break;
    }

    case GX_ITM_1: {
        pCmd = r.dl.indTexMtx1;
        offset = GX_BP_REG_INDMTX1A - GX_BP_REG_INDMTX0A;
        break;
    }

    case GX_ITM_2: {
        pCmd = r.dl.indTexMtx2;
        offset = GX_BP_REG_INDMTX2A - GX_BP_REG_INDMTX0A;
        break;
    }

    default: {
        return;
    }
    }

    // Hardware stores as -17
    scaleExp += static_cast<s8>(17);

    // clang-format off
    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 0],
        static_cast<u32>((static_cast<int>(lbl_eu_80669A98 * rMtx._00) & GX_BP_INDMTXA_M00_LMASK) << GX_BP_INDMTXA_M00_SHIFT) |
        static_cast<u32>((static_cast<int>(lbl_eu_80669A98 * rMtx._10) & GX_BP_INDMTXA_M10_LMASK) << GX_BP_INDMTXA_M10_SHIFT) |
        static_cast<u32>((scaleExp >> 0 & GX_BP_INDMTXA_EXP_LMASK) << GX_BP_INDMTXA_EXP_SHIFT) |
        static_cast<u32>((offset + GX_BP_REG_INDMTX0A) << GX_BP_OPCODE_SHIFT));

    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 1],
        static_cast<u32>((static_cast<int>(lbl_eu_80669A98 * rMtx._01) & GX_BP_INDMTXB_M01_LMASK) << GX_BP_INDMTXB_M01_SHIFT) |
        static_cast<u32>((static_cast<int>(lbl_eu_80669A98 * rMtx._11) & GX_BP_INDMTXB_M11_LMASK) << GX_BP_INDMTXB_M11_SHIFT) |
        static_cast<u32>((scaleExp >> 2 & GX_BP_INDMTXB_EXP_LMASK) << GX_BP_INDMTXB_EXP_SHIFT) |
        static_cast<u32>((offset + GX_BP_REG_INDMTX0B) << GX_BP_OPCODE_SHIFT));

    detail::ResWriteBPCmd(&pCmd[GX_BP_CMD_SZ * 2],
        static_cast<u32>((static_cast<int>(lbl_eu_80669A98 * rMtx._02) & GX_BP_INDMTXC_M02_LMASK) << GX_BP_INDMTXC_M02_SHIFT) |
        static_cast<u32>((static_cast<int>(lbl_eu_80669A98 * rMtx._12) & GX_BP_INDMTXC_M12_LMASK) << GX_BP_INDMTXC_M12_SHIFT) |
        static_cast<u32>((scaleExp >> 4 & GX_BP_INDMTXC_EXP_LMASK) << GX_BP_INDMTXC_EXP_SHIFT) |
        static_cast<u32>((offset + GX_BP_REG_INDMTX0C) << GX_BP_OPCODE_SHIFT));
    // clang-format on
}

/******************************************************************************
 *
 * ResMatChan
 *
 ******************************************************************************/
inline void ResMatChan::GXSetChanMatColor(GXChannelID id, GXColor color) {
    GXColor& rMatColor = ref().chan[id & 1].matColor;

    if ((id & 2) == 0) {
        rMatColor.r = color.r;
        rMatColor.g = color.g;
        rMatColor.b = color.b;

        if (id == GX_COLOR0A0 || id == GX_COLOR1A1) {
            rMatColor.a = color.a;
        }
    } else {
        rMatColor.a = color.a;
    }
}

void ResMatChan::GXSetChanAmbColor(GXChannelID id, GXColor color) {
    GXColor& rAmbColor = ref().chan[id & 1].ambColor;

    if ((id & 2) == 0) {
        rAmbColor.r = color.r;
        rAmbColor.g = color.g;
        rAmbColor.b = color.b;

        if (id == GX_COLOR0A0 || id == GX_COLOR1A1) {
            rAmbColor.a = color.a;
        }
    } else {
        rAmbColor.a = color.a;
    }
}

inline void ResMatChan::GXSetChanCtrl(GXChannelID id, GXBool enable, GXColorSrc ambSrc,
                               GXColorSrc matSrc, GXLightID lightMask,
                               GXDiffuseFn diffuseFn, GXAttnFn attnFn) {
    Chan& rChan = ref().chan[id & 1];

    u32* pCtrl;
    if ((id & 2) == 0) {
        pCtrl = &rChan.paramChanCtrlC;
    } else {
        pCtrl = &rChan.paramChanCtrlA;
    }

    // clang-format off
    u32 mask = (matSrc)                                        << GX_XF_COLOR0CNTRL_MATSRC_SHIFT      | 
               (enable)                                        << GX_XF_COLOR0CNTRL_LIGHT_SHIFT       |
               (lightMask & 0b1111)                            << GX_XF_COLOR0CNTRL_LMASKLO_SHIFT     |
               (ambSrc)                                        << GX_XF_COLOR0CNTRL_AMBSRC_SHIFT      |
               (attnFn == GX_AF_SPEC ? GX_DF_NONE : diffuseFn) << GX_XF_COLOR0CNTRL_DIFFUSEATTN_SHIFT |
               (attnFn != GX_AF_NONE)                          << GX_XF_COLOR0CNTRL_ATTNENABLE_SHIFT  |
               (attnFn != GX_AF_SPEC)                          << GX_XF_COLOR0CNTRL_ATTNSELECT_SHIFT  |
               ((lightMask >> 4) & 0b1111)                     << GX_XF_COLOR0CNTRL_LMASKHI_SHIFT;
    // clang-format on

    *pCtrl = mask;

    if (id == GX_COLOR0A0 || id == GX_COLOR1A1) {
        rChan.paramChanCtrlA = mask;
    }
}

bool ResMatChan::GXGetChanMatColor(GXChannelID id, GXColor* pColor) const {
    *pColor = ref().chan[id & 1].matColor;
    return true;
}

bool ResMatChan::GXGetChanAmbColor(GXChannelID id, GXColor* pColor) const {
    *pColor = ref().chan[id & 1].ambColor;
    return true;
}

bool ResMatChan::GXGetChanCtrl(GXChannelID id, GXBool* pEnable,
                               GXColorSrc* pAmbSrc, GXColorSrc* pMatSrc,
                               GXLightID* pLightMask, GXDiffuseFn* pDiffuseFn,
                               GXAttnFn* pAttnFn) const {
    const Chan& rChan = ref().chan[id & 1];

    u32 ctrl;
    if ((id & 2) == 0) {
        ctrl = rChan.paramChanCtrlC;
    } else {
        ctrl = rChan.paramChanCtrlA;
    }

    if (pEnable != NULL) {
        *pEnable = ctrl >> GX_XF_COLOR0CNTRL_LIGHT_SHIFT &
                   GX_XF_COLOR0CNTRL_LIGHT_LMASK;
    }

    if (pAmbSrc != NULL) {
        *pAmbSrc =
            static_cast<GXColorSrc>(ctrl >> GX_XF_COLOR0CNTRL_AMBSRC_SHIFT &
                                    GX_XF_COLOR0CNTRL_AMBSRC_LMASK);
    }

    if (pMatSrc != NULL) {
        *pMatSrc =
            static_cast<GXColorSrc>(ctrl >> GX_XF_COLOR0CNTRL_MATSRC_SHIFT &
                                    GX_XF_COLOR0CNTRL_MATSRC_LMASK);
    }

    u32 lightMaskLo = ctrl >> GX_XF_COLOR0CNTRL_LMASKLO_SHIFT &
                      GX_XF_COLOR0CNTRL_LMASKLO_LMASK;

    u32 lightMaskHi = ctrl >> GX_XF_COLOR0CNTRL_LMASKHI_SHIFT &
                      GX_XF_COLOR0CNTRL_LMASKHI_LMASK;

    if (pLightMask != NULL) {
        *pLightMask = static_cast<GXLightID>(lightMaskLo | lightMaskHi << 4);
    }

    GXDiffuseFn diff;
    GXAttnFn attn;

    if (!(ctrl >> GX_XF_COLOR0CNTRL_ATTNSELECT_SHIFT &
          GX_XF_COLOR0CNTRL_ATTNSELECT_LMASK)) {
        attn = GX_AF_SPEC;
        diff = GX_DF_NONE;

    } else if (!(ctrl >> GX_XF_COLOR0CNTRL_ATTNENABLE_SHIFT &
                 GX_XF_COLOR0CNTRL_ATTNENABLE_LMASK)) {
        attn = GX_AF_NONE;
        diff = static_cast<GXDiffuseFn>(
            ctrl >> GX_XF_COLOR0CNTRL_DIFFUSEATTN_SHIFT &
            GX_XF_COLOR0CNTRL_DIFFUSEATTN_LMASK);

    } else {
        attn = GX_AF_SPOT;
        diff = static_cast<GXDiffuseFn>(
            ctrl >> GX_XF_COLOR0CNTRL_DIFFUSEATTN_SHIFT &
            GX_XF_COLOR0CNTRL_DIFFUSEATTN_LMASK);
    }

    if (pDiffuseFn != NULL) {
        *pDiffuseFn = diff;
    }

    if (pAttnFn != NULL) {
        *pAttnFn = attn;
    }

    return true;
}

ResMatChan ResMatChan::CopyTo(void* pDst) const {
    ResChanData* pData = static_cast<ResChanData*>(pDst);
    const ResChanData& r = ref();

    *pData = r;
    return ResMatChan(pData);
}

/******************************************************************************
 *
 * ResMatTexCoordGen
 *
 ******************************************************************************/
void ResMatTexCoordGen::CallDisplayList(u8 numGens, bool sync) const {
    if (numGens == 0) {
        return;
    }

    const ResTexCoordGenDL& r = ref();

    if (sync) {
        if (numGens < 2) {
            GXCallDisplayList(const_cast<ResTexCoordGenDL*>(&r), 32);
        } else if (numGens < 4) {
            GXCallDisplayList(const_cast<ResTexCoordGenDL*>(&r), 64);
        } else if (numGens < 8) {
            GXCallDisplayList(const_cast<ResTexCoordGenDL*>(&r), 128);
        } else {
            GXCallDisplayList(const_cast<ResTexCoordGenDL*>(&r),
                              sizeof(ResTexCoordGenDL));
        }
    } else {
        if (numGens < 2) {
            GXFastCallDisplayList(const_cast<ResTexCoordGenDL*>(&r), 32);
        } else if (numGens < 4) {
            GXFastCallDisplayList(const_cast<ResTexCoordGenDL*>(&r), 64);
        } else if (numGens < 8) {
            GXFastCallDisplayList(const_cast<ResTexCoordGenDL*>(&r), 128);
        } else {
            GXFastCallDisplayList(const_cast<ResTexCoordGenDL*>(&r),
                                  sizeof(ResTexCoordGenDL));
        }
    }
}

inline bool ResMatTexCoordGen::GXGetTexCoordGen2(GXTexCoordID id, GXTexGenType* pFunc,
                                          GXTexGenSrc* pParam,
                                          GXBool* pNormalize,
                                          u32* pPostMtx) const {
    const u8* pCmd = ref().dl.texCoordGen[id];
    if (pCmd[0] == 0) {
        return false;
    }

    u32 regTex;
    detail::ResReadXFCmd(&pCmd[GX_XF_CMD_SZ * 0], &regTex);

    u32 regDualTex;
    detail::ResReadXFCmd(&pCmd[GX_XF_CMD_SZ * 1], &regDualTex);

    if (pNormalize != NULL) {
        *pNormalize = regDualTex >> GX_XF_DUALTEX_NORMALIZE_SHIFT &
                      GX_XF_DUALTEX_NORMALIZE_LMASK;
    }

    if (pPostMtx != NULL) {
        // @note BASEROW is actually only six bits
        *pPostMtx =
            (regDualTex >> GX_XF_DUALTEX_BASEROW_SHIFT & 0xFF) + GX_PTTEXMTX0;
    }

    u32 row;
    u32 tgType;

    tgType = regTex >> GX_XF_TEX_TEXGENTYPE_SHIFT & GX_XF_TEX_TEXGENTYPE_LMASK;
    row = regTex >> GX_XF_TEX_SRCROW_SHIFT & GX_XF_TEX_SRCROW_LMASK;

    if (tgType == GX_XF_TG_REGULAR) {
        // clang-format off
        GXTexGenType type = static_cast<GXTexGenType>(
            (regTex >> GX_XF_TEX_PROJTYPE_SHIFT & GX_XF_TEX_PROJTYPE_LMASK) ^ 1);
        // clang-format on

        GXTexGenSrc param;

        switch (row) {
        case 0: {
            param = GX_TG_POS;
            break;
        }

        case 1: {
            param = GX_TG_NRM;
            break;
        }

        case 3: {
            param = GX_TG_BINRM;
            break;
        }

        case 4: {
            param = GX_TG_TANGENT;
            break;
        }

        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12: {
            param = static_cast<GXTexGenSrc>(row - 1);
            break;
        }

        case 2:
        default: {
            return false;
        }
        }

        if (pFunc != NULL) {
            *pFunc = type;
        }

        if (pParam != NULL) {
            *pParam = param;
        }

        return true;
    } else if (row == 2) {
        if (pFunc != NULL) {
            *pFunc = GX_TG_SRTG;
        }

        if (pParam != NULL) {
            *pParam = tgType == GX_XF_TG_CLR0 ? GX_TG_COLOR0 : GX_TG_COLOR1;
        }

        return true;
    } else if (tgType == GX_XF_TG_BUMP) {
        if (pFunc != NULL) {
            // @note Assumes BUMPSRCLIGHT is the leftmost field
            *pFunc = static_cast<GXTexGenType>(
                (regTex >> GX_XF_TEX_BUMPSRCLIGHT_SHIFT) + GX_TG_BUMP0);
        }

        if (pParam != NULL) {
            *pParam =
                static_cast<GXTexGenSrc>((regTex >> GX_XF_TEX_BUMPSRCTEX_SHIFT &
                                          GX_XF_TEX_BUMPSRCTEX_LMASK) +
                                         GX_TG_TEXCOORD0);
        }

        return true;
    } else {
        return false;
    }

    return false;
}

inline void ResMatTexCoordGen::GXSetTexCoordGen2(GXTexCoordID id, GXTexGenType func,
                                          GXTexGenSrc param, GXBool normalize,
                                          u32 postMtx) {
    u8* pCmd = ref().dl.texCoordGen[id];

    u32 form = GX_XF_TEX_FORM_AB11;
    u32 tgType;
    u32 proj = GX_XF_TEX_PROJ_ST;

    u32 row = 5;
    u32 embossRow = 5;
    u32 embossLit = 0;

    switch (param) {
    case GX_TG_POS: {
        row = 0;
        form = GX_XF_TEX_FORM_ABC1;
        break;
    }

    case GX_TG_NRM: {
        row = 1;
        form = GX_XF_TEX_FORM_ABC1;
        break;
    }

    case GX_TG_BINRM: {
        row = 3;
        form = GX_XF_TEX_FORM_ABC1;
        break;
    }

    case GX_TG_TANGENT: {
        row = 4;
        form = GX_XF_TEX_FORM_ABC1;
        break;
    }

    case GX_TG_COLOR0: {
        row = 2;
        break;
    }

    case GX_TG_COLOR1: {
        row = 2;
        break;
    }

    case GX_TG_TEX0: {
        row = 5;
        break;
    }

    case GX_TG_TEX1: {
        row = 6;
        break;
    }

    case GX_TG_TEX2: {
        row = 7;
        break;
    }

    case GX_TG_TEX3: {
        row = 8;
        break;
    }

    case GX_TG_TEX4: {
        row = 9;
        break;
    }

    case GX_TG_TEX5: {
        row = 10;
        break;
    }

    case GX_TG_TEX6: {
        row = 11;
        break;
    }

    case GX_TG_TEX7: {
        row = 12;
        break;
    }

    case GX_TG_TEXCOORD0: {
        embossRow = 0;
        break;
    }

    case GX_TG_TEXCOORD1: {
        embossRow = 1;
        break;
    }

    case GX_TG_TEXCOORD2: {
        embossRow = 2;
        break;
    }

    case GX_TG_TEXCOORD3: {
        embossRow = 3;
        break;
    }

    case GX_TG_TEXCOORD4: {
        embossRow = 4;
        break;
    }

    case GX_TG_TEXCOORD5: {
        embossRow = 5;
        break;
    }

    case GX_TG_TEXCOORD6: {
        embossRow = 6;
        break;
    }

    default: {
        break;
    }
    }

    switch (func) {
    case GX_TG_MTX2x4: {
        tgType = GX_XF_TG_REGULAR;
        break;
    }

    case GX_TG_MTX3x4: {
        tgType = GX_XF_TG_REGULAR;
        proj = GX_XF_TEX_PROJ_STQ;
        break;
    }

    case GX_TG_BUMP0:
    case GX_TG_BUMP1:
    case GX_TG_BUMP2:
    case GX_TG_BUMP3:
    case GX_TG_BUMP4:
    case GX_TG_BUMP5:
    case GX_TG_BUMP6:
    case GX_TG_BUMP7: {
        tgType = GX_XF_TG_BUMP;
        embossLit = func - GX_TG_BUMP0;
        break;
    }

    case GX_TG_SRTG: {
        if (param == GX_TG_COLOR0) {
            tgType = GX_XF_TG_CLR0;
        } else /* GX_TG_COLOR1 */ {
            tgType = GX_XF_TG_CLR1;
        }
        break;
    }

    default: {
        return;
    }
    }

    // clang-format off
    detail::ResWriteXFCmd(&pCmd[GX_XF_CMD_SZ * 0], id + GX_XF_REG_TEX0,
        proj      << GX_XF_TEX_PROJTYPE_SHIFT   |
        form      << GX_XF_TEX_INPUTFORM_SHIFT  |
        tgType    << GX_XF_TEX_TEXGENTYPE_SHIFT |
        row       << GX_XF_TEX_SRCROW_SHIFT     |
        embossRow << GX_XF_TEX_BUMPSRCTEX_SHIFT |
        embossLit << GX_XF_TEX_BUMPSRCLIGHT_SHIFT);

    detail::ResWriteXFCmd(&pCmd[GX_XF_CMD_SZ * 1], id + GX_XF_REG_DUALTEX0,
        (postMtx - GX_PTTEXMTX0) << GX_XF_DUALTEX_BASEROW_SHIFT |
        normalize                << GX_XF_DUALTEX_NORMALIZE_SHIFT);
    // clang-format on
}

/******************************************************************************
 *
 * ResMat
 *
 ******************************************************************************/
bool ResMat::Bind(const ResFile file) {
    u32 i;
    u32 bindNum = 0;

    u32 texPlttInfoNum = GetNumResTexPlttInfo();
    for (i = 0; i < texPlttInfoNum; i++) {
        if (GetResTexPlttInfo(i).Bind(file, GetResTexObj(), GetResTlutObj())) {
            bindNum++;
        }
    }

    return bindNum == texPlttInfoNum;
}

void ResMat::Release() {
    u32 i;

    u32 texPlttInfoNum = GetNumResTexPlttInfo();
    for (i = 0; i < texPlttInfoNum; i++) {
        GetResTexPlttInfo(i).Release(GetResTexObj(), GetResTlutObj());
    }
}

void ResMat::Init() {
    DC::StoreRangeNoSync(GetResMatDLData(), sizeof(ResMatDLData));
}

ResTev ResMat::GetResTev() {
    return ofs_to_obj<ResTev>(ref().toResTevData);
}

inline ResTev ResMat::GetResTev() const {
    return ofs_to_obj<ResTev>(ref().toResTevData);
}

/******************************************************************************
 *
 * ResTexPlttInfo
 *
 ******************************************************************************/
inline void ResTexPlttInfo::BindTex_(const ResTex tex, ResTexObj texObj) {
    ResTexPlttInfoData& r = ref();
    r.pTexData = const_cast<ResTexData*>(&tex.ref());

    void* pTexData;
    u16 width, height;
    f32 minLod, maxLod;
    GXBool mipmap;

    GXTexObj* pGXObj = texObj.GetTexObj(r.mapID);

    if (IsCIFmt()) {
        GXCITexFmt fmtCi;
        tex.GetTexObjCIParam(&pTexData, &width, &height, &fmtCi, &minLod,
                             &maxLod, &mipmap);

        GXInitTexObjCI(pGXObj, pTexData, width, height,
                       static_cast<GXTexFmt>(fmtCi), r.wrap_s, r.wrap_t, mipmap,
                       r.tlutID);
    } else {
        GXTexFmt fmt;
        tex.GetTexObjParam(&pTexData, &width, &height, &fmt, &minLod, &maxLod,
                           &mipmap);

        GXInitTexObj(pGXObj, pTexData, width, height, fmt, r.wrap_s, r.wrap_t,
                     mipmap);
    }

    GXInitTexObjLOD(pGXObj, r.min_filt, r.mag_filt, minLod, maxLod, r.lod_bias,
                    r.bias_clamp, r.do_edge_lod, r.max_aniso);

    texObj.Validate(r.mapID);
}

inline void ResTexPlttInfo::BindPltt_(const ResPltt pltt, ResTlutObj tlutObj) {
    ResTexPlttInfoData& r = ref();
    r.pPlttData = const_cast<ResPlttData*>(&pltt.ref());

    // @bug Shouldn't this be the TLUT id?
    GXTlutObj* pGXObj = tlutObj.GetTlut(static_cast<GXTlut>(r.mapID));

    GXInitTlutObj(pGXObj, const_cast<u16*>(pltt.GetPlttData()), pltt.GetFmt(),
                  pltt.GetNumEntries());

    // @bug Shouldn't this be the TLUT id?
    tlutObj.Validate(static_cast<GXTlut>(r.mapID));
}

bool ResTexPlttInfo::Bind(const ResFile file, ResTexObj texObj,
                          ResTlutObj tlutObj) {
    bool success = true;
    ResName texName = GetTexResName();

    if (ref().pTexData == NULL) {
        ResTex tex = file.GetResTex(texName);

        if (tex.IsValid() && (!IsCIFmt() || tex.IsCIFmt())) {
            BindTex_(tex, texObj);
        } else {
            success = false;
        }
    }

    if (ref().pPlttData == NULL) {
        ResName plttName = GetPlttResName();

        if (plttName.IsValid()) {
            ResPltt pltt = file.GetResPltt(plttName);

            if (pltt.IsValid()) {
                BindPltt_(pltt, tlutObj);
            } else {
                success = false;
            }
        }
    }

    return success;
}

inline void ResTexPlttInfo::Release(ResTexObj texObj, ResTlutObj tlutObj) {
    ResTexPlttInfoData& r = ref();

    r.pTexData = NULL;
    r.pPlttData = NULL;

    texObj.Invalidate(r.mapID);
    // @bug Shouldn't this be the TLUT id?
    tlutObj.Invalidate(static_cast<GXTlut>(r.mapID));
}

/******************************************************************************
 *
 * ResMatFur
 *
 ******************************************************************************/
f32 ResMatFur::GetLyrRate(u32 layer) const {
    if (IsValid()) {
        u32 value = ref().value;

        switch (ref().type) {
        case 0:
            return (f32)(layer + 1) / (f32)value;
        case 1:
            return (f32)pow((f32)(layer + 1) / (f32)value, lbl_eu_80669A70);
        default:
            return 0.0f;
        }
    }

    return 0.0f;
}

/******************************************************************************
 *
 * ResMat (fur / user data accessors)
 *
 ******************************************************************************/
ResMatFur ResMat::GetResMatFur() {
    return ResMatFur(ofs_to_ptr<ResMatFurData>(ref().toResMatFurData));
}

void* ResMat::GetResUserData() {
    return ofs_to_ptr<void>(ref().toResUserData);
}

} // namespace g3d
} // namespace nw4r


// Absorbed sdata2 pool for resmat (shared pool 0x80669A68-0x8066A178)
// Retail 0x710 bytes. Emitted as a single sdata2 array; individual
// lbl_eu_ symbols are added via postprocess add_symbols (see UNIT_RULES).
__declspec(section ".sdata2") __attribute__((aligned(8)))
unsigned char sdata2_pool_g3d_resmat[1808] = {
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0xE3, 0x33, 0x33, 0x40, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x3A, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x44, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x36, 0x0B, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x3F, 0x36, 0x0B, 0x61,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xB4, 0x00, 0x00, 0x42, 0xB4, 0x00, 0x00,
    0xC2, 0xB4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xAA, 0x24, 0x25,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x21, 0x93, 0x92, 0xEF, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x15, 0x02, 0xF9,
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x03, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x44, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0xBF, 0x80, 0x00, 0x00, 0x35, 0x86, 0x37, 0xBD,
    0x3F, 0x80, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x3B, 0x80, 0x80, 0x81, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x21, 0x93, 0x92, 0xEF, 0x21, 0x93, 0x92, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x70, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x42, 0x70, 0x00, 0x00, 0x3F, 0xAA, 0xAA, 0xAB, 0x3D, 0xCC, 0xCC, 0xCD,
    0x44, 0x7A, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0x80, 0x00, 0x00,
    0x3F, 0x36, 0x0B, 0x61, 0x3F, 0x36, 0x0B, 0x61, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x36, 0x0B, 0x61, 0x3F, 0x00, 0x00, 0x00, 0xBF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x36, 0x0B, 0x61, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x36, 0x0B, 0x61, 0xBF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0x4A, 0x74, 0x24, 0x00, 0xBF, 0x80, 0x00, 0x00, 0x42, 0xB4, 0x00, 0x00, 0x41, 0x20, 0x00, 0x00,
    0x41, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3C, 0x8E, 0xFA, 0x35, 0x3B, 0x80, 0x80, 0x81, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00,
    0x3F, 0x36, 0x0B, 0x61, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0xBA, 0x83, 0x12, 0x6F, 0x3A, 0x83, 0x12, 0x6F, 0x3F, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
    0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0xB8, 0xAA, 0x3B, 0x3F, 0x31, 0x72, 0x18, 0x41, 0xB8, 0xAA, 0x3B, 0x43, 0x80, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x3F, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x42, 0x00, 0x00, 0x00, 0x42, 0x80, 0x00, 0x00, 0xC2, 0x80, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3C, 0xC9, 0x0F, 0xDB, 0x00, 0x00, 0x00, 0x00,
    0x47, 0x80, 0x00, 0x00, 0x47, 0x80, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0xC9, 0x0F, 0xDB,
    0x3F, 0x36, 0x0B, 0x61, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00,
    0x46, 0xFA, 0x00, 0x00, 0x46, 0xFA, 0x00, 0x00, 0x3F, 0xAA, 0xAA, 0xAB, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x47, 0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x80, 0x00, 0x00,
    0x46, 0xFF, 0xFE, 0x00, 0x42, 0xFE, 0x00, 0x00, 0x42, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x43, 0x7F, 0x00, 0x00, 0x40, 0xC0, 0x00, 0x00,
    0x43, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0xC2, 0xB4, 0xCC, 0xCD, 0x47, 0x7F, 0xFF, 0x00, 0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xBD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x41, 0xC0, 0x00, 0x00,
    0x3C, 0x00, 0x00, 0x00, 0x40, 0xA0, 0x00, 0x00, 0x42, 0x70, 0x00, 0x00, 0x41, 0x20, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x48, 0x00, 0x00, 0x3D, 0xCC, 0xCC, 0xCD, 0x40, 0x00, 0x00, 0x00,
    0x3F, 0x7D, 0x70, 0xA4, 0x43, 0x20, 0x00, 0x00, 0x3E, 0xCC, 0xCC, 0xCD, 0x3F, 0x80, 0x00, 0x00, 0x3D, 0x23, 0xD7, 0x0A,
    0x3F, 0x7D, 0x70, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x3C, 0xA3, 0xD7, 0x0A, 0x40, 0x40, 0x00, 0x00, 0x3F, 0x19, 0x99, 0x9A,
    0x3E, 0xCC, 0xCC, 0xCD, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xC8, 0x00, 0x00,
    0x44, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00,
    0x42, 0x00, 0x00, 0x00, 0x42, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3C, 0x00, 0x00, 0x00, 0x3E, 0xC8, 0x00, 0x00, 0x3C, 0x80, 0x00, 0x00, 0x42, 0xFE, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x47, 0x6A, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x42, 0xFE, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x42, 0x7C, 0x00, 0x00, 0xBF, 0x80, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x42, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x42, 0xFE, 0x00, 0x00,
    0x42, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0xF5, 0xC2, 0x8F, 0x3F, 0x61, 0x47, 0xAE,
    0x3F, 0x00, 0x00, 0x00, 0x3F, 0x8F, 0x5C, 0x29, 0x3F, 0x59, 0x99, 0x9A, 0x3E, 0x19, 0x99, 0x98, 0x40, 0x00, 0x00, 0x00,
    0x3E, 0xB3, 0x33, 0x34, 0xBF, 0x80, 0x00, 0x00, 0xC0, 0x40, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x40, 0xC0, 0x00, 0x00, 0xC2, 0xB4, 0xCC, 0xCD,
    0x41, 0x20, 0x00, 0x00, 0xBF, 0x80, 0x00, 0x00, 0x43, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x0A, 0xDE, 0x7F,
    0x3F, 0x66, 0x66, 0x66, 0x3D, 0x08, 0x88, 0x89, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x42, 0x7C, 0x00, 0x00, 0x3C, 0x80, 0x00, 0x00, 0x42, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
    0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x43, 0x30, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
