#include "kyoshin/cf/chain/CChainEffect.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/voice/CCharVoiceMan.hpp"
#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_MAIN.hpp"
#include "kyoshin/cf/voice/cvsys/HpVoiceApi.hpp"
#include <decomp.h>
#include "kyoshin/cfsys/CfObjectImplPc.hpp"
// Local real dispatch for voice nodes: mirrors CVS_THREAD's vtable at +0x1C.
// Defined here to avoid pulling CVS_THREAD's extern "C" overloads into this TU.
struct VoiceNodePrefix {
    u8 _00[0xC];
    u32 _0C;
    u32 _10;
    s32 _14;
    u32 _18;
};
class __declspec(novtable) VoiceNode : public VoiceNodePrefix {
public:
    virtual void PlayThreadVoice();
    virtual void DetachThreadVoice(u32 id);
    virtual int blank1();
    virtual int func_802A1EA0();
    virtual void* func_802A3740(void* a, void* b);
    virtual int blank2();
};
#include "kyoshin/UnkClass_805764CC.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

// Character-voice constant pool (retail .sdata2: 0x80668C68..0x80668C88).
// The 0x20-byte slice holds the six manager floats plus the (30.0, 0.0) pair;
// the pool continues in the neighbour units (0x80668C88+), so those constants
// are extern there, not defined here. A single struct freezes MWCC's
// declaration order (bare individuals reorder by first use).
struct Sdata2_CCharVoiceMan {
    float f0;      // 80668C68 0.0
    float f1;      // 80668C6C 150.0
    float f2;      // 80668C70 0.5
    float f3;      // 80668C74 1800.0
    float f4;      // 80668C78 3.0
    float f5;      // 80668C7C 1.0
};
// The trailing (30.0, 0.0) pair lives in its own object so 30.0 sits at
// offset 0 and every use is a plain sda21 load: a nonzero member offset makes
// MWCC materialise the section base (`li r3, 0` + `lfs 0x18(r3)`), four bytes
// more than retail's single `lfs`.
struct Sdata2_CCharVoiceManTail {
    float c30;     // 80668C80 30.0
    float c0;      // 80668C84 0.0
};
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
const Sdata2_CCharVoiceMan sdata2_CCharVoiceMan = {
    0.0f, 150.0f, 0.5f, 1800.0f, 3.0f, 1.0f
};
__declspec(section ".sdata2") __attribute__((used, aligned(8)))
const Sdata2_CCharVoiceManTail sdata2_CCharVoiceManTail = { 30.0f, 0.0f };
#define lbl_eu_80668C68 sdata2_CCharVoiceMan.f0
#define lbl_eu_80668C7C sdata2_CCharVoiceMan.f5
#define lbl_eu_80668C80 sdata2_CCharVoiceManTail.c30

// Global-scope free functions matching the retail C names: MWCC mangles a
// global no-arg getInstance() to getInstance__Fv and a global
// registerFactoryEvent(void*, cf::IFactoryEvent*) to
// registerFactoryEvent__FPvPQ22cf13IFactoryEvent, so the ctor/dtor call relocs emit
// the exact retail symbols (declaring them as UnkClass_805764CC members would
// mangle with the class prefix). getInstance is already declared
// `extern void* getInstance()` by kyoshin/cf/object/CAIAction.hpp in the
// include chain; B8804 gets its own global decl here. unlinkFactoryById is the
// bare extern "C" retail symbol (0x800B91FC) used by the destructor.
void registerFactoryEvent(void* self, cf::IFactoryEvent* event);
// These were previously provided by the CAIAction.hpp include chain (dropped
// to avoid a duplicate getInstance__Q22cf14CBattleManagerFv declaration);
// plain C++ linkage mangles them to the retail names getInstance__Fv /
// CBattleMan_FetchVisionObj__FPv (symbol-map mapped).
extern UnkClass_805764CC* getInstance();

// include chain; C++ linkage mangles to the retail names).
u32 CtrlRemote_TouchBitByArg(u32 resourceId);
void CtrlRemote_SetSharedBit(u32 owner, u32 flag);
bool isGlobalCamFlagSet(int mask);                // isGlobalCamFlagSet__Fi

namespace cf{
    CCharVoiceMan::CCharVoiceMan(){
        const float resetFloat = lbl_eu_80668C68;
        mAllocOffset = 0;
        mHeadOffset = 0;
        mNodeHead = 0;
        mNodeTail = 0;
        mActive = 1;
        mPaused = 0;
        mVoiceTimer = resetFloat;
        mPrevBattleCount = 0;
        mGaugeState = 0;
        mInteractState = 0;
        mFieldFlag = 0;
        mAutoTalkPending = 0;
        mPcVoiceCount = 0;
        mPendingVoice = 0;
        mBattleActive = 0;
        mPendingActorId = 0;
        mPendingActionId = 0;
        UnkClass_805764CC* classPtr = (UnkClass_805764CC*)getInstance();
        registerFactoryEvent(classPtr, (cf::IFactoryEvent*)this);
    }

    CCharVoiceMan::~CCharVoiceMan(){
        UnkClass_805764CC* classPtr = (UnkClass_805764CC*)getInstance();
        unlinkFactoryById(classPtr, (cf::IFactoryEvent*)this);
    }
}

cf::CCharVoiceMan* spCharVoiceMan; // CCharVoiceMan singleton
// typed .sbss second word (replaces the old sbss byte filler)
extern "C" { u32 sVoiceEventIdCounter; } // global character-voice counter

// C++-mangled findObjectById__Fi (actor-id -> action source).
extern void* findObjectById(BOOL id);

extern "C" bool CCharVoiceMan_AlwaysTrue() { return true; }

