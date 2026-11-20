#include "monolib/device.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util.hpp"

extern "C" bool func_eu_8044A600();  // SCGetLanguage() == 0 (JP console); declared in the src-side CDeviceFont.hpp which conflicts with the include-dir headers here
#include "monolib/lib.hpp"
#include "monolib/work.hpp"

using namespace ml;


// The name buffers are retail-owned globals (JP symbols.txt names them
// spNotRunningDeviceName / spColdStartNotRunningDeviceName); everything else
// in this TU (vtables, RTTI records, base lists and type-name strings) is
// emitted by MWCC from the real CDevice / CDeviceException hierarchy.

// Inline copy of CWorkThread::isRunning() visible only in this TU so the
// retail inline shape (no bl) reproduces in isInitialized/isAllReady.
// CWorkRoot.cpp keeps the strong out-of-line definition.
inline bool CWorkThread::isRunning() const {
    bool exception;
    if (mFlags & THREAD_FLAG_EXCEPTION) {
        exception = true;
    } else {
        exception = (mMsgQueue.find(EVT_EXCEPTION) >= 0);
    }

    bool result = false;
    if (!exception) {
        bool stateOK = (mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN);
        if (stateOK) {
            result = true;
        }
    }
    return result;
}


// Retail keeps this class TU-local (anonymous namespace).
namespace {
    class CDeviceException;
}
// File-scope C-linkage declaration: an anon-namespace declaration of this name
// makes MWCC emit/reference the @unnamed-mangled symbol instead of the retail
// lbl_eu_80665654 (reloc-name drift).
extern "C" CDeviceException* lbl_eu_80665654;
namespace {
    class CDeviceException;
    //size: 0x1c8
    class CDeviceException : public CWorkThread {
    public:
        CDeviceException(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD) {
            lbl_eu_80665654 = this;
        }
        ~CDeviceException(){ lbl_eu_80665654 = nullptr; }
        virtual bool wkStandbyLogout() {
            if (mChildren.empty() == false) return false;
            return CWorkThread::wkStandbyLogout();
        }
        static CDeviceException* getInstance(){ return lbl_eu_80665654; }

        DECL_WORKTHREAD_CREATE(CDeviceException);

        //0x0: vtable
        //0x0-1c4: CWorkThread
        u32 unk1C4;

    private:
        static const int MAX_CHILD = 64;
        // static CDeviceException* spInstance; -> extern "C" lbl_eu_80665654 below
    };
}
// Read-only mirror of the private CMsgParam<8> tail inside CWorkThread
// (queue internals at 0x1A4-0x1B0): lets isInitialized run the retail's fully
// inlined EVT_EXCEPTION ring scan without an out-of-line find() call
// (CLibG3d / CDeviceSC pattern).
struct CDeviceMsgQueueData {
    u8 pad[0x48];                // vtable + thread header
    int mState;                  //0x48 (CWorkThread::mState, signed compare)
    u8 pad2[0x5C - 0x4C];        //0x4C
    reslist<CWorkThread*> mChildren; //0x5C (CWorkThread::mChildren)
    u32 mThreadFlags;            //0x7C (CWorkThread::mFlags)
    u8 pad3[0x1A4 - 0x80];       //0x80..0x1A4 (queue vtable + entries)
    CMsgParamEntry* mArrayPtr;   // 0x1A4 (mMsgQueue.mArrayPtr)
    u32 mFront;                  // 0x1A8 (mMsgQueue.mFront)
    u32 mSize;                   // 0x1AC (mMsgQueue.mSize)
    u32 mCapacity;               // 0x1B0 (mMsgQueue.mCapacity)
    u8 pad4[0x1C4 - 0x1B4];      //0x1B4
    u32 unk1C4;                  // 0x1C4 (CDeviceBase::mFlags)
};

// Retail .sbss run, in address order: 0x80665650 (the CDevice singleton),
// 0x80665654 (the exception singleton) and the 8-byte slot at 0x80665658.
CDevice* CDevice::spInstance;
// Defined at file scope with C linkage (was anon-namespace -> MWCC mangled the
// symbol as lbl_eu_80665654__21@unnamed@CDevice_cpp@, drifting the reloc name).
extern "C" CDeviceException* lbl_eu_80665654 = nullptr;
extern "C" { u64 lbl_eu_80665658; }

extern "C" {
extern CDevice* lbl_eu_80665650;             // CDevice::spInstance
}

// The region-name pointers are owned by another TU; keep them imported.
extern "C" const char* lbl_eu_80663670;  // CDevice::devSys1String
extern "C" const char* lbl_eu_80663674;

