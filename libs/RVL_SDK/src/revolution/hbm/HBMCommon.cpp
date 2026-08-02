// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/hbm/HBMCommon
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <revolution/MEM.h>
#include <revolution/OS.h>
#include <revolution/HBM/HBMTypes.h>
#include <revolution/version.h>

RVL_LIB_VERSION(HBM, "Feb 24 2010", "16:19:07", "0x4302_145");

namespace homebutton {
class HomeButton {
public:
    static HomeButton* getInstance();
    static void deleteInstance();
    void init();
    void draw();
    void update_sound();
    void setAdjustFlag(int flag);
    void calc(const HBMControllerData* pController);
    HBMSelectBtnNum getSelectBtnNum();
    const HBMDataInfo* getHBMDataInfo() const {
        return mpHBInfo;
    }

private:
    int mSequence;               // at 0x0
    const HBMDataInfo* mpHBInfo; // at 0x4
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

void HBMDelete() {
    homebutton::HomeButton* pBtn = homebutton::HomeButton::getInstance();
    const HBMDataInfo* pInfo = pBtn->getHBMDataInfo();
    homebutton::HomeButton::deleteInstance();

    /* 1 = the library owns the heap and should tear it down; 0/2 = app
       allocator or no embedded memory, nothing to destroy */
    s32 result;
    if (pInfo->pAllocator != NULL)
        result = 0;
    else {
        result = 2;
        if (pInfo->mem != NULL)
            result = 1;
    }

    if (result == 1) {
        MEMDestroyExpHeap(spAllocator->heap);
    }
}

void HBMInit() {
    OSRegisterVersion(__HBMVersion);
    homebutton::HomeButton::getInstance()->init();
}

HBMSelectBtnNum HBMCalc(const HBMControllerData* pController) {
    homebutton::HomeButton::getInstance()->calc(pController);
    return homebutton::HomeButton::getInstance()->getSelectBtnNum();
}

void HBMDraw() {
    homebutton::HomeButton::getInstance()->draw();
}

void HBMSetAdjustFlag(int flag) {
    homebutton::HomeButton::getInstance()->setAdjustFlag(flag);
}

void HBMCreateSound(const void* data, void* ptr, unsigned long size){
    homebutton::HomeButton::getInstance();
    homebutton::InitAxSound(data, ptr, size);
}

void HBMDeleteSound(void){}

}

extern "C" void HBMUpdateSound() {
    homebutton::HomeButton::getInstance()->update_sound();
}
