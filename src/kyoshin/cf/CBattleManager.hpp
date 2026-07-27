#pragma once

#include <types.h>
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/IBattleEvent.hpp"
#include "kyoshin/cf/chain/CChain.hpp"
#include "kyoshin/cf/chain/UnkClass_800D8DBC.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"
#include "monolib/util.hpp"
#include <cstring>

namespace cf{
    class UnkClass_8018C5FC {
    public:
        UnkClass_8018C5FC();
        
        u8 unk0[8];
    };

    class UnkClass_80192BF4 {
    public:
        UnkClass_80192BF4();
        void __ct__80192C10();

        u8 unk0[0xC];
    };

    struct CBattleManager_Struct1 {
        CBattleManager_Struct1(){
            std::memset(unk0, 0, sizeof(unk0));
        }

        u8 unk0[8];
    };

    struct CBattleManager_Struct2 {
        CBattleManager_Struct2(){
            clear();
        }

        void clear(){
            std::memset(unk0, 0, sizeof(unk0));
        }

        CBattleManager_Struct1 unk0[32];
    };

    //size: 0x283D8
    class CBattleManager : public IFactoryEvent {
    public:
        CBattleManager();
        virtual ~CBattleManager(); //0x8
        virtual void FactoryEvent2(); //0x10
        virtual void func_80085220(u32 r4, u32 r5); //0x1C
        virtual void func_800E2584(); //0x20
        virtual void func_800F42A0(); //0x24
        virtual void func_800885F0(); //0x28
        virtual void func_800EA410(); //0x2C
        virtual void func_800EA420(); //0x30
        virtual void func_800EA460(); //0x34
        virtual void func_800EA470(); //0x38
        virtual void func_800EA998(); //0x3C

        static CBattleManager* getInstance();
        static void func_800D9190();
        static void func_800D91D0();
        void func_800D9218();

        //0x0: vtable
        //0x0-4: IFactoryEvent
        reslist<CfObjectActor*> mActorList1; //0x4
        reslist<CfObjectActor*> mActorList2; //0x24
        reslist<CfObjectActor*> mActorList3; //0x44
        reslist<IBattleEvent*> mBattleEventList; //0x64
        u32 unk84; //0x84
        float unk88;
        u32 unk8C;
        float unk90;
        CBattleManager_Struct2 unk94;
        UnkClass_8018C5FC unk194; //0x194
        UnkClass_80192BF4 unk19C; //0x19C
        CChain mChain; //0x1A8
        UnkClass_800D8DBC unk20C8; //0x20C8
        CSuddenCommu mSuddenCommu; //0x216C
        CVision mVision; //0x219C

        static u32 lbl_804F8228[];

    protected:
        static CBattleManager* spInstance;
    };
}

// --- Standalone function access structs (CBattleManager.cpp) ---

// Intrusive linked-list node: +0x00 = next, +0x08 = data ptr
struct SimpleListNode {
    SimpleListNode* next;
    void* data;
};

// Return layout of func_8009EC9C (accessed at +0x1C, cast_int_arith L63/L80)
struct UnkStruct_8009EC9C_Ret {
    u8 pad_00[0x1C];
    u8 unk1C;
};

// Layout for func_800EA384 self: list sentinel ptr at +0x08
struct Func800EA384_Self {
    u8 pad_00[0x08];
    SimpleListNode* listHead;
};

// Layout for func_800F4004 this_: list sentinel ptr at +0x48
struct Func800F4004_Self {
    u8 pad_00[0x48];
    SimpleListNode* listHead;
};
