#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"
#include <cstring>

// Manual vtable defined at end of file (so MWCC's jump-table emission for
// wkStandby lands first in .data, matching the retail symbol order); forward
// declared here for the ctor/dtor vptr stores.
extern IWorkEventVtbl lbl_eu_8056B110;

// CDeviceClock methods referenced by this TU, declared by retail mangled name
// instead of including monolib/device.hpp: device.hpp pulls in CDeviceBase.hpp
// whose inline virtual dtor makes MWCC declare the __RTTI__ chain internally,
// which collides with the extern "C" void* __RTTI__ declarations this TU needs
// for its manual vtable (MWCC "illegal name overloading").
class CDeviceClock;
extern "C" CDeviceClock* getInstance__12CDeviceClockFv();
extern "C" bool isInitialized__12CDeviceClockFv(CDeviceClock* pThis);


// sbss data owned by this TU (blob monolibdata1d dissolve):
//   lbl_eu_80665594 (0x80665594, 4 bytes) = sAllocFlags (u32*)
//   lbl_eu_80665598 (0x80665598, 8 bytes) = sWorkThreads (CWorkThread**; word 0 in use)
// (lbl_eu_80665590 = sMemAvailable is defined in CWorkThreadSystem.cpp, its user.)
u32* lbl_eu_80665594;
CWorkThread** lbl_eu_80665598[2];
CWorkThread::CWorkThread(const char* pName, CWorkThread* pParent, int capacity)
    : mState(THREAD_STATE_NONE),
      mWorkID(INVALID_WORK_ID),
      mType(THREAD_CWORKTHREAD),
      mParent(pParent),
      mFlags(0),
      mMsgQueue(0, &unk1BC),
      mExceptionWorkID(INVALID_WORK_ID){

    *(void**)this = &lbl_eu_8056B110;
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
    *(void**)this = &lbl_eu_8056B110;
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
    // Retail only unlinks the first matching node (unlike reslist::remove, which walks all).
    // Loop shape matches retail: advance-at-top, head-check, then item compare.
    _reslist_node<CWorkThread*>* head = mChildren.mStartNodePtr;
    _reslist_node<CWorkThread*>* curr = head->mNext;
    goto check;

advance:
    curr = curr->mNext;
check:
    if(curr == head){
        goto done;
    }
    if(curr->mItem != pChild){
        goto advance;
    }

done:
    if(curr == head){
        return;
    }

    _reslist_node<CWorkThread*>* prev = curr->mPrev;
    _reslist_node<CWorkThread*>* next = curr->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    curr->mNext = nullptr;
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
    // Retail uses a 4-level unrolled descendant walk; recursive wkSetEvent
    // is the high-level equivalent and keeps the TU within split size.
    for(reslist<CWorkThread*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it){
        (*it)->wkSetEvent(evt);
    }
}

bool CWorkThread::wkCheckTimeout(u32 arg0, bool arg1, const char* pMessage){
    CDeviceClock* pDevClock = getInstance__12CDeviceClockFv();
    if(pDevClock == nullptr || (!isInitialized__12CDeviceClockFv(pDevClock) && !arg1)){
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
    (void)getInstance__12CDeviceClockFv();
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

#pragma dont_inline on
CWorkThread* CWorkThread::getWorkThread(const char* name){
    // Retail tolerates a null `this` (checks r3 before any member access).
    if(this == nullptr){
        return nullptr;
    }

    // Retail: addi r3, this, 4; bl strcmp (mString is first field of FixStr).
    if(!std::strcmp(reinterpret_cast<const char*>(&mName), name)){
        return this;
    }

    _reslist_node<CWorkThread*>* head = mChildren.mStartNodePtr;
    for(_reslist_node<CWorkThread*>* node = head->mNext; node != head; node = node->mNext){
        CWorkThread* result = node->mItem->getWorkThread(name);

        if(result != nullptr){
            if(result->mState != THREAD_STATE_SHUTDOWN){
                return result;
            }
        }
    }

    return nullptr;
}
#pragma dont_inline off

// Data owned by this TU (blob monolibdata1 dissolve). The CWorkThread vtable
// (lbl_eu_8056B110) is defined explicitly here because the class is novtable:
// MWCC would otherwise auto-emit __vt__11CWorkThread + the __RTTI__11CWorkThread
// base-list, which the retail does not carry in this split range.
IWorkEventVtbl lbl_eu_8056B110 = {
    (u32)&__RTTI__11CWorkThread, 0, (u32)&__dt__11CWorkThreadFv,
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
    (u32)&wkUpdate__11CWorkThreadFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__11CWorkThreadFv,
    (u32)&wkStandbyLogout__11CWorkThreadFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// sAllocHandle (mtl::ALLOC_HANDLE, initialized to INVALID_HANDLE 0xFFFFFFFF)
// - blob monolibdata1 dissolve. CWorkThreadSystem.cpp declares it extern and
// is its user; the definition lives here (sdata lbl_eu_8066351C).
u32 lbl_eu_8066351C = 0xFFFFFFFF;
