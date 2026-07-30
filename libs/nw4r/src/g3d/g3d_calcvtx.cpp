#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void CalcVtx(ResMdl mdl, AnmObjShp* pAnmShp, ResVtxPosData** ppVtxPosTable,
             ResVtxNrmData** ppVtxNrmTable, ResVtxClrData** ppVtxClrTable) {
    u32 numEntries = mdl.GetResVtxPosNumEntries();

    for (u32 idx = 0; idx < numEntries; idx++) {
        if (!pAnmShp->TestExistence(idx))
            continue;

        ShpAnmResult result = {};
        const ShpAnmResult* pR = pAnmShp->GetResult(&result, idx);
        u32 flags = pR->flags;

        if (!(flags & ShpAnmResult::FLAG_ANM_EXISTS))
            continue;

        // =================== POSITIONS ===================
        if (flags & ShpAnmResult::FLAG_ANM_VTXPOS) {
            ResVtxPosData* pVtxPosData = *ppVtxPosTable;
            if (pVtxPosData->toVtxPosArray != 0) {
                u8* pPosBase = (u8*)pVtxPosData + pVtxPosData->toVtxPosArray;
                const void* pBaseData = NULL;
                u8 baseStride = 0;
                if (pR->baseShapeWeight != 0.0f)
                    pR->baseShapeVtxSet.resVtxPos.GetArray(&pBaseData, &baseStride);

                u32 numPos = pR->baseShapeVtxSet.resVtxPos.ref().numPos;
                for (u32 v = 0; v < numPos; v++) {
                    f32 x = 0.0f, y = 0.0f, z = 0.0f;
                    if (pBaseData != NULL) {
                        const f32* pSrc = (const f32*)pBaseData + v * 3;
                        f32 w = pR->baseShapeWeight;
                        x = pSrc[0] * w;
                        y = pSrc[1] * w;
                        z = pSrc[2] * w;
                    }
                    for (u32 ks = 0; ks < pR->numKeyShape; ks++) {
                        f32 weight = pR->keyShape[ks].weight;
                        if (weight != 0.0f) {
                            ResVtxPos kVtx = pR->keyShape[ks].vtxSet.resVtxPos;
                            if (kVtx.IsValid() && kVtx.ref().toVtxPosArray != 0) {
                                const f32* pSrc = (const f32*)((const u8*)kVtx.ptr() + kVtx.ref().toVtxPosArray) + v * 3;
                                x += pSrc[0] * weight;
                                y += pSrc[1] * weight;
                                z += pSrc[2] * weight;
                            }
                        }
                    }
                    f32* pDst = (f32*)(pPosBase + v * 12);
                    pDst[0] = x;
                    pDst[1] = y;
                    pDst[2] = z;
                }
                DCStoreRange(pPosBase, numPos * 12);
            }
        }

        // =================== NORMALS ===================
        if ((flags & ShpAnmResult::FLAG_ANM_VTXNRM) && ppVtxNrmTable != NULL) {
            const ResVtxNrmData* pBaseNrm = pR->baseShapeVtxSet.resVtxNrm.ptr();
            if (pBaseNrm != NULL) {
                ResVtxNrmData* pVtxNrmData = ppVtxNrmTable[pBaseNrm->id];
                if (pVtxNrmData->toVtxNrmArray != 0) {
                    u8* pNrmBase = (u8*)pVtxNrmData + pVtxNrmData->toVtxNrmArray;
                    const void* pBaseData = NULL;
                    u8 baseStride = 0;
                    if (pR->baseShapeWeight != 0.0f)
                        pR->baseShapeVtxSet.resVtxNrm.GetArray(&pBaseData, &baseStride);

                    u32 numNrm = pR->baseShapeVtxSet.resVtxNrm.ref().numNrm;
                    for (u32 v = 0; v < numNrm; v++) {
                        f32 x = 0.0f, y = 0.0f, z = 0.0f;
                        if (pBaseData != NULL) {
                            const f32* pSrc = (const f32*)pBaseData + v * 3;
                            f32 w = pR->baseShapeWeight;
                            x = pSrc[0] * w;
                            y = pSrc[1] * w;
                            z = pSrc[2] * w;
                        }
                        for (u32 ks = 0; ks < pR->numKeyShape; ks++) {
                            f32 weight = pR->keyShape[ks].weight;
                            if (weight != 0.0f) {
                                ResVtxNrm kVtx = pR->keyShape[ks].vtxSet.resVtxNrm;
                                if (kVtx.IsValid() && kVtx.ref().toVtxNrmArray != 0) {
                                    const f32* pSrc = (const f32*)((const u8*)kVtx.ptr() + kVtx.ref().toVtxNrmArray) + v * 3;
                                    x += pSrc[0] * weight;
                                    y += pSrc[1] * weight;
                                    z += pSrc[2] * weight;
                                }
                            }
                        }
                        f32* pDst = (f32*)(pNrmBase + v * 12);
                        pDst[0] = x;
                        pDst[1] = y;
                        pDst[2] = z;
                    }
                    DCStoreRange(pNrmBase, numNrm * 12);
                }
            }
        }

        // =================== COLORS ===================
        if ((flags & ShpAnmResult::FLAG_ANM_VTXCLR) && ppVtxClrTable != NULL) {
            const ResVtxClrData* pBaseClr = pR->baseShapeVtxSet.resVtxClr.ptr();
            if (pBaseClr != NULL) {
                ResVtxClrData* pVtxClrData = ppVtxClrTable[pBaseClr->id];
                if (pVtxClrData->toVtxClrArray != 0) {
                    u8* pClrBase = (u8*)pVtxClrData + pVtxClrData->toVtxClrArray;
                    const void* pBaseData = NULL;
                    u8 baseStride = 0;
                    if (pR->baseShapeWeight != 0.0f)
                        pR->baseShapeVtxSet.resVtxClr.GetArray(&pBaseData, &baseStride);

                    u32 numClr = pR->baseShapeVtxSet.resVtxClr.ref().numClr;
                    for (u32 v = 0; v < numClr; v++) {
                        f32 r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
                        if (pBaseData != NULL) {
                            const u8* pSrc = (const u8*)pBaseData + v * 4;
                            f32 w = pR->baseShapeWeight;
                            r = pSrc[0] * w;
                            g = pSrc[1] * w;
                            b = pSrc[2] * w;
                            a = pSrc[3] * w;
                        }
                        for (u32 ks = 0; ks < pR->numKeyShape; ks++) {
                            f32 weight = pR->keyShape[ks].weight;
                            if (weight != 0.0f) {
                                ResVtxClr kVtx = pR->keyShape[ks].vtxSet.resVtxClr;
                                if (kVtx.IsValid() && kVtx.ref().toVtxClrArray != 0) {
                                    const u8* pSrc = (const u8*)kVtx.ptr() + kVtx.ref().toVtxClrArray + v * 4;
                                    r += pSrc[0] * weight;
                                    g += pSrc[1] * weight;
                                    b += pSrc[2] * weight;
                                    a += pSrc[3] * weight;
                                }
                            }
                        }
                        pClrBase[v * 4 + 0] = (u8)r;
                        pClrBase[v * 4 + 1] = (u8)g;
                        pClrBase[v * 4 + 2] = (u8)b;
                        pClrBase[v * 4 + 3] = (u8)a;
                    }
                    DCStoreRange(pClrBase, numClr * 4);
                }
            }
        }

        ppVtxPosTable++;
    }
}

} // namespace g3d
} // namespace nw4r