#include "CLibG3d.hpp"
#include "monolib/data_vtables.hpp"
#include <nw4r/g3d/g3d_init.h>
#include <nw4r/g3d/g3d_state.h>

// The device/procroot headers pull in classes with inline virtual dtors that,
// combined with this TU's sbss globals, trip MWCC 10322 once the manual vtable
// declares extern __RTTI__. Those statics are all out-of-line, so forward-
// declare them instead of including the headers.
class CDeviceVI;
class CProcRoot;
extern "C" int isInitialized__9CDeviceGXFv();
extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
extern "C" CProcRoot* getInstance__9CProcRootFv();

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

// Forward decls for the data block at the bottom (CLib.cpp pattern).
extern "C" u32 __vt__7CLibG3d[];   // retail vtable symbol (symbols.txt .data:0x8056D0F0)
extern "C" void __dt__7CLibG3dFv();
extern "C" void wkStandbyLogin__7CLibG3dFv();
extern "C" void wkStandbyLogout__7CLibG3dFv();

CLibG3d::CLibG3d(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, 4) {
    *(void**)this = (void*)__vt__7CLibG3d;
    lbl_eu_806656F0[0] = this;
    mType = THREAD_CLIBG3D;
}

CLibG3d::~CLibG3d() {
    lbl_eu_806656F0[0] = nullptr;
}

CLibG3d* CLibG3d::getInstance() {
    return lbl_eu_806656F0[0];
}

bool CLibG3d::isInitialized() {
    // G3D counts as "not initialized" while an exception is pending, or while
    // an EVT_EXCEPTION message is still queued waiting to be processed.
    CLibG3d* inst = lbl_eu_806656F0[0];
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
    if (isInitialized__9CDeviceGXFv()) {
        nw4r::g3d::G3dInit(true);
        nw4r::g3d::G3DState::SetRenderModeObj(*getRenderModeObj__9CDeviceVIFv());
        return CWorkThread::wkStandbyLogin();
    }
    return false;
}

bool CLibG3d::wkStandbyLogout() {
    if (mChildren.empty() && getInstance__9CProcRootFv() == nullptr) {
        return CWorkThread::wkStandbyLogout();
    }
    return false;
}

// ===== Dissolved monolibdata2 data owned by this TU =====
// RTTI class-name / typeinfo references (foreign): the CLibG3d name string
// (.sdata2 lbl_eu_8066A528) and the two .rodata name strings live in other
// units' retail ranges; extern them rather than re-defining.
extern "C" u32 lbl_eu_8066A528;      // .sdata2 "CLibG3d" name string (foreign)
extern "C" u32 lbl_eu_80523118;      // .rodata RTTI name (foreign)
extern "C" u32 lbl_eu_80523128;      // .rodata RTTI name (foreign)
extern "C" u32 lbl_eu_806637B0[2];   // own .sdata RTTI locator
extern "C" u32 lbl_eu_8056D190[6];   // own .data base-list

// [.data] 0x8056D0F0-0x8056D19x (0xA0): __vt__7CLibG3d.
// Base class CWorkThread is novtable, so CLibG3d's vtable/RTTI are manual.
extern "C" u32 __vt__7CLibG3d[40] = {
    (u32)&lbl_eu_806637B0, 0x00000000, (u32)&__dt__7CLibG3dFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__7CLibG3dFv,
    (u32)&wkStandbyLogout__7CLibG3dFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
// RTTI base list [IWorkEvent,0, CWorkThread,0, 0,0] (0x18).
extern "C" u32 lbl_eu_8056D190[6] = {
    (u32)&__RTTI__10IWorkEvent, 0x00000000, (u32)&__RTTI__11CWorkThread,
    0x00000000, 0x00000000, 0x00000000,
};

// [.sdata] 0x806637B0-0x806637C8 (0x18): RTTI locator block.
extern "C" u32 lbl_eu_806637B0[2] = { (u32)&lbl_eu_8066A528, (u32)&lbl_eu_8056D190 };
extern "C" u32 lbl_eu_806637B8 = (u32)&lbl_eu_80523118;
extern "C" u32 lbl_eu_806637BC = (u32)&lbl_eu_80523128;
extern "C" u32 lbl_eu_806637C0[2] = { 0xFFFFFFFF, 0x00000000 };

// [.sbss] 0x806656F0-0x80665700 (0x10 zero-fill; only size/align are compared).
// lbl_eu_806656F0 is the CLibG3d singleton; the following words pad the
// section to the retail 0x10 span.
CLibG3d* lbl_eu_806656F0[2];
u32 lbl_eu_806656F8[2];
