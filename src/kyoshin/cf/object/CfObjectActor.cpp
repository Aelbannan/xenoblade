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
void __dt__Q22cf13CfObjectActorFv(void* self);
void CObjectParam_UnkVirtualFunc2__Q22cf12CfObjectMoveFv(void* self);

void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc5() {}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc8() {}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc9() {}
struct CfObjectActorData {
    u8 _pad00[0x3f60];
    void* mSomePtr;     // 0x3f60
    u8 _pad3f64[0x658];
    void* mOtherPtr;    // 0x45bc
};

struct CfObjectActorInner {
    u8 _pad00[0x37c];
    void* mArgStorage;  // 0x37c
};

void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc11(void* arg) {
    CfObjectActorData* data = reinterpret_cast<CfObjectActorData*>(this);
    void* p = data->mSomePtr;
    if (p != 0) {
        static_cast<CfObjectActorInner*>(p)->mArgStorage = arg;
    }
    data->mOtherPtr = arg;
}

void func_8016FF14(){}
void func_80170AB0(){}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc3() {}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc4() {}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc7() {}
void cf::CfObjectActor::CfObjectActor_UnkVirtualFunc10() {}


void cf::CfObjectActor::CActorParam_UnkVirtualFunc23() {
    void* adjusted = (char*)this + 0x3e9c;
    void** vtable = *(void***)adjusted;
    void (*func)(void*) = (void (*)(void*))vtable[0xe0 / 4];
    func(adjusted);
}