FixStr<64> spNotRunningDeviceName;
FixStr<64> spColdStartNotRunningDeviceName;
//Handles for the DeviceSystem1/DeviceSystem2 regions, which live in MEM1/MEM2 respectively.
// The retail slots at 0x80663678/0x8066367C precede this TU's .sdata slice;
// createRegions() assigns them through the imported names.
extern "C" mtl::ALLOC_HANDLE lbl_eu_80663678;  // CDevice::sDeviceRegion1Handle
extern "C" mtl::ALLOC_HANDLE lbl_eu_8066367C;  // CDevice::sDeviceRegion2Handle

CDevice::~CDevice(){
    spInstance = nullptr;
}

CDevice* CDevice::getInstance(){
    return spInstance;
}

int CDevice::getDevSys1Handle(){
    return lbl_eu_80663678;   // sDeviceRegion1Handle
}

int CDevice::getDevSys2Handle(){
    return lbl_eu_8066367C;   // sDeviceRegion2Handle
}

// Intrusive child-list shapes used by the fully-inlined isReady scans below.
// mChildren is a circular std::list-style list: the sentinel node pointer sits
// at CWorkThread+0x60, nodes are {next, prev, value}.
struct CDeviceChildNode {
    CDeviceChildNode* next;     //0x0
    void* prev;                 //0x4
    CWorkThread* thread;        //0x8
};
struct CDeviceChildList {
    u8 pad[0x60];
    CDeviceChildNode* sentinel; //0x60
};

bool CDevice::isAllReady(){
    // Retail inlines isRunning() here and re-reads the instance from SDA in
    // the loop condition (the child pointer reuses the cached register).
    // Retail caches the ring-scan counter in its own reg ahead of the SDA
    // singleton load.
    int i;
    int foundIndex;
    const CDeviceMsgQueueData* q = reinterpret_cast<const CDeviceMsgQueueData*>(lbl_eu_80665650);

    // Inlined exception check + ring scan (same shape as isInitialized).
    bool busy;
    if(q->mThreadFlags & THREAD_FLAG_EXCEPTION){
        busy = true;
    } else {
        for(i = 0; i < q->mSize; i++){
            if(q->mArrayPtr[(q->mFront + i) % q->mCapacity].command == EVT_EXCEPTION){
                foundIndex = i;
                goto done;
            }
        }
        foundIndex = -1;
    done:
        busy = foundIndex >= 0;
    }

    // isRunning() tail: not waiting on an exception and logged in / running.
    bool running = !busy &&
        (q->mState == THREAD_STATE_LOGIN || q->mState == THREAD_STATE_RUN);
    if(!running) return false;

    // Declaration order drives callee-saved coloring (first declared -> r31):
    // retail holds the node pointer in r31 and the accumulator in r30.
    CDeviceChildNode* node;
    bool result;
    result = true;

    // begin() folds the sentinel load into a scratch reg; the end sentinel is
    // re-read through the SDA singleton every iteration (the name copy may
    // alias-mutate the list, so MWCC cannot cache it).
    node = reinterpret_cast<const CDeviceChildList*>(q)->sentinel->next;
    for(;
        node != reinterpret_cast<const CDeviceChildList*>(lbl_eu_80665650)->sentinel;
        node = node->next)
    {
        CWorkThread* thread = node->thread;
        const CDeviceMsgQueueData* tq = reinterpret_cast<const CDeviceMsgQueueData*>(thread);

        // Inlined exception check + ring scan.
        bool tBusy;
        if(tq->mThreadFlags & THREAD_FLAG_EXCEPTION){
            tBusy = true;
        } else {
            for(i = 0; i < tq->mSize; i++){
                if(tq->mArrayPtr[(tq->mFront + i) % tq->mCapacity].command == EVT_EXCEPTION){
                    foundIndex = i;
                    goto tdone;
                }
            }
            foundIndex = -1;
        tdone:
            tBusy = foundIndex >= 0;
        }

        // isRunning() tail: not waiting on an exception and logged in / running.
        bool tRunning = !tBusy &&
            (tq->mState == THREAD_STATE_LOGIN || tq->mState == THREAD_STATE_RUN);

        //If a device that isn't running is found, save its name
        if(!tRunning){
            spNotRunningDeviceName = thread->mName.c_str();
        }

        result = result & tRunning;
    }

    return result;
}

