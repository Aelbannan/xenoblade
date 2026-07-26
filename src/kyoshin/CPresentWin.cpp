// Auto-scaffolded catalog TU for kyoshin/CPresentWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_8022DB6C(void* self) { return ((u8*)self)[0x30]; }







extern "C" u8 func_8022DB74(void* self) { return ((u8*)self)[0x38]; }



extern "C" void func_8022DD90() {}

extern "C" void func_8022E204() {}

extern "C" void func_8022E254() {}

extern "C" void func_8022E2F8() {}

extern "C" void func_8022E3A8(void) {}

extern "C" void func_8022E3AC() {}

extern "C" u8 func_8022E488(void* self) { return ((u8*)self)[0x31]; }

extern "C" u8 func_8022E490(void* self) { return ((u8*)self)[0x36]; }

extern "C" void func_8022E498() {}

extern "C" u8 func_8022E4FC(void* self) { return ((u8*)self)[0x32]; }

extern "C" u8 func_8022E504(void* self) { return ((u8*)self)[0x33]; }

extern "C" void func_8022E50C() {}

extern "C" void func_8022E558() {}

extern "C" void func_8022E5B0() {}

extern "C" void func_8022E698() {}

extern "C" void func_8022E744() {}

extern "C" void func_8022E7F0() {}

extern "C" u8 func_8022E868(void* self, u32 r4) {
    u8 limit = *(u8*)((u8*)self + 0x41);
    if (r4 >= limit) return 0;
    // Force add r3,r3,r4 by using (u8*)self as base
    return *(u8*)((u8*)self + 0x39 + r4);
}
