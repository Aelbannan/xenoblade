// CVS_THREAD_EHP: Voice thread for EHP (Emergency HP recovery) sequences.
// Five matched functions: completion callback, voice removal, slot-rotation
// advance, slot-2 play, and the standalone EHP voice selector.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "monolib/math/Random.hpp"
#include <string.h>

// 0x4330000080000000 s32->f32 conversion magic (2^52 + 2^31): retail-named
// sdata2 object referenced by the implicit-cast literal pool.
extern f64 lbl_eu_80668C98;

// Init-data tables (3 u32s each: {0, -1, callback}). Each slot state carries
// a {field_0, field_4, callback} triple that the rotation function copies into
// unk0/unk4/unk8 when the rotating index is not at the stop index.

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; EHP subclass returns 0xB4 (180).
int func_802A6818() {
    return 0xB4;
}

// us-802a8e4c (func_802A6718)
// Completion callback: if no active voice is playing, invoke the
// playback-start virtual (CVS_THREAD::func_802A3B50, vtable slot 0).
void func_802A6718(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// us-802a8e94 (func_802A6760)
// Remove a voice from all slots by matching its embedded CCharVoice pointer.
// A CVoiceHandle stores the CCharVoice at offset 0x3E9C, so a non-null handle
// is biased by 0x3E9C before comparing against the incoming voice pointer.
void func_802A6760(CVS_THREAD_EHP* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    // Slot 0x20
    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }

    // Slot 0x24
    handle = self->field_0x24;
    biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x24 = NULL;
    }

    // Rotating slots 0x2C..0x34 (count in field_0x3c)
    CVSlotWalk* cursor = (CVSlotWalk*)self;
    int i;
    for (i = 0; i < self->field_0x3c; i++) {
        handle = cursor->slot;
        biased = (CCharVoice*)handle;
        if (handle != NULL) {
            biased = &handle->voice;
        }
        if (biased == voicePtr) {
            cursor->slot = NULL;
        }
        // advance one handle slot
        cursor = (CVSlotWalk*)((CVoiceHandle**)cursor + 1);
    }
}

// us-802a8cc0 (func_802A658C)
// Advance the rotating index (field_0x38) forward or backward depending on the
// direction flag (field_0x44), wrapping at the bounds (0..field_0x3c). When the
// index lands on the stop index (field_0x40), the playback-start virtual is
// invoked; otherwise the slot-state triple is reloaded from lbl_eu_80539B14.
void func_802A658C(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    if (self->field_0x44 == 0) {
        // Forward: increment, wrap to 0 once past the bound.
        int newIdx = self->field_0x38 + 1;
        self->field_0x38 = newIdx;
        if (self->field_0x3c < newIdx) {
            self->field_0x38 = 0;
        }
    } else {
        // Backward: decrement, wrap to bound-1 on underflow.
        int newIdx = self->field_0x38 - 1;
        self->field_0x38 = newIdx;
        if (newIdx < 0) {
            self->field_0x38 = self->field_0x3c - 1;
        }
    }

    // Reload the slot-state triple unless the index landed on the stop index.
    if (self->field_0x38 != self->field_0x40) {
        u32 v0;
        const u32* p = lbl_eu_80539B14;
        v0 = *p++;
        (*(CVSlotState*)self).field_4 = *p++;
        (*(CVSlotState*)self).field_0 = v0;
        (*(CVSlotState*)self).callback = *p;
    } else {
        self->func_802A3B50();
    }
}

// us-802a8d84 (func_802A6650)
// Play function for slot 2. Reloads the slot-state triple from lbl_eu_80539B20,
// finds a free voice handle (excluding slot 2's own handle), and if that handle
// is inactive plays a random voice ID (mtRand(2) + 0x51D). On any failure the
// playback-start virtual is invoked as a fallback.
void func_802A6650(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    // Reload slot-state triple {field_0, field_4, callback}. The field_0x24
    // read is hoisted between the table loads (retail interleave).
    u32 v0;
    const u32* p = lbl_eu_80539B20;
    v0 = *p++;
    CVoiceHandle* handle = self->field_0x24;
    (*(CVSlotState*)self).field_4 = *p++;
    (*(CVSlotState*)self).field_0 = v0;
    (*(CVSlotState*)self).callback = *p;

    handle = func_802A7998(handle);
    if (handle != NULL) {
        if (((CVoiceChainVTV*)handle)->isActive() == 0) {
            // Voice is not active -- play a random voice ID. Bias the handle
            // to its embedded CCharVoice in place (retail reuses the handle
            // register for the biased pointer).
            CCharVoice* voicePtr = (CCharVoice*)handle;
            if (handle != NULL) {
                voicePtr = &handle->voice;
            }
            int voiceId = ml::math::mtRand(2);
            if (func_802A3C44(self, voicePtr, voiceId + 0x51D) != 0) {
                return;
            }
        }
    }

    // Fallback: invoke the playback-start virtual.
    self->func_802A3B50();
}

