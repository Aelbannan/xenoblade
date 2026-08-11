// Auto-scaffolded catalog TU for kyoshin/CQstLogList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CQstLogList.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <stdio.h>

u8 CQstLogList::func_80227CCC() { return mSortEnabled; }

// Copies a 0x22-byte quest-log entry: 2 head bytes + 4x8-byte word loop.
// Retail emits an mtctr/bdnz counted loop; under the unit-locked -O4,p this
// shape unrolls no matter the source form (MWCC_REFERENCE walls 6/13) - the
// bottom-test do-while below is the closest surviving loop shape.
extern "C" void func_80227994(CQstLogListEntry* pDst, const CQstLogListEntry* pSrc) {
    pDst->mField0 = pSrc->mField0;
    pDst->mField1 = pSrc->mField1;
    u32* dstWords = (u32*)pDst->mData;
    const u32* srcWords = (const u32*)pSrc->mData;
    u32 i = 4;
    do {
        dstWords[0] = srcWords[0];
        dstWords[1] = srcWords[1];
        dstWords += 2;
        srcWords += 2;
    } while (--i != 0);
}


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
        nw4r::lyt::Pane* root = self->mpLayout->GetRootPane();
        nw4r::lyt::Pane* pane1 = root->FindPaneByName(&lbl_eu_80509AB4[0x33], true);
        nw4r::lyt::Pane* pane2 = root->FindPaneByName(&lbl_eu_80509AB4[0x2a], true);
        nw4r::math::VEC3 pos;
        func_80137924(&pos, pane2, pane1, root);
        func_801D3430(&self->mSortMenuData, &pos);
        func_80228C04(self);
        func_801D216C(&self->mCur18[0], 1);
        func_801D3330(&self->mSortMenuData);
        self->mSortDescending = 1;
        func_80228B10(self);
        func_80138078(2);
    }
}

// Sort-menu confirm/select: rebuilds the quest list (func_80228C98) and
// re-sorts (func_80228B10) when the sort menu reports an active button.
void func_80228280(CQstLogList* self, int flag) {
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

void func_802282F8(){}

// retail: lwz r0,lbl_eu_80664730@sda21; clrlwi r3,r0,16 = (u16)global
extern "C" u32 func_80228394() { extern u32 lbl_eu_80664730; return lbl_eu_80664730 & 0xFFFF; }

void func_802283A0(CQstLogList* self) {
    f32 frame = lbl_eu_80668584;
    nw4r::lyt::AnimTransform* anim = self->mpAnim0;
    if (func_80137444(anim, frame) != 0) {
        self->field_0x174 = 2;
        func_80228544(self);
    }
}

void func_802283EC(CQstLogList* self) {
    f32 frame = lbl_eu_80668584;
    nw4r::lyt::AnimTransform* anim = self->mpAnim1;
    if (func_80137444(anim, frame) != 0) {
        self->field_0x174 = 3;
        func_801D216C(&self->mCur18[0], 1);
        func_80228B10(self);
        self->mSortEnabled = 1;
    }
}

void func_8022844C(CQstLogList* self) {
    if (func_80137510(self->mpAnim1, lbl_eu_80668584) != 0) {
        self->field_0x174 = 5;
        func_802284E4(self);
    }
}

void func_80228498(CQstLogList* self) {
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
                                                    u32 mode, u8 a6, u8 a7) {
    char buf[0x20];
    const char* str = func_8013639C(name, &lbl_eu_80509AB4[0x42], questId);
    sprintf(buf, &lbl_eu_80509AB4[0x48], index);
    func_80136B4C(self->mpLayout, buf, (char*)str, 0);
    u32 color = a6 != 0 ? 0xC81E1EFFu : lbl_eu_80662860;
    func_80137B44(self->mpLayout, buf, color);

    // Detail text: BDAT lookup chain (name -> column A -> column B -> row
    // string) formatted into the second pane.
    u16 a = func_80136254(name, &lbl_eu_80509AB4[0x54], questId);
    u16 b = func_80136254((const char*)lbl_eu_80664098, &lbl_eu_80509AB4[0x5b], a);
    u8 c = func_8013600C(&lbl_eu_80509AB4[0x63], &lbl_eu_80509AB4[0x70], b);
    const char* str2 = func_8013639C(lbl_eu_806640A8, &lbl_eu_80509AB4[0x76], c);
    sprintf(buf, &lbl_eu_80509AB4[0x7b], index);
    func_80136B4C(self->mpLayout, buf, (char*)str2, 0);
    func_80137B44(self->mpLayout, buf, color);

    sprintf(buf, &lbl_eu_80509AB4[0x87], index);
    nw4r::lyt::Pane* pane1 = self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
    const char* str3 = 0;
    if (mode >= 0xFE && mode <= 0xFF) {
        str3 = (const char*)self->mArcResAcc->GetResource(
            0x74696D67u, &lbl_eu_80509AB4[0xa7], 0);
    } else if (mode == 1) {
        str3 = (const char*)self->mArcResAcc->GetResource(
            0x74696D67u, &lbl_eu_80509AB4[0x94], 0);
    }
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
extern "C" __declspec(noinline) void func_802289F8(CQstLogList* self) {
    char buf[0x20];
    for (u8 i = 1; i <= 10; i++) {
        sprintf(buf, &lbl_eu_80509AB4[0x48], i);
        func_80136B4C(self->mpLayout, buf, &lbl_eu_80509AB4[0x106], 0);
        sprintf(buf, &lbl_eu_80509AB4[0x7b], i);
        func_80136B4C(self->mpLayout, buf, &lbl_eu_80509AB4[0x106], 0);
        sprintf(buf, &lbl_eu_80509AB4[0x87], i);
        nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != 0) {
            func_80124270(pane, 0);
        }
        sprintf(buf, &lbl_eu_80509AB4[0xba], i);
        pane = self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
        if (pane != 0) {
            func_80124270(pane, 0);
        }
    }
}

// Scroll-bar/page refresh helper (retail name unmangled - extern "C" keeps
// the bl relocs from the scroll/page handlers bound to the retail symbol).
extern "C" __declspec(noinline) void func_802285A4(CQstLogList* self) {
    // Quest text table entry selected by the two signed index bytes.
    int off = (self->field_0x17B + self->field_0x17C) * 0x22;
    s8 e0 = (s8)lbl_eu_80576670[off];
    u8 e1 = lbl_eu_80576670[off + 1];
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
        u16 questId = entry->f0;
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
    lbl_eu_8066472C = total;
    if (CScrollBar_isVisible(&self->mScrollBar) != 0) {
        func_801F36BC(&self->mScrollBar, 10, (u16)lbl_eu_8066472C);
    }
}

// Rebuild the quest list display: with the sort menu open, copy its selection
// state into the cursor; otherwise format the quest icon pane name, position
// the icon against the proportion pane and move the cursor to it.
extern "C" __declspec(noinline) void func_80228B10(CQstLogList* self) {
    char buf[0x20];
    nw4r::math::VEC3 pos;
    if (self->mSortDescending != 0) {
        nw4r::math::VEC3 tmp;
        func_801D3454(&tmp,
                      reinterpret_cast<CSortMenu*>(&self->mSortMenuData));
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(&tmp);
    } else {
        sprintf(buf, &lbl_eu_80509AB4[0xBA], self->field_0x17D + 1);
        nw4r::lyt::Pane* pane1 =
            self->mpLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 = self->mpLayout->GetRootPane()
                                      ->FindPaneByName(&lbl_eu_80509AB4[0x33], true);
        func_80137924(&pos, pane1, pane2, self->mpLayout->GetRootPane());
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(&pos);
    }
}

extern "C" void func_80228C04(CQstLogList* self) {}

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
    self->mField0 = 0xFF;
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

extern "C" void func_80227AC4() {}

// Draws the layout plus the scroll bar, sort menu and cursor when the
// quest-log layout is loaded (field_0x170).
void func_80227B6C(CQstLogList* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x170 != 0) {
        func_80137038(self->mpLayout, drawInfo, 0, 1);
        func_801F35B0(&self->mScrollBar, drawInfo);
        func_801D31F8(&self->mSortMenuData, drawInfo);
        func_801D20B0(&self->mCur18[0], drawInfo);
    }
}

