#pragma once

#include <types.h>
#include "kyoshin/cf/voice/CCharVoice.hpp"
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

    struct CVoiceBtlSlot {
        u32 id0;
        u32 id1;
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
        u8 mArena[0x3E9C - 4];
        u8 moveBasePlaceholder[4];
        u8 unk3EA0[0x3F00 - 0x3EA0];
        u32 field_3F00;
        u8 unk3F04[4];
        u32 field_3F08;
        u8 unk3F0C[0x1C];
        u16 field_3F28;
        u8 unk3F2A[0x36];
        CVoiceF3F60Obj* field_3F60;
    int CharVoiceMan_TryPlayCharVoice();
    };

    struct CBattleCountAccessor {
        u8 unk0[0x194];
        int field_194;
    };
    struct CVoiceSrcNode {
        u8 pad0[4];
        void* field_4;
    };

    // Character-voice manager singleton: registers itself with the factory
    // manager (IFactoryEvent) and owns the per-frame voice-event list plus the
    // ring arena its nodes are allocated from.
    class CCharVoiceMan : public IFactoryEvent {
    public:
        CCharVoiceMan();
        virtual ~CCharVoiceMan();
        virtual void FactoryEvent2(CfObject* obj);
        u8 mArena[0x200];
        u32 mAllocOffset;
        u32 mHeadOffset;
        CSoundNode* mNodeHead;
        CSoundNode* mNodeTail;
        u8 mActive;
        u8 mPaused;
        u8 unk216[2];
        float mVoiceTimer;
        u32 mPrevBattleCount;
        u8 mGaugeState;
        u8 mInteractState;
        u8 mFieldFlag;
        u8 mAutoTalkPending;
        u32 mPcVoiceCount;
        u8 mBattleVoiceLatched;
        u8 mPendingVoice;
        u8 mBattleActive;
        u8 unk22B;
        u32 mPendingActorId;
        u32 mPendingActionId;
    };
}
class CScn;
extern "C" u32 sVoiceEventIdCounter;
extern "C" mtl::ALLOC_HANDLE Scn_CallUnk8C_V10(void* src);
extern "C" int BtlBeg_IsVoiceBit_03A4(void* self);
extern "C" cf::CSoundNode* VisBreak_AllocVoiceHandle();
class CVS_THREAD_BUF { public: static cf::CSoundNode* create(); };
class CVS_THREAD_VISION_TELL { public: static CVS_THREAD_VISION_TELL* create(struct CVoiceHandle*, struct CVoiceHandle*); };
extern "C" cf::CSoundNode* ReviveVxInitVoice3C();
extern "C" cf::CSoundNode* ErrWin_PlayBattleVoice_5970();
extern "C" cf::CSoundNode* __ct__CVS_THREAD_ORDER(struct CVoiceHandle* owner);
class CVS_THREAD_TENSION_UP { public: static CVS_THREAD_TENSION_UP* create(struct CVoiceFactory*, int); };
extern "C" cf::CSoundNode* playEhpStartVoice(void* actor);
namespace cf { class CVS_THREAD_CHAIN { public: static CVS_THREAD_CHAIN* create(); }; }
class CVS_THREAD_SUDDEN { public: static CVS_THREAD_SUDDEN* create(); };
extern "C" cf::CSoundNode* requestOrderConfigVoice();
extern "C" cf::CSoundNode* FindThreadVoiceNode();
extern "C" cf::CSoundNode* chainVoiceLifetimeHandler(void* a, void* c);
class CVS_THREAD_HP { public: static CVS_THREAD_HP* create(struct CVoiceHandle*, int); };
class CVS_THREAD_EHP { public: static CVS_THREAD_EHP* create(struct CVoiceHandle*, struct CVoiceHandle*, s32); };
extern "C" cf::CSoundNode* chainVoiceActorTrigger(void* self, int flag);
extern "C" int isBattleStatusActive(void* self, int id);
extern "C" int __ptmf_cmpr(void* a, void* b);
extern "C" long __ptmf_test(void* ptmf);
class CVS_THREAD;
// PMF reference entry owned by CCharVoiceMan.cpp; compared by address via
// __ptmf_cmpr/__ptmf_test, never dispatched here.
struct CCharVoice;
typedef void (CCharVoice::*VoiceManPmf)();
extern "C" VoiceManPmf voiceNoopPmfEntry;
extern "C" void* MapFx_ResolveOwner(void* src);
extern "C" void unlinkFactoryById(void* self, cf::IFactoryEvent* event);
class CVS_THREAD_VISION_BREAK { public: static CVS_THREAD_VISION_BREAK* create(struct CVoiceHandle*, int); };
extern "C" int IsThreadVoiceActive(void* node);
extern "C" cf::CSoundNode* selectEhpVoicePair(int newCount, int oldCount);
class CVS_THREAD_BATTLE_MAIN { public: static void* create(int); };
class CVS_THREAD_BATTLE_END_SP { public: static CVS_THREAD_BATTLE_END_SP* create(); };
class CVS_THREAD_BATTLE_END { public: static CVS_THREAD_BATTLE_END* create(int); };
class CVS_THREAD_BATTLE_BEGIN { public: static CVS_THREAD_BATTLE_BEGIN* create(struct CVoiceHandle*, struct CVoiceHandle*); };
extern "C" cf::CfObjectImplPc18* getCfObjectPc__FPQ22cf12CfObjectMove(void* self);
extern "C" cf::CSoundNode* BtlBeg_PlayLine_0344(void* src);
extern "C" u32 getQueuedFileEventCount__Q22cf13CfGameManagerFv(void);
extern "C" void* HpVoice_FindBySubState(int a);
extern "C" void RunVoiceNodeHandler(void* node);
extern "C" void releaseThreadVoice(void* node);
extern "C" u8  visionSharedStateFlags;
extern "C" u16 gmGlobalFlag42;
extern "C" cf::CSoundNode* playFaintVoice();
extern "C" int CCharVoiceMan_EnqueueFaintVoice(void);
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);

extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
#include "kyoshin/cf/CBattleManager.hpp"
class CVS_THREAD_PARTY_GAGE { public: static CVS_THREAD_PARTY_GAGE* create(int, int); };
class CVS_THREAD_FAINT { public: static CVS_THREAD_FAINT* create(struct CVoiceHandle*, struct CVoiceHandle*); };
class CVS_THREAD_DOWN { public: static CVS_THREAD_DOWN* create(struct CVoiceHandle*, struct CVoiceHandle*); };
class CVS_THREAD_REVIVE { public: static CVS_THREAD_REVIVE* create(struct CVoiceHandle*, struct CVoiceHandle*); };
class CVS_THREAD_HAGE { public: static CVS_THREAD_HAGE* create(struct CVoiceHandle*, struct CVoiceHandle*); };
extern "C" cf::CSoundNode* BattleGaugeTierVoiceGate();
extern "C" cf::CSoundNode* PlayRandomVoice();
extern "C" cf::CSoundNode* TutorialList_PlayTutorialVoice();
extern "C" cf::CSoundNode* ReviveVxInitVoice6E();
extern "C" cf::CSoundNode* PartyGage_InitVoiceId_8628(void* self);
