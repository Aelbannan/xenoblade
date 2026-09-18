// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplObj
// Replace stubs with high-level C/C++ during decomp.

#include <stdlib.h> // atoi

#include "kyoshin/cfsys/CfObjectImplObj.hpp"

extern "C" void MoveImplRunEffectVirtuals(void* self);

// us-800caf20: per-frame prep - init via MoveImplInitFields, dispatch the
// vtable 0xE4 query and feed its result + the +0x14/+0x18 sub-objects
// into func_8015BB3C.
void ObjImplPrepFrame(cf::CfObjectImplObj* self) {
    MoveImplInitFields(self);
    func_8015BB3C(self->vfE4(), (u8*)self->field_14, (u8*)self->field_18);
}

void* ObjImplGetUnk68(void* self) { return (void*)((u8*)self + 0x68); }

// us-800caf74: dispatch the embedded event id via MoveImplPushEventId, then point
// the +0x368 CPartsChange at the driver sub-object's +0x98 event id.
void ObjImplPushEventResolve(cf::CfObjectImplObj* self) {
    MoveImplPushEventId((CfObjectImplMoveObj*)self);
    CPartsChange_ResolveSlotNames(reinterpret_cast<cf::CPartsChange*>(self->mPartsChange), 0,
                  (PartsChangeIf*)self->field_14->field_0x98);
}

extern "C" void CPartsChange_SyncSlotState(void* a, void* b, int c);
extern "C" void ObjImplSyncSlotState(void* self, void* b) { CPartsChange_SyncSlotState((char*)self + 0x368, b, 1); }

void ObjImplNoop(void) {}

// us-800cafc4: command dispatch - when the driver's +0xC4 id matches, parse
// the param's digit char (modes 0x10/0x11; atoi when '0'-'9') and issue the
// driver's vtable 0x204 command (0x23, 0, parsed-value, 0, 0).
void ObjImplDispatchCmd(cf::CfObjectImplObj* self, u32 id, cf::CfObjCmdParam* param) {
    if (id != self->field_14->field_0xC4) {
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
// delete-flag tail). The member dtor is called DIRECTLY (retail reloc is
// __dt__Q22cf12CPartsChangeFv, no vtable dispatch, no vptr re-store); the
// C++ virtual member-dtor syntax would emit a vtable dispatch + vptr store.
// __declspec(novtable) on the class suppresses the dtor's implicit vptr
// re-store (class is never instantiated; retail dtor has no store).
extern "C" void __dt__Q22cf12CPartsChangeFv(cf::CPartsChange* self, int flags);
cf::CfObjectImplObj::~CfObjectImplObj() {
    __dt__Q22cf12CPartsChangeFv(reinterpret_cast<cf::CPartsChange*>(mPartsChange), -1);
}

void ObjImplEffectThunkC(void* self) { ((void(*)(void*))MoveImplRunEffectVirtuals)((char*)self - 0xc); }

// Adjusted-this destructor thunks (retail: addi + tail-call the dtor).
extern "C" void __dt__Q22cf15CfObjectImplObjFv(cf::CfObjectImplObj* self);

extern "C" void ObjImplDtorThunkC(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplObjFv)((char*)self - 0xc); }

extern "C" void ObjImplDtorThunk10(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplObjFv)((char*)self - 0x10); }
