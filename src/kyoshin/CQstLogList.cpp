#define IWORK_EVENT_INLINE_DTOR

// Auto-scaffolded catalog TU for kyoshin/CQstLogList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
extern "C" {
    void func_80139124(void*);
    void func_80138078(int);
}
#include "kyoshin/CQstLogList.hpp"
// CfObjectImplMove.hpp (via harness_catalog.hpp) declares lbl_eu_80526324 as
// const char[]; code_80135FDC.hpp's u8[] decl of the same name clashes and
// this TU never uses it - rename it away for this one include.
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device.hpp"
#include "monolib/util/MemManager.hpp"

#include <stdio.h>
#include "kyoshin/code_80135FDC_State.hpp"

// name-guess: quest-pane vertex colour cached by this TU from the layout.
u32 sQuestLogPaneColor = 0xFFFFFFFF;

// Quest-log list constructor. Members come up in declaration order (the
// CSortMenu/QstData wrappers run the retail ctor calls); the three re-init
// blocks are assignments from temporaries, like the menu Init functions.
CQstLogList::CQstLogList(u16 arg2)
    : mFileHandle(0), mArcResAcc(0), mpLayout(0), mpAnim0(0), mpAnim1(0),
      mCur18(0), mScrollBar(0), field_0x170(0), field_0x174(0), field_0x178(0),
      mSortEnabled(1), mSortDescending(0), field_0x17B(0), field_0x17C(0),
      field_0x17D(0), field_0x17E(0), field_0x180(arg2) {
    CQstLogListEntry buf;          // per-entry text buffer

    // Prime the 32 quest text entries: 0xFF/0x00 head bytes + a cleared
    // "%s" string, copied into the shared table. Counter is u32 truncated
    // at the multiply (retail clrlwi) with an unsigned loop compare.
    for (u32 tbl_i = 0; tbl_i < 0x20; tbl_i++) {
        buf.mField0 = -1;
        buf.mField1 = 0;
        sprintf((char*)&buf.mData[0], &lbl_eu_80509AB4[0x10], 0);
        QstLogList_CopyEntry(
            (CQstLogListEntry*)&sQuestLogEntries[(u8)tbl_i * 0x22], &buf);
    }

    sQuestEntryCount = 0;

    // CScrollBar's implicit operator= is emitted out of line (weak
    // __as__10CScrollBarFRC10CScrollBar) because the class is polymorphic.
    mScrollBar = CScrollBar(1);
    mSortMenu = CSortMenu();
    mQstData = QstData();
}

int CQstLogList::IsSortEnabled() { return mSortEnabled; }

// Copies a 0x22-byte quest-log entry: 2 head bytes + 4x8-byte word loop.
// Struct assignment reproduces retail exactly (MWCC emits the counted loop
// with lwzu/stwu update addressing).
extern "C" CQstLogListEntry* QstLogList_CopyEntry(CQstLogListEntry* pDst, const CQstLogListEntry* pSrc) {
    *pDst = *pSrc;
    return pDst;
}


CQstLogList::~CQstLogList() {}

int CQstLogList::IsSortDescending() { return mSortDescending; }

// Returns the quest-list's own "layout loaded" byte only when the scroll bar
// AND the sort menu are both visible (mirrors CSortMenu::IsReady's
// scrollbar-gate pattern).
int CQstLogList::ScrollFlag() {
    if (mScrollBar.isVisible() == 0) {
        return 0;
    }
    if (mSortMenu.IsReady() != 0) {
        return field_0x178;
    }
    return 0;
}


// Quest-log scroll down. With the sort menu open, scroll down through its
// options instead; otherwise advance the row index (field_0x17D), wrapping
// through the scroll position (field_0x17E) against the shared boundary
// (sQuestListScrollBase).
void CQstLogList::ScrollDown() {
    if (mSortDescending != 0) {
        mSortMenu.CursorDown();
        MoveCursor();
    } else {
        if (sQuestListScrollBase >= 10) {
            u8 d = field_0x17D + 1;
            field_0x17D = (s8)d;
            if ((s8)d >= 10) {
                s16 e = field_0x17E + 1;
                field_0x17D = 9;
                field_0x17E = e;
                if (e > sQuestListScrollBase - 10) {
                    field_0x17D = 0;
                    field_0x17E = 0;
                }
            }
        } else {
            u8 d = field_0x17D + 1;
            field_0x17D = (s8)d;
            if ((s8)d >= sQuestListScrollBase) {
                field_0x17D = 0;
                field_0x17E = 0;
            }
        }
        RefreshRows();
        MoveCursor();
        mScrollBar.PlaceThumb((u16)field_0x17E);
    }
    playUISound(1);
}

