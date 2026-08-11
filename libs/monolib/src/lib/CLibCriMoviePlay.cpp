// CLibCriMoviePlay - CRI movie playback manager
// Inherits from CWorkThread (0x1C4) + CDeviceVICb (0x4)

// Same-directory header (retail-verified layout); the include-dir copy under
// libs/monolib/include/ is a stale reconstruction and must not be used here.
#include "libs/monolib/src/lib/CLibCriMoviePlay.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkControl.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CDrawGX.hpp"
#include "monolib/core/CViewRoot.hpp"
#include <revolution/GX.h>
#include <cstring>

using namespace mtl;
using namespace ml;

// CRI Sofdec player API (mwPly*) declarations live in CLibCriMoviePlay.hpp.

// Singleton instance pointer (sbss: lbl_eu_806656E0). Alias keeps the human
// name in source while MWCC emits the retail linker symbol.
#define sInstance lbl_eu_806656E0
CLibCriMoviePlay* lbl_eu_806656E0 = nullptr;

// External globals from sdata2
extern "C" {
    // KColor constants for TEV stages
    extern u32 lbl_eu_8066A4D8;  // KColor0 word0
    extern u32 lbl_eu_8066A4DC;  // KColor0 word1
    extern u32 lbl_eu_8066A4E0;  // KColor1
    extern u32 lbl_eu_8066A4E4;  // KColor2
    extern u32 lbl_eu_8066A4E8;  // KColor3
    extern u32 lbl_eu_8066A4EC;  // KColor4
    extern float lbl_eu_8066A4F0; // 0.0f
    extern float lbl_eu_8066A4F4; // 1.0f
    extern float lbl_eu_8066A4F8; // PAL frame rate / scale
    extern float lbl_eu_8066A4FC; // NTSC frame rate / scale
    extern const char lbl_eu_8052301C[]; // wkUpdate status strings
    extern u32 lbl_eu_8056CF48;  // vtable base
    extern u32 identity__Q22ml6CMat34; // identity matrix
}

// External function declarations
extern "C" {
    // GX cache
    void func_8044B5C0__8CGXCacheFv(void* cache);
    void func_8044A6C8__8CGXCacheFii(void* cache, int a, int b);
    void func_8044BE38__8CGXCacheFv(void* cache);
    void func_80442DA8__9CViewRootFv();
    void* cacheInstance__9CDeviceGX;

    // Memory manager
    void func_80434A4C__Q23mtl10MemManagerFb(bool flag);
    bool isTvFormatPal__9CDeviceVIFv();
    bool hasFlow__12CWorkControlFv();

    // CRT
    void __dl__FPv(void* ptr);
}

// ============================================================================
// Constructor (us-8045dcb0)
// CLibCriMoviePlay::CLibCriMoviePlay(const char*, CWorkThread*)
// ============================================================================
CLibCriMoviePlay::CLibCriMoviePlay(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0)
    , CDeviceVICb()
{
    // Set vtables
    *(u32*)this = (u32)&lbl_eu_8056CF48;         // main vtable
    *(u32*)((u8*)this + 0x1C4) = (u32)&lbl_eu_8056CF48 + 0xA0; // CDeviceVICb vtable

    // Initialize entry fields
    mEntries[0].mActive = false;
    mEntries[0].mPlayerId = 0;

    // Initialize remaining fields
    mStreamIdCounter = 0;
    mPlaybackState = false;
    mPauseCounter = 0;

    // Store singleton
    sInstance = this;

    // Set thread type
    mType = (ThreadType)0x10; // THREAD_CLIBCRIMOVIEPLAY

    // Clear entries and movie state
    memset(&mEntries[0], 0, 0x490); // 4 * 0x124

    // Initialize entry slot IDs (entries 1-3)
    MovieEntry* entry = &mEntries[1];
    for (int i = 1; i < 4; i++) {
        entry->mActive = false;
        entry->mPlayerId = 0;
        entry++;
    }

    // Zero movie state fields
    mStreamIdCounter = 0;
    mPlaybackState = false;
    mPauseCounter = 0;
}

// ============================================================================
// Destructor (us-8045dd94)
// CLibCriMoviePlay::~CLibCriMoviePlay()
// ============================================================================
CLibCriMoviePlay::~CLibCriMoviePlay() {
    // Clear singleton
    sInstance = nullptr;

    // Base sub-object destructors run automatically in reverse declaration
    // order: CDeviceVICb (at 0x1C4) first, then CWorkThread.
}

