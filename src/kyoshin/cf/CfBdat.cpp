// Retail's ~CfBdat elides the empty base-dtor call; the inline-empty
// IWorkEvent dtor shape reproduces that (retail ~CfBdat is 0x40, not 0x58).
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CfMapMineManager.hpp"
#include "monolib/util.hpp"

#include <cstdio>
#include <cstring>
#include <revolution/os/OSFastCast.h>

using namespace ml;

// Reads a single byte/halfword column from a bdat table: retail stores the
// raw column word to its own frame slot, then reloads the low half
// (stw + lhz round-trip).
union BdatColS {
    u32 w;
    s16 h;
};

// Unsigned variant: lzh reload of the low half.
union BdatColU {
    u32 w;
    u16 h;
};

// ---- static-initializer support (retail __sinit_\CfBdat_cpp) ----
// Retail's sinit initializes an anonymous 4-byte prototype object at
// lbl_eu_80664194 whose first word is the address of the lbl_eu_8052E718
// data block, then stores the object's address into lbl_eu_80664190.  The
// non-trivial ctor on the global object below makes MWCC emit the auto-sinit
// that inlines this ctor (data-pointer store + instance-pointer store).
u8 lbl_eu_8052E718[0x88];
void* lbl_eu_80664190;
void* lbl_eu_806640A0;   // spLandmarkListFileData (data_defs.cpp skips this address)

CfBdatSinitProto lbl_eu_80664194;

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
        // Equip-slot name lookup: read the row's equip-type column (+0x38b)
        // and name column (+0x394) from the shared equip table, then use the
        // type to select one of the per-type item tables and return that
        // table's name column for the row.
        // Address-of deref reads keep the raw column words in frame slots
        // (retail stw/lhz round-trip; see MWCC_CASES func_800BF920).
        u32 typeVal;
        u16 rowName;
        u32 typeRaw;
        u32 nameRaw;
        typeRaw = getBdatStringColumnValue(lbl_eu_806640EC, &lbl_eu_80500FA4[0x38b], index);
        typeVal = *(const u16*)&typeRaw;
        nameRaw = getBdatStringColumnValue(lbl_eu_806640EC, &lbl_eu_80500FA4[0x394], index);
        rowName = *(const u16*)&nameRaw;
        if(index == 0){
            return &lbl_eu_80500FA4[0x39b];
        }
        // Per-equip-type file-data table: index 3 shares the equip list,
        // indexes 4-8 all map to ITM_equiplist.
        BdatFileData* tables[] = {
            (BdatFileData*)nullptr,
            lbl_eu_806640F0,
            lbl_eu_806640F4,
            lbl_eu_806640D8,
            lbl_eu_806640F8,
            lbl_eu_806640F8,
            lbl_eu_806640F8,
            lbl_eu_806640F8,
            lbl_eu_806640F8,
            lbl_eu_806640FC,
            lbl_eu_80664104,
            lbl_eu_80664108,
            lbl_eu_8066410C,
            lbl_eu_80664110,
        };
        return (const char*)getBdatStringColumnValue(tables[typeVal], &lbl_eu_80500FA4[0x386], rowName);
    }

    u32 CfBdat::func_801422A8(u32 param1){
        // Weapon-id translator: ids with top-5-bit tag 5 pass through; other
        // nonzero ids index the ITM_wpnlist table via its name column.
        if(param1 != 0){
            void* data;
            if((param1 >> 27) == 5){
                return param1;
            }
            u32 result = 0;
            if(param1 & 0xFFFF){
                data = lbl_eu_806640F4;
                int total = (int)(func_8003B1EC(data) + func_8003B41C(data));
                if((u16)param1 < total){
                    result = func_800AA714((const char*)getBdatStringColumnValue(data, &lbl_eu_80500FA4[0x34f], (u16)param1));
                }
            }
            return result;
        }
        return 0;
    }

const char* CfBdat::getBdatStringEntry(u16 index) {
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
            result = func_800AA714((const char*)getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x34f], index));
        }
    }
    return result;
}

