#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CCollepedia
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CtrlObjectParamSymbols.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/CCollepedia.hpp"
#include <revolution/tpl/TPL.h>
#include "nw4r/lyt.h"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "kyoshin/CTagProcessor.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CItemImpl.hpp"

// name-guess: 30-entry BDAT category row lookup table owned by this TU.
static const u16 sClpPageCategoryTable[30] = {
    0x0000, 0x0000, 0x0001, 0x0008, 0x000F, 0x0016, 0x001A, 0x0020, 0x0027, 0x002E, 0x0000, 0x0032, 0x0036, 0x003C, 0x0000, 0x0040, 0x0047, 0x004E, 0x0054, 0x0058, 0x0000, 0x005F, 0x0000, 0x0066, 0x006D, 0x0074, 0x0078, 0x0000, 0x0000, 0x0000
};

// C-linkage imports (functions + data from other TUs) live in CCollepedia.hpp
// (see the "C-linkage imports" section at the end of that header).

// These functions are declared in code_80135FDC.hpp

int sprintf(char*, const char*, ...);

// Forward declarations for stubs defined at end of file
extern "C" void clpPrevPageIx(CLPCatalog*);
extern "C" u8 clpGetPageCat(CLPCatalog*);
extern "C" u32 func_80254144(CLPCatalog*);
extern "C" void clpRefreshPage(CCollepedia*);
extern "C" void clpSetSelTxt(CCollepedia*);
extern "C" void clpRefreshCur(CCollepedia*);
extern "C" void clpNextCatPg(CCollepedia*);
extern "C" void clpPrevCatPg(CCollepedia*);
// Forward declarations for functions defined before target callers
extern "C" void clpPgOpenDone(CLPPageInfo*);
extern "C" void clpPgCloseEnd(CLPPageInfo*);
extern "C" void clpRebindAnim(CCLPCur*);
extern "C" u16 func_80254204(CLPCatalog*, u32, u32);
extern "C" u8 clpGetCellCnt(CLPCatalog*, u32, u32);
extern "C" void clpUpdateProg(CLPCatalog*);
extern "C" void clpEnterBrows(CCollepedia*);
extern "C" void clpWaitOutro(CCollepedia*);
extern "C" void clpEnterIdle(CCollepedia*);
extern "C" void clpEnabDetCur(CCollepedia*);
extern "C" void clpEnterDetail(CCollepedia*);
extern "C" void collepediaCreateItemMulti(CCollepedia*);
extern "C" void clpOpenComplete(CCollepedia*);
extern "C" void clpPlayTblSe(CCollepedia*);


// C-linkage declarations for functions from code_80135FDC.cpp to avoid reloc name drift
// (provided by code_80135FDC.hpp in the current tree)

CCLPCur::CCLPCur(nw4r::lyt::ArcResourceAccessor* pAccessor) {
    mArcResAcc = pAccessor;
    mpLayout = nullptr;
    mpAnimTrans0 = nullptr;
    mpAnimTrans1 = nullptr;
    mActive = 0;
    mVisible = 0;
}

CCLPCur::~CCLPCur() {}

// us-80255440
// Load the page-arrow cursor layout, bind its loop animation, position panes
void CCLPCur::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, "mf00_reg00_curs09.brlyt");
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc,
        "mf00_reg00_curs09_roop.brlan");

    // Set up position for first pane
    float vec1[3];
    code80135FDC_setVec3((nw4r::math::VEC3*)vec1, lbl_eu_806687F0, lbl_eu_806687F4, lbl_eu_806687F8);

    // Find first pane and copy position
    nw4r::lyt::Pane* pane1 = mpLayout->GetRootPane()->FindPaneByName(
        "pic_pgarw_r", true);
    copyVEC3((void*)&pane1->GetTranslate(), vec1);

    // Set up position for second pane
    float vec2[3];
    code80135FDC_setVec3((nw4r::math::VEC3*)vec2, lbl_eu_806687FC, lbl_eu_806687F4, lbl_eu_806687F8);

    // Find second pane and copy position
    nw4r::lyt::Pane* pane2 = mpLayout->GetRootPane()->FindPaneByName(
        "pic_pgarw_l", true);
    copyVEC3((void*)&pane2->GetTranslate(), vec2);

    // Call UnbindAllAnimation, then bind specific ones
    mpLayout->UnbindAllAnimation();

    clpRebindAnim(this);
}

extern "C" void clpCurAnimate(CCLPCur* this_) {
    if (this_->mpLayout == nullptr) return;
    if (this_->mVisible == 0) {
        advanceAnimTransform(this_->mpAnimTrans0, lbl_eu_80668800);
    }
    this_->mpLayout->Animate(0);
}

// us-80257984
// Check if animation at field_3C is done, then disable/enable animations and set state
extern "C" void clpWaitIntro(CCollepedia* this_) {
    if (advanceAnimTransform(this_->field_3C, lbl_eu_80668800) != 0) {
        this_->field_38->SetAnimationEnable(this_->field_44, false);
        this_->field_38->SetAnimationEnable(this_->field_3C, false);
        this_->field_38->SetAnimationEnable(this_->field_40, true);
        this_->field_49 = 2;
    }
}

// Render layout if active and non-null
extern "C" void clpCurDrawLyt(CCLPCur* this_, nw4r::lyt::DrawInfo* drawInfo) {
    if (this_->mpLayout == nullptr) return;
    if (this_->mActive == 0) return;
    drawLayout(this_->mpLayout, drawInfo, 0, 1);
}

extern "C" void clpCurCleanup(CCLPCur* this_) {
    this_->mActive = 0;
    this_->mpAnimTrans0 = nullptr;
    this_->mpAnimTrans1 = nullptr;
    if (this_->mpLayout != nullptr) {
        delete this_->mpLayout;
        this_->mpLayout = nullptr;
    }
}

// us-80255630
// Unbind animation, bind new animation, enable animation, set frame to 0, animate
extern "C" void clpRebindAnim(CCLPCur* this_) {
    this_->mpLayout->UnbindAnimation(this_->mpAnimTrans1);
    this_->mpLayout->BindAnimation(this_->mpAnimTrans0);
    this_->mpLayout->SetAnimationEnable(this_->mpAnimTrans0, true);
    this_->mpAnimTrans0->SetFrame(lbl_eu_806687F8);
    this_->mpLayout->Animate(0);
}

void clpInitPgInfo(CLPPageInfo* obj, nw4r::lyt::ArcResourceAccessor* pAccessor) {
    obj->mpAccessor = pAccessor;
    obj->mpLayout = 0;
    obj->mpAnimTrans = 0;
    obj->mState = 0;
    obj->field_0D = 1;
    obj->field_0E = 0;
}

CLPPageInfo::~CLPPageInfo() {}

