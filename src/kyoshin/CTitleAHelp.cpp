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

extern void func_80124270(nw4r::lyt::Pane*, u32);
extern void copyVEC3(nw4r::math::VEC3*, nw4r::math::VEC3*);

// Retail keeps these state-machine helpers unmangled (C linkage); declare
// them extern "C" so the switch in func_801C3FF0 emits the retail relocs.
extern "C" void func_801C477C(CTitleAHelp*);
extern "C" void func_801C47F8(CTitleAHelp*);
extern "C" void func_801C484C(CTitleAHelp*);
extern "C" void func_801C48E0(CTitleAHelp*);

extern u16 lbl_eu_80533E60[];

// Retail constructor symbol (extern "C" to avoid MWCC mangling to __ct__11CTitleAHelpFPcUc)
#pragma optimize_for_size on
// Retail dtor shares the ctor's optimize_for_size region (stmw frame).
CTitleAHelp::~CTitleAHelp() {}
extern "C" CTitleAHelp* __ct__CTitleAHelp(CTitleAHelp* self, char* arg1, u8 arg2) {
    extern void* lbl_eu_80534500[];
    extern void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);

    *(const void**)self = lbl_eu_80534500;
    __ct__17UnkClass_8045F564Fv(&self->unk4);
    self->mFileHandle = nullptr;
    self->mArcResourceAccessor = nullptr;
    self->mLayout = nullptr;
    self->mAnimTrans20 = nullptr;
    self->mAnimTrans24 = nullptr;
    self->unk28 = 0;
    self->unk2c = 0;
    self->mName = arg1;
    self->unk34 = arg2;
    self->unk35 = 0;
    self->unk36 = 1;
    self->unk37 = 0;
    return self;
}
#pragma optimize_for_size off

void CTitleAHelp::CTitleAHelp_load() {
    u32 allocHandle = mtl::MemManager::getHandleMEM2();
    mFileHandle = CDeviceFile::readFile(allocHandle, "/menu/TitleAHelp.arc", this, 0, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);
}

void CTitleAHelp::func_801C3FF0() {
    if(unk28 == 0) return;
    switch(unk2c) {
        case 1:
            func_801C477C(this);
            break;
        case 2:
            func_801C47F8(this);
            break;
        case 4:
            func_801C484C(this);
            break;
        case 5:
            func_801C48E0(this); // retail falls through to Animate (no break)
    }
    mLayout->Animate(0);
}

void CTitleAHelp::func_801C4080(nw4r::lyt::DrawInfo* drawInfo) {
    if (unk28 == 0)
        return;
    func_80137038(mLayout, drawInfo, 0, 1);
}

