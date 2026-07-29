// Auto-scaffolded catalog TU for kyoshin/CExchangeWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
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
extern const char lbl_eu_8050A740[];
extern void func_80137924(void*, void*, void*, void*);
extern void func_80138078(u32);
extern float lbl_eu_80668610;
extern "C" void* createArcResourceAccessor__10CLibLayoutFv();

// Constructor — defined as global function with __ct__ prefix to match
// retail C-linkage symbol __ct__CExchangeWin (avoids 12-prefix mangling).
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);

CExchangeWin* __ct__CExchangeWin(CExchangeWin* self) {
    self->mVtbl = lbl_eu_80536640;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    self->mFileHandle = nullptr;
    self->mAccessor = nullptr;
    self->mLayout = nullptr;
    self->mAnimTransform = nullptr;
    self->field_24 = 0;
    self->field_25 = 1;
    self->_26 = 0;
    self->field_27 = 1;
    return self;
}

extern "C" u8 func_8022D08C(void* self) { return ((CExchangeWin*)self)->field_25; }

extern "C" u8 func_8022D094(void* self) { return ((CExchangeWin*)self)->field_24; }

extern "C" u8 func_8022D09C(void* self) { return ((CExchangeWin*)self)->field_27; }

// If _26 is already non-zero, do nothing; otherwise initialize state and fire event 0xd
extern "C" void func_8022D0A4(CExchangeWin* self) {
    if (self->_26 != 0) {
        return;
    }
    self->_26 = 1;
    self->field_27 = 0;
    self->field_24 = 1;
    func_80138078(0xd);
}

extern "C" __attribute__((noinline)) void func_8022D1F8(CExchangeWin* self) {
    float f = lbl_eu_80668610;
    u32 r = func_80137444(self->mAnimTransform, f);
    if (r) {
        self->_26 = 2;
        self->field_27 = 1;
    }
}

extern "C" __attribute__((noinline)) void func_8022D244(CExchangeWin* self) {}

// OnFileEvent — loads the layout from a file, sets up fonts, animations,
// text fields, and a texture resource with pane sizing from TPL dimensions.
bool CExchangeWin::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle != pEventFile->mFileHandle) {
        return false;
    }

    mMemRegion.createRegion(
        mtl::MemManager::getHandleMEM2(),
        0x8000,
        &lbl_eu_8050A740[0x4e],
        0
    );

    Class_8045F858 helper(&mMemRegion);

    void* arcData = mFileHandle->mData;
    mFileHandle->mData = nullptr;
    mtl::MemManager::func_80434A4C(false);

    mAccessor = CLibLayout::createArcResourceAccessor();
    mAccessor->Attach(arcData, &lbl_eu_8050A740[0x5b]);

    func_80136E84(&mLayout, mAccessor, &lbl_eu_8050A740[0x5f]);
    func_80136F08(mLayout, &mAnimTransform, mAccessor,
                  (char*)&lbl_eu_8050A740[0x78]);

    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    void* fontObj = CDeviceFont::func_80452C10(1, mLayout);
    typedef u32 (*FontVFn)(void*);
    u32 fontResult =
        (*reinterpret_cast<FontVFn**>(fontObj))[0x24 / 4](fontObj);
    func_8013676C(rootPane, fontResult);

    mLayout->SetAnimationEnable(mAnimTransform, true);
    mLayout->Animate(0);

    // Set text fields from message table
    char* t1 =
        func_80136190((char*)&lbl_eu_8050A740[0x94],
                       (char*)&lbl_eu_8050A740[0x9d], 0x24);
    func_80136B4C(mLayout, (char*)&lbl_eu_8050A740[0xa2], t1, 0);

    char* t2 =
        func_80136190((char*)&lbl_eu_8050A740[0x94],
                       (char*)&lbl_eu_8050A740[0x9d], 0x25);
    func_80136B4C(mLayout, (char*)&lbl_eu_8050A740[0xaf], t2, 0);

    char* t3 =
        func_80136190((char*)&lbl_eu_8050A740[0x94],
                       (char*)&lbl_eu_8050A740[0x9d], 0x26);
    func_80136B4C(mLayout, (char*)&lbl_eu_8050A740[0xba], t3, 0);

    char* t4 =
        func_80136190((char*)&lbl_eu_8050A740[0xc5],
                       (char*)&lbl_eu_8050A740[0xd3], 0x2b);
    func_80136B4C(mLayout, (char*)&lbl_eu_8050A740[0xd8], t4, 0);

    // Choose file ID based on game manager state
    const char* fileID = &lbl_eu_8050A740[0xed];
    if (cf::CfGameManager::func_80086F9C(-1) != 0) {
        fileID = &lbl_eu_8050A740[0xe4];
    }

    u16 keyVal =
        func_8013606C((char*)&lbl_eu_8050A740[0xc5], (char*)fileID, 0x2b);
    char* name = func_80138F78(keyVal);

    nw4r::lyt::ArcResourceAccessor* resAcc =
        CUICfManager::func_801355F4();
    TPLPalette* resource = (TPLPalette*)resAcc->GetResource(
        nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, name, nullptr);

    if (resource != nullptr) {
        func_80137E7C(mLayout, (char*)&lbl_eu_8050A740[0xf6], resource);

        nw4r::lyt::Pane* pane = mLayout->GetRootPane()->FindPaneByName(
            (char*)&lbl_eu_8050A740[0xf6], true);
        if (pane != nullptr) {
            TPLHeader* header = resource->descriptorArray->textureHeader;
            pane->mSize.width = (f32)header->width;
            pane->mSize.height = (f32)header->height;
        }
    }

    if (mLayout != nullptr) {
        field_25 = 1;
    }

    mFileHandle = nullptr;
    mMemRegion.func_8045F810();

    return true;
}