extern "C" void CCharVoiceMan_ClearFieldFlag() {
    cf::CCharVoiceMan* voice_man = spCharVoiceMan;
    if (voice_man != 0)
        voice_man->mFieldFlag = 0;
}
// If the character-voice-disable flag is clear, flag the manager's byte 0x229.
extern "C" void CCharVoiceMan_FlagPendingVoice() {
    if (gmGameFlags & 0x00400000)
        return;
    spCharVoiceMan->mPendingVoice = 1;
}
// Ring-buffer allocate `size` bytes from CCharVoiceMan's 0x200-byte arena
// [head mAllocOffset / tail mHeadOffset]; returns 0 if full or the flag/cooldown
// gates are active.
extern "C" u8* CCharVoiceMan_AllocVoiceArena(int size) {
    if (gmGameFlags & 0x10000000)
        return 0;
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (m->mPaused != 0)
        return 0;
    if ((int)m->mAllocOffset < (int)m->mHeadOffset) {
        // Linear region still free before the tail.
        if ((int)m->mHeadOffset - (int)m->mAllocOffset < size)
            return 0;
        m->mAllocOffset = (int)m->mAllocOffset + size;
        return spCharVoiceMan->mArena + spCharVoiceMan->mAllocOffset - size;
    }
    // Wrapped: space up to the 0x200 limit, else restart at the buffer base.
    if (0x200 - (int)m->mAllocOffset < size) {
        if ((int)m->mHeadOffset < size)
            return 0;
        m->mAllocOffset = size;
        return spCharVoiceMan->mArena;
    }
    m->mAllocOffset = (int)m->mAllocOffset + size;
    return spCharVoiceMan->mArena + spCharVoiceMan->mAllocOffset - size;
}
extern "C" unsigned int CCharVoiceMan_MakeVoiceEventId(unsigned int value) { unsigned int counter = sVoiceEventIdCounter; sVoiceEventIdCounter = counter + 1; return (counter << 16) | (value & 0xFFFF); }
extern "C" int CCharVoiceMan_NullVoiceDispatch(void* self) { return 0; }

// Allocate the 0x234-byte CCharVoiceMan and store it as the singleton.
extern "C" void CCharVoiceMan_CreateSingleton() {
    mtl::ALLOC_HANDLE handle = Scn_CallUnk8C_V10(gmScene);
    cf::CCharVoiceMan* man = new (handle) cf::CCharVoiceMan();
    spCharVoiceMan = man;
    sVoiceEventIdCounter = 0;
}
// Reset the voice-event list: run each node's level-thread handler (+0x08),
// then clear the list and all resetable manager fields.
extern "C" void CCharVoiceMan_FlushAll() {
    bool match;
    cf::CSoundNode* node;
    cf::CCharVoiceMan* m = spCharVoiceMan;
    m->mPaused = 0;
    node = m->mNodeHead;
    while (node != 0) {
        match = false;
        if (__ptmf_cmpr(node, &voiceNoopPmfEntry) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match)
            ((VoiceNode*)node)->PlayThreadVoice();
        node = node->next;
    }
    m->mNodeHead = 0;
    float reset = lbl_eu_80668C68;
    m->mNodeTail = 0;
    m->mAllocOffset = 0;
    m->mHeadOffset = 0;
    spCharVoiceMan->mVoiceTimer = lbl_eu_80668C68;
    spCharVoiceMan->mPrevBattleCount = 0;
    spCharVoiceMan->mGaugeState = 0;
    spCharVoiceMan->mInteractState = 0;
    spCharVoiceMan->mAutoTalkPending = 0;
    spCharVoiceMan->mPcVoiceCount = 0;
    spCharVoiceMan->mPendingVoice = 0;
    spCharVoiceMan->mBattleActive = 0;
    spCharVoiceMan->mPendingActorId = 0;
    spCharVoiceMan->mPendingActionId = 0;
}
// Battle-gauge gate check (defined below); forward-declared for CCharVoiceMan_Tick.
// Retail references this helper by its bare unmangled symbol (C linkage).
extern "C" int CCharVoiceMan_CheckBattleGauge(cf::CCharVoiceMan* self);