// Target us-8025572c
// Finish second-page (detail view) setup after clpInitPgInfo: load the detail
// layout, bind its animation, install the device font, fill the fixed text
// panes, then resolve the sample texture and size its pane.
// Shape mirrors the FULL_MATCHed CExchangeWin layout-init function.
// -O4,s: retail uses the _savegpr_29/_restgpr_29 frame.
//
// OPEN ITEM (single reloc-name drift): the u16->f32 pane-size casts pool the
// 2^52 conversion magic as TU-local @N in .sdata2; retail references the
// named lbl_eu_80668808. Source cannot name it (MWCC will not pool plain
// casts onto a declared symbol - see the CArtsInfo ConvS32ToF64 note), so
// this needs the standard pool_patterns rule in tools/postprocess_reloc_names.py:
//   "CCollepedia.o": UnitRules(
//       pool_patterns=((struct.pack(">II", MAGIC_HI, 0), "lbl_eu_80668808"),),
//       trim_sdata2_size=0,
//   ),
// With that rule the function is byte-identical (mismatch: 0).
extern "C" void clpSetupDetPg(CLPPageInfo* pg) {
    buildLayout(&pg->mpLayout, pg->mpAccessor, "mf70_cf60_syswin12.brlyt");
    bindLayoutAnimTransform(pg->mpLayout, &pg->mpAnimTrans, pg->mpAccessor, "mf70_cf60_syswin12_in.brlan");

    // Bind the device font: root pane first (retail loads it before the
    // CDeviceFont call), then fetch the font handle from vtable slot 9.
    nw4r::lyt::Pane* rootPane = pg->mpLayout->GetRootPane();
    void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, pg->mpLayout);
    u32 fontHandle = (u32)((IDeviceFontInfo*)fontObj)->getFont();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontHandle);

    char* tagStr = (char*)CUICfManager_getPackedFont9C();
    setLayoutTextBoxFont(pg->mpLayout, "txt_sysmes03_00", (u32)tagStr);
    setLayoutTextBoxFont(pg->mpLayout, "txt_sysmes04_00", (u32)tagStr);

    pg->mpLayout->SetAnimationEnable(pg->mpAnimTrans, true);
    pg->mpLayout->Animate(0);

    // Fixed description/label panes: five category labels plus the header.
    const char* cat12 = BdatTouchStringCell("MNU_collect", "name", 12);
    LayoutSetTextBoxFmtValue(pg->mpLayout, "txt_sysmes02", cat12, 0);
    const char* cat15 = BdatTouchStringCell("MNU_collect", "name", 15);
    LayoutSetTextBoxFmtValue(pg->mpLayout, "txt_sysmes03", cat15, 0);
    const char* cat16 = BdatTouchStringCell("MNU_collect", "name", 16);
    LayoutSetTextBoxFmtValue(pg->mpLayout, "txt_sysmes04", cat16, 0);
    const char* cat13 = BdatTouchStringCell("MNU_collect", "name", 13);
    LayoutSetTextBoxFmtValue(pg->mpLayout, "txt_slct01", cat13, 0);
    const char* cat14 = BdatTouchStringCell("MNU_collect", "name", 14);
    LayoutSetTextBoxFmtValue(pg->mpLayout, "txt_slct02", cat14, 0);
    const char* keyHelp = BdatTouchStringCell("MNU_kyeassign", "help", 0x2b);
    LayoutSetTextBoxFmtValue(pg->mpLayout, "txt_close00", keyHelp, 0);

    // Sample-texture msg id: lookup table picked by controller type; result is
    // arg2 of the id lookup (retail keeps it in r4 across the branch merge).
    const char* texTable = cf::CfGameManager::isClassicController(-1) != 0
        ? "fileID_2" : "fileID_1";

    u16 id = BdatGetU16ByTableKey("MNU_kyeassign", texTable, 0x2b);
    char* texName = MakeTplNameSysFile(id);

    nw4r::lyt::ArcResourceAccessor* resAcc = CUICfManager_getArcResourceAccessor();
    TPLPalette* resource = (TPLPalette*)resAcc->GetResource(0x74696D67, texName, NULL);

    if (resource != NULL) {
        PaneSetTexPaletteByName(pg->mpLayout, "pic_btn00", resource);

        // TPL dims captured before the pane lookup (retail keeps them in
        // callee-saved regs across FindPaneByName).
        TPLHeader* header = resource->descriptorArray->textureHeader;
        u16 w = header->width;
        u16 h = header->height;

        nw4r::lyt::Pane* pane = pg->mpLayout->GetRootPane()->FindPaneByName("pic_btn00", true);
        if (pane != NULL) {
            CLPSize size;
            size.width = (f32)(u32)w;
            size.height = (f32)(u32)h;
            pane->SetSize(*reinterpret_cast<nw4r::lyt::Size*>(&size));
        }
    }
}

// Delete the detail layout if present, then clear the pointer
extern "C" void clpDelPageLyt(CLPPageInfo* this_) {
    if (this_->mpLayout != nullptr) {
        delete this_->mpLayout;
        this_->mpLayout = nullptr;
    }
}

// If layout is loaded, handle state machine and animate
extern "C" void clpUpdPgState(CLPPageInfo* this_) {
    if (this_->mpLayout == nullptr) {
        return;
    }
    switch ((int)this_->mState) {
    case 1:
        clpPgOpenDone(this_);
        break;
    case 3:
        clpPgCloseEnd(this_);
        break;
    }
    this_->mpLayout->Animate(0);
}

extern "C" void clpOpenDetail(CLPPageInfo* this_) {
    if (this_->mState != 0) return;
    this_->mState = 1;
    this_->field_0D = 0;
    this_->field_0E = 1;
    playUISound(0xD);
}

// Return whether the row's cells have all been collected.
extern "C" u32 clpGetRowDone(CLPCatalog* obj, u32 index) {
    if (index >= 6) return 0;
    return obj->mPageTable.mPages[(s8)obj->mCurrentPage].mRows[index].mDone;
}

// Return the BDAT sub-category recorded for a row.
extern "C" u8 clpGetRowSubc(CLPCatalog* obj, u32 index) {
    if (index >= 6) return 0;
    return obj->mPageTable.mPages[(s8)obj->mCurrentPage].mRows[index].mSubCategory;
}

// If the page is open (2), start closing (3) and play SE 0xE
extern "C" void clpCloseDetPg(CLPPageInfo* this_) {
    if (this_->mState != 2) return;
    this_->mState = 3;
    this_->field_0D = 0;
    playUISound(0xE);
}

// Set the two page-number text boxes on the detail layout
extern "C" void clpSetPageNum(CLPPageInfo* this_, unsigned char arg) {
    setLayoutTextBoxNumber(this_->mpLayout, "txt_sysmes03_00", (unsigned char)1);
    setLayoutTextBoxNumber(this_->mpLayout, "txt_sysmes04_00", arg);
}

// name-guess: the retail pool contains an orphan "txt_sysmes01" between
// pic_btn00 and pic_btnbs01; its original caller was link-stripped.
void clpSetSysMessage01Pane(CLPPageInfo* self) {
    setLayoutTextBoxNumber(self->mpLayout, "txt_sysmes01", 0);
}

extern "C" void clpSetPageTex(CLPPageInfo* this_, char* arg2, char* arg3) {
    if (arg2 != NULL) {
        PaneSetTexPaletteByName(this_->mpLayout, "pic_btnbs01", (TPLPalette*)arg2);
    }
    if (arg3 != NULL) {
        PaneSetTexPaletteByName(this_->mpLayout, "pic_icon01", (TPLPalette*)arg3);
    }
}

// us-80255bac
// Format a pane name, find two panes by name, and call ScalePaneWorldPos to copy text
// Retail saves r28-r31 with stmw and uses a 0x40-byte stack frame
// Retail reloads rootPane from *(layout+0x10) each time (no caching)
extern "C" void clpCalcCurPos(nw4r::math::VEC3* out, CLPPageInfo* container, int arg3) {
    char buf[0x28];
    sprintf(buf, "txt_slct%02d", arg3 + 1);
    nw4r::lyt::Pane* pane1 = container->mpLayout->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = container->mpLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
    ScalePaneWorldPos(out, pane1, pane2, container->mpLayout->GetRootPane());
}

// Check if the open animation is finished; if so, set state to 2
extern "C" void clpPgOpenDone(CLPPageInfo* self) {
    if (advanceAnimTransform(self->mpAnimTrans, lbl_eu_80668800) != 0) {
        self->mState = 2;
        self->field_0D = 1;
    }
}


// Once the open animation has rewound, reset the state flags
extern "C" void clpPgCloseEnd(CLPPageInfo* this_) {
    if (AnimRewindFrame(this_->mpAnimTrans, lbl_eu_80668800) != 0) {
        this_->mState = 0;
        this_->field_0D = 1;
        this_->field_0E = 0;
    }
}

// Init the catalog header (count=0, index=-1) and zero the page table.
extern "C" CLPCatalog* clpInitEStore(CLPCatalog* this_) {
    this_->mCount = 0;
    this_->mCurrentPage = -1;
    memset(&this_->mPageTable, 0, sizeof(this_->mPageTable));
    return this_;
}

CLPCatalog::~CLPCatalog() {}

