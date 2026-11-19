// Decomp of kyoshin/CTutorialList.
// Tutorial list widget (IWorkEvent): loads the list layout, owns the cursor,
// scrollbar, sort menu and the tutorial id table.

// Headers in the harness_catalog.hpp chain declare lbl_eu_80526324/
// lbl_eu_80526300 as const char[], while code_80135FDC.hpp (pulled in via
// CTutorialList.hpp) declares them as u8[]. This TU never uses either -
// keep the char decls unrenamed and rename every later (u8) expansion away.
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/CTutorialList.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <stdio.h>
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Layout / BDAT helpers with unmangled retail names.
extern "C" u32 CtrlRemote_TouchBitByArg(u32 resourceId);
extern "C" u32 Bdat_GetMaxRow_B1EC(void* fp);
extern "C" void* Bdat_GetTable_AA34();
#include "kyoshin/cf/CfGameManager.hpp"
extern "C" int KyoshinHeap_GetField44();

// .sdata2 u16 -> f32 conversion goes through MWCC's 0x4330-magic double.
// Global term/active flag: the loaded common BDAT table (.sbss word, sda21).
u32 lbl_eu_80664BF0;  // loaded MNU_ttrl BDAT table (.sbss, owned by this unit)

// Overlay structs for OnFileEvent's 'timg' message resource: the resource
// object holds a chain whose first field points at the u16 row/column pair
// used to size the message panes.
struct CTutorialCoords {
    u16 c0;  // 0x0 column
    u16 c2;  // 0x2 row
};
struct CTutorialMsgChain {
    CTutorialCoords* pCoords;  // 0x0
};
// Whole-pane view so the size fields land at their real offsets (Pane::mSize
// at +0x4C: width, height); the stores are two separate float conversions.
struct CTutorialPaneView {
    u8 _00[0x4c];
    f32 width;
    f32 height;
};
struct CTutorialMsgObj {
    u8 gap[0x8];
    CTutorialMsgChain* chain;  // 0x8
};

// Id table ctor: zero the three summary halfwords.
CTutorialIdTable::CTutorialIdTable() {
    mCount = 0;
    mSelRow = 0;
    mScrollBase = 0;
}

CTutorialIdTable::~CTutorialIdTable() {}

// Rebuild the tutorial id table: collect every active tutorial whose BDAT
// category matches the filter (all of them when filter==0), sort ascending by
// name key, then locate the target id to derive the row/scroll split stored
// after the table.
// -O4,s (unit) keeps the retail rolled mtctr/bdnz loops.
void CTutorialIdTable::Rebuild(u16 target, int filter) {
    if (lbl_eu_80664BF0 == 0) return;
    mCount = 0;
    void* table = (void*)lbl_eu_80664BF0;
    int total = Bdat_GetMaxRow_B1EC(table);
    const int catFilter = filter - 1;
    for (int id = 1; id <= total; id++) {
        if (CtrlRemote_TouchBitByArg(id + 0x333f) == 0) continue;
        if (filter != 0 &&
            (int)(u8)BdatGetU8Direct((const void*)table, (const char*)("type_cat"), id) != catFilter)
            continue;
        mIds[mCount++] = id;
    }
    // Bubble-sort ascending by each entry's name lookup key; stop as soon as
    // a full pass performs no swap.
    for (int i = 0; i < mCount; i++) {
        int swapped = 0;
        for (int j = 0; j < (int)(mCount - 1 - i); j++) {
            u16 a = mIds[j];
            u16* p = &mIds[j];
            u16 b = p[1];
            if ((u8)BdatGetU8Direct((const void*)table, (const char*)("order"), a) >
                (u8)BdatGetU8Direct((const void*)table, (const char*)("order"), b)) {
                swapped = 1;
                const u16 x = p[1] ^ p[0];
                const u16 nb = p[1] ^ x;
                p[1] = nb;
                p[0] = x ^ nb;
            }
        }
        if (swapped == 0) break;
    }
    if (target != 0) {
        // Locate the target id and derive its page row / scroll base.
        int pos = 0;
        const u16* e = &mIds[0];
        for (int idx = 0; idx < mCount; idx++) {
            if (*e == target) {
                pos = idx;
                break;
            }
            ++e;
        }
        mSelRow = pos >= 10 ? 9 : pos;
        // Retail assigns the raw split then clamps negatives with a second
        // store (sth before the extsh. sign test).
        s16 base = pos - 9;
        mScrollBase = base;
        if (base < 0) {
            mScrollBase = 0;
        }
    }
}

