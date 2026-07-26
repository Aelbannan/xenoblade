// Auto-scaffolded catalog TU for kyoshin/CMiniMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
extern "C" void __dt__13CMenuMiniMap2Fv(void* self);

extern "C" void func_8011C434(void* self) { ((void(*)(void*))__dt__13CMenuMiniMap2Fv)((char*)self - 0x58); }

extern "C" void __dt__6CMMTexFv() {}

extern "C" void func_80117734() {}

extern "C" void OnFileEvent__6CMMTexFP10CEventFile() {}

extern "C" void __ct__CMiniMap() {}

extern "C" void __dt__8CMiniMapFv() {}

extern "C" void func_80117C30() {}

extern "C" void func_80118058() {}

extern "C" void OnFileEvent__8CMiniMapFP10CEventFile() {}

extern "C" void func_80118854() {}

extern "C" void func_8011B05C() {}


extern "C" void Init__13CMenuMiniMap2Fv() {}

extern "C" void Term__13CMenuMiniMap2Fv() {}

extern "C" void Move__13CMenuMiniMap2Fv() {}

extern "C" void cbRenderBefore__13CMenuMiniMap2Fv(void* self) {}

extern "C" void __ct__8011C1B8() {}

extern u32 lbl_eu_80663F20;
extern u32 lbl_eu_80663FB0;
extern "C" bool func_8011C2E8() {
    u32 v = lbl_eu_80663FB0;
    return ((-v) | v) >> 31;
}

extern "C" void func_8011C2FC() {}

extern "C" void func_8011C400()
{
    if (lbl_eu_80663FB0 != 0)
    {
        *((unsigned char*)lbl_eu_80663FB0 + 0x54) = 1;
    }
}

extern "C" void func_8011C43C(void* self) { ((void(*)(void*))cbRenderBefore__13CMenuMiniMap2Fv)((char*)self - 0x5c); }

extern "C" void func_8011C444(void* self) { ((void(*)(void*))__dt__13CMenuMiniMap2Fv)((char*)self - 0x5c); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" float lbl_eu_806670A0;
extern "C" float lbl_eu_80661E48;
extern "C" float lbl_eu_806670CC;
extern "C" float lbl_eu_80663FB4;

extern "C" void sinit_8011C418() {
    lbl_eu_80663FB4 = lbl_eu_806670CC * (lbl_eu_806670A0 * lbl_eu_80661E48);
}
