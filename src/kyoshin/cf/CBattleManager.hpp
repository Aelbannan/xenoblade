#pragma once

#include <types.h>
#include <cstring>
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "kyoshin/cf/IBattleEvent.hpp"
#include "kyoshin/cf/chain/CChain.hpp"
#include "kyoshin/cf/chain/UnkClass_800D8DBC.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"
#include "monolib/util/reslist.hpp"

// Lean public header: the class plus the types it holds by value. The
// CBattleManager.cpp-only views and C-linkage imports live in
// kyoshin/cf/CBattleManagerImports.hpp.

namespace cf {
    class CfObjectActor;
}

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

    // 8 bytes: 4-byte key + 1-byte count + 3 padding
    struct CBattleManager_Struct1 {
        s32 key;    // +0x00
        u8 count;   // +0x04
        u8 pad[3];  // +0x05..0x07
    };

    struct CBattleManager_Struct2 {
        CBattleManager_Struct2(){
            // POD walk: declare end first (Rule A → r31) but assign start
            // first so the addi order matches the implicit array ctor.
            CBattleManager_Struct1* end;
            CBattleManager_Struct1* start = unk0;
            end = unk0 + 32;
            do {
                std::memset(start, 0, sizeof(*start));
                start++;
            } while (start < end);
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
        virtual void setPartyMaskFlag(u32 r4, u32 r5); //0x1C
        virtual void CBattleMan_ClearStateMask(u32 mask); //0x20
        virtual void CBattleMan_ZeroStateMask(); //0x24
        virtual s32 CheckUnk84Flag(u32 mask); //0x28
        virtual void CBattleMan_TickVision(); //0x2C
        virtual void* CBattleMan_QueryVision(); //0x30
        virtual void CBattleMan_SetTimers(float a, float b, unsigned long c); //0x34
        virtual void CBattleMan_ResetTimer(); //0x38
        virtual void CBattleMan_EmitVisionEvent(void* arg); //0x3C

        void* CBattleMan_FetchVisionObj();

        static CBattleManager* getInstance();
        // TUs that cannot include this class reach the same retail symbol
        // through kyoshin/cf/CBattleManagerApi.hpp (the one shared copy).
        static void getBattleStateA();
        static void getBattleStateB();
        void resetBattleState();

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

        // sound-id table (10 known entries; retail .rodata symbol)
        // static u32 lbl_804F8228[]; -> moved to global lbl_eu_804FCA08

    protected:
        // static CBattleManager* spInstance; -> extern "C" lbl_eu_80663F00 in .cpp
    };
}

