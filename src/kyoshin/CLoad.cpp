// Auto-scaffolded catalog TU for kyoshin/CLoad
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_802AE6AC(void* self) { return ((u8*)self)[0x2B]; }







extern "C" u8 func_802AE6B4(void* self) { return ((u8*)self)[0x28]; }

extern "C" u8 func_802AE6BC(void* self) { return ((u8*)self)[0x2C]; }





extern "C" void func_802AE8E0(void* self) {
    if (*(u32*)((u8*)self + 0x1C) != 0) {
        *(u8*)((u8*)self + 0x2B) = 1;
        *(u8*)((u8*)self + 0x28) = 1;
    }
}

void CLoad::OnFileEvent() {}
