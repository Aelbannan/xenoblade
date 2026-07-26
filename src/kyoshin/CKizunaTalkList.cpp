// Auto-scaffolded catalog TU for kyoshin/CKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CKizunaTalkList.hpp"
#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_8027355C(CKizunaTalkList* self) {
    return self->mNeedsRebuild;
}

extern "C" void __ct__CKizunaTalkList() {}

extern "C" void __dt__15CKizunaTalkListFv() {}

extern "C" void func_802732F4() {}

extern "C" void func_8027336C() {}

extern "C" void func_8027340C() {}

extern "C" void func_8027346C() {}

extern "C" void func_80273518() {}


extern "C" void func_80273564() {}

extern "C" void func_802735F0() {}

extern "C" void func_80273654() {}

extern "C" void func_80273710() {}

extern "C" void func_802737E0() {}

extern "C" void func_8027387C() {}

extern "C" void func_80273938() {}

extern "C" void func_80273984() {}

extern "C" void func_802739D8() {}

extern "C" void func_80273A24() {}

extern "C" void func_80273A70() {}

extern "C" void func_80273AD0() {}

extern "C" void func_80273B30() {}

extern "C" void func_802740E4() {}

extern "C" void func_802741B0() {}

extern "C" void OnFileEvent__15CKizunaTalkListFP10CEventFile() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_80274458() {}

extern "C" void* func_80273040(void* self, u32 r4) {
    // Use signed compare to match retail cmpwi
    if ((s32)r4 >= 0x100) return 0;
    return (u8*)self + r4 * 0x14;
}