// Stub functions needed by CItemBoxGrid
extern "C" void func_8022D0D0(void* self) {
    CExchangeWin* s = (CExchangeWin*)self;
    if (s->_26 != 2) {
        return;
    }
    s->_26 = 3;
    s->field_27 = 0;
    func_80138078(0xe);
}
extern "C" void func_8022D0F8(void* dst, void* src, u8 val) {
    char buf[40];
    CExchangeWin* win;
    nw4r::lyt::Pane* pane1;
    nw4r::lyt::Pane* pane2;

    win = (CExchangeWin*)src;
    sprintf(buf, &lbl_eu_8050A740[0x18], val + 1);
    pane1 = win->mLayout->GetRootPane()->FindPaneByName(buf, true);
    pane2 = win->mLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A740[0x25], true);
    func_80137924(dst, pane1, pane2, win->mLayout->GetRootPane());
}
extern "C" void func_8022CF2C(CExchangeWin* self) {
    self->mFileHandle = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(),
        lbl_eu_8050A740,
        (IWorkEvent*)self,
        0,
        0
    );
    self->field_25 = 0;
}

// func_8022CF7C — update loop: drives animation state machine and calls
// mLayout->Animate(0) when field_24 is set. Dispatches to func_8022D1F8
// for _26==1 (entering) and func_8022D244 for _26==3 (exiting).
extern "C" void func_8022CF7C(CExchangeWin* self) {
    s32 s;

    if (self->field_24 == 0) {
        return;
    }
    s = self->_26;
    if (s == 1) {
        func_8022D1F8(self);
    } else if (s == 3) {
        func_8022D244(self);
    }
    self->mLayout->Animate(0);
}

extern "C" void func_8022CFEC(void* self, nw4r::lyt::DrawInfo* drawInfo) {
    CExchangeWin* s = (CExchangeWin*)self;
    if (s->field_24 == 0) {
        return;
    }
    if (s->_26 == 0) {
        return;
    }
    func_80137038(s->mLayout, drawInfo, 0, 1);
}

// func_8022D018 — teardown: releases file handle, destroys the layout,
// releases the arc resource accessor, and cleans up the memory region.
extern "C" void func_8022D018(CExchangeWin* self) {
    func_801390E0(&self->mFileHandle);
    self->field_24 = 0;
    if (self->mLayout != NULL) {
        delete self->mLayout;
        self->mLayout = NULL;
    }
    func_80139124(self->mAccessor);
    self->mMemRegion.func_8045F778();
}
