#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/cf/voice/CCharVoiceMan.hpp"
#include "kyoshin/UnkClass_805764CC.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// Global-scope free functions matching the retail C names: MWCC mangles a
// global no-arg getInstance() to getInstance__Fv and a global
// registerFactoryEvent(void*, cf::IFactoryEvent*) to
// registerFactoryEvent__FPvPQ22cf13IFactoryEvent, so the ctor/dtor call relocs emit
// the exact retail symbols (declaring them as UnkClass_805764CC members would
// mangle with the class prefix). getInstance is already declared
// `extern void* getInstance()` by kyoshin/cf/object/CAIAction.hpp in the
// include chain; B8804 gets its own global decl here. func_800B88E0 is the
// bare extern "C" retail symbol (0x800B91FC) used by the destructor.
void registerFactoryEvent(void* self, cf::IFactoryEvent* event);
// These were previously provided by the CAIAction.hpp include chain (dropped
// to avoid a duplicate getInstance__Q22cf14CBattleManagerFv declaration);
// plain C++ linkage mangles them to the retail names getInstance__Fv /
// func_800EA444__FPv (symbol-map mapped).
extern void* getInstance();
void* func_800EA444(void* bm);
// Voice-cue / user-owned-sound helpers (previously via the CBattleManager.hpp
// include chain; C++ linkage mangles to the retail names).
u32 func_8009CF8C(u32 resourceId);
void func_8009D018(u32 owner, u32 flag);
bool isGlobalCamFlagSet(int mask);                // isGlobalCamFlagSet__Fi

namespace cf{
    CCharVoiceMan::CCharVoiceMan(){
        *(void**)this = (void*)lbl_eu_805398CC;  // shared-pool retail vtable
        const float resetFloat = lbl_eu_80668C68;
        allocHead = 0;
        headOffset = 0;
        nodeHead = 0;
        nodeTail = 0;
        unk214 = 1;
        unk215 = 0;
        unk218 = resetFloat;
        unk21C = 0;
        unk220 = 0;
        unk221 = 0;
        fieldFlag = 0;
        autoTalkPending = 0;
        unk224 = 0;
        unk229 = 0;
        unk22A = 0;
        unk22C = 0;
        unk230 = 0;
        UnkClass_805764CC* classPtr = (UnkClass_805764CC*)getInstance();
        registerFactoryEvent(classPtr, (cf::IFactoryEvent*)this);
    }

    CCharVoiceMan::~CCharVoiceMan(){
        *(void**)this = (void*)lbl_eu_805398CC;  // vtable re-store (deleting dtor)
        UnkClass_805764CC* classPtr = (UnkClass_805764CC*)getInstance();
        func_800B88E0(classPtr, (cf::IFactoryEvent*)this);
    }
}

cf::CCharVoiceMan* lbl_eu_80664A58; // CCharVoiceMan singleton

// C++-mangled findObjectById__Fi (actor-id -> action source).
extern void* findObjectById(BOOL id);

bool func_802A1EA0() { return true; }

void func_802A1F9C() {
    cf::CCharVoiceMan* voice_man = lbl_eu_80664A58;
    if (voice_man != 0)
        voice_man->fieldFlag = 0;
}
// If the character-voice-disable flag is clear, flag the manager's byte 0x229.
void func_802A2CF0() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    lbl_eu_80664A58->unk229 = 1;
}
// Ring-buffer allocate `size` bytes from CCharVoiceMan's 0x200-byte arena
// [head allocHead / tail headOffset]; returns 0 if full or the flag/cooldown
// gates are active.
u8* func_802A34E4(int size) {
    if (lbl_eu_80663E24 & 0x10000000)
        return 0;
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (m->unk215 != 0)
        return 0;
    if ((int)m->allocHead < (int)m->headOffset) {
        // Linear region still free before the tail.
        if ((int)m->headOffset - (int)m->allocHead < size)
            return 0;
        m->allocHead = (int)m->allocHead + size;
        return (u8*)lbl_eu_80664A58 + 4 + lbl_eu_80664A58->allocHead - size;
    }
    // Wrapped: space up to the 0x200 limit, else restart at the buffer base.
    if (0x200 - (int)m->allocHead < size) {
        if ((int)m->headOffset < size)
            return 0;
        m->allocHead = size;
        return (u8*)lbl_eu_80664A58 + 4;
    }
    m->allocHead = (int)m->allocHead + size;
    return (u8*)lbl_eu_80664A58 + 4 + lbl_eu_80664A58->allocHead - size;
}
unsigned int func_802A35A0(unsigned int value) { unsigned int counter = lbl_eu_80664A5C; lbl_eu_80664A5C = counter + 1; return (counter << 16) | (value & 0xFFFF); }
int func_802A3740(void* self) { return 0; }

