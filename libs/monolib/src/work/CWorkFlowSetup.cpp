#include "monolib/lib.hpp"
#include "monolib/work.hpp"

// Retail sbss singleton label for CWorkFlowSetup::spInstance (MWCC_REFERENCE §1a).
// Defined here (global scope, so the symbol stays unmangled) so this object
// carries the sbss entry its own functions reference.
// Retail sbss singleton slot lbl_eu_806655E0 (8 bytes; word 0 in use) - blob monolibdata1d dissolve
CWorkFlowSetup* lbl_eu_806655E0[2];

CWorkFlowSetup::CWorkFlowSetup(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0), unk1C4(0){
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
