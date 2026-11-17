#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CExchangeWin
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <stdio.h>
#include <revolution/tpl/TPL.h>
#include "kyoshin/CExchangeWin.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
extern void playUISound(u32);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void* region);



CExchangeWin::CExchangeWin() {
    mFileHandle = nullptr;
    mAccessor = nullptr;
    mLayout = nullptr;
    mAnimTransform = nullptr;
    field_24 = 0;
    field_25 = 1;
    _26 = 0;
    field_27 = 1;
}

// Destructor - compiler handles member destructors automatically.
// The explicit mMemRegion.~UnkClass_8045F564() call is NOT added here
// because MWCC generates it automatically for non-trivial members,
// and adding it explicitly causes a double call.
CExchangeWin::~CExchangeWin() {
}

void CExchangeWin::ExWin_LoadFile() {
    mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(),
        lbl_eu_8050A740,
        this,
        0,
        0
    );
    field_25 = 0;
}

// ExWin_TickUpdate - update loop: drives animation state machine and calls
// mLayout->Animate(0) when field_24 is set. Dispatches to ExWin_AdvanceEnter
// for _26==1 (entering) and ExWin_AdvanceExit for _26==3 (exiting).
void CExchangeWin::ExWin_TickUpdate() {
    if (field_24 == 0) {
        return;
    }
    switch (_26) {
    case 1:
        ExWin_AdvanceEnter();
        break;
    case 3:
        ExWin_AdvanceExit();
        break;
    }
    mLayout->Animate(0);
}

void CExchangeWin::ExWin_DrawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (field_24 == 0) {
        return;
    }
    if (_26 == 0) {
        return;
    }
    drawLayout(mLayout, drawInfo, 0, 1);
}

// ExWin_Teardown - teardown: releases file handle, destroys the layout,
// releases the arc resource accessor, and cleans up the memory region.
void CExchangeWin::ExWin_Teardown() {
    closeFileHandle(&mFileHandle);
    field_24 = 0;
    if (mLayout != NULL) {
        delete mLayout;
        mLayout = NULL;
    }
    releaseArcResourceAccessor(mAccessor);
    deleteRegion__17UnkClass_8045F564Fv(&mMemRegion);
}

int CExchangeWin::getField25() { return field_25; }

int CExchangeWin::getField24() { return field_24; }

int CExchangeWin::getField27() { return field_27; }

// If _26 is already non-zero, do nothing; otherwise initialize state and fire event 0xd
void CExchangeWin::ExWin_RequestOpen() {
    if (_26 != 0) {
        return;
    }
    _26 = 1;
    field_27 = 0;
    field_24 = 1;
    playUISound(0xd);
}

void CExchangeWin::ExWin_RequestClose() {
    if (_26 != 2) {
        return;
    }
    _26 = 3;
    field_27 = 0;
    playUISound(0xe);
}

// Pane world position of option `val` scaled into the caller's space. The
// destination is the caller's VEC3 (retail r3), the window is r4, so this
// stays a free function.
extern "C" void ExWin_CopyPanePair(void* dst, CExchangeWin* src, u8 val) {
    char buf[40];
    CExchangeWin* win;
    nw4r::lyt::Pane* pane1;
    nw4r::lyt::Pane* pane2;

    win = src;
    sprintf(buf, &lbl_eu_8050A740[0x18], val + 1);
    pane1 = win->mLayout->GetRootPane()->FindPaneByName(buf, true);
    pane2 = win->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A740[0x25], true);
    ScalePaneWorldPos((nw4r::math::VEC3*)dst, pane1, pane2, win->mLayout->GetRootPane());
}

void CExchangeWin::ExWin_SetTwoTexts(char* text1, char* text2) {
    LayoutSetTextBoxFmtValue(mLayout, (char*)&lbl_eu_8050A740[0x34], text1, 0);
    LayoutSetTextBoxFmtValue(mLayout, (char*)&lbl_eu_8050A740[0x41], text2, 0);
}

__attribute__((noinline)) void CExchangeWin::ExWin_AdvanceEnter() {
    float f = 1.0f;
    if (advanceAnimTransform(mAnimTransform, f)) {
        _26 = 2;
        field_27 = 1;
    }
}

void CExchangeWin::ExWin_AdvanceExit() {
    float f = 1.0f;
    if (AnimRewindFrame(mAnimTransform, f)) {
        _26 = 0;
        field_27 = 1;
        field_24 = 0;
    }
}

