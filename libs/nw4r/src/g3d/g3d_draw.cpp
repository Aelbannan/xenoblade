#include <nw4r/g3d.h>
#include <nw4r/math.h>

#include <algorithm>

namespace nw4r {
namespace g3d {

namespace {

/******************************************************************************
 *
 * FrontToBack
 *
 ******************************************************************************/
bool FrontToBack(const detail::workmem::MdlZ& rA,
                 const detail::workmem::MdlZ& rB) {

    if (rA.priority < rB.priority) {
        return true;
    }

    if (rA.priority > rB.priority) {
        return false;
    }

    return rA.Z < rB.Z;
}

/******************************************************************************
 *
 * BackToFront
 *
 ******************************************************************************/
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

    if (rA.Z == rB.Z) {
        return rA.matID < rB.matID;
    }

    return false;
}

/******************************************************************************
 *
 * SetupDraw1Mat1ShpSwap
 *
 ******************************************************************************/
void SetupDraw1Mat1ShpSwap(Draw1Mat1ShpSwap* pSwap,
                           DrawResMdlReplacement* pReplacement, u32 idx) {

    if (pReplacement->tlutObjDataArray != NULL) {
        pSwap->texObj = pReplacement->tlutObjDataArray[idx * 0x104];
    } else {
        pSwap->texObj = NULL;
    }

    if (pReplacement->texSrtDataArray != NULL) {
        pSwap->tlutObj = pReplacement->texSrtDataArray[idx * 0x64];
    } else {
        pSwap->tlutObj = NULL;
    }

    if (pReplacement->chanDataArray != NULL) {
        pSwap->genMode = pReplacement->chanDataArray[idx * 0x248];
    } else {
        pSwap->genMode = NULL;
    }

    if (pReplacement->genModeDataArray != NULL) {
        pSwap->tev = pReplacement->genModeDataArray[idx * 0x28];
    } else {
        pSwap->tev = NULL;
    }

    if (pReplacement->matMiscDataArray != NULL) {
        pSwap->pix = pReplacement->matMiscDataArray[idx * 8];
    } else {
        pSwap->pix = NULL;
    }

    if (pReplacement->pixDLArray != NULL) {
        pSwap->tevColor = pReplacement->pixDLArray[idx * 0x20];
    } else {
        pSwap->tevColor = NULL;
    }

    if (pReplacement->tevColorDLArray != NULL) {
        pSwap->indMtxAndScale = pReplacement->tevColorDLArray[idx * 0x100];
    } else {
        pSwap->indMtxAndScale = NULL;
    }

    if (pReplacement->indMtxAndScaleDLArray != NULL) {
        pSwap->chan = pReplacement->indMtxAndScaleDLArray[idx * 0x40];
    } else {
        pSwap->chan = NULL;
    }

    if (pReplacement->texCoordGenDLArray != NULL) {
        pSwap->texCoordGen = pReplacement->texCoordGenDLArray[idx * 0x80];
    } else {
        pSwap->texCoordGen = NULL;
    }

    if (pReplacement->tevDataArray != NULL) {
        pSwap->misc = pReplacement->tevDataArray[idx * 0xa0];
    } else {
        pSwap->misc = NULL;
    }

    if (pReplacement->vtxPosTable != NULL) {
        pSwap->texSrt = pReplacement->vtxPosTable[idx * 0x100];
    } else {
        pSwap->texSrt = NULL;
    }

    if (pReplacement->visArray != NULL) {
        pSwap->_pad0[0] = 0;
        pSwap->_pad0[1] = 0;
        pSwap->_pad0[2] = 0;
        return;
    }

    pSwap->vtxNrmTable = pReplacement->vtxNrmTable;
    pSwap->vtxClrTable = pReplacement->vtxClrTable;
    pSwap->vtxPosTable = pReplacement->vtxPosTable;
}

} // namespace
} // namespace g3d
} // namespace nw4r
