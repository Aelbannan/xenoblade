#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_800AA008.hpp"
#include "kyoshin/cf/CfMapMineManager.hpp"
#include "monolib/util.hpp"

#include <cstdio>
#include <cstring>
#include <revolution/os/OSFastCast.h>

using namespace ml;

namespace cf{
    void* CfBdat::spBtlPcListFileData;
    void* CfBdat::spBtlEneListFileData;
    void* CfBdat::spFldNpcListFileData;
    void* CfBdat::lbl_8066698C;
    void* CfBdat::spLandmarkListFileData;

    void* CfBdat::spCurRouteListFileData;
    void* CfBdat::spFldMapListFileData;
    void* CfBdat::spCurMapEffListFileData;
    void* CfBdat::spCurMapObjListFileData;
    void* CfBdat::spCurMapLodListFileData;
    void* CfBdat::spCurMapSeListFileData;
    void* CfBdat::spCurFldGimCamListFileData;
    void* CfBdat::spCurLItemListFileData;
    void* CfBdat::spCurExTalkListFileData;
    void* CfBdat::spCurMineListFileData;
    void* CfBdat::spCurBtlEneListFileData;

    void* CfBdat::spFldValPopListFileData;
    void* CfBdat::spFldDmObjListFileData;
    void* CfBdat::spBtlSkillListFileData;
    void* CfBdat::spBtlGrowListFileData;
    void* CfBdat::spBtlBuffListFileData;
    void* CfBdat::spFldPointListFileData;
    void* CfBdat::spFldTboxListFileData;

    void* CfBdat::spItmItemListFileData;
    void* CfBdat::lbl_806669E0;
    void* CfBdat::spItmWpnListFileData;
    void* CfBdat::spItmEquipListFileData;
    void* CfBdat::spItmCrystalListFileData;
    void* CfBdat::spItmDropCrystalListFileData;
    void* CfBdat::spItmCollectListFileData;
    void* CfBdat::spItmMaterialListFileData;
    void* CfBdat::spItmValuableListFileData;
    void* CfBdat::spItmArtsListFileData;
    void* CfBdat::spItmHeadListFileData;
    void* CfBdat::spItmBodyListFileData;
    void* CfBdat::spItmArmListFileData;
    void* CfBdat::spItmWaistListFileData;
    void* CfBdat::spItmLeggListFileData;

    void* CfBdat::spCurGimListFileData;
    void* CfBdat::spCurFieldLockFileData;
    void* CfBdat::spCurElvGmFileData;
    void* CfBdat::spCurWarpGmFileData;
    void* CfBdat::spCurJumpGmFileData;
    void* CfBdat::spCurItemGmFileData;
    void* CfBdat::spCurGimSvOffFileData;
    void* CfBdat::spCurGimEneFileData;
    void* CfBdat::spCurGimMessFileData;
    void* CfBdat::spCurDropNmlListFileData;
    void* CfBdat::spCurDropRarListFileData;
    void* CfBdat::spCurDropSprListFileData;

    void* CfBdat::spBtlPsvSkillFileData;
    void* CfBdat::spBtlPsvLinkFileData;
    void* CfBdat::spBtlPssListFileData;
    void* CfBdat::spBtlCamListFileData;
    void* CfBdat::spBtlCamDataListFileData;
    void* CfBdat::spBtlCrystalNameListFileData;

    void* CfBdat::spMnuItemFileData;
    void* CfBdat::spMnuEveStartFileData;
    void* CfBdat::spMnuEveTableFileData;

    CFileHandle* CfBdat::lbl_80666A6C;
    void* CfBdat::lbl_80666A70;
    u32 CfBdat::lbl_80666A74;
    void* CfBdat::lbl_80666A78;
    float CfBdat::lbl_80666A7C;
    CfBdat CfBdat::lbl_80666A80;
    void* CfBdat::lbl_80666A84;

    
    void* CfBdat::lbl_80577510[28];

    const char* CfBdat::lbl_80531EC0[] = {
        "",
        "EXwp_L",
        "EXwp_R",
        "EXwp_S",
        "EXwp_S",
        "EXwp_H",
        "EXwp_HL",
        "EXwp_HR",
        "EXwp_HR"
    };

    CfBdat::~CfBdat(){
    }

    void CfBdat::func_801414CC(){
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
            CBdat::func_8003AA8C(3);
            DELETE_OBJ(lbl_80666A70);
        }

