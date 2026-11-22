#define IWORK_EVENT_INLINE_DTOR
#include "decomp.h"
#include "kyoshin/cf/CfBdat.hpp"
#include "types.h"

// .data (retail 0xC0): quest name table, then the compiler-emitted CfBdat vtable + RTTI base list.
// Typed .rodata (retail 0x420): type name + bdat table-name pool + face names.
// Sizes are retail gaps so the section packs exactly.


// Typed .sdata (retail 0x8): "pc1" + pad to section size.
__declspec(section ".sdata") __attribute__((used, aligned(8))) char lbl_eu_80662228[8] = "pc1";

// Plain BDAT file-name literals at their real getFP/format use sites.
extern "C" void Bdat_GetTable_AA34();
extern "C" u32 getBdatStringColumnValue(void* b, const char* c, s32 i);
extern "C" void *cfCamEventBdatTableA, *cfCamEventBdatTableB, *gGimmickBdatFile;
extern "C" u32 gKizunagramBdatTableId; extern void* gpBattleCtrlBdatTable;
static void* mapName(void*& dst, const char* fmt, int m, int a) { char n[0x40]; snprintf(n, sizeof(n), fmt, m, a); return dst = getFP(n); }
void cf::CfBdat::loadBdatFileTable() { Bdat_GetTable_AA34();
    ::cf::CfBdat::spBtlPcListFileData = getFP("BTL_pclist");
    ::cf::CfBdat::spBtlEneListFileData = getFP("BTL_enelist");
    ::cf::CfBdat::spBtlSkillListFileData = getFP("BTL_skilllist");
    ::cf::CfBdat::spBtlGrowListFileData = getFP("BTL_growlist");
    ::cf::CfBdat::spBtlBuffListFileData = getFP("BTL_bufflist");
    ::cf::CfBdat::spFldPointListFileData = getFP("FLD_pointlist");
    ::cf::CfBdat::spFldTboxListFileData = getFP("FLD_tboxlist");
    ::cfCamEventBdatTableA = getFP("BTL_camlist");
    ::cfCamEventBdatTableB = getFP("BTL_camdatalist");
    ::cf::CfBdat::spBtlCrystalNameListFileData = getFP("BTL_crystalnamelist");
    ::cf::CfBdat::spMnuItemFileData = getFP("MNU_item");
    ::cf::CfBdat::spFldNpcListFileData = getFP("FLD_npclist");
    ::cf::CfBdat::spFldMapListFileData = getFP("FLD_maplist");
    ::cf::CfBdat::spLandmarkListFileData = getFP("landmarklist");
    ::cf::CfBdat::spFldValPopListFileData = getFP("FLD_valpoplist");
    ::cf::CfBdat::spFldDmObjListFileData = getFP("FLD_dmobjlist");
    ::cf::CfBdat::spItmItemListFileData = getFP("ITM_itemlist");
    ::cf::CfBdat::spItmWpnListFileData = getFP("ITM_wpnlist");
    ::cf::CfBdat::spItmEquipListFileData = getFP("ITM_equiplist");
    ::cf::CfBdat::spItmCrystalListFileData = getFP("ITM_crystallist");
    ::cf::CfBdat::spItmDropCrystalListFileData = getFP("ITM_dropcrystallist");
    ::cf::CfBdat::spItmCollectListFileData = getFP("ITM_collectlist");
    ::cf::CfBdat::spItmMaterialListFileData = getFP("ITM_materiallist");
    ::cf::CfBdat::spItmValuableListFileData = getFP("ITM_valuablelist");
    ::cf::CfBdat::spItmArtsListFileData = getFP("ITM_artslist");
    ::cf::CfBdat::spItmHeadListFileData = getFP("ITM_headlist");
    ::cf::CfBdat::spItmBodyListFileData = getFP("ITM_bodylist");
    ::cf::CfBdat::spItmArmListFileData = getFP("ITM_armlist");
    ::cf::CfBdat::spItmWaistListFileData = getFP("ITM_waistlist");
    lbl_eu_8066418C = 0.0f;
    ::cf::CfBdat::spItmLeggListFileData = getFP("ITM_legglist");
    ::cf::CfBdat::spBtlPsvSkillFileData = getFP("BTL_PSVskill");
    ::gKizunagramBdatTableId = (u32)getFP("BTL_PSVlink");
    ::cf::CfBdat::spBtlPssListFileData = getFP("BTL_PSSlist");
}
// name-guess: no retail/JP map name for this .data table (us lbl_eu_8052E6F0).
const char* sBdatExwpTags[10] = {
    "", "EXwp_L", "EXwp_R", "EXwp_R", "EXwp_S", "EXwp_H",
    "EXwp_HL", "EXwp_HR", "EXwp_HR", 0,
};