// Main per-frame character-voice tick. Flush the level-thread list on a
// presentation pause, then run every gate (battle count, party gauge, field
// id, per-player flags and random gauges) that can enqueue a voice node.
extern "C" void CCharVoiceMan_Tick(){
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (m->mNodeHead != 0 && (gmGameFlags & 0x10000000)) {
        // Presentation pause: run every node's level-thread handler then flush.
        m->mPaused = 0;
        cf::CSoundNode* node = m->mNodeHead;
        while (node != 0) {
            bool match = false;
            if (__ptmf_cmpr(node, &voiceNoopPmfEntry) != 0 && __ptmf_test(node) != 0)
                match = true;
            if (match)
                ((VoiceNode*)node)->PlayThreadVoice();
            node = node->next;
        }
        m->mNodeHead = 0;
        m->mNodeTail = 0;
        m->mAllocOffset = 0;
        m->mHeadOffset = 0;
    }
    m->mPendingVoice = 0;
    if (m->mPaused == 0) {
        // Battle-participant count changed: gate a frequency voice node.
        int oldCount = m->mPrevBattleCount;
        void* bm = getInstance__Q22cf14CBattleManagerFv();
        int count = 0;
        cf::CVoiceBattleNode* cur;
        cf::CVoiceBattleNode* sentinel = ((cf::CBattleManagerNodeList*)bm)->sentinel;
        for (cur = sentinel->next; cur != sentinel; cur = cur->next)
            count++;
        m->mPrevBattleCount = count;
        if (count != oldCount && !(m->mVoiceTimer < 150.0f) &&
            !(gmGameFlags & 0x00400000)) {
            cf::CSoundNode* node = selectEhpVoicePair(count, oldCount);
            if (node != 0) {
                cf::CSoundNode* tail = m->mNodeTail;
                if (tail != 0)
                    tail->next = node;
                if (m->mNodeHead == 0) {
                    m->mNodeHead = node;
                    m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
                }
                m->mNodeTail = node;
            }
        }

        // Battle-participant play-gate: resolve the current player to a
        // battle object and, if its browse gate passes, voice the pending
        // action source (mPendingActionId) as a node.
        if (!(gmGameFlags & 0x00400000)) {
            cf::CVoiceBFC68* br =
                (cf::CVoiceBFC68*)getCfObjectPc__FPQ22cf12CfObjectMove(getPlayer__Q22cf13CfGameManagerFi(0));
            if (br != 0) {
                cf::CfObjectImplPcED4* edge = (cf::CfObjectImplPcED4*)br->field_3ED4;
                if (edge != 0 && edge->vf40(0x800) == 0) {
                    if (m->mPendingActionId != 0) {
                        void* src = MapFx_ResolveOwner(findObjectById((BOOL)m->mPendingActionId));
                        if (src != 0) {
                            cf::CSoundNode* node = BtlBeg_PlayLine_0344(src);
                            if (node != 0) {
                                cf::CSoundNode* tail = m->mNodeTail;
                                if (tail != 0)
                                    tail->next = node;
                                if (m->mNodeHead == 0) {
                                    m->mNodeHead = node;
                                    m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
                                }
                                m->mNodeTail = node;
                            }
                        }
                    }
                    m->mPendingActionId = 0;
                }
            }
        }

        // Party-gauge rising edge: enqueue a levelled voice node.
        u8 oldGauge = m->mGaugeState;
        m->mGaugeState = (u8)CCharVoiceMan_CheckBattleGauge(m);
        if (oldGauge == 0 && m->mGaugeState != 0 && !(gmGameFlags & 0x00400000)) {
            cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_BATTLE_MAIN::create(1);
            if (node != 0) {
                cf::CSoundNode* tail = m->mNodeTail;
                if (tail != 0)
                    tail->next = node;
                if (m->mNodeHead == 0) {
                    m->mNodeHead = node;
                    m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
                }
                m->mNodeTail = node;
            }
        }

        // Interact/battle voice: probe the top gauges and enqueue on rising edge.
        u8 oldInteract = m->mInteractState;
        void* bm2 = getInstance__Q22cf14CBattleManagerFv();
        cf::CVoiceBattleNode* cur2;
        int icount = 0;
        cf::CVoiceBattleNode* sentinel2 = ((cf::CBattleManagerNodeList*)bm2)->sentinel;
        for (cur2 = sentinel2->next; cur2 != sentinel2; cur2 = cur2->next)
            icount++;
        int iresult;
        cf::CVoiceActorState* arr2[3];
        if (icount <= 0) {
            iresult = 0;
        } else {
            int n = HpVoice_CollectIdle((CVoiceHandle**)arr2, 3, 0);
            if (n <= 1) {
                iresult = 0;
            } else {
                float limit = 0.5f;
                int i = 0;
                cf::CVoiceActorState** it = arr2;
                while (i < n) {
                    // Fail (result 0) when any top gauge reaches the limit.
                    if (limit <= ((cf::CfObjectImplPc18*)*it)->v130()) {
                        iresult = 0;
                        goto interactStore;
                    }
                    ++it;
                    ++i;
                }
                iresult = 1;
            }
        }
interactStore:
        m->mInteractState = (u8)iresult;
        if (oldInteract == 0 && m->mInteractState != 0 && !(gmGameFlags & 0x00400000)) {
            cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_BATTLE_MAIN::create(0);
            if (node != 0) {
                cf::CSoundNode* tail = m->mNodeTail;
                if (tail != 0)
                    tail->next = node;
                if (m->mNodeHead == 0) {
                    m->mNodeHead = node;
                    m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
                }
                m->mNodeTail = node;
            }
        }

        // Field-id window [0x109,0x115]: retail keeps two separate signed
        // cmpi branches; swapping the operand order (const OP v) blocks
        // MWCC's unsigned range-fusion (MWCC_PATTERNS "swapped-operand
        // range guards").
        int phase = getQueuedFileEventCount__Q22cf13CfGameManagerFv();
        u32 gaugeBit = 1 << (gmGlobalFlag42 - 1);
        if (0x108 >= phase)
            goto noFieldFlag;
        if (0x116 <= phase)
            goto noFieldFlag;
        if (!(gaugeBit & 0xFE00))
            goto noFieldFlag;
        m->mFieldFlag = 1;
noFieldFlag:;

        // Battle voice auto-talk: resolve the current player and fire.
        if (m->mAutoTalkPending != 0) {
            cf::CVoiceSrcNode* srcNode = (cf::CVoiceSrcNode*)HpVoice_FindBySubState(1);
            cf::CfObjectMove* player = (cf::CfObjectMove*)getPlayer__Q22cf13CfGameManagerFi(0);
            if (player != 0)
                player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
            if (srcNode != 0 && player != 0) {
                cf::CfObjectImplPcBattle4* inner = (cf::CfObjectImplPcBattle4*)srcNode->field_4;
                u32 sv = *inner->vf30();
                // Resolution failure keeps the pending flag set to retry next
                // frame; every other path falls into the single clear store.
                if (testActorStatus(srcNode, (int*)&sv, 6) == 0)
                    goto noAutoTalkClear;
                if (!(gmGameFlags & 0x00400000))
                    BmVoice_PlayGated((CVoiceHandle*)player, 0xbb9, 0x14);
            }
            m->mAutoTalkPending = 0;
        }
noAutoTalkClear:;

        // No battle participants and the flag is clear: rolling gauge node.
        if (m->mBattleActive != 0) {
            void* bm3 = getInstance__Q22cf14CBattleManagerFv();
            cf::CVoiceBattleNode* cur3;
            int acount = 0;
            cf::CVoiceBattleNode* sentinel3 = ((cf::CBattleManagerNodeList*)bm3)->sentinel;
            for (cur3 = sentinel3->next; cur3 != sentinel3; cur3 = cur3->next)
                acount++;
            if (acount <= 0 && !(gmGameFlags & 0x00400000)) {
                float g = m->mVoiceTimer;
                float f1 = g / 1800.0f;
                bool ge = (3.0f <= f1);
                m->mVoiceTimer = lbl_eu_80668C68;
                bool appended = false;
                if (ml::math::mtRand(0x64) < 5) {
                    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_BATTLE_END_SP::create();
                    if (node != 0) {
                        cf::CSoundNode* tail = m->mNodeTail;
                        if (tail != 0)
                            tail->next = node;
                        if (m->mNodeHead == 0) {
                            m->mNodeHead = node;
                            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
                        }
                        m->mNodeTail = node;
                        appended = true;
                    }
                }
                if (!appended) {
                    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_BATTLE_END::create((int)ge);
                    if (node != 0) {
                        cf::CSoundNode* tail = m->mNodeTail;
                        if (tail != 0)
                            tail->next = node;
                        if (m->mNodeHead == 0) {
                            m->mNodeHead = node;
                            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
                        }
                        m->mNodeTail = node;
                    }
                }
            }
        }
        m->mBattleActive = 0;

        // Non-presentation: sweep level-thread handlers then pop leading
        // non-matching nodes off the list.
        if (m->mNodeHead != 0 && !(gmGameFlags & 0x00400000)) {
            cf::CSoundNode* cur = m->mNodeHead;
            while (cur != 0) {
                RunVoiceNodeHandler(cur);
                cur = cur->next;
            }
            cf::CSoundNode* head;
            // Reload the head from the manager every iteration like retail:
            // the loop condition dereferences m->mNodeHead fresh each pass.
            while ((head = m->mNodeHead) != 0) {
                bool match = false;
                if (__ptmf_cmpr(head, &voiceNoopPmfEntry) != 0 && __ptmf_test(head) != 0)
                    match = true;
                if (match)
                    break;
                // Retail keeps an explicit re-read + null-guard here instead
                // of reusing the loop-condition head pointer.
                if (m->mNodeHead != 0) {
                    cf::CSoundNode* nxt = m->mNodeHead->next;
                    if (nxt != 0) {
                        m->mNodeHead = nxt;
                        m->mHeadOffset = (u32)nxt - (u32)&m->mArena[0];
                    } else {
                        m->mNodeHead = 0;
                        m->mNodeTail = 0;
                        m->mAllocOffset = 0;
                        m->mHeadOffset = 0;
                    }
                }
            }
        }

        // Passive cell count: accumulate every tick unless the flag blocks it.
        if (!(visionSharedStateFlags & 1) && !(gmGameFlags & 0x00400000))
            m->mVoiceTimer += lbl_eu_80668C7C;
    }
}
// Battle-voice state probe: only for an interact/battle-flagged actor in
// state id 1 whose +0x3F60 chain selects a voice id in [0x10, 0x17]. When the
// source is dead (or luck holds on a 15/100 roll) flag byte 0x223, then always
// poke the sub-id handler with (voicePtr + 0x3508, 1).
extern "C" void CCharVoiceMan_CheckAutoTalk(cf::CVoiceActorState* self) {
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (!(self->field_3F00 & 0x2))
        return;
    if (self->field_3F28 != 1)
        return;
    int v = self->field_3F60->field_8->field_18;
    if (v < 0x10)
        return;
    if (v > 0x17)
        return;
    {
        u8* t = (u8*)(v + 0x3508);
        if (CtrlRemote_TouchBitByArg((int)t) == 0 || ml::math::mtRand(0x64) < 0xF)
            m->mAutoTalkPending = 1;
        CtrlRemote_SetSharedBit((u32)t, 1);
    }
}
// Battle-slot voice hook: if the battle slot manager reports a slot whose id
// matches b's voice id, copy a's pending id into mPendingActorId.
extern "C" void CCharVoiceMan_HookBattleSlotVoice(cf::CVoiceActorInfo* a, cf::CVoiceActorInfo* b) {
    cf::CCharVoiceMan* m = spCharVoiceMan;
    cf::CVoiceBtlSlot* bs = (cf::CVoiceBtlSlot*)cf::CBattleManager::getInstance()->CBattleMan_FetchVisionObj();
    if (bs != 0) {
        getInstance__Q22cf13CfGameManagerFv();
        if (isGlobalCamFlagSet(0x04000000) == 0) {
            if (a != 0 && b != 0) {
                u32 af = a->field_3F10;
                u32 bf = b->field_3F10;
                if (bf == bs->id0 || bf == bs->id1)
                    m->mPendingActorId = af;
            }
        }
    }
}
extern "C" void CCharVoiceMan_DestroySingleton() {
    // Virtual-delete the singleton manager (vtable+8 dtor slot, deleting
    // flag r4=1); delete's own null-check reuses the outer compare, giving
    // retail's double beq. The manager field store is inside the guard.
    if (spCharVoiceMan != 0) {
        delete spCharVoiceMan;
        spCharVoiceMan = 0;
    }
}
// Set the manager's voice-enable byte, then run every matching node's +0x14
// voice probe; nodes whose probe returns nonzero are handed to releaseThreadVoice.
extern "C" void CCharVoiceMan_ReleaseVoices(u8 flag) {
    spCharVoiceMan->mPaused = flag;
    bool match;
    cf::CSoundNode* node = spCharVoiceMan->mNodeHead;
    while (node != 0) {
        match = false;
        if (__ptmf_cmpr(node, (void*)&voiceNoopPmfEntry) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match && ((VoiceNode*)node)->func_802A1EA0() != 0)
            releaseThreadVoice(node);
        node = node->next;
    }
}
// Character-voice play gate: if the actor is interact/battle-flagged and its
// state checks pass, resolve the target's voice action and fire a battle/lvl
// voice node.
// Retail references this helper by its bare unmangled symbol (C linkage).
// (memberized: CCharVoiceMan_TryPlayCharVoice -> CVoiceActorState::CharVoiceMan_TryPlayCharVoice; declaration moved into class CVoiceActorState)
int cf::CVoiceActorState::CharVoiceMan_TryPlayCharVoice() {
    if (!(this->field_3F00 & 0x2))
        return 0;
    if (((cf::CfObjectImplPc18*)this)->v2BC() != 0)
        return 0;
    if (this->field_3F28 != 1)
        return 0;
    // State already registers either slot 0xCF or 0xD0; otherwise bail.
    if (isBattleStatusActive(&this->mArena[4], 0xcf) == 0 && isBattleStatusActive(&this->mArena[4], 0xd0) == 0)
        return 0;
    cf::CfObjectImplPcSub3E9C* move = &((cf::CfObjectImplPc18*)this)->mSub;
    u32 moveRes = move->sf4C();
    void* r = MapFx_ResolveOwner(findObjectById((BOOL)moveRes));
    if (r == 0)
        return 0;
    if (((cf::CVoiceActorState*)r)->field_3F28 != 0x10c)
        return 0;
    BmVoice_PlayGated((CVoiceHandle*)this, 0xfa5, 0x14);
    return 1;
}
// Register a freshly-created sound node, then clear byte 0x22C.
extern "C" void CCharVoiceMan_PushFreshSoundNode() {
    cf::CSoundNode* node = VisBreak_AllocVoiceHandle();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
    spCharVoiceMan->mPendingActorId = 0;
}
// Append a freshly-created sound node to the manager's voice-event list.
extern "C" void CCharVoiceMan_PushChainVoiceNode(void* a, void* b) {
    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_VISION_TELL::create((CVoiceHandle*)a, (CVoiceHandle*)b);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
// Resolve a pending voice-action id (from the manager's 0x22C holder) and
// enqueue an action node plus a clear node, if the arguments are non-null.
extern "C" void CCharVoiceMan_EnqueuePendingActionVoice(void* a, void* b, void* c) {
    if (b == 0) {
        u32 pending = spCharVoiceMan->mPendingActorId;
        if (pending != 0)
            b = (void*)MapFx_ResolveOwner((void*)findObjectById((BOOL)pending));
    }
    if (b != 0) {
        cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_VISION_BREAK::create((CVoiceHandle*)b, (int)a);
        cf::CCharVoiceMan* m = spCharVoiceMan;
        if (node != 0) {
            cf::CSoundNode* tail = m->mNodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->mNodeHead == 0) {
                m->mNodeHead = node;
                m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
            }
            m->mNodeTail = node;
        }
    }
    if (c != 0) {
        cf::CSoundNode* node = (cf::CSoundNode*)BmVoice_Play32((CVoiceHandle*)c);
        cf::CCharVoiceMan* m = spCharVoiceMan;
        if (node != 0) {
            cf::CSoundNode* tail = m->mNodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->mNodeHead == 0) {
                m->mNodeHead = node;
                m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
            }
            m->mNodeTail = node;
        }
    }
}
// Player-voice enqueue: if the actor's battle move-base resolves to a live
// battle actor, construct a battle-begin node and append it to the list.
extern "C" void CCharVoiceMan_EnqueueBattleBeginVoice(cf::CVoiceActorState* self) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CfObjectImplPcSub3E9C* move = &((cf::CfObjectImplPc18*)self)->mSub;
    u32 moveRes = move->sf4C();
    void* actor = findObjectById((BOOL)moveRes);
    if (actor != 0)
        actor = (void*)((u8*)actor - 0x3E9C);
    if (actor == 0)
        return;
    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_BATTLE_BEGIN::create((CVoiceHandle*)self, (CVoiceHandle*)actor);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
