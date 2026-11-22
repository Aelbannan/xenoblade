#include "kyoshin/help/CHelp_Target.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
#include "kyoshin/cf/CfEnumObject.hpp"

extern cf::CfObjectPc* getCfObjectPc(cf::CfObjectMove* objMove);

// Defined in CfObjectEff.cpp with the real C++ name; a plain declaration
// emits the retail linker symbol.
void* getEffOwner__(void* obj);

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

        if(objPc->CObjectParam_getSelfObjectId() == false) return false;

        switch(unkC){
            case 1:
                // Simple existence check
                if(getEffOwner__(findObjectById(objPc->CObjectParam_getSelfObjectId())) == nullptr) return false;
            break;
            case 2:
            {
                cf::CActorParam* actor = static_cast<cf::CActorParam*>(
                    getEffOwner__(findObjectById(objPc->CObjectParam_getSelfObjectId())));
                if(actor == nullptr) return false;
                if((s32)actor->unk15F0 == 2) break;
                else return false;
            }
            break;
            case 3:
            {
                CfEnumObject* object = static_cast<CfEnumObject*>(
                    getValidObject(findObjectById(objPc->CObjectParam_getSelfObjectId())));
                if(object == nullptr) return false;
                if((s8)object->type91 == 0xC) break;
                else return false;
            }
            break;
        }

        return true;
    }
}
