// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectTbox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectTbox.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

namespace cf {}
using namespace cf;

// Global presentation event/presentation bitfield shared across kyoshin.

// us-801fa970 - constructor: base CfObjectObj ctor, retail vtable restore,
// clear field_73C, release any inherited +0xB0 resource (manual-vtable slot
// 0x8 with delete-flag 1) and null it, init the flag fields, then allocate a
// fresh 0x1C-byte CfResTboxImpl and store it at +0xB0.
cf::CfObjectTbox::CfObjectTbox() : CfObjectObj() {
    *(void**)this = (void*)lbl_eu_80534FB8;
    field_73C = 0;
    // Both null-tests fold onto one compare before the release call.
    if (mSubObjB0 != 0) {
        if (mSubObjB0 != 0) {
            reinterpret_cast<CfTboxSubB0Vt*>(mSubObjB0)->_v008(1);
        }
        mSubObjB0 = 0;
    }
    field_734 = 1;
    field_738 = 0;
    field_720 = 0;
    // The impl ctor returns the object in r3, so assigning it back keeps `res`
    // in volatile r3 for the mSubObjB0 store (no callee-saved slot).
    void* res = mtl::MemManager::allocate(0x1c, func_80061FFC());
    if (res != 0) {
        res = (void*)__ct__cf_CfResTboxImpl((cf::CfResTboxImpl*)res, this);
    }
    mSubObjB0 = res;
}

cf::CfObjectTbox::~CfObjectTbox() {
    // Run the CfObject cleanup routine; MWCC then emits the base dtor + deletion.
    CfObject_UnkVirtualFunc6();
}

void cf::CfObjectTbox::initTbox() {
    // Pull the eventFlags bit 22 (0x400000); pass its inverse as the arg.
    this->CfObject_UnkVirtualFunc66(!(lbl_eu_80663E24 & 0x400000));
    this->func_800BFB90();
}

extern "C" void func_801F8E20(u8* self) {
    extern void func_802753F8(void*);
    void* ptr = *(void**)((char*)self + 0x38);
    if (ptr != 0) func_802753F8(ptr);
}