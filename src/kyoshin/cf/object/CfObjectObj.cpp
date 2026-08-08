// cf::CfObjectObj - reconstructed functions for kyoshin/cf/object/CfObjectObj

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectObj.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfResObjImpl.hpp"

// Imported from CfObjectImplObj TU; C-linkage (retail symbol is unmangled
// func_800CA580). Takes the resource-object handle and a sub-id.
extern "C" void func_800CA580(void* self, u16 id);

// Target 1: us-800c04d0  - simple bool/int return after a virtual init call.
int cf::CfObjectObj::func_800BFA88() {
    // Qualified call forces direct (non-virtual) dispatch to the CfObjectModel impl.
    this->CfObjectModel::CfObject_UnkVirtualFunc2();
    return 1;
}

// Target 2: us-800c06b0  - find the containing CfObjectPc for a CfObjectMove subobject.
// The CfObjectMove subobject lives at +0x3E9C inside CfObjectPc, so the reverse
// offset recovers the outer object. Only valid when the flag is set.
cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove) {
    if (objMove == 0) {
        return 0;
    }
    // Redundant inner null re-check is CSE'd into cr1 (retail's beqlr cr1).
    if (objMove != 0 && (objMove->unk64 & 0x2) != 0) {
        return (cf::CfObjectPc*)((u8*)objMove - 0x3E9C);
    }
    return 0;
}

// Target 3: us-800c05d8  - run the model update, then consume a pending helper id.
void cf::CfObjectObj::func_800BFB90() {
    this->CfObject_UnkVirtualFunc4();
    if (this->CfObject_UnkVirtualFunc9() != 0 && this->field_71C != 0) {
        func_800CA580(this->mSubObj38, this->field_71C);
        this->field_71C = 0;
    }
}

// Target 4: us-800c045c  - deleting destructor: run cleanup, MWCC then emits the
// base dtor + operator delete.
cf::CfObjectObj::~CfObjectObj() {
    this->CfObject_UnkVirtualFunc6();
}

// Target 5: us-800c063c  - dispatch a helper id, or store it if not dispatchable.
extern "C" void func_800BFBF4(cf::CfObjectObj* self, u16 id) {
    if (self->mSubObj38 != 0 && self->CfObject_UnkVirtualFunc9() != 0) {
        func_800CA580(self->mSubObj38, id);
        self->field_71C = 0;
    } else {
        self->field_71C = id;
    }
}