u16 CTutorialIdTable::GetEntryIdAt(u16 index) {
    if (index >= mCount)
        return 0;
    return mIds[index];
}

// Tutorial list constructor (retail __ct__13CTutorialListFUs). Members are
// constructed in declaration order from the init list; the body then rebuilds
// the scrollbar, sort menu and id table by assigning from temporaries.
CTutorialList::CTutorialList(u16 count)
    : mRegion(), mFileHandle0(0), mFileHandle1(0), mAccessor(0), mLayout(0),
      mAnim24(0), mAnim28(0), mCursor(0), mScrollBar(0), mSortMenu(),
      mField174(0), mState175(0), mField176(0), mInitialized(1),
      mField178(0), mField17A(0), mField17C(count), mField17E(0), mIdTable() {
    mScrollBar = CScrollBar(1);
    mSortMenu = CSortMenu();
    mIdTable = CTutorialIdTable();
}

CTutorialList::~CTutorialList() {}

// File loading: request the list file and the common archive file, then
// initialise the scrollbar and the sort-menu sub-object.
void CTutorialList::TutorialList_LoadLayoutFiles() {
    mFileHandle0 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/TutorialList.arc", this, 0, 0);
    mFileHandle1 = CDeviceFile::readCommonArchiveFile(
        (mtl::ALLOC_HANDLE)KyoshinHeap_GetField44(),
        "/common/jp/bdat_menu_ttrl.bin", this, 0, 0);
    mScrollBar.loadLayoutArc();
    mSortMenu.Init();
    mField176 = 0;
}

// Per-frame update: dispatch on the anim state byte, animate the layout, then
// move the cursor, scrollbar and sort-menu sub-object.
void CTutorialList::TutorialList_UpdatePerFrame() {
    if (mField174 == 0) return;
    switch (mState175) {
    case 1:
        TutorialList_OnOpenAnimDone();
        break;
    case 2:
        TutorialList_OnIdleAnimDone();
        break;
    case 4:
        TutorialList_OnCloseAnimRewind();
        break;
    case 5:
        TutorialList_OnShutdownAnimRewind();
        break;
    }
    mLayout->Animate(0);
    mCursor.Update();
    mScrollBar.UpdateDispatch();
    mSortMenu.Move();
}

// Render the tutorial list when the visible gate byte is set: draw the
// layout, scrollbar, sort menu and cursor with the given draw info.
void CTutorialList::TutorialList_DrawVisible(nw4r::lyt::DrawInfo* drawInfo) {
    if (mField174 != 0) {
        drawLayout(mLayout, drawInfo, 0, 1);
        mScrollBar.draw(drawInfo);
        mSortMenu.Draw(drawInfo);
        mCursor.DrawLayout(drawInfo);
    }
}

// Release the list widget: free the CBdat index, close the two file handles,
// release the bound layout and resource accessor, then run the
// cursor/scrollbar/sort-menu/region teardown helpers.
void CTutorialList::TutorialList_ReleaseResources() {
    CBdat::getEntry(4);
    closeFileHandle(&mFileHandle0);
    closeFileHandle(&mFileHandle1);
    mField174 = 0;
    if (mLayout != 0) {
        delete mLayout;
        mLayout = 0;
    }
    releaseArcResourceAccessor(mAccessor);
    lbl_eu_80664BF0 = 0;
    mCursor.cleanup();
    mScrollBar.Teardown();
    mSortMenu.Term();
    mRegion.deleteRegion();
}

int CTutorialList::TutorialList_IsListVisible() {
    // Inner test written as != so MWCC emits beq-forward + li-tail like retail.
    if (mScrollBar.isVisible() == 0)
        return 0;
    if (mSortMenu.IsReady() != 0)
        return mField176;
    return 0;
}