// OnFileEvent - loads the layout from a file, sets up fonts, animations,
// text fields, and a texture resource with pane sizing from TPL dimensions.
bool CExchangeWin::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        mMemRegion.createRegion(
            mtl::MemManager::getHandleMEM2(),
            0x8000,
            &lbl_eu_8050A740[0x4e],
            0
        );

        // Locals are declared in reverse stack-slot order (MWCC assigns slots
        // in reverse declaration order) so the layout matches retail:
        // size@0x10, regionGuard@0x8.
        CExchangeWinSize size;
        Class_8045F858 regionGuard(&mMemRegion);

    void* arcData = mFileHandle->mData;
    mFileHandle->mData = nullptr;
    mtl::MemManager::setMemInitFlag(false);

    mAccessor = CLibLayout::createArcResourceAccessor();
    mAccessor->Attach(arcData, &lbl_eu_8050A740[0x5b]);

    buildLayout(&mLayout, mAccessor, &lbl_eu_8050A740[0x5f]);
    bindLayoutAnimTransform(mLayout, &mAnimTransform, mAccessor,
                  &lbl_eu_8050A740[0x78]);

    // Bind the shared font: root pane first (retail loads it into r29 before
    // the CDeviceFont call), then fetch the font handle from vtable slot 9.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    void* fontObj = CDeviceFont::getFontInfo(1, mLayout);
    u32 fontResult = static_cast<CExchangeWinFontView*>(fontObj)->sf9();
    ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

    mLayout->SetAnimationEnable(mAnimTransform, true);
    mLayout->Animate(0);

    // Set text fields from message table
    char* t1 = BdatTouchStringCell(&lbl_eu_8050A740[0x94], &lbl_eu_8050A740[0x9d], 0x24);
    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A740[0xa2], t1, 0);

    char* t2 = BdatTouchStringCell(&lbl_eu_8050A740[0x94], &lbl_eu_8050A740[0x9d], 0x25);
    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A740[0xaf], t2, 0);

    char* t3 = BdatTouchStringCell(&lbl_eu_8050A740[0x94], &lbl_eu_8050A740[0x9d], 0x26);
    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A740[0xba], t3, 0);

    char* t4 = BdatTouchStringCell(&lbl_eu_8050A740[0xc5], &lbl_eu_8050A740[0xd3], 0x2b);
    LayoutSetTextBoxFmtValue(mLayout, &lbl_eu_8050A740[0xd8], t4, 0);

    // Choose the message texture name by the game-manager mode flag (ternary
    // keeps the selection after the call so MWCC loads it straight into r4).
    const char* fileID = isClassicController__Q22cf13CfGameManagerFv(-1) != 0
                             ? &lbl_eu_8050A740[0xe4]
                             : &lbl_eu_8050A740[0xed];

    u16 keyVal = BdatGetU16ByTableKey(&lbl_eu_8050A740[0xc5], fileID, 0x2b);
    char* name = MakeTplNameSysFile(keyVal);

    nw4r::lyt::ArcResourceAccessor* resAcc = CUICfManager_getArcResourceAccessor();
    TPLPalette* resource =
        (TPLPalette*)resAcc->GetResource(0x74696D67, name, 0);

    if (resource != nullptr) {
        PaneSetTexPaletteByName(mLayout, &lbl_eu_8050A740[0xf6], resource);

        // The TPL dims are captured before the pane lookup: retail keeps them
        // in callee-saved r29/r30 across the FindPaneByName call.
        TPLHeader* header = resource->descriptorArray->textureHeader;
        u16 w = header->width;
        u16 h = header->height;
        nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(
            &lbl_eu_8050A740[0xf6], true);
        if (pane != nullptr) {
            // u16 -> f32 via the shared 0x43300000 magic double, then size
            // the picture pane (CfTFile.cpp / CMenuKizunaTalk convention).
            size.width = (f32)(u32)w;
            size.height = (f32)(u32)h;
            pane->SetSize(*reinterpret_cast<nw4r::lyt::Size*>(&size));
        }
    }

    if (mLayout != nullptr) {
        field_25 = 1;
    }

    mFileHandle = nullptr;
    mMemRegion.validateHeap();

    return true;
    }
    return false;
}