// us-80255d78
// Build the collepedia page list:
//  1. collect the ids of every "collectible" item (kind 0xa), capped at 0x12c,
//  2. append one page (stride 0x140) per BDAT category (2..0x19) that has at
//     least one unlocked, uncompleted entry,
//  3. bubble-sort pages by their category sort key,
//  4. select the default category page and append a sentinel page (0x1a),
//  5. distribute every collected item into the pages' 6x5 grids, starting a
//     new row whenever the item sub-category changes.
extern "C" void clpBuildPages(CLPCatalog* self_) {
    extern void clpFillEntry(CLPCatalog* self, int a, int b, int c, int itemId);

    u16 n = 0;
    u16 numIds = (u16)Bdat_GetMaxRow_B1EC((void*)::cf::CfBdat::spItmItemListFileData);
    u16 ids[0x12c];
    for (u16 id = 1; id <= numIds; id++) {
        if ((u8)BdatGetItemType(id) == 0xa) {
            ids[n] = id;
            n++;
            if (n >= 0x12c) break;
        }
    }

    // One page per category with at least one active, uncompleted entry.
    void* tblA = ::cf::CfBdat::spLandmarkListFileData;
    void* tblB = ::cf::CfBdat::spFldMapListFileData;
    u32 numA = Bdat_GetMaxRow_B1EC(tblA);
    for (u8 cat = 2; cat <= 0x19; cat++) {
        switch (cat) {
        case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 0xa: case 0xb: case 0xc: case 0xd: case 0xe: case 0xf: case 0x10:
        case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16:
        case 0x17: case 0x18: case 0x19:
            break;
        }
        for (u16 j = 1; j <= (u16)numA; j++) {
            if ((u8)BdatGetU8Direct((const void*)tblA, "mapID", j) == cat &&
                BdatGetU8Direct((const void*)tblA, "category", j) == 0 &&
                CtrlRemote_TouchBitByArg(j + 0x20c8) != 0) {
                u8 cnt = self_->mCount;
                self_->mPageTable.mPages[cnt].mCategory = cat;
                self_->mCount = cnt + 1;
                break;
            }
        }
    }

    // Bubble-sort pages by their BDAT sort key (adjacent-page swap).
    u8 pageCount = self_->mCount;
    for (u8 i = 0; i < pageCount; i++) {
        u8 swapped = 0;
        for (u8 j = 0; j < pageCount - 1 - i; j++) {
            CLPPage& pg = self_->mPageTable.mPages[j];
            u8 keyCur = pg.mCategory;
            u8 keyNext = self_->mPageTable.mPages[j + 1].mCategory;
            if ((u8)BdatGetU8Direct((const void*)tblB, "index", keyCur) >
                (u8)BdatGetU8Direct((const void*)tblB, "index", keyNext)) {
                pg.mCategory = keyNext;
                self_->mPageTable.mPages[j + 1].mCategory = keyCur;
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }

    // Select the page matching the default category.
    u32 defCat = lbl_eu_80664184;
    for (u8 k = 0; k < self_->mCount; k++) {
        if ((u32)self_->mPageTable.mPages[k].mCategory == defCat) {
            self_->mCurrentPage = k;
            break;
        }
    }

    // Append the sentinel page (category 0x1a); if no default page was found,
    // select the last real page instead.
    u8 cnt = self_->mCount;
    u8 newCnt = cnt + 1;
    self_->mPageTable.mPages[cnt].mCategory = 0x1a;
    u8 sel = self_->mCurrentPage;
    self_->mCount = newCnt;
    if ((s8)sel == -1) {
        self_->mCurrentPage = newCnt - 1;
    }

    // Distribute every collected item across the pages' grids. A row break is
    // inserted when the item's sub-category (+0x180 column) differs from the
    // previous row's; same-sub-category items stack in the current row.
    for (u8 g = 0; g < self_->mCount; g++) {
        CLPPage& grp = self_->mPageTable.mPages[g];
        u8 cat = grp.mCategory;
        u8 row = 0;
        u8 col = 0;
        for (u16 c = 0; c < n; c++) {
            u16 itemId = ids[c];
            u16 kind = BdatGetItemId(itemId);
            if ((u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmCollectListFileData, "mapID", kind) != cat) {
                continue;
            }
            u8 sub = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmCollectListFileData, "type", kind);
            if (row == 0) {
                grp.mRows[0].mSubCategory = sub;
                clpFillEntry(self_, g, row, 0, itemId);
                row = 1;
                col = 1;
            } else if (sub != grp.mRows[row - 1].mSubCategory) {
                grp.mRows[row].mSubCategory = sub;
                clpFillEntry(self_, g, row, 0, itemId);
                row++;
                col = 1;
            } else {
                clpFillEntry(self_, g, row - 1, col++, itemId);
            }
        }
        grp.mRowCount = row;
    }

    clpUpdateProg(self_);
}

extern "C" bool clpIsClaimabl(CLPCatalog* this_, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return false;
    if (arg2 >= 5) return false;
    return this_->mPageTable.mPages[(s8)this_->mCurrentPage].mRows[arg1].mCells[arg2].mState == 2;
}

// us-80256178
// Handle item selection in the collepedia grid. Every access goes through
// the typed page-table / row / cell indexing (retail rematerializes the page
// index per access rather than caching a cell pointer).
// Grant path falls through first (if remaining<=0), consume call branched to.
// The cached-cell spelling was not faithful; it also wrote mSortKey instead
// of the cell's unlock-state byte, so the typed form is used here.
extern "C" void clpSelectItem(CLPCatalog* self_, u32 row, u32 col) {
    if (row >= 6) return;
    if (col >= 5) return;

    CtrlObjectParam_SetStateBitFlag(
        self_->mPageTable.mPages[(s8)self_->mCurrentPage].mRows[row].mCells[col].mKind, 1);

    self_->mPageTable.mPages[(s8)self_->mCurrentPage].mRows[row].mCells[col].mState = 3;

    u8 count = clpGetCellCnt(self_, row, col);
    s32 remaining = count - 1;

    u16 itemId =
        self_->mPageTable.mPages[(s8)self_->mCurrentPage].mRows[row].mCells[col].mItemId;
    u32 res = CItem_findRecByFamily(itemId);

    // Grant path falls through first in retail; consume call is branched to
    if (remaining <= 0) {
        // Grant the item via its impl instance (virtual slot 0x10 = vf10)
        CItemImpl* itemInst = CItem_initItemImplInstances((CItemData*)res);
        itemInst->vf10((CItemData*)res);
    } else {
        CItemData_initFromFamily((void*)res, itemId, remaining);
    }

    clpUpdateProg(self_);
}

// Increment selected page, wrap to 0 at the catalog count.
extern "C" void clpNextPageIx(CLPCatalog* obj) {
    obj->mCurrentPage++;
    if ((s8)obj->mCurrentPage >= (s32)obj->mCount) {
        obj->mCurrentPage = 0;
    }
}

extern "C" void clpPrevPageIx(CLPCatalog* obj) {
    obj->mCurrentPage = obj->mCurrentPage - 1;
    if ((signed char)obj->mCurrentPage < 0) {
        obj->mCurrentPage = obj->mCount - 1;
    }
}

extern "C" char* clpGetPageNam(CLPCatalog* self) {
    u8 val = self->mPageTable.mPages[(s8)self->mCurrentPage].mCategory;
    if (val == 0x1a) {
        return BdatTouchStringCell("MNU_collect", "name", 0x14);
    } else {
        return BdatGetPtrDirect(::cf::CfBdat::spFldMapListFileData, "name", val);
    }
}

extern "C" u8 clpGetPageCat(CLPCatalog* thisPtr) {
    return thisPtr->mPageTable.mPages[(s8)thisPtr->mCurrentPage].mCategory;
}

// Return a formatted string for the row's sub-category if it is set.
extern "C" char* clpGetRowName(CLPCatalog* self, u32 index) {
    if (index >= 6) return 0;
    u8 val = self->mPageTable.mPages[(s8)self->mCurrentPage].mRows[index].mSubCategory;
    if (val != 0) {
        return BdatTouchStringCell("MNU_collect", "name", val);
    }
    return 0;
}

extern "C" u32 func_80254144(CLPCatalog* ptr) {
    return ptr->mPageTable.mPages[(s8)ptr->mCurrentPage].mRowCount;
}

// Return the cell's unlock-state byte.
extern "C" u8 clpGetCelStat(CLPCatalog* self, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    return self->mPageTable.mPages[(s8)self->mCurrentPage].mRows[arg2].mCells[arg3].mState;
}

extern "C" u16 func_80254204(CLPCatalog* p1, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    return p1->mPageTable.mPages[(s8)p1->mCurrentPage].mRows[arg2].mCells[arg3].mSortKey;
}

extern "C" u16 clpGetCelIcon(CLPCatalog* this_, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    u16 val = this_->mPageTable.mPages[(s8)this_->mCurrentPage].mRows[arg2].mCells[arg3].mItemId;
    if (val == 0) return 0;
    return BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, "icon_base", val);
}

// Load the collected-progress float of the current page.
extern "C" float clpGetPagePrg(CLPCatalog* this_) {
    return this_->mPageTable.mPages[(s8)this_->mCurrentPage].mProgress;
}

// us-8025650c
// Look up an entry in the sub-array, check +0x10 u16 and +0x16 byte.
// If byte != 1, resolve string via BdatGetPtrDirect.
extern "C" char* clpGetEntryStr(CLPCatalog* self_, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return nullptr;
    if (arg2 >= 5) return nullptr;
    CLPCell* cell = &self_->mPageTable.mPages[(s8)self_->mCurrentPage].mRows[arg1].mCells[arg2];
    u16 val = cell->mKind;
    if (val == 0) return nullptr;
    u8 b = cell->mState;
    if (b == 1) return nullptr;
    return (char*)BdatGetPtrDirect(::cf::CfBdat::spItmCollectListFileData, "name", val);
}

