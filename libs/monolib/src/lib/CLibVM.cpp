#include "monolib/lib/CLibVM.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/core/CProcRoot.hpp"

// CLibVM login/logout callbacks, set via CLibVM::setCallbacks().
extern void (*lbl_eu_80665724)();
extern void (*lbl_eu_80665728)();

CLibVM::CLibVM(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0) {
    spInstance = this;
    mType = THREAD_CLIBVM;
}

extern "C" CLibVM* __dt__6CLibVMFv(CLibVM* self, int deleting) {
    if (self != 0) {
        lbl_eu_80665720 = 0;
        __dt__11CWorkThreadFv(self, 0);
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CLibVM* CLibVM::spInstance;

CLibVM* CLibVM::getInstance() {
    return spInstance;
}

bool CLibVM::isInitialized() {
    CLibVM* inst = reinterpret_cast<CLibVM*>(lbl_eu_80665720);
    // Busy if exception-flagged or a "2" message is queued.
    bool busy = (inst->mFlags & THREAD_FLAG_EXCEPTION) != 0;
    if (!busy) {
        busy = inst->mMsgQueue.find(2) >= 0;
    }
    if (busy) return false;
    return inst->mState == 2 || inst->mState == 3;
}

void CLibVM::setCallbacks(void (*pLogin)(), void (*pLogout)()) {
    lbl_eu_80665724 = pLogin;
    lbl_eu_80665728 = pLogout;
}

bool CLibVM::wkStandbyLogin() {
    if (CDeviceGX::isInitialized()) {
        if (lbl_eu_80665724) lbl_eu_80665724();
        return CWorkThread::wkStandbyLogin();
    }
    return false;
}

bool CLibVM::wkStandbyLogout() {
    if (mChildren.empty() && CProcRoot::getInstance() == nullptr) {
        if (lbl_eu_80665728) lbl_eu_80665728();
        return CWorkThread::wkStandbyLogout();
    }
    return false;
}