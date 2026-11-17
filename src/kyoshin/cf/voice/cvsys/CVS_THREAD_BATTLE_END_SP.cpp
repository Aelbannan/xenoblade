// CVS_THREAD_BATTLE_END_SP: voice thread for special battle-end sequences.
// Matches the sibling CVS_THREAD_DOWN / CVS_THREAD_HAGE / VISION_BREAK
// patterns: voice removal via biased CCharVoice compare, state-triple reset
// via load-with-update, is-active check via vtable[0x2BC/4], and random
// voice command-string selection.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END_SP.hpp"
#include "monolib/math/Random.hpp"

// us-802ae1a8 (CVSBattleEndSPResetState)
// If no voice is active, reset the base state triple in the object header
// (offsets 0x00..0x08) from the init table lbl_eu_8053A1AC.
void CVSBattleEndSPResetState(CVS_THREAD_BATTLE_END_SP* self) {
    if (IsThreadVoiceActive(self) == 0) {
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

// us-802ae1f8 (CVSBattleEndSPRemoveVoice)
// Remove a voice from the slot array by matching its embedded CCharVoice.
// Each non-null handle is biased by +0x3E9C before comparing, so a slot is
// cleared when its embedded voice matches voicePtr.
//
// NOTE: the explicit word-wise walk of self (p += 4) reproduces retail's
// cursor register (r5); the remaining 7-insn diff is a pure counter/handle
// register-color swap (decomp i=r4/handle=r6 vs retail i=r6/handle=r4) that
// resisted every declaration-order permutation tried.
void CVSBattleEndSPRemoveVoice(CVS_THREAD_BATTLE_END_SP* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);

    CVoiceHandle* handle;
    CCharVoice* vp;
    CVS_THREAD_BATTLE_END_SP* p = self;
    int i;
    for (i = 0; self->count > i; i++) {
        handle = p->slots[0];
        // Bias the handle to its embedded CCharVoice (BATTLE_END idiom).
        vp = (CCharVoice*)handle;
        if (handle != NULL) {
            vp = &handle->voice;
        }
        if (vp == voicePtr) {
            p->slots[0] = NULL;
        }
        // Walk the slot array one word per iteration (match-pinned cursor).
        p = (CVS_THREAD_BATTLE_END_SP*)((u8*)p + 4);
    }
}

// us-802ae038 (CVSBattleEndSPResetAndSweep)
// Advance the special battle-end sequence. Each command string is a 2-byte
// pair: [0] = the voice-state to trigger on, [1] = an extra parameter whose
// absolute value (plus 0xCE4) is the voice ID to play. If the matching slot's
// voice is inactive it is played; otherwise (or when the state matches no
// active handle) the sequence ends via the blank1() virtual. Steps the
// command string by 2 bytes while a full pair remains.
void CVSBattleEndSPResetAndSweep(CVS_THREAD_BATTLE_END_SP* self) {
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
                if (h != NULL && HpVoice_MapSubState(h) == cmd0) {
                    found = self->slots[i];
                    goto matched;
                }
                cur = (CVS_THREAD_BATTLE_END_SP*)((u8*)cur + 4);
            }
            found = NULL;
        }
