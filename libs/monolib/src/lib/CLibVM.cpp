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

CLibVM::~CLibVM() {
    spInstance = 0;
}

CLibVM* CLibVM::spInstance;

CLibVM* CLibVM::getInstance() {
    return spInstance;
}

bool CLibVM::isInitialized(){
    const CLibVM* inst = spInstance;

    bool exception;
    if (inst->mFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        // Inlined CMsgParam<8>::find(EVT_EXCEPTION): ring walk over the queue.
        const CMsgParam<8>* queue = &inst->mMsgQueue;
        int found;
        u32 i = 0;
        for (; i < queue->mRing.mSize; i++) {
            if (queue->mRing.mArrayPtr[(queue->mRing.mFront + i) % queue->mRing.mCapacity].command == EVT_EXCEPTION) {
                found = (int)i;
                goto merged;
            }
        }
        found = -1;
merged:
        exception = found >= 0;
    }

    // Running once the thread has logged in and started running.
    bool result = false;
    if (!exception) {
        bool stateOK = true;
        int state = inst->CWorkThread::mState;
        if (state != THREAD_STATE_LOGIN && state != THREAD_STATE_RUN) {
            stateOK = false;
        }
        if (stateOK) {
            result = true;
        }
    }
    return result;
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
void (*lbl_eu_80665724)();   // +4
void (*lbl_eu_80665728)();   // +8
void (*lbl_eu_8066572C)();   // +0xC (2nd word of the 8B retail 80665728 label)
