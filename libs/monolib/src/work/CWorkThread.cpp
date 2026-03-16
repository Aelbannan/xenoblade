#include "monolib/device.hpp"
#include "monolib/work.hpp"

CWorkThread::CWorkThread(const char* pName, CWorkThread* pParent, int capacity)
    : mState(THREAD_STATE_NONE),
      mWorkID(INVALID_WORK_ID),
      mType(THREAD_CWORKTHREAD),
      mParent(pParent),
      mFlags(0),
      mMsgQueue(0),
      unk1BC(0),
      mExceptionWorkID(INVALID_WORK_ID){

    mAllocHandle = CWorkThreadSystem::sAllocHandle;
    mName = pName;
    mWorkID = CWorkThreadSystem::allocWID(this);

    if(capacity > 0){
        mChildren.reserve(mAllocHandle, capacity);
    }

    if(pParent != nullptr && pParent->isEvent3()){
        mFlags |= THREAD_FLAG_EVT3;
    }

    if(pParent != nullptr && (pParent->mFlags & THREAD_FLAG_EVT4)){
        mFlags |= THREAD_FLAG_EVT4;
    }

    if(pParent != nullptr && (pParent->mFlags & THREAD_FLAG_PAUSE)){
        mFlags |= THREAD_FLAG_PAUSE;
    }

    if(pParent != nullptr && (pParent->mFlags & THREAD_FLAG_EVT7)){
        mFlags |= THREAD_FLAG_EVT7;
    }

    if(pParent != nullptr && (pParent->mFlags & THREAD_FLAG_EVT9)){
        mFlags |= THREAD_FLAG_EVT9;
    }

    if(pParent != nullptr && (pParent->mFlags & THREAD_FLAG_APPEXCEPTION)){
        mFlags |= THREAD_FLAG_APPEXCEPTION;
    }

    if(pParent != nullptr && (pParent->mFlags & THREAD_FLAG_NO_EVENT)){
        mFlags |= THREAD_FLAG_NO_EVENT;
    }
}

CWorkThread::~CWorkThread(){
    if(!mChildren.empty()){
        for(reslist<CWorkThread*>::iterator it = mChildren.begin(); it != mChildren.end(); it++){
            //Do nothing???
            ;
        }
    }

    CWorkThreadSystem::freeWID(mWorkID);
}

void CWorkThread::wkReplaceHasChild(int capacity){
    if(capacity > 0){
        mChildren.destroyList();
        mChildren.reserve(mAllocHandle, capacity);
    }
}

void CWorkThread::wkEntryChild(CWorkThread* pChild, bool prepend){
    if(prepend){
        //Add the new child at the start
        mChildren.push_front(pChild);
    }else{
        //Add the new child at the end
        mChildren.push_back(pChild);
    }

    pChild->mParent = this;
}

void CWorkThread::wkRemoveChild(CWorkThread* pChild){
    mChildren.remove(pChild);
}

void CWorkThread::wkSetEvent(EVT evt){
    if(evt == EVT_NONE){
        mFlags |= THREAD_FLAG_NO_EVENT;
    }else{
        mMsgQueue.enqueue(evt);
    }

    wkSetEventChild(evt);
}

void CWorkThread::wkSetEventChild(EVT evt){
    for(reslist<CWorkThread*>::iterator it = mChildren.begin(); it != mChildren.end(); it++){
        (*it)->wkSetEvent(evt);
    }
}

bool CWorkThread::wkCheckTimeout(u32 arg0, bool arg1, const char* pMessage){
    CDeviceClock* pDevClock = CDeviceClock::getInstance();
    if(pDevClock == nullptr || (!pDevClock->isInitialized() && !arg1)){
        return false;
    }

    if(mFlags & THREAD_FLAG_EVT1){
        return true;
    }

    if(mState != THREAD_STATE_INIT && mState != THREAD_STATE_LOGIN && mState != THREAD_STATE_RUN){
        return true;
    }

    if(arg0 == 0){
        wkSetEvent(EVT_1);
        CWorkUtil::dispTree(CWorkControl::getInstance());
        return true;
    }

    return arg1;
}

bool CWorkThread::wkIsCurrent() const{
    if(mParent != nullptr){
        return this == mParent->wkGetChild();
    }

    return true;
}

CWorkThread* CWorkThread::getWorkThread(WORK_ID wid){
    if(wid == INVALID_WORK_ID){
        return nullptr;
    }

    return CWorkThreadSystem::sWorkThreads[wid];
}

void CWorkThread::func_804385CC(u32){}

void CWorkThread::wkTimeoutInit(){
    (void)CDeviceClock::getInstance();
}

