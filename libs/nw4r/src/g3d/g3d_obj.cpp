#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("G3dObj")> lbl_eu_8051D640 = {
    sizeof("G3dObj"), "G3dObj"};

NW4R_G3D_RTTI_DEF(G3dObj);

bool G3dObj::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D640);
}

const G3dObj::TypeObj G3dObj::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D640);
}

const char* G3dObj::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}

G3dObj::~G3dObj() {
    MEMFreeToAllocator(mpHeap, this);
}

void G3dObj::Destroy() {
    G3dObj* pParent = GetParent();

    if (pParent != NULL) {
        pParent->G3dProc(G3DPROC_CHILD_DETACHED, 0, this);
    }

    delete this;
}

} // namespace g3d
} // namespace nw4r

// Defined inline via NW4R_G3D_RTTI_DECL_BASE(G3dObj) in g3d_obj.h
