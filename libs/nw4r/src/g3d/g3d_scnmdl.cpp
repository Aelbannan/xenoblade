// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

ResTexSrt ScnMdl::CopiedMatAccess::GetResTexSrt(bool markDirty) {
    if (mpScnMdl != NULL && mTexSrt.IsValid()) {
        if (markDirty) {
            u32* dirtyFlags = mpScnMdl->mpMatBufferDirtyFlag;
            dirtyFlags[mMatID] |= 0x4;
        }
        return mTexSrt;
    }
    return ResTexSrt(NULL);
}

ResGenMode ScnMdl::CopiedMatAccess::GetResGenMode(bool markDirty) {
    if (mpScnMdl != NULL && mGenMode.IsValid()) {
        if (markDirty) {
            u32* dirtyFlags = mpScnMdl->mpMatBufferDirtyFlag;
            dirtyFlags[mMatID] |= 0x10;
        }
        return mGenMode;
    }
    return ResGenMode(NULL);
}

ResMatChan ScnMdl::CopiedMatAccess::GetResMatChan(bool markDirty) {
    if (mpScnMdl != NULL && mChan.IsValid()) {
        if (markDirty) {
            u32* dirtyFlags = mpScnMdl->mpMatBufferDirtyFlag;
            dirtyFlags[mMatID] |= 0x8;
        }
        return mChan;
    }
    return ResMatChan(NULL);
}

// The retail "Ex" accessors are shifted one field against the header's
// declaration order: GetResMatIndMtxAndScaleEx reads/writes mPix and
// GetResMatTexCoordGenEx reads/writes mTevColor; only GetResTevEx uses its own
// member. Offsets verified against the retail bodies (0x20 / 0x24 / 0x30).
ResMatPix ScnMdl::CopiedMatAccess::GetResMatIndMtxAndScaleEx() {
    if (mpScnMdl != NULL) {
        if (mPix.IsValid()) {
            return mPix;
        }
        ResMat mat = ResMdl(mpScnMdl->GetResMdl()).GetResMat(mMatID);
        return mat.GetResMatPix();
    }
    return ResMatPix(NULL);
}

ResMatTevColor ScnMdl::CopiedMatAccess::GetResMatTexCoordGenEx() {
    if (mpScnMdl != NULL) {
        if (mTevColor.IsValid()) {
            return mTevColor;
        }
        ResMat mat = ResMdl(mpScnMdl->GetResMdl()).GetResMat(mMatID);
        return mat.GetResMatTevColor();
    }
    return ResMatTevColor(NULL);
}

ResTev ScnMdl::CopiedMatAccess::GetResTevEx() {
    if (mpScnMdl != NULL) {
        if (mTev.IsValid()) {
            return mTev;
        }
        ResMat mat = ResMdl(mpScnMdl->GetResMdl()).GetResMat(mMatID);
        return mat.GetResTev();
    }
    return ResTev(NULL);
}

} // namespace g3d
} // namespace nw4r

// MonolithSoft extension bit in the buffer option: when set, vertex data is
// restored from the model resource when a shape animation object is removed
// (the request is kept in mFlagRestoreVtx and mirrored inverted in
// mReplacement.flag).
static const u32 BUFFER_RESTORE_VTX = (1 << 24);

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(ScnMdl);

bool ScnMdl::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(TYPE_NAME) ? true
         : other == ScnMdlSimple::GetTypeObjStatic() ? true
         : other == ScnLeaf::GetTypeObjStatic() ? true
         : other == ScnObj::GetTypeObjStatic() ? true
         : (other == G3dObj::GetTypeObjStatic());
}

const G3dObj::TypeObj ScnMdl::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}
const char* ScnMdl::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

bool ScnMdl::SetScnObjOption(u32 option, u32 value) {
    switch (option) {
    case OPTID_VISBUFFER_REFRESH_NEEDED:
        if (value) {
            mFlagVisBuffer &= ~VISBUFFER_NOT_REFRESH_NEEDED;
        } else {
            mFlagVisBuffer |= VISBUFFER_NOT_REFRESH_NEEDED;
        }
        break;

    default:
        return ScnMdlSimple::SetScnObjOption(option, value);
    }
    return true;
}

bool ScnMdl::GetScnObjOption(u32 option, u32* pValue) const {
    if (!pValue) {
        return false;
    }

    switch (option) {
    case OPTID_VISBUFFER_REFRESH_NEEDED:
        *pValue = (mFlagVisBuffer & VISBUFFER_NOT_REFRESH_NEEDED) ? 0 : 1;
        break;

    default:
        return ScnMdlSimple::GetScnObjOption(option, pValue);
    }
    return true;
}

AnmObj* ScnMdl::RemoveAnmObj(AnmObjType type) {
    if (type == ANMOBJTYPE_SHP) {
        AnmObj* pAnmObj = mpAnmObjShp;
        RemoveAnmObj(pAnmObj);
        return pAnmObj;
    }
    return ScnMdlSimple::RemoveAnmObj(type);
}

AnmObj* ScnMdl::GetAnmObj(AnmObjType type) {
    if (type == ANMOBJTYPE_SHP) {
        return mpAnmObjShp;
    }
    return ScnMdlSimple::GetAnmObj(type);
}

const AnmObj* ScnMdl::GetAnmObj(AnmObjType type) const {
    if (type == ANMOBJTYPE_SHP) {
        return mpAnmObjShp;
    }
    return ScnMdlSimple::GetAnmObj(type);
}

// Releases the shape animation object before running the base destructor.
ScnMdl::~ScnMdl() {
    if (mpAnmObjShp != NULL) {
        RemoveAnmObj(mpAnmObjShp);
    }
}

ScnMdl::CopiedVisAccess::CopiedVisAccess(ScnMdl* pScnMdl, u32 id) {
    bool valid = false;
    if (pScnMdl != NULL) {
        ResMdl mdl(pScnMdl->GetResMdl());
        ResNode node = mdl.GetResNode(id);
        if (node.IsValid()) {
            valid = true;
        }
    }
    if (valid) {
        mpScnMdl = pScnMdl;
        mNodeID = id;
        if (pScnMdl->mReplacement.visArray != NULL) {
            mpVis = pScnMdl->mReplacement.visArray + id;
        } else {
            mpVis = NULL;
        }
    } else {
        mNodeID = id;
        mpScnMdl = NULL;
        mpVis = NULL;
    }
}

