#include <nw4hbm/lyt/lyt_common.h>
#include <nw4hbm/lyt.h>
#include <nw4hbm/math.h>
#include <nw4hbm/ut.h>

#include <revolution/GX.h>
#include <revolution/TPL.h>

#include <cstddef>
#include <cstring>


namespace nw4hbm {
namespace lyt {
namespace detail {

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
bool EqualsResName(const char* pLhs, const char* pRhs) {
    return std::strncmp(pLhs, pRhs, NW4R_LYT_RES_NAME_LEN) == 0;
}

bool EqualsMaterialName(const char* pLhs, const char* pRhs) {
    return std::strncmp(pLhs, pRhs, NW4R_LYT_RES_NAME_LEN) == 0;
}

bool EqualsPaneName(const char* pLhs, const char* pRhs) {
    return std::strncmp(pLhs, pRhs, NW4R_LYT_RES_NAME_LEN) == 0;
}

bool TestFileHeader(const res::BinaryFileHeader& rHeader) {
    return rHeader.byteOrder == 0xFEFF && rHeader.version == 8;
}

bool TestFileHeader(const res::BinaryFileHeader& rHeader, u32 signature) {
    return GetSignatureInt(rHeader.signature) == signature &&
           rHeader.byteOrder == 0xFEFF &&
           rHeader.version == 8;
}

/******************************************************************************
 *
 * TexCoordAry
 *
 ******************************************************************************/
TexCoordAry::TexCoordAry() : mCap(0), mNum(0), mpData(NULL) {}

void TexCoordAry::Free() {
    if (mpData != NULL) {
        Layout::FreeMemory(mpData);
        mpData = NULL;
        mCap = 0;
        mNum = 0;
    }
}

void TexCoordAry::Reserve(u8 num) {
    if (mCap < num) {
        if (mpData != NULL) {
            Layout::FreeMemory(mpData);
            mpData = NULL;
            mCap = 0;
            mNum = 0;
        }
        mpData = static_cast<TexCoord*>(Layout::AllocMemory(num * sizeof(TexCoord)));
        if (mpData != NULL) {
            mCap = num;
        }
    }
}

void TexCoordAry::SetSize(u8 num) {
    if (mpData != NULL && num <= mCap) {
        static const math::VEC2 sDefault[4] = {
            math::VEC2(0.0f, 0.0f),
            math::VEC2(1.0f, 0.0f),
            math::VEC2(0.0f, 1.0f),
            math::VEC2(1.0f, 1.0f)
        };
        for (int j = mNum; j < num; j++) {
            for (int i = 0; i < 4; i++) {
                mpData[j][i] = sDefault[i];
            }
        }
        mNum = num;
    }
}

void TexCoordAry::SetCoord(u32 idx, const math::VEC2* coord) {
    for(int i = 0; i < VERTEXCOLOR_MAX; i++)
    {
        mpData[idx][i] = coord[i];
    }
}

void TexCoordAry::Copy(const void* pSrc, u8 num) {
    mNum = ut::Max<u8>(mNum, num);
    const TexCoord* pSrcCoords = static_cast<const TexCoord*>(pSrc);

    for (int j = 0; j < num; j++) {
        for (int i = 0; i < VERTEXCOLOR_MAX; i++) {
            mpData[j][i] = pSrcCoords[j][i];
        }
    }
}

void DrawLine(const math::VEC2& pos, const Size& size, ut::Color color) {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG,
                  GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, color);

    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                  GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE,
                          GX_CH_ALPHA);

    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
                   GX_LO_SET);
    GXSetLineWidth(6, 0);

    // Draw rectangle outline as line strip: TL -> TR -> BR -> BL -> TL
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
    {
        GXPosition2f32(pos.x, pos.y);
        GXPosition2f32(pos.x + size.width, pos.y);
        GXPosition2f32(pos.x + size.width, pos.y + size.height);
        GXPosition2f32(pos.x, pos.y + size.height);
        GXPosition2f32(pos.x, pos.y);
    }
    GXEnd();
}

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
bool IsModulateVertexColor(ut::Color* pColors, u8 glbAlpha) {
    if (glbAlpha != 255) {
        return true;
    }

    if (pColors != NULL && (pColors[0] != 0xFFFFFFFF ||
                            pColors[1] != 0xFFFFFFFF ||
                            pColors[2] != 0xFFFFFFFF ||
                            pColors[3] != 0xFFFFFFFF)) {
        return true;
    }

    return false;
}

ut::Color MultipleAlpha(ut::Color color, u8 alpha) {
    ut::Color result = color;

    if (alpha != 255) {
        result.a = color.a * alpha / 255;
    }

    return result;
}

