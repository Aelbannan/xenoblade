// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectTbox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectTbox.hpp"
#include "kyoshin/cf/CfResTboxImpl.hpp"  // complete type for delete below
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

namespace cf {}
using namespace cf;

// Global presentation event/presentation bitfield shared across kyoshin.

// us-801fa970 - ctor: the CfObjectObj base ctor runs first (it allocates a
// CfResObjImpl at +0xB0), then the Tbox vtable is stored and the base
// resource is replaced by a CfResTboxImpl.
cf::CfObjectTbox::CfObjectTbox() {
    field_73C = 0;
    // The +0xB0 resource is a CfResTboxImpl (vptr at +0x10, deleting dtor at
    // slot +0x08); delete emits the retail li r4,1 + virtual dispatch, and
    // its null guard is retail's second beq (single cmpwi, two beqs).
    if (mSubObjB0 != 0) {
        delete (CfResTboxImpl*)mSubObjB0;
        mSubObjB0 = 0;
    }
    field_734 = 1;
    field_738 = 0;
    field_720 = 0;
    // The impl ctor returns the object in r3, so assigning it back keeps `res`
    // in volatile r3 for the mSubObjB0 store (no callee-saved slot).
    void* res = mtl::MemManager::allocate(0x1c, CfRes_getAllocHandle());
    if (res != 0) {
        res = (void*)__ct__cf_CfResTboxImpl((cf::CfResTboxImpl*)res, this);
    }
    mSubObjB0 = res;
}

// Member dtor: MWCC emits the vtable restore, the CfObjectObj base dtor
// call and the delete-flag wrapper.
cf::CfObjectTbox::~CfObjectTbox() {
    // Run the CfObject cleanup routine; MWCC then emits the base dtor + deletion.
    CfObject_releaseMoveTargets();
}

void cf::CfObjectTbox::initTbox() {
    // Pull the eventFlags bit 22 (0x400000); pass its inverse as the arg.
    this->setPointEnabled(!(gmGameFlags & 0x400000));
    this->update();
}

struct TboxFwd38View { u8 _pad[0x38]; void* impl38; };
extern "C" void CfObjectTbox_ForwardToImpl(u8* self) {
    extern void tboxSyncChild(void*);
    void* ptr = ((TboxFwd38View*)self)->impl38;
    if (ptr != 0) tboxSyncChild(ptr);
}
