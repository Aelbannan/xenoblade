#include "kyoshin/CBgTex.hpp"

#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// US retail keeps most callee names unmangled in CBgTex.o relocs; the C++
// member manglings are only kept for __dt__ and OnFileEvent (see CBgTex.hpp).

// Retail frames for this unit use the stmw/lmw block save (2-3 callee-saved
// registers), which needs the -O4,s size-optimized lowering (MWCC_CASES
// §16 stmw/lmw: -O4,p would emit separate stw/lwz pairs). The unit is
// configured with -O4,s -func_align 4 in configure.py.
CBgTex::CBgTex(u8 arg) : CBgTexVtblBase(), mMemRegion() {
    mFileHandle = nullptr;
    mLayout = nullptr;
    mLayoutReady = false;
    mLoaded = 0;
    mPtmMode = arg;
}

CBgTex::~CBgTex() {}

void CBgTex::func_801C3A24() {
    u32 handle = mtl::MemManager::getHandleMEM2();
    mMemRegion.createRegion(handle, 0x2000, lbl_eu_80505370, 1);
    Class_8045F858 unusedVar(&mMemRegion);
    mtl::MemManager::setMemInitFlag(false);

    u8 regionId = static_cast<u8>(lbl_eu_80664184);
    if (regionId == 0x19) {
        buildLayout(&mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x0A);
    } else if (regionId == 0x1A) {
        buildLayout(&mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x1E);
    } else {
        switch (func_801372B4(regionId)) {
        case 1:
            buildLayout(&mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x32);
            break;
        case 2:
            buildLayout(&mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x46);
            break;
        case 3:
            buildLayout(&mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x5A);
            break;
        case 4:
            buildLayout(&mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x6E);
            break;
        case 5:
            buildLayout(&mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x82);
            break;
        case 0:
            buildLayout(&mLayout, lbl_eu_80664464, lbl_eu_80505370 + 0x32);
            break;
        default:
            break;
        }
    }

    nw4r::lyt::Pane* pane =
        mLayout->GetRootPane()->FindPaneByName(lbl_eu_80505370 + 0x96, true);
    func_80124270(pane, !mPtmMode);
    pane = mLayout->GetRootPane()->FindPaneByName(lbl_eu_80505370 + 0xA0, true);
    func_80124270(pane, !mPtmMode);
    this->func_801C3E3C();
    mMemRegion.func_8045F810();
}

bool CBgTex::func_801C3C14() {
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
    CFileHandle* handle = CDeviceFile::readFile(
        allocHandle, file, reinterpret_cast<IWorkEvent*>(this), 0, 0);
    mFileHandle = handle;
    CDeviceFile::setHandleFlag1(handle);
    return false;
}

void CBgTex::func_801C3D54() {
    if (mLayoutReady == false)
        return;
    mLayout->Animate(0);
}

void CBgTex::func_801C3D7C(nw4r::lyt::DrawInfo* drawInfo) {
    if (mLayoutReady == false)
        return;
    drawLayout(mLayout, drawInfo, 0, 1);
}

void CBgTex::func_801C3D9C() {
    lbl_eu_80664460 -= 1;
    func_801390E0(&mFileHandle);
    mLayoutReady = false;
    CDeviceVI::waitForDrawDone();
    if (mLayout != nullptr) {
        delete mLayout;
        mLayout = nullptr;
    }
    if (lbl_eu_80664460 <= 0) {
        releaseArcResourceAccessor(lbl_eu_80664464);
        lbl_eu_80664464 = nullptr;
    }
    mMemRegion.func_8045F778();
}

u8 CBgTex::func_801C3E34() {
    return mLoaded;
}

void CBgTex::func_801C3E3C() {
    if (mLayout == nullptr)
        return;
    mLoaded = 1;
    mLayoutReady = true;
}

bool CBgTex::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        void* pArchive = mFileHandle->getData();
        nw4r::lyt::ArcResourceAccessor* accessor =
            CLibLayout::createArcResourceAccessor();
        lbl_eu_80664464 = accessor;
        accessor->Attach(pArchive, lbl_eu_80505370 + 0x136);
        mFileHandle = nullptr;
        this->func_801C3A24();
        return true;
    }
    return false;
}