matched:
        if (found == NULL) {
            self->PlayThreadVoice();
            return;
        }

        if (((CVS_THREAD_BATTLE_END_SP_Vtbl*)found)->isVoiceActive() == 0) {
            // Voice inactive -- bias the handle to its embedded CCharVoice
            // and play the command's voice ID (|param| + 0xCE4).
            int voiceId = labs(self->cmdString[1]);
            CCharVoice* vp = (CCharVoice*)found;
            if (found != NULL) {
                vp = &found->voice;
            }
            if (TryRequestVoice(self, vp,
                              voiceId + CVS_THREAD_BATTLE_END_SP::VOICE_ID_BIAS) == 0) {
                self->PlayThreadVoice();
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
// this thread type. Retail exports this as the unmangled symbol CVSBattleEndSPGetBufSize,
// so the definition keeps the retail name.
int CVSBattleEndSPGetBufSize() {
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
extern "C" CVS_THREAD_BATTLE_END_SP* __ct__802AB5F0() {
    // Collect inactive voice handles from the voice-manager list. The loop
    // condition reloads the sentinel each iteration, as retail does.
    CVoiceManager* mgr = (CVoiceManager*)getListB28();
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
                if ((s8)ids[idx] == HpVoice_MapSubState(active[j])) {
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
    if (HpVoice_SeqBelow94() != 0 && cmdString == (const u32*)&lbl_eu_80668DB0) {
        return NULL;
    }

    // Allocate a handle buffer (discarded) and the object itself.
    if (CCharVoiceMan_DispatchById(0x46, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_BATTLE_END_SP* self = (CVS_THREAD_BATTLE_END_SP*)CCharVoiceMan_AllocVoiceArena(0x34);
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

// Retail section reconstruction (measured in
// .scratch/dlabels/a2/CVS_THREAD_BATTLE_END_SP.txt). The factory scans the
// command-list table lbl_eu_8053A100 (a NULL-terminated array of entry
// pointers): each entry E is {id-string, command*, ..., NULL} where E[0]
// points at an .sdata2 id-byte string (byte 0 = required active count,
// bytes 1..count = expected voice IDs) and E[1..] point at command strings
// (2-byte trigger/param pairs, NUL-pair terminated) living in .sdata2 or
// .rodata. lbl_eu_80668DB0 is the shutdown-sentinel command string the
// factory compares by address. Then come three init-state triples, the
// 7-word vtable, the trailing typeinfo-base word, and the .sdata typeinfo
// object ({name, trailing}). Global-scope data symbols: MWCC does not
// mangle them.
//
// NOTE (frozen-rule residual): this TU carries a UNIT_RULES
// extern_data_sections=(".data", ".rodata", ".sdata") entry that
// zeroes these sections in the link-time reloc.o. The retail split object
// genuinely carries them (0x298/0x140/0x8, reconstructed byte-identical
// below), so that rule entry is stale and should be removed; until then
// the link keeps resolving these labels as UNDEF. __RTTI__Q22cf10CVS_THREAD lives in
// the CCharVoiceMan range and is referenced, not defined, here.
extern "C" void PlayThreadVoice();
extern "C" bool CCharVoiceMan_AlwaysTrue();
extern "C" int CCharVoiceMan_NullVoiceDispatch(void* self);
extern u32 __RTTI__Q22cf10CVS_THREAD[2];
extern u32 __RTTI__Q22cf24CVS_THREAD_BATTLE_END_SP[2];

// Packing helpers: MWCC 8-aligns file-scope objects of 8+ bytes, but the
// retail tables pack records at 4-byte intervals. Wrapping 8-/16-byte
// records in align-1 structs recovers the retail layout while keeping every
// retail label a real symbol (same bytes, same offsets, same names).
// Blob packing: MWCC 8-aligns file-scope objects of 8+ bytes, but
// the retail tables pack records at 4-byte intervals (one original
// byte table per section, sub-labelled by the splitter). The three
// static blobs below reproduce the retail bytes contiguously;
// .data entries address records via blob+offset (same linked
// address the retail per-record reloc resolves to).

// sdata2 records lbl_eu_80668CB8..lbl_eu_80668DAC (offsets +0x0..+0xF7)
__declspec(section ".sdata2") static const u8 sdata2blob1[0xF8] = {
    0x02, 0x01, 0x02, 0x00, 0x01, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x02, 0x02, 0x01, 0x03, 0x00, 0x00, 0x02, 0x01, 0x03, 0x00,
    0x01, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x02, 0x03,
    0x02, 0x05, 0x01, 0x07, 0x03, 0x02, 0x00, 0x00, 0x03, 0x01, 0x02, 0x04,
    0x03, 0x01, 0x02, 0x05, 0x03, 0x01, 0x02, 0x06, 0x03, 0x01, 0x02, 0x07,
    0x03, 0x01, 0x03, 0x04, 0x03, 0x01, 0x03, 0x05, 0x03, 0x01, 0x03, 0x06,
    0x03, 0x01, 0x03, 0x07, 0x03, 0x01, 0x04, 0x05, 0x03, 0x01, 0x04, 0x06,
    0x04, 0x04, 0x06, 0x06, 0x01, 0x17, 0x00, 0x00, 0x03, 0x01, 0x04, 0x07,
    0x03, 0x01, 0x05, 0x06, 0x03, 0x01, 0x05, 0x07, 0x07, 0x06, 0x01, 0x1A,
    0x05, 0x06, 0x00, 0x00, 0x03, 0x01, 0x06, 0x07, 0x06, 0x09, 0x07, 0x07,
    0x01, 0x1B, 0x00, 0x00, 0x02, 0x03, 0x05, 0x00, 0x02, 0x0D, 0x05, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x03, 0x04, 0x03, 0x02, 0x03, 0x05,
    0x02, 0x10, 0x05, 0x08, 0x03, 0x0D, 0x00, 0x00, 0x03, 0x02, 0x03, 0x06,
    0x03, 0x02, 0x03, 0x07, 0x03, 0x02, 0x04, 0x05, 0x03, 0x02, 0x04, 0x06,
    0x02, 0x17, 0x06, 0x0C, 0x04, 0x08, 0x00, 0x00, 0x03, 0x02, 0x04, 0x07,
    0x02, 0x18, 0x07, 0x09, 0x04, 0x09, 0x00, 0x00, 0x03, 0x02, 0x05, 0x06,
    0x03, 0x02, 0x05, 0x07, 0x03, 0x02, 0x06, 0x07, 0x03, 0x03, 0x04, 0x05,
    0x04, 0x0A, 0x03, 0x11, 0x05, 0x0D, 0x00, 0x00, 0x03, 0x03, 0x04, 0x06,
    0x03, 0x03, 0x04, 0x07, 0x07, 0x0C, 0x04, 0x0C, 0x03, 0x13, 0x00, 0x00,
    0x03, 0x03, 0x06, 0x07, 0x07, 0x0D, 0x06, 0x12, 0x03, 0x14, 0x00, 0x00,
    0x03, 0x04, 0x05, 0x06, 0x03, 0x04, 0x05, 0x07,
};
extern const u8 lbl_eu_80668DB0[8] = { 0x05, 0x0F, 0x04, 0x0E, 0x07, 0x0E, 0x00, 0x00 };
// sdata2 records lbl_eu_80668DB8..lbl_eu_80668DC4 (retail +0x100..+0x10F)
__declspec(section ".sdata2") static const u8 sdata2blob2[0x10] = {
    0x03, 0x04, 0x06, 0x07, 0x07, 0x0F, 0x04, 0x0F, 0x06, 0x15, 0x00, 0x00,
    0x03, 0x05, 0x06, 0x07,
};

// rodata command-string records lbl_eu_805109D0..lbl_eu_80510AE4 (retail +0x0..+0x11F)
static const u8 rodatablob[0x120] = {
    0x01, 0x04, 0x02, 0x03, 0x01, 0x05, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x06, 0x01, 0x08, 0x04, 0x01, 0x01, 0x09, 0x02, 0xF9, 0x00, 0x00,
    0x02, 0x08, 0x05, 0x01, 0x02, 0x09, 0x01, 0x0A, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x01, 0x02, 0x0A, 0x06, 0x02, 0x02, 0x0B, 0x01, 0x0B, 0x00, 0x00,
    0x01, 0x0C, 0x07, 0x01, 0x07, 0x02, 0x02, 0x0C, 0x01, 0x0D, 0x00, 0x00,
    0x04, 0x02, 0x03, 0x03, 0x01, 0x0E, 0x03, 0x04, 0x01, 0x0F, 0x03, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x03, 0xFA, 0x05, 0x02, 0x01, 0x11,
    0x03, 0xF9, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x06, 0x03, 0x03, 0x08,
    0x06, 0x04, 0x01, 0x12, 0x06, 0x05, 0x01, 0x0A, 0x03, 0xF7, 0x00, 0x00,
    0x01, 0x13, 0x03, 0x0A, 0x07, 0x03, 0x01, 0x14, 0x03, 0x09, 0x07, 0x04,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x15, 0x04, 0x03, 0x01, 0x16, 0x05, 0x04,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x18, 0x04, 0x03, 0x01, 0x16, 0x07, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x07, 0x05, 0x05, 0x06, 0x08, 0x01, 0x19,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x03, 0x0B, 0x02, 0x0E, 0x04, 0x06,
    0x02, 0x0F, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0A, 0x03, 0x0E,
    0x02, 0x11, 0x06, 0x0B, 0x02, 0x12, 0x03, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x13, 0x03, 0x10, 0x07, 0x08, 0x02, 0x14, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x15, 0x04, 0x07, 0x05, 0x09, 0x02, 0x16, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x19, 0x05, 0x0A, 0x06, 0x0D, 0x05, 0x0B, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x1A, 0x05, 0x0C, 0x07, 0x0A, 0x02, 0x1B, 0x00, 0x00, 0x00, 0x00,
    0x07, 0x0B, 0x06, 0x0E, 0x02, 0x1C, 0x06, 0x0F, 0x02, 0x1D, 0x00, 0x00,
    0x06, 0x10, 0x04, 0x0B, 0x06, 0x11, 0x03, 0x12, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x13, 0x04, 0x0D, 0x05, 0x0E, 0x06, 0x14, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x16, 0x07, 0x10, 0x06, 0x17, 0x05, 0x10, 0x00, 0x00, 0x00, 0x00,
};
extern const char lbl_eu_80510AF0[0x20] = "cf::CVS_THREAD_BATTLE_END_SP";

u32 lbl_eu_80539F48[5] = { (u32)&sdata2blob1[0x0], (u32)&sdata2blob1[0x4], (u32)&sdata2blob1[0xC], (u32)&rodatablob[0x0], 0 };
u32 lbl_eu_80539F5C[3] = { (u32)&sdata2blob1[0x14], (u32)&sdata2blob1[0x18], 0 };
u32 lbl_eu_80539F68[3] = { (u32)&sdata2blob1[0x20], (u32)&sdata2blob1[0x24], 0 };
u32 lbl_eu_80539F74[3] = { (u32)&sdata2blob1[0x2C], (u32)&rodatablob[0xC], 0 };
u32 lbl_eu_80539F80[3] = { (u32)&sdata2blob1[0x30], (u32)&rodatablob[0x18], 0 };
u32 lbl_eu_80539F8C[3] = { (u32)&sdata2blob1[0x34], (u32)&rodatablob[0x24], 0 };
u32 lbl_eu_80539F98[3] = { (u32)&sdata2blob1[0x38], (u32)&rodatablob[0x30], 0 };
u32 lbl_eu_80539FA4[3] = { (u32)&sdata2blob1[0x3C], (u32)&rodatablob[0x3C], 0 };
u32 lbl_eu_80539FB0[3] = { (u32)&sdata2blob1[0x40], (u32)&rodatablob[0x4C], 0 };
u32 lbl_eu_80539FBC[3] = { (u32)&sdata2blob1[0x44], (u32)&rodatablob[0x5C], 0 };
u32 lbl_eu_80539FC8[3] = { (u32)&sdata2blob1[0x48], (u32)&rodatablob[0x6C], 0 };
u32 lbl_eu_80539FD4[3] = { (u32)&sdata2blob1[0x4C], (u32)&rodatablob[0x7C], 0 };
u32 lbl_eu_80539FE0[3] = { (u32)&sdata2blob1[0x50], (u32)&sdata2blob1[0x54], 0 };
u32 lbl_eu_80539FEC[3] = { (u32)&sdata2blob1[0x5C], (u32)&rodatablob[0x88], 0 };
u32 lbl_eu_80539FF8[3] = { (u32)&sdata2blob1[0x60], (u32)&rodatablob[0x94], 0 };
u32 lbl_eu_8053A004[3] = { (u32)&sdata2blob1[0x64], (u32)&sdata2blob1[0x68], 0 };
u32 lbl_eu_8053A010[3] = { (u32)&sdata2blob1[0x70], (u32)&sdata2blob1[0x74], 0 };
u32 lbl_eu_8053A01C[3] = { (u32)&sdata2blob1[0x7C], (u32)&sdata2blob1[0x80], 0 };
u32 lbl_eu_8053A028[3] = { (u32)&sdata2blob1[0x88], (u32)&rodatablob[0xA0], 0 };
u32 lbl_eu_8053A034[3] = { (u32)&sdata2blob1[0x8C], (u32)&sdata2blob1[0x90], 0 };
u32 lbl_eu_8053A040[3] = { (u32)&sdata2blob1[0x98], (u32)&rodatablob[0xB0], 0 };
u32 lbl_eu_8053A04C[3] = { (u32)&sdata2blob1[0x9C], (u32)&rodatablob[0xC0], 0 };
u32 lbl_eu_8053A058[3] = { (u32)&sdata2blob1[0xA0], (u32)&rodatablob[0xCC], 0 };
u32 lbl_eu_8053A064[3] = { (u32)&sdata2blob1[0xA4], (u32)&sdata2blob1[0xA8], 0 };
u32 lbl_eu_8053A070[3] = { (u32)&sdata2blob1[0xB0], (u32)&sdata2blob1[0xB4], 0 };
u32 lbl_eu_8053A07C[3] = { (u32)&sdata2blob1[0xBC], (u32)&rodatablob[0xD8], 0 };
u32 lbl_eu_8053A088[3] = { (u32)&sdata2blob1[0xC0], (u32)&rodatablob[0xE4], 0 };
u32 lbl_eu_8053A094[3] = { (u32)&sdata2blob1[0xC4], (u32)&rodatablob[0xF0], 0 };
u32 lbl_eu_8053A0A0[3] = { (u32)&sdata2blob1[0xC8], (u32)&sdata2blob1[0xCC], 0 };
u32 lbl_eu_8053A0AC[3] = { (u32)&sdata2blob1[0xD4], (u32)&rodatablob[0xFC], 0 };
u32 lbl_eu_8053A0B8[3] = { (u32)&sdata2blob1[0xD8], (u32)&sdata2blob1[0xDC], 0 };
u32 lbl_eu_8053A0C4[3] = { (u32)&sdata2blob1[0xE4], (u32)&sdata2blob1[0xE8], 0 };
u32 lbl_eu_8053A0D0[3] = { (u32)&sdata2blob1[0xF0], (u32)&rodatablob[0x108], 0 };
u32 lbl_eu_8053A0DC[3] = { (u32)&sdata2blob1[0xF4], (u32)&lbl_eu_80668DB0, 0 };
u32 lbl_eu_8053A0E8[3] = { (u32)&sdata2blob2[0x0], (u32)&sdata2blob2[0x4], 0 };
u32 lbl_eu_8053A0F4[3] = { (u32)&sdata2blob2[0xC], (u32)&rodatablob[0x114], 0 };

u32 lbl_eu_8053A100[37] = {
    (u32)&lbl_eu_80539F48, (u32)&lbl_eu_80539F5C, (u32)&lbl_eu_80539F68,
    (u32)&lbl_eu_80539F74, (u32)&lbl_eu_80539F80, (u32)&lbl_eu_80539F8C,
    (u32)&lbl_eu_80539F98, (u32)&lbl_eu_80539FA4, (u32)&lbl_eu_80539FB0,
    (u32)&lbl_eu_80539FBC, (u32)&lbl_eu_80539FC8, (u32)&lbl_eu_80539FD4,
    (u32)&lbl_eu_80539FE0, (u32)&lbl_eu_80539FEC, (u32)&lbl_eu_80539FF8,
    (u32)&lbl_eu_8053A004, (u32)&lbl_eu_8053A010, (u32)&lbl_eu_8053A01C,
    (u32)&lbl_eu_8053A028, (u32)&lbl_eu_8053A034, (u32)&lbl_eu_8053A040,
    (u32)&lbl_eu_8053A04C, (u32)&lbl_eu_8053A058, (u32)&lbl_eu_8053A064,
    (u32)&lbl_eu_8053A070, (u32)&lbl_eu_8053A07C, (u32)&lbl_eu_8053A088,
    (u32)&lbl_eu_8053A094, (u32)&lbl_eu_8053A0A0, (u32)&lbl_eu_8053A0AC,
    (u32)&lbl_eu_8053A0B8, (u32)&lbl_eu_8053A0C4, (u32)&lbl_eu_8053A0D0,
    (u32)&lbl_eu_8053A0DC, (u32)&lbl_eu_8053A0E8, (u32)&lbl_eu_8053A0F4,
    0,
};

u32 lbl_eu_8053A194[3] = {
    0,
    0xFFFFFFFF,
    (u32)CVSBattleEndSPResetAndSweep,
};

u32 lbl_eu_8053A1A0[3] = {
    0,
    0xFFFFFFFF,
    (u32)CVSBattleEndSPResetState,
};

u32 lbl_eu_8053A1AC[3] = {
    0,
    0xFFFFFFFF,
    (u32)CVSBattleEndSPResetAndSweep,
};

u32 lbl_eu_8053A1B8[7] = {
    (u32)__RTTI__Q22cf24CVS_THREAD_BATTLE_END_SP,
    0,
    (u32)PlayThreadVoice,
    (u32)CVSBattleEndSPRemoveVoice,
    (u32)CVSBattleEndSPGetBufSize,
    (u32)CCharVoiceMan_AlwaysTrue,
    (u32)CCharVoiceMan_NullVoiceDispatch,
};

u32 lbl_eu_8053A1D4[3] = {
    (u32)__RTTI__Q22cf10CVS_THREAD,
    0,
    0,
};

__declspec(section ".sdata") extern u32 __RTTI__Q22cf24CVS_THREAD_BATTLE_END_SP[2] = {
    (u32)lbl_eu_80510AF0,
    (u32)lbl_eu_8053A1D4,
};