// Quest-log page up. With the sort menu open, page up through its options;
// otherwise step the scroll position (field_0x17E) back by a page of 10.
void CQstLogList::PageUp() {
    if (mSortDescending != 0) {
        mSortMenu.PageUp();
        MoveCursor();
    } else {
        if (sQuestListScrollBase >= 10) {
            s16 v = field_0x17E - 10;
            field_0x17E = v;
            if (v < 0) {
                field_0x17D = (s8)(v + 9);
                field_0x17E = 0;
                if ((s8)field_0x17D < 0) {
                    field_0x17D = 0;
                }
            }
        } else {
            field_0x17D = 0;
            field_0x17E = 0;
        }
        RefreshRows();
        MoveCursor();
        mScrollBar.PlaceThumb((u16)field_0x17E);
    }
    playUISound(1);
}

// Quest-log page down. With the sort menu open, page down through its
// options; otherwise step the scroll position forward by a page of 10.
void CQstLogList::PageDown() {
    if (mSortDescending != 0) {
        mSortMenu.PageDown();
        MoveCursor();
    } else {
        if (sQuestListScrollBase >= 10) {
            s16 v = field_0x17E + 10;
            field_0x17E = v;
            if (v > sQuestListScrollBase - 10) {
                u8 b = (u8)(v - (sQuestListScrollBase - 10));
                field_0x17D = (s8)b;
                field_0x17E = sQuestListScrollBase - 10;
                if ((s8)b >= 10) {
                    field_0x17D = 9;
                }
            }
        } else {
            field_0x17D = (s8)(sQuestListScrollBase - 1);
            field_0x17E = 0;
            if ((s8)field_0x17D < 0) {
                field_0x17D = 0;
            }
        }
        RefreshRows();
        MoveCursor();
        mScrollBar.PlaceThumb((u16)field_0x17E);
    }
    playUISound(1);
}


// Quest-log sort toggle. With the sort menu active, closing the menu (current
// direction descending) resets the direction and rebuilds; otherwise the
// sort-menu cursor is positioned under the list and the direction is enabled.
void CQstLogList::ToggleSort() {
    if (mSortMenu.IsIdle() == 0) {
        return;
    }
    if (mSortDescending != 0) {
        mCur18.SetVisible(1);
        mSortMenu.Close();
        mSortDescending = 0;
        RefreshHeader();
        MoveCursor();
        playUISound(6);
    } else {
        // Four live values here (self/pool base/root/pane1) drive the
        // retail r28-r31 allocation.
        // Nested-call argument form (right-to-left eval): pane1 is
        // evaluated first and must survive the second FindPaneByName call,
        // reproducing the retail r29/r30/r31 assignment.
        nw4r::math::VEC3 pos;
        char* const s = lbl_eu_80509AB4;
        nw4r::lyt::Pane* root = mpLayout->GetRootPane();
        ScalePaneWorldPos(&pos, root->FindPaneByName(&s[0x2a], true),
                      root->FindPaneByName(&s[0x33], true), root);
        mSortMenu.SetPos(&pos);
        FillSortMenu();
        mCur18.SetVisible(1);
        mSortMenu.Open();
        mSortDescending = 1;
        MoveCursor();
        playUISound(2);
    }
}

// Sort-menu confirm/select: rebuilds the quest list (QstLogList_RefreshHeader) and
// re-sorts (QstLogList_MoveCursor) when the sort menu reports an active button.
void CQstLogList::ConfirmSort(int flag) {
    if (mSortMenu.IsIdle() != 0) {
        mCur18.SetVisible(1);
        mSortMenu.Close();
        mSortDescending = 0;
        RefreshHeader();
        MoveCursor();
        if (flag == 0) {
            playUISound(6);
        }
    }
}

// retail: lwz r0,sSelectedQuestId@sda21; clrlwi r3,r0,16 = (u16)global
u32 CQstLogList::GetSelectedQuestLo() { return sSelectedQuestId & 0xFFFF; }

