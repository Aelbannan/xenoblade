// cf::CfObjectObj - reconstructed functions for kyoshin/cf/object/CfObjectObj
// Functions are kept in retail .text order so hexdiff's offset-based symbol
// fallback stays aligned.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectObj.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfResObjImpl.hpp"

#include "kyoshin/cf/object/CfObjectTbox.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

// us-800c03d4  - constructor: the CfObjectMove base ctor runs first, then
// the compiler stores __vt__Q22cf11CfObjectObj. Zero the helper field, then guard the CfResObjImpl
// resource allocation behind a __dynamic_cast check (the cast succeeds only
// when the object is a CfObjectTbox, whose own ctor allocates its
// resource impl, in which case the 0x1C-byte allocation is skipped).
cf::CfObjectObj::CfObjectObj() {
    field_71C = 0;
    if (dynamic_cast<CfObjectTbox*>(this) == 0) {
        // The ctor returns the object in r3, so assigning it back keeps `res`
        // in volatile r3 for the mSubObjB0 store (no callee-saved slot).
        cf::CfResObjImpl* res =
            (cf::CfResObjImpl*)mtl::MemManager::allocate(0x1c, CfRes_getAllocHandle());
        if (res != 0) {
            res = __ct__cf_CfResObjImpl(res, this);
        }
        mSubObjB0 = res;
    }
}

// us-800c045c  - virtual destructor: the compiler re-stores the vtable,
// run the slot +0x68 virtual cleanup; MWCC appends the CfObjectMove base
// dtor call (flag 0) and the delete-flag wrapper.
cf::CfObjectObj::~CfObjectObj() {
    CfObject_releaseMoveTargets();
}

// us-800c04d0  - simple bool/int return after a virtual init call.
int cf::CfObjectObj::initialize() {
    // Qualified call forces direct (non-virtual) dispatch to the CfObjectModel impl.
    this->CfObjectModel::CfObject_UnkVirtualFunc2();
    return 1;
}

// us-800c04f4  - retail is a 4-byte blr stub.
void cf::CfObjectObj::reset() {}

// us-800c04f8  - init helper: two vtable init calls, clears two flags,
// dispatches two helper-ids through the 0x144 virtual, then sets bit-derived
// flags on arg4/arg5. Returns 1.
int cf::CfObjectObj::setup(u32 arg4, u32 arg5) {
    // 0x17c then 0x178 virtual init calls on the model.
    CfObjectModel_releaseModelList();
    CfObjectModel_releaseModelSub();
    // Clear helper-id / dispatch flags (0x90, 0x94 live in locked base padding).
    cf::CfObjectMove90View* flags90 = (cf::CfObjectMove90View*)this;
    flags90->field_90 = 0;
    flags90->field_94 = 0;
    // vtable+0x144 helper dispatch (CfObject_setAnimSlotEntry, retail
    // 2-arg slot); genuine virtual dispatch reproduces the r12
    // double-load with args-then-dispatch ordering.
    CfObject_setAnimSlotEntry(1, arg4);
    CfObject_setAnimSlotEntry(0, arg5);
    // Mark the sub-object active (+0xB0 is CfResObjImpl*; field_08 is u16).
    static_cast<cf::CfResObjImpl*>(mSubObjB0)->field_08 = 1;
    if (arg4 != 0)
        field_0x6C |= 0x20;
    if (arg5 != 0)
        field_0x6C |= 0x10;
    return 1;
}

// us-800c05d8  - run the model update, then consume a pending helper id.
void cf::CfObjectObj::update() {
    // Qualified call forces direct (non-virtual) dispatch to CfObjectMove's
    // member impl (retail uses a plain bl, not a vtable dispatch).
    this->CfObjectMove::CfObject_UnkVirtualFunc4();
    if (this->CfObject_isMoveActiveNow() != 0 && this->field_71C != 0) {
        ObjImplSyncSlotState(this->mSubObj38, this->field_71C);
        this->field_71C = 0;
    }
}

// us-800c063c  - dispatch a helper id, or store it if not dispatchable.
// Retail branches straight to the epilogue when mSubObj38 is null (no store
// then); the id is stored only when mSubObj38 != 0 but the virtual check fails.
void cf::CfObjectObj::setMoveId(u16 id) {
    if (mSubObj38 != 0) {
        if (CfObject_isMoveActiveNow() != 0) {
            ObjImplSyncSlotState(mSubObj38, id);
            field_71C = 0;
        } else {
            field_71C = id;
        }
    }
}

// us-800c06b0  - recover the owning CfObjectPc from its embedded CfObjectMove
// subobject. Only valid when the subobject's player flag is set.
cf::CfObjectPc* getCfObjectPc(cf::CfObjectMove* objMove) {
    if (objMove != 0 && (objMove->unk64 & 0x2) != 0) {
        return static_cast<cf::CfObjectPc*>(objMove);
    }
    return 0;
}
