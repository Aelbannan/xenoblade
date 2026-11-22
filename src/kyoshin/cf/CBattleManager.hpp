#pragma once

#include <types.h>
#include <cstring>
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "kyoshin/cf/IBattleEvent.hpp"
#include "kyoshin/cf/chain/CChain.hpp"
#include "kyoshin/cf/chain/UnkClass_800D8DBC.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"
#include "kyoshin/menu/code_8018C5FC.hpp"
#include "monolib/util/reslist.hpp"

// Lean public header: the class plus the types it holds by value. The
// CBattleManager.cpp-only views and C-linkage imports live in
// kyoshin/cf/CBattleManagerImports.hpp.

namespace cf {
    class CfObjectActor;
}

namespace cf{
    class UnkClass_80192BF4 {
    public:
        UnkClass_80192BF4();
        void __ct__80192C10();

        /* 0x00 */ u32 field_0x00; // counter/state
        /* 0x04 */ f32 field_0x04; // timer/accumulator
        /* 0x08 */ f32 field_0x08; // value
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
        virtual void FactoryEvent2(CfObject* obj); //0x10
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
        static CBattleManager* spInstance;
        // TUs that cannot include this class reach the same retail symbol

        static void getBattleStateA();
        static void getBattleStateB();
        void resetBattleState();

        //0x0: vtable

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
        // static u32 lbl_804F8228[]; -> moved to global sAttackSoundIdTable

    protected:
        // (spInstance is the real static member declared above)
    };
}

// Single owner API for cf::CBattleManager. These declarations used to live in
// kyoshin/cf/CBattleManager.hpp; callers now include this header.
//
// The singleton has two spellings for the same retail symbol: the real static
// member above and this C-linkage import for TUs that keep the pointer opaque.
extern "C" void* getInstance__Q22cf14CBattleManagerFv();

// The old CBattleManagerSlot28 view is gone: vtable slot 0x28 is the real
// cf::CBattleManager::CheckUnk84Flag(u32) declared above.

// Per-frame battle-manager update (retail-unmangled updateBattleManager).
extern "C" void updateBattleManager(cf::CBattleManager* self);

// Battle event dispatch (retail-unmangled func_800F3970).
extern "C" void func_800F3970(void* self, void* obj1, void* obj2, s32 idx, s32 addVal);

struct BattleObjAccessor;
struct BattleMoveObjAccessor;
struct Func800EA9A8_Arg5;

bool CBattleMan_ListHasValue(void* mgr, unsigned int value);
void* CBattleMan_NextListItem(void* self);
void* CBattleMan_FindNextActorRef(void* self, void* value);
void CBattleMan_HandleMoveType45(void* self, BattleObjAccessor* obj, void* enemy,
                                 BattleMoveObjAccessor* move);
void CBattleMan_FireActorEvent918(void* self, BattleObjAccessor* actor,
                                  Func800EA9A8_Arg5* arg, void* id);
void CBattleMan_RunBattleEvent(u32 a, u32 b, void* c, u32 d);
void CBattleMan_ClearEventNotify(void* self, u32 id, u16 subId);
void CBattleMan_ClearActorStatus(void* ignored, void* actor, void* arg);
int CBattleMan_GetUnk94Count(void* self, int key);
void CBattleMan_SetPartyFlagReset(cf::CBattleManager* mgr, u32 value);
void CBattleMan_BumpUnk94Key(cf::CBattleManager* mgr, s32 key);
s32 CBattleMan_FindActorByKind(cf::CBattleManager* mgr, s32 kind);
void CBattleMan_RaiseBattleFlag10(cf::CBattleManager* mgr);
void CBattleMan_DropBattleFlag10(cf::CBattleManager* mgr);
void CBattleMan_MarkList48Flag40(void* self);
void CBattleMan_RefreshChainEnum(cf::CBattleManager* mgr);

