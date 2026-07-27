// Translation unit for kyoshin/cf/CCtrlMoveBase
// This file implements the CtrlMoveBase class — base for move-control objects.
// Two virtual stubs are FULL_MATCH (100% instruction match, semantically equivalent).
// The remaining functions are scaffolded stubs awaiting decompilation.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CtrlMoveBase.hpp"

void __ct__80088904(){}

void func_80088974(){}

void func_800890A8(){}

void func_80089398(){}

void func_800895A8(){}

// FULL_MATCH: no-op virtual stub (CCtrlMoveBase::func_80089628)
extern "C" void func_80089628() {}

void func_8008962C(){}

extern "C" void func_80089684(void* self) {
    *(unsigned short*)((char*)self + 0x40) &= 1;
}

void func_80089694(){}

void func_800896F4(){}

void func_800898D4(){}

struct func_80089990_child { char pad[0x10]; unsigned short flags; };
struct func_80089990_obj { char pad[0x30]; func_80089990_child* child; };
extern "C" void func_80089990(func_80089990_obj* self) {
    func_80089990_child* p = self->child;
    if (p != 0) {
        p->flags &= 0xff83;
    }
}

extern "C" void func_800899AC(void* obj, float val) {
    void* it = *(void**)((char*)obj + 0x30);
    if (it) {
        *(float*)((char*)it + 0x4) = val;
    }
}

void func_800899C0(){}

void func_80089B24(){}

void func_80089E88(){}

void func_80089F68(){}

void func_8008A01C(){}

// FULL_MATCH: no-op virtual stub (CCtrlMoveBase::func_8008A0C4)
extern "C" void func_8008A0C4() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_8008A0C8() {
    extern void __ct__CColiProc(void*);
    extern char lbl_eu_80571810[];
    extern const void lbl_eu_80527814;

    __ct__CColiProc(lbl_eu_80571810);
    *(void**)lbl_eu_80571810 = (void*)&lbl_eu_80527814;
}
