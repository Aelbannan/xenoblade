// CSortMenu - pop-up sort-order list widget

// Inline-empty ~IWorkEvent: retail ~CSortMenu elides the base call (same as
// CScrollBar / CBattery).
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CSortMenu.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"

#include <cstdio>
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Pane-position helper from code_80135FDC (retail-unmangled).

void closeFileHandle(CFileHandle**);
extern "C" u16 CItemBlock_getFlag120EC(int);

// Text panes of the five visible rows.
static const char* sRowPaneNames[] = {
    "txt_menu01_01", "txt_menu01_02", "txt_menu01_03", "txt_menu01_04", "txt_menu01_05",
};

CSortMenu::CSortMenu()
    : mMemRegion(), mFileHandle(NULL), mArcResAcc(NULL), mpLayout(NULL), mpAnimTrans0(NULL),
      mpAnimTrans1(NULL), mOpen(0), mLoaded(0), mState(0), mIdle(1), mScrollBar(0), mCount(0),
      mCursor(0), mTop(0) {}

CSortMenu::~CSortMenu() {}

void CSortMenu::Init() {
    mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(), "menu/jp/SortMenu.arc",
                                        this, 0, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);
    mScrollBar = CScrollBar(2);
    mScrollBar.loadLayoutArc();
}

void CSortMenu::Move() {
    if (mOpen == 0) return;

    switch (mState) {
    case 1:
        StateOpen();
        break;
    case 2:
        StateOpenInfo();
        break;
    case 4:
        StateCloseInfo();
        break;
    case 5:
        StateClose();
        break;
    }

    mpLayout->Animate(0);
    mScrollBar.UpdateDispatch();
}

void CSortMenu::Draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (mOpen == 0) return;
    drawLayout(mpLayout, drawInfo, 0, 1);
    if (mCount > 5) {
        mScrollBar.draw(drawInfo);
    }
}

void CSortMenu::Term() {
    closeFileHandle(&mFileHandle);
    mOpen = 0;
    if (mpLayout != NULL) {
        delete mpLayout;
        mpLayout = NULL;
    }
    releaseArcResourceAccessor(mArcResAcc);
    mArcResAcc = NULL;
    mMemRegion.deleteRegion();
    mScrollBar.Teardown();
}

int CSortMenu::IsReady() {
    if (mScrollBar.isVisible() != 0) {
        return mLoaded;
    }
    return 0;
}

int CSortMenu::IsOpen() {
    return mOpen;
}

int CSortMenu::IsIdle() {
    return mIdle;
}

// Each GetRootPane() is spelled out fresh - retail reloads mpLayout before
// every use instead of caching the root pane.
void CSortMenu::Open() {
    if (mState != 0) return;

    mState = 1;
    mIdle = 0;
    mOpen = 1;

    nw4r::lyt::Pane* paneScr = mpLayout->GetRootPane()->FindPaneByName("nul_scr", true);
    nw4r::lyt::Pane* paneProp = mpLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
    nw4r::math::VEC3 pos;
    ScalePaneWorldPos(&pos, paneScr, paneProp, mpLayout->GetRootPane());

    mScrollBar.InitRootPane(&pos);
    mScrollBar.UpdateThumb(5, mCount);
    mScrollBar.requestScrollIn();

    Refresh();
}

void CSortMenu::Close() {
    if (mState != 3) return;
    mState = 4;
    mIdle = 0;
    mScrollBar.requestScrollOut();
}

void CSortMenu::SetPos(const nw4r::math::VEC3* pos) {
    nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
    rootPane->SetTranslate(*pos);
}

// World position of the cursor row (returned by value: the result pointer
// arrives in r3, this in r4).
nw4r::math::VEC3 CSortMenu::GetCursorPos() {
    nw4r::math::VEC3 pos;
    char name[0x1c];
    sprintf(name, "%s", sRowPaneNames[(s8)mCursor]);

    nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
    nw4r::lyt::Pane* paneProp = rootPane->FindPaneByName("nul_proportion", true);
    nw4r::lyt::Pane* paneRow = mpLayout->GetRootPane()->FindPaneByName(name, true);

    ScalePaneWorldPos(&pos, paneRow, paneProp, rootPane);
    return pos;
}

void CSortMenu::Clear() {
    mCount = 0;
}

void CSortMenu::Add(const char* entry) {
    if (mCount < 32) {
        mEntries[mCount++] = entry;
    }
}

void CSortMenu::SetSelect(u8 select) {
    nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName("win_sortmenu01", true);

    // Local copy of the pane size region; member-wise so MWCC emits an
    // lfs/stfs pair (a whole-struct copy collapses to lwz).
    PaneSizeRegion size;
    CSortMenuPaneSizeView* paneView = (CSortMenuPaneSizeView*)pane;

    size.width = paneView->width;
    size.height = paneView->height;

    // Visible rows: 5 unless fewer entries exist.
    s32 rows = (mCount < 5) ? (s32)mCount : 5;
    size.height = 21.6f * (f32)rows;
    paneView->width = size.width;
    paneView->height = size.height;

    mCursor = select;
    mTop = 0;

    if ((s8)select >= 5) {
        mTop = select - 4;
        mCursor = 4;
    }

    Refresh();
}

void CSortMenu::CursorUp() {
    mCursor = (u8)(mCursor - 1);
    if ((s8)mCursor < 0) {
        mCursor = 0;
        mTop = (u8)(mTop - 1);

        if ((s8)mTop < 0) {
            u8 count = mCount;
            if (count >= 5) {
                mCursor = 4;
                mTop = count - 5;
            } else {
                mCursor = (u8)(count - 1);
                mTop = 0;
                if ((s8)mCursor < 0) {
                    mCursor = 0;
                }
            }
        }
    }
    Refresh();
}

