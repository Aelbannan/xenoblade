// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>

void* GetResTexSrt__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFb(void* self, bool enable)
{
    unsigned char* object = static_cast<unsigned char*>(self);
    void* resource = *reinterpret_cast<void**>(object);
    if (resource == 0 || *reinterpret_cast<void**>(object + 0x10) == 0)
        return 0;
    if (enable)
    {
        unsigned int index = *reinterpret_cast<unsigned int*>(object + 0x4);
        unsigned int* flags = *reinterpret_cast<unsigned int**>(static_cast<unsigned char*>(resource) + 0x140);
        flags[index] |= 0x4;
    }
    return *reinterpret_cast<void**>(object + 0x10);
}

namespace nw4r {
namespace g3d {

ResMatChan ScnMdl::CopiedMatAccess::GetResMatChan(bool markDirty) {
    if (mpScnMdl != NULL && mChan.IsValid()) {
        if (markDirty) {
            u32* dirtyFlags =
                *reinterpret_cast<u32**>(reinterpret_cast<u8*>(mpScnMdl) + 0x140);
            dirtyFlags[mMatID] |= 0x8;
        }
        return mChan;
    }
    return ResMatChan(NULL);
}

// The shared header does not declare the CopiedMatAccess "Ex" accessors, so
// they are emitted as free functions bound to the retail mangled symbols.
// Overlay of the private CopiedMatAccess field layout (offsets verified
// against the retail CopiedMatAccess ctor). NB: in retail the two accessors
// below are shifted by one field - GetResMatIndMtxAndScaleEx reads/writes
// mPix (dlPix at DL+0x0) and GetResMatTexCoordGenEx reads/writes mTevColor
// (dlTevColor at DL+0x20); only GetResTevEx uses its own field.
struct CopiedMatAccessExView {
    ScnMdl* mpScnMdl;           // at 0x0
    u32 mMatID;                 // at 0x4
    u8 field_0x08[0x20 - 0x8];  // ResTexObj .. ResMatMisc
    ResMatPix mPix;             // at 0x20
    ResMatTevColor mTevColor;   // at 0x24
    u8 field_0x28[0x30 - 0x28]; // ResMatIndMtxAndScale, ResMatTexCoordGen
    ResTev mTev;                // at 0x30
};

ResMatPix GetResMatIndMtxAndScaleEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(
    CopiedMatAccessExView* self) {
    if (self->mpScnMdl != NULL) {
        if (self->mPix.IsValid()) {
            return self->mPix;
        }
        ResMat mat = ResMdl(self->mpScnMdl->GetResMdl()).GetResMat(self->mMatID);
        return mat.GetResMatPix();
    }
    return ResMatPix(NULL);
}

ResMatTevColor GetResMatTexCoordGenEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(
    CopiedMatAccessExView* self) {
    if (self->mpScnMdl != NULL) {
        if (self->mTevColor.IsValid()) {
            return self->mTevColor;
        }
        ResMat mat = ResMdl(self->mpScnMdl->GetResMdl()).GetResMat(self->mMatID);
        return mat.GetResMatTevColor();
    }
    return ResMatTevColor(NULL);
}

ResTev GetResTevEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(
    CopiedMatAccessExView* self) {
    if (self->mpScnMdl != NULL) {
        if (self->mTev.IsValid()) {
            return self->mTev;
        }
        ResMat mat = ResMdl(self->mpScnMdl->GetResMdl()).GetResMat(self->mMatID);
        return mat.GetResTev();
    }
    return ResTev(NULL);
}

} // namespace g3d
} // namespace nw4r

void* GetResGenMode__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFb(void* self, bool enable) {
    struct Model {
        unsigned char pad[0x140];
        unsigned* flags;
    };
    struct Access {
        Model* model;
        unsigned index;
        unsigned char pad[0x10];
        void* resGenMode;
    };
    Access* access = static_cast<Access*>(self);
    if (access->model == 0 || access->resGenMode == 0)
        return 0;
    if (enable)
        access->model->flags[access->index] |= 0x10;
    return access->resGenMode;
}

