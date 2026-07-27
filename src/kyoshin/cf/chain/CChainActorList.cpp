#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChainChance.hpp"

namespace cf {
    CChainActorList::CChainActorList(){

    }

    CChainActorList::~CChainActorList(){
        
    }
}

// Initializes partial CChainChance fields to 0
// Called from CChain constructor during chain attack chance setup
void func_8027C098(cf::CChainChance* self) {
    self->mChainCount = 0;
    self->mField08 = 0;
    self->mField0A = 0;
    self->mField0C = 0;
}
void func_8027C5CC(){}
void func_8027CA98(){}

void func_8027B164(){}
void func_8027B200(){}
void func_8027B2CC(){}
void func_8027B770(){}
void func_8027B814(){}
void func_8027B8C8(){}
void func_8027BA0C(){}
void func_8027BB4C(){}
void func_8027BC14(){}
void func_8027BE84(){}
void func_8027BF58(){}
void func_8027EEF4(int);

void func_8027BFE0(unsigned int param) {
    if (param >= 0xBB8) {
        func_8027EEF4(0x33);
    }
    if (param >= 0x7530) {
        func_8027EEF4(0x34);
    }
    if (param >= 0x186A0) {
        func_8027EEF4(0x35);
    }
}
void func_8027C040(){}
void func_8027C0B0(){}
void func_8027C154(){}
void func_8027C1A8(){}
void func_8027C33C(){}
void func_8027C45C(){}
void func_8027C49C(){}
void func_8027C560(){}
void func_8027C5E4(){}
void func_8027C6B4(){}
void func_8027C924(){}
void func_8027CA0C(){}
void func_8027CAE0(){}
void func_8027CBE8(){}
void func_8027CC3C(){}
void func_8027CD08(){}