// ============================================================================
// setupGXState (us-8045de04)
// CLibCriMoviePlay::func_80459DEC()
// Sets up GX TEV pipeline for movie rendering
// ============================================================================
void CLibCriMoviePlay::setupGXState() {
    GXSetNumTexGens(2);

    // Tex coord gen 0 and 1
    GXSetTexCoordGen2((GXTexCoordID)0, (GXTexGenType)1, (GXTexGenSrc)4, 0x3C, (GXBool)0, (GXPTTexMtx)0x7D);
    GXSetTexCoordGen2((GXTexCoordID)1, (GXTexGenType)1, (GXTexGenSrc)4, 0x3C, (GXBool)0, (GXPTTexMtx)0x7D);

    GXSetNumTevStages(4);

    // TEV Stage 0 - YUV to RGB conversion (Y * Cr)
    GXSetTevOrder((GXTevStageID)0, (GXTexCoordID)0, (GXTexMapID)0, (GXChannelID)0xFF);
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)8, (GXTevColorArg)0xE, (GXTevColorArg)2);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)0, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)7, (GXTevAlphaArg)4, (GXTevAlphaArg)6, (GXTevAlphaArg)1);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)1, (GXTevBias)0, (GXTevScale)0, (GXBool)0, (GXTevRegID)0);
    GXSetTevKColorSel((GXTevStageID)0, (GXTevKColorSel)0xC);
    GXSetTevKAlphaSel((GXTevStageID)0, (GXTevKAlphaSel)0x1C);
    GXSetTevSwapMode((GXTevStageID)0, (GXTevSwapSel)0, (GXTevSwapSel)1);

    // TEV Stage 1 - YUV to RGB conversion (Cb component)
    GXSetTevOrder((GXTevStageID)1, (GXTexCoordID)1, (GXTexMapID)1, (GXChannelID)0xFF);
    GXSetTevColorIn((GXTevStageID)1, (GXTevColorArg)0xF, (GXTevColorArg)8, (GXTevColorArg)0xE, (GXTevColorArg)0);
    GXSetTevColorOp((GXTevStageID)1, (GXTevOp)0, (GXTevBias)0, (GXTevScale)1, (GXBool)0, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)1, (GXTevAlphaArg)7, (GXTevAlphaArg)4, (GXTevAlphaArg)6, (GXTevAlphaArg)0);
    GXSetTevAlphaOp((GXTevStageID)1, (GXTevOp)1, (GXTevBias)0, (GXTevScale)0, (GXBool)0, (GXTevRegID)0);
    GXSetTevKColorSel((GXTevStageID)1, (GXTevKColorSel)0xD);
    GXSetTevKAlphaSel((GXTevStageID)1, (GXTevKAlphaSel)0x1D);
    GXSetTevSwapMode((GXTevStageID)1, (GXTevSwapSel)0, (GXTevSwapSel)0);

    // TEV Stage 2 - Color combination
    GXSetTevOrder((GXTevStageID)2, (GXTexCoordID)0, (GXTexMapID)0, (GXChannelID)0xFF);
    GXSetTevColorIn((GXTevStageID)2, (GXTevColorArg)0xF, (GXTevColorArg)8, (GXTevColorArg)0xE, (GXTevColorArg)0);
    GXSetTevColorOp((GXTevStageID)2, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)2, (GXTevAlphaArg)7, (GXTevAlphaArg)4, (GXTevAlphaArg)6, (GXTevAlphaArg)0);
    GXSetTevAlphaOp((GXTevStageID)2, (GXTevOp)1, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetTevKColorSel((GXTevStageID)2, (GXTevKColorSel)0xE);
    GXSetTevKAlphaSel((GXTevStageID)2, (GXTevKAlphaSel)0x1E);
    GXSetTevSwapMode((GXTevStageID)2, (GXTevSwapSel)0, (GXTevSwapSel)2);

    // TEV Stage 3 - Final output
    GXSetTevOrder((GXTevStageID)3, (GXTexCoordID)0xFF, (GXTexMapID)0xFF, (GXChannelID)0xFF);
    GXSetTevColorIn((GXTevStageID)3, (GXTevColorArg)0, (GXTevColorArg)1, (GXTevColorArg)0xE, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)3, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)3, (GXTevAlphaArg)7, (GXTevAlphaArg)7, (GXTevAlphaArg)7, (GXTevAlphaArg)7);
    GXSetTevAlphaOp((GXTevStageID)3, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetTevSwapMode((GXTevStageID)3, (GXTevSwapSel)0, (GXTevSwapSel)0);
    GXSetTevKColorSel((GXTevStageID)3, (GXTevKColorSel)0xF);

    // Set TEV register colors (signed 10-bit)
    GXColorS10 regColor;
    regColor.r = (s16)(lbl_eu_8066A4D8 & 0xFFFF);
    regColor.g = (s16)((lbl_eu_8066A4D8 >> 16) & 0xFFFF);
    regColor.b = (s16)(lbl_eu_8066A4DC & 0xFFFF);
    regColor.a = (s16)((lbl_eu_8066A4DC >> 16) & 0xFFFF);
    GXSetTevColorS10(GX_TEVREG1, regColor);

    // Set KColors
    GXColor kColor0;
    kColor0.r = (u8)(lbl_eu_8066A4E0 & 0xFF);
    kColor0.g = (u8)((lbl_eu_8066A4E0 >> 8) & 0xFF);
    kColor0.b = (u8)((lbl_eu_8066A4E0 >> 16) & 0xFF);
    kColor0.a = (u8)((lbl_eu_8066A4E0 >> 24) & 0xFF);
    GXSetTevKColor(GX_KCOLOR0, kColor0);

    GXColor kColor1;
    kColor1.r = (u8)(lbl_eu_8066A4E4 & 0xFF);
    kColor1.g = (u8)((lbl_eu_8066A4E4 >> 8) & 0xFF);
    kColor1.b = (u8)((lbl_eu_8066A4E4 >> 16) & 0xFF);
    kColor1.a = (u8)((lbl_eu_8066A4E4 >> 24) & 0xFF);
    GXSetTevKColor(GX_KCOLOR1, kColor1);

    GXColor kColor2;
    kColor2.r = (u8)(lbl_eu_8066A4E8 & 0xFF);
    kColor2.g = (u8)((lbl_eu_8066A4E8 >> 8) & 0xFF);
    kColor2.b = (u8)((lbl_eu_8066A4E8 >> 16) & 0xFF);
    kColor2.a = (u8)((lbl_eu_8066A4E8 >> 24) & 0xFF);
    GXSetTevKColor(GX_KCOLOR2, kColor2);

    GXColor kColor3;
    kColor3.r = (u8)(lbl_eu_8066A4EC & 0xFF);
    kColor3.g = (u8)((lbl_eu_8066A4EC >> 8) & 0xFF);
    kColor3.b = (u8)((lbl_eu_8066A4EC >> 16) & 0xFF);
    kColor3.a = (u8)((lbl_eu_8066A4EC >> 24) & 0xFF);
    GXSetTevKColor(GX_KCOLOR3, kColor3);

    // Set swap mode tables
    GXSetTevSwapModeTable(GX_TEV_SWAP0, (GXTevColorChan)3, (GXTevColorChan)3, (GXTevColorChan)3, (GXTevColorChan)3);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, (GXTevColorChan)3, (GXTevColorChan)0, (GXTevColorChan)0, (GXTevColorChan)0);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, (GXTevColorChan)0, (GXTevColorChan)3, (GXTevColorChan)0, (GXTevColorChan)0);

    GXSetNumChans(0);
    GXSetNumIndStages(0);
}

