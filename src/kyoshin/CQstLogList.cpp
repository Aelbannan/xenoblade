// Auto-scaffolded catalog TU for kyoshin/CQstLogList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CQstLogList.hpp"
// CfObjectImplMove.hpp (via harness_catalog.hpp) declares lbl_eu_80526324 as
// const char[]; code_80135FDC.hpp's u8[] decl of the same name clashes and
// this TU never uses it - rename it away for this one include.
#define lbl_eu_80526324 lbl_eu_80526324_unused_u8_decl
#define lbl_eu_80526300 lbl_eu_80526300_unused_u8_decl
#include "kyoshin/code_80135FDC.hpp"
#undef lbl_eu_80526324
#include "monolib/work/CEventFile.hpp"

#include <stdio.h>

u8 CQstLogList::func_80227CCC() { return mSortEnabled; }

// Copies a 0x22-byte quest-log entry: 2 head bytes + 4x8-byte word loop.
// Struct assignment reproduces retail exactly (MWCC emits the counted loop
// with lwzu/stwu update addressing); optimize_for_size suppresses the
// -O4,p unroll (MWCC_CASES line 226).
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) CQstLogListEntry* func_80227994(CQstLogListEntry* pDst, const CQstLogListEntry* pSrc) {
    *pDst = *pSrc;
    return pDst;
}
#pragma pop


u8 CQstLogList::func_80227CD4() { return mSortDescending; }

// Returns the quest-list's own "layout loaded" byte only when the scroll bar
// AND the sort menu are both visible (mirrors CSortMenu::func_801D32DC's
// scrollbar-gate pattern).
u8 func_80227C70(CQstLogList* self) {
    if (CScrollBar_isVisible(&self->mScrollBar) == 0) {
        return 0;
    }
    if (func_801D32DC(reinterpret_cast<CSortMenu*>(&self->mSortMenuData)) == 0) {
        return 0;
    }
    return self->field_0x178;
}


// Quest-log scroll down. With the sort menu open, scroll down through its
// options instead; otherwise advance the row index (field_0x17D), wrapping
// through the scroll position (field_0x17E) against the shared boundary
// (lbl_eu_8066472C).
void func_80227EC8(CQstLogList* self) {
    if (self->mSortDescending != 0) {
        func_801D3698(reinterpret_cast<CSortMenu*>(&self->mSortMenuData));
        func_80228B10(self);
    } else {
        if (lbl_eu_8066472C >= 10) {
            u8 d = self->field_0x17D + 1;
            self->field_0x17D = (s8)d;
            if ((s8)d >= 10) {
                s16 e = self->field_0x17E + 1;
                self->field_0x17D = 9;
                self->field_0x17E = e;
                if (e > lbl_eu_8066472C - 10) {
                    self->field_0x17D = 0;
                    self->field_0x17E = 0;
                }
            }
        } else {
            u8 d = self->field_0x17D + 1;
            self->field_0x17D = (s8)d;
            if ((s8)d >= lbl_eu_8066472C) {
                self->field_0x17D = 0;
                self->field_0x17E = 0;
            }
        }
        func_802285A4(self);
        func_80228B10(self);
        func_801F3850(&self->mScrollBar, (u16)self->field_0x17E);
    }
    func_80138078(1);
}

// Quest-log page up. With the sort menu open, page up through its options;
// otherwise step the scroll position (field_0x17E) back by a page of 10.
void func_80227FC0(CQstLogList* self) {
    if (self->mSortDescending != 0) {
        func_801D3724(reinterpret_cast<CSortMenu*>(&self->mSortMenuData));
        func_80228B10(self);
    } else {
        if (lbl_eu_8066472C >= 10) {
            s16 v = self->field_0x17E - 10;
            self->field_0x17E = v;
            if (v < 0) {
                self->field_0x17D = (s8)(v + 9);
                self->field_0x17E = 0;
                if ((s8)self->field_0x17D < 0) {
                    self->field_0x17D = 0;
                }
            }
        } else {
            self->field_0x17D = 0;
            self->field_0x17E = 0;
        }
        func_802285A4(self);
        func_80228B10(self);
        func_801F3850(&self->mScrollBar, (u16)self->field_0x17E);
    }
    func_80138078(1);
}

// Quest-log page down. With the sort menu open, page down through its
// options; otherwise step the scroll position forward by a page of 10.
void func_8022807C(CQstLogList* self) {
    if (self->mSortDescending != 0) {
        func_801D377C(reinterpret_cast<CSortMenu*>(&self->mSortMenuData));
        func_80228B10(self);
    } else {
        if (lbl_eu_8066472C >= 10) {
            s16 v = self->field_0x17E + 10;
            self->field_0x17E = v;
            if (v > lbl_eu_8066472C - 10) {
                u8 b = (u8)(v - (lbl_eu_8066472C - 10));
                self->field_0x17D = (s8)b;
                self->field_0x17E = lbl_eu_8066472C - 10;
                if ((s8)b >= 10) {
                    self->field_0x17D = 9;
                }
            }
        } else {
            self->field_0x17D = (s8)(lbl_eu_8066472C - 1);
            self->field_0x17E = 0;
            if ((s8)self->field_0x17D < 0) {
                self->field_0x17D = 0;
            }
        }
        func_802285A4(self);
        func_80228B10(self);
        func_801F3850(&self->mScrollBar, (u16)self->field_0x17E);
    }
    func_80138078(1);
}

