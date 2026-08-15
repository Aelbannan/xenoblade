#include "monolib/lib/CLibVM.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/core/CProcRoot.hpp"

// CLibVM login/logout callbacks, set via CLibVM::setCallbacks().
extern void (*lbl_eu_80665724)();
extern void (*lbl_eu_80665728)();

CLibVM::CLibVM(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 0) {
    lbl_eu_80665720 = this;
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

// Inline copy of CWorkThread::isRunning() visible only in this TU (same trick
// as CDeviceGX.cpp): the retail isInitialized inlines the member call with the
// this-arg bound to the instance, which births the global load before the
// find-loop index (inst=r6 / index=r7). CWorkRoot.cpp keeps the strong
// out-of-line definition.
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

bool CLibVM::isInitialized() {
    // Same shape as CDeviceGX::isInitialized: the inlined isRunning() member
    // call reproduces the retail register layout (no call emitted).
    return lbl_eu_80665720->isRunning();
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