void cf::CfBdat::loadMapBdatFileDataPointers(int mapId, int areaId) {
    mapName(::cf::CfBdat::spCurRouteListFileData, "routelist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurMapEffListFileData, "mapefflist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurMapObjListFileData, "mapobjlist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurMapLodListFileData, "maplodlist%02d%02d", mapId, areaId);
    mapName(::gpBattleCtrlBdatTable, "mapselist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurFldGimCamListFileData, "FLD_GimCamList%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurLItemListFileData, "Litemlist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurExTalkListFileData, "extalklist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurMineListFileData, "minelist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurBtlEneListFileData, "BTL_enelist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurGimListFileData, "Gimlist%02d%02d", mapId, areaId);
    mapName((void*&)::gLockBdatTable, "FieldLock%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurElvGmFileData, "ElvGm%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurWarpGmFileData, "WarpGm%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurJumpGmFileData, "JumpGm%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurItemGmFileData, "ItemGm%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurGimSvOffFileData, "GimSVoff%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurGimEneFileData, "GimEne%02d%02d", mapId, areaId);
    mapName(::gGimmickBdatFile, "GimMess%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurDropNmlListFileData, "drop_nmllist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurDropRarListFileData, "drop_rarlist%02d%02d", mapId, areaId);
    mapName(::cf::CfBdat::spCurDropSprListFileData, "drop_sprlist%02d%02d", mapId, areaId);
}
void cf::CfBdat::resetMapBdatFileDataPointers() { void* b = ::cf::CfBdat::spFldMapListFileData;
    getBdatStringColumnValue(b, "resource", 0);
    getBdatStringColumnValue(b, "posX", 0);
    getBdatStringColumnValue(b, "posY", 0);
    getBdatStringColumnValue(b, "posZ", 0);
    getBdatStringColumnValue(b, "pc%d", 0);
    getBdatStringColumnValue(b, "parts", 0);
}

// compiler-owned virtual definitions
cf::CfBdat::CfBdat() {}
cf::CfBdat::~CfBdat() {}

// Typed .sbss
// Zero-fill section: only size (0x98) and align (8) are compared.
// First object carries aligned(8) so the section keeps retail align 8.
void* cf::CfBdat::spBtlPcListFileData __attribute__((aligned(8)));
void* cf::CfBdat::spBtlEneListFileData;
void* cf::CfBdat::spFldNpcListFileData;
void* lbl_eu_8066409C;
void* cf::CfBdat::spLandmarkListFileData;
void* cf::CfBdat::spCurRouteListFileData;
void* cf::CfBdat::spFldMapListFileData;
void* cf::CfBdat::spCurMapEffListFileData;
void* cf::CfBdat::spCurMapObjListFileData;
void* cf::CfBdat::spCurMapLodListFileData;
void* gpBattleCtrlBdatTable;
void* cf::CfBdat::spCurFldGimCamListFileData;
void* cf::CfBdat::spCurLItemListFileData;
void* cf::CfBdat::spCurExTalkListFileData;
void* cf::CfBdat::spCurMineListFileData;
void* cf::CfBdat::spCurBtlEneListFileData;
void* cf::CfBdat::spFldValPopListFileData;
void* cf::CfBdat::spFldDmObjListFileData;
void* cf::CfBdat::spBtlSkillListFileData;
void* cf::CfBdat::spBtlGrowListFileData;
void* cf::CfBdat::spBtlBuffListFileData;
void* cf::CfBdat::spFldPointListFileData;
void* cf::CfBdat::spFldTboxListFileData;
void* cf::CfBdat::spItmItemListFileData;
void* lbl_eu_806640F0;
void* cf::CfBdat::spItmWpnListFileData;
void* cf::CfBdat::spItmEquipListFileData;
void* cf::CfBdat::spItmCrystalListFileData;
void* cf::CfBdat::spItmDropCrystalListFileData;
void* cf::CfBdat::spItmCollectListFileData;
void* cf::CfBdat::spItmMaterialListFileData;
void* cf::CfBdat::spItmValuableListFileData;
void* cf::CfBdat::spItmArtsListFileData;
void* cf::CfBdat::spItmHeadListFileData;
void* cf::CfBdat::spItmBodyListFileData;
void* cf::CfBdat::spItmArmListFileData;
void* cf::CfBdat::spItmWaistListFileData;
void* cf::CfBdat::spItmLeggListFileData;

extern "C" u32 Bdat_GetMaxRow_B1EC(void* bdat);
extern "C" u32 Bdat_GetRowBase_B41C(void* bdat);
extern "C" u32 Tok_ParseFile(const char* path);
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);
extern "C" void* CItem_resolveFamilyBdat(u32 v, void* outA, void* outB);
extern "C" u32 bdatPcPartsRes(u32 param1, s16 param2, u32 param3, u32 param4);


