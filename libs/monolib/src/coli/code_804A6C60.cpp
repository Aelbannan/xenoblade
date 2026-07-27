// Translation unit: monolib/src/coli/code_804A6C60
// Collision subsystem — method dispatch table handlers and utilities.
// Each stub below is a catalog placeholder pending decompilation.

#include <harness_catalog.h>
#include <monolib/coli/coli_types.hpp>

using ml::coli::CColiObject;

void func_804A6C60(void){}

void func_804A6D90(void){}

void func_804A6DC0(void){}

void func_804A6E20(void){}

void func_804A70F8(void){}

void func_804A732C(void){}

void func_804A73A0(void){}

void func_804A763C(void){}

void func_804A7834(void){}

void func_804A7878(void){}

void func_804A790C(){}

void func_804A79B4(){}

void func_804A7ACC(){}

void func_804A7BDC(){}

void func_804A7C64(){}

void func_804A7D1C(){}

void func_804A7E18(){}

void func_804A7E7C(){}

extern u32 lbl_eu_80665938;
u32 func_804A7EC8(u32 unused, u32 val) { lbl_eu_80665938 = val; return unused; }

void func_804A7ED0(){}

void func_804A7F0C(){}

void func_804A7F50(){}

// No-op handler. Used as a default stub in collision dispatch tables for
// method slots that require no action (e.g. optional callbacks).
void func_804A822C() {}

void func_804A8230(){}

void func_804A8850(){}

void func_804A8BE0(){}

void func_804A8CB0(){}

void func_804A98C4(){}

void func_804AA4F4(){}

void func_804AA504(){}

void func_804AA870(){}

void func_804AAA98(){}

void func_804AAD90(){}

void func_804AB524(){}

void func_804AB538(){}

void func_804ABA08(){}

void func_804ABA68(){}

void func_804ABAF0(){}

void func_804ABBF4(){}

void func_804ABCA4(){}

void func_804ABD0C(){}

void func_804ABDD4(){}

void func_804ABE84(){}

void func_804ABF08(){}

void func_804AC020(){}

void func_804AC198(){}

void func_804AC3B0(){}

void func_804AC4E4(){}

void func_804AC4F4(){}

void func_804AC57C(){}

void func_804AC5D8(){}

// Default false-return handler. Used as a placeholder in collision method
// dispatch tables for boolean-returning virtual method slots that are
// not overridden (always returns 0 / false). The self pointer is unused.
int func_804AC61C(CColiObject* /*self*/) { return 0; }

void func_804AC624(){}

void func_804AC9F4(){}

void func_804ACD9C(){}

void func_804AD1E0(){}

void func_804AD410(){}

void func_804AD8FC(){}

void func_804ADD3C(){}

void func_804AE0D0(){}

void func_804AE11C(){}

void func_804AE388(){}

void func_804AE9A4(){}

void func_804AEC8C(){}

void func_804AF07C(){}

void func_804AF09C(){}

void func_804AF2F0(){}

void func_804AF310(){}

void func_804AF32C(){}

void func_804AF808(){}

void func_804AF98C(){}

void func_804AFA08(){}

void func_804AFB28(){}

void func_804B028C(){}

void func_804B06FC(){}

void func_804B073C(){}

void func_804B077C(){}

void func_804B07F0(){}

void func_804B0818(){}

void func_804B08A0(){}

void func_804B0924(){}

void __dt__804B095C(){}

void func_804B09C8(){}

void func_804B0A6C(void){}

void func_804B0A74(void){}

void func_804B0A7C(){}

void func_804B0AD4(){}

void func_804B0B0C(){}

void func_804B0B54(){}

void func_804B0C0C(){}

void func_804B0CE8(){}

void func_804B0DF4(){}

void func_804B0EA0(){}

void func_804B102C(){}

void func_804B1130(){}

void func_804B1164(){}

void func_804B192C(){}

void func_804B19CC(){}

void func_804B1AD8(){}

void func_804B1BDC(){}

void func_804B1C9C(){}

void func_804B1DC0(void* self, int arg) {
    int* flags = (int*)((char*)self + 0xa8);
    if (arg != 0) {
        *flags &= ~0x100;
    } else {
        *flags = (*flags | 0x100) & ~0x0E;
    }
}

void func_804B1DEC(){}

void func_804B204C(){}

void func_804B21A8(){}

void func_804B236C(){}

void func_804B24A4(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804B2524(){}
