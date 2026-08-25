#pragma once

#include <types.h>
#include <revolution/GX.h>
#include "monolib/work/CWorkThread.hpp"
#include "monolib/device/CDeviceVICb.hpp"

// Singleton instance pointer (retail .sbss: lbl_eu_806656E0).
// Declared at global scope so MWCC emits the unmangled retail name.
class CLibCriMoviePlay;
extern CLibCriMoviePlay* lbl_eu_806656E0;

// Forward declarations for CRI movie player API
struct MovieEntry;
extern "C" {
    u32 mwPlyCalcWorkCprmSfd(void* cprm);
    void mwPlyStartFname(void* ply, const char* filename);
    int mwPlyGetStat(void* ply);
    void mwPlyStop(void* ply);
    void mwPlyPause(void* ply, int pause);
    int mwPlyGetOutVol(void* ply);
    void mwPlySetOutVol(void* ply, int vol);
    void mwPlyInitSfdFx(void* fxprm);
    void mwPlyGetCurFrm(void* ply, void* frm);
    void mwPlyRelCurFrm(void* ply);
    void mwPlyFxSetOutBufPitchHeight(void* ply, u16 pitch, u16 height);
    void mwPlyFxCnvFrmY84C44(void* ply, void* frm, void* yBuf, void* cbcrBuf);
    void* criware_8039FF34(void* cprm);
    void criware_803A09B4(void* ply);
    void ADXM_ExecMain(void);
    void VIWaitForRetrace(void);

    // Retail-named entry-release helper (defined in CLibCriMoviePlay.cpp).
    // C-linkage so the mangled retail name is emitted; kept outlined (large
    // body) so callers reference the symbol directly.
    void func_8045A54C__16CLibCriMoviePlayFv(MovieEntry* entry, int flags);
}

// Movie playback entry (0x124 bytes)
struct MovieEntry {
    u32 mFlags;              // 0x00
    void* mPlyHandle;        // 0x04 - CRI movie player handle (opaque)
    // CRI cprm block (passed to mwPlyCalcWorkCprmSfd at 0x08); the work
    // buffer pointer/size slots at 0x20/0x24 overlap the cprm tail.
    u32 mCprmMode;           // 0x08 - cprm[0] = 1
    u32 mCprmFormat;         // 0x0C - cprm[1] = 0x5B8D80
    u32 mCprmWidth;          // 0x10 - cprm[2] = 0x280 (640)
    u32 mCprmHeight;         // 0x14 - cprm[3] = 0x1C8 (456)
    u32 mCprmType;           // 0x18 - cprm[4] = 2
    u32 mCprmFlags;          // 0x1C - cprm[5] = 1
    void* mWorkBuf;          // 0x20 - CRI work buffer (allocated in startMovie)
    u32 mWorkSize;           // 0x24 - calculated work buffer size (inside cprm)
    u32 field_0x28;          // 0x28 - zeroed before playback starts
    u8 field_0x2C[0x28];     // 0x2C - unused cprm tail
    u32 mAllocHandle;        // 0x54 - CRI alloc handle
    u32 mAllocHandle2;       // 0x58 - secondary allocation handle
    char mFilename[0x40];    // 0x5C - filename buffer
    u32 mFilenameLen;        // 0x9C - strlen of mFilename
    bool mActive;            // 0xA0 - entry active flag
    void* mTexBufY;          // 0xA4 - Y texture buffer pointer (written from void* allocate)
    u32 mTexBufYSize;        // 0xA8 - Y texture buffer size
    void* mTexBufCbCr;       // 0xAC - CbCr texture buffer pointer
    u32 mTexBufCbCrSize;     // 0xB0 - CbCr texture buffer size
    GXTexObj mTexObjY;       // 0xB4 - GX texture object for Y plane
    GXTexObj mTexObjCbCr;    // 0xD4 - GX texture object for CbCr plane
    u16 mTexWidth;           // 0xF4 - texture width
    u16 mTexHeight;          // 0xF6 - texture height
    s32 mAction;             // 0xF8 - movie action/state machine (signed: retail cmpi)
    u32 mPlayerId;           // 0xFC - player identifier / packed ID
    u32 mField100;           // 0x100
    float mColor[4];         // 0x104 - RGBA overlay color
    s32 mPlaybackState;      // 0x114 - playback state from mwPlyGetStat (signed: retail cmpi switch)
    bool mGlobalPause;       // 0x118 - global pause flag
    bool mPauseOverride;     // 0x119 - pause override flag
    void* mSavedTexBufY;     // 0x11C - saved Y buffer ptr (for cleanup)
    void* mSavedTexBufCbCr;  // 0x120 - saved CbCr buffer ptr (for cleanup)
};

// Size check: 0x124 = 292 bytes
// static_assert(sizeof(MovieEntry) == 0x124, "MovieEntry must be 0x124 bytes");

class __declspec(novtable) CLibCriMoviePlay : public CWorkThread, public CDeviceVICb {
public:
    CLibCriMoviePlay(const char* pName, CWorkThread* pParent);
    virtual ~CLibCriMoviePlay();

    // CWorkThread virtual overrides
    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    // CDeviceVICb virtual override (thunk adjusts this by -0x1C4)
    virtual void viBeginFrame();

    // Movie player methods
    // (func_80459DEC / func_8045A260 / func_8045A8C8 are retail-named free
    // functions defined in CLibCriMoviePlay.cpp; their retail ABIs pass the
    // object/text-map selectors in registers that do not match member mangling.)
    static MovieEntry* func_8045A1B0();               // func_8045A1B0
    // (func_8045A260: retail-named free function defined in the .cpp; its
    // retail ABI passes five arguments that do not match the () mangling.)
    void func_8045B1DC() {}                            // func_8045B1DC (empty)
    int func_8045B1E0();                               // func_8045B1E0
    void OnPauseTrigger(bool pause);                   // OnPauseTrigger
    void func_8045AE84();                              // per-frame texture update

    static CLibCriMoviePlay* getInstance() { return lbl_eu_806656E0; }

    // Fields kept public (retail-verified offsets); the retail-name wrappers
    // below are free functions and need direct access.
    MovieEntry mEntries[4];                // 0x1C8 - four movie playback slots
    u32 mStreamIdCounter;                  // 0x658 - stream ID counter
    bool mPlaybackState;                   // 0x65C - has active playback
    u32 mPauseCounter;                     // 0x660 - pause reference counter
};
// Total size: 0x664 bytes (CWorkThread 0x1C4 + CDeviceVICb 0x4 + 4*0x124 + 0xC)
