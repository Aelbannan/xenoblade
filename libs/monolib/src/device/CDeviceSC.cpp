// The retail CDeviceSC/CDeviceBase classes are effectively novtable: the
// retail ctor stores the manual vtable (lbl_eu_8056BD38) and no auto
// vtables/RTTI survive in the split (the retail linker GC'd the weak
// emissions). The headers lack __declspec(novtable), so declare it here via
// forward declarations BEFORE the class definitions come into scope -- MWCC
// merges the attribute and skips the implicit vptr-store/RTTI emissions that
// otherwise overshoot .data/.rodata/.sdata.
class __declspec(novtable) CDeviceBase;
class __declspec(novtable) CDeviceSC;

#include "monolib/device.hpp"
#include "monolib/data_vtables.hpp"
#include <revolution/SC.h>

// Retail vtable (defined in the data block below) -- forward-declared so the
// ctor can store it (novtable classes have no implicit vptr store).
extern "C" u32 lbl_eu_8056BD38[0xA0 / 4];

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
    *(u32**)this = lbl_eu_8056BD38;
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

u8 CDeviceSC::getLanguage(){
    if(lbl_eu_80665640[0] == nullptr){
        return SCGetLanguage();
    }
    return lbl_eu_80665640[0]->mLanguage;
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

bool func_eu_8044A600(){
    return SCGetLanguage() == 0;
}

// ---- Retail data (blob monolibdata1/monolibdata1d dissolve) ----
// rodata 0x80522980 (class-name string), sdata 0x806635F8 (class info),
// data 0x8056BD38 (vtable) and 0x8056BDD8 (RTTI base list). The retail uses
// the custom monolib RTTI/vtable layout, so these are defined explicitly as
// u32 word tables. The vtable-slot callees (__dt__, WorkEvent*, wk*, RTTI
// objects) are declared in monolib/data_vtables.hpp.

// rodata 0x80522980: RTTI class-name string "CDeviceSC" (exactly 10 bytes
// incl NUL; align(4) so the object is not padded to a 16-byte slot).
extern "C" __declspec(align(4)) const char lbl_eu_80522980[10] =
    {0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x53,0x43,0x00};  /* "CDeviceSC\0" */

// sdata 0x806635F8: class info {classname, RTTI base list} (declared in
// data_vtables.hpp, defined below).

// data 0x8056BD38: CDeviceSC vtable (custom layout [classinfo, 0, dtor,
// 32 IWorkEvent handlers, wkUpdate, wkRender, wkRenderAfter, wkStandbyLogin,
// wkStandbyLogout, wkStandbyExceptionRetry]).
extern "C" u32 lbl_eu_8056BD38[0xA0 / 4] = {
    (u32)&lbl_eu_806635F8, 0, (u32)&__dt__9CDeviceSCFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__9CDeviceSCFv,
    (u32)&wkStandbyLogout__9CDeviceSCFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

// data 0x8056BDD8: RTTI base list [IWorkEvent, 0, CWorkThread, 0, CDeviceBase
// classinfo, 0, 0, 0].
extern "C" u32 lbl_eu_8056BDD8[0x20 / 4] = {
    (u32)&__RTTI__10IWorkEvent, 0,
    (u32)&__RTTI__11CWorkThread, 0,
    (u32)&lbl_eu_806635F0, 0, 0, 0,
};

// sdata 0x806635F8: class info {classname, RTTI base list}.
extern "C" u32 lbl_eu_806635F8[2] = {
    (u32)&lbl_eu_80522980,
    (u32)&lbl_eu_8056BDD8,
};