int CTutorialList::TutorialList_IsInitialized() { return mInitialized; }

// Initialise the list widget: latch the state byte, clear the initialised
// flag, seed the scrollbar fade colours and sizes, then run the two
// per-frame helpers.
void CTutorialList::TutorialList_OpenListInit() {
    if (mState175 != 0) return;
    mState175 = 1;
    mInitialized = 0;
    TutorialList_EnableOpenAnims();

    f32 v[3];
    v[0] = 144.0f;
    v[1] = 108.0f;
    v[2] = 0.0f;
    mScrollBar.InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(v));
    mScrollBar.UpdateThumb(0xa, mIdTable.mCount);
    mScrollBar.PlaceThumb(mField17A);
    TutorialList_UpdateRowTexts();
    TutorialList_MoveCursorToRow();
}

// Start closing the list once anim state 3 is reached: latch state 4, clear
// the initialised flag, swap the anim transforms, park the cursor, request
// the scrollbar scroll-out and play sound 6.
void CTutorialList::TutorialList_BeginClose() {
    if (mState175 == 3) {
        mState175 = 4;
        mInitialized = 0;
        TutorialList_EnableCloseAnims();
        mCursor.SetVisible(0);
        mScrollBar.requestScrollOut();
        playUISound(6);
    }
}

// Page up: wheel the sort menu back one page when idle; when the menu is
// still animating, decrement the page byte and wrap to the previous 10-row
// block of the content.
void CTutorialList::TutorialList_PageUpIdle() {
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() == 0)
            return;
        mSortMenu.CursorUp();
        nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
        mCursor.setRootPaneTranslate(&pos);
    } else {
        // idle page-up: wrap to the previous 10-row block of the content
        u8 page = mField178;
        u16 content = mIdTable.mCount;
        u8 cnt8 = (u8)content;
        mField178 = page - 1;
        if ((s8)(u8)(page - 1) < 0) {
            mField178 = 0;
            s16 sel = mField17A;
            mField17A = sel - 1;
            // explicit (s16) conversion -> separate subi + extsh. like retail
            if ((s16)(sel - 1) < 0) {
                if (cnt8 >= 10) {
                    mField178 = 9;
                    mField17A = cnt8 - 10;
                } else {
                    // retail reuses the zero reg here: both stores are 0
                    mField178 = cnt8 - 1;
                    mField17A = 0;
                    if ((s8)(u8)(cnt8 - 1) < 0)
                        mField178 = 0;
                }
            }
        }
        TutorialList_UpdateRowTexts();
        TutorialList_MoveCursorToRow();
        mScrollBar.PlaceThumb(mField17A);
    }
    playUISound(1);
}

// Page up (alt): same gate shape as PageUpIdle but with different wrap
// arithmetic on the page/selection bytes.
void CTutorialList::TutorialList_PageUpWrap() {
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() == 0)
            return;
        mSortMenu.CursorDown();
        nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
        mCursor.setRootPaneTranslate(&pos);
    } else {
        // idle page-up: step the page byte and wrap to the previous block
        u8 count = (u8)mIdTable.mCount;
        if (count >= 10) {
            mField178 = mField178 + 1;
            if ((s8)mField178 >= 10) {
                int limit = count - 10;
                mField178 = 9;
                mField17A = (s16)mField17A + 1;
                if ((int)(s16)mField17A > limit) {
                    mField178 = 0;
                    mField17A = 0;
                }
            }
        } else {
            mField178 = mField178 + 1;
            if ((s8)(u8)mField178 >= count) {
                mField178 = 0;
                mField17A = 0;
            }
        }
        TutorialList_UpdateRowTexts();
        TutorialList_MoveCursorToRow();
        mScrollBar.PlaceThumb(mField17A);
    }
    playUISound(1);
}

