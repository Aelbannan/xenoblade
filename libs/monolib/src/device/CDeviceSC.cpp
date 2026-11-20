#include "monolib/device.hpp"
#include <revolution/SC.h>

// Retail sbss singleton slot lbl_eu_80665640 (declared in CDeviceSC.hpp;
// 8 bytes, word 0 in use) - blob monolibdata1d dissolve.
CDeviceSC* lbl_eu_80665640[2];


// novtable: member init-list stores are scheduled BEFORE any body statement,
// so a manual vptr store written as the first body statement would be sunk
// below all of them; retail keeps the vptr install right after the base-ctor
// call. Assign the members in the body after the vptr store instead
// (MWCC_PATTERNS "novtable ctor vptr store").
CDeviceSC::CDeviceSC(const char* pName, CWorkThread* pParent) :
CDeviceBase(pName, pParent, MAX_CHILD){
    mAspectRatio = SC_ASPECT_STD;
    mEuRgb60Mode = SC_EURGB_50_HZ;
    mLanguage = SC_LANG_JP;
    mProgMode = SC_INTERLACED;
    mSoundMode = SC_SND_STEREO;
    // Zero the 0x1CD tail byte (reserved field; its name lives in the shared
    // CDeviceSC.hpp header).
    (&mSoundMode)[1] = 0;
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



extern "C" bool func_eu_8044A600(){
    return SCGetLanguage() == 0;
}


// Mirror view over the singleton's thread flag / message-queue tail fields
// (the private CMsgParam<8> members mArrayPtr/mFront/mSize/mCapacity sit at
// 0x1A4..0x1B0 inside CWorkThread::mMsgQueue). The retail inlines
// CMsgParam<8>::find(EVT_EXCEPTION) here, so we scan the queue directly.
class CDeviceSCFields {
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


u8 CDeviceSC::getLanguage(){
    if(lbl_eu_80665640[0] == nullptr){
        return SCGetLanguage();
    }
    return lbl_eu_80665640[0]->mLanguage;
}



// Inline copy of CWorkThread::isRunning()'s exception/queue scan written
// directly in isInitialized (kept body-local: a standalone static helper gets
// emitted out-of-line and busts the exact-fit split budget even when fully
// inlined -- see CDeviceFileCri KB case). The thread flag short-circuits,
// otherwise CMsgParam<8>::find (divwu modulo loop) scans for EVT_EXCEPTION.
bool CDeviceSC::isInitialized(){
    const CDeviceSCFields* self = (const CDeviceSCFields*)lbl_eu_80665640[0];
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
