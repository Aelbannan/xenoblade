#include "CLibG3d.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CProcRoot.hpp"
#include <nw4r/g3d/g3d_init.h>
#include <nw4r/g3d/g3d_state.h>

// CMsgParam<8> keeps its queue internals private, so mirror the tail layout of
// CWorkThread's mMsgQueue member (mMsgQueue at 0x80 -> internals at 0x1A4-
// 0x1B0) to let the inline EVT_EXCEPTION scan below read the ring buffer
// directly. Field types/offsets mirror CMsgParam.hpp exactly.
struct CMsgQueueData {
    u8 pad[0x1A4];               // CWorkThread prefix + vtable + mEntries[8]
    CMsgParamEntry* mArrayPtr;   // 0x1A4 (mMsgQueue.mArrayPtr)
    u32 mFront;                  // 0x1A8 (mMsgQueue.mFront)
    u32 mSize;                   // 0x1AC (mMsgQueue.mSize)
    u32 mCapacity;               // 0x1B0 (mMsgQueue.mCapacity)
};

CLibG3d::CLibG3d(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 4) {
    lbl_eu_806656F0 = this;
    mType = THREAD_CLIBG3D;
}

CLibG3d::~CLibG3d() {
    lbl_eu_806656F0 = nullptr;
}

CLibG3d* CLibG3d::getInstance() {
    return lbl_eu_806656F0;
}

bool CLibG3d::isInitialized() {
    // G3D counts as "not initialized" while an exception is pending, or while
    // an EVT_EXCEPTION message is still queued waiting to be processed.
    CLibG3d* inst = lbl_eu_806656F0;
    bool busy;
    if (inst->checkFlag(THREAD_FLAG_EXCEPTION)) {
        busy = true;
    } else {
        // Scan the message queue for a pending EVT_EXCEPTION. Written inline
        // (mirroring CMsgParam::find()) rather than as a member call so the
        // scan index stays a direct local - an inlined callee's index would
        // otherwise win the r6/r7 split and break the retail register layout.
        CMsgQueueData* q = reinterpret_cast<CMsgQueueData*>(inst);
        int i;
        int foundIndex;
        for (i = 0; i < q->mSize; i++) {
            if (q->mArrayPtr[(q->mFront + i) % q->mCapacity].command == EVT_EXCEPTION) {
                foundIndex = i;
                goto done;
            }
        }
        foundIndex = -1;  // not found
    done:
        busy = foundIndex >= 0;
    }
    return !busy
        && (inst->mState == THREAD_STATE_LOGIN
            || inst->mState == THREAD_STATE_RUN);
}

bool CLibG3d::wkStandbyLogin() {
    if (CDeviceGX::isInitialized()) {
        nw4r::g3d::G3dInit(true);
        nw4r::g3d::G3DState::SetRenderModeObj(*CDeviceVI::getRenderModeObj());
        return CWorkThread::wkStandbyLogin();
    }
    return false;
}

bool CLibG3d::wkStandbyLogout() {
    if (mChildren.empty() && CProcRoot::getInstance() == nullptr) {
        return CWorkThread::wkStandbyLogout();
    }
    return false;
}