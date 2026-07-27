// Auto-scaffolded catalog TU for kyoshin/CPartyState
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPartyState.hpp"

u8 func_801FD17C(CPartyState* self) { return self->field_0x30; }







u8 func_801FD184(CPartyState* self) { return self->field_0x31; }

u8 func_801FD18C(CPartyState* self) { return self->field_0x57; }





void func_801FD304(){}

void func_801FD3D4(){}

void func_801FD48C(){}

u32 func_801FD580(CPartyState* self) {
    s8 val = self->field_0x4D;
    // Match rlwinm extraction of sign bit followed by xori
    return ((u32)(val >> 31) & 1) ^ 1;
}

void func_801FD594(){}

void func_801FD5C4(){}

u8 func_801FD5F4(CPartyState* self) { return self->field_0x4E; }

u8 func_801FD5FC(CPartyState* self) { return self->field_0x4C; }

void func_801FD604(){}

void func_801FD6F0(){}

void func_801FD76C(){}

void func_801FD7CC(){}

void func_801FD848(){}

void func_801FD8A0(){}

void func_801FD8F8(){}

void func_801FDA7C(){}

void func_801FE0C8(){}

void func_801FE154(){}

void func_801FE20C(){}

void func_801FE39C(){}

void CPartyState::OnFileEvent() {}
