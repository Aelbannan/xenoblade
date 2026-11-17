#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CMapSel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMapSelImports.hpp"
#include "kyoshin/CScrollBar.hpp"
// CMapSel.hpp declares the C-ABI layout helpers (PaneSetTexPaletteByName 2-arg etc.);
// skip code_80135FDC.hpp's caller-tuned exports so the differing arity decls
// don't clash (same mechanism code_80135FDC.cpp itself uses).
#define CODE_80135FDC_CPP
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include <nw4r/lyt.h>
void playUISound(u32);

/* __ct__CMapSel - Construct the map-selection widget: set the IWorkEvent
   vtable, construct the sub-objects (UnkClass_8045F564 mem region, scrollbar,
   cursor), then copy a flag-1 temporary CScrollBar's body over the embedded
   scrollbar (retail skips the +0x00 vtable). Afterwards build the map grid:
   collect the valid maps (ids 2..28, types 10/20/22/26/28 excluded), bubble-
   sort the grid by BDAT sort key, and place the selection on the current map. */
// Constructor: set the IWorkEvent vtable, construct sub-objects, then build
// the sorted map grid (see comment above the old free-function form).
CMapSel::CMapSel()
    : mFileHandle(nullptr), mFileHandle2(nullptr), mArcAccessor(nullptr),
      mAllocatedMem(nullptr), mLayout(nullptr), mAnimTransform1(nullptr),
      mAnimTransform2(nullptr), mActive(0), mState(0), mVisible(0),
      mInitFlag(1), mScrollBar(0), mCursor(nullptr) {
    float timer = lbl_eu_80668738;
    mSelY = 0;
    mSelX = 0;
    mCount = 0;
    mImagePending = 0;
    mTimer = timer;
    {
        CScrollBar bar(1);
        mScrollBar = bar;
    }

    // --- Build the map grid ---------------------------------------------
    void* mapTable = lbl_eu_806640A0;    // BDAT table for map ids
    void* orderTable = lbl_eu_806640A8;  // BDAT table for sort keys
    s32 mapCount = (s32)Bdat_GetMaxRow_B1EC(mapTable);
    u8 curMap = (u8)lbl_eu_80664184;     // current map id -> initial selection

    // For each map id 2..28 (types 10/20/22/26/28 excluded), append the map
    // to the grid when it is a valid map of that type. mapId stays an s32
    // counter (retail truncates per use); the switch operand is the u8 view,
    // which selects the retail jump-table dispatch.
    for (u8 mapId = 2; mapId <= 0x1c; mapId++) {
        char* base = lbl_eu_8050B4A8;        // pane-name / BDAT format string
        switch (mapId) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        case 8: case 9: case 11: case 12: case 13: case 14: case 15: case 16:
        case 17: case 18: case 19: case 21: case 23: case 24: case 25: case 27:
            for (s32 j = 1; j <= mapCount; j++) {
                if (BdatGetU8Direct((const void*)mapTable, base, (u32)j) == mapId) {
                    if (CtrlRemote_TouchBitByArg((u32)(j + 0x20c8)) != 0) {
                        u8 idx = mCount;
                        mGrid[idx] = mapId;
                        mCount = idx + 1;
                        break;
                    }
                }
            }
            break;
        case 10: case 20: case 22: case 26: case 28:
            ;  // empty: falls to the loop increment
        }
    }

    // Bubble-sort the grid by BDAT sort key (early exit when a pass makes
    // no swap). Count is re-read from memory each check, matching retail;
    // both keys land in u8 temps so lookups truncate via clrlwi.
    for (u8 pass = 0; pass < mCount; pass++) {
        char* keys = lbl_eu_8050B4A8;        // BDAT sort-key field name (+6)
        int swapped = 0;
        for (u8 k = 0; k < mCount - 1 - pass; k++) {
            // Retail hoists grid[k+1] into a callee-saved temp before the
            // first lookup, but passes grid[k] inline as its third argument.
            u8 b = mGrid[k + 1];
            u8 keyA = BdatGetU8Direct((const void*)orderTable, keys + 6,
                                    mGrid[k]);
            u8 keyB = BdatGetU8Direct((const void*)orderTable, keys + 6, b);
            if (keyA > keyB) {
                mGrid[k] ^= mGrid[k + 1];
                mGrid[k + 1] ^= mGrid[k];
                mGrid[k] ^= mGrid[k + 1];
                swapped = 1;
            }
        }
        if (!swapped) break;
    }

    // Place the selection on the current map: grid index -> (y, x) cell.
    for (u8 i = 0; i < mCount; i++) {
        if (mGrid[i] == curMap) {
            int v = i + 1;
            mSelY = (s8)v;
            // Retail re-uses the u8-truncated v (clrlwi) and sign-extends it
            // for the >= 10 test; the branches subtract from the truncated form.
            if ((s8)(u8)v >= 10) {
                mSelX = (s8)((u8)v - 10);
                mSelY = 9;
            } else {
                mSelY = (s8)((u8)v - 1);
            }
            break;
        }
    }
}

