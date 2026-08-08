#include "monolib/device.hpp"
#include <revolution/SC.h>

CDeviceSC* CDeviceSC::spInstance;

// Retail SDA reloc for the TU singleton (MWCC_REFERENCE §1a).
extern "C" CDeviceSC* lbl_eu_80665640;


CDeviceSC::CDeviceSC(const char* pName, CWorkThread* pParent) :
CDeviceBase(pName, pParent, MAX_CHILD),
mAspectRatio(SC_ASPECT_STD),
mEuRgb60Mode(SC_EURGB_50_HZ),
mLanguage(SC_LANG_JP),
mProgMode(SC_INTERLACED),
mSoundMode(SC_SND_STEREO),
unk1CD(0){
    spInstance = this;
    SCInit();
}

CDeviceSC::~CDeviceSC(){
    spInstance = nullptr;
}

CDeviceSC* CDeviceSC::getInstance(){
    return spInstance;
}

bool CDeviceSC::isWideAspectRatio(){
    return spInstance->mAspectRatio == SC_ASPECT_WIDE;
}

bool CDeviceSC::isSoundModeMono(){
    return spInstance->mSoundMode == SC_SND_MONO;
}

u8 CDeviceSC::getLanguage(){
    if(lbl_eu_80665640 == nullptr){
        return SCGetLanguage();
    }
    return lbl_eu_80665640->mLanguage;
}

bool CDeviceSC::isInitialized(){
    return spInstance->isRunning();
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
