#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "decomp.h"

CWorkControl* CWorkControl::spInstance = nullptr;

#pragma dont_inline on
CWorkControl::CWorkControl(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD){
    spInstance = this;
    mType = THREAD_CWORKCONTROL;
}
#pragma dont_inline off

#pragma optimize_for_size on
CWorkControl::~CWorkControl(){
    spInstance = nullptr;
}
#pragma optimize_for_size off

CWorkControl* CWorkControl::getInstance(){
    return spInstance;
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

    CWorkControl* pControl = CWorkControl::getInstance();
    CWorkFlowSetup::create("CWorkFlowSetup", pControl);
    return true;
}
#pragma optimize_for_size off

bool CWorkControl::wkStandbyLogout(){
    if(mChildren.empty() && CDevice::getInstance() == nullptr){
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

#pragma optimize_for_size on
CWorkControl* CWorkControl::create(CWorkThread* pParent){
    CWorkControl* pWorkControl = new (CWorkThreadSystem::getWorkMem()) CWorkControl("CWorkControl", pParent);
    CWorkUtil::entryWork(pWorkControl, pParent, false);
    return pWorkControl;
}
#pragma optimize_for_size off