// Allocate the 0x234-byte CCharVoiceMan and store it as the singleton.
void func_802A14B8() {
    mtl::ALLOC_HANDLE handle = func_80496004(lbl_eu_80663E14);
    cf::CCharVoiceMan* man =
        (cf::CCharVoiceMan*)mtl::MemManager::allocate(0x234, handle);
    if (man != 0)
        man = __ct__CCharVoiceMan(man);
    lbl_eu_80664A58 = man;
    lbl_eu_80664A5C = 0;
}
// Reset the voice-event list: run each node's level-thread handler (+0x08),
// then clear the list and all resetable manager fields.
void func_802A1500() {
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    m->unk215 = 0;
    cf::CSoundNode* node = m->nodeHead;
    while (node != 0) {
        bool match = false;
        if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match)
            ((cf::CVoiceNodeIf*)node)->vf08();
        node = node->next;
    }
    m->nodeHead = 0;
    float reset = lbl_eu_80668C68;
    m->nodeTail = 0;
    m->allocHead = 0;
    m->headOffset = 0;
    lbl_eu_80664A58->unk218 = lbl_eu_80668C68;
    lbl_eu_80664A58->unk21C = 0;
    lbl_eu_80664A58->unk220 = 0;
    lbl_eu_80664A58->unk221 = 0;
    lbl_eu_80664A58->autoTalkPending = 0;
    lbl_eu_80664A58->unk224 = 0;
    lbl_eu_80664A58->unk229 = 0;
    lbl_eu_80664A58->unk22A = 0;
    lbl_eu_80664A58->unk22C = 0;
    lbl_eu_80664A58->unk230 = 0;
}
// Battle-gauge gate check (defined below); forward-declared for func_802A1610.
// Retail references this helper by its bare unmangled symbol (C linkage).
extern "C" int func_802A38C8(cf::CCharVoiceMan* self);

