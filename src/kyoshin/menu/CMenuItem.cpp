// Auto-scaffolded catalog TU for kyoshin/menu/CMenuItem
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuItem.hpp"
void __ct__CMenuItem(){}

extern f32 lbl_eu_80664258;
extern "C" unsigned long func_80167A18(void) { return *(unsigned long*)(&lbl_eu_80664258) != 0; }

void func_80167A2C(){}

void func_80167C30(void* self) { reinterpret_cast<CMenuItem*>((char*)self - 0x58)->cbRenderBefore(); }

void func_80167C38(void* self) { reinterpret_cast<CMenuItem*>((char*)self - 0x58)->~CMenuItem(); }
