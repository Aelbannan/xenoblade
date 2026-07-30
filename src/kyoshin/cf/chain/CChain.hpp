#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainMember.hpp"
#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/chain/CChainTime.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"
#include "kyoshin/cf/chain/CChainCombo.hpp"

namespace cf {
    //size: 0x1F0C?
    class CChain {
    public:
        CChain();
        ~CChain(){}
        void func_8027728C();
        int getZero_78E04();
        bool chkActorList();
        u16 getChainCount();
        void setFieldAndClear(int val);
        int getZero_A584();
        int getZero_A9D0();
        int getZero_A9D8();
        int getZero_A9E0();
        int getZero_A9EC();
        int getZero_A9F4();
        int getZero_A9FC();
        int getZero_AA04();

        u8 unk0[0x18];
        CChainActorList mChainActorList; //0x18
        CChainMember mChainMember; //0x1DC8
        CChainTimer mChainTimer1; //0x1EB4
        CChainTimer mChainTimer2; //0x1EBC
        CChainTime mChainTime; //0x1EC4
        CChainChance mChainChance; //0x1EDC
        CChainCombo mChainCombo; //0x1EF4
        u8 unk1F0C[0x14];
    };
}