u32 func_80141BA0(u32 c0, u32 c1){
    // Pack the two name chars into a token, format it into a string, then
    // return the field-map table row whose name column matches (else 0).
    FixStr<64> nameBuf;
    void* fp = lbl_eu_806640A8;
    func_800AA33C(nameBuf, func_800AA2BC(c0, c1), 0, 0);
    if(fp != nullptr){
        int row = (int)func_8003B41C(fp);
        for(int end = row + (int)func_8003B1EC(fp), rowIndex = row; rowIndex < end; rowIndex++){
            if(strcmp(nameBuf.c_str(), (const char*)getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x34f], rowIndex)) == 0){
                return (u32)rowIndex;
            }
        }
    }
    return 0;
}

void func_80141C6C(u32 a, u32 b){
    // Reset some cached Bdat pointers, then scan the field-map table for a row
    // whose name column matches the string packed from (a, b), caching the row.
    // The reset/shared-store layout mirrors retail: fp==0 and the not-found
    // loop exit both fall into the same `li i,0` before the single store.
    const char* nameCol;
    int end;
    int row;
    u8* fp;
    u32 prevRow;
    // Cached-state update: retail loads both int values before the float
    // copy, then stores (load-load-load / store-store schedule).
    prevRow = lbl_eu_80664184;
    fp = (u8*)lbl_eu_806640A8;
    lbl_eu_8066418C = lbl_eu_80667368;
    lbl_eu_80664188 = prevRow;
    FixStr<64> nameBuf;
    func_800AA33C(nameBuf, func_800AA2BC(a, b), 0, 0);
    if(fp == nullptr) goto reset;
    {
        row = (int)func_8003B41C(fp);
        end = row + (int)func_8003B1EC(fp);
        // Column-name base stays in a register across the loop; the +0x34f
        // column offset is applied per iteration.
        nameCol = lbl_eu_80500FA4;
        for(; row < end; ){
            if(strcmp(nameBuf.c_str(), (const char*)getBdatStringColumnValue(fp, &nameCol[0x34f], row)) == 0){
                goto store;
            }
            row++;
        }
    }
reset:
    row = 0;
store:
    lbl_eu_80664184 = (u32)row;
}
u32 func_80141D48(u16* outA, u16* outB, int index){
    // Fld-map row lookup: resolve the row's name column into an id, then split
    // the id into two packed u16 fields and a validity flag.
    func_8003AA34();
    u32 id = func_800AA714((const char*)getBdatStringColumnValue(lbl_eu_806640A8, &lbl_eu_80500FA4[0x34f], index));
    // id layout: bits 13-19 -> outA, bits 0-9 -> outB (retail uses rotl masks)
    *outA = ((id << 12) | (id >> 20)) & 0x7F;
    *outB = ((id << 22) | (id >> 10)) & 0x3FF;
    return ((0 - id) | id) >> 31;
}
// Signed variant of CfGimmickItem's BdatCol spill union.