// AnmObjShp TypeName data lives in the g3d_anmshp unit (US .rodata 0x8051D610,
// JP splits: g3d_anmshp .rodata 0x10); AnmObjShp::GetTypeObjStatic() below
// resolves it through symbols.txt.

bool ScnMdl::CopiedVisAccess::SetVisibilityEx(bool visible) {
    if (mpScnMdl != NULL) {
        if (mpVis != NULL) {
            // Mark the vis buffer dirty only when the stored visibility
            // actually changes; then write the new value into the buffer.
            if ((visible && !*mpVis) || (!visible && *mpVis)) {
                mpScnMdl->mFlagVisBuffer |=
                    VISBUFFER_DIRTY;
            }
            *mpVis = visible;
        } else {
            ResNode node = ResMdl(mpScnMdl->GetResMdl()).GetResNode(mNodeID);
            if (node.IsValid()) {
                if (visible) {
                    node.ref().flags |= ResNodeData::FLAG_VISIBLE;
                } else {
                    node.ref().flags &= ~ResNodeData::FLAG_VISIBLE;
                }
            }
        }
        return true;
    }
    return false;
}

bool ScnMdl::SetAnmObj(AnmObj* pObj, AnmObjType type) {
    if (pObj != NULL && pObj->GetParent() == NULL) {
        // Only SHP (and the NOT_SPECIFIED catch-all) are handled here; every
        // other type falls through to the simple model implementation.
        if ((u32)type - ANMOBJTYPE_SHP <= 1) {
            AnmObj* pAnmObj =
                (pObj != NULL &&
                 pObj->IsDerivedFrom(AnmObjShp::GetTypeObjStatic()))
                    ? pObj
                    : NULL;

            if (pAnmObj != NULL) {
                if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
                    return false;
                }

                if (mpAnmObjShp !=
                    NULL) {
                    RemoveAnmObj(
                        mpAnmObjShp);
                }

                mpAnmObjShp =
                    static_cast<AnmObjShp*>(pAnmObj);
                pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);

                mReplacement.flag &= ~1;
                return true;
            }

            if (type == ANMOBJTYPE_NOT_SPECIFIED) {
                return ScnMdlSimple::SetAnmObj(pObj, type);
            }
            return false;
        } else {
            return ScnMdlSimple::SetAnmObj(pObj, type);
        }
    }
    return false;
}

bool ScnMdl::RemoveAnmObj(AnmObj* pObj) {
    if (pObj == NULL) {
        return false;
    }

    if (pObj == mpAnmObjShp) {
        mpAnmObjShp->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjShp = NULL;

        if (!(mFlagRestoreVtx & 1)) {
            mReplacement.flag |= 1;
            return true;
        }

        // Restore the original vertex data from the model resource into the
        // shape-animation vertex buffers before releasing the shape object.
        if (mReplacement.vtxPosTable != NULL) {
            u32 num = ResMdl(GetResMdl()).GetResVtxPosNumEntries();
            for (u32 i = 0; i < num; i++) {
                ResVtxPos vtxPos = ResMdl(GetResMdl()).GetResVtxPos(i);
                if (vtxPos.ptr() != mReplacement.vtxPosTable[i]) {
                    vtxPos.CopyTo(mReplacement.vtxPosTable[i]);
                }
            }
        }
        if (mReplacement.vtxNrmTable != NULL) {
            u32 num = ResMdl(GetResMdl()).GetResVtxNrmNumEntries();
            for (u32 i = 0; i < num; i++) {
                ResVtxNrm vtxNrm = ResMdl(GetResMdl()).GetResVtxNrm(i);
                if (vtxNrm.ptr() != mReplacement.vtxNrmTable[i]) {
                    vtxNrm.CopyTo(mReplacement.vtxNrmTable[i]);
                }
            }
        }
        if (mReplacement.vtxClrTable != NULL) {
            u32 num = ResMdl(GetResMdl()).GetResVtxClrNumEntries();
            for (u32 i = 0; i < num; i++) {
                ResVtxClr vtxClr = ResMdl(GetResMdl()).GetResVtxClr(i);
                if (vtxClr.ptr() != mReplacement.vtxClrTable[i]) {
                    vtxClr.CopyTo(mReplacement.vtxClrTable[i]);
                }
            }
        }
        return true;
    }

    return ScnMdlSimple::RemoveAnmObj(pObj);
}

