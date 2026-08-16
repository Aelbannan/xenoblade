#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"
#include <revolution/BASE.h>
#include <revolution/OS.h>
#include <revolution/VI.h>

extern void func_80496998();
extern void func_80496994();
extern UNKTYPE* func_804BC9EC();
extern void func_804BCC1C(UNKTYPE* r3);
extern void func_804BC9F4(UNKTYPE* r3, u32 r4);

// ==== blob monolibdata1/1d dissolve: retail data owned by this TU ====
// rodata 0x80522790 (0xC): RTTI class-name string. rodata 0x8052279C (0x7C):
// the pooled string block; code references it by offset below so no local
// @NNNN pool entries are emitted (retail relocs name lbl_eu_8052279C).
const char lbl_eu_80522790[] = "CWorkSystem";
const char lbl_eu_8052279C[0x7C] =
    "exit wii power off\n\0exit wii reset\n\0CWorkSystemMem\0CWorkSystemCache\0"
    "CWorkSystemPack\0\x83\x8d\x83\x4f\x83\x41\x83\x45\x83\x67\x82\xc9\x8e\xb8\x94\x73\x82\xb5"
    "\x82\xdc\x82\xb5\x82\xbd\0CWorkSystem\0\0\0";
// data 0x8056B9F0 (0xA0): CWorkSystem vtable; data 0x8056BA90 (0x18): RTTI
// cast table {IWorkEvent,0},{CWorkThread,0},{0,0}.
IWorkEventVtbl lbl_eu_8056B9F0 = {
    (u32)&lbl_eu_806635C8, 0, (u32)&__dt__11CWorkSystemFv,
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
    (u32)&wkUpdate__11CWorkSystemFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__11CWorkSystemFv,
    (u32)&wkStandbyLogout__11CWorkSystemFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
RttiBaseList2 lbl_eu_8056BA90 = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};
// sdata 0x806635C8 (0x8): RTTI typeinfo { class-name, cast-table }.
u32 lbl_eu_806635C8[2] = { (u32)&lbl_eu_80522790, (u32)&lbl_eu_8056BA90 };

// sbss data owned by this TU (blob monolibdata1d dissolve):
//   lbl_eu_80665618 (4 bytes) = sExitFunc (ExitFunc)
//   lbl_eu_8066561C (4 bytes) = spInstance (CWorkSystem*)
CWorkSystem::ExitFunc lbl_eu_80665618;
CWorkSystem* lbl_eu_8066561C;

DECOMP_DONT_INLINE CWorkSystem::CWorkSystem(const char *pName, CWorkThread *pParent) : CWorkThread(pName, pParent, 32) {
    // novtable: write the retail vptr (0x8056B9F0) first so the stores land in
    // retail order (vptr, then the member inits).
    *(void**)this = (void*)&lbl_eu_8056B9F0;
    mMemHandle = mtl::INVALID_HANDLE;
    mPowerOff = false;
    mReset = false;
    mSaveLoadInvalidReset = false;
    lbl_eu_8066561C = this;
    mType = THREAD_CWORKSYSTEM;
}

#pragma push
#pragma optimize_for_size on
CWorkSystem::~CWorkSystem(){
    lbl_eu_8066561C = nullptr;
}
#pragma pop

CWorkSystem* CWorkSystem::getInstance(){
    return lbl_eu_8066561C;
}

bool CWorkSystem::isOff(){
    if(lbl_eu_8066561C == nullptr) return false;
    return isPowerOff() || isReset();
}

mtl::ALLOC_HANDLE CWorkSystem::getMem(){
    return lbl_eu_8066561C->mMemHandle;
}

bool CWorkSystem::isPowerOff() {
    return lbl_eu_8066561C->mPowerOff;
}

bool CWorkSystem::isReset() {
    return lbl_eu_8066561C->mReset;
}


void CWorkSystem::setSaveLoadInvalidReset(bool state){
    CWorkSystem* instance = lbl_eu_8066561C;
    if(instance != nullptr){
        instance->mSaveLoadInvalidReset = state;
    }
}

void CWorkSystem::wkUpdate(){
    if(mSaveLoadInvalidReset) return;

    //Power-button shutdown request.
    if(mPowerOff == false && CErrorWii::isPowerCallbackCalled()){
        mPowerOff = true;
        VISetBlack(TRUE);
        VIFlush();
        VIWaitForRetrace();
        callExitFunc();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        OSReport(&lbl_eu_8052279C[0x00]); // "exit wii power off\n"
        OSShutdownSystem();
    }

    //Reset-button restart request.
    if(mReset == false && CErrorWii::isResetCallbackCalled()){
        mReset = true;
        VISetBlack(TRUE);
        VIFlush();
        VIWaitForRetrace();
        callExitFunc();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        OSReport(&lbl_eu_8052279C[0x14]); // "exit wii reset\n"
        OSRestart(0);
    }
}

bool CWorkSystem::wkStandbyLogin(){
    if(!CDeviceFile::isInitialized()) return false;

    CWorkSystemMem::create(&lbl_eu_8052279C[0x24], this); // "CWorkSystemMem"
    mMemHandle = CWorkSystemMem::getHandle();
    CWorkSystemCache::create(&lbl_eu_8052279C[0x33], this); // "CWorkSystemCache"
    CWorkSystemPack::create(&lbl_eu_8052279C[0x44], this); // "CWorkSystemPack"
    CScriptCode::create(this);
    CProcRoot::create(this);
    CViewRoot::create(this);

    CDeviceGX::updateVerticalFilter(VFILTER_NONE);
    func_80496994();
    CTaskManager::Create();

    //Initialize the mersenne twister RNG class
    s64 time = CDeviceClock::getTimeNow();
    ml::math::mtInit(time & 0xFFFFFFFF);

    UNKTYPE* r3 = func_804BC9EC();
    func_804BC9F4(r3, 0);

    //Call base
    return CWorkThread::wkStandbyLogin();
}

bool CWorkSystem::wkStandbyLogout(){
    //"Failed to log out"
    wkCheckTimeout(30000, !hasChild(this), &lbl_eu_8052279C[0x54]); // "ログアウトに失敗しました"

    if(CScriptCode::getInstance() != nullptr){
        func_80496998();
    }

    if(!hasChild(this)){
        CTaskManager::Release();
        UNKTYPE* r3 = func_804BC9EC();
        func_804BCC1C(r3); //probably class function

        mMemHandle = mtl::INVALID_HANDLE;

        //Call base
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

// Retail Fv create: getInstance -> getWorkMem -> placement-new (the name
// string "CWorkSystem" is pooled at lbl_eu_8052279C+109) -> entryWork.
// auto_inline off keeps the same-TU ctor a real call (retail reloc
// __ct__11CWorkSystemFPCcP11CWorkThread; MWCC would otherwise inline it).
#pragma push
#pragma auto_inline off
CWorkSystem* CWorkSystem::create() {
    CWorkThread* parent = CWorkControl::getInstance();
    WORK_ID id = CWorkThreadSystem::getWorkMem();
    CWorkSystem* obj = new (id) CWorkSystem(&lbl_eu_8052279C[0x6D], parent); // "CWorkSystem"
    CWorkUtil::entryWork(obj, parent, false);
    return obj;
}
#pragma pop

void CWorkSystem::setExitFunc(ExitFunc func){
    lbl_eu_80665618 = func;
}

void CWorkSystem::callExitFunc(){
    if(lbl_eu_80665618 != nullptr){
        lbl_eu_80665618();
        CLibCri::func_80459A80();
        CDeviceFileCri::func_8044F964();
        CPadManager::destroy();
    }
}
