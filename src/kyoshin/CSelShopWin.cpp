// Catalog TU for kyoshin/CSelShopWin.
//
// Definitions are ordered to match retail .text layout (ctor, dtor, ...,
// isShopWinLoaded, ..., getShopWinPanePos, advanceShopWinOpen, rewindShopWinClose, ...).
// driveShopWinAnim calls advanceShopWinOpen/rewindShopWinClose which are defined AFTER it,
// so -inline auto leaves those as real `bl` calls.

// Inline-empty ~IWorkEvent: the ctor's cleanup action references the (link-deduped)
// weak copy, and ~CSelShopWin elides the base call (retail shape).
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/CSelShopWin.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

// US retail keeps these func_8022* symbols unmangled (C linkage) even though
// they operate on a CSelShopWin; only ctor/dtor/OnFileEvent stay C++ mangled.
// (memberized: advanceShopWinOpen -> CSelShopWin::advanceShopWinOpen; declaration moved into class CSelShopWin)
// (memberized: rewindShopWinClose -> CSelShopWin::rewindShopWinClose; declaration moved into class CSelShopWin)

// Opaque resource-object layout used by OnFileEvent.
struct UnkCoords {
    u16 c0;  // 0x0
    u16 c2;  // 0x2
};
struct UnkMsgObjChain {
    UnkCoords* pCoords;  // 0x0
};
struct UnkMsgObj {
    u8 gap[0x8];
    UnkMsgObjChain* chain;  // 0x8
};

CSelShopWin::CSelShopWin() : mMemRegion() {
    mFileHandle = nullptr;
    mAccessor = nullptr;
    mLayout = nullptr;
    mAnimTransform = nullptr;
    mIsLayoutBuilt = 0;
    mIsLoaded = 1;
    mAnimState = 0;
    mAnimActive = 1;
}

// Retail saves r30/r31 with a single stmw (block save). -O4,p emits separate
// stw pairs; optimize_for_size flips to the stmw/lmw frame (MWCC_CASES
// §11162, CItemBoxGrid PushUniqueItemId pattern).
#pragma push
#pragma optimize_for_size on
CSelShopWin::~CSelShopWin() {
}
#pragma pop

/* Start loading the shop window arc resources. */
void CSelShopWin::loadShopWinArc() {
    mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/ShopSubWin.arc",
        this, 0, 0);
    mIsLoaded = 0;
}

/* Drive the open/close animation once per frame while the layout is built.
   mLayout->Animate advances the bound animation transforms. */
void CSelShopWin::driveShopWinAnim() {
    if (mIsLayoutBuilt == 0) return;
    switch (mAnimState) {
        case 1:
            this->advanceShopWinOpen();
            break;
        case 3:
            this->rewindShopWinClose();
            break;
    }
    mLayout->Animate(0);
}

void CSelShopWin::drawShopWin(nw4r::lyt::DrawInfo* drawInfo) {
    if (mIsLayoutBuilt == 0) return;
    if (mAnimState == 0) return;
    return drawLayout(mLayout, drawInfo, 0, 1);
}