        lbl_80666A70 = nullptr;
        lbl_80666A6C = nullptr;
    }

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

    //This feels a bit janky, but if it works it works :p
    #define GET_MAP_BDAT_PTR(dest, name) \
    string.format(STR(name)"%02d%02d", mapId, areaId); \
    dest = CBdat::getFP(string.c_str());

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

    const char* CfBdat::func_801421C4(u16 index){
        return nullptr;
    }

    u32 CfBdat::func_801422A8(u32 param1){
        if(param1 != 0){
            if((param1 >> 27) == 5) return param1;
            u32 result = 0;
            if((param1 & 0xFFFF) != 0){
                void* data = spItmWpnListFileData;
                u32 total = CBdat::func_8003B41C(data) + CBdat::func_8003B1EC(data);
                u32 id = param1 & 0xFFFF;
                if((s32)id < (s32)total){
                    result = func_800AA714(CBdat::getBdatStringColumnValue(data, &lbl_eu_80500FA4[0x34f], id));
                }
            }
            return result;
        }
        return 0;
    }

const char* CfBdat::func_801424A8(u16 index) {
    return lbl_eu_8052E6F0[index];
}

    bool CfBdat::OnFileEvent(CEventFile* pEventFile){
        bool result = false;

        if(lbl_80666A6C == pEventFile->mFileHandle){
            if(pEventFile->unk0 == 1 && pEventFile->field_14 != 0){
                CBdat::func_8003AA78(3, lbl_80666A70);
                spMnuEveStartFileData = CBdat::getFP(&lbl_eu_80500FA4[0x3C1]);
                spMnuEveTableFileData = CBdat::getFP(&lbl_eu_80500FA4[0x3CF]);
            }
            
            lbl_80666A6C = nullptr;
            result = true;
        }

        return result;
    }

}

// Free functions (global scope so MWCC keeps them unmangled). These operate
// on the bdat file-data globals: summing row spans, translating a column
// value into an item-id, or scanning for a row whose name column matches.

u32 func_80141B54(){
    // Refresh the bdat singleton, then sum the valid row span of the current
    // route-list table (row base + row count).
    CBdat::func_8003AA34();
    void* fp = cf::CfBdat::spCurRouteListFileData;
    u32 rowCount = CBdat::func_8003B1EC(fp);
    u32 rowBase = CBdat::func_8003B41C(fp);
    return rowBase + rowCount;
}

u32 func_80141FE0(int index){
    // Translate a weapon-list column value into an item-id.
    u32 result = 0;
    if(index != 0){
        void* fp = cf::CfBdat::spItmWpnListFileData;
        int rowBase = (int)CBdat::func_8003B41C(fp);
        int rowCount = (int)CBdat::func_8003B1EC(fp);
        if(index < rowBase + rowCount){
            result = func_800AA714(CBdat::getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x34f], index));
        }
    }
    return result;
}

u32 func_80141BA0(u32 c0, u32 c1){
    // Pack the two name chars into a token, format it into a string, then
    // return the field-map table row whose name column matches (else 0).
    void* fp = cf::CfBdat::spFldMapListFileData;
    FixStr<64> nameBuf;
    func_800AA33C(nameBuf, func_800AA2BC(c0, c1), 0, 0);
    if(fp != nullptr){
        int row = (int)CBdat::func_8003B41C(fp);
        int rowCount = (int)CBdat::func_8003B1EC(fp);
        for(int rowIndex = row; rowIndex < row + rowCount; rowIndex++){
            if(strcmp(nameBuf.c_str(), CBdat::getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x34f], rowIndex)) == 0){
                return (u32)rowIndex;
            }
        }
    }
    return 0;
}

