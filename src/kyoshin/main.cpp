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

// US/EU error / path helpers (kyoshin/CErrMes, CNandData, CDeviceFile, CDeviceFont).
extern "C" {
extern u8 lbl_eu_8065FD00[];
extern s8 lbl_eu_80663B18;

void func_eu_802B12DC();
void func_eu_802B11C0();
const char* func_eu_802B14D4();
const char* func_eu_802B14E0();
const char* func_eu_802B14EC();
const wchar_t* func_eu_802B133C();
const wchar_t* func_eu_802B1354();
const wchar_t* func_eu_802B136C();
const wchar_t* func_eu_802B13E4();
const wchar_t* func_eu_802B13FC();
const wchar_t* func_eu_802B1414();
void func_eu_802B1334();

void func_eu_804520B0(const void* p);
void func_eu_80457318(const wchar_t* msg);

// Mangled CDesktop::entryTable — asm bl target (PLAN.md §17.6).
void entryTable__8CDesktopFPQ28CDesktop16DESKTOP_ICON_DEFb(DesktopIcon*, bool);
}

static DesktopIcon sGameMainIcon = {
#if defined(VERSION_JP)
    "ゲームメイン",
#else //EU/US
    "GAME_MAIN",
#endif
    &CGame::GameMain
};

#if !defined(VERSION_JP)
// Non-const so these sit in .data immediately after sGameMainIcon (retail main
// keeps r30 = &sGameMainIcon and uses r30+0x10 / +0x30 / +0x50).
static const char* sLanguageFolderPaths[8] = {
    "/jp/",
    "/en/",
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
    "\\en\\",
    "\\de\\",
    "\\fr\\",
    "\\sp\\",
    "\\it\\",
    nullptr,
};
#endif

//Static file callback functions.

void OnBdatFileLoaded(void* pData, u32 length) {
    CBdat::func_8003AA78(0, pData);
}

void OnBdatFileUnloaded(void* pData, u32 length) {
    CBdat::func_8003AA8C(0);
}

void OnAidatFileLoaded(void* pData, u32 length){
    func_8014A86C(pData);
}

void OnAidatFileUnloaded(void* pData, u32 length){
    func_8014A8F8();
}

void OnHbmstopFileLoaded(void* pData, u32 length){
    CLibHbm::loadTplImage(pData);
}

void OnHbmstopFileUnloaded(void* pData, u32 length){
    CLibHbm::removeTplImage();
}

//List of files contained in the static.arc archive
static StaticArcFileData sStaticArcFiles[10] = {
    {"SHA","dvddata/etc/shadow.sha",HANDLE_MEM2,nullptr,nullptr},
    {"CAM","dvddata/etc/cam.chr",HANDLE_MEM2,nullptr,nullptr},
    {"EFF","dvddata/etc/eff.chr",HANDLE_MEM2,nullptr,nullptr},
    {"ARROW","dvddata/etc/arrow.mdo",HANDLE_MEM2,nullptr,nullptr},
#if defined(VERSION_JP)
    {"43","dvddata/menu/Mode43.arc",HANDLE_MEM2,nullptr,nullptr},
    {"BDAT","dvddata/common/jp/bdat.bin",HANDLE_MEM2,&OnBdatFileLoaded,&OnBdatFileUnloaded},
#else //EU/US
    {"43","dvddata/menu/jp/Mode43.arc",HANDLE_MEM2,nullptr,nullptr},
    {"BDAT","common/jp/bdat_common.bin",HANDLE_MEM2,&OnBdatFileLoaded,&OnBdatFileUnloaded},
#endif
    {"AIDAT","dvddata/etc/ai.bin",HANDLE_MEM2,&OnAidatFileLoaded,&OnAidatFileUnloaded},
    {"HIKARI","dvddata/etc/hikari.brres",HANDLE_MEM2,nullptr,nullptr},
    {"HBMSTOP","dvddata/etc/hbmstop.tpl",HANDLE_MEM2,&OnHbmstopFileLoaded,&OnHbmstopFileUnloaded}
};

static const char* const scStaticArcStr =
#if defined(VERSION_JP)
"static.arc";
#else //EU/US
"lang/jp/static.arc";
#endif

static const char* const sPkhFilenames[13] = {
#if defined(VERSION_JP)
    "ahx.pkh",
    "adx.pkh",
    "chr.pkh",
    "common.pkh",
    "eff.pkh",
    "font.pkh",
    "map.pkh",
    "menu.pkh",
    "obj.pkh",
    "script.pkh",
    "snd.pkh",
    "work.pkh",
    nullptr
#else //EU/US
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
#endif
};

//VM initialization callback functions.

void vmInitPluginRegistCallback(){
    vmInit();
    pluginRegist();
}

void vmInitCallback() {
    vmInit();
}

#ifdef __MWERKS__
void main(){
#else
int main(){
#endif
    // Keep .data base in a named local so MWCC relocates via sGameMainIcon
    // (not ...data.0) and can emit stmw r30/r31 like retail.
    char* dataBase = reinterpret_cast<char*>(&sGameMainIcon);
    lbl_eu_8065FD00[0x125] = 0;
    CBdat::func_8003AA50();
    func_eu_802B12DC();
    func_eu_802B11C0();
    func_eu_804520B0(func_eu_802B14D4());
    func_eu_804520B0(func_eu_802B14E0());
    func_eu_804520B0(func_eu_802B14EC());
#if !defined(VERSION_JP)
    func_eu_804520B0(dataBase + 0x10);
    func_eu_804520B0(dataBase + 0x30);
#endif
    CDeviceFileCri::func_80450B14(func_eu_802B133C());
    CDeviceFileCri::func_80450B1C(func_eu_802B1354());
    CDeviceFileCri::func_80450B24(func_eu_802B136C());
    func_804DAA90(func_eu_802B13E4());
    func_804DAA98(func_eu_802B13FC());
    func_804DAAA0(func_eu_802B1414());
    lbl_eu_80663B18 = 0;
    mtl::MemManager::MemRegion::setRegionMaxSize(0x686000, 0);
    CDeviceVI::setUseStaticHandle(true);
    CDeviceGX::setValues(GX_PF_RGB8_Z24, 0x180000);
    // PLAN.md §17.6: MWCC peeps C `dataBase+0` / even asm `addi r3,r30,0` to
    // `mr r3,r30`. Emit addi encoding as opword + the retail li/bl call site.
    // A live `int clear` local next to this asm reshuffles r30/r31; use 0 lit.
    DECOMP_ASM_INSN_BEGIN
    asm {
        opword 0x387E0000
        li r4, 1
        bl entryTable__8CDesktopFPQ28CDesktop16DESKTOP_ICON_DEFb
    }
    DECOMP_ASM_INSN_END
    CLibStaticData::saveStaticFileArray(reinterpret_cast<StaticArcFileData*>(dataBase + 0x50));
    CLibVM::setCallbacks(&vmInitPluginRegistCallback, &vmInitCallback);
    CWorkSystemPack::SaveStaticArcFilenameStringPtr(&scStaticArcStr);
    CWorkSystemPack::SavePkhFilenamesArrayPtr(sPkhFilenames);
    CActParamData::func_80057CDC();
    CLibHbm::func_8045D5C8(true);
    func_eu_80457318(func_eu_802B136C());
    CWorkRoot::run();
    func_eu_802B1334();
}