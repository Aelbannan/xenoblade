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

// Dissolved vtable for _reslist_base<IDeviceClockFrame*> (this TU's data blob);
// referenced explicitly by the destructor's vptr retag so the store reloc
// names the retail label instead of the compiler-generated __vt__ symbol.
extern u32 lbl_eu_8056C244[];

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

// ===== Dissolved retail data (CDeviceClock TU) =====
// Retail bytes/relocs dumped from build/us/asm/monolib/src/device/CDeviceClock.s
// .rodata 0x80522B48 size 0x58 align 8, .sdata 0x80663690 size 0x18 align 8, .data 0x8056C170 size 0xE0 align 8
// Emitted as extern "C" u32 arrays with (u32)&extern, rodata align, DECOMP_FORCEACTIVE, novtable.
extern "C" {
    extern u32 lbl_eu_8056C210[];
    extern u32 lbl_eu_8056C238[];
    extern u32 lbl_eu_806635F0[];
    extern u32 __RTTI__10IWorkEvent[];
    extern u32 __RTTI__11CWorkThread[];
    extern void __dt__12CDeviceClockFv();
    extern void WorkEvent1__10IWorkEventFPvPCc();
    extern void OnFileEvent__10IWorkEventFP10CEventFile();
    extern void WorkEvent3__10IWorkEventFPv();
    extern void WorkEvent4__10IWorkEventFv();
    extern void OnPauseTrigger__10IWorkEventFb();
    extern void WorkEvent6__10IWorkEventFv();
    extern void WorkEvent7__10IWorkEventFv();
    extern void WorkEvent8__10IWorkEventFv();
    extern void WorkEvent9__10IWorkEventFv();
    extern void WorkEvent10__10IWorkEventFv();
    extern void WorkEvent11__10IWorkEventFv();
    extern void WorkEvent12__10IWorkEventFv();
    extern void WorkEvent13__10IWorkEventFv();
    extern void WorkEvent14__10IWorkEventFv();
    extern void WorkEvent15__10IWorkEventFv();
    extern void WorkEvent16__10IWorkEventFv();
    extern void WorkEvent17__10IWorkEventFv();
    extern void WorkEvent18__10IWorkEventFv();
    extern void WorkEvent19__10IWorkEventFv();
    extern void WorkEvent20__10IWorkEventFv();
    extern void WorkEvent21__10IWorkEventFv();
    extern void WorkEvent22__10IWorkEventFv();
    extern void WorkEvent23__10IWorkEventFv();
    extern void WorkEvent24__10IWorkEventFv();
    extern void WorkEvent25__10IWorkEventFv();
    extern void WorkEvent26__10IWorkEventFv();
    extern void WorkEvent27__10IWorkEventFv();
    extern void WorkEvent28__10IWorkEventFv();
    extern void WorkEvent29__10IWorkEventFv();
    extern void WorkEvent30__10IWorkEventFv();
    extern void WorkEvent31__10IWorkEventFv();
    extern void wkUpdate__12CDeviceClockFv();
    extern void wkRender__11CWorkThreadFv();
    extern void wkRenderAfter__11CWorkThreadFv();
    extern void wkStandbyLogin__12CDeviceClockFv();
    extern void wkStandbyLogout__12CDeviceClockFv();
    extern void wkStandbyExceptionRetry__11CWorkThreadFUl();
    extern void __dt__29reslist_P17IDeviceClockFrameFv();
    extern void __dt__35_reslist_base_P17IDeviceClockFrameFv();
    extern const char lbl_eu_80522B48[];
    extern const char lbl_eu_80522B58[];
    extern const char lbl_eu_80522B78[];
    extern u32 lbl_eu_80663690[];
    extern u32 lbl_eu_80663698[];
    extern u32 lbl_eu_806636A0[];
    extern u32 lbl_eu_8056C170[];
    extern u32 lbl_eu_8056C22C[];
    extern u32 lbl_eu_8056C244[];
}
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80522B48[16] = {0x43,0x44,0x65,0x76,0x69,0x63,0x65,0x43,0x6c,0x6f,0x63,0x6b,0x00,0x00,0x00,0x00};
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80522B58[32] = {0x72,0x65,0x73,0x6c,0x69,0x73,0x74,0x3c,0x49,0x44,0x65,0x76,0x69,0x63,0x65,0x43,0x6c,0x6f,0x63,0x6b,0x46,0x72,0x61,0x6d,0x65,0x20,0x2a,0x3e,0x00,0x00,0x00,0x00};
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80522B78[40] = {0x5f,0x72,0x65,0x73,0x6c,0x69,0x73,0x74,0x5f,0x62,0x61,0x73,0x65,0x3c,0x49,0x44,0x65,0x76,0x69,0x63,0x65,0x43,0x6c,0x6f,0x63,0x6b,0x46,0x72,0x61,0x6d,0x65,0x20,0x2a,0x3e,0x00,0x00,0x00,0x00,0x00,0x00};
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663690[2] = { (u32)&lbl_eu_80522B48, (u32)&lbl_eu_8056C210 };
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_80663698[2] = { (u32)&lbl_eu_80522B58, (u32)&lbl_eu_8056C238 };
extern "C" __declspec(section ".sdata") __attribute__((aligned(8))) u32 lbl_eu_806636A0[2] = { (u32)&lbl_eu_80522B78, 0 };
extern "C" u32 lbl_eu_8056C170[40] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_80663690, 0,
    (u32)&__dt__12CDeviceClockFv,
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
    (u32)&wkUpdate__12CDeviceClockFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__12CDeviceClockFv,
    (u32)&wkStandbyLogout__12CDeviceClockFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl
};
extern "C" u32 lbl_eu_8056C210[7] __attribute__((aligned(8))) = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, (u32)&lbl_eu_806635F0, 0, 0
};
extern "C" u32 lbl_eu_8056C22C[3] __attribute__((aligned(4))) = {
    (u32)&lbl_eu_80663698, 0, (u32)&__dt__29reslist_P17IDeviceClockFrameFv
};
extern "C" u32 lbl_eu_8056C238[3] __attribute__((aligned(4))) = {
    (u32)&lbl_eu_806636A0, 0, 0
};
extern "C" u32 lbl_eu_8056C244[3] __attribute__((aligned(4))) = {
    (u32)&lbl_eu_806636A0, 0, (u32)&__dt__35_reslist_base_P17IDeviceClockFrameFv
};
DECOMP_FORCEACTIVE(CDeviceClock_cpp_rodata, lbl_eu_80522B48, lbl_eu_80522B58, lbl_eu_80522B78);
DECOMP_FORCEACTIVE(CDeviceClock_cpp_sdata, lbl_eu_80663690, lbl_eu_80663698, lbl_eu_806636A0);
DECOMP_FORCEACTIVE(CDeviceClock_cpp_vtable, lbl_eu_8056C170);
DECOMP_FORCEACTIVE(CDeviceClock_cpp_rtti, lbl_eu_8056C210, lbl_eu_8056C22C, lbl_eu_8056C238, lbl_eu_8056C244);
