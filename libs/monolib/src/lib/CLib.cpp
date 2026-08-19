#include "monolib/lib.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"

CLib* CLib::spInstance;

CLib::CLib(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD) {
    spInstance = this;
    mType = THREAD_CLIB;
}

CLib::~CLib(){
    spInstance = nullptr;
}

CLib* CLib::getInstance(){
    return spInstance;
}

// Inline copy of CWorkThread::isRunning() visible only in this TU (same trick
// as CLibVM.cpp / CDeviceGX.cpp): the retail isInitialized inlines the member
// call with the this-arg bound to the instance, which births the global load
// before the find-loop index (inst=r6 / index=r7). CWorkRoot.cpp keeps the
// strong out-of-line definition.
inline bool CWorkThread::isRunning() const {
    bool exception;
    if (mFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        exception = mMsgQueue.find(EVT_EXCEPTION) >= 0;
    }

    bool result = false;
    if (!exception) {
        bool stateOK = mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN;
        if (stateOK) {
            result = true;
        }
    }
    return result;
}

bool CLib::isInitialized(){
    if(!spInstance->isRunning()) return false;
    if(CLibHbm::getInstance() == nullptr) return false;
    if(!CLibHbm::isInitialized()) return false;
    if(CLibG3d::getInstance() == nullptr) return false;
    if(!CLibG3d::isInitialized()) return false;
    if(CLibLayout::getInstance() == nullptr) return false;
    if(!CLibLayout::isInitialized()) return false;
    if(CLibVM::getInstance() == nullptr) return false;
    if(!CLibVM::isInitialized()) return false;
    if(CLibStaticData::getInstance() == nullptr) return false;
    return CLibStaticData::isInitialized();
}

void CLib::createLibs(){
    CREATE_WORKTHREAD(CLibHbm, this);
    CREATE_WORKTHREAD(CLibG3d, this);
    CREATE_WORKTHREAD(CLibLayout, this);
    CREATE_WORKTHREAD(CLibVM, this);
    CREATE_WORKTHREAD(CLibStaticData, this);
}

bool CLib::wkStandbyLogin(){
    if(CDevice::isAllReady()){
        createLibs();

        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

bool CLib::wkStandbyLogout(){
    //"Failed to log out"
    wkCheckTimeout(30000, mChildren.empty() && CWorkSystem::getInstance() == nullptr, "ログアウトに失敗しました");

    if(mChildren.empty() && CWorkSystem::getInstance() == nullptr) return CWorkThread::wkStandbyLogout();
    return false;
}

CLib* CLib::create(){
    CLib* lib = CREATE_WORKTHREAD(CLib, CWorkControl::getInstance());
    return lib;
}
