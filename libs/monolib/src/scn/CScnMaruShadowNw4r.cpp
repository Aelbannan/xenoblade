// CScnMaruShadowNw4r — scene shadow rendering (nw4r framework)
//
// Translation unit contains:
//   - CScnMaruShadowNw4r constructor/destructor
//   - Shadow property accessors and configuration helpers
//   - Static initializer registered in .ctors
//
// FULL_MATCH functions:
//   func_8048EA9C — returns 1 (capability check / stub)
//   sinit_8048EAA4 — static initializer (empty, .ctors registration)

#include <harness_catalog.h>

// --- LLM-HARNESS stubs (non-matching, address-labeled) ---

// us-80491238: __ct__CScnMaruShadowNw4r (constructor, 0x60 bytes)
// LLM-HARNESS-BEGIN: us-80491238
extern "C" void __ct__CScnMaruShadowNw4r() {}
// LLM-HARNESS-END: us-80491238

// us-80491298: __dt__4CScnFvMaruShadowNw4r (destructor, 0x40 bytes)
// LLM-HARNESS-BEGIN: us-80491298
extern "C" void __dt__4CScnFvMaruShadowNw4r() {}
// LLM-HARNESS-END: us-80491298

// us-804912d8: func_8048D264 (0x29C bytes)
// LLM-HARNESS-BEGIN: us-804912d8
extern "C" void func_8048D264() {}
// LLM-HARNESS-END: us-804912d8

// us-80491574: func_8048D500 (0x658 bytes)
// LLM-HARNESS-BEGIN: us-80491574
extern "C" void func_8048D500() {}
// LLM-HARNESS-END: us-80491574

// us-80491bcc: func_8048DB58 (0x18 bytes)
// LLM-HARNESS-BEGIN: us-80491bcc
extern "C" void func_8048DB58() {}
// LLM-HARNESS-END: us-80491bcc

// us-80491be4: func_8048DB70 (0x1A8 bytes)
// LLM-HARNESS-BEGIN: us-80491be4
extern "C" void func_8048DB70() {}
// LLM-HARNESS-END: us-80491be4

// us-80491d8c: func_8048DD18 (0x5E0 bytes)
// LLM-HARNESS-BEGIN: us-80491d8c
extern "C" void func_8048DD18() {}
// LLM-HARNESS-END: us-80491d8c

// us-8049236c: func_8048E2F8 (0x384 bytes)
// LLM-HARNESS-BEGIN: us-8049236c
extern "C" void func_8048E2F8() {}
// LLM-HARNESS-END: us-8049236c

// us-804926f0: func_8048E67C (0x20 bytes)
// LLM-HARNESS-BEGIN: us-804926f0
extern "C" void func_8048E67C() {}
// LLM-HARNESS-END: us-804926f0

// us-80492710: func_8048E69C (0x39C bytes)
// LLM-HARNESS-BEGIN: us-80492710
extern "C" void func_8048E69C() {}
// LLM-HARNESS-END: us-80492710

// us-80492aac: func_8048EA38 (0x8 bytes) — stores float to sda21 global
// LLM-HARNESS-BEGIN: us-80492aac
extern "C" void func_8048EA38() {}
// LLM-HARNESS-END: us-80492aac

// us-80492ab4: func_8048EA40 (0x8 bytes) — loads float from sda21 global
// LLM-HARNESS-BEGIN: us-80492ab4
extern "C" void func_8048EA40() {}
// LLM-HARNESS-END: us-80492ab4

// us-80492abc: func_8048EA48 (0x2C bytes) — sets/clears flag 0x80 in halfword + stores float
// LLM-HARNESS-BEGIN: us-80492abc
extern "C" void func_8048EA48() {}
// LLM-HARNESS-END: us-80492abc

// us-80492ae8: func_8048EA74 (0x28 bytes) — sets/clears flag 0x100 in halfword
// LLM-HARNESS-BEGIN: us-80492ae8
extern "C" void func_8048EA74() {}
// LLM-HARNESS-END: us-80492ae8

// --- FULL_MATCH functions ---

// func_8048EA9C — capability check stub, always returns true.
// Retail: li r3, 1; blr  (8 bytes)
// LLM-HARNESS-BEGIN: us-80492b10
extern "C" int func_8048EA9C() { return 1; }
// LLM-HARNESS-END: us-80492b10

// --- hard-symbol stubs (scaffold_hard_symbols) ---

// sinit_8048EAA4 — static initializer registered in .ctors.
// Empty body; used as constructor hook for the TU.
// Retail: blr  (4 bytes)
// LLM-HARNESS-BEGIN: us-80492b18
extern "C" void sinit_8048EAA4() {}
// LLM-HARNESS-END: us-80492b18
