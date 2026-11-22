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

// Retail inlines CMsgParam<8>::find (and through it CWorkThread::isException /
// isRunning) into this TU, while CWorkRoot.cpp keeps the strong out-of-line
// find (same TU-local inline specialization as CLibHbm.cpp).
template <>
inline int CMsgParam<8>::find(u32 msg) const {
    for (int i = 0; i < mRing.mSize; i++) {
        if (mRing.mArrayPtr[(mRing.mFront + i) % mRing.mCapacity].command == msg) {
            return i;
        }
    }
    return -1;
}

inline bool CWorkThread::isRunning() const {
    return !isException() && (mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN);
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
// Retail .sbss run, in address order: CDevice::spInstance,
// the exception singleton and CDeviceClock::spInstance.
CDevice* CDevice::spInstance;
// Defined at file scope with C linkage (was anon-namespace -> MWCC mangled the
// symbol as lbl_eu_80665654__21@unnamed@CDevice_cpp@, drifting the reloc name).
extern "C" CDeviceException* lbl_eu_80665654 = nullptr;
CDeviceClock* CDeviceClock::spInstance;

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

bool CDevice::isAllReady(){
    if(!spInstance->isRunning()) return false;

    bool result = true;
    for(reslist<CWorkThread*>::iterator it = spInstance->mChildren.begin(); it != spInstance->mChildren.end(); it++){
        CWorkThread* thread = *it;
        bool running = thread->isRunning();
        //If a device that isn't running is found, save its name
        if(!running){
            spNotRunningDeviceName = thread->mName.c_str();
        }
        result = result & running;
    }

    return result;
}

bool CDevice::isColdStartReady(){
    if(!spInstance->isRunning()) return false;

    bool result = true;
    for(reslist<CWorkThread*>::iterator it = spInstance->mChildren.begin(); it != spInstance->mChildren.end(); it++){
        //BUG: no check that cast is valid - skip devices that were never created.
        if(!static_cast<CDeviceBase*>(*it)->CDeviceBase_inline2()) continue;
        CWorkThread* thread = *it;
        if(!static_cast<CDeviceBase*>(thread)->CDeviceBase_inline2()) continue;
        bool running = thread->isRunning();
        //If a cold-start device isn't running yet, save its name.
        if(!running){
            spColdStartNotRunningDeviceName = thread->mName.c_str();
        }
        result = result & running;
    }

    return result;
}

bool CDevice::isInitialized(){
    if(!spInstance->isRunning()) return false;

    bool result = true;
    for(reslist<CWorkThread*>::iterator it = spInstance->mChildren.begin(); it != spInstance->mChildren.end(); it++){
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
        createDeviceVI("CDeviceVI", spInstance);
    }
    if(CDeviceGX::getInstance() == nullptr){
        CDeviceGX::create("CDeviceGX", spInstance);
    }
    if(CDeviceFont::getInstance() == nullptr){
        CDeviceFont::create("CDeviceFont", spInstance);
    }
    if(CDeviceRemotePad::getInstance() == nullptr){
        CDeviceRemotePad::create("CDeviceRemotePAD", spInstance);
    }
    if(CDeviceClock::getInstance() == nullptr){
        CDeviceClock::create("CDeviceClock", spInstance);
    }
    if(CDeviceSC::getInstance() == nullptr){
        CDeviceSC::create("CDeviceSC", spInstance);
    }
    if(CDeviceFile::getInstance() == nullptr){
        CDeviceFile::create("CDeviceFile", spInstance);
    }
    if(CLibCri::getInstance() == nullptr){
        CLibCri::create("CLibCri", spInstance);
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
