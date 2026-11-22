#define IWORK_EVENT_INLINE_DTOR
// kyoshin/menu/code_80272AD8 - the award-list widget (CPlayAwardEntry /
// CPlayAwardEntryArray / CPlayAwardList and their PlayAward_* helpers).
//
// MWCC emits a TU's generated code - the static initialiser and the MI
// adjusting thunks - at the END of its .text, so retail's CMenuPlayAward .text
// stops right after [@88@cbRenderBefore / @88@__dt__] at 0x80272AD8. The
// widget that follows (0x80272AD8..0x80274258) is this TU: it owns the
// CPlayAwardList vtable and base list (.data 0x80537C20..0x80537CB8), its
// RTTI record (.sdata 0x806629D0), its "CPlayAwardList" type-name string and
// the string pool that follows it (.rodata 0x8050E7B0..0x8050E958), the
// sdata2 pool from 0x806689A8 and the sPlayAwardBdat scratch word (.sbss).
//
// Only the declarations come from CMenuPlayAward.hpp; the class members are
// defined here.

#include <nw4r/lyt/lyt_drawInfo.h>
#include <stdio.h>   // sprintf (award pane names)
#include "monolib/lib/CLibLayout.hpp"
#include <revolution/gx/GXPixel.h>  // GXSetZMode

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_80135FDC.hpp" // AnimRewindFrame / advanceAnimTransform (anim frame advance)
#include "kyoshin/cf/CfGameManager.hpp" // cf::CfGameManager::getCurrentPad / getCfPadData
#include "monolib/core/CPadManager.hpp" // CPad::mPressedButtonFlags
#include "kyoshin/menu/CMenuPlayAward.hpp"
#include "monolib/work/CEventFile.hpp" // CEventFile (event->mFileHandle in OnFileEvent)
#include "monolib/device/CDeviceFile.hpp"  // CDeviceFile::readFile / readCommonArchiveFile
#include "monolib/device/CDeviceVI.hpp"       // CDeviceVI::waitForDrawDone
#include "monolib/util/MemManager.hpp"        // mtl::MemManager::allocate
#include "monolib/work/CProcess.hpp"          // CProcess::Regist
#include "monolib/work/CWorkThreadSystem.hpp" // CWorkThreadSystem::getWorkMem
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

CPlayAwardEntry::CPlayAwardEntry() {
    word0 = 0;
    word1 = 0;
}

CPlayAwardEntry::~CPlayAwardEntry() {}

// Entry-array constructor: the 512 award entries are constructed by the
// compiler (array helper), then the trailing state fields are initialised.
CPlayAwardEntryArray::CPlayAwardEntryArray() {
    field_1000 = 0;
    field_1001 = 0;
    field_1004 = 0.0f;
    mPageIndex = 0;
    field_100C = 0;
    field_100E = 0;
    field_1008[0] = 0;
    field_1008[1] = 0;
}

