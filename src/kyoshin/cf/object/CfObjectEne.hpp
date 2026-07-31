#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectActor.hpp"

namespace cf {
    //size: 0x45CC
    class CfObjectEne : public CfObjectActor {
    public:
        virtual ~CfObjectEne();
        virtual void func_800ADB2C();
        virtual void func_800ADBD4();
        virtual void func_800ADDA8();
        virtual void func_800AEC68();

        //0x0: vtable
        //0x0-45BC: CfObjectActor
        u8 unk45BC[0x10];
    void CActorParam_UnkVirtualFunc166();
    void CActorParam_UnkVirtualFunc167();
    void CfObject_UnkVirtualFunc2();
    void CObjectParam_UnkVirtualFunc4();
    void CfObject_UnkVirtualFunc4();
    void func_800B069C();
    };
}

namespace cf {

class CfObjectActor {
public:
    virtual ~CfObjectActor();
    void CfObjectMove_UnkVirtualFunc15();

    // TODO: add fields
    void CfObjectActor_UnkVirtualFunc2();
    void CActorParam_UnkVirtualFunc3();
    void CActorParam_UnkVirtualFunc2();
    void CfObjectActor_UnkVirtualFunc13();
    void CfObjectActor_UnkVirtualFunc12();
    void CBattleState_UnkVirtualFunc18();
    void CBattleState_UnkVirtualFunc17();
    void CBattleState_UnkVirtualFunc3();
    void CBattleState_UnkVirtualFunc1();
    void CBattleState_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc31();
    void* CObjectParam_UnkVirtualFunc2();
    void CfObjectMove_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc14();
};
} // namespace cf
namespace cf {

class CActorParam {
public:
    void CActorParam_UnkVirtualFunc98();
    void CActorParam_UnkVirtualFunc148();
    void CActorParam_UnkVirtualFunc146();
    void CActorParam_UnkVirtualFunc134();
    void CActorParam_UnkVirtualFunc130();
    void CActorParam_UnkVirtualFunc123();
    void CActorParam_UnkVirtualFunc120();
    void CActorParam_UnkVirtualFunc103();
    void CActorParam_UnkVirtualFunc92();
    void CActorParam_UnkVirtualFunc88();
    void CActorParam_UnkVirtualFunc84();
    void CActorParam_UnkVirtualFunc51();
    void CActorParam_UnkVirtualFunc45();
    void CActorParam_UnkVirtualFunc42();
    void CActorParam_UnkVirtualFunc39();
    void CActorParam_UnkVirtualFunc36();
    void CActorParam_UnkVirtualFunc34();
    void CActorParam_UnkVirtualFunc31();
    void CActorParam_UnkVirtualFunc28();
    void CActorParam_UnkVirtualFunc25();

    // TODO: add fields
    void CActorParam_UnkVirtualFunc20();
    void CActorParam_UnkVirtualFunc18();
    void CActorParam_UnkVirtualFunc112();
    void CActorParam_UnkVirtualFunc114();
    void CActorParam_UnkVirtualFunc118();
    void CActorParam_UnkVirtualFunc168();
    void CActorParam_UnkVirtualFunc181();
    void CActorParam_UnkVirtualFunc178();
    void CActorParam_UnkVirtualFunc173();
    void CActorParam_UnkVirtualFunc172();
    void CActorParam_UnkVirtualFunc171();
    void CActorParam_UnkVirtualFunc170();
    void CActorParam_UnkVirtualFunc169();
    void CActorParam_UnkVirtualFunc164();
    void CActorParam_UnkVirtualFunc163();
    void CActorParam_UnkVirtualFunc162();
    void CActorParam_UnkVirtualFunc161();
    void CActorParam_UnkVirtualFunc151();
    void CActorParam_UnkVirtualFunc145();
    void CActorParam_UnkVirtualFunc147();
    void CActorParam_UnkVirtualFunc139();
    void CActorParam_UnkVirtualFunc133();
    void CActorParam_UnkVirtualFunc131();
    void CActorParam_UnkVirtualFunc128();
    void CActorParam_UnkVirtualFunc124();
    void CActorParam_UnkVirtualFunc121();
    void CActorParam_UnkVirtualFunc111();
    void CActorParam_UnkVirtualFunc110();
    void CActorParam_UnkVirtualFunc109();
    void CActorParam_UnkVirtualFunc108();
    void CActorParam_UnkVirtualFunc107();
    void CActorParam_UnkVirtualFunc105();
    void CActorParam_UnkVirtualFunc104();
    void CActorParam_UnkVirtualFunc102();
    void CActorParam_UnkVirtualFunc101();
    void CActorParam_UnkVirtualFunc99();
    void CActorParam_UnkVirtualFunc97();
    void CActorParam_UnkVirtualFunc96();
    void CActorParam_UnkVirtualFunc95();
    void CActorParam_UnkVirtualFunc93();
    void CActorParam_UnkVirtualFunc80();
    void CActorParam_UnkVirtualFunc79();
    void CActorParam_UnkVirtualFunc78();
    void CActorParam_UnkVirtualFunc77();
    void CActorParam_UnkVirtualFunc75();
    void CActorParam_UnkVirtualFunc73();
    void CActorParam_UnkVirtualFunc72();
    void CActorParam_UnkVirtualFunc71();
    void CActorParam_UnkVirtualFunc69();
    void CActorParam_UnkVirtualFunc66();
    void CActorParam_UnkVirtualFunc63();
    void CActorParam_UnkVirtualFunc62();
    void CActorParam_UnkVirtualFunc59();
    void CActorParam_UnkVirtualFunc57();
    void CActorParam_UnkVirtualFunc56();
    void CActorParam_UnkVirtualFunc53();
    void CActorParam_UnkVirtualFunc50();
    void CActorParam_UnkVirtualFunc49();
    void CActorParam_UnkVirtualFunc47();
    void CActorParam_UnkVirtualFunc44();
    void CActorParam_UnkVirtualFunc43();
    void CActorParam_UnkVirtualFunc41();
    void CActorParam_UnkVirtualFunc32();
    void CActorParam_UnkVirtualFunc30();
    void CActorParam_UnkVirtualFunc27();
    void CActorParam_UnkVirtualFunc24();
    void CActorParam_UnkVirtualFunc17();
    void CActorParam_UnkVirtualFunc15();
};
} // namespace cf
namespace cf {

class CBattleState {
public:
    void CBattleState_UnkVirtualFunc24();
    void CBattleState_UnkVirtualFunc23();
    void CBattleState_UnkVirtualFunc22();
    void CBattleState_UnkVirtualFunc21();
    void CBattleState_UnkVirtualFunc20();

    // TODO: add fields
    void CBattleState_UnkVirtualFunc28();
    void CBattleState_UnkVirtualFunc27();
    void CBattleState_UnkVirtualFunc25();
};
} // namespace cf

