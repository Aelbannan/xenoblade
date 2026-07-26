// Translation unit: monolib/src/coli/code_804A6C60
// Collision subsystem — method dispatch table handlers and utilities.
// Each stub below is a catalog placeholder pending decompilation.

#include <harness_catalog.h>
#include <monolib/coli/coli_types.hpp>

using ml::coli::CColiObject;

extern "C" void func_804A6C60(void) {}

extern "C" void func_804A6D90(void) {}

extern "C" void func_804A6DC0(void) {}

extern "C" void func_804A6E20(void) {}

extern "C" void func_804A70F8(void) {}

extern "C" void func_804A732C(void) {}

extern "C" void func_804A73A0(void) {}

extern "C" void func_804A763C(void) {}

extern "C" void func_804A7834(void) {}

extern "C" void func_804A7878(void) {}

extern "C" void func_804A790C() {}

extern "C" void func_804A79B4() {}

extern "C" void func_804A7ACC() {}

extern "C" void func_804A7BDC() {}

extern "C" void func_804A7C64() {}

extern "C" void func_804A7D1C() {}

extern "C" void func_804A7E18() {}

extern "C" void func_804A7E7C() {}

extern u32 lbl_eu_80665938;
extern "C" u32 func_804A7EC8(u32 unused, u32 val) { lbl_eu_80665938 = val; return unused; }

extern "C" void func_804A7ED0() {}

extern "C" void func_804A7F0C() {}

extern "C" void func_804A7F50() {}

// No-op handler. Used as a default stub in collision dispatch tables for
// method slots that require no action (e.g. optional callbacks).
extern "C" void func_804A822C() {}

extern "C" void func_804A8230() {}

extern "C" void func_804A8850() {}

extern "C" void func_804A8BE0() {}

extern "C" void func_804A8CB0() {}

extern "C" void func_804A98C4() {}

extern "C" void func_804AA4F4() {}

extern "C" void func_804AA504() {}

extern "C" void func_804AA870() {}

extern "C" void func_804AAA98() {}

extern "C" void func_804AAD90() {}

extern "C" void func_804AB524() {}

extern "C" void func_804AB538() {}

extern "C" void func_804ABA08() {}

extern "C" void func_804ABA68() {}

extern "C" void func_804ABAF0() {}

extern "C" void func_804ABBF4() {}

extern "C" void func_804ABCA4() {}

extern "C" void func_804ABD0C() {}

extern "C" void func_804ABDD4() {}

extern "C" void func_804ABE84() {}

extern "C" void func_804ABF08() {}

extern "C" void func_804AC020() {}

extern "C" void func_804AC198() {}

extern "C" void func_804AC3B0() {}

extern "C" void func_804AC4E4() {}

extern "C" void func_804AC4F4() {}

extern "C" void func_804AC57C() {}

extern "C" void func_804AC5D8() {}

// Default false-return handler. Used as a placeholder in collision method
// dispatch tables for boolean-returning virtual method slots that are
// not overridden (always returns 0 / false). The self pointer is unused.
extern "C" int func_804AC61C(CColiObject* /*self*/) { return 0; }

extern "C" void func_804AC624() {}

extern "C" void func_804AC9F4() {}

extern "C" void func_804ACD9C() {}

extern "C" void func_804AD1E0() {}

extern "C" void func_804AD410() {}

extern "C" void func_804AD8FC() {}

extern "C" void func_804ADD3C() {}

extern "C" void func_804AE0D0() {}

extern "C" void func_804AE11C() {}

extern "C" void func_804AE388() {}

extern "C" void func_804AE9A4() {}

extern "C" void func_804AEC8C() {}

extern "C" void func_804AF07C() {}

extern "C" void func_804AF09C() {}

extern "C" void func_804AF2F0() {}

extern "C" void func_804AF310() {}

extern "C" void func_804AF32C() {}

extern "C" void func_804AF808() {}

extern "C" void func_804AF98C() {}

extern "C" void func_804AFA08() {}

extern "C" void func_804AFB28() {}

extern "C" void func_804B028C() {}

extern "C" void func_804B06FC() {}

extern "C" void func_804B073C() {}

extern "C" void func_804B077C() {}

extern "C" void func_804B07F0() {}

extern "C" void func_804B0818() {}

extern "C" void func_804B08A0() {}

extern "C" void func_804B0924() {}

extern "C" void __dt__804B095C() {}

extern "C" void func_804B09C8() {}

extern "C" void func_804B0A6C(void) {}

extern "C" void func_804B0A74(void) {}

extern "C" void func_804B0A7C() {}

extern "C" void func_804B0AD4() {}

extern "C" void func_804B0B0C() {}

extern "C" void func_804B0B54() {}

extern "C" void func_804B0C0C() {}

extern "C" void func_804B0CE8() {}

extern "C" void func_804B0DF4() {}

extern "C" void func_804B0EA0() {}

extern "C" void func_804B102C() {}

extern "C" void func_804B1130() {}

extern "C" void func_804B1164() {}

extern "C" void func_804B192C() {}

extern "C" void func_804B19CC() {}

extern "C" void func_804B1AD8() {}

extern "C" void func_804B1BDC() {}

extern "C" void func_804B1C9C() {}

extern "C" void func_804B1DC0(void* self, int arg) {
    int* flags = (int*)((char*)self + 0xa8);
    if (arg != 0) {
        *flags &= ~0x100;
    } else {
        *flags = (*flags | 0x100) & ~0x0E;
    }
}

extern "C" void func_804B1DEC() {}

extern "C" void func_804B204C() {}

extern "C" void func_804B21A8() {}

extern "C" void func_804B236C() {}

extern "C" void func_804B24A4() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_804B2524() {}
