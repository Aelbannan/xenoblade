// CScnMaruShadowNw4r -- scene shadow rendering (nw4r framework)
//
// Translation unit contains:
//   - CScnMaruShadowNw4r constructor/destructor
//   - Shadow property accessors and configuration helpers
//   - Static initializer registered in .ctors
//
// FULL_MATCH functions:
//   func_8048EA9C -- returns 1 (capability check / stub)
//   sinit_8048EAA4 -- static initializer (empty, .ctors registration)

#include <harness_catalog.h>


// us-80491238: __ct__CScnMaruShadowNw4r (constructor, 0x60 bytes)
void __ct__CScnMaruShadowNw4r(){}

// us-80491298: __dt__4CScnFvMaruShadowNw4r (destructor, 0x40 bytes)
extern "C" void __dt__4CScnFvMaruShadowNw4r() {}

// us-804912d8: func_8048D264 (0x29C bytes)
void func_8048D264(){}

// us-80491574: func_8048D500 (0x658 bytes)
void func_8048D500(){}

// us-80491bcc: func_8048DB58 (0x18 bytes)
void func_8048DB58(){}

// us-80491be4: func_8048DB70 (0x1A8 bytes)
void func_8048DB70(){}

// us-80491d8c: func_8048DD18 (0x5E0 bytes)
void func_8048DD18(){}

// us-8049236c: func_8048E2F8 (0x384 bytes)
void func_8048E2F8(){}

// us-804926f0: func_8048E67C (0x20 bytes)
float lbl_eu_8066A994;
float lbl_eu_8066A9CC;

void func_8048E67C(void* obj, int condition) {
    if (condition == 0) {
        *(float*)((char*)obj + 0x1c) = lbl_eu_8066A9CC;
    } else {
        *(float*)((char*)obj + 0x1c) = lbl_eu_8066A994;
    }
}

// us-80492710: func_8048E69C (0x39C bytes)
void func_8048E69C(){}

// us-80492aac: func_8048EA38 (0x8 bytes) -- stores float to sda21 global
extern "C" void func_8048EA38(float v){
    extern float lbl_eu_80663964;
    lbl_eu_80663964 = v;
}

// us-80492ab4: func_8048EA40 (0x8 bytes) -- loads float from sda21 global
float func_8048EA40()
{
    extern float lbl_eu_80663964;
    return lbl_eu_80663964;
}

// us-80492abc: func_8048EA48 (0x2C bytes) -- sets/clears flag 0x80 in halfword + stores float
void func_8048EA48(){}

// us-80492ae8: func_8048EA74 (0x28 bytes) -- sets/clears flag 0x100 in halfword
void func_8048EA74(){}

// --- FULL_MATCH functions ---

// func_8048EA9C -- capability check stub, always returns true.
// Retail: li r3, 1; blr  (8 bytes)
extern "C" int func_8048EA9C() { return 1; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---

// sinit_8048EAA4 -- static initializer registered in .ctors.
// Empty body; used as constructor hook for the TU.
// Retail: blr  (4 bytes)
extern "C" void sinit_8048EAA4() {}
