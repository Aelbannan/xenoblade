// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/hbm/HBMCommon
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <revolution/MEM.h>
#include <revolution/OS.h>
#include <revolution/version.h>

RVL_LIB_VERSION(HBM, "Feb 24 2010", "16:19:07", "0x4302_145");

namespace homebutton {
class HomeButton {
public:
    static HomeButton* getInstance();
    void init();
    void draw();
    void update_sound();
};
void InitAxSound(const void* data, void* ptr, unsigned long size);
}

MEMAllocator sAllocator;
MEMAllocator* spAllocator = &sAllocator;

void* HBMAllocMem(u32 size) {
    return MEMAllocFromAllocator(spAllocator, size);
}

void HBMFreeMem(void* pBlock) {
    MEMFreeToAllocator(spAllocator, pBlock);
}

extern "C" {

void HBMCreate(){}

void HBMDelete(){}

void HBMInit() {
    OSRegisterVersion(__HBMVersion);
    homebutton::HomeButton::getInstance()->init();
}

void HBMCalc(){}

void HBMDraw() {
    homebutton::HomeButton::getInstance()->draw();
}

void HBMSetAdjustFlag(){}

void HBMCreateSound(const void* data, void* ptr, unsigned long size){
    homebutton::HomeButton::getInstance();
    homebutton::InitAxSound(data, ptr, size);
}

void HBMDeleteSound(void){}

}

extern "C" void HBMUpdateSound() {
    homebutton::HomeButton::getInstance()->update_sound();
}
