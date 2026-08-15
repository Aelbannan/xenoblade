// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplObj
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cfsys/CfObjectImplObj.hpp"

// us-800caf20: per-frame prep - init via func_800CA948, dispatch the
// vtable 0xE4 query and feed its result + the +0x14/+0x18 sub-objects
// into func_8015BB3C.
void func_800CA4EC(cf::CfObjectImplObj* self) {
    func_800CA948(self);
    func_8015BB3C(self->vfE4(), (u8*)self->field_14, (u8*)self->field_18);
}

void* func_800CA538(void* self) { return (void*)((u8*)self + 0x68); }

// us-800caf74: dispatch the embedded event id via func_800CAB00, then point
// the +0x368 CPartsChange at the driver sub-object's +0x98 event id.
void func_800CA540(cf::CfObjectImplObj* self) {
    func_800CAB00((CfObjectImplMoveObj*)self);
    func_80192E80(&self->mPartsChange, 0,
                  (PartsChangeIf*)self->field_14->field_0x98);
}

extern "C" void func_801930A0(void* a, void* b, int c);
extern "C" void func_800CA580(void* self, void* b) { func_801930A0((char*)self + 0x368, b, 1); }

void func_800CA58C(void) {}

// us-800cafc4: command dispatch - when the driver's +0xC4 id matches, parse
// the param's digit char (modes 0x10/0x11; atoi when '0'-'9') and issue the
// driver's vtable 0x204 command (0x23, 0, parsed-value, 0, 0).
void func_800CA590(cf::CfObjectImplObj* self, u32 id, cf::CfObjCmdParam* param) {
    if (self->field_14->field_0xC4 != id) {
        return;
    }
    u8 mode = param->field_0xA;
    if (mode == 0x10) {
        u8 c = param->field_0x1C;
        int val = -1;
        if ((u8)(c - 0x30) <= 9) {
            val = atoi((const char*)&param->field_0x1C);
        }
        self->field_14->vfn204(0x23, 0, val, 0, 0);
    } else if (mode == 0x11) {
        u8 c = param->field_0x1C;
        int val = -1;
        if ((u8)(c - 0x30) <= 9) {
            val = atoi((const char*)&param->field_0x1C);
        }
        self->field_14->vfn204(0x23, 0, val, 0, 0);
    }
}

// us-800cb0a4: deleting destructor - release the embedded CPartsChange
// (MWCC auto-emits the null-check, the -1 member-dtor flag and the
// delete-flag tail).
cf::CfObjectImplObj::~CfObjectImplObj() {
    mPartsChange.~CPartsChange();
}

void func_800CEE28(void* self);
void func_800CA6CC(void* self) { ((void(*)(void*))func_800CEE28)((char*)self - 0xc); }

// Adjusted-this destructor thunks (retail: addi + tail-call the dtor).
extern "C" void __dt__Q22cf15CfObjectImplObjFv(cf::CfObjectImplObj* self);

extern "C" void func_800CA6D4(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplObjFv)((char*)self - 0xc); }

extern "C" void func_800CA6DC(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplObjFv)((char*)self - 0x10); }
