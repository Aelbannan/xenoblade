#include "monolib/device.hpp"
#include "monolib/core.hpp"
#include "monolib/lib.hpp"
#include "monolib/data_vtables.hpp"

// Blob monolibdata1d dissolve: define the CDeviceRemotePad vtable
// (0x8056BBC0), RTTI base list (0x8056BC60) and sbss singleton
// (0x80665638) with the retail names/bytes.
u32 lbl_eu_8056BBC0[0xA0 / 4] = {
    (u32)&lbl_eu_806635E8, 0, (u32)&__dt__16CDeviceRemotePadFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__16CDeviceRemotePadFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__16CDeviceRemotePadFv,
    (u32)&wkStandbyLogout__16CDeviceRemotePadFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
u32 lbl_eu_8056BC60[] = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0,
    (u32)&lbl_eu_806635F0, 0, 0, 0,
};
CDeviceRemotePad* lbl_eu_80665638[2];

// Own TU owns CDeviceBase's dtor: retail places it (0x8044A2AC) at the head
// of this unit's slice, right before CDeviceRemotePad's dtor.

CDeviceRemotePad::CDeviceRemotePad(const char* pName, CWorkThread* pParent) :
CDeviceBase(pName, pParent, MAX_CHILD),
mPadUpdateFunc(nullptr){
    lbl_eu_80665638[0] = this;

    for(int i = 0; i < TOTAL_CONTROLLERS; i++){
        mpPads[i] = nullptr;
    }
}

CDeviceRemotePad::~CDeviceRemotePad(){
    lbl_eu_80665638[0] = nullptr;
}

CDeviceRemotePad* CDeviceRemotePad::getInstance(){
    return lbl_eu_80665638[0];
}

bool CDeviceRemotePad::isConnected(u32 index){
    return lbl_eu_80665638[0]->mpPads[index]->mConnected;
}

u32 CDeviceRemotePad::getHeldButtonFlags(u32 index){
    return lbl_eu_80665638[0]->mpPads[index]->mHeldButtonFlags;
}

u32 CDeviceRemotePad::getPressedButtonFlags(u32 index){
    return lbl_eu_80665638[0]->mpPads[index]->mPressedButtonFlags;
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
