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
#include <revolution/SC.h>

// Retail vtable (defined in the data block below) -- forward-declared so the
// ctor can store it (novtable classes have no implicit vptr store).
extern "C" u32 lbl_eu_8056BD38[0xA0 / 4];

// Retail sbss singleton slot lbl_eu_80665640 (8 bytes; word 0 in use) - blob monolibdata1d dissolve
CDeviceSC* lbl_eu_80665640[2];

CDeviceSC::CDeviceSC(const char* pName, CWorkThread* pParent) :
CDeviceBase(pName, pParent, MAX_CHILD),
mAspectRatio(SC_ASPECT_STD),
mEuRgb60Mode(SC_EURGB_50_HZ),
mLanguage(SC_LANG_JP),
mProgMode(SC_INTERLACED),
mSoundMode(SC_SND_STEREO),
unk1CD(0){
    *(void**)this = (void*)&lbl_eu_8056BD38;  // novtable: store the retail vtable by hand
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

// Inlined equivalent of CMsgParam<8>::find(msg): returns the matching slot
// index, or -1 when exhausted. Written as an inline helper so MWCC merges the
// return value with the induction variable (retail register allocation).
// Fields are re-read through the instance pointer inside the loop, matching
// the retail load pattern.
static inline int findMsg(u32 msg){
    CDeviceSCFields* pInst = (CDeviceSCFields*)lbl_eu_80665640[0];
    for (u32 i = 0; i < pInst->mMsgSize; i++) {
        if (pInst->mMsgArray[(pInst->mMsgFront + i) % pInst->mMsgCapacity].command == msg) {
            return i;
        }
    }
    return -1;
}

bool CDeviceSC::isInitialized(){
    CDeviceSCFields* inst = (CDeviceSCFields*)lbl_eu_80665640[0];

    // Exception flag set or a queued EVT_EXCEPTION event means the device is
    // not initialized.
    bool hasException;
    if (inst->mThreadFlags & CWorkThread::THREAD_FLAG_EXCEPTION) {
        hasException = true;
    } else {
        // Inlined queue scan for a queued EVT_EXCEPTION event.
        hasException = findMsg(CWorkThread::EVT_EXCEPTION) >= 0;
    }

    // Initialized when the thread has logged in and started running.
    // Initialized when the thread has logged in and started running.
    if (!hasException) {
        int state = inst->mState;
        if (state == CWorkThread::THREAD_STATE_LOGIN ||
            state == CWorkThread::THREAD_STATE_RUN) {
            return true;
        }
    }
    return false;
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
// u32 word tables. The __RTTI__ externs are declared in an anonymous
// namespace: with -RTTI on MWCC auto-generates the same C-linkage names for
// the class hierarchy, which would otherwise clash with file-scope
// declarations ("illegal name overloading"); the anonymous namespace hides
// the names while extern "C" still emits the exact retail reloc symbols.
namespace {
extern "C" void* __RTTI__10IWorkEvent;
extern "C" void* __RTTI__11CWorkThread;
}

extern "C" void* lbl_eu_806635F0;                  // RTTI CDeviceBase (sdata)
extern "C" void __dt__9CDeviceSCFv();
extern "C" void wkStandbyLogin__9CDeviceSCFv();
extern "C" void wkStandbyLogout__9CDeviceSCFv();
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
extern "C" int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern "C" int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern "C" int WorkEvent3__10IWorkEventFPv(void*);
extern "C" int WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb(int);
extern "C" int WorkEvent6__10IWorkEventFv();
extern "C" int WorkEvent7__10IWorkEventFv();
extern "C" int WorkEvent8__10IWorkEventFv();
extern "C" int WorkEvent9__10IWorkEventFv();
extern "C" int WorkEvent10__10IWorkEventFv();
extern "C" int WorkEvent11__10IWorkEventFv();
extern "C" int WorkEvent12__10IWorkEventFv();
extern "C" int WorkEvent13__10IWorkEventFv();
extern "C" int WorkEvent14__10IWorkEventFv();
extern "C" int WorkEvent15__10IWorkEventFv();
extern "C" int WorkEvent16__10IWorkEventFv();
extern "C" int WorkEvent17__10IWorkEventFv();
extern "C" int WorkEvent18__10IWorkEventFv();
extern "C" int WorkEvent19__10IWorkEventFv();
extern "C" int WorkEvent20__10IWorkEventFv();
extern "C" int WorkEvent21__10IWorkEventFv();
extern "C" int WorkEvent22__10IWorkEventFv();
extern "C" int WorkEvent23__10IWorkEventFv();
extern "C" int WorkEvent24__10IWorkEventFv();
extern "C" int WorkEvent25__10IWorkEventFv();
extern "C" int WorkEvent26__10IWorkEventFv();
extern "C" int WorkEvent27__10IWorkEventFv();
extern "C" int WorkEvent28__10IWorkEventFv();
extern "C" int WorkEvent29__10IWorkEventFv();
extern "C" int WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();

// rodata 0x80522980: RTTI class-name string "CDeviceSC" (exactly 10 bytes
// incl NUL; align(4) so the object is not padded to a 16-byte slot).
extern "C" __declspec(align(4)) const char lbl_eu_80522980[10] =
    {0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x53,0x43,0x00};  /* "CDeviceSC\0" */

// sdata 0x806635F8: class info {classname, RTTI base list} (defined below).
extern "C" u32 lbl_eu_806635F8[2];

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
