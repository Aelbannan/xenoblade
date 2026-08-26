// CVS_THREAD_BATTLE_END_SP: voice thread for special battle-end sequences.
// Matches the sibling CVS_THREAD_DOWN / CVS_THREAD_HAGE / VISION_BREAK
// patterns: voice removal via biased CCharVoice compare, state-triple reset
// via load-with-update, is-active check via vtable[0x2BC/4], and random
// voice command-string selection.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END_SP.hpp"
#include "monolib/math/Random.hpp"

// us-802ae1a8 (func_802ABA70)
// If no voice is active, reset the base state triple in the object header
// (offsets 0x00..0x08) from the init table lbl_eu_8053A1AC.
void func_802ABA70(CVS_THREAD_BATTLE_END_SP* self) {
    if (func_802A3E88(self) == 0) {
        // Pointer increment reproduces the lwzu/spread load-with-update.
        // v0 is declared first so MWCC colours it (the lwzu destination) into
        // the lower register r3, and p into r4 -- matching retail ordering.
        CVS_THREAD_STATE3* st = (CVS_THREAD_STATE3*)self;
        u32 v0;
        const u32* p = lbl_eu_8053A1AC;
        v0 = *p++;
        st->word4 = *p++;
        st->word0 = v0;
        st->word8 = *p;
    }
}

// us-802ae1f8 (func_802ABAC0)
// Remove a voice from the slot array by matching its embedded CCharVoice.
// Each non-null handle is biased by +0x3E9C before comparing, so a slot is
// cleared when its embedded voice matches voicePtr.
//
// NOTE: the explicit word-wise walk of self (p += 4) reproduces retail's
// cursor register (r5); the remaining 7-insn diff is a pure counter/handle
// register-color swap (decomp i=r4/handle=r6 vs retail i=r6/handle=r4) that
// resisted every declaration-order permutation tried.
void func_802ABAC0(CVS_THREAD_BATTLE_END_SP* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    CVoiceHandle* handle;
    CVS_THREAD_BATTLE_END_SP* p = self;
    int i;
    for (i = 0; self->count > i; i++) {
        handle = p->slots[0];
        // Bias the handle to its embedded CCharVoice in place.
        if (handle != NULL) {
            handle = (CVoiceHandle*)&handle->voice;
        }
        if ((CCharVoice*)handle == voicePtr) {
            p->slots[0] = NULL;
        }
        // Walk the slot array one word per iteration.
        p = (CVS_THREAD_BATTLE_END_SP*)((u8*)p + 4);
    }
}

