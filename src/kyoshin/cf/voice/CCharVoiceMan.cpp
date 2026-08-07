#include "kyoshin/cf/voice/CCharVoiceMan.hpp"
#include "kyoshin/UnkClass_805764CC.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/MemManager.hpp"

// CCharVoiceMan ctor symbol (placement construction of a heaped 0x234 buffer);
// kept external so the call site emits a real call rather than being inlined.
extern "C" cf::CCharVoiceMan* __ct__CCharVoiceMan(cf::CCharVoiceMan* self);
// Retail @sda21 reset float for unk218 (declared early so the ctor can use it).
extern "C" float lbl_eu_80668C68;

namespace cf{
    CCharVoiceMan::CCharVoiceMan(){
        float resetFloat = lbl_eu_80668C68;
        unk204 = 0;
        unk208 = 0;
        unk20C = 0;
        unk210 = 0;
        unk214 = 1;
        unk215 = 0;
        unk218 = resetFloat;
        unk21C = 0;
        unk220 = 0;
        unk221 = 0;
        unk222 = 0;
        unk223 = 0;
        unk224 = 0;
        unk229 = 0;
        unk22A = 0;
        unk22C = 0;
        unk230 = 0;
        UnkClass_805764CC* classPtr = UnkClass_805764CC::func_800B07E8();
        classPtr->func_800B8804(this);
    }

    CCharVoiceMan::~CCharVoiceMan(){
        UnkClass_805764CC* classPtr = UnkClass_805764CC::func_800B07E8();
        classPtr->func_800B88E0(this);
    }
}

// ── retail @sda21 globals ────────────────────────────────────────────────────
extern "C" u32 lbl_eu_80663E24;    // presentation/event bitfield (bit 0x00400000)
extern "C" void* lbl_eu_80663E14;  // scene / allocation-source handle
extern "C" u32 lbl_eu_80664A5C;    // global character-voice counter
extern "C" float lbl_eu_80668C68;  // single-precision reset value for unk218
cf::CCharVoiceMan* lbl_eu_80664A58; // CCharVoiceMan singleton

// Scene-model helper returning an allocation-region handle (mtl MemManager).
extern "C" mtl::ALLOC_HANDLE func_80496004(void* src);

// Sibling voice-module free functions (cvsys TUs) — un-mangled retail names.
extern "C" int func_802B03A4(void* self);
extern "C" cf::CSoundNode* func_802A9604();
extern "C" cf::CSoundNode* __ct__802A4E48();
extern "C" cf::CSoundNode* __ct__802A96C0();
// Additional retail node constructors / helpers used by the enqueue functions.
extern "C" cf::CSoundNode* func_802A8B6C();
extern "C" cf::CSoundNode* func_802B5970();
extern "C" cf::CSoundNode* __ct__CVS_THREAD_ORDER(int order);
extern "C" cf::CSoundNode* __ct__802A8DE8();
extern "C" cf::CSoundNode* func_802A6958();
extern "C" cf::CSoundNode* __ct__802A5830();
extern "C" cf::CSoundNode* __ct__802A8C04();
extern "C" cf::CSoundNode* func_802B8D4C();
// Node factory helpers for the frequency / level-up voice enqueue functions.
extern "C" cf::CSoundNode* func_802A3EF0();
extern "C" cf::CSoundNode* func_802AF3DC(void* self);
extern "C" cf::CSoundNode* func_802A5A14(void* a, void* c);
extern "C" cf::CSoundNode* __ct__802A7254(void* self, void* c);
extern "C" cf::CSoundNode* __ct__802A5ED4();
extern "C" cf::CSoundNode* func_802AF4FC(void* self, void* c);
extern "C" cf::CSoundNode* func_802AF43C(void* a, void* b, void* c);
extern "C" cf::CSoundNode* func_802A5B04(void* self, int flag);
extern "C" cf::CSoundNode* func_802AF49C(void* self);
// Per-object state flag lookup (tests a slot at self+0x8 against an id).
extern "C" int func_80148778(void* self, int id);
// MWCC member-function-pointer runtime helpers (compare / probe).
extern "C" int __ptmf_cmpr(void* a, void* b);
extern "C" long __ptmf_test(void* ptmf);
// Reference member-function-pointer the voice nodes are matched against.
extern "C" void* lbl_eu_805398C0;
// Resolve a voice-action object from a voice source id (C-linkage).
extern "C" void* func_8016FE34(void* src);
// C++-mangled func_800B708C__Fi (actor-id -> action source).
extern void* func_800B708C(BOOL id);
// Play a specific character-voice sound (cvsys sibling; C-linkage retail name).
extern "C" void func_802AF9D0(void* r3, int voiceId, int r5);
extern "C" cf::CSoundNode* func_802AF56C(void* c);
extern "C" int func_802A7870(void* arr, int capacity, int unused);
extern "C" cf::CSoundNode* __ct__802A92D8(void* b, void* a);
extern "C" void* func_802A3E88(void* node);

