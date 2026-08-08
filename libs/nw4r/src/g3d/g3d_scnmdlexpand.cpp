// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_scnmdlexpand
// Replace stubs with high-level C/C++ during decomp.

#include <nw4r/g3d.h>
#include <nw4r/g3d/g3d_scnmdlexpand.hpp>

// lbl_* are the ONLY symbols allowed with extern "C".

// Concrete type forward for Remove parameter.

namespace nw4r { namespace g3d { class ScnObj; } }

// Target 4: destructor
void __dt__Q34nw4r3g3d12ScnMdlExpandFv(unsigned char* self, int flags) {
    (void)self; (void)flags;
}

// Target 5: Remove
bool Remove__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObj(unsigned char* self, nw4r::g3d::ScnObj* pObj) {
    (void)self; (void)pObj;
    return false;
}

// Remaining stubs
void Construct__Q34nw4r3g3d12ScnMdlExpandFP12MEMAllocatorPUlUlPQ34nw4r3g3d12ScnMdlSimple() {}
void PushBack__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObjUl() {}
void Insert__Q34nw4r3g3d12ScnMdlExpandFUlPQ34nw4r3g3d6ScnObj() {}
void Remove__Q34nw4r3g3d12ScnMdlExpandFUl() {}
void G3dProc__Q34nw4r3g3d12ScnMdlExpandFUlUlPv() {}

namespace nw4r {
namespace g3d {

extern "C" const nw4r::g3d::G3dObj::ResNameDataT<sizeof("ScnMdlExpand")> lbl_eu_8051D7D0 = {sizeof("ScnMdlExpand"), "ScnMdlExpand"};

bool ScnMdlExpand::IsDerivedFrom(G3dObj::TypeObj other) const {
    return other == TypeObj(lbl_eu_8051D7D0) ? true
         : other == TypeObj(lbl_eu_8051D788) ? true
         : other == TypeObj(lbl_eu_8051D768) ? true
         : (other == TypeObj(lbl_eu_8051D640));
}

const G3dObj::TypeObj ScnMdlExpand::GetTypeObj() const {
    return TypeObj(lbl_eu_8051D7D0);
}

const char* ScnMdlExpand::GetTypeName() const {
    return GetTypeObj().GetTypeName();
}


} // namespace g3d
} // namespace nw4r
