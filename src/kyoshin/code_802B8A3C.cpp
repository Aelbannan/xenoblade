// code_802B8A3C: CVS_THREAD_ORDER methods and voice-thread playback stubs.
// FULL_MATCH: func_802B8D44 -- buffer-size getter (virtual method override).

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_ORDER.hpp"
#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_802B8A3C.hpp"

// ── Target 1: us-802bb76c (func_802B8CFC) ─────────────────────────────────
// Completion callback: if no active voice is playing, invoke the playback
// virtual (blank1, vtable slot 3 / offset 0x8).
void func_802B8CFC(CVS_THREAD_ORDER* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802bb4ac (__ct__CVS_THREAD_ORDER) ────────────────────────
// Factory/constructor for CVS_THREAD_ORDER. Reserves a discarded 0x78-byte
// handle buffer, allocates the 0x24-byte object, constructs the base, sets
// the vtable/owner fields, and copies the init-state triple from
// lbl_eu_8053B7E8. Returns NULL on allocation failure.
CVS_THREAD_ORDER* __ct__CVS_THREAD_ORDER(CVoiceHandle* owner) {
    if (func_802A330C(0x78, 0) == NULL) {
        return NULL;
    }
    CVS_THREAD_ORDER* self = (CVS_THREAD_ORDER*)func_802A34E4(0x24);
    if (self == NULL) {
        return NULL;
    }

    // Construct the base (can throw -> EH guard), then set vtable + slot.
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD();
            ((void**)self)[7] = (void**)lbl_eu_8053B818;
            ((CVS_THREAD_ORDER_Data*)self)->field_0x20 = owner;
        } catch (...) {
            throw;
        }
    }

    // Copy the init-state triple into the first 3 u32s (outside try).
    const u32* base = (u32*)lbl_eu_8053B7E8;
    self->unk0 = (u32*)base[0];
    self->unk4 = base[1];
    self->unk8 = base[2];

    return self;
}