// us-8025658c
// Look up an entry in the sub-array, check if item is unlocked (via CtrlObjectParam_GetStateBitFlag), format its name
// Returns nullptr if the entry is invalid or locked
extern "C" char* clpGetItemName(CLPCatalog* self_, u32 arg2, u32 arg3) {
    if (gpClpItemMsgTable == NULL) return NULL;
    if (arg2 >= 6) return NULL;
    if (arg3 >= 5) return NULL;

    CLPCell* cell = &self_->mPageTable.mPages[(s8)self_->mCurrentPage].mRows[arg2].mCells[arg3];

    u16 val = cell->mNameId;
    if (val == 0) return NULL;

    u16 check = cell->mKind;
    if (CtrlObjectParam_GetStateBitFlag(check) != 0) {
        return (char*)BdatGetPtrDirect(gpClpItemMsgTable, "comment", val);
    }
    return NULL;
}

// Read u16 at offset 0xE within the indexed sub-array entry
// If non-zero, resolve via CItem_sumFamilyByte6 and return as u8
// The sub-array is indexed by: (s8)self[1] * 0x140 + arg2 * 0x34 + arg3 * 0xA + 0xE
extern "C" u8 clpGetCellCnt(CLPCatalog* self, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    u16 val = self->mPageTable.mPages[(s8)self->mCurrentPage].mRows[arg2].mCells[arg3].mItemId;
    if (val != 0) {
        return (u8)CItem_sumFamilyByte6(val);
    }
    return 0;
}

// Whether the current page has been fully collected.
extern "C" bool clpIsPageDone(CLPCatalog* this_) {
    CLPPage* page = &this_->mPageTable.mPages[(s8)this_->mCurrentPage];
    return *(bool*)&page->mDone;
}

// us-802566d8
// Fill one grid entry through its CLPCell fields:
// item id, category lookups, and an unlock-state byte.
extern "C" void clpFillEntry(CLPCatalog* self_, int a, int b, int c, u16 itemId) {
    CLPCell* cell = &self_->mPageTable.mPages[a].mRows[b].mCells[c];

    cell->mItemId = itemId;

    u16 kind = BdatGetItemId(itemId);

    cell->mKind = kind;

    cell->mSortKey =
        BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, "icon", (int)itemId);
    cell->mNameId =
        BdatGetU16Direct((const void*)::cf::CfBdat::spItmItemListFileData, "comment", (int)itemId);

    // Unlock state: 0=kindless, 3=owned, 2=new/claimable, 1=locked
    if (kind == 0) {
        cell->mState = 0;
    } else if (CtrlObjectParam_GetStateBitFlag(kind) != 0) {
        cell->mState = 3;
    } else if (CItem_findRecByFamily(itemId) != 0) {
        cell->mState = 2;
    } else {
        cell->mState = 1;
    }
}

// us-80256ae4
// Destructor for CCollepedia.
// Calls sub-object destructors and frees memory if dealloc_flag > 0.

// us-802567fc
// Recompute collection progress for every category page.
// A row counts as "complete" when the leading non-empty slots of its 5
// columns all have unlock-state 3 (collected); a page is complete when all
// its rows are complete. The per-page progress float at +8 is either 1.0
// (complete page) or collected-count / seen-count.
extern "C" void clpUpdateProg(CLPCatalog* self_) {
    u8* self = (u8*)self_;
    u8* group;
    u8* row;
    u8 i;
    u8 groupDone;
    u8 rowCount;
    u8 r;
    u8 rowDone;
    u8 c;

    for (i = 0; i < self[0]; i++) {
        group = self + i * 0x140;
        rowCount = group[5];
        groupDone = 1;
        for (r = 0; r < rowCount; r++) {
            row = group + r * 0x34;
            rowDone = 1;
            for (c = 0; c < 5; c++) {
                u8 state = row[c * 0xA + 0x16];
                if (state == 0) break;
                if (state == 3) continue;
                rowDone = 0;
                break;
            }
            row[0xD] = rowDone;
            if (rowDone == 0) groupDone = 0;
        }
        group[6] = groupDone;
    }

    // Ratio of collected (state 3) columns over columns reached before hitting
    // an empty slot, seeded with the constant below.
    float one = lbl_eu_80668800;
    {
        u8* row2;
        u8* grp;
        u8 ii;
        for (ii = 0; ii < self[0]; ii++) {
            grp = self + ii * 0x140;
            if (grp[6] != 0) {
                *(float*)(grp + 8) = one;
            } else {
            float denom = lbl_eu_806687F8;
            float num = denom;
            u8 cnt = grp[5];
            u8 rr;
            u8 cc;
            for (rr = 0; rr < cnt; rr++) {
                row2 = grp + rr * 0x34;
                for (cc = 0; cc < 5; cc++) {
                    u8 state = row2[cc * 0xA + 0x16];
                    if (state == 0) break;
                    if (state == 3) num += one;
                    denom += one;
                }
            }
            *(float*)(grp + 8) = num / denom;
            }
        }
    }
}

CCollepedia::~CCollepedia() {}

// us-8025695c - CCollepedia constructor.
// Stores vtable, constructs both memory regions, cursors and the embedded
// CSysWin at +0x9C, initializes the entry storage, then re-initializes the
// window from a stack temporary (copying everything after the vtable ptr).
CCollepedia::CCollepedia()
    : field_24(nullptr), field_28(nullptr), field_2C(nullptr), field_30(nullptr),
      field_34(nullptr), field_38(nullptr), field_3C(nullptr), field_40(nullptr),
      field_44(nullptr), field_48(0), field_49(0), field_4C(0), field_50(0),
      field_51(1), mCur07(nullptr), mClpCur(nullptr), mCur18(nullptr), mSysWin(0),
      field_D8(0), field_D9(0), field_DA(0) {
    clpInitEStore(&mCatalog);
    clpInitPgInfo(&mPageInfo, nullptr);

    // Re-initialise the embedded window from a stack temporary.
    mSysWin = CSysWin(0);

    gpClpItemMsgTable = nullptr;
    gpClpCollectMsgTable = nullptr;
}

// us-80256b68
// Initialize CCollepedia: load 3 files, init CSysWin, init sub-array storage.
// The entry storage is built in a stack temp and copied member-wise to match
// retail codegen (byte stores for the header, bulk struct copy for the data).
void clpStartLoads(CCollepedia* this_) {
    this_->field_24 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), "menu/jp/Collepedia.arc", this_, 0, 0);

    this_->field_28 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), "menu/jp/tpl/ItemIcon.arc", this_, 0, 0);

    this_->field_2C = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        KyoshinHeap_GetField44(), "common/jp/bdat_menu_item.bin", this_, 0, 0);

    // Virtual call on CSysWin at this+0x9C: loadSystemArc at vtable +0x88.
    this_->mSysWin.loadSystemArc();

    CLPInitTemp tmp;
    tmp._00 = 0;
    tmp._01 = -1;
    memset(&tmp.data, 0, sizeof(tmp.data));
    this_->mCatalog.mCount = tmp._00;
    this_->mCatalog.mCurrentPage = tmp._01;
    this_->mCatalog.mPageTable = tmp.data;

    clpBuildPages(&this_->mCatalog);
}

// us-80256c5c
// Per-frame state machine dispatch on field_49, then update all sub-objects.
// Routing mirrors retail jumptable_eu_80537378 (17 entries): slots 0/7/10/14
// are empty and route to the shared tail.
extern "C" void clpDoFrameUpd(CCollepedia* this_) {
    if (this_->field_48 == 0) return;
    if (this_->field_49 == 0) return;

    switch (this_->field_49) {
    case 1:
        clpWaitIntro(this_);
        break;
    case 2:
        clpEnterBrows(this_);
        break;
    case 3:
        // Keep polling the intro anim transform until it finishes
        advanceAnimTransform(this_->field_44, lbl_eu_80668800);
        break;
    case 4:
        clpWaitOutro(this_);
        break;
    case 5:
        clpEnterIdle(this_);
        break;
    case 6:
        clpEnabDetCur(this_);
        break;
    case 7:
        break;
    case 8:
        clpEnterDetail(this_);
        break;
    case 9:
        if (this_->mSysWin.isActive()) {
            this_->field_49 = 0xA;
        }
        break;
    case 10:
        break;
    case 11:
        collepediaCreateItemMulti(this_);
        break;
    case 12:
        clpOpenComplete(this_);
        break;
    case 13:
        if (this_->mSysWin.isActive()) {
            this_->field_49 = 0xE;
        }
        break;
    case 14:
        break;
    case 15:
        clpPlayTblSe(this_);
        break;
    case 16:
        if (!GetItemMulti_IsActiveFlag()) {
            this_->field_49 = 3;
        }
        break;
    }

    this_->field_38->Animate(0);
    this_->mCur07.Update();
    clpCurAnimate(&this_->mClpCur); // +0x6c
    this_->mSysWin.dispatchPhase();
    // Retail calls the second page's state machine with the +0x28EC sub-object
    clpUpdPgState(&this_->mPageInfo);
    this_->mCur18.Update(); // +0x84
}