// Sort-menu open-in animation start (one-shot on mode 0): switch animations,
// prime the scroll bar geometry and request scroll-in.
void CQstLogList::BeginSortOpen() {
    if (field_0x174 == 0) {
        field_0x174 = 1;
        mSortEnabled = 0;
        ShowOpenAnim();
        MoveCursor();
        nw4r::math::VEC3 pos(214.0f, 118.0f, 0.0f);
        mScrollBar.InitRootPane(&pos);
        mScrollBar.UpdateThumb(10, (u16)sQuestListScrollBase);
        mScrollBar.PlaceThumb((u16)field_0x17E);
        mScrollBar.requestScrollIn();
    }
}

void CQstLogList::StepOpenAnim() {
    f32 frame = 1.0f;
    nw4r::lyt::AnimTransform* anim = mpAnim0;
    if (advanceAnimTransform(anim, frame) != 0) {
        field_0x174 = 2;
        ShowSortAnim();
    }
}

void CQstLogList::StepSortAnim() {
    f32 frame = 1.0f;
    nw4r::lyt::AnimTransform* anim = mpAnim1;
    if (advanceAnimTransform(anim, frame) != 0) {
        field_0x174 = 3;
        mCur18.SetVisible(1);
        MoveCursor();
        mSortEnabled = 1;
    }
}

void CQstLogList::RewindSortAnim() {
    if (AnimRewindFrame(mpAnim1, 1.0f) != 0) {
        field_0x174 = 5;
        ShowOpenAnim();
    }
}

void CQstLogList::RewindOpenAnim() {
    if (AnimRewindFrame(mpAnim0, 1.0f) != 0) {
        field_0x174 = 0;
        mSortEnabled = 1;
    }
}

void CQstLogList::ShowOpenAnim() {
    mpLayout->SetAnimationEnable(mpAnim1, false);
    mpLayout->SetAnimationEnable(mpAnim0, true);
}

void CQstLogList::ShowSortAnim() {
    mpLayout->SetAnimationEnable(mpAnim0, false);
    mpLayout->SetAnimationEnable(mpAnim1, true);
}

// One quest-row display update: name pane, detail-pane text chain and the
// icon pane. mode selects the icon resource key (0xFE/0xFF = unused-row
// state, 1 = the active row's texture); a6 picks the color pool; a7 picks
// the icon pane's resource key.
void CQstLogList::DrawRow(const char* name, int questId, int index,
                                                    int mode, u8 a6, u8 a7) {
    char buf[0x20];
    const char* str = BdatGetPtrDirect(name, &lbl_eu_80509AB4[0x42], questId);
    sprintf(buf, &lbl_eu_80509AB4[0x48], index);
    LayoutSetTextBoxFmtValue(mpLayout, buf, (char*)str, 0);
    PaneSetVtxColorByName(mpLayout, buf,
                  a6 != 0 ? 0xC81E1EFFu : sQuestLogPaneColor);

    // Detail text: BDAT lookup chain (name -> column A -> column B -> row
    // string) formatted into the second pane.
    u16 a = BdatGetU16Direct(name, &lbl_eu_80509AB4[0x54], questId);
    u16 b = BdatGetU16Direct((const char*)::cf::CfBdat::spFldNpcListFileData, &lbl_eu_80509AB4[0x5b], a);
    u8 c = BdatGetU8ByTableKey(&lbl_eu_80509AB4[0x63], &lbl_eu_80509AB4[0x70], b);
    const char* str2 = BdatGetPtrDirect(::cf::CfBdat::spFldMapListFileData, &lbl_eu_80509AB4[0x76], c);
    sprintf(buf, &lbl_eu_80509AB4[0x7b], index);
    LayoutSetTextBoxFmtValue(mpLayout, buf, (char*)str2, 0);
    PaneSetVtxColorByName(mpLayout, buf,
                  a6 != 0 ? 0xC81E1EFFu : sQuestLogPaneColor);

    sprintf(buf, &lbl_eu_80509AB4[0x87], index);
    nw4r::lyt::Pane* pane1 = mpLayout->GetRootPane()->FindPaneByName(buf, true);
    const char* str3 = 0;
    // Row-state icon: mode 1 = active-row texture, 0xFE/0xFF = unused-row
    // state. Goto-chain keeps the mode==1 body in the fall-through like
    // retail (if-else-if emits bne-skips with the wrong block order).
    if ((u32)(mode - 0xFE) <= 1) goto modeFE;
    if (mode == 1) goto modeOne;
    goto iconDone;
modeOne:
    str3 = (const char*)mArcResAcc->GetResource(
        0x74696D67u, &lbl_eu_80509AB4[0x94], 0);
    goto iconDone;
modeFE:
    str3 = (const char*)mArcResAcc->GetResource(
        0x74696D67u, &lbl_eu_80509AB4[0xa7], 0);
iconDone:
    if (str3 != 0) {
        PaneSetTexPalette(pane1, (TPLPalette*)str3);
        setPaneVisible(pane1, 1);
    }

    sprintf(buf, &lbl_eu_80509AB4[0xba], index);
    nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName(buf, true);
    const char* str4 = 0;
    switch (a7) {
    case 0:
        str4 = (const char*)mArcResAcc->GetResource(
            0x74696D67u, &lbl_eu_80509AB4[0xca], 0);
        break;
    case 1:
        str4 = (const char*)mArcResAcc->GetResource(
            0x74696D67u, &lbl_eu_80509AB4[0xde], 0);
        break;
    case 3:
        str4 = (const char*)mArcResAcc->GetResource(
            0x74696D67u, &lbl_eu_80509AB4[0xf2], 0);
        break;
    default:
        break;
    }
    if (str4 != 0) {
        PaneSetTexPalette(pane2, (TPLPalette*)str4);
        setPaneVisible(pane2, 1);
    }
}

