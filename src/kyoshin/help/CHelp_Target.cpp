#include "kyoshin/help/CHelp_Target.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* getCfObjectPc(cf::CfObjectMove* objMove);
extern int isGlobalCamFlagSet(int r3);

// Retail layout shim: CObjectParam lives at offset 0x3E9C in CfObjectPc.
// This matches the retail binary where the vtable for the CfObjectMove
// subobject (containing CObjectParam via CfObject -> CObjectParam) is at +0x3E9C.
struct RetailCfObjectPc {
    u8 pad[0x3E9C];
    cf::CObjectParam objectParam;
};

// Minimal view into CActorParam region for accessing field at offset 0x15F0.
struct CActorParam15F0View {
    u8 pad[0x15F0];
    s32 unk15F0; // enemy type/state id
};

// Minimal view for accessing field at offset 0x91.
struct Object91View {
    u8 pad[0x91];
    s8 unk91; // state/category flag
};

namespace cf{
    // Validate whether the current help target satisfies conditions
    // for this help type. Checks object existence, global state,
    // and type-specific criteria via switch on unkC (1=simple, 2=enemy, 3=object).
    bool CHelp_Target::CHelp_UnkVirtualFunc3(){
        CfObjectMove* objMove = cf::CfGameManager::getPlayer(0);
        CfObjectPc* objPc = getCfObjectPc(objMove);
        if(cf::CfGameManager::isSceneLoading() != 0) return false;
        
        CfGameManager* gameMgr = CfGameManager::getInstance();
        (void)gameMgr; // unused but retail calls it
        
        if(isGlobalCamFlagSet(0x4000000) != 0) return false;
        if(objPc == nullptr) return false;

        // Access CObjectParam at retail offset 0x3E9C via layout shim
        RetailCfObjectPc* retailObj = reinterpret_cast<RetailCfObjectPc*>(objPc);
        if(retailObj->objectParam.CObjectParam_UnkVirtualFunc5() == false) return false;

        void* resultObj;
        
        switch(unkC){
            case 1:
                // Simple existence check
                if(func_800AD860(findObjectById(retailObj->objectParam.CObjectParam_UnkVirtualFunc5())) == nullptr) return false;
            break;
            case 2:
            {
                // Check enemy type field at offset 0x15F0
                resultObj = func_800AD860(findObjectById(retailObj->objectParam.CObjectParam_UnkVirtualFunc5()));
                if(resultObj == nullptr) return false;
                CActorParam15F0View* actorView = static_cast<CActorParam15F0View*>(resultObj);
                if(actorView->unk15F0 == 2) break;
                else return false;
            }
            break;
            case 3:
            {
                // Check object state flag at offset 0x91
                resultObj = getValidObject(findObjectById(retailObj->objectParam.CObjectParam_UnkVirtualFunc5()));
                if(resultObj == nullptr) return false;
                Object91View* objectView = static_cast<Object91View*>(resultObj);
                if(objectView->unk91 == 0xC) break;
                else return false;
            }
            break;
        }

        return true;
    }
}