// us-80256da0
// Render the CCollepedia display: draw layout, sub-cursors, and syswin if active
// Retail reads field_48/field_49 and field_38 from r3 (original this) before any reg copy
extern "C" void clpDrawWindow(CCollepedia* this_, nw4r::lyt::DrawInfo* drawInfo) {
    if (this_->field_48 == 0) return;
    if (this_->field_49 == 0) return;
    
    drawLayout(this_->field_38, drawInfo, 0, 1);
    clpCurDrawLyt(&this_->mClpCur, drawInfo);
    
    if (this_->mPageInfo.mpLayout != NULL) {
        drawLayout(this_->mPageInfo.mpLayout, drawInfo, 0, 1);
    }
    
    this_->mCur07.DrawLayout(drawInfo);
    this_->mSysWin.drawLayout(reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo));
    this_->mCur18.DrawLayout(drawInfo);
}

// us-80256e40
// Cleanup CCollepedia: free files, layouts, sub-objects
void clpFreeAllRes(CCollepedia* this_) {
    CBdat::getEntry(2);
    closeFileHandle((CFileHandle**)&this_->field_24);
    closeFileHandle((CFileHandle**)&this_->field_28);
    closeFileHandle((CFileHandle**)&this_->field_2C);

    this_->field_48 = 0;
    clpDelPageLyt(&this_->mPageInfo);

    // Free layout at +0x38 via its virtual deleting dtor
    // (single if: `delete` itself emits the second null-check seen in retail)
    if (this_->field_38 != nullptr) {
        delete this_->field_38;
        this_->field_38 = nullptr;
    }

    // Release arc resource accessors
    releaseArcResourceAccessor(this_->field_30);
    releaseArcResourceAccessor(this_->field_34);

    // Free heap object at +0x4C via its virtual deleting dtor
    if (this_->field_4C != 0) {
        delete (CTagProcessor*)this_->field_4C;
        this_->field_4C = 0;
    }

    // Tear down memory regions backing the layouts
    this_->mRegion0.deleteRegion();
    this_->mRegion1.deleteRegion();

    // Sub-objects at +0x54 and +0x84: cursor cleanup at vtable +0x0C.
    this_->mCur07.cleanup();

    clpCurCleanup(&this_->mClpCur);
    this_->mSysWin.termLayout();

    this_->mCur18.cleanup();
}

extern "C" u8 clpWinIsReady(CCollepedia* self) {
    if (self->mSysWin.isReady()) {
        return self->field_50;
    }
    return 0;
}

struct CCollepediaState {
    u8 _00[0x49];
    u8 initialized;
    u8 _4A[7];
    u8 field51;
    u8 _52[0x28F9 - 0x52];
    u8 condition;
};

extern "C" unsigned char clpGetInputOk(u8* self) {
    CCollepediaState* state = (CCollepediaState*)self;
    return state->condition ? state->field51 : 0;
}

extern "C" void clpBeginOpenS(u8* p) {
    CCollepediaState* state = (CCollepediaState*)p;
    if (state->initialized != 0) return;
    state->initialized = 1;
    state->field51 = 0;
}

// us-80256fc8
// Close the collepedia detail view: set state, reset sub-object, disable animations, enable outro, play sound
void clpReqCloseVw(CCollepedia* this_) {
    if (this_->mPageInfo.field_0D == 0) return;
    if (!this_->mSysWin.isActive()) return;
    if (this_->mSysWin.isOpened()) return;
    if (this_->field_49 != 3) return;

    this_->field_49 = 4;
    this_->field_51 = 0;
    this_->mCur07.SetVisible(0);

    this_->field_38->SetAnimationEnable(this_->field_44, false);
    this_->field_38->SetAnimationEnable(this_->field_3C, false);
    this_->field_38->SetAnimationEnable(this_->field_40, true);

    playUISound(6);
}

// us-802570a0
// Decrement timer counters (field_DA or field_D9) with wrapping, update display, schedule event
void clpCursorLeft(CCollepedia* this_) {
    if (this_->mSysWin.isOpened()) return;

    if (this_->mPageInfo.field_0E != 0) {
        if (this_->mPageInfo.field_0D == 0) return;

        u8 val = this_->field_DA - 1;
        this_->field_DA = val;
        if ((s8)val < 0) {
            this_->field_DA = 1;
        }
        clpRefreshCur(this_);
    } else {
        if (this_->field_49 != 3) return;

        u8 val = this_->field_D9 - 1;
        this_->field_D9 = val;
        if ((s8)val < 0) {
            this_->field_D9 = (u8)(func_80254144(&this_->mCatalog) - 1);
        }
        clpRefreshCur(this_);
        clpSetSelTxt(this_);
    }

    playUISound(1);
}

// us-80257168
// Increment timer-based navigation: increment field_DA with wrap, or advance field_D9 and update display
void clpCursorRght(CCollepedia* this_) {
    if (this_->mSysWin.isOpened()) return;

    if (this_->mPageInfo.field_0E != 0) {
        if (this_->mPageInfo.field_0D == 0) return;

        this_->field_DA = this_->field_DA + 1;
        if ((s8)this_->field_DA >= 2) {
            this_->field_DA = 0;
        }
        clpRefreshCur(this_);
    } else {
        if (this_->field_49 != 3) return;

        this_->field_D9 = this_->field_D9 + 1;
        if ((s8)this_->field_D9 >= (s32)(func_80254144(&this_->mCatalog) & 0xFF)) {
            this_->field_D9 = 0;
        }
        clpRefreshCur(this_);
        clpSetSelTxt(this_);
    }

    playUISound(1);
}

// Timer-based countdown that calls clpPrevCatPg when field_D8 reaches 0
void clpCursorUp(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->mPageInfo.field_0E != 0) return;
    if (this_->mSysWin.isOpened()) return;

    u8 d8 = this_->field_D8;
    if ((s8)d8 == 0) {
        clpPrevCatPg(this_);
        this_->field_D8 = 4;
        clpRefreshCur(this_);
        clpSetSelTxt(this_);
    } else {
        u8 val = d8 - 1;
        this_->field_D8 = val;
        if ((s8)val < 0) {
            this_->field_D8 = 4;
        }
        clpRefreshCur(this_);
        clpSetSelTxt(this_);
    }

    playUISound(1);
}

// us-802572f0
// Increment field_D8 with wrap at 5; when field_D8 == 4 call clpNextCatPg, then update display
void clpCursorDown(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->mPageInfo.field_0E != 0) return;
    if (this_->mSysWin.isOpened()) return;

    if ((s8)this_->field_D8 == 4) {
        clpNextCatPg(this_);
        this_->field_D8 = 0;
        clpRefreshCur(this_);
        clpSetSelTxt(this_);
    } else {
        u8 val = this_->field_D8 + 1;
        this_->field_D8 = val;
        if ((s8)val >= 5) {
            this_->field_D8 = 0;
        }
        clpRefreshCur(this_);
        clpSetSelTxt(this_);
    }

    playUISound(1);
}

// us-802573a8
// Decrement field_D9 with wrapping, update display, and schedule next event
extern "C" void clpNextCatPg(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->mPageInfo.field_0E != 0) return;
    if (this_->mSysWin.isOpened()) return;

    clpNextPageIx(&this_->mCatalog);
    u32 count = func_80254144(&this_->mCatalog) & 0xFF;

    // Force field_D9 sign-extend before comparing with masked count
    if ((s8)this_->field_D9 >= (s32)count) {
        this_->field_D9 = (u8)(func_80254144(&this_->mCatalog) - 1);
        clpRefreshCur(this_);
    }

    clpRefreshPage(this_);
    clpSetSelTxt(this_);
    playUISound(10);
}