// Resets the ten quest-row panes: formats each pane name, clears its text
// and hides empty panes.
void CQstLogList::ClearRows() {
    char buf[0x20];
    // No explicit pool-base local: MWCC CSEs the lbl_eu_80509AB4 base into
    // r31 itself (lis hoisted above the LR store, addi sunk to first use),
    // alongside self (r29) and counter (r30) -> retail stmw frame.
    // Counter is u32 with explicit u8 casts at the vararg call sites
    // (retail truncates only there).
    u32 i = 1;
    do {
        sprintf(buf, &lbl_eu_80509AB4[0x48], (u8)i);
        LayoutSetTextBoxFmtValue(mpLayout, buf, &lbl_eu_80509AB4[0x106], 0);
        sprintf(buf, &lbl_eu_80509AB4[0x7b], (u8)i);
        LayoutSetTextBoxFmtValue(mpLayout, buf, &lbl_eu_80509AB4[0x106], 0);
        sprintf(buf, &lbl_eu_80509AB4[0x87], (u8)i);
        nw4r::lyt::Pane* pane =
            mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            setPaneVisible(pane, 0);
        }
        sprintf(buf, &lbl_eu_80509AB4[0xba], (u8)i);
        pane = mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            setPaneVisible(pane, 0);
        }
    } while (++i <= 10);
}

// Scroll-bar/page refresh helper (retail name unmangled - extern "C" keeps
// the bl relocs from the scroll/page handlers bound to the retail symbol).
void CQstLogList::RefreshRows() {
    // Quest text table entry selected by the two signed index bytes.
    // Declaration order matters here: MWCC pre-colors the callee-saved
    // homes for row/e1/questId/e0 in this order, matching retail.
    const char* row;
    u8 e1;
    u32 questId;
    s32 e0;
    s32 sum = field_0x17B + field_0x17C;
    int off = sum * 0x22;
    row = &sQuestLogEntries[off];
    e0 = (s8)sQuestLogEntries[off];
    e1 = row[1];
    ClearRows();
    sQuestListScrollBase = 0;
    mQstData.Sort(e1, (u8)e0);

    // Rebuild the visible rows from the quest-info buffer, starting at the
    // current scroll position; stop after ten rows or the list end.
    int i = 0;
    u16 total = mQstData.GetCount();
    u32 cur = (u16)field_0x17E;
    for (; (u16)cur < total; i++, cur++) {
        if (i >= 10) break;
        QstInfo* entry = mQstData.GetInfo((u16)cur);
        questId = entry->mId;
        u32 idx = SearchThresholdIdx(questId);
        u8 res = (u8)CtrlRemote_TouchBitByArg(questId + 0x220);
        DrawRow((const char*)lbl_eu_80573D18[idx], questId,
                      (u8)(i + 1), res, entry->mShow, entry->mState);
        if (i == field_0x17D) {
            sSelectedQuestId = questId;
            sSelectedQuestIndex = idx;
            sSelectedQuestResource = res;
        }
    }
    sQuestListScrollBase = (u16)total;
    if (mScrollBar.isVisible()) {
        mScrollBar.UpdateThumb(10, (u16)sQuestListScrollBase);
    }
}

