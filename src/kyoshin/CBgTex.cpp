#include "kyoshin/CBgTex.hpp"

#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// Local import (retail unmangled); same decl as CExchangeWin.cpp.
void closeFileHandle(CFileHandle**);

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

void CBgTex::BgTex_SetupRegion_3A24() {
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
        switch (MapValueToRank6(regionId)) {
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
    setPaneVisible(pane, !mPtmMode);
    pane = mLayout->GetRootPane()->FindPaneByName(lbl_eu_80505370 + 0xA0, true);
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
        file = lbl_eu_80505370 + 0xAA;
    } else if (regionId == 0x1A) {
        file = lbl_eu_80505370 + 0xBE;
    } else {
        switch (MapValueToRank6(regionId)) {
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
        accessor->Attach(pArchive, lbl_eu_80505370 + 0x136);
        mFileHandle = nullptr;
        this->BgTex_SetupRegion_3A24();
        return true;
    }
    return false;
}

// ----- retail data (wave DL) -----
// .rodata string pool (0x140, lbl_eu_80505370): layout/region/arc names.
// Every lbl_eu_80505370+N site in SetupRegion/Acquire/OnFileEvent lands on
// a string start (0x00 CBgTexLyt .. 0x136 arc); short init zero-fills the
// 6 tail bytes to 0x140. Non-const + declspec holds it in .rodata.
__declspec(section ".rodata") __attribute__((used, aligned(8)))
char lbl_eu_80505370[0x140] =
    "CBgTexLyt\0"
    "mf00_reg_bg06.brlyt\0"
    "mf00_reg_bg07.brlyt\0"
    "mf00_reg_bg01.brlyt\0"
    "mf00_reg_bg02.brlyt\0"
    "mf00_reg_bg03.brlyt\0"
    "mf00_reg_bg04.brlyt\0"
    "mf00_reg_bg05.brlyt\0"
    "pic_ptm00\0"
    "pic_ptm01\0"
    "menu/jp/BgTex06.arc\0"
    "menu/jp/BgTex07.arc\0"
    "menu/jp/BgTex01.arc\0"
    "menu/jp/BgTex02.arc\0"
    "menu/jp/BgTex03.arc\0"
    "menu/jp/BgTex04.arc\0"
    "menu/jp/BgTex05.arc\0"
    "arc\0";
// .data vtable block (retail rela.data order): DB8 head {extern,0,0,0},
// DC8 IWorkEvent vtable {sdata-locator, 0, dtor, 31 handlers}, E50 RTTI
// trailer {RTTI,0,0,0}. Plain defs stay in .data (every slot nonzero via
// reloc or literal). Handler decls mirror data_vtables.hpp spellings but
// stay TU-local (that header is intentionally not included here).
extern u32 lbl_eu_80662410[];
extern u32 lbl_eu_80662650[];
extern "C" {
void __dt__6CBgTexFv();
void OnFileEvent__6CBgTexFP10CEventFile();
void WorkEvent1__10IWorkEventFPvPCc();
void WorkEvent3__10IWorkEventFPv();
void WorkEvent4__10IWorkEventFv();
void OnPauseTrigger__10IWorkEventFb();
void WorkEvent6__10IWorkEventFv();
void WorkEvent7__10IWorkEventFv();
void WorkEvent8__10IWorkEventFv();
void WorkEvent9__10IWorkEventFv();
void WorkEvent10__10IWorkEventFv();
void WorkEvent11__10IWorkEventFv();
void WorkEvent12__10IWorkEventFv();
void WorkEvent13__10IWorkEventFv();
void WorkEvent14__10IWorkEventFv();
void WorkEvent15__10IWorkEventFv();
void WorkEvent16__10IWorkEventFv();
void WorkEvent17__10IWorkEventFv();
void WorkEvent18__10IWorkEventFv();
void WorkEvent19__10IWorkEventFv();
void WorkEvent20__10IWorkEventFv();
void WorkEvent21__10IWorkEventFv();
void WorkEvent22__10IWorkEventFv();
void WorkEvent23__10IWorkEventFv();
void WorkEvent24__10IWorkEventFv();
void WorkEvent25__10IWorkEventFv();
void WorkEvent26__10IWorkEventFv();
void WorkEvent27__10IWorkEventFv();
void WorkEvent28__10IWorkEventFv();
void WorkEvent29__10IWorkEventFv();
void WorkEvent30__10IWorkEventFv();
void WorkEvent31__10IWorkEventFv();
// __RTTI__10IWorkEvent cannot be spelled directly (MWCC 10322 under -RTTI
// on, deferred to the last declaration); stand-in unifies natively.
extern u32 rtti_10IWorkEvent[];
}
__declspec(section ".data") __attribute__((used, aligned(4)))
u32 lbl_eu_80533DB8[4] = { (u32)lbl_eu_80662410, 0, 0, 0 };
__declspec(section ".data") __attribute__((used, aligned(4)))
u32 lbl_eu_80533DC8[0x22] = {
    (u32)lbl_eu_80662650, 0, (u32)__dt__6CBgTexFv,
    (u32)WorkEvent1__10IWorkEventFPvPCc, (u32)OnFileEvent__6CBgTexFP10CEventFile,
    (u32)WorkEvent3__10IWorkEventFPv, (u32)WorkEvent4__10IWorkEventFv,
    (u32)OnPauseTrigger__10IWorkEventFb, (u32)WorkEvent6__10IWorkEventFv,
    (u32)WorkEvent7__10IWorkEventFv, (u32)WorkEvent8__10IWorkEventFv,
    (u32)WorkEvent9__10IWorkEventFv, (u32)WorkEvent10__10IWorkEventFv,
    (u32)WorkEvent11__10IWorkEventFv, (u32)WorkEvent12__10IWorkEventFv,
    (u32)WorkEvent13__10IWorkEventFv, (u32)WorkEvent14__10IWorkEventFv,
    (u32)WorkEvent15__10IWorkEventFv, (u32)WorkEvent16__10IWorkEventFv,
    (u32)WorkEvent17__10IWorkEventFv, (u32)WorkEvent18__10IWorkEventFv,
    (u32)WorkEvent19__10IWorkEventFv, (u32)WorkEvent20__10IWorkEventFv,
    (u32)WorkEvent21__10IWorkEventFv, (u32)WorkEvent22__10IWorkEventFv,
    (u32)WorkEvent23__10IWorkEventFv, (u32)WorkEvent24__10IWorkEventFv,
    (u32)WorkEvent25__10IWorkEventFv, (u32)WorkEvent26__10IWorkEventFv,
    (u32)WorkEvent27__10IWorkEventFv, (u32)WorkEvent28__10IWorkEventFv,
    (u32)WorkEvent29__10IWorkEventFv, (u32)WorkEvent30__10IWorkEventFv,
    (u32)WorkEvent31__10IWorkEventFv,
};
__declspec(section ".data") __attribute__((used, aligned(4)))
u32 lbl_eu_80533E50[4] = { (u32)&rtti_10IWorkEvent, 0, 0, 0 };
// .sdata2: class-name tag + two conversion doubles (both code orphans).
__declspec(section ".sdata2") __attribute__((used, aligned(1)))
char lbl_eu_80667F18[8] = "CBgTex";
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
extern const double lbl_eu_80667F20 = 4503599627370496.0;
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
extern const double lbl_eu_80667F28 = 1.0;
// .sdata locator pair {class-name, vtable-RTTI-slot} (retail rela.sdata).
// Plain def: nonzero small data lands in .sdata on its own (a declspec
// def here would overload against the tentative decl above).
u32 lbl_eu_80662650[2] = { (u32)lbl_eu_80667F18, (u32)lbl_eu_80533E50 };
// .sbss pair (retail NOBITS 0x8): definitions match the header decls
// exactly (s32 counter, accessor pointer); zero-init keeps them in .sbss.
s32 lbl_eu_80664460 = 0;
nw4r::lyt::ArcResourceAccessor* lbl_eu_80664464 = 0;
// ----- end retail data -----
