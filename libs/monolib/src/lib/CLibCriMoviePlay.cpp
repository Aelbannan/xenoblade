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

// Own retail-named data (defined at the bottom of this file, retail order).
extern "C" {
    extern u32 lbl_eu_8056CF48[48];          // __vt__16CLibCriMoviePlay (+ sub-vtable)
    extern u32 lbl_eu_80663798[2];           // .sdata RTTI locator pair
    extern const char lbl_eu_8052301C[0x34]; // wkUpdate status strings
}

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
// func_80459DEC (us-8045de04)
// Sets up the GX TEV pipeline for movie rendering (YUV->RGB).
// Retail ABI: (r3 = stage-1 tex map, r4 = stage-0/2 tex map); the caller
// passes (GX_TEXMAP0, GX_TEXMAP1). The TEVREG1/KColor structs are filled
// with direct word copies from the sdata2 pool.
// ============================================================================
extern "C" void func_80459DEC__16CLibCriMoviePlayFv(unsigned int texMapStage1, unsigned int texMapStage02) {
    GXSetNumTexGens(2);

    // Tex coord gen 0 and 1
    GXSetTexCoordGen2((GXTexCoordID)0, (GXTexGenType)1, (GXTexGenSrc)4, 0x3C, (GXBool)0, (GXPTTexMtx)0x7D);
    GXSetTexCoordGen2((GXTexCoordID)1, (GXTexGenType)1, (GXTexGenSrc)4, 0x3C, (GXBool)0, (GXPTTexMtx)0x7D);

    GXSetNumTevStages(4);

    // TEV Stage 0 - YUV->RGB (CbCr texture)
    GXSetTevOrder((GXTevStageID)0, (GXTexCoordID)0, (GXTexMapID)texMapStage02, (GXChannelID)0xFF);
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)8, (GXTevColorArg)0xE, (GXTevColorArg)2);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)0, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)7, (GXTevAlphaArg)4, (GXTevAlphaArg)6, (GXTevAlphaArg)1);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)1, (GXTevBias)0, (GXTevScale)0, (GXBool)0, (GXTevRegID)0);
    GXSetTevKColorSel((GXTevStageID)0, (GXTevKColorSel)0xC);
    GXSetTevKAlphaSel((GXTevStageID)0, (GXTevKAlphaSel)0x1C);
    GXSetTevSwapMode((GXTevStageID)0, (GXTevSwapSel)0, (GXTevSwapSel)1);

    // TEV Stage 1 - YUV->RGB (Y texture)
    GXSetTevOrder((GXTevStageID)1, (GXTexCoordID)1, (GXTexMapID)texMapStage1, (GXChannelID)0xFF);
    GXSetTevColorIn((GXTevStageID)1, (GXTevColorArg)0xF, (GXTevColorArg)8, (GXTevColorArg)0xE, (GXTevColorArg)0);
    GXSetTevColorOp((GXTevStageID)1, (GXTevOp)0, (GXTevBias)0, (GXTevScale)1, (GXBool)0, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)1, (GXTevAlphaArg)7, (GXTevAlphaArg)4, (GXTevAlphaArg)6, (GXTevAlphaArg)0);
    GXSetTevAlphaOp((GXTevStageID)1, (GXTevOp)0, (GXTevBias)0, (GXTevScale)1, (GXBool)0, (GXTevRegID)0);
    GXSetTevKColorSel((GXTevStageID)1, (GXTevKColorSel)0xD);
    GXSetTevKAlphaSel((GXTevStageID)1, (GXTevKAlphaSel)0x1D);
    GXSetTevSwapMode((GXTevStageID)1, (GXTevSwapSel)0, (GXTevSwapSel)0);

    // TEV Stage 2 - color combination (CbCr texture)
    GXSetTevOrder((GXTevStageID)2, (GXTexCoordID)0, (GXTexMapID)texMapStage02, (GXChannelID)0xFF);
    GXSetTevColorIn((GXTevStageID)2, (GXTevColorArg)0xF, (GXTevColorArg)8, (GXTevColorArg)0xE, (GXTevColorArg)0);
    GXSetTevColorOp((GXTevStageID)2, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)2, (GXTevAlphaArg)7, (GXTevAlphaArg)4, (GXTevAlphaArg)6, (GXTevAlphaArg)0);
    GXSetTevAlphaOp((GXTevStageID)2, (GXTevOp)1, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetTevKColorSel((GXTevStageID)2, (GXTevKColorSel)0xE);
    GXSetTevKAlphaSel((GXTevStageID)2, (GXTevKAlphaSel)0x1E);
    GXSetTevSwapMode((GXTevStageID)2, (GXTevSwapSel)0, (GXTevSwapSel)2);

    // TEV Stage 3 - final output
    GXSetTevOrder((GXTevStageID)3, (GXTexCoordID)0xFF, (GXTexMapID)0xFF, (GXChannelID)0xFF);
    GXSetTevColorIn((GXTevStageID)3, (GXTevColorArg)0, (GXTevColorArg)1, (GXTevColorArg)0xE, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)3, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)3, (GXTevAlphaArg)7, (GXTevAlphaArg)7, (GXTevAlphaArg)7, (GXTevAlphaArg)7);
    GXSetTevAlphaOp((GXTevStageID)3, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXSetTevSwapMode((GXTevStageID)3, (GXTevSwapSel)0, (GXTevSwapSel)0);
    GXSetTevKColorSel((GXTevStageID)3, (GXTevKColorSel)0xF);

    // Set TEV register color (single pooled-deref temporary reproduces
    // retail's 2-store shape; the TEVREG1 enum value in this header tree is
    // off by one from retail). OPEN ITEM: retail's second word reloc points
    // at lbl_eu_8066A4DC@0 while this form emits lbl_eu_8066A4D8@4 (same
    // address, different anchor) - the witness reloc-gate rejects it.
    GXSetTevColorS10((GXTevRegID)1, *(const GXColorS10*)&lbl_eu_8066A4D8);

    GXSetTevKColor((GXTevKColorID)0, *(const GXColor*)&lbl_eu_8066A4E0);
    GXSetTevKColor((GXTevKColorID)1, *(const GXColor*)&lbl_eu_8066A4E4);
    GXSetTevKColor((GXTevKColorID)2, *(const GXColor*)&lbl_eu_8066A4E8);
    GXSetTevKColor((GXTevKColorID)3, *(const GXColor*)&lbl_eu_8066A4EC);

    // Set swap mode tables
    GXSetTevSwapModeTable(GX_TEV_SWAP0, (GXTevColorChan)0, (GXTevColorChan)1, (GXTevColorChan)2, (GXTevColorChan)3);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, (GXTevColorChan)0, (GXTevColorChan)3, (GXTevColorChan)3, (GXTevColorChan)3);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, (GXTevColorChan)0, (GXTevColorChan)0, (GXTevColorChan)3, (GXTevColorChan)0);

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
extern "C" int func_8045A260__16CLibCriMoviePlayFv(const char* filename, u32 allocHandle,
                                  u32 allocHandle2, bool waitForStart,
                                  bool useAlternateBuf) {
    if (sInstance == nullptr) return -1;

    MovieEntry* entry = CLibCriMoviePlay::func_8045A1B0();
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
// ============================================================================
// isMoviePlaying / unsetPauseFlag / hasActiveMovie / setPauseState
// Implemented directly as the retail-named wrappers at the bottom of this
// file (their retail symbols take the arguments in registers that do not
// match a member-function ABI).
// ============================================================================
// renderMovie (us-8045e8e0)
// CLibCriMoviePlay::func_8045A8C8()
// Renders a movie frame to the screen using GX
// Returns true on success, false if movie not found
// ============================================================================
extern "C" bool func_8045A8C8__16CLibCriMoviePlayFv(int id) {
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

        // Setup GX state for movie rendering (stage-1 map = Y, stage-0/2 = CbCr)
        func_80459DEC__16CLibCriMoviePlayFv(GX_TEXMAP0, GX_TEXMAP1);

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
        GXTexCoord2f32(lbl_eu_8066A4F0, lbl_eu_8066A4F0);

        // Vertex 1: (width, 0)
        GXPosition2s16(x2, 0);
        GXTexCoord2f32(lbl_eu_8066A4F4, lbl_eu_8066A4F0);

        // Vertex 2: (width, height)
        GXPosition2s16(x2, y2);
        GXTexCoord2f32(lbl_eu_8066A4F4, lbl_eu_8066A4F4);

        // Vertex 3: (0, height)
        GXPosition2s16(0, y2);
        GXTexCoord2f32(lbl_eu_8066A4F0, lbl_eu_8066A4F4);

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
extern "C" void func_8045AE84__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
    MovieEntry* entry = &self->mEntries[0];

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
            GXInitTexObjLOD(&entry->mTexObjY, GX_LINEAR, GX_LINEAR, lbl_eu_8066A4F0, lbl_eu_8066A4F0, lbl_eu_8066A4F0, (GXBool)0, (GXBool)0, (GXAnisotropy)0);
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
            GXInitTexObjLOD(&entry->mTexObjCbCr, GX_LINEAR, GX_LINEAR, lbl_eu_8066A4F0, lbl_eu_8066A4F0, lbl_eu_8066A4F0, (GXBool)0, (GXBool)0, (GXAnisotropy)0);
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
        u32 i;
        MovieEntry* entry = &sInstance->mEntries[0];
        for (i = 0; i < 4; i++) {
            if (entry->mPlyHandle != nullptr) {
                int pa = 0;
                if (!entry->mPauseOverride && !entry->mGlobalPause &&
                    sInstance->mPauseCounter == 0) {
                    // no pause requested - leave pa at 0
                } else {
                    pa = 1;
                }
                mwPlyPause(entry->mPlyHandle, pa);
            }
            entry++;
        }
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
    func_8045AE84__16CLibCriMoviePlayFv(this);
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

// Finds the movie entry whose player ID matches id (id == -1 matches none).

    void func_8045A48C__16CLibCriMoviePlayFv(int id) {
        MovieEntry* entry = nullptr;
        if (sInstance != nullptr && (u32)id + 0x10000 != 0xFFFF) {
            entry = &sInstance->mEntries[0];
            if (entry->mPlayerId != (u32)id) {
                if ((++entry)->mPlayerId != (u32)id) {
                    if ((++entry)->mPlayerId != (u32)id) {
                        if ((++entry)->mPlayerId != (u32)id) {
                            entry = nullptr;
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

    // Returns true if any active movie matches id (id == -1 matches any).
    bool func_8045A644__16CLibCriMoviePlayFv(int id) {
        if (sInstance == nullptr) return false;

        for (int i = 0; i < 4; i++) {
            const MovieEntry& entry = sInstance->mEntries[i];
            if (entry.mPlyHandle != nullptr &&
                (entry.mPlayerId == (u32)id || (u32)id + 0x10000 == 0xFFFF)) {
                return true;
            }
        }

        return false;
    }

    void func_8045A708__16CLibCriMoviePlayFv(int id) {
        if (sInstance == nullptr) return;

        // Clear the global pause flag on the movie matching id.
        MovieEntry* entry = &sInstance->mEntries[0];
        for (int i = 0; i < 4; i++) {
            if (entry->mPlyHandle != nullptr && entry->mPlayerId == (u32)id) {
                if (!entry->mGlobalPause) return;

                entry->mGlobalPause = false;
                bool overrideState = entry->mPauseOverride;

                MovieEntry* other = &sInstance->mEntries[0];
                for (int j = 0; j < 4; j++) {
                    if (other->mPlyHandle != nullptr) {
                        other->mPauseOverride = overrideState;
                        int pa = 0;
                        if (!overrideState) {
                            if (!other->mGlobalPause) {
                                if (sInstance->mPauseCounter != 0) {
                                    pa = 1;
                                }
                            } else {
                                pa = 1;
                            }
                        }
                        mwPlyPause(other->mPlyHandle, pa);
                    }
                    other++;
                }
                return;
            }
            entry++;
        }
    }

    // Returns true if the movie matching id is globally paused.
    bool func_8045A7F8__16CLibCriMoviePlayFv(int id) {
        if (sInstance == nullptr) return false;

        for (int i = 0; i < 4; i++) {
            const MovieEntry& entry = sInstance->mEntries[i];
            if (entry.mPlyHandle != nullptr && entry.mPlayerId == (u32)id &&
                entry.mGlobalPause) {
                return true;
            }
        }

        return false;
    }

    // (wkUpdate__16 / wkStandbyLogout__16: the member definitions themselves
    // carry the retail mangled names - duplicate extern "C" wrappers here
    // would shadow the bodies with 0x10-byte tail calls.)
    extern "C" void wkUpdate__16CLibCriMoviePlayFv();
    extern "C" bool wkStandbyLogout__16CLibCriMoviePlayFv();

    void func_8045B1DC__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        // Empty
    }

    // Sets the pause override flag on the movie matching id.
    void func_8045B310__16CLibCriMoviePlayFv(bool pause, u32 id) {
        if (sInstance == nullptr) return;

        MovieEntry* entry = &sInstance->mEntries[0];
        u32 i;
        for (i = 0; i < 4; i++) {
            if (entry->mPlyHandle != nullptr) {
                if (entry->mPlayerId == id || (u32)id + 0x10000 == 0xFFFF) {
                    entry->mPauseOverride = pause;
                    int pa = 0;
                    if (!pause && !entry->mGlobalPause &&
                        sInstance->mPauseCounter == 0) {
                        // leave unpaused
                    } else {
                        pa = 1;
                    }
                    mwPlyPause(entry->mPlyHandle, pa);
                }
            }
            entry++;
        }
    }

    // Virtual thunks (adjust this pointer by -0x1C4)
    void func_8045B3D4__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        // Thunk for CDeviceVICb::viBeginFrame
        ((CLibCriMoviePlay*)((u8*)self - 0x1C4))->func_8045B1DC();
    }

    void func_8045B3DC__16CLibCriMoviePlayFv(CLibCriMoviePlay* self) {
        // Thunk for CDeviceVICb update
        func_8045AE84__16CLibCriMoviePlayFv((CLibCriMoviePlay*)((u8*)self - 0x1C4));
    }

}

// ===== BISECT D2: + blob def =====
extern "C" const char lbl_eu_80523008[0x11] = "CLibCriMoviePlay";

extern "C" const char lbl_eu_8052301C[0x34] = {
    0x92, 0xE2, 0x8E, 0x7E, 0x92, 0x86, 0x00, 0x8D,
    0xC4, 0x90, 0xB6, 0x8F, 0x80, 0x94, 0xF5, 0x92,
    0x86, 0x00, 0x8D, 0xC4, 0x90, 0xB6, 0x92, 0x86,
    0x00, 0x8D, 0xC4, 0x90, 0xB6, 0x8F, 0x49, 0x97,
    0xB9, 0x00, 0x83, 0x47, 0x83, 0x89, 0x81, 0x5B,
    0x94, 0xAD, 0x90, 0xB6, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

// CDeviceVICb deleting-dtor vtable slot (retail .text @0x8045F414): a bare
// tail branch to the complete destructor. The retail symbol spells
// "@452@__dt__16CLibCriMoviePlayFv", which C++ cannot declare; UNIT_RULES
// exact_renames renames this thunk to the retail name (CDeviceVI.cpp
// thunk_456 pattern).
extern "C" void __dt__16CLibCriMoviePlayFv();
asm void thunk_452_dt(void) {
    nofralloc
    b __dt__16CLibCriMoviePlayFv
}

// ===== Vtable + RTTI + locator (dissolved monolibdata2) =====
// Foreign function words (retail-named; C-linkage decls emit the mangled
// names verbatim). __RTTI__10IWorkEvent / __RTTI__11CWorkThread are NOT
// declared here: under -RTTI on MWCC auto-generates those typeinfo symbols
// and an extern "C" declaration collides (10322) - those two slots stay 0
// and get UNIT_RULES inject_relocs (CDevice.o pattern).
extern "C" int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern "C" int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern "C" int WorkEvent3__10IWorkEventFPv(void*);
extern "C" int WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__16CLibCriMoviePlayFb(int);
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
extern "C" int WorkEvent31__10IWorkEventFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" bool wkStandbyLogin__16CLibCriMoviePlayFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
extern "C" void viBeginFrame__11CDeviceVICbFv();
extern "C" u32 lbl_eu_80663618[];   // type_info vtable (foreign)

// [.data] 0x8056CF48-0x8056D008 (0xC0): __vt__16CLibCriMoviePlay (40-slot
// CWorkThread chain) followed by the CDeviceVICb sub-vtable (-0x1C4). Both
// bases are __declspec(novtable), so the vtable is spelled by hand
// (CLibG3d.cpp pattern); every word is an explicit &reloc so the object
// reproduces the retail reloc names exactly.
extern "C" u32 lbl_eu_8056CF48[48] = {
    (u32)&lbl_eu_80663798, 0x00000000,
    (u32)&__dt__16CLibCriMoviePlayFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__16CLibCriMoviePlayFb,
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
    (u32)&wkUpdate__16CLibCriMoviePlayFv,
    (u32)&wkRender__11CWorkThreadFv, (u32)&wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__16CLibCriMoviePlayFv,
    (u32)&wkStandbyLogout__16CLibCriMoviePlayFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
    // CDeviceVICb sub-vtable (this -0x1C4)
    (u32)&lbl_eu_80663798, 0xFFFFFE3C,
    (u32)&thunk_452_dt,
    (u32)&func_8045B3DC__16CLibCriMoviePlayFv,
    (u32)&func_8045B3D4__16CLibCriMoviePlayFv,
    (u32)&viBeginFrame__11CDeviceVICbFv,
    (u32)&func_8045AE84__16CLibCriMoviePlayFv,
    (u32)&func_8045B1DC__16CLibCriMoviePlayFv,
};

// [.data] 0x8056D008-0x8056D028 (0x20): typeinfo / base-list block
// ([type_info vtbl, 0x1C4], [__RTTI__10IWorkEvent, 0], [__RTTI__11CWorkThread, 0], [0, 0]).
// The two typeinfo words reference the foreign lbl_eu_80663618 placeholder
// (UNDEF here, so the file bytes stay 0 like retail); UNIT_RULES
// retarget_relocs repoints them at the __RTTI__ names - declaring
// "extern __RTTI__" directly collides with MWCC's implicit RTTI-on
// declaration (10322).
// Each placeholder must be a DISTINCT UNDEF symbol never referenced
// elsewhere in the TU: MWCC shares one symtab entry per target (deduping
// relocs), and retarget_relocs renames the whole entry - a shared name
// would corrupt every other reference to it (e.g. the dtor's __dl__FPv
// call). These two dummies exist only as reloc anchors.
extern "C" u32 decomp_rtti_anchor_10IWorkEvent[];
extern "C" u32 decomp_rtti_anchor_11CWorkThread[];
extern "C" u32 lbl_eu_8056D008[8] = {
    (u32)&lbl_eu_80663618, 0x000001C4,
    (u32)&decomp_rtti_anchor_10IWorkEvent, 0x00000000,
    (u32)&decomp_rtti_anchor_11CWorkThread, 0x00000000,
    0x00000000, 0x00000000,
};

// [.sdata] 0x80663798-0x806637A0 (0x8): RTTI locator pair
// { class-name string, typeinfo block } referenced by both vtable halves.
extern "C" u32 lbl_eu_80663798[2] = {
    (u32)(const char*)lbl_eu_80523008,
    (u32)(const u32*)lbl_eu_8056D008,
};