extern "C" void func_80228C98(CQstLogList* self);
extern "C" void func_80228C04(CQstLogList* self);

// Quest-log sort toggle. With the sort menu active, closing the menu (current
// direction descending) resets the direction and rebuilds; otherwise the
// sort-menu cursor is positioned under the list and the direction is enabled.
// pragma optimize_for_size: retail saves r28-r31 via stmw/lmw.
#pragma push
#pragma optimize_for_size on
void func_80228164(CQstLogList* self) {
    if (func_801D3328(&self->mSortMenuData) == 0) {
        return;
    }
    if (self->mSortDescending != 0) {
        func_801D216C(&self->mCur18[0], 1);
        func_801D3408(&self->mSortMenuData);
        self->mSortDescending = 0;
        func_80228C98(self);
        func_80228B10(self);
        func_80138078(6);
    } else {
        // Four live values here (self/pool base/root/pane1) drive the
        // retail r28-r31 allocation.
        // Nested-call argument form (right-to-left eval): pane1 is
        // evaluated first and must survive the second FindPaneByName call,
        // reproducing the retail r29/r30/r31 assignment.
        nw4r::math::VEC3 pos;
        char* const s = lbl_eu_80509AB4;
        nw4r::lyt::Pane* root = self->mpLayout->GetRootPane();
        func_80137924(&pos, root->FindPaneByName(&s[0x2a], true),
                      root->FindPaneByName(&s[0x33], true), root);
        func_801D3430(&self->mSortMenuData, &pos);
        func_80228C04(self);
        func_801D216C(&self->mCur18[0], 1);
        func_801D3330(&self->mSortMenuData);
        self->mSortDescending = 1;
        func_80228B10(self);
        func_80138078(2);
    }
}
#pragma pop

// Sort-menu confirm/select: rebuilds the quest list (func_80228C98) and
// re-sorts (func_80228B10) when the sort menu reports an active button.
// noinline: retail keeps this an out-of-line call from func_802282F8.
// pragma optimize_for_size: retail saves r30-r31 via stmw/lmw.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void func_80228280(CQstLogList* self, int flag) {
    if (func_801D3328(&self->mSortMenuData) != 0) {
        func_801D216C(&self->mCur18[0], 1);
        func_801D3408(&self->mSortMenuData);
        self->mSortDescending = 0;
        func_80228C98(self);
        func_80228B10(self);
        if (flag == 0) {
            func_80138078(6);
        }
    }
}
#pragma pop

// retail: lwz r0,lbl_eu_80664730@sda21; clrlwi r3,r0,16 = (u16)global
extern "C" u32 func_80228394() { extern u32 lbl_eu_80664730; return lbl_eu_80664730 & 0xFFFF; }

extern "C" __declspec(noinline) void func_802283A0(CQstLogList* self) {
    f32 frame = lbl_eu_80668584;
    nw4r::lyt::AnimTransform* anim = self->mpAnim0;
    if (func_80137444(anim, frame) != 0) {
        self->field_0x174 = 2;
        func_80228544(self);
    }
}

extern "C" __declspec(noinline) void func_802283EC(CQstLogList* self) {
    f32 frame = lbl_eu_80668584;
    nw4r::lyt::AnimTransform* anim = self->mpAnim1;
    if (func_80137444(anim, frame) != 0) {
        self->field_0x174 = 3;
        func_801D216C(&self->mCur18[0], 1);
        func_80228B10(self);
        self->mSortEnabled = 1;
    }
}

extern "C" __declspec(noinline) void func_8022844C(CQstLogList* self) {
    if (func_80137510(self->mpAnim1, lbl_eu_80668584) != 0) {
        self->field_0x174 = 5;
        func_802284E4(self);
    }
}

extern "C" __declspec(noinline) void func_80228498(CQstLogList* self) {
    if (func_80137510(self->mpAnim0, lbl_eu_80668584) != 0) {
        self->field_0x174 = 0;
        self->mSortEnabled = 1;
    }
}

extern "C" __declspec(noinline) void func_802284E4(CQstLogList* self) {
    self->mpLayout->SetAnimationEnable(self->mpAnim1, false);
    self->mpLayout->SetAnimationEnable(self->mpAnim0, true);
}

extern "C" __declspec(noinline) void func_80228544(CQstLogList* self) {
    self->mpLayout->SetAnimationEnable(self->mpAnim0, false);
    self->mpLayout->SetAnimationEnable(self->mpAnim1, true);
}

