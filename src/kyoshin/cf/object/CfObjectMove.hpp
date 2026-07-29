#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"

namespace cf {
    //min size: 0x715
    class CfObjectMove : public CfObjectModel {
    public:
        CfObjectMove();
        //vtable 1 (CfObject)
        virtual ~CfObjectMove();
        //vtable 1 (CfObjectMove)
        virtual void CfObjectMove_UnkVirtualFunc1();  //0x1C8
        virtual void CfObjectMove_UnkVirtualFunc2();  //0x1CC
        virtual void CfObjectMove_UnkVirtualFunc3();  //0x1D0
        virtual void CfObjectMove_UnkVirtualFunc4();  //0x1D4
        virtual void CfObjectMove_UnkVirtualFunc5();  //0x1D8
        virtual void CfObjectMove_UnkVirtualFunc6();  //0x1DC
        virtual void CfObjectMove_UnkVirtualFunc7();  //0x1E0
        virtual void CfObjectMove_UnkVirtualFunc8();  //0x1E4
        virtual void CfObjectMove_UnkVirtualFunc9();  //0x1E8
        virtual void CfObjectMove_UnkVirtualFunc10(); //0x1EC
        virtual void CfObjectMove_UnkVirtualFunc11(); //0x1F0
        virtual void CfObjectMove_UnkVirtualFunc12(); //0x1F4
        virtual void CfObjectMove_UnkVirtualFunc13(); //0x1F8
        virtual void CfObjectMove_UnkVirtualFunc14(); //0x1FC
        virtual void CfObjectMove_UnkVirtualFunc15(); //0x200
        virtual void CfObjectMove_UnkVirtualFunc16(); //0x204
        virtual void CfObjectMove_UnkVirtualFunc17(); //0x208
        virtual void CfObjectMove_UnkVirtualFunc18(); //0x20C
        virtual void CfObjectMove_UnkVirtualFunc19(); //0x210
        virtual void CfObjectMove_UnkVirtualFunc20(); //0x214
        virtual void CfObjectMove_UnkVirtualFunc21(); //0x218
        virtual void CfObjectMove_UnkVirtualFunc22(); //0x21C
        virtual void CfObjectMove_UnkVirtualFunc23(); //0x220

        //0x0: vtable
        //0x0-BE: CfObjectModel
        // Field layout starting at offset 0xBE:
        u8 _BE[6];              // 0xBE-0xC3
        void* mTargetC4;         // 0xC4-0xC7
        u8 _C8[0x544];           // 0xC8-0x60B
        u8 _60C_region[0xB4];   // 0x60C-0x6BF
        void* mTarget6C0;         // 0x6C0-0x6C3
        u8 _6C4[5];              // 0x6C4-0x6C8
        u8 mFlags6C9;             // 0x6C9
        u8 _6CA[0x26];           // 0x6CA-0x6EF
        float mMoveSpeed;         // 0x6F0-0x6F3
        u8 _6F4[0x21];           // 0x6F4-0x714
        u8 unk715[3];            // 0x715-0x717
    void CfObject_UnkVirtualFunc4();
    void CfObject_UnkVirtualFunc7();
    void CfObject_UnkVirtualFunc6();
    void CfObjectModel_UnkVirtualFunc1();
    void CfObjectModel_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc5();
    void CfObject_UnkVirtualFunc46();
    void CfObject_UnkVirtualFunc47();
    void CfObject_UnkVirtualFunc49();
    void CfObject_UnkVirtualFunc64();
    void CfObject_UnkVirtualFunc65();
    void CfObject_UnkVirtualFunc19();
    void CfObject_UnkVirtualFunc22();
    void CfObject_UnkVirtualFunc25();
    void CfObject_UnkVirtualFunc26();
    void CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27();
    void CfObject_UnkVirtualFunc30();
    void CfObject_UnkVirtualFunc32();
    void CfObject_UnkVirtualFunc33();
    void CfObject_UnkVirtualFunc13();
    void CfObject_UnkVirtualFunc57();
    void CObjectParam_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc14();
    void CfObject_UnkVirtualFunc15();
    void CfObject_UnkVirtualFunc16();
    void CfObject_UnkVirtualFunc17();
    void CfObjectModel_UnkVirtualFunc18();
    bool CfObject_UnkVirtualFunc9();
    void CfObject_UnkVirtualFunc10();
    void CfObject_UnkVirtualFunc61();
    void CfObject_UnkVirtualFunc62();
    void CfObject_UnkVirtualFunc12();
    void CfObject_UnkVirtualFunc66();
    void CfObjectModel_UnkVirtualFunc19();
    void CfObjectModel_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc37();
    void CfObject_UnkVirtualFunc38();
    void CfObject_UnkVirtualFunc39();
    void CfObject_UnkVirtualFunc40();
    void CfObject_UnkVirtualFunc42();
    void CfObject_UnkVirtualFunc43();
    void CfObject_UnkVirtualFunc45();
    void CfObject_UnkVirtualFunc70();
    void CfObject_UnkVirtualFunc50();
    void CfObject_UnkVirtualFunc51();
    void CfObject_UnkVirtualFunc60();
    void CfObject_UnkVirtualFunc29(float value);
    void setMoveSpeed(float value);
    void resetMoveSpeed();
    void updatePos();
    void* getUnk54();
    int getSubState();
    void freeSub();
    void setSubFieldC(unsigned short val);
    int getSubFieldA();
    void setSubFieldA(unsigned short val);
    int getSubFieldE();
    void setSubFieldE(unsigned short val);
    void virtCall10();
    int nullsub_25();
    int nullsub_26();
    int nullsub_27();
    int nullsub_28();
    int isActive();
    void setBit6c9(unsigned long bit);
    cf::CfObjectMove* testFlag8();
    };
}