// Page down: advance one page while the sort menu is busy; otherwise wheel it
// forward and move the cursor onto the new entry.
void CTutorialList::TutorialList_PageDownStep() {
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() == 0)
            return;
        mSortMenu.PageUp();
        nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
        mCursor.setRootPaneTranslate(&pos);
    } else {
        u8 count = (u8)mIdTable.mCount;
        if (count >= 10) {
            s16 sel = mField17A;
            s16 v = sel - 10;
            mField17A = v;
            if (v < 0) {
                // page byte = last row of the previous block; clamp when negative
                int b = v + 9;
                mField178 = b;
                mField17A = 0;
                if ((s8)(u8)b < 0)
                    mField178 = 0;
            }
        } else {
            mField178 = 0;
            mField17A = 0;
        }
        TutorialList_UpdateRowTexts();
        TutorialList_MoveCursorToRow();
        mScrollBar.PlaceThumb(mField17A);
    }
    playUISound(1);
}

// Page down (alt): busy path wheels the menu forward; idle path advances the
// selection by 10 and re-derives the page byte.
void CTutorialList::TutorialList_PageDownBlock() {
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() == 0)
            return;
        mSortMenu.PageDown();
        nw4r::math::VEC3 pos = mSortMenu.GetCursorPos();
        mCursor.setRootPaneTranslate(&pos);
    } else {
        // idle page-down: advance one full 10-row block
        u8 count = (u8)mIdTable.mCount;
        if (count >= 10) {
            int limit = count - 10;
            s16 v = (s16)mField17A + 10;
            mField17A = v;
            if ((int)v > limit) {
                u8 d = (u8)((int)v - limit);
                mField178 = d;
                mField17A = limit;
                if ((s8)d >= 10)
                    mField178 = 9;
            }
        } else {
            // keep the decrement unmasked in an int so retail's single-reg
            // subi/stb/extsb shape is reproduced
            int last = count - 1;
            mField178 = last;
            mField17A = 0;
            if ((s8)(u8)last < 0)
                mField178 = 0;
        }
        TutorialList_UpdateRowTexts();
        TutorialList_MoveCursorToRow();
        mScrollBar.PlaceThumb(mField17A);
    }
    playUISound(1);
}

u16 CTutorialList::TutorialList_GetSelectedEntryId() {
    return mIdTable.GetEntryIdAt((u16)((s8)mField178 + (s16)mField17A));
}

void CTutorialList::TutorialList_RefreshRowTexts() { TutorialList_UpdateRowTexts(); }

// Confirm/open: while the sort menu is animating just refresh cursor/page
// text; on first open resolve the two anchor panes around the scrollbar,
// place the sort menu there, rebuild its entries and move the cursor onto the
// selected row.
void CTutorialList::TutorialList_ConfirmOpenMenu() {
    if (mSortMenu.IsOpen() != 0) {
        if (mSortMenu.IsIdle() == 0)
            return;
        mCursor.SetVisible(1);
        mSortMenu.Close();
        TutorialList_UpdateTitleText();
        TutorialList_MoveCursorToRow();
        playUISound(6);
        return;
    }
    nw4r::math::VEC3 pos;
    nw4r::lyt::Pane* root = mLayout->GetRootPane();
    // Both panes resolved inline in the call: MWCC evaluates args
    // right-to-left, so the bar pane (arg 3) is looked up and spilled first.
    ScalePaneWorldPos(&pos,
                  root->FindPaneByName("nul_btn", true),
                  root->FindPaneByName("nul_proportion", true),
                  root);
    mSortMenu.SetPos(&pos);
    TutorialList_RebuildSortEntries();
    mCursor.SetVisible(1);
    mSortMenu.Open();
    nw4r::math::VEC3 curPos = mSortMenu.GetCursorPos();
    mCursor.setRootPaneTranslate(&curPos);
    playUISound(2);
}

// Tail-calls the sort menu's visibility query on the embedded sort menu.
int CTutorialList::TutorialList_IsSortMenuActive() { return mSortMenu.IsOpen(); }