// If the passed actor is active, teed a u32 from +0x3F10 into mPendingActionId.
extern "C" void CCharVoiceMan_LatchPendingVoiceId(cf::CVoiceActorInfo* self) {
    if (BtlBeg_IsVoiceBit_03A4(self) != 0) {
        spCharVoiceMan->mPendingActionId = self->field_3F10;
    }
}
// Enqueue a break-freq node [chainVoiceLifetimeHandler(a,c)] and a party-gauge node
// [BmVoice_Play12C(a,b,c)] onto the listener list (unless paused).
extern "C" void CCharVoiceMan_EnqueueBreakAndGaugeVoice(void* a, void* b, void* c) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = chainVoiceLifetimeHandler(a, c);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
    node = (cf::CSoundNode*)BmVoice_Play12C((CVoiceHandle*)a);
    cf::CCharVoiceMan* m2 = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m2->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m2->mNodeHead == 0) {
            m2->mNodeHead = node;
            m2->mHeadOffset = (u32)node - (u32)&m2->mArena[0];
        }
        m2->mNodeTail = node;
    }
}
// Battle/interact voice trigger. If the actor's battle flag is set, resolve a
// battle-voice node (gated on battle count + actor flag); otherwise enqueue a
// plain voice node.
extern "C" void CCharVoiceMan_TriggerBattleOrPlainVoice(cf::CVoiceActorState* self) {
    if (self->field_3F00 & 0x2) {
        if (gmGameFlags & 0x00400000)
            return;
        int useBattle = 0;
        if (((cf::CBattleCountAccessor*)getInstance__Q22cf14CBattleManagerFv())->field_194 < 0x64 &&
            (self->field_3F08 & 0x10000))
            useBattle = 1;
        cf::CSoundNode* node = chainVoiceActorTrigger(self, useBattle);
        cf::CCharVoiceMan* m = spCharVoiceMan;
        if (node != 0) {
            cf::CSoundNode* tail = m->mNodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->mNodeHead == 0) {
                m->mNodeHead = node;
                m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
            }
            m->mNodeTail = node;
        }
    } else {
        cf::CSoundNode* node = (cf::CSoundNode*)BmVoice_Play0A((CVoiceHandle*)self);
        cf::CCharVoiceMan* m = spCharVoiceMan;
        if (node != 0) {
            cf::CSoundNode* tail = m->mNodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->mNodeHead == 0) {
                m->mNodeHead = node;
                m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
            }
            m->mNodeTail = node;
        }
    }
}
// Construct a FAINT voice node and enqueue it; return its event id (or -1).
extern "C" int CCharVoiceMan_EnqueueFaintVoice(void) {
    if (gmGameFlags & 0x00400000)
        return -1;
    cf::CSoundNode* node = playFaintVoice();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    bool appended;
    if (node == 0) {
        appended = false;
    } else {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
        appended = true;
    }
    if (appended)
        return (int)node->field_18;
    return -1;
}
// Battle/interact pending-voice hook: on the first pass run the state probe
// (CCharVoiceMan_TryPlayCharVoice); if it fires, latch byte 0x228 so later passes fall through
// to the plain battle-voice node enqueue instead.
extern "C" void CCharVoiceMan_HookPendingBattleVoice(cf::CVoiceActorState* self) {
    if (gmGameFlags & 0x00400000)
        return;
    if (spCharVoiceMan->mBattleVoiceLatched == 0) {
        if (self->CharVoiceMan_TryPlayCharVoice() != 0) {
            spCharVoiceMan->mBattleVoiceLatched = 1;
            return;
        }
    }
    cf::CSoundNode* node = PartyGage_InitVoiceId_8628(self);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
// If the passed actor's move-base is the current player, raise the manager's
// voice count; on the 3rd+ tick roll a 25% chance to play the 0x89c battle
// voice with a 0x118 flavour.
extern "C" void CCharVoiceMan_TickPlayerVoiceCount(cf::CVoiceActorBase* actor) {
    if (gmGameFlags & 0x00400000)
        return;
    const u8* movePc = (const u8*)actor;
    if (actor != 0)
        movePc = (const u8*)&actor->moveBase;
    if (movePc == (const u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
        cf::CCharVoiceMan* m = spCharVoiceMan;
        m->mPcVoiceCount++;
        if ((int)m->mPcVoiceCount >= 3) {
            if (ml::math::mtRand(0x64) < 0x19)
                BmVoice_PlayGated((CVoiceHandle*)actor, 0x89c, 0x118);
        }
    }
}
// If the passed actor's move-base is the current player, clear voice count 0x224.
extern "C" void CCharVoiceMan_ClearPlayerVoiceCount(cf::CVoiceActorBase* actor) {
    if (!(gmGameFlags & 0x00400000)) {
        const u8* movePc = (const u8*)actor;
        if (actor != 0)
            movePc = (const u8*)&actor->moveBase;
        if (movePc == (const u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
            spCharVoiceMan->mPcVoiceCount = 0;
        }
    }
}
// If both actors are flagged and luck holds, enqueue a battle-voice event.
extern "C" void CCharVoiceMan_PlayPairVoice(cf::CVoiceActorState* self, cf::CVoiceActorState* other) {
    if (gmGameFlags & 0x00400000)
        return;
    if ((self->field_3F00 & 0x2) == 0)
        return;
    if ((other->field_3F00 & 0x4) == 0)
        return;
    cf::CCharVoiceMan* m = spCharVoiceMan;
    float f = m->mVoiceTimer / lbl_eu_80668C80;
    if (lbl_eu_80668C80 < f)
        return;
    if (ml::math::mtRand(0x64) >= 0x32)
        return;
    cf::CfObjectMove* player = (cf::CfObjectMove*)getPlayer__Q22cf13CfGameManagerFi(0);
    if (player != 0)
        player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
    if (player != 0)
        BmVoice_PlayGated((CVoiceHandle*)player, 0x899, 0x118);
}
extern "C" void CCharVoiceMan_EnqueueDamageVoice(cf::CVoiceActorState* self, int unused, void* c) {
    if (gmGameFlags & 0x00400000)
        return;
    if (self->field_3F00 & 0x2) {
        cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_HP::create((CVoiceHandle*)self, (int)c);
        cf::CCharVoiceMan* m = spCharVoiceMan;
        if (node != 0) {
            cf::CSoundNode* tail = m->mNodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->mNodeHead == 0) {
                m->mNodeHead = node;
                m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
            }
            m->mNodeTail = node;
        }
    } else {
        cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_EHP::create((CVoiceHandle*)self, (CVoiceHandle*)unused, (s32)c);
        cf::CCharVoiceMan* m = spCharVoiceMan;
        if (node != 0) {
            cf::CSoundNode* tail = m->mNodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->mNodeHead == 0) {
                m->mNodeHead = node;
                m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
            }
            m->mNodeTail = node;
        }
        cf::CSoundNode* node2 = (cf::CSoundNode*)BmVoice_PlayReqB4((CVoiceHandle*)self, (int)c);
        cf::CCharVoiceMan* m2 = spCharVoiceMan;
        if (node2 != 0) {
            cf::CSoundNode* tail = m2->mNodeTail;
            if (tail != 0)
                tail->next = node2;
            if (m2->mNodeHead == 0) {
                m2->mNodeHead = node2;
                m2->mHeadOffset = (u32)node2 - (u32)&m2->mArena[0];
            }
            m2->mNodeTail = node2;
        }
    }
}
extern "C" void CCharVoiceMan_EnqueueHurtVoice() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = CVS_THREAD_BUF::create();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueBattleEventVoice() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = CVS_THREAD_BUF::create();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueGaugeClampVoice(int a, int b, int c) {
    if (gmGameFlags & 0x00400000)
        return;
    if (b != c && c < b) {
        cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_TENSION_UP::create((CVoiceFactory*)a, b);
        cf::CCharVoiceMan* m = spCharVoiceMan;
        if (node != 0) {
            cf::CSoundNode* tail = m->mNodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->mNodeHead == 0) {
                m->mNodeHead = node;
                m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
            }
            m->mNodeTail = node;
        }
    }
}
// Append a freshly constructed sound node to the voice-event list.
extern "C" void CCharVoiceMan_EnqueuePartyGageVoice(int a, int b) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_PARTY_GAGE::create(a, b);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueCtrlActVoiceA(void* a, void* b) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_FAINT::create((CVoiceHandle*)a, (CVoiceHandle*)b);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueCtrlActVoiceB(void* a, void* b) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_DOWN::create((CVoiceHandle*)a, (CVoiceHandle*)b);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueGaugeResultVoice(void* a, void* b) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_REVIVE::create((CVoiceHandle*)a, (CVoiceHandle*)b);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueCtrlActVoiceC(void* a, void* b) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_HAGE::create((CVoiceHandle*)a, (CVoiceHandle*)b);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueOrderVoice0() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER((CVoiceHandle*)0);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueOrderVoice1() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER((CVoiceHandle*)1);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueOrderVoice2() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER((CVoiceHandle*)2);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueGaugePairVoice() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = BattleGaugeTierVoiceGate();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
// Retail takes an actor argument that flows straight into playEhpStartVoice;
// keeping r3 live across the singleton load makes MWCC color the manager r4.
extern "C" void CCharVoiceMan_FlushPendingActorVoice(void* actor) {
    cf::CCharVoiceMan* m;
    if (!(gmGameFlags & 0x00400000)) {
        m = spCharVoiceMan;
        if (m->mPendingVoice != 0) {
            cf::CSoundNode* node = playEhpStartVoice(actor);
            m = spCharVoiceMan;
            if (node != 0) {
                cf::CSoundNode* tail = m->mNodeTail;
                if (tail != 0)
                    tail->next = node;
                if (m->mNodeHead == 0) {
                    m->mNodeHead = node;
                    m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
                }
                m->mNodeTail = node;
            }
        }
    }
}
extern "C" void CCharVoiceMan_ResetAndAppend() {
    const float reset = lbl_eu_80668C68;
    cf::CCharVoiceMan* m0 = spCharVoiceMan;
    m0->mVoiceTimer = reset;
    spCharVoiceMan->mPcVoiceCount = 0;
    spCharVoiceMan->mBattleVoiceLatched = 0;
    spCharVoiceMan->mBattleActive = 0;
    cf::CSoundNode* node = requestOrderConfigVoice();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
// If the battle-manager actor ring has any entries and the flag is clear,
// flag the manager's byte 0x22A.
extern "C" void CCharVoiceMan_FlagBattleActiveVoice() {
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    cf::CVoiceBattleNode* cur = 0;
    int count = 0;
    cf::CVoiceBattleNode* sent = ((cf::CBattleListAccessor*)bm)->list0;
    for (cur = sent->next; cur != sent; cur = cur->next)
        count++;
    if (count > 0 && !(gmGameFlags & 0x00400000)) {
        spCharVoiceMan->mBattleActive = 1;
    }
}
// C++-mangled findObjectById__Fi (declared not extern "C" so MWCC mangles it).
extern void* findObjectById(BOOL id);

// Play a level-up-style character voice: resolve the actor id -> voice action,
// and enqueue a fresh HP-thread node (re-checking the presentation flag).
extern "C" void CCharVoiceMan_PlayLevelUpVoice(int id) {
    if (gmGameFlags & 0x00400000)
        return;
    void* owner = MapFx_ResolveOwner(findObjectById(id));
    if (owner == 0)
        return;
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = (cf::CSoundNode*)(uintptr_t)HpVoice_InitVoiceId((CVoiceHandle*)owner);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueDownedActorVoice() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = PlayRandomVoice();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
// Enqueue a frequency-voice node [FindThreadVoiceNode] and a level-up node
// [BmVoice_PlayA0(self)] onto the manager's event list (unless paused).
extern "C" void CCharVoiceMan_EnqueueFreqAndLevelVoice(void* self) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = FindThreadVoiceNode();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
    node = (cf::CSoundNode*)BmVoice_PlayA0((CVoiceHandle*)self);
    cf::CCharVoiceMan* m2 = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m2->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m2->mNodeHead == 0) {
            m2->mNodeHead = node;
            m2->mHeadOffset = (u32)node - (u32)&m2->mArena[0];
        }
        m2->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueuePcStateVoice(void* obj) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = TutorialList_PlayTutorialVoice();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueuePcActionVoice(void* obj) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = (cf::CSoundNode*)(uintptr_t)HpVoice_InitPlayerVoice((CVoiceHandle*)obj);
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueArtsVoice() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = ReviveVxInitVoice6E();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueItemVoice() {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = ReviveVxInitVoice3C();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" void CCharVoiceMan_EnqueueRebindVoice(void* a, void* b, void* c) {
    if (gmGameFlags & 0x00400000)
        return;
    cf::CSoundNode* node = ErrWin_PlayBattleVoice_5970();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    if (node != 0) {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
    }
}
extern "C" int CCharVoiceMan_AllocChainVoiceId() {
    cf::CSoundNode* node = (cf::CSoundNode*)cf::CVS_THREAD_CHAIN::create();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    bool appended;
    if (node == 0) {
        appended = false;
    } else {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
        appended = true;
    }
    if (appended)
        return (int)node->field_18;
    return -1;
}
extern "C" int CCharVoiceMan_AllocCommuVoiceId() {
    cf::CSoundNode* node = (cf::CSoundNode*)CVS_THREAD_SUDDEN::create();
    cf::CCharVoiceMan* m = spCharVoiceMan;
    bool appended;
    if (node == 0) {
        appended = false;
    } else {
        cf::CSoundNode* tail = m->mNodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->mNodeHead == 0) {
            m->mNodeHead = node;
            m->mHeadOffset = (u32)node - (u32)&m->mArena[0];
        }
        m->mNodeTail = node;
        appended = true;
    }
    if (appended)
        return (int)node->field_18;
    return -1;
}
// Battle/manual voice-hook: if the voice-enabled byte (mActive) is clear, bail
// with 0. Otherwise scan the voice-event list three times, each loop probing
// the +0x10 level slot of matching nodes: loop 1 bails if any probe is below
// `a`; loop 2 resets the pending hook when a probe equals `a` (b==1 bails;
// b==0 runs the +0x08 level-thread handler); loop 3 runs +0x08 on nodes whose
// probe exceeds `a`. Returns 1 on success.
extern "C" int CCharVoiceMan_DispatchById(int a, int b) {
    if (spCharVoiceMan->mActive == 0)
        return 0;

    bool match;
    cf::CSoundNode* node = spCharVoiceMan->mNodeHead;
    if (node == 0)
        goto success;
    while (node != 0) {
        match = false;
        if (__ptmf_cmpr(node, &voiceNoopPmfEntry) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match) {
            if (((VoiceNode*)node)->blank1() < a)
                return 0;
        }
        node = node->next;
    }

    node = spCharVoiceMan->mNodeHead;
    while (node != 0) {
        match = false;
        if (__ptmf_cmpr(node, &voiceNoopPmfEntry) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match) {
            if (((VoiceNode*)node)->blank1() == a) {
                if (b == 1)
                    return 0;
                if (b == 0)
                    ((VoiceNode*)node)->PlayThreadVoice();
            }
        }
        node = node->next;
    }

    node = spCharVoiceMan->mNodeHead;
    while (node != 0) {
        match = false;
        if (__ptmf_cmpr(node, &voiceNoopPmfEntry) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match) {
            if (a < ((VoiceNode*)node)->blank1())
                ((VoiceNode*)node)->PlayThreadVoice();
        }
        node = node->next;
    }
success:
    return 1;
}
// Same-TU inline search helper (MWCC_CASES 'bne next; b found' recipe): the
// loop written directly folds `cmplw; bne latch; b found' into `beq found',
// but a helper with `return node' inside the loop keeps retail's
// branch-over-branch split after inlining.
static __inline cf::CSoundNode* FindVoiceNodeById(u32 id) {
    bool match;
    cf::CSoundNode* node;
    for (node = spCharVoiceMan->mNodeHead; node != 0; node = node->next) {
        match = __ptmf_cmpr(node, &voiceNoopPmfEntry) != 0 &&
                __ptmf_test(node) != 0;
        if (match == 0)
            continue;
        if (id == node->field_18)
            return node;
    }
    // Not found: the loop-exit path nulls the node; the found jump keeps it.
    return 0;
}

