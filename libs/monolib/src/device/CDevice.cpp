#include "monolib/device.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util.hpp"

extern "C" bool func_eu_8044A600();  // SCGetLanguage() == 0 (JP console); declared in the src-side CDeviceFont.hpp which conflicts with the include-dir headers here
#include "monolib/lib.hpp"
#include "monolib/work.hpp"

using namespace ml;


// Retail data-symbol imports for the anonymous-ns CDeviceException: its
// vtable (.data, monolibdata blob) and the shared string pool that holds
// "CDeviceException" at +0x5c. Global-scope names are not mangled, so no
// extern "C" is needed.
extern u32 lbl_eu_8056C000[];
extern const char lbl_eu_80522AD0[];
// Retail name of the spNotRunningDeviceName FixStr (.bss, monolibdata blob).
extern FixStr<64> lbl_eu_806574F8;

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


// Forward decl for the anonymous-ns singleton (defined extern "C" below).
namespace {
    class CDeviceException;
}
extern "C" extern CDeviceException* lbl_eu_80665654;

namespace {
    //size: 0x1c8
    class CDeviceException : public CWorkThread {
    public:
        CDeviceException(const char* pName, CWorkThread* pParent) : CWorkThread(pName, pParent, MAX_CHILD) {
            // Non-virtual class: store the retail rodata vtable manually so the
            // reloc names lbl_eu_8056C000 (CDeviceFont pattern). Retail inlines
            // this whole ctor into wkStandbyLogin.
            *(u32**)this = (u32*)lbl_eu_8056C000;
            lbl_eu_80665654 = this;
        }
        ~CDeviceException();
        bool wkStandbyLogout();
        static CDeviceException* getInstance();

        DECL_WORKTHREAD_CREATE(CDeviceException);

        //0x0: vtable
        //0x0-1c4: CWorkThread
        u32 unk1C4;

    private:
        static const int MAX_CHILD = 64;
        // static CDeviceException* spInstance; -> extern "C" lbl_eu_80665654 below
    };
}

CDevice* CDevice::spInstance;
extern "C" CDeviceException* lbl_eu_80665654;

// Retail sbss labels for the two TU singletons (MWCC_REFERENCE §1a).
extern "C" {
extern CDevice* lbl_eu_80665650;             // CDevice::spInstance
}
const char* CDevice::devSys1String = "DeviceSystem1";
const char* CDevice::devSys2String = "DeviceSystem2";
//Unused strings for region names?
FixStr<64> CDevice::spNotRunningDeviceName;
FixStr<64> CDevice::spColdStartNotRunningDeviceName;
//Handles for the DeviceSystem1/DeviceSystem2 regions, which live in MEM1/MEM2 respectively
mtl::ALLOC_HANDLE CDevice::sDeviceRegion1Handle = mtl::INVALID_HANDLE;
mtl::ALLOC_HANDLE CDevice::sDeviceRegion2Handle = mtl::INVALID_HANDLE;

CDevice::~CDevice(){
    spInstance = nullptr;
}

CDevice* CDevice::getInstance(){
    return spInstance;
}

int CDevice::getDevSys1Handle(){
    return sDeviceRegion1Handle;
}

int CDevice::getDevSys2Handle(){
    return sDeviceRegion2Handle;
}

bool CDevice::isAllReady(){
    // Retail inlines isRunning() here and re-reads the instance from SDA in
    // the loop condition (the child pointer reuses the cached register).
    if(!lbl_eu_80665650->isRunning()) return false;

    bool result = true;

    for(reslist<CWorkThread*>::iterator it = lbl_eu_80665650->mChildren.begin(); it != lbl_eu_80665650->mChildren.end(); it++){
        CWorkThread* thread = *it;

        bool running = thread->isRunning();

        //If a device that isn't running is found, save its name
        if(!running){
            // Keep the name pointer in a local so MWCC hoists it into a
            // callee-saved register across the strlen call (retail r27).
            const char* name = thread->mName.c_str();
            lbl_eu_806574F8 = name;
        }

        result &= running;
    }

    return result;
}