// us-802ae038 (func_802AB900)
// Advance the special battle-end sequence. Each command string is a 2-byte
// pair: [0] = the voice-state to trigger on, [1] = an extra parameter whose
// absolute value (plus 0xCE4) is the voice ID to play. If the matching slot's
// voice is inactive it is played; otherwise (or when the state matches no
// active handle) the sequence ends via the blank1() virtual. Steps the
// command string by 2 bytes while a full pair remains.
void func_802AB900(CVS_THREAD_BATTLE_END_SP* self) {
    // Restore the base state triple. Declaring v0 before p colours v0 into
    // the lower register (the lwzu destination), matching retail.
    CVS_THREAD_STATE3* st = (CVS_THREAD_STATE3*)self;
    u32 v0;
    const u32* p = lbl_eu_8053A1A0;
    v0 = *p++;
    st->word4 = *p++;
    st->word0 = v0;
    st->word8 = *p;

    int cmd0;
    int i;
    CVoiceHandle* found;
    CVS_THREAD_BATTLE_END_SP* cur;

    for (;;) {
        cmd0 = *self->cmdString;
        if (cmd0 <= 0) {
            found = NULL;
        } else {
            // Walk the slot array word-by-word; on a match the slot itself
            // becomes the result.
            found = NULL;
            cur = self;
            for (i = 0; i < self->count; i++) {
                CVoiceHandle* h = cur->slots[0];
                if (h != NULL && func_802A77E8(h) == cmd0) {
                    found = self->slots[i];
                    goto matched;
                }
                cur = (CVS_THREAD_BATTLE_END_SP*)((u8*)cur + 4);
            }
            found = NULL;
        }
matched:
        if (found == NULL) {
            self->func_802A3B50();
            return;
        }

        if (((CVS_THREAD_BATTLE_END_SP_Vtbl*)found)->isVoiceActive() == 0) {
            // Voice inactive -- bias the handle to its embedded CCharVoice
            // and play the command's voice ID (|param| + 0xCE4).
            int voiceId = labs(self->cmdString[1]);
            if (found != NULL) {
                found = (CVoiceHandle*)&found->voice;
            }
            if (func_802A3C44(self, (CCharVoice*)found,
                              voiceId + CVS_THREAD_BATTLE_END_SP::VOICE_ID_BIAS) == 0) {
                self->func_802A3B50();
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

// Virtual override body (vtable[2]): returns the logical allocation size for
// this thread type. Retail exports this as the unmangled symbol func_802ABB38,
// so the definition keeps the retail name.
int func_802ABB38() {
    return CVS_THREAD_BATTLE_END_SP::BUFFER_SIZE;
}

// us-802add28 (__ct__802AB5F0)
// Constructor / factory for the special battle-end voice thread. Scans the
// global voice-manager circular node list for handles whose voice is
// currently INACTIVE, then selects a command string from the command-list
// table whose trigger matches the active count and whose expected voice IDs
// all appear among the inactive handles. Allocates a 0x46-byte handle buffer
// (discarded) and a 0x34-byte object, initialises it, and returns it.
// The construction tail sits in a try block whose catch rethrows via the
// runtime __throw(0,0,0); retail guards it with a redundant null re-check.
CVS_THREAD_BATTLE_END_SP* __ct__802AB5F0() {
    // Collect inactive voice handles from the voice-manager list. The loop
    // condition reloads the sentinel each iteration, as retail does.
    CVoiceManager* mgr = (CVoiceManager*)func_800B6BA4();
    CVoiceHandle* active[8];
    CVoiceHandle* handle;
    int count = 0;

    CVoiceListNode* node = mgr->field_4->field_0;
    while (node != mgr->field_4) {
        // Bias the embedded CCharVoice back to its owning handle.
        handle = (CVoiceHandle*)node->field_8;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }
        if (((CVS_THREAD_BATTLE_END_SP_Vtbl*)handle)->isVoiceActive() == 0) {
            active[count++] = handle;
        }
        node = node->field_0;
    }

    if (count <= 0) {
        return NULL;
    }

    // Search the command-list table for an entry that matches the active set.
    // Each table entry E is a NULL-terminated array of pointers:
    //   E[0]       -> id-byte string (byte 0 = required active count,
    //                 bytes 1..count = expected voice IDs)
    //   E[1..len]  -> candidate command strings
    //   E[len+1]   -> NULL terminator
    // Assignment-in-condition shares the table load between the NULL test
    // and the body (retail reuses the tested word as the entry pointer).
    const u32* cmdString = NULL;
    const u32* work;
    const u32* entry;
    for (work = lbl_eu_8053A100;
         (entry = (const u32*)*work) != NULL;
         work++) {
        const char* ids = (const char*)entry[0];
        if ((s8)ids[0] != count) {
            continue;
        }
        // Verify each expected voice ID (bytes 1..count) is present.
        int allMatch = 1;
        int idx = 1;
        while (idx < count + 1) {
            int found = 0;
            int j = 0;
            while (j < count) {
                if ((s8)ids[idx] == func_802A77E8(active[j])) {
                    found = 1;
                    break;
                }
                j++;
            }
            if (found == 0) {
                allMatch = 0;
                break;
            }
            idx++;
        }
        if (allMatch == 0) {
            continue;
        }

        // Count the command pointers (terminated by a NULL entry).
        // Reload the table entry like retail; walk and pick share that word.
        const u32* base = (const u32*)*work;
        const u32* w = base;
        int len = 0;
        while (w[1] != 0) {
            w++;
            len++;
        }
        // Pick a random command string from the entry.
        const u32* pick = base + ml::math::mtRand(len);
        cmdString = (const u32*)pick[1];
        break;
    }

    if (cmdString == NULL) {
        return NULL;
    }
    // The shutdown sentinel forces a NULL result when the factory gate is up.
    if (func_802A8140() != 0 && cmdString == (const u32*)&lbl_eu_80668DB0) {
        return NULL;
    }

    // Allocate a handle buffer (discarded) and the object itself.
    if (func_802A330C(0x46, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_BATTLE_END_SP* self = (CVS_THREAD_BATTLE_END_SP*)func_802A34E4(0x34);
    if (self == NULL) {
        return NULL;
    }

    // Redundant null re-check reproduces retail's `beq` guard on the EH
    // region; r3 still holds self so the base ctor needs no argument reload.
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD();
            ((CVS_THREAD_BATTLE_END_SP_raw*)self)->vtable =
                (u32*)lbl_eu_8053A1B8;
            self->count = count;
            self->cmdString = (char*)cmdString;
            int k;
            for (k = 0; k < count; k++) {
                self->slots[k] = active[k];
            }
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Final base-state triple (installs the handler ptmf at offset 0).
    // Typed 12-byte ptmf struct copy reproduces retail's grouped schedule.
    *(VoiceCb*)self = *(VoiceCb*)lbl_eu_8053A194;
    return self;
}