#include "decomp.h"
#include "types.h"

// Typed .data (retail 0xC0): quest pointer table + IWorkEvent vtable block + RTTI tail.
// Uses u32 + (u32)& casts so function relocs emit as ADDR32 with zero file bytes (retail shape).
extern "C" void __dt__Q22cf6CfBdatFv();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__Q22cf6CfBdatFP10CEventFile();
extern "C" void WorkEvent3__10IWorkEventFPv();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" char lbl_eu_80662230[];

// Typed .rodata (retail 0x420): type name + bdat table-name pool + face names.
// Sizes are retail gaps so the section packs exactly.
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80500F98[0x0C] = "cf::CfBdat";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80500FA4[0x3E4] =
    "BTL_pclist\0"
    "BTL_enelist\0"
    "BTL_skilllist\0"
    "BTL_growlist\0"
    "BTL_bufflist\0"
    "FLD_pointlist\0"
    "FLD_tboxlist\0"
    "BTL_camlist\0"
    "BTL_camdatalist\0"
    "BTL_crystalnamelist\0"
    "MNU_item\0"
    "FLD_npclist\0"
    "FLD_maplist\0"
    "landmarklist\0"
    "FLD_valpoplist\0"
    "FLD_dmobjlist\0"
    "ITM_itemlist\0"
    "ITM_wpnlist\0"
    "ITM_equiplist\0"
    "ITM_crystallist\0"
    "ITM_dropcrystallist\0"
    "ITM_collectlist\0"
    "ITM_materiallist\0"
    "ITM_valuablelist\0"
    "ITM_artslist\0"
    "ITM_headlist\0"
    "ITM_bodylist\0"
    "ITM_armlist\0"
    "ITM_waistlist\0"
    "ITM_legglist\0"
    "BTL_PSVskill\0"
    "BTL_PSVlink\0"
    "BTL_PSSlist\0"
    "routelist%02d%02d\0"
    "mapefflist%02d%02d\0"
    "mapobjlist%02d%02d\0"
    "maplodlist%02d%02d\0"
    "mapselist%02d%02d\0"
    "FLD_GimCamList%02d%02d\0"
    "Litemlist%02d%02d\0"
    "extalklist%02d%02d\0"
    "minelist%02d%02d\0"
    "BTL_enelist%02d%02d\0"
    "Gimlist%02d%02d\0"
    "FieldLock%02d%02d\0"
    "ElvGm%02d%02d\0"
    "WarpGm%02d%02d\0"
    "JumpGm%02d%02d\0"
    "ItemGm%02d%02d\0"
    "GimSVoff%02d%02d\0"
    "GimEne%02d%02d\0"
    "GimMess%02d%02d\0"
    "drop_nmllist%02d%02d\0"
    "drop_rarlist%02d%02d\0"
    "drop_sprlist%02d%02d\0"
    "resource\0"
    "posX\0"
    "posY\0"
    "posZ\0"
    "pc%d\0"
    "parts\0"
    "bat_mount\0"
    "def_mount\0"
    "name\0"
    "itemType\0"
    "itemID\0"
    "\x96\xA2\x92\xE8\x8B\x60\x83\x41\x83\x43\x83\x65\x83\x80\0"
    "common/jp/bdat_eve.bin\0"
    "MNU_eve_start\0"
    "MNU_eve_table";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80501388[0x0C] = "face_n01";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_80501394[0x0C] = "face_m01";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805013A0[0x0C] = "face_n02";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_805013AC[0x0C] = "face_m02";

// Typed .sdata (retail 0x8): "pc1" + pad to section size.
__declspec(section ".sdata") __attribute__((used, aligned(8))) char lbl_eu_80662228[8] = "pc1";

