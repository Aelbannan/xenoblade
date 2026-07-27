#include "kyoshin/cf/chain/CChainCombo.hpp"

namespace cf{
    CChainCombo::CChainCombo(){
        func_80294824__FPv(&mGauge);
        mArtsType = 0;
        mComboCount = 0;
        mPending = false;
        func_80294834__FPv(&mGauge);
        func_802AA338__Fv();
    }
    
    void CChainCombo::func1(){
        mArtsType = 0;
        mComboCount = 0;
        mPending = false;
        func_80294834__FPv(&mGauge);
        func_802AA338__Fv();
    }
}

void func_80293E24(){}
void func_80293EEC(){}