// Rebuild the quest list display: with the sort menu open, copy its selection
// state into the cursor; otherwise format the quest icon pane name, position
// the icon against the proportion pane and move the cursor to it.
// Locals are declared buf/pos/tmp so the stack slots land at the retail
// offsets (buf 0x20, pos 0x14, tmp 0x08) keeping r29/r30/r31 live (stmw).
void CQstLogList::MoveCursor() {
    char buf[0x20];
    nw4r::math::VEC3 pos;
    if (mSortDescending != 0) {
        nw4r::math::VEC3 tmp = mSortMenu.GetCursorPos();
        mCur18.setRootPaneTranslate(&tmp);
    } else {
        char* const s = lbl_eu_80509AB4;
        sprintf(buf, &s[0xBA], field_0x17D + 1);
        nw4r::lyt::Pane* pane1 =
            mpLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()
                                      ->FindPaneByName(&s[0x33], true);
        ScalePaneWorldPos(&pos, pane1, pane2, mpLayout->GetRootPane());
        mCur18.setRootPaneTranslate(&pos);
    }
}

// Re-populate the embedded sort menu's array from the shared quest text table
// (one entry per logged quest) and select the current page.
void CQstLogList::FillSortMenu() {
    if (mSortMenu.IsReady() == 0) {
        return;
    }
    mSortMenu.Clear();
    for (u8 i = 0; i < (int)sQuestEntryCount; i++) {
        mSortMenu.Add(&sQuestLogEntries[i * 0x22] + 2);
    }
    mSortMenu.SetSelect((u8)(field_0x17B + field_0x17C));
}

// Rebuild the quest list display from the sort-menu/quest state: formats the
// quest text pane name and loads the entry text (LayoutSetTextBoxText).
void CQstLogList::RefreshHeader() {
    LayoutSetTextBoxText(mpLayout, &lbl_eu_80509AB4[0x107],
                  &sQuestLogEntries[(field_0x17B + field_0x17C) * 0x22] + 2, 0);
}

// Constructor for the 0x22-byte quest-log entry records; called as the array
// element ctor by sinit_80229378 via __construct_array. Retail symbol is
// unmangled (C linkage) - extern "C" keeps the sinit reloc bound to the
// retail name, and the entry ctor returns self (retail `mr r3, r31`).
extern "C" CQstLogListEntry* QstLogList_InitEntry(CQstLogListEntry* self) {
    self->mField0 = -1;  // retail li r5,-1 (signed constant); 0xFF would emit li r5,255
    self->mField1 = 0x00;
    // "%s" (lbl_eu_80509AB4 + 0x10) with a null pointer - clears the text.
    sprintf((char*)&self->mData[0], &lbl_eu_80509AB4[0x10], 0);
    return self;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void __construct_array(void*, void*, int, int, int);
extern "C" void sinit_80229378() {
    __construct_array(sQuestLogEntries, (void*)QstLogList_InitEntry, 0, 0x22, 0x20);
}

// Loads the quest-log arc file (lbl_eu_80509AB4+0x13) into the mem2 handle,
// then primes the scroll bar and sort menu layouts.
void CQstLogList::LoadArc() {
    mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(), &lbl_eu_80509AB4[0x13],
                                        this, 0, 0);
    mScrollBar.loadLayoutArc();
    mSortMenu.Init();
    field_0x178 = 0;
}