bool func_802A1EA0() { return true; }

void func_802A1F9C() {
    cf::CCharVoiceMan* voice_man = lbl_eu_80664A58;
    if (voice_man != 0)
        voice_man->unk222 = 0;
}
// If the character-voice-disable flag is clear, flag the manager's byte 0x229.
void func_802A2CF0() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    lbl_eu_80664A58->unk229 = 1;
}
// Ring-buffer allocate `size` bytes from CCharVoiceMan's 0x200-byte arena
// [head ra_204 / tail ra_208]; returns 0 if full or the flag/cooldown gates are
// active.
u8* func_802A34E4(int size) {
    if (lbl_eu_80663E24 & 0x10000000)
        return 0;
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (m->unk215 != 0)
        return 0;
    int tail = (int)m->unk208;
    int head = (int)m->unk204;
    if (head < tail) {
        // Linear region still free before the tail.
        if (tail - head < size)
            return 0;
        m->unk204 = head + size;
        return (u8*)lbl_eu_80664A58 + 4 + lbl_eu_80664A58->unk204 - size;
    }
    // Wrapped: space up to the 0x200 limit, else restart at the buffer base.
    if (0x200 - head < size) {
        if (tail < size)
            return 0;
        m->unk204 = size;
        return (u8*)lbl_eu_80664A58 + 4;
    }
    m->unk204 = head + size;
    return (u8*)lbl_eu_80664A58 + 4 + lbl_eu_80664A58->unk204 - size;
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
    cf::CSoundNode* node = m->unk20C;
    void* const ref = &lbl_eu_805398C0;
    while (node != 0) {
        bool match = false;
        if (__ptmf_cmpr(node, ref) != 0 && __ptmf_test(node) != 0)
            match = true;
        if (match)
            node->field_1C->fn_08(node);
        node = node->next;
    }
    m->unk20C = 0;
    float reset = lbl_eu_80668C68;
    m->unk210 = 0;
    m->unk204 = 0;
    m->unk208 = 0;
    lbl_eu_80664A58->unk218 = lbl_eu_80668C68;
    lbl_eu_80664A58->unk21C = 0;
    lbl_eu_80664A58->unk220 = 0;
    lbl_eu_80664A58->unk221 = 0;
    lbl_eu_80664A58->unk223 = 0;
    lbl_eu_80664A58->unk224 = 0;
    lbl_eu_80664A58->unk229 = 0;
    lbl_eu_80664A58->unk22A = 0;
    lbl_eu_80664A58->unk22C = 0;
    lbl_eu_80664A58->unk230 = 0;
}
// Create a battle-voice source node from an old battle-count (cvsys sibling).
extern "C" cf::CSoundNode* func_802A6820(int oldCount);
// Pure voice nodes created with an int argument (1=levelled, 0=normal).
extern "C" cf::CSoundNode* __ct__802AF5CC(int a);
extern "C" cf::CSoundNode* __ct__802AB5F0();
extern "C" cf::CSoundNode* __ct__802A4870(int a);
// Battle-participant gauge-list probe (fills an array, returns count).
extern "C" int func_802A7870(void* arr, int capacity, int unused);
// Convert a CfObjectMove to a battle-participant object.
extern "C" void* func_800BFC68(cf::CfObjectMove* self);
// Create a battle-voice node from a voice-action source.
extern "C" cf::CSoundNode* func_802B0344(void* src);
// Current battle/field phase id.
extern "C" int func_800822F4(void);
// Create a battle-voice trigger node.
extern "C" void* func_802A7A54(int a);
// Run one node's level-thread handler (list sweep).
extern "C" void func_802A3ACC(void* node);
// Check a just-resolved voice action against a source handle.
extern "C" int func_80174C98(void* r3, u32* r4, int r5);
// Byte/word/shift @sda21 globals used by func_802A1610.
extern "C" u8  lbl_eu_80663DA0;
extern "C" u16 lbl_eu_80663E42;
extern "C" float lbl_eu_80668C6C;
extern "C" float lbl_eu_80668C70;
extern "C" float lbl_eu_80668C74;
extern "C" float lbl_eu_80668C78;
extern "C" float lbl_eu_80668C7C;

