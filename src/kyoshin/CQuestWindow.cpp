// Auto-scaffolded catalog TU for kyoshin/CQuestWindow
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

u32 func_80122448(void) {
    extern u32 lbl_eu_80663FD0;
    return lbl_eu_80663FD0;
}

// (lbl_eu_80663FD0 != 0) - retail lwz sda21; subic; subfe
extern "C" bool func_80122450() { extern u32 lbl_eu_80663FD0; return lbl_eu_80663FD0 != 0; }

int lbl_eu_80663D1C;

int func_80042864() {
    return lbl_eu_80663D1C != 0;
}

extern u32 lbl_eu_80663F24;
void func_80122460(){ lbl_eu_80663F24 = 0; }

void func_80122654(){}

void func_801226C8(){}

void func_8012278C(void* self){}

extern "C" void cbRenderBefore__12CQuestWindowFv() {}

void func_80122B2C(){}

void func_80122BB0(){}

void func_80122C08(){}

void func_80122EF8(){}

void func_801231C4(){}

void func_80124270(){}

// retail: lfs f1,0(r4); lfs f0,4(r4); stfs f1,0x4c(r3); stfs f0,0x50(r3)
extern "C" void func_80124288(u8* self, float* src){
    ((float*)(self + 0x4C))[0] = src[0];
    ((float*)(self + 0x4C))[1] = src[1];
}

void func_8012429C(){}

void func_801242FC(){}

void func_8012435C(){}

void __dt__12CQuestWindowFv();
void OnFileEvent__12CQuestWindowFP10CEventFile(void* self) { ((void(*)(void*))func_8012278C)((char*)self - 0x6c); }

void func_801245D4(void* self) { ((void(*)(void*))__dt__12CQuestWindowFv)((char*)self - 0x6c); }

void func_801245DC(void* self) { ((void(*)(void*))cbRenderBefore__12CQuestWindowFv)((char*)self - 0x70); }

extern "C" void func_801245E4(void* self) { ((void(*)(void*))__dt__12CQuestWindowFv)((char*)self - 0x70); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (via harness_catalog.hpp);
// specializations below emit the retail Move/Draw/dtor symbols.
class IUIWindow;
template<> void CTTask<IUIWindow>::Move() {}
template<> void CTTask<IUIWindow>::Draw() {}

extern "C" void func_8012246C() {}

extern "C" void Init__12CQuestWindowFv() {}
extern "C" void Term__12CQuestWindowFv() {}
