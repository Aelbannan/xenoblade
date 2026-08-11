#include <nw4r/g3d.h>
#include <nw4r/math.h>
#include <nw4r/ut.h>

#include <revolution/BASE.h>

#include <algorithm>

// MWCC's <algorithm> doesn't declare std::sort. The retail nw4r build
// instantiated the MSL implementation in this TU (sort<...> and the
// __sort132 median-of-3 helper), so define the templates here to get the
// same instantiated symbols (sort<PQ54...MdlZ, ...> and __sort132<...>).
namespace std {

// Median-of-3 over the pivot candidates *_F, *_P, *_L (retail __sort132
// body: the pivot slot *_P ends up holding the median).
template <typename _Pr, typename _RI>
void __sort132(_RI _F, _RI _L, _RI _P, _Pr _C) {
    bool cPF = _C(*_P, *_F);
    bool cLP = _C(*_L, *_P);

    if (cPF && cLP) {
        swap(*_F, *_L);
    } else if (cPF || cLP) {
        if (_C(*_L, *_F)) {
            swap(*_F, *_L);
        }
        if (cPF) {
            swap(*_F, *_P);
        } else {
            swap(*_L, *_P);
        }
    }
}

template <typename _RI, typename _Pr>
void sort(_RI _F, _RI _L, _Pr _P) {
    static int shuffle = 0;

    for (;;) {
        int count = _L - _F;
        if (count <= 1) {
            return;
        }

        if (count <= 20) {
            // Selection sort for small ranges.
            for (_RI i = _F; i != _L - 1; ++i) {
                _RI min = i;
                for (_RI j = i + 1; j != _L; ++j) {
                    if (_P(*j, *min)) {
                        min = j;
                    }
                }
                if (min != i) {
                    swap(*min, *i);
                }
            }
            return;
        }

        // Quicksort for larger ranges; the pivot is picked from two
        // pseudo-random positions near the 1/4 and 3/4 marks (a static
        // rotating counter avoids repeated sorts of similar data hitting
        // the same pivot).
        {
            int c = shuffle;
            int c1 = c + 1;
            _RI p1 = _F + (count / 4 + c % 5);
            if (c1 >= 5) {
                c1 = -4;
            }
            shuffle = c1 + 1;
            _RI p2 = _F + (count * 3 / 4 + c1 % 5);
            if (shuffle >= 5) {
                shuffle = -4;
            }

            // The reference-comparator instantiation is the one retail emits.
            __sort132<_Pr&, _RI>(p1, p2, _L - 1, _P);

            // Partition [_F, _L) around the pivot kept at _L - 1.
            _RI pivot = _L - 1;
            _RI i = _F;
            _RI j = _L - 1;
            while (_P(*i, *pivot)) {
                ++i;
            }
            do {
                --j;
                if (i == j) {
                    break;
                }
            } while (!_P(*j, *pivot));
            while (i < j) {
                swap(*i, *j);
                ++i;
                while (_P(*i, *pivot)) {
                    ++i;
                }
                do {
                    --j;
                } while (!_P(*j, *pivot));
            }

            if (i == _F) {
                // The pivot is the smallest element: move it to the front and
                // partition the remainder with the comparisons reversed.
                swap(*i, *pivot);
                ++i;
                j = _L - 1;
                if (!_P(*_F, *(_L - 1))) {
                    while (i != _L && !_P(*_F, *i)) {
                        ++i;
                    }
                    if (i < j) {
                        swap(*i, *j);
                    }
                }
                while (i < j) {
                    while (!_P(*_F, *i)) {
                        ++i;
                    }
                    do {
                        --j;
                    } while (_P(*_F, *j));
                    if (i >= j) {
                        break;
                    }
                    swap(*i, *j);
                    ++i;
                }
                _F = i;
            } else {
                // Recurse on the smaller side, loop on the larger side.
                // The recursion always uses the reference comparator form.
                int left = i - _F;
                int right = _L - i;
                if (left < right) {
                    sort<_RI, _Pr&>(_F, i, _P);
                    _F = i;
                } else {
                    sort<_RI, _Pr&>(i, _L, _P);
                    _L = i;
                }
            }
        }
    }
}

} // namespace std

