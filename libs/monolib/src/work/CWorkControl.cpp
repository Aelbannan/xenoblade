#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"
#include "decomp.h"

// Minimal forward declaration of CDevice for the getInstance() call in
// wkStandbyLogout (emits the retail-mangled getInstance__7CDeviceFv).
// The full monolib/device/CDevice.hpp cannot be included here: it pulls in
// CDeviceBase.hpp whose inline virtual dtor makes MWCC declare the __RTTI__
// chain internally, colliding with the C-linkage void* __RTTI__ declarations
// this TU needs for its manual RTTI base list (MWCC "illegal name overloading").
class CDevice {
public:
    static CDevice* getInstance();
};

// Data owned by this TU (blob monolibdata1d dissolve):
//   lbl_eu_80522678 (.rodata) = CWorkControl RTTI name string
//   lbl_eu_80522688 (.rodata) = work-flow display-name pool (retail bytes)
//   lbl_eu_806635B0 (.sdata)  = lbl_eu_806635B0 {name, base-list}
//   lbl_eu_8056B7C8 (.data)   = CWorkControl vtable
//   lbl_eu_8056B868 (.data)   = RTTI base list {IWE,0,CWT,0,0,0}
//   CWorkControl::spInstance (.sbss) is defined below.
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

// lbl_eu_806635B0 = {name, base-list} (.sdata).
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

CWorkControl* CWorkControl::spInstance;
CWorkControl::CWorkControl(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD){
    *(IWorkEventVtbl**)this = &lbl_eu_8056B7C8;
    spInstance = this;
    mType = THREAD_CWORKCONTROL;
}
CWorkControl::~CWorkControl(){
    spInstance = nullptr;
}

CWorkControl* CWorkControl::getInstance(){
    return spInstance;
}
bool CWorkControl::setFlowSetup(){
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

void CWorkControl::pause(bool paused){
    getInstance()->wkSetEventChild(paused ? EVT_PAUSE : EVT_UNPAUSE);
}
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

bool CWorkControl::wkStandbyLogin(){
    setFlowSetup();
    return CWorkThread::wkStandbyLogin();
}

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
// base CWorkThread*, explicit WORK_ID mem local declared first (regresses
// scheduling: name lis/addi sinks past bl allocate), lazy name assign inside
// ctor arg (inlines the ctor, 112B), inline-name arg in new-expression (same
// ctor-inline regression), inline helper returning the name (no change),
// null-init ptr decl (elided), explicit MemManager::allocate + pointer
// placement-new (124B, extra null-path store). KB (btm_inq/ocUnit): MWCC
// colors callee-saved locals by web-creation order, but the constant name web
// is rematerialized so its creation order cannot be forced from source.
// Placement-new result web claims r31 regardless (DECOMP_INLINE helper also
// no-op: it expands to plain inline).
// FLAG PROBES (2026-08-24, both negative, no source change): mw_version=
// GC/3.0a5.2 leaves create unchanged and blows the unit split by 88B;
// -O4,s is now the unit flag (replaces the old per-function size pragmas), create unchanged. Remaining idea:
// register-renaming witness certification (cycle) for EQUIVALENT_MATCH.
// RE-RUN (2026-10-18): mangling verified clean (retail C++-mangled); -ipa off
// probed - create unchanged, setFlowSetup regressed 20-structural + split FAIL,
// reverted; alias-temp (CWorkThread* copy) CSE-merged no-op; void* mem temp +
// standard placement-new regressed (reloc shift), reverted. Color wall stands.
// RE-PROBES (2026-10-18, all negative, reverted to two-local baseline):
// declare-then-assign (result decl first) no-op; &pool[0x6C] array-index form
// no-op; name inlined into ctor arg restructures (16B frame, 2 saved regs,
// name computed post-null-check, 22 structural) - confirms two-local shape;
// the size codegen (now the -O4,s unit flag) is required. cycle witness-gate: abi-boundary (rho r30->r31) -
// witness-CLOSED, only FULL_MATCH can accept.
CWorkControl* CWorkControl::create(CWorkThread* pParent){
    const char* name = lbl_eu_80522688 + 0x6C;
    CWorkControl* pWorkControl =
        new (CWorkThreadSystem::getWorkMem()) CWorkControl(name, pParent);
    CWorkUtil::entryWork(pWorkControl, pParent, false);
    return pWorkControl;
}