// Advance the list: gate on the sort-menu active/button flags, move the
// cursor, refresh the sort menu, run the per-frame helpers and (when
// requested) play the confirm sound.
void CTutorialList::TutorialList_AdvanceSelection(int arg) {
    if (mSortMenu.IsOpen() == 0) return;
    if (mSortMenu.IsIdle() == 0) return;
    mCursor.SetVisible(1);
    mSortMenu.Close();
    TutorialList_UpdateTitleText();
    TutorialList_MoveCursorToRow();
    if (arg == 0) {
        playUISound(6);
    }
}

// Rebuild the list content once the sort menu finished loading: gate on the
// sort-menu active/button flags, latch the page byte, rebuild the id table,
// reset the page/selection ids, size the scrollbar to the content and play
// the confirm sound.
void CTutorialList::TutorialList_RebuildAfterLoad() {
    if (mSortMenu.IsOpen() == 0) return;
    if (mSortMenu.IsIdle() == 0) return;
    s32 page = mSortMenu.GetCursor();
    mField17E = (s8)page;
    mIdTable.Rebuild(0, (u8)page);
    mField178 = 0;
    mField17A = 0;
    mScrollBar.UpdateThumb(0xa, mIdTable.mCount);
    mScrollBar.PlaceThumb(mField17A);
    TutorialList_AdvanceSelection(1);
    TutorialList_UpdateRowTexts();
    playUISound(3);
}

int CTutorialList::TutorialList_GetMenuStateCode() {
    return (mSortMenu.IsOpen() != 0) + 0x73;
}

// Animation-finish handlers: the +0x24/+0x28 anim transform reached its end
// frame -> latch the state byte and run the follow-up.
void CTutorialList::TutorialList_OnOpenAnimDone() {
    if (advanceAnimTransform(mAnim24, 1.0f) != 0) {
        mState175 = 2;
        TutorialList_EnableCloseAnims();
        mScrollBar.requestScrollIn();
    }
}

void CTutorialList::TutorialList_OnIdleAnimDone() {
    if (advanceAnimTransform(mAnim28, 1.0f) != 0) {
        mState175 = 3;
        TutorialList_MoveCursorToRow();
        mInitialized = 1;
    }
}

void CTutorialList::TutorialList_OnCloseAnimRewind() {
    if (AnimRewindFrame(mAnim28, 1.0f) != 0) {
        mState175 = 5;
        TutorialList_EnableOpenAnims();
    }
}

void CTutorialList::TutorialList_OnShutdownAnimRewind() {
    if (AnimRewindFrame(mAnim24, 1.0f) != 0) {
        mState175 = 0;
        mInitialized = 1;
    }
}

// Disable the +0x28 anim transform and enable the +0x24 one.
void CTutorialList::TutorialList_EnableOpenAnims() {
    mLayout->SetAnimationEnable(mAnim28, false);
    mLayout->SetAnimationEnable(mAnim24, true);
}

// Re-enable the two anim transforms on the layout: mAnim24 disabled, mAnim28
// enabled.
void CTutorialList::TutorialList_EnableCloseAnims() {
    mLayout->SetAnimationEnable(mAnim24, false);
    mLayout->SetAnimationEnable(mAnim28, true);
}

// Per-frame tutorial list upkeep. For each of the 10 rows: format the icon
// pane name ("pic_icon%02d") and the title pane name ("txt_tit%02d"), resolve
// the icon pane through the layout root-pane vtable call, and either push the
// title text directly (empty string) or walk the text-lookup path keyed on
// the id table.
void CTutorialList::TutorialList_UpdateRowTexts() {
    for (u32 i = 0; i < 10; i++) {
        char buf[0x20];
        // Retail computes the id-table index early (live across the layout
        // calls, kept in callee-saved r28); the mField17A read sign-extends
        // (lha).
        u8 idx = (u8)(i + (s16)mField17A);
        sprintf(buf, "pic_icon%02d", (u8)i + 1);
        nw4r::lyt::Pane* pane = mLayout->GetRootPane();
        nw4r::lyt::Pane* text = pane->FindPaneByName(buf, true);
        setPaneVisible(text, 0);
        sprintf(buf, "txt_tit%02d", (u8)i + 1);
        u16 sel = mIdTable.GetEntryIdAt(idx);
        if (sel == 0) {
            LayoutSetTextBoxFmtValue(mLayout, buf, "", 0);
        } else {
            char* txt = BdatGetPtrDirect((const void*)lbl_eu_80664BF0, "title", sel);
            LayoutSetTextBoxFmtValue(mLayout, buf, txt, 0);
            if (BdatGetU8Direct((const void*)lbl_eu_80664BF0, (const char*)("type"), sel) == 0) {
                if (CtrlRemote_TouchBitByArg((u16)sel + 0x33bf) == 0) {
                    setPaneVisible(text, 1);
                }
            }
        }
    }
}

