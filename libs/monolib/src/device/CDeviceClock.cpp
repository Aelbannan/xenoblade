class __declspec(novtable) CDeviceBase;
#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include "monolib/core.hpp"
#include <cstring>
#include <decomp.h>

extern template class reslist<IDeviceClockFrame*>;
extern template class _reslist_base<IDeviceClockFrame*>;

// Retail stores the clock singleton pointer in this .sbss slot (referenced
// directly by isInitialized rather than via spInstance).
extern CDeviceClock* lbl_eu_80665658;

CDeviceClock* CDeviceClock::spInstance;

/* Won't match because of stupid extab issue (the start pc for the first pc action
is 0xA0 instead of 0xA4) */
CDeviceClock::CDeviceClock(const char* pName, CWorkThread* pWorkThread) :
CDeviceBase(pName,pWorkThread,0),
unk1C8(0),
unk1F0(0),
mUpdateTime(0),
mFrameStartTime(0),
mFrameDuration(0) {
    spInstance = this;
    std::memset((void*)&mCalendar, 0, sizeof(OSCalendarTime));
    unk1F0 = getTimeNow();
    unk1C8 |= 1;
    mFrameList.reserve(mAllocHandle, 16);
}

CDeviceClock::~CDeviceClock(){
    // Retail stores via the dissolved .sbss slot, not the mangled static.
    lbl_eu_80665658 = nullptr;
    // NOTE: the remaining member destruction (inlined _reslist_base dtor:
    // vptr retag + node unlink + sentinel ring close + array free + base
    // CWorkThread dtor + conditional delete) is generated implicitly by
    // MWCC and is byte-identical; only its vptr-store reloc still names
    // __vt__35_reslist_base<P17IDeviceClockFrame> instead of the dissolved
    // lbl_eu_8056C244 (needs a postprocess exact_rename; no source lever).
}

CDeviceClock* CDeviceClock::getInstance(){
    return spInstance;
}

// Mirror view over the singleton's thread flag / message-queue tail fields
// (the private CMsgParam<8> members mArrayPtr/mFront/mSize/mCapacity sit at
// 0x1A4..0x1B0 inside CWorkThread::mMsgQueue). The retail inlines
// CWorkThread::isRunning() + CMsgParam<8>::find(EVT_EXCEPTION) here, so we
// scan the queue directly (same shape as CDeviceSC::isInitialized).
class CDeviceClockFields {
public:
    u8 field_0x0[0x48];              //0x0
    int mState;                      //0x48 (CWorkThread::ThreadState)
    u8 field_0x4C[0x7C - 0x4C];      //0x4C
    u32 mThreadFlags;                //0x7C (CWorkThread::ThreadFlags)
    u8 field_0x80[0x1A4 - 0x80];     //0x80..0x1A4 (queue vtable + entries)
    CMsgParamEntry* mMsgArray;       //0x1A4 (CMsgParam::mArrayPtr)
    u32 mMsgFront;                   //0x1A8 (CMsgParam::mFront)
    u32 mMsgSize;                    //0x1AC (CMsgParam::mSize)
    u32 mMsgCapacity;                //0x1B0 (CMsgParam::mCapacity)
};

bool CDeviceClock::isInitialized(){
    const CDeviceClockFields* self = (const CDeviceClockFields*)lbl_eu_80665658;
    bool busy;
    if (self->mThreadFlags & CWorkThread::THREAD_FLAG_EXCEPTION) {
        busy = true;
    } else {
        int i;
        int foundIndex;
        for (i = 0; i < self->mMsgSize; i++) {
            if (self->mMsgArray[(self->mMsgFront + i) % self->mMsgCapacity].command == CWorkThread::EVT_EXCEPTION) {
                foundIndex = i;
                goto done;
            }
        }
        foundIndex = -1;
    done:
        busy = foundIndex >= 0;
    }

    return !busy
        && (self->mState == CWorkThread::THREAD_STATE_LOGIN ||
            self->mState == CWorkThread::THREAD_STATE_RUN);
}

s64 CDeviceClock::getTimeNow(){
    return OSGetTime();
}

void CDeviceClock::onStartFrame(){
    s64 time = getTimeNow();
    spInstance->mFrameStartTime = time;

    //Trigger the event function for each class in the list
    for(reslist<IDeviceClockFrame*>::iterator it = spInstance->mFrameList.begin(); it != spInstance->mFrameList.end(); it++){
        (*it)->onStartFrame();
    }
}

void CDeviceClock::onEndFrame(){
    s64 time = getTimeNow();
    spInstance->mFrameDuration = time - spInstance->mFrameStartTime;
    
    //Trigger the event function for each class in the list
    for(reslist<IDeviceClockFrame*>::iterator it = spInstance->mFrameList.begin(); it != spInstance->mFrameList.end(); it++){
        (*it)->onEndFrame();
    }
}

void CDeviceClock::wkUpdate(){
    mUpdateTime = getTimeNow();
    OSTicksToCalendarTime(mUpdateTime, &mCalendar);
}

bool CDeviceClock::wkStandbyLogin(){
    CStopwatchUtil::initialize();
    return CWorkThread::wkStandbyLogin(); //Call base
}

bool CDeviceClock::wkStandbyLogout(){
    if(mChildren.empty()){
        if(CDeviceSC::getInstance() == nullptr && CWorkSystem::getInstance() == nullptr
        && CLib::getInstance() == nullptr){
            return CWorkThread::wkStandbyLogout(); //Call base
        }
    }

    return false;
}