// ============================================================================
// findFreeEntry (us-8045e1c8)
// CLibCriMoviePlay::func_8045A1B0()
// Returns pointer to first free movie entry, or NULL if none available
// ============================================================================
MovieEntry* CLibCriMoviePlay::func_8045A1B0() {
    if (sInstance == nullptr) return nullptr;

    MovieEntry* entry = &sInstance->mEntries[0];
    for (int i = 0; i < 4; i++) {
        if (entry->mPlyHandle != nullptr) return nullptr;
        entry++;
    }

    // Find the first entry with no active handle
    entry = &sInstance->mEntries[0];
    for (int i = 0; i < 4; i++) {
        if (entry->mPlyHandle == nullptr) return entry;
        entry++;
    }

    return nullptr;
}

// ============================================================================
// startMovie (us-8045e278)
// CLibCriMoviePlay::func_8045A260()
// Starts movie playback with given parameters
// Returns stream ID on success, -1 on failure
// ============================================================================
int CLibCriMoviePlay::startMovie(const char* filename, u32 allocHandle,
                                  u32 allocHandle2, bool waitForStart,
                                  bool useAlternateBuf) {
    if (sInstance == nullptr) return -1;

    MovieEntry* entry = func_8045A1B0();
    if (entry == nullptr) return -1;

    // Initialize entry state
    entry->mPlayerId = 0;
    entry->mFlags = 1;

    // Set up CRI cprm structure for SFD playback
    u32* cprm = (u32*)entry->mCprmData;
    cprm[0] = 0x55B80;  // format/mode
    cprm[1] = 0x280;    // width 640
    cprm[2] = 0x1C8;    // height 456
    cprm[3] = 2;        // type

    entry->mFlags = 1;

    // Calculate required work buffer size
    u32 workSize = mwPlyCalcWorkCprmSfd(cprm);
    entry->mWorkSize = workSize;

    // Allocate work buffer
    void* workBuf = MemManager::allocate_tail(allocHandle, workSize, 0x20);
    entry->mTexBufCbCr = workBuf;  // reuse field for work buffer

    if (workBuf == nullptr) {
        func_8045A54C__16CLibCriMoviePlayFv(nullptr, 0);
        return -1;
    }

    // Create CRI movie player
    entry->mPlyHandle = criware_8039FF34(cprm);

    // Copy filename
    size_t nameLen = strlen(filename);
    entry->mPlayerId = nameLen;  // reuse field
    strcpy(entry->mFilename, filename);

    // Store allocation handles
    entry->mAllocHandle = (void*)allocHandle;
    entry->mAllocHandle2 = allocHandle2;

    // Generate unique stream ID
    u32 rawId = *(u32*)entry->mCprmData;
    u32 counter = sInstance->mStreamIdCounter;
    entry->mStreamId = (rawId & 0xFF) | ((counter & 0xFF) << 8);
    sInstance->mStreamIdCounter = counter + 1;

    // Initialize playback state
    entry->mActive = true;
    entry->mTexBufY = nullptr;
    entry->mTexBufCbCr = nullptr;
    entry->mColor[0] = lbl_eu_8066A4F0;
    entry->mColor[1] = lbl_eu_8066A4F0;
    entry->mColor[2] = lbl_eu_8066A4F0;
    entry->mColor[3] = lbl_eu_8066A4F0;
    entry->mAction = 3;
    entry->mField100 = 0;
    entry->mGlobalPause = useAlternateBuf;
    entry->mPauseOverride = false;

    // Start playback
    mwPlyStartFname(entry->mPlyHandle, filename);

    // Handle pause synchronization with other entries
    if (sInstance != nullptr) {
        u32 entryId = entry->mStreamId;
        bool pauseState = entry->mPauseOverride;

        MovieEntry* other = &sInstance->mEntries[0];
        for (int i = 0; i < 4; i++) {
            if (other->mPlyHandle != nullptr) {
                bool shouldPause = false;
                if (other->mStreamId == entryId ||
                    ((other->mStreamId + 0x10000) & 0xFFFF) == (entryId & 0xFFFF)) {
                    other->mPauseOverride = pauseState;
                    shouldPause = pauseState;
                    if (!shouldPause) {
                        if (!other->mGlobalPause) {
                            if (sInstance->mPauseCounter != 0) {
                                shouldPause = true;
                            }
                        } else {
                            shouldPause = true;
                        }
                    }
                    mwPlyPause(other->mPlyHandle, shouldPause ? 1 : 0);
                }
            }
            other++;
        }
    }

    // Wait for playback to start if requested
    if (waitForStart) {
        while (mwPlyGetStat(entry->mPlyHandle) == 1) {
            ADXM_ExecMain();
            VIWaitForRetrace();
        }
    }

    // Update playback state
    sInstance->mPlaybackState = !waitForStart;
    return entry->mStreamId;
}

