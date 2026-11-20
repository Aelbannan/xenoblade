// Inline-empty ~IWorkEvent (see IWorkEvent.hpp): retail elides the empty
// base-dtor call in ~CTitleAHelp (CArcItem.cpp pattern).
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CTitleAHelp.hpp"

#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/work/CEventFile.hpp"
#include "revolution/gx/GXTypes.h"

#include <nw4r/lyt.h>
#include <nw4r/ut.h>
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"

CTitleAHelp::CTitleAHelp(char* arg1, u8 arg2) {
    mFileHandle = nullptr;
    mArcResourceAccessor = nullptr;
    mLayout = nullptr;
    mAnimTrans20 = nullptr;
    mAnimTrans24 = nullptr;
    unk28 = 0;
    unk2c = 0;
    mName = arg1;
    unk34 = arg2;
    unk35 = 0;
    unk36 = 1;
    unk37 = 0;
}






CTitleAHelp::~CTitleAHelp() {}

void CTitleAHelp::CTitleAHelp_load() {
    u32 allocHandle = mtl::MemManager::getHandleMEM2();
    mFileHandle = CDeviceFile::readFile(allocHandle, "/menu/TitleAHelp.arc", this, 0, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);
}

void CTitleAHelp::updateHelp() {
    if(unk28 == 0) return;
    switch(unk2c) {
        case 1:
            stepOpenAnim();
            break;
        case 2:
            stepShownAnim();
            break;
        case 4:
            stepCloseAnim();
            break;
        case 5:
            stepHiddenAnim(); // retail falls through to Animate (no break)
    }
    mLayout->Animate(0);
}

void CTitleAHelp::drawHelp(nw4r::lyt::DrawInfo* drawInfo) {
    if (unk28 == 0)
        return;
    drawLayout(mLayout, drawInfo, 0, 1);
}