// Sub-object dtors run in reverse construction order; retail uses an
// stmw/lmw frame, which the scoped size optimization selects.
#pragma push
#pragma optimize_for_size on
CMapSel::~CMapSel() {}
#pragma pop

/* startLoad - Start the map-select UI: request the layout archive from
   MEM2 and mark the pending-map flag so loadSelectedMapResource begins the countdown,
   then read the scrollbar's layout arc. */
void CMapSel::startLoad() {
    mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), &lbl_eu_8050B4A8[0xc],
        this, 0, 0);
    mImagePending = 1;
    loadSelectedMapResource();
    mScrollBar.loadLayoutArc();
}

/* update - Per-frame update: drive the state machine, then animate the
   layout, scrollbar, and cursor. Guards on mActive (widget active) and
   mState (not uninitialized); the state dispatch mirrors retail (1: loading,
   2: visible, 3: transitioning out, 4: closing, 5: post-close cleanup). */
__declspec(noinline) void CMapSel::update() {
    if (mActive != 0 && mState != 0) {
        switch (mState) {
        case 1:
            tickOpen();
            break;
        case 2:
            tickOpenIn();
            break;
        case 3:
            loadSelectedMapResource();
            refreshImage();
            break;
        case 4:
            tickClose();
            break;
        case 5:
            tickCloseEnd();
            break;
        }
        mLayout->Animate(0);
        mScrollBar.UpdateDispatch();
        Cur_Update(&mCursor);
    }
}

/* draw - Draw the map-select UI: layout, scrollbar, and cursor.
   Guards on mActive (widget active) and mState (not uninitialized). */
void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

/* Retail uses an stmw/lmw r30 frame; the scoped size-optimization pragma is
   what selects that shape under -O4,p (also fixes the param copy order). */
#pragma push
#pragma optimize_for_size on
void CMapSel::draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (mActive != 0 && mState != 0) {
        drawLayout(mLayout, drawInfo, 0, 1);
        mScrollBar.draw(drawInfo);
        Cur_DrawLayout(&mCursor, drawInfo);
    }
}
#pragma pop

/* term - Tear down the map-select widget: release both file
   handles, deactivate, then (after vblank sync) delete the layout, free the
   map-data buffer, release the arc accessor / mem region / scrollbar, and
   run the cursor's release virtual. The doubled null-check before the
   layout virtual mirrors retail codegen. */
