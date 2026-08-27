#pragma once

#include "monolib/work/CWorkThread.hpp"
#include "monolib/device/CDeviceVICb.hpp"
#include "monolib/util/CErrorWii.hpp"
#include <revolution/GX.h>
#include <types.h>

// One movie playback slot. Entry stride is retail-verified (0x124);
// individual field offsets are reconstruction-quality (not yet retail-verified).
struct MovieEntry {
    bool mActive;            // 0x000
    u8 _pad00[3];
    u32 mPlayerId;           // 0x004
    u32 mStreamId;           // 0x008
    void* mPlyHandle;        // 0x00C
    char mFilename[0x30];    // 0x010
    u32 mFlags;              // 0x040
    u32 mAction;             // 0x044
    u32 mWorkSize;           // 0x048
    u32 mAllocHandle;        // 0x04C
    u32 mAllocHandle2;       // 0x050
    void* mCprmData;         // 0x054
    u32 mColor[4];            // 0x058 (RGBA color table)
    bool mGlobalPause;        // 0x068
    bool mPauseOverride;      // 0x069
    bool mPlaybackState;      // 0x06A
    u8 _pad06B;
    u32 mTexBufYSize;         // 0x06C
    u32 mTexBufCbCrSize;      // 0x070
    void* mSavedTexBufY;      // 0x074
    void* mSavedTexBufCbCr;   // 0x078
    void* mTexBufY;           // 0x07C
    void* mTexBufCbCr;        // 0x080
    u16 mTexWidth;            // 0x084
    u16 mTexHeight;           // 0x086
    u8 _pad088[0x18];
    GXTexObj mTexObjY;        // 0x0A0
    GXTexObj mTexObjCbCr;     // 0x0C0
    u8 _pad0E0[0x20];
    u32 mField100;            // 0x100
    u32 _pad104[8];           // 0x104..0x123
};

class CLibCriMoviePlay : public CWorkThread, public CDeviceVICb, public IErrorWii {
public:
    CLibCriMoviePlay(const char* pName, CWorkThread* pParent);
    virtual ~CLibCriMoviePlay();
    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    virtual void errorWiiCB();
    virtual void viBeginFrame();
    void updateMovieTextures();
    void getMovieWorkSize();
    void OnPauseTrigger(bool pause);
    void setMoviePause(bool pause);

    void setupGXState();
    void setupMovieGfx();
    void findFreeMovieEntry();
    void startMovie();
    void stopMovie();
    void releaseMovieEntry();
    void isMoviePlaying();
    void clearMovieGlobalPause();
    void isMovieGlobalPaused();
    void renderMovie();
    void stopMovieById(int id);
    void releaseEntry(MovieEntry* entry);
    bool isMoviePlaying(int id);
    void unsetPauseFlag(int id);
    bool hasActiveMovie(int id);
    bool renderMovie(int id);
    void updateMovies();
    void setPauseState(bool pause);
    static MovieEntry* findFreeEntry();
    int getWorkSize();
    int startMovie(const char* filename, u32 allocHandle, u32 allocHandle2,
                    bool waitForStart, bool useAlternateBuf);
    void onMovieViBegin();
    void handleViBeginFrame();
    void forwardUpdateMovies();
    void func_8045B3E4();

    // Singleton instance (sda21 lbl_eu_806656E0)
    static CLibCriMoviePlay* sInstance;

    // Movie entries (4 * 0x124, retail base 0x2EC)
    MovieEntry mEntries[4];
    u32 mStreamIdCounter;
    bool mPlaybackState;
    u8 mPauseCounter;
    u32 mType;
};
