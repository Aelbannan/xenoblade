// Auto-scaffolded catalog TU for kyoshin/cf/chain/CChainActorEne
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainActorEne.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"

// Operates on the CChainEffect at offset 0x74; compares r4 against effect.unk8's target
extern "C" void func_802A0AA0(cf::CChainEffect* effect);

// LLM-HARNESS-BEGIN: us-8028378c
extern "C" void func_80281308() {}
// LLM-HARNESS-END: us-8028378c

// LLM-HARNESS-BEGIN: us-802837c0
extern "C" void func_8028133C() {}
// LLM-HARNESS-END: us-802837c0

// LLM-HARNESS-BEGIN: us-80283808
extern "C" void func_80281384() {}
// LLM-HARNESS-END: us-80283808

// LLM-HARNESS-BEGIN: us-802838bc
extern "C" void func_80281438() {}
// LLM-HARNESS-END: us-802838bc

// LLM-HARNESS-BEGIN: us-802838e4
extern "C" void func_80281460() {}
// LLM-HARNESS-END: us-802838e4

// LLM-HARNESS-BEGIN: us-802838f0
extern "C" void func_8028146C() {}
// LLM-HARNESS-END: us-802838f0

// LLM-HARNESS-BEGIN: us-80283968
extern "C" void func_802814E4() {}
// LLM-HARNESS-END: us-80283968

// LLM-HARNESS-BEGIN: us-80283a3c
extern "C" void func_802815B8() {}
// LLM-HARNESS-END: us-80283a3c

// LLM-HARNESS-BEGIN: us-80283b80
extern "C" void func_802816FC() {}
// LLM-HARNESS-END: us-80283b80

// LLM-HARNESS-BEGIN: us-80283cc0
extern "C" void func_8028183C() {}
// LLM-HARNESS-END: us-80283cc0

// LLM-HARNESS-BEGIN: us-80283d58
// Tail-calls func_802A0AA0 with &self->mChainEffect, forwarding remaining arguments
extern "C" void func_802818D4(cf::CChainActorEne* self) {
    func_802A0AA0(&self->mChainEffect);
}
// LLM-HARNESS-END: us-80283d58

// LLM-HARNESS-BEGIN: us-80283d60
// Returns whether the enemy chain actor is valid/active
extern "C" s32 func_802818DC(cf::CChainActorEne* self) {
    return 1;
}
// LLM-HARNESS-END: us-80283d60

// LLM-HARNESS-BEGIN: us-80283d68
extern "C" void func_802818E4() {}
// LLM-HARNESS-END: us-80283d68

// LLM-HARNESS-BEGIN: us-80283d7c
extern "C" void func_802818F8() {}
// LLM-HARNESS-END: us-80283d7c
