// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/hbm/HBMCommon
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void HBMAllocMem__FUl(){}

void HBMFreeMem__FPv(){}

void HBMCreate(){}

void HBMDelete(){}

void HBMInit(){}

void HBMCalc(){}

void HBMDraw(){}

void HBMSetAdjustFlag(){}

void HBMCreateSound(const void* data, void* ptr, unsigned long size){
    extern void getInstance__Q210homebutton10HomeButtonFv(void);
    extern void InitAxSound__10homebuttonFPCvPvUl(const void*, void*, unsigned long);
    getInstance__Q210homebutton10HomeButtonFv();
    InitAxSound__10homebuttonFPCvPvUl(data, ptr, size);
}

void HBMDeleteSound(void){}

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
