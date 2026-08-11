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
    u8 mCprmData[0x1C];      // 0x08 - CRI cprm structure inline data
    u32 mWorkSize;           // 0x24 - calculated work buffer size (inside cprm)
    u8 mCprmData2[0x2C];     // 0x28 - remaining cprm fields
    void* mAllocHandle;      // 0x54 - CRI alloc handle stored in a pointer slot
    u32 mAllocHandle2;       // 0x58 - secondary allocation handle
    bool mActive;            // 0x5C - entry active/has-filename flag
    char mFilename[0x43];    // 0x5D - filename buffer
    u32 mStreamId;           // 0xA0 - stream ID
    void* mTexBufY;          // 0xA4 - Y texture buffer pointer (written from void* allocate)
    u32 mTexBufYSize;        // 0xA8 - Y texture buffer size
    void* mTexBufCbCr;       // 0xAC - CbCr texture buffer pointer
    u32 mTexBufCbCrSize;     // 0xB0 - CbCr texture buffer size
    GXTexObj mTexObjY;       // 0xB4 - GX texture object for Y plane
    GXTexObj mTexObjCbCr;    // 0xD4 - GX texture object for CbCr plane
    u16 mTexWidth;           // 0xF4 - texture width
    u16 mTexHeight;          // 0xF6 - texture height
    u32 mAction;             // 0xF8 - movie action/state machine
    u32 mPlayerId;           // 0xFC - player identifier / packed ID
    u32 mField100;           // 0x100
    float mColor[4];         // 0x104 - RGBA overlay color
    u32 mPlaybackState;      // 0x114 - playback state from mwPlyGetStat
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
    void setupGXState();                              // func_80459DEC
    static MovieEntry* func_8045A1B0();               // func_8045A1B0
    int startMovie(const char* filename, u32 allocHandle,
                   u32 allocHandle2, bool waitForStart,
                   bool useAlternateBuf);              // func_8045A260
    bool isMoviePlaying(int id);                       // func_8045A644
    void unsetPauseFlag(int id);                       // func_8045A708
    bool hasActiveMovie(int id);                       // func_8045A7F8
    bool renderMovie(int id);                          // func_8045A8C8
    void updateMovies();                               // func_8045AE84
    void func_8045B1DC() {}                            // func_8045B1DC (empty)
    int func_8045B1E0();                               // func_8045B1E0
    void OnPauseTrigger(bool pause);                   // OnPauseTrigger
    void setPauseState(bool pause);                    // func_8045B310

    static CLibCriMoviePlay* getInstance() { return lbl_eu_806656E0; }

    // Fields kept public (retail-verified offsets); the retail-name wrappers
    // below are free functions and need direct access.
    MovieEntry mEntries[4];                // 0x1C8 - four movie playback slots
    u32 mStreamIdCounter;                  // 0x658 - stream ID counter
    bool mPlaybackState;                   // 0x65C - has active playback
    u32 mPauseCounter;                     // 0x660 - pause reference counter
};
// Total size: 0x664 bytes (CWorkThread 0x1C4 + CDeviceVICb 0x4 + 4*0x124 + 0xC)
