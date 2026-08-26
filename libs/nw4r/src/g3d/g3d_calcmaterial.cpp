#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void CalcMaterialDirectly(ResMdl mdl, AnmObjTexPat* pAnmTexPat,
                          AnmObjTexSrt* pAnmTexSrt, AnmObjMatClr* pAnmMatClr) {
    u32 numMat = mdl.GetResMatNumEntries();

    for (u32 i = 0; i < numMat; i++) {
        ResMat mat = mdl.GetResMat(i);

        // Texture pattern animation.
        if (pAnmTexPat != NULL && pAnmTexPat->TestExistence(i)) {
            TexPatAnmResult patResult;

            const TexPatAnmResult* pResult =
                pAnmTexPat->GetResult(&patResult, i);
            ApplyTexPatAnmResult(mat.GetResTexObj(), mat.GetResTlutObj(),
                                 pResult);
        }

        // Texture SRT animation.
        // NOTE: texSrt is declared before indMtx to match retail instruction
        // scheduling (the indMtx address computation must be hoisted into the
        // GetResTexSrt load slot).
        if (pAnmTexSrt != NULL && pAnmTexSrt->TestExistence(i)) {
            ResTexSrt texSrt = mat.GetResTexSrt();
            ResMatIndMtxAndScale indMtx = mat.GetResMatIndMtxAndScale();

            TexSrtAnmResult srtResult;
            const TexSrtAnmResult* pResult =
                pAnmTexSrt->GetResult(&srtResult, i);

            ApplyTexSrtAnmResult(texSrt, indMtx, pResult);
            indMtx.DCStore(false);
        }

        // Material color animation.
        if (pAnmMatClr != NULL && pAnmMatClr->TestExistence(i)) {
            ResMatTevColor tevColor = mat.GetResMatTevColor();
            ResMatChan chan = mat.GetResMatChan();

            ClrAnmResult clrResult;
            const ClrAnmResult* pResult =
                pAnmMatClr->GetResult(&clrResult, i);

            ApplyClrAnmResult(chan, tevColor, pResult);
            tevColor.DCStore(false);
        }
    }
}

} // namespace g3d
} // namespace nw4r