extern "C" void func_80227BD8() {}
extern "C" void func_80227CDC() {}

// Sort-menu exit callback: when the sort menu is closing (mode 3) switch to
// mode 4, disable sorting and request scroll-bar out.
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

// Quest-log list constructor (retail symbol __ct__CQstLogList, C-linkage
// ctor name). Builds the sub-objects, primes the shared quest text table
// (lbl_eu_80576670) and copies the temp-constructed CSortMenu / QstData
// bodies into the members the same way the retail ctor does.
CQstLogList::CQstLogList(u16 arg2) {
    CScrollBarData tmp;          // temp CScrollBar body (member copy source)
    CQstLogListEntry buf;        // per-entry text buffer
    CQstLogListSortMenuData tmp2;  // temp CSortMenu body (member copy source)
    CQstLogListQstData tmp3;     // temp quest-info buffer (member copy source)

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
    // "%s" string, copied into the shared table.
    for (u8 i = 0; i < 0x20; i++) {
        buf.mField0 = 0xFF;
        buf.mField1 = 0;
        sprintf((char*)&buf.mData[0], &lbl_eu_80509AB4[0x10], 0);
        func_80227994((CQstLogListEntry*)&lbl_eu_80576670[i * 0x22], &buf);
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
    __ct__UnkClass_8011C974(&mSortMenuData.mUnk04[0], &tmp2.mUnk04[0]);
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
    for (int k = 0; k < 16; k++) {
        mSortMenuData.mArray[k * 2] = tmp2.mArray[k * 2];
        mSortMenuData.mArray[k * 2 + 1] = tmp2.mArray[k * 2 + 1];
    }
    mSortMenuData.mCount = tmp2.mCount;
    mSortMenuData.mPage = tmp2.mPage;
    mSortMenuData.mSubPage = tmp2.mSubPage;
    __dt__9CSortMenuFv(&tmp2, -1);

    // Build a temp quest-info buffer and copy the 0x400 records plus the
    // 6-byte tail into the member, then destroy the temp.
    func_80226FAC(&tmp3.mList[0]);
    u32 k2 = 0;
    do {
        copyQstInfo(&mQstData.mList[k2 * 8], &tmp3.mList[k2 * 8]);
        k2++;
    } while (k2 < 0x400);
    mQstData.field_2000 = tmp3.field_2000;
    mQstData.field_2002 = tmp3.field_2002;
    mQstData.field_2004 = tmp3.field_2004;
    mQstData.field_2005 = tmp3.field_2005;
    __destroy_arr(&tmp3, (void*)&__dt__80227030, 8, 0x400);
}
