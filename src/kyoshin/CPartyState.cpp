// Auto-scaffolded catalog TU for kyoshin/CPartyState
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPartyState.hpp"
#include "kyoshin/CEquipItemBox.hpp"

u8 CPartyState::func_801FD17C() { return field_0x30; }


u8 CPartyState::func_801FD184() { return field_0x31; }

u8 CPartyState::func_801FD18C() { return field_0x57; }


void func_801FD304(){}

void func_801FD3D4(){}

void func_801FD48C(){}

u32 CPartyState::func_801FD580() {
    s8 val = field_0x4D;
    // Match rlwinm extraction of sign bit followed by xori
    return ((u32)(val >> 31) & 1) ^ 1;
}

void func_801FD594(){}

void func_801FD5C4(){}

u8 CPartyState::func_801FD5F4() { return field_0x4E; }

u8 CPartyState::func_801FD5FC() { return field_0x4C; }

void func_801FD604(){}

void func_801FD6F0(){}

// Party-state advance: when the +0x1C gate is set, refresh the shared cursor
// sound state, rebuild the state panel, and reset the visibility flags.
extern "C" void func_801FD8A0(CPartyState* self) {
    if (self->field_0x1C != 0) {
        func_80139198(0);
        func_801FDA7C(self);
        self->field_0x30 = 1;
        self->field_0x28 = 1;
        self->field_0x4C = 0;
    }
}

// Destroys the embedded cursor and scratch-memory sub-objects (reverse
// declaration order); MWCC emits the delete guard from the incoming flag.
CPartyState::~CPartyState() {}

// Intro-animation finish handler: when the +0x24 anim transform reaches the
// target frame, move to state 3, show the cursor, and run the shared tail.
extern "C" void func_801FD76C(CPartyState* self) {
    if (func_80137444(self->mAnimTrans0, lbl_eu_80668218) != 0) {
        self->field_0x2C = 3;
        self->field_0x31 = 1;
        func_801D216C(&self->mCur22, 1);
        func_801FD8F8(self);
    }
}

void func_801FD7CC(){}

void func_801FD848(){}

// Advance the party-state panel out of state 3: set state 4, hide the cursor,
// and play the cancel sound unless the +0x4E flag is set.
extern "C" void func_801FD1BC(CPartyState* self) {
    if (self->field_0x2C == 3) {
        self->field_0x2C = 4;
        self->field_0x31 = 0;
        func_801D2BFC(&self->mCur22, 0xff);
        if (self->field_0x4E == 0) {
            func_80138078(6);
        }
    }
}

// Selection change: wrap the current selection by one step based on the party
// member count, then run the shared refresh and play the cursor sound.
extern "C" void func_801FD220(CPartyState* self) {
    u8 memberCount = code80135FDC_getByte_64077();
    u8 cur = self->field_0x4C;
    s8 s = (s8)cur;
    u8 next = (u8)(memberCount - 1);
    if ((u32)(s - 4) <= 2u) {
        self->field_0x4C = (u8)(cur - 1);
    } else if (s == 3) {
        self->field_0x4C = next;
    }
    func_801FD8F8(self);
    func_80138078(1);
}

void func_801FE0C8(){}

void func_801FE154(){}

void func_801FE20C(){}

void func_801FE39C(){}

void CPartyState::OnFileEvent() {}

extern "C" void func_801FCF5C() {}
extern "C" void func_801FCFF4() {}
extern "C" void func_801FD0A0() {}
extern "C" void func_801FD0F4() {}
extern "C" void func_801FD194() {}
extern "C" void func_801FD290() {}
