// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/hbm/HBMCommon
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <revolution/MEM.h>
#include <revolution/OS.h>
#include <revolution/HBM/HBMTypes.h>
#include <revolution/version.h>

#include <nw4hbm/lyt/lyt_layout.h>

RVL_LIB_VERSION(HBM, "Feb 24 2010", "16:19:07", "0x4302_145");

namespace homebutton {
class HomeButton {
public:
    static void createInstance(const HBMDataInfo* pDataInfo);
    static HomeButton* getInstance();
    static void deleteInstance();
    void create();
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
void ShutdownAxSound();
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

void HBMCreate(const HBMDataInfo* pDataInfo) {
    s32 allocType;

    if (pDataInfo->pAllocator != NULL) {
        allocType = 0;
    } else {
        allocType = 2;
        if (pDataInfo->mem != NULL) {
            allocType = 1;
        }
    }

    if (allocType == 1) {
        MEMInitAllocatorForExpHeap(
            &sAllocator, MEMCreateExpHeapEx(pDataInfo->mem, pDataInfo->memSize, 0),
            0x20);
        spAllocator = &sAllocator;
    }

    if (pDataInfo->pAllocator != NULL) {
        allocType = 0;
    } else {
        allocType = 2;
        if (pDataInfo->mem != NULL) {
            allocType = 1;
        }
    }

    switch (allocType) {
    case 0:
        nw4hbm::lyt::Layout::SetAllocator(pDataInfo->pAllocator);
        spAllocator = pDataInfo->pAllocator;
        break;
    case 1:
        nw4hbm::lyt::Layout::SetAllocator(spAllocator);
        break;
    case 2:
        spAllocator = nw4hbm::lyt::Layout::GetAllocator();
        break;
    }

    homebutton::HomeButton::createInstance(pDataInfo);
    homebutton::HomeButton::getInstance()->create();
}

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

void HBMDeleteSound(void){
    homebutton::ShutdownAxSound();
}

}

extern "C" void HBMUpdateSound() {
    homebutton::HomeButton::getInstance()->update_sound();
}