// One quest-row display update: name pane, detail-pane text chain and the
// icon pane. mode selects the icon resource key (0xFE/0xFF = unused-row
// state, 1 = the active row's texture); a6 picks the color pool; a7 picks
// the icon pane's resource key.
extern "C" __declspec(noinline) void func_802286F4(CQstLogList* self,
                                                    const char* name,
                                                    int questId, int index,
                                                    int mode, u8 a6, u8 a7) {
    char buf[0x20];
    const char* str = func_8013639C(name, &lbl_eu_80509AB4[0x42], questId);
    sprintf(buf, &lbl_eu_80509AB4[0x48], index);
    func_80136B4C(self->mpLayout, buf, (char*)str, 0);
    func_80137B44(self->mpLayout, buf,
                  a6 != 0 ? 0xC81E1EFFu : lbl_eu_80662860);

    // Detail text: BDAT lookup chain (name -> column A -> column B -> row
    // string) formatted into the second pane.
    u16 a = func_80136254(name, &lbl_eu_80509AB4[0x54], questId);
    u16 b = func_80136254((const char*)lbl_eu_80664098, &lbl_eu_80509AB4[0x5b], a);
    u8 c = func_8013600C(&lbl_eu_80509AB4[0x63], &lbl_eu_80509AB4[0x70], b);
    const char* str2 = func_8013639C(lbl_eu_806640A8, &lbl_eu_80509AB4[0x76], c);
    sprintf(buf, &lbl_eu_80509AB4[0x7b], index);
    func_80136B4C(self->mpLayout, buf, (char*)str2, 0);
    func_80137B44(self->mpLayout, buf,
                  a6 != 0 ? 0xC81E1EFFu : lbl_eu_80662860);

    sprintf(buf, &lbl_eu_80509AB4[0x87], index);
    nw4r::lyt::Pane* pane1 = self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
    const char* str3 = 0;
    // Row-state icon: mode 1 = active-row texture, 0xFE/0xFF = unused-row
    // state. Goto-chain keeps the mode==1 body in the fall-through like
    // retail (if-else-if emits bne-skips with the wrong block order).
    if ((u32)(mode - 0xFE) <= 1) goto modeFE;
    if (mode == 1) goto modeOne;
    goto iconDone;
modeOne:
    str3 = (const char*)self->mArcResAcc->GetResource(
        0x74696D67u, &lbl_eu_80509AB4[0x94], 0);
    goto iconDone;
modeFE:
    str3 = (const char*)self->mArcResAcc->GetResource(
        0x74696D67u, &lbl_eu_80509AB4[0xa7], 0);
iconDone:
    if (str3 != 0) {
        func_80137F88(pane1, str3);
        func_80124270(pane1, 1);
    }

    sprintf(buf, &lbl_eu_80509AB4[0xba], index);
    nw4r::lyt::Pane* pane2 = self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
    const char* str4 = 0;
    switch (a7) {
    case 0:
        str4 = (const char*)self->mArcResAcc->GetResource(
            0x74696D67u, &lbl_eu_80509AB4[0xca], 0);
        break;
    case 1:
        str4 = (const char*)self->mArcResAcc->GetResource(
            0x74696D67u, &lbl_eu_80509AB4[0xde], 0);
        break;
    case 3:
        str4 = (const char*)self->mArcResAcc->GetResource(
            0x74696D67u, &lbl_eu_80509AB4[0xf2], 0);
        break;
    default:
        break;
    }
    if (str4 != 0) {
        func_80137F88(pane2, str4);
        func_80124270(pane2, 1);
    }
}

// Resets the ten quest-row panes: formats each pane name, clears its text
// and hides empty panes.
// pragma optimize_for_size: retail saves r29-r31 via stmw/lmw; plain -O4,p
// emits reversed individual stw's (MWCC_CASES CPartyState/CTagProcessor).
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802289F8(CQstLogList* self) {
    char buf[0x20];
    // No explicit pool-base local: MWCC CSEs the lbl_eu_80509AB4 base into
    // r31 itself (lis hoisted above the LR store, addi sunk to first use),
    // alongside self (r29) and counter (r30) -> retail stmw frame.
    // Counter is u32 with explicit u8 casts at the vararg call sites
    // (retail truncates only there).
    u32 i = 1;
    do {
        sprintf(buf, &lbl_eu_80509AB4[0x48], (u8)i);
        func_80136B4C(self->mpLayout, buf, &lbl_eu_80509AB4[0x106], 0);
        sprintf(buf, &lbl_eu_80509AB4[0x7b], (u8)i);
        func_80136B4C(self->mpLayout, buf, &lbl_eu_80509AB4[0x106], 0);
        sprintf(buf, &lbl_eu_80509AB4[0x87], (u8)i);
        nw4r::lyt::Pane* pane =
            self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            func_80124270(pane, 0);
        }
        sprintf(buf, &lbl_eu_80509AB4[0xba], (u8)i);
        pane = self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != NULL) {
            func_80124270(pane, 0);
        }
    } while (++i <= 10);
}
#pragma pop

