// Auto-scaffolded catalog TU for kyoshin/menu/CMenuZeal
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern unsigned long lbl_eu_806642A0;

#include "kyoshin/menu/CMenuZeal.hpp"

extern "C" void __dt__9CMenuZealFv(void*, int);
extern "C" void cbRenderBefore__9CMenuZealFv(void*);

void __ct__CMenuZeal(){}

CMenuZeal::~CMenuZeal() {}

void CMenuZeal::Init() {}

void CMenuZeal::Term() {}

void CMenuZeal::Move() {}

void CMenuZeal::cbRenderBefore() {}

void func_8017FC88(){}

extern "C" u32 func_8017FD44() { return (u32)lbl_eu_806642A0; }

void func_8017FD4C(){}

void func_8017FEF0(){}

void func_8017FF60(void* self) { ((void(*)(void*))__dt__9CMenuZealFv)((char*)self - 0x58); }

void func_8017FF68(void* self) { ((void(*)(void*))cbRenderBefore__9CMenuZealFv)((char*)self - 0x5c); }

extern "C" void func_8017FF70(void* self) { ((void(*)(void*))__dt__9CMenuZealFv)((char*)self - 0x5c); }
