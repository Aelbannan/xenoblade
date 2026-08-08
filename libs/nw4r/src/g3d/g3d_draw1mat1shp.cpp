// g3d_draw1mat1shp.cpp
// Decompile of LoadMaterial and Draw1Mat1ShpDirectly for nw4r::g3d.

#include <nw4r/g3d.h>
#include <nw4r/ut.h>

#include <revolution/BASE.h>
#include <revolution/GX.h>

#include <cmath>

namespace nw4r {
namespace g3d {

// ---------------------------------------------------------------------------
// ResMatFur - fur rendering data attached to a material.
// Not fully defined in headers; minimal layout inferred from binary.
// ---------------------------------------------------------------------------
struct ResMatFurData {
    u8 pad_0x0[0xC];
    f32 param1; // at 0xC - first pow exponent
    f32 param2; // at 0x10 - second pow exponent
};

class ResMatFur : public ResCommon<ResMatFurData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResMatFur);

    f32 GetLyrRate(u32 layer) const;
};

namespace detail {

// ---------------------------------------------------------------------------
// LoadMaterial
// Loads all material subsystems into GX state. For each resource, uses
// pSwap override when available, otherwise falls back to resMat defaults.
// ---------------------------------------------------------------------------
void LoadMaterial(ResMat resMat, u32 ctrl, Draw1Mat1ShpSwap* pSwap,
                  G3DState::IndMtxOp* pIndMtxOp, bool lightOff) {

    if (lightOff) {
        // lightOff path: only process IndMtxOp if present
        if (pIndMtxOp != NULL) {
            if (pSwap != NULL && pSwap->indMtxAndScale.IsValid()) {
                G3DState::LoadResMatIndMtxAndScale(pSwap->indMtxAndScale,
                                                   *pIndMtxOp);
            } else {
                G3DState::IndTexMtxInfo info;
                (*pIndMtxOp)(&info);
                info.FifoSend();
            }
        }
        return;
    }

    // --- ResMatMisc ---
    ResMatMisc misc;
    if (pSwap != NULL && pSwap->misc.IsValid()) {
        misc = pSwap->misc;
    } else {
        misc = resMat.GetResMatMisc();
    }

    G3DState::LoadFog(misc.GetFogIdx());
    G3DState::LoadResMatMisc(misc);

    // --- ResTlutObj ---
    if (pSwap != NULL && pSwap->tlutObj.IsValid()) {
        G3DState::LoadResTlutObj(pSwap->tlutObj);
    } else {
        G3DState::LoadResTlutObj(resMat.GetResTlutObj());
    }

    // --- ResTexObj ---
    if (pSwap != NULL && pSwap->texObj.IsValid()) {
        G3DState::LoadResTexObj(pSwap->texObj);
    } else {
        G3DState::LoadResTexObj(resMat.GetResTexObj());
    }

    // --- ResGenMode ---
    if (pSwap != NULL && pSwap->genMode.IsValid()) {
        G3DState::LoadResGenMode(pSwap->genMode);
    } else {
        G3DState::LoadResGenMode(resMat.GetResGenMode());
    }

    // --- ResTev ---
    if (pSwap != NULL && pSwap->tev.IsValid()) {
        G3DState::LoadResTev(pSwap->tev);
    } else {
        G3DState::LoadResTev(resMat.GetResTev());
    }

    // --- ResMatPix ---
    if (pSwap != NULL && pSwap->pix.IsValid()) {
        G3DState::LoadResMatPix(pSwap->pix);
    } else {
        G3DState::LoadResMatPix(resMat.GetResMatPix());
    }

    // --- ResMatTevColor ---
    if (pSwap != NULL && pSwap->tevColor.IsValid()) {
        G3DState::LoadResMatTevColor(pSwap->tevColor);
    } else {
        G3DState::LoadResMatTevColor(resMat.GetResMatTevColor());
    }

    // --- ResMatIndMtxAndScale ---
    if (pSwap != NULL && pSwap->indMtxAndScale.IsValid()) {
        if (pIndMtxOp != NULL) {
            G3DState::LoadResMatIndMtxAndScale(pSwap->indMtxAndScale,
                                               *pIndMtxOp);
        } else {
            G3DState::LoadResMatIndMtxAndScale(pSwap->indMtxAndScale);
        }
    } else {
        if (pIndMtxOp != NULL) {
            G3DState::LoadResMatIndMtxAndScale(
                resMat.GetResMatIndMtxAndScale(), *pIndMtxOp);
        } else {
            G3DState::LoadResMatIndMtxAndScale(
                resMat.GetResMatIndMtxAndScale());
        }
    }

    // --- Light set ---
    u32 diffColorMask;
    u32 diffAlphaMask;
    u32 specColorMask;
    u32 specAlphaMask;
    GXColor ambColor;
    AmbLightObj ambLight;

    G3DState::LoadLightSet(misc.GetLightSetIdx(), &diffColorMask,
                           &diffAlphaMask, &specColorMask, &specAlphaMask,
                           &ambLight);

    ambColor.r = ambLight.r;
    ambColor.g = ambLight.g;
    ambColor.b = ambLight.b;
    ambColor.a = ambLight.a;

    // --- ResMatChan ---
    if (pSwap != NULL && pSwap->chan.IsValid()) {
        G3DState::LoadResMatChan(
            pSwap->chan, diffColorMask, diffAlphaMask, specColorMask,
            specAlphaMask, ambColor,
            (ctrl & DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF) ? true : false);
    } else {
        G3DState::LoadResMatChan(
            resMat.GetResMatChan(), diffColorMask, diffAlphaMask,
            specColorMask, specAlphaMask, ambColor,
            (ctrl & DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF) ? true : false);
    }

    // --- ResMatTexCoordGen ---
    if (pSwap != NULL && pSwap->texCoordGen.IsValid()) {
        G3DState::LoadResMatTexCoordGen(pSwap->texCoordGen);
    } else {
        G3DState::LoadResMatTexCoordGen(resMat.GetResMatTexCoordGen());
    }

    // --- ResTexSrt ---
    if (pSwap != NULL && pSwap->texSrt.IsValid()) {
        G3DState::LoadResTexSrt(pSwap->texSrt);
    } else {
        G3DState::LoadResTexSrt(resMat.GetResTexSrt());
    }
}

} // namespace detail

