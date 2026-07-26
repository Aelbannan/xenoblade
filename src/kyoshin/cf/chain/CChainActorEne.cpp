// Auto-scaffolded catalog TU for kyoshin/cf/chain/CChainActorEne
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainActorEne.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"

// Operates on the CChainEffect at offset 0x74; compares r4 against effect.unk8's target
extern "C" void func_802A0AA0(cf::CChainEffect* effect);

extern "C" void func_80281308() {}

extern "C" void func_8028133C() {}

extern "C" void func_80281384() {}

extern "C" void func_80281438() {}

extern "C" void func_80281460() {}

extern "C" void func_8028146C() {}

extern "C" void func_802814E4() {}

extern "C" void func_802815B8() {}

extern "C" void func_802816FC() {}

extern "C" void func_8028183C() {}

// Tail-calls func_802A0AA0 with &self->mChainEffect, forwarding remaining arguments
extern "C" void func_802818D4(cf::CChainActorEne* self) {
    func_802A0AA0(&self->mChainEffect);
}

// Returns whether the enemy chain actor is valid/active
extern "C" s32 func_802818DC(cf::CChainActorEne* self) {
    return 1;
}

extern "C" void func_802818E4() {}

extern "C" void func_802818F8() {}