ScnMdl::CopiedMatAccess::CopiedMatAccess(ScnMdl* pScnMdl, u32 id) {
    mTexObj = ResTexObj(NULL);
    mTlutObj = ResTlutObj(NULL);
    mTexSrt = ResTexSrt(NULL);
    mChan = ResMatChan(NULL);
    mGenMode = ResGenMode(NULL);
    mMatMisc = ResMatMisc(NULL);
    mPix = ResMatPix(NULL);
    mTevColor = ResMatTevColor(NULL);
    mIndMtxAndScale = ResMatIndMtxAndScale(NULL);
    mTexCoordGen = ResMatTexCoordGen(NULL);
    mTev = ResTev(NULL);

    bool valid = false;
    if (pScnMdl != NULL) {
        ResMat mat = ResMdl(pScnMdl->GetResMdl()).GetResMat(id);
        if (mat.IsValid()) {
            valid = true;
        }
    }

    if (valid) {
        mpScnMdl = pScnMdl;
        mMatID = id;

        if (pScnMdl->mReplacement.texObjDataArray != NULL) {
            mTexObj = ResTexObj(&pScnMdl->mReplacement.texObjDataArray[id]);
        } else {
            mTexObj = ResTexObj(NULL);
        }
        if (pScnMdl->mReplacement.tlutObjDataArray != NULL) {
            mTlutObj = ResTlutObj(&pScnMdl->mReplacement.tlutObjDataArray[id]);
        } else {
            mTlutObj = ResTlutObj(NULL);
        }
        if (pScnMdl->mReplacement.texSrtDataArray != NULL) {
            mTexSrt = ResTexSrt(&pScnMdl->mReplacement.texSrtDataArray[id]);
        } else {
            mTexSrt = ResTexSrt(NULL);
        }
        if (pScnMdl->mReplacement.chanDataArray != NULL) {
            mChan = ResMatChan(&pScnMdl->mReplacement.chanDataArray[id]);
        } else {
            mChan = ResMatChan(NULL);
        }
        if (pScnMdl->mReplacement.genModeDataArray != NULL) {
            mGenMode = ResGenMode(&pScnMdl->mReplacement.genModeDataArray[id]);
        } else {
            mGenMode = ResGenMode(NULL);
        }
        if (pScnMdl->mReplacement.matMiscDataArray != NULL) {
            mMatMisc = ResMatMisc(&pScnMdl->mReplacement.matMiscDataArray[id]);
        } else {
            mMatMisc = ResMatMisc(NULL);
        }
        if (pScnMdl->mReplacement.pixDLArray != NULL) {
            mPix = ResMatPix(&pScnMdl->mReplacement.pixDLArray[id]);
        } else {
            mPix = ResMatPix(NULL);
        }
        if (pScnMdl->mReplacement.tevColorDLArray != NULL) {
            mTevColor = ResMatTevColor(&pScnMdl->mReplacement.tevColorDLArray[id]);
        } else {
            mTevColor = ResMatTevColor(NULL);
        }
        if (pScnMdl->mReplacement.indMtxAndScaleDLArray != NULL) {
            mIndMtxAndScale =
                ResMatIndMtxAndScale(&pScnMdl->mReplacement.indMtxAndScaleDLArray[id]);
        } else {
            mIndMtxAndScale = ResMatIndMtxAndScale(NULL);
        }
        if (pScnMdl->mReplacement.texCoordGenDLArray != NULL) {
            mTexCoordGen = ResMatTexCoordGen(&pScnMdl->mReplacement.texCoordGenDLArray[id]);
        } else {
            mTexCoordGen = ResMatTexCoordGen(NULL);
        }
        if (pScnMdl->mReplacement.tevDataArray != NULL) {
            mTev = ResTev(&pScnMdl->mReplacement.tevDataArray[id]);
        } else {
            mTev = ResTev(NULL);
        }
    } else {
        mMatID = id;
        mpScnMdl = NULL;
        mTexObj = ResTexObj(NULL);
        mTlutObj = ResTlutObj(NULL);
        mTexSrt = ResTexSrt(NULL);
        mChan = ResMatChan(NULL);
        mGenMode = ResGenMode(NULL);
        mMatMisc = ResMatMisc(NULL);
        mPix = ResMatPix(NULL);
        mTevColor = ResMatTevColor(NULL);
        mIndMtxAndScale = ResMatIndMtxAndScale(NULL);
        mTexCoordGen = ResMatTexCoordGen(NULL);
        mTev = ResTev(NULL);
    }
}

void ScnMdl::CleanMatBuffer(u32 idx, u32 option) {
    ResMat mat = ResMdl(GetResMdl()).GetResMat(idx);

    // Restore the original per-material data from the model resource into
    // each replacement buffer selected by the option bits.
    if (option & BUFFER_RESTEXOBJ) {
        if (mReplacement.texObjDataArray != NULL) {
            mat.GetResTexObj().CopyTo(&mReplacement.texObjDataArray[idx]);
        }
    }
    if (option & BUFFER_RESTLUTOBJ) {
        if (mReplacement.tlutObjDataArray != NULL) {
            mat.GetResTlutObj().CopyTo(&mReplacement.tlutObjDataArray[idx]);
        }
    }
    if (option & BUFFER_RESTEXSRT) {
        if (mReplacement.texSrtDataArray != NULL) {
            mat.GetResTexSrt().CopyTo(&mReplacement.texSrtDataArray[idx]);
        }
    }
    if (option & BUFFER_RESCHAN) {
        if (mReplacement.chanDataArray != NULL) {
            mat.GetResMatChan().CopyTo(&mReplacement.chanDataArray[idx]);
        }
    }
    if (option & BUFFER_RESGENMODE) {
        if (mReplacement.genModeDataArray != NULL) {
            mat.GetResGenMode().CopyTo(&mReplacement.genModeDataArray[idx]);
        }
    }
    if (option & BUFFER_RESMATMISC) {
        if (mReplacement.matMiscDataArray != NULL) {
            mat.GetResMatMisc().CopyTo(&mReplacement.matMiscDataArray[idx]);
        }
    }
    if (option & BUFFER_RESMATPIX) {
        if (mReplacement.pixDLArray != NULL) {
            mat.GetResMatPix().CopyTo(&mReplacement.pixDLArray[idx]).DCStore(false);
        }
    }
    if (option & BUFFER_RESTEVCOLOR) {
        if (mReplacement.tevColorDLArray != NULL) {
            mat.GetResMatTevColor()
                .CopyTo(&mReplacement.tevColorDLArray[idx])
                .DCStore(false);
        }
    }
    if (option & BUFFER_RESMATINDMTXSCALE) {
        if (mReplacement.indMtxAndScaleDLArray != NULL) {
            mat.GetResMatIndMtxAndScale()
                .CopyTo(&mReplacement.indMtxAndScaleDLArray[idx])
                .DCStore(false);
        }
    }
    if (option & BUFFER_RESMATTEXCOORDGEN) {
        if (mReplacement.texCoordGenDLArray != NULL) {
            mat.GetResMatTexCoordGen()
                .CopyTo(&mReplacement.texCoordGenDLArray[idx])
                .DCStore(false);
        }
    }
    if (option & BUFFER_RESTEV) {
        if (mReplacement.tevDataArray != NULL) {
            mat.GetResTev().CopyTo(&mReplacement.tevDataArray[idx]).DCStore(false);
        }
    }

    mpMatBufferDirtyFlag[idx] &=
        ~option;
}

inline ScnMdl::ScnMdl(MEMAllocator* pAllocator, ResMdl mdl,
                      math::MTX34* pWorldMtxArray, u32* pWorldMtxAttribArray,
                      math::MTX34* pViewPosMtxArray,
                      math::MTX33* pViewNrmMtxArray,
                      math::MTX34* pViewTexMtxArray, int numView,
                      int numViewMtx, DrawResMdlReplacement* pReplacement,
                      u32* pMatBufferDirtyFlag)
    : ScnMdlSimple(pAllocator, mdl, pWorldMtxArray, pWorldMtxAttribArray,
                   pViewPosMtxArray, pViewNrmMtxArray, pViewTexMtxArray,
                   numView, numViewMtx) {
    mpAnmObjShp = NULL;
    mFlagVisBuffer = 0;
    mpMatBufferDirtyFlag = pMatBufferDirtyFlag;
    mReplacement = *pReplacement;
}

