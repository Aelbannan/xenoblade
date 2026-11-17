#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/CBgTex.hpp"

#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>
#include "kyoshin/code_80135FDC_State.hpp"

// Local import (retail unmangled); same decl as CExchangeWin.cpp.

// US retail keeps most callee names unmangled in CBgTex.o relocs; the C++
// member manglings are only kept for __dt__ and OnFileEvent (see CBgTex.hpp).

// Retail frames for this unit use the stmw/lmw block save (2-3 callee-saved
// registers), which needs the -O4,s size-optimized lowering (MWCC_CASES
// §16 stmw/lmw: -O4,p would emit separate stw/lwz pairs). The unit is
// configured with -O4,s -func_align 4 in configure.py.
CBgTex::CBgTex(u8 arg) : mMemRegion() {
    mFileHandle = nullptr;
    mLayout = nullptr;
    mLayoutReady = false;
    mLoaded = 0;
    mPtmMode = arg;
}

CBgTex::~CBgTex() {}

void CBgTex::BgTex_SetupRegion_3A24() {
    u32 handle = mtl::MemManager::getHandleMEM2();
    mMemRegion.createRegion(handle, 0x2000, "CBgTexLyt", 1);
    Class_8045F858 unusedVar(&mMemRegion);
    mtl::MemManager::setMemInitFlag(false);

    u8 regionId = static_cast<u8>(lbl_eu_80664184);
    if (regionId == 0x19) {
        buildLayout(&mLayout, lbl_eu_80664464, "mf00_reg_bg06.brlyt");
    } else if (regionId == 0x1A) {
        buildLayout(&mLayout, lbl_eu_80664464, "mf00_reg_bg07.brlyt");
    } else {
        switch (MapValueToRank6(regionId)) {
        case 1:
            buildLayout(&mLayout, lbl_eu_80664464, "mf00_reg_bg01.brlyt");
            break;
        case 2:
            buildLayout(&mLayout, lbl_eu_80664464, "mf00_reg_bg02.brlyt");
            break;
        case 3:
            buildLayout(&mLayout, lbl_eu_80664464, "mf00_reg_bg03.brlyt");
            break;
        case 4:
            buildLayout(&mLayout, lbl_eu_80664464, "mf00_reg_bg04.brlyt");
            break;
        case 5:
            buildLayout(&mLayout, lbl_eu_80664464, "mf00_reg_bg05.brlyt");
            break;
        case 0:
            buildLayout(&mLayout, lbl_eu_80664464, "mf00_reg_bg01.brlyt");
            break;
        default:
            break;
        }
    }

    nw4r::lyt::Pane* pane =
        mLayout->GetRootPane()->FindPaneByName("pic_ptm00", true);
    setPaneVisible(pane, !mPtmMode);
    pane = mLayout->GetRootPane()->FindPaneByName("pic_ptm01", true);
    setPaneVisible(pane, !mPtmMode);
    this->BgTex_MarkLoaded_3E3C();
    mMemRegion.validateHeap();
}

bool CBgTex::BgTex_Acquire_3C14() {
    nw4r::lyt::ArcResourceAccessor* accessor = lbl_eu_80664464;
    s32 count = lbl_eu_80664460;
    lbl_eu_80664460 = count + 1;
    if (accessor != nullptr) {
        return true;
    }

    char* file = nullptr;
    u8 regionId = static_cast<u8>(lbl_eu_80664184);
    if (regionId == 0x19) {
        file = "menu/jp/BgTex06.arc";
    } else if (regionId == 0x1A) {
        file = "menu/jp/BgTex07.arc";
    } else {
        switch (MapValueToRank6(regionId)) {
        case 1:
            file = "menu/jp/BgTex01.arc";
            break;
        case 2:
            file = "menu/jp/BgTex02.arc";
            break;
        case 3:
            file = "menu/jp/BgTex03.arc";
            break;
        case 4:
            file = "menu/jp/BgTex04.arc";
            break;
        case 5:
            file = "menu/jp/BgTex05.arc";
            break;
        case 0:
            file = "menu/jp/BgTex01.arc";
            break;
        }
    }

    u32 allocHandle = mtl::MemManager::getHandleMEM2();
    CFileHandle* handle = CDeviceFile::readFile(
        allocHandle, file, this, 0, 0);
    mFileHandle = handle;
    CDeviceFile::setHandleFlag1(handle);
    return false;
}

void CBgTex::BgTex_Tick_3D54() {
    if (mLayoutReady == false)
        return;
    mLayout->Animate(0);
}

void CBgTex::BgTex_Draw_3D7C(nw4r::lyt::DrawInfo* drawInfo) {
    if (mLayoutReady == false)
        return;
    drawLayout(mLayout, drawInfo, 0, 1);
}

void CBgTex::BgTex_Release_3D9C() {
    lbl_eu_80664460 -= 1;
    closeFileHandle(&mFileHandle);
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
    mMemRegion.deleteRegion();
}

u8 CBgTex::BgTex_IsLoaded_3E34() {
    return mLoaded;
}

void CBgTex::BgTex_MarkLoaded_3E3C() {
    if (mLayout == nullptr)
        return;
    mLoaded = 1;
    mLayoutReady = true;
}

bool CBgTex::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        // getData() is void* ABI; attach buffer typed as u8* (CUICfManager idiom).
        u8* pArchive = static_cast<u8*>(mFileHandle->getData());
        nw4r::lyt::ArcResourceAccessor* accessor =
            CLibLayout::createArcResourceAccessor();
        lbl_eu_80664464 = accessor;
        accessor->Attach(pArchive, "arc");
        mFileHandle = nullptr;
        this->BgTex_SetupRegion_3A24();
        return true;
    }
    return false;
}

// .sbss pair (retail NOBITS 0x8): definitions match the header decls
// exactly (s32 counter, accessor pointer); zero-init keeps them in .sbss.
s32 lbl_eu_80664460 = 0;
nw4r::lyt::ArcResourceAccessor* lbl_eu_80664464 = 0;
// ----- end retail data -----