// File-load completion callback: builds the quest-log layout from the
// freshly-loaded arc, primes the shared quest-text table with the four fixed
// rows plus every quest type present in the BDAT quest table (sorted by its
// sort key), then sizes the quest panes from the 'timg' message resource.
bool CQstLogList::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
    // Local declaration order fixes the retail stack frame (MWCC allocates
    // the later declarations at lower offsets): quest-type index array at
    // 0xE8, then the four fixed-row buffers and the sorted-row buffer down
    // to 0x30 (0x24 stride).
    u8 indices[0x3C];          // collected quest types
    CQstLogListEntry row0;
    CQstLogListEntry row1;
    CQstLogListEntry row2;
    CQstLogListEntry row3;
    CQstLogListEntry rowSort;  // entry buffer reused for sorted rows

    // Scratch heap region (RAII Class_8045F858 guard), then detach the file
    // buffer and attach it to the layout arc resource accessor. The handle
    // is fetched before the string-pool base is materialized (retail order).
    u32 mem2 = mtl::MemManager::getHandleMEM2();
    char* const s = lbl_eu_80509AB4;
    mMemRegion.createRegion((int)mem2, 0x10000, &s[0x112], 0);
    Class_8045F858 regionGuard(&mMemRegion);

    // Detach the file buffer before the accessor is created; it is attached
    // to the layout arc resource accessor below.
    void* fileData = mFileHandle->getData();
    mtl::MemManager::setMemInitFlag(false);
    mArcResAcc =
        (nw4r::lyt::ArcResourceAccessor*)CLibLayout::createArcResourceAccessor();
    mArcResAcc->Attach(fileData, &s[0x11e]);

    buildLayout(&mpLayout, mArcResAcc, &s[0x122]);
    bindLayoutAnimTransform(mpLayout, &mpAnim0, mArcResAcc, &s[0x135]);
    bindLayoutAnimTransform(mpLayout, &mpAnim1, mArcResAcc, &s[0x14b]);

    // Bind the font handle into the layout's root pane.
    nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
    void* fontObj = CDeviceFont::getFontInfo(1, mpLayout);
    u32 fontResult = ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

    ShowOpenAnim();
    mpLayout->Animate(0);

    // Build the cursor on the stack, copy its body into the member region
    // (skipping the +0x00 vtable pointer) and destroy the temp.
    mCur18 = CCur18(CUICfManager_getArcResourceAccessor());
    mCur18.initLayout();

    // Prime the four fixed quest-log rows (0xFF/0x00 head bytes + formatted
    // text), appending each entry to the shared quest-text table. The
    // buffers are declared in reverse use order so the stack slots land at
    // the retail offsets (b4 lowest, b0 highest).
    // Prime the four fixed quest-log rows (0xFF head byte + formatted text),
    // appending each entry to the shared quest-text table.
    row0.mField0 = 0xFF;
    row0.mField1 = 0;
    sprintf((char*)&row0.mData[0], &s[0x10],
            BdatTouchStringCell(&s[0x166], &s[0x76], 0x44));
    char* const table = sQuestLogEntries;
    u32 n0 = sQuestEntryCount;
    sQuestEntryCount = n0 + 1;
    QstLogList_CopyEntry((CQstLogListEntry*)&table[n0 * 0x22], &row0);

    row1.mField0 = 0xFF;
    row1.mField1 = 1;
    sprintf((char*)&row1.mData[0], &s[0x10],
            BdatTouchStringCell(&s[0x166], &s[0x76], 0x45));
    u32 n1 = sQuestEntryCount;
    sQuestEntryCount = n1 + 1;
    QstLogList_CopyEntry((CQstLogListEntry*)&table[n1 * 0x22], &row1);

    row2.mField0 = 0xFF;
    row2.mField1 = 2;
    sprintf((char*)&row2.mData[0], &s[0x10],
            BdatTouchStringCell(&s[0x166], &s[0x76], 0x46));
    u32 n2 = sQuestEntryCount;
    sQuestEntryCount = n2 + 1;
    QstLogList_CopyEntry((CQstLogListEntry*)&table[n2 * 0x22], &row2);

    row3.mField0 = 0xFF;
    row3.mField1 = 3;
    sprintf((char*)&row3.mData[0], &s[0x10],
            BdatTouchStringCell(&s[0x166], &s[0x76], 0x47));
    u32 n3 = sQuestEntryCount;
    sQuestEntryCount = n3 + 1;
    QstLogList_CopyEntry((CQstLogListEntry*)&table[n3 * 0x22], &row3);

    // Collect every quest type (2..0x1c) that has at least one active,
    // uncompleted quest in the BDAT quest table. Types 6..0x1c dispatch
    // through the retail jump table into the shared quest scan below.
    s32 numQuests = (s32)Bdat_GetMaxRow_B1EC(::cf::CfBdat::spLandmarkListFileData);
    u8 count = 0;
    for (u8 i = 2; i <= 0x1c; i++) {
        switch (i) {
        case 6: case 7: case 8: case 9: case 0xA: case 0xB: case 0xC: case 0xD:
        case 0xE: case 0xF: case 0x10: case 0x11: case 0x12: case 0x13: case 0x14:
        case 0x15: case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A: case 0x1B:
        case 0x1C:
            for (s32 j = 1; j <= numQuests; j++) {
                if (BdatGetU8Direct((const void*)::cf::CfBdat::spLandmarkListFileData, &s[0x70], j) == i &&
                    BdatGetU8Direct((const void*)::cf::CfBdat::spLandmarkListFileData, &s[0x170], j) == 0 &&
                    CtrlRemote_TouchBitByArg((u32)(j + 0x20c8)) != 0) {
                    indices[count++] = i;
                    break;
                }
            }
            break;
        }
    }

    // Bubble-sort the collected quest types by their BDAT sort key (biggest
    // key first) and stop early once a pass makes no swap.
    u8 n = count;
    s8 last = (s8)(n - 1);
    for (u8 outer = 0; outer < n; outer++) {
        bool swapped = false;
        for (s8 inner = 0; inner < (s8)(last - outer); inner++) {
            u8 a = indices[inner];
            u8 b = indices[inner + 1];
            if (BdatGetU8Direct((const void*)::cf::CfBdat::spFldMapListFileData, &s[0xa], a) >
                BdatGetU8Direct((const void*)::cf::CfBdat::spFldMapListFileData, &s[0xa], b)) {
                u8 tmpSwap = a;
                indices[inner] = b;
                indices[inner + 1] = tmpSwap;
                swapped = true;
            }
        }
        if (!swapped) break;
    }

    // Append the sorted quest types as rows 5+ (byte1 = 4) with their names.
    for (u8 k = 0; k < count; k++) {
        u8 idx = indices[k];
        char* text = BdatGetPtrDirect(::cf::CfBdat::spFldMapListFileData, &s[0x76], idx);
        rowSort.mField0 = idx;
        rowSort.mField1 = 4;
        sprintf((char*)&rowSort.mData[0], &s[0x10], text);
        u32 nk = sQuestEntryCount;
        sQuestEntryCount = nk + 1;
        QstLogList_CopyEntry((CQstLogListEntry*)&table[nk * 0x22], &rowSort);
    }

    // Refresh the sort-menu header text and rebuild the list display.
    char* t = BdatTouchStringCell(&s[0x166], &s[0x76], 2);
    LayoutSetTextBoxFmtValue(mpLayout, &s[0x179], t, 0);
    RefreshHeader();

    // Look up the quest-log message texture ('timg') and size the panes from
    // its row/column counts.
    const char* sel = cf::CfGameManager::isClassicController(-1) != 0
                          ? &s[0x185]
                          : &s[0x18e];
    u32 msgId = BdatGetU16ByTableKey(&s[0x197], sel, 0x61);
    char* texName = MakeTplNameSysFile((u32)(u16)msgId);
    CQstLogListMsgObj* obj = (CQstLogListMsgObj*)CUICfManager_getArcResourceAccessor()->GetResource(
        0x74696D67, texName, 0);
    if (obj != 0) {
        PaneSetTexPaletteByName(mpLayout, &s[0x1a5], (TPLPalette*)obj);
        CQstLogListCoords* coords = obj->chain->pCoords;
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(
            &s[0x1a5], true);
        if (pane != 0) {
            reinterpret_cast<PaneSizeRegion*>(pane)->width = coords->c2;
            reinterpret_cast<PaneSizeRegion*>(pane)->height = coords->c0;
        }
    }

    // Store the quest text color fetched from the log pane.
    sQuestLogPaneColor = mpLayout->GetRootPane()
                          ->FindPaneByName(&s[0x1b1], true)
                          ->GetVtxColor(0);

    mQstData.Refresh();
    RefreshRows();

    // Position the selection on the quest-log's own list entry.
    u16 listIdx = mQstData.FindIndex(field_0x180);
    if (listIdx < 10) {
        field_0x17D = (s8)listIdx;
        field_0x17E = 0;
    } else {
        field_0x17D = 9;
        field_0x17E = (s16)(listIdx - 9);
    }

    RefreshRows();
    field_0x178 = 1;
    field_0x170 = 1;
    mFileHandle = 0;
    mMemRegion.validateHeap();
    return 1;
    }
    return 0;
}

