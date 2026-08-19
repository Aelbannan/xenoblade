#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include "monolib/data_vtables.hpp"

// Data owned by this TU (blob monolibdata1d dissolve):
//   lbl_eu_80522708 (.rodata) = CWorkFlowSetup RTTI name string
//   lbl_eu_806635B8 (.sdata)  = __RTTI__14CWorkFlowSetup {name, base-list}
//   lbl_eu_8056B880 (.data)   = CWorkFlowSetup vtable
//   lbl_eu_8056B920 (.data)   = RTTI base list {IWE,0,CWT,0,0,0}
//   lbl_eu_806655E0 (.sbss)   = spInstance singleton (word 0 in use)
extern "C" const char lbl_eu_80522708[16] = {0x43,0x57,0x6F,0x72,0x6B,0x46,0x6C,0x6F,0x77,0x53,0x65,0x74,0x75,0x70,0x00,0x00};  /* "CWorkFlowSetup" pad to 0x80522708-18 */

extern IWorkEventVtbl lbl_eu_8056B880;
extern RttiBaseList2 lbl_eu_8056B920;

// __RTTI__14CWorkFlowSetup = {name, base-list} (.sdata).
u32 lbl_eu_806635B8[2] = {(u32)lbl_eu_80522708, (u32)&lbl_eu_8056B920};

IWorkEventVtbl lbl_eu_8056B880 = {
    (u32)&lbl_eu_806635B8, 0, (u32)&__dt__14CWorkFlowSetupFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__14CWorkFlowSetupFv,
    (u32)&wkStandbyLogout__14CWorkFlowSetupFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
RttiBaseList2 lbl_eu_8056B920 = {
    (u32)&__RTTI__10IWorkEvent, 0, (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

// Retail sbss singleton label for CWorkFlowSetup::spInstance (MWCC_CASES §1a).
// Defined here (global scope, so the symbol stays unmangled) so this object
// carries the sbss entry its own functions reference.
// Retail sbss singleton slot lbl_eu_806655E0 (8 bytes; word 0 in use) - blob monolibdata1d dissolve
CWorkFlowSetup* lbl_eu_806655E0[2];

CWorkFlowSetup::CWorkFlowSetup(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0), unk1C4(0){
    *(void**)this = &lbl_eu_8056B880;
    lbl_eu_806655E0[0] = this;
}

// Unit is compiled with -O4,s (configure.py): retail dtor saves r30+r31 via
// stmw/lmw, not separate stw/lwz.
CWorkFlowSetup::~CWorkFlowSetup(){
    lbl_eu_806655E0[0] = nullptr;
}

CWorkFlowSetup* CWorkFlowSetup::getInstance(){
    return lbl_eu_806655E0[0];
}

// Only stand by once the common library is fully initialized; otherwise refuse.
bool CWorkFlowSetup::wkStandbyLogin(){
    if(CLib::isInitialized()){
        wkSetEvent(EVT_NONE);
        return CWorkThread::wkStandbyLogin();
    }

    return false;
}

// Cannot log out while child threads (flow stages) are still attached.
bool CWorkFlowSetup::wkStandbyLogout(){
    return !mChildren.empty() ? false : CWorkThread::wkStandbyLogout();
}