// ---------------------------------------------------------------------------
// ScnMdl::Construct
//
// Single-allocation factory. The layout mirrors the ScnMdlSimple base
// (world/view matrix arrays, world-matrix attribute array) followed by the
// per-material replacement buffers selected by bufferOption, the vertex-data
// tables, and the copied vertex buffers. The ScnMdlSimple ctor runs on the
// block, then the vtable is patched to ScnMdl's (retail has no out-of-line
// ScnMdl ctor symbol).
// ---------------------------------------------------------------------------
ScnMdl* ScnMdl::Construct(MEMAllocator* pAllocator, u32* pSize, ResMdl mdl,
                          u32 bufferOption, int numView) {
    if (mdl.ptr() == NULL) {
        return NULL;
    }

    if (numView == 0) {
        numView = 1;
    } else if (numView > 16) {
        numView = 16;
    }

    u32 numPosNrmMtx = mdl.GetResMdlInfo().GetNumPosNrmMtx();
    u32 numViewMtx = mdl.GetResMdlInfo().GetNumViewMtx();
    u32 numMat = mdl.GetResMatNumEntries();
    u32 numNode = mdl.GetResNodeNumEntries();

    bool needNrm = mdl.GetResMdlInfo().ref().need_nrm_mtx_array != 0;
    bool needTex = mdl.GetResMdlInfo().ref().need_tex_mtx_array != 0;

    // View matrix array sizes (one per view, 32-byte aligned stride).
    u32 sizeViewPosMtx = numView * align32(numViewMtx * sizeof(math::MTX34));
    u32 sizeViewNrmMtx =
        needNrm ? numView * align32(numViewMtx * sizeof(math::MTX33)) : 0;
    u32 sizeViewTexMtx = needTex ? sizeViewPosMtx : 0;

    // Per-material replacement buffer sizes (0 when the option bit is off).
    u32 sizeDirtyFlags = numMat * 4;
    u32 sizeTexObj =
        (bufferOption & BUFFER_RESTEXOBJ) ? numMat * sizeof(ResTexObjData) : 0;
    u32 sizeTlutObj =
        (bufferOption & BUFFER_RESTLUTOBJ) ? numMat * sizeof(ResTlutObjData) : 0;
    u32 sizeTexSrt =
        (bufferOption & BUFFER_RESTEXSRT) ? numMat * sizeof(ResTexSrtData) : 0;
    u32 sizeChan =
        (bufferOption & BUFFER_RESCHAN) ? numMat * sizeof(ResChanData) : 0;
    u32 sizeGenMode =
        (bufferOption & BUFFER_RESGENMODE) ? numMat * sizeof(ResGenModeData) : 0;
    u32 sizeMatMisc =
        (bufferOption & BUFFER_RESMATMISC) ? numMat * sizeof(ResMatMiscData) : 0;
    u32 sizeVis = (bufferOption & BUFFER_RESANMVIS) ? numNode : 0;
    u32 sizePix =
        (bufferOption & BUFFER_RESMATPIX) ? numMat * sizeof(ResPixDL) : 0;
    u32 sizeTevColor =
        (bufferOption & BUFFER_RESTEVCOLOR) ? numMat * sizeof(ResTevColorDL) : 0;
    u32 sizeIndMtx = (bufferOption & BUFFER_RESMATINDMTXSCALE)
                         ? numMat * sizeof(ResIndMtxAndScaleDL)
                         : 0;
    u32 sizeTexCoordGen = (bufferOption & BUFFER_RESMATTEXCOORDGEN)
                              ? numMat * sizeof(ResTexCoordGenDL)
                              : 0;
    u32 sizeTev =
        (bufferOption & BUFFER_RESTEV) ? numMat * sizeof(ResTevData) : 0;

    // Vertex table sizes plus the copy size of every referenced vertex
    // buffer (a vertex buffer is referenced when some shape uses it).
    u32 sizeVtxPosTable = 0;
    u32 sizeVtxPosData = 0;
    if (bufferOption & BUFFER_RESVTXPOS) {
        u32 numVtxPos = mdl.GetResVtxPosNumEntries();
        u32 numShp = mdl.GetResShpNumEntries();
        sizeVtxPosTable = numVtxPos * 4;
        for (u32 i = 0; i < numVtxPos; i++) {
            ResVtxPos vtxPos = mdl.GetResVtxPos(i);
            u32 j = 0;
            for (; j < numShp; j++) {
                ResShp shp = mdl.GetResShp(j);
                if (vtxPos == shp.GetResVtxPos()) {
                    break;
                }
            }
            if (j != numShp) {
                sizeVtxPosData += align32(vtxPos.GetSize());
            }
        }
    }

    u32 sizeVtxNrmTable = 0;
    u32 sizeVtxNrmData = 0;
    if (bufferOption & BUFFER_RESVTXNRM) {
        u32 numVtxNrm = mdl.GetResVtxNrmNumEntries();
        u32 numShp = mdl.GetResShpNumEntries();
        sizeVtxNrmTable = numVtxNrm * 4;
        for (u32 i = 0; i < numVtxNrm; i++) {
            ResVtxNrm vtxNrm = mdl.GetResVtxNrm(i);
            u32 j = 0;
            for (; j < numShp; j++) {
                ResShp shp = mdl.GetResShp(j);
                if (vtxNrm == shp.GetResVtxNrm()) {
                    break;
                }
            }
            if (j != numShp) {
                sizeVtxNrmData += align32(vtxNrm.GetSize());
            }
        }
    }

    u32 sizeVtxClrTable = 0;
    u32 sizeVtxClrData = 0;
    if (bufferOption & BUFFER_RESVTXCLR) {
        u32 numVtxClr = mdl.GetResVtxClrNumEntries();
        u32 numShp = mdl.GetResShpNumEntries();
        sizeVtxClrTable = numVtxClr * 4;
        for (u32 i = 0; i < numVtxClr; i++) {
            ResVtxClr vtxClr = mdl.GetResVtxClr(i);
            u32 j = 0;
            for (; j < numShp; j++) {
                ResShp shp = mdl.GetResShp(j);
                bool used = true;
                if (vtxClr != shp.GetResVtxClr(0) &&
                    vtxClr != shp.GetResVtxClr(1)) {
                    used = false;
                }
                if (used) {
                    break;
                }
            }
            if (j != numShp) {
                sizeVtxClrData += align32(vtxClr.GetSize());
            }
        }
    }

    // Offsets of every sub-array within the single backing allocation. The
    // world matrix array starts right after the aligned base object (with
    // room for the two-matrix cache used by the LC DMA path).
    const u32 ofsWorldMtxArray =
        align32(sizeof(ScnMdlSimple)) + 2 * sizeof(math::MTX34);
    u32 ofsWorldMtxAttribArray =
        align32(ofsWorldMtxArray + numPosNrmMtx * sizeof(math::MTX34));
    u32 ofsViewPosMtxArray =
        align32(ofsWorldMtxAttribArray + numPosNrmMtx * sizeof(u32));
    u32 ofsViewNrmMtxArray = align32(ofsViewPosMtxArray + sizeViewPosMtx);
    u32 ofsViewTexMtxArray = align4(ofsViewNrmMtxArray + sizeViewNrmMtx);
    u32 ofsDirtyFlags = align32(ofsViewTexMtxArray + sizeViewTexMtx);
    u32 ofsTexObjDataArray = align4(ofsDirtyFlags + sizeDirtyFlags);
    u32 ofsTlutObjDataArray = align4(ofsTexObjDataArray + sizeTexObj);
    u32 ofsTexSrtDataArray = align4(ofsTlutObjDataArray + sizeTlutObj);
    u32 ofsChanDataArray = align4(ofsTexSrtDataArray + sizeTexSrt);
    u32 ofsGenModeDataArray = align4(ofsChanDataArray + sizeChan);
    u32 ofsMatMiscDataArray = align4(ofsGenModeDataArray + sizeGenMode);
    u32 ofsVisArray = align32(ofsMatMiscDataArray + sizeMatMisc);
    u32 ofsPixDLArray = align32(ofsVisArray + sizeVis);
    u32 ofsTevColorDLArray = align32(ofsPixDLArray + sizePix);
    u32 ofsIndMtxAndScaleDLArray = align32(ofsTevColorDLArray + sizeTevColor);
    u32 ofsTexCoordGenDLArray =
        align32(ofsIndMtxAndScaleDLArray + sizeIndMtx);
    u32 ofsTevDataArray = align32(ofsTexCoordGenDLArray + sizeTexCoordGen);
    u32 ofsVtxPosTable = align32(ofsTevDataArray + sizeTev);
    u32 ofsVtxNrmTable = ofsVtxPosTable + sizeVtxPosTable;
    u32 ofsVtxClrTable = ofsVtxNrmTable + sizeVtxNrmTable;
    u32 ofsVtxPosData = align32(ofsVtxClrTable + sizeVtxClrTable);
    u32 ofsVtxNrmData = align32(ofsVtxPosData + sizeVtxPosData);
    u32 ofsVtxClrData = align32(ofsVtxNrmData + sizeVtxNrmData);
    u32 total = align32(ofsVtxClrData + sizeVtxClrData);

    if (pSize != NULL) {
        *pSize = total;
    }

    ScnMdl* pObj = NULL;
    if (pAllocator != NULL) {
        u8* buf = static_cast<u8*>(MEMAllocFromAllocator(pAllocator, total));
        if (buf == NULL) {
            return NULL;
        }
        // Vertex restore request (bit 24): kept in mFlagRestoreVtx.
        u32 vtxRestore = bufferOption & BUFFER_RESTORE_VTX;

        // Replacement flag: 1 when no vertex restore is requested.
        u32 replFlag = 0;
        if (!vtxRestore) {
            replFlag = 1;
        }

        // Resolve the per-buffer pointers inside the allocation (NULL when
        // the matching option bit is clear).
        u8* pVisArray = sizeVis ? buf + ofsVisArray : NULL;
        ResTexObjData* pTexObjDataArray =
            sizeTexObj
                ? reinterpret_cast<ResTexObjData*>(buf + ofsTexObjDataArray)
                : NULL;
        ResTlutObjData* pTlutObjDataArray =
            sizeTlutObj
                ? reinterpret_cast<ResTlutObjData*>(buf + ofsTlutObjDataArray)
                : NULL;
        ResTexSrtData* pTexSrtDataArray =
            sizeTexSrt
                ? reinterpret_cast<ResTexSrtData*>(buf + ofsTexSrtDataArray)
                : NULL;
        ResChanData* pChanDataArray =
            sizeChan ? reinterpret_cast<ResChanData*>(buf + ofsChanDataArray)
                     : NULL;
        ResGenModeData* pGenModeDataArray =
            sizeGenMode
                ? reinterpret_cast<ResGenModeData*>(buf + ofsGenModeDataArray)
                : NULL;
        ResMatMiscData* pMatMiscDataArray =
            sizeMatMisc
                ? reinterpret_cast<ResMatMiscData*>(buf + ofsMatMiscDataArray)
                : NULL;
        ResPixDL* pPixDLArray =
            sizePix ? reinterpret_cast<ResPixDL*>(buf + ofsPixDLArray) : NULL;
        ResTevColorDL* pTevColorDLArray =
            sizeTevColor
                ? reinterpret_cast<ResTevColorDL*>(buf + ofsTevColorDLArray)
                : NULL;
        ResIndMtxAndScaleDL* pIndMtxAndScaleDLArray =
            sizeIndMtx
                ? reinterpret_cast<ResIndMtxAndScaleDL*>(
                      buf + ofsIndMtxAndScaleDLArray)
                : NULL;
        ResTexCoordGenDL* pTexCoordGenDLArray =
            sizeTexCoordGen
                ? reinterpret_cast<ResTexCoordGenDL*>(
                      buf + ofsTexCoordGenDLArray)
                : NULL;
        ResTevData* pTevDataArray =
            sizeTev ? reinterpret_cast<ResTevData*>(buf + ofsTevDataArray)
                    : NULL;
        ResVtxPosData** pVtxPosTable =
            sizeVtxPosTable
                ? reinterpret_cast<ResVtxPosData**>(buf + ofsVtxPosTable)
                : NULL;
        ResVtxNrmData** pVtxNrmTable =
            sizeVtxNrmTable
                ? reinterpret_cast<ResVtxNrmData**>(buf + ofsVtxNrmTable)
                : NULL;
        ResVtxClrData** pVtxClrTable =
            sizeVtxClrTable
                ? reinterpret_cast<ResVtxClrData**>(buf + ofsVtxClrTable)
                : NULL;

        // Copy the vertex data actually referenced by a shape into the
        // replacement buffers; unreferenced entries keep their resource
        // pointers.
        if (pVtxPosTable != NULL) {
            u32 numVtxPos = mdl.GetResVtxPosNumEntries();
            u32 numShp = mdl.GetResShpNumEntries();
            u32 ofs = ofsVtxPosData;
            for (u32 i = 0; i < numVtxPos; i++) {
                ResVtxPos vtxPos = mdl.GetResVtxPos(i);
                u32 j = 0;
                for (; j < numShp; j++) {
                    ResShp shp = mdl.GetResShp(j);
                    if (vtxPos == shp.GetResVtxPos()) {
                        break;
                    }
                }
                if (j != numShp) {
                    pVtxPosTable[i] =
                        reinterpret_cast<ResVtxPosData*>(buf + ofs);
                    ofs += align32(vtxPos.GetSize());
                    vtxPos.CopyTo(pVtxPosTable[i]);
                } else {
                    pVtxPosTable[i] = vtxPos.ptr();
                }
            }
        }

        if (pVtxNrmTable != NULL) {
            u32 numVtxNrm = mdl.GetResVtxNrmNumEntries();
            u32 numShp = mdl.GetResShpNumEntries();
            u32 ofs = ofsVtxNrmData;
            for (u32 i = 0; i < numVtxNrm; i++) {
                ResVtxNrm vtxNrm = mdl.GetResVtxNrm(i);
                u32 j = 0;
                for (; j < numShp; j++) {
                    ResShp shp = mdl.GetResShp(j);
                    if (vtxNrm == shp.GetResVtxNrm()) {
                        break;
                    }
                }
                if (j != numShp) {
                    pVtxNrmTable[i] =
                        reinterpret_cast<ResVtxNrmData*>(buf + ofs);
                    ofs += align32(vtxNrm.GetSize());
                    vtxNrm.CopyTo(pVtxNrmTable[i]);
                } else {
                    pVtxNrmTable[i] = vtxNrm.ptr();
                }
            }
        }

        if (pVtxClrTable != NULL) {
            u32 numVtxClr = mdl.GetResVtxClrNumEntries();
            u32 numShp = mdl.GetResShpNumEntries();
            u32 ofs = ofsVtxClrData;
            for (u32 i = 0; i < numVtxClr; i++) {
                ResVtxClr vtxClr = mdl.GetResVtxClr(i);
                u32 j = 0;
                for (; j < numShp; j++) {
                    ResShp shp = mdl.GetResShp(j);
                    bool used = true;
                    if (vtxClr != shp.GetResVtxClr(0) &&
                        vtxClr != shp.GetResVtxClr(1)) {
                        used = false;
                    }
                    if (used) {
                        break;
                    }
                }
                if (j != numShp) {
                    pVtxClrTable[i] =
                        reinterpret_cast<ResVtxClrData*>(buf + ofs);
                    ofs += align32(vtxClr.GetSize());
                    vtxClr.CopyTo(pVtxClrTable[i]);
                } else {
                    pVtxClrTable[i] = vtxClr.ptr();
                }
            }
        }

        DrawResMdlReplacement replacement;
        replacement.flag = replFlag;
        replacement.visArray = pVisArray;
        replacement.texObjDataArray = pTexObjDataArray;
        replacement.tlutObjDataArray = pTlutObjDataArray;
        replacement.texSrtDataArray = pTexSrtDataArray;
        replacement.chanDataArray = pChanDataArray;
        replacement.genModeDataArray = pGenModeDataArray;
        replacement.matMiscDataArray = pMatMiscDataArray;
        replacement.pixDLArray = pPixDLArray;
        replacement.tevColorDLArray = pTevColorDLArray;
        replacement.indMtxAndScaleDLArray = pIndMtxAndScaleDLArray;
        replacement.texCoordGenDLArray = pTexCoordGenDLArray;
        replacement.tevDataArray = pTevDataArray;
        replacement.vtxPosTable = pVtxPosTable;
        replacement.vtxNrmTable = pVtxNrmTable;
        replacement.vtxClrTable = pVtxClrTable;

        pObj = new (buf) ScnMdl(
            pAllocator, mdl,
            reinterpret_cast<math::MTX34*>(buf + ofsWorldMtxArray),
            reinterpret_cast<u32*>(buf + ofsWorldMtxAttribArray),
            reinterpret_cast<math::MTX34*>(buf + ofsViewPosMtxArray),
            sizeViewNrmMtx
                ? reinterpret_cast<math::MTX33*>(buf + ofsViewNrmMtxArray)
                : NULL,
            sizeViewTexMtx
                ? reinterpret_cast<math::MTX34*>(buf + ofsViewTexMtxArray)
                : NULL,
            numView, numViewMtx, &replacement,
            reinterpret_cast<u32*>(buf + ofsDirtyFlags));
        pObj->mFlagRestoreVtx = vtxRestore ? 1 : 0;
        pObj->InitBuffer();
    }
    return pObj;
}

