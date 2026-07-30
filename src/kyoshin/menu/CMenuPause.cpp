// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPause
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuPause.hpp"

// forward declarations for scaffold thunk references
extern const char lbl_eu_806647C8[];
void __dt__10CMenuPauseFv(void*);
void cbRenderBefore__10CMenuPauseFv(void*);

CMenuPause::~CMenuPause() {}

void CMenuPause::Init() {}

void CMenuPause::Term() {}

void CMenuPause::Move() {}

void CMenuPause::cbRenderBefore() {}

void __ct__CMenuPause(){}

unsigned long CMenuPause::isInitialized() {
    unsigned long v = *(unsigned long*)(lbl_eu_806647C8);
    return !!v;
}

void func_80252538(){}

void func_80252564(){}

void func_80252628(void* self) { ((void(*)(void*))__dt__10CMenuPauseFv)((char*)self - 0x58); }

void func_80252630(void* self) { ((void(*)(void*))cbRenderBefore__10CMenuPauseFv)((char*)self - 0x5c); }

void func_80252638(void* self) { ((void(*)(void*))__dt__10CMenuPauseFv)((char*)self - 0x5c); }