// Typed .sdata2 (retail 0x60): floats, EXwp tags, face pointers, 2^52, 4.0, 1.0.
// Struct freezes retail declaration order (MWCC reorders loose floats by first-use).
struct Sdata2_CfBdat {
    float f0;          // +0x00 lbl_eu_80667368 0.0f (lfs pool)
    char empty[4];     // +0x04 lbl_eu_8066736C "" (empty string for quest table slot 0)
    char exL[8];        // +0x08 lbl_eu_80667370 "EXwp_L"
    char exR[8];        // +0x10 lbl_eu_80667378 "EXwp_R"
    char exS[8];        // +0x18 lbl_eu_80667380 "EXwp_S"
    char exH[8];        // +0x20 lbl_eu_80667388 "EXwp_H"
    char exHL[8];       // +0x28 lbl_eu_80667390 "EXwp_HL"
    char exHR[8];       // +0x30 lbl_eu_80667398 "EXwp_HR"
    float zero2[2];     // +0x38 lbl_eu_806673A0 {0.0f, 0.0f}
    const char* face[4];// +0x40 lbl_eu_806673A8..B4 -> face_n01/m01/n02/m02
    double d52;         // +0x50 lbl_eu_806673B8 2^52 (int->double magic)
    float f4;           // +0x58 lbl_eu_806673C0 4.0f
    float f1;           // +0x5C lbl_eu_806673C4 1.0f
};
__declspec(section ".sdata2") __attribute__((used, aligned(8))) const Sdata2_CfBdat sdata2_CfBdat = {
    0.0f, "", "EXwp_L", "EXwp_R", "EXwp_S", "EXwp_H", "EXwp_HL", "EXwp_HR",
    {0.0f, 0.0f}, {lbl_eu_80501388, lbl_eu_80501394, lbl_eu_805013A0, lbl_eu_805013AC},
    4503599627370496.0, 4.0f, 1.0f
};
#define lbl_eu_80667368 sdata2_CfBdat.f0
#define lbl_eu_8066736C sdata2_CfBdat.empty
#define lbl_eu_80667370 sdata2_CfBdat.exL
#define lbl_eu_80667378 sdata2_CfBdat.exR
#define lbl_eu_80667380 sdata2_CfBdat.exS
#define lbl_eu_80667388 sdata2_CfBdat.exH
#define lbl_eu_80667390 sdata2_CfBdat.exHL
#define lbl_eu_80667398 sdata2_CfBdat.exHR
#define lbl_eu_806673A0 sdata2_CfBdat.zero2
#define lbl_eu_806673B8 sdata2_CfBdat.d52
#define lbl_eu_806673C0 sdata2_CfBdat.f4
#define lbl_eu_806673C4 sdata2_CfBdat.f1

extern "C" u32 lbl_eu_8052E6F0[10] = {
    (u32)&sdata2_CfBdat.empty, (u32)&sdata2_CfBdat.exL, (u32)&sdata2_CfBdat.exR,
    (u32)&sdata2_CfBdat.exR, (u32)&sdata2_CfBdat.exS, (u32)&sdata2_CfBdat.exH,
    (u32)&sdata2_CfBdat.exHL, (u32)&sdata2_CfBdat.exHR, (u32)&sdata2_CfBdat.exHR, 0x00000000,
};
extern "C" u32 lbl_eu_8052E718[34] = {
    (u32)&lbl_eu_80662230, 0x00000000, (u32)&__dt__Q22cf6CfBdatFv, (u32)&WorkEvent1__10IWorkEventFPvPCc,
    (u32)&OnFileEvent__Q22cf6CfBdatFP10CEventFile, (u32)&WorkEvent3__10IWorkEventFPv,
    (u32)&WorkEvent4__10IWorkEventFv, (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
};
extern "C" u32 lbl_eu_8052E7A0[4] = {
    (u32)&__RTTI__10IWorkEvent, 0x00000000, 0x00000000, 0x00000000,
};

// Typed .sbss (retail 0x98 = 38 words): bdat file-data pointers 80664090..8124.
// Zero-fill section: only size (0x98) and align (8) are compared.
// First object carries aligned(8) so the section keeps retail align 8.
void* lbl_eu_80664090 __attribute__((aligned(8)));
void* lbl_eu_80664094;
void* lbl_eu_80664098;
void* lbl_eu_8066409C;
void* lbl_eu_806640A0;
void* lbl_eu_806640A4;
void* lbl_eu_806640A8;
void* lbl_eu_806640AC;
void* lbl_eu_806640B0;
void* lbl_eu_806640B4;
void* lbl_eu_806640B8;
void* lbl_eu_806640BC;
void* lbl_eu_806640C0;
void* lbl_eu_806640C4;
void* lbl_eu_806640C8;
void* lbl_eu_806640CC;
void* lbl_eu_806640D0;
void* lbl_eu_806640D4;
void* lbl_eu_806640D8;
void* lbl_eu_806640DC;
void* lbl_eu_806640E0;
void* lbl_eu_806640E4;
void* lbl_eu_806640E8;
void* lbl_eu_806640EC;
void* lbl_eu_806640F0;
void* lbl_eu_806640F4;
void* lbl_eu_806640F8;
void* lbl_eu_806640FC;
void* lbl_eu_80664100;
void* lbl_eu_80664104;
void* lbl_eu_80664108;
void* lbl_eu_8066410C;
void* lbl_eu_80664110;
void* lbl_eu_80664114;
void* lbl_eu_80664118;
void* lbl_eu_8066411C;
void* lbl_eu_80664120;
void* lbl_eu_80664124;

extern "C" u32 Bdat_GetMaxRow_B1EC(void* bdat);
extern "C" u32 Bdat_GetRowBase_B41C(void* bdat);
extern "C" u32 func_800AA714(const char* path);
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);