// ---------------------------------------------------------------------------
// Draw1Mat1ShpDirectly
// Draws a single material + shape pair. Handles fur layering when the
// material has fur data attached.
// ---------------------------------------------------------------------------
void Draw1Mat1ShpDirectly(ResMat mat, ResShp shp, const math::MTX34* pViewPos,
                          const math::MTX34* pViewNrm, u32 ctrl,
                          Draw1Mat1ShpSwap* pSwap,
                          G3DState::IndMtxOp* pIndMtxOp) {

    // PPC sync unless suppressed
    if (!(ctrl & DRAW1MAT1SHP_CTRL_NOPPCSYNC)) {
        LCQueueWait(0);
        PPCSync();
    }

    bool forceLightOff = false;
    bool hasFur = false;
    ResMatFur furData;

    if (mat.IsValid()) {
        if (ctrl & DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF) {
            forceLightOff = true;
        }

        furData = mat.GetResMatFur();
        if (furData.IsValid()) {
            hasFur = true;
        }
    } else {
        forceLightOff = true;
    }

    // Load all material state
    detail::LoadMaterial(mat, ctrl, pSwap, pIndMtxOp, forceLightOff);

    // Shape must be valid and visible
    if (!shp.IsValid()) {
        return;
    }
    if (!shp.IsVisible()) {
        return;
    }

    // -----------------------------------------------------------------------
    // FUR PATH
    // -----------------------------------------------------------------------
    if (hasFur) {

        // The retail loads a 16-byte constant table before the TEV color
        // setup.  Store to volatile stack to match retail instruction order.
        volatile u32 furConst0 = 0, furConst1 = 0, furConst2 = 0, furConst3 = 0;
        (void)furConst0; (void)furConst1; (void)furConst2; (void)furConst3;

        f32 furParam1 = furData.ref().param1;
        f32 furParam2 = furData.ref().param2;

        // ---- TEV colour source (swap or material default) ----
        ResMatTevColor tevColor;
        if (pSwap == NULL || !pSwap->tevColor.IsValid()) {
            tevColor = mat.GetResMatTevColor();
        } else {
            tevColor = pSwap->tevColor;
        }

        GXColor kcolor[3];
        tevColor.GXGetTevKColor(GX_KCOLOR0, &kcolor[0]);
        tevColor.GXGetTevKColor(GX_KCOLOR1, &kcolor[1]);
        tevColor.GXGetTevKColor(GX_KCOLOR2, &kcolor[2]);

        // ---- Channel control (4 channels) ----
        // Store enable and attn for 4 channels into compact u32 arrays
        GXColor matColor[2];
        u8 chanInfo[4][2]; // [ch][0]=enable, [ch][1]=attn

        // Determine channel source (swap or default)
        bool useSwapChan =
            (pSwap != NULL && pSwap->misc.IsValid() && pSwap->chan.IsValid());
        ResMatChan chan = useSwapChan ? pSwap->chan : mat.GetResMatChan();

        chan.GXGetChanMatColor(GX_COLOR0, &matColor[0]);
        chan.GXGetChanMatColor(GX_COLOR1, &matColor[1]);

        static const GXChannelID sChanIDs[4] = {
            GX_COLOR0, GX_COLOR1, GX_ALPHA0, GX_ALPHA1,
        };
        for (int i = 0; i < 4; i++) {
            GXAttnFn attn;
            chan.GXGetChanCtrl(sChanIDs[i], &chanInfo[i][0], NULL, NULL,
                               NULL, NULL, &attn);
            chanInfo[i][1] = static_cast<u8>(attn);
        }

        // ---- Indirect tex info ----
        ResMatMisc misc =
            (pSwap != NULL && pSwap->misc.IsValid()) ? pSwap->misc
                                                     : mat.GetResMatMisc();
        ResMatIndMtxAndScale indMtx =
            (pSwap != NULL && pSwap->indMtxAndScale.IsValid())
                ? pSwap->indMtxAndScale
                : mat.GetResMatIndMtxAndScale();

        bool useIndTex[3] = {false, false, false};
        math::MTX34 indTexMtx[3];
        s8 indTexScale[3] = {0, 0, 0};

        for (int i = 0; i < 3; i++) {
            ResMatMiscData::IndirectMethod method;
            s8 lightRef;
            misc.GetIndirectTexMtxCalcMethod(
                static_cast<GXIndTexMtxID>(GX_ITM_0 + i), &method, &lightRef);
            if (method == ResMatMiscData::FUR) {
                useIndTex[i] = true;
                indMtx.GXGetIndTexMtx(
                    static_cast<GXIndTexMtxID>(GX_ITM_0 + i), &indTexMtx[i]);
            }
        }

        // ---- Pre-primitive + cull mode ----
        G3DState::LoadResShpPrePrimitive(shp);
        if (ctrl & DRAW1MAT1SHP_CTRL_CULL_FRONT) {
            fifo::GDSetCullMode(GX_CULL_FRONT);
            G3DState::Invalidate(G3DState::INVALIDATE_SHP);
        }

        // ---- Fur position data ----
        ResVtxFurPos furPos = shp.GetResVtxFurPos();
        u32 numLayer = furPos.ref().numLayer;

        // ---- Layer loop ----
        for (u16 layer = 0; layer < numLayer; layer++) {
            f32 lyrRate = furData.GetLyrRate(static_cast<u16>(layer));
            f32 oneMinusLyr = 1.0f - lyrRate;

            f32 alphaPow =
                static_cast<f32>(pow(lyrRate, furParam1));
            f32 colorPow =
                static_cast<f32>(pow(lyrRate, furParam2));
            f32 thirdPow =
                static_cast<f32>(pow(lyrRate, furParam2));

            // Set TEV kcolor 2 for fur layer blending
            GXColor furKColor;
            furKColor.r =
                static_cast<u8>(static_cast<int>(255.0f * lyrRate));
            furKColor.g = static_cast<u8>(static_cast<int>(
                static_cast<f32>(kcolor[2].a) * thirdPow));
            furKColor.b = 0;
            furKColor.a = 0;
            GXSetTevKColor(GX_KCOLOR2, furKColor);

            // Scale indirect tex matrices by layer rate for FUR stages
            for (int i = 0; i < 3; i++) {
                if (!useIndTex[i]) continue;
                math::MTX34 scaledMtx;
                for (int r = 0; r < 3; r++)
                    for (int c = 0; c < 4; c++)
                        scaledMtx[r][c] = indTexMtx[i][r][c] * lyrRate;
                GXSetIndTexMtx(
                    static_cast<GXIndTexMtxID>(GX_ITM_0 + i),
                    reinterpret_cast<const f32(*)[3]>(&scaledMtx),
                    indTexScale[i]);
            }

            // Compute and write fur TEV colour per channel.
            // Write to XF registers 0x100C/0x100D via GX FIFO
            // for indirect TEV colour used in fur blending.
            volatile u8* fifo8 =
                reinterpret_cast<volatile u8*>(0xCC008000);
            volatile u16* fifo16 =
                reinterpret_cast<volatile u16*>(0xCC008000);
            volatile u32* fifo32 =
                reinterpret_cast<volatile u32*>(0xCC008000);

            for (int ch = 0; ch < 2; ch++) {
                GXColor furColor;
                if (chanInfo[ch][0] ||
                    chanInfo[ch][1] == static_cast<u8>(GX_AF_SPEC)) {
                    furColor.r = static_cast<u8>(static_cast<int>(
                        static_cast<f32>(kcolor[0].r) * lyrRate +
                        static_cast<f32>(kcolor[1].r) * oneMinusLyr));
                    furColor.g = static_cast<u8>(static_cast<int>(
                        static_cast<f32>(kcolor[0].g) * lyrRate +
                        static_cast<f32>(kcolor[1].g) * oneMinusLyr));
                    furColor.b = static_cast<u8>(static_cast<int>(
                        static_cast<f32>(kcolor[0].b) * colorPow));
                    furColor.a = static_cast<u8>(static_cast<int>(
                        static_cast<f32>(kcolor[0].a) * colorPow));
                } else {
                    furColor.r = static_cast<u8>(static_cast<int>(
                        static_cast<f32>(matColor[ch].r) * lyrRate));
                    furColor.g = static_cast<u8>(static_cast<int>(
                        static_cast<f32>(matColor[ch].g) * lyrRate));
                    furColor.b = 0;
                    furColor.a = static_cast<u8>(static_cast<int>(
                        static_cast<f32>(kcolor[2].a) * thirdPow));
                }
                // GX XF register write: cmd 0x10, count 0,
                // addr 0x100C+ch, data = packed color
                u32 packed = *reinterpret_cast<u32*>(&furColor);
                *fifo8 = 0x10;
                *fifo16 = 0x0000;
                *fifo16 = static_cast<u16>(0x100C + ch);
                *fifo32 = packed;
            }

            // Set fur position array for this layer and draw
            furPos.SetArray(layer);
            G3DState::LoadResShpPrimitive(shp, pViewPos, pViewNrm);
        }

    // -------------------------------------------------------------------
    // NORMAL (NON-FUR) PATH
    // -------------------------------------------------------------------
    } else {

        if (!(ctrl & DRAW1MAT1SHP_CTRL_NOSWAPSHP)) {
            G3DState::LoadResShpPrePrimitive(shp);

            if (pSwap != NULL) {
                if (pSwap->vtxPosTable != NULL) {
                    ResVtxPos(pSwap->vtxPosTable[shp.ref().idVtxPosition])
                        .SetArray();
                }

                if (pSwap->vtxNrmTable != NULL) {
                    int nrmID = shp.ref().idVtxNormal;

                    if (nrmID >= 0) {
                        ResVtxNrm(pSwap->vtxNrmTable[nrmID]).SetArray();
                    }
                }

                if (pSwap->vtxClrTable != NULL) {
                    int clrID = shp.ref().idVtxColor[0];

                    if (clrID >= 0) {
                        ResVtxClr(pSwap->vtxClrTable[clrID])
                            .SetArray(GX_VA_CLR0);
                    }

                    clrID = shp.ref().idVtxColor[1];

                    if (clrID >= 0) {
                        ResVtxClr(pSwap->vtxClrTable[clrID])
                            .SetArray(GX_VA_CLR1);
                    }
                }
            }
        }

        if (ctrl & DRAW1MAT1SHP_CTRL_CULL_FRONT) {
            fifo::GDSetCullMode(GX_CULL_FRONT);
            G3DState::Invalidate(G3DState::INVALIDATE_SHP);
        }

        G3DState::LoadResShpPrimitive(shp, pViewPos, pViewNrm);
    }
}

} // namespace g3d
} // namespace nw4r
