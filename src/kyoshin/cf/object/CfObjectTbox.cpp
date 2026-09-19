// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectTbox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectTbox.hpp"
#include "kyoshin/cf/CfResTboxImpl.hpp"  // complete type for delete below
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

namespace cf {}
using namespace cf;

// Global presentation event/presentation bitfield shared across kyoshin.

// us-801fa970 - free-function ctor (retail mangled name). Calls the flat
// Obj base ctor rather than a C++ base initializer (which would emit the
// mangled __ct__Q22cf11CfObjectObjFv reloc retail does not carry).
extern "C" cf::CfObjectTbox* __ct__Q22cf12CfObjectTboxFv(cf::CfObjectTbox* self) {
    __ct__cf_CfObjectObj(self);
    *(void**)self = (void*)lbl_eu_80534FB8;
    self->field_73C = 0;
    // The +0xB0 resource is a CfResTboxImpl (vptr at +0x10, deleting dtor at
    // slot +0x08); delete emits the retail li r4,1 + virtual dispatch, and
    // its null guard is retail's second beq (single cmpwi, two beqs).
    if (self->mSubObjB0 != 0) {
        delete (CfResTboxImpl*)self->mSubObjB0;
        self->mSubObjB0 = 0;
    }
    self->field_734 = 1;
    self->field_738 = 0;
    self->field_720 = 0;
    // The impl ctor returns the object in r3, so assigning it back keeps `res`
    // in volatile r3 for the mSubObjB0 store (no callee-saved slot).
    void* res = mtl::MemManager::allocate(0x1c, CfRes_getAllocHandle());
    if (res != 0) {
        res = (void*)__ct__cf_CfResTboxImpl((cf::CfResTboxImpl*)res, self);
    }
    self->mSubObjB0 = res;
    return self;
}

// Member dtor keeps retail codegen (vtable restore + base call shape). The
// synthesised base reloc is mangled __dt__Q22cf11CfObjectObjFv; reloc-map
// equates it to retail __dt__800BFA14.
cf::CfObjectTbox::~CfObjectTbox() {
    // Run the CfObject cleanup routine; MWCC then emits the base dtor + deletion.
    CfObject_releaseMoveTargets();
}

void cf::CfObjectTbox::initTbox() {
    // Pull the eventFlags bit 22 (0x400000); pass its inverse as the arg.
    this->setPointEnabled(!(lbl_eu_80663E24 & 0x400000));
    this->update();
}

extern "C" void CfObjectTbox_ForwardToImpl(u8* self) {
    extern void func_802753F8(void*);
    void* ptr = *(void**)((char*)self + 0x38);
    if (ptr != 0) func_802753F8(ptr);
}
