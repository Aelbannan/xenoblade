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
    void* __ct__CLibCriMoviePlay(void* mem, const char* name, CWorkThread* parent);
    void* __ct__CLibCriStreamingPlay(void* mem, const char* name, CWorkThread* parent);
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
    int func_804DE010(const char* ext);
    bool func_804DDD54(const char* ext, const char* filename,
                       int* out1, int* out2, int* out3, int* out4);
}

// Retail sbss singletons (owned by the CDeviceFontLoader/CDeviceFont unit's
// sbss range; plain extern keeps them out of this unit's sections):
//   lbl_eu_806656D8 = CLibCri::spInstance, lbl_eu_806656DC = AX callback ptr.
extern "C" CLibCri* lbl_eu_806656D8;
extern "C" void (*lbl_eu_806656DC)(void);

// Base-class helpers (retail flat names) for the extern "C" ctor/dtor/logins.
extern "C" void __ct__11CWorkThreadFPCcP11CWorkThreadi(void* self, const char* name, void* parent, int capacity);
extern "C" bool wkStandbyLogin__11CWorkThreadFv(void* self);
extern "C" bool wkStandbyLogout__11CWorkThreadFv(void* self);
extern "C" void __dl__FPv(void* p);
extern "C" void __dt__11CDeviceVICbFv(void* self, int flag);
extern "C" void __ct__11CDeviceVICbFv(void* self);
extern "C" void __dt__11CWorkThreadFv(void* self, int flag);
// CLibCri full-object constructor. Retail exports it under the plain member
// mangling (cross-TU callers construct via `new CLibCri(...)`), and the
// defining unit must emit exactly that symbol, so the C linkage is fixed by
// this declaration; the out-of-line definition below inherits it.
extern "C" void* __ct__7CLibCriFPCcP11CWorkThread(CLibCri* object, const char* pName, CWorkThread* pParent);
extern "C" u32 lbl_eu_8056CE58[52]; // CLibCri primary vtable (defined below)

// Foreign vtable-slot symbols (retail flat names). The __RTTI__* typeinfo
// symbols cannot be spelled in this TU (-RTTI on reserves them once the
// IWorkEvent/CWorkThread class definitions are visible -> MWCC 10322), so the
// two ScnGroup RTTI slots use stand-in names renamed onto the retail symbols
// by UNIT_RULES exact_renames (same recipe as CLibLayout.o / CWorkRoot.o).
extern "C" {
    void viAfterDrawDone__11CDeviceVICbFv();
    void viBeginFrame__11CDeviceVICbFv();
    u32 lbl_eu_80663618[];   // nw4r g3d ScnGroup RTTI chain anchor (foreign .sdata)
    char lbl_eu_8066A4D0[];  // "CLibCri" RTTI name string (CGXCache shared .sdata2 pool)
    extern void* rtti_10IWorkEvent;  // stand-in for __RTTI__10IWorkEvent
    extern void* rtti_11CWorkThread; // stand-in for __RTTI__11CWorkThread
    // IWorkEvent virtual handlers (weak defaults live in kyoshin/CGame.cpp).
    int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
    int OnFileEvent__10IWorkEventFP10CEventFile(void*);
    int WorkEvent3__10IWorkEventFPv(void*);
    int WorkEvent4__10IWorkEventFv();
    void OnPauseTrigger__10IWorkEventFb(int);
    int WorkEvent6__10IWorkEventFv();
    int WorkEvent7__10IWorkEventFv();
    int WorkEvent8__10IWorkEventFv();
    int WorkEvent9__10IWorkEventFv();
    int WorkEvent10__10IWorkEventFv();
    int WorkEvent11__10IWorkEventFv();
    int WorkEvent12__10IWorkEventFv();
    int WorkEvent13__10IWorkEventFv();
    int WorkEvent14__10IWorkEventFv();
    int WorkEvent15__10IWorkEventFv();
    int WorkEvent16__10IWorkEventFv();
    int WorkEvent17__10IWorkEventFv();
    int WorkEvent18__10IWorkEventFv();
    int WorkEvent19__10IWorkEventFv();
    int WorkEvent20__10IWorkEventFv();
    int WorkEvent21__10IWorkEventFv();
    int WorkEvent22__10IWorkEventFv();
    int WorkEvent23__10IWorkEventFv();
    int WorkEvent24__10IWorkEventFv();
    int WorkEvent25__10IWorkEventFv();
    int WorkEvent26__10IWorkEventFv();
    int WorkEvent27__10IWorkEventFv();
    int WorkEvent28__10IWorkEventFv();
    int WorkEvent29__10IWorkEventFv();
    int WorkEvent30__10IWorkEventFv();
    int WorkEvent31__10IWorkEventFv();
    // CWorkThread work-method vtable slots.
    void wkRender__11CWorkThreadFv();
    void wkRenderAfter__11CWorkThreadFv();
    void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
}
extern "C" u32 __vt__Q34nw4r3g3d8ScnGroup[8]; // nw4r ScnGroup vtable (defined below)