/* Tear down the shop window layout and free its resources. */
void CSelShopWin::teardownShopWin() {
    closeFileHandle(&mFileHandle);
    mIsLayoutBuilt = 0;
    if (mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
    releaseArcResourceAccessor(mAccessor);
    mMemRegion.deleteRegion();
}

/* Returns whether the shop window has finished loading resources
   and is ready for interaction. (C-linkage: isShopWinLoaded) */
int CSelShopWin::isShopWinLoaded() {
    return mIsLoaded;
}

/* Returns whether a show/hide animation is currently active.
   (C-linkage: isShopWinAnimActive) */
int CSelShopWin::isShopWinAnimActive() {
    return mAnimActive;
}

void CSelShopWin::showShopWin() {
    if (mAnimState != 0) return;
    mAnimState = 1;
    mAnimActive = 0;
    return playUISound(0xd);
}

void CSelShopWin::hideShopWin() {
    if (mAnimState != 2) return;
    mAnimState = 3;
    mAnimActive = 0;
    return playUISound(0xe);
}

#pragma push
#pragma optimize_for_size on
/* Determine the absolute screen position of the two named panes in the shop
   window layout (format a pane-name from idx+1, find both panes, sum ancestor
translates via ScalePaneWorldPos). C-linkage (retail unmangled getShopWinPanePos).
   Each GetRootPane() is a fresh load, matching retail's per-use reload. */
extern "C" void getShopWinPanePos(nw4r::math::VEC3* pOutPos, CSelShopWin* window, int idx) {
    char buf[0x28];
    sprintf(buf, "txt_%02d", idx + 1);
    nw4r::lyt::Pane* pane1 = window->mLayout->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = window->mLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
    ScalePaneWorldPos(pOutPos, pane1, pane2, window->mLayout->GetRootPane());
}
#pragma pop

#pragma push
#pragma auto_inline off
void CSelShopWin::advanceShopWinOpen() {
    float f = 1.0f;
    if (advanceAnimTransform(this->mAnimTransform, f) != 0) {
        this->mAnimState = 2;
        this->mAnimActive = 1;
    }
}

void CSelShopWin::rewindShopWinClose() {
    float f = 1.0f;
    if (AnimRewindFrame(this->mAnimTransform, f) != 0) {
        this->mAnimState = 0;
        this->mAnimActive = 1;
    }
}
#pragma pop

extern "C" void markShopWinBuilt(void* self) {
    CSelShopWin* window = static_cast<CSelShopWin*>(self);
    if (window->mLayout != 0) {
        window->mIsLoaded = 1;
        window->mIsLayoutBuilt = 1;
    }
}

/* -O4,s shape: retail saves r29-r31 via _savegpr_29/_restgpr_29 helpers. */
#pragma push
#pragma optimize_for_size on
bool CSelShopWin::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        // Build the shop window layout from the freshly-loaded arc.
        mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
        mMemRegion.createRegion(handle, 0x8000, "CSelShopWin", 0);
        Class_8045F858 sp8(&mMemRegion);
        void* archive = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(archive, "arc");
        buildLayout(&mLayout, mAccessor, "mf70_cf60_syswin05.brlyt");
        bindLayoutAnimTransform(mLayout, &mAnimTransform, mAccessor, "mf70_cf60_syswin05_in.brlan");

        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        IDeviceFontInfo* fontObj = static_cast<IDeviceFontInfo*>(
            getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout));
        nw4r::ut::Font* font = fontObj->getFont();
        ApplyFontToTxtPanes(rootPane, font);

        mLayout->SetAnimationEnable(mAnimTransform, 1);
        mLayout->Animate(0);

        nw4r::lyt::Pane* info =
            mLayout->GetRootPane()->FindPaneByName("nul_value", true);
        info->SetVisible(false);

        char* text = NULL;
        cf::CfObjectMove* player = static_cast<cf::CfObjectMove*>(
            getPlayer__Q22cf13CfGameManagerFi(0));
        if (player != NULL) {
            int actorId = player->CObjectParam_getSelfObjectId();
            if (actorId != 0) {
                cf::CfObjectMove* srcActor =
                    static_cast<cf::CfObjectMove*>(findObjectById(actorId));
                if (srcActor != NULL) {
                    text = GetBdatNameByNumId(
                        static_cast<const char*>(srcActor->CObjectParam_getParamPtr()));
                }
            }
        }

        LayoutSetTextBoxFmtValue(mLayout, "txt_titlemes", text, 0);
        {
            char* t = BdatTouchStringCell("MNU_shop", "name", 8);
            LayoutSetTextBoxFmtValue(mLayout, "txt_01", t, 0);
        }
        {
            char* t = BdatTouchStringCell("MNU_shop", "name", 9);
            LayoutSetTextBoxFmtValue(mLayout, "txt_02", t, 0);
        }
        {
            char* t = BdatTouchStringCell("MNU_shop", "name", 10);
            LayoutSetTextBoxFmtValue(mLayout, "txt_03", t, 0);
        }
        char* t2b = BdatTouchStringCell("MNU_kyeassign", "help", 0x2b);
        LayoutSetTextBoxFmtValue(mLayout, "txt_close00", t2b, 0);
        LayoutSetTextBoxFmtValue(mLayout, "txt_close01", t2b, 0);

        // Ternary keeps both format strings as immediate addi offsets
        // (retail preloads 0xe2 then overwrites with 0xd9 when nonzero).
        u16 val = static_cast<u16>(BdatGetU16ByTableKey(
            "MNU_kyeassign",
            cf::CfGameManager::isClassicController(-1) != 0 ? "fileID_2"
                                                           : "fileID_1",
            0x2b));
        char* mssg = MakeTplNameSysFile(val);

        // Look up the persistent shop message object by its 'timg' key.
        nw4r::lyt::ArcResourceAccessor* mgr = CUICfManager_getArcResourceAccessor();
        UnkMsgObj* obj = static_cast<UnkMsgObj*>(mgr->GetResource(0x74696d67U, mssg, 0));
        if (obj != NULL) {
            PaneSetTexPaletteByName(mLayout, "pic_btn00", (TPLPalette*)obj);
            PaneSetTexPaletteByName(mLayout, "pic_btn01", (TPLPalette*)obj);

            // Two u16 counts ([+2] row, [+0] column) offsetting the two
            // panes; the plain integer-to-double casts emit the 2^52 magic.
            UnkCoords* coords = obj->chain->pCoords;
            u16 row = coords->c2;
            u16 col = coords->c0;
            nw4r::lyt::Pane* p1 = mLayout->GetRootPane()->FindPaneByName("pic_btn00", true);
            if (p1 != NULL) {
                // Materialize both int->double conversions up front so MWCC
                // shares one 2^52-magic lfd across the two subtractions.
                double dRow = row;
                double dCol = col;
                float src[2];
                src[0] = dRow;
                src[1] = dCol;
                writePanePos(p1, src);
            }
            nw4r::lyt::Pane* p2 = mLayout->GetRootPane()->FindPaneByName("pic_btn01", true);
            if (p2 != NULL) {
                double dRow = row;
                double dCol = col;
                float src[2];
                src[0] = dRow;
                src[1] = dCol;
                writePanePos(p2, src);
            }
        }

        ::markShopWinBuilt(this);
        mFileHandle = nullptr;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}
#pragma pop
