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
extern "C" void func_8027C098(cf::CChainChance* self) {
    self->mChainCount = 0;
    self->mField08 = 0;
    self->mField0A = 0;
    self->mField0C = 0;
}
extern "C" void func_8027C5CC() {}
extern "C" void func_8027CA98() {}

extern "C" void func_8027B164() {}
extern "C" void func_8027B200() {}
extern "C" void func_8027B2CC() {}
extern "C" void func_8027B770() {}
extern "C" void func_8027B814() {}
extern "C" void func_8027B8C8() {}
extern "C" void func_8027BA0C() {}
extern "C" void func_8027BB4C() {}
extern "C" void func_8027BC14() {}
extern "C" void func_8027BE84() {}
extern "C" void func_8027BF58() {}
extern "C" void func_8027EEF4(int);

extern "C" void func_8027BFE0(unsigned int param) {
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
extern "C" void func_8027C040() {}
extern "C" void func_8027C0B0() {}
extern "C" void func_8027C154() {}
extern "C" void func_8027C1A8() {}
extern "C" void func_8027C33C() {}
extern "C" void func_8027C45C() {}
extern "C" void func_8027C49C() {}
extern "C" void func_8027C560() {}
extern "C" void func_8027C5E4() {}
extern "C" void func_8027C6B4() {}
extern "C" void func_8027C924() {}
extern "C" void func_8027CA0C() {}
extern "C" void func_8027CAE0() {}
extern "C" void func_8027CBE8() {}
extern "C" void func_8027CC3C() {}
extern "C" void func_8027CD08() {}
