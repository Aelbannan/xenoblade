#pragma once

#include "monolib/work.hpp"
#include "monolib/device.hpp"
#include "monolib/util.hpp"
#include "kyoshin/plugin/ocBdat.hpp"

// Opaque bdat file-data table type (actual layout handled by ocBdat helpers).
struct BdatFileData;

// C-linkage imports for the bdat helpers.
extern "C" u32 Tok_ParseFile(const char* path);
// the CBdat static-member forms in ocBdat.hpp mangle the reloc name, so the
// free-function forms are used here where reloc-name-exact codegen is needed.



// Retail sbss bdat file-pointer globals (anonymous lbl_eu_* labels in retail).
// The CfBdat static members are kept for the file-data assignment functions.
extern u8* gLockBdatTable;      // lock-gimmick BDAT table (.sbss)
extern u32 gCachedFldMapRow;     // cached fld-map row (mirror of 80664184)
extern float lbl_eu_8066418C;   // fld-map cache float
// .data block whose address is stored into lbl_eu_80664194 by the sinit.

namespace cf{

    class CfBdat : public IWorkEvent {
    public:
        CfBdat();
        virtual ~CfBdat();
        virtual bool OnFileEvent(CEventFile* pEventFile);

        static void loadBdatFileTable();
        static void resetMapBdatFileDataPointers();
        static void loadMapBdatFileDataPointers(int mapId, int areaId);
        static void* bdatFetchCache(const char* pName);
        static const char* bdatGetItemName(u16 index);
        static u32 resolveWpnTok(u32 param1);
        static const char* getBdatStringEntry(u16 index);

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

        static void* spCurGimListFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurFieldLockFileData;
        static void* spCurElvGmFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurWarpGmFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurJumpGmFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurItemGmFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurGimSvOffFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurGimEneFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurGimMessFileData;
        static void* spCurDropNmlListFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurDropRarListFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spCurDropSprListFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name

        static void* spBtlPsvSkillFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spBtlPsvLinkFileData;
        static void* spBtlPssListFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spBtlCamListFileData;
        static void* spBtlCamDataListFileData;
        static void* spBtlCrystalNameListFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name

        static void* spMnuItemFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spMnuEveStartFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name
        static void* spMnuEveTableFileData;  // name-guess: file-data pointer loaded from the matching BDAT table name

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

// ---- static-initializer support (retail __sinit_\CfBdat_cpp) ----
// The retail sinit initializes an anonymous 4-byte prototype object at
// lbl_eu_80664194 (first word = address of the lbl_eu_8052E718 data block)
// and stores the object's address into lbl_eu_80664190.  The inline ctor
// lets MWCC emit the auto-sinit with the ctor body inlined (see CfBdat.cpp).
extern "C" u32 lbl_eu_8052E718[34];
extern void* lbl_eu_80664190;

struct CfBdatSinitProto {
    void* mData;   // +0x00: address of lbl_eu_8052E718

    CfBdatSinitProto(){
        mData = (void*)lbl_eu_8052E718;
        lbl_eu_80664190 = this;
    }
};

extern CfBdatSinitProto lbl_eu_80664194;