// Scroll-bar/page refresh helper (retail name unmangled - extern "C" keeps
// the bl relocs from the scroll/page handlers bound to the retail symbol).
extern "C" __declspec(noinline) void func_802285A4(CQstLogList* self) {
    // Quest text table entry selected by the two signed index bytes.
    // Declaration order matters here: MWCC pre-colors the callee-saved
    // homes for row/e1/questId/e0 in this order, matching retail.
    const char* row;
    u8 e1;
    u32 questId;
    s32 e0;
    s32 sum = self->field_0x17B + self->field_0x17C;
    int off = sum * 0x22;
    row = &lbl_eu_80576670[off];
    e0 = (s8)lbl_eu_80576670[off];
    e1 = row[1];
    func_802289F8(self);
    lbl_eu_8066472C = 0;
    func_80227260(&self->mQstData.mList[0], e1, (u8)e0);

    // Rebuild the visible rows from the quest-info buffer, starting at the
    // current scroll position; stop after ten rows or the list end.
    int i = 0;
    u16 total = selectQstIndex(&self->mQstData.mList[0]);
    u32 cur = (u16)self->field_0x17E;
    for (; (u16)cur < total; i++, cur++) {
        if (i >= 10) break;
        CQstLogListQstInfo* entry = func_802276F4(&self->mQstData.mList[0], (u16)cur);
        questId = entry->f0;
        u32 idx = func_80138138(questId);
        u8 res = (u8)func_8009CF8C(questId + 0x220);
        func_802286F4(self, (const char*)lbl_eu_80573D18[idx], questId,
                      (u8)(i + 1), res, entry->f5, entry->f6);
        if (i == self->field_0x17D) {
            lbl_eu_80664730 = questId;
            lbl_eu_80664734 = idx;
            lbl_eu_80664738 = res;
        }
    }
    lbl_eu_8066472C = (u16)total;
    if (CScrollBar_isVisible(&self->mScrollBar)) {
        func_801F36BC(&self->mScrollBar, 10, (u16)lbl_eu_8066472C);
    }
}

// Rebuild the quest list display: with the sort menu open, copy its selection
// state into the cursor; otherwise format the quest icon pane name, position
// the icon against the proportion pane and move the cursor to it.
// Locals are declared buf/pos/tmp so the stack slots land at the retail
// offsets (buf 0x20, pos 0x14, tmp 0x08) keeping r29/r30/r31 live (stmw).
// pragma optimize_for_size: retail saves r29-r31 via stmw/lmw.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80228B10(CQstLogList* self) {
    char buf[0x20];
    nw4r::math::VEC3 pos;
    nw4r::math::VEC3 tmp;
    if (self->mSortDescending != 0) {
        func_801D3454(&tmp,
                      reinterpret_cast<CSortMenu*>(&self->mSortMenuData));
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(&tmp);
    } else {
        char* const s = lbl_eu_80509AB4;
        sprintf(buf, &s[0xBA], self->field_0x17D + 1);
        nw4r::lyt::Pane* pane1 =
            self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 = self->mpLayout->GetRootPane()
                                      ->FindPaneByName(&s[0x33], true);
        func_80137924(&pos, pane1, pane2, self->mpLayout->GetRootPane());
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(&pos);
    }
}
#pragma pop

// Re-populate the embedded sort menu's array from the shared quest text table
// (one entry per logged quest) and select the current page.
// noinline: retail keeps this an out-of-line call from func_80228164.
// pragma optimize_for_size: retail saves r29-r31 via stmw/lmw.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void func_80228C04(CQstLogList* self) {
    if (func_801D32DC(reinterpret_cast<CSortMenu*>(&self->mSortMenuData)) == 0) {
        return;
    }
    func_801D350C(&self->mSortMenuData);
    for (u8 i = 0; i < (int)lbl_eu_80664728; i++) {
        func_801D3518(&self->mSortMenuData, &lbl_eu_80576670[i * 0x22] + 2);
    }
    func_801D353C(&self->mSortMenuData,
                  (u8)(self->field_0x17B + self->field_0x17C));
}
#pragma pop

// Rebuild the quest list display from the sort-menu/quest state: formats the
// quest text pane name and loads the entry text (func_80136A1C).
extern "C" __declspec(noinline) void func_80228C98(CQstLogList* self) {
    func_80136A1C(self->mpLayout, &lbl_eu_80509AB4[0x107],
                  &lbl_eu_80576670[(self->field_0x17B + self->field_0x17C) * 0x22] + 2, 0);
}

extern "C" void OnFileEvent__12CQstLogListFv() {}

// Constructor for the 0x22-byte quest-log entry records; called as the array
// element ctor by sinit_80229378 via __construct_array. Retail symbol is
// unmangled (C linkage) - extern "C" keeps the sinit reloc bound to the
// retail name, and the entry ctor returns self (retail `mr r3, r31`).
extern "C" CQstLogListEntry* func_80229398(CQstLogListEntry* self) {
    self->mField0 = -1;  // retail li r5,-1 (signed constant); 0xFF would emit li r5,255
    self->mField1 = 0x00;
    // "%s" (lbl_eu_80509AB4 + 0x10) with a null pointer - clears the text.
    sprintf((char*)&self->mData[0], &lbl_eu_80509AB4[0x10], 0);
    return self;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void __construct_array(void*, void*, int, int, int);
extern "C" void sinit_80229378() {
    __construct_array(lbl_eu_80576670, (void*)func_80229398, 0, 0x22, 0x20);
}

// Loads the quest-log arc file (lbl_eu_80509AB4+0x13) into the mem2 handle,
// then primes the scroll bar and sort menu layouts.
void func_80227A60(CQstLogList* self) {
    self->mFileHandle = (CFileHandle*)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_80509AB4[0x13], self, 0, 0);
    func_801F34F4(&self->mScrollBar);
    func_801D3064(&self->mSortMenuData);
    self->field_0x178 = 0;
}