void CTitleAHelp::teardown() {
    closeFileHandle(&mFileHandle);
    unk28 = 0;
    if(mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
    releaseArcResourceAccessor(mArcResourceAccessor);
    unk4.deleteRegion();
}

int CTitleAHelp::isInitialized() { return unk35; }

u8 CTitleAHelp::isActive() {
	return unk28;
}

int CTitleAHelp::func_801C4124() { return unk36; }

void CTitleAHelp::func_801C412C() {
    if (unk2c == 0) {
        unk2c = 1;
        unk36 = 0;
    }
}

void CTitleAHelp::beginClose() {
    if (unk2c == 3) {
        unk2c = 4;
        unk36 = 0;
    }
}

void CTitleAHelp::reopenFromClose() {
    if(unk37 != 0 && unk2c == 4) {
        unk2c = 2;
        unk36 = 0;
    }
}

void CTitleAHelp::markReplayClose() {
    if(unk2c == 3) {
        unk2c = 4;
        unk36 = 0;
        unk37 = 1;
    }
}

extern char lbl_eu_805054BC[];
void CTitleAHelp::setNameText(char* arg) {
    if(mLayout == nullptr) return;
    LayoutSetTextBoxFmtValue(mLayout, lbl_eu_805054BC + 0x17, arg, 0);
}

// Lays out the title-A help screen: swaps button textures by bdat key assign
// and re-flows help text boxes left-to-right with the shared icon width.
void CTitleAHelp::func_801C41E8(u8 arg) {
    if (mLayout == nullptr) return;

    HelpRow* row = &lbl_eu_80533E60[arg];
    char buf1[0x20];
    char buf2[0x20];

    for (u8 i = 0; i < 6; i++) {
        sprintf(buf1, lbl_eu_805054BC + 0x1f, i);
        sprintf(buf2, lbl_eu_805054BC + 0x2b, i);

        u16 tableVal = row->cell[i];
        if (tableVal != 0) {
            setPaneVisible(mLayout->GetRootPane()->FindPaneByName(buf1, true), 1);
            setPaneVisible(mLayout->GetRootPane()->FindPaneByName(buf2, true), 1);

            const char* fileID = lbl_eu_805054BC + 0x37;
            if (cf::CfGameManager::isClassicController(-1) != 0) {
                fileID = lbl_eu_805054BC + 0x40;
            }

            u16 keyVal = BdatGetU16ByTableKey(lbl_eu_805054BC + 0x49, fileID, tableVal);
            char* texName = MakeTplNameSysFile(keyVal);

            nw4r::lyt::ArcResourceAccessor* accessor = ::CUICfManager_getArcResourceAccessor();
            void* resource = accessor->GetResource(
                nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, texName, NULL);

            if (resource != nullptr) {
                PaneSetTexPaletteByName(mLayout, (const char*)buf1, (TPLPalette*)resource);

                nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(buf1, true);
                if (pane != nullptr) {
                    // Resource -> pointer table -> image header (height @0, width @2).
                    u32* texPtr = *(u32**)((u8*)resource + 8);
                    u16 width = *(u16*)(*texPtr + 2);
                    u16 height = *(u16*)(*texPtr + 0);
                    pane->SetSize(nw4r::lyt::Size((f32)width, (f32)height));
                }
            }

            char* helpText = BdatTouchStringCell(lbl_eu_805054BC + 0x49, lbl_eu_805054BC + 0x57, tableVal);
            LayoutSetTextBoxFmtValue(mLayout, buf2, helpText, 0);
        } else {
            setPaneVisible(mLayout->GetRootPane()->FindPaneByName(buf1, true), 0);
            setPaneVisible(mLayout->GetRootPane()->FindPaneByName(buf2, true), 0);
        }
    }

    // Second pass: flow the text boxes left to right, placing each button icon
    // just left of its text and advancing by measured text + icon width.
    u16 someWidth = 0;
    nw4r::math::VEC3 oldVec;
    nw4r::math::VEC3 newVec;
    char buf3[0x20];
    char buf4[0x20];

    for (u8 i = 0; i < 6; i++) {
        sprintf(buf3, lbl_eu_805054BC + 0x2b, i);

        nw4r::lyt::TextBox* textBox = (nw4r::lyt::TextBox*)mLayout->GetRootPane()->FindPaneByName(buf3, true);
        if (!isPaneVisible(textBox)) return;

        if (i == 0) {
            copyVEC3(&oldVec, &textBox->GetTranslate());
        }

        newVec = oldVec;
        newVec.x -= (f32)someWidth;
        copyVEC3((void*)&textBox->GetTranslate(), &newVec);

        nw4r::ut::Font* font = const_cast<nw4r::ut::Font*>(textBox->GetFont());
        const wchar_t* text = textBox->GetString();

        u8 j = 0;
        while (text[j] != 0) {
            someWidth += (u16)font->GetCharWidth(text[j]);
            j++;
        }
        someWidth -= 4;

        sprintf(buf4, lbl_eu_805054BC + 0x1f, i);
        nw4r::lyt::Pane* picPane = mLayout->GetRootPane()->FindPaneByName(buf4, true);

        copyVEC3(&newVec, &oldVec);
        newVec.x -= (f32)someWidth;
        copyVEC3((void*)&picPane->GetTranslate(), &newVec);

        someWidth = (int)((f32)someWidth + picPane->GetSize().width);

        if (i < 5) {
            sprintf(buf3, lbl_eu_805054BC + 0x2b, i + 1);
            nw4r::lyt::TextBox* nextPane = (nw4r::lyt::TextBox*)mLayout->GetRootPane()->FindPaneByName(buf3, true);
            if (!isPaneVisible(nextPane)) return;

            wchar_t firstChar = nextPane->GetString()[0];
            if (firstChar != 0x2b && firstChar != 0xff0b) {
                someWidth += 0x10;
            }
        } else {
            someWidth += 0x10;
        }
    }
}

void CTitleAHelp::func_801C4654(u32 arg) {
    if(mLayout == nullptr) return;
    nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(lbl_eu_805054BC + 0x5c, true);
    setPaneVisible(pane, arg);
}

void CTitleAHelp::setInfoText(char* arg) {
    if(mLayout == nullptr) return;
    LayoutSetTextBoxFmtValue(mLayout, lbl_eu_805054BC + 0x64, arg, 0);
}

void CTitleAHelp::func_801C46DC(u32 arg) {
    if(mLayout == nullptr) return;
    nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(lbl_eu_805054BC + 0x70, true);
    setPaneVisible(pane, arg);
}

void CTitleAHelp::setActive(u8 arg) {
    unk28 = arg;
}

// retail: r4 = lbl_eu_805054BC+0x17 (string), r5/r6 = &lbl_eu_80664468/70 (.sbss addrs)
extern char lbl_eu_805054BC[];
extern u32 lbl_eu_80664468;
extern u32 lbl_eu_80664470;
extern u32 lbl_eu_80664478;
extern u32 lbl_eu_80664480;
void CTitleAHelp::applyPaneTevColors() {
    PaneMatSetTevColorsByName(mLayout, lbl_eu_805054BC + 0x17, (const GXColorS10*)&lbl_eu_80664468, (const GXColorS10*)&lbl_eu_80664470);
}

void CTitleAHelp::applyPaneTevColorsAlt() {
    PaneMatSetTevColorsByName(mLayout, lbl_eu_805054BC + 0x17, (const GXColorS10*)&lbl_eu_80664478, (const GXColorS10*)&lbl_eu_80664480);
}

void CTitleAHelp::stepOpenAnim() {
    if(advanceAnimTransform(mAnimTrans20, 1.0f)) {
        mLayout->SetAnimationEnable(mAnimTrans20, 0);
        mLayout->SetAnimationEnable(mAnimTrans24, 1);
        unk2c = 2;
    }
}

void CTitleAHelp::stepShownAnim() {
    if(advanceAnimTransform(mAnimTrans24, 1.0f)) {
        unk2c = 3;
        unk36 = 1;
        unk37 = 0;
    }
}

void CTitleAHelp::stepCloseAnim() {
    if(AnimRewindFrame(mAnimTrans24, 1.0f)) {
        if(unk37 == 0) {
            mLayout->SetAnimationEnable(mAnimTrans24, 0);
            mLayout->SetAnimationEnable(mAnimTrans20, 1);
            unk2c = 5;
        } else {
            unk36 = 1;
        }
    }
}

void CTitleAHelp::stepHiddenAnim() {
    if(AnimRewindFrame(mAnimTrans20, 1.0f)) {
        unk2c = 0;
        unk36 = 1;
    }
}

// Retail emits an stmw frame here (-O4,s unit).
bool CTitleAHelp::OnFileEvent(CEventFile* pEventFile) {
    if(mFileHandle == pEventFile->mFileHandle) {
        u32 allocHandle = mtl::MemManager::getHandleMEM2();
        unk4.createRegion(allocHandle, 0x3200, &lbl_eu_805054BC[0x7c], 1);

        Class_8045F858 memRegion(&unk4);
        void* data = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);

        mArcResourceAccessor = CLibLayout::createArcResourceAccessor();
        mArcResourceAccessor->Attach(data, &lbl_eu_805054BC[0x88]);

        buildLayout(&mLayout, mArcResourceAccessor, &lbl_eu_805054BC[0x8c]);
        bindLayoutAnimTransform(mLayout, &mAnimTrans20, mArcResourceAccessor, &lbl_eu_805054BC[0x9f]);
        bindLayoutAnimTransform(mLayout, &mAnimTrans24, mArcResourceAccessor, &lbl_eu_805054BC[0xb5]);

        // Retail loads the root pane (layout + 0x10) before the font call and
        // keeps it live across ApplyFontToTxtPanes.
        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        u32 fontHandle = static_cast<CTitleAHelpFontView*>(
            CDeviceFont::getFontInfo(1, mLayout))->vf7();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontHandle);

        mLayout->SetAnimationEnable(mAnimTrans24, false);
        mLayout->SetAnimationEnable(mAnimTrans20, true);
        mLayout->Animate(0);

        // Hide the caution pane; retail re-reads layout+0x10 here rather than
        // reusing the root pane held for ApplyFontToTxtPanes.
        setPaneVisible(mLayout->GetRootPane()->FindPaneByName(&lbl_eu_805054BC[0x70], true), false);

        setNameText(mName);
        func_801C41E8(unk34);
        unk35 = 1;
        unk28 = 1;
        mFileHandle = nullptr;

        unk4.validateHeap();
        return true;
    }
    return false;
}

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
bool isPaneVisible(nw4r::lyt::Pane* self) {
    const u8* bytes = reinterpret_cast<const u8*>(self);
    return (bytes[0xBB] & 1) != 0;
}

void sinit_801C4AE4() {
    setGXColorS10(reinterpret_cast<GXColorS10*>(&lbl_eu_80664468), 0xa8, 0x52, 0x08, 0x00);
    setGXColorS10(reinterpret_cast<GXColorS10*>(&lbl_eu_80664470), 0xa8, 0x52, 0x08, 0xff);

    setGXColorS10(reinterpret_cast<GXColorS10*>(&lbl_eu_80664478), 0x05, 0x80, 0xa6, 0x00);
    setGXColorS10(reinterpret_cast<GXColorS10*>(&lbl_eu_80664480), 0x05, 0x80, 0xa6, 0xff);
}

GXColorS10* setGXColorS10(GXColorS10* color, s16 r, s16 g, s16 b, s16 a) {
    color->a = a;
    color->b = b;
    color->g = g;
    color->r = r;
    return color;
}


