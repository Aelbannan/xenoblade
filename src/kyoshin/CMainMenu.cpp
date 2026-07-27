// Auto-scaffolded catalog TU for kyoshin/CMainMenu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
void __dt__9CMainMenuFv();
u32 func_800FEDF8(void) {
    extern u32 lbl_eu_80663F18;
    return lbl_eu_80663F18;
}









void func_800FEF20(){}

void func_800FEF4C(){}

void __ct__800FF300(void* self){}

void CMainMenu::cbRenderBefore() {}

void func_800FF6BC(){}

void func_800FF738(){}

void CMainMenu::func_800FF778() {}

void func_800FF8B0(){}

void func_800FF914(void){}

void func_800FF920(){}

void func_80100E14(){}

void func_801010B8(){}

void func_801018F4(){}

void func_80101A88(){}

void func_80101BF8(){}

void OnFileEvent__9CMainMenuFP10CEventFile(void* self) { ((void(*)(void*))__ct__800FF300)((char*)self - 0x58); }

void func_80102008(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x58); }

void func_80102010(void* self) { ((void(*)(void*))cbRenderBefore__9CMainMenuFv)((char*)self - 0x5c); }

extern "C" void func_80102018(void* self) { ((void(*)(void*))__dt__9CMainMenuFv)((char*)self - 0x5c); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

class IUICf;
template<> void CTTask<IUICf>::Move() {}
template<> void CTTask<IUICf>::Draw() {}
