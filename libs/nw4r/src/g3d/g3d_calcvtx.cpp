#include <nw4r/g3d.h>

// Struct member offset constants for ASM blocks
#define VEC3_X(p) 0(p)
#define VEC3_Y(p) 4(p)
#define VEC3_Z(p) 8(p)
#define COLOR_R(p) 0(p)
#define COLOR_B(p) 2(p)

namespace nw4r {
namespace g3d {

void CalcVtx(ResMdl mdl, AnmObjShp* pAnmShp, ResVtxPosData** ppVtxPosTable,
             ResVtxNrmData** ppVtxNrmTable, ResVtxClrData** ppVtxClrTable) {
    using nw4r::math::VEC3;

    u32 numEntries = mdl.GetResVtxPosNumEntries();

    for (u32 idx = 0; idx < numEntries; idx++) {
        if (!pAnmShp->TestExistence(idx)) {
            continue;
        }

        ShpAnmResult result;
        const ShpAnmResult* pResult = pAnmShp->GetResult(&result, idx);

        if (!(pResult->flags & ShpAnmResult::FLAG_ANM_EXISTS)) {
            continue;
        }

        // =================== POSITIONS ===================
        if (pResult->flags & ShpAnmResult::FLAG_ANM_VTXPOS) {
            struct KeyShape {
                const void* pVtx;
                f32 weight;
            };

            ResVtxPos basePos = pResult->baseShapeVtxSet.resVtxPos;
            KeyShape keyShape[ShpAnmResult::MAX_KEY_SHAPE + 1];
            int numKeyShape = 0;

            ResVtxPosData* pVtxPosData = ppVtxPosTable[idx];
            s32 toPosOfs = pVtxPosData->toVtxPosArray;
            f32* pVtxPosBuf;

            if (toPosOfs == 0) {
                pVtxPosBuf = NULL;
            } else {
                pVtxPosBuf = reinterpret_cast<f32*>(
                    reinterpret_cast<u8*>(pVtxPosData) + toPosOfs);
            }

            if (pResult->baseShapeWeight != 0.0f) {
                const void* pBase;
                u8 stride;
                basePos.GetArray(&pBase, &stride);
                keyShape[numKeyShape].pVtx = pBase;
                keyShape[numKeyShape].weight = pResult->baseShapeWeight;
                numKeyShape++;
            }

            for (int i = 0; i < static_cast<int>(pResult->numKeyShape); i++) {
                if (pResult->keyShape[i].weight == 0.0f) {
                    continue;
                }

                ResVtxPos key = pResult->keyShape[i].vtxSet.resVtxPos;
                keyShape[numKeyShape].pVtx = key.GetData();
                keyShape[numKeyShape].weight = pResult->keyShape[i].weight;
                numKeyShape++;
            }

            u16 numVtx = basePos.GetNumVtxPos();
            f32* const pVtxPosBufEnd =
                reinterpret_cast<f32*>(reinterpret_cast<u8*>(pVtxPosBuf) +
                                       numVtx * sizeof(VEC3));
            const KeyShape* const pKeyEnd = keyShape + numKeyShape;

            for (; pVtxPosBuf < pVtxPosBufEnd;
                 pVtxPosBuf = reinterpret_cast<f32*>(
                     reinterpret_cast<u8*>(pVtxPosBuf) + sizeof(VEC3))) {
                register f32 xy, z_;

                KeyShape& rFirst = keyShape[0];
                register f32 firstWeight = rFirst.weight;
                register const void* pVtx = rFirst.pVtx;

                ASM(
                    psq_l xy, VEC3_X(pVtx), 1, 0
                    psq_l z_, VEC3_Z(pVtx), 1, 0
                    ps_mul xy, xy, firstWeight
                    ps_mul z_, z_, firstWeight
                )

                rFirst.pVtx = reinterpret_cast<const void*>(
                    reinterpret_cast<const u8*>(rFirst.pVtx) + sizeof(VEC3));

                for (KeyShape* pKey = &keyShape[1]; pKey < pKeyEnd; pKey++) {
                    register f32 keyWeight = pKey->weight;
                    register const void* pKeyVtx = pKey->pVtx;
                    register f32 key_xy, key_z_;

                    ASM(
                        psq_l key_xy, VEC3_X(pKeyVtx), 1, 0
                        psq_l key_z_, VEC3_Z(pKeyVtx), 1, 0
                        ps_madd xy, key_xy, keyWeight, xy
                        ps_madd z_, key_z_, keyWeight, z_
                    )

                    pKey->pVtx = reinterpret_cast<const void*>(
                        reinterpret_cast<const u8*>(pKey->pVtx) +
                        sizeof(VEC3));
                }

                register void* pDst = pVtxPosBuf;

                ASM(
                    psq_st xy, VEC3_X(pDst), 1, 0
                    psq_st z_, VEC3_Z(pDst), 1, 0
                )
            }

            if (toPosOfs != 0) {
                DCStoreRange(reinterpret_cast<f32*>(
                    reinterpret_cast<u8*>(pVtxPosData) + toPosOfs),
                    numVtx * sizeof(VEC3));
            }
        }

        // =================== NORMALS ===================
        if ((pResult->flags & ShpAnmResult::FLAG_ANM_VTXNRM) &&
            ppVtxNrmTable != NULL) {
            struct KeyShape {
                const void* pVtx;
                f32 weight;
            };

            ResVtxNrm baseNrm = pResult->baseShapeVtxSet.resVtxNrm;

            ResVtxNrmData* pBaseNrmData = baseNrm.ptr();
            if (pBaseNrmData == NULL) {
                goto check_clr;
            }

            ResVtxNrmData* pVtxNrmData = ppVtxNrmTable[pBaseNrmData->id];
            KeyShape keyShape[ShpAnmResult::MAX_KEY_SHAPE + 1];
            int numKeyShape = 0;

            s32 toNrmOfs = pVtxNrmData->toVtxNrmArray;
            f32* pVtxNrmBuf;

            if (toNrmOfs == 0) {
                pVtxNrmBuf = NULL;
            } else {
                pVtxNrmBuf = reinterpret_cast<f32*>(
                    reinterpret_cast<u8*>(pVtxNrmData) + toNrmOfs);
            }

            if (pResult->baseShapeWeight != 0.0f) {
                const void* pBase;
                u8 stride;
                baseNrm.GetArray(&pBase, &stride);
                keyShape[numKeyShape].pVtx = pBase;
                keyShape[numKeyShape].weight = pResult->baseShapeWeight;
                numKeyShape++;
            }

            for (int i = 0; i < static_cast<int>(pResult->numKeyShape); i++) {
                if (pResult->keyShape[i].weight == 0.0f) {
                    continue;
                }

                ResVtxNrm key = pResult->keyShape[i].vtxSet.resVtxNrm;
                keyShape[numKeyShape].pVtx = key.GetData();
                keyShape[numKeyShape].weight = pResult->keyShape[i].weight;
                numKeyShape++;
            }

            u16 numNrm = baseNrm.GetNumVtxNrm();
            f32* const pVtxNrmBufEnd =
                reinterpret_cast<f32*>(reinterpret_cast<u8*>(pVtxNrmBuf) +
                                       numNrm * sizeof(VEC3));
            const KeyShape* const pKeyEnd = keyShape + numKeyShape;

            for (; pVtxNrmBuf < pVtxNrmBufEnd;
                 pVtxNrmBuf = reinterpret_cast<f32*>(
                     reinterpret_cast<u8*>(pVtxNrmBuf) + sizeof(VEC3))) {
                register f32 xy, z_;

                KeyShape& rFirst = keyShape[0];
                register f32 firstWeight = rFirst.weight;
                register const void* pVtx = rFirst.pVtx;

                ASM(
                    psq_l xy, VEC3_X(pVtx), 1, 0
                    psq_l z_, VEC3_Z(pVtx), 1, 0
                    ps_mul xy, xy, firstWeight
                    ps_mul z_, z_, firstWeight
                )

                rFirst.pVtx = reinterpret_cast<const void*>(
                    reinterpret_cast<const u8*>(rFirst.pVtx) + sizeof(VEC3));

                for (KeyShape* pKey = &keyShape[1]; pKey < pKeyEnd; pKey++) {
                    register f32 keyWeight = pKey->weight;
                    register const void* pKeyVtx = pKey->pVtx;
                    register f32 key_xy, key_z_;

                    ASM(
                        psq_l key_xy, VEC3_X(pKeyVtx), 1, 0
                        psq_l key_z_, VEC3_Z(pKeyVtx), 1, 0
                        ps_madd xy, key_xy, keyWeight, xy
                        ps_madd z_, key_z_, keyWeight, z_
                    )

                    pKey->pVtx = reinterpret_cast<const void*>(
                        reinterpret_cast<const u8*>(pKey->pVtx) +
                        sizeof(VEC3));
                }

                register void* pDst = pVtxNrmBuf;

                ASM(
                    psq_st xy, VEC3_X(pDst), 1, 0
                    psq_st z_, VEC3_Z(pDst), 1, 0
                )
            }

            if (toNrmOfs != 0) {
                DCStoreRange(reinterpret_cast<f32*>(
                    reinterpret_cast<u8*>(pVtxNrmData) + toNrmOfs),
                    numNrm * sizeof(VEC3));
            }
        }

    check_clr:
        // =================== COLORS ===================
        if ((pResult->flags & ShpAnmResult::FLAG_ANM_VTXCLR) &&
            ppVtxClrTable != NULL) {
            struct KeyShape {
                const void* pVtx;
                f32 weight;
            };

            ResVtxClr baseClr = pResult->baseShapeVtxSet.resVtxClr;

            ResVtxClrData* pBaseClrData = baseClr.ptr();
            if (pBaseClrData == NULL) {
                goto next_entry;
            }

            ResVtxClrData* pVtxClrData = ppVtxClrTable[pBaseClrData->id];
            KeyShape keyShape[ShpAnmResult::MAX_KEY_SHAPE + 1];
            int numKeyShape = 0;

            s32 toClrOfs = pVtxClrData->toVtxClrArray;
            u8* pVtxClrBuf;

            if (toClrOfs == 0) {
                pVtxClrBuf = NULL;
            } else {
                pVtxClrBuf = reinterpret_cast<u8*>(pVtxClrData) + toClrOfs;
            }

            if (pResult->baseShapeWeight != 0.0f) {
                const void* pBase;
                u8 stride;
                baseClr.GetArray(&pBase, &stride);
                keyShape[numKeyShape].pVtx = pBase;
                keyShape[numKeyShape].weight = pResult->baseShapeWeight;
                numKeyShape++;
            }

            for (int i = 0; i < static_cast<int>(pResult->numKeyShape); i++) {
                if (pResult->keyShape[i].weight == 0.0f) {
                    continue;
                }

                ResVtxClr key = pResult->keyShape[i].vtxSet.resVtxClr;
                keyShape[numKeyShape].pVtx = key.GetData();
                keyShape[numKeyShape].weight = pResult->keyShape[i].weight;
                numKeyShape++;
            }

            u16 numClr = baseClr.GetNumVtxClr();
            u8* const pVtxClrBufEnd = pVtxClrBuf + numClr * 4;
            const KeyShape* const pKeyEnd = keyShape + numKeyShape;

            for (; pVtxClrBuf < pVtxClrBufEnd; pVtxClrBuf += 4) {
                register f32 rg, ba;

                KeyShape& rFirst = keyShape[0];
                register f32 firstWeight = rFirst.weight;
                register const void* pVtx = rFirst.pVtx;

                ASM(
                    psq_l rg, COLOR_R(pVtx), 1, 2
                    psq_l ba, COLOR_B(pVtx), 1, 2
                    ps_mul rg, rg, firstWeight
                    ps_mul ba, ba, firstWeight
                )

                rFirst.pVtx = reinterpret_cast<const void*>(
                    reinterpret_cast<const u8*>(rFirst.pVtx) + 4);

                for (KeyShape* pKey = &keyShape[1]; pKey < pKeyEnd; pKey++) {
                    register f32 keyWeight = pKey->weight;
                    register const void* pKeyVtx = pKey->pVtx;
                    register f32 key_rg, key_ba;

                    ASM(
                        psq_l key_rg, COLOR_R(pKeyVtx), 1, 2
                        psq_l key_ba, COLOR_B(pKeyVtx), 1, 2
                        ps_madd rg, key_rg, keyWeight, rg
                        ps_madd ba, key_ba, keyWeight, ba
                    )

                    pKey->pVtx = reinterpret_cast<const void*>(
                        reinterpret_cast<const u8*>(pKey->pVtx) + 4);
                }

                register void* pDst = pVtxClrBuf;

                ASM(
                    psq_st rg, COLOR_R(pDst), 1, 2
                    psq_st ba, COLOR_B(pDst), 1, 2
                )
            }

            if (toClrOfs != 0) {
                DCStoreRange(reinterpret_cast<u8*>(pVtxClrData) + toClrOfs,
                             numClr * 4);
            }
        }

    next_entry:;
    }
}

} // namespace g3d
} // namespace nw4r