// us-802a8f54 (func_802A6820)
// Standalone EHP voice selector. Given two party-slot indices (a, b), it picks
// a voice ID based on their relationship, allocates a 0xAA-byte buffer, and
// plays the voice on the currently free handle. Returns 0 in all paths.
int func_802A6820(int a, int b) {
    if (a == b) {
        return 0;
    }

    CVoiceHandle* handle = func_802A7998(NULL);
    if (handle == NULL || ((CVoiceChainVTV*)handle)->isActive() != 0) {
        // Handle unusable (free-list miss or voice still active).
        return 0;
    }

    // Call the sub-object's virtual (slot 12 @ 0x30); it returns a pointer to
    // the category value used by the func_80174C98 gate.
    CVSubObj* subobj = (CVSubObj*)handle->unk4;
    u32 value = *((CVSubChainVTV*)subobj)->getRef();
    if (func_80174C98(handle, &value, 0x803) == 0) {
        return 0;
    }

    // Choose the voice ID from the relationship between a and b.
    if (b < a && a >= 2) {
        b = ml::math::mtRand(2) + 0x6A5;
    } else if (a < b && a == 1) {
        b = 0x6A7;
    } else {
        return 0;
    }

    // Allocate the 0xAA-byte playback buffer.
    if (func_802A330C(0xAA, 1) == NULL) {
        return 0;
    }

    // Play the selected voice on the (biased) handle.
    CCharVoice* voicePtr = (CCharVoice*)handle;
    if (handle != NULL) {
        voicePtr = &handle->voice;
    }
    func_802A3D54(voicePtr, b, 0xAA);
    return 0;
}

// us-802a8608 (factory __ct__802A5ED4)
// EHP thread factory. Gates on a negative third parameter and manager flag
// bit 2 (word at handle +0x3F00), then on the owner's HP ratio: the gauge
// fraction (max/cur) must exceed 0.0 (or 0.3 in the duplicated re-check)
// while (max-hp)/cur is positive. Allocates the 0xB4 handle buffer and the
// 0x48-byte object, base-constructs it, installs the EHP vtable manually,
// zeroes the rotating slots, randomizes the rotation direction, and installs
// the default slot-state triple. Returns NULL on any gate failure.
CVS_THREAD_EHP* __ct__802A5ED4(CVoiceHandle* handle, CVoiceHandle* owner2, s32 hp) {
    CVS_THREAD_EHP* self;
    int ok;
    float cur;
    float ratio;
    float frac;

    if (hp >= 0) {
        return NULL;
    }
    if ((handle->field_0x3F00 & 4) == 0) {
        return NULL;
    }

    // Gauge computation (textually duplicated in retail):
    //   frac = max / cur, ratio = (max - hp) / cur.
    // The hp s32->double step must be recomputed inside each block (retail
    // does not keep it live in an FPR across the virtual calls).
    cur = ((CVoiceChainVTV*)handle)->getCur();
    ratio = (((CVoiceChainVTV*)handle)->getMax() - (double)hp) / cur;
    cur = ((CVoiceChainVTV*)handle)->getCur();
    frac = ((CVoiceChainVTV*)handle)->getMax() / cur;

    ok = frac > lbl_eu_80668C90 && lbl_eu_80668C90 < ratio;
    if (ok == 0) {
        // Duplicated re-check with the 0.3 threshold.
        cur = ((CVoiceChainVTV*)handle)->getCur();
        ratio = (((CVoiceChainVTV*)handle)->getMax() - (double)hp) / cur;
        cur = ((CVoiceChainVTV*)handle)->getCur();
        frac = ((CVoiceChainVTV*)handle)->getMax() / cur;

        ok = frac > lbl_eu_80668C94 && lbl_eu_80668C94 < ratio;
        if (ok == 0) {
            return NULL;
        }
    }

    // Allocate the handle buffer (0xB4 bytes, discarded) and the object.
    if (func_802A330C(0xB4, 1) == NULL) {
        return NULL;
    }
    self = (CVS_THREAD_EHP*)func_802A34E4(0x48);
    if (self == NULL) {
        return NULL;
    }

    // Retail guards the constructor region with a redundant self null
    // re-check (the `beq`) and a zero-arg __throw pad; the guarded try-block
    // form below reproduces that exception lowering (see the CVS_THREAD_DOWN
    // factory for the same shape).
    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD(self);
            ((void**)self)[7] = (void*)lbl_eu_80539B2C;
            self->field_0x20 = handle;
            self->field_0x24 = owner2;
            self->field_0x28 = hp;
            memset(self->field_0x2c, 0, sizeof self->field_0x2c);
            self->field_0x3c = 0;
            self->field_0x38 = 0;
            self->field_0x40 = 0;
            self->field_0x44 = ml::math::mtRand(2) != 0;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Install the default slot-state triple from the init table.
    const u32* fin = lbl_eu_80539AB8;
    u32 w1 = fin[1];
    u32 w0 = fin[0];
    u32* dst = (u32*)self;
    dst[0] = w0;
    dst[1] = w1;
    dst[2] = fin[2];
    return self;
}