__declspec(noinline) void CMapSel::term() {
    closeFileHandle(&mFileHandle);
    closeFileHandle(&mFileHandle2);
    mActive = 0;
    CDeviceVI::waitForDrawDone();
    if (mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
    if (mAllocatedMem != nullptr) {
        mtl::MemManager::deallocate(mAllocatedMem);
        mAllocatedMem = nullptr;
    }
    releaseArcResourceAccessor(mArcAccessor);
    mMemRegion.deleteRegion();
    mScrollBar.Teardown();
    mCursor.cleanup();
}

// Scrollbar-visible check: when the scrollbar at +0x34 is visible, return
// the +0x32 flag (lbz), else 0.
int CMapSel::getVisibleFlag() {
    if (mScrollBar.isVisible() != 0) {
        return mVisible;
    }
    return 0;
}

// FULL_MATCH: reads the initial-setup flag at +0x33 (set to 1 in ctor, cleared on play/close)
// Out-of-line C-linkage copy so the retail getMapSelInitFlag symbol pairs
// (the header inline member keeps serving C++ callers).
int CMapSel::getInitFlag() {
    return mInitFlag;
}

// One-shot init: when the +0x31 byte is clear, set it and +0x33, then run
// the two init hooks.
void CMapSel::init() {
    if (mState == 0) {
        mState = 1;
        mInitFlag = 0;
        refreshGridText();
        updateCursor();
    }
}

// When the +0x31 state is 3, advance to 4, clear +0x33, run the scrollbar
// cleanup at +0x74 and play sound 6.
void CMapSel::beginClose() {
    if (mState == 3) {
        mState = 4;
        mInitFlag = 0;
        Cur_SetVisible(&mCursor, 0);
        playUISound(6);
    }
}

/* moveUp - Move the selection up one step. When the top of a column
   is passed, step to the previous column; past the first column, wrap to the
   last map entry (grid has 10 rows; mCount is the map count). */
__declspec(noinline) void CMapSel::moveUp() {
    // u8 locals + (s8) sign tests reproduce the retail clrlwi/extsb. shape.
    u8 y = mSelY - 1;
    mSelY = y;
    if ((s8)y < 0) {
        // Read through an unsigned cast so the load isn't sign-extended,
        // then hold the decrement as a raw int (retail keeps it untruncated
        // in place and materializes the (s8) test in a temp).
        mSelY = 0;
        int x = (u8)mSelX - 1;
        mSelX = x;
        if ((s8)(u8)x < 0) {
            u8 count = mCount;
            if (count >= 10) {
                mSelY = 9;
                mSelX = count - 10;
            } else {
                u8 cy = count - 1;
                mSelY = cy;
                mSelX = 0;
                if ((s8)cy < 0) mSelY = 0;
            }
        }
    }
    refreshGridText();
    updateCursor();
    s8 cur = mSelX;
    mScrollBar.PlaceThumb((u16)cur);
    requestImage();
    playUISound(1);
}

/* moveDown - Move the selection right one column. Wraps from the bottom
   row of a column to the top of the next column; past the last column, wraps
   to the first entry. mCount is the map count (grid has 10 rows). */
__declspec(noinline) void CMapSel::moveDown() {
    u8 count = mCount;
    if (count >= 10) {
        s8 y = (s8)((u8)(mSelY + 1));
        mSelY = y;
        if (y >= 10) {
            mSelY = 9;
            s8 x = (s8)((u8)(mSelX + 1));
            mSelX = x;
            if (x > (count - 10)) {
                mSelY = 0;
                mSelX = 0;
            }
        }
    } else {
        s8 y = (s8)((u8)(mSelY + 1));
        mSelY = y;
        if (y >= count) {
            mSelY = 0;
            mSelX = 0;
        }
    }
    refreshGridText();
    updateCursor();
    mScrollBar.PlaceThumb(mSelX);
    requestImage();
    playUISound(1);
}

/* movePageUp - Move the selection left/up by one column of 10 (mirror of
   moveMapSelPageDown). Wraps from the top row of a column back into the previous
   column near its bottom row; below zero clamps to the first entry.
   mCount is the map count (grid has 10 rows). */
__declspec(noinline) void CMapSel::movePageUp() {
    u8 count = mCount;
    if (count >= 10) {
        mSelX -= 10;
        if ((s8)mSelX < 0) {
            mSelY = mSelX + 9;
            mSelX = 0;
            if ((s8)mSelY < 0) mSelY = 0;
        }
    } else {
        mSelY = 0;
        mSelX = 0;
    }
    refreshGridText();
    updateCursor();
    mScrollBar.PlaceThumb(mSelX);
    requestImage();
    playUISound(1);
}

/* movePageDown - Move the selection down one row (grid has 10 columns;
   mCount is the map count). Wraps past the last row to the next
   column and clamps the row index to 9. */
__declspec(noinline) void CMapSel::movePageDown() {
    // Grid has 10 rows; mCount is the map count. Signed (s8)
    // comparisons against the count mirror the retail cmpw/cmpwi forms.
    u8 count = mCount;
    if (count >= 10) {
        u8 nx = mSelX + 10;
        mSelX = nx;
        if ((s8)nx > count - 10) {
            mSelY = nx - (count - 10);
            mSelX = count - 10;
            if ((s8)(u8)(nx - (count - 10)) >= 10) mSelY = 9;
        }
    } else {
        // Store the raw (unclipped) difference; recompute the u8-truncated
        // form only for the sign test.
        mSelY = count - 1;
        mSelX = 0;
        if ((s8)(u8)(count - 1) < 0) mSelY = 0;
    }
    refreshGridText();
    updateCursor();
    mScrollBar.PlaceThumb(mSelX);
    requestImage();
    playUISound(1);
}

u8 CMapSel::getMapGridCell() {
    s8 x = mSelX;
    s8 y = mSelY;
    return mGrid[x + y];
}

/* tickOpen - Finish the map-data load: when animation 1 completes,
   enable both animations, mark the widget visible (state 2), and configure
   the scrollbar for the current selection. mCount is the map count. */
__declspec(noinline) void CMapSel::tickOpen() {
    if (advanceAnimTransform(mAnimTransform1, lbl_eu_8066873C) != 0) {
        mLayout->SetAnimationEnable(mAnimTransform1, false);
        mLayout->SetAnimationEnable(mAnimTransform2, true);
        mState = 2;
        nw4r::math::VEC3 pos(lbl_eu_80668740, lbl_eu_80668744, lbl_eu_80668738);
        mScrollBar.InitRootPane(&pos);
        mScrollBar.UpdateThumb(10, mCount);
        s8 selX = mSelX;
        mScrollBar.PlaceThumb((u16)selX);
        mScrollBar.requestScrollIn();
    }
}

/* tickOpenIn - State 2 handler: once the map-open-in animation on
   mAnimTransform2 finishes, move to state 3 (transitioning out), flag +0x33,
   reset the cursor and refresh its grid position. */
__declspec(noinline) void CMapSel::tickOpenIn() {
    if (advanceAnimTransform(mAnimTransform2, lbl_eu_8066873C) != 0) {
        mState = 3;
        mInitFlag = 1;
        Cur_SetVisible(&mCursor, 1);
        updateCursor();
    }
}

/* tickClose - Poll animation completion on mAnimTransform2, then enable both
   animations, set state to 5 (post-close cleanup), and notify the scrollbar. */
__declspec(noinline) void CMapSel::tickClose() {
    if (AnimRewindFrame(mAnimTransform2, 1.0f) != 0) {
        mLayout->SetAnimationEnable(mAnimTransform2, false);
        mLayout->SetAnimationEnable(mAnimTransform1, true);
        mState = 5;
        mScrollBar.requestScrollOut();
    }
}

__declspec(noinline) void CMapSel::tickCloseEnd() {
    if (AnimRewindFrame(mAnimTransform1, lbl_eu_8066873C) != 0) {
        mInitFlag = 1;
        mState = 0;
    }
}

// When the +0x24 layout is present, run the grid rebuild (refreshMapGridText) and
// raise the +0x32/+0x30 flags.
__declspec(noinline) void CMapSel::activate() {
    if (mLayout != nullptr) {
        refreshGridText();
        mVisible = 1;
        mActive = 1;
    }
}

/* refreshGridText - Refresh the 10 grid cells' text. For each column index i
   (0..9), sprintf the pane name, then set the cell text from the BDAT map
   table; when the column matches the current selection X, also set the
   selected-row labels. Out-of-range columns are blanked with the placeholder
   string at +0x52. */
__declspec(noinline) void CMapSel::refreshGridText() {
    char buf[32];
    char* base = lbl_eu_8050B4A8;
    for (u32 i = 0; i < 10; i++) {
        sprintf(buf, base + 0x1f, (u8)i + 1);
        if ((s8)mSelX + (u8)i < mCount) {
            u8 g = mGrid[(s8)mSelX + (u8)i];
            char* res = BdatGetPtrDirect(lbl_eu_806640A8, base + 0x2b, g);
            LayoutSetTextBoxFmtValue(mLayout, buf, res, 0);
            if ((s8)mSelY == (u8)i) {
                LayoutSetTextBoxFmtValue(mLayout, base + 0x30, res, 0);
                char* res2 = BdatGetPtrDirect(lbl_eu_806640A8, base + 0x3a, g);
                LayoutSetTextBoxFmtValue(mLayout, base + 0x46, res2, 0);
            }
        } else {
            LayoutSetTextBoxFmtValue(mLayout, buf, base + 0x52, 0);
        }
    }
}

/* refreshImage - Show/hide the map-selection images. When no file is in
   flight, the map-data buffer is loaded and the pending flag is clear, set the
   current-map text pane's texture directly from the buffer and show it;
   otherwise re-fetch the "timg" resource from the arc and (when present) use
   it for the same pane. */
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CMapSel::refreshImage() {
    char* base;
    if (mFileHandle2 != 0 || mAllocatedMem == 0 ||
        mImagePending != 0) {
        base = lbl_eu_8050B4A8;
        const void* res = mArcAccessor->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x53, 0);
        if (res != 0) {
            PaneSetTexPaletteByName(mLayout, base + 0x66, (TPLPalette*)res);
        }
        return;
    }
    base = lbl_eu_8050B4A8;
    PaneSetTexPaletteByName(mLayout, base + 0x66, (TPLPalette*)mAllocatedMem);
    nw4r::lyt::Pane* pane =
        mLayout->GetRootPane()->FindPaneByName(base + 0x66, true);
    setPaneVisible(pane, 1);
}
#pragma pop