// File-load completion callback: builds the quest-log layout from the
// freshly-loaded arc, primes the shared quest-text table with the four fixed
// rows plus every quest type present in the BDAT quest table (sorted by its
// sort key), then sizes the quest panes from the 'timg' message resource.
int CQstLogList::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
    // Local declaration order fixes the retail stack frame: region guard
    // 0x0C, cursor temp 0x18, five 0x24-stride entry buffers 0x30..0xE2 and
    // the quest-type index array at 0xE8.
    u8 regionBuf[8];           // 0x0C - RAII scratch-region guard storage
    u8 tmpCur[0x18];           // 0x18 - stack-constructed cursor
    CQstLogListEntry rowSort;  // 0x30 - entry buffer reused for sorted rows
    CQstLogListEntry row3;     // 0x54
    CQstLogListEntry row2;     // 0x78
    CQstLogListEntry row1;     // 0x9C
    CQstLogListEntry row0;     // 0xC0
    u8 indices[0x3C];          // 0xE8 - collected quest types

    // Scratch heap region (RAII Class_8045F858 guard), then detach the file
    // buffer and attach it to the layout arc resource accessor. The handle
    // is fetched before the string-pool base is materialized (retail order).
    void* mem2 = getHandleMEM2__Q23mtl10MemManagerFv();
    char* const s = lbl_eu_80509AB4;
    createRegion__17UnkClass_8045F564FiiPCci(
        &mUnk04[0], (int)mem2, 0x10000, &s[0x112], 0);
    __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, &mUnk04[0]);

    // The detached file-data pointer is loaded inline at the Attach call
    // site so it never gets a stack slot.
    setMemInitFlag__Q23mtl10MemManagerFb(false);
    mArcResAcc =
        (nw4r::lyt::ArcResourceAccessor*)createArcResourceAccessor__10CLibLayoutFv();
    mArcResAcc->Attach(mFileHandle->getData(), &s[0x11e]);

    func_80136E84(&mpLayout, mArcResAcc, &s[0x122]);
    func_80136F08(mpLayout, &mpAnim0, mArcResAcc, &s[0x135]);
    func_80136F08(mpLayout, &mpAnim1, mArcResAcc, &s[0x14b]);

    // Bind the font handle into the layout's root pane.
    nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
    void* fontObj =
        getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout);
    u32 fontResult = ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
    func_8013676C(rootPane, fontResult);

    func_802284E4(this);
    mpLayout->Animate(0);

    // Build the cursor on the stack, copy its body into the member region
    // (skipping the +0x00 vtable pointer) and destroy the temp.
    __ct__CCur18(tmpCur, func_801355F4());
    CCur18Data* curDst = reinterpret_cast<CCur18Data*>(&mCur18[0]);
    CCur18Data* curSrc = reinterpret_cast<CCur18Data*>(tmpCur);
    curDst->field_4 = curSrc->field_4;
    curDst->field_8 = curSrc->field_8;
    curDst->field_C = curSrc->field_C;
    curDst->field_10 = curSrc->field_10;
    curDst->field_14 = curSrc->field_14;
    curDst->field_15 = curSrc->field_15;
    __dt__6CCur18Fv(tmpCur, -1);
    reinterpret_cast<CCur18View*>(&mCur18[0])->vf02();

    // Prime the four fixed quest-log rows (0xFF/0x00 head bytes + formatted
    // text), appending each entry to the shared quest-text table. The
    // buffers are declared in reverse use order so the stack slots land at
    // the retail offsets (b4 lowest, b0 highest).
    // Prime the four fixed quest-log rows (0xFF head byte + formatted text),
    // appending each entry to the shared quest-text table.
    row0.mField0 = 0xFF;
    row0.mField1 = 0;
    sprintf((char*)&row0.mData[0], &s[0x10],
            func_80136190(&s[0x166], &s[0x76], 0x44));
    char* const table = lbl_eu_80576670;
    u32 n0 = lbl_eu_80664728;
    lbl_eu_80664728 = n0 + 1;
    func_80227994((CQstLogListEntry*)&table[n0 * 0x22], &row0);

    row1.mField0 = 0xFF;
    row1.mField1 = 1;
    sprintf((char*)&row1.mData[0], &s[0x10],
            func_80136190(&s[0x166], &s[0x76], 0x45));
    u32 n1 = lbl_eu_80664728;
    lbl_eu_80664728 = n1 + 1;
    func_80227994((CQstLogListEntry*)&table[n1 * 0x22], &row1);

    row2.mField0 = 0xFF;
    row2.mField1 = 2;
    sprintf((char*)&row2.mData[0], &s[0x10],
            func_80136190(&s[0x166], &s[0x76], 0x46));
    u32 n2 = lbl_eu_80664728;
    lbl_eu_80664728 = n2 + 1;
    func_80227994((CQstLogListEntry*)&table[n2 * 0x22], &row2);

    row3.mField0 = 0xFF;
    row3.mField1 = 3;
    sprintf((char*)&row3.mData[0], &s[0x10],
            func_80136190(&s[0x166], &s[0x76], 0x47));
    u32 n3 = lbl_eu_80664728;
    lbl_eu_80664728 = n3 + 1;
    func_80227994((CQstLogListEntry*)&table[n3 * 0x22], &row3);

    // Collect every quest type (2..0x1c) that has at least one active,
    // uncompleted quest in the BDAT quest table. Types 6..0x1c dispatch
    // through the retail jump table into the shared quest scan below.
    s32 numQuests = (s32)func_8003B1EC(lbl_eu_806640A0);
    u8 count = 0;
    for (u8 i = 2; i <= 0x1c; i++) {
        switch (i) {
        case 6: case 7: case 8: case 9: case 0xA: case 0xB: case 0xC: case 0xD:
        case 0xE: case 0xF: case 0x10: case 0x11: case 0x12: case 0x13: case 0x14:
        case 0x15: case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A: case 0x1B:
        case 0x1C:
            for (s32 j = 1; j <= numQuests; j++) {
                if (func_801361E8((u32)lbl_eu_806640A0, &s[0x70], j) == i &&
                    func_801361E8((u32)lbl_eu_806640A0, &s[0x170], j) == 0 &&
                    func_8009CF8C((u32)(j + 0x20c8)) != 0) {
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
            if (func_801361E8((u32)lbl_eu_806640A8, &s[0xa], a) >
                func_801361E8((u32)lbl_eu_806640A8, &s[0xa], b)) {
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
        char* text = func_8013639C(lbl_eu_806640A8, &s[0x76], idx);
        rowSort.mField0 = idx;
        rowSort.mField1 = 4;
        sprintf((char*)&rowSort.mData[0], &s[0x10], text);
        u32 nk = lbl_eu_80664728;
        lbl_eu_80664728 = nk + 1;
        func_80227994((CQstLogListEntry*)&table[nk * 0x22], &rowSort);
    }

    // Refresh the sort-menu header text and rebuild the list display.
    char* t = func_80136190(&s[0x166], &s[0x76], 2);
    func_80136B4C(mpLayout, &s[0x179], t, 0);
    func_80228C98(this);

    // Look up the quest-log message texture ('timg') and size the panes from
    // its row/column counts.
    const char* sel = isClassicController__Q22cf13CfGameManagerFv(-1) != 0
                          ? &s[0x185]
                          : &s[0x18e];
    u16 msgId = func_8013606C(&s[0x197], sel, 0x61);
    char* texName = func_80138F78((u32)msgId);
    CQstLogListMsgObj* obj = (CQstLogListMsgObj*)func_801355F4()->GetResource(
        0x74696D67, texName, 0);
    if (obj != 0) {
        func_80137E7C(mpLayout, &s[0x1a5], obj);
        CQstLogListCoords* coords = obj->chain->pCoords;
        nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName(
            &s[0x1a5], true);
        if (pane != 0) {
            reinterpret_cast<PaneSizeRegion*>(pane)->width = coords->c2;
            reinterpret_cast<PaneSizeRegion*>(pane)->height = coords->c0;
        }
    }

    // Store the quest text color fetched from the log pane.
    lbl_eu_80662860 = mpLayout->GetRootPane()
                          ->FindPaneByName(&s[0x1b1], true)
                          ->GetVtxColor(0);

    func_802270CC(&mQstData.mList[0]);
    func_802285A4(this);

    // Position the selection on the quest-log's own list entry.
    u16 listIdx = func_80227710(&mQstData.mList[0], field_0x180);
    if (listIdx < 10) {
        field_0x17D = (s8)listIdx;
        field_0x17E = 0;
    } else {
        field_0x17D = 9;
        field_0x17E = (s16)(listIdx - 9);
    }

    func_802285A4(this);
    field_0x178 = 1;
    field_0x170 = 1;
    mFileHandle = 0;
    validateHeap__17UnkClass_8045F564Fv(&mUnk04[0]);
    __dt__14Class_8045F858Fv(regionBuf, -1);
    return 1;
    }
    return 0;
}

// Draws the layout plus the scroll bar, sort menu and cursor when the
// quest-log layout is loaded (field_0x170).
// pragma optimize_for_size: retail saves r30-r31 via stmw/lmw.
#pragma push
#pragma optimize_for_size on
void func_80227B6C(CQstLogList* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x170 != 0) {
        func_80137038(self->mpLayout, drawInfo, 0, 1);
        func_801F35B0(&self->mScrollBar, drawInfo);
        func_801D31F8(&self->mSortMenuData, drawInfo);
        func_801D20B0(&self->mCur18[0], drawInfo);
    }
}
#pragma pop

// Unload the quest-log screen: close the arc file handle, delete the layout,
// release the resource accessor and tear down cursor/scroll bar/sort menu.
extern "C" void func_80227BD8(CQstLogList* self) {
    func_801390E0(&self->mFileHandle);
    self->field_0x170 = 0;
    if (self->mpLayout != 0) {
        delete self->mpLayout;
        self->mpLayout = 0;
    }
    func_80139124(self->mArcResAcc);
    reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf03();
    func_801F35DC(&self->mScrollBar);
    func_801D3258(&self->mSortMenuData);
    deleteRegion__17UnkClass_8045F564Fv(&self->mUnk04[0]);
}

// Sort-menu open-in animation start (one-shot on mode 0): switch animations,
// prime the scroll bar geometry and request scroll-in.
extern "C" void func_80227CDC(CQstLogList* self) {
    if (self->field_0x174 == 0) {
        self->field_0x174 = 1;
        self->mSortEnabled = 0;
        func_802284E4(self);
        func_80228B10(self);
        f32 pos[3];
        pos[0] = lbl_eu_80668578;
        pos[1] = lbl_eu_8066857C;
        pos[2] = lbl_eu_80668580;
        func_801F3670(&self->mScrollBar, pos);
        func_801F36BC(&self->mScrollBar, 10, (u16)lbl_eu_8066472C);
        func_801F3850(&self->mScrollBar, (u16)self->field_0x17E);
        func_801F367C(&self->mScrollBar);
    }
}

// Sort-menu confirm: adopt the selected page/entry indices into the list,
// reset the row cursor and rebuild; otherwise clear sorting once entries exist.
extern "C" void func_802282F8(CQstLogList* self) {
    if (self->mSortDescending != 0) {
        if (func_801D3328(&self->mSortMenuData) == 0) {
            return;
        }
        self->field_0x17B = func_801D3808(&self->mSortMenuData);
        self->field_0x17C = func_801D3810(&self->mSortMenuData);
        self->field_0x17D = 0;
        self->field_0x17E = 0;
        func_80228280(self, 1);
        func_802285A4(self);
    } else {
        if (lbl_eu_8066472C > 0) {
            self->mSortEnabled = 0;
        }
    }
    func_80138078(3);
}

// Per-frame update while the quest-log list is loaded: run the active open/
// close animation step, then advance layout, cursor, scroll bar and sort menu.
extern "C" void func_80227AC4(CQstLogList* self) {
    if (self->field_0x170 != 0) {
        switch (self->field_0x174) {
        case 1:
            func_802283A0(self);
            break;
        case 2:
            func_802283EC(self);
            break;
        case 4:
            func_8022844C(self);
            break;
        case 5:
            func_80228498(self);
            break;
        }
        self->mpLayout->Animate(0);
        func_801D202C(&self->mCur18[0]);
        func_801F3540(&self->mScrollBar);
        func_801D3160(&self->mSortMenuData);
    }
}

// Sort-menu exit callback: when the sort menu is closing (mode 3) switch to
// mode 4, disable sorting and request scroll-bar out.
// pragma optimize_for_size: retail saves r30-r31 (self + flag) via stmw/lmw.
#pragma push
#pragma optimize_for_size on
void func_80227D78(CQstLogList* self, int flag) {
    if (self->field_0x174 == 3) {
        self->field_0x174 = 4;
        self->mSortEnabled = 0;
        func_80228544(self);
        func_801D216C(&self->mCur18[0], 0);
        func_801F369C(&self->mScrollBar);
        if (flag != 0) {
            func_80138078(6);
        }
    }
}
#pragma pop

// Quest-log scroll up. With the sort menu open, scroll up through its
// options; otherwise step the row index (field_0x17D) back, wrapping through
// the scroll position (field_0x17E) against the shared boundary.
void func_80227DE8(CQstLogList* self) {
    if (self->mSortDescending != 0) {
        func_801D3620(reinterpret_cast<CSortMenu*>(&self->mSortMenuData));
        func_80228B10(self);
    } else {
        u8 d = self->field_0x17D - 1;
        self->field_0x17D = (s8)d;
        if ((s8)d < 0) {
            self->field_0x17D = 0;
            self->field_0x17E -= 1;
            if (self->field_0x17E < 0) {
                if (lbl_eu_8066472C >= 10) {
                    self->field_0x17D = 9;
                    self->field_0x17E = lbl_eu_8066472C - 10;
                } else {
                    u8 g = (u8)(lbl_eu_8066472C - 1);
                    self->field_0x17D = (s8)g;
                    self->field_0x17E = 0;
                    if ((s8)g < 0) {
                        self->field_0x17D = 0;
                    }
                }
            }
        }
        func_802285A4(self);
        func_80228B10(self);
        func_801F3850(&self->mScrollBar, (u16)self->field_0x17E);
    }
    func_80138078(1);
}

#pragma push
#pragma optimize_for_size on
CQstLogList::~CQstLogList() {
    // 8x0x400 quest-info array; the element dtor is the C-style
    // __dt__80227030 (defined in CMenuQstCnt.cpp). The addic-style null guard
    // reproduces the retail member-array destruction check.
    u8* arr = &mQstData.mList[0];
    if (arr != 0) {
        __destroy_arr(arr, (void*)&__dt__80227030, 8, 0x400);
    }
    // Sub-objects destroyed in reverse declaration order via retail imports
    // (opaque buffers here - CSortMenu.hpp conflicts with CScrollBar.hpp's
    // UnkClass ctor stub, so the real classes cannot be embedded).
    __dt__9CSortMenuFv(&mSortMenuData, -1);
    __dt__10CScrollBarFv(&mScrollBar, -1);
    __dt__6CCur18Fv(&mCur18[0], -1);
    __dt__17UnkClass_8045F564Fv(&mUnk04[0], -1);
}
#pragma pop

// Quest-log list constructor (retail symbol __ct__CQstLogList, C-linkage
// ctor name). Builds the sub-objects, primes the shared quest text table
// (lbl_eu_80576670) and copies the temp-constructed CSortMenu / QstData
// bodies into the members the same way the retail ctor does.
// pragma optimize_for_size: retail keeps the sort-menu region copy rolled
// (mtctr/bdnz); -O4,p fully unrolls it.
#pragma push
#pragma optimize_for_size on
CQstLogList::CQstLogList(u16 arg2) {
    // Declaration order sets the retail stack slots: tmp3 0x160, tmp2 0x70,
    // buf 0x48, tmp 0x08 (frame 0x2180).
    CQstLogListQstData tmp3;       // temp quest-info buffer (member copy source)
    CQstLogListSortMenuData tmp2;  // temp CSortMenu body (member copy source)
    CQstLogListEntry buf;          // per-entry text buffer
    CScrollBarData tmp;            // temp CScrollBar body (member copy source)
    const u8* pSrc;                // temp QstInfo copy cursor
    u8* pDst;                      // member QstInfo copy cursor

    mVtable = (u32)lbl_eu_80536288;
    __ct__17UnkClass_8045F564Fv(&mUnk04[0]);
    mFileHandle = 0;
    mArcResAcc = 0;
    mpLayout = 0;
    mpAnim0 = 0;
    mpAnim1 = 0;
    __ct__CCur18(&mCur18[0], 0);
    __ct__CScrollBar(&mScrollBar, 0);
    __ct__CSortMenu(&mSortMenuData);
    field_0x170 = 0;
    field_0x174 = 0;
    field_0x178 = 0;
    mSortEnabled = 1;
    mSortDescending = 0;
    field_0x17B = 0;
    field_0x17C = 0;
    field_0x17D = 0;
    field_0x17E = 0;
    field_0x180 = arg2;
    func_80226FAC(&mQstData.mList[0]);

    // Prime the 32 quest text entries: 0xFF/0x00 head bytes + a cleared
    // "%s" string, copied into the shared table. Counter is u32 truncated
    // at the multiply (retail clrlwi) with an unsigned loop compare.
    for (u32 tbl_i = 0; tbl_i < 0x20; tbl_i++) {
        buf.mField0 = -1;
        buf.mField1 = 0;
        sprintf((char*)&buf.mData[0], &lbl_eu_80509AB4[0x10], 0);
        func_80227994(
            (CQstLogListEntry*)&lbl_eu_80576670[(u8)tbl_i * 0x22], &buf);
    }

    lbl_eu_80664728 = 0;

    // Build a temp CScrollBar (flag 1) and copy its body into the member.
    __ct__CScrollBar(&tmp, 1);
    func_8011C998(&mScrollBar, &tmp);
    __dt__10CScrollBarFv(&tmp, -1);

    // Build a temp CSortMenu and copy its body into the member, skipping the
    // vtable at +0x0 (UnkClass region via the copy helper, then the pointer
    // fields, flags, scrollbar, array and tail bytes).
    __ct__CSortMenu(&tmp2);
    __ct__UnkClass_8011C974(&mSortMenuData.mUnk04[0], (const u32*)&tmp2.mUnk04[0]);
    mSortMenuData.mFileHandle = tmp2.mFileHandle;
    mSortMenuData.mArcResAcc = tmp2.mArcResAcc;
    mSortMenuData.mpLayout = tmp2.mpLayout;
    mSortMenuData.mpAnimTrans0 = tmp2.mpAnimTrans0;
    mSortMenuData.mpAnimTrans1 = tmp2.mpAnimTrans1;
    mSortMenuData.field_0x28 = tmp2.field_0x28;
    mSortMenuData.field_0x29 = tmp2.field_0x29;
    mSortMenuData.field_0x2A = tmp2.field_0x2A;
    mSortMenuData.field_0x2B = tmp2.field_0x2B;
    func_8011C998(&mSortMenuData.mScrollBar, &tmp2.mScrollBar);
    // 0x80-byte raw region copy (sort-menu +0x68..+0xE8: pad word, array,
    // count/page/subpage).
    {
        // 0x80-byte block struct assign: MWCC emits its canonical
        // lwz/lwzu + stw/stwu mtctr loop for whole-struct copies.
        struct SortMenuRegion {
            u8 bytes[0x80];
        };
        *reinterpret_cast<SortMenuRegion*>(
            reinterpret_cast<char*>(&mSortMenuData) + 0x6C) =
            *reinterpret_cast<const SortMenuRegion*>(
                reinterpret_cast<const char*>(&tmp2) + 0x6C);
    }
    mSortMenuData.mCount = tmp2.mCount;
    mSortMenuData.mPage = tmp2.mPage;
    mSortMenuData.mSubPage = tmp2.mSubPage;
    __dt__9CSortMenuFv(&tmp2, -1);

    // Build a temp quest-info buffer and copy the 0x400 records plus the
    // 6-byte tail into the member, then destroy the temp.
    func_80226FAC(&tmp3.mList[0]);
    // 0x400-record copy; retail compares the running member pointer against
    // the member-buffer end (cmplw), not a counter.
    {
        pDst = &mQstData.mList[0];
        pSrc = &tmp3.mList[0];
        // Bottom-checked loop; end address hoisted by the compiler.
        do {
            copyQstInfo(pDst, pSrc);
            pDst += 8;
            pSrc += 8;
        } while (pDst < &mQstData.mList[8 * 0x400]);
    }
    mQstData.field_2000 = tmp3.field_2000;
    mQstData.field_2002 = tmp3.field_2002;
    mQstData.field_2004 = tmp3.field_2004;
    mQstData.field_2005 = tmp3.field_2005;
    __destroy_arr(&tmp3, (void*)&__dt__80227030, 8, 0x400);
}
#pragma pop
