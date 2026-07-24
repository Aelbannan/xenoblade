#include "kyoshin/cf/chain/CChainCombo.hpp"

namespace cf{
    CChainCombo::CChainCombo(){
        func_80294824(&mGauge);
        mArtsType = 0;
        mComboCount = 0;
        mPending = false;
        func_80294834(&mGauge);
        func_802AA338();
    }
    
    void CChainCombo::func1(){
        mArtsType = 0;
        mComboCount = 0;
        mPending = false;
        func_80294834(&mGauge);
        func_802AA338();
    }
}

// LLM-HARNESS-BEGIN: us-80296420
extern "C" void func_80293E24() {}
// LLM-HARNESS-END: us-80296420
// LLM-HARNESS-BEGIN: us-802964e8
extern "C" void func_80293EEC() {}
// LLM-HARNESS-END: us-802964e8
