// CLibCri - CRI middleware manager for monolib
// Decompiled high-level C++ from retail assembly

#include "monolib/lib/CLibCri.hpp"
#include "monolib/lib/CLibCriMoviePlay.hpp"
#include "monolib/lib/CLibCriStreamingPlay.hpp"
#include "monolib/device/CDevice.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/work/CWorkUtil.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <revolution/ax/AX.h>
#include <revolution/ax/AXOut.h>
#include <revolution/ai/ai.h>
#include <cstring>

// CRI middleware extern declarations
extern "C" {
    void MIXUpdateSettings(void);
    void MIXInit(void);
    void MIXQuit(void);
    void ADXM_SetCbErr(void (*func)(), void* arg);
    void ADXWII_SetupDvdFs(int mode);
    void ADXM_SetupFramework(int maxSounds, int reserved);
    void ADXT_SetDefSvrFreq(int freq);
    void ADXT_Init(void);
    void ADXT_Finish(void);
    void ADXM_ShutdownFramework(void);
    void ADXM_ExecMain(void);
    void AXQuit(void);
}

// CLibCriStreamingPlay factory function (static, 8 args)
// Called from func_8045997C with various parameters
extern "C" int func_8045B5AC(
    const char* filename,
    u32 allocHandle,
    int fileHandle,
    int arg3,
    int arg4,
    bool useExtended,
    int arg6,
    int arg7
);

// CWorkSystemPack stubs
extern "C" {
    int func_804DE010(const char* ext);
    void func_804DDD54(const char* ext, const char* filename,
                       int* out1, int* out2, int* out3, int* out4);
}

// Singleton instance pointer (sda21-relative: lbl_eu_806656D8)
CLibCri* CLibCri::spInstance;

// AX callback function pointer (sda21-relative: lbl_eu_806656DC)
static void (*sAXCallback)(void);

// Free function wrapper for AX callback registration
static void sAXCallbackWrapper() {
    CLibCri* instance = CLibCri::getInstance();
    if (instance != nullptr) {
        instance->func_80459830();
    }
}

// String constant (CRI file extension marker: lbl_eu_80522FD8)
extern "C" const char lbl_eu_80522FD8[];

// Empty error callback (free function)
extern "C" void func_80459C70() {}

// Forwarding stubs to CLibCriStreamingPlay
extern "C" {
    void func_8045B970__20CLibCriStreamingPlayFv();
    void func_8045BAB0__20CLibCriStreamingPlayFv();
    void func_8045BBA0__20CLibCriStreamingPlayFv();
    void func_8045BC4C__20CLibCriStreamingPlayFv();
    void func_8045CA4C__20CLibCriStreamingPlayFv();
    void func_8045C67C__20CLibCriStreamingPlayFv();
    void func_8045C700__20CLibCriStreamingPlayFv();
    void func_8045C8B0__20CLibCriStreamingPlayFv();
    void func_8045BE48__20CLibCriStreamingPlayFv();
    void func_8045CCFC__20CLibCriStreamingPlayFv();
    void func_8045CFDC__20CLibCriStreamingPlayFv();
    void func_8045D03C__20CLibCriStreamingPlayFv();
}

// Forwarding stubs to CLibCriMoviePlay
extern "C" {
    void func_8045A260__16CLibCriMoviePlayFv();
    void func_8045A48C__16CLibCriMoviePlayFv();
    void func_8045B310__16CLibCriMoviePlayFv();
    void func_8045A708__16CLibCriMoviePlayFv();
    void func_8045A7F8__16CLibCriMoviePlayFv();
    void func_8045A644__16CLibCriMoviePlayFv();
    void func_8045A8C8__16CLibCriMoviePlayFv();
    void func_8045B1E0__16CLibCriMoviePlayFv();
}

// ============================================================================
// CLibCri AX callback handler
// ============================================================================
void CLibCri::func_80459830() {
    MIXUpdateSettings();
    if (sAXCallback != nullptr) {
        sAXCallback();
    }
}

// ============================================================================
// CLibCri constructor
// ============================================================================
CLibCri::CLibCri(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 2)
{
    CDeviceVICb();
    mType = THREAD_CLIBCRI; // 0xF
    spInstance = this;
    CErrorWii::addCallback(static_cast<IErrorWii*>(this));
}

// ============================================================================
// CLibCri destructor
// ============================================================================
CLibCri::~CLibCri() {
    if (this == nullptr) return;
    CErrorWii::removeCallback(static_cast<IErrorWii*>(this));
    spInstance = nullptr;
}

// ============================================================================
// File playback dispatch function
// ============================================================================
int CLibCri::func_8045997C(const char* filename, u32 allocHandle, int fileHandle) {
    if (allocHandle == (u32)-1) {
        allocHandle = mtl::MemManager::getHandleMEM2();
    }

    const char* ext = ml::CPathUtil::getFileExtPtr(filename);
    if (ext == nullptr) {
        return -1;
    }

    if (func_804DE010(ext) != 0) {
        int out1, out2, out3, out4;
        func_804DDD54(ext, filename, &out1, &out2, &out3, &out4);
        if (out1 != 0) {
            bool hasPrefix = (strstr(filename, lbl_eu_80522FD8) != nullptr);
            return func_8045B5AC(filename, allocHandle, fileHandle,
                                out2, out3, hasPrefix, out4, 0);
        }
    }

    return func_8045B5AC(filename, allocHandle, fileHandle, -1, -1, false, 0, 0);
}

