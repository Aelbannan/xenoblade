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

// Cast-only SI for the minimap sub-object virtual calls (slots 0x2C/0x38)
struct MiniMapIf {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void vf2C(void* a, u32 b);
    virtual void _v030(); virtual void _v034(); virtual void vf38(void* a);
};
extern "C" void func_8011C2FC(void) {
    void* g = (void*)lbl_eu_80663FB0;
    if (!g) return;
    ((u8*)g)[0x8d4] = 3;
    ((u8*)g)[0x7c] = 3;
    ((u8*)g)[0x7d] = 0;
    if (*(void**)((u8*)g + 0x70)) {
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf2C(*(void**)((u8*)g + 0x78), 0);
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf2C(*(void**)((u8*)g + 0x74), 1);
        ((MiniMapIf*)*(void**)((u8*)g + 0x70))->vf38(0);
    }
    ((u8*)g)[0xa8] = 3;
    ((u8*)g)[0xa9] = 0;
    if (*(void**)((u8*)g + 0x9c)) {
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf2C(*(void**)((u8*)g + 0xa4), 0);
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf2C(*(void**)((u8*)g + 0xa0), 1);
        ((MiniMapIf*)*(void**)((u8*)g + 0x9c))->vf38(0);
    }
}

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
