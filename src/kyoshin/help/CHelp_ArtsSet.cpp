#include "kyoshin/help/CHelp_ArtsSet.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" bool func_8022F530();

namespace cf{
    bool CHelp_ArtsSet::func_802B7D9C(){
        if(unkC != 0 && CfGameManager::func_800822F4() < unkC){
            return false;
        }
        return func_8022F530();
    }
}
