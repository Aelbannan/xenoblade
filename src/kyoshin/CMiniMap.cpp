// Auto-scaffolded catalog TU for kyoshin/CMiniMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMiniMap.hpp"
void func_8011C434(void* self) { reinterpret_cast<CMenuMiniMap2*>((char*)self - 0x58)->~CMenuMiniMap2(); }

void func_80117734(){}

void CMMTex::OnFileEvent() {}

void __ct__CMiniMap(){}

CMiniMap::~CMiniMap() {}

void func_80117C30(){}

void func_80118058(){}

void CMiniMap::OnFileEvent() {}

void func_80118854(){}

void func_8011B05C(){}


void CMenuMiniMap2::Init() {}

void CMenuMiniMap2::Term() {}

void CMenuMiniMap2::Move() {}

void CMenuMiniMap2::cbRenderBefore() {}

void __ct__8011C1B8(){}

extern u32 lbl_eu_80663F20;
extern u32 lbl_eu_80663FB0;
bool func_8011C2E8() {
    u32 v = lbl_eu_80663FB0;
    return ((-v) | v) >> 31;
}

void func_8011C2FC(){}

void func_8011C400()
{
    if (lbl_eu_80663FB0 != 0)
    {
        *((unsigned char*)lbl_eu_80663FB0 + 0x54) = 1;
    }
}

extern "C" void func_8011C43C(void* self) { reinterpret_cast<CMenuMiniMap2*>((char*)self - 0x5c)->cbRenderBefore(); }

extern "C" void func_8011C444(void* self) { reinterpret_cast<CMenuMiniMap2*>((char*)self - 0x5c)->~CMenuMiniMap2(); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" float lbl_eu_806670A0;
extern "C" float lbl_eu_80661E48;
extern "C" float lbl_eu_806670CC;
extern "C" float lbl_eu_80663FB4;

extern "C" void sinit_8011C418() {
    lbl_eu_80663FB4 = lbl_eu_806670CC * (lbl_eu_806670A0 * lbl_eu_80661E48);
}
