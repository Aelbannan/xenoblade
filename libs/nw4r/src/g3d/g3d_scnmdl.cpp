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

void GetResMatIndMtxAndScaleEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(){}

void GetResMatTexCoordGenEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(){}

void GetResTevEx__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFv(){}

void __ct__Q44nw4r3g3d6ScnMdl15CopiedMatAccessFPQ34nw4r3g3d6ScnMdlUl(){}

void SetVisibilityEx__Q44nw4r3g3d6ScnMdl15CopiedVisAccessFb(){}

void __ct__Q44nw4r3g3d6ScnMdl15CopiedVisAccessFPQ34nw4r3g3d6ScnMdlUl(){}

void Construct__Q34nw4r3g3d6ScnMdlFP12MEMAllocatorPUlQ34nw4r3g3d6ResMdlUli(){}

void ScnMdl_G3DPROC_CALC_MAT__Q34nw4r3g3d6ScnMdlFUlPv(){}

void G3dProc__Q34nw4r3g3d6ScnMdlFUlUlPv(){}

void InitBuffer__Q34nw4r3g3d6ScnMdlFv(){}

void CleanMatBuffer__Q34nw4r3g3d6ScnMdlFUlUl(){}

void SetAnmObj__Q34nw4r3g3d6ScnMdlFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType(){}

void RemoveAnmObj__Q34nw4r3g3d6ScnMdlFPQ34nw4r3g3d6AnmObj(){}



void __dt__Q34nw4r3g3d6ScnMdlFv(){}

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

} // namespace g3d
} // namespace nw4r