// ---------------------------------------------------------------------------
// ScnMdl::InitBuffer
//
// Rebuilds the visibility buffer from the model's node visibility flags and
// copies every per-material replacement buffer back from the model resource,
// clearing the dirty flags.
// ---------------------------------------------------------------------------
void ScnMdl::InitBuffer() {
    ResMdl mdl = GetResMdl();

    u32 numMat = mdl.GetResMatNumEntries();
    u32 numNode = mdl.GetResNodeNumEntries();

    if (mReplacement.visArray != NULL) {
        u32 i = 0;
        if (numNode > 0) {
            for (; i < numNode; i++) {
                ResNode node = mdl.GetResNode(i);
                bool visible;
                if (node.IsValid()) {
                    visible =
                        (node.ref().flags & ResNodeData::FLAG_VISIBLE) != 0;
                } else {
                    visible = false;
                }
                if (visible) {
                    mReplacement.visArray[i] = 1;
                } else {
                    mReplacement.visArray[i] = 0;
                }
            }
        }
    }

    for (u32 i = 0; i < numMat; i++) {
        ResMat mat = mdl.GetResMat(i);
        mpMatBufferDirtyFlag[i] = 0;

        if (mReplacement.texObjDataArray != NULL) {
            mat.GetResTexObj().CopyTo(&mReplacement.texObjDataArray[i]);
        }
        if (mReplacement.tlutObjDataArray != NULL) {
            mat.GetResTlutObj().CopyTo(&mReplacement.tlutObjDataArray[i]);
        }
        if (mReplacement.texSrtDataArray != NULL) {
            mat.GetResTexSrt().CopyTo(&mReplacement.texSrtDataArray[i]);
        }
        if (mReplacement.chanDataArray != NULL) {
            mat.GetResMatChan().CopyTo(&mReplacement.chanDataArray[i]);
        }
        if (mReplacement.genModeDataArray != NULL) {
            mat.GetResGenMode().CopyTo(&mReplacement.genModeDataArray[i]);
        }
        if (mReplacement.matMiscDataArray != NULL) {
            mat.GetResMatMisc().CopyTo(&mReplacement.matMiscDataArray[i]);
        }
        if (mReplacement.pixDLArray != NULL) {
            mat.GetResMatPix().CopyTo(&mReplacement.pixDLArray[i]).DCStore(false);
        }
        if (mReplacement.tevColorDLArray != NULL) {
            mat.GetResMatTevColor()
                .CopyTo(&mReplacement.tevColorDLArray[i])
                .DCStore(false);
        }
        if (mReplacement.indMtxAndScaleDLArray != NULL) {
            mat.GetResMatIndMtxAndScale()
                .CopyTo(&mReplacement.indMtxAndScaleDLArray[i])
                .DCStore(false);
        }
        if (mReplacement.texCoordGenDLArray != NULL) {
            mat.GetResMatTexCoordGen()
                .CopyTo(&mReplacement.texCoordGenDLArray[i])
                .DCStore(false);
        }
        if (mReplacement.tevDataArray != NULL) {
            mat.GetResTev().CopyTo(&mReplacement.tevDataArray[i]).DCStore(false);
        }
    }
}