/* loadSelectedMapResource - Count down the map-select transition timer; when it
   expires, tear down the previous map-data load (release the handle and the
   allocated buffer) and start loading the newly selected map's file. */
__declspec(noinline) void CMapSel::loadSelectedMapResource() {
    CDeviceVI::waitForDrawDone();
    if (mTimer > lbl_eu_80668738) {
        mTimer -= lbl_eu_8066873C;
        return;
    }
    if (mImagePending == 0) return;
    mImagePending = 0;
    closeFileHandle(&mFileHandle2);
    if (mAllocatedMem != 0) {
        mtl::MemManager::deallocate(mAllocatedMem);
        mAllocatedMem = 0;
    }
    char buf[64];
    char* base = lbl_eu_8050B4A8;
    u8 idx = mGrid[(s8)mSelX + (s8)mSelY];
    u16 res = BdatGetU16Direct(lbl_eu_806640A8, base + 0x6e, idx);
    char* name = MakeTplNameSysFile(res);
    sprintf(buf, base + 0x79, name);
    mFileHandle2 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), buf,
        this, 0, 0);
}

__declspec(noinline) void CMapSel::requestImage() {
    mImagePending = 1;
    mTimer = lbl_eu_8066873C;
    setPaneVisible(mLayout->GetRootPane()->FindPaneByName(lbl_eu_8050B4A8 + 0x66, true), 0);
}

