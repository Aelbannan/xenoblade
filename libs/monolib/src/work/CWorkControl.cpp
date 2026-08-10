#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "decomp.h"

DECOMP_FORCEACTIVE(CWorkControl_cpp,
                 "CWorkFlowTvMode",
                 "CWorkFlowSetup",
                 "CWorkFlowShutdownAll",
                 "CWorkFlowWiiMenu",
                 "CWorkFlowWiiReset",
                 "CWorkFlowWiiPowerOff");

CWorkControl* CWorkControl::spInstance = nullptr;

CWorkControl::CWorkControl(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD){
    spInstance = this;
    mType = THREAD_CWORKCONTROL;
}

CWorkControl::~CWorkControl(){
    spInstance = nullptr;
}

CWorkControl* CWorkControl::getInstance(){
    return spInstance;
}

void CWorkControl::pause(bool paused){
    getInstance()->wkSetEventChild(paused ? EVT_PAUSE : EVT_UNPAUSE);
}

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

bool CWorkControl::wkStandbyLogin(){
    setFlowSetup();
    return CWorkThread::wkStandbyLogin();
}

bool CWorkControl::setFlowSetup(){
    if(CWorkFlowSetup::getInstance() != nullptr){
        return true;
    }

    CWorkControl* pControl = CWorkControl::getInstance();
    CWorkFlowSetup::create("CWorkFlowSetup", pControl);
    return true;
}

bool CWorkControl::wkStandbyLogout(){
    if(mChildren.empty() && CDevice::getInstance() == nullptr){
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

CWorkControl* CWorkControl::create(CWorkThread* pParent){
    CWorkControl* pWorkControl = new (CWorkThreadSystem::getWorkMem()) CWorkControl("CWorkControl", pParent);
    CWorkUtil::entryWork(pWorkControl, pParent, false);
    return pWorkControl;
}