// Draws the layout plus the scroll bar, sort menu and cursor when the
// quest-log layout is loaded (field_0x170).
void CQstLogList::Draw(nw4r::lyt::DrawInfo* drawInfo) {
    if (field_0x170 != 0) {
        drawLayout(mpLayout, drawInfo, 0, 1);
        mScrollBar.draw(drawInfo);
        mSortMenu.Draw(drawInfo);
        mCur18.DrawLayout(drawInfo);
    }
}

// Unload the quest-log screen: close the arc file handle, delete the layout,
// release the resource accessor and tear down cursor/scroll bar/sort menu.
void CQstLogList::Teardown() {
    closeFileHandle(&mFileHandle);
    field_0x170 = 0;
    if (mpLayout != 0) {
        delete mpLayout;
        mpLayout = 0;
    }
    releaseArcResourceAccessor(mArcResAcc);
    mCur18.cleanup();
    mScrollBar.Teardown();
    mSortMenu.Term();
    mMemRegion.deleteRegion();
}

// Sort-menu confirm: adopt the selected page/entry indices into the list,
// reset the row cursor and rebuild; otherwise clear sorting once entries exist.
void CQstLogList::ApplySortSelection() {
    if (mSortDescending != 0) {
        if (mSortMenu.IsIdle() == 0) {
            return;
        }
        field_0x17B = mSortMenu.GetCursor();
        field_0x17C = mSortMenu.GetTop();
        field_0x17D = 0;
        field_0x17E = 0;
        ConfirmSort(1);
        RefreshRows();
    } else {
        if (sQuestListScrollBase > 0) {
            mSortEnabled = 0;
        }
    }
    playUISound(3);
}

