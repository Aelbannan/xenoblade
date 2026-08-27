// code_802B8A3C: CVS_THREAD_ORDER methods and voice-thread playback stubs.
// FULL_MATCH: func_802B8D44 -- buffer-size getter (virtual method override).

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_ORDER.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_802B8A3C.hpp"

// us-802bb76c (func_802B8CFC)
// Completion callback: if no active voice is playing, invoke the playback
// virtual (blank1, vtable slot 3 / offset 0x8).
void func_802B8CFC(CVS_THREAD_ORDER* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// us-802bb4ac (__ct__CVS_THREAD_ORDER)
// Factory/constructor for CVS_THREAD_ORDER. Reserves a discarded 0x78-byte
// handle buffer, allocates the 0x24-byte object, constructs the base, sets
// the vtable/owner fields, and copies the init-state triple from
// lbl_eu_8053B7E8. Returns NULL on allocation failure.
CVS_THREAD_ORDER* __ct__CVS_THREAD_ORDER(CVoiceHandle* owner) {
    u32 v1;

    if (func_802A330C(0x78, 0) == NULL) {
        return NULL;
    }
    CVS_THREAD_ORDER* self = (CVS_THREAD_ORDER*)func_802A34E4(0x24);
    if (self == NULL) {
        return NULL;
    }
    CVS_THREAD_ORDER_raw* raw = (CVS_THREAD_ORDER_raw*)self;

    // Retail guards the constructor region with a redundant self null
    // re-check (the `beq`) and a zero-arg __throw pad; the try-block form
    // below reproduces that exception lowering.
    if (self != NULL) {
        try {
            // Base constructor (self still live in r3), then vtable + owner.
            __ct__cf_CVS_THREAD();
            raw->vtable = (u32*)lbl_eu_8053B818;
            raw->field_0x20 = owner;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Copy init data from the global table into the object's first 3 u32s.
    const u32* base = (const u32*)(u32)lbl_eu_8053B7E8;
    v1 = base[1];
    raw->state0 = base[0];
    raw->state1 = v1;
    raw->state2 = base[2];

    return self;
}

// us-802bb57c (func_802B8B0C)
// (Re)initialise the thread and start playback. If a battle is active, or
// the player's voice is idle and the per-slot voice id allows it, plays the
// configured voice for this thread's slot.
void func_802B8B0C(CVS_THREAD_ORDER* self) {
    // Copy init-state triple. First element read via post-increment (lwzu);
    // words 1 and 2 are indexed off the base pointer.
    u32 v0;
    u32* src = (u32*)(u32)lbl_eu_8053B800;
    v0 = *src++;
    OrderThreadWords* words = (OrderThreadWords*)self;
    words->words[1] = src[0];
    words->words[0] = v0;
    words->words[2] = src[1];

    if (((BattleMgrView*)cf::CBattleManager::getInstance())->field_0x20C8 != 0) {
        // Battle is active: jump straight to playback.
        self->func_802A3B50();
        return;
    }

    CVoiceHandle* handle = (CVoiceHandle*)func_8016FE34(cf::CfGameManager::getPlayer(0));
    if (handle == NULL) {
        return;
    }
    // Genuine virtual call via the view class so MWCC emits the retail
    // r12-chain for the vtable-0x2bc probe.
    if (((CVoiceDevView*)handle)->mAt2BC() != 0) {
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

// us-802bb670 (func_802B8C00)
// Playback dispatcher. If the voice is already active or a battle is in
// progress, completes early; otherwise copies the init triple, enumerates the
// player's battle-voice list and plays a random voice (mtRand(2)+0x9C9) for
// each idle entry.
void func_802B8C00(CVS_THREAD_ORDER* self) {
    u32 v0;
    u32* src;
    CVoiceHandle* handle;
    CVoiceHandle* arr[3];
    CVoiceHandle** bb;
    int zz;
    int count;
    int rnd;

    if (func_802A3E88(self) != 0) {
        return;
    }
    if (((BattleMgrView*)cf::CBattleManager::getInstance())->field_0x20C8 != 0) {
        self->func_802A3B50();
        return;
    }

    // Copy init-state triple: word 0 read via post-increment (lwzu), words
    // 1 and 2 read back through the retained base pointer.
    src = (u32*)(u32)lbl_eu_8053B80C;
    v0 = *src++;
    ((OrderThreadWords*)self)->words[1] = src[0];
    ((OrderThreadWords*)self)->words[0] = v0;
    ((OrderThreadWords*)self)->words[2] = src[1];

    handle = (CVoiceHandle*)func_8016FE34(cf::CfGameManager::getPlayer(0));
    count = func_802A7870(arr, 3, handle);

    // Counter/walker loop matching retail's register layout: reusing
    // `handle` as the entry variable (instead of a separate loop local)
    // is what puts its web in r30 like retail.
    bb = arr;
    zz = 0;
    while (zz < count) {
        handle = *bb;
        CCharVoice* biased = (handle != NULL) ? &handle->voice : (CCharVoice*)handle;
        // Rand result held in its own temp; the bias is added at the call
        // site (retail keeps mr/addi separate around the argument setup).
        rnd = ml::math::mtRand(2);
        if (func_802A3C44(self, biased, rnd + 0x9C9) == 0) {
            self->func_802A3B50();
        }
        bb++;
        zz++;
    }
}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; ORDER subclass returns 0x78 (120).
int func_802B8D44(CVS_THREAD_ORDER* self) {
    return CVS_THREAD_ORDER::BUFFER_SIZE;
}

// us-802bb7bc (func_802B8D4C)
// Resolve a config entry from the player's voice selector; failing that, walk
// the gimmick list comparing each object's name against 0x803. Play the found
// entry's voice. Returns 0 unconditionally.
int func_802B8D4C() {
    OrderConfigEntry* cfg;

    // First resolution path: player's embedded voice selector. The virtual
    // probe runs on the biased voice pointer (handle + 0x3E9C), which retail
    // fuses into an lwzu.
    CVoiceHandle* handle = (CVoiceHandle*)func_8016FE34((void*)cf::CfGameManager::getPlayer(0));
    if (handle == NULL) {
        cfg = NULL;
    } else {
        int sel = ((CVoiceSelView*)&handle->voice)->mAt4C();
        GimmickHandleData* src = (GimmickHandleData*)func_8016FE34(findObjectById(sel));
        if (src == NULL) {
            cfg = NULL;
        } else {
            OrderConfigEntry* e = lbl_eu_8053B840;
            while (e->key != 0) {
                if (e->key == src->field_3F28) {
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

    // Second resolution path: count live battle objects, then scan the
    // gimmick list for one whose probed name matches key 0x803.
    if (cfg == NULL) {
        BattleMgrView* bm = (BattleMgrView*)cf::CBattleManager::getInstance();
        int count = 0;
        u8* p = *(u8**)bm->listHead;
        u8* list = bm->listHead;
        for (; p != list; p = *(u8**)p) {
            count++;
        }

        if (count <= 0) {
            cfg = NULL;
        } else {
            OrderGimmickList* glist = func_800B6BC8();
            OrderGimmickNode* cur;
            for (cur = glist->head->next; cur != glist->head; cur = cur->next) {
                // De-bias the gimmick object pointer back to its handle.
                GimmickHandleData* gh = (GimmickHandleData*)cur->object;
                if (gh != NULL) {
                    gh = (GimmickHandleData*)((char*)gh - 0x3E9C);
                }

                GimmickNameValue* nameObj = ((CGimmickNameView*)gh->field_4)->mAt30();
                u32 val = nameObj->name;
                if (func_80174C98(gh, &val, 0x803) != 0) {
                    OrderConfigEntry* e = lbl_eu_8053B840;
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
            cfg = NULL;
        }
    }

    // Retail carries a redundant self-reassignment here (the inlined search
    // helper's merge point); reproduced as a ternary.
    cfg = (cfg != NULL) ? cfg : NULL;

    // Play the configured voice, if any.
    s16 voiceId;
    CVoiceHandle* playHandle;
    if (cfg != NULL) {
        playHandle = (CVoiceHandle*)func_802A7A54(cfg->field_4);
        if (playHandle == NULL) {
            voiceId = -1;
        } else {
            voiceId = cfg->field_6;
        }
    } else {
        voiceId = -1;
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

    // Bias the handle to its embedded voice object (+0x3E9C) in place,
    // matching retail's single r26 web.
    if (playHandle != NULL) {
        playHandle = (CVoiceHandle*)&playHandle->voice;
    }
    func_802A3D54((CCharVoice*)playHandle, voiceId, 0x14);
    return 0;
}