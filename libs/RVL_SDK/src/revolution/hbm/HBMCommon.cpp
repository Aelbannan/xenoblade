// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/hbm/HBMCommon
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void HBMAllocMem__FUl() {}

extern "C" void HBMFreeMem__FPv() {}

extern "C" void HBMCreate() {}

extern "C" void HBMDelete() {}

extern "C" void HBMInit() {}

extern "C" void HBMCalc() {}

extern "C" void HBMDraw() {}

extern "C" void HBMSetAdjustFlag() {}

extern "C" void HBMCreateSound(const void* data, void* ptr, unsigned long size) {
    extern void getInstance__Q210homebutton10HomeButtonFv(void);
    extern void InitAxSound__10homebuttonFPCvPvUl(const void*, void*, unsigned long);
    getInstance__Q210homebutton10HomeButtonFv();
    InitAxSound__10homebuttonFPCvPvUl(data, ptr, size);
}

extern "C" void HBMDeleteSound(void) {}

namespace homebutton {
class HomeButton {
public:
    static HomeButton* getInstance();
    void update_sound();
};
}

extern "C" void HBMUpdateSound() {
    homebutton::HomeButton::getInstance()->update_sound();
}
