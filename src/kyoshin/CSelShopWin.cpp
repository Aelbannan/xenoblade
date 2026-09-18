// Catalog TU for kyoshin/CSelShopWin.
//
// Definitions are ordered to match retail .text layout (ctor, dtor, ...,
// isShopWinLoaded, ..., getShopWinPanePos, advanceShopWinOpen, rewindShopWinClose, ...).
// driveShopWinAnim calls advanceShopWinOpen/rewindShopWinClose which are defined AFTER it,
// so -inline auto leaves those as real `bl` calls.

#include "kyoshin/CSelShopWin.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

// US retail keeps these func_8022* symbols unmangled (C linkage) even though
// they operate on a CSelShopWin; only ctor/dtor/OnFileEvent stay C++ mangled.
extern "C" void advanceShopWinOpen(CSelShopWin* self);
extern "C" void rewindShopWinClose(CSelShopWin* self);

// Opaque overlay structs used by OnFileEvent (retail layout unknown).
struct ShopPaneInfo {  // extends nw4r::lyt::Pane field we touch
    u8 gap[0xBB];
    u8 field_BB;  // 0xBB
};
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

/* Read-only views over opaque retail objects. Declaring the slots as real
   virtuals makes MWCC emit the canonical virtual-call sequence (lwz r12,0(r3);
   lwz r12,off(r12); bctrl) instead of an indirect call through a caller-saved
   register copy. Never constructed, so no vtables are emitted here. */
struct ShopFontView {
    virtual void vfn_04();
    virtual void vfn_08();
    virtual void vfn_0C();
    virtual void vfn_10();
    virtual void vfn_14();
    virtual void vfn_18();
    virtual void vfn_1C();
    virtual u32 getSpacing();  // +0x24
};
struct ShopPlayerView {
    virtual void vfn_04();
    virtual void vfn_08();
    virtual void vfn_0C();
    virtual void vfn_10();
    virtual void vfn_14();
    virtual void vfn_18();
    virtual void vfn_1C();
    virtual void vfn_20();
    virtual void vfn_24();
    virtual void vfn_28();
    virtual void vfn_2C();
    virtual void vfn_30();
    virtual void vfn_34();
    virtual void vfn_38();
    virtual void vfn_3C();
    virtual void vfn_40();
    virtual void vfn_44();
    virtual void* getMenuSource();  // +0x4c
};
struct ShopActorView {
    virtual void vfn_04();
    virtual void vfn_08();
    virtual void vfn_0C();
    virtual void vfn_10();
    virtual void vfn_14();
    virtual void vfn_18();
    virtual void vfn_1C();
    virtual void vfn_20();
    virtual void vfn_24();
    virtual void vfn_28();
    virtual void vfn_2C();
    virtual void vfn_30();
    virtual void vfn_34();
    virtual void vfn_38();
    virtual const char* getName();  // +0x40
};

CSelShopWin::CSelShopWin() : CSelShopWinVtblBase(), mMemRegion() {
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
// §11162, CItemBoxGrid func_801C5158 pattern).
#pragma push
#pragma optimize_for_size on
CSelShopWin::~CSelShopWin() {
}
#pragma pop

/* Start loading the shop window arc resources. */
void CSelShopWin::loadShopWinArc() {
    mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), lbl_eu_8050A62C,
        reinterpret_cast<IWorkEvent*>(this), 0, 0);
    mIsLoaded = 0;
}

/* Drive the open/close animation once per frame while the layout is built.
   mLayout->Animate advances the bound animation transforms. */
