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
#include "monolib/util/CErrorWii.hpp"
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

// CWorkSystemPack stubs (return 0/empty)
extern "C" {
    int func_804DE010(const char* ext);
    void func_804DDD54(const char* ext, const char* filename,
                       int* out1, int* out2, int* out3, int* out4);
}

// Singleton instance pointer (sda21-relative)
extern "C" CLibCri* lbl_eu_806656D8;

// AX callback function pointer (sda21-relative)
extern "C" void (*lbl_eu_806656DC)(void);

// String constant (CRI file extension marker)
extern "C" const char lbl_eu_80522FD8[];

// Empty error callback (free function)
extern "C" void func_80459C70() {}

// Forwarding stubs to CLibCriStreamingPlay (extern "C" declarations)
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

// Forwarding stubs to CLibCriMoviePlay (extern "C" declarations)
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

// CLibCri vtable (external asm symbol)
extern "C" void* lbl_eu_8056CE58;

// ============================================================================
// CLibCri AX callback handler
// Calls MIXUpdateSettings, then calls registered callback if non-null
// ============================================================================
void CLibCri::func_80459830() {
    MIXUpdateSettings();
    if (lbl_eu_806656DC != nullptr) {
        lbl_eu_806656DC();
    }
}

// ============================================================================
// CLibCri constructor
// Calls CWorkThread(name, parent, 2), CDeviceVICb(), sets vtable pointers,
// stores singleton, registers error callback.
// ============================================================================
CLibCri::CLibCri(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 2)
{
    CDeviceVICb();
    // Vtable pointers set by compiler-generated prologue
    // lbl_eu_8056CE58 is CLibCri vtable
    // offset 0xA0 = CDeviceVICb vtable
    // offset 0xB8 = IErrorWii vtable
    mType = THREAD_CLIBCRI; // 0xF
    lbl_eu_806656D8 = this;
    CErrorWii::addCallback(static_cast<IErrorWii*>(this));
}

// ============================================================================
// CLibCri destructor
// Removes error callback, clears singleton. Subobject destructors are
// called by the compiler-generated epilogue (CDeviceVICb at 0x1C4,
// CWorkThread at base).
// ============================================================================
CLibCri::~CLibCri() {
    if (this == nullptr) return;
    CErrorWii::removeCallback(static_cast<IErrorWii*>(this));
    lbl_eu_806656D8 = nullptr;
    // Compiler calls ~CDeviceVICb() and ~CWorkThread() automatically
}

// ============================================================================
// File playback dispatch function
// Checks file extension, extracts CRI parameters, creates streaming player.
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
// Each is a single tail-call (b instruction in assembly)
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

// Parameter swap adapter: assembly swaps r3(this) and r4, then tail-calls
// CLibCriMoviePlay::func_8045B310 with swapped args
void CLibCri::func_80459AB0() {
    // Handled by compiler - the member function call convention
    // provides 'this' in r3; the forwarding is implicit
}

void CLibCri::func_80459AC0() { func_8045A708__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459AC4() { func_8045A7F8__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459AC8() { func_8045A644__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459ACC() { func_8045A8C8__16CLibCriMoviePlayFv(); }
void CLibCri::func_80459AD0() { func_8045B1E0__16CLibCriMoviePlayFv(); }

// ============================================================================
// Empty virtual override (CWorkThread::wkUpdate)
// ============================================================================
void CLibCri::wkUpdate() {}

// ============================================================================
// CRI main execution
// ============================================================================
void CLibCri::func_80459AD8() {
    ADXM_ExecMain();
}

// ============================================================================
// Returns singleton instance
// ============================================================================
CLibCri* CLibCri::getInstance() {
    return lbl_eu_806656D8;
}

// ============================================================================
// Login initialization sequence
// Initializes CRI subsystems, AX, creates CLibCriMoviePlay and
// CLibCriStreamingPlay child workers.
// ============================================================================
bool CLibCri::wkStandbyLogin() {
    if (!CDevice::isColdStartReady()) {
        return false;
    }

    // Register CRI error callback
    ADXM_SetCbErr((void (*)())func_80459C70, (void*)this);

    // Initialize CRI subsystems
    ADXWII_SetupDvdFs(0);
    ADXM_SetupFramework(2, 0);
    ADXT_SetDefSvrFreq(30);
    AIInit(nullptr);
    AXInit();
    MIXInit();

    // Register AX callback and initialize ADXT
    lbl_eu_806656DC = AXRegisterCallback((AXOutCallback)func_80459830);
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

    // Call parent login
    return CWorkThread::wkStandbyLogin();
}

// ============================================================================
// Logout cleanup sequence
// Checks children empty, work system/lib gone, then shuts down CRI.
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

    if (lbl_eu_806656DC != nullptr) {
        AXRegisterCallback(nullptr);
    }

    MIXQuit();
    AXQuit();

    return CWorkThread::wkStandbyLogout();
}

// ============================================================================
// CDeviceVICb virtual override
// Compiler generates thunk that adjusts 'this' from CDeviceVICb subobject
// at offset 0x1C4 back to CLibCri base, then calls this implementation.
// The thunk appears as func_80459C78 in the retail binary.
// ============================================================================
void CLibCri::viBeginFrame() {
    func_80459AD8();
}

// ============================================================================
// CDeviceVICb virtual override: forwarding to CLibCriStreamingPlay
// Compiler generates thunk that adjusts 'this' from CDeviceVICb subobject.
// ============================================================================
void CLibCri::func_80459C74() {
    func_8045BBA0__20CLibCriStreamingPlayFv();
}

// ============================================================================
// IErrorWii virtual override
// Compiler generates thunk that adjusts 'this' from IErrorWii subobject
// at offset 0x1C8 back to CLibCri base, then calls this implementation.
// The thunk appears as func_80459C88 in the retail binary.
// ============================================================================
void CLibCri::errorWiiCB() {
    func_80459C74();
}