// ============================================================================
// releaseEntry (us-8045e564)
// CLibCriMoviePlay::func_8045A54C()
// Cleans up and releases movie entries. Body lives in the retail-named
// wrapper func_8045A54C__16CLibCriMoviePlayFv (see below) so MWCC keeps it
// outlined (large body) and other functions call it directly.
// ============================================================================
// isMoviePlaying (us-8045e65c)
// CLibCriMoviePlay::func_8045A644()
// Returns true if a movie with the given ID is currently playing
// ============================================================================
bool CLibCriMoviePlay::isMoviePlaying(int id) {
    if (sInstance == nullptr) return false;

    MovieEntry* entries = &sInstance->mEntries[0];

    // Check entry 0
    if (entries[0].mPlyHandle != nullptr) {
        if (entries[0].mStreamId == (u32)id ||
            (((u32)id + 0x10000) & 0xFFFF) == 0xFFFF) {
            return true;
        }
    }

    // Check entries 1-3
    MovieEntry* cur = &sInstance->mEntries[1];
    for (int i = 1; i < 4; i++) {
        if (cur->mPlyHandle != nullptr) {
            if (cur->mStreamId == (u32)id ||
                (((u32)id + 0x10000) & 0xFFFF) == 0xFFFF) {
                return true;
            }
        }
        cur++;
    }

    return false;
}

// ============================================================================
// unsetPauseFlag (us-8045e720)
// CLibCriMoviePlay::func_8045A708()
// Clears the global pause flag for the entry matching the given ID
// ============================================================================
void CLibCriMoviePlay::unsetPauseFlag(int id) {
    if (sInstance == nullptr) return;

    MovieEntry* entries = &sInstance->mEntries[0];

    for (int i = 0; i < 4; i++) {
        MovieEntry* cur = &entries[i];
        if (cur->mPlyHandle == nullptr) continue;

        if (cur->mStreamId == (u32)id) {
            // Found matching entry - clear its global pause
            if (!cur->mGlobalPause) return;

            cur->mGlobalPause = false;
            bool overrideState = cur->mPauseOverride;

            // Update pause state for all entries
            MovieEntry* other = &sInstance->mEntries[0];
            for (int j = 0; j < 4; j++) {
                if (other->mPlyHandle != nullptr) {
                    other->mPauseOverride = overrideState;
                    bool shouldPause = overrideState;
                    if (!shouldPause) {
                        if (!other->mGlobalPause) {
                            if (sInstance->mPauseCounter != 0) {
                                shouldPause = true;
                            }
                        }
                    }
                    mwPlyPause(other->mPlyHandle, shouldPause ? 1 : 0);
                }
                other++;
            }
            return;
        }
    }
}

// ============================================================================
// hasActiveMovie (us-8045e810)
// CLibCriMoviePlay::func_8045A7F8()
// Returns true if any entry has active playback matching the ID
// ============================================================================
bool CLibCriMoviePlay::hasActiveMovie(int id) {
    if (sInstance == nullptr) return false;

    MovieEntry* entries = &sInstance->mEntries[0];

    // Check entry 0 with active flag
    if (entries[0].mPlyHandle != nullptr) {
        if (entries[0].mStreamId == (u32)id && entries[0].mGlobalPause) {
            return true;
        }
    }

    // Check entries 1-3
    MovieEntry* cur = &sInstance->mEntries[1];
    for (int i = 1; i < 4; i++) {
        if (cur->mPlyHandle != nullptr) {
            if (cur->mStreamId == (u32)id && cur->mGlobalPause) {
                return true;
            }
        }
        cur++;
    }

    return false;
}

