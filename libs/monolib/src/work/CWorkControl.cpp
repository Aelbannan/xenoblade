#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "decomp.h"

// sbss singleton lbl_eu_806655D8 (8 bytes; word 0 in use) - blob monolibdata1d dissolve
CWorkControl* lbl_eu_806655D8[2];

#pragma dont_inline on
CWorkControl::CWorkControl(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD){
    lbl_eu_806655D8[0] = this;
    mType = THREAD_CWORKCONTROL;
}
#pragma dont_inline off

extern char lbl_eu_80522688[];

#pragma optimize_for_size on
CWorkControl::~CWorkControl(){
    lbl_eu_806655D8[0] = nullptr;
}
#pragma optimize_for_size off

CWorkControl* CWorkControl::getInstance(){
    return lbl_eu_806655D8[0];
}

void CWorkControl::pause(bool paused){
    getInstance()->wkSetEventChild(paused ? EVT_PAUSE : EVT_UNPAUSE);
}

#pragma optimize_for_size on
bool CWorkControl::hasFlow(){
    if(CWorkFlowWiiMenu::getInstance() != nullptr){
        return true;
    }

    if(CWorkFlowShutdownAll::getInstance() != nullptr){
        return true;
    }

    if(CWorkFlowWiiReset::getInstance() != nullptr){
        return true;
    }

    if(CWorkFlowWiiPowerOff::getInstance() != nullptr){
        return true;
    }

    // Last flow: single getInstance call whose nonzero result is the return
    // (retail emits the subic/subfe bool-idiom on this one call).
    return CWorkFlowSetup::getInstance() != nullptr;
}
#pragma optimize_for_size off

bool CWorkControl::wkStandbyLogin(){
    setFlowSetup();
    return CWorkThread::wkStandbyLogin();
}

#pragma optimize_for_size on
DECOMP_DONT_INLINE bool CWorkControl::setFlowSetup(){
    if(CWorkFlowSetup::getInstance() != nullptr){
        return true;
    }

    const char* name = &lbl_eu_80522688[0x10];
    CWorkControl* pControl = CWorkControl::getInstance();
    CWorkFlowSetup::create(name, pControl);
    return true;
}
#pragma optimize_for_size off

bool CWorkControl::wkStandbyLogout(){
    if(mChildren.empty() && CDevice::getInstance() == nullptr){
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

// Factory: allocate from the work heap, construct in place, register. NOTE:
// retail keeps pParent/name/result in three callee-saved registers (stmw r29)
// and materializes the name before the alloc call; MWCC from high-level C
// uses two (stmw r30) and reuses the name base after the call, leaving the
// function 0x70 vs 0x74 -- open item.
extern char lbl_eu_80522688[];

#pragma optimize_for_size on
CWorkControl* CWorkControl::create(CWorkThread* pParent){
    const char* name = lbl_eu_80522688 + 0x6C;
    CWorkControl* pWorkControl =
        new (CWorkThreadSystem::getWorkMem()) CWorkControl(name, pParent);
    CWorkUtil::entryWork(pWorkControl, pParent, false);
    return pWorkControl;
}
#pragma optimize_for_size off
