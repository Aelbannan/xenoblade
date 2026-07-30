// Auto-scaffolded catalog TU for kyoshin/CSysWinBuff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// forward declarations for scaffold thunk references
void func_80274A84(void*);
void __dt__11CSysWinBuffFv(void*);
void cbRenderBefore__11CSysWinBuffFv(void*);


u32 getInstance__11CSysWinBuffFv(void) {
    extern u32 lbl_eu_806648E0;
    return lbl_eu_806648E0;
}








void OnFileEvent__11CSysWinBuffFP10CEventFile(void* self) { ((void(*)(void*))func_80274A84)((char*)self - 0x6c); }

void func_80274B08(void* self) { ((void(*)(void*))__dt__11CSysWinBuffFv)((char*)self - 0x6c); }

void func_80274B10(void* self) { ((void(*)(void*))cbRenderBefore__11CSysWinBuffFv)((char*)self - 0x70); }

extern "C" void func_80274B18(void* self) { ((void(*)(void*))__dt__11CSysWinBuffFv)((char*)self - 0x70); }

extern "C" void func_80274B20() {}

extern "C" void func_80274B24() {}