namespace cf {
class CfBdat {
public:
    static u32 func_801422A8(u32 param1);
};
}

// Weapon-id translator: ids with top-5-bit tag 5 pass through; other
// nonzero ids index the ITM_wpnlist table via its name column.
u32 cf::CfBdat::func_801422A8(u32 param1) {
    if (param1 != 0) {
        u32 result;
        void* data;
        if ((param1 >> 27) == 5) {
            return param1;
        }
        result = 0;
        if (param1 & 0xFFFF) {
            data = lbl_eu_806640F4;
            int total = (int)(Bdat_GetMaxRow_B1EC(data) + Bdat_GetRowBase_B41C(data));
            if ((u16)param1 < total) {
                result = func_800AA714((const char*)getBdatStringColumnValue(
                    data, &lbl_eu_80500FA4[0x34f], (u16)param1));
            }
        }
        return result;
    }
    return 0;
}

// --- restored from git history (base:gone); do not expand beyond these functions ---
// from commit 2c8ff987ff65 needle=getBdatStringEntry

const char* CfBdat::getBdatStringEntry(u16 index) {
    return lbl_eu_8052E6F0[index];
}

// from commit 2c8ff987ff65 needle=loadBdatFileTable

    void CfBdat::loadBdatFileTable(){
        CBdat::func_8003AA34();
        spBtlPcListFileData = CBdat::getFP("BTL_pclist");
        spBtlEneListFileData = CBdat::getFP("BTL_enelist");
        spBtlSkillListFileData = CBdat::getFP("BTL_skilllist");
        spBtlGrowListFileData = CBdat::getFP("BTL_growlist");
        spBtlBuffListFileData = CBdat::getFP("BTL_bufflist");
        spFldPointListFileData = CBdat::getFP("FLD_pointlist");
        spFldTboxListFileData = CBdat::getFP("FLD_tboxlist");
        spBtlCamListFileData = CBdat::getFP("BTL_camlist");
        spBtlCamDataListFileData = CBdat::getFP("BTL_camdatalist");
        spBtlCrystalNameListFileData = CBdat::getFP("BTL_crystalnamelist");
        spMnuItemFileData = CBdat::getFP("MNU_item");
        spFldNpcListFileData = CBdat::getFP("FLD_npclist");
        spFldMapListFileData = CBdat::getFP("FLD_maplist");
        spLandmarkListFileData = CBdat::getFP("landmarklist");
        spFldValPopListFileData = CBdat::getFP("FLD_valpoplist");
        spFldDmObjListFileData = CBdat::getFP("FLD_dmobjlist");
        spItmItemListFileData = CBdat::getFP("ITM_itemlist");
        spItmWpnListFileData = CBdat::getFP("ITM_wpnlist");
        spItmEquipListFileData = CBdat::getFP("ITM_equiplist");
        spItmCrystalListFileData = CBdat::getFP("ITM_crystallist");
        spItmDropCrystalListFileData = CBdat::getFP("ITM_dropcrystallist");
        spItmCollectListFileData = CBdat::getFP("ITM_collectlist");
        spItmMaterialListFileData = CBdat::getFP("ITM_materiallist");
        spItmValuableListFileData = CBdat::getFP("ITM_valuablelist");
        spItmArtsListFileData = CBdat::getFP("ITM_artslist");
        spItmHeadListFileData = CBdat::getFP("ITM_headlist");
        spItmBodyListFileData = CBdat::getFP("ITM_bodylist");
        spItmArmListFileData = CBdat::getFP("ITM_armlist");
        spItmWaistListFileData = CBdat::getFP("ITM_waistlist");
        spItmLeggListFileData = CBdat::getFP("ITM_legglist");

        lbl_80666A74 = 0;
        lbl_80666A78 = nullptr;
        lbl_80666A7C = 0;
        lbl_8066698C = nullptr;
        
        spBtlPsvSkillFileData = CBdat::getFP("BTL_PSVskill");
        spBtlPsvLinkFileData = CBdat::getFP("BTL_PSVlink");
        spBtlPssListFileData = CBdat::getFP("BTL_PSSlist");

        const char* temp[ARRAY_SIZE(lbl_80577510)] = {
            "JNL_quest0000",
            "JNL_quest0101",
            "JNL_quest0201",
            "JNL_quest0301",
            "JNL_quest0401",
            "JNL_quest0402",
            "JNL_quest0501",
            "JNL_quest0601",
            "JNL_quest0701",
            "JNL_quest0801",
            "JNL_quest0901",
            "JNL_quest1001",
            "JNL_quest1101",
            "JNL_quest1201",
            "JNL_quest1202",
            "JNL_quest1301",
            "JNL_quest1401",
            "JNL_quest1501",
            "JNL_quest1601",
            "JNL_quest1701",
            "JNL_quest1801",
            "JNL_quest1901",
            "JNL_quest2001",
            "JNL_quest2101",
            "JNL_quest2201",
            "JNL_quest2301",
            "JNL_quest2401"
        };

        for(int i = 0; i < (int)ARRAY_SIZE(lbl_80577510); i++){
            lbl_80577510[i] = CBdat::getFP(temp[i]);
        }

        resetMapBdatFileDataPointers();

        if(lbl_80666A6C != nullptr){
            CDeviceFile::cancel(lbl_80666A6C);
            lbl_80666A6C = nullptr;
        }

        if(lbl_80666A70 != nullptr){
            CBdat::getEntry(3);
            DELETE_OBJ(lbl_80666A70);
        }

        lbl_80666A70 = nullptr;
        lbl_80666A6C = nullptr;
    }