// us-8025744c
// Decrement field_D9 with wrapping (calls clpPrevPageIx instead of clpNextPageIx)
extern "C" void clpPrevCatPg(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->mPageInfo.field_0E != 0) return;
    if (this_->mSysWin.isOpened()) return;

    clpPrevPageIx(&this_->mCatalog);
    int count = (int)func_80254144(&this_->mCatalog) & 0xFF;
    if ((s8)this_->field_D9 >= count) {
        this_->field_D9 = (u8)(func_80254144(&this_->mCatalog) - 1);
        clpRefreshCur(this_);
    }

    clpRefreshPage(this_);
    clpSetSelTxt(this_);
    playUISound(10);
}

// us-802574f0
// Handle CCollepedia navigation: check state and dispatch actions.
// Early-return states are tested as an unsigned compare chain (cmplwi).
void clpOnCancel(CCollepedia* this_) {
    u32 state = this_->field_49;
    if (state == 9) return;
    if (state == 0xB) return;
    if (state == 0xC) return;
    if (state == 0xD) return;
    if (state == 0xF) return;
    if (state == 0x10) return;

    if (this_->mSysWin.isOpened() != 0) {
        if (this_->mSysWin.isActive() == 0) return;
        s32 cur = this_->field_49;
        switch (cur) {
        case 0xA:
            this_->field_49 = 0xB;
            break;
        case 0xE:
            this_->field_49 = 0xF;
            break;
        }
        this_->mSysWin.advancePhase3();
        return;
    }

    // CSysWin is not active, check sub-array conditions
    if (this_->mPageInfo.field_0E == 0) return;
    if (this_->mPageInfo.field_0D == 0) return;

    clpCloseDetPg(&this_->mPageInfo);
    this_->mCur18.SetVisible(0);
    this_->field_49 = 8;
    this_->field_51 = 0;
    playUISound(6);
}

// us-802575e8
// Per-frame advance for the collepedia selection flow.
// Early-return states are tested as a signed compare chain (cmpwi/beq).
extern "C" void clpOnConfirm(CCollepedia* this_) {
    u32 state = this_->field_49;
    if (state == 9) return;
    if (state == 0xB) return;
    if (state == 0xC) return;
    if (state == 0xD) return;
    if (state == 0xF) return;
    if (state == 0x10) return;

    if (this_->mSysWin.isOpened()) {
        if (this_->mSysWin.isActive() == 0) return;
        // Two-case switch: MWCC branches forward to the grouped case bodies
        switch (this_->field_49) {
        case 0xA:
            this_->field_49 = 0xB;
            break;
        case 0xE:
            this_->field_49 = 0xF;
            break;
        }
        this_->mSysWin.advancePhase3();
        return;
    }

    if (this_->mPageInfo.field_0E != 0) {
        if (this_->mPageInfo.field_0D == 0) return;

        if ((s8)this_->field_DA == 0) {
            // Grid page: grant the selected item, refresh both display pages,
            // then play a category-keyed fanfare.
            clpSelectItem(&this_->mCatalog, this_->field_D9, this_->field_D8);
            clpRefreshPage(this_);
            clpSetSelTxt(this_);

            u32 cat = clpGetRowSubc(&this_->mCatalog, this_->field_D9);
            // Switch range-check covers 0..8; case 8 is empty (fanfare only)
            switch (cat) {
            case 0: playUISound(0x4B); break;
            case 1: playUISound(0x4C); break;
            case 2: playUISound(0x4D); break;
            case 3: playUISound(0x4E); break;
            case 4: playUISound(0x4F); break;
            case 5: playUISound(0x50); break;
            case 6: playUISound(0x51); break;
            case 7:
            case 8: playUISound(0x52); break;
            }
            incrementEventCounter(0x84);
        } else {
            // Detail page confirm: schedule sound 6 instead.
            playUISound(6);
        }

        clpCloseDetPg(&this_->mPageInfo);
        this_->mCur18.SetVisible(0);
        this_->field_49 = 8;
        this_->field_51 = 0;
        return;
    }

    // List page: open the detail view for the selected entry.
    if (clpIsClaimabl(&this_->mCatalog, this_->field_D9, this_->field_D8) == 0) return;

    char* desc = clpGetEntryStr(&this_->mCatalog, this_->field_D9, this_->field_D8);
    LayoutSetTextBoxFmtValue(this_->mPageInfo.mpLayout, "txt_sysmes01", desc, 0);

    clpSetPageNum(&this_->mPageInfo,
        clpGetCellCnt(&this_->mCatalog, this_->field_D9, this_->field_D8));

    char* name1 = MakeTplNameSysFile((u16)clpGetCelIcon(&this_->mCatalog, this_->field_D9, this_->field_D8));
    void* tex1 = this_->field_30->GetResource(0x74696D67, name1, NULL);

    char* name2 = MakeTplNameSysFile((u16)func_80254204(&this_->mCatalog, this_->field_D9, this_->field_D8));
    void* tex2 = this_->field_34->GetResource(0x74696D67, name2, NULL);

    clpSetPageTex(&this_->mPageInfo, (char*)tex1, (char*)tex2);
    clpOpenDetail(&this_->mPageInfo);

    this_->field_DA = 0;
    this_->mCur07.SetVisible(0);
    this_->field_49 = 6;
    this_->field_51 = 0;
    playUISound(3);
}

int clpCanClaim(CCollepedia* self) {
    return clpIsClaimabl(&self->mCatalog, self->field_D9, self->field_D8);
}

extern "C" u8 clpIsOverlay(CCollepedia* self) {
    if (self->mSysWin.isOpened()) {
        return 1;
    }
    return self->mPageInfo.field_0E;
}

// Returns 1 if any block condition is active, otherwise calls GetItemMulti_IsActiveFlag
extern "C" u32 clpIsBusy(CCollepedia* this_) {
    if (this_->field_49 != 3) {
        return 1;
    }
    if (this_->mSysWin.isOpened()) {
        return 1;
    }
    if (this_->mPageInfo.field_0E != 0) {
        return 1;
    }
    return GetItemMulti_IsActiveFlag();
}

// us-80257a1c
// Check if animation at field_40 is done; if so, set state, init sub-object, switch animations
void clpEnterBrows(CCollepedia* this_) {
    if (advanceAnimTransform(this_->field_40, lbl_eu_80668800) != 0) {
        this_->field_49 = 3;
        this_->field_51 = 1;
        this_->mCur07.SetVisible(1);

        this_->field_38->SetAnimationEnable(this_->field_3C, false);
        this_->field_38->SetAnimationEnable(this_->field_40, false);
        this_->field_38->SetAnimationEnable(this_->field_44, true);

        clpRefreshCur(this_);
    }
}

// us-80257ad0
// Check if animation at field_40 is still playing; if so, disable field_40 and field_44, enable field_3C, set state to 5
void clpWaitOutro(CCollepedia* this_) {
    if (AnimRewindFrame(this_->field_40, lbl_eu_80668800) != 0) {
        this_->field_38->SetAnimationEnable(this_->field_40, false);
        this_->field_38->SetAnimationEnable(this_->field_44, false);
        this_->field_38->SetAnimationEnable(this_->field_3C, true);
        this_->field_49 = 5;
    }
}

// Check if animation at field_3C is done, then set state and init sub-object
extern "C" void clpEnterIdle(CCollepedia* this_) {
    if (AnimRewindFrame(this_->field_3C, lbl_eu_80668800) != 0) {
        this_->field_51 = 1;
        this_->field_49 = 0;
        this_->mCur07.SetVisible(0);
    }
}

extern "C" void clpEnabDetCur(CCollepedia* this_) {
    if (this_->mPageInfo.field_0D == 0) return;
    this_->field_49 = 7;
    this_->field_51 = 1;
    this_->mCur18.SetVisible(1);
    clpRefreshCur(this_);
}

// us-80257c18
// Open the collepedia detail view: set state, enable sub-object, update display, load file
void clpEnterDetail(CCollepedia* this_) {
    if (this_->mPageInfo.field_0D == 0) return;

    this_->field_49 = 3;
    this_->field_51 = 1;
    this_->mCur07.SetVisible(1);

    clpRefreshCur(this_);

    if (clpGetRowDone(&this_->mCatalog, this_->field_D9) == 0) return;

    this_->field_49 = 9;

    char* str = (char*)BdatTouchStringCell("MNU_collect", "name", 0x12);
    this_->mSysWin.setKindContent(reinterpret_cast<char*>(str), 0);
    this_->mSysWin.applyKind(1);
    this_->mSysWin.openPhase1();

    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    CTaskGame_openStream(0, "snd/adx/j01.adx", (u32)handle, 2, 1, 0,
                         MenuSnd_GetMasterVol_95EC());
}

