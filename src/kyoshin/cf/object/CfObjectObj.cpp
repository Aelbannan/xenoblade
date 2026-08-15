// cf::CfObjectObj - reconstructed functions for kyoshin/cf/object/CfObjectObj
// Functions are kept in retail .text order so hexdiff's offset-based symbol
// fallback maps the unmangled retail names (__ct__cf_CfObjectObj,
// __dt__800BFA14, func_800BFAB0, func_800BFBF4) onto the mangled decomp ones.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectObj.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfResObjImpl.hpp"

// CfObjectPc.hpp (read-only) currently fails to compile due to an internal
// virtual-override mismatch (CActorParam_UnkVirtualFunc86 return type); only
// the incomplete type is needed here (func_800BFC68 pointer arithmetic).
namespace cf {
class CfObjectPc;
}

// us-800c03d4  - constructor: base CfObjectMove ctor, retail vtable restore
// (novtable scheme), zero the helper field, then guard the CfResObjImpl
// resource allocation behind a __dynamic_cast check (the cast succeeds only
// when the object already carries the resource impl, in which case the
// 0x1C-byte allocation is skipped).
cf::CfObjectObj::CfObjectObj() {
    *(void**)this = (void*)lbl_eu_80529B4C;
    field_71C = 0;
    if (__dynamic_cast(this, 0, &lbl_eu_80661D18, &lbl_eu_80661D20, 0) == 0) {
        // The ctor returns the object in r3, so assigning it back keeps `res`
        // in volatile r3 for the mSubObjB0 store (no callee-saved slot).
        void* res = mtl::MemManager::allocate(0x1c, func_80061FFC());
        if (res != 0) {
            res = (void*)__ct__cf_CfResObjImpl(res, this);
        }
        this->mSubObjB0 = res;
    }
}

// us-800c045c  - deleting destructor: restore the retail vtable, run the slot
// +0x68 virtual cleanup; MWCC auto-generates the base CfObjectMove dtor call
// (flag 0) and the null-check + delete-flag wrapper (CfObjectNpc pattern).
cf::CfObjectObj::~CfObjectObj() {
    *(void**)this = (void*)lbl_eu_80529B4C;
    this->CfObject_UnkVirtualFunc6();
}

// us-800c04d0  - simple bool/int return after a virtual init call.
int cf::CfObjectObj::func_800BFA88() {
    // Qualified call forces direct (non-virtual) dispatch to the CfObjectModel impl.
    this->CfObjectModel::CfObject_UnkVirtualFunc2();
    return 1;
}

// us-800c04f4  - retail is a 4-byte blr stub.
void cf::CfObjectObj::func_800BFAAC() {}

// us-800c04f8  - init helper: two vtable init calls, clears two flags,
// dispatches two helper-ids through the 0x144 virtual, then sets bit-derived
// flags on arg4/arg5. Returns 1.
int cf::CfObjectObj::func_800BFAB0(u32 arg4, u32 arg5) {
    // 0x17c then 0x178 virtual init calls on the model.
    this->CfObjectModel_UnkVirtualFunc2();
    this->CfObjectModel_UnkVirtualFunc1();
    // Clear helper-id / dispatch flags (0x90, 0x94 live in locked base padding).
    *(u32*)((u8*)this + 0x90) = 0;
    *(u32*)((u8*)this + 0x94) = 0;
    // vtable+0x144 helper dispatch; retail re-reads the vtable on every call
    // via genuine virtual dispatch (r12 double-load, args-then-dispatch). A
    // plain vtable deref makes MWCC use a temp base reg, so cast through a
    // virtual-dispatch view struct instead (see CfFn144View in the header).
    cf::CfFn144View* v144 = reinterpret_cast<cf::CfFn144View*>(this);
    v144->fn144(1, arg4);
    v144->fn144(0, arg5);
    // Mark the sub-object active (b0 is base-class void*; +8 is a u16 field).
    *(u16*)((u8*)this->mSubObjB0 + 8) = 1;
    if (arg4 != 0)
        *((u32*)((u8*)this + 0x6C)) |= 0x20;
    if (arg5 != 0)
        *((u32*)((u8*)this + 0x6C)) |= 0x10;
    return 1;
}

// us-800c05d8  - run the model update, then consume a pending helper id.
void cf::CfObjectObj::func_800BFB90() {
    // Qualified call forces direct (non-virtual) dispatch to CfObjectMove's
    // member impl (retail uses a plain bl, not a vtable dispatch).
    this->CfObjectMove::CfObject_UnkVirtualFunc4();
    if (this->CfObject_UnkVirtualFunc9() != 0 && this->field_71C != 0) {
        func_800CA580(this->mSubObj38, this->field_71C);
        this->field_71C = 0;
    }
}

// us-800c063c  - dispatch a helper id, or store it if not dispatchable.
// Retail branches straight to the epilogue when mSubObj38 is null (no store
// then); the id is stored only when mSubObj38 != 0 but the virtual check fails.
void func_800BFBF4(cf::CfObjectObj* self, u16 id) {
    if (self->mSubObj38 != 0) {
        if (self->CfObject_UnkVirtualFunc9() != 0) {
            func_800CA580(self->mSubObj38, id);
            self->field_71C = 0;
        } else {
            self->field_71C = id;
        }
    }
}

// us-800c06b0  - find the containing CfObjectPc for a CfObjectMove subobject.
// The CfObjectMove subobject lives at +0x3E9C inside CfObjectPc, so the reverse
// offset recovers the outer object. Only valid when the flag is set.
cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove) {
    if (objMove != 0 && (objMove->unk64 & 0x2) != 0) {
        // Dead null-guard: return objMove as a raw address (never taken here) so
        // MWCC emits `beqlr cr1` without any class-base adjustment.
        if (objMove != 0)
            return (cf::CfObjectPc*)((u8*)objMove - 0x3E9C);
        return (cf::CfObjectPc*)((u8*)objMove);
    }
    return 0;
}
