#include "kyoshin/cf/object/CfObjectActor.hpp"

namespace cf {
    /* TODO: find out what base class the static cast is
    casting down to */
    CfObjectActor::CfObjectActor() :
    CActorParam(static_cast<CObjectParam*>(this), nullptr) {
        
    }

float CfObjectActor::CfObjectActor_UnkVirtualFunc6() {
    // Float field at absolute offset 0x3EE8 within CfObjectActor
    // (0x4C within the CfObjectMove subobject at 0x3E9C)
    return *(float*)(reinterpret_cast<uintptr_t>(this) + 0x3EE8);
}
}

// Forward declarations for thunks
extern "C" void __dt__Q22cf13CfObjectActorFv(void* self);
extern "C" void CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(void* self);

extern "C" void CfObjectActor_UnkVirtualFunc5__Q22cf13CfObjectActorFv() {}
extern "C" void CfObjectActor_UnkVirtualFunc8__Q22cf13CfObjectActorFv() {}
extern "C" void CfObjectActor_UnkVirtualFunc9__Q22cf13CfObjectActorFv() {}
extern "C" void CfObjectActor_UnkVirtualFunc11__Q22cf13CfObjectActorFv(cf::CfObjectActor* self, void* arg) {
    void* p = *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 0x3f60);
    if (p != 0) {
        *reinterpret_cast<void**>(reinterpret_cast<char*>(p) + 0x37c) = arg;
    }
    *reinterpret_cast<void**>(reinterpret_cast<char*>(self) + 0x45bc) = arg;
}

extern "C" void func_8016FF14() {}
extern "C" void func_80170AB0() {}
extern "C" void CfObjectActor_UnkVirtualFunc3__Q22cf13CfObjectActorFv() {}
extern "C" void CfObjectActor_UnkVirtualFunc4__Q22cf13CfObjectActorFv() {}
extern "C" void CfObjectActor_UnkVirtualFunc7__Q22cf13CfObjectActorFv() {}
extern "C" void CfObjectActor_UnkVirtualFunc10__Q22cf13CfObjectActorFv() {}


extern "C" void CActorParam_UnkVirtualFunc23__Q22cf13CfObjectActorFv(void* self) {
    self = (char*)self + 0x3e9c;
    void** vtable = *(void***)self;
    void (*func)(void*) = (void (*)(void*))vtable[0xe0 / 4];
    func(self);
}