// us-802a88b0 (func_802A617C)
// EHP playback selector driven by the owner's HP gauge. Gates on both owner
// slots being live, reads the HP ratio through the gauge object's float
// getters (slots 74/75), and either collects free rotating slots (low-gauge
// case) or plays a random damage voice on slot 0x24's embedded CCharVoice
// (high-gauge case). Falls back to the playback-start virtual otherwise.
void func_802A617C(CVS_THREAD_EHP* self) {
    if (self->field_0x20 != NULL && self->field_0x24 != NULL &&
        ((CVoiceChainVTV*)self->field_0x24)->isActive() == 0) {
        // Gauge computation (textually duplicated in retail):
        //   ratio = (max - hp) / cur, then cur is re-fetched and
        //   frac = max / cur.
        float hp = (float)self->field_0x28;
        CVoiceHandle* gauge = self->field_0x20;
        float cur = ((CVoiceChainVTV*)gauge)->getCur();
        float ratio = (((CVoiceChainVTV*)gauge)->getMax() - hp) / cur;
        cur = ((CVoiceChainVTV*)gauge)->getCur();
        float frac = ((CVoiceChainVTV*)gauge)->getMax() / cur;

        // Retail materializes the branch condition into a boolean.
        int lowGauge = frac <= lbl_eu_80668C90 && lbl_eu_80668C90 < ratio;
        if (lowGauge != 0) {
            // Low-gauge path: install the slot-state triple, collect free
            // rotating handles, pick a random index among them.
            u32 v0;
            const u32* p = lbl_eu_80539AC4;
            v0 = *p++;
            CVoiceHandle* exclude = self->field_0x24;
            (*(CVSlotState*)self).field_4 = *p++;
            (*(CVSlotState*)self).field_0 = v0;
            (*(CVSlotState*)self).callback = *p;

            int count = func_802A7870(self->field_0x2c, 3, exclude);
            self->field_0x3c = count;
            if (count > 0) {
                int idx = ml::math::mtRand(count);
                self->field_0x40 = idx;
                self->field_0x38 = idx;
                return;
            }
        } else {
            // High-gauge path: duplicated gauge computation with the 0.3
            // threshold, then play a random damage voice on slot 0x24.
            float hp = (float)self->field_0x28;
            CVoiceHandle* gauge = self->field_0x20;
            float cur = ((CVoiceChainVTV*)gauge)->getCur();
            float ratio = (((CVoiceChainVTV*)gauge)->getMax() - hp) / cur;
            cur = ((CVoiceChainVTV*)gauge)->getCur();
            float frac = ((CVoiceChainVTV*)gauge)->getMax() / cur;

            int highGauge = frac <= lbl_eu_80668C94 && lbl_eu_80668C94 < ratio;
            if (highGauge != 0) {
                u32 v0;
                const u32* p = lbl_eu_80539AD0;
                v0 = *p++;
                CVoiceHandle* handle = self->field_0x24;
                (*(CVSlotState*)self).field_4 = *p++;
                (*(CVSlotState*)self).field_0 = v0;
                (*(CVSlotState*)self).callback = *p;

                // Bias a non-null handle to its embedded CCharVoice.
                CCharVoice* voicePtr = (CCharVoice*)handle;
                if (handle != NULL) {
                    voicePtr = &handle->voice;
                }
                int rand = ml::math::mtRand(2);
                if (func_802A3C44(self, voicePtr, rand + 0x517) != 0) {
                    return;
                }
            }
        }
    }

    // Fallback: fire the playback-start virtual (vtable offset 8).
    self->func_802A3B50();
}