void __ct__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFPQ34nw4r3g3d6ScnMdlUl(){}

// ScnMdl vtable (data lives in nw4r_data.o; the shared header declares no
// ScnMdl ctor, so Construct patches the base ctor's vtable manually). The
// incomplete-array declaration keeps MWCC from choosing SDA-relative
// addressing (retail materialises the vtable address with lis/addi).
extern char lbl_eu_805697A0[];

class ScnMdlVtblSlot {
public:
    u32 field_0x0; // vtable pointer
};

// MonolithSoft extension bit in the buffer option: when set, vertex data is
// restored from the model resource when a shape animation object is removed
// (stored inverted in the DrawResMdlReplacement flag at 0x144).
static const u32 BUFFER_RESTORE_VTX = (1 << 24);

void IsDerivedFrom__Q34nw4r3g3d6ScnMdlCFQ44nw4r3g3d6G3dObj7TypeObj(){}

void GetTypeName__Q34nw4r3g3d6ScnMdlCFv(){}

extern "C" void* GetTypeObj__Q34nw4r3g3d6ScnMdlCFv(void) { return (void*)lbl_eu_8051D7C0; }

namespace nw4r {
namespace g3d {

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("ScnMdl")> lbl_eu_8051D7C0 = {sizeof("ScnMdl"), "ScnMdl"};

bool ScnMdl::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D7C0) ? true
         : other == TypeObj(lbl_eu_8051D7A8) ? true
         : other == TypeObj(lbl_eu_8051D778) ? true
         : other == TypeObj(lbl_eu_8051D768) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj ScnMdl::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D7C0);
}

const char* ScnMdl::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

// Retail ScnMdl tail-field layout. The header's ScnMdl declares a shadowing
// mpAnmObjShp that shifts its own fields by 4; in retail, mpAnmObjShp is the
// member inherited from ScnMdlSimple (at 0x138) and mFlagVisBuffer follows at
// 0x13C. Access the tail fields through this overlay view of the retail layout.
struct ScnMdlTailView {
    u8 field_0x00[0x138];       // ScnMdlSimple storage (incl. mpAnmObjChr..TexSrt)
    AnmObjShp* mpAnmObjShp;     // at 0x138 (inherited from ScnMdlSimple)
    u32 mFlagVisBuffer;         // at 0x13C
    u32* mpMatBufferDirtyFlag;  // at 0x140
    u32 field_0x144;            // at 0x144 (DrawResMdlReplacement flag)
    u8* mpVisBuffer;            // at 0x148 (DrawResMdlReplacement.visArray)
    ResTexObjData* texObjDataArray;             // at 0x14C
    ResTlutObjData* tlutObjDataArray;           // at 0x150
    ResTexSrtData* texSrtDataArray;             // at 0x154
    ResChanData* chanDataArray;                 // at 0x158
    ResGenModeData* genModeDataArray;           // at 0x15C
    ResMatMiscData* matMiscDataArray;           // at 0x160
    ResPixDL* pixDLArray;                       // at 0x164
    ResTevColorDL* tevColorDLArray;             // at 0x168
    ResIndMtxAndScaleDL* indMtxAndScaleDLArray; // at 0x16C
    ResTexCoordGenDL* texCoordGenDLArray;       // at 0x170
    ResTevData* tevDataArray;                   // at 0x174
    ResVtxPosData** vtxPosTable;                // at 0x178
    ResVtxNrmData** vtxNrmTable;                // at 0x17C
    ResVtxClrData** vtxClrTable;                // at 0x180
    u32 field_0x184;                            // at 0x184 (shape-anm vertex restore flag)
};