// Main per-frame character-voice tick. Flush the level-thread list on a
// presentation pause, then run every gate (battle count, party gauge, field
// id, per-player flags and random gauges) that can enqueue a voice node.
void func_802A1610(){
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (m->nodeHead != 0 && (lbl_eu_80663E24 & 0x10000000)) {
        // Presentation pause: run every node's level-thread handler then flush.
        m->unk215 = 0;
        cf::CSoundNode* node = m->nodeHead;
        while (node != 0) {
            bool match = false;
            if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0)
                match = true;
            if (match)
                ((cf::CVoiceNodeIf*)node)->vf08();
            node = node->next;
        }
        m->nodeHead = 0;
        m->nodeTail = 0;
        m->allocHead = 0;
        m->headOffset = 0;
    }
    m->unk229 = 0;
    if (m->unk215 == 0) {
        // Battle-participant count changed: gate a frequency voice node.
        int oldCount = m->unk21C;
        void* bm = getInstance__Q22cf14CBattleManagerFv();
        int count = 0;
        cf::CVoiceBattleNode* cur;
        cf::CVoiceBattleNode* sentinel = ((cf::CBattleManagerNodeList*)bm)->sentinel;
        for (cur = sentinel->next; cur != sentinel; cur = cur->next)
            count++;
        m->unk21C = count;
        if (count != oldCount && !(m->unk218 < lbl_eu_80668C6C) &&
            !(lbl_eu_80663E24 & 0x00400000)) {
            cf::CSoundNode* node = func_802A6820(count, oldCount);
            if (node != 0) {
                cf::CSoundNode* tail = m->nodeTail;
                if (tail != 0)
                    tail->next = node;
                if (m->nodeHead == 0) {
                    m->nodeHead = node;
                    m->headOffset = (u32)node - (u32)&m->unk4[0];
                }
                m->nodeTail = node;
            }
        }

        // Battle-participant play-gate: resolve the current player to a
        // battle object and, if its browse gate passes, voice the pending
        // action source (unk230) as a node.
        if (!(lbl_eu_80663E24 & 0x00400000)) {
            cf::CVoiceBFC68* br =
                (cf::CVoiceBFC68*)func_800BFC68__FPQ22cf12CfObjectMove(getPlayer__Q22cf13CfGameManagerFi(0));
            if (br != 0) {
                cf::CVoiceEdge* edge = (cf::CVoiceEdge*)br->field_3ED4;
                if (edge != 0 && ((cf::CVoiceEdgeIf*)edge)->fn_40(0x800) == 0) {
                    if (m->unk230 != 0) {
                        void* src = func_8016FE34(findObjectById((BOOL)m->unk230));
                        if (src != 0) {
                            cf::CSoundNode* node = func_802B0344(src);
                            if (node != 0) {
                                cf::CSoundNode* tail = m->nodeTail;
                                if (tail != 0)
                                    tail->next = node;
                                if (m->nodeHead == 0) {
                                    m->nodeHead = node;
                                    m->headOffset = (u32)node - (u32)&m->unk4[0];
                                }
                                m->nodeTail = node;
                            }
                        }
                    }
                    m->unk230 = 0;
                }
            }
        }

        // Party-gauge rising edge: enqueue a levelled voice node.
        u8 oldGauge = m->unk220;
        m->unk220 = (u8)func_802A38C8(m);
        if (oldGauge == 0 && m->unk220 != 0 && !(lbl_eu_80663E24 & 0x00400000)) {
            cf::CSoundNode* node = __ct__802AF5CC(1);
            if (node != 0) {
                cf::CSoundNode* tail = m->nodeTail;
                if (tail != 0)
                    tail->next = node;
                if (m->nodeHead == 0) {
                    m->nodeHead = node;
                    m->headOffset = (u32)node - (u32)&m->unk4[0];
                }
                m->nodeTail = node;
            }
        }

        // Interact/battle voice: probe the top gauges and enqueue on rising edge.
        u8 oldInteract = m->unk221;
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
            int n = func_802A7870(arr2, 3, 0);
            if (n <= 1) {
                iresult = 0;
            } else {
                float limit = lbl_eu_80668C70;
                int i = 0;
                cf::CVoiceActorState** it = arr2;
                while (i < n) {
                    // Fail (result 0) when any top gauge reaches the limit.
                    if (limit <= ((cf::CVoiceActorStateIf*)*it)->fn_130()) {
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
        m->unk221 = (u8)iresult;
        if (oldInteract == 0 && m->unk221 != 0 && !(lbl_eu_80663E24 & 0x00400000)) {
            cf::CSoundNode* node = __ct__802AF5CC(0);
            if (node != 0) {
                cf::CSoundNode* tail = m->nodeTail;
                if (tail != 0)
                    tail->next = node;
                if (m->nodeHead == 0) {
                    m->nodeHead = node;
                    m->headOffset = (u32)node - (u32)&m->unk4[0];
                }
                m->nodeTail = node;
            }
        }

        // Field-id window [0x109,0x115]: retail keeps two separate signed
        // cmpi branches; swapping the operand order (const OP v) blocks
        // MWCC's unsigned range-fusion (MWCC_PATTERNS "swapped-operand
        // range guards").
        int phase = getQueuedFileEventCount__Q22cf13CfGameManagerFv();
        if (0x108 >= phase)
            goto noFieldFlag;
        if (0x116 <= phase)
            goto noFieldFlag;
        u32 gaugeBit = 1 << (lbl_eu_80663E42 - 1);
        if (!(gaugeBit & 0xFE00))
            goto noFieldFlag;
        m->fieldFlag = 1;
noFieldFlag:;

        // Battle voice auto-talk: resolve the current player and fire.
        if (m->autoTalkPending != 0) {
            cf::CVoiceSrcNode* srcNode = (cf::CVoiceSrcNode*)func_802A7A54(1);
            cf::CfObjectMove* player = (cf::CfObjectMove*)getPlayer__Q22cf13CfGameManagerFi(0);
            if (player != 0)
                player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
            if (srcNode != 0 && player != 0) {
                cf::CVoiceSrcInner* inner = srcNode->field_4;
                u32 sv = *((u32*)((cf::CVoiceSrcInnerIf*)inner)->fn_30());
                // Resolution failure keeps the pending flag set to retry next
                // frame; every other path falls into the single clear store.
                if (func_80174C98(srcNode, (int*)&sv, 6) == 0)
                    goto noAutoTalkClear;
                if (!(lbl_eu_80663E24 & 0x00400000))
                    func_802AF9D0(player, 0xbb9, 0x14);
            }
            m->autoTalkPending = 0;
        }
noAutoTalkClear:;

        // No battle participants and the flag is clear: rolling gauge node.
        if (m->unk22A != 0) {
            void* bm3 = getInstance__Q22cf14CBattleManagerFv();
            cf::CVoiceBattleNode* cur3;
            int acount = 0;
            cf::CVoiceBattleNode* sentinel3 = ((cf::CBattleManagerNodeList*)bm3)->sentinel;
            for (cur3 = sentinel3->next; cur3 != sentinel3; cur3 = cur3->next)
                acount++;
            if (acount <= 0 && !(lbl_eu_80663E24 & 0x00400000)) {
                float g = m->unk218;
                float f1 = g / lbl_eu_80668C74;
                bool ge = (lbl_eu_80668C78 <= f1);
                m->unk218 = lbl_eu_80668C68;
                bool appended = false;
                if (ml::math::mtRand(0x64) < 5) {
                    cf::CSoundNode* node = __ct__802AB5F0();
                    if (node != 0) {
                        cf::CSoundNode* tail = m->nodeTail;
                        if (tail != 0)
                            tail->next = node;
                        if (m->nodeHead == 0) {
                            m->nodeHead = node;
                            m->headOffset = (u32)node - (u32)&m->unk4[0];
                        }
                        m->nodeTail = node;
                        appended = true;
                    }
                }
                if (!appended) {
                    cf::CSoundNode* node = __ct__802A4870((int)ge);
                    if (node != 0) {
                        cf::CSoundNode* tail = m->nodeTail;
                        if (tail != 0)
                            tail->next = node;
                        if (m->nodeHead == 0) {
                            m->nodeHead = node;
                            m->headOffset = (u32)node - (u32)&m->unk4[0];
                        }
                        m->nodeTail = node;
                    }
                }
            }
        }
        m->unk22A = 0;

        // Non-presentation: sweep level-thread handlers then pop leading
        // non-matching nodes off the list.
        if (m->nodeHead != 0 && !(lbl_eu_80663E24 & 0x00400000)) {
            cf::CSoundNode* cur = m->nodeHead;
            while (cur != 0) {
                func_802A3ACC(cur);
                cur = cur->next;
            }
            cf::CSoundNode* head;
            // Reload the head from the manager every iteration like retail:
            // the loop condition dereferences m->nodeHead fresh each pass.
            while ((head = m->nodeHead) != 0) {
                bool match = false;
                if (__ptmf_cmpr(head, &lbl_eu_805398C0) != 0 && __ptmf_test(head) != 0)
                    match = true;
                if (match)
                    break;
                // Retail keeps an explicit re-read + null-guard here instead
                // of reusing the loop-condition head pointer.
                if (m->nodeHead != 0) {
                    cf::CSoundNode* nxt = m->nodeHead->next;
                    if (nxt != 0) {
                        m->nodeHead = nxt;
                        m->headOffset = (u32)nxt - (u32)&m->unk4[0];
                    } else {
                        m->nodeHead = 0;
                        m->nodeTail = 0;
                        m->allocHead = 0;
                        m->headOffset = 0;
                    }
                }
            }
        }

        // Passive cell count: accumulate every tick unless the flag blocks it.
        if (!(lbl_eu_80663DA0 & 1) && !(lbl_eu_80663E24 & 0x00400000))
            m->unk218 += lbl_eu_80668C7C;
    }
}
// Battle-voice state probe: only for an interact/battle-flagged actor in
// state id 1 whose +0x3F60 chain selects a voice id in [0x10, 0x17]. When the
// source is dead (or luck holds on a 15/100 roll) flag byte 0x223, then always
// poke the sub-id handler with (voicePtr + 0x3508, 1).
void func_802A1C68(cf::CVoiceActorState* self) {
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
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
        if (func_8009CF8C((int)t) == 0 || ml::math::mtRand(0x64) < 0xF)
            m->autoTalkPending = 1;
        func_8009D018((u32)t, 1);
    }
}
// Battle-slot voice hook: if the battle slot manager reports a slot whose id
// matches b's voice id, copy a's pending id into unk22C.
void func_802A1D04(cf::CVoiceActorInfo* a, cf::CVoiceActorInfo* b) {
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    cf::CVoiceBtlSlot* bs = (cf::CVoiceBtlSlot*)func_800EA444(getInstance__Q22cf14CBattleManagerFv());
    if (bs != 0) {
        getInstance__Q22cf13CfGameManagerFv();
        if (isGlobalCamFlagSet(0x04000000) == 0) {
            if (a != 0 && b != 0) {
                u32 af = a->field_3F10;
                u32 bf = b->field_3F10;
                if (bf == bs->id0 || bf == bs->id1)
                    m->unk22C = af;
            }
        }
    }
}
void func_802A1DA8() {
    // Virtual-delete the singleton manager (vtable+8 dtor slot, deleting
    // flag r4=1); delete's own null-check reuses the outer compare, giving
    // retail's double beq. The manager field store is inside the guard.
    if (lbl_eu_80664A58 != 0) {
        delete lbl_eu_80664A58;
        lbl_eu_80664A58 = 0;
    }
}
// Set the manager's voice-enable byte, then run every matching node's +0x14
// voice probe; nodes whose probe returns nonzero are handed to func_802A3E74.
void func_802A1DF0(u8 flag) {
    lbl_eu_80664A58->unk215 = flag;
    cf::CSoundNode* node = lbl_eu_80664A58->nodeHead;
    while (node != 0) {
        bool match = false;
        if (__ptmf_cmpr(node, (void*)&lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match && ((cf::CVoiceNodeIf*)node)->vf14() != 0)
            func_802A3E74(node);
        node = node->next;
    }
}
// Character-voice play gate: if the actor is interact/battle-flagged and its
// state checks pass, resolve the target's voice action and fire a battle/lvl
// voice node.
// Retail references this helper by its bare unmangled symbol (C linkage).
extern "C" int func_802A1EA8(cf::CVoiceActorState* self);
int func_802A1EA8(cf::CVoiceActorState* self) {
    if (!(self->field_3F00 & 0x2))
        return 0;
    if (((cf::CVoiceActorStateIf*)self)->fn_2BC() != 0)
        return 0;
    if (self->field_3F28 != 1)
        return 0;
    // State already registers either slot 0xCF or 0xD0; otherwise bail.
    if (func_80148778(&self->unk4[4], 0xcf) == 0 && func_80148778(&self->unk4[4], 0xd0) == 0)
        return 0;
    cf::CVoiceMoveIf* move = (cf::CVoiceMoveIf*)((u8*)self + 0x3E9C);
    void* moveRes = move->fn_4C();
    void* r = func_8016FE34(findObjectById((BOOL)(u32)moveRes));
    if (r == 0)
        return 0;
    if (((cf::CVoiceActorState*)r)->field_3F28 != 0x10c)
        return 0;
    func_802AF9D0(self, 0xfa5, 0x14);
    return 1;
}
// Register a freshly-created sound node, then clear byte 0x22C.
void func_802A1FB4() {
    cf::CSoundNode* node = func_802A9604();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
    lbl_eu_80664A58->unk22C = 0;
}
// Append a freshly-created sound node to the manager's voice-event list.
void func_802A201C() {
    cf::CSoundNode* node = __ct__802A96C0();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
// Resolve a pending voice-action id (from the manager's 0x22C holder) and
// enqueue an action node plus a clear node, if the arguments are non-null.
void func_802A2078(void* a, void* b, void* c) {
    if (b == 0) {
        u32 pending = lbl_eu_80664A58->unk22C;
        if (pending != 0)
            b = (void*)func_8016FE34((void*)findObjectById((BOOL)pending));
    }
    if (b != 0) {
        cf::CSoundNode* node = __ct__802A92D8(b, a);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->nodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->nodeHead == 0) {
                m->nodeHead = node;
                m->headOffset = (u32)node - (u32)&m->unk4[0];
            }
            m->nodeTail = node;
        }
    }
    if (c != 0) {
        cf::CSoundNode* node = func_802AF56C(c);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->nodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->nodeHead == 0) {
                m->nodeHead = node;
                m->headOffset = (u32)node - (u32)&m->unk4[0];
            }
            m->nodeTail = node;
        }
    }
}
// Player-voice enqueue: if the actor's battle move-base resolves to a live
// battle actor, construct a battle-begin node and append it to the list.
void func_802A216C(cf::CVoiceActorState* self) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CVoiceMoveIf* move = (cf::CVoiceMoveIf*)((u8*)self + 0x3E9C);
    void* moveRes = move->fn_4C();
    void* actor = findObjectById((BOOL)(u32)moveRes);
    if (actor != 0)
        actor = (void*)((u8*)actor - 0x3E9C);
    if (actor == 0)
        return;
    cf::CSoundNode* node = __ct__802AFA80(self, actor);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