// Explicitly instantiate the median-of-3 helper so it is emitted as its own
// symbol (retail __sort132<...>); MWCC otherwise auto-inlines it into sort.
namespace std {
template void __sort132<
    bool (*&)(const nw4r::g3d::detail::workmem::MdlZ&,
              const nw4r::g3d::detail::workmem::MdlZ&),
    nw4r::g3d::detail::workmem::MdlZ*>(
    nw4r::g3d::detail::workmem::MdlZ*, nw4r::g3d::detail::workmem::MdlZ*,
    nw4r::g3d::detail::workmem::MdlZ*,
    bool (*&)(const nw4r::g3d::detail::workmem::MdlZ&,
              const nw4r::g3d::detail::workmem::MdlZ&));
} // namespace std

// Float constants shared by the loop body. Retail loads these once before the
// loop and keeps them in f29-f31 (lbl_eu_80669C48 = 0.0f, 80669C4C = -2.0f,
// 80669C50 = 1.0f); referencing the retail names keeps the SDA21 relocs right.
extern f32 lbl_eu_80669C48;
extern f32 lbl_eu_80669C4C;
extern f32 lbl_eu_80669C50;

namespace nw4r {
namespace g3d {

namespace detail {

G3DState::IndMtxOp* GetIndMtxOp(ResMat mat, ResNode node, ResShp shp) {
    if (!mat.IsValid() || !shp.IsValid()) {
        return NULL;
    }

    ResMatMisc misc = mat.GetResMatMisc();

    ResMatMiscData::IndirectMethod method[GX_ITM_2 - GX_ITM_0 + 1];
    s8 lightRef[GX_ITM_2 - GX_ITM_0 + 1];

    misc.GetIndirectTexMtxCalcMethod(GX_ITM_0, &method[GX_ITM_0 - 1],
                                     &lightRef[GX_ITM_0 - 1]);
    misc.GetIndirectTexMtxCalcMethod(GX_ITM_1, &method[GX_ITM_1 - 1],
                                     &lightRef[GX_ITM_1 - 1]);
    misc.GetIndirectTexMtxCalcMethod(GX_ITM_2, &method[GX_ITM_2 - 1],
                                     &lightRef[GX_ITM_2 - 1]);

    if (method[GX_ITM_0 - 1] == ResMatMiscData::WARP &&
        method[GX_ITM_1 - 1] == ResMatMiscData::WARP &&
        method[GX_ITM_2 - 1] == ResMatMiscData::WARP) {
        return NULL;
    }

    bool nrmMtxInit = false;
    math::MTX34 nrmMtx;

    u32 i;

    G3DState::IndMtxOp& rOp = *G3DState::GetIndMtxOp();
    rOp.Reset();

    // Load the shared constants once (retail keeps them in f29-f31 across the
    // whole loop: f29 = -2.0f, f30 = 1.0f, f31 = 0.0f).
    const f32 cNegTwo = lbl_eu_80669C4C;
    const f32 cOne = lbl_eu_80669C50;
    const f32 cZero = lbl_eu_80669C48;

    for (i = 0; i < GX_ITM_2 - GX_ITM_0 + 1; i++) {
        GXIndTexMtxID id = static_cast<GXIndTexMtxID>(i + 1);

        if (method[i] == ResMatMiscData::WARP) {
            continue;
        }

        if (!nrmMtxInit) {
            nrmMtxInit = true;

            if (shp.ptr()->curMtxIdx >= 0) {
                if (shp.ptr()->curMtxIdx == node.GetMtxID()) {
                    const math::MTX33* pViewNrm =
                        G3DState::GetViewNrmMtxPtr(shp.ptr()->curMtxIdx);

                    nrmMtx._00 = pViewNrm->_00;
                    nrmMtx._01 = pViewNrm->_01;
                    nrmMtx._02 = pViewNrm->_02;
                    nrmMtx._10 = pViewNrm->_10;
                    nrmMtx._11 = pViewNrm->_11;
                    nrmMtx._12 = pViewNrm->_12;
                    nrmMtx._20 = pViewNrm->_20;
                    nrmMtx._21 = pViewNrm->_21;
                    nrmMtx._22 = pViewNrm->_22;
                } else {
                    ResMdl mdl = mat.GetParent();

                    int shpNodeID =
                        mdl.GetResMdlInfo().GetNodeIDFromMtxID(
                            shp.ptr()->curMtxIdx);

                    ResNode shpNode = mdl.GetResNode(shpNodeID);

                    math::MTX34Mult(
                        &nrmMtx,
                        reinterpret_cast<const math::MTX34*>(
                            &shpNode.ref().invModelMtx),
                        reinterpret_cast<const math::MTX34*>(
                            &node.ref().modelMtx));

                    const math::MTX33* pViewNrm =
                        G3DState::GetViewNrmMtxPtr(
                            shp.ptr()->curMtxIdx);

                    math::MTX34 viewNrm34;
                    viewNrm34._00 = pViewNrm->_00;
                    viewNrm34._01 = pViewNrm->_01;
                    viewNrm34._02 = pViewNrm->_02;
                    viewNrm34._03 = cZero;
                    viewNrm34._10 = pViewNrm->_10;
                    viewNrm34._11 = pViewNrm->_11;
                    viewNrm34._12 = pViewNrm->_12;
                    viewNrm34._13 = cZero;
                    viewNrm34._20 = pViewNrm->_20;
                    viewNrm34._21 = pViewNrm->_21;
                    viewNrm34._22 = pViewNrm->_22;
                    viewNrm34._23 = cZero;

                    math::MTX34Mult(&nrmMtx, &viewNrm34, &nrmMtx);
                }

                // The 4th column is zeroed here so the viewNrm copy path and
                // the concat path share the same stores; the camera-matrix
                // path skips the normalization entirely.
                nrmMtx._03 = cZero;
                nrmMtx._13 = cZero;
                nrmMtx._23 = cZero;

                math::VEC3 col0(nrmMtx._00, nrmMtx._10, nrmMtx._20);
                math::VEC3Normalize(&col0, &col0);
                nrmMtx._00 = col0.x;
                nrmMtx._10 = col0.y;
                nrmMtx._20 = col0.z;

                math::VEC3 col1(nrmMtx._01, nrmMtx._11, nrmMtx._21);
                math::VEC3Normalize(&col1, &col1);
                nrmMtx._01 = col1.x;
                nrmMtx._11 = col1.y;
                nrmMtx._21 = col1.z;

                math::VEC3 col2(nrmMtx._02, nrmMtx._12, nrmMtx._22);
                math::VEC3Normalize(&col2, &col2);
                nrmMtx._02 = col2.x;
                nrmMtx._12 = col2.y;
                nrmMtx._22 = col2.z;
            } else {
                math::MTX34Copy(&nrmMtx, G3DState::GetCameraMtxPtr());
            }
        }

        const LightObj* pLight = G3DState::GetLightObj(lightRef[i]);

        if (pLight != NULL && pLight->IsEnable()) {
            math::VEC3 lightVec;

            if (pLight->IsSpotLight()) {
                pLight->GetLightDir(&lightVec);

                if (lightVec.x == cZero &&
                    lightVec.y == cZero &&
                    lightVec.z == cZero) {
                    pLight->GetLightPos(&lightVec);
                    lightVec.x = -lightVec.x;
                    lightVec.y = -lightVec.y;
                    lightVec.z = -lightVec.z;
                    math::VEC3Normalize(&lightVec, &lightVec);
                }
            } else if (pLight->IsDiffuseLight()) {
                pLight->GetLightPos(&lightVec);
                lightVec.x = -lightVec.x;
                lightVec.y = -lightVec.y;
                lightVec.z = -lightVec.z;
                math::VEC3Normalize(&lightVec, &lightVec);
            } else {
                math::VEC3 H;
                pLight->GetLightDir(&H);
                lightVec.x = cNegTwo * H.z * H.x;
                lightVec.y = cNegTwo * H.z * H.y;
                lightVec.z = cNegTwo * H.z * H.z + cOne;
                math::VEC3Normalize(&lightVec, &lightVec);
            }

            rOp.SetNrmMapMtx(id, &lightVec, &nrmMtx, method[i]);
        } else {
            rOp.SetNrmMapMtx(id, NULL, &nrmMtx, method[i]);
        }
    }

    return &rOp;
}

} // namespace detail

namespace {

// Sort comparators for the MdlZ work-memory arrays. Retail places these in
// the anonymous namespace (mangled @unnamed@g3d_draw_cpp@), matching the
// DrawResMdlLoop helpers below.
bool FrontToBack(const detail::workmem::MdlZ& rA,
                 const detail::workmem::MdlZ& rB) {
    if (rA.priority < rB.priority) {
        return true;
    }

    if (rA.priority > rB.priority) {
        return false;
    }

    return rA.Z > rB.Z;
}

bool BackToFront(const detail::workmem::MdlZ& rA,
                 const detail::workmem::MdlZ& rB) {
    if (rA.priority < rB.priority) {
        return true;
    }

    if (rA.priority > rB.priority) {
        return false;
    }

    if (rA.Z < rB.Z) {
        return true;
    }

    if (rA.Z == rB.Z && rA.matID < rB.matID) {
        return true;
    }

    return false;
}

// Retail DrawResMdlReplacement has a leading u32 flag (bit0 = "drop vtx
// tables") before visArray; g3d_draw.h is missing it, so mirror the retail
// layout here to get the correct field offsets.
struct DrawResMdlReplacementLayout {
    u32 flag;                                    // at 0x0
    u8* visArray;                                // at 0x4
    ResTexObjData* texObjDataArray;              // at 0x8
    ResTlutObjData* tlutObjDataArray;            // at 0xC
    ResTexSrtData* texSrtDataArray;              // at 0x10
    ResChanData* chanDataArray;                  // at 0x14
    ResGenModeData* genModeDataArray;            // at 0x18
    ResMatMiscData* matMiscDataArray;            // at 0x1C
    ResPixDL* pixDLArray;                        // at 0x20
    ResTevColorDL* tevColorDLArray;              // at 0x24
    ResIndMtxAndScaleDL* indMtxAndScaleDLArray;  // at 0x28
    ResTexCoordGenDL* texCoordGenDLArray;        // at 0x2C
    ResTevData* tevDataArray;                    // at 0x30
    ResVtxPosData** vtxPosTable;                 // at 0x34
    ResVtxNrmData** vtxNrmTable;                 // at 0x38
    ResVtxClrData** vtxClrTable;                 // at 0x3C
};

void SetupDraw1Mat1ShpSwap(Draw1Mat1ShpSwap* pSwap,
                           DrawResMdlReplacement* pReplacement, u32 id) {

    const DrawResMdlReplacementLayout* pRepl =
        reinterpret_cast<const DrawResMdlReplacementLayout*>(pReplacement);

    if (pRepl->texObjDataArray != NULL) {
        pSwap->texObj = ResTexObj(&pRepl->texObjDataArray[id]);
    } else {
        pSwap->texObj = ResTexObj(NULL);
    }

    if (pRepl->tlutObjDataArray != NULL) {
        pSwap->tlutObj = ResTlutObj(&pRepl->tlutObjDataArray[id]);
    } else {
        pSwap->tlutObj = ResTlutObj(NULL);
    }

    if (pRepl->texSrtDataArray != NULL) {
        pSwap->texSrt = ResTexSrt(&pRepl->texSrtDataArray[id]);
    } else {
        pSwap->texSrt = ResTexSrt(NULL);
    }

    if (pRepl->chanDataArray != NULL) {
        pSwap->chan = ResMatChan(&pRepl->chanDataArray[id]);
    } else {
        pSwap->chan = ResMatChan(NULL);
    }

    if (pRepl->genModeDataArray != NULL) {
        pSwap->genMode = ResGenMode(&pRepl->genModeDataArray[id]);
    } else {
        pSwap->genMode = ResGenMode(NULL);
    }

    if (pRepl->matMiscDataArray != NULL) {
        pSwap->misc = ResMatMisc(&pRepl->matMiscDataArray[id]);
    } else {
        pSwap->misc = ResMatMisc(NULL);
    }

    if (pRepl->pixDLArray != NULL) {
        pSwap->pix = ResMatPix(&pRepl->pixDLArray[id]);
    } else {
        pSwap->pix = ResMatPix(NULL);
    }

    if (pRepl->tevColorDLArray != NULL) {
        pSwap->tevColor = ResMatTevColor(&pRepl->tevColorDLArray[id]);
    } else {
        pSwap->tevColor = ResMatTevColor(NULL);
    }

    if (pRepl->indMtxAndScaleDLArray != NULL) {
        pSwap->indMtxAndScale =
            ResMatIndMtxAndScale(&pRepl->indMtxAndScaleDLArray[id]);
    } else {
        pSwap->indMtxAndScale = ResMatIndMtxAndScale(NULL);
    }

    if (pRepl->texCoordGenDLArray != NULL) {
        pSwap->texCoordGen =
            ResMatTexCoordGen(&pRepl->texCoordGenDLArray[id]);
    } else {
        pSwap->texCoordGen = ResMatTexCoordGen(NULL);
    }

    if (pRepl->tevDataArray != NULL) {
        pSwap->tev = ResTev(&pRepl->tevDataArray[id]);
    } else {
        pSwap->tev = ResTev(NULL);
    }

    if (pRepl->flag & 1) {
        pSwap->vtxPosTable = NULL;
        pSwap->vtxNrmTable = NULL;
        pSwap->vtxClrTable = NULL;
    } else {
        pSwap->vtxPosTable = pRepl->vtxPosTable;
        pSwap->vtxNrmTable = pRepl->vtxNrmTable;
        pSwap->vtxClrTable = pRepl->vtxClrTable;
    }
}

void DrawResMdlLoop(const ResMdl mdl, const u8* pByteCode, u32 drawMode) {

#define pDrawCmd reinterpret_cast<const ResByteCodeData::DrawParams*>(pByteCode)

    u8 c;

    ResMat mat;
    ResShp shp;
    ResNode node;
    ResMat prevMat;

    // ctrl bit 4 = "skip material state" (material already active / ignored)
    u32 ctrl = DRAW1MAT1SHP_CTRL_NOPPCSYNC;

    if (drawMode & RESMDL_DRAWMODE_FORCE_LIGHTOFF) {
        ctrl |= DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF;
    }

    if (drawMode & RESMDL_DRAWMODE_IGNORE_MATERIAL) {
        ctrl |= 0x10;
    }

    for (; (c = *pByteCode) != ResByteCodeData::END;
         pByteCode += sizeof(ResByteCodeData::DrawParams)) {

        node = mdl.GetResNode(pDrawCmd->nodeIdHi << 8 | pDrawCmd->nodeIdLo);
        shp = mdl.GetResShp(pDrawCmd->shpIdHi << 8 | pDrawCmd->shpIdLo);

        if (!node.IsVisible()) {
            continue;
        }

        if (shp.ptr()->flag & ResShpData::FLAG_INVISIBLE) {
            continue;
        }

        mat = mdl.GetResMat(pDrawCmd->matIdHi << 8 | pDrawCmd->matIdLo);

        u32 matReuseCtrl = ctrl | 0x10;
        Draw1Mat1ShpDirectly(mat, shp, NULL, NULL,
                             mat == prevMat ? matReuseCtrl : ctrl, NULL,
                             detail::GetIndMtxOp(mat, node, shp));

        prevMat = mat;
    }

#undef pDrawCmd
}

void DrawResMdlLoop(const ResMdl mdl, const u8* pByteCode,
                    DrawResMdlReplacement* pReplacement, u32 drawMode) {

#define pDrawCmd reinterpret_cast<const ResByteCodeData::DrawParams*>(pByteCode)

    u8 c;

    ResMat mat;
    ResMat prevMat;

    ResShp shp;
    ResNode node;

    const DrawResMdlReplacementLayout* pRepl =
        reinterpret_cast<const DrawResMdlReplacementLayout*>(pReplacement);

    Draw1Mat1ShpSwap swap;
    // Retail zeroes the whole swap struct up front (the header ctor only
    // clears the vtx tables), so clear every field explicitly.
    swap.texObj = ResTexObj(NULL);
    swap.tlutObj = ResTlutObj(NULL);
    swap.genMode = ResGenMode(NULL);
    swap.tev = ResTev(NULL);
    swap.pix = ResMatPix(NULL);
    swap.tevColor = ResMatTevColor(NULL);
    swap.indMtxAndScale = ResMatIndMtxAndScale(NULL);
    swap.chan = ResMatChan(NULL);
    swap.texCoordGen = ResMatTexCoordGen(NULL);
    swap.misc = ResMatMisc(NULL);
    swap.texSrt = ResTexSrt(NULL);
    swap.vtxPosTable = NULL;
    swap.vtxNrmTable = NULL;
    swap.vtxClrTable = NULL;

    // ctrl bit 4 = "skip material state" (material already active / ignored)
    u32 ctrl = DRAW1MAT1SHP_CTRL_NOPPCSYNC;

    if (drawMode & RESMDL_DRAWMODE_FORCE_LIGHTOFF) {
        ctrl |= DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF;
    }

    if (drawMode & RESMDL_DRAWMODE_IGNORE_MATERIAL) {
        ctrl |= 0x10;
    }

    u32 matReuseCtrl = ctrl | 0x10;

    for (; (c = *pByteCode) != ResByteCodeData::END;
         pByteCode += sizeof(ResByteCodeData::DrawParams)) {

        node = mdl.GetResNode(pDrawCmd->nodeIdHi << 8 | pDrawCmd->nodeIdLo);
        shp = mdl.GetResShp(pDrawCmd->shpIdHi << 8 | pDrawCmd->shpIdLo);

        bool visible;
        if (pRepl->visArray != NULL) {
            visible = pRepl->visArray[node.GetID()] != 0;
        } else {
            visible = node.IsVisible();
        }

        if (!visible) {
            continue;
        }

        if (shp.ptr()->flag & ResShpData::FLAG_INVISIBLE) {
            continue;
        }

        mat = mdl.GetResMat(pDrawCmd->matIdHi << 8 | pDrawCmd->matIdLo);

        SetupDraw1Mat1ShpSwap(&swap, pReplacement, mat.GetID());

        Draw1Mat1ShpDirectly(mat, shp, NULL, NULL,
                             mat == prevMat ? matReuseCtrl : ctrl, &swap,
                             detail::GetIndMtxOp(mat, node, shp));

        prevMat = mat;
    }

#undef pDrawCmd
}

void DrawResMdlLoop(const ResMdl mdl, const detail::workmem::MdlZ* pMdlZArray,
                    u32 numMdlZ, u32 drawMode) {
    ResMat mat;
    ResMat prevMat;

    ResShp shp;
    ResNode node;

    u32 matReuseCtrl;
    u32 i;
    // ctrl bit 4 = "skip material state" (material already active / ignored)
    u32 ctrl = DRAW1MAT1SHP_CTRL_NOPPCSYNC;

    if (drawMode & RESMDL_DRAWMODE_FORCE_LIGHTOFF) {
        ctrl |= DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF;
    }

    if (drawMode & RESMDL_DRAWMODE_IGNORE_MATERIAL) {
        ctrl |= 0x10;
    }

    matReuseCtrl = ctrl | 0x10;

    for (i = 0; i < numMdlZ; i++) {

        shp = mdl.GetResShp(pMdlZArray[i].shpID);

        if (shp.ptr()->flag & ResShpData::FLAG_INVISIBLE) {
            continue;
        }

        mat = mdl.GetResMat(pMdlZArray[i].matID);
        node = mdl.GetResNode(pMdlZArray[i].nodeID);

        Draw1Mat1ShpDirectly(mat, shp, NULL, NULL,
                             mat == prevMat ? matReuseCtrl : ctrl, NULL,
                             detail::GetIndMtxOp(mat, node, shp));

        prevMat = mat;
    }
}

void DrawResMdlLoop(const ResMdl mdl, const detail::workmem::MdlZ* pMdlZArray,
                    u32 numMdlZ, DrawResMdlReplacement* pReplacement,
                    u32 drawMode) {
    u32 i;

    ResMat mat;
    ResMat prevMat;

    ResShp shp;
    ResNode node;

    Draw1Mat1ShpSwap swap;

    // ctrl bit 4 = "skip material state" (material already active / ignored)
    u32 ctrl = DRAW1MAT1SHP_CTRL_NOPPCSYNC;

    if (drawMode & RESMDL_DRAWMODE_FORCE_LIGHTOFF) {
        ctrl |= DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF;
    }

    if (drawMode & RESMDL_DRAWMODE_IGNORE_MATERIAL) {
        ctrl |= 0x10;
    }

    u32 matReuseCtrl = ctrl | 0x10;

    for (i = 0; i < numMdlZ; i++) {
        const detail::workmem::MdlZ& rMdlZ = pMdlZArray[i];

        shp = mdl.GetResShp(rMdlZ.shpID);

        if (shp.ptr()->flag & ResShpData::FLAG_INVISIBLE) {
            continue;
        }

        mat = mdl.GetResMat(rMdlZ.matID);
        node = mdl.GetResNode(rMdlZ.nodeID);

        SetupDraw1Mat1ShpSwap(&swap, pReplacement, mat.GetID());

        u32 drawCtrl = ctrl;
        if (mat == prevMat) {
            drawCtrl = matReuseCtrl;
        }

        Draw1Mat1ShpDirectly(mat, shp, NULL, NULL, drawCtrl, &swap,
                             detail::GetIndMtxOp(mat, node, shp));

        prevMat = mat;
    }
}

detail::workmem::MdlZ* SetUpMdlZ(u32* pNumMdlZ, const ResMdl mdl,
                                 const math::MTX34* pViewPosMtxArray,
                                 const u8* pByteCode,
                                 DrawResMdlReplacement* pReplacement) {

#define pDrawCmd reinterpret_cast<const ResByteCodeData::DrawParams*>(pByteCode)

    u16 nodeID;
    u8 c;
    u32 mdlZNum = 0;
    ResNode node;
    u32 mtxID;

    u32 viewMtxNum = mdl.GetResMdlInfo().GetNumViewMtx();
    detail::workmem::MdlZ* pMdlZArray = detail::workmem::GetMdlZTemporary();

    const DrawResMdlReplacementLayout* pRepl =
        reinterpret_cast<const DrawResMdlReplacementLayout*>(pReplacement);

    for (; (c = *pByteCode) != ResByteCodeData::END;
         pByteCode += sizeof(ResByteCodeData::DrawParams)) {

        nodeID = pDrawCmd->nodeIdHi << 8 | pDrawCmd->nodeIdLo;
        node = mdl.GetResNode(nodeID);

        bool visible;
        if (pReplacement == NULL || pRepl->visArray == NULL) {
            visible = node.IsVisible();
        } else {
            visible = pRepl->visArray[node.GetID()] != 0;
        }

        if (!visible) {
            continue;
        }

        detail::workmem::MdlZ& rMdlZ = pMdlZArray[mdlZNum];

        rMdlZ.nodeID = nodeID;
        rMdlZ.matID = pDrawCmd->matIdHi << 8 | pDrawCmd->matIdLo;
        rMdlZ.shpID = pDrawCmd->shpIdHi << 8 | pDrawCmd->shpIdLo;

        mtxID = node.GetMtxID();

        if (mtxID < viewMtxNum) {
            rMdlZ.Z = pViewPosMtxArray[mtxID]._23;
        } else {
            rMdlZ.Z = 1.0f;
        }

        rMdlZ.priority = pDrawCmd->priority;

        mdlZNum++;
    }

    *pNumMdlZ = mdlZNum;
    return pMdlZArray;

#undef pDrawCmd
}

} // namespace

void DrawResMdlDirectly(const ResMdl mdl, const math::MTX34* pViewPosMtxArray,
                        const math::MTX33* pViewNrmMtxArray,
                        const math::MTX34* pViewEnvMtxArray,
                        const u8* pByteCodeOpa, const u8* pByteCodeXlu,
                        DrawResMdlReplacement* pReplacement, u32 drawMode) {

    G3DState::SetViewPosNrmMtxArray(pViewPosMtxArray, pViewNrmMtxArray,
                                    pViewEnvMtxArray);

    if (!(drawMode & RESMDL_DRAWMODE_NOPPCSYNC)) {
        ut::LC::QueueWait(0);
        PPCSync();
    }

    if (pByteCodeOpa != NULL) {
        if (drawMode & RESMDL_DRAWMODE_SORT_OPA_Z) {
            u32 numMdlZ;
            detail::workmem::MdlZ* pMdlZArray = SetUpMdlZ(
                &numMdlZ, mdl, pViewPosMtxArray, pByteCodeOpa, pReplacement);

            std::sort(pMdlZArray, pMdlZArray + numMdlZ, FrontToBack);

            if (pReplacement != NULL) {
                DrawResMdlLoop(mdl, pMdlZArray, numMdlZ, pReplacement,
                               drawMode);
            } else {
                DrawResMdlLoop(mdl, pMdlZArray, numMdlZ, drawMode);
            }
        } else if (pReplacement != NULL) {
            DrawResMdlLoop(mdl, pByteCodeOpa, pReplacement, drawMode);
        } else {
            DrawResMdlLoop(mdl, pByteCodeOpa, drawMode);
        }
    }

    if (pByteCodeXlu != NULL) {
        if (drawMode & RESMDL_DRAWMODE_SORT_XLU_Z) {
            u32 numMdlZ;
            detail::workmem::MdlZ* pMdlZArray = SetUpMdlZ(
                &numMdlZ, mdl, pViewPosMtxArray, pByteCodeXlu, pReplacement);

            std::sort(pMdlZArray, pMdlZArray + numMdlZ, BackToFront);

            if (pReplacement != NULL) {
                DrawResMdlLoop(mdl, pMdlZArray, numMdlZ, pReplacement,
                               drawMode);
            } else {
                DrawResMdlLoop(mdl, pMdlZArray, numMdlZ, drawMode);
            }
        } else if (pReplacement != NULL) {
            DrawResMdlLoop(mdl, pByteCodeXlu, pReplacement, drawMode);
        } else {
            DrawResMdlLoop(mdl, pByteCodeXlu, drawMode);
        }
    }

    G3DState::SetViewPosNrmMtxArray(NULL, NULL, NULL);
}

} // namespace g3d
} // namespace nw4r
