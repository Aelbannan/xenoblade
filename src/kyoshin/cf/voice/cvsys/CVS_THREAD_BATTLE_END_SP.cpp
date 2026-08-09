// CVS_THREAD_BATTLE_END_SP: voice thread for special battle-end sequences.
// Matches the sibling CVS_THREAD_DOWN / CVS_THREAD_HAGE / VISION_BREAK
// patterns: voice removal via biased CCharVoice compare, state-triple reset
// via load-with-update, is-active check via vtable[0x2BC/4], and random
// voice command-string selection.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END_SP.hpp"
#include "monolib/math/Random.hpp"

// vtable[0x2BC/4] is-active check on a voice handle.
typedef int (*IsActiveFunc)(CVoiceHandle*);

// ── Target 1: us-802ae1a8 (func_802ABA70) ──────────────────────────────────
// If no voice is active, reset the base state triple in self->unk0..unk8
// from the init table lbl_eu_8053A1AC.
void func_802ABA70(CVS_THREAD_BATTLE_END_SP* self) {
    if (func_802A3E88(self) == 0) {
        // Pointer increment reproduces the lwzu/spread load-with-update.
        // v0 is declared first so MWCC colours it (the lwzu destination) into
        // the lower register r3, and p into r4 -- matching retail ordering.
        u32 v0;
        const u32* p = lbl_eu_8053A1AC;
        v0 = *p++;
        self->unk4 = *p++;
        self->unk0 = (u32*)v0;
        self->unk8 = *p;
    }
}

// ── Target 2: us-802ae1f8 (func_802ABAC0) ──────────────────────────────────
// Remove a voice from the slot array by matching its embedded CCharVoice.
// Each non-null handle is biased by +0x3E9C before comparing, so a slot is
// cleared when its embedded voice matches voicePtr.
void func_802ABAC0(CVS_THREAD_BATTLE_END_SP* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    CVoiceHandle* handle;
    CCharVoice* biased;
    int i;
    for (i = 0; i < self->count; i++) {
        handle = self->slots[i];
        biased = (CCharVoice*)handle;
        if (handle != NULL) {
            biased = &handle->voice;
        }
        if (biased == voicePtr) {
            self->slots[i] = NULL;
        }
    }
}

// ── Target 3: us-802ae038 (func_802AB900) ──────────────────────────────────
// Advance the special battle-end sequence. Each command string is a 2-byte
// pair: [0] = the voice-state to trigger on, [1] = an extra parameter whose
// absolute value (plus 0xCE4) is the voice ID to play. If the matching slot's
// voice is inactive it is played; otherwise (or when the state matches no
// active handle) the sequence ends via the blank1() virtual. Steps the
// command string by 2 bytes while a full pair remains.
void func_802AB900(CVS_THREAD_BATTLE_END_SP* self) {
    // Restore the base state triple.
    const u32* p = lbl_eu_8053A1A0;
    u32 v0 = *p++;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    for (;;) {
        s8 cmd0 = self->cmdString[0];
        CVoiceHandle* found = NULL;
        if (cmd0 > 0) {
            int i;
            for (i = 0; i < self->count; i++) {
                CVoiceHandle* h = self->slots[i];
                if (h != NULL && func_802A77E8(h) == cmd0) {
                    found = self->slots[i];
                    break;
                }
            }
        }

        if (found == NULL) {
            self->blank1();
            return;
        }

        if (((IsActiveFunc)found->vtable[0x2BC / 4])(found) == 0) {
            // Voice inactive -- play the command's voice ID.
            int voiceId = labs(self->cmdString[1]) + 0xCE4;
            if (func_802A3C44(self, &found->voice, voiceId) == 0) {
                self->blank1();
                return;
            }
        }

        // Advance to the next 2-byte command; stop when the string ends or
        // a new (positive) trigger byte is reached.
        self->cmdString += 2;
        if (self->cmdString[0] == 0) {
            return;
        }
        if (self->cmdString[1] > 0) {
            return;
        }
    }
}

// Virtual override: returns the logical allocation size for this thread type.
int CVS_THREAD_BATTLE_END_SP::blank1() {
    return BUFFER_SIZE;
}

// ── Target 4: us-802add28 (__ct__802AB5F0) ────────────────────────────────
// Constructor / factory for the special battle-end voice thread. Scans the
// global voice-manager circular node list for handles whose voice is
// currently INACTIVE, then selects a command string from the command-list
// table whose trigger matches the active count and whose expected voice IDs
// all appear among the inactive handles. Allocates a 0x46-byte handle buffer
// (discarded) and a 0x34-byte object, initialises it, and returns it.
CVS_THREAD_BATTLE_END_SP* __ct__802AB5F0() {
    // Collect inactive voice handles from the voice-manager list.
    CVoiceManager* mgr = func_800B6BA4();
    CVoiceListNode* node = mgr->field_4->field_0;
    CVoiceHandle* active[8];
    int count = 0;

    while (node != mgr->field_4) {
        CCharVoice* voice = node->field_8;
        CVoiceHandle* h = (CVoiceHandle*)voice;
        if (voice != NULL) {
            // Bias the embedded CCharVoice back to its owning handle.
            h = (CVoiceHandle*)((u8*)voice - 0x3E9C);
        }
        if (((IsActiveFunc)h->vtable[0x2BC / 4])(h) == 0) {
            active[count++] = h;
        }
        node = node->field_0;
    }

    if (count == 0) {
        return NULL;
    }

    // Search the command-list table for an entry that matches the active set.
    const void* const* tbl = (const void* const*)lbl_eu_8053A100;
    const char* selected = NULL;
    for (const void* const* e = tbl; *e != NULL; e++) {
        const char* list = (const char*)*e;
        if ((s8)list[0] != count) {
            continue;
        }
        // Verify each expected voice ID (bytes 1..count) is present.
        int allMatch = 1;
        for (int idx = 1; idx <= count; idx++) {
            int found = 0;
            for (int j = 0; j < count; j++) {
                if ((s8)list[idx] == func_802A77E8(active[j])) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                allMatch = 0;
                break;
            }
        }
        if (!allMatch) {
            continue;
        }

        // Count word-sized command entries (terminated by a zero word).
        const u32* lw = (const u32*)list;
        int len = 0;
        while (lw[1] != 0) {
            lw++;
            len++;
        }
        // Pick a random command from the list.
        int rnd = ml::math::mtRand(len);
        selected = (const char*)((const u32*)list)[1 + rnd];
        break;
    }

    if (selected == NULL) {
        return NULL;
    }
    // The shutdown sentinel forces a NULL result when the factory gate is up.
    if (func_802A8140() != 0 && selected == (const char*)&lbl_eu_80668DB0) {
        return NULL;
    }

    // Allocate a handle buffer (discarded) and the object itself.
    if (func_802A330C(0x46, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_BATTLE_END_SP* obj = (CVS_THREAD_BATTLE_END_SP*)func_802A34E4(0x34);
    if (obj == NULL) {
        return NULL;
    }

    // Construct the object. This portion lives inside the exception handler.
    try {
        __ct__cf_CVS_THREAD(obj);
        ((void**)obj)[7] = (void**)lbl_eu_8053A1B8;
        obj->count = count;
        obj->cmdString = (char*)selected;
        for (int k = 0; k < count; k++) {
            obj->slots[k] = active[k];
        }
    } catch (...) {
        throw;
    }

    // Final base-state triple.
    const u32* fin = lbl_eu_8053A194;
    obj->unk0 = (u32*)fin[0];
    obj->unk4 = fin[1];
    obj->unk8 = fin[2];
    return obj;
}