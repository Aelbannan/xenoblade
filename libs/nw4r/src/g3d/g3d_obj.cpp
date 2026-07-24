#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(G3dObj);

G3dObj::~G3dObj() {
    Dealloc(mpHeap, this);
}

void G3dObj::Destroy() {
    G3dObj* pParent = GetParent();

    if (pParent != NULL) {
        pParent->G3dProc(G3DPROC_CHILD_DETACHED, 0, this);
    }

    delete this;
}

DECOMP_FORCEACTIVE(g3d_obj_cpp,
                   G3dObj::IsDerivedFrom);

} // namespace g3d
} // namespace nw4r

// LLM-HARNESS-BEGIN: us-803e96bc
extern "C" void IsDerivedFrom__Q34nw4r3g3d6G3dObjCFQ44nw4r3g3d6G3dObj7TypeObj() {}
// LLM-HARNESS-END: us-803e96bc
// LLM-HARNESS-BEGIN: us-803e96d8
extern "C" void GetTypeName__Q34nw4r3g3d6G3dObjCFv() {}
// LLM-HARNESS-END: us-803e96d8
// LLM-HARNESS-BEGIN: us-803e9708
extern "C" void* GetTypeObj__Q34nw4r3g3d6G3dObjCFv(void) { return 0; }
// LLM-HARNESS-END: us-803e9708
