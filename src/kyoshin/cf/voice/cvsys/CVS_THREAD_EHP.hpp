#pragma once

#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

struct CVoiceHandle;

// CVS_THREAD_EHP: Voice thread for EHP (Emergency HP recovery) sequences.
// Object size 0x48 (72 bytes). The buffer-size virtual (func_802A6818, the
// CVS_THREAD::blank1 slot) returns 0xB4 (180). The EHP vtable
// (lbl_eu_80539B2C) is assigned manually by the factory __ct__802A5ED4.
//
// Field layout (base CVS_THREAD occupies 0x00-0x1F, vtable at 0x1C):
//   0x20  field_0x20   voice handle slot 1 (owner1)
//   0x24  field_0x24   voice handle slot 2 (owner2)
//   0x28  field_0x28   third constructor parameter
//   0x2C  field_0x2c   voice handle array (3 slots)
//   0x38  field_0x38   current rotating index
//   0x3C  field_0x3c   slot count / wrap bound (index range 0..field_0x3c)
//   0x40  field_0x40   stop/target index (triggers playback virtual)
//   0x44  field_0x44   direction flag (0 = forward, nonzero = backward)
class CVS_THREAD_EHP : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0xB4;

    CVoiceHandle* field_0x20;    // 0x20: voice handle slot 1
    CVoiceHandle* field_0x24;    // 0x24: voice handle slot 2
    s32 field_0x28;              // 0x28: third constructor parameter
    CVoiceHandle* field_0x2c[3]; // 0x2C: voice handle slots
    s32 field_0x38;              // 0x38: current rotating index
    s32 field_0x3c;              // 0x3C: slot count / wrap bound
    s32 field_0x40;              // 0x40: stop/target index
    u8 field_0x44;               // 0x44: direction flag
};

// Forward declaration of a polymorphic sub-object reached through a voice
// handle (CVoiceHandle+0x04). Only the vtable pointer at offset 0 is used.
struct CVSubObj {
    void** vtable;               // 0x00: vtable pointer
};

// Voice-handle type. The actual CCharVoice is embedded at offset 0x3E9C
// within the handle allocation (0x3E9C bytes of handle data + CCharVoice).
// Code biases a handle pointer by 0x3E9C to reach the embedded CCharVoice.
struct CVoiceHandle {
    void** vtable;               // 0x00: vtable pointer
    CVSubObj* field_0x04;        // 0x04: sub-object pointer (used by func_802A6820)
    u8 _pad[0x3E9C - 0x08];      // 0x08-0x3E9B: handle data
    CCharVoice voice;            // 0x3E9C: the actual voice object
    u8 _pad_after_voice[0x3F08 - 0x3EDC];
    u32 field_0x3f08;            // manager flag tested by func_802A6958
};

// The global voice-handle list is a circular list.  The node's payload is
// stored at +8; +0 is the next node and the list's +4 is its end marker.
struct CVoiceHandleListNode {
    CVoiceHandleListNode* next;
    CVoiceHandleListNode* previous;
    CVoiceHandle* value;
};

struct CVoiceHandleList {
    CVoiceHandleListNode* head;
    CVoiceHandleListNode* end;
};

extern "C" {
// Sibling TU functions (unmangled global symbols).
int func_802A3E88(CVS_THREAD* self);
void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
CVoiceHandle* func_802A7998(CVoiceHandle* exclude);
CVoiceHandle* func_802A330C(int size, int align);
int func_80174C98(CVoiceHandle* handle, u32* value, int arg);
CVoiceHandleList* func_800B6BC8();
int func_802A7FE4(CVoiceHandle* handle);
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_80539B14[3]; // {0, -1, func_802A6408}
extern "C" u32 lbl_eu_80539B20[3]; // {0, -1, func_802A6718}
