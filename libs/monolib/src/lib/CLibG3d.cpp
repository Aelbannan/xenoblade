#include "monolib/lib/CLibG3d.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CProcRoot.hpp"
#include <nw4r/g3d/g3d_init.h>
#include <nw4r/g3d/g3d_state.h>

// Singleton instance backpointer to this CLibG3d (retail data symbol).
extern CLibG3d* lbl_eu_806656F0;

CLibG3d::CLibG3d(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 4) {
    lbl_eu_806656F0 = this;
    mType = THREAD_CLIBG3D;
}

CLibG3d::~CLibG3d() {
    lbl_eu_806656F0 = NULL;
}

CLibG3d* CLibG3d::getInstance() {
    return lbl_eu_806656F0;
}

bool CLibG3d::isInitialized() {
    // G3D counts as "not initialized" while an exception is pending, or while
    // an EVENT_2 message is still queued waiting to be processed.
    CLibG3d* inst = lbl_eu_806656F0;
    bool result;
    if (inst->mFlags & THREAD_FLAG_EXCEPTION) {
        result = true;
    } else {
        result = inst->mMsgQueue.find(2) >= 0;
    }
    if (result)
        return false;
    return inst->mState == THREAD_STATE_LOGIN || inst->mState == THREAD_STATE_RUN;
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