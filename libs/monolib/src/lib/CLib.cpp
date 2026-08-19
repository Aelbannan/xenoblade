#include "monolib/lib.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"

// Retail singleton pointer lives in a foreign .sbss range.
extern CLib* lbl_eu_806656D0;
// Forward decls for the data block at the bottom.
extern "C" u32 lbl_eu_8056CDA0[];
extern "C" void __dt__4CLibFv();
extern "C" void wkStandbyLogin__4CLibFv();
extern "C" void wkStandbyLogout__4CLibFv();
extern "C" u32 lbl_eu_8066A4C8;

CLib::CLib(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD) {
    // Retail vtable store (novtable class -> stored manually).
    *(void**)this = (void*)lbl_eu_8056CDA0;
    lbl_eu_806656D0 = this;
    mType = THREAD_CLIB;
}

CLib::~CLib(){
    lbl_eu_806656D0 = nullptr;
}

CLib* CLib::getInstance(){
    return lbl_eu_806656D0;
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
    if(!lbl_eu_806656D0->isRunning()) return false;
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

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056CDA0-0x8056CE58 (184B): CLib vtable (40 words) + RTTI
// base-list. The vtable slots reference the member functions defined above
// (dtor/wkStandbyLogin/wkStandbyLogout) and the IWorkEvent/CWorkThread
// virtuals (foreign).
extern "C" void* __RTTI__10IWorkEvent;
extern "C" void* __RTTI__11CWorkThread;
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile();
extern "C" void WorkEvent3__10IWorkEventFPv();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
extern "C" u32 lbl_eu_8056CDA0[];
extern "C" u32 lbl_eu_8056CE40[];
extern "C" u32 lbl_eu_80663788[];

extern "C" u32 lbl_eu_8056CDA0[40] = {
    (u32)&lbl_eu_80663788, 0x00000000, (u32)&__dt__4CLibFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__4CLibFv,
    (u32)&wkStandbyLogout__4CLibFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
extern "C" u32 lbl_eu_8056CE40[6] = {
    (u32)&__RTTI__10IWorkEvent, 0x00000000, (u32)&__RTTI__11CWorkThread,
    0x00000000, 0x00000000, 0x00000000,
};
DECOMP_FORCEACTIVE(CLib_cpp, lbl_eu_8056CDA0);

// [.sdata] 0x80663788-0x80663790 (8B): RTTI locator {name, base-list}.
extern "C" u32 lbl_eu_80663788[2] = { (u32)&lbl_eu_8066A4C8, (u32)&lbl_eu_8056CE40 };
