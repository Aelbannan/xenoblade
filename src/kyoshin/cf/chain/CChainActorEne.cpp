// Auto-scaffolded catalog TU for kyoshin/cf/chain/CChainActorEne
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/chain/CChainActorEne.hpp"
#include "kyoshin/cf/chain/CChainEffect.hpp"

struct CEIfShift { char pad[0x70]; };
struct CEIf : CEIfShift {
    virtual void _v0008();
    virtual void _v000C();
    virtual void _v0010();
    virtual void _v0014();
    virtual void _v0018();
    virtual void _v001C();
    virtual void _v0020();
    virtual void _v0024();
    virtual void _v0028();
    virtual void _v002C();
    virtual void _v0030();
    virtual void _v0034();
    virtual void _v0038();
    virtual void _v003C();
    virtual void _v0040();
    virtual void _v0044();
    virtual void vf0048(void* a);
};

// Operates on the CChainEffect at offset 0x74; compares r4 against effect.unk8's target
void func_802A0AA0(cf::CChainEffect* effect);

void func_80281308(){}

void func_8028133C(){}

void func_80281384(){}

void func_80281438(){}

void func_80281460(){}

void func_8028146C(){}

void func_802814E4(){}

void func_802815B8(){}

void func_802816FC(){}

void func_8028183C(){}

// Tail-calls func_802A0AA0 with &this->mChainEffect, forwarding remaining arguments
void cf::CChainActorEne::func_802818D4() {
    extern void func_802A0AA0(cf::CChainEffect* effect);
    func_802A0AA0(&this->mChainEffect);
}

// Returns whether the enemy chain actor is valid/active
s32 cf::CChainActorEne::func_802818DC() {
    return 1;
}

extern "C" void func_802818E4(void* self, void* arg) { reinterpret_cast<CEIf*>(self)->vf0048(*(void**)arg); }

void func_802818F8(){}