// us-80142a78 - resolve a family/item id into an equip/resource handle.
// Category 2 reads the ITM_wpnlist name column; 4..8 map onto bdatPcPartsRes slots.
extern "C" u32 bdatResolveItem(u32 owner, u32 itemId, u32 flags) {
    if (itemId == 0) {
        return 0;
    }
    u16 category;
    u16 index;
    CItem_resolveFamilyBdat(itemId, &category, &index);
    if (category == 2) {
        return Tok_ParseFile((const char*)getBdatStringColumnValue(
            ::cf::CfBdat::spItmWpnListFileData, "resource", (s32)index));
    }
    u32 slot = 0;
    if (category == 4) {
        slot = 1;
    } else if (category == 5) {
        slot = 2;
    } else if (category == 6) {
        slot = 3;
    } else if (category == 7) {
        slot = 4;
    } else if (category == 8) {
        slot = 5;
    }
    return bdatPcPartsRes(owner, (s16)index, slot, flags);
}

// Weapon-id translator: ids with top-5-bit tag 5 pass through; other
// nonzero ids index the ITM_wpnlist table via its name column.
u32 cf::CfBdat::resolveWpnTok(u32 param1) {
    if (param1 != 0) {
        u32 result;
        void* data;
        if ((param1 >> 27) == 5) {
            return param1;
        }
        result = 0;
        if (param1 & 0xFFFF) {
            data = ::cf::CfBdat::spItmWpnListFileData;
            int total = (int)(Bdat_GetMaxRow_B1EC(data) + Bdat_GetRowBase_B41C(data));
            if ((u16)param1 < total) {
                result = Tok_ParseFile((const char*)getBdatStringColumnValue(
                    data, "resource", (u16)param1));
            }
        }
        return result;
    }
    return 0;
}

const char* cf::CfBdat::getBdatStringEntry(u16 index) {
    return sBdatExwpTags[index];
}

// Remaining table-name literals at their real lookup sites.
extern "C" void* bdatLookupRes(void* bdat, int index) { return (void*)getBdatStringColumnValue(bdat, "resource", index); }
extern "C" void* bdatSetResRow(void* bdat, int index) { return (void*)getBdatStringColumnValue(bdat, "resource", index); }
extern "C" void* bdatUnpackRes(void* bdat, int index) { return (void*)getBdatStringColumnValue(bdat, "resource", index); }
extern "C" void* bdatResByIndex(void* bdat, int index) { return (void*)getBdatStringColumnValue(bdat, "resource", index); }
extern "C" u32 bdatMountValue(void* bdat, int index, int which) {
    const char* col = which ? "bat_mount" : "def_mount";
    return getBdatStringColumnValue(bdat, col, index);
}
extern "C" const char* bdatGetItemName(u16 index) {
    void* items = ::cf::CfBdat::spItmItemListFileData;
    getBdatStringColumnValue(items, "name", index);
    getBdatStringColumnValue(items, "itemType", index);
    getBdatStringColumnValue(items, "itemID", index);
    return "\x96\xA2\x92\xE8\x8B\x60\x83\x41\x83\x43\x83\x65\x83\x80";
}
bool cf::CfBdat::OnFileEvent(CEventFile*) {
    ::cf::CfBdat::spMnuEveStartFileData = getFP("common/jp/bdat_eve.bin");
    ::cf::CfBdat::spMnuEveStartFileData = getFP("MNU_eve_start");
    ::cf::CfBdat::spMnuEveTableFileData = getFP("MNU_eve_table");
    return false;
}
