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
#include "monolib/math/CRect16.hpp"
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

extern u32 __vt__16CLibCriMoviePlay[]; // emitted by MWCC from the class

// External globals from sdata2
extern "C" {
    extern f32 identity__Q22ml6CMat34[12]; // ml::CMat34::identity matrix
}

// External function declarations
extern "C" {
    // GX cache (typed like CScnBloom / CView -- void* erased at codegen)
    void updateOrthoGX__8CGXCacheFv(CGXCache* cache);
    void setBlendState__8CGXCacheFii(CGXCache* cache, int a, int b);
    void resetGXStateA__8CGXCacheFv(CGXCache* cache);
    void updateViewRoot__9CViewRootFv();
    // `extern` is required: the retail object references this name as UNDEF
    // (4 x R_PPC_EMB_SDA21 in .text); without it this is a tentative
    // definition and collides with CDeviceGX.cpp's definition.
    extern CGXCache* cacheInstance__9CDeviceGX;

    // Memory manager
    void setMemInitFlag__Q23mtl10MemManagerFb(bool flag);
    bool isTvFormatPal__9CDeviceVIFv();
    bool hasFlow__12CWorkControlFv();

    // CRT

}


// Vptr layout view over the CLibCriMoviePlay MI object: primary
// (CWorkThread) vptr at 0x000 plus the CDeviceVICb sub-vptr at 0x1C4.
// Matches CLibCriVptrView shape used by CLibCri.cpp.
struct CLibCriMoviePlayVptrView {
    u32* vtPrimary;  //0x000 CWorkThread primary vptr
    u32* pad[112];   //0x004 .. 0x1C3
    u32* vtViCb;     //0x1C4 CDeviceVICb sub-vptr
};