// ---------------------------------------------------------------------------
// ScnMdl::ScnMdl_G3DPROC_CALC_MAT
//
// Restores the dirty per-material replacement buffers, then re-applies the
// texture-pattern / texture-SRT / material-color animation results for this
// material and marks the affected buffers dirty again.
// ---------------------------------------------------------------------------
void ScnMdl::ScnMdl_G3DPROC_CALC_MAT(u32 param, void* pInfo) {

    CheckCallback_CALC_MAT(CALLBACK_TIMING_A, param, pInfo);

    ResMdl mdl = GetResMdl();
    u32 numMat = mdl.GetResMatNumEntries();

    for (u32 i = 0; i < numMat; i++) {
        ResMat mat = mdl.GetResMat(i);

        // Restore the buffers selected by the dirty bits.
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESTEXOBJ) {
            CleanMatBuffer(i, BUFFER_RESTEXOBJ);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESTLUTOBJ) {
            CleanMatBuffer(i, BUFFER_RESTLUTOBJ);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESMATINDMTXSCALE) {
            CleanMatBuffer(i, BUFFER_RESMATINDMTXSCALE);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESTEXSRT) {
            CleanMatBuffer(i, BUFFER_RESTEXSRT);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESCHAN) {
            CleanMatBuffer(i, BUFFER_RESCHAN);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESTEVCOLOR) {
            CleanMatBuffer(i, BUFFER_RESTEVCOLOR);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESGENMODE) {
            CleanMatBuffer(i, BUFFER_RESGENMODE);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESMATMISC) {
            CleanMatBuffer(i, BUFFER_RESMATMISC);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESMATPIX) {
            CleanMatBuffer(i, BUFFER_RESMATPIX);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESMATTEXCOORDGEN) {
            CleanMatBuffer(i, BUFFER_RESMATTEXCOORDGEN);
        }
        if (mpMatBufferDirtyFlag[i] & BUFFER_RESTEV) {
            CleanMatBuffer(i, BUFFER_RESTEV);
        }

        // Texture pattern animation: recompute the result and apply it.
        if (GetAnmObjTexPat() != NULL && GetAnmObjTexPat()->TestExistence(i)) {
            TexPatAnmResult patResult;
            for (u32 j = 0; j < TexPatAnmResult::NUM_OF_ANMS; j++) {
                patResult.tex[j] = ResTex(NULL);
            }
            for (u32 j = 0; j < TexPatAnmResult::NUM_OF_ANMS; j++) {
                patResult.pltt[j] = ResPltt(NULL);
            }

            const TexPatAnmResult* pResult =
                GetAnmObjTexPat()->GetResult(&patResult, i);

            ResTexObj texObj;
            if (mReplacement.texObjDataArray != NULL) {
                texObj = ResTexObj(&mReplacement.texObjDataArray[i]);
            } else {
                texObj = mat.GetResTexObj();
            }
            ResTlutObj tlutObj;
            if (mReplacement.tlutObjDataArray != NULL) {
                tlutObj = ResTlutObj(&mReplacement.tlutObjDataArray[i]);
            } else {
                tlutObj = mat.GetResTlutObj();
            }

            ApplyTexPatAnmResult(texObj, tlutObj, pResult);
            mpMatBufferDirtyFlag[i] |=
                BUFFER_RESTEXOBJ | BUFFER_RESTLUTOBJ;
        }

        // Texture SRT animation: recompute the result and apply it.
        if (GetAnmObjTexSrt() != NULL && GetAnmObjTexSrt()->TestExistence(i)) {
            TexSrtAnmResult srtResult;
            const TexSrtAnmResult* pResult =
                GetAnmObjTexSrt()->GetResult(&srtResult, i);

            ResTexSrt texSrt;
            if (mReplacement.texSrtDataArray != NULL) {
                texSrt = ResTexSrt(&mReplacement.texSrtDataArray[i]);
            } else {
                texSrt = mat.GetResTexSrt();
            }
            ResMatIndMtxAndScale indMtx;
            if (mReplacement.indMtxAndScaleDLArray != NULL) {
                indMtx = ResMatIndMtxAndScale(&mReplacement.indMtxAndScaleDLArray[i]);
            } else {
                indMtx = mat.GetResMatIndMtxAndScale();
            }

            ApplyTexSrtAnmResult(texSrt, indMtx, pResult);
            indMtx.DCStore(false);
            mpMatBufferDirtyFlag[i] |=
                BUFFER_RESTEXSRT | BUFFER_RESMATINDMTXSCALE;
        }

        // Material color animation: recompute the result and apply it.
        if (GetAnmObjMatClr() != NULL && GetAnmObjMatClr()->TestExistence(i)) {
            ResMatTevColor tevColor;
            if (mReplacement.tevColorDLArray != NULL) {
                tevColor = ResMatTevColor(&mReplacement.tevColorDLArray[i]);
            } else {
                tevColor = mat.GetResMatTevColor();
            }
            ResMatChan chan;
            if (mReplacement.chanDataArray != NULL) {
                chan = ResMatChan(&mReplacement.chanDataArray[i]);
            } else {
                chan = mat.GetResMatChan();
            }

            ClrAnmResult clrResult;
            const ClrAnmResult* pResult =
                GetAnmObjMatClr()->GetResult(&clrResult, i);

            ApplyClrAnmResult(chan, tevColor, pResult);
            tevColor.DCStore(false);
            mpMatBufferDirtyFlag[i] |= BUFFER_RESCHAN | BUFFER_RESTEVCOLOR;
        }
    }

    CheckCallback_CALC_MAT(CALLBACK_TIMING_C, param, pInfo);
}

