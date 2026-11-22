#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(G3dObj);

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



bool G3dObj::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(TYPE_NAME);
}



const char* G3dObj::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}



const G3dObj::TypeObj G3dObj::GetTypeObj() const {
    return TypeObj(TYPE_NAME);
}


} // namespace g3d
} // namespace nw4r
