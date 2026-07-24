#include "monolib/lib/CLibHbmControl.hpp"
#include "monolib/lib/CLibHbm.hpp"
#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/device.hpp"
#include <revolution/WPAD.h>
#include <cstring>

CLibHbmControl::CLibHbmControl(const char* pName, CWorkThread* pParent) : CProc(pName, pParent, MAX_CHILD),
mHbmPhase(0),
mWaitTimer(0) {
    spInstance = this;
    mType = THREAD_CLIBHBMCONTROL;
    std::memset(&mHBMControllerData, 0, sizeof(HBMControllerData));
}

CLibHbmControl::~CLibHbmControl(){
    spInstance = nullptr;
}

CLibHbmControl* CLibHbmControl::getInstance(){
    return spInstance;
}

bool CLibHbmControl::isActive(){
    CLibHbmControl* hbmControl = spInstance;

    if(hbmControl == nullptr) return false;
    return hbmControl->mHbmPhase == 3;
}

// Advances the HBM (Home Button Menu) state machine each frame.
// Phase 0: load arc file   → 1
// Phase 1: wait for file I/O, countdown timer, then → 2 (or back to 0 on error)
// Phase 2: init HBM system  → 3
// Phase 3: active — polls WPAD controllers, processes HBM button selections
void CLibHbmControl::wkUpdate(){
    switch(mHbmPhase){
        case 0:
            CLibHbm::loadHbmArcFile();
            mWaitTimer = 0xD2;
            mHbmPhase++;
            break;
        case 1:
            if(!CDeviceFileCri::getInstance()->isException()){
                mWaitTimer--;
                if(mWaitTimer <= 0){
                    wkSetEvent(EVT_NONE);
                }

                break;
            }

            wkSetEvent(EVT_NONE);

            if(CLibHbm::checkFlag6()){
                mHbmPhase = 0;
            }else if(CLibHbm::isHbmMemPointerValid()){
                mHbmPhase++;
            }
            break;
        case 2:
            CLibHbm::initHbm();
            mHbmPhase++;
            break;
        case 3:
            for(int i = 0; i < WPAD_MAX_CONTROLLERS; i++){
                CWpadStatus* wpadStatus = CDeviceRemotePad::getWpadStatus(i);

            }
            break;
        default:
            break;
    }

    HBMUpdateSound();

    HBMSelectBtnNum selectBtnNum = HBMCalc(&mHBMControllerData);

    switch(selectBtnNum){
        case HBM_SELECT_HOMEBTN:
            wkSetEvent(EVT_NONE);
            break;
        case HBM_SELECT_BTN1:
            returnToWiiMenu(false);
            break;
        case HBM_SELECT_BTN2:
            resetGame(false);
            break;
        default:
            break;
    }
}

void CLibHbmControl::wkRender(){
    
}

CLibHbmControl* CLibHbmControl::create(){
    CLibHbmControl* hbmControl = CREATE_WORKTHREAD(CLibHbmControl, CLibHbm::getInstance());

    hbmControl->unk1E4 = CDesktop::getView()->mWorkID;

    return hbmControl;
}

bool CLibHbmControl::wkStandbyLogin(){
    return CProc::wkStandbyLogin();
}


bool CLibHbmControl::wkStandbyLogout(){
    return CProc::wkStandbyLogout();
}