/* updateCursor - Refresh the cursor position on the map grid: format the
   current row's pane name from the selection Y, look up the two panes that
   bracket the selection, and move the cursor to the computed point.
   Retail uses an stmw/lmw r29 frame; the scoped size pragma selects it. */
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void CMapSel::updateCursor() {
    /* Strings live at buf+4 (retail sp+0x18) */
    char buf[0x2C];
    char* base = lbl_eu_8050B4A8;
    sprintf(buf + 4, base + 0x1f, mSelY + 1);
    nw4r::lyt::Pane* pane1 =
        mLayout->GetRootPane()->FindPaneByName(buf + 4, true);
    nw4r::lyt::Pane* pane2 = mLayout->GetRootPane()->FindPaneByName(
        base + 0x88, true);
    nw4r::math::VEC3 pos;
    ScalePaneWorldPos(&pos, pane1, pane2, mLayout->GetRootPane());
    mCursor.setRootPaneTranslate(&pos);
}
#pragma pop

/* CMapSel::OnFileEvent - Build the map-select layout when the main file
   finishes loading: create the scratch mem region + host, detach the file
   buffer into the arc accessor, build the layout and both animations, bind
   the font, and copy a freshly-constructed CCur18 cursor into +0x74. If the
   event instead matches the second handle, adopt its buffer as mAllocatedMem. */
#pragma push
#pragma optimize_for_size on
bool CMapSel::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
        mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
        char* base = lbl_eu_8050B4A8;
        mMemRegion.createRegion(mem2, 0x14000, base + 0x97, 0);
        Class_8045F858 memHost(&mMemRegion);
        void* fileData = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);
        mArcAccessor = CLibLayout::createArcResourceAccessor();
        mArcAccessor->Attach(fileData, base + 0x9f);
        buildLayout(&mLayout, mArcAccessor, base + 0xa3);
        bindLayoutAnimTransform(mLayout, &mAnimTransform1, mArcAccessor, base + 0xbb);
        bindLayoutAnimTransform(mLayout, &mAnimTransform2, mArcAccessor, base + 0xd6);

        // Bind the font: take the layout root pane, ask the font object for
        // its pane, and push it back onto the root.
        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        ApplyFontToTxtPanes(rootPane,
                            (const nw4r::ut::Font*)static_cast<IDeviceFontInfo*>(
                                CDeviceFont::getFontInfo(1, mLayout))->getFont());

        mLayout->SetAnimationEnable(mAnimTransform2, false);
        mLayout->SetAnimationEnable(mAnimTransform1, true);
        mLayout->Animate(0);

        // Build the embedded cursor, then poke its first virtual and run the
        // post-build hook.
        mCursor = CCur18(CUICfManager_getArcResourceAccessor());
        mCursor.initLayout();
        activate();
        mFileHandle = 0;
        mMemRegion.validateHeap();
        return true;
    }

    if (mFileHandle2 == event->mFileHandle) {
        u8* data = mFileHandle2->mData;
        mFileHandle2->mData = 0;
        mAllocatedMem = data;
        mFileHandle2 = 0;
        return true;
    }
    return false;
}
#pragma pop