// If the passed actor is active, teed a u32 from +0x3F10 into unk230.
void func_802A2210(cf::CVoiceActorInfo* self) {
    if (func_802B03A4(self) != 0) {
        lbl_eu_80664A58->unk230 = self->field_3F10;
    }
}
// Enqueue a break-freq node [func_802A5A14(a,c)] and a party-gauge node
// [func_802AF43C(a,b,c)] onto the listener list (unless paused).
void func_802A2250(void* a, void* b, void* c) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A5A14(a, c);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
    node = func_802AF43C(a, b, c);
    cf::CCharVoiceMan* m2 = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m2->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m2->nodeHead == 0) {
            m2->nodeHead = node;
            m2->headOffset = (u32)node - (u32)&m2->unk4[0];
        }
        m2->nodeTail = node;
    }
}
// Battle/interact voice trigger. If the actor's battle flag is set, resolve a
// battle-voice node (gated on battle count + actor flag); otherwise enqueue a
// plain voice node.
void func_802A232C(cf::CVoiceActorState* self) {
    if (self->field_3F00 & 0x2) {
        if (lbl_eu_80663E24 & 0x00400000)
            return;
        int useBattle = 0;
        if (((cf::CBattleCountAccessor*)getInstance__Q22cf14CBattleManagerFv())->field_194 < 0x64 &&
            (self->field_3F08 & 0x10000))
            useBattle = 1;
        cf::CSoundNode* node = func_802A5B04(self, useBattle);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->nodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->nodeHead == 0) {
                m->nodeHead = node;
                m->headOffset = (u32)node - (u32)&m->unk4[0];
            }
            m->nodeTail = node;
        }
    } else {
        cf::CSoundNode* node = func_802AF49C(self);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->nodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->nodeHead == 0) {
                m->nodeHead = node;
                m->headOffset = (u32)node - (u32)&m->unk4[0];
            }
            m->nodeTail = node;
        }
    }
}
// Construct a FAINT voice node and enqueue it; return its event id (or -1).
int func_802A2424(void) {
    if (lbl_eu_80663E24 & 0x00400000)
        return -1;
    cf::CSoundNode* node = func_802A6DF4();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    bool appended;
    if (node == 0) {
        appended = false;
    } else {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
        appended = true;
    }
    if (appended)
        return (int)node->field_18;
    return -1;
}
// Battle/interact pending-voice hook: on the first pass run the state probe
// (func_802A1EA8); if it fires, latch byte 0x228 so later passes fall through
// to the plain battle-voice node enqueue instead.
void func_802A24B4(cf::CVoiceActorState* self) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (lbl_eu_80664A58->unk228 == 0) {
        if (func_802A1EA8(self) != 0) {
            lbl_eu_80664A58->unk228 = 1;
            return;
        }
    }
    cf::CSoundNode* node = func_802A8628(self);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
