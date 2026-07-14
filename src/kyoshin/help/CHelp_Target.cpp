#include "kyoshin/help/CHelp_Target.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"

extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);
extern UNKWORD func_8006EF04(UNKWORD r3);

namespace cf{
    bool CHelp_Target::CHelp_UnkVirtualFunc3(){
        struct RetailCfObjectPc {
            u8 pad[0x3E9C];
            CObjectParam objectParam;
        };

        CfObjectMove* objMove = cf::CfGameManager::func_80082D54(0);
        CfObjectPc* objPc = func_800BFC68(objMove);
        if(cf::CfGameManager::func_800829B8() != 0) return false;
        
        CfGameManager* unused = CfGameManager::getInstance(); //sure, just don't use it
        
        if(func_8006EF04(0x4000000) != 0) return false;
        if(objPc == nullptr) return false;
        RetailCfObjectPc* retailObjPc = reinterpret_cast<RetailCfObjectPc*>(objPc);
        if(retailObjPc->objectParam.CObjectParam_UnkVirtualFunc5() == false) return false;

        UNKTYPE* smthElse;
        UNKTYPE* smthElse1;
        
        switch(unkC){
            case 1:
                if(func_800AD860(func_800B708C(retailObjPc->objectParam.CObjectParam_UnkVirtualFunc5())) == nullptr) return false;
            break;
            case 2:
                smthElse = func_800AD860(func_800B708C(retailObjPc->objectParam.CObjectParam_UnkVirtualFunc5()));
                if(smthElse == nullptr) return false;
                if(*(int*)((u32)smthElse + 0x15F0) == 2) break;
                else return false;
            break;
            case 3:
                smthElse1 = func_800C1228(func_800B708C(retailObjPc->objectParam.CObjectParam_UnkVirtualFunc5()));
                if(smthElse1 == nullptr) return false;
                if(*(s8*)((u32)smthElse1 + 0x91) == 0xC) break;
                else return false;
            break;
        }

        return true;
    }
}
