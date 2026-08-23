#include "monolib/lib.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"

// RTTI descriptor pair for the CLib typeinfo object (defined in the CGXCache
// .sdata2 pool, lbl_eu_8066A4C8).
extern "C" u32 lbl_eu_8066A4C8[2];

// CLib vtable, defined explicitly at the bottom of this TU (novtable class).
struct IWorkEventVtbl;
extern IWorkEventVtbl lbl_eu_8056CDA0;

// Forward decls: the vtable's typeinfo word points at the .sdata pair, which
// points at the RTTI base-list array (both defined at the bottom of this TU).
extern void* lbl_eu_80663788[2];
extern void* lbl_eu_8056CE40[6];

// Retail sbss singleton - DEFINED in core/CException.cpp (.sbss, 8B slot at
// 0x806656D0); this TU only references it, so declare it extern (retail
// CLib.o carries no .sbss).
extern "C" CLib* lbl_eu_806656D0;
#define spInstance lbl_eu_806656D0
#include <decomp.h>

CLib::CLib(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD) {
    // novtable: assign the retail vtable label explicitly (CToken/CWorkThread
    // recipe) so the inlined/out-of-line ctor stores stay byte-identical.
    *(void**)this = (void*)&lbl_eu_8056CDA0;
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

// Data owned by this TU (blob monolibdata2 dissolve).
//
// .data 0x8056CDA0: CLib vtable (0xA0), defined explicitly because the class
// is __declspec(novtable). Word 0 is the typeinfo pointer to the .sdata pair
// lbl_eu_8056CDA0 -> lbl_eu_80663788 below (-RTTI off suppresses it in the
// auto-emitted form).
//
// .data 0x8056CE40: lbl_eu_8056CE40 RTTI base-list array (0x18)
// { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 }.
//
// .sdata 0x80663788: lbl_eu_80663788 CLib typeinfo pair (0x8)
// { &lbl_eu_8066A4C8 (CGXCache .sdata2 pool), &lbl_eu_8056CE40 }.
//
// The string literals MWCC pools into local .rodata ship from the shared
// CException.cpp pool lbl_eu_80522F88: renamed + stripped by the UNIT_RULES
// entry in tools/postprocess_reloc_names.py.
extern "C" void __dt__4CLibFv(void*, int);
extern "C" void wkStandbyLogin__4CLibFv();
extern "C" void wkStandbyLogout__4CLibFv();

IWorkEventVtbl lbl_eu_8056CDA0 = {
    (u32)&lbl_eu_80663788[0], 0, (u32)&__dt__4CLibFv,
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

void* lbl_eu_8056CE40[6] = {
    (void*)&__RTTI__10IWorkEvent, nullptr,
    (void*)&__RTTI__11CWorkThread, nullptr,
    nullptr, nullptr,
};

void* lbl_eu_80663788[2] = {
    (void*)lbl_eu_8066A4C8,
    (void*)lbl_eu_8056CE40,
};