// ============================================================================
// Constructor (us-8045dcb0)
// CLibCriMoviePlay::CLibCriMoviePlay(const char*, CWorkThread*)
// Both bases are __declspec(novtable), so the hand-spelled retail vtable
// (lbl_eu_8056CF48) is installed explicitly into both base slots.
// ============================================================================
// Retail-evidence shadows: __ct__CLibCriMoviePlay bls the two base ctors
// directly (retail relocs at +0x1c/+0x24), so the TU calls them by their
// retail mangled names (placement new would emit null-guard branches).
extern "C" void __ct__11CWorkThreadFPCcP11CWorkThreadi(CWorkThread* self, const char* name, CWorkThread* parent, int arg);
extern "C" void __ct__11CDeviceVICbFv(CDeviceVICb* self);
extern "C" CLibCriMoviePlay* __ct__CLibCriMoviePlay(CLibCriMoviePlay* mem, const char* pName, CWorkThread* pParent) {
    __ct__11CWorkThreadFPCcP11CWorkThreadi(mem, pName, pParent, 0);
    __ct__11CDeviceVICbFv((CDeviceVICb*)((char*)mem + 0x1C4));
    // Mark entries 1..3 inactive before the bulk clear below (retail keeps
    // these stores even though memset overwrites them).
    // Install the vtables into the CWorkThread slot (offset 0) and the
    // CDeviceVICb sub-vtable (base + 0xA0, at object offset 0x1C4).
    CLibCriMoviePlayVptrView* vp = (CLibCriMoviePlayVptrView*)mem;
    vp->vtPrimary = __vt__16CLibCriMoviePlay;
    vp->vtViCb = __vt__16CLibCriMoviePlay + 0x28;

    // Clear the active flag / filename length of all four entries (retail
    // keeps these stores even though memset below overwrites them).
    // Install the vtables into the CWorkThread slot (offset 0) and the
    // CDeviceVICb sub-vtable (base + 0xA0, at object offset 0x1C4).
    vp->vtPrimary = __vt__16CLibCriMoviePlay;
    vp->vtViCb = __vt__16CLibCriMoviePlay + 0x28;

    // Entry 0 explicitly, then walk entries 1..3 (retail keeps these stores
    // even though memset below overwrites them).
    mem->mEntries[0].mActive = false;
    mem->mEntries[0].mFilenameLen = 0;

    MovieEntry* entry = &mem->mEntries[1];
    while (entry < &mem->mEntries[4]) {
        entry->mActive = false;
        entry->mFilenameLen = 0;
        entry++;
    }

    mem->mStreamIdCounter = 0;
    mem->mPlaybackState = false;
    mem->mPauseCounter = 0;

    // Register singleton
    sInstance = mem;

    // Thread type: THREAD_CLIBCRIMOVIEPLAY
    mem->mType = (CWorkThread::ThreadType)0x10;

    // Clear all movie entries
    memset(&mem->mEntries[0], 0, 0x490); // 4 * sizeof(MovieEntry)

    // Initialize entry flags
    mem->mEntries[0].mFlags = 0;
    mem->mEntries[1].mFlags = 1;
    mem->mEntries[2].mFlags = 2;
    mem->mEntries[3].mFlags = 3;
    return mem;
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
// setupMovieGfx (us-8045de04)
// Sets up the GX TEV pipeline for movie rendering (YUV->RGB).
// Retail ABI: (r3 = stage-1 tex map, r4 = stage-0/2 tex map); the caller
// passes (GX_TEXMAP0, GX_TEXMAP1). The TEVREG1/KColor structs are filled
// with direct word copies from the sdata2 pool.
// ============================================================================
extern "C" void setupMovieGfx__16CLibCriMoviePlayFv(unsigned int texMapStage1, unsigned int texMapStage02) {
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

    // YUV -> RGB conversion constants.
    // NOTE: the TEVREG1 enum value in this header tree is off by one from
    // retail.
    GXColorS10 tevColor = { -111, 0, -138, 68 };
    GXSetTevColorS10((GXTevRegID)1, tevColor);

    GXColor kColor0 = { 0x66, 0x00, 0xFF, 0x32 };
    GXColor kColor1 = { 0x94, 0x00, 0x94, 0x94 };
    GXColor kColor2 = { 0xCB, 0x00, 0x05, 0xCF };
    GXColor kColor3 = { 0x00, 0xFF, 0x00, 0x00 };
    GXSetTevKColor((GXTevKColorID)0, kColor0);
    GXSetTevKColor((GXTevKColorID)1, kColor1);
    GXSetTevKColor((GXTevKColorID)2, kColor2);
    GXSetTevKColor((GXTevKColorID)3, kColor3);

    // Set swap mode tables
    GXSetTevSwapModeTable(GX_TEV_SWAP0, (GXTevColorChan)0, (GXTevColorChan)1, (GXTevColorChan)2, (GXTevColorChan)3);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, (GXTevColorChan)0, (GXTevColorChan)3, (GXTevColorChan)3, (GXTevColorChan)3);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, (GXTevColorChan)0, (GXTevColorChan)0, (GXTevColorChan)3, (GXTevColorChan)0);

    GXSetNumChans(0);
    GXSetNumIndStages(0);
}