void func_80141C6C(u32 a, u32 b){
    // Reset some cached Bdat pointers, then scan the field-map table for a row
    // whose name column matches the string packed from (a, b), caching the row.
    cf::CfBdat::lbl_80666A7C = lbl_eu_80667368;
    cf::CfBdat::lbl_80666A78 = (void*)cf::CfBdat::lbl_80666A74;
    void* fp = cf::CfBdat::spFldMapListFileData;
    FixStr<64> nameBuf;
    int row = 0;
    func_800AA33C(nameBuf, func_800AA2BC(a, b), 0, 0);
    if(fp != nullptr){
        int rowBase = (int)CBdat::func_8003B41C(fp);
        int rowCount = (int)CBdat::func_8003B1EC(fp);
        int i;
        for(i = rowBase; i < rowBase + rowCount; i++){
            if(strcmp(nameBuf.c_str(), CBdat::getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x34f], i)) == 0) break;
        }
        if(i < rowBase + rowCount) row = i;
    }
    cf::CfBdat::lbl_80666A74 = (u32)row;
}
void func_80141D48(){}
void func_80141DC4(float* out, int index){
    // Landmark bdat table: read three s16 landmark columns (0x358/0x35d/0x362)
    // and expose them as the landmark scale floats (s16->f32 via GQR5 fast cast).
    CBdat::func_8003AA34();
    void* fp = cf::CfBdat::spLandmarkListFileData;
    s16 s0 = *(s16*)CBdat::getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x358], index);
    s16 s1 = *(s16*)CBdat::getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x35d], index);
    s16 s2 = *(s16*)CBdat::getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x362], index);
    out[0] = __OSs16tof32(&s0);
    out[1] = __OSs16tof32(&s1);
    out[2] = __OSs16tof32(&s2);
}
u32 func_80141E90(u32 param1, u32 param2, u32 param3, u32 param4){
    // Skill/arts helper: grind an arts id into an item id, or translate an
    // equip-type byte into the matching item list row.
    u32 result = 0;
    if(param3 != 0){
        if((s32)param2 <= 0 && param3 < 6){
            if(param1 == 8) return func_800AA2E8(param1, 4, param3);
            return func_800AA2E8(param1, 1, param3);
        }
    }
    if(param2 != 0){
        snprintf(lbl_eu_80662228, 8, &lbl_eu_80500FA4[0x367], param1);
        void* table = cf::CfBdat::spItmEquipListFileData;
        u8 v1 = *(const u8*)CBdat::getBdatStringColumnValue(table, lbl_eu_80662228, param2);
        u8 v2 = *(const u8*)CBdat::getBdatStringColumnValue(table, &lbl_eu_80500FA4[0x36c], param2);
        if(param3 == 0) param3 = v2;
        u16 tsel = (u16)(param3 & 0xFFFF);
        void* rt;
        if(tsel == 1) rt = cf::CfBdat::spItmHeadListFileData;
        else if(tsel == 2) rt = cf::CfBdat::spItmBodyListFileData;
        else if(tsel == 3) rt = cf::CfBdat::spItmArmListFileData;
        else if(tsel == 4) rt = cf::CfBdat::spItmWaistListFileData;
        else if(tsel == 5) rt = cf::CfBdat::spItmLeggListFileData;
        else rt = nullptr;
        if(v1 != 0){
            result = func_800AA714(CBdat::getBdatStringColumnValue(rt, &lbl_eu_80500FA4[0x34f], v1));
        }
    }
    return result;
}

u32 func_80142074(void* arg1, u16 arg2, u32 arg3){
    if(arg2 == 0) return 0;
    u16 a;
    s16 b;
    func_80157F04(arg2, &a, &b);
    if(a == 2){
        return func_800AA714(CBdat::getBdatStringColumnValue(cf::CfBdat::spItmWpnListFileData, &lbl_eu_80500FA4[0x34f], (u16)b));
    }
    int t = 0;
    if(a == 4) t = 1;
    else if(a == 5) t = 2;
    else if(a == 6) t = 3;
    else if(a == 7) t = 4;
    else if(a == 8) t = 5;
    return func_80141E90((u32)arg1, (u32)b, (u32)t, arg3);
}
void func_80142154(){}

u32 func_8014235C(u32 param1, const char* column, u32 param3){
    u32 result = 0;
    if(param1 != 0){
        CBdat::func_8003AA34();
        void* fp = cf::CfBdat::spItmWpnListFileData;
        u32 rowBase = CBdat::func_8003B41C(fp);
        u32 rowCount = CBdat::func_8003B1EC(fp);
        if(param1 < rowBase + rowCount){
            u8 val = *(const u8*)CBdat::getBdatStringColumnValue(fp, column, param1);
            if(val == 3) return 1 + !param3;
            if(val == 8) return 6 + !param3;
            if(val == 1 || val == 6){
                if(param3 == 0){
                    result = 0;
                }
            }
        }
    }
    return result;
}
void func_80142428(){}
