#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CAIAction.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"

// Retail float constants referenced by CfObjectActor functions (SDA2 access).
extern float lbl_eu_80667738;   // CActorParam_UnkVirtualFunc34 / CfObjectActor_UnkVirtualFunc5 threshold

extern float lbl_eu_8066776C;   // CfObjectActor_UnkVirtualFunc8 fallback value

// Retail symbol is Fv but the real ABI passes (self, arg, f1, f2, f3);
// CfObjectActor::CActorParam_UnkVirtualFunc140 tail-calls this base entry
// (defined in CActorParam.cpp).
extern "C" void CActorParam_UnkVirtualFunc140__Q22cf11CActorParamFv(cf::CActorParam* self, void* arg, f32 a, f32 b, f32 c);

namespace cf {
    //min size: 0x45BC
    class CfObjectActor : public CActorParam, public CAIAction, public CfObjectMove {
    public:
        CfObjectActor();
        //vtable 4 (CfObjectActor)
        virtual ~CfObjectActor();                  //0x5A0
        virtual void CfObjectActor_UnkVirtualFunc2();  //0x5A4
        virtual void CfObjectActor_UnkVirtualFunc3();  //0x5A8
        virtual void CfObjectActor_UnkVirtualFunc4();  //0x5AC
        virtual void CfObjectActor_UnkVirtualFunc5();  //0x5B0
        virtual float CfObjectActor_UnkVirtualFunc6();  //0x5B4
        virtual void CfObjectActor_UnkVirtualFunc7();  //0x5B8
        virtual float CfObjectActor_UnkVirtualFunc8();  //0x5BC (retail returns a float in f1)
        virtual void CfObjectActor_UnkVirtualFunc9();  //0x5C0
        virtual void CfObjectActor_UnkVirtualFunc10(); //0x5C4
        virtual void CfObjectActor_UnkVirtualFunc11(void* arg); //0x5C8
        virtual void CfObjectActor_UnkVirtualFunc12(); //0x5CC
        virtual void CfObjectActor_UnkVirtualFunc13(); //0x5D0

        //0x0: vtable 1
        //0x0-3380: CActorParam
        //0x3380: vtable 2
        //0x3380-3e9c: CAIAction
        //0x3e9c: vtable 3
        //0x3e9c-45b4: CfObjectMove
        u8 unk45B4[0x8];
    void CActorParam_UnkVirtualFunc1();
    void CActorParam_UnkVirtualFunc140();
    void CActorParam_UnkVirtualFunc179();
    void CActorParam_UnkVirtualFunc180();
    void CActorParam_UnkVirtualFunc33();
    void CActorParam_UnkVirtualFunc35();
    void CActorParam_UnkVirtualFunc34();
    void CActorParam_UnkVirtualFunc54();
    void CActorParam_UnkVirtualFunc60();
    void CActorParam_UnkVirtualFunc4();
    void CActorParam_UnkVirtualFunc21();
    void CActorParam_UnkVirtualFunc23();
    void func_801725DC();
    };

    // Vtable proxy for CfObjectActor vtable 1 slot +0x120
    // (CActorParam_UnkVirtualFunc35): retail passes (float, int, int, int)
    // through the slot even though the base header declares it no-arg. Dummy
    // slots pin the offset (MWCC puts the Nth declared virtual at (N+1)*4).
    class CfActorVt120 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C(); virtual void _d020(); virtual void _d024();
        virtual void _d028(); virtual void _d02C(); virtual void _d030(); virtual void _d034();
        virtual void _d038(); virtual void _d03C(); virtual void _d040(); virtual void _d044();
        virtual void _d048(); virtual void _d04C(); virtual void _d050(); virtual void _d054();
        virtual void _d058(); virtual void _d05C(); virtual void _d060(); virtual void _d064();
        virtual void _d068(); virtual void _d06C(); virtual void _d070(); virtual void _d074();
        virtual void _d078(); virtual void _d07C(); virtual void _d080(); virtual void _d084();
        virtual void _d088(); virtual void _d08C(); virtual void _d090(); virtual void _d094();
        virtual void _d098(); virtual void _d09C(); virtual void _d0A0(); virtual void _d0A4();
        virtual void _d0A8(); virtual void _d0AC(); virtual void _d0B0(); virtual void _d0B4();
        virtual void _d0B8(); virtual void _d0BC(); virtual void _d0C0(); virtual void _d0C4();
        virtual void _d0C8(); virtual void _d0CC(); virtual void _d0D0(); virtual void _d0D4();
        virtual void _d0D8(); virtual void _d0DC(); virtual void _d0E0(); virtual void _d0E4();
        virtual void _d0E8(); virtual void _d0EC(); virtual void _d0F0(); virtual void _d0F4();
        virtual void _d0F8(); virtual void _d0FC(); virtual void _d100(); virtual void _d104();
        virtual void _d108(); virtual void _d10C(); virtual void _d110(); virtual void _d114();
        virtual void _d118(); virtual void _d11C();
        virtual void m120(float value, int a, int b, int c);  // vtable +0x120
    };

    // Vtable proxy for the CBattleState subobject vtable (this+0x8), slot
    // +0x20 (CBattleState_UnkVirtualFunc7; retail passes an int - the base
    // header declares the slot no-arg). See CfObjectActor_UnkVirtualFunc3.
    class CfBattleVt20 {
    public:
        virtual void _d008(); virtual void _d00C(); virtual void _d010(); virtual void _d014();
        virtual void _d018(); virtual void _d01C();
        virtual void m20(int val);  // vtable +0x20
    };

    // Object holding a float at +0x7C (target of the CAIAction trailer word
    // read by CfObjectActor_UnkVirtualFunc8).
    struct CfFloat7C {
        u8 _pad[0x7C];
        float field_0x7C;  // 0x7C
    };

    // View of CfObjectActor's word at absolute offset 0x3E74 (CAIAction
    // trailer-area word, used as a pointer by CfObjectActor_UnkVirtualFunc8).
    struct CfActorField3E74 {
        u8 _pad[0x3E74];
        u32 field_0x3E74;  // 0x3E74
    };

    // View of CfObjectActor's word at absolute offset 0x3F10 (the
    // CfObjectModel.field_0x74 slot inside the CfObjectMove subobject) - used
    // by CActorParam_UnkVirtualFunc140 to compare two actors.
    struct CfActorField3F10 {
        u8 _pad[0x3F10];
        u32 field_0x3F10;  // 0x3F10
    };
}