// ── Target 3: us-802bb57c (func_802B8B0C) ─────────────────────────────────
// (Re)initialise the thread and start playback. If a battle is active, or
// the player's voice is idle and the per-slot voice id allows it, plays the
// configured voice for this thread's slot.
void func_802B8B0C(CVS_THREAD_ORDER* self) {
    // Copy init-state triple using an incrementing source pointer so MWCC
    // keeps the lwzu pattern on the first element.
    u32* src = (u32*)lbl_eu_8053B800;
    u32 v0 = *src++;
    u32 v1 = *src++;
    self->unk4 = v1;
    self->unk0 = (u32*)v0;
    u32 v2 = *src++;
    self->unk8 = v2;

    if (((BattleMgrView*)cf::CBattleManager::getInstance())->field_0x20C8 != 0) {
        // Battle is active: jump straight to playback.
        self->func_802A3B50();
        return;
    }

    CVoiceHandle* handle = (CVoiceHandle*)func_8016FE34((void*)cf::CfGameManager::getPlayer(0));
    if (handle == NULL) {
        return;
    }
    if (((int (*)(CVoiceHandle*))handle->vtable[0x2BC / 4])(handle) != 0) {
        return;
    }

    // Look up the voice id for this thread's slot; play only if positive.
    int idx = ((u32*)self)[8];   // owner/slot field at +0x20
    s32 tableVal = lbl_eu_8053B7F4[idx];
    if (tableVal <= 0) {
        return;
    }

    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (func_802A3C44(self, biased, tableVal) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 4: us-802bb670 (func_802B8C00) ─────────────────────────────────
// Playback dispatcher. If the voice is already active or a battle is in
// progress, completes early; otherwise copies the init triple, enumerates the
// player's battle-voice list and plays a random voice (mtRand(2)+0x9C9) for
// each idle entry.
void func_802B8C00(CVS_THREAD_ORDER* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }
    if (((BattleMgrView*)cf::CBattleManager::getInstance())->field_0x20C8 != 0) {
        self->func_802A3B50();
        return;
    }

    u32* src = (u32*)lbl_eu_8053B80C;
    u32 v0 = *src++;
    u32 v1 = *src++;
    self->unk4 = v1;
    self->unk0 = (u32*)v0;
    u32 v2 = *src++;
    self->unk8 = v2;

    CVoiceHandle* handle = (CVoiceHandle*)func_8016FE34((void*)cf::CfGameManager::getPlayer(0));
    CVoiceHandle* arr[3];
    int count = func_802A7870(arr, 3, handle);

    for (int i = 0; i < count; i++) {
        CVoiceHandle* h = arr[i];
        CCharVoice* biased = (CCharVoice*)h;
        if (h != NULL) {
            biased = &h->voice;
        }
        int voiceId = ml::math::mtRand(2) + 0x9C9;
        if (func_802A3C44(self, biased, voiceId) == 0) {
            self->func_802A3B50();
        }
    }
}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; ORDER subclass returns 0x78 (120).
int func_802B8D44(CVS_THREAD_ORDER* self) {
    return CVS_THREAD_ORDER::BUFFER_SIZE;
}

// ── Target 5: us-802bb7bc (func_802B8D4C) ─────────────────────────────────
// Search the voice-config table for a matching voice and play it. Resolution
// first uses the player's voice selector; failing that, iterates the battle
// object list and gimmick list looking for a matching selector.
int func_802B8D4C() {
    OrderConfigEntry* cfg = NULL;

    // First resolution path: player's embedded voice selector.
    CVoiceHandle* handle = (CVoiceHandle*)func_8016FE34((void*)cf::CfGameManager::getPlayer(0));
    if (handle != NULL) {
        int sel = ((int (*)(CCharVoice*))((void**)handle->voice.mVtable)[0x4C / 4])(&handle->voice);
        void* src = func_8016FE34(func_800B708C(sel));
        if (src != NULL) {
            OrderConfigEntry* e = &lbl_eu_8053B840[0];
            while (e->key != 0) {
                if (e->key == ((GimmickHandleData*)src)->field_3F28) {
                    break;
                }
                e++;
            }
            if (e->key == 0) {
                e = NULL;
            }
            cfg = e;
        }
    }

    // Second resolution path: scan battle objects + gimmicks when the first
    // search found nothing.
    if (cfg == NULL) {
        BattleMgrView* bm = (BattleMgrView*)cf::CBattleManager::getInstance();
        void* head = bm->listHead;
        int count = 0;
        void* p = *(void**)head;
        while (p != head) {
            p = *(void**)p;
            count++;
        }

        if (count > 0) {
            OrderGimmickList* glist = func_800B6BC8();
            for (OrderGimmickNode* node = glist->head->next; node != glist->head; node = node->next) {
                GimmickHandleData* gh = (GimmickHandleData*)node->object;
                if (gh != NULL) {
                    gh = (GimmickHandleData*)((char*)node->object - 0x3E9C);
                }

                void* result = ((void* (*)(void*))((void**)gh->field_4)[0x30 / 4])(gh->field_4);
                u32 val = *(u32*)result;
                if (func_80174C98(gh, &val, 0x803) != 0) {
                    OrderConfigEntry* e = &lbl_eu_8053B840[0];
                    while (e->key != 0) {
                        if (e->key == gh->field_3F28) {
                            break;
                        }
                        e++;
                    }
                    if (e->key == 0) {
                        e = NULL;
                    }
                    if (e != NULL) {
                        cfg = e;
                        break;
                    }
                }
            }
        }
    }

    // Play the configured voice, if any.
    int voiceId = -1;
    void* playHandle = NULL;
    if (cfg != NULL) {
        void* h = func_802A7A54(cfg->field_4);
        if (h != NULL) {
            playHandle = h;
            voiceId = cfg->field_6;
        }
    }
    if (voiceId <= 0) {
        return 0;
    }
    if (func_802A330C(0x14, 1) == NULL) {
        return 0;
    }
    if (func_802A34E4(0x20) == NULL) {
        return 0;
    }

    CCharVoice* voice = (CCharVoice*)playHandle;
    if (playHandle != NULL) {
        voice = (CCharVoice*)((char*)playHandle + 0x3E9C);
    }
    func_802A3D54(voice, voiceId, 0x14);
    return 0;
}