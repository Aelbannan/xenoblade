#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CAIAction.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"

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
        virtual void CfObjectActor_UnkVirtualFunc8();  //0x5BC
        virtual void CfObjectActor_UnkVirtualFunc9();  //0x5C0
        virtual void CfObjectActor_UnkVirtualFunc10(); //0x5C4
        virtual void CfObjectActor_UnkVirtualFunc11(); //0x5C8
        virtual void CfObjectActor_UnkVirtualFunc12(); //0x5CC
        virtual void CfObjectActor_UnkVirtualFunc13(); //0x5D0

        //0x0: vtable 1
        //0x0-3380: CActorParam
        //0x3380: vtable 2
        //0x3380-3e9c: CAIAction
        //0x3e9c: vtable 3
        //0x3e9c-45b4: CfObjectMove
        u8 unk45B4[0x8];
    CfObjectActor();
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
}