void CSortMenu::CursorDown() {
    u8 count = mCount;
    if (count >= 5) {
        u8 cursorU = (u8)(mCursor + 1);
        mCursor = cursorU;
        s8 cursor = (s8)cursorU;
        if (cursor >= 5) {
            mCursor = 4;
            s32 top = (s32)mTop + 1;
            mTop = (u8)top;
            u8 topU = (u8)top;
            s8 topS = (s8)topU;
            if (topS > (s32)(count - 5)) {
                mCursor = 0;
                mTop = 0;
            }
        }
    } else {
        u8 cursorU = (u8)(mCursor + 1);
        mCursor = cursorU;
        s8 cursor = (s8)cursorU;
        if (cursor >= count) {
            mCursor = 0;
            mTop = 0;
        }
    }
    Refresh();
}

void CSortMenu::PageUp() {
    u8 count = mCount;
    if (count >= 5) {
        u8 topU = (u8)(mTop - 5);
        mTop = topU;
        if ((s8)topU < 0) {
            mCursor = (u8)(topU + 4);
            mTop = 0;
            if ((s8)(u8)(topU + 4) < 0) {
                mCursor = 0;
            }
        }
    } else {
        mCursor = 0;
        mTop = 0;
    }
    Refresh();
}

void CSortMenu::PageDown() {
    u8 count = mCount;
    if (count >= 5) {
        s32 maxTop = (s32)(count - 5);
        s32 top = (s32)mTop + 5;
        mTop = (u8)top;
        u8 topU = (u8)top;
        if ((s8)topU > maxTop) {
            // New cursor comes from the truncated byte, not the sign-extended sum.
            s32 cursor = (s32)topU - maxTop;
            mCursor = (u8)cursor;
            mTop = (u8)maxTop;
            if ((s8)mCursor >= 5) {
                mCursor = 4;
            }
        }
    } else {
        mCursor = (u8)(count - 1);
        mTop = 0;
        if ((s8)mCursor < 0) {
            mCursor = 0;
        }
    }
    Refresh();
}

u8 CSortMenu::GetSelect() {
    return mCursor + mTop;
}

int CSortMenu::GetCursor() {
    return mCursor;
}

int CSortMenu::GetTop() {
    return mTop;
}

// Split a category's stored sort index into cursor row + first visible entry.
void CSortMenu::CalcSelect(int category, u8* outCursor, u8* outTop) {
    *outCursor = CItemBlock_getFlag120EC(category);
    *outTop = 0;
    if ((s8)*outCursor >= 5) {
        *outTop = *outCursor - 4;
        *outCursor = 4;
    }
}

__declspec(noinline) void CSortMenu::StateOpen() {
    if (advanceAnimTransform(mpAnimTrans0, 1.0f) != 0) {
        mState = 2;
        mpLayout->SetAnimationEnable(mpAnimTrans0, false);
        mpLayout->SetAnimationEnable(mpAnimTrans1, true);
        mpLayout->Animate(0);
    }
}

__declspec(noinline) void CSortMenu::StateOpenInfo() {
    if (advanceAnimTransform(mpAnimTrans1, 1.0f) != 0) {
        mState = 3;
        mIdle = 1;
    }
}

__declspec(noinline) void CSortMenu::StateCloseInfo() {
    if (AnimRewindFrame(mpAnimTrans1, 1.0f) != 0) {
        mState = 5;
        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);
    }
}

__declspec(noinline) void CSortMenu::StateClose() {
    if (AnimRewindFrame(mpAnimTrans0, 1.0f) != 0) {
        mState = 0;
        mIdle = 1;
        mOpen = 0;
    }
}

// noinline (here and on the State* handlers): retail calls these via bl from
// every caller; MWCC -inline auto otherwise inlines them.
// Slots past the entry count show an empty string.
__declspec(noinline) void CSortMenu::Refresh() {
    // u8 counter: retail masks i to 8 bits at every use (clrlwi/clrlslwi).
    for (u8 i = 0; i < 5; i++) {
        int idx = i + (s8)mTop;
        if (idx >= (int)mCount) {
            LayoutSetTextBoxFmtValue(mpLayout, (char*)sRowPaneNames[i], "", 0);
        } else {
            LayoutSetTextBoxFmtValue(mpLayout, (char*)sRowPaneNames[i], (char*)mEntries[idx], 0);
        }
    }
    // OPEN: still lbz/extsb in r0 then rlwinm->r4 (93.3%); CMenuArtsSet sibling
    // has the same split.
    mScrollBar.PlaceThumb((s8)mTop);
}

bool CSortMenu::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x2000, "CSortMenu", 1);
        Class_8045F858 region = Class_8045F858(&mMemRegion);

        void* fileData = mFileHandle->getData();

        mtl::MemManager::setMemInitFlag(false);
        mArcResAcc = CLibLayout::createArcResourceAccessor();
        mArcResAcc->Attach(fileData, "arc");

        buildLayout(&mpLayout, mArcResAcc, "mf00_reg00_sort.brlyt");
        bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, "mf00_reg00_sort_in.brlan");
        bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc,
                                "mf00_reg00_sort_info_in.brlan");

        ApplyFontToTxtPanes(mpLayout->GetRootPane(),
                            (const nw4r::ut::Font*)(((CDeviceFontItf*)CDeviceFont::getFontInfo(1, mpLayout))->getResource()));

        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);

        if (mpLayout != NULL) {
            mLoaded = 1;
        }

        mFileHandle = NULL;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}
