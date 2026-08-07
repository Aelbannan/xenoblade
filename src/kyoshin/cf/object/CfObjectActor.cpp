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


struct IfE0 {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8(); virtual void _v0DC();
    virtual void vfE0();
};
struct IfDC {
    virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
    virtual void _v028(); virtual void _v02C(); virtual void _v030(); virtual void _v034();
    virtual void _v038(); virtual void _v03C(); virtual void _v040(); virtual void _v044();
    virtual void _v048(); virtual void _v04C(); virtual void _v050(); virtual void _v054();
    virtual void _v058(); virtual void _v05C(); virtual void _v060(); virtual void _v064();
    virtual void _v068(); virtual void _v06C(); virtual void _v070(); virtual void _v074();
    virtual void _v078(); virtual void _v07C(); virtual void _v080(); virtual void _v084();
    virtual void _v088(); virtual void _v08C(); virtual void _v090(); virtual void _v094();
    virtual void _v098(); virtual void _v09C(); virtual void _v0A0(); virtual void _v0A4();
    virtual void _v0A8(); virtual void _v0AC(); virtual void _v0B0(); virtual void _v0B4();
    virtual void _v0B8(); virtual void _v0BC(); virtual void _v0C0(); virtual void _v0C4();
    virtual void _v0C8(); virtual void _v0CC(); virtual void _v0D0(); virtual void _v0D4();
    virtual void _v0D8();
    virtual void vfDC();
};
extern "C" void CActorParam_UnkVirtualFunc23__Q22cf13CfObjectActorFv(void* self) {
    reinterpret_cast<IfE0*>((u8*)self + 0x3e9c)->vfE0();
}

// CActorParam_UnkVirtualFunc21: store float then adjust-tail-call vt+0xDC
extern "C" void CActorParam_UnkVirtualFunc21__Q22cf13CfObjectActorFv(void* self, float v) {
    *(float*)((u8*)self + 0x15e8) = v;
    reinterpret_cast<IfDC*>((u8*)self + 0x3e9c)->vfDC();
}


extern "C" void CActorParam_UnkVirtualFunc140__Q22cf13CfObjectActorFv() {}
extern "C" void CActorParam_UnkVirtualFunc179__Q22cf13CfObjectActorFv() {}
extern "C" void CActorParam_UnkVirtualFunc180__Q22cf13CfObjectActorFv() {}
extern "C" void CActorParam_UnkVirtualFunc33__Q22cf13CfObjectActorFv() {}
extern "C" void CActorParam_UnkVirtualFunc35__Q22cf13CfObjectActorFv() {}
extern "C" void CActorParam_UnkVirtualFunc34__Q22cf13CfObjectActorFv() {}
extern "C" void CActorParam_UnkVirtualFunc54__Q22cf13CfObjectActorFv() {}
extern "C" void CActorParam_UnkVirtualFunc60__Q22cf13CfObjectActorFv() {}
extern "C" void CActorParam_UnkVirtualFunc4__Q22cf13CfObjectActorFv() {}
