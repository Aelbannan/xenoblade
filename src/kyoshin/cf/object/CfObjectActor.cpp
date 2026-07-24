#include "kyoshin/cf/object/CfObjectActor.hpp"

namespace cf {
    /* TODO: find out what base class the static cast is
    casting down to */
    CfObjectActor::CfObjectActor() :
    CActorParam(static_cast<CObjectParam*>(this), nullptr) {
        
    }

// LLM-HARNESS-BEGIN: us-80173828
float CfObjectActor::CfObjectActor_UnkVirtualFunc6() {
    // Float field at absolute offset 0x3EE8 within CfObjectActor
    // (0x4C within the CfObjectMove subobject at 0x3E9C)
    return *(float*)(reinterpret_cast<uintptr_t>(this) + 0x3EE8);
}
// LLM-HARNESS-END: us-80173828
}

// Forward declarations for thunks
extern "C" void __dt__Q22cf13CfObjectActorFv(void* self);
extern "C" void CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(void* self);

// LLM-HARNESS-BEGIN: us-80173514
extern "C" void CfObjectActor_UnkVirtualFunc5__Q22cf13CfObjectActorFv() {}
// LLM-HARNESS-END: us-80173514
// LLM-HARNESS-BEGIN: us-801738a4
extern "C" void CfObjectActor_UnkVirtualFunc8__Q22cf13CfObjectActorFv() {}
// LLM-HARNESS-END: us-801738a4
// LLM-HARNESS-BEGIN: us-801738c0
extern "C" void CfObjectActor_UnkVirtualFunc9__Q22cf13CfObjectActorFv() {}
// LLM-HARNESS-END: us-801738c0
// LLM-HARNESS-BEGIN: us-801739bc
extern "C" void CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv() {}
// LLM-HARNESS-END: us-801739bc

// LLM-HARNESS-BEGIN: us-80171310
extern "C" void func_8016FF14() {}
// LLM-HARNESS-END: us-80171310
// LLM-HARNESS-BEGIN: us-80171eac
extern "C" void func_80170AB0() {}
// LLM-HARNESS-END: us-80171eac
// LLM-HARNESS-BEGIN: us-80172b64
extern "C" void CfObjectActor_UnkVirtualFunc3__Q22cf13CfObjectActorFv() {}
// LLM-HARNESS-END: us-80172b64
// LLM-HARNESS-BEGIN: us-80172bbc
extern "C" void CfObjectActor_UnkVirtualFunc4__Q22cf13CfObjectActorFv() {}
// LLM-HARNESS-END: us-80172bbc
// LLM-HARNESS-BEGIN: us-80173830
extern "C" void CfObjectActor_UnkVirtualFunc7__Q22cf13CfObjectActorFv() {}
// LLM-HARNESS-END: us-80173830
// LLM-HARNESS-BEGIN: us-80173924
extern "C" void CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv() {}
// LLM-HARNESS-END: us-80173924
