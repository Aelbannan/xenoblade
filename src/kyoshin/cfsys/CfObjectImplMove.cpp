// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplMove
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cfsys/CfObjectImplMove.hpp"

void func_800CA948(CfObjectImplMoveObj* self) {
    // Init helper: -1 at 0x20, 0 at 0x1c, shared float constant at 0x24.
    self->field_0x20 = 0xFFFFFFFF;
    self->field_0x1C = 0;
    self->field_0x24 = lbl_eu_80666C60;
}

void func_800CA964(){}

void func_800CAA44(){}

void func_800CAB00(CfObjectImplMoveObj* self) {
    // Dispatch the sub-object's event id to the callback source (this+0xc).
    unsigned int id = self->mSubObj->field_0x98;
    if (id == 0) {
        return;
    }
    void* src = self;
    if (self != 0) {
        src = &self->field_0x0C;
    }
    func_80482AB8(id, src);
}

void func_800CAB2C(void) {}

void func_800CAB30(){}

void func_800CB21C(CfObjectImplMoveObj* self, u32 id) {
    // Tail-call dispatch: each case issues an embedded sub-object virtual call
    // (vtable 0x204) and returns; retail compiles each case to a bctr tail call.
    // Case order is retail's body emission order (MWCC emits in source order).
    switch (id) {
    case 9:
        self->field_0x18->sub.vfn204(0x1f, 0, -1, 0, 0);
        break;
    case 6:
        self->field_0x18->sub.vfn204(0x28, 0, -1, 0, 0);
        break;
    case 12:
        self->field_0x18->sub.vfn204(0x29, 0, -1, 0, 0);
        break;
    case 11:
        self->field_0x18->sub.vfn204(0x2a, 0, -1, 0, 0);
        break;
    case 0xce:
        self->field_0x18->sub.vfn204(0x1c, 0, -1, 0, 0);
        break;
    case 0xcf:
        self->field_0x18->sub.vfn204(0x1e, 0, -1, 0, 0);
        break;
    case 0xd0:
        self->field_0x18->sub.vfn204(0x1e, 0, -1, 0, 0);
        break;
    case 0xd1:
        self->field_0x18->sub.vfn204(0x1d, 0, -1, 0, 0);
        break;
    case 15:
        self->field_0x18->sub.vfn204(0x1, 0, -1, 0, 0);
        break;
    case 0x111:
        self->field_0x18->sub.vfn204(0x20, 0, -1, 0, 0);
        break;
    case 0x112:
        self->field_0x18->sub.vfn204(0x21, 0, -1, 0, 0);
        break;
    }
}

void func_800CB454(){}

void func_800CB94C(){}

void func_800CB9AC(CfObjectImplMoveObj* self, u32 id) {
    // Presentation/event gate: when the 0x04000000 event flag is set, skip all
    // move dispatch. Otherwise map the incoming id to an embedded sub-object
    // command (vtable 0x20c). Case order is retail's body emission order.
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04(0x04000000)) {
        return;
    }
    switch (id) {
    case 9:
        self->field_0x18->sub.vfn20C(0x1f);
        break;
    case 6:
        self->field_0x18->sub.vfn20C(0x28);
        break;
    case 12:
        self->field_0x18->sub.vfn20C(0x29);
        break;
    case 11:
        self->field_0x18->sub.vfn20C(0x2a);
        break;
    case 0xce:
        // Event 0xce is gated on the +0x8 sub-object's handler accepting it;
        // only then issue command 0x1c.
        if (func_80148778((u8*)self->field_0x18 + 8, 0xce) == 0) {
            self->field_0x18->sub.vfn20C(0x1c);
        }
        break;
    case 0xcf:
        self->field_0x18->sub.vfn20C(0x1e);
        break;
    case 0xd0:
        self->field_0x18->sub.vfn20C(0x1e);
        break;
    case 0xd1:
        self->field_0x18->sub.vfn20C(0x1d);
        break;
    case 15:
        self->field_0x18->sub.vfn20C(0x1);
        break;
    case 0x111:
        self->field_0x18->sub.vfn20C(0x20);
        break;
    case 0x112:
        self->field_0x18->sub.vfn20C(0x21);
        break;
    case 0x113:
    case 0x114:
    case 0x115:
    case 0x116:
        self->field_0x18->sub.vfn20C(0x26);
        break;
    }
}

void func_800CBBD8(){}

void func_800CC01C(void) {}

void func_800CC020(){}

void func_800CC5DC(){}

