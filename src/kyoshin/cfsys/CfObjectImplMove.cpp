// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cfsys/CfObjectImplMove.hpp"

void func_800CA948(CfObjectImplMoveObj* self) {
    // Init helper: -1 at 0x20, 0 at 0x1c, shared float constant at 0x24.
    f32 v = lbl_eu_80666C60;
    self->field_0x20 = 0xFFFFFFFF;
    self->field_0x1C = 0;
    self->field_0x24 = v;
}

void func_800CA964(){}

void func_800CAA44(){}

void func_800CAB00(CfObjectImplMoveObj* self) {
    // Dispatch the sub-object's event id to the callback source (this+0xc).
    unsigned int id = self->mSubObj->field_0x98;
    if (id == 0) {
        return;
    }
    void* src = self ? &self->field_0x0C : nullptr;
    func_80482AB8(id, src);
}

void func_800CAB2C(void) {}

void func_800CAB30(){}

void func_800CB21C(){}

void func_800CB454(){}

void func_800CB94C(){}

void func_800CB9AC(){}

void func_800CBBD8(){}

void func_800CC01C(void) {}

void func_800CC020(){}

void func_800CC5DC(){}

void func_800CC638(){}

void func_800CC964(){}

void func_800CD268(){}

void func_800CD460(){}

void func_800BE824(void*, unsigned int);

void CfObjectImplMoveData::func_800CD5C0(unsigned int a, unsigned int b) {
    CfObjectImplMoveSubObj* inner = (CfObjectImplMoveSubObj*)this->mSubObj;
    if (a == inner->mSomeId) {
        func_800BE824(inner, b);
    }
}

void func_800CD5DC(){}

void func_800CE544(){}

void func_800CE6A0(){}

void func_800CE8AC(CfObjectImplMoveObj* self) {
    // Virtual dispatch on the sub-object embedded at +0x3e9c of the actor
    // object (vtable slot 0x4c), then chain the result through
    // func_800B708C (actor id lookup) into func_8016FE34.
    func_8016FE34(func_800B708C(
        (int)self->field_0x18->sub.vfn13()));
}

void func_800CE8E4(){}

void func_800CEA34(){}

void func_800CEB68(){}

void func_800CEBE0(){}

void func_800CED64(){}

void func_800CEE28(){}

void func_800CEE7C(void) {}

void func_800CEE80(){}

void func_800CF064(){}

void func_800CF810(){}

void cf::CfObjectImplMove::func_800CFFA0(unsigned int* param) {
    unsigned int* self = reinterpret_cast<unsigned int*>(this);
    if (param == (unsigned int*)self[7]) {
        param[44] = 0;
        self[7] = 0;
    }
}

void cf::CfObjectImplMove::func_800CFFBC() {
    // Adjusted-this destructor thunk: CfObjectImplMove sits at +0xc inside
    // its containing object; retail adjusts this by -0xc and tail-calls the
    // destructor (1-arg form, no delete flag).
    __dt__Q22cf16CfObjectImplMoveFv(reinterpret_cast<u8*>(this) - 0xc);
}

void cf::CfObjectImplMove::func_800CFFC4() {
    // Adjusted-this destructor thunk: CfObjectImplMove sits at +0x10 inside
    // its containing object; retail adjusts this by -0x10 and tail-calls the
    // destructor (1-arg form, no delete flag).
    __dt__Q22cf16CfObjectImplMoveFv(reinterpret_cast<u8*>(this) - 0x10);
}