bool CDevice::isColdStartReady(){
    if(!spInstance->isRunning()) return false;

    bool result = true;

    for(reslist<CWorkThread*>::iterator it = spInstance->mChildren.begin(); it != spInstance->mChildren.end(); it++){
        //BUG: no check that cast is valid
        CDeviceBase* device = static_cast<CDeviceBase*>(*it);

        if(device->CDeviceBase_inline2()){
            bool running = device->isRunning();

            if(!running){
                const char* name = device->mName.c_str();
                spColdStartNotRunningDeviceName = name;
            }

            result &= running;

        }

    }

    return result;
}

bool CDevice::isInitialized(){
    // Retail inlines isRunning() here; keep the instance in a local so the
    // single SDA load is cached for the whole function (retail holds it in r7).
    CDevice* instance = lbl_eu_80665650;
    if(!instance->isRunning()) return false;

    bool result = true;

    for(reslist<CWorkThread*>::iterator it = instance->mChildren.begin(); it != instance->mChildren.end(); it++){
        CDeviceBase* deviceBase = static_cast<CDeviceBase*>(*it);
        if(!(deviceBase->mFlags & CDeviceBase::FLAG_CREATED)) result = false;
    }

    return result;
}

void CDevice::initDevices(){
    if(CDeviceVI::getInstance() == nullptr){
        CDeviceVI::create("CDeviceVI", spInstance);
    }
    if(CDeviceGX::getInstance() == nullptr){
        CDeviceGX::create("CDeviceGX", spInstance);
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
    if(CDeviceFont::getInstance() == nullptr){
        CDeviceFont::create("CDeviceFont", spInstance);
    }
    if(CDeviceFile::getInstance() == nullptr){
        CDeviceFile::create("CDeviceFile", spInstance);
    }
    if(CLibCri::getInstance() == nullptr){
        CLibCri::create("CLibCri", spInstance);
    }

    //Feels a bit strange to put this in CDeviceGX
    CDeviceGX::setDevicesInitializedFlag(true);

    // Keep the unreferenced retail getInstance__...CDeviceExceptionFv symbol
    // emitted (MWCC drops unreferenced internal-linkage functions under -ipa).
    (void)CDeviceException::getInstance();
}

#pragma dont_inline on
CDeviceException* CDeviceException::getInstance(){
    // Retail SDA reloc is lbl_eu_80665654@sda21.
    return lbl_eu_80665654;
}
#pragma dont_inline off

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
    return create("CDevice", CWorkControl::getInstance());
}

void CDevice::createRegions(){
    //TODO: what is the extra 0x80?
    int deviceRegion1Size = CDeviceGX::getHeapSize() + CDeviceFontLayer::func_80454E78() + 0x80;
    deviceRegion1Size += CDeviceVI::usingStaticHandle() ? 0 : CDeviceVI::getXfbBuffersSize();
    // Region2 is 0x40000, bumped to 0x110000 on a JP (language 0) console.
    int deviceRegion2Size = 0x40000;
    if (func_eu_8044A600()) {
        deviceRegion2Size = 0x110000;
    }
    int mem1 = mtl::MemManager::getHandleMEM1();
    sDeviceRegion1Handle = mtl::MemManager::create(mem1, deviceRegion1Size, devSys1String);
    sDeviceRegion2Handle = mtl::MemManager::create(mtl::MemManager::getHandleMEM2(), deviceRegion2Size, devSys2String);
}

void CDevice::deleteRegions(){
    mtl::MemManager::erase(sDeviceRegion1Handle);
    mtl::MemManager::erase(sDeviceRegion2Handle);
    sDeviceRegion1Handle = mtl::INVALID_HANDLE;
    sDeviceRegion2Handle = mtl::INVALID_HANDLE;
}

CDeviceException::~CDeviceException(){
    lbl_eu_80665654 = nullptr;
}

bool CDeviceException::wkStandbyLogout(){
    if(mChildren.empty() == false) return false;
    return CWorkThread::wkStandbyLogout();
}
