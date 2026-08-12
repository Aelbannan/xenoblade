// Auto-scaffolded catalog TU for kyoshin/cf/CSuddenCommu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"

namespace cf {}
using namespace cf;

cf::CSuddenCommu::CSuddenCommu() {}

void func_801BA1DC(){}

extern "C" void func_801BA250(void* self) { *(u32*)((u8*)self + 0x20) = 0; }

void func_801BA25C(){}

unsigned long func_801BA2C8(void* self) {
    unsigned long v = *(unsigned long*)((char*)self + 0x14);
    return !!v;
}

void func_801BA2DC(){}

__declspec(noinline) void func_801BA490(CSuddenCommu* self) {}

__declspec(noinline) void func_801BA978(CSuddenCommu* self) {}

void func_801BAB94(){}

void func_801BAD24(){}

void func_801BADE4(){}

void func_801BB464(){}

void func_801BB818() {}

void func_801BB81C(){}

void func_801BB91C(){}

// Voice-node gate: if no voice node is registered for the id at +0x10,
// record state 4 at +0x14 (flag dispatch in func_801BB9DC picks it up).
void func_801BB998(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        self->field_14 = 4;
    }
}

// Flag dispatch: reads the flag word at +0x24. If any of bits 5/6/7
// (0xE0) are set, dispatch on bit 5 (0x20) -> set flag 0x4 and call
// func_801BA490, else bit 6 (0x40) -> func_801BA490, else func_801BA978.
void func_801BB9DC(CSuddenCommu* self) {
    u32 flags = self->field_24;
    if ((flags & 0xE0) == 0) return;
    if (flags & 0x20) {
        self->field_24 |= 0x4;
        func_801BA490(self);
    } else if (flags & 0x40) {
        func_801BA490(self);
    } else {
        func_801BA978(self);
    }
}

void func_801BBA14(CSuddenCommu* self) {
    func_801BC6A4(self, self->field_6, 1);
    self->field_14 = 6;
}

// Same voice-node gate as func_801BB998, but the recorded state depends on
// +0x0A and bit 2 of the +0x24 flag word (0x4).
void func_801BBA50(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        if (self->field_A == 1 || (self->field_24 & 0x4) == 0) {
            self->field_14 = 9;
        } else {
            self->field_14 = 7;
        }
    }
}

void func_801BBAB8(CSuddenCommu* self) {
    func_801BC6A4(self, self->field_8, 3);
    self->field_14 = 8;
}

void func_801BBAF4(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        self->field_14 = 9;
    }
}

void func_801BBB38(CSuddenCommu* self) {
    func_801BC6A4(self, self->field_4, 5);
    self->field_14 = 10;
}

void func_801BBB74(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        self->field_14 = 0xD;
    }
}

void func_801BBBB8(CSuddenCommu* self) {
    func_801BC6A4(self, self->field_4, 2);
    self->field_14 = 12;
}

void func_801BBBF4(CSuddenCommu* self) {
    bool missing = !func_802A3748(self->field_10);
    if (missing) {
        self->field_14 = 0xD;
    }
}

void func_801BBC38(){}

void func_801BBCBC(){}

void func_801BC474(){}

void func_801BC590(){}

__declspec(noinline) void func_801BC6A4(CSuddenCommu* self, int val, int num) {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_801BC86C(){}