void CSelShopWin::driveShopWinAnim() {
    if (mIsLayoutBuilt == 0) return;
    switch (mAnimState) {
        case 1:
            advanceShopWinOpen(this);
            break;
        case 3:
            rewindShopWinClose(this);
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
u8 CSelShopWin::isShopWinLoaded() {
    return mIsLoaded;
}

/* Returns whether a show/hide animation is currently active.
   (C-linkage: isShopWinAnimActive) */
u8 CSelShopWin::isShopWinAnimActive() {
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
translates via func_80137924). C-linkage (retail unmangled getShopWinPanePos).
   Each GetRootPane() is a fresh load, matching retail's per-use reload. */
extern "C" void getShopWinPanePos(nw4r::math::VEC3* pOutPos, CSelShopWin* window, int idx) {
    char buf[0x28];
    sprintf(buf, &lbl_eu_8050A62C[0x17], idx + 1);
    nw4r::lyt::Pane* pane1 = window->mLayout->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = window->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A62C[0x20], true);
    func_80137924(pOutPos, pane1, pane2, window->mLayout->GetRootPane());
}
#pragma pop

#pragma push
#pragma auto_inline off
extern "C" void advanceShopWinOpen(CSelShopWin* self) {
    float f = lbl_eu_80668600;
    if (advanceAnimTransform(self->mAnimTransform, f) != 0) {
        self->mAnimState = 2;
        self->mAnimActive = 1;
    }
}

extern "C" void rewindShopWinClose(CSelShopWin* self) {
    float f = lbl_eu_80668600;
    if (AnimRewindFrame(self->mAnimTransform, f) != 0) {
        self->mAnimState = 0;
        self->mAnimActive = 1;
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
        mMemRegion.createRegion(handle, 0x8000, &lbl_eu_8050A62C[0x2f], 0);
        Class_8045F858 sp8(&mMemRegion);
        void* archive = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);
        mAccessor = CLibLayout::createArcResourceAccessor();
        mAccessor->Attach(archive, &lbl_eu_8050A62C[0x3b]);
        buildLayout(&mLayout, mAccessor, &lbl_eu_8050A62C[0x3f]);
        bindLayoutAnimTransform(mLayout, &mAnimTransform, mAccessor, &lbl_eu_8050A62C[0x58]);

        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        ShopFontView* fontObj =
            (ShopFontView*)getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout);
        u32 fontResult = fontObj->getSpacing();
        func_8013676C(rootPane, fontResult);

        mLayout->SetAnimationEnable(mAnimTransform, 1);
        mLayout->Animate(0);

        // Clear bit 0 of the info pane's byte at +0xBB.
        ShopPaneInfo* info = reinterpret_cast<ShopPaneInfo*>(
            mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A62C[0x74], true));
        info->field_BB &= 0xfe;

        char* text = NULL;
        ShopPlayerView* player = (ShopPlayerView*)getPlayer__Q22cf13CfGameManagerFi(0);
        if (player != NULL) {
            void* sub = player->getMenuSource();
            if (sub != NULL) {
                ShopActorView* srcActor = (ShopActorView*)findObjectById(reinterpret_cast<int>(sub));
                if (srcActor != NULL) {
                    text = static_cast<char*>(func_80138DA4(srcActor->getName()));
                }
            }
        }

        LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A62C[0x7e], text, 0);
        {
            char* t = BdatTouchStringCell(&lbl_eu_8050A62C[0x8b], &lbl_eu_8050A62C[0x94], 8);
            LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A62C[0x99], t, 0);
        }
        {
            char* t = BdatTouchStringCell(&lbl_eu_8050A62C[0x8b], &lbl_eu_8050A62C[0x94], 9);
            LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A62C[0xa0], t, 0);
        }
        {
            char* t = BdatTouchStringCell(&lbl_eu_8050A62C[0x8b], &lbl_eu_8050A62C[0x94], 10);
            LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A62C[0xa7], t, 0);
        }
        char* t2b = BdatTouchStringCell(&lbl_eu_8050A62C[0xae], &lbl_eu_8050A62C[0xbc], 0x2b);
        LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A62C[0xc1], t2b, 0);
        LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A62C[0xcd], t2b, 0);

        // Ternary keeps both format strings as immediate addi offsets
        // (retail preloads 0xe2 then overwrites with 0xd9 when nonzero).
        u16 val = static_cast<u16>(BdatGetU16ByTableKey(
            &lbl_eu_8050A62C[0xae],
            isClassicController__Q22cf13CfGameManagerFv(-1) != 0 ? &lbl_eu_8050A62C[0xd9]
                                                           : &lbl_eu_8050A62C[0xe2],
            0x2b));
        char* mssg = MakeTplNameSysFile(val);

        // Look up the persistent shop message object by its 'timg' key.
        nw4r::lyt::ArcResourceAccessor* mgr = CUICfManager_getArcResourceAccessor();
        UnkMsgObj* obj = static_cast<UnkMsgObj*>(mgr->GetResource(0x74696d67U, mssg, 0));
        if (obj != NULL) {
            PaneSetTexPaletteByName(mLayout, &lbl_eu_8050A62C[0xeb], obj);
            PaneSetTexPaletteByName(mLayout, &lbl_eu_8050A62C[0xf5], obj);

            // Two u16 counts ([+2] row, [+0] column) offsetting the two
            // panes; u32->float via the named 2^52+2^31 magic double keeps
            // the sdata2 reloc on lbl_eu_80668608.
            UnkCoords* coords = obj->chain->pCoords;
            u16 row = coords->c2;
            u16 col = coords->c0;
            nw4r::lyt::Pane* p1 = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A62C[0xeb], true);
            if (p1 != NULL) {
                // Materialize both int->double conversions up front so MWCC
                // shares one 2^52-magic lfd across the two subtractions.
                double dRow = row;
                double dCol = col;
                float src[2];
                src[0] = dRow - lbl_eu_80668608;
                src[1] = dCol - lbl_eu_80668608;
                writePanePos(p1, src);
            }
            nw4r::lyt::Pane* p2 = mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A62C[0xf5], true);
            if (p2 != NULL) {
                double dRow = row;
                double dCol = col;
                float src[2];
                src[0] = dRow - lbl_eu_80668608;
                src[1] = dCol - lbl_eu_80668608;
                writePanePos(p2, src);
            }
        }

        ::markShopWinBuilt(this);
        mFileHandle = nullptr;
        mMemRegion.func_8045F810();
        return true;
    }
    return false;
}
#pragma pop