// ---------------------------------------------------------------------------
// ScnMdl::G3dProc
// ---------------------------------------------------------------------------
void ScnMdl::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
    case G3DPROC_GATHER_SCNOBJ:
        ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ(param,
                                         static_cast<IScnObjGather*>(pInfo));
        break;

    case G3DPROC_CALC_WORLD: {
        ScnMdlSmpl_CalcPosture(param, static_cast<const math::MTX34*>(pInfo));

        // Rebuild the visibility buffer when it is marked dirty and the
        // refresh has not been suppressed.
        if ((mFlagVisBuffer & VISBUFFER_NOT_REFRESH_NEEDED) == 0 &&
            (mFlagVisBuffer & VISBUFFER_DIRTY) != 0) {
            ResMdl mdl = GetResMdl();
            u32 numNode = mdl.GetResNodeNumEntries();

            if (mReplacement.visArray != NULL) {
                u32 i = 0;
                if (numNode != 0) {
                    for (; i < numNode; i++) {
                    ResNode node = mdl.GetResNode(i);
                    bool visible;
                    if (node.IsValid()) {
                        visible = (node.ref().flags &
                                   ResNodeData::FLAG_VISIBLE) != 0;
                    } else {
                        visible = false;
                    }
                    if (visible) {
                        mReplacement.visArray[i] = 1;
                    } else {
                        mReplacement.visArray[i] = 0;
                    }
                    }
                }
            }
            mFlagVisBuffer &= ~VISBUFFER_DIRTY;
        }

        // Apply the visibility animation result into the vis buffer (or the
        // node flags when no buffer is present) and mark the buffer dirty.
        if (GetAnmObjVis() != NULL) {
            if (mReplacement.visArray != NULL) {
                ResMdl mdl = GetResMdl();
                ApplyVisAnmResult(mReplacement.visArray, mdl,
                                  GetAnmObjVis());
                mFlagVisBuffer |= VISBUFFER_DIRTY;
            } else {
                ResMdl mdl = GetResMdl();
                ApplyVisAnmResult(mdl, GetAnmObjVis());
            }
        }

        CheckCallback_CALC_WORLD(CALLBACK_TIMING_C, param, pInfo);
        break;
    }

    case G3DPROC_CALC_MAT:
        ScnMdl_G3DPROC_CALC_MAT(param, pInfo);
        break;

    case G3DPROC_CALC_VIEW:
        ScnMdlSmpl_G3DPROC_CALC_VIEW(param,
                                     static_cast<const math::MTX34*>(pInfo));
        break;

    case G3DPROC_DRAW_OPA:
        CheckCallback_DRAW_OPA(CALLBACK_TIMING_A, param, pInfo);
        DrawResMdlDirectly(
            GetResMdl(), GetViewPosMtxArray(), GetViewNrmMtxArray(),
            GetViewTexMtxArray(), GetByteCodeDrawOpa(), NULL,
            &mReplacement,
            (pInfo != NULL) ? *static_cast<const u32*>(pInfo) : GetDrawMode());
        CheckCallback_DRAW_OPA(CALLBACK_TIMING_C, param, pInfo);
        break;

    case G3DPROC_DRAW_XLU:
        CheckCallback_DRAW_XLU(CALLBACK_TIMING_A, param, pInfo);
        DrawResMdlDirectly(
            GetResMdl(), GetViewPosMtxArray(), GetViewNrmMtxArray(),
            GetViewTexMtxArray(), NULL, GetByteCodeDrawXlu(),
            &mReplacement,
            (pInfo != NULL) ? *static_cast<const u32*>(pInfo) : GetDrawMode());
        CheckCallback_DRAW_XLU(CALLBACK_TIMING_C, param, pInfo);
        break;

    case G3DPROC_UPDATEFRAME:
        UpdateFrame();
        if (mpAnmObjShp != NULL) {
            mpAnmObjShp->UpdateFrame();
        }
        break;

    case G3DPROC_CHILD_DETACHED:
        RemoveAnmObj(static_cast<AnmObj*>(pInfo));
        break;

    case G3DPROC_CALC_VTX:
        if (mpAnmObjShp != NULL) {
            CalcVtx(GetResMdl(), mpAnmObjShp, mReplacement.vtxPosTable,
                    mReplacement.vtxNrmTable, mReplacement.vtxClrTable);
        }
        break;

    default:
        DefG3dProcScnLeaf(task, param, pInfo);
        break;
    }
}

} // namespace g3d
} // namespace nw4r
