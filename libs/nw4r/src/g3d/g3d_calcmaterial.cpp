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
            // Clear only the tex/pltt index arrays; the existence flags are
            // written unconditionally by GetResult.
            TexPatAnmResult patResult;
            for (int j = 0; j < TexPatAnmResult::NUM_OF_ANMS; j++) {
                patResult.tex[j] = ResTex(NULL);
            }
            for (int j = 0; j < TexPatAnmResult::NUM_OF_ANMS; j++) {
                patResult.pltt[j] = ResPltt(NULL);
            }

            const TexPatAnmResult* pResult =
                pAnmTexPat->GetResult(&patResult, i);
            ApplyTexPatAnmResult(mat.GetResTexObj(), mat.GetResTlutObj(),
                                 pResult);
        }

        // Texture SRT animation.
        if (pAnmTexSrt != NULL && pAnmTexSrt->TestExistence(i)) {
            ResMatIndMtxAndScale indMtx = mat.GetResMatIndMtxAndScale();
            ResTexSrt texSrt = mat.GetResTexSrt();

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