// Rebuild the award-list entry array from the BDAT award table: count the
// present award types, sort their indices by sort key, then append each
// award's text binding to the page indexed by its type key. Skips entirely
// when the array was already built (field_1000 nonzero).
void PlayAward_BuildEntries(CPlayAwardEntryArray* self) {
    if (self->field_1000 != 0) {
        return;
    }

    const char* const s = "JNL_playaward";
    void* bdat = sPlayAwardBdat;
    void* fp = getFP(s);
    self->field_1001 = (u8)Bdat_GetMaxRow_B1EC(fp);

    // Count the award types present in the BDAT award table (row 0x312c +
    // (i % 200)); the total goes into field_1000.
    for (u8 i = 1; i <= self->field_1001; i++) {
        if (CtrlRemote_TouchBitByArg(0x312c + (u8)(i % 200)) != 0) {
            self->field_1000++;
        }
    }

    // Page-progress ratio: present / total, scaled.
    self->field_1004 =
        (float)self->field_1000 / (float)self->field_1001 * 100.0f;

    CPlayAwardEntry entry;
    u8 buf[0x100];
    u8 count = self->field_1001;
    // Fill the sort buffer with the award indices 1..count.
    for (u8 j = 0; j < count; j++) {
        buf[j] = j + 1;
    }

    // Bubble-sort the indices by their BDAT sort key (column +0xe).
    for (u8 outer = 0; outer < self->field_1001 - 1; outer++) {
        int swapped = 0;
        for (u8 inner = 0; inner < (self->field_1001 - 1) - outer; inner++) {
            u8 a = buf[inner];
            u8 b = buf[inner + 1];
            if (BdatGetU8Direct((const void*)bdat, "order", a) >
                BdatGetU8Direct((const void*)bdat, "order", b)) {
                buf[inner] = b;
                buf[inner + 1] = a;
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }

    // Build the per-page award entries: bind the text for each award and
    // append it to the page indexed by its type key.
    char* def = BdatTouchStringCell("MNU_update", "info", 0x10);
    for (u8 k = 0; k < count; k++) {
        u8 val = buf[k];
        u8 page = (u8)(BdatGetU8Direct((const void*)bdat, "genre", val) - 1);
        int has = CtrlRemote_TouchBitByArg(0x312c + val % 200) != 0;

        u8 key2 = BdatGetU8Direct((const void*)bdat, "com1_terms1", val);
        u16 key3 = BdatGetU16Direct(bdat, "com1_terms2", val);
        int flag = 0;
        if (key2 <= self->field_1000 && key3 <= CtrlRemote_TouchBitByArg(0x20)) {
            flag = 1;
        }

        char* s1;
        char* s2;
        if (has != 0) {
            s1 = BdatGetPtrDirect(fp, "title", val);
            s2 = BdatGetPtrDirect(bdat, "comment2", val);
        } else if (flag != 0) {
            s1 = def;
            s2 = BdatGetPtrDirect(bdat, "comment1", val);
        } else {
            s1 = def;
            s2 = def;
        }

        entry.word0 = (u32)s1;
        entry.word1 = (u32)s2;
        u8 cnt = self->field_1008[page];
        self->field_1008[page] = cnt + 1;
        PlayAward_CopyEntry(&self->mEntries[page * 0x200 + cnt], &entry);

        // Remember the scroll position for the entry the player is on.
        u8 cur = code80135FDC_getByte_6407F();
        if (cur != 0 && cur == val) {
            self->mPageIndex = page;
            s16 c = (s16)(self->field_1008[page] - 1);
            self->field_100C = c;
            self->field_100E = 0;
            if (c >= 9) {
                self->field_100E = (s16)(c - 8);
                self->field_100C = 8;
            }
        }
    }
}

// Copy an 8-byte award entry (load both words before storing, like entries[i] = src).
void PlayAward_CopyEntry(CPlayAwardEntry* dst, const CPlayAwardEntry* src) { *dst = *src; }

// Entry lookup: array base + (pageIndex << 11) + ((param & 0xFF) << 3);
// returns 0 when param >= 0x100.
u8* PlayAward_FindEntry(CPlayAwardEntryArray* self, int param) {
    if (param >= 256) return 0;
    return (u8*)&self->mEntries[0] + (self->mPageIndex << 11) + ((param & 0xFF) << 3);
}

CPlayAwardList::CPlayAwardList(int)
    : mFileHandle(0), mFileHandle2(0), mArcAccessor1C(0), mLayout20(0), mAnimTrans24(0),
      mAnimTrans28(0), field_0x2C(0), mCursor(0), mScrollBar(0), field_0x88(0), field_0x89(0),
      field_0x8A(0), field_0x8B(1), field_0x8C(0), field_0x8E(0), field_0x90(0), mEntryArray() {
    mScrollBar = CScrollBar(5);
}

CPlayAwardList::~CPlayAwardList() {}

// Load the award-list layout arc files: the list arc via readFile (handle from
// MEM2) and the common archive via readCommonArchiveFile (handle re-queried by
// KyoshinHeap_GetField44). Then build the scroll bar layout and clear the flag byte.
void PlayAward_LoadLayouts(CPlayAwardList* self) {
    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/PlayAwardList.arc", self, 0, 0);
    self->mFileHandle2 = CDeviceFile::readCommonArchiveFile(
        KyoshinHeap_GetField44(), "/common/jp/bdat_menu_award.bin", self, 0, 0);
    self->mScrollBar.loadLayoutArc();
    self->field_0x8A = 0;
}

// Per-frame award-list move: when the list is loaded (field_0x88), run the
// state-transition handler for the current state byte, advance the layout
// animation, and update the cursor + scroll bar.
void PlayAward_UpdateList(CPlayAwardList* self) {
    if (self->field_0x88 != 0) {
        switch (self->field_0x89) {
        case 1:
            PlayAward_FinishOpenAnimA(self);
            break;
        case 2:
            PlayAward_FinishOpenAnimB(self);
            break;
        case 4:
            PlayAward_FinishCloseAnimA(self);
            break;
        case 5:
            PlayAward_FinishCloseAnimB(self);
            break;
        }
        self->mLayout20->Animate(0);
        self->mCursor.Update();
        self->mScrollBar.UpdateDispatch();
    }
}

// Draw the award-list layout, scroll bar and cursor when the list is loaded
// (field_0x88 nonzero).
void PlayAward_DrawList(CPlayAwardList* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x88 != 0) {
        drawLayout(self->mLayout20, drawInfo, 0, 1);
        self->mScrollBar.draw(drawInfo);
        self->mCursor.DrawLayout(drawInfo);
    }
}

// Release the award-list widgets: close the two file handles, destroy the
// layout + 0x2C object, release the arc resource accessor, then run the
// cursor/scrollbar/region teardown helpers.
void PlayAward_ReleaseList(CPlayAwardList* self) {
    closeFileHandle(&self->mFileHandle);
    closeFileHandle(&self->mFileHandle2);
    self->field_0x88 = 0;
    if (self->mLayout20 != 0) {
        delete self->mLayout20;
        self->mLayout20 = 0;
    }
    if (self->field_0x2C != 0) {
        delete reinterpret_cast<CPlayAwardAttrObj*>(self->field_0x2C);
        self->field_0x2C = 0;
    }
    releaseArcResourceAccessor(self->mArcAccessor1C);
    self->mCursor.cleanup();
    self->mScrollBar.Teardown();
    self->mMemRegion.deleteRegion();
}


// Award list "ready to show" query: visible when the embedded scroll bar is
// visible AND the flag byte at 0x8A is set. Int return so callers compare
// with cmpwi directly (cf. CMenuTutorialList.hpp).
int PlayAward_IsListReady(CPlayAwardList* self) {
    if (self->mScrollBar.isVisible()) {
        return self->field_0x8A;
    }
    return 0;
}

int PlayAward_IsReadyFlag(CPlayAwardList* self) { return self->field_0x8B; }

void PlayAward_RequestOpen(CPlayAwardList* self) {
    if (self->field_0x89 != 0) return;
    self->field_0x89 = 1;
    self->field_0x8B = 0;
    PlayAward_BindCloseAnims(self);

    // Scroll the bar in with the 3-float init vector.
    float vec[3];
    vec[0] = 252.0f;
    vec[1] = 88.0f;
    vec[2] = 0.0f;
    self->mScrollBar.InitRootPane(reinterpret_cast<const nw4r::math::VEC3*>(vec));

    self->mScrollBar.UpdateThumb(9, self->mEntryArray.field_1008[self->mEntryArray.mPageIndex]);

    // Restore the scroll position from the entry-array state (reads as u16,
    // stored into the s16 fields; the >= 9 clamp compares the sign-extended
    // original).
    u16 c = (u16)self->mEntryArray.field_100C;
    u16 e = (u16)self->mEntryArray.field_100E;
    self->field_0x8C = (s16)c;
    self->field_0x8E = (s16)e;
    if ((s16)c >= 9) self->field_0x8C = 8;
    if (self->field_0x8C < 0) self->field_0x8C = 0;
    if (self->field_0x8E < 0) self->field_0x8E = 0;

    self->field_0x90 = self->mEntryArray.mPageIndex;
    self->mScrollBar.PlaceThumb(self->field_0x8E);
    PlayAward_RefreshPageTitles(self);
    PlayAward_RefreshEntryPanes(self);
    PlayAward_RefreshCursor(self);
}

// Award-list close request (retail PlayAward_RequestClose): when the list is in its
// "open" state (3), mark it closing (4), reset the ready flag, stop the open
// animations, hide the cursor, scroll the bar out and play the close sound.
void PlayAward_RequestClose(CPlayAwardList* self) {
    if (self->field_0x89 == 3) {
        self->field_0x89 = 4;
        self->field_0x8B = 0;
        PlayAward_BindOpenAnims(self);
        self->mCursor.SetVisible(0);
        self->mScrollBar.requestScrollOut();
        playUISound(6);
    }
}

// Award-list cursor down: decrement the scroll position, and when it wraps
// past the bottom of the page, move to the previous page (9-entry rows,
// 8-entry remainder page). Refresh the list + scroll bar afterwards.
void PlayAward_MoveCursorUp(CPlayAwardList* self) {
    u8 page = self->mEntryArray.mPageIndex;
    u8 count = self->mEntryArray.field_1008[page];
    self->field_0x8C = self->field_0x8C - 1;
    if (self->field_0x8C < 0) {
        self->field_0x8C = 0;
        self->field_0x8E = self->field_0x8E - 1;
        if (self->field_0x8E < 0) {
            if (count >= 9) {
                self->field_0x8C = 8;
                self->field_0x8E = count - 9;
            } else {
                s16 c = count - 1;
                self->field_0x8C = c;
                self->field_0x8E = 0;
                if (c < 0) {
                    self->field_0x8C = 0;
                }
            }
        }
    }
    PlayAward_RefreshEntryPanes(self);
    PlayAward_RefreshCursor(self);
    self->mScrollBar.PlaceThumb(self->field_0x8E);
    playUISound(1);
}

// Award-list cursor down: advance the scroll position, wrapping through the
// 9-entry page / 8-entry remainder pages. Refresh the list + scroll bar.
void PlayAward_MoveCursorDown(CPlayAwardList* self) {
    u8 count = self->mEntryArray.field_1008[self->mEntryArray.mPageIndex];
    s16 off;
    int delta;
    if (count >= 9) {
        s16 cur = self->field_0x8C + 1;
        self->field_0x8C = cur;
        if (cur >= 9) {
            self->field_0x8C = 8;
            delta = count - 9;
            off = (s16)(self->field_0x8E + 1);
            self->field_0x8E = off;
            if (off > delta) {
                self->field_0x8C = 0;
                self->field_0x8E = 0;
            }
        }
    } else {
        s16 cur = self->field_0x8C + 1;
        self->field_0x8C = cur;
        if (cur >= count) {
            self->field_0x8C = 0;
            self->field_0x8E = 0;
        }
    }
    PlayAward_RefreshEntryPanes(self);
    PlayAward_RefreshCursor(self);
    self->mScrollBar.PlaceThumb(self->field_0x8E);
    playUISound(1);
}

// Award-list scroll step: when the current page's entry count reaches 9,
// decrement the scroll offset by 9 (wrapping through the 8-entry remainder),
// otherwise reset both scroll fields. Then refresh the list and scroll bar.
void PlayAward_ScrollPageUp(CPlayAwardList* self) {
    if (self->mEntryArray.field_1008[self->mEntryArray.mPageIndex] >= 9) {
        s16 scroll = self->field_0x8E - 9;
        self->field_0x8E = scroll;
        if (scroll < 0) {
            self->field_0x8C = scroll + 8;
            self->field_0x8E = 0;
            if ((s16)(scroll + 8) < 0) {
                self->field_0x8C = 0;
            }
        }
    } else {
        self->field_0x8C = 0;
        self->field_0x8E = 0;
    }
    PlayAward_RefreshEntryPanes(self);
    PlayAward_RefreshCursor(self);
    self->mScrollBar.PlaceThumb(self->field_0x8E);
    playUISound(1);
}

// Award-list cursor up: when the current page's entry count reaches 9,
// advance the scroll offset by 9 (wrapping through the 8-entry remainder),
// otherwise reset both scroll fields. Refresh the list + scroll bar.
void PlayAward_ScrollPageDown(CPlayAwardList* self) {
    u8 count = self->mEntryArray.field_1008[self->mEntryArray.mPageIndex];
    if (count >= 9) {
        int delta = count - 9;
        s16 off = self->field_0x8E + 9;
        self->field_0x8E = off;
        if (off > delta) {
            s16 diff = off - delta;
            self->field_0x8C = diff;
            self->field_0x8E = delta;
            if (diff >= 9) {
                self->field_0x8C = 8;
            }
        }
    } else {
        self->field_0x8C = count - 1;
        self->field_0x8E = 0;
        if (self->field_0x8C < 0) {
            self->field_0x8C = 0;
        }
    }
    PlayAward_RefreshEntryPanes(self);
    PlayAward_RefreshCursor(self);
    self->mScrollBar.PlaceThumb(self->field_0x8E);
    playUISound(1);
}

// Page-flip: increment the 0x90 page byte (wrapping 2 -> 0), stash the page
// index for entry lookup, then re-range the scroll bar and refresh the list.
void PlayAward_FlipPage(CPlayAwardList* self) {
    self->field_0x90++;
    if (self->field_0x90 >= 2) {
        self->field_0x90 = 0;
    }
    self->field_0x8C = 0;
    self->field_0x8E = 0;
    u8 page = (u8)self->field_0x90;
    if (page < 2) {
        self->mEntryArray.mPageIndex = page;
    }
    u8 count = self->mEntryArray.field_1008[self->mEntryArray.mPageIndex];
    self->mScrollBar.UpdateThumb(9, count);
    self->mScrollBar.PlaceThumb(self->field_0x8E);
    PlayAward_RefreshPageTitles(self);
    PlayAward_RefreshEntryPanes(self);
    PlayAward_RefreshCursor(self);
    playUISound(0xa);
}

// Page-flip helper: 0x67 when the 0x90 page byte is nonzero, else 0x66.
s32 PlayAward_GetPageCue(CPlayAwardList* self) { return (self->field_0x90 != 0) + 0x66; }

// Award list open transition: once the list's forward animation reaches its
// end, mark the list state as showing (2), run the page/scroll setup and
// request the scroll bar scroll-in.
void PlayAward_FinishOpenAnimA(CPlayAwardList* self) {
    if (advanceAnimTransform(self->mAnimTrans24, 1.0f) != 0) {
        self->field_0x89 = 2;
        PlayAward_BindOpenAnims(self);
        self->mScrollBar.requestScrollIn();
    }
}

// Award list open transition: once the list's forward animation reaches its
// end, mark the list state as opening (3), run the page/scroll setup and set
// the ready flag.
void PlayAward_FinishOpenAnimB(CPlayAwardList* self) {
    if (advanceAnimTransform(self->mAnimTrans28, 1.0f) != 0) {
        self->field_0x89 = 3;
        PlayAward_RefreshCursor(self);
        self->field_0x8B = 1;
    }
}

// Award list close transition: once the list's reverse animation reaches its
// start, mark the list state as closed (5) and run the close sequence.
void PlayAward_FinishCloseAnimA(CPlayAwardList* self) {
    if (AnimRewindFrame(self->mAnimTrans28, 1.0f) != 0) {
        self->field_0x89 = 5;
        PlayAward_BindCloseAnims(self);
    }
}

// Award list close transition (reverse): when the list's reverse animation
// reaches its start, reset the state (0) and set the ready flag (1).
void PlayAward_FinishCloseAnimB(CPlayAwardList* self) {
    if (AnimRewindFrame(self->mAnimTrans24, 1.0f) != 0) {
        self->field_0x89 = 0;
        self->field_0x8B = 1;
    }
}

// Award list close transition: bind the reverse/forward animation transforms
// to the layout (anim28 disabled, anim24 enabled).
void PlayAward_BindCloseAnims(CPlayAwardList* self) {
    self->mLayout20->SetAnimationEnable(self->mAnimTrans28, false);
    self->mLayout20->SetAnimationEnable(self->mAnimTrans24, true);
}

// Award list open transition: bind the forward/reverse animation transforms
// to the layout (anim24 disabled, anim28 enabled).
void PlayAward_BindOpenAnims(CPlayAwardList* self) {
    self->mLayout20->SetAnimationEnable(self->mAnimTrans24, false);
    self->mLayout20->SetAnimationEnable(self->mAnimTrans28, true);
}

// Refresh the 9 award-entry panes of the current page: format each pane name
// and bind the entry words into the layout (first text white, second text in
// the field_0x2C attribute color).
void PlayAward_RefreshEntryPanes(CPlayAwardList* self) {
    char buf[0x20];
    for (u8 i = 0; i < 9; i++) {
        CPlayAwardEntry* entry = reinterpret_cast<CPlayAwardEntry*>(
            PlayAward_FindEntry(&self->mEntryArray, (u8)(i + self->field_0x8E)));
        sprintf(buf, "txt_tit%02d", i + 1);
        LayoutSetTextBoxFmtValue(self->mLayout20, buf, reinterpret_cast<char*>(entry->word0), 0);
        sprintf(buf, "txt_exp%02d", i + 1);
        LayoutSetTextBoxFmtValue(self->mLayout20, buf, reinterpret_cast<char*>(entry->word1),
                      self->field_0x2C);
    }
}

// Refresh the cursor position: format the current pane name, show the cursor,
// find the two anchor panes, translate the cursor's target position and move
// the cursor to it (retail reloads the root pane each time).
void PlayAward_RefreshCursor(CPlayAwardList* self) {
    nw4r::math::VEC3 pos;
    char buf[0x20];
    sprintf(buf, "txt_tit%02d", self->field_0x8C + 1);
    self->mCursor.SetVisible(1);
    nw4r::lyt::Pane* pane1 = self->mLayout20->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = self->mLayout20->GetRootPane()->FindPaneByName("nul_proportion", true);
    ScalePaneWorldPos(&pos, pane1, pane2, self->mLayout20->GetRootPane());
    self->mCursor.setRootPaneTranslate(&pos);
}

void PlayAward_RefreshPageTitles(CPlayAwardList* self) {
    // Load the two page-title strings ("%s"-style templates from the pool).
    char* s1;
    char* s2;
    void* fp = getFP("MNU_update");
    s1 = BdatGetPtrDirect(fp, "info", 0xa);
    s2 = BdatGetPtrDirect(fp, "info", 0xb);
    LayoutSetTextBoxFmtValue(self->mLayout20, "txt_cat00",
                  self->field_0x90 != 0 ? s2 : s1, 0);

    // Resolve the award title and the two per-page help strings, then look up
    // the 'timg' texture for the page and size its pane from the row/col.
    char* s5;
    char* s4 = BdatTouchStringCell("MNU_kyeassign", "help", 0x8a);
    s5 = BdatTouchStringCell("MNU_kyeassign", "help", 0x89);
    const char* sel = cf::CfGameManager::isClassicController(-1) != 0
                          ? "fileID_2"
                          : "fileID_1";
    u16 msgId = BdatGetU16ByTableKey("MNU_kyeassign", sel, 0x3a);
    char* texName = MakeTplNameSysFile((u32)msgId);
    nw4r::lyt::ArcResourceAccessor* mgr = CUICfManager_getArcResourceAccessor();
    CPlayAwardMsgObj* obj = reinterpret_cast<CPlayAwardMsgObj*>(
        mgr->GetResource(0x74696D67, texName, 0));
    if (obj != 0) {
        PaneSetTexPaletteByName(self->mLayout20, "pic_soatbtn", (TPLPalette*)obj);
        CPlayAwardCoords* coords = obj->chain->pCoords;
        u16 row = coords->row;
        u16 col = coords->col;
        nw4r::lyt::Pane* pane = self->mLayout20->GetRootPane()->FindPaneByName(
            "pic_soatbtn", true);
        if (pane != 0) {
            float src[2];
            src[0] = (float)row;
            src[1] = (float)col;
            reinterpret_cast<CPlayAwardPaneSize*>(pane)->width = src[0];
            reinterpret_cast<CPlayAwardPaneSize*>(pane)->height = src[1];
        }
    }

    // Bind the page title/help strings (help string differs by page).
    LayoutSetTextBoxFmtValue(self->mLayout20, "txt_btn",
                  self->field_0x90 == 0 ? s5 : s4, 0);
    char* s6 = BdatTouchStringCell("MNU_battle", "name", 0x19);
    LayoutSetTextBoxFmtValue(self->mLayout20, "txt_btntit", s6, 0);
}

// Award-list load handler: when the layout arc accessor is ready and the
// common archive has finished loading, reset the entry array and rebuild it,
// then refresh the list.
void PlayAward_LoadEntries(CPlayAwardList* self) {
    if (self->mArcAccessor1C != 0 && self->mFileHandle2 == 0) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    self->field_0x8A = 1;
    self->field_0x88 = 1;
    self->mEntryArray = CPlayAwardEntryArray();
    PlayAward_BuildEntries(&self->mEntryArray);
    PlayAward_RefreshPageTitles(self);
    PlayAward_RefreshEntryPanes(self);
}

// File-load completion callback: when the award-list arc (mFileHandle) is
// ready, build the whole layout (region scratch guard, tag processor, arc
// accessor, layout + anim transforms, font binding, text bindings, award
// panes, cursor); when the common archive (mFileHandle2) is ready, detach its
// buffer into the BDAT tables and refresh. Returns 1 when handled.
bool CPlayAwardList::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
        // Scratch heap region guard around the layout build.
        u8 regionBuf[8];
        char buf[0x20];
        mMemRegion.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x10000,
                                "CPlayAwardList", 0);
        __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, &mMemRegion);

        // Detach the loaded file buffer and hand it to the layout accessor.
        void* fileData = mFileHandle->getData();
        setMemInitFlag__Q23mtl10MemManagerFb(false);

        // Allocate the tag processor (0x858 bytes, layout memory pool).
        void* tagProc = mtl::MemManager::allocate(
            0x858, CLibLayout::getAllocHandle());
        if (tagProc != 0) {
            tagProc = __ct__CTagProcessor(tagProc);
        }
        field_0x2C = (u32)tagProc;

        // Build the layout + forward/reverse anim transforms from the arc.
        mArcAccessor1C = (nw4r::lyt::ArcResourceAccessor*)
            CLibLayout::createArcResourceAccessor();
        mArcAccessor1C->Attach(fileData, "arc");
        buildLayout(&mLayout20, mArcAccessor1C, "mf31_award00.brlyt");
        bindLayoutAnimTransform(mLayout20, &mAnimTrans24, mArcAccessor1C,
                      "mf31_award00_in.brlan");
        bindLayoutAnimTransform(mLayout20, &mAnimTrans28, mArcAccessor1C,
                      "mf31_award00_info_in.brlan");

        // Bind the shared font into the layout's root pane.
        nw4r::lyt::Pane* rootPane = mLayout20->GetRootPane();
        CPlayAwardFontObj* fontObj = (CPlayAwardFontObj*)
            getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout20);
        u32 fontResult = fontObj->getFontHandle();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

        setLayoutTextBoxFont(mLayout20, "txt_val01", reinterpret_cast<u32>(CUICfManager_getPackedFont9C()));
        PlayAward_BindCloseAnims(this);
        mLayout20->Animate(0);

        // Bind the three page-title/help strings.
        void* fp = getFP("MNU_update");
        LayoutSetTextBoxFmtValue(mLayout20, "txt_cat00",
                      BdatGetPtrDirect(fp, "info", 0xa), 0);
        LayoutSetTextBoxFmtValue(mLayout20, "txt_cat01",
                      BdatGetPtrDirect(fp, "info", 0xb), 0);
        LayoutSetTextBoxFmtValue(mLayout20, "txt_val01",
                      BdatGetPtrDirect(fp, "info", 0xd), 0);

        // Bind the eight award panes and stash the tag processor on each.
        for (u8 i = 1; i <= 9; i++) {
            sprintf(buf, "txt_exp%02d", i);
            nw4r::lyt::Pane* pane =
                mLayout20->GetRootPane()->FindPaneByName(buf, true);
            reinterpret_cast<CPlayAwardPaneUser*>(pane)->field_F8 = field_0x2C;
        }

        mCursor = CCur18(CUICfManager_getArcResourceAccessor());
        mCursor.initLayout();

        // Refresh the list, release the file handle and the scratch region.
        PlayAward_LoadEntries(this);
        mFileHandle = 0;
        mMemRegion.validateHeap();
        __dt__14Class_8045F858Fv(regionBuf, -1);
        return 1;
    }
    if (mFileHandle2 == event->mFileHandle) {
        // Common archive finished: detach its buffer into the BDAT tables.
        void* fileData2 = mFileHandle2->getData();
        CBdat::setBdatEntry(2, fileData2);
        Bdat_GetTable_AA34();
        sPlayAwardBdat = getFP("MNU_playaward");
        mFileHandle2 = 0;
        PlayAward_LoadEntries(this);
        return 1;
    }
    return 0;
}

// name-guess: loaded award BDAT table pointer set from getFP("MNU_playaward")
void* sPlayAwardBdat = nullptr;