void func_800CC638(CfObjectImplMoveObj* self, u32 id, CfMoveParam* param) {
    // Move-request driver: gate on the driver's id, then resolve the move
    // object from the request mode, reposition it for the 0x65/0x66 event
    // pair, and finally attach partner objects / fire the count effect.
    // The driver/mode are re-read from memory at each use (retail reloads
    // 0x14(r28) / 0xa(r29)), so no locals are cached across calls.
    if (id != self->mSubObj->mSomeId) {
        return;
    }
    CfMoveEventObj* evt = (CfMoveEventObj*)self->mSubObj->field_0x98;
    if (evt != nullptr && evt->f17() > lbl_eu_80666C88) {
        return;
    }
    u8 mode = param->field_0xA;    // scratch r3 (born first, dies before calls)
    u8 type7;                       // saved r31 (1st declared local)
    CfMoveEffObj* obj = nullptr;    // saved r30 (2nd)
    u8 bit7;                        // saved r27 (3rd)
    type7 = param->field_0xE & 0x7f;   // low 7 bits: effect type
    bit7 = (param->field_0xE >> 7) & 1;  // bit 7: flag passed to vfn194
    if (mode == 0) {
        obj = (CfMoveEffObj*)func_8008187C__Q22cf13CfGameManagerFv(param->field_0xC);
    } else if (mode == 2) {
        obj = (CfMoveEffObj*)func_800817BC__Q22cf13CfGameManagerFv(param->field_0xC, 0);
    } else if (mode == 1) {
        obj = (CfMoveEffObj*)self->mSubObj->vfn220(param->field_0xC);
    }
    if (obj == nullptr) {
        return;
    }
    if (param->field_0xA == 0 && param->field_0xC >= 0x65 && param->field_0xC <= 0x66) {
        // Reposition: pull the driver's position objects and hand the merged
        // vector to the move object's slot 0x9C. The posObj fields are copied
        // as words (float -> u32 bit-cast), matching retail stfs/lwz/stw.
        CfMovePosObj* posObj = (CfMovePosObj*)self->mSubObj->vfn128();
        CfMoveVec3f* acVec = (CfMoveVec3f*)self->mSubObj->vfnAC();
        CfMoveVec3f pos = *acVec;
        if (posObj != nullptr) {
            CfMoveFloatBits b0, b1, b2;
            b0.f = posObj->field_0xC;
            b1.f = posObj->field_0x1C;
            b2.f = posObj->field_0x2C;
            *(u32*)&pos.x = b0.w;
            *(u32*)&pos.y = b1.w;
            *(u32*)&pos.z = b2.w;
        }
        obj->vfn9C(&pos);
        return;
    }
    // Common path: attach to the driver, pass the flag, then dispatch by
    // effect type (bitmask {1,4,5,6} or {0x85,0x86} reaches the actor id).
    func_800ACF78(obj, self->mSubObj, 0);
    obj->vfn194(bit7);
    if (self->mSubObj->field_0x98 != 0) {
        func_800ACEF8(obj, (u8*)self->mSubObj->field_0x98 + 0x304);
    }
    u32 t = type7 + 0xff;
    int cond = 0;
    if ((u8)t <= 5 && ((1u << t) & 0x39) != 0) {
        cond = 1;
    }
    int keep = 1;
    if (cond == 0 && (u8)(type7 + 0x7b) > 1) {
        keep = 0;
    }
    if (keep != 0) {
        func_800ACFD8(obj, func_800B708C((int)self->field_0x18->vf298()->field_0x4));
    }
    if (type7 == 2 || type7 == 5 || type7 == 0x85) {
        if (self->mSubObj->field_0x6F8 != 0) {
            func_800ACF78(obj, nullptr,
                (u32)((CfMoveDriverSubObj*)self->mSubObj->field_0x6F8)->vfnA8());
        }
    } else if (type7 == 3 || type7 == 6 || type7 == 0x86) {
        if (self->mSubObj->field_0x6FC != 0) {
            func_800ACF78(obj, nullptr,
                (u32)((CfMoveDriverSubObj*)self->mSubObj->field_0x6FC)->vfnA8());
        }
    }
    if (param->field_0xF != 0 && obj != 0 && obj->field_0x94 != 0) {
        // u8 count -> f32 via the shared 0x43300000 magic double.
        CfMoveF64Conv conv;
        conv.w[1] = (u32)param->field_0xF;
        conv.w[0] = 0x43300000;
        func_804E3CDC(obj->field_0x94, (f32)(conv.d - lbl_eu_80666C90), lbl_eu_80666C64);
    }
}

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
