#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"
#include "decomp.h"

// Minimal local declaration of CDevice::getInstance: emits the retail-mangled
// call getInstance__7CDeviceFv without including monolib/device.hpp. That
// header pulls in CDeviceBase.hpp whose inline virtual dtor makes MWCC
// declare the __RTTI__ chain internally, which collides with the C-linkage
// void* __RTTI__ declarations this TU needs for its manual RTTI base list
// (MWCC "illegal name overloading").
class CDevice {
public:
    static CDevice* getInstance();
};

// Data owned by this TU (blob monolibdata1d dissolve):
//   lbl_eu_80522678 (.rodata) = CWorkControl RTTI name string
//   lbl_eu_80522688 (.rodata) = work-flow display-name pool (retail bytes)
//   lbl_eu_806635B0 (.sdata)  = __RTTI__12CWorkControl {name, base-list}
//   lbl_eu_8056B7C8 (.data)   = CWorkControl vtable
//   lbl_eu_8056B868 (.data)   = RTTI base list {IWE,0,CWT,0,0,0}
//   lbl_eu_806655D8 (.sbss)   = spInstance singleton (word 0 in use)
extern "C" const char lbl_eu_80522678[] = "CWorkControl";
extern "C" const char lbl_eu_80522688[0x80] =
    "CWorkFlowTvMode\0"
    "CWorkFlowSetup\0"
    "CWorkFlowShutdownAll\0"
    "CWorkFlowWiiMenu\0"
    "CWorkFlowWiiReset\0"
    "CWorkFlowWiiPowerOff\0"
    "CWorkControl\0";

extern IWorkEventVtbl lbl_eu_8056B7C8;
extern RttiBaseList2 lbl_eu_8056B868;

// __RTTI__12CWorkControl = {name, base-list} (.sdata).
u32 lbl_eu_806635B0[2] = {(u32)lbl_eu_80522678, (u32)&lbl_eu_8056B868};

IWorkEventVtbl lbl_eu_8056B7C8 = {
    (u32)&lbl_eu_806635B0, 0, (u32)&__dt__12CWorkControlFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__12CWorkControlFv,
    (u32)&wkStandbyLogout__12CWorkControlFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
RttiBaseList2 lbl_eu_8056B868 = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

// sbss singleton lbl_eu_806655D8 (8 bytes; word 0 in use) - blob monolibdata1d dissolve
CWorkControl* lbl_eu_806655D8[2];

#pragma dont_inline on
CWorkControl::CWorkControl(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD){
    *(void**)this = &lbl_eu_8056B7C8;
    lbl_eu_806655D8[0] = this;
    mType = THREAD_CWORKCONTROL;
}
#pragma dont_inline off

#pragma optimize_for_size on
CWorkControl::~CWorkControl(){
    lbl_eu_806655D8[0] = nullptr;
}
#pragma optimize_for_size off

CWorkControl* CWorkControl::getInstance(){
    return lbl_eu_806655D8[0];
}

void CWorkControl::pause(bool paused){
    getInstance()->wkSetEventChild(paused ? EVT_PAUSE : EVT_UNPAUSE);
}

#pragma optimize_for_size on
bool CWorkControl::hasFlow(){
    if(CWorkFlowWiiMenu::getInstance() != nullptr){
        return true;
    }

    if(CWorkFlowShutdownAll::getInstance() != nullptr){
        return true;
    }

    if(CWorkFlowWiiReset::getInstance() != nullptr){
        return true;
    }

    if(CWorkFlowWiiPowerOff::getInstance() != nullptr){
        return true;
    }

    // Last flow: single getInstance call whose nonzero result is the return
    // (retail emits the subic/subfe bool-idiom on this one call).
    return CWorkFlowSetup::getInstance() != nullptr;
}
#pragma optimize_for_size off

bool CWorkControl::wkStandbyLogin(){
    setFlowSetup();
    return CWorkThread::wkStandbyLogin();
}

#pragma optimize_for_size on
DECOMP_DONT_INLINE bool CWorkControl::setFlowSetup(){
    const char* name;
    CWorkThread* pControl;

    if(CWorkFlowSetup::getInstance() != nullptr){
        return true;
    }

    name = &lbl_eu_80522688[0x10];
    pControl = CWorkControl::getInstance();
    CWorkFlowSetup::create(name, pControl);
    return true;
}
#pragma optimize_for_size off

bool CWorkControl::wkStandbyLogout(){
    if(mChildren.empty() && CDevice::getInstance() == nullptr){
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

// Factory: allocate from the work heap, construct in place, register.
// OPEN ITEM: structural 0 / size exact / relocs clean; residual 6 pure
// reg-swaps - retail colors name=r31, result=r30, decomp is reversed.
// Ruled out: decl order (both ways), declare-then-assign, result typed as
// base CWorkThread*, explicit WORK_ID mem local (regresses scheduling),
// inline-name arg (regresses scheduling). Placement-new result web claims
// r31 regardless; next ideas: noinline pragma variant, parent-local copy
// with address-taken, mw_version probe.

#pragma optimize_for_size on
CWorkControl* CWorkControl::create(CWorkThread* pParent){
    const char* name = lbl_eu_80522688 + 0x6C;
    CWorkControl* pWorkControl =
        new (CWorkThreadSystem::getWorkMem()) CWorkControl(name, pParent);
    CWorkUtil::entryWork(pWorkControl, pParent, false);
    return pWorkControl;
}
#pragma optimize_for_size off
