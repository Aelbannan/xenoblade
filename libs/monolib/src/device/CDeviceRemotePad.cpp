#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include "monolib/lib.hpp"

// Own TU owns CDeviceBase's dtor: retail places it (0x8044A2AC) at the head
// of this unit's slice, right before CDeviceRemotePad's dtor.
CDeviceBase::~CDeviceBase() {}

CDeviceRemotePad::CDeviceRemotePad(const char* pName, CWorkThread* pParent) :
CDeviceBase(pName, pParent, MAX_CHILD),
mPadUpdateFunc(nullptr){
    lbl_eu_80665638 = this;

    for(int i = 0; i < TOTAL_CONTROLLERS; i++){
        mpPads[i] = nullptr;
    }
}

CDeviceRemotePad::~CDeviceRemotePad(){
    lbl_eu_80665638 = nullptr;
}

CDeviceRemotePad* CDeviceRemotePad::getInstance(){
    return lbl_eu_80665638;
}

bool CDeviceRemotePad::isConnected(u32 index){
    return lbl_eu_80665638->mpPads[index]->mConnected;
}

u32 CDeviceRemotePad::getHeldButtonFlags(u32 index){
    return lbl_eu_80665638->mpPads[index]->mHeldButtonFlags;
}

u32 CDeviceRemotePad::getPressedButtonFlags(u32 index){
    return lbl_eu_80665638->mpPads[index]->mPressedButtonFlags;
}

CPad* CDeviceRemotePad::getMainGCPad(){
    return CPadManager::getMainGCPad();
}

//0-3: Wii controllers, 4-7: GC controllers
CPad* CDeviceRemotePad::getPadData(u32 index){
    return CPadManager::getPadData(index >= WPAD_MAX_CONTROLLERS ? PAD_SYSTEM_GC : PAD_SYSTEM_WII,
    index % WPAD_MAX_CONTROLLERS);
}

CWpadStatus* CDeviceRemotePad::getWpadStatus(u32 index){
    return CPadManager::getWpadStatus(index);
}

void CDeviceRemotePad::wkUpdate(){
    mPadUpdateFunc();
}

bool CDeviceRemotePad::wkStandbyLogin(){
    if(CDevice::isColdStartReady()){
        PadUpdateFunc func = CPadManager::initialize(mtl::MemManager::getHandleMEM2());
        mPadUpdateFunc = func;

        for(u32 i = 0; i < TOTAL_CONTROLLERS; i++){
            mpPads[i] = getPadData(i);
        }

        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CDeviceRemotePad::wkStandbyLogout(){
    if(mChildren.empty() && CWorkSystem::getInstance() == nullptr && CLib::getInstance() == nullptr){
        CPadManager::destroy();
        mPadUpdateFunc = nullptr;
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}