// Party-gauge gate check (defined below); forward-declared for func_802A1610.
int func_802A38C8(cf::CCharVoiceMan* self);

// Main per-frame character-voice tick. Flush the level-thread list on a
// presentation pause, then run every gate (battle count, party gauge, field
// id, per-player flags and random gauges) that can enqueue a voice node.
void func_802A1610(){
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (m->unk20C != 0 && (lbl_eu_80663E24 & 0x10000000)) {
        // Presentation pause: run every node's level-thread handler then flush.
        m->unk215 = 0;
        cf::CSoundNode* node = m->unk20C;
        void* const ref = &lbl_eu_805398C0;
        while (node != 0) {
            bool match = false;
            if (__ptmf_cmpr(node, ref) != 0 && __ptmf_test(node) != 0)
                match = true;
            if (match)
                node->field_1C->fn_08(node);
            node = node->next;
        }
        m->unk20C = 0;
        m->unk210 = 0;
        m->unk204 = 0;
        m->unk208 = 0;
    }
    m->unk229 = 0;
    if (m->unk215 == 0) {
        // Battle-participant count changed: gate a frequency voice node.
        int oldCount = m->unk21C;
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        cf::CVoiceBattleNode* sentinel = ((cf::CBattleManagerNodeList*)bm)->sentinel;
        int count = 0;
        for (cf::CVoiceBattleNode* cur = sentinel->next; cur != sentinel; cur = cur->next)
            count++;
        m->unk21C = count;
        if (count != oldCount && m->unk218 >= lbl_eu_80668C6C &&
            !(lbl_eu_80663E24 & 0x00400000)) {
            cf::CSoundNode* node = func_802A6820(oldCount);
            if (node != 0) {
                cf::CSoundNode* tail = m->unk210;
                if (tail != 0)
                    tail->next = node;
                if (m->unk20C == 0) {
                    m->unk20C = node;
                    m->unk208 = (u32)node - (u32)&m->unk4[0];
                }
                m->unk210 = node;
            }
        }

        // Battle-participant play-gate: resolve the current player to a
        // battle object and, if its browse gate passes, voice the pending
        // action source (unk230) as a node.
        if (!(lbl_eu_80663E24 & 0x00400000)) {
            cf::CVoiceBFC68* br =
                (cf::CVoiceBFC68*)func_800BFC68(cf::CfGameManager::getPlayer(0));
            if (br != 0) {
                cf::CVoiceEdge* edge = (cf::CVoiceEdge*)br->field_3ED4;
                if (edge != 0 && edge->vtable->fn_40(edge, 0x800) == 0) {
                    if (m->unk230 != 0) {
                        void* src = func_8016FE34(func_800B708C((BOOL)m->unk230));
                        if (src != 0) {
                            cf::CSoundNode* node = func_802B0344(src);
                            if (node != 0) {
                                cf::CSoundNode* tail = m->unk210;
                                if (tail != 0)
                                    tail->next = node;
                                if (m->unk20C == 0) {
                                    m->unk20C = node;
                                    m->unk208 = (u32)node - (u32)&m->unk4[0];
                                }
                                m->unk210 = node;
                            }
                        }
                    }
                    m->unk230 = 0;
                }
            }
        }

        // Party-gauge rising edge: enqueue a levelled voice node.
        u8 oldGauge = m->unk220;
        u8 newGauge = (u8)func_802A38C8(m);
        m->unk220 = newGauge;
        if (oldGauge == 0 && newGauge != 0 && !(lbl_eu_80663E24 & 0x00400000)) {
            cf::CSoundNode* node = __ct__802AF5CC(1);
            if (node != 0) {
                cf::CSoundNode* tail = m->unk210;
                if (tail != 0)
                    tail->next = node;
                if (m->unk20C == 0) {
                    m->unk20C = node;
                    m->unk208 = (u32)node - (u32)&m->unk4[0];
                }
                m->unk210 = node;
            }
        }

        // Interact/battle voice: probe the top gauges and enqueue on rising edge.
        u8 oldInteract = m->unk221;
        cf::CBattleManager* bm2 = cf::CBattleManager::getInstance();
        sentinel = ((cf::CBattleManagerNodeList*)bm2)->sentinel;
        int icount = 0;
        for (cf::CVoiceBattleNode* cur = sentinel->next; cur != sentinel; cur = cur->next)
            icount++;
        // iresult is only assigned on call-free paths right before the store,
        // so it stays in a caller-saved register instead of forcing r26.
        int iresult;
        if (icount > 0) {
            cf::CVoiceActorState* arr[3];
            int n = func_802A7870(arr, 3, 0);
            if (n > 1) {
                float limit = lbl_eu_80668C70;
                int i = 0;
                for (; i < n; i++) {
                    if (arr[i]->vtable->fn_130(arr[i]) >= limit)
                        break;
                }
                iresult = (i == n) ? 1 : 0;
            } else {
                iresult = 0;
            }
        } else {
            iresult = 0;
        }
        m->unk221 = (u8)iresult;
        if (oldInteract == 0 && iresult != 0 && !(lbl_eu_80663E24 & 0x00400000)) {
            cf::CSoundNode* node = __ct__802AF5CC(0);
            if (node != 0) {
                cf::CSoundNode* tail = m->unk210;
                if (tail != 0)
                    tail->next = node;
                if (m->unk20C == 0) {
                    m->unk20C = node;
                    m->unk208 = (u32)node - (u32)&m->unk4[0];
                }
                m->unk210 = node;
            }
        }

        // Field-id window [0x108, 0x116): flag byte 0x222 on a mask hit.
        int phase = func_800822F4();
        if (phase > 0x108 && phase < 0x116) {
            u32 mask = 1u << (lbl_eu_80663E42 - 1);
            if (mask & 0x7F)
                m->unk222 = 1;
        }

        // Battle voice auto-talk: resolve the current player and fire.
        if (m->unk223 != 0) {
            cf::CVoiceSrcNode* srcNode = (cf::CVoiceSrcNode*)func_802A7A54(1);
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
            if (player != 0)
                player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
            if (srcNode != 0 && player != 0) {
                cf::CVoiceSrcInner* inner = srcNode->field_4;
                void* ret = inner->vtable->fn_30(inner);
                u32 sv = *(u32*)ret;
                if (func_80174C98(srcNode, &sv, 6) != 0) {
                    if (!(lbl_eu_80663E24 & 0x00400000))
                        func_802AF9D0(player, 0xbb9, 0x14);
                    // fn_08 resolved OK: clear the pending flag.
                    m->unk223 = 0;
                }
                // func_80174C98 failed: keep the flag set to retry next frame.
            } else {
                m->unk223 = 0;
            }
        }

        // No battle participants and the flag is clear: rolling gauge node.
        if (m->unk22A != 0) {
            cf::CBattleManager* bm3 = cf::CBattleManager::getInstance();
            sentinel = ((cf::CBattleManagerNodeList*)bm3)->sentinel;
            int acount = 0;
            for (cf::CVoiceBattleNode* cur = sentinel->next; cur != sentinel; cur = cur->next)
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
                        cf::CSoundNode* tail = m->unk210;
                        if (tail != 0)
                            tail->next = node;
                        if (m->unk20C == 0) {
                            m->unk20C = node;
                            m->unk208 = (u32)node - (u32)&m->unk4[0];
                        }
                        m->unk210 = node;
                        appended = true;
                    }
                }
                if (!appended) {
                    cf::CSoundNode* node = __ct__802A4870((int)ge);
                    if (node != 0) {
                        cf::CSoundNode* tail = m->unk210;
                        if (tail != 0)
                            tail->next = node;
                        if (m->unk20C == 0) {
                            m->unk20C = node;
                            m->unk208 = (u32)node - (u32)&m->unk4[0];
                        }
                        m->unk210 = node;
                    }
                }
            }
        }
        m->unk22A = 0;

        // Non-presentation: sweep level-thread handlers then pop leading
        // non-matching nodes off the list.
        if (m->unk20C != 0 && !(lbl_eu_80663E24 & 0x00400000)) {
            cf::CSoundNode* cur = m->unk20C;
            while (cur != 0) {
                func_802A3ACC(cur);
                cur = cur->next;
            }
            cf::CSoundNode* head = m->unk20C;
            while (head != 0) {
                bool match = false;
                if (__ptmf_cmpr(head, &lbl_eu_805398C0) != 0 && __ptmf_test(head) != 0)
                    match = true;
                if (match)
                    break;
                cf::CSoundNode* nxt = head->next;
                if (nxt == 0) {
                    m->unk20C = 0;
                    m->unk210 = 0;
                    m->unk204 = 0;
                    m->unk208 = 0;
                } else {
                    m->unk20C = nxt;
                    m->unk208 = (u32)nxt - (u32)&m->unk4[0];
                }
                head = m->unk20C;
            }
        }

        // Passive cell count: accumulate every tick unless the flag blocks it.
        if (!(lbl_eu_80663DA0 & 1) && !(lbl_eu_80663E24 & 0x00400000))
            m->unk218 += lbl_eu_80668C7C;
    }
}
void func_802A1C68(){}
void func_802A1D04(){}
void func_802A1DA8() {
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (m != 0) {
        m->FactoryEvent2();
        lbl_eu_80664A58 = 0;
    }
}
void func_802A1DF0(){}
// Character-voice play gate: if the actor is interact/battle-flagged and its
// state checks pass, resolve the target's voice action and fire a battle/lvl
// voice node.
int func_802A1EA8(cf::CVoiceActorState* self) {
    if (!(self->field_3F00 & 0x2))
        return 0;
    cf::CVoiceActorVTable* vt = self->vtable;
    if (vt->fn_2BC(self) != 0)
        return 0;
    if (self->field_3F28 != 1)
        return 0;
    // State already registers either slot 0xCF or 0xD0; otherwise bail.
    if (func_80148778(&self->unk4[4], 0xcf) == 0 && func_80148778(&self->unk4[4], 0xd0) == 0)
        return 0;
    cf::CVoiceMoveBase* move = &self->moveBase;
    void* moveRes = move->vtable->fn_4C(move);
    void* r = func_8016FE34(func_800B708C((BOOL)(u32)moveRes));
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
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
    lbl_eu_80664A58->unk22C = 0;
}
// Append a freshly-created sound node to the manager's voice-event list.
void func_802A201C() {
    cf::CSoundNode* node = __ct__802A96C0();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
// Resolve a pending voice-action id (from the manager's 0x22C holder) and
// enqueue an action node plus a clear node, if the arguments are non-null.
void func_802A2078(void* a, void* b, void* c) {
    if (b == 0) {
        u32 pending = lbl_eu_80664A58->unk22C;
        if (pending != 0)
            b = (void*)func_8016FE34((void*)func_800B708C((BOOL)pending));
    }
    if (b != 0) {
        cf::CSoundNode* node = __ct__802A92D8(b, a);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
    }
    if (c != 0) {
        cf::CSoundNode* node = func_802AF56C(c);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
    }
}
void func_802A216C(){}
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
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
    node = func_802AF43C(a, b, c);
    cf::CCharVoiceMan* m2 = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m2->unk210;
        if (tail != 0)
            tail->next = node;
        if (m2->unk20C == 0) {
            m2->unk20C = node;
            m2->unk208 = (u32)node - (u32)&m2->unk4[0];
        }
        m2->unk210 = node;
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
        if (((cf::CBattleCountAccessor*)cf::CBattleManager::getInstance())->field_194 < 0x64 &&
            (self->field_3F08 & 0x10000))
            useBattle = 1;
        cf::CSoundNode* node = func_802A5B04(self, useBattle);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
    } else {
        cf::CSoundNode* node = func_802AF49C(self);
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
    }
}
extern "C" cf::CSoundNode* func_802A6DF4();
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
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
        appended = true;
    }
    if (appended)
        return (int)node->field_18;
    return -1;
}
void func_802A24B4(){}
void func_802A2558(){}
// If the passed actor's move-base is the current player, clear voice count 0x224.
void func_802A25EC(cf::CVoiceActorBase* actor) {
    if (!(lbl_eu_80663E24 & 0x00400000)) {
        const u8* movePc = (const u8*)actor;
        if (actor != 0)
            movePc = (const u8*)&actor->moveBase;
        if (movePc == (const u8*)cf::CfGameManager::getPlayer(0)) {
            lbl_eu_80664A58->unk224 = 0;
        }
    }
}
extern "C" float lbl_eu_80668C80;  // 0x3F00-flag probability divisor
extern "C" float lbl_eu_80668C70;  // battle-state gauge threshold
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
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
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
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
    } else {
        cf::CSoundNode* node = __ct__802A5ED4();
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
        cf::CSoundNode* node2 = func_802AF4FC(self, c);
        cf::CCharVoiceMan* m2 = lbl_eu_80664A58;
        if (node2 != 0) {
            cf::CSoundNode* tail = m2->unk210;
            if (tail != 0)
                tail->next = node2;
            if (m2->unk20C == 0) {
                m2->unk20C = node2;
                m2->unk208 = (u32)node2 - (u32)&m2->unk4[0];
            }
            m2->unk210 = node2;
        }
    }
}void func_802A27F4() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A4E48();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A285C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A4E48();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A28C4(int a, int b, int c) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (b != c && c < b) {
        cf::CSoundNode* node = __ct__802A8DE8();
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
    }
}
// ── retail node constructors (cvsys TUs) — un-mangled retail names ──────────
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


