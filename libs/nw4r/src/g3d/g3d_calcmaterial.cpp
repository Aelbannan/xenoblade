#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void CalcMaterialDirectly(ResMdl mdl, AnmObjTexPat* pAnmTexPat,
                          AnmObjTexSrt* pAnmTexSrt, AnmObjMatClr* pAnmMatClr) {
    u32 numMat = mdl.GetResMatNumEntries();

    for (u32 i = 0; i < numMat; i++) {
        ResMat mat = mdl.GetResMat(i);

        if (pAnmTexPat != NULL) {
            if (pAnmTexPat->TestExistence(i)) {
                TexPatAnmResult patResult = {};
                pAnmTexPat->GetResult(&patResult, i);
                ApplyTexPatAnmResult(mat.GetResTexObj(), mat.GetResTlutObj(),
                                     &patResult);
            }
        }

        if (pAnmTexSrt != NULL) {
            if (pAnmTexSrt->TestExistence(i)) {
                TexSrtAnmResult srtResult = {};
                pAnmTexSrt->GetResult(&srtResult, i);
                ApplyTexSrtAnmResult(mat.GetResTexSrt(),
                                     mat.GetResMatIndMtxAndScale(), &srtResult);
                mat.GetResMatIndMtxAndScale().DCStore(false);
            }
        }

        if (pAnmMatClr != NULL) {
            if (pAnmMatClr->TestExistence(i)) {
                ClrAnmResult clrResult = {};
                pAnmMatClr->GetResult(&clrResult, i);
                ApplyClrAnmResult(mat.GetResMatChan(), mat.GetResMatTevColor(),
                                  &clrResult);
                mat.GetResMatTevColor().DCStore(false);
            }
        }
    }
}

} // namespace g3d
} // namespace nw4r
