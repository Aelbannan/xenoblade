#pragma once

// TU-private imports for kyoshin/cf/chain/CChainEffect.cpp (moved out of the public
// CChainEffect.hpp, which keeps the classes).

#include "kyoshin/cf/chain/CChainEffect.hpp"
// Cross-TU C-ABI imports. These retail symbols are emitted unmangled, so they
// are declared with C linkage (matches the C-linkage import blocks used across
// the codebase). getInstance is a plain global C++ function whose retail
// symbol already carries the MWCC `__Fv` mangle, so it is left as-is.
extern "C" {
void setTargetObj_(void* obj, void* target);
void setChildB59__(void* obj, s8 val);
void removeManagedObject(void* self, void* target);
struct CChainManager* getReslistBE8(void);
void* CollObjHasFlag64Bit5(void* param);
extern f32 lbl_eu_80668C50;
}
// Plain global C++ callee (retail symbol getInstance__Fv).
void* getInstance(void);