bool CDevice::isColdStartReady(){
    // Same fully-inlined shape as isAllReady: exception ring scan + state test
    // on the singleton first, then a walk of the child list.
    int i;
    int foundIndex;
    const CDeviceMsgQueueData* q = reinterpret_cast<const CDeviceMsgQueueData*>(lbl_eu_80665650);

    // Inlined exception check + ring scan.
    bool busy;
    if(q->mThreadFlags & THREAD_FLAG_EXCEPTION){
        busy = true;
    } else {
        for(i = 0; i < q->mSize; i++){
            if(q->mArrayPtr[(q->mFront + i) % q->mCapacity].command == EVT_EXCEPTION){
                foundIndex = i;
                goto done;
            }
        }
        foundIndex = -1;
    done:
        busy = foundIndex >= 0;
    }

    // isRunning() tail: not waiting on an exception and logged in / running.
    bool running = !busy &&
        (q->mState == THREAD_STATE_LOGIN || q->mState == THREAD_STATE_RUN);
    if(!running) return false;

    // Node pointer walks the list; the end sentinel is re-read through the SDA
    // singleton every iteration (the name copy may alias-mutate the list).
    // Node pointer walks the list; the end sentinel is re-read through the SDA
    // singleton every iteration (the name copy may alias-mutate the list).
    CDeviceChildNode* node;
    bool result;
    result = true;

    node = reinterpret_cast<const CDeviceChildList*>(q)->sentinel->next;
    for(;
        node != reinterpret_cast<const CDeviceChildList*>(lbl_eu_80665650)->sentinel;
        node = node->next)
    {
        //BUG: no check that cast is valid - skip devices that were never created.
        //BUG: no check that cast is valid - skip devices that were never created.
        if(!static_cast<CDeviceBase*>(node->thread)->CDeviceBase_inline2()) continue;
        CWorkThread* thread = node->thread;
        const CDeviceMsgQueueData* tq = reinterpret_cast<const CDeviceMsgQueueData*>(thread);
        if(!(static_cast<CDeviceBase*>(thread)->CDeviceBase_inline2())) continue;

        // Inlined exception check + ring scan.
        bool tBusy;
        if(tq->mThreadFlags & THREAD_FLAG_EXCEPTION){
            tBusy = true;
        } else {
            for(i = 0; i < tq->mSize; i++){
                if(tq->mArrayPtr[(tq->mFront + i) % tq->mCapacity].command == EVT_EXCEPTION){
                    foundIndex = i;
                    goto tdone;
                }
            }
            foundIndex = -1;
        tdone:
            tBusy = foundIndex >= 0;
        }

        // isRunning() tail: not waiting on an exception and logged in / running.
        bool tRunning = !tBusy &&
            (tq->mState == THREAD_STATE_LOGIN || tq->mState == THREAD_STATE_RUN);

        //If a cold-start device isn't running yet, save its name.
        if(!tRunning){
            spColdStartNotRunningDeviceName = thread->mName.c_str();
        }

        result = result & tRunning;
    }

    return result;
}

bool CDevice::isInitialized(){
    // Retail caches the SDA singleton load in a callee-saved reg for the whole
    // frameless body; the ring-scan counter takes the other one.
    int i;
    int foundIndex;
    // Retail caches the SDA singleton load in a callee-saved reg for the whole
    // frameless body; the ring-scan counter takes the other one.
    const CDeviceMsgQueueData* q = reinterpret_cast<const CDeviceMsgQueueData*>(lbl_eu_80665650);

    // Inlined exception check + ring scan.
    bool busy;
    if(q->mThreadFlags & THREAD_FLAG_EXCEPTION){
        busy = true;
    } else {
        for(i = 0; i < q->mSize; i++){
            if(q->mArrayPtr[(q->mFront + i) % q->mCapacity].command == EVT_EXCEPTION){
                foundIndex = i;
                goto done;
            }
        }
        foundIndex = -1;
    done:
        busy = foundIndex >= 0;
    }

    // isRunning() tail: not waiting on an exception and logged in / running.
    bool running = !busy &&
        (q->mState == THREAD_STATE_LOGIN || q->mState == THREAD_STATE_RUN);
    if(!running) return false;

    bool result = true;

    for(reslist<CWorkThread*>::iterator it = q->mChildren.begin(); it != q->mChildren.end(); it++){
        CDeviceBase* deviceBase = static_cast<CDeviceBase*>(*it);
        if(!(deviceBase->mFlags & CDeviceBase::FLAG_CREATED)) result = false;
    }

    return result;
}

// NOTE (open item): decomp's CDeviceVI is 4 bytes short (sizeof 0x2c4); retail
// allocates 0x2c8 per device (likely one more tail word / align-8 member in the
// retail class). Fixing it needs a one-line CDeviceVI.hpp change (add
// `u32 unk2C4;` at the class tail), which is outside this session's writable
// scope; initDevices therefore keeps one size-constant diff on the VI block.
// Placement-new overload letting a call site pass the allocation size
// explicitly: used by initDevices because retail allocates 0x2c8 bytes for
// CDeviceVI while the frozen shared header layout sums to sizeof 0x2c4.
inline void* operator new(size_t size, mtl::ALLOC_HANDLE handle, size_t overrideSize){
    return mtl::MemManager::allocate(overrideSize, handle);
}

