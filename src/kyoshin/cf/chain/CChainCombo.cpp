#include "kyoshin/cf/chain/CChainCombo.hpp"

namespace cf{
    CChainCombo::CChainCombo(){
        func_80294824(&c);
        w = 0;
        a = 0;
        b = false;
        func_80294834(&c);
        func_802AA338();
    }
    
    void CChainCombo::func1(){
        w = 0;
        a = 0;
        b = false;
        func_80294834(&c);
        func_802AA338();
    }
}

// LLM-HARNESS-BEGIN: us-80296420
extern "C" void func_80293E24() {}
// LLM-HARNESS-END: us-80296420
// LLM-HARNESS-BEGIN: us-802964e8
extern "C" void func_80293EEC() {}
// LLM-HARNESS-END: us-802964e8
