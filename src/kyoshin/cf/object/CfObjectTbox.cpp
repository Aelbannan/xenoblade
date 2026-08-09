// Auto-scaffolded catalog TU for kyoshin/cf/object/CfObjectTbox
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectTbox.hpp"

namespace cf {}
using namespace cf;

// Global presentation event/presentation bitfield shared across kyoshin.
extern u32 lbl_eu_80663E24;

cf::CfObjectTbox::CfObjectTbox() : CfObjectObj() {
    field_73C = 0;
    field_734 = 1;
    field_738 = 0;
    field_720 = 0;
}

cf::CfObjectTbox::~CfObjectTbox() {
    // Run the CfObject cleanup routine; MWCC then emits the base dtor + deletion.
    CfObject_UnkVirtualFunc6();
}

void cf::CfObjectTbox::func_801F8DD0() {
    // Pull the eventFlags bit 22 (0x400000); pass its inverse as the arg.
    this->CfObject_UnkVirtualFunc66(!(lbl_eu_80663E24 & 0x400000));
    this->func_800BFB90();
}

extern "C" void func_801F8E20(u8* self) {
    extern void func_802753F8(void*);
    void* ptr = *(void**)((char*)self + 0x38);
    if (ptr != 0) func_802753F8(ptr);
}
