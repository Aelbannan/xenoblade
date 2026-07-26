// Auto-scaffolded catalog TU for kyoshin/CPartyState
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_801FD17C(void* self) { return ((u8*)self)[0x30]; }







extern "C" u8 func_801FD184(void* self) { return ((u8*)self)[0x31]; }

extern "C" u8 func_801FD18C(void* self) { return ((u8*)self)[0x57]; }





extern "C" void func_801FD304() {}

extern "C" void func_801FD3D4() {}

extern "C" void func_801FD48C() {}

extern "C" u32 func_801FD580(void* self) {
    s8 val = *(s8*)((u8*)self + 0x4D);
    // Match rlwinm extraction of sign bit followed by xori
    return ((u32)(val >> 31) & 1) ^ 1;
}

extern "C" void func_801FD594() {}

extern "C" void func_801FD5C4() {}

extern "C" u8 func_801FD5F4(void* self) { return ((u8*)self)[0x4E]; }

extern "C" u8 func_801FD5FC(void* self) { return ((u8*)self)[0x4C]; }

extern "C" void func_801FD604() {}

extern "C" void func_801FD6F0() {}

extern "C" void func_801FD76C() {}

extern "C" void func_801FD7CC() {}

extern "C" void func_801FD848() {}

extern "C" void func_801FD8A0() {}

extern "C" void func_801FD8F8() {}

extern "C" void func_801FDA7C() {}

extern "C" void func_801FE0C8() {}

extern "C" void func_801FE154() {}

extern "C" void func_801FE20C() {}

extern "C" void func_801FE39C() {}

extern "C" void OnFileEvent__11CPartyStateFP10CEventFile() {}
