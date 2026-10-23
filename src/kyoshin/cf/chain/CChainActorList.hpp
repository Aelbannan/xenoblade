#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"
#include "monolib/util.hpp"


namespace cf {
    // Standalone mirror of the chain-actor layout (0x80 bytes, manual vtable at
    // +0x70, non-standard ABI). Deliberately NOT derived from CChainActor:
    // retail Pc/Ene dtors are plain empty+delete with no base/member dtor
    // calls, which requires these classes to have no non-trivial base or
    // dtor-ful members.
    class CChainActorPc {
    public:
        u32 unk0;          //0x0 battle-object key/pointer
        u8 field_4[0x60];  //0x4 CChainTemp-ish buffer
        u8 field_64;       //0x64
        u8 _pad65[3];
        void* field_68;    //0x68 sub-object vtable
        u16 unk6C;         //0x6C
        u8 _pad6E[2];
        void* mVTable;     //0x70 manual vtable
        u8 field_74[0xC];  //0x74 CChainEffect region (no dtor)

        CChainActorPc();
        ~CChainActorPc();
    };

    class CChainActorEne {
    public:
        u32 unk0;          //0x0 battle-object key/pointer
        u8 field_4[0x60];  //0x4 CChainTemp-ish buffer
        u8 field_64;       //0x64
        u8 _pad65[3];
        void* field_68;    //0x68 sub-object vtable
        u16 unk6C;         //0x6C
        u8 _pad6E[2];
        void* mVTable;     //0x70 manual vtable
        u8 field_74[0xC];  //0x74 CChainEffect region (no dtor)

        CChainActorEne();
        ~CChainActorEne();
    };

    //size: 0x1d80
    class UnkClass_8027AD70 {
    public:
        CChainActorPc mPlayerActors[3]; //0x0
        CChainActorEne mEnemyActors[56]; //0x180

        UnkClass_8027AD70(){}
        ~UnkClass_8027AD70(){}
    };

    //size: 0x1DB0
    class CChainActorList {
    public:
        UnkClass_8027AD70 unk0; //0x0
        u8 unk1D80[8];
        reslist<CChainActor*> mChainActorList; //0x1D88
        u8 unk1DA8[4];
        //0x1DAC: vtable

        CChainActorList();
        virtual ~CChainActorList();
    };

    // size 0xE8: fixed array of CChainActor* with an element count
    // (field at 0xE0) and a byte flag (field at 0xE4).
    class CChainList {
    public:
        CChainActor* mActors[0x38]; //0x0
        u32 mCount;                 //0xE0
        u8 mFlag;                   //0xE4
    };

    // Mirror of CChainChance with signed 16-bit fields: Chain_TickChanceCount reads the
    // 0x0/0x8/0xA fields sign-extended (lha), while CChainChance.hpp declares
    // them u16. Layout-twin (same offsets/sizes) so the free function's param
    // can force signed loads without touching CChainChance.hpp.
    class CChainChanceS {
    public:
        s16 mChainCount; //0x0
        u8 unk2[2];      //0x2
        u32 unk4;        //0x4
        s16 mField08;    //0x8
        s16 mField0A;    //0xA
        u8 mField0C;     //0xC
        u8 _pad0D[3];    //0xD
        //0x10: vtable
    };

    // Minimal object manipulated by Chain_RollActivation (u16 field at 0x0).
    class CChainMsg {
    public:
        u16 field_0x0; //0x0
    };

    // Minimal object manipulated by ChainCounter_Drain (s32 field at 0x0).
    class CChainCounter {
    public:
        s32 field_0x0; //0x0
    };

    // Large object manipulated by Chain_EmitCountEvents (u32 flag at 0x3F00).
    class CChainFlag {
    public:
        u8 field_0[0x3F00]; //0x0
        u32 field_0x3F00;   //0x3F00
    };

    // 3x8 float table mirroring arts-param tuning values plus an enable flag
    // (manipulated by ChainMusic_SaveSlots / ChainMusic_RestoreSlots).
    class CChainMusic {
    public:
        float mSlots[3][8]; //0x0
        u8 mEnabled;        //0x60
    };

    // State object manipulated by Chain_ReadPadAction (s16 at 0x0, u8 at 0xC).
    class CChainAction {
    public:
        s16 field_0;  //0x0
        u8 unk2[10];  //0x2
        u8 field_0xc; //0xc
    };
}
