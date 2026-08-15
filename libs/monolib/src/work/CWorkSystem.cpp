#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include <revolution/BASE.h>
#include <revolution/OS.h>
#include <revolution/VI.h>

extern void func_80496998();
extern void func_80496994();
extern UNKTYPE* func_804BC9EC();
extern void func_804BCC1C(UNKTYPE* r3);
extern void func_804BC9F4(UNKTYPE* r3, u32 r4);

// sbss data owned by this TU (blob monolibdata1d dissolve):
//   lbl_eu_80665618 (4 bytes) = sExitFunc (ExitFunc)
//   lbl_eu_8066561C (4 bytes) = spInstance (CWorkSystem*)
CWorkSystem::ExitFunc lbl_eu_80665618;
CWorkSystem* lbl_eu_8066561C;

CWorkSystem::CWorkSystem(const char *pName, CWorkThread *pParent) : CWorkThread(pName, pParent, 32),
mMemHandle(mtl::INVALID_HANDLE),
mPowerOff(false),
mReset(false),
mSaveLoadInvalidReset(false) {
    lbl_eu_8066561C = this;
    mType = THREAD_CWORKSYSTEM;
}

CWorkSystem::~CWorkSystem(){
    lbl_eu_8066561C = nullptr;
}

CWorkSystem* CWorkSystem::getInstance(){
    return lbl_eu_8066561C;
}

bool CWorkSystem::isOff(){
    if(lbl_eu_8066561C == nullptr) return false;
    return isPowerOff() || isReset();
}

mtl::ALLOC_HANDLE CWorkSystem::getMem(){
    return lbl_eu_8066561C->mMemHandle;
}

bool CWorkSystem::isPowerOff() {
    return lbl_eu_8066561C->mPowerOff;
}

bool CWorkSystem::isReset() {
    return lbl_eu_8066561C->mReset;
}


void CWorkSystem::setSaveLoadInvalidReset(bool state){
    CWorkSystem* instance = lbl_eu_8066561C;
    if(instance != nullptr){
        instance->mSaveLoadInvalidReset = state;
    }
}

void CWorkSystem::wkUpdate(){
    if(mSaveLoadInvalidReset) return;

    //Power-button shutdown request.
    if(mPowerOff == false && CErrorWii::isPowerCallbackCalled()){
        mPowerOff = true;
        VISetBlack(TRUE);
        VIFlush();
        VIWaitForRetrace();
        callExitFunc();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        OSReport("exit wii power off\n");
        OSShutdownSystem();
    }

    //Reset-button restart request.
    if(mReset == false && CErrorWii::isResetCallbackCalled()){
        mReset = true;
        VISetBlack(TRUE);
        VIFlush();
        VIWaitForRetrace();
        callExitFunc();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        OSReport("exit wii reset\n");
        OSRestart(0);
    }
}

bool CWorkSystem::wkStandbyLogin(){
    if(!CDeviceFile::isInitialized()) return false;

    CREATE_WORKTHREAD(CWorkSystemMem, this);
    mMemHandle = CWorkSystemMem::getHandle();
    CREATE_WORKTHREAD(CWorkSystemCache, this);
    CREATE_WORKTHREAD(CWorkSystemPack, this);
    CScriptCode::create(this);
    CProcRoot::create(this);
    CViewRoot::create(this);

    CDeviceGX::updateVerticalFilter(VFILTER_NONE);
    func_80496994();
    CTaskManager::Create();

    //Initialize the mersenne twister RNG class
    s64 time = CDeviceClock::getTimeNow();
    ml::math::mtInit(time & 0xFFFFFFFF);

    UNKTYPE* r3 = func_804BC9EC();
    func_804BC9F4(r3, 0);

    //Call base
    return CWorkThread::wkStandbyLogin();
}

bool CWorkSystem::wkStandbyLogout(){
    //"Failed to log out"
    wkCheckTimeout(30000, !hasChild(this), "ログアウトに失敗しました");

    if(CScriptCode::getInstance() != nullptr){
        func_80496998();
    }

    if(!hasChild(this)){
        CTaskManager::Release();
        UNKTYPE* r3 = func_804BC9EC();
        func_804BCC1C(r3); //probably class function

        mMemHandle = mtl::INVALID_HANDLE;

        //Call base
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

CWorkSystem* CWorkSystem::create(){
    return CREATE_WORKTHREAD(CWorkSystem, CWorkControl::getInstance());
}

void CWorkSystem::setExitFunc(ExitFunc func){
    lbl_eu_80665618 = func;
}

void CWorkSystem::callExitFunc(){
    if(lbl_eu_80665618 != nullptr){
        lbl_eu_80665618();
        CLibCri::func_80459A80();
        CDeviceFileCri::func_8044F964();
        CPadManager::destroy();
    }
}
