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
// US/EU retail-named data.  Every symbol below carries the exact retail linker
// name so the .data/.rodata/.sdata2 relocs match the split object byte-for-byte
// (run.py data diff).  Declaration order inside each group is the retail
// section emission order (verified with a Wii/1.1 -O4,p MWCC probe):
//   .rodata:  GAME_MAIN, "lang/jp/static.arc", 9 long sPkhFilenames strings,
//             sPkhFilenames pointer array, 9 dvddata path strings.
//   .sdata2:  scStaticArcStr (8B pointer pair), adx/eff/map/obj/snd.pkh,
//             6 "/xx/" lang strings, 6 "\\xx\\" lang strings, 9 arc tags.
//   .data:    sGameMainIcon, sLanguageFolderPaths[8], languageFolderPaths[8],
//             sStaticArcFiles[10] (contiguous; main() uses r30 + 0x10/0x30/0x50).

// ---- .rodata strings ----
const char lbl_eu_804FA258[0xA] = "GAME_MAIN";
const char lbl_eu_804FA264[0x13] = "lang/jp/static.arc";
const char lbl_eu_804FA278[0xF] = "ahx/jp/ahx.pkh";
const char lbl_eu_804FA288[0x15] = "common/jp/common.pkh";
const char lbl_eu_804FA2A0[0x17] = "mapbdat/jp/mapbdat.pkh";
const char lbl_eu_804FA2B8[0x11] = "menu/jp/menu.pkh";
const char lbl_eu_804FA2CC[0x15] = "script/jp/script.pkh";
const char lbl_eu_804FA2E4[0xF] = "chr/jp/chr.pkh";
const char lbl_eu_804FA2F4[0x9] = "font.pkh";
// ---- .sdata2 objects (retail .sdata2 0x80665AB8..0x80665B80) ----
static const char* const scStaticArcStr[2] = { lbl_eu_804FA264, nullptr };
const char lbl_eu_80665AC0[8] = "adx.pkh";
const char lbl_eu_80665AC8[8] = "eff.pkh";
const char lbl_eu_80665AD0[8] = "map.pkh";
const char lbl_eu_80665AD8[8] = "obj.pkh";
const char lbl_eu_80665AE0[8] = "snd.pkh";
const char lbl_eu_80665AE8[5] = "/jp/";
const char lbl_eu_80665AF0[5] = "/en/";
const char lbl_eu_80665AF8[5] = "/de/";
const char lbl_eu_80665B00[5] = "/fr/";
const char lbl_eu_80665B08[5] = "/sp/";
const char lbl_eu_80665B10[5] = "/it/";
const char lbl_eu_80665B18[5] = "\\jp\\";
const char lbl_eu_80665B20[5] = "\\en\\";
const char lbl_eu_80665B28[5] = "\\de\\";
const char lbl_eu_80665B30[5] = "\\fr\\";
const char lbl_eu_80665B38[5] = "\\sp\\";
const char lbl_eu_80665B40[5] = "\\it\\";
const char lbl_eu_80665B48[4] = "SHA";
const char lbl_eu_80665B4C[4] = "CAM";
const char lbl_eu_80665B50[4] = "EFF";
const char lbl_eu_80665B54[6] = "ARROW";
const char lbl_eu_80665B5C[3] = "43";
const char lbl_eu_80665B60[5] = "BDAT";
const char lbl_eu_80665B68[6] = "AIDAT";
const char lbl_eu_80665B70[7] = "HIKARI";
const char lbl_eu_80665B78[8] = "HBMSTOP";


// Static.arc per-file load/unload callbacks, registered via sStaticArcFiles
// below and invoked by CLibStaticData when the archive entry is paged in/out.

void OnBdatFileLoaded(void* pData, u32 length) {
    CBdat::func_8003AA78(0, pData);
}