bool ScnMdl::SetScnObjOption(u32 option, u32 value) {
    ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(this);
    switch (option) {
    case OPTID_VISBUFFER_REFRESH_NEEDED:
        if (value) {
            tail.mFlagVisBuffer &= ~VISBUFFER_NOT_REFRESH_NEEDED;
        } else {
            tail.mFlagVisBuffer |= VISBUFFER_NOT_REFRESH_NEEDED;
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

    const ScnMdlTailView& tail =
        *reinterpret_cast<const ScnMdlTailView*>(this);
    switch (option) {
    case OPTID_VISBUFFER_REFRESH_NEEDED:
        *pValue = (tail.mFlagVisBuffer & VISBUFFER_NOT_REFRESH_NEEDED) ? 0 : 1;
        break;

    default:
        return ScnMdlSimple::GetScnObjOption(option, pValue);
    }
    return true;
}

AnmObj* ScnMdl::RemoveAnmObj(AnmObjType type) {
    if (type == ANMOBJTYPE_SHP) {
        AnmObj* pAnmObj = reinterpret_cast<ScnMdlTailView*>(this)->mpAnmObjShp;
        RemoveAnmObj(pAnmObj);
        return pAnmObj;
    }
    return ScnMdlSimple::RemoveAnmObj(type);
}

AnmObj* ScnMdl::GetAnmObj(AnmObjType type) {
    if (type == ANMOBJTYPE_SHP) {
        return reinterpret_cast<ScnMdlTailView*>(this)->mpAnmObjShp;
    }
    return ScnMdlSimple::GetAnmObj(type);
}

const AnmObj* ScnMdl::GetAnmObj(AnmObjType type) const {
    if (type == ANMOBJTYPE_SHP) {
        return reinterpret_cast<const ScnMdlTailView*>(this)->mpAnmObjShp;
    }
    return ScnMdlSimple::GetAnmObj(type);
}

// Releases the shape animation object (read at retail offset 0x138, the
// member inherited from ScnMdlSimple) before running the base destructor.
ScnMdl::~ScnMdl() {
    ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(this);
    if (tail.mpAnmObjShp != NULL) {
        RemoveAnmObj(tail.mpAnmObjShp);
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
        ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(pScnMdl);
        if (tail.mpVisBuffer != NULL) {
            mpVis = tail.mpVisBuffer + id;
        } else {
            mpVis = NULL;
        }
    } else {
        mNodeID = id;
        mpScnMdl = NULL;
        mpVis = NULL;
    }
}

// AnmObjShp TypeName data (referenced by ScnMdl::SetAnmObj for the SHP
// attach check). Emitted here until the g3d_anmshp unit owns it.
extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("AnmObjShp")>
    lbl_eu_8051D610 = {sizeof("AnmObjShp"), "AnmObjShp"};

bool ScnMdl::CopiedVisAccess::SetVisibilityEx(bool visible) {
    if (mpScnMdl != NULL) {
        if (mpVis != NULL) {
            // Mark the vis buffer dirty only when the stored visibility
            // actually changes; then write the new value into the buffer.
            if ((visible && !*mpVis) || (!visible && *mpVis)) {
                reinterpret_cast<ScnMdlTailView*>(mpScnMdl)->mFlagVisBuffer |=
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
                 pObj->IsDerivedFrom(TypeObj(lbl_eu_8051D610)))
                    ? pObj
                    : NULL;

            if (pAnmObj != NULL) {
                if (!pAnmObj->TestAnmFlag(AnmObj::FLAG_ANM_BOUND)) {
                    return false;
                }

                if (reinterpret_cast<ScnMdlTailView*>(this)->mpAnmObjShp !=
                    NULL) {
                    RemoveAnmObj(
                        reinterpret_cast<ScnMdlTailView*>(this)->mpAnmObjShp);
                }

                reinterpret_cast<ScnMdlTailView*>(this)->mpAnmObjShp =
                    static_cast<AnmObjShp*>(pAnmObj);
                pAnmObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);

                reinterpret_cast<ScnMdlTailView*>(this)->field_0x144 &= ~1;
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

    ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(this);
    if (pObj == tail.mpAnmObjShp) {
        tail.mpAnmObjShp->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        tail.mpAnmObjShp = NULL;

        if (!(tail.field_0x184 & 1)) {
            tail.field_0x144 |= 1;
            return true;
        }

        // Restore the original vertex data from the model resource into the
        // shape-animation vertex buffers before releasing the shape object.
        if (tail.vtxPosTable != NULL) {
            u32 num = ResMdl(GetResMdl()).GetResVtxPosNumEntries();
            for (u32 i = 0; i < num; i++) {
                ResVtxPos vtxPos = ResMdl(GetResMdl()).GetResVtxPos(i);
                if (vtxPos.ptr() != tail.vtxPosTable[i]) {
                    vtxPos.CopyTo(tail.vtxPosTable[i]);
                }
            }
        }
        if (tail.vtxNrmTable != NULL) {
            u32 num = ResMdl(GetResMdl()).GetResVtxNrmNumEntries();
            for (u32 i = 0; i < num; i++) {
                ResVtxNrm vtxNrm = ResMdl(GetResMdl()).GetResVtxNrm(i);
                if (vtxNrm.ptr() != tail.vtxNrmTable[i]) {
                    vtxNrm.CopyTo(tail.vtxNrmTable[i]);
                }
            }
        }
        if (tail.vtxClrTable != NULL) {
            u32 num = ResMdl(GetResMdl()).GetResVtxClrNumEntries();
            for (u32 i = 0; i < num; i++) {
                ResVtxClr vtxClr = ResMdl(GetResMdl()).GetResVtxClr(i);
                if (vtxClr.ptr() != tail.vtxClrTable[i]) {
                    vtxClr.CopyTo(tail.vtxClrTable[i]);
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

        ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(pScnMdl);
        if (tail.texObjDataArray != NULL) {
            mTexObj = ResTexObj(&tail.texObjDataArray[id]);
        } else {
            mTexObj = ResTexObj(NULL);
        }
        if (tail.tlutObjDataArray != NULL) {
            mTlutObj = ResTlutObj(&tail.tlutObjDataArray[id]);
        } else {
            mTlutObj = ResTlutObj(NULL);
        }
        if (tail.texSrtDataArray != NULL) {
            mTexSrt = ResTexSrt(&tail.texSrtDataArray[id]);
        } else {
            mTexSrt = ResTexSrt(NULL);
        }
        if (tail.chanDataArray != NULL) {
            mChan = ResMatChan(&tail.chanDataArray[id]);
        } else {
            mChan = ResMatChan(NULL);
        }
        if (tail.genModeDataArray != NULL) {
            mGenMode = ResGenMode(&tail.genModeDataArray[id]);
        } else {
            mGenMode = ResGenMode(NULL);
        }
        if (tail.matMiscDataArray != NULL) {
            mMatMisc = ResMatMisc(&tail.matMiscDataArray[id]);
        } else {
            mMatMisc = ResMatMisc(NULL);
        }
        if (tail.pixDLArray != NULL) {
            mPix = ResMatPix(&tail.pixDLArray[id]);
        } else {
            mPix = ResMatPix(NULL);
        }
        if (tail.tevColorDLArray != NULL) {
            mTevColor = ResMatTevColor(&tail.tevColorDLArray[id]);
        } else {
            mTevColor = ResMatTevColor(NULL);
        }
        if (tail.indMtxAndScaleDLArray != NULL) {
            mIndMtxAndScale =
                ResMatIndMtxAndScale(&tail.indMtxAndScaleDLArray[id]);
        } else {
            mIndMtxAndScale = ResMatIndMtxAndScale(NULL);
        }
        if (tail.texCoordGenDLArray != NULL) {
            mTexCoordGen = ResMatTexCoordGen(&tail.texCoordGenDLArray[id]);
        } else {
            mTexCoordGen = ResMatTexCoordGen(NULL);
        }
        if (tail.tevDataArray != NULL) {
            mTev = ResTev(&tail.tevDataArray[id]);
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
    ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(this);

    // Restore the original per-material data from the model resource into
    // each replacement buffer selected by the option bits.
    if (option & BUFFER_RESTEXOBJ) {
        if (tail.texObjDataArray != NULL) {
            mat.GetResTexObj().CopyTo(&tail.texObjDataArray[idx]);
        }
    }
    if (option & BUFFER_RESTLUTOBJ) {
        if (tail.tlutObjDataArray != NULL) {
            mat.GetResTlutObj().CopyTo(&tail.tlutObjDataArray[idx]);
        }
    }
    if (option & BUFFER_RESTEXSRT) {
        if (tail.texSrtDataArray != NULL) {
            mat.GetResTexSrt().CopyTo(&tail.texSrtDataArray[idx]);
        }
    }
    if (option & BUFFER_RESCHAN) {
        if (tail.chanDataArray != NULL) {
            mat.GetResMatChan().CopyTo(&tail.chanDataArray[idx]);
        }
    }
    if (option & BUFFER_RESGENMODE) {
        if (tail.genModeDataArray != NULL) {
            mat.GetResGenMode().CopyTo(&tail.genModeDataArray[idx]);
        }
    }
    if (option & BUFFER_RESMATMISC) {
        if (tail.matMiscDataArray != NULL) {
            mat.GetResMatMisc().CopyTo(&tail.matMiscDataArray[idx]);
        }
    }
    if (option & BUFFER_RESMATPIX) {
        if (tail.pixDLArray != NULL) {
            mat.GetResMatPix().CopyTo(&tail.pixDLArray[idx]).DCStore(false);
        }
    }
    if (option & BUFFER_RESTEVCOLOR) {
        if (tail.tevColorDLArray != NULL) {
            mat.GetResMatTevColor()
                .CopyTo(&tail.tevColorDLArray[idx])
                .DCStore(false);
        }
    }
    if (option & BUFFER_RESMATINDMTXSCALE) {
        if (tail.indMtxAndScaleDLArray != NULL) {
            mat.GetResMatIndMtxAndScale()
                .CopyTo(&tail.indMtxAndScaleDLArray[idx])
                .DCStore(false);
        }
    }
    if (option & BUFFER_RESMATTEXCOORDGEN) {
        if (tail.texCoordGenDLArray != NULL) {
            mat.GetResMatTexCoordGen()
                .CopyTo(&tail.texCoordGenDLArray[idx])
                .DCStore(false);
        }
    }
    if (option & BUFFER_RESTEV) {
        if (tail.tevDataArray != NULL) {
            mat.GetResTev().CopyTo(&tail.tevDataArray[idx]).DCStore(false);
        }
    }

    reinterpret_cast<ScnMdlTailView*>(this)->mpMatBufferDirtyFlag[idx] &=
        ~option;
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
        pObj = reinterpret_cast<ScnMdl*>(buf);

        // Vertex restore request (bit 24): kept for the tail flag at 0x184.
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

        if (pObj != NULL) {
            // Base construction + vtable patch (no out-of-line ScnMdl ctor
            // in retail).
            new (buf) ScnMdlSimple(
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
                numView, numViewMtx);
            reinterpret_cast<ScnMdlVtblSlot*>(pObj)->field_0x0 =
                reinterpret_cast<u32>(&lbl_eu_805697A0);

            // ScnMdl-specific tail fields.
            ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(pObj);
            tail.mpAnmObjShp = NULL;
            tail.mFlagVisBuffer = 0;
            tail.mpMatBufferDirtyFlag =
                reinterpret_cast<u32*>(buf + ofsDirtyFlags);
            tail.field_0x144 = replFlag;
            tail.mpVisBuffer = pVisArray;
            tail.texObjDataArray = pTexObjDataArray;
            tail.tlutObjDataArray = pTlutObjDataArray;
            tail.texSrtDataArray = pTexSrtDataArray;
            tail.chanDataArray = pChanDataArray;
            tail.genModeDataArray = pGenModeDataArray;
            tail.matMiscDataArray = pMatMiscDataArray;
            tail.pixDLArray = pPixDLArray;
            tail.tevColorDLArray = pTevColorDLArray;
            tail.indMtxAndScaleDLArray = pIndMtxAndScaleDLArray;
            tail.texCoordGenDLArray = pTexCoordGenDLArray;
            tail.tevDataArray = pTevDataArray;
            tail.vtxPosTable = pVtxPosTable;
            tail.vtxNrmTable = pVtxNrmTable;
            tail.vtxClrTable = pVtxClrTable;
            tail.field_0x184 = vtxRestore ? 1 : 0;
        }
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
    ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(this);
    ResMdl mdl = GetResMdl();

    u32 numMat = mdl.GetResMatNumEntries();
    u32 numNode = mdl.GetResNodeNumEntries();

    if (tail.mpVisBuffer != NULL && numNode != 0) {
        for (u32 i = 0; i < numNode; i++) {
            ResNode node = mdl.GetResNode(i);
            bool visible;
            if (node.IsValid()) {
                visible =
                    (node.ref().flags & ResNodeData::FLAG_VISIBLE) != 0;
            } else {
                visible = false;
            }
            if (visible) {
                tail.mpVisBuffer[i] = 1;
            } else {
                tail.mpVisBuffer[i] = 0;
            }
        }
    }

    for (u32 i = 0; i < numMat; i++) {
        ResMat mat = mdl.GetResMat(i);
        tail.mpMatBufferDirtyFlag[i] = 0;

        if (tail.texObjDataArray != NULL) {
            mat.GetResTexObj().CopyTo(&tail.texObjDataArray[i]);
        }
        if (tail.tlutObjDataArray != NULL) {
            mat.GetResTlutObj().CopyTo(&tail.tlutObjDataArray[i]);
        }
        if (tail.texSrtDataArray != NULL) {
            mat.GetResTexSrt().CopyTo(&tail.texSrtDataArray[i]);
        }
        if (tail.chanDataArray != NULL) {
            mat.GetResMatChan().CopyTo(&tail.chanDataArray[i]);
        }
        if (tail.genModeDataArray != NULL) {
            mat.GetResGenMode().CopyTo(&tail.genModeDataArray[i]);
        }
        if (tail.matMiscDataArray != NULL) {
            mat.GetResMatMisc().CopyTo(&tail.matMiscDataArray[i]);
        }
        if (tail.pixDLArray != NULL) {
            mat.GetResMatPix().CopyTo(&tail.pixDLArray[i]).DCStore(false);
        }
        if (tail.tevColorDLArray != NULL) {
            mat.GetResMatTevColor()
                .CopyTo(&tail.tevColorDLArray[i])
                .DCStore(false);
        }
        if (tail.indMtxAndScaleDLArray != NULL) {
            mat.GetResMatIndMtxAndScale()
                .CopyTo(&tail.indMtxAndScaleDLArray[i])
                .DCStore(false);
        }
        if (tail.texCoordGenDLArray != NULL) {
            mat.GetResMatTexCoordGen()
                .CopyTo(&tail.texCoordGenDLArray[i])
                .DCStore(false);
        }
        if (tail.tevDataArray != NULL) {
            mat.GetResTev().CopyTo(&tail.tevDataArray[i]).DCStore(false);
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
    ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(this);

    CheckCallback_CALC_MAT(CALLBACK_TIMING_A, param, pInfo);

    ResMdl mdl = GetResMdl();
    u32 numMat = mdl.GetResMatNumEntries();

    for (u32 i = 0; i < numMat; i++) {
        ResMat mat = mdl.GetResMat(i);

        // Restore the buffers selected by the dirty bits.
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESTEXOBJ) {
            CleanMatBuffer(i, BUFFER_RESTEXOBJ);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESTLUTOBJ) {
            CleanMatBuffer(i, BUFFER_RESTLUTOBJ);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESMATINDMTXSCALE) {
            CleanMatBuffer(i, BUFFER_RESMATINDMTXSCALE);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESTEXSRT) {
            CleanMatBuffer(i, BUFFER_RESTEXSRT);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESCHAN) {
            CleanMatBuffer(i, BUFFER_RESCHAN);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESTEVCOLOR) {
            CleanMatBuffer(i, BUFFER_RESTEVCOLOR);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESGENMODE) {
            CleanMatBuffer(i, BUFFER_RESGENMODE);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESMATMISC) {
            CleanMatBuffer(i, BUFFER_RESMATMISC);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESMATPIX) {
            CleanMatBuffer(i, BUFFER_RESMATPIX);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESMATTEXCOORDGEN) {
            CleanMatBuffer(i, BUFFER_RESMATTEXCOORDGEN);
        }
        if (tail.mpMatBufferDirtyFlag[i] & BUFFER_RESTEV) {
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
            if (tail.texObjDataArray != NULL) {
                texObj = ResTexObj(&tail.texObjDataArray[i]);
            } else {
                texObj = mat.GetResTexObj();
            }
            ResTlutObj tlutObj;
            if (tail.tlutObjDataArray != NULL) {
                tlutObj = ResTlutObj(&tail.tlutObjDataArray[i]);
            } else {
                tlutObj = mat.GetResTlutObj();
            }

            ApplyTexPatAnmResult(texObj, tlutObj, pResult);
            tail.mpMatBufferDirtyFlag[i] |=
                BUFFER_RESTEXOBJ | BUFFER_RESTLUTOBJ;
        }

        // Texture SRT animation: recompute the result and apply it.
        if (GetAnmObjTexSrt() != NULL && GetAnmObjTexSrt()->TestExistence(i)) {
            TexSrtAnmResult srtResult;
            const TexSrtAnmResult* pResult =
                GetAnmObjTexSrt()->GetResult(&srtResult, i);

            ResTexSrt texSrt;
            if (tail.texSrtDataArray != NULL) {
                texSrt = ResTexSrt(&tail.texSrtDataArray[i]);
            } else {
                texSrt = mat.GetResTexSrt();
            }
            ResMatIndMtxAndScale indMtx;
            if (tail.indMtxAndScaleDLArray != NULL) {
                indMtx = ResMatIndMtxAndScale(&tail.indMtxAndScaleDLArray[i]);
            } else {
                indMtx = mat.GetResMatIndMtxAndScale();
            }

            ApplyTexSrtAnmResult(texSrt, indMtx, pResult);
            indMtx.DCStore(false);
            tail.mpMatBufferDirtyFlag[i] |=
                BUFFER_RESTEXSRT | BUFFER_RESMATINDMTXSCALE;
        }

        // Material color animation: recompute the result and apply it.
        if (GetAnmObjMatClr() != NULL && GetAnmObjMatClr()->TestExistence(i)) {
            ResMatTevColor tevColor;
            if (tail.tevColorDLArray != NULL) {
                tevColor = ResMatTevColor(&tail.tevColorDLArray[i]);
            } else {
                tevColor = mat.GetResMatTevColor();
            }
            ResMatChan chan;
            if (tail.chanDataArray != NULL) {
                chan = ResMatChan(&tail.chanDataArray[i]);
            } else {
                chan = mat.GetResMatChan();
            }

            ClrAnmResult clrResult;
            const ClrAnmResult* pResult =
                GetAnmObjMatClr()->GetResult(&clrResult, i);

            ApplyClrAnmResult(chan, tevColor, pResult);
            tevColor.DCStore(false);
            tail.mpMatBufferDirtyFlag[i] |= BUFFER_RESCHAN | BUFFER_RESTEVCOLOR;
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

    ScnMdlTailView& tail = *reinterpret_cast<ScnMdlTailView*>(this);

    switch (task) {
    case G3DPROC_GATHER_SCNOBJ:
        ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ(param,
                                         static_cast<IScnObjGather*>(pInfo));
        break;

    case G3DPROC_CALC_WORLD: {
        ScnMdlSmpl_CalcPosture(param, static_cast<const math::MTX34*>(pInfo));

        // Rebuild the visibility buffer when it is marked dirty and the
        // refresh has not been suppressed.
        if ((tail.mFlagVisBuffer & VISBUFFER_NOT_REFRESH_NEEDED) == 0 &&
            (tail.mFlagVisBuffer & VISBUFFER_DIRTY) != 0) {
            ResMdl mdl = GetResMdl();
            u32 numNode = mdl.GetResNodeNumEntries();

            if (tail.mpVisBuffer != NULL && numNode != 0) {
                for (u32 i = 0; i < numNode; i++) {
                    ResNode node = mdl.GetResNode(i);
                    bool visible;
                    if (node.IsValid()) {
                        visible = (node.ref().flags &
                                   ResNodeData::FLAG_VISIBLE) != 0;
                    } else {
                        visible = false;
                    }
                    if (visible) {
                        tail.mpVisBuffer[i] = 1;
                    } else {
                        tail.mpVisBuffer[i] = 0;
                    }
                }
            }
            tail.mFlagVisBuffer &= ~VISBUFFER_DIRTY;
        }

        // Apply the visibility animation result into the vis buffer (or the
        // node flags when no buffer is present) and mark the buffer dirty.
        if (GetAnmObjVis() != NULL) {
            if (tail.mpVisBuffer != NULL) {
                ApplyVisAnmResult(tail.mpVisBuffer, GetResMdl(),
                                  GetAnmObjVis());
                tail.mFlagVisBuffer |= VISBUFFER_DIRTY;
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
            reinterpret_cast<DrawResMdlReplacement*>(&tail.field_0x144),
            (pInfo != NULL) ? *static_cast<const u32*>(pInfo) : GetDrawMode());
        CheckCallback_DRAW_OPA(CALLBACK_TIMING_C, param, pInfo);
        break;

    case G3DPROC_DRAW_XLU:
        CheckCallback_DRAW_XLU(CALLBACK_TIMING_A, param, pInfo);
        DrawResMdlDirectly(
            GetResMdl(), GetViewPosMtxArray(), GetViewNrmMtxArray(),
            GetViewTexMtxArray(), NULL, GetByteCodeDrawXlu(),
            reinterpret_cast<DrawResMdlReplacement*>(&tail.field_0x144),
            (pInfo != NULL) ? *static_cast<const u32*>(pInfo) : GetDrawMode());
        CheckCallback_DRAW_XLU(CALLBACK_TIMING_C, param, pInfo);
        break;

    case G3DPROC_UPDATEFRAME:
        UpdateFrame();
        if (tail.mpAnmObjShp != NULL) {
            tail.mpAnmObjShp->UpdateFrame();
        }
        break;

    case G3DPROC_CHILD_DETACHED:
        RemoveAnmObj(static_cast<AnmObj*>(pInfo));
        break;

    case G3DPROC_CALC_VTX:
        if (tail.mpAnmObjShp != NULL) {
            CalcVtx(GetResMdl(), tail.mpAnmObjShp, tail.vtxPosTable,
                    tail.vtxNrmTable, tail.vtxClrTable);
        }
        break;

    default:
        DefG3dProcScnLeaf(task, param, pInfo);
        break;
    }
}

} // namespace g3d
} // namespace nw4r
