#pragma once

#include "monolib/work.hpp"
#include "monolib/device.hpp"
#include "monolib/util.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" const char lbl_eu_80500FA4[];
extern "C" const char* lbl_eu_8052E6F0[];

// C-linkage imports for the bdat helpers.  These are retail-unmangled globals;
// the CBdat static-member forms in ocBdat.hpp mangle the reloc name, so the
// free-function forms are used here where reloc-name-exact codegen is needed.
extern "C" void* func_8003AA34();
extern "C" u32 func_8003B41C(void* bdat);
extern "C" u32 func_8003B1EC(void* bdat);
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* column, s32 index);
extern "C" u32 func_800AA2BC(u32 a, u32 b);
extern "C" int func_800AA33C(ml::FixStr<64>& buf, u32 packed, int prefixFlag, int suffixFlag);
extern "C" u32 func_800AA714(const char* path);
extern "C" u32 func_800AA2E8(u32 a, u32 b, u32 c);
extern "C" u32 func_80086B3C__Q22cf13CfGameManagerFv();

// Retail sbss bdat file-pointer globals (anonymous lbl_eu_* labels in retail).
// The CfBdat static members are kept for the file-data assignment functions.
extern void* lbl_eu_806640A0;   // spLandmarkListFileData
extern void* lbl_eu_806640A8;   // spFldMapListFileData
extern void* lbl_eu_806640F4;   // spItmWpnListFileData
extern void* lbl_eu_806640F8;   // spItmEquipListFileData
extern void* lbl_eu_80664114;   // spItmHeadListFileData
extern void* lbl_eu_80664118;   // spItmBodyListFileData
extern void* lbl_eu_8066411C;   // spItmArmListFileData
extern void* lbl_eu_80664120;   // spItmWaistListFileData
extern void* lbl_eu_80664124;   // spItmLeggListFileData
extern void* lbl_eu_80664190;   // sinit instance pointer
// Cached field-map row state written by func_80141C6C (retail sbss labels).
extern u32 lbl_eu_80664184;     // cached fld-map row index
extern u32 lbl_eu_80664188;     // cached fld-map row (mirror of 80664184)
extern float lbl_eu_8066418C;   // fld-map cache float
// .data block whose address is stored into lbl_eu_80664194 by the sinit.
extern u8 lbl_eu_8052E718[0x88];

namespace cf{

    class CfBdat : IWorkEvent {
    public:
        CfBdat(){}
        virtual ~CfBdat();
        virtual bool OnFileEvent(CEventFile* pEventFile);

        static void func_801414CC();
        static void resetMapBdatFileDataPointers();
        static void loadMapBdatFileDataPointers(int mapId, int areaId);
        static void* func_80141B20(const char* pName);
        static const char* func_801421C4(u16 index);
        static u32 func_801422A8(u32 param1);
        static const char* func_801424A8(u16 index);

        //Pointers to file data for bdat files
        static void* spBtlPcListFileData;
        static void* spBtlEneListFileData;
        static void* spFldNpcListFileData;
        static void* lbl_8066698C;
        static void* spLandmarkListFileData;

        static void* spCurRouteListFileData;
        static void* spFldMapListFileData;
        static void* spCurMapEffListFileData;
        static void* spCurMapObjListFileData;
        static void* spCurMapLodListFileData;
        static void* spCurMapSeListFileData;
        static void* spCurFldGimCamListFileData;
        static void* spCurLItemListFileData;
        static void* spCurExTalkListFileData;
        static void* spCurMineListFileData;
        static void* spCurBtlEneListFileData;

        static void* spFldValPopListFileData;
        static void* spFldDmObjListFileData;
        static void* spBtlSkillListFileData;
        static void* spBtlGrowListFileData;
        static void* spBtlBuffListFileData;
        static void* spFldPointListFileData;
        static void* spFldTboxListFileData;
        static void* spItmItemListFileData;
        static void* lbl_806669E0;

        //Item bdat files
        static void* spItmWpnListFileData;
        static void* spItmEquipListFileData;
        static void* spItmCrystalListFileData;
        static void* spItmDropCrystalListFileData;
        static void* spItmCollectListFileData;
        static void* spItmMaterialListFileData;
        static void* spItmValuableListFileData;
        static void* spItmArtsListFileData;
        static void* spItmHeadListFileData;
        static void* spItmBodyListFileData;
        static void* spItmArmListFileData;
        static void* spItmWaistListFileData;
        static void* spItmLeggListFileData;

        static void* spCurGimListFileData;
        static void* spCurFieldLockFileData;
        static void* spCurElvGmFileData;
        static void* spCurWarpGmFileData;
        static void* spCurJumpGmFileData;
        static void* spCurItemGmFileData;
        static void* spCurGimSvOffFileData;
        static void* spCurGimEneFileData;
        static void* spCurGimMessFileData;
        static void* spCurDropNmlListFileData;
        static void* spCurDropRarListFileData;
        static void* spCurDropSprListFileData;

        static void* spBtlPsvSkillFileData;
        static void* spBtlPsvLinkFileData;
        static void* spBtlPssListFileData;
        static void* spBtlCamListFileData;
        static void* spBtlCamDataListFileData;
        static void* spBtlCrystalNameListFileData;

        static void* spMnuItemFileData;
        static void* spMnuEveStartFileData;
        static void* spMnuEveTableFileData;

        static CFileHandle* lbl_80666A6C;
        static void* lbl_80666A70;
        static u32 lbl_80666A74;
        static void* lbl_80666A78;
        static float lbl_80666A7C;
        static void* lbl_80666A84;

        static void* lbl_80577510[];
        static const char* lbl_80531EC0[];
    };

} //namespace cf

// Data-pool constants referenced by CfBdat.cpp free functions (global scope so
// MWCC emits them unmangled).
extern float lbl_eu_80667368;
extern char lbl_eu_80662228[4];  // fixed size so MWCC keeps the .sdata SDA21 reloc

// ---- static-initializer support (retail __sinit_\CfBdat_cpp) ----
// The retail sinit initializes an anonymous 4-byte prototype object at
// lbl_eu_80664194 (first word = address of the lbl_eu_8052E718 data block)
// and stores the object's address into lbl_eu_80664190.  The inline ctor
// lets MWCC emit the auto-sinit with the ctor body inlined (see CfBdat.cpp).
extern u8 lbl_eu_8052E718[0x88];
extern void* lbl_eu_80664190;

struct CfBdatSinitProto {
    void* mData;   // +0x00: address of lbl_eu_8052E718

    CfBdatSinitProto(){
        mData = (void*)lbl_eu_8052E718;
        lbl_eu_80664190 = this;
    }
};

extern CfBdatSinitProto lbl_eu_80664194;