// ============================================================================
// findFreeEntry (us-8045e1c8)
// CLibCriMoviePlay::findFreeMovieEntry()
// Returns pointer to first free movie entry, or NULL if none available
// ============================================================================
MovieEntry* CLibCriMoviePlay::findFreeMovieEntry() {
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
// CLibCriMoviePlay::startMovie()
// Starts movie playback with given parameters
// Returns stream ID on success, -1 on failure
// ============================================================================
extern "C" int startMovie__16CLibCriMoviePlayFv(const char* filename, u32 allocHandle,
                                  u32 allocHandle2, bool globalPause,
                                  bool waitFinish) {
    if (sInstance == nullptr) return -1;

    MovieEntry* entry = CLibCriMoviePlay::findFreeMovieEntry();
    if (entry == nullptr) return -1;

    // Fill the inline cprm block (retail zeroes the tail word first, then
    // writes cprm[0..5] in order). zero/one stay live as shared constants.
    u32 zero = 0;
    u32 one = 1;
    entry->field_0x28 = zero;
    entry->mCprmMode = one;
    entry->mCprmFormat = 0x5B8D80;
    entry->mCprmWidth = 0x280;
    entry->mCprmHeight = 0x1C8;
    entry->mCprmType = 2;
    entry->mCprmFlags = one;

    // Calculate the CRI work area size and allocate it (16-aligned)
    u32 workSize = mwPlyCalcWorkCprmSfd(&entry->mCprmMode);
    entry->mWorkSize = workSize;

    u8* workBuf = static_cast<u8*>(
        MemManager::allocate_tail(allocHandle, workSize, 0x20));
    entry->mWorkBuf = workBuf;

    if (workBuf == nullptr) {
        releaseMovieEntry__16CLibCriMoviePlayFv(entry, 0);
        return -1;
    }

    // Create the CRI player and register the movie name
    entry->mPlyHandle = criware_8039FF34(&entry->mCprmMode);

    entry->mFilenameLen = strlen(filename);
    strcpy(entry->mFilename, filename);

    entry->mAllocHandle = allocHandle;
    entry->mGlobalPause = globalPause;
    entry->mAllocHandle2 = allocHandle2;

    // Stream id: low byte of the entry flags plus the global counter shifted
    // up by one byte (24-bit insert).
    u32 counter = sInstance->mStreamIdCounter;
    entry->mPlayerId = entry->mFlags & 0xFF;
    entry->mPlayerId |= (counter & 0x00FFFFFF) << 8;
    sInstance->mStreamIdCounter = counter + 1;

    // Reset per-entry playback state
    entry->mActive = true;
    entry->mTexBufY = nullptr;
    entry->mTexBufCbCr = nullptr;
    entry->mColor[0] = 0.0f;
    entry->mColor[1] = 0.0f;
    entry->mColor[2] = 0.0f;
    entry->mColor[3] = 0.0f;
    entry->mAction = 3;
    entry->mField100 = 0;
    entry->mPauseOverride = false;

    // Kick off playback
    mwPlyStartFname(entry->mPlyHandle, filename);

    // Propagate the new entry's pause state to every active entry sharing
    // its stream id (id -1 matches all).
    if (sInstance != nullptr) {
        MovieEntry* other = &sInstance->mEntries[0];
        for (int i = 0; i < 4; i++) {
            if (other->mPlyHandle != nullptr &&
                (other->mPlayerId == entry->mPlayerId ||
                 entry->mPlayerId == 0xFFFFFFFF)) {
                other->mPauseOverride = entry->mPauseOverride;
                int pa = 0;
                if (!entry->mPauseOverride && !other->mGlobalPause &&
                    sInstance->mPauseCounter == 0) {
                    // not paused
                } else {
                    pa = 1;
                }
                mwPlyPause(other->mPlyHandle, pa);
            }
            other++;
        }
    }

    // Optionally block until playback leaves the "starting" state
    if (waitFinish) {
        while (mwPlyGetStat(entry->mPlyHandle) == 1) {
            ADXM_ExecMain();
            VIWaitForRetrace();
        }
    }

    sInstance->mPlaybackState = !waitFinish;
    return entry->mPlayerId;
}

// ============================================================================
// releaseEntry (us-8045e564)
// CLibCriMoviePlay::releaseMovieEntry()
// Cleans up and releases movie entries. Body lives in the retail-named
// wrapper releaseMovieEntry__16CLibCriMoviePlayFv (see below) so MWCC keeps it
// outlined (large body) and other functions call it directly.
// ============================================================================
// ============================================================================
// isMoviePlaying / unsetPauseFlag / hasActiveMovie / setPauseState
// Implemented directly as the retail-named wrappers at the bottom of this
// file (their retail symbols take the arguments in registers that do not
// match a member-function ABI).
// ============================================================================
// renderMovie (us-8045e8e0)
// CLibCriMoviePlay::renderMovie()
// Renders a movie frame to the screen using GX
// Returns true on success, false if movie not found
// ============================================================================
// Renders the movie frame matching id as a textured quad spanning rect.
// Retail ABI passes id in r3 and the destination rectangle in r4 despite the
// Fv mangling.
extern "C" bool renderMovie__16CLibCriMoviePlayFv(int id, const ml::CRect16& rect) {
    if (sInstance == nullptr) return false;

    MovieEntry* entry = sInstance->mEntries;

    for (int i = 0; i < 4; i++, entry++) {
        if (entry->mPlyHandle == nullptr) continue;
        if (entry->mPlayerId != (u32)id) continue;
        if (entry->mTexBufCbCr == nullptr) continue;

        // Flush GX cache and bind both movie textures
        updateOrthoGX__8CGXCacheFv(cacheInstance__9CDeviceGX);

        GXLoadTexObj(&entry->mTexObjY, GX_TEXMAP0);
        GXLoadTexObj(&entry->mTexObjCbCr, GX_TEXMAP1);

        // YUV->RGB TEV setup (stage-1 map = Y, stage-0/2 map = CbCr)
        setupMovieGfx__16CLibCriMoviePlayFv(GX_TEXMAP0, GX_TEXMAP1);

        setBlendState__8CGXCacheFii(cacheInstance__9CDeviceGX, 0, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);

        // Identity matrices for texture and position transforms
        GXLoadTexMtxImm((const f32(*)[4])identity__Q22ml6CMat34, GX_TEXMTX0, (GXMtxType)1);

        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXLoadPosMtxImm((const f32(*)[4])identity__Q22ml6CMat34, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, (GXCompCnt)1, (GXCompType)3, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, (GXCompCnt)1, (GXCompType)4, 0);

        // Retail GX_QUADS is 0x98 (vendored enum differs)
        GXBegin((GXPrimitive)0x98, GX_VTXFMT0, 4);

        GXPosition3s16(rect.mPos.x, rect.mPos.y, 0);
        GXTexCoord1f32(0.0f);
        GXTexCoord1f32(0.0f);

        GXPosition3s16(rect.mPos.x + rect.mSize.x, rect.mPos.y, 0);
        GXTexCoord1f32(1.0f);
        GXTexCoord1f32(0.0f);

        GXPosition3s16(rect.mPos.x, rect.mPos.y + rect.mSize.y, 0);
        GXTexCoord1f32(0.0f);
        GXTexCoord1f32(1.0f);

        GXPosition3s16(rect.mPos.x + rect.mSize.x, rect.mPos.y + rect.mSize.y, 0);
        GXTexCoord1f32(1.0f);
        GXTexCoord1f32(1.0f);

        // Flush the quad, then draw the tint overlay on top
        resetGXStateA__8CGXCacheFv(cacheInstance__9CDeviceGX);
        updateViewRoot__9CViewRootFv();

        CDrawGX drawGx;
        drawGx.setZCompare(0);
        drawGx.setZWriteEnable(0);
        drawGx.setCol((const CCol4&)entry->mColor[0]);
        drawGx.renderRect(rect);

        resetGXStateA__8CGXCacheFv(cacheInstance__9CDeviceGX);
        updateViewRoot__9CViewRootFv();
        // (destructor runs automatically at scope exit)

        return true;
    }

    return false;
}

// ============================================================================
// updateMovies (us-8045eeb0)
// CLibCriMoviePlay::viBeforeDrawDone()
// Updates texture buffers for all active movie entries
// ============================================================================
void CLibCriMoviePlay::viBeforeDrawDone() {
    MovieEntry* entry = mEntries;

    for (u32 i = 0; i < 4; i++) {
        if (entry->mPlyHandle != nullptr) {
            if (entry->mActive) {
                // Current frame info block from CRI ([0] = frame tag)
                u32 frameData[0x28];
                mwPlyGetCurFrm(entry->mPlyHandle, frameData);

                if (frameData[0] != 0) {
                    // Texture setup needed while either plane buffer is missing
                    if (entry->mTexBufCbCr == nullptr || entry->mTexBufY == nullptr) {
                        // Derive texture dimensions: half-resolution planes,
                        // each dimension rounded up to a multiple of 32.
                        s32 wRaw = frameData[3];
                        s32 wRem = (wRaw / 2) & 0x1F;
                        s32 wHalf = wRaw / 2;
                        if (wRem != 0) {
                            wHalf = wHalf + 0x20 - wRem;
                        }
                        u32 tRem = (wHalf * 2) & 0x1F;
                        u32 texW = (wHalf * 2) & 0x1FFFF;
                        u32 cw = wHalf & 0xFFFF;
                        u32 ch = ((s32)frameData[4] / 2) & 0xFFFF;
                        if (tRem != 0) {
                            texW = texW + 0x20 - tRem;
                        }

                        entry->mTexWidth = texW;
                        entry->mTexHeight = frameData[4];

                        if (entry->mTexBufY == nullptr) {
                            entry->mTexBufYSize = GXGetTexBufferSize(
                                entry->mTexWidth, entry->mTexHeight,
                                (u32)1, GX_FALSE, (u8)0);

                            if (entry->mAction == 0) {
                                entry->mTexBufY = static_cast<u8*>(MemManager::allocate_tail(                                    (u32)entry->mAllocHandle, entry->mTexBufYSize, 0x20));
                            } else if (entry->mAction == 3) {
                                MemManager::setOptimalAlloc(true);
                                entry->mTexBufY = static_cast<u8*>(MemManager::allocate_head(                                    entry->mAllocHandle2, entry->mTexBufYSize, 0x20));
                                if (entry->mTexBufY == nullptr) {
                                    entry->mTexBufY = static_cast<u8*>(MemManager::allocate_head(                                        MemManager::getHandleMEM2(), entry->mTexBufYSize, 0x20));
                                }
                                MemManager::setOptimalAlloc(false);
                            } else {
                                // Fixed address mode: park the Y plane at the
                                // pre-set address, aligned up to 32 bytes.
                                u32 addr = entry->mField100;
                                if (addr != 0) {
                                    u32 rem = addr & 0x1F;
                                    if (rem != 0) {
                                        addr = addr + 0x20 - rem;
                                    }
                                    entry->mTexBufY = reinterpret_cast<u8*>(addr);
                                }
                            }

                            // Initialize Y texture object
                            GXInitTexObj(&entry->mTexObjY, entry->mTexBufY,
                                         entry->mTexWidth, entry->mTexHeight,
                                         (GXTexFmt)1, (GXTexWrapMode)0, (GXTexWrapMode)0, GX_FALSE);
                            GXInitTexObjLOD(&entry->mTexObjY, (GXTexFilter)0, (GXTexFilter)0,
                                            0.0f, 0.0f, 0.0f, (GXBool)0, (GXBool)0, (GXAnisotropy)0);
                        }

                        if (entry->mTexBufCbCr == nullptr) {
                            entry->mTexBufCbCrSize = GXGetTexBufferSize(
                                cw, ch, (u32)3, GX_FALSE, (u8)0);

                            if (entry->mAction == 0) {
                                entry->mTexBufCbCr = static_cast<u8*>(MemManager::allocate_tail(                                    (u32)entry->mAllocHandle, entry->mTexBufCbCrSize, 0x20));
                            } else if (entry->mAction == 3) {
                                MemManager::setOptimalAlloc(true);
                                entry->mTexBufCbCr = static_cast<u8*>(MemManager::allocate_head(                                    entry->mAllocHandle2, entry->mTexBufCbCrSize, 0x20));
                                if (entry->mTexBufCbCr == nullptr) {
                                    entry->mTexBufCbCr = static_cast<u8*>(MemManager::allocate_head(                                        MemManager::getHandleMEM2(), entry->mTexBufCbCrSize, 0x20));
                                }
                                MemManager::setOptimalAlloc(false);
                            } else {
                                // CbCr plane follows the Y plane, aligned up to 32.
                                if (entry->mTexBufY != nullptr) {
                                    u32 addr = (u32)entry->mTexBufY + entry->mTexBufYSize;
                                    u32 rem = addr & 0x1F;
                                    if (rem != 0) {
                                        addr = addr + 0x20 - rem;
                                    }
                                    entry->mTexBufCbCr = reinterpret_cast<u8*>(addr);
                                }
                            }

                            // Initialize CbCr texture object
                            GXInitTexObj(&entry->mTexObjCbCr, entry->mTexBufCbCr,
                                         cw, ch, (GXTexFmt)3,
                                         (GXTexWrapMode)0, (GXTexWrapMode)0, GX_FALSE);
                            GXInitTexObjLOD(&entry->mTexObjCbCr, (GXTexFilter)0, (GXTexFilter)0,
                                            0.0f, 0.0f, 0.0f, (GXBool)0, (GXBool)0, (GXAnisotropy)0);
                        }
                    }

                    // Convert and upload the frame into both planes
                    if (entry->mTexBufCbCr != nullptr) {
                        if (entry->mTexBufY != nullptr) {
                            mwPlyFxSetOutBufPitchHeight(entry->mPlyHandle,
                                                        entry->mTexWidth, entry->mTexHeight);
                            mwPlyFxCnvFrmY84C44(entry->mPlyHandle, frameData,
                                                entry->mTexBufY, entry->mTexBufCbCr);
                            DCFlushRangeNoSync(entry->mTexBufY, entry->mTexBufYSize);
                            DCFlushRangeNoSync(entry->mTexBufCbCr, entry->mTexBufCbCrSize);
                            mwPlyRelCurFrm(entry->mPlyHandle);
                        }
                    }
                }
            }
        }

        entry++;
    }
}

// ============================================================================
// onMovieViBegin (us-8045f208)
// Empty function - just returns
// ============================================================================
// (declared inline in header)

// ============================================================================
// getWorkSize (us-8045f20c)
// CLibCriMoviePlay::getMovieWorkSize()
// Calculates required work buffer size for movie playback
// ============================================================================
int CLibCriMoviePlay::getMovieWorkSize() {
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
    const char* str;
    const char* table = "\x92\xe2\x8e\x7e\x92\x86\x00\x8d\xc4\x90\xb6\x8f\x80\x94\xf5\x92\x86\x00\x8d\xc4\x90\xb6\x92\x86\x00\x8d\xc4\x90\xb6\x8f\x49\x97\xb9\x00\x83\x47\x83\x89\x81\x5b\x94\xad\x90\xb6\x00\x00\x00\x00\x00\x00\x00";
    bool hasActive = false;
    MovieEntry* entry = &mEntries[0];
    u32 i;
    volatile u32 statusLen;
    char statusBuf[0x40];

    for (i = 0; i < 4; i++) {
        if (entry->mPlyHandle == nullptr) {
            // Entry not in use - free its saved texture buffers under the
            // memory-manager lock.
            setMemInitFlag__Q23mtl10MemManagerFb(false);
            if (entry->mSavedTexBufY != nullptr) {
                MemManager::deallocate(entry->mSavedTexBufY);
                entry->mSavedTexBufY = nullptr;
            }
            if (entry->mSavedTexBufCbCr != nullptr) {
                MemManager::deallocate(entry->mSavedTexBufCbCr);
                entry->mSavedTexBufCbCr = nullptr;
            }
            setMemInitFlag__Q23mtl10MemManagerFb(true);
            continue;
        }

        // Get playback status
        int stat = mwPlyGetStat(entry->mPlyHandle);
        entry->mPlaybackState = stat;

        if (stat == 3 || stat == 4 || stat == 0) {
            // Playback ended or error - release entry
            releaseMovieEntry__16CLibCriMoviePlayFv(entry, 1);
            continue;
        }

        // Active playback - poll output volume
        int volume = mwPlyGetOutVol(entry->mPlyHandle);

        // Build status string for debug
        statusLen = 0;
        statusBuf[0] = '\0';

        switch (entry->mPlaybackState) {
        case 0:
            str = table;
            statusLen = strlen(str);
            strcpy(statusBuf, str);
            break;
        case 1:
            str = table + 7;
            statusLen = strlen(str);
            strcpy(statusBuf, str);
            hasActive = true;
            break;
        case 2:
            str = table + 0x12;
            statusLen = strlen(str);
            strcpy(statusBuf, str);
            break;
        case 3:
            str = table + 0x19;
            statusLen = strlen(str);
            strcpy(statusBuf, str);
            break;
        case 4:
            str = table + 0x22;
            statusLen = strlen(str);
            strcpy(statusBuf, str);
            break;
        }

        // Mute while a flow event is active (-960 selected via mask)
        bool flowActive = hasFlow__12CWorkControlFv();
        mwPlySetOutVol(entry->mPlyHandle, flowActive ? -960 : 0);
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
                      ? 50.0f : 59.94f;
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
    // Refuse to log out unless this thread has no children left
    if (mChildren.begin() == mChildren.end()) {
        // A movie is still active if any entry of the singleton holds a
        // player handle (no singleton means nothing is active).
        bool anyActive;
        if (sInstance == nullptr) {
            anyActive = false;
        } else {
            MovieEntry* entry = sInstance->mEntries;
            if (entry->mPlyHandle != nullptr) {
                anyActive = true;
            } else if (entry[1].mPlyHandle != nullptr) {
                anyActive = true;
            } else if (entry[2].mPlyHandle != nullptr) {
                anyActive = true;
            } else {
                anyActive = false;
            }
        }

        if (!anyActive) {
            // Release saved texture buffers of all four local entries under
            // the memory-manager lock.
            MovieEntry* cur = &mEntries[0];
        for (int i = 0; i < 4; i++) {
            setMemInitFlag__Q23mtl10MemManagerFb(false);
            if (cur->mSavedTexBufY != nullptr) {
                MemManager::deallocate(cur->mSavedTexBufY);
                cur->mSavedTexBufY = nullptr;
            }
            if (cur->mSavedTexBufCbCr != nullptr) {
                MemManager::deallocate(cur->mSavedTexBufCbCr);
                cur->mSavedTexBufCbCr = nullptr;
            }
            setMemInitFlag__Q23mtl10MemManagerFb(true);
            cur++;
        }

            return CWorkThread::wkStandbyLogout();
        }
    }

    return false;
}


// ============================================================================
// External linkage wrappers (called from CLibCri forwarding stubs)
// ============================================================================
extern "C" {
    // Forward declarations (definitions below, in retail order)
    void releaseMovieEntry__16CLibCriMoviePlayFv(MovieEntry* entry, int flags);


// Finds the movie entry whose player ID matches id (id == -1 matches none).
// The singleton pointer is tested twice (duplicated guard) so MWCC emits the
// retail cmp + beq/bne pair with the null-assignment block in between.

// Finds the movie entry whose player ID matches id (id == -1 matches none).
// The search peeks entry[1] then advances, matching the retail load schedule
// (constant 0x220 offset per stage with the stride addi hoisted above cmpl).

    void stopMovie__16CLibCriMoviePlayFv(int id) {
        MovieEntry* entry;

        if (sInstance == nullptr) {
            return;
        }

        if ((u32)id + 0x10000 == 0xFFFF) {
            entry = nullptr;
        } else {
            entry = &sInstance->mEntries[0];
            if (entry->mPlayerId == (u32)id) goto stop;
            if (entry[1].mPlayerId == (u32)id) {
                entry++;
                goto stop;
            }
            entry++;
            if (entry[1].mPlayerId == (u32)id) {
                entry++;
                goto stop;
            }
            entry++;
            if (entry[1].mPlayerId == (u32)id) {
                entry++;
                goto stop;
            }
            entry = nullptr;
        }
stop:
        if (entry != nullptr) {
            mwPlyStop(entry->mPlyHandle);
            releaseMovieEntry__16CLibCriMoviePlayFv(entry, 0);
        }
    }

    void releaseMovieEntry__16CLibCriMoviePlayFv(MovieEntry* entry, int flags) {
        if (sInstance == nullptr) return;

        // Lock memory manager
        setMemInitFlag__Q23mtl10MemManagerFb(false);

        MovieEntry* cur = &sInstance->mEntries[0];
        for (u32 i = 0; i < 4; i++) {
            if (entry == nullptr || entry == cur) {
                // Destroy CRI player handle
                if (cur->mPlyHandle != nullptr) {
                    criware_803A09B4(cur->mPlyHandle);
                    cur->mPlyHandle = nullptr;
                }

                // Handle different action states
                if (cur->mAction == 0 || cur->mAction == 3) {
                    // Free work buffer and save texture state
                    if (cur->mWorkBuf != nullptr) {
                        MemManager::deallocate(cur->mWorkBuf);
                        cur->mWorkBuf = nullptr;
                    }
                    cur->mSavedTexBufY = cur->mTexBufY;
                    cur->mSavedTexBufCbCr = cur->mTexBufCbCr;
                } else if (cur->mAction == 2) {
                    // Free work buffer (alternate-buffer mode)
                    if (cur->mWorkBuf != nullptr) {
                        MemManager::deallocate(cur->mWorkBuf);
                        cur->mWorkBuf = nullptr;
                    }
                }
            }
            cur++;
        }

        // Unlock memory manager
        setMemInitFlag__Q23mtl10MemManagerFb(true);
    }

    // Returns true if any active movie matches id (id == -1 matches any).
    bool isMoviePlaying__16CLibCriMoviePlayFv(u32 id) {
        if (sInstance == nullptr) return false;

        MovieEntry* cur = &sInstance->mEntries[0];
        for (int i = 0; i < 4; i++) {
            if (cur->mPlyHandle != nullptr &&
                (cur->mPlayerId == id || id + 0x10000 == 0xFFFF)) {
                return true;
            }
            cur++;
        }

        return false;
    }

    // Clears the global-pause flag on the movie matching id, then propagates
    // the resulting pause state to every active entry (the matched entry's
    // pause-override flag is fanned out to all entries before evaluating the
    // per-entry pause conditions).
    void clearMovieGlobalPause__16CLibCriMoviePlayFv(int id) {
        MovieEntry* entry;

        if (sInstance == nullptr) {
            return;
        }

        entry = &sInstance->mEntries[0];
        for (int i = 0; i < 4; i++, entry++) {
            if (entry->mPlyHandle != nullptr && entry->mPlayerId == (u32)id &&
                entry->mGlobalPause) {
                entry->mGlobalPause = false;
                bool overrideState = entry->mPauseOverride;

                if (sInstance == nullptr) {
                    return;
                }

                u32 j;
                MovieEntry* other = &sInstance->mEntries[0];
                for (j = 0; j < 4; j++) {
                    if (other->mPlyHandle == nullptr) {
                        goto nextOther;
                    }
                    other->mPauseOverride = overrideState;
                    // Reloaded ahead of the pause tests so the call argument
                    // sits in r3 before the flag branches.
                    MwPly* ply = other->mPlyHandle;
                    int pa = 0;
                    if (!overrideState && !other->mGlobalPause &&
                        sInstance->mPauseCounter == 0) {
                        // no pause requested
                    } else {
                        pa = 1;
                    }
                    mwPlyPause(ply, pa);
                nextOther:
                    other++;
                }
                return;
            }
        }
    }

    // Returns true if the movie matching id is globally paused.
    bool isMovieGlobalPaused__16CLibCriMoviePlayFv(int id) {
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

    // Sets the pause override flag on the movie matching id (id == -1
    // matches none). The player handle is loaded before evaluating the pause
    // conditions so the call argument is ready ahead of the branches.
    // Sets the pause override flag on the movie matching id (id == -1
    // matches none).
    void setMoviePause__16CLibCriMoviePlayFv(bool pause, u32 id) {
        if (sInstance == nullptr) return;

        u32 i;
        MovieEntry* entry = &sInstance->mEntries[0];
        for (i = 0; i < 4; i++) {
            if (entry->mPlyHandle != nullptr &&
                (entry->mPlayerId == id || (u32)id + 0x10000 == 0xFFFF)) {
                entry->mPauseOverride = pause;
                int pa = 0;
                if (!pause && !entry->mGlobalPause &&
                    sInstance->mPauseCounter == 0) {
                    // nothing to pause
                } else {
                    pa = 1;
                }
                mwPlyPause(entry->mPlyHandle, pa);
            }
            entry++;
        }
    }

}

void CLibCriMoviePlay::viAfterDrawDone() {
}



