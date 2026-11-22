#include "monolib/lib.hpp"
#include "monolib/work.hpp"
#include "monolib/work/CWorkFlowSetup.hpp"

CWorkFlowSetup* CWorkFlowSetup::spInstance;

CWorkFlowSetup::CWorkFlowSetup(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0){
    unk1C4 = 0;
    spInstance = this;
}

// Unit is compiled with -O4,s (configure.py): retail dtor saves r30+r31 via
// stmw/lmw, not separate stw/lwz.
CWorkFlowSetup::~CWorkFlowSetup(){
    spInstance = nullptr;
}

CWorkFlowSetup* CWorkFlowSetup::getInstance(){
    return spInstance;
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