void CTitleAHelp::func_801C40A0() {
    func_801390E0(&mFileHandle);
    unk28 = 0;
    if(mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
    func_80139124(mArcResourceAccessor);
    unk4.func_8045F778();
}

u8 CTitleAHelp::func_801C4114() { return unk35; }

u8 CTitleAHelp::func_801C411C() {
	return unk28;
}

u8 CTitleAHelp::isIdle() { return unk36; }

void CTitleAHelp::func_801C412C() {
    if (unk2c == 0) {
        unk2c = 1;
        unk36 = 0;
    }
}

void CTitleAHelp::func_801C414C() {
    if (unk2c == 3) {
        unk2c = 4;
        unk36 = 0;
    }
}

void CTitleAHelp::func_801C416C() {
    if(unk37 != 0 && unk2c == 4) {
        unk2c = 2;
        unk36 = 0;
    }
}

void CTitleAHelp::func_801C4198() {
    if(unk2c == 3) {
        unk2c = 4;
        unk36 = 0;
        unk37 = 1;
    }
}

extern char lbl_eu_805054BC[];
extern "C" void func_801C41C0(CTitleAHelp* self, char* arg) {
    if(self->mLayout == nullptr) return;
    func_80136B4C(self->mLayout, lbl_eu_805054BC + 0x17, arg, 0);
}

void CTitleAHelp::func_801C41E8(u8 arg) {
    if (mLayout == nullptr) return;

    u16* row = &lbl_eu_80533E60[arg * 7];

    for (u8 i = 0; i < 6; i++) {
        char buf1[0x20];
        sprintf(buf1, "pic_btn%02d", i);
        char buf2[0x20];
        sprintf(buf2, "txt_hlp%02d", i);

        u16 tableVal = row[i];
        if (tableVal != 0) {
            nw4r::lyt::Pane* root = mLayout->GetRootPane();
            func_80124270(root->FindPaneByName(buf1, true), 1);
            func_80124270(root->FindPaneByName(buf2, true), 1);

            const char* fileID = "fileID_1";
            if (cf::CfGameManager::func_80086F9C(-1) != 0) {
                fileID = "fileID_2";
            }

            u16 keyVal = func_8013606C("MNU_kyeassign", (char*)fileID, tableVal);
            char* name = func_80138F78(keyVal);

            nw4r::lyt::ArcResourceAccessor* resAcc = CUICfManager::func_801355F4();
            void* resource = resAcc->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, name, nullptr);

            if (resource != nullptr) {
                func_80137E7C(mLayout, buf1, resource);

                nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(buf1, true);
                if (pane != nullptr) {
                    u32* texPtr = *(u32**)((u8*)resource + 8);
                    u16 width = *(u16*)(*texPtr + 2);
                    u16 height = *(u16*)(*texPtr + 0);
                    pane->SetSize(nw4r::lyt::Size((f32)width, (f32)height));
                }
            }

            char* helpText = (char*)func_80136190("MNU_kyeassign", "help", tableVal);
            func_80136B4C(mLayout, buf2, helpText, 0);
        } else {
            nw4r::lyt::Pane* root = mLayout->GetRootPane();
            func_80124270(root->FindPaneByName(buf1, true), 0);
            func_80124270(root->FindPaneByName(buf2, true), 0);
        }
    }

    u16 someWidth = 0;
    nw4r::math::VEC3 oldVec;

    for (u8 i = 0; i < 6; i++) {
        char buf3[0x20];
        sprintf(buf3, "txt_hlp%02d", i);

        nw4r::lyt::TextBox* textBox = (nw4r::lyt::TextBox*)mLayout->GetRootPane()->FindPaneByName(buf3, true);
        if (!func_801C4648(textBox)) return;

        if (i == 0) {
            oldVec = textBox->GetTranslate();
        }

        nw4r::math::VEC3 newVec = oldVec;
        newVec.x -= (f32)someWidth;
        textBox->SetTranslate(newVec);

        nw4r::ut::Font* font = (nw4r::ut::Font*)textBox->GetFont();
        const wchar_t* str = textBox->GetString();

        u8 j = 0;
        while (str[j] != 0) {
            someWidth += (u16)font->GetCharWidth(str[j]);
            j++;
        }
        someWidth -= 4;

        char buf4[0x20];
        sprintf(buf4, "pic_btn%02d", i);
        nw4r::lyt::Pane* picPane = mLayout->GetRootPane()->FindPaneByName(buf4, true);

        copyVEC3(&newVec, &oldVec);
        newVec.x -= (f32)someWidth;
        picPane->SetTranslate(newVec);

        someWidth = (int)((f32)someWidth + picPane->GetSize().width);

        if (i < 5) {
            sprintf(buf3, "txt_hlp%02d", i + 1);
            nw4r::lyt::TextBox* nextPane = (nw4r::lyt::TextBox*)mLayout->GetRootPane()->FindPaneByName(buf3, true);
            if (!func_801C4648(nextPane)) return;

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
    nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName("nul_ttl", true);
    func_80124270(pane, arg);
}

extern "C" void func_801C46B4(CTitleAHelp* self, char* arg) {
    if(self->mLayout == nullptr) return;
    func_80136B4C(self->mLayout, lbl_eu_805054BC + 0x64, arg, 0);
}

void CTitleAHelp::func_801C46DC(u32 arg) {
    if(mLayout == nullptr) return;
    nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName("nul_caution", true);
    func_80124270(pane, arg);
}

void CTitleAHelp::func_801C473C(u8 arg) {
    unk28 = arg;
}

// retail: r4 = lbl_eu_805054BC+0x17 (string), r5/r6 = &lbl_eu_80664468/70 (.sbss addrs)
extern char lbl_eu_805054BC[];
extern u32 lbl_eu_80664468;
extern u32 lbl_eu_80664470;
extern u32 lbl_eu_80664478;
extern u32 lbl_eu_80664480;
extern "C" void func_801C4744(CTitleAHelp* self) {
    func_80139A18(self->mLayout, lbl_eu_805054BC + 0x17, &lbl_eu_80664468, &lbl_eu_80664470);
}

extern "C" void func_801C4760(CTitleAHelp* self) {
    func_80139A18(self->mLayout, lbl_eu_805054BC + 0x17, &lbl_eu_80664478, &lbl_eu_80664480);
}

extern "C" void __declspec(noinline) func_801C477C(CTitleAHelp* self) {
    if(func_80137444(self->mAnimTrans20, 1.0f)) {
        self->mLayout->SetAnimationEnable(self->mAnimTrans20, 0);
        self->mLayout->SetAnimationEnable(self->mAnimTrans24, 1);
        self->unk2c = 2;
    }
}

extern "C" void __declspec(noinline) func_801C47F8(CTitleAHelp* self) {
    if(func_80137444(self->mAnimTrans24, 1.0f)) {
        self->unk2c = 3;
        self->unk36 = 1;
        self->unk37 = 0;
    }
}

extern "C" void __declspec(noinline) func_801C484C(CTitleAHelp* self) {
    if(func_80137510(self->mAnimTrans24, 1.0f)) {
        if(self->unk37 == 0) {
            self->mLayout->SetAnimationEnable(self->mAnimTrans24, 0);
            self->mLayout->SetAnimationEnable(self->mAnimTrans20, 1);
            self->unk2c = 5;
        } else {
            self->unk36 = 1;
        }
    }
}

extern "C" void __declspec(noinline) func_801C48E0(CTitleAHelp* self) {
    if(func_80137510(self->mAnimTrans20, 1.0f)) {
        self->unk2c = 0;
        self->unk36 = 1;
    }
}

bool CTitleAHelp::OnFileEvent(CEventFile* pEventFile) {
    if(mFileHandle == pEventFile->mFileHandle) {
        u32 allocHandle = mtl::MemManager::getHandleMEM2();
        unk4.createRegion(allocHandle, 0x3200, "CTitleAHelp", 1);

        Class_8045F858 unkClass8045F858(&unk4);
        void* data = mFileHandle->getData();
        mtl::MemManager::func_80434A4C(false);

        mArcResourceAccessor = CLibLayout::createArcResourceAccessor();
        mArcResourceAccessor->Attach(data, "arc");

        func_80136E84(&mLayout, mArcResourceAccessor, "mf00_hlp_tit.brlyt");
        func_80136F08(mLayout, &mAnimTrans20, mArcResourceAccessor, "mf00_hlp_tit_in.brlan");
        func_80136F08(mLayout, &mAnimTrans24, mArcResourceAccessor, "mf00_hlp_tit_change.brlan");

        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();

        void* fontObj = CDeviceFont::func_80452C10(1, mLayout);
        void** vtable = *(void***)fontObj;
        u32 (*slot9)(void*) = (u32 (*)(void*))vtable[0x24 / 4];
        u32 result = slot9(fontObj);

        func_8013676C(rootPane, result);

        mLayout->SetAnimationEnable(mAnimTrans24, false);
        mLayout->SetAnimationEnable(mAnimTrans20, true);
        mLayout->Animate(0);

        nw4r::lyt::Pane* nulCautionPane = mLayout->GetRootPane()->FindPaneByName("nul_caution", true);
        func_80124270(nulCautionPane, 0);

        func_801C41C0(mName);
        func_801C41E8(unk34);
        unk35 = 1;
        unk28 = 1;
        mFileHandle = nullptr;

        unk4.func_8045F810();
        return true;
    }
    return false;
}

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
bool func_801C4648(nw4r::lyt::Pane* self) {
    const u8* bytes = reinterpret_cast<const u8*>(self);
    return (bytes[0xBB] & 1) != 0;
}

// Retail symbol func_801C4B60 is unmangled (C linkage): declare it extern "C"
// so the definition and sinit's calls all emit the retail reloc name.
extern "C" void func_801C4B60(GXColorS10*, s16, s16, s16, s16);

void sinit_801C4AE4() {
    func_801C4B60(reinterpret_cast<GXColorS10*>(&lbl_eu_80664468), 0xa8, 0x52, 0x08, 0x00);
    func_801C4B60(reinterpret_cast<GXColorS10*>(&lbl_eu_80664470), 0xa8, 0x52, 0x08, 0xff);

    func_801C4B60(reinterpret_cast<GXColorS10*>(&lbl_eu_80664478), 0x05, 0x80, 0xa6, 0x00);
    func_801C4B60(reinterpret_cast<GXColorS10*>(&lbl_eu_80664480), 0x05, 0x80, 0xa6, 0xff);
}

void __declspec(noinline) func_801C4B60(GXColorS10* color, s16 r, s16 g, s16 b, s16 a) {
    color->a = a;
    color->b = b;
    color->g = g;
    color->r = r;
}


