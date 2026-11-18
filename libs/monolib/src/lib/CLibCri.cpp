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
#include "monolib/work/CWorkSystem.hpp"
#include "monolib/lib/CLib.hpp"
#include <revolution/ax/AX.h>
#include <revolution/ax/AXOut.h>
#include <revolution/ai/ai.h>
#include <cstring>

// CRI middleware extern declarations
extern "C" {
    // Retail exports these ctors under aliased short names (the defining
    // units emit the full mangled symbols); declared here per the approved
    // reloc-name fix so the bl targets carry the retail reloc names.
    CLibCriMoviePlay* __ct__CLibCriMoviePlay(CLibCriMoviePlay* mem, const char* name, CWorkThread* parent);
    CLibCriStreamingPlay* __ct__CLibCriStreamingPlay(CLibCriStreamingPlay* mem, const char* name, CWorkThread* parent);
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

// CWorkSystemPack stubs
extern "C" {
    int hasPackFile(const char* ext);
    bool func_804DDD54(const char* ext, const char* filename,
                       int* out1, int* out2, int* out3, int* out4);
}

// CLibCri::spInstance is defined by CException.cpp (retail .sbss 0x806656D8,
// the TU that owns 0x806656C0-0x806656E0); only the AX callback pointer is
// imported here (retail 0x806656DC, same slice).
extern "C" void (*lbl_eu_806656DC)(void);

// Forwarding stubs to CLibCriStreamingPlay.
// Retail CLibCri.o still relocates these callers to the pre-recovery
// func_* linker names; keep those spellings on the call reloc.
extern "C" {
    void isStreamActive__20CLibCriStreamingPlayFv();
    void stopStream__20CLibCriStreamingPlayFv();
    void stopAllStreams__20CLibCriStreamingPlayFv();
    void setStreamPause__20CLibCriStreamingPlayFv();
    void fadeStreamVolume__20CLibCriStreamingPlayFv();
    void getStreamVolume__20CLibCriStreamingPlayFv();
    void setStreamVolume__20CLibCriStreamingPlayFv();
    void setStreamVolumeScale__20CLibCriStreamingPlayFv();
    void getStreamPosition__20CLibCriStreamingPlayFv();
    void setStreamPanVolume__20CLibCriStreamingPlayFv();
    void calcStreamBufferSize__20CLibCriStreamingPlayFv();
    void isStreamPaused__20CLibCriStreamingPlayFv();
}

// Forwarding stubs to CLibCriMoviePlay
extern "C" {
    void startMovie__16CLibCriMoviePlayFv();
    void stopMovie__16CLibCriMoviePlayFv();
    void setMoviePause__16CLibCriMoviePlayFv(int, int);
    void clearMovieGlobalPause__16CLibCriMoviePlayFv();
    void isMovieGlobalPaused__16CLibCriMoviePlayFv();
    void isMoviePlaying__16CLibCriMoviePlayFv();
    void renderMovie__16CLibCriMoviePlayFv();
    void getMovieWorkSize__16CLibCriMoviePlayFv();
}

// Empty error callback (free function)
extern "C" void handleCriError() {}

// ============================================================================
// CLibCri AX callback handler
// ============================================================================
void CLibCri::handleAxCallback() {
    MIXUpdateSettings();
    if (lbl_eu_806656DC != nullptr) {
        lbl_eu_806656DC();
    }
}

// ============================================================================
// CLibCri constructor
// ============================================================================
CLibCri::CLibCri(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 2) {
    spInstance = this;
    mType = CWorkThread::THREAD_CLIBCRI;
    CErrorWii::addCallback(this);
}

CLibCri::~CLibCri() {
    CErrorWii::removeCallback(this);
    spInstance = nullptr;
}

// ============================================================================
// File playback dispatch function
// ============================================================================
int CLibCri::dispatchFilePlayback(const char* filename, u32 allocHandle, int fileHandle) {
    if (allocHandle == (u32)-1) {
        allocHandle = mtl::MemManager::getHandleMEM2();
    }

    const char* ext = ml::CPathUtil::getFileExtPtr(filename);
    if (ext == nullptr) {
        return -1;
    }

    if (hasPackFile(ext) != 0) {
        int out1, out2, out3, out4;
        if (func_804DDD54(ext, filename, &out1, &out2, &out3, &out4) != 0) {
            bool hasPrefix = (strstr(filename, lbl_eu_80522FD8) != nullptr);
            return startStream__20CLibCriStreamingPlayFv((const char*)out1, allocHandle, fileHandle, out2, out3, hasPrefix);
        }
    }

    return startStream__20CLibCriStreamingPlayFv(filename, allocHandle, fileHandle, -1, -1, false);
}

// ============================================================================
// Forwarding stubs to CLibCriStreamingPlay
// ============================================================================
void CLibCri::isStreamActive() { isStreamActive__20CLibCriStreamingPlayFv(); }
void CLibCri::stopStream() { stopStream__20CLibCriStreamingPlayFv(); }
void CLibCri::stopAllStreams() { stopAllStreams__20CLibCriStreamingPlayFv(); }
void CLibCri::setStreamPause() { setStreamPause__20CLibCriStreamingPlayFv(); }
void CLibCri::fadeStreamVolume() { fadeStreamVolume__20CLibCriStreamingPlayFv(); }
void CLibCri::getStreamVolume() { getStreamVolume__20CLibCriStreamingPlayFv(); }
void CLibCri::setStreamVolume() { setStreamVolume__20CLibCriStreamingPlayFv(); }
void CLibCri::setStreamVolumeScale() { setStreamVolumeScale__20CLibCriStreamingPlayFv(); }
void CLibCri::getStreamPosition() { getStreamPosition__20CLibCriStreamingPlayFv(); }
void CLibCri::setStreamPanVolume() { setStreamPanVolume__20CLibCriStreamingPlayFv(); }
void CLibCri::calcStreamBufferSize() { calcStreamBufferSize__20CLibCriStreamingPlayFv(); }
void CLibCri::isStreamPaused() { isStreamPaused__20CLibCriStreamingPlayFv(); }

// ============================================================================
// Forwarding stubs to CLibCriMoviePlay
// ============================================================================
void CLibCri::startMovie() { startMovie__16CLibCriMoviePlayFv(); }
void CLibCri::stopMovie() { stopMovie__16CLibCriMoviePlayFv(); }
void CLibCri::clearMoviePause() { clearMovieGlobalPause__16CLibCriMoviePlayFv(); }
void CLibCri::isMovieGlobalPaused() { isMovieGlobalPaused__16CLibCriMoviePlayFv(); }
void CLibCri::isMoviePlaying() { isMoviePlaying__16CLibCriMoviePlayFv(); }
void CLibCri::renderMovie() { renderMovie__16CLibCriMoviePlayFv(); }
void CLibCri::getMovieWorkSize() { getMovieWorkSize__16CLibCriMoviePlayFv(); }

// Parameter swap adapter (free-function form: the retail body swaps the two
// incoming register args before tail-calling, which a no-arg member cannot
// spell). Incoming r3=a, r4=b are forwarded as f(b, a).
extern "C" void setMoviePause__7CLibCriFv(int a, int b) {
    setMoviePause__16CLibCriMoviePlayFv(b, a);
}

// Empty virtual override
void CLibCri::wkUpdate() {}

// CRI main execution; the CDeviceVICb slot holds the this-adjusting thunk to
// this body.
void CLibCri::viBeforeDrawDone() {
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
    // Retail shape: branch FORWARD over the whole init body when the device
    // is not cold-start ready, falling into a shared `li r3, 0` exit.
    bool result;
    if (CDevice::isColdStartReady()) {
        ADXM_SetCbErr((void (*)())handleCriError, (void*)this);
        ADXWII_SetupDvdFs(0);
        ADXM_SetupFramework(2, 0);
        ADXT_SetDefSvrFreq(30);
        AIInit(nullptr);
        AXInit();
        MIXInit();
        lbl_eu_806656DC = AXRegisterCallback((void (*)(void))&CLibCri::handleAxCallback);
        ADXT_Init();

        // Create CLibCriMoviePlay (0x668 bytes)
        const char* movieName = &lbl_eu_80522FD8[5];
        u32 workMem = CWorkThreadSystem::getWorkMem();
        CLibCriMoviePlay* movie = (CLibCriMoviePlay*)mtl::MemManager::allocate(0x668, workMem);
        if (movie != nullptr) {
            movie = __ct__CLibCriMoviePlay(movie, movieName, this);
        }
        CWorkUtil::entryWork(movie, this, false);

        // Create CLibCriStreamingPlay (0x4B8 bytes)
        const char* streamName = &lbl_eu_80522FD8[0x16];
        workMem = CWorkThreadSystem::getWorkMem();
        CLibCriStreamingPlay* stream = (CLibCriStreamingPlay*)mtl::MemManager::allocate(0x4B8, workMem);
        if (stream != nullptr) {
            stream = __ct__CLibCriStreamingPlay(stream, streamName, this);
        }
        CWorkUtil::entryWork(stream, this, false);

        result = CWorkThread::wkStandbyLogin();
    } else {
        result = false;
    }
    return result;
}

// ============================================================================
// Logout cleanup sequence
// ============================================================================
bool CLibCri::wkStandbyLogout() {
    if (mChildren.begin() != mChildren.end()) goto return_false;
    if (CWorkSystem::getInstance() != nullptr) goto return_false;
    if (CLib::getInstance() != nullptr) goto return_false;

    ADXT_Finish();
    ADXM_ShutdownFramework();

    void (*cb)() = lbl_eu_806656DC;
    if (cb != nullptr) {
        AXRegisterCallback(cb);
    }

    MIXQuit();
    AXQuit();

    return CWorkThread::wkStandbyLogout();

return_false:
    return false;
}

// ============================================================================
// IErrorWii virtual override
// ============================================================================
void CLibCri::errorWiiCB() {
    stopAllStreams__20CLibCriStreamingPlayFv();
}

// [.rodata] 0x80522FD8-0x80523008 (48B): ".ahx" / "CLibCriMoviePlay" /
// "CLibCriStreamingPlay" (referenced by dispatchFilePlayback and wkStandbyLogin).
// Declared `extern const char[]` in CLibCri.hpp, so defined as a char array.
extern "C" const char lbl_eu_80522FD8[48] =
    ".ahx\0CLibCriMoviePlay\0CLibCriStreamingPlay\0\0\0\0\0";