// us-80257cf0
// If CSysWin is active, set state, look up table entry, format string, call UIWin_CreateItemMulti
extern "C" void collepediaCreateItemMulti(CCollepedia* this_) {
    if (!this_->mSysWin.isActive()) return;

    this_->field_49 = 12;

    // Force lhzx into r3 (retail) by keeping the table entry as the sole
    // live result of the index expression before the +1 / fmt setup.
    u32 idx = clpGetPageCat(&this_->mCatalog);
    s32 d9 = (s8)this_->field_D9;
    u16 entry = sClpPageCategoryTable[idx & 0xFF];
    u32 count = (u32)entry + 1;
    u32 result = BdatGetU16Direct(gpClpCollectMsgTable, "itemID",
        (u32)(u16)count + d9);

    u32 zero = 0;
    UIWin_CreateItemMulti((u16)result, 0, 0, 0, 0, 0, 0, 0, zero);
}

// us-80257d9c
// Initialize collepedia display: check condition, set up CSysWin with text, play sounds, load file
void clpOpenComplete(CCollepedia* this_) {
    if (GetItemMulti_IsActiveFlag()) return;

    this_->field_49 = 3;

    if (clpIsPageDone(&this_->mCatalog) == 0) return;

    this_->field_49 = 0xD;

    char* str = (char*)BdatTouchStringCell("MNU_collect", "name", 0x13);
    this_->mSysWin.setKindContent(reinterpret_cast<char*>(str), 0);
    this_->mSysWin.applyKind(1);
    this_->mSysWin.openPhase1();

    incrementEventCounter(0x85);
    incrementEventCounter(0x86);

    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    CTaskGame_openStream(0, "snd/adx/j02.adx", (u32)handle, 2, 1, 0,
                         MenuSnd_GetMasterVol_95EC());
}

// us-80257e64
// If CSysWin is active, do a table lookup and format a sound/effect call
void clpPlayTblSe(CCollepedia* this_) {
    if (!this_->mSysWin.isActive()) return;

    this_->field_49 = 0x10;

    // Table lookup folded into the call so the index comes straight from clpGetPageCat
    u32 result = BdatGetU16Direct(gpClpCollectMsgTable, "itemID",
        sClpPageCategoryTable[clpGetPageCat(&this_->mCatalog) & 0xFF]);
    UIWin_CreateItemMulti((u16)result, 0, 0, 0, 0, 0, 0, 0, 0);
}

// us-80257efc
// Check layout/resource pointers and the two SDA string-pool pointers, then update display
void clpTryShow(CCollepedia* this_) {
    if (this_->field_38 == nullptr || this_->field_34 == nullptr ||
        gpClpItemMsgTable == nullptr || gpClpCollectMsgTable == nullptr) {
        return;
    }
    clpSetPageVis(this_);
    clpRefreshPage(this_);
    clpSetSelTxt(this_);
    this_->field_50 = 1;
    this_->field_48 = 1;
}

// us-80257f78
// Load the texture resource for the detail pane selected by `mode` and show it.
// -O4,s gives the retail stmw/lmw block-save frame.
void clpSetSlotTex(CCollepedia* this_, const char* name, int mode) {
    void* res = NULL;
    // Mode 3 intentionally shares mode 2's resource and falls through without break.
    switch (mode) {
    case 0:
        res = this_->field_30->GetResource(0x74696D67, "mf55_sys01_icbsno00.tpl", NULL);
        break;
    case 1:
        res = this_->field_30->GetResource(0x74696D67, "mf55_sys01_icbsof00.tpl", NULL);
        break;
    case 2:
        res = this_->field_30->GetResource(0x74696D67, "mf55_sys01_icbs00.tpl", NULL);
        break;
    case 3:
        res = this_->field_30->GetResource(0x74696D67, "mf55_sys01_icbs00.tpl", NULL);
    }

    if (res != NULL) {
        char buf[0x28];
        sprintf(buf, "pic_btnbs%02d", name + 1);
        PaneSetTexPaletteByName(this_->field_38, (const char*)buf, (TPLPalette*)res);
    }
}

// us-802580cc
// Like clpSetSlotTex but mode 3 resolves the resource name from an item id.
// -O4,s gives the retail stmw/lmw block-save frame (three call sites total).
void clpSetSlotIcn(CCollepedia* this_, const char* name, u32 mode, int id) {
    void* tex;
    if (mode == 3) {
        if (id != 0)
            // Detail view: resolve the texture name from the item id via the second accessor.
            tex = this_->field_34->GetResource(0x74696D67, MakeTplNameSysFile(id), NULL);
        else
            tex = this_->field_30->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
    } else {
        tex = this_->field_30->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
    }

    if (tex != NULL) {
        char buf[0x28];
        sprintf(buf, "pic_icon%02d", name + 1);
        PaneSetTexPaletteByName(this_->field_38, (const char*)buf, (TPLPalette*)tex);
    }
}

// Manual signed-int -> double conversion (docs/MWCC_PATTERNS.md 7i idiom,
// see CArtsInfo ConvS32ToF64): build the 0x4330000080000000 bit pattern and
// subtract the shared sdata2 magic so the lfd references lbl_eu_80668818
// instead of a TU-local pool label.
static double clpConvS32ToF64(s32 x) {
    union {
        double d;
        u32 w[2];
    } u;
    // xoris word first, then 0x43300000, or MWCC hoists the lis out of order.
    u.w[1] = (u32)x ^ 0x80000000;
    u.w[0] = 0x43300000;
    return (double)x;
}

// us-802581d4
// Refresh the detail page: show/hide panes, move the cursor, description
// text, collected-count label, per-entry rows and texture slots.
// Retail frame is 0x70 with _savegpr_25: only r25-r31 stay live across calls.
extern "C" void clpRefreshPage(CCollepedia* this_) {
    char buf[0x18];
    u8 idx = (u8)(this_->mCatalog.mCurrentPage + 1); // retail reads the index byte unsigned
    setLayoutTextBoxNumber(this_->field_38, "txt_pg00", idx);

    // Move the cursor pane right by one slot width from the saved position.
    // The int->double step references the shared 2^52 pool entry explicitly.
    nw4r::lyt::Pane* cursorPane =
        this_->field_38->GetRootPane()->FindPaneByName("pic_pghole_on", true);
    nw4r::math::VEC3 pos = *(nw4r::math::VEC3*)&this_->field_DC[0];
    pos.x = 16.0f * clpConvS32ToF64((s32)idx - 1) + pos.x;
    copyVEC3((float*)((u8*)cursorPane + 0x2C), &pos);

    LayoutSetTextBoxFmtValue(this_->field_38, "txt_map",
        clpGetPageNam(&this_->mCatalog), 0);

    // "collected / total" label: scale the completion ratio to slot count.
    // Call first, constant load second - matches retail schedule.
    float ratio = clpGetPagePrg(&this_->mCatalog);
    u8 slot = (u8)(100.0f * ratio); // fctiwz truncation
    char* label = BdatTouchStringCell("MNU_collect", "name", 0xB);
    sprintf(buf, "%d%s", slot, label);
    LayoutSetTextBoxText(this_->field_38, "txt_cmp", buf, 0);

    u32 count = func_80254144(&this_->mCatalog);
    // Exactly six rows exist per page; visibility still follows the count.
    for (u8 i = 0; i < 6; i++) {
        // Retail computes i+1 once per iteration into a callee-saved reg.
        u8 num = (u8)(i + 1);
        sprintf(buf, "txt_ct%02d", num);
        LayoutSetTextBoxFmtValue(this_->field_38, buf, clpGetRowName(&this_->mCatalog, i), 0);

        sprintf(buf, "nul_icon%02d", num);
        nw4r::lyt::Pane* namePane =
            this_->field_38->GetRootPane()->FindPaneByName(buf, true);
        setPaneVisible(namePane, i < (u8)count);

        sprintf(buf, "pic_cmpbs%02d", num);
        nw4r::lyt::Pane* numPane =
            this_->field_38->GetRootPane()->FindPaneByName(buf, true);
        setPaneVisible(numPane, clpGetRowDone(&this_->mCatalog, i));
    }

    // Fill the 6x5 grid: texture slots and item icons per visible entry.
    for (u8 row = 0; row < (u32)(u8)count; row++) {
        u32 base = row * 5;
        for (u8 col = 0; col < 5; col++) {
            u32 mode = col + base;
            clpSetSlotTex(this_, (const char*)clpGetCelStat(&this_->mCatalog, row, col),
                (u8)mode);
            u16 id = func_80254204(&this_->mCatalog, row, col);
            clpSetSlotIcn(this_, (const char*)clpGetCelStat(&this_->mCatalog, row, col),
                (u8)mode, id);
        }
    }

    // Final pane visibility follows the page's completion flag.
    bool done = clpIsPageDone(&this_->mCatalog);
    nw4r::lyt::Pane* markPane = this_->field_38->GetRootPane()->FindPaneByName(
        "nul_compall", true);
    setPaneVisible(markPane, done);
}
// Set two pane names on the layout using the current sub-array entry
extern "C" void clpSetSelTxt(CCollepedia* this_) {
    char* str1 = clpGetEntryStr(&this_->mCatalog, this_->field_D9, this_->field_D8);
    LayoutSetTextBoxFmtValue(this_->field_38, "txt_name", str1, 0);
    char* str2 = clpGetItemName(&this_->mCatalog, this_->field_D9, this_->field_D8);
    LayoutSetTextBoxFmtValue(this_->field_38, "txt_comment", str2, this_->field_4C);
}
// us-80258550 Refresh the collepedia display: detail view routes through the shared
// formatter, list view formats the current entry name and moves the cursor.
extern "C" void clpRefreshCur(CCollepedia* this_) {
    // Scratch block: detail-path pane-name buffer at +0x00, cursor
    // position VEC3 at +0x0C, list-path page-name buffer at +0x18.
    u8 work[0x38];
    if (this_->mPageInfo.field_0E != 0) {
        clpCalcCurPos(reinterpret_cast<nw4r::math::VEC3*>(work),
            &this_->mPageInfo, this_->field_DA);

        this_->mCur18.setRootPaneTranslate((nw4r::math::VEC3*)work);
    } else {
        char* name = reinterpret_cast<char*>(work + 0x18);
        sprintf(reinterpret_cast<char*>(work + 0x18), "pic_btnbs%02d",
            (s8)this_->field_D9 * 5 + (s8)this_->field_D8 + 1);

        nw4r::lyt::Pane* pane1 = this_->field_38->GetRootPane()->FindPaneByName(name, true);
        nw4r::lyt::Pane* pane2 = this_->field_38->GetRootPane()->FindPaneByName("nul_proportion", true);

        ScalePaneWorldPos(reinterpret_cast<nw4r::math::VEC3*>(work + 0x0C), pane1, pane2,
            this_->field_38->GetRootPane());

        this_->mCur07.setRootPaneTranslate(
            reinterpret_cast<nw4r::math::VEC3*>(work + 0x0C));
    }
}

