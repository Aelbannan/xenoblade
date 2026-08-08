#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"

namespace cf {
    class CChainActorPc : public CChainActor {

    };
}


// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// Forward: free function operating on the CChain/CChainActor memory layout
extern "C" void CChain_setFieldAndClear(void*, int);

extern "C" void func_802A08F4(void*);
extern "C" void func_802A0904(void*);
extern "C" void func_80279DC0(void*);
extern "C" int func_80148778(void*, int);
extern "C" int func_8027A024(void*, void*);
extern "C" int* func_8009ECB0();

// Retail symbol: func_80279F6C
extern "C" void func_80279F6C(void*, int);