// from commit 6d2ef546718a needle=func_80141B54

// LLM-HARNESS-BEGIN: us-80142558
extern "C" void func_80141B54() {}
// LLM-HARNESS-END: us-80142558
// LLM-HARNESS-BEGIN: us-801425a4
extern "C" void func_80141BA0() {}
// LLM-HARNESS-END: us-801425a4
// LLM-HARNESS-BEGIN: us-80142670
extern "C" void func_80141C6C() {}
// LLM-HARNESS-END: us-80142670
// LLM-HARNESS-BEGIN: us-8014274c
extern "C" void func_80141D48() {}
// LLM-HARNESS-END: us-8014274c
// LLM-HARNESS-BEGIN: us-801427c8
extern "C" void func_80141DC4() {}
// LLM-HARNESS-END: us-801427c8
// LLM-HARNESS-BEGIN: us-80142894
extern "C" void func_80141E90() {}
// LLM-HARNESS-END: us-80142894
// LLM-HARNESS-BEGIN: us-801429e4
extern "C" void func_80141FE0() {}
// LLM-HARNESS-END: us-801429e4
// LLM-HARNESS-BEGIN: us-80142a78
extern "C" void func_80142074() {}
// LLM-HARNESS-END: us-80142a78
// LLM-HARNESS-BEGIN: us-80142b58
extern "C" void func_80142154() {}
// LLM-HARNESS-END: us-80142b58
// LLM-HARNESS-BEGIN: us-80142d60
extern "C" void func_8014235C() {}
// LLM-HARNESS-END: us-80142d60
// LLM-HARNESS-BEGIN: us-80142e2c
extern "C" void func_80142428() {}

