#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"

// Blob monolibdata1d dissolve: define the CWorkSystemMem vtable
// (0x8056BAA8), its RTTI base list (0x8056BB48) and the sbss
// singleton (0x80665620) here with the retail names/bytes.
IWorkEventVtbl lbl_eu_8056BAA8 = {
    (u32)&lbl_eu_806635D0, 0, (u32)&__dt__14CWorkSystemMemFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__14CWorkSystemMemFv,
    (u32)&wkStandbyLogout__14CWorkSystemMemFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
RttiBaseList2 lbl_eu_8056BB48 = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};
CWorkSystemMem* lbl_eu_80665620[2];

CWorkSystemMem::CWorkSystemMem(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 1) {
    // novtable: write the retail vptr (0x8056BAA8) + mHandle = -1 by hand so
    // the stores land in retail order (vptr first, then mHandle).
    *(void**)this = &lbl_eu_8056BAA8;
    mHandle = mtl::INVALID_HANDLE;
    lbl_eu_80665620[0] = this;
    mHandle = mtl::MemManager::create(mtl::MemManager::getHandleMEM2(), REGION_SIZE, mName.c_str());
}

CWorkSystemMem::~CWorkSystemMem(){
    lbl_eu_80665620[0] = nullptr;
}

mtl::ALLOC_HANDLE CWorkSystemMem::getHandle(){
    return lbl_eu_80665620[0]->mHandle;
}

bool CWorkSystemMem::wkStandbyLogin(){
    return CWorkThread::wkStandbyLogin();
}

bool CWorkSystemMem::wkStandbyLogout(){
    if(mChildren.empty()){
        if(mParent->hasSingleChild() && mParent->wkGetChild() == this){
            mtl::MemManager::erase(mHandle);
            mHandle = mtl::INVALID_HANDLE;
            return CWorkThread::wkStandbyLogout();
        }
    }

    return false;
}