// Append a freshly constructed sound node to the voice-event list.
void func_802A293C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_PARTY_GAGE();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A29A4() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A6AA8();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2A0C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A5B88();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2A74() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A86CC();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2ADC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__802A6E84();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2B44() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(0);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2BB0() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(1);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2C1C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = __ct__CVS_THREAD_ORDER(2);
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2C88() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802B9064();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2D0C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (lbl_eu_80664A58->unk229 != 0) {
        cf::CSoundNode* node = func_802A6958();
        cf::CCharVoiceMan* m = lbl_eu_80664A58;
        if (node != 0) {
            cf::CSoundNode* tail = m->unk210;
            if (tail != 0)
                tail->next = node;
            if (m->unk20C == 0) {
                m->unk20C = node;
                m->unk208 = (u32)node - (u32)&m->unk4[0];
            }
            m->unk210 = node;
        }
    }
}
void func_802A2D84() {
    float reset = lbl_eu_80668C68;
    cf::CCharVoiceMan* m0 = lbl_eu_80664A58;
    m0->unk218 = reset;
    lbl_eu_80664A58->unk224 = 0;
    lbl_eu_80664A58->unk228 = 0;
    lbl_eu_80664A58->unk22A = 0;
    cf::CSoundNode* node = func_802B8D4C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
// If the battle-manager actor ring has any entries and the flag is clear,
// flag the manager's byte 0x22A.
void func_802A2E08() {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    cf::CVoiceBattleNode* sent = ((cf::CBattleListAccessor*)bm)->list0;
    int count = 0;
    for (cf::CVoiceBattleNode* cur = sent->next; cur != sent; cur = cur->next)
        count++;
    if (count > 0 && !(lbl_eu_80663E24 & 0x00400000)) {
        lbl_eu_80664A58->unk22A = 1;
    }
}
// Convert an actor id to a voice action; C-linkage retail name func_8016FE34.
extern "C" void* func_8016FE34(void* r3);
extern "C" cf::CSoundNode* func_802A7744();
// C++-mangled func_800B708C__Fi (declared not extern "C" so MWCC mangles it).
extern void* func_800B708C(BOOL id);

// Play a level-up-style character voice: resolve the actor id -> voice action,
// and enqueue a fresh HP-thread node (re-checking the presentation flag).
void func_802A2E68(int id) {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    if (func_8016FE34(func_800B708C(id)) == 0)
        return;
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A7744();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A2EEC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A4798();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
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
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
    node = func_802AF3DC(self);
    cf::CCharVoiceMan* m2 = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m2->unk210;
        if (tail != 0)
            tail->next = node;
        if (m2->unk20C == 0) {
            m2->unk20C = node;
            m2->unk208 = (u32)node - (u32)&m2->unk4[0];
        }
        m2->unk210 = node;
    }
}
void func_802A300C() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802AE38C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A3074() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A7674();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A30DC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A8AC8();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A3144() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802A8B6C();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
void func_802A31AC() {
    if (lbl_eu_80663E24 & 0x00400000)
        return;
    cf::CSoundNode* node = func_802B5970();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (node != 0) {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
    }
}
int func_802A3214() {
    cf::CSoundNode* node = __ct__802A5830();
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    bool appended;
    if (node == 0) {
        appended = false;
    } else {
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
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
        cf::CSoundNode* tail = m->unk210;
        if (tail != 0)
            tail->next = node;
        if (m->unk20C == 0) {
            m->unk20C = node;
            m->unk208 = (u32)node - (u32)&m->unk4[0];
        }
        m->unk210 = node;
        appended = true;
    }
    if (appended)
        return (int)node->field_18;
    return -1;
}
void func_802A330C(){}
// Scan the voice-event list for the node whose type matches the reference pmf
// AND whose +0x18 id matches `arg`, then run its level-thread handler (+0x08).
void func_802A35B8(u32 arg) {
    cf::CCharVoiceMan* m = lbl_eu_80664A58;
    if (m == 0)
        return;
    cf::CSoundNode* node = m->unk20C;
    while (node != 0) {
        if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0 &&
            node->field_18 == arg)
            break;
        node = node->next;
    }
    if (node != 0)
        node->field_1C->fn_08(node);
}
// Scan the list for a matching node (type + id), then run its battle-voice
// dispatch (+0x18) with two extra args; returns its result (0 if not found).
void* func_802A3680(void* a, void* b, void* c) {
    cf::CSoundNode* node = lbl_eu_80664A58->unk20C;
    while (node != 0) {
        if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0 &&
            node->field_18 == (u32)a)
            break;
        node = node->next;
    }
    if (node == 0)
        return 0;
    return node->field_1C->fn_18(node, b, c);
}
// Scan for a node whose type matches the reference pmf and whose +0x18 id is
// `arg`; if found, hand it to func_802A3E88 (0 if not found).
void* func_802A3748(u32 arg) {
    cf::CSoundNode* node = lbl_eu_80664A58->unk20C;
    while (node != 0) {
        if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0 &&
            node->field_18 == arg)
            break;
        node = node->next;
    }
    if (node == 0)
        return 0;
    return func_802A3E88(node);
}
// Factory tick: if the incoming actor is in an interact/battle state (any of
// bits 0x2/0x4/0x8 at +0x64), broadcast to every matching type of voice node.
void CCharVoiceMan_FactoryEvent2(void* self, void* actor) {
    u32 flags = ((cf::CVoiceFactoryActor*)actor)->field_64;
    if (!(flags & 0x2) && !(flags & 0x8) && !(flags & 0x4))
        return;
    cf::CSoundNode* node = lbl_eu_80664A58->unk20C;
    while (node != 0) {
        if (__ptmf_cmpr(node, &lbl_eu_805398C0) != 0 && __ptmf_test(node) != 0)
            node->field_1C->fn_0C(node, actor);
        node = node->next;
    }
}
// Check battle-state voice gates: the current player and its target actor must
// both be in the right state/level to allow a character-voice to fire. Returns
// 1 when every candidate gauge passes, else 0.
int func_802A38C8(cf::CCharVoiceMan* self) {
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    cf::CVoiceBattleNode* sentinel = ((cf::CBattleManagerNodeList*)bm)->sentinel;
    cf::CVoiceBattleNode* cur = sentinel->next;
    int count = 0;
    while (cur != sentinel) {
        cur = cur->next;
        count++;
    }
    if (count <= 0)
        return 0;

    cf::CVoiceActorState* player =
        (cf::CVoiceActorState*)cf::CfGameManager::getPlayer(0);
    if (player != 0)
        player = (cf::CVoiceActorState*)((u8*)player - 0x3E9C);
    if (player == 0)
        return 0;

    void* mv = player->moveBase.vtable->fn_4C(&player->moveBase);
    void* r = func_800B708C((BOOL)(u32)mv);
    cf::CVoiceActorState* other = (cf::CVoiceActorState*)r;
    if (other != 0)
        other = (cf::CVoiceActorState*)((u8*)r - 0x3E9C);
    if (other == 0)
        return 0;
    if (!(other->field_3F00 & 0x4))
        return 0;

    int a = player->vtable->fn_108(player);
    int diff = a - 3;
    int b = other->vtable->fn_108(other);
    if (b < diff)
        return 0;

    float f = other->vtable->fn_130(other);
    if (lbl_eu_80668C70 < f)
        return 0;

    cf::CVoiceActorState* arr[3];
    int n = func_802A7870(arr, 3, 0);
    if (n <= 1)
        return 0;
    float limit = lbl_eu_80668C70;
    for (int i = 0; i < n; i++) {
        cf::CVoiceActorState* obj = arr[i];
        float gf = obj->vtable->fn_130(obj);
        if (gf < limit)
            return 0;
    }
    return 1;
}