void func_80141DC4(float* out, int index){
    // Landmark bdat table: read three s16 landmark columns (0x358/0x35d/0x362)
    // and expose them as the landmark scale floats (s16->f32 via GQR5 fast cast).
    func_8003AA34();
    void* fp = lbl_eu_806640A0;
    volatile BdatColS c0, c1, c2;
    s16 x, y, z;
    c0.w = getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x358], index);
    x = c0.h;
    c1.w = getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x35d], index);
    y = c1.h;
    c2.w = getBdatStringColumnValue(fp, &lbl_eu_80500FA4[0x362], index);
    z = c2.h;
    OSs16tof32(&x, &out[0]);
    OSs16tof32(&y, &out[1]);
    OSs16tof32(&z, &out[2]);
}
u32 func_80141E90(u32 param1, u32 param2, u32 param3, u32 param4){
    // Skill/arts helper: grind an arts id into an item id, or translate an
    // equip-type byte into the matching item list row.
    u32 result = 0;
    void* table;
    if(param3 != 0){
        if((s32)param2 <= 0 && param3 < 6){
            if(param1 == 8) return func_800AA2E8(param1, 4, param3);
            return func_800AA2E8(param1, 1, param3);
        }
    }
    if(param2 != 0){
        snprintf(lbl_eu_80662228, 8, &lbl_eu_80500FA4[0x367], param1);
        BdatFileData* fpEquip = lbl_eu_806640F8;
        u32 val1 = getBdatStringColumnValue(fpEquip, lbl_eu_80662228, param2);
        // Byte-read the first bytes of the stored u32s: retail spills the call
        // results to the stack then `lbz`es them back - taking the addresses
        // keeps the u32s memory-resident so MWCC reproduces that shape.
        int v1 = *(const u8*)&val1;
        u32 val2 = getBdatStringColumnValue(fpEquip, &lbl_eu_80500FA4[0x36c], param2);
        u8 v2 = *(const u8*)&val2;
        if(param3 == 0) param3 = v2;
        u16 tsel = (u16)(param3 & 0xFFFF);
        void* rt;
        if(tsel == 1) rt = lbl_eu_80664114;
        else if(tsel == 2) rt = lbl_eu_80664118;
        else if(tsel == 3) rt = lbl_eu_8066411C;
        else if(tsel == 4) rt = lbl_eu_80664120;
        else if(tsel == 5) rt = lbl_eu_80664124;
        else rt = nullptr;
        if(v1 != 0){
            result = func_800AA714((const char*)getBdatStringColumnValue(rt, &lbl_eu_80500FA4[0x34f], v1));
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
        return func_800AA714((const char*)getBdatStringColumnValue(cf::CfBdat::spItmWpnListFileData, &lbl_eu_80500FA4[0x34f], (u16)b));
    }
    int t = 0;
    if(a == 4) t = 1;
    else if(a == 5) t = 2;
    else if(a == 6) t = 3;
    else if(a == 7) t = 4;
    else if(a == 8) t = 5;
    return func_80141E90((u32)arg1, (u32)b, (u32)t, arg3);
}
// Reads a single byte column from the current game-manager bdat table:
// mode != 0 selects the +0x372 column, mode == 0 the +0x37c one.
u32 func_80142154(u32 param1, u32 mode){
    u32 result = 0;
    if(param1 != 0){
        func_8003AA34();
        void* fp = (void*)getGlobalWord640F4__Q22cf13CfGameManagerFv();
        const char* col = &lbl_eu_80500FA4[0x37c];
        if(mode != 0) col = &lbl_eu_80500FA4[0x372];
        // Retail spills the call result to the stack and lbz's it back.
        u32 val = getBdatStringColumnValue(fp, col, (s32)param1);
        result = *(const u8*)&val;
    }
    return result;
}

u32 func_8014235C(u32 param1, const char* column, u32 param3){
    u32 result = 0;
    if(param1 != 0){
        func_8003AA34();
        void* fp = (void*)getGlobalWord640F4__Q22cf13CfGameManagerFv();
        u32 rowBase = func_8003B41C(fp);
        u32 rowCount = func_8003B1EC(fp);
        if(param1 < rowBase + rowCount){
            // Retail spills the call result to the stack and lbz's it back.
            u32 val = getBdatStringColumnValue(fp, column, param1);
            result = *(const u8*)&val;
            if(result == 3) return 1 + !param3;
            if(result == 8) return 6 + !param3;
            if(result == 1 || result == 6){
                if(param3 == 0){
                    result = 0;
                }
            }
        }
    }
    return result;
}
// Returns the 1-based index of the first entry in the lbl_eu_8052E6F0 name
// table (entries 1..8) that matches name, or 0 if name is null / not found.
u32 func_80142428(const char* name){
    if(name != nullptr){
        for(u32 i = 1; i < 9; i++){
            if(strcmp(name, lbl_eu_8052E6F0[i]) == 0){
                return i;
            }
        }
    }
    return 0;
}
