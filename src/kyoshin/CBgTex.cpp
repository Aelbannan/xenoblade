#include "kyoshin/CBgTex.hpp"

#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

extern void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
extern void func_801390E0(CFileHandle**);

// US retail keeps these callee names unmangled in CBgTex.o relocs.
extern "C" void func_80124270(nw4r::lyt::Pane*, u32);
extern "C" u8 func_801372B4(u8);

extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);

// Shared string pool (split1 .rodata).
extern "C" char lbl_eu_80505370[];

// SDA globals (retail names; defined in common/sbss splits).
extern "C" u32 lbl_eu_80664184;
extern "C" s32 lbl_eu_80664460;
extern "C" nw4r::lyt::ArcResourceAccessor* lbl_eu_80664464;

extern "C" void func_801C3E3C(CBgTex* self);

// Emit ctor before dtor so .text order matches retail (avoids 0xC align pad).
extern "C" CBgTex* __ct__CBgTex(CBgTex* self, u8 arg) {
    self->mVtbl = lbl_eu_80533DC8;
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);
    self->mFileHandle = nullptr;
    self->mLayout = nullptr;
    self->mLayoutReady = false;
    self->mLoaded = 0;
    self->mPtmMode = arg;
    return self;
}

CBgTex::~CBgTex() {}


extern "C" void func_801C3A24(CBgTex* self) {
    u32 handle = mtl::MemManager::getHandleMEM2();
    self->mMemRegion.createRegion(handle, 0x2000, lbl_eu_80505370, 1);
    Class_8045F858 unusedVar = Class_8045F858(&self->mMemRegion);
    mtl::MemManager::func_80434A4C(false);

    u8 regionId = static_cast<u8>(lbl_eu_80664184);
    if (regionId == 0x19) {
        func_80136E84(&self->mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x0A);
    } else if (regionId == 0x1A) {
        func_80136E84(&self->mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x1E);
    } else {
        switch (func_801372B4(regionId)) {
        case 1:
            func_80136E84(&self->mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x32);
            break;
        case 2:
            func_80136E84(&self->mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x46);
            break;
        case 3:
            func_80136E84(&self->mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x5A);
            break;
        case 4:
            func_80136E84(&self->mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x6E);
            break;
        case 5:
            func_80136E84(&self->mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x82);
            break;
        case 0:
            func_80136E84(&self->mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x32);
            break;
        default:
            break;
        }
    }

    nw4r::lyt::Pane* pane =
        self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80505370 + 0x96, true);
    func_80124270(pane, !self->mPtmMode);
    pane = self->mLayout->GetRootPane()->FindPaneByName(lbl_eu_80505370 + 0xA0, true);
    func_80124270(pane, !self->mPtmMode);
    func_801C3E3C(self);
    self->mMemRegion.func_8045F810();
}

extern "C" bool func_801C3C14(CBgTex* self) {
    nw4r::lyt::ArcResourceAccessor* accessor = lbl_eu_80664464;
    s32 count = lbl_eu_80664460;
    lbl_eu_80664460 = count + 1;
    if (accessor != nullptr) {
        return true;
    }

    char* file = nullptr;
    u8 regionId = static_cast<u8>(lbl_eu_80664184);
    if (regionId == 0x19) {
        file = lbl_eu_80505370 + 0xAA;
    } else if (regionId == 0x1A) {
        file = lbl_eu_80505370 + 0xBE;
    } else {
        switch (func_801372B4(regionId)) {
        case 1:
            file = lbl_eu_80505370 + 0xD2;
            break;
        case 2:
            file = lbl_eu_80505370 + 0xE6;
            break;
        case 3:
            file = lbl_eu_80505370 + 0xFA;
            break;
        case 4:
            file = lbl_eu_80505370 + 0x10E;
            break;
        case 5:
            file = lbl_eu_80505370 + 0x122;
            break;
        case 0:
            file = lbl_eu_80505370 + 0xD2;
            break;
        }
    }

    u32 allocHandle = mtl::MemManager::getHandleMEM2();
    self->mFileHandle = CDeviceFile::readFile(
        allocHandle, file, reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::setHandleFlag1(self->mFileHandle);
    return false;
}

extern "C" void func_801C3D54(CBgTex* self) {
    if (self->mLayoutReady == false)
        return;
    self->mLayout->Animate(0);
}

extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mLayoutReady == false)
        return;
    func_80137038(self->mLayout, drawInfo, 0, 1);
}

extern "C" void func_801C3D9C(CBgTex* self) {
    lbl_eu_80664460 -= 1;
    func_801390E0(&self->mFileHandle);
    self->mLayoutReady = false;
    CDeviceVI::waitForDrawDone();
    if (self->mLayout != nullptr) {
        delete self->mLayout;
        self->mLayout = nullptr;
    }
    if (lbl_eu_80664460 <= 0) {
        func_80139124(lbl_eu_80664464);
        lbl_eu_80664464 = nullptr;
    }
    self->mMemRegion.func_8045F778();
}

extern "C" u8 func_801C3E34(CBgTex* self) {
    return self->mLoaded;
}

extern "C" void func_801C3E3C(CBgTex* self) {
    if (self->mLayout == nullptr)
        return;
    self->mLoaded = 1;
    self->mLayoutReady = true;
}

bool CBgTex::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        void* pArchive = mFileHandle->getData();
        lbl_eu_80664464 = CLibLayout::createArcResourceAccessor();
        lbl_eu_80664464->Attach(pArchive, lbl_eu_80505370 + 0x136);
        mFileHandle = nullptr;
        func_801C3A24(this);
        return true;
    }
    return false;
}