// ============================================================================
// renderMovie (us-8045e8e0)
// CLibCriMoviePlay::func_8045A8C8()
// Renders a movie frame to the screen using GX
// Returns true on success, false if movie not found
// ============================================================================
bool CLibCriMoviePlay::renderMovie(int id) {
    if (sInstance == nullptr) return false;

    MovieEntry* entries = &sInstance->mEntries[0];

    for (int i = 0; i < 4; i++) {
        MovieEntry* cur = &entries[i];
        if (cur->mPlyHandle == nullptr) continue;
        if (cur->mStreamId != (u32)id) continue;
        if (cur->mTexBufY == nullptr) continue;

        // Flush GX cache
        func_8044B5C0__8CGXCacheFv(cacheInstance__9CDeviceGX);

        // Load textures
        GXLoadTexObj(&cur->mTexObjY, GX_TEXMAP0);
        GXLoadTexObj(&cur->mTexObjCbCr, GX_TEXMAP1);

        // Setup GX state for movie rendering
        setupGXState();

        // Configure GX cache
        func_8044A6C8__8CGXCacheFii(cacheInstance__9CDeviceGX, 0, 0);
        GXSetZMode(false, GX_ALWAYS, false);

        // Load identity texture matrix
        GXLoadTexMtxImm((const float(*)[4])&identity__Q22ml6CMat34, GX_TEXMTX0, GX_MTX_3x4);

        // Setup vertex descriptors
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXLoadPosMtxImm((const float(*)[4])&identity__Q22ml6CMat34, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);

        // Setup vertex format
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_S16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

        // Draw textured quad (GX_QUADS = 0x98)
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        s16 x = (s16)cur->mTexWidth;
        s16 y = (s16)cur->mTexHeight;
        s16 x2 = (s16)cur->mTexWidth;
        s16 y2 = (s16)cur->mTexHeight;

        // Vertex 0: (0, 0)
        GXPosition2s16(0, 0);
        GXTexCoord2f32(0.0f, 0.0f);

        // Vertex 1: (width, 0)
        GXPosition2s16(x2, 0);
        GXTexCoord2f32(lbl_eu_8066A4F4, 0.0f);

        // Vertex 2: (width, height)
        GXPosition2s16(x2, y2);
        GXTexCoord2f32(lbl_eu_8066A4F4, lbl_eu_8066A4F4);

        // Vertex 3: (0, height)
        GXPosition2s16(0, y2);
        GXTexCoord2f32(0.0f, lbl_eu_8066A4F4);

        // Flush and render
        func_8044BE38__8CGXCacheFv(cacheInstance__9CDeviceGX);
        func_80442DA8__9CViewRootFv();

        // Draw overlay rectangle with color
        CDrawGX drawGx;
        drawGx.func_80456570(0);
        drawGx.func_8045657C(0);
        drawGx.setCol((const CCol4&)cur->mColor[0]);
        drawGx.renderRect((const CRect16&)cur->mTexWidth);

        // Finalize
        func_8044BE38__8CGXCacheFv(cacheInstance__9CDeviceGX);
        func_80442DA8__9CViewRootFv();
        drawGx.~CDrawGX();

        return true;
    }

    return false;
}