// Move the cursor onto the current tutorial row: format the page number,
// resolve the root pane, find the page-name pane and the scrollbar thumb
// pane, ask ScalePaneWorldPos for the position between them and feed it to the
// cursor's Move virtual (vtable +0x10).
void CTutorialList::TutorialList_MoveCursorToRow() {
    // Retail frame: pos at sp+0x08 (16-byte slot), name buffer at sp+0x18.
    char name[0x28];
    f32 pos[4];
    sprintf(name, "nul_tit%02d", (int)mField178 + 1);
    mCursor.SetVisible(1);
    nw4r::lyt::Pane* pagePane =
        mLayout->GetRootPane()->FindPaneByName(name, true);
    nw4r::lyt::Pane* barPane =
        mLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
    ScalePaneWorldPos((nw4r::math::VEC3*)pos, pagePane, barPane, mLayout->GetRootPane());
    mCursor.setRootPaneTranslate((const nw4r::math::VEC3*)pos);
}

// Rebuild the sort menu entries: reset the menu, push the four tutorial-row
// labels (text ids 0x25-0x28) and select the current page.
void CTutorialList::TutorialList_RebuildSortEntries() {
    if (mSortMenu.IsOpen() != 0) return;
    mSortMenu.Clear();
    mSortMenu.Add(BdatTouchStringCell("MNU_main", "name", 0x25));
    mSortMenu.Add(BdatTouchStringCell("MNU_main", "name", 0x26));
    mSortMenu.Add(BdatTouchStringCell("MNU_main", "name", 0x27));
    mSortMenu.Add(BdatTouchStringCell("MNU_main", "name", 0x28));
    mSortMenu.SetSelect(mField17E);
}

void CTutorialList::TutorialList_UpdateTitleText() {
    char* text = BdatTouchStringCell("MNU_main", "name", mField17E + 0x25);
    LayoutSetTextBoxFmtValue(mLayout, "txt_btn", text, 0);
}

// Open the tutorial list once the layout resource finished loading: flag the
// visible selection, rebuild the id table, copy the row/selection ids and run
// the per-frame helper.
void CTutorialList::TutorialList_OpenAfterFilesReady() {
    if (mAccessor == 0 || mFileHandle1 != 0) return;
    mField176 = 1;
    mField174 = 1;
    mIdTable.Rebuild(mField17C, 0);
    mField178 = (s8)mIdTable.mSelRow;
    mField17A = (u16)(s32)mIdTable.mScrollBase;
    TutorialList_UpdateRowTexts();
}