// Mirror of the CDeviceVI DECL_WORKTHREAD_CREATE body with the retail 0x2c8
// allocation size passed explicitly (the shared header layout is 4 bytes
// short); the parameterized inline keeps the retail register allocation.
static inline CDeviceVI* createDeviceVI(const char* pName, CWorkThread* pParent){
    CDeviceVI* device = new (CWorkThreadSystem::getWorkMem(), 0x2C8) CDeviceVI(pName, pParent);
    CWorkUtil::entryWork(device, pParent, false);
    device->mFlags |= CDeviceBase::FLAG_CREATED;
    return device;
}

void CDevice::initDevices(){
    // Device creation order fixes the TU literal-pool layout (retail
    // lbl_eu_80522AD0): VI, GX, Font, RemotePAD, Clock, SC, File, Cri.
    // VI/GX/Clock/SC use custom create() bodies that set FLAG_CREATED after
    // entryWork; Font/RemotePad/File/Cri use the plain WORKTHREAD macro.
    if(CDeviceVI::getInstance() == nullptr){
        // Retail allocates 0x2c8 bytes for CDeviceVI here; see the operator
        // new overload above (the shared header layout is 4 bytes short).
        createDeviceVI("CDeviceVI", lbl_eu_80665650);
    }
    if(CDeviceGX::getInstance() == nullptr){
        CDeviceGX::create("CDeviceGX", lbl_eu_80665650);
    }
    if(CDeviceFont::getInstance() == nullptr){
        CDeviceFont::create("CDeviceFont", lbl_eu_80665650);
    }
    if(CDeviceRemotePad::getInstance() == nullptr){
        CDeviceRemotePad::create("CDeviceRemotePAD", lbl_eu_80665650);
    }
    if(CDeviceClock::getInstance() == nullptr){
        CDeviceClock::create("CDeviceClock", lbl_eu_80665650);
    }
    if(CDeviceSC::getInstance() == nullptr){
        CDeviceSC::create("CDeviceSC", lbl_eu_80665650);
    }
    if(CDeviceFile::getInstance() == nullptr){
        CDeviceFile::create("CDeviceFile", lbl_eu_80665650);
    }
    if(CLibCri::getInstance() == nullptr){
        CLibCri::create("CLibCri", lbl_eu_80665650);
    }

    //Feels a bit strange to put this in CDeviceGX
    CDeviceGX::setDevicesInitializedFlag(true);
}


bool CDevice::wkStandbyLogin(){
    // "CDeviceException" is a pooled literal in retail; the shared pool base
    // (lbl_eu_80522AD0) places it at +0x5c after the initDevices strings.
    CDeviceException::create("CDeviceException", this);
    CDevice::initDevices();
    this->wkSetEvent(EVT_9);
    return CWorkThread::wkStandbyLogin();
}

bool CDevice::wkStandbyLogout(){
    if(mChildren.empty() && CWorkSystem::getInstance() == nullptr
    && CLib::getInstance() == nullptr){
        return CWorkThread::wkStandbyLogout();
    }

    return false;
}

CDevice* CDevice::create(){
    // "CDevice" joins the TU string pool (@stringBase0 -> retail
    // lbl_eu_80522AD0) at +0x6d.
    return CDevice::create("CDevice", CWorkControl::getInstance());
}

void CDevice::createRegions(){
    //TODO: what is the extra 0x80?
    int deviceRegion1Size = CDeviceGX::getHeapSize() + CDeviceFontLayer::getFontHeapSize() + 0x80;
    deviceRegion1Size += CDeviceVI::usingStaticHandle() ? 0 : CDeviceVI::getXfbBuffersSize();
    // Region2 is 0x40000, bumped to 0x110000 on a JP (language 0) console.
    int deviceRegion2Size = 0x40000;
    if (func_eu_8044A600()) {
        deviceRegion2Size = 0x110000;
    }
    int mem1 = mtl::MemManager::getHandleMEM1();
    lbl_eu_80663678 = mtl::MemManager::create(mem1, deviceRegion1Size, lbl_eu_80663670);
    lbl_eu_8066367C = mtl::MemManager::create(mtl::MemManager::getHandleMEM2(), deviceRegion2Size, lbl_eu_80663674);
}

void CDevice::deleteRegions(){
    mtl::MemManager::erase(lbl_eu_80663678);
    mtl::MemManager::erase(lbl_eu_8066367C);
    lbl_eu_80663678 = mtl::INVALID_HANDLE;
    lbl_eu_8066367C = mtl::INVALID_HANDLE;
}