// ============================================================================
// updateMovies (us-8045eeb0)
// CLibCriMoviePlay::func_8045AE84()
// Updates texture buffers for all active movie entries
// ============================================================================
void CLibCriMoviePlay::updateMovies() {
    MovieEntry* entry = &mEntries[0];

    for (int i = 0; i < 4; i++) {
        if (entry->mPlyHandle == nullptr) goto next;
        if (!entry->mActive) goto next;

        // Get current frame data
        u32 frameData[8];
        mwPlyGetCurFrm(entry->mPlyHandle, frameData);

        if (frameData[0] == 0) goto next;

        // Check if textures need setup
        if (entry->mTexBufY != nullptr && entry->mTexBufCbCr != nullptr) {
            goto skipAlloc;
        }

        // Calculate texture dimensions from frame data
        int width_raw = (int)frameData[3];
        int width_half = (width_raw + (width_raw >> 31)) >> 1;
        int width_field = (width_half << 1);
        if ((width_field & 0x1F) != 0) {
            width_field = (width_field + 0x20) - (width_field & 0x1F);
        }
        u16 texWidth = (u16)(width_half & 0xFFFF);
        u16 texWidthScaled = (u16)(width_field & 0xFFFF);

        int height_raw = (int)frameData[4];
        int height_half = (height_raw + (height_raw >> 31)) >> 1;
        int height_field = (height_half << 1);
        if ((height_field & 0x1F) != 0) {
            height_field = (height_field + 0x20) - (height_field & 0x1F);
        }
        u16 texHeight = (u16)(height_half & 0xFFFF);
        u16 texHeightScaled = (u16)(height_field & 0xFFFF);

        entry->mTexWidth = texWidthScaled;
        entry->mTexHeight = height_raw;

        if (entry->mTexBufY == nullptr) {
            // Allocate Y texture buffer
            u32 yBufSize = GXGetTexBufferSize(texWidthScaled, height_raw,
                                               GX_TF_RGBA8, GX_FALSE, (u8)0);
            entry->mTexBufYSize = yBufSize;

            if (entry->mAction == 0) {
                void* buf = MemManager::allocate_tail(
                    (u32)entry->mAllocHandle, yBufSize, 0x20);
                entry->mTexBufY = buf;
            } else if (entry->mAction == 3) {
                MemManager::setOptimalAlloc(true);
                void* buf = MemManager::allocate_head(
                    entry->mAllocHandle2, yBufSize, 0x20);
                if (buf == nullptr) {
                    mtl::ALLOC_HANDLE mem2 = MemManager::getHandleMEM2();
                    buf = MemManager::allocate_head(mem2, yBufSize, 0x20);
                }
                entry->mTexBufY = buf;
                MemManager::setOptimalAlloc(false);
            } else {
                if (entry->mField100 != 0) {
                    u32 aligned = entry->mField100;
                    if ((aligned & 0x1F) != 0) {
                        aligned = (aligned + 0x20) - (aligned & 0x1F);
                    }
                    entry->mTexBufY = (void*)aligned;
                }
            }

            // Initialize Y texture object
            GXInitTexObj(&entry->mTexObjY, entry->mTexBufY, texWidthScaled, height_raw, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXInitTexObjLOD(&entry->mTexObjY, GX_LINEAR, GX_LINEAR, lbl_eu_8066A4F0, 0.0f, 0.0f, (GXBool)0, (GXBool)0, (GXAnisotropy)0);
        }

skipAlloc:
        if (entry->mTexBufY != nullptr) {
            // Allocate CbCr texture buffer
            u32 cbcrBufSize = GXGetTexBufferSize(texWidth, texHeight,
                                                  (GXTexFmt)10 /* GX_TF_YUV422 (not in vendored GXTypes.h) */, GX_FALSE, (u8)0);
            entry->mTexBufCbCrSize = cbcrBufSize;

            if (entry->mAction == 0) {
                void* buf = MemManager::allocate_tail(
                    (u32)entry->mAllocHandle, cbcrBufSize, 0x20);
                entry->mTexBufCbCr = buf;
            } else if (entry->mAction == 3) {
                MemManager::setOptimalAlloc(true);
                void* buf = MemManager::allocate_head(entry->mAllocHandle2, cbcrBufSize, 0x20);
                if (buf == nullptr) {
                    mtl::ALLOC_HANDLE mem2 = MemManager::getHandleMEM2();
                    buf = MemManager::allocate_head(mem2, cbcrBufSize, 0x20);
                }
                entry->mTexBufCbCr = buf;
                MemManager::setOptimalAlloc(false);
            } else {
                // Use Y buffer end, aligned
                u32 addr = (u32)entry->mTexBufY + entry->mTexBufYSize;
                if ((addr & 0x1F) != 0) {
                    addr = (addr + 0x20) - (addr & 0x1F);
                }
                entry->mTexBufCbCr = (void*)addr;
            }

            // Initialize CbCr texture object
            GXInitTexObj(&entry->mTexObjCbCr, entry->mTexBufCbCr, texWidth, texHeight, (GXTexFmt)10 /* GX_TF_YUV422 (not in vendored GXTypes.h) */, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXInitTexObjLOD(&entry->mTexObjCbCr, GX_LINEAR, GX_LINEAR, lbl_eu_8066A4F0, 0.0f, 0.0f, (GXBool)0, (GXBool)0, (GXAnisotropy)0);
        }

        // Convert and upload frame data
        if (entry->mTexBufCbCr != nullptr && entry->mTexBufY != nullptr) {
            mwPlyFxSetOutBufPitchHeight(entry->mPlyHandle,
                                        entry->mTexWidth, entry->mTexHeight);
            mwPlyFxCnvFrmY84C44(entry->mPlyHandle, frameData,
                                entry->mTexBufY, entry->mTexBufCbCr);
            DCFlushRangeNoSync(entry->mTexBufY, entry->mTexBufYSize);
            DCFlushRangeNoSync(entry->mTexBufCbCr, entry->mTexBufCbCrSize);
            mwPlyRelCurFrm(entry->mPlyHandle);
        }

next:
        entry++;
    }
}

// ============================================================================
// func_8045B1DC (us-8045f208)
// Empty function - just returns
// ============================================================================
// (declared inline in header)

// ============================================================================
// getWorkSize (us-8045f20c)
// CLibCriMoviePlay::func_8045B1E0()
// Calculates required work buffer size for movie playback
// ============================================================================
int CLibCriMoviePlay::func_8045B1E0() {
    // Set up temporary cprm structure (0x4C bytes on the stack)
    u32 cprm[0x13];
    memset(cprm, 0, 0x4C);

    cprm[8] = 0;
    cprm[0] = 1;       // format
    cprm[1] = 0x5B8D80; // mode
    cprm[2] = 0x280;   // width 640
    cprm[3] = 0x1C8;   // height 456
    cprm[4] = 2;       // type
    cprm[5] = 1;       // flags

    u32 size = mwPlyCalcWorkCprmSfd(cprm);
    return size + 0x100;
}

// ============================================================================
// OnPauseTrigger (us-8045f27c)
// CLibCriMoviePlay::OnPauseTrigger(bool pause)
// Handles pause trigger events
// ============================================================================
void CLibCriMoviePlay::OnPauseTrigger(bool pause) {
    if (pause) {
        mPauseCounter++;
    } else {
        int newCount = mPauseCounter - 1;
        mPauseCounter = newCount;
        if (newCount < 0) {
            mPauseCounter = 0;
        }
    }

    // Update pause state for all entries
    if (sInstance != nullptr) {
        MovieEntry* entry = &sInstance->mEntries[0];
        for (int i = 0; i < 4; i++) {
            if (entry->mPlyHandle != nullptr) {
                bool shouldPause = false;
                if (entry->mPauseOverride) {
                    shouldPause = true;
                } else if (!entry->mGlobalPause) {
                    if (sInstance->mPauseCounter != 0) {
                        shouldPause = true;
                    }
                }
                mwPlyPause(entry->mPlyHandle, shouldPause ? 1 : 0);
            }
            entry++;
        }
    }
}

// ============================================================================
// setPauseState (us-8045f33c)
// CLibCriMoviePlay::func_8045B310(bool pause)
// Sets pause state for entries matching the given ID
// ============================================================================
void CLibCriMoviePlay::setPauseState(bool pause) {
    if (sInstance == nullptr) return;

    MovieEntry* entry = &sInstance->mEntries[0];
    for (int i = 0; i < 4; i++) {
        if (entry->mPlyHandle != nullptr) {
            entry->mPauseOverride = pause;
            bool shouldPause = pause;
            if (!shouldPause) {
                if (!entry->mGlobalPause) {
                    if (sInstance->mPauseCounter != 0) {
                        shouldPause = true;
                    }
                }
            }
            mwPlyPause(entry->mPlyHandle, shouldPause ? 1 : 0);
        }
        entry++;
    }
}

// ============================================================================
// wkUpdate (us-8045eb54)
// CLibCriMoviePlay::wkUpdate()
// Main update loop - processes movie playback state
// ============================================================================
void CLibCriMoviePlay::wkUpdate() {
    bool hasActive = false;
    MovieEntry* entry = &mEntries[0];

    for (int i = 0; i < 4; i++) {
        if (entry->mPlyHandle == nullptr) {
            // Clean up freed entry
            MemManager::setOptimalAlloc(false);
            if (entry->mSavedTexBufY != nullptr) {
                MemManager::deallocate(entry->mSavedTexBufY);
                entry->mSavedTexBufY = nullptr;
            }
            if (entry->mSavedTexBufCbCr != nullptr) {
                MemManager::deallocate(entry->mSavedTexBufCbCr);
                entry->mSavedTexBufCbCr = nullptr;
            }
            MemManager::setOptimalAlloc(true);
            continue;
        }

        // Get playback status
        int stat = mwPlyGetStat(entry->mPlyHandle);
        entry->mPlaybackState = stat;

        if (stat == 3 || stat == 4 || stat == 0) {
            // Playback ended or error - release entry
            func_8045A54C__16CLibCriMoviePlayFv(entry, 0);
            continue;
        }

        // Active playback - update volume
        int volume = mwPlyGetOutVol(entry->mPlyHandle);

        // Build status string for debug
        char statusBuf[0x40];
        u32 statusLen = 0;
        const char* statusStr = nullptr;

        switch (entry->mPlaybackState) {
        case 0:
            statusStr = lbl_eu_8052301C;
            statusLen = strlen(statusStr);
            strcpy(statusBuf, statusStr);
            break;
        case 1:
            statusStr = lbl_eu_8052301C + 7;
            statusLen = strlen(statusStr);
            strcpy(statusBuf, statusStr);
            hasActive = true;
            break;
        case 2:
            statusStr = lbl_eu_8052301C + 0x12;
            statusLen = strlen(statusStr);
            strcpy(statusBuf, statusStr);
            break;
        case 3:
            statusStr = lbl_eu_8052301C + 0x19;
            statusLen = strlen(statusStr);
            strcpy(statusBuf, statusStr);
            break;
        case 4:
            statusStr = lbl_eu_8052301C + 0x22;
            statusLen = strlen(statusStr);
            strcpy(statusBuf, statusStr);
            break;
        }

        // Apply volume based on flow control
        bool flowActive = hasFlow__12CWorkControlFv();
        int volAdjust = flowActive ? 0 : -960;
        mwPlySetOutVol(entry->mPlyHandle, volAdjust);
    }

    mPlaybackState = hasActive;
}

// ============================================================================
// wkStandbyLogin (us-8045ed1c)
// CLibCriMoviePlay::wkStandbyLogin()
// ============================================================================
bool CLibCriMoviePlay::wkStandbyLogin() {
    // Initialize SFD effects with the TV-format frame rate
    u32 fxData[8];
    memset(fxData, 0, 0x20);

    float frameRate = CDeviceVI::isTvFormatPal()
                      ? lbl_eu_8066A4F8 : lbl_eu_8066A4FC;
    *(float*)fxData = frameRate;
    fxData[3] = 1;

    mwPlyInitSfdFx(fxData);

    // Call base class login
    return CWorkThread::wkStandbyLogin();
}

// ============================================================================
// wkStandbyLogout (us-8045ed88)
// CLibCriMoviePlay::wkStandbyLogout()
// ============================================================================
bool CLibCriMoviePlay::wkStandbyLogout() {
    // Check if any entries are still active
    CWorkThread* child = mChildren.front();
    if (child->mChildren.front() == child) {
        // Check all entries for active handles
        bool anyActive = false;
        MovieEntry* entry = &sInstance->mEntries[0];
        for (int i = 0; i < 4; i++) {
            if (entry->mPlyHandle != nullptr) {
                anyActive = true;
                break;
            }
            entry++;
        }

        if (!anyActive) {
            // Clean up all entries
            MovieEntry* cur = &mEntries[0];
            for (int i = 0; i < 4; i++) {
                MemManager::setOptimalAlloc(false);
                if (cur->mSavedTexBufY != nullptr) {
                    MemManager::deallocate(cur->mSavedTexBufY);
                    cur->mSavedTexBufY = nullptr;
                }
                if (cur->mSavedTexBufCbCr != nullptr) {
                    MemManager::deallocate(cur->mSavedTexBufCbCr);
                    cur->mSavedTexBufCbCr = nullptr;
                }
                MemManager::setOptimalAlloc(true);
                cur++;
            }

            return CWorkThread::wkStandbyLogout();
        }
    }

    return false;
}

// ============================================================================
// viBeginFrame (us-8045f400)
// CDeviceVICb virtual override - thunk adjusts this by -0x1C4
// ============================================================================
void CLibCriMoviePlay::viBeginFrame() {
    // Called every video frame - update movie textures
    updateMovies();
}

// ============================================================================
// External linkage wrappers (called from CLibCri forwarding stubs)
// ============================================================================
extern "C" {
    // Forward declarations (definitions below, in retail order)
    void func_8045A54C__16CLibCriMoviePlayFv(MovieEntry* entry, int flags);

    void __ct__CLibCriMoviePlay(const char* name, CWorkThread* parent) {
        new ((void*)0) CLibCriMoviePlay(name, parent);
    }

    void func_80459DEC__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        self->setupGXState();
    }

    int func_8045A260__16CLibCriMoviePlayFv(CLibCriMoviePlay* self,
        const char* filename, u32 allocHandle, u32 allocHandle2,
        bool waitForStart, bool useAlternateBuf) {
        return self->startMovie(filename, allocHandle, allocHandle2,
                               waitForStart, useAlternateBuf);
    }

    void func_8045A48C__16CLibCriMoviePlayFv(int id) {
        MovieEntry* entry = nullptr;
        if (sInstance != nullptr) {
            if ((u32)id + 0x10000 == 0xFFFF) {
                entry = nullptr;
            } else {
                MovieEntry* cur = &sInstance->mEntries[0];
                if (cur->mPlayerId == (u32)id) {
                    entry = cur;
                } else {
                    cur++;
                    if (cur->mPlayerId == (u32)id) {
                        entry = cur;
                    } else {
                        cur++;
                        if (cur->mPlayerId == (u32)id) {
                            entry = cur;
                        } else {
                            cur++;
                            if (cur->mPlayerId == (u32)id) {
                                entry = cur;
                            }
                        }
                    }
                }
            }
        }

        if (entry != nullptr) {
            mwPlyStop(entry->mPlyHandle);
            func_8045A54C__16CLibCriMoviePlayFv(entry, 0);
        }
    }

    void func_8045A54C__16CLibCriMoviePlayFv(MovieEntry* entry, int flags) {
        if (sInstance == nullptr) return;

        // Lock memory manager
        MemManager::setOptimalAlloc(false);

        MovieEntry* cur = &sInstance->mEntries[0];
        for (int i = 0; i < 4; i++) {
            if (entry == nullptr || entry == cur) {
                // Destroy CRI player handle
                if (cur->mPlyHandle != nullptr) {
                    criware_803A09B4(cur->mPlyHandle);
                    cur->mPlyHandle = nullptr;
                }

                // Handle different action states
                if (cur->mAction == 0 || cur->mAction == 3) {
                    // Free work buffer and save texture state
                    if (cur->mTexBufCbCr != nullptr) {
                        MemManager::deallocate(cur->mTexBufCbCr);
                        cur->mTexBufCbCr = nullptr;
                    }
                    cur->mSavedTexBufY = cur->mTexBufY;
                    cur->mSavedTexBufCbCr = cur->mTexBufCbCr;
                } else if (cur->mAction == 2) {
                    // Free alternate buffer
                    if (cur->mTexBufCbCr != nullptr) {
                        MemManager::deallocate(cur->mTexBufCbCr);
                        cur->mTexBufCbCr = nullptr;
                    }
                }
            }
            cur++;
        }

        // Unlock memory manager
        MemManager::setOptimalAlloc(true);
    }

    bool func_8045A644__16CLibCriMoviePlayFv(CLibCriMoviePlay* self, int id) {
        return self->isMoviePlaying(id);
    }

    void func_8045A708__16CLibCriMoviePlayFv(CLibCriMoviePlay* self, int id) {
        self->unsetPauseFlag(id);
    }

    bool func_8045A7F8__16CLibCriMoviePlayFv(CLibCriMoviePlay* self, int id) {
        return self->hasActiveMovie(id);
    }

    bool func_8045A8C8__16CLibCriMoviePlayFv(CLibCriMoviePlay* self, int id) {
        return self->renderMovie(id);
    }

    void wkUpdate__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        self->wkUpdate();
    }

    bool wkStandbyLogout__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        return self->wkStandbyLogout();
    }

    void func_8045AE84__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        self->updateMovies();
    }

    void func_8045B1DC__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        // Empty
    }

    void OnPauseTrigger__16CLibCriMoviePlayFb(CLibCriMoviePlay* self, bool pause) {
        self->OnPauseTrigger(pause);
    }

    void func_8045B310__16CLibCriMoviePlayFv(CLibCriMoviePlay* self, bool pause) {
        self->setPauseState(pause);
    }

    // Virtual thunks (adjust this pointer by -0x1C4)
    void func_8045B3D4__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        // Thunk for CDeviceVICb::viBeginFrame
        ((CLibCriMoviePlay*)((u8*)self - 0x1C4))->func_8045B1DC();
    }

    void func_8045B3DC__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        // Thunk for CDeviceVICb update
        ((CLibCriMoviePlay*)((u8*)self - 0x1C4))->updateMovies();
    }

    void func_8045B3E4__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        // Thunk for CDeviceVICb destructor
        ((CLibCriMoviePlay*)((u8*)self - 0x1C4))->~CLibCriMoviePlay();
    }
}