void MultipleAlpha(ut::Color* pDst, const ut::Color* pSrc, u8 alpha) {
    for (int i = 0; i < VERTEXCOLOR_MAX; i++) {
        pDst[i] = MultipleAlpha(pSrc[i], alpha);
    }
}

void SetVertexFormat(bool modulate, u8 numCoord) {
    GXClearVtxDesc();

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

    if (modulate) {
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    }

    for (int i = 0; i < numCoord; i++) {
        GXSetVtxDesc(static_cast<GXAttr>(GX_VA_TEX0 + i), GX_DIRECT);
    }

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);

    if (modulate) {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    }

    for (int i = 0; i < numCoord; i++) {
        GXSetVtxAttrFmt(GX_VTXFMT0, static_cast<GXAttr>(GX_VA_TEX0 + i),
                        GX_TEX_ST, GX_F32, 0);
    }
}

void DrawQuad(const math::VEC2& rBase, const Size& rSize, u8 num,
              const TexCoord* pCoords, const ut::Color* pColors) {

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    {
        GXPosition2f32(rBase.x, rBase.y);
        if (pColors != NULL) {
            GXColor1u32(pColors[VERTEXCOLOR_LT]);
        }
        for (int i = 0; i < num; i++) {
            GXTexCoord2f32(pCoords[i][VERTEXCOLOR_LT].x,
                           pCoords[i][VERTEXCOLOR_LT].y);
        }

        GXPosition2f32(rBase.x + rSize.width, rBase.y);
        if (pColors != NULL) {
            GXColor1u32(pColors[VERTEXCOLOR_RT]);
        }
        for (int i = 0; i < num; i++) {
            GXTexCoord2f32(pCoords[i][VERTEXCOLOR_RT].x,
                           pCoords[i][VERTEXCOLOR_RT].y);
        }

        GXPosition2f32(rBase.x + rSize.width, rBase.y + rSize.height);
        if (pColors != NULL) {
            GXColor1u32(pColors[VERTEXCOLOR_RB]);
        }
        for (int i = 0; i < num; i++) {
            GXTexCoord2f32(pCoords[i][VERTEXCOLOR_RB].x,
                           pCoords[i][VERTEXCOLOR_RB].y);
        }

        GXPosition2f32(rBase.x, rBase.y + rSize.height);
        if (pColors != NULL) {
            GXColor1u32(pColors[VERTEXCOLOR_LB]);
        }
        for (int i = 0; i < num; i++) {
            GXTexCoord2f32(pCoords[i][VERTEXCOLOR_LB].x,
                           pCoords[i][VERTEXCOLOR_LB].y);
        }
    }
    GXEnd();
}

void DrawQuad(const math::VEC2& rBase, const Size& rSize, u8 num,
              const TexCoord* pCoords, const ut::Color* pColors, u8 alpha) {

    ut::Color colorWork[VERTEXCOLOR_MAX];

    if (pColors != NULL) {
        MultipleAlpha(colorWork, pColors, alpha);
    }

    DrawQuad(rBase, rSize, num, pCoords, pColors ? colorWork : NULL);
}

/******************************************************************************
 *
 * InitGXTexObjFromTPL
 *
 ******************************************************************************/
void InitGXTexObjFromTPL(GXTexObj* pTexObj, TPLPalette* pTpl, u32 idx) {
    // Resolve TPL file addresses if not already linked
    if (reinterpret_cast<u32>(pTpl->descriptorArray) < 0x80000000) {
        TPLBind(pTpl);
    }

    TPLDescriptor* pDesc = TPLGet(pTpl, idx);
    TPLHeader* pHeader = pDesc->textureHeader;

    // Mipmap if the texture has multiple LOD levels
    GXBool mipmap = pHeader->minLOD != pHeader->maxLOD;

    if (pDesc->CLUTHeader != NULL) {
        GXInitTexObjCI(pTexObj, pHeader->data, pHeader->width, pHeader->height,
                       static_cast<GXTexFmt>(pHeader->format), pHeader->wrapS,
                       pHeader->wrapT, mipmap, 0);
        GXInitTexObjUserData(pTexObj, pDesc->CLUTHeader);
    } else {
        GXInitTexObj(pTexObj, pHeader->data, pHeader->width, pHeader->height,
                     static_cast<GXTexFmt>(pHeader->format), pHeader->wrapS,
                     pHeader->wrapT, mipmap);
    }

    // Set up LOD — reload header pointer since r9 clobbered by GXInitTexObj*
    pHeader = pDesc->textureHeader;
    GXInitTexObjLOD(pTexObj, pHeader->minFilter, pHeader->magFilter,
                    static_cast<f32>(pHeader->minLOD),
                    static_cast<f32>(pHeader->maxLOD),
                    pHeader->LODBias, false, pHeader->edgeLODEnable,
                    GX_ANISO_1);
}

} // namespace detail
} // namespace lyt
} // namespace nw4hbm