// File-load completion callback: on the layout-archive handle, rebuild the
// whole scene (region guard, arc accessor, layout + two anims, font binding,
// message pane sizing, embedded cursor); on the shared-archive handle, hand
// its data to the BDAT layer and latch the shared table pointer.
bool CTutorialList::OnFileEvent(CEventFile* event) {
    if (mFileHandle0 == event->mFileHandle) {
        // Layout archive finished loading: rebuild the whole scene.
        mRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x10000,
                             "CTutorialList", 0);
        Class_8045F858 regionHost(&mRegion);

        // getData() reads and clears the handle's buffer pointer.
        void* fileData = mFileHandle0->getData();
        mtl::MemManager::setMemInitFlag(false);

        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(fileData, "arc");

        buildLayout(&mLayout, mAccessor, "mf85_opt01_tutlist.brlyt");
        bindLayoutAnimTransform(mLayout, &mAnim24, mAccessor,
                                "mf85_opt01_tutlist_in.brlan");
        bindLayoutAnimTransform(mLayout, &mAnim28, mAccessor,
                                "mf85_opt01_tutlist_info_in.brlan");

        // Bind the device font onto the layout's root pane. Retail hoists
        // the root-pane fetch (+0x10 raw field) above the font lookup.
        nw4r::lyt::Pane* rootPane = *(nw4r::lyt::Pane**)((char*)mLayout + 0x10);
        u32 fontResult = (u32)static_cast<IDeviceFontInfo*>(
            CDeviceFont::getFontInfo(1, mLayout))->getFont();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

        TutorialList_EnableOpenAnims();
        mLayout->Animate(0);

        char* title = BdatTouchStringCell("MNU_main", "name", 0x24);
        LayoutSetTextBoxFmtValue(mLayout, "txt_btntit", title, 0);

        // Pick the message archive variant from the game-manager mode.
        const char* sel = cf::CfGameManager::isClassicController(-1) != 0
                              ? "fileID_2"
                              : "fileID_1";
        u16 msgId = BdatGetU16ByTableKey("MNU_kyeassign", sel, 0x61);
        char* texName = MakeTplNameSysFile((u32)msgId);
        CTutorialMsgObj* obj =
            (CTutorialMsgObj*)CUICfManager_getArcResourceAccessor()->GetResource(0x74696d67, texName, 0);
        if (obj != 0) {
            PaneSetTexPaletteByName(mLayout, (const char*)((void*)"pic_soatbtn"), (TPLPalette*)obj);
            // Row/col counts are lhz-loaded before the pane lookup; the
            // u16->f32 stores expand to MWCC's 0x4330-magic int-to-double
            // sequence against the .sdata2 pool constant.
            u16 row = obj->chain->pCoords->c2;
            u16 col = obj->chain->pCoords->c0;
            nw4r::lyt::Pane* pane = *(nw4r::lyt::Pane**)((char*)mLayout + 0x10);
            pane = pane->FindPaneByName("pic_soatbtn", true);
            if (pane != 0) {
                CTutorialPaneView* sz = reinterpret_cast<CTutorialPaneView*>(pane);
                struct { f32 width, height; } size = {(f32)row, (f32)col};
                sz->width = size.width;
                sz->height = size.height;
            }
        }

        TutorialList_UpdateTitleText();

        // Rebuild the embedded cursor: assignment from a temporary, then run
        // the cursor layout-init virtual.
        mCursor = CCur18(CUICfManager_getArcResourceAccessor());
        mCursor.initLayout();

        TutorialList_OpenAfterFilesReady();
        mFileHandle0 = 0;
        mRegion.validateHeap();
        return true;
    }
    if (mFileHandle1 == event->mFileHandle) {
        // Shared archive hit: release its buffered data into the BDAT layer.
        void* data2 = mFileHandle1->getData();
        CBdat::setBdatEntry(4, data2);
        Bdat_GetTable_AA34();
        lbl_eu_80664BF0 = (u32)getFP("MNU_ttrl");
        mFileHandle1 = 0;
        TutorialList_OpenAfterFilesReady();
        return true;
    }
    return false;
}

// Play the tutorial voice: gate on the handle's +0x3F00 bit-1 flag, the
// voice-idle virtual (vtable slot 0x2BC), the battle-state mapping and a 0xA0
// handle-buffer allocation, then play voice 0xDD2 through the embedded
// +0x3E9C sub-object. Always returns 0.
extern "C" int TutorialList_PlayTutorialVoice(CVoiceHandle* self) {
    if ((self->field_0x3F00 & 0x2) == 0) return 0;
    if (((CVoiceHandleVTV*)self)->isActive() != 0) return 0;
    if (HpVoice_MapSubState(self) != 5) return 0;
    if (CCharVoiceMan_DispatchById(0xa0, 1) == 0) return 0;
    CCharVoice* voice = (CCharVoice*)self;
    if (self != 0) voice = &self->voice;
    RequestVoicePlayback(voice, 0xdd2, 0xa0);
    return 0;
}