// Scan the voice-event list for the node whose type matches the reference pmf
// AND whose +0x18 id matches `arg`, then run its level-thread handler (+0x08).
// Uses the inline search helper so the id-compare keeps retail's
// branch-over-branch split (`cmplw; bne latch; b found`).
extern "C" void CCharVoiceMan_PlayNodeById(u32 arg) {
    cf::CSoundNode* node;
    if (spCharVoiceMan == 0)
        return;
    node = FindVoiceNodeById(arg);
    if (node != 0)
        ((VoiceNode*)node)->PlayThreadVoice();
}
extern "C" void* CCharVoiceMan_ForwardNodeCall(void* a, void* b, void* c) {
    cf::CSoundNode* node = FindVoiceNodeById((u32)a);
    if (node == 0)
        return 0;
    return ((VoiceNode*)node)->func_802A3740(b, c);
}
// Scan for a node whose type matches the reference pmf and whose +0x18 id is
// `arg`; if found, hand it to IsThreadVoiceActive (0 if not found). The pmf
// reference is passed as a direct address-of-global expression so MWCC hoists
// `lis r31, lbl@ha` before the loop and emits `addi @l` per iteration.
// Uses the inline search helper so the id-compare keeps retail's
// branch-over-branch split (`cmplw; bne latch; b found`).
extern "C" int CCharVoiceMan_IsNodeActive(u32 arg) {
    cf::CSoundNode* node = FindVoiceNodeById(arg);
    if (node == 0)
        return 0;
    return (int)IsThreadVoiceActive(node);
}
// Factory tick: if the incoming actor is in an interact/battle state (any of
// bits 0x2/0x4/0x8 at +0x64), broadcast to every matching type of voice node.
void cf::CCharVoiceMan::FactoryEvent2(cf::CfObject* actor) {
    cf::CCharVoiceMan* self = this;
    u32 flags = ((cf::CVoiceFactoryActor*)actor)->field_64;
    if (!(flags & 0x2) && !(flags & 0x8) && !(flags & 0x4))
        return;
    bool match;
    cf::CSoundNode* node = spCharVoiceMan->mNodeHead;
    while (node != 0) {
        match = false;
        if (__ptmf_cmpr(node, (void*)&voiceNoopPmfEntry) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match)
            ((VoiceNode*)node)->DetachThreadVoice((u32)actor);
        node = node->next;
    }
}
// Check battle-state voice gates: the current player and its target actor must
// both be in the right state/level to allow a character-voice to fire. Returns
// 1 when every candidate gauge passes, else 0.
// Retail references this helper by its bare unmangled symbol (C linkage), so
// the definition keeps C linkage to emit the exact call-site reloc name.
extern "C" int CCharVoiceMan_CheckBattleGauge(cf::CCharVoiceMan* self) {
    // Hoisted so MWCC colors this call-crossing local into r31 like retail.
    cf::CVoiceActorState* actor;
    cf::CBattleManagerNodeList* list = (cf::CBattleManagerNodeList*)getInstance__Q22cf14CBattleManagerFv();
    int count = 0;
    cf::CVoiceBattleNode* cur;
    for (cur = list->sentinel->next; list->sentinel != cur; cur = cur->next)
        count++;
    if (count <= 0)
        return 0;

    cf::CVoiceActorState* p = (cf::CVoiceActorState*)getPlayer__Q22cf13CfGameManagerFi(0);
    if (p != 0)
        p = (cf::CVoiceActorState*)((u8*)p - 0x3E9C);
    // Plain copy keeps the actor local materialized before the branch.
    actor = p;
    if (actor == 0)
        return 0;
    // Advance p onto the embedded move base and dispatch its +0x4C virtual;
    // p is dead after this, so MWCC folds the add into an update-form vptr
    // load (retail lwzu r12, 0x3e9c(r3)).
    p = (cf::CVoiceActorState*)((u8*)p + 0x3E9C);
    u32 mv = ((cf::CfObjectImplPcSub3E9C*)p)->sf4C();

    cf::CVoiceActorState* other = (cf::CVoiceActorState*)findObjectById((BOOL)(u32)mv);
    if (other != 0)
        other = (cf::CVoiceActorState*)((u8*)other - 0x3E9C);
    if (other == 0)
        return 0;
    if (!(other->field_3F00 & 0x4))
        return 0;

    int diff = ((cf::CfObjectImplPc18*)actor)->v108() - 3;
    if (((cf::CfObjectImplPc18*)other)->v108() < diff)
        return 0;

    if (0.5f < ((cf::CfObjectImplPc18*)other)->v130())
        return 0;

    cf::CVoiceActorState* arr[3];
    cf::CVoiceActorState** it;
    int cnt;
    int i;
    // Same compare-then-copy shape: retail interleaves the count copy
    // between cmpwi r3,1 and bgt.
    if ((cnt = HpVoice_CollectIdle((CVoiceHandle**)arr, 3, 0)) <= 1)
        return 0;
    float limit = 0.5f;
    for (it = arr, i = 0; i < cnt; i++) {
        cf::CVoiceActorState* obj = *it;
        if (((cf::CfObjectImplPc18*)obj)->v130() < limit)
            return 0;
        ++it;
    }
    return 1;
}