// us-802a8b3c (func_802A6408)
// EHP start request: install the slot-state triple from lbl_eu_80539ADC, then
// if both the owner (field_0x24) and the current rotating slot are idle, pick
// a voice ID from the owner's battle state and play it on the rotating slot's
// embedded CCharVoice. Any failure falls back to the playback-start virtual.
void func_802A6408(CVS_THREAD_EHP* self) {
    u32 v0;
    const u32* p = lbl_eu_80539ADC;
    v0 = *p++;
    CVoiceHandle* owner = self->field_0x24;
    (*(CVSlotState*)self).field_4 = *p++;
    (*(CVSlotState*)self).field_0 = v0;
    (*(CVSlotState*)self).callback = *p;

    int voiceId = -1;

    if (owner != NULL && ((CVoiceChainVTV*)owner)->isActive() == 0) {
        CVoiceHandle* rot = self->field_0x2c[self->field_0x38];
        if (rot != NULL && ((CVoiceChainVTV*)rot)->isActive() == 0) {
            switch (func_802A77E8(self->field_0x24)) {
            case 0:
                voiceId = 0xAF1;
                break;
            case 1:
                voiceId = 0xAF2;
                break;
            case 2:
                voiceId = 0xAF3;
                break;
            case 3:
                voiceId = 0xAF4;
                break;
            case 4:
                voiceId = 0xAF5;
                break;
            case 5:
            case 10:
                // Case 10 shares this arm: retail table has 11 entries
                // (cmpli 10) with only one pair-check block.
                // Ternary arm order matters: retail emits li 0x450; bne; li 0xAF6.
                voiceId =
                    (func_802A7B90(self->field_0x2c[self->field_0x38], self->field_0x24) == 0) ? 0xAF6 : 0x450;
                break;
            case 6:
                voiceId = 0xAF7;
                break;
            case 7:
                voiceId = 0xAF8;
                break;
            case 8:
                voiceId = 0xAF9;
                break;
            case 9:
                voiceId = 0xAFA;
                break;
            default:
                voiceId = -1;
                break;
            }

            if (voiceId > 0) {
                // Retail re-reads the rotating slot here (calls above may have
                // changed it); bias a non-null handle to its CCharVoice.
                CVoiceHandle* handle = self->field_0x2c[self->field_0x38];
                CCharVoice* voicePtr = (CCharVoice*)handle;
                if (handle != NULL) {
                    voicePtr = &handle->voice;
                }
                if (func_802A3C44(self, voicePtr, voiceId) != 0) {
                    return;
                }
            }
        }
    }

    // Fallback: invoke the playback-start virtual.
    self->func_802A3B50();
}

// us-802a908c (func_802A6958)
// EHP thread start request. Gated on manager flag bit 15 (word at handle
// +0x3F08) and on the handle itself being inactive. Scans the global voice
// handle list for any active handle whose category passes the
// func_80174C98 gate; if none qualifies nothing plays. Otherwise allocates
// the 0x28-byte playback buffer and plays voice 0xA8D on this handle's
// embedded CCharVoice. Returns 0 in all paths.
int func_802A6958(CVoiceHandle* self) {
    CVoiceHandle* handle;
    CVoiceHandleListNode* node;
    int played;
    CVoiceHandleList* list;
    played = 0;

    if ((((CVoiceHandleFlags*)self)->field_0x3F08 & 0x10000) == 0) {
        return 0;
    }
    if (((CVoiceChainVTV*)self)->isActive() != 0) {
        return 0;
    }

    // Walk the global handle list; each node's value is biased by +0x3E9C
    // (points at the embedded CCharVoice), so undo the bias to recover the
    // handle base before dispatching virtuals.
    list = func_800B6BC8();
    for (node = list->end->next; node != list->end; node = node->next) {
        handle = node->value;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }
        if (((CVoiceChainVTV*)handle)->isActive() == 0) {
            // Category check through the polymorphic sub-object reached via
            // handle+0x04 (virtual slot 12 @ 0x30).
            CVSubObj* subobj = (CVSubObj*)handle->unk4;
            u32 value = *((CVSubChainVTV*)subobj)->getRef();
            if (func_80174C98(handle, &value, 0x803) != 0) {
                played = 1;
                break;
            }
        }
    }

    if (played == 0) {
        return 0;
    }
    // Allocate the 0x28-byte playback buffer.
    if (func_802A330C(0x28, 1) == NULL) {
        return 0;
    }
    if (func_802A7FE4(self) != 0) {
        return 0;
    }
    // Bias self to its embedded CCharVoice and play.
    CCharVoice* voicePtr = (CCharVoice*)self;
    if (self != NULL) {
        voicePtr = &self->voice;
    }
    func_802A3D54(voicePtr, 0xA8D, 0x28);
    return 0;
}