// Empty error callback (free function)
extern "C" void handleCriError() {}

// Forwarding stubs to CLibCriStreamingPlay
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

// ============================================================================
// CLibCri AX callback handler
// ============================================================================
void CLibCri::handleAxCallback() {
    MIXUpdateSettings();
    if (lbl_eu_806656DC != nullptr) {
        lbl_eu_806656DC();
    }
}

// Vptr layout view over the CLibCri MI object: the primary (CWorkThread)
// vptr at 0x000 plus the two secondary-base vptrs at 0x1C4 (CDeviceVICb) and
// 0x1C8 (IErrorWii). Lets the ctor/dtor re-point the three slots at the
// dissolved retail vtable blob through typed fields.
struct CLibCriVptrView {
    void* vtPrimary;   //0x000 CWorkThread primary vptr
    void* pad[112];    //0x004
    void* vtViCb;      //0x1C4 CDeviceVICb sub-vptr
    void* vtErrorWii;  //0x1C8 IErrorWii sub-vptr
};

// ============================================================================
// CLibCri constructor (see declaration above for why the retail symbol is a
// flat full-object ctor).
// ============================================================================
void* __ct__7CLibCriFPCcP11CWorkThread(CLibCri* object, const char* pName, CWorkThread* pParent) {
    __ct__11CWorkThreadFPCcP11CWorkThreadi(object, pName, pParent, 2);
    __ct__11CDeviceVICbFv(&((CLibCriVptrView*)object)->vtViCb);
    // full-object construction: restore the primary + both MI sub-vptrs.
    // The two secondary slots point at the CDeviceVICb / IErrorWii
    // sub-vtables carved out of the dissolved blob (+0xA0 / +0xB8), spelled
    // as element indexes into the u32 word array.
    CLibCriVptrView* vp = (CLibCriVptrView*)object;
    vp->vtPrimary = &lbl_eu_8056CE58[0];
    vp->vtViCb = &lbl_eu_8056CE58[0xA0 / sizeof(u32)];
    vp->vtErrorWii = &lbl_eu_8056CE58[0xB8 / sizeof(u32)];
    lbl_eu_806656D8 = object;
    object->mType = CWorkThread::THREAD_CLIBCRI;
    // Implicit guarded CLibCri* -> IErrorWii* conversion (null-check +
    // this-adjust +0x1C8); do not spell the null check by hand. The ctor
    // returns object (retail keeps it live across the call for the epilog).
    CErrorWii::addCallback(static_cast<IErrorWii*>(object));
    return object;
}

