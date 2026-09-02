#pragma once

#include <types.h>
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"

namespace cf{
    class CfObjectMove;
    class CfObjectImplPc18;
    class CfObjectImplPcSub3E9C;
    class CfObjectImplPcED4;
    class CfObjectImplPcBattle4;
    class CVS_THREAD;

    struct CSoundNode {
        u8 unk0[0xC];
        CSoundNode* next;
        u8 unk10[0x8];
        u32 field_18;
        void* field_1C;
        u8 unk20[0x10];
    };

    struct CVoiceFactoryActor {
        u8 unk0[0x64];
        u32 field_64;
    };

    struct CVoiceBtlSlot {
        u32 id0;
        u32 id1;
    };

    struct CVoiceActorInfo {
        u8 unk0[0x3F10];
        u32 field_3F10;
    };

    struct CVoiceActorBase {
        u8 unk0[0x3E9C];
        u8 moveBase;
    };

    struct CVoiceF3F60Sub {
        u8 unk0[0x18];
        int field_18;
    };
    struct CVoiceF3F60Obj {
        u8 unk0[0x8];
        CVoiceF3F60Sub* field_8;
    };

    struct CVoiceActorState {
        void* vtable;
        u8 unk4[0x3E9C - 4];
        u8 moveBasePlaceholder[4];
        u8 unk3EA0[0x3F00 - 0x3EA0];
        u32 field_3F00;
        u8 unk3F04[4];
        u32 field_3F08;
        u8 unk3F0C[0x1C];
        u16 field_3F28;
        u8 unk3F2A[0x36];
        CVoiceF3F60Obj* field_3F60;
    };

    struct CBattleCountAccessor {
        u8 unk0[0x194];
        int field_194;
    };
    struct CVoiceBattleNode {
        CVoiceBattleNode* next;
    };
    struct CBattleListAccessor {
        u8 unk0[0x28];
        CVoiceBattleNode* list0;
    };
    struct CBattleManagerNodeList {
        u8 unk0[0x48];
        CVoiceBattleNode* sentinel;
    };
    struct CVoiceBFC68 {
        u8 pad0[0x3ED4];
        void* field_3ED4;
    };
    struct CVoiceSrcNode {
        u8 pad0[4];
        void* field_4;
    };

    class __declspec(novtable) CCharVoiceMan {
    public:
        CCharVoiceMan();
        virtual ~CCharVoiceMan();
        virtual void FactoryEvent1();
        virtual void FactoryEvent2(int eventId);
        virtual void FactoryEvent3();
        virtual void FactoryEvent4();
        u8 unk4[0x200];
        u32 allocHead;
        u32 headOffset;
        CSoundNode* nodeHead;
        CSoundNode* nodeTail;
        u8 unk214;
        u8 unk215;
        u8 unk216[2];
        float unk218;
        u32 unk21C;
        u8 unk220;
        u8 unk221;
        u8 fieldFlag;
        u8 autoTalkPending;
        u32 unk224;
        u8 unk228;
        u8 unk229;
        u8 unk22A;
        u8 unk22B;
        u32 unk22C;
        u32 unk230;
    };
}
extern "C" cf::CCharVoiceMan* __ct__CCharVoiceMan(cf::CCharVoiceMan* self);
extern "C" const float lbl_eu_80668C68;
extern "C" u8 lbl_eu_805398CC[28];
class CScn;
extern "C" u32 lbl_eu_80664A5C;
extern "C" mtl::ALLOC_HANDLE func_80496004(void* src);
extern "C" int func_802B03A4(void* self);
extern "C" cf::CSoundNode* func_802A9604();
extern "C" cf::CSoundNode* __ct__802A4E48();
extern "C" cf::CSoundNode* __ct__802A96C0();
extern "C" cf::CSoundNode* func_802A8B6C();
extern "C" cf::CSoundNode* func_802B5970();
extern "C" cf::CSoundNode* __ct__CVS_THREAD_ORDER(int order);
extern "C" cf::CSoundNode* __ct__802A8DE8();
extern "C" cf::CSoundNode* func_802A6958(void* actor);
extern "C" cf::CSoundNode* __ct__802A5830();
extern "C" cf::CSoundNode* __ct__802A8C04();
extern "C" cf::CSoundNode* func_802B8D4C();
extern "C" cf::CSoundNode* func_802A3EF0();
extern "C" cf::CSoundNode* func_802AF3DC(void* self);
extern "C" cf::CSoundNode* func_802A5A14(void* a, void* c);
extern "C" cf::CSoundNode* __ct__802A7254(void* self, void* c);
extern "C" cf::CSoundNode* __ct__802A5ED4();
extern "C" cf::CSoundNode* func_802AF4FC(void* self, void* c);
extern "C" cf::CSoundNode* func_802AF43C(void* a, void* b, void* c);
extern "C" cf::CSoundNode* func_802A5B04(void* self, int flag);
extern "C" cf::CSoundNode* func_802AF49C(void* self);
extern "C" int func_80148778(void* self, int id);
extern "C" int __ptmf_cmpr(void* a, void* b);
extern "C" long __ptmf_test(void* ptmf);
class CVS_THREAD;
extern "C" u8 lbl_eu_805398C0[12];
extern "C" void* func_8016FE34(void* src);
extern "C" void func_802AF9D0(void* r3, int voiceId, int r5);
extern "C" void func_800B88E0(void* self, cf::IFactoryEvent* event);
extern "C" cf::CSoundNode* func_802AF56C(void* c);
extern "C" int func_802A7870(void* arr, int capacity, int unused);
extern "C" cf::CSoundNode* __ct__802A92D8(void* b, void* a);
extern "C" int func_802A3E88(void* node);
extern "C" cf::CSoundNode* func_802A6820(int newCount, int oldCount);
extern "C" cf::CSoundNode* __ct__802AF5CC(int a);
extern "C" cf::CSoundNode* __ct__802AB5F0();
extern "C" cf::CSoundNode* __ct__802A4870(int a);
extern "C" cf::CSoundNode* __ct__802AFA80(void* handle, void* actor);
extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void* self);
extern "C" cf::CSoundNode* func_802B0344(void* src);
extern "C" u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv(void);
extern "C" void* func_802A7A54(int a);
extern "C" void func_802A3ACC(void* node);
extern "C" void func_802A3E74(void* node);
extern "C" u8  lbl_eu_80663DA0;
extern "C" u16 lbl_eu_80663E42;
extern "C" float lbl_eu_80668C6C;
extern "C" float lbl_eu_80668C70;
extern "C" float lbl_eu_80668C74;
extern "C" float lbl_eu_80668C78;
extern "C" float lbl_eu_80668C7C;
extern "C" cf::CSoundNode* func_802A6DF4();
extern "C" float lbl_eu_80668C80;
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
#include "kyoshin/cf/CBattleManagerApi.hpp"
extern "C" cf::CSoundNode* __ct__CVS_THREAD_PARTY_GAGE();
extern "C" cf::CSoundNode* __ct__802A6AA8();
extern "C" cf::CSoundNode* __ct__802A5B88();
extern "C" cf::CSoundNode* __ct__802A86CC();
extern "C" cf::CSoundNode* __ct__802A6E84();
extern "C" cf::CSoundNode* func_802B9064();
extern "C" cf::CSoundNode* func_802A4798();
extern "C" cf::CSoundNode* func_802AE38C();
extern "C" cf::CSoundNode* func_802A7674();
extern "C" cf::CSoundNode* func_802A8AC8();
extern "C" cf::CSoundNode* func_802A7744();
extern "C" cf::CSoundNode* func_802A8628(void* self);