// Per-frame update while the quest-log list is loaded: run the active open/
// close animation step, then advance layout, cursor, scroll bar and sort menu.
void CQstLogList::FrameUpdate() {
    if (field_0x170 != 0) {
        switch (field_0x174) {
        case 1:
            StepOpenAnim();
            break;
        case 2:
            StepSortAnim();
            break;
        case 4:
            RewindSortAnim();
            break;
        case 5:
            RewindOpenAnim();
            break;
        }
        mpLayout->Animate(0);
        mCur18.Update();
        mScrollBar.UpdateDispatch();
        mSortMenu.Move();
    }
}

// Sort-menu exit callback: when the sort menu is closing (mode 3) switch to
// mode 4, disable sorting and request scroll-bar out.
void CQstLogList::EndSortMode(int flag) {
    if (field_0x174 == 3) {
        field_0x174 = 4;
        mSortEnabled = 0;
        ShowSortAnim();
        mCur18.SetVisible(0);
        mScrollBar.requestScrollOut();
        if (flag != 0) {
            playUISound(6);
        }
    }
}

// Quest-log scroll up. With the sort menu open, scroll up through its
// options; otherwise step the row index (field_0x17D) back, wrapping through
// the scroll position (field_0x17E) against the shared boundary.
void CQstLogList::ScrollUp() {
    if (mSortDescending != 0) {
        mSortMenu.CursorUp();
        MoveCursor();
    } else {
        u8 d = field_0x17D - 1;
        field_0x17D = (s8)d;
        if ((s8)d < 0) {
            field_0x17D = 0;
            field_0x17E -= 1;
            if (field_0x17E < 0) {
                if (sQuestListScrollBase >= 10) {
                    field_0x17D = 9;
                    field_0x17E = sQuestListScrollBase - 10;
                } else {
                    u8 g = (u8)(sQuestListScrollBase - 1);
                    field_0x17D = (s8)g;
                    field_0x17E = 0;
                    if ((s8)g < 0) {
                        field_0x17D = 0;
                    }
                }
            }
        }
        RefreshRows();
        MoveCursor();
        mScrollBar.PlaceThumb((u16)field_0x17E);
    }
    playUISound(1);
}

// Unreferenced in retail (mwld dead-strips it), but it must exist in this TU:
// under -O4,s MWCC refuses to inline an inline/implicit function that has more
// than one call site in the translation unit (docs/MWCC_CASES.md, "When MWCC
// inlines a copy assignment"). Retail's ctor calls QstInfo's implicit
// operator= out of line inside the inlined QstData copy, while CMenuQuestLog
// (one site) inlines it, so the original CQstLogList.cpp had a second
// QstInfo assignment in code that did not survive the link. Its exact shape
// is unknown; this is the minimal stand-in.
void QstLogList_CopyQstInfo(QstInfo* dst, const QstInfo* src) { *dst = *src; }
