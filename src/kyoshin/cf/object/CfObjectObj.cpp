// cf::CfObjectObj - reconstructed functions for kyoshin/cf/object/CfObjectObj
// Functions are kept in retail .text order so hexdiff's offset-based symbol
// fallback maps the unmangled retail names (__ct__cf_CfObjectObj,
// __dt__800BFA14, ObjObj_ReleaseModels_FAB0, ObjObj_SetMoveId_FBF4) onto the mangled decomp ones.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectObj.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfResObjImpl.hpp"

// Local overlay: base CfObjectModel keeps mSubObjB0 as void*; this TU owns a
// CfResObjImpl at +0xB0 (same scheme as CfObjectPoint typing mSubObj38).
struct ObjB0View {
    u8 pad[0xB0];
    cf::CfResObjImpl* mSubObjB0;
};

// CfObjectPc.hpp (read-only) currently fails to compile due to an internal
// virtual-override mismatch (CActorParam_getCurrencyBalance return type); only
// the incomplete type is needed here (getCfObjectPc pointer arithmetic).
namespace cf {
class CfObjectPc;
}

// us-800c03d4  - forced-name constructor (retail symbol is the unmangled
// free function __ct__cf_CfObjectObj, not a mangled member ctor): run the base
// CfObjectMove ctor via its literal mangled name, restore the retail vtable
// (novtable scheme), zero the helper field, then guard the CfResObjImpl
// resource allocation behind a __dynamic_cast check (the cast succeeds only
// when the object already carries the resource impl, in which case the
// 0x1C-byte allocation is skipped).
cf::CfObjectObj* __ct__cf_CfObjectObj(cf::CfObjectObj* ths) {
    __ct__Q22cf12CfObjectMoveFv(ths);
    // Manual vptr store (novtable); typed as u32* to match lbl array.
    *reinterpret_cast<u32**>(ths) = reinterpret_cast<u32*>(lbl_eu_80529B4C);
    ths->field_71C = 0;
    if (__dynamic_cast(ths, 0, &lbl_eu_80661D18, &lbl_eu_80661D20, 0) == 0) {
        // The ctor returns the object in r3, so assigning it back keeps `res`
        // in volatile r3 for the mSubObjB0 store (no callee-saved slot).
        cf::CfResObjImpl* res =
            (cf::CfResObjImpl*)mtl::MemManager::allocate(0x1c, CfRes_getAllocHandle());
        if (res != 0) {
            res = __ct__cf_CfResObjImpl(res, ths);
        }
        ((ObjB0View*)ths)->mSubObjB0 = res;
    }
    return ths;
}

// us-800c045c  - deleting destructor (retail forced-name __dt__800BFA14):
// null-check, restore the retail vtable, run the slot +0x68 virtual cleanup,
// run the base CfObjectMove dtor with flag 0, then free the object when the
// delete flag is positive and return self. Written as a free function with
// the literal retail name (CTaskGameEff __dt__80044BB0 / CTagProcessor
// __dt__8012596C pattern): MWCC emits the name verbatim as a real .text
// FUNC symbol, which the acceptance certifier requires (an ABS alias on a
// member dtor is invisible to its symbol scan).
cf::CfObjectObj* __dt__800BFA14(cf::CfObjectObj* ths, int deleteFlag) {
    if (ths != 0) {
        *reinterpret_cast<u32**>(ths) = reinterpret_cast<u32*>(lbl_eu_80529B4C);
        ths->CfObject_releaseMoveTargets();
        __dt__Q22cf12CfObjectMoveFv(ths, 0);
        if (deleteFlag > 0) {
            __dl__FPv(ths);
        }
    }
    return ths;
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
int cf::CfObjectObj::ObjObj_ReleaseModels_FAB0(u32 arg4, u32 arg5) {
    // 0x17c then 0x178 virtual init calls on the model.
    this->CfObjectModel_releaseModelList();
    this->CfObjectModel_releaseModelSub();
    // Clear helper-id / dispatch flags (0x90, 0x94 live in locked base padding).
    cf::CfObjectMove90View* flags90 = (cf::CfObjectMove90View*)this;
    flags90->field_90 = 0;
    flags90->field_94 = 0;
    // vtable+0x144 helper dispatch (CfObject_setAnimSlotEntry, retail
    // 2-arg slot); genuine virtual dispatch reproduces the r12
    // double-load with args-then-dispatch ordering.
    this->CfObject_setAnimSlotEntry(1, arg4);
    this->CfObject_setAnimSlotEntry(0, arg5);
    // Mark the sub-object active (+0xB0 is CfResObjImpl*; field_08 is u16).
    ((ObjB0View*)this)->mSubObjB0->field_08 = 1;
    cf::CfObjectMoveFlags6C* flags6C = (cf::CfObjectMoveFlags6C*)this;
    if (arg4 != 0)
        flags6C->field_6C |= 0x20;
    if (arg5 != 0)
        flags6C->field_6C |= 0x10;
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
void ObjObj_SetMoveId_FBF4(cf::CfObjectObj* ths, u16 id) {
    if (ths->mSubObj38 != 0) {
        if (ths->CfObject_isMoveActiveNow() != 0) {
            ObjImplSyncSlotState(ths->mSubObj38, id);
            ths->field_71C = 0;
        } else {
            ths->field_71C = id;
        }
    }
}

// us-800c06b0  - find the containing CfObjectPc for a CfObjectMove subobject.
// The CfObjectMove subobject lives at +0x3E9C inside CfObjectPc, so the reverse
// offset recovers the outer object. Only valid when the flag is set.
cf::CfObjectPc* getCfObjectPc(cf::CfObjectMove* objMove) {
    if (objMove != 0 && (objMove->unk64 & 0x2) != 0) {
        // Dead null-guard: return objMove as a raw address (never taken here) so
        // MWCC emits `beqlr cr1` without any class-base adjustment.
        if (objMove != 0)
            return (cf::CfObjectPc*)((u8*)objMove - 0x3E9C);
        return (cf::CfObjectPc*)((u8*)objMove);
    }
    return 0;
}
