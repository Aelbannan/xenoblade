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
    if (lbl_eu_806656DC != nullptr) {
        lbl_eu_806656DC();
    }
}

// ============================================================================
// CLibCri constructor (extern "C" free-function form: a real member ctor
// would auto-emit __vt__7CLibCri + __RTTI__ locators here, blowing the data
// sections; the retail vtable is the dissolved blob below).
// ============================================================================
extern "C" void __ct__7CLibCriFPCcP11CWorkThread(CLibCri* self, const char* pName, CWorkThread* pParent) {
    __ct__11CWorkThreadFPCcP11CWorkThreadi(self, pName, pParent, 2);
    lbl_eu_806656D8 = self;
    self->mType = CWorkThread::THREAD_CLIBCRI; // 0xF
    CErrorWii::addCallback(static_cast<IErrorWii*>(self));
}

// ============================================================================
// CLibCri destructor (extern "C" free-function form, see ctor note).
// ============================================================================
extern "C" void __dt__7CLibCriFv(CLibCri* self, int flag) {
    if (self != nullptr) {
        CErrorWii::removeCallback(static_cast<IErrorWii*>(self));
        lbl_eu_806656D8 = nullptr;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
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

// Empty virtual override (extern "C" free-function form: no auto vtable).
extern "C" void wkUpdate__7CLibCriFv(CLibCri* self) { (void)self; }

// CRI main execution
void CLibCri::func_80459AD8() {
    ADXM_ExecMain();
}

// Returns singleton instance
CLibCri* CLibCri::getInstance() {
    return lbl_eu_806656D8;
}

// ============================================================================
// Login initialization sequence
// ============================================================================
extern "C" bool wkStandbyLogin__7CLibCriFv(CLibCri* self) {
    if (!CDevice::isColdStartReady()) {
        return false;
    }

    ADXM_SetCbErr((void (*)())func_80459C70, (void*)self);
    ADXWII_SetupDvdFs(0);
    ADXM_SetupFramework(2, 0);
    ADXT_SetDefSvrFreq(30);
    AIInit(nullptr);
    AXInit();
    MIXInit();
    lbl_eu_806656DC = AXRegisterCallback((void (*)(void))&CLibCri::func_80459830);
    ADXT_Init();

    // Create CLibCriMoviePlay (0x668 bytes)
    const char* movieName = &lbl_eu_80522FD8[5];
    u32 workMem = CWorkThreadSystem::getWorkMem();
    CLibCriMoviePlay* moviePlay =
        (CLibCriMoviePlay*)mtl::MemManager::allocate(0x668, workMem);
    if (moviePlay != nullptr) {
        moviePlay = new (moviePlay) CLibCriMoviePlay(movieName, self);
    }
    CWorkUtil::entryWork(moviePlay, self, false);

    // Create CLibCriStreamingPlay (0x4B8 bytes)
    const char* streamName = &lbl_eu_80522FD8[0x16];
    workMem = CWorkThreadSystem::getWorkMem();
    CLibCriStreamingPlay* streamPlay =
        (CLibCriStreamingPlay*)mtl::MemManager::allocate(0x4B8, workMem);
    if (streamPlay != nullptr) {
        streamPlay = new (streamPlay) CLibCriStreamingPlay(streamName, self);
    }
    CWorkUtil::entryWork(streamPlay, self, false);

    return wkStandbyLogin__11CWorkThreadFv(self);
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

    if (lbl_eu_806656DC != nullptr) {
        AXRegisterCallback(nullptr);
    }

    MIXQuit();
    AXQuit();

    return wkStandbyLogout__11CWorkThreadFv(self);

return_false:
    return false;
}

// ============================================================================
// CDeviceVICb virtual override: retail keeps ONLY the @452@ thunk (spelled
// as a raw vtable word below) + the real body func_80459AD8 (ADXM_ExecMain).
// ============================================================================

// ============================================================================
// CDeviceVICb virtual override
// ============================================================================
void CLibCri::func_80459C74() {
    func_8045BBA0__20CLibCriStreamingPlayFv();
}

// ============================================================================
// IErrorWii virtual override: retail keeps ONLY the @456@ thunk (raw vtable
// word below) + func_80459C74 (the real body).
// ============================================================================

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// The retail split object carries these sections as RAW words (the DOL's
// reloc table has no entries for this unit's data), so every pointer is a
// plain constant - no relocs - and the thunk/@-prefixed names (which C++
// cannot spell) are reproduced as raw addresses.
//
// [.data] 0x8056CE58-0x8056CF48 (240B): CLibCri primary vtable (208B: 40-slot
// CWorkThread chain + CDeviceVICb sub-vtable + IErrorWii sub-vtable) followed
// by the nw4r ScnGroup RTTI chain (32B).
extern "C" u32 lbl_eu_8056CE58[52] = {
    0x80663790, 0x00000000, 0x8045D8EC, 0x8003A1D4, 0x8003A1CC, 0x8003A1C4,
    0x8003A1BC, 0x8003A1B8, 0x8003A1B0, 0x8003A1A8, 0x8003A1A0, 0x8003A198,
    0x8003A190, 0x8003A188, 0x8003A180, 0x8003A178, 0x8003A170, 0x8003A168,
    0x8003A160, 0x8003A158, 0x8003A150, 0x8003A148, 0x8003A140, 0x8003A138,
    0x8003A130, 0x8003A128, 0x8003A120, 0x8003A118, 0x8003A110, 0x8003A108,
    0x8003A100, 0x8003A0F8, 0x8003A0F0, 0x8003A0EC, 0x8045DAE4, 0x8003A1E8,
    0x8003A1E4, 0x8045DAF4, 0x8045DC08, 0x8003A1DC,
    0x80663790, 0xFFFFFE3C, 0x8045DC98, 0x8045DC90, 0x801677E8, 0x8044B4B0,
    0x80663790, 0xFFFFFE38, 0x8045DCA8, 0x8045DCA0, 0x8045DAE8, 0x8045DC8C,
};
extern "C" u32 __vt__Q34nw4r3g3d8ScnGroup[8] = {
    0x80663618, 0x000001C4, 0x806618A8, 0x00000000,
    0x806618A0, 0x00000000, 0x00000000, 0x00000000,
};

// [.rodata] 0x80522FD8-0x80523008 (48B): ".ahx" / "CLibCriMoviePlay" /
// "CLibCriStreamingPlay" (referenced by func_8045997C and wkStandbyLogin).
// Declared `extern const char[]` in CLibCri.hpp, so defined as a char array.
extern "C" const char lbl_eu_80522FD8[48] = {
    0x2E,0x61,0x68,0x78,0x00,
    0x43,0x4C,0x69,0x62,0x43,0x72,0x69,0x4D,0x6F,0x76,0x69,0x65,0x50,0x6C,0x61,0x79,0x00,
    0x43,0x4C,0x69,0x62,0x43,0x72,0x69,0x53,0x74,0x72,0x65,0x61,0x6D,0x69,0x6E,0x67,0x50,0x6C,0x61,0x79,0x00,
    0x00,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x80663790-0x80663798 (8B): RTTI locator { name (foreign sdata2
// lbl_eu_8066A4D0 "CLibCri"), base sub-vtable lbl_eu_8056CF28 }.
extern "C" u32 lbl_eu_80663790[2] = {
    0x8066A4D0, 0x8056CF28,
};

// dissolved monolibdata2 - lib/CLibCri data now provided via retail copy (additive edit)
