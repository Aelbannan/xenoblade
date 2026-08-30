#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

// Retail type-name data (nw4r_data.s lbl_eu_8051D640, .rodata). Defined by
// the shared data object; the RTTI accessors reference it by name.
extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("G3dObj")>
    lbl_eu_8051D640;

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
    // Retail dtor stores the vtable pointer right after the null check; with
    // novtable the store is explicit (lbl_eu_805695B0 = G3dObj vtable).
    *(void**)this = (void*)lbl_eu_805695B0;
    MEMFreeToAllocator(mpHeap, this);
}

void G3dObj::Destroy() {
    G3dObj* pParent = GetParent();

    if (pParent != NULL) {
        pParent->G3dProc(G3DPROC_CHILD_DETACHED, 0, this);
    }

    delete this;
}

extern "C" void IsDerivedFrom__Q34nw4r3g3d6G3dObjCFQ44nw4r3g3d6G3dObj7TypeObj();
extern "C" void __dt__Q34nw4r3g3d6G3dObjFv();
extern "C" void GetTypeObj__Q34nw4r3g3d6G3dObjCFv();
extern "C" void GetTypeName__Q34nw4r3g3d6G3dObjCFv();
extern "C" __declspec(section ".data") void* lbl_eu_805695B0[8] = {
    nullptr, nullptr,
    (void*)&IsDerivedFrom__Q34nw4r3g3d6G3dObjCFQ44nw4r3g3d6G3dObj7TypeObj,
    nullptr,
    (void*)&__dt__Q34nw4r3g3d6G3dObjFv,
    (void*)&GetTypeObj__Q34nw4r3g3d6G3dObjCFv,
    (void*)&GetTypeName__Q34nw4r3g3d6G3dObjCFv,
    nullptr
};

} // namespace g3d
} // namespace nw4r
