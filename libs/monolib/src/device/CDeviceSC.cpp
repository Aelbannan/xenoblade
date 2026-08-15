#include "monolib/device.hpp"
#include <revolution/SC.h>

// Retail sbss singleton slot lbl_eu_80665640 (8 bytes; word 0 in use) - blob monolibdata1d dissolve
CDeviceSC* lbl_eu_80665640[2];

CDeviceSC::CDeviceSC(const char* pName, CWorkThread* pParent) :
CDeviceBase(pName, pParent, MAX_CHILD),
mAspectRatio(SC_ASPECT_STD),
mEuRgb60Mode(SC_EURGB_50_HZ),
mLanguage(SC_LANG_JP),
mProgMode(SC_INTERLACED),
mSoundMode(SC_SND_STEREO),
unk1CD(0){
    lbl_eu_80665640[0] = this;
    SCInit();
}

CDeviceSC::~CDeviceSC(){
    lbl_eu_80665640[0] = nullptr;
}

CDeviceSC* CDeviceSC::getInstance(){
    return lbl_eu_80665640[0];
}

bool CDeviceSC::isWideAspectRatio(){
    return lbl_eu_80665640[0]->mAspectRatio == SC_ASPECT_WIDE;
}

bool CDeviceSC::isSoundModeMono(){
    return lbl_eu_80665640[0]->mSoundMode == SC_SND_MONO;
}

u8 CDeviceSC::getLanguage(){
    if(lbl_eu_80665640[0] == nullptr){
        return SCGetLanguage();
    }
    return lbl_eu_80665640[0]->mLanguage;
}

bool CDeviceSC::isInitialized(){
    CDeviceSC* pThis = lbl_eu_80665640[0];
    return pThis->isException() == false &&
           (pThis->mState == THREAD_STATE_LOGIN || pThis->mState == THREAD_STATE_RUN);
}

bool CDeviceSC::wkStandbyLogin(){
    if(SCCheckStatus() == SC_STATUS_OK){
        if(CDeviceClock::isInitialized()){
            //Update SC values
            mAspectRatio = SCGetAspectRatio();
            mEuRgb60Mode = SCGetEuRgb60Mode();
            mLanguage = SCGetLanguage();
            mProgMode = SCGetProgressiveMode();
            mSoundMode = SCGetSoundMode();
            return CWorkThread::wkStandbyLogin(); //Call the base method
        }
    }

    return false;
}

bool CDeviceSC::wkStandbyLogout(){
    if(mChildren.empty()){
        if(CDeviceVI::getInstance() == nullptr){
            return CWorkThread::wkStandbyLogout();
        }
    }

    return false;
}

bool func_eu_8044A600(){
    return SCGetLanguage() == 0;
}