// us-80258658
// Iterate sub-array entries, set pane visibility for each, copy position for current entry, update display
void clpSetPageVis(CCollepedia* this_) {
    char buf[0x20];
    u8 count = this_->mCatalog.mCount;
    for (u8 i = 1; i <= 0x16; i++) {
        sprintf(buf, "pic_pghole%02d", i);

        setPaneVisible(this_->field_38->GetRootPane()->FindPaneByName(buf, true), i <= count);

        if (i == count) {
            copyVEC3(this_->field_DC,
                (void*)&this_->field_38->GetRootPane()->FindPaneByName(buf, true)->GetTranslate());
        }
    }

    setLayoutTextBoxNumber(this_->field_38, "txt_pg01", count);
    setLayoutTextBoxNumber(this_->field_38, "txt_pg00", 1);
}

// us-80258748
// File-load completion callback for the three collepedia archives:
//  - field_24: main menu arc -> builds the full layout, fonts, animations,
//    cursors (+0x54/+0x6C/+0x84) and the second-page info record (+0x28EC).
//  - field_28: detail-page arc -> resource accessor only.
//  - field_2C: common BDAT archive -> releases the tables into the two
//    global message-file pointers.
bool CCollepedia::OnFileEvent(CEventFile* pEventFile) {
    if (field_24 == pEventFile->mFileHandle) {
        UnkClass_8045F564* memRegion = &mRegion0;
        int mem2 = (int)getHandleMEM2__Q23mtl10MemManagerFv();
        memRegion->createRegion(mem2, 0x8000,
                                "CCollepedia", 0);

        Class_8045F858 regionGuard(memRegion);

        // Detach the arc payload from the handle and attach it to the layout
        // resource accessor.
        void* fileData = reinterpret_cast<CFileHandle*>(field_24)->getData();
        mtl::MemManager::setMemInitFlag(false);

        void* tagMem = mtl::MemManager::allocate(
            CLibLayout::getAllocHandle(), 0x858);
        if (tagMem != NULL) {
            tagMem = __ct__CTagProcessor(tagMem);
        }
        field_4C = (u32)tagMem;

        field_30 = CLibLayout::createArcResourceAccessor();
        field_30->Attach(fileData, "arc");

        buildLayout(&field_38, field_30, "mf55_sys01_cl00.brlyt");
        bindLayoutAnimTransform(field_38, &field_3C, field_30, "mf55_sys01_cl00_in.brlan");
        bindLayoutAnimTransform(field_38, &field_40, field_30, "mf55_sys01_cl00_in_info.brlan");
        bindLayoutAnimTransform(field_38, &field_44, field_30, "mf55_sys01_cl00_compall.brlan");

        // Bind the device font into every text pane via the root pane.
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, field_38);
        u32 fontHandle = (u32)((IDeviceFontInfo*)fontObj)->getFont();
        ApplyFontToTxtPanes(field_38->GetRootPane(), (const nw4r::ut::Font*)fontHandle);

        char* tagStr = (char*)getPackedFont();
        setLayoutTextBoxFont(field_38, "txt_pg00", (u32)tagStr);
        setLayoutTextBoxFont(field_38, "txt_pg01", (u32)tagStr);
        setLayoutTextBoxFont(field_38, "txt_cmp", (u32)tagStr);

        field_38->SetAnimationEnable(field_40, false);
        field_38->SetAnimationEnable(field_44, false);
        field_38->SetAnimationEnable(field_3C, true);
        field_38->Animate(0);

        char* titleText =
            BdatTouchStringCell("MNU_collect", "name", 0xa);
        LayoutSetTextBoxFmtValue(field_38, "txt_cmptit", titleText, 0);

        // Install our tag processor on the detail text-box pane.
        nw4r::lyt::Pane* textBox =
            field_38->GetRootPane()->FindPaneByName("txt_comment", true);
        if (textBox != NULL) {
            static_cast<nw4r::lyt::TextBox*>(textBox)->SetTagProcessor(
                reinterpret_cast<nw4r::ut::WideTagProcessor*>(tagMem));
        }

        // Stack-build the +0x6C cursor, copy its body (everything after the
        // vtable pointer), refresh its position, then mark it active.
        mClpCur = CCLPCur(field_30);
        mClpCur.initLayout();
        mClpCur.mActive = 1;

        // Second-page info record built from a stack temporary.
        CLPPageInfo pageInfo;
        clpInitPgInfo(&pageInfo, field_30);
        mPageInfo = pageInfo;

        clpSetupDetPg(&mPageInfo);

        // +0x84 cursor (CCur18): stack-build, copy, refresh.
        mCur18 = CCur18(CUICfManager_getArcResourceAccessor());
        mCur18.initLayout();

        // +0x54 cursor (CCur07): same pattern.
        mCur07 = CCur07(field_30);
        mCur07.initLayout();

        clpTryShow(this);
        field_24 = nullptr;
        memRegion->validateHeap();
        return true;
    }

    if (field_28 == pEventFile->mFileHandle) {
        UnkClass_8045F564* memRegion = &mRegion1;
        int mem2 = (int)getHandleMEM2__Q23mtl10MemManagerFv();
        memRegion->createRegion(mem2, 0x100,
                                "CCollepediaTex", 0);

        Class_8045F858 regionGuard(memRegion);

        void* fileData = reinterpret_cast<CFileHandle*>(field_28)->getData();
        mtl::MemManager::setMemInitFlag(false);

        field_34 = CLibLayout::createArcResourceAccessor();
        field_34->Attach(fileData, "arc");

        clpTryShow(this);
        field_28 = nullptr;
        memRegion->validateHeap();
        return true;
    }

    if (field_2C == pEventFile->mFileHandle) {
        void* data = reinterpret_cast<CFileHandle*>(field_2C)->getData();
        CBdat::setBdatEntry(2, data);

        // Resolve the two message files used by the item/quest name lookups.
        Bdat_GetTable_AA34();
        gpClpItemMsgTable = getFP("MNU_item_mes_b");
        Bdat_GetTable_AA34();
        gpClpCollectMsgTable = (char*)getFP("MNU_col");

        clpTryShow(this);
        field_2C = nullptr;
        return true;
    }

    return false;
}

void* gpClpItemMsgTable;
char* gpClpCollectMsgTable;