// --- typified .data (layout from build/us/asm/kyoshin/cf/voice/CCharVoiceMan.s) ---
// Two vtable-shaped rows, one locator row, and one PMF entry. Slots
// carrying relocs emit zero bytes + ADDR32 relocs in the .o, matching
// retail (the gate compares bytes; reloc names are ignored once bytes
// match). Slot targets owned by other TUs are extern (UNDEF here, resolved
// at link via the global symbol map).
extern "C" {
void __dt__Q22cf12CChainEffectFv();
void __dt__Q22cf10CCharVoiceFv();
}
extern char __RTTI__Q22cf12CChainEffect[];
extern char __RTTI__Q22cf11IObjectInfo[];
// cf::CCharVoice's RTTI record lives in this unit's .sdata (retail 0x80662CA0
// = {&"cf::CCharVoice", 0}); the class belongs to the CCharVoice.cpp neighbour,
// so only its RTTI object is emitted here.
extern const char lbl_eu_805106C4[];
extern "C" const void* __RTTI__Q22cf10CCharVoice[2] = { lbl_eu_805106C4, 0 };
struct VoiceVtableRow {
    const void* typeinfo; // +0x0 sdata locator
    u32 reserved;         // +0x4 zero
    const void* dtor;     // +0x8 deleting dtor (or 0)
    const void* slotC;    // +0xC method or null
};
__declspec(section ".data") __attribute__((used, aligned(8)))
const VoiceVtableRow lbl_eu_80539890 = {
    __RTTI__Q22cf12CChainEffect, 0,
    (const void*)__dt__Q22cf12CChainEffectFv, (const void*)chainClearLinkOnObjMatch,
};
__declspec(section ".data") __attribute__((used, aligned(8)))
const VoiceVtableRow lbl_eu_805398A0 = { __RTTI__Q22cf11IObjectInfo, 0, 0, 0 };
__declspec(section ".data") __attribute__((used, aligned(8)))
const VoiceVtableRow lbl_eu_805398B0 = {
    __RTTI__Q22cf10CCharVoice, 0, (const void*)__dt__Q22cf10CCharVoiceFv, 0,
};
__declspec(section ".data") __attribute__((used, aligned(8)))
VoiceManPmf voiceNoopPmfEntry = &CCharVoice::voiceNoop;