// from commit 6d2ef546718a needle=func_80141FE0
// LLM-HARNESS-END: us-80142894
// LLM-HARNESS-BEGIN: us-801429e4
extern "C" void func_80141FE0() {}
// LLM-HARNESS-END: us-801429e4
// LLM-HARNESS-BEGIN: us-80142a78
extern "C" void func_80142074() {}
// LLM-HARNESS-END: us-80142a78
// LLM-HARNESS-BEGIN: us-80142b58
extern "C" void func_80142154() {}
// LLM-HARNESS-END: us-80142b58
// LLM-HARNESS-BEGIN: us-80142d60
extern "C" void func_8014235C() {}
// LLM-HARNESS-END: us-80142d60
// LLM-HARNESS-BEGIN: us-80142e2c
extern "C" void func_80142428() {}

// from commit c253729b6102 needle=func_80141B20

    void* CfBdat::func_80141B20(const char* pName){
        void* fileDataPtr;

        if(pName != nullptr){
            fileDataPtr = CBdat::getFP(pName);
        }else{
            fileDataPtr = nullptr;
        }

        lbl_8066698C = fileDataPtr;
        return fileDataPtr;
    }

// from commit c253729b6102 needle=func_801421C4

    const char* CfBdat::func_801421C4(u16 index){
        return nullptr;
    }

// from commit c253729b6102 needle=loadMapBdatFileDataPointers

    void CfBdat::loadMapBdatFileDataPointers(int mapId, int areaId){
        FixStr<64> string;

        GET_MAP_BDAT_PTR(spCurRouteListFileData, routelist);
        GET_MAP_BDAT_PTR(spCurMapEffListFileData, mapefflist);
        GET_MAP_BDAT_PTR(spCurMapObjListFileData, mapobjlist);
        GET_MAP_BDAT_PTR(spCurMapLodListFileData, maplodlist);
        GET_MAP_BDAT_PTR(spCurMapSeListFileData, mapselist);
        GET_MAP_BDAT_PTR(spCurFldGimCamListFileData, FLD_GimCamList);
        GET_MAP_BDAT_PTR(spCurLItemListFileData, Litemlist);
        GET_MAP_BDAT_PTR(spCurExTalkListFileData, extalklist);
        GET_MAP_BDAT_PTR(spCurMineListFileData, minelist);
        GET_MAP_BDAT_PTR(spCurBtlEneListFileData, BTL_enelist);
        GET_MAP_BDAT_PTR(spCurGimListFileData, Gimlist);
        GET_MAP_BDAT_PTR(spCurFieldLockFileData, FieldLock);
        GET_MAP_BDAT_PTR(spCurElvGmFileData, ElvGm);
        GET_MAP_BDAT_PTR(spCurWarpGmFileData, WarpGm);
        GET_MAP_BDAT_PTR(spCurJumpGmFileData, JumpGm);
        GET_MAP_BDAT_PTR(spCurItemGmFileData, ItemGm);
        GET_MAP_BDAT_PTR(spCurGimSvOffFileData, GimSVoff);
        GET_MAP_BDAT_PTR(spCurGimEneFileData, GimEne);
        GET_MAP_BDAT_PTR(spCurGimMessFileData, GimMess);
        GET_MAP_BDAT_PTR(spCurDropNmlListFileData, drop_nmllist);
        GET_MAP_BDAT_PTR(spCurDropRarListFileData, drop_rarlist);
        GET_MAP_BDAT_PTR(spCurDropSprListFileData, drop_sprlist);
    }

// from commit c253729b6102 needle=resetMapBdatFileDataPointers

    void CfBdat::resetMapBdatFileDataPointers(){
        spCurRouteListFileData = nullptr;
        spCurMapEffListFileData = nullptr;
        spCurMapObjListFileData = nullptr;
        spCurMapLodListFileData = nullptr;
        spCurMapSeListFileData = nullptr;
        spCurFldGimCamListFileData = nullptr;
        spCurLItemListFileData = nullptr;
        spCurExTalkListFileData = nullptr;
        spCurMineListFileData = nullptr;
        spCurBtlEneListFileData = nullptr;
        spCurGimListFileData = nullptr;
        spCurFieldLockFileData = nullptr;
        spCurElvGmFileData = nullptr;
        spCurWarpGmFileData = nullptr;
        spCurJumpGmFileData = nullptr;
        spCurItemGmFileData = nullptr;
        spCurGimSvOffFileData = nullptr;
        spCurGimEneFileData = nullptr;
        spCurGimMessFileData = nullptr;
        spCurDropNmlListFileData = nullptr;
        spCurDropRarListFileData = nullptr;
        spCurDropSprListFileData = nullptr;
    }

