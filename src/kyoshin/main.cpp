#include "decomp.h"
#include "kyoshin/cf/object/CAIAction.hpp"
#include "kyoshin/plugin/pluginMain.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/CGame.hpp"
#include "kyoshin/action/CActParamData.hpp"
#include "monolib/nand/CNand.hpp"
#include "monolib/vm/yvm2.h"
#include "monolib/core.hpp"
#include "monolib/lib.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "monolib/util.hpp"
#include "monolib/device/CDeviceFileCri.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "revolution/gx/GXTypes.h"

// Retail imports for main.cpp - proper header so there is no local
// C-linkage import block (lint rule).
#include "kyoshin/main_imports.hpp"
#include "kyoshin/CErrMes.hpp"
#include "kyoshin/CNandData.hpp"

#if defined(VERSION_JP)
// ---------------------------------------------------------------------------
// VERSION_JP data: kept in the old literal form (JP retail data layout differs
// from EU/US; only the US build is matched against retail).
static DesktopIcon sGameMainIcon = {
    "ゲームメイン",
    &CGame::GameMain
};
static const char* sLanguageFolderPaths[8] = {
    "/jp/", "/en/", "/en/", "/de/", "/fr/", "/sp/", "/it/", nullptr,
};
const char* languageFolderPaths[8] = {
    "\\jp\\", "\\en\\", "\\en\\", "\\de\\", "\\fr\\", "\\sp\\", "\\it\\", nullptr,
};

#else
// ---------------------------------------------------------------------------
// US/EU data.  String literals appear in retail first-use order; MWCC's
// -str reuse,pool,readonly emits them as the same .rodata/.sdata2 pools
// (verified with run.py data diff).  The .data arrays are contiguous in the
// retail order: sGameMainIcon, sLanguageFolderPaths[8], languageFolderPaths[8],
// sStaticArcFiles[10] (MWCC addresses them off one base register).

DesktopIcon sGameMainIcon = {
    "GAME_MAIN",
    &CGame::GameMain
};

static const char* const scStaticArcStr[2] = { "lang/jp/static.arc", nullptr };


// Static.arc per-file load/unload callbacks, registered via sStaticArcFiles
// below and invoked by CLibStaticData when the archive entry is paged in/out.

void OnBdatFileLoaded(void* pData, u32 length) {
    CBdat::setBdatEntry(0, pData);
}

void OnBdatFileUnloaded(void* pData, u32 length) {
    CBdat::getEntry(0);
}

void OnAidatFileLoaded(void* pData, u32 length){
    initAIActionTable(pData);
}

void OnAidatFileUnloaded(void* pData, u32 length){
    clearAIActionTable();
}

void OnHbmstopFileLoaded(void* pData, u32 length){
    CLibHbm::loadTplImage(pData);
}

void OnHbmstopFileUnloaded(void* pData, u32 length){
    CLibHbm::removeTplImage();
}

// ---- .rodata pointer array (retail .rodata +0xA8) ----
const char* const sPkhFilenames[13] = {
    "ahx/jp/ahx.pkh",
    "common/jp/common.pkh",
    "mapbdat/jp/mapbdat.pkh",
    "menu/jp/menu.pkh",
    "script/jp/script.pkh",
    "adx.pkh",
    "chr/jp/chr.pkh",
    "eff.pkh",
    "font.pkh",
    "map.pkh",
    "obj.pkh",
    "snd.pkh",
    nullptr
};

// ---- .rodata dvddata path strings ----

// ---- .data arrays (contiguous, retail .data 0x80524A80..0x80524B98) ----
const char* sLanguageFolderPaths[8] = {
    "/jp/",
    "/en/",
    "/de/",
    "/fr/",
    "/sp/",
    "/it/",
    nullptr,
};

const char* languageFolderPaths[8] = {
    "\\jp\\",
    "\\en\\",
    "\\de\\",
    "\\fr\\",
    "\\sp\\",
    "\\it\\",
    nullptr,
};

//List of files contained in the static.arc archive
StaticArcFileData sStaticArcFiles[10] = {
    {"SHA", "dvddata/etc/shadow.sha", HANDLE_MEM2, nullptr, nullptr},
    {"CAM", "dvddata/etc/cam.chr", HANDLE_MEM2, nullptr, nullptr},
    {"EFF", "dvddata/etc/eff.chr", HANDLE_MEM2, nullptr, nullptr},
    {"ARROW", "dvddata/etc/arrow.mdo", HANDLE_MEM2, nullptr, nullptr},
    {"43", "dvddata/menu/jp/Mode43.arc", HANDLE_MEM2, nullptr, nullptr},
    {"BDAT", "common/jp/bdat_common.bin", HANDLE_MEM2, &OnBdatFileLoaded, &OnBdatFileUnloaded},
    {"AIDAT", "dvddata/etc/ai.bin", HANDLE_MEM2, &OnAidatFileLoaded, &OnAidatFileUnloaded},
    {"HIKARI", "dvddata/etc/hikari.brres", HANDLE_MEM2, nullptr, nullptr},
    {"HBMSTOP", "dvddata/etc/hbmstop.tpl", HANDLE_MEM2, &OnHbmstopFileLoaded, &OnHbmstopFileUnloaded},
    {nullptr, nullptr, HANDLE_MEM1, nullptr, nullptr}
};
#endif

// VM initialization callbacks handed to CLibVM::setCallbacks: the first run
// once during VM startup (init + plugin registration), the second on every VM
// (re)initialization.

void vmInitPluginRegistCallback(){
    vmInit();
    pluginRegist();
}

void vmInitCallback() {
    vmInit();
}

#ifdef __MWERKS__
#pragma optimize_for_size on
void main(){
#else
int main(){
#endif
    nand.setShare(false);
    CBdat::clearEntries();
    loadErrMesTable();
    NandData_SetupBanner_11C0();
    func_eu_804520B0(getErrMesLangTable0());
    func_eu_804520B0(getErrMesLangTable1());
    func_eu_804520B0(getErrMesLangTable2());
#if !defined(VERSION_JP)
    func_eu_804520B0(sLanguageFolderPaths);
    func_eu_804520B0(languageFolderPaths);
#endif
    CDeviceFileCri::setCriFilePathA(getErrMesText01());
    CDeviceFileCri::setCriFilePathB(getErrMesText02());
    CDeviceFileCri::setCriFilePathC(getErrMesText03());
    setGeneralNandErrorMessage(getErrMesText08());
    setSaveNandErrorMessage(getErrMesText09());
    setExtraNandErrorMessage(getErrMesText10());
    gErrMesInitFlag = 0;
    mtl::MemManager::MemRegion::setRegionMaxSize(0x686000, 0);
    CDeviceVI::setUseStaticHandle(true);
    CDeviceGX::setValues(GX_PF_RGB8_Z24, 0x180000);
    entryTable__8CDesktopFPQ28CDesktop16DESKTOP_ICON_DEFb(&sGameMainIcon, 1);
    CLibStaticData::saveStaticFileArray(sStaticArcFiles);
    CLibVM::setCallbacks(&vmInitPluginRegistCallback, &vmInitCallback);
    CWorkSystemPack::SaveStaticArcFilenameStringPtr(scStaticArcStr);
    CWorkSystemPack::SavePkhFilenamesArrayPtr(sPkhFilenames);
    CActParamData::initParamStatics();
    CLibHbm::setHbmActiveFlag(true);
    CDeviceFont_SetSharedText(getErrMesText03());
    CWorkRoot::run();
    touchErrMesEntry();
}
#pragma optimize_for_size off