// ============================================================================
// CLibCri destructor (extern "C" free-function form, see ctor note).
// ============================================================================
// ============================================================================
// auto_inline off: -ipa file would otherwise inline this dtor body into the
// @452@/@456@ dtor thunks below (retail keeps them as 2-insn tail calls).
// ============================================================================
#pragma auto_inline off
extern "C" void* __dt__7CLibCriFv(CLibCri* self, int flag) {
    char* base = (char*)self;
    if (self != nullptr) {
        IErrorWii* errCb = reinterpret_cast<IErrorWii*>(base);
        // full-object destruction: restore the primary + both MI sub-vptrs
        *(void**)base = &lbl_eu_8056CE58;
        *(void**)(base + 0x1C4) = (char*)&lbl_eu_8056CE58 + 0xA0;
        *(void**)(base + 0x1C8) = (char*)&lbl_eu_8056CE58 + 0xB8;
        if (errCb != nullptr) {
            errCb = reinterpret_cast<IErrorWii*>(base + 0x1C8);
        }
        CErrorWii::removeCallback(errCb);
        lbl_eu_806656D8 = nullptr;
        __dt__11CDeviceVICbFv(base + 0x1C4, 0);
        __dt__11CWorkThreadFv(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#pragma auto_inline on

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

    if (func_804DE010(ext) != 0) {
        int out1, out2, out3, out4;
        if (func_804DDD54(ext, filename, &out1, &out2, &out3, &out4) != 0) {
            bool hasPrefix = (strstr(filename, lbl_eu_80522FD8) != nullptr);
            return func_8045B5AC(out1, allocHandle, fileHandle, out2, out3, hasPrefix);
        }
    }

    return func_8045B5AC((int)filename, allocHandle, fileHandle, -1, -1, false);
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

// Empty virtual override (extern "C" free-function form: no auto vtable).
extern "C" void wkUpdate__7CLibCriFv(CLibCri* self) { (void)self; }

// CRI main execution (free-function form: the retail vtable references the
// flat symbol execCriMain__7CLibCriFv directly). auto_inline off keeps
// @452@viBeginFrame's call opaque instead of folding the thunk away.
#pragma auto_inline off
extern "C" void execCriMain__7CLibCriFv(CLibCri* self) {
    (void)self;
    ADXM_ExecMain();
}
#pragma auto_inline on

// Returns singleton instance
CLibCri* CLibCri::getInstance() {
    return lbl_eu_806656D8;
}

// ============================================================================
// Login initialization sequence
// ============================================================================
extern "C" bool wkStandbyLogin__7CLibCriFv(CLibCri* self) {
    // Retail shape: branch FORWARD over the whole init body when the device
    // is not cold-start ready, falling into a shared `li r3, 0` exit.
    bool result;
    if (CDevice::isColdStartReady()) {
        ADXM_SetCbErr((void (*)())handleCriError, (void*)self);
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
        void* movieMem = mtl::MemManager::allocate(0x668, workMem);
        if (movieMem != nullptr) {
            movieMem = __ct__CLibCriMoviePlay(movieMem, movieName, self);
        }
        CWorkUtil::entryWork((CWorkThread*)movieMem, self, false);

        // Create CLibCriStreamingPlay (0x4B8 bytes)
        const char* streamName = &lbl_eu_80522FD8[0x16];
        workMem = CWorkThreadSystem::getWorkMem();
        void* streamMem = mtl::MemManager::allocate(0x4B8, workMem);
        if (streamMem != nullptr) {
            streamMem = __ct__CLibCriStreamingPlay(streamMem, streamName, self);
        }
        CWorkUtil::entryWork((CWorkThread*)streamMem, self, false);

        result = wkStandbyLogin__11CWorkThreadFv(self);
    } else {
        result = false;
    }
    return result;
}

// ============================================================================
// Logout cleanup sequence
// ============================================================================
extern "C" bool wkStandbyLogout__7CLibCriFv(CLibCri* self) {
    if (self->mChildren.begin() != self->mChildren.end()) goto return_false;
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

    return wkStandbyLogout__11CWorkThreadFv(self);

return_false:
    return false;
}

// ============================================================================
// CDeviceVICb virtual override: retail keeps ONLY the @452@ thunk (spelled
// as a raw vtable word below) + the real body execCriMain (ADXM_ExecMain).
// ============================================================================

// ============================================================================
// CDeviceVICb virtual override (free-function form, see execCriMain note).
// ============================================================================
#pragma auto_inline off
extern "C" void onErrorCallback__7CLibCriFv(CLibCri* self) {
    (void)self;
    stopAllStreams__20CLibCriStreamingPlayFv();
}
#pragma auto_inline on

// ============================================================================
// This-adjusting MI thunks (retail keeps four 2-insn thunks at the tail of
// .text: addi r3,r3,-delta ; b <real body>). The @N@-prefixed retail symbols
// cannot be spelled in C++, so they are defined under placeholder names and
// renamed onto the retail names by UNIT_RULES exact_renames (§17.6).
// ============================================================================
extern "C" void thunk452_viBeginFrame(CLibCri* self) {
    execCriMain__7CLibCriFv((CLibCri*)((char*)self - 0x1C4));
}
extern "C" void thunk452_dt(CLibCri* self, int flag) {
    __dt__7CLibCriFv((CLibCri*)((char*)self - 0x1C4), flag);
}
extern "C" void thunk456_errorWiiCB(CLibCri* self) {
    onErrorCallback__7CLibCriFv((CLibCri*)((char*)self - 0x1C8));
}
extern "C" void thunk456_dt(CLibCri* self, int flag) {
    __dt__7CLibCriFv((CLibCri*)((char*)self - 0x1C8), flag);
}

// ============================================================================
// IErrorWii virtual override: retail keeps ONLY the @456@ thunk (raw vtable
// word below) + onErrorCallback (the real body).
// ============================================================================

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// The retail split object carries these sections as relocated words, so every
// pointer slot is initialized with &retail-symbol to emit the exact retail
// reloc; only the two vtable offset-to-top deltas and zero pads are plain
// constants. The thunk/@-prefixed names (which C++ cannot spell) resolve via
// UNIT_RULES exact_renames on the postprocessed copy.
//
// [.sdata] 0x80663790-0x80663798 (8B): RTTI locator { name (foreign sdata2
// lbl_eu_8066A4D0 "CLibCri"), base sub-vtable __vt__Q34nw4r3g3d8ScnGroup }.
extern "C" u32 lbl_eu_80663790[2] = {
    (u32)lbl_eu_8066A4D0,
    (u32)__vt__Q34nw4r3g3d8ScnGroup,
};
//
// [.data] 0x8056CE58-0x8056CF48 (240B): CLibCri primary vtable (208B: 40-slot
// CWorkThread chain + CDeviceVICb sub-vtable + IErrorWii sub-vtable) followed
// by the nw4r ScnGroup RTTI chain (32B).
extern "C" u32 lbl_eu_8056CE58[52] = {
    (u32)&lbl_eu_80663790,
    0x00000000,
    (u32)&__dt__7CLibCriFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc,
    (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv,
    (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv,
    (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv,
    (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv,
    (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv,
    (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv,
    (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv,
    (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv,
    (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv,
    (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv,
    (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv,
    (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv,
    (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv,
    (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv,
    (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__7CLibCriFv,
    (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__7CLibCriFv,
    (u32)&wkStandbyLogout__7CLibCriFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
    // CDeviceVICb sub-vtable (this-adjust -0x1C4)
    (u32)&lbl_eu_80663790,
    0xFFFFFE3C,
    (u32)&thunk452_dt,
    (u32)&thunk452_viBeginFrame,
    (u32)&viAfterDrawDone__11CDeviceVICbFv,
    (u32)&viBeginFrame__11CDeviceVICbFv,
    // IErrorWii sub-vtable (this-adjust -0x1C8)
    (u32)&lbl_eu_80663790,
    0xFFFFFE38,
    (u32)&thunk456_dt,
    (u32)&thunk456_errorWiiCB,
    (u32)&execCriMain__7CLibCriFv,
    (u32)&onErrorCallback__7CLibCriFv,
};
extern "C" u32 __vt__Q34nw4r3g3d8ScnGroup[8] = {
    (u32)&lbl_eu_80663618, 0x000001C4,
    (u32)&rtti_10IWorkEvent, 0x00000000,
    (u32)&rtti_11CWorkThread, 0x00000000,
    0x00000000, 0x00000000,
};

// [.rodata] 0x80522FD8-0x80523008 (48B): ".ahx" / "CLibCriMoviePlay" /
// "CLibCriStreamingPlay" (referenced by dispatchFilePlayback and wkStandbyLogin).
// Declared `extern const char[]` in CLibCri.hpp, so defined as a char array.
extern "C" const char lbl_eu_80522FD8[48] = {
    0x2E,0x61,0x68,0x78,0x00,
    0x43,0x4C,0x69,0x62,0x43,0x72,0x69,0x4D,0x6F,0x76,0x69,0x65,0x50,0x6C,0x61,0x79,0x00,
    0x43,0x4C,0x69,0x62,0x43,0x72,0x69,0x53,0x74,0x72,0x65,0x61,0x6D,0x69,0x6E,0x67,0x50,0x6C,0x61,0x79,0x00,
    0x00,0x00,0x00,0x00,0x00,
};

// dissolved monolibdata2 - lib/CLibCri data now provided via retail copy (additive edit)
