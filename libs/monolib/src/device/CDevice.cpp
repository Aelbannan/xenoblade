#include "monolib/device.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/util.hpp"

extern "C" bool func_eu_8044A600();  // SCGetLanguage() == 0 (JP console); declared in the src-side CDeviceFont.hpp which conflicts with the include-dir headers here
#include "monolib/lib.hpp"
#include "monolib/work.hpp"

using namespace ml;


// --- Dissolved retail data (CDevice TU) ---
// Retail bytes/relocs dumped from build/us/asm/monolib/src/device/CDevice.s
// .rodata 0x80522AA8 (0x28+0x78), .data 0x8056C000 (0xA0+0x18+0xA0+0x18), .sdata 0x80663680 (0x10)
// .bss 0x806574F8 (0x44+0x44), .sbss 0x80665650 (0x10)
// Emitted as extern "C" u32 arrays with (u32)&extern, rodata align, DECOMP_FORCEACTIVE, novtable (CDevice is novtable via header).
// Local slot decls instead of including monolib/data_vtables.hpp:
// its file-scope `extern "C" void* __RTTI__10IWorkEvent/__RTTI__11CWorkThread`
// collide with MWCC's implicit RTTI names in this TU under -ipa file
// (error 10322 illegal name overloading; see MWCC_CASES CDeviceGX / LODMemMan).
extern "C" int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern "C" int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern "C" int WorkEvent3__10IWorkEventFPv(void*);
extern "C" int WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb(int);
extern "C" int WorkEvent6__10IWorkEventFv();
extern "C" int WorkEvent7__10IWorkEventFv();
extern "C" int WorkEvent8__10IWorkEventFv();
extern "C" int WorkEvent9__10IWorkEventFv();
extern "C" int WorkEvent10__10IWorkEventFv();
extern "C" int WorkEvent11__10IWorkEventFv();
extern "C" int WorkEvent12__10IWorkEventFv();
extern "C" int WorkEvent13__10IWorkEventFv();
extern "C" int WorkEvent14__10IWorkEventFv();
extern "C" int WorkEvent15__10IWorkEventFv();
extern "C" int WorkEvent16__10IWorkEventFv();
extern "C" int WorkEvent17__10IWorkEventFv();
extern "C" int WorkEvent18__10IWorkEventFv();
extern "C" int WorkEvent19__10IWorkEventFv();
extern "C" int WorkEvent20__10IWorkEventFv();
extern "C" int WorkEvent21__10IWorkEventFv();
extern "C" int WorkEvent22__10IWorkEventFv();
extern "C" int WorkEvent23__10IWorkEventFv();
extern "C" int WorkEvent24__10IWorkEventFv();
extern "C" int WorkEvent25__10IWorkEventFv();
extern "C" int WorkEvent26__10IWorkEventFv();
extern "C" int WorkEvent27__10IWorkEventFv();
extern "C" int WorkEvent28__10IWorkEventFv();
extern "C" int WorkEvent29__10IWorkEventFv();
extern "C" int WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
// CWorkThread work-method vtable slots.
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);

