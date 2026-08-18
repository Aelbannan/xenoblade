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
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// Vtable forward refs by retail mangled names (CMdlLook namespace pattern).
namespace CLibVMBlob {
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyLogin__6CLibVMFv();
extern "C" void wkStandbyLogout__6CLibVMFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile();
extern "C" void WorkEvent3__10IWorkEventFPv();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
}
extern "C" u32 lbl_eu_8066A5B0;   // .sdata2 string
extern "C" u32 lbl_eu_8056D5E8;   // .data object
extern "C" u32 lbl_eu_806637F0[2]; // this unit's sdata

// [.data] 0x8056D548-0x8056D600 (0xB8 = 184B): CLibVM vtable
extern "C" u32 lbl_eu_8056D548[46] = {
    (u32)&lbl_eu_806637F0, 0x00000000,
    (u32)&__dt__6CLibVMFv,
    (u32)&CLibVMBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&CLibVMBlob::OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&CLibVMBlob::WorkEvent3__10IWorkEventFPv,
    (u32)&CLibVMBlob::WorkEvent4__10IWorkEventFv,
    (u32)&CLibVMBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&CLibVMBlob::WorkEvent6__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent7__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent8__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent9__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent10__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent11__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent12__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent13__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent14__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent15__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent16__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent17__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent18__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent19__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent20__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent21__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent22__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent23__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent24__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent25__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent26__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent27__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent28__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent29__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent30__10IWorkEventFv,
    (u32)&CLibVMBlob::WorkEvent31__10IWorkEventFv,
    (u32)&CLibVMBlob::wkUpdate__11CWorkThreadFv,
    (u32)&CLibVMBlob::wkRender__11CWorkThreadFv,
    (u32)&CLibVMBlob::wkRenderAfter__11CWorkThreadFv,
    (u32)&CLibVMBlob::wkStandbyLogin__6CLibVMFv,
    (u32)&CLibVMBlob::wkStandbyLogout__6CLibVMFv,
    (u32)&CLibVMBlob::wkStandbyExceptionRetry__11CWorkThreadFUl,
    (u32)&CLibVMBlob::__RTTI__10IWorkEvent, 0x00000000,
    (u32)&CLibVMBlob::__RTTI__11CWorkThread, 0x00000000,
    0x00000000, 0x00000000,
};

// [.sdata] 0x806637F0-0x806637F8 (8B)
extern "C" u32 lbl_eu_806637F0[2] = { (u32)&lbl_eu_8066A5B0, (u32)&lbl_eu_8056D5E8 };

// [.sbss] 0x80665720-0x80665730 (16B) zero-fill. Word 0 = CLibVM::spInstance
// (defined above); words 1-3 fill the retail 80665724(4B)+80665728(8B) block.
void (*lbl_eu_80665724)();   // +4
void (*lbl_eu_80665728)();   // +8
void (*lbl_eu_8066572C)();   // +0xC (2nd word of the 8B retail 80665728 label)