void OnBdatFileUnloaded(void* pData, u32 length) {
    CBdat::getEntry(0);
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

// ---- .rodata pointer array (retail .rodata +0xA8) ----
const char* const sPkhFilenames[13] = {
    lbl_eu_804FA278,
    lbl_eu_804FA288,
    lbl_eu_804FA2A0,
    lbl_eu_804FA2B8,
    lbl_eu_804FA2CC,
    lbl_eu_80665AC0,
    lbl_eu_804FA2E4,
    lbl_eu_80665AC8,
    lbl_eu_804FA2F4,
    lbl_eu_80665AD0,
    lbl_eu_80665AD8,
    lbl_eu_80665AE0,
    nullptr
};

// ---- .rodata dvddata path strings ----
const char lbl_eu_804FA334[0x17] = "dvddata/etc/shadow.sha";
const char lbl_eu_804FA34C[0x14] = "dvddata/etc/cam.chr";
const char lbl_eu_804FA360[0x14] = "dvddata/etc/eff.chr";
const char lbl_eu_804FA374[0x16] = "dvddata/etc/arrow.mdo";
const char lbl_eu_804FA38C[0x1B] = "dvddata/menu/jp/Mode43.arc";
const char lbl_eu_804FA3A8[0x1A] = "common/jp/bdat_common.bin";
const char lbl_eu_804FA3C4[0x13] = "dvddata/etc/ai.bin";
const char lbl_eu_804FA3D8[0x19] = "dvddata/etc/hikari.brres";
const char lbl_eu_804FA3F8[0x18] = "dvddata/etc/hbmstop.tpl";

// ---- .data arrays (contiguous, retail .data 0x80524A80..0x80524B98) ----
DesktopIcon sGameMainIcon = {
    lbl_eu_804FA258,
    &CGame::GameMain
};

const char* sLanguageFolderPaths[8] = {
    lbl_eu_80665AE8,
    lbl_eu_80665AF0,
    lbl_eu_80665AF0,
    lbl_eu_80665AF8,
    lbl_eu_80665B00,
    lbl_eu_80665B08,
    lbl_eu_80665B10,
    nullptr,
};

const char* languageFolderPaths[8] = {
    lbl_eu_80665B18,
    lbl_eu_80665B20,
    lbl_eu_80665B20,
    lbl_eu_80665B28,
    lbl_eu_80665B30,
    lbl_eu_80665B38,
    lbl_eu_80665B40,
    nullptr,
};

//List of files contained in the static.arc archive
StaticArcFileData sStaticArcFiles[10] = {
    {lbl_eu_80665B48, lbl_eu_804FA334, HANDLE_MEM2, nullptr, nullptr},
    {lbl_eu_80665B4C, lbl_eu_804FA34C, HANDLE_MEM2, nullptr, nullptr},
    {lbl_eu_80665B50, lbl_eu_804FA360, HANDLE_MEM2, nullptr, nullptr},
    {lbl_eu_80665B54, lbl_eu_804FA374, HANDLE_MEM2, nullptr, nullptr},
    {lbl_eu_80665B5C, lbl_eu_804FA38C, HANDLE_MEM2, nullptr, nullptr},
    {lbl_eu_80665B60, lbl_eu_804FA3A8, HANDLE_MEM2, &OnBdatFileLoaded, &OnBdatFileUnloaded},
    {lbl_eu_80665B68, lbl_eu_804FA3C4, HANDLE_MEM2, &OnAidatFileLoaded, &OnAidatFileUnloaded},
    {lbl_eu_80665B70, lbl_eu_804FA3D8, HANDLE_MEM2, nullptr, nullptr},
    {lbl_eu_80665B78, lbl_eu_804FA3F8, HANDLE_MEM2, &OnHbmstopFileLoaded, &OnHbmstopFileUnloaded},
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
    // Keep .data base in a named local so MWCC relocates via sGameMainIcon
    // (not ...data.0) and can emit stmw r30/r31 like retail.
    char* dataBase = reinterpret_cast<char*>(&sGameMainIcon);
    lbl_eu_8065FD00[0x125] = 0;
    CBdat::clearEntries();
    func_eu_802B12DC();
    func_eu_802B11C0();
    func_eu_804520B0(func_eu_802B14D4());
    func_eu_804520B0(func_eu_802B14E0());
    func_eu_804520B0(func_eu_802B14EC());
#if !defined(VERSION_JP)
    func_eu_804520B0(dataBase + 0x10);
    func_eu_804520B0(dataBase + 0x30);
#endif
    CDeviceFileCri::setCriFilePathA(func_eu_802B133C());
    CDeviceFileCri::setCriFilePathB(func_eu_802B1354());
    CDeviceFileCri::setCriFilePathC(func_eu_802B136C());
    setGeneralNandErrorMessage(func_eu_802B13E4());
    setSaveNandErrorMessage(func_eu_802B13FC());
    setExtraNandErrorMessage(func_eu_802B1414());
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
    CWorkSystemPack::SaveStaticArcFilenameStringPtr(scStaticArcStr);
    CWorkSystemPack::SavePkhFilenamesArrayPtr(sPkhFilenames);
    CActParamData::func_80057CDC();
    CLibHbm::setHbmActiveFlag(true);
    func_eu_80457318(func_eu_802B136C());
    CWorkRoot::run();
    func_eu_802B1334();
}
#pragma optimize_for_size off