// If the passed actor's move-base is the current player, raise the manager's
// voice count; on the 3rd+ tick roll a 25% chance to play the 0x89c battle
// voice with a 0x118 flavour.
void func_802A2558(cf::CVoiceActorBase* actor) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    const u8* movePc = (const u8*)actor;
    if (actor != 0)
        movePc = (const u8*)&actor->moveBase;
    if (movePc == (const u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        m->unk224++;
        if ((int)m->unk224 >= 3) {
            if (ml::math::mtRand(0x64) < 0x19)
                func_802AF9D0(actor, 0x89c, 0x118);
        }
    }
}
// If the passed actor's move-base is the current player, clear voice count 0x224.
void func_802A25EC(cf::CVoiceActorBase* actor) {
    if (!(lbl_eu_80663E24 & 0x00400000)) {
        const u8* movePc = (const u8*)actor;
        if (actor != 0)
            movePc = (const u8*)&actor->moveBase;
        if (movePc == (const u8*)getPlayer__Q22cf13CfGameManagerFi(0)) {
            lbl_eu_80664A58->unk224 = 0;
        }
    }
}
// If both actors are flagged and luck holds, enqueue a battle-voice event.
void func_802A2648(cf::CVoiceActorState* self, cf::CVoiceActorState* other) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if ((self->field_3F00 & 0x2) == 0)
        return;
    if ((other->field_3F00 & 0x4) == 0)
        return;
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    float f = m->unk218 / lbl_eu_80668C80;
    if (lbl_eu_80668C80 < f)
        return;
    if (ml::math::mtRand(0x64) >= 0x32)
        return;
    cf::CfObjectMove* player = (cf::CfObjectMove*)getPlayer__Q22cf13CfGameManagerFi(0);
    if (player != 0)
        player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
    if (player != 0)
        func_802AF9D0(player, 0x899, 0x118);
}
void func_802A26D8(cf::CVoiceActorState* self, int unused, void* c) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (self->field_3F00 & 0x2) {
        cf::CSoundNode* node = __ct__802A7254(self, c);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->nodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->nodeHead == 0) {
                m->nodeHead = node;
                m->headOffset = (u32)node - (u32)&m->unk4[0];
            }
            m->nodeTail = node;
        }
    } else {
        cf::CSoundNode* node = __ct__802A5ED4();
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->nodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->nodeHead == 0) {
                m->nodeHead = node;
                m->headOffset = (u32)node - (u32)&m->unk4[0];
            }
            m->nodeTail = node;
        }
        cf::CSoundNode* node2 = func_802AF4FC(self, c);
        cf::CCharVoiceMan* m2 = lbl_eu_80664A58;
        if (node2 != 0) {
            cf::CSoundNode* tail = m2->nodeTail;
            if (tail != 0)
                tail->next = node2;
            if (m2->nodeHead == 0) {
                m2->nodeHead = node2;
                m2->headOffset = (u32)node2 - (u32)&m2->unk4[0];
            }
            m2->nodeTail = node2;
        }
    }
}void func_802A27F4() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A4E48();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A285C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A4E48();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A28C4(int a, int b, int c) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (b != c && c < b) {
        cf::CSoundNode* node = __ct__802A8DE8();
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->nodeTail;
            if (tail != 0)
                tail->next = node;
            if (m->nodeHead == 0) {
                m->nodeHead = node;
                m->headOffset = (u32)node - (u32)&m->unk4[0];
            }
            m->nodeTail = node;
        }
    }
}
// Append a freshly constructed sound node to the voice-event list.
void func_802A293C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_PARTY_GAGE();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A29A4() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A6AA8();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A2A0C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A5B88();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A2A74() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A86CC();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A2ADC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A6E84();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A2B44() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(0);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A2BB0() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(1);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A2C1C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(2);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A2C88() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802B9064();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
// Retail takes an actor argument that flows straight into func_802A6958;
// keeping r3 live across the singleton load makes MWCC color the manager r4.
void func_802A2D0C(void* actor) {
    cf::CCharVoiceMan* m;
    if (!(lbl_eu_80663E24 & 0x00400000)) {
        m = lbl_eu_80664A58;
        if (m->unk229 != 0) {
            cf::CSoundNode* node = func_802A6958(actor);
            m = lbl_eu_80664A58;
            if (node != 0) {
                cf::CSoundNode* tail = m->nodeTail;
                if (tail != 0)
                    tail->next = node;
                if (m->nodeHead == 0) {
                    m->nodeHead = node;
                    m->headOffset = (u32)node - (u32)&m->unk4[0];
                }
                m->nodeTail = node;
            }
        }
    }
}
void func_802A2D84() {
    const float reset = lbl_eu_80668C68;
    cf::CCharVoiceMan* m0 = lbl_eu_80664A58;
    m0->unk218 = reset;
    lbl_eu_80664A58->unk224 = 0;
    lbl_eu_80664A58->unk228 = 0;
    lbl_eu_80664A58->unk22A = 0;
    cf::CSoundNode* node = func_802B8D4C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
// If the battle-manager actor ring has any entries and the flag is clear,
// flag the manager's byte 0x22A.
void func_802A2E08() {
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    cf::CVoiceBattleNode* cur = 0;
    int count = 0;
    cf::CVoiceBattleNode* sent = ((cf::CBattleListAccessor*)bm)->list0;
    for (cur = sent->next; cur != sent; cur = cur->next)
        count++;
    if (count > 0 && !(lbl_eu_80663E24 & 0x00400000)) {
        lbl_eu_80664A58->unk22A = 1;
    }
}
// C++-mangled findObjectById__Fi (declared not extern "C" so MWCC mangles it).
extern void* findObjectById(BOOL id);

// Play a level-up-style character voice: resolve the actor id -> voice action,
// and enqueue a fresh HP-thread node (re-checking the presentation flag).
void func_802A2E68(int id) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (func_8016FE34(findObjectById(id)) == 0)
        return;
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A7744();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A2EEC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A4798();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
// Enqueue a frequency-voice node [func_802A3EF0] and a level-up node
// [func_802AF3DC(self)] onto the manager's event list (unless paused).
void func_802A2F54(void* self) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A3EF0();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
    node = func_802AF3DC(self);
    cf::CCharVoiceMan* m2 = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m2->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m2->nodeHead == 0) {
            m2->nodeHead = node;
            m2->headOffset = (u32)node - (u32)&m2->unk4[0];
        }
        m2->nodeTail = node;
    }
}
void func_802A300C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802AE38C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A3074() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A7674();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A30DC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A8AC8();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A3144() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A8B6C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
void func_802A31AC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802B5970();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
    }
}
int func_802A3214() {
    cf::CSoundNode* node = __ct__802A5830();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    bool appended;
    if (node == 0) {
        appended = false;
    } else {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
        appended = true;
    }
    if (appended)
        return (int)node->field_18;
    return -1;
}
int func_802A3290() {
    cf::CSoundNode* node = __ct__802A8C04();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    bool appended;
    if (node == 0) {
        appended = false;
    } else {
        cf::CSoundNode* tail = m->nodeTail;
        if (tail != 0)
            tail->next = node;
        if (m->nodeHead == 0) {
            m->nodeHead = node;
            m->headOffset = (u32)node - (u32)&m->unk4[0];
        }
        m->nodeTail = node;
        appended = true;
    }
    if (appended)
        return (int)node->field_18;
    return -1;
}
// Battle/manual voice-hook: if the voice-enabled byte (unk214) is clear, bail
// with 0. Otherwise scan the voice-event list three times, each loop probing
// the +0x10 level slot of matching nodes: loop 1 bails if any probe is below
// `a`; loop 2 resets the pending hook when a probe equals `a` (b==1 bails;
// b==0 runs the +0x08 level-thread handler); loop 3 runs +0x08 on nodes whose
// probe exceeds `a`. Returns 1 on success.
int func_802A330C(int a, int b) {
    if (lbl_eu_80664A58->unk214 == 0)
        return 0;

    cf::CSoundNode* node = lbl_eu_80664A58->nodeHead;
    if (node == 0)
        goto success;
    while (node != 0) {
        bool match = false;
        if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match) {
            if (((cf::CVoiceNodeIf*)node)->vf10() < a)
                return 0;
        }
        node = node->next;
    }

    node = lbl_eu_80664A58->nodeHead;
    while (node != 0) {
        bool match = false;
        if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match) {
            if (((cf::CVoiceNodeIf*)node)->vf10() == a) {
                if (b == 1)
                    return 0;
                if (b == 0)
                    ((cf::CVoiceNodeIf*)node)->vf08();
            }
        }
        node = node->next;
    }

    node = lbl_eu_80664A58->nodeHead;
    while (node != 0) {
        bool match = false;
        if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match) {
            if (a < ((cf::CVoiceNodeIf*)node)->vf10())
                ((cf::CVoiceNodeIf*)node)->vf08();
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
    for (cf::CSoundNode* node = lbl_eu_80664A58->nodeHead; node != 0;
         node = node->next) {
        int match = __ptmf_cmpr(node, &lbl_eu_805398C0) != 0 &&
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
void func_802A35B8(u32 arg) {
    cf::CSoundNode* node;
    if (lbl_eu_80664A58 == 0)
        return;
    node = FindVoiceNodeById(arg);
    if (node != 0)
        ((cf::CVoiceNodeIf*)node)->vf08();
}
void* func_802A3680(void* a, void* b, void* c) {
    cf::CSoundNode* node = FindVoiceNodeById((u32)a);
    if (node == 0)
        return 0;
    return ((cf::CVoiceNodeIf*)node)->vf18(b, c);
}
// Scan for a node whose type matches the reference pmf and whose +0x18 id is
// `arg`; if found, hand it to func_802A3E88 (0 if not found). The pmf
// reference is passed as a direct address-of-global expression so MWCC hoists
// `lis r31, lbl@ha` before the loop and emits `addi @l` per iteration.
// Uses the inline search helper so the id-compare keeps retail's
// branch-over-branch split (`cmplw; bne latch; b found`).
int func_802A3748(u32 arg) {
    cf::CSoundNode* node = FindVoiceNodeById(arg);
    if (node == 0)
        return 0;
    return (int)func_802A3E88(node);
}
// Factory tick: if the incoming actor is in an interact/battle state (any of
// bits 0x2/0x4/0x8 at +0x64), broadcast to every matching type of voice node.
void CCharVoiceMan_FactoryEvent2(void* self, void* actor) {
    u32 flags = ((cf::CVoiceFactoryActor*)actor)->field_64;
    if (!(flags & 0x2) && !(flags & 0x8) && !(flags & 0x4))
        return;
    cf::CSoundNode* node = lbl_eu_80664A58->nodeHead;
    while (node != 0) {
        bool match = false;
        if (__ptmf_cmpr(node, (void*)&lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match)
            ((cf::CVoiceNodeIf*)node)->vf0C(actor);
        node = node->next;
    }
}
// Check battle-state voice gates: the current player and its target actor must
// both be in the right state/level to allow a character-voice to fire. Returns
// 1 when every candidate gauge passes, else 0.
// Retail references this helper by its bare unmangled symbol (C linkage), so
// the definition keeps C linkage to emit the exact call-site reloc name.
extern "C" int func_802A38C8(cf::CCharVoiceMan* self) {
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
    void* mv = ((cf::CVoiceMoveIf*)p)->fn_4C();

    cf::CVoiceActorState* other = (cf::CVoiceActorState*)findObjectById((BOOL)(u32)mv);
    if (other != 0)
        other = (cf::CVoiceActorState*)((u8*)other - 0x3E9C);
    if (other == 0)
        return 0;
    if (!(other->field_3F00 & 0x4))
        return 0;

    int diff = ((cf::CVoiceActorStateIf*)actor)->fn_108() - 3;
    if (((cf::CVoiceActorStateIf*)other)->fn_108() < diff)
        return 0;

    if (lbl_eu_80668C70 < ((cf::CVoiceActorStateIf*)other)->fn_130())
        return 0;

    cf::CVoiceActorState* arr[3];
    cf::CVoiceActorState** it;
    int cnt;
    int i;
    // Same compare-then-copy shape: retail interleaves the count copy
    // between cmpwi r3,1 and bgt.
    if ((cnt = func_802A7870(arr, 3, 0)) <= 1)
        return 0;
    float limit = lbl_eu_80668C70;
    for (it = arr, i = 0; i < cnt; i++) {
        cf::CVoiceActorState* obj = *it;
        if (((cf::CVoiceActorStateIf*)obj)->fn_130() < limit)
            return 0;
        ++it;
    }
    return 1;
}
