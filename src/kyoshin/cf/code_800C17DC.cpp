// Auto-scaffolded catalog TU for kyoshin/cf/code_800C17DC
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void func_800C17DC(){}

void func_800C1900(){}

void func_800C1A18(){}

void func_800C1B30(){}

void func_800C1CAC(){}

void func_800C1CC4(){}

void func_800C1DF0(){}

void func_800C1E40(){}

void func_800C1E9C(){}

void func_800C1EB8(){}

// Default no-op callback stored in the callback table at lbl_eu_805739F8.
// Called by func_800C1EB8 when it iterates the callback array and a slot has no
// registered handler.  The empty body corresponds to a single blr instruction.
extern "C" void func_800C1F28(void) {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
struct Unk805739F8 {
    unsigned char pad[0x20];
    unsigned int field20;
    unsigned char field24;
};

extern "C" Unk805739F8 lbl_eu_805739F8;

extern "C" void sinit_800C1F2C() {
    lbl_eu_805739F8.field20 = 0;
    lbl_eu_805739F8.field24 = 0;
}