// ============================================================================
// Forwarding stubs to CLibCriStreamingPlay
// ============================================================================
void CLibCri::func_80459A78() { func_8045B970__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A7C() { func_8045BAB0__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A80() { func_8045BBA0__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A84() { func_8045BC4C__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A88() { func_8045CA4C__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A8C() { func_8045C67C__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A90() { func_8045C700__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A94() { func_8045C8B0__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A98() { func_8045BE48__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459A9C() { func_8045CCFC__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459AA0() { func_8045CFDC__20CLibCriStreamingPlayFv(); }
void CLibCri::func_80459AA4() { func_8045D03C__20CLibCriStreamingPlayFv(); }

// ============================================================================
// Forwarding stubs to CLibCriMoviePlay
// ============================================================================
void CLibCri::func_80459AA8() { func_8045A260__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459AAC() { func_8045A48C__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459AC0() { func_8045A708__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459AC4() { func_8045A7F8__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459AC8() { func_8045A644__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459ACC() { func_8045A8C8__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459AD0() { func_8045B1E0__16CLibCriMoviePlayFv(); }

// Parameter swap adapter
void CLibCri::func_80459AB0() {
    // Assembly swaps this (r3) and second arg (r4), then tail-calls
    // CLibCriMoviePlay::func_8045B310
}

// Empty virtual override
void CLibCri::wkUpdate() {}

// CRI main execution
void CLibCri::func_80459AD8() {
    ADXM_ExecMain();
}

// Returns singleton instance
CLibCri* CLibCri::getInstance() {
    return spInstance;
}

// ============================================================================
// Login initialization sequence
// ============================================================================
bool CLibCri::wkStandbyLogin() {
    if (!CDevice::isColdStartReady()) {
        return false;
    }

    ADXM_SetCbErr((void (*)())func_80459C70, (void*)this);
    ADXWII_SetupDvdFs(0);
    ADXM_SetupFramework(2, 0);
    ADXT_SetDefSvrFreq(30);
    AIInit(nullptr);
    AXInit();
    MIXInit();
    sAXCallback = AXRegisterCallback(sAXCallbackWrapper);
    ADXT_Init();

    // Create CLibCriMoviePlay (0x668 bytes)
    const char* movieName = &lbl_eu_80522FD8[5];
    u32 workMem = CWorkThreadSystem::getWorkMem();
    CLibCriMoviePlay* moviePlay =
        (CLibCriMoviePlay*)mtl::MemManager::allocate(0x668, workMem);
    if (moviePlay != nullptr) {
        moviePlay = new (moviePlay) CLibCriMoviePlay(movieName, this);
    }
    CWorkUtil::entryWork(moviePlay, this, false);

    // Create CLibCriStreamingPlay (0x4B8 bytes)
    const char* streamName = &lbl_eu_80522FD8[0x16];
    workMem = CWorkThreadSystem::getWorkMem();
    CLibCriStreamingPlay* streamPlay =
        (CLibCriStreamingPlay*)mtl::MemManager::allocate(0x4B8, workMem);
    if (streamPlay != nullptr) {
        streamPlay = new (streamPlay) CLibCriStreamingPlay(streamName, this);
    }
    CWorkUtil::entryWork(streamPlay, this, false);

    return CWorkThread::wkStandbyLogin();
}

// ============================================================================
// Logout cleanup sequence
// ============================================================================
bool CLibCri::wkStandbyLogout() {
    CWorkThread* childFront = mChildren.front();
    if (childFront->mChildren.front() != childFront) {
        return false;
    }

    if (CWorkThreadSystem::getInstance() != nullptr) {
        return false;
    }
    if (CLib::getInstance() != nullptr) {
        return false;
    }

    ADXT_Finish();
    ADXM_ShutdownFramework();

    if (sAXCallback != nullptr) {
        AXRegisterCallback(nullptr);
    }

    MIXQuit();
    AXQuit();

    return CWorkThread::wkStandbyLogout();
}

// ============================================================================
// CDeviceVICb virtual override (viBeginFrame)
// Compiler generates thunk that adjusts this from CDeviceVICb subobject
// ============================================================================
void CLibCri::viBeginFrame() {
    func_80459AD8();
}

// ============================================================================
// CDeviceVICb virtual override
// ============================================================================
void CLibCri::func_80459C74() {
    func_8045BBA0__20CLibCriStreamingPlayFv();
}

// ============================================================================
// IErrorWii virtual override (errorWiiCB)
// Compiler generates thunk that adjusts this from IErrorWii subobject
// ============================================================================
void CLibCri::errorWiiCB() {
    func_80459C74();
}