bool CWorkThread::wkStandbyInit(){
    mState = THREAD_STATE_INIT;
    wkTimeoutInit();
    return true;
}

bool CWorkThread::wkStandbyRun(){
    if(isNoEvent()){
        mState = THREAD_STATE_RUN;
        wkTimeoutInit();
    }

    return mState == THREAD_STATE_RUN;
}

bool CWorkThread::wkStandbyShutdown(){
    mState = THREAD_STATE_SHUTDOWN;
    wkTimeoutInit();
    return true;
}

void CWorkThread::wkStandby(){
    mFlags &= 0xFFFF;

    while(!mMsgQueue.empty()){
        switch(mMsgQueue.front().command){
            case EVT_1:{
                mFlags |= THREAD_FLAG_EVT1;
                break;
            }

            case EVT_EXCEPTION:{
                mFlags |= THREAD_FLAG_EXCEPTION;
                mExceptionWorkID = mMsgQueue.front().wid;
                break;
            }

            case EVT_3:{
                mFlags |= THREAD_FLAG_EVT3;
                break;
            }

            case EVT_4:{
                mFlags |= THREAD_FLAG_EVT4;
                break;
            }

            case EVT_PAUSE:{
                mFlags |= THREAD_FLAG_PAUSE;
                OnPauseTrigger(true);
                break;
            }

            case EVT_UNPAUSE:{
                mFlags &= ~THREAD_FLAG_PAUSE;
                OnPauseTrigger(false);
                break;
            }

            case EVT_7:{
                if(!(mFlags & THREAD_FLAG_EVT9)){
                    mFlags |= THREAD_FLAG_EVT7;
                }
                break;
            }

            case EVT_8:{
                if(!(mFlags & THREAD_FLAG_EVT9)){
                    mFlags &= ~THREAD_FLAG_EVT7;
                }
                break;
            }

            case EVT_APPEXCEPTION_ON:{
                mFlags |= THREAD_FLAG_APPEXCEPTION;
                OnPauseTrigger(true);
                break;
            }

            case EVT_APPEXCEPTION_OFF:{
                mFlags &= ~THREAD_FLAG_APPEXCEPTION;
                OnPauseTrigger(false);
                break;
            }

            case EVT_9:{
                mFlags |= THREAD_FLAG_EVT9;
                break;
            }
        }

        mMsgQueue.pop();
    }

    if(!(mFlags & THREAD_FLAG_EXCEPTION)){
        switch(mState){
            case THREAD_STATE_NONE:{
                if(!wkStandbyInit()){
                    break;
                }

                //FALLTHROUGH
            }

            case THREAD_STATE_INIT:{
                if(isNoEvent()){
                    mState = THREAD_STATE_LOGIN;
                    wkTimeoutInit();
                } else if(!wkStandbyLogin()){
                    break;
                }

                //FALLTHROUGH
            }

            case THREAD_STATE_LOGIN:{
                if(!wkStandbyRun()){
                    break;
                }

                //FALLTHROUGH
            }

            case THREAD_STATE_RUN:{
                if(!wkStandbyLogout()){
                    break;
                }

                //FALLTHROUGH
            }

            case THREAD_STATE_LOGOUT:{
                wkStandbyShutdown();

                //FALLTHROUGH
            }

            case THREAD_STATE_SHUTDOWN:
            default:{
                break;
            }
        }
    } else if(wkStandbyExceptionRetry(mExceptionWorkID)){
            CWorkThread* pExceptionThread = getWorkThread(mExceptionWorkID);

            if(pExceptionThread != nullptr){
                pExceptionThread->wkSetEvent(EVT_NONE);
            }

            mExceptionWorkID = INVALID_WORK_ID;
            mFlags &= ~THREAD_FLAG_EXCEPTION;
    }
}

bool CWorkThread::wkStandbyLogin(){
    mState = THREAD_STATE_LOGIN;
    wkTimeoutInit();
    return true;
}

bool CWorkThread::wkStandbyLogout(){
    mState = THREAD_STATE_LOGOUT;
    wkTimeoutInit();
    return true;
}

void CWorkThread::wkUpdate(){}

CWorkThread* CWorkThread::getWorkThread(const char* name){
    if(name == nullptr){
        return nullptr;
    }

    if(mName == name){
        return this;
    }

    for(reslist<CWorkThread*>::iterator it = mChildren.begin(); it != mChildren.end(); it++){
        CWorkThread* result = (*it)->getWorkThread(name);

        if(result != nullptr && result->mState != THREAD_STATE_SHUTDOWN){
            return result;
        }
    }

    return nullptr;
}
