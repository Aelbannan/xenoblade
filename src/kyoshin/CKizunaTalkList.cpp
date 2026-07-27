// Auto-scaffolded catalog TU for kyoshin/CKizunaTalkList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CKizunaTalkList.hpp"
#include "kyoshin/harness_catalog.hpp"

u8 func_8027355C(CKizunaTalkList* self) {
    return self->mNeedsRebuild;
}

void __ct__CKizunaTalkList(){}

void CKizunaTalkList::~CKizunaTalkList() {}

void func_802732F4(){}

void func_8027336C(){}

void func_8027340C(){}

void func_8027346C(){}

void func_80273518(){}


void func_80273564(){}

void func_802735F0(){}

void func_80273654(){}

void func_80273710(){}

void func_802737E0(){}

void func_8027387C(){}

void func_80273938(){}

void func_80273984(){}

void func_802739D8(){}

void func_80273A24(){}

void func_80273A70(){}

void func_80273AD0(){}

void func_80273B30(){}

void func_802740E4(){}

void func_802741B0(){}

void CKizunaTalkList::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80274458(){}

extern "C" void* func_80273040(void* self, u32 r4) {
    // Use signed compare to match retail cmpwi
    if ((s32)r4 >= 0x100) return 0;
    return (u8*)self + r4 * 0x14;
}
