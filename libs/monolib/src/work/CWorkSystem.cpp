#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include <revolution/BASE.h>
#include <revolution/OS.h>
#include <revolution/VI.h>

extern void resetScriptCode();
extern void execScriptCode();
extern UNKTYPE* getScnHandle();
extern void resetScnData(UNKTYPE* scn);
extern void Coli_ParseChunks(UNKTYPE* scn, u32 flags);


// sbss data owned by this TU (blob monolibdata1d dissolve):
CWorkSystem::ExitFunc CWorkSystem::sExitFunc;
CWorkSystem* CWorkSystem::spInstance;

CWorkSystem::CWorkSystem(const char *pName, CWorkThread *pParent) : CWorkThread(pName, pParent, 32) {
    mMemHandle = mtl::INVALID_HANDLE;
    mPowerOff = false;
    mReset = false;
    mSaveLoadInvalidReset = false;
    spInstance = this;
    mType = THREAD_CWORKSYSTEM;
}

CWorkSystem::~CWorkSystem(){
    spInstance = nullptr;
}

CWorkSystem* CWorkSystem::getInstance(){
    return spInstance;
}

bool CWorkSystem::isOff(){
    if(spInstance == nullptr) return false;
    return isPowerOff() || isReset();
}

mtl::ALLOC_HANDLE CWorkSystem::getMem(){
    return spInstance->mMemHandle;
}

bool CWorkSystem::isPowerOff() {
    return spInstance->mPowerOff;
}

bool CWorkSystem::isReset() {
    return spInstance->mReset;
}


void CWorkSystem::setSaveLoadInvalidReset(bool state){
    CWorkSystem* instance = spInstance;
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

// Retail uses stmw/lmw r30 (size-optimized codegen) in this function.
bool CWorkSystem::wkStandbyLogin(){
    if(!CDeviceFile::isInitialized()) return false;

    // Named locals force MWCC to evaluate each name pointer before the create()
    // call's getWorkMem and keep it live across the inlined body (retail r31).
    const char* memName = "CWorkSystemMem";
    CWorkSystemMem* systemMem = CWorkSystemMem::create(memName, this);
    (void)systemMem;
    mMemHandle = CWorkSystemMem::getHandle();

    const char* cacheName = "CWorkSystemCache";
    CWorkSystemCache* systemCache = CWorkSystemCache::create(cacheName, this);
    (void)systemCache;

    const char* packName = "CWorkSystemPack";
    CWorkSystemPack* systemPack = CWorkSystemPack::create(packName, this);
    (void)systemPack;
    CScriptCode::create(this);
    CProcRoot::create(this);
    CViewRoot::create(this);

    CDeviceGX::updateVerticalFilter(VFILTER_NONE);
    execScriptCode();
    CTaskManager::Create();

    //Initialize the mersenne twister RNG class
    s64 time = CDeviceClock::getTimeNow();
    ml::math::mtInit(time & 0xFFFFFFFF);

    UNKTYPE* scn = getScnHandle();
    Coli_ParseChunks(scn, 0);

    //Call base
    return CWorkThread::wkStandbyLogin();
}

bool CWorkSystem::wkStandbyLogout(){
    //"Failed to log out"
    wkCheckTimeout(30000, !hasChild(this),
                   "\x83\x8d\x83\x4f\x83\x41\x83\x45\x83\x67\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd");

    if(CScriptCode::getInstance() != nullptr){
        resetScriptCode();
    }

    if(!hasChild(this)){
        CTaskManager::Release();
        UNKTYPE* scn = getScnHandle();
        resetScnData(scn); //probably class function

        mMemHandle = mtl::INVALID_HANDLE;

        //Call base
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

// Parameterless create: forwards to the DECL_WORKTHREAD_CREATE(CWorkSystem)
// inline with the class name and the CWorkControl root as parent.
CWorkSystem* CWorkSystem::create() {
    return create("CWorkSystem", CWorkControl::getInstance());
}

void CWorkSystem::setExitFunc(ExitFunc func){
    sExitFunc = func;
}

void CWorkSystem::callExitFunc(){
    if(sExitFunc != nullptr){
        sExitFunc();
        CLibCri::stopAllStreams();
        CDeviceFileCri::closeAdxfHandle();
        CPadManager::destroy();
    }
}