extern "C" {
    extern const char lbl_eu_80522AA8[];
    extern const char lbl_eu_8066A3A8[];
    extern u32 lbl_eu_80663680[];
    extern u32 lbl_eu_80663688[];
    extern u32 lbl_eu_8056C158[];
}
// Namespace-scoped extern "C" decls (CLibVM.cpp CLibVMBlob pattern): a file-scope
// declaration of __RTTI__10IWorkEvent collides with MWCC's implicit RTTI name in
// this TU under -ipa file -> error 10322 illegal name overloading.
namespace CDeviceBlob {
extern "C" void* __RTTI__10IWorkEvent;
extern "C" void* __RTTI__11CWorkThread;
}
extern "C" void wkStandbyLogin__11CWorkThreadFv();
extern "C" void wkStandbyLogin__7CDeviceFv();
extern "C" void wkStandbyLogout__7CDeviceFv();
extern "C" void __dt__7CDeviceFv();
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80522AA8[40] = "@unnamed@CDevice_cpp@::CDeviceException";
extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const u32 lbl_eu_80522AD0[30] = {
    0x43446576, 0x69636556, 0x49004344, 0x65766963, 0x65475800,
    0x43446576, 0x69636546, 0x6F6E7400, 0x43446576, 0x69636552,
    0x656D6F74, 0x65504144, 0x00434465, 0x76696365, 0x436C6F63,
    0x6B004344, 0x65766963, 0x65534300, 0x43446576, 0x69636546,
    0x696C6500, 0x434C6962, 0x43726900, 0x43446576, 0x69636545,
    0x78636570, 0x74696F6E, 0x00434465, 0x76696365, 0x00000000
};
extern "C" u32 lbl_eu_8056C000[40] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_80663680, 0x00000000, 0,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__11CWorkThreadFv,
    0, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl
};
extern "C" __declspec(section ".data") __attribute__((aligned(8))) u32 lbl_eu_8056C0A0[6] = { (u32)&CDeviceBlob::__RTTI__10IWorkEvent, 0x00000000, (u32)&CDeviceBlob::__RTTI__11CWorkThread, 0x00000000, 0x00000000, 0x00000000 };
extern "C" u32 lbl_eu_8056C0B8[40] __attribute__((aligned(8))) = {
    (u32)&lbl_eu_80663688, 0x00000000, (u32)&__dt__7CDeviceFv,
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
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__7CDeviceFv,
    (u32)&wkStandbyLogout__7CDeviceFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl
};
extern "C" __declspec(section ".data") __attribute__((aligned(8))) u32 lbl_eu_8056C158[6] = { (u32)&CDeviceBlob::__RTTI__10IWorkEvent, 0x00000000, (u32)&CDeviceBlob::__RTTI__11CWorkThread, 0x00000000, 0x00000000, 0x00000000 };
extern "C" u32 lbl_eu_80663680[2] __attribute__((aligned(8))) = { (u32)&lbl_eu_80522AA8, (u32)&lbl_eu_8056C0A0 };
extern "C" u32 lbl_eu_80663688[2] __attribute__((aligned(8))) = { (u32)&lbl_eu_8066A3A8, (u32)&lbl_eu_8056C158 };

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
            // Non-virtual class: store the retail rodata vtable manually so the
            // reloc names lbl_eu_8056C000 (CDeviceFont pattern). Retail inlines
            // this whole ctor into wkStandbyLogin.
            *(u32**)this = (u32*)lbl_eu_8056C000;
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
// Defined at file scope with C linkage (was anon-namespace -> MWCC mangled the
// symbol as lbl_eu_80665654__21@unnamed@CDevice_cpp@, drifting the reloc name).
extern "C" CDeviceException* lbl_eu_80665654 = nullptr;

CDevice* CDevice::spInstance;
// Retail sbss labels for the two TU singletons (MWCC_CASES §1a).
extern "C" {
extern CDevice* lbl_eu_80665650;             // CDevice::spInstance
}
// Retail .sdata labels for the region-name pointer pair (the class-static
// member symbols drift the reloc names; retail owns these four words as one
// .sdata run at 0x80663670).
extern "C" const char* lbl_eu_80663670 = "DeviceSystem1";  // CDevice::devSys1String
extern "C" const char* lbl_eu_80663674 = "DeviceSystem2";  // CDevice::devSys2String
//Unused strings for region names?
FixStr<64> CDevice::spNotRunningDeviceName;
FixStr<64> CDevice::spColdStartNotRunningDeviceName;
//Handles for the DeviceSystem1/DeviceSystem2 regions, which live in MEM1/MEM2 respectively.
// Retail stores these as zero-initialized .sbss slots (lbl_eu_80665658, 8B total);
// createRegions() assigns them before use, so leave them default-initialized.
extern "C" mtl::ALLOC_HANDLE lbl_eu_80663678;  // CDevice::sDeviceRegion1Handle
extern "C" mtl::ALLOC_HANDLE lbl_eu_8066367C;  // CDevice::sDeviceRegion2Handle
mtl::ALLOC_HANDLE lbl_eu_80663678;
mtl::ALLOC_HANDLE lbl_eu_8066367C;

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
    lbl_eu_80663678 = mtl::MemManager::create(mem1, deviceRegion1Size, lbl_eu_80663670);
    lbl_eu_8066367C = mtl::MemManager::create(mtl::MemManager::getHandleMEM2(), deviceRegion2Size, lbl_eu_80663674);
}

void CDevice::deleteRegions(){
    mtl::MemManager::erase(lbl_eu_80663678);
    mtl::MemManager::erase(lbl_eu_8066367C);
    lbl_eu_80663678 = mtl::INVALID_HANDLE;
    lbl_eu_8066367C = mtl::INVALID_HANDLE;
}
